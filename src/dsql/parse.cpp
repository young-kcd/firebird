#ifndef lint
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)
#define YYEMPTY -1
#define yyclearin (DSQL_yychar=(YYEMPTY))
#define yyerrok (DSQL_yyerrflag=0)
#define YYRECOVERING() (DSQL_yyerrflag!=0)
static int yygrowstack();
#define YYPREFIX "yy"
#line 2 "parse.y"
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
#include "../jrd/y_ref.h"
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
#include "../jrd/thd_proto.h"
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
#define YYSTACK_USE_ALLOCA 1

#define YYSTYPE  dsql_nod*
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

#line 232 "parse.cpp"
#define YYERRCODE 256
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
const short yylhs[] = {                                        -1,
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
  229,  229,  230,  230,  340,  340,  341,  341,  343,  343,
  333,  345,  345,  346,  346,  346,  349,  350,  350,  351,
  351,  353,  353,  347,  347,  348,  348,  354,  354,  238,
  238,   32,  239,  239,  239,  239,  239,  239,  239,  239,
  233,  233,  355,  355,  356,  234,  234,  232,  232,  334,
  334,  357,  358,  358,  358,  358,  358,  359,  359,  360,
  360,  361,  361,  362,  362,  362,  363,  363,  364,  364,
   10,   10,    7,    7,  366,  367,  368,   20,   20,  369,
  370,  371,  371,  187,  342,  342,  209,  209,    3,    3,
  373,  373,  373,  375,  375,   62,  374,  374,  376,  376,
   35,   35,   86,   85,   85,  365,  365,  377,  378,  378,
  210,  210,  210,  137,  372,  372,  112,  112,  112,  112,
  112,  382,  382,  382,  382,  382,  382,  381,  381,  381,
  384,  384,  384,  384,  384,  384,  384,  384,  384,  384,
  385,  385,  385,  385,  385,  385,  385,  385,  390,  390,
  390,  390,  390,  390,  390,  390,  390,  390,  390,  390,
  390,  390,  390,  390,  396,  396,  386,  386,  387,  387,
  387,  387,  388,  388,  392,  392,  393,  393,  393,  393,
  391,  394,  389,  389,  380,  380,  380,  383,  383,  383,
  397,  397,  398,  395,  400,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,  147,  147,  147,  401,  399,  399,  142,  142,  407,
  407,  407,  407,  407,  403,  403,  403,  403,  403,  223,
  143,  143,  144,  145,  145,  145,  145,  145,   52,   52,
   21,   21,  246,  408,   55,  283,  277,  277,   96,  402,
  402,  402,  402,  409,  409,  409,  409,  409,  409,  409,
  409,  409,  409,  409,  410,  411,  413,  412,  412,  415,
  416,  416,  406,  406,  404,  405,  405,  417,  417,  418,
  418,  419,  419,  422,  422,  420,  420,  425,  425,  424,
  421,  423,  414,  414,  414,  414,  414,  414,  414,  414,
  339,  339,  146,   49,  284,  121,  139,  379,  156,  180,
  270,   64,   61,  215,  113,   67,  344,  211,   36,   42,
  352,  269,   40,   43,  208,   41,  297,  264,  264,  426,
  426,  426,  426,  426,  426,  426,  426,  426,  426,  426,
  426,  426,  426,  426,  426,  426,
};
const short yylen[] = {                                         2,
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
    5,    1,    3,    3,    1,    1,    1,    3,    7,    7,
    5,    3,    0,    1,    1,    1,    2,    0,    1,    1,
    1,    0,    3,    1,    3,    1,    0,    3,    1,    3,
    1,    3,    3,    1,    1,    3,    1,    2,    1,    3,
    3,    1,    2,    3,    3,    3,    3,    1,    3,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    3,    3,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    1,    1,    5,    6,    3,    4,
    5,    6,    3,    4,    3,    4,    3,    4,    4,    5,
    4,    4,    3,    4,    1,    1,    1,    1,    1,    1,
    1,    3,    3,   11,   11,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    2,    3,    3,    3,    3,
    3,    3,    3,    3,    1,    1,    1,    1,    3,    1,
    1,    1,    1,    1,    4,    1,    3,    1,    2,    1,
    1,    1,    1,    1,    1,    1,    2,    2,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    1,    2,    1,    1,    1,    1,    1,    2,    1,    1,
    1,    1,    1,    4,    5,    5,    5,    5,    5,    5,
    5,    5,    5,    5,    6,    1,    6,    1,    4,    7,
    2,    0,    4,    3,    6,    1,    1,    6,    6,    1,
    1,    4,    6,    4,    5,    3,    5,    4,    5,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,
};
const short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
  558,  559,  560,  564,  565,  566,    0,  621,  733,  734,
  738,  739,  471,    0,    0,    0,    0,    0,    0,  405,
  576,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,    0,    0,   96,    0,  913,    0,   22,  911,
    0,    0,   74,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,    0,    0,    0,
   38,   35,   37,    0,   36,    0,  998, 1000, 1002, 1001,
 1007, 1003, 1010, 1008, 1004, 1006, 1009, 1005, 1011, 1012,
 1013, 1015, 1016, 1014,    0,    0,    0,    0,   33,   69,
   67,  990,  999,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  653,    0,    0,    0,
  690,    0,  992,    0,    0,    0,    0,  108,  997,  567,
    0,    2,    0,    0,    0,    0,  435,  436,  437,  438,
  439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
  459,  460,    0,  433,  434,    0,  982,    0,  412,  986,
    0,  409,  989,    0,  692,    0,  408,  993,    0,  574,
  909,    0,  151,  104,    0,  105,    0,  201,  978,  774,
    0,  103,  146,  985,    0,    0,   99,    0,  106,  915,
    0,  100,    0,  101,    0,  102,  996,  147,  107,  629,
  630,    0,  914,  912,    0,   75,    0,  983,    0,  981,
  492,  485,    0,  491,  496,  486,  487,  494,  488,  489,
  490,  495,    0,    0,    0,   31,    0,   40,  761,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  919,    0,  900,  647,  647,
  648,  658,  660,  971,  661,    0,  662,  652,  648,    0,
    0,    0,    0,    0,    0,    0,  593,  582,    0,  585,
  587,  588,  589,  590,  596,  597,    0,  991,  691,    0,
  109,    0,  110,    0,  111,    0,    0,    0,  622,    0,
    0,    0,    0,    0,    0,    0,  472,    0,    0,  375,
    0,  204,    0,  415,  406,  469,  470,    0,    0,    0,
    0,    0,    0,  424,    0,  381,  382,    0,  580,  578,
  910,    0,    0,    0,    0,    0,    0,    0,  154,  133,
    0,   97,    0,    0,  112,    0,  122,  121,    0,    0,
    0,    0,    0,    0,  974,   76,    0,    0,    0,  735,
  736,  493,    0,    0,    0,    0,  878,  906,    0,  973,
    0,  880,    0,    0,  907,    0,  901,  892,  891,  890,
    0,    0,    0,    0,  882,  883,  884,  893,  894,  902,
  903,    0,  904,  905,    0,    0,    0,  908,    0,    0,
    0,    0,    0,  896,    0,  771,  875,  876,  877,  746,
  881,  352,  861,    0,    0,  860,    0,  747,    0,  857,
  858,  859,  862,  863,  864,  895,  920,  921,  922,  923,
  936,  938,  946,  947,  950,  951,  202,    0,    0,    0,
  356,    0,    0,    0,  764,    0,    0,   28,    0,    0,
    0,   72,   70,  994,   68,   34,    0,    0,    0,  766,
    0,   53,    0,    0,    0,    0,    0,  572,    0,    0,
    0,  655,  657,    0,  664,    0,    0,  665,    0,  651,
    0,  619,    0,  594,    0,  591,    0,  592,  617,    0,
  612,    0,    0,  586,  775,  742,    0,    0,    0,    0,
    0,    0,  569,  568,    0,    0,  626,  623,    0,    0,
    0,  169,    0,    0,  130,    0,  477,    0,  478,  473,
    0,    0,  419,  421,  420,    0,  422,    0,  416,    0,
    0,  255,    0,    0,    0,  428,  429,    0,  462,    0,
    0,    0,    0,    0,    0,    0,  386,  385,  480,    0,
    0,  150,    0,    0,  159,    0,  158,    0,    0,  165,
  148,    0,  163,  155,    0,    0,  515,  550,  518,    0,
  551,    0,    0,  519,    0,    0,    0,  542,  554,  514,
  537,  516,  517,  512,  499,  375,  497,  498,  500,    0,
    0,  509,  510,  511,  513,    0,    0,    0,    0,    0,
  113,    0,  114,    0,  125,    0,    0,  185,    0,    0,
  373,    0,  204,    0,    0,   84,    0,    0,  756,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  777,  779,  788,  791,  792,  793,  794,  795,  796,  797,
  798,  799,  800,    0,    0,    0,  897,    0,    0,    0,
    0,    0,    0,  898,  899,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  317,  995,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  260,  507,    0,    0,    0,  354,    0,    0,    0,  763,
   29,    0,    0,   45,   73,    0,    0,   27,    0,  769,
    0,    0,    0,  732,    0,    0,    0,    0,    0,  571,
    0,  647,    0,    0,    0,    0,  669,    0,  562,    0,
  917,  561,    0,  595,    0,  603,  599,  598,    0,    0,
    0,  614,  602,    0,    0,  740,  741,    0,    0,    0,
  373,    0,    0,    0,    0,    0,  620,    0,  131,    0,
  476,  375,  418,    0,  375,  376,  423,    0,    0,  251,
  373,  224,  979,    0,    0,  375,    0,    0,  193,  373,
    0,    0,    0,    0,  229,  223,  226,  227,  228,  427,
  467,  466,  426,  425,    0,    0,    0,    0,    0,    0,
  384,    0,  375,    0,  156,  160,    0,    0,  164,    0,
    0,  538,  539,  555,    0,  552,    0,    0,  545,  543,
  544,    0,    0,    0,    0,  508,    0,    0,    0,    0,
  546,    0,    0,  373,    0,  184,    0,  187,  189,  190,
    0,    0,    0,    0,    0,  204,   79,    0,    0,   78,
  526,  975,  527,    0,    0,    0,    0,  778,  790,    0,
    0,  848,  849,  850,    0,  782,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  963,
  964,  965,  966,  967,  968,  969,  970,    0,    0,    0,
    0,    0,    0,  956,    0,    0,    0,    0,  873,  319,
  874,  944,    0,    0,  869,  977,    0,    0,    0,    0,
    0,  748,    0,  879,  773,  772,  358,  357,    0,  258,
    0,    0,    0,  266,  765,    0,    0,    0,    0,    0,
    0,    0,    0,   54,   62,   64,    0,    0,   71,    0,
    0,    0,  768,    0,    0,    0,    0,    0,    0,    0,
    0,  656,  659,    0,    0,    0,    0,  674,  675,  676,
    0,    0,  666,  987,  668,  563,  918,  604,  613,  608,
  607,    0,  618,  776,  743,  744,  373,    0,    0,  649,
  628,  627,    0,    0,  639,  643,  475,    0,    0,  168,
    0,  172,    0,    0,    0,  417,  373,  373,  263,  376,
  256,    0,  195,  375,  197,    0,  376,  192,  198,    0,
    0,    0,    0,  430,  431,  375,  373,  376,    0,    0,
    0,  402,    0,  180,  181,  157,    0,  167,  916,    0,
    0,    0,    0,    0,  553,    0,    0,  375,  376,    0,
  503,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  182,    0,    0,    0,    0,  362,
    0,   83,    0,    0,    0,    0,   88,   85,    0,    0,
    0,  737,  980,    0,    0,  783,    0,    0,  789,    0,
    0,    0,    0,    0,  826,  825,    0,    0,    0,  833,
  851,  843,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  781,    0,    0,    0,  204,    0,  924,    0,
    0,  939,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  952,    0,  962,    0,    0,    0,
    0,    0,    0,  943,  885,    0,  253,  272,    0,    0,
  353,  267,   66,   58,   59,   65,   60,   61,    0,    0,
   23,    0,    0,    0,   43,    0,    0,    0,  750,  770,
    0,    0,  749,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  693,    0,    0,    0,    0,    0,
  610,  611,  615,    0,  183,    0,    0,    0,    0,    0,
    0,  173,    0,    0,    0,  413,  207,  208,  209,  210,
  211,  212,  888,    0,    0,  257,    0,    0,  205,    0,
    0,    0,    0,  230,    0,  463,  464,  432,    0,    0,
    0,    0,    0,    0,  375,  482,  376,  166,  976,    0,
    0,  521,    0,  520,  556,    0,  548,    0,    0,    0,
  501,    0,  529,    0,    0,  531,  534,  536,  373,    0,
    0,    0,  120,    0,  188,  375,    0,    0,  374,    0,
   98,    0,  118,    0,   94,    0,    0,    0,    0,   87,
    0,  841,  842,  785,    0,  787,    0,    0,    0,    0,
    0,    0,    0,  844,    0,    0,    0,    0,    0,    0,
    0,  834,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  930,  929,    0,    0,
  926,  925,    0,  934,  933,  932,  931,  928,  927,    0,
    0,  958,    0,    0,    0,  957,    0,    0,    0,    0,
    0,  259,    0,    0,  269,  270,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  351,
    0,    0,    0,    0,  278,  283,  277,    0,    0,    0,
  287,  288,  289,  290,  291,  292,  294,  295,  297,  298,
  299,  302,    0,    0,    0,  345,  346,  347,    0,    0,
   57,   63,   55,   56,   24,  754,  755,    0,  752,  760,
  759,  757,    0,  731,    0,    0,    0,  685,    0,    0,
    0,  699,  695,  697,    0,    0,    0,    0,    0,  374,
  632,  633,  650,    0,  178,  174,    0,  376,  889,  261,
  262,  376,    0,    0,    0,  215,    0,    0,  231,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  479,  523,
    0,  524,    0,  135,    0,    0,    0,  139,  141,  142,
  143,  504,  506,  502,  533,    0,    0,  376,    0,    0,
    0,    0,  359,    0,  363,  375,   89,    0,    0,   91,
    0,    0,    0,    0,    0,    0,  853,  852,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  203,  945,  935,  937,
    0,    0,  954,  953,    0,  959,  948,  949,    0,    0,
  268,    0,  306,    0,    0,    0,  301,    0,    0,  300,
  331,  332,    0,  988,  348,  349,    0,  286,  293,    0,
  296,  279,    0,  284,    0,  336,  285,  303,    0,    0,
  329,   41,    0,    0,    0,  688,  686,    0,    0,    0,
  703,    0,    0,  645,  376,  360,  634,  175,  376,  145,
  250,    0,    0,  191,  216,    0,    0,    0,    0,  217,
  220,    0,    0,  376,  400,  401,  397,  396,  399,  398,
    0,  522,  549,  144,  132,  140,    0,  375,  247,  127,
  376,    0,    0,  377,  364,    0,    0,   92,   93,   80,
    0,  812,  820,    0,  813,  821,  816,  824,    0,    0,
  814,  822,  815,  823,  809,  817,  810,  818,  811,  819,
    0,  940,  955,    0,    0,  275,  271,    0,  304,    0,
    0,    0,    0,    0,  333,    0,    0,  324,  325,    0,
  344,    0,    0,    0,    0,  339,  337,    0,    0,    0,
  751,    0,    0,  678,    0,    0,  713,  715,    0,  710,
    0,  248,  414,  194,  199,  218,    0,    0,  221,    0,
  233,  235,  404,  376,    0,  379,    0,    0,    0,  369,
  370,    0,    0,   77,   95,    0,    0,  274,    0,  305,
    0,    0,  308,    0,  314,    0,  316,    0,    0,  343,
  342,  984,  341,    0,    0,    0,    0,    0,    0,    0,
  677,    0,  704,  714,    0,  222,    0,    0,    0,    0,
    0,  249,  376,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  327,  326,  338,
  340,    0,    0,    0,    0,  682,    0,    0,  721,    0,
    0,    0,    0,    0,    0,  232,    0,  239,    0,  238,
  380,  372,    0,    0,  365,  119,    0,    0,  276,    0,
    0,  307,  311,    0,    0,    0,    0,    0,    0,    0,
  680,  723,  712,    0,    0,  724,    0,  720,  219,    0,
    0,  243,  242,  241,    0,    0,    0,    0,    0,    0,
    0,    0,  328,    0,  683,  719,    0,    0,  246,  244,
  245,    0,    0,  855,  322,  312,    0,  334,  309,  646,
    0,    0,    0,  726,    0,    0,  310,    0,  725,    0,
    0,  728,    0,  854,  729,
};
const short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1411,   28,   29, 1412,
   31,   32,   33,   34,   35,   36,   37, 1413,   39, 1414,
  649,  125,  477,  972, 1211,  126,  723,  973,  127,  481,
  728,  151,  128,  129,  268,  995,  141,  974,  975,  206,
  235,  130,  976,  131,  483,   83,  246,  386,  433,  644,
 1105,  434,  869,  615,  228,  846,  645,  646,  647, 1107,
  247, 1446,   75,  196,   76,  242, 1881, 1321,  227,  232,
  234,  236,  222,  214,  216,  229,  239,  158,  321,  323,
  325,  375,  631,  633,  474,  269,  795,  552,  659, 1036,
  379,  636, 1093, 1313,  583,  751, 1066,  544,  545,  565,
  371,  616, 1079, 1506, 1624, 1256, 1507, 1508, 1509, 1510,
 1511,  660,  223,  215,  367,  591,  368,  369,  592,  593,
 1288,  546, 1030, 1031, 1032, 1251,  381,  857,  858,  859,
  860,  797, 1047, 1484,  617,  799,  436,  862,  945,  468,
 1370, 1257,  437,  438,  439,  440,  441, 1485, 1486,  568,
 1630, 1779, 1274,  805,  806,  792,  807,  808,  809, 1780,
 1781, 1853,  349,  473,  962, 1415,  202,  561,  959,  562,
 1040,  469,  963,  964, 1199, 1394, 1395, 1396, 1687, 1112,
  734, 1416, 1417, 1418, 1419, 1595, 1420, 1421, 1422, 1423,
 1424, 1425, 1426, 1427, 1428, 1429, 1430, 1431, 1432, 1433,
  442, 1794, 1434, 1697, 1837, 1694,  443,  699,  444,  445,
 1435, 1596, 1705, 1706, 1761, 1436, 1437, 1438, 1587,  265,
  470,  471,  446, 1098, 1319, 1523, 1099, 1100,  296,  506,
 1653,  390, 1240, 1468, 1738, 1739, 1740, 1741, 1238,  358,
  579,  823, 1286,  580,  821,   80,   60,  353,  207,  166,
  336,  193,  343,  199,  786,  783,  344,  354,  813,  355,
  575, 1058,  194,  447,  337,  581, 1287,   97,  205,  251,
  618,  619,  712, 1080,  713, 1081, 1082,  622,  623,  624,
  625, 1073, 1071,  873,  626,  627,  628,  839,  629,  836,
   41,   42,   43,   44,   45,   46,  160,  534,   62,  499,
  210,  360,  308,  309,  310,  311,  312,  313,  314,  315,
  316,  757,  520, 1012, 1243,  521,  522,  762,   47,  165,
  331,  541,  777,   48,  536,  537, 1021, 1473, 1247, 1617,
 1025,  145,  745, 1614,  502,  298,  146,  147,  297,  507,
  508,  448,  748, 1005,  996,  997,  998,  999, 1000, 1713,
 1771,  449, 1807, 1232, 1610, 1611, 1809, 1721, 1810, 1811,
 1812, 1848, 1882, 1884,  489,   49,   50,  391,   51,   52,
  528,  529,  981, 1219, 1448, 1452,  490,  731,  220,  661,
  662,  887,  888,  663,  664,  665,  666,  667,  668,  669,
  670,  671,  672,  673, 1342, 1128, 1130, 1131,  944,  698,
  450,  451,  452,  453,  454,  455,  456,  244,  457,  458,
  459,  460,  461,  928,  462, 1562,  463,  464,  465,  466,
  689,  932, 1188, 1183,  690,  133,
};
const short yysindex[] = {                                   3263,
 1235,  -78, 3439,  148, 1041,  201, 3545, -119,10358,  808,
  508, 9799,  -78,  954,  895, 9949,  -67, 9949, -105,    0,
  841,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  861,    0,    0,    0,
    0,    0,    0,12066, 9949, 9949, 9949, 9949, 9949,    0,
    0,  573, 1579, 9949, 9949, 9949,  660, 9949,  593, 9949,
 9949,    0, 9949, 9949,    0, 1192,    0,  641,    0,    0,
  988, 9949,    0, 9949, 9949, 9949, 1027, 9949, 9949, 9949,
 9949,  593, 9949, 9949, 9949, 9949,    0, 9949, 1145, 1017,
    0,    0,    0, 1328,    0, 1328,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1060, 1070,  212, 1418,    0,    0,
    0,    0,    0, 9949, 9949, 9949, 1084, 1105, 1143,  193,
 2351, 1123,  162,  205, 1019, 1101,    0, 9949, 1230, 1790,
    0, 1222,    0, 9949, 9949, 9949, 9949,    0,    0,    0,
 9949,    0, 1371,  -17, 1135, 1687,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  872,    0,    0, 1579,    0,  759,    0,    0,
 1621,    0,    0, 1243,    0,  892,    0,    0, 1241,    0,
    0, 1273,    0,    0, 1142,    0, 1406,    0,    0,    0,
 1579,    0,    0,    0,  -55, 1621,    0,  584,    0,    0,
 1424,    0, 1400,    0, 1328,    0,    0,    0,    0,    0,
    0, 1410,    0,    0, 1282,    0, 1420,    0, 1339,    0,
    0,    0, 1282,    0,    0,    0,    0,    0,    0,    0,
    0,    0,10705, 9949, 1412,    0, 9949,    0,    0,    0,
 7909, 1407,10395, 9949, 1068, 1473, 1777, 1528, 1526, 7909,
 1454,10395, 1481, 1488, 1354,    0,12385,    0,    0,    0,
    0,    0,    0,    0,    0, 9902,    0,    0,    0, 1447,
 9949, 1531, 1437,  665, 9949, 1469,    0,    0, 1790,    0,
    0,    0,    0,    0,    0,    0, 9949,    0,    0, 1621,
    0, 1424,    0, 1328,    0, 1538,12385, 1494,    0,12385,
 1624,   11, 1432, 1438, 1434, 1687,    0, 1685,   81,    0,
 9949,    0,  872,    0,    0,    0,    0, 9949, 1412, 7013,
 1509, 7421, 1936,    0,12066,    0,    0,  639,    0,    0,
    0, 1928, 1928, 1579, 1625, 1579,  502, 1142,    0,    0,
 9561,    0, 9949, 9949,    0, 1412,    0,    0, 1717,  453,
 1965, 9949, 1745, 9949,    0,    0, 1967,  267, 7194,    0,
    0,    0, 1980, 1996, 2005, 1610,    0,    0, 2027,    0,
 2033,    0, 2046, 2048,    0, 2079,    0,    0,    0,    0,
    0, 2094, 1620, 1659,    0,    0,    0,    0,    0,    0,
    0, 2095,    0,    0,10791, 2096, 2098,    0,12385,12385,
10141, 9949, 2099,    0, 2114,    0,    0,    0,    0,    0,
    0,    0,    0, 2056, 2062,    0,    0,    0, 2101,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 9561, 2103, 1668,
    0, 2122, 1901, 1734,    0, 9949, 1732,    0, 9949, 9949,
  105,    0,    0,    0,    0,    0, 9949, 9949,  832,    0,
 9949,    0, 1846, 9949, 2130, 7909, 1773,    0, 9949,11021,
 1743,    0,    0,12385,    0, 1866, 2143,    0, 1929,    0,
  159,    0,  392,    0,  557,    0,  557,    0,    0, 2144,
    0,  143, 1768,    0,    0,    0, 2151,  106, 2140, 1412,
 2166, 1948,    0,    0, 1831, 2171,    0,    0,  460, 1774,
 1764,    0, 1869, 1871,    0, 1579,    0, 1874,    0,    0,
 1950, 1953,    0,    0,    0, 1927,    0, 9561,    0, 9561,
 1863,    0, 1973, 9949,13104,    0,    0,  858,    0, 9949,
  412, 1243, 1841, 1806, 1775,    0,    0,    0,    0,  -48,
 1849,    0, 1791,  593,    0, 1579,    0, 1976, 1900,    0,
    0,  502,    0,    0,  266, 1803,    0,    0,    0, 1807,
    0, 1856, 2217,    0, 1909, 2220,   59,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2173,
  139,    0,    0,    0,    0,  118, 2222, 2229, 2220, 1621,
    0, 1954,    0, 2007,    0, 1579, 1579,    0, 7013,  892,
    0, 1891,    0, 1864, 2233,    0, 8784, 4307,    0, 1895,
 1903, 2247, 7763, 2250,    0,    0,    0, 6940, 8106,   29,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1317,11269,  634,    0, 9949,12385, 1365,
 1392, 1479, 2609,    0,    0,12385, 7849, 2114, 1843,  631,
12385,11269,  811,  811,  954, 1878, 1868, 2256,    0,    0,
11349, 9949,12385,12385,12385,12385,12385,11269,12385, 3627,
    0,    0, 2021, 2237, 9949,    0, 9949, 2004, 9949,    0,
    0,10040, 1872,    0,    0, 1845,10395,    0, 1988,    0,
 1890,  954, 2267,    0, 1988,10040, 1990, 1993, 9949,    0,
 1878,    0, 1949, 2645, 1867,11269,    0, 9949,    0,  -89,
    0,    0,  -13,    0, 1870,    0,    0,    0, 9949,  977,
 9949,    0,    0, 9949, 9949,    0,    0,11269, 2051, 7013,
    0, 1820,12385,12385, 1939, 1823,    0, 1579,    0,  -42,
    0,    0,    0, 2296,    0,    0,    0,  149, 9949,    0,
    0,    0,    0, 2067, 2302,    0, 2043, 2061,    0,    0,
 2000, 2011, 1328, 2070,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  593, 9949,13278, 1966, 1970, 1974,
    0,  593,    0, 1471,    0,    0, 1941, 1579,    0,  152,
 1946,    0,    0,    0,  593,    0, 2217,  154,    0,    0,
    0, 2063,  154, 1947,  154,    0, 2326,  593,  593,  593,
    0, 1412, 9949,    0, 2012,    0, 1892,    0,    0,    0,
  639, 1958, 9949, 1908,   80,    0,    0, 2337, 2104,    0,
    0,    0,    0,  267, 9949, 1962, 8079,    0,    0, 1962,
 7763,    0,    0,    0, 7802,    0,  110,  497,12385,12385,
 8337, 2340,  539, 8435,12385, 8693, 1071, 8951, 9049,11588,
 9307, 9565, 9663, 7849, 7849,12385,12385, 2121,12385, 2347,
12385, 2345, 1959,12385,12385,12385,12385,12385,12385,    0,
    0,    0,    0,    0,    0,    0,    0, 2071,  998,   13,
12385,  683,11269,    0, 7849, 1896, 2349, 1019,    0,    0,
    0,    0, 2114, 1920,    0,    0, 2115,  811,  811,  860,
  860,    0,  979,    0,    0,    0,    0,    0, 1924,    0,
 1952, 2125, 2004,    0,    0, 9949, 9949, 9949, 9949, 9949,
 9949,  171, 2357,    0,    0,    0,10040, 2017,    0,  -22,
 2037, 9949,    0, 1019,11269, 2037, 2361, 2357,10040, 8167,
 2088,    0,    0, 2082, 2369, 2366, 1403,    0,    0,    0,
 7849, 2085,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  650,    0,    0,    0,    0,    0, 1925, 1958,    0,
    0,    0, 2114, 9949,    0,    0,    0, 1928, 2146,    0,
  -42,    0, 2381, 7849, 6840,    0,    0,    0,    0,    0,
    0, 2004,    0,    0,    0, 6840,    0,    0,    0, 1328,
 1328, 1940, 2388,    0,    0,    0,    0,    0,  422,  504,
  723,    0, 2179,    0,    0,    0, 9949,    0,    0,  148,
 1932, 2029, 2021, 2389,    0, 1791, 1944,    0,    0, 1119,
    0, 2394, 9949, 1394,  593, 2414, 2415, 2417, 2198,  892,
 2004, 1928, 1871, 7013,    0, 1849, 1019, 2013, 2035,    0,
 1937,    0,  593, 2074, 2160, 8784,    0,    0,  593, 1985,
 2169,    0,    0, 2444, 2445,    0, 7849, 7849,    0, 7849,
 7849, 2576, 2114, 2449,    0,    0, 2114, 2455,11827,    0,
    0,    0, 2145, 2456, 2114, 2465, 1804, 2466, 2114, 2467,
12385,12385, 2340,12385,11907, 2472, 2114, 2474, 2475, 2114,
 2478,12385, 2114, 2479, 2114, 2480, 2481, 2114, 2483, 2486,
 2114, 2487,    0, 2266, 2008, 2016,    0, 2084,    0, 2107,
12385,    0, 2282, 2329, 2625, 2776, 2878, 2892,12385,12385,
11269, 2114, 2097,11269,    0,12385,    0, 2221,  628,12385,
11269,12385,12385,    0,    0, 9949,    0,    0, 9949,10925,
    0,    0,    0,    0,    0,    0,    0,    0, 2203,10040,
    0,10040,  171, 2357,    0,  372,  372,   72,    0,    0,
 9902, 2028,    0, 2361, 2357, 2489, 8167, 2494, 1403, 2495,
11269, 1929, 2645, 2149,    0, 2150, 2152, 2196, 2271, 2215,
    0,    0,    0, 2004,    0, 2013,  852, 2503, 1791, 2157,
 1791,    0, 7849,  666, 1580,    0,    0,    0,    0,    0,
    0,    0,    0, 6840, 6840,    0, 2125,12385,    0, 2263,
 2148, 1940, 1192,    0, 7849,    0,    0,    0, 2167, 2172,
 2175, 2180, 2184, 2191,    0,    0,    0,    0,    0, 2511,
  148,    0, 2128,    0,    0,  154,    0, 6840, 2181,  154,
    0,  154,    0, 2021, 2516,    0,    0,    0,    0,  639,
 2125, 1791,    0, 1871,    0,    0, 9902, 2301,    0,   16,
    0, 2503,    0, 2540,    0, 2547, 1579,  260, 2548,    0,
 1579,    0,    0,    0, 2266,    0, 2266,12385, 2183, 2183,
  954, 2555, 2034,    0, 2183, 2183,12385, 2183, 2183, 2787,
 2114,    0, 1812,12385, 2114, 2183, 2183, 2183, 2183, 2114,
 2183, 2183, 2183, 2183, 2183, 2183,    0,    0, 9561, 2556,
    0,    0, 2927,    0,    0,    0,    0,    0,    0, 3178,
 1701,    0,11269, 2293, 2168,    0,11269, 3536, 2041, 1606,
 2114,    0, 9561, 2546,    0,    0, 2313,    0, 4587,  463,
 2549, 2567, 2270,12385, 2553, 2554, 5828, 9949, 9949,    0,
 2557, 2561, 2275, 2566,    0,    0,    0, 2318,11489, 2568,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2569,  477, 2571,    0,    0,    0,    0, 2245,
    0,    0,    0,    0,    0,    0,    0, 2197,    0,    0,
    0,    0, 1866,    0, 8167, 2489, 1929,    0, 2054, 9949,
 1403,    0,    0,    0, 2645,12385, 7849, 2236, 2125,    0,
    0,    0,    0, 1471,    0,    0,  712,    0,    0,    0,
    0,    0, 2114, 2359, 2263,    0,  978, 9949,    0, 2303,
  755, 2199, 2304, 2201, 2343, 2308, 2346, 2004,    0,    0,
 2607,    0, 2608,    0, 2298, 2359, 2181,    0,    0,    0,
    0,    0,    0,    0,    0, 2004, 1849,    0, 1471, 2179,
 2331, 2272,    0, 1958,    0,    0,    0, 2285,  -71,    0,
 2021, 2288, 2114, 2619, 2621, 1019,    0,    0, 2628, 2630,
 2114, 2632, 2633,12385,12385, 2114, 2637, 2641, 2646, 2647,
 2651, 2654, 2655, 2657, 2658, 2659,    0,    0,    0,    0,
12385, 2664,    0,    0,11269,    0,    0,    0, 1867,  183,
    0, 2368,    0,12146, 9949,12385,    0, 7849, 2114,    0,
    0,    0, 2648,    0,    0,    0, 9949,    0,    0, 4067,
    0,    0,  580,    0, 2258,    0,    0,    0,  512, 2670,
    0,    0,  372, 1867, 9949,    0,    0, 1106, 2114, 2667,
    0,   29, 1009,    0,    0,    0,    0,    0,    0,    0,
    0, 2671, 9949,    0,    0, 2375, 2374, 9949, 1940,    0,
    0, 1328, 9949,    0,    0,    0,    0,    0,    0,    0,
 2125,    0,    0,    0,    0,    0, 2125,    0,    0,    0,
    0, 3557, 1867,    0,    0,12385, 1579,    0,    0,    0,
 1579,    0,    0,12385,    0,    0,    0,    0, 2114, 2114,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2114,    0,    0, 2085, 6840,    0,    0, 2689,    0, 2001,
10705, 1587,  776, 2672,    0, 2396, 1378,    0,    0, 2696,
    0, 9949, 9949,  148,  161,    0,    0, 2239, 2404, 7849,
    0, 2085, 2705,    0, 7849,12385,    0,    0, 2386,    0,
 2715,    0,    0,    0,    0,    0, 1940, 1328,    0, 2379,
    0,    0,    0,    0, 2179,    0, 3557, 2713, 1403,    0,
    0, 2085, 2114,    0,    0, 1606, 2215,    0, 1962,    0,
 2353, 4067,    0, 2332,    0, 4067,    0, 4067, 4033,    0,
    0,    0,    0,10925,  580,12385, 4067,  847, 2215, 9949,
    0,   29,    0,    0, 9137,    0, 2379,  531, 1940, 2383,
 2390,    0,    0, 1403, 2730, 3557, 2431, 2215, 2735, 1867,
 2236, 2736, 3020, 2720, 1760,10925, 2737,    0,    0,    0,
    0, 1556,  225, 2476, 2236,    0, 2060, 9949,    0, 2741,
 2740, 1257, 1940,  910,  910,    0, 2488,    0, 2352,    0,
    0,    0, 1403, 3557,    0,    0, 2085, 2400,    0, 4067,
 2737,    0,    0, 2484, 4067, 2502, 2497,10925, 2400, 9949,
    0,    0,    0, 9137, 2758,    0, 9949,    0,    0, 2341,
  829,    0,    0,    0, 1155, 2215, 1135, 2065,10925,  219,
 9949,10925,    0, 1135,    0,    0, 9949, 2468,    0,    0,
    0, 7849, 2236,    0,    0,    0,10925,    0,    0,    0,
 2759, 2765, 2767,    0,   29, 2400,    0, 9949,    0, 9949,
 1135,    0, 2769,    0,    0,
};
const short yyrindex[] = {                                      0,
    0,  222,  928,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  153, 7274,    0,    0,    0,    0,    0,    0,
 2811,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  198,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  349,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2477,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   61, 2429,
    0,    0,    0,  312,    0,  312,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2432,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1066,    0,    0,10221, 7533,    0,    0,    0, 1274,
    0,    0,    0, 1616,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  246,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  458,    0,    0,    0,    0,  763,    0,    0, 1327,    0,
    0,    0,    0,    0,  107,    0,10671,    0,    0,    0,
    0,    0,    0,    0,    0,  458,    0,  525,    0,    0,
 2780,    0,    0,    0, 2558,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1362,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1395,    0, 2562,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1293,    0,    0,    0,
    0,    0,    0,    0,10294,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  942,    0,    0, 1402,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  458,
    0, 2780,    0, 2558,    0, 1405,    0,    0,    0,    0,
  439,    0,    0,    0,    0, 1409,    0, 2559,    0,    0,
    0,    0, 1452,    0,    0,    0,    0,    0, 2562,  972,
13186,    0, 1474,    0,    0,    0,    0,  354,    0,    0,
    0, 2377, 2377,    0,    0,    0, 1482,  138,    0,    0,
    0,    0,    0,    0,    0, 2562,    0,    0, 1652,    0,
    0,    0,    0,    0,    0,    0, 5405, 2442,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1971,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2679,    0,    0,    0, 2983, 3287,    0,    0,    0,
    0,    0,    0,    0,  705,    0,    0,    0,    0,    0,
    0,    0,    0,  130, 3591,    0, 2373,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1491,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1522,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2519,    0, 4553,    0,
    0,    0,    0,    0,  218,    0,  218,    0,    0, 1073,
    0, 1078, 1344,    0,    0,    0,    0, 1362,    0, 2562,
    0,    0,    0,    0,  136,  213,    0,    0,  782,    0,
  819,    0,    0, 1200,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2559,    0,    0,
 1261,    0, 2371,    0,    0,  566,    0,    0,    0,    0,
  714,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1524,    0,    0, 3921,12722,    0,    0,    0,12734,
    0,    0, 5855,    0,    0,12944,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2224,
 2450,    0,    0,    0,    0,12997,12746,    0,12944,  458,
    0,    0,    0,    0,    0,    0,    0,    0,  972, 1007,
    0,    0,    0,    0, 2425,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 6028, 6165, 6335,    0,    0,  122,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,12465,    0,12465,    0,    0,    0,12465,
12465,12465,    0,    0,    0,    0,    0, 2391,    0,    0,
    0,    0, 4631, 4935,10475, 2801,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2490,    0,    0,    0,    0, 2575,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  315,    0,
    0, 7274,    0,    0,  315,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  913,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  713,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  972,
    0,  141,    0,    0,  541,    0,    0,    0,    0, 1031,
    0,    0,    0,    0,    0,    0,    0, 2066,    0,    0,
    0,    0,    0, 2807,    0,    0, 1401, 2276,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  432,  754,  778,
    0,    0,    0,  293,    0,    0,    0,    0,    0,    0,
 4961,    0,    0,    0,    0,    0, 5855,    0,    0,    0,
    0,  393,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2562,    0,    0,  302,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 8984,    0,    0,    0,  245,    0,
    0,    0,    0, 2539,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,12465,    0,    0,
    0,    0, 1332,    0,    0,    0, 3895, 5151, 5455, 4111,
 4415,    0,    0,    0,    0,    0,    0,    0,    0,    0,
10861,    0, 2578,    0,    0,    0,    0,    0,    0,    0,
    0, 1525, 1527,    0,    0,    0,    0,    0,    0,    0,
 1604,    0,    0,10221,    0, 1604, 1607, 1613,    0,    0,
    0,    0,    0,    0, 1053,  783,  544,    0,    0,    0,
    0, 1030,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  798,    0,    0,    0,    0, 2377, 2393,    0,
 1209,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2575,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1477,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1623,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 8603,    0,    0,    0,    0,    0,    0,    0,
    0, 1495,    0,    0,    0,    0,    0,    0,    0, 1007,
 2575, 2377, 1628,  972,    0, 2588,10221, 1638,  116,    0,
    0,    0,    0, 8984,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  911,    0,    0,    0, 1759,    0,    0,    0,
    0,    0,    0,    0, 2182,    0, 2457,    0, 2887,    0,
    0,    0,    0,    0,    0,    0, 3063,    0,    0, 3397,
    0,    0, 4319,    0, 4839,    0,    0, 5288,    0,    0,
 5764,    0,    0, 1639,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2419,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  739,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1525, 1664,    0, 2423,  350,    0,    0,    0,
    0,    0,    0, 1677, 1722, 1751,    0,    0, 2512, 1398,
    0, 5680,    0, 2517,    0, 2521, 2523,    0,    0,  133,
    0,    0,    0, 2575,    0, 1638,    0,  605,    0, 2409,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1730,
    0, 1477, 2477,    0,    0,    0,    0,    0,  786,  846,
  863,  916,  918,  924,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  906,    0,
    0,    0,    0, 2702,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1753,    0,    0,    0,    0,    0,    0,
    0, 1763,    0,    0,    0,    0,    0,  192,    0,    0,
    0,    0,    0,    0,  128,    0,  303,    0,    0,    0,
10475,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 6239,    0, 6399,    0, 6476,    0,    0,    0,    0, 6487,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2827,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1451,    0,    0,    0,    0,    0,    0, 9398,    0,    0,
    0,    0,    0,    0,    0, 1782, 1819,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  853,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1824,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1780, 5535,    0,    0,    0,
  592,    0,    0,    0,    0,    0,    0,  207,    0,    0,
    0,    0,    0, 1083,    0,    0,    0,    0,    0,    0,
    0,    0, 2828, 1655, 2858,    0, 2559,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2575,    0,    0,
    0,    0,    0,    0,    0, 1802,  967,    0,    0,    0,
    0,    0,    0,    0,    0, 2575, 2588,    0,  351,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  845,    0, 6586,    0,    0,12465,    0,    0,    0,    0,
 6628,    0,    0,    0,    0, 6688,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  283, 2812,
    0,    0,    0,    0,    0,    0,    0,    0, 2818,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2572,    0,    0,    0,    0,    0,
    0,    0,  350,  241,  815,    0,    0, 2512, 1076, 1271,
    0, 1275, 2122,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3407,    0,
    0,  165,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  304,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 6765, 6825,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2256,    0,    0,   47,    0,    0,    0,    0,    0,    0,
  168,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  278, 3585,    0,    0,    0,    0,    0, 2839,    0,
    0,    0,    0,    0,    0,    0, 3407, 1546,    0,  190,
    0,    0,    0,    0,    0,    0,    0,  616,  554,    0,
    0,  240, 2828,    0,    0,    0,   31,    0,    0,    0,
 2834,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  550,    0,    0,    0,    0,  119,    0,
    0, 3889,    0,    0, 2122,    0, 1676,    0, 1477, 1825,
 1983,    0,    0, 2512, 1462,    0,    0,  238,    0,  283,
   76,    0,    0,    0,    0,  550, 2837,    0,    0,    0,
    0,    0, 2582,    0,  113,    0,    0, 1433,    0,    0,
 2847,    0, 3407,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1080,    0,    0,    0,   47,    3,    0,    0,
 2841,    0,    0,12981,    0,    0,    0,  550,   70,    0,
    0,    0,    0, 2122,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2512,   31, 2857,    0,  550,    0,
    0,  550,    0, 1323,    0,    0,    0, 2080,    0,    0,
    0,    0,   76,    0,    0,    0,  550,    0,    0,    0,
 2862,    0,    0,    0, 2977,    3,    0,    0,    0,    0,
 2857,    0,    0,    0,    0,
};
const short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2904,    0,    0, 2906,
    0,    0,    0,    0,    0,    0,    0,   10,    0, 2907,
   -3,  907,  -66, -682, 1699,  964, -293, -257, 2901, 2634,
    0,    8,    0, 2639, -101,   33,    0,  361, -489,  -41,
  -62,  -45, -177, 2644, 2188,    0,    0,    0,  126,    0,
    0,  -35, 1816, -589,  -74, -964, 2281, 2064, -316, 1828,
 2848, -278,    0,  -61,    0, -496,  -54,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -654, -582, 1826, -288,  999, -903,
    0,    0,    0,    0, -326, -123,-1117, 1836, -350,  -52,
    0, -541,    0,    0, 1436,-1004,    0, 1431,    0,    0,
 2602, -676,  -49,    0,    0,    0,    0, 2577,    0, 2355,
 1865,    0,    0,    0, 1918,    0, 2631, 2185, 1858, 2600,
 2604,    0, 1387,    0, -437, 2163,  -32, -580, 1335, -250,
    0,    0, -943, -938, -879, -876, -873,    0, 1478, -795,
    0, 1185,-1205, 1484,    0, 2395,    0,    0,    0, 1194,
 1186, 1158,  -94, -192, -839, -847,    0,    0,    0, 2187,
    0, -575,    0, 2022,    0,    0,    0,    0,    0,-1158,
 -842, -926,    0,    0,    0,    0, -199,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1288,    0,    0,  417,    0,    0, -176,    0, -404,    2,
 1577, 1393,    0, 1226,    0,    0,    0,    0,    0,    0,
    0, 2277, -130, 1975, 1747, 1534,    0, -967, -878, -484,
    0, -509, -817, -616,    0, -561, 1258,  993, -520, -547,
 -747, -913, -836,    0,    0,   -1,    0,    0,    0,    0,
    0, 2643,    0,    0,    0,    0, 2653, 2433,    0,    0,
    0,    0,    0,   -9, 2665,    0,    0,    0,  -10,    0,
    0,    0, -349, 2161, -348, 1708, -476,    0,    0,    0,
    0,    0, -917,    0,    0,    0,    0, 2380,    0, 2174,
    0,    0,    0,    0,    0,    0,  173,    0, 3000,    0,
    0,    0,    0,    0, 2706,    0,    0,    0,    0, 2504,
    0, 2499,    0,    0,    0, 2264,    0,    0,    0, -546,
   71,    0,    0,  671,    0, 2259,    0,    0,    0,    0,
    0, -632,-1042, -639, -196,  981,    0, 2885, 1658,    0,
 2287, -280, -965,-1404,    0, -923, 2040,    0,    0,    0,
    0, -146,    0,    0,    0, 1319, 1427,    0, 1193,    0,
 1238,    0, -255,    0,    0,    0,    0, 2520,    0,    0,
    0, -445, 2312, 2068,-1077,    0,    0,    0, -695,   62,
  112,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2829, 1306, 1907,    0, -597,    0,
    0,    0, -867,    0,    0,    0, 1796,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -308, 1882,    0,    0,
};
#define YYTABLESIZE 13800
const short yytable[] = {                                     132,
   79,  198,  132,  221,  270,  154,  153,  319,  159,   38,
  930,  217,  290,  293,  956,  509,  590,  258,  766,  289,
  292,  620,  621,  796,  252,  558,  697,  213,  238,  233,
  711,  218,  261, 1114,  752,  256,  584, 1115,  772,  255,
 1397, 1269,  730,  625,  195,  197,  200,  203,  153,  208,
  262,  556,  260,  987,  219,  197,  224,  870,  203, 1192,
  153,  208,  938,  237,  132,  204, 1489,  230, 1014,  625,
 1229,  707,  248,  154,  153,  250,  197,  231,  248,  224,
  200,  203,  259,  153,  208,  237,  132,  702,  203,  201,
  230, 1258,  861,  503,  324,  482, 1259,  560,  300,  984,
  226,  276, 1258,  278,  482, 1221,  152, 1259, 1294,  650,
  625,  953,  711, 1096, 1201,  361,  711,  526,  707, 1104,
  787,  708,  788,  257,  219,  153,  219,  798,  625,  318,
  263,  376,  707,  383,  288,  631, 1266,  153,  224, 1449,
  635,  960,  277, 1270,  318,  203,  153,  237,  727,  765,
 1119,  159,  577,  711, 1278, 1260,  563,  848, 1261,  707,
  345, 1262,  708,  322,  762,  152, 1260, 1263,  784, 1261,
  527,  711, 1262,  707,  361, 1299,  631,  707, 1263,  631,
  708,  635, 1316,  634,  635,  372,  761,  320,  318,  240,
 1019,  707,   78,  779,  631, 1070,  153,  625, 1076,  635,
  737,  287, 1267,  750, 1765,  762,  711, 1039,  762, 1038,
 1042,  577,  624,  493, 1210,  620,  621,  605, 1317, 1049,
 1052,  577,  532,  762,  288,  530,  320, 1163, 1164,  845,
  240,  467,   90,  240,  475, 1002,  274,  707,  625,  702,
  709,  590,  328, 1685,  291,  638,  512,  711,  240, 1585,
 1586, 1311,  818,  624,  219,  274,  625,  219, 1189, 1480,
 1481,  153, 1758,  484,  132,  711, 1460,  288, 1758,  515,
  153,  624,  484, 1091,  904, 1524,  605,  702,  478,  804,
  577,  709,   98, 1658,  525,   86,  638,  478,   86,  819,
  904,  200,  179, 1504, 1213,  153,  707, 1216,  702,  709,
 1451,  126,  725,  709,  638,  830, 1224, 1226,  557, 1461,
   78,   78,  519, 1028,  753,  467,  831,  218,  702,  571,
 1258, 1258,  573,  709,  355, 1259, 1259,  842,  585,  642,
  587,  219,  632,  326,  155,  840,  702,  769,  219, 1514,
  219,  527,  219,  786,  638,  576,  373, 1569,  579,  528,
  179,  179, 1525,  481, 1258,  762, 1618, 1254,  542, 1259,
  126, 1077,  709,  203,  208, 1806,  156,  554,  286, 1248,
  387, 1117,  153,  753,  200, 1502,   61,  702,  392, 1056,
  555,  157,  374, 1499, 1260, 1260, 1029, 1261, 1261,  640,
 1262, 1262,  136,  820,  908, 1006, 1263, 1263,  905,   14,
  161, 1650,   99,  841, 1469,  630,  152,  579,  528,  179,
 1604,  937,  481, 1217,  905,  844,   78,  707, 1260, 1482,
 1659, 1261,  700, 1729, 1262,  517,  707,  952, 1696,  738,
 1263,  389, 1097,  702,  288, 1865, 1244,  153,  640,  708,
 1334, 1335,  702, 1336, 1337,  991, 1322, 1181, 1037,  708,
  707,  136,  708,  631,  152,  525, 1264, 1265,  635,  824,
  760,  708,  711, 1518, 1764,  509,  153, 1271, 1272,  203,
  484,  355,  707, 1103, 1487,  631, 1277,  153,  988,  640,
  635,  153, 1046,  721,  203,  153,  153, 1016, 1836,  159,
  389, 1605, 1594, 1033,  729, 1118, 1035,  640,  735,  711,
   90,  706,  704,  478,  705,  707,  707, 1045,  708,  825,
  780,  724,  282,  784,  707,  625,  870,  708, 1323,  707,
   84, 1776, 1877,  625,  711, 1711,  724,  543,  707, 1069,
  624, 1343,  800, 1529, 1063,  852, 1220,  625,  560,  318,
  642, 1608, 1310,  672, 1456,  992,  711,  762, 1106,  482,
  826,  707,  624,  367,  793,  219, 1655,  389,  726,  605,
  793,  708, 1517,  638,  847, 1015, 1660,  702,  709,  361,
  702,  709,  707, 1816, 1620,  708, 1477,  320, 1621, 1622,
 1222,  642,  230,  957,  672,  638,  707,  672,  577,  631,
  625,  673,  179,  711,  635, 1111,  711,  367, 1491,  642,
  855,  856,  672, 1648,  641,   77,  218, 1849,  702, 1069,
  709,  286,  367,  709, 1649,  366,  286,  481,  527,  286,
 1392, 1615,  605,  152, 1209,  605, 1007,  709,  912,  219,
  577,  709,  673,  711,  709,  673,  709,  631,  872,  707,
  179,  605,  708,  749,  871,  641,  711,  273,  179,  126,
  673,  625, 1187,  707,  153,   86,  631, 1664, 1641, 1089,
  711,  635,  286,  641,  702,  762,  624,  605,  224,  709,
  136,  740,  136,  702,  366,  910, 1647,  707,  709,  702,
 1748,  753,  467, 1651,  467,  179,  965,  179,  786, 1487,
  240,  707,  946,  702,  762,  389,  648,  648,  179,  638,
  219, 1249, 1878, 1790,  745,  219, 1478,  219, 1536,  219,
  179, 1722,  484,  403,  878, 1723,  528,  484,  625, 1214,
 1442,  252, 1444,   77,   77, 1530,  484,  711, 1516,  203,
 1732, 1225,  525,  624,  203,  179, 1453,  218, 1004,  702,
 1054, 1258, 1027,  709,  703,  745, 1259, 1736,  745,  153,
  481,  153, 1619,  387,  219, 1268,  467,  709, 1120, 1279,
  219,  709,  383,  745,  879, 1312,  519, 1276, 1013,  886,
  136,  724,  403, 1086, 1087, 1088, 1490,  388,  640,  219,
  136,  636,  671, 1055, 1057,  394, 1389, 1106, 1203, 1298,
 1612, 1206, 1068, 1733, 1599, 1634,  753,  637,  702, 1734,
  637,  588,  648,  709, 1281, 1260,  219,  219, 1261,  179,
  123, 1262,  387,  230,  679,  306, 1754, 1263,  644, 1503,
 1062,  383,  636,  671, 1708, 1513, 1459, 1557,  389,   77,
 1782, 1814, 1521, 1074,  329,  527,  388, 1800,  637, 1789,
  636,  671, 1701,  153,  394,  395,  230,  230,  230,  542,
 1369,  377,  706,  153,  679,  679,  637,  707,  679,  644,
 1090,  672,  391, 1280, 1575, 1113, 1747,  330,  252, 1834,
 1101,  672, 1382,  679,  672, 1384,  811,  644,  812, 1821,
  642,  367, 1121,  672,  367,  530,  700, 1804,  530,  904,
 1132, 1702,  640,  294, 1769,  774,  700,  577, 1783,  162,
 1187, 1693, 1703, 1187,  395,  137, 1792, 1207,  578,  673,
  835, 1863,  709,  211,  212,  390, 1133,  393,  138,  673,
  225,  391,  673,  392, 1788, 1208, 1205,  904,   14, 1600,
  672,  673, 1876,  755,  700, 1879,  909,  933,  934,  672,
  509,  600, 1116,  366,  641, 1282,  366,  515,  378,  525,
 1887,  835, 1007,  709,  835, 1570,  484,  203,  208,  484,
  237,  132,  978,  779,  137, 1442,  138,  484,  745,  835,
 1576,  709, 1815,  904,  390,  139,  393,  403,  673,  484,
  484,  209,  392,  672,  203,  123,  123,  673,  879, 1184,
 1185,  475,  879,  367,  642,  672, 1498,  672,  538, 1204,
  600,  434,  330, 1228,  756,  367, 1704,  367,  152, 1856,
 1305,  745,  745,  905,  219,  346,  904,  387,  589,  620,
  621,  383, 1193, 1283,  745,  138,  383, 1520, 1325,  702,
  170,  673,  383, 1768, 1329,  978,  509,  904, 1772,  706,
  704,  388,  705,  673,  707,  673,  282,  283,  516,  394,
   77,  905,  689,  527, 1241,  366,  330, 1289,  641, 1684,
 1284,  218, 1387,  956,  672,  575, 1290,  366,  475,  366,
  702, 1195,  606, 1289, 1563,  705,  249,  616, 1566,  368,
  134,  935,  179,  230,  219, 1447, 1447, 1450,  702,  170,
  702,  219,  347,  689, 1712,  703,  689,  905,  243,  636,
  671,  230, 1187,  517,  284, 1242, 1187,  230,  904,  395,
  671,  689,  673,  671,  745,  637,  705,  609,  530,  705,
  518,  636,  671,  368,  575, 1474,  391, 1476, 1870,  338,
 1791,  606,  679, 1186,  705,  679,  616,  637,  368,  702,
  905,  179,  679, 1742,  703,  679, 1393,  135,  356,  679,
  387, 1828, 1805,  530,  679,  745,  745,  679,  644,  383,
  280,  905, 1300,  467,  745, 1839,  218,  525,  609,  671,
  330, 1825,  835,  679,  388, 1784,  801,  339,  671,  390,
 1871,  393,  394,  375,  264,  137,  219,  392, 1519, 1398,
 1439,  330,  116,  745,  745, 1885,  745,  679,  804,  474,
  484,  679,  484,  745,  745,  745,  745,  745,  171,  745,
  679, 1301,  745,  745,  745,  745,  745,  484, 1787,  148,
  745,  745,  671,  203, 1823,  357,  745,  679,  835,  116,
  709,  835,  905, 1886,  671,  636,  671, 1656,  835, 1873,
  709,  835,  395,  709,  375,  835,  138,  163,  732,  225,
  835,  637,  709,  835,  679,  530, 1683,  802,  474,  391,
  468,  435, 1855, 1787,  116,  383,  679,  171,  679,  835,
  701,  504,  527,  584,  706,  733,  383,  702,  294,  510,
 1827, 1857,  703,  600, 1187,  501, 1850, 1501,  170,  340,
  225, 1528, 1864,  835,  435, 1532,  835,  835,  803,  709,
  164,  170, 1787,  671,  468,  330,  835,  341,  709,  245,
  530,  701,  390, 1607,  393,  706,  689, 1180,  600,  468,
  392,  295,  638,  835,  149,  535,  581, 1851,  539,  701,
  170,  150,  584,  706, 1787,  679,  170, 1574,  170, 1891,
  179,  342, 1141,  601, 1471,  835,  600,  702,  253,  600,
  835, 1717,  709,  179, 1472, 1626,   81,   82, 1142, 1735,
  702,  709,  835,  638,  835,  600,  709,  267,  101,  702,
  689,  225,  886,  689, 1852,  886, 1718, 1627,  170, 1010,
  689,  638,  179,  689, 1628,  581,  525,  689,  179,  197,
  179,  600,  689,  705,  320,  689, 1011, 1584, 1113, 1113,
  206,  583,  601, 1143,  570,  103,  705,  368,  410, 1439,
  368,  689,  225, 1698,  606,  705,  702,  266, 1629,  616,
  709, 1758,  700,  688,  886,  702, 1234,  693,  694,  696,
  179,  835, 1144,  709, 1719,  689, 1757, 1300,  179,  689,
 1235,  206,  271, 1700,  206,  484,  143,  144,  689,  606,
  318,  411,  272,  320,  616,  203,  240,  474, 1714,  206,
  583,  275,  705,  570, 1236,  689,  171,  410,  279,  702,
  474,  705, 1447,  407,  104, 1234,  234,  606,  153,  171,
  606,  161,  616,  702,  105,  616, 1304,  280, 1715, 1235,
   44,  887,  689,  241,  887, 1632,  606,  362,  741, 1145,
  350,  616,  743,  351,  689,  474,  689,  474,  171,  106,
  411,  179,  689, 1236,  171,  705,  171,  234, 1237,  368,
  234,   52,  606,  162,   42,  281,   25,  616,   53,  705,
 1730,  368,  407,  368,  363,  234,  364, 1872,  505,   54,
  161,  689,  689,  887,  689,  762,   55,  170,  352,   44,
  702,  689,  689,  689,  689,  689,  171,  689,  285,  365,
  689,  689,  689,  689,  689,  203,  301, 1237,  689,  689,
 1441,   56, 1443,  689,  689, 1698,  294, 1113, 1731, 1698,
   52, 1798,  162,   42,  366,   25,  762,  505,  701,  762,
 1698, 1699,  706, 1845,  144,  318,  705,  706,  704,  179,
  705,  701,  707,  758,  762, 1700,   48, 1691, 1709, 1700,
  701, 1700,   50,  946,  706,  692, 1698,  527,  153,  906,
 1700, 1744,  483,  200,  294, 1745, 1777,  128,  706,  704,
 1846,  705, 1892,  707, 1893, 1728,   57,  378,  780,  317,
 1760,  154,  153, 1847,  327, 1753, 1700,  706,  704,  527,
  705,  294,  707, 1698,  200,  330,  885,  701, 1698,  692,
  348,  706,  758,   26,  359,   48,  701,  914,   58,  370,
  706,   50,   59,  435,  692,  240,   49,  913,  206,  780,
  206,  483,  780, 1700,  929,  601,  128,  206, 1700,  936,
  435,  527,  197, 1762,  916,  200,  378,  780,  200,  943,
 1763,  947,  948,  949,  950,  951,  435,  943,  716,  767,
  701,  715,  527,  200,  706,  527,  240,  382,  674,  240,
  601,   51,   26, 1234,  701,  171,  154,  153,  706,  213,
  527,  525,  674,  361,  240,   49,  767, 1235,  294,  380,
  674,  385,  706,  704,  435,  705,  384,  707,  601,  219,
   46,  601,  129, 1699, 1439,  388,  674, 1699,  804, 1799,
 1004, 1236,  117,  525, 1808,  153,  435,  601, 1699,  722,
  213,  535, 1023,  213,  720,  154,  153,  719,  206,   47,
   51,  918,  369,  742,  706,  704, 1439,  705,  213,  707,
  389,  701, 1868,  601, 1699,  706,  369, 1808,  153,  804,
  206,  200,  804, 1758,  369,  525,  722,  206,  479,   46,
  674,  129,  487,  154,  153, 1237,  488,  804, 1833,  722,
  369,  117,  472,  762,  236,  762,  525, 1000, 1439,  525,
 1004, 1699,  762, 1808,  153,  702, 1699,  200,   47, 1000,
  703,  206, 1000,  492,  525,  706,  704,  491,  705, 1439,
  707, 1113, 1439,  706,  704,  494,  705,  200,  707,  498,
  200, 1064, 1065,  496, 1005,  236,  702, 1439,  236,  991,
  497,  703,  706,  704,  369,  705, 1005,  707,  200, 1005,
  200,  988,  511,  236,  978,  702,  514, 1122, 1123, 1127,
  703,  513, 1135, 1137, 1139, 1835, 1147, 1150, 1153, 1155,
 1158, 1161,  523,  790, 1165, 1166,  789, 1168,  940, 1170,
   14,  708, 1173, 1174, 1175, 1176, 1177, 1178,  939,  706,
  704,  533,  705,  762,  707,  744, 1752, 1874,  762, 1182,
  983,  435, 1095,  982, 1880, 1094,  692,  706,  704, 1190,
  705,  540,  707,  692,  332,  762,  692,  547, 1102,  549,
  692,  866,  762,  240,  548,  240,  780,  333,  692,  780,
 1194, 1894,  240, 1193, 1197, 1245,  780, 1196, 1094,  780,
  998,  551, 1292,  780,  692, 1291,  267,  569,  780,  572,
  702,  780,  237,  435, 1297,  703,  762, 1296,  582,  993,
  706,  704,  334,  705,  335,  707,  586,  780,  692, 1172,
  706,  704,  635,  705,  639,  707,  643,  225,  641,  213,
  974,  998,  998,  998,  998,  998,  998,  998, 1561,  674,
  804,  780,  702,  237,  780,  780,  237,  703,  692,  998,
  998,  998,  998,  692,  780,  675,  408,  409,  410,  211,
  212,  237,  706,  704,  676,  705,  762,  707, 1367,  706,
  704,  780,  705,  240,  707,  692, 1368,  706,  704, 1750,
  705,  998,  707,  998,  418,  419,  678,  692, 1454,  692,
  677,  708,  679,  780, 1538,  240,  804, 1193,  780,  804,
  684, 1568,  240,  702,  708,  680,  804,  681,  703,  804,
  780,  702,  780,  804, 1606,  240,  703,  708,  804,  708,
 1841,  804,  236, 1840,  236, 1875,  373,  225, 1758,  373,
  702,  236,  124,  124, 1347,  703,  240,  804,  682,  685,
  730,  994, 1545,  730, 1371,  706,  704,  943,  705,  225,
  707,  302,  241,  683,  686,  691,  225,  692,  701, 1350,
 1351,  804, 1353, 1355,  804,  804,  710, 1372,  706,  704,
 1360,  705,  709,  707,  804,  706,  704,  702,  705,  780,
  707,  717,  703,  714,  718,  736,  303,  722, 1795, 1373,
  225,  804, 1797,  727,  739,  702,  240, 1380, 1381,  435,
  703,  805,  435, 1803, 1182,  744,  746,  759, 1388,  435,
 1390, 1391,  747,  804,  304,  763,  764,  305,  804, 1136,
  768, 1140,  236, 1148, 1151,  770, 1156, 1159, 1162, 1831,
  804,  771,  804,  306,  773,  775,  778,  776,  542,  435,
  794,  781,  805,  507,  236,  805,  785,  782,  702,  435,
  784,  236,  998,  703,  998,  998,  791,  815,  702,  307,
  805,  816,  998,  703,  817,  822, 1858,  828,  286,  832,
  998, 1860,  827,  833,  834,  998,  835,  837,  998,  838,
  237,  849,  237,  843,  507,  236, 1483,  507,  850,  237,
  854,  853,  998,  863,  865,  497,  866,  998,  998,  804,
  702,  998,  507,  874,  875,  703,  876,  702,  998,  880,
  998,  998,  703,  931,  998,  702,  941,  844,  998,  288,
  703,  998,  961,  998,  980,  998,  985,  977,  978,  989,
  998,  998,  990,  998, 1017,  435,  497, 1008, 1001,  497,
 1024, 1020, 1374,  706,  704,  236,  705, 1026,  707,  998,
  998,  907,  998,  911,  497, 1034, 1533,  915,  917,  919,
 1043, 1044, 1046, 1050,  794, 1541,  998, 1053,  998,  998,
  998, 1059, 1546,  998, 1051, 1060,  998,  998, 1067, 1061,
  237, 1072, 1078,  702, 1083, 1085,  998, 1092,  703, 1375,
  706,  704,  978,  705, 1097,  707, 1109, 1110,  732, 1129,
  998,  435,  237,  998, 1167,  435,  702, 1169, 1171,  237,
 1179,  703, 1191,  702,  702, 1200,  107, 1198,  703,  998,
 1212, 1215, 1579, 1218, 1210,  998,  998, 1227, 1231, 1233,
  998, 1250, 1239,  978,  978,  978,  978,  978,  991,  978,
 1253,  998,  998,  237,  998,  108,  109, 1275,  110, 1295,
  998,  978,  978,  978,  978,  111,  112,  113,  114,  115,
 1273,  116, 1285,  805,  117,  118,  119,  120,  121,  530,
 1293, 1302,  122,  123, 1306, 1307,  829, 1308,  124,  998,
  998, 1309,  998,  978, 1609,  978, 1318, 1103, 1327,  998,
  998,  998,  998,  998, 1320,  998, 1330, 1331,  998,  998,
  998,  998,  998,  237, 1332, 1333,  998,  998, 1339,  530,
  530,  998,  998,  530, 1340, 1345, 1344,  829,   14,  805,
  829,  507,  805,  507, 1346, 1348, 1349,  507,  530,  805,
  507, 1356,  805, 1357, 1358,  829,  805, 1359, 1361, 1362,
 1363,  805, 1364,  507,  805, 1365, 1366,  904, 1386, 1440,
  530, 1383, 1455,  530, 1457, 1458, 1462, 1463, 1465, 1464,
  805,  107, 1669, 1670, 1466, 1467,  719, 1475,  530,  564,
  530, 1500, 1492,  497, 1488,  497, 1515, 1493, 1505, 1681,
 1494,  702,  497,  435,  805, 1495,  703,  805,  805, 1496,
  108,  109, 1690,  110, 1692,  497, 1497,  805, 1522, 1526,
  111,  112,  113,  114,  115, 1069,  116, 1527, 1531,  117,
  118,  119,  120,  121,  805, 1537, 1558,  122,  123, 1341,
 1564,  507, 1565,  124, 1571, 1572, 1578, 1577,  702,  135,
  100, 1580, 1581,  703, 1590, 1588,  805,  706,  704, 1589,
  705,  805,  707,  507, 1591, 1592, 1597, 1598, 1601, 1602,
  507, 1613, 1603,  805,  978,  805,  978,  978, 1623, 1633,
 1635, 1636, 1637, 1638,  978, 1639, 1640, 1642, 1643, 1644,
 1652,  101,  978,  497, 1743, 1657, 1654,  978, 1661, 1662,
  978, 1663, 1746,  102,  507, 1376,  706,  704, 1665,  705,
 1666,  707, 1667, 1668,  978,  497,  978, 1671, 1002,  978,
  978, 1672,  497,  978,  994, 1688, 1673, 1674,  103,  435,
  978, 1675,  978,  978, 1676, 1677,  978, 1678, 1679, 1680,
  978,  530,  805,  978, 1682,  978, 1695,  978, 1593, 1710,
 1716, 1724,  978,  978, 1609,  978,  497, 1727,  829, 1002,
 1002, 1002, 1002, 1002, 1002, 1002, 1726,  530, 1749,  530,
 1755,  978,  978,  530,  978, 1756,  530, 1002, 1002, 1002,
 1002, 1759,  530, 1767, 1770,  530, 1766,  530,  978,  530,
  978,  978,  978, 1774, 1775,  978, 1786,  104,  978,  978,
  530, 1778, 1793,  530, 1802, 1817, 1796,  105,  978, 1002,
 1822, 1002, 1819, 1824,  829, 1826, 1829,  829, 1832, 1838,
 1758, 1843,  978, 1844,  829,  978,  163,  829, 1814,  530,
 1859,  829,  106, 1815, 1861,  530,  829, 1867,  530,  829,
 1862,  978, 1888, 1869, 1883, 1889, 1890,  978,  978, 1895,
    1,   30,  978,  631,   32,  829, 1377,  706,  704,  186,
  705,  762,  707,  978,  978,  254,  978,  530,  706,  704,
  528,  705,  978,  707,  149,   82,  375, 1338,  663,  829,
  465,  961,  829,  829,  745,  265,  196,  528,  264,  530,
  176,  403,  829,  960,  700,  702,  530,  214,  528,  698,
  703,  978,  978,  694,  978,  696,  177,  942,  376,  829,
  206,  978,  978,  978,  978,  978,  315,  978,  716,  281,
  978,  978,  978,  978,  978,  335,  808,  718,  978,  978,
  530,  829,  323,  978,  978,  350,  829,  638,  214,  321,
  530,  214,  727,   27,  702,   30,   40,  530,  829,  703,
  829, 1445,  140,  486,  979,  495,  214,  485, 1378,  706,
  704, 1328,  705,  864,  707,  530, 1324,  808, 1314, 1108,
  808, 1326, 1379,  706,  704,  254,  705, 1646,  707,  553,
 1002, 1645, 1002, 1002,  594,  808,  829, 1303, 1252,  566,
 1002, 1315,  531,  567, 1018,  530, 1686, 1725, 1002,  530,
 1048, 1813, 1625, 1002,  810, 1818, 1002, 1559,  706,  704,
 1631,  705, 1854,  707, 1820, 1041,  371,  829, 1751,  530,
 1002,  530, 1007, 1583, 1202, 1002, 1002, 1707,  530, 1002,
 1801,  958, 1470, 1246, 1785,  559, 1002,  574, 1002, 1002,
  550,  530, 1002, 1616,  814, 1084, 1002, 1512,  851, 1002,
 1075, 1002,  142, 1002,  524,  758,  754,  371, 1002, 1002,
  371, 1002, 1009, 1007, 1007, 1007, 1007, 1007, 1007, 1007,
  299, 1022, 1003, 1230, 1773,  371, 1866, 1002, 1002, 1720,
 1002, 1007, 1007, 1007, 1007, 1842,  986,  767, 1544, 1352,
 1479,    0,    0, 1223, 1002,  702, 1002, 1002, 1002, 1830,
  703, 1002,  806,    0, 1002, 1002,  702, 1385,    0,    0,
    0,  703,    0, 1007, 1002, 1007,    0,  432,    0,  530,
  920,  921,  922,  923,  924,  925,  926,  927, 1002,    0,
    0, 1002,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  530,    0,  806,  107,    0,  806, 1002,  530,    0,
    0,    0,    0, 1002, 1002,    0,    0,    0, 1002,    0,
    0,  806,    0,    0,    0,    0,    0,    0,    0, 1002,
 1002,    0, 1002,  108,  109,  225,  110,  214, 1002,    0,
    0,    0,  530,  111,  112,  113,  114,  115,  808,  116,
    0,    0,  117,  118,  119,  120,  121,  702,    0,    0,
  122,  123,  703,    0,    0,    0,  124, 1002, 1002,    0,
 1002,  702,    0,    0,    0,    0,  703, 1002, 1002, 1002,
 1002, 1002,    0, 1002,    0,    0, 1002, 1002, 1002, 1002,
 1002,    0,    0,    0, 1002, 1002,    0,    0,    0, 1002,
 1002,    0,    0,    0,  808,    0,  702,  808,    0,    0,
    0,  703,    0,    0,  808,    0,    0,  808, 1560,  706,
  704,  808,  705,    0,  707,    0,  808,    0,    0,  808,
    0,    0,    0,    0,    0,  225,    0,    0,    0,    0,
    0,    0,    0,    0, 1007,  808, 1007, 1007,    0,    0,
    0,    0,    0,    0, 1007,    0,    0,  225,    0,    0,
    0,    0, 1007,    0,  225,    0,    0, 1007,    0,  808,
 1007,    0,  808,  808,    0,    0,    0,    0,    0,    0,
    0,    0,  808,    0, 1007,    0, 1003,    0,    0, 1007,
 1007,    0,    0, 1007,    0,    0,    0,  371,  225,  808,
 1007,    0, 1007, 1007,  371,    0, 1007,  371,    0,    0,
 1007,  371,    0, 1007,    0, 1007,    0, 1007,    0,  371,
    0,  808, 1007, 1007,  806, 1007,  808, 1003, 1003, 1003,
 1003, 1003, 1003, 1003,    0,  371,    0,    0,  808,    0,
  808, 1007, 1007,    0, 1007, 1003, 1003, 1003, 1003,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1007,  371,
 1007, 1007, 1007,    0,    0, 1007,    0,    0, 1007, 1007,
    0,    0,    0,    0,    0,    0,    0, 1003, 1007, 1003,
  806,    0,    0,  806,    0,    0,    0,    0,    0,  371,
  806,    0, 1007,  806,    0, 1007,  807,  806,    0,    0,
    0,    0,  806,    0,    0,  806,  234,  808,    0,    0,
    0, 1007,    0,    0,    0,    0,  371, 1007, 1007,    0,
    0,  806, 1007,    0,    0,    0,    0,    0,  371,    0,
  371,    0,    0, 1007, 1007,    0, 1007,  807,    0,    0,
  807,    0, 1007,    0,    0,  806,    0,  234,  806,  806,
  234,    0,    0,    0,    0,  807,    0,  702,  806,    0,
    0,    0,  703,    0,    0,  234,    0,    0,    0,    0,
    0, 1007, 1007,    0, 1007,  806,    0,    0,    0,  107,
    0, 1007, 1007, 1007, 1007, 1007,    0, 1007,    0,    0,
 1007, 1007, 1007, 1007, 1007,    0,    0,  806, 1007, 1007,
    0,    0,  806, 1007, 1007,    0,    0,    0,  108,  109,
    0,  110,    0,    0,  806,    0,  806,    0,  111,  112,
  113,  114,  115,    1,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,    0,
    0,  124,    0,    0,    2,    0,    0,    0, 1003,    0,
 1003, 1003,    3,    0,    0,    0,    0,    0, 1003,    4,
    0,    5,    0,    6,    0,    0, 1003,    0,    7,    0,
    0, 1003,    0,    0, 1003,    8, 1567,  706,  704,    0,
  705,    0,  707,  806,  681,    0,    0,    0, 1003,    9,
  856,    0,    0, 1003, 1003,    0, 1737, 1003,    0,    0,
   10,    0,    0,    0, 1003,    0, 1003, 1003,    0,    0,
 1003,    0,    0,    0, 1003,    0,    0, 1003,    0, 1003,
    0, 1003,    0,    0,    0,  681, 1003, 1003,  681, 1003,
    0,  856,  856,  856,  856,  856,    0,  856,    0,    0,
    0,    0,    0,  681,    0, 1003, 1003,    0, 1003,  856,
  856,  856,  856,    0,    0,    0,    0,    0,  807,    0,
    0,    0, 1003,    0, 1003, 1003, 1003,   11,  955, 1003,
    0,    0, 1003, 1003,   12,    0,    0,   13,    0,   14,
   15,    0, 1003,  856,  234,    0,  234,    0,    0,    0,
    0,    0,    0,  234,    0,    0, 1003,    0,    0, 1003,
    0,    0,    0,    0,   16,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  807, 1003,    0,  807,    0,    0,
    0, 1003, 1003,    0,  807,    0, 1003,  807,    0,    0,
    0,  807,   63,    0,    0,    0,  807, 1003, 1003,  807,
 1003,    0,    0,   64,    0,    0, 1003,    0,    0,    0,
   65,    0,    0,   17,    0,  807,    0,    0,    0,    0,
    0,    0,    0,   66,    0,    0,    0,    0,   18,   19,
    0,    0,    0,    0,    0, 1003, 1003,    0, 1003,  807,
    0,    0,  807,  807,  234, 1003, 1003, 1003, 1003, 1003,
    0, 1003,  807,    0, 1003, 1003, 1003, 1003, 1003,    0,
    0,    0, 1003, 1003,    0,    0,  234, 1003, 1003,  807,
    0,    0,    0,  234,    0,  702,    0,    0,    0,    0,
  703,    0,    0,    0,   67,    0,    0,    0,    0,    0,
    0,  807,    0,    0,    0,    0,  807,    0,    0,    0,
   68,    0,    0,    0,    0,    0,    0,  234,  807,   85,
  807,    0,  856,    0,  856,  856,   86,   69,    0,    0,
   87,   88,  856,    0,    0,    0,    0,    0,    0,   89,
  856,    0,   70,    0,    0,  856,   71,    0,  856,   72,
    0,   90,    0,    0,    0,    0,    0,   73,  684,    0,
    0,    0,  856,    0,  868,    0,    0,  856,  856,    0,
    0,  856,  681,    0,    0,  681,   74,    0,  856,    0,
  856,  856,  681,    0,  856,  681,    0,  807,  856,  681,
  528,  856,  954,  856,  681,  856,    0,  681,    0,  684,
  856,  856,  684,  856,    0,  868,  868,  868,  868,  868,
    0,  868,    0,  681,    0,    0,   91,  684,    0,  856,
  856,    0,  856,  868,  868,  868,  868,    0,    0,    0,
    0,  528,    0,   92,  528,    0,  856,  681,  856,  856,
  856,  681,    0,  856,    0,    0,  856,  856,   93,  528,
  681,  528,   94,    0,    0,    0,  856,  868,    0,    0,
    0,    0,    0,   95,    0,    0,    0,  681,    0,    0,
  856,    0,    0,  856,    0,    0,    0,    0,    0,    0,
    0,    0,   96,    0,    0,    0,  107,    0,    0,  856,
    0,    0,    0,    0,  681,  856,  856,    0,    0,    0,
  856,    0,    0,    0,    0,    0,  681,    0,  681,    0,
    0,  856,  856,    0,  856,  108,  109,    0,  110,    0,
  856,    0,    0,    0,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,  955,    0,    0,    0,  124,  856,
  856,    0,  856,    0,    0,    0,  107,    0,    0,  856,
  856,  856,  856,  856,    0,  856,    0,    0,  856,  856,
  856,  856,  856,    0,    0,  681,  856,  856,    0,    0,
  871,  856,  856,    0,    0,  108,  109,    0,  110,    0,
    0,    0,    0,    0,  432,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,    0,
    0,  871,  871,  871,  871,  871,  868,  871,  868,  868,
    0,    0,    0,    0,    0,    0,  868, 1534, 1535,  871,
  871,  871,  871, 1539, 1540,    0, 1542, 1543,  528,  868,
    0,    0,  868,    0, 1547, 1548, 1549, 1550,    0, 1551,
 1552, 1553, 1554, 1555, 1556,    0,  868,  528,  528,    0,
  528,  868,  868,  871,  528,  868,  684,  528,    0,  684,
    0,    0,  868,    0,  868,  868,  684,    0,  868,  684,
  528,    0,  868,  684,    0,  868,  528,  868,  684,  868,
    0,  684,    0,    0,  868,  868,    0,  868,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  684,    0,    0,
    0,    0,    0,  868,  868,    0,  868,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  868,  684,  868,  868,  868,  684,    0,  868,    0,    0,
  868,  868,    0,    0,  684,    0,    0,    0,    0,    0,
  868,    0,    0,    0,    0,    0,    0,    0,  528,    0,
    0,  684,    0,    0,  868,    0,    0,  868,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  837,    0,
  528,    0,    0,  868,    0,    0,    0,  528,  684,  868,
  868,    0,    0,    0,  868,    0,  528,    0,  528,    0,
  684,    0,  684,    0,    0,  868,  868,    0,  868,    0,
    0,   78,    0,    0,  868,    0,  528,    0,    0,  837,
    0,  528,  837,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  871,    0,  871,  871,    0,  837,    0,    0,
    0,    0,  871,  868,  868,    0,  868,    0,    0,    0,
  528,    0,    0,  868,  868,  868,  868,  868,  871,  868,
    0,    0,  868,  868,  868,  868,  868,    0,    0,  684,
  868,  868,  871,    0,  872,  868,  868,  871,  871,    0,
    0,  871,    0,    0,    0,    0,    0,    0,  871,    0,
  871,  871,    0,    0,  871,    0,    0,    0,  871,    0,
    0,  871,    0,  871,    0,  871,    0,    0,    0,    0,
  871,  871,    0,  871,    0,  872,  872,  872,  872,  872,
    0,  872,    0,    0,    0,    0,    0,    0,    0,  871,
  871,    0,  871,  872,  872,  872,  872,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  871,    0,  871,  871,
  871,    0,  107,  871,    0,    0,  871,  871,    0,    0,
    0,    0,    0,    0,    0,    0,  871,  872,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  871,  108,  109,  871,  110,    0,  107,    0,    0,    0,
    0,  111,  112,  113,  114,  115,    0,  116,    0,  871,
  117,  118,  119,  120,  121,  871,  871,    0,  122,  123,
  871,    0,    0,    0,  124,  108,  109,    0,  110,    0,
    0,  871,  871,    0,  871,  111,  112,  113,  114,  115,
  871,  116,    0,    0,  117,  118,  119,  120,  121,    0,
  837,    0,  122,  123,    0,    0,    0,    0,  124,    0,
    0,    0,    0,    0,    0,    0,  667,    0,    0,  871,
  871,    0,  871,    0,    0,    0,    0,    0,    0,  871,
  871,  871,  871,  871,    0,  871,    0,    0,  871,  871,
  871,  871,  871,    0,    0,    0,  871,  871,    0,    0,
  866,  871,  871,    0,    0,    0,  837,    0,    0,  837,
    0,    0,    0,    0,    0, 1573,  837,    0,    0,  837,
    0,    0,    0,  837,    0,    0,    0,    0,  837,    0,
    0,  837,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  866,    0,  866,  866,  866,  872,  837,  872,  872,
    0,    0,    0,    0,    0,    0,  872,    0,    0,  866,
  866,  866,  866,    0,    0,    0,    0,    0,    0,    0,
    0,  837,  872,    0,  837,  837,    0,    0,    0,    0,
    0,    0,    0,    0,  837,    0,  872,    0,    0,    0,
    0,  872,  872,  866,    0,  872,    0,    0,    0,    0,
    0,  837,  872,    0,  872,  872,    0,    0,  872,    0,
    0,    0,  872,    0,    0,  872,    0,  872,    0,  872,
    0,    0,    0,  837,  872,  872,    0,  872,  837,    0,
    0,    0,    0,    0,   77,    0,  107,    0,    0,    0,
  837,    0,  837,  872,  872,    0,  872,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  872,    0,  872,  872,  872,  108,  109,  872,  110,    0,
  872,  872,    0,    0,    0,  111,  112,  113,  114,  115,
  872,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,  872,    0,    0,  872,  124,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  801,  837,
    0,    0,    0,  872,    0,    0,    0,    0,    0,  872,
  872,    0,    0,    0,  872,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  872,  872,    0,  872,    0,
    0,    0,  667,    0,  872,    0,    0,    0,    0,  801,
    0,    0,  801,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  866,    0,  866,  866,    0,  801,    0,    0,
    0,    0,  866,  872,  872,    0,  872,    0,    0,    0,
    0,    0,    0,  872,  872,  872,  872,  872,  866,  872,
    0,    0,  872,  872,  872,  872,  872,    0,    0,    0,
  872,  872,  866,    0,  865,  872,  872,  866,  866,    0,
    0,  866,    0,    0,    0,    0,    0,    0,  866,    0,
  866,  866,    0,    0,  866,    0,    0,    0,  866,    0,
  525,  866,    0,  866,    0,  866,    0,    0,    0,    0,
  866,  866,    0,  866,    0,  865,    0,  865,  865,  865,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  866,
  866,    0,  866,  865,  865,  865,  865,    0,    0,    0,
    0,  525,    0,    0,  525,    0,  866,    0,  866,  866,
  866,    0,  670,  866,    0,    0,  866,  866,    0,  525,
    0,  525,    0,    0,    0,    0,  866,  865,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  866,  670,  670,  866,  670,    0,  107,    0,    0,    0,
    0,  670,  670,  670,  670,  670,    0,  670,    0,  866,
  670,  670,  670,  670,  670,  866,  866,    0,  670,  670,
  866,    0,    0,    0,  670,  108,  109,    0,  110,    0,
    0,  866,  866,    0,  866,  111,  112,  113,  114,  115,
  866,  116,    0,    0,  117,  118,  119,  120,  121,    0,
  801,    0,  122,  123,    0,    0,    0,    0,  124,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  866,
  866,    0,  866,    0,    0,    0,    0,    0,    0,  866,
  866,  866,  866,  866,    0,  866,    0,    0,  866,  866,
  866,  866,  866,    0,    0,    0,  866,  866,    0,    0,
  867,  866,  866,    0,    0,    0,  801,    0,    0,  801,
    0,    0,    0,    0,    0,    0,  801,    0,    0,  801,
    0,    0,    0,  801,    0,    0,    0,    0,  801,    0,
    0,  801,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  867,    0,  867,  867,  867,  865,  801,  865,  865,
    0,    0,    0,    0,    0,    0,  865,    0,    0,  867,
  867,  867,  867,    0,    0,    0,    0,    0,  525,    0,
    0,  801,  865,    0,  801,  801,    0,    0,    0,    0,
    0,    0,    0,    0,  801,    0,  865,  525,  525,    0,
  525,  865,  865,  867,  525,  865,    0,  525,    0,    0,
    0,  801,  865,    0,  865,  865,    0,    0,  865,    0,
  525,    0,  865,    0,    0,  865,  525,  865,    0,  865,
    0,    0,    0,  801,  865,  865,    0,  865,  801,    0,
    0,    0,    0,    0,    0,    0,    0,  802,    0,    0,
  801,    0,  801,  865,  865,    0,  865,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  865,    0,  865,  865,  865,    0,    0,  865,    0,    0,
  865,  865,    0,    0,    0,    0,    0,    0,  802,    0,
  865,  802,    0,    0,    0,    0,    0,    0,  525,    0,
    0,    0,    0,    0,  865,    0,  802,  865,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  801,
  525,    0,    0,  865,    0,    0,    0,  525,    0,  865,
  865,    0,    0,    0,  865,    0,    0,    0,  525,    0,
    0,    0,    0,    0,    0,  865,  865,    0,  865,    0,
    0,    0,    0,    0,  865,    0,  525,    0,    0,    0,
    0,  525,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  867,    0,  867,  867,    0,    0,    0,    0,
    0,    0,  867,  865,  865,    0,  865,    0,    0,    0,
  525,    0,    0,  865,  865,  865,  865,  865,  867,  865,
    0,    0,  865,  865,  865,  865,  865,    0,    0,    0,
  865,  865,  867,    0,  870,  865,  865,  867,  867,    0,
    0,  867,    0,    0,    0,    0,    0,    0,  867,    0,
  867,  867,    0,    0,  867,    0,    0,    0,  867,    0,
    0,  867,    0,  867,    0,  867,    0,    0,    0,    0,
  867,  867,    0,  867,    0,  870,    0,  870,  870,  870,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  867,
  867,    0,  867,  870,  870,  870,  870,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  867,    0,  867,  867,
  867,    0,    0,  867,  670,    0,  867,  867,    0,    0,
    0,    0,    0,    0,    0,    0,  867,  870,    0,  802,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  867,    0,    0,  867,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  670,  670,    0,    0,  670,  867,
    0,    0,    0,    0,    0,  867,  867,    0,    0,    0,
  867,    0,    0,  670,    0,    0,    0,    0,    0,    0,
    0,  867,  867,    0,  867,  802,    0,    0,  802,    0,
  867,    0,    0,    0,    0,  802,    0,    0,  802,    0,
    0,    0,  802,    0,    0,    0,    0,  802,    0,    0,
  802,    0,    0,    0,    0,    0,    0,    0,    0,  867,
  867,    0,  867,    0,    0,    0,  802,    0,    0,  867,
  867,  867,  867,  867,    0,  867,    0,    0,  867,  867,
  867,  867,  867,    0,    0,    0,  867,  867,    0,    0,
  802,  867,  867,  802,  802,    0,    0,  204,    0,  687,
    0,  204,    0,  802,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  204,    0,    0,    0,  204,
  802,    0,  204,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  870,    0,  870,  870,
  687,    0,  802,  687,    0,    0,  870,  802,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  687,  802,
    0,  802,  870,  204,    0,    0,    0,    0,    0,  204,
  204,  204,    0,  204,  204,  204,  870,  204,    0,    0,
    0,  870,  870,  803,    0,  870,    0,    0,    0,    0,
    0,    0,  870,    0,  870,  870,    0,  204,  870,  204,
    0,    0,  870,    0,    0,  870,    0,  870,    0,  870,
    0,    0,    0,    0,  870,  870,    0,  870,    0,    0,
    0,    0,    0,    0,  803,    0,    0,  803,  802,    0,
  204,    0,    0,  870,  870,   81,  870,    0,    0,    0,
    0,    0,  803,    0,    0,    0,    0,  204,    0,    0,
  870,    0,  870,  870,  870,    0,    0,  870,    0,    0,
  870,  870,    0,    0,    0,    0,    0,    0,    0,  204,
  870,    0,  670,    0,  557,  670,    0,    0,    0,    0,
    0,    0,  670,    0,  870,  670,    0,  870,    0,  670,
    0,    0,    0,    0,  670,    0,    0,  670,    0,    0,
    0,    0,    0,  870,  204,  204, 1582,    0,    0,  870,
  870,    0,    0,  670,  870,  557,    0,    0,  557,    0,
    0,  204,    0,    0,    0,  870,  870,    0,  870,    0,
    0,    0,    0,  557,  870,  557,    0,  670,    0,    0,
    0,  670,    0,    0,    0,    0,    0,    0,    0,    0,
  670,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  870,  870,  557,  870,  670,    0,    0,
    0,    0,    0,  870,  870,  870,  870,  870,    0,  870,
    0,    0,  870,  870,  870,  870,  870,    0,    0,    0,
  870,  870,    0,    0,  670,  870,  870,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  670,    0,  670,    0,
    0,    0,    0,    0,  670,    0,    0,  687,    0,    0,
  687,    0,    0,    0,    0,    0,    0,  687,    0,    0,
  687,    0,    0,    0,  687,    0,    0,    0,    0,  687,
    0,    0,  687,  670,  670,  803,  670,  845,    0,    0,
    0,    0,    0,  670,  670,  670,  670,  670,  687,  670,
    0,    0,  670,  670,  670,  670,  670,    0,    0,    0,
  670,  670,    0,    0,    0,  670,  670,    0,    0,    0,
    0,    0,  687,    0,    0,    0,  687,    0,  845, 1011,
 1011,  845, 1011, 1011, 1011,  687,    0,    0,    0,    0,
    0,  803,    0,    0,  803,    0,  845, 1011, 1011, 1011,
    0,  803,  687,    0,  803,    0,    0,    0,  803,    0,
    0,    0,    0,  803,    0,    0,  803,    0,    0,    0,
    0,    0,  557,    0,    0,    0,    0,    0, 1011,  687,
    0,    0,  803,    0,    0,    0,    0,    0,  557,    0,
    0,  687,  557,  687,  557,    0,    0,    0,  557,  670,
    0,  557,    0,    0,    0,    0,  803,    0,    0,  803,
  803,    0,    0,    0,  557,    0,    0,    0,    0,  803,
  557,    0,    0,  557,  846,    0,    0,    0,  670,  670,
    0,  670,    0,    0,    0,    0,  803,    0,  670,  670,
  670,  670,  670,    0,  670,    0,    0,  670,  670,  670,
  670,  670,    0,    0,    0,  670,  670,    0,  803,    0,
  687,  670,    0,  803,    0,  846, 1012, 1012,  846, 1012,
 1012, 1012,    0,    0,    0,  803,    0,  803,    0,    0,
    0,    0,    0,  846, 1012, 1012, 1012,    0,    0,    0,
    0,    0,  557,    0,    0,    0,    0,    0,  836,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  557, 1012,    0,    0,    0,    0,
    0,  557,    0,    0,    0,  557,    0,    0,    0,    0,
    0,    0,  557,    0,    0,    0,    0,    0,    0,  836,
    0,    0,  836,    0,  803,    0,    0,  107,    0,  845,
  557,    0,    0,    0,    0,  557,    0,  836,    0, 1011,
    0,    0,    0,    0,    0,    0,    0, 1011,    0,    0,
    0,    0, 1011,    0,    0, 1011,  108,  109,    0,  110,
  557,    0,    0,    0,  557,    0,  111,  112,  113,  114,
  115,    0,  116,    0,  847,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,  845,    0,    0,  845,  124,
    0, 1011,    0,    0,    0,  845,    0,    0,  845,    0,
 1011,    0,  845,    0,    0,    0,    0,  845, 1011,    0,
  845,    0,    0,    0,    0,  847, 1013, 1013,  847, 1013,
 1013, 1013,    0,    0,    0,    0,  845, 1011,    0, 1011,
    0,    0,    0,  847, 1013, 1013, 1013,    0,  830,    0,
    0,    0,    0, 1011,    0, 1011, 1011, 1011,    0,    0,
  845,    0,    0,  845,  845,    0,    0,    0,    0,    0,
    0,    0,    0,  845,    0, 1013,  846,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1012,    0,    0,  830,
  845,    0,  830,    0, 1012,    0,    0,    0,    0, 1012,
    0,    0, 1012,    0,    0,    0, 1011,  830,    0,    0,
    0,    0,  845,    0,    0,    0,    0,  845,    0,    0,
    0,    0,    0,    0,    0,  838,    0,    0,    0,  845,
    0,  845,  846,    0,    0,  846,  839,    0, 1012,    0,
    0,    0,  846,    0,    0,  846,    0, 1012,    0,  846,
  836,    0,    0,    0,  846, 1012,    0,  846,    0,    0,
    0,    0,    0,    0,    0,    0,  838,    0,    0,  838,
    0,    0,    0,  846, 1012,    0, 1012,  839,    0,    0,
  839,    0,    0,    0,  838,    0,    0,    0,    0,    0,
 1012,    0, 1012, 1012, 1012,  839,    0,  846,  845,    0,
  846,  846,    0,    0,    0,    0,  836,    0,    0,  836,
  846,    0,    0,    0,    0,    0,  836,    0,    0,  836,
    0,    0,    0,  836,    0,    0,    0,  846,  836,    0,
    0,  836,    0,    0,    0,  827,    0,    0,    0,    0,
    0,    0,    0, 1012,    0,    0,  847,  836,    0,  846,
    0,    0,    0,    0,  846,    0, 1013,    0,    0,    0,
    0,    0,    0,    0, 1013,    0,  846,    0,  846, 1013,
    0,  836, 1013,    0,  836,  836,  827,  831,    0,  827,
    0,    0,    0,    0,  836,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  827,    0,    0,    0,    0,    0,
    0,  836,  847,    0,    0,  847,    0,    0, 1013,    0,
  830,    0,  847,    0,    0,  847,    0, 1013,  831,  847,
    0,  831,    0,  836,  847, 1013,    0,  847,  836,    0,
    0,    0,    0,    0,    0,  846,  831,  840,    0,    0,
  836,    0,  836,  847, 1013,    0, 1013,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1013,    0, 1013, 1013, 1013,    0,  830,  847,    0,  830,
  847,  847,    0,    0,    0,    0,  830,    0,  840,  830,
  847,  840,    0,  830,    0,    0,    0,  838,  830,    0,
    0,  830,    0,    0,    0,    0,  840,  847,  839,    0,
    0,    0,    0,    0,    0,    0,    0,  830,    0,  836,
    0,    0,    0, 1013,  828,    0,    0,    0,    0,  847,
    0,    0,    0,    0,  847,    0,    0,    0,    0,    0,
    0,  830,    0,    0,  830,  830,  847,    0,  847,    0,
    0,    0,    0,  838,  830,    0,  838,    0,    0,    0,
    0,    0,    0,  838,  839,  828,  838,  839,  828,    0,
  838,  830,    0,    0,  839,  838,    0,  839,  838,    0,
    0,  839,    0,  828,  832,    0,  839,    0,    0,  839,
    0,    0,    0,  830,  838,    0,    0,    0,  830,    0,
    0,    0,    0,    0,    0,  839,    0,  827,    0,    0,
  830,    0,  830,    0,    0,  847,    0,    0,  838,    0,
    0,  838,  838,    0,    0,  832,    0,    0,  832,  839,
    0,  838,  839,  839,    0,    0,    0,    0,    0,    0,
    0,    0,  839,  832, 1255,    0,    0,    0,  838,  831,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  839,
    0,    0,    0,  827,    0,    0,  827,    0,    0,    0,
  838,    0,    0,  827,    0,  838,  827,    0,    0,  830,
  827,  839,    0,    0,    0,  827,  839,  838,  827,  838,
    0,    0,    0,    0,    0,    0,    0,    0,  839,    0,
  839,    0,    0,    0,  827,  831,    0,    0,  831,  840,
    0,    0,    0,    0,    0,  831,    0,    0,  831,    0,
    0,    0,  831,    0,    0,    0,    0,  831,  827,    0,
  831,  827,  827,    0,    0,    0,    0,    0,    0,  658,
    0,  827,  429,    0,  430,    0,  831,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  838,  432,  827,    0,
    0,    0,  288,    0,    0,  840,    0,  839,  840,    0,
  831,    0,    0,  831,  831,  840,    0,    0,  840,    0,
  827,    0,  840,  831,    0,  827,  828,  840,    0,    0,
  840,    0,    0,    0,    0,    0,    0,  827,    0,  827,
  831,    0,    0,    0,    0,    0,  840,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  831,    0,    0,    0,    0,  831,    0,    0,
  840,    0,    0,  840,  840,    0,    0,    0,    0,  831,
    0,  831,  828,  840,    0,  828,  832,    0,    0,    0,
    0,    0,  828,    0,    0,  828,    0,    0,    0,  828,
  840,    0,    0,    0,  828,    0,  827,  828,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  840,  828,    0,    0,    0,  840,    0,    0,
    0,    0,    0,    0,  396,    0,    0,    0,    0,  840,
    0,  840,  832,    0,    0,  832,    0,  828,  831,    0,
  828,  828,  832,    0,    0,  832,    0,    0,    0,  832,
  828,    0,  398,    0,  832,    0,    0,  832,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  828,    0,    0,
    0,    0,    0,  832,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,  828,
    0,    0,    0,    0,  828,    0,  393,  832,  840,    0,
  832,  832,    0,    0,    0,  394,  828,    0,  828,    0,
  832,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,    0,    0,  658,  396,  397,  429,  832,  430,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  432,    0,  652,    0,    0,  288,    0,    0,  832,
    0,    0,  398,    0,  832,  399,  405,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  832,    0,  832,    0,
    0,    0,  407,    0,    0,  828,    0,    0,    0,    0,
    0,    0,    0,  401,  402,    0,  408,  409,  410,  564,
  211,  212,    0,    0,    0,  403,    0,    0,    0,  404,
    0,    0,    0,  654,    0,  654,  654,  881,  654,  413,
  414,  415,  416,  417,  418,  419,  420,  421,    0,    0,
    0,  654,    0,    0,  423,  424,  654,    0,    0,    0,
    0,    0,    0,  428,    0,  832,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  695,    0,    0,    0,
  654,    0,    0,    0,    0,    0,  405,    0,    0,    0,
    0,    0,  406,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  408,  409,  410,  411,
  211,  212,    0,    0,    0,    0,    0,    0,    0,    0,
  412,    0,    0,    0,    0,    0,    0,    0,    0,  413,
  414,  415,  416,  417,  418,  419,  420,  421,  108,  422,
    0,  110,    0,    0,  423,  424,    0,  425,  426,  427,
  113,  114,  115,  428,  116,    0,    0,  117,  118,  119,
  120,  121,  882,  883,  884,  122,  123,    0,    0,    0,
  393,  124,    0,    0,    0,    0,    0,    0,    0,  394,
    0,    0,  107,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,  651,    0,    0,  396,  397,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  108,  109,    0,  110,    0,    0,  652,    0,    0,
    0,  111,  112,  113,  114,  115,  398,  116,    0,  399,
  117,  118,  119,  120,  121,    0,    0,    0,  122,  123,
    0,    0,    0,  654,  124,    0,    0,    0,    0,    0,
  654,    0,    0,    0,    0,    0,    0,  401,  402,  654,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  403,
    0,    0,  654,  404,    0,    0,    0,    0,  654,  654,
    0,  653,  648,    0,  648,  648,  654,  648,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  648,    0,    0,    0,    0,  648,  654,    0,    0,  654,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  654,    0,    0,    0,    0,    0,
  405,    0,    0,    0,    0,  654,  406,  654,  654,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,  654,
    0,    0,    0,  654,    0,    0,    0,    0,    0,    0,
  408,  409,  410,  411,  211,  212,    0,    0,    0,    0,
    0,    0,    0,    0,  412,    0,    0,    0,    0,    0,
    0,    0,    0,  413,  414,  415,  416,  417,  418,  419,
  420,  421,  108,  422,    0,  110,    0,    0,  423,  424,
    0,  425,  426,  427,  113,  114,  115,  428,  116,    0,
  654,  117,  118,  655,  656,  657,  654,  570,    0,  122,
  123,    0,    0,    0,    0,  124,  654,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  654,  654,  654,  654,  654,  654,    0,    0,    0,    0,
    0,    0,    0,    0,  654,    0,    0,    0,    0,    0,
    0,    0,    0,  654,  654,  654,  654,  654,  654,  654,
  654,  654,  654,  654,    0,  654,    0,    0,  654,  654,
    0,  654,  654,  654,  654,  654,  654,  654,  654,    0,
    0,  654,  654,  654,  654,  654,    0,    0,    0,  654,
  654,    0,  648,    0,    0,  654,    0,    0,    0,  648,
    0,    0,  658,    0,    0,  429,    0,  430,  648,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  432,  648,    0,    0,    0,  288,    0,  648,  648,    0,
    0,    0,    0,    0,    0,  648,    0,    0,    0,    0,
    0,    0,  939,  706,  704,    0,  705,    0,  707,    0,
    0,    0,    0,    0,    0,  648,    0,    0,  648,    0,
    0,  902,  901,  903,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  107,    0,    0,    0,  648,    0,  648,  648,  658,    0,
    0,  429,    0,  430,    0,    0,    0,    0,  648,    0,
    0,    0,  648,    0,    0,    0,  432,    0,    0,  108,
  109,  288,  110,    0,    0,    0,    0,    0,    0,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  648,
    0,    0,    0,    0,    0,  648,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  648,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  648,
  648,  648,  648,  648,  648,    0,    0,    0,    0,    0,
    0,    0,    0,  648,    0,    0,    0,    0,    0,    0,
    0,    0,  648,  648,  648,  648,  648,  648,  648,  648,
  648,  648,  648,    0,  648,    0,    0,  648,  648,  393,
  648,  648,  648,  648,  648,  648,  648,  648,  394,    0,
  648,  648,  648,  648,  648,    0,    0,    0,  648,  648,
    0,  395,    0,    0,  648,    0,    0,  396,  397,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  889,    0,    0,  652,    0,    0,    0,
    0,  702,    0,    0,    0,  398,  703,    0,  399,  890,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,  401,  402,  658,    0,
    0,  429,    0,  430,  394,  891,    0,    0,  403,    0,
    0,    0,  404,    0,  892,    0,  432,  395,    0,    0,
  877,  288,  893,  396,  397,    0,    0,  706,  704,    0,
  705,    0,  707,    0,    0,    0,    0,    0,    0,    0,
    0,  894,  652,  895,    0,  902,  901,  903,    0,    0,
    0,  398,    0,    0,  399,    0,    0,  896,    0,  897,
  898,  899,    0,  654,    0,    0,    0,    0,    0,  405,
    0,    0,    0,    0,    0,  406,    0,    0,    0,    0,
    0,    0,  401,  402,    0,  407,    0,    0,    0,    0,
    0,    0,    0,    0,  403,    0,    0,    0,  404,  408,
  409,  410,  411,  211,  212,    0,  653,    0,    0,    0,
  900,    0,    0,  412,    0,    0,    0,    0,    0,    0,
    0,    0,  413,  414,  415,  416,  417,  418,  419,  420,
  421,  108,  422,    0,  110,    0,    0,  423,  424,    0,
  425,  426,  427,  113,  114,  115,  428,  116,    0,  654,
  117,  118,  655,  656,  657,  405,    0,    0,  122,  123,
    0,  406,    0,    0,  124,    0,    0,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  408,  409,  410,  411,  211,
  212,    0,    0,    0,    0,    0,    0,    0,    0,  412,
    0,    0,    0,    0,    0,    0,    0,    0,  413,  414,
  415,  416,  417,  418,  419,  420,  421,  108,  422,    0,
  110,    0,  476,  423,  424,  393,  425,  426,  427,  113,
  114,  115,  428,  116,  394,    0,  117,  118,  655,  656,
  657,    0,    0,    0,  122,  123,    0,  395,  107,    0,
  124,    0,    0,  396,  397,    0,  500,  889,    0,  429,
    0,  430,    0,    0,    0,  702,    0,    0,    0,    0,
  703,    0,  652,  890,  432,    0,    0,  108,  109,  288,
  110,  398,    0,    0,  399,    0,    0,  111,  112,  113,
  114,  115,    0,  116,    0,    0,  117,  118,  119,  120,
  121,    0,    0,    0,  122,  123,    0,    0,    0,  891,
  124,    0,  401,  402,    0,    0,    0,    0,  892,    0,
    0,    0,    0,    0,  403,    0,  893,    0,  404,    0,
    0,    0,    0,    0,    0,    0,  877,    0,    0,    0,
    0,    0,    0,    0,    0,  894,    0,  895,    0,    0,
    0,    0,    0,    0,  500,    0,    0,  429,    0,  430,
    0,  896,    0,  897,  898,  899,    0,    0,    0,    0,
    0,    0,  432,    0,  966,    0,    0,  288,    0,  654,
    0,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,  406,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  900,  408,  409,  410,  411,  211,
  212,    0,    0,    0,    0,    0,    0,    0,    0,  412,
    0,    0,    0,    0,    0,    0,    0,    0,  413,  414,
  415,  416,  417,  418,  419,  420,  421,  108,  422,    0,
  110,    0,    0,  423,  424,    0,  425,  426,  427,  113,
  114,  115,  428,  116,    0,    0,  117,  118,  119,  120,
  121,    0,    0,    0,  122,  123, 1124,    0,    0, 1125,
  124,    0,  530,  393,    0,    0,    0,    0,    0,  969,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  395,  107,    0,    0,    0,
    0,  396,  397,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  530,    0,    0,  530,    0,    0,    0,
    0,    0,    0,    0,    0,  108,  109,    0,  110,  398,
    0,  530,  399,  530,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,    0,
  401,  402,    0,    0, 1134,    0,    0, 1125,    0,    0,
    0,  393,  403,    0,    0,    0,  404,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,  396,
  397,    0,  500,    0,    0,  429,    0,  430,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,  288,    0,  398,    0,    0,
  399, 1126,    0,  405,    0,    0,    0,    0,    0,  406,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  407,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
    0,    0,    0,  408,  409,  410,  411,  211,  212,    0,
  403,    0,    0,    0,  404,    0,    0,  412,    0,    0,
    0,    0,    0,    0,    0,    0,  413,  414,  415,  416,
  417,  418,  419,  420,  421,  108,  422,    0,  110,    0,
    0,  423,  424,    0,  425,  426,  427,  113,  114,  115,
  428,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124, 1126,
  530,  405,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  407,    0,    0,
  530,    0,  530,    0,    0,    0,  530,    0,    0,  530,
    0,  408,  409,  410,  411,  211,  212,    0,    0,    0,
    0,    0,  530,    0,    0,  412,    0,    0,  530,    0,
    0,    0,    0,    0,  413,  414,  415,  416,  417,  418,
  419,  420,  421,  108,  422,    0,  110,    0,    0,  423,
  424,    0,  425,  426,  427,  113,  114,  115,  428,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,    0,
  122,  123, 1138,    0,    0, 1125,  124,    0,    0,  393,
    0,    0,    0,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  530,  395,    0,    0,    0,    0,    0,  396,  397,    0,
  500,    0,    0,  429,    0,  430,    0,    0,    0,    0,
    0,    0,  530,    0,    0,    0,    0,    0,  432,  530,
    0,    0,    0,  288,    0,  398,    0,    0,  399,    0,
  530,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  530,    0,
    0,    0,    0,  530,    0,    0,  401,  402,    0,    0,
    0,    0,    0,    0,    0,    0,  867,    0,  403,    0,
  596,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  530,    0,  868,    0,    0,    0,  597,    0,
    0,  598,    0,    0,    0,    0,    0,    0,  500,    0,
    0,  429,    0,  430,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,  288,    0,    0,    0,    0,    0, 1126,    0,  405,
    0,    0,  599,    0,    0,  406,    0,    0,  600,  601,
  602,    0,  603,  604,  605,  407,  606,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  408,
  409,  410,  411,  211,  212,    0,  607,    0,  608,    0,
    0,    0,    0,  412,    0,    0,    0,    0,    0,    0,
    0,    0,  413,  414,  415,  416,  417,  418,  419,  420,
  421,  108,  422,    0,  110,    0,    0,  423,  424,  609,
  425,  426,  427,  113,  114,  115,  428,  116,    0,    0,
  117,  118,  119,  120,  121,    0,  610,    0,  122,  123,
 1146,    0,    0, 1125,  124,    0,    0,  393,    0,    0,
    0,    0,    0,    0,    0,    0,  394,    0,  611,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,  396,  397,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  204,    0,    0,    0,
  204,    0,    0,  612,  613,    0,    0,    0,    0,    0,
    0,    0,    0,  398,  204,    0,  399,    0,  204,    0,
  614,  204,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  401,  402,    0,    0, 1149,    0,
    0, 1125,    0,    0,    0,  393,  403,    0,    0,    0,
  404,    0,  204,    0,  394,    0,    0,    0,  204,  204,
  204,    0,  204,  204,  204,    0,  204,  395,    0,    0,
    0,    0,    0,  396,  397,    0,  500,    0,    0,  429,
    0,  430,    0,    0,    0,    0,  204,    0,  204,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,  288,
    0,  398,    0,    0,  399, 1126,    0,  405,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  204,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,    0,    0,  204,  408,  409,  410,
  411,  211,  212,    0,  403,    0,    0,    0,  404,    0,
    0,  412,    0,    0,    0,    0,    0,    0,  204,    0,
  413,  414,  415,  416,  417,  418,  419,  420,  421,  108,
  422,    0,  110,    0,    0,  423,  424,    0,  425,  426,
  427,  113,  114,  115,  428,  116,    0,    0,  117,  118,
  119,  120,  121,  204,  204,    0,  122,  123,    0,    0,
    0,    0,  124, 1126,    0,  405,    0,    0,    0, 1717,
  204,  406,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1718,  408,  409,  410,  411,  211,
  212,    0,    0,    0,    0,    0,    0,    0,    0,  412,
    0,    0,    0,    0,    0,    0,    0,    0,  413,  414,
  415,  416,  417,  418,  419,  420,  421,  108,  422,    0,
  110,    0,    0,  423,  424,    0,  425,  426,  427,  113,
  114,  115,  428,  116,    0,    0,  117,  118,  119,  120,
  121,    0, 1719,    0,  122,  123, 1154,    0,    0, 1125,
  124,    0,    0,  393,    0,    0,    0,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  395,  107,    0,    0,    0,
    0,  396,  397,    0,  500,    0,    0,  429,    0,  430,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  432,    0,    0,  108,  109,  288,  110,  398,
    0,    0,  399,    0,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,    0,
  401,  402,    0,    0,    0,    0,    0,    0,    0,    0,
  978,    0,  403,    0,  978,    0,  404,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  980,    0,  978,    0,    0,  978,    0,    0,    0,    0,
    0,    0,  500,    0,    0,  429,    0,  430,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,  288,    0,    0,    0,    0,
    0, 1126,    0,  405,    0,    0,  978,    0,    0,  406,
    0,    0,  978,  978,  978,    0,  978,  978,  978,  407,
  978,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  408,  409,  410,  411,  211,  212,    0,
  978,    0,  978,    0,    0,    0,    0,  412,    0,    0,
    0,    0,    0,    0,    0,    0,  413,  414,  415,  416,
  417,  418,  419,  420,  421,  108,  422,    0,  110,    0,
    0,  423,  424,  978,  425,  426,  427,  113,  114,  115,
  428,  116,    0,    0,  117,  118,  119,  120,  121,    0,
  978,    0,  122,  123, 1157,    0,    0, 1125,  124,    0,
    0,  393,    0,  595,    0,    0,    0,  596,    0,    0,
  394,    0,  978,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,  597,    0,    0,  598,  396,
  397,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  978,  978,    0,
    0,    0,    0,    0,    0,    0,    0,  398,    0,    0,
  399,    0,    0,    0,  978,    0,    0,    0,    0,  599,
    0,    0,    0,    0,    0,  600,  601,  602,    0,  603,
  604,  605,    0,  606,    0,    0,    0,    0,  401,  402,
    0,    0, 1160,    0,    0, 1125,    0,    0,    0,  393,
  403,    0,    0,  607,  404,  608,    0,    0,  394,    0,
    0,  500,    0,  505,  429,    0,  430,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,  396,  397,  432,
    0,    0,    0,    0,  288,    0,  609,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  610,    0,  398,    0,    0,  399, 1126,
    0,  405,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0,  611,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,  401,  402,    0,    0,
    0,  408,  409,  410,  411,  211,  212,    0,  403,    0,
    0,    0,  404,    0,    0,  412,    0,    0,    0,    0,
  612,  613,    0,    0,  413,  414,  415,  416,  417,  418,
  419,  420,  421,  108,  422,    0,  110,  614,  100,  423,
  424,    0,  425,  426,  427,  113,  114,  115,  428,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,    0,
  122,  123,    0,    0,    0,    0,  124, 1126,    0,  405,
    0,    0,    0,    0,    0,  406,    0,    0,    0,  101,
    0,    0,    0,    0,    0,  407,    0,    0,    0,    0,
    0,  102,    0,    0,    0,    0,    0,    0,    0,  408,
  409,  410,  411,  211,  212,  137,    0,    0,    0,    0,
    0,    0,    0,  412,    0,    0,  103,    0,    0,    0,
    0,    0,  413,  414,  415,  416,  417,  418,  419,  420,
  421,  108,  422,    0,  110,    0,    0,  423,  424,    0,
  425,  426,  427,  113,  114,  115,  428,  116,  393,    0,
  117,  118,  119,  120,  121,    0,    0,  394,  122,  123,
  500,    0,    0,  429,  124,  430,    0,    0,    0,    0,
  395,    0,    0,    0,    0,    0,  396,  397,  432,    0,
    0,    0,    0,  288,    0,  104,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,  398,    0,    0,  399,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  106,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,  401,  402,    0,  107,    0,
  972,    0,  972,  972,    0,  972,    0,  403,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,  972,    0,
    0,    0,    0,  972,    0,    0,    0,  108,  109,    0,
  110,    0,    0,    0,    0,    0,    0,  111,  112,  113,
  114,  115,    0,  116,    0,    0,  117,  118,  119,  120,
  121,    0,    0,    0,  122,  123,    0,    0,    0,    0,
  124,    0,    0,    0,    0,    0,    0,    0,  405,    0,
    0,    0,    0,    0,  406,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  408,  409,
  410,  411,  211,  212,    0,    0,    0,  966,    0,    0,
    0,    0,  412,    0,    0,    0,    0,    0,    0,    0,
    0,  413,  414,  415,  416,  417,  418,  419,  420,  421,
  108,  422,    0,  110,    0,    0,  423,  424,    0,  425,
  426,  427,  113,  114,  115,  428,  116,  393,  107,  117,
  118,  119,  120,  121,    0,    0,  394,  122,  123,    0,
    0,    0,    0,  124,    0,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,  396,  397,  108,  109,    0,
  110,  967,    0,    0,    0,    0,    0,  111,  112,  113,
  114,  115,    0,  116,    0,    0,  117,  118,  119,  120,
  121,    0,    0,  398,  122,  123,  399,    0,    0,    0,
  124,    0,    0,    0,    0,    0,    0,  968,    0,    0,
    0,    0,  969,    0,    0,    0,    0,  972,  970,    0,
    0,    0,  400,    0,  401,  402,  972,    0,    0,  107,
    0,    0,    0,    0,    0,    0,  403,  971,    0,  972,
  404,    0,    0,    0,  654,  972,  972,  654,    0,  654,
    0,    0,    0,    0,    0,    0,    0,    0,  108,  109,
    0,  110,  654,    0,    0,    0,    0,  654,  111,  112,
  113,  114,  115,  972,  116,    0,  972,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,  695,    0,    0,
    0,  124,    0,    0,    0,    0,    0,  405,    0,    0,
    0,    0,  972,  406,  972,  972,    0,    0,    0,    0,
    0,    0,    0,  407,    0,    0,  972,    0,    0,    0,
  972,    0,    0,    0,    0,    0,    0,  408,  409,  410,
  411,  211,  212,    0,    0,    0,    0,    0,    0,    0,
    0,  412,    0,    0,    0,    0,    0,  100,    0,    0,
  413,  414,  415,  416,  417,  418,  419,  420,  421,  108,
  422,    0,  110,    0,    0,  423,  424,    0,  425,  426,
  427,  113,  114,  115,  428,  116,    0,  972,  117,  118,
  119,  120,  121,  972,    0,    0,  122,  123,  101,    0,
    0,    0,  124,  972,    0,    0,    0,    0,    0,    0,
  102,    0,    0,    0,    0,    0,    0,  972,  972,  972,
  972,  972,  972,    0,    0,    0,    0,    0,    0,    0,
    0,  972,    0,    0,    0,  103,    0,    0,    0,    0,
  972,  972,  972,  972,  972,  972,  972,  972,  972,  972,
  972,    0,  972,    0,    0,  972,  972,    0,  972,  972,
  972,  972,  972,  972,  972,  972,    0,    0,  972,  972,
  972,  972,  972,    0,  654,    0,  972,  972,    0,    0,
    0,  654,  972,    0,  431,    0,    0,  429,    0,  430,
  654,    0,    0,  573,    0,    0,    0,    0,    0,    0,
    0,    0,  432,  654,  104,    0,    0,  288,    0,  654,
  654,    0,    0,    0,  105,    0,    0,  654,    0,    0,
    0,    0,  573,  573,    0,  573,    0,    0,    0,    0,
    0,    0,  573,  573,  573,  573,  573,  654,  573,  106,
  654,  573,  573,  573,  573,  573,    0,    0,    0,  573,
  573,    0,    0,    0,    0,  573,    0,  107,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  654,  654,
  500,    0,    0,  429,    0,  430,    0,  480,    0,    0,
  654,    0,    0,    0,  654,    0,  108,  109,  432,  110,
    0,    0,    0,  288,  107,    0,  111,  112,  113,  114,
  115,    0,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,    0,  124,
    0,    0,    0,  108,  109,    0,  110,    0,    0,    0,
    0,    0,    0,  111,  112,  113,  114,  115,    0,  116,
    0,  654,  117,  118,  119,  120,  121,  654,    0,    0,
  122,  123,    0,    0,    0,    0,  124,  654,    0,    0,
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
  500,    0,    0,  429,    0,  430,  394,    0,    0,    0,
  403,    0,    0,    0,  404,    0,  134,    0,  432,  395,
    0,    0,    0,  288,    0,  396,  397,    0,    0,    0,
    0,    0,    0,  134,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  398,    0,  134,  399,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  405,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,    0,  401,  402,    0,  407,    0,    0,
  134,  134,    0,    0,    0,    0,  403,    0,    0,    0,
  404,  408,  409,  410,  411,  211,  212,  134,    0,    0,
    0,    0,    0,    0,    0,  412,    0,    0,    0,    0,
    0,    0,    0,    0,  413,  414,  415,  416,  417,  418,
  419,  420,  421,  108,  422, 1200,  110,    0,    0,  423,
  424,    0,  425,  426,  427,  113,  114,  115,  428,  116,
    0,    0,  117,  118,  119,  120,  121,  405,    0,    0,
  122,  123,    0,  406,    0,    6,  124,    0,    0,    0,
    0,    0,    0,  407,    0,    0, 1399, 1400,    0, 1401,
    0,  687,    0,    0,    0,    0,    0,  408,  409,  410,
  411,  211,  212,    0,    0,    0, 1402,    0,    0,    0,
    0,  412, 1403,    0,    0,    0,    0,    0,    0,    0,
  413,  414,  415,  416,  417,  418,  419,  420,  421,  108,
  422,    0,  110,    0,    0,  423,  424,  393,  425,  426,
  427,  113,  114,  115,  428,  116,  394,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,  500,  395,
    0,  429,  124,  430,    0,  396,  397,    0,    0,    0,
  273,    0, 1404,    0,    0,    0,  432,    0,    0,    0,
    0,  288,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   14,    0,  398,    0,    0,  399,    0,    0,  273,
  273,    0,  273,    0,    0,    0, 1405,    0,    0,  273,
  273,  273,  273,  273,    0,  273,   16,    0,  273,  273,
  273,  273,  273,    0,  401,  402,  273,  273,    0,    0,
    0,    0,  273,    0,  107,    0,  403,    0,  500,  942,
  404,  429,    0,  430,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,  288,    0, 1406,  109,    0,  110,    0,    0,    0,
    0,    0,    0,  111,  112,  113,  114,  115,    0,  116,
    0,    0,  117, 1407,  119,  120,  121,  695,    0,    0,
  122,  123, 1408, 1409, 1410,    0,  124,  405,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,  409,  410,
  411,  211,  212,    0,    0,    0,    0,    0,    0,    0,
    0,  412,    0,    0,    0,    0,    0,    0,    0,    0,
  413,  414,  415,  416,  417,  418,  419,  420,  421,  108,
  422,    0,  110,    0,    0,  423,  424,    0,  425,  426,
  427,  113,  114,  115,  428,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,  393,  122,  123,    0,    0,
    0,    0,  124,    0,  394,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,  396,  397,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  398,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,    0,    0,    0,    0,
  400,    0,  401,  402,  394,    0,    0,  500,    0,    0,
  429,    0,  430,    0,  403,    0,    0,  395,  404,    0,
    0,    0,    0,  396,  397,  432,    0,    0,    0,    0,
  288,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  398,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,  406,  401,  402,    0,    0,    0,    0,    0,    0,
    0,  407,    0,    0,  403,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  408,  409,  410,  411,  211,
  212,    0,    0,    0,    0,    0,    0,    0,    0,  412,
    0,    0,    0,    0,    0,    0,    0,    0,  413,  414,
  415,  416,  417,  418,  419,  420,  421,  108,  422, 1200,
  110,    0,    0,  423,  424,    0,  425,  426,  427,  113,
  114,  115,  428,  116,    0,  405,  117,  118,  119,  120,
  121,  406,    0,    0,  122,  123,    0,    0,    0,    6,
  124,  407,    0,    0,    0,    0,    0,    0,    0,    0,
 1399, 1400,    0, 1401,    0,  408,  409,  410,  411,  211,
  212,    0,    0,    0,    0,    0,    0,    0,    0,  412,
 1402,    0,    0,    0,    0,    0, 1403,    0,  413,  414,
  415,  416,  417,  418,  419,  420,  421,  108,  422,    0,
  110,    0,    0,  423,  424,    0,  425,  426,  427,  113,
  114,  115,  428,  116,  393,    0,  117,  118,  119,  120,
  121,    0,    0,  394,  122,  123,  500,    0,    0,  429,
  124,  430,    0,    0,    0,    0,  395,    0,    0,    0,
    0,    0,  396,  397,  432,    0, 1404,    0,    0,  288,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   14,    0,    0,    0,    0,
  398,    0,    0,  399,    0,    0,    0,    0,    0,    0,
 1405,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   16,    0,    0,    0,    0,    0,    0,    0,    0, 1593,
    0,  401,  402,    0,    0,    0,  500,    0,  107,  429,
    0,  430,    0,  403,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,  288,
    0,    0,    0,    0,    0,    0,    0, 1406,  109,    0,
  110,    0,    0,    0,    0,    0,    0,  111,  112,  113,
  114,  115,    0,  116,    0,    0,  117, 1407,  119,  120,
  121,    0,    0,    0,  122,  123, 1408, 1409, 1410,    0,
  124,    0,    0,    0,  405,    0,    0,    0,    0,    0,
  406,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1152,    0,    0,  408,  409,  410,  411,  211,  212,
    0,    0,    0,    0,    0,    0,    0,    0,  412,    0,
    0,    0,    0,    0,    0,    0,    0,  413,  414,  415,
  416,  417,  418,  419,  420,  421,  108,  422,    0,  110,
    0,    0,  423,  424,    0,  425,  426,  427,  113,  114,
  115,  428,  116,  393,    0,  117,  118,  119,  120,  121,
    0,    0,  394,  122,  123,    0,    0,    0,    0,  124,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,  396,  397,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  398,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  393,    0,    0,    0,    0,    0,    0,
  401,  402,  394,    0,    0,  500,    0,    0,  429,    0,
  430,    0,  403,    0,    0,  395,  404,    0,    0,    0,
    0,  396,  397,  432, 1689,    0,    0,    0,  288,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  398,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1341,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  405,    0,    0,    0,    0,    0,  406,
  401,  402,    0,    0,    0,    0,    0,    0,    0,  407,
    0,    0,  403,    0,    0,    0,  404,    0,    0,    0,
    0,    0,    0,  408,  409,  410,  411,  211,  212,    0,
    0,    0,    0,    0,    0,    0,    0,  412,    0,    0,
    0,    0,    0,    0,    0,    0,  413,  414,  415,  416,
  417,  418,  419,  420,  421,  108,  422,    0,  110,    0,
    0,  423,  424,    0,  425,  426,  427,  113,  114,  115,
  428,  116,    0,  405,  117,  118,  119,  120,  121,  406,
    0,    0,  122,  123,    0,    0,    0,    0,  124,  407,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1354,    0,    0,  408,  409,  410,  411,  211,  212,    0,
    0,    0,    0,    0,    0,    0,    0,  412,    0,    0,
    0,    0,    0,    0,    0,    0,  413,  414,  415,  416,
  417,  418,  419,  420,  421,  108,  422,    0,  110,    0,
    0,  423,  424,    0,  425,  426,  427,  113,  114,  115,
  428,  116,  393,    0,  117,  118,  119,  120,  121,    0,
    0,  394,  122,  123,  500,    0,    0,  429,  124,  430,
    0,    0,    0,    0,  395,    0,    0,    0,    0,    0,
  396,  397,  432,    0,    0,    0,    0,  288,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  398,    0,
    0,  399,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,    0,    0,    0,  972,    0,    0,  972,    0,  972,
    0,  403,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,  972,    0,    0,  107,    0,  972,    0,    0,
    0,    0,    0,    0,  167,  168,  169,  170,  171,  172,
  173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
    0,    0,  183,  184,  108,  109,    0,  110,    0,    0,
  185,  186,    0,    0,  111,  112,  113,  114,  115,  187,
  116,  188,  405,  117,  118,  119,  120,  121,  406,    0,
    0,  122,  123,  189,  190,  191,  192,  124,  407,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  408,  409,  410,  411,  211,  212,    0,    0,
    0,    0,    0,    0,    0,    0,  412,    0,    0,    0,
    0,    0,    0,    0,    0,  413,  414,  415,  416,  417,
  418,  419,  420,  421,  108,  422,    0,  110,    0,    0,
  423,  424,    0,  425,  426,  427,  113,  114,  115,  428,
  116,  393,    0,  117,  118,  119,  120,  121,    0,    0,
  394,  122,  123,    0,    0,    0,    0,  124,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,  396,
  397,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  398,    0,    0,
  399,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  540,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  972,    0,  541,    0,    0,    0,    0,  401,  402,
  972,    0,    0,    0,    0,  535,    0,    0,    0,    0,
  403,    0,    0,  972,  404,    0,    0,    0,    0,  972,
  972,  540,  540,    0,    0,  540,    0,    0,    0,    0,
    0,    0,    0,  541,  541,    0,    0,  541,    0,    0,
  540,    0,  540,    0,    0,    0,  535,  972,    0,  535,
  972,    0,  541,    0,  541,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  535,    0,  535,    0,    0,    0,
    0,  405,  540,    0,    0,    0,    0,  406,  972,  972,
    0,    0,    0,    0,  541,    0,    0,  407,    0,    0,
  972,    0,    0,    0,  972,    0,  535,    0,    0,    0,
    0,  408,  409,  410,  411,  211,  212,    0,    0,    0,
    0,    0,    0,    0,    0,  412,    0,    0,    0,    0,
    0,    0,    0,    0,  413,  414,  415,  416,  417,  418,
  419,  420,  421,  108,  422,    0,  110,    0,    0,  423,
  424,    0,  425,  426,  427,  113,  114,  115,  428,  116,
    0,  972,  117,  118,  119,  120,  121,  972,    0,    0,
  122,  123,    0,    0,    0,    0,  124,  972,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  972,  972,  972,  972,  972,  972,    0,    0,    0,
    0,    0,    0,    0,    0,  972,    0,    0,    0,    0,
    0,    0,    0,  547,  972,  972,  972,  972,  972,  972,
  972,  972,  972,  972,  972,    0,  972,    0,    0,  972,
  972,    0,  972,  972,  972,  972,  972,  972,  972,  972,
    0,    0,  972,  972,  972,  972,  972,    0,    0,  540,
  972,  972,    0,    0,  547,    0,  972,  547,    0,    0,
    0,  541,    0,    0,    0,  540,  532,    0,  540,  540,
    0,  540,  547,  535,  547,  540,    0,  541,  540,    0,
  541,  541,    0,  541,    0,    0,    0,  541,    0,  535,
  541,  540,  535,  535,    0,  535,    0,  540,    0,  535,
  540,    0,  535,  541,  547,    0,    0,  532,    0,  541,
  532,    0,  541,    0,    0,  535,    0,    0,    0,    0,
    0,  535,    0,    0,  535,  532,    0,  532,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  532,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  540,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  541,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  540,    0,  535,    0,    0,    0,    0,  540,    0,
    0,    0,  540,  541,    0,    0,    0,    0,    0,  540,
  541,    0,    0,    0,  541,  535,    0,    0,    0,    0,
    0,  541,  535,    0,    0,    0,  535,  540,    0,    0,
    0,    0,  540,  535,    0,    0,    0,    0,    0,  541,
    0,    0,    0,    0,  541,    0,    0,    0,    0,    0,
    0,  535,    0,    0,    0,    0,  535,  540,    0,    0,
    0,  540,    0,    0,    0,    0,    0,    0,    0,  541,
    0,  547,    0,  541,    0,    0,    0,    0,    0,    0,
    0,  535,    0,    0,    0,  535,    0,  547,    0,    0,
    0,  547,    0,  547,    0,    0,    0,  547,    0,    0,
  547,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  547,    0,    0,    0,    0,    0,  547,
    0,  313,  547,    0,  532,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  532,    0,    0,    0,  532,    0,  532,    0,    0,    0,
  532,  313,    0,  532,    0,    0,    0,    0,  313,    0,
    0,    0,  313,  313,    0,  313,  532,    0,  313,    0,
    0,    0,  532,    0,    0,  532,    0,    0,    0,    0,
    0,    0,  313,    0,    0,    0,    0,    0,  313,    0,
    0,  547,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  547,    0,    0,    0,    0,    0,    0,
  547,    0,    0,    0,  547,    0,    0,    0,    0,    0,
    0,  547,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  532,    0,  595,    0,  313,  547,
  596,    0,    0,    0,  547,    0,    0,  794,    0,    0,
    0,    0,    0,    0,    0,    0,  532,  313,  597,    0,
    0,  598,    0,  532,    0,    0,    0,  532,    0,  547,
    0,    0,  313,  547,  532,    0,    0,    0,    0,    0,
    0,    0,  313,    0,    0,    0,    0,    0,    0,    0,
    0,  313,  532,  313,    0,    0,    0,  532,    0,    0,
  313,    0,  599,    0,    0,    0,    0,    0,  600,  601,
  602,    0,  603,  604,  605,    0,  606,    0,    0,    0,
    0,    0,  532,    0,    0,    0,  532,    0,    0,  313,
  313,    0,  313,    0,    0,    0,  607,    0,  608,  313,
  313,  313,  313,  313,    0,  313,    0,    0,  313,  313,
  313,  313,  313,    0,    0,    0,  313,  313,  313,  313,
  313,    0,  313,    0,    0,    0,    0,    0,    0,  609,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  610,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  611,    0,
  595,    0,    0,    0,  596,    0,    0,    0,    0,    0,
    0,    0,    0,  107,    0,    0,    0,    0,    0,    0,
    0,    0,  597,    0,    0,  598,    0,    0,    0,    0,
    0,    0,    0,  612,  613,    0,    0,    0,    0,    0,
    0,    0,  108,  109,    0,  110,    0,    0,    0,    0,
  614,    0,  111,  112,  113,  114,  115,    0,  116,    0,
    0,  117,  118,  119,  120,  121,  599,    0,    0,  122,
  123,    0,  600,  601,  602,  124,  603,  604,  605,    0,
  606,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  461,    0,    0,    0,    0,
  607,    0,  608,    0,    0,  461,  461,  461,  461,  461,
  461,  461,  461,  461,  461,  461,  461,  461,  461,  461,
    0,    0,  461,  461,  461,  461,    0,  461,    0,    0,
  461,  461,    0,  609,  461,  461,  461,  461,  461,  461,
  461,  461,    0,  461,  461,  461,  461,  461,    0,    0,
  610,  461,  461,  461,  461,  461,  461,  461,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  611,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  107,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  612,  613,    0,
    0,    0,    0,    0,    0,    0,  108,  109,    0,  110,
    0,    0,    0,    0,  614,    0,  111,  112,  113,  114,
  115,    0,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,    0,  124,
};
const short yycheck[] = {                                       9,
    4,   56,   12,   65,  106,   16,   16,  154,   18,    0,
  687,   64,  143,  144,  710,  296,  367,   92,  528,  143,
  144,  371,  371,  565,   86,  342,  431,   63,   74,   71,
  468,   64,   95,  876,  511,   90,  363,  880,  535,   89,
 1199, 1046,  488,   41,   54,   55,   56,   57,   58,   59,
   96,  340,   94,  736,   64,   65,   66,  647,   68,  938,
   70,   71,  695,   73,   74,   58, 1272,   69,  764,    0,
  994,   41,   82,   84,   84,   85,   86,   70,   88,   89,
   90,   91,   93,   93,   94,   95,   96,   41,   98,   57,
   92, 1035,  640,  290,  157,  273, 1035,  348,  148,  732,
   68,  134, 1046,  136,  282,  984,    0, 1046, 1073,  388,
   41,  709,    0,  861,  962,    0,   41,  317,    0,   40,
  558,    0,  560,   91,  134,  135,  136,  565,   59,    0,
   98,  226,    0,  235,   63,    0, 1040,    0,  148, 1217,
    0,  717,  135, 1047,  154,  155,  156,  157,   44,   44,
   41,  161,    0,   41, 1058, 1035,  349,   40, 1035,   41,
  196, 1035,   41,  156,    0,   59, 1046, 1035,   41, 1046,
  317,   59, 1046,   41,   59, 1079,   41,   59, 1046,   44,
   59,   41, 1096,  376,   44,  221,   44,  155,   59,    0,
  771,   59,   45,  544,   59,   44,   59,    0,   45,   59,
  494,   40, 1042,   45,   44,   41,    0,  788,   44,   61,
  791,   59,    0,  280,   44,  565,  565,    0, 1097,  800,
  803,    0,  324,   59,   63,  320,   59,  904,  905,   91,
   41,  264,   41,   44,  267,  745,   44,    0,   41,    0,
    0,  592,  260,   61,   40,    0,  301,   41,   59, 1408,
 1409, 1091,  301,   41,  264,   44,   59,  267,  935, 1264,
 1265,  271,   44,  273,  274,   59, 1232,   63,   44,  283,
  280,   59,  282,  854,  262,  260,   59,    0,  271,  568,
   59,   41,  402,  355,  317,   41,   41,  280,   44,  338,
  262,  301,    0, 1298,  977,  305,   59,  320,   59,   59,
 1218,    0,  480,    0,   59,   40,  989,  990,  341, 1233,
   45,   45,  305,  356,    0,  348,  595,  350,   41,  352,
 1264, 1265,  355,   41,  264, 1264, 1265,  616,  364,  384,
  366,  341,  374,  161,  402,  277,   59,  530,  348, 1304,
  350,  488,  352,   41,  380,  355,  402, 1390,    0,    0,
    0,   59, 1320,    0, 1298,   44, 1474, 1034,  348, 1298,
   59,  838,   59,  373,  374, 1770,  434,  287,  458, 1024,
  245,  262,  382,   59,  384, 1293,  455,  331,  253,  817,
  300,  449,  438, 1287, 1264, 1265,  429, 1264, 1265,  382,
 1264, 1265,    0,  442,  675,  485, 1264, 1265,  386,  417,
  506, 1519,  522,  345, 1244,  373,  300,   59,   59,   59,
 1453,  692,   59,  436,  386,  277,   45,  387, 1298, 1267,
  492, 1298,  432, 1629, 1298,  439,  396,  708, 1587,  496,
 1298,    0,  417,  387,   63, 1840, 1017,  300,    0,  318,
 1117, 1118,  396, 1120, 1121,  739, 1101,  435,  300,  328,
  318,   59,  331,  318,  348,  488, 1037, 1038,  318,  583,
  318,  340,  387, 1311,  304,  746,  476, 1050, 1051,  479,
  480,  411,  340,  394, 1270,  340, 1057,  487,  736,   41,
  340,  491,  300,  476,  494,  348,  496,  768,  264,  499,
   59, 1457, 1419,  782,  487,  386,  785,   59,  491,  387,
  309,   42,   43,  496,   45,  387,   47,  796,  387,  584,
  546,  479,  320,  386,  396,  318, 1106,  396, 1101,  387,
  320, 1727,  304,  521,  318, 1603,  494,  517,  396,  458,
  318, 1129,  565,  274,  823,  630,  982,  340,  789,  410,
    0, 1465, 1090,    0, 1227,  742,  340,  383,  865,  727,
  586,  521,  340,    0,  564,  565, 1524,  452,  454,  342,
  570,  440, 1310,  318,  447,  765, 1531,  521,  328,  454,
  331,  331,  440, 1779, 1478,  454, 1253,  410, 1482, 1483,
  985,   41,  584,  714,   41,  340,  454,   44,  436,  454,
  521,    0,  300,  387,  454,  874,  521,   44, 1275,   59,
  636,  637,   59, 1517,    0,  458,  639, 1813,  331,  458,
  328,  458,   59,  331, 1518,    0,  458,  264,  765,  458,
 1196, 1469,  405,  517,  454,  408,  750,  387,  678,  639,
  409,  328,   41,  521,  331,   44,  396,  502,  648,  521,
  348,  424,  521,  485,  648,   41,  440,  436,  356,  348,
   59,  454,  933,  521,  517,  411,  521, 1536, 1498,  852,
  454,  521,  458,   59,  387,  501,  454,  450,  678,  387,
  278,  499,  280,  396,   59,   42, 1516,  440,  396,  440,
 1685,  367,  715, 1520,  717,  393,  719,  395,  386, 1485,
  501,  454,  702,  454,  383,  264,  431,  431,  348,  454,
  710, 1028, 1861, 1746,    0,  715,   41,  717, 1341,  719,
  418, 1615,  722,    0,  653, 1619,  367,  727,  521,  977,
 1210,  264, 1212,  458,  458,  466,  736,  521, 1309,  739,
 1634,  989,  765,  521,  744,  443, 1221,  770,  748,  280,
  815, 1685,  778,  440,  285,   41, 1685, 1651,   44,  759,
  397,  761,   41,    0,  764, 1044,  789,  454,  262,  338,
  770,  521,    0,   59,  653, 1092,  759, 1056,  761,  658,
  378,  739,   59,  848,  849,  850, 1273,    0,  340,  789,
  273,    0,    0,  816,  817,    0, 1191, 1104,  966, 1078,
 1467,  969,  828, 1641,  318,   41,  405,    0,  521, 1647,
  348,  300,  431,  521,  301, 1685,  816,  817, 1685,  517,
  286, 1685,   59,  815,    0,  424,   41, 1685,    0, 1296,
  822,   59,   41,   41,  313, 1302, 1231, 1369,  397,  458,
 1734,  301, 1317,  835,  164,  982,   59, 1764,   41, 1743,
   59,   59,  263,  853,   59,    0,  848,  849,  850,  348,
 1167,  268,   42,  863,   40,   41,   59,   47,   44,   41,
  853,  318,    0,  442,  402,  875, 1684,  318,  411, 1796,
  863,  328, 1181,   59,  331, 1184,  465,   59,  467, 1783,
  340,  328,  386,  340,  331,   41,  343,   41,   44,  262,
  352,  312,  454,  260, 1712,  436,  343,  259, 1735,   59,
 1181, 1578,  323, 1184,   59,    0, 1749,  970,  270,  318,
    0, 1838,    0,  461,  462,    0,  378,    0,   12,  328,
  261,   59,  331,    0, 1742,  971,  968,  262,  417,  453,
  387,  340, 1859,  377,  343, 1862,  303,  307,  308,  396,
 1221,    0,  881,  328,  340,  442,  331,  283,  365,  982,
 1877,   41, 1076,   41,   44, 1393,  966,  967,  968,  969,
  970,  971,  397, 1314,   59, 1455,    0,  977,  264,   59,
  508,   59,  442,  262,   59,   12,   59,  264,  387,  989,
  990,  409,   59,  440,  994,  461,  462,  396,  877,  307,
  308, 1024,  881,  440,  454,  452, 1285,  454,  328,  967,
   59,  436,  453,  994,  448,  452,  427,  454,   16, 1827,
 1085,  307,  308,  386, 1024,  257,  262,  264,  517, 1369,
 1369,  259,   44,  301,  320,   59,  264, 1316, 1103,    0,
    0,  440,  270, 1710, 1109,  470, 1317,  262, 1715,   42,
   43,  264,   45,  452,   47,  454,  308,  141,  384,  264,
  458,  386,    0, 1200,  405,  440,  318, 1067,  454, 1569,
  338, 1094,  435, 1759,  521,    0, 1070,  452, 1101,  454,
   41,   93,    0, 1083, 1383,    0,   84,    0, 1387,    0,
  273,  451,    0, 1085, 1094, 1216, 1217, 1218,   59,   59,
  280, 1101,  334,   41, 1604,  285,   44,  386,  458,  318,
  318, 1103, 1383,  439,  141,  456, 1387, 1109,  262,  264,
  328,   59,  521,  331,  410,  318,   41,  405,  274,   44,
  456,  340,  340,   44,   59, 1249,  264, 1251,  300,  258,
 1747,   59,  318,  451,   59,  321,   59,  340,   59,  280,
  386,   59,  328, 1653,  285,  331, 1199,  340,  257,  335,
  397, 1791, 1769,  309,  340,  451,  452,  343,  340,  397,
  308,  386,   44, 1196,  460, 1805, 1199, 1200,  456,  387,
  318, 1788,  262,  359,  397, 1737,  319,  306,  396,  264,
  352,  264,  397,  278,   40,  280, 1196,  264, 1312, 1199,
 1200,  453,  265,  489,  490, 1872,  492,  383, 1487,    0,
 1210,  387, 1212,  499,  500,  501,  502,  503,    0,  505,
  396,   93,  508,  509,  510,  511,  512, 1227, 1739,  325,
  516,  517,  440, 1233, 1786,  334,  522,  413,  318,  302,
  318,  321,  386, 1873,  452,  454,  454, 1526,  328, 1856,
  328,  331,  397,  331,  278,  335,  280,  387,  417,  278,
  340,  454,  340,  343,  440,  411, 1565,  400,   59,  397,
    0,  263, 1824, 1784,  337,  259,  452,   59,  454,  359,
    0,  291, 1419,    0,    0,  444,  270,  280,  260,  299,
 1790, 1828,  285,  342, 1565,  287,  377, 1291,  258,  418,
  319, 1327, 1839,  383,  296, 1331,  386,  387,  441,  387,
  440,  271, 1823,  521,   44,  453,  396,  436,  396,  322,
  466,   41,  397, 1460,  397,   41,  264,  320,  377,   59,
  397,  303,    0,  413,  430,  327,    0,  418,  330,   59,
  300,  437,   59,   59, 1855,  521,  306, 1399,  308, 1886,
  258,  470,  272,    0,  493,  435,  405,  318,  322,  408,
  440,  343,  440,  271,  503,  378,  316,  317,  288, 1648,
  331,    0,  452,   41,  454,  424,  454,   40,  301,  340,
  318,  400,   41,  321,  465,   44,  368,  400,  348,  403,
  328,   59,  300,  331,  407,   59, 1419,  335,  306, 1399,
  308,  450,  340,  318,    0,  343,  420, 1407, 1408, 1409,
    0,    0,   59,  333,    0,  338,  331,  328,    0, 1419,
  331,  359,  441, 1590,  342,  340,  387,  401,  441,  342,
   59,   44,  343,  425,   93,  396,  321,  429,  430,  431,
  348,  521,  362,  521,  426,  383,   59,   44,  356,  387,
  335,   41,  383, 1590,   44, 1455,  493,  494,  396,  377,
 1460,    0,  383,   59,  377, 1465,  265,  258, 1605,   59,
   59,   44,  387,   59,  359,  413,  258,   59,  385,  440,
  271,  396, 1603,    0,  407,  321,    0,  405, 1488,  271,
  408,    0,  405,  454,  417,  408,   93,  383,  383,  335,
    0,   41,  440,  302,   44, 1488,  424,  356,  500,  429,
  258,  424,  504,  261,  452,  306,  454,  308,  300,  442,
   59,  429,  460,  359,  306,  440,  308,   41,  413,  440,
   44,    0,  450,    0,    0,  383,    0,  450,  294,  454,
 1632,  452,   59,  454,  393,   59,  395,  383,   44,  305,
   59,  489,  490,   93,  492,    0,  312,  517,  306,   59,
  521,  499,  500,  501,  502,  503,  348,  505,  436,  418,
  508,  509,  510,  511,  512, 1575,  337,  413,  516,  517,
 1210,  337, 1212,  521,  522, 1752,  260, 1587, 1633, 1756,
   59, 1758,   59,   59,  443,   59,   41,   93,  318,   44,
 1767, 1590,  318,  337,  494, 1605,  521,   42,   43,  517,
   45,  331,   47,    0,   59, 1752,    0, 1575, 1599, 1756,
  340, 1758,    0, 1623,  340,    0, 1793, 1764, 1628,  303,
 1767, 1657,    0, 1633,  260, 1661, 1728,    0,   42,   43,
  374,   45, 1888,   47, 1890, 1628,  402,    0,    0,  418,
 1702, 1652, 1652,  387,  274,   59, 1793,   42,   43, 1796,
   45,  260,   47, 1830,    0,  521,  658,  387, 1835,   44,
   40,  387,   59,    0,  424,   59,  396,  303,  434,  264,
  396,   59,  438,  675,   59,    0,    0,  679,  278,   41,
  280,   59,   44, 1830,  686,  342,   59,  287, 1835,  691,
  692, 1838, 1702, 1703,  303,   41,   59,   59,   44,  701,
 1704,  703,  704,  705,  706,  707,  708,  709,   41,  417,
  440,   44, 1859,   59,  440, 1862,   41,  318,  321,   44,
  377,    0,   59,  321,  454,  517, 1737, 1737,  454,    0,
 1877, 1764,  335,  461,   59,   59,  444,  335,  260,  316,
  343,  460,   42,   43,  746,   45,  337,   47,  405, 1759,
    0,  408,    0, 1752, 1764,  336,  359, 1756,    0, 1758,
 1770,  359,    0, 1796, 1775, 1775,  768,  424, 1767,  337,
   41,  773,  774,   44,   41, 1786, 1786,   44,  378,    0,
   59,  303,  321,   41,   42,   43, 1796,   45,   59,   47,
  452,  521, 1847,  450, 1793,  521,  335, 1808, 1808,   41,
  400,    0,   44,   44,  343, 1838,  374,  407,  402,   59,
  413,   59,  340, 1824, 1824,  413,   40,   59,   59,  387,
  359,   59,  411,  278,    0,  280, 1859,   46, 1838, 1862,
 1840, 1830,  287, 1844, 1844,  280, 1835, 1847,   59,   58,
  285,  441,   61,  318, 1877,   42,   43,  320,   45, 1859,
   47, 1861, 1862,   42,   43,  402,   45, 1867,   47,  506,
   59,  391,  392,  383,   46,   41,  280, 1877,   44,   46,
  383,  285,   42,   43,  413,   45,   58,   47, 1888,   61,
 1890,   58,  436,   59,   61,  280,  450,  889,  890,  891,
  285,  361,  894,  895,  896,  340,  898,  899,  900,  901,
  902,  903,  434,   41,  906,  907,   44,  909,   41,  911,
  417,   44,  914,  915,  916,  917,  918,  919,   41,   42,
   43,  384,   45,  378,   47,  320,  340, 1857,  383,  931,
   41,  933,   41,   44, 1864,   44,  321,   42,   43,   44,
   45,  318,   47,  328,  258,  400,  331,  516,   41,  516,
  335,   44,  407,  278,  517,  280,  318,  271,  343,  321,
   41, 1891,  287,   44,   41,   41,  328,   44,   44,  331,
    0,  287,   41,  335,  359,   44,   40,  469,  340,   44,
  280,  343,    0,  985,   41,  285,  441,   44,   61,   41,
   42,   43,  306,   45,  308,   47,  372,  359,  383,   41,
   42,   43,  286,   45,   40,   47,   40,  278,  264,  280,
   40,   41,   42,   43,   44,   45,   46,   47,  318,   40,
  262,  383,  280,   41,  386,  387,   44,  285,  413,   59,
   60,   61,   62,  418,  396,   40,  457,  458,  459,  461,
  462,   59,   42,   43,   40,   45,  501,   47,   41,   42,
   43,  413,   45,  378,   47,  440,   41,   42,   43,   59,
   45,   91,   47,   93,  485,  486,   40,  452,   41,  454,
  461,   44,   40,  435,   41,  400,  318,   44,  440,  321,
  461,   41,  407,  280,   44,   40,  328,   40,  285,  331,
  452,  280,  454,  335,   41,  265,  285,   44,  340,   44,
   41,  343,  278,   44,  280,   41,   41,  378,   44,   44,
  280,  287,  461,  462,  311,  285,  441,  359,   40,  461,
   41,   40,  311,   44,   41,   42,   43, 1129,   45,  400,
   47,  342,  302,   40,   40,   40,  407,   40,   40, 1141,
 1142,  383, 1144, 1145,  386,  387,   46,   41,   42,   43,
 1152,   45,   91,   47,  396,   42,   43,  280,   45,  521,
   47,   40,  285,   61,  264,  320,  377,  436, 1752, 1171,
  441,  413, 1756,   44,  402,  280,  501, 1179, 1180, 1181,
  285,    0, 1184, 1767, 1186,  320,   44,   44, 1190, 1191,
 1192, 1193,  264,  435,  405,  428,   46,  408,  440,  894,
   61,  896,  378,  898,  899,   40,  901,  902,  903, 1793,
  452,  264,  454,  424,   44,  442,  348,  454,  348, 1221,
  284,  348,   41,    0,  400,   44,  300,  278,  280, 1231,
  278,  407,  262,  285,  264,  265,  264,  397,  280,  450,
   59,  436,  272,  285,  470,  397, 1830,  348,  458,  447,
  280, 1835,  277,  447,  399,  285,   40,  349,  288,   40,
  278,   40,  280,   91,   41,  441, 1268,   44,   40,  287,
  264,  318,  302,  383,  411,    0,   44,  307,  308,  521,
  280,  311,   59,  389,  382,  285,   40,  280,  318,   40,
  320,  321,  285,  451,  324,  280,   41,  277,  328,   63,
  285,  331,  299,  333,  317,  335,   40,  436,  464,  320,
  340,  341,  320,  343,  264, 1317,   41,  448,  452,   44,
  382,  502,   41,   42,   43,  501,   45,  505,   47,  359,
  360,  674,  362,  676,   59,   40, 1338,  680,  681,  682,
  274,   40,  300,  344,  284, 1347,  376,  278,  378,  379,
  380,  386, 1354,  383,  344,  386,  386,  387,  418,  386,
  378,  416,  300,  280,  418,   40,  396,  356,  285,   41,
   42,   43,    0,   45,  417,   47,   40,  274,  417,   40,
  410, 1383,  400,  413,  264, 1387,  280,   41,   44,  407,
  320,  285,   44,  280,  280,  271,  460,  446,  285,  429,
   44,  385, 1404,  367,   44,  435,  436,  320,   40,   44,
  440,  266,  328,   41,   42,   43,   44,   45,   46,   47,
   40,  451,  452,  441,  454,  489,  490,   40,  492,   41,
  460,   59,   60,   61,   62,  499,  500,  501,  502,  503,
  501,  505,  264,  262,  508,  509,  510,  511,  512,    0,
  422,   58,  516,  517,   41,   41,    0,   41,  522,  489,
  490,  264,  492,   91, 1466,   93,  454,  394,  309,  499,
  500,  501,  502,  503,  440,  505,  492,  309,  508,  509,
  510,  511,  512,  501,   41,   41,  516,  517,   40,    0,
   41,  521,  522,   44,   40,   40,  352,   41,  417,  318,
   44,  278,  321,  280,   40,   40,   40,  284,   59,  328,
  287,   40,  331,   40,   40,   59,  335,   40,   40,   40,
   40,  340,   40,  300,  343,   40,   40,  262,  308,  327,
   41,  435,   44,   44,   41,   41,  388,  388,  343,  388,
  359,  460, 1544, 1545,  274,  331,   44,  391,   59,  287,
   61,   41,  386,  278,  407,  280,   41,  386,  378, 1561,
  386,  280,  287, 1565,  383,  386,  285,  386,  387,  386,
  489,  490, 1574,  492, 1576,  300,  386,  396,  278,   40,
  499,  500,  501,  502,  503,  458,  505,   41,   41,  508,
  509,  510,  511,  512,  413,   41,   41,  516,  517,  417,
  308,  378,  435,  522,   59,  293,   40,   59,  280,  340,
  260,   59,   59,  285,  340,   59,  435,   42,   43,   59,
   45,  440,   47,  400,   59,  308,   59,   59,   58,  385,
  407,  396,  436,  452,  262,  454,  264,  265,  280,  337,
  442,  338,  442,  301,  272,  338,  301,   41,   41,  352,
  320,  301,  280,  378, 1656,  371,  385,  285,  371,   41,
  288,   41, 1664,  313,  441,   41,   42,   43,   41,   45,
   41,   47,   41,   41,  302,  400,  304,   41,    0,  307,
  308,   41,  407,  311,   40,  318,   41,   41,  338, 1691,
  318,   41,  320,  321,   41,   41,  324,   41,   41,   41,
  328,    0,  521,  331,   41,  333,   59,  335,  451,   40,
   44,   41,  340,  341, 1716,  343,  441,  344,  262,   41,
   42,   43,   44,   45,   46,   47,  352,  278,   40,  280,
   59,  359,  360,  284,  362,  340,  287,   59,   60,   61,
   62,   46,   41,  340,   40,   44,  508,  258,  376,  300,
  378,  379,  380,  368,   40,  383,   44,  407,  386,  387,
   59,  383,  410,  274, 1766,  383,  435,  417,  396,   91,
   41,   93,  383,  343,  318,   41,   41,  321,   59,  304,
   44,   41,  410,   44,  328,  413,  387,  331,  301,  300,
  307,  335,  442,  442,  293,  306,  340,   40,  309,  343,
  304,  429,   44,  463,  337,   41,   40,  435,  436,   41,
    0,  383,  440,  337,  383,  359,   41,   42,   43,   40,
   45,  264,   47,  451,  452,  264,  454,  378,   42,   43,
  389,   45,  460,   47,  458,  411,  278,  262,  320,  383,
  470,  451,  386,  387,   44,  271,   40,  309,  271,  400,
  458,  264,  396,  435,  343,  280,  407,    0,  436,  343,
  285,  489,  490,  343,  492,  343,  458,   41,   41,  413,
   59,  499,  500,  501,  502,  503,   59,  505,   40,  308,
  508,  509,  510,  511,  512,  304,    0,   41,  516,  517,
  441,  435,   59,  521,  522,   59,  440,   41,   41,   59,
  411,   44,   41,    0,  280,    0,    0,  418,  452,  285,
  454, 1213,   12,  275,  727,  282,   59,  274,   41,   42,
   43, 1106,   45,  643,   47,  436, 1101,   41, 1093,  866,
   44, 1104,   41,   42,   43,   88,   45, 1507,   47,  338,
  262, 1506,  264,  265,  368,   59,  592, 1083, 1031,  350,
  272, 1094,  322,  350,  770,  466, 1570, 1623,  280,  470,
  798, 1777, 1485,  285,  570, 1780,  288,   41,   42,   43,
 1487,   45, 1815,   47, 1781,  789,    0,  521, 1691,  278,
  302,  280,    0, 1407,  963,  307,  308, 1595,  287,  311,
 1765,  715, 1246, 1019, 1737,  343,  318,  355,  320,  321,
  336,  300,  324, 1470,  572,  845,  328, 1300,  629,  331,
  837,  333,   13,  335,  309,  517,  513,   41,  340,  341,
   44,  343,  759,   41,   42,   43,   44,   45,   46,   47,
  146,  773,  746,  994, 1716,   59, 1844,  359,  360, 1613,
  362,   59,   60,   61,   62, 1808,  735,  528,  262, 1143,
 1255,   -1,   -1,  986,  376,  280,  378,  379,  380,   40,
  285,  383,    0,   -1,  386,  387,  280, 1186,   -1,   -1,
   -1,  285,   -1,   91,  396,   93,   -1,   58,   -1,  378,
  472,  473,  474,  475,  476,  477,  478,  479,  410,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  400,   -1,   41,  460,   -1,   44,  429,  407,   -1,
   -1,   -1,   -1,  435,  436,   -1,   -1,   -1,  440,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,  489,  490,  278,  492,  280,  460,   -1,
   -1,   -1,  441,  499,  500,  501,  502,  503,  262,  505,
   -1,   -1,  508,  509,  510,  511,  512,  280,   -1,   -1,
  516,  517,  285,   -1,   -1,   -1,  522,  489,  490,   -1,
  492,  280,   -1,   -1,   -1,   -1,  285,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,  521,
  522,   -1,   -1,   -1,  318,   -1,  280,  321,   -1,   -1,
   -1,  285,   -1,   -1,  328,   -1,   -1,  331,   41,   42,
   43,  335,   45,   -1,   47,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,  359,  264,  265,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,  400,   -1,   -1,
   -1,   -1,  280,   -1,  407,   -1,   -1,  285,   -1,  383,
  288,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,  302,   -1,    0,   -1,   -1,  307,
  308,   -1,   -1,  311,   -1,   -1,   -1,  321,  441,  413,
  318,   -1,  320,  321,  328,   -1,  324,  331,   -1,   -1,
  328,  335,   -1,  331,   -1,  333,   -1,  335,   -1,  343,
   -1,  435,  340,  341,  262,  343,  440,   41,   42,   43,
   44,   45,   46,   47,   -1,  359,   -1,   -1,  452,   -1,
  454,  359,  360,   -1,  362,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  383,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,  396,   93,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,  413,
  328,   -1,  410,  331,   -1,  413,    0,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,    0,  521,   -1,   -1,
   -1,  429,   -1,   -1,   -1,   -1,  440,  435,  436,   -1,
   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,  451,  452,   -1,  454,   41,   -1,   -1,
   44,   -1,  460,   -1,   -1,  383,   -1,   41,  386,  387,
   44,   -1,   -1,   -1,   -1,   59,   -1,  280,  396,   -1,
   -1,   -1,  285,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,  413,   -1,   -1,   -1,  460,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  435,  516,  517,
   -1,   -1,  440,  521,  522,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,  452,   -1,  454,   -1,  499,  500,
  501,  502,  503,  261,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,  522,   -1,   -1,  282,   -1,   -1,   -1,  262,   -1,
  264,  265,  290,   -1,   -1,   -1,   -1,   -1,  272,  297,
   -1,  299,   -1,  301,   -1,   -1,  280,   -1,  306,   -1,
   -1,  285,   -1,   -1,  288,  313,   41,   42,   43,   -1,
   45,   -1,   47,  521,    0,   -1,   -1,   -1,  302,  327,
    0,   -1,   -1,  307,  308,   -1,   40,  311,   -1,   -1,
  338,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   41,  340,  341,   44,  343,
   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   59,   -1,  359,  360,   -1,  362,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,  405,   42,  383,
   -1,   -1,  386,  387,  412,   -1,   -1,  415,   -1,  417,
  418,   -1,  396,   93,  278,   -1,  280,   -1,   -1,   -1,
   -1,   -1,   -1,  287,   -1,   -1,  410,   -1,   -1,  413,
   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  318,  429,   -1,  321,   -1,   -1,
   -1,  435,  436,   -1,  328,   -1,  440,  331,   -1,   -1,
   -1,  335,  294,   -1,   -1,   -1,  340,  451,  452,  343,
  454,   -1,   -1,  305,   -1,   -1,  460,   -1,   -1,   -1,
  312,   -1,   -1,  491,   -1,  359,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  325,   -1,   -1,   -1,   -1,  506,  507,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  383,
   -1,   -1,  386,  387,  378,  499,  500,  501,  502,  503,
   -1,  505,  396,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,  400,  521,  522,  413,
   -1,   -1,   -1,  407,   -1,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,   -1,  386,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,
  402,   -1,   -1,   -1,   -1,   -1,   -1,  441,  452,  305,
  454,   -1,  262,   -1,  264,  265,  312,  419,   -1,   -1,
  316,  317,  272,   -1,   -1,   -1,   -1,   -1,   -1,  325,
  280,   -1,  434,   -1,   -1,  285,  438,   -1,  288,  441,
   -1,  337,   -1,   -1,   -1,   -1,   -1,  449,    0,   -1,
   -1,   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   -1,  311,  318,   -1,   -1,  321,  468,   -1,  318,   -1,
  320,  321,  328,   -1,  324,  331,   -1,  521,  328,  335,
    0,  331,  296,  333,  340,  335,   -1,  343,   -1,   41,
  340,  341,   44,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,  359,   -1,   -1,  402,   59,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   41,   -1,  419,   44,   -1,  376,  383,  378,  379,
  380,  387,   -1,  383,   -1,   -1,  386,  387,  434,   59,
  396,   61,  438,   -1,   -1,   -1,  396,   93,   -1,   -1,
   -1,   -1,   -1,  449,   -1,   -1,   -1,  413,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  468,   -1,   -1,   -1,  460,   -1,   -1,  429,
   -1,   -1,   -1,   -1,  440,  435,  436,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,  451,  452,   -1,  454,  489,  490,   -1,  492,   -1,
  460,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   42,   -1,   -1,   -1,  522,  489,
  490,   -1,  492,   -1,   -1,   -1,  460,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  521,  516,  517,   -1,   -1,
    0,  521,  522,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,   58,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   41,   42,   43,   44,   45,  262,   47,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272, 1339, 1340,   59,
   60,   61,   62, 1345, 1346,   -1, 1348, 1349,  258,  285,
   -1,   -1,  288,   -1, 1356, 1357, 1358, 1359,   -1, 1361,
 1362, 1363, 1364, 1365, 1366,   -1,  302,  277,  278,   -1,
  280,  307,  308,   93,  284,  311,  318,  287,   -1,  321,
   -1,   -1,  318,   -1,  320,  321,  328,   -1,  324,  331,
  300,   -1,  328,  335,   -1,  331,  306,  333,  340,  335,
   -1,  343,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,  383,  378,  379,  380,  387,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,
   -1,  413,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
  400,   -1,   -1,  429,   -1,   -1,   -1,  407,  440,  435,
  436,   -1,   -1,   -1,  440,   -1,  416,   -1,  418,   -1,
  452,   -1,  454,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   45,   -1,   -1,  460,   -1,  436,   -1,   -1,   41,
   -1,  441,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   59,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
  470,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,  521,
  516,  517,  302,   -1,    0,  521,  522,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,  460,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,  489,  490,  413,  492,   -1,  460,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,  429,
  508,  509,  510,  511,  512,  435,  436,   -1,  516,  517,
  440,   -1,   -1,   -1,  522,  489,  490,   -1,  492,   -1,
   -1,  451,  452,   -1,  454,  499,  500,  501,  502,  503,
  460,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  262,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   44,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
    0,  521,  522,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   59,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,  262,  359,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  383,  288,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,  302,   -1,   -1,   -1,
   -1,  307,  308,   93,   -1,  311,   -1,   -1,   -1,   -1,
   -1,  413,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,  435,  340,  341,   -1,  343,  440,   -1,
   -1,   -1,   -1,   -1,  458,   -1,  460,   -1,   -1,   -1,
  452,   -1,  454,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,  489,  490,  383,  492,   -1,
  386,  387,   -1,   -1,   -1,  499,  500,  501,  502,  503,
  396,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  410,   -1,   -1,  413,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,  521,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
  436,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,  320,   -1,  460,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   59,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  302,   -1,    0,  521,  522,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
    0,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,  376,   -1,  378,  379,
  380,   -1,  460,  383,   -1,   -1,  386,  387,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,  489,  490,  413,  492,   -1,  460,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,  429,
  508,  509,  510,  511,  512,  435,  436,   -1,  516,  517,
  440,   -1,   -1,   -1,  522,  489,  490,   -1,  492,   -1,
   -1,  451,  452,   -1,  454,  499,  500,  501,  502,  503,
  460,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  262,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
    0,  521,  522,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,  262,  359,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  258,   -1,
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
   -1,   -1,   -1,   -1,  410,   -1,   59,  413,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  521,
  400,   -1,   -1,  429,   -1,   -1,   -1,  407,   -1,  435,
  436,   -1,   -1,   -1,  440,   -1,   -1,   -1,  418,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,  436,   -1,   -1,   -1,
   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
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
  380,   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   93,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   41,   -1,   -1,   44,  429,
   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,   -1,   -1,
  440,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,  318,   -1,   -1,  321,   -1,
  460,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,  359,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
  383,  521,  522,  386,  387,   -1,   -1,  273,   -1,    0,
   -1,  277,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  291,   -1,   -1,   -1,  295,
  413,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   41,   -1,  435,   44,   -1,   -1,  272,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,  452,
   -1,  454,  288,  339,   -1,   -1,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,  302,  353,   -1,   -1,
   -1,  307,  308,    0,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,  373,  324,  375,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  521,   -1,
  406,   -1,   -1,  359,  360,  411,  362,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,  423,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,
  396,   -1,  318,   -1,    0,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,  410,  331,   -1,  413,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,  429,  480,  481,   59,   -1,   -1,  435,
  436,   -1,   -1,  359,  440,   41,   -1,   -1,   44,   -1,
   -1,  497,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   59,  460,   61,   -1,  383,   -1,   -1,
   -1,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   91,  492,  413,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,  440,  521,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,  489,  490,  262,  492,    0,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  359,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,  521,  522,   -1,   -1,   -1,
   -1,   -1,  383,   -1,   -1,   -1,  387,   -1,   41,   42,
   43,   44,   45,   46,   47,  396,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,   59,   60,   61,   62,
   -1,  328,  413,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   91,  440,
   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,  452,  278,  454,  280,   -1,   -1,   -1,  284,  460,
   -1,  287,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,  396,
  306,   -1,   -1,  309,    0,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,  413,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,  435,   -1,
  521,  522,   -1,  440,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  400,   91,   -1,   -1,   -1,   -1,
   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,
   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,  521,   -1,   -1,  460,   -1,  262,
  436,   -1,   -1,   -1,   -1,  441,   -1,   59,   -1,  272,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,  489,  490,   -1,  492,
  466,   -1,   -1,   -1,  470,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,    0,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  318,   -1,   -1,  321,  522,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,    0,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   -1,   91,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   41,
  413,   -1,   44,   -1,  280,   -1,   -1,   -1,   -1,  285,
   -1,   -1,  288,   -1,   -1,   -1,  429,   59,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  452,
   -1,  454,  318,   -1,   -1,  321,    0,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
  262,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,  359,  360,   -1,  362,   41,   -1,   -1,
   44,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   59,   -1,  383,  521,   -1,
  386,  387,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,
  396,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,  413,  340,   -1,
   -1,  343,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,  262,  359,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,  272,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  280,   -1,  452,   -1,  454,  285,
   -1,  383,  288,   -1,  386,  387,   41,    0,   -1,   44,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
   -1,  413,  318,   -1,   -1,  321,   -1,   -1,  324,   -1,
  262,   -1,  328,   -1,   -1,  331,   -1,  333,   41,  335,
   -1,   44,   -1,  435,  340,  341,   -1,  343,  440,   -1,
   -1,   -1,   -1,   -1,   -1,  521,   59,    0,   -1,   -1,
  452,   -1,  454,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,  318,  383,   -1,  321,
  386,  387,   -1,   -1,   -1,   -1,  328,   -1,   41,  331,
  396,   44,   -1,  335,   -1,   -1,   -1,  262,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   59,  413,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,  521,
   -1,   -1,   -1,  429,    0,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,  452,   -1,  454,   -1,
   -1,   -1,   -1,  318,  396,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,  318,   41,  331,  321,   44,   -1,
  335,  413,   -1,   -1,  328,  340,   -1,  331,  343,   -1,
   -1,  335,   -1,   59,    0,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,  435,  359,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,  359,   -1,  262,   -1,   -1,
  452,   -1,  454,   -1,   -1,  521,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   41,   -1,   -1,   44,  383,
   -1,  396,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   59,   45,   -1,   -1,   -1,  413,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  435,   -1,   -1,  328,   -1,  440,  331,   -1,   -1,  521,
  335,  435,   -1,   -1,   -1,  340,  440,  452,  343,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,  359,  318,   -1,   -1,  321,  262,
   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,  383,   -1,
  343,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,  396,   43,   -1,   45,   -1,  359,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  521,   58,  413,   -1,
   -1,   -1,   63,   -1,   -1,  318,   -1,  521,  321,   -1,
  383,   -1,   -1,  386,  387,  328,   -1,   -1,  331,   -1,
  435,   -1,  335,  396,   -1,  440,  262,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
  413,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,  452,
   -1,  454,  318,  396,   -1,  321,  262,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
  413,   -1,   -1,   -1,  340,   -1,  521,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,  359,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,  295,   -1,   -1,   -1,   -1,  452,
   -1,  454,  318,   -1,   -1,  321,   -1,  383,  521,   -1,
  386,  387,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
  396,   -1,  323,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  352,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,  267,  383,  521,   -1,
  386,  387,   -1,   -1,   -1,  276,  452,   -1,  454,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   40,  295,  296,   43,  413,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,  314,   -1,   -1,   63,   -1,   -1,  435,
   -1,   -1,  323,   -1,  440,  326,  427,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,  443,   -1,   -1,  521,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  457,  458,  459,  287,
  461,  462,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   40,   -1,   42,   43,  378,   45,  480,
  481,  482,  483,  484,  485,  486,  487,  488,   -1,   -1,
   -1,   58,   -1,   -1,  495,  496,   63,   -1,   -1,   -1,
   -1,   -1,   -1,  504,   -1,  521,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,
  421,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,  517,   -1,   -1,   -1,
  267,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,  292,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,  314,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  323,  505,   -1,  326,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,  260,  522,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,
   -1,  378,   40,   -1,   42,   43,  303,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  352,  433,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  427,  508,  509,  510,  511,  512,  433,  287,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,  260,   -1,   -1,  522,   -1,   -1,   -1,  267,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  289,   -1,   -1,   -1,   63,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,  352,   -1,  354,  355,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   58,   -1,   -1,  489,
  490,   63,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  267,
  498,  499,  500,  501,  502,  503,  504,  505,  276,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,  289,   -1,   -1,  522,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  272,   -1,   -1,  314,   -1,   -1,   -1,
   -1,  280,   -1,   -1,   -1,  323,  285,   -1,  326,  288,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,  354,  355,   40,   -1,
   -1,   43,   -1,   45,  276,  324,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,  333,   -1,   58,  289,   -1,   -1,
  378,   63,  341,  295,  296,   -1,   -1,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,  314,  362,   -1,   60,   61,   62,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,
  458,  459,  460,  461,  462,   -1,  378,   -1,   -1,   -1,
  429,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  421,
  508,  509,  510,  511,  512,  427,   -1,   -1,  516,  517,
   -1,  433,   -1,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,  434,  495,  496,  267,  498,  499,  500,  501,
  502,  503,  504,  505,  276,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,  289,  460,   -1,
  522,   -1,   -1,  295,  296,   -1,   40,  272,   -1,   43,
   -1,   45,   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,
  285,   -1,  314,  288,   58,   -1,   -1,  489,  490,   63,
  492,  323,   -1,   -1,  326,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,  324,
  522,   -1,  354,  355,   -1,   -1,   -1,   -1,  333,   -1,
   -1,   -1,   -1,   -1,  366,   -1,  341,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  328,   -1,   -1,   63,   -1,  421,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  429,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,  260,   -1,   -1,  263,
  522,   -1,    0,  267,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,  460,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  323,
   -1,   59,  326,   61,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
  354,  355,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,  323,   -1,   -1,
  326,  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  425,
  258,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,  300,   -1,   -1,  471,   -1,   -1,  306,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  260,   -1,   -1,  263,  522,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  378,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,  400,   -1,   -1,   -1,   -1,   -1,   58,  407,
   -1,   -1,   -1,   63,   -1,  323,   -1,   -1,  326,   -1,
  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,   -1,
   -1,   -1,   -1,  441,   -1,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,  366,   -1,
  277,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,   -1,  291,   -1,   -1,   -1,  295,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,
   -1,   -1,  339,   -1,   -1,  433,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,  443,  353,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  406,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  423,   -1,  516,  517,
  260,   -1,   -1,  263,  522,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,   -1,
  277,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,  291,   -1,  326,   -1,  295,   -1,
  497,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,  366,   -1,   -1,   -1,
  370,   -1,  339,   -1,  276,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,  323,   -1,   -1,  326,  425,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,  423,  457,  458,  459,
  460,  461,  462,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,  480,  481,   -1,  516,  517,   -1,   -1,
   -1,   -1,  522,  425,   -1,  427,   -1,   -1,   -1,  343,
  497,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  426,   -1,  516,  517,  260,   -1,   -1,  263,
  522,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,  460,   -1,   -1,   -1,
   -1,  295,  296,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,  489,  490,   63,  492,  323,
   -1,   -1,  326,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  273,   -1,  366,   -1,  277,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  293,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,  425,   -1,  427,   -1,   -1,  339,   -1,   -1,  433,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,  443,
  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
  373,   -1,  375,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,  406,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  423,   -1,  516,  517,  260,   -1,   -1,  263,  522,   -1,
   -1,  267,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
  276,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,  295,   -1,   -1,  298,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,  497,   -1,   -1,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
  366,   -1,   -1,  373,  370,  375,   -1,   -1,  276,   -1,
   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,   -1,  406,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  423,   -1,  323,   -1,   -1,  326,  425,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,  497,  260,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,  301,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,  313,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,  327,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,  338,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  276,  516,  517,
   40,   -1,   -1,   43,  522,   45,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
   -1,   -1,   -1,   63,   -1,  407,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,   -1,  460,   -1,
   40,   -1,   42,   43,   -1,   45,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,  328,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,  460,  508,
  509,  510,  511,  512,   -1,   -1,  276,  516,  517,   -1,
   -1,   -1,   -1,  522,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,  489,  490,   -1,
  492,  402,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  323,  516,  517,  326,   -1,   -1,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,  438,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,  267,  449,   -1,
   -1,   -1,  352,   -1,  354,  355,  276,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,  366,  468,   -1,  289,
  370,   -1,   -1,   -1,   40,  295,  296,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   58,   -1,   -1,   -1,   -1,   63,  499,  500,
  501,  502,  503,  323,  505,   -1,  326,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  417,   -1,   -1,
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
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  338,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  260,   -1,  516,  517,   -1,   -1,
   -1,  267,  522,   -1,   40,   -1,   -1,   43,   -1,   45,
  276,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  289,  407,   -1,   -1,   63,   -1,  295,
  296,   -1,   -1,   -1,  417,   -1,   -1,  303,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  323,  505,  442,
  326,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   40,   -1,   -1,   43,   -1,   45,   -1,  443,   -1,   -1,
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
   -1,   -1,   -1,   -1,  354,  355,   -1,  443,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  457,  458,  459,  460,  461,  462,  497,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,  271,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,  427,   -1,   -1,
  516,  517,   -1,  433,   -1,  301,  522,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,  312,  313,   -1,  315,
   -1,  451,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,  332,   -1,   -1,   -1,
   -1,  471,  338,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,  267,  498,  499,
  500,  501,  502,  503,  504,  505,  276,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   40,  289,
   -1,   43,  522,   45,   -1,  295,  296,   -1,   -1,   -1,
  460,   -1,  398,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  323,   -1,   -1,  326,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,  432,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,  442,   -1,  508,  509,
  510,  511,  512,   -1,  354,  355,  516,  517,   -1,   -1,
   -1,   -1,  522,   -1,  460,   -1,  366,   -1,   40,   41,
  370,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  417,   -1,   -1,
  516,  517,  518,  519,  520,   -1,  522,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  267,  516,  517,   -1,   -1,
   -1,   -1,  522,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,  276,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,  366,   -1,   -1,  289,  370,   -1,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,  271,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  427,  508,  509,  510,  511,
  512,  433,   -1,   -1,  516,  517,   -1,   -1,   -1,  301,
  522,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  312,  313,   -1,  315,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
  332,   -1,   -1,   -1,   -1,   -1,  338,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  276,  516,  517,   40,   -1,   -1,   43,
  522,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   58,   -1,  398,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
   -1,  354,  355,   -1,   -1,   -1,   40,   -1,  460,   43,
   -1,   45,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  516,  517,   -1,   -1,   -1,   -1,  522,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,  276,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,
   -1,  295,  296,   58,   59,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  427,  508,  509,  510,  511,  512,  433,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  454,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  276,  516,  517,   40,   -1,   -1,   43,  522,   45,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,  460,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,  469,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,   -1,  499,  500,  501,  502,  503,  504,
  505,  506,  427,  508,  509,  510,  511,  512,  433,   -1,
   -1,  516,  517,  518,  519,  520,  521,  522,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  276,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,    0,   -1,   -1,   -1,   -1,  354,  355,
  276,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
  366,   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,  295,
  296,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,
   59,   -1,   61,   -1,   -1,   -1,   41,  323,   -1,   44,
  326,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   -1,  427,   91,   -1,   -1,   -1,   -1,  433,  354,  355,
   -1,   -1,   -1,   -1,   91,   -1,   -1,  443,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   91,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,  427,  508,  509,  510,  511,  512,  433,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,  258,
  516,  517,   -1,   -1,   41,   -1,  522,   44,   -1,   -1,
   -1,  258,   -1,   -1,   -1,  274,    0,   -1,  277,  278,
   -1,  280,   59,  258,   61,  284,   -1,  274,  287,   -1,
  277,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,  274,
  287,  300,  277,  278,   -1,  280,   -1,  306,   -1,  284,
  309,   -1,  287,  300,   91,   -1,   -1,   41,   -1,  306,
   44,   -1,  309,   -1,   -1,  300,   -1,   -1,   -1,   -1,
   -1,  306,   -1,   -1,  309,   59,   -1,   61,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  378,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  400,   -1,  378,   -1,   -1,   -1,   -1,  407,   -1,
   -1,   -1,  411,  400,   -1,   -1,   -1,   -1,   -1,  418,
  407,   -1,   -1,   -1,  411,  400,   -1,   -1,   -1,   -1,
   -1,  418,  407,   -1,   -1,   -1,  411,  436,   -1,   -1,
   -1,   -1,  441,  418,   -1,   -1,   -1,   -1,   -1,  436,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,
   -1,  436,   -1,   -1,   -1,   -1,  441,  466,   -1,   -1,
   -1,  470,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  466,
   -1,  258,   -1,  470,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  466,   -1,   -1,   -1,  470,   -1,  274,   -1,   -1,
   -1,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,
  287,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,  306,
   -1,  271,  309,   -1,  258,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,
  284,  301,   -1,  287,   -1,   -1,   -1,   -1,  308,   -1,
   -1,   -1,  312,  313,   -1,  315,  300,   -1,  318,   -1,
   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,
   -1,   -1,  332,   -1,   -1,   -1,   -1,   -1,  338,   -1,
   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,
  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,
   -1,  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  378,   -1,  273,   -1,  398,  436,
  277,   -1,   -1,   -1,  441,   -1,   -1,  284,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  400,  417,  295,   -1,
   -1,  298,   -1,  407,   -1,   -1,   -1,  411,   -1,  466,
   -1,   -1,  432,  470,  418,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  436,  453,   -1,   -1,   -1,  441,   -1,   -1,
  460,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
   -1,   -1,  466,   -1,   -1,   -1,  470,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,  373,   -1,  375,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
  497,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,  339,   -1,   -1,  516,
  517,   -1,  345,  346,  347,  522,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
  373,   -1,  375,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  406,  499,  500,  501,  502,  503,  504,
  505,  506,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  423,  516,  517,  518,  519,  520,  521,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 522
#if YYDEBUG
const char * const yyname[] = {
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
const char * const yyrule[] = {
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
"non_reserved_word : BLOCK",
"non_reserved_word : BACKUP",
"non_reserved_word : KW_DIFFERENCE",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
static int yynerrs;

int DSQL_yyerrflag;


int DSQL_yychar;

short *DSQL_DSQL_yyssp;
static YYSTYPE *yyvsp;
static YYSTYPE yyval;
static YYSTYPE yylval;
short *DSQL_yyss;
short *DSQL_DSQL_yysslim;
static YYSTYPE *yyvs;
static int yystacksize;
#line 4189 "parse.y"


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
	tsql* tdsql = GET_THREAD_DATA;

	if (field_name == NULL)
	{
		dsql_fld* field =
			FB_NEW_RPT(*tdsql->tsql_default, sizeof (INTERNAL_FIELD_NAME)) dsql_fld;
		strcpy (field->fld_name, (TEXT*) INTERNAL_FIELD_NAME);
		return field;
	}
	const dsql_str* string = (dsql_str*) field_name->nod_arg[1];
	dsql_fld* field =
		FB_NEW_RPT(*tdsql->tsql_default, strlen ((SCHAR*) string->str_data)) dsql_fld;
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
	tsql* tdsql = GET_THREAD_DATA;
		   
	dsql_fil* temp_file = FB_NEW(*tdsql->tsql_default) dsql_fil;

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
	tsql* tdsql = GET_THREAD_DATA;

	if (!node)
		return node;

	DsqlNodStack stack;
	stack_nodes (node, stack);
	USHORT l = stack.getCount();

	dsql_nod* old  = node;
	node = FB_NEW_RPT(*tdsql->tsql_default, l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	dsql_nod** ptr = node->nod_arg + node->nod_count;

	while (stack.notEmpty())
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
	tsql* tdsql = GET_THREAD_DATA;

	dsql_nod* node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
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
	tsql* tdsql = GET_THREAD_DATA;

	dsql_nod* node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	VA_START (ptr, count);

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
	tsql* tdsql = GET_THREAD_DATA;

	dsql_nod* node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	VA_START (ptr, count);

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

#line 6319 "parse.cpp"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = DSQL_DSQL_yyssp - DSQL_yyss;
    newss = DSQL_yyss ? (short *)realloc(DSQL_yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    DSQL_yyss = newss;
    DSQL_DSQL_yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    DSQL_DSQL_yysslim = DSQL_yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
dsql_yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            lex.dsql_debug = yyn - '0';
    }
#endif

    yynerrs = 0;
    DSQL_yyerrflag = 0;
    DSQL_yychar = (-1);

    if (DSQL_yyss == NULL && yygrowstack()) goto yyoverflow;
    DSQL_DSQL_yyssp = DSQL_yyss;
    yyvsp = yyvs;
    *DSQL_DSQL_yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
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
        if (DSQL_DSQL_yyssp >= DSQL_DSQL_yysslim && yygrowstack())
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
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
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
                if (DSQL_DSQL_yyssp >= DSQL_DSQL_yysslim && yygrowstack())
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
#line 537 "parse.y"
{ DSQL_parse = yyvsp[0]; }
break;
case 2:
#line 539 "parse.y"
{ DSQL_parse = yyvsp[-1]; }
break;
case 22:
#line 562 "parse.y"
{ prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 23:
#line 571 "parse.y"
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 24:
#line 575 "parse.y"
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 25:
#line 579 "parse.y"
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-4], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 26:
#line 583 "parse.y"
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 27:
#line 586 "parse.y"
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 29:
#line 592 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 30:
#line 596 "parse.y"
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 31:
#line 598 "parse.y"
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 32:
#line 600 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 34:
#line 605 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 35:
#line 609 "parse.y"
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
break;
case 36:
#line 613 "parse.y"
{ yyval = make_node (nod_select, (int) 0, NULL); }
break;
case 37:
#line 615 "parse.y"
{ yyval = make_node (nod_insert, (int) 0, NULL); }
break;
case 38:
#line 617 "parse.y"
{ yyval = make_node (nod_delete, (int) 0, NULL); }
break;
case 39:
#line 619 "parse.y"
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
break;
case 40:
#line 621 "parse.y"
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
break;
case 41:
#line 625 "parse.y"
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 42:
#line 627 "parse.y"
{ yyval = 0; }
break;
case 43:
#line 631 "parse.y"
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 44:
#line 633 "parse.y"
{ yyval = 0; }
break;
case 45:
#line 637 "parse.y"
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
break;
case 46:
#line 645 "parse.y"
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-5]); }
break;
case 47:
#line 650 "parse.y"
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 48:
#line 655 "parse.y"
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 49:
#line 660 "parse.y"
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 50:
#line 665 "parse.y"
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 51:
#line 670 "parse.y"
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 52:
#line 674 "parse.y"
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, make_list(yyvsp[-2]), make_list(yyvsp[0]),
				NULL, NULL); }
