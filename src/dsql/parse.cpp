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
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
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

/*const int MIN_CACHE_BUFFERS	= 250;*/
/*const int DEF_CACHE_BUFFERS	= 1000;*/

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
#define BEFORE 269
#define BEGIN 270
#define BETWEEN 271
#define BLOB 272
#define BY 273
#define CAST 274
#define CHARACTER 275
#define CHECK 276
#define COLLATE 277
#define COMMA 278
#define COMMIT 279
#define COMMITTED 280
#define COMPUTED 281
#define CONCATENATE 282
#define CONDITIONAL 283
#define CONSTRAINT 284
#define CONTAINING 285
#define COUNT 286
#define CREATE 287
#define CSTRING 288
#define CURRENT 289
#define CURSOR 290
#define DATABASE 291
#define DATE 292
#define DB_KEY 293
#define KW_DEBUG 294
#define DECIMAL 295
#define DECLARE 296
#define DEFAULT 297
#define KW_DELETE 298
#define DESC 299
#define DISTINCT 300
#define DO 301
#define DOMAIN 302
#define DROP 303
#define ELSE 304
#define END 305
#define ENTRY_POINT 306
#define EQL 307
#define ESCAPE 308
#define EXCEPTION 309
#define EXECUTE 310
#define EXISTS 311
#define EXIT 312
#define EXTERNAL 313
#define FILTER 314
#define FOR 315
#define FOREIGN 316
#define FROM 317
#define FULL 318
#define FUNCTION 319
#define GDSCODE 320
#define GEQ 321
#define GENERATOR 322
#define GEN_ID 323
#define GRANT 324
#define GROUP 325
#define GTR 326
#define HAVING 327
#define IF 328
#define KW_IN 329
#define INACTIVE 330
#define INNER 331
#define INPUT_TYPE 332
#define INDEX 333
#define INSERT 334
#define INTEGER 335
#define INTO 336
#define IS 337
#define ISOLATION 338
#define JOIN 339
#define KEY 340
#define KW_CHAR 341
#define KW_DEC 342
#define KW_DOUBLE 343
#define KW_FILE 344
#define KW_FLOAT 345
#define KW_INT 346
#define KW_LONG 347
#define KW_NULL 348
#define KW_NUMERIC 349
#define KW_UPPER 350
#define KW_VALUE 351
#define LENGTH 352
#define LPAREN 353
#define LEFT 354
#define LEQ 355
#define LEVEL 356
#define LIKE 357
#define LSS 358
#define MANUAL 359
#define MAXIMUM 360
#define MAX_SEGMENT 361
#define MERGE 362
#define MESSAGE 363
#define MINIMUM 364
#define MODULE_NAME 365
#define NAMES 366
#define NATIONAL 367
#define NATURAL 368
#define NCHAR 369
#define NEQ 370
#define NO 371
#define NOT 372
#define NOT_GTR 373
#define NOT_LSS 374
#define OF 375
#define ON 376
#define ONLY 377
#define OPTION 378
#define OR 379
#define ORDER 380
#define OUTER 381
#define OUTPUT_TYPE 382
#define OVERFLOW 383
#define PAGE 384
#define PAGES 385
#define KW_PAGE_SIZE 386
#define PARAMETER 387
#define PASSWORD 388
#define PLAN 389
#define POSITION 390
#define POST_EVENT 391
#define PRECISION 392
#define PRIMARY 393
#define PRIVILEGES 394
#define PROCEDURE 395
#define PROTECTED 396
#define READ 397
#define REAL 398
#define REFERENCES 399
#define RESERVING 400
#define RETAIN 401
#define RETURNING_VALUES 402
#define RETURNS 403
#define REVOKE 404
#define RIGHT 405
#define RPAREN 406
#define ROLLBACK 407
#define SEGMENT 408
#define SELECT 409
#define SET 410
#define SHADOW 411
#define KW_SHARED 412
#define SINGULAR 413
#define KW_SIZE 414
#define SMALLINT 415
#define SNAPSHOT 416
#define SOME 417
#define SORT 418
#define SQLCODE 419
#define STABILITY 420
#define STARTING 421
#define STATISTICS 422
#define SUB_TYPE 423
#define SUSPEND 424
#define SUM 425
#define TABLE 426
#define THEN 427
#define TO 428
#define TRANSACTION 429
#define TRIGGER 430
#define UNCOMMITTED 431
#define UNION 432
#define UNIQUE 433
#define UPDATE 434
#define USER 435
#define VALUES 436
#define VARCHAR 437
#define VARIABLE 438
#define VARYING 439
#define VERSION 440
#define VIEW 441
#define WAIT 442
#define WHEN 443
#define WHERE 444
#define WHILE 445
#define WITH 446
#define WORK 447
#define WRITE 448
#define FLOAT_NUMBER 449
#define NUMBER 450
#define NUMERIC 451
#define SYMBOL 452
#define STRING 453
#define INTRODUCER 454
#define ACTION 455
#define ADMIN 456
#define CASCADE 457
#define FREE_IT 458
#define RESTRICT 459
#define ROLE 460
#define COLUMN 461
#define TYPE 462
#define EXTRACT 463
#define YEAR 464
#define MONTH 465
#define DAY 466
#define HOUR 467
#define MINUTE 468
#define SECOND 469
#define WEEKDAY 470
#define YEARDAY 471
#define TIME 472
#define TIMESTAMP 473
#define CURRENT_DATE 474
#define CURRENT_TIME 475
#define CURRENT_TIMESTAMP 476
#define NUMBER64BIT 477
#define SCALEDINT 478
#define CURRENT_USER 479
#define CURRENT_ROLE 480
#define KW_BREAK 481
#define SUBSTRING 482
#define RECREATE 483
#define KW_DESCRIPTOR 484
#define FIRST 485
#define SKIP 486
#define CURRENT_CONNECTION 487
#define CURRENT_TRANSACTION 488
#define BIGINT 489
#define CASE 490
#define NULLIF 491
#define COALESCE 492
#define USING 493
#define NULLS 494
#define LAST 495
#define ROW_COUNT 496
#define LOCK 497
#define SAVEPOINT 498
#define RELEASE 499
#define STATEMENT 500
#define LEAVE 501
#define INSERTING 502
#define UPDATING 503
#define DELETING 504
#define KW_INSERTING 505
#define KW_UPDATING 506
#define KW_DELETING 507
#define BACKUP 508
#define KW_DIFFERENCE 509
#define OPEN 510
#define CLOSE 511
#define FETCH 512
#define ROWS 513
#define BLOCK 514
#define IIF 515
#define SCALAR_ARRAY 516
#define CROSS 517
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
   50,   50,   50,   57,   57,   58,   60,   60,   60,   60,
   51,   51,   61,   61,   62,   62,   62,   62,   62,   47,
   64,   64,    5,   66,   66,   66,   66,   66,   66,   66,
   66,   66,   66,   66,   13,   81,   81,   81,   14,   85,
   85,   68,   68,   71,   71,   71,   79,   94,   94,   94,
   95,   95,   96,   96,   97,   97,  101,  101,   78,  104,
  104,  106,  106,  107,  107,  110,  110,  111,  112,  112,
  113,  114,   76,   80,   77,   98,   98,  117,  118,  118,
  120,  120,  121,  121,  121,  121,  121,  119,  119,  122,
  122,  123,  123,  123,  102,  125,  126,  126,  127,  127,
  128,  128,  129,  129,  129,  100,  100,  100,   73,   83,
  130,  130,  130,  131,  131,  132,  132,  133,  133,  133,
  139,   90,   90,  135,  135,  108,  108,  103,  143,  144,
   59,  136,  136,  109,  109,  109,  109,  109,  109,  137,
  137,  151,  151,  152,  154,  154,  154,  154,  154,  134,
  153,  153,  158,  158,  158,  158,  160,  161,  162,  156,
  156,  157,  155,  155,  155,  155,  155,  163,  164,  165,
  165,  165,  165,   72,   82,   86,  170,  166,  166,  167,
  167,  171,  171,  173,  172,  172,  175,  174,  174,  174,
  168,  168,  176,  176,  177,  179,  179,  180,  178,  178,
  182,  182,  181,  184,  184,  169,  186,  186,  186,  187,
  187,  185,  185,  185,  185,  185,  185,  185,  185,  185,
  185,  185,  185,  185,  185,  185,  185,  185,  185,  185,
  190,  190,  191,  192,  193,  194,  199,  200,  195,  195,
  196,  208,  197,  209,  203,  203,  203,  204,  204,  204,
  206,  206,  206,  206,  198,  205,  205,  201,  201,  201,
  207,  207,  188,  188,  214,  215,  215,  216,  216,  216,
  216,  202,  202,  202,  218,  220,  219,  221,   11,   12,
  222,  222,  223,  223,  224,   75,   84,  141,  228,   91,
   93,  227,  227,   74,   87,  229,  229,  229,  230,  233,
  233,  234,  234,  234,  234,  234,  234,  234,  234,  234,
  234,  234,  234,  234,  234,  234,  231,  231,  232,    2,
  236,  236,  236,  236,  236,  236,  236,  244,  245,  242,
  242,  246,  246,  246,  246,  246,  246,  246,  237,  237,
  247,  247,  247,  247,  247,  247,  247,  241,  252,  252,
  252,  252,  252,  252,  252,  252,  252,  252,  252,  252,
  252,  252,  252,  252,  252,  252,  252,  252,  252,  252,
  252,  252,  252,  252,  252,  252,  252,  249,  249,  251,
  251,  250,  248,  248,  248,  243,  243,  239,  240,  240,
  254,  254,  254,  254,  254,  238,  255,  255,  256,  256,
    8,  257,  257,  257,  257,  257,  257,  257,  257,  257,
  257,  257,  257,  105,  105,  138,  138,  260,  260,  263,
  263,  265,  265,   54,   54,  262,  262,  262,  262,  262,
  262,  262,  262,  262,  270,  270,  261,  261,  261,  261,
  272,  272,  271,  271,  271,   56,   56,  267,  267,  267,
  264,  264,  264,  276,  276,  276,  275,  275,  274,  274,
  274,  268,  268,  277,  277,  277,  278,  278,  269,  269,
  269,  269,  279,  279,   19,   19,   19,  281,  281,  281,
   17,   17,   17,  283,  284,  287,  287,  285,  289,  289,
    4,   16,  288,  288,  290,  290,  291,  291,  280,  292,
  292,  293,  293,  294,  294,  294,  294,  295,  295,  296,
  296,  297,  297,  299,  299,  299,  300,  300,  300,  301,
  301,  301,  298,  303,  303,  303,  304,  304,  302,  302,
  305,  307,  307,  306,  306,  282,   18,  308,  308,  310,
  310,  309,  309,  226,  314,  315,  311,  311,  311,  316,
  318,  327,  328,  329,  330,  319,  319,  319,  319,  331,
  331,  331,  332,  332,  332,  320,  320,  321,  321,  334,
  334,  335,  335,  336,  336,  322,  338,  338,  339,  339,
  341,  341,  341,  343,  344,  344,  345,  345,  347,  347,
  340,  340,  340,  348,  349,  350,  352,  352,  353,  354,
  342,  342,  355,  355,  356,  356,   32,  351,  351,  351,
  351,  351,  357,  357,  324,  324,  358,  358,  359,  325,
  325,  323,  323,  326,  326,  360,  361,  361,  361,  361,
  361,  362,  362,  363,  363,  364,  364,  365,  365,  365,
  366,  366,  367,  367,  312,  312,  368,  368,  369,   69,
   69,   69,  371,  371,  370,  370,  313,  313,  313,   10,
   10,    7,    7,  373,  374,  375,   20,   20,  376,  377,
  378,  378,  189,    3,    3,  380,  380,  380,  382,  382,
  271,  381,  381,  383,  383,   35,   35,   89,   88,   88,
  372,  372,  384,  385,  385,  212,  212,  212,  140,  379,
  379,  115,  115,  115,  115,  115,  389,  389,  389,  389,
  389,  389,  388,  388,  388,  391,  391,  391,  391,  391,
  391,  391,  391,  391,  391,  391,  392,  392,  392,  392,
  392,  392,  392,  392,  398,  398,  398,  398,  398,  398,
  398,  398,  398,  398,  398,  398,  398,  398,  398,  398,
  403,  403,  393,  393,  394,  394,  395,  395,  395,  395,
  396,  396,  400,  400,  401,  401,  401,  401,  399,  402,
  397,  397,  387,  387,  387,  390,  390,  390,  404,  404,
  405,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,  150,  150,
  150,  406,  211,  211,  145,  145,  412,  412,  412,  412,
  412,  408,  408,  408,  408,  408,  225,  146,  146,  147,
  148,  148,  148,  148,  148,   52,   52,   21,   21,  235,
  413,   55,  273,  266,  266,   99,  407,  407,  407,  407,
  414,  414,  414,  414,  414,  414,  414,  414,  414,  414,
  414,  415,  416,  418,  417,  417,  420,  421,  421,  411,
  411,  409,  410,  410,  422,  422,  422,  423,  423,  424,
  424,  427,  427,  425,  425,  430,  430,  429,  426,  428,
  419,  419,  419,  419,  419,  419,  419,  419,  333,  333,
  317,  317,  149,   49,   65,  124,  142,  386,  159,  183,
  259,   67,   63,  217,  116,   70,  337,  213,   36,   42,
  346,  258,   40,   43,  210,   41,  286,  253,  253,  431,
  431,  431,  431,  431,  431,  431,  431,  431,  431,  431,
  431,  431,  431,  431,  431,  431,  431,
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
    0,    1,    3,    1,    3,    3,    0,    2,    2,    1,
    1,    3,    3,    2,    0,    2,    2,    1,    3,    9,
    1,    1,    2,    3,    7,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    4,    2,
    2,    1,    0,    1,    1,    6,    6,    1,    1,    0,
    0,    1,    0,    4,    0,    1,    1,    2,    8,    1,
    0,    3,    0,    0,    1,    1,    2,    1,    1,    1,
    2,    6,    1,    1,    3,    0,    1,    1,    0,    1,
    1,    2,    3,    4,    2,    2,    3,    0,    1,    1,
    2,    1,    4,    3,    3,    1,    0,    1,    1,    2,
    3,    4,    0,    1,    2,    0,    1,    1,    5,    5,
    3,    2,    0,    1,    3,    1,    1,    6,    3,    2,
    6,    2,    1,    2,    2,    2,    0,    1,    1,    2,
    0,    2,    0,    1,    1,    1,    1,    1,    1,    0,
    1,    1,    2,    2,    2,    5,    1,    2,    3,    2,
    2,    0,    1,    1,    1,    1,    3,    4,    8,    4,
    0,    6,    1,    1,    2,    2,    0,    3,    3,    1,
    2,    2,    2,    8,    8,    8,    8,    3,    0,    4,
    0,    1,    3,    4,    1,    3,    2,    3,    3,    1,
    1,    0,    1,    2,    4,    1,    1,    3,    1,    0,
    2,    1,    6,    1,    1,    3,    1,    2,    0,    1,
    2,    2,    2,    1,    1,    1,    1,    1,    1,    2,
    1,    1,    2,    1,    1,    1,    2,    2,    1,    2,
    3,    4,    2,    6,    4,    8,    9,    6,    8,    6,
    4,    0,    4,    2,    1,    3,    0,    2,    4,    0,
    1,    1,    3,    3,    7,    2,    0,    2,    2,    3,
    3,    0,    1,    2,    4,    1,    3,    2,    2,    2,
    1,    1,    1,    1,    2,    2,    5,    0,    4,    7,
    3,    0,    1,    3,    3,    7,    7,    0,    0,    0,
    0,    3,    0,    9,    9,    1,    1,    0,    2,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    5,    5,    5,    5,    5,    5,    2,    0,    4,    2,
    3,    3,    2,    2,    3,    3,    2,    3,    6,    1,
    2,    4,    3,    2,    2,    2,    2,    3,    1,    3,
    3,    3,    2,    2,    4,    4,    5,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    0,    2,    2,    0,    1,    2,
    2,    4,    3,    2,    2,    7,    1,    0,    1,    0,
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
    2,    3,    0,    1,    3,    4,    3,    3,    0,    2,
    0,    2,    0,    3,    3,    3,    1,    4,    4,    1,
    9,    0,    0,    0,    0,    3,    2,    1,    0,    3,
    5,    3,    2,    5,    2,    1,    1,    1,    1,    1,
    3,    1,    3,    1,    0,    2,    1,    3,    1,    1,
    1,    1,    3,    6,    1,    0,    3,    0,    1,    3,
    1,    1,    1,    4,    5,    5,    1,    1,    2,    4,
    4,    2,    3,    0,    1,    2,    1,    1,    2,    2,
    2,    0,    1,    0,    3,    0,    1,    3,    1,    2,
    0,    2,    0,    2,    0,    4,    1,    2,    1,    1,
    0,    1,    3,    2,    1,    1,    2,    1,    4,    3,
    1,    3,    4,    0,    3,    0,    1,    3,    3,    1,
    1,    0,    1,    1,    4,    0,    2,    4,    0,    8,
    5,    1,    1,    4,    4,    4,    1,    1,    5,    5,
    1,    3,    3,    7,    7,    5,    3,    0,    1,    1,
    1,    2,    0,    1,    1,    1,    0,    3,    1,    3,
    1,    0,    3,    1,    3,    1,    3,    3,    1,    1,
    3,    1,    2,    1,    3,    3,    1,    2,    3,    3,
    3,    3,    1,    3,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    3,    3,    3,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    1,    1,    5,    6,    5,    6,    3,    4,    5,    6,
    3,    4,    3,    4,    3,    4,    4,    5,    4,    4,
    3,    4,    1,    1,    1,    1,    1,    1,    1,    3,
    3,    1,    1,    1,    1,    1,    1,    1,    1,    1,
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
    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    0,  555,  556,  557,  561,  562,  563,    0,  627,  630,
  742,  743,  747,  748,  468,    0,    0,    0,    0,    0,
    0,  400,  573,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  122,    0,    0,  103,    0,  920,    0,
   22,  918,    0,    0,   74,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  481,    0,
    0,    0,   38,   35,   37,    0,   36,    0, 1008, 1010,
 1012, 1011, 1017, 1013, 1018, 1014, 1016, 1019, 1015, 1020,
 1021, 1022, 1024, 1025, 1023, 1026, 1027,    0,    0,    0,
    0,   33,   69,   67, 1000, 1009,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  638,
    0,    0,    0,  685, 1002,    0,    0,    0,    0,    0,
  115, 1007,  564,    0,    2,    0,    0,    0,    0,    0,
    0,  430,  431,  432,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  456,  450,  451,  452,  453,  454,  455,  457,    0,
  428,  429,    0,  992,    0,  407,  996,    0,  404,  999,
    0,  687,    0,  403, 1003,    0,  571,  916,    0,  158,
  111,    0,  112,    0,  208,  988,  779,    0,  110,  153,
  995,    0,    0,  106,    0,  113,  922,    0,  107,    0,
  108,    0,  109, 1006,  154,  114,  730,  731,    0,  921,
  919,    0,   75,    0,  993,    0,  991,  489,  482,    0,
  488,  493,  483,  484,  491,  485,  486,  487,  492,    0,
    0,    0,   31,    0,   40,  766,   39,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  926,    0,  907,  632,  632,  633,  643,  645,
  979,  646,    0,  647,  637,  633,    0,    0,    0,    0,
    0,    0,    0,  590,  579,    0,  582,  584,  585,  586,
  587,  593,  594, 1001,  686,    0,    0,  116,    0,  117,
    0,  118,    0,    0,    0,  617,    0,    0,  981,    0,
    0,  624,    0,    0,    0,    0,    0,  469,    0,    0,
  370,    0,  211,    0,  410,  401,  466,  467,    0,    0,
    0,    0,    0,    0,  419,    0,  376,  377,    0,  577,
  575,  917,    0,    0,    0,    0,    0,    0,    0,  161,
  140,    0,  104,    0,    0,  119,    0,  129,  128,    0,
    0,    0,    0,    0,    0,  984,   76,    0,    0,    0,
  744,  745,  490,    0,    0,    0,    0,  884,  913,    0,
  983,    0,  886,    0,    0,  914,    0,  908,  899,  898,
  897,    0,    0,    0,    0,  889,  890,  891,  900,  901,
  909,  910,    0,  911,  912,    0,    0,    0,  915,    0,
    0,    0,    0,    0,    0,  903,    0,  776,  881,  882,
  883,  888,  887,  359,  867,    0,    0,  866,    0,    0,
  863,  864,  865,  868,  869,  870,  902,  927,  928,  929,
  930,  943,  945,  953,  954,  958,  959,  209,    0,    0,
    0,  363,    0,    0,    0,  769,    0,    0,   28,    0,
    0,    0,   72,   70, 1004,   68,   34,    0,    0,    0,
  771,    0,   53,    0,    0,    0,    0,    0,  569,    0,
    0,    0,  640,  642,    0,  649,    0,    0,    0,  650,
  636,    0,  616,    0,  591,    0,  588,    0,  589,  614,
    0,  609,    0,    0,  583,  780,  751,    0,    0,    0,
    0,    0,    0,  566,  565,    0,  618,  622,    0,    0,
  727,  629,  628,    0,  176,    0,    0,  137,    0,  474,
    0,  475,  470,    0,    0,  414,  416,  415,    0,  417,
    0,  411,    0,    0,  262,    0,    0,    0,  423,  424,
    0,  459,    0,    0,    0,    0,    0,    0,    0,  381,
  380,  477,    0,    0,  157,    0,    0,  166,    0,  165,
    0,    0,  172,  155,    0,  170,  162,    0,    0,  512,
  547,  515,    0,  548,    0,    0,  516,    0,    0,    0,
  539,  551,  511,  534,  513,  514,  509,  496,  370,  494,
  495,  497,    0,    0,  506,  507,  508,  510,    0,    0,
    0,    0,    0,  120,    0,  121,    0,  132,    0,    0,
  192,    0,    0,  368,    0,  211,    0,    0,   84,    0,
  102,    0,  101,  985,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  782,  784,  793,  796,  797,  798,
  799,  800,  801,  802,  803,  804,  805,  806,    0,    0,
    0,  904,    0,    0,    0,    0,    0,    0,  905,  906,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  324, 1005,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  267,  504,    0,    0,
    0,  361,    0,    0,    0,  768,   29,    0,    0,   45,
   73,    0,    0,   27,    0,  774,    0,    0,  741,    0,
    0,    0,    0,    0,  568,    0,  632,    0,  654,    0,
    0,    0,    0,  559,    0,  924,  558,    0,  592,    0,
  600,  596,  595,    0,    0,    0,  611,  599,    0,    0,
  749,  750,    0,    0,    0,  368,    0,    0,    0,    0,
    0,  138,    0,  473,  370,  413,    0,  370,  371,  418,
    0,    0,  258,  368,  231,  989,    0,    0,  370,    0,
    0,  200,  368,    0,    0,    0,    0,  236,  230,  233,
  234,  235,  422,  464,  463,  421,  420,    0,    0,    0,
    0,    0,    0,  379,    0,  370,    0,  163,  167,    0,
    0,  171,    0,    0,  761,    0,  535,  536,  552,    0,
  549,    0,    0,  542,  540,  541,    0,    0,    0,    0,
  505,    0,    0,    0,    0,  543,    0,    0,  368,    0,
  191,    0,  194,  196,  197,    0,    0,    0,    0,    0,
  211,   79,    0,    0,   78,    0,    0,    0,    0,  783,
  795,    0,    0,  856,  857,  858,    0,  787,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  971,  972,  973,  974,  975,  976,  977,  978,    0,
    0,    0,    0,    0,    0,  964,    0,    0,    0,    0,
  879,  326,    0,  880,  951,    0,  875,  987,    0,    0,
    0,    0,    0,    0,    0,  885,  778,  777,  365,  364,
    0,  265,    0,    0,    0,  273,  770,    0,    0,    0,
    0,    0,    0,    0,    0,   54,   62,   64,    0,    0,
   71,    0,    0,    0,  773,    0,    0,    0,    0,    0,
    0,    0,  641,  644,  997,  653,    0,    0,    0,    0,
  659,  660,  661,  662,  671,  672,  673,    0,    0,  651,
  560,  925,  601,  610,  605,  604,    0,  615,  781,  752,
    0,  368,    0,    0,  634,  729,  728,    0,  472,    0,
    0,  175,    0,  179,    0,    0,    0,  412,  368,  368,
  270,  371,  263,    0,  202,  370,  204,    0,  371,  199,
  205,    0,    0,    0,    0,  425,  426,  370,  368,  371,
    0,    0,    0,  397,    0,  187,  188,  164,    0,  174,
  523,  524,  923,    0,    0,    0,    0,    0,  550,    0,
    0,  370,  371,    0,  500,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  189,    0,
    0,    0,   83,    0,    0,    0,    0,   91,   85,    0,
    0,   90,   86,    0,  746,  990,    0,    0,  788,    0,
    0,  794,    0,    0,    0,    0,    0,  832,  831,    0,
    0,    0,  841,  859,    0,  851,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  786,    0,    0,    0,
  211,    0,  931,    0,    0,  946,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  960,    0,
    0,    0,    0,    0,    0,    0,  626,  950,  892,    0,
  260,  279,    0,    0,  360,  274,   66,   58,   59,   65,
   60,   61,    0,    0,   23,    0,    0,    0,   43,    0,
    0,    0,  755,  775,    0,  754,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  688,    0,    0,
    0,    0,    0,    0,    0,  607,  608,  612,    0,  190,
    0,    0,    0,    0,    0,  180,    0,    0,    0,  408,
  214,  215,  216,  217,  218,  219,  895,    0,    0,  264,
    0,    0,  212,    0,    0,    0,    0,  237,    0,  460,
  461,  427,    0,    0,    0,    0,    0,    0,  370,  479,
  371,  173,  986,    0,    0,  518,    0,  517,  553,    0,
  545,    0,    0,    0,  498,    0,  526,    0,    0,  528,
  531,  533,  368,    0,    0,    0,  127,    0,  195,  370,
    0,  369,  105,    0,  125,    0,   94,    0,    0,    0,
    0,   88,   89,    0,  849,  850,  790,    0,  792,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  852,    0,
    0,    0,    0,    0,    0,    0,  842,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  937,  936,    0,    0,  933,  932,    0,  941,  940,
  939,  938,  935,  934,    0,    0,  966,    0,    0,    0,
  965,    0,    0,    0,    0,  266,    0,    0,  276,  277,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  358,    0,    0,    0,    0,  285,  290,
  284,    0,    0,    0,  294,  295,  296,  297,  298,  299,
  301,  302,  304,  305,  306,  309,    0,    0,    0,  352,
  353,  354,    0,    0,   57,   63,   55,   56,   24,  760,
  759,    0,  757,  765,  764,  762,  740,    0,    0,    0,
  663,    0,    0,    0,  693,  691,  689,    0,  690,    0,
    0,    0,    0,    0,    0,  369,  733,  734,  635,    0,
  185,  181,    0,  371,  896,  268,  269,  371,    0,    0,
    0,  222,    0,    0,  238,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  476,  520,    0,  521,    0,  142,
    0,    0,    0,  146,  148,  149,  150,  501,  503,  499,
  530,    0,    0,  371,    0,    0,    0,  366,  370,   92,
    0,    0,   98,   93,    0,    0,    0,    0,    0,  860,
    0,  861,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  210,  952,  942,  944,    0,    0,  962,  961,
    0,  967,  955,  957,    0,    0,  275,    0,  313,    0,
    0,    0,  308,    0,    0,  307,  338,  339,    0,  998,
  355,  356,    0,  293,  300,    0,  303,  286,    0,  291,
    0,  343,  292,  310,    0,    0,  336,   41,    0,    0,
  683,  681,    0,  674,    0,    0,    0,  697,    0,    0,
  631,  371,  367,  735,  182,  371,  152,  257,    0,    0,
  198,  223,    0,    0,    0,    0,  224,  227,    0,    0,
  371,  395,  396,  392,  391,  394,  393,    0,  519,  546,
  151,  139,  147,    0,  370,  254,  134,  371,  372,    0,
    0,   96,    0,   80,    0,  818,  826,  625,    0,  819,
  827,  822,  830,    0,    0,  820,  828,  821,  829,  815,
  823,  816,  824,  817,  825,    0,  947,  963,    0,    0,
  282,  278,    0,  311,    0,    0,    0,    0,    0,  340,
    0,    0,  331,  332,    0,  351,    0,    0,    0,    0,
  346,  344,    0,    0,    0,  756,    0,  665,  675,    0,
    0,  676,  677,  678,    0,  707,  709,    0,  704,    0,
  255,  409,  201,  206,  225,    0,    0,  228,    0,  240,
  242,  399,  371,    0,  374,    0,   77,   99,  100,  956,
  281,    0,  312,    0,    0,  315,    0,  321,    0,  323,
    0,    0,  350,  349,  994,  348,    0,    0,    0,    0,
    0,    0,  664,    0,    0,  698,  708,    0,  229,    0,
    0,    0,    0,    0,  256,  371,    0,    0,    0,    0,
    0,    0,    0,  334,  333,  345,  347,    0,    0,    0,
  669,    0,    0,    0,  715,    0,    0,    0,    0,    0,
    0,  239,    0,  246,    0,  245,  375,  126,  283,    0,
    0,  314,  318,    0,    0,    0,    0,    0,    0,  667,
  680,  717,  706,    0,    0,  718,    0,  714,  226,    0,
    0,  250,  249,  248,    0,    0,    0,    0,    0,  335,
  670,  713,    0,    0,  253,  251,  252,  329,  319,    0,
  341,  316,    0,    0,    0,  720,  317,    0,  719,    0,
  722,    0,  723,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1425,   28,   29, 1426,
   31,   32,   33,   34,   35,   36,   37, 1427,   39, 1428,
  845,  128,  488,  984, 1225,  129,  739,  985,  130,  492,
  744,  489,  131,  132,  275, 1008,  144,  986,  987,  213,
  242,  133,  988,  134,  494,   85,  253,  397,  445,  657,
 1116,  446,  884,  628,  235,  861,  658,  659,  660, 1123,
 1118, 1544,  254,  662,  663,   77,  203,   78,  249, 1873,
 1333,  234,  239,  241,  243,  229,  221,  223,  236,  246,
  161,  328,  330,  332,  386,  644,  646,  485,  276,  808,
  565,  673, 1048,  390,  649, 1107, 1327,  596,  766, 1078,
  557,  558,  578,  382,  629, 1093, 1522, 1641, 1270, 1523,
 1524, 1525, 1526, 1527,  674,  230,  222,  378,  604,  379,
  380,  605,  606, 1302,  559, 1042, 1043, 1044, 1265,  392,
  872,  873,  874,  875,  810, 1059, 1500,  630,  812,  448,
  877,  957,  479, 1385, 1271,  449,  450,  451,  452,  453,
 1501, 1502,  581, 1647, 1792, 1288,  818,  819,  805,  820,
  821,  822, 1793, 1794, 1853,  360,  484,  974, 1429,  209,
  574,  971,  575, 1052,  480,  975,  976, 1213, 1408, 1409,
 1410, 1702, 1125, 1430, 1431, 1432, 1433, 1611, 1434, 1435,
 1436, 1437, 1438, 1439, 1440, 1441, 1442, 1443, 1444, 1445,
 1446, 1447,  454, 1800, 1448, 1712, 1837, 1709,  455,  715,
  456,  457, 1449, 1612, 1720, 1721, 1774, 1450, 1451, 1452,
 1603,  272,  481,  482,  458,   41, 1332, 1538,  369,  592,
  836, 1300,  593,  834,   82,   62,  364,  214,  171,  347,
  200,  354,  206,  799,  796,  355,  365,  826,  366,  588,
 1070,  201,  459,  348,  594, 1301,   99,  212,  258,  631,
  632,  728, 1094,  729, 1095, 1096,  635,  636,  637,  638,
 1461, 1087, 1085,  639,  640,  641,  854,  642,  851,   42,
   43,   44,   45,   46,   47,  163,  545,   64,  510,  217,
  371,  315,  316,  317,  318,  319,  320,  321,  322,  323,
  772,  531, 1027, 1258,  532,  533,  777,  167,  336,  547,
   48,  170,  342, 1356,  714,   49,  340,   50,  148,  303,
  518,  762,  401, 1255, 1484, 1631,  513,  305, 1262, 1634,
  149,  150,  304,  519,  520,  760, 1006, 1009, 1010, 1011,
 1012, 1013, 1014, 1727, 1783,  460, 1812, 1015, 1016, 1017,
 1253, 1732, 1733, 1734, 1245,  157, 1476, 1627, 1628, 1815,
 1740, 1816, 1817, 1818, 1848, 1874, 1876,  550,  551, 1036,
 1489,  500,   51,   52,  402,   53,   54,  539,  540,  993,
 1233, 1462, 1466,  501,  747,  227,  675,  676,  899,  900,
  677,  678,  679,  680,  681,  682,  683,  684,  685,  686,
  687,  688, 1141, 1143, 1144,  461,  462,  463,  464,  465,
  466,  467,  251,  468,  469,  470,  471,  472,  940,  473,
 1578,  474,  475,  476,  477,  704,  944, 1202, 1197,  705,
  136,
};
static short yysindex[] = {                                   5212,
 2416, -161, 3782,   74, 1292,  -20, 2707,  277, 8849,  483,
   75, 6961, -161, 1296,  -48, 5469, 1025, 5469, -125,    0,
  323,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  366,    0,    0,    0,    0,    0,    0,  -96,    0,    0,
    0,    0,    0,    0,    0,14224, 5469, 5469, 5469, 5469,
 5469,    0,    0,  291, 1236, 5469, 5469, 5469,  440, 5469,
  430, 5469, 5469,    0, 5469, 5469,    0, 1022,    0,  451,
    0,    0,  399, 5469,    0, 5469, 5469, 5469,  586, 5469,
 5469, 5469, 5469,  430, 5469, 5469, 5469, 5469,    0, 5469,
  890,  656,    0,    0,    0, 1052,    0, 1052,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  738,  819,   62,
 1172,    0,    0,    0,    0,    0, 5469, 5469, 5469,  955,
  927,  933,  101, 1479,  944,   85,   99,  829,  916,    0,
 5469, 1082, 1409,    0,    0, 5469, 1068, 5469, 5469, 5469,
    0,    0,    0, 5469,    0, 1088,  976, 1299, 1192, 1056,
 1305,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  977,
    0,    0, 1236,    0,  616,    0,    0, 1540,    0,    0,
 1073,    0,  691,    0,    0, 1179,    0,    0, 1135,    0,
    0,  527,    0, 1359,    0,    0,    0, 1236,    0,    0,
    0,  678, 1540,    0,   39,    0,    0, 1316,    0, 1361,
    0, 1052,    0,    0,    0,    0,    0,    0, 1363,    0,
    0, 1229,    0, 1373,    0, 1274,    0,    0,    0, 1229,
    0,    0,    0,    0,    0,    0,    0,    0,    0,13254,
 5469, 1298,    0, 5469,    0,    0,    0, 4372, 1330, 5422,
 5469, 1273, 1406, 1695, 1431, 1439, 4372, 1366, 5422, 1391,
 1395, 1277,    0,13361,    0,    0,    0,    0,    0,    0,
    0,    0,11609,    0,    0,    0, 1370, 5469, 1452, 1360,
  613, 5469, 1384,    0,    0, 1409,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 5469, 1540,    0, 1316,    0,
 1052,    0, 1437, 1447, 1331,    0,13361, 1424,    0, 1424,
13361,    0,  -85, 1364, 1374, 1376, 1305,    0, 1601, 1112,
    0, 5469,    0,  977,    0,    0,    0,    0, 5469, 1298,
 3391, 1430, 4073, 1843,    0,14224,    0,    0,  454,    0,
    0,    0, 1834, 1834, 1236, 1531, 1236,  -84,  527,    0,
    0, 9795,    0, 5469, 5469,    0, 1298,    0,    0, 1617,
  -74, 1869, 5469, 1648, 5469,    0,    0, 1878, 3472, 8719,
    0,    0,    0, 1890, 1891, 1895, 1462,    0,    0, 1898,
    0, 1899,    0, 1903, 1906,    0, 1907,    0,    0,    0,
    0,    0, 1908, 1496, 1497,    0,    0,    0,    0,    0,
    0,    0, 1913,    0,    0,11867, 1916, 1917,    0, 1920,
13361,13361,12086, 5469, 1921,    0, 2939,    0,    0,    0,
    0,    0,    0,    0,    0, 1924, 1873,    0,    0, 1919,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 9795, 1911,
 1007,    0, 1933, 1712, 1532,    0, 5469, 1549,    0, 5469,
 5469,  150,    0,    0,    0,    0,    0, 5469, 5469,  559,
    0, 5469,    0, 1661, 5469, 1935, 4372, 1590,    0, 5469,
12086, 1234,    0,    0,13361,    0, 2307, 1669, 1947,    0,
    0,  163,    0,  715,    0,  -46,    0,  -46,    0,    0,
 1952,    0,  116, 1577,    0,    0,    0, 1953,   88, 1939,
 1298, 1962, 1744,    0,    0, 5469,    0,    0, 1754, 1967,
    0,    0,    0, 1149,    0, 1673, 1674,    0, 1236,    0,
 1682,    0,    0, 1737, 1738,    0,    0,    0, 1718,    0,
 9795,    0, 9795, 1598,    0, 1764, 5469,14058,    0,    0,
  683,    0, 5469,  316, 1073, 1639, 1602, 1572,    0,    0,
    0,    0,  -32, 1647,    0, 1588,  430,    0, 1236,    0,
 1765, 1706,    0,    0,  -84,    0,    0,  202, 1600,    0,
    0,    0, 1613,    0, 1663, 2017,    0, 1713, 2024,   26,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1974,  138,    0,    0,    0,    0,  192, 2029,
 2034, 2024, 1540,    0, 1760,    0, 1813,    0, 1236, 1236,
    0, 3391,  691,    0, 1703,    0, 1677, 2037,    0, 9192,
    0, 1702,    0,    0, 1710, 2046, 8957, 2047,    0,    0,
    0, 8475, 5993,  543,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1197,13361,
  396,    0, 5469,13361, 1246, 1254, 1257, 2532,    0,    0,
13361, 9196, 2939, 1646,  498,13361,13361, 9196,  170,  170,
 1801, 1619,  -96, 2052,    0,    0,12193, 5469,13361,13361,
13361,13361,13361,13361,13361, 2872,    0,    0, 1823, 2028,
 5469,    0, 5469, 1804, 5469,    0,    0, 8611, 1675,    0,
    0, 1652, 5422,    0, 1788,    0, 1625, 2064,    0, 1788,
 8611, 1792, 1807, 5469,    0, 1801,    0, 1980,    0, 5469,
 2791, 1684,13361,    0,  731,    0,    0,  306,    0, 1681,
    0,    0,    0, 5469,  988, 5469,    0,    0, 5469, 5469,
    0,    0,13361, 1849, 3391,    0, 2082, 1635,13361,13361,
 1236,    0,  677,    0,    0,    0, 2096,    0,    0,    0,
  330, 5469,    0,    0,    0,    0, 1866, 2101,    0, 1845,
 1864,    0,    0, 1806, 1808, 1052, 1874,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  430, 5469,14166,
 1775, 1776, 1781,    0,  430,    0, 1480,    0,    0, 1743,
 1236,    0, 3472,  125,    0, 1750,    0,    0,    0,  430,
    0, 2017,   90,    0,    0,    0, 1865,   90, 1755,   90,
    0, 2124,  430,  430,  430,    0, 1298, 5469,    0, 1814,
    0, 1650,    0,    0,    0,  454, 1424, 5469, 1670,   98,
    0,    0, 2127,   22,    0, 3472, 5469, 1424, 9434,    0,
    0, 1424, 8957,    0,    0,    0, 5704,    0,  205,  564,
13361,13361, 9541, 2131,  810, 9799,13361,10057,  597,10164,
10422,12412,10680,10787,11045, 9196, 9196,13361,13361, 2506,
13361, 2133,13361, 2140, 2025,13361,13361,13361,13361,13361,
13361,    0,    0,    0,    0,    0,    0,    0,    0, 1858,
 1478,  468,13361,  980,13361,    0, 9196, 2268, 2570,   77,
    0,    0, 1056,    0,    0, 1690,    0,    0, 1910,  170,
  170, 1070, 1070, 2939,  931,    0,    0,    0,    0,    0,
 1708,    0, 1751, 1915, 1804,    0,    0, 5469, 5469, 5469,
 5469, 5469, 5469,  166, 2144,    0,    0,    0, 8611, 1812,
    0,  418, 1835, 5469,    0,13361, 1835, 2154, 2144, 8611,
 4891, 1882,    0,    0,    0,    0, 2571, 2161, 2160, 1075,
    0,    0,    0,    0,    0,    0,    0, 9196, 1885,    0,
    0,    0,    0,    0,    0,    0,  452,    0,    0,    0,
 2939,    0, 1715, 1424,    0,    0,    0, 2939,    0, 1834,
 1941,    0,  677,    0, 2176, 9196, 5005,    0,    0,    0,
    0,    0,    0, 1804,    0,    0,    0, 5005,    0,    0,
    0, 1052, 1052, 1724, 2178,    0,    0,    0,    0,    0,
  285,  -31,  337,    0, 1957,    0,    0,    0, 5469,    0,
    0,    0,    0,   74, 1721, 1810, 1823, 2181,    0, 1588,
 1742,    0,    0, 1012,    0, 2170, 5469, 1141,  430, 2189,
 2191, 2192, 1973,  691, 1804, 1834, 1674, 3391,    0, 1647,
 1793, 1711,    0,  430, 1853, 1936, 9192,    0,    0,  430,
  169,    0,    0, 1937,    0,    0, 2204, 2205,    0, 9196,
 9196,    0, 9196, 9196, 2924, 2939, 2211,    0,    0, 2939,
 2213,12086,    0,    0, 1938,    0,   30, 2218, 2939, 2219,
 1727, 2221, 2939, 2227,13361,13361, 2131,13361,12670, 2231,
 2939, 2235, 2239, 2939, 2241,13361, 2939, 2244, 2939, 2249,
 2250, 2939, 2252, 2253, 2939, 2254,    0, 2014, 2054, 2136,
    0, 2207,    0, 2374,13361,    0, 2389, 2413, 2423, 2479,
 2489, 2496,13361,13361,13361, 2939, 1870,13361,    0,13361,
 2939, 1991,  549,13361,13361,13361,    0,    0,    0, 5469,
    0,    0, 5469, 9674,    0,    0,    0,    0,    0,    0,
    0,    0, 1979, 8611,    0, 8611,  166, 2144,    0,  218,
  218,   57,    0,    0, 1747,    0, 2154, 2144, 2260, 4891,
 2257, 1075, 2273,13361, 2053, 2791, 1940,    0, 1940,  907,
 1940, 1981, 1985, 2056, 2000,    0,    0,    0, 1804,    0,
 1793,  640, 1588, 1949, 1588,    0, 9196,  404, 1201,    0,
    0,    0,    0,    0,    0,    0,    0, 5005, 5005,    0,
 1915,13361,    0, 2051, 1942, 1724, 1022,    0, 9196,    0,
    0,    0, 1955, 1960, 1963, 1964, 1965, 1966,    0,    0,
    0,    0,    0, 2306,   74,    0, 1901,    0,    0,   90,
    0, 5005, 1989,   90,    0,   90,    0, 1823, 2312,    0,
    0,    0,    0,  454, 1915, 1588,    0, 1674,    0,    0,
 2081,    0,    0, 2082,    0, 2325,    0, 2335, 1236,  -38,
 2337,    0,    0, 1236,    0,    0,    0, 2014,    0, 2014,
13361, 1424, 1424, 1759,  -96, 2338,13361, 2049,    0, 1424,
 1424,13361, 1424, 1424, 2948, 2939,    0, 1774,13361, 2939,
 1424, 1424, 1424, 1424, 2939, 1424, 1424, 1424, 1424, 1424,
 1424,    0,    0, 9795, 2342,    0,    0, 2642,    0,    0,
    0,    0,    0,    0, 2779, 1655,    0,13361, 2079, 1958,
    0,13361, 2814, 1779, 3080,    0, 9795, 2327,    0,    0,
 2097,    0, 3179,  315, 2332, 2352, 2057,13361, 2336, 2341,
 3779, 5469, 5469,    0, 2346, 2348, 2058, 2353,    0,    0,
    0, 2091, 8367, 2355,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2361,   43, 2365,    0,
    0,    0,    0, 2033,    0,    0,    0,    0,    0,    0,
    0, 1997,    0,    0,    0,    0,    0, 4891, 2260, 2053,
    0, 1783, 5469, 1075,    0,    0,    0, 2087,    0, 2791,
 2791,13361, 9196, 2038, 1915,    0,    0,    0,    0, 1480,
    0,    0,  413,    0,    0,    0,    0,    0, 2939, 2158,
 2051,    0,  -87, 5469,    0, 2104,  433, 2005, 2106, 2007,
 2145, 2108, 2151, 1804,    0,    0, 2409,    0, 2410,    0,
 2109, 2158, 1989,    0,    0,    0,    0,    0,    0,    0,
    0, 1804, 1647,    0, 1480, 1957, 2074,    0,    0,    0,
 2094,  -95,    0,    0, 1823, 2098, 2939, 2420, 2421,    0,
 1056,    0, 2939,13361, 2426, 2428, 2939, 2430, 2431,13361,
13361, 2939, 2435, 2436, 2437, 2438, 2439, 2440, 2446, 2447,
 2449, 2450,    0,    0,    0,    0,13361, 2456,    0,    0,
13361,    0,    0,    0,13361,  379,    0, 2184,    0,12777,
 5469,13361,    0, 9196, 2939,    0,    0,    0, 2441,    0,
    0,    0, 5469,    0,    0, 3091,    0,    0,  -22,    0,
 2063,    0,    0,    0,  350, 2467,    0,    0,  218, 5469,
    0,    0, 2791,    0, 2070, 2939, 2464,    0,  543,  642,
    0,    0,    0,    0,    0,    0,    0,    0, 2470, 5469,
    0,    0, 2171, 2183, 5469, 1724,    0,    0, 1052, 5469,
    0,    0,    0,    0,    0,    0,    0, 1915,    0,    0,
    0,    0,    0, 1915,    0,    0,    0,    0,    0,13361,
 1236,    0, 2067,    0, 1236,    0,    0,    0, 2939,    0,
    0,    0,    0, 2939, 2939,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2939,    0,    0, 2906, 5005,
    0,    0, 2487,    0, 1861,13254, 1554,  626, 2469,    0,
 2193, 1426,    0,    0, 2494,    0, 5469, 5469,   74,  122,
    0,    0, 2035, 2206, 9196,    0, 2512,    0,    0, 9196,
 2517,    0,    0,    0,13361,    0,    0, 2196,    0, 2519,
    0,    0,    0,    0,    0, 1724, 1052,    0, 2186,    0,
    0,    0,    0, 1957,    0, 2939,    0,    0,    0,    0,
    0, 1424,    0, 2162, 3091,    0, 2119,    0, 3091,    0,
 3091, 3050,    0,    0,    0,    0, 9674,  -22,13361, 3091,
  796, 5469,    0,  543, 5469,    0,    0, 5279,    0, 2186,
  -15, 1724, 2194, 2210,    0,    0, 2526, 2531, 2750, 2522,
 1511, 9674, 2538,    0,    0,    0,    0, 1199,  252, 2287,
    0, 1796, 1816, 5469,    0, 2550, 2554,  530, 1724,  740,
  740,    0, 2301,    0, 2166,    0,    0,    0,    0, 3091,
 2538,    0,    0, 2297, 3091, 2313, 2303, 9674, 5469,    0,
    0,    0,    0, 5279, 2567,    0, 5469,    0,    0, 2155,
  864,    0,    0,    0, 1822, 9674,  147, 5469, 9674,    0,
    0,    0, 5469, 2283,    0,    0,    0,    0,    0, 9674,
    0,    0, 2575, 2579, 2581,    0,    0, 5469,    0, 5469,
    0, 2582,    0,
};
static short yyrindex[] = {                                      0,
    0,  216,  877,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  127,11283,    0,    0,    0,    0,    0,    0,
 2625,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  178,    0,    0,    0,    0,    0,    0,  368,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  838,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2294,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  168, 2255,    0,    0,    0,  152,    0,  152,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2256,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  929,    0,    0,12996,11390,    0,
    0,    0, 1097,    0,    0,  123,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  862,    0, 2224,  363,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  429,    0,    0,
    0,    0,  480,    0,    0, 1143,    0,    0,    0,    0,
    0,  133,    0,10418,    0,    0,    0,    0,    0,    0,
    0,    0,  429,    0,  -39,    0,    0, 2588,    0,    0,
    0, 2372,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1207,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1223,
    0, 2373,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1001,    0,    0,    0,    0,    0,    0,
    0, 6047,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  748,    0,    0, 1230,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  429,    0, 2588,    0,
 2372,    0, 1232,  385,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1314,    0, 2363,    0,
    0,    0,    0, 1319,    0,    0,    0,    0,    0, 2373,
  730,14290,    0, 1327,    0,    0,    0,    0,  318,    0,
    0,    0, 2190, 2190,    0,    0,    0, 1339,  140,    0,
    0,    0,    0,    0,    0,    0, 2373,    0,    0, 1428,
    0,    0,    0,    0,    0,    0,    0, 5870,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2001,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2835,    0,    0,    0, 3135, 3435,    0, 3735,
    0,    0,    0,    0,    0,    0,  300,    0,    0,    0,
    0,    0,    0,    0,    0,  179, 4035,    0, 2533,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1342,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1348,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 3350,    0, 2326,    0,
    0,    0,    0,    0,    0,  757,    0,  757,    0,    0,
 1019,    0, 1162, 1264,    0,    0,    0,    0, 1207,    0,
 2373,    0,    0,    0,    0,    0,    0,    0,  503,  591,
    0,    0,    0,  621,    0,    0, 1062,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2363,    0,    0, 1219,    0, 2180,    0,    0,  909,    0,
    0,    0,    0,  292,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1358,    0,    0, 3726, 4818,    0,
    0,    0,10950,    0,    0,13880,    0,    0,13916,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1469, 1875,    0,    0,    0,    0,13959,13827,
    0,13916,  429,    0,    0,    0,    0,    0,    0,    0,
    0,  730, 1185,    0,    0,    0,    0, 2242,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1211, 6341,
 6590,    0,    0,  453,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,13580,    0,
13580,    0,    0,    0,13580,13580,13580,    0,    0,    0,
    0,    0, 2201,    0,    0,    0,    0,    0, 5224, 5513,
 2602,    0,   29,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 4752,    0,
    0,    0,    0, 2377,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  136,    0,    0,    0,    0,  136,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  620,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  868,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  730,    0,  410,  113,    0,    0,
    0,    0,  874,    0,    0,    0,    0,    0,    0,    0,
 1829,    0,    0,    0,    0,    0, 2609,    0,    0, 1508,
 1446,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  387,  694,  830,    0,    0,    0,  705,    0,    0,    0,
    0,    0,    0,    0,    0, 4026,    0,    0,    0,    0,
    0,13880,    0,    0,    0,    0,  806,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2373,    0,    0,  257,
    0,    0,    0,    0,    0,    0,    0,    0,    0,10053,
    0,    0,    0,  182,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2611,    0,    0,    0,    0,    0, 4335, 5802,
 6091, 4635, 4935, 1031,    0,    0,    0,    0,    0,    0,
    0,    0, 8216,    0, 2383,    0,    0,    0,    0,    0,
    0,    0,    0, 1369, 1379,    0,    0,    0,    0,    0,
    0,    0, 1381,    0,    0,    0, 1381, 1388, 1394,    0,
    0,    0,    0,    0,    0,    0,    0, 6685,  268,  184,
    0,    0,    0,    0,    0,    0,    0,    0,  472,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  159,    0,    0,    0,    0,    0,    0,  924,    0, 2190,
 2208,    0,  883,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2377,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1300,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 4326,    0,    0,    0,
    0,    0,    0,    0,    0, 1225,    0,    0,    0,    0,
    0,    0,    0, 1185, 2377, 2190, 1403,  730,    0, 2391,
 1408,    0,    0,    0,10053,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1294,    0,    0,    0, 1627,
    0,    0,    0,    0,    0,    0,    0,    0, 2323,    0,
 6438,    0, 6974,    0,    0,    0,    0,    0,    0,    0,
 7004,    0,    0, 7175,    0,    0, 7247,    0, 7307,    0,
    0, 7382,    0,    0, 7454,    0,    0, 1855,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2230,    0,    0,    0,    0,
  991,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  672,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1369, 1450,    0, 2232,
  189,    0,    0,    0,    0,    0, 1456, 1465, 1468,    0,
    0, 2324, 1043,    0, 6760,    0, 2328,    0, 2328, 2324,
 2328,    0,    0,    0,  623,    0,    0,    0, 2377,    0,
 1408,    0,    0, 2214,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2197,    0, 1300, 2294,    0,    0,    0,
    0,    0,  843,  858,  896,  908,  920,  925,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1079,    0,    0,    0,    0, 1460,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1475,    0,    0,
    0,    0,    0, 1484,    0,    0,    0,    0,    0,  157,
    0,    0,    0,    0,    0,    0,    0,  199,    0,  250,
    0,    0,    0,    0,   29,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 7526,    0, 7598,    0, 7658,
    0,    0,    0,    0, 7733,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2624,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10729,    0,    0,    0,    0,    0,    0,    0, 1528,
 1557,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  702,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1671,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1494, 2229,
    0,    0,    0,  190,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  813,    0,    0,    0,    0,    0,  587,
    0,    0,    0,    0,    0,    0,    0,    0, 2627, 1336,
 2236,    0, 2363,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2377,    0,    0,    0,    0,    0,    0,
    0, 1502, 1087,    0,    0,    0,    0,    0,    0,    0,
    0, 2377, 2391,    0,  294,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  270,    0, 7805,    0,    0,    0,
 2611,    0, 7877,    0,    0,    0, 7949,    0,    0,    0,
    0, 8009,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2613,    0,    0,    0,    0,
    0,    0,    0,    0, 2615,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2371,    0,    0,    0,    0,    0,    0,    0,  189, 1644,
    0,    0,    0,    0, 2324,  156,  674,    0,  794, 2630,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1892,    0,    0,  165,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  160,    0,    0,    0,    0,    0, 8084,    0,
    0,    0,    0, 8156, 8228,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2637,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   84,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  986,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2639,    0,    0,
    0,    0,    0,    0,    0, 1892,  875,    0,  161,    0,
    0,    0,    0,    0,    0, 2627,    0,    0,    0,    0,
    0,    0,    0, 2621,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   46,    0,    0,    0,
    0,    0,    0, 8291,    0,    0,    0, 2630,    0,  850,
    0, 1300,  952, 1165,    0,    0,    0,    0,    0,    0,
    0,   46, 2623,    0,    0,    0,    0,    0, 2387,    0,
    0,    0,    0, 1150,    0,    0, 2651,    0, 1892,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2634,    0,    0,13988,    0,    0,    0,   46,    0,    0,
    0,    0,    0, 2630,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   46,    0,    0,   46,    0,
    0,    0,    0, 1833,    0,    0,    0,    0,    0,   46,
    0,    0, 2653,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2696,    0,    0, 2697,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2698,
    1,  267,   82, -629, 1474,  704, -312, -330, 2692, 2417,
    0,    8,    0, 2427,  -98,  -47,    0, -273, -455,  -36,
  -68,  -43, -218, 2433, 1972,    0,    0,    0, 1045,    0,
    0,  -51, 1591, -556,  -67, -973, 2060, 1836, -315,    0,
 1604,    0, 2644, 1841, 1893,    0,  -63,    0, -513,  -57,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -535, -151, 1623,
 -336, 1204, -889,    0,    0,    0,    0, -334, -126, -322,
 1632, -350,  -58,    0, -548,    0,    0, 1218, -989,    0,
 1224,    0,    0, 2392, -612,  -49,    0,    0,    0,    0,
 2369,    0, 2146, 1656,    0,    0,    0, 1709,    0, 2425,
 1975, 1649, 2398, 2401,    0, 1177,    0, -447, 1961,  -44,
 -633, 1128, -249,    0,    0,-1002, -950, -946, -883, -873,
    0, 1275,-1026,    0,  984,-1186, 1272,    0, 2199,    0,
    0,    0,  983,  987,  958, -116, -253, -917, -933,    0,
    0,    0, 1982,    0, -659,    0, 1811,    0,    0,    0,
    0,    0,-1170, -510,    0,    0,    0,    0, -217,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1081,    0,    0,  175,    0,    0, -344,    0,
 -404,   87, 1371, 1180,    0, 1015,    0,    0,    0,    0,
    0,    0,    0, 2068, -130, -427, 1539, 1315, -550, -758,
 -995,-1297,    0,    0,  -40,    0,    0,    0,    0,    0,
 2443,    0,    0,    0,    0, 2448, 2220,    0,    0,    0,
    0,    0,   -9, 2459,    0,    0,    0,  -10,    0,    0,
    0, -364, 1943, -356, 1493, -488,    0,    0,    0,    0,
 2209,    0, -466,    0,    0,    0, 2172,    0, 1971,    0,
    0,    0,    0,    0,    0,  -23,    0, 2799,    0,    0,
    0,    0,    0, 2497,    0,    0,    0,    0, 2295,    0,
 2299,    0,    0,    0, 2055,    0,    0,    0,    0,    0,
 -338, -669, -888, 2050,    0, 1337,    0,    0,    0,    0,
    0,    0,  -35,    0,    0,    0, -199, 1233,    0,    0,
    0, 2676, 1293,    0, 2065, -991,-1451,    0, -891, 1826,
-1259,    0,    0,    0,    0, -143,    0,    0,    0,    0,
 1580,    0,    0,    0,    0, 2753,  416,    0, 1108, 1214,
    0, 1005,    0, 1039,    0, -168,    0,    0, 2076,    0,
    0,    0,    0,    0, 2329,    0,    0,    0, -400, 2095,
 1876,-1073,    0,    0,    0, -707, -417,  188,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2019, 1714,    0,    0,    0, -871,    0,    0,
    0, 1594,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -934, 1666,    0,
    0,
};
#define YYTABLESIZE 14807
static short yytable[] = {                                     135,
  205,   38,  135,  228,   81,  156,  155,  224,  162,  277,
  787,  208,  325,  220,  569,  297,  300,  633,  968,  296,
  299,  225,  233,  154,  259,  634,  265,  603,  268,  809,
  237,  727,  245,  767,  263,  788,  240,  571,  712,  597,
 1215,  262, 1411,  953, 1272,  264,  202,  204,  207,  210,
  155,  215,  270,  237,  269, 1272,  226,  204,  231,  267,
  210,  493,  155,  215, 1207,  244,  135,  211, 1283,  726,
  493, 1029,  749,  972,  255,  156,  155,  257,  204,  238,
  255,  231,  207,  210,  266,  155,  215,  244,  135,  942,
  210,  331,  283,  154,  285,  950, 1273,  514,  746, 1505,
 1274,  307,  876,  885,  713,  281,  576, 1273,  537,  573,
  327, 1274,  736, 1308, 1330, 1242,  387, 1110,   80,  295,
 1206,  998,  687,  800,  294,  801,  574,  226,  155,  226,
  811,  780,  159,  647, 1090,  758, 1281, 1115,  298,  160,
  333,  231,  327,  394,  281,  284,  324,  295,  210,  155,
  244,  356, 1034,  736,  162,  699,  736, 1463,  753,  776,
  247,  295, 1280, 1275,  767, 1778,  329, 1051, 1084, 1284,
 1054,  736,  713, 1276, 1275, 1277,  383,  619,  325, 1061,
 1292,  687,  538,  657, 1276,  574, 1277, 1325,  525,  658,
 1771,  159,  752,  743,  758,  767,  699,   95,  160,  699,
   97,  247,  753, 1313,  247,  767,  792,  765,  767, 1224,
  541,  722,  601,  633,  699,  574,  723,  753,  619,  247,
 1624,  634,   87,  767,  657,   87,  478,  657,  860,  486,
  658,  863,  543,  658, 1542, 1105,  619,  325, 1668,  789,
 1716,  844,  657,  130,  817, 1132,   80,  525,  658,  890,
  523, 1601, 1602, 1473,  603, 1672,  133, 1503,  555,  555,
 1397,  226,   80, 1399,  226,  831, 1295,  656,  155,  650,
  495,  135,  741,  151,  574, 1272, 1272,  155,  141,  495,
  295,  536, 1820,  168, 1643,   63, 1717,  784, 1496, 1497,
  791,  398,  857,  186, 1121, 1771,   86, 1718,  207,  893,
  855,  832,  155, 1177, 1178, 1644,  388,  570,  656, 1272,
  527, 1645,  956,  527,  478,  133,  225,  478,  584,  530,
  965,  586, 1520,  598,  770,  600,  656, 1273, 1273, 1358,
 1811, 1274, 1274,  713, 1203,  169,  643,  655,  916,  651,
  893, 1485,  226,  893, 1530, 1646,  139, 1498,  645,  226,
  398,  226,  186,  226, 1474,  538,  589, 1615,  893, 1227,
  337, 1273,  739, 1729, 1091, 1274,  856,  726,  504, 1122,
 1237, 1239,  164,  152,  210,  215,  478, 1359,  218,  219,
  153,  165, 1068,  155,  621,  207,  384, 1861, 1673,  664,
 1050, 1534,  893,  771, 1275, 1275, 1719,  389, 1259,  661,
  653,  833, 1296,  739, 1276, 1276, 1277, 1277,  726,  620,
  290, 1515,  859,  130,  130, 1278, 1279,  289, 1821, 1543,
  999,  739, 1777,  556,  602,  621,  726,  736, 1275,  159,
  775,  362, 1711, 1268,  716, 1291,  160,  922, 1276, 1700,
 1277, 1002,  740,  621, 1494,  384,  718, 1870,  736, 1111,
  620,  719,  702, 1636,  536,  917, 1796,  740, 1045, 1748,
 1127, 1047,   95, 1579, 1128,   97, 1130, 1582,  620,  837,
  699,  696, 1057, 1651, 1503, 1129,  159,  155, 1620,  378,
  210,  495,  699,  160, 1114,  327,  755, 1616,  155,  280,
  337,  699,  155,  702,  737,  210,  758,  155,  657, 1075,
  162,  486,  732,  781,  658,  745, 1083,  793,  657,  750,
  657,  702,  696,  619,  658, 1836,  658, 1347, 1348,  657,
 1349, 1350,  692,   79,  493,  658,  867,  767,  692,  838,
  696,  400,  687,  813,  293,  699,  226, 1665,  378,  293,
  767,  726,  527,  732,  699, 1726,  732,  839,  293,  525,
 1406, 1632,  573, 1324,  574,  398,  237, 1003,  736, 1789,
  885,  732, 1030,  657, 1117, 1533,  687,  806,  226,  658,
  362, 1674,  657,  806, 1083,  527, 1334,  789,  658, 1241,
  325,  478,  656, 1131,   87,  526,  186,  699,  753, 1625,
  725, 1235,  656, 1234,  656,  742, 1658,  870,  871,  969,
  133,  699,  753,  656, 1637, 1822, 1261,  225, 1638, 1639,
 1469, 1223,  293, 1103, 1664,  657,  574,  527, 1293,  703,
  737,  658,  701,  619,  843,  736, 1049,  657,  791,  657,
  862,  725, 1849,  658, 1297,  658,  538,  186, 1022,  764,
  843,  159,  226,  924, 1666,  186, 1698,  656,  160,  725,
  384,   79, 1342,  247, 1493,  301,  656,  767, 1228, 1723,
  703,  737, 1678,  701, 1064,  916, 1767,   79,  699, 1238,
 1298,  100,  527,  695,  916, 1058, 1507,  739,  703,  737,
  166,  701,  726,  231, 1343, 1551,  478, 1871,  478, 1532,
  977,  216,  259,  382,  916,  921,  657, 1272,  739,  656,
  232,  893,  658,  726,  186, 1263,  740,  478,  958, 1591,
 1761,  656,  590,  656,  695,  142,  226,  252, 1294, 1282,
  621,  226,  591,  226, 1752,  226, 1019,  527,  495,  916,
 1753, 1290,  695,  495, 1230,  536,  528, 1354,  378, 1039,
  225,  495, 1741,  378,  210,  620, 1742,  597,  378, 1273,
 1005,  210,  382, 1274,  137, 1312,  602,  478,   14, 1217,
 1066, 1751, 1220,  186,  155, 1465,  155,  702, 1456,  226,
 1458, 1326,  824, 1506,  825,  226,  384,  702, 1755,  702,
  656,  530,  917, 1028, 1067, 1069,  696,  237,  702, 1080,
  101,  917,  226,  700, 1074, 1100, 1101, 1102,  696, 1117,
 1404,  945,  946, 1355,  916,  143,  597,  696,  739, 1088,
  916,  917,  705,  726, 1592,  602, 1275,  732,  138,  226,
  226, 1519,  237,  237,  237, 1133, 1276, 1529, 1277,  383,
  621,  259,  702,  664,  700, 1573, 1810,  576,  732, 1472,
 1518,  702,  389, 1081,  186, 1231,  917,  291, 1256,  247,
  538,  696,  700,  705,  891,  620,  186,  390,  155,  898,
  696,  623, 1845, 1795,  143, 1384, 1797, 1155,  155,  378,
 1629,  705,  357,  177,  767, 1104,  664, 1126,  373,   79,
  726, 1156,  178,  186,  702, 1112,  661,  916,  383,  186,
  247,  186,  526,  247, 1195,  386,  576, 1846,  702, 1257,
  250,  389,  623,  696,  260,  725, 1827,  385,  247, 1847,
 1285, 1286,  374, 1221,  375,  767,  390,  696,  767,  388,
  623,  917, 1610,  738,  387, 1157,  725,  917,  572,  271,
  186, 1218,  177,  767,  703,  737,  376,  701,  186, 1222,
  947,  178, 1134, 1219,  703,  358,  703,  367,  732,  536,
 1455,  243, 1457, 1158,  386,  703,  737,  382,  701, 1586,
 1335,  377, 1514, 1022,  738,  702,  385,   14,  495,  210,
  215,  495,  244,  135,  724, 1402,  289,  792,  388,  495,
 1736, 1708,  738,  387,  696,  668,  337,  572,  695,  527,
  495,  495,  243, 1536,  748,  243,  732,  210,  814,  703,
  695,  186,  701, 1737,  917,  232,  287,  186,  703,  695,
  243,  701, 1456, 1355, 1355,  732,  337, 1159,  603,  633,
  368, 1355, 1355, 1209, 1355, 1355,  668,  634, 1040,  668,
  894, 1319, 1355, 1355, 1355, 1355,  725, 1355, 1355, 1355,
 1355, 1355, 1355,  528,  668,  232, 1337,  732,  186,  273,
  731,  703, 1341,  695,  701, 1314,  186,  916,  237, 1738,
  529,  471,  695,  225,  968,  703,  737,  486,  701, 1303,
  538,  894,  384,  237,  894,  815,  891,  603,  144,  237,
  891,  143,  143,  382, 1304,  597,  145, 1303,  301,  894,
  186,  274,  186,  383,  602,  186,  581, 1041,  226, 1460,
 1460, 1464,  226,  725, 1315,  695,  389,  385,  700, 1145,
 1850,  768, 1781,  278,  186,  816,  337, 1784,  597,  695,
  471,  390,  232,  894, 1487,  247,  247,  705,  302,  700,
  313,  177,  703,  247, 1488,  701, 1490,  144, 1492,  186,
  178,  123,  578,  177,  597,  145,  337,  597,  705, 1851,
  767,  767,  178,  602, 1407,  581,  602, 1146,  767,  386,
 1866,  613,  232,  597,  244,  478,  817, 1635,  225,  536,
  177,  385,  602,  700,  917,  123,  177,  143,  177,  178,
  293, 1147,  700,  388, 1314,  178,  695,  178,  387,  597,
  722,  720,  705,  721,  279,  723, 1852,  623,  602, 1535,
  226,  578, 1670, 1412, 1453,  244,  703, 1021,  244,  123,
  853, 1867, 1667,  186,  495,  282,  495,  177,  465,  383,
  613,  247,  327,  244, 1247,  700,  178,  243,  243,  580,
  495,  567,  389, 1318,  349,  243,  210, 1248,  738,  700,
  722,  720,  247,  721,  705,  723,  767,  390,  247, 1813,
  767,  853, 1020, 1020,  853, 1020, 1020, 1020,  705,  738,
 1249, 1713,  465,  598,  606,  703, 1806,  767,  502,  853,
 1020, 1020, 1020,  767,  757,  722,  720,  465,  721,  350,
  723,  327,  247, 1198, 1199,  386,  247, 1541,  580,  538,
  567, 1834, 1546,  843,  970,  970,  398,  385,  988,  241,
  668, 1020,  287,  668,  403, 1517,  700,  767,  288,  388,
  668, 1251,  668,  405,  387,  606,  668,  502,  406,  471,
  248,  668,  598,  243,  668,  705,  402, 1860, 1754, 1622,
  361,  471,  286,  362,  843,  207,  429,  843,  168,  668,
  241,   44,  247,  241,  243, 1869,  718,   52, 1872, 1590,
  243,  719,  843,  668,  370,  144,  603,  169,  241, 1877,
  659,  668,  370,  145,  471,  668,  471,  767,   42,  738,
  988,  292,  405,  659,  668,  363,  207,  406,   25,  207,
  763,  659,  177, 1025,  243,  402,  351,   48,  536,  603,
  668,  178, 1247,   50,  207,  567,  659,  168,  480, 1026,
   44,  147,  135,  204,  352, 1248,   52,  373,  568,  772,
  659, 1600, 1126, 1126,  308,  603,  169,  668,  603,  158,
 1713,  335, 1200, 1453, 1713,  718, 1804,   42, 1249,  668,
  719,  668,  894,  970,  603, 1713,  772,   25,  353,  763,
  244,  244, 1250,  378,  243,  494,   48,  659,  244,   26,
  159,  338,   50,  378, 1713,   49,  301,  480,  495,  527,
  603,  135, 1715,  324,   51,  160,  373,   46,  504, 1771,
  210,  210,  853,  447,  136,  718, 1728,  326,  668, 1251,
  719, 1020,  716,  124, 1770, 1713,  494, 1020, 1460,  494,
 1713,  339, 1020,   47,  155, 1020,  918,  512,  668,  613,
  527,  207,  668,  527,  494,  301,  517,  213,   26,  504,
  718, 1649,  504,  301,   49,  719,  301,  716,  527,  722,
  720,  334,  721,   51,  723,  853,   46,  504,  853,  716,
  515, 1020,  613,  136, 1835,  853,  244,  853,  521, 1020,
  549,  853,  124, 1706,  554,  926,  853, 1020,  213,  853,
 1749,  213,   47,  928, 1771,  843,  930,  244,  613,  659,
  207,  613,  343,  244,  853, 1020,  213, 1020,  341, 1833,
  103,  337,  736, 1010,  344,  735,  790,  613,  853,  359,
 1020,  210, 1020, 1020, 1020, 1010,  853,  372, 1010,  853,
  853, 1252, 1750, 1126,  370,  722,  720,  244,  721,  853,
  723,  598, 1015,  613,   83,   84,  105,  345,  843,  346,
  324,  843, 1766,  210, 1015,  853, 1724, 1015,  843, 1757,
  843, 1715,  381, 1759,  843, 1715,  810, 1715,  391,  843,
  958, 1020,  843,  538,  598,  155, 1715,  853,  803,  703,
  207,  802,  853,  666,  709,  710,  711,  843, 1790,  419,
  420,  421, 1747, 1773,  853, 1715,  853,  244,  538,  952,
  598,  843,  724,  598, 1477,  995, 1479,  810,  994,  843,
  810,  106,  843,  843,  552,  393,  553,  429,  430,  598,
  396,  107,  843,  666,  666,  810, 1715,  666,  218,  219,
 1109, 1715, 1714, 1108,  538,  395,  722,  720,  843,  721,
  483,  723,  666,  853,  399,  598,  108,  204, 1775, 1881,
 1113, 1882,  538,  881,  756,  538, 1001,  400,  758, 1776,
  843,  494,  494,  853,  490,  843,  538,  853,  998,  494,
 1208,  988,  536,  724,  499,  527,  527,  843,  102,  843,
  486,  498,  494,  527,  504,  504,  309,  502, 1211,  504,
  274, 1210,  504,  503,  718, 1260,  527,  536, 1108,  719,
  505, 1306,  226, 1798, 1305,  504,  507, 1453,  722,  720,
  508,  721, 1005,  723,  509,  226,  103, 1814,  155,  310,
  146,  147, 1311,  213,  213, 1310,  843, 1467,  104, 1864,
  724,  213, 1453,  536, 1194,  722,  720,  522,  721, 1550,
  723,  525,  724, 1814,  155,  311,  843,  524,  312,  534,
  843,  536,  105,  544,  536,  722,  720,  494,  721, 1584,
  723,  546,  724, 1621,  313,  536,  724,  548, 1453, 1005,
  718,  527,   14, 1814,  155,  719, 1840,  207,  494, 1839,
  504,  951,  722,  720,  494,  721, 1453,  723, 1126, 1453,
  314, 1714,  527,  207,  785, 1714, 1841, 1805,  527,  735,
 1453,  504, 1868, 1076, 1077, 1771, 1714,  504,  207,  368,
  207,  560,  368,  724,  527,  897,  724,  106,  494,  213,
  131,  131,  561,  562,  564, 1714,  585,  107,  810, 1765,
  582,  241,  527,  920,  595,  785,  599,  925,  785,  648,
  213,  504,  722,  720,  941,  721,  213,  723,  652,  948,
  949,  654,  108,  785,  692,  527, 1714,  656,  527, 1763,
  447, 1714,  959,  960,  961,  962,  963,  964,  447,  689,
  690,  718,  241,  527,  691,  241,  719,  693,  694, 1801,
  213,  810,  695, 1803,  810,  696,  697,  698,  699,  700,
  241,  810,  701,  810, 1809,  706,  707,  810,  666,  708,
  717,  666,  810,  725,  726,  810,  517,  724,  666, 1577,
  666,  730,  733, 1831,  666,  734,  738,  751,  743,  666,
  810,  919,  666,  923,  754,  761, 1031,  927,  929,  931,
  763,  807,  549, 1038,  810,  774,  778,  666,  779,  783,
 1008,  785,  810,  718, 1855,  810,  810,  786,  719, 1857,
  789,  666,  795,  797,  798,  810,  791,  555,  247,  666,
 1004,  722,  720,  666,  721,  794,  723,  804,  828,  829,
  718,  810,  666,  830, 1362,  719,  835,  293,  847,  840,
  984, 1008, 1008, 1008, 1008, 1008, 1008, 1008,  666,  841,
  718,  848,  248,  810,  849,  719,  850,  852,  810, 1008,
 1008, 1008, 1008,  853,  858, 1186,  722,  720,  864,  721,
  810,  723,  810,  865,  868,  666,  869,  718,  878,  880,
  881, 1561,  719,  886,  887,  888,  892,  666,  943,  666,
  295, 1008,  954, 1008, 1382,  722,  720,  859,  721,  973,
  723,  992,  989,  996, 1135, 1136, 1140,  990, 1000, 1149,
 1151, 1153, 1032, 1161, 1164, 1167, 1169, 1172, 1175,  810,
 1023, 1179, 1180, 1001, 1182,  735, 1184, 1018, 1035, 1187,
 1188, 1189, 1190, 1191, 1192, 1046,  666,  718, 1055,  810,
 1056, 1058,  719,  810,  807, 1062, 1196, 1063, 1201, 1065,
  527,  527, 1079, 1071, 1072,  527,  666, 1086,  527, 1073,
  666, 1092,  109, 1099, 1097, 1106, 1120,  241,  241,  785,
 1142,  527,  785, 1183, 1193,  241, 1383,  722,  720,  785,
  721,  785,  723, 1185, 1214,  785,  718, 1226, 1212, 1229,
  785,  110,  111,  785,  112, 1232,  220, 1224, 1240,  447,
 1244,  113,  114, 1246,  115,  116, 1264,  117,  785, 1254,
  118,  119,  120,  121,  122, 1267, 1287, 1289,  123,  124,
 1299, 1309,  785, 1307,  125,  126,  127, 1316,  655, 1320,
  785, 1321, 1322,  785,  785,  221, 1323,  220, 1331, 1114,
  220, 1339, 1344,  785, 1345, 1346,  527, 1386,  722,  720,
 1352,  721, 1353,  723, 1357,  220,  718, 1360, 1361,  785,
 1363,  719, 1008,  241, 1008, 1008, 1364,  527,  655,  655,
 1371, 1008,  655,  527, 1372,  916,  221, 1008, 1373,  221,
 1374,  785, 1008, 1376,  241, 1008,  785,  655, 1377, 1378,
  241, 1379, 1380, 1381,  221, 1401, 1398, 1470,  785, 1008,
  785,  718, 1454, 1468, 1008, 1008,  719,  527, 1008,  722,
  720, 1204,  721, 1471,  723, 1008,  759, 1008, 1008, 1480,
 1475, 1008,  811, 1481,  241, 1008, 1483, 1008, 1482, 1008,
  718, 1008, 1491, 1508,  577,  719, 1008, 1008, 1509, 1008,
 1504, 1510, 1511, 1512, 1513,  447, 1516,  785,  722,  720,
 1083,  721, 1531,  723, 1008, 1008, 1537, 1008, 1365, 1366,
 1521, 1368, 1370,  811, 1539, 1554,  811,  785, 1008, 1375,
 1008,  785, 1008, 1008, 1008, 1540, 1008, 1545, 1552, 1008,
 1008,  811, 1574, 1580, 1581, 1587, 1588, 1247, 1388, 1008,
 1593, 1594,  138, 1606, 1596, 1608, 1395, 1396, 1201, 1597,
 1248, 1201, 1008, 1196, 1604, 1008, 1605, 1403,  447, 1405,
 1618, 1607,  718, 1613, 1387,  722,  720,  719,  721, 1614,
  723, 1008, 1617, 1249, 1619, 1623, 1630, 1008, 1008, 1389,
  722,  720, 1008,  721, 1640,  723, 1650, 1250, 1652, 1653,
 1654, 1656, 1655, 1008, 1008, 1730, 1008,  447, 1657, 1659,
 1660, 1669, 1008, 1390,  722,  720, 1661,  721, 1671,  723,
 1676, 1677, 1675, 1391,  722,  720, 1680,  721, 1681,  723,
 1682, 1683,  232,  220, 1251, 1686, 1687, 1688, 1689, 1690,
 1691, 1008, 1008,  718, 1008, 1499, 1692, 1693,  719, 1694,
 1695, 1008, 1008, 1008, 1008, 1008, 1697, 1008, 1703, 1710,
 1008, 1008, 1008, 1008, 1008, 1609, 1725, 1735, 1008, 1008,
 1743,  232,  221, 1008, 1008, 1008, 1008, 1008, 1745, 1392,
  722,  720, 1746,  721, 1758,  723, 1762, 1768, 1769, 1393,
  722,  720,  988,  721, 1779,  723, 1394,  722,  720, 1772,
  721, 1780,  723,  655,  718, 1802,  655,  722,  720,  719,
  721, 1782,  723,  655, 1547,  655, 1785, 1787, 1788,  655,
 1553, 1791, 1731, 1799,  655, 1557, 1828,  655,  232, 1823,
  759, 1829, 1562,  988,  988,  988,  988,  988, 1001,  988,
 1832, 1771,  655,  718,  811, 1825, 1252, 1838,  719,  232,
 1843,  988,  988,  988,  988,  232,  655, 1844, 1820, 1821,
 1856, 1201, 1858, 1859,  655, 1201, 1863,  232,  655, 1865,
 1007,  722,  720, 1205,  721, 1875,  723,  655, 1878, 1879,
 1880, 1595, 1883,  988,    1,  988,  732,  193,  232,  232,
   30,   32,  982,  655,  232,  767,  261,  811,  370,  156,
  811,  462,  648,  969,   82,  893,  272,  811,  203,  811,
  718,  739,  271,  811,  398,  719,  968,  183,  811,  525,
  655,  811,  692,  184,  949,  718,  694,  371,  232,  711,
  719,  213,  655,  322,  655,  288,  811,  948,  710,  330,
  655,  357, 1575,  722,  720, 1626,  721,  342,  723,  718,
  811,  712,  328,  721,  719,   27,   30,   40,  811,  718,
 1459,  811,  811,  143,  719,  506,   55, 1340,  497,  655,
  655,  811,  655,  496,  991,  879, 1119,   56, 1338,  655,
  655,  655,  655,  655,   57,  655, 1124,  811,  655,  655,
  655,  655,  655,  261, 1336, 1082,  655,  655, 1328, 1662,
  566,  655,  655,  655,  655,  655, 1663,  607,   58,  811,
  842, 1266, 1317,  542,  811,  718, 1329, 1679,  579, 1033,
  719,  580, 1701, 1684, 1685,  718,  811, 1744,  811, 1181,
  719, 1060,  718, 1819, 1648, 1642, 1826,  719, 1854, 1824,
 1696,  823,  718, 1053, 1201, 1216, 1764,  719, 1699, 1830,
 1722, 1599, 1807, 1705,  988, 1707,  988,  988,  970, 1486,
 1633,  572, 1098,  988,  827,  563, 1528,  444,  587,  988,
   59,  145,  535,  866,  988,  811,  846,  988,  769, 1576,
  722,  720, 1089,  721,  306,  723,  773, 1020, 1024, 1478,
 1007,  988, 1243,  988, 1012,  811,  988,  988,  256,  811,
  988,   60, 1786, 1739,  997,   61,  718,  988, 1862,  988,
  988,  719, 1842,  988, 1583,  722,  720,  988,  721,  988,
  723,  988, 1495,  988, 1037, 1400,    0,  782,  988,  988,
 1367,  988, 1236, 1756,    0, 1012, 1012, 1012, 1012, 1012,
 1012, 1012,    0,    0,    0,    0,  988,  988,    0,  988,
    0,    0,    0, 1012, 1012, 1012, 1012,    0,    0,    0,
  988,    0,  988,    0,  988,  988,  988,    0,  988,  447,
    0,  988,  988,  967,    0,    0,    0,    0,  718,    0,
    0,  988,    0,  719, 1150, 1012, 1154, 1012, 1162, 1165,
    0, 1170, 1173, 1176,  988,    0,    0,  988, 1626,    0,
    0,    0,    0,    0,    0,    0, 1760,  722,  720,    0,
  721,    0,  723,  988,    0,    0,    0,    0,    0,  988,
  988,    0,    0,    0,  988,  722,  720,    0,  721,    0,
  723,    0,    0,    0,    0,  988,  988,    0,  988,   14,
  722,  720, 1808,  721,  988,  723,    0,    0,    0,  722,
  720,    0,  721,    0,  723,  932,  933,  934,  935,  936,
  937,  938,  939,    0,    0,    0,    0,    0,   87,    0,
    0,    0,    0,  988,  988,   88,  988,    0,    0,   89,
   90,    0,  109,  988,  988,  988,  988,  988,   91,  988,
    0,    0,  988,  988,  988,  988,  988,    0,    0,   92,
  988,  988,    0,    0,    0,  988,  988,  988,  988,  988,
    0,  110,  111,    0,  112,  718,    0,    0,    0,    0,
  719,  113,  114,    0,  115,  116,    0,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
  718,  967,    0,    0,    0,  719, 1012,    0, 1012, 1012,
    0,   93,    0,    0,    0, 1012,    0,    0,    0,    0,
    0, 1012,    0,    0,    0,    0, 1012,   94,    0, 1012,
    0,  722,  720, 1585,  721,    0,  723,    0,    0,    0,
    0,    0,   95, 1012, 1017,    0,   96,    0, 1012, 1012,
    0,    0, 1012,    0,    0,    0,    0,   97,  444, 1012,
    0, 1012, 1012,    0,    0, 1012,    0,    0,    0, 1012,
    0, 1012,    0, 1012,  966, 1012,   98,    0,    0,    0,
 1012, 1012,    0, 1012,    0, 1017, 1017, 1017, 1017, 1017,
 1017, 1017,  718,    0,    0, 1351,    0,  719, 1012, 1012,
    0, 1012,    0, 1017, 1017, 1017, 1017,    0,    0,    0,
  718,  109, 1012,    0, 1012,  719, 1012, 1012, 1012, 1560,
 1012,    0,    0, 1012, 1012,  718,    0,    0,    0,    0,
  719,    0,    0, 1012,  718, 1017,    0, 1017,    0,  719,
  110,  111,    0,  112,    0,    0, 1012, 1589,    0, 1012,
  113,  114,  109,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0, 1012,    0,  123,  124,    0,
    0, 1012, 1012,  125,  126,  127, 1012,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0, 1012, 1012,    0,
 1012,  113,  114,    0,  115,  116, 1012,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0, 1012, 1012,    0, 1012,    0,
    0,    0,    0,  109,    0, 1012, 1012, 1012, 1012, 1012,
    0, 1012,    0,    0, 1012, 1012, 1012, 1012, 1012,    0,
    0,    0, 1012, 1012,    0,    0,    0, 1012, 1012, 1012,
 1012, 1012,  110,  111,    0,  112,  718,    0,    0,    0,
    0,  719,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  126,  127,    0,    0,
    0,    0,    0,  652,    0,    0, 1017,    0, 1017, 1017,
    0, 1548, 1549,    0,    0, 1017,    0,    0,    0, 1555,
 1556, 1017, 1558, 1559,    0,    0, 1017,    0,    0, 1017,
 1563, 1564, 1565, 1566,    0, 1567, 1568, 1569, 1570, 1571,
 1572,    0,    0, 1017, 1013,    0,    0,    0, 1017, 1017,
    0,    0, 1017,    0,    0,    0,    0,    0,    0, 1017,
    0, 1017, 1017,    0,    0, 1017,    0,    0,    0, 1017,
    0, 1017,    0, 1017,    0, 1017,    0,    0,    0,    0,
 1017, 1017,    0, 1017,    0, 1013, 1013, 1013, 1013, 1013,
 1013, 1013,    0,    0,    0,    0,    0,    0, 1017, 1017,
    0, 1017,    0, 1013, 1013, 1013, 1013,    0,    0,    0,
    0,  109, 1017,    0, 1017,    0, 1017, 1017, 1017,    0,
 1017,    0,    0, 1017, 1017,    0,   80,    0,    0,    0,
    0,    0,    0, 1017,    0, 1013,    0, 1013,    0,    0,
  110,  111,    0,  112,    0,    0, 1017,    0,    0, 1017,
  113,  114,  109,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0, 1017,    0,  123,  124,    0,
    0, 1017, 1017,  125,  126,  127, 1017,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0, 1017, 1017,    0,
 1017,  113,  114,    0,  115,  116, 1017,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0, 1017, 1017,    0, 1017,    0,
    0,    0,    0,    0,    0, 1017, 1017, 1017, 1017, 1017,
  109, 1017,    0,    0, 1017, 1017, 1017, 1017, 1017,    0,
    0,    0, 1017, 1017,    0,    0,    0, 1017, 1017, 1017,
 1017, 1017,    0,    0,    0,    0,    0,    0,    0,  110,
  111,    0,  112,    0,    0,    0,  652,    0,    0,  113,
  114,    0,  115,  116,  577,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  126,  127,    0, 1013,    0, 1013, 1013,
    0,    0,    0,    0,    0, 1013,    0,    0,    0,    0,
    0, 1013,    0,    0,    0,    0, 1013,    0,    0, 1013,
    0,    0,    0,    0,    0,  525,    0,    0,    0,    0,
    0,    0,    0, 1013, 1026,    0,    0,    0, 1013, 1013,
    0,    0, 1013,    0,    0,    0,    0,    0,    0, 1013,
    0, 1013, 1013,    0,    0, 1013,    0,    0,    0, 1013,
    0, 1013,    0, 1013,    0, 1013,  525,    0,    0,  525,
 1013, 1013,    0, 1013,    0, 1026, 1026, 1026, 1026, 1026,
 1026, 1026,    0,    0,  525,    0,  525,    0, 1013, 1013,
    0, 1013,    0, 1026, 1026, 1026, 1026,    0,    0,    0,
    0,  655, 1013,    0, 1013,    0, 1013, 1013, 1013,    0,
 1013,    0,    0, 1013, 1013,    0,    0,    0,    0,    0,
    0,    0,    0, 1013,    0, 1026,    0, 1026,    0,    0,
  655,  655,    0,  655,    0,    0, 1013, 1598,    0, 1013,
  655,  655,  109,  655,  655,    0,  655,    0,    0,  655,
  655,  655,  655,  655,    0, 1013,    0,  655,  655,    0,
    0, 1013, 1013,  655,  655,  655, 1013,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0, 1013, 1013,    0,
 1013,  113,  114,    0,  115,  116, 1013,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0, 1013, 1013,    0, 1013,    0,
    0,   79,    0,  109,    0, 1013, 1013, 1013, 1013, 1013,
    0, 1013,    0,    0, 1013, 1013, 1013, 1013, 1013,    0,
    0,    0, 1013, 1013,    0,    0,    0, 1013, 1013, 1013,
 1013, 1013,  110,  111,    0,  112,    0,    0,    0,    0,
    0,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,  525,    0,  125,  126,  127,    0,    0,
    0,    0,    0,    0,    0,    0, 1026,    0, 1026, 1026,
  525,  525,  525,    0,    0, 1026,  525,    0,    0,  525,
    0, 1026,    0,    0,    0,    0, 1026,    0,    0, 1026,
    0,    0,  525,    0,    0,  522,    0,    0,  525,    0,
    0,    0,    0, 1026,  862,    0,    0,    0, 1026, 1026,
    0,    0, 1026,    0,    0,    0,    0,    0,    0, 1026,
    0, 1026, 1026,    0,    0, 1026,    0,    0,    0, 1026,
    0, 1026,    0, 1026,    0, 1026,  522,    0,    0,  522,
 1026, 1026,   65, 1026,    0,  862,  862,  862,  862,  862,
    0,  862,    0,   66,  522,    0,  522,    0, 1026, 1026,
   67, 1026,    0,  862,  862,  862,  862,  525,    0,    0,
    0,    0, 1026,   68, 1026,    0, 1026, 1026, 1026,    0,
 1026,    0,    0, 1026, 1026,    0,    0,    0,  525,    0,
    0,    0,    0, 1026,  525,    0,    0,  862,    0,    0,
    0,    0,    0,  525,    0,  525, 1026,    0,    0, 1026,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  525,    0, 1026,    0,    0,  525,    0,
   69, 1026, 1026,    0,    0,    0, 1026,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   70, 1026, 1026,    0,
 1026,    0,    0,    0,    0,    0, 1026,  525,    0,    0,
    0,    0,   71,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   72,    0,    0,
    0,   73,    0,    0,   74, 1026, 1026,    0, 1026,    0,
    0,    0,   75,    0,    0, 1026, 1026, 1026, 1026, 1026,
  109, 1026,    0,    0, 1026, 1026, 1026, 1026, 1026,    0,
    0,   76, 1026, 1026,    0,    0,    0, 1026, 1026, 1026,
 1026, 1026,    0,    0,    0,    0,    0,    0,    0,  110,
  111,    0,  112,    0,    0,    0,    0,    0,    0,  113,
  114,    0,  115,  116,    0,  117,    0,    0,  118,  119,
  120,  121,  122,  522,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  126,  127,    0,  862,    0,  862,  862,
  522,  522,  522,    0,    0,  862,  522,    0,    0,  522,
    0,  862,    0,    0,    0,    0,  862,    0,    0,  862,
    0,    0,  522,    0,    0,  527,    0,    0,  522,    0,
    0,    0,    0,  862,  874,    0,    0,    0,  862,  862,
    0,    0,  862,    0,    0,    0,    0,    0,    0,  862,
    0,  862,  862,    0,    0,  862,  583,    0,    0,  862,
    0,  862,    0,  862,    0,  862,  527,    0,    0,  527,
  862,  862,    0,  862,    0,  874,  874,  874,  874,  874,
    0,  874,    0,    0,  527,    0,  527,    0,  862,  862,
    0,  862,    0,  874,  874,  874,  874,  522,    0,    0,
    0,    0,  862,    0,  862,    0,  862,  862,  862,    0,
  862,    0,    0,  862,  862,    0,    0,    0,  522,    0,
    0,    0,    0,  862,  522,    0,    0,  874,    0,    0,
    0,    0,    0,    0,    0,  522,  862,    0,    0,  862,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  522,    0,  862,    0,    0,  522,    0,
    0,  862,  862,    0,    0,    0,  862,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  862,  862,    0,
  862,    0,    0,    0,    0,    0,  862,  522,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  862,  862,    0,  862,    0,
    0,    0,    0,    0,  109,  862,  862,  862,  862,  862,
    0,  862,    0,    0,  862,  862,  862,  862,  862,    0,
    0,    0,  862,  862,    0,    0,    0,  862,  862,  862,
  862,  862,    0,  110,  111,    0,  112,    0,    0,    0,
    0,    0,    0,  113,  114,    0,  115,  116,    0,  117,
    0,    0,  118,  119,  120,  121,  122,    0,    0,    0,
  123,  124,    0,  527,    0,    0,  125,  126,  127,    0,
    0,    0,    0,    0,    0,    0,  874,    0,  874,  874,
    0,  527,  527,    0,    0,  874,  527,    0,    0,  527,
    0,    0,    0,    0,    0,    0,  874,    0,    0,  874,
    0,    0,  527,    0,    0,    0,    0,    0,  527,    0,
    0,    0,    0,  874,  877,    0,    0,    0,  874,  874,
    0,    0,  874,    0,    0,    0,    0,    0,    0,  874,
    0,  874,  874,    0,    0,  874,    0,    0,    0,  874,
    0,  874,    0,  874,    0,  874,    0,    0,    0,    0,
  874,  874,    0,  874,    0,  877,  877,  877,  877,  877,
    0,  877,    0,    0,    0,    0,    0,    0,  874,  874,
    0,  874,    0,  877,  877,  877,  877,  527,    0,    0,
    0,    0,  874,    0,  874,    0,  874,  874,  874,    0,
  874,    0,    0,  874,  874,    0,    0,    0,  527,    0,
    0,    0,    0,  874,  527,    0,    0,  877,    0,    0,
    0,    0,    0,    0,    0,  527,  874,    0,    0,  874,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  527,    0,  527,    0,  874,    0,    0,  527,    0,
    0,  874,  874,    0,    0,    0,  874,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  874,  874,    0,
  874,    0,    0,    0,    0,    0,  874,  527,    0,    0,
    0,    0,  527,    0,    0,  527,    0,  487,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  527,    0,  527,    0,    0,  874,  874,  537,  874,    0,
    0,    0,    0,  109,    0,  874,  874,  874,  874,  874,
    0,  874,    0,    0,  874,  874,  874,  874,  874,    0,
    0,    0,  874,  874,    0,    0,    0,  874,  874,  874,
  874,  874,  110,  111,    0,  112,    0,  537,  537,    0,
    0,  537,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,  537,    0,  537,  123,
  124,    0,    0,    0,    0,  125,  126,  127,    0,    0,
    0,    0,    0,    0,    0,    0,  877,    0,  877,  877,
    0,    0,    0,    0,    0,  877,    0,    0,  537,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  877,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  877,  878,    0,    0,    0,  877,  877,
    0,    0,  877,    0,    0,    0,    0,    0,    0,  877,
    0,  877,  877,    0,    0,  877,    0,    0,    0,  877,
    0,  877,    0,  877,    0,  877,    0,    0,    0,    0,
  877,  877,    0,  877,    0,  878,  878,  878,  878,  878,
    0,  878,    0,    0,    0,    0,    0,    0,  877,  877,
    0,  877,    0,  878,  878,  878,  878,    0,    0,    0,
    0,    0,  877,    0,  877,    0,  877,  877,  877,  527,
  877,    0,    0,  877,  877,    0,    0,    0,    0,    0,
    0,    0,    0,  877,  527,    0,    0,  878,    0,    0,
    0,    0,    0,    0,    0,    0,  877,    0,    0,  877,
    0,    0,    0,    0,    0,    0,    0,    0,  527, 1269,
    0,    0,    0,    0,  527,  877,    0,  527,    0,    0,
    0,  877,  877,    0,    0,    0,  877,    0,    0,    0,
    0,    0,    0,    0,    0,  537,    0,  877,  877,    0,
  877,    0,    0,    0,    0,    0,  877,    0,    0,    0,
  537,    0,  537,  537,  537,    0,    0,    0,  537,  527,
    0,  537,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  537,  877,  877,    0,  877,    0,
  537,    0,    0,  537,    0,  877,  877,  877,  877,  877,
    0,  877,    0,    0,  877,  877,  877,  877,  877,    0,
    0,    0,  877,  877,    0,    0,    0,  877,  877,  877,
  877,  877,    0,    0,  527,    0,    0,    0,    0,    0,
    0,  527,    0,    0,    0,  537,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  527,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  537,
    0,    0,    0,    0,    0,    0,  878,    0,  878,  878,
    0,    0,    0,    0,    0,  878,    0,    0,    0,  527,
  537,    0,    0,  527,    0,  978,  537,    0,    0,  878,
  537,    0,    0,  872,    0,    0,    0,  537,    0,    0,
    0,    0,    0,  878,    0,    0,    0,    0,  878,  878,
    0,    0,  878,    0,    0,  537,    0,    0,    0,  878,
  537,  878,  878,    0,    0,  878,    0,    0,    0,  878,
    0,  878,    0,  878,  872,  878,  872,  872,  872,    0,
  878,  878,    0,  878,    0,  537,    0,    0,    0,  537,
    0,    0,  872,  872,  872,  872,    0,    0,  878,  878,
    0,  878,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,  878,    0,  878,    0,  878,  878,  878,    0,
  878,    0,    0,  878,  878,    0,  872,    0,    0,    0,
    0,    0,    0,  878,  409,  981,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  878,    0,    0,  878,
    0,    0,  109,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,    0,  878,    0,    0,    0,    0,
    0,  878,  878,    0,    0,    0,  878,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0,  878,  878,    0,
  878,  113,  114,    0,  115,  116,  878,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0,  878,  878,    0,  878,    0,
    0,    0,    0,  416,    0,  878,  878,  878,  878,  878,
    0,  878,    0,    0,  878,  878,  878,  878,  878,  418,
    0,    0,  878,  878,    0,    0,    0,  878,  878,  878,
  878,  878,    0,  419,  420,  421,    0,  218,  219,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    1,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  872,    0,  872,  872,    0,
    2,  434,  435,    0,  872,    0,    0,    0,    3,    0,
  439,    0,    0,    0,    0,    4,    0,    5,  872,    6,
    0,    0,  871,    0,    7,    0,    0,    0,    0,    0,
    0,    8,  872,    0,    0,    0,    0,  872,  872,    0,
    0,  872,    0,    0,    0,    9,    0,    0,  872,    0,
  872,  872,    0,    0,  872,   10,    0,    0,  872,    0,
  872,    0,  872,  871,  872,  871,  871,  871,    0,  872,
  872,    0,  872,    0,    0,    0,    0,    0,    0,    0,
    0,  871,  871,  871,  871,    0,    0,  872,  872,    0,
  872,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  872,    0,  872,    0,  872,  872,  872,    0,  872,
    0,    0,  872,  872,    0,  871,    0,    0,   11,    0,
    0,    0,  872,    0,    0,   12,    0, 1736,   13,    0,
   14,   15,    0,    0,    0,  872,    0,    0,  872,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1737,    0,    0,    0,  872,   16,    0,    0,    0,    0,
  872,  872,    0,    0,    0,  872,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  872,  872,    0,  872,
    0,    0,    0,    0,    0,  872,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   17,    0, 1738,    0,    0,    0,
    0,    0,    0,    0,  872,  872,    0,  872,    0,   18,
   19,    0,    0,    0,  872,  872,  872,  872,  872,    0,
  872,    0,    0,  872,  872,  872,  872,  872,    0,    0,
  109,  872,  872,    0,    0,    0,  872,  872,  872,  872,
  872,    0,    0,    0,  951,  722,  720,    0,  721,    0,
  723,    0,    0,    0,    0,    0,    0,    0,    0,  110,
  111,    0,  112,  914,  913,  915,    0,    0,    0,  113,
  114,    0,  115,  116,  871,  117,  871,  871,  118,  119,
  120,  121,  122,  871,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  126,  127,    0,    0,  871,    0,    0,
    0,  873,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  871,    0,    0,    0,    0,  871,  871,    0,    0,
  871,    0,    0,    0,    0,    0,    0,  871,    0,  871,
  871,    0,    0,  871,    0,    0,    0,  871,    0,  871,
    0,  871,  873,  871,  873,  873,  873,    0,  871,  871,
    0,  871,    0,    0,    0,    0,  491,    0,    0,    0,
  873,  873,  873,  873,    0,    0,  871,  871,    0,  871,
    0,    0,    0,  109,    0,    0,    0,    0,    0,    0,
  871,    0,  871,    0,  871,  871,  871,    0,  871,    0,
    0,  871,  871,    0,  873,    0,    0,    0,    0,    0,
    0,  871,  110,  111,    0,  112,    0,    0,    0,    0,
    0,    0,  113,  114,  871,  115,  116,  871,  117,    0,
  109,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,  871,    0,  125,  126,  127,    0,  871,
  871,    0,    0,    0,  871,    0,    0,    0,    0,  110,
  111,    0,  112,    0,    0,  871,  871,    0,  871,  113,
  114,    0,  115,  116,  871,  117,    0,    0,  118,  119,
  120,  121,  122,    0,  901,    0,  123,  124,    0,    0,
  718,    0,  125,  126,  127,  719,    0,    0,  902,    0,
    0,    0,    0,  871,  871,    0,  871,    0,    0,    0,
    0,    0,    0,  871,  871,  871,  871,  871,    0,  871,
    0,    0,  871,  871,  871,  871,  871,    0,    0,    0,
  871,  871,    0,    0,  903,  871,  871,  871,  871,  871,
    0,    0,  904,    0,  722,  720,    0,  721,    0,  723,
  905,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  914,  913,  915,    0,    0,    0,  906,    0,
  907,    0,    0,  873,    0,  873,  873,    0,    0,    0,
    0,    0,  873,  908,    0,  909,  910,  911,    0,    0,
    0,    0,    0,    0,    0,    0,  873,    0,    0,    0,
  876,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  873,    0,    0,    0,    0,  873,  873,    0,    0,  873,
    0,    0,    0,    0,    0,    0,  873,    0,  873,  873,
    0,    0,  873,    0,  912,    0,  873,    0,  873,    0,
  873,  876,  873,  876,  876,  876,    0,  873,  873,    0,
  873,  211,    0,    0,  211,    0,    0,    0,    0,  876,
  876,  876,  876,    0,    0,  873,  873,  211,  873,    0,
    0,  211,    0,    0,  211,    0,    0,    0,    0,  873,
    0,  873,    0,  873,  873,  873,    0,  873,    0,    0,
  873,  873,    0,  876,    0,    0,    0,    0,    0,    0,
  873,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  873,  211,    0,  873,    0,    0,    0,
  211,  211,  211,    0,  211,  211,  211,    0,  211,    0,
    0,    0,  873,    0,    0,    0,    0,    0,  873,  873,
    0,    0,    0,  873,    0,    0,  211,    0,  211,    0,
    0,    0,    0,    0,  873,  873,    0,  873,    0,    0,
    0,    0,    0,  873,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  901,    0,    0,    0,  211,    0,  718,
    0,    0,   81,    0,  719,    0,    0,  902,    0,    0,
    0,    0,  873,  873,  211,  873,    0,    0,    0,    0,
    0,    0,  873,  873,  873,  873,  873,    0,  873,    0,
    0,  873,  873,  873,  873,  873,  211,    0,    0,  873,
  873,    0,    0,  903,  873,  873,  873,  873,  873,    0,
    0,  904,    0,    0,    0,    0,    0,    0,    0,  905,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  854,  211,  211,    0,    0,    0,    0,  906,    0,  907,
    0,    0,  876,    0,  876,  876,    0,    0,  211,    0,
    0,  876,  908,    0,  909,  910,  911,    0,    0,    0,
    0,    0,    0,    0,    0,  876,    0,    0,    0,    0,
    0,  854, 1021, 1021,  854, 1021, 1021, 1021,    0,  876,
    0,    0,    0,    0,  876,  876,    0,    0,  876,  854,
 1021, 1021, 1021,    0,    0,  876,    0,  876,  876,    0,
    0,  876,    0,  912,    0,  876,    0,  876,    0,  876,
    0,  876,    0,    0,    0,    0,  876,  876,    0,  876,
    0, 1021,    0,    0,    0,    0,    0,  837,    0,    0,
    0,    0,    0,    0,  876,  876,    0,  876,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  876,    0,
  876,    0,  876,  876,  876,    0,  876,    0,    0,  876,
  876,    0,    0,    0,    0,    0,    0,    0,  837,  876,
    0,  837,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  876,    0,    0,  876,  837,    0,  570,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  876,    0,    0,    0,    0,    0,  876,  876,    0,
    0,    0,  876,    0,    0,    0,    0,  570,  570,    0,
  570,    0,    0,  876,  876,    0,  876,  570,  570,    0,
  570,  570,  876,  570,    0,    0,  570,  570,  570,  570,
  570,    0,    0,    0,  570,  570,    0,    0,    0,    0,
  570,  570,  570,    0,    0,    0,    0,    0,    0,    0,
    0,  876,  876,    0,  876,    0,    0,    0,    0,    0,
    0,  876,  876,  876,  876,  876,    0,  876,    0,  855,
  876,  876,  876,  876,  876,    0,    0,    0,  876,  876,
    0,    0,  854,  876,  876,  876,  876,  876,    0,    0,
    0, 1021,    0,    0,    0,    0,    0, 1021,    0,    0,
    0,    0, 1021,    0,    0, 1021,    0,    0,    0,    0,
  855, 1022, 1022,  855, 1022, 1022, 1022,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  855, 1022,
 1022, 1022,    0,    0,    0,  854,    0,    0,  854,    0,
    0, 1021,    0,    0,    0,  854,    0,  854,    0, 1021,
    0,  854,    0,    0,    0,    0,  854, 1021,    0,  854,
 1022,    0,    0,    0,  684,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  854, 1021,    0, 1021,    0,  837,
    0,    0,    0,    0,    0,    0,    0,    0,  854,    0,
 1021,    0, 1021, 1021, 1021,    0,  854,    0,    0,  854,
  854,    0,    0,    0,    0,  684,    0,    0,  684,  854,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  684,    0,  854,    0,    0,    0,    0,
    0,    0,  837,    0,    0,  837,    0,    0,    0,  682,
    0, 1021,  837,    0,  837,    0,    0,  854,  837,    0,
    0,    0,  854,  837,    0,    0,  837,    0,    0,    0,
    0,    0,    0,    0,  854,    0,  854,    0,    0,    0,
    0,  837,    0,    0,    0,    0,    0,    0,    0,    0,
  682,    0,    0,  682,    0,  837,    0,    0,    0,    0,
    0,    0,    0,  837,    0,    0,  837,  837,  682,    0,
    0,    0,    0,    0,    0,    0,  837,    0,    0,    0,
    0,    0,    0,  854,    0,    0,    0,    0,    0,    0,
    0,    0,  837,    0,    0,    0,    0,    0,    0,    0,
    0,  855,    0,  854,    0,    0,    0,  854,    0,    0,
 1022,    0,    0,    0,  837,    0, 1022,    0,    0,  837,
    0, 1022,    0,    0, 1022,    0,    0,    0,    0,    0,
    0,  837,    0,  837,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  855,    0,    0,  855,    0,    0,
 1022,    0,    0,    0,  855,    0,  855,    0, 1022,    0,
  855,    0,    0,    0,    0,  855, 1022,    0,  855,    0,
  837,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  855, 1022,    0, 1022,    0,  684,    0,
  837,    0,    0,    0,  837,    0,    0,  855,    0, 1022,
    0, 1022, 1022, 1022,    0,  855,    0,    0,  855,  855,
    0,    0,    0,  814,    0,    0,    0,    0,  855,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  855,    0,    0,    0,    0,  684,
    0,    0,  684,  812,    0,    0,    0,    0,    0,  684,
 1022,  684,    0,    0,  814,  684,  855,  814,    0,    0,
  684,  855,    0,  684,    0,    0,    0,    0,    0,    0,
    0,    0,  814,  855,    0,  855,    0,    0,  684,    0,
    0,    0,    0,    0,  812,    0,    0,  812,    0,    0,
    0,    0,  684,    0,    0,    0,    0,    0,    0,    0,
  684,    0,  812,    0,  684,    0,    0,    0,    0,    0,
    0,    0,    0,  684,  682,    0,    0,  682,    0,    0,
    0,    0,  855,    0,  682,    0,  682,    0,    0,  684,
  682,    0,    0,    0,    0,  682,    0,    0,  682,    0,
    0,    0,  855,    0,    0,    0,  855,    0,    0,    0,
    0,    0,    0,  682,    0,    0,  684,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  682,  684,    0,
  684,    0,    0,    0,    0,  682,  684,    0,    0,  682,
    0,    0,    0,    0,    0,    0,    0,    0,  682,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  682,  684,  684,    0,  684,    0,
    0,    0,    0,    0,  813,  684,  684,  684,  684,  684,
    0,  684,    0,    0,  684,  684,  684,  684,  684,    0,
    0,  682,  684,  684,    0,    0,    0,  684,  684,  684,
  684,  684,    0,  682,    0,  682,    0,    0,    0,    0,
    0,  655,    0,    0,    0,  813,    0,    0,  813,    0,
  102,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  813,    0,  814,    0,    0,    0,    0,
  655,  655,    0,  655,    0,    0,  845,    0,    0,    0,
  655,  655,  682,  655,  655,    0,  655,    0,  103,  655,
  655,  655,  655,  655,    0,  812,    0,  655,  655,    0,
  104,    0,  682,  655,  655,  655,  682,    0,    0,    0,
    0,    0,    0,    0,  140,    0,    0,  845,  814,    0,
  845,  814,    0,    0,  105,    0,    0,    0,  814,    0,
  814,    0,    0,    0,  814,  845,  807,    0,    0,  814,
    0,    0,  814,    0,    0,    0,    0,    0,  812,    0,
    0,  812,    0,    0,    0,    0,    0,  814,  812,    0,
  812,    0,    0,    0,  812,    0,    0,    0,    0,  812,
    0,  814,  812,    0,    0,    0,    0,  807,    0,  814,
  807,    0,  814,  814,    0,    0,    0,  812,    0,  106,
    0,    0,  814,    0,    0,  807,    0,    0,    0,  107,
    0,  812,    0,    0,    0,    0,    0,    0,  814,  812,
    0,  808,  812,  812,    0,    0,    0,    0,    0,    0,
    0,    0,  812,    0,  108,    0,    0,    0,    0,    0,
  814,    0,    0,    0,    0,  814,    0,    0,  812,    0,
    0,    0,  109,    0,    0,    0,    0,  814,    0,  814,
    0,    0,  808,    0,    0,  808,    0,    0,    0,    0,
  812,    0,    0,    0,    0,  812,  813,    0,    0,    0,
  808,  110,  111,    0,  112,    0,    0,  812,    0,  812,
    0,  113,  114,  809,  115,  116,    0,  117,    0,    0,
  118,  119,  120,  121,  122,    0,  814,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  814,    0,    0,  813,
  814,    0,  813,    0,  809,    0,  812,  809,    0,  813,
    0,  813,    0,    0,    0,  813,    0,    0,  845,    0,
  813,    0,  809,  813,    0,    0,  812,    0,    0,    0,
  812,    0,    0,    0,    0,  844,    0,    0,  813,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  813,    0,    0,    0,    0,    0,    0,    0,
  813,    0,    0,  813,  813,    0,    0,    0,    0,    0,
    0,  845,    0,  813,  845,    0,  844,    0,  807,  844,
    0,  845,    0,  845,    0,    0,    0,  845,    0,  813,
    0,    0,  845,    0,  844,  845,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  838,    0,    0,
  845,  813,    0,    0,    0,    0,  813,    0,    0,    0,
    0,    0,    0,    0,  845,    0,    0,    0,  813,    0,
  813,  807,  845,    0,  807,  845,  845,    0,    0,    0,
    0,  807,    0,  807,    0,  845,    0,  807,  838,    0,
    0,  838,  807,  808,    0,  807,    0,    0,    0,    0,
    0,  845,    0,    0,    0,    0,  838,  846,    0,    0,
  807,    0,    0,    0,    0,    0,    0,  813,    0,    0,
    0,    0,    0,  845,  807,    0,    0,    0,  845,    0,
    0,    0,  807,    0,    0,  807,  807,  813,    0,    0,
  845,  813,  845,    0,    0,  807,  808,    0,  846,  808,
    0,  846,    0,    0,    0,    0,  808,    0,  808,    0,
    0,  807,  808,    0,    0,  809,  846,  808,    0,    0,
  808,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  847,  807,    0,  808,    0,    0,  807,  845,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  808,
  807,    0,  807,    0,    0,    0,    0,  808,    0,  845,
  808,  808,    0,  845,    0,    0,    0,    0,  809,    0,
  808,  809,    0,  847,    0,    0,  847,    0,  809,    0,
  809,    0,    0,    0,  809,    0,  808,  844,    0,  809,
    0,  847,  809,    0,    0,    0,    0,    0,    0,  807,
    0,    0,    0,    0,  835,    0,    0,  809,  808,    0,
    0,    0,    0,  808,    0,    0,    0,    0,    0,  807,
    0,  809,    0,  807,    0,  808,    0,  808,    0,  809,
    0,    0,  809,  809,    0,    0,    0,    0,    0,    0,
  844,    0,  809,  844,    0,  835,    0,    0,  835,    0,
  844,    0,  844,    0,    0,    0,  844,    0,  809,  838,
    0,  844,    0,  835,  844,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  808,    0,  833,    0,    0,  844,
  809,    0,    0,    0,    0,  809,    0,    0,    0,    0,
    0,    0,    0,  844,  808,    0,    0,  809,  808,  809,
    0,  844,    0,    0,  844,  844,    0,    0,    0,    0,
    0,    0,  838,    0,  844,  838,    0,  833,    0,  846,
  833,    0,  838,    0,  838,    0,    0,    0,  838,    0,
  844,    0,    0,  838,    0,  833,  838,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  809,    0,  839,    0,
    0,  838,  844,    0,    0,    0,    0,  844,    0,    0,
    0,    0,    0,    0,    0,  838,  809,    0,    0,  844,
  809,  844,  846,  838,    0,  846,  838,  838,    0,    0,
    0,    0,  846,    0,  846,    0,  838,    0,  846,  839,
    0,    0,  839,  846,  847,    0,  846,    0,    0,    0,
    0,    0,  838,    0,    0,    0,    0,  839,  848,    0,
    0,  846,    0,    0,    0,    0,    0,    0,  844,    0,
    0,    0,    0,    0,  838,  846,    0,    0,    0,  838,
    0,    0,    0,  846,    0,    0,  846,  846,  844,    0,
    0,  838,  844,  838,    0,    0,  846,  847,    0,  848,
  847,    0,  848,    0,    0,    0,    0,  847,    0,  847,
    0,    0,  846,  847,    0,    0,  835,  848,  847,    0,
    0,  847,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  834,  846,    0,  847,    0,    0,  846,
  838,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  847,  846,    0,  846,    0,    0,    0,    0,  847,    0,
  838,  847,  847,    0,  838,    0,    0,    0,    0,  835,
    0,  847,  835,    0,  834,    0,    0,  834,    0,  835,
    0,  835,    0,    0,    0,  835,    0,  847,  833,    0,
  835,    0,  834,  835,    0,    0,    0,    0,    0,    0,
  846,    0,    0,    0,    0,  836,    0,    0,  835,  847,
    0,    0,    0,    0,  847,    0,    0,    0,    0,    0,
  846,    0,  835,    0,  846,    0,  847,    0,  847,    0,
  835,    0,    0,  835,  835,    0,    0,    0,    0,    0,
    0,  833,    0,  835,  833,    0,  836,    0,    0,  836,
    0,  833,    0,  833,    0,    0,    0,  833,    0,  835,
  839,    0,  833,    0,  836,  833,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  847,    0,  840,    0,    0,
  833,  835,    0,    0,    0,    0,  835,    0,    0,    0,
    0,    0,    0,    0,  833,  847,    0,    0,  835,  847,
  835,    0,  833,    0,    0,  833,  833,    0,    0,    0,
    0,    0,    0,  839,    0,  833,  839,    0,  840,    0,
  848,  840,    0,  839,    0,  839,    0,    0,    0,  839,
    0,  833,    0,    0,  839,    0,  840,  839,    0,    0,
  679,    0,    0,    0,    0,    0,    0,  835,    0,    0,
    0,    0,  839,  833,    0,    0,    0,    0,  833,    0,
    0,    0,    0,    0,    0,    0,  839,  835,    0,    0,
  833,  835,  833,  848,  839,    0,  848,  839,  839,    0,
    0,  679,    0,  848,  679,  848,    0,  839,    0,  848,
    0,    0,    0,    0,  848,  834,    0,  848,    0,  679,
    0,    0,    0,  839,    0,    0,    0,    0,    0,    0,
    0,    0,  848,    0,    0,    0,    0,    0,    0,  833,
    0,    0,    0,    0,    0,  839,  848,    0,    0,    0,
  839,    0,    0,    0,  848,    0,    0,  848,  848,  833,
    0,    0,  839,  833,  839,    0,    0,  848,  834,    0,
    0,  834,    0,    0,    0,    0,    0,    0,  834,    0,
  834,    0,    0,  848,  834,    0,    0,  836,    0,  834,
    0,    0,  834,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  848,    0,  834,    0,    0,
  848,  839,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  834,  848,    0,  848,    0,    0,    0,    0,  834,
    0,  839,  834,  834,    0,  839,    0,    0,    0,    0,
  836,    0,  834,  836,    0,    0,    0,    0,    0,    0,
  836,    0,  836,    0,    0,    0,  836,    0,  834,  840,
    0,  836,    0,    0,  836,    0,    0,    0,    0,    0,
    0,  848,    0,    0,    0,    0,    0,    0,    0,  836,
  834,    0,    0,    0,  672,  834,    0,  441,    0,  442,
    0,  848,    0,  836,    0,  848,    0,  834,    0,  834,
    0,  836,  444,    0,  836,  836,    0,  295,    0,    0,
    0,    0,  840,    0,  836,  840,    0,    0,    0,    0,
    0,    0,  840,    0,  840,    0,    0,    0,  840,    0,
  836,    0,    0,  840,    0,    0,  840,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  834,    0,    0,    0,
    0,  840,  836,    0,    0,    0,    0,  836,    0,    0,
    0,    0,    0,    0,    0,  840,  834,    0,    0,  836,
  834,  836,    0,  840,    0,  679,  840,  840,  679,    0,
    0,    0,    0,    0,    0,  679,  840,  679,    0,    0,
    0,  679,    0,    0,    0,    0,  679,    0,    0,  679,
    0,    0,  840,    0,    0,    0, 1214,    0,    0,    0,
    0,    0,    0,    0,  679,    0,    0,    0,  836,    0,
    0,    0,    0,    0,  840,    0,    0,    0,  679,  840,
    0,    0,    0,    0,    6,    0,  679,  280,  836,    0,
  679,  840,  836,  840,    0, 1413, 1414,    0, 1415,  679,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1416,  679,  280,  280,    0,  280,
 1417,    0,    0,    0,    0,    0,  280,  280,    0,  280,
  280,    0,  280,    0,    0,  280,  280,  280,  280,  280,
  840,    0,  679,  280,  280,    0,    0,    0,    0,  280,
  280,  280,    0,    0,  679,    0,  679,    0,    0,    0,
  840,  404,    0,    0,  840,    0,    0,    0,  405,    0,
    0,    0,    0,    0,    0,    0,    0, 1418,  672,    0,
  406,  441,    0,  442,    0,    0,  407,  408,    0,    0,
    0,    0,    0,    0,    0,   14,  444,    0,    0,    0,
    0,  295,    0,  679,    0,  666,    0,    0,    0,    0,
 1419,    0,    0,    0,  409,    0,    0,  410,    0,    0,
   16,    0,    0,  679,    0,    0,    0,  679,    0, 1609,
    0,    0,    0,    0,    0,    0,    0,    0,  109,    0,
    0,    0,  411,    0,  412,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
    0,    0,    0,    0,    0,    0,  893, 1420,  111,    0,
  112,    0,    0,    0,    0,    0,    0,  113,  114,    0,
  115,  116,    0,  117,    0,    0,  118, 1421,  120,  121,
  122,    0,    0,    0,  123,  124, 1422, 1423, 1424,    0,
  125,  126,  127,   14,    0,    0,    0,  668,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,    0,  978,    0,  423,    0,    0,
    0,    0,    0,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,  894,
  895,  896,  123,  124,    0,  404,    0,    0,  125,  440,
  127,    0,  405,    0,    0,    0,  672,    0,    0,  441,
    0,  442,    0,    0,  406,  979,    0,  665,    0,    0,
  407,  408,    0,    0,  444,    0,    0,    0,    0,  295,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  666,
    0,    0,    0,    0,    0,    0,    0,    0,  409,    0,
  980,  410,    0,    0,    0,  981,    0,    0,    0,    0,
    0,  982,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  109,    0,    0,    0,  411,    0,  412,  413,
  983,    0,    0,    0,    0,    0,    0,    0,  414,    0,
    0,    0,  415,    0,    0,    0,    0,    0,    0,    0,
  667,  110,  111,    0,  112,    0,    0,    0,    0,    0,
    0,  113,  114,    0,  115,  116,    0,  117,  102,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,  668,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,  103,    0,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0,  104,    0,
    0,    0,    0,    0,    0,    0,    0,  419,  420,  421,
  422,  218,  219,    0,    0,    0,    0,    0,    0,    0,
    0,  423,  105,    0,    0,    0,    0,    0,    0,    0,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,    0,    0,  434,  435,    0,  436,  437,
  438,    0,  115,  116,  439,  117,    0,    0,  118,  119,
  669,  670,  671,  404,    0,    0,  123,  124,    0,    0,
  405,    0,  125,  440,  127,  672,    0,    0,  441,    0,
  442,    0,  406,    0,    0,    0,    0,  106,  407,  408,
    0,    0,    0,  444,    0,    0,    0,  107,  295,    0,
    0,    0,    0,    0,    0,    0,    0,  666,    0,    0,
    0,    0,    0,    0,    0,    0,  409,    0,    0,  410,
    0,    0,  108,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  109,    0,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,    0,    0,    0,    0,    0,    0,  889,  110,
  111,    0,  112,    0,    0,    0,    0,    0,    0,  113,
  114,    0,  115,  116,    0,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  126,  127,    0,    0,    0,    0,  668,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  669,  670,
  671,    0,  404,  882,  123,  124,  609,    0,    0,  405,
  125,  440,  127,  672,    0,    0,  441,    0,  442,  883,
    0,  406,    0,  610,    0,    0,  611,  407,  408,    0,
    0,  444,    0,    0,    0,    0,  295,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  666,    0,    0,    0,
    0,    0,    0,    0,    0,  409,    0,    0,  410,    0,
    0,    0,    0,    0,    0,    0,  612,    0,    0,    0,
    0,    0,  613,  614,  615,    0,  616,  617,  618,    0,
  619,    0,    0,  411,    0,  412,  413,    0,    0,    0,
    0,    0,    0,    0,    0,  414,    0,    0,  620,  415,
  621,    0,    0,    0,    0,    0,    0,  667,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  511,    0,    0,  441,    0,  442,    0,    0,    0,  622,
    0,    0,    0,    0,    0,    0,    0,    0,  444,    0,
    0,    0,    0,  295,    0,    0,  623,    0,  668,    0,
    0,    0,    0,    0,  416,    0,    0,    0,    0,    0,
  417,    0,    0,    0,    0,    0,    0,    0,  624,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  419,  420,  421,  422,  218,  219,
    0,    0,    0,    0,    0,    0,    0,    0,  423,    0,
    0,    0,    0,  625,  626,    0,    0,  424,  425,  426,
  427,  428,  429,  430,  431,  432,  110,  433,    0,  112,
  627,    0,  434,  435,    0,  436,  437,  438,    0,  115,
  116,  439,  117,    0,    0,  118,  119,  669,  670,  671,
  404,    0,    0,  123,  124,    0,    0,  405,    0,  125,
  440,  127,    0,    0,    0,    0,    0,    0,    0,  406,
    0,    0,    0,    0,    0,  407,  408,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  666,    0,    0,    0,    0,    0,
    0,    0,    0,  409,    0,    0,  410,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  411,    0,  412,  413,    0,    0,    0,    0,    0,
    0,    0,    0,  414,    0,    0,    0,  415,    0,    0,
 1137,    0,    0, 1138,    0,  889,    0,  404,    0,    0,
    0,    0,    0,    0,  405,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  406,    0,    0,    0,
    0,    0,  407,  408,    0,    0,    0,    0,  511,    0,
    0,  441,    0,  442,    0,    0,  668,    0,    0,    0,
    0,    0,  416,    0,    0,    0,  444,    0,  417,    0,
  409,  295,    0,  410,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  419,  420,  421,  422,  218,  219,  411,    0,
  412,  413,    0,    0,    0,    0,  423,    0,    0,    0,
  414,    0,    0,    0,  415,  424,  425,  426,  427,  428,
  429,  430,  431,  432,  110,  433,    0,  112,    0,    0,
  434,  435,    0,  436,  437,  438,    0,  115,  116,  439,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124, 1214,    0,    0,    0,  125,  440,  127,
    0,    0,    0,    0,    0,    0,    0, 1139,    0,  416,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
    0,    6,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0, 1413, 1414,    0, 1415,    0,    0,    0,  419,
  420,  421,  422,  218,  219,    0,    0,    0,    0,    0,
    0, 1416,    0,  423,    0,    0,    0, 1417,    0,    0,
    0,    0,  424,  425,  426,  427,  428,  429,  430,  431,
  432,  110,  433,    0,  112,    0,    0,  434,  435,    0,
  436,  437,  438,    0,  115,  116,  439,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  440,  127,    0, 1148,    0,
    0, 1138,    0,    0, 1418,  404,  608,    0,    0,  609,
    0,    0,  405,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   14,    0,  406,    0,  610,    0,    0,  611,
  407,  408,    0,    0,    0,    0,  511, 1419,    0,  441,
    0,  442,    0,    0,    0,    0,    0,   16,    0,    0,
    0,    0,    0,    0,  444,    0,    0,    0,  409,  295,
    0,  410,    0,    0,    0,  109,    0,    0,    0,  612,
    0,    0,    0,    0,    0,  613,  614,  615,    0,  616,
  617,  618,    0,  619,    0,    0,  411,    0,  412,  413,
    0,    0,    0,    0, 1420,  111,    0,  112,  414,    0,
    0,  620,  415,  621,  113,  114,    0,  115,  116,    0,
  117,    0,    0,  118, 1421,  120,  121,  122,    0,    0,
    0,  123,  124, 1422, 1423, 1424,    0,  125,  126,  127,
    0,    0,  622,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  511,    0,    0,  441,    0,  442,  623,
    0,    0,    0,    0,    0, 1139,    0,  416,    0,    0,
    0,  444,    0,  417,    0,    0,  295,    0,    0,    0,
    0,  624,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  419,  420,  421,
  422,  218,  219,    0,    0,    0,    0,    0,    0,    0,
    0,  423,    0,    0,    0,    0,  625,  626,    0,    0,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,  627,    0,  434,  435,    0,  436,  437,
  438,    0,  115,  116,  439,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  440,  127,    0, 1152,    0,    0, 1138,
    0,    0,    0,  404,  211,    0,    0,  211,    0,    0,
  405,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  211,    0,  406,    0,  211,    0,    0,  211,  407,  408,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  409,    0,    0,  410,
    0,    0,    0,    0,    0,    0,    0,  211,    0,    0,
    0,    0,    0,  211,  211,  211,    0,  211,  211,  211,
    0,  211,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,  211,
  415,  211,    0, 1160,    0,    0, 1138,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,  405,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  406,
  211,    0,    0,    0,    0,  407,  408,    0,    0,    0,
    0,  511,    0,    0,  441,    0,  442,  211,    0,    0,
    0,    0,    0, 1139,    0,  416,    0,    0,    0,  444,
    0,  417,    0,  409,  295,    0,  410,    0,    0,  211,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,  411,    0,  412,  413,    0,    0,    0,    0,  423,
    0,    0,    0,  414,  211,  211,    0,  415,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,  211,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    0,
  125,  440,  127,    0,    0,    0,    0,    0,    0,    0,
 1139,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  419,  420,  421,  422,  218,  219,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,    0,
    0,    0,    0,    0,    0,  424,  425,  426,  427,  428,
  429,  430,  431,  432,  110,  433,    0,  112,    0,    0,
  434,  435,    0,  436,  437,  438,    0,  115,  116,  439,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,    0,    0,    0,    0,  125,  440,  127,
    0, 1163,    0,    0, 1138,    0,    0,    0,  404,  141,
    0,    0,  141,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,  141,
    0,    0,  141,  407,  408,    0,    0,    0,    0,  511,
    0,    0,  441,    0,  442,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  444,    0,    0,
    0,  409,  295,    0,  410,    0,    0,    0,    0,    0,
    0,    0,  141,    0,    0,    0,    0,    0,  141,  141,
  141,    0,  141,  141,  141,    0,  141,    0,    0,  411,
    0,  412,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,  141,  415,  141,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  141,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  511,    0,    0,  441,
    0,  442,  141,    0,    0,    0,    0,    0, 1139,    0,
  416,    0,    0,    0,  444,    0,  417,    0,    0,  295,
    0,    0,    0,    0,  141,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,    0,  141,
  141,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,  141,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  440,  127,    0, 1168,
    0,    0, 1138,    0,    0,    0,  404,    0,    0,  538,
    0,    0,    0,  405,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  406,    0,    0,    0,    0,
    0,  407,  408,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  538,
  538,    0,    0,  538,    0,    0,    0,    0,    0,  409,
  988,    0,  410,  988,    0,    0,    0,    0,  538,    0,
  538,    0,    0,    0,    0,    0,    0,    0,  990,    0,
  988,    0,    0,  988,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
  538,    0,    0,  415,    0,    0, 1171,    0,    0, 1138,
    0,    0,    0,  404,    0,    0,    0,    0,    0,    0,
  405,    0,    0,  988,    0,    0,    0,    0,    0,  988,
  988,  988,  406,  988,  988,  988,    0,  988,  407,  408,
    0,    0,    0,    0,  511,    0,    0,  441,    0,  442,
    0,    0,    0,    0,    0,  988, 1139,  988,  416,    0,
    0,    0,  444,    0,  417,    0,  409,  295,    0,  410,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  988,    0,  419,  420,
  421,  422,  218,  219,  411,    0,  412,  413,    0,    0,
    0,    0,  423,  988,    0,    0,  414,    0,    0,    0,
  415,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,  988,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  440,  127,    0,    0,    0,    0,
  988,  988,    0, 1139,    0,  416,    0,  538,    0,    0,
    0,  417,    0,    0,    0,    0,    0,  988,    0,    0,
    0,  418,  538,    0,  538,  538,  538,    0,    0,    0,
  538,    0,    0,  538,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,  538,    0,    0,  423,
    0,    0,  538,    0,    0,  538,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,  538,    0,    0,
  125,  440,  127,    0, 1174,    0,    0, 1138,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,  405,    0,
    0,  538,  639,    0,  639,  639,    0,  639,    0,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
  639,    0,  538,    0,    0,  639,    0,    0,  538,    0,
    0,    0,  538,    0,    0,    0,    0,    0,    0,  538,
    0,    0,    0,    0,  409,    0,    0,  410,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  538,    0,    0,
    0,    0,  538,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,  412,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,  538,  415,    0,
    0,  538,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  633,
    0,  633,  633,    0,  633,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  633,    0,    0,
    0,    0,  633,    0,    0,    0,    0,    0,    0,    0,
    0, 1139,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,    0,    0,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,  639,    0,  118,  119,  120,  121,  122,  639,
    0,    0,  123,  124,    0,    0,  639,    0,  125,  440,
  127,    0,    0,    0,    0,    0,    0,    0,  639,    0,
    0,    0,    0,    0,  639,  639,    0,    0,    0,    0,
    0,    0,  639,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  639,    0,    0,  639,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  639,    0,  639,  639,    0,    0,    0,    0,    0,    0,
    0,    0,  639,    0,    0,    0,  639,    0,  511,  633,
  516,  441,    0,  442,    0,    0,  633,    0,    0,    0,
    0,    0,    0,  633,    0,    0,  444,    0,    0,    0,
    0,  295,    0,    0,    0,  633,    0,    0,    0,    0,
    0,  633,  633,    0,    0,    0,    0,    0,    0,  633,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  639,    0,    0,    0,    0,    0,  639,    0,  633,
    0,    0,  633,    0,    0,    0,    0,  639,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  639,  639,  639,  639,  639,  639,  633,    0,  633,
  633,    0,    0,    0,    0,  639,    0,    0,    0,  633,
    0,    0,    0,  633,  639,  639,  639,  639,  639,  639,
  639,  639,  639,  639,  639,    0,  639,    0,    0,  639,
  639,    0,  639,  639,  639,    0,  639,  639,  639,  639,
    0,    0,  639,  639,  639,  639,  639,    0,    0,    0,
  639,  639,    0,    0,    0,    0,  639,  639,  639,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  633,    0,
    0,    0,    0,    0,  633,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  633,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  633,  633,
  633,  633,  633,  633,    0,    0,    0,    0,    0,    0,
    0,    0,  633,    0,    0,    0,    0,    0,    0,    0,
    0,  633,  633,  633,  633,  633,  633,  633,  633,  633,
  633,  633,    0,  633,    0,  404,  633,  633,    0,  633,
  633,  633,  405,  633,  633,  633,  633,    0,    0,  633,
  633,  633,  633,  633,  406,    0,    0,  633,  633,    0,
  407,  408,    0,  633,  633,  633,  511,    0,    0,  441,
    0,  442,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  444,    0,    0,    0,  409,  295,
    0,  410,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  411,    0,  412,  413,
    0,    0,    0,    0,    0,    0,    0,    0,  414,    0,
    0,    0,  415,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  419,  420,  421,
  422,  218,  219,    0,    0,    0,    0,    0,    0,    0,
    0,  423,    0,    0,    0,    0,    0,    0,    0,    0,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,    0,    0,  434,  435,    0,  436,  437,
  438,    0,  115,  116,  439,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  440,  127,  511,    0,    0,  441,    0,
  442,    0,    0,  404,    0,    0,    0,    0,    0,    0,
  405,    0,    0,  444,    0,    0,    0,    0,  295,    0,
    0,    0,  406,    0,    0,    0,    0,    0,  407,  408,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  409,    0,    0,  410,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,  511,  955,    0,  441,    0,  442,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  444,    0,    0,    0,    0,  295,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,  702,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,  404,  434,  435,    0,  436,  437,  438,  405,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,  406,    0,    0,  123,  124,    0,  407,  408,    0,
  125,  440,  127,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  409,    0,    0,  410,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  411,    0,  412,  413,    0,    0,    0,
    0,    0,    0,    0,    0,  414,    0,    0,    0,  415,
    0,  511,    0,    0,  441,    0,  442,    0,    0,  404,
    0,    0,    0,    0,    0,    0,  405,    0,    0,  444,
    0,    0,    0,    0,  295,    0,    0,    0,  406,    0,
    0,    0,    0,    0,  407,  408,    0,    0,    0,    0,
    0,    0,    0,    0,   14,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  416,    0,    0,    0,    0,    0,
  417,    0,  409,    0,    0,  410,    0,    0,    0,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  419,  420,  421,  422,  218,  219,
  411,    0,  412,  413,    0,    0,    0,    0,  423,    0,
    0,    0,  414,    0,    0,    0,  415,  424,  425,  426,
  427,  428,  429,  430,  431,  432,  110,  433,    0,  112,
    0,    0,  434,  435,    0,  436,  437,  438,    0,  115,
  116,  439,  117,    0,    0,  118,  119,  120,  121,  122,
    0,    0,    0,  123,  124,    0,    0,    0,    0,  125,
  440,  127,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  416,    0,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  419,  420,  421,  422,  218,  219,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,    0,    0,
    0,    0,    0,    0,  424,  425,  426,  427,  428,  429,
  430,  431,  432,  110,  433,    0,  112,    0,  404,  434,
  435,    0,  436,  437,  438,  405,  115,  116,  439,  117,
    0,    0,  118,  119,  120,  121,  122,  406,    0,    0,
  123,  124,    0,  407,  408,    0,  125,  440,  127,  511,
    0,    0,  441,    0,  442,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  444,    0,    0,
    0,  409,  295,    0,  410,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  411,
    0,  412,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  511,    0,    0,  441,
    0,  442,    0,    0,    0,    0,    0,    0,    0,    0,
  416,    0,    0,    0,  444, 1704,  417,    0,    0,  295,
    0,    0,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1166,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,    0,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  440,  127,    0,    0,
    0,    0,    0,    0,    0,    0,  404,    0,    0,    0,
    0,    0,    0,  405,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  406,    0,    0,    0,    0,
    0,  407,  408,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  409,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
    0,    0,    0,  415,    0,  980,    0,  980,  980,    0,
  980,    0,    0,  404,    0,    0,    0,    0,    0,    0,
  405,    0,    0,  980,    0,    0,    0,    0,  980,    0,
    0,    0,  406,    0,    0,    0,    0,    0,  407,  408,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  416,    0,
    0,    0,    0,    0,  417,    0,  409,    0,    0,  410,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1369,    0,    0,  419,  420,
  421,  422,  218,  219,  411,    0,  412,  413,    0,    0,
    0,    0,  423,    0,    0,    0,  414,    0,    0,    0,
  415,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  440,  127,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,  980,  434,  435,    0,  436,  437,  438,  980,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,  980,    0,    0,  123,  124,    0,  980,  980,    0,
  125,  440,  127,  443,    0,    0,  441,    0,  442,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  444,    0,    0,    0,  980,  295,    0,  980,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  980,    0,  980,  980,    0,    0,    0,
    0,    0,    0,    0,    0,  980,    0,    0,    0,  980,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  511,    0,    0,  441,    0,  442,    0,    0,    0,    0,
    0,    0,    0,    0,  980,    0,    0,    0,  444,    0,
  980,    0,    0,  295,    0,    0,    0,    0,    0,    0,
  980,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  980,  980,  980,  980,  980,  980,
    0,    0,    0,    0,    0,    0,    0,    0,  980,    0,
    0,    0,    0,    0,    0,    0,    0,  980,  980,  980,
  980,  980,  980,  980,  980,  980,  980,  980,    0,  980,
    0,    0,  980,  980,    0,  980,  980,  980,    0,  980,
  980,  980,  980,    0,    0,  980,  980,  980,  980,  980,
    0,    0,    0,  980,  980,    0,    0,    0,    0,  980,
  980,  980,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,  405,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  406,
    0,    0,    0,    0,    0,  407,  408,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  409,    0,    0,  410,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  411,    0,  412,  413,    0,    0,    0,    0,    0,
    0,    0,    0,  414,    0,    0,    0,  415,    0,  980,
    0,    0,  980,    0,  980,    0,    0,  404,    0,    0,
    0,    0,    0,    0,  405,    0,    0,  980,    0,    0,
    0,    0,  980,    0,    0,    0,  406,    0,    0,    0,
    0,    0,  407,  408,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  416,    0,    0,    0,    0,    0,  417,    0,
  409,    0,    0,  410,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  419,  420,  421,  422,  218,  219,  411,    0,
  412,  413,    0,    0,    0,    0,  423,    0,    0,    0,
  414,    0,    0,    0,  415,  424,  425,  426,  427,  428,
  429,  430,  431,  432,  110,  433,    0,  112,    0,    0,
  434,  435,    0,  436,  437,  438,    0,  115,  116,  439,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,    0,    0,    0,    0,  125,  440,  127,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  416,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  419,
  420,  421,  422,  218,  219,    0,    0,    0,    0,    0,
    0,    0,    0,  423,    0,    0,  532,    0,    0,    0,
    0,    0,  424,  425,  426,  427,  428,  429,  430,  431,
  432,  110,  433,    0,  112,    0,  980,  434,  435,    0,
  436,  437,  438,  980,  115,  116,  439,  117,    0,    0,
  118,  119,  120,  121,  122,  980,    0,  532,  123,  124,
  532,  980,  980,    0,  125,  440,  127,    0,    0,  554,
    0,    0,    0,    0,    0,  532,    0,  532,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  980,
    0,    0,  980,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  544,    0,  532,    0,    0,
  554,    0,    0,  554,    0,    0,    0,  980,    0,  980,
  980,    0,    0,    0,    0,    0,    0,    0,  554,  980,
  554,    0,    0,  980,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  544,    0,  529,  544,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  554,    0,    0,    0,  544,    0,  544,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  980,  529,
    0,    0,  529,    0,  980,    0,  544,    0,    0,    0,
    0,    0,    0,    0,  980,    0,    0,  529,    0,  529,
    0,    0,    0,    0,    0,    0,    0,    0,  980,  980,
  980,  980,  980,  980,    0,    0,    0,    0,    0,    0,
    0,    0,  980,    0,    0,    0,    0,    0,    0,  529,
    0,  980,  980,  980,  980,  980,  980,  980,  980,  980,
  980,  980,    0,  980,    0,    0,  980,  980,    0,  980,
  980,  980,    0,  980,  980,  980,  980,    0,    0,  980,
  980,  980,  980,  980,  532,    0,    0,  980,  980,    0,
    0,    0,    0,  980,  980,  980,    0,    0,    0,  532,
    0,  532,  532,  532,    0,    0,    0,  532,    0,    0,
  532,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  532,    0,    0,    0,    0,    0,  532,
    0,    0,  532,    0,    0,    0,    0,  554,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  554,    0,    0,  554,  554,    0,    0,    0,
  554,    0,    0,  554,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  544,  532,    0,  554,    0,    0,    0,
    0,    0,  554,    0,    0,  554,    0,    0,  544,    0,
    0,  544,  544,    0,    0,    0,  544,    0,  532,  544,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  544,    0,    0,    0,  529,    0,  544,  532,
    0,  544,    0,    0,    0,  532,    0,  554,    0,  532,
    0,  529,    0,    0,  529,  529,  532,    0,    0,  529,
    0,    0,  529,    0,    0,    0,    0,    0,    0,    0,
    0,  554,    0,    0,  532,  529,    0,  320,    0,  532,
    0,  529,    0,  544,  529,    0,    0,    0,    0,    0,
    0,    0,  554,    0,    0,    0,    0,    0,  554,    0,
    0,    0,  554,    0,  532,  320,    0,  544,  532,  554,
    0,    0,  320,    0,    0,    0,  320,  320,    0,  320,
    0,    0,  320,    0,    0,    0,  529,  554,  544,    0,
    0,    0,  554,    0,  544,  320,    0,    0,  544,    0,
    0,  320,    0,    0,    0,  544,    0,    0,    0,  608,
  529,    0,  609,    0,    0,    0,    0,  554,  807,    0,
    0,  554,    0,  544,    0,    0,    0,    0,  544,  610,
    0,  529,  611,    0,    0,    0,    0,  529,    0,    0,
    0,  529,    0,    0,    0,    0,    0,    0,  529,    0,
    0,    0,    0,  544,    0,    0,    0,  544,  320,    0,
    0,    0,    0,    0,    0,    0,  529,    0,    0,    0,
    0,  529,  612,    0,    0,    0,  320,    0,  613,  614,
  615,    0,  616,  617,  618,    0,  619,    0,    0,    0,
    0,  320,    0,    0,    0,    0,  529,    0,    0,    0,
  529,  320,    0,    0,  620,    0,  621,    0,    0,    0,
  320,    0,  320,    0,    0,    0,    0,  608,    0,  320,
  609,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  622,    0,  610,    0,    0,
  611,    0,    0,    0,    0,    0,    0,    0,  320,  320,
    0,  320,  623,    0,    0,    0,    0,    0,  320,  320,
    0,  320,  320,    0,  320,    0,    0,  320,  320,  320,
  320,  320,    0,    0,  624,  320,  320,  320,  320,  320,
  612,  320,  320,  320,    0,    0,  613,  614,  615,  109,
  616,  617,  618,    0,  619,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  625,
  626,    0,  620,    0,  621,    0,    0,    0,  110,  111,
    0,  112,    0,    0,    0,    0,  627,    0,  113,  114,
    0,  115,  116,    0,  117,    0,    0,  118,  119,  120,
  121,  122,    0,  622,    0,  123,  124,    0,    0,    0,
    0,  125,  126,  127,    0,    0,    0,    0,    0,    0,
  623,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  624,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  109,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  625,  626,    0,
    0,    0,    0,    0,    0,    0,  110,  111,    0,  112,
    0,    0,    0,    0,  627,    0,  113,  114,    0,  115,
  116,    0,  117,    0,    0,  118,  119,  120,  121,  122,
    0,    0,    0,  123,  124,  109,    0,    0,    0,  125,
  126,  127,    0,    0,  172,  173,  174,  175,  176,  177,
  178,  179,  180,  181,  182,  183,  184,  185,  186,  187,
    0,    0,  188,  189,  110,  111,    0,  112,    0,    0,
  190,  191,    0,    0,  113,  114,  192,  115,  116,  193,
  117,  194,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,  195,  196,  197,  198,  125,  126,  127,
  199,  458,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,  458,    0,    0,  458,  458,
  458,  458,    0,  458,    0,    0,  458,  458,    0,    0,
  458,  458,  458,  458,  458,  458,  458,  458,    0,  458,
  458,  458,  458,  458,    0,    0,    0,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,
};
static short yycheck[] = {                                       9,
   58,    0,   12,   67,    4,   16,   16,   66,   18,  108,
  546,   59,  156,   65,  351,  146,  147,  382,  726,  146,
  147,   66,   70,   16,   88,  382,   94,  378,   97,  578,
   71,  479,   76,  522,   92,  549,   73,  353,  443,  374,
  974,   91, 1213,  713, 1047,   93,   56,   57,   58,   59,
   60,   61,  100,   94,   98, 1058,   66,   67,   68,   96,
   70,  280,   72,   73,  953,   75,   76,   60, 1058,   41,
  289,  779,  500,  733,   84,   86,   86,   87,   88,   72,
   90,   91,   92,   93,   95,   95,   96,   97,   98,  702,
  100,  160,  137,   86,  139,  708, 1047,  297,  499, 1286,
 1047,  151,  653,  660,  443,   44,  360, 1058,  326,  359,
  158, 1058,    0, 1087, 1110, 1007,  233,  876,   45,   63,
   44,  751,    0,  571,   40,  573,    0,  137,  138,  139,
  578,   44,    0,  387,   45,    0, 1054,   40,   40,    0,
  164,  151,   59,  242,   44,  138,  156,   63,  158,  159,
  160,  203,  786,   41,  164,    0,   44, 1231,    0,   44,
    0,   63, 1052, 1047,    0,   44,  159,  801,   44, 1059,
  804,   59,  511, 1047, 1058, 1047,  228,    0,    0,  813,
 1070,   59,  326,    0, 1058,   59, 1058, 1105,    0,    0,
   44,   59,  505,   44,   59,   44,   41,   41,   59,   44,
   41,   41,   44, 1093,   44,   41,  557,   45,   44,   44,
  327,   42,  297,  578,   59,    0,   47,   59,   41,   59,
 1480,  578,   41,   59,   41,   44,  271,   44,   91,  274,
   41,   40,  331,   44,  273,  869,   59,   59, 1536,   41,
  263,   40,   59,  283,  581,   41,   45,   59,   59,  667,
  308, 1422, 1423, 1245,  605,  351,    0, 1284,  344,  344,
 1195,  271,   45, 1198,  274,  298,  298,    0,  278,  344,
  280,  281,  491,  322,   59, 1278, 1279,  287,   12,  289,
   63,  326,  298,  380,  372,  447,  309,  541, 1278, 1279,
   41,    0,  629,    0,  273,   44,  317,  320,  308,    0,
  275,  334,  312,  916,  917,  393,  268,  352,   41, 1312,
   41,  399,  717,   44,  359,   59,  361,    0,  363,  312,
  725,  366, 1312,  375,  371,  377,   59, 1278, 1279,  300,
 1782, 1278, 1279,  672,  947,  432,  384,  395,  262,  391,
   41, 1259,  352,   44, 1318,  433,  272, 1281,  385,  359,
   59,  361,   59,  363, 1246,  499,  366,  315,   59,  989,
  315, 1312,    0, 1623,  853, 1312,  341,    0,  287,  348,
 1000, 1001,  498,  422,  384,  385,   59,  348,  453,  454,
  429,   59,  830,  393,    0,  395,    0, 1839,  484,  399,
   61, 1325,   93,  440, 1278, 1279,  419,  359, 1032,  399,
  393,  434,  434,   41, 1278, 1279, 1278, 1279,   41,    0,
  144, 1301,  275,  453,  454, 1049, 1050,  317,  434,  458,
  751,   59,  301,  509,  509,   41,   59,  315, 1312,  297,
  315,  264, 1603, 1046,  444, 1069,  297,   42, 1312,   61,
 1312,  754,  490,   59,   41,   59,  277,  301,  336,  877,
   41,  282,    0,   41,  499,  379, 1754,  505,  795, 1646,
  888,  798,  306, 1398,  892,  306,  262, 1402,   59,  596,
  315,    0,  809,   41, 1501,  893,  344,  487, 1470,    0,
  490,  491,  327,  344,  387,  402,  510,  445,  498,  428,
  445,  336,  502,   41,  487,  505,  361,  507,  315,  836,
  510,  546,    0,  539,  315,  498,  450,  559,  325,  502,
  327,   59,   41,  336,  325,  264,  327, 1130, 1131,  336,
 1133, 1134,  339,  450,  743,  336,  643,  376,  339,  597,
   59,  444,  410,  578,  450,  380,  546, 1533,   59,  450,
  376,  513,  273,   41,  389, 1619,   44,  599,  450,  361,
 1210, 1485,  802, 1104,  428,  264,  597,  757,  446, 1746,
 1117,   59,  780,  380,  880, 1324,  444,  577,  578,  380,
  403, 1545,  389,  583,  450,  306, 1112,  379,  389, 1007,
  402,  264,  315,  379,  403,  280,    0,  432,  507, 1481,
    0,  996,  325,  994,  327,  446, 1514,  649,  650,  730,
  344,  446,  444,  336, 1494, 1792, 1034,  652, 1498, 1499,
 1240,  446,  450,  867, 1532,  432,  401,  348,  334,    0,
    0,  432,    0,  446,  423,  513,  297,  444,  379,  446,
  439,   41, 1819,  444,  298,  446,  780,  344,  765,  477,
  423,  509,  652,  693, 1534,   59, 1581,  380,  509,   59,
  264,  450,  484,  493, 1267,  260,  389,  493,  989,  310,
   41,   41, 1551,   41,  816,  262,   41,  450,  513, 1000,
  334,  395,  403,    0,  262,  297, 1289,  315,   59,   59,
  315,   59,  315,  693,  516, 1355,  731, 1858,  733, 1323,
  735,  401,  264,    0,  262,  300,  513, 1700,  336,  432,
  261,  402,  513,  336,    0, 1040,  754,  390,  718,  395,
 1700,  444,  259,  446,   41,   12,  726,  319,  434, 1056,
  336,  731,  269,  733, 1658,  735,  762,  458,  738,  262,
 1664, 1068,   59,  743,  317,  780,  431, 1142,  259,  791,
  785,  751, 1632,  264,  754,  336, 1636,    0,  269, 1700,
  760,  761,   59, 1700,  272, 1092,    0,  802,  409,  978,
  828, 1651,  981,   59,  774, 1232,  776,  315, 1224,  779,
 1226, 1106,  457, 1287,  459,  785,  390,  325, 1668,  327,
  513,  774,  379,  776,  829,  830,  315,  828,  336,  841,
  514,  379,  802,    0,  835,  863,  864,  865,  327, 1115,
 1205,  304,  305, 1142,  262,    0,   59,  336,  446,  850,
  262,  379,    0,  446,  500,   59, 1700,  315,  336,  829,
  830, 1310,  863,  864,  865,  262, 1700, 1316, 1700,    0,
  446,  403,  380,  843,   41, 1384,   41,    0,  336, 1244,
 1307,  389,    0,  843,  258,  428,  379,  144,  397,    0,
  994,  380,   59,   41,  667,  446,  270,    0,  868,  672,
  389,    0,  333, 1753,   59, 1181, 1756,  271,  878,  390,
 1483,   59,  257,    0,    0,  868,  886,  887,  352,  450,
  513,  285,    0,  297,  432,  878,  886,  262,   59,  303,
   41,  305,  280,   44,  427,    0,   59,  368,  446,  448,
  450,   59,   41,  432,  319,  315, 1796,    0,   59,  380,
 1062, 1063,  386,  982,  388,   41,   59,  446,   44,    0,
   59,  379, 1433,    0,    0,  329,  336,  379,    0,   40,
  344,  979,   59,   59,  315,  315,  410,  315,  352,  983,
  443,   59,  379,  980,  325,  330,  327,  257,  446,  994,
 1224,    0, 1226,  357,   59,  336,  336,  264,  336, 1407,
 1112,  435, 1299, 1090,   41,  513,   59,  409,  978,  979,
  980,  981,  982,  983,   44,  427,  305, 1328,   59,  989,
  339, 1594,   59,   59,  513,    0,  315,   59,  315,  377,
 1000, 1001,   41, 1330,  436,   44,  494, 1007,  316,  380,
  327,  297,  380,  362,  379,  276,  305,  421,  389,  336,
   59,  389, 1468, 1352, 1353,  513,  315,  421,    0, 1384,
  330, 1360, 1361,   93, 1363, 1364,   41, 1384,  352,   44,
    0, 1099, 1371, 1372, 1373, 1374,  446, 1376, 1377, 1378,
 1379, 1380, 1381,  431,   59,  316, 1114,   41,  344,  394,
   44,  432, 1120,  380,  432,   44,  352,  262, 1099,  418,
  448,    0,  389, 1108, 1772,  446,  446, 1112,  446, 1079,
 1214,   41,  395, 1114,   44,  393,  889,   59,    0, 1120,
  893,  276,  277,  390, 1084,  338,    0, 1097,  260,   59,
  386,   40,  388,  264,  338,  509,    0,  421, 1108, 1230,
 1231, 1232, 1112,  513,   93,  432,  264,  430,  315,  300,
  371,  397, 1725,  376,  410,  433,  445, 1730,  371,  446,
   59,  264,  393,   93,  485,  276,  277,  315,  300,  336,
  416,  258,  513,  284,  495,  513, 1263,   59, 1265,  435,
  258,  265,    0,  270,  397,   59,  445,  400,  336,  410,
  276,  277,  270,  397, 1213,   59,  400,  348,  284,  264,
  297,    0,  433,  416,    0, 1210, 1503, 1490, 1213, 1214,
  297,  264,  416,  380,  379,  299,  303,  372,  305,  297,
  450,  372,  389,  264,   44,  303,  513,  305,  264,  442,
   42,   43,  380,   45,  376,   47,  457,  336,  442, 1326,
 1210,   59, 1539, 1213, 1214,   41,    0,  477,   44,  333,
    0,  348, 1535,  509, 1224,   44, 1226,  344,    0,  390,
   59,  372,    0,   59,  318,  432,  344,  276,  277,    0,
 1240,    0,  390,   93,  258,  284, 1246,  331,  315,  446,
   42,   43,  393,   45,  432,   47,  372,  390,  399, 1785,
  376,   41,   42,   43,   44,   45,   46,   47,  446,  336,
  354, 1606,   44,    0,  397,   59, 1777,  393,   44,   59,
   60,   61,   62,  399,   41,   42,   43,   59,   45,  303,
   47,   59,  433,  304,  305,  390,  265, 1339,   59, 1433,
   59, 1802, 1344,    0,  304,  305,  252,  390,  390,    0,
  315,   91,  376,  318,  260, 1305,  513,  433,  376,  390,
  325,  405,  327,    0,  390,  448,  331,   93,    0,  258,
  299,  336,   59,  372,  339,  513,    0, 1838, 1665, 1473,
  258,  270,  378,  261,   41,    0,  428,   44,    0,  354,
   41,    0,  493,   44,  393, 1856,  277,    0, 1859, 1413,
  399,  282,   59,  368,  276,  277,  338,    0,   59, 1870,
  318,  376,  276,  277,  303,  380,  305,  493,    0,  446,
  462,  428,   59,  331,  389,  303,   41,   59,    0,   44,
    0,  339,  509,  396,  433,   59,  410,    0, 1433,  371,
  405,  509,  318,    0,   59,  284,  354,   59,    0,  412,
   59,  486,    0, 1413,  428,  331,   59,    0,  297,  409,
  368, 1421, 1422, 1423,  333,  397,   59,  432,  400,  395,
 1765,  446,  443, 1433, 1769,  277, 1771,   59,  354,  444,
  282,  446,  402,  443,  416, 1780,  436,   59,  462,   59,
  276,  277,  368,  259,  493,    0,   59,  405,  284,    0,
  426,  260,   59,  269, 1799,    0,  260,   59, 1468,    0,
  442,   59, 1606, 1473,    0,  441,   59,    0,    0,   44,
 1480, 1481,  262,  270,    0,  277, 1620,  410,  493,  405,
  282,  271,  333,    0,   59, 1830,   41,  277, 1619,   44,
 1835,  300,  282,    0, 1504,  285,  300,  294,  513,  338,
   41,    0,  517,   44,   59,  260,  303,    0,   59,   41,
  277, 1504,   44,  260,   59,  282,  260,  368,   59,   42,
   43,  434,   45,   59,   47,  315,   59,   59,  318,  380,
  298,  321,  371,   59,  336,  325,  372,  327,  306,  329,
  337,  331,   59, 1591,  341,  300,  336,  337,   41,  339,
 1649,   44,   59,  300,   44,  262,  300,  393,  397,  517,
   59,  400,  258,  399,  354,  355,   59,  357,  513,   59,
  298,  273,   41,   46,  270,   44,  428,  416,  368,   40,
  370, 1591,  372,  373,  374,   58,  376,  453,   61,  379,
  380,  517, 1650, 1603,  416,   42,   43,  433,   45,  389,
   47,  338,   46,  442,  313,  314,  334,  303,  315,  305,
 1620,  318,   59, 1623,   58,  405, 1615,   61,  325, 1671,
  327, 1765,  264, 1675,  331, 1769,    0, 1771,  313,  336,
 1640,  421,  339, 1777,  371, 1645, 1780,  427,   41,  436,
 1650,   44,  432,    0,  441,  442,  443,  354, 1747,  449,
  450,  451, 1645, 1717,  444, 1799,  446,  493, 1802,   41,
  397,  368,   44,  400, 1249,   41, 1251,   41,   44,  376,
   44,  399,  379,  380,  338,  315,  340,  477,  478,  416,
  452,  409,  389,   40,   41,   59, 1830,   44,  453,  454,
   41, 1835, 1606,   44, 1838,  333,   42,   43,  405,   45,
  403,   47,   59,  493,  332,  442,  434, 1717, 1718, 1878,
   41, 1880, 1856,   44,  511, 1859,   46,  444,  515, 1719,
  427,  276,  277,  513,  395,  432, 1870,  517,   58,  284,
   41,   61, 1777,   44,   40,  276,  277,  444,  260,  446,
 1785,  336,  297,  284,  276,  277,  338,  317,   41,  281,
   40,   44,  284,  315,  277,   41,  297, 1802,   44,  282,
  395,   41, 1772, 1762,   44,  297,  376, 1777,   42,   43,
  376,   45, 1782,   47,  498, 1785,  298, 1788, 1788,  371,
  485,  486,   41,  276,  277,   44,  493,   41,  310, 1847,
   44,  284, 1802, 1838,  317,   42,   43,  428,   45,   41,
   47,  442,   44, 1814, 1814,  397,  513,  356,  400,  426,
  517, 1856,  334,  377, 1859,   42,   43,  372,   45,   41,
   47,  375,   44,   41,  416, 1870,   44,  497, 1838, 1839,
  277,  372,  409, 1844, 1844,  282,   41, 1847,  393,   44,
  372,   41,   42,   43,  399,   45, 1856,   47, 1858, 1859,
  442, 1765,  393, 1863,    0, 1769,   41, 1771,  399,   44,
 1870,  393,   41,  384,  385,   44, 1780,  399, 1878,   41,
 1880,  508,   44,   41,    0,  672,   44,  399,  433,  372,
  453,  454,  509,  508,  284, 1799,   44,  409,  262,  336,
  461,    0,  433,  690,   61,   41,  366,  694,   44,  283,
  393,  433,   42,   43,  701,   45,  399,   47,   40,  706,
  707,  264,  434,   59,  453,   41, 1830,   40,   44,   59,
  717, 1835,  719,  720,  721,  722,  723,  724,  725,   40,
   40,  277,   41,   59,   40,   44,  282,   40,   40, 1765,
  433,  315,   40, 1769,  318,   40,   40,   40,  453,  453,
   59,  325,   40,  327, 1780,   40,   40,  331,  315,   40,
   40,  318,  336,   91,   46,  339,  763,   44,  325,  315,
  327,   61,   40, 1799,  331,  264,  428,  317,   44,  336,
  354,  689,  339,  691,  395,  317,  783,  695,  696,  697,
   44,  281,  789,  790,  368,   44,  420,  354,   46,   61,
    0,   40,  376,  277, 1830,  379,  380,  264,  282, 1835,
   44,  368,  276,  276,  297,  389,  344,  344,  265,  376,
   41,   42,   43,  380,   45,  344,   47,  264,  390,  428,
  277,  405,  389,  462,  308,  282,  390,  450,  439,  275,
   40,   41,   42,   43,   44,   45,   46,   47,  405,  344,
  277,  439,  299,  427,  392,  282,   40,  345,  432,   59,
   60,   61,   62,   40,   91,   41,   42,   43,   40,   45,
  444,   47,  446,   40,  315,  432,  264,  277,  376,  403,
   44,  308,  282,  382,  375,   40,   40,  444,  443,  446,
   63,   91,   41,   93,   41,   42,   43,  275,   45,  296,
   47,  314,  428,   40,  901,  902,  903,  456,  317,  906,
  907,  908,  264,  910,  911,  912,  913,  914,  915,  493,
  440,  918,  919,  317,  921,   44,  923,  444,  494,  926,
  927,  928,  929,  930,  931,   40,  493,  277,  273,  513,
   40,  297,  282,  517,  281,  340,  943,  340,  945,  276,
  276,  277,  410,  379,  379,  281,  513,  408,  284,  379,
  517,  297,  452,   40,  410,  352,   40,  276,  277,  315,
   40,  297,  318,   41,  317,  284,   41,   42,   43,  325,
   45,  327,   47,   44,  270,  331,  277,   44,  438,  378,
  336,  481,  482,  339,  484,  361,    0,   44,  317,  996,
   40,  491,  492,   44,  494,  495,  266,  497,  354,  325,
  500,  501,  502,  503,  504,   40,  493,   40,  508,  509,
  264,   41,  368,  414,  514,  515,  516,   58,    0,   41,
  376,   41,   41,  379,  380,    0,  264,   41,  446,  387,
   44,  306,  306,  389,   41,   41,  372,   41,   42,   43,
   40,   45,   40,   47,  317,   59,  277,   40,   40,  405,
   40,  282,  262,  372,  264,  265,   40,  393,   40,   41,
   40,  271,   44,  399,   40,  262,   41,  277,   40,   44,
   40,  427,  282,   40,  393,  285,  432,   59,   40,   40,
  399,   40,   40,   40,   59,  305,  427,   41,  444,  299,
  446,  277,  324,   44,  304,  305,  282,  433,  308,   42,
   43,   44,   45,   41,   47,  315,  264,  317,  318,  339,
  381,  321,    0,  339,  433,  325,  327,  327,  273,  329,
  277,  331,  384,  379,  284,  282,  336,  337,  379,  339,
  399,  379,  379,  379,  379, 1142,   41,  493,   42,   43,
  450,   45,   41,   47,  354,  355,  276,  357, 1155, 1156,
  372, 1158, 1159,   41,   40,  317,   44,  513,  368, 1166,
  370,  517,  372,  373,  374,   41,  376,   41,   41,  379,
  380,   59,   41,  305,  427,   59,  290,  318, 1185,  389,
   59,   40,  336,  336,   59,  305, 1193, 1194, 1195,   59,
  331, 1198,  402, 1200,   59,  405,   59, 1204, 1205, 1206,
  378,   59,  277,   59,   41,   42,   43,  282,   45,   59,
   47,  421,   58,  354,  428,  339,  389,  427,  428,   41,
   42,   43,  432,   45,  277,   47,  333,  368,  434,  334,
  434,  334,  298,  443,  444,  376,  446, 1244,  298,   41,
   41,  378,  452,   41,   42,   43,  348,   45,  365,   47,
   41,   41,  365,   41,   42,   43,   41,   45,   41,   47,
   41,   41,  276,  277,  405,   41,   41,   41,   41,   41,
   41,  481,  482,  277,  484, 1282,   41,   41,  282,   41,
   41,  491,  492,  493,  494,  495,   41,  497,  315,   59,
  500,  501,  502,  503,  504,  443,   40,   44,  508,  509,
   41,  276,  277,  513,  514,  515,  516,  517,  348,   41,
   42,   43,  340,   45,  458,   47,   40,   59,  336,   41,
   42,   43,    0,   45,  500,   47,   41,   42,   43,   46,
   45,  336,   47,  315,  277,  427,  318,   42,   43,  282,
   45,   40,   47,  325, 1351,  327,   40,  362,   40,  331,
 1357,  376,  493,  402,  336, 1362,   41,  339,  372,  376,
  264,   41, 1369,   41,   42,   43,   44,   45,   46,   47,
   59,   44,  354,  277,  262,  376,  517,  301,  282,  393,
   41,   59,   60,   61,   62,  399,  368,   44,  298,  434,
  304, 1398,  290,  301,  376, 1402,   40,  372,  380,  455,
   40,   42,   43,   44,   45,  333,   47,  389,   44,   41,
   40, 1418,   41,   91,    0,   93,  333,   40,  393,  433,
  376,  376,  409,  405,  399,  264,  264,  315,  276,  450,
  318,  462,  317,  443,  403,   44,  270,  325,   40,  327,
  277,   41,  270,  331,  264,  282,  427,  450,  336,  428,
  432,  339,  339,  450,   41,  277,  339,   41,  433,   40,
  282,   59,  444,   59,  446,  305,  354,   41,   40,   59,
  452,   59,   41,   42,   43, 1482,   45,  301,   47,  277,
  368,   41,   59,   41,  282,    0,    0,    0,  376,  277,
 1227,  379,  380,   12,  282,  289,  291, 1117,  282,  481,
  482,  389,  484,  281,  743,  656,  881,  302, 1115,  491,
  492,  493,  494,  495,  309,  497,  886,  405,  500,  501,
  502,  503,  504,   90, 1112,  843,  508,  509, 1107, 1522,
  349,  513,  514,  515,  516,  517, 1523,  379,  333,  427,
  605, 1043, 1097,  329,  432,  277, 1108, 1554,  361,  785,
  282,  361, 1586, 1560, 1561,  277,  444, 1640,  446,  264,
  282,  811,  277, 1790, 1503, 1501, 1794,  282, 1821, 1793,
 1577,  583,  277,  802, 1581,  975, 1706,  282, 1585,   40,
 1611, 1421, 1778, 1590,  262, 1592,  264,  265,  731, 1261,
 1486,  354,  860,  271,  585,  347, 1314,   58,  366,  277,
  395,   13,  316,  642,  282,  493,  608,  285,  524,   41,
   42,   43,  852,   45,  149,   47,  528,  763,  774, 1250,
   40,  299, 1007,  301,    0,  513,  304,  305,   86,  517,
  308,  426, 1735, 1630,  750,  430,  277,  315, 1844,  317,
  318,  282, 1814,  321,   41,   42,   43,  325,   45,  327,
   47,  329, 1269,  331,  789, 1200,   -1,  539,  336,  337,
 1157,  339,  997, 1670,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376, 1706,
   -1,  379,  380,   42,   -1,   -1,   -1,   -1,  277,   -1,
   -1,  389,   -1,  282,  906,   91,  908,   93,  910,  911,
   -1,  913,  914,  915,  402,   -1,   -1,  405, 1735,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   -1,
   45,   -1,   47,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,  409,
   42,   43, 1779,   45,  452,   47,   -1,   -1,   -1,   42,
   43,   -1,   45,   -1,   47,  464,  465,  466,  467,  468,
  469,  470,  471,   -1,   -1,   -1,   -1,   -1,  302,   -1,
   -1,   -1,   -1,  481,  482,  309,  484,   -1,   -1,  313,
  314,   -1,  452,  491,  492,  493,  494,  495,  322,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,  333,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
   -1,  481,  482,   -1,  484,  277,   -1,   -1,   -1,   -1,
  282,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
  277,   42,   -1,   -1,   -1,  282,  262,   -1,  264,  265,
   -1,  395,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,  411,   -1,  285,
   -1,   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,  426,  299,    0,   -1,  430,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,  441,   58,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,  293,  331,  460,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,  277,   -1,   -1,  262,   -1,  282,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
  277,  452,  368,   -1,  370,  282,  372,  373,  374,  262,
  376,   -1,   -1,  379,  380,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  389,  277,   91,   -1,   93,   -1,  282,
  481,  482,   -1,  484,   -1,   -1,  402,   59,   -1,  405,
  491,  492,  452,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,  452,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  481,  482,   -1,  484,  277,   -1,   -1,   -1,
   -1,  282,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   44,   -1,   -1,  262,   -1,  264,  265,
   -1, 1352, 1353,   -1,   -1,  271,   -1,   -1,   -1, 1360,
 1361,  277, 1363, 1364,   -1,   -1,  282,   -1,   -1,  285,
 1371, 1372, 1373, 1374,   -1, 1376, 1377, 1378, 1379, 1380,
 1381,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,  452,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
  452,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,  317,   -1,   -1,  491,
  492,   -1,  494,  495,  284,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   59,   -1,  405,
  491,  492,  452,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  450,   -1,  452,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,  258,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,  291,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,  302,   59,   -1,   61,   -1,  354,  355,
  309,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,  322,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,  408,   -1,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,
  379,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  395,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,
   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,   -1,
   -1,  430,   -1,   -1,  433,  481,  482,   -1,  484,   -1,
   -1,   -1,  441,   -1,   -1,  491,  492,  493,  494,  495,
  452,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,  460,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,  258,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  262,   -1,  264,  265,
  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,  284,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,  452,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,  258,   -1,   -1,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,  426,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   61,   -1,   -1,  481,  482,    0,  484,   -1,
   -1,   -1,   -1,  452,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  481,  482,   -1,  484,   -1,   40,   41,   -1,
   -1,   44,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   59,   -1,   61,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,  258,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,  273,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,   45,
   -1,   -1,   -1,   -1,  303,  421,   -1,  306,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
  273,   -1,  275,  276,  277,   -1,   -1,   -1,  281,  348,
   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  297,  481,  482,   -1,  484,   -1,
  303,   -1,   -1,  306,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,  458,
  393,   -1,   -1,  462,   -1,  325,  399,   -1,   -1,  285,
  403,   -1,   -1,    0,   -1,   -1,   -1,  410,   -1,   -1,
   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,  428,   -1,   -1,   -1,  315,
  433,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   41,  331,   43,   44,   45,   -1,
  336,  337,   -1,  339,   -1,  458,   -1,   -1,   -1,  462,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,  292,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,  389,  320,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,  419,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,  435,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,  449,  450,  451,   -1,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  261,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  262,   -1,  264,  265,   -1,
  279,  487,  488,   -1,  271,   -1,   -1,   -1,  287,   -1,
  496,   -1,   -1,   -1,   -1,  294,   -1,  296,  285,  298,
   -1,   -1,    0,   -1,  303,   -1,   -1,   -1,   -1,   -1,
   -1,  310,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,  324,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,  334,   -1,   -1,  325,   -1,
  327,   -1,  329,   41,  331,   43,   44,   45,   -1,  336,
  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   93,   -1,   -1,  397,   -1,
   -1,   -1,  389,   -1,   -1,  404,   -1,  339,  407,   -1,
  409,  410,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  362,   -1,   -1,   -1,  421,  434,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  483,   -1,  418,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  498,
  499,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
  452,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,   -1,   -1,   -1,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   60,   61,   62,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,  262,  497,  264,  265,  500,  501,
  502,  503,  504,  271,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,  285,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,  389,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  402,  494,  495,  405,  497,   -1,
  452,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,  421,   -1,  514,  515,  516,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,  443,  444,   -1,  446,  491,
  492,   -1,  494,  495,  452,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,  271,   -1,  508,  509,   -1,   -1,
  277,   -1,  514,  515,  516,  282,   -1,   -1,  285,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,  321,  513,  514,  515,  516,  517,
   -1,   -1,  329,   -1,   42,   43,   -1,   45,   -1,   47,
  337,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   60,   61,   62,   -1,   -1,   -1,  355,   -1,
  357,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,  271,  370,   -1,  372,  373,  374,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,  421,   -1,  325,   -1,  327,   -1,
  329,   41,  331,   43,   44,   45,   -1,  336,  337,   -1,
  339,  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,  354,  355,  288,  357,   -1,
   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,  335,   -1,  405,   -1,   -1,   -1,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,  367,   -1,  369,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,  398,   -1,  277,
   -1,   -1,  403,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,  481,  482,  415,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,  437,   -1,   -1,  508,
  509,   -1,   -1,  321,  513,  514,  515,  516,  517,   -1,
   -1,  329,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  337,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,  472,  473,   -1,   -1,   -1,   -1,  355,   -1,  357,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  489,   -1,
   -1,  271,  370,   -1,  372,  373,  374,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,  299,
   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   59,
   60,   61,   62,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,  421,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,
   -1,   91,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,  389,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  402,   -1,   -1,  405,   59,   -1,  452,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,  443,  444,   -1,  446,  491,  492,   -1,
  494,  495,  452,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,    0,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,  262,  513,  514,  515,  516,  517,   -1,   -1,
   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,
   91,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,  405,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,    0,
   -1,  421,  325,   -1,  327,   -1,   -1,  427,  331,   -1,
   -1,   -1,  432,  336,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,
  271,   -1,   -1,   -1,  427,   -1,  277,   -1,   -1,  432,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,  264,   -1,
  513,   -1,   -1,   -1,  517,   -1,   -1,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,  315,
   -1,   -1,  318,    0,   -1,   -1,   -1,   -1,   -1,  325,
  421,  327,   -1,   -1,   41,  331,  427,   44,   -1,   -1,
  336,  432,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  444,   -1,  446,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,   59,   -1,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,  493,   -1,  325,   -1,  327,   -1,   -1,  405,
  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,
   -1,   -1,   -1,  354,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,  444,   -1,
  446,   -1,   -1,   -1,   -1,  376,  452,   -1,   -1,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  405,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,    0,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,  432,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
  260,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,  262,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,    0,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,  298,  500,
  501,  502,  503,  504,   -1,  262,   -1,  508,  509,   -1,
  310,   -1,  513,  514,  515,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  324,   -1,   -1,   41,  315,   -1,
   44,  318,   -1,   -1,  334,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   59,    0,   -1,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,  354,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,  368,  339,   -1,   -1,   -1,   -1,   41,   -1,  376,
   44,   -1,  379,  380,   -1,   -1,   -1,  354,   -1,  399,
   -1,   -1,  389,   -1,   -1,   59,   -1,   -1,   -1,  409,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,  405,  376,
   -1,    0,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,  434,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,  405,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,  262,   -1,   -1,   -1,
   59,  481,  482,   -1,  484,   -1,   -1,  444,   -1,  446,
   -1,  491,  492,    0,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,  493,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,  315,
  517,   -1,  318,   -1,   41,   -1,  493,   44,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,  262,   -1,
  336,   -1,   59,  339,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,   -1,   -1,   -1,    0,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,  389,  318,   -1,   41,   -1,  262,   44,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  405,
   -1,   -1,  336,   -1,   59,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
  354,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,  444,   -1,
  446,  315,  376,   -1,  318,  379,  380,   -1,   -1,   -1,
   -1,  325,   -1,  327,   -1,  389,   -1,  331,   41,   -1,
   -1,   44,  336,  262,   -1,  339,   -1,   -1,   -1,   -1,
   -1,  405,   -1,   -1,   -1,   -1,   59,    0,   -1,   -1,
  354,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,   -1,  427,  368,   -1,   -1,   -1,  432,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,  513,   -1,   -1,
  444,  517,  446,   -1,   -1,  389,  315,   -1,   41,  318,
   -1,   44,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
   -1,  405,  331,   -1,   -1,  262,   59,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,  427,   -1,  354,   -1,   -1,  432,  493,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
  444,   -1,  446,   -1,   -1,   -1,   -1,  376,   -1,  513,
  379,  380,   -1,  517,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   41,   -1,   -1,   44,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,  262,   -1,  336,
   -1,   59,  339,   -1,   -1,   -1,   -1,   -1,   -1,  493,
   -1,   -1,   -1,   -1,    0,   -1,   -1,  354,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,  513,
   -1,  368,   -1,  517,   -1,  444,   -1,  446,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  389,  318,   -1,   41,   -1,   -1,   44,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  405,  262,
   -1,  336,   -1,   59,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,    0,   -1,   -1,  354,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,  513,   -1,   -1,  444,  517,  446,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  389,  318,   -1,   41,   -1,  262,
   44,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
  405,   -1,   -1,  336,   -1,   59,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,    0,   -1,
   -1,  354,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,  513,   -1,   -1,  444,
  517,  446,  315,  376,   -1,  318,  379,  380,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,  389,   -1,  331,   41,
   -1,   -1,   44,  336,  262,   -1,  339,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,   -1,   59,    0,   -1,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,   -1,  427,  368,   -1,   -1,   -1,  432,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,  513,   -1,
   -1,  444,  517,  446,   -1,   -1,  389,  315,   -1,   41,
  318,   -1,   44,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,  405,  331,   -1,   -1,  262,   59,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  427,   -1,  354,   -1,   -1,  432,
  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,  444,   -1,  446,   -1,   -1,   -1,   -1,  376,   -1,
  513,  379,  380,   -1,  517,   -1,   -1,   -1,   -1,  315,
   -1,  389,  318,   -1,   41,   -1,   -1,   44,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  405,  262,   -1,
  336,   -1,   59,  339,   -1,   -1,   -1,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   -1,    0,   -1,   -1,  354,  427,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
  513,   -1,  368,   -1,  517,   -1,  444,   -1,  446,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,  389,  318,   -1,   41,   -1,   -1,   44,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  405,
  262,   -1,  336,   -1,   59,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,    0,   -1,   -1,
  354,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,  513,   -1,   -1,  444,  517,
  446,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  389,  318,   -1,   41,   -1,
  262,   44,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,  405,   -1,   -1,  336,   -1,   59,  339,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,  513,   -1,   -1,
  444,  517,  446,  315,  376,   -1,  318,  379,  380,   -1,
   -1,   41,   -1,  325,   44,  327,   -1,  389,   -1,  331,
   -1,   -1,   -1,   -1,  336,  262,   -1,  339,   -1,   59,
   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,  493,
   -1,   -1,   -1,   -1,   -1,  427,  368,   -1,   -1,   -1,
  432,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,  513,
   -1,   -1,  444,  517,  446,   -1,   -1,  389,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,  405,  331,   -1,   -1,  262,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,  354,   -1,   -1,
  432,  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,  444,   -1,  446,   -1,   -1,   -1,   -1,  376,
   -1,  513,  379,  380,   -1,  517,   -1,   -1,   -1,   -1,
  315,   -1,  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  405,  262,
   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  427,   -1,   -1,   -1,   40,  432,   -1,   43,   -1,   45,
   -1,  513,   -1,  368,   -1,  517,   -1,  444,   -1,  446,
   -1,  376,   58,   -1,  379,  380,   -1,   63,   -1,   -1,
   -1,   -1,  315,   -1,  389,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
  405,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,
   -1,  354,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,  513,   -1,   -1,  444,
  517,  446,   -1,  376,   -1,  315,  379,  380,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,  389,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,  405,   -1,   -1,   -1,  270,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,  368,  432,
   -1,   -1,   -1,   -1,  298,   -1,  376,  452,  513,   -1,
  380,  444,  517,  446,   -1,  309,  310,   -1,  312,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  328,  405,  481,  482,   -1,  484,
  334,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
  493,   -1,  432,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,
  513,  267,   -1,   -1,  517,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  391,   40,   -1,
  286,   43,   -1,   45,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  409,   58,   -1,   -1,   -1,
   -1,   63,   -1,  493,   -1,  311,   -1,   -1,   -1,   -1,
  424,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
  434,   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  372,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  514,  515,  516,  409,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  505,
  506,  507,  508,  509,   -1,  267,   -1,   -1,  514,  515,
  516,   -1,  274,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  286,  395,   -1,  289,   -1,   -1,
  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
  430,  323,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,  348,   -1,  350,  351,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  372,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,  260,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,  298,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  310,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,  334,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,
  274,   -1,  514,  515,  516,   40,   -1,   -1,   43,   -1,
   45,   -1,  286,   -1,   -1,   -1,   -1,  399,  292,  293,
   -1,   -1,   -1,   58,   -1,   -1,   -1,  409,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,  267,  272,  508,  509,  275,   -1,   -1,  274,
  514,  515,  516,   40,   -1,   -1,   43,   -1,   45,  288,
   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,  364,
  369,   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,  398,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,  415,   -1,  413,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,  472,  473,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
  260,   -1,   -1,  263,   -1,  372,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   58,   -1,  425,   -1,
  320,   63,   -1,  323,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  270,   -1,   -1,   -1,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,  298,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,  309,  310,   -1,  312,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,  463,   -1,   -1,   -1,  334,   -1,   -1,
   -1,   -1,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  260,   -1,
   -1,  263,   -1,   -1,  391,  267,  272,   -1,   -1,  275,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  409,   -1,  286,   -1,  292,   -1,   -1,  295,
  292,  293,   -1,   -1,   -1,   -1,   40,  424,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  320,   63,
   -1,  323,   -1,   -1,   -1,  452,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,  360,   -1,
   -1,  367,  364,  369,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,  415,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,
   -1,   58,   -1,  425,   -1,   -1,   63,   -1,   -1,   -1,
   -1,  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  288,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,
  364,  369,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
  398,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,  415,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   58,
   -1,  425,   -1,  320,   63,   -1,  323,   -1,   -1,  437,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,  360,  472,  473,   -1,  364,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,  272,
   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,  292,
   -1,   -1,  295,  292,  293,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,  320,   63,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,  367,  364,  369,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,  415,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   58,   -1,  425,   -1,   -1,   63,
   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,  472,
  473,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,  489,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,    0,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,  320,
  272,   -1,  323,  275,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,
  292,   -1,   -1,  295,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   91,   -1,   -1,  364,   -1,   -1,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,
  342,  343,  286,  345,  346,  347,   -1,  349,  292,  293,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,  367,  417,  369,  419,   -1,
   -1,   -1,   58,   -1,  425,   -1,  320,   63,   -1,  323,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,  449,  450,
  451,  452,  453,  454,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,  463,  415,   -1,   -1,  360,   -1,   -1,   -1,
  364,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,  437,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
  472,  473,   -1,  417,   -1,  419,   -1,  258,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,  489,   -1,   -1,
   -1,  435,  273,   -1,  275,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,  463,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,  348,   -1,   -1,
  514,  515,  516,   -1,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,  372,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   58,   -1,  393,   -1,   -1,   63,   -1,   -1,  399,   -1,
   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,  458,  364,   -1,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,  260,   -1,  500,  501,  502,  503,  504,  267,
   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   40,  260,
   42,   43,   -1,   45,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,  300,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,  267,  487,  488,   -1,  490,
  491,  492,  274,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  286,   -1,   -1,  508,  509,   -1,
  292,  293,   -1,  514,  515,  516,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  320,   63,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   40,   41,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,  267,  487,  488,   -1,  490,  491,  492,  274,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  286,   -1,   -1,  508,  509,   -1,  292,  293,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,  267,  487,
  488,   -1,  490,  491,  492,  274,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,  286,   -1,   -1,
  508,  509,   -1,  292,  293,   -1,  514,  515,  516,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,  320,   63,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   58,   59,  425,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,  450,
  451,  452,  453,  454,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,  267,  487,  488,   -1,  490,  491,  492,  274,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  286,   -1,   -1,  508,  509,   -1,  292,  293,   -1,
  514,  515,  516,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,  320,   63,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   58,   -1,
  425,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,  267,  487,  488,   -1,
  490,  491,  492,  274,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  286,   -1,   41,  508,  509,
   44,  292,  293,   -1,  514,  515,  516,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   91,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,  360,
   61,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,    0,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   41,
   -1,   -1,   44,   -1,  425,   -1,   91,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   91,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  258,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,  273,
   -1,  275,  276,  277,   -1,   -1,   -1,  281,   -1,   -1,
  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,
   -1,   -1,  306,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,  348,   -1,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,  273,   -1,
   -1,  276,  277,   -1,   -1,   -1,  281,   -1,  372,  284,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  297,   -1,   -1,   -1,  258,   -1,  303,  393,
   -1,  306,   -1,   -1,   -1,  399,   -1,  348,   -1,  403,
   -1,  273,   -1,   -1,  276,  277,  410,   -1,   -1,  281,
   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  372,   -1,   -1,  428,  297,   -1,  270,   -1,  433,
   -1,  303,   -1,  348,  306,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  393,   -1,   -1,   -1,   -1,   -1,  399,   -1,
   -1,   -1,  403,   -1,  458,  298,   -1,  372,  462,  410,
   -1,   -1,  305,   -1,   -1,   -1,  309,  310,   -1,  312,
   -1,   -1,  315,   -1,   -1,   -1,  348,  428,  393,   -1,
   -1,   -1,  433,   -1,  399,  328,   -1,   -1,  403,   -1,
   -1,  334,   -1,   -1,   -1,  410,   -1,   -1,   -1,  272,
  372,   -1,  275,   -1,   -1,   -1,   -1,  458,  281,   -1,
   -1,  462,   -1,  428,   -1,   -1,   -1,   -1,  433,  292,
   -1,  393,  295,   -1,   -1,   -1,   -1,  399,   -1,   -1,
   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,   -1,   -1,  458,   -1,   -1,   -1,  462,  391,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,
   -1,  433,  335,   -1,   -1,   -1,  409,   -1,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
   -1,  424,   -1,   -1,   -1,   -1,  458,   -1,   -1,   -1,
  462,  434,   -1,   -1,  367,   -1,  369,   -1,   -1,   -1,
  443,   -1,  445,   -1,   -1,   -1,   -1,  272,   -1,  452,
  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,   -1,  292,   -1,   -1,
  295,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,  415,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,  437,  508,  509,  510,  511,  512,
  335,  514,  515,  516,   -1,   -1,  341,  342,  343,  452,
  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,   -1,  367,   -1,  369,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,  489,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  398,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
  415,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,  489,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,  452,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,  461,  462,  463,  464,  465,  466,
  467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
   -1,   -1,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,   -1,  491,  492,  493,  494,  495,  496,
  497,  498,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,  513,  514,  515,  516,
  517,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  462,  463,  464,  465,  466,  467,  468,  469,  470,
  471,  472,  473,  474,  475,  476,   -1,   -1,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,   -1,
  491,  492,  493,  494,  495,  496,  497,  498,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,  517,
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
"AS","ASC","AT","AVG","AUTO","BEFORE","BEGIN","BETWEEN","BLOB","BY","CAST",
"CHARACTER","CHECK","COLLATE","COMMA","COMMIT","COMMITTED","COMPUTED",
"CONCATENATE","CONDITIONAL","CONSTRAINT","CONTAINING","COUNT","CREATE",
"CSTRING","CURRENT","CURSOR","DATABASE","DATE","DB_KEY","KW_DEBUG","DECIMAL",
"DECLARE","DEFAULT","KW_DELETE","DESC","DISTINCT","DO","DOMAIN","DROP","ELSE",
"END","ENTRY_POINT","EQL","ESCAPE","EXCEPTION","EXECUTE","EXISTS","EXIT",
"EXTERNAL","FILTER","FOR","FOREIGN","FROM","FULL","FUNCTION","GDSCODE","GEQ",
"GENERATOR","GEN_ID","GRANT","GROUP","GTR","HAVING","IF","KW_IN","INACTIVE",
"INNER","INPUT_TYPE","INDEX","INSERT","INTEGER","INTO","IS","ISOLATION","JOIN",
"KEY","KW_CHAR","KW_DEC","KW_DOUBLE","KW_FILE","KW_FLOAT","KW_INT","KW_LONG",
"KW_NULL","KW_NUMERIC","KW_UPPER","KW_VALUE","LENGTH","LPAREN","LEFT","LEQ",
"LEVEL","LIKE","LSS","MANUAL","MAXIMUM","MAX_SEGMENT","MERGE","MESSAGE",
"MINIMUM","MODULE_NAME","NAMES","NATIONAL","NATURAL","NCHAR","NEQ","NO","NOT",
"NOT_GTR","NOT_LSS","OF","ON","ONLY","OPTION","OR","ORDER","OUTER",
"OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","KW_PAGE_SIZE","PARAMETER","PASSWORD",
"PLAN","POSITION","POST_EVENT","PRECISION","PRIMARY","PRIVILEGES","PROCEDURE",
"PROTECTED","READ","REAL","REFERENCES","RESERVING","RETAIN","RETURNING_VALUES",
"RETURNS","REVOKE","RIGHT","RPAREN","ROLLBACK","SEGMENT","SELECT","SET",
"SHADOW","KW_SHARED","SINGULAR","KW_SIZE","SMALLINT","SNAPSHOT","SOME","SORT",
"SQLCODE","STABILITY","STARTING","STATISTICS","SUB_TYPE","SUSPEND","SUM",
"TABLE","THEN","TO","TRANSACTION","TRIGGER","UNCOMMITTED","UNION","UNIQUE",
"UPDATE","USER","VALUES","VARCHAR","VARIABLE","VARYING","VERSION","VIEW","WAIT",
"WHEN","WHERE","WHILE","WITH","WORK","WRITE","FLOAT_NUMBER","NUMBER","NUMERIC",
"SYMBOL","STRING","INTRODUCER","ACTION","ADMIN","CASCADE","FREE_IT","RESTRICT",
"ROLE","COLUMN","TYPE","EXTRACT","YEAR","MONTH","DAY","HOUR","MINUTE","SECOND",
"WEEKDAY","YEARDAY","TIME","TIMESTAMP","CURRENT_DATE","CURRENT_TIME",
"CURRENT_TIMESTAMP","NUMBER64BIT","SCALEDINT","CURRENT_USER","CURRENT_ROLE",
"KW_BREAK","SUBSTRING","RECREATE","KW_DESCRIPTOR","FIRST","SKIP",
"CURRENT_CONNECTION","CURRENT_TRANSACTION","BIGINT","CASE","NULLIF","COALESCE",
"USING","NULLS","LAST","ROW_COUNT","LOCK","SAVEPOINT","RELEASE","STATEMENT",
"LEAVE","INSERTING","UPDATING","DELETING","KW_INSERTING","KW_UPDATING",
"KW_DELETING","BACKUP","KW_DIFFERENCE","OPEN","CLOSE","FETCH","ROWS","BLOCK",
"IIF","SCALAR_ARRAY","CROSS",
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
"arg_desc : init_data_type udf_data_type param_mechanism",
"param_mechanism :",
"param_mechanism : BY KW_DESCRIPTOR",
"param_mechanism : BY SCALAR_ARRAY",
"param_mechanism : KW_NULL",
"return_value1 : return_value",
"return_value1 : '(' return_value ')'",
"return_value : init_data_type udf_data_type return_mechanism",
"return_value : PARAMETER pos_short_integer",
"return_mechanism :",
"return_mechanism : BY KW_VALUE",
"return_mechanism : BY KW_DESCRIPTOR",
"return_mechanism : FREE_IT",
"return_mechanism : BY KW_DESCRIPTOR FREE_IT",
"filter_decl_clause : symbol_filter_name INPUT_TYPE blob_filter_subtype OUTPUT_TYPE blob_filter_subtype ENTRY_POINT sql_string MODULE_NAME sql_string",
"blob_filter_subtype : symbol_blob_subtype_name",
"blob_filter_subtype : signed_short_integer",
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
"cursor_declaration_item : symbol_cursor_name CURSOR FOR '(' select ')'",
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
"view_clause : symbol_view_name column_parens_opt AS begin_string select_expr check_opt end_string",
"rview_clause : symbol_view_name column_parens_opt AS begin_string select_expr check_opt end_string",
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
"keyword_or_column : USING",
"keyword_or_column : CROSS",
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
"select : select_expr for_update_clause lock_clause",
"for_update_clause : FOR UPDATE for_update_list",
"for_update_clause :",
"for_update_list : OF column_list",
"for_update_list :",
"lock_clause : WITH LOCK",
"lock_clause :",
"select_expr : select_expr_body order_clause rows_clause",
"column_select : select_expr_body order_clause rows_clause",
"column_singleton : select_expr_body order_clause rows_clause",
"select_expr_body : query_term",
"select_expr_body : select_expr_body UNION distinct_noise query_term",
"select_expr_body : select_expr_body UNION ALL query_term",
"query_term : query_spec",
"query_spec : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause",
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
"table_reference : table_primary",
"table_primary : table_proc",
"table_primary : derived_table",
"table_primary : '(' joined_table ')'",
"derived_table : '(' select_expr ')' as_noise correlation_name derived_column_list",
"correlation_name : symbol_table_alias_name",
"correlation_name :",
"derived_column_list : '(' alias_list ')'",
"derived_column_list :",
"alias_list : symbol_item_alias_name",
"alias_list : alias_list ',' symbol_item_alias_name",
"joined_table : cross_join",
"joined_table : natural_join",
"joined_table : qualified_join",
"cross_join : table_reference CROSS JOIN table_primary",
"natural_join : table_reference NATURAL join_type JOIN table_primary",
"qualified_join : table_reference join_type JOIN table_reference join_specification",
"join_specification : join_condition",
"join_specification : named_columns_join",
"join_condition : ON search_condition",
"named_columns_join : USING '(' column_list ')'",
"table_proc : symbol_procedure_name table_proc_inputs as_noise symbol_table_alias_name",
"table_proc : symbol_procedure_name table_proc_inputs",
"table_proc_inputs : '(' value_list ')'",
"table_proc_inputs :",
"table_name : simple_table_name",
"table_name : symbol_table_name symbol_table_alias_name",
"simple_table_name : symbol_table_name",
"join_type : INNER",
"join_type : LEFT outer_noise",
"join_type : RIGHT outer_noise",
"join_type : FULL outer_noise",
"join_type :",
"outer_noise : OUTER",
"outer_noise :",
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
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' value_list ')'",
"insert : INSERT INTO simple_table_name ins_column_parens_opt select_expr",
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
"predicate : distinct_predicate",
"predicate : between_predicate",
"predicate : like_predicate",
"predicate : in_predicate",
"predicate : null_predicate",
"predicate : quantified_predicate",
"predicate : exists_predicate",
"predicate : containing_predicate",
"predicate : starting_predicate",
"predicate : singular_predicate",
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
"distinct_predicate : value IS DISTINCT FROM value",
"distinct_predicate : value IS NOT DISTINCT FROM value",
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
"exists_predicate : EXISTS '(' select_expr ')'",
"singular_predicate : SINGULAR '(' select_expr ')'",
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
"distinct_noise : DISTINCT",
"distinct_noise :",
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
"non_reserved_word : INSERTING",
"non_reserved_word : UPDATING",
"non_reserved_word : DELETING",
"non_reserved_word : BLOCK",
"non_reserved_word : BACKUP",
"non_reserved_word : KW_DIFFERENCE",
"non_reserved_word : IIF",
"non_reserved_word : SCALAR_ARRAY",
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
		strcpy (field->fld_name, INTERNAL_FIELD_NAME);
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

	if (node)
	{
		DsqlNodStack stack;
		stack_nodes(node, stack);
		USHORT l = stack.getCount();

		dsql_nod* old = node;
		node = FB_NEW_RPT(*tdsql->getDefaultPool(), l) dsql_nod;
		node->nod_count = l;
		node->nod_type = nod_list;
		if (MemoryPool::blk_type(old) == dsql_type_nod)
		{
			node->nod_flags = old->nod_flags;
		}
		dsql_nod** ptr = node->nod_arg + node->nod_count;

		while (stack.hasData())
			*--ptr = stack.pop();
	}

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

	va_end(ptr);
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

	va_end(ptr);
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
					switch (sym->sym_keyword) {
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
						switch (nextToken) {
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
							  yyvsp[-2], yyvsp[0]); }
