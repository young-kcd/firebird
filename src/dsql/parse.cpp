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
 * 2004.01.16 Vlad Horsun: added support for default parameters and 
 *   EXECUTE BLOCK statement
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>

#include "gen/iberror.h"
#include "../dsql/dsql.h"
#include "../jrd/ibase.h"
#include "../jrd/flags.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/parse_proto.h"
#include "../dsql/keywords.h"
#include "../dsql/misc_func.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd.h"
#include "../jrd/err_proto.h"

static void	yyerror(const TEXT*);

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

/* Make bison allocate static stack */
#define YYINITDEPTH 2048

/* Using this option causes build problems on Win32 with bison 1.28*/
/*#define YYSTACK_USE_ALLOCA 1*/

typedef dsql_nod* YYSTYPE;
#define YYSTYPE YYSTYPE
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME[] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING[] = "";

inline SLONG trigger_type_suffix(const int slot1, const int slot2, const int slot3)
{
	return ((slot1 << 1) | (slot2 << 3) | (slot3 << 5));
}


dsql_nod* DSQL_parse;

/*static void	yyerror(const TEXT*); redeclaration.*/

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, bool* stmt_ambiguous

#include "../dsql/chars.h"

const int MAX_TOKEN_LEN = 256;

static const TEXT* lex_position();
#ifdef NOT_USED_OR_REPLACED
static bool		long_int(dsql_nod*, SLONG*);
#endif
static dsql_fld*	make_field (dsql_nod*);
static dsql_fil*	make_file();
static dsql_nod*	make_list (dsql_nod*);
static dsql_nod*	make_node (NOD_TYPE, int, ...);
static dsql_nod*	make_parameter (void);
static dsql_nod*	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(dsql_nod*, SLONG*, SSHORT);
#endif
static void	stack_nodes (dsql_nod*, DsqlNodStack&);
inline static int	yylex (USHORT, USHORT, USHORT, bool*);
static void	yyabandon (SSHORT, ISC_STATUS);

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
	dsql_fld* g_field;
	dsql_fil* g_file;
	dsql_nod* g_field_name;
	bool log_defined, cache_defined;
	int dsql_debug;
	
	/* Actual lexer state begins from here */
	const TEXT* beginning;
	const TEXT* ptr;
	const TEXT* end;
	const TEXT* last_token;
	const TEXT* line_start;
	const TEXT* last_token_bk;
	const TEXT* line_start_bk;
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
		bool* stmt_ambiguous);
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
#define BACKUP 516
#define KW_DIFFERENCE 517
#define OPEN 518
#define CLOSE 519
#define FETCH 520
#define ROWS 521
#define BLOCK 522
#define YYERRCODE 256
static short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    9,    9,    9,    9,    9,   23,   23,   22,
   22,   22,   33,   33,   26,   34,   34,   34,   34,   34,
   25,   25,   31,   31,   27,   15,   15,   15,   15,   15,
   15,   15,   37,   28,   28,   28,   28,   38,   38,   38,
   38,   24,   24,   39,   39,   39,   29,   29,   44,   30,
   30,   45,   45,    6,   46,   46,   48,   53,   53,   53,
   50,   50,   50,   57,   57,   58,   58,   51,   51,   60,
   60,   60,   60,   60,   47,    5,   63,   63,   63,   63,
   63,   63,   63,   63,   63,   63,   63,   13,   78,   78,
   78,   14,   82,   82,   65,   65,   68,   68,   68,   76,
   91,   91,   91,   92,   92,   93,   93,   94,   94,   98,
   98,   75,  101,  101,  103,  103,  104,  104,  107,  107,
  108,  109,  109,  110,  111,   73,   77,   74,   95,   95,
  114,  115,  115,  117,  117,  118,  118,  118,  118,  118,
  116,  116,  119,  119,  120,  120,  120,   99,  122,  123,
  123,  124,  124,  125,  125,  126,  126,  126,   97,   97,
   97,   70,   80,  127,  127,  127,  128,  128,  129,  129,
  130,  130,  130,  136,   87,   87,  132,  132,  105,  105,
  100,  140,  141,   59,  133,  133,  106,  106,  106,  106,
  106,  106,  134,  134,  148,  148,  149,  151,  151,  151,
  151,  151,  131,  150,  150,  155,  155,  155,  155,  157,
  158,  159,  153,  153,  154,  152,  152,  152,  152,  152,
  160,  161,  162,  162,  162,  162,   69,   79,   83,  167,
  163,  163,  164,  164,  168,  168,  170,  169,  169,  172,
  171,  171,  171,  165,  165,  173,  173,  174,  176,  176,
  177,  175,  175,  179,  179,  178,  182,  182,  166,  184,
  184,  184,  185,  185,  183,  183,  183,  183,  183,  183,
  183,  183,  183,  183,  183,  183,  183,  183,  183,  183,
  183,  183,  183,  188,  188,  189,  190,  191,  192,  197,
  198,  193,  193,  194,  206,  195,  207,  201,  201,  201,
  202,  202,  202,  204,  204,  204,  204,  196,  203,  203,
  199,  199,  199,  205,  205,  186,  186,  212,  213,  213,
  214,  214,  214,  214,  200,  200,  200,  216,  218,  217,
  219,   11,   12,  220,  220,  221,  221,  222,   72,   81,
  224,  227,  227,  227,  228,  231,  235,  235,  236,  236,
  237,  237,  138,  226,   88,   90,  225,  225,   71,   84,
  240,  240,  240,  241,  244,  244,  245,  245,  245,  245,
  245,  245,  245,  245,  245,  245,  245,  245,  245,  245,
  245,  242,  242,  243,    2,  247,  247,  247,  247,  247,
  247,  247,  255,  256,  253,  253,  257,  257,  257,  257,
  257,  257,  257,  248,  248,  258,  258,  258,  258,  258,
  258,  258,  252,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  260,  260,  262,  262,  261,  259,  259,  259,  254,  254,
  250,  251,  251,  265,  265,  265,  265,  265,  249,  266,
  266,  267,  267,    8,  268,  268,  268,  268,  268,  268,
  268,  268,  268,  268,  268,  268,  102,  102,  135,  135,
  271,  271,  274,  274,  276,  276,   54,   54,  273,  273,
  273,  273,  273,  273,  273,  273,  273,  281,  281,  272,
  272,  272,  272,  282,  282,   62,   62,   62,   56,   56,
  278,  278,  278,  275,  275,  275,  287,  287,  287,  286,
  286,  285,  285,  285,  279,  279,  288,  288,  288,  289,
  289,  280,  280,  280,  280,  290,  290,   19,   19,   19,
  292,  292,  292,   17,   17,   17,  294,  295,  298,  298,
  296,  300,  300,    4,   16,  299,  299,  301,  301,  302,
  302,  291,  303,  303,  304,  304,  305,  305,  305,  305,
  306,  306,  307,  307,  308,  308,  310,  310,  310,  311,
  311,  311,  312,  312,  312,  309,  314,  314,  314,  315,
  315,  313,  313,  316,  318,  318,  317,  317,  293,   18,
  319,  319,  319,  320,  320,  325,  325,  326,   66,   66,
   66,  328,  328,  327,  327,  321,  321,  321,  322,  322,
  331,  331,  323,  323,  324,  181,  335,  336,  329,  330,
  332,  332,  332,  332,  337,  337,  337,  338,  338,  338,
  229,  229,  230,  230,  340,  340,  341,  341,  342,  342,
  333,  344,  344,  345,  345,  345,  348,  349,  349,  350,
  350,  352,  352,  346,  346,  347,  347,  353,  353,  238,
  238,   32,  239,  239,  239,  239,  239,  239,  239,  239,
  233,  233,  354,  354,  355,  234,  234,  232,  232,  334,
  334,  356,  357,  357,  357,  357,  357,  358,  358,  359,
  359,  360,  360,  361,  361,  361,  362,  362,  363,  363,
   10,   10,    7,    7,  365,  366,  367,   20,   20,  368,
  369,  370,  370,  187,    3,    3,  372,  372,  372,  374,
  374,   62,  373,  373,  375,  375,   35,   35,   86,   85,
   85,  364,  364,  376,  377,  377,  210,  210,  210,  137,
  371,  371,  112,  112,  112,  112,  112,  381,  381,  381,
  381,  381,  381,  380,  380,  380,  383,  383,  383,  383,
  383,  383,  383,  383,  383,  383,  384,  384,  384,  384,
  384,  384,  384,  384,  389,  389,  389,  389,  389,  389,
  389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
  395,  395,  385,  385,  386,  386,  386,  386,  387,  387,
  391,  391,  392,  392,  392,  392,  390,  393,  388,  388,
  379,  379,  379,  382,  382,  382,  396,  396,  397,  394,
  398,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,  147,  147,
  147,  399,  209,  209,  142,  142,  405,  405,  405,  405,
  405,  401,  401,  401,  401,  401,  223,  143,  143,  144,
  145,  145,  145,  145,  145,   52,   52,   21,   21,  246,
  406,   55,  283,  277,  277,   96,  400,  400,  400,  400,
  407,  407,  407,  407,  407,  407,  407,  407,  407,  407,
  407,  408,  409,  411,  410,  410,  413,  414,  414,  404,
  404,  402,  403,  403,  415,  415,  416,  416,  417,  417,
  420,  420,  418,  418,  423,  423,  422,  419,  421,  412,
  412,  412,  412,  412,  412,  412,  412,  339,  339,  146,
   49,  284,  121,  139,  378,  156,  180,  270,   64,   61,
  215,  113,   67,  343,  211,   36,   42,  351,  269,   40,
   43,  208,   41,  297,  264,  264,  424,  424,  424,  424,
  424,  424,  424,  424,  424,  424,  424,  424,  424,  424,
  424,  424,  424,
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
    0,    1,    1,    2,    1,    4,    3,    3,    1,    0,
    1,    1,    2,    3,    4,    0,    1,    2,    0,    1,
    1,    5,    5,    3,    2,    0,    1,    3,    1,    1,
    6,    3,    2,    6,    2,    1,    2,    2,    2,    0,
    1,    1,    2,    0,    2,    0,    1,    1,    1,    1,
    1,    1,    0,    1,    1,    2,    2,    2,    5,    1,
    2,    3,    2,    2,    0,    1,    1,    1,    1,    3,
    4,    8,    4,    0,    6,    1,    1,    2,    2,    0,
    3,    3,    1,    2,    2,    2,    8,    8,    8,    8,
    3,    0,    4,    0,    1,    3,    4,    1,    3,    2,
    3,    3,    1,    1,    0,    1,    2,    4,    1,    1,
    3,    1,    0,    2,    1,    6,    1,    1,    3,    1,
    2,    0,    1,    2,    2,    2,    1,    1,    1,    1,
    1,    1,    2,    1,    1,    2,    1,    1,    1,    2,
    2,    1,    2,    3,    4,    2,    6,    4,    8,    9,
    6,    8,    6,    4,    0,    4,    2,    1,    3,    0,
    2,    4,    0,    1,    1,    3,    3,    7,    2,    0,
    2,    2,    3,    3,    0,    1,    2,    4,    1,    3,
    2,    2,    2,    1,    1,    1,    1,    2,    2,    5,
    0,    4,    7,    3,    0,    1,    3,    3,    7,    7,
    1,    1,    3,    4,    7,    2,    1,    3,    1,    1,
    6,    3,    0,    0,    0,    0,    3,    0,    9,    9,
    1,    1,    0,    2,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    3,    3,    5,    5,    5,    5,    5,
    5,    2,    0,    4,    2,    3,    3,    2,    2,    3,
    3,    2,    3,    6,    1,    2,    4,    3,    2,    2,
    2,    2,    3,    1,    3,    3,    3,    2,    2,    4,
    4,    5,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    2,    1,    1,    1,    0,    2,    2,
    0,    1,    2,    2,    4,    3,    2,    2,    7,    1,
    0,    1,    0,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    3,    2,    2,    2,    1,    1,    1,    1,
    4,    5,    1,    3,    1,    3,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    4,
    4,    6,    5,    3,    0,    2,    2,    0,    3,    0,
    4,    1,    5,    4,    1,    4,    1,    2,    2,    1,
    1,    1,    2,    2,    2,    2,    0,    3,    5,    1,
    1,    2,    3,    1,    2,    3,    0,    1,    1,    1,
    5,    5,    6,    1,    1,    1,    2,    4,    1,    0,
    5,    1,    0,    3,    2,    1,    0,    2,    0,    1,
    0,    3,    1,    0,    1,    2,    1,    1,    1,    1,
    2,    2,    1,    2,    3,    1,    1,    3,    3,    1,
    2,    3,    1,    2,    0,    2,    1,    1,    0,    1,
    1,    1,    3,    2,    3,    0,    1,    3,    4,    5,
    1,    3,    4,    3,    0,    1,    3,    3,    1,    1,
    0,    1,    1,    4,    0,    2,    4,    0,    3,    0,
    2,    0,    2,    0,    9,   11,    0,    0,    0,    0,
    3,    2,    1,    0,    3,    5,    3,    2,    5,    2,
    1,    1,    1,    1,    1,    3,    1,    3,    1,    0,
    2,    1,    3,    1,    1,    1,    6,    1,    0,    3,
    0,    1,    3,    6,    3,    4,    2,    3,    0,    1,
    2,    1,    1,    1,    2,    1,    2,    1,    2,    0,
    3,    0,    1,    3,    1,    2,    0,    2,    0,    2,
    0,    4,    1,    2,    1,    1,    0,    1,    3,    2,
    1,    1,    2,    1,    4,    3,    1,    3,    4,    0,
    8,    5,    1,    1,    4,    4,    4,    1,    1,    5,
    5,    1,    3,    3,    7,    7,    5,    3,    0,    1,
    1,    1,    2,    0,    1,    1,    1,    0,    3,    1,
    3,    1,    0,    3,    1,    3,    1,    3,    3,    1,
    1,    3,    1,    2,    1,    3,    3,    1,    2,    3,
    3,    3,    3,    1,    3,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    3,    3,    3,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    1,    1,    5,    6,    3,    4,    5,    6,    3,    4,
    3,    4,    3,    4,    4,    5,    4,    4,    3,    4,
    1,    1,    1,    1,    1,    1,    1,    3,    3,   11,
   11,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    3,    3,    3,    3,    3,    3,    3,    3,
    1,    1,    1,    1,    3,    1,    1,    1,    1,    1,
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
    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
  558,  559,  560,  564,  565,  566,    0,  621,  733,  734,
  738,  739,  471,    0,    0,    0,    0,    0,    0,  405,
  576,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,    0,    0,   96,    0,  910,    0,   22,  908,
    0,    0,   74,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,    0,    0,    0,
   38,   35,   37,    0,   36,    0,  995,  997,  999,  998,
 1004, 1000, 1007, 1005, 1001, 1003, 1006, 1002, 1008, 1009,
 1010, 1012, 1013, 1011,    0,    0,    0,    0,   33,   69,
   67,  987,  996,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  653,    0,    0,    0,
  690,    0,  989,    0,    0,    0,    0,  108,  994,  567,
    0,    2,    0,    0,    0,    0,  435,  436,  437,  438,
  439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
  459,  460,    0,  433,  434,    0,  979,    0,  412,  983,
    0,  409,  986,    0,  692,    0,  408,  990,    0,  574,
  906,    0,  151,  104,    0,  105,    0,  201,  975,  770,
    0,  103,  146,  982,    0,    0,   99,    0,  106,  912,
    0,  100,    0,  101,    0,  102,  993,  147,  107,  629,
  630,    0,  911,  909,    0,   75,    0,  980,    0,  978,
  492,  485,    0,  491,  496,  486,  487,  494,  488,  489,
  490,  495,    0,    0,    0,   31,    0,   40,  757,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  916,    0,  897,  647,  647,
  648,  658,  660,  968,  661,    0,  662,  652,  648,    0,
    0,    0,    0,    0,    0,    0,  593,  582,    0,  585,
  587,  588,  589,  590,  596,  597,    0,  988,  691,    0,
  109,    0,  110,    0,  111,    0,    0,    0,  622,    0,
    0,    0,    0,    0,    0,    0,  472,    0,    0,  375,
    0,  204,    0,  415,  406,  469,  470,    0,    0,    0,
    0,    0,    0,  424,    0,  381,  382,    0,  580,  578,
  907,    0,    0,    0,    0,    0,    0,    0,  154,  133,
    0,   97,    0,    0,  112,    0,  122,  121,    0,    0,
    0,    0,    0,    0,  971,   76,    0,    0,    0,  735,
  736,  493,    0,    0,    0,    0,  874,  903,    0,  970,
    0,  876,    0,    0,  904,    0,  898,  889,  888,  887,
    0,    0,    0,    0,  879,  880,  881,  890,  891,  899,
  900,    0,  901,  902,    0,    0,    0,  905,    0,    0,
    0,    0,    0,  893,    0,  767,  871,  872,  873,  878,
  877,  352,  857,    0,    0,  856,    0,    0,  853,  854,
  855,  858,  859,  860,  892,  917,  918,  919,  920,  933,
  935,  943,  944,  947,  948,  202,    0,    0,    0,  356,
    0,    0,    0,  760,    0,    0,   28,    0,    0,    0,
   72,   70,  991,   68,   34,    0,    0,    0,  762,    0,
   53,    0,    0,    0,    0,    0,  572,    0,    0,    0,
  655,  657,    0,  664,    0,    0,    0,  665,  651,    0,
  619,    0,  594,    0,  591,    0,  592,  617,    0,  612,
    0,    0,  586,  771,  742,    0,    0,    0,    0,    0,
    0,  569,  568,    0,    0,  626,  623,    0,    0,    0,
  169,    0,    0,  130,    0,  477,    0,  478,  473,    0,
    0,  419,  421,  420,    0,  422,    0,  416,    0,    0,
  255,    0,    0,    0,  428,  429,    0,  462,    0,    0,
    0,    0,    0,    0,    0,  386,  385,  480,    0,    0,
  150,    0,    0,  159,    0,  158,    0,    0,  165,  148,
    0,  163,  155,    0,    0,  515,  550,  518,    0,  551,
    0,    0,  519,    0,    0,    0,  542,  554,  514,  537,
  516,  517,  512,  499,  375,  497,  498,  500,    0,    0,
  509,  510,  511,  513,    0,    0,    0,    0,    0,  113,
    0,  114,    0,  125,    0,    0,  185,    0,    0,  373,
    0,  204,    0,    0,   84,    0,    0,  752,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  773,
  775,  784,  787,  788,  789,  790,  791,  792,  793,  794,
  795,  796,    0,    0,    0,  894,    0,    0,    0,    0,
    0,    0,  895,  896,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  317,  992,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  260,
  507,    0,    0,    0,  354,    0,    0,    0,  759,   29,
    0,    0,   45,   73,    0,    0,   27,    0,  765,    0,
    0,    0,  732,    0,    0,    0,    0,    0,  571,    0,
  647,    0,  669,    0,    0,    0,    0,  562,    0,  914,
  561,    0,  595,    0,  603,  599,  598,    0,    0,    0,
  614,  602,    0,    0,  740,  741,    0,    0,    0,  373,
    0,    0,    0,    0,    0,  620,    0,  131,    0,  476,
  375,  418,    0,  375,  376,  423,    0,    0,  251,  373,
  224,  976,    0,    0,  375,    0,    0,  193,  373,    0,
    0,    0,    0,  229,  223,  226,  227,  228,  427,  467,
  466,  426,  425,    0,    0,    0,    0,    0,    0,  384,
    0,  375,    0,  156,  160,    0,    0,  164,    0,    0,
  538,  539,  555,    0,  552,    0,    0,  545,  543,  544,
    0,    0,    0,    0,  508,    0,    0,    0,    0,  546,
    0,    0,  373,    0,  184,    0,  187,  189,  190,    0,
    0,    0,    0,    0,  204,   79,    0,    0,   78,  526,
  972,  527,    0,    0,    0,    0,  774,  786,    0,    0,
  844,  845,  846,    0,  778,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  960,  961,
  962,  963,  964,  965,  966,  967,    0,    0,    0,    0,
    0,    0,  953,    0,    0,    0,    0,  869,  319,  870,
  941,    0,  865,  974,    0,    0,    0,    0,    0,    0,
    0,  875,  769,  768,  358,  357,    0,  258,    0,    0,
    0,  266,  761,    0,    0,    0,    0,    0,    0,    0,
    0,   54,   62,   64,    0,    0,   71,    0,    0,    0,
  764,    0,    0,    0,    0,    0,    0,    0,    0,  656,
  659,  984,  668,    0,    0,    0,    0,  674,  675,  676,
    0,    0,  666,  563,  915,  604,  613,  608,  607,    0,
  618,  772,  743,    0,  373,    0,    0,  649,  628,  627,
    0,    0,  639,  643,  475,    0,    0,  168,    0,  172,
    0,    0,    0,  417,  373,  373,  263,  376,  256,    0,
  195,  375,  197,    0,  376,  192,  198,    0,    0,    0,
    0,  430,  431,  375,  373,  376,    0,    0,    0,  402,
    0,  180,  181,  157,    0,  167,  913,    0,    0,    0,
    0,    0,  553,    0,    0,  375,  376,    0,  503,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  182,    0,    0,    0,    0,  362,    0,   83,
    0,    0,    0,    0,   88,   85,    0,    0,    0,  737,
  977,    0,    0,  779,    0,    0,  785,    0,    0,    0,
    0,    0,  822,  821,    0,    0,    0,  829,  847,  839,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  777,    0,    0,    0,  204,    0,  921,    0,    0,  936,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  949,    0,    0,    0,    0,    0,    0,    0,
  940,  882,    0,  253,  272,    0,    0,  353,  267,   66,
   58,   59,   65,   60,   61,    0,    0,   23,    0,    0,
    0,   43,    0,    0,    0,  746,  766,    0,    0,  745,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  693,    0,    0,    0,    0,    0,  610,  611,  615,
    0,  183,    0,    0,    0,    0,    0,    0,  173,    0,
    0,    0,  413,  207,  208,  209,  210,  211,  212,  885,
    0,    0,  257,    0,    0,  205,    0,    0,    0,    0,
  230,    0,  463,  464,  432,    0,    0,    0,    0,    0,
    0,  375,  482,  376,  166,  973,    0,    0,  521,    0,
  520,  556,    0,  548,    0,    0,    0,  501,    0,  529,
    0,    0,  531,  534,  536,  373,    0,    0,    0,  120,
    0,  188,  375,    0,    0,  374,    0,   98,    0,  118,
    0,   94,    0,    0,    0,    0,   87,    0,  837,  838,
  781,    0,  783,    0,    0,    0,    0,    0,    0,    0,
  840,    0,    0,    0,    0,    0,    0,    0,  830,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  927,  926,    0,    0,  923,  922,    0,
  931,  930,  929,  928,  925,  924,    0,    0,  955,    0,
    0,    0,  954,    0,    0,    0,    0,  259,    0,    0,
  269,  270,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  351,    0,    0,    0,    0,
  278,  283,  277,    0,    0,    0,  287,  288,  289,  290,
  291,  292,  294,  295,  297,  298,  299,  302,    0,    0,
    0,  345,  346,  347,    0,    0,   57,   63,   55,   56,
   24,  750,  751,    0,  748,  756,  755,  753,    0,  731,
    0,    0,    0,  685,    0,    0,    0,  699,  695,  697,
    0,    0,    0,    0,    0,  374,  632,  633,  650,    0,
  178,  174,    0,  376,  886,  261,  262,  376,    0,    0,
    0,  215,    0,    0,  231,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  479,  523,    0,  524,    0,  135,
    0,    0,    0,  139,  141,  142,  143,  504,  506,  502,
  533,    0,    0,  376,    0,    0,    0,    0,  359,    0,
  363,  375,   89,    0,    0,   91,    0,    0,    0,    0,
    0,    0,  848,  849,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  203,  942,  932,  934,    0,    0,  951,  950,
    0,  956,  945,  946,    0,    0,  268,    0,  306,    0,
    0,    0,  301,    0,    0,  300,  331,  332,    0,  985,
  348,  349,    0,  286,  293,    0,  296,  279,    0,  284,
    0,  336,  285,  303,    0,    0,  329,   41,    0,    0,
    0,  688,  686,    0,    0,    0,  703,    0,    0,  645,
  376,  360,  634,  175,  376,  145,  250,    0,    0,  191,
  216,    0,    0,    0,    0,  217,  220,    0,    0,  376,
  400,  401,  397,  396,  399,  398,    0,  522,  549,  144,
  132,  140,    0,  375,  247,  127,  376,    0,    0,  377,
  364,    0,    0,   92,   93,   80,    0,  808,  816,    0,
  809,  817,  812,  820,    0,    0,  810,  818,  811,  819,
  805,  813,  806,  814,  807,  815,    0,  937,  952,    0,
    0,  275,  271,    0,  304,    0,    0,    0,    0,    0,
  333,    0,    0,  324,  325,    0,  344,    0,    0,    0,
    0,  339,  337,    0,    0,    0,  747,    0,    0,  678,
    0,    0,  713,  715,    0,  710,    0,  248,  414,  194,
  199,  218,    0,    0,  221,    0,  233,  235,  404,  376,
    0,  379,    0,    0,    0,  369,  370,    0,    0,   77,
   95,    0,    0,  274,    0,  305,    0,    0,  308,    0,
  314,    0,  316,    0,    0,  343,  342,  981,  341,    0,
    0,    0,    0,    0,    0,    0,  677,    0,  704,  714,
    0,  222,    0,    0,    0,    0,    0,  249,  376,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  327,  326,  338,  340,    0,    0,    0,
    0,  682,    0,    0,  721,    0,    0,    0,    0,    0,
    0,  232,    0,  239,    0,  238,  380,  372,    0,    0,
  365,  119,    0,    0,  276,    0,    0,  307,  311,    0,
    0,    0,    0,    0,    0,    0,  680,  723,  712,    0,
    0,  724,    0,  720,  219,    0,    0,  243,  242,  241,
    0,    0,    0,    0,    0,    0,    0,    0,  328,    0,
  683,  719,    0,    0,  246,  244,  245,    0,    0,  851,
  322,  312,    0,  334,  309,  646,    0,    0,    0,  726,
    0,    0,  310,    0,  725,    0,    0,  728,    0,  850,
  729,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1407,   28,   29, 1408,
   31,   32,   33,   34,   35,   36,   37, 1409,   39, 1410,
  648,  125,  476,  970, 1208,  126,  722,  971,  127,  480,
  727,  151,  128,  129,  268,  995,  141,  972,  973,  206,
  235,  130,  974,  131,  482,   83,  246,  386,  433,  643,
 1103,  434,  868,  614,  228,  845,  644,  645,  646, 1105,
  247, 1442,   75,  196,   76,  242, 1877, 1318,  227,  232,
  234,  236,  222,  214,  216,  229,  239,  158,  321,  323,
  325,  375,  630,  632,  473,  269,  794,  551,  658, 1034,
  379,  635, 1091, 1310,  582,  750, 1064,  543,  544,  564,
  371,  615, 1077, 1502, 1620, 1253, 1503, 1504, 1505, 1506,
 1507,  659,  223,  215,  367,  590,  368,  369,  591,  592,
 1285,  545, 1028, 1029, 1030, 1248,  381,  856,  857,  858,
  859,  796, 1045, 1480,  616,  798,  436,  861,  943,  467,
 1367, 1254,  437,  438,  439,  440,  441, 1481, 1482,  567,
 1626, 1775, 1271,  804,  805,  791,  806,  807,  808, 1776,
 1777, 1849,  349,  472,  960, 1411,  202,  560,  957,  561,
 1038,  468,  961,  962, 1196, 1390, 1391, 1392, 1683, 1110,
  733, 1412, 1413, 1414, 1415, 1591, 1416, 1417, 1418, 1419,
 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 1428, 1429,
  442, 1790, 1430, 1693, 1833, 1690,  443,  698,  444,  445,
 1431, 1592, 1701, 1702, 1757, 1432, 1433, 1434, 1583,  265,
  469,  470,  446, 1096, 1316, 1519, 1097, 1098,  296,  506,
 1649,  390, 1237, 1464, 1734, 1735, 1736, 1737, 1235,  358,
  578,  822, 1283,  579,  820,   80,   60,  353,  207,  166,
  336,  193,  343,  199,  785,  782,  344,  354,  812,  355,
  574, 1056,  194,  447,  337,  580, 1284,   97,  205,  251,
  617,  618,  711, 1078,  712, 1079, 1080,  621,  622,  623,
  624, 1071, 1069,  872,  625,  626,  627,  838,  628,  835,
   41,   42,   43,   44,   45,   46,  160,  533,   62,  498,
  210,  360,  308,  309,  310,  311,  312,  313,  314,  315,
  316,  756,  519, 1010, 1240,  520,  521,  761,   47,  165,
  331,  540,  776,   48,  535,  536, 1019, 1469, 1244, 1613,
 1023,  145,  746, 1610,  501,  298,  146,  147,  297,  507,
  508,  744,  993,  996,  997,  998,  999, 1000, 1709, 1767,
  448, 1803, 1229, 1606, 1607, 1805, 1717, 1806, 1807, 1808,
 1844, 1878, 1880,  488,   49,   50,  391,   51,   52,  527,
  528,  979, 1216, 1444, 1448,  489,  730,  220,  660,  661,
  886,  887,  662,  663,  664,  665,  666,  667,  668,  669,
  670,  671,  672, 1340, 1126, 1128, 1129,  697,  449,  450,
  451,  452,  453,  454,  455,  244,  456,  457,  458,  459,
  460,  927,  461, 1558,  462,  463,  464,  465,  688,  931,
 1186, 1181,  689,  133,
};
static short yysindex[] = {                                   7539,
 1374,  -71, 3587,   65, 1032,  393, 3271, -121, 7998,  540,
  446, 9828,  -71, 1079,  591, 7583,  563, 7583,  350,    0,
  943,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  382,    0,    0,    0,
    0,    0,    0,12843, 7583, 7583, 7583, 7583, 7583,    0,
    0,  644, 1406, 7583, 7583, 7583,  761, 7583,  617, 7583,
 7583,    0, 7583, 7583,    0, 1389,    0,  711,    0,    0,
 1014, 7583,    0, 7583, 7583, 7583, 1077, 7583, 7583, 7583,
 7583,  617, 7583, 7583, 7583, 7583,    0, 7583, 1225,  896,
    0,    0,    0, 1372,    0, 1372,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1050, 1105,  165, 1380,    0,    0,
    0,    0,    0, 7583, 7583, 7583, 1125, 1226, 1291,  162,
 1967, 1026,  216,  281,  875, 1176,    0, 7583, 1346, 1461,
    0, 1301,    0, 7583, 7583, 7583, 7583,    0,    0,    0,
 7583,    0, 1435,  -81, 1222, 1585,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1115,    0,    0, 1406,    0,   54,    0,    0,
 1685,    0,    0, 1321,    0,  513,    0,    0, 1315,    0,
    0, 1288,    0,    0, 1012,    0, 1492,    0,    0,    0,
 1406,    0,    0,    0,  462, 1685,    0,   44,    0,    0,
 1454,    0, 1468,    0, 1372,    0,    0,    0,    0,    0,
    0, 1443,    0,    0, 1341,    0, 1522,    0, 1376,    0,
    0,    0, 1341,    0,    0,    0,    0,    0,    0,    0,
    0,    0,11864, 7583, 1441,    0, 7583,    0,    0,    0,
 9181, 1470, 9510, 7583, 1204, 1523, 1834, 1556, 1565, 9181,
 1487, 9510, 1511, 1515, 1403,    0,11944,    0,    0,    0,
    0,    0,    0,    0,    0,10175,    0,    0,    0, 1478,
 7583, 1570, 1466, 1009, 7583, 1509,    0,    0, 1461,    0,
    0,    0,    0,    0,    0,    0, 7583,    0,    0, 1685,
    0, 1454,    0, 1372,    0, 1581,11944, 1572,    0,11944,
 1652,  -17, 1477, 1488, 1501, 1585,    0, 1753,  699,    0,
 7583,    0, 1115,    0,    0,    0,    0, 7583, 1441, 4107,
 1552, 5520, 1998,    0,12843,    0,    0, 1059,    0,    0,
    0, 1987, 1987, 1406, 1680, 1406,  703, 1012,    0,    0,
 7616,    0, 7583, 7583,    0, 1441,    0,    0, 1767,  482,
 2018, 7583, 1795, 7583,    0,    0, 2021,  238, 7200,    0,
    0,    0, 2026, 2027, 2028, 1608,    0,    0, 2037,    0,
 2040,    0, 2047, 2052,    0, 2056,    0,    0,    0,    0,
    0, 2058, 1638, 1657,    0,    0,    0,    0,    0,    0,
    0, 2072,    0,    0,10261, 2073, 2080,    0,11944,11944,
10491, 7583, 2088,    0, 2979,    0,    0,    0,    0,    0,
    0,    0,    0, 2089, 2051,    0,    0, 2097,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 7616, 2085,  309,    0,
 2108, 1886, 1733,    0, 7583, 1724,    0, 7583, 7583,  115,
    0,    0,    0,    0,    0, 7583, 7583,  678,    0, 7583,
    0, 1842, 7583, 2119, 9181, 1762,    0, 7583,10491, 1671,
    0,    0,11944,    0, 2634, 1845, 2124,    0,    0,   66,
    0,  829,    0,  498,    0,  498,    0,    0, 2125,    0,
   94, 1746,    0,    0,    0, 2138,   63, 2121, 1441, 2145,
 1924,    0,    0, 2205, 2149,    0,    0, 1087, 1747, 1741,
    0, 1848, 1855,    0, 1406,    0, 1857,    0,    0, 1932,
 1936,    0,    0,    0, 1925,    0, 7616,    0, 7616, 1734,
    0, 1958, 7583,12761,    0,    0,  704,    0, 7583,  681,
 1321, 1827, 1792, 1760,    0,    0,    0,    0,  -12, 1835,
    0, 1775,  617,    0, 1406,    0, 1957, 1883,    0,    0,
  703,    0,    0,  209, 1793,    0,    0,    0, 1796,    0,
 1860, 2204,    0, 1909, 2226,   14,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2169,   92,
    0,    0,    0,    0,   48, 2236, 2241, 2226, 1685,    0,
 1964,    0, 2020,    0, 1406, 1406,    0, 4107,  513,    0,
 1902,    0, 1875, 2243,    0, 8772, 3577,    0, 1901, 1907,
 2257, 7286, 2260,    0,    0,    0, 6946, 7244,   85,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  900,11944,  188,    0, 7583,11944, 1153, 1219,
 1304, 2217,    0,    0,11944, 7516, 2979, 1850,  -91,11944,
11944, 1067, 1067, 1079, 1837, 1743, 2265,    0,    0,10748,
 7583,11944,11944,11944,11944,11944,11944,11944, 3049,    0,
    0, 2030, 2245, 7583,    0, 7583, 2010, 7583,    0,    0,
10567, 1885,    0,    0, 1853, 9510,    0, 2005,    0, 1752,
 1079, 2283,    0, 2005,10567, 2004, 2006, 7583,    0, 1837,
    0, 2059,    0, 7583, 2136, 1873,11944,    0,  771,    0,
    0,   51,    0, 1879,    0,    0,    0, 7583, 1314, 7583,
    0,    0, 7583, 7583,    0,    0,11944, 2065, 4107,    0,
 1828,11944,11944, 1949, 1831,    0, 1406,    0,  525,    0,
    0,    0, 2293,    0,    0,    0,  319, 7583,    0,    0,
    0,    0, 2060, 2297,    0, 2038, 2057,    0,    0, 1996,
 1999, 1372, 2067,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  617, 7583,12935, 1960, 1961, 1966,    0,
  617,    0, 1516,    0,    0, 1937, 1406,    0,   46, 1934,
    0,    0,    0,  617,    0, 2204,  137,    0,    0,    0,
 2054,  137, 1938,  137,    0, 2317,  617,  617,  617,    0,
 1441, 7583,    0, 2007,    0, 1803,    0,    0,    0, 1059,
 1948, 7583, 1821,  242,    0,    0, 2326, 2098,    0,    0,
    0,    0,  238, 7583, 1965, 7764,    0,    0, 1965, 7286,
    0,    0,    0, 5842,    0,  426,  400,11944,11944, 7864,
 2337, 1082, 8125,11944, 8383, 1264, 8481, 8739,10828, 8997,
 9095, 9353, 7516, 7516,11944,11944, 2702,11944, 2344,11944,
 2342, 2093,11944,11944,11944,11944,11944,11944,    0,    0,
    0,    0,    0,    0,    0,    0, 2074, 1705,  -61,11944,
  971,11944,    0, 7516, 2389, 2587,  875,    0,    0,    0,
    0, 1881,    0,    0, 2110, 1067, 1067, 1396, 1396, 2979,
 1038,    0,    0,    0,    0,    0, 1891,    0, 1947, 2126,
 2010,    0,    0, 7583, 7583, 7583, 7583, 7583, 7583,  161,
 2351,    0,    0,    0,10567, 2011,    0,  -53, 2036, 7583,
    0,  875,11944, 2036, 2360, 2351,10567,10656, 2087,    0,
    0,    0,    0, 2039, 2368, 2367, 1575,    0,    0,    0,
 7516, 2084,    0,    0,    0,    0,    0,    0,    0,  421,
    0,    0,    0, 2979,    0, 1900, 1948,    0,    0,    0,
 2979, 7583,    0,    0,    0, 1987, 2150,    0,  525,    0,
 2375, 7516, 6852,    0,    0,    0,    0,    0,    0, 2010,
    0,    0,    0, 6852,    0,    0,    0, 1372, 1372, 1919,
 2382,    0,    0,    0,    0,    0,   23,  369, 1044,    0,
 2160,    0,    0,    0, 7583,    0,    0,   65, 1916, 2003,
 2030, 2385,    0, 1775, 1922,    0,    0, 1081,    0, 2369,
 7583, 1098,  617, 2397, 2398, 2399, 2184,  513, 2010, 1987,
 1855, 4107,    0, 1835,  875, 1997, 2012,    0, 2105,    0,
  617, 2061, 2141, 8772,    0,    0,  617, 1971, 2155,    0,
    0, 2426, 2427,    0, 7516, 7516,    0, 7516, 7516, 3019,
 2979, 2429,    0,    0, 2979, 2436,11067,    0,    0,    0,
 2127, 2438, 2979, 2440, 1688, 2441, 2979, 2442,11944,11944,
 2337,11944,11306, 2446, 2979, 2447, 2448, 2979, 2452,11944,
 2979, 2455, 2979, 2456, 2457, 2979, 2461, 2462, 2979, 2463,
    0, 2242, 2220, 2232,    0, 2273,    0, 2376,11944,    0,
 2430, 2796, 2814, 2823, 2836, 2912,11944,11944,11944, 2979,
 2078,11944,    0,11944, 2979, 2198,  469,11944,11944,11944,
    0,    0, 7583,    0,    0, 7583,11526,    0,    0,    0,
    0,    0,    0,    0,    0, 2181,10567,    0,10567,  161,
 2351,    0,  431,  431,  149,    0,    0,10175, 1923,    0,
 2360, 2351, 2465,10656, 2474, 1575, 2475,11944, 2254, 2136,
 2131,    0, 2133, 2139, 2177, 2252, 2201,    0,    0,    0,
 2010,    0, 1997,  975, 2489, 1775, 2143, 1775,    0, 7516,
  677, 1668,    0,    0,    0,    0,    0,    0,    0,    0,
 6852, 6852,    0, 2126,11944,    0, 2249, 2130, 1919, 1389,
    0, 7516,    0,    0,    0, 2159, 2166, 2168, 2180, 2182,
 2188,    0,    0,    0,    0,    0, 2502,   65,    0, 2109,
    0,    0,  137,    0, 6852, 2185,  137,    0,  137,    0,
 2030, 2505,    0,    0,    0,    0, 1059, 2126, 1775,    0,
 1855,    0,    0,10175, 2299,    0,   45,    0, 2489,    0,
 2543,    0, 2544, 1406,   53, 2547,    0, 1406,    0,    0,
    0, 2242,    0, 2242,11944, 2167, 2167, 1079, 1930, 2548,
    0, 2167, 2167,11944, 2167, 2167, 3293, 2979,    0, 1808,
11944, 2979, 2167, 2167, 2167, 2167, 2979, 2167, 2167, 2167,
 2167, 2167, 2167,    0,    0, 7616, 2549,    0,    0, 2920,
    0,    0,    0,    0,    0,    0, 2943, 1610,    0,11944,
 2285, 2164,    0,11944, 2957, 1943, 1721,    0, 7616, 2541,
    0,    0, 2308,    0, 1573,  258, 2550, 2562, 2263,11944,
 2552, 2553, 4617, 7583, 7583,    0, 2559, 2560, 2280, 2564,
    0,    0,    0, 2316,12671, 2574,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2581,   61,
 2584,    0,    0,    0,    0, 2258,    0,    0,    0,    0,
    0,    0,    0, 2213,    0,    0,    0,    0, 1845,    0,
10656, 2465, 2254,    0, 1954, 7583, 1575,    0,    0,    0,
 2136,11944, 7516, 2255, 2126,    0,    0,    0,    0, 1516,
    0,    0,  721,    0,    0,    0,    0,    0, 2979, 2370,
 2249,    0, 1140, 7583,    0, 2320,  822, 2218, 2321, 2221,
 2365, 2329, 2372, 2010,    0,    0, 2627,    0, 2629,    0,
 2330, 2370, 2185,    0,    0,    0,    0,    0,    0,    0,
    0, 2010, 1835,    0, 1516, 2160, 2358, 2298,    0, 1948,
    0,    0,    0, 2327,  344,    0, 2030, 2328, 2979, 2646,
 2656,  875,    0,    0, 2659, 2660, 2979, 2662, 2664,11944,
11944, 2979, 2665, 2671, 2672, 2673, 2675, 2676, 2678, 2681,
 2684, 2685,    0,    0,    0,    0,11944, 2686,    0,    0,
11944,    0,    0,    0, 1873,  376,    0, 2411,    0,11386,
 7583,11944,    0, 7516, 2979,    0,    0,    0, 2677,    0,
    0,    0, 7583,    0,    0, 3658,    0,    0,  732,    0,
 2279,    0,    0,    0,  -18, 2692,    0,    0,  431, 1873,
 7583,    0,    0, 1281, 2979, 2690,    0,   85,  651,    0,
    0,    0,    0,    0,    0,    0,    0, 2696, 7583,    0,
    0, 2386, 2395, 7583, 1919,    0,    0, 1372, 7583,    0,
    0,    0,    0,    0,    0,    0, 2126,    0,    0,    0,
    0,    0, 2126,    0,    0,    0,    0, 2434, 1873,    0,
    0,11944, 1406,    0,    0,    0, 1406,    0,    0,11944,
    0,    0,    0,    0, 2979, 2979,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2979,    0,    0, 2084,
 6852,    0,    0, 2706,    0, 2761,11864, 1695,  895, 2683,
    0, 2408, 1410,    0,    0, 2711,    0, 7583, 7583,   65,
  146,    0,    0, 2250, 2421, 7516,    0, 2084, 2725,    0,
 7516,11944,    0,    0, 2404,    0, 2733,    0,    0,    0,
    0,    0, 1919, 1372,    0, 2380,    0,    0,    0,    0,
 2160,    0, 2434, 2731, 1575,    0,    0, 2084, 2979,    0,
    0, 1721, 2201,    0, 1965,    0, 2371, 3658,    0, 2349,
    0, 3658,    0, 3658, 2746,    0,    0,    0,    0,11526,
  732,11944, 3658,  904, 2201, 7583,    0,   85,    0,    0,
10951,    0, 2380,  427, 1919, 2394, 2402,    0,    0, 1575,
 2749, 2434, 2449, 2201, 2750, 1873, 2255, 2752, 1437, 2735,
 1415,11526, 2751,    0,    0,    0,    0, 1442,  349, 2493,
 2255,    0, 1956, 7583,    0, 2757, 2755, 1052, 1919,  524,
  524,    0, 2504,    0, 2373,    0,    0,    0, 1575, 2434,
    0,    0, 2084, 2420,    0, 3658, 2751,    0,    0, 2507,
 3658, 2516, 2508,11526, 2420, 7583,    0,    0,    0,10951,
 2771,    0, 7583,    0,    0, 2353,  544,    0,    0,    0,
 1773, 2201, 1222, 1982,11526,  275, 7583,11526,    0, 1222,
    0,    0, 7583, 2480,    0,    0,    0, 7516, 2255,    0,
    0,    0,11526,    0,    0,    0, 2774, 2778, 2781,    0,
   85, 2420,    0, 7583,    0, 7583, 1222,    0, 2782,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  243, 1023,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  144, 9611,    0,    0,    0,    0,    0,    0,
 2822,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  116,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  911,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2487,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   22, 2444,
    0,    0,    0,  142,    0,  142,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2445,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1047,    0,    0,11625, 9691,    0,    0,    0, 1179,
    0,    0,    0, 2071,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  276,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   24,    0,    0,    0,    0,  833,    0,    0, 1218,    0,
    0,    0,    0,    0,   97,    0,10141,    0,    0,    0,
    0,    0,    0,    0,    0,   24,    0,  457,    0,    0,
 2785,    0,    0,    0, 2567,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1248,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1302,    0, 2568,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  847,    0,    0,    0,
    0,    0,    0,    0, 9764,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  416,    0,    0, 1333,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   24,
    0, 2785,    0, 2567,    0, 1369,    0,    0,    0,    0,
  225,    0,    0,    0,    0, 1390,    0, 2556,    0,    0,
    0,    0, 1482,    0,    0,    0,    0,    0, 2568,  984,
12988,    0, 1525,    0,    0,    0,    0,  838,    0,    0,
    0, 2384, 2384,    0,    0,    0, 1529,  132,    0,    0,
    0,    0,    0,    0,    0, 2568,    0,    0, 1462,    0,
    0,    0,    0,    0,    0,    0, 8416, 2458,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1859,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2709,    0,    0,    0, 3013, 3317,    0,    0,    0,
    0,    0,    0,    0, 1111,    0,    0,    0,    0,    0,
    0,    0,    0,  240, 3621,    0, 2400,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1549,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1553,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3354,    0, 2524,    0,    0,    0,
    0,    0,    0,  564,    0,  564,    0,    0,  621,    0,
  665,  891,    0,    0,    0,    0, 1248,    0, 2568,    0,
    0,    0,    0,  499,  313,    0,    0,  722,    0,  846,
    0,    0, 1257,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2556,    0,    0, 1167,
    0, 2379,    0,    0,  618,    0,    0,    0,    0, 1094,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1559,    0,    0, 3951, 5318,    0,    0,    0,12451,    0,
    0,12475,    0,    0,12528,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1871, 2723,
    0,    0,    0,    0,12540,12463,    0,12528,   24,    0,
    0,    0,    0,    0,    0,    0,    0,  984, 1287,    0,
    0,    0,    0, 2435,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 5720, 5907, 6027,    0,    0,  584,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,12183,    0,12183,    0,    0,    0,12183,12183,
12183,    0,    0,    0,    0,    0, 2407,    0,    0,    0,
    0, 4661, 4965, 9945, 2801,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1917,    0,    0,    0,    0, 2579,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  164,    0,    0,
 9611,    0,    0,  164,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  809,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  528,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  984,    0,
  622,    0,    0,  398,    0,    0,    0,    0,  820,    0,
    0,    0,    0,    0,    0,    0, 1995,    0,    0,    0,
    0,    0, 2813,    0,    0,  876, 4286,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  849, 1005, 1006,    0,
    0,    0,  582,    0,    0,    0,    0,    0,    0, 4471,
    0,    0,    0,    0,    0,12475,    0,    0,    0,    0,
 1320,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2568,    0,    0,  363,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 9030,    0,    0,    0,  197,    0,    0,
    0,    0, 2554,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,12183,    0,    0,    0,
    0,    0,    0,    0, 3925, 5181, 5485, 4141, 4445, 1181,
    0,    0,    0,    0,    0,    0,    0,    0, 9865,    0,
 2591,    0,    0,    0,    0,    0,    0,    0,    0, 1576,
 1577,    0,    0,    0,    0,    0,    0,    0, 1578,    0,
    0,11625,    0, 1578, 1603, 1606,    0,    0,    0,    0,
    0,    0,    0,    0,  854,  180,  231,    0,    0,    0,
    0,   96,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  174,    0,    0,    0,    0,    0,    0,
  801,    0,    0,    0,    0, 2384, 2413,    0,  987,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2579,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1527,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1645,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 4798,    0,    0,    0,    0,    0,    0,    0,    0, 1550,
    0,    0,    0,    0,    0,    0,    0, 1287, 2579, 2384,
 1648,  984,    0, 2605,11625, 1656,  135,    0,    0,    0,
    0, 9030,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1651,    0,    0,    0, 2789,    0,    0,    0,    0,    0,
    0,    0, 3093,    0, 3397,    0, 4349,    0,    0,    0,
    0,    0,    0,    0, 4869,    0,    0, 5291,    0,    0,
 6091,    0, 6123,    0,    0, 6168,    0,    0, 6251,    0,
    0, 6796,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2439,
    0,    0,    0,    0, 1073,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  613,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1576,
 1673,    0, 2437,  328,    0,    0,    0,    0,    0,    0,
 1677, 1682, 1696,    0,    0, 2532, 1307,    0,  991,    0,
 2537,    0, 2539, 2542,    0,    0,  487,    0,    0,    0,
 2579,    0, 1656,    0,  831,    0, 2428,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2157,    0, 1527, 2487,
    0,    0,    0,    0,    0, 1041, 1049, 1133, 1151, 1173,
 1252,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  934,    0,    0,    0,    0,
 5328,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1699,    0,    0,    0,    0,    0,    0,    0, 1700,    0,
    0,    0,    0,    0,  170,    0,    0,    0,    0,    0,
    0,  201,    0,  256,    0,    0,    0, 9945,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6325,    0, 6331,
    0, 6471,    0,    0,    0,    0, 6491,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2843,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 9444,    0,    0,    0,    0,    0,    0,
    0, 1739, 1778,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  964,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1799,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1706, 2070,    0,    0,    0, 1080,    0,    0,    0,
    0,    0,    0,  656,    0,    0,    0,    0,    0, 1158,
    0,    0,    0,    0,    0,    0,    0,    0, 2846, 1644,
 2888,    0, 2556,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2579,    0,    0,    0,    0,    0,    0,
    0, 1708, 1239,    0,    0,    0,    0,    0,    0,    0,
    0, 2579, 2605,    0,  628,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  236,    0, 6565,    0,
    0,12183,    0,    0,    0,    0, 6628,    0,    0,    0,
    0, 6639,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  461, 2832,    0,    0,    0,    0,
    0,    0,    0,    0, 2834,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2588,    0,    0,    0,    0,    0,    0,    0,  328,  130,
  888,    0,    0, 2532,  497,  204,    0,  527, 2855,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1829,    0,    0,  154,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  819,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 6702, 6776,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2856,    0,    0,   77,
    0,    0,    0,    0,    0,    0,  108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  185, 3919,    0,
    0,    0,    0,    0, 2108,    0,    0,    0,    0,    0,
    0,    0, 1829, 1183,    0,  193,    0,    0,    0,    0,
    0,    0,    0,  419, 1057,    0,    0,  362, 2846,    0,
    0,    0,   72,    0,    0,    0, 2845,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  341,
    0,    0,    0,    0,  109,    0,    0, 4135,    0,    0,
 2855,    0, 1280,    0, 1527, 1391, 1555,    0,    0, 2532,
 1838,    0,    0,  337,    0,  461,   67,    0,    0,    0,
    0,  341, 2847,    0,    0,    0,    0,    0, 2603,    0,
  143,    0,    0, 1099,    0,    0, 2872,    0, 1829,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1207,    0,
    0,    0,   77,   27,    0,    0, 2859,    0,    0,12565,
    0,    0,    0,  341,  151,    0,    0,    0,    0, 2855,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2532,   72, 2874,    0,  341,    0,    0,  341,    0, 1202,
    0,    0,    0, 2013,    0,    0,    0,    0,   67,    0,
    0,    0,  341,    0,    0,    0, 2879,    0,    0,    0,
 3308,   27,    0,    0,    0,    0, 2874,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2925,    0,    0, 2927,
    0,    0,    0,    0,    0,    0,    0,   10,    0, 2928,
    1,  663,   -2, -570, 1720,  745, -286,  106, 2919, 2658,
    0,  -44,    0, 2663, -102,  -40,    0,  347, -909,    7,
  -65,  -43, -219, 2667, 2222,    0,    0,    0, 1500,    0,
    0,  -24, 1854, -575,  -74, -935, 2307, 2095, -307, 1862,
 2864, -290,    0,  -57,    0, -490,  -54,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -190,  -38, 1869, -321, 1114, -869,
    0,    0,    0,    0, -303, -110,-1115, 1878, -343,  -52,
    0, -541,    0,    0, 1473, -992,    0, 1467,    0,    0,
 2638, -675,  -20,    0,    0,    0,    0, 2604,    0, 2387,
 1896,    0,    0,    0, 1950,    0, 2661, 2211, 1899, 2642,
 2643,    0, 1429,    0, -440, 2199,  -32, -629, 1386, -185,
    0,    0, -913, -910, -891, -884, -871,    0, 1528,-1071,
    0, 1233,-1176, 1531,    0, 2443,    0,    0,    0, 1231,
 1242, 1208,  -27, -255, -821, -861,    0,    0,    0, 2227,
    0, -616,    0, 2064,    0,    0,    0,    0,    0,-1154,
 -838,-1114,    0,    0,    0,    0, -212,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1344,    0,    0,  389,    0,    0, -159,    0, -393,  229,
 1625, 1444,    0, 1271,    0,    0,    0,    0,    0,    0,
    0, 2322, -128, 2017, 1800, 1579,    0,-1007, -842, -889,
    0, -526, -786, -433,    0, -703, 1305,  329, -282, -510,
 -754, -980,-1004,    0,    0,  -49,    0,    0,    0,    0,
    0, 2693,    0,    0,    0,    0, 2704, 2468,    0,    0,
    0,    0,    0,   -9, 2717,    0,    0,    0,  -10,    0,
    0,    0, -350, 2219, -349, 1768, -470,    0,    0,    0,
    0,    0, -901,    0,    0,    0,    0, 2450,    0, 2231,
    0,    0,    0,    0,    0,    0,  126,    0, 3057,    0,
    0,    0,    0,    0, 2768,    0,    0,    0,    0, 2570,
    0, 2563,    0,    0,    0, 2325,    0,    0,    0,  -45,
  -47,    0,    0,  160,    0, 2312,    0,    0,    0,    0,
    0, -628,-1081, -689, -168,  945,    0, 2934, 1370,    0,
 2339,-1051,-1444,    0, -922, 2096,    0,    0,    0,    0,
 -129,    0,    0,    0, 1385, 1489,    0, 1260,    0, 1298,
    0,  -77,    0,    0,    0,    0, 2582,    0,    0,    0,
 -417, 2377, 2128,-1033,    0,    0,    0, -696, -395,  538,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2132, 1311, 1972,    0,    0,    0,    0,
 -867,    0,    0,    0, 1863,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -676, 1941,    0,    0,
};
#define YYTABLESIZE 13510
static short yytable[] = {                                     132,
  765,  198,  132,  270,   79,  154,  153,  221,  159,   38,
  929,  217,  954,  204,  290,  293,  201,  258,  555,  230,
  619,  620,  795,  589,  319,  231,  710,  226,  252,  261,
  238,  218,  289,  292,  557,  256, 1112,  696,  213,  751,
 1113, 1393,  230,  771,  195,  197,  200,  203,  153,  208,
  257, 1266,  262,  481,  219,  197,  224,  263,  203,  583,
  153,  208,  481,  237,  132,  937, 1012,  625,  255,  729,
  869, 1226,  248,  154,  153,  250,  197,  233,  248,  224,
  200,  203,  259,  153,  208,  237,  132,  847,  203, 1068,
  277,  324, 1485,  562, 1190,  702,  152,  649, 1198,  958,
  260,  276,  982,  278,  525, 1094,  764,  711,  707,   78,
  749,  322,  707, 1313,  320,  625,  786,  702,  787, 1255,
  633,  502, 1256,  797,  219,  153,  219,  300,  860,  709,
 1255,  153,  383, 1256,  361, 1291,  702,  760,  224, 1218,
 1017, 1257,  711,  577,  318,  203,  153,  237, 1258,  707,
  625,  159, 1257,  758,  702,  152,  625, 1037,  726, 1258,
 1040, 1259,  559,  749,  985, 1260,  320,  707, 1263, 1047,
  709,  345, 1259,  744,  625, 1267, 1260, 1456,  328,  671,
 1445, 1074,  844,  711,  702,  758, 1275,  526,  709, 1761,
  153,  625,  240,  361,  758, 1483,  372,  758,  376,  778,
  903,  711,  577,  701, 1207,  274,  736, 1296,  274,  625,
   90,  288,  758,  619,  620,  932,  933,  744, 1264, 1002,
  671,  531,  749, 1089,  640,  702,  477, 1161, 1162,  909,
  672,  466,  744,  240,  474,  477,  240,   86,  671,  318,
   86,  780,  577,  702,  701,  803,  511,  589,  829, 1581,
 1582,  240, 1314,   78,  219,  287,  877,  219, 1187,  724,
  518,  153,  701,  483,  132,  640, 1213, 1308, 1476, 1477,
  153,  672,  483,  768,  672,  638,  530,  492,  288,  530,
   98, 1102,   78,  640,  524,  355,  326,  252,  817,  672,
  839,  200,  529,  841, 1704,  153,  782, 1438,  318, 1440,
 1590,  577, 1500,  830, 1520, 1565,  942, 1457,  556, 1521,
  346,  377,  624, 1447,  951,  466,  638,  218, 1754,  570,
  291, 1802,  572,  329,  904,  818, 1525,  528, 1449,  641,
  541,  219,  629,  514,  638,   14,  707,  639,  219,  584,
  219,  586,  219,  288,  152,  575,  903, 1255, 1255,  715,
 1256, 1256,  714,  624, 1614,  637, 1251,  526,  840,  934,
 1276,  702,  126,  203,  208, 1510, 1075, 1600,  843, 1257,
 1257,  624,  153, 1179,  200, 1054, 1258, 1258, 1595, 1036,
  631, 1255, 1214,   61, 1256, 1241,  528,  347, 1498, 1259,
 1259, 1861, 1754, 1260, 1260,  707,  152,  642,   14, 1646,
   99, 1601, 1478, 1257, 1210, 1261, 1262,  702,  378, 1483,
 1258,  759,  249,  702, 1495,  600, 1221, 1223,  366, 1465,
  702,  126,  699, 1259, 1517, 1274,  702, 1260, 1692,  819,
  720,  153,  355,  625,  252,  702, 1681,  723,  642, 1331,
 1332,  728, 1333, 1334,  152,  734, 1514,  294, 1725, 1760,
  477,  989,  723,  711,  524,  625,  642,  709,  707, 1031,
  709, 1095, 1033,  702, 1277,  153, 1117,  707,  203,  483,
  904,  823,  702, 1043,  600,   78,  153,  366,   90,  153,
  153,  282,  702,  203, 1114,  153,  707,  537,  159,  516,
  908,  702,  737,  288,  846,  707,  705,  671,  631,  542,
 1061,  709, 1379, 1067,  707, 1381,  481,  671,  824,  530,
  671, 1647, 1651, 1596,  389,  702,  709,  320, 1526,  671,
  779,  701,   77,  286,  758,  709,  706,  707,  869,  711,
  749,  799, 1644,  230,  701,  702,  758,  705, 1604,  631,
  705, 1438,  631,  701,  530,  707, 1772,  625,  672,  702,
  748, 1013, 1513,  792,  219,  705, 1104,  631,  672,  792,
  825,  672, 1217,  605,  640, 1707,  671,  706,  725,  625,
  672,  702,  990,  700, 1473,  671, 1388, 1307, 1873,  577,
  702,  179, 1109,  708,  955,  706,  780,  711,  361, 1219,
  701, 1656,  707,  638,  286, 1087, 1487,  702, 1812,  701,
  273,  851,  559, 1611, 1616,  218, 1067,   86, 1617, 1618,
  854,  855, 1832,  152, 1206,  638,  702,  672, 1035,  671,
  606,  635,  605,  739,  708,  744,  672,  179,  219,  707,
  624,  671, 1845,  671,  526, 1101,  625,  871, 1005,  647,
  179,  782,  708,  701, 1645, 1796,  530,  870,  153,  318,
  709,  577,  624, 1452,  758,  711,  911,  701,  330, 1571,
 1786, 1118,  635,  711,  616,  635,   77,  224,  647, 1278,
  672,  625, 1637,  286,  138, 1044, 1512, 1830,  640,  606,
  635,  466,  672,  466,  672,  963,  179, 1115, 1744, 1660,
 1643,  944,  702,  240,  528,   77,  711,  723, 1654,  219,
  671,  530, 1874, 1559,  219,  702,  219, 1562,  219, 1532,
  126,  483,   84,  518,  711, 1011,  483, 1474,  136, 1859,
 1265,  636, 1246,  616,  701,  483, 1779, 1810,  203,  638,
  903,  524, 1273, 1339,  992,  203,  218,  642,  286, 1052,
 1872, 1718,  123, 1875, 1200, 1719,  366, 1203,  153,  366,
  153,  672, 1025,  219, 1295,  466,  139,  600, 1883,  219,
 1728, 1615,  636, 1050,  230, 1572,  624, 1255,  163,  356,
 1256, 1060, 1084, 1085, 1086, 1729,  707, 1732,  219, 1486,
  636, 1730, 1053, 1055, 1072, 1119, 1309, 1608,  709, 1257,
  707,  709,  600,  330, 1104, 1386, 1258,  230,  230,  230,
  637,  702, 1066,  283,  707,  219,  219, 1088,  709, 1259,
 1279, 1116,  134, 1260,  705,  702,  631, 1099,  709,  170,
  600,  164, 1499,  600, 1553, 1238,  707,  705, 1509,  636,
  641, 1245,  383,  624, 1455, 1655,  705,  481,  631,  600,
  986,  637,  153, 1866,  706,  644,  357,  709,  389,  709,
  526,  642,  153,  689,  904,  161,  709, 1366,  366,  637,
 1778,  647, 1630,  373, 1111,  600,  706,  709, 1811, 1785,
  366,  641,  366,  707,  754,  206, 1239,  709,  170,  135,
 1026,  179,  707,  705, 1679,  284,  644,  679,   77,  641,
  601,  383,  705, 1743,  689, 1867,  481,  689, 1689,  374,
 1846,  708, 1204, 1384,  644,  605, 1788,  389, 1319, 1817,
  579,  708,  689,  706,  708,  148,  206,  123,  123,  206,
  282, 1765,  706,  708, 1201, 1205,  707,  679,  679,  179,
  330,  679,  609,  137,  206, 1750,  705,  179,  903,  635,
  707, 1847,  211,  212, 1800,  755,  679,  524, 1566,  601,
  705, 1784,  631, 1027,  483,  203,  208,  483,  237,  132,
 1494,  635,  606, 1005,  155,  483,  706,  778,  605,  579,
  708,  605, 1202,  711,  179,  179,  179,  483,  483,  708,
  706,  709,  903,  609,  203,  553,  171,  605, 1848,  474,
  687, 1516,  137, 1713, 1697,  711,  156,  606,  554,  179,
  631,  162,  587, 1225,  387,  388,  616,  707, 1302, 1268,
 1269,  157,  219,  605,  975,  619,  620,  705, 1714,  631,
  149,  225,  800,  708,  179,  606, 1322,  150,  606, 1780,
 1764,  687, 1326,  230,  687, 1768, 1852,  708, 1680,  636,
  394,  616,  711, 1698,  606,  171,  575,  706,  395,  687,
  541,  230,  209,  434, 1699, 1286,  367,  230,  954,  218,
 1320,  636,  904,  387,  388,  330,  474,  526, 1287,  616,
  606, 1286,  616, 1708,   77,  635, 1715,  170, 1819,  673,
 1211,  707,  219,  903, 1443, 1443, 1446,  975,  616,  219,
  170,  383, 1222,  403,  731,  711,  383, 1824,  179,  394,
  367,  481,  383,  801,  708,  575,  904,  395,  705,  711,
  883, 1835,  389,  706,  616,  367, 1851,  689,  637,  170,
  673,  732, 1738,  673, 1297,  170,  709,  170,  705,  703,
 1192,  704,  391,  706,  294, 1470,  709, 1472,  673,  709,
  637, 1297,  635, 1389,  802,  810,  709,  811,  709,  709,
  390,  883,  403,  206,  883,  206,  903,  179, 1700,  294,
  466,  803,  206,  218,  524,  903,  468,  170,  243,  883,
  641,  689,  393, 1298,  689,  636,  711,  295,  584, 1882,
  884,  689,  758,  219,  689,  644, 1394, 1435,  689,  878,
 1301,  391, 1881,  689,  885,  709,  689,  483, 1515,  483,
 1652,  638,  905,  883,  709,  679,  368,  904,  679,  390,
  468,  375,  689,  137,  483,  679,  179,  581,  679,  588,
  203,  884,  679,  758,  884,  468,  758,  679,  286,  383,
  679,  393,  601,  752,  481,  503,  689,  584,  138,  884,
  689,  758,  638,  509,  171,  389,  679,  709,  709,  689,
  368,  392,  306,  206,  637, 1004,  474,  171,  709, 1823,
  638,  225,  709,  763,  264,  368,  689,  601,  387,  388,
  679,  280,  709,  884,  679,  206,  581, 1182, 1183,  240,
  904,  330,  206,  679,  641,  526,  171,  116, 1497,  904,
  763,  514,  171,  689,  171,  601,  266,  138,  601, 1524,
  679,  320,  225, 1528,  394,  689,  709,  689,  687, 1787,
  392,  687,  395,  689,  601,  474,  206,  576,  687,  136,
  240,  687, 1731,  240,  116,  687, 1603,  679,  577,  709,
  687, 1801,  583,  687,  171,  245,  170, 1570,  240,  679,
  601,  679,  689,  689, 1280,  689,  701,   81,   82,  687,
 1821,  702,  689,  689,  689,  689,  689,  403,  689,  116,
  320,  689,  689,  689,  689,  689,  701,  362,  570,  689,
  689,  702,  338,  687,  689,  689,  435,  687,  136,  959,
  959, 1281,  524,  225,  367,  197,  687,  367, 1841,  410,
  236,  583,  515, 1580, 1111, 1111,  391,  673,  253,  700,
  500,  387,  388,  687,  363, 1435,  364,  673,  679,  505,
  673,  267,  294,  878,  390,  179,  330,  878, 1869,  673,
  339, 1184,  700,  275,  225, 1842, 1694,  570,  179,  365,
  687,  236,  271, 1130,  236,  722,  393,  394, 1843, 1628,
  534,  483,  687,  538,  687,  395,  318,  516,  410,  236,
  670,  203, 1783, 1754,  366,  913, 1696,  179, 1754, 1131,
  758,  285,  758,  179,  517,  179,  673, 1467, 1753,  758,
 1443, 1710,  722, 1829,  153,  673, 1826, 1468,  294,  670,
  670,  411,  670,  705,  703,  722,  704,  272,  706,  670,
  670,  670,  670,  670,  432,  670,  367, 1783,  670,  670,
  670,  670,  670,  171,  101,  179,  670,  670,  367,  279,
  367,  687,  670,  179,  474,  392,  375, 1622,  138,  673,
  883,  915,  773,  959,  407, 1726,  234,  474,  161,  391,
 1687,  673,  340,  673,  368, 1139, 1783,  368,  687, 1623,
  411,  103,  692,  693,  695,  383, 1624,  390,   44,  700,
  341, 1140,   52, 1437,  237, 1439,  383,  240,  162,  240,
  758,  203,  474,  294,  474,  758,  240,  234, 1783,  393,
  234,  143,  144, 1111, 1727,   42,   25,  754,  350, 1724,
 1625,  351,  758,  407,  342,  234,  179,  161, 1694,  758,
  884,  318, 1694,  505, 1794,  237, 1141,  136,  237,  136,
  673, 1231,   48, 1694, 1705,   50,  917,   44,  280,  944,
  104,   52,  740,  237,  153, 1232,  742,  162, 1696,  200,
  105, 1773, 1696,  758, 1696, 1142,  352,  674, 1740, 1694,
  526, 1569, 1741, 1696,   42,   25,  754,  154,  153, 1233,
 1756,  674,  505,  200,  483,  106,  368,  128,  392,  674,
  831,  705,  703,  240,  704,  378,  706,  240,  368, 1696,
  368,   48,  526, 1711,   50,  674, 1694,   53,  236,  144,
  236, 1694,   26,  281,  179,  701,   49,  236,   54,  240,
  702,   51,  301,  758,  200,   55,  240,  200,  197, 1758,
  241,  831, 1143, 1234,  831,   46, 1696,  136,  129,  117,
 1759, 1696,  200,  483,  526,   47,  128,  200,  327,  831,
   56,  741,  705,  703,  378,  704, 1008,  706,  317,  674,
  240,  701,  154,  153,  348,  526,  702,  524,  526,  705,
  703,   26,  704, 1009,  706,   49,  705,  703,  359,  704,
   51,  706,  330,  526,  387,  219,  705,  703,  361,  704,
 1435,  706,  392, 1749,   46,  370,  992,  129,  117,  524,
 1804,  153,  705,  703,   47,  704,  200,  706,  236,  380,
  884,  154,  153,  719,  789,   57,  718,  788, 1853,  384,
  240, 1831, 1435,  939,  997,  382,  707,  907, 1864, 1860,
  236,  912,  981, 1804,  153,  980,  997,  236,  928,  997,
  385,  524,  302,  935,  936, 1870, 1888,   58, 1889,  154,
  153,   59, 1876,  435, 1695,  945,  946,  947,  948,  949,
  950,  435,  524, 1002, 1435,  524,  992,  389,  234, 1804,
  153,  236,  237,  200,  237, 1002, 1887,  303, 1002, 1890,
  524,  237,  332, 1093,  988, 1435, 1092, 1111, 1435,  705,
  703,  471,  704,  200,  706,  333,  985,  388,  995,  975,
  505, 1100,  486, 1435,  865,  304,  211,  212,  305,  234,
  507,  478,  234,  487,  200,  490,  200,  938,  705,  703,
 1014,  704,  491,  706,  306,  534, 1021,  234,  493,  701,
  334,  236,  335,  495,  702, 1231,  107,  496,  971,  995,
  995,  995,  995,  995,  995,  995, 1062, 1063,  497, 1232,
  307,  507,  831,  510,  507,  513,  530,  995,  995,  995,
  995, 1191,  124,  124,  707,  108,  109, 1557,  110,  507,
  512, 1194,  237, 1233, 1193,  111,  112,  113,  114,  115,
 1242,  116,  522, 1092,  117,  118,  119,  120,  121,  995,
  701,  995,  122,  123,  237,  702, 1289,  530,  124, 1288,
  530,  237, 1294, 1450,  532, 1293,  707,  701,  831,  539,
 1533,  831,  702,  707,  701,  530, 1695,  530,  831,  702,
 1695,  831, 1795, 1564,  701,  831,  707, 1234,   14,  702,
  831, 1695,  546,  831, 1602,  237, 1837,  707, 1344, 1836,
  701, 1120, 1121, 1125,  547,  702, 1133, 1135, 1137,  831,
 1145, 1148, 1151, 1153, 1156, 1159,  548, 1695, 1163, 1164,
  568, 1166, 1871, 1168, 1178, 1754, 1171, 1172, 1173, 1174,
 1175, 1176,  107,  831, 1748,  373,  831,  831,  373,  550,
  745,  571,  906, 1180,  910, 1185,  831,  581,  914,  916,
  918,  585,  634,  730, 1695,  237,  730,  638,  640, 1695,
  642,  108,  109,  831,  110,  673,  674,  675,  676,  670,
  692,  111,  112,  113,  114,  115,  677,  116,  994,  678,
  117,  118,  119,  120,  121,  831,  679,  701,  122,  123,
  831,  680,  702, 1231,  124,  681,  435,  682,  683,  991,
  705,  703,  831,  704,  831,  706,  234, 1232,  234,  670,
  670,  685,  690,  670,  692,  234,  701,  684, 1541,  691,
  995,  702,  995,  995,  408,  409,  410,  700,  670,  692,
  995, 1233,  707, 1170,  705,  703, 1791,  704,  995,  706,
 1793,  708,  709,  995,  267,  713,  995,  716,  507,  717,
  507, 1799,  418,  419,  507, 1868,  213,  507,  369,  721,
  995,  735,  726,  738,  745,  995,  995,  747,  758,  995,
  507,  831,  369,  762,  530,  994,  995, 1827,  995,  995,
  369,  767,  995,  763,  769, 1234,  995,  770,  774,  995,
  530,  995,  772,  995,  775,  777,  369,  213,  995,  995,
  213,  995,  541, 1134,  780, 1138,  234, 1146, 1149,  781,
 1154, 1157, 1160,  783, 1854,  213,  530,  995,  995, 1856,
  995,  790,  530,  814,  784,  530,  100,  815,  234,  816,
  827,  821,  286,  826,  995,  234,  995,  995,  995,  831,
  435,  995,  832,  834,  995,  995,  705,  703,  507,  704,
  369,  706, 1347, 1348,  995, 1350, 1352,  836,  833,  842,
 1364,  705,  703, 1357,  704,  837,  706,  101,  995,  234,
  507,  995, 1365,  705,  703,  848,  704,  507,  706,  102,
  849,  852, 1370,  853,  862,  864,  865,  995,  874,  873,
 1377, 1378, 1185,  995,  995, 1185,  875, 1180,  995,  879,
  930, 1385,  435, 1387,  103,  940,  843,  288,  959,  995,
  995,  507,  995, 1368,  705,  703,  976,  704,  995,  706,
  975,  978,  983,  987, 1001,  988, 1006,  530, 1015, 1018,
 1022,  505, 1032, 1041,  530, 1024, 1042, 1044,  701, 1048,
  793,  435, 1049,  702, 1051, 1057, 1058,  995,  995, 1070,
  995, 1059,  530, 1076, 1065, 1081, 1083,  995,  995,  995,
  995,  995, 1090,  995, 1095, 1107,  995,  995,  995,  995,
  995, 1108,  701,  104,  995,  995, 1127,  702, 1479,  995,
  995,  731,  530,  105, 1167, 1169,  530,  670,  793,  701,
  670,  692, 1195, 1177, 1209, 1212, 1197,  670,  692,  975,
  670,  692, 1215, 1207,  670,  692, 1224, 1228,  106,  670,
 1230, 1236,  670,  692, 1250, 1247, 1369,  705,  703, 1270,
  704, 1272,  706, 1282, 1290, 1292, 1299,  505,  670,  692,
  705,  703, 1188,  704,  225,  706,  213, 1303, 1304, 1305,
  975,  975,  975,  975,  975,  988,  975, 1306, 1529, 1324,
 1315, 1317,  670,  692, 1101,   14,  670, 1537,  975,  975,
  975,  975, 1327, 1328, 1542,  670, 1329, 1330, 1336,  240,
 1371,  705,  703, 1733,  704, 1337,  706, 1342, 1341, 1343,
 1345, 1346,  670,  692,  701, 1353, 1354, 1355,  692,  702,
  975, 1356,  975, 1185, 1358, 1359, 1360, 1185,  107,  701,
 1361, 1362, 1363,  903,  702, 1383,  241, 1436, 1451,  670,
  692,  701, 1380, 1575, 1453, 1454,  702,  743, 1458, 1461,
 1459,  670,  692,  670,  692, 1462, 1460,  108,  109,  670,
  110, 1463,  718, 1471,  225,  563, 1484,  111,  112,  113,
  114,  115, 1496,  116, 1488, 1511,  117,  118,  119,  120,
  121, 1489,  701, 1490,  122,  123,  225,  702,  670,  670,
  124,  670, 1501,  225,  107, 1491, 1067, 1492,  670,  670,
  670,  670,  670, 1493,  670, 1605, 1518,  670,  670,  670,
  670,  670, 1522, 1338, 1523,  670,  670, 1527, 1534, 1554,
  670,  670, 1560,  108,  109,  107,  110,  225, 1561, 1567,
 1568, 1574,  135,  111,  112,  113,  114,  115, 1573,  116,
 1576, 1577,  117,  118,  119,  120,  121, 1584, 1585, 1586,
  122,  123, 1587, 1588,  108,  109,  124,  110,  705,  703,
 1189,  704, 1593,  706,  111,  112,  113,  114,  115, 1594,
  116, 1597, 1598,  117,  118,  119,  120,  121, 1599, 1619,
 1609,  122,  123, 1665, 1666,  701, 1629,  124, 1632, 1631,
  702,  975, 1633,  975,  975, 1634, 1635, 1638,  701, 1639,
 1677,  975, 1636,  702, 1185,  705,  703, 1648,  704,  975,
  706, 1640, 1650, 1686,  975, 1688, 1658,  975,  919,  920,
  921,  922,  923,  924,  925,  926, 1659, 1653, 1657, 1661,
 1662,  975, 1663,  975, 1664, 1667,  975,  975,  999,  701,
  975, 1668, 1669, 1670,  702, 1671, 1672,  975, 1673,  975,
  975, 1674,  530,  975, 1675, 1676, 1678,  975, 1684, 1589,
  975, 1706,  975, 1712,  975, 1691, 1720, 1722, 1723,  975,
  975, 1751,  975,  705,  703, 1745,  704, 1752,  706,  999,
  999,  999,  999,  999,  999,  999, 1755, 1762,  975,  975,
 1763,  975, 1774,  530, 1766, 1739,  530,  999,  999,  999,
  999, 1770, 1771, 1742, 1782,  975, 1813,  975,  975,  975,
 1789,  530,  975, 1792, 1815,  975,  975,  953,  800, 1818,
 1822, 1820, 1825, 1828, 1754,  975, 1834, 1839, 1840,  999,
  435,  999,  705,  703, 1810,  704,  163,  706, 1857,  975,
 1863, 1858,  975, 1855, 1811, 1865, 1879, 1884, 1885, 1746,
 1886,    1, 1891,  631,  186, 1605,   30,   32,  975,  800,
  758,  254,  800,  375,  975,  975, 1372,  705,  703,  975,
  704,  149,  706,  663,  883,   82,  528,  800,  465,  265,
  975,  975,  196,  975, 1373,  705,  703,  958,  704,  975,
  706,  264,  528, 1374,  705,  703,  701,  704,  403,  706,
  176,  702,  528,  957,  700, 1798, 1375,  705,  703,  698,
  704,  694,  706,  939,  696,  177,  376,  214,  975,  975,
  206,  975,  315,  107,  717,  281,  938,  743,  975,  975,
  975,  975,  975,  323,  975,  350,  335,  975,  975,  975,
  975,  975,  718,  701,  638,  975,  975,  321,  702,  727,
  975,  975,  108,  109,   27,  110,   30,   40,  214, 1441,
  140,  214,  111,  112,  113,  114,  115,  485,  116,  494,
  484,  117,  118,  119,  120,  121,  214,  977,  863,  122,
  123,  254, 1376,  705,  703,  124,  704, 1325,  706, 1106,
 1555,  705,  703, 1323,  704, 1165,  706, 1321, 1311, 1642,
  999,  593,  999,  999, 1641,  552, 1300,  828, 1249, 1016,
  999,  701,  530, 1556,  705,  703,  702,  704,  999,  706,
 1312,  565,  566,  999, 1682, 1046,  999, 1563,  705,  703,
  530,  704,  530,  706, 1721, 1809,  530, 1816, 1621,  530,
  999,  809, 1004, 1627, 1039,  999,  999, 1814, 1850,  999,
  705,  703,  530,  704, 1199,  706,  999, 1579,  999,  999,
 1747, 1797,  999, 1243, 1703,  956,  999, 1781,  813,  999,
  701,  999, 1466,  999, 1612,  702,  558,  573,  999,  999,
  800,  999,  549, 1004, 1004, 1004, 1004, 1004, 1004, 1004,
  705,  703, 1082,  704, 1508,  706, 1073,  999,  999,  142,
  999, 1004, 1004, 1004, 1004,  701,  523,  850,  757,  299,
  702,  753, 1007, 1020,  999, 1003,  999,  999,  999, 1227,
  953,  999,  801,  701,  999,  999, 1769, 1716,  702, 1862,
  530, 1838,  701, 1004,  999, 1004,  800,  702,  766,  800,
  984, 1220, 1349,    0, 1475,  701,  800,    0,  999,  800,
  702,  999,  530,  800, 1382,    0,    0,    0,  800,  530,
    0,  800,    0,  801,    0,    0,  801,  999,    0,    0,
    0,    0,    0,  999,  999,    0,    0,  800,  999,    0,
    0,  801,    0,    0,    0,    0,    0,    0,    0,  999,
  999,    0,  999,  530,    0,  225,    0,  214,  999,    0,
    0,  800,    0,    0,  800,  800,    0,    0,    0,    0,
    0,    0,    0,    0,  800,    0,    0,    0,    0,    0,
    0,  701,    0,    0,    0,    0,  702,  999,  999,  701,
  999,  800,    0,    0,  702,  107,    0,  999,  999,  999,
  999,  999,    0,  999,    0,    0,  999,  999,  999,  999,
  999,    0,  701,  800,  999,  999,    0,  702,  800,  999,
  999,    0,    0,    0,  108,  109,  701,  110,    0,    0,
  800,  702,  800,    0,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117,  118,  119,  120,  121,  701,    0,
    0,  122,  123,  702,    0,  225,    0,  124,    0,    0,
    0,    0,    0,    0, 1004,    0, 1004, 1004,    0,    0,
 1335,    0,    0,    0, 1004,    0,    0,  225,    0,    0,
    0,    0, 1004,    0,  225,    0,    0, 1004,  701,    0,
 1004,    0,    0,  702,    0,    0,    0,  371,    0,  800,
    0,    0,    0,    0, 1004,    0, 1000,    0,    0, 1004,
 1004,    0,    0, 1004,    0,    0,    0,    0,  225,    0,
 1004,    0, 1004, 1004,  705,  703, 1004,  704,    0,  706,
 1004,    0,    0, 1004,  952, 1004,    0, 1004,  371,    0,
    0,  371, 1004, 1004,  801, 1004,    0, 1000, 1000, 1000,
 1000, 1000, 1000, 1000,    0,    0,  371,    0,    0,    0,
    0, 1004, 1004,    0, 1004, 1000, 1000, 1000, 1000,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1004,    0,
 1004, 1004, 1004,    0,    0, 1004,  825,  667, 1004, 1004,
    0,    0,    0,    0,    0,    0,    0, 1000, 1004, 1000,
  801,    0,    0,  801,    0,    0,    0,    0,    0,    0,
  801,    0, 1004,  801,    0, 1004,    0,  801,    0,    0,
    0,    0,  801,    0,    0,  801,    0,  825,    0,    0,
  825, 1004,    0,    0,    0,    0,    0, 1004, 1004,    0,
    0,  801, 1004,    0,    0,  825,    0,    0,    0,    0,
    0,    0,    0, 1004, 1004,    0, 1004, 1530, 1531,    0,
    0,    0, 1004, 1535, 1536,  801, 1538, 1539,  801,  801,
    0,    0,    0,    0, 1543, 1544, 1545, 1546,  801, 1547,
 1548, 1549, 1550, 1551, 1552,    0,    0,    0,    0,    0,
    0, 1004, 1004,    0, 1004,  801,    0,    0,  107,    0,
    0, 1004, 1004, 1004, 1004, 1004,    0, 1004,    0,    0,
 1004, 1004, 1004, 1004, 1004,    0,    0,  801, 1004, 1004,
    0,    0,  801, 1004, 1004,    0,    0,  108,  109,    0,
  110,    0,    0,    0,  801,    0,  801,  111,  112,  113,
  114,  115,    0,  116, 1540,    0,  117,  118,  119,  120,
  121,    0,    0,    0,  122,  123,    0,    0,    0,    0,
  124,    0,  701,    0,    0,   85,    0,  702, 1000,    0,
 1000, 1000,   86,    0,    0,    0,   87,   88, 1000,    0,
    0,    0,    0,    0,    0,   89, 1000,    0,    0,    0,
    0, 1000,    0,    0, 1000,    0,    0,   90,    0,    0,
    0,    0,    0,  801,    0,    0,    0,    0, 1000,    0,
  852,   78,    0, 1000, 1000,    0,    0, 1000,  371,    0,
    0,    0,    0,    0, 1000,  371, 1000, 1000,  371,    0,
 1000,    0,  371,    0, 1000,    0,    0, 1000,    0, 1000,
  371, 1000,    0,    0,    0,    0, 1000, 1000,  825, 1000,
    0,  852,  852,  852,  852,  852,  371,  852,    0,    0,
    0,    0,   91,  667,    0, 1000, 1000,    0, 1000,  852,
  852,  852,  852,    0,    0,    0,    0,    0,    0,   92,
  371,    0, 1000,    0, 1000, 1000, 1000,    0,    0, 1000,
    0,    0, 1000, 1000,   93,    0,    0,    0,   94,    0,
    0,    0, 1000,  852,  825,  432,    0,  825,    0,   95,
  371,    0,    0,    0,  825,    0, 1000,  825,    0, 1000,
    0,  825,    0,    0,    0,    0,  825,    0,   96,  825,
    0,    0,    0,    0,    0, 1000,    0,  371,    0,    0,
    0, 1000, 1000,    0,    0,  825, 1000,    0,    0,  371,
    0,  371,    0,    0,    0,    0,    0, 1000, 1000,    0,
 1000,    0,    0,    0,    0,    0, 1000,    0,    0,  825,
    0,    0,  825,  825,    0,    0,    0,    0,    0,    0,
    0,    0,  825,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1000, 1000,    0, 1000,  825,
    0,    0,    0,  670,    0, 1000, 1000, 1000, 1000, 1000,
    0, 1000,    0,    0, 1000, 1000, 1000, 1000, 1000,    0,
    0,  825, 1000, 1000,    0,    0,  825, 1000, 1000,    0,
    0,    0,  670,  670,    0,  670,    0,    0,  825,    0,
  825,    0,  670,  670,  670,  670,  670,    0,  670,    0,
    0,  670,  670,  670,  670,  670,    0,    0,    0,  670,
  670,    0,    0,    0,    0,  670,    0,    0,    0,    0,
   63,    0,  852,    0,  852,  852,    0,    0,    0,    0,
    0,   64,  852,    0,    0,    0,    0,    0,   65,    0,
  852,    0,    0,    0,    0,  852,    0,    0,  852,    0,
    0,   66,    0,    0,    0,    0,    0,  825,  681,    0,
    0,    0,  852,    0,  864,    0,    0,  852,  852,    0,
    0,  852,    0,    0,    0,    0,    0,    0,  852,    0,
  852,  852,    0,    0,  852,    0,    0,    0,  852,    0,
  528,  852,    0,  852,    0,  852,    0,    0,    0,  681,
  852,  852,  681,  852,    0,  864,  864,  864,  864,  864,
    0,  864,   67,    0,    0,    0,    0,  681,    0,  852,
  852,    0,  852,  864,  864,  864,  864,    0,   68,    0,
    0,  528,    0,    0,  528,    0,  852,    0,  852,  852,
  852,    0,    0,  852,    0,   69,  852,  852,    0,  528,
    0,  528,    0,    0,    0,    0,  852,  864,    0,    0,
   70,    0,    0,    0,   71,    0,    0,   72,    0,    0,
  852,    0,    0,  852,   77,   73,  107,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  852,
    0,    0,    0,    0,   74,  852,  852,    0,    0,    0,
  852,    0,    0,    0,    0,  108,  109,    0,  110,    0,
    0,  852,  852,    0,  852,  111,  112,  113,  114,  115,
  852,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  852,
  852,    0,  852,    0,    0,    0,    0,  107,    0,  852,
  852,  852,  852,  852,    0,  852,    0,    0,  852,  852,
  852,  852,  852,    0,  684,    0,  852,  852,    0,    0,
  867,  852,  852,    0,    0,    0,  108,  109,    0,  110,
    0,    0,    0,    0,    0,    0,  111,  112,  113,  114,
  115,    0,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,  684,    0,    0,  684,  124,
    0,  867,  867,  867,  867,  867,  864,  867,  864,  864,
    0,    0,    0,  684,    0,    0,  864,    0,    0,  867,
  867,  867,  867,    0,    0,    0,    0,    0,  528,  864,
    0,    0,  864,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  864,  528,  528,    0,
  528,  864,  864,  867,  528,  864,  681,  528,    0,  681,
    0,    0,  864,    0,  864,  864,  681,    0,  864,  681,
  528,    0,  864,  681,    0,  864,  528,  864,  681,  864,
    0,  681,    0,    0,  864,  864,    0,  864,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  681,    0,    0,
    0,    0,    0,  864,  864,  497,  864,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  864,  681,  864,  864,  864,  681,    0,  864,    0,    0,
  864,  864,    0,    0,  681,    0,    0,    0,    0,    0,
  864,    0,    0,    0,    0,    0,  497,    0,  528,  497,
    0,  681,    0,    0,  864,    0,    0,  864,    0,    0,
    0,    0,    0,    0,  497,    0,    0,    0,  804,    0,
  528,    0,    0,  864,    0,    0,    0,  528,  681,  864,
  864,    0,    0,    0,  864,    0,  528,    0,  528,    0,
  681,    0,  681,    0,    0,  864,  864,    0,  864,    0,
    0,    0,    0,    0,  864,    0,  528,    0,    0,  804,
    0,  528,  804,  563,    0,    0,    0,    0,    0,    0,
    0,    0,  867,    0,  867,  867,    0,  804,    0,    0,
    0,    0,  867,  864,  864,    0,  864,    0,    0,    0,
  528,    0,    0,  864,  864,  864,  864,  864,  867,  864,
    0,    0,  864,  864,  864,  864,  864,    0,    0,  681,
  864,  864,  867,    0,  868,  864,  864,  867,  867,    0,
    0,  867,  684,    0,    0,  684,    0,    0,  867,    0,
  867,  867,  684,    0,  867,  684,    0,    0,  867,  684,
  525,  867,    0,  867,  684,  867,    0,  684,    0,    0,
  867,  867,    0,  867,    0,  868,  868,  868,  868,  868,
    0,  868,    0,  684,    0,    0,    0,    0,    0,  867,
  867,    0,  867,  868,  868,  868,  868,    0,    0,    0,
    0,  525,    0,    0,  525,    0,  867,  684,  867,  867,
  867,  684,    0,  867,    0,    0,  867,  867,    0,  525,
  684,  525,    0,    0,    0,    0,  867,  868,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  684,    0,    0,
  867,    0,    0,  867,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  497,    0,  497,  107,    0,    0,  867,
    0,    0,  497,    0,  684,  867,  867,    0,    0,    0,
  867,    0,    0,    0,    0,  497,  684,    0,  684,    0,
    0,  867,  867,    0,  867,  108,  109,    0,  110,    0,
  867,    0,    0,    0,    0,  111,  112,  113,  114,  115,
  804,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,  867,
  867,    0,  867,    0,    0,    0,    0,    0,    0,  867,
  867,  867,  867,  867,    0,  867,    0,    0,  867,  867,
  867,  867,  867,    0,    0,  684,  867,  867,    0,    0,
  862,  867,  867,  497,    0,    0,  804,    0,    0,  804,
    0,    0,    0,    0,    0, 1578,  804,    0,    0,  804,
    0,    0,    0,  804,    0,  497,    0,    0,  804,    0,
    0,  804,  497,    0,    0,    0,    0,    0,    0,    0,
    0,  862,    0,  862,  862,  862,  868,  804,  868,  868,
    0,    0,    0,    0,    0,    0,  868,    0,    0,  862,
  862,  862,  862,    0,    0,    0,  497,    0,  525,    0,
    0,  804,  868,    0,  804,  804,    0,    0,    0,    0,
    0,    0,    0,    0,  804,    0,  868,  525,  525,    0,
  525,  868,  868,  862,  525,  868,    0,  525,    0,    0,
    0,  804,  868,    0,  868,  868,    0,    0,  868,    0,
  525,    0,  868,    0,    0,  868,  525,  868,    0,  868,
    0,    0,    0,  804,  868,  868,    0,  868,  804,    0,
    0,    0,    0,    0,    0,    0,    0,  530,    0,    0,
  804,    0,  804,  868,  868,    0,  868,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  868,    0,  868,  868,  868,    0,    0,  868,    0,    0,
  868,  868,    0,    0,    0,    0,    0,    0,  530,    0,
  868,  530,    0,    0,    0,    0,    0,    0,  525,    0,
    0,    0,    0,    0,  868,    0,  530,  868,  530,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  802,  804,
  525,    0,    0,  868,    0,    0,    0,  525,    0,  868,
  868,    0,    0,    0,  868,    0,    0,    0,  525,    0,
    0,    0,    0,    0,    0,  868,  868,    0,  868,    0,
    0,    0,    0,    0,  868,    0,  525,    0,    0,  802,
    0,  525,  802,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  862,    0,  862,  862,    0,  802,    0,    0,
    0,    0,  862,  868,  868,    0,  868,    0,    0,    0,
  525,    0,    0,  868,  868,  868,  868,  868,  862,  868,
    0,    0,  868,  868,  868,  868,  868,    0,    0,    0,
  868,  868,  862,    0,  861,  868,  868,  862,  862,    0,
    0,  862,    0,    0,    0,    0,    0,    0,  862,    0,
  862,  862,    0,    0,  862,    0,    0,    0,  862,    0,
    0,  862,    0,  862,    0,  862,    0,    0,    0,    0,
  862,  862,    0,  862,    0,  861,    0,  861,  861,  861,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  862,
  862,    0,  862,  861,  861,  861,  861,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  862,    0,  862,  862,
  862,    0,    0,  862,    0,    0,  862,  862,    0,    0,
    0,    0,    0,    0,    0,  530,  862,  861,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  862,    0,    0,  862,    0,  530,  107,  530,    0,    0,
    0,  530,    0,    0,  530,    0,    0,    0,    0,  862,
    0,    0,    0,    0,    0,  862,  862,  530,    0,    0,
  862,    0,    0,  530,    0,  108,  109,    0,  110,    0,
    0,  862,  862,    0,  862,  111,  112,  113,  114,  115,
  862,  116,    0,    0,  117,  118,  119,  120,  121,    0,
  802,    0,  122,  123,    0,    0,    0,    0,  124,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  862,
  862,    0,  862,    0,    0,    0,    0,    0,    0,  862,
  862,  862,  862,  862,    0,  862,    0,    0,  862,  862,
  862,  862,  862,    0,    0,  530,  862,  862,    0,    0,
  863,  862,  862,    0,    0,    0,  802,    0,    0,  802,
    0,    0,    0,    0,    0,    0,  802,  530,    0,  802,
    0,    0,    0,  802,  530,    0,    0,    0,  802,    0,
    0,  802,    0,    0,    0,  530,    0,    0,    0,    0,
    0,  863,    0,  863,  863,  863,  861,  802,  861,  861,
    0,    0,    0,  530,    0,    0,  861,    0,  530,  863,
  863,  863,  863,    0,    0,    0,    0,    0,    0,    0,
    0,  802,  861,    0,  802,  802,    0,    0,    0,    0,
    0,    0,    0,    0,  802,    0,  861,  530,    0,    0,
    0,  861,  861,  863,    0,  861,    0,    0,    0,    0,
    0,  802,  861,    0,  861,  861,    0,    0,  861,    0,
  803,    0,  861,    0,    0,  861,    0,  861,    0,  861,
    0,    0,    0,  802,  861,  861,    0,  861,  802,    0,
    0,    0,    0,    0,    0,    0,    0,  540,    0,    0,
  802,    0,  802,  861,  861,    0,  861,  530,    0,    0,
    0,  803,    0,    0,  803,    0,    0,    0,    0,    0,
  861,    0,  861,  861,  861,    0,    0,  861,    0,  803,
  861,  861,    0,    0,    0,    0,    0,  540,  540,    0,
  861,  540,    0,    0,    0,    0,    0,    0,  530,    0,
    0,  530,    0,    0,  861,    0,  540,  861,  540,    0,
    0,    0,    0,    0,    0,    0,  530,    0,    0,  802,
    0,    0,    0,  861,    0,    0,    0,    0,    0,  861,
  861,    0,    0,    0,  861,    0,    0,    0,  540,    0,
    0,    0,    0,    0,    0,  861,  861,    0,  861,    0,
    0,    0,    0,    0,  861,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  863,    0,  863,  863,    0,    0,    0,    0,
    0,    0,  863,  861,  861,    0,  861,    0,    0,    0,
    0,    0,    0,  861,  861,  861,  861,  861,  863,  861,
    0,    0,  861,  861,  861,  861,  861,    0,    0,    0,
  861,  861,  863,    0,  866,  861,  861,  863,  863,    0,
    0,  863,    0,    0,    0,    0,    0,    0,  863,    0,
  863,  863,    0,    0,  863,    0,    0,    0,  863,    0,
    0,  863,    0,  863,    0,  863,    0,    0,    0,    0,
  863,  863,    0,  863,    0,  866,    0,  866,  866,  866,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  863,
  863,    0,  863,  866,  866,  866,  866,    0,    0,    0,
    0,    0,  803,    0,    0,    0,  863,    0,  863,  863,
  863,    0,    0,  863,    0,    0,  863,  863,    0,    0,
    0,    0,    0,    0,    0,  540,  863,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  863,  540,    0,  863,  540,  540,    0,  540,    0,    0,
    0,  540,    0,    0,  540,  530,    0,  530,  803,  863,
    0,  803,    0,    0,  530,  863,  863,  540,  803,    0,
  863,  803,    0,  540,    0,  803,  540,  530,    0,    0,
  803,  863,  863,  803,  863,    0,    0,    0,    0,    0,
  863,    0,    0,    0,    0,    0,    0,    0,    0,  803,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  863,
  863,    0,  863,  803,    0,    0,  803,  803,    0,  863,
  863,  863,  863,  863,    0,  863,  803,    0,  863,  863,
  863,  863,  863,    0,    0,  540,  863,  863,    0,    0,
    0,  863,  863,  803,    0,  530,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  540,    0,  841,
    0,    0,    0,    0,  540,  803,    0,  530,  540,    0,
  803,    0,    0,    0,  530,  540,    0,    0,    0,    0,
    0,    0,  803,    0,  803,    0,  866,    0,  866,  866,
    0,    0,    0,  540,    0,    0,  866,    0,  540,    0,
  841, 1008, 1008,  841, 1008, 1008, 1008,    0,  530,    0,
    0,    0,  866,    0,    0,    0,    0,    0,  841, 1008,
 1008, 1008,    0,  540,    0,    0,  866,  540,    0,    0,
    0,  866,  866,    0,    0,  866,    0,    0,    0,    0,
    0,    0,  866,    0,  866,  866,  569,    0,  866,    0,
 1008,  803,  866,    0,    0,  866,    0,  866,    0,  866,
    0,    0,    0,    0,  866,  866,    0,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  866,  866,    0,  866,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  866,    0,  866,  866,  866,    0,    0,  866,    0,    0,
  866,  866,    0,    0,    0,    0,    0,    0,    0,    0,
  866,    0,  938,  705,  703,    0,  704,    0,  706,    0,
    0,    0,    0,    0,  866,    0,    0,  866,    0,    0,
    0,  901,  900,  902,    0,    0,  842,    0,    0,    0,
    0,    0,    0,  866,    0,    0,    0,    0,    0,  866,
  866,    0,    0,    0,  866,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  866,  866,    0,  866,    0,
    0,    0,    0,    0,  866,    0,    0,  842, 1009, 1009,
  842, 1009, 1009, 1009,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  842, 1009, 1009, 1009,    0,
    0,    0,    0,  866,  866,    0,  866,    0,    0,  107,
    0,  841,    0,  866,  866,  866,  866,  866,    0,  866,
    0, 1008,  866,  866,  866,  866,  866, 1009,    0, 1008,
  866,  866,    0,    0, 1008,  866,  866, 1008,  108,  109,
    0,  110,    0,    0,    0,    0,    0,    0,  111,  112,
  113,  114,  115,    0,  116,    0,  843,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,  841,    0,    0,
  841,  124,    0, 1008,    0,    0,    0,  841,    0,    0,
  841,    0, 1008,    0,  841,    0,    0,    0,    0,  841,
 1008,    0,  841,    0,    0,    0,    0,  843, 1010, 1010,
  843, 1010, 1010, 1010,    0,    0,    0,    0,  841, 1008,
    0, 1008,    0,    0,    0,  843, 1010, 1010, 1010,    0,
  833,    0,    0,    0,    0, 1008,    0, 1008, 1008, 1008,
    0,    0,  841,    0,    0,  841,  841,    0,    0,    0,
    0,    0,    0,  888,    0,  841,    0, 1010,    0,    0,
    0,  701,  797,    0,    0,    0,  702,    0,    0,  889,
    0,  833,  841,    0,  833,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1008,  833,
    0,    0,    0,    0,  841,    0,    0,    0,    0,  841,
    0,    0,    0,  797,    0,  890,  797,  798,  842,    0,
    0,  841,    0,  841,  891,    0,    0,    0, 1009,    0,
    0,  797,  892,    0,    0,    0, 1009,    0,    0,    0,
    0, 1009,    0,    0, 1009,    0,    0,    0,    0,    0,
    0,  893,    0,  894,    0,    0,    0,    0,  798,    0,
    0,  798,    0,    0,    0,    0,    0,  895,    0,  896,
  897,  898,    0,    0,  842,    0,  798,  842,    0,    0,
 1009,    0,    0,    0,  842,    0,    0,  842,    0, 1009,
  841,  842,    0,    0,    0,    0,  842, 1009,    0,  842,
  799,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  842, 1009,    0, 1009,    0,
  899,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1009,    0, 1009, 1009, 1009,    0,  843,  842,
    0,  799,  842,  842,  799,    0,    0,    0, 1010,    0,
    0,    0,  842,    0,    0,    0, 1010,    0,    0,  799,
    0, 1010,    0,    0, 1010,    0,    0,    0,    0,  842,
    0,    0,    0,    0,  832,    0,    0,    0,    0,    0,
  826,    0,    0,    0,    0, 1009,    0,    0,    0,    0,
    0,  842,    0,    0,  843,    0,  842,  843,    0,    0,
 1010,    0,  833,    0,  843,    0,    0,  843,  842, 1010,
  842,  843,    0,    0,    0,  832,  843, 1010,  832,  843,
    0,  826,    0,    0,  826,    0,    0,    0,    0,    0,
    0,    0,    0,  832,  797,  843, 1010,    0, 1010,  826,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1010,    0, 1010, 1010, 1010,    0,  833,  843,
    0,  833,  843,  843,    0,    0,    0,    0,  833,    0,
    0,  833,  843,    0,    0,  833,    0,  842,    0,  798,
  833,    0,    0,  833,    0,    0,    0,    0,    0,  843,
  797,    0,    0,  797,    0,    0,    0,    0,    0,  833,
  797,    0,    0,  797,    0, 1010,    0,  797,    0,    0,
    0,  843,  797,    0,    0,  797,  843,    0,    0,    0,
  834,    0,    0,  833,    0,    0,  833,  833,  843,    0,
  843,  797,    0,    0,    0,  798,  833,    0,  798,    0,
  835,    0,    0,    0,    0,  798,    0,    0,  798,    0,
    0,    0,  798,  833,    0,  797,    0,  798,  797,  797,
  798,  834,  799,    0,  834,    0,    0,    0,  797,    0,
    0,    0,    0,    0,    0,  833,  798,    0,    0,  834,
  833,  835,    0,    0,  835,  797,    0,    0,    0,    0,
    0,    0,  833,    0,  833,    0,    0,  843,    0,  835,
  798,    0,    0,  798,  798,    0,    0,  797,    0,    0,
    0,    0,  797,  798,  823,    0,    0,    0,  799,    0,
    0,  799,    0,    0,  797,    0,  797,    0,  799,    0,
  798,  799,    0,    0,    0,  799,  832,    0,    0,    0,
  799,    0,  826,  799,    0,    0,    0,    0,    0,    0,
    0,    0,  798,    0,    0,  823,    0,  798,  823,  799,
    0,  833,    0,    0,    0,    0,    0,    0,    0,  798,
    0,  798,    0,  823,    0,    0,    0,  827,    0,    0,
    0,    0,    0,  799,    0,    0,  799,  799,  836,    0,
    0,    0,  832,  797,    0,  832,  799,    0,  826,    0,
    0,  826,  832,    0,    0,  832,    0,    0,  826,  832,
    0,  826,    0,  799,  832,  826,    0,  832,  827,    0,
  826,  827,    0,  826,    0,    0,    0,    0,    0,  836,
    0,    0,  836,  832,    0,  799,  827,    0,  798,  826,
  799,    0,    0,    0,    0,    0,    0,  836,    0,    0,
    0,  824,  799,    0,  799,    0,    0,  832,    0,    0,
  832,  832,    0,  826,    0,    0,  826,  826,    0,    0,
  832,    0,    0,    0,    0,    0,  826,    0,    0,    0,
    0,    0,  834,    0,    0,    0,    0,  832,    0,    0,
    0,    0,  824,  826,    0,  824,    0,    0,    0,    0,
    0,    0,  835,    0,    0,    0,    0,    0,    0,  832,
  824,    0,    0,    0,  832,  826,    0,    0,    0,    0,
  826,  799,    0,    0,    0,  828,  832,    0,  832,    0,
    0,    0,  826,    0,  826,    0,    0,    0,  834,    0,
    0,  834,    0,    0,    0,  776,    0,    0,  834,    0,
    0,  834,    0,    0,    0,  834,    0,    0,  835,    0,
  834,  835,    0,  834,    0,    0,  828,    0,  835,  828,
    0,  835,    0,    0,    0,  835,  823,    0,    0,  834,
  835,    0,    0,  835,  828,    0,  776,    0,    0,  776,
    0,    0,    0,    0,    0,  832,    0,    0,    0,  835,
    0,  826,    0,  834,  776,    0,  834,  834,    0,    0,
    0,    0,    0,    0,    0,    0,  834,    0,    0,    0,
    0,    0,    0,  835,    0,    0,  835,  835,    0,    0,
    0,    0,  823,  834,    0,  823,  835,    0,    0,  827,
    0,    0,  823,    0,    0,  823, 1252,    0,    0,  823,
  836,    0,    0,  835,  823,  834,    0,  823,    0,    0,
  834,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  834,  823,  834,  835,    0,    0,    0,    0,
  835,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  835,    0,  835,  827,    0,  823,  827,    0,
  823,  823,    0,    0,    0,  827,  836,    0,  827,  836,
  823,    0,  827,  824,    0,    0,  836,  827,    0,  836,
  827,    0,    0,  836,    0,    0,    0,  823,  836,    0,
    0,  836,    0,    0,    0,  657,  827,    0,  429,    0,
  430,  834,    0,    0,    0,    0,    0,  836,    0,  823,
    0,    0,    0,  432,  823,    0,    0,    0,  288,    0,
  827,  835,    0,  827,  827,    0,  823,    0,  823,  824,
    0,  836,  824,  827,  836,  836,    0,    0,    0,  824,
    0,    0,  824,    0,  836,    0,  824,  828,    0,    0,
  827,  824,    0,    0,  824,    0,    0,    0,    0,    0,
    0,  836,    0,    0,    0,    0,    0,    0,    0,    0,
  824,    0,  827,    0,    0,    0,    0,  827,    0,    0,
    0,    0,    0,  836,    0,    0,    0,    0,  836,  827,
    0,  827,    0,    0,  824,  823,    0,  824,  824,    0,
  836,    0,  836,  828,    0,    0,  828,  824,    0,    0,
    0,    0,    0,  828,    0,    0,  828,    0,    0,    0,
  828,    0,    0,  776,  824,  828,  776,    0,  828,    0,
    0,    0,    0,  776,    0,    0,  776,    0,    0,    0,
  776,    0,    0,    0,  828,  776,  824,    0,  776,    0,
    0,  824,    0,    0,    0,    0,  396,    0,  827,    0,
    0,    0,    0,  824,  776,  824,    0,    0,  828,  836,
    0,  828,  828,    0,    0,    0,    0,    0,    0,    0,
    0,  828,    0,    0,  398,    0,    0,    0,  776,    0,
    0,  776,  776,    0,    0,    0,    0,    0,  828,    0,
    0,  776,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,  776,    0,
  828,    0,  393,    0,    0,  828,    0,    0,    0,    0,
    0,  394,  824,    0,    0,    0,    0,  828,    0,  828,
  776,    0,    0,    0,  395,  776,    0,    0,    0,  657,
  396,  397,  429,    0,  430,    0,    0,  776,    0,  776,
    0,    0,    0,    0,    0,    0,    0,  432,    0,  651,
    0,    0,  288,    0,    0,    0,    0,    0,  398,    0,
    0,  399,    0,    0,    0,    0,    0,    0,  405,    0,
    0,    0,    0,    0,    0,  705,  703,    0,  704,    0,
  706,    0,    0,    0,  407,    0,  828,  400,    0,  401,
  402,    0,    0,  901,  900,  902,    0,    0,  408,  409,
  410,  403,  211,  212,    0,  404,  776,    0,    0,    0,
    0,    0,    0,  880,    0,  657,    0,    0,  429,    0,
  430,  413,  414,  415,  416,  417,  418,  419,  420,  421,
    0,    0,    0,  432,    0,    0,  423,  424,  288,    0,
    0,    0,    0,    0,    0,  428,    0,    0,    0,    0,
    0,    0,  694,    0,    0,    0,  653,    0,    0,    0,
    0,    0,  405,    0,    0,    0,    0,    0,  406,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  408,  409,  410,  411,  211,  212,    0,    0,
    0,    0,    0,    0,    0,    0,  412,    0,    0,    0,
    0,    0,    0,    0,    0,  413,  414,  415,  416,  417,
  418,  419,  420,  421,  108,  422,    0,  110,    0,    0,
  423,  424,    0,  425,  426,  427,  113,  114,  115,  428,
  116,    0,    0,  117,  118,  119,  120,  121,  881,  882,
  883,  122,  123,    0,    0,    0,  393,  124,    0,    0,
    0,    0,    0,    0,    0,  394,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,  650,    0,    0,  396,  397,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  651,    0,  888,    0,    0,    0,    0,
    0,    0,  398,  701,    0,  399,    0,    0,  702,    0,
    0,  889,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  400,  393,  401,  402,  657,    0,    0,  429,    0,
  430,  394,    0,    0,    0,  403,    0,  890,    0,  404,
    0,    0,    0,  432,  395,    0,  891,  652,  288,    0,
  396,  397,    0,    0,  892,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  651,
    0,    0,    0,  893,    0,  894,    0,    0,  398,    0,
    0,  399,    0,    0,    0,    0,    0,    0,    0,  895,
  653,  896,  897,  898,    0,    0,  405,    0,    0,    0,
    0,    0,  406,    0,    0,    0,    0,  400,    0,  401,
  402,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,  403,    0,    0,    0,  404,  408,  409,  410,  411,
  211,  212,    0,  876,    0,    0,    0,    0,    0,    0,
  412,    0,  899,    0,    0,    0,    0,    0,    0,  413,
  414,  415,  416,  417,  418,  419,  420,  421,  108,  422,
    0,  110,    0,    0,  423,  424,    0,  425,  426,  427,
  113,  114,  115,  428,  116,    0,  653,  117,  118,  654,
  655,  656,  405,    0,    0,  122,  123,    0,  406,    0,
    0,  124,    0,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  408,  409,  410,  411,  211,  212,    0,    0,
    0,    0,    0,    0,    0,    0,  412,    0,    0,    0,
    0,    0,    0,    0,    0,  413,  414,  415,  416,  417,
  418,  419,  420,  421,  108,  422,    0,  110,    0,    0,
  423,  424,  393,  425,  426,  427,  113,  114,  115,  428,
  116,  394,    0,  117,  118,  654,  655,  656,    0,    1,
    0,  122,  123,  657,  395,    0,  429,  124,  430,    0,
  396,  397,    0,    0,    0,    0,    0,    0,    0,    0,
    2,  432,    0,    0,    0,    0,  288,    0,    3,  651,
    0,    0,    0,    0,    0,    4,    0,    5,  398,    6,
    0,  399,    0,    0,    7,    0,    0,    0,    0,    0,
    0,    8,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    9,    0,  400,    0,  401,
  402,    0,    0,    0,    0,    0,   10,    0,    0,    0,
    0,  403,    0,    0,    0,  404,    0,    0,  594,    0,
    0,    0,  595,  652,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  499,    0,    0,  429,    0,  430,    0,
  596,    0,    0,  597,    0,    0,    0,    0,    0,    0,
    0,  432,    0,    0,    0,    0,  288,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  653,    0,    0,    0,
    0,    0,  405,   11,    0,    0,    0,    0,  406,    0,
   12,    0,    0,   13,  598,   14,   15,    0,  407,    0,
  599,  600,  601,    0,  602,  603,  604,    0,  605,    0,
    0,    0,  408,  409,  410,  411,  211,  212,    0,    0,
   16,    0,    0,    0,    0,    0,  412,    0,  606,    0,
  607,    0,    0,    0,    0,  413,  414,  415,  416,  417,
  418,  419,  420,  421,  108,  422,    0,  110,    0,    0,
  423,  424,    0,  425,  426,  427,  113,  114,  115,  428,
  116,  608,    0,  117,  118,  654,  655,  656,    0,   17,
  393,  122,  123,    0,    0,    0,    0,  124,  609,  394,
    0,    0,  107,    0,   18,   19,    0,    0,    0,    0,
    0,    0,  395,    0,    0,    0,    0,    0,  396,  397,
  610,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  108,  109,    0,  110,    0,    0,  651,    0,    0,
    0,  111,  112,  113,  114,  115,  398,  116,    0,  399,
  117,  118,  119,  120,  121,  611,  612,    0,  122,  123,
    0,    0,    0,    0,  124,    0,    0,    0,    0,    0,
    0,    0,  613,    0,    0,  400,    0,  401,  402,    0,
    0,    0,    0, 1122,    0,    0, 1123,    0,    0,  403,
  393,    0,    0,  404,    0,    0,    0,    0,    0,  394,
    0,  876,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,    0,    0,    0,  396,  397,
    0,    0,    0,    0,  499,    0,    0,  429,    0,  430,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  432,    0,  653,    0,  398,  288,    0,  399,
  405,    0,    0,    0,    0,    0,  406,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,    0,    0,    0,  400,    0,  401,  402,    0,
  408,  409,  410,  411,  211,  212,    0,    0,    0,  403,
    0,    0,    0,  404,  412,    0,    0,    0,    0,    0,
    0,    0,    0,  413,  414,  415,  416,  417,  418,  419,
  420,  421,  108,  422,    0,  110,    0,  100,  423,  424,
    0,  425,  426,  427,  113,  114,  115,  428,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123,    0,    0,    0,    0,  124,    0,    0, 1124,    0,
  405,    0,    0,    0,    0,    0,  406,    0,  101,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,    0,
  102,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,  409,  410,  411,  211,  212,    0,    0,    0,    0,
    0,    0,    0,    0,  412,  103,    0,    0,    0,    0,
    0,    0,    0,  413,  414,  415,  416,  417,  418,  419,
  420,  421,  108,  422,    0,  110,    0,    0,  423,  424,
    0,  425,  426,  427,  113,  114,  115,  428,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123,    0,    0,    0, 1132,  124,    0, 1123,    0,    0,
    0,  393,    0,    0,    0,    0,    0,    0,    0,    0,
  394,    0,    0,    0,  104,    0,    0,    0,    0,    0,
    0,    0,    0,  395,  105,    0,    0,    0,    0,  396,
  397,    0,  499,    0,    0,  429,    0,  430,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  106,
  432,    0,    0,    0,    0,  288,    0,  398,    0,    0,
  399,    0,    0,    0,    0,    0,    0,  107,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  400,    0,  401,  402,
    0,    0,    0,    0,    0,    0,  108,  109,    0,  110,
  403,    0,    0,    0,  404,    0,  111,  112,  113,  114,
  115,    0,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,    0,  124,
  499,    0,    0,  429,    0,  430,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  432,    0,
    0,    0,    0,  288,    0,    0,    0,    0,    0, 1124,
    0,  405,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,  409,  410,  411,  211,  212,    0,    0,    0,
    0,    0,    0,    0,    0,  412,    0,    0,    0,    0,
    0,    0,    0,    0,  413,  414,  415,  416,  417,  418,
  419,  420,  421,  108,  422,    0,  110,    0,    0,  423,
  424,    0,  425,  426,  427,  113,  114,  115,  428,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,    0,
  122,  123, 1136,    0,    0, 1123,  124,    0,    0,  393,
    0,    0,    0,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,  396,  397,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  204,    0,
    0,    0,  204,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  398,  204,    0,  399,    0,
  204,    0,    0,  204,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  400,    0,  401,  402,    0,    0,
 1144,    0,    0, 1123,    0,    0,    0,  393,  403,    0,
    0,    0,  404,    0,  204,    0,  394,    0,    0,    0,
  204,  204,  204,    0,  204,  204,  204,    0,  204,  395,
    0,    0,    0,    0,    0,  396,  397,    0,  499,    0,
    0,  429,    0,  430,    0,    0,    0,    0,  204,    0,
  204,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,  288,    0,  398,    0,    0,  399, 1124,    0,  405,
    0,    0,    0,    0,    0,  406,    0,    0,    0,    0,
    0,  204,    0,    0,    0,  407,   81,    0,    0,    0,
    0,    0,  400,    0,  401,  402,    0,    0,  204,  408,
  409,  410,  411,  211,  212,    0,  403,    0,    0,    0,
  404,    0,    0,  412,    0,    0,    0,    0,    0,    0,
  204,    0,  413,  414,  415,  416,  417,  418,  419,  420,
  421,  108,  422,    0,  110,    0,    0,  423,  424,    0,
  425,  426,  427,  113,  114,  115,  428,  116,    0,    0,
  117,  118,  119,  120,  121,  204,  204,    0,  122,  123,
    0,    0,    0,    0,  124, 1124,    0,  405,    0,    0,
    0,    0,  204,  406,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,  409,  410,
  411,  211,  212,    0,    0,    0,    0,    0,    0,    0,
    0,  412,    0,    0,    0,    0,    0,    0,    0,    0,
  413,  414,  415,  416,  417,  418,  419,  420,  421,  108,
  422,    0,  110,    0,    0,  423,  424,    0,  425,  426,
  427,  113,  114,  115,  428,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123, 1147,    0,
    0, 1123,  124,    0,    0,  393,    0,    0,    0,    0,
    0,    0,    0,    0,  394,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,  396,  397,    0,  499,    0,    0,  429,
    0,  430,    0,    0,  866,    0,    0,    0,  595,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,  288,
    0,  398,  867,    0,  399,    0,  596,    0,    0,  597,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,  401,  402,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  403,    0,    0,    0,  404,    0,
  598,    0,    0,    0,    0,    0,  599,  600,  601,    0,
  602,  603,  604,    0,  605,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  499,    0,    0,  429,    0,  430,
    0,    0,    0,    0,  606,    0,  607,    0,    0,    0,
    0,    0,  432,    0,    0,    0,    0,  288,    0,    0,
    0,    0,    0, 1124,    0,  405,    0,    0,    0,    0,
    0,  406,    0,    0,    0,    0,    0,  608,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  609,  408,  409,  410,  411,  211,
  212,    0,    0,    0,    0,    0,    0,    0,    0,  412,
    0,    0,    0,    0,    0,    0,  610,    0,  413,  414,
  415,  416,  417,  418,  419,  420,  421,  108,  422,    0,
  110,    0,    0,  423,  424,    0,  425,  426,  427,  113,
  114,  115,  428,  116,    0,    0,  117,  118,  119,  120,
  121,  611,  612,    0,  122,  123, 1152,    0,    0, 1123,
  124,    0,    0,  393,    0,    0,    0,    0,  613,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,  396,  397,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  204,    0,    0,    0,  204,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  398,
  204,    0,  399,    0,  204,    0,    0,  204,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  400,    0,
  401,  402,    0,    0, 1155,    0,    0, 1123,    0,    0,
    0,  393,  403,    0,    0,    0,  404,    0,  204,    0,
  394,    0,    0,    0,  204,  204,  204,    0,  204,  204,
  204,    0,  204,  395,    0,    0,    0,    0,    0,  396,
  397,    0,  499,    0,    0,  429,    0,  430,    0,    0,
    0,    0,  204,    0,  204,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,  288,    0,  398,    0,    0,
  399, 1124,    0,  405,    0,    0,    0,    0,    0,  406,
    0,    0,    0,    0,    0,  204,    0,    0,    0,  407,
    0,    0,    0,    0,    0,    0,  400,    0,  401,  402,
    0,    0,  204,  408,  409,  410,  411,  211,  212,    0,
  403,    0,    0,    0,  404,    0,    0,  412,    0,    0,
    0,    0,    0,    0,  204,    0,  413,  414,  415,  416,
  417,  418,  419,  420,  421,  108,  422,    0,  110,    0,
    0,  423,  424,    0,  425,  426,  427,  113,  114,  115,
  428,  116,    0,    0,  117,  118,  119,  120,  121,  204,
  204,    0,  122,  123,    0,    0,    0,    0,  124, 1124,
    0,  405,    0,    0,    0,    0,  204,  406,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,  409,  410,  411,  211,  212,    0,    0,    0,
    0,    0,    0,    0,    0,  412,    0,    0,    0,    0,
    0,    0,    0,    0,  413,  414,  415,  416,  417,  418,
  419,  420,  421,  108,  422,    0,  110,    0,    0,  423,
  424,    0,  425,  426,  427,  113,  114,  115,  428,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,    0,
  122,  123, 1158,    0,  475, 1123,  124,    0,    0,  393,
    0,    0,    0,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  107,  395,    0,    0,    0,    0,    0,  396,  397,    0,
  654,    0,  654,  654,    0,  654,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  654,  108,
  109,    0,  110,  654,    0,  398,    0,    0,  399,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,    0,  400,    0,  401,  402,    0,    0,
    0,    0,    0,    0,    0,    0,  975,    0,  403,    0,
  975,    0,  404,    0,    0,    0,    0,    0,    0,    0,
  648,    0,  648,  648,    0,  648,  977,    0,  975,    0,
    0,  975,    0,    0,    0,    0,    0,    0,  648,    0,
    0,    0,    0,  648,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1124,    0,  405,
    0,    0,  975,    0,    0,  406,    0,    0,  975,  975,
  975,    0,  975,  975,  975,  407,  975,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  408,
  409,  410,  411,  211,  212,    0,  975,    0,  975,    0,
    0,    0,    0,  412,    0,    0,    0,    0,    0,    0,
    0,    0,  413,  414,  415,  416,  417,  418,  419,  420,
  421,  108,  422,    0,  110,    0,    0,  423,  424,  975,
  425,  426,  427,  113,  114,  115,  428,  116,    0,    0,
  117,  118,  119,  120,  121,    0,  975,    0,  122,  123,
  654,    0,    0,    0,  124,    0,    0,  654,    0,    0,
    0,    0,    0,    0,    0,    0,  654,    0,  975,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  654,
    0,    0,    0,    0,    0,  654,  654,    0,    0,    0,
    0,    0,    0,  654,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  975,  975,    0,    0,    0,    0,    0,
    0,    0,    0,  654,    0,    0,  654,    0,    0,    0,
  975,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  648,    0,  479,    0,    0,    0,    0,  648,    0,    0,
    0,    0,  654,    0,  654,  654,  648,    0,    0,  107,
    0,    0,    0,    0,    0,    0,  654,    0,    0,  648,
  654,    0,    0,    0,  654,  648,  648,  654,    0,  654,
    0,    0,    0,  648,    0,    0,    0,    0,  108,  109,
    0,  110,  654,    0,    0,    0,    0,  654,  111,  112,
  113,  114,  115,  648,  116,    0,  648,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,    0,
    0,  124,    0,    0,    0,    0,    0,  654,    0,    0,
    0,    0,  648,  654,  648,  648,    0,    0,    0,    0,
    0,    0,    0,  654,    0,    0,  648,    0,    0,    0,
  648,    0,    0,    0,    0,    0,    0,  654,  654,  654,
  654,  654,  654,    0,    0,    0,    0,    0,    0,    0,
    0,  654,    0,    0,    0,    0,    0,  100,    0,    0,
  654,  654,  654,  654,  654,  654,  654,  654,  654,  654,
  654,    0,  654,    0,    0,  654,  654,    0,  654,  654,
  654,  654,  654,  654,  654,  654,    0,  648,  654,  654,
  654,  654,  654,  648,    0,    0,  654,  654,  101,    0,
    0,    0,  654,  648,    0,    0,    0,    0,    0,    0,
  102,    0,    0,    0,    0,    0,    0,  648,  648,  648,
  648,  648,  648,    0,  137,    0,    0,    0,    0,    0,
    0,  648,    0,    0,    0,  103,    0,    0,    0,    0,
  648,  648,  648,  648,  648,  648,  648,  648,  648,  648,
  648,    0,  648,    0,    0,  648,  648,    0,  648,  648,
  648,  648,  648,  648,  648,  648,    0,    0,  648,  648,
  648,  648,  648,    0,  654,    0,  648,  648,    0,    0,
    0,  654,  648,    0,  499,    0,  504,  429,    0,  430,
  654,    0,    0,  573,    0,    0,    0,    0,    0,    0,
    0,    0,  432,  654,  104,    0,    0,  288,    0,  654,
  654,    0,    0,    0,  105,    0,    0,  654,    0,    0,
    0,    0,  573,  573,    0,  573,    0,    0,    0,    0,
    0,    0,  573,  573,  573,  573,  573,  654,  573,  106,
  654,  573,  573,  573,  573,  573,    0,    0,    0,  573,
  573,    0,    0,    0,    0,  573,    0,  107,    0,    0,
    0,    0,    0,    0,    0,    0,  654,    0,  654,  654,
  499,    0,    0,  429,    0,  430,    0,    0,    0,    0,
  654,    0,    0,    0,  654,    0,  108,  109,  432,  110,
    0,    0,    0,  288,  273,    0,  111,  112,  113,  114,
  115,    0,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,    0,  124,
    0,    0,    0,  273,  273,    0,  273,    0,    0,    0,
    0,    0,    0,  273,  273,  273,  273,  273,    0,  273,
    0,  654,  273,  273,  273,  273,  273,  654,    0,    0,
  273,  273,    0,    0,    0,    0,  273,  654,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  654,  654,  654,  654,  654,  654,    0,    0,    0,
    0,    0,    0,  134,    0,  654,    0,  134,    0,    0,
    0,    0,    0,    0,  654,  654,  654,  654,  654,  654,
  654,  654,  654,  654,  654,  134,  654,    0,  134,  654,
  654,  393,  654,  654,  654,  654,  654,  654,  654,  654,
  394,    0,  654,  654,  654,  654,  654,    0,    0,    0,
  654,  654,    0,  395,    0,    0,  654,    0,    0,  396,
  397,    0,    0,    0,    0,    0,    0,    0,    0,  134,
    0,    0,    0,    0,    0,  134,  134,  134,    0,  134,
  134,  134,    0,  134,    0,    0,    0,  398,    0,    0,
  399,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  134,    0,  134,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  393,  401,  402,
  499,    0,    0,  429,    0,  430,  394,    0,    0,    0,
  403,    0,    0,    0,  404,    0,  134,    0,  432,  395,
    0,    0,    0,  288,    0,  396,  397,    0,    0,    0,
    0,    0,    0,  134,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  398,    0,  134,  399,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  405,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,  400,    0,  401,  402,    0,  407,    0,    0,
  134,  134,    0,    0,    0,    0,  403,    0,    0,    0,
  404,  408,  409,  410,  411,  211,  212,  134,    0,    0,
    0,    0,    0,    0,    0,  412,    0,    0,    0,    0,
    0,    0,    0,    0,  413,  414,  415,  416,  417,  418,
  419,  420,  421,  108,  422,    0,  110,    0,    0,  423,
  424,    0,  425,  426,  427,  113,  114,  115,  428,  116,
    0,    0,  117,  118,  119,  120,  121,  405,    0,    0,
  122,  123,    0,  406,    0,    0,  124,    0,    0,    0,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,  686,    0,    0,    0,    0,    0,  408,  409,  410,
  411,  211,  212,    0,    0,    0,    0,    0,    0,    0,
    0,  412,    0,    0,    0,    0,    0,    0,    0,    0,
  413,  414,  415,  416,  417,  418,  419,  420,  421,  108,
  422,    0,  110,    0,    0,  423,  424,  393,  425,  426,
  427,  113,  114,  115,  428,  116,  394,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,  395,
    0,    0,  124,    0,    0,  396,  397,  499,  941,    0,
  429,    0,  430,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  432,    0,    0,    0,    0,
  288,    0,    0,  398,    0,    0,  399,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  400,    0,  401,  402,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  403,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,  499,    0,    0,
  429,    0,  430,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  432,    0,    0,    0,    0,
  288,    0,    0,    0,  964,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  694,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  405,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,  409,  410,
  411,  211,  212,    0,    0,    0,    0,    0,    0,    0,
    0,  412,    0,    0,    0,    0,    0,    0,  965,    0,
  413,  414,  415,  416,  417,  418,  419,  420,  421,  108,
  422,    0,  110,  964,    0,  423,  424,    0,  425,  426,
  427,  113,  114,  115,  428,  116,    0,    0,  117,  118,
  119,  120,  121,    0,  966,    0,  122,  123,    0,  967,
    0,    0,  124,    0,  393,  968,    0,    0,    0,    0,
    0,    0,    0,  394,    0,    0,  107,    0,    0,    0,
    0,    0,    0,    0,  969,    0,  395,    0,    0,    0,
    0,    0,  396,  397,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  108,  109,    0,  110,    0,
    0,    0,    0,    0,    0,  111,  112,  113,  114,  115,
  398,  116,    0,  399,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,    0,
    0,    0,    0,    0,  393,    0,    0,    0,  967,  400,
    0,  401,  402,  394,    0,    0,  499,    0,    0,  429,
    0,  430,    0,  403,    0,  107,  395,  404,    0,    0,
    0,    0,  396,  397,  432,    0,    0,    0,    0,  288,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  108,  109,    0,  110,    0,    0,
  398,    0,    0,  399,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117,  118,  119,  120,  121,    0,    0,
    0,  122,  123,    0,  405,    0,    0,  124,    0,  400,
  406,  401,  402,    0,    0,    0,    0,    0,    0,    0,
  407,    0,    0,  403,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,  408,  409,  410,  411,  211,  212,
    0,    0,    0,    0,    0,    0,    0,    0,  412,    0,
    0,    0,    0,    0,    0,    0,    0,  413,  414,  415,
  416,  417,  418,  419,  420,  421,  108,  422,    0,  110,
    0,    0,  423,  424,    0,  425,  426,  427,  113,  114,
  115,  428,  116,    0,  405,  117,  118,  119,  120,  121,
  406,    0,    0,  122,  123,    0,    0,    0,    0,  124,
  407,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1150,    0,    0,  408,  409,  410,  411,  211,  212,
    0,    0,    0, 1713,    0,    0,    0,    0,  412,    0,
    0,    0,    0,    0,    0,    0,    0,  413,  414,  415,
  416,  417,  418,  419,  420,  421,  108,  422, 1714,  110,
    0,    0,  423,  424,    0,  425,  426,  427,  113,  114,
  115,  428,  116,  393,    0,  117,  118,  119,  120,  121,
    0,    0,  394,  122,  123,  499,    0,    0,  429,  124,
  430,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,  396,  397,  432,    0,    0,    0,    0,  288,    0,
    0,    0,    0,    0,    0,    0, 1715,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  398,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  107,    0,    0,    0,    0,    0,    0,    0,  400,    0,
  401,  402,    0,    0,    0,  499,    0,    0,  429,    0,
  430,    0,  403,    0,    0,    0,  404,    0,    0,  108,
  109,    0,  110,  432, 1685,    0,    0,    0,  288,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1338,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  405,    0,    0,    0,    0,    0,  406,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  407,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  408,  409,  410,  411,  211,  212,    0,
    0,    0,    0,    0,    0,    0,    0,  412,    0,    0,
    0,    0,    0,    0,    0,    0,  413,  414,  415,  416,
  417,  418,  419,  420,  421,  108,  422,    0,  110,    0,
    0,  423,  424,    0,  425,  426,  427,  113,  114,  115,
  428,  116,  393,    0,  117,  118,  119,  120,  121,    0,
    0,  394,  122,  123,    0,    0,    0,    0,  124,    0,
    0,    0,    0,    0,  395,    0,    0,    0,    0,    0,
  396,  397,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  398,    0,
    0,  399,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  393,    0,    0,    0,    0,  400,    0,  401,
  402,  394,    0,    0,  969,    0,  969,  969,    0,  969,
    0,  403,    0,    0,  395,  404,    0,    0,    0,    0,
  396,  397,  969,    0,    0,    0,    0,  969,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  398,    0,
    0,  399,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  405,    0,    0,    0,    0,  400,  406,  401,
  402,    0,    0,    0,    0,    0,    0,    0,  407,    0,
    0,  403,    0,    0,    0,  404,    0,    0,    0, 1351,
    0,    0,  408,  409,  410,  411,  211,  212,    0,    0,
    0,    0,    0,    0,    0,    0,  412,    0,    0,    0,
    0,    0,    0,    0,    0,  413,  414,  415,  416,  417,
  418,  419,  420,  421,  108,  422, 1197,  110,    0,    0,
  423,  424,    0,  425,  426,  427,  113,  114,  115,  428,
  116,    0,  405,  117,  118,  119,  120,  121,  406,    0,
    0,  122,  123,    0,    0,    0,    6,  124,  407,    0,
    0,    0,    0,    0,    0,    0,    0, 1395, 1396,    0,
 1397,    0,  408,  409,  410,  411,  211,  212,    0,    0,
    0,    0,    0,    0,    0,    0,  412, 1398,    0,    0,
    0,    0,    0, 1399,    0,  413,  414,  415,  416,  417,
  418,  419,  420,  421,  108,  422,    0,  110,    0,    0,
  423,  424,    0,  425,  426,  427,  113,  114,  115,  428,
  116,  969,    0,  117,  118,  119,  120,  121,    0,    0,
  969,  122,  123,  431,    0,    0,  429,  124,  430,    0,
    0,    0,    0,  969,    0,    0,    0,    0,    0,  969,
  969,  432,    0, 1400,    0,    0,  288,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   14,    0,    0,    0,    0,  969,    0,    0,
  969,    0,    0,    0,    0,    0,    0, 1401,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,    0,    0,
    0,    0,    0,    0,    0,    0,  969,    0,  969,  969,
    0,    0,    0,  499,    0,  107,  429,    0,  430,    0,
  969,    0,    0,    0,  969,    0,    0,    0,    0,    0,
    0,  432,    0,    0,    0,    0,  288,    0,    0,    0,
    0,    0,    0,    0, 1402,  109,    0,  110,    0,    0,
    0,    0,    0,    0,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117, 1403,  119,  120,  121,    0,    0,
    0,  122,  123, 1404, 1405, 1406,    0,  124,    0,    0,
    0,  969,    0,    0,    0,    0,    0,  969,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  969,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  969,  969,  969,  969,  969,  969,    0,    0,    0,
    0,    0,    0,    0,    0,  969,    0,    0,    0,    0,
    0,    0,    0,    0,  969,  969,  969,  969,  969,  969,
  969,  969,  969,  969,  969,    0,  969,    0,    0,  969,
  969,    0,  969,  969,  969,  969,  969,  969,  969,  969,
  393,    0,  969,  969,  969,  969,  969,    0,    0,  394,
  969,  969,    0,    0,    0,    0,  969,    0,    0,    0,
    0,    0,  395,    0,    0,    0,    0,    0,  396,  397,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  398,    0,    0,  399,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  393,    0,    0,    0,    0,  400,    0,  401,  402,  394,
    0,    0,  969,    0,    0,  969,    0,  969,    0,  403,
    0,    0,  395,  404,    0,    0,    0,    0,  396,  397,
  969,    0,    0,    0,    0,  969,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  398,    0,    0,  399,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  405,    0,    0,    0,    0,  400,  406,  401,  402,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,  403,
    0,    0,    0,  404,    0,    0,    0,    0,    0,    0,
  408,  409,  410,  411,  211,  212,    0,    0,    0,    0,
    0,    0,    0,    0,  412,    0,    0,    0,    0,    0,
    0,    0,    0,  413,  414,  415,  416,  417,  418,  419,
  420,  421,  108,  422,    0,  110,    0,    0,  423,  424,
    0,  425,  426,  427,  113,  114,  115,  428,  116,    0,
  405,  117,  118,  119,  120,  121,  406,    0,    0,  122,
  123,    0,    0,    0,    0,  124,  407,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,  409,  410,  411,  211,  212,    0,    0,    0,    0,
    0,    0,    0,    0,  412,    0,    0,    0,    0,    0,
    0,    0,    0,  413,  414,  415,  416,  417,  418,  419,
  420,  421,  108,  422,    0,  110,    0,    0,  423,  424,
    0,  425,  426,  427,  113,  114,  115,  428,  116,  969,
  541,  117,  118,  119,  120,  121,    0,    0,  969,  122,
  123,    0,  535,    0,    0,  124,    0,    0,    0,    0,
    0,  969,    0,    0,  557,    0,    0,  969,  969,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  541,  541,    0,    0,  541,    0,    0,    0,    0,    0,
    0,    0,    0,  535,    0,  969,  535,    0,  969,  541,
    0,  541,    0,    0,    0,  557,    0,    0,  557,    0,
    0,  535,    0,  535,    0,    0,    0,  547,    0,    0,
    0,    0,    0,  557,  969,  557,  969,  969,    0,  532,
    0,  541,    0,    0,    0,    0,    0,    0,  969,    0,
    0,    0,  969,  535,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  557,    0,    0,  547,    0,
    0,  547,    0,    0,    0,    0,    0,    0,    0,    0,
  532,    0,    0,  532,    0,    0,  547,    0,  547,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  532,    0,
  532,    0,    0,    0,    0,    0,    0,    0,    0,  969,
    0,    0,    0,    0,    0,  969,    0,    0,  547,    0,
    0,    0,    0,    0,    0,  969,    0,    0,    0,    0,
  532,    0,    0,    0,    0,    0,    0,    0,    0,  969,
  969,  969,  969,  969,  969,    0,    0,    0,    0,    0,
    0,    0,    0,  969,    0,    0,    0,    0,    0,    0,
    0,    0,  969,  969,  969,  969,  969,  969,  969,  969,
  969,  969,  969,    0,  969,    0,    0,  969,  969,    0,
  969,  969,  969,  969,  969,  969,  969,  969,    0,    0,
  969,  969,  969,  969,  969,    0,    0,    0,  969,  969,
    0,    0,    0,    0,  969,    0,    0,    0,  541,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  535,    0,    0,    0,  541,    0,    0,  541,  541,    0,
  541,    0,  557,    0,  541,    0,  535,  541,    0,  535,
  535,    0,  535,    0,    0,    0,  535,    0,  557,  535,
  541,    0,  557,    0,  557,    0,  541,    0,  557,  541,
    0,  557,  535,    0,    0,    0,    0,    0,  535,    0,
    0,  535,    0,    0,  557,    0,    0,    0,    0,    0,
  557,    0,    0,  557,    0,  547,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  532,    0,    0,
    0,  547,    0,    0,    0,  547,    0,  547,    0,    0,
    0,  547,    0,  532,  547,    0,    0,  532,    0,  532,
    0,    0,    0,  532,    0,    0,  532,  547,  541,    0,
    0,    0,    0,  547,    0,  313,  547,    0,    0,  532,
  535,    0,    0,    0,    0,  532,    0,    0,  532,    0,
  541,    0,  557,    0,    0,    0,    0,  541,    0,    0,
    0,  541,  535,    0,    0,  313,    0,    0,  541,  535,
    0,    0,  313,  535,  557,    0,  313,  313,    0,  313,
  535,  557,  313,    0,    0,  557,  541,    0,    0,    0,
    0,  541,  557,    0,    0,    0,  313,    0,  535,    0,
    0,    0,  313,  535,    0,  547,    0,    0,    0,    0,
  557,    0,    0,    0,    0,  557,  541,  532,    0,    0,
  541,    0,    0,    0,    0,    0,    0,  547,  535,    0,
    0,    0,  535,    0,  547,    0,    0,    0,  547,  532,
  557, 1197,    0,    0,  557,  547,  532,    0,    0,    0,
  532,    0,    0,    0,    0,    0,    0,  532,    0,    0,
    0,    0,  313,  547,    0,    0,    0,    0,  547,    0,
    0,    6,    0,    0,    0,  532,    0,    0,    0,    0,
  532,  313, 1395, 1396,    0, 1397,    0,    0,    0,    0,
    0,    0,    0,  547,    0,    0,  313,  547,    0,    0,
    0,    0, 1398,    0,    0,  532,  313,    0, 1399,  532,
    0,    0,    0,    0,    0,  313,    0,  313,    0,    0,
    0,    0,    0,    0,  313,    0,    0,    0,    0,    0,
    0,    0,    0,  594,    0,    0,    0,  595,    0,    0,
    0,    0,    0,    0,  793,    0,    0,    0,    0,    0,
    0,    0,    0,  313,  313,  596,  313,    0,  597,    0,
    0,    0,    0,  313,  313,  313,  313,  313, 1400,  313,
    0,    0,  313,  313,  313,  313,  313,    0,    0,    0,
  313,  313,  313,  313,  313,    0,  313,   14,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  598,
    0,    0, 1401,    0,    0,  599,  600,  601,    0,  602,
  603,  604,   16,  605,    0,    0,    0,    0,    0,    0,
    0, 1589,    0,    0,    0,    0,    0,    0,    0,    0,
  107,    0,    0,  606,    0,  607,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1402,
  109,    0,  110,    0,    0,    0,  608,    0,    0,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117, 1403,
  119,  120,  121,  609,    0,    0,  122,  123, 1404, 1405,
 1406,    0,  124,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  610,    0,  594,    0,    0,
    0,  595,    0,    0,    0,    0,    0,    0,    0,    0,
  107,    0,    0,    0,    0,    0,    0,    0,    0,  596,
    0,    0,  597,    0,    0,    0,    0,    0,    0,    0,
  611,  612,    0,    0,    0,    0,    0,    0,    0,  108,
  109,    0,  110,    0,    0,    0,    0,  613,    0,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117,  118,
  119,  120,  121,  598,    0,    0,  122,  123,    0,  599,
  600,  601,  124,  602,  603,  604,    0,  605,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  107,    0,    0,    0,    0,  606,    0,  607,
    0,  167,  168,  169,  170,  171,  172,  173,  174,  175,
  176,  177,  178,  179,  180,  181,  182,    0,    0,  183,
  184,  108,  109,    0,  110,    0,    0,  185,  186,    0,
  608,  111,  112,  113,  114,  115,  187,  116,  188,    0,
  117,  118,  119,  120,  121,    0,    0,  609,  122,  123,
  189,  190,  191,  192,  124,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  610,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  107,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  611,  612,    0,    0,    0,    0,
    0,    0,    0,  108,  109,    0,  110,    0,    0,    0,
    0,  613,    0,  111,  112,  113,  114,  115,    0,  116,
    0,    0,  117,  118,  119,  120,  121,  461,    0,    0,
  122,  123,    0,    0,    0,    0,  124,  461,  461,  461,
  461,  461,  461,  461,  461,  461,  461,  461,  461,  461,
  461,  461,    0,    0,  461,  461,  461,  461,    0,  461,
    0,    0,  461,  461,    0,    0,  461,  461,  461,  461,
  461,  461,  461,  461,    0,  461,  461,  461,  461,  461,
    0,    0,    0,  461,  461,  461,  461,  461,  461,  461,
};
static short yycheck[] = {                                       9,
  527,   56,   12,  106,    4,   16,   16,   65,   18,    0,
  686,   64,  709,   58,  143,  144,   57,   92,  340,   69,
  371,  371,  564,  367,  154,   70,  467,   68,   86,   95,
   74,   64,  143,  144,  342,   90,  875,  431,   63,  510,
  879, 1196,   92,  534,   54,   55,   56,   57,   58,   59,
   91, 1044,   96,  273,   64,   65,   66,   98,   68,  363,
   70,   71,  282,   73,   74,  694,  763,   41,   89,  487,
  646,  994,   82,   84,   84,   85,   86,   71,   88,   89,
   90,   91,   93,   93,   94,   95,   96,   40,   98,   44,
  135,  157, 1269,  349,  937,    0,    0,  388,  960,  716,
   94,  134,  731,  136,  317,  860,   44,   41,    0,   45,
   45,  156,   41, 1094,  155,    0,  557,   41,  559, 1033,
  376,  290, 1033,  564,  134,  135,  136,  148,  639,    0,
 1044,    0,  235, 1044,    0, 1071,   41,   44,  148,  982,
  770, 1033,    0,    0,  154,  155,  156,  157, 1033,   41,
    0,  161, 1044,    0,   59,   59,   41,  787,   44, 1044,
  790, 1033,  348,    0,  735, 1033,   59,   59, 1038,  799,
   41,  196, 1044,    0,   59, 1045, 1044, 1229,  260,    0,
 1214,   45,   91,   41,    0,   44, 1056,  317,   59,   44,
   59,   41,    0,   59,   41, 1267,  221,   44,  226,  543,
  262,   59,   59,    0,   44,   44,  493, 1077,   44,   59,
   41,   63,   59,  564,  564,  307,  308,   44, 1040,  746,
   41,  324,   59,  853,    0,   41,  271,  903,  904,   42,
    0,  264,   59,   41,  267,  280,   44,   41,   59,    0,
   44,   41,    0,   59,   41,  567,  301,  591,   40, 1404,
 1405,   59, 1095,   45,  264,   40,  652,  267,  934,  479,
  305,  271,   59,  273,  274,   41,  320, 1089, 1261, 1262,
  280,   41,  282,  529,   44,    0,   41,  280,   63,   44,
  402,   40,   45,   59,  317,  264,  161,  264,  301,   59,
  277,  301,  320,  615,  313,  305,   41, 1207,   59, 1209,
 1415,   59, 1295,  594,  260, 1387,  700, 1230,  341, 1317,
  257,  268,    0, 1215,  708,  348,   41,  350,   44,  352,
   40, 1766,  355,  164,  386,  338,  274,    0, 1218,  384,
  348,  341,  373,  283,   59,  417,    0,  382,  348,  364,
  350,  366,  352,   63,   16,  355,  262, 1261, 1262,   41,
 1261, 1262,   44,   41, 1470,  380, 1032,  487,  345,  451,
  338,    0,    0,  373,  374, 1301,  837, 1449,  277, 1261,
 1262,   59,  382,  435,  384,  816, 1261, 1262,  318,   61,
  374, 1295,  436,  455, 1295, 1015,   59,  334, 1290, 1261,
 1262, 1836,   44, 1261, 1262,   59,  300,    0,  417, 1515,
  522, 1453, 1264, 1295,  975, 1035, 1036,  331,  365, 1481,
 1295,  318,   84,  318, 1284,    0,  987,  988,    0, 1241,
   59,   59,  432, 1295, 1314, 1055,  331, 1295, 1583,  442,
  475,  300,  411,  318,  411,  340,   61,  478,   41, 1115,
 1116,  486, 1118, 1119,  348,  490, 1308,  260, 1625,  304,
  495,  738,  493,  387,  487,  340,   59,  328,  387,  781,
  331,  417,  784,  387,  442,  475,   41,  396,  478,  479,
  386,  582,  396,  795,   59,   45,  486,   59,  309,  348,
  490,  320,  387,  493,  880,  495,    0,  328,  498,  439,
  303,  396,  495,   63,  447,  387,    0,  318,    0,  517,
  822,   41, 1179,  458,  396, 1182,  726,  328,  583,  274,
  331, 1516, 1520,  453,  452,  331,  387,  410,  466,  340,
  545,  318,  458,  458,  383,  396,    0,   41, 1104,  387,
  367,  564, 1513,  583,  331,  440,  383,   41, 1461,   41,
   44, 1451,   44,  340,  309,   59, 1723,  521,  318,  454,
  485,  764, 1307,  563,  564,   59,  864,   59,  328,  569,
  585,  331,  980,    0,  340, 1599,  387,   41,  454,  454,
  340,  387,  741,  343, 1250,  396, 1193, 1088,  304,  436,
  396,    0,  873,    0,  713,   59,  386,  521,  454,  983,
  387, 1527,  521,  318,  458,  851, 1272,  521, 1775,  396,
  436,  629,  788, 1465, 1474,  638,  458,  411, 1478, 1479,
  635,  636,  264,  517,  454,  340,  521,  387,  300,  440,
    0,    0,   59,  498,   41,  452,  396,    0,  638,  521,
  318,  452, 1809,  454,  764,  394,  521,  647,  749,  431,
   59,  386,   59,  440, 1514, 1760,  411,  647,  517,  410,
  521,  409,  340, 1224,  501,    0,  677,  454,  318,  402,
 1742,  262,   41,  521,    0,   44,  458,  677,  431,  301,
  440,  521, 1494,  458,   12,  300, 1306, 1792,  454,   59,
   59,  714,  452,  716,  454,  718,   59,  262, 1681, 1532,
 1512,  701,  331,  501,  367,  458,   41,  738,  355,  709,
  521,  466, 1857, 1380,  714,  521,  716, 1384,  718, 1338,
  348,  721,  320,  758,   59,  760,  726,   41,  273, 1834,
 1042,    0, 1026,   59,  521,  735, 1731,  301,  738,  454,
  262,  764, 1054, 1127,  744,  745,  769,  340,  458,  814,
 1855, 1611,  286, 1858,  964, 1615,  328,  967,  758,  331,
  760,  521,  777,  763, 1076,  788,   12,  342, 1873,  769,
 1630,   41,   41,  802,  814,  508,  454, 1681,  387,  257,
 1681,  821,  847,  848,  849, 1637,  440, 1647,  788, 1270,
   59, 1643,  815,  816,  834,  386, 1090, 1463,  328, 1681,
  454,  331,  377,  453, 1102, 1189, 1681,  847,  848,  849,
    0,  440,  827,  141,  318,  815,  816,  852,    0, 1681,
  442,  386,  273, 1681,  318,  454,  318,  862,    0,    0,
  405,  440, 1293,  408, 1366,  405,  340,  331, 1299,  348,
    0, 1022,    0,  521, 1228,  492,  340,    0,  340,  424,
  735,   41,  852,  300,  318,    0,  334,  387,    0,   41,
  980,  454,  862,    0,  386,  506,  396, 1165,  440,   59,
 1730,  431,   41,  402,  874,  450,  340,   59,  442, 1739,
  452,   41,  454,  387,  377,    0,  456,   59,   59,  340,
  356,  300,  396,  387, 1561,  141,   41,    0,  458,   59,
    0,   59,  396, 1680,   41,  352,   59,   44, 1574,  438,
  377,  318,  968,  435,   59,  342, 1745,   59, 1099, 1779,
    0,  328,   59,  387,  331,  325,   41,  461,  462,   44,
  308, 1708,  396,  340,  965,  969,  440,   40,   41,  348,
  318,   44,  405,    0,   59,   41,  440,  356,  262,  318,
  454,  418,  461,  462,   41,  448,   59,  980, 1389,   59,
  454, 1738,  454,  429,  964,  965,  966,  967,  968,  969,
 1282,  340,  342, 1074,  402,  975,  440, 1311,  405,   59,
  387,  408,  966,  318,  393,  348,  395,  987,  988,  396,
  454,  521,  262,  456,  994,  287,    0,  424,  465, 1022,
    0, 1313,   59,  343,  263,  340,  434,  377,  300,  418,
  502,   59,  300,  994,    0,    0,  342,  521, 1083, 1048,
 1049,  449, 1022,  450,  397, 1366, 1366,  521,  368,  521,
  430,  261,  319,  440,  443,  405, 1101,  437,  408, 1733,
 1706,   41, 1107, 1083,   44, 1711, 1823,  454, 1565,  318,
    0,  377,  387,  312,  424,   59,    0,  521,    0,   59,
  348, 1101,  409,  436,  323, 1065,    0, 1107, 1755, 1092,
 1099,  340,  386,   59,   59,  453, 1099, 1197, 1068,  405,
  450, 1081,  408, 1600,  458,  454,  426,  258, 1782,    0,
  975,   44, 1092,  262, 1213, 1214, 1215,  470,  424, 1099,
  271,  259,  987,    0,  417,  440,  264, 1787,  517,   59,
   44,  264,  270,  400,  521,   59,  386,   59,   42,  454,
    0, 1801,  264,   47,  450,   59, 1820,  264,  318,  300,
   41,  444, 1649,   44,   44,  306,  318,  308,   42,   43,
   93,   45,    0,   47,  260, 1246,  328, 1248,   59,  331,
  340,   44,  521, 1196,  441,  465,  328,  467,  340,  331,
    0,   41,   59,  278,   44,  280,  262,    0,  427,  260,
 1193, 1483,  287, 1196, 1197,  262,    0,  348,  458,   59,
  340,  318,    0,   93,  321,  454,  521,  303,    0, 1869,
    0,  328,    0, 1193,  331,  340, 1196, 1197,  335,  652,
   93,   59, 1868,  340,  657,  387,  343, 1207, 1309, 1209,
 1522,    0,  303,   93,  396,  318,    0,  386,  321,   59,
   44,  278,  359,  280, 1224,  328,   59,    0,  331,  517,
 1230,   41,  335,   41,   44,   59,   44,  340,  458,  397,
  343,   59,  342,  405,  397,  291,  383,   59,    0,   59,
  387,   59,   41,  299,  258,  397,  359,    0,  440,  396,
   44,    0,  424,  378,  454,  485,    0,  271,  440, 1786,
   59,  278,  454,  417,   40,   59,  413,  377,  264,  264,
  383,  308,  454,   93,  387,  400,   59,  307,  308,    0,
  386,  318,  407,  396,  454, 1415,  300,  265, 1288,  386,
  444,  283,  306,  440,  308,  405,  401,   59,  408, 1324,
  413,    0,  319, 1328,  264,  452,   59,  454,  318, 1743,
   59,  321,  264,  460,  424,   59,  441,  259,  328,    0,
   41,  331, 1644,   44,  302,  335, 1456,  440,  270,  521,
  340, 1765,    0,  343,  348,  322,  517, 1395,   59,  452,
  450,  454,  489,  490,  301,  492,  280,  316,  317,  359,
 1784,  285,  499,  500,  501,  502,  503,  264,  505,  337,
   59,  508,  509,  510,  511,  512,  280,  356,    0,  516,
  517,  285,  258,  383,  521,  522,  263,  387,   59,  307,
  308,  338, 1415,  400,  328, 1395,  396,  331,  337,    0,
    0,   59,  384, 1403, 1404, 1405,  264,  318,  322,  343,
  287,  397,  397,  413,  393, 1415,  395,  328,  521,  296,
  331,   40,  260,  876,  264,  258,  453,  880, 1852,  340,
  306,  451,  343,   44,  441,  374, 1586,   59,  271,  418,
  440,   41,  383,  352,   44,  337,  264,  397,  387, 1484,
  327, 1451,  452,  330,  454,  397, 1456,  439,   59,   59,
  460, 1461, 1735,   44,  443,  303, 1586,  300,   44,  378,
  278,  436,  280,  306,  456,  308,  387,  493,   59,  287,
 1599, 1601,  374,   59, 1484,  396,   40,  503,  260,  489,
  490,    0,  492,   42,   43,  387,   45,  383,   47,  499,
  500,  501,  502,  503,   58,  505,  440, 1780,  508,  509,
  510,  511,  512,  517,  301,  348,  516,  517,  452,  385,
  454,  521,  522,  356,  258,  264,  278,  378,  280,  440,
  410,  303,  436,  451,    0, 1628,    0,  271,    0,  397,
 1571,  452,  418,  454,  328,  272, 1819,  331,  425,  400,
   59,  338,  429,  430,  431,  259,  407,  397,    0,  343,
  436,  288,    0, 1207,    0, 1209,  270,  278,    0,  280,
  378, 1571,  306,  260,  308,  383,  287,   41, 1851,  397,
   44,  493,  494, 1583, 1629,    0,    0,    0,  258, 1624,
  441,  261,  400,   59,  470,   59,  429,   59, 1748,  407,
  410, 1601, 1752,   44, 1754,   41,  333,  278,   44,  280,
  521,  321,    0, 1763, 1595,    0,  303,   59,  383, 1619,
  407,   59,  499,   59, 1624,  335,  503,   59, 1748, 1629,
  417, 1724, 1752,  441, 1754,  362,  306,  321, 1653, 1789,
 1760,   59, 1657, 1763,   59,   59,   59, 1648, 1648,  359,
 1698,  335,   93,    0,    0,  442,  440,    0,  397,  343,
    0,   42,   43,  265,   45,    0,   47,  378,  452, 1789,
  454,   59, 1792,  383,   59,  359, 1826,  294,  278,  494,
  280, 1831,    0,  383,  517,  280,    0,  287,  305,  400,
  285,    0,  337,  501,   41,  312,  407,   44, 1698, 1699,
  302,   41,  429,  413,   44,    0, 1826,  378,    0,    0,
 1700, 1831,   59,   59, 1834,    0,   59,    0,  274,   59,
  337,   41,   42,   43,   59,   45,  403,   47,  418,  413,
  441,  280, 1733, 1733,   40, 1855,  285, 1760, 1858,   42,
   43,   59,   45,  420,   47,   59,   42,   43,  424,   45,
   59,   47,  521, 1873,  245, 1755,   42,   43,  461,   45,
 1760,   47,  253,   59,   59,  264, 1766,   59,   59, 1792,
 1771, 1771,   42,   43,   59,   45,   59,   47,  378,  316,
  657, 1782, 1782,   41,   41,  402,   44,   44, 1824,  337,
  501,  340, 1792,   41,   46,  318,   44,  674, 1843, 1835,
  400,  678,   41, 1804, 1804,   44,   58,  407,  685,   61,
  460, 1834,  342,  690,  691, 1853, 1884,  434, 1886, 1820,
 1820,  438, 1860,  700, 1586,  702,  703,  704,  705,  706,
  707,  708, 1855,   46, 1834, 1858, 1836,  452,    0, 1840,
 1840,  441,  278, 1843,  280,   58, 1882,  377,   61, 1887,
 1873,  287,  258,   41,   46, 1855,   44, 1857, 1858,   42,
   43,  411,   45, 1863,   47,  271,   58,  336,    0,   61,
  747,   41,  340, 1873,   44,  405,  461,  462,  408,   41,
    0,  402,   44,   40, 1884,  320, 1886,   41,   42,   43,
  767,   45,  318,   47,  424,  772,  773,   59,  402,  280,
  306,  501,  308,  383,  285,  321,  460,  383,   40,   41,
   42,   43,   44,   45,   46,   47,  391,  392,  506,  335,
  450,   41,  262,  436,   44,  450,    0,   59,   60,   61,
   62,   41,  461,  462,   44,  489,  490,  318,  492,   59,
  361,   41,  378,  359,   44,  499,  500,  501,  502,  503,
   41,  505,  434,   44,  508,  509,  510,  511,  512,   91,
  280,   93,  516,  517,  400,  285,   41,   41,  522,   44,
   44,  407,   41,   41,  384,   44,   44,  280,  318,  318,
   41,  321,  285,   44,  280,   59, 1748,   61,  328,  285,
 1752,  331, 1754,   41,  280,  335,   44,  413,  417,  285,
  340, 1763,  516,  343,   41,  441,   41,   44,  311,   44,
  280,  888,  889,  890,  517,  285,  893,  894,  895,  359,
  897,  898,  899,  900,  901,  902,  516, 1789,  905,  906,
  469,  908,   41,  910,  320,   44,  913,  914,  915,  916,
  917,  918,  460,  383,  340,   41,  386,  387,   44,  287,
  320,   44,  673,  930,  675,  932,  396,   61,  679,  680,
  681,  372,  286,   41, 1826,  501,   44,   40,  264, 1831,
   40,  489,  490,  413,  492,   40,   40,   40,  461,    0,
    0,  499,  500,  501,  502,  503,   40,  505,   40,   40,
  508,  509,  510,  511,  512,  435,   40,  280,  516,  517,
  440,   40,  285,  321,  522,   40,  983,   40,  461,   41,
   42,   43,  452,   45,  454,   47,  278,  335,  280,   40,
   41,   40,   40,   44,   44,  287,  280,  461,  311,   40,
  262,  285,  264,  265,  457,  458,  459,   40,   59,   59,
  272,  359,   44,   41,   42,   43, 1748,   45,  280,   47,
 1752,   91,   46,  285,   40,   61,  288,   40,  278,  264,
  280, 1763,  485,  486,  284,  383,    0,  287,  321,  436,
  302,  320,   44,  402,  320,  307,  308,   44,   44,  311,
  300,  521,  335,  428,  258,   40,  318, 1789,  320,  321,
  343,   61,  324,   46,   40,  413,  328,  264,  442,  331,
  274,  333,   44,  335,  454,  348,  359,   41,  340,  341,
   44,  343,  348,  893,  348,  895,  378,  897,  898,  278,
  900,  901,  902,  278, 1826,   59,  300,  359,  360, 1831,
  362,  264,  306,  397,  300,  309,  260,  436,  400,  470,
  348,  397,  458,  277,  376,  407,  378,  379,  380,  447,
 1127,  383,  447,   40,  386,  387,   42,   43,  378,   45,
  413,   47, 1139, 1140,  396, 1142, 1143,  349,  399,   91,
   41,   42,   43, 1150,   45,   40,   47,  301,  410,  441,
  400,  413,   41,   42,   43,   40,   45,  407,   47,  313,
   40,  318, 1169,  264,  383,  411,   44,  429,  382,  389,
 1177, 1178, 1179,  435,  436, 1182,   40, 1184,  440,   40,
  451, 1188, 1189, 1190,  338,   41,  277,   63,  299,  451,
  452,  441,  454,   41,   42,   43,  464,   45,  460,   47,
  436,  317,   40,  320,  452,  320,  448,  411,  264,  502,
  382, 1218,   40,  274,  418,  505,   40,  300,  280,  344,
  284, 1228,  344,  285,  278,  386,  386,  489,  490,  416,
  492,  386,  436,  300,  418,  418,   40,  499,  500,  501,
  502,  503,  356,  505,  417,   40,  508,  509,  510,  511,
  512,  274,  280,  407,  516,  517,   40,  285, 1265,  521,
  522,  417,  466,  417,   41,   44,  470,  318,  284,  280,
  321,  321,  446,  320,   44,  385,  271,  328,  328,    0,
  331,  331,  367,   44,  335,  335,  320,   40,  442,  340,
   44,  328,  343,  343,   40,  266,   41,   42,   43,  501,
   45,   40,   47,  264,  422,   41,   58, 1314,  359,  359,
   42,   43,   44,   45,  278,   47,  280,   41,   41,   41,
   41,   42,   43,   44,   45,   46,   47,  264, 1335,  309,
  454,  440,  383,  383,  394,  417,  387, 1344,   59,   60,
   61,   62,  492,  309, 1351,  396,   41,   41,   40,  265,
   41,   42,   43,   40,   45,   40,   47,   40,  352,   40,
   40,   40,  413,  413,  280,   40,   40,   40,  418,  285,
   91,   40,   93, 1380,   40,   40,   40, 1384,  460,  280,
   40,   40,   40,  262,  285,  308,  302,  327,   44,  440,
  440,  280,  435, 1400,   41,   41,  285,  264,  388,  343,
  388,  452,  452,  454,  454,  274,  388,  489,  490,  460,
  492,  331,   44,  391,  378,  287,  407,  499,  500,  501,
  502,  503,   41,  505,  386,   41,  508,  509,  510,  511,
  512,  386,  280,  386,  516,  517,  400,  285,  489,  490,
  522,  492,  378,  407,  460,  386,  458,  386,  499,  500,
  501,  502,  503,  386,  505, 1462,  278,  508,  509,  510,
  511,  512,   40,  417,   41,  516,  517,   41,   41,   41,
  521,  522,  308,  489,  490,  460,  492,  441,  435,   59,
  293,   40,  340,  499,  500,  501,  502,  503,   59,  505,
   59,   59,  508,  509,  510,  511,  512,   59,   59,  340,
  516,  517,   59,  308,  489,  490,  522,  492,   42,   43,
   44,   45,   59,   47,  499,  500,  501,  502,  503,   59,
  505,   58,  385,  508,  509,  510,  511,  512,  436,  280,
  396,  516,  517, 1540, 1541,  280,  337,  522,  338,  442,
  285,  262,  442,  264,  265,  301,  338,   41,  280,   41,
 1557,  272,  301,  285, 1561,   42,   43,  320,   45,  280,
   47,  352,  385, 1570,  285, 1572,   41,  288,  472,  473,
  474,  475,  476,  477,  478,  479,   41,  371,  371,   41,
   41,  302,   41,  304,   41,   41,  307,  308,    0,  280,
  311,   41,   41,   41,  285,   41,   41,  318,   41,  320,
  321,   41,    0,  324,   41,   41,   41,  328,  318,  451,
  331,   40,  333,   44,  335,   59,   41,  352,  344,  340,
  341,   59,  343,   42,   43,   40,   45,  340,   47,   41,
   42,   43,   44,   45,   46,   47,   46,  508,  359,  360,
  340,  362,  383,   41,   40, 1652,   44,   59,   60,   61,
   62,  368,   40, 1660,   44,  376,  383,  378,  379,  380,
  410,   59,  383,  435,  383,  386,  387,   42,    0,   41,
   41,  343,   41,   59,   44,  396,  304,   41,   44,   91,
 1687,   93,   42,   43,  301,   45,  387,   47,  293,  410,
   40,  304,  413,  307,  442,  463,  337,   44,   41,   59,
   40,    0,   41,  337,   40, 1712,  383,  383,  429,   41,
  264,  264,   44,  278,  435,  436,   41,   42,   43,  440,
   45,  458,   47,  320,   44,  411,  389,   59,  470,  271,
  451,  452,   40,  454,   41,   42,   43,  451,   45,  460,
   47,  271,  309,   41,   42,   43,  280,   45,  264,   47,
  458,  285,  436,  435,  343, 1762,   41,   42,   43,  343,
   45,  343,   47,   41,  343,  458,   41,    0,  489,  490,
   59,  492,   59,  460,   40,  308,   41,  264,  499,  500,
  501,  502,  503,   59,  505,   59,  304,  508,  509,  510,
  511,  512,   41,  280,   41,  516,  517,   59,  285,   41,
  521,  522,  489,  490,    0,  492,    0,    0,   41, 1210,
   12,   44,  499,  500,  501,  502,  503,  275,  505,  282,
  274,  508,  509,  510,  511,  512,   59,  726,  642,  516,
  517,   88,   41,   42,   43,  522,   45, 1104,   47,  865,
   41,   42,   43, 1102,   45,  264,   47, 1099, 1091, 1503,
  262,  368,  264,  265, 1502,  338, 1081,  591, 1029,  769,
  272,  280,  322,   41,   42,   43,  285,   45,  280,   47,
 1092,  350,  350,  285, 1566,  797,  288,   41,   42,   43,
  278,   45,  280,   47, 1619, 1773,  284, 1777, 1481,  287,
  302,  569,    0, 1483,  788,  307,  308, 1776, 1811,  311,
   42,   43,  300,   45,  961,   47,  318, 1403,  320,  321,
 1687, 1761,  324, 1017, 1591,  714,  328, 1733,  571,  331,
  280,  333, 1243,  335, 1466,  285,  343,  355,  340,  341,
  262,  343,  336,   41,   42,   43,   44,   45,   46,   47,
   42,   43,  844,   45, 1297,   47,  836,  359,  360,   13,
  362,   59,   60,   61,   62,  280,  309,  628,  516,  146,
  285,  512,  758,  772,  376,  747,  378,  379,  380,  994,
   42,  383,    0,  280,  386,  387, 1712, 1609,  285, 1840,
  378, 1804,  280,   91,  396,   93,  318,  285,  527,  321,
  734,  984, 1141,   -1, 1252,  280,  328,   -1,  410,  331,
  285,  413,  400,  335, 1184,   -1,   -1,   -1,  340,  407,
   -1,  343,   -1,   41,   -1,   -1,   44,  429,   -1,   -1,
   -1,   -1,   -1,  435,  436,   -1,   -1,  359,  440,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,  441,   -1,  278,   -1,  280,  460,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,  280,   -1,   -1,   -1,   -1,  285,  489,  490,  280,
  492,  413,   -1,   -1,  285,  460,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  280,  435,  516,  517,   -1,  285,  440,  521,
  522,   -1,   -1,   -1,  489,  490,  280,  492,   -1,   -1,
  452,  285,  454,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  280,   -1,
   -1,  516,  517,  285,   -1,  378,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
  262,   -1,   -1,   -1,  272,   -1,   -1,  400,   -1,   -1,
   -1,   -1,  280,   -1,  407,   -1,   -1,  285,  280,   -1,
  288,   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,  521,
   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,  307,
  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,  441,   -1,
  318,   -1,  320,  321,   42,   43,  324,   45,   -1,   47,
  328,   -1,   -1,  331,  296,  333,   -1,  335,   41,   -1,
   -1,   44,  340,  341,  262,  343,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   59,   -1,   -1,   -1,
   -1,  359,  360,   -1,  362,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,    0,   44,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,  396,   93,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,  410,  331,   -1,  413,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   41,   -1,   -1,
   44,  429,   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,
   -1,  359,  440,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454, 1336, 1337,   -1,
   -1,   -1,  460, 1342, 1343,  383, 1345, 1346,  386,  387,
   -1,   -1,   -1,   -1, 1353, 1354, 1355, 1356,  396, 1358,
 1359, 1360, 1361, 1362, 1363,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,  413,   -1,   -1,  460,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  435,  516,  517,
   -1,   -1,  440,  521,  522,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,  452,   -1,  454,  499,  500,  501,
  502,  503,   -1,  505,  262,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
  522,   -1,  280,   -1,   -1,  305,   -1,  285,  262,   -1,
  264,  265,  312,   -1,   -1,   -1,  316,  317,  272,   -1,
   -1,   -1,   -1,   -1,   -1,  325,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,  337,   -1,   -1,
   -1,   -1,   -1,  521,   -1,   -1,   -1,   -1,  302,   -1,
    0,   45,   -1,  307,  308,   -1,   -1,  311,  321,   -1,
   -1,   -1,   -1,   -1,  318,  328,  320,  321,  331,   -1,
  324,   -1,  335,   -1,  328,   -1,   -1,  331,   -1,  333,
  343,  335,   -1,   -1,   -1,   -1,  340,  341,  262,  343,
   -1,   41,   42,   43,   44,   45,  359,   47,   -1,   -1,
   -1,   -1,  402,  320,   -1,  359,  360,   -1,  362,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,  419,
  383,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,
   -1,   -1,  386,  387,  434,   -1,   -1,   -1,  438,   -1,
   -1,   -1,  396,   93,  318,   58,   -1,  321,   -1,  449,
  413,   -1,   -1,   -1,  328,   -1,  410,  331,   -1,  413,
   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,  468,  343,
   -1,   -1,   -1,   -1,   -1,  429,   -1,  440,   -1,   -1,
   -1,  435,  436,   -1,   -1,  359,  440,   -1,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  413,
   -1,   -1,   -1,  460,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  435,  516,  517,   -1,   -1,  440,  521,  522,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  452,   -1,
  454,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,   -1,
  294,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,  305,  272,   -1,   -1,   -1,   -1,   -1,  312,   -1,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,  325,   -1,   -1,   -1,   -1,   -1,  521,    0,   -1,
   -1,   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
    0,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   41,
  340,  341,   44,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,  386,   -1,   -1,   -1,   -1,   59,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,  402,   -1,
   -1,   41,   -1,   -1,   44,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,  419,  386,  387,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,
  434,   -1,   -1,   -1,  438,   -1,   -1,  441,   -1,   -1,
  410,   -1,   -1,  413,  458,  449,  460,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,  468,  435,  436,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,  451,  452,   -1,  454,  499,  500,  501,  502,  503,
  460,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  460,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,    0,   -1,  516,  517,   -1,   -1,
    0,  521,  522,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   41,   -1,   -1,   44,  522,
   -1,   41,   42,   43,   44,   45,  262,   47,  264,  265,
   -1,   -1,   -1,   59,   -1,   -1,  272,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  258,  285,
   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  302,  277,  278,   -1,
  280,  307,  308,   93,  284,  311,  318,  287,   -1,  321,
   -1,   -1,  318,   -1,  320,  321,  328,   -1,  324,  331,
  300,   -1,  328,  335,   -1,  331,  306,  333,  340,  335,
   -1,  343,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,
   -1,   -1,   -1,  359,  360,    0,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,  383,  378,  379,  380,  387,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   41,   -1,  378,   44,
   -1,  413,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,    0,   -1,
  400,   -1,   -1,  429,   -1,   -1,   -1,  407,  440,  435,
  436,   -1,   -1,   -1,  440,   -1,  416,   -1,  418,   -1,
  452,   -1,  454,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,  436,   -1,   -1,   41,
   -1,  441,   44,  287,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   59,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
  470,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,  521,
  516,  517,  302,   -1,    0,  521,  522,  307,  308,   -1,
   -1,  311,  318,   -1,   -1,  321,   -1,   -1,  318,   -1,
  320,  321,  328,   -1,  324,  331,   -1,   -1,  328,  335,
    0,  331,   -1,  333,  340,  335,   -1,  343,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,  359,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,  376,  383,  378,  379,
  380,  387,   -1,  383,   -1,   -1,  386,  387,   -1,   59,
  396,   61,   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  278,   -1,  280,  460,   -1,   -1,  429,
   -1,   -1,  287,   -1,  440,  435,  436,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,  300,  452,   -1,  454,   -1,
   -1,  451,  452,   -1,  454,  489,  490,   -1,  492,   -1,
  460,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
  262,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  521,  516,  517,   -1,   -1,
    0,  521,  522,  378,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   59,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,  400,   -1,   -1,  340,   -1,
   -1,  343,  407,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,  262,  359,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,  441,   -1,  258,   -1,
   -1,  383,  288,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,  302,  277,  278,   -1,
  280,  307,  308,   93,  284,  311,   -1,  287,   -1,   -1,
   -1,  413,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
  300,   -1,  328,   -1,   -1,  331,  306,  333,   -1,  335,
   -1,   -1,   -1,  435,  340,  341,   -1,  343,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
  452,   -1,  454,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
  396,   44,   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,
   -1,   -1,   -1,   -1,  410,   -1,   59,  413,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,  521,
  400,   -1,   -1,  429,   -1,   -1,   -1,  407,   -1,  435,
  436,   -1,   -1,   -1,  440,   -1,   -1,   -1,  418,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,  436,   -1,   -1,   41,
   -1,  441,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   59,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
  470,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  302,   -1,    0,  521,  522,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  396,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,  278,  460,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,  436,  300,   -1,   -1,
  440,   -1,   -1,  306,   -1,  489,  490,   -1,  492,   -1,
   -1,  451,  452,   -1,  454,  499,  500,  501,  502,  503,
  460,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  262,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  378,  516,  517,   -1,   -1,
    0,  521,  522,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,  400,   -1,  331,
   -1,   -1,   -1,  335,  407,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,  262,  359,  264,  265,
   -1,   -1,   -1,  436,   -1,   -1,  272,   -1,  441,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  383,  288,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,  302,  470,   -1,   -1,
   -1,  307,  308,   93,   -1,  311,   -1,   -1,   -1,   -1,
   -1,  413,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
    0,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,  435,  340,  341,   -1,  343,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
  452,   -1,  454,  359,  360,   -1,  362,    0,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   59,
  386,  387,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
  396,   44,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,  410,   -1,   59,  413,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,  521,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
  436,   -1,   -1,   -1,  440,   -1,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  302,   -1,    0,  521,  522,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  396,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,  274,   -1,  413,  277,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,  278,   -1,  280,  318,  429,
   -1,  321,   -1,   -1,  287,  435,  436,  300,  328,   -1,
  440,  331,   -1,  306,   -1,  335,  309,  300,   -1,   -1,
  340,  451,  452,  343,  454,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,  383,   -1,   -1,  386,  387,   -1,  499,
  500,  501,  502,  503,   -1,  505,  396,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  378,  516,  517,   -1,   -1,
   -1,  521,  522,  413,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,    0,
   -1,   -1,   -1,   -1,  407,  435,   -1,  400,  411,   -1,
  440,   -1,   -1,   -1,  407,  418,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,  436,   -1,   -1,  272,   -1,  441,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,  441,   -1,
   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,  466,   -1,   -1,  302,  470,   -1,   -1,
   -1,  307,  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,  287,   -1,  324,   -1,
   91,  521,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,
   -1,   60,   61,   62,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
  436,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  460,
   -1,  262,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,  272,  508,  509,  510,  511,  512,   91,   -1,  280,
  516,  517,   -1,   -1,  285,  521,  522,  288,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,    0,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  318,   -1,   -1,
  321,  522,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   -1,   -1,   -1,   59,   60,   61,   62,   -1,
    0,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,  272,   -1,  396,   -1,   91,   -1,   -1,
   -1,  280,    0,   -1,   -1,   -1,  285,   -1,   -1,  288,
   -1,   41,  413,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,   59,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   41,   -1,  324,   44,    0,  262,   -1,
   -1,  452,   -1,  454,  333,   -1,   -1,   -1,  272,   -1,
   -1,   59,  341,   -1,   -1,   -1,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,  362,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  318,   -1,   59,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
  521,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
  429,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,  262,  383,
   -1,   41,  386,  387,   44,   -1,   -1,   -1,  272,   -1,
   -1,   -1,  396,   -1,   -1,   -1,  280,   -1,   -1,   59,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,  318,   -1,  440,  321,   -1,   -1,
  324,   -1,  262,   -1,  328,   -1,   -1,  331,  452,  333,
  454,  335,   -1,   -1,   -1,   41,  340,  341,   44,  343,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,  262,  359,  360,   -1,  362,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,  318,  383,
   -1,  321,  386,  387,   -1,   -1,   -1,   -1,  328,   -1,
   -1,  331,  396,   -1,   -1,  335,   -1,  521,   -1,  262,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  413,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,  359,
  328,   -1,   -1,  331,   -1,  429,   -1,  335,   -1,   -1,
   -1,  435,  340,   -1,   -1,  343,  440,   -1,   -1,   -1,
    0,   -1,   -1,  383,   -1,   -1,  386,  387,  452,   -1,
  454,  359,   -1,   -1,   -1,  318,  396,   -1,  321,   -1,
    0,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,  413,   -1,  383,   -1,  340,  386,  387,
  343,   41,  262,   -1,   44,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   -1,  435,  359,   -1,   -1,   59,
  440,   41,   -1,   -1,   44,  413,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,  521,   -1,   59,
  383,   -1,   -1,  386,  387,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,  396,    0,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,  452,   -1,  454,   -1,  328,   -1,
  413,  331,   -1,   -1,   -1,  335,  262,   -1,   -1,   -1,
  340,   -1,  262,  343,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   41,   -1,  440,   44,  359,
   -1,  521,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,   59,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,    0,   -1,
   -1,   -1,  318,  521,   -1,  321,  396,   -1,  318,   -1,
   -1,  321,  328,   -1,   -1,  331,   -1,   -1,  328,  335,
   -1,  331,   -1,  413,  340,  335,   -1,  343,   41,   -1,
  340,   44,   -1,  343,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,  359,   -1,  435,   59,   -1,  521,  359,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,    0,  452,   -1,  454,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   41,  413,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   59,   -1,   -1,   -1,  440,  435,   -1,   -1,   -1,   -1,
  440,  521,   -1,   -1,   -1,    0,  452,   -1,  454,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,    0,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,  318,   -1,
  340,  321,   -1,  343,   -1,   -1,   41,   -1,  328,   44,
   -1,  331,   -1,   -1,   -1,  335,  262,   -1,   -1,  359,
  340,   -1,   -1,  343,   59,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,  521,   -1,   -1,   -1,  359,
   -1,  521,   -1,  383,   59,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,  318,  413,   -1,  321,  396,   -1,   -1,  262,
   -1,   -1,  328,   -1,   -1,  331,   45,   -1,   -1,  335,
  262,   -1,   -1,  413,  340,  435,   -1,  343,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,  359,  454,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  318,   -1,  383,  321,   -1,
  386,  387,   -1,   -1,   -1,  328,  318,   -1,  331,  321,
  396,   -1,  335,  262,   -1,   -1,  328,  340,   -1,  331,
  343,   -1,   -1,  335,   -1,   -1,   -1,  413,  340,   -1,
   -1,  343,   -1,   -1,   -1,   40,  359,   -1,   43,   -1,
   45,  521,   -1,   -1,   -1,   -1,   -1,  359,   -1,  435,
   -1,   -1,   -1,   58,  440,   -1,   -1,   -1,   63,   -1,
  383,  521,   -1,  386,  387,   -1,  452,   -1,  454,  318,
   -1,  383,  321,  396,  386,  387,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  396,   -1,  335,  262,   -1,   -1,
  413,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,  452,
   -1,  454,   -1,   -1,  383,  521,   -1,  386,  387,   -1,
  452,   -1,  454,  318,   -1,   -1,  321,  396,   -1,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,  318,  413,  340,  321,   -1,  343,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,  359,  340,  435,   -1,  343,   -1,
   -1,  440,   -1,   -1,   -1,   -1,  295,   -1,  521,   -1,
   -1,   -1,   -1,  452,  359,  454,   -1,   -1,  383,  521,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,   -1,  323,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  352,   -1,   -1,   -1,   -1,  413,   -1,
  435,   -1,  267,   -1,   -1,  440,   -1,   -1,   -1,   -1,
   -1,  276,  521,   -1,   -1,   -1,   -1,  452,   -1,  454,
  435,   -1,   -1,   -1,  289,  440,   -1,   -1,   -1,   40,
  295,  296,   43,   -1,   45,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  314,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,  443,   -1,  521,  352,   -1,  354,
  355,   -1,   -1,   60,   61,   62,   -1,   -1,  457,  458,
  459,  366,  461,  462,   -1,  370,  521,   -1,   -1,   -1,
   -1,   -1,   -1,  378,   -1,   40,   -1,   -1,   43,   -1,
   45,  480,  481,  482,  483,  484,  485,  486,  487,  488,
   -1,   -1,   -1,   58,   -1,   -1,  495,  496,   63,   -1,
   -1,   -1,   -1,   -1,   -1,  504,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,  513,  514,
  515,  516,  517,   -1,   -1,   -1,  267,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,  292,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  314,   -1,  272,   -1,   -1,   -1,   -1,
   -1,   -1,  323,  280,   -1,  326,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  352,  267,  354,  355,   40,   -1,   -1,   43,   -1,
   45,  276,   -1,   -1,   -1,  366,   -1,  324,   -1,  370,
   -1,   -1,   -1,   58,  289,   -1,  333,  378,   63,   -1,
  295,  296,   -1,   -1,  341,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,
   -1,   -1,   -1,  360,   -1,  362,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
  421,  378,  379,  380,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,  457,  458,  459,  460,
  461,  462,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,  429,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  421,  508,  509,  510,
  511,  512,  427,   -1,   -1,  516,  517,   -1,  433,   -1,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,   -1,  508,  509,  510,  511,  512,   -1,  261,
   -1,  516,  517,   40,  289,   -1,   43,  522,   45,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  282,   58,   -1,   -1,   -1,   -1,   63,   -1,  290,  314,
   -1,   -1,   -1,   -1,   -1,  297,   -1,  299,  323,  301,
   -1,  326,   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,
   -1,  313,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  327,   -1,  352,   -1,  354,
  355,   -1,   -1,   -1,   -1,   -1,  338,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  273,   -1,
   -1,   -1,  277,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  405,   -1,   -1,   -1,   -1,  433,   -1,
  412,   -1,   -1,  415,  339,  417,  418,   -1,  443,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
  442,   -1,   -1,   -1,   -1,   -1,  471,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  406,   -1,  508,  509,  510,  511,  512,   -1,  491,
  267,  516,  517,   -1,   -1,   -1,   -1,  522,  423,  276,
   -1,   -1,  460,   -1,  506,  507,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,  314,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  323,  505,   -1,  326,
  508,  509,  510,  511,  512,  480,  481,   -1,  516,  517,
   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  497,   -1,   -1,  352,   -1,  354,  355,   -1,
   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,  366,
  267,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,  276,
   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  421,   -1,  323,   63,   -1,  326,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,  260,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,  301,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
  313,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,  338,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,  260,  522,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,  407,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,  417,   -1,   -1,   -1,   -1,  295,
  296,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,
   58,   -1,   -1,   -1,   -1,   63,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
  366,   -1,   -1,   -1,  370,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  260,   -1,   -1,  263,  522,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,  291,   -1,  326,   -1,
  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,  366,   -1,
   -1,   -1,  370,   -1,  339,   -1,  276,   -1,   -1,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,  323,   -1,   -1,  326,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,  406,   -1,   -1,   -1,  443,  411,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,   -1,  423,  457,
  458,  459,  460,  461,  462,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
  445,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,  480,  481,   -1,  516,  517,
   -1,   -1,   -1,   -1,  522,  425,   -1,  427,   -1,   -1,
   -1,   -1,  497,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  260,   -1,
   -1,  263,  522,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,  323,  291,   -1,  326,   -1,  295,   -1,   -1,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  423,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,  480,  481,   -1,  516,  517,  260,   -1,   -1,  263,
  522,   -1,   -1,  267,   -1,   -1,   -1,   -1,  497,   -1,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
  291,   -1,  326,   -1,  295,   -1,   -1,  298,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,  366,   -1,   -1,   -1,  370,   -1,  339,   -1,
  276,   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,  323,   -1,   -1,
  326,  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,
   -1,   -1,  423,  457,  458,  459,  460,  461,  462,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,  480,
  481,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  425,
   -1,  427,   -1,   -1,   -1,   -1,  497,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  260,   -1,  434,  263,  522,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  489,
  490,   -1,  492,   63,   -1,  323,   -1,   -1,  326,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  522,   -1,  352,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,  366,   -1,
  277,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   42,   43,   -1,   45,  293,   -1,  295,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,
   -1,   -1,  339,   -1,   -1,  433,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,  443,  353,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  406,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  423,   -1,  516,  517,
  260,   -1,   -1,   -1,  522,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
  497,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  260,   -1,  443,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,  276,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,  289,
  370,   -1,   -1,   -1,   40,  295,  296,   43,   -1,   45,
   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   58,   -1,   -1,   -1,   -1,   63,  499,  500,
  501,  502,  503,  323,  505,   -1,  326,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,  522,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,  352,  433,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  427,  508,  509,
  510,  511,  512,  433,   -1,   -1,  516,  517,  301,   -1,
   -1,   -1,  522,  443,   -1,   -1,   -1,   -1,   -1,   -1,
  313,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  338,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  260,   -1,  516,  517,   -1,   -1,
   -1,  267,  522,   -1,   40,   -1,   42,   43,   -1,   45,
  276,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  289,  407,   -1,   -1,   63,   -1,  295,
  296,   -1,   -1,   -1,  417,   -1,   -1,  303,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  323,  505,  442,
  326,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,  489,  490,   58,  492,
   -1,   -1,   -1,   63,  460,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,  427,  508,  509,  510,  511,  512,  433,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,  273,   -1,  471,   -1,  277,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,  295,  492,   -1,  298,  495,
  496,  267,  498,  499,  500,  501,  502,  503,  504,  505,
  276,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,  289,   -1,   -1,  522,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  352,  267,  354,  355,
   40,   -1,   -1,   43,   -1,   45,  276,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,  406,   -1,   58,  289,
   -1,   -1,   -1,   63,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,  445,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,  443,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  457,  458,  459,  460,  461,  462,  497,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,  427,   -1,   -1,
  516,  517,   -1,  433,   -1,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,  267,  498,  499,
  500,  501,  502,  503,  504,  505,  276,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,  289,
   -1,   -1,  522,   -1,   -1,  295,  296,   40,   41,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,  328,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,  328,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  438,   -1,  516,  517,   -1,  443,
   -1,   -1,  522,   -1,  267,  449,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  468,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
  323,  505,   -1,  326,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,  443,  352,
   -1,  354,  355,  276,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,  366,   -1,  460,  289,  370,   -1,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
  323,   -1,   -1,  326,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,  427,   -1,   -1,  522,   -1,  352,
  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  427,  508,  509,  510,  511,  512,
  433,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,  343,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,  368,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  516,  517,   40,   -1,   -1,   43,  522,
   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  489,
  490,   -1,  492,   58,   59,   -1,   -1,   -1,   63,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  276,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,  276,   -1,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,
  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,  352,  433,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  454,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,  271,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  427,  508,  509,  510,  511,  512,  433,   -1,
   -1,  516,  517,   -1,   -1,   -1,  301,  522,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,
  315,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,  332,   -1,   -1,
   -1,   -1,   -1,  338,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  276,  516,  517,   40,   -1,   -1,   43,  522,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   58,   -1,  398,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,
   -1,   -1,   -1,   40,   -1,  460,   43,   -1,   45,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,
  516,  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,  276,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  366,
   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  352,  433,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  427,  508,  509,  510,  511,  512,  433,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
    0,  508,  509,  510,  511,  512,   -1,   -1,  276,  516,
  517,   -1,    0,   -1,   -1,  522,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,    0,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,  323,   44,   -1,  326,   59,
   -1,   61,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   59,   -1,   61,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   59,  352,   61,  354,  355,   -1,    0,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   91,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,   -1,   -1,   -1,  274,   -1,   -1,  277,  278,   -1,
  280,   -1,  258,   -1,  284,   -1,  274,  287,   -1,  277,
  278,   -1,  280,   -1,   -1,   -1,  284,   -1,  274,  287,
  300,   -1,  278,   -1,  280,   -1,  306,   -1,  284,  309,
   -1,  287,  300,   -1,   -1,   -1,   -1,   -1,  306,   -1,
   -1,  309,   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,
  306,   -1,   -1,  309,   -1,  258,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,  274,  287,   -1,   -1,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,   -1,  287,  300,  378,   -1,
   -1,   -1,   -1,  306,   -1,  271,  309,   -1,   -1,  300,
  378,   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,   -1,
  400,   -1,  378,   -1,   -1,   -1,   -1,  407,   -1,   -1,
   -1,  411,  400,   -1,   -1,  301,   -1,   -1,  418,  407,
   -1,   -1,  308,  411,  400,   -1,  312,  313,   -1,  315,
  418,  407,  318,   -1,   -1,  411,  436,   -1,   -1,   -1,
   -1,  441,  418,   -1,   -1,   -1,  332,   -1,  436,   -1,
   -1,   -1,  338,  441,   -1,  378,   -1,   -1,   -1,   -1,
  436,   -1,   -1,   -1,   -1,  441,  466,  378,   -1,   -1,
  470,   -1,   -1,   -1,   -1,   -1,   -1,  400,  466,   -1,
   -1,   -1,  470,   -1,  407,   -1,   -1,   -1,  411,  400,
  466,  271,   -1,   -1,  470,  418,  407,   -1,   -1,   -1,
  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,
   -1,   -1,  398,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,  301,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,  417,  312,  313,   -1,  315,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  466,   -1,   -1,  432,  470,   -1,   -1,
   -1,   -1,  332,   -1,   -1,  466,  442,   -1,  338,  470,
   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,   -1,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,  295,  492,   -1,  298,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  398,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  518,  519,  520,   -1,  522,  417,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,  432,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,  442,  353,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,  406,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,  423,   -1,   -1,  516,  517,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,  273,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  295,
   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,  339,   -1,   -1,  516,  517,   -1,  345,
  346,  347,  522,  349,  350,  351,   -1,  353,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,  469,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,  483,  484,   -1,   -1,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  406,  499,  500,  501,  502,  503,  504,  505,  506,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  423,  516,  517,
  518,  519,  520,  521,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,  497,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  460,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
  483,  484,   -1,   -1,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,   -1,  499,  500,  501,  502,
  503,  504,  505,  506,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  518,  519,  520,  521,  522,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 522
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
"KW_UPDATING","KW_DELETING","BACKUP","KW_DIFFERENCE","OPEN","CLOSE","FETCH",
"ROWS","BLOCK",
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
"statement : invoke_block",
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
"db_rem_option : DEFAULT CHARACTER SET symbol_character_set_name",
"db_rem_option : KW_DIFFERENCE KW_FILE sql_string",
"db_file : file1 sql_string file_desc1",
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
"input_parameters : '(' input_proc_parameters ')'",
"input_parameters :",
"output_parameters : RETURNS '(' output_proc_parameters ')'",
"output_parameters :",
"input_proc_parameters : input_proc_parameter",
"input_proc_parameters : input_proc_parameters ',' input_proc_parameter",
"input_proc_parameter : simple_column_def_name non_array_type default_par_opt end_trigger",
"output_proc_parameters : proc_parameter",
"output_proc_parameters : output_proc_parameters ',' proc_parameter",
"proc_parameter : simple_column_def_name non_array_type",
"default_par_opt : DEFAULT begin_string default_value",
"default_par_opt : '=' begin_string default_value",
"default_par_opt : begin_string",
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
"exec_sql : EXECUTE STATEMENT value ';'",
"for_select : label_opt FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : label_opt FOR EXECUTE STATEMENT value INTO variable_list DO proc_block",
"exec_into : EXECUTE STATEMENT value INTO variable_list ';'",
"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
"if_then_else : IF '(' search_condition ')' THEN proc_block",
"post_event : POST_EVENT value event_argument_opt ';'",
"event_argument_opt :",
"singleton_select : select INTO variable_list ';'",
"variable : ':' symbol_variable_name",
"proc_inputs : value_list",
"proc_inputs : '(' value_list ')'",
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
"invoke_block : EXECUTE BLOCK block_input_params output_parameters AS local_declaration_list full_proc_block",
"block_input_params : '(' block_parameters ')'",
"block_input_params :",
"block_parameters : block_parameter",
"block_parameters : block_parameters ',' block_parameter",
"block_parameter : proc_parameter '=' parameter",
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
"keyword_or_column : ROWS",
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
"db_alter_clause : ADD KW_DIFFERENCE KW_FILE sql_string",
"db_alter_clause : DROP KW_DIFFERENCE KW_FILE",
"db_alter_clause : BEGIN BACKUP",
"db_alter_clause : END BACKUP",
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
"select : union_expr order_clause rows_clause for_update_clause lock_clause",
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
"rows_clause : ROWS value",
"rows_clause : ROWS value TO value",
"rows_clause :",
"for_update_clause : FOR UPDATE for_update_list",
"for_update_clause :",
"for_update_list : OF column_list",
"for_update_list :",
"lock_clause : WITH LOCK",
"lock_clause :",
"select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause",
"ordered_select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause order_clause rows_clause",
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
"select_item : value",
"select_item : value as_noise symbol_item_alias_name",
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
"table_proc_inputs : '(' value_list ')'",
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
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' value_list ')'",
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
"assignment : update_column_name '=' value",
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
"column_select : SELECT limit_clause distinct_clause value from_clause where_clause group_clause having_clause plan_clause order_clause rows_clause",
"column_singleton : SELECT limit_clause distinct_clause value from_clause where_clause group_clause having_clause plan_clause order_clause rows_clause",
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
"value : null_value",
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
"cast_specification : CAST '(' value AS data_type_descriptor ')'",
"case_expression : case_abbreviation",
"case_expression : case_specification",
"case_abbreviation : NULLIF '(' value ',' value ')'",
"case_abbreviation : COALESCE '(' value ',' value_list ')'",
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
"case_result : value",
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
"non_reserved_word : BLOCK",
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
	for (const TOK* token = KEYWORD_getTokens(); token->tok_string; ++token)
	{
		DSQL_SYM symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) dsql_sym;
		symbol->sym_string = (TEXT *) token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		dsql_str* str = FB_NEW_RPT(*DSQL_permanent_pool, symbol->sym_length) dsql_str;
		str->str_length = symbol->sym_length;
		strncpy((char*)str->str_data, (char*)symbol->sym_string, symbol->sym_length);
		symbol->sym_object = (void *) str;
		HSHD_insert(symbol);
	}
}


