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
#define IIF 523
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
  404,  402,  403,  403,  415,  415,  415,  416,  416,  417,
  417,  420,  420,  418,  418,  423,  423,  422,  419,  421,
  412,  412,  412,  412,  412,  412,  412,  412,  339,  339,
  146,   49,  284,  121,  139,  378,  156,  180,  270,   64,
   61,  215,  113,   67,  343,  211,   36,   42,  351,  269,
   40,   43,  208,   41,  297,  264,  264,  424,  424,  424,
  424,  424,  424,  424,  424,  424,  424,  424,  424,  424,
  424,  424,  424,  424,  424,
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
    3,    6,    1,    1,    6,    8,    6,    1,    1,    4,
    6,    4,    5,    3,    5,    4,    5,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,
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
   38,   35,   37,    0,   36,    0,  996,  998, 1000,  999,
 1005, 1001, 1008, 1006, 1002, 1004, 1007, 1003, 1009, 1010,
 1011, 1013, 1014, 1012, 1015,    0,    0,    0,    0,   33,
   69,   67,  988,  997,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  653,    0,    0,
    0,  690,    0,  990,    0,    0,    0,    0,  108,  995,
  567,    0,    2,    0,    0,    0,    0,  435,  436,  437,
  438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  452,  453,  454,  455,  456,  457,
  458,  459,  460,    0,  433,  434,    0,  980,    0,  412,
  984,    0,  409,  987,    0,  692,    0,  408,  991,    0,
  574,  906,    0,  151,  104,    0,  105,    0,  201,  976,
  770,    0,  103,  146,  983,    0,    0,   99,    0,  106,
  912,    0,  100,    0,  101,    0,  102,  994,  147,  107,
  629,  630,    0,  911,  909,    0,   75,    0,  981,    0,
  979,  492,  485,    0,  491,  496,  486,  487,  494,  488,
  489,  490,  495,    0,    0,    0,   31,    0,   40,  757,
   39,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  916,    0,  897,  647,
  647,  648,  658,  660,  969,  661,    0,  662,  652,  648,
    0,    0,    0,    0,    0,    0,    0,  593,  582,    0,
  585,  587,  588,  589,  590,  596,  597,    0,  989,  691,
    0,  109,    0,  110,    0,  111,    0,    0,    0,  622,
    0,    0,    0,    0,    0,    0,    0,  472,    0,    0,
  375,    0,  204,    0,  415,  406,  469,  470,    0,    0,
    0,    0,    0,    0,  424,    0,  381,  382,    0,  580,
  578,  907,    0,    0,    0,    0,    0,    0,    0,  154,
  133,    0,   97,    0,    0,  112,    0,  122,  121,    0,
    0,    0,    0,    0,    0,  972,   76,    0,    0,    0,
  735,  736,  493,    0,    0,    0,    0,  874,  903,    0,
  971,    0,  876,    0,    0,  904,    0,  898,  889,  888,
  887,    0,    0,    0,    0,  879,  880,  881,  890,  891,
  899,  900,    0,  901,  902,    0,    0,    0,  905,    0,
    0,    0,    0,    0,    0,  893,    0,  767,  871,  872,
  873,  878,  877,  352,  857,    0,    0,  856,    0,    0,
  853,  854,  855,  858,  859,  860,  892,  917,  918,  919,
  920,  933,  935,  943,  944,  948,  949,  202,    0,    0,
    0,  356,    0,    0,    0,  760,    0,    0,   28,    0,
    0,    0,   72,   70,  992,   68,   34,    0,    0,    0,
  762,    0,   53,    0,    0,    0,    0,    0,  572,    0,
    0,    0,  655,  657,    0,  664,    0,    0,    0,  665,
  651,    0,  619,    0,  594,    0,  591,    0,  592,  617,
    0,  612,    0,    0,  586,  771,  742,    0,    0,    0,
    0,    0,    0,  569,  568,    0,    0,  626,  623,    0,
    0,    0,  169,    0,    0,  130,    0,  477,    0,  478,
  473,    0,    0,  419,  421,  420,    0,  422,    0,  416,
    0,    0,  255,    0,    0,    0,  428,  429,    0,  462,
    0,    0,    0,    0,    0,    0,    0,  386,  385,  480,
    0,    0,  150,    0,    0,  159,    0,  158,    0,    0,
  165,  148,    0,  163,  155,    0,    0,  515,  550,  518,
    0,  551,    0,    0,  519,    0,    0,    0,  542,  554,
  514,  537,  516,  517,  512,  499,  375,  497,  498,  500,
    0,    0,  509,  510,  511,  513,    0,    0,    0,    0,
    0,  113,    0,  114,    0,  125,    0,    0,  185,    0,
    0,  373,    0,  204,    0,    0,   84,    0,    0,  752,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  773,  775,  784,  787,  788,  789,  790,  791,  792,
  793,  794,  795,  796,    0,    0,    0,  894,    0,    0,
    0,    0,    0,    0,  895,  896,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  317,  993,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  260,  507,    0,    0,    0,  354,    0,    0,
    0,  759,   29,    0,    0,   45,   73,    0,    0,   27,
    0,  765,    0,    0,    0,  732,    0,    0,    0,    0,
    0,  571,    0,  647,    0,  669,    0,    0,    0,    0,
  562,    0,  914,  561,    0,  595,    0,  603,  599,  598,
    0,    0,    0,  614,  602,    0,    0,  740,  741,    0,
    0,    0,  373,    0,    0,    0,    0,    0,  620,    0,
  131,    0,  476,  375,  418,    0,  375,  376,  423,    0,
    0,  251,  373,  224,  977,    0,    0,  375,    0,    0,
  193,  373,    0,    0,    0,    0,  229,  223,  226,  227,
  228,  427,  467,  466,  426,  425,    0,    0,    0,    0,
    0,    0,  384,    0,  375,    0,  156,  160,    0,    0,
  164,    0,    0,  538,  539,  555,    0,  552,    0,    0,
  545,  543,  544,    0,    0,    0,    0,  508,    0,    0,
    0,    0,  546,    0,    0,  373,    0,  184,    0,  187,
  189,  190,    0,    0,    0,    0,    0,  204,   79,    0,
    0,   78,  526,  973,  527,    0,    0,    0,    0,  774,
  786,    0,    0,  844,  845,  846,    0,  778,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  961,  962,  963,  964,  965,  966,  967,  968,    0,
    0,    0,    0,    0,    0,  954,    0,    0,    0,    0,
    0,  869,  319,  870,  941,    0,  865,  975,    0,    0,
    0,    0,    0,    0,    0,  875,  769,  768,  358,  357,
    0,  258,    0,    0,    0,  266,  761,    0,    0,    0,
    0,    0,    0,    0,    0,   54,   62,   64,    0,    0,
   71,    0,    0,    0,  764,    0,    0,    0,    0,    0,
    0,    0,    0,  656,  659,  985,  668,    0,    0,    0,
    0,  674,  675,  676,    0,    0,  666,  563,  915,  604,
  613,  608,  607,    0,  618,  772,  743,    0,  373,    0,
    0,  649,  628,  627,    0,    0,  639,  643,  475,    0,
    0,  168,    0,  172,    0,    0,    0,  417,  373,  373,
  263,  376,  256,    0,  195,  375,  197,    0,  376,  192,
  198,    0,    0,    0,    0,  430,  431,  375,  373,  376,
    0,    0,    0,  402,    0,  180,  181,  157,    0,  167,
  913,    0,    0,    0,    0,    0,  553,    0,    0,  375,
  376,    0,  503,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  182,    0,    0,    0,
    0,  362,    0,   83,    0,    0,    0,    0,   88,   85,
    0,    0,    0,  737,  978,    0,    0,  779,    0,    0,
  785,    0,    0,    0,    0,    0,  822,  821,    0,    0,
    0,  829,  847,  839,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  777,    0,    0,    0,  204,    0,
  921,    0,    0,  936,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  950,    0,    0,    0,
    0,    0,    0,    0,    0,  940,  882,    0,  253,  272,
    0,    0,  353,  267,   66,   58,   59,   65,   60,   61,
    0,    0,   23,    0,    0,    0,   43,    0,    0,    0,
  746,  766,    0,    0,  745,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  693,    0,    0,    0,
    0,    0,  610,  611,  615,    0,  183,    0,    0,    0,
    0,    0,    0,  173,    0,    0,    0,  413,  207,  208,
  209,  210,  211,  212,  885,    0,    0,  257,    0,    0,
  205,    0,    0,    0,    0,  230,    0,  463,  464,  432,
    0,    0,    0,    0,    0,    0,  375,  482,  376,  166,
  974,    0,    0,  521,    0,  520,  556,    0,  548,    0,
    0,    0,  501,    0,  529,    0,    0,  531,  534,  536,
  373,    0,    0,    0,  120,    0,  188,  375,    0,    0,
  374,    0,   98,    0,  118,    0,   94,    0,    0,    0,
    0,   87,    0,  837,  838,  781,    0,  783,    0,    0,
    0,    0,    0,    0,    0,  840,    0,    0,    0,    0,
    0,    0,    0,  830,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  927,  926,
    0,    0,  923,  922,    0,  931,  930,  929,  928,  925,
  924,    0,    0,  956,    0,    0,    0,  955,    0,    0,
    0,    0,    0,  259,    0,    0,  269,  270,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  351,    0,    0,    0,    0,  278,  283,  277,    0,
    0,    0,  287,  288,  289,  290,  291,  292,  294,  295,
  297,  298,  299,  302,    0,    0,    0,  345,  346,  347,
    0,    0,   57,   63,   55,   56,   24,  750,  751,    0,
  748,  756,  755,  753,    0,  731,    0,    0,    0,  685,
    0,    0,    0,  699,  695,  697,    0,    0,    0,    0,
    0,  374,  632,  633,  650,    0,  178,  174,    0,  376,
  886,  261,  262,  376,    0,    0,    0,  215,    0,    0,
  231,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  479,  523,    0,  524,    0,  135,    0,    0,    0,  139,
  141,  142,  143,  504,  506,  502,  533,    0,    0,  376,
    0,    0,    0,    0,  359,    0,  363,  375,   89,    0,
    0,   91,    0,    0,    0,    0,    0,    0,  848,  849,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  203,  942,
  932,  934,    0,    0,  952,  951,    0,  957,  945,  947,
    0,    0,    0,  268,    0,  306,    0,    0,    0,  301,
    0,    0,  300,  331,  332,    0,  986,  348,  349,    0,
  286,  293,    0,  296,  279,    0,  284,    0,  336,  285,
  303,    0,    0,  329,   41,    0,    0,    0,  688,  686,
    0,    0,    0,  703,    0,    0,  645,  376,  360,  634,
  175,  376,  145,  250,    0,    0,  191,  216,    0,    0,
    0,    0,  217,  220,    0,    0,  376,  400,  401,  397,
  396,  399,  398,    0,  522,  549,  144,  132,  140,    0,
  375,  247,  127,  376,    0,    0,  377,  364,    0,    0,
   92,   93,   80,    0,  808,  816,    0,  809,  817,  812,
  820,    0,    0,  810,  818,  811,  819,  805,  813,  806,
  814,  807,  815,    0,  937,  953,    0,    0,    0,  275,
  271,    0,  304,    0,    0,    0,    0,    0,  333,    0,
    0,  324,  325,    0,  344,    0,    0,    0,    0,  339,
  337,    0,    0,    0,  747,    0,    0,  678,    0,    0,
  713,  715,    0,  710,    0,  248,  414,  194,  199,  218,
    0,    0,  221,    0,  233,  235,  404,  376,    0,  379,
    0,    0,    0,  369,  370,    0,    0,   77,   95,    0,
  946,    0,  274,    0,  305,    0,    0,  308,    0,  314,
    0,  316,    0,    0,  343,  342,  982,  341,    0,    0,
    0,    0,    0,    0,    0,  677,    0,  704,  714,    0,
  222,    0,    0,    0,    0,    0,  249,  376,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  327,  326,  338,  340,    0,    0,    0,    0,
  682,    0,    0,  721,    0,    0,    0,    0,    0,    0,
  232,    0,  239,    0,  238,  380,  372,    0,    0,  365,
  119,    0,    0,  276,    0,    0,  307,  311,    0,    0,
    0,    0,    0,    0,    0,  680,  723,  712,    0,    0,
  724,    0,  720,  219,    0,    0,  243,  242,  241,    0,
    0,    0,    0,    0,    0,    0,    0,  328,    0,  683,
  719,    0,    0,  246,  244,  245,    0,    0,  851,  322,
  312,    0,  334,  309,  646,    0,    0,    0,  726,    0,
    0,  310,    0,  725,    0,    0,  728,    0,  850,  729,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1413,   28,   29, 1414,
   31,   32,   33,   34,   35,   36,   37, 1415,   39, 1416,
  650,  126,  478,  974, 1213,  127,  725,  975,  128,  482,
  730,  152,  129,  130,  269,  999,  142,  976,  977,  207,
  236,  131,  978,  132,  484,   83,  247,  387,  435,  645,
 1107,  436,  871,  616,  229,  848,  646,  647,  648, 1109,
  248, 1448,   75,  197,   76,  243, 1886, 1323,  228,  233,
  235,  237,  223,  215,  217,  230,  240,  159,  322,  324,
  326,  376,  632,  634,  475,  270,  797,  553,  660, 1038,
  380,  637, 1095, 1315,  584,  753, 1068,  545,  546,  566,
  372,  617, 1081, 1508, 1627, 1258, 1509, 1510, 1511, 1512,
 1513,  661,  224,  216,  368,  592,  369,  370,  593,  594,
 1290,  547, 1032, 1033, 1034, 1253,  382,  859,  860,  861,
  862,  799, 1049, 1486,  618,  801,  438,  864,  947,  469,
 1372, 1259,  439,  440,  441,  442,  443, 1487, 1488,  569,
 1633, 1784, 1276,  807,  808,  794,  809,  810,  811, 1785,
 1786, 1858,  350,  474,  964, 1417,  203,  562,  961,  563,
 1042,  470,  965,  966, 1201, 1396, 1397, 1398, 1691, 1114,
  736, 1418, 1419, 1420, 1421, 1598, 1422, 1423, 1424, 1425,
 1426, 1427, 1428, 1429, 1430, 1431, 1432, 1433, 1434, 1435,
  444, 1799, 1436, 1701, 1842, 1698,  445,  701,  446,  447,
 1437, 1599, 1709, 1710, 1766, 1438, 1439, 1440, 1590,  266,
  471,  472,  448, 1100, 1321, 1525, 1101, 1102,  297,  508,
 1656,  391, 1242, 1470, 1742, 1743, 1744, 1745, 1240,  359,
  580,  825, 1288,  581,  823,   80,   60,  354,  208,  167,
  337,  194,  344,  200,  788,  785,  345,  355,  815,  356,
  576, 1060,  195,  449,  338,  582, 1289,   97,  206,  252,
  619,  620,  714, 1082,  715, 1083, 1084,  623,  624,  625,
  626, 1075, 1073,  875,  627,  628,  629,  841,  630,  838,
   41,   42,   43,   44,   45,   46,  161,  535,   62,  500,
  211,  361,  309,  310,  311,  312,  313,  314,  315,  316,
  317,  759,  521, 1014, 1245,  522,  523,  764,   47,  166,
  332,  542,  779,   48,  537,  538, 1023, 1475, 1249, 1620,
 1027,  146,  749, 1617,  503,  299,  147,  148,  298,  509,
  510,  747,  997, 1000, 1001, 1002, 1003, 1004, 1717, 1776,
  450, 1812, 1234, 1613, 1614, 1814, 1725, 1815, 1816, 1817,
 1853, 1887, 1889,  490,   49,   50,  392,   51,   52,  529,
  530,  983, 1221, 1450, 1454,  491,  733,  221,  662,  663,
  889,  890,  664,  665,  666,  667,  668,  669,  670,  671,
  672,  673,  674, 1345, 1130, 1132, 1133,  700,  451,  452,
  453,  454,  455,  456,  457,  245,  458,  459,  460,  461,
  462,  930,  463, 1564,  464,  465,  466,  467,  690,  934,
 1190, 1185,  691,  134,
};
static short yysindex[] = {                                   4251,
 1383, -135, 3718,  125, 1398,   64, 3361,  -24, 8692,  534,
  631,12957, -135, 1330,  726, 9857,  730, 9857,  -14,    0,
  773,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   87,    0,    0,    0,
    0,    0,    0,13622, 9857, 9857, 9857, 9857, 9857,    0,
    0,  653, 1392, 9857, 9857, 9857,  936, 9857,  801, 9857,
 9857,    0, 9857, 9857,    0,   53,    0,  813,    0,    0,
  909, 9857,    0, 9857, 9857, 9857,  983, 9857, 9857, 9857,
 9857,  801, 9857, 9857, 9857, 9857,    0, 9857, 1252,  989,
    0,    0,    0, 1318,    0, 1318,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1001, 1022,  239, 1397,    0,
    0,    0,    0,    0, 9857, 9857, 9857, 1060, 1080, 1090,
  122, 1319, 1039,  142,  189,  938, 1013,    0, 9857, 1186,
 1402,    0, 1113,    0, 9857, 9857, 9857, 9857,    0,    0,
    0, 9857,    0, 1271,  113, 1032, 1370,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  721,    0,    0, 1392,    0,  524,    0,
    0, 1533,    0,    0, 1484,    0,  658,    0,    0, 1137,
    0,    0, 1117,    0,    0, 1242,    0, 1329,    0,    0,
    0, 1392,    0,    0,    0, -110, 1533,    0,  604,    0,
    0, 1294,    0, 1308,    0, 1318,    0,    0,    0,    0,
    0,    0, 1302,    0,    0, 1173,    0, 1327,    0, 1202,
    0,    0,    0, 1173,    0,    0,    0,    0,    0,    0,
    0,    0,    0,12102, 9857, 1256,    0, 9857,    0,    0,
    0, 7083, 1270, 9263, 9857, 1187, 1350, 1666, 1388, 1405,
 7083, 1355, 9263, 1379, 1404, 1269,    0,12341,    0,    0,
    0,    0,    0,    0,    0,    0,10378,    0,    0,    0,
 1360, 9857, 1453, 1387,  783, 9857, 1395,    0,    0, 1402,
    0,    0,    0,    0,    0,    0,    0, 9857,    0,    0,
 1533,    0, 1294,    0, 1318,    0, 1449,12341, 1430,    0,
12341, 1546,  -16, 1373, 1349, 1378, 1370,    0, 1584, 1321,
    0, 9857,    0,  721,    0,    0,    0,    0, 9857, 1256,
 7510, 1437, 8076, 1860,    0,13622,    0,    0, 1448,    0,
    0,    0, 1853, 1853, 1392, 1544, 1392,  514, 1242,    0,
    0, 9073,    0, 9857, 9857,    0, 1256,    0,    0, 1632,
  643, 1901, 9857, 1663, 9857,    0,    0, 1912,  110, 7361,
    0,    0,    0, 1921, 1931, 1938, 1472,    0,    0, 1942,
    0, 1943,    0, 1975, 1981,    0, 1983,    0,    0,    0,
    0,    0, 1985, 1527, 1574,    0,    0,    0,    0,    0,
    0,    0, 1987,    0,    0,10608, 2005, 2007,    0, 2017,
12341,12341,10862, 9857, 2018,    0, 2225,    0,    0,    0,
    0,    0,    0,    0,    0, 1995, 1968,    0,    0, 2029,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 9073, 2002,
 1699,    0, 2036, 1817, 1742,    0, 9857, 1647,    0, 9857,
 9857,  150,    0,    0,    0,    0,    0, 9857, 9857,  872,
    0, 9857,    0, 1775, 9857, 2048, 7083, 1703,    0, 9857,
10862, 2214,    0,    0,12341,    0, 2171, 1786, 2068,    0,
    0,   63,    0, 1119,    0,  610,    0,  610,    0,    0,
 2071,    0,  146, 1693,    0,    0,    0, 2076,   81, 2062,
 1256, 2086, 1863,    0,    0, 1799, 2093,    0,    0, 1303,
 1697, 1687,    0, 1810, 1813,    0, 1392,    0, 1814,    0,
    0, 1885, 1886,    0,    0,    0, 1868,    0, 9073,    0,
 9073, 1772,    0, 1883, 9857,13447,    0,    0,  615,    0,
 9857,  327, 1484, 1773, 1733, 1701,    0,    0,    0,    0,
  638, 1777,    0, 1718,  801,    0, 1392,    0, 1900, 1831,
    0,    0,  514,    0,    0,  164, 1735,    0,    0,    0,
 1736,    0, 1782, 2144,    0, 1838, 2146,  547,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2098,  129,    0,    0,    0,    0,   95, 2150, 2153, 2146,
 1533,    0, 1888,    0, 1939,    0, 1392, 1392,    0, 7510,
  658,    0, 1825,    0, 1798, 2167,    0, 7700, 3747,    0,
 1826, 1843, 2179, 7609, 2188,    0,    0,    0, 7264, 8473,
   33,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  969,12341,  764,    0, 9857,12341,
 1182, 1192, 1200, 2998,    0,    0,12341, 7848, 2225, 1780,
  439,12341,12341, 7848,  720,  720, 1330, 2336, 1781, 2191,
    0,    0,10948, 9857,12341,12341,12341,12341,12341,12341,
12341, 3139,    0,    0, 1957, 2172, 9857,    0, 9857, 1941,
 9857,    0,    0, 9512, 1801,    0,    0, 1778, 9263,    0,
 1924,    0, 1807, 1330, 2204,    0, 1924, 9512, 1925, 1928,
 9857,    0, 2336,    0, 2358,    0, 9857, 2096, 1800,12341,
    0, 1166,    0,    0,  -41,    0, 1816,    0,    0,    0,
 9857,  -11, 9857,    0,    0, 9857, 9857,    0,    0,12341,
 1994, 7510,    0, 1771,12341,12341, 1889, 1776,    0, 1392,
    0,  415,    0,    0,    0, 2234,    0,    0,    0,  165,
 9857,    0,    0,    0,    0, 2009, 2245,    0, 1990, 2003,
    0,    0, 1950, 1952, 1318, 2014,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  801, 9857,13567, 1909,
 1917, 1918,    0,  801,    0, 1468,    0,    0, 1890, 1392,
    0,   73, 1894,    0,    0,    0,  801,    0, 2144,  162,
    0,    0,    0, 2006,  162, 1891,  162,    0, 2265,  801,
  801,  801,    0, 1256, 9857,    0, 1955,    0, 1841,    0,
    0,    0, 1448, 1897, 9857, 1847,   96,    0,    0, 2282,
 2052,    0,    0,    0,    0,  110, 9857, 1911, 7945,    0,
    0, 1911, 7609,    0,    0,    0, 7898,    0,  375,  462,
12341,12341, 8203, 2284,  518, 8461,12341, 8561,  981, 8819,
 9077,11178, 9177, 9435, 9693, 7848, 7848,12341,12341, 2792,
12341, 2288,12341, 2287, 2378,12341,12341,12341,12341,12341,
12341,    0,    0,    0,    0,    0,    0,    0,    0, 2012,
 1492,   25,12341,  559,12341,    0, 7848, 2602, 2889,  655,
  938,    0,    0,    0,    0, 1861,    0,    0, 2059,  720,
  720, 1043, 1043, 2225, 1190,    0,    0,    0,    0,    0,
 1869,    0, 1913, 2069, 1941,    0,    0, 9857, 9857, 9857,
 9857, 9857, 9857,  190, 2299,    0,    0,    0, 9512, 1961,
    0,  -18, 1980, 9857,    0,  938,12341, 1980, 2304, 2299,
 9512, 4811, 2033,    0,    0,    0,    0, 1464, 2318, 2316,
 1135,    0,    0,    0, 7848, 2037,    0,    0,    0,    0,
    0,    0,    0,  703,    0,    0,    0, 2225,    0, 1879,
 1897,    0,    0,    0, 2225, 9857,    0,    0,    0, 1853,
 2100,    0,  415,    0, 2327, 7848, 5352,    0,    0,    0,
    0,    0,    0, 1941,    0,    0,    0, 5352,    0,    0,
    0, 1318, 1318, 1872, 2329,    0,    0,    0,    0,    0,
  399,  387,  797,    0, 2106,    0,    0,    0, 9857,    0,
    0,  125, 1902, 1954, 1957, 2333,    0, 1718, 1906,    0,
    0, 1347,    0, 2322, 9857, 1435,  801, 2341, 2343, 2344,
 2125,  658, 1941, 1853, 1813, 7510,    0, 1777,  938, 1937,
 1956,    0, 1608,    0,  801, 2001, 2097, 7700,    0,    0,
  801, 1915, 2099,    0,    0, 2370, 2373,    0, 7848, 7848,
    0, 7848, 7848, 2820, 2225, 2375,    0,    0, 2225, 2377,
11432,    0,    0,    0, 2070, 2384, 2225, 2388, 1731, 2389,
 2225, 2392,12341,12341, 2284,12341,11518, 2394, 2225, 2396,
 2397, 2225, 2400,12341, 2225, 2401, 2225, 2402, 2403, 2225,
 2404, 2406, 2225, 2409,    0, 2164, 2442, 2450,    0, 2537,
    0, 2678,12341,    0, 2696, 2724, 3010, 3047, 3071, 3110,
12341,12341,12341, 2225, 2019,12341,    0,12341, 2225, 2145,
  539,12341,12341,12341,12341,    0,    0, 9857,    0,    0,
 9857,13316,    0,    0,    0,    0,    0,    0,    0,    0,
 2137, 9512,    0, 9512,  190, 2299,    0,  251,  251,  148,
    0,    0,10378, 1907,    0, 2304, 2299, 2408, 4811, 2424,
 1135, 2430,12341, 2208, 2096, 2085,    0, 2088, 2089, 2135,
 2212, 2151,    0,    0,    0, 1941,    0, 1937,  684, 2456,
 1718, 2113, 1718,    0, 7848,  429, 1411,    0,    0,    0,
    0,    0,    0,    0,    0, 5352, 5352,    0, 2069,12341,
    0, 2219, 2105, 1872,   53,    0, 7848,    0,    0,    0,
 2121, 2127, 2128, 2130, 2131, 2133,    0,    0,    0,    0,
    0, 2480,  125,    0, 2064,    0,    0,  162,    0, 5352,
 2148,  162,    0,  162,    0, 1957, 2483,    0,    0,    0,
    0, 1448, 2069, 1718,    0, 1813,    0,    0,10378, 2249,
    0,  484,    0, 2456,    0, 2488,    0, 2489, 1392,  -35,
 2497,    0, 1392,    0,    0,    0, 2164,    0, 2164,12341,
 2112, 2112, 1330, 1916, 2499,    0, 2112, 2112,12341, 2112,
 2112, 3383, 2225,    0, 1893,12341, 2225, 2112, 2112, 2112,
 2112, 2225, 2112, 2112, 2112, 2112, 2112, 2112,    0,    0,
 9073, 2500,    0,    0, 3227,    0,    0,    0,    0,    0,
    0, 3235, 1887,    0,12341, 2237, 2107,    0,12341, 3337,
 1951, 3373, 1711,    0, 9073, 2494,    0,    0, 2255,    0,
 4275,    5, 2496, 2517, 2220,12341, 2506, 2509, 4589, 9857,
 9857,    0, 2510, 2512, 2232, 2514,    0,    0,    0, 2251,
13261, 2515,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2518,  -60, 2529,    0,    0,    0,
    0, 2205,    0,    0,    0,    0,    0,    0,    0, 2155,
    0,    0,    0,    0, 1786,    0, 4811, 2408, 2208,    0,
 1966, 9857, 1135,    0,    0,    0, 2096,12341, 7848, 2193,
 2069,    0,    0,    0,    0, 1468,    0,    0,  436,    0,
    0,    0,    0,    0, 2225, 2312, 2219,    0,  955, 9857,
    0, 2257,  463, 2158, 2264, 2161, 2308, 2272, 2314, 1941,
    0,    0, 2576,    0, 2581,    0, 2271, 2312, 2148,    0,
    0,    0,    0,    0,    0,    0,    0, 1941, 1777,    0,
 1468, 2106, 2305, 2239,    0, 1897,    0,    0,    0, 2256,
   24,    0, 1957, 2259, 2225, 2585, 2587,  938,    0,    0,
 2590, 2592, 2225, 2594, 2595,12341,12341, 2225, 2598, 2600,
 2601, 2607, 2610, 2612, 2613, 2614, 2615, 2616,    0,    0,
    0,    0,12341, 2619,    0,    0,12341,    0,    0,    0,
12341, 1800,  326,    0, 2347,    0,11766, 9857,12341,    0,
 7848, 2225,    0,    0,    0, 2603,    0,    0,    0, 9857,
    0,    0, 4747,    0,    0,  665,    0, 2210,    0,    0,
    0,  447, 2628,    0,    0,  251, 1800, 9857,    0,    0,
 1005, 2225, 2627,    0,   33,  754,    0,    0,    0,    0,
    0,    0,    0,    0, 2633, 9857,    0,    0, 2323, 2332,
 9857, 1872,    0,    0, 1318, 9857,    0,    0,    0,    0,
    0,    0,    0, 2069,    0,    0,    0,    0,    0, 2069,
    0,    0,    0,    0, 2526, 1800,    0,    0,12341, 1392,
    0,    0,    0, 1392,    0,    0,12341,    0,    0,    0,
    0, 2225, 2225,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2225,    0,    0, 3414, 2037, 5352,    0,
    0, 2637,    0, 1657,12102, 1637,  466, 2625,    0, 2338,
  970,    0,    0, 2641,    0, 9857, 9857,  125,  139,    0,
    0, 2185, 2354, 7848,    0, 2037, 2655,    0, 7848,12341,
    0,    0, 2330,    0, 2659,    0,    0,    0,    0,    0,
 1872, 1318,    0, 2317,    0,    0,    0,    0, 2106,    0,
 2526, 2660, 1135,    0,    0, 2037, 2225,    0,    0, 1711,
    0, 2151,    0, 1911,    0, 2295, 4747,    0, 2274,    0,
 4747,    0, 4747, 2836,    0,    0,    0,    0,13316,  665,
12341, 4747,  737, 2151, 9857,    0,   33,    0,    0, 9950,
    0, 2317,  435, 1872, 2324, 2331,    0,    0, 1135, 2665,
 2526, 2367, 2151, 2671, 1800, 2193, 2672, 1643, 2658, 1058,
13316, 2674,    0,    0,    0,    0, 1704,  225, 2420, 2193,
    0, 1988, 9857,    0, 2687, 2685, 1415, 1872,  833,  833,
    0, 2432,    0, 2289,    0,    0,    0, 1135, 2526,    0,
    0, 2037, 2355,    0, 4747, 2674,    0,    0, 2427, 4747,
 2451, 2441,13316, 2355, 9857,    0,    0,    0, 9950, 2707,
    0, 9857,    0,    0, 2286,  778,    0,    0,    0, 1352,
 2151, 1032, 1992,13316,  206, 9857,13316,    0, 1032,    0,
    0, 9857, 2414,    0,    0,    0, 7848, 2193,    0,    0,
    0,13316,    0,    0,    0, 2709, 2715, 2717,    0,   33,
 2355,    0, 9857,    0, 9857, 1032,    0, 2718,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  218, 1057,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  143, 9779,    0,    0,    0,    0,    0,    0,
 2758,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  157,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  441,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2423,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -34, 2380,
    0,    0,    0,   98,    0,   98,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2385,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1085,    0,    0,12005,10038,    0,    0,    0,
 1158,    0,    0,    0, 4664,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  487,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   47,    0,    0,    0,    0, 1093,    0,    0, 1236,
    0,    0,    0,    0,    0,  104,    0, 9110,    0,    0,
    0,    0,    0,    0,    0,    0,   47,    0,  459,    0,
    0, 2733,    0,    0,    0, 2519,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1243,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1266,    0, 2520,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1217,    0,    0,
    0,    0,    0,    0,    0,10180,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  891,    0,    0, 1324,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   47,    0, 2733,    0, 2519,    0, 1361,    0,    0,    0,
    0,  219,    0,    0,    0,    0, 1369,    0, 2498,    0,
    0,    0,    0, 1427,    0,    0,    0,    0,    0, 2520,
  445,13676,    0, 1432,    0,    0,    0,    0,  268,    0,
    0,    0, 2337, 2337,    0,    0,    0, 1467,  137,    0,
    0,    0,    0,    0,    0,    0, 2520,    0,    0, 1552,
    0,    0,    0,    0,    0,    0,    0, 6295, 2398,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1958,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2799,    0,    0,    0, 3103, 3407,    0, 3711,
    0,    0,    0,    0,    0,    0,  853,    0,    0,    0,
    0,    0,    0,    0,    0,  235, 4015,    0, 2490,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1493,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1505,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 3444,    0, 2454,    0,
    0,    0,    0,    0,    0,  885,    0,  885,    0,    0,
 1136,    0, 1142, 1317,    0,    0,    0,    0, 1243,    0,
 2520,    0,    0,    0,    0,  316,  267,    0,    0,  569,
    0,  883,    0,    0, 1385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2498,    0,
    0, 1216,    0, 2311,    0,    0,  685,    0,    0,    0,
    0,  641,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1516,    0,    0, 6330,12702,    0,    0,    0,
12867,    0,    0,12913,    0,    0,12940,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1750, 1949,    0,    0,    0,    0,13142,12875,    0,12940,
   47,    0,    0,    0,    0,    0,    0,    0,    0,  445,
 1525,    0,    0,    0,    0, 2374,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 6410, 6493, 6632,    0,    0,
  553,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12589,    0,12589,    0,    0,    0,
12589,12589,12589,    0,    0,    0,    0,    0, 2339,    0,
    0,    0,    0,    0, 5087, 5391,10292, 2744,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2503,    0,    0,    0,    0, 2525,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  323,    0,    0, 9779,    0,    0,  323,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1011,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  769,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  445,    0,  232,    0,    0,  289,    0,    0,    0,
    0,  854,    0,    0,    0,    0,    0,    0,    0, 1999,
    0,    0,    0,    0,    0, 2751,    0,    0, 2372, 1997,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  279,
  403,  529,    0,    0,    0,  480,    0,    0,    0,    0,
    0,    0, 8729,    0,    0,    0,    0,    0,12913,    0,
    0,    0,    0,  775,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2520,    0,    0,  303,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 8199,    0,    0,    0,
  281,    0,    0,    0,    0, 2491,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12589,    0,    0,    0,    0,    0,    0,    0, 4319, 5623,
 5927, 4551, 4855, 1040,    0,    0,    0,    0,    0,    0,
    0,    0,10515,    0, 2531,    0,    0,    0,    0,    0,
    0,    0,    0, 1517, 1555,    0,    0,    0,    0,    0,
    0,    0, 1577,    0,    0,12005,    0, 1577, 1591, 1596,
    0,    0,    0,    0,    0,    0,    0,    0,  914,  184,
  625,    0,    0,    0,    0, 1047,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  231,    0,    0,
    0,    0,    0,    0,  714,    0,    0,    0,    0, 2337,
 2346,    0,  876,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2525,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1336,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1605,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 7258,    0,    0,    0,    0,    0,
    0,    0,    0, 1436,    0,    0,    0,    0,    0,    0,
    0, 1525, 2525, 2337, 1653,  445,    0, 2541,12005, 1658,
  144,    0,    0,    0,    0, 8199,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  878,    0,    0,    0, 1867,    0,
    0,    0,    0,    0,    0,    0, 2570,    0, 2879,    0,
 3183,    0,    0,    0,    0,    0,    0,    0, 3487,    0,
    0, 3791,    0,    0, 4095,    0, 4631,    0,    0, 5167,
    0,    0, 5703,    0,    0, 1171,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2371,    0,    0,    0,    0,  624,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  363,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1517, 1665,    0, 2379,  346,    0,
    0,    0,    0,    0,    0, 1668, 1671, 1705,    0,    0,
 2464, 1177,    0, 2180,    0, 2473,    0, 2476, 2477,    0,
    0, 1068,    0,    0,    0, 2525,    0, 1658,    0,  386,
    0, 2366,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2055,    0, 1336, 2423,    0,    0,    0,    0,    0,
  601,  652,  843,  847,  851,  879,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  895,    0,    0,    0,    0, 2386,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1721,    0,    0,    0,    0,
    0,    0,    0, 1729,    0,    0,    0,    0,    0,  111,
    0,    0,    0,    0,    0,    0,  271,    0,  288,    0,
    0,    0,10292,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 6643,    0, 6715,    0, 6802,    0,    0,    0,
    0, 6844,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2785,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,10139,
    0,    0,    0,    0,    0,    0,    0, 1769, 1837,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  695,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1948,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1734, 6007,    0,
    0,    0,  798,    0,    0,    0,    0,    0,    0,  121,
    0,    0,    0,    0,    0,  555,    0,    0,    0,    0,
    0,    0,    0,    0, 2788, 1515, 2667,    0, 2498,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2525,
    0,    0,    0,    0,    0,    0,    0, 1738,  908,    0,
    0,    0,    0,    0,    0,    0,    0, 2525, 2541,    0,
  370,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  422,    0, 6946,    0,    0,12589,    0,    0,
    0,    0, 6957,    0,    0,    0,    0, 7048,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   60, 2779,    0,    0,    0,    0,    0,    0,    0,
    0, 2789,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2539,    0,    0,
    0,    0,    0,    0,    0,  346,  118, 4242,    0,    0,
 2464, 1042, 1131,    0, 1279, 2811,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2502,    0,    0,  240,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  605,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 7059, 7122,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2812,    0,    0,    0,   82,    0,    0,
    0,    0,    0,    0,   86,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  151, 1572,    0,    0,    0,
    0,    0, 2814,    0,    0,    0,    0,    0,    0,    0,
 2502,  602,    0,  241,    0,    0,    0,    0,    0,    0,
    0,  697,  927,    0,    0,  195, 2788,    0,    0,    0,
    0,   97,    0,    0,    0, 2796,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   -3,    0,
    0,    0,    0,  158,    0,    0, 5697,    0,    0, 2811,
    0, 1069,    0, 1336, 1262, 1521,    0,    0, 2464, 1566,
    0,    0,  130,    0,   60,   30,    0,    0,    0,    0,
   -3, 2797,    0,    0,    0,    0,    0, 2553,    0,  186,
    0,    0, 1431,    0,    0, 2830,    0, 2502,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  957,    0,    0,
    0,   82,   61,    0,    0, 2813,    0,    0,13163,    0,
    0,    0,   -3,  132,    0,    0,    0,    0, 2811,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2464,
   97, 2833,    0,   -3,    0,    0,   -3,    0, 1124,    0,
    0,    0, 2050,    0,    0,    0,    0,   30,    0,    0,
    0,   -3,    0,    0,    0, 2834,    0,    0,    0, 5161,
   61,    0,    0,    0,    0, 2833,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2881,    0,    0, 2883,
    0,    0,    0,    0,    0,    0,    0,   10,    0, 2884,
    1,  352,  -59, -638, 1670,  369, -274, -569, 2882, 2621,
    0,  -44,    0, 2617, -102,  -40,    0, -178,-1033,   -4,
  -65,  -43, -220, 2620, 2168,    0,    0,    0,  750,    0,
    0,  -22, 1788, -542,  -67, -997, 2262, 2031, -309, 1802,
 2819, -298,    0,  -57,    0, -496,  -54,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -108,  228, 1808, -281, 1169, -888,
    0,    0,    0,    0, -326, -133, -486, 1820, -344,  -51,
    0, -533,    0,    0, 1408, -996,    0, 1403,    0,    0,
 2578, -600,  -20,    0,    0,    0,    0, 2549,    0, 2334,
 1839,    0,    0,    0, 1895,    0, 2599, 2163, 1850, 2586,
 2597,    0, 1376,    0, -447, 2152,  -32, -605, 1325, -251,
    0,    0, -917, -915, -898, -877, -872,    0, 1473, -992,
    0, 1172,-1232, 1466,    0, 2390,    0,    0,    0, 1176,
 1174, 1144,   37, -278, -838, -854,    0,    0,    0, 2174,
    0, -624,    0, 2010,    0,    0,    0,    0,    0,-1164,
 -855,-1234,    0,    0,    0,    0, -203,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1272,    0,    0,  490,    0,    0, -289,    0, -398,  377,
 1559, 1372,    0, 1201,    0,    0,    0,    0,    0,    0,
    0, 2260, -129, 1953, 1724, 1506,    0,-1114, -875, -843,
    0, -511, -460, -440,    0, -454, 1244,  962, -582, -532,
 -722, -942,-1307,    0,    0,  -48,    0,    0,    0,    0,
    0, 2631,    0,    0,    0,    0, 2640, 2415,    0,    0,
    0,    0,    0,   -9, 2657,    0,    0,    0,  -10,    0,
    0,    0, -353, 2149, -352, 1695, -473,    0,    0,    0,
    0,    0, -245,    0,    0,    0,    0, 2387,    0, 2169,
    0,    0,    0,    0,    0,    0,  194,    0, 2990,    0,
    0,    0,    0,    0, 2704,    0,    0,    0,    0, 2505,
    0, 2521,    0,    0,    0, 2269,    0,    0,    0,  -85,
  -31,    0,    0,   92,    0, 2246,    0,    0,    0,    0,
    0, -627,-1092, -596, -195,  919,    0, 2873, 1371,    0,
 2273,-1060, -745,    0, -874, 2034,    0,    0,    0,    0,
 -154,    0,    0,    0, 1313, 1424,    0, 1195,    0, 1228,
    0, -434,    0,    0,    0,    0, 2530,    0,    0,    0,
 -421, 2309, 2066,-1007,    0,    0,    0, -669, -410,   68,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2217, 1296, 1905,    0,    0,    0,    0,
 -870,    0,    0,    0, 1790,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -363, 1870,    0,    0,
};
#define YYTABLESIZE 14199
static short yytable[] = {                                     133,
  320,  199,  133,  271,   79,  155,  154,  222,  160,   38,
  290,  293,  218,  205,  291,  294,  202,  768,  621,  622,
  231,  713, 1116,  591,  259,  232, 1117,  227,  253,  262,
  239,  219,  798,  559,  699,  257, 1399,  585,  754,  774,
  214, 1491,  958,  231,  196,  198,  201,  204,  154,  209,
  258, 1271,  263,  483,  220,  198,  225,  264,  204,  557,
  154,  209,  483,  238,  133, 1195,  234,  732,  256,  941,
  711,  564,  249,  155,  154,  251,  198, 1296,  249,  225,
  201,  204,  260,  154,  209,  238,  133,  932,  204,  261,
  651,  278,  325,  940,  962,  504, 1016,  561,  635,  989,
  709,  625,  277,  152,  279,  872,  986,  752,  863, 1203,
 1223,  789,  323,  790,  527,  321, 1072,  709,  800, 1260,
  711, 1261,  702, 1231,  767,  220,  154,  220,  301,  707,
 1260,  625, 1261,  384,  850, 1106,  153,  707, 1262,  225,
 1098,  758,  577,  361,  320,  319,  204,  154,  238, 1262,
  702,   90,  160, 1268,   78, 1318,  625,  707,  709, 1263,
 1272,  711,  152,  528, 1264,  275, 1265, 1021,  990,   78,
 1263, 1280,  625, 1462,  346, 1264,  709, 1265, 1444,  711,
 1446,  288, 1770,  671, 1041,  711, 1597, 1044,  707,  763,
  625,  702, 1301,  729,  702,  153, 1051,  625,  707,  373,
  781,  577,  361,  832,  289, 1269, 1078, 1527,   78,  702,
  289, 1451,  621,  622, 1654,  625,  707,  577,  640,  847,
  739,  494,  533, 1319,  671, 1040,  711,  479,  292,  355,
  744,  635,  468, 1212,  318,  476,  479, 1006, 1531,  758,
  240,  516,  671,  880,  711, 1588, 1589,  513,  591, 1763,
 1093,  289,  771,  702, 1313,  220,  330, 1602,  220,  640,
  727,  520,  154,  377,  485,  133,  624,  481, 1763, 1482,
 1483,  154,  635,  485,  744,  635,  577,  640,  389, 1489,
  758,  240,  275,  758,  240,  526,  906,  806,  642,  744,
  635,  374,  201,  318,  906,   78,  154,  833,  758,  240,
 1572, 1218,  126, 1506,  946, 1165, 1166,  624, 1516,  558,
  252,  780,  955,  289,  330,  631,  468,  241,  219,   61,
  572,   86,  749,  574,   86,  624,  481,  375,  782,  642,
  643,  543,  220,  631,  528,  844, 1191,  389,  641,  220,
 1215,  220,  586,  220,  588,  528,  577,  642, 1260, 1260,
 1261, 1261, 1226, 1228,  242,  327,  631,  531,  639,  631,
 1463,  126, 1607,  139,  204,  209, 1079, 1262, 1262,  179,
  633, 1058,  329,  154,  631,  201,  355,   98, 1661, 1455,
  140,  749, 1260,   84, 1261,  641, 1689,  709, 1263, 1263,
  709, 1012, 1603, 1264, 1264, 1265, 1265,  518, 1608, 1733,
 1501, 1262,  387,  152,  528,  846, 1578, 1471, 1013, 1216,
  907, 1658,  702, 1246, 1484, 1121,  711, 1219,  907,   90,
  539, 1227, 1263, 1444,  702, 1700,  641, 1264,  179, 1265,
 1532, 1788,  723, 1266, 1267, 1256,  153,  740,  711,  726,
  579,  283, 1769,  731,  641,  709,  709,  737,  709,  330,
  826,  152,  479, 1279,  726,  709,  526,  252, 1520, 1183,
  711,  387,  530,  762, 1039,  530,  993,  154,  702, 1480,
  204,  485, 1118,  164,  625, 1523, 1622,  702,  154,  179,
  758,  702,  154,  707,  153,  204,  638,  154, 1841, 1105,
  160,  162,  707,  284, 1489,  320,  625,   99, 1781,  579,
  544,  671, 1035, 1637,  709, 1037, 1759,  711,  483, 1882,
  285,  671, 1579,  709,  671, 1662, 1047,  827, 1336, 1337,
  287, 1338, 1339,  671,  782,  702,  165,  638,  388,   14,
 1071,  481,  390,  802, 1805, 1663,  231,  702,  179,  561,
  649,  849,  389, 1065,  707,  638,  702,  751,  994,  635,
  711, 1821,  708,  707,  179,  795,  220, 1108,  640, 1312,
  711,  795, 1222, 1017,  828,  872, 1839,   77,  636,  707,
  671,  635,  711, 1394,  711, 1091, 1651, 1113,  577,  671,
  709,  625,   77,  707,  624, 1854,  959,  388, 1224, 1519,
 1458, 1623, 1611,  708,  649, 1624, 1625,  361, 1715,  287,
  394,  758,  702,  728,  709, 1071,  624,  219, 1868,  636,
  625,  708,  528,  179,  857,  858, 1618,  707, 1009,  287,
  152,   77,  758,  671,  672, 1048,  577,  636,  642, 1881,
  220, 1652, 1884,  631,  702,  671, 1119,  671,  709,  874,
  403,  711,  758, 1211,  318,  758,  287, 1892,  702,  873,
  126,  395,  625,  153, 1479,  631,  780, 1795,  914,  394,
  758, 1644, 1667,  709,  481,  672,  387,  854,  672,  225,
  282,  702,  640,  782,  274,  389, 1493,  625,  707, 1650,
  330,  649,  744,  672,  468,  635,  468, 1283,  967,  749,
  906,   86, 1753,  742,  948,  530,  366,  906, 1194,  403,
  726, 1883,  220, 1251,  671, 1518,  711,  220,   77,  220,
  395,  220,  528,  637,  485, 1538,  520,  179, 1015,  485,
  624,  881,  225, 1122,  906,  641,  888,  906,  485, 1726,
  530,  204, 1344, 1727,  526, 1819, 1281,  996,  204,  219,
  758,  240,  642, 1526,  123,  935,  936, 1205, 1736, 1056,
 1208,  154,  635,  154,  637,  366,  220, 1029,  468, 1712,
 1120,  708,  220,  225, 1270, 1740,  709, 1314,  231,  631,
 1030, 1260,  637, 1261,  136, 1064, 1278, 1809, 1492,  179,
  347,  220, 1088, 1089, 1090, 1057, 1059,  624, 1076, 1737,
 1262,  813,  388,  814, 1391, 1738, 1108,  673, 1300,  387,
  906,  231,  231,  231,  638,  912,  135, 1070,  220,  220,
 1092, 1263,  179,  589,  907,  330, 1264,  631, 1265, 1384,
 1103,  907, 1386,  842, 1505,  179,  638,  179, 1284,  528,
 1515,  163,  530,  136, 1461,  179,  631, 1559,  673,  641,
 1282,  673,  391, 1031,  225,  154,  390, 1123,  907, 1787,
  393,  907,  883,  170,  179,  154,  673,  348, 1794, 1371,
  179,  543,  179,   14,  394, 1186, 1187, 1115, 1615, 1134,
  708,  378,  179,  136,  179,  171, 1820,  831,  392,  758,
  708,  758,  644,  708,  605,  225,  636,  530,  758,  937,
  600,  843,  708,  883,  137, 1135,  883,  179, 1797, 1826,
 1099,  391,  179,  137,  403,  390, 1209,  138,  636,  393,
  179,  883,  170,  689,  357,  395,  906, 1250,  831,  123,
  123,  831,  179,  644,  907,  388,  367, 1705, 1206, 1210,
  960,  960,  709,  803,  171,  709,  831,  392,  820,  708,
  638,  644,  672,  605, 1009,  883,  881, 1573,  708,  600,
  881,  526,  672,  137,  689,  672,  368,  689,  485,  204,
  209,  485,  238,  133,  672, 1207,  138,  700,  379,  485,
  367,  781,  689, 1389, 1453,  821, 1706,  153,  339,  758,
 1697,  485,  485,  179,  758,  367,  757, 1707,  204, 1621,
  638,  358,  708,  476, 1324,  388,  179,  394,  906,  704,
  368,  758,  280,  393,  705, 1500,  708, 1230,  758, 1188,
  709,  672,  330, 1763,  804,  368,  220,  621,  622, 1307,
  672, 1565,  636,  295,  366, 1568,  340,  366, 1762, 1811,
  590,  637, 1054, 1443, 1653, 1445, 1522, 1327,  231,  884,
  907,  705,  758, 1331,  709,  250,  702,  528,  395, 1504,
  149,  709,  136,  637,  136,  805,  231,  758,  709, 1291,
 1688,  210,  231,  219,  672,  516,  911,  707,  240,  709,
  476,  179, 1292,  708,  960, 1291,  672, 1875,  672,  822,
  884,  976,  705,  884,  575,  705,  220,  702, 1449, 1449,
 1452, 1708,  383,  220,  958, 1716, 1721, 1285,  884, 1870,
  705, 1763,  758,  212,  213,  702,  391, 1243,  707,  240,
  390,  170,  240, 1773,  393,  673, 1838, 1476, 1777, 1478,
  434, 1722,  907,  638,  170,  673,  707,  240,  673, 1876,
  701,  156,  884,  171, 1286,  606,  366,  673,  341,  831,
  700,  616,  392,  575, 1746,  672,  171,  330,  366, 1395,
  366,  383,  136,  170,  976,  150,  342,  584, 1244,  170,
 1792,  170,  151,  157,  638,  468,  517,  637,  219,  526,
  776,  701,  375,  609,  137,  171, 1473,  689,  158, 1723,
 1521,  171,  638,  171,  673,  375, 1474,  138,  220,  701,
  343, 1400, 1441,  673,  606,  831,  226,  295,  831, 1833,
  616,  170,  485, 1686,  485,  831, 1792,  806,  831, 1855,
  505,  776,  831, 1844,  776,  468,  584,  831,  511,  485,
  831,  518,  644,  171,  609,  204,  605, 1752,  295,  776,
  246,  689,  600,  710,  689,  581,  831,  673,  519,  391,
  296,  689,  709,  390,  689, 1792, 1659,  393,  689,  673,
 1856,  673, 1143,  689,  367, 1774,  689,  367,   77,  468,
  831,  236,  883,  831,  831,  320,  528,  600, 1144,  700,
  244,  908,  689,  831,  468,  392, 1890, 1792,  706, 1273,
 1274, 1891, 1197, 1832,  368, 1793, 1789,  368,  734,  605,
  831,  265,  605, 1503,  581,  600,  689, 1857,  600,  700,
  689,  709,  236, 1702,  254,  236, 1530, 1610,  605,  689,
 1534, 1796,  831, 1145,  600,  735,  601,  831,  673,  706,
  236,  116,  704,  583,  320, 1236,  689,  705,  709,  831,
 1325,  831, 1629, 1810,  605,  234, 1828,  706,  709, 1237,
  600,  709, 1146, 1577,  708,  706,  240,  707,  240,  709,
  709,  383, 1830,  689, 1630,  240,  383,  268,  116,  705,
  570, 1631,  383, 1238,  702,  689,  367,  689,  410, 1739,
  170, 1861,  705,  689, 1860,  601,  234,  702,  367,  234,
  367,  705,  583,  272,  474,  707,  702, 1719,  526,  267,
 1302,  198,  171,  116,  234, 1632,  368,  709,  831, 1587,
 1115, 1115,  689,  689,  273,  689,  709,  707,  368, 1147,
  368, 1441,  689,  689,  689,  689,  689, 1239,  689,  570,
 1878,  689,  689,  689,  689,  689,  411,  410,  705,  689,
  689,  407,  437,  702,  689,  689,  689,  705, 1704, 1303,
  276,  295,  702,  474,  280, 1635,  240,  485,  701,  884,
  709,  295,  319, 1718,  707, 1236,  502,  204, 1897,  295,
 1898,  701,  281,  707,  709,  507,  161, 1702,  240, 1237,
  701, 1702,  282, 1803,  286,  240, 1449,  606, 1302,  505,
  154,  705, 1702,  616,  916,  411,  702,  101,  776,  383,
  407,  776,   44, 1238,  918,  705,  536,  674,  776,  540,
  702,  776,  920,  998,   52,  776,  145,  707, 1702,  240,
  776,  674,  606,  776,  200,  162,   42,  701,  616,  674,
  237,  707,  302,  755,  103,  161,  701, 1306,  505,  776,
  318,  709, 1734,  708,  706,  674,  707, 1695,  709,  236,
  606,  236,  307,  606,  328, 1702,  616, 1239,  236,  616,
 1702,   44,  331,  776,   25,  200,  776,  776,  200,  606,
  360,  237,  705,   52,  237,  616,  776,  702,  204,  240,
  701,  681,  349,  200,  162,   42,  754,  362,  100,  237,
 1115, 1735,  704,  776,  701,  606, 1732,  705,  707,  674,
   48,  616,  371,  104,  689,   50,  706,  363,  319,  695,
  696,  698, 1704,  105,  483,  776, 1704,  555, 1704,  381,
  776, 1713,  681,   25,  528,  681,  948, 1704,  706,  101,
  556,  154,  776,  287,  776,  383,  201,  333,  106, 1782,
  681,  102,  386,  763,  364,  754,  365, 1748,  385,  236,
  334, 1749,  474, 1704,  155,  154,  528,  268, 1765,   48,
 1008,  701,  128,  390,   50,  474,  103,  378,  601,  366,
  763,  236,  389,  483,   26,  706,  473,   49,  236,  743,
   51,  480, 1236,  745,  706,  335,   53,  336,  708,  706,
 1704,  707, 1835,  709,  367, 1704, 1237,   54,  528,  488,
  474,  776,  474,  601,   55, 1758,  198, 1767,  708,  706,
  434,  707,  236,  709,   46,  489,  578,  492, 1768,  528,
 1238,  128,  528,   81,   82, 1755,  378,  579,  706,   56,
  129,  601,  493,   26,  601,  104,   49,  528,  117,   51,
  155,  154,  706,   47, 1877,  105,  526,  200,  776,  718,
  601,  351,  717,  303,  352,  708,  706, 1862,  707,  507,
  709, 1850,  708,  706,  220,  707,  495,  709, 1869, 1441,
  106,  497,  236,   46, 1239,  996,  601,  722,  526, 1813,
  154,  704,  708,  706,  499,  707,  705,  709,  304,  129,
  155,  154,  722,  383,   57,  721,  498,  117, 1851,  353,
  507, 1441,   47,  507,  383,  512,  200, 1873,  237,  706,
  237, 1852, 1813,  154,  722, 1896,  305,  237,  507,  306,
  526, 1182,  792,  514,  998,  791,   58,  722,  155,  154,
   59,  943,  144,  145,  710,  307,  998,  887,  524,  998,
 1879,  526,  534, 1441,  526,  996,  515, 1885, 1813,  154,
  708,  706,  201,  707,  910,  709,   14,  985,  915,  526,
  984,  308,  212,  213, 1441,  931, 1115, 1441, 1066, 1067,
  938,  939,  201,  541, 1899,  549,  800,  409,  410,  411,
  552,  437, 1441,  949,  950,  951,  952,  953,  954,  437,
   14, 1097, 1003,  201, 1096,  201,  369, 1104,  548,  681,
  868,  796,  681,  550, 1003,  419,  420, 1003,  237,  681,
  369, 1196,  681,  573,  710,  570,  681,  800,  369, 1199,
  800,  681, 1198,  583,  681,  587,  704,  636,  507, 1247,
  237,  705, 1096,  107,  369,  800,  642,  237,  708,  706,
  681,  707,  678,  709,  708,  706,  704,  707, 1018,  709,
  640,  705, 1294,  536, 1025, 1293, 1299, 1456,  530, 1298,
  710,  644,  108,  109,  681,  110, 1539,  996,  681,  710,
  675,  237,  111,  112,  113,  114,  115,  681,  116, 1703,
  676,  117,  118,  119,  120,  121, 1757,  677,  369,  122,
  123,  679,  680,  704,  681,  124,  125,  685,  705,  530,
  704, 1570,  530,  989,  710,  705,  497,  972,  996,  996,
  996,  996,  996,  996,  996,  986, 1609,  530,  976,  710,
  704,  681,  124,  124,  681,  705,  996,  996,  996,  996,
  682,  237,  683,  681,  684,  681,  687,  507, 1846,  507,
  748, 1845, 1880,  507,  686, 1763,  507,  497,  710,  373,
  497, 1349,  373, 1840,  692,  909,  693,  913,  996,  507,
  996,  917,  919,  921,  213,  497,  694,  703,  711, 1124,
 1125, 1129,  716,  241, 1137, 1139, 1141,  107, 1149, 1152,
 1155, 1157, 1160, 1163,  712,  719, 1167, 1168,  704, 1170,
  720, 1172,  724,  705, 1175, 1176, 1177, 1178, 1179, 1180,
  730,  729,  681,  730,  738,  213,  108,  109,  213,  110,
  242, 1184,  107, 1189,  741,  748,  111,  112,  113,  114,
  115,  750,  116,  213,  761,  117,  118,  119,  120,  121,
  765,  766,  770,  122,  123,  772,  773,  507,  800,  124,
  125,  108,  109, 1703,  110,  998,  775, 1703,  777, 1804,
  778,  111,  112,  113,  114,  115,  793,  116, 1703,  507,
  117,  118,  119,  120,  121,  437,  507,  780,  122,  123,
  543,  783,  784,  786,  124,  125,  704,  787,  818,  817,
  819,  705,  704,  824, 1703,  287,  829,  705,  830,  687,
  836,  834,  835,  837,  800,  840,  839,  800,  845,  851,
  507, 1138,  852, 1142,  800, 1150, 1153,  800, 1158, 1161,
 1164,  800,  856, 1547, 1563,  855,  800,  865,  867,  800,
  868, 1703,  708,  706,  876,  707, 1703,  709,  878,  996,
  687,  996,  996,  687,  877,  800,  530,  882,  530,  996,
  933,  944,  530,  846,  289,  530,  979,  996,  687,  963,
  982,  980,  996,  987,  991,  996, 1800,  992,  530,  800,
 1802, 1005,  800,  800,  744,  708,  706, 1019,  707,  996,
  709, 1808,  800, 1010,  996,  996,  708,  706,  996,  707,
 1026,  709, 1022, 1036,  497,  996,  497,  996,  996,  800,
 1028,  996, 1045,  497, 1046,  996,  796, 1836,  996, 1048,
  996, 1055,  996, 1052, 1061, 1053,  497,  996,  996,  437,
  996,  800, 1062, 1063, 1087, 1080,  800, 1069, 1085, 1074,
 1094, 1352, 1353, 1099, 1355, 1357,  996,  996,  800,  996,
  800, 1111, 1362, 1131, 1863, 1112,  530,  734, 1171, 1865,
 1173, 1181,  225,  996,  213,  996,  996,  996,  704, 1202,
  996, 1375, 1214,  996,  996, 1217, 1220, 1212,  530, 1382,
 1383, 1189, 1229,  996, 1189,  530, 1184, 1233, 1200, 1235,
 1390,  437, 1392, 1393, 1241, 1252, 1255,  996, 1277, 1287,
  996,  206, 1275, 1297,  497, 1295,  942,  708,  706, 1304,
  707, 1308,  709, 1309, 1310,  530,  996,  800, 1311,  530,
 1320,  507,  996,  996, 1105, 1322,  497,  996,  995,  708,
  706,  437,  707,  497,  709, 1329, 1332, 1333,  996,  996,
 1334,  996,  206, 1335, 1341,  206, 1342,  996, 1174,  708,
  706, 1346,  707, 1347,  709,  906,  530, 1348, 1350,  530,
  206, 1351,  225, 1358,  746, 1359, 1360,  497, 1485, 1361,
 1363, 1364, 1365, 1366,  530, 1367,  996,  996, 1368,  996,
  704, 1457, 1388, 1385,  225,  705,  996,  996,  996,  996,
  996,  225,  996, 1442, 1459,  996,  996,  996,  996,  996,
 1460,  746, 1464,  996,  996, 1465, 1466, 1467,  996,  996,
  996, 1469, 1369,  708,  706, 1468,  707,  507,  709,  976,
 1370,  708,  706,  704,  707,  225,  709,  687,  705,  721,
  687,  234,  530, 1477,  704,  565, 1494,  687, 1535,  705,
  687, 1490, 1495, 1496,  687, 1497, 1498, 1543, 1499,  687,
 1502, 1071,  687, 1517, 1548, 1507, 1524, 1528, 1343, 1529,
  976,  976,  976,  976,  976,  989,  976, 1533,  687, 1540,
 1560, 1567,  234,  530, 1566,  234,  530, 1575,  976,  976,
  976,  976, 1574, 1189, 1580,  107, 1581, 1189, 1595,  136,
  234,  530,  687,  530, 1583, 1741,  687, 1584, 1591,  801,
 1592, 1593, 1594, 1600, 1582,  687, 1601, 1373,  708,  706,
  976,  707,  976,  709,  108,  109, 1604,  110, 1616, 1605,
 1606, 1626,  687, 1636,  111,  112,  113,  114,  115, 1638,
  116, 1639, 1640,  117,  118,  119,  120,  121, 1641, 1642,
  801,  122,  123,  801, 1643,  704, 1645,  124,  125,  687,
  705, 1646, 1647, 1657, 1655, 1665, 1660, 1666,  801, 1664,
 1668,  687, 1669,  687, 1670, 1671, 1612,  704, 1674,  670,
 1675, 1676,  705,  708,  706, 1192,  707, 1677,  709,  206,
 1678,  206, 1679, 1680, 1681, 1682, 1683,  704,  206, 1685,
 1596, 1699,  705,  530, 1692,  530,  214, 1714,  670,  670,
 1720,  670,  530, 1728, 1730, 1731, 1754, 1761,  670,  670,
  670,  670,  670, 1760,  670,  530, 1764,  670,  670,  670,
  670,  670, 1771, 1772, 1775,  670,  670, 1779, 1780, 1783,
  687,  670,  670, 1791, 1798, 1827, 1822,  214, 1801, 1829,
  214, 1831, 1834, 1824, 1672, 1673, 1837, 1763, 1374,  708,
  706,  704,  707, 1843,  709,  214,  705, 1848, 1849,  704,
 1820, 1684, 1819, 1864,  705, 1189, 1376,  708,  706, 1687,
  707,  164,  709, 1866, 1867, 1694, 1872, 1696, 1874,  206,
 1888,  976, 1893,  976,  976, 1894, 1895,    1, 1900,  631,
  530,  976,   30,  530, 1377,  708,  706,   32,  707,  976,
  709,  206,  186,  663,  976,  375,  530,  976,  206,  234,
  465,  234,  758,  254,   82,  530,  528,  883,  234,  959,
  196,  976,  530,  976,  149,  265,  976,  976, 1000,  528,
  976,  264,  530,  176,  403,  958,  700,  976,  530,  976,
  976,  530,  206,  976,  528,  698,  704,  976,  694,  696,
  976,  705,  976,  177,  976,  939,  530, 1747,  376,  976,
  976,  801,  976,  708,  706, 1750,  707,  206,  709, 1000,
 1000, 1000, 1000, 1000, 1000, 1000,  281,  315,  976,  976,
  717,  976,  938,  716,  323,  350,  335, 1000, 1000, 1000,
 1000,  708,  706,  437,  707,  976,  709,  976,  976,  976,
  718,  321,  976,  638,  727,  976,  976,  957,  825,  234,
   27,  704,   30,   40, 1447,  976,  705,  801, 1612, 1000,
  801, 1000,  487,  141,  486, 1330,  981,  801, 1110,  976,
  801,  234,  976,  496,  801,  866,  255, 1328,  234,  801,
 1326, 1649,  801,  530, 1316, 1648,  554,  595,  976,  825,
  530,  532,  825, 1305,  976,  976,  831, 1254,  801,  976,
  708,  706, 1193,  707, 1020,  709,  567,  825,  530, 1807,
  976,  976,  234,  976,  225, 1317,  214,  568, 1690,  976,
 1729, 1050,  801, 1818, 1634,  801,  801,  704, 1823, 1628,
  812, 1825,  705, 1859, 1043,  801, 1756, 1586,  530, 1711,
 1806, 1472,  530, 1248, 1204,  704,  960, 1619,  976,  976,
  705,  976,  801,  560, 1790,  107,  575,  816,  976,  976,
  976,  976,  976,  551,  976, 1086, 1514,  976,  976,  976,
  976,  976,  143,  704,  801,  976,  976, 1077,  705,  801,
  976,  976,  976,  525,  108,  109,  853,  110,  756,  300,
 1024,  801, 1007,  801,  111,  112,  113,  114,  115, 1011,
  116, 1232, 1778,  117,  118,  119,  120,  121,  760, 1724,
 1847,  122,  123, 1871,  225,  988, 1481,  124,  125, 1354,
 1378,  708,  706, 1225,  707, 1169,  709, 1387,  769,    0,
 1000,    0, 1000, 1000,    0,    0,  225,    0,    0,    0,
 1000,  704,    0,  225,    0,    0,  705,    0, 1000,    0,
    0, 1340,    0, 1000,    0,    0, 1000, 1379,  708,  706,
  801,  707,    0,  709,    0,    0,    0,    0,    0,  704,
 1000,    0, 1005,    0,  705, 1000, 1000,  225,    0, 1000,
    0, 1380,  708,  706,    0,  707, 1000,  709, 1000, 1000,
    0,    0, 1000,    0,    0,    0, 1000,    0,    0, 1000,
    0, 1000,    0, 1000,    0,    0,    0,    0, 1000, 1000,
  825, 1000,    0, 1005, 1005, 1005, 1005, 1005, 1005, 1005,
 1381,  708,  706,    0,  707,    0,  709, 1000, 1000,    0,
 1000, 1005, 1005, 1005, 1005,    0,    0,    0,  704,    0,
    0,    0,    0,  705, 1000,    0, 1000, 1000, 1000,    0,
  957, 1000,  804,    0, 1000, 1000,    0,    0,    0,    0,
    0,    0,    0, 1005, 1000, 1005,  825,    0,    0,  825,
    0,    0,    0,    0,    0,    0,  825,    0, 1000,  825,
    0, 1000,    0,  825,    0,    0,    0,    0,  825,    0,
    0,  825,    0,  804,    0,    0,  804, 1000,    0,    0,
    0,    0,    0, 1000, 1000,    0,    0,  825, 1000,    0,
    0,  804,    0,    0,    0,    0,    0,    0,    0, 1000,
 1000,    0, 1000,    0,    0,    0,    0,    0, 1000,    0,
    0,  825,    0,    0,  825,  825,    0, 1561,  708,  706,
    0,  707,    0,  709,  825, 1562,  708,  706,    0,  707,
    0,  709,    0,    0,    0,    0,    0, 1000, 1000,  704,
 1000,  825,    0,    0,  705,  107,    0, 1000, 1000, 1000,
 1000, 1000,    0, 1000,    0,    0, 1000, 1000, 1000, 1000,
 1000,    0,    0,  825, 1000, 1000,    0,    0,  825, 1000,
 1000, 1000,    0,    0,  108,  109,  704,  110,    0,    0,
  825,  705,  825,    0,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117,  118,  119,  120,  121,    0,    0,
  704,  122,  123,    0,    0,  705,    0,  124,  125,    0,
    0,    0,    0,    0, 1005,    0, 1005, 1005,    0,    0,
    0,    0,    0,    0, 1005,    0,    0, 1569,  708,  706,
    0,  707, 1005,  709,    0,    0,    0, 1005,    0,  704,
 1005,    0,    0,    0,  705,    0,    0,    0,    0,  825,
    0,    0,    0,    0, 1005,    0, 1001,    0,    0, 1005,
 1005,    0,    0, 1005,  708,  706, 1571,  707,    0,  709,
 1005,    0, 1005, 1005,  708,  706, 1005,  707,    0,  709,
 1005,    0,    0, 1005,  956, 1005,    0, 1005,    0,    0,
    0,    0, 1005, 1005,  804, 1005,    0, 1001, 1001, 1001,
 1001, 1001, 1001, 1001, 1751,  708,  706,    0,  707,    0,
  709, 1005, 1005,    0, 1005, 1001, 1001, 1001, 1001,  922,
  923,  924,  925,  926,  927,  928,  929,    0, 1005,    0,
 1005, 1005, 1005,    0,    0, 1005,  802,  667, 1005, 1005,
    0,    0,    0,    0,    0,    0,    0, 1001, 1005, 1001,
  804,    0,    0,  804,    0,    0,  704,    0,    0,    0,
  804,  705, 1005,  804,  704, 1005,    0,  804,    0,  705,
    0,    0,  804,    0,    0,  804,    0,  802,    0,    0,
  802, 1005,    0,    0,    0,    0,    0, 1005, 1005,    0,
    0,  804, 1005,    0,    0,  802,    0,    0,    0,    0,
    0,    0,    0, 1005, 1005,    0, 1005, 1536, 1537,    0,
    0,    0, 1005, 1541, 1542,  804, 1544, 1545,  804,  804,
    0,    0,    0,    0, 1549, 1550, 1551, 1552,  804, 1553,
 1554, 1555, 1556, 1557, 1558,    0,    0,    0,    0,    0,
    0, 1005, 1005,    0, 1005,  804,    0,    0,  107,    0,
    0, 1005, 1005, 1005, 1005, 1005,    0, 1005,    0,    0,
 1005, 1005, 1005, 1005, 1005,    0,  704,  804, 1005, 1005,
    0,  705,  804, 1005, 1005, 1005,    0,  108,  109,    0,
  110,    0,    0,    0,  804,    0,  804,  111,  112,  113,
  114,  115,    0,  116, 1546,    0,  117,  118,  119,  120,
  121,    0,  704,    0,  122,  123,    0,  705,    0,    0,
  124,  125,  704,    0,    0,   85,    0,  705, 1001,    0,
 1001, 1001,   86,    0,    0,    0,   87,   88, 1001,    0,
    0,    0,    0,    0,    0,   89, 1001,    0,    0,    0,
    0, 1001,    0,  704, 1001,    0,    0,   90,  705,    0,
    0,    0,    0,  804,    0,    0,    0,    0, 1001,    0,
 1015,    0,    0, 1001, 1001,    0,    0, 1001,    0,    0,
    0,    0,    0,    0, 1001,    0, 1001, 1001,    0,    0,
 1001,    0,    0,    0, 1001,    0,    0, 1001,    0, 1001,
    0, 1001,    0,    0,    0,    0, 1001, 1001,  802, 1001,
    0, 1015, 1015, 1015, 1015, 1015, 1015, 1015,    0,    0,
    0,    0,   91,  667,    0, 1001, 1001,    0, 1001, 1015,
 1015, 1015, 1015,    0,    0,    0,    0,    0,    0,   92,
    0,    0, 1001,    0, 1001, 1001, 1001,    0,    0, 1001,
  803,   78, 1001, 1001,   93,    0,    0,    0,   94,    0,
    0, 1015, 1001, 1015,  802,    0,    0,  802,    0,   95,
    0,    0,    0,    0,  802,    0, 1001,  802,    0, 1001,
    0,  802,    0,    0,    0,    0,  802,    0,   96,  802,
    0,  803,    0,    0,  803, 1001,    0,    0,    0,    0,
    0, 1001, 1001,    0,    0,  802, 1001,    0,    0,  803,
    0,    0,    0,    0,    0,    0,    0, 1001, 1001,    0,
 1001,    0,    0,    0,    0,    0, 1001,    0,    0,  802,
    0,    0,  802,  802,    0,    0,    0,    0,    0,    0,
    0,    0,  802,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1001, 1001,    0, 1001,  802,
    0,    0,    0,  670,    0, 1001, 1001, 1001, 1001, 1001,
    0, 1001,    0,    0, 1001, 1001, 1001, 1001, 1001,    0,
    0,  802, 1001, 1001,    0,    0,  802, 1001, 1001, 1001,
    0,    0,  670,  670,    0,  670,    0,    0,  802,    0,
  802,    0,  670,  670,  670,  670,  670,    0,  670,    0,
    0,  670,  670,  670,  670,  670,    0,    0,    0,  670,
  670,    0,    0,    0,    0,  670,  670,    0,    0,    0,
    0,    0, 1015,    0, 1015, 1015,    0,    0,    0,    0,
    0,    0, 1015,    0,    0,    0,    0,    0,    0,    0,
 1015,    0,    0,    0,    0, 1015,    0,    0, 1015,    0,
    0,    0,    0,    0,    0,    0,    0,  802,    0,    0,
    0,   63, 1015,    0,  852,    0,    0, 1015, 1015,    0,
    0, 1015,   64,    0,    0,    0,    0,    0, 1015,   65,
 1015, 1015,    0,    0, 1015,    0,    0,    0, 1015,    0,
    0, 1015,   66, 1015,    0, 1015,    0,    0,    0,    0,
 1015, 1015,  803, 1015,    0,  852,  852,  852,  852,  852,
    0,  852,    0,    0,    0,    0,    0,    0,    0, 1015,
 1015,    0, 1015,  852,  852,  852,  852,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1015,    0, 1015, 1015,
 1015,    0,    0, 1015,  833,    0, 1015, 1015,    0,    0,
    0,    0,    0,   67,    0,    0, 1015,  852,  803,    0,
    0,  803,    0,    0,    0,    0,    0,    0,  803,   68,
 1015,  803,    0, 1015,    0,  803,    0,    0,    0,    0,
  803,    0,    0,  803,    0,  833,   69,    0,  833, 1015,
    0,    0,    0,    0,    0, 1015, 1015,    0,    0,  803,
 1015,   70,    0,  833,    0,   71,    0,    0,   72,    0,
    0, 1015, 1015,    0, 1015,    0,   73,    0,    0,    0,
 1015,    0,    0,  803,    0,    0,  803,  803,    0,    0,
    0,    0,    0,    0,    0,   74,  803,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1015,
 1015,    0, 1015,  803,   77,    0,  107,    0,    0, 1015,
 1015, 1015, 1015, 1015,    0, 1015,    0,    0, 1015, 1015,
 1015, 1015, 1015,    0,    0,  803, 1015, 1015,    0,    0,
  803, 1015, 1015, 1015,    0,  108,  109,    0,  110,    0,
    0,  679,  803,    0,  803,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,  125,
    0,    0,    0,    0,    0,    0,  852,    0,  852,  852,
    0,  679,  679,    0,    0,  679,  852,    0,    0,    0,
    0,    0,    0,    0,  852,    0,    0,    0,    0,  852,
  679,    0,  852,    0,    0,    0,    0,    0,    0,    0,
    0,  803,    0,    0,    0,    0,  852,    0,  864,    0,
    0,  852,  852,    0,    0,  852,    0,    0,    0,    0,
    0,    0,  852, 1576,  852,  852,    0,    0,  852,    0,
    0,    0,  852,    0,    0,  852,    0,  852,    0,  852,
    0,    0,    0,    0,  852,  852,  833,  852,    0,  864,
  864,  864,  864,  864,    0,  864,    0,    0,    0,    0,
    0,    0,    0,  852,  852,    0,  852,  864,  864,  864,
  864,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  852,    0,  852,  852,  852,    0,    0,  852,    0,    0,
  852,  852,    0,    0,    0,    0,    0,    0,    0,    0,
  852,  864,  833,    0,    0,  833,    0,    0,    0,    0,
    0,    0,  833,    0,  852,  833,    0,  852,    0,  833,
    0,    0,    0,    0,  833,    0,    0,  833,    0,    0,
    0,    0,    0,  852,    0,    0,    0,    0,    0,  852,
  852,    0,    0,  833,  852,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  852,  852,    0,  852,    0,
    0,    0,    0,    0,  852,    0,    0,  833,    0,    0,
  833,  833,    0,    0,    0,    0,    0,    0,    0,    0,
  833,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  852,  852,    0,  852,  833,    0,    0,
    0,    1,    0,  852,  852,  852,  852,  852,    0,  852,
    0,    0,  852,  852,  852,  852,  852,    0,    0,  833,
  852,  852,    2,    0,  833,  852,  852,  852,    0,    0,
    3,    0,    0,    0,    0,    0,  833,    4,  833,    5,
  867,    6,    0,    0,    0,    0,    7,    0,    0,  679,
    0,    0,  679,    8,    0,    0,    0,    0,    0,  679,
    0,    0,  679,    0,    0,    0,  679,    9,    0,    0,
  864,  679,  864,  864,  679,    0,    0,    0,   10,    0,
  864,  867,  867,  867,  867,  867,    0,  867,    0,    0,
  679,    0,    0,  864,    0,    0,  864,    0,    0,  867,
  867,  867,  867,    0,    0,  833,    0,    0,    0,    0,
  864,    0,    0,    0,  679,  864,  864,    0,  679,  864,
  797,    0,    0,    0,    0,    0,  864,  679,  864,  864,
    0,    0,  864,  867,    0,    0,  864, 1585,    0,  864,
    0,  864,    0,  864,  679,   11,    0,    0,  864,  864,
    0,  864,   12,  692,    0,   13,    0,   14,   15,    0,
    0,  797,    0,    0,  797,    0,    0,  864,  864,    0,
  864,  679,    0,    0,    0,    0,    0,    0,    0,  797,
    0,    0,   16,  679,  864,  679,  864,  864,  864,    0,
    0,  864,    0,    0,  864,  864,    0,  692,    0,    0,
    0,    0,    0,    0,  864,    0,    0,    0,    0,    0,
    0,    0,  692,    0,    0,    0,    0,    0,  864,    0,
    0,  864,    0,    0,  107,    0,    0,    0,    0,    0,
    0,   17,    0,    0,    0,    0,    0,  864,    0,    0,
    0,    0,    0,  864,  864,    0,   18,   19,  864,    0,
    0,    0,  679,  108,  109,    0,  110,    0,    0,  864,
  864,    0,  864,  111,  112,  113,  114,  115,  864,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,    0,
  122,  123,    0,    0,    0,    0,  124,  125,    0,    0,
    0,    0,    0,    0,  434,    0,    0,  864,  864,    0,
  864,    0,  867,    0,  867,  867,    0,  864,  864,  864,
  864,  864,  867,  864,    0,    0,  864,  864,  864,  864,
  864,    0,    0,    0,  864,  864,    0,    0,  867,  864,
  864,  864,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  867,    0,  868,    0,    0,  867,  867,    0,
    0,  867,    0,    0,    0,    0,    0,    0,  867,    0,
  867,  867,    0,    0,  867,    0,    0,    0,  867,    0,
    0,  867,    0,  867,    0,  867,    0,    0,    0,    0,
  867,  867,  797,  867,    0,  868,  868,  868,  868,  868,
    0,  868,    0,    0,    0,    0,    0,    0,    0,  867,
  867,    0,  867,  868,  868,  868,  868,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  867,    0,  867,  867,
  867,    0,    0,  867,    0,    0,  867,  867,    0,    0,
    0,    0,    0,    0,    0,    0,  867,  868,  797,    0,
    0,  797,    0,    0,    0,    0,    0,    0,  797,    0,
  867,  797,    0,  867,    0,  797,    0,    0,    0,    0,
  797,    0,    0,  797,    0,    0,    0,    0,    0,  867,
    0,    0,    0,    0,  692,  867,  867,    0,    0,  797,
  867,  692,    0,    0,  692,    0,    0,    0,  692,    0,
    0,  867,  867,    0,  867,    0,  692,    0,    0,    0,
  867,    0,    0,  797,    0,    0,  797,  797,    0,    0,
    0,    0,  692,    0,    0,    0,  797,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  867,
  867,    0,  867,  797,    0,    0,  692,    0,  107,  867,
  867,  867,  867,  867,    0,  867,    0,    0,  867,  867,
  867,  867,  867,    0,    0,  797,  867,  867,    0,    0,
  797,  867,  867,  867,    0,    0,  692,  108,  109,    0,
  110,  692,  797,    0,  797,    0,  862,  111,  112,  113,
  114,  115,    0,  116,    0,    0,  117,  118,  119,  120,
  121,    0,    0,  692,  122,  123,    0,    0,    0,    0,
  124,  125,    0,    0,    0,  692,  868,  692,  868,  868,
    0,    0,    0,    0,    0,    0,  868,  862,    0,  862,
  862,  862,    0,    0,    0,    0,    0,    0,  968,    0,
    0,    0,  868,    0,    0,  862,  862,  862,  862,    0,
    0,  797,    0,    0,    0,    0,  868,    0,    0,    0,
  371,  868,  868,    0,    0,  868,  798,    0,    0,    0,
    0,    0,  868,    0,  868,  868,    0,    0,  868,  862,
    0,    0,  868,    0,    0,  868,    0,  868,    0,  868,
    0,    0,    0,    0,  868,  868,    0,  868,    0,    0,
    0,  371,    0,    0,  371,    0,  107,  798,    0,    0,
  798,    0,    0,  868,  868,    0,  868,    0,    0,  371,
    0,    0,    0,    0,    0,  798,    0,    0,    0,    0,
  868,    0,  868,  868,  868,  108,  109,  868,  110,    0,
  868,  868,    0,    0,    0,  111,  112,  113,  114,  115,
  868,  116,    0,  971,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,  868,    0,    0,  868,  124,  125,
  107,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  868,    0,    0,    0,    0,    0,  868,
  868,    0,    0,    0,  868,    0,    0,    0,    0,  108,
  109,    0,  110,    0,    0,  868,  868,    0,  868,  111,
  112,  113,  114,  115,  868,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,  125,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  868,  868,    0,  868,    0,  862,    0,
  862,  862,    0,  868,  868,  868,  868,  868,  862,  868,
    0,    0,  868,  868,  868,  868,  868,    0,    0,    0,
  868,  868,    0,    0,  862,  868,  868,  868,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  862,    0,
  861,    0,    0,  862,  862,    0, 1257,  862,    0,    0,
    0,    0,    0,    0,  862,    0,  862,  862,    0,    0,
  862,    0,    0,    0,  862,    0,    0,  862,    0,  862,
    0,  862,    0,    0,    0,    0,  862,  862,  798,  862,
    0,  861,    0,  861,  861,  861,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  862,  862,    0,  862,  861,
  861,  861,  861,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  862,    0,  862,  862,  862,    0,    0,  862,
    0,    0,  862,  862,    0,    0,    0,    0,    0,    0,
    0,  371,  862,  861,  798,    0,    0,  798,  371,    0,
    0,  371,    0,    0,  798,  371,  862,  798,    0,  862,
    0,  798,    0,  371,    0,    0,  798,    0,    0,  798,
    0,    0,    0,    0,    0,  862,    0,    0,    0,  371,
    0,  862,  862,    0,    0,  798,  862,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  862,  862,    0,
  862,    0,    0,  371,    0,    0,  862,    0,    0,  798,
    0,    0,  798,  798,    0,    0,    0,    0,    0,    0,
    0,    0,  798,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  371,    0,  862,  862,    0,  862,  798,
    0,    0,    0,    0,    0,  862,  862,  862,  862,  862,
    0,  862,    0,    0,  862,  862,  862,  862,  862,    0,
  371,  798,  862,  862,    0,    0,  798,  862,  862,  862,
    0,    0,  371,    0,  371,    0,    0,    0,  798,    0,
  798,    0,  863,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  397,    0,    0,    0,
    0,    0,  861,    0,  861,  861,    0,    0,    0,    0,
    0,    0,  861,  863,    0,  863,  863,  863,    0,    0,
    0,    0,    0,    0,  399,    0,    0,    0,  861,    0,
    0,  863,  863,  863,  863,    0,    0,  798,    0,    0,
    0,    0,  861,    0,    0,    0,  684,  861,  861,    0,
    0,  861,  799,  401,    0,    0,    0,    0,  861,    0,
  861,  861,    0,    0,  861,  863,    0,    0,  861,    0,
    0,  861,    0,  861,    0,  861,    0,    0,    0,    0,
  861,  861,    0,  861,    0,    0,    0,  684,    0,    0,
  684,    0,    0,  799,    0,    0,  799,    0,    0,  861,
  861,    0,  861,    0,    0,  684,    0,    0,    0,    0,
    0,  799,    0,    0,    0,    0,  861,    0,  861,  861,
  861,    0,    0,  861,    0,    0,  861,  861,  406,    0,
    0,    0,    0,    0,    0,    0,  861,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,    0,    0,    0,
  861,    0,    0,  861,    0,    0,    0,    0,  409,  410,
  411,    0,  212,  213,    0,    0,    0,    0,    0,  861,
    0,    0,    0,    0,    0,  861,  861,    0,    0,    0,
  861,  414,  415,  416,  417,  418,  419,  420,  421,  422,
    0,  861,  861,    0,  861,    0,  424,  425,    0,    0,
  861,    0,    0,    0,    0,  429,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  861,
  861,    0,  861,    0,  863,    0,  863,  863,    0,  861,
  861,  861,  861,  861,  863,  861,    0,    0,  861,  861,
  861,  861,  861,    0,    0,    0,  861,  861,    0,    0,
  863,  861,  861,  861,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  863,    0,  866,    0,    0,  863,
  863,    0,    0,  863,    0,    0,    0,    0,    0,    0,
  863,    0,  863,  863,    0,    0,  863,    0,    0,    0,
  863,    0,    0,  863,    0,  863,    0,  863,    0,    0,
    0,    0,  863,  863,  799,  863,    0,  866,    0,  866,
  866,  866,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  863,  863,    0,  863,  866,  866,  866,  866,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  863,    0,
  863,  863,  863,    0,    0,  863,  670,    0,  863,  863,
    0,    0,    0,    0,  684,    0,    0,  684,  863,  866,
  799,    0,    0,  799,  684,    0,    0,  684,    0,    0,
  799,  684,  863,  799,    0,  863,  684,  799,    0,  684,
    0,    0,  799,    0,    0,  799,  670,  670,    0,    0,
  670,  863,    0,    0,    0,  684,    0,  863,  863,    0,
    0,  799,  863,    0,    0,  670,    0,    0,    0,    0,
    0,    0,    0,  863,  863,    0,  863,    0,    0,  684,
    0,    0,  863,  684,    0,  799,    0,    0,  799,  799,
    0,    0,  684,    0,    0,    0,    0,    0,  799,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  684,
    0,  863,  863,    0,  863,  799,    0,    0,    0,    0,
    0,  863,  863,  863,  863,  863,    0,  863,    0,    0,
  863,  863,  863,  863,  863,    0,  684,  799,  863,  863,
    0,    0,  799,  863,  863,  863,    0,    0,  684,    0,
  684,    0,    0,    0,  799,    0,  799,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  866,    0,
  866,  866,    0,    0,    0,    0,    0,    0,  866,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  866,    0,    0,  684,    0,    0,
    0,    0,    0,  799,    0,    0,    0,    0,  866,    0,
    0,    0,    0,  866,  866,    0,    0,  866,    0,    0,
    0,    0,    0,    0,  866,    0,  866,  866,    0,    0,
  866,    0,    0,    0,  866,    0,    0,  866,    0,  866,
    0,  866,    0,    0,    0,    0,  866,  866,    0,  866,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  866,  866,    0,  866,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  866,    0,  866,  866,  866,    0,    0,  866,
    0,    0,  866,  866,    0,    0,    0,    0,    0,    0,
    0,    0,  866,    0,  670,    0,    0,  670,    0,  528,
    0,    0,    0,    0,  670,    0,  866,  670,    0,  866,
    0,  670,    0,    0,    0,    0,  670,    0,    0,  670,
    0,    0,    0,    0,    0,  866,    0,    0,    0,    0,
    0,  866,  866,    0,    0,  670,  866,    0,    0,    0,
  528,    0,    0,  528,    0,    0,    0,  866,  866,    0,
  866,    0,    0,    0,    0,    0,  866,    0,  528,  670,
  528,    0,    0,  670,    0,    0,    0,    0,    0,    0,
    0,    0,  670,    0,    0,    0,    0,    0,    0,  841,
    0,    0,    0,    0,    0,  866,  866,    0,  866,  670,
    0,    0,    0,    0,    0,  866,  866,  866,  866,  866,
    0,  866,    0,    0,  866,  866,  866,  866,  866,    0,
    0,    0,  866,  866,    0,    0,  670,  866,  866,  866,
  841, 1009, 1009,  841, 1009, 1009, 1009,    0,  670,    0,
  670,    0,    0,    0,    0,    0,  670,    0,  841, 1009,
 1009, 1009,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  842,    0,    0,  670,  670,    0,  670,    0,
 1009,    0,    0,    0,    0,  670,  670,  670,  670,  670,
    0,  670,    0,    0,  670,  670,  670,  670,  670,    0,
    0,    0,  670,  670,    0,    0,    0,  670,  670,  670,
    0,    0,    0,  842, 1010, 1010,  842, 1010, 1010, 1010,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  842, 1010, 1010, 1010,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  204,    0,    0,
    0,  204,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1010,    0,  204,    0,  528,    0,  204,
    0,    0,  204,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  528,  528,    0,  528,
    0,    0,    0,  528,    0,    0,  528,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  528,
    0,  843,    0,  204,    0,  528,    0,    0,    0,  204,
  204,  204,  832,  204,  204,  204,    0,  204,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  204,    0,  204,
    0,  841,  843, 1011, 1011,  843, 1011, 1011, 1011,    0,
    0, 1009,    0,  832,    0,    0,  832,    0,    0, 1009,
  843, 1011, 1011, 1011, 1009,    0,    0, 1009,    0,    0,
  204,  832,    0,    0,    0,   81,    0,  528,    0,    0,
    0,    0,    0,    0,  826,    0,    0,  204,    0,    0,
    0,    0, 1011,    0,    0,    0,    0,  841,    0,  528,
  841,    0,    0, 1009,    0,    0,  528,  841,    0,  204,
  841,    0, 1009,    0,  841,  528,    0,  528,    0,  841,
 1009,    0,  841,    0,  842,  826,    0,    0,  826,    0,
    0,    0,    0,    0, 1010,  528,    0,    0,  841, 1009,
  528, 1009, 1010,  826,  204,  204,    0, 1010,    0,    0,
 1010,    0,    0,    0,    0, 1009,    0, 1009, 1009, 1009,
    0,  204,  841,    0,    0,  841,  841,    0,    0,  528,
    0,  834,    0,    0,    0,  841,    0,    0,    0,    0,
  842,    0,    0,  842,    0,    0, 1010,    0,    0,    0,
  842,    0,  841,  842,    0, 1010,    0,  842,    0,    0,
    0,    0,  842, 1010,    0,  842,    0,    0, 1009,    0,
    0,    0,  834,  835,  841,  834,    0,    0,    0,  841,
    0,  842, 1010,    0, 1010,    0,    0,    0,    0,    0,
  834,  841,    0,  841,    0,    0,    0,    0, 1010,    0,
 1010, 1010, 1010,    0,    0,  842,    0,    0,  842,  842,
    0,    0,    0,    0,  835,    0,    0,  835,  842,    0,
    0,    0,    0,  843,    0,    0,    0,    0,    0,    0,
    0,    0,  835, 1011,  832,  842,    0,    0,    0,    0,
    0, 1011,    0,    0,    0,    0, 1011,    0,    0, 1011,
    0, 1010,    0,    0,    0,    0,    0,  842,    0,    0,
  841,    0,  842,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  842,  823,  842,    0,    0,  843,
    0,    0,  843,    0,    0, 1011,  827,    0,    0,  843,
  832,    0,  843,  832, 1011,    0,  843,    0,    0,    0,
  832,  843, 1011,  832,  843,    0,  826,  832,    0,    0,
    0,    0,  832,    0,    0,  832,  823,    0,    0,  823,
  843, 1011,    0, 1011,    0,    0,    0,  827,    0,    0,
  827,  832,    0,    0,  823,    0,    0, 1011,    0, 1011,
 1011, 1011,    0,  842,  843,  827,    0,  843,  843,    0,
    0,    0,    0,    0,    0,  832,    0,  843,  832,  832,
    0,    0,  826,    0,    0,  826,    0,    0,  832,    0,
    0,    0,  826,    0,  843,  826,    0,  836,    0,  826,
    0,    0,    0,    0,  826,  832,    0,  826,  824,    0,
 1011,    0,    0,  834,    0,    0,  843,    0,    0,    0,
    0,  843,    0,  826,    0,    0,    0,  832,    0,    0,
    0,    0,  832,  843,    0,  843,    0,    0,  836,    0,
    0,  836,    0,    0,  832,    0,  832,  826,    0,  824,
  826,  826,  824,    0,    0,  835,  836,    0,    0,    0,
  826,    0,    0,    0,    0,    0,    0,  824,    0,  834,
    0,  828,  834,    0,    0,    0,    0,  826,    0,  834,
    0,    0,  834,    0,    0,    0,  834,    0,    0,    0,
    0,  834,    0,    0,  834,    0,    0,    0,    0,  826,
    0,    0,  843,    0,  826,    0,    0,    0,    0,    0,
  834,  835,  828,  832,  835,  828,  826,    0,  826,    0,
    0,  835,    0,    0,  835,    0,    0,    0,  835,    0,
  828,    0,    0,  835,  834,    0,  835,  834,  834,    0,
    0,    0,    0,    0,    0,    0,    0,  834,    0,    0,
    0,    0,  835,    0,    0,    0,    0,  823,    0,    0,
    0,    0,    0,    0,  834,    0,    0,    0,  827,    0,
    0,    0,    0,    0,    0,    0,  835,    0,    0,  835,
  835,    0,    0,    0,    0,  826,  834,    0,    0,  835,
    0,  834,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  834,    0,  834,  835,  530,    0,    0,
    0,    0,    0,  823,    0,    0,  823,    0,    0,    0,
    0,    0,    0,  823,  827,    0,  823,  827,  835,    0,
  823,    0,    0,  835,  827,  823,    0,  827,  823,    0,
    0,  827,    0,    0,    0,  835,  827,  835,  530,  827,
    0,  530,    0,  659,  823,    0,  431,    0,  432,  836,
    0,    0,    0,    0,    0,  827,  530,    0,  530,    0,
  824,  434,  834,    0,    0,    0,  289,    0,  823,    0,
    0,  823,  823,    0,    0,    0,    0,    0,    0,  827,
    0,  823,  827,  827,    0,    0,    0,    0,    0,    0,
    0,    0,  827,    0,    0,    0,    0,    0,  823,    0,
    0,    0,    0,    0,  835,  836,    0,    0,  836,  827,
    0,    0,    0,    0,    0,  836,  824,    0,  836,  824,
  823,    0,  836,  828,    0,  823,  824,  836,    0,  824,
  836,  827,    0,  824,    0,    0,  827,  823,  824,  823,
  659,  824,    0,  431,    0,  432,  836,    0,  827,    0,
  827,    0,    0,    0,    0,    0,    0,  824,  434,    0,
    0,    0,    0,  289,    0,    0,    0,    0,    0,    0,
  836,    0,    0,  836,  836,    0,    0,    0,    0,  828,
    0,  824,  828,  836,  824,  824,    0,    0,    0,  828,
    0,    0,  828,    0,  824,    0,  828,    0,    0,    0,
  836,  828,    0,    0,  828,    0,  823,    0,    0,    0,
    0,  824,    0,    0,    0,    0,    0,  827,    0,    0,
  828,    0,  836,    0,    0,    0,    0,  836,    0,    0,
    0,    0,    0,  824,    0,    0,    0,    0,  824,  836,
    0,  836,    0,    0,  828,    0,    0,  828,  828,    0,
  824,    0,  824,    0,    0,  530,  477,  828,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  394,    0,    0,    0,  828,  530,    0,  530,    0,  395,
    0,  530,  107,    0,  530,    0,    0,    0,    0,    0,
    0,    0,  396,    0,    0,    0,  828,  530,  397,  398,
    0,  828,    0,  530,    0,    0,    0,    0,  836,    0,
    0,  108,  109,  828,  110,  828,    0,  653,    0,  824,
    0,  111,  112,  113,  114,  115,  399,  116,    0,  400,
  117,  118,  119,  120,  121,    0,    0,    0,  122,  123,
    0,    0,    0,    0,  124,  125,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  401,    0,  402,  403,    0,
    0,    0,    0,    0,    0,    0,    0,  394,    0,  404,
    0,    0,    0,  405,    0,  530,  395,    0,    0,    0,
    0,  883,  828,    0,    0,    0,    0,    0,  659,  396,
    0,  431,  652,  432,    0,  397,  398,  530,    0,    0,
    0,    0,    0,    0,  530,    0,  434,    0,    0,    0,
    0,  289,    0,    0,  653,  530,    0,    0,    0,    0,
  697,    0,    0,  399,  655,    0,  400,    0,    0,    0,
  406,    0,    0,  530,    0,    0,  407,    0,  530,    0,
    0,    0,    0,    0,    0,    0,  408,    0,    0,    0,
    0,    0,  401,    0,  402,  403,    0,    0,    0,    0,
  409,  410,  411,  412,  212,  213,  404,  530,    0,    0,
  405,    0,    0,    0,  413,    0,    0,    0,  654,    0,
    0,    0,    0,  414,  415,  416,  417,  418,  419,  420,
  421,  422,  108,  423,    0,  110,    0,    0,  424,  425,
    0,  426,  427,  428,  113,  114,  115,  429,  116,    0,
    0,  117,  118,  119,  120,  121,  884,  885,  886,  122,
  123,  655,    0,    0,    0,  124,  430,  406,    0,    0,
    0,    0,    0,  407,    0,    0,  565,    0,    0,    0,
    0,    0,    0,  408,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  409,  410,  411,
  412,  212,  213,    0,    0,    0,    0,    0,    0,    0,
    0,  413,    0,    0,    0,    0,    0,    0,    0,    0,
  414,  415,  416,  417,  418,  419,  420,  421,  422,  108,
  423,    0,  110,    0,    0,  424,  425,    0,  426,  427,
  428,  113,  114,  115,  429,  116,    0,    0,  117,  118,
  656,  657,  658,    0,    0,  394,  122,  123,    0,    0,
    0,    0,  124,  430,  395,    0,    0,  659,    0,    0,
  431,    0,  432,    0,    0,    0,    0,  396,    0,    0,
    0,    0,    0,  397,  398,  434,    0,    0,    0,    0,
  289,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  653,    0,    0,    0,    0,    0,    0,    0,
    0,  399,    0,    0,  400,    0,    0,    0,  942,  708,
  706,    0,  707,    0,  709,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  904,  903,  905,
  401,    0,  402,  403,    0,    0,    0,    0,    0,  107,
    0,    0,  869,    0,  404,    0,  597,    0,  405,    0,
    0,    0,    0,    0,  659,    0,  879,  431,    0,  432,
  870,    0,    0,    0,  598,    0,    0,  599,  108,  109,
    0,  110,  434,    0,    0,    0,    0,  289,  111,  112,
  113,  114,  115,    0,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,  655,
    0,  124,  125,    0,    0,  406,    0,    0,  600,    0,
    0,  407,    0,    0,  601,  602,  603,    0,  604,  605,
  606,  408,  607,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  409,  410,  411,  412,  212,
  213,    0,  608,    0,  609,    0,    0,    0,    0,  413,
    0,    0,    0,    0,    0,    0,    0,    0,  414,  415,
  416,  417,  418,  419,  420,  421,  422,  108,  423,    0,
  110,    0,    0,  424,  425,  610,  426,  427,  428,  113,
  114,  115,  429,  116,  394,    0,  117,  118,  656,  657,
  658,    0,  611,  395,  122,  123,    0,    0,    0,    0,
  124,  430,    0,    0,    0,    0,  396,    0,    0,    0,
    0,    0,  397,  398,  612,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  653,    0,    0,    0,    0,    0,    0,    0,  891,
  399,    0,    0,  400,    0,    0,    0,  704,    0,  613,
  614,    0,  705,    0,    0,  892,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  615,    0,    0,  401,
    0,  402,  403,    0,    0,    0,    0,    0,    0,    0,
    0,  394,    0,  404,    0,    0,    0,  405,    0,    0,
  395,  893,    0,    0,    0,  654,    0,    0,    0,    0,
  894,    0,    0,  396,    0,    0,    0,    0,  895,  397,
  398,    0,  501,    0,    0,  431,    0,  432,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  896,  653,  897,
  434,    0,    0,    0,    0,  289,    0,  399,  655,    0,
  400,    0,    0,  898,  406,  899,  900,  901,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,  401,    0,  402,  403,
    0,    0,    0,    0,  409,  410,  411,  412,  212,  213,
  404,    0,    0,    0,  405,    0,    0,    0,  413,    0,
    0,    0,  879,    0,    0,    0,  902,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,    0,  426,  427,  428,  113,  114,
  115,  429,  116,    0,    0,  117,  118,  656,  657,  658,
    0,    0,  571,  122,  123,  655,    0,    0,    0,  124,
  430,  406,    0,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  409,  410,  411,  412,  212,  213,    0,    0,    0,
    0,    0,    0,    0,    0,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,  415,  416,  417,  418,  419,
  420,  421,  422,  108,  423,    0,  110,    0,    0,  424,
  425,    0,  426,  427,  428,  113,  114,  115,  429,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,    0,
  122,  123, 1126,    0,    0, 1127,  124,  430,    0,  394,
    0,  204,    0,    0,    0,  204,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  204,
    0,  396,    0,  204,    0,    0,  204,  397,  398,    0,
  501,    0,    0,  431,    0,  432,    0,    0,    0,    0,
    0,    0,    0,    0,  708,  706,    0,  707,  434,  709,
    0,    0,    0,  289,    0,  399,    0,    0,  400,    0,
    0,    0,  904,  903,  905,  107,    0,  204,    0,    0,
    0,    0,    0,  204,  204,  204,    0,  204,  204,  204,
    0,  204,    0,    0,  401,    0,  402,  403,    0,    0,
    0,    0,    0,    0,  108,  109,    0,  110,  404,    0,
    0,  204,  405,  204,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117,  118,  119,  120,  121,    0,    0,
    0,  122,  123,    0,    0,    0,    0,  124,  125,    0,
  501,    0,    0,  431,  204,  432,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  434,    0,
    0,  204,    0,  289,    0,    0,    0, 1128,    0,  406,
    0,    0,    0,    0,    0,  407,    0,    0,    0,    0,
    0,    0,    0,  204,    0,  408,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  409,
  410,  411,  412,  212,  213,    0,    0,    0,    0,    0,
    0,    0,    0,  413,    0,    0,    0,    0,  204,  204,
    0,    0,  414,  415,  416,  417,  418,  419,  420,  421,
  422,  108,  423,    0,  110,  204,    0,  424,  425,    0,
  426,  427,  428,  113,  114,  115,  429,  116,    0,    0,
  117,  118,  119,  120,  121,    0,    0,    0,  122,  123,
 1136,    0,    0, 1127,  124,  430,    0,  394,  525,    0,
    0,    0,    0,    0,    0,    0,  395,    0,    0,    0,
    0,    0,    0,    0,  891,    0,    0,    0,    0,  396,
    0,    0,  704,    0,    0,  397,  398,  705,    0,    0,
  892,    0,    0,    0,    0,    0,    0,    0,    0,  525,
    0,    0,  525,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  399,    0,    0,  400,  525,    0,  525,
    0,    0,    0,    0,    0,    0,  893,    0,    0,    0,
    0,    0,    0,    0,    0,  894,    0,    0,    0,    0,
    0,    0,  401,  895,  402,  403,    0,    0,    0,    0,
 1140,    0,    0, 1127,    0,    0,  404,  394,    0,    0,
  405,    0,  896,    0,  897,    0,  395,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  898,  396,
  899,  900,  901,    0,    0,  397,  398,    0,  501,    0,
    0,  431,    0,  432,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  434,    0,    0,    0,
    0,  289,    0,  399,    0, 1128,  400,  406,    0,    0,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,  902,    0,  408,    0,    0,    0,    0,    0,    0,
    0,    0,  401,    0,  402,  403,    0,  409,  410,  411,
  412,  212,  213,    0,    0,    0,  404,    0,    0,    0,
  405,  413,    0,    0,    0,    0,    0,    0,    0,    0,
  414,  415,  416,  417,  418,  419,  420,  421,  422,  108,
  423,  100,  110,    0,    0,  424,  425,    0,  426,  427,
  428,  113,  114,  115,  429,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,  430,    0, 1128,  525,  406,    0,    0,
    0,    0,  101,  407,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  408,  102,  525,  525,    0,  525,    0,
    0,    0,  525,    0,    0,  525,    0,  409,  410,  411,
  412,  212,  213,    0,    0,    0,    0,    0,  525,  103,
    0,  413,    0,    0,  525,    0,    0,    0,    0,    0,
  414,  415,  416,  417,  418,  419,  420,  421,  422,  108,
  423,    0,  110,    0,    0,  424,  425,    0,  426,  427,
  428,  113,  114,  115,  429,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123, 1148,    0,
    0, 1127,  124,  430,    0,  394,    0,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,  104,    0,
    0,    0,    0,    0,    0,    0,  525,  396,  105,    0,
    0,    0,    0,  397,  398,    0,  501,    0,    0,  431,
    0,  432,    0,    0,    0,    0,    0,    0,  525,    0,
    0,    0,    0,  106,  434,  525,    0,    0,    0,  289,
    0,  399,    0,    0,  400,    0,  525,    0,    0,    0,
    0,  107,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  525,    0,    0,    0,    0,  525,
  401,    0,  402,  403,    0,    0,    0,    0,    0,    0,
  108,  109,    0,  110,  404,    0,    0,    0,  405,    0,
  111,  112,  113,  114,  115,    0,  116,    0,  525,  117,
  118,  119,  120,  121,    0,    0,    0,  122,  123,    0,
    0,    0,    0,  124,  125,    0,  501,    0,    0,  431,
    0,  432,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  434,    0,    0,    0,    0,  289,
    0,    0,    0, 1128,    0,  406,    0,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  409,  410,  411,  412,  212,
  213,    0,    0,    0,    0,    0,    0,    0,    0,  413,
    0,    0,    0,    0,    0,    0,    0,    0,  414,  415,
  416,  417,  418,  419,  420,  421,  422,  108,  423,    0,
  110,    0,    0,  424,  425,    0,  426,  427,  428,  113,
  114,  115,  429,  116,    0,    0,  117,  118,  119,  120,
  121,    0,    0,    0,  122,  123, 1151,    0,    0, 1127,
  124,  430,    0,  394,    0,  596,    0,    0,    0,  597,
    0,    0,  395,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  396,    0,  598,    0,    0,
  599,  397,  398,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  134,    0,    0,    0,  134,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  399,
    0,    0,  400,    0,  134,    0,    0,  134,    0,    0,
    0,  600,    0,    0,    0,    0,    0,  601,  602,  603,
    0,  604,  605,  606,    0,  607,    0,    0,  401,    0,
  402,  403,    0,    0,    0,    0, 1156,    0,    0, 1127,
    0,    0,  404,  394,    0,  608,  405,  609,  134,    0,
    0,    0,  395,    0,  134,  134,  134,    0,  134,  134,
  134,    0,  134,    0,    0,  396,    0,    0,    0,    0,
    0,  397,  398,    0,  501,    0,    0,  431,  610,  432,
    0,    0,  134,    0,  134,    0,    0,    0,    0,    0,
    0,    0,  434,    0,    0,  611,    0,  289,    0,  399,
    0, 1128,  400,  406,    0,    0,    0,    0,    0,  407,
    0,    0,    0,    0,    0,  134,    0,  612,    0,  408,
    0,    0,    0,    0,    0,    0,    0,    0,  401,    0,
  402,  403,  134,  409,  410,  411,  412,  212,  213,    0,
    0,    0,  404,    0,    0,    0,  405,  413,    0,    0,
    0,    0,  613,  614,  134,    0,  414,  415,  416,  417,
  418,  419,  420,  421,  422,  108,  423,    0,  110,  615,
    0,  424,  425,    0,  426,  427,  428,  113,  114,  115,
  429,  116,    0,    0,  117,  118,  119,  120,  121,  134,
  134,    0,  122,  123,    0,    0,    0,    0,  124,  430,
    0, 1128,    0,  406,    0,    0,  134,    0,    0,  407,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  408,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  409,  410,  411,  412,  212,  213,    0,
    0,    0,    0,    0,    0,    0,    0,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,  415,  416,  417,
  418,  419,  420,  421,  422,  108,  423,    0,  110,    0,
    0,  424,  425,    0,  426,  427,  428,  113,  114,  115,
  429,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123, 1159,    0,    0, 1127,  124,  430,
    0,  394,    0,    0,    0,  481,    0,    0,    0,    0,
  395,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  107,  396,    0,    0,    0,    0,    0,  397,
  398,    0,  501,    0,    0,  431,    0,  432,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  434,  108,  109,    0,  110,  289,    0,  399,    0,    0,
  400,  111,  112,  113,  114,  115,    0,  116,    0,    0,
  117,  118,  119,  120,  121,    0,    0,    0,  122,  123,
    0,    0,    0,    0,  124,  125,  401,    0,  402,  403,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,  405,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  654,    0,
  654,  654,    0,  654,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  654,    0,    0,  968,
    0,  654,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1128,
    0,  406,    0,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  409,  410,  411,  412,  212,  213,    0,    0,    0,
    0,    0,    0,    0,    0,  413,    0,    0,    0,    0,
    0,    0,    0,  969,  414,  415,  416,  417,  418,  419,
  420,  421,  422,  108,  423,    0,  110,    0,    0,  424,
  425,    0,  426,  427,  428,  113,  114,  115,  429,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,  970,
  122,  123, 1162,    0,  971, 1127,  124,  430,    0,  394,
  972,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,  107,    0,    0,    0,    0,    0,    0,    0,  973,
    0,  396,    0,    0,    0,    0,    0,  397,  398,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  108,  109,    0,  110,    0,    0,    0,    0,    0,    0,
  111,  112,  113,  114,  115,  399,  116,    0,  400,  117,
  118,  119,  120,  121,    0,    0,    0,  122,  123,    0,
    0,    0,    0,  124,  125,    0,    0,    0,  654,    0,
    0,    0,    0,    0,  401,  654,  402,  403,    0,    0,
    0,    0,    0,    0,  654,    0,    0,    0,  404,    0,
    0,    0,  405,    0,    0,    0,    0,  654,    0,    0,
    0,    0,    0,  654,  654,    0,    0,  648,    0,  648,
  648,  654,  648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  648,    0,    0,    0,    0,
  648,  654,    0,    0,  654,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1128,    0,  406,
    0,    0,    0,    0,    0,  407,    0,    0,    0,    0,
  654,    0,  654,  654,    0,  408,    0,    0,    0,    0,
    0,    0,    0,    0,  654,    0,    0,    0,  654,  409,
  410,  411,  412,  212,  213,    0,    0,    0,    0,    0,
    0,    0,    0,  413,    0,    0,    0,    0,    0,    0,
    0,    0,  414,  415,  416,  417,  418,  419,  420,  421,
  422,  108,  423,    0,  110,    0,    0,  424,  425,    0,
  426,  427,  428,  113,  114,  115,  429,  116,    0,    0,
  117,  118,  119,  120,  121,  654,    0,    0,  122,  123,
    0,  654,    0,    0,  124,  430,    0,    0,    0,    0,
    0,  654,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  654,  654,  654,  654,  654,
  654,    0,    0,    0,    0,    0,    0,    0,    0,  654,
    0,    0,    0,    0,    0,    0,    0,    0,  654,  654,
  654,  654,  654,  654,  654,  654,  654,  654,  654,    0,
  654,    0,    0,  654,  654,    0,  654,  654,  654,  654,
  654,  654,  654,  654,    0,    0,  654,  654,  654,  654,
  654,    0, 1721,    0,  654,  654,    0,  648,    0,    0,
  654,  654,    0,    0,  648,    0,    0,    0,    0,    0,
    0,    0,    0,  648,    0,    0,  107, 1722,    0,    0,
    0,    0,    0,    0,    0,    0,  648,    0,    0,    0,
    0,  654,  648,  648,  654,    0,  654,    0,    0,    0,
  648,    0,    0,    0,    0,  108,  109,    0,  110,  654,
    0,    0,    0,    0,  654,  111,  112,  113,  114,  115,
  648,  116,    0,  648,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0, 1723,    0,    0,  124,  125,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  648,
    0,  648,  648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  648,    0,    0,    0,  648,    0,  107,
    0,  976,    0,    0,    0,  976,    0,  501,    0,  506,
  431,    0,  432,    0,    0,    0,    0,    0,    0,    0,
    0,  978,    0,  976,    0,  434,  976,    0,  108,  109,
  289,  110,    0,    0,    0,    0,    0,    0,  111,  112,
  113,  114,  115,    0,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,  648,  122,  123,    0,    0,    0,
  648,  124,  125,    0,    0,    0,    0,  976,    0,    0,
  648,    0,    0,  976,  976,  976,    0,  976,  976,  976,
    0,  976,    0,    0,  648,  648,  648,  648,  648,  648,
    0,    0,    0,    0,    0,    0,    0,    0,  648,    0,
    0,  976,    0,  976,    0,    0,    0,  648,  648,  648,
  648,  648,  648,  648,  648,  648,  648,  648,    0,  648,
    0,    0,  648,  648,    0,  648,  648,  648,  648,  648,
  648,  648,  648,    0,  976,  648,  648,  648,  648,  648,
    0,  654,    0,  648,  648,    0,    0,    0,  654,  648,
  648,  976,    0,    0,    0,    0,    0,  654,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  654,    0,    0,  976,    0,    0,  654,  654,    0,    0,
    0,    0,    0,    0,  654,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  654,    0,    0,  654,  976,  976,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  976,    0,    0,    0,  573,
    0,    0,    0,  654,  394,  654,  654,  501,    0,    0,
  431,    0,  432,  395,    0,    0,    0,  654,    0,    0,
    0,  654,    0,    0,    0,  434,  396,    0,  573,  573,
  289,  573,  397,  398,    0,    0,    0,    0,  573,  573,
  573,  573,  573,    0,  573,    0,    0,  573,  573,  573,
  573,  573,    0,    0,    0,  573,  573,    0,    0,    0,
  399,  573,  573,  400,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  654,    0,
    0,    0,    0,    0,  654,    0,    0,    0,    0,  401,
    0,  402,  403,    0,  654,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  405,  654,  654,
  654,  654,  654,  654,    0,    0,    0,    0,    0,    0,
    0,    0,  654,    0,    0,    0,    0,    0,    0,    0,
    0,  654,  654,  654,  654,  654,  654,  654,  654,  654,
  654,  654,    0,  654,    0,    0,  654,  654,    0,  654,
  654,  654,  654,  654,  654,  654,  654,    0,    0,  654,
  654,  654,  654,  654,  406,    0,    0,  654,  654,    0,
  407,    0,    0,  654,  654,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  409,  410,  411,  412,  212,  213,
    0,    0,    0,    0,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,  394,  426,  427,  428,  113,  114,
  115,  429,  116,  395,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,  396,    0,    0,  124,
  430,  501,  397,  398,  431,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  434,
    0,    0,    0,    0,  289,    0,    0,    0,    0,    0,
  399,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  401,
    0,  402,  403,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  404,  273,    0,    0,  405,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  501,  945,    0,
  431,    0,  432,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  273,  273,  434,  273,    0,    0,    0,
  289,    0,    0,  273,  273,  273,  273,  273,    0,  273,
    0,    0,  273,  273,  273,  273,  273,    0,    0,    0,
  273,  273,    0,    0,  406,    0,  273,  273,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,    0,    0,  688,    0,
    0,    0,    0,    0,  409,  410,  411,  412,  212,  213,
    0,    0,    0,    0,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,    0,  426,  427,  428,  113,  114,
  115,  429,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,  394,  124,
  430,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  396,    0,    0,    0,    0,    0,  397,  398,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,  400,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  401,  394,  402,  403,  501,    0,    0,
  431,    0,  432,  395,    0,    0,    0,  404,    0,    0,
    0,  405,    0,    0,    0,  434,  396,    0,    0,    0,
  289,    0,  397,  398,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,  400,    0,    0,    0,    0,  697,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,  401,
    0,  402,  403,    0,  408,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  405,  409,  410,
  411,  412,  212,  213,    0,    0,    0,    0,    0,    0,
    0,    0,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,    0,  110,    0,    0,  424,  425,    0,  426,
  427,  428,  113,  114,  115,  429,  116,    0,    0,  117,
  118,  119,  120,  121,  406,    0,    0,  122,  123,    0,
  407,    0,    0,  124,  430,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  409,  410,  411,  412,  212,  213,
    0,    0,    0,    0,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,  394,  426,  427,  428,  113,  114,
  115,  429,  116,  395,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,  396,    0,    0,  124,
  430,  501,  397,  398,  431,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  434,
    0,    0,    0,    0,  289,    0,    0,    0,    0,    0,
  399,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  401,
    0,  402,  403,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  405,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  501,    0,    0,
  431,    0,  432,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  434,    0,    0,    0,    0,
  289,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  406,    0,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1154,    0,    0,  409,  410,  411,  412,  212,  213,
    0,    0,    0,    0,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,    0,  426,  427,  428,  113,  114,
  115,  429,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,  394,  124,
  430,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  396,    0,    0,    0,    0,    0,  397,  398,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,  400,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  401,  394,  402,  403,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,  404,    0,    0,
    0,  405,    0,    0,    0,  501,  396,    0,  431,    0,
  432,    0,  397,  398,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  434, 1693,    0,    0,    0,  289,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,  400,    0,    0,    0,    0, 1343,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,  401,
    0,  402,  403,    0,  408,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  405,  409,  410,
  411,  412,  212,  213,    0,    0,    0,    0,    0,    0,
    0,    0,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,    0,  110,    0,    0,  424,  425,    0,  426,
  427,  428,  113,  114,  115,  429,  116,    0,    0,  117,
  118,  119,  120,  121,  406,    0,    0,  122,  123,    0,
  407,    0,    0,  124,  430,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1356,    0,    0,  409,  410,  411,  412,  212,  213,
    0,    0,    0,    0,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,    0,  426,  427,  428,  113,  114,
  115,  429,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,  394,  122,  123,    0,    0,    0,    0,  124,
  430,  395,    0,    0,  970,    0,  970,  970,    0,  970,
    0,    0,    0,    0,  396,    0,    0,    0,    0,    0,
  397,  398,  970,    0,    0,    0,    0,  970,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  401,    0,  402,
  403,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,  405,    0,    0,    0,    0,
    0,  433,    0,    0,  431,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  434,
    0,    0,    0,    0,  289,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  406,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  408,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  409,  410,  411,  412,  212,  213,    0,    0,
    0,    0,    0,    0,    0,    0,  413,    0,    0,    0,
    0,    0,    0,    0,    0,  414,  415,  416,  417,  418,
  419,  420,  421,  422,  108,  423,    0,  110,    0,    0,
  424,  425,    0,  426,  427,  428,  113,  114,  115,  429,
  116,  970,    0,  117,  118,  119,  120,  121,    0,    0,
  970,  122,  123,    0,    0,    0,    0,  124,  430,    0,
    0,    0,    0,  970,    0,    0,    0,    0,    0,  970,
  970,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  970,    0,    0,
  970,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  970,    0,  970,  970,
    0,    0,    0,    0,    0,    0,    0,    0,  394,    0,
  970,    0,    0,    0,  970,    0,    0,  395,    0,    0,
  501,    0,    0,  431,    0,  432,    0,    0,    0,    0,
  396,    0,    0,    0,    0,    0,  397,  398,  434,    0,
    0,    0,    0,  289,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,  400,    0,    0,
    0,  970,    0,    0,    0,    0,    0,  970,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  970,    0,    0,
    0,    0,    0,  401,    0,  402,  403,    0,    0,    0,
    0,  970,  970,  970,  970,  970,  970,  404,    0,    0,
    0,  405,    0,    0,    0,  970,    0,    0,    0,    0,
    0,    0,    0,    0,  970,  970,  970,  970,  970,  970,
  970,  970,  970,  970,  970,    0,  970,    0,    0,  970,
  970,    0,  970,  970,  970,  970,  970,  970,  970,  970,
    0,    0,  970,  970,  970,  970,  970,    0,    0,    0,
  970,  970,    0,    0,    0,    0,  970,  970,  406,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  409,  410,
  411,  412,  212,  213,    0,    0,    0,    0,    0,    0,
    0,    0,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,    0,  110,    0,    0,  424,  425,    0,  426,
  427,  428,  113,  114,  115,  429,  116,  394,    0,  117,
  118,  119,  120,  121,    0,    0,  395,  122,  123,    0,
    0,    0,    0,  124,  430,    0,    0,    0,  970,  396,
    0,  970,    0,  970,    0,  397,  398,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  970,    0,    0,    0,
    0,  970,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  399,    0,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  401,    0,  402,  403,    0,    0,    0,    0,
    0,  540,    0,    0,    0,    0,  404,    0,    0,    0,
  405,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  540,  540,    0,    0,  540,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  540,    0,  540,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  408,    0,    0,    0,    0,    0,    0,
    0,    0,  540,    0,    0,    0,    0,  409,  410,  411,
  412,  212,  213,    0,    0,    0,    0,    0,    0,    0,
    0,  413,    0,    0,    0,    0,    0,    0,    0,    0,
  414,  415,  416,  417,  418,  419,  420,  421,  422,  108,
  423,    0,  110,    0,    0,  424,  425,    0,  426,  427,
  428,  113,  114,  115,  429,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,  970,  122,  123,    0,    0,
    0,    0,  124,  430,  970,    0,  541,    0,    0,    0,
    0,    0,    0,    0,  535,    0,    0,  970,    0,    0,
    0,    0,    0,  970,  970,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  541,  541,    0,    0,
  541,  970,  557,    0,  970,  535,    0,    0,  535,    0,
    0,    0,    0,    0,    0,  541,    0,  541,    0,    0,
    0,    0,    0,  535,    0,  535,    0,    0,    0,  547,
  970,    0,  970,  970,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  557,  970,    0,  557,  541,  970,  540,
    0,    0,    0,    0,    0,  535,    0,    0,    0,    0,
    0,  557,    0,  557,    0,  540,    0,    0,  540,  540,
  547,  540,    0,  547,    0,  540,    0,    0,  540,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  547,    0,
  547,  540,    0,  557,    0,    0,    0,  540,    0,    0,
  540,    0,    0,    0,    0,  970,    0,    0,    0,    0,
    0,  970,    0,    0,    0,    0,    0,    0,    0,    0,
  547,  970,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  970,  970,  970,  970,  970,
  970,    0,    0,    0,    0,    0,    0,    0,    0,  970,
    0,    0,    0,    0,    0,    0,    0,    0,  970,  970,
  970,  970,  970,  970,  970,  970,  970,  970,  970,  540,
  970,    0,    0,  970,  970,    0,  970,  970,  970,  970,
  970,  970,  970,  970,    0,    0,  970,  970,  970,  970,
  970,  540,    0,    0,  970,  970,    0,    0,  540,    0,
  970,  970,  540,    0,    0,    0,    0,    0,    0,  540,
    0,    0,    0,    0,  541,    0,    0,    0,    0,    0,
    0,    0,  535,    0,    0,    0,    0,  540,    0,    0,
  541,  532,  540,  541,  541,    0,  541,    0,  535,    0,
  541,  535,  535,  541,  535,    0,    0,    0,  535,    0,
    0,  535,    0,    0,    0,    0,  541,  540,    0,    0,
  557,  540,  541,    0,  535,  541,    0,    0,    0,    0,
  535,    0,  532,  535,    0,  532,  557,    0,    0,    0,
  557,    0,  557,    0,    0,    0,  557,  547,    0,  557,
  532,    0,  532,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  557,  547,    0,    0,  100,  547,  557,  547,
    0,  557,    0,  547,    0,    0,  547,    0,    0,    0,
    0,    0,  532,    0,    0,    0,    0,    0,    0,  547,
    0,    0,    0,    0,  541,  547,    0,    0,  547,    0,
    0,    0,  535,    0,    0,    0,    0,  101,    0,    0,
    0,    0,    0,    0,    0,    0,  541,    0,    0,  102,
    0,    0,    0,  541,  535,    0,    0,  541,    0,    0,
    0,  535,    0,  138,  541,  535,    0,    0,    0,    0,
  557,    0,  535,    0,  103,    0,    0,    0,    0,    0,
    0,    0,  541,    0,    0,    0,    0,  541,    0,    0,
  535,    0,  557,    0,    0,  535,    0,  547,    0,  557,
    0,    0,    0,  557,    0,    0,    0,    0,    0,    0,
  557,    0,  541,    0,    0,    0,  541,    0,    0,  547,
  535,    0,    0,    0,  535,    0,  547,    0,  557,    0,
  547,    0,    0,  557,    0,    0,    0,  547,    0,    0,
    0,    0,    0,  104,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  105,    0,  547,    0,    0,  557,    0,
  547,    0,  557,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  106,  532,
    0,    0,    0,    0,    0,  547,    0,    0,    0,  547,
    0,    0,    0,    0,    0,  532,  107,    0,    0,  532,
    0,  532,    0,    0,    0,  532,    0,    0,  532,    0,
    0,    0,    0,  313,    0,    0,    0,    0,    0,    0,
    0,  532,    0,    0,    0,  108,  109,  532,  110,    0,
  532,    0,    0,    0,    0,  111,  112,  113,  114,  115,
    0,  116,    0,  313,  117,  118,  119,  120,  121,    0,
  313,    0,  122,  123,  313,  313,    0,  313,  124,  125,
  313,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  313,    0,    0,    0,    0,    0,
  313,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  532,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1202,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  532,    0,    0,    0,    0,    0,    0,  532,    0,
    0,    0,  532,    0,    0,    0,    0,    0,    0,  532,
  313,    6,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1401, 1402,    0, 1403,    0,  532,    0,  313,
    0,    0,  532,    0,    0,    0, 1202,    0,    0,    0,
    0,    0, 1404,    0,  313,    0,    0,    0, 1405,    0,
    0,    0,    0,    0,  313,    0,    0,  532,    0,    0,
    0,  532,    0,  313,    0,  313,    6,    0,    0,    0,
    0,    0,  313,    0,    0,    0,    0, 1401, 1402,    0,
 1403,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1404,    0,    0,
    0,  313,  313, 1405,  313,    0,    0,    0, 1406,    0,
    0,  313,  313,  313,  313,  313,    0,  313,    0,    0,
  313,  313,  313,  313,  313,    0,    0,   14,  313,  313,
  313,  313,  313,    0,  313,  313,    0,    0,    0,    0,
    0,    0, 1407,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   16,    0,    0,    0,    0,    0,    0,    0,
    0, 1596,    0, 1406,    0,    0,    0,    0,    0,  596,
  107,    0,    0,  597,    0,    0,    0,    0,    0,    0,
  796,    0,   14,    0,    0,    0,    0,    0,    0,    0,
    0,  598,    0,    0,  599,    0,    0, 1407,    0, 1408,
  109,    0,  110,    0,    0,    0,    0,   16,    0,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117, 1409,
  119,  120,  121,    0,    0,  107,  122,  123, 1410, 1411,
 1412,    0,  124,  125,    0,  600,    0,    0,    0,    0,
    0,  601,  602,  603,    0,  604,  605,  606,    0,  607,
    0,    0,    0,    0, 1408,  109,    0,  110,    0,    0,
    0,    0,    0,    0,  111,  112,  113,  114,  115,  608,
  116,  609,    0,  117, 1409,  119,  120,  121,    0,    0,
    0,  122,  123, 1410, 1411, 1412,    0,  124,  125,  596,
    0,    0,    0,  597,    0,    0,    0,    0,    0,    0,
    0,    0,  610,    0,    0,    0,    0,    0,    0,    0,
    0,  598,    0,    0,  599,    0,    0,    0,    0,  611,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  612,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  600,  107,    0,    0,    0,
    0,  601,  602,  603,    0,  604,  605,  606,    0,  607,
    0,    0,    0,    0,    0,    0,  613,  614,    0,    0,
    0,    0,    0,    0,    0,  108,  109,    0,  110,  608,
    0,  609,    0,  615,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,  125,
    0,    0,  610,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  611,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  612,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  107,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  613,  614,    0,    0,
    0,    0,    0,    0,    0,  108,  109,    0,  110,    0,
    0,    0,    0,  615,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,  107,  122,  123,    0,    0,    0,    0,  124,  125,
  168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
  178,  179,  180,  181,  182,  183,    0,    0,  184,  185,
  108,  109,    0,  110,    0,    0,  186,  187,    0,    0,
  111,  112,  113,  114,  115,  188,  116,  189,    0,  117,
  118,  119,  120,  121,    0,  461,    0,  122,  123,  190,
  191,  192,  193,  124,  125,  461,  461,  461,  461,  461,
  461,  461,  461,  461,  461,  461,  461,  461,  461,  461,
    0,    0,  461,  461,  461,  461,    0,  461,    0,    0,
  461,  461,    0,    0,  461,  461,  461,  461,  461,  461,
  461,  461,    0,  461,  461,  461,  461,  461,    0,    0,
    0,  461,  461,  461,  461,  461,  461,  461,  461,
};
static short yycheck[] = {                                       9,
  155,   56,   12,  106,    4,   16,   16,   65,   18,    0,
  144,  145,   64,   58,  144,  145,   57,  529,  372,  372,
   69,  469,  878,  368,   92,   70,  882,   68,   86,   95,
   74,   64,  566,  343,  433,   90, 1201,  364,  512,  536,
   63, 1274,  712,   92,   54,   55,   56,   57,   58,   59,
   91, 1048,   96,  274,   64,   65,   66,   98,   68,  341,
   70,   71,  283,   73,   74,  941,   71,  489,   89,  697,
   41,  350,   82,   84,   84,   85,   86, 1075,   88,   89,
   90,   91,   93,   93,   94,   95,   96,  688,   98,   94,
  389,  136,  158,  694,  719,  291,  766,  349,  377,  738,
   41,   41,  135,    0,  137,  648,  734,   45,  641,  964,
  986,  559,  157,  561,  318,  156,   44,    0,  566, 1037,
    0, 1037,   41,  998,   44,  135,  136,  137,  149,    0,
 1048,    0, 1048,  236,   40,   40,    0,   41, 1037,  149,
  863,   44,    0,    0,   59,  155,  156,  157,  158, 1048,
    0,   41,  162, 1042,   45, 1098,    0,    0,   41, 1037,
 1049,   41,   59,  318, 1037,   44, 1037,  773,  738,   45,
 1048, 1060,   41, 1234,  197, 1048,   59, 1048, 1212,   59,
 1214,   40,   44,    0,  790,    0, 1421,  793,   59,   44,
   59,   41, 1081,   44,    0,   59,  802,   41,   41,  222,
  545,   59,   59,   40,   63, 1044,   45, 1322,   45,   59,
   63, 1219,  566,  566, 1522,   59,   59,    0,    0,   91,
  495,  281,  325, 1099,   41,   61,   41,  272,   40,  264,
    0,    0,  265,   44,    0,  268,  281,  749,  274,    0,
    0,  283,   59,  654,   59, 1410, 1411,  302,  593,   44,
  856,   63,  531,   59, 1093,  265,  165,  318,  268,   41,
  481,  306,  272,  227,  274,  275,    0,    0,   44, 1266,
 1267,  281,   41,  283,   44,   44,   59,   59,    0, 1272,
   41,   41,   44,   44,   44,  318,  262,  569,    0,   59,
   59,  402,  302,   59,  262,   45,  306,  596,   59,   59,
 1393,  320,    0, 1300,  703,  906,  907,   41, 1306,  342,
  264,   41,  711,   63,  318,    0,  349,  265,  351,  455,
  353,   41,    0,  356,   44,   59,   59,  438,   41,   41,
  385,  348,  342,  374,  489,  617,  937,   59,  383,  349,
  979,  351,  365,  353,  367,    0,  356,   59, 1266, 1267,
 1266, 1267,  991,  992,  302,  162,   41,  321,  381,   44,
 1235,   59, 1455,   12,  374,  375,  840, 1266, 1267,    0,
  375,  819,  260,  383,   59,  385,  411,  402,  355, 1223,
   12,   59, 1300,  320, 1300,    0,   61,  328, 1266, 1267,
  331,  403,  453, 1266, 1267, 1266, 1267,  439, 1459, 1632,
 1289, 1300,    0,  300,   59,  277,  402, 1246,  420,  979,
  386, 1526,  331, 1019, 1269,   41,  387,  436,  386,  309,
  329,  991, 1300, 1457,  434, 1590,   41, 1300,   59, 1300,
  466, 1739,  477, 1039, 1040, 1036,  300,  497,  318,  480,
    0,  320,  304,  488,   59,  328,  387,  492,  331,  453,
  584,  348,  497, 1059,  495,  396,  489,  411, 1313,  435,
  340,   59,   41,  318,  300,   44,  741,  477,  387,   41,
  480,  481,  883,  387,  318, 1319,   41,  396,  488,    0,
  383,  331,  492,  387,  348,  495,    0,  497,  264,  394,
  500,  506,  396,  142, 1487,  410,  340,  522, 1731,   59,
  517,  318,  784,   41,  387,  787,   41,  387,  729,  304,
  142,  328,  508,  396,  331,  492,  798,  585, 1119, 1120,
  458, 1122, 1123,  340,  547,  331,  440,   41,    0,  417,
  458,  264,  452,  566, 1769, 1533,  585,  387,   59,  791,
  431,  447,  264,  825,  387,   59,  396,  485,  744,  318,
  521, 1784,    0,  396,    0,  565,  566,  867,  340, 1092,
  440,  571,  984,  767,  587, 1108, 1801,  458,    0,  440,
  387,  340,  387, 1198,  454,  854, 1519,  876,  436,  396,
  521,  521,  458,  454,  318, 1818,  716,   59,  987, 1312,
 1229, 1480, 1467,   41,  431, 1484, 1485,  454, 1606,  458,
    0,    0,  521,  454,    0,  458,  340,  640, 1843,   41,
  454,   59,  767,   59,  637,  638, 1471,  521,  752,  458,
  517,  458,  383,  440,    0,  300,  409,   59,  340, 1864,
  640, 1520, 1867,  318,  440,  452,  262,  454,  521,  649,
    0,  521,   41,  454,  410,   44,  458, 1882,  454,  649,
  348,    0,  521,  517, 1255,  340,  386, 1750,  679,   59,
   59, 1500, 1538,   59,  397,   41,  264,  631,   44,  679,
  308,  521,  454,  386,  436,  397, 1277,  521,  521, 1518,
  318,  431,  452,   59,  717,  454,  719,  301,  721,  367,
  262,  411, 1689,  500,  704,  274,    0,  262,   44,   59,
  741, 1866,  712, 1030,  521, 1311,  521,  717,  458,  719,
   59,  721,  367,    0,  724, 1343,  761,  348,  763,  729,
  454,  654,  278,  262,  262,  340,  659,  262,  738, 1618,
  309,  741, 1131, 1622,  767,  301,  338,  747,  748,  772,
  501,  501,  454,  260,  286,  307,  308,  968, 1637,  817,
  971,  761,  521,  763,   41,   59,  766,  780,  791,  313,
  386,   42,  772,  319, 1046, 1654,   47, 1094,  817,  454,
  356, 1689,   59, 1689,    0,  824, 1058,   41, 1275,  300,
  257,  791,  850,  851,  852,  818,  819,  521,  837, 1644,
 1689,  465,  264,  467, 1193, 1650, 1106,    0, 1080,  397,
  262,  850,  851,  852,  318,   42,  273,  830,  818,  819,
  855, 1689,  258,  300,  386,  453, 1689,  502, 1689, 1183,
  865,  386, 1186,  277, 1298,  271,  340,  348,  442,  984,
 1304,   59,  411,   59, 1233,  356,  521, 1371,   41,  454,
  442,   44,    0,  429,  400,  855,    0,  386,  386, 1738,
    0,  386,    0,    0,  300,  865,   59,  334, 1747, 1169,
  306,  348,  308,  417,  264,  307,  308,  877, 1469,  352,
  318,  268,  393,  340,  395,    0,  442,    0,    0,  278,
  328,  280,    0,  331,    0,  441,  318,  466,  287,  451,
    0,  345,  340,   41,    0,  378,   44,  418, 1754, 1788,
  417,   59,  348,  273,  264,   59,  972,    0,  340,   59,
  356,   59,   59,    0,  257,  264,  262, 1026,   41,  461,
  462,   44,  443,   41,  386,  397,    0,  263,  969,  973,
  307,  308,  328,  319,   59,  331,   59,   59,  301,  387,
  454,   59,  318,   59, 1078,   93,  879, 1395,  396,   59,
  883,  984,  328,   59,   41,  331,    0,   44,  968,  969,
  970,  971,  972,  973,  340,  970,   59,  343,  365,  979,
   44, 1316,   59,  435, 1220,  338,  312,   16,  258,  378,
 1581,  991,  992,  429,  383,   59,  377,  323,  998, 1476,
  348,  334,  440, 1026, 1103,  246,  517,  397,  262,  280,
   44,  400,  308,  254,  285, 1287,  454,  998,  407,  451,
    0,  387,  318,   44,  400,   59, 1026, 1371, 1371, 1087,
  396, 1385,  454,  260,  328, 1389,  306,  331,   59, 1775,
  517,  318,  805, 1212, 1521, 1214, 1318, 1105, 1087,    0,
  386,    0,  441, 1111,  440,   84,    0, 1202,  397, 1295,
  325,   41,  278,  340,  280,  441, 1105,  448,  454, 1069,
 1572,  409, 1111, 1096,  440,  283,  303,    0,    0,   59,
 1103,  517, 1072,  521,  451, 1085,  452,  300,  454,  442,
   41,  397,   41,   44,    0,   44, 1096,   41, 1218, 1219,
 1220,  427,    0, 1103, 1764, 1607,  343,  301,   59, 1845,
   59,   44,  501,  461,  462,   59,  264,  405,   41,   41,
  264,  258,   44, 1714,  264,  318,   59, 1251, 1719, 1253,
  436,  368,  386,    0,  271,  328,   59,   59,  331,  352,
    0,  402,   93,  258,  338,    0,  440,  340,  418,  262,
  343,    0,  264,   59, 1656,  521,  271,  453,  452, 1201,
  454,   59,  378,  300,  470,  430,  436,    0,  456,  306,
 1743,  308,  437,  434,   41, 1198,  384,  454, 1201, 1202,
    0,   41,  278,  405,  280,  300,  493,  264,  449,  426,
 1314,  306,   59,  308,  387,  278,  503,  280, 1198,   59,
  470, 1201, 1202,  396,   59,  318,  261,  260,  321, 1796,
   59,  348, 1212, 1567, 1214,  328, 1789, 1489,  331,  377,
  292,   41,  335, 1810,   44,    0,   59,  340,  300, 1229,
  343,  439,  340,  348,  456, 1235,  342, 1688,  260,   59,
  322,  318,  342,   44,  321,    0,  359,  440,  456,  397,
  303,  328,    0,  397,  331, 1828, 1528,  397,  335,  452,
  418,  454,  272,  340,  328, 1716,  343,  331,  458,   44,
  383,    0,  410,  386,  387,    0, 1421,  377,  288,  343,
  458,  303,  359,  396,   59,  397, 1877, 1860,    0, 1052,
 1053, 1878,   93, 1795,  328, 1746, 1741,  331,  417,  405,
  413,   40,  408, 1293,   59,  405,  383,  465,  408,  343,
  387,   59,   41, 1593,  322,   44, 1329, 1462,  424,  396,
 1333, 1752,  435,  333,  424,  444,    0,  440,  521,   41,
   59,  265,  280,    0,   59,  321,  413,  285,  318,  452,
 1103,  454,  378, 1774,  450,    0, 1791,   59,  328,  335,
  450,  331,  362, 1401,   42,   43,  278,   45,  280,   47,
  340,  259, 1793,  440,  400,  287,  264,   40,  302,  318,
    0,  407,  270,  359,  318,  452,  440,  454,    0, 1651,
  517, 1832,  331,  460, 1829,   59,   41,  331,  452,   44,
  454,  340,   59,  383,    0,  318,  340,  383, 1421,  401,
   44, 1401,  517,  337,   59,  441,  440,  387,  521, 1409,
 1410, 1411,  489,  490,  383,  492,  396,  340,  452,  429,
  454, 1421,  499,  500,  501,  502,  503,  413,  505,   59,
 1861,  508,  509,  510,  511,  512,    0,   59,  387,  516,
  517,    0,  264,  387,  521,  522,  523,  396, 1593,   93,
   44,  260,  396,   59,  385, 1490,  378, 1457,  318,  410,
  440,  260, 1462, 1608,  387,  321,  288, 1467, 1893,  260,
 1895,  331,  383,  396,  454,  297,    0, 1757,  400,  335,
  340, 1761,  383, 1763,  436,  407, 1606,  342,   44,   44,
 1490,  440, 1772,  342,  303,   59,  440,  301,  318,  397,
   59,  321,    0,  359,  303,  454,  328,  321,  328,  331,
  454,  331,  303,   40,    0,  335,  494,  440, 1798,  441,
  340,  335,  377,  343,    0,    0,    0,  387,  377,  343,
    0,  454,  337,  405,  338,   59,  396,   93,   93,  359,
  418,  521, 1635,   42,   43,  359,   45, 1578,   47,  278,
  405,  280,  424,  408,  274, 1835,  405,  413,  287,  408,
 1840,   59,  521,  383,    0,   41,  386,  387,   44,  424,
  424,   41,  521,   59,   44,  424,  396,  521, 1578,  501,
  440,    0,   40,   59,   59,   59,    0,  461,  260,   59,
 1590, 1636,  280,  413,  454,  450, 1631,  285,  521,  413,
    0,  450,  264,  407,  426,    0,  318,  356, 1608,  431,
  432,  433, 1757,  417,    0,  435, 1761,  287, 1763,  316,
  440, 1602,   41,   59, 1769,   44, 1626, 1772,  340,  301,
  300, 1631,  452,  458,  454,  318, 1636,  258,  442, 1732,
   59,  313,  460,  417,  393,   59,  395, 1660,  337,  378,
  271, 1664,  258, 1798, 1655, 1655, 1801,   40, 1706,   59,
  485,  521,    0,  452,   59,  271,  338,    0,  342,  418,
  444,  400,  336,   59,    0,  387,  411,    0,  407,  501,
    0,  402,  321,  505,  396,  306,  294,  308,   42,   43,
 1835,   45,   40,   47,  443, 1840,  335,  305, 1843,  340,
  306,  521,  308,  377,  312,   59, 1706, 1707,   42,   43,
   58,   45,  441,   47,    0,   40,  259,  320, 1708, 1864,
  359,   59, 1867,  316,  317,   59,   59,  270,  440,  337,
    0,  405,  318,   59,  408,  407,   59, 1882,    0,   59,
 1741, 1741,  454,    0,  383,  417, 1769,    0,  436,   41,
  424,  258,   44,  342,  261,   42,   43, 1833,   45,    0,
   47,  337,   42,   43, 1764,   45,  402,   47, 1844, 1769,
  442,  383,  501,   59,  413, 1775,  450,  337, 1801, 1780,
 1780,  280,   42,   43,  506,   45,  285,   47,  377,   59,
 1791, 1791,   41,  259,  402,   44,  383,   59,  374,  306,
   41, 1801,   59,   44,  270,  436,   59, 1852,  278,  521,
  280,  387, 1813, 1813,  374, 1891,  405,  287,   59,  408,
 1843,  320,   41,  361,   46,   44,  434,  387, 1829, 1829,
  438,   41,  493,  494,   44,  424,   58,  659,  434,   61,
 1862, 1864,  384, 1843, 1867, 1845,  450, 1869, 1849, 1849,
   42,   43, 1852,   45,  676,   47,  417,   41,  680, 1882,
   44,  450,  461,  462, 1864,  687, 1866, 1867,  391,  392,
  692,  693, 1872,  318, 1896,  517,    0,  457,  458,  459,
  287,  703, 1882,  705,  706,  707,  708,  709,  710,  711,
  417,   41,   46, 1893,   44, 1895,  321,   41,  516,  318,
   44,  284,  321,  516,   58,  485,  486,   61,  378,  328,
  335,   41,  331,   44,   44,  469,  335,   41,  343,   41,
   44,  340,   44,   61,  343,  372,  280,  286,  750,   41,
  400,  285,   44,  460,  359,   59,  264,  407,   42,   43,
  359,   45,  461,   47,   42,   43,  280,   45,  770,   47,
   40,  285,   41,  775,  776,   44,   41,   41,    0,   44,
   44,   40,  489,  490,  383,  492,   41,    0,  387,   44,
   40,  441,  499,  500,  501,  502,  503,  396,  505, 1593,
   40,  508,  509,  510,  511,  512,  340,   40,  413,  516,
  517,   40,   40,  280,  413,  522,  523,  461,  285,   41,
  280,   41,   44,   46,   44,  285,    0,   40,   41,   42,
   43,   44,   45,   46,   47,   58,   41,   59,   61,   44,
  280,  440,  461,  462,   40,  285,   59,   60,   61,   62,
   40,  501,   40,  452,   40,  454,   40,  278,   41,  280,
  320,   44,   41,  284,  461,   44,  287,   41,   44,   41,
   44,  311,   44,  340,   40,  675,   40,  677,   91,  300,
   93,  681,  682,  683,    0,   59,   40,   40,   91,  891,
  892,  893,   61,  265,  896,  897,  898,  460,  900,  901,
  902,  903,  904,  905,   46,   40,  908,  909,  280,  911,
  264,  913,  436,  285,  916,  917,  918,  919,  920,  921,
   41,   44,  521,   44,  320,   41,  489,  490,   44,  492,
  302,  933,  460,  935,  402,  320,  499,  500,  501,  502,
  503,   44,  505,   59,   44,  508,  509,  510,  511,  512,
  428,   46,   61,  516,  517,   40,  264,  378,  262,  522,
  523,  489,  490, 1757,  492,   40,   44, 1761,  442, 1763,
  454,  499,  500,  501,  502,  503,  264,  505, 1772,  400,
  508,  509,  510,  511,  512,  987,  407,  348,  516,  517,
  348,  348,  278,  278,  522,  523,  280,  300,  436,  397,
  470,  285,  280,  397, 1798,  458,  277,  285,  348,    0,
  399,  447,  447,   40,  318,   40,  349,  321,   91,   40,
  441,  896,   40,  898,  328,  900,  901,  331,  903,  904,
  905,  335,  264,  311,  318,  318,  340,  383,  411,  343,
   44, 1835,   42,   43,  389,   45, 1840,   47,   40,  262,
   41,  264,  265,   44,  382,  359,  278,   40,  280,  272,
  451,   41,  284,  277,   63,  287,  436,  280,   59,  299,
  317,  464,  285,   40,  320,  288, 1757,  320,  300,  383,
 1761,  452,  386,  387,   41,   42,   43,  264,   45,  302,
   47, 1772,  396,  448,  307,  308,   42,   43,  311,   45,
  382,   47,  502,   40,  278,  318,  280,  320,  321,  413,
  505,  324,  274,  287,   40,  328,  284, 1798,  331,  300,
  333,  278,  335,  344,  386,  344,  300,  340,  341, 1131,
  343,  435,  386,  386,   40,  300,  440,  418,  418,  416,
  356, 1143, 1144,  417, 1146, 1147,  359,  360,  452,  362,
  454,   40, 1154,   40, 1835,  274,  378,  417,   41, 1840,
   44,  320,  278,  376,  280,  378,  379,  380,  280,  271,
  383, 1173,   44,  386,  387,  385,  367,   44,  400, 1181,
 1182, 1183,  320,  396, 1186,  407, 1188,   40,  446,   44,
 1192, 1193, 1194, 1195,  328,  266,   40,  410,   40,  264,
  413,    0,  501,   41,  378,  422,   41,   42,   43,   58,
   45,   41,   47,   41,   41,    0,  429,  521,  264,  441,
  454, 1223,  435,  436,  394,  440,  400,  440,   41,   42,
   43, 1233,   45,  407,   47,  309,  492,  309,  451,  452,
   41,  454,   41,   41,   40,   44,   40,  460,   41,   42,
   43,  352,   45,   40,   47,  262,   41,   40,   40,   44,
   59,   40,  378,   40,  264,   40,   40,  441, 1270,   40,
   40,   40,   40,   40,   59,   40,  489,  490,   40,  492,
  280,   44,  308,  435,  400,  285,  499,  500,  501,  502,
  503,  407,  505,  327,   41,  508,  509,  510,  511,  512,
   41,  264,  388,  516,  517,  388,  388,  343,  521,  522,
  523,  331,   41,   42,   43,  274,   45, 1319,   47,    0,
   41,   42,   43,  280,   45,  441,   47,  318,  285,   44,
  321,    0,    0,  391,  280,  287,  386,  328, 1340,  285,
  331,  407,  386,  386,  335,  386,  386, 1349,  386,  340,
   41,  458,  343,   41, 1356,  378,  278,   40,  417,   41,
   41,   42,   43,   44,   45,   46,   47,   41,  359,   41,
   41,  435,   41,   41,  308,   44,   44,  293,   59,   60,
   61,   62,   59, 1385,   59,  460,   40, 1389,  308,  340,
   59,   59,  383,   61,   59,   40,  387,   59,   59,    0,
   59,  340,   59,   59, 1406,  396,   59,   41,   42,   43,
   91,   45,   93,   47,  489,  490,   58,  492,  396,  385,
  436,  280,  413,  337,  499,  500,  501,  502,  503,  442,
  505,  338,  442,  508,  509,  510,  511,  512,  301,  338,
   41,  516,  517,   44,  301,  280,   41,  522,  523,  440,
  285,   41,  352,  385,  320,   41,  371,   41,   59,  371,
   41,  452,   41,  454,   41,   41, 1468,  280,   41,  460,
   41,   41,  285,   42,   43,   44,   45,   41,   47,  278,
   41,  280,   41,   41,   41,   41,   41,  280,  287,   41,
  451,   59,  285,  278,  318,  280,    0,   40,  489,  490,
   44,  492,  287,   41,  352,  344,   40,  340,  499,  500,
  501,  502,  503,   59,  505,  300,   46,  508,  509,  510,
  511,  512,  508,  340,   40,  516,  517,  368,   40,  383,
  521,  522,  523,   44,  410,   41,  383,   41,  435,  343,
   44,   41,   41,  383, 1546, 1547,   59,   44,   41,   42,
   43,  280,   45,  304,   47,   59,  285,   41,   44,  280,
  442, 1563,  301,  307,  285, 1567,   41,   42,   43, 1571,
   45,  387,   47,  293,  304, 1577,   40, 1579,  463,  378,
  337,  262,   44,  264,  265,   41,   40,    0,   41,  337,
  258,  272,  383,  378,   41,   42,   43,  383,   45,  280,
   47,  400,   40,  320,  285,  278,  274,  288,  407,  278,
  470,  280,  264,  264,  411,  400,  389,   44,  287,  451,
   40,  302,  407,  304,  458,  271,  307,  308,    0,  309,
  311,  271,  300,  458,  264,  435,  343,  318,  306,  320,
  321,  309,  441,  324,  436,  343,  280,  328,  343,  343,
  331,  285,  333,  458,  335,   41,  441, 1659,   41,  340,
  341,  262,  343,   42,   43, 1667,   45,   59,   47,   41,
   42,   43,   44,   45,   46,   47,  308,   59,  359,  360,
   40,  362,   41,   40,   59,   59,  304,   59,   60,   61,
   62,   42,   43, 1695,   45,  376,   47,  378,  379,  380,
   41,   59,  383,   41,   41,  386,  387,   42,    0,  378,
    0,  280,    0,    0, 1215,  396,  285,  318, 1720,   91,
  321,   93,  276,   12,  275, 1108,  729,  328,  868,  410,
  331,  400,  413,  283,  335,  644,   88, 1106,  407,  340,
 1103, 1509,  343,  411, 1095, 1508,  339,  369,  429,   41,
  418,  323,   44, 1085,  435,  436,  593, 1033,  359,  440,
   42,   43,   44,   45,  772,   47,  351,   59,  436, 1771,
  451,  452,  441,  454,  278, 1096,  280,  351, 1573,  460,
 1626,  800,  383, 1782, 1489,  386,  387,  280, 1785, 1487,
  571, 1786,  285, 1820,  791,  396, 1695, 1409,  466, 1598,
 1770, 1248,  470, 1021,  965,  280,  717, 1472,  489,  490,
  285,  492,  413,  344, 1741,  460,  356,  573,  499,  500,
  501,  502,  503,  337,  505,  847, 1302,  508,  509,  510,
  511,  512,   13,  280,  435,  516,  517,  839,  285,  440,
  521,  522,  523,  310,  489,  490,  630,  492,  514,  147,
  775,  452,  750,  454,  499,  500,  501,  502,  503,  761,
  505,  998, 1720,  508,  509,  510,  511,  512,  518, 1616,
 1813,  516,  517, 1849,  378,  737, 1257,  522,  523, 1145,
   41,   42,   43,  988,   45,  264,   47, 1188,  529,   -1,
  262,   -1,  264,  265,   -1,   -1,  400,   -1,   -1,   -1,
  272,  280,   -1,  407,   -1,   -1,  285,   -1,  280,   -1,
   -1,  262,   -1,  285,   -1,   -1,  288,   41,   42,   43,
  521,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,  280,
  302,   -1,    0,   -1,  285,  307,  308,  441,   -1,  311,
   -1,   41,   42,   43,   -1,   45,  318,   47,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
  262,  343,   -1,   41,   42,   43,   44,   45,   46,   47,
   41,   42,   43,   -1,   45,   -1,   47,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,  376,   -1,  378,  379,  380,   -1,
   42,  383,    0,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   91,  396,   93,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  410,  331,
   -1,  413,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   41,   -1,   -1,   44,  429,   -1,   -1,
   -1,   -1,   -1,  435,  436,   -1,   -1,  359,  440,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   41,   42,   43,
   -1,   45,   -1,   47,  396,   41,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,  489,  490,  280,
  492,  413,   -1,   -1,  285,  460,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  435,  516,  517,   -1,   -1,  440,  521,
  522,  523,   -1,   -1,  489,  490,  280,  492,   -1,   -1,
  452,  285,  454,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  280,  516,  517,   -1,   -1,  285,   -1,  522,  523,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,   41,   42,   43,
   -1,   45,  280,   47,   -1,   -1,   -1,  285,   -1,  280,
  288,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,  521,
   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,  307,
  308,   -1,   -1,  311,   42,   43,   44,   45,   -1,   47,
  318,   -1,  320,  321,   42,   43,  324,   45,   -1,   47,
  328,   -1,   -1,  331,  296,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,  262,  343,   -1,   41,   42,   43,
   44,   45,   46,   47,   41,   42,   43,   -1,   45,   -1,
   47,  359,  360,   -1,  362,   59,   60,   61,   62,  472,
  473,  474,  475,  476,  477,  478,  479,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,    0,   44,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,  396,   93,
  318,   -1,   -1,  321,   -1,   -1,  280,   -1,   -1,   -1,
  328,  285,  410,  331,  280,  413,   -1,  335,   -1,  285,
   -1,   -1,  340,   -1,   -1,  343,   -1,   41,   -1,   -1,
   44,  429,   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,
   -1,  359,  440,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454, 1341, 1342,   -1,
   -1,   -1,  460, 1347, 1348,  383, 1350, 1351,  386,  387,
   -1,   -1,   -1,   -1, 1358, 1359, 1360, 1361,  396, 1363,
 1364, 1365, 1366, 1367, 1368,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,  413,   -1,   -1,  460,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  280,  435,  516,  517,
   -1,  285,  440,  521,  522,  523,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,  452,   -1,  454,  499,  500,  501,
  502,  503,   -1,  505,  262,   -1,  508,  509,  510,  511,
  512,   -1,  280,   -1,  516,  517,   -1,  285,   -1,   -1,
  522,  523,  280,   -1,   -1,  305,   -1,  285,  262,   -1,
  264,  265,  312,   -1,   -1,   -1,  316,  317,  272,   -1,
   -1,   -1,   -1,   -1,   -1,  325,  280,   -1,   -1,   -1,
   -1,  285,   -1,  280,  288,   -1,   -1,  337,  285,   -1,
   -1,   -1,   -1,  521,   -1,   -1,   -1,   -1,  302,   -1,
    0,   -1,   -1,  307,  308,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,  262,  343,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,  402,  320,   -1,  359,  360,   -1,  362,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,
    0,   45,  386,  387,  434,   -1,   -1,   -1,  438,   -1,
   -1,   91,  396,   93,  318,   -1,   -1,  321,   -1,  449,
   -1,   -1,   -1,   -1,  328,   -1,  410,  331,   -1,  413,
   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,  468,  343,
   -1,   41,   -1,   -1,   44,  429,   -1,   -1,   -1,   -1,
   -1,  435,  436,   -1,   -1,  359,  440,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  413,
   -1,   -1,   -1,  460,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  435,  516,  517,   -1,   -1,  440,  521,  522,  523,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  452,   -1,
  454,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,  523,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  521,   -1,   -1,
   -1,  294,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   -1,  311,  305,   -1,   -1,   -1,   -1,   -1,  318,  312,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,  325,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,  262,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,  386,   -1,   -1,  396,   93,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,  402,
  410,  331,   -1,  413,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   41,  419,   -1,   44,  429,
   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,   -1,  359,
  440,  434,   -1,   59,   -1,  438,   -1,   -1,  441,   -1,
   -1,  451,  452,   -1,  454,   -1,  449,   -1,   -1,   -1,
  460,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  468,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,  413,  458,   -1,  460,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  435,  516,  517,   -1,   -1,
  440,  521,  522,  523,   -1,  489,  490,   -1,  492,   -1,
   -1,    0,  452,   -1,  454,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  523,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   40,   41,   -1,   -1,   44,  272,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,  285,
   59,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  521,   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,
   -1,  307,  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   59,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,  262,  343,   -1,   41,
   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   93,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,  410,  331,   -1,  413,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
  436,   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,  413,   -1,   -1,
   -1,  261,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,  435,
  516,  517,  282,   -1,  440,  521,  522,  523,   -1,   -1,
  290,   -1,   -1,   -1,   -1,   -1,  452,  297,  454,  299,
    0,  301,   -1,   -1,   -1,   -1,  306,   -1,   -1,  318,
   -1,   -1,  321,  313,   -1,   -1,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,  327,   -1,   -1,
  262,  340,  264,  265,  343,   -1,   -1,   -1,  338,   -1,
  272,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
  359,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,  521,   -1,   -1,   -1,   -1,
  302,   -1,   -1,   -1,  383,  307,  308,   -1,  387,  311,
    0,   -1,   -1,   -1,   -1,   -1,  318,  396,  320,  321,
   -1,   -1,  324,   93,   -1,   -1,  328,   59,   -1,  331,
   -1,  333,   -1,  335,  413,  405,   -1,   -1,  340,  341,
   -1,  343,  412,    0,   -1,  415,   -1,  417,  418,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,  359,  360,   -1,
  362,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,  442,  452,  376,  454,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,  491,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,
   -1,   -1,   -1,  435,  436,   -1,  506,  507,  440,   -1,
   -1,   -1,  521,  489,  490,   -1,  492,   -1,   -1,  451,
  452,   -1,  454,  499,  500,  501,  502,  503,  460,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,  523,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,  489,  490,   -1,
  492,   -1,  262,   -1,  264,  265,   -1,  499,  500,  501,
  502,  503,  272,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,  288,  521,
  522,  523,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,  262,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   93,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,
  410,  331,   -1,  413,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,  321,  435,  436,   -1,   -1,  359,
  440,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,  451,  452,   -1,  454,   -1,  343,   -1,   -1,   -1,
  460,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,  413,   -1,   -1,  383,   -1,  460,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  435,  516,  517,   -1,   -1,
  440,  521,  522,  523,   -1,   -1,  413,  489,  490,   -1,
  492,  418,  452,   -1,  454,   -1,    0,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  440,  516,  517,   -1,   -1,   -1,   -1,
  522,  523,   -1,   -1,   -1,  452,  262,  454,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   41,   -1,   43,
   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,
   -1,   -1,  288,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,  521,   -1,   -1,   -1,   -1,  302,   -1,   -1,   -1,
    0,  307,  308,   -1,   -1,  311,    0,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   93,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,  460,   41,   -1,   -1,
   44,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,  489,  490,  383,  492,   -1,
  386,  387,   -1,   -1,   -1,  499,  500,  501,  502,  503,
  396,  505,   -1,  443,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  410,   -1,   -1,  413,  522,  523,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
  436,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,  451,  452,   -1,  454,  499,
  500,  501,  502,  503,  460,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  522,  523,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,  262,   -1,
  264,  265,   -1,  499,  500,  501,  502,  503,  272,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,  288,  521,  522,  523,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  302,   -1,
    0,   -1,   -1,  307,  308,   -1,   45,  311,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,  262,  343,
   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  321,  396,   93,  318,   -1,   -1,  321,  328,   -1,
   -1,  331,   -1,   -1,  328,  335,  410,  331,   -1,  413,
   -1,  335,   -1,  343,   -1,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,  359,
   -1,  435,  436,   -1,   -1,  359,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,  383,   -1,   -1,  460,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,  489,  490,   -1,  492,  413,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  440,  435,  516,  517,   -1,   -1,  440,  521,  522,  523,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,  452,   -1,
  454,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   41,   -1,   43,   44,   45,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,   -1,  288,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  521,   -1,   -1,
   -1,   -1,  302,   -1,   -1,   -1,    0,  307,  308,   -1,
   -1,  311,    0,  352,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   93,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,  359,
  360,   -1,  362,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,  427,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,  457,  458,
  459,   -1,  461,  462,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,   -1,   -1,
  440,  480,  481,  482,  483,  484,  485,  486,  487,  488,
   -1,  451,  452,   -1,  454,   -1,  495,  496,   -1,   -1,
  460,   -1,   -1,   -1,   -1,  504,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,  262,   -1,  264,  265,   -1,  499,
  500,  501,  502,  503,  272,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
  288,  521,  522,  523,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,  307,
  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,  262,  343,   -1,   41,   -1,   43,
   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  360,   -1,  362,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,    0,   -1,  386,  387,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,  396,   93,
  318,   -1,   -1,  321,  328,   -1,   -1,  331,   -1,   -1,
  328,  335,  410,  331,   -1,  413,  340,  335,   -1,  343,
   -1,   -1,  340,   -1,   -1,  343,   40,   41,   -1,   -1,
   44,  429,   -1,   -1,   -1,  359,   -1,  435,  436,   -1,
   -1,  359,  440,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,  383,
   -1,   -1,  460,  387,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,  489,  490,   -1,  492,  413,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  440,  435,  516,  517,
   -1,   -1,  440,  521,  522,  523,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  288,   -1,   -1,  521,   -1,   -1,
   -1,   -1,   -1,  521,   -1,   -1,   -1,   -1,  302,   -1,
   -1,   -1,   -1,  307,  308,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,  318,   -1,   -1,  321,   -1,    0,
   -1,   -1,   -1,   -1,  328,   -1,  410,  331,   -1,  413,
   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,
   -1,  435,  436,   -1,   -1,  359,  440,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   59,  383,
   61,   -1,   -1,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  413,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,  440,  521,  522,  523,
   41,   42,   43,   44,   45,   46,   47,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,  489,  490,   -1,  492,   -1,
   91,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,  521,  522,  523,
   -1,   -1,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   91,   -1,  291,   -1,  258,   -1,  295,
   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  277,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,
   -1,    0,   -1,  339,   -1,  306,   -1,   -1,   -1,  345,
  346,  347,    0,  349,  350,  351,   -1,  353,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,  262,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,  272,   -1,   41,   -1,   -1,   44,   -1,   -1,  280,
   59,   60,   61,   62,  285,   -1,   -1,  288,   -1,   -1,
  406,   59,   -1,   -1,   -1,  411,   -1,  378,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,  423,   -1,   -1,
   -1,   -1,   91,   -1,   -1,   -1,   -1,  318,   -1,  400,
  321,   -1,   -1,  324,   -1,   -1,  407,  328,   -1,  445,
  331,   -1,  333,   -1,  335,  416,   -1,  418,   -1,  340,
  341,   -1,  343,   -1,  262,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,  272,  436,   -1,   -1,  359,  360,
  441,  362,  280,   59,  480,  481,   -1,  285,   -1,   -1,
  288,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,  497,  383,   -1,   -1,  386,  387,   -1,   -1,  470,
   -1,    0,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,  413,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,  429,   -1,
   -1,   -1,   41,    0,  435,   44,   -1,   -1,   -1,  440,
   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,
   59,  452,   -1,  454,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  396,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  272,  262,  413,   -1,   -1,   -1,   -1,
   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,
   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
  521,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,    0,  454,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,  324,    0,   -1,   -1,  328,
  318,   -1,  331,  321,  333,   -1,  335,   -1,   -1,   -1,
  328,  340,  341,  331,  343,   -1,  262,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   41,   -1,   -1,   44,
  359,  360,   -1,  362,   -1,   -1,   -1,   41,   -1,   -1,
   44,  359,   -1,   -1,   59,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,  521,  383,   59,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,  383,   -1,  396,  386,  387,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,  396,   -1,
   -1,   -1,  328,   -1,  413,  331,   -1,    0,   -1,  335,
   -1,   -1,   -1,   -1,  340,  413,   -1,  343,    0,   -1,
  429,   -1,   -1,  262,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,  359,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,  452,   -1,  454,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,  452,   -1,  454,  383,   -1,   41,
  386,  387,   44,   -1,   -1,  262,   59,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,  318,
   -1,    0,  321,   -1,   -1,   -1,   -1,  413,   -1,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,  435,
   -1,   -1,  521,   -1,  440,   -1,   -1,   -1,   -1,   -1,
  359,  318,   41,  521,  321,   44,  452,   -1,  454,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   59,   -1,   -1,  340,  383,   -1,  343,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,  521,  435,   -1,   -1,  396,
   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,  454,  413,    0,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,  318,   -1,  331,  321,  435,   -1,
  335,   -1,   -1,  440,  328,  340,   -1,  331,  343,   -1,
   -1,  335,   -1,   -1,   -1,  452,  340,  454,   41,  343,
   -1,   44,   -1,   40,  359,   -1,   43,   -1,   45,  262,
   -1,   -1,   -1,   -1,   -1,  359,   59,   -1,   61,   -1,
  262,   58,  521,   -1,   -1,   -1,   63,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,  383,
   -1,  396,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,  521,  318,   -1,   -1,  321,  413,
   -1,   -1,   -1,   -1,   -1,  328,  318,   -1,  331,  321,
  435,   -1,  335,  262,   -1,  440,  328,  340,   -1,  331,
  343,  435,   -1,  335,   -1,   -1,  440,  452,  340,  454,
   40,  343,   -1,   43,   -1,   45,  359,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,   -1,  359,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,  318,
   -1,  383,  321,  396,  386,  387,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  396,   -1,  335,   -1,   -1,   -1,
  413,  340,   -1,   -1,  343,   -1,  521,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  521,   -1,   -1,
  359,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,  452,
   -1,  454,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
  452,   -1,  454,   -1,   -1,  258,  434,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,  413,  278,   -1,  280,   -1,  276,
   -1,  284,  460,   -1,  287,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,  435,  300,  295,  296,
   -1,  440,   -1,  306,   -1,   -1,   -1,   -1,  521,   -1,
   -1,  489,  490,  452,  492,  454,   -1,  314,   -1,  521,
   -1,  499,  500,  501,  502,  503,  323,  505,   -1,  326,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,  522,  523,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,  366,
   -1,   -1,   -1,  370,   -1,  378,  276,   -1,   -1,   -1,
   -1,  378,  521,   -1,   -1,   -1,   -1,   -1,   40,  289,
   -1,   43,  292,   45,   -1,  295,  296,  400,   -1,   -1,
   -1,   -1,   -1,   -1,  407,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  314,  418,   -1,   -1,   -1,   -1,
  417,   -1,   -1,  323,  421,   -1,  326,   -1,   -1,   -1,
  427,   -1,   -1,  436,   -1,   -1,  433,   -1,  441,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,  366,  470,   -1,   -1,
  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,  378,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,  513,  514,  515,  516,
  517,  421,   -1,   -1,   -1,  522,  523,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,  287,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  267,  516,  517,   -1,   -1,
   -1,   -1,  522,  523,  276,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,   62,
  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,  273,   -1,  366,   -1,  277,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   40,   -1,  378,   43,   -1,   45,
  291,   -1,   -1,   -1,  295,   -1,   -1,  298,  489,  490,
   -1,  492,   58,   -1,   -1,   -1,   -1,   63,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,  421,
   -1,  522,  523,   -1,   -1,  427,   -1,   -1,  339,   -1,
   -1,  433,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,  443,  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,  406,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,  423,  276,  516,  517,   -1,   -1,   -1,   -1,
  522,  523,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
  323,   -1,   -1,  326,   -1,   -1,   -1,  280,   -1,  480,
  481,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  497,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  276,  324,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
  333,   -1,   -1,  289,   -1,   -1,   -1,   -1,  341,  295,
  296,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,  314,  362,
   58,   -1,   -1,   -1,   -1,   63,   -1,  323,  421,   -1,
  326,   -1,   -1,  376,  427,  378,  379,  380,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  378,   -1,   -1,   -1,  429,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  287,  516,  517,  421,   -1,   -1,   -1,  522,
  523,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  260,   -1,   -1,  263,  522,  523,   -1,  267,
   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  291,
   -1,  289,   -1,  295,   -1,   -1,  298,  295,  296,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   42,   43,   -1,   45,   58,   47,
   -1,   -1,   -1,   63,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   60,   61,   62,  460,   -1,  339,   -1,   -1,
   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  366,   -1,
   -1,  373,  370,  375,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  523,   -1,
   40,   -1,   -1,   43,  406,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,  423,   -1,   63,   -1,   -1,   -1,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  445,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,  480,  481,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,  497,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  260,   -1,   -1,  263,  522,  523,   -1,  267,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,  289,
   -1,   -1,  280,   -1,   -1,  295,  296,  285,   -1,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  333,   -1,   -1,   -1,   -1,
   -1,   -1,  352,  341,  354,  355,   -1,   -1,   -1,   -1,
  260,   -1,   -1,  263,   -1,   -1,  366,  267,   -1,   -1,
  370,   -1,  360,   -1,  362,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  289,
  378,  379,  380,   -1,   -1,  295,  296,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,  323,   -1,  425,  326,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  429,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,  260,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  522,  523,   -1,  425,  258,  427,   -1,   -1,
   -1,   -1,  301,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  313,  277,  278,   -1,  280,   -1,
   -1,   -1,  284,   -1,   -1,  287,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,  300,  338,
   -1,  471,   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  260,   -1,
   -1,  263,  522,  523,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,  407,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  378,  289,  417,   -1,
   -1,   -1,   -1,  295,  296,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,
   -1,   -1,   -1,  442,   58,  407,   -1,   -1,   -1,   63,
   -1,  323,   -1,   -1,  326,   -1,  418,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,  366,   -1,   -1,   -1,  370,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  470,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,  522,  523,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,  260,   -1,   -1,  263,
  522,  523,   -1,  267,   -1,  273,   -1,   -1,   -1,  277,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,  295,   -1,   -1,
  298,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,  295,   -1,   -1,  298,   -1,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,
   -1,   -1,  366,  267,   -1,  373,  370,  375,  339,   -1,
   -1,   -1,  276,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   40,   -1,   -1,   43,  406,   45,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,  423,   -1,   63,   -1,  323,
   -1,  425,  326,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,  406,   -1,  445,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,
  354,  355,  423,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,  471,   -1,   -1,
   -1,   -1,  480,  481,  445,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,  497,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,  480,
  481,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  523,
   -1,  425,   -1,  427,   -1,   -1,  497,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  260,   -1,   -1,  263,  522,  523,
   -1,  267,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  489,  490,   -1,  492,   63,   -1,  323,   -1,   -1,
  326,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,  522,  523,  352,   -1,  354,  355,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  328,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,  438,
  516,  517,  260,   -1,  443,  263,  522,  523,   -1,  267,
  449,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  468,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,  323,  505,   -1,  326,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,  522,  523,   -1,   -1,   -1,  260,   -1,
   -1,   -1,   -1,   -1,  352,  267,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   40,   -1,   42,
   43,  303,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,  427,   -1,   -1,  516,  517,
   -1,  433,   -1,   -1,  522,  523,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  343,   -1,  516,  517,   -1,  260,   -1,   -1,
  522,  523,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,  460,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   40,  295,  296,   43,   -1,   45,   -1,   -1,   -1,
  303,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   58,
   -1,   -1,   -1,   -1,   63,  499,  500,  501,  502,  503,
  323,  505,   -1,  326,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,  426,   -1,   -1,  522,  523,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,  460,
   -1,  273,   -1,   -1,   -1,  277,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  293,   -1,  295,   -1,   58,  298,   -1,  489,  490,
   63,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  427,  516,  517,   -1,   -1,   -1,
  433,  522,  523,   -1,   -1,   -1,   -1,  339,   -1,   -1,
  443,   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,  373,   -1,  375,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  406,  508,  509,  510,  511,  512,
   -1,  260,   -1,  516,  517,   -1,   -1,   -1,  267,  522,
  523,  423,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,  445,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,  480,  481,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  497,   -1,   -1,   -1,  460,
   -1,   -1,   -1,  352,  267,  354,  355,   40,   -1,   -1,
   43,   -1,   45,  276,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   58,  289,   -1,  489,  490,
   63,  492,  295,  296,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
  323,  522,  523,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,  427,   -1,   -1,  516,  517,   -1,
  433,   -1,   -1,  522,  523,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,  267,  498,  499,  500,  501,  502,
  503,  504,  505,  276,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,  289,   -1,   -1,  522,
  523,   40,  295,  296,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,  460,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   58,  492,   -1,   -1,   -1,
   63,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,  427,   -1,  522,  523,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,  267,  522,
  523,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  352,  267,  354,  355,   40,   -1,   -1,
   43,   -1,   45,  276,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   58,  289,   -1,   -1,   -1,
   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,  417,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,  427,   -1,   -1,  516,  517,   -1,
  433,   -1,   -1,  522,  523,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,  267,  498,  499,  500,  501,  502,
  503,  504,  505,  276,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,  289,   -1,   -1,  522,
  523,   40,  295,  296,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,  267,  522,
  523,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  352,  267,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   40,  289,   -1,   43,   -1,
   45,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   59,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,  417,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,  427,   -1,   -1,  516,  517,   -1,
  433,   -1,   -1,  522,  523,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  267,  516,  517,   -1,   -1,   -1,   -1,  522,
  523,  276,   -1,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  276,  516,  517,   -1,   -1,   -1,   -1,  522,  523,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,  276,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,  523,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  276,  516,  517,   -1,
   -1,   -1,   -1,  522,  523,   -1,   -1,   -1,   40,  289,
   -1,   43,   -1,   45,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   61,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   91,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  267,  516,  517,   -1,   -1,
   -1,   -1,  522,  523,  276,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,   -1,
   44,  323,    0,   -1,  326,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,    0,
  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,  366,   -1,   44,   91,  370,  258,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,  274,   -1,   -1,  277,  278,
   41,  280,   -1,   44,   -1,  284,   -1,   -1,  287,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,  300,   -1,   91,   -1,   -1,   -1,  306,   -1,   -1,
  309,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,  378,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,  400,   -1,   -1,  516,  517,   -1,   -1,  407,   -1,
  522,  523,  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,
   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  258,   -1,   -1,   -1,   -1,  436,   -1,   -1,
  274,    0,  441,  277,  278,   -1,  280,   -1,  274,   -1,
  284,  277,  278,  287,  280,   -1,   -1,   -1,  284,   -1,
   -1,  287,   -1,   -1,   -1,   -1,  300,  466,   -1,   -1,
  258,  470,  306,   -1,  300,  309,   -1,   -1,   -1,   -1,
  306,   -1,   41,  309,   -1,   44,  274,   -1,   -1,   -1,
  278,   -1,  280,   -1,   -1,   -1,  284,  258,   -1,  287,
   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  300,  274,   -1,   -1,  260,  278,  306,  280,
   -1,  309,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,
   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,  300,
   -1,   -1,   -1,   -1,  378,  306,   -1,   -1,  309,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,  301,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,   -1,  313,
   -1,   -1,   -1,  407,  400,   -1,   -1,  411,   -1,   -1,
   -1,  407,   -1,  327,  418,  411,   -1,   -1,   -1,   -1,
  378,   -1,  418,   -1,  338,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,
  436,   -1,  400,   -1,   -1,  441,   -1,  378,   -1,  407,
   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,
  418,   -1,  466,   -1,   -1,   -1,  470,   -1,   -1,  400,
  466,   -1,   -1,   -1,  470,   -1,  407,   -1,  436,   -1,
  411,   -1,   -1,  441,   -1,   -1,   -1,  418,   -1,   -1,
   -1,   -1,   -1,  407,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  436,   -1,   -1,  466,   -1,
  441,   -1,  470,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,  258,
   -1,   -1,   -1,   -1,   -1,  466,   -1,   -1,   -1,  470,
   -1,   -1,   -1,   -1,   -1,  274,  460,   -1,   -1,  278,
   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  300,   -1,   -1,   -1,  489,  490,  306,  492,   -1,
  309,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,  301,  508,  509,  510,  511,  512,   -1,
  308,   -1,  516,  517,  312,  313,   -1,  315,  522,  523,
  318,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  332,   -1,   -1,   -1,   -1,   -1,
  338,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  378,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,
   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,
  398,  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  312,  313,   -1,  315,   -1,  436,   -1,  417,
   -1,   -1,  441,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  332,   -1,  432,   -1,   -1,   -1,  338,   -1,
   -1,   -1,   -1,   -1,  442,   -1,   -1,  466,   -1,   -1,
   -1,  470,   -1,  451,   -1,  453,  301,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,   -1,  312,  313,   -1,
  315,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  332,   -1,   -1,
   -1,  489,  490,  338,  492,   -1,   -1,   -1,  398,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  417,  516,  517,
  518,  519,  520,   -1,  522,  523,   -1,   -1,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,   -1,  398,   -1,   -1,   -1,   -1,   -1,  273,
  460,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,
  284,   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  295,   -1,   -1,  298,   -1,   -1,  432,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  442,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  460,  516,  517,  518,  519,
  520,   -1,  522,  523,   -1,  339,   -1,   -1,   -1,   -1,
   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,  353,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,  373,
  505,  375,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  518,  519,  520,   -1,  522,  523,  273,
   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,  423,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  339,  460,   -1,   -1,   -1,
   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,  353,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  373,
   -1,  375,   -1,  497,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  523,
   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  423,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  460,  516,  517,   -1,   -1,   -1,   -1,  522,  523,
  469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,   -1,   -1,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,   -1,
  499,  500,  501,  502,  503,  504,  505,  506,   -1,  508,
  509,  510,  511,  512,   -1,  460,   -1,  516,  517,  518,
  519,  520,  521,  522,  523,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,   -1,  499,  500,  501,  502,  503,  504,
  505,  506,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  518,  519,  520,  521,  522,  523,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 523
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
"ROWS","BLOCK","IIF",
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
"case_abbreviation : IIF '(' search_condition ',' value ',' value ')'",
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
"non_reserved_word : IIF",
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
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
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
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 962:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 963:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 964:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 965:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 966:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 967:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 968:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
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
