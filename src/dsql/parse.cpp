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
  391,  391,  391,  391,  391,  392,  392,  392,  392,  392,
  392,  392,  392,  397,  397,  397,  397,  397,  397,  397,
  397,  397,  397,  397,  397,  397,  397,  397,  397,  402,
  402,  393,  393,  394,  394,  394,  394,  395,  395,  399,
  399,  400,  400,  400,  400,  398,  401,  396,  396,  387,
  387,  387,  390,  390,  390,  403,  403,  404,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,  150,  150,  150,  405,  211,
  211,  145,  145,  411,  411,  411,  411,  411,  407,  407,
  407,  407,  407,  225,  146,  146,  147,  148,  148,  148,
  148,  148,   52,   52,   21,   21,  235,  412,   55,  273,
  266,  266,   99,  406,  406,  406,  406,  413,  413,  413,
  413,  413,  413,  413,  413,  413,  413,  413,  414,  415,
  417,  416,  416,  419,  420,  420,  410,  410,  408,  409,
  409,  421,  421,  421,  422,  422,  423,  423,  426,  426,
  424,  424,  429,  429,  428,  425,  427,  418,  418,  418,
  418,  418,  418,  418,  418,  333,  333,  317,  317,  149,
   49,   65,  124,  142,  386,  159,  183,  259,   67,   63,
  217,  116,   70,  337,  213,   36,   42,  346,  258,   40,
   43,  210,   41,  286,  253,  253,  430,  430,  430,  430,
  430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
  430,  430,  430,  430,
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
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    3,    3,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    1,
    1,    5,    6,    3,    4,    5,    6,    3,    4,    3,
    4,    3,    4,    4,    5,    4,    4,    3,    4,    1,
    1,    1,    1,    1,    1,    1,    3,    3,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    2,    3,
    3,    3,    3,    3,    3,    3,    3,    1,    1,    1,
    1,    3,    1,    1,    1,    1,    1,    1,    4,    1,
    3,    1,    2,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    2,    1,    1,    1,    1,
    1,    2,    1,    1,    1,    1,    1,    4,    5,    5,
    5,    5,    5,    5,    5,    5,    5,    5,    6,    1,
    6,    1,    4,    7,    2,    0,    4,    3,    6,    1,
    1,    6,    8,    6,    1,    1,    4,    6,    4,    5,
    3,    5,    4,    5,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    0,    1,    0,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    0,  555,  556,  557,  561,  562,  563,    0,  627,  630,
  742,  743,  747,  748,  468,    0,    0,    0,    0,    0,
    0,  400,  573,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  122,    0,    0,  103,    0,  917,    0,
   22,  915,    0,    0,   74,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  481,    0,
    0,    0,   38,   35,   37,    0,   36,    0, 1005, 1007,
 1009, 1008, 1014, 1010, 1015, 1011, 1013, 1016, 1012, 1017,
 1018, 1019, 1021, 1022, 1020, 1023, 1024,    0,    0,    0,
    0,   33,   69,   67,  997, 1006,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  638,
    0,    0,    0,  685,  999,    0,    0,    0,    0,    0,
  115, 1004,  564,    0,    2,    0,    0,    0,    0,    0,
    0,  430,  431,  432,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  456,  450,  451,  452,  453,  454,  455,  457,    0,
  428,  429,    0,  989,    0,  407,  993,    0,  404,  996,
    0,  687,    0,  403, 1000,    0,  571,  913,    0,  158,
  111,    0,  112,    0,  208,  985,  779,    0,  110,  153,
  992,    0,    0,  106,    0,  113,  919,    0,  107,    0,
  108,    0,  109, 1003,  154,  114,  730,  731,    0,  918,
  916,    0,   75,    0,  990,    0,  988,  489,  482,    0,
  488,  493,  483,  484,  491,  485,  486,  487,  492,    0,
    0,    0,   31,    0,   40,  766,   39,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  923,    0,  904,  632,  632,  633,  643,  645,
  976,  646,    0,  647,  637,  633,    0,    0,    0,    0,
    0,    0,    0,  590,  579,    0,  582,  584,  585,  586,
  587,  593,  594,  998,  686,    0,    0,  116,    0,  117,
    0,  118,    0,    0,    0,  617,    0,    0,  978,    0,
    0,  624,    0,    0,    0,    0,    0,  469,    0,    0,
  370,    0,  211,    0,  410,  401,  466,  467,    0,    0,
    0,    0,    0,    0,  419,    0,  376,  377,    0,  577,
  575,  914,    0,    0,    0,    0,    0,    0,    0,  161,
  140,    0,  104,    0,    0,  119,    0,  129,  128,    0,
    0,    0,    0,    0,    0,  981,   76,    0,    0,    0,
  744,  745,  490,    0,    0,    0,    0,  881,  910,    0,
  980,    0,  883,    0,    0,  911,    0,  905,  896,  895,
  894,    0,    0,    0,    0,  886,  887,  888,  897,  898,
  906,  907,    0,  908,  909,    0,    0,    0,  912,    0,
    0,    0,    0,    0,    0,  900,    0,  776,  878,  879,
  880,  885,  884,  359,  864,    0,    0,  863,    0,    0,
  860,  861,  862,  865,  866,  867,  899,  924,  925,  926,
  927,  940,  942,  950,  951,  955,  956,  209,    0,    0,
    0,  363,    0,    0,    0,  769,    0,    0,   28,    0,
    0,    0,   72,   70, 1001,   68,   34,    0,    0,    0,
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
  102,    0,  101,  982,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  782,  784,  793,  796,  797,  798,
  799,  800,  801,  802,  803,  804,  805,    0,    0,    0,
  901,    0,    0,    0,    0,    0,    0,  902,  903,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  324, 1002,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  267,  504,    0,    0,    0,
  361,    0,    0,    0,  768,   29,    0,    0,   45,   73,
    0,    0,   27,    0,  774,    0,    0,  741,    0,    0,
    0,    0,    0,  568,    0,  632,    0,  654,    0,    0,
    0,    0,  559,    0,  921,  558,    0,  592,    0,  600,
  596,  595,    0,    0,    0,  611,  599,    0,    0,  749,
  750,    0,    0,    0,  368,    0,    0,    0,    0,    0,
  138,    0,  473,  370,  413,    0,  370,  371,  418,    0,
    0,  258,  368,  231,  986,    0,    0,  370,    0,    0,
  200,  368,    0,    0,    0,    0,  236,  230,  233,  234,
  235,  422,  464,  463,  421,  420,    0,    0,    0,    0,
    0,    0,  379,    0,  370,    0,  163,  167,    0,    0,
  171,    0,    0,  761,    0,  535,  536,  552,    0,  549,
    0,    0,  542,  540,  541,    0,    0,    0,    0,  505,
    0,    0,    0,    0,  543,    0,    0,  368,    0,  191,
    0,  194,  196,  197,    0,    0,    0,    0,    0,  211,
   79,    0,    0,   78,    0,    0,    0,    0,  783,  795,
    0,    0,  853,  854,  855,    0,  787,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  968,  969,  970,  971,  972,  973,  974,  975,    0,    0,
    0,    0,    0,    0,  961,    0,    0,    0,    0,  876,
  326,    0,  877,  948,    0,  872,  984,    0,    0,    0,
    0,    0,    0,    0,  882,  778,  777,  365,  364,    0,
  265,    0,    0,    0,  273,  770,    0,    0,    0,    0,
    0,    0,    0,    0,   54,   62,   64,    0,    0,   71,
    0,    0,    0,  773,    0,    0,    0,    0,    0,    0,
    0,  641,  644,  994,  653,    0,    0,    0,    0,  659,
  660,  661,  662,  671,  672,  673,    0,    0,  651,  560,
  922,  601,  610,  605,  604,    0,  615,  781,  752,    0,
  368,    0,    0,  634,  729,  728,    0,  472,    0,    0,
  175,    0,  179,    0,    0,    0,  412,  368,  368,  270,
  371,  263,    0,  202,  370,  204,    0,  371,  199,  205,
    0,    0,    0,    0,  425,  426,  370,  368,  371,    0,
    0,    0,  397,    0,  187,  188,  164,    0,  174,  523,
  524,  920,    0,    0,    0,    0,    0,  550,    0,    0,
  370,  371,    0,  500,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  189,    0,    0,
    0,   83,    0,    0,    0,    0,   91,   85,    0,    0,
   90,   86,    0,  746,  987,    0,    0,  788,    0,    0,
  794,    0,    0,    0,    0,    0,  831,  830,    0,    0,
    0,  838,  856,  848,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  786,    0,    0,    0,  211,    0,
  928,    0,    0,  943,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  957,    0,    0,    0,
    0,    0,    0,    0,  626,  947,  889,    0,  260,  279,
    0,    0,  360,  274,   66,   58,   59,   65,   60,   61,
    0,    0,   23,    0,    0,    0,   43,    0,    0,    0,
  755,  775,    0,  754,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  688,    0,    0,    0,    0,
    0,    0,    0,  607,  608,  612,    0,  190,    0,    0,
    0,    0,    0,  180,    0,    0,    0,  408,  214,  215,
  216,  217,  218,  219,  892,    0,    0,  264,    0,    0,
  212,    0,    0,    0,    0,  237,    0,  460,  461,  427,
    0,    0,    0,    0,    0,    0,  370,  479,  371,  173,
  983,    0,    0,  518,    0,  517,  553,    0,  545,    0,
    0,    0,  498,    0,  526,    0,    0,  528,  531,  533,
  368,    0,    0,    0,  127,    0,  195,  370,    0,  369,
  105,    0,  125,    0,   94,    0,    0,    0,    0,   88,
   89,    0,  846,  847,  790,    0,  792,    0,    0,    0,
    0,    0,    0,    0,  849,    0,    0,    0,    0,    0,
    0,    0,  839,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  934,  933,    0,
    0,  930,  929,    0,  938,  937,  936,  935,  932,  931,
    0,    0,  963,    0,    0,    0,  962,    0,    0,    0,
    0,  266,    0,    0,  276,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  358,
    0,    0,    0,    0,  285,  290,  284,    0,    0,    0,
  294,  295,  296,  297,  298,  299,  301,  302,  304,  305,
  306,  309,    0,    0,    0,  352,  353,  354,    0,    0,
   57,   63,   55,   56,   24,  760,  759,    0,  757,  765,
  764,  762,  740,    0,    0,    0,  663,    0,    0,    0,
  693,  691,  689,    0,  690,    0,    0,    0,    0,    0,
    0,  369,  733,  734,  635,    0,  185,  181,    0,  371,
  893,  268,  269,  371,    0,    0,    0,  222,    0,    0,
  238,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  476,  520,    0,  521,    0,  142,    0,    0,    0,  146,
  148,  149,  150,  501,  503,  499,  530,    0,    0,  371,
    0,    0,    0,  366,  370,   92,    0,    0,   98,   93,
    0,    0,    0,    0,    0,  857,    0,  858,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  210,  949,  939,  941,
    0,    0,  959,  958,    0,  964,  952,  954,    0,    0,
  275,    0,  313,    0,    0,    0,  308,    0,    0,  307,
  338,  339,    0,  995,  355,  356,    0,  293,  300,    0,
  303,  286,    0,  291,    0,  343,  292,  310,    0,    0,
  336,   41,    0,    0,  683,  681,    0,  674,    0,    0,
    0,  697,    0,    0,  631,  371,  367,  735,  182,  371,
  152,  257,    0,    0,  198,  223,    0,    0,    0,    0,
  224,  227,    0,    0,  371,  395,  396,  392,  391,  394,
  393,    0,  519,  546,  151,  139,  147,    0,  370,  254,
  134,  371,  372,    0,    0,   96,    0,   80,    0,  817,
  825,  625,  818,  826,  821,  829,    0,    0,  819,  827,
  820,  828,  814,  822,  815,  823,  816,  824,    0,  944,
  960,    0,    0,  282,  278,    0,  311,    0,    0,    0,
    0,    0,  340,    0,    0,  331,  332,    0,  351,    0,
    0,    0,    0,  346,  344,    0,    0,    0,  756,    0,
  665,  675,    0,    0,  676,  677,  678,    0,  707,  709,
    0,  704,    0,  255,  409,  201,  206,  225,    0,    0,
  228,    0,  240,  242,  399,  371,    0,  374,    0,   77,
   99,  100,  953,  281,    0,  312,    0,    0,  315,    0,
  321,    0,  323,    0,    0,  350,  349,  991,  348,    0,
    0,    0,    0,    0,    0,  664,    0,    0,  698,  708,
    0,  229,    0,    0,    0,    0,    0,  256,  371,    0,
    0,    0,    0,    0,    0,    0,  334,  333,  345,  347,
    0,    0,    0,  669,    0,    0,    0,  715,    0,    0,
    0,    0,    0,    0,  239,    0,  246,    0,  245,  375,
  126,  283,    0,    0,  314,  318,    0,    0,    0,    0,
    0,    0,  667,  680,  717,  706,    0,    0,  718,    0,
  714,  226,    0,    0,  250,  249,  248,    0,    0,    0,
    0,    0,  335,  670,  713,    0,    0,  253,  251,  252,
  329,  319,    0,  341,  316,    0,    0,    0,  720,  317,
    0,  719,    0,  722,    0,  723,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1421,   28,   29, 1422,
   31,   32,   33,   34,   35,   36,   37, 1423,   39, 1424,
  844,  128,  488,  983, 1223,  129,  738,  984,  130,  492,
  743,  489,  131,  132,  275, 1007,  144,  985,  986,  213,
  242,  133,  987,  134,  494,   85,  253,  397,  445,  657,
 1115,  446,  883,  628,  235,  860,  658,  659,  660, 1122,
 1117, 1540,  254,  662,  663,   77,  203,   78,  249, 1866,
 1331,  234,  239,  241,  243,  229,  221,  223,  236,  246,
  161,  328,  330,  332,  386,  644,  646,  485,  276,  807,
  565,  673, 1047,  390,  649, 1106, 1325,  596,  765, 1077,
  557,  558,  578,  382,  629, 1092, 1518, 1635, 1268, 1519,
 1520, 1521, 1522, 1523,  674,  230,  222,  378,  604,  379,
  380,  605,  606, 1300,  559, 1041, 1042, 1043, 1263,  392,
  871,  872,  873,  874,  809, 1058, 1496,  630,  811,  448,
  876,  956,  479, 1381, 1269,  449,  450,  451,  452,  453,
 1497, 1498,  581, 1641, 1785, 1286,  817,  818,  804,  819,
  820,  821, 1786, 1787, 1846,  360,  484,  973, 1425,  209,
  574,  970,  575, 1051,  480,  974,  975, 1211, 1404, 1405,
 1406, 1695, 1124, 1426, 1427, 1428, 1429, 1605, 1430, 1431,
 1432, 1433, 1434, 1435, 1436, 1437, 1438, 1439, 1440, 1441,
 1442, 1443,  454, 1793, 1444, 1705, 1830, 1702,  455,  714,
  456,  457, 1445, 1606, 1713, 1714, 1767, 1446, 1447, 1448,
 1597,  272,  481,  482,  458,   41, 1330, 1534,  369,  592,
  835, 1298,  593,  833,   82,   62,  364,  214,  171,  347,
  200,  354,  206,  798,  795,  355,  365,  825,  366,  588,
 1069,  201,  459,  348,  594, 1299,   99,  212,  258,  631,
  632,  727, 1093,  728, 1094, 1095,  635,  636,  637,  638,
 1457, 1086, 1084,  639,  640,  641,  853,  642,  850,   42,
   43,   44,   45,   46,   47,  163,  545,   64,  510,  217,
  371,  315,  316,  317,  318,  319,  320,  321,  322,  323,
  771,  531, 1026, 1256,  532,  533,  776,  167,  336,  547,
   48,  170,  342, 1354,  713,   49,  340,   50,  148,  303,
  518,  761,  401, 1253, 1480, 1625,  513,  305, 1260, 1628,
  149,  150,  304,  519,  520,  759, 1005, 1008, 1009, 1010,
 1011, 1012, 1013, 1720, 1776,  460, 1805, 1014, 1015, 1016,
 1251, 1725, 1726, 1727, 1243,  157, 1472, 1621, 1622, 1808,
 1733, 1809, 1810, 1811, 1841, 1867, 1869,  550,  551, 1035,
 1485,  500,   51,   52,  402,   53,   54,  539,  540,  992,
 1231, 1458, 1462,  501,  746,  227,  675,  676,  898,  899,
  677,  678,  679,  680,  681,  682,  683,  684,  685,  686,
  687, 1140, 1142, 1143,  461,  462,  463,  464,  465,  466,
  467,  251,  468,  469,  470,  471,  472,  939,  473, 1572,
  474,  475,  476,  477,  703,  943, 1200, 1195,  704,  136,
};
static short yysindex[] = {                                   5493,
 1468, -120, 3172,   74,  862,   64, 3962, -170, 8330,  386,
  125, 8807, -120, 1118,  396, 4840,  950, 4840,  244,    0,
  700,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  453,    0,    0,    0,    0,    0,    0,  275,    0,    0,
    0,    0,    0,    0,    0,14182, 4840, 4840, 4840, 4840,
 4840,    0,    0,  408, 1266, 4840, 4840, 4840,  577, 4840,
  622, 4840, 4840,    0, 4840, 4840,    0, 1084,    0,  642,
    0,    0,  836, 4840,    0, 4840, 4840, 4840,  871, 4840,
 4840, 4840, 4840,  622, 4840, 4840, 4840, 4840,    0, 4840,
 1069,  812,    0,    0,    0, 1268,    0, 1268,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  940,  964,   79,
 1194,    0,    0,    0,    0,    0, 4840, 4840, 4840,  975,
 1003, 1014,  155, 1259,  832,   75,  184,   42,  910,    0,
 4840, 1072, 1199,    0,    0, 4840, 1077, 4840, 4840, 4840,
    0,    0,    0, 4840,    0,  979, 1046, 1237,  478,  987,
 1412,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  649,
    0,    0, 1266,    0,   78,    0,    0, 1477,    0,    0,
 1283,    0,  473,    0,    0, 1132,    0,    0, 1113,    0,
    0, 1187,    0, 1314,    0,    0,    0, 1266,    0,    0,
    0,  690, 1477,    0,  531,    0,    0, 1271,    0, 1304,
    0, 1268,    0,    0,    0,    0,    0,    0, 1294,    0,
    0, 1209,    0, 1307,    0, 1236,    0,    0,    0, 1209,
    0,    0,    0,    0,    0,    0,    0,    0,    0,13212,
 4840, 1241,    0, 4840,    0,    0,    0, 4362, 1308, 5123,
 4840,  890, 1342, 1667, 1395, 1414, 4362, 1336, 5123, 1360,
 1361, 1244,    0,13319,    0,    0,    0,    0,    0,    0,
    0,    0,11567,    0,    0,    0, 1322, 4840, 1402, 1326,
  729, 4840, 1334,    0,    0, 1199,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 4840, 1477,    0, 1271,    0,
 1268,    0, 1401, 1405, 1287,    0,13319, 1379,    0, 1379,
13319,    0,  265, 1286, 1290, 1305, 1412,    0, 1534,  794,
    0, 4840,    0,  649,    0,    0,    0,    0, 4840, 1241,
 4281, 1369, 4540, 1793,    0,14182,    0,    0,  771,    0,
    0,    0, 1774, 1774, 1266, 1486, 1266,  427, 1187,    0,
    0, 9255,    0, 4840, 4840,    0, 1241,    0,    0, 1558,
  463, 1813, 4840, 1595, 4840,    0,    0, 1825, 2862, 8683,
    0,    0,    0, 1847, 1851, 1865, 1417,    0,    0, 1884,
    0, 1886,    0, 1895, 1896,    0, 1900,    0,    0,    0,
    0,    0, 1902, 1429, 1484,    0,    0,    0,    0,    0,
    0,    0, 1904,    0,    0,11825, 1905, 1913,    0, 1915,
13319,13319,12044, 4840, 1921,    0, 3001,    0,    0,    0,
    0,    0,    0,    0,    0, 1922, 1874,    0,    0, 1927,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 9255, 1918,
 1550,    0, 1928, 1710, 1582,    0, 4840, 1561,    0, 4840,
 4840,   89,    0,    0,    0,    0,    0, 4840, 4840, 1025,
    0, 4840,    0, 1665, 4840, 1948, 4362, 1599,    0, 4840,
12044,  973,    0,    0,13319,    0, 1427, 1678, 1952,    0,
    0,   53,    0,  -93,    0,  591,    0,  591,    0,    0,
 1955,    0,  149, 1583,    0,    0,    0, 1958,  116, 1944,
 1241, 1966, 1743,    0,    0, 4840,    0,    0, 1720, 1964,
    0,    0,    0,  629,    0, 1666, 1670,    0, 1266,    0,
 1671,    0,    0, 1735, 1740,    0,    0,    0, 1721,    0,
 9255,    0, 9255, 1604,    0, 1753, 4840,14016,    0,    0,
  643,    0, 4840,  205, 1283, 1630, 1593, 1562,    0,    0,
    0,    0,  -21, 1635,    0, 1576,  622,    0, 1266,    0,
 1754, 1684,    0,    0,  427,    0,    0,  252, 1600,    0,
    0,    0, 1606,    0, 1651, 2013,    0, 1712, 2015,   26,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1965,  215,    0,    0,    0,    0,  180, 2018,
 2021, 2015, 1477,    0, 1747,    0, 1799,    0, 1266, 1266,
    0, 4281,  473,    0, 1688,    0, 1669, 2025,    0, 5860,
    0, 1692,    0,    0, 1695, 2035, 8915, 2036,    0,    0,
    0, 8438, 5405,   98,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  878,13319,  391,
    0, 4840,13319, 1143, 1197, 1292, 2742,    0,    0,13319,
 9160, 3001, 1639,  317,13319,13319, 9160,  645,  645, 1508,
 1609,  275, 2043,    0,    0,12151, 4840,13319,13319,13319,
13319,13319,13319,13319, 2048,    0,    0, 1811, 2026, 4840,
    0, 4840, 1796, 4840,    0,    0,11451, 1675,    0,    0,
 1638, 5123,    0, 1784,    0, 1631, 2059,    0, 1784,11451,
 1794, 1801, 4840,    0, 1508,    0, 1622,    0, 4840, 2440,
 1672,13319,    0, 1108,    0,    0,  440,    0, 1679,    0,
    0,    0, 4840,  -91, 4840,    0,    0, 4840, 4840,    0,
    0,13319, 1846, 4281,    0, 2085, 1636,13319,13319, 1266,
    0,  -88,    0,    0,    0, 2091,    0,    0,    0,  179,
 4840,    0,    0,    0,    0, 1859, 2094,    0, 1838, 1857,
    0,    0, 1800, 1804, 1268, 1873,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  622, 4840,14124, 1772,
 1773, 1777,    0,  622,    0, 1340,    0,    0, 1748, 1266,
    0, 2862,   70,    0, 1755,    0,    0,    0,  622,    0,
 2013,   77,    0,    0,    0, 1870,   77, 1758,   77,    0,
 2130,  622,  622,  622,    0, 1241, 4840,    0, 1819,    0,
 1664,    0,    0,    0,  771, 1379, 4840, 1700,  112,    0,
    0, 2132,   37,    0, 2862, 4840, 1379, 9392,    0,    0,
 1379, 8915,    0,    0,    0, 5116,    0,  370,  448,13319,
13319, 9499, 2135,  615, 9757,13319,10015, 1090,10122,10380,
12370,10638,10745,11003, 9160, 9160,13319,13319, 1643,13319,
 2136,13319, 2138, 1791,13319,13319,13319,13319,13319,13319,
    0,    0,    0,    0,    0,    0,    0,    0, 1863, 1107,
  518,13319,  808,13319,    0, 9160, 2491, 2578,  212,    0,
    0,  987,    0,    0, 1711,    0,    0, 1908,  645,  645,
 1189, 1189, 3001,  828,    0,    0,    0,    0,    0, 1738,
    0, 1741, 1917, 1796,    0,    0, 4840, 4840, 4840, 4840,
 4840, 4840,  101, 2144,    0,    0,    0,11451, 1818,    0,
  -30, 1837, 4840,    0,13319, 1837, 2147, 2144,11451, 4881,
 1880,    0,    0,    0,    0, 1724, 2160, 2158, 1542,    0,
    0,    0,    0,    0,    0,    0, 9160, 1878,    0,    0,
    0,    0,    0,    0,    0,  -97,    0,    0,    0, 3001,
    0, 1751, 1379,    0,    0,    0, 3001,    0, 1774, 1943,
    0,  -88,    0, 2170, 9160, 4012,    0,    0,    0,    0,
    0,    0, 1796,    0,    0,    0, 4012,    0,    0,    0,
 1268, 1268, 1718, 2172,    0,    0,    0,    0,    0,  -18,
  413,  405,    0, 1949,    0,    0,    0, 4840,    0,    0,
    0,    0,   74, 1752, 1806, 1811, 2176,    0, 1576, 1759,
    0,    0, 1010,    0, 2165, 4840, 1353,  622, 2188, 2189,
 2190, 1970,  473, 1796, 1774, 1670, 4281,    0, 1635, 1789,
 2174,    0,  622, 1850, 1935, 5860,    0,    0,  622, -205,
    0,    0, 1937,    0,    0, 2205, 2206,    0, 9160, 9160,
    0, 9160, 9160, 1907, 3001, 2209,    0,    0, 3001, 2210,
12044,    0,    0,    0, 1903, 2216, 3001, 2218, 1441, 2219,
 3001, 2222,13319,13319, 2135,13319,12628, 2223, 3001, 2224,
 2226, 3001, 2228,13319, 3001, 2231, 3001, 2238, 2240, 3001,
 2242, 2243, 3001, 2244,    0, 2024, 1803, 1999,    0, 2100,
    0, 2112,13319,    0, 2119, 2361, 2382, 2402, 2416, 2423,
13319,13319,13319, 3001, 1860,13319,    0,13319, 3001, 1983,
  595,13319,13319,13319,    0,    0,    0, 4840,    0,    0,
 4840,10255,    0,    0,    0,    0,    0,    0,    0,    0,
 1967,11451,    0,11451,  101, 2144,    0,   71,   71,  123,
    0,    0, 1776,    0, 2147, 2144, 2248, 4881, 2252, 1542,
 2258,13319, 2037, 2440, 1929,    0, 1929, 1013, 1929, 1963,
 1974, 2030, 1989,    0,    0,    0, 1796,    0, 1789,  838,
 1576, 1934, 1576,    0, 9160,  376, 1297,    0,    0,    0,
    0,    0,    0,    0,    0, 4012, 4012,    0, 1917,13319,
    0, 2038, 1924, 1718, 1084,    0, 9160,    0,    0,    0,
 1941, 1946, 1953, 1954, 1959, 1960,    0,    0,    0,    0,
    0, 2287,   74,    0, 1881,    0,    0,   77,    0, 4012,
 1962,   77,    0,   77,    0, 1811, 2296,    0,    0,    0,
    0,  771, 1917, 1576,    0, 1670,    0,    0, 2066,    0,
    0, 2085,    0, 2303,    0, 2305, 1266,  -78, 2312,    0,
    0, 1266,    0,    0,    0, 2024,    0, 2024,13319, 1379,
 1379, 1785,  275, 2314,    0, 1379, 1379,13319, 1379, 1379,
 2831, 3001,    0, 1685,13319, 3001, 1379, 1379, 1379, 1379,
 3001, 1379, 1379, 1379, 1379, 1379, 1379,    0,    0, 9255,
 2316,    0,    0, 2469,    0,    0,    0,    0,    0,    0,
 2479, 1407,    0,13319, 2062, 1947,    0,13319, 2534, 1817,
 2682,    0, 9255, 2309,    0,    0, 2081,    0, 3169, -107,
 2317, 2333, 2039,13319, 2321, 2324, 3462, 4840, 4840,    0,
 2331, 2341, 2069, 2348,    0,    0,    0, 2104, 9632, 2351,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2353,  -77, 2356,    0,    0,    0,    0, 2041,
    0,    0,    0,    0,    0,    0,    0, 1993,    0,    0,
    0,    0,    0, 4881, 2248, 2037,    0, 1830, 4840, 1542,
    0,    0,    0, 2087,    0, 2440, 2440,13319, 9160, 2046,
 1917,    0,    0,    0,    0, 1340,    0,    0,  379,    0,
    0,    0,    0,    0, 3001, 2151, 2038,    0, 1000, 4840,
    0, 2097,  397, 1998, 2105, 2004, 2142, 2107, 2148, 1796,
    0,    0, 2407,    0, 2409,    0, 2103, 2151, 1962,    0,
    0,    0,    0,    0,    0,    0,    0, 1796, 1635,    0,
 1340, 1949, 2074,    0,    0,    0, 2088, -114,    0,    0,
 1811, 2089, 3001, 2415, 2419,    0,  987,    0, 2421, 2426,
 3001, 2428, 2432,13319,13319, 3001, 2435, 2438, 2445, 2447,
 2448, 2454, 2455, 2456, 2460, 2461,    0,    0,    0,    0,
13319, 2467,    0,    0,13319,    0,    0,    0,13319,  292,
    0, 2194,    0,12735, 4840,13319,    0, 9160, 3001,    0,
    0,    0, 2458,    0,    0,    0, 4840,    0,    0, 2569,
    0,    0,  770,    0, 2070,    0,    0,    0,  337, 2475,
    0,    0,   71, 4840,    0,    0, 2440,    0, 1760, 3001,
 2474,    0,   98,  801,    0,    0,    0,    0,    0,    0,
    0,    0, 2478, 4840,    0,    0, 2180, 2191, 4840, 1718,
    0,    0, 1268, 4840,    0,    0,    0,    0,    0,    0,
    0, 1917,    0,    0,    0,    0,    0, 1917,    0,    0,
    0,    0,    0,13319, 1266,    0, 2079,    0, 1266,    0,
    0,    0,    0,    0,    0,    0, 3001, 3001,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3001,    0,
    0, 2772, 4012,    0,    0, 2501,    0, 2956,13212, 1380,
  398, 2487,    0, 2211, 1121,    0,    0, 2508,    0, 4840,
 4840,   74,  214,    0,    0, 2055, 2225, 9160,    0, 2518,
    0,    0, 9160, 2519,    0,    0,    0,13319,    0,    0,
 2203,    0, 2538,    0,    0,    0,    0,    0, 1718, 1268,
    0, 2198,    0,    0,    0,    0, 1949,    0, 3001,    0,
    0,    0,    0,    0, 1379,    0, 2186, 2569,    0, 2155,
    0, 2569,    0, 2569, 2481,    0,    0,    0,    0,10255,
  770,13319, 2569,  403, 4840,    0,   98, 4840,    0,    0,
 4240,    0, 2198,  464, 1718, 2215, 2221,    0,    0, 2554,
 2557, 2239, 2540, 1127,10255, 2558,    0,    0,    0,    0,
 1092,  409, 2313,    0, 1834, 1836, 4840,    0, 2565, 2571,
 1200, 1718,  434,  434,    0, 2315,    0, 2183,    0,    0,
    0,    0, 2569, 2558,    0,    0, 2320, 2569, 2338, 2328,
10255, 4840,    0,    0,    0,    0, 4240, 2590,    0, 4840,
    0,    0, 2177,  617,    0,    0,    0, 1842,10255,  312,
 4840,10255,    0,    0,    0, 4840, 2298,    0,    0,    0,
    0,    0,10255,    0,    0, 2591, 2595, 2597,    0,    0,
 4840,    0, 4840,    0, 2598,    0,
};
static short yyrindex[] = {                                      0,
    0,  200,  605,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  153,11241,    0,    0,    0,    0,    0,    0,
 2640,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  290,    0,    0,    0,    0,    0,    0,  139,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  656,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2318,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   19, 2266,    0,    0,    0,  152,    0,  152,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2270,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  689,    0,    0,12954,11348,    0,
    0,    0,  743,    0,    0,  173,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  429,    0, 2241,  208,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   31,    0,    0,
    0,    0,  892,    0,    0,  884,    0,    0,    0,    0,
    0,   62,    0, 9753,    0,    0,    0,    0,    0,    0,
    0,    0,   31,    0,   58,    0,    0, 2607,    0,    0,
    0, 2388,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1005,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1082,
    0, 2389,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1192,    0,    0,    0,    0,    0,    0,
    0, 5170,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  508,    0,    0, 1083,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   31,    0, 2607,    0,
 2388,    0, 1133,  348,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1153,    0, 2381,    0,
    0,    0,    0, 1174,    0,    0,    0,    0,    0, 2389,
 1049,14248,    0, 1182,    0,    0,    0,    0,  176,    0,
    0,    0, 2212, 2212,    0,    0,    0, 1211,  169,    0,
    0,    0,    0,    0,    0,    0, 2389,    0,    0, 1358,
    0,    0,    0,    0,    0,    0,    0, 5571,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1990,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2825,    0,    0,    0, 3125, 3425,    0, 3725,
    0,    0,    0,    0,    0,    0,  888,    0,    0,    0,
    0,    0,    0,    0,    0,  185, 4025,    0, 2525,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1228,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1247,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2740,    0, 2343,    0,
    0,    0,    0,    0,    0,  894,    0,  894,    0,    0,
  564,    0,  646, 1035,    0,    0,    0,    0, 1005,    0,
 2389,    0,    0,    0,    0,    0,    0,    0,  148,  234,
    0,    0,    0,  364,    0,    0,  734,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2381,    0,    0, 1370,    0, 2199,    0,    0,  484,    0,
    0,    0,    0,  402,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1252,    0,    0, 3116, 4808,    0,
    0,    0,10908,    0,    0,13838,    0,    0,13874,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1694, 1897,    0,    0,    0,    0,13917,13785,
    0,13874,   31,    0,    0,    0,    0,    0,    0,    0,
    0, 1049, 1099,    0,    0,    0,    0, 2260,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 6331, 6580,
 6726,    0,    0,  821,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,13538,    0,13538,
    0,    0,    0,13538,13538,13538,    0,    0,    0,    0,
    0, 2229,    0,    0,    0,    0,    0, 5214, 5503, 2623,
    0,   65,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1518,    0,    0,
    0,    0, 2410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  270,    0,    0,    0,    0,  270,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  889,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  560,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1049,    0,  378,  182,    0,    0,    0,
    0,  107,    0,    0,    0,    0,    0,    0,    0, 1848,
    0,    0,    0,    0,    0, 2641,    0,    0, 4131, 2716,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  371,
  445,  840,    0,    0,    0,  737,    0,    0,    0,    0,
    0,    0,    0,    0, 3416,    0,    0,    0,    0,    0,
13838,    0,    0,    0,    0,  450,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2389,    0,    0,  255,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 8296,    0,
    0,    0,  147,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2644,    0,    0,    0,    0,    0, 4325, 5792, 6081,
 4625, 4925,  896,    0,    0,    0,    0,    0,    0,    0,
    0, 5990,    0, 2417,    0,    0,    0,    0,    0,    0,
    0,    0, 1263, 1300,    0,    0,    0,    0,    0,    0,
    0, 1318,    0,    0,    0, 1318, 1356, 1371,    0,    0,
    0,    0,    0,    0,    0,    0, 1012,  250,  204,    0,
    0,    0,    0,    0,    0,    0,    0, 1027,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  135,
    0,    0,    0,    0,    0,    0,  383,    0, 2212, 2236,
    0,  556,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2410,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1198,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1378,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 3716,    0,    0,    0,    0,
    0,    0,    0,    0, 1397,    0,    0,    0,    0,    0,
    0,    0, 1099, 2410, 2212, 1394, 1049,    0, 2430, 1426,
    0,    0,    0, 8296,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  790,    0,    0,    0,  869,    0,
    0,    0,    0,    0,    0,    0, 1554,    0, 2265,    0,
 6932,    0,    0,    0,    0,    0,    0,    0, 7108,    0,
    0, 7205,    0,    0, 7284,    0, 7414,    0,    0, 7442,
    0,    0, 7517,    0,    0, 1633,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2268,    0,    0,    0,    0,  823,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  568,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1263, 1436,    0, 2271,  296,    0,
    0,    0,    0,    0, 1440, 1463, 1475,    0,    0, 2357,
 1471,    0, 6902,    0, 2362,    0, 2362, 2357, 2362,    0,
    0,    0,  156,    0,    0,    0, 2410,    0, 1426,    0,
    0, 2253,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2201,    0, 1198, 2318,    0,    0,    0,    0,    0,
  919,  967, 1021, 1041, 1048, 1055,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  209,    0,    0,    0,    0, 3864,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1501,    0,    0,    0,    0,
    0, 1523,    0,    0,    0,    0,    0,  146,    0,    0,
    0,    0,    0,    0,    0,  162,    0,  216,    0,    0,
    0,    0,   65,    0,    0,    0,    0,    0,    0,    0,
    0, 7633,    0, 7661,    0, 7784,    0,    0,    0,    0,
 7867,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2661,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8778,    0,    0,
    0,    0,    0,    0,    0, 1571, 1588,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  746,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1637,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1524, 6698,    0,    0,    0,  882,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  713,
    0,    0,    0,    0,    0,  590,    0,    0,    0,    0,
    0,    0,    0,    0, 2663, 1502, 2372,    0, 2381,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2410,
    0,    0,    0,    0,    0,    0,    0, 1531,  842,    0,
    0,    0,    0,    0,    0,    0,    0, 2410, 2430,    0,
  266,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  837,    0, 7939,    0,    0,    0, 2644,    0,    0,    0,
 8011,    0,    0,    0,    0, 8083,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2647,
    0,    0,    0,    0,    0,    0,    0,    0, 2648,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2403,    0,    0,    0,    0,    0,
    0,    0,  296, 6501,    0,    0,    0,    0, 2357,  966,
 1220,    0, 1227, 2670,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 4164,
    0,    0,  170,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  161,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 8143, 8227,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2671,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  103,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1868,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2673,    0,    0,    0,    0,    0,    0,    0, 4164,  988,
    0,  177,    0,    0,    0,    0,    0,    0, 2663,    0,
    0,    0,    0,    0,    0,    0, 2655,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -76,
    0,    0,    0,    0,    0,    0, 8290,    0,    0,    0,
 2670,    0, 1288,    0, 1198, 1530, 1655,    0,    0,    0,
    0,    0,    0,    0,  -76, 2656,    0,    0,    0,    0,
    0, 2418,    0,    0,    0,    0, 1234,    0,    0, 2676,
    0, 4164,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2659,    0,    0,13946,    0,    0,    0,
  -76,    0,    0,    0,    0,    0, 2670,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -76,    0,
    0,  -76,    0,    0,    0,    0, 1862,    0,    0,    0,
    0,    0,  -76,    0,    0, 2681,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2728,    0,    0, 2732,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2735,
    1,  719,  160, -585, 1512,  720, -304, -519, 2726, 2463,
    0,    8,    0, 2477,  -98,  -47,    0,   32, -948,  -40,
  -61,  -38, -179, 2464, 2020,    0,    0,    0,  673,    0,
    0,  -45, 1647, -590,  -75, -982, 2108, 1887, -328,    0,
 1658,    0, 2679, 1891, 1938,    0,  -59,    0, -512,  -57,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -535,   13, 1674,
 -255, 1195, -933,    0,    0,    0,    0, -335, -131, -577,
 1677, -351,  -49,    0, -548,    0,    0, 1270,-1016,    0,
 1258,    0,    0, 2437, -673,  -48,    0,    0,    0,    0,
 2412,    0, 2187, 1697,    0,    0,    0, 1756,    0, 2465,
 2011, 1690, 2439, 2442,    0, 1219,    0, -447, 1991,  -44,
 -622, 1170, -219,    0,    0, -992, -937, -914, -889, -885,
    0, 1309, -658,    0, 1026,-1167, 1313,    0, 2235,    0,
    0,    0, 1033, 1022, 1007, -143, -260, -899, -861,    0,
    0,    0, 2022,    0, -660,    0, 1853,    0,    0,    0,
    0,    0,-1166, -511,    0,    0,    0,    0, -221,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1123,    0,    0,  512,    0,    0,  -19,    0,
 -388,  592, 1411, 1226,    0, 1061,    0,    0,    0,    0,
    0,    0,    0, 2106, -133, -408, 1575, 1355, -551, -767,
 -996, -892,    0,    0,    3,    0,    0,    0,    0,    0,
 2472,    0,    0,    0,    0, 2485, 2250,    0,    0,    0,
    0,    0,   -9, 2494,    0,    0,    0,  -10,    0,    0,
    0, -361, 1985, -356, 1533, -491,    0,    0,    0,    0,
 2245,    0, -906,    0,    0,    0, 2207,    0, 1996,    0,
    0,    0,    0,    0,    0,    7,    0, 2835,    0,    0,
    0,    0,    0, 2535,    0,    0,    0,    0, 2334,    0,
 2329,    0,    0,    0, 2090,    0,    0,    0,    0,    0,
 -374, -672, -910, 1925,    0,  935,    0,    0,    0,    0,
    0,    0, -329,    0,    0,    0, -224,  875,    0,    0,
    0, 2734, 1161,    0, 2113,-1030, -872,    0, -865, 1871,
-1169,    0,    0,    0,    0, -152,    0,    0,    0,    0,
 1617,    0,    0,    0,    0, 2795,  229,    0, 1160, 1265,
    0, 1053,    0, 1089,    0, -217,    0,    0, 2114,    0,
    0,    0,    0,    0, 2352,    0,    0,    0, -455, 2154,
 1910,-1010,    0,    0,    0, -707, -451,  178,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1698, 1745,    0,    0,    0, -882,    0,    0,    0,
 1641,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -907, 1713,    0,    0,
};
#define YYTABLESIZE 14765
static short yytable[] = {                                     135,
  205,   38,  135,  325,   81,  156,  155,  228,  162,  277,
  786,  208,  297,  300,  296,  299,  224,  967,  265,  220,
  633,  225,  233,  154,  571,  634,  603,  941,  259,  808,
  766,  726,  240,  949,  263,  268,  787,  245,  597,  952,
 1281, 1205,  262,  745, 1407,  264,  202,  204,  207,  210,
  155,  215,  270, 1270,  711,  267,  226,  204,  231,  269,
  210,  159,  155,  215, 1270,  244,  135,  211,  712,  884,
 1028,  971,  514,  237,  255,  156,  155,  257,  204,  238,
  255,  231,  207,  210,  266,  155,  215,  244,  135,  387,
  210,  748,  283,  154,  285,  569,  237,  764,  331,  576,
  493,  875,  307, 1306,  537,  726,  177, 1109, 1271,  493,
  327, 1213, 1328, 1083,  294,   80, 1501, 1278,   80, 1271,
  159, 1089,  281,  799, 1282,  800,  647,  226,  155,  226,
  810, 1272,  742,  295,  753, 1290,  712,  295,  726,  573,
 1240,  231, 1272,  394, 1222,  284,  324,  732,  210,  155,
  244, 1114,  574, 1279,  162,  701, 1273,  356, 1311,  779,
 1274,  327, 1033, 1275,  997,  177,  329, 1273,  160,  767,
  333, 1274,  687,  538, 1275,  478,  247, 1050,  753,  726,
 1053,  736,  383,  541,  325,  295,   95,   87,  732, 1060,
   87,  732,  775,  753, 1538,  767,  701,  726,  281,  574,
  751,   97,  789,  657, 1323,  791,  732,  739,  144,  780,
  767,  574, 1469,  767,  701,  889,  633,  247, 1459,  862,
  247,  634,  736,  298,  100,  736,  478,  160,  767,  486,
  998,  687,  543,  725,  478,  247, 1666, 1609,  337, 1049,
  736, 1175, 1176,  325,  657, 1104,  295,  657,  739,  656,
  523, 1595, 1596,  603,  133, 1204,  791, 1771,  574, 1492,
 1493,  226,  657, 1039,  226,  186,  739,  144,  155,  758,
  495,  135, 1201, 1452,  725, 1454,  830,  155, 1340,  495,
  783,  536,  362, 1270, 1270, 1393, 1228, 1585, 1395,  619,
  656,  843,  725, 1516,  259,  525,   80,  712,  207, 1254,
  854,  301,  155,  767, 1024,  859, 1618,  570,  656, 1120,
 1341,  740,  831,  133,  478, 1291,  225, 1270,  584,  530,
 1025,  586,  313, 1461,  186,  816,   63,  955,  758,  598,
  619,  600, 1040, 1526,  357,  964,  643,  655, 1271, 1271,
  130,  302,  226,  101,  645,  651,  538,  621,  619,  226,
 1255,  226, 1693,  226,  525, 1764,  589, 1481,  159,  915,
 1090, 1272, 1272,  737,  177, 1511,  855, 1610,  337, 1667,
  384, 1266, 1271,  856,  210,  215,  177,  620, 1470, 1539,
   86, 1067,  738,  155, 1121,  207, 1273, 1273,  621,  664,
 1274, 1274, 1586, 1275, 1275, 1272,  139, 1229, 1514,  661,
  653,  398, 1225,  177,  737,  159,  621,  358, 1257,  177,
 1131,  177,  832, 1235, 1237, 1292, 1490, 1494,  620, 1630,
 1273,  362,  737,  738, 1274, 1276, 1277, 1275,  623,  384,
 1704, 1018,  921,  259,  715, 1614,  620, 1645, 1760,  478,
 1128,  738,  739, 1803,  382, 1289,  504, 1722, 1001,  143,
  177,   95, 1764,  726,  536, 1345, 1346,  739, 1347, 1348,
  398, 1530,  732,  774,  836,  160,   97, 1110, 1226,  623,
  701,  289, 1741,  915,  726, 1048,  916,  155, 1126, 1236,
  210,  495, 1127,  732,  370,  144, 1573,  623,  155,  858,
 1576,  701,  155,  842,  736,  210,  736,  155, 1113,  866,
  162,  486,  293,  382,  327,  744,  280,  597,  143,  749,
  130,  130,  160,  792, 1770, 1452,  754,  736,  657, 1082,
   79,  837,  739,   79,  293,  884,  293,  767,  657,  763,
  657, 1002, 1659,  812,  741,  701,  226, 1232, 1044,  657,
  789, 1046,  692,  739,  701,  767, 1221, 1402,  725,   87,
 1116, 1322, 1056,  838, 1529,  178, 1631, 1029, 1668,  400,
 1632, 1633,  493,  603,  656,  478,  597,  805,  226,  725,
  159, 1782, 1082,  805,  656, 1332,  656,  726,  753, 1074,
  574,  573,  687,  657,  726,  656,  325,  701, 1057,  186,
  916, 1489,  657,  732,  791,  968, 1660, 1239,  133,  237,
  574,  701, 1719,  869,  870, 1102, 1233,  225,  555,  186,
 1652, 1619, 1863, 1503,  178,  177,  687, 1815,  861, 1626,
  944,  945,  603, 1499, 1259,  619,  538,  736, 1658,  656,
  758, 1129, 1021,  293,  384,  657, 1672,  915,  656, 1662,
  915,  732,  226,  923, 1842,  613, 1716,  657,  186,  657,
  301,  726, 1465,  739,  168,  576,  525,  137,  915,  915,
  732,  823,  767,  824,  915,  398,  752, 1691,  701,  247,
  721,  719, 1829,  720,  842,  722, 1754,  160,  737,  725,
 1547,  656,  231,  621, 1864,  478,  721,  478,  572,  976,
  920,  722, 1734,  656,  736,  656, 1735,  738, 1528,  737,
 1270,   79, 1295, 1261,  613,  739,  169,  957,  382, 1132,
 1293, 1744,  705,  620,  576,  226,  657,  151,  738,  526,
  226,  138,  226,  601,  226,  143,  143,  495, 1748,  367,
  141,  142,  495,  471,  536,  619,  186,  338, 1296,  225,
  495,  164,  581,  210, 1038,   14,  725,  572, 1130, 1004,
  210, 1065, 1352,  705,  916, 1271,  478,  916,  165,  946,
  384, 1813,  656,  155,  623,  155, 1353,  166,  226, 1324,
  555,  705, 1502,  556,  226,  916,  916,  339, 1272,  915,
  530,  916, 1027, 1066, 1068, 1116, 1099, 1100, 1101,  840,
 1745,  226,  471,  621, 1079,  186, 1746, 1215,  388, 1280,
 1218,  581,  368, 1273, 1843, 1623,  650, 1274,  216,  737,
 1275, 1288, 1788,  178, 1400, 1790, 1515,  152,  226,  226,
  702,  143, 1525,  620,  153,  178, 1133, 1063,  738,  237,
  840, 1567,  664,  840,  382, 1310, 1073,  232, 1499,  383,
  538,  145, 1080, 1844,  890,  597, 1294,  186,  840,  897,
 1380, 1087,  178, 1468, 1789, 1820,  915,  155,  178,  186,
  178,  702,  290,  291,  237,  237,  237,  155,  809,  123,
  528,  723,  289,  985, 1103,  664, 1125,  527,  597,  702,
  527,  658,  337,  578, 1111,  661,  186,  890,  703,  389,
 1845,  378,  186,  602,  186,  891,  916, 1814,  383,  178,
  145,  603, 1804,  123,  597,  717,  349,  597, 1629,  809,
  718,  429,  809, 1859, 1701,  218,  219, 1604,  389, 1219,
 1207,  717,  658,  597,  398,  658,  718,  809,  890,  703,
 1216,  890,  403,  186,  603,  602,  891,  123, 1217,  891,
  658,  186,  578, 1220, 1193,  985,  890,  703,  536,  597,
  378,  350,  602, 1661,  891, 1580,  606, 1021,  813, 1854,
  603,  769, 1144,  603, 1860,  699,  390,  495,  210,  215,
  495,  244,  135,  916,  791, 1353, 1353,  389,  495,  603,
  890, 1353, 1353,  613, 1353, 1353, 1145,  767,  891,  495,
  495,  471, 1353, 1353, 1353, 1353,  210, 1353, 1353, 1353,
 1353, 1353, 1353,  471,  703,  603,  699,  606,  526,  699,
  186,  684,  337,  756,  721,  719,  613,  720,  633,  722,
  386, 1398, 1317,  634,  699,  390,  696,  705,  767,  590,
  770,  767, 1709,  186,  598,  814,  471, 1335,  471,  591,
  385, 1510,  613, 1339, 1774,  613,  767,  388,  705, 1777,
  287,  840,  684, 1312,  387,  684,  789,  967,  351,  538,
  337,  613,  225,  703,  178,  890,  486,  696, 1301,  890,
  684,   79, 1532, 1283, 1284,  815,  352,  567, 1710,  386,
  186,  327,  580, 1302,  384,  696, 1301,  613,  186, 1711,
  568,  250,  705,  598, 1456, 1456, 1460,  226,  186,  385,
  237,  226, 1313,  383,  840,  527,  388,  840,  271,  527,
  353, 1196, 1197,  387,  840,  237,  840,  370,  145,  385,
  840,  237,  186, 1333,  186,  840,  967,  967,  840, 1486,
  809, 1488,  567,  721,  719,  702,  720,  301,  722, 1729,
  327,  580,  527,  840,  705,  702,  186,  702,  721,  719,
  378,  720,  405,  722,  252,  378,  702,  840,  705,  528,
  378, 1403, 1730,  478, 1764,  840,  225,  536,  840,  840,
 1764,  186,  515,  406,   83,   84,  529,  917,  840, 1763,
  521,  402,  389,  809,  527, 1826,  809,  103, 1712,  260,
  337,  567, 1531,  809,  840,  809,  658,  241,  226,  809,
  702, 1408, 1449,  703,  809,  273,  658,  809,  658,  702,
  168,  405,  495,  703,  495,  703,  840,  658, 1731,  695,
  692,  840,  809,  105,  703,  705,  700,   44,  495,  383,
  390,  602,  406,  840,  210,  840,  809,  282,  241,  527,
  402,  241, 1806,  816,  809,  186,   52,  809,  809,  717,
 1198,  169,  702, 1451,  718, 1453,  241,  809, 1799,  292,
  695,  658,   42,  767,  767,  967,  702,  700,  703,  168,
  658,  767,  552,  809,  553,  684,  538,  703,  695, 1664,
  699,  378,  840, 1827,  386,  700,   44,  247,  106,  890,
  602, 1537,  699,  602,  527,  809, 1542,  891,  107,   25,
  809,  699,  840, 1513,  385,   52,  840,  274,  389,  602,
  169,  388,  809,  658,  809,  278, 1616,  763,  387, 1853,
  703,   42, 1483,  108,  232,  658,  684,  658,  247,  684,
 1245,  247, 1484,  702,  703,  602,  684, 1862,  684,  279,
 1865,  696,  684, 1246,  158,  699,  247,  684,  247, 1584,
  684, 1870,  286,  696,  699,   48,  390,  378,   25,  767,
 1153,  809,  696,  767,  232,  684, 1247,  378,  717,  465,
   50, 1637,  598,  718, 1154,  159,  763,  480,  287,  684,
  767,  809,  248,  717,  536,  809,  767,  684,  718,  288,
  160,  684, 1638,  135,  658,  147, 1312,  699, 1639,  204,
  684,  703,  301, 1747,  308,  598,  696, 1594, 1125, 1125,
  386,  699,  334,  465,   48,  696,  684, 1249, 1155, 1449,
  767,  721,  719, 1192,  720,  373,  722, 1828,  465,   50,
  385,  598, 1640,   14,  598,   26,  480,  388, 1759,   49,
  502,  232,  925,  684,  387, 1316, 1156, 1708,  721,  719,
  598,  720,  135,  722,  495,  684,  301,  684,  696,  324,
  747, 1721,   51,  684,  447,  717,  210,  210,  721,  719,
  718,  720,  696,  722,   46, 1473,  598, 1475,  699, 1456,
  767,  232,  721,  719,  373,  720,  326,  722,  512,  502,
  155,  335,  684,  684,   26,  684,  927,  517,   49,  341,
  136,  207,  684,  684,  684,  684,  684, 1643,  684,  337,
 1157,  684,  684,  684,  684,  684,  359,  527,  102,  684,
  684,   51,  124,   47,  684,  684,  684,  684,  684,  243,
  207,  549, 1838,   46,  695,  554,  309, 1699,  373,  696,
  361,  700,  207,  362, 1742,  207,  695,  370,  950,  721,
  719,  301,  720,  810,  722,  695,  103,  293,  527,  136,
  207,  527,  700,  247,  247,  372,  716, 1839,  104,  310,
  243,  247,  374,  243,  375,  210,  527,  381,  527, 1840,
 1706,  124,   47,  391, 1020,  363, 1743, 1125,  243,  207,
  731,  929,  105,  730,  810,  311,  376,  810,  312,  695,
  772,  716,  146,  147,  324, 1708,  700,  210,  695, 1708,
 1717, 1708,  810,  716,  313,  700, 1007,  538,  393, 1750,
 1708,  377,  735, 1752,  957,  734,  395,  772, 1007,  155,
  702, 1007,  785, 1012,  207,  708,  709,  710,  399, 1708,
  314, 1783,  538,  483,  802, 1012, 1740,  801, 1012,  951,
 1766,  695,  723, 1874,  244, 1875,  717,  106,  700,  247,
  396,  718, 1003,  721,  719,  695,  720,  107,  722,  343,
 1708,  994,  700,  785,  993, 1708,  785,  498,  538,  400,
  247,  344,  998,  717,  721,  719,  247,  720,  718,  722,
  758,  785,  108,  504,  995,  244,  538,  985,  244,  538,
  204, 1768,  490,  717, 1108,  755,  499, 1107,  718,  757,
  538,  502, 1769,  244,  345, 1758,  346,  717,  218,  219,
  247, 1571,  718, 1075, 1076,  536,  721,  719,  503,  720,
  505,  722,  695,  486,  504,  507,  508,  504, 1706,  700,
 1112,  509, 1706,  880, 1797,  419,  420,  421, 1358,  522,
  536, 1206,  504, 1706,  723,  226, 1791,  524,   55,  534,
 1449,  721,  719, 1006,  720, 1004,  722,  525,  226,   56,
 1807,  155, 1706,  429,  430,  527,   57,  544, 1209,  546,
  247, 1208, 1857,  548,  717, 1449,  536,   14,  659,  718,
  527, 1258, 1304,  560, 1107, 1303, 1807,  155,  561, 1309,
   58,  659, 1308, 1706,  536,  243,  243,  536, 1706,  659,
  131,  131,  562,  243,  527,  810, 1463,  564,  536,  723,
  527, 1449, 1004,  527,  659, 1546, 1807,  155,  723,  582,
  207, 1184,  721,  719,  595,  720,  585,  722,  659, 1449,
  648, 1125, 1449, 1378,  721,  719,  207,  720,  918,  722,
  922,  599,  652, 1449,  926,  928,  930, 1578,  654, 1245,
  723,  207,   59,  207,  656,  527,  896,  668,  810,  691,
 1615,  810, 1246,  723, 1833,  659, 1834, 1832,  810,  734,
  810,  698, 1861,  919,  810, 1764,  688,  924,  368,  810,
  689,  368,  810,   60,  940, 1247,  527,   61,  717,  947,
  948,  243,  724,  718,  690,  724, 1179,  810,  668, 1248,
  447,  668,  958,  959,  960,  961,  962,  963,  447,  717,
  527,  810,  243,  692,  718,  693,  668,  527,  243,  810,
  244,  244,  810,  810,  694,  695,  699,  527,  244,  696,
  527,  697,  810,  700,  705,  527, 1249,  785,  721,  719,
  785,  720,  706,  722,  707,  527,  517,  785,  810,  785,
  716,  717,  243,  785,  724,  723,  718,  732,  785,  504,
  504,  785,  725,  733,  504,  527, 1030,  504,  729,  527,
  810,  750,  549, 1037,  247,  810,  785,  659,  737, 1005,
  504,  742, 1555,  753,  760,  762,  717,  810,  773,  810,
  785,  718,  777,  778,  782,  784,  785,  788,  785,  790,
  794,  785,  785,  555,  793,  796,  803,  797,  248,  827,
  828,  785,  243,  829,  834,  293,  244,  840,  839,  981,
 1005, 1005, 1005, 1005, 1005, 1005, 1005,  785,  846, 1379,
  721,  719,  848,  720,  847,  722,  810,  244, 1005, 1005,
 1005, 1005,  849,  244,  852,  857,  851,  863, 1250,  785,
  864,  867,  868,  877,  785,  504,  810,  717,  880,  886,
  810,  879,  718,  885,  887,  891,  785, 1245,  785,  717,
 1005,  942, 1005,  953,  718,  858,  504,  244,  295,  966,
 1246,  972,  504,  989, 1134, 1135, 1139,  991,  995, 1147,
 1149, 1151,  988, 1159, 1162, 1165, 1167, 1170, 1173, 1031,
  999, 1177, 1178, 1247, 1180, 1017, 1182, 1000, 1022, 1185,
 1186, 1187, 1188, 1189, 1190,  785,  504, 1248,  734, 1034,
 1045, 1054,   14, 1055, 1057, 1723, 1194,  806, 1199, 1061,
 1382,  721,  719, 1062,  720,  785,  722,  244, 1064,  785,
 1070, 1071, 1383,  721,  719, 1072,  720, 1078,  722, 1385,
  721,  719, 1085,  720, 1249,  722, 1091, 1096, 1349, 1098,
 1105, 1119,  527,  527, 1141,  109, 1181,  527, 1210, 1191,
  527, 1183,  668,  717,  717,  668, 1212, 1224,  718,  447,
 1222, 1707,  668,  527,  668, 1227, 1238, 1230,  668, 1242,
  220, 1244, 1252,  668,  110,  111,  668,  112, 1262, 1265,
 1285, 1287, 1297,  274,  113,  114, 1307,  115,  116, 1305,
  117,  668, 1314,  118,  119,  120,  121,  122, 1318, 1319,
 1320,  123,  124, 1321, 1329,  668, 1113,  125,  126,  127,
 1337,  220, 1342,  668,  220, 1343, 1344,  668, 1350, 1351,
 1355, 1005, 1724, 1005, 1005, 1356,  668, 1357, 1359,  220,
 1005, 1360, 1367, 1368,  834, 1369, 1005, 1370,  527, 1794,
 1372, 1005,  668, 1796, 1005,  717, 1250, 1373, 1823, 1374,
  718, 1375, 1376, 1377, 1802,  915, 1394, 1397, 1005,  527,
 1450, 1464, 1466, 1005, 1005,  527,  444, 1005, 1467,  668,
  758, 1476, 1478, 1824, 1005,  834, 1005, 1005,  834, 1471,
 1005,  668, 1477,  668, 1005, 1479, 1005, 1487, 1005, 1504,
 1005,  577, 1500,  834, 1505, 1005, 1005, 1512, 1005,  527,
 1082, 1506, 1507, 1517, 1848,  447, 1527, 1508, 1509, 1850,
  965, 1533, 1535, 1005, 1005, 1536, 1005, 1361, 1362, 1707,
 1364, 1366, 1541, 1707, 1548, 1798, 1568, 1005, 1371, 1005,
  668, 1005, 1005, 1005, 1707, 1005, 1574, 1581, 1005, 1005,
 1582,  221, 1588, 1575,  138, 1587,  717, 1384, 1005, 1590,
  668,  718, 1591, 1707,  668, 1391, 1392, 1199,  717, 1598,
 1199, 1005, 1194,  718, 1005,  717, 1399,  447, 1401, 1599,
  718, 1386,  721,  719, 1600,  720, 1601,  722, 1602, 1607,
 1005, 1608,  221, 1611, 1707,  221, 1005, 1005, 1612, 1707,
 1613, 1005, 1387,  721,  719, 1617,  720, 1634,  722, 1644,
  221, 1646, 1005, 1005, 1624, 1005,  447, 1648, 1647, 1649,
 1650, 1005, 1388,  721,  719, 1651,  720, 1653,  722, 1654,
 1655, 1663, 1665, 1669,  806, 1670, 1389,  721,  719, 1671,
  720, 1673,  722, 1390,  721,  719, 1674,  720, 1675,  722,
 1005, 1005, 1676, 1005, 1495, 1679,  232,  220, 1680, 1006,
 1005, 1005, 1005, 1005, 1005, 1681, 1005, 1682, 1683, 1005,
 1005, 1005, 1005, 1005, 1684, 1685, 1686, 1005, 1005,  109,
 1687, 1688, 1005, 1005, 1005, 1005, 1005, 1690, 1696, 1569,
  721,  719, 1603,  720, 1718,  722, 1703, 1728, 1736, 1570,
  721,  719,  966,  720,  985,  722,  834, 1738,  110,  111,
 1739,  112,  721,  719, 1202,  720, 1751,  722,  113,  114,
 1755,  115,  116, 1543,  117, 1761, 1762,  118,  119,  120,
  121,  122, 1551, 1765, 1772,  123,  124, 1775, 1778, 1556,
 1773,  125,  126,  127, 1780,  985,  985,  985,  985,  985,
  998,  985,  232, 1784, 1577,  721,  719, 1781,  720,  834,
  722, 1795,  834,  985,  985,  985,  985, 1792, 1199,  834,
 1816,  834, 1199,  232, 1821,  834, 1818, 1822, 1825,  232,
  834, 1764, 1148,  834, 1152, 1836, 1160, 1163, 1589, 1168,
 1171, 1174, 1813, 1831, 1837,  985, 1814,  985,  834,  721,
  719, 1203,  720, 1849,  722,  109,  444, 1851, 1852, 1856,
 1868, 1858,  834,  232, 1871, 1872, 1873,  717, 1876,    1,
  834,   30,  718,  834,  834,   32,  193,  232,  221,  979,
  732,  767,  261,  834,  110,  111,  370,  112,  717,  648,
  462,  156,   82,  718,  113,  114,  890,  115,  116,  834,
  117,  966, 1620,  118,  119,  120,  121,  122,  717,  272,
  203,  123,  124,  718,  739,  183,  271,  125,  126,  127,
  109,  834,  717,  398,  965,  692,  834,  718,  525,  717,
  694,  946,  184,  371,  718,  213,  322,  288,  834,  711,
  834,  945,  710,  330,  357,  494,  712,  328,  342,  110,
  111,  721,  112,  721,  719, 1579,  720,   27,  722,  113,
  114,   30,  115,  116,   40,  117, 1455,  143,  118,  119,
  120,  121,  122,  232,  496,  717,  123,  124, 1677, 1678,
  718,  506,  125,  126,  127,  717,  494,  834,  497,  494,
  718,  990, 1338,  878,  232, 1689, 1118,  717,  261, 1199,
  232, 1336,  718, 1692,  494, 1123, 1657,  834, 1698, 1081,
 1700,  834, 1326,  652, 1334,  566,  985, 1656,  985,  985,
  607,  841, 1315,  542, 1032,  985, 1327, 1264, 1694,  579,
 1059,  985,  580, 1737,  232, 1636,  985, 1817, 1812,  985,
  717, 1642, 1753,  721,  719,  718,  720,  822,  722, 1819,
 1847, 1757, 1052,  985, 1009,  985, 1214, 1593,  985,  985,
 1715, 1800,  985, 1482,  826,  969, 1627,  587,  572,  985,
  563,  985,  985, 1097, 1524,  985, 1088,  145,  865,  985,
  535,  985,  845,  985,  717,  985,  772,  768, 1749,  718,
  985,  985, 1023,  985, 1474, 1009, 1009, 1009, 1009, 1009,
 1009, 1009,  721,  719, 1019,  720, 1241,  722,  985,  985,
  256,  985,  306, 1009, 1009, 1009, 1009, 1779, 1732, 1855,
  781,  109,  985,  447,  985, 1835,  985,  985,  985, 1363,
  985, 1036,  996,  985,  985, 1234,   80, 1491,    0,    0,
 1396,    0,    0,  985,    0, 1009,    0, 1009,    0,    0,
  110,  111, 1620,  112,    0,    0,  985,    0,    0,  985,
  113,  114,  109,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0,  985,    0,  123,  124,    0,
    0,  985,  985,  125,  126,  127,  985,    0,  717,    0,
    0,  110,  111,  718,  112,    0, 1801,  985,  985,    0,
  985,  113,  114,    0,  115,  116,  985,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,  494,  494,    0,  125,  126,  127,  721,  719,  494,
  720,    0,  722,    0,    0,  985,  985,    0,  985,    0,
    0,    0,  494,    0, 1756,  985,  985,  985,  985,  985,
  109,  985,    0,    0,  985,  985,  985,  985,  985,    0,
    0,    0,  985,  985,    0,    0,    0,  985,  985,  985,
  985,  985,  721,  719,    0,  720,    0,  722,  717,  110,
  111,    0,  112,  718,    0,    0,  652,    0,    0,  113,
  114,    0,  115,  116,    0,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  126,  127,    0, 1009,  494, 1009, 1009,
    0,    0, 1554,    0,    0, 1009,    0,    0,    0,    0,
    0, 1009,    0,    0,    0,    0, 1009,  717,  494, 1009,
    0,    0,  718,    0,  494,  525,    0,    0,    0,    0,
    0,    0,    0, 1009, 1014,    0,    0,    0, 1009, 1009,
    0,    0, 1009,    0,    0,    0,    0,    0,    0, 1009,
    0, 1009, 1009,    0,    0, 1009,    0,    0,  494, 1009,
    0, 1009,    0, 1009,    0, 1009,  525,    0,    0,  525,
 1009, 1009,    0, 1009,    0, 1014, 1014, 1014, 1014, 1014,
 1014, 1014,    0,    0,  525,    0,  525,    0, 1009, 1009,
    0, 1009,    0, 1014, 1014, 1014, 1014,    0,    0,    0,
    0,  655, 1009,    0, 1009,    0, 1009, 1009, 1009,    0,
 1009,    0,    0, 1009, 1009,  931,  932,  933,  934,  935,
  936,  937,  938, 1009,    0, 1014,    0, 1014,    0,    0,
  655,  655,    0,  655,    0,    0, 1009, 1583,    0, 1009,
  655,  655,  717,  655,  655,    0,  655,  718,    0,  655,
  655,  655,  655,  655,    0, 1009,    0,  655,  655,    0,
    0, 1009, 1009,  655,  655,  655, 1009,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1009, 1009,    0,
 1009,    0,    0,    0, 1544, 1545, 1009,  717,    0,    0,
 1549, 1550,  718, 1552, 1553,    0,    0,    0,    0,    0,
    0, 1557, 1558, 1559, 1560,    0, 1561, 1562, 1563, 1564,
 1565, 1566,    0,    0,    0, 1009, 1009,    0, 1009,    0,
    0,   79,    0,  109,    0, 1009, 1009, 1009, 1009, 1009,
    0, 1009,    0,    0, 1009, 1009, 1009, 1009, 1009,    0,
    0,    0, 1009, 1009,    0,    0,    0, 1009, 1009, 1009,
 1009, 1009,  110,  111,    0,  112,    0,    0,    0,    0,
    0,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,  525,    0,  125,  126,  127,    0,    0,
    0,    0,    0,    0,    0,    0, 1014,    0, 1014, 1014,
  525,  525,  525,    0,    0, 1014,  525,    0,    0,  525,
    0, 1014,    0,    0,    0,    0, 1014,    0,    0, 1014,
    0,    0,  525,    0,    0,  522,    0,    0,  525,    0,
    0,    0,    0, 1014, 1010,    0,    0,    0, 1014, 1014,
    0,    0, 1014,    0,    0,    0,    0,    0,    0, 1014,
    0, 1014, 1014,    0,    0, 1014,    0,    0,    0, 1014,
    0, 1014,    0, 1014,    0, 1014,  522,    0,    0,  522,
 1014, 1014,   65, 1014,    0, 1010, 1010, 1010, 1010, 1010,
 1010, 1010,    0,   66,  522,    0,  522,    0, 1014, 1014,
   67, 1014,    0, 1010, 1010, 1010, 1010,  525,    0,    0,
    0,    0, 1014,   68, 1014,    0, 1014, 1014, 1014,    0,
 1014,    0,    0, 1014, 1014,    0,    0,    0,  525,    0,
    0,    0,    0, 1014,  525, 1010,    0, 1010,    0,    0,
 1592,    0,    0,  525,    0,  525, 1014,    0,    0, 1014,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  525,    0, 1014,    0,    0,  525,    0,
   69, 1014, 1014,    0,    0,    0, 1014,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   70, 1014, 1014,    0,
 1014,    0,    0,    0,    0,    0, 1014,  525,    0,    0,
    0,    0,   71,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   72,    0,    0,
    0,   73,    0,    0,   74, 1014, 1014,    0, 1014,    0,
    0,    0,   75,    0,    0, 1014, 1014, 1014, 1014, 1014,
  109, 1014,    0,    0, 1014, 1014, 1014, 1014, 1014,    0,
    0,   76, 1014, 1014,    0,    0,    0, 1014, 1014, 1014,
 1014, 1014,    0,    0,    0,    0,    0,    0,    0,  110,
  111,    0,  112,    0,    0,    0,    0,    0,    0,  113,
  114,    0,  115,  116,    0,  117,    0,    0,  118,  119,
  120,  121,  122,  522,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  126,  127,    0, 1010,    0, 1010, 1010,
  522,  522,  522,    0,    0, 1010,  522,    0,    0,  522,
    0, 1010,    0,    0,    0,    0, 1010,    0,    0, 1010,
    0,    0,  522,    0,    0,  527,    0,    0,  522,    0,
    0,    0,    0, 1010, 1023,    0,    0,    0, 1010, 1010,
    0,    0, 1010,    0,    0,    0,    0,    0,    0, 1010,
    0, 1010, 1010,    0,    0, 1010,    0,    0,    0, 1010,
    0, 1010,    0, 1010,    0, 1010,  527,    0,    0,  527,
 1010, 1010,    0, 1010,    0, 1023, 1023, 1023, 1023, 1023,
 1023, 1023,    0,    0,  527,    0,  527,    0, 1010, 1010,
    0, 1010,    0, 1023, 1023, 1023, 1023,  522,    0,    0,
    0,    0, 1010,    0, 1010,    0, 1010, 1010, 1010,    0,
 1010,    0,    0, 1010, 1010,    0,    0,    0,  522,    0,
    0,    0,    0, 1010,  522, 1023,    0, 1023,    0,    0,
    0,    0,    0,    0,    0,  522, 1010,    0,    0, 1010,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  522,    0, 1010,    0,    0,  522,    0,
    0, 1010, 1010,    0,    0,    0, 1010,    0,    0,    0,
    0,    0,    0,  527,    0,    0,    0, 1010, 1010,    0,
 1010,    0,    0,    0,    0,    0, 1010,  522,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  527, 1010, 1010,  527, 1010,    0,
    0,    0,    0,  109,    0, 1010, 1010, 1010, 1010, 1010,
    0, 1010,  527,    0, 1010, 1010, 1010, 1010, 1010,    0,
    0,    0, 1010, 1010,    0,    0,    0, 1010, 1010, 1010,
 1010, 1010,  110,  111,    0,  112,    0,    0,    0,    0,
    0,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,  527,    0,  125,  126,  127,    0,    0,
    0,    0,    0,    0,    0,    0, 1023,    0, 1023, 1023,
    0,  527,  527,    0,    0, 1023,  527,    0,    0,  527,
    0, 1023,    0,    0,    0,    0, 1023,    0,    0, 1023,
    0,    0,  527,    0,    0,    0,    0,    0,  527,    0,
    0,    0,    0, 1023,  859,    0,    0,    0, 1023, 1023,
    0,    0, 1023,    0,    0,    0,    0,    0,    0, 1023,
    0, 1023, 1023,    0,    0, 1023,    0,    0,    0, 1023,
    0, 1023,    0, 1023,    0, 1023, 1267,    0,    0,    0,
 1023, 1023,    0, 1023,    0,  859,  859,  859,  859,  859,
    0,  859,    0,    0,    0,    0,    0,    0, 1023, 1023,
    0, 1023,    0,  859,  859,  859,  859,  527,    0,    0,
    0,    0, 1023,    0, 1023,    0, 1023, 1023, 1023,    0,
 1023,    0,    0, 1023, 1023,    0,    0,    0,  527,    0,
    0,    0,    0, 1023,  527,    0,    0,  859,    0,    0,
    0,    0,    0,    0,    0,  527, 1023,    0,    0, 1023,
  213,    0,    0,    0,    0,    0,    0,    0,    0,  527,
  527,    0,    0,  527,    0, 1023,    0,  527,  527,    0,
    0, 1023, 1023,    0,    0,    0, 1023,    0,    0,    0,
  527,    0,    0,  241,    0,    0,    0, 1023, 1023,    0,
 1023,  213,    0,    0,  213,    0, 1023,  527,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  213,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  241, 1023, 1023,  241, 1023,    0,
    0,    0,    0,    0,    0, 1023, 1023, 1023, 1023, 1023,
    0, 1023,  241,    0, 1023, 1023, 1023, 1023, 1023,    0,
    0,    0, 1023, 1023,    0,  527,    0, 1023, 1023, 1023,
 1023, 1023,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  527,    0,    0,    0,
    0,    0,  527,   87,    0,    0,    0,    0,    0,    0,
   88,    0,    0,    0,   89,   90,    0,    0,    0,    0,
    0,    0,    0,   91,    0,    0,  859,    0,  859,  859,
    0,    0,    0,    0,   92,  859,  527,    0,    0,    0,
    0,  859,    0,  407,    0,    0,  859,    0,    0,  859,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  859,  871,    0,    0,    0,  859,  859,
    0,  409,  859,    0,    0,    0,    0,    0,    0,  859,
    0,  859,  859,    0,    0,  859,    0,    0,    0,  859,
    0,  859,    0,  859,    0,  859,   93,    0,    0,  411,
  859,  859,    0,  859,    0,  871,  871,  871,  871,  871,
    0,  871,   94,    0,    0,    0,    0,    0,  859,  859,
    0,  859,    0,  871,  871,  871,  871,   95,    0,    0,
    0,   96,  859,    0,  859,    0,  859,  859,  859,    0,
  859,    0,   97,  859,  859,    0,  213,  213,    0,    0,
    0,    0,    0,  859,  213,    0,    0,  871,    0,    0,
    0,   98,    0,    0,    0,    0,  859,    0,    0,  859,
  416,    0,    0,    0,    0,    0,    0,    0,    0,  241,
  241,    0,    0,    0,    0,  859,  418,  241,    0,    0,
    0,  859,  859,    0,    0,    0,  859,    0,    0,    0,
  419,  420,  421,    0,  218,  219,    0,  859,  859,    0,
  859,    0,    0,    0,    0,    0,  859,    0,    0,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,    0,    0,    0,    0,    0,    0,  434,  435,
    0,    0,  213,    0,    0,  859,  859,  439,  859,    0,
    0,    0,    0,    0,    0,  859,  859,  859,  859,  859,
    0,  859,    0,  213,  859,  859,  859,  859,  859,  213,
    0,    0,  859,  859,    0,  241,    0,  859,  859,  859,
  859,  859,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  241,    0,    0,    0,
    0,    0,  241,  213,  577,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1729,    0,
    0,    0,    0,    0,    0,    0,  871,    0,  871,  871,
    0,    0,    0,    0,    0,  871,  241,    0,    0,    0,
    0, 1730,    0,    0,    0,    0,  871,    0,    0,  871,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  871,  874,    0,    0,    0,  871,  871,
    0,    0,  871,    0,    0,    0,    0,    0,    0,  871,
    0,  871,  871,    0,    0,  871,    0,    0,    0,  871,
    0,  871,    0,  871,    0,  871,    0, 1731,    0,    0,
  871,  871,    0,  871,    0,  874,  874,  874,  874,  874,
    0,  874,    0,    0,    0,    0,    0,    0,  871,  871,
    0,  871,    0,  874,  874,  874,  874,    0,    0,    0,
    0,  109,  871,    0,  871,    0,  871,  871,  871,    0,
  871,    0,    0,  871,  871,    0,    0,    0,    0,    0,
    0,    0,    0,  871,    0,    0,    0,  874,    0,    0,
  110,  111,    0,  112,    0,    0,  871,    0,    0,  871,
  113,  114,  109,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0,  871,    0,  123,  124,    0,
    0,  871,  871,  125,  126,  127,  871,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0,  871,  871,    0,
  871,  113,  114,    0,  115,  116,  871,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,  487,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0,  871,  871,  537,  871,    0,
    0,    0,    0,  109,    0,  871,  871,  871,  871,  871,
    0,  871,    0,  583,  871,  871,  871,  871,  871,    0,
    0,    0,  871,  871,    0,    0,    0,  871,  871,  871,
  871,  871,  110,  111,    0,  112,    0,  537,  537,    0,
    0,  537,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,  537,    0,  537,  123,
  124,    0,    0,    0,    0,  125,  126,  127,    0,    0,
    0,    0,    0,    0,    0,    0,  874,    0,  874,  874,
    0,    0,    0,    0,    0,  874,    0,    0,  537,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  874,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  874,  875,    0,    0,    0,  874,  874,
    0,    0,  874,    0,    0,    0,    0,    0,    0,  874,
    0,  874,  874,    0,    0,  874,    0,    0,    0,  874,
    0,  874,    0,  874,    0,  874,    0,    0,    0,    0,
  874,  874,    0,  874,    0,  875,  875,  875,  875,  875,
    0,  875,    0,    0,    0,    0,    0,    0,  874,  874,
    0,  874,    0,  875,  875,  875,  875,    0,    0,    0,
    0,  109,  874,    0,  874,    0,  874,  874,  874,    0,
  874,    0,    0,  874,  874,    0,    0,    0,    0,    0,
    0,    0,    0,  874,    0,    0,    0,  875,    0,    0,
  110,  111,    0,  112,    0,    0,  874,    0,    0,  874,
  113,  114,    0,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0,  874,    0,  123,  124,    0,
    0,  874,  874,  125,  126,  127,  874,    0,    0,    0,
    0,    0,    0,    0,    0,  537,    0,  874,  874,    0,
  874,    0,    0,    0,    0,    0,  874,    0,    0,    0,
  537,    0,  537,  537,  537,    0,    0,    0,  537,    0,
    0,  537,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  537,  874,  874,    0,  874,    0,
  537,    0,    0,  537,    0,  874,  874,  874,  874,  874,
    0,  874,    0,    0,  874,  874,  874,  874,  874,    0,
    0,    0,  874,  874,    0,    0,    0,  874,  874,  874,
  874,  874,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  537,  950,  721,  719,    0,
  720,    0,  722,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  913,  912,  914,    0,  537,
    0,    0,    0,    0,    0,    0,  875,    0,  875,  875,
    0,    0,    0,    0,    0,  875,    0,    0,    0,    0,
  537,    0,    0,    0,    0,  977,  537,    0,    0,  875,
  537,    0,    0,  869,    0,    0,    0,  537,    0,    0,
    0,    0,    0,  875,    0,    0,    0,    0,  875,  875,
    0,    0,  875,    0,    0,  537,    0,    0,    0,  875,
  537,  875,  875,    0,    0,  875,    0,    0,    0,  875,
    0,  875,    0,  875,  869,  875,  869,  869,  869,    0,
  875,  875,    0,  875,    0,  537,    0,    0,    0,  537,
    0,    0,  869,  869,  869,  869,    0,    0,  875,  875,
    0,  875,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  109,  875,    0,  875,    0,  875,  875,  875,    0,
  875,    0,    0,  875,  875,    0,  869,    0,    0,    0,
    0,    0,    0,  875,    0,  980,    0,    0,    0,    0,
  110,  111,    0,  112,    0,    0,  875,    0,    0,  875,
  113,  114,  109,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0,  875,    0,  123,  124,    0,
    0,  875,  875,  125,  126,  127,  875,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0,  875,  875,    0,
  875,  113,  114,    0,  115,  116,  875,  117,    0,    0,
  118,  119,  120,  121,  122,    0,  900,    0,  123,  124,
    0,    0,  717,    0,  125,  126,  127,  718,    0,    0,
  901,    0,    0,    0,    0,  875,  875,    0,  875,    0,
    0,    0,    0,    0,    0,  875,  875,  875,  875,  875,
    0,  875,    0,    0,  875,  875,  875,  875,  875,    0,
    0,    0,  875,  875,    0,    0,  902,  875,  875,  875,
  875,  875,    0,    0,  903,    0,  721,  719,    0,  720,
    0,  722,  904,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  913,  912,  914,    0,    0,    0,
  905,    0,  906,    0,    0,  869,    0,  869,  869,    0,
    0,    0,    0,    0,  869,  907,    0,  908,  909,  910,
    0,    0,    0,    0,    0,    0,    0,    0,  869,    0,
    0,    0,  868,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  869,    0,    0,    0,    0,  869,  869,    0,
    0,  869,    0,    0,    0,    0,    0,    0,  869,    0,
  869,  869,    0,    0,  869,    0,  911,    0,  869,    0,
  869,    0,  869,  868,  869,  868,  868,  868,    0,  869,
  869,    0,  869,    0,    0,    0,    0,  491,    0,    0,
    0,  868,  868,  868,  868,    0,    0,  869,  869,    0,
  869,    0,    0,    0,  109,    0,    0,    0,    0,    0,
    0,  869,    0,  869,    0,  869,  869,  869,    0,  869,
    0,    0,  869,  869,    0,  868,    0,    0,    0,    0,
    0,    0,  869,  110,  111,    0,  112,    0,    0,    0,
    0,    0,    0,  113,  114,  869,  115,  116,  869,  117,
    0,  570,  118,  119,  120,  121,  122,    0,    0,    0,
  123,  124,    0,    0,  869,    0,  125,  126,  127,    0,
  869,  869,    0,    0,    0,  869,    0,    0,    0,    0,
  570,  570,    0,  570,    0,    0,  869,  869,    0,  869,
  570,  570,    0,  570,  570,  869,  570,    0,    0,  570,
  570,  570,  570,  570,    0,  900,    0,  570,  570,    0,
    0,  717,    0,  570,  570,  570,  718,    0,    0,  901,
    0,    0,    0,    0,  869,  869,    0,  869,    0,    0,
    0,    0,    0,    0,  869,  869,  869,  869,  869,    0,
  869,    0,    0,  869,  869,  869,  869,  869,    0,    0,
    0,  869,  869,    0,    0,  902,  869,  869,  869,  869,
  869,    0,    0,  903,    0,    0,    0,    0,    0,    0,
    0,  904,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    1,    0,    0,    0,    0,    0,  905,
    0,  906,    0,    0,  868,    0,  868,  868,    0,    0,
    0,    2,    0,  868,  907,    0,  908,  909,  910,    3,
    0,    0,    0,    0,    0,    0,    4,  868,    5,    0,
    6,  870,    0,    0,    0,    7,    0,    0,    0,    0,
    0,  868,    8,    0,    0,    0,  868,  868,    0,    0,
  868,    0,    0,    0,    0,    0,    9,  868,    0,  868,
  868,    0,    0,  868,    0,  911,   10,  868,    0,  868,
    0,  868,  870,  868,  870,  870,  870,    0,  868,  868,
    0,  868,  211,    0,    0,  211,    0,    0,    0,    0,
  870,  870,  870,  870,    0,    0,  868,  868,  211,  868,
    0,    0,  211,    0,    0,  211,    0,    0,    0,    0,
  868,    0,  868,    0,  868,  868,  868,    0,  868,    0,
    0,  868,  868,    0,  870,    0,    0,    0,    0,   11,
    0,  868,    0,    0,    0,    0,   12,    0,    0,   13,
    0,   14,   15,    0,  868,  211,    0,  868,    0,    0,
    0,  211,  211,  211,    0,  211,  211,  211,    0,  211,
    0,    0,    0,  868,    0,    0,   16,    0,    0,  868,
  868,    0,    0,    0,  868,    0,    0,  211,    0,  211,
    0,    0,    0,    0,    0,  868,  868,    0,  868,    0,
    0,    0,    0,    0,  868,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  211,    0,
    0,    0,    0,   81,    0,   17,    0,    0,    0,    0,
    0,    0,    0,  868,  868,  211,  868,    0,    0,    0,
   18,   19,    0,  868,  868,  868,  868,  868,    0,  868,
    0,    0,  868,  868,  868,  868,  868,  211,    0,    0,
  868,  868,    0,    0,    0,  868,  868,  868,  868,  868,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  211,  211,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  870,    0,  870,  870,    0,    0,  211,
    0,    0,  870,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  870,    0,    0,    0,
  873,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  870,    0,    0,    0,    0,  870,  870,    0,    0,  870,
    0,    0,    0,    0,    0,    0,  870,    0,  870,  870,
    0,    0,  870,    0,    0,    0,  870,    0,  870,    0,
  870,  873,  870,  873,  873,  873,    0,  870,  870,    0,
  870,  881,    0,    0,  609,    0,    0,    0,    0,  873,
  873,  873,  873,    0,    0,  870,  870,  882,  870,    0,
    0,  610,    0,    0,  611,    0,    0,    0,    0,  870,
    0,  870,    0,  870,  870,  870,    0,  870,    0,    0,
  870,  870,    0,  873,    0,    0,    0,    0,    0,    0,
  870,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  870,  612,    0,  870,    0,    0,    0,
  613,  614,  615,    0,  616,  617,  618,    0,  619,    0,
    0,    0,  870,    0,    0,    0,    0,    0,  870,  870,
    0,    0,    0,  870,    0,    0,  620,    0,  621,    0,
    0,    0,    0,    0,  870,  870,    0,  870,    0,    0,
    0,    0,    0,  870,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  622,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  870,  870,  623,  870,    0,    0,    0,    0,
    0,    0,  870,  870,  870,  870,  870,    0,  870,    0,
    0,  870,  870,  870,  870,  870,  624,    0,    0,  870,
  870,    0,    0,    0,  870,  870,  870,  870,  870,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  850,  625,  626,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  873,    0,  873,  873,    0,    0,  627,    0,
    0,  873,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  873,    0,    0,    0,    0,
    0,  850, 1017, 1017,  850, 1017, 1017, 1017,    0,  873,
    0,    0,    0,    0,  873,  873,    0,    0,  873,  850,
 1017, 1017, 1017,    0,    0,  873,    0,  873,  873,    0,
    0,  873,    0,    0,    0,  873,    0,  873,    0,  873,
    0,  873,    0,    0,    0,    0,  873,  873,    0,  873,
    0, 1017,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  873,  873,    0,  873,    0,    0,
    0,  280,    0,    0,    0,    0,    0,    0,  873,    0,
  873,    0,  873,  873,  873,    0,  873,    0,    0,  873,
  873,    0,    0,    0,    0,    0,    0,    0,    0,  873,
  280,  280,    0,  280,    0,    0,    0,    0,    0,    0,
  280,  280,  873,  280,  280,  873,  280,    0,    0,  280,
  280,  280,  280,  280,    0,    0,    0,  280,  280,    0,
  666,  873,    0,  280,  280,  280,    0,  873,  873,    0,
    0,    0,  873,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  873,  873,    0,  873,    0,    0,    0,
    0,    0,  873,    0,    0,    0,    0,    0,    0,    0,
  666,  666,    0,    0,  666,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  666,
    0,  873,  873,    0,  873,    0,    0,    0,    0,    0,
    0,  873,  873,  873,  873,  873,    0,  873,    0,  851,
  873,  873,  873,  873,  873,    0,    0,    0,  873,  873,
    0,    0,  850,  873,  873,  873,  873,  873,    0,    0,
    0, 1017,    0,    0,    0,    0,    0, 1017,    0,    0,
    0,    0, 1017,    0,    0, 1017,    0,    0,    0,    0,
  851, 1018, 1018,  851, 1018, 1018, 1018,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  851, 1018,
 1018, 1018,    0,    0,    0,  850,    0,    0,  850,    0,
    0, 1017,    0,    0,    0,  850,    0,  850,    0, 1017,
    0,  850,    0,    0,    0,    0,  850, 1017,    0,  850,
 1018,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  850, 1017,    0, 1017,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  655,  850,    0,
 1017,    0, 1017, 1017, 1017,    0,  850,    0,    0,  850,
  850,    0,    0,    0,    0,    0,    0,    0,    0,  850,
    0,    0,    0,    0,    0,  852,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  850,    0,  655,  655,    0,
    0,  655,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1017,    0,    0,    0,    0,  655,  850,    0,    0,
    0,    0,  850,    0,    0,    0,  852, 1019, 1019,  852,
 1019, 1019, 1019,    0,  850,    0,  850,    0,    0,    0,
    0,    0,    0,    0,  852, 1019, 1019, 1019,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  666, 1019,    0,  666,    0,
    0,    0,    0,  850,    0,  666,    0,  666,    0,    0,
    0,  666,    0,    0,    0,    0,  666,    0,    0,  666,
    0,  851,    0,  850,    0,    0,    0,  850,    0,    0,
 1018,    0,    0,    0,  666,    0, 1018,    0,    0,    0,
    0, 1018,    0,    0, 1018,    0,    0,    0,  666,    0,
    0,    0,    0,    0,    0,    0,  666,    0,    0,    0,
  666,    0,    0,    0,    0,    0,    0,    0,    0,  666,
    0,    0,    0,    0,  851,    0,    0,  851,    0,    0,
 1018,  682,    0,    0,  851,  666,  851,    0, 1018,    0,
  851,    0,    0,    0,    0,  851, 1018,    0,  851,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  813,  666,  851, 1018,    0, 1018,    0,    0,    0,
    0,    0,  682,    0,  666,  682,  666,  851,    0, 1018,
    0, 1018, 1018, 1018,    0,  851,    0,    0,  851,  851,
  682,    0,    0,    0,    0,    0,    0,    0,  851,    0,
    0,    0,  813,    0,    0,  813,    0,    0,    0,    0,
    0,    0,    0,    0,  851,    0,    0,  852,    0,    0,
  813,    0,    0,  666,    0,    0, 1019,    0,    0,    0,
 1018,    0, 1019,    0,    0,    0,  851, 1019,    0,    0,
 1019,  851,  655,  666,    0,  655,    0,  666,    0,    0,
    0,    0,  655,  851,  655,  851,    0,    0,  655,    0,
    0,    0,    0,  655,    0,    0,  655,    0,    0,    0,
  852,    0,    0,  852,    0,    0, 1019,    0,    0,    0,
  852,  655,  852,    0, 1019,    0,  852,    0,    0,    0,
    0,  852, 1019,    0,  852,  655,    0,    0,    0,    0,
    0,    0,  851,  655,    0,    0,    0,  655,    0,  852,
 1019,    0, 1019,    0,    0,    0,  655,    0,    0,    0,
    0,    0,  851,  852,    0, 1019,  851, 1019, 1019, 1019,
    0,  852,  655,    0,  852,  852,    0,  811,    0,    0,
    0,    0,    0,    0,  852,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  655,
  852,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  655,    0,  655,    0,    0, 1019,    0,  811,  655,
    0,  811,  852,    0,    0,    0,    0,  852,    0,    0,
    0,    0,    0,    0,    0,    0,  811,    0,    0,  852,
    0,  852,    0,    0,    0,    0,    0,    0,  655,  655,
    0,  655,    0,    0,    0,    0,    0,    0,  655,  655,
  655,  655,  655,  813,  655,    0,    0,  655,  655,  655,
  655,  655,    0,    0,  812,  655,  655,    0,    0,    0,
  655,  655,  655,  655,  655,    0,  682,    0,  852,  682,
    0,    0,    0,    0,    0,    0,  682,    0,  682,    0,
    0,    0,  682,    0,    0,    0,    0,  682,  852,    0,
  682,    0,  852,    0,    0,  812,  813,    0,  812,  813,
    0,    0,    0,    0,    0,  682,  813,    0,  813,    0,
    0,    0,  813,  812,    0,    0,    0,  813,    0,  682,
  813,    0,    0,    0,    0,    0,    0,  682,    0,    0,
    0,  682,    0,  842,    0,  813,    0,    0,    0,    0,
  682,    0,    0,    0,    0,    0,    0,    0,    0,  813,
    0,    0,    0,    0,    0,    0,  682,  813,    0,    0,
  813,  813,    0,    0,    0,    0,    0,    0,    0,    0,
  813,    0,    0,    0,  842,    0,    0,  842,    0,    0,
    0,    0,    0,  682,    0,    0,  813,    0,    0,    0,
    0,    0,  842,    0,    0,  682,    0,  682,    0,    0,
    0,    0,    0,  655,    0,    0,    0,    0,  813,    0,
    0,    0,    0,  813,    0,    0,    0,    0,    0,  811,
    0,    0,    0,    0,    0,  813,    0,  813,    0,    0,
    0,    0,  655,  655,    0,  655,    0,    0,    0,    0,
    0,    0,  655,  655,  682,  655,  655,    0,  655,    0,
    0,  655,  655,  655,  655,  655,    0,    0,    0,  655,
  655,    0,    0,  806,  682,  655,  655,  655,  682,    0,
    0,    0,  811,    0,  813,  811,    0,    0,    0,    0,
    0,    0,  811,    0,  811,    0,    0,    0,  811,    0,
    0,  807,    0,  811,  813,    0,  811,    0,  813,    0,
    0,    0,    0,    0,  806,    0,    0,  806,    0,    0,
    0,  811,    0,    0,    0,    0,  812,    0,    0,    0,
    0,    0,  806,    0,    0,  811,    0,    0,    0,    0,
    0,    0,  807,  811,    0,  807,  811,  811,    0,    0,
    0,    0,    0,    0,    0,    0,  811,    0,    0,    0,
  807,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  811,    0,    0,    0,  808,    0,    0,  812,
    0,    0,  812,    0,    0,    0,    0,    0,    0,  812,
    0,  812,    0,    0,  811,  812,    0,    0,    0,  811,
  812,    0,    0,  812,    0,  842,    0,    0,    0,    0,
    0,  811,    0,  811,    0,    0,    0,  808,  812,    0,
  808,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  812,    0,    0,  808,    0,    0,    0,    0,
  812,    0,    0,  812,  812,    0,    0,    0,    0,    0,
    0,    0,    0,  812,    0,    0,    0,    0,  842,    0,
  811,  842,    0,    0,    0,    0,    0,    0,  842,  812,
  842,    0,    0,    0,  842,    0,    0,    0,    0,  842,
  811,    0,  842,    0,  811,    0,    0,    0,    0,    0,
    0,  812,  841,    0,    0,    0,  812,  842,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  812,    0,
  812,  842,    0,    0,    0,    0,    0,    0,    0,  842,
  835,    0,  842,  842,    0,    0,    0,    0,    0,    0,
    0,    0,  842,  841,    0,  806,  841,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  842,    0,
    0,  841,    0,    0,    0,    0,    0,  812,    0,    0,
    0,  835,    0,  807,  835,    0,    0,    0,    0,    0,
  842,    0,    0,    0,    0,  842,    0,  812,    0,  835,
    0,  812,    0,    0,    0,    0,    0,  842,  806,  842,
    0,  806,    0,    0,    0,    0,    0,    0,  806,    0,
  806,    0,    0,    0,  806,    0,    0,    0,    0,  806,
    0,    0,  806,    0,    0,    0,  807,    0,    0,  807,
    0,    0,    0,    0,    0,    0,  807,  806,  807,    0,
    0,    0,  807,    0,    0,    0,  842,  807,  808,    0,
  807,  806,    0,  843,    0,    0,    0,    0,    0,  806,
    0,    0,  806,  806,    0,  807,  842,    0,    0,    0,
  842,    0,  806,    0,    0,    0,    0,    0,    0,  807,
    0,    0,    0,    0,    0,    0,    0,  807,  806,    0,
  807,  807,    0,    0,  843,    0,    0,  843,    0,    0,
  807,  808,    0,    0,  808,    0,    0,    0,    0,    0,
  806,  808,  843,  808,    0,  806,  807,  808,    0,    0,
    0,    0,  808,    0,    0,  808,    0,  806,    0,  806,
    0,    0,    0,    0,    0,    0,  844,    0,  807,    0,
  808,    0,    0,  807,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  808,  807,    0,  807,    0,    0,
    0,    0,  808,    0,  841,  808,  808,    0,    0,    0,
    0,    0,    0,    0,    0,  808,  806,  844,    0,    0,
  844,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  808,  835,    0,    0,  844,  806,    0,    0,    0,
  806,    0,    0,    0,  807,    0,    0,    0,  832,    0,
    0,    0,    0,  808,    0,    0,    0,  841,  808,    0,
  841,    0,    0,    0,  807,    0,    0,  841,  807,  841,
  808,    0,  808,  841,    0,    0,    0,    0,  841,    0,
    0,  841,    0,    0,    0,  835,    0,    0,  835,  832,
    0,    0,  832,    0,    0,  835,  841,  835,    0,    0,
    0,  835,    0,    0,    0,    0,  835,  832,    0,  835,
  841,    0,    0,    0,    0,    0,    0,    0,  841,  808,
  836,  841,  841,    0,  835,    0,    0,    0,    0,    0,
    0,  841,    0,    0,    0,    0,    0,    0,  835,  808,
    0,    0,    0,  808,    0,    0,  835,  841,    0,  835,
  835,    0,    0,    0,    0,  843,    0,    0,    0,  835,
    0,  836,    0,    0,  836,    0,    0,    0,    0,  841,
    0,    0,    0,    0,  841,  835,    0,    0,    0,  836,
    0,    0,    0,    0,    0,    0,  841,    0,  841,    0,
    0,    0,  845,    0,    0,    0,    0,  835,    0,    0,
    0,    0,  835,    0,    0,    0,    0,    0,  843,    0,
    0,  843,    0,    0,  835,    0,  835,    0,  843,    0,
  843,    0,    0,    0,  843,    0,    0,    0,    0,  843,
    0,    0,  843,  845,    0,  841,  845,    0,  844,    0,
    0,    0,    0,    0,    0,    0,    0,  843,    0,    0,
    0,  845,  833,    0,    0,  841,    0,    0,    0,  841,
    0,  843,    0,  835,    0,    0,    0,    0,    0,  843,
    0,    0,  843,  843,    0,    0,    0,    0,    0,    0,
    0,    0,  843,  835,    0,    0,    0,  835,    0,    0,
    0,  844,    0,  833,  844,    0,  833,    0,  843,    0,
    0,  844,    0,  844,    0,    0,    0,  844,    0,    0,
  832,  833,  844,    0,    0,  844,    0,    0,    0,    0,
  843,    0,    0,    0,    0,  843,    0,    0,    0,    0,
  844,    0,    0,    0,    0,    0,  837,  843,    0,  843,
    0,    0,    0,    0,  844,    0,    0,    0,    0,    0,
    0,    0,  844,    0,    0,  844,  844,    0,    0,    0,
    0,    0,    0,  832,    0,  844,  832,    0,    0,    0,
    0,    0,    0,  832,    0,  832,    0,  837,    0,  832,
  837,  844,  836,    0,  832,    0,  843,  832,    0,    0,
    0,    0,    0,    0,    0,  837,    0,    0,    0,  679,
    0,    0,  832,  844,    0,    0,  843,    0,  844,    0,
  843,    0,    0,    0,    0,    0,  832,    0,    0,    0,
  844,    0,  844,    0,  832,    0,    0,  832,  832,    0,
    0,    0,    0,    0,    0,  836,    0,  832,  836,    0,
  679,    0,    0,  679,    0,  836,    0,  836,    0,    0,
    0,  836,    0,  832,  845,    0,  836,    0,  679,  836,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  844,
    0,    0,    0,    0,  836,  832,    0,    0,    0,    0,
  832,    0,    0,    0,    0,    0,    0,    0,  836,  844,
    0,    0,  832,  844,  832,    0,  836,    0,    0,  836,
  836,    0,    0,    0,    0,    0,    0,  845,    0,  836,
  845,    0,    0,    0,  833,    0,    0,  845,    0,  845,
    0,    0,    0,  845,    0,  836,    0,    0,  845,    0,
    0,  845,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  832,    0,    0,    0,    0,  845,  836,    0,    0,
    0,    0,  836,    0,    0,    0,    0,    0,    0,    0,
  845,  832,    0,    0,  836,  832,  836,  833,  845,    0,
  833,  845,  845,    0,    0,    0,    0,  833,    0,  833,
    0,  845,    0,  833,    0,    0,    0,  672,  833,    0,
  441,  833,  442,    0,    0,    0,    0,  845,  837,    0,
    0,    0,    0,    0,    0,  444,  833,    0,    0,    0,
  295,    0,    0,  836,    0,    0,    0,    0,    0,  845,
  833,    0,    0,    0,  845,    0,    0,    0,  833,    0,
    0,  833,  833,  836,    0,    0,  845,  836,  845,    0,
    0,  833,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  837,    0,    0,  837,    0,    0,  833,    0,    0,
    0,  837,    0,  837,    0,    0,    0,  837,    0,    0,
    0,    0,  837,    0,    0,  837,    0,  211,    0,  833,
  211,    0,    0,    0,  833,  845,    0,    0,    0,    0,
  837,    0,    0,  211,    0,    0,  833,  211,  833,  102,
  211,    0,    0,    0,  837,  845,    0,    0,    0,  845,
    0,    0,  837,    0,  679,  837,  837,  679,    0,    0,
    0,    0,    0,    0,  679,  837,  679,    0,    0,    0,
  679,    0,    0,    0,    0,  679,    0,  103,  679,    0,
  211,  837,    0,    0,    0,  833,  211,  211,  211,  104,
  211,  211,  211,  679,  211,    0,    0,    0,    0,    0,
    0,    0,    0,  837,    0,  833,    0,  679,  837,  833,
    0,    0,  211,  105,  211,  679,    0,    0,    0,  679,
  837,    0,  837,    0,    0,    0,    0,    0,  679,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  211,  679,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  404,    0,    0,    0,    0,    0,
  211,  405,    0,    0,    0,    0,    0,    0,    0,  837,
    0,  679,  672,  406,    0,  441,    0,  442,  106,  407,
  408,    0,  211,  679,    0,  679,    0,    0,  107,  837,
  444,    0,    0,  837,    0,  295,    0,    0,  666,    0,
    0,    0,    0,    0,    0,    0,    0,  409,    0,    0,
  410,    0,    0,  108,    0,    0,    0,  211,  211,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  109,  679,    0,  211,  411,    0,  412,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,    0,    0,
    0,  415,  679,    0,    0,    0,  679,    0,    0,  892,
  110,  111,    0,  112,    0,    0,    0,    0,    0,    0,
  113,  114,    0,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  126,  127,   14,    0,    0,    0,
  668,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  419,  420,  421,  422,
  218,  219,    0,    0,    0,    0,    0,    0,    0,    0,
  423,    0,    0,    0,    0,    0,    0,    0,    0,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,    0,  434,  435,    0,  436,  437,  438,
    0,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,  893,  894,  895,  123,  124,    0,    0,  404,
    0,  125,  440,  127,  672,    0,  405,  441,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,  665,  444,    0,  407,  408,    0,  295,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  666,    0,    0,    0,    0,    0,    0,
    0,    0,  409,    0,    0,  410,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  411,    0,  412,  413,    0,    0,    0,    0,    0,    0,
    0,    0,  414,    0,    0,    0,  415,    0,    0,  985,
    0,    0,  985,    0,  667,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  102,  987,    0,  985,
    0,    0,  985,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  668,    0,    0,    0,    0,
    0,  416,    0,    0,  103,    0,    0,  417,    0,    0,
    0,    0,  985,    0,    0,    0,  104,  418,  985,  985,
  985,    0,  985,  985,  985,    0,  985,    0,    0,    0,
  140,  419,  420,  421,  422,  218,  219,    0,    0,    0,
  105,    0,    0,    0,  985,  423,  985,    0,    0,    0,
    0,    0,    0,    0,  424,  425,  426,  427,  428,  429,
  430,  431,  432,  110,  433,    0,  112,    0,    0,  434,
  435,    0,  436,  437,  438,  985,  115,  116,  439,  117,
    0,  404,  118,  119,  669,  670,  671,    0,  405,    0,
  123,  124,  985,    0,    0,    0,  125,  440,  127,  672,
  406,    0,  441,    0,  442,  106,  407,  408,    0,    0,
    0,    0,    0,    0,  985,  107,    0,  444,    0,    0,
    0,    0,  295,    0,    0,  666,    0,    0,    0,    0,
    0,    0,    0,    0,  409,    0,    0,  410,    0,    0,
  108,    0,    0,    0,    0,    0,    0,    0,    0,  985,
  985,    0,    0,    0,    0,    0,    0,    0,  109,    0,
    0,    0,  411,    0,  412,  413,  985,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
    0,    0,    0,    0,    0,    0,  888,  110,  111,    0,
  112,    0,    0,    0,    0,    0,    0,  113,  114,    0,
  115,  116,    0,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    0,
  125,  126,  127,    0,    0,    0,    0,  668,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,    0,    0,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  669,  670,  671,    0,
    0,    0,  123,  124,    0,    0,  404,    0,  125,  440,
  127,  672,    0,  405,  441,    0,  442,    0,    0,    0,
    0,    0,    0,    0,    0,  406,    0,    0,    0,  444,
    0,  407,  408,    0,  295,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  666,    0,    0,    0,    0,    0,    0,    0,    0,  409,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
    0,    0,    0,  415,    0,    0,  608,    0,    0,  609,
    0,  667,    0,    0,    0,    0,    0,    0,  511,    0,
    0,  441,    0,  442,    0,    0,  610,    0,    0,  611,
    0,    0,    0,    0,    0,    0,  444,    0,    0,    0,
    0,  295,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  668,    0,    0,    0,    0,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,  612,
    0,    0,    0,    0,  418,  613,  614,  615,    0,  616,
  617,  618,    0,  619,    0,    0,    0,    0,  419,  420,
  421,  422,  218,  219,    0,    0,    0,    0,    0,    0,
    0,  620,  423,  621,    0,    0,    0,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,  622,  115,  116,  439,  117,    0,  404,  118,
  119,  669,  670,  671,    0,  405,    0,  123,  124,  623,
    0,    0,    0,  125,  440,  127,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,    0,
    0,  624,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  666,    0,    0,    0,    0,    0,    0,    0,
    0,  409,    0,    0,  410,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  625,  626,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  411,
    0,  412,  413,  627,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,    0, 1136,    0,
    0, 1137,    0,  888,    0,  404,    0,    0,    0,    0,
    0,    0,  405,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  406,    0,    0,    0,    0,    0,
  407,  408,    0,    0,    0,    0,  511,    0,    0,  441,
    0,  442,    0,    0,  668,    0,    0,    0,    0,    0,
  416,    0,    0,    0,  444,    0,  417,    0,  409,  295,
    0,  410,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,  411,    0,  412,  413,
    0,    0,    0,    0,  423,    0,    0,    0,  414,    0,
    0,    0,  415,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124, 1212,    0,    0,    0,  125,  440,  127,    0,    0,
    0,    0,    0,    0,    0, 1138,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,    6,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
 1409, 1410,    0, 1411,    0,    0,    0,  419,  420,  421,
  422,  218,  219,    0,    0,    0,    0,    0,    0, 1412,
    0,  423,    0,    0,    0, 1413,    0,    0,    0,    0,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,    0,    0,  434,  435,    0,  436,  437,
  438,    0,  115,  116,  439,  117,    0,    0,  118,  119,
  120,  121,  122,    0,    0,    0,  123,  124,    0,    0,
    0,    0,  125,  440,  127,    0, 1146,    0,    0, 1137,
    0,    0, 1414,  404,  141,    0,    0,  141,    0,    0,
  405,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,  406,    0,  141,    0,    0,  141,  407,  408,
    0,    0,    0,    0,  511, 1415,    0,  441,    0,  442,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,  444,    0, 1603,    0,  409,  295,    0,  410,
    0,    0,    0,  109,    0,    0,    0,  141,    0,    0,
    0,    0,    0,  141,  141,  141,    0,  141,  141,  141,
    0,  141,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0, 1416,  111,    0,  112,  414,    0,    0,  141,
  415,  141,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118, 1417,  120,  121,  122,    0,    0,    0,  123,
  124, 1418, 1419, 1420,    0,  125,  126,  127,    0,    0,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  511,    0,    0,  441,    0,  442,  141,    0,    0,
    0,    0,    0, 1138,    0,  416,    0,    0,    0,  444,
    0,  417,    0,    0,  295,    0,    0,    0,    0,  141,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,  141,  141,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,  141,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    0,
  125,  440,  127,    0, 1150,    0,    0, 1137,    0,    0,
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
    0, 1158,    0,    0, 1137,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,  511,
    0,    0,  441,    0,  442,    0,    0,    0,    0,    0,
    0, 1138,    0,  416,    0,    0,    0,  444,    0,  417,
    0,  409,  295,    0,  410,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,  411,
    0,  412,  413,    0,    0,    0,    0,  423,    0,    0,
    0,  414,    0,    0,    0,  415,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124, 1212,    0,    0,    0,  125,  440,
  127,    0,    0,    0,    0,    0,    0,    0, 1138,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    6,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0, 1409, 1410,    0, 1411,    0,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,    0, 1412,    0,  423,    0,    0,    0, 1413,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  440,  127,    0, 1161,
    0,    0, 1137,    0,    0, 1414,  404,    0,    0,    0,
    0,    0,    0,  405,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   14,    0,  406,    0,    0,    0,    0,
    0,  407,  408,    0,    0,    0,    0,  511, 1415,    0,
  441,    0,  442,    0,    0,    0,    0,    0,   16,    0,
    0,    0,    0,    0,    0,  444,    0,    0,    0,  409,
  295,    0,  410,    0,    0,    0,  109,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0, 1416,  111,    0,  112,  414,
    0,    0,    0,  415,    0,  113,  114,    0,  115,  116,
    0,  117,    0,    0,  118, 1417,  120,  121,  122,    0,
    0,    0,  123,  124, 1418, 1419, 1420,    0,  125,  126,
  127,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  511,    0,    0,  441,    0,  442,
    0,    0,    0,    0,    0,    0, 1138,    0,  416,    0,
    0,    0,  444,    0,  417,    0,    0,  295,    0,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  419,  420,
  421,  422,  218,  219,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  440,  127,    0, 1166,    0,    0,
 1137,    0,    0,    0,  404,    0,    0,  538,    0,    0,
    0,  405,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  407,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  538,  538,    0,
    0,  538,    0,    0,    0,    0,    0,  409,    0,    0,
  410,    0,    0,    0,    0,    0,  538,    0,  538,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  411,    0,  412,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  538,    0,
    0,  415,    0,    0, 1169,    0,    0, 1137,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,  405,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
    0,    0,  511,    0,    0,  441,    0,  442,    0,    0,
    0,    0,    0,    0, 1138,    0,  416,    0,    0,    0,
  444,    0,  417,    0,  409,  295,    0,  410,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  419,  420,  421,  422,
  218,  219,  411,    0,  412,  413,    0,    0,    0,    0,
  423,    0,    0,    0,  414,    0,    0,    0,  415,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,    0,  434,  435,    0,  436,  437,  438,
    0,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,    0,    0,    0,  123,  124,    0,    0,    0,
    0,  125,  440,  127,    0,    0,    0,    0,    0,    0,
    0, 1138,    0,  416,    0,  538,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
  538,    0,  538,  538,  538,    0,    0,    0,  538,    0,
    0,  538,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,  538,    0,    0,  423,    0,    0,
  538,    0,    0,  538,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,  538,    0,    0,  125,  440,
  127,    0, 1172,    0,    0, 1137,    0,    0,    0,  404,
    0,    0,    0,    0,    0,    0,  405,    0,    0,  538,
  639,    0,  639,  639,    0,  639,    0,    0,  406,    0,
    0,    0,    0,    0,  407,  408,    0,    0,  639,    0,
  538,    0,    0,  639,    0,    0,  538,    0,    0,    0,
  538,    0,    0,    0,    0,    0,    0,  538,    0,    0,
    0,    0,  409,    0,    0,  410,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  538,    0,    0,    0,    0,
  538,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  411,    0,  412,  413,    0,    0,    0,    0,    0,    0,
    0,    0,  414,    0,    0,  538,  415,    0,    0,  538,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  633,    0,  633,
  633,    0,  633,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  633,    0,    0,    0,    0,
  633,    0,    0,    0,    0,    0,    0,    0,    0, 1138,
    0,  416,    0,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  419,  420,  421,  422,  218,  219,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,    0,    0,
    0,    0,    0,    0,  424,  425,  426,  427,  428,  429,
  430,  431,  432,  110,  433,    0,  112,    0,    0,  434,
  435,    0,  436,  437,  438,    0,  115,  116,  439,  117,
  639,    0,  118,  119,  120,  121,  122,  639,    0,    0,
  123,  124,    0,    0,  639,    0,  125,  440,  127,    0,
    0,    0,    0,    0,    0,    0,  639,    0,    0,    0,
    0,    0,  639,  639,    0,    0,    0,    0,    0,    0,
  639,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  639,    0,    0,  639,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  639,    0,
  639,  639,    0,    0,    0,    0,    0,    0,    0,    0,
  639,    0,    0,    0,  639,    0,  511,  633,  516,  441,
    0,  442,    0,    0,  633,    0,    0,    0,    0,    0,
    0,  633,    0,    0,  444,    0,    0,    0,    0,  295,
    0,    0,    0,  633,    0,    0,    0,    0,    0,  633,
  633,    0,    0,    0,    0,    0,    0,  633,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  639,
    0,    0,    0,    0,    0,  639,    0,  633,    0,    0,
  633,    0,    0,    0,    0,  639,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  639,
  639,  639,  639,  639,  639,  633,    0,  633,  633,    0,
    0,    0,    0,  639,    0,    0,    0,  633,    0,    0,
    0,  633,  639,  639,  639,  639,  639,  639,  639,  639,
  639,  639,  639,    0,  639,    0,    0,  639,  639,    0,
  639,  639,  639,    0,  639,  639,  639,  639,    0,    0,
  639,  639,  639,  639,  639,    0,    0,    0,  639,  639,
    0,    0,    0,    0,  639,  639,  639,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  633,    0,    0,    0,
    0,    0,  633,    0,    0,  977,    0,    0,    0,    0,
    0,    0,  633,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  633,  633,  633,  633,
  633,  633,    0,    0,    0,    0,    0,    0,    0,    0,
  633,    0,    0,    0,    0,    0,    0,    0,    0,  633,
  633,  633,  633,  633,  633,  633,  633,  633,  633,  633,
    0,  633,    0,  404,  633,  633,    0,  633,  633,  633,
  405,  633,  633,  633,  633,  978,    0,  633,  633,  633,
  633,  633,  406,    0,    0,  633,  633,    0,  407,  408,
    0,  633,  633,  633,  511,    0,    0,  441,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  979,    0,  444,    0,    0,  980,  409,  295,    0,  410,
    0,  981,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  109,    0,    0,    0,    0,    0,    0,    0,
  982,    0,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,  110,  111,    0,  112,    0,    0,    0,    0,    0,
    0,  113,  114,    0,  115,  116,    0,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,    0,    0,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    0,
  125,  440,  127,  511,    0,    0,  441,    0,  442,    0,
    0,  404,    0,    0,    0,    0,    0,    0,  405,    0,
    0,  444,    0,    0,    0,    0,  295,    0,    0,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  409,    0,    0,  410,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,  412,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
  511,  954,    0,  441,    0,  442,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  444,    0,
    0,    0,    0,  295,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,  701,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,    0,    0,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
  404,  434,  435,    0,  436,  437,  438,  405,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,  406,
    0,    0,  123,  124,    0,  407,  408,    0,  125,  440,
  127,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  409,    0,    0,  410,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  411,    0,  412,  413,    0,    0,    0,    0,    0,
    0,    0,    0,  414,    0,    0,    0,  415,    0,  511,
    0,    0,  441,    0,  442,    0,    0,  404,    0,    0,
    0,    0,    0,    0,  405,    0,    0,  444,    0,    0,
    0,    0,  295,    0,    0,    0,  406,    0,    0,    0,
    0,    0,  407,  408,    0,    0,    0,    0,    0,    0,
    0,    0,   14,    0,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,  423,    0,    0,    0,    0,    0,    0,
    0,    0,  424,  425,  426,  427,  428,  429,  430,  431,
  432,  110,  433,    0,  112,    0,  404,  434,  435,    0,
  436,  437,  438,  405,  115,  116,  439,  117,    0,    0,
  118,  119,  120,  121,  122,  406,    0,    0,  123,  124,
    0,  407,  408,    0,  125,  440,  127,  511,    0,    0,
  441,    0,  442,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  444,    0,    0,    0,  409,
  295,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
    0,    0,    0,  415,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  511,    0,    0,  441,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,  416,    0,
    0,    0,  444, 1697,  417,    0,    0,  295,    0,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1164,    0,    0,  419,  420,
  421,  422,  218,  219,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  440,  127,    0,    0,    0,    0,
    0,    0,    0,    0,  404,    0,    0,    0,    0,    0,
    0,  405,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  407,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  409,    0,    0,
  410,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  411,    0,  412,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,    0,    0,
    0,  415,    0,  977,    0,  977,  977,    0,  977,    0,
    0,  404,    0,    0,    0,    0,    0,    0,  405,    0,
    0,  977,    0,    0,    0,    0,  977,    0,    0,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  417,    0,  409,    0,    0,  410,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1365,    0,    0,  419,  420,  421,  422,
  218,  219,  411,    0,  412,  413,    0,    0,    0,    0,
  423,    0,    0,    0,  414,    0,    0,    0,  415,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,    0,  434,  435,    0,  436,  437,  438,
    0,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,    0,    0,    0,  123,  124,    0,    0,    0,
    0,  125,  440,  127,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,    0,    0,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
  977,  434,  435,    0,  436,  437,  438,  977,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,  977,
    0,    0,  123,  124,    0,  977,  977,    0,  125,  440,
  127,  443,    0,    0,  441,    0,  442,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  444,
    0,    0,    0,  977,  295,    0,  977,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  977,    0,  977,  977,    0,    0,    0,    0,    0,
    0,    0,    0,  977,    0,    0,    0,  977,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  511,    0,
    0,  441,    0,  442,    0,    0,    0,    0,    0,    0,
    0,    0,  977,    0,    0,    0,  444,    0,  977,    0,
    0,  295,    0,    0,    0,    0,    0,    0,  977,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  977,  977,  977,  977,  977,  977,    0,    0,
    0,    0,    0,    0,    0,    0,  977,    0,    0,    0,
    0,    0,    0,    0,    0,  977,  977,  977,  977,  977,
  977,  977,  977,  977,  977,  977,    0,  977,    0,    0,
  977,  977,    0,  977,  977,  977,    0,  977,  977,  977,
  977,    0,    0,  977,  977,  977,  977,  977,    0,    0,
    0,  977,  977,    0,    0,    0,    0,  977,  977,  977,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  409,    0,    0,  410,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  411,
    0,  412,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,  977,    0,    0,
  977,    0,  977,    0,    0,  404,    0,    0,    0,    0,
    0,    0,  405,    0,    0,  977,    0,    0,    0,    0,
  977,    0,    0,    0,  406,    0,    0,    0,    0,    0,
  407,  408,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  416,    0,    0,    0,    0,    0,  417,    0,  409,    0,
    0,  410,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,  411,    0,  412,  413,
    0,    0,    0,    0,  423,    0,    0,    0,  414,    0,
    0,    0,  415,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  440,  127,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  419,  420,  421,
  422,  218,  219,    0,    0,    0,    0,    0,    0,    0,
    0,  423,    0,    0,  532,    0,    0,    0,    0,    0,
  424,  425,  426,  427,  428,  429,  430,  431,  432,  110,
  433,    0,  112,    0,  977,  434,  435,    0,  436,  437,
  438,  977,  115,  116,  439,  117,    0,    0,  118,  119,
  120,  121,  122,  977,    0,  532,  123,  124,  532,  977,
  977,    0,  125,  440,  127,    0,    0,  554,    0,    0,
    0,    0,    0,  532,    0,  532,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  977,    0,    0,
  977,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  544,    0,  532,    0,    0,  554,    0,
    0,  554,    0,    0,    0,  977,    0,  977,  977,    0,
    0,    0,    0,    0,    0,    0,  554,  977,  554,    0,
    0,  977,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  544,    0,  529,  544,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  554,    0,
    0,    0,  544,    0,  544,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  977,  529,    0,    0,
  529,    0,  977,    0,  544,    0,    0,    0,    0,    0,
    0,    0,  977,    0,    0,  529,    0,  529,    0,    0,
    0,    0,    0,    0,    0,    0,  977,  977,  977,  977,
  977,  977,    0,    0,    0,    0,    0,    0,    0,    0,
  977,    0,    0,    0,    0,    0,    0,  529,    0,  977,
  977,  977,  977,  977,  977,  977,  977,  977,  977,  977,
    0,  977,    0,    0,  977,  977,    0,  977,  977,  977,
    0,  977,  977,  977,  977,    0,    0,  977,  977,  977,
  977,  977,  532,    0,    0,  977,  977,    0,    0,    0,
    0,  977,  977,  977,    0,    0,    0,  532,    0,  532,
  532,  532,    0,    0,    0,  532,    0,    0,  532,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  532,    0,    0,    0,    0,    0,  532,    0,    0,
  532,    0,    0,    0,    0,  554,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  554,    0,    0,  554,  554,    0,    0,    0,  554,    0,
    0,  554,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  544,  532,    0,  554,    0,    0,    0,    0,    0,
  554,    0,    0,  554,    0,    0,  544,    0,    0,  544,
  544,    0,    0,    0,  544,    0,  532,  544,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  544,    0,    0,    0,  529,    0,  544,  532,    0,  544,
    0,    0,    0,  532,    0,  554,    0,  532,    0,  529,
    0,    0,  529,  529,  532,    0,    0,  529,    0,    0,
  529,    0,    0,    0,    0,    0,    0,    0,    0,  554,
    0,    0,  532,  529,    0,  320,    0,  532,    0,  529,
    0,  544,  529,    0,    0,    0,    0,    0,    0,    0,
  554,    0,    0,    0,    0,    0,  554,    0,    0,    0,
  554,    0,  532,  320,    0,  544,  532,  554,    0,    0,
  320,    0,    0,    0,  320,  320,    0,  320,    0,    0,
  320,    0,    0,    0,  529,  554,  544,    0,    0,    0,
  554,    0,  544,  320,    0,    0,  544,    0,    0,  320,
    0,    0,    0,  544,    0,    0,    0,  608,  529,    0,
  609,    0,    0,    0,    0,  554,  806,    0,    0,  554,
    0,  544,    0,    0,    0,    0,  544,  610,    0,  529,
  611,    0,    0,    0,    0,  529,    0,    0,    0,  529,
    0,    0,    0,    0,    0,    0,  529,    0,    0,    0,
    0,  544,    0,    0,    0,  544,  320,    0,    0,    0,
    0,    0,    0,    0,  529,    0,    0,    0,    0,  529,
  612,    0,    0,    0,  320,    0,  613,  614,  615,    0,
  616,  617,  618,    0,  619,    0,    0,    0,    0,  320,
    0,    0,    0,    0,  529,    0,    0,    0,  529,  320,
    0,    0,  620,    0,  621,    0,    0,    0,  320,    0,
  320,    0,    0,    0,    0,  608,    0,  320,  609,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  622,    0,  610,    0,    0,  611,    0,
    0,    0,    0,    0,    0,    0,  320,  320,    0,  320,
  623,    0,    0,    0,    0,    0,  320,  320,    0,  320,
  320,    0,  320,    0,    0,  320,  320,  320,  320,  320,
    0,    0,  624,  320,  320,  320,  320,  320,  612,  320,
  320,  320,    0,    0,  613,  614,  615,  109,  616,  617,
  618,    0,  619,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  625,  626,    0,
  620,    0,  621,    0,    0,    0,  110,  111,    0,  112,
    0,    0,    0,    0,  627,    0,  113,  114,    0,  115,
  116,    0,  117,    0,    0,  118,  119,  120,  121,  122,
    0,  622,    0,  123,  124,    0,    0,    0,    0,  125,
  126,  127,    0,    0,    0,    0,    0,    0,  623,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  624,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  109,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  625,  626,    0,    0,    0,
    0,    0,    0,    0,  110,  111,    0,  112,    0,    0,
    0,    0,  627,    0,  113,  114,    0,  115,  116,    0,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,  109,    0,    0,    0,  125,  126,  127,
    0,    0,  172,  173,  174,  175,  176,  177,  178,  179,
  180,  181,  182,  183,  184,  185,  186,  187,    0,    0,
  188,  189,  110,  111,    0,  112,    0,    0,  190,  191,
    0,    0,  113,  114,  192,  115,  116,  193,  117,  194,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,  195,  196,  197,  198,  125,  126,  127,  199,  458,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,  458,    0,    0,  458,  458,  458,  458,
    0,  458,    0,    0,  458,  458,    0,    0,  458,  458,
  458,  458,  458,  458,  458,  458,    0,  458,  458,  458,
  458,  458,    0,    0,    0,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,
};
static short yycheck[] = {                                       9,
   58,    0,   12,  156,    4,   16,   16,   67,   18,  108,
  546,   59,  146,  147,  146,  147,   66,  725,   94,   65,
  382,   66,   70,   16,  353,  382,  378,  701,   88,  578,
  522,  479,   73,  707,   92,   97,  549,   76,  374,  712,
 1057,  952,   91,  499, 1211,   93,   56,   57,   58,   59,
   60,   61,  100, 1046,  443,   96,   66,   67,   68,   98,
   70,    0,   72,   73, 1057,   75,   76,   60,  443,  660,
  778,  732,  297,   71,   84,   86,   86,   87,   88,   72,
   90,   91,   92,   93,   95,   95,   96,   97,   98,  233,
  100,  500,  137,   86,  139,  351,   94,   45,  160,  360,
  280,  653,  151, 1086,  326,   41,    0,  875, 1046,  289,
  158,  973, 1109,   44,   40,   45, 1284, 1051,   45, 1057,
   59,   45,   44,  571, 1058,  573,  387,  137,  138,  139,
  578, 1046,   44,   63,    0, 1069,  511,   63,    0,  359,
 1006,  151, 1057,  242,   44,  138,  156,    0,  158,  159,
  160,   40,    0, 1053,  164,    0, 1046,  203, 1092,   44,
 1046,   59,  785, 1046,  750,   59,  159, 1057,    0,    0,
  164, 1057,    0,  326, 1057,    0,    0,  800,   44,   41,
  803,    0,  228,  327,    0,   63,   41,   41,   41,  812,
   44,   44,   44,   59,  273,   44,   41,   59,   44,    0,
  505,   41,   41,    0, 1104,  557,   59,    0,    0,  539,
   41,   59, 1243,   44,   59,  667,  578,   41, 1229,   40,
   44,  578,   41,   40,  395,   44,  271,   59,   59,  274,
  750,   59,  331,    0,   59,   59,  351,  315,  315,   61,
   59,  915,  916,   59,   41,  868,   63,   44,   41,    0,
  308, 1418, 1419,  605,    0,   44,   41,   44,   59, 1276,
 1277,  271,   59,  352,  274,    0,   59,   59,  278,    0,
  280,  281,  946, 1222,   41, 1224,  298,  287,  484,  289,
  541,  326,  264, 1276, 1277, 1193,  317,  395, 1196,    0,
   41,   40,   59, 1310,  264,    0,   45,  672,  308,  397,
  275,  260,  312,  397,  396,   91, 1476,  352,   59,  273,
  516,  491,  334,   59,  359,  334,  361, 1310,  363,  312,
  412,  366,  416, 1230,   59,  581,  447,  716,   59,  375,
   41,  377,  421, 1316,  257,  724,  384,  395, 1276, 1277,
  283,  300,  352,  514,  385,  391,  499,    0,   59,  359,
  448,  361,   61,  363,   59,   44,  366, 1257,  297,  262,
  852, 1276, 1277,    0,  258, 1299,  341,  445,  445,  484,
    0, 1045, 1310,  629,  384,  385,  270,    0, 1244,  458,
  317,  829,    0,  393,  348,  395, 1276, 1277,   41,  399,
 1276, 1277,  500, 1276, 1277, 1310,  272,  428, 1305,  399,
  393,    0,  988,  297,   41,  344,   59,  330, 1031,  303,
   41,  305,  434,  999, 1000,  434,   41, 1279,   41,   41,
 1310,  403,   59,   41, 1310, 1048, 1049, 1310,    0,   59,
 1597,  761,   42,  403,  444, 1466,   59,   41,   41,  264,
  892,   59,  490,   41,    0, 1068,  287, 1617,  753,    0,
  344,  306,   44,  315,  499, 1129, 1130,  505, 1132, 1133,
   59, 1323,  315,  315,  596,  297,  306,  876,  988,   41,
  315,  317, 1640,  262,  336,  297,  379,  487,  887,  999,
  490,  491,  891,  336,  276,  277, 1394,   59,  498,  275,
 1398,  336,  502,  423,  487,  505,  315,  507,  387,  643,
  510,  546,  450,   59,  402,  498,  428,    0,   59,  502,
  453,  454,  344,  559,  301, 1464,  510,  336,  315,  450,
  450,  597,  315,  450,  450, 1116,  450,  376,  325,  477,
  327,  756, 1529,  578,  446,  380,  546,  993,  794,  336,
  379,  797,  339,  336,  389,  376,  446, 1208,  315,  403,
  879, 1103,  808,  599, 1322,    0, 1490,  779, 1541,  444,
 1494, 1495,  742,    0,  315,  390,   59,  577,  578,  336,
  509, 1739,  450,  583,  325, 1111,  327,  513,  444,  835,
  428,  801,  410,  380,  446,  336,  402,  432,  297,    0,
  379, 1265,  389,  446,  379,  729, 1530, 1006,  344,  597,
  401,  446, 1613,  649,  650,  866,  995,  652,  344,  344,
 1510, 1477,  301, 1287,   59,  509,  444, 1785,  439, 1481,
  304,  305,   59, 1282, 1033,  336,  779,  446, 1528,  380,
  361,  262,  764,  450,  264,  432, 1547,  262,  389, 1532,
  262,  494,  652,  692, 1812,    0,  310,  444,   59,  446,
  260,  513, 1238,  446,  380,    0,  361,  272,  262,  262,
  513,  457,  493,  459,  262,  264,  507, 1575,  513,  493,
   42,   43,  264,   45,  423,   47, 1693,  509,  315,  446,
 1353,  432,  692,  336, 1851,  730,   42,  732,    0,  734,
  300,   47, 1626,  444,  513,  446, 1630,  315, 1321,  336,
 1693,  450,  298, 1039,   59,  753,  432,  717,  264,  262,
  298, 1645,    0,  336,   59,  725,  513,  322,  336,  280,
  730,  336,  732,  297,  734,  276,  277,  737, 1662,  257,
   12,   12,  742,    0,  779,  446,    0,  260,  334,  784,
  750,  498,    0,  753,  790,  409,  513,   59,  379,  759,
  760,  827, 1141,   41,  379, 1693,  801,  379,   59,  443,
  390,  298,  513,  773,  336,  775, 1141,  315,  778, 1105,
  344,   59, 1285,  509,  784,  379,  379,  300, 1693,  262,
  773,  379,  775,  828,  829, 1114,  862,  863,  864,    0,
 1652,  801,   59,  446,  840,   59, 1658,  977,  268, 1055,
  980,   59,  330, 1693,  371, 1479,  344, 1693,  401,  446,
 1693, 1067, 1746,  258, 1203, 1749, 1308,  422,  828,  829,
    0,  372, 1314,  446,  429,  270,  379,  815,  446,  827,
   41, 1380,  842,   44,  390, 1091,  834,  261, 1497,    0,
  993,    0,  842,  410,  667,  338,  434,  258,   59,  672,
 1179,  849,  297, 1242, 1747, 1789,  262,  867,  303,  270,
  305,   41,  144,  144,  862,  863,  864,  877,    0,  265,
  431,   44,  305,  390,  867,  885,  886,   41,  371,   59,
   44,    0,  315,    0,  877,  885,  297,    0,    0,  359,
  457,    0,  303,    0,  305,    0,  379,  434,   59,  344,
   59,  338, 1775,  299,  397,  277,  258,  400, 1486,   41,
  282,  428,   44,  297, 1588,  453,  454, 1429,    0,  981,
   93,  277,   41,  416,  252,   44,  282,   59,   41,   41,
  978,   44,  260,  344,  371,  509,   41,  333,  979,   44,
   59,  352,   59,  982,  427,  462,   59,   59,  993,  442,
   59,  303,   59, 1531,   59, 1403,  397, 1089,  316, 1832,
  397,  371,  348,  400,  348,    0,    0,  977,  978,  979,
  980,  981,  982,  379, 1326, 1350, 1351,   59,  988,  416,
   93, 1356, 1357,  338, 1359, 1360,  372,    0,   93,  999,
 1000,  258, 1367, 1368, 1369, 1370, 1006, 1372, 1373, 1374,
 1375, 1376, 1377,  270,    0,  442,   41,  448,  280,   44,
  421,    0,  445,   41,   42,   43,  371,   45, 1380,   47,
    0,  427, 1098, 1380,   59,   59,    0,  315,   41,  259,
  440,   44,  263,  297,    0,  393,  303, 1113,  305,  269,
    0, 1297,  397, 1119, 1718,  400,   59,    0,  336, 1723,
  305,  262,   41,   44,    0,   44,  428, 1765,  410, 1212,
  315,  416, 1107,   59,  509,  888, 1111,   41, 1078,  892,
   59,  450, 1328, 1061, 1062,  433,  428,  284,  309,   59,
  344,    0,    0, 1083,  395,   59, 1096,  442,  352,  320,
  297,  450,  380,   59, 1228, 1229, 1230, 1107,  509,   59,
 1098, 1111,   93,  264,  315,  377,   59,  318,   40,  273,
  462,  304,  305,   59,  325, 1113,  327,  276,  277,  430,
  331, 1119,  386, 1111,  388,  336,  304,  305,  339, 1261,
  262, 1263,    0,   42,   43,  315,   45,  260,   47,  339,
   59,   59,  306,  354,  432,  325,  410,  327,   42,   43,
  259,   45,    0,   47,  319,  264,  336,  368,  446,  431,
  269, 1211,  362, 1208,   44,  376, 1211, 1212,  379,  380,
   44,  435,  298,    0,  313,  314,  448,  300,  389,   59,
  306,    0,  264,  315,  348,   59,  318,  298,  419,  319,
  445,   59, 1324,  325,  405,  327,  315,    0, 1208,  331,
  380, 1211, 1212,  315,  336,  394,  325,  339,  327,  389,
    0,   59, 1222,  325, 1224,  327,  427,  336,  418,    0,
  339,  432,  354,  334,  336,  513,    0,    0, 1238,  390,
  264,  338,   59,  444, 1244,  446,  368,   44,   41,  403,
   59,   44, 1778, 1499,  376,  509,    0,  379,  380,  277,
  443,    0,  432, 1222,  282, 1224,   59,  389, 1770,  428,
   41,  380,    0,  276,  277,  443,  446,   41,  380,   59,
  389,  284,  338,  405,  340,  264, 1429,  389,   59, 1535,
  315,  390,  493, 1795,  264,   59,   59,    0,  399,  402,
  397, 1337,  327,  400,  458,  427, 1342,  402,  409,    0,
  432,  336,  513, 1303,  264,   59,  517,   40,  390,  416,
   59,  264,  444,  432,  446,  376, 1469,    0,  264, 1831,
  432,   59,  485,  434,  276,  444,  315,  446,   41,  318,
  318,   44,  495,  513,  446,  442,  325, 1849,  327,  376,
 1852,  315,  331,  331,  395,  380,   59,  336,  265, 1409,
  339, 1863,  378,  327,  389,    0,  390,  259,   59,  372,
  271,  493,  336,  376,  316,  354,  354,  269,  277,    0,
    0,  372,  338,  282,  285,  426,   59,    0,  376,  368,
  393,  513,  299,  277, 1429,  517,  399,  376,  282,  376,
  441,  380,  393,    0,  513,  486,   44,  432,  399, 1409,
  389,  513,  260, 1659,  333,  371,  380, 1417, 1418, 1419,
  390,  446,  434,   44,   59,  389,  405,  405,  329, 1429,
  433,   42,   43,  317,   45,    0,   47,  336,   59,   59,
  390,  397,  433,  409,  400,    0,   59,  390,   59,    0,
   44,  393,  300,  432,  390,   93,  357, 1600,   42,   43,
  416,   45,   59,   47, 1464,  444,  260,  446,  432, 1469,
  436, 1614,    0,  452,  270,  277, 1476, 1477,   42,   43,
  282,   45,  446,   47,    0, 1247,  442, 1249,  513, 1613,
  493,  433,   42,   43,   59,   45,  410,   47,  294,   93,
 1500,  446,  481,  482,   59,  484,  300,  303,   59,  513,
    0,    0,  491,  492,  493,  494,  495, 1500,  497,  273,
  421,  500,  501,  502,  503,  504,   40,    0,  260,  508,
  509,   59,    0,    0,  513,  514,  515,  516,  517,    0,
    0,  337,  333,   59,  315,  341,  338, 1585,  352,  513,
  258,  315,   41,  261, 1643,   44,  327,  416,   41,   42,
   43,  260,   45,    0,   47,  336,  298,  450,   41,   59,
   59,   44,  336,  276,  277,  453,  333,  368,  310,  371,
   41,  284,  386,   44,  388, 1585,   59,  264,   61,  380,
 1600,   59,   59,  313,  477,  303, 1644, 1597,   59,   59,
   41,  300,  334,   44,   41,  397,  410,   44,  400,  380,
  409,  368,  485,  486, 1614, 1758,  380, 1617,  389, 1762,
 1609, 1764,   59,  380,  416,  389,   46, 1770,  315, 1665,
 1773,  435,   41, 1669, 1634,   44,  333,  436,   58, 1639,
  436,   61,    0,   46, 1644,  441,  442,  443,  332, 1792,
  442, 1740, 1795,  403,   41,   58, 1639,   44,   61,   41,
 1710,  432,   44, 1871,    0, 1873,  277,  399,  432,  372,
  452,  282,   41,   42,   43,  446,   45,  409,   47,  258,
 1823,   41,  446,   41,   44, 1828,   44,  336, 1831,  444,
  393,  270,   46,  277,   42,   43,  399,   45,  282,   47,
  264,   59,  434,    0,   58,   41, 1849,   61,   44, 1852,
 1710, 1711,  395,  277,   41,  511,   40,   44,  282,  515,
 1863,  317, 1712,   59,  303,  336,  305,  277,  453,  454,
  433,  315,  282,  384,  385, 1770,   42,   43,  315,   45,
  395,   47,  513, 1778,   41,  376,  376,   44, 1758,  513,
   41,  498, 1762,   44, 1764,  449,  450,  451,  308,  428,
 1795,   41,   59, 1773,   44, 1765, 1755,  356,  291,  426,
 1770,   42,   43,   40,   45, 1775,   47,  442, 1778,  302,
 1781, 1781, 1792,  477,  478,  258,  309,  377,   41,  375,
  493,   44, 1840,  497,  277, 1795, 1831,  409,  318,  282,
  273,   41,   41,  508,   44,   44, 1807, 1807,  509,   41,
  333,  331,   44, 1823, 1849,  276,  277, 1852, 1828,  339,
  453,  454,  508,  284,  297,  262,   41,  284, 1863,   44,
  303, 1831, 1832,  306,  354,   41, 1837, 1837,   44,  461,
 1840,   41,   42,   43,   61,   45,   44,   47,  368, 1849,
  283, 1851, 1852,   41,   42,   43, 1856,   45,  688,   47,
  690,  366,   40, 1863,  694,  695,  696,   41,  264,  318,
   44, 1871,  395, 1873,   40,  348,  672,    0,  315,  453,
   41,  318,  331,   44,   41,  405,   41,   44,  325,   44,
  327,  453,   41,  689,  331,   44,   40,  693,   41,  336,
   40,   44,  339,  426,  700,  354,    0,  430,  277,  705,
  706,  372,   41,  282,   40,   44,  264,  354,   41,  368,
  716,   44,  718,  719,  720,  721,  722,  723,  724,  277,
  403,  368,  393,   40,  282,   40,   59,  410,  399,  376,
  276,  277,  379,  380,   40,   40,  453,   41,  284,   40,
   44,   40,  389,   40,   40,  428,  405,  315,   42,   43,
  318,   45,   40,   47,   40,   59,  762,  325,  405,  327,
   40,  277,  433,  331,   91,   44,  282,   40,  336,  276,
  277,  339,   46,  264,  281,  458,  782,  284,   61,  462,
  427,  317,  788,  789,  265,  432,  354,  517,  428,    0,
  297,   44,  308,  395,  317,   44,  277,  444,   44,  446,
  368,  282,  420,   46,   61,   40,  264,   44,  376,  344,
  276,  379,  380,  344,  344,  276,  264,  297,  299,  390,
  428,  389,  493,  462,  390,  450,  372,  344,  275,   40,
   41,   42,   43,   44,   45,   46,   47,  405,  439,   41,
   42,   43,  392,   45,  439,   47,  493,  393,   59,   60,
   61,   62,   40,  399,   40,   91,  345,   40,  517,  427,
   40,  315,  264,  376,  432,  372,  513,  277,   44,  375,
  517,  403,  282,  382,   40,   40,  444,  318,  446,  277,
   91,  443,   93,   41,  282,  275,  393,  433,   63,   42,
  331,  296,  399,  456,  900,  901,  902,  314,   40,  905,
  906,  907,  428,  909,  910,  911,  912,  913,  914,  264,
  317,  917,  918,  354,  920,  444,  922,  317,  440,  925,
  926,  927,  928,  929,  930,  493,  433,  368,   44,  494,
   40,  273,  409,   40,  297,  376,  942,  281,  944,  340,
   41,   42,   43,  340,   45,  513,   47,  493,  276,  517,
  379,  379,   41,   42,   43,  379,   45,  410,   47,   41,
   42,   43,  408,   45,  405,   47,  297,  410,  262,   40,
  352,   40,  276,  277,   40,  452,   41,  281,  438,  317,
  284,   44,  315,  277,  277,  318,  270,   44,  282,  995,
   44, 1600,  325,  297,  327,  378,  317,  361,  331,   40,
    0,   44,  325,  336,  481,  482,  339,  484,  266,   40,
  493,   40,  264,   40,  491,  492,   41,  494,  495,  414,
  497,  354,   58,  500,  501,  502,  503,  504,   41,   41,
   41,  508,  509,  264,  446,  368,  387,  514,  515,  516,
  306,   41,  306,  376,   44,   41,   41,  380,   40,   40,
  348,  262,  493,  264,  265,   40,  389,   40,   40,   59,
  271,   40,   40,   40,    0,   40,  277,   40,  372, 1758,
   40,  282,  405, 1762,  285,  277,  517,   40,   40,   40,
  282,   40,   40,   40, 1773,  262,  427,  305,  299,  393,
  324,   44,   41,  304,  305,  399,   58,  308,   41,  432,
  264,  339,  273, 1792,  315,   41,  317,  318,   44,  381,
  321,  444,  339,  446,  325,  327,  327,  384,  329,  379,
  331,  284,  399,   59,  379,  336,  337,   41,  339,  433,
  450,  379,  379,  372, 1823, 1141,   41,  379,  379, 1828,
  293,  276,   40,  354,  355,   41,  357, 1153, 1154, 1758,
 1156, 1157,   41, 1762,   41, 1764,   41,  368, 1164,  370,
  493,  372,  373,  374, 1773,  376,  305,   59,  379,  380,
  290,    0,   40,  427,  336,   59,  277, 1183,  389,   59,
  513,  282,   59, 1792,  517, 1191, 1192, 1193,  277,   59,
 1196,  402, 1198,  282,  405,  277, 1202, 1203, 1204,   59,
  282,   41,   42,   43,  336,   45,   59,   47,  305,   59,
  421,   59,   41,   58, 1823,   44,  427,  428,  378, 1828,
  428,  432,   41,   42,   43,  339,   45,  277,   47,  333,
   59,  434,  443,  444,  389,  446, 1242,  434,  334,  298,
  334,  452,   41,   42,   43,  298,   45,   41,   47,   41,
  348,  378,  365,  365,  281,   41,   41,   42,   43,   41,
   45,   41,   47,   41,   42,   43,   41,   45,   41,   47,
  481,  482,   41,  484, 1280,   41,  276,  277,   41,   40,
  491,  492,  493,  494,  495,   41,  497,   41,   41,  500,
  501,  502,  503,  504,   41,   41,   41,  508,  509,  452,
   41,   41,  513,  514,  515,  516,  517,   41,  315,   41,
   42,   43,  443,   45,   40,   47,   59,   44,   41,   41,
   42,   43,   42,   45,    0,   47,  262,  348,  481,  482,
  340,  484,   42,   43,   44,   45,  458,   47,  491,  492,
   40,  494,  495, 1349,  497,   59,  336,  500,  501,  502,
  503,  504, 1358,   46,  500,  508,  509,   40,   40, 1365,
  336,  514,  515,  516,  362,   41,   42,   43,   44,   45,
   46,   47,  372,  376,   41,   42,   43,   40,   45,  315,
   47,  427,  318,   59,   60,   61,   62,  402, 1394,  325,
  376,  327, 1398,  393,   41,  331,  376,   41,   59,  399,
  336,   44,  905,  339,  907,   41,  909,  910, 1414,  912,
  913,  914,  298,  301,   44,   91,  434,   93,  354,   42,
   43,   44,   45,  304,   47,  452,   58,  290,  301,   40,
  333,  455,  368,  433,   44,   41,   40,  277,   41,    0,
  376,  376,  282,  379,  380,  376,   40,  276,  277,  409,
  333,  264,  264,  389,  481,  482,  276,  484,  277,  317,
  462,  450,  403,  282,  491,  492,   44,  494,  495,  405,
  497,  443, 1478,  500,  501,  502,  503,  504,  277,  270,
   40,  508,  509,  282,   41,  450,  270,  514,  515,  516,
  452,  427,  277,  264,  427,  339,  432,  282,  428,  277,
  339,   41,  450,   41,  282,   59,   59,  305,  444,   40,
  446,   41,   40,   59,   59,    0,   41,   59,  301,  481,
  482,   41,  484,   42,   43,   44,   45,    0,   47,  491,
  492,    0,  494,  495,    0,  497, 1225,   12,  500,  501,
  502,  503,  504,  372,  281,  277,  508,  509, 1554, 1555,
  282,  289,  514,  515,  516,  277,   41,  493,  282,   44,
  282,  742, 1116,  656,  393, 1571,  880,  277,   90, 1575,
  399, 1114,  282, 1579,   59,  885, 1519,  513, 1584,  842,
 1586,  517, 1106,   44, 1111,  349,  262, 1518,  264,  265,
  379,  605, 1096,  329,  784,  271, 1107, 1042, 1580,  361,
  810,  277,  361, 1634,  433, 1497,  282, 1786, 1783,  285,
  277, 1499,   41,   42,   43,  282,   45,  583,   47, 1787,
 1814, 1699,  801,  299,    0,  301,  974, 1417,  304,  305,
 1605, 1771,  308, 1259,  585,  730, 1482,  366,  354,  315,
  347,  317,  318,  859, 1312,  321,  851,   13,  642,  325,
  316,  327,  608,  329,  277,  331,  528,  524, 1664,  282,
  336,  337,  773,  339, 1248,   41,   42,   43,   44,   45,
   46,   47,   42,   43,  762,   45, 1006,   47,  354,  355,
   86,  357,  149,   59,   60,   61,   62, 1728, 1624, 1837,
  539,  452,  368, 1699,  370, 1807,  372,  373,  374, 1155,
  376,  788,  749,  379,  380,  996,   45, 1267,   -1,   -1,
 1198,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
  481,  482, 1728,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,  452,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,  277,   -1,
   -1,  481,  482,  282,  484,   -1, 1772,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,  276,  277,   -1,  514,  515,  516,   42,   43,  284,
   45,   -1,   47,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,  297,   -1,   59,  491,  492,  493,  494,  495,
  452,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   42,   43,   -1,   45,   -1,   47,  277,  481,
  482,   -1,  484,  282,   -1,   -1,  317,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  262,  372,  264,  265,
   -1,   -1,  262,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,  277,  393,  285,
   -1,   -1,  282,   -1,  399,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,  433,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,  464,  465,  466,  467,  468,
  469,  470,  471,  389,   -1,   91,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   59,   -1,  405,
  491,  492,  277,  494,  495,   -1,  497,  282,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1, 1350, 1351,  452,  277,   -1,   -1,
 1356, 1357,  282, 1359, 1360,   -1,   -1,   -1,   -1,   -1,
   -1, 1367, 1368, 1369, 1370,   -1, 1372, 1373, 1374, 1375,
 1376, 1377,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
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
   46,   47,   -1,  302,   59,   -1,   61,   -1,  354,  355,
  309,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,  322,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,   -1,
   59,   -1,   -1,  408,   -1,  410,  402,   -1,   -1,  405,
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
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,  481,  482,   44,  484,   -1,
   -1,   -1,   -1,  452,   -1,  491,  492,  493,  494,  495,
   -1,  497,   59,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,  258,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   45,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,  405,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
  277,   -1,   -1,  428,   -1,  421,   -1,  284,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
  297,   -1,   -1,    0,   -1,   -1,   -1,  443,  444,   -1,
  446,   41,   -1,   -1,   44,   -1,  452,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,  481,  482,   44,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   59,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,  372,   -1,  513,  514,  515,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,
   -1,   -1,  399,  302,   -1,   -1,   -1,   -1,   -1,   -1,
  309,   -1,   -1,   -1,  313,  314,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  322,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,  333,  271,  433,   -1,   -1,   -1,
   -1,  277,   -1,  292,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,  320,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,  395,   -1,   -1,  348,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,  411,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  426,   -1,   -1,
   -1,  430,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,  441,  379,  380,   -1,  276,  277,   -1,   -1,
   -1,   -1,   -1,  389,  284,   -1,   -1,   93,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
  419,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
  277,   -1,   -1,   -1,   -1,  421,  435,  284,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
  449,  450,  451,   -1,  453,  454,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,   -1,   -1,   -1,   -1,   -1,   -1,  487,  488,
   -1,   -1,  372,   -1,   -1,  481,  482,  496,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,  393,  500,  501,  502,  503,  504,  399,
   -1,   -1,  508,  509,   -1,  372,   -1,  513,  514,  515,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,
   -1,   -1,  399,  433,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,  433,   -1,   -1,   -1,
   -1,  362,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,  418,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,  452,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,  426,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,    0,  484,   -1,
   -1,   -1,   -1,  452,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,  284,  500,  501,  502,  503,  504,   -1,
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
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
  273,   -1,  275,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  297,  481,  482,   -1,  484,   -1,
  303,   -1,   -1,  306,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   60,   61,   62,   -1,  372,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
  393,   -1,   -1,   -1,   -1,  325,  399,   -1,   -1,  285,
  403,   -1,   -1,    0,   -1,   -1,   -1,  410,   -1,   -1,
   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,  428,   -1,   -1,   -1,  315,
  433,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   41,  331,   43,   44,   45,   -1,
  336,  337,   -1,  339,   -1,  458,   -1,   -1,   -1,  462,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,  435,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,  452,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,  271,   -1,  508,  509,
   -1,   -1,  277,   -1,  514,  515,  516,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,  321,  513,  514,  515,
  516,  517,   -1,   -1,  329,   -1,   42,   43,   -1,   45,
   -1,   47,  337,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,
  355,   -1,  357,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,  271,  370,   -1,  372,  373,  374,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,  421,   -1,  325,   -1,
  327,   -1,  329,   41,  331,   43,   44,   45,   -1,  336,
  337,   -1,  339,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,  389,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  402,  494,  495,  405,  497,
   -1,  452,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,  421,   -1,  514,  515,  516,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,  446,
  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  271,   -1,  508,  509,   -1,
   -1,  277,   -1,  514,  515,  516,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,  321,  513,  514,  515,  516,
  517,   -1,   -1,  329,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  337,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  261,   -1,   -1,   -1,   -1,   -1,  355,
   -1,  357,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,  279,   -1,  271,  370,   -1,  372,  373,  374,  287,
   -1,   -1,   -1,   -1,   -1,   -1,  294,  285,  296,   -1,
  298,    0,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,
   -1,  299,  310,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,  324,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,  421,  334,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,  288,  357,
   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,  397,
   -1,  389,   -1,   -1,   -1,   -1,  404,   -1,   -1,  407,
   -1,  409,  410,   -1,  402,  335,   -1,  405,   -1,   -1,
   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,  349,
   -1,   -1,   -1,  421,   -1,   -1,  434,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,  367,   -1,  369,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,
   -1,   -1,   -1,  403,   -1,  483,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,  415,  484,   -1,   -1,   -1,
  498,  499,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,  437,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  489,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  415,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,  437,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  489,   -1,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,  299,
   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   59,
   60,   61,   62,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  402,  494,  495,  405,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
    0,  421,   -1,  514,  515,  516,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
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
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  405,   -1,   40,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  421,   -1,   -1,   -1,   -1,   59,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   91,   -1,  318,   -1,
   -1,   -1,   -1,  493,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   -1,  262,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,
  271,   -1,   -1,   -1,  354,   -1,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,   -1,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,
  321,    0,   -1,   -1,  325,  405,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  432,  354,  355,   -1,  357,   -1,   -1,   -1,
   -1,   -1,   41,   -1,  444,   44,  446,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  405,   -1,   -1,  262,   -1,   -1,
   59,   -1,   -1,  493,   -1,   -1,  271,   -1,   -1,   -1,
  421,   -1,  277,   -1,   -1,   -1,  427,  282,   -1,   -1,
  285,  432,  315,  513,   -1,  318,   -1,  517,   -1,   -1,
   -1,   -1,  325,  444,  327,  446,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,  354,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,  368,   -1,   -1,   -1,   -1,
   -1,   -1,  493,  376,   -1,   -1,   -1,  380,   -1,  354,
  355,   -1,  357,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,  513,  368,   -1,  370,  517,  372,  373,  374,
   -1,  376,  405,   -1,  379,  380,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  432,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,  421,   -1,   41,  452,
   -1,   44,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,  262,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,    0,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,   -1,  315,   -1,  493,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,  513,   -1,
  339,   -1,  517,   -1,   -1,   41,  315,   -1,   44,  318,
   -1,   -1,   -1,   -1,   -1,  354,  325,   -1,  327,   -1,
   -1,   -1,  331,   59,   -1,   -1,   -1,  336,   -1,  368,
  339,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,
   -1,  380,   -1,    0,   -1,  354,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,  405,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  432,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,    0,  513,  514,  515,  516,  517,   -1,
   -1,   -1,  315,   -1,  493,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,    0,   -1,  336,  513,   -1,  339,   -1,  517,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,  354,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   41,  376,   -1,   44,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,    0,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,   -1,  427,  331,   -1,   -1,   -1,  432,
  336,   -1,   -1,  339,   -1,  262,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   41,  354,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,   -1,   59,   -1,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,  315,   -1,
  493,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,  405,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
  513,   -1,  339,   -1,  517,   -1,   -1,   -1,   -1,   -1,
   -1,  427,    0,   -1,   -1,   -1,  432,  354,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
    0,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   41,   -1,  262,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   41,   -1,  262,   44,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,  513,   -1,   59,
   -1,  517,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,  354,  327,   -1,
   -1,   -1,  331,   -1,   -1,   -1,  493,  336,  262,   -1,
  339,  368,   -1,    0,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,  354,  513,   -1,   -1,   -1,
  517,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  405,   -1,
  379,  380,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
  389,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,
  427,  325,   59,  327,   -1,  432,  405,  331,   -1,   -1,
   -1,   -1,  336,   -1,   -1,  339,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,  427,   -1,
  354,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,  444,   -1,  446,   -1,   -1,
   -1,   -1,  376,   -1,  262,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,  493,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  405,  262,   -1,   -1,   59,  513,   -1,   -1,   -1,
  517,   -1,   -1,   -1,  493,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,  315,  432,   -1,
  318,   -1,   -1,   -1,  513,   -1,   -1,  325,  517,  327,
  444,   -1,  446,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,  315,   -1,   -1,  318,   41,
   -1,   -1,   44,   -1,   -1,  325,  354,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   59,   -1,  339,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  493,
    0,  379,  380,   -1,  354,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,  368,  513,
   -1,   -1,   -1,  517,   -1,   -1,  376,  405,   -1,  379,
  380,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,  389,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,  405,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,  444,   -1,  446,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   41,   -1,  493,   44,   -1,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,
   -1,   59,    0,   -1,   -1,  513,   -1,   -1,   -1,  517,
   -1,  368,   -1,  493,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,  513,   -1,   -1,   -1,  517,   -1,   -1,
   -1,  315,   -1,   41,  318,   -1,   44,   -1,  405,   -1,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
  262,   59,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
  354,   -1,   -1,   -1,   -1,   -1,    0,  444,   -1,  446,
   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  389,  318,   -1,   -1,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,   41,   -1,  331,
   44,  405,  262,   -1,  336,   -1,  493,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,    0,
   -1,   -1,  354,  427,   -1,   -1,  513,   -1,  432,   -1,
  517,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,
  444,   -1,  446,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,
   41,   -1,   -1,   44,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,  405,  262,   -1,  336,   -1,   59,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,
   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,  513,
   -1,   -1,  444,  517,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   -1,   -1,   -1,  262,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,  513,   -1,   -1,  444,  517,  446,  315,  376,   -1,
  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,  389,   -1,  331,   -1,   -1,   -1,   40,  336,   -1,
   43,  339,   45,   -1,   -1,   -1,   -1,  405,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  354,   -1,   -1,   -1,
   63,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,  427,
  368,   -1,   -1,   -1,  432,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,   -1,  318,   -1,   -1,  405,   -1,   -1,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
   -1,   -1,  336,   -1,   -1,  339,   -1,  272,   -1,  427,
  275,   -1,   -1,   -1,  432,  493,   -1,   -1,   -1,   -1,
  354,   -1,   -1,  288,   -1,   -1,  444,  292,  446,  260,
  295,   -1,   -1,   -1,  368,  513,   -1,   -1,   -1,  517,
   -1,   -1,  376,   -1,  315,  379,  380,  318,   -1,   -1,
   -1,   -1,   -1,   -1,  325,  389,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,   -1,  336,   -1,  298,  339,   -1,
  335,  405,   -1,   -1,   -1,  493,  341,  342,  343,  310,
  345,  346,  347,  354,  349,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,  513,   -1,  368,  432,  517,
   -1,   -1,  367,  334,  369,  376,   -1,   -1,   -1,  380,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
  415,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,
   -1,  432,   40,  286,   -1,   43,   -1,   45,  399,  292,
  293,   -1,  437,  444,   -1,  446,   -1,   -1,  409,  513,
   58,   -1,   -1,  517,   -1,   63,   -1,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,  434,   -1,   -1,   -1,  472,  473,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,  493,   -1,  489,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,  513,   -1,   -1,   -1,  517,   -1,   -1,  372,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,  409,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  505,  506,  507,  508,  509,   -1,   -1,  267,
   -1,  514,  515,  516,   40,   -1,  274,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,  289,   58,   -1,  292,  293,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,  272,
   -1,   -1,  275,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,  290,   -1,  292,
   -1,   -1,  295,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,  298,   -1,   -1,  425,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,  310,  435,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
  324,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
  334,   -1,   -1,   -1,  367,  463,  369,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,  398,  494,  495,  496,  497,
   -1,  267,  500,  501,  502,  503,  504,   -1,  274,   -1,
  508,  509,  415,   -1,   -1,   -1,  514,  515,  516,   40,
  286,   -1,   43,   -1,   45,  399,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,  437,  409,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,   -1,  348,   -1,  350,  351,  489,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  372,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,  267,   -1,  514,  515,
  516,   40,   -1,  274,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   58,
   -1,  292,  293,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   -1,  272,   -1,   -1,  275,
   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  292,   -1,   -1,  295,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  435,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  367,  463,  369,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,  398,  494,  495,  496,  497,   -1,  267,  500,
  501,  502,  503,  504,   -1,  274,   -1,  508,  509,  415,
   -1,   -1,   -1,  514,  515,  516,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,  437,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,  489,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,  260,   -1,
   -1,  263,   -1,  372,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,  391,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
   -1,   -1,   -1,   -1,   40,  424,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  443,   -1,  320,   63,   -1,  323,
   -1,   -1,   -1,  452,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,  360,   -1,   -1,  367,
  364,  369,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  514,  515,  516,   -1,   -1,
  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,  415,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   58,
   -1,  425,   -1,   -1,   63,   -1,   -1,   -1,   -1,  437,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  260,   -1,   -1,  263,   -1,   -1,
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
   -1,   -1,  508,  509,  270,   -1,   -1,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,  298,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,  309,  310,   -1,  312,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,  463,   -1,   -1,   -1,  334,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  260,
   -1,   -1,  263,   -1,   -1,  391,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  409,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   40,  424,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  320,
   63,   -1,  323,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,  360,
   -1,   -1,   -1,  364,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   58,   -1,  425,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,    0,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   91,   -1,
   -1,  364,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,
   58,   -1,  425,   -1,  320,   63,   -1,  323,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,  258,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
  273,   -1,  275,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,  297,   -1,   -1,  463,   -1,   -1,
  303,   -1,   -1,  306,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,  348,   -1,   -1,  514,  515,
  516,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,  372,
   40,   -1,   42,   43,   -1,   45,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,
  393,   -1,   -1,   63,   -1,   -1,  399,   -1,   -1,   -1,
  403,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,  458,  364,   -1,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
  260,   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,
  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
  300,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   40,  260,   42,   43,
   -1,   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,
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
   -1,   -1,  425,   -1,   -1,  325,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,  267,  487,  488,   -1,  490,  491,  492,
  274,  494,  495,  496,  497,  395,   -1,  500,  501,  502,
  503,  504,  286,   -1,   -1,  508,  509,   -1,  292,  293,
   -1,  514,  515,  516,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  430,   -1,   58,   -1,   -1,  435,  320,   63,   -1,  323,
   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   40,   41,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
  267,  487,  488,   -1,  490,  491,  492,  274,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  286,
   -1,   -1,  508,  509,   -1,  292,  293,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,  267,  487,  488,   -1,
  490,  491,  492,  274,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  286,   -1,   -1,  508,  509,
   -1,  292,  293,   -1,  514,  515,  516,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  320,
   63,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   58,   59,  425,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   40,   -1,   42,   43,   -1,   45,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  446,   -1,   -1,  449,  450,  451,  452,
  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
  267,  487,  488,   -1,  490,  491,  492,  274,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  286,
   -1,   -1,  508,  509,   -1,  292,  293,   -1,  514,  515,
  516,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,  320,   63,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   58,   -1,  425,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,  267,  487,  488,   -1,  490,  491,
  492,  274,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  286,   -1,   41,  508,  509,   44,  292,
  293,   -1,  514,  515,  516,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   91,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,  360,   61,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,    0,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   41,   -1,   -1,
   44,   -1,  425,   -1,   91,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   59,   -1,   61,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  258,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,  273,   -1,  275,
  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,
  306,   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  273,   -1,   -1,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  258,  348,   -1,  297,   -1,   -1,   -1,   -1,   -1,
  303,   -1,   -1,  306,   -1,   -1,  273,   -1,   -1,  276,
  277,   -1,   -1,   -1,  281,   -1,  372,  284,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  297,   -1,   -1,   -1,  258,   -1,  303,  393,   -1,  306,
   -1,   -1,   -1,  399,   -1,  348,   -1,  403,   -1,  273,
   -1,   -1,  276,  277,  410,   -1,   -1,  281,   -1,   -1,
  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,  428,  297,   -1,  270,   -1,  433,   -1,  303,
   -1,  348,  306,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  393,   -1,   -1,   -1,   -1,   -1,  399,   -1,   -1,   -1,
  403,   -1,  458,  298,   -1,  372,  462,  410,   -1,   -1,
  305,   -1,   -1,   -1,  309,  310,   -1,  312,   -1,   -1,
  315,   -1,   -1,   -1,  348,  428,  393,   -1,   -1,   -1,
  433,   -1,  399,  328,   -1,   -1,  403,   -1,   -1,  334,
   -1,   -1,   -1,  410,   -1,   -1,   -1,  272,  372,   -1,
  275,   -1,   -1,   -1,   -1,  458,  281,   -1,   -1,  462,
   -1,  428,   -1,   -1,   -1,   -1,  433,  292,   -1,  393,
  295,   -1,   -1,   -1,   -1,  399,   -1,   -1,   -1,  403,
   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,   -1,
   -1,  458,   -1,   -1,   -1,  462,  391,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,  433,
  335,   -1,   -1,   -1,  409,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,  424,
   -1,   -1,   -1,   -1,  458,   -1,   -1,   -1,  462,  434,
   -1,   -1,  367,   -1,  369,   -1,   -1,   -1,  443,   -1,
  445,   -1,   -1,   -1,   -1,  272,   -1,  452,  275,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   -1,  292,   -1,   -1,  295,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
  415,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,  437,  508,  509,  510,  511,  512,  335,  514,
  515,  516,   -1,   -1,  341,  342,  343,  452,  345,  346,
  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,
  367,   -1,  369,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,  489,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  398,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  437,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,  489,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  452,   -1,   -1,   -1,  514,  515,  516,
   -1,   -1,  461,  462,  463,  464,  465,  466,  467,  468,
  469,  470,  471,  472,  473,  474,  475,  476,   -1,   -1,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,   -1,  491,  492,  493,  494,  495,  496,  497,  498,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,  513,  514,  515,  516,  517,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  462,
  463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
  473,  474,  475,  476,   -1,   -1,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,   -1,  491,  492,
  493,  494,  495,  496,  497,  498,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,
  513,  514,  515,  516,  517,
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
case 806:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 808:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 810:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 834:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 835:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 836:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 837:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 838:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 839:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 840:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 842:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 843:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 844:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 845:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 846:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 847:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 848:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 849:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 850:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 851:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 852:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
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
case 857:
{ yyval = make_list (yyvsp[-1]); }
break;
case 858:
{ yyval = yyvsp[-1]; }
break;
case 868:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 869:
{ yyval = yyvsp[0]; }
break;
case 870:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 871:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 872:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 873:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 874:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 875:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 876:
{ yyval = yyvsp[-1]; }
break;
case 877:
{ yyval = yyvsp[-1]; }
break;
case 881:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 882:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 883:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 886:
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
case 887:
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
case 888:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 889:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 891:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 893:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 894:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 895:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 896:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 897:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 898:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 900:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 901:
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
case 902:
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
case 903:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 904:
{ yyval = make_parameter (); }
break;
case 905:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 906:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 907:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 908:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 909:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 910:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 911:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 912:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 913:
{ yyval = yyvsp[0]; }
break;
case 914:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 916:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 917:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 918:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 919:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 920:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 922:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 923:
{ yyval = yyvsp[0];}
break;
case 928:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 929:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 930:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 932:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 933:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 935:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 936:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 938:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 939:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 941:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 943:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 944:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 945:
{ yyval = yyvsp[0]; }
break;
case 946:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 947:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 949:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 952:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 953:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 954:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 957:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 958:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 959:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 960:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 961:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 962:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 963:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 964:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 968:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 969:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 970:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 971:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 972:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 974:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 980:
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