void LEX_string (
	const TEXT* string,
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


static const TEXT* lex_position (void)
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
static bool long_int(dsql_nod* string,
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

	for (const UCHAR* p = (UCHAR*)((dsql_str*) string)->str_data; 
		 classes[*p] & CHR_DIGIT; p++)
	{
		if (!(classes[*p] & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol(((dsql_str*) string)->str_data);

	return true;
}
#endif

static dsql_fld* make_field (dsql_nod* field_name)
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
	tsql* tdsql = DSQL_get_thread_data();

	if (field_name == NULL)
	{
		dsql_fld* field =
			FB_NEW_RPT(*tdsql->getDefaultPool(), sizeof (INTERNAL_FIELD_NAME)) dsql_fld;
		strcpy (field->fld_name, (TEXT*) INTERNAL_FIELD_NAME);
		return field;
	}
	const dsql_str* string = (dsql_str*) field_name->nod_arg[1];
	dsql_fld* field =
		FB_NEW_RPT(*tdsql->getDefaultPool(), strlen ((SCHAR*) string->str_data)) dsql_fld;
	strcpy (field->fld_name, (TEXT*) string->str_data);

	return field;
}


static dsql_fil* make_file()
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
	tsql* tdsql = DSQL_get_thread_data();
		   
	dsql_fil* temp_file = FB_NEW(*tdsql->getDefaultPool()) dsql_fil;

	return temp_file;
}


static dsql_nod* make_list (dsql_nod* node)
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
	tsql* tdsql = DSQL_get_thread_data();

	if (!node)
		return node;

	DsqlNodStack stack;
	stack_nodes (node, stack);
	USHORT l = stack.getCount();

	dsql_nod* old  = node;
	node = FB_NEW_RPT(*tdsql->getDefaultPool(), l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	dsql_nod** ptr = node->nod_arg + node->nod_count;

	while (stack.hasData())
		*--ptr = stack.pop();

	return node;
}


static dsql_nod* make_parameter (void)
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
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), 1) dsql_nod;
	node->nod_type = nod_parameter;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = 1;
	node->nod_arg[0] = (dsql_nod*)(IPTR) lex.param_number++;

	return node;
}


