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
#define CURRENT_DATABASE 518
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
   59,  136,  136,  109,  109,  109,  109,  109,  109,  109,
  137,  137,  152,  152,  153,  155,  155,  155,  155,  155,
  134,  154,  154,  159,  159,  159,  159,  161,  162,  163,
  157,  157,  158,  156,  156,  156,  156,  156,  164,  165,
  166,  166,  166,  166,   72,   82,   86,  171,  167,  167,
  168,  168,  172,  172,  174,  173,  173,  176,  175,  175,
  175,  169,  169,  177,  177,  178,  180,  180,  181,  179,
  179,  183,  183,  182,  185,  185,  170,  187,  187,  187,
  188,  188,  186,  186,  186,  186,  186,  186,  186,  186,
  186,  186,  186,  186,  186,  186,  186,  186,  186,  186,
  186,  191,  191,  192,  193,  194,  195,  200,  201,  196,
  196,  197,  209,  198,  210,  204,  204,  204,  205,  205,
  205,  207,  207,  207,  207,  199,  206,  206,  202,  202,
  202,  208,  208,  189,  189,  215,  216,  216,  217,  217,
  217,  217,  203,  203,  203,  219,  221,  220,  222,   11,
   12,  223,  223,  224,  224,  225,   75,   84,  141,  229,
   91,   93,  228,  228,   74,   87,  230,  230,  230,  231,
  234,  234,  235,  235,  235,  235,  235,  235,  235,  235,
  235,  235,  235,  235,  235,  235,  235,  232,  232,  233,
    2,  237,  237,  237,  237,  237,  237,  237,  245,  246,
  243,  243,  247,  247,  247,  247,  247,  247,  247,  238,
  238,  248,  248,  248,  248,  248,  248,  248,  242,  253,
  253,  253,  253,  253,  253,  253,  253,  253,  253,  253,
  253,  253,  253,  253,  253,  253,  253,  253,  253,  253,
  253,  253,  253,  253,  253,  253,  253,  250,  250,  252,
  252,  251,  249,  249,  249,  244,  244,  240,  241,  241,
  255,  255,  255,  255,  255,  239,  256,  256,  257,  257,
    8,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  258,  258,  258,  105,  105,  138,  138,  261,  261,  264,
  264,  266,  266,   54,   54,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  271,  271,  262,  262,  262,  262,
  273,  273,  272,  272,  272,   56,   56,  268,  268,  268,
  265,  265,  265,  277,  277,  277,  276,  276,  275,  275,
  275,  269,  269,  278,  278,  278,  279,  279,  270,  270,
  270,  270,  280,  280,   19,   19,   19,  282,  282,  282,
   17,   17,   17,  284,  285,  288,  288,  286,  290,  290,
    4,   16,  289,  289,  291,  291,  292,  292,  281,  293,
  293,  294,  294,  295,  295,  295,  295,  296,  296,  297,
  297,  298,  298,  300,  300,  300,  301,  301,  301,  302,
  302,  302,  299,  304,  304,  304,  305,  305,  303,  303,
  306,  308,  308,  307,  307,  283,   18,  309,  309,  311,
  311,  310,  310,  227,  315,  316,  312,  312,  312,  317,
  319,  328,  329,  330,  331,  320,  320,  320,  320,  332,
  332,  332,  333,  333,  333,  321,  321,  322,  322,  335,
  335,  336,  336,  337,  337,  323,  339,  339,  340,  340,
  342,  342,  342,  344,  345,  345,  346,  346,  348,  348,
  341,  341,  341,  349,  350,  351,  353,  353,  354,  355,
  343,  343,  356,  356,  357,  357,   32,  352,  352,  352,
  352,  352,  358,  358,  325,  325,  359,  359,  360,  326,
  326,  324,  324,  327,  327,  361,  362,  362,  362,  362,
  362,  363,  363,  364,  364,  365,  365,  366,  366,  366,
  367,  367,  368,  368,  313,  313,  369,  369,  370,   69,
   69,   69,  372,  372,  371,  371,  314,  314,  314,   10,
   10,    7,    7,  374,  375,  376,   20,   20,  377,  378,
  379,  379,  190,    3,    3,  381,  381,  381,  383,  383,
  272,  382,  382,  384,  384,   35,   35,   89,   88,   88,
  373,  373,  385,  386,  386,  213,  213,  213,  140,  380,
  380,  115,  115,  115,  115,  115,  390,  390,  390,  390,
  390,  390,  389,  389,  389,  392,  392,  392,  392,  392,
  392,  392,  392,  392,  392,  392,  393,  393,  393,  393,
  393,  393,  393,  393,  399,  399,  399,  399,  399,  399,
  399,  399,  399,  399,  399,  399,  399,  399,  399,  399,
  404,  404,  394,  394,  395,  395,  396,  396,  396,  396,
  397,  397,  401,  401,  402,  402,  402,  402,  400,  403,
  398,  398,  388,  388,  388,  391,  391,  391,  405,  405,
  406,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,  151,
  151,  151,  407,  212,  212,  145,  145,  413,  413,  413,
  413,  413,  409,  409,  409,  409,  409,  226,  146,  146,
  147,  148,  149,  149,  149,  149,  149,   52,   52,   21,
   21,  236,  414,   55,  274,  267,  267,   99,  408,  408,
  408,  408,  415,  415,  415,  415,  415,  415,  415,  415,
  415,  415,  415,  416,  417,  419,  418,  418,  421,  422,
  422,  412,  412,  410,  411,  411,  423,  423,  423,  424,
  424,  425,  425,  428,  428,  426,  426,  431,  431,  430,
  427,  429,  420,  420,  420,  420,  420,  420,  420,  420,
  334,  334,  318,  318,  150,   49,   65,  124,  142,  387,
  160,  184,  260,   67,   63,  218,  116,   70,  338,  214,
   36,   42,  347,  259,   40,   43,  211,   41,  287,  254,
  254,  432,  432,  432,  432,  432,  432,  432,  432,  432,
  432,  432,  432,  432,  432,  432,  432,  432,  432,  432,
  432,
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
    0,    2,    0,    1,    1,    1,    1,    1,    1,    1,
    0,    1,    1,    2,    2,    2,    5,    1,    2,    3,
    2,    2,    0,    1,    1,    1,    1,    3,    4,    8,
    4,    0,    6,    1,    1,    2,    2,    0,    3,    3,
    1,    2,    2,    2,    8,    8,    8,    8,    3,    0,
    4,    0,    1,    3,    4,    1,    3,    2,    3,    3,
    1,    1,    0,    1,    2,    4,    1,    1,    3,    1,
    0,    2,    1,    6,    1,    1,    3,    1,    2,    0,
    1,    2,    2,    2,    1,    1,    1,    1,    1,    1,
    2,    1,    1,    2,    1,    1,    1,    2,    2,    1,
    2,    3,    4,    2,    6,    4,    8,    9,    6,    8,
    6,    4,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    7,    2,    0,    2,    2,
    3,    3,    0,    1,    2,    4,    1,    3,    2,    2,
    2,    1,    1,    1,    1,    2,    2,    5,    0,    4,
    7,    3,    0,    1,    3,    3,    7,    7,    0,    0,
    0,    0,    3,    0,    9,    9,    1,    1,    0,    2,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    5,    5,    5,    5,    5,    5,    2,    0,    4,
    2,    3,    3,    2,    2,    3,    3,    2,    3,    6,
    1,    2,    4,    3,    2,    2,    2,    2,    3,    1,
    3,    3,    3,    2,    2,    4,    4,    5,    1,    1,
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
    1,    1,    1,    1,    1,    3,    1,    1,    1,    1,
    1,    1,    4,    1,    3,    1,    2,    1,    1,    1,
    1,    1,    1,    1,    2,    2,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    1,
    2,    1,    1,    1,    1,    1,    2,    1,    1,    1,
    1,    1,    4,    5,    5,    5,    5,    5,    5,    5,
    5,    5,    5,    6,    1,    6,    1,    4,    7,    2,
    0,    4,    3,    6,    1,    1,    6,    8,    6,    1,
    1,    4,    6,    4,    5,    3,    5,    4,    5,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    0,    1,    0,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    0,  555,  556,  557,  561,  562,  563,    0,  627,  630,
  742,  743,  747,  748,  468,    0,    0,    0,    0,    0,
    0,  401,  573,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  122,    0,    0,  103,    0,  922,    0,
   22,  920,    0,    0,   74,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  481,    0,
    0,    0,   38,   35,   37,    0,   36,    0, 1010, 1030,
 1031, 1012, 1014, 1013, 1019, 1015, 1020, 1016, 1018, 1021,
 1017, 1022, 1023, 1024, 1026, 1027, 1025, 1028, 1029,    0,
    0,    0,    0,   33,   69,   67, 1002, 1011,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  638,    0,    0,    0,  685, 1004,    0,    0,    0,
    0,    0,  115, 1009,  564,    0,    2,    0,    0,    0,
    0,    0,    0,  431,  432,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  455,  449,  450,  451,  452,  453,  454,  456,  457,
    0,  429,  430,    0,  994,    0,  408,  998,    0,  405,
 1001,    0,  687,    0,  404, 1005,    0,  571,  918,    0,
  158,  111,    0,  112,    0,  208,  990,  779,    0,  110,
  153,  997,    0,    0,  106,    0,  113,  924,    0,  107,
    0,  108,    0,  109, 1008,  154,  114,  730,  731,    0,
  923,  921,    0,   75,    0,  995,    0,  993,  489,  482,
    0,  488,  493,  483,  484,  491,  485,  486,  487,  492,
    0,    0,    0,   31,    0,   40,  766,   39,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  928,    0,  908,  632,  632,  633,  643,
  645,  981,  646,    0,  647,  637,  633,    0,    0,    0,
    0,    0,    0,    0,  590,  579,    0,  582,  584,  585,
  586,  587,  593,  594, 1003,  686,    0,    0,  116,    0,
  117,    0,  118,    0,    0,    0,  617,    0,    0,  983,
    0,    0,  624,    0,    0,    0,    0,    0,  469,    0,
    0,  371,    0,  211,    0,  411,  402,  466,  467,    0,
    0,    0,    0,    0,    0,  420,    0,  377,  378,    0,
  577,  575,  919,    0,    0,    0,    0,    0,    0,    0,
  161,  140,    0,  104,    0,    0,  119,    0,  129,  128,
    0,    0,    0,    0,    0,    0,  986,   76,    0,    0,
    0,    0,  745,  490,    0,    0,    0,    0,  885,  915,
    0,  985,    0,  887,    0,    0,  916,    0,  909,  900,
  899,  898,    0,    0,    0,    0,  890,  891,  892,  901,
  902,  910,  911,    0,  913,  914,    0,    0,    0,  917,
    0,  912,    0,    0,    0,    0,    0,  904,    0,  776,
  881,  882,  883,  884,  889,  888,  360,  867,    0,    0,
  866,    0,    0,  863,  864,  865,  868,  869,  870,  903,
  929,  930,  931,  932,  945,  947,  955,  956,  960,  961,
  209,    0,    0,    0,  364,    0,    0,    0,  769,    0,
    0,   28,    0,    0,    0,   72,   70, 1006,   68,   34,
    0,    0,    0,  771,    0,   53,    0,    0,    0,    0,
    0,  569,    0,    0,    0,  640,  642,    0,  649,    0,
    0,    0,  650,  636,    0,  616,    0,  591,    0,  588,
    0,  589,  614,    0,  609,    0,    0,  583,  780,  751,
    0,    0,    0,    0,    0,    0,  566,  565,    0,  618,
  622,    0,    0,  727,  629,  628,    0,  176,    0,    0,
  137,    0,  474,    0,  475,  470,    0,    0,  415,  417,
  416,    0,  418,    0,  412,    0,    0,  263,    0,    0,
    0,  424,  425,    0,  459,    0,    0,    0,    0,    0,
    0,    0,  382,  381,  477,    0,    0,  157,    0,    0,
  166,    0,  165,    0,    0,  172,  155,    0,  170,  162,
    0,    0,  512,  547,  515,    0,  548,    0,    0,  516,
    0,    0,    0,  539,  551,  511,  534,  513,  514,  509,
  496,  371,  494,  495,  497,    0,    0,  506,  507,  508,
  510,    0,    0,    0,    0,    0,  120,    0,  121,    0,
  132,    0,    0,  192,    0,    0,  369,    0,  211,    0,
    0,   84,    0,  102,    0,  101,  987,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  782,  784,  793,
  796,  797,  798,  799,  800,  801,  802,  803,  804,  805,
  806,    0,    0,    0,    0,    0,  905,    0,    0,    0,
    0,    0,    0,  906,  907,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  325,
 1007,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  268,  504,    0,    0,    0,  362,    0,    0,    0,
  768,   29,    0,    0,   45,   73,    0,    0,   27,    0,
  774,    0,    0,  741,    0,    0,    0,    0,    0,  568,
    0,  632,    0,  654,    0,    0,    0,    0,  559,    0,
  926,  558,    0,  592,    0,  600,  596,  595,    0,    0,
    0,  611,  599,    0,    0,    0,  750,    0,    0,    0,
  369,    0,    0,    0,    0,    0,  138,    0,  473,  371,
  414,    0,  371,  372,  419,    0,    0,  259,  369,  232,
  991,    0,    0,  371,    0,    0,  200,  369,    0,    0,
    0,    0,  237,  231,  234,  235,  236,  423,  464,  463,
  422,  421,    0,    0,    0,    0,    0,    0,  380,    0,
  371,    0,  163,  167,    0,    0,  171,    0,    0,  761,
    0,  535,  536,  552,    0,  549,    0,    0,  542,  540,
  541,    0,    0,    0,    0,  505,    0,    0,    0,    0,
  543,    0,    0,  369,    0,  191,    0,  194,  196,  197,
    0,    0,    0,    0,    0,  211,   79,    0,    0,   78,
    0,    0,    0,    0,  783,  795,    0,    0,  856,  857,
  858,    0,  787,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  707,  709,    0,  704,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  973,  974,  975,  976,  977,  978,  979,  980,
    0,    0,    0,    0,    0,    0,  966,    0,    0,    0,
    0,  879,  327,    0,  880,  953,    0,  875,  989,    0,
    0,    0,    0,    0,    0,    0,  886,  778,  777,  366,
  365,    0,  266,    0,    0,    0,  274,  770,    0,    0,
    0,    0,    0,    0,    0,    0,   54,   62,   64,    0,
    0,   71,    0,    0,    0,  773,    0,    0,    0,    0,
    0,    0,    0,  641,  644,  999,  653,    0,    0,    0,
    0,  659,  660,  661,  662,  671,  672,  673,    0,    0,
  651,  560,  927,  601,  610,  605,  604,    0,  615,  781,
  752,    0,    0,  369,    0,    0,  634,  729,  728,    0,
  472,    0,    0,  175,    0,  179,    0,    0,    0,  413,
  369,  369,  271,  372,  264,    0,  202,  371,  204,    0,
  372,  199,  205,    0,    0,    0,    0,  426,  427,  371,
  369,  372,    0,    0,    0,  398,    0,  187,  188,  164,
    0,  174,  523,  524,  925,    0,    0,    0,    0,    0,
  550,    0,    0,  371,  372,    0,  500,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  189,    0,    0,    0,   83,    0,    0,    0,    0,   91,
   85,    0,    0,   90,   86,    0,  746,  992,    0,    0,
  788,    0,    0,  794,    0,    0,    0,    0,    0,  832,
  831,    0,    0,    0,  841,  859,    0,  851,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  786,    0,
  708,    0,  744,    0,    0,  211,    0,  933,    0,    0,
  948,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  962,    0,    0,    0,    0,    0,    0,
    0,  626,  952,  893,    0,  261,  280,    0,    0,  361,
  275,   66,   58,   59,   65,   60,   61,    0,    0,   23,
    0,    0,    0,   43,    0,    0,    0,  755,  775,    0,
  754,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  688,    0,    0,    0,    0,    0,    0,    0,
  607,  608,  612,    0,    0,  190,    0,    0,    0,    0,
    0,  180,    0,    0,    0,  409,  214,  215,  216,  217,
  218,  219,  220,  896,    0,    0,  265,    0,    0,  212,
    0,    0,    0,    0,  238,    0,  460,  461,  428,    0,
    0,    0,    0,    0,    0,  371,  479,  372,  173,  988,
    0,    0,  518,    0,  517,  553,    0,  545,    0,    0,
    0,  498,    0,  526,    0,    0,  528,  531,  533,  369,
    0,    0,    0,  127,    0,  195,  371,    0,  370,  105,
    0,  125,    0,   94,    0,    0,    0,    0,   88,   89,
    0,  849,  850,  790,    0,  792,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  852,    0,    0,    0,    0,
    0,    0,    0,  842,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  715,
    0,    0,    0,  939,  938,    0,    0,  935,  934,    0,
  943,  942,  941,  940,  937,  936,    0,    0,  968,    0,
    0,    0,  967,    0,    0,    0,    0,  267,    0,    0,
  277,  278,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  359,    0,    0,    0,    0,
  286,  291,  285,    0,    0,    0,  295,  296,  297,  298,
  299,  300,  302,  303,  305,  306,  307,  310,    0,    0,
    0,  353,  354,  355,    0,    0,   57,   63,   55,   56,
   24,  760,  759,    0,  757,  765,  764,  762,  740,    0,
    0,    0,  663,    0,    0,    0,  693,  691,  689,    0,
  690,    0,    0,    0,    0,    0,  749,    0,  370,  733,
  734,  635,    0,  185,  181,    0,  372,  897,  269,  270,
  372,    0,    0,    0,  223,    0,    0,  239,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  476,  520,    0,
  521,    0,  142,    0,    0,    0,  146,  148,  149,  150,
  501,  503,  499,  530,    0,    0,  372,    0,    0,    0,
  367,  371,   92,    0,    0,   98,   93,    0,    0,    0,
    0,    0,  860,    0,  861,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  717,  706,    0,    0,  718,
    0,  714,  210,  954,  944,  946,    0,    0,  964,  963,
    0,  969,  957,  959,    0,    0,  276,    0,  314,    0,
    0,    0,  309,    0,    0,  308,  339,  340,    0, 1000,
  356,  357,    0,  294,  301,    0,  304,  287,    0,  292,
    0,  344,  293,  311,    0,    0,  337,   41,    0,    0,
  683,  681,    0,  674,    0,    0,    0,  697,    0,  631,
  372,  368,  735,  182,  372,  152,  258,    0,    0,  198,
  224,    0,    0,    0,    0,  225,  228,    0,    0,  372,
  396,  397,  393,  392,  395,  394,    0,  519,  546,  151,
  139,  147,    0,  371,  255,  134,  372,  373,    0,    0,
   96,    0,   80,    0,  818,  826,  625,    0,  819,  827,
  822,  830,    0,    0,  820,  828,  821,  829,  815,  823,
  816,  824,  817,  825,  713,    0,    0,    0,  949,  965,
    0,    0,  283,  279,    0,  312,    0,    0,    0,    0,
    0,  341,    0,    0,  332,  333,    0,  352,    0,    0,
    0,    0,  347,  345,    0,    0,    0,  756,    0,  665,
  675,    0,    0,  676,  677,  678,    0,  256,  410,  201,
  206,  226,    0,    0,  229,    0,  241,  243,  400,  372,
    0,  375,    0,   77,   99,  100,    0,    0,    0,  720,
  958,  282,    0,  313,    0,    0,  316,    0,  322,    0,
  324,    0,    0,  351,  350,  996,  349,    0,    0,    0,
    0,    0,    0,  664,    0,    0,  698,  230,    0,    0,
    0,    0,    0,  257,  372,    0,    0,  719,    0,    0,
    0,    0,    0,    0,    0,  335,  334,  346,  348,    0,
    0,    0,  669,    0,    0,    0,    0,    0,  240,    0,
  247,    0,  246,  376,  126,  722,    0,  284,    0,    0,
  315,  319,    0,    0,    0,    0,    0,    0,  667,  680,
  227,    0,    0,  251,  250,  249,  723,    0,    0,    0,
    0,    0,  336,  670,  254,  252,  253,  330,  320,    0,
  342,  317,  318,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1447,   28,   29, 1448,
   31,   32,   33,   34,   35,   36,   37, 1449,   39, 1450,
  850,  130,  491,  995, 1240,  131,  744,  996,  132,  495,
  749,  492,  133,  134,  276, 1019,  146,  997,  998,  214,
  243,  135,  999,  136,  497,   85,  254,  398,  447,  660,
 1128,  448,  889,  631,  236,  866,  661,  662,  663, 1135,
 1130, 1567,  255,  665,  666,   77,  204,   78,  250, 1787,
 1350,  235,  240,  242,  244,  230,  222,  224,  237,  247,
  163,  329,  331,  333,  387,  647,  649,  488,  277,  813,
  568,  676, 1060,  391,  652, 1119, 1344,  599,  771, 1090,
  560,  561,  581,  383,  632, 1105, 1545, 1670, 1286, 1546,
 1547, 1548, 1549, 1550,  677,  231,  223,  379,  607,  380,
  381,  608,  609, 1319,  562, 1054, 1055, 1056, 1281,  393,
  877,  878,  879,  880,  815, 1071, 1523,  633,  817,  450,
  882,  968,  482, 1407, 1287,  451,  452,  453,  454,  455,
  456, 1524, 1525,  584, 1676, 1821, 1305,  823,  824,  810,
  825,  826,  827, 1822, 1823, 1875,  361,  487,  985, 1451,
  210,  577,  982,  578, 1064,  483,  986,  987, 1228, 1430,
 1431, 1432, 1734, 1137, 1452, 1453, 1454, 1455, 1641, 1456,
 1457, 1458, 1459, 1460, 1461, 1462, 1463, 1464, 1465, 1466,
 1467, 1468, 1469,  457, 1832, 1470, 1744, 1866, 1741,  458,
  720,  459,  460, 1471, 1642, 1752, 1753, 1805, 1472, 1473,
 1474, 1633,  273,  484,  485,  461,   41, 1349, 1561,  370,
  595,  841, 1317,  596,  839,   82,   62,  365,  215,  173,
  348,  201,  355,  207,  804,  801,  356,  366,  831,  367,
  591, 1082,  202,  462,  349,  597, 1318,   99,  213,  259,
  634,  635,  733, 1106,  734, 1107, 1108,  638,  639,  640,
  641, 1483, 1099, 1097,  642,  643,  644,  859,  645,  856,
   42,   43,   44,   45,   46,   47,  165,  548,   64,  513,
  218,  372,  316,  317,  318,  319,  320,  321,  322,  323,
  324,  777,  534, 1038, 1273,  535,  536,  782,  169,  337,
  550,   48,  172,  343, 1373,  719,   49,  341,   50,  150,
  304,  521,  767,  402, 1270, 1506,  693,  516,  306, 1278,
 1663,  151,  152,  305,  522,  523,  765, 1017, 1020, 1021,
 1022, 1023, 1024, 1025, 1759, 1814,  463, 1844, 1026, 1027,
 1028, 1268, 1764, 1765, 1766, 1260,  159, 1498, 1657, 1658,
 1400,  927, 1401, 1402, 1403, 1602, 1788, 1790,  553,  554,
 1048, 1512,  503,   51,   52,  403,   53,   54,  542,  543,
 1004, 1248, 1484, 1488,  504,  752,  228,  678,  679,  904,
  905,  680,  681,  682,  683,  684,  685,  686,  687,  688,
  689,  690,  691, 1153, 1155, 1156,  464,  465,  466,  467,
  468,  469,  470,  252,  471,  472,  473,  474,  475,  951,
  476, 1608,  477,  478,  479,  480,  709,  955, 1217, 1212,
  710,  138,
};
static short yysindex[] = {                                   5950,
 1300, -167, 2468,   77, 1361,   50, 3387,  -90, 9082,  758,
  101, 8853, -167, 1266,  630, 8544, 1243, 8544,  -86,    0,
  496,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  401,    0,    0,    0,    0,    0,    0,  360,    0,    0,
    0,    0,    0,    0,    0,15075, 8544, 8544, 8544, 8544,
 8544,    0,    0,  517, 1315, 8544, 8544, 8544,  732, 8544,
  512, 8544, 8544,    0, 8544, 8544,    0,  563,    0,  741,
    0,    0,  739, 8544,    0, 8544, 8544, 8544,  799, 8544,
 8544, 8544, 8544,  512, 8544, 8544, 8544, 8544,    0, 8544,
 1061,  735,    0,    0,    0, 1163,    0, 1163,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  861,
  875,  118, 1216,    0,    0,    0,    0,    0, 8544, 8544,
 8544,  904,  947,  951,  173, 1023,  906,  145,  150,  984,
  868,    0, 8544, 1075, 2129,    0,    0, 8544, 1011, 8544,
 8544, 8544,    0,    0,    0, 8544,    0,  997,  992, 1181,
 1117,  945, 1204,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  765,    0,    0, 1315,    0,  431,    0,    0, 1455,    0,
    0, 1282,    0,  718,    0,    0, 1090,    0,    0, 1121,
    0,    0, 1200,    0, 1349,    0,    0,    0, 1315,    0,
    0,    0,  341, 1455,    0,  460,    0,    0, 1326,    0,
 1330,    0, 1163,    0,    0,    0,    0,    0,    0, 1346,
    0,    0, 1240,    0, 1400,    0, 1294,    0,    0,    0,
 1240,    0,    0,    0,    0,    0,    0,    0,    0,    0,
14145, 8544, 1354,    0, 8544,    0,    0,    0, 8153, 1366,
 8620, 8544, 1114, 1427, 1735, 1479, 1471, 8153, 1416, 8620,
 1439, 1443, 1323,    0,14384,    0,    0,    0,    0,    0,
    0,    0,    0,12222,    0,    0,    0, 1399, 8544, 1473,
 1409,  797, 8544, 1419,    0,    0, 2129,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 8544, 1455,    0, 1326,
    0, 1163,    0, 1488, 1493, 1372,    0,14384, 1470,    0,
 1470,14384,    0, -111, 1374, 1379, 1375, 1204,    0, 1606,
  620,    0, 8544,    0,  765,    0,    0,    0,    0, 8544,
 1354, 4945, 1426, 5234, 1849,    0,15075,    0,    0,  998,
    0,    0,    0, 1840, 1840, 1315, 1546, 1315,  509, 1200,
    0,    0,10208,    0, 8544, 8544,    0, 1354,    0,    0,
 1636,  734, 1890, 8544, 1674, 8544,    0,    0, 1901, 3916,
 8720, 1553,    0,    0, 1904, 1911, 1913, 1492,    0,    0,
 1917,    0, 1919,    0, 1928, 1938,    0, 1942,    0,    0,
    0,    0,    0, 1945, 1537, 1544,    0,    0,    0,    0,
    0,    0,    0, 1947,    0,    0,12461, 1959, 1962,    0,
 1963,    0,14384,14384,12699, 8544, 1975,    0, 1445,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1973, 1929,
    0,    0, 1976,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10208, 1958, 1312,    0, 1981, 1768, 1599,    0, 8544,
 1605,    0, 8544, 8544,   54,    0,    0,    0,    0,    0,
 8544, 8544,  556,    0, 8544,    0, 1717, 8544, 1994, 8153,
 1651,    0, 8544,12699, 1585,    0,    0,14384,    0, 2213,
 1730, 2006,    0,    0,   88,    0,  400,    0,  407,    0,
  407,    0,    0, 2018,    0,  156, 1643,    0,    0,    0,
 2010,  148, 2005, 1354, 2027, 1807,    0,    0, 8544,    0,
    0, 1746, 2029,    0,    0,    0, 1322,    0, 1731, 1737,
    0, 1315,    0, 1745,    0,    0, 1798, 1815,    0,    0,
    0, 1797,    0,10208,    0,10208, 1622,    0, 1828, 8544,
14936,    0,    0,  836,    0, 8544, 1073, 1282, 1705, 1669,
 1652,    0,    0,    0,    0,  792, 1708,    0, 1649,  512,
    0, 1315,    0, 1830, 1763,    0,    0,  509,    0,    0,
  222, 1677,    0,    0,    0, 1678,    0, 1726, 2079,    0,
 1775, 2081,  491,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2032,  120,    0,    0,    0,
    0,  170, 2084, 2089, 2081, 1455,    0, 1816,    0, 1869,
    0, 1315, 1315,    0, 4945,  718,    0, 1759,    0, 1736,
 2098,    0, 5676,    0, 1761,    0,    0, 1776, 2107, 8958,
 2112,    0,    0,    0, 8475, 5799,   22,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  515, 1773, 1185,14384,  465,    0, 8544,14384, 1210,
 1351, 1358, 2403,    0,    0,14384, 9197, 1445, 1711,  -14,
14384,14384, 9197,  828,  828, 1657, 1644,  360, 2115,    0,
    0,12953, 8544,14384,14384,14384,14384,14384,14384,14384,
 6128,    0,    0, 1883, 2101, 8544,    0, 8544, 1867, 8544,
    0,    0, 9319, 1741,    0,    0, 1714, 8620,    0, 1859,
    0, 1645, 2134,    0, 1859, 9319, 1868, 1872, 8544,    0,
 1657,    0, 1833,    0, 8544, 2086, 1747,14384,    0,  666,
    0,    0,  -61,    0, 1753,    0,    0,    0, 8544,  777,
 8544,    0,    0, 8544, 8544, 1553,    0,14384, 1931, 4945,
    0, 2152, 1703,14384,14384, 1315,    0,  668,    0,    0,
    0, 2158,    0,    0,    0,  196, 8544,    0,    0,    0,
    0, 1930, 2161,    0, 1907, 1924,    0,    0, 1866, 1875,
 1163, 1937,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  512, 8544,15017, 1848, 1850, 1851,    0,  512,
    0, 1469,    0,    0, 1809, 1315,    0, 3916,  106,    0,
 1799,    0,    0,    0,  512,    0, 2079,  136,    0,    0,
    0, 1941,  136, 1824,  136,    0, 2188,  512,  512,  512,
    0, 1354, 8544,    0, 1888,    0, 1650,    0,    0,    0,
  998, 1470, 8544, 1667,  253,    0,    0, 2201,   61,    0,
 3916, 8544, 1470, 9435,    0,    0, 1470, 8958,    0,    0,
    0, 5510,    0,  254,  532,14384,14384, 9694, 2204,  698,
 9953,14384,10212,  939,10471,10730,13191,10989,11248,11507,
 9197, 9197,    0,    0, 1885,    0, 2205,  945,14384,14384,
 2517,14384, 2208,14384, 2207, 1855,14384,14384,14384,14384,
14384,14384,    0,    0,    0,    0,    0,    0,    0,    0,
 1940, 1520,  531,14384,    3,14384,    0, 9197, 1873, 2454,
  174,    0,    0,  945,    0,    0, 1671,    0,    0, 1982,
  828,  828, 1093, 1093, 1445,  209,    0,    0,    0,    0,
    0, 1675,    0, 1832, 2002, 1867,    0,    0, 8544, 8544,
 8544, 8544, 8544, 8544,  160, 2232,    0,    0,    0, 9319,
 1902,    0,  -21, 1921, 8544,    0,14384, 1921, 2241, 2232,
 9319, 5812, 1969,    0,    0,    0,    0, 2039, 2247, 2244,
  932,    0,    0,    0,    0,    0,    0,    0, 9197, 1968,
    0,    0,    0,    0,    0,    0,    0,  701,    0,    0,
    0, 1773, 1445,    0, 1679, 1470,    0,    0,    0, 1445,
    0, 1840, 2030,    0,  668,    0, 2257, 9197, 3528,    0,
    0,    0,    0,    0,    0, 1867,    0,    0,    0, 3528,
    0,    0,    0, 1163, 1163, 1806, 2262,    0,    0,    0,
    0,    0,  -17,  -73,  866,    0, 2040,    0,    0,    0,
 8544,    0,    0,    0,    0,   77, 1680, 1889, 1883, 2264,
    0, 1649, 1693,    0,    0,  359,    0, 2252, 8544, 1212,
  512, 2270, 2273, 2275, 2054,  718, 1867, 1840, 1737, 4945,
    0, 1708, 1876, 2556,    0,  512, 1932, 2015, 5676,    0,
    0,  512,  845,    0,    0, 2017,    0,    0, 2283, 2285,
    0, 9197, 9197,    0, 9197, 9197, 1884, 1445, 2287,    0,
    0, 1445, 2290,12699,    0,    0, 2014,    0,  725, 2297,
 1445, 2299, 1535, 2300, 1445, 2302,14384,14384, 2204,14384,
13430, 2306, 1445, 2308, 2310, 1445, 2311,14384, 1445, 2312,
 1445, 2319, 2321, 1445, 2328, 2329, 1445, 2330,    0, 2111,
    0,11436,    0, 1905, 1934,    0, 1971,    0, 1984,14384,
    0, 2012, 2035, 2043, 2061, 2236, 2348,14384,14384,14384,
 1445, 1948,14384,    0,14384, 1445, 2069,  782,14384,14384,
14384,    0,    0,    0, 8544,    0,    0, 8544,11122,    0,
    0,    0,    0,    0,    0,    0,    0, 2057, 9319,    0,
 9319,  160, 2232,    0,  261,  261,  171,    0,    0, 1702,
    0, 2241, 2232, 2350, 5812, 2356,  932, 2357,14384, 2135,
 2086, 2023,    0, 2023,  884, 2023, 2066, 2067, 2137, 2085,
    0,    0,    0,  945, 1867,    0, 1876,  948, 1649, 2036,
 1649,    0, 9197,  461, 1486,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3528, 3528,    0, 2002,14384,    0,
 2130, 2020, 1806,  563,    0, 9197,    0,    0,    0, 2037,
 2042, 2047, 2051, 2056, 2058,    0,    0,    0,    0,    0,
 2397,   77,    0, 1991,    0,    0,  136,    0, 3528, 2070,
  136,    0,  136,    0, 1883, 2402,    0,    0,    0,    0,
  998, 2002, 1649,    0, 1737,    0,    0, 2170,    0,    0,
 2152,    0, 2407,    0, 2409, 1315,    4, 2411,    0,    0,
 1315,    0,    0,    0, 2111,    0, 2111,14384, 1470, 1470,
 1729,  360, 2412,14384, 2138,    0, 1470, 1470,14384, 1470,
 1470, 2167, 1445,    0, 1578,14384, 1445, 1470, 1470, 1470,
 1470, 1445, 1470, 1470, 1470, 1470, 1470, 1470, 8544,    0,
 2415, 2419,  960,    0,    0,10208, 2420,    0,    0, 2366,
    0,    0,    0,    0,    0,    0, 2417, 1424,    0,14384,
 2160, 2045,    0,14384, 2428, 1757, 2608,    0,10208, 2422,
    0,    0, 2176,    0, 4356,  315, 2433, 2447, 2157,14384,
 2443, 2444, 4796, 8544, 8544,    0, 2445, 2448, 2172, 2452,
    0,    0,    0, 2209,11382, 2453,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2456,  -67,
 2458,    0,    0,    0,    0, 2128,    0,    0,    0,    0,
    0,    0,    0, 2091,    0,    0,    0,    0,    0, 5812,
 2350, 2135,    0, 1762, 8544,  932,    0,    0,    0, 2183,
    0, 2086, 2086,14384, 9197, 1553,    0, 2002,    0,    0,
    0,    0, 1469,    0,    0,  470,    0,    0,    0,    0,
    0, 1445, 2248, 2130,    0,  943, 8544,    0, 2191,  622,
 2094, 2203, 2116, 2237, 2215, 2253, 1867,    0,    0, 2511,
    0, 2520,    0, 2217, 2248, 2070,    0,    0,    0,    0,
    0,    0,    0,    0, 1867, 1708,    0, 1469, 2040, 2185,
    0,    0,    0, 2210, -105,    0,    0, 1883, 2211, 1445,
 2525, 2528,    0,  945,    0, 1445,14384, 2531, 2532, 1445,
 2533, 2538,14384,14384, 1445, 2541, 2543, 2550, 2551, 2557,
 2558, 2563, 2566, 2567, 2568,    0,    0,11436, 2570,    0,
 8544,    0,    0,    0,    0,    0,14384, 2573,    0,    0,
14384,    0,    0,    0,14384,  350,    0, 2282,    0,13668,
 8544,14384,    0, 9197, 1445,    0,    0,    0, 2552,    0,
    0,    0, 8544,    0,    0, 4056,    0,    0,  944,    0,
 2173,    0,    0,    0,  -84, 2575,    0,    0,  261, 8544,
    0,    0, 2086,    0, 1097, 1445, 2576,    0,   22,    0,
    0,    0,    0,    0,    0,    0,    0, 2578, 8544,    0,
    0, 2274, 2281, 8544, 1806,    0,    0, 1163, 8544,    0,
    0,    0,    0,    0,    0,    0, 2002,    0,    0,    0,
    0,    0, 2002,    0,    0,    0,    0,    0,14384, 1315,
    0, 2166,    0, 1315,    0,    0,    0, 1445,    0,    0,
    0,    0, 1445, 1445,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 8544, 2294, 1445,    0,    0,
 2441, 3528,    0,    0, 2588,    0, 2697,14145, 1123,  637,
 2574,    0, 2293, 1199,    0,    0, 2586,    0, 8544, 8544,
   77,  243,    0,    0, 2136, 2298, 9197,    0, 2595,    0,
    0, 9197, 2599,    0,    0,    0,14384,    0,    0,    0,
    0,    0, 1806, 1163,    0, 2268,    0,    0,    0,    0,
 2040,    0, 1445,    0,    0,    0, 2603, 2613, 2617,    0,
    0,    0, 1470,    0, 2258, 4056,    0, 2240,    0, 4056,
    0, 4056, 3156,    0,    0,    0,    0,11122,  944,14384,
 4056,  664, 8544,    0,   22, 8544,    0,    0, 2268,  451,
 1806, 2288, 2295,    0,    0, 2622, 8544,    0, 8544, 2632,
 2856, 2616, 1217,11122, 2635,    0,    0,    0,    0, 1265,
  231, 2380,    0, 1764, 1772, 1806,  -82,  -82,    0, 2391,
    0, 2256,    0,    0,    0,    0, 2650,    0, 4056, 2635,
    0,    0, 2388, 4056, 2414, 2392,11122, 8544,    0,    0,
    0, 2242,  823,    0,    0,    0,    0, 1790,11122,  481,
 8544,11122,    0,    0,    0,    0,    0,    0,    0,11122,
    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  165, 1015,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  176,11745,    0,    0,    0,    0,    0,    0,
 2706,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  227,    0,    0,    0,    0,    0,    0,   99,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  310,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2374,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    5, 2332,    0,    0,    0,  159,    0,  159,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2336,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  972,    0,    0,13907,
11984,    0,    0,    0, 1005,    0,    0,  180,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  936,    0,
 2307,  197,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  457,    0,
    0,    0,    0,  406,    0,    0, 1008,    0,    0,    0,
    0,    0,  135,    0,10726,    0,    0,    0,    0,    0,
    0,    0,    0,  457,    0,  367,    0,    0, 2679,    0,
    0,    0, 2460,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  139,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1162,    0, 2461,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  838,    0,    0,    0,    0,    0,
    0,    0, 9582,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  686,    0,    0, 1176,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  457,    0, 2679,
    0, 2460,    0, 1239,  313,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1287,    0, 2446,
    0,    0,    0,    0, 1338,    0,    0,    0,    0,    0,
 2461,  803,15142,    0, 1371,    0,    0,    0,    0,  242,
    0,    0,    0, 2276, 2276,    0,    0,    0, 1381,  147,
    0,    0,    0,    0,    0,    0,    0, 2461,    0,    0,
 1403,    0,    0,    0,    0,    0,    0,    0, 9690,    0,
    0,   41,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1863,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2641,    0,    0,    0, 2941, 3241,    0,
 3541,    0,    0,    0,    0,    0,    0,    0,  415,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  134, 3841,
    0, 2341,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1391,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 3756,
    0, 2410,    0,    0,    0,    0,    0,    0,  753,    0,
  753,    0,    0, 1013,    0, 1047, 1104,    0,    0,    0,
    0,  139,    0, 2461,    0,    0,    0,    0,    0,    0,
    0,  188,  105,    0,    0,    0,  244,    0,    0,  909,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2446,    0,    0, 1380,    0, 2266,    0,
    0,  589,    0,    0,    0,    0,  427,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1401,    0,    0,
 4732, 4624,    0,    0,    0, 9959,    0,    0,10995,    0,
    0,12980,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2747, 2818,    0,    0,    0,
    0,14736,10477,    0,12980,  457,    0,    0,    0,    0,
    0,    0,    0,    0,  803, 1337,    0,    0,    0,    0,
 2326,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 6147, 6396, 6542,    0,    0,  617,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1959,  107,14622,    0,14622,    0,    0,    0,14622,
14622,14622,    0,    0,    0,    0,    0, 2292,    0,    0,
    0,    0,    0, 5030, 5319, 2689,    0,   65,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2866,    0,    0,    0,    0, 2467,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  255,
    0,    0,    0,    0,  255,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  685,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  844,
    0,    0,    0,    0,    0,   41,    0,    0,    0,  803,
    0,  488,  142,    0,    0,    0,    0,  603,    0,    0,
    0,    0,    0,    0,    0, 1795,    0,    0,    0,    0,
    0, 2698,    0,    0, 1608, 2052,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  389,  423,  679,    0,    0,
    0,  271,    0,    0,    0,    0,    0,    0,    0,    0,
 7135,    0,    0,    0,    0,    0,10995,    0,    0,    0,
    0,  808,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2461,    0,    0,  322,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 8811,    0,    0,    0,  295,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2701,    0,    0, 1425,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2707,    0,    0,    0,    0,    0, 4141,
 5608, 5897, 4441, 4741,  846,    0,    0,    0,    0,    0,
    0,    0,    0, 9777,    0, 2480,    0,    0,    0,    0,
    0,    0,    0,    0, 1468, 1472,    0,    0,    0,    0,
    0,    0,    0, 1477,    0,    0,    0, 1477, 1500, 1512,
    0,    0,    0,    0,    0,    0,    0,    0, 6514,  195,
  970,    0,    0,    0,    0,    0,    0,    0,    0, 1218,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  107,  161,    0,    0,    0,    0,    0,    0,  384,
    0, 2276, 2304,    0,  889,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2467,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1303,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1514,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 8435,    0,
    0,    0,    0,    0,    0,    0,    0, 1229,    0,    0,
    0,    0,    0,    0,    0, 1337, 2467, 2276, 1516,  803,
    0, 2488, 1524,    0,    0,    0, 8811,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  610,    0,    0,
    0,  886,    0,    0,    0,    0,    0,    0,    0,    0,
 1190,    0, 2100,    0, 6997,    0,    0,    0,    0,    0,
    0,    0, 7069,    0,    0, 7100,    0,    0, 7273,    0,
 7304,    0,    0, 7403,    0,    0, 7434,    0,    0, 1733,
    0, 1959,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2331,    0,    0,    0,    0,  729,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -23,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1468, 1525,    0, 1399,  276,    0,    0,    0,    0,
    0, 1528, 1534, 1557,    0,    0, 2416, 1818,    0, 6793,
    0, 2418,    0, 2418, 2416, 2418,    0,    0,    0,  542,
    0,    0,    0, 1425, 2467,    0, 1524,    0,    0, 2313,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  994,    0, 1303, 2374,    0,    0,    0,    0,    0,  755,
  764,  768,  807,  833,  914,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  683,
    0,    0,    0,    0, 3118,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1560,    0,    0,    0,    0,    0,
 1594,    0,    0,    0,    0,    0,  208,    0,    0,    0,
    0,    0,    0,    0,  323,    0,  325,    0,    0,    0,
    0,   65,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 7533,    0, 7564,    0, 7663,    0,    0,    0,
    0, 7694,    0,    0,    0,    0,    0,    0, 1170,    0,
    0, 2720,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2723,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 6198,    0,    0,    0,    0,    0,    0,
    0, 1503, 1610,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  651,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1615,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1603, 6718,    0,    0,    0, 1420,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  182,    0,    0,    0,    0,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0, 2724, 1345, 1478,    0, 2446,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2467,    0,    0,    0,
    0,    0,    0,    0, 1631,  785,    0,    0,    0,    0,
    0,    0,    0,    0, 2467, 2488,    0,  386,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  623,    0, 7793,
    0,    0,    0, 2707,    0, 7824,    0,    0,    0, 7923,
    0,    0,    0,    0, 7954,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1959,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2708,    0,    0,    0,    0,
    0,    0,    0,    0, 2712,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2469,    0,    0,    0,    0,    0,    0,    0,  276, 8260,
    0,    0,    0,    0, 2416,  523, 1382,    0, 1378,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3120,    0,    0,  879,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  224,    0,    0,    0,    0,    0, 8053,    0,    0,
    0,    0, 8084, 8183,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1803, 2725,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  113,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  963,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 3120, 1556,    0,  155,    0,    0,    0,    0,
    0,    0, 2724,    0,    0,    0, 2731,    0,    0,    0,
    0,    0,    0,    0, 2716,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   42,    0,    0,
    0,    0,    0,    0, 8214,    0,    0,    0, 1637,    0,
 1303, 1899, 2024,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   42, 2717,    0,    0,    0,    0,    0,
 2477,    0,    0,    0,    0, 3120,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2727,
    0,    0,14754,    0,    0,    0,   42,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   42,    0,
    0,   42,    0,    0,    0,    0,    0,    0,    0,   42,
    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2779,    0,    0, 2782,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2783,
   -3,  331,   71, -685, 1547,  688, -317,   39, 2784, 2505,
    0,    8,    0, 2514, -102,  -40,    0,  300, -778,  -34,
  -72,  -43, -216, 2516, 2053,    0,    0,    0,  557,    0,
    0,  -45, 1673, -553,  -66, -970, 2141, 1918, -327,    0,
 1676,    0, 2715, 1916, 1961,    0,  -62,    0, -516,  -50,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -537,   81, 1686,
 -320, 1054, -928,    0,    0,    0,    0, -335, -135,-1216,
 1695, -358,  -51,    0, -550,    0,    0, 1270, -997,    0,
 1271,    0,    0, 2466, -670,  -37,    0,    0,    0,    0,
 2439,    0, 2212, 1715,    0,    0,    0, 1771,    0, 2497,
 2038, 1709, 2478, 2484,    0, 1215,    0, -453, 2044,  -44,
 -631, 1161, -246,    0,    0, -958, -955, -946, -933, -903,
 -896,    0, 1324, -847,    0, 1020,-1247, 1325,    0, 2267,
    0,    0,    0, 1029, 1039, 1017, -107, -250, -895, -882,
    0,    0,    0, 2068,    0, -646,    0, 1892,    0,    0,
    0,    0,    0,-1184, -966,    0,    0,    0,    0, -242,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1138,    0,    0,   30,    0,    0, -476,
    0, -411,  949, 1437, 1241,    0, 1072,    0,    0,    0,
    0,    0,    0,    0, 2147, -138, -427, 1607, 1377, -531,
 -711,-1004,-1314,    0,    0,   25,    0,    0,    0,    0,
    0, 2521,    0,    0,    0,    0, 2534, 2303,    0,    0,
    0,    0,    0,   -9, 2539,    0,    0,    0,    7,    0,
    0,    0, -366, 2028, -365, 1561, -490,    0,    0,    0,
    0, 2284,    0, -384,    0,    0,    0, 2254,    0, 2059,
    0,    0,    0,    0,    0,    0,  190,    0, 2884,    0,
    0,    0,    0,    0, 2583,    0,    0,    0,    0, 2375,
    0, 2373,    0,    0,    0, 2132,    0,    0,    0,    0,
    0, -256, -648, -856, 2082,    0, 1214,    0,    0,    0,
    0,    0,    0, -333,    0,    0, -748, -189,  608,    0,
    0,    0, 2757, 1304,    0, 2145,-1053,-1539,    0, -901,
 1897, -831,    0,    0,    0,    0, -154,    0,    0,    0,
    0, 1654,    0,    0,    0,    0, 2831,  478,    0, 1153,
 2229,    0, 1331,    0, 1523,    0,  -42,    0,    0, 2139,
    0,    0,    0,    0,    0, 2382,    0,    0,    0, -456,
 2175, 1923,-1101,    0,    0,    0, -715, -458,  201,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1124, 1765,    0,    0,    0, -891,    0,
    0,    0, 1647,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -973, 1720,
    0,    0,
};
#define YYTABLESIZE 15660
static short yytable[] = {                                     137,
   81,   38,  137,  326,  229,  278,  157,  206,  164,  298,
  301,  792,  297,  300,  225,  979,  636,  637,  209,  221,
  606,  226,  158,  156,  269,  260,  574,  266,  732,  234,
  814,  572,  246,  717,  772,  793,  953, 1042,  241,  600,
  705,  264,  961, 1433,  928,  751,  203,  205,  208,  211,
  157,  216,  265,  263,  270, 1528,  227,  205,  232,  271,
  211,  268,  157,  216,  496,  245,  137,  212, 1040,  964,
 1009, 1193, 1300,  496,  256,  754,  157,  258,  205,  239,
  256,  232,  208,  211,  540,  157,  216,  245,  137,  332,
  211,  983,  158,  156,  284,  238,  286,  748,  726,  705,
 1288,  267, 1230, 1289,  725,  726,  726, 1222,  517,  890,
  579, 1288, 1290,  576, 1289,  308, 1257, 1347,  238,  328,
  805,   80,  806, 1290,  881, 1291,  388,  816, 1325,  227,
  157,  227,  770,  326,  159, 1297, 1291,  650,  703,  726,
  395,  736, 1301,  232, 1485,  725,  160,  285,  325, 1096,
  211,  157,  245, 1309,  248, 1292,  164,  726,  357, 1046,
  753,  282, 1293,  725,  574,  726, 1292, 1294,  330, 1122,
 1298,  328,  541, 1293, 1063,  574, 1330, 1066, 1294,  687,
 1102,  705,  736,  384,  295,  736, 1073,  732,  718,  299,
  757,  785,  326,  159,  656,  248,  739,  703,  248,  781,
  736,  797,  767, 1239,  753,  160, 1495,  296,  786,  868,
  865,  895,  296,  248,  636,  637,  282, 1221,  529,  753,
  544, 1342,  705,  574, 1312, 1755,  619,  481,  732,  546,
  489,  732,  558,  296,  574,  656, 1419,  739,  687, 1421,
  705,  478, 1117,  737, 1697, 1701,  732, 1645,   95,  606,
 1189, 1190,  729,  656,  758,  739, 1062,  718,  526, 1631,
 1632,  849,  227,  822,   97,  227,   80,  619,  363,  157,
  186,  498,  137, 1843, 1802,  525, 1565,  746,  157,   63,
  498,  290,  539,  921,  737,  619, 1809, 1218, 1872,  956,
  957,  338, 1127,  789, 1144, 1245, 1664, 1519, 1520,  208,
  478, 1224,  737,  157,  100,   80, 1213, 1214,  573,  576,
  967,  862,  621,  758, 1242,  481, 1310,  226,  976,  587,
  533,  133,  589,  296,   14, 1252, 1254, 1873, 1884,  186,
  601, 1543,  603, 1133,  525,   87, 1288, 1288,   87, 1289,
 1289, 1696,  143,  227,  646,  658,  654,  541, 1290, 1290,
  227,  648,  227,  621,  227,  334,  338,  592,  507, 1496,
 1313, 1291, 1291,  789, 1553,  791,   86, 1103,  576,  531,
 1288,  621,  141, 1289, 1874,  211,  216, 1646, 1702, 1508,
  133, 1080, 1290,  738,  157,  186,  208, 1284,  385, 1538,
  667, 1292, 1292, 1274,  864, 1291,  664,  559, 1293, 1293,
  922,  656, 1331, 1294, 1294,  379, 1246,  363, 1134,  186,
 1732,  166, 1275,  726,  894, 1521, 1311, 1507,  718,  725,
  705,  338,  383,  101,  738, 1292,  399, 1775,  958, 1295,
 1296,  159, 1293, 1030,  726,  921,  721, 1294, 1650, 1141,
  725, 1013,  738,  160,  186, 1215, 1609,  385, 1743, 1308,
 1612, 1332,  745, 1526, 1123,  894,  736,  539,  894, 1557,
 1478, 1566, 1480,  842,  379, 1139, 1825,  745,  186, 1140,
  780, 1364, 1365,  894, 1366, 1367,  291,  736,  159, 1057,
  157,  383, 1059,  211,  498,  399,  338,  620, 1640,  290,
  160,  157, 1061, 1069, 1865,  157,  705,  742,  211,  747,
  157, 1517,  732,  164,  489,  478,  933,  894,  750,  656,
 1665,  739,  755,   95,  328, 1142,  798,  705,  703,  656,
 1087,  656,  699,  732, 1802, 1818,   79,  703,  620,   97,
  656,  496,  739,  843,  767,  326,  818,  294,  872,  227,
  753,  701, 1041, 1808,  726,  281,  620, 1758, 1249,  753,
  725, 1694,  922,  705,  167, 1095,  844, 1129,  737,  687,
  576,  705,  619,  699,  769,  574,  699,  186,  687, 1491,
  811,  227, 1014, 1849,  656,  890,  811,  726, 1428,  737,
  758,  699,  701,  656, 1341,  294, 1351,  736, 1666,  687,
 1256,  401, 1667, 1668,  294, 1250,  980, 1703, 1871,  294,
  701, 1655,  177,  574,  753, 1238,  875,  876,  867,  843,
  226,  726, 1516,  705,  186,  758,  702,  725, 1277,  726,
 1095, 1115,  186,  687,  238, 1661,  656,  705, 1695, 1556,
  541,  478, 1143,  732, 1033, 1530,  525, 1730,  656, 1126,
  656, 1687,  739,  159,  848,  227, 1070,  248,  621,  130,
  843,  703,  385,  843,  736,  160,  186,  702,  186, 1693,
  935,  177, 1680,  527,  379,  133,  527,  186,  843,  379,
 1654,   79,  619,  753,  379,  702, 1526, 1798,  384,  186,
  186,  732,  144,  848,  703,  597,  383,  358,  232,  737,
  399,  481,  687,  481,  705,  988, 1891,   87,  738,  144,
  732,  789,  760,  791, 1842,  186,  186,  656, 1555, 1621,
   79, 1478,  186,  969,  186,  168, 1279, 1707,  745,  738,
  260,  227,  921, 1574,  302,  703,  227,  389,  227,  186,
  227,  921, 1768,  498, 1792,  385, 1769,  384,  498,  170,
  539,  144, 1371,  703,  597,  226,  498, 1299, 1847,  211,
 1051, 1778,  602,  186,  390, 1016,  211, 1660,  621, 1307,
  359,  186,  481,  391,  932,  860, 1078,  387, 1782,  157,
  386,  157, 1232, 1288,  227, 1235, 1289,  775,  385,  186,
  227, 1890, 1343, 1329,  145, 1290,  533, 1529, 1039, 1079,
 1081,  171,  921, 1145, 1010,  379,  773,  227, 1291, 1129,
 1092, 1112, 1113, 1114, 1779,  604,  386,  143, 1426,  399,
 1780,  602,  383,  390, 1622,  314,  894,  404,  390,  130,
  130, 1761,  391,  620,  227,  227,  387,  248, 1292,  738,
  186,  861,  389,  292, 1659, 1293, 1542,  699,  667,  922,
 1294, 1838, 1552,  145, 1093,  895,  776, 1494,  922,  699,
  541, 1824,  558,  923, 1826, 1603,  701,  238,  699,  260,
  177,  249, 1487,  157, 1086,  386,  143, 1863, 1406,  727,
  896,  843,  177,  157,  728,  903,  924,  701,  767, 1100,
 1116,  667, 1138,  921, 1848,  810,  895,  664,  178,  895,
 1124,  389,  238,  238,  238,  527, 1854, 1372,  921,  177,
 1883, 1076,  699,  570,  895,  177,  518,  177,  471,  922,
 1146,  699, 1889,  388,  524, 1892,  571,  217,  186,  767,
 1236,  701,  767, 1893,  843,  921,  810,  843,  527,  810,
  701,  702,  925,  620,  843,  623,  843,  767,  895, 1541,
  843,  702,  384,  702,  810,  843,  177,  178,  843, 1233,
 1237,  153,  702, 1740,  699,  288, 1234, 1210,  371,  144,
  539,   79,  668,  843,   14,  338, 1033,  471,  699,  657,
  527,  572,  388,  701,  368, 1616,  623,  843,  990,  498,
  211,  216,  498,  245,  137,  843,  797,  701,  843,  843,
  498,  753,  233,  221,  623, 1537,  702, 1157,  843,  703,
  922,  498,  498,  668,  581,  702,  668,  578,  211,  703,
  657,  703,  603,  657,  843,  922,  430,  605,  390, 1052,
  703,  668,  350,  597, 1375,  527, 1559,  391,  657,  139,
  572,  387,  972,  972,  221,  699,  843,  221, 1243,  636,
  637,  843,  922,  921, 1336, 1158,  613,  369,  702, 1253,
  990,  154,  221,  843,  701,  843,  597,  253,  155, 1354,
  371,  145,  702,  581,  703, 1358,  578,  351,  384, 1159,
  386,  603, 1376,  703,  541,  226,  529,  653,  233,  489,
  527, 1320,  597,  143,  143,  597, 1812,  979, 1053,  836,
  602, 1815, 1321,  140,  896,  338,  389, 1271,  896, 1320,
  272,  597,  843,  598,  723,  613, 1482, 1482, 1486,  724,
  227,  177, 1372, 1372,  227,  294,  703,  261,  233, 1886,
 1372, 1372,  843, 1372, 1372,  837,  843,  597,  274,  702,
  703, 1372, 1372, 1372, 1372,  238, 1372, 1372, 1372, 1372,
 1372, 1372, 1032, 1513,  390, 1515,  178,  810, 1272,  602,
  238,  819,  602,  391, 1302, 1303,  238,  387,  178, 1745,
  922,  328,  598, 1314,  727,  725,  471,  726,  602,  728,
 1887,  972, 1036,  530,  352,  580, 1429,  388,  471,  143,
  481, 1797,  157,  226,  539,  178,  219,  220, 1037,  811,
  251,  178,  353,  178,  602,  233,  386,  703, 1399, 1315,
  810, 1262,  275,  810, 1352,  822, 1748, 1558, 1424, 1167,
  810,  471,  810,  471, 1263,  227,  810,  696, 1434, 1475,
  328,  810,  389, 1168,  810,  838,  354,  531,  820,  498,
  811,  498,  178,  811,  580,  233,  279, 1264,  567,  810,
  606, 1699, 1802,  302,  532,  498,  772,  895,  811, 1262,
  280,  211, 1749,  810,  767, 1331,  593, 1801,  696,  283,
 1802,  810, 1263, 1750,  810,  810,  594, 1169,  821,  233,
  221,  623,  502,  772,  810, 1862,  696,  668, 1845,  123,
  668,  287,  102,  303,  657, 1264,  406,  668, 1266,  668,
  810,  606, 1599,  668,  657, 1170,  657,  567,  668, 1265,
  541,  668,  242,  388, 1335,  657,  727,  725,  692,  726,
 1564,  728,  810,  123, 1672, 1569,  668,  810, 1540, 1745,
  103,  502,  288, 1745,  449, 1836,  289, 1600, 1359,  810,
  668,  810,  104,  293, 1745, 1673, 1266,  407,  668, 1601,
 1652, 1674,  668,  242,  207,  406,  242,  123,  515,  657,
  603,  668,  737,  149, 1745,  736,  105,  520,  657, 1171,
 1360,  242, 1751,  727,  725,  233,  726,  668,  728,  723,
  403,  767, 1620, 1781,  724, 1675,  339,  700,  810,  465,
  168,  695, 1745,  603,  613,  207,  233, 1745,  207,  157,
   44,  552,  233,   52,  668,  557,  407,  178,  810,  723,
  169,  657,  810,  207,  724, 1399,  668,  309,  668,  603,
  539,  103,  603,  657, 1262,  657,  340,  613,  700,  658,
  327,  106,  695,  465,  739,  205,  233, 1263,  603,  403,
  335,  107, 1510, 1630, 1138, 1138,  700,  336,  465,  168,
  695,  598, 1511,  613,  302, 1475,  613,  105, 1267,   44,
 1264,  811,   52,  338,  603,  668,  108,  342, 1796,  169,
  658,  344,  613,  658, 1265,  727,  725,   42,  726,  302,
  728,   25, 1762,  345,  598,  668,  763,  222,  658,  668,
  498, 1747,  657,  739,  929,  325,  727,  725,  613,  726,
  708,  728,  211,  211,  360, 1760,  714,  715,  716,   48,
  598, 1266,  716,  598,  811,  371,  346,  811,  347,  937,
 1482,   50,  106,  480,  811,  135,  811,  157,  222,  598,
  811,  222,  107,  374,   26,  811,   42,   49,  811,  829,
   25,  830,  696,   51, 1678,  763,  222,  716, 1477,  362,
 1479,  723,  363,  811,  696,  598,  724,  108, 1012,  716,
 1727,  374,  555,  696,  556,  767,   46,  811,   48,  136,
 1012,  727,  725, 1012,  726,  811,  728,  761,  811,  811,
   50,  763,  480,  373,  135, 1776,  727,  725,  811,  726,
 1738,  728,  374,   26,  364,  375,   49,  376,  157, 1763,
   55,  208,   51,  124,  811,  379,  767,  696,  723,  767,
 1864,   56,   47,  724, 1399,  379,  696,  213,   57,  377,
  302,  211,  382, 1267,  767,   46,  811,  302,  136,  727,
  725,  811,  726, 1138,  728,  762,  727,  725, 1777,  726,
  207,  728,   58,  811,  378,  811,  248,  160,  392,  741,
  325, 1747,  740,  211,  394, 1747, 1756, 1747,  213,  696,
  939,  213,  124,  541, 1784, 1017, 1747,  941, 1786,  969,
 1003,   47,  808,  696,  157,  807,  213, 1017,  161,  208,
 1017, 1819, 1000,   83,   84,  990, 1747,  248,  396,  541,
  248, 1774,  811,  162,  963, 1006, 1804,  729, 1005,  207,
 1121,  397,  700, 1120,   59,  248,  695,  962,  727,  725,
  723,  726,  811,  728, 1747,  724,  811, 1125,  695, 1747,
  886, 1223,  541,  700,  729, 1226,  208,  695, 1225, 1276,
 1323,  723, 1120, 1322,  541,   60,  724,  541,  902,   61,
  696,  400,  785, 1328,  658,  541, 1327,  401, 1607,  205,
 1806, 1499, 1489, 1501,  658,  729,  658, 1807,  931,  795,
  148,  149,  936,  233,  222,  658,  486,  700,  692,  952,
  493,  695,  501,  539,  959,  960,  700,  219,  220, 1573,
  695,  489,  729,  785,  502,  449,  785,  970,  971,  972,
  973,  974,  975,  449, 1856,  506, 1857,  727,  725,  539,
  726,  785,  728,  227, 1830,  505,  723, 1614, 1475,  658,
  729,  724, 1651, 1016, 1869,  729,  227, 1868,  658,  700,
  508,  723, 1870,  695,  510,  740,  724,  208,  511,  208,
  512,  520,  539,  700, 1475, 1833,  525,  695,  527, 1835,
 1888,  767,  767, 1802,  539,  369, 1209,  539,  369,  767,
 1841, 1043, 1379,  724,  537,  539,  724,  552, 1050,  233,
  528,  658, 1088, 1089,  723,  131,  131, 1475, 1016,  724,
 1860,  723, 1010,  658,  547,  658,  724,  549,  551, 1475,
  233, 1138, 1475, 1015,  727,  725,  233,  726,   14,  728,
 1475,  563,  565,  213,  213, 1584,  585,  564, 1878,  567,
  700,  213,  588, 1880,  695, 1201,  727,  725,  244,  726,
  598,  728,  986, 1010, 1010, 1010, 1010, 1010, 1010, 1010,
  233,  602,  248,  248,  727,  725, 1219,  726,  651,  728,
  248, 1010, 1010, 1010, 1010,  727,  725,  767,  726,  655,
  728,  767,  658,  723,  420,  421,  422,  657,  724,  244,
  659,  692,  244,  694,  697, 1404,  727,  725,  767,  726,
  695,  728,  696, 1010,  767, 1010,  698,  244,  699, 1147,
 1148, 1152,  430,  431, 1161, 1163, 1165,  700, 1173, 1176,
 1179, 1181, 1184, 1187, 1405,  727,  725,  701,  726,  213,
  728,  702, 1194, 1195,  703, 1197,  706, 1199,  767,  704,
 1202, 1203, 1204, 1205, 1206, 1207,  705,  930,  711,  934,
  213,  712,  713,  938,  940,  942,  213, 1211,  248, 1216,
  248, 1408,  727,  725,  722,  726,  729,  728,  735,  730,
  738,  731,  723,  245, 1409,  727,  725,  724,  726,  248,
  728,  739,  743,  756, 1162,  248, 1166,  748, 1174, 1177,
  213, 1182, 1185, 1188,  249,  759,  766,  785,  767,  768,
  785,  494, 1411,  727,  725,  784,  726,  785,  728,  785,
  449,  779,  783,  785,  245,  788,  790,  245,  785,  248,
  791,  785,  794,  800,  796, 1412,  727,  725, 1018,  726,
  558,  728,  245, 1413,  727,  725,  785,  726,  799,  728,
  802,  809,  494,  803,  833,  494,  834,  840,  294,  837,
  785, 1414,  727,  725,  845,  726,  846,  728,  785,  723,
  494,  785,  785,  835,  724,  852,  853,  854,  855,  857,
  858,  785,  863,  869, 1010, 1018, 1010, 1010,  870,  248,
  873,  723,  874, 1010,  883,  659,  724,  785,  885, 1010,
  837,  886,  891,  837, 1010, 1368,  893, 1010,  659,  723,
  892,  897,  170,  954,  724,  965,  659,  864,  837,  785,
  723, 1010,  984,  296,  785,  724, 1010, 1010, 1000, 1001,
 1010,  659, 1003, 1007,  244,  244,  785, 1010,  785, 1010,
 1010,  723,  244, 1010, 1011,  659,  724, 1010, 1012, 1010,
 1029, 1010, 1034, 1010, 1044,  740, 1047, 1058, 1010, 1010,
 1068, 1010, 1067, 1070,  812, 1074, 1098,  449,  727,  725,
  723,  726, 1077,  728, 1075,  724, 1010, 1010, 1091, 1010,
 1382, 1383,  659, 1385, 1387,  785, 1083, 1111, 1084, 1085,
 1010, 1392, 1010, 1109, 1010, 1010, 1010, 1104, 1010, 1118,
 1132, 1010, 1010, 1154, 1192,  785, 1191,  723, 1198,  785,
 1200, 1010,  724, 1410,  727,  725, 1208,  726,  723,  728,
  723, 1417, 1418, 1216, 1010,  724, 1216, 1010, 1211, 1227,
  244, 1229, 1425,  449, 1427, 1241, 1415,  727,  725, 1244,
  726, 1247,  728, 1010, 1239, 1255, 1259, 1261,  723, 1010,
 1010,  244, 1269,  724, 1010, 1280, 1283,  244, 1304,  245,
  245, 1306, 1324, 1316, 1326, 1010, 1010,  245, 1010, 1333,
 1337,  723,  449, 1338, 1010, 1339,  724, 1340, 1126,  723,
 1356, 1348, 1361, 1362,  724, 1363, 1369,  494,  494, 1370,
 1374,  244, 1010, 1010,  659,  494, 1377,  723, 1378, 1380,
  990, 1381,  724, 1010, 1010, 1388, 1010, 1389,  494, 1390,
 1391, 1393, 1522, 1010, 1010, 1010, 1010, 1010, 1394, 1010,
 1395,  837, 1010, 1010, 1010, 1010, 1010, 1396, 1397, 1398,
 1010, 1010,  921, 1423, 1420, 1010, 1010, 1010, 1010, 1010,
 1476,  990,  990,  990,  990,  990, 1003,  990, 1416,  727,
  725,  244,  726, 1490,  728,  245, 1492, 1493,  764,  990,
  990,  990,  990, 1497, 1502, 1503, 1605,  727,  725, 1504,
  726, 1505,  728,  580,  837, 1531,  245,  837, 1527, 1514,
 1532, 1570,  245,  494,  837, 1533,  837, 1576, 1583, 1534,
  837,  990, 1580,  990, 1535,  837, 1536, 1539,  837, 1585,
 1095, 1544, 1554,  723,  494, 1560, 1562,   14,  724, 1563,
  494, 1568, 1575,  837, 1577, 1597,  245, 1606,  727,  725,
 1604,  726, 1598,  728, 1610, 1618,  310,  837, 1613,  727,
  725, 1611,  726, 1216,  728,  837,  764, 1216,  837,  837,
 1617, 1791,  727,  725,  494,  726, 1624,  728,  837,  723,
  109, 1623,  140, 1625,  724,  727,  725, 1220,  726,  311,
  728, 1626, 1627, 1634,  837, 1648, 1635, 1636,  110,  111,
 1637, 1643,  723, 1638, 1644, 1647,  245,  724, 1649,  112,
  113, 1653,  114, 1679, 1669,  312,  837, 1681,  313,  115,
  116,  837,  117,  118, 1684,  119, 1682,  109,  120,  121,
  122,  123,  124,  837,  314,  837,  125,  126, 1685, 1683,
 1686, 1688,  127,  128,  129,  110,  111, 1656,  727,  725,
 1689,  726, 1698,  728, 1690, 1705,  112,  113, 1706,  114,
  315, 1709, 1710, 1711, 1700, 1704,  115,  116, 1712,  117,
  118, 1715,  119, 1716, 1746,  120,  121,  122,  123,  124,
 1717, 1718,  837,  125,  126,  275, 1735, 1719, 1720,  127,
  128,  129,  990, 1721,  990,  990, 1722, 1723, 1724, 1726,
 1742,  990,  837, 1729, 1757, 1639,  837,  990, 1770, 1767,
 1773, 1772,  990, 1785,  723,  990, 1789, 1793, 1800,  724,
 1708, 1803, 1799, 1811, 1813, 1810, 1713, 1714, 1816,  990,
 1014,  990,  723, 1820,  990,  990, 1827,  724,  990,  727,
  725, 1615,  726, 1828,  728,  990, 1829,  990,  990, 1831,
 1728,  990, 1855, 1850, 1216,  990, 1834,  990, 1731,  990,
 1852,  990, 1858, 1737, 1861, 1739,  990,  990, 1802,  990,
 1867, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1847, 1848,
 1877, 1879, 1882,  723,  990,  990, 1885,  990,  724, 1014,
 1014, 1014, 1014, 1881,  723,    1,  732,   30,  990,  724,
  990,   32,  990,  990,  990,  984,  990,  723,  193,  990,
  990,  371,  724,  767,  262,  156,  648,  462,   82,  990,
  723, 1014,  894, 1014,  971,  724,  273,  203,  727,  725,
  710,  726,  990,  728, 1746,  990,  504,  739, 1746,  272,
 1837,  399, 1783,  183,  692, 1794,  694,  970,   65, 1746,
  712,  990,  184,  951,  372,  950,  213,  990,  990,   66,
  323,  721,  990,  289,  331,  358,   67,  343,   27, 1746,
 1196,   30,   40,  990,  990,  329,  990,  504, 1481,   68,
  504,  449,  990,  723,  509,  145,  500,  499,  724,  884,
 1002, 1357, 1355, 1131,  262,  504, 1136, 1746, 1094, 1353,
  990,  990, 1746, 1345, 1691,  569, 1692,  527,  610,  847,
 1656,  990,  990, 1334,  990, 1282,  545, 1045, 1346, 1771,
 1733,  990,  990,  990,  990,  990,  812,  990, 1846,  582,
  990,  990,  990,  990,  990,  583,   69, 1671,  990,  990,
 1677, 1853,  828,  990,  990,  990,  990,  990,  527, 1072,
 1851,  527,   70, 1840, 1876,  527,  943,  944,  945,  946,
  947,  948,  949,  950, 1065, 1795,  527, 1231,   71, 1629,
 1839, 1754,  981, 1509,  723, 1662,  566,  590,  575,  724,
  832, 1551, 1110,   72,  851, 1859,  147,   73,  871,  538,
   74,  774, 1014,  778, 1014, 1014,  527,  307,   75,  527,
 1035, 1014, 1031,  446, 1258, 1101,  257, 1014, 1500, 1817,
  926, 1596, 1014,  787,  527, 1014,  527,   76, 1725, 1008,
 1251, 1518, 1049, 1384, 1422,    0,    0,    0,    0, 1014,
 1019,    0,    0,    0, 1014, 1014,    0,    0, 1014,    0,
    0,    0,    0,    0,    0, 1014,    0, 1014, 1014,    0,
    0, 1014,    0,    0,    0, 1014,    0, 1014,    0, 1014,
    0, 1014,    0,  723,    0,    0, 1014, 1014,  724, 1014,
    0, 1019, 1019, 1019, 1019, 1019, 1019, 1019,    0,    0,
    0,    0,    0,    0, 1014, 1014,    0, 1014,    0, 1019,
 1019, 1019, 1019,    0,    0,    0,    0,  109, 1014,    0,
 1014,    0, 1014, 1014, 1014,    0, 1014,    0,    0, 1014,
 1014,    0,  504,  504,    0,  110,  111,  504,    0, 1014,
  504, 1019,    0, 1019,    0,    0,  112,  113,    0,  114,
    0,    0, 1014,  504,    0, 1014,  115,  116,    0,  117,
  118,    0,  119,    0,    0,  120,  121,  122,  123,  124,
    0, 1014,    0,  125,  126,    0,    0, 1014, 1014,  127,
  128,  129, 1014,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1014, 1014,    0, 1014,    0,    0,    0,
    0,    0, 1014,  527,  527,    0,    0,    0,  527,    0,
    0,  527,    0,    0,    0,    0,    0,    0,    0,    0,
 1014, 1014,    0,    0,  527,    0,    0,  527,  504,  242,
    0, 1014, 1014,  527, 1014,    0,    0,    0,    0,    0,
    0, 1014, 1014, 1014, 1014, 1014,    0, 1014,  527,  504,
 1014, 1014, 1014, 1014, 1014,  504,    0,    0, 1014, 1014,
    0,    0,    0, 1014, 1014, 1014, 1014, 1014,  527,    0,
  242,  527,  527,  242,    0,    0,    0,    0,  527,    0,
    0,  527,    0,    0,    0,    0,  527,    0,  242,  504,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  527,
    0,    0,    0,    0,    0,    0,    0,  978,    0,    0,
    0,    0, 1019,    0, 1019, 1019,    0,    0,    0,    0,
  527, 1019,    0,  527,    0,    0,  527, 1019,    0,    0,
    0,    0, 1019,    0,    0, 1019,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1019,
 1015,    0,    0,    0, 1019, 1019,    0,    0, 1019,    0,
  527,    0,    0,    0,    0, 1019,    0, 1019, 1019,    0,
    0, 1019,    0,    0,    0, 1019,    0, 1019,  527, 1019,
    0, 1019,    0,    0,    0,  527, 1019, 1019,    0, 1019,
    0, 1015, 1015, 1015, 1015, 1015, 1015, 1015,    0,    0,
    0,    0,    0,  527, 1019, 1019,    0, 1019,    0, 1015,
 1015, 1015, 1015,    0,    0,    0,    0,  109, 1019,    0,
 1019,    0, 1019, 1019, 1019,    0, 1019,    0,    0, 1019,
 1019,    0,    0,  527,    0,  110,  111,  527,    0, 1019,
    0, 1015,    0, 1015,    0,    0,  112,  113,    0,  114,
    0,    0, 1019,    0,    0, 1019,  115,  116,    0,  117,
  118,    0,  119,    0,    0,  120,  121,  122,  123,  124,
    0, 1019,    0,  125,  126,    0,    0, 1019, 1019,  127,
  128,  129, 1019,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1019, 1019,    0, 1019,    0,    0,    0,
    0,    0, 1019,  527,  527,  242,  242,    0,    0,    0,
    0,  527,    0,  242,    0,    0,    0,    0,    0,    0,
 1019, 1019,    0,    0,  527,    0,    0,    0,    0,    0,
    0, 1019, 1019,    0, 1019,    0,    0,    0,    0,    0,
    0, 1019, 1019, 1019, 1019, 1019,    0, 1019,    0,    0,
 1019, 1019, 1019, 1019, 1019,    0,    0,    0, 1019, 1019,
 1571, 1572,    0, 1019, 1019, 1019, 1019, 1019, 1578, 1579,
    0, 1581, 1582,    0,    0,    0,    0,    0,    0, 1586,
 1587, 1588, 1589,    0, 1590, 1591, 1592, 1593, 1594, 1595,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  527,
    0,  242,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1015,    0, 1015, 1015,    0,    0,    0,    0,
  527, 1015,  242,    0,    0,    0,  527, 1015,  242,    0,
    0,    0, 1015,    0,    0, 1015,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1015,
 1028,    0,    0,    0, 1015, 1015,    0,    0, 1015,    0,
  527,    0,  242,    0,    0, 1015,    0, 1015, 1015,    0,
    0, 1015,    0,    0,    0, 1015,    0, 1015,    0, 1015,
    0, 1015, 1285,    0,    0,    0, 1015, 1015,    0, 1015,
    0, 1028, 1028, 1028, 1028, 1028, 1028, 1028,    0,    0,
    0,    0,    0,    0, 1015, 1015,    0, 1015,    0, 1028,
 1028, 1028, 1028,    0,    0,    0,    0,  109, 1015,    0,
 1015,    0, 1015, 1015, 1015,    0, 1015,    0,    0, 1015,
 1015,    0,    0,    0,    0,  110,  111,    0,    0, 1015,
    0, 1028,    0, 1028,    0,    0,  112,  113,    0,  114,
    0,    0, 1015,    0,    0, 1015,  115,  116,    0,  117,
  118,    0,  119,    0,    0,  120,  121,  122,  123,  124,
    0, 1015,    0,  125,  126,    0,    0, 1015, 1015,  127,
  128,  129, 1015,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1015, 1015,    0, 1015,    0,   87,    0,
    0,    0, 1015,    0,    0,   88,    0,    0,    0,   89,
   90,    0,    0,    0,    0,    0,    0,    0,   91,    0,
 1015, 1015,    0,    0,    0,    0,    0,    0,    0,   92,
    0, 1015, 1015,    0, 1015,    0,    0,    0,    0,    0,
    0, 1015, 1015, 1015, 1015, 1015,    0, 1015,    0,    0,
 1015, 1015, 1015, 1015, 1015,    0,    0,    0, 1015, 1015,
    0,    0,    0, 1015, 1015, 1015, 1015, 1015,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   93,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   94,    0,  652,
    0,    0, 1028,    0, 1028, 1028,    0,    0,    0,    0,
    0, 1028,   95,    0,    0,    0,   96, 1028,    0,  408,
    0,    0, 1028,    0,    0, 1028,    0,   97,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1028,
  862,    0,    0,    0, 1028, 1028,   98,  410, 1028,    0,
    0,    0,    0,    0,    0, 1028,    0, 1028, 1028,    0,
    0, 1028,    0,    0,    0, 1028,    0, 1028,    0, 1028,
    0, 1028,    0,    0,    0,  412, 1028, 1028,    0, 1028,
    0,  862,  862,  862,  862,  862,    0,  862,    0,    0,
    0,    0,    0,    0, 1028, 1028,    0, 1028,    0,  862,
  862,  862,  862,    0,    0,    0,    0,    0, 1028,    0,
 1028,    0, 1028, 1028, 1028,    0, 1028,    0,    0, 1028,
 1028,    0,    0,    0,    0,    0,    0,    0,    0, 1028,
    0,    0,    0,  862,    0,    0,    0,    0,    0,    0,
    0,    0, 1028,    0,    0, 1028,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   80, 1028,  419,    0,    0,    0,    0, 1028, 1028,    0,
    0,    0, 1028,    0,    0,    0,  420,  421,  422,    0,
  219,  220,    0, 1028, 1028,    0, 1028,    0,    0,    0,
    0,    0, 1028,    0,    0,    0,    0,    0,    0,  425,
  426,  427,  428,  429,  430,  431,  432,  433,    0,    0,
 1028, 1028,    0,    0,  435,  436,    0,    0,    0,    0,
    0, 1028, 1028,  440, 1028,    0,    0,    0,    0,    0,
    0, 1028, 1028, 1028, 1028, 1028,    0, 1028,    0,    0,
 1028, 1028, 1028, 1028, 1028,  442,    0,    0, 1028, 1028,
    0,    0,    0, 1028, 1028, 1028, 1028, 1028,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  652,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  862,    0,  862,  862,    0,    0,    0,    0,
    0,  862,    0,  446,    0,    0,    0,  862,    0,    0,
    0,    0,  862,    0,    0,  862,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  862,
  874,    0,    0,    0,  862,  862,    0,    0,  862,    0,
    0,    0,    0,    0,    0,  862,    0,  862,  862,    0,
    0,  862,    0,    0,    0,  862,    0,  862,    0,  862,
    0,  862,    0,    0,    0,    0,  862,  862,    0,  862,
    0,  874,  874,  874,  874,  874,    0,  874,    0,    0,
    0,    0,    0,    0,  862,  862,    0,  862,    0,  874,
  874,  874,  874,    0,    0,    0,    0,  655,  862,    0,
  862,    0,  862,  862,  862,    0,  862,    0,    0,  862,
  862,    0,    0,    0,    0,  655,  655,    0,    0,  862,
    0,    0,    0,  874,    0,    0,  655,  655,    0,  655,
    0,    0,  862,    0,    0,  862,  655,  655,    0,  655,
  655,    0,  655,    0,    0,  655,  655,  655,  655,  655,
    0,  862,    0,  655,  655,    0,    0,  862,  862,  655,
  655,  655,  862,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  862,  862,    0,  862,    0,    0,    0,
    0,    0,  862,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  862,  862,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  862,  862,    0,  862,    0,    0,    0,    0,    0,
    0,  862,  862,  862,  862,  862,    0,  862,    0,    0,
  862,  862,  862,  862,  862,    0,    0,    0,  862,  862,
    0,    0,    0,  862,  862,  862,  862,  862,    0,    0,
    0,    0,    0,    0,    0,   79,    0,  109,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  110,  111,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  112,  113,    0,  114,
    0,    0,  874,    0,  874,  874,  115,  116,    0,  117,
  118,  874,  119,    0, 1619,  120,  121,  122,  123,  124,
    0,    0,  874,  125,  126,  874,    0,    0,    0,  127,
  128,  129,    0,    0,    0,    0,    0,    0,    0,  874,
  877,    0,    0,    0,  874,  874,    0,    0,  874,    0,
    0,    0,    0,    0,    0,  874,    0,  874,  874,    0,
    0,  874,    0,    0,    0,  874,    0,  874,    0,  874,
    0,  874,    0,    0,    0,    0,  874,  874,    0,  874,
    0,  877,  877,  877,  877,  877,    0,  877,    0,    0,
    0,    0,    0,    0,  874,  874,    0,  874,    0,  877,
  877,  877,  877,    0,    0,    0,    0,  109,  874,    0,
  874,    0,  874,  874,  874,    0,  874,    0,    0,  874,
  874,    0,    0,    0,    0,  110,  111,    0,    0,  874,
    0,    0,    0,  877,    0,    0,  112,  113,    0,  114,
    0,    0,  874,    0,    0,  874,  115,  116,    0,  117,
  118,    0,  119,    0,    0,  120,  121,  122,  123,  124,
    0,  874,    0,  125,  126,    0,    0,  874,  874,  127,
  128,  129,  874,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  874,  874,    0,  874,    0,    0,    0,
    0,    0,  874,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  874,  874,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  874,  874,  537,  874,    0,    0,    0,    0,    0,
    0,  874,  874,  874,  874,  874,    0,  874,    0,    0,
  874,  874,  874,  874,  874,    0,    0,    0,  874,  874,
    0,    0,    0,  874,  874,  874,  874,  874,    0,    0,
    0,    0,    0,  537,  537,    0,    0,  537,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  537,    0,  537,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  877,    0,  877,  877,    0,    0,    0,    0,
    0,  877,    0,    0,  537,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  877,    0,    0,    0,    0,
    0,  525,    0,    0,    0,    0,    0,    0,    0,  877,
  878,    0,    0,    0,  877,  877,    0,    0,  877,    0,
    0,    0,    0,    0,    0,  877,    0,  877,  877,    0,
    0,  877,    0,    0,    0,  877,    0,  877,    0,  877,
    0,  877,  525,    0,    0,  525,  877,  877,    0,  877,
    0,  878,  878,  878,  878,  878,    0,  878,    0,    0,
  525,    0,  525,    0,  877,  877,    0,  877,    0,  878,
  878,  878,  878,    0,    0,    0,    0,  109,  877,    0,
  877,    0,  877,  877,  877,    0,  877,    0,    0,  877,
  877,    0,    0,    0,    0,  110,  111,    0,    0,  877,
    0,    0,    0,  878,    0,    0,  112,  113,    0,  114,
    0,    0,  877,    0,    0,  877,  115,  116,    0,  117,
  118,    0,  119,    0, 1628,  120,  121,  122,  123,  124,
    0,  877,    0,  125,  126,    0,    0,  877,  877,  127,
  128,  129,  877,    0,    0,    0,    0,    0,    0,    0,
    0,  537,    0,  877,  877,    0,  877,    0,    0,    0,
    0,    0,  877,    0,    0,    0,  537,    0,  537,  537,
  537,    0,    0,    0,  537,    0,    0,  537,    0,    0,
  877,  877,    0,    0,    0,    0,    0,    0,    0,    0,
  537,  877,  877,    0,  877,    0,  537,    0,    0,  537,
    0,  877,  877,  877,  877,  877,    0,  877,    0,    0,
  877,  877,  877,  877,  877,    0,    0,    0,  877,  877,
    0,    0,    0,  877,  877,  877,  877,  877,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  537,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  525,
    0,    0,    0,    0,    0,  537,    0,    0,    0,    0,
    0,    0,  878,    0,  878,  878,  525,  525,  525,    0,
    0,  878,  525,    0,    0,  525,  537,    0,    0,    0,
    0,    0,  537,    0,    0,  878,  537,    0,  525,  872,
    0,    0,    0,  537,  525,    0,    0,    0,    0,  878,
    0,    0,    0,    0,  878,  878,    0,    0,  878,    0,
    0,  537,    0,    0,    0,  878,  537,  878,  878,    0,
    0,  878,    0,    0,    0,  878,    0,  878,    0,  878,
  872,  878,  872,  872,  872,    0,  878,  878,    0,  878,
    0,  537,    0,    0,    0,  537,    0,    0,  872,  872,
  872,  872,    0,    0,  878,  878,    0,  878,    0,    0,
    0,    0,    0,  525,    0,    0,    0,    0,  878,    0,
  878,    0,  878,  878,  878,    0,  878,    0,    0,  878,
  878,    0,  872,    0,  525,    0,    0,    0,    0,  878,
  525,    0,    0,    0,    0,    0,    0,    0,    0,  525,
    0,  525,  878,    0,    0,  878,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  525,
    0,  878,    0,    0,  525,    0,    0,  878,  878,    0,
    0,    0,  878,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  878,  878,    0,  878,    0,    0,    0,
    0,    0,  878,  525,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  878,  878,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  878,  878,    0,  878,    0,    0,    0,  580,    0,
    0,  878,  878,  878,  878,  878,    0,  878,    0,    0,
  878,  878,  878,  878,  878,    0,    0,  109,  878,  878,
    0,    0,    0,  878,  878,  878,  878,  878,    0,    0,
    0,    0,    0,    0,    0,  110,  111,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  112,  113,    0,  114,
    0,    0,    0,    0,    0,    0,  115,  116,    0,  117,
  118,  872,  119,  872,  872,  120,  121,  122,  123,  124,
  872,    0,    0,  125,  126,    0,    0,    0,    0,  127,
  128,  129,    0,    0,  872,    0,    0,    0,  871,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  872,    0,
    0,    0,    0,  872,  872,    0,    0,  872,    0,    0,
    0,    0,    0,    0,  872,    0,  872,  872,    0,    0,
  872,    0,    0,    0,  872,    0,  872,    0,  872,  871,
  872,  871,  871,  871,    0,  872,  872,    0,  872,    0,
    0,    0,    0,    0,    0,    0,    0,  871,  871,  871,
  871,    0,    0,  872,  872,    0,  872,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  109,  872,    0,  872,
    0,  872,  872,  872,    0,  872,    0,    0,  872,  872,
    0,  871,    0,    0,  110,  111,    0,    0,  872,    0,
    0,    0,    0,    0,    0,  112,  113,    0,  114,    0,
    0,  872,    0,    0,  872,  115,  116,    0,  117,  118,
    0,  119,    0,    0,  120,  121,  122,  123,  124,    0,
  872,    0,  125,  126,    0,    0,  872,  872,  127,  128,
  129,  872,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  872,  872,    0,  872,    0,    0,    0,    0,
    0,  872,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  872,
  872,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  872,  872,    0,  872,    0,    0,    0,  586,    0,    0,
  872,  872,  872,  872,  872,    0,  872,    0,    0,  872,
  872,  872,  872,  872,    0,    0,    0,  872,  872,    0,
    0,    0,  872,  872,  872,  872,  872,    0,    0,    0,
  962,  727,  725,    0,  726,    0,  728,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  919,
  918,  920,    0,    0,    0,    0,    0,    0,    0,    0,
  871,    0,  871,  871,    0,    0,    0,    0,    0,  871,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  871,    0,    0,    0,  873,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  871,    0,    0,
    0,    0,  871,  871,    0,    0,  871,    0,    0,    0,
    0,    0,    0,  871,    0,  871,  871,    0,    0,  871,
    0,    0,    0,  871,    0,  871,    0,  871,  873,  871,
  873,  873,  873,    0,  871,  871,    0,  871,    0,    0,
    0,    0,    0,    0,    0,    0,  873,  873,  873,  873,
    0,    0,  871,  871,    0,  871,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  109,  871,    0,  871,    0,
  871,  871,  871,    0,  871,    0,    0,  871,  871,    0,
  873,    0,    0,  110,  111,    0,    0,  871,    0,    0,
    0,    0,    0,    0,  112,  113,    0,  114,    0,    0,
  871,    0,    0,  871,  115,  116,    0,  117,  118,    0,
  119,    0,    0,  120,  121,  122,  123,  124,    0,  871,
    0,  125,  126,    0,    0,  871,  871,  127,  128,  129,
  871,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  871,  871,    0,  871,    0,    0,    0,    0,    0,
  871,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  906,    0,    0,    0,    0,    0,  723,    0,  871,  871,
    0,  724,    0,    0,  907,    0,    0,    0,    0,  871,
  871,    0,  871,    0,    0,    0,    0,    0,    0,  871,
  871,  871,  871,  871,    0,  871,    0,    0,  871,  871,
  871,  871,  871,    0,    0,    0,  871,  871,    0,    0,
  908,  871,  871,  871,  871,  871,    0,    0,  909,    0,
  727,  725,    0,  726,    0,  728,  910,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  919,  918,
  920,    0,    0,    0,  911,    0,  912,    0,    0,  873,
    0,  873,  873,    0,    0,    0,    0,    0,  873,  913,
    0,  914,  915,  916,    0,    0,    0,    0,    0,    0,
    0,    0,  873,    0,    0,    0,  876,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  873,    0,    0,    0,
    0,  873,  873,    0,    0,  873,    0,    0,    0,    0,
    0,    0,  873,    0,  873,  873,    0,    0,  873,    0,
  917,    0,  873,    0,  873,    0,  873,  876,  873,  876,
  876,  876,    0,  873,  873,    0,  873,  887,    0,    0,
  612,    0,    0,    0,    0,  876,  876,  876,  876,    0,
    0,  873,  873,  888,  873,    0,    0,  613,    0,    0,
  614,    0,    0,    0,    0,  873,    0,  873,    0,  873,
  873,  873,    0,  873,    0,    0,  873,  873,    0,  876,
    0,    0,    0,    0,    0,    0,  873,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  873,
  615,    0,  873,    0,    0,    0,  616,  617,  618,    0,
  619,  620,  621,    0,  622,    0,    0,    0,  873,    0,
    0,    0,    0,    0,  873,  873,    0,    0,    0,  873,
    0,    0,  623,    0,  624,    0,    0,    0,    0,    0,
  873,  873,    0,  873,    0,    0,    0,    0,    0,  873,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  906,
    0,    0,    0,  625,    0,  723,    0,  873,  873,    0,
  724,    0,    0,  907,    0,    0,    0,    0,  873,  873,
  626,  873,    0,    0,    0,    0,    0,    0,  873,  873,
  873,  873,  873,    0,  873,    0,    0,  873,  873,  873,
  873,  873,  627,    0,    0,  873,  873,    0,    0,  908,
  873,  873,  873,  873,  873,    0,    0,  909,    0,    0,
    0,    0,    0,    0,    0,  910,  989,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  853,  628,  629,    0,
    0,    0,    0,  911,    0,  912,    0,    0,  876,    0,
  876,  876,    0,    0,  630,    0,    0,  876,  913,  978,
  914,  915,  916,    0,    0,    0,    0,    0,    0,    0,
    0,  876,    0,    0,    0,    0,    0,  853, 1022, 1022,
  853, 1022, 1022, 1022,    0,  876,    0,    0,    0,    0,
  876,  876,    0,    0,  876,  853, 1022, 1022, 1022,    0,
    1,  876,    0,  876,  876,    0,    0,  876,    0,  917,
    0,  876,    0,  876,    0,  876,    0,  876,    2,    0,
    0,    0,  876,  876,    0,  876,    3, 1022,    0,    0,
    0,    0,    0,    4,    0,    5,  992,    6,    0,    0,
  876,  876,    7,  876,    0,    0,    0,    0,    0,    8,
    0,    0,    0,  109,  876,    0,  876,    0,  876,  876,
  876,    0,  876,    9,    0,  876,  876,    0,    0,    0,
    0,  110,  111,   10,    0,  876,    0,    0,    0,    0,
    0,    0,  112,  113,    0,  114,    0,    0,  876,    0,
    0,  876,  115,  116,    0,  117,  118,    0,  119,    0,
    0,  120,  121,  122,  123,  124,    0,  876,    0,  125,
  126,    0,    0,  876,  876,  127,  128,  129,  876,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  876,
  876,    0,  876,    0,    0,    0,   11,    0,  876,    0,
    0,    0,    0,   12,    0,    0,   13,    0,   14,   15,
    0,    0,    0,    0,    0,    0,  876,  876,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  876,  876,    0,
  876,    0,    0,   16,    0,    0,    0,  876,  876,  876,
  876,  876,    0,  876,    0,  854,  876,  876,  876,  876,
  876,    0,    0,    0,  876,  876,    0,    0,  853,  876,
  876,  876,  876,  876,    0,    0,    0, 1022,    0,    0,
  977,    0,    0, 1022,    0,    0,    0,    0, 1022,    0,
    0, 1022,   17,    0,    0,    0,  854, 1023, 1023,  854,
 1023, 1023, 1023,    0,    0,    0,    0,   18,   19,    0,
    0,    0,    0,    0,  854, 1023, 1023, 1023,    0,    0,
    0,  853,    0,    0,  853,    0,    0, 1022,    0,  990,
    0,  853,  990,  853,    0, 1022,    0,  853,    0,    0,
    0,    0,  853, 1022,    0,  853, 1023,  992,    0,  990,
    0,    0,  990,    0,    0,    0,    0,    0,    0,    0,
  853, 1022,    0, 1022,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  684,  853,    0, 1022,    0, 1022, 1022,
 1022,    0,  853,    0,    0,  853,  853,    0,    0,    0,
    0,    0,  990,    0,    0,  853,    0,    0,  990,  990,
  990,  855,  990,  990,  990,    0,  990,    0,    0,    0,
    0,  853,    0,    0,  684,    0,    0,  684,    0,    0,
    0,    0,    0,    0,  990,    0,  990, 1022,    0,    0,
    0,    0,  684,  853,    0,    0,    0,    0,  853,  109,
    0,    0,  855, 1024, 1024,  855, 1024, 1024, 1024,    0,
  853,    0,  853,    0,    0,  990,    0,  110,  111,    0,
  855, 1024, 1024, 1024,    0,    0,    0,    0,  112,  113,
    0,  114,  990,    0,    0,    0,    0,    0,  115,  116,
    0,  117,  118,    0,  119,    0,    0,  120,  121,  122,
  123,  124, 1024,    0,  990,  125,  126,    0,    0,  853,
    0,  127,  128,  129,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  854,    0,  853,
    0,    0,    0,  853,    0,    0, 1023,    0,    0,  990,
  990,    0, 1023,    0,    0,    0,    0, 1023,    0,    0,
 1023,    0,    0,    0,    0,    0,  990,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  854,    0,    0,  854,    0,    0, 1023,  655,    0,    0,
  854,    0,  854,    0, 1023,    0,  854,    0,    0,    0,
    0,  854, 1023,    0,  854,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  854,
 1023,    0, 1023,    0,    0,    0,    0,  655,  655,    0,
    0,  655,    0,  854,    0, 1023,    0, 1023, 1023, 1023,
    0,  854,    0,    0,  854,  854,  655,  684,    0,    0,
    0,    0,    0,    0,  854,    0,    0,    0,    0,    0,
    0,    0,  682,    0,    0,    0,    0,    0,    0,    0,
  854,    0,    0,  855,    0,    0,    0,    0,    0,    0,
    0,    0, 1024,    0,    0,    0, 1023,    0, 1024,    0,
    0,    0,  854, 1024,    0,    0, 1024,  854,  684,    0,
    0,  684,    0,  682,    0,    0,  682,    0,  684,  854,
  684,  854,    0,    0,  684,    0,    0,    0,    0,  684,
    0,  682,  684,    0,    0,    0,  855,    0,    0,  855,
    0,    0, 1024,    0,    0,    0,  855,  684,  855,    0,
 1024,    0,  855,    0,    0,    0,    0,  855, 1024,    0,
  855,  684,    0,    0,    0,    0,    0,    0,  854,  684,
    0,    0,    0,  684,    0,  855, 1024,    0, 1024,    0,
    0,    0,  684,    0,    0,    0,    0,    0,  854,  855,
    0, 1024,  854, 1024, 1024, 1024,    0,  855,  684,    0,
  855,  855,    0,    0,    0,    0,    0,    0,    0,    0,
  855,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  684,  855,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  684,    0,  684,
    0,    0, 1024,    0,    0,  684,    0,    0,  855,    0,
    0,    0,    0,  855,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  684,  684,  855,    0,  855,    0,    0,
    0,    0,    0,    0,  684,  684,  814,  684,    0,    0,
    0,    0,    0,    0,  684,  684,  684,  684,  684,    0,
  684,    0,    0,  684,  684,  684,  684,  684,    0,    0,
    0,  684,  684,    0,    0,    0,  684,  684,  684,  684,
  684,    0,  655,    0,  855,  655,    0,  814,    0,    0,
  814,    0,  655,    0,  655,    0,    0,    0,  655,    0,
    0,    0,    0,  655,  855,  814,  655,    0,  855,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  812,    0,
    0,  655,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  655,    0,    0,    0,    0,
    0,    0,    0,  655,    0,    0,    0,  655,    0,  813,
    0,    0,    0,    0,    0,    0,  655,  682,    0,  812,
  682,    0,  812,    0,    0,    0,    0,  682,    0,  682,
    0,    0,  655,  682,    0,    0,    0,  812,  682,    0,
    0,  682,    0,    0,  522,    0,    0,    0,    0,    0,
  813,    0,    0,  813,    0,    0,  682,    0,    0,  655,
    0,    0,    0,    0,    0,    0,    0,    0,  813,    0,
  682,  655,    0,  655,    0,    0,    0,    0,  682,  655,
    0,    0,  682,    0,    0,  522,    0,    0,  522,    0,
    0,  682,    0,    0,    0,    0,    0,  655,  655,    0,
    0,    0,    0,  522,    0,  522,    0,  682,  655,  655,
    0,  655,    0,    0,    0,    0,    0,    0,  655,  655,
  655,  655,  655,    0,  655,    0,    0,  655,  655,  655,
  655,  655,    0,    0,  682,  655,  655,    0,    0,    0,
  655,  655,  655,  655,  655,    0,  682,    0,  682,    0,
    0,    0,    0,    0,  655,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  814,    0,
    0,    0,  655,  655,    0,    0,    0,    0,    0,    0,
    0,    0,  845,  655,  655,    0,  655,    0,    0,    0,
    0,    0,    0,  655,  655,  682,  655,  655,    0,  655,
    0,    0,  655,  655,  655,  655,  655,    0,    0,    0,
  655,  655,    0,  807,    0,  682,  655,  655,  655,  682,
    0,  814,    0,  845,  814,    0,  845,    0,    0,    0,
    0,  814,    0,  814,    0,    0,    0,  814,    0,    0,
  812,  845,  814,    0,    0,  814,    0,    0,    0,    0,
    0,    0,    0,    0,  807,    0,    0,  807,    0,    0,
  814,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  813,  807,    0,  814,    0,    0,    0,    0,    0,
    0,    0,  814,    0,    0,  814,  814,    0,    0,    0,
    0,    0,    0,  812,    0,  814,  812,    0,    0,    0,
    0,    0,  522,  812,    0,  812,    0,    0,    0,  812,
    0,  814,  808,    0,  812,    0,    0,  812,    0,  522,
  522,  522,    0,    0,  813,  522,    0,  813,  522,    0,
    0,    0,  812,  814,  813,    0,  813,    0,  814,    0,
  813,  522,    0,  809,    0,  813,  812,  522,  813,    0,
  814,    0,  814,  808,  812,    0,  808,  812,  812,    0,
    0,    0,    0,  813,    0,    0,    0,  812,    0,    0,
    0,  808,    0,    0,    0,    0,    0,  813,    0,    0,
    0,    0,    0,  812,  809,  813,    0,  809,  813,  813,
    0,    0,    0,    0,    0,    0,    0,    0,  813,  814,
    0,    0,  809,    0,    0,  812,    0,    0,    0,    0,
  812,    0,    0,    0,  813,    0,  522,    0,    0,  814,
    0,    0,  812,  814,  812,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  813,  522,    0,    0,
    0,  813,  844,  522,  845,    0,    0,    0,    0,    0,
    0,    0,    0,  813,  522,  813,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  812,  522,  838,    0,  807,    0,  522,    0,    0,
    0,    0,    0,  844,    0,    0,  844,    0,    0,    0,
    0,  812,    0,    0,    0,  812,    0,  845,    0,    0,
  845,  844,  813,    0,    0,    0,  522,  845,    0,  845,
    0,    0,    0,  845,  838,    0,    0,  838,  845,    0,
    0,  845,  813,    0,    0,    0,  813,    0,  807,    0,
    0,  807,  838,    0,    0,    0,  845,    0,  807,    0,
  807,    0,    0,    0,  807,    0,    0,    0,    0,  807,
  845,    0,  807,    0,    0,    0,    0,    0,  845,    0,
    0,  845,  845,    0,    0,    0,    0,  807,    0,    0,
    0,  845,  846,    0,  808,    0,    0,    0,    0,    0,
    0,  807,    0,    0,    0,    0,    0,  845,    0,  807,
    0,    0,  807,  807,    0,    0,    0,    0,    0,    0,
    0,    0,  807,  847,    0,  809,    0,    0,    0,  845,
    0,    0,    0,  846,  845,    0,  846,    0,  807,    0,
    0,    0,    0,    0,    0,    0,  845,  808,  845,    0,
  808,  846,    0,    0,    0,    0,    0,  808,    0,  808,
  807,    0,    0,  808,  847,  807,    0,  847,  808,    0,
    0,  808,    0,    0,    0,    0,    0,  807,  809,  807,
    0,  809,  847,    0,    0,    0,  808,    0,  809,    0,
  809,    0,    0,    0,  809,  845,    0,    0,    0,  809,
  808,    0,  809,    0,    0,    0,    0,    0,  808,    0,
    0,  808,  808,    0,    0,  845,    0,  809,    0,  845,
    0,  808,  835,    0,  844,    0,  807,    0,    0,    0,
    0,  809,    0,    0,    0,    0,    0,  808,    0,  809,
    0,    0,  809,  809,    0,    0,  807,    0,    0,    0,
  807,    0,  809,  833,    0,  838,    0,    0,    0,  808,
    0,    0,    0,  835,  808,    0,  835,    0,  809,    0,
    0,    0,    0,    0,    0,    0,  808,  844,  808,    0,
  844,  835,    0,    0,    0,    0,    0,  844,    0,  844,
  809,    0,    0,  844,  833,  809,    0,  833,  844,    0,
    0,  844,    0,    0,    0,    0,    0,  809,  838,  809,
    0,  838,  833,    0,    0,    0,  844,    0,  838,    0,
  838,    0,    0,    0,  838,  808,    0,    0,    0,  838,
  844,    0,  838,    0,    0,    0,    0,    0,  844,    0,
    0,  844,  844,    0,    0,  808,    0,  838,    0,  808,
    0,  844,  839,    0,  846,    0,  809,    0,    0,    0,
    0,  838,    0,    0,    0,    0,    0,  844,    0,  838,
    0,    0,  838,  838,    0,    0,  809,    0,    0,    0,
  809,    0,  838,  848,    0,  847,    0,    0,    0,  844,
    0,    0,    0,  839,  844,    0,  839,    0,  838,    0,
    0,    0,    0,    0,    0,    0,  844,  846,  844,    0,
  846,  839,    0,    0,    0,    0,    0,  846,    0,  846,
  838,    0,    0,  846,  848,  838,    0,  848,  846,    0,
    0,  846,    0,    0,    0,    0,    0,  838,  847,  838,
    0,  847,  848,    0,    0,    0,  846,    0,  847,    0,
  847,    0,    0,    0,  847,  844,    0,    0,    0,  847,
  846,    0,  847,    0,    0,    0,    0,    0,  846,    0,
    0,  846,  846,    0,    0,  844,    0,  847,    0,  844,
    0,  846,  834,    0,  835,    0,  838,    0,    0,    0,
    0,  847,    0,    0,    0,    0,    0,  846,    0,  847,
    0,    0,  847,  847,    0,    0,  838,    0,    0,    0,
  838,    0,  847,  836,    0,  833,    0,    0,    0,  846,
    0,    0,    0,  834,  846,    0,  834,    0,  847,    0,
    0,    0,    0,    0,    0,    0,  846,  835,  846,    0,
  835,  834,    0,    0,    0,    0,    0,  835,    0,  835,
  847,    0,    0,  835,  836,  847,    0,  836,  835,    0,
    0,  835,    0,    0,    0,    0,    0,  847,  833,  847,
    0,  833,  836,    0,    0,    0,  835,    0,  833,    0,
  833,    0,    0,    0,  833,  846,    0,    0,    0,  833,
  835,    0,  833,    0,    0,    0,    0,    0,  835,    0,
    0,  835,  835,    0,    0,  846,    0,  833,    0,  846,
    0,  835,  840,    0,  839,    0,  847,    0,    0,    0,
    0,  833,    0,    0,    0,    0,    0,  835,    0,  833,
    0,    0,  833,  833,    0,    0,  847,    0,    0,    0,
  847,    0,  833,  679,    0,  848,    0,    0,    0,  835,
    0,    0,    0,  840,  835,    0,  840,    0,  833,    0,
    0,    0,    0,    0,    0,    0,  835,  839,  835,    0,
  839,  840,    0,    0,    0,    0,    0,  839,    0,  839,
  833,    0,    0,  839,  679,  833,    0,  679,  839,  666,
    0,  839,    0,    0,    0,    0,    0,  833,  848,  833,
    0,  848,  679,    0,    0,    0,  839,    0,  848,    0,
  848,    0,    0,    0,  848,  835,    0,    0,    0,  848,
  839,    0,  848,    0,    0,    0,    0,    0,  839,  666,
  666,  839,  839,  666,    0,  835,    0,  848,    0,  835,
    0,  839,    0,    0,  834,    0,  833,    0,  666,    0,
    0,  848,    0,    0,    0,    0,    0,  839,    0,  848,
    0,    0,  848,  848,    0,    0,  833,    0,    0,    0,
  833,    0,  848,    0,    0,  836,    0,    0,    0,  839,
    0,    0,    0,    0,  839,    0,    0,    0,  848,    0,
    0,    0,    0,    0,    0,    0,  839,  834,  839,    0,
  834,    0,    0,    0,    0,    0,    0,  834,    0,  834,
  848,    0,    0,  834,    0,  848,    0,    0,  834,    0,
    0,  834,    0,    0,    0,    0,    0,  848,  836,  848,
    0,  836,    0,    0,    0,    0,  834,    0,  836,    0,
  836,    0,    0,    0,  836,  839,    0,    0,    0,  836,
  834,    0,  836,    0,    0,    0,    0,    0,  834,    0,
    0,  834,  834,    0,  527,  839,    0,  836,    0,  839,
    0,  834,    0,    0,  840,    0,  848,    0,    0,    0,
    0,  836,    0,    0,    0,    0,    0,  834,    0,  836,
    0,    0,  836,  836,    0,    0,  848,    0,    0,    0,
  848,    0,  836,    0,    0,  527,    0,    0,  527,  834,
    0,    0,    0,    0,  834,    0,    0,    0,  836,    0,
    0,    0,    0,  527,    0,  527,  834,  840,  834,    0,
  840,    0,    0,    0,    0,    0,    0,  840,    0,  840,
  836,    0,    0,  840,  675,  836,    0,  443,  840,  444,
    0,  840,    0,    0,    0,    0,    0,  836,  679,  836,
    0,  679,  446,    0,    0,    0,  840,  296,  679,    0,
  679,    0,    0,    0,  679,  834,    0,    0,    0,  679,
  840,    0,  679,    0,    0,    0,    0,    0,  840,    0,
    0,  840,  840,    0,    0,  834,    0,  679,    0,  834,
    0,  840,    0,    0,  666,    0,  836,  666,  490,    0,
    0,  679,    0,    0,  666,    0,  666,  840,    0,  679,
  666,    0,    0,  679,    0,  666,  836,    0,  666,    0,
  836,    0,  679,    0,  109,    0,    0,    0,    0,  840,
    0,    0,    0,  666,  840,    0,    0,    0,  679,    0,
    0,    0,  110,  111,    0,    0,  840,  666,  840,    0,
    0,    0,    0,  112,  113,  666,  114,    0,    0,  666,
    0,    0,    0,  115,  116,  679,  117,  118,  666,  119,
    0,    0,  120,  121,  122,  123,  124,  679,    0,  679,
  125,  126,    0,    0,  666,    0,  127,  128,  129,    0,
    0,    0,    0,    0,    0,  840,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  666,  527,    0,    0,  840,    0,    0,    0,  840,
    0,    0,    0,  666,    0,  666,  679,    0,    0,    0,
  527,  527,    0,    0,    0,  527,    0,    0,  527,    0,
    0,    0,    0,    0,    0,    0,  679,    0,    0,    0,
  679,  527,    0,    0,    0,    0,    0,  527,    0,    0,
    0,  405,    0,    0,    0,    0,    0,    0,  406,    0,
    0,    0,  666,    0,    0,    0,    0,    0,    0,  675,
  407,    0,  443,    0,  444,    0,  408,  409,    0,    0,
    0,    0,  666,    0,    0,    0,  666,  446,    0,    0,
    0,    0,  296,    0,    0,  669,    0,    0,    0,    0,
    0,    0,    0,    0,  410,    0,    0,  411,    0,    0,
    0,    0,    0,    0,    0,    0,  527,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  412,    0,  413,  414,    0,  527,    0,    0,
    0,    0,    0,  527,  415,    0,    0,    0,  416,    0,
    0,    0,    0,    0,  527,    0,  898,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  527,    0,    0,    0,    0,  527,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   14,    0,    0,    0,  671,    0,    0,
    0,    0,    0,  417,    0,    0,  527,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  419,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,  421,  422,  423,  219,  220,    0,
    0,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,    0,    0,    0,  110,  111,  425,  426,  427,  428,
  429,  430,  431,  432,  433,  112,  434,    0,  114,    0,
    0,  435,  436,    0,  437,  438,  439,    0,  117,  118,
  440,  119,    0,    0,  120,  121,  122,  123,  124,  899,
  900,  901,  125,  126,    0,    0,  405,    0,  127,  441,
  129,    0,  442,  406,    0,  109,    0,  675,    0,    0,
  443,    0,  444,    0,    0,  407,    0,    0,  668,    0,
    0,  408,  409,  110,  111,  446,    0,    0,    0,    0,
  296,    0,    0,    0,  112,  113,    0,  114,    0,    0,
  669,    0,    0,    0,  115,  116,    0,  117,  118,  410,
  119,    0,  411,  120,  121,  122,  123,  124,    0,    0,
    0,  125,  126,    0,  494,    0,    0,  127,  128,  129,
    0,    0,    0,    0,    0,    0,    0,  412,    0,  413,
  414,  109,    0,    0,    0,    0,    0,    0,    0,  415,
    0,    0,  211,  416,    0,  211,    0,    0,    0,  110,
  111,  670,    0,    0,    0,    0,    0,    0,  211,    0,
  112,  113,  211,  114,    0,  211,    0,    0,    0,    0,
  115,  116,  102,  117,  118,    0,  119,    0,    0,  120,
  121,  122,  123,  124,    0,    0,    0,  125,  126,    0,
    0,    0,  671,  127,  128,  129,    0,    0,  417,    0,
    0,    0,    0,    0,  418,  211,    0,    0,    0,    0,
  103,  211,  211,  211,  419,  211,  211,  211,    0,  211,
    0,    0,  104,    0,    0,    0,    0,    0,  420,  421,
  422,  423,  219,  220,    0,    0,  142,  211,    0,  211,
    0,    0,  424,    0,    0,    0,  105,    0,    0,  110,
  111,  425,  426,  427,  428,  429,  430,  431,  432,  433,
  112,  434,    0,  114,    0,    0,  435,  436,  211,  437,
  438,  439,    0,  117,  118,  440,  119,    0,    0,  120,
  121,  672,  673,  674,  405,  211,    0,  125,  126,    0,
    0,  406,    0,  127,  441,  129,  675,  442,    0,  443,
    0,  444,    0,  407,    0,    0,    0,  211,    0,  408,
  409,  106,    0,    0,  446,    0,    0,    0,    0,  296,
    0,  107,    0,    0,    0,    0,    0,    0,  669,    0,
    0,    0,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,  211,  211,    0,    0,  108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  211,
    0,    0,    0,    0,  109,  412,    0,  413,  414,    0,
    0,    0,    0,    0,    0,    0,    0,  415,    0,    0,
    0,  416,  110,  111,    0,    0,    0,    0,    0,  894,
    0,    0,    0,  112,  113,    0,  114,    0,    0,    0,
    0,  102,    0,  115,  116,    0,  117,  118,    0,  119,
    0,    0,  120,  121,  122,  123,  124,    0,    0,    0,
  125,  126,    0,    0,    0,    0,  127,  128,  129,    0,
  671,    0,    0,    0,    0,    0,  417,    0,    0,  103,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,  104,  419,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,  421,  422,  423,
  219,  220,    0,    0,    0,  105,    0,    0,    0,    0,
  424,    0,    0,    0,    0,    0,    0,  110,  111,  425,
  426,  427,  428,  429,  430,  431,  432,  433,  112,  434,
    0,  114,    0,    0,  435,  436,    0,  437,  438,  439,
    0,  117,  118,  440,  119,    0,    0,  120,  121,  672,
  673,  674,    0,  405,    0,  125,  126,    0,    0,    0,
  406,  127,  441,  129,  675,  442,    0,  443,    0,  444,
  106,    0,  407,    0,    0,    0,    0,    0,  408,  409,
  107,    0,  446,    0,    0,    0,    0,  296,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  669,    0,    0,
    0,    0,    0,    0,    0,  108,  410,    0,    0,  411,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  109,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  412,    0,  413,  414,    0,    0,
    0,  110,  111,    0,    0,    0,  415,    0,    0,    0,
  416,    0,  112,  113,    0,  114,    0,    0,  670,    0,
    0,    0,  115,  116,    0,  117,  118,    0,  119,    0,
    0,  120,  121,  122,  123,  124,    0,    0,    0,  125,
  126,    0,    0,    0,    0,  127,  128,  129,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  671,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  989,    0,  420,  421,  422,  423,  219,
  220,    0,    0,    0,    0,    0,    0,    0,    0,  424,
    0,    0,    0,    0,    0,    0,  110,  111,  425,  426,
  427,  428,  429,  430,  431,  432,  433,  112,  434,    0,
  114,    0,    0,  435,  436,    0,  437,  438,  439,    0,
  117,  118,  440,  119,    0,    0,  120,  121,  672,  673,
  674,  405,    0,    0,  125,  126,    0,    0,  406,    0,
  127,  441,  129,  990,  442,    0,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,  408,  409,    0,    0,
    0,    0,    0,  514,    0,    0,  443,    0,  444,    0,
    0,    0,    0,    0,    0,  669,    0,    0,  991,    0,
    0,  446,    0,  992,  410,    0,  296,  411,    0,  993,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  109,    0,    0,    0,    0,    0,    0,    0,  994,    0,
    0,    0,  412,    0,  413,  414,    0,    0,  110,  111,
    0,    0,    0,    0,  415,    0,    0,    0,  416,  112,
  113,    0,  114,    0,    0,    0,  894,    0,    0,  115,
  116,    0,  117,  118,    0,  119,    0,    0,  120,  121,
  122,  123,  124,    0,    0,    0,  125,  126,    0,    0,
    0,    0,  127,  128,  129,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  671,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  419,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,  421,  422,  423,  219,  220,    0,
    0,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,    0,    0,    0,  110,  111,  425,  426,  427,  428,
  429,  430,  431,  432,  433,  112,  434,    0,  114,    0,
    0,  435,  436,    0,  437,  438,  439,    0,  117,  118,
  440,  119,    0,    0,  120,  121,  122,  123,  124,    0,
    0,    0,  125,  126,    0,    0,    0,    0,  127,  441,
  129,    0,  442, 1149,    0,    0, 1150,    0,  538,    0,
  405,  211,    0,    0,  211,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  211,    0,  407,
    0,  211,    0,    0,  211,  408,  409,    0,    0,    0,
    0,    0,  514,    0,    0,  443,    0,  444,  538,  538,
    0,    0,  538,    0,    0,    0,    0,    0,    0,    0,
  446,    0,    0,  410,    0,  296,  411,  538,    0,  538,
    0,    0,    0,    0,  211,    0,    0,    0,    0,    0,
  211,  211,  211,  570,  211,  211,  211,    0,  211,    0,
    0,  412,    0,  413,  414,    0,    0,    0,    0,  538,
    0,  570,  570,  415,    0,    0,  211,  416,  211,    0,
    0,    0,  570,  570,    0,  570,    0,    0,    0,    0,
    0,    0,  570,  570,    0,  570,  570,    0,  570,    0,
    0,  570,  570,  570,  570,  570,    0,  211,    0,  570,
  570,    0,   81,    0,    0,  570,  570,  570,    0,    0,
    0,    0,    0,    0,  211,    0,    0,    0,    0,    0,
 1151,    0,  417,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,  211,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  420,  421,  422,  423,  219,  220,    0,    0,
    0,    0,    0,    0,    0,    0,  424,    0,    0,    0,
    0,  211,  211,  110,  111,  425,  426,  427,  428,  429,
  430,  431,  432,  433,  112,  434,    0,  114,  211,    0,
  435,  436,    0,  437,  438,  439,    0,  117,  118,  440,
  119,    0,    0,  120,  121,  122,  123,  124,    0,    0,
    0,  125,  126,    0,    0,    0,    0,  127,  441,  129,
    0,  442, 1160,    0,    0, 1150,  538,    0,    0,  405,
    0,    0,    0,    0,    0,    0,  406,    0,  281,    0,
    0,  538,    0,  538,  538,  538,    0,    0,  407,  538,
    0,    0,  538,    0,  408,  409,  281,  281,    0,    0,
    0,  514,    0,    0,  443,  538,  444,  281,  281,    0,
  281,  538,    0,    0,  538,    0,    0,  281,  281,  446,
  281,  281,  410,  281,  296,  411,  281,  281,  281,  281,
  281,    0,    0,    0,  281,  281,    0,    0,    0,    0,
  281,  281,  281,    0,    0,    0,    0,    0,    0,    0,
  412,    0,  413,  414,    0,    0,  538,    0,    0,    0,
    0,    0,  415,    0,    0,    0,  416,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  538,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  538,    0,    0,    0,    0,    0,  538,    0,    0,
    0,  538,    0,    0,    0,    0,    0,    0,  538, 1151,
    0,  417,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,  538,  419,    0,    0,
    0,  538,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,  421,  422,  423,  219,  220,    0,    0,    0,
    0,    0,    0,    0,    0,  424,  538,    0,    0,    0,
  538,    0,  110,  111,  425,  426,  427,  428,  429,  430,
  431,  432,  433,  112,  434,    0,  114,    0,    0,  435,
  436,    0,  437,  438,  439,    0,  117,  118,  440,  119,
    0,    0,  120,  121,  122,  123,  124,    0,    0,    0,
  125,  126,    0,    0,    0,    0,  127,  441,  129,    0,
  442, 1164,    0,    0, 1150,    0,  532,    0,  405,  611,
    0,    0,  612,    0,    0,  406,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  407,    0,  613,
    0,    0,  614,  408,  409,    0,    0,    0,    0,    0,
  514,    0,    0,  443,    0,  444,    0,  532,    0,    0,
  532,    0,    0,    0,    0,    0,    0,    0,  446,    0,
    0,  410,    0,  296,  411,  532,    0,  532,    0,    0,
    0,    0,  615,    0,    0,    0,    0,    0,  616,  617,
  618,    0,  619,  620,  621,    0,  622,    0,    0,  412,
    0,  413,  414,    0,    0,    0,    0,  532,    0,    0,
    0,  415,    0,    0,  623,  416,  624,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  625,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  626,    0,    0,    0,    0,    0, 1151,    0,
  417,    0,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,  627,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  420,  421,  422,  423,  219,  220,    0,    0,    0,    0,
    0,    0,    0,    0,  424,    0,    0,    0,    0,  628,
  629,  110,  111,  425,  426,  427,  428,  429,  430,  431,
  432,  433,  112,  434,    0,  114,  630,    0,  435,  436,
    0,  437,  438,  439,    0,  117,  118,  440,  119,    0,
    0,  120,  121,  122,  123,  124,    0,    0,    0,  125,
  126,    0,    0,    0,    0,  127,  441,  129,    0,  442,
 1172,    0,    0, 1150,  532,    0,    0,  405,    0,    0,
    0,    0,    0,    0,  406,    0,    0,    0,    0,  532,
    0,  532,  532,  532,    0,    0,  407,  532,    0,    0,
  532,    0,  408,  409,    0,    0,    0,    0,    0,  514,
    0,    0,  443,  532,  444,    0,    0,    0,    0,  532,
    0,    0,  532,    0,    0,    0,    0,  446,    0,    0,
  410,    0,  296,  411,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  412,    0,
  413,  414,    0,    0,  532,    0,    0,    0,    0,    0,
  415,    0,    0,    0,  416,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  532,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  532,
    0,    0,    0,    0,    0,  532,    0,    0,    0,  532,
    0,    0,    0,    0,    0,    0,  532, 1151,    0,  417,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,  532,  419,    0,    0,    0,  532,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  420,
  421,  422,  423,  219,  220,    0,    0,    0,    0,    0,
    0,    0,    0,  424,  532,    0,    0,    0,  532,    0,
  110,  111,  425,  426,  427,  428,  429,  430,  431,  432,
  433,  112,  434,    0,  114,    0,    0,  435,  436,    0,
  437,  438,  439,    0,  117,  118,  440,  119,    0,    0,
  120,  121,  122,  123,  124,    0,    0,    0,  125,  126,
    0,    0,    0,    0,  127,  441,  129,    0,  442, 1175,
    0,    0, 1150,    0,  554,    0,  405,  141,    0,    0,
  141,    0,    0,  406,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  407,    0,  141,    0,    0,
  141,  408,  409,    0,    0,    0,    0,    0,  514,    0,
    0,  443,    0,  444,    0,  554,    0,    0,  554,    0,
    0,    0,    0,    0,    0,    0,  446,    0,    0,  410,
    0,  296,  411,  554,    0,  554,    0,    0,    0,    0,
  141,    0,    0,    0,    0,    0,  141,  141,  141,    0,
  141,  141,  141,    0,  141,    0,    0,  412,    0,  413,
  414,    0,    0,    0,    0,  554,    0,    0,    0,  415,
    0,    0,  141,  416,  141,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  141,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  141,    0,    0,    0,    0,    0, 1151,    0,  417,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,  141,    0,  419,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  420,  421,
  422,  423,  219,  220,    0,    0,    0,    0,    0,    0,
    0,    0,  424,    0,    0,    0,    0,  141,  141,  110,
  111,  425,  426,  427,  428,  429,  430,  431,  432,  433,
  112,  434,    0,  114,  141,    0,  435,  436,    0,  437,
  438,  439,    0,  117,  118,  440,  119,    0,    0,  120,
  121,  122,  123,  124,    0,    0,    0,  125,  126,    0,
    0,    0,    0,  127,  441,  129,    0,  442, 1180,    0,
    0, 1150,  554,    0,    0,  405,    0,    0,    0,    0,
    0,    0,  406,    0,    0,    0,    0,  554,    0,    0,
  554,  554,    0,    0,  407,  554,    0,    0,  554,    0,
  408,  409,    0,    0,    0,    0,    0,  514,    0,    0,
  443,  554,  444,    0,    0,    0,    0,  554,    0,    0,
  554,    0,    0,    0,    0,  446,    0,    0,  410,    0,
  296,  411,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  412,    0,  413,  414,
    0,    0,  554,    0,    0,    0,    0,    0,  415,    0,
    0,    0,  416,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  554,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  554,    0,    0,
    0, 1229,    0,  554,    0,    0,    0,  554,    0,    0,
    0,    0,    0,    0,  554, 1151,    0,  417,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    6,
    0,    0,  554,  419,    0,    0,    0,  554,    0,    0,
 1435, 1436,    0, 1437,    0,    0,    0,  420,  421,  422,
  423,  219,  220,    0,    0,    0,    0,    0,    0, 1438,
    0,  424,  554,    0,    0, 1439,  554,    0,  110,  111,
  425,  426,  427,  428,  429,  430,  431,  432,  433,  112,
  434,    0,  114,    0,    0,  435,  436,    0,  437,  438,
  439,    0,  117,  118,  440,  119,    0,    0,  120,  121,
  122,  123,  124,    0,    0,    0,  125,  126,    0,    0,
    0,    0,  127,  441,  129,    0,  442, 1183,    0,    0,
 1150,    0, 1440,    0,  405,    0,    0,    0,    0,    0,
    0,  406,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,  407,    0,    0,    0,    0,    0,  408,
  409,    0,    0,    0,    0, 1441,  514,    0,    0,  443,
    0,  444,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0,  446,    0,    0,  410,    0,  296,
  411,    0,    0,  109,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  110,  111,    0,    0,  412,    0,  413,  414,    0,
    0,    0, 1442,  113,    0,  114,    0,  415,    0,    0,
    0,  416,  115,  116,    0,  117,  118,    0,  119,    0,
    0,  120, 1443,  122,  123,  124,    0,    0,    0,  125,
  126, 1444, 1445, 1446,    0,  127,  128,  129,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1229,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1151,    0,  417,    0,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    6,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
 1435, 1436,    0, 1437,    0,    0,  420,  421,  422,  423,
  219,  220,    0,    0,    0,    0,    0,    0,    0, 1438,
  424,    0,    0,    0,    0, 1439,    0,  110,  111,  425,
  426,  427,  428,  429,  430,  431,  432,  433,  112,  434,
    0,  114,    0,    0,  435,  436,    0,  437,  438,  439,
    0,  117,  118,  440,  119,    0,    0,  120,  121,  122,
  123,  124,    0,    0,    0,  125,  126,    0,    0,    0,
    0,  127,  441,  129,    0,  442, 1186,    0,    0, 1150,
    0,    0, 1440,  405,  923,    0,    0,    0,    0,    0,
  406,    0,    0,    0,  639,    0,  639,  639,    0,  639,
   14,    0,  407,    0,    0,    0,    0,  924,  408,  409,
    0,    0,  639,    0,    0, 1441,    0,  639,    0,    0,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0, 1639,    0,  410,    0,    0,  411,
    0,    0,    0,  109,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  110,  111,  925,  412,    0,  413,  414,    0,    0,
    0,    0, 1442,  113,    0,  114,  415,    0,    0,    0,
  416,    0,  115,  116,    0,  117,  118,    0,  119,    0,
    0,  120, 1443,  122,  123,  124,    0,  109,    0,  125,
  126, 1444, 1445, 1446,    0,  127,  128,  129,    0,    0,
    0,    0,    0,    0,    0,  110,  111,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  112,  113,    0,  114,
    0,    0,    0, 1151,    0,  417,  115,  116,    0,  117,
  118,  418,  119,    0,    0,  120,  121,  122,  123,  124,
    0,  419,    0,  125,  126,    0,    0,    0,    0,  127,
  128,  129,    0,    0,    0,  420,  421,  422,  423,  219,
  220,    0,    0,    0,    0,    0,    0,    0,    0,  424,
    0,    0,    0,    0,    0,    0,  110,  111,  425,  426,
  427,  428,  429,  430,  431,  432,  433,  112,  434,    0,
  114,    0,    0,  435,  436,    0,  437,  438,  439,    0,
  117,  118,  440,  119,  639,    0,  120,  121,  122,  123,
  124,  639,    0,    0,  125,  126,    0,    0,  639,    0,
  127,  441,  129,  633,  442,  633,  633,    0,  633,    0,
  639,    0,    0,    0,    0,    0,  639,  639,    0,    0,
    0,  633,    0,    0,  639,    0,  633,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  639,    0,    0,  639,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  639,    0,  639,  639,    0,    0,    0,    0,
    0,    0,    0,    0,  639,    0,    0,    0,  639,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  639,    0,    0,    0,    0,    0,  639,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  639,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  639,  639,  639,  639,  639,  639,    0,
    0,    0,    0,    0,    0,    0,    0,  639,    0,    0,
    0,    0,    0,    0,  639,  639,  639,  639,  639,  639,
  639,  639,  639,  639,  639,  639,  639,    0,  639,    0,
    0,  639,  639,    0,  639,  639,  639,    0,  639,  639,
  639,  639,    0,  633,  639,  639,  639,  639,  639,    0,
  633,    0,  639,  639,    0,    0,    0,  633,  639,  639,
  639,  514,  639,  519,  443,    0,  444,    0,    0,  633,
    0,    0,    0,    0,    0,  633,  633,    0,    0,  446,
    0,    0,    0,  633,  296,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  633,    0,    0,  633,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  633,    0,  633,  633,    0,    0,    0,    0,    0,
    0,    0,    0,  633,    0,    0,    0,  633,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  633,    0,    0,    0,    0,    0,  633,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  633,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  633,  633,  633,  633,  633,  633,    0,    0,
    0,    0,    0,    0,    0,    0,  633,    0,    0,    0,
    0,    0,    0,  633,  633,  633,  633,  633,  633,  633,
  633,  633,  633,  633,  633,  633,    0,  633,    0,    0,
  633,  633,    0,  633,  633,  633,    0,  633,  633,  633,
  633,    0,    0,  633,  633,  633,  633,  633,  405,    0,
    0,  633,  633,    0,    0,  406,    0,  633,  633,  633,
  514,  633,    0,  443,    0,  444,    0,  407,    0,    0,
    0,    0,    0,  408,  409,    0,    0,    0,  446,    0,
    0,    0,    0,  296,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  410,    0,    0,  411,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  412,
    0,  413,  414,    0,    0,    0,    0,    0,    0,    0,
    0,  415,    0,    0,    0,  416,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  417,    0,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  420,  421,  422,  423,  219,  220,    0,    0,    0,    0,
    0,    0,    0,    0,  424,    0,    0,    0,    0,    0,
    0,  110,  111,  425,  426,  427,  428,  429,  430,  431,
  432,  433,  112,  434,    0,  114,    0,    0,  435,  436,
    0,  437,  438,  439,    0,  117,  118,  440,  119,    0,
    0,  120,  121,  122,  123,  124,    0,  405,    0,  125,
  126,    0,    0,    0,  406,  127,  441,  129,  514,  442,
    0,  443,    0,  444,    0,    0,  407,    0,    0,    0,
    0,    0,  408,  409,    0,    0,  446,    0,    0,    0,
    0,  296,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  410,    0,    0,  411,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  412,    0,
  413,  414,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,    0,    0,  416,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,  707,    0,    0,    0,    0,    0,  420,
  421,  422,  423,  219,  220,    0,    0,    0,    0,    0,
    0,    0,    0,  424,    0,    0,    0,    0,    0,    0,
  110,  111,  425,  426,  427,  428,  429,  430,  431,  432,
  433,  112,  434,    0,  114,    0,    0,  435,  436,    0,
  437,  438,  439,    0,  117,  118,  440,  119,    0,    0,
  120,  121,  122,  123,  124,  405,    0,    0,  125,  126,
    0,    0,  406,    0,  127,  441,  129,    0,  442,  544,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
  408,  409,  514,  966,    0,  443,    0,  444,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  446,    0,    0,    0,    0,  296,    0,    0,  410,    0,
  544,  411,    0,  544,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  544,    0,
  544,    0,    0,    0,    0,    0,  412,    0,  413,  414,
    0,    0,    0,    0,    0,    0,    0,    0,  415,    0,
    0,    0,  416,    0,    0,    0,    0,    0,    0,    0,
  544,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   14,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  420,  421,  422,
  423,  219,  220,    0,    0,    0,    0,    0,    0,    0,
    0,  424,    0,    0,    0,    0,    0,    0,  110,  111,
  425,  426,  427,  428,  429,  430,  431,  432,  433,  112,
  434,    0,  114,    0,    0,  435,  436,    0,  437,  438,
  439,    0,  117,  118,  440,  119,    0,    0,  120,  121,
  122,  123,  124,    0,    0,    0,  125,  126,    0,    0,
    0,    0,  127,  441,  129,    0,  442,    0,    0,  405,
    0,    0,    0,    0,    0,    0,  406,    0,    0,    0,
  514,    0,    0,  443,    0,  444,    0,  544,  407,    0,
    0,    0,    0,    0,  408,  409,    0,    0,  446,    0,
    0,    0,  544,  296,    0,  544,  544,    0,    0,    0,
  544,    0,    0,  544,    0,    0,    0,    0,    0,    0,
    0,    0,  410,    0,    0,  411,  544,    0,    0,    0,
    0,    0,  544,    0,    0,  544,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  412,    0,  413,  414,    0,    0,    0,    0,    0,    0,
    0,    0,  415,    0,    0,    0,  416,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  544,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  544,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  417,  544,    0,    0,    0,    0,  418,  544,    0,
    0,    0,  544,    0,    0,    0,    0,  419,    0,  544,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,  421,  422,  423,  219,  220,  544,    0,    0,
    0,    0,  544,    0,    0,  424,    0,    0,    0,    0,
    0,    0,  110,  111,  425,  426,  427,  428,  429,  430,
  431,  432,  433,  112,  434,    0,  114,  544,    0,  435,
  436,  544,  437,  438,  439,    0,  117,  118,  440,  119,
    0,    0,  120,  121,  122,  123,  124,  405,    0,    0,
  125,  126,    0,    0,  406,    0,  127,  441,  129,  514,
  442,    0,  443,    0,  444,    0,  407,    0,    0,    0,
    0,    0,  408,  409,    0,    0,    0,  446,    0,    0,
    0,    0,  296,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  410,    0,    0,  411,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  412,    0,
  413,  414,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,    0,    0,  416,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1178,    0,    0,  420,
  421,  422,  423,  219,  220,    0,    0,    0,    0,    0,
    0,    0,    0,  424,    0,    0,    0,    0,    0,    0,
  110,  111,  425,  426,  427,  428,  429,  430,  431,  432,
  433,  112,  434,    0,  114,    0,    0,  435,  436,    0,
  437,  438,  439,    0,  117,  118,  440,  119,    0,    0,
  120,  121,  122,  123,  124,    0,  405,    0,  125,  126,
    0,    0,    0,  406,  127,  441,  129,  514,  442,    0,
  443,    0,  444,    0,    0,  407,    0,    0,    0,    0,
    0,  408,  409,    0,    0,  446, 1736,    0,    0,    0,
  296,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  410,
    0,    0,  411,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  412,    0,  413,
  414,    0,    0,    0,    0,    0,    0,    0,    0,  415,
    0,    0,    0,  416,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  419,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1386,    0,    0,  420,  421,
  422,  423,  219,  220,    0,    0,    0,    0,    0,    0,
    0,    0,  424,    0,    0,    0,    0,    0,    0,  110,
  111,  425,  426,  427,  428,  429,  430,  431,  432,  433,
  112,  434,    0,  114,    0,    0,  435,  436,    0,  437,
  438,  439,    0,  117,  118,  440,  119,    0,    0,  120,
  121,  122,  123,  124,  405,    0,    0,  125,  126,    0,
    0,  406,    0,  127,  441,  129,  982,  442,  982,  982,
    0,  982,    0,  407,    0,    0,    0,    0,    0,  408,
  409,    0,    0,    0,  982,    0,    0,    0,    0,  982,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  412,    0,  413,  414,    0,
    0,    0,    0,    0,    0,    0,    0,  415,    0,    0,
    0,  416,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,  421,  422,  423,
  219,  220,    0,    0,    0,    0,    0,    0,    0,    0,
  424,    0,    0,    0,    0,    0,    0,  110,  111,  425,
  426,  427,  428,  429,  430,  431,  432,  433,  112,  434,
    0,  114,    0,    0,  435,  436,    0,  437,  438,  439,
    0,  117,  118,  440,  119,    0,    0,  120,  121,  122,
  123,  124,    0,  982,    0,  125,  126,    0,    0,    0,
  982,  127,  441,  129,  445,  442,    0,  443,    0,  444,
    0,    0,  982,    0,    0,    0,    0,    0,  982,  982,
    0,    0,  446,    0,    0,    0,    0,  296,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  982,    0,    0,  982,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  982,    0,  982,  982,    0,    0,
    0,    0,    0,    0,    0,    0,  982,    0,    0,    0,
  982,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  982,    0,    0,    0,    0,
    0,  982,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  982,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  982,  982,  982,  982,  982,
  982,    0,    0,    0,    0,    0,    0,    0,    0,  982,
    0,    0,    0,    0,    0,    0,  982,  982,  982,  982,
  982,  982,  982,  982,  982,  982,  982,  982,  982,    0,
  982,    0,    0,  982,  982,    0,  982,  982,  982,    0,
  982,  982,  982,  982,    0,    0,  982,  982,  982,  982,
  982,  405,    0,    0,  982,  982,    0,    0,  406,    0,
  982,  982,  982,  514,  982,    0,  443,    0,  444,    0,
  407,    0,    0,    0,    0,    0,  408,  409,    0,    0,
    0,  446,    0,    0,    0,    0,  296,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  410,    0,    0,  411,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  412,    0,  413,  414,    0,    0,    0,    0,
    0,    0,    0,    0,  415,    0,    0,    0,  416,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  419,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,  421,  422,  423,  219,  220,    0,
    0,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,    0,    0,    0,  110,  111,  425,  426,  427,  428,
  429,  430,  431,  432,  433,  112,  434,    0,  114,    0,
    0,  435,  436,    0,  437,  438,  439,    0,  117,  118,
  440,  119,    0,    0,  120,  121,  122,  123,  124,    0,
  405,    0,  125,  126,    0,    0,    0,  406,  127,  441,
  129,  982,  442,    0,  982,    0,  982,    0,    0,  407,
    0,    0,    0,    0,    0,  408,  409,    0,    0,  982,
    0,    0,    0,    0,  982,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  410,    0,    0,  411,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  412,    0,  413,  414,  529,    0,    0,    0,    0,
    0,    0,    0,  415,    0,    0,    0,  416,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  529,    0,    0,  529,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  529,    0,  529,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,    0,    0,    0,  529,    0,    0,    0,
    0,    0,  420,  421,  422,  423,  219,  220,    0,    0,
    0,    0,    0,    0,    0,    0,  424,    0,    0,    0,
    0,    0,    0,  110,  111,  425,  426,  427,  428,  429,
  430,  431,  432,  433,  112,  434,    0,  114,    0,    0,
  435,  436,    0,  437,  438,  439,    0,  117,  118,  440,
  119,    0,    0,  120,  121,  122,  123,  124,  982,    0,
    0,  125,  126,    0,    0,  982,    0,  127,  441,  129,
    0,  442,    0,    0,    0,    0,    0,  982,    0,    0,
    0,    0,    0,  982,  982,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  982,    0,    0,  982,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  982,
    0,  982,  982,    0,    0,    0,    0,    0,    0,    0,
    0,  982,    0,    0,    0,  982,    0,    0,    0,    0,
    0,    0,    0,  529,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  529,    0,
    0,  529,  529,    0,    0,    0,  529,    0,    0,  529,
    0,    0,    0,  321,    0,    0,    0,    0,    0,    0,
    0,    0,  529,    0,    0,    0,    0,    0,  529,    0,
  982,  529,    0,    0,    0,    0,  982,    0,    0,    0,
    0,  321,    0,    0,    0,    0,  982,    0,  321,    0,
    0,    0,  321,  321,    0,  321,    0,    0,  321,    0,
  982,  982,  982,  982,  982,  982,    0,    0,    0,    0,
    0,  321,    0,  529,  982,    0,    0,  321,    0,    0,
    0,  982,  982,  982,  982,  982,  982,  982,  982,  982,
  982,  982,  982,  982,    0,  982,    0,  529,  982,  982,
    0,  982,  982,  982,    0,  982,  982,  982,  982,    0,
    0,  982,  982,  982,  982,  982,    0,    0,  529,  982,
  982,    0,    0,    0,  529,  982,  982,  982,  529,  982,
    0,    0,    0,    0,  321,  529,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  321,  529,    0,    0,    0,    0,  529,    0,
    0,    0,    0,    0,    0,    0,    0,  321,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  321,    0,    0,
    0,    0,    0,  529,    0,    0,  321,  529,  321,    0,
    0,    0,    0,    0,    0,  321,    0,  611,    0,    0,
  612,    0,    0,    0,    0,    0,  812,    0,    0,    0,
    0,    0,    0,  321,  321,    0,    0,  613,    0,    0,
  614,    0,    0,    0,  321,  321,    0,  321,    0,    0,
    0,    0,    0,    0,  321,  321,    0,  321,  321,    0,
  321,    0,    0,  321,  321,  321,  321,  321,    0,    0,
    0,  321,  321,  321,  321,  321,    0,  321,  321,  321,
  615,    0,    0,    0,    0,    0,  616,  617,  618,    0,
  619,  620,  621,    0,  622,    0,    0,    0,  611,    0,
    0,  612,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  623,    0,  624,    0,    0,    0,  613,    0,
    0,  614,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  625,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  626,  615,    0,    0,    0,    0,    0,  616,  617,  618,
    0,  619,  620,  621,    0,  622,    0,    0,    0,    0,
    0,    0,  627,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  623,    0,  624,    0,  109,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  110,  111,  628,  629,    0,
    0,    0,    0,    0,  625,    0,  112,  113,    0,  114,
    0,    0,    0,    0,  630,    0,  115,  116,    0,  117,
  118,  626,  119,    0,    0,  120,  121,  122,  123,  124,
    0,    0,    0,  125,  126,    0,    0,    0,    0,  127,
  128,  129,    0,  627,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  109,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  110,  111,  628,  629,
    0,    0,    0,    0,    0,    0,    0,  112,  113,    0,
  114,    0,    0,    0,    0,  630,    0,  115,  116,    0,
  117,  118,    0,  119,    0,    0,  120,  121,  122,  123,
  124,    0,    0,    0,  125,  126,  109,    0,    0,    0,
  127,  128,  129,    0,    0,  174,  175,  176,  177,  178,
  179,  180,  181,  182,  110,  111,  183,  184,  185,  186,
  187,    0,    0,  188,  189,  112,  113,    0,  114,    0,
    0,  190,  191,    0,    0,  115,  116,  192,  117,  118,
  193,  119,  194,    0,  120,  121,  122,  123,  124,    0,
    0,    0,  125,  126,  195,  196,  197,  198,  127,  128,
  129,  199,  200,  458,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,  458,    0,    0,
  458,  458,  458,  458,    0,  458,    0,    0,  458,  458,
    0,    0,  458,  458,  458,  458,  458,  458,  458,  458,
    0,  458,  458,  458,  458,  458,    0,    0,    0,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
};
static short yycheck[] = {                                       9,
    4,    0,   12,  158,   67,  108,   16,   58,   18,  148,
  149,  549,  148,  149,   66,  731,  383,  383,   59,   65,
  379,   66,   16,   16,   97,   88,  354,   94,  482,   70,
  581,  352,   76,  445,  525,  552,  707,  786,   73,  375,
    0,   92,  713, 1228,  693,  502,   56,   57,   58,   59,
   60,   61,   93,   91,   98, 1303,   66,   67,   68,  100,
   70,   96,   72,   73,  281,   75,   76,   60,  784,  718,
  756,  928, 1070,  290,   84,  503,   86,   87,   88,   72,
   90,   91,   92,   93,  327,   95,   96,   97,   98,  162,
  100,  738,   86,   86,  139,   71,  141,   44,    0,   59,
 1059,   95,  985, 1059,    0,   41,    0,  964,  298,  663,
  361, 1070, 1059,  360, 1070,  153, 1018, 1122,   94,  160,
  574,   45,  576, 1070,  656, 1059,  234,  581, 1099,  139,
  140,  141,   45,    0,    0, 1064, 1070,  388,    0,   41,
  243,    0, 1071,  153, 1246,   41,    0,  140,  158,   44,
  160,  161,  162, 1082,    0, 1059,  166,   59,  204,  791,
    0,   44, 1059,   59,    0,   59, 1070, 1059,  161,  881,
 1066,   59,  327, 1070,  806,    0, 1105,  809, 1070,    0,
   45,    0,   41,  229,   40,   44,  818,    0,  445,   40,
  508,   44,   59,   59,    0,   41,    0,   59,   44,   44,
   59,  560,   44,   44,   44,   59, 1260,   63,  542,   40,
   91,  670,   63,   59,  581,  581,   44,   44,  280,   59,
  328, 1117,   41,   59,  298,  310,    0,  272,   41,  332,
  275,   44,  344,   63,   59,   41, 1210,   41,   59, 1213,
   59,    0,  874,    0, 1559,  351,   59,  315,   41,  608,
  921,  922,   44,   59,    0,   59,   61,  514,  309, 1444,
 1445,   40,  272,  584,   41,  275,   45,   41,  264,  279,
    0,  281,  282, 1813,   44,    0,  273,  494,  288,  447,
  290,  305,  327,  262,   41,   59,   44,  958,  371,  304,
  305,  315,   40,  544,   41,  317, 1513, 1295, 1296,  309,
   59,   93,   59,  313,  395,   45,  304,  305,  353,    0,
  722,  632,    0,   59, 1000,  360,  334,  362,  730,  364,
  313,    0,  367,   63,  409, 1011, 1012,  410, 1868,   59,
  376, 1329,  378,  273,   59,   41, 1295, 1296,   44, 1295,
 1296, 1558,   12,  353,  385,  396,  392,  502, 1295, 1296,
  360,  386,  362,   41,  364,  166,  315,  367,  288, 1261,
  434, 1295, 1296,   41, 1335,   41,  317,  858,   59,  431,
 1329,   59,  272, 1329,  457,  385,  386,  445,  484, 1275,
   59,  835, 1329,    0,  394,    0,  396, 1058,    0, 1318,
  400, 1295, 1296, 1042,  275, 1329,  400,  509, 1295, 1296,
  379,  394,   44, 1295, 1296,    0,  428,  403,  348,    0,
   61,  498, 1044,  315,    0, 1298,  434, 1274,  675,  315,
  380,  445,    0,  514,   41, 1329,    0, 1675,  443, 1061,
 1062,  297, 1329,  767,  336,  262,  446, 1329, 1492,  898,
  336,  759,   59,  297,   59,  443, 1420,   59, 1633, 1081,
 1424,   93,  493, 1301,  882,   41,  315,  502,   44, 1342,
 1239,  458, 1241,  599,   59,  893, 1781,  508,   59,  897,
  315, 1142, 1143,   59, 1145, 1146,  146,  336,  344,  800,
  490,   59,  803,  493,  494,   59,  445,    0, 1455,  317,
  344,  501,  297,  814,  264,  505,  315,  490,  508,  446,
  510,   41,  315,  513,  549,  264,   42,   93,  501,  315,
   41,  315,  505,  306,  402,  262,  562,  336,  380,  325,
  841,  327,    0,  336,   44, 1773,  450,  389,   41,  306,
  336,  748,  336,  600,  376,  402,  581,  450,  646,  549,
  380,    0,  785,  301,  446,  428,   59, 1649, 1005,  389,
  446, 1556,  379,  513,   59,  450,  602,  885,  315,  380,
  807,  380,  336,   41,  477,  401,   44,  297,  389, 1255,
  580,  581,  762, 1821,  380, 1129,  586,  513, 1225,  336,
  510,   59,   41,  389, 1116,  450, 1124,  446, 1517,  410,
 1018,  444, 1521, 1522,  450, 1007,  735, 1568, 1846,  450,
   59, 1503,    0,  428,  444,  446,  652,  653,  439,    0,
  655,  513, 1283,  432,  344,  361,    0,  513, 1046,  513,
  450,  872,  352,  444,  600, 1508,  432,  446, 1557, 1341,
  785,  390,  379,  446,  770, 1306,  361, 1611,  444,  387,
  446, 1537,  446,  509,  423,  655,  297,  493,  336,  283,
   41,  513,  264,   44,  513,  509,  386,   41,  388, 1555,
  698,   59,   41,   41,  259,  344,   44,  258,   59,  264,
 1502,  450,  446,  513,  269,   59, 1524,   41,    0,  270,
  410,  494,    0,  423,    0,    0,  264,  257,  698,  446,
  264,  736,  513,  738,  513,  740, 1881,  403,  315,   12,
  513,  379,  513,  379,   41,  435,  297,  513, 1340,  395,
  450, 1490,  303,  723,  305,  315, 1052, 1574,  759,  336,
  264,  731,  262, 1372,  260,   41,  736,  268,  738,  344,
  740,  262, 1661,  743, 1732,  395, 1665,   59,  748,  380,
  785,   59, 1154,   59,   59,  790,  756, 1068,  298,  759,
  796, 1680,    0,  344,    0,  765,  766, 1506,  446, 1080,
  330,  352,  807,    0,  300,  275,  833,    0, 1697,  779,
  430,  781,  989, 1732,  784,  992, 1732,  371,  390,  509,
  790,  301, 1118, 1104,    0, 1732,  779, 1304,  781,  834,
  835,  432,  262,  262,  756,  390,  397,  807, 1732, 1127,
  846,  868,  869,  870, 1687,  297,    0,    0, 1220,  253,
 1693,   59,  390,   59,  500,  416,  402,  261,  359,  453,
  454, 1653,   59,  336,  834,  835,   59,  265, 1732,  446,
  421,  341,    0,  146, 1505, 1732, 1327,  315,  848,  379,
 1732, 1808, 1333,   59,  848,    0,  440, 1259,  379,  327,
 1005, 1780,  344,  339, 1783, 1406,  315,  833,  336,  403,
  258,  299, 1247,  873,  840,   59,   59, 1834, 1196,   42,
  670,  262,  270,  883,   47,  675,  362,  336,    0,  855,
  873,  891,  892,  262,  434,    0,   41,  891,    0,   44,
  883,   59,  868,  869,  870,  273, 1825, 1154,  262,  297,
 1867,  821,  380,  284,   59,  303,  299,  305,    0,  379,
  379,  389, 1879,    0,  307, 1882,  297,  401,  509,   41,
  993,  380,   44, 1890,  315,  262,   41,  318,  306,   44,
  389,  315,  418,  446,  325,    0,  327,   59,   93, 1324,
  331,  325,  264,  327,   59,  336,  344,   59,  339,  990,
  994,  322,  336, 1624,  432,  305,  991,  427,  276,  277,
 1005,  450,    0,  354,  409,  315, 1102,   59,  446,    0,
  348,    0,   59,  432,  257, 1429,   41,  368,  390,  989,
  990,  991,  992,  993,  994,  376, 1345,  446,  379,  380,
 1000,  436,  261,    0,   59, 1316,  380,  300,  389,  315,
  379, 1011, 1012,   41,    0,  389,   44,    0, 1018,  325,
   41,  327,    0,   44,  405,  379,  428,  509,  264,  352,
  336,   59,  258,  338,  300,  403, 1347,  264,   59,  272,
   59,  264,  304,  305,   41,  513,  427,   44, 1000, 1406,
 1406,  432,  379,  262, 1111,  348,    0,  330,  432, 1011,
  462,  422,   59,  444,  513,  446,  371,  319,  429, 1126,
  276,  277,  446,   59,  380, 1132,   59,  303,  390,  372,
  264,   59,  348,  389, 1229, 1120,  280,  344,  276, 1124,
  458, 1091,  397,  276,  277,  400, 1757, 1803,  421,  298,
  338, 1762, 1096,  336,  894,  445,  264,  397,  898, 1109,
   40,  416,  493,    0,  277,   59, 1245, 1246, 1247,  282,
 1120,  509, 1369, 1370, 1124,  450,  432,  319,  316,  297,
 1377, 1378,  513, 1380, 1381,  334,  517,  442,  394,  513,
  446, 1388, 1389, 1390, 1391, 1111, 1393, 1394, 1395, 1396,
 1397, 1398,  477, 1279,  390, 1281,  258,  262,  448,  397,
 1126,  316,  400,  390, 1074, 1075, 1132,  390,  270, 1636,
  379,    0,   59,  298,   42,   43,  258,   45,  416,   47,
  348,  443,  396,  377,  410,    0, 1228,  264,  270,  372,
 1225,   59, 1192, 1228, 1229,  297,  453,  454,  412,    0,
  450,  303,  428,  305,  442,  393,  390,  513, 1192,  334,
  315,  318,   40,  318, 1124, 1526,  263, 1343,  427,  271,
  325,  303,  327,  305,  331, 1225,  331,    0, 1228, 1229,
   59,  336,  390,  285,  339,  434,  462,  431,  393, 1239,
   41, 1241,  344,   44,   59,  433,  376,  354,    0,  354,
  397, 1562,   44,  260,  448, 1255,  409,  402,   59,  318,
  376, 1261,  309,  368,  376,   44,  259,   59,   41,   44,
   44,  376,  331,  320,  379,  380,  269,  329,  433,  276,
  277,  336,   44,  436,  389,   59,   59,  315, 1816,  265,
  318,  378,  260,  300,  315,  354,    0,  325,  405,  327,
  405,  448,  333,  331,  325,  357,  327,   59,  336,  368,
 1455,  339,    0,  390,   93,  336,   42,   43,  339,   45,
 1356,   47,  427,  299,  372, 1361,  354,  432, 1322, 1796,
  298,   93,  376, 1800,  271, 1802,  376,  368,  484,  444,
  368,  446,  310,  428, 1811,  393,  405,    0,  376,  380,
 1495,  399,  380,   41,    0,   59,   44,  333,  295,  380,
  338,  389,   41,  486, 1831,   44,  334,  304,  389,  421,
  516,   59,  419,   42,   43,  372,   45,  405,   47,  277,
    0,  493, 1435, 1694,  282,  433,  260,    0,  493,    0,
    0,    0, 1859,  371,  338,   41,  393, 1864,   44, 1399,
    0,  338,  399,    0,  432,  342,   59,  509,  513,  277,
    0,  432,  517,   59,  282, 1399,  444,  333,  446,  397,
 1455,  298,  400,  444,  318,  446,  300,  371,   41,    0,
  410,  399,   41,   44,    0, 1435,  433,  331,  416,   59,
  434,  409,  485, 1443, 1444, 1445,   59,  446,   59,   59,
   59,  338,  495,  397,  260, 1455,  400,  334,  517,   59,
  354,  262,   59,  273,  442,  493,  434,  513,  336,   59,
   41,  258,  416,   44,  368,   42,   43,    0,   45,  260,
   47,    0,  376,  270,  371,  513,    0,    0,   59,  517,
 1490, 1636,  513,   59,  300, 1495,   42,   43,  442,   45,
  437,   47, 1502, 1503,   40, 1650,  443,  444,  445,    0,
  397,  405,  333,  400,  315,  416,  303,  318,  305,  300,
 1649,    0,  399,    0,  325,    0,  327, 1527,   41,  416,
  331,   44,  409,    0,    0,  336,   59,    0,  339,  457,
   59,  459,  315,    0, 1527,   59,   59,  368, 1239,  258,
 1241,  277,  261,  354,  327,  442,  282,  434,   46,  380,
 1601,  352,  339,  336,  341,    0,    0,  368,   59,    0,
   58,   42,   43,   61,   45,  376,   47,  514,  379,  380,
   59,  518,   59,  453,   59, 1678,   42,   43,  389,   45,
 1621,   47,   59,   59,  303,  386,   59,  388, 1598,  493,
  291, 1601,   59,    0,  405,  259,   41,  380,  277,   44,
  336,  302,    0,  282, 1598,  269,  389,    0,  309,  410,
  260, 1621,  264,  517,   59,   59,  427,  260,   59,   42,
   43,  432,   45, 1633,   47,   41,   42,   43, 1679,   45,
    0,   47,  333,  444,  435,  446,    0,  395,  313,   41,
 1650, 1796,   44, 1653,  315, 1800, 1645, 1802,   41,  432,
  300,   44,   59, 1808, 1700,   46, 1811,  300, 1704, 1669,
   46,   59,   41,  446, 1674,   44,   59,   58,  426, 1679,
   61, 1774,   58,  313,  314,   61, 1831,   41,  333, 1834,
   44, 1674,  493,  441,   41,   41, 1749,   44,   44,   59,
   41,  452,  315,   44,  395,   59,  315,   41,   42,   43,
  277,   45,  513,   47, 1859,  282,  517,   41,  327, 1864,
   44,   41, 1867,  336,   44,   41, 1726,  336,   44,   41,
   41,  277,   44,   44, 1879,  426,  282, 1882,  675,  430,
  513,  332,    0,   41,  315, 1890,   44,  444,  315, 1749,
 1750, 1264,   41, 1266,  325,   44,  327, 1751,  695,  428,
  485,  486,  699,  276,  277,  336,  403,  380,  339,  706,
  395,  380,  336, 1808,  711,  712,  389,  453,  454,   41,
  389, 1816,   44,   41,   40,  722,   44,  724,  725,  726,
  727,  728,  729,  730, 1827,  315, 1829,   42,   43, 1834,
   45,   59,   47, 1803, 1793,  317,  277,   41, 1808,  380,
   44,  282,   41, 1813,   41,   44, 1816,   44,  389,  432,
  395,  277,   41,  432,  376,   44,  282, 1827,  376, 1829,
  498,  768, 1867,  446, 1834, 1796,  428,  446,  356, 1800,
   41,  276,  277,   44, 1879,   41,  317, 1882,   44,  284,
 1811,  788,  308,   41,  426, 1890,   44,  794,  795,  372,
  442,  432,  384,  385,  277,  453,  454, 1867, 1868,  282,
 1831,  277,    0,  444,  377,  446,  282,  375,  497, 1879,
  393, 1881, 1882,   41,   42,   43,  399,   45,  409,   47,
 1890,  508,  508,  276,  277,  308,  461,  509, 1859,  284,
  513,  284,   44, 1864,  513,   41,   42,   43,    0,   45,
   61,   47,   40,   41,   42,   43,   44,   45,   46,   47,
  433,  366,  276,  277,   42,   43,   44,   45,  283,   47,
  284,   59,   60,   61,   62,   42,   43,  372,   45,   40,
   47,  376,  513,  277,  449,  450,  451,  264,  282,   41,
   40,  389,   44,   40,  453,   41,   42,   43,  393,   45,
   40,   47,   40,   91,  399,   93,   40,   59,   40,  906,
  907,  908,  477,  478,  911,  912,  913,   40,  915,  916,
  917,  918,  919,  920,   41,   42,   43,   40,   45,  372,
   47,   40,  929,  930,   40,  932,   40,  934,  433,  453,
  937,  938,  939,  940,  941,  942,  453,  694,   40,  696,
  393,   40,   40,  700,  701,  702,  399,  954,  372,  956,
  265,   41,   42,   43,   40,   45,   44,   47,   61,   91,
   40,   46,  277,    0,   41,   42,   43,  282,   45,  393,
   47,  264,  428,  317,  911,  399,  913,   44,  915,  916,
  433,  918,  919,  920,  299,  395,  317,  315,  493,   44,
  318,    0,   41,   42,   43,   46,   45,  325,   47,  327,
 1007,   44,  420,  331,   41,   61,   40,   44,  336,  433,
  264,  339,   44,  276,  344,   41,   42,   43,   40,   45,
  344,   47,   59,   41,   42,   43,  354,   45,  344,   47,
  276,  264,   41,  297,  390,   44,  428,  390,  450,    0,
  368,   41,   42,   43,  275,   45,  344,   47,  376,  277,
   59,  379,  380,  462,  282,  439,  439,  392,   40,  345,
   40,  389,   91,   40,  262,   40,  264,  265,   40,  493,
  315,  277,  264,  271,  376,  318,  282,  405,  403,  277,
   41,   44,  382,   44,  282,  262,   40,  285,  331,  277,
  375,   40,  380,  443,  282,   41,  339,  275,   59,  427,
  277,  299,  296,   63,  432,  282,  304,  305,  428,  456,
  308,  354,  314,   40,  276,  277,  444,  315,  446,  317,
  318,  277,  284,  321,  317,  368,  282,  325,  317,  327,
  444,  329,  440,  331,  264,   44,  494,   40,  336,  337,
   40,  339,  273,  297,  281,  340,  408, 1154,   42,   43,
  277,   45,  276,   47,  340,  282,  354,  355,  410,  357,
 1167, 1168,  405, 1170, 1171,  493,  379,   40,  379,  379,
  368, 1178,  370,  410,  372,  373,  374,  297,  376,  352,
   40,  379,  380,   40,   40,  513,  362,  277,   41,  517,
   44,  389,  282, 1200,   42,   43,  317,   45,  277,   47,
  277, 1208, 1209, 1210,  402,  282, 1213,  405, 1215,  438,
  372,  270, 1219, 1220, 1221,   44,   41,   42,   43,  378,
   45,  361,   47,  421,   44,  317,   40,   44,  277,  427,
  428,  393,  325,  282,  432,  266,   40,  399,  493,  276,
  277,   40,  414,  264,   41,  443,  444,  284,  446,   58,
   41,  277, 1259,   41,  452,   41,  282,  264,  387,  277,
  306,  446,  306,   41,  282,   41,   40,  276,  277,   40,
  317,  433,  470,  471,  517,  284,   40,  277,   40,   40,
    0,   40,  282,  481,  482,   40,  484,   40,  297,   40,
   40,   40, 1299,  491,  492,  493,  494,  495,   40,  497,
   40,  262,  500,  501,  502,  503,  504,   40,   40,   40,
  508,  509,  262,  305,  427,  513,  514,  515,  516,  517,
  324,   41,   42,   43,   44,   45,   46,   47,   41,   42,
   43,  493,   45,   44,   47,  372,   41,   41,  264,   59,
   60,   61,   62,  381,  339,  339,   41,   42,   43,  273,
   45,  327,   47,  284,  315,  379,  393,  318,  399,  384,
  379, 1368,  399,  372,  325,  379,  327, 1374,  262,  379,
  331,   91, 1379,   93,  379,  336,  379,   41,  339, 1386,
  450,  372,   41,  277,  393,  276,   40,  409,  282,   41,
  399,   41,   41,  354,  317,   41,  433,   41,   42,   43,
   41,   45,   44,   47,  305,  290,  338,  368,   41,   42,
   43,  427,   45, 1420,   47,  376,  264, 1424,  379,  380,
   59,   41,   42,   43,  433,   45,   40,   47,  389,  277,
  452,   59,  336, 1440,  282,   42,   43,   44,   45,  371,
   47,   59,   59,   59,  405,  378,   59,  336,  470,  471,
   59,   59,  277,  305,   59,   58,  493,  282,  428,  481,
  482,  339,  484,  333,  277,  397,  427,  434,  400,  491,
  492,  432,  494,  495,  298,  497,  334,  452,  500,  501,
  502,  503,  504,  444,  416,  446,  508,  509,  334,  434,
  298,   41,  514,  515,  516,  470,  471, 1504,   42,   43,
   41,   45,  378,   47,  348,   41,  481,  482,   41,  484,
  442,   41,   41,   41,  365,  365,  491,  492,   41,  494,
  495,   41,  497,   41, 1636,  500,  501,  502,  503,  504,
   41,   41,  493,  508,  509,   40,  315,   41,   41,  514,
  515,  516,  262,   41,  264,  265,   41,   41,   41,   40,
   59,  271,  513,   41,   40,  443,  517,  277,   41,   44,
  340,  348,  282,  458,  277,  285,  333,   40,  336,  282,
 1577,   46,   59,  336,   40,  500, 1583, 1584,   40,  299,
    0,  301,  277,  376,  304,  305,   44,  282,  308,   42,
   43,   44,   45,   41,   47,  315,   40,  317,  318,  402,
 1607,  321,   41,  376, 1611,  325,  427,  327, 1615,  329,
  376,  331,   41, 1620,   59, 1622,  336,  337,   44,  339,
  301,   41,   42,   43,   44,   45,   46,   47,  298,  434,
   41,  304,  301,  277,  354,  355,  455,  357,  282,   59,
   60,   61,   62,  290,  277,    0,  333,  376,  368,  282,
  370,  376,  372,  373,  374,  409,  376,  277,   40,  379,
  380,  276,  282,  264,  264,  450,  317,  462,  403,  389,
  277,   91,   44,   93,  443,  282,  270,   40,   42,   43,
   40,   45,  402,   47, 1796,  405,    0,   41, 1800,  270,
 1802,  264, 1699,  450,  339,   59,  339,  427,  291, 1811,
   41,  421,  450,   41,   41,   41,   59,  427,  428,  302,
   59,   41,  432,  305,   59,   59,  309,  301,    0, 1831,
  264,    0,    0,  443,  444,   59,  446,   41, 1242,  322,
   44, 1738,  452,  277,  290,   12,  283,  282,  282,  659,
  748, 1129, 1127,  886,   90,   59,  891, 1859,  848, 1124,
  470,  471, 1864, 1119, 1545,  350, 1546,    0,  380,  608,
 1767,  481,  482, 1109,  484, 1055,  330,  790, 1120, 1669,
 1616,  491,  492,  493,  494,  495,  281,  497, 1819,  362,
  500,  501,  502,  503,  504,  362,  379, 1524,  508,  509,
 1526, 1823,  586,  513,  514,  515,  516,  517,   41,  816,
 1822,   44,  395, 1810, 1848,    0,  464,  465,  466,  467,
  468,  469,  470,  471,  807, 1738,   59,  986,  411, 1443,
 1809, 1641,  736, 1277,  277, 1509,  348,  367,  355,  282,
  588, 1331,  865,  426,  611,   40,   13,  430,  645,  317,
  433,  527,  262,  531,  264,  265,   41,  151,  441,   44,
  779,  271,  768,   58, 1018,  857,   86,  277, 1265, 1767,
  692, 1399,  282,  542,   59,  285,   61,  460, 1598,  755,
 1008, 1285,  794, 1169, 1215,   -1,   -1,   -1,   -1,  299,
    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,  277,   -1,   -1,  336,  337,  282,  339,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,  276,  277,   -1,  470,  471,  281,   -1,  389,
  284,   91,   -1,   93,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,  402,  297,   -1,  405,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,
  515,  516,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  297,   -1,   -1,    0,  372,    0,
   -1,  481,  482,  258,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,  273,  393,
  500,  501,  502,  503,  504,  399,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,   41,   -1,
   41,   44,  297,   44,   -1,   -1,   -1,   -1,  303,   -1,
   -1,  306,   -1,   -1,   -1,   -1,   59,   -1,   59,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   42,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
  393,  271,   -1,  348,   -1,   -1,  399,  277,   -1,   -1,
   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
  433,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,  403,  329,
   -1,  331,   -1,   -1,   -1,  410,  336,  337,   -1,  339,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,   -1,  428,  354,  355,   -1,  357,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,  458,   -1,  470,  471,  462,   -1,  389,
   -1,   91,   -1,   93,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,
  515,  516,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,  276,  277,  276,  277,   -1,   -1,   -1,
   -1,  284,   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
 1369, 1370,   -1,  513,  514,  515,  516,  517, 1377, 1378,
   -1, 1380, 1381,   -1,   -1,   -1,   -1,   -1,   -1, 1388,
 1389, 1390, 1391,   -1, 1393, 1394, 1395, 1396, 1397, 1398,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
  393,  271,  393,   -1,   -1,   -1,  399,  277,  399,   -1,
   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
  433,   -1,  433,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   45,   -1,   -1,   -1,  336,  337,   -1,  339,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,
   -1,   91,   -1,   93,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,
  515,  516,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,  302,   -1,
   -1,   -1,  452,   -1,   -1,  309,   -1,   -1,   -1,  313,
  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  322,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  333,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  395,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  411,   -1,   44,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,  271,  426,   -1,   -1,   -1,  430,  277,   -1,  292,
   -1,   -1,  282,   -1,   -1,  285,   -1,  441,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
    0,   -1,   -1,   -1,  304,  305,  460,  320,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,  348,  336,  337,   -1,  339,
   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  402,   -1,   -1,  405,  419,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   45,  421,  435,   -1,   -1,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,   -1,  449,  450,  451,   -1,
  453,  454,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,   -1,   -1,
  470,  471,   -1,   -1,  487,  488,   -1,   -1,   -1,   -1,
   -1,  481,  482,  496,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,  518,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  317,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,  271,   -1,   58,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,
   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,
   -1,   -1,   -1,   93,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,
  515,  516,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  450,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,  262,   -1,  264,  265,  491,  492,   -1,  494,
  495,  271,  497,   -1,   59,  500,  501,  502,  503,  504,
   -1,   -1,  282,  508,  509,  285,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,
   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,
   -1,   -1,   -1,   93,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,
  515,  516,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,    0,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,   -1,   -1,
   -1,   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,  271,   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   41,   -1,   -1,   44,  336,  337,   -1,  339,
   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
   59,   -1,   61,   -1,  354,  355,   -1,  357,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,
   -1,   -1,   -1,   93,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   59,  500,  501,  502,  503,  504,
   -1,  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,
  515,  516,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  258,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,  273,   -1,  275,  276,
  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  297,  481,  482,   -1,  484,   -1,  303,   -1,   -1,  306,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,  275,  276,  277,   -1,
   -1,  271,  281,   -1,   -1,  284,  393,   -1,   -1,   -1,
   -1,   -1,  399,   -1,   -1,  285,  403,   -1,  297,    0,
   -1,   -1,   -1,  410,  303,   -1,   -1,   -1,   -1,  299,
   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
   -1,  428,   -1,   -1,   -1,  315,  433,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   41,  331,   43,   44,   45,   -1,  336,  337,   -1,  339,
   -1,  458,   -1,   -1,   -1,  462,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   93,   -1,  393,   -1,   -1,   -1,   -1,  389,
  399,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  408,
   -1,  410,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,  421,   -1,   -1,  433,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,  284,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,  452,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,  262,  497,  264,  265,  500,  501,  502,  503,  504,
  271,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   41,
  331,   43,   44,   45,   -1,  336,  337,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   93,   -1,   -1,  470,  471,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,
  516,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,  284,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,   -1,   -1,   -1,
   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   41,  331,
   43,   44,   45,   -1,  336,  337,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   93,   -1,   -1,  470,  471,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,
   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,  516,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,  470,  471,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
  321,  513,  514,  515,  516,  517,   -1,   -1,  329,   -1,
   42,   43,   -1,   45,   -1,   47,  337,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,
   62,   -1,   -1,   -1,  355,   -1,  357,   -1,   -1,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,  370,
   -1,  372,  373,  374,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
  421,   -1,  325,   -1,  327,   -1,  329,   41,  331,   43,
   44,   45,   -1,  336,  337,   -1,  339,  272,   -1,   -1,
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
   -1,   -1,   -1,  398,   -1,  277,   -1,  470,  471,   -1,
  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,  481,  482,
  415,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,  437,   -1,   -1,  508,  509,   -1,   -1,  321,
  513,  514,  515,  516,  517,   -1,   -1,  329,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  337,  325,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,  472,  473,   -1,
   -1,   -1,   -1,  355,   -1,  357,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,  489,   -1,   -1,  271,  370,   42,
  372,  373,  374,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  285,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,  299,   -1,   -1,   -1,   -1,
  304,  305,   -1,   -1,  308,   59,   60,   61,   62,   -1,
  261,  315,   -1,  317,  318,   -1,   -1,  321,   -1,  421,
   -1,  325,   -1,  327,   -1,  329,   -1,  331,  279,   -1,
   -1,   -1,  336,  337,   -1,  339,  287,   91,   -1,   -1,
   -1,   -1,   -1,  294,   -1,  296,  435,  298,   -1,   -1,
  354,  355,  303,  357,   -1,   -1,   -1,   -1,   -1,  310,
   -1,   -1,   -1,  452,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,  324,   -1,  379,  380,   -1,   -1,   -1,
   -1,  470,  471,  334,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,  402,   -1,
   -1,  405,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  421,   -1,  508,
  509,   -1,   -1,  427,  428,  514,  515,  516,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
  444,   -1,  446,   -1,   -1,   -1,  397,   -1,  452,   -1,
   -1,   -1,   -1,  404,   -1,   -1,  407,   -1,  409,  410,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,  434,   -1,   -1,   -1,  491,  492,  493,
  494,  495,   -1,  497,   -1,    0,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  262,  513,
  514,  515,  516,  517,   -1,   -1,   -1,  271,   -1,   -1,
  293,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,
   -1,  285,  483,   -1,   -1,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,  498,  499,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,  315,   -1,   -1,  318,   -1,   -1,  321,   -1,  272,
   -1,  325,  275,  327,   -1,  329,   -1,  331,   -1,   -1,
   -1,   -1,  336,  337,   -1,  339,   91,  290,   -1,  292,
   -1,   -1,  295,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,  335,   -1,   -1,  389,   -1,   -1,  341,  342,
  343,    0,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
   -1,  405,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,  367,   -1,  369,  421,   -1,   -1,
   -1,   -1,   59,  427,   -1,   -1,   -1,   -1,  432,  452,
   -1,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
  444,   -1,  446,   -1,   -1,  398,   -1,  470,  471,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,  415,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   91,   -1,  437,  508,  509,   -1,   -1,  493,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  513,
   -1,   -1,   -1,  517,   -1,   -1,  271,   -1,   -1,  472,
  473,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,  489,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,  321,    0,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   -1,   -1,   -1,   40,   41,   -1,
   -1,   44,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   59,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  405,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,  421,   -1,  277,   -1,
   -1,   -1,  427,  282,   -1,   -1,  285,  432,  315,   -1,
   -1,  318,   -1,   41,   -1,   -1,   44,   -1,  325,  444,
  327,  446,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   59,  339,   -1,   -1,   -1,  315,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,  354,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,  368,   -1,   -1,   -1,   -1,   -1,   -1,  493,  376,
   -1,   -1,   -1,  380,   -1,  354,  355,   -1,  357,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,  513,  368,
   -1,  370,  517,  372,  373,  374,   -1,  376,  405,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  432,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,  421,   -1,   -1,  452,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,    0,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,   -1,  315,   -1,  493,  318,   -1,   41,   -1,   -1,
   44,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,  513,   59,  339,   -1,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,  389,  315,   -1,   41,
  318,   -1,   44,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,  405,  331,   -1,   -1,   -1,   59,  336,   -1,
   -1,  339,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,  354,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
  368,  444,   -1,  446,   -1,   -1,   -1,   -1,  376,  452,
   -1,   -1,  380,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,   59,   -1,   61,   -1,  405,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,  432,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,   -1,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,    0,   -1,  513,  514,  515,  516,  517,
   -1,  315,   -1,   41,  318,   -1,   44,   -1,   -1,   -1,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
  262,   59,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   59,   -1,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  389,  318,   -1,   -1,   -1,
   -1,   -1,  258,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,  405,    0,   -1,  336,   -1,   -1,  339,   -1,  275,
  276,  277,   -1,   -1,  315,  281,   -1,  318,  284,   -1,
   -1,   -1,  354,  427,  325,   -1,  327,   -1,  432,   -1,
  331,  297,   -1,    0,   -1,  336,  368,  303,  339,   -1,
  444,   -1,  446,   41,  376,   -1,   44,  379,  380,   -1,
   -1,   -1,   -1,  354,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,  405,   41,  376,   -1,   44,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,  493,
   -1,   -1,   59,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,  405,   -1,  372,   -1,   -1,  513,
   -1,   -1,  444,  517,  446,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,  393,   -1,   -1,
   -1,  432,    0,  399,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,  410,  446,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  493,  428,    0,   -1,  262,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,  513,   -1,   -1,   -1,  517,   -1,  315,   -1,   -1,
  318,   59,  493,   -1,   -1,   -1,  462,  325,   -1,  327,
   -1,   -1,   -1,  331,   41,   -1,   -1,   44,  336,   -1,
   -1,  339,  513,   -1,   -1,   -1,  517,   -1,  315,   -1,
   -1,  318,   59,   -1,   -1,   -1,  354,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,  354,   -1,   -1,
   -1,  389,    0,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,    0,   -1,  262,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   41,  432,   -1,   44,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,
  318,   59,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
  427,   -1,   -1,  331,   41,  432,   -1,   44,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   59,   -1,   -1,   -1,  354,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  513,   -1,  354,   -1,  517,
   -1,  389,    0,   -1,  262,   -1,  493,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  389,    0,   -1,  262,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   41,  432,   -1,   44,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,
  318,   59,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
  427,   -1,   -1,  331,   41,  432,   -1,   44,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   59,   -1,   -1,   -1,  354,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  513,   -1,  354,   -1,  517,
   -1,  389,    0,   -1,  262,   -1,  493,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  389,    0,   -1,  262,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   41,  432,   -1,   44,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,
  318,   59,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
  427,   -1,   -1,  331,   41,  432,   -1,   44,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   59,   -1,   -1,   -1,  354,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  513,   -1,  354,   -1,  517,
   -1,  389,    0,   -1,  262,   -1,  493,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  389,    0,   -1,  262,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   41,  432,   -1,   44,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,
  318,   59,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
  427,   -1,   -1,  331,   41,  432,   -1,   44,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   59,   -1,   -1,   -1,  354,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  513,   -1,  354,   -1,  517,
   -1,  389,    0,   -1,  262,   -1,  493,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  389,    0,   -1,  262,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   41,  432,   -1,   44,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,
  318,   59,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
  427,   -1,   -1,  331,   41,  432,   -1,   44,  336,    0,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   59,   -1,   -1,   -1,  354,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   40,
   41,  379,  380,   44,   -1,  513,   -1,  354,   -1,  517,
   -1,  389,   -1,   -1,  262,   -1,  493,   -1,   59,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  389,   -1,   -1,  262,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
  427,   -1,   -1,  331,   -1,  432,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   -1,   -1,   -1,   -1,  354,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,    0,  513,   -1,  354,   -1,  517,
   -1,  389,   -1,   -1,  262,   -1,  493,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  405,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  389,   -1,   -1,   41,   -1,   -1,   44,  427,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   59,   -1,   61,  444,  315,  446,   -1,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
  427,   -1,   -1,  331,   40,  432,   -1,   43,  336,   45,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   58,   -1,   -1,   -1,  354,   63,  325,   -1,
  327,   -1,   -1,   -1,  331,  493,   -1,   -1,   -1,  336,
  368,   -1,  339,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  513,   -1,  354,   -1,  517,
   -1,  389,   -1,   -1,  315,   -1,  493,  318,  426,   -1,
   -1,  368,   -1,   -1,  325,   -1,  327,  405,   -1,  376,
  331,   -1,   -1,  380,   -1,  336,  513,   -1,  339,   -1,
  517,   -1,  389,   -1,  452,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,  354,  432,   -1,   -1,   -1,  405,   -1,
   -1,   -1,  470,  471,   -1,   -1,  444,  368,  446,   -1,
   -1,   -1,   -1,  481,  482,  376,  484,   -1,   -1,  380,
   -1,   -1,   -1,  491,  492,  432,  494,  495,  389,  497,
   -1,   -1,  500,  501,  502,  503,  504,  444,   -1,  446,
  508,  509,   -1,   -1,  405,   -1,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  432,  258,   -1,   -1,  513,   -1,   -1,   -1,  517,
   -1,   -1,   -1,  444,   -1,  446,  493,   -1,   -1,   -1,
  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,   -1,
  517,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,   40,
  286,   -1,   43,   -1,   45,   -1,  292,  293,   -1,   -1,
   -1,   -1,  513,   -1,   -1,   -1,  517,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,  393,   -1,   -1,
   -1,   -1,   -1,  399,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,  410,   -1,  372,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  409,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,  462,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  505,
  506,  507,  508,  509,   -1,   -1,  267,   -1,  514,  515,
  516,   -1,  518,  274,   -1,  452,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,  286,   -1,   -1,  289,   -1,
   -1,  292,  293,  470,  471,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  311,   -1,   -1,   -1,  491,  492,   -1,  494,  495,  320,
  497,   -1,  323,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,  435,   -1,   -1,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,  272,  364,   -1,  275,   -1,   -1,   -1,  470,
  471,  372,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,
  481,  482,  292,  484,   -1,  295,   -1,   -1,   -1,   -1,
  491,  492,  260,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  413,  514,  515,  516,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,  335,   -1,   -1,   -1,   -1,
  298,  341,  342,  343,  435,  345,  346,  347,   -1,  349,
   -1,   -1,  310,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,  324,  367,   -1,  369,
   -1,   -1,  463,   -1,   -1,   -1,  334,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,  398,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  267,  415,   -1,  508,  509,   -1,
   -1,  274,   -1,  514,  515,  516,   40,  518,   -1,   43,
   -1,   45,   -1,  286,   -1,   -1,   -1,  437,   -1,  292,
  293,  399,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,  472,  473,   -1,   -1,  434,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
   -1,   -1,   -1,   -1,  452,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,  470,  471,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,  260,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,  298,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  310,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,  334,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  267,   -1,  508,  509,   -1,   -1,   -1,
  274,  514,  515,  516,   40,  518,   -1,   43,   -1,   45,
  399,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
  409,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  434,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,  481,  482,   -1,  484,   -1,   -1,  372,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  325,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,
  514,  515,  516,  395,  518,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,  430,   -1,
   -1,   58,   -1,  435,  320,   -1,   63,  323,   -1,  441,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  481,
  482,   -1,  484,   -1,   -1,   -1,  372,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  260,   -1,   -1,  263,   -1,    0,   -1,
  267,  272,   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,  286,
   -1,  292,   -1,   -1,  295,  292,  293,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   40,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,  320,   -1,   63,  323,   59,   -1,   61,
   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,
  341,  342,  343,  452,  345,  346,  347,   -1,  349,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   91,
   -1,  470,  471,  360,   -1,   -1,  367,  364,  369,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  398,   -1,  508,
  509,   -1,  403,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,  472,  473,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,  489,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  260,   -1,   -1,  263,  258,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,  452,   -1,
   -1,  273,   -1,  275,  276,  277,   -1,   -1,  286,  281,
   -1,   -1,  284,   -1,  292,  293,  470,  471,   -1,   -1,
   -1,   40,   -1,   -1,   43,  297,   45,  481,  482,   -1,
  484,  303,   -1,   -1,  306,   -1,   -1,  491,  492,   58,
  494,  495,  320,  497,   63,  323,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,  348,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  393,   -1,   -1,   -1,   -1,   -1,  399,   -1,   -1,
   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,  410,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,  435,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,  458,   -1,   -1,   -1,
  462,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  260,   -1,   -1,  263,   -1,    0,   -1,  267,  272,
   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,  292,
   -1,   -1,  295,  292,  293,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,  320,   -1,   63,  323,   59,   -1,   61,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   91,   -1,   -1,
   -1,  360,   -1,   -1,  367,  364,  369,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,  472,
  473,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,  489,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  260,   -1,   -1,  263,  258,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,  273,
   -1,  275,  276,  277,   -1,   -1,  286,  281,   -1,   -1,
  284,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,  297,   45,   -1,   -1,   -1,   -1,  303,
   -1,   -1,  306,   -1,   -1,   -1,   -1,   58,   -1,   -1,
  320,   -1,   63,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,   -1,  399,   -1,   -1,   -1,  403,
   -1,   -1,   -1,   -1,   -1,   -1,  410,  417,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,  435,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,  458,   -1,   -1,   -1,  462,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  260,
   -1,   -1,  263,   -1,    0,   -1,  267,  272,   -1,   -1,
  275,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,  292,   -1,   -1,
  295,  292,  293,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  320,
   -1,   63,  323,   59,   -1,   61,   -1,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,  360,
   -1,   -1,  367,  364,  369,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  415,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,  472,  473,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  260,   -1,
   -1,  263,  258,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,  273,   -1,   -1,
  276,  277,   -1,   -1,  286,  281,   -1,   -1,  284,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,  297,   45,   -1,   -1,   -1,   -1,  303,   -1,   -1,
  306,   -1,   -1,   -1,   -1,   58,   -1,   -1,  320,   -1,
   63,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,
   -1,  270,   -1,  399,   -1,   -1,   -1,  403,   -1,   -1,
   -1,   -1,   -1,   -1,  410,  417,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,  298,
   -1,   -1,  428,  435,   -1,   -1,   -1,  433,   -1,   -1,
  309,  310,   -1,  312,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,  328,
   -1,  463,  458,   -1,   -1,  334,  462,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  260,   -1,   -1,
  263,   -1,  391,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,  424,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,  320,   -1,   63,
  323,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  270,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,  298,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  309,  310,   -1,  312,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  328,
  463,   -1,   -1,   -1,   -1,  334,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  260,   -1,   -1,  263,
   -1,   -1,  391,  267,  339,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,   45,
  409,   -1,  286,   -1,   -1,   -1,   -1,  362,  292,  293,
   -1,   -1,   58,   -1,   -1,  424,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  418,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,  360,   -1,   -1,   -1,
  364,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  452,   -1,  508,
  509,  510,  511,  512,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,  417,   -1,  419,  491,  492,   -1,  494,
  495,  425,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  435,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,  260,   -1,  500,  501,  502,  503,
  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,
  514,  515,  516,   40,  518,   42,   43,   -1,   45,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   58,   -1,   -1,  300,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,  260,  500,  501,  502,  503,  504,   -1,
  267,   -1,  508,  509,   -1,   -1,   -1,  274,  514,  515,
  516,   40,  518,   42,   43,   -1,   45,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,
   -1,   -1,   -1,  300,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,
   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,
   40,  518,   -1,   43,   -1,   45,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  267,   -1,  508,
  509,   -1,   -1,   -1,  274,  514,  515,  516,   40,  518,
   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  267,   -1,   -1,  508,  509,
   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,    0,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   40,   41,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,
   41,  323,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,  258,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,
   -1,   -1,  273,   63,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,  393,   -1,   -1,   -1,   -1,  425,  399,   -1,
   -1,   -1,  403,   -1,   -1,   -1,   -1,  435,   -1,  410,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  428,   -1,   -1,
   -1,   -1,  433,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,  458,   -1,  487,
  488,  462,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,
  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,   40,
  518,   -1,   43,   -1,   45,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,  267,   -1,  508,  509,
   -1,   -1,   -1,  274,  514,  515,  516,   40,  518,   -1,
   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   58,   59,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,
   -1,  274,   -1,  514,  515,  516,   40,  518,   42,   43,
   -1,   45,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  267,   -1,  508,  509,   -1,   -1,   -1,
  274,  514,  515,  516,   40,  518,   -1,   43,   -1,   45,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,
  514,  515,  516,   40,  518,   -1,   43,   -1,   45,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  267,   -1,  508,  509,   -1,   -1,   -1,  274,  514,  515,
  516,   40,  518,   -1,   43,   -1,   45,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,
   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,
   -1,  518,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,
   -1,  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,
   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,
  419,  306,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,  298,   -1,   -1,   -1,   -1,  435,   -1,  305,   -1,
   -1,   -1,  309,  310,   -1,  312,   -1,   -1,  315,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,  328,   -1,  348,  463,   -1,   -1,  334,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,  372,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,  393,  508,
  509,   -1,   -1,   -1,  399,  514,  515,  516,  403,  518,
   -1,   -1,   -1,   -1,  391,  410,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  409,  428,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,  458,   -1,   -1,  443,  462,  445,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  272,   -1,   -1,
  275,   -1,   -1,   -1,   -1,   -1,  281,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  292,   -1,   -1,
  295,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,   -1,  272,   -1,
   -1,  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  367,   -1,  369,   -1,   -1,   -1,  292,   -1,
   -1,  295,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  415,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,
   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,
   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,   -1,  369,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,   -1,
   -1,   -1,   -1,   -1,  398,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,  489,   -1,  491,  492,   -1,  494,
  495,  415,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  437,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,  489,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,  452,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,  461,  462,  463,  464,  465,
  466,  467,  468,  469,  470,  471,  472,  473,  474,  475,
  476,   -1,   -1,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,   -1,  491,  492,  493,  494,  495,
  496,  497,  498,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,  510,  511,  512,  513,  514,  515,
  516,  517,  518,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  462,  463,  464,  465,  466,  467,  468,
  469,  470,  471,  472,  473,  474,  475,  476,   -1,   -1,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,   -1,  491,  492,  493,  494,  495,  496,  497,  498,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,  513,  514,  515,  516,  517,  518,
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
"IIF","SCALAR_ARRAY","CROSS","CURRENT_DATABASE",
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
"default_value : current_database",
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
"keyword_or_column : CURRENT_DATABASE",
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
"value : current_database",
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
"current_database : CURRENT_DATABASE",
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
"non_reserved_word : WEEKDAY",
"non_reserved_word : YEARDAY",
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
case 221:
{ yyval = NULL; }
break;
case 224:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 225:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 226:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 227:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 229:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 230:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 231:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 232:
{ yyval = yyvsp[0]; }
break;
case 233:
{ yyval = NULL ;}
break;
case 238:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 240:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 241:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 242:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 243:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 244:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 245:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 246:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 247:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 248:
{ yyval = NULL;}
break;
case 249:
{ yyval = yyvsp[0];}
break;
case 250:
{ yyval = yyvsp[0];}
break;
case 251:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 252:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 253:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 254:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 255:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 256:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 257:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 258:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 259:
{ yyval = make_list (yyvsp[-1]); }
break;
case 260:
{ yyval = NULL; }
break;
case 261:
{ yyval = make_list (yyvsp[-1]); }
break;
case 262:
{ yyval = NULL; }
break;
case 264:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 265:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 267:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 268:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 269:
{ yyval = yyvsp[0]; }
break;
case 270:
{ yyval = yyvsp[0]; }
break;
case 271:
{ yyval = (dsql_nod*) NULL; }
break;
case 272:
{ yyval = make_list (yyvsp[0]); }
break;
case 273:
{ yyval = NULL; }
break;
case 275:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 276:
{ yyval = yyvsp[-1]; }
break;
case 279:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 280:
{ yyval = NULL; }
break;
case 281:
{ yyval = NULL; }
break;
case 282:
{ yyval = yyvsp[0]; }
break;
case 283:
{ yyval = yyvsp[0]; }
break;
case 284:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 287:
{ yyval = yyvsp[-1]; }
break;
case 288:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 289:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 290:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 292:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 308:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 309:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 314:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 315:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 316:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 317:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 319:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 321:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 322:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 323:
{ yyval = NULL; }
break;
case 324:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
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
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 337:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 338:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 341:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 342:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 343:
{ yyval = NULL; }
break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 356:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 357:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 358:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 359:
{ yyval = NULL; }
break;
case 360:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 361:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 362:
{ yyval = make_list (yyvsp[-1]); }
break;
case 363:
{ yyval = NULL; }
break;
case 365:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 366:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 367:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 368:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 369:
{ lex.beginning = lex_position(); }
break;
case 370:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 371:
{ lex.beginning = lex.last_token; }
break;
case 372:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 373:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 374:
{ yyval = 0; }
break;
case 375:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 376:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 377:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 379:
{ yyval = NULL; }
break;
case 380:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 381:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 399:
{ yyval = NULL; }
break;
case 400:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 401:
{ yyval = yyvsp[0]; }
break;
case 402:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 403:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 404:
{ yyval = yyvsp[0]; }
break;
case 405:
{ yyval = yyvsp[0]; }
break;
case 406:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 407:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 408:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 412:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 413:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 417:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 422:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 427:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 428:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 429:
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
				}
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
case 885:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 886:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 887:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 890:
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
case 891:
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
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 899:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 900:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 901:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 902:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 904:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 905:
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
case 906:
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
case 907:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
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
{ yyval = make_node (nod_current_database, 0, NULL); }
break;
case 913:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 914:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 915:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 916:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 917:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 918:
{ yyval = yyvsp[0]; }
break;
case 919:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 921:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 922:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 923:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 924:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 925:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 927:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 928:
{ yyval = yyvsp[0];}
break;
case 933:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 934:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 935:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 936:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 937:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 938:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 939:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 940:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 942:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 943:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 944:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 946:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 949:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 950:
{ yyval = yyvsp[0]; }
break;
case 951:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 952:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 953:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 954:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 957:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 958:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 959:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 962:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 963:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 964:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 965:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 966:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 967:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 968:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 969:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 973:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 974:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 975:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 976:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 977:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 978:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 979:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 980:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 985:
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