break;
case 87:
{ yyval = NULL; }
break;
case 88:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
break;
case 89:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_scalar_array, CONSTANT_SLONG); }
break;
case 90:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_ref_with_null, CONSTANT_SLONG); }
break;
case 92:
{ yyval = yyvsp[-1]; }
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); }
break;
case 94:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
break;
case 95:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_reference, CONSTANT_SLONG); }
break;
case 96:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_value, CONSTANT_SLONG); }
break;
case 97:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
break;
case 98:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_reference), CONSTANT_SLONG); }
break;
case 99:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_descriptor), CONSTANT_SLONG); }
break;
case 100:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 101:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 102:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 103:
{ yyval = yyvsp[0]; }
break;
case 104:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 105:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
break;
case 116:
{ yyval = yyvsp[0]; }
break;
case 117:
{ yyval = yyvsp[0]; }
break;
case 118:
{ yyval = yyvsp[0]; }
break;
case 119:
{ yyval = yyvsp[0]; }
break;
case 120:
{ yyval = yyvsp[0]; }
break;
case 121:
{ yyval = yyvsp[0]; }
break;
case 122:
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 123:
{ yyval = NULL; }
break;
case 124:
{ yyval = make_list (yyvsp[0]); }
break;
case 126:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 127:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 128:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 129:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 130:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 131:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 132:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 133:
{ yyval = (dsql_nod*) 0;}
break;
case 134:
{ yyval = yyvsp[-1]; }
break;
case 135:
{ yyval = NULL; }
break;
case 138:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 139:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 140:
{ yyval = NULL; }
break;
case 141:
{ yyval = NULL; }
break;
case 142:
{ yyval = yyvsp[0]; }
break;
case 143:
{ yyval = NULL; }
break;
case 144:
{ yyval = NULL; }
break;
case 147:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 148:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 151:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 152:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 153:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
break;
case 154:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
break;
case 155:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 158:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
break;
case 159:
{yyval = NULL;}
break;
case 162:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 163:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 164:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 165:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 166:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 167:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 168:
{yyval = NULL;}
break;
case 171:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 173:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 174:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 175:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 176:
{ lex.g_file  = make_file();}
break;
case 181:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 182:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 189:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 190:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 191:
{ yyval = yyvsp[0]; }
break;
case 192:
{ yyval = yyvsp[0]; }
break;
case 193:
{ yyval = NULL; }
break;
case 195:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 198:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 199:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 200:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 201:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 204:
{ yyval = NULL; }
break;
case 205:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 206:
{ yyval = yyvsp[0]; }
break;
case 207:
{ yyval = NULL; }
break;
case 208:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 209:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 210:
{ yyval = yyvsp[-1]; }
break;
case 211:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 212:
{ yyval = yyvsp[0]; }
break;
case 213:
{ yyval = NULL; }
break;
case 220:
{ yyval = NULL; }
break;
case 223:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 224:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 225:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 226:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 228:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 229:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 230:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 231:
{ yyval = yyvsp[0]; }
break;
case 232:
{ yyval = NULL ;}
break;
case 237:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 238:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 240:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 241:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 242:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 243:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 244:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 245:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 246:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 247:
{ yyval = NULL;}
break;
case 248:
{ yyval = yyvsp[0];}
break;
case 249:
{ yyval = yyvsp[0];}
break;
case 250:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 251:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 252:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 253:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 254:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 255:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 256:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 257:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 258:
{ yyval = make_list (yyvsp[-1]); }
break;
case 259:
{ yyval = NULL; }
break;
case 260:
{ yyval = make_list (yyvsp[-1]); }
break;
case 261:
{ yyval = NULL; }
break;
case 263:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 264:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 266:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 267:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 268:
{ yyval = yyvsp[0]; }
break;
case 269:
{ yyval = yyvsp[0]; }
break;
case 270:
{ yyval = (dsql_nod*) NULL; }
break;
case 271:
{ yyval = make_list (yyvsp[0]); }
break;
case 272:
{ yyval = NULL; }
break;
case 274:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 275:
{ yyval = yyvsp[-1]; }
break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
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
case 283:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 286:
{ yyval = yyvsp[-1]; }
break;
case 287:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 288:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 289:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 291:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 307:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 308:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 311:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 314:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 315:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 316:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 317:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 319:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 321:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 322:
{ yyval = NULL; }
break;
case 323:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 324:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 325:
{ yyval = make_list (yyvsp[0]); }
break;
case 326:
{ yyval = make_list (yyvsp[-1]); }
break;
case 327:
{ yyval = NULL; }
break;
case 328:
{ yyval = make_list (yyvsp[0]); }
break;
case 329:
{ yyval = make_list (yyvsp[-1]); }
break;
case 330:
{ yyval = NULL; }
break;
case 333:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 335:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 336:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 337:
{ yyval = NULL; }
break;
case 338:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 339:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 341:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 342:
{ yyval = NULL; }
break;
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 355:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 356:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 357:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 358:
{ yyval = NULL; }
break;
case 359:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 360:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 361:
{ yyval = make_list (yyvsp[-1]); }
break;
case 362:
{ yyval = NULL; }
break;
case 364:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 365:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
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
{ lex.beginning = lex_position(); }
break;
case 369:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 370:
{ lex.beginning = lex.last_token; }
break;
case 371:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
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
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 375:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 376:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 377:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 378:
{ yyval = NULL; }
break;
case 379:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 380:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 398:
{ yyval = NULL; }
break;
case 399:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
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
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
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
case 458:
{ yyval = NULL; }
break;
case 459:
{ yyval = NULL; }
break;
case 460:
{ yyval = NULL; }
break;
case 461:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 462:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 463:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 464:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 465:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 466:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 467:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 468:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 470:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 471:
{ yyval = yyvsp[0]; }
break;
case 472:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 473:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 474:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 475:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 476:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
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
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 503:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 509:
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
case 513:
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
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 521:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 522:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 523:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
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
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
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
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
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
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
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
case 546:
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
case 549:
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
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 559:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 560:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
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
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 605:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 606:
{ yyval = (dsql_nod*) 0; }
break;
case 607:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 608:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 610:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 611:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 612:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
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
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 618:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 619:
{ yyval = NULL; }
break;
case 620:
{ yyval = yyvsp[0]; }
break;
case 621:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 622:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 623:
{ yyval = NULL; }
break;
case 624:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 625:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 626:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 627:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 628:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 629:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 631:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 637:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 638:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 639:
{ yyval = 0; }
break;
case 640:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 641:
{ yyval = yyvsp[-2]; }
break;
case 642:
{ yyval = yyvsp[-1]; }
break;
case 643:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 656:
{ yyval = make_list (yyvsp[0]); }
break;
case 658:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 663:
{ yyval = yyvsp[-1]; }
break;
case 664:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 666:
{ yyval = NULL; }
break;
case 667:
{ yyval = make_list (yyvsp[-1]); }
break;
case 668:
{ yyval = NULL; }
break;
case 670:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 674:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 675:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 676:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 679:
{ yyval = yyvsp[0]; }
break;
case 680:
{ yyval = make_list (yyvsp[-1]); }
break;
case 681:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 682:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 683:
{ yyval = make_list (yyvsp[-1]); }
break;
case 684:
{ yyval = NULL; }
break;
case 686:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 687:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 688:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 689:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 690:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 691:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 695:
{ yyval = make_list (yyvsp[0]); }
break;
case 696:
{ yyval = NULL; }
break;
case 698:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 700:
{ yyval = yyvsp[0]; }
break;
case 701:
{ yyval = NULL; }
break;
case 702:
{ yyval = yyvsp[0]; }
break;
case 703:
{ yyval = NULL; }
break;
case 704:
{ yyval = yyvsp[0]; }
break;
case 705:
{ yyval = NULL; }
break;
case 706:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 707:
{ yyval = 0; }
break;
case 708:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 709:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 710:
{ yyval = 0; }
break;
case 711:
{ yyval = 0; }
break;
case 713:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 714:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 717:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 718:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 719:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 720:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 722:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 723:
{ yyval = make_list (yyvsp[-1]); }
break;
case 724:
{ yyval = 0; }
break;
case 725:
{ yyval = make_list (yyvsp[0]); }
break;
case 726:
{ yyval = 0; }
break;
case 728:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 729:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 730:
{ yyval = 0; }
break;
case 731:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 732:
{ yyval = 0; }
break;
case 733:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 734:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 735:
{ yyval = yyvsp[-1]; }
break;
case 736:
{ yyval = 0; }
break;
case 737:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 738:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 739:
{ yyval = NULL; }
break;
case 740:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 741:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 744:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 745:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 749:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 750:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 752:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 753:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 754:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 755:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 757:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 761:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 762:
{ yyval = yyvsp[0]; }
break;
case 764:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 767:
{ yyval = NULL; }
break;
case 768:
{ yyval = make_list (yyvsp[-1]); }
break;
case 770:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 772:
{ yyval = NULL; }
break;
case 773:
{ yyval = make_list (yyvsp[-1]); }
break;
case 775:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 778:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 779:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = yyvsp[-1]; }
break;
case 795:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 808:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 810:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 834:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
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
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 854:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 855:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 856:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 857:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 858:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 860:
{ yyval = make_list (yyvsp[-1]); }
break;
case 861:
{ yyval = yyvsp[-1]; }
break;
case 871:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 872:
{ yyval = yyvsp[0]; }
break;
case 873:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 874:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 875:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 876:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 877:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 878:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 879:
{ yyval = yyvsp[-1]; }
break;
case 880:
{ yyval = yyvsp[-1]; }
break;
case 884:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 885:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 886:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 889:
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
case 890:
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
case 891:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 892:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 894:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 896:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 897:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 898:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 899:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 900:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 901:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 903:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 904:
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
case 905:
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
case 906:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 907:
{ yyval = make_parameter (); }
break;
case 908:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 909:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 910:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 911:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 912:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 913:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 914:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 915:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 916:
{ yyval = yyvsp[0]; }
break;
case 917:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 919:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 920:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 921:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 922:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 923:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 925:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 926:
{ yyval = yyvsp[0];}
break;
case 931:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 932:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 933:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 936:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 937:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 938:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 939:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 940:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 942:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 944:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 946:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 947:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 948:
{ yyval = yyvsp[0]; }
break;
case 949:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 950:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 951:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 952:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 955:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 956:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
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
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 972:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 974:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 976:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 977:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 978:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 983:
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