static dsql_nod* make_node (NOD_TYPE	type,
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
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

	return node;
}


static dsql_nod* make_flag_node (NOD_TYPE	type,
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
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

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
static bool short_int(dsql_nod* string,
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

	if (((dsql_str*) string)->str_length > 5) {
		return false;
	}

	for (UCHAR* p = (UCHAR*)((dsql_str*) string)->str_data; 
		classes[*p] & CHR_DIGIT; p++)
	{
		if (!(classes[*p] & CHR_DIGIT)) {
			return false;
		}
	}

	/* there are 5 or fewer digits, it's value may still be greater
	 * than 32767... */

	SCHAR buf[10];	
	buf[0] = ((dsql_str*) string)->str_data[0];
	buf[1] = ((dsql_str*) string)->str_data[1];
	buf[2] = ((dsql_str*) string)->str_data[2];
	buf[3] = ((dsql_str*) string)->str_data[3];
	buf[4] = ((dsql_str*) string)->str_data[4];
	buf[5] = '\0';

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

static void stack_nodes (dsql_nod*	node,
						 DsqlNodStack& stack)
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
	if (node->nod_type != nod_list)
	{
		stack.push(node);
		return;
	}

	/* To take care of cases where long lists of nodes are in a chain
	   of list nodes with exactly one entry, this algorithm will look
	   for a pattern of repeated list nodes with two entries, the first
	   being a list node and the second being a non-list node.   Such
	   a list will be reverse linked, and then re-reversed, stacking the
	   non-list nodes in the process.   The purpose of this is to avoid
	   massive recursion of this function. */

	dsql_nod* start_chain = node;
	dsql_nod* end_chain = NULL;
	dsql_nod* curr_node = node;
	dsql_nod* next_node = node->nod_arg[0];
	while ( curr_node->nod_count == 2 &&
			curr_node->nod_arg[0]->nod_type == nod_list &&
			curr_node->nod_arg[1]->nod_type != nod_list &&
			next_node->nod_arg[0]->nod_type == nod_list &&
			next_node->nod_arg[1]->nod_type != nod_list)
	{

		/* pattern was found so reverse the links and go to next node */

		dsql_nod* save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
	}

	/* see if any chain was found */

	if (end_chain)
	{

		/* first, handle the rest of the nodes */
		/* note that next_node still points to the first non-pattern node */

		stack_nodes (next_node, stack);

		/* stack the non-list nodes and reverse the chain on the way back */
		
		curr_node = end_chain;
		while (true)
		{
			stack.push(curr_node->nod_arg[1]);
			if ( curr_node == start_chain)
				break;
			dsql_nod* save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
		}
		return;
	}

	dsql_nod** ptr = node->nod_arg;
	for (const dsql_nod* const* const end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes (*ptr, stack);
}

inline static int yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	bool* stmt_ambiguous)
{
	const int temp =
		lex.yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
	lex.prev_prev_keyword = lex.prev_keyword;
	lex.prev_keyword = temp;
	return temp;
}

int LexerState::yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	bool* stmt_ambiguous)
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
	SSHORT	c;

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

		tok_class = classes[c];

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
		char* p = string;
		for (; ptr < end && classes[static_cast<UCHAR>(*ptr)] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			check_copy_incr(p, UPPER7(*ptr), string);
		}
		
		check_bound(p, string);
		*p = 0;

		/* make a string value to hold the name, the name 
		 * is resolved in pass1_constant */

		yylval = (dsql_nod*) (MAKE_string(string, p - string))->str_data;

		return INTRODUCER;
	}

	/* parse a quoted string, being sure to look for double quotes */

	if (tok_class & CHR_QUOTE)
	{
		char* buffer = string;
		size_t buffer_len = sizeof (string);
		const char* buffer_end = buffer + buffer_len - 1;
		char* p;
		for (p = buffer; ; ++p)
		{
			if (ptr >= end)
			{
				if (buffer != string)
					gds__free (buffer);
				return -1;
			}
			// Care about multi-line constants and identifiers
			if (*ptr == '\n') {
				lines++;
				line_start = ptr + 1;
			}
			/* *ptr is quote - if next != quote we're at the end */
			if ((*ptr == c) && ((++ptr == end) || (*ptr != c)))
				break;
			if (p > buffer_end)
			{
				char* const new_buffer = (char*) gds__alloc (2 * buffer_len);
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
			*stmt_ambiguous = true; /* string delimited by double quotes could be
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
				yylval = (dsql_nod*) MAKE_string(buffer, p - buffer);
				dsql_str* delimited_id_str = (dsql_str*) yylval;
				delimited_id_str->str_flags |= STR_delimited_id;
				if (buffer != string)
					gds__free (buffer);
				return SYMBOL;
			}
		}
		yylval = (dsql_nod*) MAKE_string(buffer, p - buffer);
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

	fb_assert(ptr <= end);

	if ((tok_class & CHR_DIGIT) ||
		((c == '.') && (ptr < end) && (classes[static_cast<UCHAR>(*ptr)] & CHR_DIGIT)))
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
			if (have_exp_digit && (! (classes[c]  & CHR_DIGIT)))
				/* First non-digit after exponent and digit terminates
				 the token. */
				break;
			else if (have_exp_sign && (! (classes[c]  & CHR_DIGIT)))
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
				else if ( classes[c]  & CHR_DIGIT )
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
			else if (classes[c] & CHR_DIGIT)
			{
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = true;

				if (number >= limit_by_10)
				{
				/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
					}
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
			fb_assert(have_digit);

			if (have_exp_digit)
			{
				yylval = (dsql_nod*) MAKE_string(last_token, ptr - last_token);
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
					yylval = (dsql_nod*) (IPTR) number;
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
							   isc_arg_string,
							   ERR_string( last_token, ptr - last_token ),
							   isc_arg_end );
						ERRD_post_warning( isc_dsql_warning_number_ambiguous1,
							   isc_arg_end );
					}

					yylval = (dsql_nod*) MAKE_string(last_token, ptr - last_token);

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
		char* p = string;
		check_copy_incr(p, UPPER (c), string);
		for (; ptr < end && classes[static_cast<UCHAR>(*ptr)] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			check_copy_incr(p, UPPER (*ptr), string);
		}

		check_bound(p, string);
		*p = 0;
		dsql_sym* sym =
			HSHD_lookup (NULL, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
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
				(prev_prev_keyword == '(' || prev_prev_keyword == NOT || prev_prev_keyword == AND ||
				 prev_prev_keyword == OR || prev_prev_keyword == ON || prev_prev_keyword == HAVING ||
				 prev_prev_keyword == WHERE || prev_prev_keyword == WHEN) )
			{			
				LexerState savedState = lex;
				int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
				lex = savedState;
				if (nextToken == OR || nextToken == AND) {
					switch(sym->sym_keyword) {
					case INSERTING:
						yylval = (dsql_nod*) sym->sym_object;
						return KW_INSERTING;
					case UPDATING:
						yylval = (dsql_nod*) sym->sym_object;
						return KW_UPDATING;
					case DELETING:
						yylval = (dsql_nod*) sym->sym_object;
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
			if ((sym->sym_keyword == FIRST && !first_detection) ||
				sym->sym_keyword == SKIP)
			{
				if (prev_keyword == SELECT || limit_clause) {
					LexerState savedState = lex;
					int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
					lex = savedState;
					if (nextToken != NUMBER && nextToken != '?' && nextToken != '(') {
						yylval = (dsql_nod*) MAKE_string(string, p - string);
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return SYMBOL;
					}
					else {
						yylval = (dsql_nod*) sym->sym_object;
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return sym->sym_keyword;
					}
				} /* else fall down and return token as SYMBOL */
			}
			else {
				yylval = (dsql_nod*) sym->sym_object;
				last_token_bk = last_token;
				line_start_bk = line_start;
				lines_bk = lines;
				return sym->sym_keyword;
			}
		}
		yylval = (dsql_nod*) MAKE_string(string, p - string);
		last_token_bk = last_token;
		line_start_bk = line_start;
		lines_bk = lines;
		return SYMBOL;
	}

	/* Must be punctuation -- test for double character punctuation */

	if (last_token + 1 < end)
	{
		dsql_sym* sym =
			HSHD_lookup (NULL, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
		(prev_keyword == '(' || prev_keyword == NOT || prev_keyword == AND || prev_keyword == OR ||
		 prev_keyword == ON || prev_keyword == HAVING || prev_keyword == WHERE || prev_keyword == WHEN) )
	{
		LexerState savedState = lex;	
		brace_analysis = true;
		int openCount = 0;
		int nextToken;
		do {
			openCount++;
			nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
		} while (nextToken == '(');
		dsql_nod* temp_val = yylval;
		if (nextToken == INSERTING || nextToken == UPDATING || nextToken == DELETING)
		{
			/* Skip closing braces. */
			while ( openCount &&
					yylex(client_dialect, db_dialect,
						  parser_version, stmt_ambiguous) == ')')
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
					(prev_prev_keyword == '(' || prev_prev_keyword == NOT || prev_prev_keyword == AND ||
					 prev_prev_keyword == OR || prev_prev_keyword == ON || prev_prev_keyword == HAVING ||
					 prev_prev_keyword == WHERE || prev_prev_keyword == WHEN) )
				{
					savedState = lex;
					int token = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
					lex = savedState;
					if (token == OR || token == AND) {
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
		}
		else {
			/* Restore status quo. */
			lex = savedState;
		}
	}

	/* Single character punctuation are simply passed on */

	return c;
}


// The argument passed to this function is ignored. Therefore, messages like
// "syntax error" and "yacc stack overflow" are never seen.
static void yyerror(const TEXT* error_string)
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
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			isc_arg_gds, isc_command_end_err,	/* Unexpected end of command */
			0);
	else
	{
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			isc_arg_gds, isc_dsql_token_unk_err,
			isc_arg_number, (SLONG) lex.lines,
			isc_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Token unknown - line %d, char %d */
			isc_arg_gds, isc_random,
			isc_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token, 0);
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

	ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) sql_code, 
		isc_arg_gds, error_symbol, 0);
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
dsql_yyparse(USHORT client_dialect, USHORT db_dialect, USHORT parser_version, bool* stmt_ambiguous)
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
case 22:
{ prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
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
{ lex.g_field->fld_dtype = dtype_blob; }
break;
case 80:
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-2]; }
break;
case 81:
{ yyval = NULL; }
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
				yyvsp[-3], MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG)); }
