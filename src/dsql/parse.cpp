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
#define EQUIV 518
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
  392,  392,  392,  392,  397,  397,  397,  397,  397,  397,
  397,  397,  397,  397,  397,  397,  397,  397,  397,  397,
  402,  402,  393,  393,  394,  394,  394,  394,  395,  395,
  399,  399,  400,  400,  400,  400,  398,  401,  396,  396,
  387,  387,  387,  390,  390,  390,  403,  403,  404,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,  150,  150,  150,  405,
  211,  211,  145,  145,  411,  411,  411,  411,  411,  407,
  407,  407,  407,  407,  225,  146,  146,  147,  148,  148,
  148,  148,  148,   52,   52,   21,   21,  235,  412,   55,
  273,  266,  266,   99,  406,  406,  406,  406,  413,  413,
  413,  413,  413,  413,  413,  413,  413,  413,  413,  414,
  415,  417,  416,  416,  419,  420,  420,  410,  410,  408,
  409,  409,  421,  421,  421,  422,  422,  423,  423,  426,
  426,  424,  424,  429,  429,  428,  425,  427,  418,  418,
  418,  418,  418,  418,  418,  418,  333,  333,  317,  317,
  149,   49,   65,  124,  142,  386,  159,  183,  259,   67,
   63,  217,  116,   70,  337,  213,   36,   42,  346,  258,
   40,   43,  210,   41,  286,  253,  253,  430,  430,  430,
  430,  430,  430,  430,  430,  430,  430,  430,  430,  430,
  430,  430,  430,  430,  430,
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
    3,    3,    3,    3,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    1,    1,    5,    6,    3,    4,    5,    6,    3,    4,
    3,    4,    3,    4,    4,    5,    4,    4,    3,    4,
    1,    1,    1,    1,    1,    1,    1,    3,    3,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    3,    3,    3,    3,    3,    3,    3,    3,    1,    1,
    1,    1,    3,    1,    1,    1,    1,    1,    1,    4,
    1,    3,    1,    2,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    1,    2,    1,    1,    1,
    1,    1,    2,    1,    1,    1,    1,    1,    4,    5,
    5,    5,    5,    5,    5,    5,    5,    5,    5,    6,
    1,    6,    1,    4,    7,    2,    0,    4,    3,    6,
    1,    1,    6,    8,    6,    1,    1,    4,    6,    4,
    5,    3,    5,    4,    5,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    0,    1,    0,
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
    0,  555,  556,  557,  561,  562,  563,    0,  627,  630,
  742,  743,  747,  748,  468,    0,    0,    0,    0,    0,
    0,  400,  573,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  122,    0,    0,  103,    0,  918,    0,
   22,  916,    0,    0,   74,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  481,    0,
    0,    0,   38,   35,   37,    0,   36,    0, 1006, 1008,
 1010, 1009, 1015, 1011, 1016, 1012, 1014, 1017, 1013, 1018,
 1019, 1020, 1022, 1023, 1021, 1024, 1025,    0,    0,    0,
    0,   33,   69,   67,  998, 1007,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  638,
    0,    0,    0,  685, 1000,    0,    0,    0,    0,    0,
  115, 1005,  564,    0,    2,    0,    0,    0,    0,    0,
    0,  430,  431,  432,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  456,  450,  451,  452,  453,  454,  455,  457,    0,
  428,  429,    0,  990,    0,  407,  994,    0,  404,  997,
    0,  687,    0,  403, 1001,    0,  571,  914,    0,  158,
  111,    0,  112,    0,  208,  986,  779,    0,  110,  153,
  993,    0,    0,  106,    0,  113,  920,    0,  107,    0,
  108,    0,  109, 1004,  154,  114,  730,  731,    0,  919,
  917,    0,   75,    0,  991,    0,  989,  489,  482,    0,
  488,  493,  483,  484,  491,  485,  486,  487,  492,    0,
    0,    0,   31,    0,   40,  766,   39,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  924,    0,  905,  632,  632,  633,  643,  645,
  977,  646,    0,  647,  637,  633,    0,    0,    0,    0,
    0,    0,    0,  590,  579,    0,  582,  584,  585,  586,
  587,  593,  594,  999,  686,    0,    0,  116,    0,  117,
    0,  118,    0,    0,    0,  617,    0,    0,  979,    0,
    0,  624,    0,    0,    0,    0,    0,  469,    0,    0,
  370,    0,  211,    0,  410,  401,  466,  467,    0,    0,
    0,    0,    0,    0,  419,    0,  376,  377,    0,  577,
  575,  915,    0,    0,    0,    0,    0,    0,    0,  161,
  140,    0,  104,    0,    0,  119,    0,  129,  128,    0,
    0,    0,    0,    0,    0,  982,   76,    0,    0,    0,
  744,  745,  490,    0,    0,    0,    0,  882,  911,    0,
  981,    0,  884,    0,    0,  912,    0,  906,  897,  896,
  895,    0,    0,    0,    0,  887,  888,  889,  898,  899,
  907,  908,    0,  909,  910,    0,    0,    0,  913,    0,
    0,    0,    0,    0,    0,  901,    0,  776,  879,  880,
  881,  886,  885,  359,  865,    0,    0,  864,    0,    0,
  861,  862,  863,  866,  867,  868,  900,  925,  926,  927,
  928,  941,  943,  951,  952,  956,  957,  209,    0,    0,
    0,  363,    0,    0,    0,  769,    0,    0,   28,    0,
    0,    0,   72,   70, 1002,   68,   34,    0,    0,    0,
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
  102,    0,  101,  983,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  782,  784,  793,  796,  797,  798,
  799,  800,  801,  802,  803,  804,  805,    0,    0,    0,
  902,    0,    0,    0,    0,    0,    0,  903,  904,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  324, 1003,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  267,  504,    0,    0,    0,
  361,    0,    0,    0,  768,   29,    0,    0,   45,   73,
    0,    0,   27,    0,  774,    0,    0,  741,    0,    0,
    0,    0,    0,  568,    0,  632,    0,  654,    0,    0,
    0,    0,  559,    0,  922,  558,    0,  592,    0,  600,
  596,  595,    0,    0,    0,  611,  599,    0,    0,  749,
  750,    0,    0,    0,  368,    0,    0,    0,    0,    0,
  138,    0,  473,  370,  413,    0,  370,  371,  418,    0,
    0,  258,  368,  231,  987,    0,    0,  370,    0,    0,
  200,  368,    0,    0,    0,    0,  236,  230,  233,  234,
  235,  422,  464,  463,  421,  420,    0,    0,    0,    0,
    0,    0,  379,    0,  370,    0,  163,  167,    0,    0,
  171,    0,    0,  761,    0,  535,  536,  552,    0,  549,
    0,    0,  542,  540,  541,    0,    0,    0,    0,  505,
    0,    0,    0,    0,  543,    0,    0,  368,    0,  191,
    0,  194,  196,  197,    0,    0,    0,    0,    0,  211,
   79,    0,    0,   78,    0,    0,    0,    0,  783,  795,
    0,    0,  854,  855,  856,    0,  787,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  969,  970,  971,  972,  973,  974,  975,  976,    0,
    0,    0,    0,    0,    0,  962,    0,    0,    0,    0,
  877,  326,    0,  878,  949,    0,  873,  985,    0,    0,
    0,    0,    0,    0,    0,  883,  778,  777,  365,  364,
    0,  265,    0,    0,    0,  273,  770,    0,    0,    0,
    0,    0,    0,    0,    0,   54,   62,   64,    0,    0,
   71,    0,    0,    0,  773,    0,    0,    0,    0,    0,
    0,    0,  641,  644,  995,  653,    0,    0,    0,    0,
  659,  660,  661,  662,  671,  672,  673,    0,    0,  651,
  560,  923,  601,  610,  605,  604,    0,  615,  781,  752,
    0,  368,    0,    0,  634,  729,  728,    0,  472,    0,
    0,  175,    0,  179,    0,    0,    0,  412,  368,  368,
  270,  371,  263,    0,  202,  370,  204,    0,  371,  199,
  205,    0,    0,    0,    0,  425,  426,  370,  368,  371,
    0,    0,    0,  397,    0,  187,  188,  164,    0,  174,
  523,  524,  921,    0,    0,    0,    0,    0,  550,    0,
    0,  370,  371,    0,  500,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  189,    0,
    0,    0,   83,    0,    0,    0,    0,   91,   85,    0,
    0,   90,   86,    0,  746,  988,    0,    0,  788,    0,
    0,  794,    0,    0,    0,    0,    0,  832,  831,    0,
    0,    0,  839,  857,  849,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  786,    0,    0,    0,
  211,    0,  929,    0,    0,  944,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  958,    0,
    0,    0,    0,    0,    0,    0,  626,  948,  890,    0,
  260,  279,    0,    0,  360,  274,   66,   58,   59,   65,
   60,   61,    0,    0,   23,    0,    0,    0,   43,    0,
    0,    0,  755,  775,    0,  754,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  688,    0,    0,
    0,    0,    0,    0,    0,  607,  608,  612,    0,  190,
    0,    0,    0,    0,    0,  180,    0,    0,    0,  408,
  214,  215,  216,  217,  218,  219,  893,    0,    0,  264,
    0,    0,  212,    0,    0,    0,    0,  237,    0,  460,
  461,  427,    0,    0,    0,    0,    0,    0,  370,  479,
  371,  173,  984,    0,    0,  518,    0,  517,  553,    0,
  545,    0,    0,    0,  498,    0,  526,    0,    0,  528,
  531,  533,  368,    0,    0,    0,  127,    0,  195,  370,
    0,  369,  105,    0,  125,    0,   94,    0,    0,    0,
    0,   88,   89,    0,  847,  848,  790,    0,  792,    0,
    0,    0,    0,    0,    0,    0,  850,    0,    0,    0,
    0,    0,    0,    0,  840,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  935,
  934,    0,    0,  931,  930,    0,  939,  938,  937,  936,
  933,  932,    0,    0,  964,    0,    0,    0,  963,    0,
    0,    0,    0,  266,    0,    0,  276,  277,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  358,    0,    0,    0,    0,  285,  290,  284,    0,
    0,    0,  294,  295,  296,  297,  298,  299,  301,  302,
  304,  305,  306,  309,    0,    0,    0,  352,  353,  354,
    0,    0,   57,   63,   55,   56,   24,  760,  759,    0,
  757,  765,  764,  762,  740,    0,    0,    0,  663,    0,
    0,    0,  693,  691,  689,    0,  690,    0,    0,    0,
    0,    0,    0,  369,  733,  734,  635,    0,  185,  181,
    0,  371,  894,  268,  269,  371,    0,    0,    0,  222,
    0,    0,  238,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  476,  520,    0,  521,    0,  142,    0,    0,
    0,  146,  148,  149,  150,  501,  503,  499,  530,    0,
    0,  371,    0,    0,    0,  366,  370,   92,    0,    0,
   98,   93,    0,    0,    0,    0,    0,  858,    0,  859,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  210,  950,
  940,  942,    0,    0,  960,  959,    0,  965,  953,  955,
    0,    0,  275,    0,  313,    0,    0,    0,  308,    0,
    0,  307,  338,  339,    0,  996,  355,  356,    0,  293,
  300,    0,  303,  286,    0,  291,    0,  343,  292,  310,
    0,    0,  336,   41,    0,    0,  683,  681,    0,  674,
    0,    0,    0,  697,    0,    0,  631,  371,  367,  735,
  182,  371,  152,  257,    0,    0,  198,  223,    0,    0,
    0,    0,  224,  227,    0,    0,  371,  395,  396,  392,
  391,  394,  393,    0,  519,  546,  151,  139,  147,    0,
  370,  254,  134,  371,  372,    0,    0,   96,    0,   80,
    0,  818,  826,  625,  819,  827,  822,  830,    0,    0,
  820,  828,  821,  829,  815,  823,  816,  824,  817,  825,
    0,  945,  961,    0,    0,  282,  278,    0,  311,    0,
    0,    0,    0,    0,  340,    0,    0,  331,  332,    0,
  351,    0,    0,    0,    0,  346,  344,    0,    0,    0,
  756,    0,  665,  675,    0,    0,  676,  677,  678,    0,
  707,  709,    0,  704,    0,  255,  409,  201,  206,  225,
    0,    0,  228,    0,  240,  242,  399,  371,    0,  374,
    0,   77,   99,  100,  954,  281,    0,  312,    0,    0,
  315,    0,  321,    0,  323,    0,    0,  350,  349,  992,
  348,    0,    0,    0,    0,    0,    0,  664,    0,    0,
  698,  708,    0,  229,    0,    0,    0,    0,    0,  256,
  371,    0,    0,    0,    0,    0,    0,    0,  334,  333,
  345,  347,    0,    0,    0,  669,    0,    0,    0,  715,
    0,    0,    0,    0,    0,    0,  239,    0,  246,    0,
  245,  375,  126,  283,    0,    0,  314,  318,    0,    0,
    0,    0,    0,    0,  667,  680,  717,  706,    0,    0,
  718,    0,  714,  226,    0,    0,  250,  249,  248,    0,
    0,    0,    0,    0,  335,  670,  713,    0,    0,  253,
  251,  252,  329,  319,    0,  341,  316,    0,    0,    0,
  720,  317,    0,  719,    0,  722,    0,  723,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1423,   28,   29, 1424,
   31,   32,   33,   34,   35,   36,   37, 1425,   39, 1426,
  844,  128,  488,  984, 1225,  129,  738,  985,  130,  492,
  743,  489,  131,  132,  275, 1008,  144,  986,  987,  213,
  242,  133,  988,  134,  494,   85,  253,  397,  445,  657,
 1116,  446,  883,  628,  235,  860,  658,  659,  660, 1123,
 1118, 1542,  254,  662,  663,   77,  203,   78,  249, 1868,
 1333,  234,  239,  241,  243,  229,  221,  223,  236,  246,
  161,  328,  330,  332,  386,  644,  646,  485,  276,  807,
  565,  673, 1048,  390,  649, 1107, 1327,  596,  765, 1078,
  557,  558,  578,  382,  629, 1093, 1520, 1637, 1270, 1521,
 1522, 1523, 1524, 1525,  674,  230,  222,  378,  604,  379,
  380,  605,  606, 1302,  559, 1042, 1043, 1044, 1265,  392,
  871,  872,  873,  874,  809, 1059, 1498,  630,  811,  448,
  876,  957,  479, 1383, 1271,  449,  450,  451,  452,  453,
 1499, 1500,  581, 1643, 1787, 1288,  817,  818,  804,  819,
  820,  821, 1788, 1789, 1848,  360,  484,  974, 1427,  209,
  574,  971,  575, 1052,  480,  975,  976, 1213, 1406, 1407,
 1408, 1697, 1125, 1428, 1429, 1430, 1431, 1607, 1432, 1433,
 1434, 1435, 1436, 1437, 1438, 1439, 1440, 1441, 1442, 1443,
 1444, 1445,  454, 1795, 1446, 1707, 1832, 1704,  455,  714,
  456,  457, 1447, 1608, 1715, 1716, 1769, 1448, 1449, 1450,
 1599,  272,  481,  482,  458,   41, 1332, 1536,  369,  592,
  835, 1300,  593,  833,   82,   62,  364,  214,  171,  347,
  200,  354,  206,  798,  795,  355,  365,  825,  366,  588,
 1070,  201,  459,  348,  594, 1301,   99,  212,  258,  631,
  632,  727, 1094,  728, 1095, 1096,  635,  636,  637,  638,
 1459, 1087, 1085,  639,  640,  641,  853,  642,  850,   42,
   43,   44,   45,   46,   47,  163,  545,   64,  510,  217,
  371,  315,  316,  317,  318,  319,  320,  321,  322,  323,
  771,  531, 1027, 1258,  532,  533,  776,  167,  336,  547,
   48,  170,  342, 1356,  713,   49,  340,   50,  148,  303,
  518,  761,  401, 1255, 1482, 1627,  513,  305, 1262, 1630,
  149,  150,  304,  519,  520,  759, 1006, 1009, 1010, 1011,
 1012, 1013, 1014, 1722, 1778,  460, 1807, 1015, 1016, 1017,
 1253, 1727, 1728, 1729, 1245,  157, 1474, 1623, 1624, 1810,
 1735, 1811, 1812, 1813, 1843, 1869, 1871,  550,  551, 1036,
 1487,  500,   51,   52,  402,   53,   54,  539,  540,  993,
 1233, 1460, 1464,  501,  746,  227,  675,  676,  898,  899,
  677,  678,  679,  680,  681,  682,  683,  684,  685,  686,
  687, 1141, 1143, 1144,  461,  462,  463,  464,  465,  466,
  467,  251,  468,  469,  470,  471,  472,  940,  473, 1574,
  474,  475,  476,  477,  703,  944, 1202, 1197,  704,  136,
};
static short yysindex[] = {                                   4919,
 1597, -147, 3476,   72, 1361,   14, 4430,  -28, 8234,   26,
   81, 8880, -147, 1084,  799, 6493, 1142, 6493, -138,    0,
  322,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   84,    0,    0,    0,    0,    0,    0,  323,    0,    0,
    0,    0,    0,    0,    0,14380, 6493, 6493, 6493, 6493,
 6493,    0,    0,  103, 1272, 6493, 6493, 6493,  403, 6493,
  218, 6493, 6493,    0, 6493, 6493,    0, 1152,    0,  282,
    0,    0,  517, 6493,    0, 6493, 6493, 6493,  526, 6493,
 6493, 6493, 6493,  218, 6493, 6493, 6493, 6493,    0, 6493,
  975,  530,    0,    0,    0,  987,    0,  987,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  557,  659,  144,
 1008,    0,    0,    0,    0,    0, 6493, 6493, 6493,  751,
  782,  805,  148,  584,  810,  166,  168,   58,  764,    0,
 6493,  955, 1234,    0,    0, 6493,  859, 6493, 6493, 6493,
    0,    0,    0, 6493,    0,  877,  929, 1108, 1005,  816,
 1508,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  389,
    0,    0, 1272,    0,   78,    0,    0, 1363,    0,    0,
 1348,    0,  638,    0,    0, 1063,    0,    0,  971,    0,
    0, 1276,    0, 1233,    0,    0,    0, 1272,    0,    0,
    0,  802, 1363,    0,   44,    0,    0, 1206,    0, 1212,
    0,  987,    0,    0,    0,    0,    0,    0, 1252,    0,
    0, 1093,    0, 1217,    0, 1158,    0,    0,    0, 1093,
    0,    0,    0,    0,    0,    0,    0,    0,    0,13285,
 6493, 1232,    0, 6493,    0,    0,    0, 4667, 1224, 4984,
 6493,  957, 1346, 1657, 1392, 1386, 4667, 1326, 4984, 1357,
 1368, 1257,    0,13392,    0,    0,    0,    0,    0,    0,
    0,    0,11640,    0,    0,    0, 1323, 6493, 1409, 1334,
  590, 6493, 1360,    0,    0, 1234,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 6493, 1363,    0, 1206,    0,
  987,    0, 1417, 1463, 1349,    0,13392, 1438,    0, 1438,
13392,    0,  -95, 1344, 1358, 1364, 1508,    0, 1584,  729,
    0, 6493,    0,  389,    0,    0,    0,    0, 6493, 1232,
 3767, 1412, 4285, 1832,    0,14380,    0,    0, 1262,    0,
    0,    0, 1818, 1818, 1272, 1512, 1272,  402, 1276,    0,
    0, 8752,    0, 6493, 6493,    0, 1232,    0,    0, 1601,
  770, 1840, 6493, 1625, 6493,    0,    0, 1845, 2744, 8756,
    0,    0,    0, 1851, 1853, 1854, 1443,    0,    0, 1858,
    0, 1860,    0, 1861, 1867,    0, 1868,    0,    0,    0,
    0,    0, 1869, 1469, 1474,    0,    0,    0,    0,    0,
    0,    0, 1894,    0,    0,11898, 1903, 1904,    0, 1907,
13392,13392,12117, 6493, 1908,    0, 2244,    0,    0,    0,
    0,    0,    0,    0,    0, 1906, 1863,    0,    0, 1909,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 8752, 1890,
 1192,    0, 1923, 1688, 1290,    0, 6493, 1541,    0, 6493,
 6493,   70,    0,    0,    0,    0,    0, 6493, 6493,  344,
    0, 6493,    0, 1656, 6493, 1937, 4667, 1589,    0, 6493,
12117, 1651,    0,    0,13392,    0, 1964, 1672, 1946,    0,
    0,  152,    0,  261,    0,  652,    0,  652,    0,    0,
 1949,    0,  126, 1579,    0,    0,    0, 1955,   76, 1942,
 1232, 1970, 1748,    0,    0, 6493,    0,    0, 1794, 1969,
    0,    0,    0,  397,    0, 1670, 1674,    0, 1272,    0,
 1676,    0,    0, 1740, 1746,    0,    0,    0, 1722,    0,
 8752,    0, 8752, 1354,    0, 1760, 6493,14224,    0,    0,
  903,    0, 6493,  622, 1348, 1638, 1602, 1577,    0,    0,
    0,    0,  936, 1650,    0, 1591,  218,    0, 1272,    0,
 1768, 1701,    0,    0,  402,    0,    0,  183, 1607,    0,
    0,    0, 1608,    0, 1662, 2008,    0, 1710, 2016,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1966,  578,    0,    0,    0,    0,  100, 2020,
 2021, 2016, 1363,    0, 1747,    0, 1800,    0, 1272, 1272,
    0, 3767,  638,    0, 1687,    0, 1678, 2030,    0, 5575,
    0, 1683,    0,    0, 1702, 2038, 8988, 2040,    0,    0,
    0, 8502, 5409,   33,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1007,13392,  177,
    0, 6493,13392, 1020, 1097, 1145, 2197,    0,    0,13392,
 9233, 2244, 1640,  407,13392,13392, 9233,  784,  784, 1698,
 1643,  323, 2044,    0,    0,12224, 6493,13392,13392,13392,
13392,13392,13392,13392, 2785,    0,    0, 1811, 2024, 6493,
    0, 6493, 1795, 6493,    0,    0, 6041, 1668,    0,    0,
 1634, 4984,    0, 1786,    0, 1644, 2061,    0, 1786, 6041,
 1788, 1797, 6493,    0, 1698,    0, 1705,    0, 6493, 2485,
 1673,13392,    0,  534,    0,    0,  -54,    0, 1681,    0,
    0,    0, 6493, 1116, 6493,    0,    0, 6493, 6493,    0,
    0,13392, 1865, 3767,    0, 2086, 1637,13392,13392, 1272,
    0,  735,    0,    0,    0, 2092,    0,    0,    0,  164,
 6493,    0,    0,    0,    0, 1871, 2093,    0, 1848, 1874,
    0,    0, 1808, 1809,  987, 1876,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  218, 6493,14322, 1771,
 1777, 1781,    0,  218,    0, 1420,    0,    0, 1752, 1272,
    0, 2744,   62,    0, 1753,    0,    0,    0,  218,    0,
 2008,   74,    0,    0,    0, 1873,   74, 1754,   74,    0,
 2117,  218,  218,  218,    0, 1232, 6493,    0, 1813,    0,
 1675,    0,    0,    0, 1262, 1438, 6493, 1726,   83,    0,
    0, 2126,   15,    0, 2744, 6493, 1438, 9465,    0,    0,
 1438, 8988,    0,    0,    0, 5042,    0,  357,  481,13392,
13392, 9572, 2128,  721, 9830,13392,10088,  674,10195,10453,
12443,13392,10711,10818,11076, 9233, 9233,13392,13392, 2535,
13392, 2130,13392, 2129, 1812,13392,13392,13392,13392,13392,
13392,    0,    0,    0,    0,    0,    0,    0,    0, 1857,
 1445,  550,13392,  643,13392,    0, 9233, 1895, 2835,  210,
    0,    0,  816,    0,    0, 1728,    0,    0, 1901,  784,
  784,  732,  732, 2244,  937,    0,    0,    0,    0,    0,
 1743,    0, 1742, 1916, 1795,    0,    0, 6493, 6493, 6493,
 6493, 6493, 6493,   94, 2141,    0,    0,    0, 6041, 1810,
    0,  -58, 1826, 6493,    0,13392, 1826, 2147, 2141, 6041,
 4104, 1878,    0,    0,    0,    0, 2189, 2156, 2161,  689,
    0,    0,    0,    0,    0,    0,    0, 9233, 1885,    0,
    0,    0,    0,    0,    0,    0,  424,    0,    0,    0,
 2244,    0, 1749, 1438,    0,    0,    0, 2244,    0, 1818,
 1947,    0,  735,    0, 2176, 9233, 4400,    0,    0,    0,
    0,    0,    0, 1795,    0,    0,    0, 4400,    0,    0,
    0,  987,  987, 1729, 2183,    0,    0,    0,    0,    0,
   11,  292,  602,    0, 1971,    0,    0,    0, 6493,    0,
    0,    0,    0,   72, 1751, 1822, 1811, 2199,    0, 1591,
 1757,    0,    0, 1061,    0, 2180, 6493, 1183,  218, 2203,
 2204, 2206, 1985,  638, 1795, 1818, 1674, 3767,    0, 1650,
 1804, 1717,    0,  218, 1872, 1951, 5575,    0,    0,  218,
 -161,    0,    0, 1952,    0,    0, 2211, 2219,    0, 9233,
 9233,    0, 9233, 9233, 2730, 2244, 2221,    0,    0, 2244,
 2224,12117,    0,    0,    0, 1924, 2231, 2244, 2234, 1322,
 2238, 2244, 2239,13392,13392, 2128,13392,12701, 2242, 2244,
 2243, 2253, 2244, 2254,13392, 2244, 2244, 2257, 2244, 2260,
 2262, 2244, 2265, 2267, 2244, 2270,    0, 2022, 2096, 2475,
    0, 2489,    0, 2504,13392,    0, 2513, 2558, 2778, 3374,
 3535, 3544,13392,13392,13392, 2244, 1884,13392,    0,13392,
 2244, 2009,  645,13392,13392,13392,    0,    0,    0, 6493,
    0,    0, 6493,10328,    0,    0,    0,    0,    0,    0,
    0,    0, 1989, 6041,    0, 6041,   94, 2141,    0,  239,
  239,   49,    0,    0, 1762,    0, 2147, 2141, 2275, 4104,
 2280,  689, 2282,13392, 2060, 2485, 1944,    0, 1944, 1075,
 1944, 1987, 1990, 2058, 2006,    0,    0,    0, 1795,    0,
 1804, 1069, 1591, 1959, 1591,    0, 9233,  375, 1517,    0,
    0,    0,    0,    0,    0,    0,    0, 4400, 4400,    0,
 1916,13392,    0, 2051, 1948, 1729, 1152,    0, 9233,    0,
    0,    0, 1974, 1978, 1981, 1993, 1995, 1996,    0,    0,
    0,    0,    0, 2308,   72,    0, 1900,    0,    0,   74,
    0, 4400, 1994,   74,    0,   74,    0, 1811, 2327,    0,
    0,    0,    0, 1262, 1916, 1591,    0, 1674,    0,    0,
 2100,    0,    0, 2086,    0, 2329,    0, 2336, 1272,   60,
 2338,    0,    0, 1272,    0,    0,    0, 2022,    0, 2022,
13392, 1438, 1438, 1778,  323, 2340,    0, 1438, 1438,13392,
 1438, 1438, 3306, 2244,    0, 1520,13392, 2244, 1438, 1438,
 1438, 1438, 2244, 1438, 1438, 1438, 1438, 1438, 1438,    0,
    0, 8752, 2343,    0,    0, 3555,    0,    0,    0,    0,
    0,    0, 3562, 1395,    0,13392, 2080, 1960,    0,13392,
 3698, 1779, 3260,    0, 8752, 2330,    0,    0, 2098,    0,
 2873, -121, 2335, 2346, 2062,13392, 2341, 2345, 3173, 6493,
 6493,    0, 2347, 2348, 2063, 2349,    0,    0,    0, 2104,
 9705, 2351,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2352,  -76, 2355,    0,    0,    0,
    0, 2036,    0,    0,    0,    0,    0,    0,    0, 1988,
    0,    0,    0,    0,    0, 4104, 2275, 2060,    0, 1790,
 6493,  689,    0,    0,    0, 2076,    0, 2485, 2485,13392,
 9233, 2028, 1916,    0,    0,    0,    0, 1420,    0,    0,
  380,    0,    0,    0,    0,    0, 2244, 2143, 2051,    0,
  814, 6493,    0, 2088,  485, 1992, 2094, 1997, 2124, 2099,
 2132, 1795,    0,    0, 2391,    0, 2395,    0, 2090, 2143,
 1994,    0,    0,    0,    0,    0,    0,    0,    0, 1795,
 1650,    0, 1420, 1971, 2066,    0,    0,    0, 2074, -110,
    0,    0, 1811, 2075, 2244, 2404, 2405,    0,  816,    0,
 2406, 2407, 2244, 2408, 2410,13392,13392, 2244, 2411, 2413,
 2414, 2415, 2416, 2417, 2418, 2420, 2422, 2423,    0,    0,
    0,    0,13392, 2424,    0,    0,13392,    0,    0,    0,
13392,  343,    0, 2152,    0,12808, 6493,13392,    0, 9233,
 2244,    0,    0,    0, 2409,    0,    0,    0, 6493,    0,
    0, 2444,    0,    0,  519,    0, 2026,    0,    0,    0,
  551, 2430,    0,    0,  239, 6493,    0,    0, 2485,    0,
 1078, 2244, 2432,    0,   33,  879,    0,    0,    0,    0,
    0,    0,    0,    0, 2436, 6493,    0,    0, 2133, 2138,
 6493, 1729,    0,    0,  987, 6493,    0,    0,    0,    0,
    0,    0,    0, 1916,    0,    0,    0,    0,    0, 1916,
    0,    0,    0,    0,    0,13392, 1272,    0, 2029,    0,
 1272,    0,    0,    0,    0,    0,    0,    0, 2244, 2244,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2244,    0,    0, 3710, 4400,    0,    0, 2440,    0, 1915,
13285, 1388,  587, 2431,    0, 2160, 1264,    0,    0, 2451,
    0, 6493, 6493,   72,  146,    0,    0, 1998, 2165, 9233,
    0, 2470,    0,    0, 9233, 2472,    0,    0,    0,13392,
    0,    0, 2151,    0, 2474,    0,    0,    0,    0,    0,
 1729,  987,    0, 2157,    0,    0,    0,    0, 1971,    0,
 2244,    0,    0,    0,    0,    0, 1438,    0, 2121, 2444,
    0, 2101,    0, 2444,    0, 2444, 2567,    0,    0,    0,
    0,10328,  519,13392, 2444,  616, 6493,    0,   33, 6493,
    0,    0, 4544,    0, 2157,  454, 1729, 2159, 2162,    0,
    0, 2478, 2486, 2227, 2480, 1440,10328, 2493,    0,    0,
    0,    0, 1460,  149, 2240,    0, 1799, 1807, 6493,    0,
 2501, 2499, 1080, 1729,  592,  592,    0, 2246,    0, 2116,
    0,    0,    0,    0, 2444, 2493,    0,    0, 2248, 2444,
 2263, 2258,10328, 6493,    0,    0,    0,    0, 4544, 2521,
    0, 6493,    0,    0, 2107,   37,    0,    0,    0, 1817,
10328,  165, 6493,10328,    0,    0,    0, 6493, 2230,    0,
    0,    0,    0,    0,10328,    0,    0, 2522, 2524, 2528,
    0,    0, 6493,    0, 6493,    0, 2542,    0,
};
static short yyrindex[] = {                                      0,
    0,  224,  774,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  153,11314,    0,    0,    0,    0,    0,    0,
 2569,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  139,    0,    0,    0,    0,    0,    0,  102,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1180,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2251,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  514, 2209,    0,    0,    0,  160,    0,  160,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2210,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1216,    0,    0,13027,11421,    0,
    0,    0, 1242,    0,    0,  118,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  613,    0, 2185,  248,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  522,    0,    0,
    0,    0,  920,    0,    0, 1253,    0,    0,    0,    0,
    0,  113,    0, 8851,    0,    0,    0,    0,    0,    0,
    0,    0,  522,    0,  532,    0,    0, 2552,    0,    0,
    0, 2331,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1256,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1258,
    0, 2332,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1035,    0,    0,    0,    0,    0,    0,
    0, 6780,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  916,    0,    0, 1259,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  522,    0, 2552,    0,
 2331,    0, 1321,  175,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1345,    0, 2326,    0,
    0,    0,    0, 1415,    0,    0,    0,    0,    0, 2332,
 1028,14446,    0, 1419,    0,    0,    0,    0,  359,    0,
    0,    0, 2154, 2154,    0,    0,    0, 1436,  127,    0,
    0,    0,    0,    0,    0,    0, 2332,    0,    0, 1373,
    0,    0,    0,    0,    0,    0,    0, 5286,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1991,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2829,    0,    0,    0, 3129, 3429,    0, 3729,
    0,    0,    0,    0,    0,    0,  201,    0,    0,    0,
    0,    0,    0,    0,    0,  222, 4029,    0, 2529,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1449,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1452,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 3044,    0, 2289,    0,
    0,    0,    0,    0,    0,  492,    0,  492,    0,    0,
 1056,    0, 1144, 1187,    0,    0,    0,    0, 1256,    0,
 2332,    0,    0,    0,    0,    0,    0,    0,  361,  154,
    0,    0,    0,  701,    0,    0, 1113,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2326,    0,    0, 1476,    0, 2145,    0,    0,  656,    0,
    0,    0,    0,  856,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1494,    0,    0, 3420,10981,    0,
    0,    0,13858,    0,    0,13947,    0,    0,13990,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1791, 1870,    0,    0,    0,    0,14118,13911,
    0,13990,  522,    0,    0,    0,    0,    0,    0,    0,
    0, 1028, 1410,    0,    0,    0,    0, 2205,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 6335, 6584,
 6797,    0,    0,  576,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,13611,    0,13611,
    0,    0,    0,13611,13611,13611,    0,    0,    0,    0,
    0, 2167,    0,    0,    0,    0,    0, 5218, 5507, 2568,
    0,   31,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 4254,    0,    0,
    0,    0, 2344,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  348,    0,    0,    0,    0,  348,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  717,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  450,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1028,    0,  199,  137,    0,    0,    0,
    0,  356,    0,    0,    0,    0,    0,    0,    0, 1819,
    0,    0,    0,    0,    0, 2571,    0,    0,  678, 1905,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  675,
  714,  718,    0,    0,    0,  135,    0,    0,    0,    0,
    0,    0,    0,    0, 3720,    0,    0,    0,    0,    0,
13947,    0,    0,    0,    0,  266,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2332,    0,    0,  257,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 5864,    0,
    0,    0,  191,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2574,    0,    0,    0,    0,    0, 4329, 5796,
 6085, 4629, 4929,  285,    0,    0,    0,    0,    0,    0,
    0,    0, 8097,    0, 2353,    0,    0,    0,    0,    0,
    0,    0,    0, 1516, 1522,    0,    0,    0,    0,    0,
    0,    0, 1533,    0,    0,    0, 1533, 1542, 1559,    0,
    0,    0,    0,    0,    0,    0,    0,  846,  491,  202,
    0,    0,    0,    0,    0,    0,    0,    0,  840,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  211,    0,    0,    0,    0,    0,    0,  838,    0, 2154,
 2168,    0,  793,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2344,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1151,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1566,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 4020,    0,    0,    0,
    0,    0,    0,    0,    0, 1200,    0,    0,    0,    0,
    0,    0,    0, 1410, 2344, 2154, 1588, 1028,    0, 2357,
 1593,    0,    0,    0, 5864,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  946,    0,    0,    0, 1556,
    0,    0,    0,    0,    0,    0,    0, 7004,    0, 7064,
    0, 7139,    0,    0,    0,    0,    0,    0,    0, 7211,
    0,    0, 7286,    0,    0, 7346, 7421,    0, 7551,    0,
    0, 7582,    0,    0, 7712,    0,    0, 1150,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2198,    0,    0,    0,    0,
  823,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  646,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1516, 1598,    0, 2196,
  387,    0,    0,    0,    0,    0, 1621, 1624, 1636,    0,
    0, 2292,  857,    0, 6860,    0, 2294,    0, 2294, 2292,
 2294,    0,    0,    0,  953,    0,    0,    0, 2344,    0,
 1593,    0,    0, 2184,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1882,    0, 1151, 2251,    0,    0,    0,
    0,    0,  867,  884,  893,  996,  999, 1017,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  869,    0,    0,    0,    0, 2720,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1641,    0,    0,
    0,    0,    0, 1646,    0,    0,    0,    0,    0,  179,
    0,    0,    0,    0,    0,    0,    0,   80,    0,   95,
    0,    0,    0,    0,   31,    0,    0,    0,    0,    0,
    0,    0,    0, 7787,    0, 7847,    0, 7922,    0,    0,
    0,    0, 7994,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2599,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 8597,
    0,    0,    0,    0,    0,    0,    0, 1490, 1671,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  804,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1677,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1648, 2135,    0,    0,
    0,  324,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  250,    0,    0,    0,    0,    0,  731,    0,    0,
    0,    0,    0,    0,    0,    0, 2601, 1414, 2383,    0,
 2326,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2344,    0,    0,    0,    0,    0,    0,    0, 1655,
  883,    0,    0,    0,    0,    0,    0,    0,    0, 2344,
 2357,    0,  268,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  456,    0, 8069,    0,    0,    0, 2574,    0,
    0,    0, 8129,    0,    0,    0,    0, 8204,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2586,    0,    0,    0,    0,    0,    0,    0,    0,
 2587,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2342,    0,    0,    0,
    0,    0,    0,    0,  387, 1690,    0,    0,    0,    0,
 2292,  252,  863,    0,  988, 2613,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1720,    0,    0,  104,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  197,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 8334, 8365,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2614,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  107,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1912,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2616,    0,    0,    0,    0,    0,    0,    0,
 1720, 1384,    0,  159,    0,    0,    0,    0,    0,    0,
 2601,    0,    0,    0,    0,    0,    0,    0, 2595,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  457,    0,    0,    0,    0,    0,    0, 6419,    0,
    0,    0, 2613,    0, 1124,    0, 1151, 1382, 1532,    0,
    0,    0,    0,    0,    0,    0,  457, 2598,    0,    0,
    0,    0,    0, 2368,    0,    0,    0,    0, 1338,    0,
    0, 2617,    0, 1720,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2615,    0,    0,14134,    0,
    0,    0,  457,    0,    0,    0,    0,    0, 2613,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  457,    0,    0,  457,    0,    0,    0,    0, 1821,    0,
    0,    0,    0,    0,  457,    0,    0, 2631,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2675,    0,    0, 2676,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2678,
    1,  275,  -31, -533, 1455,  820, -291, -506, 2673, 2398,
    0,    8,    0, 2412,  -98,  -47,    0,  430, -793,  -36,
  -63,  -43, -190, 2419, 1953,    0,    0,    0,  661,    0,
    0,  -45, 1571, -568,  -66, -977, 2043, 1816, -315,    0,
 1586,    0, 2612, 1825, 1864,    0,  -59,    0, -513,  -57,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -535, -392, 1595,
 -289, 1197, -937,    0,    0,    0,    0, -335, -133, -421,
 1605, -352,  -51,    0, -548,    0,    0, 1193,-1015,    0,
 1194,    0,    0, 2365, -680,  -26,    0,    0,    0,    0,
 2337,    0, 2112, 1626,    0,    0,    0, 1682,    0, 2397,
 1950, 1629, 2371, 2372,    0, 1156,    0, -447, 1929,  -44,
 -621, 1104, -243,    0,    0,-1002, -906, -890, -887, -885,
    0, 1245,-1011,    0,  960,-1216, 1246,    0, 2163,    0,
    0,    0,  959,  961,  934,  -23, -255, -920, -930,    0,
    0,    0, 1957,    0, -632,    0, 1776,    0,    0,    0,
    0,    0,-1115, -924,    0,    0,    0,    0, -218,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1055,    0,    0, -251,    0,    0, -214,    0,
 -403,  440, 1340, 1153,    0,  989,    0,    0,    0,    0,
    0,    0,    0, 2033, -130, -427, 1504, 1283, -557, -768,
-1001, -853,    0,    0,  -40,    0,    0,    0,    0,    0,
 2402,    0,    0,    0,    0, 2426, 2202,    0,    0,    0,
    0,    0,   -9, 2437,    0,    0,    0,  -10,    0,    0,
    0, -363, 1910, -357, 1478, -481,    0,    0,    0,    0,
 2188,    0, -301,    0,    0,    0, 2146,    0, 1954,    0,
    0,    0,    0,    0,    0,    5,    0, 2784,    0,    0,
    0,    0,    0, 2482,    0,    0,    0,    0, 2277,    0,
 2274,    0,    0,    0, 2031,    0,    0,    0,    0,    0,
 -236, -670, -884, 2818,    0, 1351,    0,    0,    0,    0,
    0,    0, -105,    0,    0,    0, -223, 1208,    0,    0,
    0, 2654, 1282,    0, 2045, -955,-1498,    0, -904, 1801,
-1291,    0,    0,    0,    0, -152,    0,    0,    0,    0,
 1560,    0,    0,    0,    0, 2723,  531,    0, 1083, 1189,
    0,  979,    0, 1013,    0,  -61,    0,    0, 2048,    0,
    0,    0,    0,    0, 2285,    0,    0,    0, -398, 2077,
 1834,-1042,    0,    0,    0, -707, -483,  190,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1427, 1685,    0,    0,    0, -882,    0,    0,    0,
 1563,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -198, 1639,    0,    0,
};
#define YYTABLESIZE 14963
static short yytable[] = {                                     135,
  205,   38,  135,  325,   81,  156,  155,  228,  162,  277,
  786,  208,  296,  299,  224,  297,  300,  968,  633,  220,
  942,  225,  233,  154,  634,  603,  950,  265,  259,  808,
  237,  726,  245,  268,  263,  787,  240,  571,  597,  711,
  766,  953, 1283, 1215, 1272,  264,  202,  204,  207,  210,
  155,  215,  270,  237,  269, 1272,  226,  204,  231,  267,
  210,  569,  155,  215,  262,  244,  135,  211, 1207, 1503,
 1029,  726,  748,  514,  255,  156,  155,  257,  204,  238,
  255,  231,  207,  210,  266,  155,  215,  244,  135,  493,
  210,  884,  283,  154,  285,  875,  331, 1409,  493,  972,
  745,  726, 1242,  767,  576, 1084, 1110,  537, 1330, 1308,
  327,  295,  159,  742, 1280,  573,   80,  687, 1090,  779,
  789, 1284, 1115,  799,  307,  800,  160,  226,  155,  226,
  810,  647, 1292, 1281,  186,  791,  736, 1224,  619,  862,
 1273,  231,  726,  394,  767,  284,  324,  767,  210,  155,
  244, 1273,  574,  725,  162, 1313, 1274,  356,  247, 1275,
  726, 1276,  767, 1034, 1277,  327,  329, 1274,  333,  775,
 1275,  159, 1276,  538,  621, 1277,  687,  736, 1051,  619,
  736, 1054,  383,  889, 1325,  160, 1620,  281, 1461, 1773,
 1061,  281, 1766,  186,  725,  736,  764,  619,  620,  247,
  891,  657,  247,  767,  791,  294,  712,  298, 1766,  387,
  753,  574,  725,  751,  633,  621,  998,  247,  922,   95,
  634,  325,  843,  574, 1050,  526,  478,   80,  295,  486,
  295,   87,  543,  621,   87, 1177, 1178,   97, 1611,  620,
 1668,  891,  657,  999,  891,  657, 1105,  739,  555,  705,
  523,  699,  603, 1206,  753,  504,  133,  620, 1230,  891,
  657,  226, 1494, 1495,  226,  143, 1203,  186,  155,  753,
  495,  135, 1501, 1587,  712, 1272, 1272,  155, 1806,  495,
  325,  536,  574,   80,  892,  783,  141, 1121,  739, 1471,
  705,  816,  699,  891,  916,  699, 1518,  137,  207,   63,
  740,  295,  155,  541, 1597, 1598,  739,  570,  705, 1272,
  699,  388,  956,  854,  478,  133,  225,  301,  584,  530,
  965,  586, 1342,  658,  143,  892,  186, 1724,  892,  598,
   86,  600, 1540, 1861,  357, 1856,  643,  655, 1483,  856,
 1528, 1472,  226,  892, 1293,  651,  538,  758,  645,  226,
 1496,  226,  139,  226, 1343,  177,  589,  302,  478,  164,
  732,  138, 1122, 1513,  658, 1268,  100,  658, 1612, 1231,
 1091, 1273, 1273, 1669,  210,  215,  528,  892, 1588,  855,
  165, 1068,  658,  155, 1862,  207,  525, 1274, 1274,  664,
 1275, 1275, 1276, 1276, 1532, 1277, 1277, 1132,  166,  661,
  653,  732,  389, 1695,  732, 1273,  758,  358, 1129,  159,
 1259,  917, 1831,  556,  177, 1492,  726,  478,  290,  732,
 1632, 1274, 1064,  160, 1275, 1743, 1276, 1278, 1279, 1277,
 1454,  186, 1456,  780,  715,  712,  301,  726,  721,  719,
  774,  720,  739,  722, 1294,  525, 1772, 1291, 1111, 1347,
 1348,  736, 1349, 1350,  536, 1227,  159,  739,  789, 1127,
 1049, 1002,  836, 1128,  289, 1865, 1237, 1239,  725, 1114,
  160,  916,  736,  791,  619,  752,  921,  155,  186,  767,
  210,  495, 1228, 1706,   95,  101,  186, 1501,  155,  725,
  656,  602,  155, 1238,  736,  210,  527,  155, 1083,  527,
  162,  486,   97,  216, 1045,  744, 1606, 1047,  327,  749,
  621, 1083, 1616,  792,  754,  741,  657, 1541, 1057,  400,
  186,   79,  186,  293, 1784, 1647,  657,  687,  657, 1661,
  837,  656, 1003,  812,  620,  767,  226,  657,  861, 1223,
  692,  143,  143,  726,  186, 1075, 1324,  726,  884,  656,
  602,  493, 1628,  838, 1633, 1531,  237,  573, 1634, 1635,
 1030,  687,  739, 1117,  705, 1670,  699,  805,  226,  186,
 1817,  280, 1721,  805, 1621,  702, 1334, 1404,  699, 1241,
  574,  657,  736,  739,  619,  705, 1491,  699,  917, 1295,
  657, 1654, 1235,   87, 1662, 1234,  767, 1844,  969,  725,
  133,  293,  891,  869,  870,  842, 1261,  225, 1505, 1660,
 1103,  186,  623,  177,  726,  293,  702,  293, 1130,  866,
  621,  159,  478,  325,  574,  177,  538, 1762,  763,  705,
 1022,  699,   79,  657,  702,  160,  916,  143,  658, 1058,
  699,  916,  226,  186,  620,  657,  349,  657,  658,  736,
  658,  247,  177,  623,  753, 1019, 1805,  767,  177,  658,
  177,  842,  692,  232, 1674,  924,  725,   79,  859, 1285,
 1286,  623, 1454,  717,  384,  732,  313,  213,  718, 1756,
 1664,  705,  231,  699, 1549,  478,  892,  478,   79,  977,
 1736,  350, 1272,  739, 1737,  705,  732,  699,  601,  177,
  737, 1530,  168,  658, 1263,  739, 1467,  958,  758, 1746,
  945,  946,  658,  382,  657,  226,  703,  383,  213, 1335,
  226,  213,  226, 1747,  226, 1296, 1750,  495,  527, 1748,
  186,  250,  495,  384,  536, 1131,  213, 1866, 1354,  225,
  495,  737, 1133,  210, 1039,  555,  916,  525,  478, 1005,
  210, 1815,   14,  917,  169,  658,  478,  703,  917,  737,
 1066,  527,  705,  155,  699,  155, 1282,  658,  226,  658,
 1326,  337,  382, 1504,  226,  703,  383,  362, 1290,  747,
  530, 1711, 1028, 1067, 1069,  259,  237, 1217, 1273,  186,
 1220,  226,  178, 1074, 1080, 1100, 1101, 1102,  351, 1117,
 1625, 1402, 1312,  527, 1274,  656,  732, 1275, 1088, 1276,
 1790,  916, 1277, 1792,  130,  656,  352,  656,  226,  226,
 1256,  237,  237,  237,  789,  721,  656, 1712, 1517,  602,
  722,  142,  664, 1569, 1527,  252,  658,  738, 1713,  696,
 1470,  538, 1081,  102,  260,  684,  606, 1801,  916,  947,
  353,  178,  858, 1822,  732,  398,  890,  155,  527, 1134,
 1718,  897,  695,  917,  177, 1382,  389,  155,  144,  526,
  656, 1257, 1829,  732, 1104,  664, 1126,  916,  738,  656,
  696,  103,  145,  390, 1112,  661,  684, 1816,  602,  684,
  702,  602,  386,  104,  367, 1791,  738,  606,  696, 1297,
  702,  337,  702,  695,  684, 1355,  916,  602, 1855, 1703,
  602,  702,  398,  527,  398,  597,  362,  105, 1221,  378,
  403,  695,  656,  273,  259,  389, 1864,  144,  917, 1867,
 1463, 1218,  278,  602,  656, 1298,  656, 1714,  384, 1222,
 1872,  145,  390, 1219, 1154,  841, 1198, 1199,  623,  536,
  289,  386,  701,  213,  213,  702, 1022, 1582, 1155,   14,
  337,  213, 1845,  291,  702,  917,  527,  368,  495,  210,
  215,  495,  244,  135,  597,  791, 1195,  382,  378,  495,
  723,  383,  106,  293,  130,  130,  841,  700,  186,  841,
  495,  495,  107,  701,  917,  385, 1395,  210,  388, 1397,
  186, 1846, 1156,  656,  841, 1516, 1247,  702,  717, 1512,
 1021,  701,  567,  718,  271,  737,  387,  108,  633, 1248,
  528,  702,  769,  917,  634,  568,  274,  186,  700, 1209,
 1157,  703, 1319,  186,  279,  186,  737,  529,  123, 1776,
 1534,  703, 1249,  703, 1779,  986,  700, 1337, 1847,  213,
  178,  282,  703, 1341,  385,  603, 1250,  388,  237,  968,
  717,  538,  178,  225,  384,  718, 1631,  486, 1145, 1303,
  213, 1400,  123,  237,  186,  387,  213,  890,  823,  237,
  824,  890,  186,  429, 1304, 1200, 1040, 1303,  702,  178,
  337,  770, 1146, 1251, 1158,  178,  703,  178,  226, 1458,
 1458, 1462,  226,  382, 1314,  703,  123,  383,  287,  684,
  213, 1663,  471,  650,  603, 1355, 1355,  986,  337,  398,
  151, 1355, 1355,  247, 1355, 1355,  968,  968,  286, 1488,
  389, 1490, 1355, 1355, 1355, 1355,  178, 1355, 1355, 1355,
 1355, 1355, 1355,  613,  370,  144,  737,  390,  703,  785,
  241,  186,  738, 1315,  696, 1041,  386,  287,  370,  145,
  684, 1405,  703,  684,  247,  478,  696,  247,  225,  536,
  684,  471,  684,  738,  659,  696,  684,  695,  378,  576,
  288,  684,  247,  378,  684, 1639,  598,  659,  378,  695,
  785,  241, 1533,  785,  241,  659,  384, 1575,  695,  684,
  226, 1578,  613, 1410, 1451, 1252, 1640,  841,  785,  241,
  659,  816, 1641,  684,  495,  572,  495, 1731,  813,  696,
  152,  684,  218,  219,  659,  684, 1314,  153,  696,  703,
  495,  385,  731,  830,  684,  730,  210,  292,  576,  186,
 1732,  581,  695,  502, 1808,  598, 1642, 1666,  337,  147,
  684,  695,  578,  597,  103,  703,  389,  327,  580,  385,
  841,  659,  388,  841,  338,  968,  301,  701,  326,  831,
  841,  696,  841,  390,  572, 1318,  841,  684,  538,  301,
  387,  841,  386,  738,  841,  696,  597,  308,  701,  684,
  105,  684,  502, 1539,  695,  814, 1733,  684, 1544,  841,
  581,  178,  700,  232,  339, 1515,  918, 1766,  695,  378,
  334,  578,  597,  841,  703,  597,  327,  580, 1618,  926,
  567,  841, 1765,  700,  841,  841,  684,  684,  341,  684,
  735,  597,  701,  734,  841,  815,  684,  684,  684,  684,
  684,  701,  684,  232,  405,  684,  684,  684,  684,  684,
  841, 1586,  696,  684,  684,  106,  301,  597,  684,  684,
  684,  684,  684,  721,  719,  107,  720,  700,  722,  832,
  471, 1749,  841,  659,  335,  695,  700,  841, 1693,  567,
  337,  243,  471,  767,  701,  385,  536, 1708,  388,  841,
  108,  841, 1247,  603,  802, 1247,  928,  801,  701,  247,
  247,  204,  359,  405,  301, 1248,  387,  247, 1248, 1596,
 1126, 1126, 1840,  207,  406,  471,  247,  471,  402,  700,
  232, 1451,  243,  372,  767,  243,  603,  767, 1249,  721,
  719, 1249,  720,  700,  722,  168,  721,  719,  841,  720,
  243,  722,  767,  772,  930, 1250, 1761, 1841,   44, 1710,
  248,   52,  603, 1725,  207,  603,  495,  207,  841, 1842,
  232,  324,  841, 1723,  785,  701,  447,  785,  210,  210,
  772,  603,  207,  406,  785,  465,  785,  402,  370, 1251,
  785,  613, 1251, 1766, 1458,  785,  721,  719,  785,  720,
  512,  722,  155,  169,  168,  247,  381,  603, 1828,  517,
  700,  721,  719,  785,  720,  515,  722,   44, 1796, 1645,
   52, 1025, 1798,  521,  613,   42,  247,  785,  391,  465,
  590,   25,  247, 1804,  598,  785,  393, 1026,  785,  785,
  591,  244,  763,  549,  465, 1008,  158,  554,  785, 1701,
  613,   48, 1826,  613,  396, 1708, 1744, 1008,  399, 1708,
 1008, 1799,  169, 1485,  785,  809,  247,  598,   50,  613,
 1708,  721,  719, 1486,  720,  480,  722,  159,  146,  147,
 1726,  309,  244, 1850,   42,  244,  785,  210, 1852, 1708,
   25,  785,  160,  598,  395,  613,  598,  135, 1745, 1126,
  244,  763,  373,  785, 1252,  785,  809,   26,  717,  809,
   48,  400,  598,  718,  310,  361,  324, 1710,  362,  210,
 1708, 1710, 1719, 1710,  809, 1708,  247,   50,  490,  538,
   49, 1752, 1710,   51,  480, 1754,  958,  373,  598, 1360,
  311,  155,  702,  312,  483,   46,  207,  708,  709,  710,
  136, 1710,  785, 1785,  538,  124,  135,   47, 1742,  313,
  363,  373, 1768, 1453,  207, 1455,   26,  243,  243,  767,
  767,  374,  785,  375,  717,  243,  785,  767,  378,  718,
  716,  717, 1710,   83,   84,  314,  718, 1710,  378,   49,
  538,  498,   51,  952,  995,  376,  723,  994,  552,  666,
  553,  756,  721,  719,   46,  720,  499,  722,  538,  136,
  503,  538,  204, 1770,  124,  716,   47,  755,  502, 1573,
  377,  757,  538,  207, 1771, 1109, 1013,  716, 1108,  241,
  505,  717,  999, 1760,  218,  219,  718,  536, 1013,  666,
  666, 1013,  507,  666,  996,  486,  717,  986,  951,  721,
  719,  718,  720,  508,  722, 1004,  721,  719,  666,  720,
  522,  722,  536,  243,  509,  767,  274,  226, 1793,  767,
  241, 1194, 1451,  241,  524,  343, 1113, 1005, 1208,  880,
  226,  723, 1809,  155,  243,  525,  767,  344,  241, 1475,
  243, 1477,  767, 1211, 1859,  534, 1210, 1451,  536, 1260,
  504, 1306, 1108,  544, 1305, 1830,  717, 1311, 1809,  155,
 1310,  718, 1465, 1076, 1077,  723,  536,  244,  244,  536,
  345, 1876,  346, 1877,  243,  244,  767,  809, 1548, 1580,
  536,  723,  723, 1451, 1005,  131,  131, 1557, 1809,  155,
 1617,  504,  207,  723,  504,  721,  719,  546,  720, 1835,
  722, 1451, 1834, 1126, 1451,  548,   14, 1836,  207,  504,
  734,  560, 1186,  721,  719, 1451,  720, 1863,  722,  368,
 1766,  724,  368,  207,  724,  207,  561,  564,  896,  527,
  809,  562,  582,  809,  243,  585,  767,  599,  595,  652,
  809,  220,  809,  648,  656,  920,  809,   55,  654,  925,
  688,  809,  689,  690,  809,  691,  941,  692,   56,  693,
  694,  948,  949,  244,  494,   57,  695,  696,  697,  809,
  527,  668,  447,  527,  959,  960,  961,  962,  963,  964,
  447,  698,  220,  809,  244,  220,  699,  717,  527,   58,
  244,  809,  718,  700,  809,  809,  721,  719, 1204,  720,
  220,  722,  705,  706,  809,  494,  707,  716,  494,  723,
  729,  733,  668,  724,  725,  668,  721,  719,  517,  720,
  809,  722,  732,  494,  244,  419,  420,  421,  737,  919,
  668,  923,  750, 1758,  717,  927,  929,  931, 1031,  718,
  742,  717,  809,  753,  549, 1038,  718,  809,  760,  762,
 1006,   59,  773,  429,  430,  241,  241,  806,  777,  809,
  778,  809,  782,  241,  666,  721,  719,  666,  720,  784,
  722,  785,  788,  790,  666,  794,  666,  555,  797,  793,
  666,  796,   60,  803,  244,  666,   61,  827,  666,  828,
  982, 1006, 1006, 1006, 1006, 1006, 1006, 1006,  829,  834,
  293, 1709,  839,  666,  840,  846,  847,  849,  809, 1006,
 1006, 1006, 1006,  848,  851,  852,  857,  666,  247,  863,
  864,  867,  877,  868,  885,  666,  504,  504,  809,  666,
  717,  504,  809,  880,  504,  718,  886,  887,  666,  891,
  879, 1006,  943, 1006,  954,  858,  295,  504,  717,  990,
  973,  241,  248,  718,  666,  989, 1135, 1136, 1140,  992,
  996, 1148, 1150, 1152, 1000, 1160, 1163, 1166, 1167, 1169,
 1172, 1175,  241, 1001, 1179, 1180, 1018, 1182,  241, 1184,
 1023,  666, 1187, 1188, 1189, 1190, 1191, 1192, 1032,  734,
 1035, 1046, 1056,  666,  655,  666, 1380,  721,  719, 1196,
  720, 1201,  722, 1055, 1058,  527,  527, 1062, 1063, 1071,
  527, 1065,  241,  527,  806, 1072, 1099,  232,  220, 1073,
 1086, 1079,  504, 1097, 1106, 1120,  527, 1142,  109, 1092,
 1183,  717, 1185, 1193,  655,  655,  718,  717,  655, 1212,
  494,  494,  666,  504, 1226, 1214, 1232, 1229,  494,  504,
 1224,  717,  447,  655, 1240, 1244,  718,  110,  111, 1709,
  112,  494,  666, 1709, 1246, 1800,  666,  113,  114, 1254,
  115,  116, 1264,  117, 1709, 1267,  118,  119,  120,  121,
  122, 1287, 1289,  504,  123,  124,  668,  758, 1007,  668,
  125,  126,  127, 1709, 1299, 1307,  668, 1316,  668, 1309,
  717,  527,  668, 1320, 1321,  718, 1322,  668, 1323, 1331,
  668, 1345, 1006,  232, 1006, 1006, 1339, 1344, 1114, 1346,
 1352, 1006,  527, 1353, 1709,  668, 1825, 1006,  527, 1709,
 1358, 1357, 1006, 1359,  232, 1006,  494, 1361, 1362,  668,
  232, 1369, 1370,  916,  444,  721,  719,  668,  720, 1006,
  722,  668, 1371, 1372, 1006, 1006, 1374,  494, 1006, 1375,
  668, 1376,  527,  494, 1377, 1006, 1378, 1006, 1006, 1379,
 1396, 1006, 1452, 1399,  232, 1006,  668, 1006, 1466, 1006,
 1468, 1006, 1469,  758, 1473, 1478, 1006, 1006, 1479, 1006,
 1480, 1149, 1481, 1153,  577, 1161, 1164,  494,  447, 1170,
 1173, 1176, 1489,  668, 1006, 1006, 1502, 1006, 1514, 1083,
 1363, 1364, 1506, 1366, 1368,  668, 1507,  668, 1006, 1508,
 1006, 1373, 1006, 1006, 1006, 1519, 1006, 1529, 1537, 1006,
 1006, 1509,  717, 1510, 1511, 1535, 1538,  718, 1543, 1006,
 1550, 1386,  221, 1570, 1576, 1590, 1577, 1584, 1583, 1393,
 1394, 1201, 1006, 1589, 1201, 1006, 1196,  138, 1602, 1592,
 1401,  447, 1403, 1593,  668, 1600, 1601, 1603, 1604, 1609,
 1610, 1006, 1613, 1614, 1619, 1615, 1626, 1006, 1006, 1636,
 1646, 1651, 1006,  221,  668, 1648,  221, 1649,  668, 1653,
 1650, 1655, 1652, 1006, 1006, 1656, 1006, 1657, 1667, 1671,
  447,  221, 1006, 1665, 1672, 1673, 1675, 1676, 1677,  655,
 1678, 1681,  655, 1682, 1683, 1684, 1685, 1686, 1687,  655,
 1688,  655, 1689, 1690, 1692,  655, 1698, 1705, 1605, 1720,
  655, 1006, 1006,  655, 1006, 1730, 1738, 1741, 1497, 1757,
 1740, 1006, 1006, 1006, 1006, 1006, 1753, 1006,  655, 1763,
 1006, 1006, 1006, 1006, 1006, 1764, 1767, 1774, 1006, 1006,
 1775,  444,  655, 1006, 1006, 1006, 1006, 1006, 1006, 1777,
  655, 1780, 1782, 1783,  655, 1381,  721,  719, 1823,  720,
  717,  722, 1794,  655, 1007,  718, 1824, 1797,  986, 1384,
  721,  719, 1786,  720, 1818,  722, 1766, 1820, 1827,  655,
 1833, 1838, 1839, 1815, 1385,  721,  719, 1545,  720, 1816,
  722, 1851, 1853, 1387,  721,  719, 1553,  720, 1854,  722,
 1858, 1860, 1870, 1558, 1874, 1873,  655, 1875,    1,  986,
  986,  986,  986,  986,  999,  986,  721,  719,  655,  720,
  655,  722, 1878,  732,   30,   32,  655,  986,  986,  986,
  986,  193, 1201,  980,  767,  261, 1201,   14, 1388,  721,
  719,  370,  720,  156,  722,  648,  462,   82,  967,  967,
  203,  891, 1591,  272,  739,  655,  655,  183,  655,  986,
  398,  986,  271,  525,  966,  655,  655,  655,  655,  655,
  692,  655,  694,  184,  655,  655,  655,  655,  655,  947,
  109,  371,  655,  655,  213,  322,  288,  655,  655,  655,
  655,  655,  711,  330,  946,  710,  357,  712,  232,  221,
  932,  933,  934,  935,  936,  937,  938,  939,  342,  110,
  111,  721,  112,  328,   27,   30, 1622,   40,  109,  113,
  114, 1457,  115,  116,  143,  117,  506, 1340,  118,  119,
  120,  121,  122,  497,  991, 1119,  123,  124,  878,  496,
 1338,  261,  125,  126,  127, 1082, 1336,  110,  111, 1124,
  112, 1328, 1658,  566, 1659,  607,  841,  113,  114,  527,
  115,  116, 1317,  117, 1266,  542,  118,  119,  120,  121,
  122,  579,  580, 1033,  123,  124, 1329, 1696, 1060, 1739,
  125,  126,  127, 1638, 1814,  822, 1644, 1821, 1819, 1849,
 1216,  717, 1679, 1680,  232, 1759,  718, 1053, 1595, 1717,
  527, 1802,  970,  527, 1484,  717, 1629,  587, 1098, 1691,
  718,  721,  719, 1201,  720,  232,  722, 1694,  527,  572,
  717,  232, 1700,  563, 1702,  718,  826,  865,   80,  717,
  986, 1526,  986,  986,  718,  845,  145,  535, 1181,  986,
  768,  772,  306, 1024, 1089,  986, 1020, 1243,  256, 1476,
  986,  717, 1781,  986, 1734,  232,  718, 1857, 1389,  721,
  719, 1837,  720,  781,  722,  997,  967,  986, 1010,  986,
 1236, 1493,  986,  986,  717, 1037,  986,    0, 1398,  718,
 1365,    0,    0,  986,    0,  986,  986,    0,    0,  986,
    0,    0,    0,  986,    0,  986,    0,  986,    0,  986,
    0,    0, 1751,    0,  986,  986,    0,  986,    0, 1010,
 1010, 1010, 1010, 1010, 1010, 1010,  721,  719, 1205,  720,
    0,  722,  986,  986,    0,  986,    0, 1010, 1010, 1010,
 1010,    0,    0,    0,    0,  109,  986,  447,  986,    0,
  986,  986,  986,    0,  986,    0,    0,  986,  986,    0,
    0,    0,    0,    0,    0,    0,    0,  986,    0, 1010,
    0, 1010,    0,    0,  110,  111, 1622,  112,    0,    0,
  986, 1585,    0,  986,  113,  114,  109,  115,  116,    0,
  117,    0,    0,  118,  119,  120,  121,  122,    0,  986,
    0,  123,  124,    0,    0,  986,  986,  125,  126,  127,
  986,    0,    0,    0,    0,  110,  111,    0,  112,    0,
 1803,  986,  986,    0,  986,  113,  114,    0,  115,  116,
  986,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0, 1351,  123,  124,    0,  527,  527,    0,  125,  126,
  127,    0,    0,  527,    0,    0,  717,    0,    0,  986,
  986,  718,  986,    0,    0,    0,  527,    0,  109,  986,
  986,  986,  986,  986,    0,  986,    0,    0,  986,  986,
  986,  986,  986,    0,    0,    0,  986,  986,    0,    0,
    0,  986,  986,  986,  986,  986,  986,  110,  111,    0,
  112,    0,    0,    0,  717,    0,    0,  113,  114,  718,
  115,  116,    0,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,  966,    0,    0,
  125,  126,  127,    0,    0,    0,    0,  652,    0,    0,
 1010,  527, 1010, 1010,    0,    0,    0,    0,    0, 1010,
    0,    0,    0,    0,    0, 1010,    0,    0,    0,    0,
 1010,  717,  527, 1010,    0,    0,  718,    0,  527,    0,
    0,    0,    0,    0,    0,    0,    0, 1010, 1015,    0,
    0,    0, 1010, 1010,    0,    0, 1010,    0,    0,    0,
    0,    0,    0, 1010,    0, 1010, 1010,    0,    0, 1010,
    0,    0,  527, 1010,    0, 1010,    0, 1010,    0, 1010,
    0,    0,    0,    0, 1010, 1010,    0, 1010,    0, 1015,
 1015, 1015, 1015, 1015, 1015, 1015,    0,    0,    0,    0,
    0,    0, 1010, 1010,    0, 1010,    0, 1015, 1015, 1015,
 1015,    0,    0,   79,    0,  109, 1010,    0, 1010,    0,
 1010, 1010, 1010,    0, 1010,    0,    0, 1010, 1010,    0,
    0,    0,    0,    0,    0,    0,    0, 1010,    0, 1015,
    0, 1015,    0,    0,  110,  111,    0,  112,    0,    0,
 1010, 1594,    0, 1010,  113,  114,  109,  115,  116,    0,
  117,    0,    0,  118,  119,  120,  121,  122,    0, 1010,
    0,  123,  124,    0,    0, 1010, 1010,  125,  126,  127,
 1010,    0,    0,    0,    0,  110,  111,    0,  112,    0,
    0, 1010, 1010,    0, 1010,  113,  114,    0,  115,  116,
 1010,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,    0,    0,    0,  125,  126,
  127,  721,  719, 1581,  720,    0,  722,    0,    0, 1010,
 1010,    0, 1010,    0,    0,    0,    0,    0,    0, 1010,
 1010, 1010, 1010, 1010,  109, 1010,    0,    0, 1010, 1010,
 1010, 1010, 1010,    0,    0,    0, 1010, 1010,    0,    0,
    0, 1010, 1010, 1010, 1010, 1010, 1010,  721,  719,    0,
  720,    0,  722,  110,  111,    0,  112,    0,    0,    0,
  652,    0,    0,  113,  114,    0,  115,  116,    0,  117,
    0,    0,  118,  119,  120,  121,  122,    0,    0,    0,
  123,  124,    0,    0,    0,    0,  125,  126,  127,    0,
 1015,    0, 1015, 1015,    0,    0,    0,    0,    0, 1015,
    0,    0,    0,    0,    0, 1015,    0,    0,    0,    0,
 1015,    0,    0, 1015, 1390,  721,  719,    0,  720,  525,
  722,    0,    0,    0,    0,    0,    0, 1015, 1011,    0,
    0,    0, 1015, 1015,    0,    0, 1015,    0,    0,    0,
    0,    0,    0, 1015,    0, 1015, 1015,    0,    0, 1015,
    0,    0,    0, 1015,    0, 1015,    0, 1015,    0, 1015,
  525,    0,    0,  525, 1015, 1015,    0, 1015,    0, 1011,
 1011, 1011, 1011, 1011, 1011, 1011,    0,    0,  525,    0,
  525,    0, 1015, 1015,    0, 1015,    0, 1011, 1011, 1011,
 1011,    0,    0,    0,    0,  655, 1015,    0, 1015,    0,
 1015, 1015, 1015,    0, 1015,    0,    0, 1015, 1015,    0,
    0,    0,    0,    0,    0,    0,    0, 1015,    0, 1011,
    0, 1011,    0,    0,  655,  655,    0,  655,    0,    0,
 1015,    0,    0, 1015,  655,  655,  717,  655,  655,    0,
  655,  718,    0,  655,  655,  655,  655,  655,    0, 1015,
    0,  655,  655,    0,    0, 1015, 1015,  655,  655,  655,
 1015,    0,    0,    0,    0,    0,    0, 1556,    0,    0,
    0, 1015, 1015,    0, 1015, 1391,  721,  719,    0,  720,
 1015,  722,  717,    0, 1392,  721,  719,  718,  720,    0,
  722,    0,    0,    0,    0, 1571,  721,  719,    0,  720,
    0,  722, 1572,  721,  719,    0,  720,    0,  722, 1015,
 1015,    0, 1015,    0,    0,    0,    0,    0,    0, 1015,
 1015, 1015, 1015, 1015,  109, 1015,    0,    0, 1015, 1015,
 1015, 1015, 1015,    0,    0,    0, 1015, 1015,    0,    0,
    0, 1015, 1015, 1015, 1015, 1015, 1015,    0,    0,    0,
  717,    0,    0,  110,  111,  718,  112,    0,    0,    0,
    0,    0,    0,  113,  114,    0,  115,  116,    0,  117,
    0,    0,  118,  119,  120,  121,  122,  525,    0,    0,
  123,  124,    0,    0,    0,    0,  125,  126,  127,    0,
 1011,    0, 1011, 1011,  525,  525,  525,    0,    0, 1011,
  525,    0,    0,  525,    0, 1011,    0,    0,    0,    0,
 1011,    0,    0, 1011,    0,    0,  525,    0,    0,  522,
    0,    0,  525,    0,    0,    0,    0, 1011, 1024,    0,
    0,    0, 1011, 1011,    0,    0, 1011,    0, 1579,  721,
  719,    0,  720, 1011,  722, 1011, 1011,    0,    0, 1011,
 1755,  721,  719, 1011,  720, 1011,  722, 1011,    0, 1011,
  522,    0,    0,  522, 1011, 1011,   65, 1011,    0, 1024,
 1024, 1024, 1024, 1024, 1024, 1024,    0,   66,  522,    0,
  522,    0, 1011, 1011,   67, 1011,    0, 1024, 1024, 1024,
 1024,  525,    0,    0,    0,    0, 1011,   68, 1011,    0,
 1011, 1011, 1011,    0, 1011,    0,    0, 1011, 1011,    0,
    0,  717,  525,    0,    0,    0,  718, 1011,  525, 1024,
  717, 1024,    0,    0,    0,  718,    0,  525,    0,  525,
 1011,  717,    0, 1011,    0,    0,  718,    0,  717,    0,
    0,    0,    0,  718,    0,    0,    0,  525,    0, 1011,
    0,    0,  525,    0,   69, 1011, 1011,    0,    0,    0,
 1011,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   70, 1011, 1011,    0, 1011,    0,    0,    0,    0,    0,
 1011,  525,    0,    0,    0,    0,   71,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   72,    0,    0,    0,   73,    0,    0,   74, 1011,
 1011,    0, 1011,    0,    0,    0,   75,    0,    0, 1011,
 1011, 1011, 1011, 1011,    0, 1011,    0,    0, 1011, 1011,
 1011, 1011, 1011,    0,    0,   76, 1011, 1011,    0,    0,
    0, 1011, 1011, 1011, 1011, 1011, 1011,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  717,    0,    0,  522,    0,  718,
    0,    0,    0,    0,    0,    0,  717,    0,    0,    0,
 1024,  718, 1024, 1024,  522,  522,  522,    0,    0, 1024,
  522,    0,    0,  522,    0, 1024,    0,    0,    0,    0,
 1024,    0,    0, 1024,    0,    0,  522,    0,    0,  527,
    0,    0,  522,    0,    0,    0,    0, 1024,  860,    0,
    0,    0, 1024, 1024,    0,    0, 1024,    0,    0,    0,
    0,    0,    0, 1024,    0, 1024, 1024,    0,    0, 1024,
  577,    0,    0, 1024,    0, 1024,    0, 1024,    0, 1024,
  527,    0,    0,  527, 1024, 1024,    0, 1024,    0,  860,
  860,  860,  860,  860,    0,  860,    0,    0,  527,    0,
  527,    0, 1024, 1024,    0, 1024,    0,  860,  860,  860,
  860,  522,    0,    0,    0,    0, 1024,    0, 1024,    0,
 1024, 1024, 1024,    0, 1024,    0,    0, 1024, 1024,    0,
    0,    0,  522,    0,    0,    0,    0, 1024,  522,    0,
    0,  860,    0,    0,    0,    0,    0,    0,    0,  522,
 1024,    0,    0, 1024,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  522,    0, 1024,
    0,    0,  522,    0,    0, 1024, 1024,    0,    0,    0,
 1024,    0,    0,    0,    0,    0,    0,    0,    0, 1546,
 1547, 1024, 1024,    0, 1024, 1551, 1552,    0, 1554, 1555,
 1024,  522,    0,    0,    0,    0, 1559, 1560, 1561, 1562,
    0, 1563, 1564, 1565, 1566, 1567, 1568,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1024,
 1024,    0, 1024,    0,    0,    0,    0,    0,  109, 1024,
 1024, 1024, 1024, 1024,    0, 1024,    0,    0, 1024, 1024,
 1024, 1024, 1024,    0,    0,    0, 1024, 1024,    0,    0,
    0, 1024, 1024, 1024, 1024, 1024, 1024,  110,  111,    0,
  112,    0,    0,  527,    0,    0,    0,  113,  114,    0,
  115,  116,    0,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,  527,    0,    0,
  125,  126,  127,    0,    0,    0,    0,    0,    0,    0,
  860,    0,  860,  860,  527,  527,  527,  527,    0,  860,
  527,    0,    0,  527,    0,  860,    0,    0,    0,    0,
  860,    0,  527,  860,  527,    0,  527,    0,    0,    0,
    0,    0,  527,    0,    0,    0,    0,  860,  872,    0,
    0,    0,  860,  860,    0,    0,  860,    0,    0,    0,
    0,    0,    0,  860,    0,  860,  860,    0,    0,  860,
    0,    0,    0,  860,    0,  860,    0,  860,    0,  860,
    0,    0,    0,    0,  860,  860,    0,  860,    0,  872,
  872,  872,  872,  872,    0,  872,    0,    0,    0,    0,
    0,    0,  860,  860,    0,  860,    0,  872,  872,  872,
  872,  527,    0,    0,    0,    0,  860,    0,  860,    0,
  860,  860,  860,    0,  860,    0,    0,  860,  860,    0,
    0,    0,  527,    0,    0,    0,    0,  860,  527,    0,
    0,  872,    0,    0,    0,    0,    0,    0,  978,  527,
  860,    0,    0,  860,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1269,    0,    0,  527,    0,  860,
    0,    0,  527,    0,    0,  860,  860,    0,    0,    0,
  860,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  860,  860,    0,  860,    0,    0,    0,    0,    0,
  860,  527,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  860,
  860,  527,  860,    0,    0,    0,    0,    0,    0,  860,
  860,  860,  860,  860,    0,  860,  527,    0,  860,  860,
  860,  860,  860,    0,    0,    0,  860,  860,  981,    0,
    0,  860,  860,  860,  860,  860,  860,    0,    0,    0,
  527,    0,    0,    0,    0,  109,  527,    0,    0,  527,
    0,    0,    0,    0,    0,    0,    0,    0,  583,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  110,  111,    0,  112,    0,    0,
  872,    0,  872,  872,  113,  114,    0,  115,  116,  872,
  117,  527,    0,  118,  119,  120,  121,  122,    0,    0,
  872,  123,  124,  872,    0,    0,    0,  125,  126,  127,
    0,    0,    0,    0,    0,    0,    0,  872,  875,    0,
    0,    0,  872,  872,    0,    0,  872,    0,    0,    0,
    0,    0,    0,  872,    0,  872,  872,    0,    0,  872,
    0,    0,    0,  872,    0,  872,  527,  872,    0,  872,
    0,    0,    0,  527,  872,  872,    0,  872,    0,  875,
  875,  875,  875,  875,    0,  875,    0,    0,    0,    0,
    0,  527,  872,  872,    0,  872,    0,  875,  875,  875,
  875,  407,    0,    0,    0,    0,  872,    0,  872,    0,
  872,  872,  872,    0,  872,    0,    0,  872,  872,    0,
    0,  527,    0,    0,    0,  527,    0,  872,    0,  409,
    0,  875,    0,    0,    0,    0,    0,    0,    0,    0,
  872,   87,    0,  872,    0,    0,  109,    0,   88,    0,
    0,    0,   89,   90,    0,    0,    0,  411,    0,  872,
    0,   91,    0,    0,    0,  872,  872,    0,    0,    0,
  872,    0,   92,    0,    0,  110,  111,    0,  112,    0,
    0,  872,  872,    0,  872,  113,  114,    0,  115,  116,
  872,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,    0,    0,    0,  125,  126,
  127,    0,    0,    0,    0,    0,    0,    0,    0,  872,
  872,    0,  872,    0,    0,    0,    0,    0,  416,  872,
  872,  872,  872,  872,   93,  872,    0,    0,  872,  872,
  872,  872,  872,    0,  418,    0,  872,  872,    0,    0,
   94,  872,  872,  872,  872,  872,  872,    0,  419,  420,
  421,    0,  218,  219,    0,   95,    0,    0,    0,   96,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   97,  424,  425,  426,  427,  428,  429,  430,  431,  432,
    0,    0, 1731,    0,    0,    0,  434,  435,    0,   98,
  875,    0,  875,  875,    0,  439,    0,    0,    0,  875,
    0,    0,    0,    0,    0, 1732,    0,    0,    0,    0,
    0,    0,    0,  875,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  875,  876,    0,
    0,    0,  875,  875,    0,    0,  875,    0,    0,    0,
    0,    0,    0,  875,    0,  875,  875,    0,    0,  875,
    0,    0,    0,  875,    0,  875,    0,  875,    0,  875,
    0, 1733,    0,    0,  875,  875,    0,  875,    0,  876,
  876,  876,  876,  876,    0,  876,    0,    0,    0,    0,
    0,    0,  875,  875,    0,  875,    0,  876,  876,  876,
  876,    0,    0,    0,    0,  109,  875,    0,  875,    0,
  875,  875,  875,    0,  875,    0,    0,  875,  875,    0,
    0,    0,    0,    0,    0,    0,    0,  875,    0,    0,
    0,  876,    0,    0,  110,  111,    0,  112,    0,    0,
  875,    0,    0,  875,  113,  114,    0,  115,  116,    0,
  117,    0,    0,  118,  119,  120,  121,  122,    0,  875,
    0,  123,  124,    0,    0,  875,  875,  125,  126,  127,
  875,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  875,  875,    0,  875,    0,    0,    0,    0,    0,
  875,    0,  951,  721,  719,    0,  720,    0,  722,    0,
    0,    0,  487,    0,    0,    0,    0,    0,    0,    0,
    0,  914,  913,  915,    0,    0,    0,    0,    0,  875,
  875,    0,  875,    0,    0,    0,    0,    0,  109,  875,
  875,  875,  875,  875,    0,  875,    0,    0,  875,  875,
  875,  875,  875,    0,    0,    0,  875,  875,    0,    0,
    0,  875,  875,  875,  875,  875,  875,  110,  111,    0,
  112,    0,    0,    0,    0,    0,    0,  113,  114,    0,
  115,  116,    0,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    1,
  125,  126,  127,    0,    0,    0,    0,    0,    0,    0,
  876,    0,  876,  876,    0,    0,    0,    2,    0,  876,
    0,    0,    0,    0,    0,    3,    0,    0,    0,    0,
    0,    0,    4,  876,    5,    0,    6,  870,    0,    0,
    0,    7,    0,    0,    0,    0,    0,  876,    8,    0,
    0,    0,  876,  876,    0,    0,  876,    0,    0,    0,
    0,    0,    9,  876,    0,  876,  876,    0,    0,  876,
    0,    0,   10,  876,    0,  876,    0,  876,  870,  876,
  870,  870,  870,    0,  876,  876,    0,  876,    0,    0,
    0,    0,    0,    0,    0,    0,  870,  870,  870,  870,
    0,    0,  876,  876,    0,  876,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  876,    0,  876,    0,
  876,  876,  876,    0,  876,    0,    0,  876,  876,    0,
  870,    0,  900,    0,    0,   11,    0,  876,  717,    0,
    0,    0,   12,  718,    0,   13,  901,   14,   15,    0,
  876,    0,    0,  876,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  876,
    0,    0,   16,    0,    0,  876,  876,    0,    0,    0,
  876,    0,  902,    0,    0,    0,    0,    0,    0,    0,
  903,  876,  876,    0,  876,    0,    0,    0,  904,    0,
  876,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  905,    0,  906,    0,
    0,   17,    0,    0,    0,    0,    0,    0,    0,  876,
  876,  907,  876,  908,  909,  910,   18,   19,  491,  876,
  876,  876,  876,  876,    0,  876,    0,    0,  876,  876,
  876,  876,  876,    0,    0,  109,  876,  876,    0,    0,
    0,  876,  876,  876,  876,  876,  876,    0,    0,    0,
  721,  719,    0,  720,    0,  722,    0,    0,    0,    0,
    0,    0,  911,    0,  110,  111,    0,  112,  914,  913,
  915,    0,    0,    0,  113,  114,    0,  115,  116,  870,
  117,  870,  870,  118,  119,  120,  121,  122,  870,    0,
    0,  123,  124,    0,    0,    0,    0,  125,  126,  127,
    0,    0,  870,    0,    0,    0,  869,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  870,    0,    0,    0,
    0,  870,  870,    0,    0,  870,    0,    0,    0,    0,
    0,    0,  870,    0,  870,  870,    0,    0,  870,    0,
    0,    0,  870,    0,  870,    0,  870,  869,  870,  869,
  869,  869,    0,  870,  870,    0,  870,  211,    0,  912,
  211,    0,    0,    0,    0,  869,  869,  869,  869,    0,
    0,  870,  870,  211,  870,    0,    0,  211,    0,    0,
  211,    0,    0,    0,    0,  870,    0,  870,    0,  870,
  870,  870,    0,  870,    0,    0,  870,  870,    0,  869,
    0,    0,    0,    0,    0,    0,  870,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  870,
  211,    0,  870,    0,    0,    0,  211,  211,  211,    0,
  211,  211,  211,    0,  211,    0,    0,    0,  870,    0,
    0,    0,    0,    0,  870,  870,    0,    0,    0,  870,
    0,    0,  211,    0,  211,    0,    0,    0,    0,    0,
  870,  870,    0,  870,    0,    0,    0,    0,    0,  870,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  900,
    0,    0,    0,  211,    0,  717,    0,    0,   81,    0,
  718,    0,    0,  901,    0,    0,    0,    0,  870,  870,
  211,  870,    0,    0,    0,    0,    0,    0,  870,  870,
  870,  870,  870,    0,  870,    0,    0,  870,  870,  870,
  870,  870,  211,    0,    0,  870,  870,    0,    0,  902,
  870,  870,  870,  870,  870,  870,    0,  903,    0,    0,
    0,    0,    0,    0,    0,  904,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  211,  211,    0,
    0,    0,    0,  905,    0,  906,    0,    0,  869,    0,
  869,  869,    0,    0,  211,    0,    0,  869,  907,    0,
  908,  909,  910,    0,    0,    0,    0,    0,    0,    0,
    0,  869,    0,    0,    0,  871,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  869,    0,    0,    0,    0,
  869,  869,    0,    0,  869,    0,    0,    0,    0,    0,
    0,  869,    0,  869,  869,    0,    0,  869,    0,  911,
    0,  869,    0,  869,    0,  869,  871,  869,  871,  871,
  871,    0,  869,  869,    0,  869,  881,    0,    0,  609,
    0,    0,    0,    0,  871,  871,  871,  871,    0,    0,
  869,  869,  882,  869,    0,    0,  610,    0,    0,  611,
    0,    0,    0,    0,  869,    0,  869,    0,  869,  869,
  869,    0,  869,    0,    0,  869,  869,    0,  871,    0,
    0,    0,    0,    0,    0,  869,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  869,  612,
    0,  869,    0,    0,    0,  613,  614,  615,    0,  616,
  617,  618,    0,  619,    0,    0,  912,  869,    0,    0,
    0,    0,    0,  869,  869,    0,    0,    0,  869,    0,
    0,  620,    0,  621,    0,    0,    0,    0,    0,  869,
  869,    0,  869,    0,    0,    0,    0,    0,  869,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  622,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  869,  869,  623,
  869,    0,    0,    0,    0,    0,    0,  869,  869,  869,
  869,  869,    0,  869,    0,    0,  869,  869,  869,  869,
  869,  624,    0,    0,  869,  869,    0,    0,    0,  869,
  869,  869,  869,  869,  869,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  625,  626,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  871,    0,  871,
  871,    0,    0,  627,    0,    0,  871,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  871,    0,    0,    0,  874,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  871,    0,    0,    0,    0,  871,
  871,    0,    0,  871,    0,    0,    0,    0,    0,    0,
  871,    0,  871,  871,    0,    0,  871,    0,    0,    0,
  871,    0,  871,    0,  871,  874,  871,  874,  874,  874,
    0,  871,  871,    0,  871,  211,    0,    0,  211,    0,
    0,    0,    0,  874,  874,  874,  874,    0,    0,  871,
  871,  211,  871,    0,    0,  211,    0,    0,  211,    0,
    0,    0,    0,  871,    0,  871,    0,  871,  871,  871,
    0,  871,    0,    0,  871,  871,    0,  874,    0,    0,
    0,    0,    0,    0,  871,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  871,  211,    0,
  871,    0,    0,    0,  211,  211,  211,    0,  211,  211,
  211,    0,  211,    0,    0,    0,  871,    0,    0,    0,
    0,    0,  871,  871,    0,    0,    0,  871,    0,    0,
  211,    0,  211,    0,    0,    0,    0,    0,  871,  871,
    0,  871,    0,    0,    0,    0,    0,  871,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  211,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  871,  871,  211,  871,
    0,    0,    0,    0,    0,    0,  871,  871,  871,  871,
  871,    0,  871,    0,    0,  871,  871,  871,  871,  871,
  211,    0,    0,  871,  871,    0,    0,    0,  871,  871,
  871,  871,  871,  871,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  851,  211,  211,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  874,    0,  874,  874,
    0,    0,  211,    0,    0,  874,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  978,    0,    0,    0,  874,
    0,    0,    0,    0,    0,  851, 1018, 1018,  851, 1018,
 1018, 1018,    0,  874,    0,    0,    0,    0,  874,  874,
    0,    0,  874,  851, 1018, 1018, 1018,    0,    0,  874,
    0,  874,  874,    0,    0,  874,    0,    0,    0,  874,
    0,  874,    0,  874,    0,  874,    0,    0,  679,    0,
  874,  874,    0,  874,    0, 1018,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  979,    0,    0,  874,  874,
    0,  874,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  874,    0,  874,    0,  874,  874,  874,  679,
  874,    0,  679,  874,  874,    0,    0,    0,    0,    0,
  980,    0,    0,  874,    0,  981,    0,  679,    0,    0,
    0,  982,    0,    0,    0,    0,  874,    0,    0,  874,
    0,    0,  109,    0,    0,    0,    0,    0,    0,    0,
  983,    0,    0,    0,    0,  874,    0,    0,    0,    0,
    0,  874,  874,    0,    0,    0,  874,    0,    0,    0,
    0,  110,  111,    0,  112,    0,    0,  874,  874,    0,
  874,  113,  114,    0,  115,  116,  874,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0,  874,  874,    0,  874,    0,
    0,    0,    0,    0,    0,  874,  874,  874,  874,  874,
    0,  874,    0,  852,  874,  874,  874,  874,  874,    0,
    0,    0,  874,  874,    0,    0,  851,  874,  874,  874,
  874,  874,  874,    0,    0, 1018,    0,    0,    0,    0,
    0, 1018,    0,    0,    0,    0, 1018,    0,    0, 1018,
    0,    0,    0,    0,  852, 1019, 1019,  852, 1019, 1019,
 1019,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  852, 1019, 1019, 1019,    0,    0,    0,  851,
    0,    0,  851,    0,    0, 1018,    0,    0,    0,  851,
    0,  851,    0, 1018,    0,  851,    0,    0,    0,    0,
  851, 1018,    0,  851, 1019,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  851, 1018,
    0, 1018,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  851,    0, 1018,    0, 1018, 1018, 1018,    0,
  851,    0,    0,  851,  851,    0,    0,    0,    0,    0,
    0,    0,    0,  851,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  679,    0,    0,  679,    0,    0,  851,
    0,    0,    0,  679,    0,  679,    0,    0,    0,  679,
    0,    0,    0,    0,  679, 1018,    0,  679,    0,    0,
    0,  851,    0,    0,    0,    0,  851,    0,    0,    0,
    0,    0,  679,    0,    0,    0,    0,    0,  851,    0,
  851,    0,    0,    0,    0,    0,  679,    0,    0,    0,
    0,    0,    0,    0,  679,    0,  853,    0,  679,    0,
    0,    0,    0,    0,    0,    0,    0,  679,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  679,    0,    0,    0,  851,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  853, 1020, 1020,
  853, 1020, 1020, 1020,    0,  852,    0,  851,    0,    0,
  679,  851, 1018,    0, 1019,  853, 1020, 1020, 1020,  682,
 1019,    0,  679,    0,  679, 1019,    0,    0, 1019,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1020,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  852,    0,
  682,  852,    0,  682, 1019,    0,    0,    0,  852,    0,
  852,  679, 1019,    0,  852,    0,    0,    0,  682,  852,
 1019,    0,  852,    0,    0,    0,    0,    0,    0,    0,
    0,  679,    0,    0,    0,  679,    0,  852, 1019,    0,
 1019,    0,    0,    0,  109,    0,    0,    0,    0,    0,
    0,  852,    0, 1019,    0, 1019, 1019, 1019,    0,  852,
    0,    0,  852,  852,    0,    0,    0,    0,    0,    0,
    0,    0,  852,  110,  111,    0,  112,    0,    0,    0,
    0,    0,    0,  113,  114,    0,  115,  116,  852,  117,
    0,    0,  118,  119,  120,  121,  122,    0,    0,    0,
  123,  124,    0,  810, 1019,    0,  125,  126,  127,    0,
  852,    0,    0,    0,    0,  852,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  852,    0,  852,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  810,    0,    0,  810,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  853,    0,
    0,    0,  810,  835,    0,    0,    0, 1020,    0,    0,
    0,    0,    0, 1020,    0,    0,  852,    0, 1020,    0,
    0, 1020,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  852,    0,    0,    0,
  852, 1019,    0,    0,  835,    0,    0,  835,    0,    0,
    0,  853,    0,    0,  853,    0,    0, 1020,    0,    0,
    0,  853,  835,  853,    0, 1020,    0,  853,    0,    0,
    0,    0,  853, 1020,    0,  853,    0,    0,  813,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  853, 1020,    0, 1020,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  853,    0, 1020,    0, 1020, 1020,
 1020,    0,  853,    0,  682,  853,  853,  682,    0,  813,
    0,    0,  813,    0,  682,  853,  682,    0,    0,    0,
  682,    0,    0,    0,    0,  682,    0,  813,  682,    0,
    0,  853,    0,    0,    0,    0,    0,    0,    0,    0,
  811,    0,    0,  682,    0,    0,    0, 1020,    0,    0,
    0,    0,    0,  853,    0,    0,    0,  682,  853,    0,
    0,  570,    0,    0,    0,  682,    0,    0,    0,  682,
  853,    0,  853,    0,    0,    0,    0,    0,  682,    0,
    0,  811,    0,    0,  811,    0,    0,    0,    0,    0,
  570,  570,    0,  570,  682,  810,    0,    0,    0,  811,
  570,  570,    0,  570,  570,    0,  570,    0,    0,  570,
  570,  570,  570,  570,    0,  812,    0,  570,  570,  853,
    0,  682,    0,  570,  570,  570,    0,    0,    0,    0,
    0,    0,    0,  682,    0,  682,    0,    0,    0,  853,
    0,  655,    0,  853, 1020,    0,    0,    0,  810,    0,
    0,  810,    0,    0,    0,  835,  812,    0,  810,  812,
  810,    0,    0,    0,  810,    0,    0,    0,    0,  810,
  655,  655,  810,  655,  812,  843,    0,    0,    0,    0,
  655,  655,  682,  655,  655,    0,  655,  810,    0,  655,
  655,  655,  655,  655,    0,    0,    0,  655,  655,    0,
    0,  810,  682,  655,  655,  655,  682,    0,  835,  810,
    0,  835,  810,  810,    0,    0,  843,    0,  835,  843,
  835,    0,  810,    0,  835,    0,    0,    0,    0,  835,
  813,    0,  835,    0,  843,    0,    0,    0,  810,    0,
    0,    0,    0,    0,    0,    0,    0,  835,    0,    0,
  814,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  810,  835,    0,    0,    0,  810,    0,    0,    0,  835,
    0,    0,  835,  835,    0,    0,    0,  810,    0,  810,
    0,    0,  835,  813,    0,    0,  813,    0,    0,    0,
    0,  814,    0,  813,  814,  813,    0,    0,  835,  813,
    0,    0,  811,    0,  813,    0,    0,  813,    0,  814,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  835,    0,  813,    0,    0,  835,  810,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  813,  835,    0,  835,
    0,    0,    0,    0,  813,    0,  810,  813,  813,    0,
  810,    0,    0,    0,    0,  811,    0,  813,  811,    0,
    0,    0,    0,    0,    0,  811,    0,  811,    0,    0,
    0,  811,    0,  813,    0,    0,  811,  812,    0,  811,
  806,    0,    0,    0,    0,    0,  835,    0,    0,    0,
    0,    0,    0,    0,  811,  813,    0,    0,    0,    0,
  813,    0,    0,    0,    0,    0,  835,    0,  811,    0,
  835,  807,  813,    0,  813,    0,  811,    0,    0,  811,
  811,  806,    0,    0,  806,    0,    0,    0,    0,  811,
  812,    0,    0,  812,    0,    0,    0,  843,    0,  806,
  812,    0,  812,    0,    0,  811,  812,    0,    0,    0,
    0,  812,  807,    0,  812,  807,    0,    0,    0,    0,
    0,  813,    0,    0,    0,    0,    0,  811,    0,  812,
  807,    0,  811,    0,    0,    0,    0,    0,    0,    0,
    0,  813,    0,  812,  811,  813,  811,    0,    0,    0,
  843,  812,    0,  843,  812,  812,    0,    0,    0,    0,
  843,    0,  843,    0,  812,    0,  843,    0,    0,    0,
    0,  843,  814,    0,  843,    0,    0,    0,    0,    0,
  812,    0,    0,    0,    0,    0,    0,    0,    0,  843,
    0,    0,    0,  811,    0,    0,    0,    0,    0,    0,
    0,  808,  812,  843,    0,    0,    0,  812,    0,    0,
    0,  843,    0,  811,  843,  843,    0,  811,    0,  812,
    0,  812,    0,    0,  843,  814,    0,    0,  814,    0,
    0,    0,    0,    0,    0,  814,    0,  814,    0,    0,
  843,  814,  808,    0,    0,  808,  814,    0,    0,  814,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  808,    0,  843,    0,  814,    0,    0,  843,  812,    0,
    0,    0,    0,    0,    0,    0,  842,    0,  814,  843,
    0,  843,    0,    0,    0,    0,  814,    0,  812,  814,
  814,    0,  812,    0,    0,    0,    0,    0,    0,  814,
    0,    0,  806,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  814,    0,  842,    0,    0,
  842,    0,    0,    0,    0,    0,    0,    0,  843,    0,
    0,    0,    0,  807,    0,  842,  836,  814,    0,    0,
    0,    0,  814,    0,    0,    0,    0,    0,  843,    0,
    0,    0,  843,    0,  814,  806,  814,    0,  806,    0,
    0,    0,    0,    0,    0,  806,    0,  806,    0,    0,
    0,  806,    0,    0,    0,    0,  806,  836,    0,  806,
  836,    0,    0,    0,    0,    0,  807,    0,    0,  807,
    0,    0,    0,    0,  806,  836,  807,    0,  807,    0,
    0,    0,  807,  814,    0,    0,    0,  807,  806,    0,
  807,  844,    0,    0,    0,    0,  806,    0,    0,  806,
  806,    0,    0,  814,    0,  807,    0,  814,    0,  806,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  807,
    0,    0,    0,    0,    0,  806,    0,  807,    0,    0,
  807,  807,  844,    0,    0,  844,    0,    0,    0,    0,
  807,    0,    0,  808,    0,    0,    0,  806,    0,    0,
  844,    0,  806,    0,    0,    0,  807,    0,    0,    0,
    0,    0,    0,  845,  806,    0,  806,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  807,    0,
    0,    0,    0,  807,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  807,  808,  807,    0,  808,
    0,    0,    0,    0,  845,    0,  808,  845,  808,    0,
    0,    0,  808,  806,    0,    0,    0,  808,  842,    0,
  808,    0,  845,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  806,    0,  808,    0,  806,  833,    0,
    0,    0,    0,    0,  807,    0,    0,    0,    0,  808,
    0,    0,    0,    0,    0,    0,    0,  808,    0,    0,
  808,  808,    0,    0,  807,    0,    0,    0,  807,    0,
  808,  842,    0,    0,  842,    0,    0,    0,  836,  833,
    0,  842,  833,  842,    0,    0,  808,  842,    0,    0,
    0,    0,  842,    0,    0,  842,    0,  833,  837,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  808,    0,
  842,    0,    0,  808,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  842,  808,    0,  808,    0,    0,
    0,  836,  842,    0,  836,  842,  842,    0,    0,  837,
    0,  836,  837,  836,    0,  842,    0,  836,    0,    0,
    0,    0,  836,  844,    0,  836,    0,  837,    0,    0,
    0,  842,    0,    0,    0,    0,    0,    0,    0,    0,
  836,    0,    0,  846,  808,    0,    0,    0,    0,    0,
    0,    0,    0,  842,  836,    0,    0,    0,  842,    0,
    0,    0,  836,    0,  808,  836,  836,    0,  808,    0,
  842,    0,  842,    0,    0,  836,  844,    0,    0,  844,
    0,    0,    0,    0,  846,    0,  844,  846,  844,    0,
    0,  836,  844,    0,    0,  845,    0,  844,    0,    0,
  844,    0,  846,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  836,    0,  844,    0,    0,  836,  842,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  844,
  836,    0,  836,    0,    0,    0,    0,  844,    0,  842,
  844,  844,    0,  842,    0,    0,    0,    0,  845,    0,
  844,  845,    0,    0,    0,    0,    0,    0,  845,    0,
  845,    0,    0,    0,  845,    0,  844,    0,    0,  845,
  833,    0,  845,  834,    0,    0,    0,    0,    0,  836,
    0,    0,    0,    0,    0,    0,    0,  845,  844,    0,
    0,    0,    0,  844,    0,    0,    0,    0,    0,  836,
    0,  845,    0,  836,  838,  844,    0,  844,    0,  845,
    0,    0,  845,  845,  834,    0,    0,  834,    0,    0,
    0,    0,  845,  833,    0,    0,  833,    0,    0,    0,
  837,    0,  834,  833,    0,  833,    0,    0,  845,  833,
    0,    0,    0,    0,  833,  838,    0,  833,  838,    0,
    0,    0,    0,    0,  844,    0,    0,    0,    0,    0,
  845,    0,  833,  838,    0,  845,    0,    0,    0,    0,
    0,    0,    0,    0,  844,    0,  833,  845,  844,  845,
    0,    0,    0,  837,  833,    0,  837,  833,  833,    0,
    0,    0,    0,  837,    0,  837,    0,  833,    0,  837,
    0,    0,    0,    0,  837,  846,    0,  837,    0,    0,
    0,    0,    0,  833,    0,    0,    0,    0,    0,    0,
    0,    0,  837,    0,    0,    0,  845,    0,    0,    0,
    0,    0,    0,  102,    0,  833,  837,    0,    0,    0,
  833,    0,    0,    0,  837,    0,  845,  837,  837,    0,
  845,    0,  833,    0,  833,    0,    0,  837,  846,    0,
    0,  846,    0,    0,    0,    0,    0,    0,  846,    0,
  846,  103,    0,  837,  846,    0,    0,    0,    0,  846,
    0,  672,  846,  104,  441,    0,  442,    0,  280,    0,
    0,    0,    0,    0,    0,  837,    0,  846,    0,  444,
  837,  833,    0,    0,  295,    0,    0,  105,    0,    0,
    0,  846,  837,    0,  837,    0,    0,  280,  280,  846,
  280,  833,  846,  846,    0,  833,    0,  280,  280,    0,
  280,  280,  846,  280,    0,  834,  280,  280,  280,  280,
  280,    0,    0,    0,  280,  280,    0,    0,  846,    0,
  280,  280,  280,    0,    0,    0,    0,    0,    0,    0,
    0,  837,    0,    0,    0,    0,  838,    0,    0,    0,
  846,    0,  106,    0,    0,  846,    0,    0,    0,    0,
    0,  837,  107,    0,    0,  837,    0,  846,  834,  846,
    0,  834,    0,    0,    0,    0,    0,    0,  834,    0,
  834,    0,    0,    0,  834,    0,    0,  108,    0,  834,
    0,    0,  834,    0,    0,    0,    0,    0,    0,  838,
    0,    0,  838,    0,    0,  109,    0,  834,    0,  838,
    0,  838,    0,    0,    0,  838,  846,    0,    0,    0,
  838,  834,    0,  838,    0,    0,    0,    0,    0,  834,
    0,    0,  834,  834,  110,  111,  846,  112,  838,    0,
  846,    0,  834,    0,  113,  114,    0,  115,  116,    0,
  117,    0,  838,  118,  119,  120,  121,  122,  834,    0,
  838,  123,  124,  838,  838,    0,    0,  125,  126,  127,
    0,    0,    0,  838,    0,    0,    0,    0,    0,    0,
  834,    0,    0,    0,    0,  834,    0,    0,  404,  838,
    0,    0,    0,    0,    0,  405,    0,  834,    0,  834,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,  838,    0,  407,  408,  672,  838,    0,  441,    0,
  442,    0,    0,    0,    0,    0,    0,    0,  838,    0,
  838,    0,  666,  444,    0,    0,    0,    0,  295,    0,
    0,  409,    0,    0,  410,    0,  834,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  834,    0,    0,  411,
  834,  412,  413,    0,    0,    0,    0,  838,    0,    0,
    0,  414,    0,    0,    0,  415,    0,    0,  986,    0,
    0,  986,    0,  892,    0,    0,    0,  838,    0,    0,
    0,  838,    0,    0,    0,    0,  988,    0,  986,    0,
    0,  986,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,    0,  668,    0,    0,    0,    0,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,  986,    0,    0,    0,    0,  418,  986,  986,  986,
    0,  986,  986,  986,    0,  986,    0,    0,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,    0,    0,  986,  423,  986,    0,    0,    0,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,  986,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,  893,  894,  895,  123,
  124,  986,    0,    0,    0,  125,  440,  127,    0,    0,
    0,    0,  404,  608,    0,    0,  609,  672,    0,  405,
  441,    0,  442,  986,    0,    0,    0,    0,    0,    0,
    0,  406,    0,  610,  665,  444,  611,  407,  408,    0,
  295,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  666,    0,  986,  986,
    0,    0,    0,    0,    0,  409,    0,    0,  410,    0,
    0,    0,    0,    0,    0,  986,  612,    0,    0,    0,
    0,    0,  613,  614,  615,    0,  616,  617,  618,    0,
  619,    0,    0,  411,    0,  412,  413,    0,    0,    0,
    0,    0,    0,    0,    0,  414,    0,    0,  620,  415,
  621,    0,  141,    0,    0,  141,    0,  667,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  102,
    0,    0,  141,    0,    0,  141,    0,    0,    0,  622,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  623,    0,  668,    0,
    0,    0,    0,    0,  416,    0,    0,  103,    0,    0,
  417,    0,    0,    0,    0,  141,    0,    0,  624,  104,
  418,  141,  141,  141,    0,  141,  141,  141,    0,  141,
    0,    0,    0,  140,  419,  420,  421,  422,  218,  219,
    0,    0,    0,  105,    0,    0,    0,  141,  423,  141,
    0,    0,    0,  625,  626,    0,    0,  424,  425,  426,
  427,  428,  429,  430,  431,  432,  110,  433,    0,  112,
  627,    0,  434,  435,    0,  436,  437,  438,  141,  115,
  116,  439,  117,    0,  404,  118,  119,  669,  670,  671,
    0,  405,    0,  123,  124,  141,    0,    0,    0,  125,
  440,  127,  672,  406,    0,  441,    0,  442,  106,  407,
  408,    0,    0,    0,    0,    0,    0,  141,  107,    0,
  444,    0,    0,    0,    0,  295,    0,    0,  666,    0,
    0,    0,    0,    0,    0,    0,    0,  409,    0,    0,
  410,    0,    0,  108,    0,    0,    0,    0,    0,    0,
    0,    0,  141,  141,    0,    0,    0,    0,    0,    0,
    0,  109,    0,    0,    0,  411,    0,  412,  413,  141,
    0,    0,    0,    0,    0,    0,    0,  414,    0,    0,
    0,  415,    0,    0,    0,    0,    0,    0,    0,  888,
  110,  111,    0,  112,    0,    0,    0,    0,    0,    0,
  113,  114,    0,  115,  116,    0,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  126,  127,    0,    0,    0,    0,
  668,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  419,  420,  421,  422,
  218,  219,    0,    0,    0,    0,    0,    0,    0,    0,
  423,    0,    0,    0,    0,    0,    0,    0,    0,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,    0,  434,  435,    0,  436,  437,  438,
    0,  115,  116,  439,  117,    0,    0,  118,  119,  669,
  670,  671,    0,    0,    0,  123,  124,    0,    0,  404,
    0,  125,  440,  127,  672,    0,  405,  441,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,    0,  444,    0,  407,  408,    0,  295,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  666,    0,    0,    0,    0,    0,    0,
    0,    0,  409,    0,    0,  410,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  411,    0,  412,  413,    0,    0,    0,    0,    0,    0,
    0,    0,  414,    0,    0,    0,  415,    0,    0,    0,
    0,    0,    0,    0,  667,    0,    0,    0,    0,    0,
    0,  511,    0,    0,  441,    0,  442,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  444,
    0,    0,    0,    0,  295,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  668,    0,    0,    0,    0,
    0,  416,    0,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  419,  420,  421,  422,  218,  219,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,    0,    0,
    0,    0,    0,    0,  424,  425,  426,  427,  428,  429,
  430,  431,  432,  110,  433,    0,  112,    0,    0,  434,
  435,    0,  436,  437,  438,    0,  115,  116,  439,  117,
    0,  404,  118,  119,  669,  670,  671,    0,  405,    0,
  123,  124,    0,    0,    0,    0,  125,  440,  127,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  666,    0,    0,    0,    0,
    0,    0,    0,    0,  409,    0,    0,  410,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,  412,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
    0, 1137,    0,    0, 1138,    0,  888,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,  511,
    0,    0,  441,    0,  442,    0,    0,  668,    0,    0,
    0,    0,    0,  416,    0,    0,    0,  444,    0,  417,
    0,  409,  295,    0,  410,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,  411,
    0,  412,  413,    0,    0,    0,    0,  423,    0,    0,
    0,  414,    0,    0,    0,  415,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124, 1214,    0,    0,    0,  125,  440,
  127,    0,    0,    0,    0,    0,    0,    0, 1139,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    6,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0, 1411, 1412,    0, 1413,    0,    0,    0,
  419,  420,  421,  422,  218,  219,    0,    0,    0,    0,
    0,    0, 1414,    0,  423,    0,    0,    0, 1415,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,    0,  434,  435,
    0,  436,  437,  438,    0,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,    0,    0,    0,    0,  125,  440,  127,    0, 1147,
    0,    0, 1138,    0,    0, 1416,  404,    0,    0,    0,
    0,    0,    0,  405,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   14,    0,  406,    0,    0,    0,    0,
    0,  407,  408,    0,    0,    0,    0,  511, 1417,    0,
  441,    0,  442,    0,    0,    0,    0,    0,   16,    0,
    0,    0,    0,    0,    0,  444,    0, 1605,    0,  409,
  295,    0,  410,    0,    0,    0,  109,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,    0,  412,
  413,    0,    0,    0,    0, 1418,  111,    0,  112,  414,
    0,    0,    0,  415,    0,  113,  114,    0,  115,  116,
    0,  117,    0,    0,  118, 1419,  120,  121,  122,    0,
    0,    0,  123,  124, 1420, 1421, 1422,    0,  125,  126,
  127,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  511,    0,    0,  441,    0,  442,
    0,    0,    0,    0,    0,    0, 1139,    0,  416,    0,
    0,    0,  444,    0,  417,    0,    0,  295,    0,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  419,  420,
  421,  422,  218,  219,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  440,  127,    0, 1151,    0,    0,
 1138,    0,    0,    0,  404,    0,    0,    0,    0,    0,
    0,  405,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  407,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  409,    0,    0,
  410,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  411,    0,  412,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,    0,    0,
    0,  415,    0,    0, 1159,    0,    0, 1138,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,  405,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  406,    0,    0,    0,    0,    0,  407,  408,    0,    0,
    0,    0,  511,    0,    0,  441,    0,  442,    0,    0,
    0,    0,    0,    0, 1139,    0,  416,    0,    0,    0,
  444,    0,  417,    0,  409,  295,    0,  410,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  419,  420,  421,  422,
  218,  219,  411,    0,  412,  413,    0,    0,    0,    0,
  423,    0,    0,    0,  414,    0,    0,    0,  415,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,    0,  434,  435,    0,  436,  437,  438,
    0,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,    0,    0,    0,  123,  124, 1214,    0,    0,
    0,  125,  440,  127,    0,    0,    0,    0,    0,    0,
    0, 1139,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    6,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0, 1411, 1412,    0, 1413,
    0,    0,    0,  419,  420,  421,  422,  218,  219,    0,
    0,    0,    0,    0,    0, 1414,    0,  423,    0,    0,
    0, 1415,    0,    0,    0,    0,  424,  425,  426,  427,
  428,  429,  430,  431,  432,  110,  433,    0,  112,    0,
    0,  434,  435,    0,  436,  437,  438,    0,  115,  116,
  439,  117,    0,    0,  118,  119,  120,  121,  122,    0,
    0,    0,  123,  124,    0,    0,    0,    0,  125,  440,
  127,    0, 1162,    0,    0, 1138,    0,    0, 1416,  404,
    0,    0,    0,    0,    0,    0,  405,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   14,    0,  406,    0,
    0,    0,    0,    0,  407,  408,    0,    0,    0,    0,
  511, 1417,    0,  441,    0,  442,    0,    0,    0,    0,
    0,   16,    0,    0,    0,    0,    0,    0,  444,    0,
    0,    0,  409,  295,    0,  410,    0,    0,    0,  109,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  411,    0,  412,  413,    0,    0,    0,    0, 1418,  111,
    0,  112,  414,    0,    0,    0,  415,    0,  113,  114,
    0,  115,  116,    0,  117,    0,    0,  118, 1419,  120,
  121,  122,    0,    0,    0,  123,  124, 1420, 1421, 1422,
    0,  125,  126,  127,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  511,    0,    0,
  441,    0,  442,    0,    0,    0,    0,    0,    0, 1139,
    0,  416,    0,    0,    0,  444,    0,  417,    0,    0,
  295,    0,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  419,  420,  421,  422,  218,  219,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,    0,    0,
    0,    0,    0,    0,  424,  425,  426,  427,  428,  429,
  430,  431,  432,  110,  433,    0,  112,    0,    0,  434,
  435,    0,  436,  437,  438,    0,  115,  116,  439,  117,
    0,    0,  118,  119,  120,  121,  122,    0,    0,    0,
  123,  124,    0,    0,    0,    0,  125,  440,  127,    0,
 1168,    0,    0, 1138,    0,    0,    0,  404,    0,    0,
  537,    0,    0,    0,  405,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  406,    0,    0,    0,
    0,    0,  407,  408,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  537,  537,    0,    0,  537,    0,    0,    0,    0,    0,
  409,    0,    0,  410,    0,    0,    0,    0,    0,  537,
    0,  537,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  411,    0,
  412,  413,    0,    0,    0,    0,    0,    0,    0,    0,
  414,  537,    0,    0,  415,    0,    0, 1171,    0,    0,
 1138,    0,    0,    0,  404,    0,    0,    0,    0,    0,
    0,  405,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  407,
  408,    0,    0,    0,    0,  511,    0,    0,  441,    0,
  442,    0,    0,    0,    0,    0,    0, 1139,    0,  416,
    0,    0,    0,  444,    0,  417,    0,  409,  295,    0,
  410,    0,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  419,
  420,  421,  422,  218,  219,  411,    0,  412,  413,    0,
    0,    0,    0,  423,    0,    0,    0,  414,    0,    0,
    0,  415,  424,  425,  426,  427,  428,  429,  430,  431,
  432,  110,  433,    0,  112,    0,    0,  434,  435,    0,
  436,  437,  438,    0,  115,  116,  439,  117,    0,    0,
  118,  119,  120,  121,  122,    0,    0,    0,  123,  124,
    0,    0,    0,    0,  125,  440,  127,    0,    0,    0,
    0,    0,    0,    0, 1139,    0,  416,    0,  537,    0,
    0,    0,  417,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  418,  537,    0,  537,  537,  537,    0,    0,
    0,  537,    0,    0,  537,    0,  419,  420,  421,  422,
  218,  219,    0,    0,    0,    0,    0,  537,    0,    0,
  423,    0,    0,  537,    0,    0,  537,    0,    0,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,    0,  434,  435,    0,  436,  437,  438,
    0,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,    0,    0,    0,  123,  124,    0,  537,    0,
    0,  125,  440,  127,    0, 1174,    0,    0, 1138,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,  405,
    0,    0,  537,  639,    0,  639,  639,    0,  639,    0,
    0,  406,    0,    0,    0,    0,    0,  407,  408,    0,
    0,  639,    0,  537,    0,    0,  639,    0,    0,  537,
    0,    0,    0,  537,    0,    0,    0,    0,    0,    0,
  537,    0,    0,    0,    0,  409,    0,    0,  410,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  537,    0,
    0,    0,    0,  537,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  411,    0,  412,  413,    0,    0,    0,
    0,    0,    0,    0,    0,  414,    0,    0,  537,  415,
    0,    0,  537,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  633,    0,  633,  633,    0,  633,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  633,    0,
    0,    0,    0,  633,    0,    0,    0,    0,    0,    0,
    0,    0, 1139,    0,  416,    0,    0,    0,    0,    0,
  417,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  419,  420,  421,  422,  218,  219,
    0,    0,    0,    0,    0,    0,    0,    0,  423,    0,
    0,    0,    0,    0,    0,    0,    0,  424,  425,  426,
  427,  428,  429,  430,  431,  432,  110,  433,    0,  112,
    0,    0,  434,  435,    0,  436,  437,  438,    0,  115,
  116,  439,  117,  639,    0,  118,  119,  120,  121,  122,
  639,    0,    0,  123,  124,    0,    0,  639,    0,  125,
  440,  127,    0,    0,    0,    0,    0,    0,    0,  639,
    0,    0,    0,    0,    0,  639,  639,    0,    0,    0,
    0,    0,    0,  639,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  639,    0,    0,  639,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  639,    0,  639,  639,    0,    0,    0,    0,    0,
    0,    0,    0,  639,    0,    0,    0,  639,    0,  511,
  633,  516,  441,    0,  442,    0,    0,  633,    0,    0,
    0,    0,    0,    0,  633,    0,    0,  444,    0,    0,
    0,    0,  295,    0,    0,    0,  633,    0,    0,    0,
    0,    0,  633,  633,    0,    0,    0,    0,    0,    0,
  633,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  639,    0,    0,    0,    0,    0,  639,    0,
  633,    0,    0,  633,    0,    0,    0,    0,  639,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  639,  639,  639,  639,  639,  639,  633,    0,
  633,  633,    0,    0,    0,    0,  639,    0,    0,    0,
  633,    0,    0,    0,  633,  639,  639,  639,  639,  639,
  639,  639,  639,  639,  639,  639,    0,  639,    0,    0,
  639,  639,    0,  639,  639,  639,    0,  639,  639,  639,
  639,    0,    0,  639,  639,  639,  639,  639,    0,    0,
    0,  639,  639,    0,    0,    0,    0,  639,  639,  639,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  633,
    0,    0,    0,    0,    0,  633,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  633,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  633,
  633,  633,  633,  633,  633,    0,    0,    0,    0,    0,
    0,    0,    0,  633,    0,    0,    0,    0,    0,    0,
    0,    0,  633,  633,  633,  633,  633,  633,  633,  633,
  633,  633,  633,    0,  633,    0,  404,  633,  633,    0,
  633,  633,  633,  405,  633,  633,  633,  633,    0,    0,
  633,  633,  633,  633,  633,  406,    0,    0,  633,  633,
    0,  407,  408,    0,  633,  633,  633,  511,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  419,  420,
  421,  422,  218,  219,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  110,  433,    0,  112,    0,    0,  434,  435,    0,  436,
  437,  438,    0,  115,  116,  439,  117,    0,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,    0,
    0,    0,    0,  125,  440,  127,  511,    0,    0,  441,
    0,  442,    0,    0,  404,    0,    0,    0,    0,    0,
    0,  405,    0,    0,  444,    0,    0,    0,    0,  295,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  407,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  409,    0,    0,
  410,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  411,    0,  412,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,    0,    0,
    0,  415,    0,  511,  955,    0,  441,    0,  442,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  444,    0,    0,    0,    0,  295,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
  701,    0,    0,    0,    0,    0,  419,  420,  421,  422,
  218,  219,    0,    0,    0,    0,    0,    0,    0,    0,
  423,    0,    0,    0,    0,    0,    0,    0,    0,  424,
  425,  426,  427,  428,  429,  430,  431,  432,  110,  433,
    0,  112,    0,  404,  434,  435,    0,  436,  437,  438,
  405,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,  406,    0,    0,  123,  124,    0,  407,  408,
    0,  125,  440,  127,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  409,    0,    0,  410,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  411,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,  511,    0,    0,  441,    0,  442,    0,    0,
  404,    0,    0,    0,    0,    0,    0,  405,    0,    0,
  444,    0,    0,    0,    0,  295,    0,    0,    0,  406,
    0,    0,    0,    0,    0,  407,  408,    0,    0,    0,
    0,    0,    0,    0,    0,   14,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,  409,    0,    0,  410,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,  420,  421,  422,  218,
  219,  411,    0,  412,  413,    0,    0,    0,    0,  423,
    0,    0,    0,  414,    0,    0,    0,  415,  424,  425,
  426,  427,  428,  429,  430,  431,  432,  110,  433,    0,
  112,    0,    0,  434,  435,    0,  436,  437,  438,    0,
  115,  116,  439,  117,    0,    0,  118,  119,  120,  121,
  122,    0,    0,    0,  123,  124,    0,    0,    0,    0,
  125,  440,  127,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  419,  420,  421,  422,  218,  219,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,    0,
    0,    0,    0,    0,    0,  424,  425,  426,  427,  428,
  429,  430,  431,  432,  110,  433,    0,  112,    0,  404,
  434,  435,    0,  436,  437,  438,  405,  115,  116,  439,
  117,    0,    0,  118,  119,  120,  121,  122,  406,    0,
    0,  123,  124,    0,  407,  408,    0,  125,  440,  127,
  511,    0,    0,  441,    0,  442,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  444,    0,
    0,    0,  409,  295,    0,  410,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  411,    0,  412,  413,    0,    0,    0,    0,    0,    0,
    0,    0,  414,    0,    0,    0,  415,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  511,    0,    0,
  441,    0,  442,    0,    0,    0,    0,    0,    0,    0,
    0,  416,    0,    0,    0,  444, 1699,  417,    0,    0,
  295,    0,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1165,    0,
    0,  419,  420,  421,  422,  218,  219,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,    0,    0,
    0,    0,    0,    0,  424,  425,  426,  427,  428,  429,
  430,  431,  432,  110,  433,    0,  112,    0,    0,  434,
  435,    0,  436,  437,  438,    0,  115,  116,  439,  117,
    0,    0,  118,  119,  120,  121,  122,    0,    0,    0,
  123,  124,    0,    0,    0,    0,  125,  440,  127,    0,
    0,    0,    0,    0,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,  405,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  406,    0,    0,    0,
    0,    0,  407,  408,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  409,    0,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  411,    0,
  412,  413,    0,    0,    0,    0,    0,    0,    0,    0,
  414,    0,    0,    0,  415,    0,  978,    0,  978,  978,
    0,  978,    0,    0,  404,    0,    0,    0,    0,    0,
    0,  405,    0,    0,  978,    0,    0,    0,    0,  978,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  407,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  416,
    0,    0,    0,    0,    0,  417,    0,  409,    0,    0,
  410,    0,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1367,    0,    0,  419,
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
    0,  112,    0,  978,  434,  435,    0,  436,  437,  438,
  978,  115,  116,  439,  117,    0,    0,  118,  119,  120,
  121,  122,  978,    0,    0,  123,  124,    0,  978,  978,
    0,  125,  440,  127,  443,    0,    0,  441,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  444,    0,    0,    0,  978,  295,    0,  978,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  978,    0,  978,  978,    0,    0,
    0,    0,    0,    0,    0,    0,  978,    0,    0,    0,
  978,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  511,    0,    0,  441,    0,  442,    0,    0,    0,
    0,    0,    0,    0,    0,  978,    0,    0,    0,  444,
    0,  978,    0,    0,  295,    0,    0,    0,    0,    0,
    0,  978,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  978,  978,  978,  978,  978,
  978,    0,    0,    0,    0,    0,    0,    0,    0,  978,
    0,    0,    0,    0,    0,    0,    0,    0,  978,  978,
  978,  978,  978,  978,  978,  978,  978,  978,  978,    0,
  978,    0,    0,  978,  978,    0,  978,  978,  978,    0,
  978,  978,  978,  978,    0,    0,  978,  978,  978,  978,
  978,    0,    0,    0,  978,  978,    0,    0,    0,    0,
  978,  978,  978,    0,    0,    0,    0,    0,    0,    0,
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
  978,    0,    0,  978,    0,  978,    0,    0,  404,    0,
    0,    0,    0,    0,    0,  405,    0,    0,  978,    0,
    0,    0,    0,  978,    0,    0,    0,  406,    0,    0,
    0,    0,    0,  407,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,  409,    0,    0,  410,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,  420,  421,  422,  218,  219,  411,
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
    0,    0,    0,    0,  423,    0,    0,  538,    0,    0,
    0,    0,    0,  424,  425,  426,  427,  428,  429,  430,
  431,  432,  110,  433,    0,  112,    0,  978,  434,  435,
    0,  436,  437,  438,  978,  115,  116,  439,  117,    0,
    0,  118,  119,  120,  121,  122,  978,  538,  538,  123,
  124,  538,  978,  978,    0,  125,  440,  127,    0,    0,
  532,    0,    0,    0,    0,    0,  538,    0,  538,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  978,    0,    0,  978,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  554,    0,  538,    0,
    0,  532,    0,    0,  532,    0,    0,    0,  978,    0,
  978,  978,    0,    0,    0,    0,    0,    0,    0,  532,
  978,  532,    0,    0,  978,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  554,    0,  544,
  554,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  532,    0,    0,    0,  554,    0,  554,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  978,
  544,    0,    0,  544,    0,  978,    0,  554,    0,    0,
    0,    0,    0,    0,    0,  978,    0,    0,  544,    0,
  544,    0,    0,    0,    0,    0,    0,    0,    0,  978,
  978,  978,  978,  978,  978,    0,    0,    0,    0,    0,
    0,    0,    0,  978,    0,    0,    0,    0,    0,    0,
  544,    0,  978,  978,  978,  978,  978,  978,  978,  978,
  978,  978,  978,    0,  978,    0,    0,  978,  978,    0,
  978,  978,  978,    0,  978,  978,  978,  978,    0,    0,
  978,  978,  978,  978,  978,  538,    0,  529,  978,  978,
    0,    0,    0,    0,  978,  978,  978,    0,    0,    0,
  538,    0,  538,  538,  538,    0,    0,    0,  538,    0,
    0,  538,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  538,    0,    0,    0,  529,    0,
  538,  529,    0,  538,    0,    0,    0,    0,  532,    0,
    0,    0,    0,    0,    0,    0,  529,    0,  529,    0,
    0,    0,    0,  532,    0,  532,  532,  532,    0,    0,
    0,  532,    0,    0,  532,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  554,  538,    0,  532,  529,    0,
    0,    0,    0,  532,    0,    0,  532,    0,    0,  554,
    0,    0,  554,  554,    0,    0,    0,  554,    0,  538,
  554,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  554,    0,    0,    0,  544,    0,  554,
  538,    0,  554,    0,    0,    0,  538,    0,  532,    0,
  538,    0,  544,    0,    0,  544,  544,  538,    0,    0,
  544,    0,    0,  544,    0,    0,    0,    0,    0,    0,
    0,    0,  532,    0,    0,  538,  544,    0,    0,    0,
  538,    0,  544,    0,  554,  544,    0,    0,    0,    0,
    0,    0,    0,  532,    0,    0,    0,    0,    0,  532,
    0,    0,    0,  532,    0,  538,    0,    0,  554,  538,
  532,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  544,  532,  554,
    0,    0,    0,  532,    0,  554,    0,    0,    0,  554,
    0,    0,    0,    0,    0,    0,  554,    0,    0,    0,
    0,  544,    0,    0,    0,    0,    0,    0,  532,    0,
    0,    0,  532,    0,  554,  529,    0,    0,    0,  554,
    0,    0,  544,    0,    0,    0,    0,    0,  544,    0,
  529,    0,  544,  529,  529,    0,    0,    0,  529,  544,
    0,  529,    0,  320,  554,    0,    0,    0,  554,    0,
    0,    0,    0,    0,  529,    0,    0,  544,    0,    0,
  529,    0,  544,  529,    0,    0,    0,    0,    0,    0,
    0,  320,    0,    0,    0,    0,    0,    0,  320,    0,
    0,    0,  320,  320,    0,  320,    0,  544,  320,    0,
    0,  544,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  320,    0,    0,    0,  529,    0,  320,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  529,
    0,    0,    0,    0,    0,  608,    0,    0,  609,    0,
    0,    0,    0,    0,  806,    0,    0,    0,    0,    0,
  529,    0,    0,    0,    0,  610,  529,    0,  611,    0,
  529,    0,    0,    0,  320,    0,    0,  529,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  320,    0,    0,  529,    0,    0,    0,    0,
  529,    0,    0,    0,    0,    0,    0,  320,  612,    0,
    0,    0,    0,    0,  613,  614,  615,  320,  616,  617,
  618,    0,  619,    0,    0,  529,  320,    0,  320,  529,
    0,    0,    0,    0,    0,  320,    0,    0,    0,    0,
  620,    0,  621,  608,    0,    0,  609,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  610,  320,  320,  611,  320,    0,    0,
    0,  622,    0,    0,  320,  320,    0,  320,  320,    0,
  320,    0,    0,  320,  320,  320,  320,  320,  623,    0,
    0,  320,  320,  320,  320,  320,    0,  320,  320,  320,
    0,    0,    0,    0,    0,    0,  612,    0,    0,    0,
  624,    0,  613,  614,  615,    0,  616,  617,  618,    0,
  619,    0,    0,    0,    0,  109,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  620,    0,
  621,    0,    0,    0,    0,  625,  626,    0,    0,    0,
    0,    0,    0,    0,  110,  111,    0,  112,    0,    0,
    0,    0,  627,    0,  113,  114,    0,  115,  116,  622,
  117,    0,    0,  118,  119,  120,  121,  122,    0,    0,
    0,  123,  124,    0,    0,    0,  623,  125,  126,  127,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  624,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  109,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  625,  626,    0,    0,    0,    0,    0,
    0,    0,  110,  111,    0,  112,    0,    0,    0,    0,
  627,    0,  113,  114,    0,  115,  116,    0,  117,    0,
    0,  118,  119,  120,  121,  122,    0,    0,    0,  123,
  124,  109,    0,    0,    0,  125,  126,  127,    0,    0,
  172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
  182,  183,  184,  185,  186,  187,    0,    0,  188,  189,
  110,  111,    0,  112,    0,    0,  190,  191,    0,    0,
  113,  114,  192,  115,  116,  193,  117,  194,    0,  118,
  119,  120,  121,  122,    0,    0,    0,  123,  124,  195,
  196,  197,  198,  125,  126,  127,  199,  458,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,  458,    0,    0,  458,  458,  458,  458,    0,  458,
    0,    0,  458,  458,    0,    0,  458,  458,  458,  458,
  458,  458,  458,  458,    0,  458,  458,  458,  458,  458,
    0,    0,    0,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,
};
static short yycheck[] = {                                       9,
   58,    0,   12,  156,    4,   16,   16,   67,   18,  108,
  546,   59,  146,  147,   66,  146,  147,  725,  382,   65,
  701,   66,   70,   16,  382,  378,  707,   94,   88,  578,
   71,  479,   76,   97,   92,  549,   73,  353,  374,  443,
  522,  712, 1058,  974, 1047,   93,   56,   57,   58,   59,
   60,   61,  100,   94,   98, 1058,   66,   67,   68,   96,
   70,  351,   72,   73,   91,   75,   76,   60,  953, 1286,
  778,   41,  500,  297,   84,   86,   86,   87,   88,   72,
   90,   91,   92,   93,   95,   95,   96,   97,   98,  280,
  100,  660,  137,   86,  139,  653,  160, 1213,  289,  732,
  499,    0, 1007,    0,  360,   44,  875,  326, 1110, 1087,
  158,   63,    0,   44, 1052,  359,   45,    0,   45,   44,
   41, 1059,   40,  571,  151,  573,    0,  137,  138,  139,
  578,  387, 1070, 1054,    0,   41,    0,   44,    0,   40,
 1047,  151,   41,  242,   41,  138,  156,   44,  158,  159,
  160, 1058,    0,    0,  164, 1093, 1047,  203,    0, 1047,
   59, 1047,   59,  785, 1047,   59,  159, 1058,  164,   44,
 1058,   59, 1058,  326,    0, 1058,   59,   41,  800,   41,
   44,  803,  228,  667, 1105,   59, 1478,   44, 1231,   44,
  812,   44,   44,   59,   41,   59,   45,   59,    0,   41,
    0,    0,   44,   44,  557,   40,  443,   40,   44,  233,
    0,   59,   59,  505,  578,   41,  750,   59,   42,   41,
  578,    0,   40,    0,   61,  280,  271,   45,   63,  274,
   63,   41,  331,   59,   44,  916,  917,   41,  315,   41,
  351,   41,   41,  750,   44,   44,  868,    0,  344,    0,
  308,    0,  605,   44,   44,  287,    0,   59,  317,   59,
   59,  271, 1278, 1279,  274,    0,  947,    0,  278,   59,
  280,  281, 1284,  395,  511, 1278, 1279,  287, 1777,  289,
   59,  326,   59,   45,    0,  541,   12,  273,   41, 1245,
   41,  581,   41,   93,  262,   44, 1312,  272,  308,  447,
  491,   63,  312,  327, 1420, 1421,   59,  352,   59, 1312,
   59,  268,  716,  275,  359,   59,  361,  260,  363,  312,
  724,  366,  484,    0,   59,   41,   59, 1619,   44,  375,
  317,  377,  273,  297,  257, 1834,  384,  395, 1259,  629,
 1318, 1246,  352,   59,  334,  391,  499,    0,  385,  359,
 1281,  361,  272,  363,  516,    0,  366,  300,    0,  498,
    0,  336,  348, 1301,   41, 1046,  395,   44,  445,  428,
  852, 1278, 1279,  484,  384,  385,  431,   93,  500,  341,
   59,  829,   59,  393,  348,  395,    0, 1278, 1279,  399,
 1278, 1279, 1278, 1279, 1325, 1278, 1279,   41,  315,  399,
  393,   41,  359,   61,   44, 1312,   59,  330,  892,  297,
 1032,  379,  264,  509,   59,   41,  315,   59,  144,   59,
   41, 1312,  815,  297, 1312, 1642, 1312, 1049, 1050, 1312,
 1224,  297, 1226,  539,  444,  672,  260,  336,   42,   43,
  315,   45,  490,   47,  434,   59,  301, 1069,  876, 1130,
 1131,  315, 1133, 1134,  499,  989,  344,  505,  379,  887,
  297,  753,  596,  891,  317,  301, 1000, 1001,  315,  387,
  344,  262,  336,  379,  336,  507,  300,  487,  344,  376,
  490,  491,  989, 1599,  306,  514,  352, 1499,  498,  336,
    0,    0,  502, 1000,  487,  505,   41,  507,  450,   44,
  510,  546,  306,  401,  794,  498, 1431,  797,  402,  502,
  336,  450, 1468,  559,  510,  446,  315,  458,  808,  444,
  386,  450,  388,  450, 1741,   41,  325,  410,  327, 1531,
  597,   41,  756,  578,  336,  376,  546,  336,  439,  446,
  339,  276,  277,  513,  410,  835, 1104,  446, 1117,   59,
   59,  742, 1483,  599, 1492, 1324,  597,  801, 1496, 1497,
  779,  444,  315,  879,  315, 1543,  315,  577,  578,  435,
 1787,  428, 1615,  583, 1479,    0, 1112, 1210,  327, 1007,
  428,  380,  446,  336,  446,  336, 1267,  336,  379,  298,
  389, 1512,  996,  403, 1532,  994,  493, 1814,  729,  446,
  344,  450,  402,  649,  650,  423, 1034,  652, 1289, 1530,
  866,  344,    0,  258,  513,  450,   41,  450,  262,  643,
  446,  509,  264,  402,  401,  270,  779,   41,  477,  380,
  764,  380,  450,  432,   59,  509,  262,  372,  315,  297,
  389,  262,  652,  509,  446,  444,  258,  446,  325,  513,
  327,  493,  297,   41,  444,  761,   41,  397,  303,  336,
  305,  423,  339,  261, 1549,  692,  513,  450,   91, 1062,
 1063,   59, 1466,  277,    0,  315,  416,    0,  282, 1695,
 1534,  432,  692,  432, 1355,  730,  402,  732,  450,  734,
 1628,  303, 1695,  446, 1632,  446,  336,  446,  297,  344,
    0, 1323,  380,  380, 1040,  753, 1240,  717,  361, 1647,
  304,  305,  389,    0,  513,  725,    0,    0,   41, 1112,
  730,   44,  732, 1654,  734,  434, 1664,  737,  273, 1660,
    0,  450,  742,   59,  779,  379,   59, 1853, 1142,  784,
  750,   41,  262,  753,  790,  344,  262,  361,  390,  759,
  760,  298,  409,  379,  432,  432,  801,   41,  379,   59,
  827,  306,  513,  773,  513,  775, 1056,  444,  778,  446,
 1106,  315,   59, 1287,  784,   59,   59,  264, 1068,  436,
  773,  263,  775,  828,  829,  264,  827,  978, 1695,   59,
  981,  801,    0,  834,  840,  862,  863,  864,  410, 1115,
 1481, 1205, 1092,  348, 1695,  315,  446, 1695,  849, 1695,
 1748,  262, 1695, 1751,  283,  325,  428,  327,  828,  829,
  397,  862,  863,  864,  428,   42,  336,  309, 1310,  338,
   47,   12,  842, 1382, 1316,  319,  513,    0,  320,    0,
 1244,  994,  842,  260,  319,    0,  397, 1772,  262,  443,
  462,   59,  275, 1791,  494,    0,  667,  867,  403,  379,
  310,  672,    0,  379,  509, 1181,    0,  877,    0,  280,
  380,  448, 1797,  513,  867,  885,  886,  262,   41,  389,
   41,  298,    0,    0,  877,  885,   41,  434,  397,   44,
  315,  400,    0,  310,  257, 1749,   59,  448,   59,  298,
  325,  445,  327,   41,   59, 1142,  262,  416, 1833, 1590,
  509,  336,  252,  458,   59,    0,  403,  334,  982,    0,
  260,   59,  432,  394,  403,   59, 1851,   59,  379, 1854,
 1232,  979,  376,  442,  444,  334,  446,  419,  264,  983,
 1865,   59,   59,  980,  271,    0,  304,  305,  336,  994,
  305,   59,    0,  276,  277,  380, 1090, 1405,  285,  409,
  315,  284,  371,  144,  389,  379,  377,  330,  978,  979,
  980,  981,  982,  983,   59, 1328,  427,  264,   59,  989,
   44,  264,  399,  450,  453,  454,   41,    0,  258,   44,
 1000, 1001,  409,   41,  379,    0, 1195, 1007,    0, 1198,
  270,  410,  329,  513,   59, 1307,  318,  432,  277, 1299,
  477,   59,  284,  282,   40,  315,    0,  434, 1382,  331,
  431,  446,  371,  379, 1382,  297,   40,  297,   41,   93,
  357,  315, 1099,  303,  376,  305,  336,  448,  265, 1720,
 1330,  325,  354,  327, 1725,  390,   59, 1114,  457,  372,
  258,   44,  336, 1120,   59,    0,  368,   59, 1099, 1767,
  277, 1214,  270, 1108,  390,  282, 1488, 1112,  348, 1079,
  393,  427,  299, 1114,  344,   59,  399,  888,  457, 1120,
  459,  892,  352,  428, 1084,  443,  352, 1097,  513,  297,
  445,  440,  372,  405,  421,  303,  380,  305, 1108, 1230,
 1231, 1232, 1112,  390,   44,  389,  333,  390,  305,  264,
  433, 1533,    0,  344,   59, 1352, 1353,  462,  315,  264,
  322, 1358, 1359,    0, 1361, 1362,  304,  305,  378, 1263,
  264, 1265, 1369, 1370, 1371, 1372,  344, 1374, 1375, 1376,
 1377, 1378, 1379,    0,  276,  277,  446,  264,  432,    0,
    0,  421,  315,   93,  315,  421,  264,  376,  276,  277,
  315, 1213,  446,  318,   41, 1210,  327,   44, 1213, 1214,
  325,   59,  327,  336,  318,  336,  331,  315,  259,    0,
  376,  336,   59,  264,  339,  372,    0,  331,  269,  327,
   41,   41, 1326,   44,   44,  339,  395, 1396,  336,  354,
 1210, 1400,   59, 1213, 1214,  517,  393,  262,   59,   59,
  354, 1501,  399,  368, 1224,    0, 1226,  339,  316,  380,
  422,  376,  453,  454,  368,  380,   44,  429,  389,  513,
 1240,  430,   41,  298,  389,   44, 1246,  428,   59,  509,
  362,    0,  380,   44, 1780,   59,  433, 1537,  445,  486,
  405,  389,    0,  338,  298,    0,  390,    0,    0,  264,
  315,  405,  264,  318,  260,  443,  260,  315,  410,  334,
  325,  432,  327,  390,   59,   93,  331,  432, 1431,  260,
  264,  336,  390,  446,  339,  446,  371,  333,  336,  444,
  334,  446,   93, 1339,  432,  393,  418,  452, 1344,  354,
   59,  509,  315,  276,  300, 1305,  300,   44,  446,  390,
  434,   59,  397,  368,   59,  400,   59,   59, 1471,  300,
    0,  376,   59,  336,  379,  380,  481,  482,  513,  484,
   41,  416,  380,   44,  389,  433,  491,  492,  493,  494,
  495,  389,  497,  316,    0,  500,  501,  502,  503,  504,
  405, 1411,  513,  508,  509,  399,  260,  442,  513,  514,
  515,  516,  517,   42,   43,  409,   45,  380,   47,  434,
  258, 1661,  427,  517,  446,  513,  389,  432, 1577,   59,
  273,    0,  270,    0,  432,  390, 1431, 1602,  390,  444,
  434,  446,  318,  338,   41,  318,  300,   44,  446,  276,
  277, 1411,   40,   59,  260,  331,  390,  284,  331, 1419,
 1420, 1421,  333,    0,    0,  303,  265,  305,    0,  432,
  393, 1431,   41,  453,   41,   44,  371,   44,  354,   42,
   43,  354,   45,  446,   47,    0,   42,   43,  493,   45,
   59,   47,   59,  409,  300,  368,   59,  368,    0, 1602,
  299,    0,  397,  376,   41,  400, 1466,   44,  513,  380,
  433, 1471,  517, 1616,  315,  513,  270,  318, 1478, 1479,
  436,  416,   59,   59,  325,    0,  327,   59,  416,  405,
  331,  338,  405,   44, 1615,  336,   42,   43,  339,   45,
  294,   47, 1502,    0,   59,  372,  264,  442,   59,  303,
  513,   42,   43,  354,   45,  298,   47,   59, 1760, 1502,
   59,  396, 1764,  306,  371,    0,  393,  368,  313,   44,
  259,    0,  399, 1775,  338,  376,  315,  412,  379,  380,
  269,    0,    0,  337,   59,   46,  395,  341,  389, 1587,
  397,    0, 1794,  400,  452, 1760, 1645,   58,  332, 1764,
   61, 1766,   59,  485,  405,    0,  433,  371,    0,  416,
 1775,   42,   43,  495,   45,    0,   47,  426,  485,  486,
  493,  338,   41, 1825,   59,   44,  427, 1587, 1830, 1794,
   59,  432,  441,  397,  333,  442,  400,    0, 1646, 1599,
   59,   59,    0,  444,  517,  446,   41,    0,  277,   44,
   59,  444,  416,  282,  371,  258, 1616, 1760,  261, 1619,
 1825, 1764, 1611, 1766,   59, 1830,  493,   59,  395, 1772,
    0, 1667, 1775,    0,   59, 1671, 1636,  352,  442,  308,
  397, 1641,  436,  400,  403,    0, 1646,  441,  442,  443,
    0, 1794,  493, 1742, 1797,    0,   59,    0, 1641,  416,
  303,   59, 1712, 1224,    0, 1226,   59,  276,  277,  276,
  277,  386,  513,  388,  277,  284,  517,  284,  259,  282,
  333,  277, 1825,  313,  314,  442,  282, 1830,  269,   59,
 1833,  336,   59,   41,   41,  410,   44,   44,  338,    0,
  340,   41,   42,   43,   59,   45,   40,   47, 1851,   59,
  315, 1854, 1712, 1713,   59,  368,   59,  511,  317,  315,
  435,  515, 1865,   59, 1714,   41,   46,  380,   44,    0,
  395,  277,   46,  336,  453,  454,  282, 1772,   58,   40,
   41,   61,  376,   44,   58, 1780,  277,   61,   41,   42,
   43,  282,   45,  376,   47,   41,   42,   43,   59,   45,
  428,   47, 1797,  372,  498,  372,   40, 1767, 1757,  376,
   41,  317, 1772,   44,  356,  258,   41, 1777,   41,   44,
 1780,   44, 1783, 1783,  393,  442,  393,  270,   59, 1249,
  399, 1251,  399,   41, 1842,  426,   44, 1797, 1833,   41,
    0,   41,   44,  377,   44,  336,  277,   41, 1809, 1809,
   44,  282,   41,  384,  385,   44, 1851,  276,  277, 1854,
  303, 1873,  305, 1875,  433,  284,  433,  262,   41,   41,
 1865,   44,   44, 1833, 1834,  453,  454,  308, 1839, 1839,
   41,   41, 1842,   44,   44,   42,   43,  375,   45,   41,
   47, 1851,   44, 1853, 1854,  497,  409,   41, 1858,   59,
   44,  508,   41,   42,   43, 1865,   45,   41,   47,   41,
   44,   41,   44, 1873,   44, 1875,  509,  284,  672,    0,
  315,  508,  461,  318,  493,   44,  493,  366,   61,   40,
  325,    0,  327,  283,   40,  689,  331,  291,  264,  693,
   40,  336,   40,   40,  339,  453,  700,   40,  302,   40,
   40,  705,  706,  372,    0,  309,   40,   40,   40,  354,
   41,    0,  716,   44,  718,  719,  720,  721,  722,  723,
  724,  453,   41,  368,  393,   44,  453,  277,   59,  333,
  399,  376,  282,   40,  379,  380,   42,   43,   44,   45,
   59,   47,   40,   40,  389,   41,   40,   40,   44,   44,
   61,  264,   41,   91,   46,   44,   42,   43,  762,   45,
  405,   47,   40,   59,  433,  449,  450,  451,  428,  688,
   59,  690,  317,   59,  277,  694,  695,  696,  782,  282,
   44,  277,  427,  395,  788,  789,  282,  432,  317,   44,
    0,  395,   44,  477,  478,  276,  277,  281,  420,  444,
   46,  446,   61,  284,  315,   42,   43,  318,   45,   40,
   47,  264,   44,  344,  325,  276,  327,  344,  297,  344,
  331,  276,  426,  264,  493,  336,  430,  390,  339,  428,
   40,   41,   42,   43,   44,   45,   46,   47,  462,  390,
  450, 1602,  275,  354,  344,  439,  439,   40,  493,   59,
   60,   61,   62,  392,  345,   40,   91,  368,  265,   40,
   40,  315,  376,  264,  382,  376,  276,  277,  513,  380,
  277,  281,  517,   44,  284,  282,  375,   40,  389,   40,
  403,   91,  443,   93,   41,  275,   63,  297,  277,  456,
  296,  372,  299,  282,  405,  428,  900,  901,  902,  314,
   40,  905,  906,  907,  317,  909,  910,  911,  912,  913,
  914,  915,  393,  317,  918,  919,  444,  921,  399,  923,
  440,  432,  926,  927,  928,  929,  930,  931,  264,   44,
  494,   40,   40,  444,    0,  446,   41,   42,   43,  943,
   45,  945,   47,  273,  297,  276,  277,  340,  340,  379,
  281,  276,  433,  284,  281,  379,   40,  276,  277,  379,
  408,  410,  372,  410,  352,   40,  297,   40,  452,  297,
   41,  277,   44,  317,   40,   41,  282,  277,   44,  438,
  276,  277,  493,  393,   44,  270,  361,  378,  284,  399,
   44,  277,  996,   59,  317,   40,  282,  481,  482, 1760,
  484,  297,  513, 1764,   44, 1766,  517,  491,  492,  325,
  494,  495,  266,  497, 1775,   40,  500,  501,  502,  503,
  504,  493,   40,  433,  508,  509,  315,  264,   40,  318,
  514,  515,  516, 1794,  264,  414,  325,   58,  327,   41,
  277,  372,  331,   41,   41,  282,   41,  336,  264,  446,
  339,   41,  262,  372,  264,  265,  306,  306,  387,   41,
   40,  271,  393,   40, 1825,  354,   40,  277,  399, 1830,
   40,  348,  282,   40,  393,  285,  372,   40,   40,  368,
  399,   40,   40,  262,   58,   42,   43,  376,   45,  299,
   47,  380,   40,   40,  304,  305,   40,  393,  308,   40,
  389,   40,  433,  399,   40,  315,   40,  317,  318,   40,
  427,  321,  324,  305,  433,  325,  405,  327,   44,  329,
   41,  331,   41,  264,  381,  339,  336,  337,  339,  339,
  273,  905,  327,  907,  284,  909,  910,  433, 1142,  913,
  914,  915,  384,  432,  354,  355,  399,  357,   41,  450,
 1154, 1155,  379, 1157, 1158,  444,  379,  446,  368,  379,
  370, 1165,  372,  373,  374,  372,  376,   41,   40,  379,
  380,  379,  277,  379,  379,  276,   41,  282,   41,  389,
   41, 1185,    0,   41,  305,   40,  427,  290,   59, 1193,
 1194, 1195,  402,   59, 1198,  405, 1200,  336,  336,   59,
 1204, 1205, 1206,   59,  493,   59,   59,   59,  305,   59,
   59,  421,   58,  378,  339,  428,  389,  427,  428,  277,
  333,  298,  432,   41,  513,  434,   44,  334,  517,  298,
  434,   41,  334,  443,  444,   41,  446,  348,  365,  365,
 1244,   59,  452,  378,   41,   41,   41,   41,   41,  315,
   41,   41,  318,   41,   41,   41,   41,   41,   41,  325,
   41,  327,   41,   41,   41,  331,  315,   59,  443,   40,
  336,  481,  482,  339,  484,   44,   41,  340, 1282,   40,
  348,  491,  492,  493,  494,  495,  458,  497,  354,   59,
  500,  501,  502,  503,  504,  336,   46,  500,  508,  509,
  336,   58,  368,  513,  514,  515,  516,  517,  518,   40,
  376,   40,  362,   40,  380,   41,   42,   43,   41,   45,
  277,   47,  402,  389,   40,  282,   41,  427,    0,   41,
   42,   43,  376,   45,  376,   47,   44,  376,   59,  405,
  301,   41,   44,  298,   41,   42,   43, 1351,   45,  434,
   47,  304,  290,   41,   42,   43, 1360,   45,  301,   47,
   40,  455,  333, 1367,   41,   44,  432,   40,    0,   41,
   42,   43,   44,   45,   46,   47,   42,   43,  444,   45,
  446,   47,   41,  333,  376,  376,  452,   59,   60,   61,
   62,   40, 1396,  409,  264,  264, 1400,  409,   41,   42,
   43,  276,   45,  450,   47,  317,  462,  403,   42,  443,
   40,   44, 1416,  270,   41,  481,  482,  450,  484,   91,
  264,   93,  270,  428,  427,  491,  492,  493,  494,  495,
  339,  497,  339,  450,  500,  501,  502,  503,  504,   41,
  452,   41,  508,  509,   59,   59,  305,  513,  514,  515,
  516,  517,   40,   59,   41,   40,   59,   41,  276,  277,
  464,  465,  466,  467,  468,  469,  470,  471,  301,  481,
  482,   41,  484,   59,    0,    0, 1480,    0,  452,  491,
  492, 1227,  494,  495,   12,  497,  289, 1117,  500,  501,
  502,  503,  504,  282,  742,  880,  508,  509,  656,  281,
 1115,   90,  514,  515,  516,  842, 1112,  481,  482,  885,
  484, 1107, 1520,  349, 1521,  379,  605,  491,  492,    0,
  494,  495, 1097,  497, 1043,  329,  500,  501,  502,  503,
  504,  361,  361,  784,  508,  509, 1108, 1582,  810, 1636,
  514,  515,  516, 1499, 1785,  583, 1501, 1789, 1788, 1816,
  975,  277, 1556, 1557,  372, 1701,  282,  801, 1419, 1607,
   41, 1773,  730,   44, 1261,  277, 1484,  366,  859, 1573,
  282,   42,   43, 1577,   45,  393,   47, 1581,   59,  354,
  277,  399, 1586,  347, 1588,  282,  585,  642,   45,  277,
  262, 1314,  264,  265,  282,  608,   13,  316,  264,  271,
  524,  528,  149,  773,  851,  277,  762, 1007,   86, 1250,
  282,  277, 1730,  285, 1626,  433,  282, 1839,   41,   42,
   43, 1809,   45,  539,   47,  749,   42,  299,    0,  301,
  997, 1269,  304,  305,  277,  788,  308,   -1, 1200,  282,
 1156,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,
   -1,   -1, 1666,   -1,  336,  337,   -1,  339,   -1,   41,
   42,   43,   44,   45,   46,   47,   42,   43,   44,   45,
   -1,   47,  354,  355,   -1,  357,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,  452,  368, 1701,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   91,
   -1,   93,   -1,   -1,  481,  482, 1730,  484,   -1,   -1,
  402,   59,   -1,  405,  491,  492,  452,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,
   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,  516,
  432,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
 1774,  443,  444,   -1,  446,  491,  492,   -1,  494,  495,
  452,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,  262,  508,  509,   -1,  276,  277,   -1,  514,  515,
  516,   -1,   -1,  284,   -1,   -1,  277,   -1,   -1,  481,
  482,  282,  484,   -1,   -1,   -1,  297,   -1,  452,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  481,  482,   -1,
  484,   -1,   -1,   -1,  277,   -1,   -1,  491,  492,  282,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,  293,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   44,   -1,   -1,
  262,  372,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,
   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,
  282,  277,  393,  285,   -1,   -1,  282,   -1,  399,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,  433,  325,   -1,  327,   -1,  329,   -1,  331,
   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,
   62,   -1,   -1,  450,   -1,  452,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   91,
   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  402,   59,   -1,  405,  491,  492,  452,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,
   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,  516,
  432,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  443,  444,   -1,  446,  491,  492,   -1,  494,  495,
  452,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   42,   43,   44,   45,   -1,   47,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,  452,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,   42,   43,   -1,
   45,   -1,   47,  481,  482,   -1,  484,   -1,   -1,   -1,
  317,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,
   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  285,   41,   42,   43,   -1,   45,    0,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,
   41,   -1,   -1,   44,  336,  337,   -1,  339,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,   -1,   59,   -1,
   61,   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   91,
   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  402,   -1,   -1,  405,  491,  492,  277,  494,  495,   -1,
  497,  282,   -1,  500,  501,  502,  503,  504,   -1,  421,
   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,  516,
  432,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,  443,  444,   -1,  446,   41,   42,   43,   -1,   45,
  452,   47,  277,   -1,   41,   42,   43,  282,   45,   -1,
   47,   -1,   -1,   -1,   -1,   41,   42,   43,   -1,   45,
   -1,   47,   41,   42,   43,   -1,   45,   -1,   47,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,  452,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,   -1,   -1,   -1,
  277,   -1,   -1,  481,  482,  282,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,  258,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  262,   -1,  264,  265,  275,  276,  277,   -1,   -1,  271,
  281,   -1,   -1,  284,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  285,   -1,   -1,  297,   -1,   -1,    0,
   -1,   -1,  303,   -1,   -1,   -1,   -1,  299,    0,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   41,   42,
   43,   -1,   45,  315,   47,  317,  318,   -1,   -1,  321,
   41,   42,   43,  325,   45,  327,   47,  329,   -1,  331,
   41,   -1,   -1,   44,  336,  337,  291,  339,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,  302,   59,   -1,
   61,   -1,  354,  355,  309,  357,   -1,   59,   60,   61,
   62,  372,   -1,   -1,   -1,   -1,  368,  322,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,  277,  393,   -1,   -1,   -1,  282,  389,  399,   91,
  277,   93,   -1,   -1,   -1,  282,   -1,  408,   -1,  410,
  402,  277,   -1,  405,   -1,   -1,  282,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,   -1,  428,   -1,  421,
   -1,   -1,  433,   -1,  379,  427,  428,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  395,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,  462,   -1,   -1,   -1,   -1,  411,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  426,   -1,   -1,   -1,  430,   -1,   -1,  433,  481,
  482,   -1,  484,   -1,   -1,   -1,  441,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,  460,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  277,   -1,   -1,  258,   -1,  282,
   -1,   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,
  262,  282,  264,  265,  275,  276,  277,   -1,   -1,  271,
  281,   -1,   -1,  284,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  285,   -1,   -1,  297,   -1,   -1,    0,
   -1,   -1,  303,   -1,   -1,   -1,   -1,  299,    0,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
  284,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,
   41,   -1,   -1,   44,  336,  337,   -1,  339,   -1,   41,
   42,   43,   44,   45,   -1,   47,   -1,   -1,   59,   -1,
   61,   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,
   62,  372,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,  393,   -1,   -1,   -1,   -1,  389,  399,   -1,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,
  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,  421,
   -1,   -1,  433,   -1,   -1,  427,  428,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 1352,
 1353,  443,  444,   -1,  446, 1358, 1359,   -1, 1361, 1362,
  452,  462,   -1,   -1,   -1,   -1, 1369, 1370, 1371, 1372,
   -1, 1374, 1375, 1376, 1377, 1378, 1379,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,  452,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  481,  482,   -1,
  484,   -1,   -1,    0,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,  258,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   41,  276,  277,   44,   -1,  271,
  281,   -1,   -1,  284,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   59,  285,   61,   -1,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,   -1,   -1,  299,    0,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,
   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   41,
   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,
   62,  372,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,  393,   -1,   -1,   -1,   -1,  389,  399,   -1,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  325,  410,
  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   45,   -1,   -1,  428,   -1,  421,
   -1,   -1,  433,   -1,   -1,  427,  428,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  258,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,  273,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  435,   -1,
   -1,  513,  514,  515,  516,  517,  518,   -1,   -1,   -1,
  297,   -1,   -1,   -1,   -1,  452,  303,   -1,   -1,  306,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  284,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  262,   -1,  264,  265,  491,  492,   -1,  494,  495,  271,
  497,  348,   -1,  500,  501,  502,  503,  504,   -1,   -1,
  282,  508,  509,  285,   -1,   -1,   -1,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,  403,  329,   -1,  331,
   -1,   -1,   -1,  410,  336,  337,   -1,  339,   -1,   41,
   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,  428,  354,  355,   -1,  357,   -1,   59,   60,   61,
   62,  292,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,  458,   -1,   -1,   -1,  462,   -1,  389,   -1,  320,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  402,  302,   -1,  405,   -1,   -1,  452,   -1,  309,   -1,
   -1,   -1,  313,  314,   -1,   -1,   -1,  348,   -1,  421,
   -1,  322,   -1,   -1,   -1,  427,  428,   -1,   -1,   -1,
  432,   -1,  333,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  443,  444,   -1,  446,  491,  492,   -1,  494,  495,
  452,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,  419,  491,
  492,  493,  494,  495,  395,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,  435,   -1,  508,  509,   -1,   -1,
  411,  513,  514,  515,  516,  517,  518,   -1,  449,  450,
  451,   -1,  453,  454,   -1,  426,   -1,   -1,   -1,  430,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  441,  472,  473,  474,  475,  476,  477,  478,  479,  480,
   -1,   -1,  339,   -1,   -1,   -1,  487,  488,   -1,  460,
  262,   -1,  264,  265,   -1,  496,   -1,   -1,   -1,  271,
   -1,   -1,   -1,   -1,   -1,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,
   -1,  418,   -1,   -1,  336,  337,   -1,  339,   -1,   41,
   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,
   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,  516,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,  426,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,  452,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  261,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,  279,   -1,  271,
   -1,   -1,   -1,   -1,   -1,  287,   -1,   -1,   -1,   -1,
   -1,   -1,  294,  285,  296,   -1,  298,    0,   -1,   -1,
   -1,  303,   -1,   -1,   -1,   -1,   -1,  299,  310,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,  324,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,  334,  325,   -1,  327,   -1,  329,   41,  331,
   43,   44,   45,   -1,  336,  337,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   93,   -1,  271,   -1,   -1,  397,   -1,  389,  277,   -1,
   -1,   -1,  404,  282,   -1,  407,  285,  409,  410,   -1,
  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,
   -1,   -1,  434,   -1,   -1,  427,  428,   -1,   -1,   -1,
  432,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  329,  443,  444,   -1,  446,   -1,   -1,   -1,  337,   -1,
  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  355,   -1,  357,   -1,
   -1,  483,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  370,  484,  372,  373,  374,  498,  499,  435,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,  452,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,   -1,   -1,   -1,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,  481,  482,   -1,  484,   60,   61,
   62,   -1,   -1,   -1,  491,  492,   -1,  494,  495,  262,
  497,  264,  265,  500,  501,  502,  503,  504,  271,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   41,  331,   43,
   44,   45,   -1,  336,  337,   -1,  339,  272,   -1,  518,
  275,   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,  354,  355,  288,  357,   -1,   -1,  292,   -1,   -1,
  295,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   93,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,
  335,   -1,  405,   -1,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,   -1,  421,   -1,
   -1,   -1,   -1,   -1,  427,  428,   -1,   -1,   -1,  432,
   -1,   -1,  367,   -1,  369,   -1,   -1,   -1,   -1,   -1,
  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  271,
   -1,   -1,   -1,  398,   -1,  277,   -1,   -1,  403,   -1,
  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,  481,  482,
  415,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,  437,   -1,   -1,  508,  509,   -1,   -1,  321,
  513,  514,  515,  516,  517,  518,   -1,  329,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  337,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,
   -1,   -1,   -1,  355,   -1,  357,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,  489,   -1,   -1,  271,  370,   -1,
  372,  373,  374,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,
  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,  421,
   -1,  325,   -1,  327,   -1,  329,   41,  331,   43,   44,
   45,   -1,  336,  337,   -1,  339,  272,   -1,   -1,  275,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  354,  355,  288,  357,   -1,   -1,  292,   -1,   -1,  295,
   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,   -1,  379,  380,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,  335,
   -1,  405,   -1,   -1,   -1,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,  518,  421,   -1,   -1,
   -1,   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,
   -1,  367,   -1,  369,   -1,   -1,   -1,   -1,   -1,  443,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  415,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,  437,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,
  514,  515,  516,  517,  518,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,  489,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   41,  331,   43,   44,   45,
   -1,  336,  337,   -1,  339,  272,   -1,   -1,  275,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,
  355,  288,  357,   -1,   -1,  292,   -1,   -1,  295,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,  335,   -1,
  405,   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
  367,   -1,  369,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
  437,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,  489,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   59,   60,   61,   62,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,    0,   -1,
  336,  337,   -1,  339,   -1,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  395,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   41,
  376,   -1,   44,  379,  380,   -1,   -1,   -1,   -1,   -1,
  430,   -1,   -1,  389,   -1,  435,   -1,   59,   -1,   -1,
   -1,  441,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  443,  444,   -1,
  446,  491,  492,   -1,  494,  495,  452,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,    0,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,  262,  513,  514,  515,
  516,  517,  518,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   91,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  405,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,  421,   -1,  339,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,    0,   -1,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,  262,   -1,  513,   -1,   -1,
  432,  517,  518,   -1,  271,   59,   60,   61,   62,    0,
  277,   -1,  444,   -1,  446,  282,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   41,  318,   -1,   44,  321,   -1,   -1,   -1,  325,   -1,
  327,  493,  329,   -1,  331,   -1,   -1,   -1,   59,  336,
  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  513,   -1,   -1,   -1,  517,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,  405,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,    0,  421,   -1,  514,  515,  516,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,   -1,   59,    0,   -1,   -1,   -1,  271,   -1,   -1,
   -1,   -1,   -1,  277,   -1,   -1,  493,   -1,  282,   -1,
   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,   -1,
  517,  518,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,  315,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
   -1,  325,   59,  327,   -1,  329,   -1,  331,   -1,   -1,
   -1,   -1,  336,  337,   -1,  339,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,  315,  379,  380,  318,   -1,   41,
   -1,   -1,   44,   -1,  325,  389,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,   -1,  336,   -1,   59,  339,   -1,
   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,  354,   -1,   -1,   -1,  421,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,  368,  432,   -1,
   -1,  452,   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,  405,  262,   -1,   -1,   -1,   59,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,    0,   -1,  508,  509,  493,
   -1,  432,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,  513,
   -1,  452,   -1,  517,  518,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,  262,   41,   -1,  325,   44,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
  481,  482,  339,  484,   59,    0,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,  354,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,  368,  513,  514,  515,  516,  517,   -1,  315,  376,
   -1,  318,  379,  380,   -1,   -1,   41,   -1,  325,   44,
  327,   -1,  389,   -1,  331,   -1,   -1,   -1,   -1,  336,
  262,   -1,  339,   -1,   59,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,  368,   -1,   -1,   -1,  432,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,  389,  315,   -1,   -1,  318,   -1,   -1,   -1,
   -1,   41,   -1,  325,   44,  327,   -1,   -1,  405,  331,
   -1,   -1,  262,   -1,  336,   -1,   -1,  339,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,  354,   -1,   -1,  432,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,  444,   -1,  446,
   -1,   -1,   -1,   -1,  376,   -1,  513,  379,  380,   -1,
  517,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,  405,   -1,   -1,  336,  262,   -1,  339,
    0,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,  513,   -1,  368,   -1,
  517,    0,  444,   -1,  446,   -1,  376,   -1,   -1,  379,
  380,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  389,
  315,   -1,   -1,  318,   -1,   -1,   -1,  262,   -1,   59,
  325,   -1,  327,   -1,   -1,  405,  331,   -1,   -1,   -1,
   -1,  336,   41,   -1,  339,   44,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,   -1,  427,   -1,  354,
   59,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  513,   -1,  368,  444,  517,  446,   -1,   -1,   -1,
  315,  376,   -1,  318,  379,  380,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,  389,   -1,  331,   -1,   -1,   -1,
   -1,  336,  262,   -1,  339,   -1,   -1,   -1,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  427,  368,   -1,   -1,   -1,  432,   -1,   -1,
   -1,  376,   -1,  513,  379,  380,   -1,  517,   -1,  444,
   -1,  446,   -1,   -1,  389,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
  405,  331,   41,   -1,   -1,   44,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,  427,   -1,  354,   -1,   -1,  432,  493,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,  368,  444,
   -1,  446,   -1,   -1,   -1,   -1,  376,   -1,  513,  379,
  380,   -1,  517,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  405,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,  262,   -1,   59,    0,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,  513,   -1,
   -1,   -1,  517,   -1,  444,  315,  446,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   41,   -1,  339,
   44,   -1,   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,  354,   59,  325,   -1,  327,   -1,
   -1,   -1,  331,  493,   -1,   -1,   -1,  336,  368,   -1,
  339,    0,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,  513,   -1,  354,   -1,  517,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,   -1,   -1,
  379,  380,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
  389,   -1,   -1,  262,   -1,   -1,   -1,  427,   -1,   -1,
   59,   -1,  432,   -1,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,  318,
   -1,   -1,   -1,   -1,   41,   -1,  325,   44,  327,   -1,
   -1,   -1,  331,  493,   -1,   -1,   -1,  336,  262,   -1,
  339,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  513,   -1,  354,   -1,  517,    0,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,
  389,  315,   -1,   -1,  318,   -1,   -1,   -1,  262,   41,
   -1,  325,   44,  327,   -1,   -1,  405,  331,   -1,   -1,
   -1,   -1,  336,   -1,   -1,  339,   -1,   59,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
  354,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,  444,   -1,  446,   -1,   -1,
   -1,  315,  376,   -1,  318,  379,  380,   -1,   -1,   41,
   -1,  325,   44,  327,   -1,  389,   -1,  331,   -1,   -1,
   -1,   -1,  336,  262,   -1,  339,   -1,   59,   -1,   -1,
   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,   -1,   -1,    0,  493,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,  368,   -1,   -1,   -1,  432,   -1,
   -1,   -1,  376,   -1,  513,  379,  380,   -1,  517,   -1,
  444,   -1,  446,   -1,   -1,  389,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   41,   -1,  325,   44,  327,   -1,
   -1,  405,  331,   -1,   -1,  262,   -1,  336,   -1,   -1,
  339,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,  354,   -1,   -1,  432,  493,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
  444,   -1,  446,   -1,   -1,   -1,   -1,  376,   -1,  513,
  379,  380,   -1,  517,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,
  262,   -1,  339,    0,   -1,   -1,   -1,   -1,   -1,  493,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,  513,
   -1,  368,   -1,  517,    0,  444,   -1,  446,   -1,  376,
   -1,   -1,  379,  380,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,  389,  315,   -1,   -1,  318,   -1,   -1,   -1,
  262,   -1,   59,  325,   -1,  327,   -1,   -1,  405,  331,
   -1,   -1,   -1,   -1,  336,   41,   -1,  339,   44,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  427,   -1,  354,   59,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  513,   -1,  368,  444,  517,  446,
   -1,   -1,   -1,  315,  376,   -1,  318,  379,  380,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,  389,   -1,  331,
   -1,   -1,   -1,   -1,  336,  262,   -1,  339,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,   -1,  493,   -1,   -1,   -1,
   -1,   -1,   -1,  260,   -1,  427,  368,   -1,   -1,   -1,
  432,   -1,   -1,   -1,  376,   -1,  513,  379,  380,   -1,
  517,   -1,  444,   -1,  446,   -1,   -1,  389,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,  298,   -1,  405,  331,   -1,   -1,   -1,   -1,  336,
   -1,   40,  339,  310,   43,   -1,   45,   -1,  452,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,  354,   -1,   58,
  432,  493,   -1,   -1,   63,   -1,   -1,  334,   -1,   -1,
   -1,  368,  444,   -1,  446,   -1,   -1,  481,  482,  376,
  484,  513,  379,  380,   -1,  517,   -1,  491,  492,   -1,
  494,  495,  389,  497,   -1,  262,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  405,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,
  427,   -1,  399,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,  513,  409,   -1,   -1,  517,   -1,  444,  315,  446,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,  434,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,  452,   -1,  354,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,
  336,  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,  481,  482,  513,  484,  354,   -1,
  517,   -1,  389,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,  368,  500,  501,  502,  503,  504,  405,   -1,
  376,  508,  509,  379,  380,   -1,   -1,  514,  515,  516,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,  267,  405,
   -1,   -1,   -1,   -1,   -1,  274,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,  427,   -1,  292,  293,   40,  432,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,   -1,  311,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,  320,   -1,   -1,  323,   -1,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,  348,
  517,  350,  351,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,  272,   -1,
   -1,  275,   -1,  372,   -1,   -1,   -1,  513,   -1,   -1,
   -1,  517,   -1,   -1,   -1,   -1,  290,   -1,  292,   -1,
   -1,  295,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,  335,   -1,   -1,   -1,   -1,  435,  341,  342,  343,
   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,  463,  369,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,  398,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  505,  506,  507,  508,
  509,  415,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,  267,  272,   -1,   -1,  275,   40,   -1,  274,
   43,   -1,   45,  437,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,  292,  289,   58,  295,  292,  293,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,  472,  473,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  335,   -1,   -1,   -1,
   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,  364,
  369,   -1,  272,   -1,   -1,  275,   -1,  372,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,
   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,  398,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,  413,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,  298,   -1,   -1,
  425,   -1,   -1,   -1,   -1,  335,   -1,   -1,  437,  310,
  435,  341,  342,  343,   -1,  345,  346,  347,   -1,  349,
   -1,   -1,   -1,  324,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,  334,   -1,   -1,   -1,  367,  463,  369,
   -1,   -1,   -1,  472,  473,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
  489,   -1,  487,  488,   -1,  490,  491,  492,  398,  494,
  495,  496,  497,   -1,  267,  500,  501,  502,  503,  504,
   -1,  274,   -1,  508,  509,  415,   -1,   -1,   -1,  514,
  515,  516,   40,  286,   -1,   43,   -1,   45,  399,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,  437,  409,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,  348,   -1,  350,  351,  489,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  267,
   -1,  514,  515,  516,   40,   -1,  274,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   58,   -1,  292,  293,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,  267,  500,  501,  502,  503,  504,   -1,  274,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,  260,   -1,   -1,  263,   -1,  372,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   58,   -1,  425,
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
   -1,   -1,   -1,   -1,   -1,   58,   -1,  443,   -1,  320,
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
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
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
  503,  504,   -1,   -1,   -1,  508,  509,  270,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,  298,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,   -1,  312,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,  328,   -1,  463,   -1,   -1,
   -1,  334,   -1,   -1,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  260,   -1,   -1,  263,   -1,   -1,  391,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   40,  424,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,  320,   63,   -1,  323,   -1,   -1,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,  360,   -1,   -1,   -1,  364,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   58,   -1,  425,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
    0,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   91,   -1,   -1,  364,   -1,   -1,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,
   -1,   -1,   -1,   58,   -1,  425,   -1,  320,   63,   -1,
  323,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,  258,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,  273,   -1,  275,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,
  463,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,  348,   -1,
   -1,  514,  515,  516,   -1,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,  372,   40,   -1,   42,   43,   -1,   45,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   58,   -1,  393,   -1,   -1,   63,   -1,   -1,  399,
   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  458,  364,
   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,  260,   -1,  500,  501,  502,  503,  504,
  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   40,
  260,   42,   43,   -1,   45,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
  300,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   40,   41,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,  267,  487,  488,   -1,  490,  491,  492,
  274,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  286,   -1,   -1,  508,  509,   -1,  292,  293,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,  267,
  487,  488,   -1,  490,  491,  492,  274,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  286,   -1,
   -1,  508,  509,   -1,  292,  293,   -1,  514,  515,  516,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,  320,   63,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   58,   59,  425,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,
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
   -1,  425,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
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
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  348,
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
   -1,   -1,   -1,   -1,  463,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,  267,  487,  488,
   -1,  490,  491,  492,  274,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  286,   40,   41,  508,
  509,   44,  292,  293,   -1,  514,  515,  516,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   91,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
  360,   61,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,    0,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   41,   -1,   -1,   44,   -1,  425,   -1,   91,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  258,   -1,    0,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,   -1,
  273,   -1,  275,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  297,   -1,   -1,   -1,   41,   -1,
  303,   44,   -1,  306,   -1,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,  273,   -1,  275,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  258,  348,   -1,  297,   91,   -1,
   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,  273,
   -1,   -1,  276,  277,   -1,   -1,   -1,  281,   -1,  372,
  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  297,   -1,   -1,   -1,  258,   -1,  303,
  393,   -1,  306,   -1,   -1,   -1,  399,   -1,  348,   -1,
  403,   -1,  273,   -1,   -1,  276,  277,  410,   -1,   -1,
  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  372,   -1,   -1,  428,  297,   -1,   -1,   -1,
  433,   -1,  303,   -1,  348,  306,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,   -1,  399,
   -1,   -1,   -1,  403,   -1,  458,   -1,   -1,  372,  462,
  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,  428,  393,
   -1,   -1,   -1,  433,   -1,  399,   -1,   -1,   -1,  403,
   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,   -1,
   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,  458,   -1,
   -1,   -1,  462,   -1,  428,  258,   -1,   -1,   -1,  433,
   -1,   -1,  393,   -1,   -1,   -1,   -1,   -1,  399,   -1,
  273,   -1,  403,  276,  277,   -1,   -1,   -1,  281,  410,
   -1,  284,   -1,  270,  458,   -1,   -1,   -1,  462,   -1,
   -1,   -1,   -1,   -1,  297,   -1,   -1,  428,   -1,   -1,
  303,   -1,  433,  306,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,  305,   -1,
   -1,   -1,  309,  310,   -1,  312,   -1,  458,  315,   -1,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,   -1,  348,   -1,  334,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,  275,   -1,
   -1,   -1,   -1,   -1,  281,   -1,   -1,   -1,   -1,   -1,
  393,   -1,   -1,   -1,   -1,  292,  399,   -1,  295,   -1,
  403,   -1,   -1,   -1,  391,   -1,   -1,  410,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  409,   -1,   -1,  428,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,  424,  335,   -1,
   -1,   -1,   -1,   -1,  341,  342,  343,  434,  345,  346,
  347,   -1,  349,   -1,   -1,  458,  443,   -1,  445,  462,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,
  367,   -1,  369,  272,   -1,   -1,  275,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  292,  481,  482,  295,  484,   -1,   -1,
   -1,  398,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,  415,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
  437,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,
  369,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,  489,   -1,  491,  492,   -1,  494,  495,  398,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  415,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
  489,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  452,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
  461,  462,  463,  464,  465,  466,  467,  468,  469,  470,
  471,  472,  473,  474,  475,  476,   -1,   -1,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,   -1,
  491,  492,  493,  494,  495,  496,  497,  498,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,  517,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  462,  463,  464,
  465,  466,  467,  468,  469,  470,  471,  472,  473,  474,
  475,  476,   -1,   -1,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,   -1,  491,  492,  493,  494,
  495,  496,  497,  498,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,  510,  511,  512,  513,  514,
  515,  516,  517,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 518
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
"IIF","SCALAR_ARRAY","CROSS","EQUIV",
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
"comparison_predicate : value EQUIV value",
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
{ yyval = make_node (nod_equiv, 2, yyvsp[-2], yyvsp[0]); }
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
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 835:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 837:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 839:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 841:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 843:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 845:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 846:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 847:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 848:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 849:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 850:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 851:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 852:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 853:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 854:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 855:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 856:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 858:
{ yyval = make_list (yyvsp[-1]); }
break;
case 859:
{ yyval = yyvsp[-1]; }
break;
case 869:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 870:
{ yyval = yyvsp[0]; }
break;
case 871:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 872:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 873:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 874:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 875:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 876:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 877:
{ yyval = yyvsp[-1]; }
break;
case 878:
{ yyval = yyvsp[-1]; }
break;
case 882:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 883:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 884:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 887:
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
case 888:
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
case 889:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 890:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 892:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 894:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 895:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 896:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 897:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 898:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 899:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 901:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 902:
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
case 903:
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
case 904:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 905:
{ yyval = make_parameter (); }
break;
case 906:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 907:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 908:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 909:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 910:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 911:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 912:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 913:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 914:
{ yyval = yyvsp[0]; }
break;
case 915:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 917:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 918:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 919:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 920:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 921:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 923:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 924:
{ yyval = yyvsp[0];}
break;
case 929:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 930:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 931:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 932:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 933:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 936:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 938:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 939:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 940:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 942:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 944:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 945:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 946:
{ yyval = yyvsp[0]; }
break;
case 947:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 948:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 949:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 950:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 953:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 954:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 955:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 958:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 959:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 960:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 961:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 962:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 963:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 964:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 965:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 969:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 970:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 971:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 972:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 974:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 976:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
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