break;
case 53:
#line 680 "parse.y"
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 55:
#line 685 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 56:
#line 687 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 57:
#line 689 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 58:
#line 693 "parse.y"
{ yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); }
break;
case 59:
#line 695 "parse.y"
{ yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); }
break;
case 60:
#line 697 "parse.y"
{ yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); }
break;
case 61:
#line 699 "parse.y"
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 63:
#line 704 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 64:
#line 711 "parse.y"
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 65:
#line 713 "parse.y"
{ yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); }
break;
case 66:
#line 715 "parse.y"
{ yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); }
break;
case 68:
#line 720 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 69:
#line 724 "parse.y"
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 71:
#line 729 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 72:
#line 733 "parse.y"
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 73:
#line 735 "parse.y"
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 74:
#line 742 "parse.y"
{ yyval = yyvsp[0];}
break;
case 75:
#line 746 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 76:
#line 748 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 77:
#line 754 "parse.y"
{ yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); }
break;
case 79:
#line 760 "parse.y"
{ lex.g_field->fld_dtype = dtype_blob; }
break;
case 80:
#line 762 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-2]; }
break;
case 81:
#line 768 "parse.y"
{ yyval = NULL; }
break;
case 83:
#line 771 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 85:
#line 776 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 86:
#line 782 "parse.y"
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
							  yyvsp[-1], NULL); }
