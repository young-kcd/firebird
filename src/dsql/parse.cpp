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
    5,    1,    1,    7,    4,    4,    1,    1,    8,    5,
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
    0,  745,  490,    0,    0,    0,    0,  884,  913,    0,
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
    0,    0,    0,  904,    0,    0,    0,    0,    0,    0,
  905,  906,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  324, 1005,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  267,  504,
    0,    0,    0,  361,    0,    0,    0,  768,   29,    0,
    0,   45,   73,    0,    0,   27,    0,  774,    0,    0,
  741,    0,    0,    0,    0,    0,  568,    0,  632,    0,
  654,    0,    0,    0,    0,  559,    0,  924,  558,    0,
  592,    0,  600,  596,  595,    0,    0,    0,  611,  599,
    0,    0,    0,  750,    0,    0,    0,  368,    0,    0,
    0,    0,    0,  138,    0,  473,  370,  413,    0,  370,
  371,  418,    0,    0,  258,  368,  231,  989,    0,    0,
  370,    0,    0,  200,  368,    0,    0,    0,    0,  236,
  230,  233,  234,  235,  422,  464,  463,  421,  420,    0,
    0,    0,    0,    0,    0,  379,    0,  370,    0,  163,
  167,    0,    0,  171,    0,    0,  761,    0,  535,  536,
  552,    0,  549,    0,    0,  542,  540,  541,    0,    0,
    0,    0,  505,    0,    0,    0,    0,  543,    0,    0,
  368,    0,  191,    0,  194,  196,  197,    0,    0,    0,
    0,    0,  211,   79,    0,    0,   78,    0,    0,    0,
    0,  783,  795,    0,    0,  856,  857,  858,    0,  787,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  707,
  709,    0,  704,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  971,
  972,  973,  974,  975,  976,  977,  978,    0,    0,    0,
    0,    0,    0,  964,    0,    0,    0,    0,  879,  326,
    0,  880,  951,    0,  875,  987,    0,    0,    0,    0,
    0,    0,    0,  885,  778,  777,  365,  364,    0,  265,
    0,    0,    0,  273,  770,    0,    0,    0,    0,    0,
    0,    0,    0,   54,   62,   64,    0,    0,   71,    0,
    0,    0,  773,    0,    0,    0,    0,    0,    0,    0,
  641,  644,  997,  653,    0,    0,    0,    0,  659,  660,
  661,  662,  671,  672,  673,    0,    0,  651,  560,  925,
  601,  610,  605,  604,    0,  615,  781,  752,    0,    0,
  368,    0,    0,  634,  729,  728,    0,  472,    0,    0,
  175,    0,  179,    0,    0,    0,  412,  368,  368,  270,
  371,  263,    0,  202,  370,  204,    0,  371,  199,  205,
    0,    0,    0,    0,  425,  426,  370,  368,  371,    0,
    0,    0,  397,    0,  187,  188,  164,    0,  174,  523,
  524,  923,    0,    0,    0,    0,    0,  550,    0,    0,
  370,  371,    0,  500,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  189,    0,    0,
    0,   83,    0,    0,    0,    0,   91,   85,    0,    0,
   90,   86,    0,  746,  990,    0,    0,  788,    0,    0,
  794,    0,    0,    0,    0,    0,  832,  831,    0,    0,
    0,  841,  859,    0,  851,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  786,    0,  708,    0,  744,
    0,    0,  211,    0,  931,    0,    0,  946,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  960,    0,    0,    0,    0,    0,    0,    0,  626,  950,
  892,    0,  260,  279,    0,    0,  360,  274,   66,   58,
   59,   65,   60,   61,    0,    0,   23,    0,    0,    0,
   43,    0,    0,    0,  755,  775,    0,  754,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  688,
    0,    0,    0,    0,    0,    0,    0,  607,  608,  612,
    0,    0,  190,    0,    0,    0,    0,    0,  180,    0,
    0,    0,  408,  214,  215,  216,  217,  218,  219,  895,
    0,    0,  264,    0,    0,  212,    0,    0,    0,    0,
  237,    0,  460,  461,  427,    0,    0,    0,    0,    0,
    0,  370,  479,  371,  173,  986,    0,    0,  518,    0,
  517,  553,    0,  545,    0,    0,    0,  498,    0,  526,
    0,    0,  528,  531,  533,  368,    0,    0,    0,  127,
    0,  195,  370,    0,  369,  105,    0,  125,    0,   94,
    0,    0,    0,    0,   88,   89,    0,  849,  850,  790,
    0,  792,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  852,    0,    0,    0,    0,    0,    0,    0,  842,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  715,    0,    0,    0,  937,
  936,    0,    0,  933,  932,    0,  941,  940,  939,  938,
  935,  934,    0,    0,  966,    0,    0,    0,  965,    0,
    0,    0,    0,  266,    0,    0,  276,  277,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  358,    0,    0,    0,    0,  285,  290,  284,    0,
    0,    0,  294,  295,  296,  297,  298,  299,  301,  302,
  304,  305,  306,  309,    0,    0,    0,  352,  353,  354,
    0,    0,   57,   63,   55,   56,   24,  760,  759,    0,
  757,  765,  764,  762,  740,    0,    0,    0,  663,    0,
    0,    0,  693,  691,  689,    0,  690,    0,    0,    0,
    0,    0,  749,    0,  369,  733,  734,  635,    0,  185,
  181,    0,  371,  896,  268,  269,  371,    0,    0,    0,
  222,    0,    0,  238,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  476,  520,    0,  521,    0,  142,    0,
    0,    0,  146,  148,  149,  150,  501,  503,  499,  530,
    0,    0,  371,    0,    0,    0,  366,  370,   92,    0,
    0,   98,   93,    0,    0,    0,    0,    0,  860,    0,
  861,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  717,  706,    0,    0,  718,    0,  714,  210,  952,
  942,  944,    0,    0,  962,  961,    0,  967,  955,  957,
    0,    0,  275,    0,  313,    0,    0,    0,  308,    0,
    0,  307,  338,  339,    0,  998,  355,  356,    0,  293,
  300,    0,  303,  286,    0,  291,    0,  343,  292,  310,
    0,    0,  336,   41,    0,    0,  683,  681,    0,  674,
    0,    0,    0,  697,    0,  631,  371,  367,  735,  182,
  371,  152,  257,    0,    0,  198,  223,    0,    0,    0,
    0,  224,  227,    0,    0,  371,  395,  396,  392,  391,
  394,  393,    0,  519,  546,  151,  139,  147,    0,  370,
  254,  134,  371,  372,    0,    0,   96,    0,   80,    0,
  818,  826,  625,    0,  819,  827,  822,  830,    0,    0,
  820,  828,  821,  829,  815,  823,  816,  824,  817,  825,
  713,    0,    0,    0,  947,  963,    0,    0,  282,  278,
    0,  311,    0,    0,    0,    0,    0,  340,    0,    0,
  331,  332,    0,  351,    0,    0,    0,    0,  346,  344,
    0,    0,    0,  756,    0,  665,  675,    0,    0,  676,
  677,  678,    0,  255,  409,  201,  206,  225,    0,    0,
  228,    0,  240,  242,  399,  371,    0,  374,    0,   77,
   99,  100,    0,    0,    0,  720,  956,  281,    0,  312,
    0,    0,  315,    0,  321,    0,  323,    0,    0,  350,
  349,  994,  348,    0,    0,    0,    0,    0,    0,  664,
    0,    0,  698,  229,    0,    0,    0,    0,    0,  256,
  371,    0,    0,  719,    0,    0,    0,    0,    0,    0,
    0,  334,  333,  345,  347,    0,    0,    0,  669,    0,
    0,    0,    0,    0,  239,    0,  246,    0,  245,  375,
  126,  722,    0,  283,    0,    0,  314,  318,    0,    0,
    0,    0,    0,    0,  667,  680,  226,    0,    0,  250,
  249,  248,  723,    0,    0,    0,    0,    0,  335,  670,
  253,  251,  252,  329,  319,    0,  341,  316,  317,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1443,   28,   29, 1444,
   31,   32,   33,   34,   35,   36,   37, 1445,   39, 1446,
  847,  128,  488,  992, 1237,  129,  741,  993,  130,  492,
  746,  489,  131,  132,  275, 1016,  144,  994,  995,  213,
  242,  133,  996,  134,  494,   85,  253,  397,  445,  657,
 1125,  446,  886,  628,  235,  863,  658,  659,  660, 1132,
 1127, 1563,  254,  662,  663,   77,  203,   78,  249, 1783,
 1346,  234,  239,  241,  243,  229,  221,  223,  236,  246,
  161,  328,  330,  332,  386,  644,  646,  485,  276,  810,
  565,  673, 1057,  390,  649, 1116, 1340,  596,  768, 1087,
  557,  558,  578,  382,  629, 1102, 1541, 1666, 1283, 1542,
 1543, 1544, 1545, 1546,  674,  230,  222,  378,  604,  379,
  380,  605,  606, 1315,  559, 1051, 1052, 1053, 1278,  392,
  874,  875,  876,  877,  812, 1068, 1519,  630,  814,  448,
  879,  965,  479, 1403, 1284,  449,  450,  451,  452,  453,
 1520, 1521,  581, 1672, 1817, 1301,  820,  821,  807,  822,
  823,  824, 1818, 1819, 1871,  360,  484,  982, 1447,  209,
  574,  979,  575, 1061,  480,  983,  984, 1225, 1426, 1427,
 1428, 1730, 1134, 1448, 1449, 1450, 1451, 1637, 1452, 1453,
 1454, 1455, 1456, 1457, 1458, 1459, 1460, 1461, 1462, 1463,
 1464, 1465,  454, 1828, 1466, 1740, 1862, 1737,  455,  717,
  456,  457, 1467, 1638, 1748, 1749, 1801, 1468, 1469, 1470,
 1629,  272,  481,  482,  458,   41, 1345, 1557,  369,  592,
  838, 1313,  593,  836,   82,   62,  364,  214,  171,  347,
  200,  354,  206,  801,  798,  355,  365,  828,  366,  588,
 1079,  201,  459,  348,  594, 1314,   99,  212,  258,  631,
  632,  730, 1103,  731, 1104, 1105,  635,  636,  637,  638,
 1479, 1096, 1094,  639,  640,  641,  856,  642,  853,   42,
   43,   44,   45,   46,   47,  163,  545,   64,  510,  217,
  371,  315,  316,  317,  318,  319,  320,  321,  322,  323,
  774,  531, 1035, 1270,  532,  533,  779,  167,  336,  547,
   48,  170,  342, 1369,  716,   49,  340,   50,  148,  303,
  518,  764,  401, 1267, 1502,  690,  513,  305, 1275, 1659,
  149,  150,  304,  519,  520,  762, 1014, 1017, 1018, 1019,
 1020, 1021, 1022, 1755, 1810,  460, 1840, 1023, 1024, 1025,
 1265, 1760, 1761, 1762, 1257,  157, 1494, 1653, 1654, 1396,
  924, 1397, 1398, 1399, 1598, 1784, 1786,  550,  551, 1045,
 1508,  500,   51,   52,  402,   53,   54,  539,  540, 1001,
 1245, 1480, 1484,  501,  749,  227,  675,  676,  901,  902,
  677,  678,  679,  680,  681,  682,  683,  684,  685,  686,
  687,  688, 1150, 1152, 1153,  461,  462,  463,  464,  465,
  466,  467,  251,  468,  469,  470,  471,  472,  948,  473,
 1604,  474,  475,  476,  477,  706,  952, 1214, 1209,  707,
  136,
};
static short yysindex[] = {                                   5306,
 1905, -163, 3585,   75, 1214,   -4, 3939, -183, 8605,   13,
   32, 8360, -163, 1110,  630, 4983, 1294, 4983, -168,    0,
  635,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  435,    0,    0,    0,    0,    0,    0,  584,    0,    0,
    0,    0,    0,    0,    0,14098, 4983, 4983, 4983, 4983,
 4983,    0,    0,  515, 1211, 4983, 4983, 4983,  661, 4983,
  494, 4983, 4983,    0, 4983, 4983,    0, 1117,    0,  516,
    0,    0,  702, 4983,    0, 4983, 4983, 4983,  714, 4983,
 4983, 4983, 4983,  494, 4983, 4983, 4983, 4983,    0, 4983,
 1004,  656,    0,    0,    0, 1062,    0, 1062,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  735,  792,   79,
 1130,    0,    0,    0,    0,    0, 4983, 4983, 4983,  807,
  820,  876,  136, 2039,  838,  147,  211,   37,  851,    0,
 4983, 1022, 1242,    0,    0, 4983,  962, 4983, 4983, 4983,
    0,    0,    0, 4983,    0,  865,  965, 1144,  796,  885,
 1323,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  932,
    0,    0, 1211,    0,   35,    0,    0, 1383,    0,    0,
 1226,    0,  715,    0,    0, 1033,    0,    0, 1050,    0,
    0, 1318,    0, 1247,    0,    0,    0, 1211,    0,    0,
    0,  303, 1383,    0,    7,    0,    0, 1236,    0, 1244,
    0, 1062,    0,    0,    0,    0,    0,    0, 1271,    0,
    0, 1159,    0, 1284,    0, 1183,    0,    0,    0, 1159,
    0,    0,    0,    0,    0,    0,    0,    0,    0,13249,
 4983, 1230,    0, 4983,    0,    0,    0, 4936, 1256, 4694,
 4983,  619, 1364, 1661, 1395, 1409, 4936, 1355, 4694, 1341,
 1385, 1267,    0,13356,    0,    0,    0,    0,    0,    0,
    0,    0,11604,    0,    0,    0, 1370, 4983, 1427, 1345,
  864, 4983, 1379,    0,    0, 1242,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 4983, 1383,    0, 1236,    0,
 1062,    0, 1440, 1447, 1332,    0,13356, 1421,    0, 1421,
13356,    0,  -92, 1342, 1347, 1343, 1323,    0, 1579,  730,
    0, 4983,    0,  932,    0,    0,    0,    0, 4983, 1230,
 3576, 1396, 4653, 1821,    0,14098,    0,    0,  872,    0,
    0,    0, 1809, 1809, 1211, 1506, 1211,  386, 1318,    0,
    0, 6262,    0, 4983, 4983,    0, 1230,    0,    0, 1590,
 -126, 1835, 4983, 1614, 4983,    0,    0, 1839, 1601, 8720,
 1491,    0,    0, 1844, 1846, 1848, 1436,    0,    0, 1850,
    0, 1859,    0, 1873, 1875,    0, 1876,    0,    0,    0,
    0,    0, 1877, 1465, 1466,    0,    0,    0,    0,    0,
    0,    0, 1887,    0,    0,11862, 1888, 1890,    0, 1894,
13356,13356,12081, 4983, 1901,    0, 3381,    0,    0,    0,
    0,    0,    0,    0,    0, 1881, 1852,    0,    0, 1900,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 6262, 1886,
  265,    0, 1908, 1686, 1603,    0, 4983, 1523,    0, 4983,
 4983,   78,    0,    0,    0,    0,    0, 4983, 4983, -102,
    0, 4983,    0, 1636, 4983, 1911, 4936, 1561,    0, 4983,
12081, 1969,    0,    0,13356,    0, 1766, 1640, 1922,    0,
    0,   70,    0,  471,    0,  564,    0,  564,    0,    0,
 1933,    0,  152, 1555,    0,    0,    0, 1934,   87, 1918,
 1230, 1947, 1731,    0,    0, 4983,    0,    0, 1790, 1952,
    0,    0,    0,  866,    0, 1654, 1657,    0, 1211,    0,
 1659,    0,    0, 1733, 1737,    0,    0,    0, 1718,    0,
 6262,    0, 6262, 1646,    0, 1741, 4983,13981,    0,    0,
  655,    0, 4983, 1200, 1226, 1628, 1589, 1557,    0,    0,
    0,    0,  442, 1630,    0, 1571,  494,    0, 1211,    0,
 1756, 1683,    0,    0,  386,    0,    0,  210, 1595,    0,
    0,    0, 1598,    0, 1648, 1988,    0, 1696, 2002,  415,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1955,  546,    0,    0,    0,    0,  181, 2010,
 2014, 2002, 1383,    0, 1742,    0, 1794,    0, 1211, 1211,
    0, 3576,  715,    0, 1687,    0, 1663, 2020,    0, 5673,
    0, 1697,    0,    0, 1705, 2050, 8952, 2051,    0,    0,
    0, 8475, 5218,   33,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  290, 1720,
 1128,13356,  192,    0, 4983,13356, 1133, 1193, 1195, 2307,
    0,    0,13356, 9197, 3381, 1664,  558,13356,13356, 9197,
  761,  761, 2281, 1652,  584, 2067,    0,    0,12188, 4983,
13356,13356,13356,13356,13356,13356,13356, 2594,    0,    0,
 1837, 2058, 4983,    0, 4983, 1831, 4983,    0,    0,11488,
 1703,    0,    0, 1674, 4694,    0, 1822,    0, 1681, 2102,
    0, 1822,11488, 1827, 1828, 4983,    0, 2281,    0, 2345,
    0, 4983, 1060, 1706,13356,    0,  581,    0,    0,   16,
    0, 1709,    0,    0,    0, 4983,  966, 4983,    0,    0,
 4983, 4983, 1491,    0,13356, 1891, 3576,    0, 2107, 1669,
13356,13356, 1211,    0, -107,    0,    0,    0, 2117,    0,
    0,    0,  163, 4983,    0,    0,    0,    0, 1897, 2118,
    0, 1867, 1892,    0,    0, 1832, 1834, 1062, 1895,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  494,
 4983,14040, 1788, 1796, 1801,    0,  494,    0, 1287,    0,
    0, 1774, 1211,    0, 1601,   66,    0, 1777,    0,    0,
    0,  494,    0, 1988,   95,    0,    0,    0, 1896,   95,
 1779,   95,    0, 2147,  494,  494,  494,    0, 1230, 4983,
    0, 1842,    0, 1682,    0,    0,    0,  872, 1421, 4983,
 1688,   77,    0,    0, 2151,  647,    0, 1601, 4983, 1421,
 9429,    0,    0, 1421, 8952,    0,    0,    0, 4929,    0,
   96,  495,13356,13356, 9536, 2159,  891, 9794,13356,10052,
  952,10159,10417,12407,10675,10782,11040, 9197, 9197,    0,
    0, 1840,    0, 2163,  885,13356,13356, 1931,13356, 2164,
13356, 2160, 2414,13356,13356,13356,13356,13356,13356,    0,
    0,    0,    0,    0,    0,    0,    0, 1893, 1428,  -54,
13356,  620,13356,    0, 9197, 2509, 2709,  130,    0,    0,
  885,    0,    0, 1689,    0,    0, 1949,  761,  761,  890,
  890, 3381,  766,    0,    0,    0,    0,    0, 1702,    0,
 1773, 1954, 1831,    0,    0, 4983, 4983, 4983, 4983, 4983,
 4983,   92, 2184,    0,    0,    0,11488, 1854,    0,  405,
 1878, 4983,    0,13356, 1878, 2192, 2184,11488, 4353, 1925,
    0,    0,    0,    0, 1998, 2203, 2200,  949,    0,    0,
    0,    0,    0,    0,    0, 9197, 1920,    0,    0,    0,
    0,    0,    0,    0,  599,    0,    0,    0, 1720, 3381,
    0, 1713, 1421,    0,    0,    0, 3381,    0, 1809, 1981,
    0, -107,    0, 2208, 9197, 3938,    0,    0,    0,    0,
    0,    0, 1831,    0,    0,    0, 3938,    0,    0,    0,
 1062, 1062, 1760, 2209,    0,    0,    0,    0,    0,  403,
   69,  378,    0, 1990,    0,    0,    0, 4983,    0,    0,
    0,    0,   75, 1728, 1841, 1837, 2215,    0, 1571, 1729,
    0,    0, 1095,    0, 2201, 4983, 1125,  494, 2217, 2223,
 2227, 2005,  715, 1831, 1809, 1657, 3576,    0, 1630, 1824,
 2034,    0,  494, 1889, 1968, 5673,    0,    0,  494,  548,
    0,    0, 1971,    0,    0, 2234, 2238,    0, 9197, 9197,
    0, 9197, 9197, 3488, 3381, 2240,    0,    0, 3381, 2241,
12081,    0,    0, 1966,    0,   -7, 2245, 3381, 2246, 1632,
 2247, 3381, 2250,13356,13356, 2159,13356,12665, 2251, 3381,
 2252, 2254, 3381, 2255,13356, 3381, 2256, 3381, 2257, 2258,
 3381, 2261, 2262, 3381, 2263,    0, 2044,    0, 6794,    0,
 2662, 2691,    0, 2751,    0, 2883,13356,    0, 3183, 3344,
 3353, 3364, 3371, 3562,13356,13356,13356, 3381, 1882,13356,
    0,13356, 3381, 2003,   27,13356,13356,13356,    0,    0,
    0, 4983,    0,    0, 4983,10292,    0,    0,    0,    0,
    0,    0,    0,    0, 1986,11488,    0,11488,   92, 2184,
    0,  318,  318,  103,    0,    0, 1744,    0, 2192, 2184,
 2283, 4353, 2284,  949, 2288,13356, 2066, 1060, 1951,    0,
 1951,  731, 1951, 1994, 1995, 2063, 2012,    0,    0,    0,
  885, 1831,    0, 1824,  691, 1571, 1956, 1571,    0, 9197,
  184, 1152,    0,    0,    0,    0,    0,    0,    0,    0,
 3938, 3938,    0, 1954,13356,    0, 2057, 1945, 1760, 1117,
    0, 9197,    0,    0,    0, 1972, 1978, 1980, 1987, 1989,
 1992,    0,    0,    0,    0,    0, 2319,   75,    0, 1898,
    0,    0,   95,    0, 3938, 2000,   95,    0,   95,    0,
 1837, 2326,    0,    0,    0,    0,  872, 1954, 1571,    0,
 1657,    0,    0, 2113,    0,    0, 2107,    0, 2351,    0,
 2353, 1211,  -75, 2354,    0,    0, 1211,    0,    0,    0,
 2044,    0, 2044,13356, 1421, 1421, 1752,  584, 2360,13356,
 2085,    0, 1421, 1421,13356, 1421, 1421, 3715, 3381,    0,
 1851,13356, 3381, 1421, 1421, 1421, 1421, 3381, 1421, 1421,
 1421, 1421, 1421, 1421, 4983,    0, 2363, 2362,  985,    0,
    0, 6262, 2367,    0,    0, 3609,    0,    0,    0,    0,
    0,    0, 3652, 1567,    0,13356, 2105, 1984,    0,13356,
 3659, 1753, 2781,    0, 6262, 2357,    0,    0, 2122,    0,
 2982,  215, 2359, 2373, 2084,13356, 2364, 2365, 3282, 4983,
 4983,    0, 2368, 2369, 2086, 2376,    0,    0,    0, 2116,
 9669, 2377,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2380,  -83, 2382,    0,    0,    0,
    0, 2048,    0,    0,    0,    0,    0,    0,    0, 2006,
    0,    0,    0,    0,    0, 4353, 2283, 2066,    0, 1758,
 4983,  949,    0,    0,    0, 2104,    0, 1060, 1060,13356,
 9197, 1491,    0, 1954,    0,    0,    0,    0, 1287,    0,
    0,  201,    0,    0,    0,    0,    0, 3381, 2170, 2057,
    0,  811, 4983,    0, 2108,  382, 2015, 2119, 2017, 2162,
 2128, 2166, 1831,    0,    0, 2425,    0, 2427,    0, 2124,
 2170, 2000,    0,    0,    0,    0,    0,    0,    0,    0,
 1831, 1630,    0, 1287, 1990, 2092,    0,    0,    0, 2109,
 -145,    0,    0, 1837, 2111, 3381, 2436, 2437,    0,  885,
    0, 3381,13356, 2440, 2443, 3381, 2444, 2446,13356,13356,
 3381, 2447, 2453, 2455, 2456, 2463, 2464, 2467, 2468, 2470,
 2476,    0,    0, 6794, 2479,    0, 4983,    0,    0,    0,
    0,    0,13356, 2480,    0,    0,13356,    0,    0,    0,
13356,  321,    0, 2205,    0,12772, 4983,13356,    0, 9197,
 3381,    0,    0,    0, 2473,    0,    0,    0, 4983,    0,
    0, 2553,    0,    0,  530,    0, 2080,    0,    0,    0,
  582, 2487,    0,    0,  318, 4983,    0,    0, 1060,    0,
 1631, 3381, 2489,    0,   33,    0,    0,    0,    0,    0,
    0,    0,    0, 2498, 4983,    0,    0, 2193, 2204, 4983,
 1760,    0,    0, 1062, 4983,    0,    0,    0,    0,    0,
    0,    0, 1954,    0,    0,    0,    0,    0, 1954,    0,
    0,    0,    0,    0,13356, 1211,    0, 2087,    0, 1211,
    0,    0,    0, 3381,    0,    0,    0,    0, 3381, 3381,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 4983, 2213, 3381,    0,    0, 3667, 3938,    0,    0,
 2507,    0, 3069,13249, 1817,  401, 2505,    0, 2221, 1286,
    0,    0, 2519,    0, 4983, 4983,   75,  144,    0,    0,
 2068, 2230, 9197,    0, 2534,    0,    0, 9197, 2536,    0,
    0,    0,13356,    0,    0,    0,    0,    0, 1760, 1062,
    0, 2207,    0,    0,    0,    0, 1990,    0, 3381,    0,
    0,    0, 2533, 2543, 2545,    0,    0,    0, 1421,    0,
 2186, 2553,    0, 2169,    0, 2553,    0, 2553, 2682,    0,
    0,    0,    0,10292,  530,13356, 2553,  615, 4983,    0,
   33, 4983,    0,    0, 2207,  367, 1760, 2214, 2222,    0,
    0, 2548, 4983,    0, 4983, 2556, 2375, 2540, 1340,10292,
 2560,    0,    0,    0,    0, 1576,  226, 2304,    0, 1782,
 1783, 1760,  489,  489,    0, 2308,    0, 2173,    0,    0,
    0,    0, 2567,    0, 2553, 2560,    0,    0, 2306, 2553,
 2322, 2312,10292, 4983,    0,    0,    0, 2161,  902,    0,
    0,    0,    0, 1795,10292,  256, 4983,10292,    0,    0,
    0,    0,    0,    0,    0,10292,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  200,  419,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  116,11278,    0,    0,    0,    0,    0,    0,
 2614,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  374,    0,    0,    0,    0,    0,    0,  410,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  870,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2285,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -24, 2243,    0,    0,    0,  101,    0,  101,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2248,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1076,    0,    0,12991,11385,    0,
    0,    0, 1105,    0,    0,  132,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  732,    0, 2212,  600,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   82,    0,    0,
    0,    0,  950,    0,    0, 1187,    0,    0,    0,    0,
    0,  112,    0, 9043,    0,    0,    0,    0,    0,    0,
    0,    0,   82,    0,  -82,    0,    0, 2577,    0,    0,
    0, 2361,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  138,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1203,
    0, 2366,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  880,    0,    0,    0,    0,    0,    0,
    0, 5803,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  934,    0,    0, 1205,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   82,    0, 2577,    0,
 2361,    0, 1225,  785,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1232,    0, 2350,    0,
    0,    0,    0, 1285,    0,    0,    0,    0,    0, 2366,
  575,14164,    0, 1308,    0,    0,    0,    0,  675,    0,
    0,    0, 2179, 2179,    0,    0,    0, 1346,  160,    0,
    0,    0,    0,    0,    0,    0, 2366,    0,    0, 1238,
    0,    0,    0,    0,    0,    0,    0, 5384,    0,    0,
  104,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1861,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2638,    0,    0,    0, 2938, 3238,    0, 3538,
    0,    0,    0,    0,    0,    0,  294,    0,    0,    0,
    0,    0,    0,    0,    0,  204, 3838,    0, 2338,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1356,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2853,    0, 2314,    0,
    0,    0,    0,    0,    0, 1359,    0, 1359,    0,    0,
 1083,    0, 1234, 1298,    0,    0,    0,    0,  138,    0,
 2366,    0,    0,    0,    0,    0,    0,    0,  370,  591,
    0,    0,    0,  814,    0,    0,  889,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2350,    0,    0, 1198,    0, 2171,    0,    0,  704,    0,
    0,    0,    0,  157,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1402,    0,    0, 3229, 4621,    0,
    0,    0, 6269,    0,    0,13822,    0,    0,13875,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1748, 1784,    0,    0,    0,    0,13911,10945,
    0,13875,   82,    0,    0,    0,    0,    0,    0,    0,
    0,  575, 1235,    0,    0,    0,    0, 2229,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 6144, 6393,
 6539,    0,    0,  190,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2600,  106,
13575,    0,13575,    0,    0,    0,13575,13575,13575,    0,
    0,    0,    0,    0, 2198,    0,    0,    0,    0,    0,
 5027, 5316, 2601,    0,   15,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1594,    0,    0,    0,    0, 2374,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  214,    0,    0,    0,
    0,  214,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  640,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  856,    0,    0,    0,
    0,    0,  104,    0,    0,    0,  575,    0,  794,  399,
    0,    0,    0,    0, 1030,    0,    0,    0,    0,    0,
    0,    0, 1797,    0,    0,    0,    0,    0, 2607,    0,
    0, 4277, 2168,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  705,  748,  770,    0,    0,    0,  904,    0,
    0,    0,    0,    0,    0,    0,    0, 3529,    0,    0,
    0,    0,    0,13822,    0,    0,    0,    0,  589,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2366,    0,
    0,  283,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 8377,    0,    0,    0,  148,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1894,    0,    0, 1403,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2608,    0,    0,    0,    0,    0, 4138, 5605, 5894, 4438,
 4738,  998,    0,    0,    0,    0,    0,    0,    0,    0,
 5850,    0, 2378,    0,    0,    0,    0,    0,    0,    0,
    0, 1420, 1426,    0,    0,    0,    0,    0,    0,    0,
 1448,    0,    0,    0, 1448, 1463, 1478,    0,    0,    0,
    0,    0,    0,    0,    0,  943,  161,  277,    0,    0,
    0,    0,    0,    0,    0,    0,  765,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  106,  134,
    0,    0,    0,    0,    0,    0,  841,    0, 2179, 2202,
    0, 1171,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2374,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1010,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1481,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 3829,    0,    0,    0,    0,
    0,    0,    0,    0, 1145,    0,    0,    0,    0,    0,
    0,    0, 1235, 2374, 2179, 1519,  575,    0, 2386, 1524,
    0,    0,    0, 8377,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1556,    0,    0,    0, 6715,    0,
    0,    0,    0,    0,    0,    0,    0, 6745,    0, 6787,
    0, 6938,    0,    0,    0,    0,    0,    0,    0, 7021,
    0,    0, 7110,    0,    0, 7143,    0, 7185,    0,    0,
 7279,    0,    0, 7354,    0,    0, 1708,    0, 2600,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2224,    0,    0,
    0,    0,  719,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  497,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1420, 1530,
    0, 2226,  264,    0,    0,    0,    0,    0, 1532, 1533,
 1548,    0,    0, 2318, 1182,    0, 6511,    0, 2321,    0,
 2321, 2318, 2321,    0,    0,    0,  207,    0,    0,    0,
 1403, 2374,    0, 1524,    0,    0, 2211,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1885,    0, 1010, 2285,
    0,    0,    0,    0,    0,  931, 1007, 1011, 1019, 1074,
 1116,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  642,    0,    0,    0,    0,
 4244,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1558,    0,    0,    0,    0,    0, 1565,    0,    0,    0,
    0,    0,  164,    0,    0,    0,    0,    0,    0,    0,
  115,    0,  245,    0,    0,    0,    0,   15,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 7443,    0,
 7476,    0, 7518,    0,    0,    0,    0, 7648,    0,    0,
    0,    0,    0,    0, 1003,    0,    0, 2623,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2627,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 9292,
    0,    0,    0,    0,    0,    0,    0, 1459, 1477,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  552,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1627,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1573, 2078,    0,    0,
    0,  571,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  590,    0,    0,    0,    0,    0,    0,  903,    0,
    0,    0,    0,    0,    0,    0,    0, 2629, 1320, 2070,
    0, 2350,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2374,    0,    0,    0,    0,    0,    0,    0,
 1622,  881,    0,    0,    0,    0,    0,    0,    0,    0,
 2374, 2386,    0,  302,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  948,    0, 7676,    0,    0,    0, 2608,
    0, 7820,    0,    0,    0, 7880,    0,    0,    0,    0,
 7955,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2600,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2613,    0,    0,    0,    0,    0,    0,    0,    0,
 2617,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2381,    0,    0,    0,
    0,    0,    0,    0,  264,  897,    0,    0,    0,    0,
 2318,  246,  813,    0,  513,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 4315,    0,    0,  288,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  168,    0,    0,
    0,    0,    0, 8077,    0,    0,    0,    0, 8105, 8180,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1804, 2637,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   89,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 8257,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 4315, 1093,
    0,  194,    0,    0,    0,    0,    0,    0, 2629,    0,
    0,    0, 2646,    0,    0,    0,    0,    0,    0,    0,
 2630,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  512,    0,    0,    0,    0,    0,    0,
 8320,    0,    0,    0,  195,    0, 1010, 1269, 1322,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  512,
 2631,    0,    0,    0,    0,    0, 2387,    0,    0,    0,
    0, 4315,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2635,    0,    0, 8852,    0,
    0,    0,  512,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  512,    0,    0,  512,    0,    0,
    0,    0,    0,    0,    0,  512,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2701,    0,    0, 2702,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2713,
   -3,  622,   -8, -570, 1476,  746, -276,  -76, 2704, 2430,
    0,    8,    0, 2438, -103,  -40,    0, -568, -461,  -34,
  -62,   -6, -215, 2441, 1976,    0,    0,    0, 1249,    0,
    0,  -51, 1597, -561,  -68, -953, 2069, 1843, -324,    0,
 1604,    0, 2640, 1847, 1899,    0,  -63,    0, -517,  -50,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -536, -392, 1616,
 -338, 1055, -846,    0,    0,    0,    0, -336, -113, -854,
 1623, -358,  -55,    0, -550,    0,    0, 1201,-1024,    0,
 1199,    0,    0, 2396, -607,  -37,    0,    0,    0,    0,
 2370,    0, 2141, 1641,    0,    0,    0, 1710,    0, 2419,
 1970, 1638, 2397, 2399,    0, 1149,    0, -452, 1950,  -44,
 -634, 1099, -248,    0,    0, -932, -923, -903, -897, -894,
    0, 1248, -906,    0,  954,-1258, 1245,    0, 2196,    0,
    0,    0,  961,  967,  939, -124, -260, -956, -938,    0,
    0,    0, 1983,    0, -662,    0, 1805,    0,    0,    0,
    0,    0,-1180, -296,    0,    0,    0,    0, -201,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1061,    0,    0, -388,    0,    0,  -89,    0,
 -342,  279, 1358, 1162,    0,  995,    0,    0,    0,    0,
    0,    0,    0, 2071, -129, -460, 1527, 1297, -551, -773,
-1006,-1369,    0,    0,  -48,    0,    0,    0,    0,    0,
 2439,    0,    0,    0,    0, 2449, 2225,    0,    0,    0,
    0,    0,   -9, 2459,    0,    0,    0,  -10,    0,    0,
    0, -367, 1946, -361, 1480, -491,    0,    0,    0,    0,
 2206,    0, -987,    0,    0,    0, 2167,    0, 1957,    0,
    0,    0,    0,    0,    0,  -23,    0, 2799,    0,    0,
    0,    0,    0, 2497,    0,    0,    0,    0, 2291,    0,
 2289,    0,    0,    0, 2040,    0,    0,    0,    0,    0,
 -267, -619, -853, 3220,    0, 1401,    0,    0,    0,    0,
    0,    0, -298,    0,    0, -747, -207, 1212,    0,    0,
    0, 2672, 1240,    0, 2072,-1072, -963,    0, -960, 1819,
-1188,    0,    0,    0,    0, -144,    0,    0,    0,    0,
 1574,    0,    0,    0,    0, 2757,  543,    0, 1081, 2156,
    0, 1254,    0, 1454,    0,  -13,    0,    0, 2059,    0,
    0,    0,    0,    0, 2323,    0,    0,    0, -462, 2106,
 1855,-1059,    0,    0,    0, -712, -454,  176,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1404, 1698,    0,    0,    0, -888,    0,    0,
    0, 1581,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -984, 1653,    0,
    0,
};
#define YYTABLESIZE 14681
static short yytable[] = {                                     135,
   81,   38,  135,  228,  277,  156,  155,  205,  162,  789,
  224,  325,  569,  220,  633,  976,  297,  300,  208,  603,
  634,  225,  237,  154,  259,  265,  729,  811,  571,  233,
  769,  790,  296,  299,  268, 1039,  748,  597,  240,  751,
 1524,  263, 1296, 1227, 1429,  237,  202,  204,  207,  210,
  155,  215,  264,  262, 1254,  726,  226,  204,  231,  270,
  210,  267,  155,  215,  493,  244,  135,  211, 1037,  245,
  925, 1190,  980,  493,  255,  156,  155,  257,  204,  238,
  255,  231,  207,  210,  266,  155,  215,  244,  135,  514,
  210,  269,  283,  154,  285,  961,  950,  331,  887,  576,
  714,  878,  958,  705, 1119,  726, 1294, 1219,  387, 1093,
  573,  159, 1343,  307,  767,  574, 1124,  327,  802,   80,
  803,  745,  281, 1285,  537,  813,  647,  226,  155,  226,
  782,  687, 1286,  753, 1285, 1236, 1141,  703,  394, 1099,
  333,  231, 1321, 1286,  767,  284,  324,  327,  210,  155,
  244,  356, 1287, 1043,  162,  789,  398, 1338, 1288,  160,
  656, 1289,  705, 1287,  726,  295,  329, 1290, 1060, 1288,
  159, 1063, 1289, 1218,  574,  715,  383,  753, 1290,  281,
 1070,  538, 1006, 1481, 1491, 1693,  294, 1805,   87,  702,
  687,   87,  753,  247,  247,  778,  703, 1561,  794,  574,
  130,  656,  541,  325,   95, 1697,  701,  918,   97,  295,
  633,  100,  892,  758, 1293,  398,  634,  650,  160,  656,
  865, 1297, 1415, 1059, 1513, 1417,  478,  543,  754,  486,
  702, 1641, 1305,  930,  247,  247, 1114,  247,  247,  362,
  783, 1661,  819,  715, 1049,  699,  603,  701,  702,  846,
  298,  555,  247,  247,   80, 1326, 1483,  523,  574, 1627,
 1628,  226,  325,  525,  226,  701, 1515, 1516,  155, 1798,
  495,  135,  758,  295,  388,  743,  657,  155,  504,  495,
  786,  536,  133,   63,  137,  791,  699,  767,  918,  699,
  859,  357, 1371,  893,  918,  526,  301, 1492,  207, 1798,
 1539,  186,  155,  139,  699,  734,   14,  570,  733, 1650,
 1186, 1187,   86, 1050,  478, 1504,  225,  657,  584,  530,
  657,  586,  525,  598,  919,  600,  218,  219,  767,  164,
  101,  767, 1537,  750,  893,  657,  302,  893, 1698,  651,
 1372,  133,  226,  643,  655,  259,  767, 1215,  138,  226,
  645,  226,  893,  226,  538, 1517,  589, 1139, 1285, 1285,
  186, 1642,   80, 1100,  358,  389, 1308, 1286, 1286,  732,
  130,  130, 1207,  619,  210,  215,  964, 1549,  362, 1077,
  295, 1728, 1562,  155,  973,  207,  893, 1287, 1287,  664,
 1522,  918, 1285, 1288, 1288,  661, 1289, 1289,  736, 1553,
  653, 1286, 1290, 1290,  715,  919, 1272, 1821,  159,  726,
  732,  919, 1771,  732,  619, 1646,  556, 1503, 1120, 1271,
  398, 1287, 1676, 1291, 1292, 1073, 1239, 1288,  732, 1136,
 1289, 1605,  619, 1137,  718, 1608, 1290, 1249, 1251,  736,
 1138, 1794,  736, 1304, 1804,  918,  528, 1281, 1739,  742,
  726,  301,  289, 1420,  536,  159,  160,  736, 1054, 1058,
 1757, 1056,  918, 1123,  742, 1027,  777, 1534,  726,   95,
  247,  247, 1066,   97, 1140,  656,  767,  155,  247, 1010,
  210,  495,  839,  705,  259,  656,  757,  656,  155, 1861,
  327,  929,  155,  789,  739,  210,  656,  155,  755, 1084,
  162,  486, 1309,  160,  702,  747,  280,  795,  919,  752,
 1814,  687,  700,  753,  702, 1092,  702,  703,  869,  293,
  687,  701,  753,  744,   79,  702,  703,  726,  840,  493,
  400, 1360, 1361,  815, 1362, 1363,  226, 1235, 1651, 1246,
  656,  687,  701,  574,  293, 1690,  766,  841,  237,  656,
   87, 1011, 1092,  700, 1253,  573, 1886, 1126, 1845, 1424,
  699, 1337,  919, 1552,  887, 1657,  247,  808,  226,  702,
  658,  700,  699,  808,  758,  687, 1683,  753,  702,  919,
 1038,  699, 1274, 1867, 1347, 1754,  701,  247,  143,  705,
  725,  657,  656,  247, 1689,  701,  293,  872,  873,  739,
  574,  657,  977,  657,  656,  325,  656,  225, 1112, 1617,
 1699,  658,  657, 1522,  658,  692,  705, 1067,  726,  864,
  159,  702, 1726,  791,  525,  699,  133,  247,  920,  658,
  705,  725,  845,  141,  699,  702,  862,  538,  701,  703,
  739,  144,  226,  918,  687,  186,  753,  143,  705,  725,
  703,  921,  701, 1030, 1660, 1838,  657,  932,  739,   79,
  293, 1247,  918,  767, 1843,  657, 1662, 1473,  160, 1475,
 1663, 1664, 1512,  656,  478, 1310, 1007,  699, 1298, 1299,
  703, 1487,  601,  123,  732,  231,  247,  247,  478,  857,
  478,  699,  985,  165, 1526,  893, 1887,  384,  703, 1692,
  144, 1551,  702, 1788,  384,  732, 1691,  922,  657,  619,
  966, 1311, 1276,  736, 1618,  742, 1703,  123,  226,  701,
  657, 1242,  657,  226,  726,  226, 1295,  226, 1348,  555,
  495,  623,  385,  478,  736,  495, 1306,  536, 1303,  833,
  845, 1048,  225,  495, 1775,  726,  210,  382, 1570,  166,
 1776,  123, 1013,  210, 1656,  858, 1142,  142,  699,  478,
  919, 1075, 1325,  384,  696,  290,  155,   79,  155,  383,
 1229,  226,  623, 1232, 1474,  834, 1476,  226, 1339,  919,
  767,  237, 1525,  530,  621, 1036, 1076, 1078, 1083,  657,
  623, 1089, 1744,  620,  226, 1285, 1109, 1110, 1111, 1126,
 1844,  289,  724, 1097, 1286,  696,  382,  725, 1367,  726,
 1764,  337,  695,  737, 1765,  732,  237,  237,  237,  619,
  861,  226,  226,  696, 1287,  621,  337,  700,  383, 1774,
 1288, 1538, 1243, 1289,  620,  664, 1307, 1548, 1745, 1290,
  738, 1090,  893,  621,  736, 1839, 1778,  900,  700, 1746,
  232, 1599,  620,  695,  737,  726,  287,  538, 1221, 1868,
  155,  953,  954,  732,  143,  143,  337,  770, 1402,  576,
  155,  695,  737, 1143, 1422,  835,  918, 1113,  664, 1135,
  145,  738,  732, 1368,  661,  658,  313, 1121,  471,  291,
  232, 1751,  700, 1655,  602,  658,  666,  658, 1869,  738,
 1880,  700,  186,  186,  705,  725,  658,  724,  722,  692,
  723,  736,  725, 1490,  739,  216,  103,  370,  144, 1130,
 1240,  232,  726, 1210, 1211,  705,  725, 1233,  576, 1820,
  389, 1250, 1822,  597,  772,  739,  666,  666,  478,  145,
  666,  337,  684,   79,  700, 1870, 1230,  471, 1747,  378,
  658,  151,  105, 1231,  703,  666,  337,  536,  700,  658,
  143,  186,  186,  168,  703,  250,  703,  232,  384,  705,
  816,  367, 1612, 1533, 1850,  703,  495,  210,  215,  495,
  244,  135,  794,  684, 1234, 1030,  684,  495,  527,  389,
   14,  527,  597,  919, 1131, 1268,  337,  894,  495,  495,
  955,  684,  658,  773, 1555,  210,  390,  232,  378,  241,
  386,  382, 1736,  567,  658,  169,  658,  106,  385,  703,
  252,  705,  970,  970, 1474,  700,  568,  107,  703,  177,
  293, 1355,  260,  383,  633,  705,  725,  720,  894, 1332,
  634,  894,  721,  271,  368,  739, 1269,  817, 1259,  273,
  241,  152,  108,  241, 1350,  338,  894, 1029,  153,  237,
 1354, 1260, 1212, 1356,  478,  390,  893,  623,  241,  386,
  893,  703,  225,  388,  237,  572,  486,  385, 1316,  696,
  237,  538,  603,  658, 1261,  703,  976,  818,  177, 1317,
  894,  696,  767,  988,  384,  339, 1316, 1368, 1368, 1015,
  696,  274,  705,  725,  581, 1368, 1368,  226, 1368, 1368,
  278,  226, 1478, 1478, 1482,  387, 1368, 1368, 1368, 1368,
  621, 1368, 1368, 1368, 1368, 1368, 1368,  695,  737,  620,
  590,  429,  388,  767,  572, 1263,  767,  382, 1327,  695,
  591,  603,  720,  526,  696, 1808,  471,  721,  695,  737,
 1811,  767,  703,  696, 1636,  738,  370,  145,  471,  383,
  186,  970, 1509,  581, 1511,  988,  720,  279, 1327, 1425,
  178,  721,  186,  282,  387, 1506,  738,  478, 1395,  155,
  225,  536, 1668,  819,  286, 1507,  578, 1328,  502,  349,
 1154,  471,  695,  471,  389,  287,  696,  465, 1882,  186,
  186,  695,  327, 1669,  580,  186,  684,  186,  378, 1670,
  696,  666,  226,  378,  666, 1430, 1471, 1331,  378, 1695,
  527,  666, 1164,  666,  567, 1554,  495,  666,  495,  178,
  621,  405,  666,  613,  350,  666, 1165,  502, 1155,  620,
  527,  465,  495, 1671,  695,  578,  186,  186,  210, 1883,
  666,  288,  606,  527,  186,  186,  465,  684,  695,  737,
  684,  327, 1156,  580,  666,  292, 1259,  684,  243,  684,
  390,  597,  666,  684,  386, 1841,  666,  696,  684, 1260,
 1166,  684,  385,  567,  406,  666,  738,  177,  772,  186,
  405,  186,  613,  792,  528,  527,  684,  598,  334,  177,
 1560,  666, 1261,  606,  597, 1565,  538,  402, 1167,  243,
  684,  529,  243,  186, 1536,  772, 1262, 1595,  684,  207,
  389,  244,  684,  186,  447,  695,  177,  243,  666, 1798,
  597,  684,  177,  597,  177,  716,  147,  388,  186,  378,
  666,  351,  666,  406, 1797,  168, 1648,  684,  512,  597,
  527, 1777, 1596, 1263,  308,   44,  598,  517,  602,  352,
  207, 1033,  244,  207, 1597,  244,  402, 1616,  767,  767,
  716,  326, 1168,  177,  684,  597,  767, 1034,  207,  387,
  244,  247,  716, 1798, 1395,  155,  684,  301,  684,  666,
   52,  549,  301,  353,  684,  554,  390,  341, 1858,  894,
  386,  169,  739, 1829,  168,  527,  536, 1831,  385,  666,
  335,  186,  186,  666,   44,  248,  337,  602, 1837,   42,
  603,  204,  359,  684,  684,   25,  684,  926,  178, 1626,
 1135, 1135,  934,  684,  684,  684,  684,  684, 1856,  684,
  178, 1471,  684,  684,  684,  684,  684,  763,  370,   52,
  684,  684,  301,  603,  301,  684,  684,  684,  684,  684,
  169,  739,   48,  388,  767, 1264, 1874,  178,  767,  724,
  722, 1876,  723,  178,  725,  178,  495,   50,   42,  603,
  480,  324,  603,  361,   25,  767,  362, 1743,  210,  210,
  705,  767,  936,  378,  938,  711,  712,  713,  603,  659,
  398, 1756,  372,  378, 1010,  387,  763, 1834,  403,  515,
  381,  109,  659,  155,  178, 1478, 1010,  521,  135, 1010,
  659,   48, 1015,  373,  603,  767,   83,   84,  363,   26,
 1674,   49,   51, 1859, 1015,  659,   50, 1015,  177,  480,
  110,  111, 1741,  112,  243,  243, 1723,   46,  391,  659,
  113,  114,  243,  115,  116,  843,  117,  136,  393,  118,
  119,  120,  121,  122,  124,  758, 1879,  123,  124,  760,
 1772,  613,   47,  125,  126,  127, 1734,  135, 1885,  309,
  343, 1888,  373, 1395,  155,  767,  659,  207,   26, 1889,
   49,   51,  344,  527,  146,  147,  843,  244,  244,  843,
  419,  420,  421,  395,  613,  244,   46,  210,  724,  722,
  396,  723,  310,  725,  843,  399,  136,  724,  722, 1135,
  723,  207,  725,  124, 1773,  345,  400,  346,  429,  430,
  613,   47,  483,  613,  527,  598,  324,  527,  311,  210,
  243,  312, 1752,  738, 1780,   80,  737, 1743, 1782,  613,
  490, 1743,  527, 1743,  527,  966,  826,  313,  827,  538,
  155,  243, 1743,  218,  219,  207, 1815,  243,  598,  373,
 1085, 1086, 1001,  724,  722,  613,  723, 1770,  725,  178,
  207, 1800, 1743,  314,  998,  538,  805,  988,  158,  804,
  131,  131,  960,  244,  598,  726,  602,  598,  659,  498,
  499,  243, 1741,  374,  720,  375, 1741,  785, 1832,  721,
 1743,  502,  207,  598,  244, 1743,  507, 1741,  538,  159,
  244, 1003, 1118,  503, 1002, 1117,  899,  376, 1122, 1220,
  538,  883,  726,  538,  160,  204, 1802, 1741,  552,  598,
  553,  538, 1223, 1803, 1206, 1222,  928,  504,  785,  505,
  933,  785,  377, 1273,  244,  602, 1117,  949,  602,  536,
  508,  243,  956,  957,  509, 1741,  785,  486, 1319, 1324,
 1741, 1318, 1323,  447,  602,  967,  968,  969,  970,  971,
  972,  447,  524,  527, 1485,  536,  525,  726,  504,  226,
 1826,  504, 1569, 1610, 1471,  726,  726,  522, 1647, 1013,
  602,  726,  226, 1495,  534, 1497,  504,  724,  722, 1852,
  723, 1853,  725,  207,  244,  207,  544,  843,  536,  517,
 1471,  546, 1865, 1866,  527, 1864,  737,  527,  548,   14,
  536,  724,  722,  536,  723, 1884,  725,  368, 1798, 1040,
  368,  536,  527,  720,  724,  549, 1047,  724,  721,  560,
  562,  527,  720, 1471, 1013,  561,  582,  721,  724,  722,
 1008,  723,  564,  725,  585, 1471,  527, 1135, 1471,  595,
  843,  599,  648,  843,  652, 1793, 1471,  654,  656,  689,
  843, 1603,  843,  691,  220,  692,  843,  693,  694,  695,
  527,  843,  724,  722,  843,  723,  527,  725,  696,  527,
  984, 1008, 1008, 1008, 1008, 1008, 1008, 1008,  720,  843,
 1742, 1860,  697,  721,  698,  699,  700,  701,  702, 1008,
 1008, 1008, 1008,  843,  726,  220,  703,  708,  220,  709,
  927,  843,  931,  710,  843,  843,  935,  937,  939, 1375,
  719,  527,  727,  220,  843,  728,  732,  735, 1259,  736,
  740, 1008,  753, 1008,  745,  756,  763, 1144, 1145, 1149,
  843, 1260, 1158, 1160, 1162,  765, 1170, 1173, 1176, 1178,
 1181, 1184,  724,  722,  780,  723,  776,  725,  785,  781,
 1191, 1192,  843, 1194, 1261, 1196,  787,  843, 1199, 1200,
 1201, 1202, 1203, 1204,  788,  791,  527,  793, 1262,  843,
  555,  843,  796,  527,  806, 1208, 1758, 1213,  797,  759,
  724,  722,  799,  723,  800,  725,  831,  830,  832,  837,
  293,  527,  785,  504,  504,  785,  843,  852,  504,  761,
  842,  504,  785,  849,  785, 1263,  850, 1015,  785,  851,
  854,  855,  720,  785,  504,  860,  785,  721,  843,  866,
   79,  527,  109,  867,  247,  527,  870,  871,  447,  527,
  527,  785,  880,  883,  527,  882,  720,  527,  843,  221,
 1742,  721,  843,  274, 1742,  785, 1833,  655,  888,  889,
  527,  110,  111,  785,  112, 1742,  785,  785,  248,  890,
  894,  113,  114,  720,  115,  116,  785,  117,  721,  168,
  118,  119,  120,  121,  122, 1742,  951,  962,  123,  124,
  221,  861,  785,  221,  125,  126,  127,  655,  655,  504,
  295,  655, 1008, 1759, 1008, 1008,  981,  720,  221,  998,
  997, 1008,  721, 1742,  785, 1000,  655, 1008, 1742,  785,
  504, 1004, 1008, 1008, 1009, 1008,  504, 1264, 1031, 1026,
  737,  785, 1792,  785, 1041,  527, 1055, 1065, 1580, 1008,
  232,  220, 1044, 1067, 1008, 1008, 1080,  494, 1008, 1064,
 1074, 1071,  809, 1072, 1081, 1008,  527, 1008, 1008, 1082,
  504, 1008,  527, 1088, 1095, 1008, 1108, 1008, 1106, 1008,
 1129, 1008, 1101, 1115, 1193,   55, 1008, 1008, 1151, 1008,
  785, 1188, 1189, 1197, 1195,  447,   56,  720,  494, 1205,
 1224,  494,  721,   57, 1008, 1008,  527, 1008, 1378, 1379,
  785, 1381, 1383, 1226,  785,  720,  494, 1238, 1008, 1388,
 1008, 1241, 1008, 1008, 1008, 1236, 1008,   58, 1244, 1008,
 1008, 1252, 1256, 1258, 1266,  720, 1277, 1280, 1302, 1008,
  721, 1406, 1300, 1312, 1320, 1322,  232, 1333, 1329, 1413,
 1414, 1213, 1008, 1334, 1213, 1008, 1208, 1335, 1336, 1344,
 1421,  447, 1423, 1352, 1358, 1123, 1357,  232, 1359, 1365,
 1366, 1008, 1370,  232, 1373, 1374, 1376, 1008, 1008, 1377,
 1384, 1385, 1008, 1386, 1387, 1389, 1390, 1391,  102,   59,
 1392, 1393, 1394, 1008, 1008,  918, 1008, 1419, 1416, 1472,
  447, 1159, 1008, 1163,  809, 1171, 1174,  232, 1179, 1182,
 1185,  959,  724,  722, 1488,  723, 1486,  725, 1489,  761,
   60, 1493, 1498, 1499,   61, 1500,  103,  988, 1501, 1510,
  577, 1008, 1008, 1523, 1008,  232,  221, 1092,  104, 1518,
 1527, 1008, 1008, 1008, 1008, 1008, 1528, 1008, 1529, 1535,
 1008, 1008, 1008, 1008, 1008, 1530, 1550, 1531, 1008, 1008,
 1532, 1540,  105, 1008, 1008, 1008, 1008, 1008,  988,  988,
  988,  988,  988, 1001,  988, 1012,  724,  722, 1556,  723,
 1558,  725,  655, 1559, 1564,  655,  988,  988,  988,  988,
 1571, 1573,  655, 1593,  655, 1594,   14, 1600,  655, 1606,
 1607, 1614, 1620,  655, 1855, 1613,  655, 1619, 1566,  138,
 1634, 1632, 1622, 1623, 1572, 1644, 1630, 1631,  988, 1576,
  988,  655,  444, 1645, 1633, 1639, 1581,  106, 1640, 1643,
 1675,  232, 1649,  494,  494,  655, 1665,  107, 1677,  109,
 1679,  494, 1678,  655, 1198,  724,  722,  655,  723, 1680,
  725, 1681,  232, 1682,  494, 1684,  655, 1685,  232, 1694,
 1213, 1686,  108, 1696, 1213, 1700, 1701, 1702,  110,  111,
 1705,  112,  655, 1706, 1707,  109, 1708, 1711,  113,  114,
 1621,  115,  116, 1712,  117, 1713, 1714,  118,  119,  120,
  121,  122,  232, 1715, 1716,  123,  124, 1717, 1718,  655,
 1719,  125,  126,  127,  110,  111, 1720,  112, 1722, 1731,
 1725,  655, 1635,  655,  113,  114, 1753,  115,  116,  655,
  117, 1738, 1763,  118,  119,  120,  121,  122, 1766,  494,
 1768,  123,  124, 1769, 1781, 1785, 1789,  125,  126,  127,
  724,  722, 1216,  723, 1652,  725, 1796,  720,  655,  655,
  494,  655,  721, 1795, 1799, 1807,  494, 1806,  655,  655,
  655,  655,  655, 1809,  655, 1812, 1823,  655,  655,  655,
  655,  655, 1816, 1824, 1825,  655,  655, 1827, 1851, 1846,
  655,  655,  655,  655,  655, 1830, 1854, 1848, 1857,  988,
  494,  988,  988, 1798, 1863, 1843, 1844, 1873,  988, 1875,
  444, 1877, 1878,    1,  988, 1881,  193,  732,   30,  988,
  982,  720,  988,   32,  767,  370,  721, 1704,  156,  261,
  648,   82,  462, 1709, 1710,  975,  988, 1012,  988,  711,
  969,  988,  988,  272,  893,  988,  203,  271,  739,  398,
  968,  183,  988,  525,  988,  988,  692, 1724,  988,  694,
  184, 1213,  988,  712,  988, 1727,  988,  949,  988,  371,
 1733,  213, 1735,  988,  988,  322,  988,  948, 1012, 1012,
 1012, 1012, 1012, 1012, 1012,  288,  721,  342,  330,  357,
  720,  988,  988,  328,  988,  721, 1012, 1012, 1012, 1012,
   27,   30, 1400,  724,  722,  988,  723,  988,  725,  988,
  988,  988,   40,  988, 1477,  143,  988,  988,  506,  497,
  999,  496, 1353,  975,  881, 1128,  988, 1351, 1012,  261,
 1012, 1401,  724,  722, 1133,  723, 1349,  725, 1341,  988,
 1688, 1687,  988, 1091,  566,  844, 1330,  542,  607, 1779,
  724,  722, 1217,  723, 1342,  725, 1042,  579,  988,  580,
 1729, 1279, 1069, 1767,  988,  988, 1673, 1667, 1842,  988,
  940,  941,  942,  943,  944,  945,  946,  947,  825, 1849,
  988,  988, 1872,  988, 1847,  720, 1062, 1228,  447,  988,
  721, 1404,  724,  722, 1791,  723, 1625,  725, 1750, 1835,
 1505, 1658,  572,  978,  587,  563, 1547, 1107,  868,  829,
 1098,  145,  535,  848,  771, 1032,  775, 1652,  988,  988,
  306,  988,  724,  722, 1611,  723,  109,  725,  988,  988,
  988,  988,  988, 1255,  988, 1496, 1028,  988,  988,  988,
  988,  988,  256, 1813,  923,  988,  988, 1721, 1592, 1046,
  988,  988,  988,  988,  988,  110,  111, 1005,  112, 1248,
 1836,  784, 1514, 1380, 1418,  113,  114,    0,  115,  116,
    0,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,    0,  974,    0,  125,  126,
  127,    0,    0,    0,    0,    0,  652,    0,    0, 1012,
    0, 1012, 1012,    0,    0,    0,    0,    0, 1012,    0,
    0,    0,    0,    0, 1012,    0,    0,    0,    0, 1012,
    0,    0, 1012, 1405,  724,  722,    0,  723,    0,  725,
    0,    0,    0,    0,    0,    0, 1012, 1017,  720,    0,
    0, 1012, 1012,  721,    0, 1012,    0,    0,    0,    0,
    0,    0, 1012,    0, 1012, 1012,    0,    0, 1012,    0,
    0,    0, 1012,    0, 1012,    0, 1012,  720, 1012,    0,
    0,    0,  721, 1012, 1012,    0, 1012,    0, 1017, 1017,
 1017, 1017, 1017, 1017, 1017,  720,    0,    0,    0,    0,
  721, 1012, 1012,    0, 1012,    0, 1017, 1017, 1017, 1017,
    0,    0,    0,    0,  109, 1012,    0, 1012,    0, 1012,
 1012, 1012,    0, 1012,    0,    0, 1012, 1012,    0,    0,
    0,    0,    0,    0,    0,    0, 1012,  720, 1017,    0,
 1017,    0,  721,  110,  111,    0,  112,    0,    0, 1012,
 1615,    0, 1012,  113,  114,  109,  115,  116,    0,  117,
    0,    0,  118,  119,  120,  121,  122,  720, 1012,    0,
  123,  124,  721,    0, 1012, 1012,  125,  126,  127, 1012,
    0,    0,    0,    0,  110,  111,    0,  112,    0,    0,
 1012, 1012,    0, 1012,  113,  114,    0,  115,  116, 1012,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,    0,    0,    0,    0,  125,  126,  127,
  724,  722,    0,  723,    0,  725,    0,    0, 1012, 1012,
    0, 1012,    0,    0,    0,    0,    0, 1790, 1012, 1012,
 1012, 1012, 1012,  109, 1012,    0,    0, 1012, 1012, 1012,
 1012, 1012,    0,    0,    0, 1012, 1012,    0,    0,    0,
 1012, 1012, 1012, 1012, 1012,    0,    0,    0,    0,  720,
    0,    0,  110,  111,  721,  112,    0,    0,    0,  652,
    0,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  126,  127,    0, 1017,
    0, 1017, 1017,    0,    0,    0,    0,    0, 1017,    0,
    0,    0,    0,    0, 1017,    0,    0,    0,    0, 1017,
    0,    0, 1017, 1407,  724,  722,    0,  723,  525,  725,
    0,    0,    0,    0,    0,    0, 1017, 1013,    0,    0,
    0, 1017, 1017,    0,    0, 1017,    0,    0,    0,    0,
    0,    0, 1017,    0, 1017, 1017,    0,    0, 1017,    0,
    0,    0, 1017,    0, 1017,    0, 1017,    0, 1017,  525,
    0,    0,  525, 1017, 1017,    0, 1017,    0, 1013, 1013,
 1013, 1013, 1013, 1013, 1013,    0,    0,  525,    0,  525,
    0, 1017, 1017,    0, 1017,    0, 1013, 1013, 1013, 1013,
    0,    0,    0,    0,  655, 1017,    0, 1017,    0, 1017,
 1017, 1017,    0, 1017,    0,    0, 1017, 1017,    0,    0,
    0,    0,    0,    0,    0,    0, 1017,    0, 1013,    0,
 1013,    0,    0,  655,  655,    0,  655,    0,    0, 1017,
 1624,    0, 1017,  655,  655,  720,  655,  655,    0,  655,
  721,    0,  655,  655,  655,  655,  655,    0, 1017,    0,
  655,  655,    0,    0, 1017, 1017,  655,  655,  655, 1017,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1017, 1017,    0, 1017, 1408,  724,  722,    0,  723, 1017,
  725,    0,    0, 1409,  724,  722,    0,  723,    0,  725,
    0,    0,    0,    0, 1410,  724,  722,    0,  723,    0,
  725, 1411,  724,  722,    0,  723,    0,  725, 1017, 1017,
    0, 1017,  724,  722,    0,  723,    0,  725, 1017, 1017,
 1017, 1017, 1017,  109, 1017,    0,    0, 1017, 1017, 1017,
 1017, 1017,    0,    0,    0, 1017, 1017,    0,    0,    0,
 1017, 1017, 1017, 1017, 1017,    0,    0,    0,    0,  720,
    0,    0,  110,  111,  721,  112,    0,    0,    0,    0,
    0,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,  525,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  126,  127,    0, 1013,
    0, 1013, 1013,  525,  525,  525,    0,    0, 1013,  525,
    0,    0,  525,    0, 1013,    0,    0,    0,    0, 1013,
    0,    0, 1013,    0,    0,  525,    0,    0,  522,  724,
  722,  525,  723,    0,  725,    0, 1013, 1026,    0,    0,
    0, 1013, 1013,    0,    0, 1013,    0,    0,    0,    0,
    0,    0, 1013,    0, 1013, 1013,    0,    0, 1013,    0,
    0,    0, 1013,    0, 1013,    0, 1013,    0, 1013,  522,
    0,    0,  522, 1013, 1013,    0, 1013,    0, 1026, 1026,
 1026, 1026, 1026, 1026, 1026,    0,    0,  522,    0,  522,
    0, 1013, 1013,    0, 1013,    0, 1026, 1026, 1026, 1026,
  525,    0, 1412,  724,  722, 1013,  723, 1013,  725, 1013,
 1013, 1013,    0, 1013,    0,    0, 1013, 1013,    0,    0,
  720,  525,    0,    0,    0,  721, 1013,  525, 1026,  720,
 1026,    0,    0,    0,  721,    0,  525,    0,  525, 1013,
  720,    0, 1013,    0,    0,  721,    0,  720,    0, 1601,
  724,  722,  721,  723,    0,  725,  525,  720, 1013,    0,
    0,  525,  721,    0, 1013, 1013,    0,    0,    0, 1013,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1013, 1013,    0, 1013,    0,    0,    0,    0,    0, 1013,
  525,    0, 1602,  724,  722,    0,  723,    0,  725, 1609,
  724,  722,    0,  723,    0,  725,    0, 1787,  724,  722,
    0,  723,    0,  725,    0,    0,    0,    0, 1013, 1013,
    0, 1013,    0,    0,    0,    0,    0,    0, 1013, 1013,
 1013, 1013, 1013,  109, 1013,    0,    0, 1013, 1013, 1013,
 1013, 1013,    0,    0,    0, 1013, 1013,    0,    0, 1364,
 1013, 1013, 1013, 1013, 1013,    0,  724,  722,    0,  723,
    0,  725,  110,  111,  720,  112,    0,    0,    0,  721,
    0,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,  522,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  126,  127,    0, 1026,
    0, 1026, 1026,  522,  522,  522,    0,    0, 1026,  522,
    0,    0,  522,    0, 1026,    0,    0,    0,    0, 1026,
    0,    0, 1026,    0,    0,  522,    0,    0,  527,    0,
    0,  522,    0,    0,    0,    0, 1026,  862,  720,    0,
    0, 1026, 1026,  721,    0, 1026,    0,    0,    0,    0,
    0,    0, 1026,    0, 1026, 1026,    0,    0, 1026,  577,
    0,    0, 1026,    0, 1026,    0, 1026,    0, 1026,  527,
    0,    0,  527, 1026, 1026,   65, 1026,    0,  862,  862,
  862,  862,  862,    0,  862,  720,   66,  527,    0,  527,
  721, 1026, 1026,   67, 1026,    0,  862,  862,  862,  862,
  522,    0,    0,    0,    0, 1026,   68, 1026,    0, 1026,
 1026, 1026,    0, 1026,    0,    0, 1026, 1026,    0,    0,
    0,  522,    0,    0,    0,    0, 1026,  522,  720,    0,
  862,    0,    0,  721,    0,  720,    0,    0,  522, 1026,
  721,    0, 1026,  720,    0,    0,    0,    0,  721,    0,
    0,    0,    0,    0,    0,    0,  522,    0, 1026,    0,
    0,  522,    0,   69, 1026, 1026,    0,    0,    0, 1026,
    0,    0,    0,    0,    0,    0, 1579,    0,    0,   70,
 1026, 1026, 1282, 1026,    0,    0,    0,    0,    0, 1026,
  522,  720,    0,    0,    0,   71,  721,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   72,    0,    0,    0,   73,    0,    0,   74, 1026, 1026,
    0, 1026,    0,    0,    0,   75,    0,  109, 1026, 1026,
 1026, 1026, 1026,    0, 1026,    0,    0, 1026, 1026, 1026,
 1026, 1026,    0,    0,   76, 1026, 1026,    0,    0,    0,
 1026, 1026, 1026, 1026, 1026,    0,  110,  111,    0,  112,
    0,    0,    0,    0,    0,    0,  113,  114,    0,  115,
  116,    0,  117,    0,    0,  118,  119,  120,  121,  122,
    0,    0,    0,  123,  124,    0,  527,    0,    0,  125,
  126,  127,    0,    0,    0,    0,    0,    0,    0,  862,
    0,  862,  862,    0,  527,  527,    0,    0,  862,  527,
    0,    0,  527,    0,  862,    0,    0,    0,    0,  862,
    0,    0,  862,    0,    0,  527,    0,    0,    0,    0,
    0,  527,    0,    0,    0,    0,  862,  874,    0,    0,
    0,  862,  862,    0,    0,  862,    0,    0,    0,    0,
    0,    0,  862,    0,  862,  862,    0,    0,  862,    0,
    0,    0,  862,    0,  862,    0,  862,    0,  862,    0,
    0,    0,    0,  862,  862,    0,  862,    0,  874,  874,
  874,  874,  874,    0,  874,    0,    0,    0,    0,    0,
    0,  862,  862,    0,  862,    0,  874,  874,  874,  874,
  527,    0,    0,    0,    0,  862,    0,  862,    0,  862,
  862,  862,    0,  862,    0,    0,  862,  862,    0,    0,
    0,  527,    0,    0,    0,    0,  862,  527,    0,  407,
  874,    0,    0,    0,    0,    0,    0,    0,  527,  862,
   87,    0,  862,  527,    0,    0,    0,   88,    0,    0,
    0,   89,   90,    0,    0,    0,  527,  409,  862,    0,
   91,  527,    0,    0,  862,  862,    0,    0,    0,  862,
    0,   92,    0,    0,    0,    0,  213,    0,    0,    0,
  862,  862,    0,  862,  527,  411,    0,  527,    0,  862,
  527,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  527,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  241,    0,    0,  213,  862,  862,
  213,  862,    0,    0,    0,    0,    0,    0,  862,  862,
  862,  862,  862,   93,  862,  213,    0,  862,  862,  862,
  862,  862,    0,    0,    0,  862,  862,    0,    0,   94,
  862,  862,  862,  862,  862,  241,  416,    0,  241,    0,
    0,    0,    0,    0,   95,    0,    0,    0,   96,    0,
    0,    0,  418,  241,    0,    0,    0,    0,    0,   97,
    0,    0,    0,    0,    0,    0,  419,  420,  421,    0,
  218,  219,    0,    0,    0,    0,    0,    0,   98,  874,
    0,  874,  874,    0,    0,    0,    0,    0,  874,  424,
  425,  426,  427,  428,  429,  430,  431,  432,    0,  874,
    0,    0,  874,    0,  434,  435,    0,    0,    0,    0,
    0,    0,    0,  439,    0,    0,  874,  877,    0,    0,
    0,  874,  874,    0,    0,  874,    0,    0,    0,    0,
    0,    0,  874,    0,  874,  874,    0,    0,  874,    0,
    0,    0,  874,    0,  874,    0,  874,    0,  874,    0,
    0,    0,    0,  874,  874,    0,  874,    0,  877,  877,
  877,  877,  877,    0,  877,    0,    0,    0,    0,    0,
    0,  874,  874,    0,  874,    0,  877,  877,  877,  877,
    0,    0,    0,    0,    0,  874,    0,  874,    0,  874,
  874,  874,    0,  874,    0,    0,  874,  874,    0,  527,
  527,    0,    0,    0,    0,    0,  874,  527,    0,    0,
  877,    0,    0,    0,    0,    0,    0,    0,    0,  874,
  527,    0,  874,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  213,  213,    0,    0,    0,    0,  874,    0,
  213,    0,    0,    0,  874,  874,    0,    0,    0,  874,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  874,  874,    0,  874, 1567, 1568,    0,    0,    0,  874,
  241,  241, 1574, 1575,    0, 1577, 1578,    0,  241,    0,
    0,    0,    0, 1582, 1583, 1584, 1585,    0, 1586, 1587,
 1588, 1589, 1590, 1591,    0,  527,    0,    0,  874,  874,
  537,  874,    0,    0,    0,    0,    0,    0,  874,  874,
  874,  874,  874,    0,  874,    0,  527,  874,  874,  874,
  874,  874,  527,    0,    0,  874,  874,    0,  213,    0,
  874,  874,  874,  874,  874,    0,    0,    0,    0,    0,
  537,  537,    0,    0,  537,    0,    0,    0,    0,  213,
    0,    0,    0,    0,    0,  213,  527,  986,    0,  537,
    0,  537,    0,    0,    0,    0,  241,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  877,
    0,  877,  877,    0,    0,    0,    0,  241,  877,  213,
    0,  537,    0,  241,    0,    0,    0,    0,    0,    0,
    0,    0,  877,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  877,  878,    0,    0,
    0,  877,  877,    0,    0,  877,    0,  241,    0,    0,
    0,    0,  877,    0,  877,  877,    0,    0,  877,    0,
    0,    0,  877,    0,  877,    0,  877,    0,  877,    0,
    0,    0,    0,  877,  877,    0,  877,    0,  878,  878,
  878,  878,  878,    0,  878,    0,    0,  989,    0,    0,
    0,  877,  877,    0,  877,    0,  878,  878,  878,  878,
    0,    0,    0,    0,  109,  877,    0,  877,    0,  877,
  877,  877,    0,  877,    0,    0,  877,  877,    0,    0,
    0,    0,    0,    0,    0,    0,  877,    0,    0,    0,
  878,    0,    0,  110,  111,    0,  112,    0,    0,  877,
    0,    0,  877,  113,  114,    0,  115,  116,    0,  117,
    0,    0,  118,  119,  120,  121,  122,    0,  877,    0,
  123,  124,    0,    0,  877,  877,  125,  126,  127,  877,
    0,    0,    0,    0,    0,    0,    0,    0,  537,    0,
  877,  877,    0,  877,    0,    0,    0,    0,    0,  877,
    0,    0,    0,  537,    0,  537,  537,  537,    0,    0,
    0,  537,    0,    0,  537,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  537,  877,  877,
    0,  877,    0,  537,    0,    0,  537,    0,  877,  877,
  877,  877,  877,    0,  877,    0,  583,  877,  877,  877,
  877,  877,    0,    0,    0,  877,  877,    0,    0,    0,
  877,  877,  877,  877,  877,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  537,  959,
  724,  722,    0,  723,    0,  725,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  916,  915,
  917,    0,  537,    0,    0,    0,    0,    0,    0,  878,
    0,  878,  878,    0,    0,    0,    0,    0,  878,    0,
    0,    0,    0,  537,    0,    0,    0,    0,    0,  537,
    0,    0,  878,  537,    0,    0,  872,    0,    0,    0,
  537,    0,    0,    0,    0,    0,  878,    0,    0,    0,
    0,  878,  878,    0,    0,  878,    0,    0,  537,    0,
    0,    0,  878,  537,  878,  878,    0,    0,  878,    0,
    0,    0,  878,    0,  878,    0,  878,  872,  878,  872,
  872,  872,    0,  878,  878,    0,  878,    0,  537,    0,
    0,    0,  537,    0,    0,  872,  872,  872,  872,    0,
    0,  878,  878,    0,  878,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  109,  878,    0,  878,    0,  878,
  878,  878,    0,  878,    0,    0,  878,  878,    0,  872,
    0,    0,    0,    0,    0,    0,  878,    0,  491,    0,
    0,    0,    0,  110,  111,    0,  112,    0,    0,  878,
    0,    0,  878,  113,  114,  109,  115,  116,    0,  117,
    0,    0,  118,  119,  120,  121,  122,    0,  878,    0,
  123,  124,    0,    0,  878,  878,  125,  126,  127,  878,
    0,    0,    0,    0,  110,  111,    0,  112,    0,    0,
  878,  878,    0,  878,  113,  114,    0,  115,  116,  878,
  117,    0,    0,  118,  119,  120,  121,  122,    0,  903,
    0,  123,  124,    0,    0,  720,    0,  125,  126,  127,
  721,    0,    0,  904,    0,    0,    0,    0,  878,  878,
    0,  878,    0,    0,    0,    0,    0,    0,  878,  878,
  878,  878,  878,    0,  878,    0,    0,  878,  878,  878,
  878,  878,    0,    0,    0,  878,  878,    0,    0,  905,
  878,  878,  878,  878,  878,    0,    0,  906,    0,  724,
  722,    0,  723,    0,  725,  907,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  916,  915,  917,
    0,    0,    0,  908,    0,  909,    0,    0,  872,    0,
  872,  872,    0,    0,    0,    0,    0,  872,  910,    0,
  911,  912,  913,    0,    0,    0,    0,    0,    0,    0,
    0,  872,    0,    0,    0,  871,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  872,    0,    0,    0,    0,
  872,  872,    0,    0,  872,    0,    0,    0,    0,    0,
    0,  872,    0,  872,  872,    0,    0,  872,    0,  914,
    0,  872,    0,  872,    0,  872,  871,  872,  871,  871,
  871,  487,  872,  872,    0,  872,    0,    0,    0,    0,
    0,    0,    0,    0,  871,  871,  871,  871,    0,    0,
  872,  872,    0,  872,    0,    0,    0,  109,    0,    0,
    0,    0,    0,    0,  872,    0,  872,    0,  872,  872,
  872,    0,  872,    0,    0,  872,  872,    0,  871,    0,
    0,    0,    0,    0,    0,  872,  110,  111,    0,  112,
    0,    0,    0,    0,    0,    0,  113,  114,  872,  115,
  116,  872,  117,    0,  109,  118,  119,  120,  121,  122,
    0,    0,    0,  123,  124,    0,    0,  872,    0,  125,
  126,  127,    0,  872,  872,    0,    0,    0,  872,    0,
    0,    0,    0,  110,  111,    0,  112,    0,    0,  872,
  872,    0,  872,  113,  114,    0,  115,  116,  872,  117,
    0,    0,  118,  119,  120,  121,  122,    0,  903,    0,
  123,  124,    0,    0,  720,    0,  125,  126,  127,  721,
    0,    0,  904,    0,    0,    0,    0,  872,  872,    0,
  872,    0,    0,    0,    0,    0,    0,  872,  872,  872,
  872,  872,    0,  872,    0,    0,  872,  872,  872,  872,
  872,    0,    0,    0,  872,  872,    0,    0,  905,  872,
  872,  872,  872,  872,    0,    0,  906,    0,    0,    0,
    0,    0,    0,    0,  907,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    1,    0,    0,    0,
    0,    0,  908,    0,  909,    0,    0,  871,    0,  871,
  871,    0,    0,    0,    2,    0,  871,  910,    0,  911,
  912,  913,    3,    0,    0,    0,    0,    0,    0,    4,
  871,    5,    0,    6,  873,    0,    0,    0,    7,    0,
    0,    0,    0,    0,  871,    8,    0,    0,    0,  871,
  871,    0,    0,  871,    0,    0,    0,    0,    0,    9,
  871,    0,  871,  871,    0,    0,  871,    0,  914,   10,
  871,    0,  871,    0,  871,  873,  871,  873,  873,  873,
    0,  871,  871,    0,  871,  211,    0,    0,  211,    0,
    0,    0,    0,  873,  873,  873,  873,    0,    0,  871,
  871,  211,  871,    0,    0,  211,    0,    0,  211,    0,
    0,    0,    0,  871,    0,  871,    0,  871,  871,  871,
    0,  871,    0,    0,  871,  871,    0,  873,    0,    0,
    0,    0,   11,    0,  871,    0,    0,    0,    0,   12,
    0,    0,   13,    0,   14,   15,    0,  871,  211,    0,
  871,    0,    0,    0,  211,  211,  211,    0,  211,  211,
  211,    0,  211,    0,    0,    0,  871,    0,    0,   16,
    0,    0,  871,  871,    0,    0,    0,  871,    0,    0,
  211,    0,  211,    0,    0,    0,    0,    0,  871,  871,
    0,  871,    0,    0,    0,    0,    0,  871,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  211,    0,    0,    0,    0,   81,    0,   17,    0,
    0,    0,    0,    0,    0,    0,  871,  871,  211,  871,
    0,    0,    0,   18,   19,    0,  871,  871,  871,  871,
  871,    0,  871,    0,    0,  871,  871,  871,  871,  871,
  211,    0,    0,  871,  871,    0,    0,    0,  871,  871,
  871,  871,  871,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  211,  211,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  873,    0,  873,  873,
    0,    0,  211,    0,    0,  873,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  873,
    0,    0,    0,  876,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  873,    0,    0,    0,    0,  873,  873,
    0,    0,  873,    0,    0,    0,    0,    0,    0,  873,
    0,  873,  873,    0,    0,  873,    0,    0,    0,  873,
    0,  873,    0,  873,  876,  873,  876,  876,  876,    0,
  873,  873,    0,  873,  884,    0,    0,  609,    0,    0,
    0,    0,  876,  876,  876,  876,    0,    0,  873,  873,
  885,  873,    0,    0,  610,    0,    0,  611,    0,    0,
    0,    0,  873,    0,  873,    0,  873,  873,  873,    0,
  873,    0,    0,  873,  873,    0,  876,    0,    0,    0,
    0,    0,    0,  873,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  873,  612,    0,  873,
    0,    0,    0,  613,  614,  615,    0,  616,  617,  618,
    0,  619,    0,    0,    0,  873,    0,    0,    0,    0,
    0,  873,  873,    0,    0,    0,  873,    0,    0,  620,
    0,  621,    0,    0,    0,    0,    0,  873,  873,    0,
  873,    0,    0,    0,    0,    0,  873,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  622,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  873,  873,  623,  873,    0,
    0,    0,    0,    0,    0,  873,  873,  873,  873,  873,
    0,  873,    0,    0,  873,  873,  873,  873,  873,  624,
    0,    0,  873,  873,    0,    0,    0,  873,  873,  873,
  873,  873,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  853,  625,  626,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  876,    0,  876,  876,    0,
    0,  627,    0,    0,  876,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  876,    0,
    0,    0,    0,    0,  853, 1020, 1020,  853, 1020, 1020,
 1020,    0,  876,    0,    0,    0,    0,  876,  876,    0,
    0,  876,  853, 1020, 1020, 1020,    0,    0,  876,    0,
  876,  876,    0,    0,  876,    0,    0,    0,  876,    0,
  876,    0,  876,    0,  876,    0,    0,    0,    0,  876,
  876,    0,  876,    0, 1020,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  876,  876,    0,
  876,    0,    0,    0,  570,    0,    0,    0,    0,    0,
    0,  876,    0,  876,    0,  876,  876,  876,  538,  876,
    0,    0,  876,  876,    0,    0,    0,    0,    0,    0,
    0,    0,  876,  570,  570,    0,  570,    0,    0,    0,
    0,    0,    0,  570,  570,  876,  570,  570,  876,  570,
    0,  280,  570,  570,  570,  570,  570,    0,  538,  538,
  570,  570,  538,    0,  876,    0,  570,  570,  570,    0,
  876,  876,    0,    0,    0,  876,    0,  538,    0,  538,
  280,  280,    0,  280,    0,    0,  876,  876,    0,  876,
  280,  280,    0,  280,  280,  876,  280,    0,    0,  280,
  280,  280,  280,  280,    0,    0,    0,  280,  280,  538,
    0,    0,    0,  280,  280,  280,    0,    0,    0,    0,
    0,    0,    0,    0,  876,  876,    0,  876,    0,    0,
    0,    0,    0,    0,  876,  876,  876,  876,  876,    0,
  876,    0,  854,  876,  876,  876,  876,  876,    0,    0,
    0,  876,  876,    0,    0,  853,  876,  876,  876,  876,
  876,    0,    0,    0, 1020,    0,    0,    0,    0,    0,
 1020,    0,    0,    0,    0, 1020,    0,    0, 1020,    0,
    0,    0,    0,  854, 1021, 1021,  854, 1021, 1021, 1021,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  854, 1021, 1021, 1021,    0,    0,    0,  853,    0,
    0,  853,    0,    0, 1020,    0,    0,    0,  853,    0,
  853,    0, 1020,    0,  853,    0,    0,    0,    0,  853,
 1020,    0,  853, 1021,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  853, 1020,    0,
 1020,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  682,  853,    0, 1020,    0, 1020, 1020, 1020,    0,  853,
    0,    0,  853,  853,    0,    0,  538,    0,    0,    0,
    0,    0,  853,  608,    0,    0,  609,    0,  855,    0,
    0,  538,    0,  538,  538,  538,    0,    0,  853,  538,
    0,  682,  538,  610,  682,    0,  611,    0,    0,    0,
    0,    0,    0,    0, 1020,  538,    0,    0,    0,  682,
  853,  538,    0,    0,  538,  853,    0,    0,    0,  855,
 1022, 1022,  855, 1022, 1022, 1022,    0,  853,    0,  853,
    0,    0,    0,    0,    0,    0,  612,  855, 1022, 1022,
 1022,    0,  613,  614,  615,    0,  616,  617,  618,    0,
  619,    0,    0,    0,    0,    0,  538,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  620, 1022,
  621,    0,    0,    0,    0,    0,  853,    0,    0,    0,
  538,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  854,    0,  853,    0,    0,  622,
  853,  538,    0, 1021,    0,    0,    0,  538,    0, 1021,
    0,  538,    0,    0, 1021,    0,  623, 1021,  538,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  538,    0,  624,    0,
    0,  538,    0,    0,    0,    0,    0,  854,    0,    0,
  854,    0,    0, 1021,  810,    0,    0,  854,    0,  854,
    0, 1021,    0,  854,    0,    0,  538,    0,  854, 1021,
  538,  854,    0,  625,  626,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  811,    0,  854, 1021,    0, 1021,
  627,    0,    0,    0,    0,  810,    0,    0,  810,    0,
  854,    0, 1021,    0, 1021, 1021, 1021,    0,  854,    0,
    0,  854,  854,  810,    0,    0,    0,    0,    0,    0,
    0,  854,    0,    0,    0,  811,  837,    0,  811,    0,
    0,    0,    0,    0,    0,    0,    0,  854,    0,    0,
  855,    0,    0,  811,    0,    0,    0,    0,    0, 1022,
    0,    0,    0, 1021,    0, 1022,    0,    0,    0,  854,
 1022,    0,    0, 1022,  854,  682,    0,  837,  682,    0,
  837,    0,    0,    0,    0,  682,  854,  682,  854,    0,
    0,  682,    0,    0,    0,  837,  682,    0,    0,  682,
    0,    0,    0,  855,    0,    0,  855,    0,    0, 1022,
    0,    0,    0,  855,  682,  855,    0, 1022,    0,  855,
    0,    0,    0,    0,  855, 1022,    0,  855,  682,    0,
    0,    0,    0,    0,    0,  854,  682,    0,    0,    0,
  682,    0,  855, 1022,    0, 1022,    0,    0,    0,  682,
    0,    0,    0,    0,    0,  854,  855,    0, 1022,  854,
 1022, 1022, 1022,    0,  855,  682,    0,  855,  855,    0,
    0,    0,    0,    0,    0,    0,    0,  855,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  814,    0,    0,
    0,    0,  682,  855,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  682,    0,  682,    0,    0, 1022,
    0,    0,  655,    0,    0,  855,    0,    0,    0,    0,
  855,    0,    0,    0,    0,    0,  810,    0,  814,    0,
    0,  814,  855,    0,  855,    0,    0,    0,    0,    0,
    0,  655,  655,    0,  655,    0,  814,    0,    0,    0,
    0,  655,  655,  682,  655,  655,  811,  655,    0,    0,
  655,  655,  655,  655,  655,    0,    0,    0,  655,  655,
  812,    0,    0,  682,  655,  655,  655,  682,    0,  810,
    0,  855,  810,    0,    0,    0,    0,    0,    0,  810,
    0,  810,    0,    0,    0,  810,    0,    0,  837,    0,
  810,  855,    0,  810,    0,  855,    0,    0,    0,  811,
    0,  812,  811,    0,  812,    0,    0,    0,  810,  811,
    0,  811,    0,    0,    0,  811,    0,    0,    0,  812,
  811,    0,  810,  811,    0,    0,    0,    0,    0,    0,
  810,    0,    0,  810,  810,    0,    0,    0,  811,    0,
    0,  837,    0,  810,  837,    0,    0,    0,    0,  813,
    0,  837,  811,  837,    0,    0,    0,  837,    0,  810,
  811,    0,  837,  811,  811,  837,    0,    0,    0,    0,
    0,    0,  920,  811,    0,    0,    0,    0,    0,    0,
  837,  810,  845,    0,    0,    0,  810,    0,    0,  811,
  813,    0,    0,  813,  837,  921,    0,    0,  810,    0,
  810,    0,  837,    0,    0,  837,  837,    0,  813,    0,
    0,  811,    0,    0,    0,  837,  811,    0,    0,    0,
    0,    0,    0,  845,  807,    0,  845,    0,  811,    0,
  811,  837,    0,    0,    0,    0,    0,    0,    0,  814,
    0,  845,    0,    0,    0,    0,    0,  810,    0,    0,
    0,  922,    0,  837,    0,    0,    0,    0,  837,    0,
    0,    0,    0,    0,    0,  807,    0,  810,  807,    0,
  837,  810,  837,    0,    0,    0,    0,  811,    0,    0,
    0,    0,    0,  807,    0,  109,    0,    0,    0,    0,
    0,    0,  814,    0,    0,  814,    0,  811,    0,    0,
    0,  811,  814,    0,  814,    0,    0,    0,  814,    0,
    0,    0,    0,  814,  110,  111,  814,  112,  808,  837,
    0,    0,  812,    0,  113,  114,    0,  115,  116,    0,
  117,  814,    0,  118,  119,  120,  121,  122,    0,  837,
    0,  123,  124,  837,    0,  814,    0,  125,  126,  127,
    0,    0,    0,  814,    0,    0,  814,  814,    0,  808,
    0,    0,  808,    0,    0,    0,  814,    0,    0,    0,
    0,    0,    0,    0,    0,  812,    0,  808,  812,    0,
    0,    0,  814,    0,    0,  812,    0,  812,    0,    0,
    0,  812,    0,  809,    0,    0,  812,    0,    0,  812,
    0,    0,    0,    0,  814,    0,    0,    0,    0,  814,
    0,  813,    0,    0,  812,    0,    0,    0,    0,    0,
    0,  814,    0,  814,    0,    0,    0,    0,  812,    0,
    0,    0,    0,    0,  809,    0,  812,  809,    0,  812,
  812,    0,    0,    0,  845,    0,    0,    0,    0,  812,
    0,    0,  809,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  813,  812,    0,  813,    0,    0,
  814,    0,    0,    0,  813,    0,  813,    0,    0,    0,
  813,    0,  844,    0,    0,  813,  807,  812,  813,    0,
  814,    0,  812,    0,  814,    0,    0,  845,    0,    0,
  845,    0,    0,  813,  812,    0,  812,  845,    0,  845,
    0,    0,    0,  845,    0,  838,    0,  813,  845,    0,
    0,  845,    0,  844,    0,  813,  844,    0,  813,  813,
    0,    0,    0,    0,    0,    0,  845,    0,  813,  807,
    0,  844,  807,    0,    0,    0,    0,    0,    0,  807,
  845,  807,    0,  812,  813,  807,  838,  846,  845,  838,
  807,  845,  845,  807,    0,    0,    0,    0,    0,    0,
    0,  845,    0,  812,  838,    0,  813,  812,  807,    0,
  808,  813,    0,    0,    0,    0,    0,  845,    0,    0,
    0,    0,  807,  813,    0,  813,    0,    0,  846,    0,
  807,  846,    0,  807,  807,    0,    0,    0,    0,  845,
    0,    0,    0,  807,  845,    0,  846,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  845,    0,  845,  807,
    0,    0,    0,  808,    0,    0,  808,    0,    0,    0,
    0,    0,  813,  808,    0,  808,    0,    0,    0,  808,
    0,  807,    0,    0,  808,  809,  807,  808,    0,    0,
    0,    0,  813,    0,    0,    0,  813,    0,  807,    0,
  807,    0,  808,    0,    0,  845,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  808,  847,    0,    0,
    0,    0,    0,    0,  808,  845,    0,  808,  808,  845,
    0,    0,    0,    0,    0,    0,    0,  808,  809,    0,
    0,  809,    0,    0,    0,  835,    0,  807,  809,    0,
  809,    0,    0,  808,  809,    0,    0,    0,  847,  809,
    0,  847,  809,    0,    0,    0,    0,  807,    0,    0,
    0,  807,    0,    0,  844,  808,  847,  809,    0,    0,
  808,    0,    0,    0,    0,    0,  835,    0,    0,  835,
    0,  809,  808,    0,  808,    0,    0,    0,    0,  809,
    0,    0,  809,  809,  835,    0,    0,  838,    0,    0,
    0,    0,  809,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  844,  809,    0,
  844,    0,    0,    0,    0,    0,    0,  844,    0,  844,
    0,  808,    0,  844,    0,    0,    0,    0,  844,  846,
  809,  844,    0,    0,    0,  809,    0,    0,    0,    0,
  838,  808,    0,  838,    0,  808,  844,  809,    0,  809,
  838,    0,  838,    0,    0,    0,  838,    0,    0,    0,
  844,  838,    0,    0,  838,    0,    0,    0,  844,  833,
    0,  844,  844,    0,    0,    0,    0,    0,    0,  838,
    0,  844,  846,    0,    0,  846,    0,    0,    0,    0,
    0,    0,  846,  838,  846,    0,  809,  844,  846,    0,
    0,  838,    0,  846,  838,  838,  846,    0,    0,    0,
  833,    0,    0,  833,  838,    0,  809,    0,    0,  844,
  809,  846,    0,    0,  844,    0,    0,    0,  833,  839,
  838,    0,    0,    0,    0,  846,  844,    0,  844,    0,
    0,    0,    0,  846,    0,    0,  846,  846,    0,    0,
    0,    0,  838,    0,    0,    0,  846,  838,    0,  847,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  838,
  839,  838,  846,  839,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  844,    0,  835,  839,    0,
    0,    0,    0,    0,  846,    0,    0,    0,    0,  846,
    0,    0,    0,    0,  848,  844,    0,    0,    0,  844,
    0,  846,  847,  846,    0,  847,    0,    0,  838,    0,
    0,    0,  847,    0,  847,    0,    0,    0,  847,    0,
    0,    0,    0,  847,    0,    0,  847,    0,  838,    0,
  835,    0,  838,  835,    0,  848,    0,    0,  848,    0,
  835,  847,  835,    0,    0,    0,  835,    0,    0,    0,
  846,  835,    0,  848,  835,  847,    0,    0,    0,    0,
    0,    0,    0,  847,    0,    0,  847,  847,    0,  835,
  846,    0,    0,    0,  846,    0,  847,    0,    0,    0,
    0,    0,    0,  835,    0,    0,    0,    0,    0,    0,
    0,  835,  847,    0,  835,  835,    0,    0,    0,    0,
    0,    0,    0,    0,  835,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  847,    0,  834,    0,    0,  847,
  835,  833,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  847,    0,  847,    0,    0,    0,    0,    0,    0,
    0,    0,  835,    0,  836,    0,    0,  835,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  834,    0,  835,
  834,  835,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  833,  834,    0,  833,    0,    0,
  847,  839,    0,    0,  833,  836,  833,    0,  836,    0,
  833,    0,    0,    0,    0,  833,    0,    0,  833,    0,
  847,    0,    0,  836,  847,    0,    0,    0,  835,    0,
    0,    0,    0,  833,    0,    0,    0,    0,    0,  840,
    0,    0,    0,    0,    0,    0,    0,  833,  835,    0,
    0,    0,  835,    0,  839,  833,    0,  839,  833,  833,
    0,    0,    0,    0,  839,    0,  839,    0,  833,    0,
  839,    0,    0,    0,    0,  839,  848,    0,  839,    0,
  840,    0,    0,  840,  833,    0,    0,    0,    0,    0,
    0,    0,    0,  839,    0,    0,    0,    0,  840,    0,
    0,    0,    0,    0,    0,    0,  833,  839,    0,    0,
    0,  833,    0,    0,    0,  839,  668,    0,  839,  839,
    0,    0,    0,  833,    0,  833,    0,    0,  839,  848,
    0,    0,  848,    0,    0,    0,    0,    0,    0,  848,
    0,  848,    0,    0,  839,  848,    0,    0,    0,    0,
  848,    0,    0,  848,    0,    0,    0,  668,    0,    0,
  668,    0,    0,    0,    0,    0,  839,    0,  848,    0,
    0,  839,  833,    0,    0,  668,    0,    0,    0,  679,
    0,    0,  848,  839,    0,  839,    0,    0,    0,    0,
  848,    0,  833,  848,  848,    0,  833,    0,  834,    0,
    0,    0,    0,  848,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  848,
  679,    0,    0,  679,    0,    0,  836,    0,    0,    0,
    0,    0,  839,    0,    0,    0,    0,    0,  679,    0,
    0,  848,    0,    0,    0,    0,  848,    0,    0,    0,
    0,  834,  839,    0,  834,    0,  839,    0,  848,    0,
  848,  834,    0,  834,    0,    0,    0,  834,    0,    0,
    0,    0,  834,    0,    0,  834,    0,    0,    0,  836,
    0,    0,  836,    0,    0,    0,    0,    0,    0,  836,
  834,  836,    0,    0,    0,  836,    0,    0,    0,    0,
  836,  840,    0,  836,  834,    0,    0,  848,    0,    0,
    0,    0,  834,    0,    0,  834,  834,    0,  836,    0,
    0,    0,    0,    0,    0,  834,    0,  848,    0,    0,
    0,  848,  836,    0,    0,    0,    0,    0,    0,    0,
  836,  834,    0,  836,  836,    0,    0,    0,    0,    0,
    0,    0,    0,  836,  840,    0,    0,  840,    0,    0,
    0,    0,    0,  834,  840,    0,  840,    0,  834,  836,
  840,    0,    0,    0,  672,  840,    0,  441,  840,  442,
  834,    0,  834,    0,    0,    0,    0,    0,    0,    0,
    0,  836,  444,  840,    0,    0,  836,  295,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  840,  836,    0,
  836,    0,    0,    0,    0,  840,    0,    0,  840,  840,
    0,    0,    0,    0,    0,    0,    0,    0,  840,  834,
    0,  668,    0,    0,  668,    0,    0,    0,    0,    0,
    0,  668,    0,  668,  840,    0,    0,  668,    0,  834,
    0,    0,  668,  834,    0,  668,    0,  836,    0,    0,
    0,    0,    0,    0,    0,    0,  840,    0,    0,    0,
  668,  840,    0,    0,    0,    0,    0,  836,    0,  102,
    0,  836,    0,  840,  668,  840,    0,    0,    0,    0,
    0,    0,  668,    0,  679,    0,  668,  679,    0,    0,
    0,    0,    0,    0,  679,  668,  679,    0,  211,    0,
  679,  211,    0,    0,    0,  679,    0,  103,  679,    0,
    0,  668,    0,    0,  211,    0,    0,    0,  211,  104,
    0,  211,  840,  679,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  140,    0,    0,    0,  679,  668,    0,
    0,    0,  840,  105,    0,  679,  840,    0,    0,  679,
  668,    0,  668,    0,    0,    0,    0,    0,  679,    0,
    0,  211,    0,    0,    0,    0,    0,  211,  211,  211,
    0,  211,  211,  211,  679,  211,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,  211,    0,  211,    0,    0,  405,  668,
    0,  679,    0,    0,    0,    0,    0,    0,  106,  672,
  406,    0,  441,  679,  442,  679,  407,  408,  107,  668,
    0,    0,    0,  668,  211,    0,    0,  444,    0,    0,
    0,    0,  295,    0,    0,  666,    0,    0,    0,    0,
    0,  211,    0,  108,  409,    0,    0,  410,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  109,  679,  211,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,  412,  413,    0,    0,    0,    0,
    0,    0,  679,    0,  414,    0,  679,    0,  415,    0,
  110,  111,    0,  112,    0,    0,  895,    0,  211,  211,
  113,  114,    0,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,  102,  211,    0,  123,  124,    0,
    0,    0,    0,  125,  126,  127,    0,    0,    0,    0,
    0,    0,    0,   14,    0,    0,    0,  668,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,  103,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,  104,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,    0,    0,    0,  423,  105,    0,
    0,    0,    0,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,  896,
  897,  898,  123,  124,    0,    0,  404,    0,  125,  440,
  127,  672,    0,  405,  441,    0,  442,    0,    0,    0,
    0,    0,    0,  106,    0,  406,    0,    0,  665,  444,
    0,  407,  408,  107,  295,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  666,    0,    0,    0,    0,    0,    0,    0,  108,  409,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  109,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
    0,    0,    0,  415,    0,  110,  111,    0,  112,    0,
    0,  667,    0,    0,    0,  113,  114,    0,  115,  116,
    0,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,    0,    0,    0,  125,  126,
  127,  320,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  668,    0,    0,    0,    0,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,  320,
    0,    0,    0,    0,  418,    0,  320,    0,    0,    0,
  320,  320,    0,  320,    0,    0,  320,    0,  419,  420,
  421,  422,  218,  219,    0,    0,    0,    0,    0,  320,
    0,    0,  423,    0,    0,  320,    0,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,    0,  404,  118,
  119,  669,  670,  671,    0,  405,    0,  123,  124,    0,
    0,    0,    0,  125,  440,  127,  672,  406,    0,  441,
    0,  442,  320,  407,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  444,    0,    0,    0,    0,  295,
  320,    0,  666,    0,    0,    0,    0,    0,    0,    0,
    0,  409,    0,    0,  410,  320,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  320,    0,    0,    0,    0,
    0,    0,    0,    0,  320,    0,  320,    0,    0,  411,
    0,  412,  413,  320,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,  141,  415,    0,  141,    0,    0,
    0,    0,    0,  891,    0,    0,    0,    0,    0,    0,
    0,    0,  320,  320,  141,  320,    0,  141,    0,    0,
    0,    0,  320,  320,    0,  320,  320,    0,  320,    0,
    0,  320,  320,  320,  320,  320,    0,    0,    0,  320,
  320,  320,  320,  320,  668,  320,  320,  320,    0,    0,
  416,    0,    0,    0,    0,    0,  417,  141,    0,    0,
    0,    0,    0,  141,  141,  141,  418,  141,  141,  141,
    0,  141,    0,    0,    0,    0,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,  141,
    0,  141,    0,    0,  423,    0,    0,    0,    0,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
  141,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  669,  670,  671,    0,  141,    0,  123,
  124,    0,    0,  404,    0,  125,  440,  127,  672,    0,
  405,  441,    0,  442,    0,    0,    0,    0,    0,  141,
    0,    0,  406,    0,    0,    0,  444,    0,  407,  408,
    0,  295,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  666,    0,    0,
    0,    0,    0,    0,  141,  141,  409,    0,    0,  410,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  141,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,    0,  988,    0,    0,  988,    0,  667,    0,
    0,    0,    0,    0,    0,  511,    0,    0,  441,    0,
  442,  990,    0,  988,    0,    0,  988,    0,    0,    0,
    0,    0,    0,  444,    0,    0,    0,    0,  295,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  668,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,    0,    0,    0,  988,    0,    0,    0,
    0,  418,  988,  988,  988,    0,  988,  988,  988,    0,
  988,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,  988,  423,
  988,    0,    0,    0,    0,    0,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,    0,  434,  435,    0,  436,  437,  438,  988,
  115,  116,  439,  117,    0,  404,  118,  119,  669,  670,
  671,    0,  405,    0,  123,  124,  988,    0,    0,    0,
  125,  440,  127,    0,  406,    0,    0,    0,    0,    0,
  407,  408,    0,    0,    0,    0,    0,    0,  988,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  666,
    0,    0,    0,    0,    0,    0,    0,    0,  409,    0,
    0,  410,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  988,  988,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  411,    0,  412,  413,
  988,    0,    0,    0,    0,    0,    0,    0,  414,    0,
    0,    0,  415,    0,    0, 1146,    0,    0, 1147,    0,
  891,    0,  404,    0,    0,    0,    0,    0,    0,  405,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  406,    0,    0,    0,    0,    0,  407,  408,    0,
    0,    0,    0,  511,    0,    0,  441,    0,  442,    0,
    0,  668,    0,    0,    0,    0,    0,  416,    0,    0,
    0,  444,    0,  417,    0,  409,  295,    0,  410,    0,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  419,  420,  421,
  422,  218,  219,  411,    0,  412,  413,    0,    0,    0,
    0,  423,    0,    0,    0,  414,    0,    0,    0,  415,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,    0,    0,  434,  435,    0,  436,  437,
  438,    0,  115,  116,  439,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124, 1226,    0,
    0,    0,  125,  440,  127,    0,    0,    0,    0,    0,
    0,    0, 1148,    0,  416,    0,    0,    0,    0,    0,
  417,    0,    0,    0,    0,    0,    6,    0,    0,    0,
  418,    0,    0,    0,    0,    0,    0, 1431, 1432,    0,
 1433,    0,    0,    0,  419,  420,  421,  422,  218,  219,
    0,    0,    0,    0,    0,    0, 1434,    0,  423,    0,
    0,    0, 1435,    0,    0,    0,    0,  424,  425,  426,
  427,  428,  429,  430,  431,  432,  110,  433,    0,  112,
    0,    0,  434,  435,    0,  436,  437,  438,    0,  115,
  116,  439,  117,    0,    0,  118,  119,  120,  121,  122,
    0,    0,    0,  123,  124,    0,    0,    0,    0,  125,
  440,  127,    0, 1157,    0,    0, 1147,    0,    0, 1436,
  404,    0,    0,    0,    0,    0,    0,  405,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   14,    0,  406,
    0,    0,    0,    0,    0,  407,  408,    0,    0,    0,
    0,  511, 1437,    0,  441,    0,  442,    0,    0,    0,
    0,    0,   16,    0,    0,    0,    0,    0,    0,  444,
    0, 1635,    0,  409,  295,    0,  410,    0,    0,    0,
  109,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  411,    0,  412,  413,    0,    0,    0,    0, 1438,
  111,    0,  112,  414,    0,    0,    0,  415,    0,  113,
  114,    0,  115,  116,    0,  117,    0,    0,  118, 1439,
  120,  121,  122,    0,    0,    0,  123,  124, 1440, 1441,
 1442,    0,  125,  126,  127,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  511,    0,
    0,  441,    0,  442,    0,    0,    0,    0,    0,    0,
 1148,    0,  416,    0,    0,    0,  444,    0,  417,    0,
    0,  295,    0,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  419,  420,  421,  422,  218,  219,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,    0,
    0,    0,    0,    0,    0,  424,  425,  426,  427,  428,
  429,  430,  431,  432,  110,  433,    0,  112,    0,    0,
  434,  435,    0,  436,  437,  438,    0,  115,  116,  439,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,    0,    0,    0,    0,  125,  440,  127,
    0, 1161,    0,    0, 1147,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  409,    0,    0,  410,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  411,
    0,  412,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,    0, 1169,    0,
    0, 1147,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,  405,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  406,    0,    0,    0,    0,    0,
  407,  408,    0,    0,    0,    0,  511,    0,    0,  441,
    0,  442,    0,    0,    0,    0,    0,    0, 1148,    0,
  416,    0,    0,    0,  444,    0,  417,    0,  409,  295,
    0,  410,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,  411,    0,  412,  413,
    0,    0,    0,    0,  423,    0,    0,    0,  414,    0,
    0,    0,  415,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124, 1226,    0,    0,    0,  125,  440,  127,    0,    0,
    0,    0,    0,    0,    0, 1148,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,    6,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
 1431, 1432,    0, 1433,    0,    0,    0,  419,  420,  421,
  422,  218,  219,    0,    0,    0,    0,    0,    0, 1434,
    0,  423,    0,    0,    0, 1435,    0,    0,    0,    0,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,    0,    0,  434,  435,    0,  436,  437,
  438,    0,  115,  116,  439,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  440,  127,    0, 1172,    0,    0, 1147,
    0,    0, 1436,  404,    0,    0,    0,    0,    0,    0,
  405,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,  406,    0,    0,    0,    0,    0,  407,  408,
    0,    0,    0,    0,  511, 1437,    0,  441,    0,  442,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,  444,    0,    0,    0,  409,  295,    0,  410,
    0,    0,    0,  109,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0, 1438,  111,    0,  112,  414,    0,    0,    0,
  415,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118, 1439,  120,  121,  122,    0,    0,    0,  123,
  124, 1440, 1441, 1442,    0,  125,  126,  127,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  511,    0,    0,  441,    0,  442,    0,    0,    0,
    0,    0,    0, 1148,    0,  416,    0,    0,    0,  444,
    0,  417,    0,    0,  295,    0,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    0,
  125,  440,  127,    0, 1177,    0,    0, 1147,    0,    0,
    0,  404,    0,    0,  532,    0,    0,    0,  405,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  532,    0,    0,  532,    0,
    0,    0,    0,    0,  409,    0,    0,  410,    0,    0,
    0,    0,    0,  532,    0,  532,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,  412,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,  532,    0,    0,  415,    0,
    0, 1180,    0,    0, 1147,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,  511,
    0,    0,  441,    0,  442,    0,    0,    0,    0,    0,
    0, 1148,    0,  416,    0,    0,    0,  444,    0,  417,
    0,  409,  295,    0,  410,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,  411,
    0,  412,  413,    0,    0,    0,    0,  423,    0,    0,
    0,  414,    0,    0,    0,  415,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,    0,    0,    0,  125,  440,
  127,    0,    0,    0,    0,    0,    0,    0, 1148,    0,
  416,    0,  532,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  418,  532,    0,  532,
  532,  532,    0,    0,    0,  532,    0,    0,  532,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,  532,    0,    0,  423,    0,    0,  532,    0,    0,
  532,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,  532,    0,    0,  125,  440,  127,    0, 1183,
    0,    0, 1147,    0,    0,    0,  404,    0,    0,    0,
    0,    0,    0,  405,    0,    0,  532,  639,    0,  639,
  639,    0,  639,    0,    0,  406,    0,    0,    0,    0,
    0,  407,  408,    0,    0,  639,    0,  532,    0,    0,
  639,    0,    0,  532,    0,    0,    0,  532,    0,    0,
    0,    0,    0,    0,  532,    0,    0,    0,    0,  409,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  532,    0,    0,    0,    0,  532,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
    0,    0,  532,  415,    0,    0,  532,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  633,    0,  633,  633,    0,  633,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  633,    0,    0,    0,    0,  633,    0,    0,
    0,    0,    0,    0,    0,    0, 1148,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  419,  420,
  421,  422,  218,  219,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,  639,    0,  118,
  119,  120,  121,  122,  639,    0,    0,  123,  124,    0,
    0,  639,    0,  125,  440,  127,    0,    0,    0,    0,
    0,    0,    0,  639,    0,    0,    0,    0,    0,  639,
  639,    0,    0,    0,    0,    0,    0,  639,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  639,    0,    0,
  639,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  639,    0,  639,  639,    0,
    0,    0,    0,    0,    0,    0,    0,  639,    0,    0,
    0,  639,    0,  511,  633,  516,  441,    0,  442,    0,
    0,  633,    0,    0,    0,    0,    0,    0,  633,    0,
    0,  444,    0,    0,    0,    0,  295,    0,    0,    0,
  633,    0,    0,    0,    0,    0,  633,  633,    0,    0,
    0,    0,    0,    0,  633,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  639,    0,    0,    0,
    0,    0,  639,    0,  633,    0,    0,  633,    0,    0,
    0,    0,  639,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  639,  639,  639,  639,
  639,  639,  633,    0,  633,  633,    0,    0,    0,    0,
  639,    0,    0,    0,  633,    0,    0,    0,  633,  639,
  639,  639,  639,  639,  639,  639,  639,  639,  639,  639,
    0,  639,    0,    0,  639,  639,    0,  639,  639,  639,
    0,  639,  639,  639,  639,    0,    0,  639,  639,  639,
  639,  639,    0,    0,    0,  639,  639,    0,    0,    0,
    0,  639,  639,  639,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  633,    0,    0,    0,    0,    0,  633,
    0,    0,  986,    0,    0,    0,    0,    0,    0,  633,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  633,  633,  633,  633,  633,  633,    0,
    0,    0,    0,    0,    0,    0,    0,  633,    0,    0,
    0,    0,    0,    0,    0,    0,  633,  633,  633,  633,
  633,  633,  633,  633,  633,  633,  633,    0,  633,    0,
  404,  633,  633,    0,  633,  633,  633,  405,  633,  633,
  633,  633,  987,    0,  633,  633,  633,  633,  633,  406,
    0,    0,  633,  633,    0,  407,  408,    0,  633,  633,
  633,  511,    0,    0,  441,    0,  442,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  988,    0,  444,
    0,    0,  989,  409,  295,    0,  410,    0,  990,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  109,
    0,    0,    0,    0,    0,    0,    0,  991,    0,    0,
    0,  411,    0,  412,  413,    0,    0,    0,    0,    0,
    0,    0,    0,  414,    0,    0,    0,  415,  110,  111,
    0,  112,    0,    0,    0,    0,    0,    0,  113,  114,
    0,  115,  116,    0,  117,    0,    0,  118,  119,  120,
  121,  122,    0,    0,    0,  123,  124,    0,    0,    0,
    0,  125,  126,  127,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  419,  420,  421,  422,  218,  219,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,    0,
    0,    0,    0,    0,    0,  424,  425,  426,  427,  428,
  429,  430,  431,  432,  110,  433,    0,  112,    0,    0,
  434,  435,    0,  436,  437,  438,    0,  115,  116,  439,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,    0,    0,    0,    0,  125,  440,  127,
  511,    0,    0,  441,    0,  442,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,  444,    0,
    0,    0,    0,  295,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  409,    0,    0,  410,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  411,
    0,  412,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,  511,  963,    0,
  441,    0,  442,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  444,    0,    0,    0,    0,
  295,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,  704,    0,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,    0,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,  404,  434,  435,
    0,  436,  437,  438,  405,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,  406,    0,    0,  123,
  124,    0,  407,  408,    0,  125,  440,  127,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  409,    0,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  411,    0,
  412,  413,    0,    0,    0,    0,    0,    0,    0,    0,
  414,    0,    0,    0,  415,    0,  511,    0,    0,  441,
    0,  442,    0,    0,  404,    0,    0,    0,    0,    0,
    0,  405,    0,    0,  444,    0,    0,    0,    0,  295,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  407,
  408,    0,    0,    0,    0,    0,    0,    0,    0,   14,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  416,
    0,    0,    0,    0,    0,  417,    0,  409,    0,    0,
  410,    0,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  419,
  420,  421,  422,  218,  219,  411,    0,  412,  413,    0,
    0,    0,    0,  423,    0,    0,    0,  414,    0,    0,
    0,  415,  424,  425,  426,  427,  428,  429,  430,  431,
  432,  110,  433,    0,  112,    0,    0,  434,  435,    0,
  436,  437,  438,    0,  115,  116,  439,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  440,  127,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  419,  420,  421,  422,
  218,  219,    0,    0,    0,    0,    0,    0,    0,    0,
  423,    0,    0,    0,    0,    0,    0,    0,    0,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,  404,  434,  435,    0,  436,  437,  438,
  405,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,  406,    0,    0,  123,  124,    0,  407,  408,
    0,  125,  440,  127,  511,    0,    0,  441,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  444,    0,    0,    0,  409,  295,    0,  410,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  511,    0,    0,  441,    0,  442,    0,    0,    0,
    0,    0,    0,    0,    0,  416,    0,    0,    0,  444,
 1732,  417,    0,    0,  295,    0,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1175,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    0,
  125,  440,  127,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,  405,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  409,    0,    0,  410,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,  412,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
  980,    0,  980,  980,    0,  980,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,  980,    0,
    0,    0,    0,  980,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,  409,    0,    0,  410,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1382,    0,    0,  419,  420,  421,  422,  218,  219,  411,
    0,  412,  413,    0,    0,    0,    0,  423,    0,    0,
    0,  414,    0,    0,    0,  415,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,    0,    0,    0,  125,  440,
  127,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,    0,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,  980,  434,  435,
    0,  436,  437,  438,  980,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,  980,    0,    0,  123,
  124,    0,  980,  980,    0,  125,  440,  127,  443,    0,
    0,  441,    0,  442,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  444,    0,    0,    0,
  980,  295,    0,  980,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  980,    0,
  980,  980,    0,    0,    0,    0,    0,    0,    0,    0,
  980,    0,    0,    0,  980,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  511,    0,    0,  441,    0,
  442,    0,    0,    0,    0,    0,    0,    0,    0,  980,
    0,    0,    0,  444,    0,  980,    0,    0,  295,    0,
    0,    0,    0,    0,    0,  980,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  980,
  980,  980,  980,  980,  980,    0,    0,    0,    0,    0,
    0,    0,    0,  980,    0,    0,    0,    0,    0,    0,
    0,    0,  980,  980,  980,  980,  980,  980,  980,  980,
  980,  980,  980,    0,  980,    0,    0,  980,  980,    0,
  980,  980,  980,    0,  980,  980,  980,  980,    0,    0,
  980,  980,  980,  980,  980,    0,    0,    0,  980,  980,
    0,    0,    0,    0,  980,  980,  980,    0,    0,    0,
    0,    0,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,  405,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  406,    0,    0,    0,    0,    0,
  407,  408,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  409,    0,
    0,  410,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  411,    0,  412,  413,
    0,    0,    0,    0,    0,    0,    0,    0,  414,    0,
    0,    0,  415,    0,  980,    0,    0,  980,    0,  980,
    0,    0,  404,    0,    0,    0,    0,    0,    0,  405,
    0,    0,  980,    0,    0,    0,    0,  980,    0,    0,
    0,  406,    0,    0,    0,    0,    0,  407,  408,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,  409,    0,    0,  410,    0,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  419,  420,  421,
  422,  218,  219,  411,    0,  412,  413,    0,    0,    0,
    0,  423,    0,    0,    0,  414,    0,    0,    0,  415,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,    0,    0,  434,  435,    0,  436,  437,
  438,    0,  115,  116,  439,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  440,  127,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  416,    0,    0,    0,    0,    0,
  417,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  419,  420,  421,  422,  218,  219,
    0,    0,    0,    0,    0,    0,    0,    0,  423,    0,
    0,  554,    0,    0,    0,    0,    0,  424,  425,  426,
  427,  428,  429,  430,  431,  432,  110,  433,    0,  112,
    0,  980,  434,  435,    0,  436,  437,  438,  980,  115,
  116,  439,  117,    0,    0,  118,  119,  120,  121,  122,
  980,    0,  554,  123,  124,  554,  980,  980,    0,  125,
  440,  127,    0,    0,  544,    0,    0,    0,    0,    0,
  554,    0,  554,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  980,    0,    0,  980,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  529,    0,  554,    0,    0,  544,    0,    0,  544,    0,
    0,    0,  980,    0,  980,  980,    0,    0,    0,    0,
    0,    0,    0,  544,  980,  544,    0,    0,  980,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  529,    0,    0,  529,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  544,    0,    0,    0,  529,
    0,  529,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  980,    0,    0,    0,    0,    0,  980,
    0,  529,    0,    0,    0,    0,    0,    0,    0,  980,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  980,  980,  980,  980,  980,  980,    0,
    0,    0,    0,    0,    0,    0,    0,  980,    0,    0,
    0,    0,    0,    0,    0,    0,  980,  980,  980,  980,
  980,  980,  980,  980,  980,  980,  980,    0,  980,    0,
    0,  980,  980,    0,  980,  980,  980,    0,  980,  980,
  980,  980,    0,    0,  980,  980,  980,  980,  980,  554,
    0,    0,  980,  980,    0,    0,    0,    0,  980,  980,
  980,    0,    0,    0,  554,    0,    0,  554,  554,    0,
    0,    0,  554,    0,    0,  554,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  554,    0,
    0,    0,    0,    0,  554,    0,    0,  554,    0,    0,
    0,    0,  544,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  544,    0,    0,
  544,  544,    0,    0,    0,  544,    0,    0,  544,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  529,  554,
    0,  544,    0,    0,    0,    0,    0,  544,    0,    0,
  544,    0,    0,  529,    0,    0,  529,  529,    0,    0,
    0,  529,    0,  554,  529,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  529,    0,    0,
    0,    0,    0,  529,  554,    0,  529,    0,    0,    0,
  554,    0,  544,    0,  554,    0,    0,    0,    0,    0,
    0,  554,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  544,    0,    0,  554,
    0,    0,  608,    0,  554,  609,    0,    0,  529,    0,
    0,  809,    0,    0,    0,    0,    0,  544,    0,    0,
    0,    0,  610,  544,    0,  611,    0,  544,    0,  554,
    0,    0,  529,  554,  544,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  544,  529,    0,    0,    0,  544,    0,  529,
    0,  608,    0,  529,  609,  612,    0,    0,    0,    0,
  529,  613,  614,  615,    0,  616,  617,  618,    0,  619,
    0,  610,  544,    0,  611,    0,  544,    0,  529,    0,
    0,    0,    0,  529,    0,    0,    0,  620,    0,  621,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  529,    0,
    0,    0,  529,    0,  612,    0,    0,    0,  622,    0,
  613,  614,  615,    0,  616,  617,  618,    0,  619,    0,
    0,    0,    0,    0,    0,  623,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  620,    0,  621,    0,
    0,    0,    0,    0,    0,    0,    0,  624,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  109,    0,    0,    0,    0,  622,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  625,  626,  623,    0,    0,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0,    0,    0,  627,
    0,  113,  114,    0,  115,  116,  624,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,  109,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  625,  626,    0,    0,    0,    0,    0,    0,    0,
  110,  111,    0,  112,    0,    0,    0,    0,  627,    0,
  113,  114,    0,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,  109,
    0,    0,    0,  125,  126,  127,    0,    0,  172,  173,
  174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
  184,  185,  186,  187,    0,    0,  188,  189,  110,  111,
    0,  112,    0,    0,  190,  191,    0,    0,  113,  114,
  192,  115,  116,  193,  117,  194,    0,  118,  119,  120,
  121,  122,    0,    0,    0,  123,  124,  195,  196,  197,
  198,  125,  126,  127,  199,  458,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
    0,    0,  458,  458,  458,  458,    0,  458,    0,    0,
  458,  458,    0,    0,  458,  458,  458,  458,  458,  458,
  458,  458,    0,  458,  458,  458,  458,  458,    0,    0,
    0,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,
};
static short yycheck[] = {                                       9,
    4,    0,   12,   67,  108,   16,   16,   58,   18,  546,
   66,  156,  351,   65,  382,  728,  146,  147,   59,  378,
  382,   66,   71,   16,   88,   94,  479,  578,  353,   70,
  522,  549,  146,  147,   97,  783,  499,  374,   73,  500,
 1299,   92, 1067,  982, 1225,   94,   56,   57,   58,   59,
   60,   61,   93,   91, 1015,   41,   66,   67,   68,  100,
   70,   96,   72,   73,  280,   75,   76,   60,  781,   76,
  690,  925,  735,  289,   84,   86,   86,   87,   88,   72,
   90,   91,   92,   93,   95,   95,   96,   97,   98,  297,
  100,   98,  137,   86,  139,  715,  704,  160,  660,  360,
  443,  653,  710,    0,  878,    0, 1063,  961,  233,   44,
  359,    0, 1119,  151,   45,    0,   40,  158,  571,   45,
  573,   44,   44, 1056,  326,  578,  387,  137,  138,  139,
   44,    0, 1056,    0, 1067,   44,   41,    0,  242,   45,
  164,  151, 1096, 1067,   44,  138,  156,   59,  158,  159,
  160,  203, 1056,  788,  164,   41,    0, 1114, 1056,    0,
    0, 1056,   59, 1067,   59,   63,  159, 1056,  803, 1067,
   59,  806, 1067,   44,   59,  443,  228,   44, 1067,   44,
  815,  326,  753, 1243, 1257, 1555,   40,   44,   41,    0,
   59,   44,   59,    0,    0,   44,   59,  273,  557,    0,
  283,   41,  327,    0,   41,  351,    0,  262,   41,   63,
  578,  395,  667,    0, 1061,   59,  578,  344,   59,   59,
   40, 1068, 1207,   61,   41, 1210,  271,  331,  505,  274,
   41,  315, 1079,   42,   41,   41,  871,   44,   44,  264,
  539,   41,  581,  511,  352,    0,  605,   41,   59,   40,
   40,  344,   59,   59,   45, 1102, 1244,  308,   59, 1440,
 1441,  271,   59,    0,  274,   59, 1291, 1292,  278,   44,
  280,  281,   59,   63,  268,  491,    0,  287,  287,  289,
  541,  326,    0,  447,  272,   41,   41,    0,  262,   44,
  629,  257,  300,    0,  262,  280,  260, 1258,  308,   44,
 1325,    0,  312,  272,   59,   41,  409,  352,   44, 1498,
  918,  919,  317,  421,  359, 1272,  361,   41,  363,  312,
   44,  366,   59,  375,  379,  377,  453,  454,   41,  498,
  514,   44, 1320,  436,   41,   59,  300,   44,  484,  391,
  348,   59,  352,  384,  395,  264,   59,  955,  336,  359,
  385,  361,   59,  363,  499, 1294,  366,  262, 1291, 1292,
   59,  445,   45,  855,  330,  359,  298, 1291, 1292,    0,
  453,  454,  427,    0,  384,  385,  719, 1331,  403,  832,
   63,   61,  458,  393,  727,  395,   93, 1291, 1292,  399,
 1297,  262, 1325, 1291, 1292,  399, 1291, 1292,    0, 1338,
  393, 1325, 1291, 1292,  672,  379, 1041, 1777,  297,    0,
   41,  379, 1671,   44,   41, 1488,  509, 1271,  879, 1039,
  264, 1325,   41, 1058, 1059,  818,  997, 1325,   59,  890,
 1325, 1416,   59,  894,  444, 1420, 1325, 1008, 1009,   41,
  895,   41,   44, 1078,  301,  262,  431, 1055, 1629,  490,
   41,  260,  317,  427,  499,  344,  297,   59,  797,  297,
 1649,  800,  262,  387,  505,  764,  315, 1314,   59,  306,
  276,  277,  811,  306,  379,  315,  376,  487,  284,  756,
  490,  491,  596,  380,  403,  325,  510,  327,  498,  264,
  402,  300,  502,  379,  487,  505,  336,  507,  507,  838,
  510,  546,  434,  344,  315,  498,  428,  559,  379,  502,
 1769,  380,    0,  380,  325,  450,  327,  380,  643,  450,
  389,  315,  389,  446,  450,  336,  389,  513,  597,  745,
  444, 1139, 1140,  578, 1142, 1143,  546,  446, 1499, 1002,
  380,  410,  336,  428,  450, 1552,  477,  599,  597,  389,
  403,  759,  450,   41, 1015,  804,  301,  882, 1817, 1222,
  315, 1113,  379, 1337, 1126, 1504,  372,  577,  578,  380,
    0,   59,  327,  583,  361,  444, 1533,  444,  389,  379,
  782,  336, 1043, 1842, 1121, 1645,  380,  393,    0,    0,
    0,  315,  432,  399, 1551,  389,  450,  649,  650,    0,
  401,  325,  732,  327,  444,  402,  446,  652,  869,  395,
 1564,   41,  336, 1520,   44,  339,  513,  297,  513,  439,
  509,  432, 1607,  379,  361,  380,  344,  433,  339,   59,
   41,   41,  423,   12,  389,  446,   91,  782,  432,    0,
   41,    0,  652,  262,  513,  344,  513,   59,   59,   59,
  513,  362,  446,  767, 1509,   41,  380,  695,   59,  450,
  450, 1004,  262,  376,  298,  389, 1513, 1236,  509, 1238,
 1517, 1518, 1280,  513,    0,  298,  753,  432, 1071, 1072,
   41, 1252,  297,  265,  315,  695,  493,  493,  733,  275,
  735,  446,  737,   59, 1302,  402, 1877,  395,   59, 1554,
   59, 1336,  513, 1728,    0,  336, 1553,  418,  432,  336,
  720,  334, 1049,  315,  500,  756, 1570,  299,  728,  513,
  444,  317,  446,  733,  315,  735, 1065,  737, 1121,  344,
  740,    0,  430,   59,  336,  745,  334,  782, 1077,  298,
  423,  793,  787,  753, 1683,  336,  756,    0, 1368,  315,
 1689,  333,  762,  763, 1502,  341,  262,   12,  513,  804,
  379,  830, 1101,   59,    0,  144,  776,  450,  778,    0,
  986,  781,   41,  989, 1236,  334, 1238,  787, 1115,  379,
  493,  830, 1300,  776,    0,  778,  831,  832,  837,  513,
   59,  843,  263,    0,  804, 1728,  865,  866,  867, 1124,
  434,  305,   42,  852, 1728,   41,   59,   47, 1151,   44,
 1657,  315,    0,    0, 1661,  446,  865,  866,  867,  446,
  275,  831,  832,   59, 1728,   41,  315,  315,   59, 1676,
 1728, 1323,  428, 1728,   41,  845,  434, 1329,  309, 1728,
    0,  845,  667,   59,  446, 1809, 1693,  672,  336,  320,
  276, 1402,   59,   41,   41,  446,  305, 1002,   93,  371,
  870,  304,  305,  494,  276,  277,  315,  397, 1193,    0,
  880,   59,   59,  379, 1217,  434,  262,  870,  888,  889,
    0,   41,  513, 1151,  888,  315,  416,  880,    0,  144,
  316,  310,  380, 1501,  509,  325,    0,  327,  410,   59,
 1864,  389,    0,    0,  315,  315,  336,   42,   43,  339,
   45,  513,   47, 1256,  315,  401,  298,  276,  277,  273,
  997,  261,  513,  304,  305,  336,  336,  990,   59, 1776,
    0, 1008, 1779,    0,  371,  336,   40,   41,  264,   59,
   44,  445,    0,  450,  432,  457,  987,   59,  419,    0,
  380,  322,  334,  988,  315,   59,  445, 1002,  446,  389,
  372,   59,   59,  380,  325,  450,  327,  393,  264,  380,
  316,  257, 1425, 1312, 1821,  336,  986,  987,  988,  989,
  990,  991, 1341,   41,  991, 1099,   44,  997,   41,   59,
  409,   44,   59,  379,  348,  397,  445,    0, 1008, 1009,
  443,   59,  432,  440, 1343, 1015,    0,  433,   59,    0,
    0,  264, 1620,  284,  444,  432,  446,  399,    0,  380,
  319,  432,  304,  305, 1486,  513,  297,  409,  389,    0,
  450,  484,  319,  264, 1402,  446,  446,  277,   41, 1108,
 1402,   44,  282,   40,  330,  446,  448,  393,  318,  394,
   41,  422,  434,   44, 1123,  260,   59,  477,  429, 1108,
 1129,  331,  443,  516,  390,   59,  891,  336,   59,   59,
  895,  432, 1117,    0, 1123,    0, 1121,   59, 1088,  315,
 1129, 1226,    0,  513,  354,  446, 1799,  433,   59, 1093,
   93,  327,    0,  390,  390,  300, 1106, 1365, 1366,   40,
  336,   40,  513,  513,    0, 1373, 1374, 1117, 1376, 1377,
  376, 1121, 1242, 1243, 1244,    0, 1384, 1385, 1386, 1387,
  336, 1389, 1390, 1391, 1392, 1393, 1394,  315,  315,  336,
  259,  428,   59,   41,   59,  405,   44,  390,   44,  327,
  269,   59,  277,  280,  380, 1753,  258,  282,  336,  336,
 1758,   59,  513,  389, 1451,  315,  276,  277,  270,  390,
  258,  443, 1276,   59, 1278,  462,  277,  376,   44, 1225,
    0,  282,  270,   44,   59,  485,  336, 1222, 1189, 1189,
 1225, 1226,  372, 1522,  378,  495,    0,   93,   44,  258,
  300,  303,  380,  305,  264,  376,  432,    0,  297,  297,
  297,  389,    0,  393,    0,  303,  264,  305,  259,  399,
  446,  315, 1222,  264,  318, 1225, 1226,   93,  269, 1558,
  273,  325,  271,  327,    0, 1339, 1236,  331, 1238,   59,
  446,    0,  336,    0,  303,  339,  285,   93,  348,  446,
  377,   44, 1252,  433,  432,   59,  344,  344, 1258,  348,
  354,  376,  397,  306,  352,  352,   59,  315,  446,  446,
  318,   59,  372,   59,  368,  428,  318,  325,    0,  327,
  264,  338,  376,  331,  264, 1812,  380,  513,  336,  331,
  329,  339,  264,   59,    0,  389,  446,  258,  409,  386,
   59,  388,   59,  428,  431,  348,  354,    0,  434,  270,
 1352,  405,  354,  448,  371, 1357, 1451,    0,  357,   41,
  368,  448,   44,  410, 1318,  436,  368,  333,  376,    0,
  390,    0,  380,  421,  270,  513,  297,   59,  432,   44,
  397,  389,  303,  400,  305,  333,  486,  264,  435,  390,
  444,  410,  446,   59,   59,    0, 1491,  405,  294,  416,
  403, 1690,  368,  405,  333,    0,   59,  303,    0,  428,
   41,  396,   41,   44,  380,   44,   59, 1431,  276,  277,
  368,  410,  421,  344,  432,  442,  284,  412,   59,  264,
   59,  265,  380,   44, 1395, 1395,  444,  260,  446,  493,
    0,  337,  260,  462,  452,  341,  390,  513,   59,  402,
  390,    0,    0, 1792,   59,  458, 1451, 1796,  390,  513,
  446,  509,  509,  517,   59,  299,  273,   59, 1807,    0,
  338, 1431,   40,  481,  482,    0,  484,  300,  258, 1439,
 1440, 1441,  300,  491,  492,  493,  494,  495, 1827,  497,
  270, 1451,  500,  501,  502,  503,  504,    0,  416,   59,
  508,  509,  260,  371,  260,  513,  514,  515,  516,  517,
   59,   59,    0,  390,  372,  517, 1855,  297,  376,   42,
   43, 1860,   45,  303,   47,  305, 1486,    0,   59,  397,
    0, 1491,  400,  258,   59,  393,  261, 1632, 1498, 1499,
  436,  399,  300,  259,  300,  441,  442,  443,  416,  318,
  252, 1646,  453,  269,   46,  390,   59, 1804,  260,  298,
  264,  452,  331, 1523,  344, 1645,   58,  306,    0,   61,
  339,   59,   46,    0,  442,  433,  313,  314,  303,    0,
 1523,    0,    0, 1830,   58,  354,   59,   61,  509,   59,
  481,  482, 1632,  484,  276,  277, 1597,    0,  313,  368,
  491,  492,  284,  494,  495,    0,  497,    0,  315,  500,
  501,  502,  503,  504,    0,  511, 1863,  508,  509,  515,
 1674,  338,    0,  514,  515,  516, 1617,   59, 1875,  338,
  258, 1878,   59, 1594, 1594,  493,  405, 1597,   59, 1886,
   59,   59,  270,    0,  485,  486,   41,  276,  277,   44,
  449,  450,  451,  333,  371,  284,   59, 1617,   42,   43,
  452,   45,  371,   47,   59,  332,   59,   42,   43, 1629,
   45,    0,   47,   59, 1675,  303,  444,  305,  477,  478,
  397,   59,  403,  400,   41,  338, 1646,   44,  397, 1649,
  372,  400, 1641,   41, 1696,   45,   44, 1792, 1700,  416,
  395, 1796,   59, 1798,   61, 1665,  457,  416,  459, 1804,
 1670,  393, 1807,  453,  454, 1675, 1770,  399,  371,  352,
  384,  385,   46,   42,   43,  442,   45, 1670,   47,  509,
   59, 1745, 1827,  442,   58, 1830,   41,   61,  395,   44,
  453,  454,   41,  372,  397,   44,  338,  400,  517,  336,
   40,  433, 1792,  386,  277,  388, 1796,    0, 1798,  282,
 1855,  317, 1722,  416,  393, 1860,  376, 1807, 1863,  426,
  399,   41,   41,  315,   44,   44,  672,  410,   41,   41,
 1875,   44,   44, 1878,  441, 1745, 1746, 1827,  338,  442,
  340, 1886,   41, 1747,  317,   44,  692,    0,   41,  395,
  696,   44,  435,   41,  433,  397,   44,  703,  400, 1804,
  376,  493,  708,  709,  498, 1855,   59, 1812,   41,   41,
 1860,   44,   44,  719,  416,  721,  722,  723,  724,  725,
  726,  727,  356,    0,   41, 1830,  442,   44,   41, 1799,
 1789,   44,   41,   41, 1804,   44,   44,  428,   41, 1809,
  442,   44, 1812, 1261,  426, 1263,   59,   42,   43, 1823,
   45, 1825,   47, 1823,  493, 1825,  377,  262, 1863,  765,
 1830,  375,   41,   41,   41,   44,   44,   44,  497,  409,
 1875,   42,   43, 1878,   45,   41,   47,   41,   44,  785,
   44, 1886,   59,  277,   41,  791,  792,   44,  282,  508,
  508,  258,  277, 1863, 1864,  509,  461,  282,   42,   43,
    0,   45,  284,   47,   44, 1875,  273, 1877, 1878,   61,
  315,  366,  283,  318,   40,   59, 1886,  264,   40,  389,
  325,  315,  327,   40,    0,   40,  331,   40,  453,   40,
  297,  336,   42,   43,  339,   45,  303,   47,   40,  306,
   40,   41,   42,   43,   44,   45,   46,   47,  277,  354,
 1632,  336,   40,  282,   40,   40,   40,  453,  453,   59,
   60,   61,   62,  368,   44,   41,   40,   40,   44,   40,
  691,  376,  693,   40,  379,  380,  697,  698,  699,  308,
   40,  348,   91,   59,  389,   46,   61,   40,  318,  264,
  428,   91,  317,   93,   44,  395,  317,  903,  904,  905,
  405,  331,  908,  909,  910,   44,  912,  913,  914,  915,
  916,  917,   42,   43,  420,   45,   44,   47,   61,   46,
  926,  927,  427,  929,  354,  931,   40,  432,  934,  935,
  936,  937,  938,  939,  264,   44,  403,  344,  368,  444,
  344,  446,  344,  410,  264,  951,  376,  953,  276,   41,
   42,   43,  276,   45,  297,   47,  428,  390,  462,  390,
  450,  428,  315,  276,  277,  318,  344,   40,  281,  264,
  275,  284,  325,  439,  327,  405,  439,   40,  331,  392,
  345,   40,  277,  336,  297,   91,  339,  282,  493,   40,
  450,  458,  452,   40,  265,  462,  315,  264, 1004,  276,
  277,  354,  376,   44,  281,  403,  277,  284,  513,    0,
 1792,  282,  517,   40, 1796,  368, 1798,    0,  382,  375,
  297,  481,  482,  376,  484, 1807,  379,  380,  299,   40,
   40,  491,  492,  277,  494,  495,  389,  497,  282,  380,
  500,  501,  502,  503,  504, 1827,  443,   41,  508,  509,
   41,  275,  405,   44,  514,  515,  516,   40,   41,  372,
   63,   44,  262,  493,  264,  265,  296,  277,   59,  456,
  428,  271,  282, 1855,  427,  314,   59,  277, 1860,  432,
  393,   40,  282,  317,  317,  285,  399,  517,  440,  444,
   44,  444,  336,  446,  264,  372,   40,   40,  308,  299,
  276,  277,  494,  297,  304,  305,  379,    0,  308,  273,
  276,  340,  281,  340,  379,  315,  393,  317,  318,  379,
  433,  321,  399,  410,  408,  325,   40,  327,  410,  329,
   40,  331,  297,  352,  264,  291,  336,  337,   40,  339,
  493,  362,   40,   44,   41, 1151,  302,  277,   41,  317,
  438,   44,  282,  309,  354,  355,  433,  357, 1164, 1165,
  513, 1167, 1168,  270,  517,  277,   59,   44,  368, 1175,
  370,  378,  372,  373,  374,   44,  376,  333,  361,  379,
  380,  317,   40,   44,  325,  277,  266,   40,   40,  389,
  282, 1197,  493,  264,  414,   41,  372,   41,   58, 1205,
 1206, 1207,  402,   41, 1210,  405, 1212,   41,  264,  446,
 1216, 1217, 1218,  306,   41,  387,  306,  393,   41,   40,
   40,  421,  317,  399,   40,   40,   40,  427,  428,   40,
   40,   40,  432,   40,   40,   40,   40,   40,  260,  395,
   40,   40,   40,  443,  444,  262,  446,  305,  427,  324,
 1256,  908,  452,  910,  281,  912,  913,  433,  915,  916,
  917,   41,   42,   43,   41,   45,   44,   47,   41,  264,
  426,  381,  339,  339,  430,  273,  298,    0,  327,  384,
  284,  481,  482,  399,  484,  276,  277,  450,  310, 1295,
  379,  491,  492,  493,  494,  495,  379,  497,  379,   41,
  500,  501,  502,  503,  504,  379,   41,  379,  508,  509,
  379,  372,  334,  513,  514,  515,  516,  517,   41,   42,
   43,   44,   45,   46,   47,   41,   42,   43,  276,   45,
   40,   47,  315,   41,   41,  318,   59,   60,   61,   62,
   41,  317,  325,   41,  327,   44,  409,   41,  331,  305,
  427,  290,   40,  336,   40,   59,  339,   59, 1364,  336,
  305,  336,   59,   59, 1370,  378,   59,   59,   91, 1375,
   93,  354,   58,  428,   59,   59, 1382,  399,   59,   58,
  333,  372,  339,  276,  277,  368,  277,  409,  434,  452,
  434,  284,  334,  376,   41,   42,   43,  380,   45,  298,
   47,  334,  393,  298,  297,   41,  389,   41,  399,  378,
 1416,  348,  434,  365, 1420,  365,   41,   41,  481,  482,
   41,  484,  405,   41,   41,  452,   41,   41,  491,  492,
 1436,  494,  495,   41,  497,   41,   41,  500,  501,  502,
  503,  504,  433,   41,   41,  508,  509,   41,   41,  432,
   41,  514,  515,  516,  481,  482,   41,  484,   40,  315,
   41,  444,  443,  446,  491,  492,   40,  494,  495,  452,
  497,   59,   44,  500,  501,  502,  503,  504,   41,  372,
  348,  508,  509,  340,  458,  333,   40,  514,  515,  516,
   42,   43,   44,   45, 1500,   47,  336,  277,  481,  482,
  393,  484,  282,   59,   46,  336,  399,  500,  491,  492,
  493,  494,  495,   40,  497,   40,   44,  500,  501,  502,
  503,  504,  376,   41,   40,  508,  509,  402,   41,  376,
  513,  514,  515,  516,  517,  427,   41,  376,   59,  262,
  433,  264,  265,   44,  301,  298,  434,   41,  271,  304,
   58,  290,  301,    0,  277,  455,   40,  333,  376,  282,
  409,  277,  285,  376,  264,  276,  282, 1573,  450,  264,
  317,  403,  462, 1579, 1580,   42,  299,    0,  301,   40,
  443,  304,  305,  270,   44,  308,   40,  270,   41,  264,
  427,  450,  315,  428,  317,  318,  339, 1603,  321,  339,
  450, 1607,  325,   41,  327, 1611,  329,   41,  331,   41,
 1616,   59, 1618,  336,  337,   59,  339,   41,   41,   42,
   43,   44,   45,   46,   47,  305,   41,  301,   59,   59,
  277,  354,  355,   59,  357,  282,   59,   60,   61,   62,
    0,    0,   41,   42,   43,  368,   45,  370,   47,  372,
  373,  374,    0,  376, 1239,   12,  379,  380,  289,  282,
  745,  281, 1126,   42,  656,  883,  389, 1124,   91,   90,
   93,   41,   42,   43,  888,   45, 1121,   47, 1116,  402,
 1542, 1541,  405,  845,  349,  605, 1106,  329,  379, 1695,
   42,   43,   44,   45, 1117,   47,  787,  361,  421,  361,
 1612, 1052,  813, 1665,  427,  428, 1522, 1520, 1815,  432,
  464,  465,  466,  467,  468,  469,  470,  471,  583, 1819,
  443,  444, 1844,  446, 1818,  277,  804,  983, 1734,  452,
  282,   41,   42,   43, 1734,   45, 1439,   47, 1637, 1805,
 1274, 1505,  354,  733,  366,  347, 1327,  862,  642,  585,
  854,   13,  316,  608,  524,  776,  528, 1763,  481,  482,
  149,  484,   42,   43,   44,   45,  452,   47,  491,  492,
  493,  494,  495, 1015,  497, 1262,  765,  500,  501,  502,
  503,  504,   86, 1763,  689,  508,  509, 1594, 1395,  791,
  513,  514,  515,  516,  517,  481,  482,  752,  484, 1005,
 1806,  539, 1282, 1166, 1212,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,  293,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   44,   -1,   -1,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,
   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,   -1,  285,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,  277,   -1,
   -1,  304,  305,  282,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,  277,  331,   -1,
   -1,   -1,  282,  336,  337,   -1,  339,   -1,   41,   42,
   43,   44,   45,   46,   47,  277,   -1,   -1,   -1,   -1,
  282,  354,  355,   -1,  357,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,  277,   91,   -1,
   93,   -1,  282,  481,  482,   -1,  484,   -1,   -1,  402,
   59,   -1,  405,  491,  492,  452,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,  277,  421,   -1,
  508,  509,  282,   -1,  427,  428,  514,  515,  516,  432,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  443,  444,   -1,  446,  491,  492,   -1,  494,  495,  452,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   59,  491,  492,
  493,  494,  495,  452,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,   -1,   -1,   -1,   -1,  277,
   -1,   -1,  481,  482,  282,  484,   -1,   -1,   -1,  317,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,
   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,   -1,  285,   41,   42,   43,   -1,   45,    0,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   41,
   -1,   -1,   44,  336,  337,   -1,  339,   -1,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   59,   -1,   61,
   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   91,   -1,
   93,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,  402,
   59,   -1,  405,  491,  492,  277,  494,  495,   -1,  497,
  282,   -1,  500,  501,  502,  503,  504,   -1,  421,   -1,
  508,  509,   -1,   -1,  427,  428,  514,  515,  516,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,  444,   -1,  446,   41,   42,   43,   -1,   45,  452,
   47,   -1,   -1,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   41,   42,   43,   -1,   45,   -1,
   47,   41,   42,   43,   -1,   45,   -1,   47,  481,  482,
   -1,  484,   42,   43,   -1,   45,   -1,   47,  491,  492,
  493,  494,  495,  452,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,   -1,   -1,   -1,   -1,  277,
   -1,   -1,  481,  482,  282,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,  258,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  262,
   -1,  264,  265,  275,  276,  277,   -1,   -1,  271,  281,
   -1,   -1,  284,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,   -1,  285,   -1,   -1,  297,   -1,   -1,    0,   42,
   43,  303,   45,   -1,   47,   -1,  299,    0,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   41,
   -1,   -1,   44,  336,  337,   -1,  339,   -1,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   59,   -1,   61,
   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,   62,
  372,   -1,   41,   42,   43,  368,   45,  370,   47,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
  277,  393,   -1,   -1,   -1,  282,  389,  399,   91,  277,
   93,   -1,   -1,   -1,  282,   -1,  408,   -1,  410,  402,
  277,   -1,  405,   -1,   -1,  282,   -1,  277,   -1,   41,
   42,   43,  282,   45,   -1,   47,  428,  277,  421,   -1,
   -1,  433,  282,   -1,  427,  428,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
  462,   -1,   41,   42,   43,   -1,   45,   -1,   47,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,  452,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  262,
  513,  514,  515,  516,  517,   -1,   42,   43,   -1,   45,
   -1,   47,  481,  482,  277,  484,   -1,   -1,   -1,  282,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,  258,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  262,
   -1,  264,  265,  275,  276,  277,   -1,   -1,  271,  281,
   -1,   -1,  284,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,   -1,  285,   -1,   -1,  297,   -1,   -1,    0,   -1,
   -1,  303,   -1,   -1,   -1,   -1,  299,    0,  277,   -1,
   -1,  304,  305,  282,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,  284,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   41,
   -1,   -1,   44,  336,  337,  291,  339,   -1,   41,   42,
   43,   44,   45,   -1,   47,  277,  302,   59,   -1,   61,
  282,  354,  355,  309,  357,   -1,   59,   60,   61,   62,
  372,   -1,   -1,   -1,   -1,  368,  322,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,  393,   -1,   -1,   -1,   -1,  389,  399,  277,   -1,
   93,   -1,   -1,  282,   -1,  277,   -1,   -1,  410,  402,
  282,   -1,  405,  277,   -1,   -1,   -1,   -1,  282,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,  421,   -1,
   -1,  433,   -1,  379,  427,  428,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,  395,
  443,  444,   45,  446,   -1,   -1,   -1,   -1,   -1,  452,
  462,  277,   -1,   -1,   -1,  411,  282,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  426,   -1,   -1,   -1,  430,   -1,   -1,  433,  481,  482,
   -1,  484,   -1,   -1,   -1,  441,   -1,  452,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,  460,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,  258,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  264,  265,   -1,  276,  277,   -1,   -1,  271,  281,
   -1,   -1,  284,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,   -1,  285,   -1,   -1,  297,   -1,   -1,   -1,   -1,
   -1,  303,   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,
   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,   62,
  372,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,  393,   -1,   -1,   -1,   -1,  389,  399,   -1,  292,
   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,  402,
  302,   -1,  405,    0,   -1,   -1,   -1,  309,   -1,   -1,
   -1,  313,  314,   -1,   -1,   -1,  428,  320,  421,   -1,
  322,  433,   -1,   -1,  427,  428,   -1,   -1,   -1,  432,
   -1,  333,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  443,  444,   -1,  446,   41,  348,   -1,   44,   -1,  452,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   41,  481,  482,
   44,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,  395,  497,   59,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  411,
  513,  514,  515,  516,  517,   41,  419,   -1,   44,   -1,
   -1,   -1,   -1,   -1,  426,   -1,   -1,   -1,  430,   -1,
   -1,   -1,  435,   59,   -1,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,   -1,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,  460,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,  472,
  473,  474,  475,  476,  477,  478,  479,  480,   -1,  282,
   -1,   -1,  285,   -1,  487,  488,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  496,   -1,   -1,  299,    0,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,
   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,  276,
  277,   -1,   -1,   -1,   -1,   -1,  389,  284,   -1,   -1,
   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,
  297,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,  277,   -1,   -1,   -1,   -1,  421,   -1,
  284,   -1,   -1,   -1,  427,  428,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,  444,   -1,  446, 1365, 1366,   -1,   -1,   -1,  452,
  276,  277, 1373, 1374,   -1, 1376, 1377,   -1,  284,   -1,
   -1,   -1,   -1, 1384, 1385, 1386, 1387,   -1, 1389, 1390,
 1391, 1392, 1393, 1394,   -1,  372,   -1,   -1,  481,  482,
    0,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,  393,  500,  501,  502,
  503,  504,  399,   -1,   -1,  508,  509,   -1,  372,   -1,
  513,  514,  515,  516,  517,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,   -1,  399,  433,  325,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,  393,  271,  433,
   -1,   91,   -1,  399,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,  433,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,
   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,   -1,  435,   -1,   -1,
   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   93,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,  402,
   -1,   -1,  405,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,   -1,
  508,  509,   -1,   -1,  427,  428,  514,  515,  516,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,
  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   -1,  273,   -1,  275,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,  481,  482,
   -1,  484,   -1,  303,   -1,   -1,  306,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,  284,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,
   62,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,
   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,   -1,  399,
   -1,   -1,  285,  403,   -1,   -1,    0,   -1,   -1,   -1,
  410,   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,   -1,  428,   -1,
   -1,   -1,  315,  433,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   41,  331,   43,
   44,   45,   -1,  336,  337,   -1,  339,   -1,  458,   -1,
   -1,   -1,  462,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   93,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,  435,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,  402,
   -1,   -1,  405,  491,  492,  452,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,   -1,
  508,  509,   -1,   -1,  427,  428,  514,  515,  516,  432,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  443,  444,   -1,  446,  491,  492,   -1,  494,  495,  452,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  271,
   -1,  508,  509,   -1,   -1,  277,   -1,  514,  515,  516,
  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  321,
  513,  514,  515,  516,  517,   -1,   -1,  329,   -1,   42,
   43,   -1,   45,   -1,   47,  337,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,   62,
   -1,   -1,   -1,  355,   -1,  357,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,   -1,   -1,   -1,  271,  370,   -1,
  372,  373,  374,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,
  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,  421,
   -1,  325,   -1,  327,   -1,  329,   41,  331,   43,   44,
   45,  426,  336,  337,   -1,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  354,  355,   -1,  357,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,   -1,  379,  380,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,  389,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  402,  494,
  495,  405,  497,   -1,  452,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,  421,   -1,  514,
  515,  516,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,  443,
  444,   -1,  446,  491,  492,   -1,  494,  495,  452,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,  271,   -1,
  508,  509,   -1,   -1,  277,   -1,  514,  515,  516,  282,
   -1,   -1,  285,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  321,  513,
  514,  515,  516,  517,   -1,   -1,  329,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  337,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  261,   -1,   -1,   -1,
   -1,   -1,  355,   -1,  357,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,  279,   -1,  271,  370,   -1,  372,
  373,  374,  287,   -1,   -1,   -1,   -1,   -1,   -1,  294,
  285,  296,   -1,  298,    0,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,   -1,  299,  310,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,  324,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,  421,  334,
  325,   -1,  327,   -1,  329,   41,  331,   43,   44,   45,
   -1,  336,  337,   -1,  339,  272,   -1,   -1,  275,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,
  355,  288,  357,   -1,   -1,  292,   -1,   -1,  295,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,
   -1,   -1,  397,   -1,  389,   -1,   -1,   -1,   -1,  404,
   -1,   -1,  407,   -1,  409,  410,   -1,  402,  335,   -1,
  405,   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,   -1,  421,   -1,   -1,  434,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
  367,   -1,  369,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,  403,   -1,  483,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,
   -1,   -1,   -1,  498,  499,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
  437,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,  489,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   41,  331,   43,   44,   45,   -1,
  336,  337,   -1,  339,  272,   -1,   -1,  275,   -1,   -1,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,
  288,  357,   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,  335,   -1,  405,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,  367,
   -1,  369,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,  437,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  472,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,  489,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   59,   60,   61,   62,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   91,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,    0,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  402,  494,  495,  405,  497,
   -1,  452,  500,  501,  502,  503,  504,   -1,   40,   41,
  508,  509,   44,   -1,  421,   -1,  514,  515,  516,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   59,   -1,   61,
  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,  446,
  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   91,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,    0,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,  262,  513,  514,  515,  516,
  517,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   91,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,  258,   -1,   -1,   -1,
   -1,   -1,  389,  272,   -1,   -1,  275,   -1,    0,   -1,
   -1,  273,   -1,  275,  276,  277,   -1,   -1,  405,  281,
   -1,   41,  284,  292,   44,   -1,  295,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,  297,   -1,   -1,   -1,   59,
  427,  303,   -1,   -1,  306,  432,   -1,   -1,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,  335,   59,   60,   61,
   62,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,   -1,   -1,  348,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,   91,
  369,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,
  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  513,   -1,   -1,  398,
  517,  393,   -1,  271,   -1,   -1,   -1,  399,   -1,  277,
   -1,  403,   -1,   -1,  282,   -1,  415,  285,  410,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,  437,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,  321,    0,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,  458,   -1,  336,  337,
  462,  339,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,  354,  355,   -1,  357,
  489,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   41,    0,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
  262,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  271,
   -1,   -1,   -1,  421,   -1,  277,   -1,   -1,   -1,  427,
  282,   -1,   -1,  285,  432,  315,   -1,   41,  318,   -1,
   44,   -1,   -1,   -1,   -1,  325,  444,  327,  446,   -1,
   -1,  331,   -1,   -1,   -1,   59,  336,   -1,   -1,  339,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,  354,  327,   -1,  329,   -1,  331,
   -1,   -1,   -1,   -1,  336,  337,   -1,  339,  368,   -1,
   -1,   -1,   -1,   -1,   -1,  493,  376,   -1,   -1,   -1,
  380,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,  513,  368,   -1,  370,  517,
  372,  373,  374,   -1,  376,  405,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,  432,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,  421,
   -1,   -1,  452,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,  262,   -1,   41,   -1,
   -1,   44,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   59,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,  262,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
    0,   -1,   -1,  513,  514,  515,  516,  517,   -1,  315,
   -1,  493,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,  262,   -1,
  336,  513,   -1,  339,   -1,  517,   -1,   -1,   -1,  315,
   -1,   41,  318,   -1,   44,   -1,   -1,   -1,  354,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   59,
  336,   -1,  368,  339,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  354,   -1,
   -1,  315,   -1,  389,  318,   -1,   -1,   -1,   -1,    0,
   -1,  325,  368,  327,   -1,   -1,   -1,  331,   -1,  405,
  376,   -1,  336,  379,  380,  339,   -1,   -1,   -1,   -1,
   -1,   -1,  339,  389,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  427,    0,   -1,   -1,   -1,  432,   -1,   -1,  405,
   41,   -1,   -1,   44,  368,  362,   -1,   -1,  444,   -1,
  446,   -1,  376,   -1,   -1,  379,  380,   -1,   59,   -1,
   -1,  427,   -1,   -1,   -1,  389,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   41,    0,   -1,   44,   -1,  444,   -1,
  446,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   59,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,  418,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,  513,   44,   -1,
  444,  517,  446,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,   -1,   59,   -1,  452,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,   -1,  513,   -1,   -1,
   -1,  517,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,  481,  482,  339,  484,    0,  493,
   -1,   -1,  262,   -1,  491,  492,   -1,  494,  495,   -1,
  497,  354,   -1,  500,  501,  502,  503,  504,   -1,  513,
   -1,  508,  509,  517,   -1,  368,   -1,  514,  515,  516,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,   59,  318,   -1,
   -1,   -1,  405,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,    0,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,
   -1,  262,   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,   41,   -1,  376,   44,   -1,  379,
  380,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,  405,   -1,  318,   -1,   -1,
  493,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,    0,   -1,   -1,  336,  262,  427,  339,   -1,
  513,   -1,  432,   -1,  517,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,  354,  444,   -1,  446,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,    0,   -1,  368,  336,   -1,
   -1,  339,   -1,   41,   -1,  376,   44,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,  389,  315,
   -1,   59,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
  368,  327,   -1,  493,  405,  331,   41,    0,  376,   44,
  336,  379,  380,  339,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,  513,   59,   -1,  427,  517,  354,   -1,
  262,  432,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,  368,  444,   -1,  446,   -1,   -1,   41,   -1,
  376,   44,   -1,  379,  380,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,  389,  432,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,  405,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,
   -1,   -1,  493,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,  427,   -1,   -1,  336,  262,  432,  339,   -1,   -1,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,  444,   -1,
  446,   -1,  354,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  376,  513,   -1,  379,  380,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,  315,   -1,
   -1,  318,   -1,   -1,   -1,    0,   -1,  493,  325,   -1,
  327,   -1,   -1,  405,  331,   -1,   -1,   -1,   41,  336,
   -1,   44,  339,   -1,   -1,   -1,   -1,  513,   -1,   -1,
   -1,  517,   -1,   -1,  262,  427,   59,  354,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,  368,  444,   -1,  446,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   59,   -1,   -1,  262,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,  405,   -1,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,  493,   -1,  331,   -1,   -1,   -1,   -1,  336,  262,
  427,  339,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
  315,  513,   -1,  318,   -1,  517,  354,  444,   -1,  446,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
  368,  336,   -1,   -1,  339,   -1,   -1,   -1,  376,    0,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,  389,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,  368,  327,   -1,  493,  405,  331,   -1,
   -1,  376,   -1,  336,  379,  380,  339,   -1,   -1,   -1,
   41,   -1,   -1,   44,  389,   -1,  513,   -1,   -1,  427,
  517,  354,   -1,   -1,  432,   -1,   -1,   -1,   59,    0,
  405,   -1,   -1,   -1,   -1,  368,  444,   -1,  446,   -1,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,  389,  432,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   41,  446,  405,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,  262,   59,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,    0,  513,   -1,   -1,   -1,  517,
   -1,  444,  315,  446,   -1,  318,   -1,   -1,  493,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,  513,   -1,
  315,   -1,  517,  318,   -1,   41,   -1,   -1,   44,   -1,
  325,  354,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
  493,  336,   -1,   59,  339,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,  354,
  513,   -1,   -1,   -1,  517,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,  405,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,    0,   -1,   -1,  432,
  405,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,    0,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,  444,
   44,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   59,   -1,  318,   -1,   -1,
  493,  262,   -1,   -1,  325,   41,  327,   -1,   44,   -1,
  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,
  513,   -1,   -1,   59,  517,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,  513,   -1,
   -1,   -1,  517,   -1,  315,  376,   -1,  318,  379,  380,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,  389,   -1,
  331,   -1,   -1,   -1,   -1,  336,  262,   -1,  339,   -1,
   41,   -1,   -1,   44,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,  368,   -1,   -1,
   -1,  432,   -1,   -1,   -1,  376,    0,   -1,  379,  380,
   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,  389,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,   -1,  405,  331,   -1,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,  427,   -1,  354,   -1,
   -1,  432,  493,   -1,   -1,   59,   -1,   -1,   -1,    0,
   -1,   -1,  368,  444,   -1,  446,   -1,   -1,   -1,   -1,
  376,   -1,  513,  379,  380,   -1,  517,   -1,  262,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,
   41,   -1,   -1,   44,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,  315,  513,   -1,  318,   -1,  517,   -1,  444,   -1,
  446,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
  354,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,  262,   -1,  339,  368,   -1,   -1,  493,   -1,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,  513,   -1,   -1,
   -1,  517,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,  405,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,   -1,  427,  325,   -1,  327,   -1,  432,  405,
  331,   -1,   -1,   -1,   40,  336,   -1,   43,  339,   45,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   58,  354,   -1,   -1,  432,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,  444,   -1,
  446,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,  493,
   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,
   -1,  325,   -1,  327,  405,   -1,   -1,  331,   -1,  513,
   -1,   -1,  336,  517,   -1,  339,   -1,  493,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
  354,  432,   -1,   -1,   -1,   -1,   -1,  513,   -1,  260,
   -1,  517,   -1,  444,  368,  446,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  315,   -1,  380,  318,   -1,   -1,
   -1,   -1,   -1,   -1,  325,  389,  327,   -1,  272,   -1,
  331,  275,   -1,   -1,   -1,  336,   -1,  298,  339,   -1,
   -1,  405,   -1,   -1,  288,   -1,   -1,   -1,  292,  310,
   -1,  295,  493,  354,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  324,   -1,   -1,   -1,  368,  432,   -1,
   -1,   -1,  513,  334,   -1,  376,  517,   -1,   -1,  380,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,
   -1,  345,  346,  347,  405,  349,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,  367,   -1,  369,   -1,   -1,  274,  493,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,  399,   40,
  286,   -1,   43,  444,   45,  446,  292,  293,  409,  513,
   -1,   -1,   -1,  517,  398,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,  415,   -1,  434,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,  493,  437,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,  513,   -1,  360,   -1,  517,   -1,  364,   -1,
  481,  482,   -1,  484,   -1,   -1,  372,   -1,  472,  473,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,  260,  489,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  409,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  310,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,  334,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  505,
  506,  507,  508,  509,   -1,   -1,  267,   -1,  514,  515,
  516,   40,   -1,  274,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,  399,   -1,  286,   -1,   -1,  289,   58,
   -1,  292,  293,  409,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,  481,  482,   -1,  484,   -1,
   -1,  372,   -1,   -1,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,  270,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,  298,
   -1,   -1,   -1,   -1,  435,   -1,  305,   -1,   -1,   -1,
  309,  310,   -1,  312,   -1,   -1,  315,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,  328,
   -1,   -1,  463,   -1,   -1,  334,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,  267,  500,
  501,  502,  503,  504,   -1,  274,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   40,  286,   -1,   43,
   -1,   45,  391,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
  409,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,  424,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,  445,   -1,   -1,  348,
   -1,  350,  351,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,  272,  364,   -1,  275,   -1,   -1,
   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  292,  484,   -1,  295,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,  413,  514,  515,  516,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,  335,   -1,   -1,
   -1,   -1,   -1,  341,  342,  343,  435,  345,  346,  347,
   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,  367,
   -1,  369,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
  398,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  415,   -1,  508,
  509,   -1,   -1,  267,   -1,  514,  515,  516,   40,   -1,
  274,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,  437,
   -1,   -1,  286,   -1,   -1,   -1,   58,   -1,  292,  293,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,  272,   -1,   -1,  275,   -1,  372,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,  290,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  435,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,  463,
  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,  398,
  494,  495,  496,  497,   -1,  267,  500,  501,  502,  503,
  504,   -1,  274,   -1,  508,  509,  415,   -1,   -1,   -1,
  514,  515,  516,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
  489,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,  260,   -1,   -1,  263,   -1,
  372,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   58,   -1,  425,   -1,  320,   63,   -1,  323,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  270,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,  298,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,   -1,
  312,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  463,   -1,
   -1,   -1,  334,   -1,   -1,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  260,   -1,   -1,  263,   -1,   -1,  391,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   40,  424,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,  443,   -1,  320,   63,   -1,  323,   -1,   -1,   -1,
  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,  360,   -1,   -1,   -1,  364,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   58,   -1,  425,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   58,   -1,  425,   -1,  320,   63,
   -1,  323,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  270,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,  298,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
  309,  310,   -1,  312,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,  328,
   -1,  463,   -1,   -1,   -1,  334,   -1,   -1,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  260,   -1,   -1,  263,
   -1,   -1,  391,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   40,  424,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,  320,   63,   -1,  323,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,  360,   -1,   -1,   -1,
  364,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   58,
   -1,  425,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,    0,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   91,   -1,   -1,  364,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   58,   -1,  425,
   -1,  320,   63,   -1,  323,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,  258,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,  273,   -1,  275,
  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,  297,   -1,   -1,  463,   -1,   -1,  303,   -1,   -1,
  306,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,  348,   -1,   -1,  514,  515,  516,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,  372,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   58,   -1,  393,   -1,   -1,
   63,   -1,   -1,  399,   -1,   -1,   -1,  403,   -1,   -1,
   -1,   -1,   -1,   -1,  410,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,  458,  364,   -1,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,  260,   -1,  500,
  501,  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,
   -1,  274,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   40,  260,   42,   43,   -1,   45,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,  325,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
  267,  487,  488,   -1,  490,  491,  492,  274,  494,  495,
  496,  497,  395,   -1,  500,  501,  502,  503,  504,  286,
   -1,   -1,  508,  509,   -1,  292,  293,   -1,  514,  515,
  516,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  430,   -1,   58,
   -1,   -1,  435,  320,   63,   -1,  323,   -1,  441,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   40,   41,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,  267,  487,  488,
   -1,  490,  491,  492,  274,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  286,   -1,   -1,  508,
  509,   -1,  292,  293,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,  267,  487,  488,   -1,  490,  491,  492,
  274,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  286,   -1,   -1,  508,  509,   -1,  292,  293,
   -1,  514,  515,  516,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,  320,   63,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   58,
   59,  425,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  446,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   40,   -1,   42,   43,   -1,   45,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  446,   -1,   -1,  449,  450,  451,  452,  453,  454,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,  267,  487,  488,
   -1,  490,  491,  492,  274,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  286,   -1,   -1,  508,
  509,   -1,  292,  293,   -1,  514,  515,  516,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
  320,   63,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   58,   -1,  425,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,  267,  487,  488,   -1,  490,  491,  492,  274,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
  286,   -1,   41,  508,  509,   44,  292,  293,   -1,  514,
  515,  516,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   91,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,  360,   61,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  258,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,
  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  348,
   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,
  306,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,
   -1,  281,   -1,  372,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,
   -1,   -1,   -1,  303,  393,   -1,  306,   -1,   -1,   -1,
  399,   -1,  348,   -1,  403,   -1,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,  428,
   -1,   -1,  272,   -1,  433,  275,   -1,   -1,  348,   -1,
   -1,  281,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,
   -1,   -1,  292,  399,   -1,  295,   -1,  403,   -1,  458,
   -1,   -1,  372,  462,  410,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  428,  393,   -1,   -1,   -1,  433,   -1,  399,
   -1,  272,   -1,  403,  275,  335,   -1,   -1,   -1,   -1,
  410,  341,  342,  343,   -1,  345,  346,  347,   -1,  349,
   -1,  292,  458,   -1,  295,   -1,  462,   -1,  428,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,  367,   -1,  369,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,   -1,
   -1,   -1,  462,   -1,  335,   -1,   -1,   -1,  398,   -1,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,
   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,  369,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,  473,  415,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,  489,
   -1,  491,  492,   -1,  494,  495,  437,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,  452,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,  489,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  452,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,  461,  462,
  463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
  473,  474,  475,  476,   -1,   -1,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,   -1,  491,  492,
  493,  494,  495,  496,  497,  498,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,
  513,  514,  515,  516,  517,  452,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  462,  463,  464,  465,  466,
  467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
   -1,   -1,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,   -1,  491,  492,  493,  494,  495,  496,
  497,  498,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,  513,  514,  515,  516,
  517,
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
"delete_searched : KW_DELETE FROM table_name where_clause plan_clause order_clause rows_clause",
"delete_positioned : KW_DELETE FROM table_name cursor_clause",
"cursor_clause : WHERE CURRENT OF symbol_cursor_name",
"update : update_searched",
"update : update_positioned",
"update_searched : UPDATE table_name SET assignments where_clause plan_clause order_clause rows_clause",
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
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 745:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 749:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 750:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
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