break;
case 89:
{ yyval = yyvsp[-1]; }
break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((dsql_str*) FUN_reference, CONSTANT_SLONG));}
break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((dsql_str*) (-1 * FUN_reference), CONSTANT_SLONG));}
break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((dsql_str*) FUN_value, CONSTANT_SLONG));}
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG));}
break;
case 94:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
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
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 122:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 123:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 124:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 125:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 126:
{ yyval = (dsql_nod*) 0;}
break;
case 127:
{ yyval = yyvsp[-1]; }
break;
case 128:
{ yyval = NULL; }
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
{ yyval = NULL; }
break;
case 137:
{ yyval = NULL; }
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
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
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
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
break;
case 152:
{yyval = NULL;}
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
{yyval = NULL;}
break;
case 164:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 166:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 167:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 168:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 169:
{ lex.g_file  = make_file();}
break;
case 174:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 175:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 182:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 183:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 184:
{ yyval = yyvsp[0]; }
break;
case 185:
{ yyval = yyvsp[0]; }
break;
case 186:
{ yyval = NULL; }
break;
case 188:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 191:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 192:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 193:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 194:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 197:
{ yyval = NULL; }
break;
case 198:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 199:
{ yyval = yyvsp[0]; }
break;
case 200:
{ yyval = NULL; }
break;
case 201:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 202:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 203:
{ yyval = yyvsp[-1]; }
break;
case 204:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 205:
{ yyval = yyvsp[0]; }
break;
case 206:
{ yyval = NULL; }
break;
case 213:
{ yyval = NULL; }
break;
case 216:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 217:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 218:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 219:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 221:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 222:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 223:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 224:
{ yyval = yyvsp[0]; }
break;
case 225:
{ yyval = NULL ;}
break;
case 230:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 231:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 232:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 233:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 234:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 235:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 236:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 237:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 238:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 239:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 240:
{ yyval = NULL;}
break;
case 241:
{ yyval = yyvsp[0];}
break;
case 242:
{ yyval = yyvsp[0];}
break;
case 243:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 244:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 245:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 246:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 247:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 248:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 249:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 250:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 251:
{ yyval = make_list (yyvsp[-1]); }
break;
case 252:
{ yyval = NULL; }
break;
case 253:
{ yyval = make_list (yyvsp[-1]); }
break;
case 254:
{ yyval = NULL; }
break;
case 256:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 257:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 259:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 260:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 261:
{ yyval = yyvsp[0]; }
break;
case 262:
{ yyval = yyvsp[0]; }
break;
case 263:
{ yyval = (dsql_nod*) NULL; }
break;
case 264:
{ yyval = make_list (yyvsp[0]); }
break;
case 265:
{ yyval = NULL; }
break;
case 267:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 268:
{ yyval = yyvsp[-1]; }
break;
case 271:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 272:
{ yyval = NULL; }
break;
case 273:
{ yyval = NULL; }
break;
case 274:
{ yyval = yyvsp[0]; }
break;
case 275:
{ yyval = yyvsp[0]; }
break;
case 276:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 279:
{ yyval = yyvsp[-1]; }
break;
case 280:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 281:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 282:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 284:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 300:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 301:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 304:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 305:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 306:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 307:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 308:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 309:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 310:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 311:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 312:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 313:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 314:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 315:
{ yyval = NULL; }
break;
case 316:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 317:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 318:
{ yyval = make_list (yyvsp[0]); }
break;
case 319:
{ yyval = make_list (yyvsp[-1]); }
break;
case 320:
{ yyval = NULL; }
break;
case 321:
{ yyval = make_list (yyvsp[0]); }
break;
case 322:
{ yyval = make_list (yyvsp[-1]); }
break;
case 323:
{ yyval = NULL; }
break;
case 326:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 327:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 328:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 329:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 330:
{ yyval = NULL; }
break;
case 331:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 332:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 333:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 334:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 335:
{ yyval = NULL; }
break;
case 337:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 338:
{ yyval = make_node (nod_on_error, (int) e_err_count,
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
case 348:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 349:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 350:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 351:
{ yyval = NULL; }
break;
case 352:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 353:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 354:
{ yyval = make_list (yyvsp[-1]); }
break;
case 355:
{ yyval = NULL; }
break;
case 357:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 358:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 359:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 361:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 362:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 363:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 364:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 365:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 366:
{ yyval = make_list (yyvsp[0]); }
break;
case 368:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 371:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 372:
{ yyval = yyvsp[-1]; }
break;
case 373:
{ lex.beginning = lex_position(); }
break;
case 374:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 375:
{ lex.beginning = lex.last_token; }
break;
case 376:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 377:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 378:
{ yyval = 0; }
break;
case 379:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 380:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 381:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 383:
{ yyval = NULL; }
break;
case 384:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 385:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 403:
{ yyval = NULL; }
break;
case 404:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 405:
{ yyval = yyvsp[0]; }
break;
case 406:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 407:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 408:
{ yyval = yyvsp[0]; }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 411:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 412:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 413:
{ yyval = yyvsp[0]; }
break;
case 414:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 416:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 417:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = yyvsp[0]; }
break;
case 420:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 421:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 422:
{ yyval = yyvsp[0]; }
break;
case 423:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 425:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 426:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 427:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 428:
{ yyval = yyvsp[0]; }
break;
case 429:
{ yyval = yyvsp[0]; }
break;
case 430:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 431:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 432:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 433:
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
			  yyval = (dsql_nod*) lex.g_field; }
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
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 473:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 474:
{ yyval = yyvsp[0]; }
break;
case 475:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 477:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 478:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 479:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 481:
{ yyval = NULL; }
break;
case 483:
{ yyval = NULL; }
break;
case 484:
{ yyval = yyvsp[0]; }
break;
case 485:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 501:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 502:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 504:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 505:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 506:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 512:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
break;
case 513:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 514:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 515:
{ 
			*stmt_ambiguous = true;
			if (client_dialect <= SQL_DIALECT_V5)
				{
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
					ERRD_post_warning (isc_sqlwarn, isc_arg_number, (SLONG) 301, 
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
case 516:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 524:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 525:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 526:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 527:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 528:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 529:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 545:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 546:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 547:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 548:
{		 
			if ( ((IPTR) yyvsp[-1] < 1) || ((IPTR) yyvsp[-1] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((IPTR) yyvsp[-1] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (isc_sqlerr,
					   isc_arg_number, (SLONG) -817,
					   isc_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   isc_arg_number, (SLONG) db_dialect,
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
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					isc_arg_end );

					}
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else 
				if ((IPTR) yyvsp[-1] < 5)
					{
					lex.g_field->fld_dtype = dtype_short; 
					lex.g_field->fld_length = sizeof (SSHORT); 
					}
				else
					{
					lex.g_field->fld_dtype = dtype_long; 
					lex.g_field->fld_length = sizeof (SLONG); 
					}
			lex.g_field->fld_precision = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 549:
{ 
			if ( ((IPTR) yyvsp[-3] < 1) || ((IPTR) yyvsp[-3] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision should be between 1 and 18 */ 
			if (((IPTR) yyvsp[-1] > (IPTR) yyvsp[-3]) || ((IPTR) yyvsp[-1] < 0))
				yyabandon (-842, isc_scale_nogt);
				/* Scale must be between 0 and precision */
			if ((IPTR) yyvsp[-3] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (isc_sqlerr,
					   isc_arg_number, (SLONG) -817,
					   isc_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   isc_arg_number, (SLONG) db_dialect,
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
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					isc_arg_end );
				  }
				  /* client_dialect >= SQL_DIALECT_V6 */
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else
				{
				if ((IPTR) yyvsp[-3] < 5)
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
			lex.g_field->fld_precision = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT)(IPTR) yyvsp[-1];
			}
break;
case 552:
{ 
			if ((IPTR) yyvsp[0] > 7)
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
case 553:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 554:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 555:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
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
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 562:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 563:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 567:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 568:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 569:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 570:
{ yyval = 0; }
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
{ yyval = NULL; }
break;
case 581:
{ yyval = NULL; }
break;
case 582:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 584:
{ yyval = NULL; }
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
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 608:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 609:
{ yyval = (dsql_nod*) 0; }
break;
case 610:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 611:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 613:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 614:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 615:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
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
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 633:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 634:
{ yyval = yyvsp[-1]; }
break;
case 635:
{ yyval = 0; }
break;
case 636:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 637:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 638:
{ yyval = NULL; }
break;
case 639:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 640:
{ yyval = 0; }
break;
case 641:
{ yyval = yyvsp[0]; }
break;
case 642:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 643:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 644:
{ yyval = 0; }
break;
case 645:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 646:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 647:
{ lex.limit_clause = true; }
break;
case 648:
{ lex.limit_clause = false; }
break;
case 649:
{ lex.first_detection = true; }
break;
case 650:
{ lex.first_detection = false; }
break;
case 651:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 652:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 653:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 654:
{ yyval = 0; }
break;
case 655:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 656:
{ yyval = yyvsp[-2]; }
break;
case 657:
{ yyval = yyvsp[-1]; }
break;
case 658:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 659:
{ yyval = yyvsp[-1]; }
break;
case 660:
{ yyval = yyvsp[0]; }
break;
case 661:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 662:
{ yyval = 0; }
break;
case 663:
{ yyval = make_list (yyvsp[0]); }
break;
case 664:
{ yyval = 0; }
break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 668:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 671:
{ yyval = make_list (yyvsp[0]); }
break;
case 673:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 677:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 679:
{ yyval = NULL; }
break;
case 680:
{ yyval = make_list (yyvsp[-1]); }
break;
case 681:
{ yyval = NULL; }
break;
case 683:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 684:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 685:
{ yyval = yyvsp[-1]; }
break;
case 686:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 687:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 688:
{ yyval = make_list (yyvsp[-1]); }
break;
case 689:
{ yyval = NULL; }
break;
case 691:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 692:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 693:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 694:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 695:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 696:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 697:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 698:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 699:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 700:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 701:
{ yyval = make_list (yyvsp[0]); }
break;
case 702:
{ yyval = 0; }
break;
case 704:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
break;
case 711:
{ yyval = 0; }
break;
case 712:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 713:
{ yyval = 0; }
break;
case 714:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 715:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 716:
{ yyval = 0; }
break;
case 717:
{ yyval = 0; }
break;
case 719:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 720:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 723:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 725:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 726:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 728:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 729:
{ yyval = make_list (yyvsp[-1]); }
break;
case 730:
{ yyval = 0; }
break;
case 731:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 732:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 736:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 737:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 740:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 741:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 743:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 744:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 745:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 747:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 748:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 752:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 753:
{ yyval = yyvsp[0]; }
break;
case 755:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 758:
{ yyval = NULL; }
break;
case 759:
{ yyval = make_list (yyvsp[-1]); }
break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 763:
{ yyval = NULL; }
break;
case 764:
{ yyval = make_list (yyvsp[-1]); }
break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 770:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 776:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = yyvsp[-1]; }
break;
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 804:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 825:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 826:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 827:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 829:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 831:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 832:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 833:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 835:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 837:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 838:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 839:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 841:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 842:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 843:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 844:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 845:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 846:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 848:
{ yyval = make_list (yyvsp[-1]); }
break;
case 849:
{ yyval = yyvsp[-1]; }
break;
case 850:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 851:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 861:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 862:
{ yyval = yyvsp[0]; }
break;
case 863:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 864:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 865:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 866:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 867:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 868:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 869:
{ yyval = yyvsp[-1]; }
break;
case 870:
{ yyval = yyvsp[-1]; }
break;
case 874:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 875:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 876:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 879:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
break;
case 880:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 0, NULL);
			}
break;
case 881:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 882:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 884:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 886:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 887:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 888:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 889:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 890:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 891:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 893:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 894:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
			}
break;
case 895:
{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
break;
case 896:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
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
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 902:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 903:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 904:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 905:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 906:
{ yyval = yyvsp[0]; }
break;
case 907:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 909:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 910:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 911:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 912:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 913:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 915:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
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
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 936:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 938:
{ yyval = yyvsp[0]; }
break;
case 939:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 940:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 942:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
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
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 961:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 962:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 963:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 964:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 965:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 966:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 967:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
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