break;
case 87:
#line 785 "parse.y"
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				yyvsp[-3], MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG)); }
break;
case 89:
#line 792 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 90:
#line 795 "parse.y"
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((dsql_str*) FUN_reference, CONSTANT_SLONG));}
break;
case 91:
#line 798 "parse.y"
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((dsql_str*) (-1 * FUN_reference), CONSTANT_SLONG));}
break;
case 92:
#line 802 "parse.y"
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((dsql_str*) FUN_value, CONSTANT_SLONG));}
break;
case 93:
#line 806 "parse.y"
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG));}
break;
case 94:
#line 809 "parse.y"
{ yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
break;
case 95:
#line 815 "parse.y"
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 96:
#line 823 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 97:
#line 827 "parse.y"
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 98:
#line 830 "parse.y"
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 99:
#line 833 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 100:
#line 835 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 101:
#line 837 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 102:
#line 839 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 103:
#line 841 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 104:
#line 843 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 105:
#line 845 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 106:
#line 847 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 107:
#line 849 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 108:
#line 854 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 109:
#line 858 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 110:
#line 860 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 111:
#line 862 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 112:
#line 872 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 113:
#line 876 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 114:
#line 878 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 115:
#line 889 "parse.y"
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 116:
#line 891 "parse.y"
{ yyval = NULL; }
break;
case 117:
#line 895 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 119:
#line 898 "parse.y"
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 120:
#line 905 "parse.y"
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 121:
#line 910 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 122:
#line 912 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 123:
#line 914 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 124:
#line 918 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 125:
#line 920 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 126:
#line 924 "parse.y"
{ yyval = (dsql_nod*) 0;}
break;
case 127:
#line 926 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 128:
#line 930 "parse.y"
{ yyval = NULL; }
break;
case 131:
#line 936 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 132:
#line 950 "parse.y"
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 133:
#line 961 "parse.y"
{ yyval = NULL; }
break;
case 134:
#line 963 "parse.y"
{ yyval = NULL; }
break;
case 135:
#line 967 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 136:
#line 969 "parse.y"
{ yyval = NULL; }
break;
case 137:
#line 973 "parse.y"
{ yyval = NULL; }
break;
case 140:
#line 979 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 141:
#line 982 "parse.y"
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 144:
#line 991 "parse.y"
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 145:
#line 995 "parse.y"
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 146:
#line 1004 "parse.y"
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
break;
case 147:
#line 1012 "parse.y"
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
break;
case 148:
#line 1021 "parse.y"
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 151:
#line 1030 "parse.y"
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
break;
case 152:
#line 1036 "parse.y"
{yyval = NULL;}
break;
case 155:
#line 1042 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 156:
#line 1046 "parse.y"
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 157:
#line 1048 "parse.y"
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 158:
#line 1050 "parse.y"
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 159:
#line 1052 "parse.y"
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 160:
#line 1054 "parse.y"
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 161:
#line 1058 "parse.y"
{yyval = NULL;}
break;
case 164:
#line 1064 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 166:
#line 1073 "parse.y"
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 167:
#line 1075 "parse.y"
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 168:
#line 1124 "parse.y"
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 169:
#line 1181 "parse.y"
{ lex.g_file  = make_file();}
break;
case 174:
#line 1193 "parse.y"
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 175:
#line 1195 "parse.y"
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 182:
#line 1212 "parse.y"
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 183:
#line 1217 "parse.y"
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 184:
#line 1222 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 185:
#line 1224 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 186:
#line 1226 "parse.y"
{ yyval = NULL; }
break;
case 188:
#line 1231 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 191:
#line 1244 "parse.y"
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 192:
#line 1247 "parse.y"
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 193:
#line 1251 "parse.y"
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 194:
#line 1260 "parse.y"
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 197:
#line 1270 "parse.y"
{ yyval = NULL; }
break;
case 198:
#line 1272 "parse.y"
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 199:
#line 1277 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 200:
#line 1279 "parse.y"
{ yyval = NULL; }
break;
case 201:
#line 1284 "parse.y"
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 202:
#line 1290 "parse.y"
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 203:
#line 1296 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 204:
#line 1300 "parse.y"
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 205:
#line 1306 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 206:
#line 1308 "parse.y"
{ yyval = NULL; }
break;
case 210:
#line 1319 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 211:
#line 1321 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 212:
#line 1323 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 213:
#line 1327 "parse.y"
{ yyval = NULL; }
break;
case 216:
#line 1333 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 217:
#line 1337 "parse.y"
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 218:
#line 1342 "parse.y"
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 219:
#line 1345 "parse.y"
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 221:
#line 1350 "parse.y"
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 222:
#line 1352 "parse.y"
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 223:
#line 1360 "parse.y"
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 224:
#line 1364 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 225:
#line 1365 "parse.y"
{ yyval = NULL ;}
break;
case 230:
#line 1375 "parse.y"
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 231:
#line 1379 "parse.y"
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 232:
#line 1385 "parse.y"
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 233:
#line 1390 "parse.y"
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 234:
#line 1397 "parse.y"
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 235:
#line 1402 "parse.y"
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 236:
#line 1409 "parse.y"
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 237:
#line 1411 "parse.y"
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 238:
#line 1413 "parse.y"
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 239:
#line 1415 "parse.y"
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 240:
#line 1417 "parse.y"
{ yyval = NULL;}
break;
case 241:
#line 1421 "parse.y"
{ yyval = yyvsp[0];}
break;
case 242:
#line 1424 "parse.y"
{ yyval = yyvsp[0];}
break;
case 243:
#line 1428 "parse.y"
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 244:
#line 1431 "parse.y"
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 245:
#line 1434 "parse.y"
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 246:
#line 1437 "parse.y"
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 247:
#line 1451 "parse.y"
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 248:
#line 1462 "parse.y"
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 249:
#line 1472 "parse.y"
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 250:
#line 1482 "parse.y"
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 251:
#line 1487 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 252:
#line 1489 "parse.y"
{ yyval = NULL; }
break;
case 253:
#line 1493 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 254:
#line 1495 "parse.y"
{ yyval = NULL; }
break;
case 256:
#line 1500 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 257:
#line 1505 "parse.y"
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 259:
#line 1511 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 260:
#line 1515 "parse.y"
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 261:
#line 1520 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 262:
#line 1522 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 263:
#line 1524 "parse.y"
{ yyval = (dsql_nod*) NULL; }
break;
case 264:
#line 1528 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 265:
#line 1530 "parse.y"
{ yyval = NULL; }
break;
case 267:
#line 1535 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 268:
#line 1539 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 271:
#line 1547 "parse.y"
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 272:
#line 1552 "parse.y"
{ yyval = NULL; }
break;
case 273:
#line 1554 "parse.y"
{ yyval = NULL; }
break;
case 274:
#line 1558 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 275:
#line 1560 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 276:
#line 1564 "parse.y"
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 279:
#line 1573 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 280:
#line 1577 "parse.y"
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 281:
#line 1579 "parse.y"
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 282:
#line 1581 "parse.y"
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 284:
#line 1586 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 300:
#line 1605 "parse.y"
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 301:
#line 1607 "parse.y"
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 304:
#line 1613 "parse.y"
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 305:
#line 1615 "parse.y"
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 306:
#line 1619 "parse.y"
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 307:
#line 1623 "parse.y"
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 308:
#line 1628 "parse.y"
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 309:
#line 1632 "parse.y"
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 310:
#line 1637 "parse.y"
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 311:
#line 1641 "parse.y"
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 312:
#line 1645 "parse.y"
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 313:
#line 1647 "parse.y"
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 314:
#line 1651 "parse.y"
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 315:
#line 1657 "parse.y"
{ yyval = NULL; }
break;
case 316:
#line 1661 "parse.y"
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 317:
#line 1666 "parse.y"
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 318:
#line 1671 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 319:
#line 1673 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 320:
#line 1675 "parse.y"
{ yyval = NULL; }
break;
case 321:
#line 1679 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 322:
#line 1681 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 323:
#line 1683 "parse.y"
{ yyval = NULL; }
break;
case 326:
#line 1689 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 327:
#line 1691 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 328:
#line 1695 "parse.y"
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 329:
#line 1699 "parse.y"
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 330:
#line 1701 "parse.y"
{ yyval = NULL; }
break;
case 331:
#line 1705 "parse.y"
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 332:
#line 1707 "parse.y"
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 333:
#line 1709 "parse.y"
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 334:
#line 1714 "parse.y"
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 335:
#line 1716 "parse.y"
{ yyval = NULL; }
break;
case 337:
#line 1721 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 338:
#line 1725 "parse.y"
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 340:
#line 1731 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 341:
#line 1735 "parse.y"
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 342:
#line 1737 "parse.y"
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 343:
#line 1739 "parse.y"
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 344:
#line 1741 "parse.y"
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 348:
#line 1750 "parse.y"
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 349:
#line 1754 "parse.y"
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 350:
#line 1758 "parse.y"
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 351:
#line 1762 "parse.y"
{ yyval = NULL; }
break;
case 352:
#line 1804 "parse.y"
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 353:
#line 1813 "parse.y"
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 354:
#line 1819 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 355:
#line 1821 "parse.y"
{ yyval = NULL; }
break;
case 357:
#line 1826 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 358:
#line 1830 "parse.y"
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 359:
#line 1837 "parse.y"
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 360:
#line 1844 "parse.y"
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 361:
#line 1863 "parse.y"
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 362:
#line 1867 "parse.y"
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 363:
#line 1869 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 364:
#line 1871 "parse.y"
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 365:
#line 1881 "parse.y"
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 366:
#line 1886 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 368:
#line 1891 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 371:
#line 1904 "parse.y"
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 372:
#line 1907 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 373:
#line 1913 "parse.y"
{ lex.beginning = lex_position(); }
break;
case 374:
#line 1917 "parse.y"
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 375:
#line 1923 "parse.y"
{ lex.beginning = lex.last_token; }
break;
case 376:
#line 1927 "parse.y"
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 377:
#line 1933 "parse.y"
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 378:
#line 1937 "parse.y"
{ yyval = 0; }
break;
case 379:
#line 1951 "parse.y"
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 380:
#line 1962 "parse.y"
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 381:
#line 1967 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 382:
#line 1969 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 383:
#line 1971 "parse.y"
{ yyval = NULL; }
break;
case 384:
#line 1975 "parse.y"
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 385:
#line 1979 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 386:
#line 1981 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 387:
#line 1985 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 388:
#line 1987 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 389:
#line 1989 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 390:
#line 1991 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 391:
#line 1993 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 392:
#line 1995 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 393:
#line 1997 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 394:
#line 1999 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 395:
#line 2001 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 396:
#line 2003 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 397:
#line 2005 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 398:
#line 2007 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 399:
#line 2009 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 400:
#line 2011 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 401:
#line 2013 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 402:
#line 2017 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 403:
#line 2019 "parse.y"
{ yyval = NULL; }
break;
case 404:
#line 2023 "parse.y"
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 405:
#line 2029 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 406:
#line 2033 "parse.y"
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 407:
#line 2036 "parse.y"
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 408:
#line 2043 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 409:
#line 2045 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 410:
#line 2047 "parse.y"
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 411:
#line 2050 "parse.y"
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 412:
#line 2053 "parse.y"
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 413:
#line 2058 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 414:
#line 2062 "parse.y"
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 416:
#line 2069 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 417:
#line 2073 "parse.y"
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 418:
#line 2082 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 419:
#line 2086 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 420:
#line 2088 "parse.y"
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 421:
#line 2090 "parse.y"
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 422:
#line 2092 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 423:
#line 2094 "parse.y"
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 425:
#line 2099 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 426:
#line 2103 "parse.y"
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 427:
#line 2105 "parse.y"
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 428:
#line 2107 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 429:
#line 2109 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 430:
#line 2115 "parse.y"
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 431:
#line 2118 "parse.y"
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 432:
#line 2120 "parse.y"
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 433:
#line 2124 "parse.y"
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 461:
#line 2161 "parse.y"
{ yyval = NULL; }
break;
case 462:
#line 2163 "parse.y"
{ yyval = NULL; }
break;
case 463:
#line 2167 "parse.y"
{ yyval = NULL; }
break;
case 464:
#line 2169 "parse.y"
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 465:
#line 2174 "parse.y"
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 466:
#line 2180 "parse.y"
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 467:
#line 2182 "parse.y"
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 468:
#line 2184 "parse.y"
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 469:
#line 2188 "parse.y"
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 470:
#line 2190 "parse.y"
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 471:
#line 2197 "parse.y"
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 473:
#line 2204 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 474:
#line 2208 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 475:
#line 2222 "parse.y"
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 476:
#line 2224 "parse.y"
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 477:
#line 2226 "parse.y"
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 478:
#line 2228 "parse.y"
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 479:
#line 2247 "parse.y"
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 481:
#line 2253 "parse.y"
{ yyval = NULL; }
break;
case 483:
#line 2258 "parse.y"
{ yyval = NULL; }
break;
case 484:
#line 2264 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 485:
#line 2268 "parse.y"
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 486:
#line 2270 "parse.y"
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 487:
#line 2272 "parse.y"
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 488:
#line 2274 "parse.y"
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 489:
#line 2276 "parse.y"
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 490:
#line 2278 "parse.y"
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 491:
#line 2280 "parse.y"
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 492:
#line 2282 "parse.y"
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 493:
#line 2284 "parse.y"
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 494:
#line 2286 "parse.y"
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 495:
#line 2288 "parse.y"
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 496:
#line 2290 "parse.y"
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 501:
#line 2305 "parse.y"
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 502:
#line 2310 "parse.y"
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 504:
#line 2318 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 505:
#line 2322 "parse.y"
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
#line 2331 "parse.y"
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 512:
#line 2344 "parse.y"
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
#line 2361 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 514:
#line 2366 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 515:
#line 2371 "parse.y"
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
#line 2390 "parse.y"
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
#line 2407 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 520:
#line 2422 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 521:
#line 2426 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 522:
#line 2432 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 523:
#line 2438 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 524:
#line 2446 "parse.y"
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 525:
#line 2450 "parse.y"
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 526:
#line 2456 "parse.y"
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 527:
#line 2460 "parse.y"
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 528:
#line 2464 "parse.y"
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 529:
#line 2470 "parse.y"
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 531:
#line 2481 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 532:
#line 2487 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 533:
#line 2493 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 534:
#line 2501 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 535:
#line 2506 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 536:
#line 2511 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 545:
#line 2536 "parse.y"
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 546:
#line 2540 "parse.y"
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
#line 2551 "parse.y"
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 548:
#line 2557 "parse.y"
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
#line 2611 "parse.y"
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
#line 2681 "parse.y"
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
#line 2694 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 554:
#line 2699 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 555:
#line 2704 "parse.y"
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 556:
#line 2711 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 557:
#line 2713 "parse.y"
{ yyval = 0; }
break;
case 561:
#line 2726 "parse.y"
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 562:
#line 2731 "parse.y"
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 563:
#line 2736 "parse.y"
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 567:
#line 2752 "parse.y"
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 568:
#line 2756 "parse.y"
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 569:
#line 2760 "parse.y"
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 570:
#line 2762 "parse.y"
{ yyval = 0; }
break;
case 571:
#line 2766 "parse.y"
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 574:
#line 2774 "parse.y"
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 575:
#line 2778 "parse.y"
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 578:
#line 2786 "parse.y"
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 579:
#line 2788 "parse.y"
{ yyval = NULL; }
break;
case 581:
#line 2793 "parse.y"
{ yyval = NULL; }
break;
case 582:
#line 2797 "parse.y"
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 584:
#line 2802 "parse.y"
{ yyval = NULL; }
break;
case 586:
#line 2807 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 591:
#line 2817 "parse.y"
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 592:
#line 2819 "parse.y"
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 593:
#line 2823 "parse.y"
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 594:
#line 2825 "parse.y"
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 595:
#line 2829 "parse.y"
{ yyval = yyvsp[0];}
break;
case 597:
#line 2834 "parse.y"
{ yyval = yyvsp[0];}
break;
case 598:
#line 2836 "parse.y"
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 599:
#line 2838 "parse.y"
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 600:
#line 2842 "parse.y"
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 601:
#line 2844 "parse.y"
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 602:
#line 2846 "parse.y"
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 603:
#line 2850 "parse.y"
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 604:
#line 2852 "parse.y"
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 605:
#line 2854 "parse.y"
{ yyval = 0; }
break;
case 606:
#line 2858 "parse.y"
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 607:
#line 2862 "parse.y"
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 608:
#line 2864 "parse.y"
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 609:
#line 2866 "parse.y"
{ yyval = (dsql_nod*) 0; }
break;
case 610:
#line 2870 "parse.y"
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 611:
#line 2872 "parse.y"
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 613:
#line 2877 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 614:
#line 2881 "parse.y"
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 615:
#line 2885 "parse.y"
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 616:
#line 2887 "parse.y"
{ yyval = 0; }
break;
case 618:
#line 2892 "parse.y"
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 619:
#line 2897 "parse.y"
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 620:
#line 2905 "parse.y"
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 621:
#line 2909 "parse.y"
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 622:
#line 2911 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 623:
#line 2913 "parse.y"
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 624:
#line 2917 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 625:
#line 2919 "parse.y"
{ yyval = 0; }
break;
case 627:
#line 2924 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 628:
#line 2928 "parse.y"
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 629:
#line 2932 "parse.y"
{ yyval = 0; }
break;
case 630:
#line 2934 "parse.y"
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 631:
#line 2936 "parse.y"
{ yyval = 0; }
break;
case 632:
#line 2940 "parse.y"
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 633:
#line 2942 "parse.y"
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 634:
#line 2946 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 635:
#line 2948 "parse.y"
{ yyval = 0; }
break;
case 636:
#line 2953 "parse.y"
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 637:
#line 2956 "parse.y"
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 638:
#line 2963 "parse.y"
{ yyval = NULL; }
break;
case 639:
#line 2967 "parse.y"
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 640:
#line 2969 "parse.y"
{ yyval = 0; }
break;
case 641:
#line 2973 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 642:
#line 2975 "parse.y"
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 643:
#line 2979 "parse.y"
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 644:
#line 2981 "parse.y"
{ yyval = 0; }
break;
case 645:
#line 2995 "parse.y"
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 646:
#line 3009 "parse.y"
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 647:
#line 3014 "parse.y"
{ lex.limit_clause = true; }
break;
case 648:
#line 3018 "parse.y"
{ lex.limit_clause = false; }
break;
case 649:
#line 3022 "parse.y"
{ lex.first_detection = true; }
break;
case 650:
#line 3026 "parse.y"
{ lex.first_detection = false; }
break;
case 651:
#line 3030 "parse.y"
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 652:
#line 3032 "parse.y"
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 653:
#line 3034 "parse.y"
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 654:
#line 3036 "parse.y"
{ yyval = 0; }
break;
case 655:
#line 3040 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 656:
#line 3042 "parse.y"
{ yyval = yyvsp[-2]; }
break;
case 657:
#line 3044 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 658:
#line 3048 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 659:
#line 3050 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 660:
#line 3052 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 661:
#line 3056 "parse.y"
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 662:
#line 3058 "parse.y"
{ yyval = 0; }
break;
case 663:
#line 3062 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 664:
#line 3064 "parse.y"
{ yyval = 0; }
break;
case 666:
#line 3069 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 668:
#line 3074 "parse.y"
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 671:
#line 3084 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 673:
#line 3089 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 677:
#line 3100 "parse.y"
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 679:
#line 3105 "parse.y"
{ yyval = NULL; }
break;
case 680:
#line 3109 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 681:
#line 3111 "parse.y"
{ yyval = NULL; }
break;
case 683:
#line 3116 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 684:
#line 3120 "parse.y"
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 685:
#line 3122 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 686:
#line 3126 "parse.y"
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 687:
#line 3129 "parse.y"
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 688:
#line 3134 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 689:
#line 3136 "parse.y"
{ yyval = NULL; }
break;
case 691:
#line 3141 "parse.y"
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 692:
#line 3146 "parse.y"
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 693:
#line 3151 "parse.y"
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 694:
#line 3153 "parse.y"
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 695:
#line 3155 "parse.y"
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 696:
#line 3157 "parse.y"
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 697:
#line 3159 "parse.y"
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 698:
#line 3161 "parse.y"
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 699:
#line 3163 "parse.y"
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 700:
#line 3165 "parse.y"
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 701:
#line 3172 "parse.y"
{ yyval = make_list (yyvsp[0]); }
break;
case 702:
#line 3174 "parse.y"
{ yyval = 0; }
break;
case 704:
#line 3179 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 706:
#line 3188 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 707:
#line 3190 "parse.y"
{ yyval = 0; }
break;
case 708:
#line 3194 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 709:
#line 3196 "parse.y"
{ yyval = 0; }
break;
case 710:
#line 3203 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 711:
#line 3205 "parse.y"
{ yyval = 0; }
break;
case 712:
#line 3209 "parse.y"
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 713:
#line 3213 "parse.y"
{ yyval = 0; }
break;
case 714:
#line 3215 "parse.y"
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 715:
#line 3217 "parse.y"
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 716:
#line 3223 "parse.y"
{ yyval = 0; }
break;
case 717:
#line 3225 "parse.y"
{ yyval = 0; }
break;
case 719:
#line 3230 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 720:
#line 3234 "parse.y"
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 723:
#line 3240 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 724:
#line 3244 "parse.y"
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 725:
#line 3246 "parse.y"
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 726:
#line 3248 "parse.y"
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 728:
#line 3253 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 729:
#line 3257 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 730:
#line 3259 "parse.y"
{ yyval = 0; }
break;
case 731:
#line 3266 "parse.y"
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 732:
#line 3269 "parse.y"
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 735:
#line 3280 "parse.y"
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 736:
#line 3284 "parse.y"
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 737:
#line 3288 "parse.y"
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 740:
#line 3299 "parse.y"
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 741:
#line 3304 "parse.y"
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 743:
#line 3310 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 744:
#line 3314 "parse.y"
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 748:
#line 3323 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 749:
#line 3330 "parse.y"
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 750:
#line 3332 "parse.y"
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 751:
#line 3336 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 752:
#line 3338 "parse.y"
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 756:
#line 3347 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 757:
#line 3351 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 759:
#line 3356 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 762:
#line 3365 "parse.y"
{ yyval = NULL; }
break;
case 763:
#line 3369 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 765:
#line 3374 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 767:
#line 3380 "parse.y"
{ yyval = NULL; }
break;
case 768:
#line 3384 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 770:
#line 3389 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 772:
#line 3395 "parse.y"
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 773:
#line 3398 "parse.y"
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 774:
#line 3403 "parse.y"
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 776:
#line 3411 "parse.y"
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 778:
#line 3419 "parse.y"
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 780:
#line 3422 "parse.y"
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
#line 3424 "parse.y"
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
#line 3429 "parse.y"
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 784:
#line 3431 "parse.y"
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
#line 3433 "parse.y"
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
#line 3437 "parse.y"
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
#line 3439 "parse.y"
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
#line 3444 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 790:
#line 3446 "parse.y"
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 801:
#line 3464 "parse.y"
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
#line 3466 "parse.y"
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 803:
#line 3468 "parse.y"
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 804:
#line 3470 "parse.y"
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
#line 3472 "parse.y"
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 806:
#line 3474 "parse.y"
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 807:
#line 3476 "parse.y"
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 808:
#line 3478 "parse.y"
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
#line 3485 "parse.y"
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
#line 3487 "parse.y"
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
#line 3489 "parse.y"
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
#line 3491 "parse.y"
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
#line 3493 "parse.y"
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
#line 3495 "parse.y"
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
#line 3497 "parse.y"
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
#line 3499 "parse.y"
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
#line 3501 "parse.y"
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
#line 3503 "parse.y"
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
#line 3505 "parse.y"
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
#line 3507 "parse.y"
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
#line 3509 "parse.y"
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
#line 3511 "parse.y"
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
#line 3513 "parse.y"
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
#line 3515 "parse.y"
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
#line 3526 "parse.y"
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 828:
#line 3528 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 829:
#line 3533 "parse.y"
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 830:
#line 3535 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 831:
#line 3537 "parse.y"
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 832:
#line 3539 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 833:
#line 3544 "parse.y"
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 834:
#line 3546 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 835:
#line 3550 "parse.y"
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 836:
#line 3552 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 837:
#line 3556 "parse.y"
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 838:
#line 3558 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 839:
#line 3560 "parse.y"
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 840:
#line 3562 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 841:
#line 3566 "parse.y"
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 842:
#line 3570 "parse.y"
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 843:
#line 3574 "parse.y"
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 844:
#line 3576 "parse.y"
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 845:
#line 3580 "parse.y"
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 846:
#line 3585 "parse.y"
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 847:
#line 3590 "parse.y"
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 848:
#line 3597 "parse.y"
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 849:
#line 3602 "parse.y"
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 850:
#line 3607 "parse.y"
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 852:
#line 3617 "parse.y"
{ yyval = make_list (yyvsp[-1]); }
break;
case 853:
#line 3621 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 854:
#line 3634 "parse.y"
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 855:
#line 3648 "parse.y"
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 865:
#line 3666 "parse.y"
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 866:
#line 3668 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 867:
#line 3670 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 868:
#line 3677 "parse.y"
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 869:
#line 3679 "parse.y"
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 870:
#line 3681 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 871:
#line 3688 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 872:
#line 3695 "parse.y"
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 873:
#line 3702 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 874:
#line 3704 "parse.y"
{ yyval = yyvsp[-1]; }
break;
case 878:
#line 3709 "parse.y"
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 879:
#line 3711 "parse.y"
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 880:
#line 3713 "parse.y"
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 881:
#line 3717 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 882:
#line 3721 "parse.y"
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
case 883:
#line 3737 "parse.y"
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
case 884:
#line 3753 "parse.y"
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 885:
#line 3757 "parse.y"
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 887:
#line 3762 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 889:
#line 3767 "parse.y"
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 890:
#line 3771 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 891:
#line 3773 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 892:
#line 3775 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 893:
#line 3777 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 894:
#line 3779 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 896:
#line 3784 "parse.y"
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 897:
#line 3786 "parse.y"
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
case 898:
#line 3802 "parse.y"
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
case 899:
#line 3818 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 900:
#line 3822 "parse.y"
{ yyval = make_parameter (); }
break;
case 901:
#line 3826 "parse.y"
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 902:
#line 3828 "parse.y"
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 903:
#line 3832 "parse.y"
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 904:
#line 3836 "parse.y"
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 905:
#line 3839 "parse.y"
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 906:
#line 3842 "parse.y"
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 907:
#line 3845 "parse.y"
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 908:
#line 3848 "parse.y"
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 909:
#line 3853 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 910:
#line 3855 "parse.y"
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 912:
#line 3861 "parse.y"
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 913:
#line 3865 "parse.y"
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 914:
#line 3872 "parse.y"
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 915:
#line 3879 "parse.y"
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 916:
#line 3886 "parse.y"
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 918:
#line 3894 "parse.y"
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 919:
#line 3898 "parse.y"
{ yyval = yyvsp[0];}
break;
case 924:
#line 3910 "parse.y"
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 925:
#line 3912 "parse.y"
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 926:
#line 3914 "parse.y"
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 927:
#line 3917 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 928:
#line 3924 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 929:
#line 3933 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 930:
#line 3940 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 931:
#line 3949 "parse.y"
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 932:
#line 3951 "parse.y"
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 933:
#line 3953 "parse.y"
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 934:
#line 3955 "parse.y"
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 935:
#line 3961 "parse.y"
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 937:
#line 3973 "parse.y"
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 939:
#line 3978 "parse.y"
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 940:
#line 3985 "parse.y"
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 941:
#line 3991 "parse.y"
{ yyval = yyvsp[0]; }
break;
case 942:
#line 3993 "parse.y"
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 943:
#line 3997 "parse.y"
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 944:
#line 3999 "parse.y"
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 945:
#line 4003 "parse.y"
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 948:
#line 4013 "parse.y"
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 949:
#line 4017 "parse.y"
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 952:
#line 4025 "parse.y"
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 953:
#line 4027 "parse.y"
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 954:
#line 4031 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 955:
#line 4033 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 956:
#line 4037 "parse.y"
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 957:
#line 4039 "parse.y"
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 958:
#line 4043 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 959:
#line 4045 "parse.y"
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 963:
#line 4058 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 964:
#line 4060 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 965:
#line 4062 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 966:
#line 4064 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 967:
#line 4066 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 968:
#line 4068 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 969:
#line 4070 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 970:
#line 4072 "parse.y"
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 973:
#line 4080 "parse.y"
{ yyval = make_node (nod_null, 0, NULL); }
break;
#line 9899 "parse.cpp"
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
    if (DSQL_DSQL_yyssp >= DSQL_DSQL_yysslim && yygrowstack())
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
