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
  182,  182,  181,  185,  185,  169,  187,  187,  187,  188,
  188,  186,  186,  186,  186,  186,  186,  186,  186,  186,
  186,  186,  186,  186,  186,  186,  186,  186,  186,  186,
  191,  191,  192,  193,  194,  195,  200,  201,  196,  196,
  197,  209,  198,  210,  204,  204,  204,  205,  205,  205,
  207,  207,  207,  207,  199,  206,  206,  202,  202,  202,
  208,  208,  189,  189,  215,  216,  216,  217,  217,  217,
  217,  203,  203,  203,  219,  221,  220,  222,   11,   12,
  223,  223,  224,  224,  225,   75,   84,  227,  230,  230,
  230,  231,  235,  239,  239,  240,  240,  241,  241,  141,
  229,   91,   93,  228,  228,   74,   87,  244,  244,  244,
  245,  248,  248,  249,  249,  249,  249,  249,  249,  249,
  249,  249,  249,  249,  249,  249,  249,  249,  246,  246,
  247,    2,  251,  251,  251,  251,  251,  251,  251,  259,
  260,  257,  257,  261,  261,  261,  261,  261,  261,  261,
  252,  252,  262,  262,  262,  262,  262,  262,  262,  256,
  267,  267,  267,  267,  267,  267,  267,  267,  267,  267,
  267,  267,  267,  267,  267,  267,  267,  267,  267,  267,
  267,  267,  267,  267,  267,  267,  267,  264,  264,  266,
  266,  265,  263,  263,  263,  258,  258,  254,  255,  255,
  269,  269,  269,  269,  269,  253,  270,  270,  271,  271,
    8,  272,  272,  272,  272,  272,  272,  272,  272,  272,
  272,  272,  272,  105,  105,  138,  138,  275,  275,  278,
  278,  280,  280,   54,   54,  277,  277,  277,  277,  277,
  277,  277,  277,  277,  285,  285,  276,  276,  276,  276,
  287,  287,  286,  286,  286,   56,   56,  282,  282,  282,
  279,  279,  279,  291,  291,  291,  290,  290,  289,  289,
  289,  283,  283,  292,  292,  292,  293,  293,  284,  284,
  284,  284,  294,  294,   19,   19,   19,  296,  296,  296,
   17,   17,   17,  298,  299,  302,  302,  300,  304,  304,
    4,   16,  303,  303,  305,  305,  306,  306,  295,  307,
  307,  308,  308,  309,  309,  309,  309,  310,  310,  311,
  311,  312,  312,  314,  314,  314,  315,  315,  315,  316,
  316,  316,  313,  318,  318,  318,  319,  319,  317,  317,
  320,  322,  322,  321,  321,  297,   18,  323,  323,  323,
  324,  324,  329,  329,  330,   69,   69,   69,  332,  332,
  331,  331,  325,  325,  325,  326,  326,  335,  335,  327,
  327,  328,  184,  339,  340,  333,  334,  336,  336,  336,
  336,  341,  341,  341,  342,  342,  342,  233,  233,  234,
  234,  344,  344,  345,  345,  346,  346,  337,  348,  348,
  349,  349,  349,  352,  353,  353,  354,  354,  356,  356,
  350,  350,  351,  351,  357,  357,  242,  242,   32,  243,
  243,  243,  243,  243,  243,  243,  243,  237,  237,  358,
  358,  359,  238,  238,  236,  236,  338,  338,  360,  361,
  361,  361,  361,  361,  362,  362,  363,  363,  364,  364,
  365,  365,  365,  366,  366,  367,  367,   10,   10,    7,
    7,  369,  370,  371,   20,   20,  372,  373,  374,  374,
  190,    3,    3,  376,  376,  376,  378,  378,  286,  377,
  377,  379,  379,   35,   35,   89,   88,   88,  368,  368,
  380,  381,  381,  213,  213,  213,  140,  375,  375,  115,
  115,  115,  115,  115,  385,  385,  385,  385,  385,  385,
  384,  384,  384,  387,  387,  387,  387,  387,  387,  387,
  387,  387,  387,  388,  388,  388,  388,  388,  388,  388,
  388,  393,  393,  393,  393,  393,  393,  393,  393,  393,
  393,  393,  393,  393,  393,  393,  393,  399,  399,  389,
  389,  390,  390,  390,  390,  391,  391,  395,  395,  396,
  396,  396,  396,  394,  397,  392,  392,  383,  383,  383,
  386,  386,  386,  400,  400,  401,  398,  402,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,   92,   92,   92,   92,   92,
   92,   92,   92,   92,   92,  150,  150,  150,  403,  212,
  212,  145,  145,  409,  409,  409,  409,  409,  405,  405,
  405,  405,  405,  226,  146,  146,  147,  148,  148,  148,
  148,  148,   52,   52,   21,   21,  250,  410,   55,  288,
  281,  281,   99,  404,  404,  404,  404,  411,  411,  411,
  411,  411,  411,  411,  411,  411,  411,  411,  412,  413,
  415,  414,  414,  417,  418,  418,  408,  408,  406,  407,
  407,  419,  419,  419,  420,  420,  421,  421,  424,  424,
  422,  422,  427,  427,  426,  423,  425,  416,  416,  416,
  416,  416,  416,  416,  416,  343,  343,  232,  232,  149,
   49,   65,  124,  142,  382,  159,  183,  274,   67,   63,
  218,  116,   70,  347,  214,   36,   42,  355,  273,   40,
   43,  211,   41,  301,  268,  268,  428,  428,  428,  428,
  428,  428,  428,  428,  428,  428,  428,  428,  428,  428,
  428,  428,  428,  428,  428,
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
    3,    0,    1,    3,    3,    7,    7,    1,    1,    4,
    4,    7,    2,    1,    3,    1,    1,    6,    3,    0,
    0,    0,    0,    3,    0,    9,    9,    1,    1,    0,
    2,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    3,    5,    5,    5,    5,    5,    5,    2,    0,
    4,    2,    3,    3,    2,    2,    3,    3,    2,    3,
    6,    1,    2,    4,    3,    2,    2,    2,    2,    3,
    1,    3,    3,    3,    2,    2,    4,    4,    5,    1,
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
    2,    3,    0,    1,    3,    4,    5,    1,    4,    4,
    3,    0,    1,    3,    3,    1,    1,    0,    1,    1,
    4,    0,    2,    4,    0,    3,    0,    2,    0,    2,
    0,    9,   11,    0,    0,    0,    0,    3,    2,    1,
    0,    3,    5,    3,    2,    5,    2,    1,    1,    1,
    1,    1,    3,    1,    3,    1,    0,    2,    1,    3,
    1,    1,    1,    6,    1,    0,    3,    0,    1,    3,
    6,    3,    4,    2,    3,    0,    1,    2,    1,    1,
    1,    2,    1,    2,    1,    2,    0,    3,    0,    1,
    3,    1,    2,    0,    2,    0,    2,    0,    4,    1,
    2,    1,    1,    0,    1,    3,    2,    1,    1,    2,
    1,    4,    3,    1,    3,    4,    0,    8,    5,    1,
    1,    4,    4,    4,    1,    1,    5,    5,    1,    3,
    3,    7,    7,    5,    3,    0,    1,    1,    1,    2,
    0,    1,    1,    1,    0,    3,    1,    3,    1,    0,
    3,    1,    3,    1,    3,    3,    1,    1,    3,    1,
    2,    1,    3,    3,    1,    2,    3,    3,    3,    3,
    1,    3,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    3,    3,    3,    3,    3,    3,    3,
    3,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    1,    1,    5,
    6,    3,    4,    5,    6,    3,    4,    3,    4,    3,
    4,    4,    5,    4,    4,    3,    4,    1,    1,    1,
    1,    1,    1,    1,    3,    3,   11,   11,    1,    1,
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
    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
  565,  566,  567,  571,  572,  573,    0,  628,  740,  741,
  745,  746,  478,    0,    0,    0,    0,    0,    0,  412,
  583,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  122,    0,    0,  103,    0,  917,    0,   22,  915,
    0,    0,   74,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  491,    0,    0,    0,
   38,   35,   37,    0,   36,    0, 1005, 1007, 1009, 1008,
 1014, 1010, 1017, 1015, 1011, 1013, 1016, 1012, 1018, 1019,
 1020, 1022, 1023, 1021, 1024, 1025,    0,    0,    0,    0,
   33,   69,   67,  997, 1006,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  660,    0,
    0,    0,  697,    0,  999,    0,    0,    0,    0,  115,
 1004,  574,    0,    2,    0,    0,    0,    0,  442,  443,
  444,  445,  446,  447,  448,  449,  450,  451,  452,  453,
  454,  455,  456,  457,  458,  459,  460,  461,  462,  463,
  464,  465,  466,  467,    0,  440,  441,    0,  989,    0,
  419,  993,    0,  416,  996,    0,  699,    0,  415, 1000,
    0,  581,  913,    0,  158,  111,    0,  112,    0,  208,
  985,  777,    0,  110,  153,  992,    0,    0,  106,    0,
  113,  919,    0,  107,    0,  108,    0,  109, 1003,  154,
  114,  636,  637,    0,  918,  916,    0,   75,    0,  990,
    0,  988,  499,  492,    0,  498,  503,  493,  494,  501,
  495,  496,  497,  502,    0,    0,    0,   31,    0,   40,
  764,   39,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  923,    0,  904,
  654,  654,  655,  665,  667,  976,  668,    0,  669,  659,
  655,    0,    0,    0,    0,    0,    0,    0,  600,  589,
    0,  592,  594,  595,  596,  597,  603,  604,    0,  998,
  698,    0,  116,    0,  117,    0,  118,    0,    0,    0,
  978,    0,    0,    0,    0,    0,    0,    0,    0,  479,
    0,    0,  382,    0,  211,    0,  422,  413,  476,  477,
    0,    0,    0,    0,    0,    0,  431,    0,  388,  389,
    0,  587,  585,  914,    0,    0,    0,    0,    0,    0,
    0,  161,  140,    0,  104,    0,    0,  119,    0,  129,
  128,    0,    0,    0,    0,    0,    0,  981,   76,    0,
    0,    0,  742,  743,  500,    0,    0,    0,    0,  881,
  910,    0,  980,    0,  883,    0,    0,  911,    0,  905,
  896,  895,  894,    0,    0,    0,    0,  886,  887,  888,
  897,  898,  906,  907,    0,  908,  909,    0,    0,    0,
  912,    0,    0,    0,    0,    0,    0,  900,    0,  774,
  878,  879,  880,  885,  884,  359,  864,    0,    0,  863,
    0,    0,  860,  861,  862,  865,  866,  867,  899,  924,
  925,  926,  927,  940,  942,  950,  951,  955,  956,  209,
    0,    0,    0,  363,    0,    0,    0,  767,    0,    0,
   28,    0,    0,    0,   72,   70, 1001,   68,   34,    0,
    0,    0,  769,    0,   53,    0,    0,    0,    0,    0,
  579,    0,    0,    0,  662,  664,    0,  671,    0,    0,
    0,  672,  658,    0,  626,    0,  601,    0,  598,    0,
  599,  624,    0,  619,    0,    0,  593,  778,  749,    0,
    0,    0,    0,    0,    0,  576,  575,    0,    0,  633,
  630,  629,    0,    0,    0,  176,    0,    0,  137,    0,
  484,    0,  485,  480,    0,    0,  426,  428,  427,    0,
  429,    0,  423,    0,    0,  262,    0,    0,    0,  435,
  436,    0,  469,    0,    0,    0,    0,    0,    0,    0,
  393,  392,  487,    0,    0,  157,    0,    0,  166,    0,
  165,    0,    0,  172,  155,    0,  170,  162,    0,    0,
  522,  557,  525,    0,  558,    0,    0,  526,    0,    0,
    0,  549,  561,  521,  544,  523,  524,  519,  506,  382,
  504,  505,  507,    0,    0,  516,  517,  518,  520,    0,
    0,    0,    0,    0,  120,    0,  121,    0,  132,    0,
    0,  192,    0,    0,  380,    0,  211,    0,    0,   84,
    0,  102,    0,  101,  982,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  780,  782,  791,  794,  795,
  796,  797,  798,  799,  800,  801,  802,  803,    0,    0,
    0,  901,    0,    0,    0,    0,    0,    0,  902,  903,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  324, 1002,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  267,  514,    0,    0,
    0,  361,    0,    0,    0,  766,   29,    0,    0,   45,
   73,    0,    0,   27,    0,  772,    0,    0,    0,  739,
    0,    0,    0,    0,    0,  578,    0,  654,    0,  676,
    0,    0,    0,    0,  569,    0,  921,  568,    0,  602,
    0,  610,  606,  605,    0,    0,    0,  621,  609,    0,
    0,  747,  748,    0,    0,    0,  380,    0,    0,    0,
    0,    0,  627,    0,  138,    0,  483,  382,  425,    0,
  382,  383,  430,    0,    0,  258,  380,  231,  986,    0,
    0,  382,    0,    0,  200,  380,    0,    0,    0,    0,
  236,  230,  233,  234,  235,  434,  474,  473,  433,  432,
    0,    0,    0,    0,    0,    0,  391,    0,  382,    0,
  163,  167,    0,    0,  171,    0,    0,  759,    0,  545,
  546,  562,    0,  559,    0,    0,  552,  550,  551,    0,
    0,    0,    0,  515,    0,    0,    0,    0,  553,    0,
    0,  380,    0,  191,    0,  194,  196,  197,    0,    0,
    0,    0,    0,  211,   79,    0,    0,   78,    0,    0,
    0,    0,  781,  793,    0,    0,  851,  852,  853,    0,
  785,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  968,  969,  970,  971,  972,  973,
  974,  975,    0,    0,    0,    0,    0,    0,  961,    0,
    0,    0,    0,    0,  876,  326,  877,  948,    0,  872,
  984,    0,    0,    0,    0,    0,    0,    0,  882,  776,
  775,  365,  364,    0,  265,    0,    0,    0,  273,  768,
    0,    0,    0,    0,    0,    0,    0,    0,   54,   62,
   64,    0,    0,   71,    0,    0,    0,  771,    0,    0,
    0,    0,    0,    0,    0,    0,  663,  666,  994,  675,
    0,    0,    0,    0,  681,  682,  683,    0,    0,  673,
  570,  922,  611,  620,  615,  614,    0,  625,  779,  750,
    0,  380,    0,    0,  656,  635,  634,    0,    0,  646,
  650,  482,    0,    0,  175,    0,  179,    0,    0,    0,
  424,  380,  380,  270,  383,  263,    0,  202,  382,  204,
    0,  383,  199,  205,    0,    0,    0,    0,  437,  438,
  382,  380,  383,    0,    0,    0,  409,    0,  187,  188,
  164,    0,  174,  533,  534,  920,    0,    0,    0,    0,
    0,  560,    0,    0,  382,  383,    0,  510,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  189,    0,    0,    0,    0,  369,    0,   83,    0,
    0,    0,    0,   91,   85,    0,    0,   90,   86,    0,
  744,  987,    0,    0,  786,    0,    0,  792,    0,    0,
    0,    0,    0,  829,  828,    0,    0,    0,  836,  854,
  846,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  784,    0,    0,    0,  211,    0,  928,    0,    0,
  943,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  957,    0,    0,    0,    0,    0,    0,
    0,    0,  947,  889,    0,  260,  279,    0,    0,  360,
  274,   66,   58,   59,   65,   60,   61,    0,    0,   23,
    0,    0,    0,   43,    0,    0,    0,  753,  773,    0,
    0,  752,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  700,    0,    0,    0,    0,    0,  617,
  618,  622,    0,  190,    0,    0,    0,    0,    0,    0,
  180,    0,    0,    0,  420,  214,  215,  216,  217,  218,
  219,  892,    0,    0,  264,    0,    0,  212,    0,    0,
    0,    0,  237,    0,  470,  471,  439,    0,    0,    0,
    0,    0,    0,  382,  489,  383,  173,  983,    0,    0,
  528,    0,  527,  563,    0,  555,    0,    0,    0,  508,
    0,  536,    0,    0,  538,  541,  543,  380,    0,    0,
    0,  127,    0,  195,  382,    0,    0,  381,    0,  105,
    0,  125,    0,   94,    0,    0,    0,    0,   88,   89,
    0,  844,  845,  788,    0,  790,    0,    0,    0,    0,
    0,    0,    0,  847,    0,    0,    0,    0,    0,    0,
    0,  837,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  934,  933,    0,    0,
  930,  929,    0,  938,  937,  936,  935,  932,  931,    0,
    0,  963,    0,    0,    0,  962,    0,    0,    0,    0,
    0,  266,    0,    0,  276,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  358,
    0,    0,    0,    0,  285,  290,  284,    0,    0,    0,
  294,  295,  296,  297,  298,  299,  301,  302,  304,  305,
  306,  309,    0,    0,    0,  352,  353,  354,    0,    0,
   57,   63,   55,   56,   24,  758,  757,    0,  755,  763,
  762,  760,    0,  738,    0,    0,    0,  692,    0,    0,
    0,  706,  702,  704,    0,    0,    0,    0,    0,  381,
  639,  640,  657,    0,  185,  181,    0,  383,  893,  268,
  269,  383,    0,    0,    0,  222,    0,    0,  238,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  486,  530,
    0,  531,    0,  142,    0,    0,    0,  146,  148,  149,
  150,  511,  513,  509,  540,    0,    0,  383,    0,    0,
    0,    0,  366,    0,    0,  382,   92,    0,    0,   98,
   93,    0,    0,    0,    0,    0,    0,  855,  856,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  210,  949,  939,
  941,    0,    0,  959,  958,    0,  964,  952,  954,    0,
    0,    0,  275,    0,  313,    0,    0,    0,  308,    0,
    0,  307,  338,  339,    0,  995,  355,  356,    0,  293,
  300,    0,  303,  286,    0,  291,    0,  343,  292,  310,
    0,    0,  336,   41,    0,    0,    0,  695,  693,    0,
    0,    0,  710,    0,    0,  652,  383,  367,  641,  182,
  383,  152,  257,    0,    0,  198,  223,    0,    0,    0,
    0,  224,  227,    0,    0,  383,  407,  408,  404,  403,
  406,  405,    0,  529,  556,  151,  139,  147,    0,  382,
  254,  134,  383,    0,    0,  384,  371,  370,    0,    0,
   96,    0,   80,    0,  815,  823,    0,  816,  824,  819,
  827,    0,    0,  817,  825,  818,  826,  812,  820,  813,
  821,  814,  822,    0,  944,  960,    0,    0,    0,  282,
  278,    0,  311,    0,    0,    0,    0,    0,  340,    0,
    0,  331,  332,    0,  351,    0,    0,    0,    0,  346,
  344,    0,    0,    0,  754,    0,    0,  685,    0,    0,
  720,  722,    0,  717,    0,  255,  421,  201,  206,  225,
    0,    0,  228,    0,  240,  242,  411,  383,    0,  386,
    0,    0,    0,  376,  377,    0,    0,   77,   99,  100,
    0,  953,    0,  281,    0,  312,    0,    0,  315,    0,
  321,    0,  323,    0,    0,  350,  349,  991,  348,    0,
    0,    0,    0,    0,    0,    0,  684,    0,  711,  721,
    0,  229,    0,    0,    0,    0,    0,  256,  383,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  334,  333,  345,  347,    0,    0,    0,
    0,  689,    0,    0,  728,    0,    0,    0,    0,    0,
    0,  239,    0,  246,    0,  245,  387,  379,    0,    0,
  372,  126,    0,    0,  283,    0,    0,  314,  318,    0,
    0,    0,    0,    0,    0,    0,  687,  730,  719,    0,
    0,  731,    0,  727,  226,    0,    0,  250,  249,  248,
    0,    0,    0,    0,    0,    0,    0,    0,  335,    0,
  690,  726,    0,    0,  253,  251,  252,    0,    0,  858,
  329,  319,    0,  341,  316,  653,    0,    0,    0,  733,
    0,    0,  317,    0,  732,    0,    0,  735,    0,  857,
  736,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1421,   28,   29, 1422,
   31,   32,   33,   34,   35,   36,   37, 1423,   39, 1424,
  838,  127,  480,  977, 1220,  128,  729,  978,  129,  484,
  734,  153,  130,  131,  270, 1002,  143,  979,  980,  208,
  237,  132,  981,  133,  486,   83,  248,  389,  437,  648,
 1112,  438,  877,  619,  230,  854,  649,  650,  651, 1119,
 1114, 1541,  249,  653,  654,   75,  198,   76,  244, 1897,
 1330,  229,  234,  236,  238,  224,  216,  218,  231,  241,
  160,  323,  325,  327,  378,  635,  637,  477,  271,  801,
  556,  664, 1041,  382,  640, 1100, 1322,  587,  757, 1071,
  548,  549,  569,  374,  620, 1086, 1516, 1636, 1265, 1517,
 1518, 1519, 1520, 1521,  665,  225,  217,  370,  595,  371,
  372,  596,  597, 1297,  550, 1035, 1036, 1037, 1260,  384,
  865,  866,  867,  868,  803, 1052, 1494,  621,  805,  440,
  870,  950,  471, 1380, 1266,  441,  442,  443,  444,  445,
 1495, 1496,  572, 1642, 1795, 1283,  811,  812,  798,  813,
  814,  815, 1796, 1797, 1869,  352,  476,  967, 1425,  204,
  565,  964,  566, 1045,  472,  968,  969, 1208, 1404, 1405,
 1406, 1701, 1121,  740, 1426, 1427, 1428, 1429, 1607, 1430,
 1431, 1432, 1433, 1434, 1435, 1436, 1437, 1438, 1439, 1440,
 1441, 1442, 1443,  446, 1810, 1444, 1711, 1853, 1708,  447,
  705,  448,  449, 1445, 1608, 1719, 1720, 1777, 1446, 1447,
 1448, 1599,  267,  473,  474,  450, 1105, 1328, 1533, 1106,
 1107,  332,  298,  510, 1665,  393, 1249, 1478, 1752, 1753,
 1754, 1755, 1247,  361,  583,  829, 1295,  584,  827,   80,
   60,  356,  209,  168,  339,  195,  346,  201,  792,  789,
  347,  357,  819,  358,  579, 1063,  196,  451,  340,  585,
 1296,   97,  207,  253,  622,  623,  718, 1087,  719, 1088,
 1089,  626,  627,  628,  629, 1457, 1080, 1078,  630,  631,
  632,  847,  633,  844,   41,   42,   43,   44,   45,   46,
  162,  537,   62,  502,  212,  363,  310,  311,  312,  313,
  314,  315,  316,  317,  318,  763,  523, 1017, 1252,  524,
  525,  768,   47,  167,  334,  545,  783,   48,  539,  540,
 1026, 1483, 1256, 1629, 1030,  147,  753, 1626,  505,  300,
  148,  149,  299,  511,  512,  751, 1000, 1003, 1004, 1005,
 1006, 1007, 1727, 1787,  452, 1823, 1241, 1622, 1623, 1825,
 1735, 1826, 1827, 1828, 1864, 1898, 1900,  492,   49,   50,
  394,   51,   52,  531,  532,  986, 1228, 1458, 1462,  493,
  737,  222,  666,  667,  892,  893,  668,  669,  670,  671,
  672,  673,  674,  675,  676,  677,  678, 1353, 1137, 1139,
 1140,  704,  453,  454,  455,  456,  457,  458,  459,  246,
  460,  461,  462,  463,  464,  933,  465, 1573,  466,  467,
  468,  469,  694,  937, 1197, 1192,  695,  135,
};
static short yysindex[] = {                                   5411,
 1571, -194, 3947,   70, 1586,  -14, 4944,  335, 9075,  624,
  142, 8254, -194, 1493,  429, 9906, 1449, 9906,  110,    0,
  623,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  492,    0,    0,    0,
    0,    0,    0,14025, 9906, 9906, 9906, 9906, 9906,    0,
    0,  302, 1531, 9906, 9906, 9906,  484, 9906,  387, 9906,
 9906,    0, 9906, 9906,    0,  792,    0,  468,    0,    0,
  487, 9906,    0, 9906, 9906, 9906,  606, 9906, 9906, 9906,
 9906,  387, 9906, 9906, 9906, 9906,    0, 9906,  939,  578,
    0,    0,    0,  987,    0,  987,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  755,  798,  230, 1158,
    0,    0,    0,    0,    0, 9906, 9906, 9906,  902,  934,
  965,  132, 1033,  796,  128,  170,   74,  867,    0, 9906,
 1086, 1390,    0, 1012,    0, 9906, 9906, 9906, 9906,    0,
    0,    0, 9906,    0, 1153,  117,  918, 1600,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  560,    0,    0, 1531,    0,  714,
    0,    0, 1397,    0,    0, 1118,    0,  730,    0,    0,
 1110,    0,    0, 1092,    0,    0,  843,    0, 1323,    0,
    0,    0, 1531,    0,    0,    0, 1378, 1397,    0,  561,
    0,    0, 1300,    0, 1305,    0,  987,    0,    0,    0,
    0,    0,    0, 1332,    0,    0, 1243,    0, 1379,    0,
 1287,    0,    0,    0, 1243,    0,    0,    0,    0,    0,
    0,    0,    0,    0,13030, 9906, 1394,    0, 9906,    0,
    0,    0,10376, 1418, 9272, 9906, 1212, 1485, 1786, 1518,
 1523,10376, 1460, 9272, 1495, 1515, 1365,    0,13262,    0,
    0,    0,    0,    0,    0,    0,    0,11269,    0,    0,
    0, 1473, 9906, 1578, 1480,  880, 9906, 1517,    0,    0,
 1390,    0,    0,    0,    0,    0,    0,    0, 9906,    0,
    0, 1397,    0, 1300,    0,  987,    0, 1574,13262, 1549,
    0, 1549,13262, 1650,  -63, 1466, 1529, 1483, 1600,    0,
 1740, 1308,    0, 9906,    0,  560,    0,    0,    0,    0,
 9906, 1394, 4458, 1581, 5088, 2010,    0,14025,    0,    0,
 1340,    0,    0,    0, 2000, 2000, 1531, 1690, 1531,  592,
  843,    0,    0, 9455,    0, 9906, 9906,    0, 1394,    0,
    0, 1785,  903, 2031, 9906, 1809, 9906,    0,    0, 2039,
 3079, 8121,    0,    0,    0, 2051, 2054, 2055, 1648,    0,
    0, 2057,    0, 2063,    0, 2064, 2069,    0, 2072,    0,
    0,    0,    0,    0, 2073, 1661, 1663,    0,    0,    0,
    0,    0,    0,    0, 2082,    0,    0,11376, 2085, 2092,
    0, 2098,13262,13262,11608, 9906, 2099,    0, 2603,    0,
    0,    0,    0,    0,    0,    0,    0, 2104, 2066,    0,
    0, 2113,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 9455, 2094, 1540,    0, 2120, 1900, 1798,    0, 9906, 1737,
    0, 9906, 9906,  145,    0,    0,    0,    0,    0, 9906,
 9906, 1359,    0, 9906,    0, 1850, 9906, 2124,10376, 1774,
    0, 9906,11608, 1930,    0,    0,13262,    0, 2153, 1854,
 2129,    0,    0,   54,    0, 1020,    0,  413,    0,  413,
    0,    0, 2130,    0,  165, 1767,    0,    0,    0, 2131,
  119, 2127, 1394, 2152, 1926,    0,    0, 1616, 2149,    0,
    0,    0, 1161, 1763, 1753,    0, 1858, 1860,    0, 1531,
    0, 1862,    0,    0, 1932, 1940,    0,    0,    0, 1922,
    0, 9455,    0, 9455, 1808,    0, 1957, 9906,13906,    0,
    0,  876,    0, 9906,  -47, 1118, 1832, 1795, 1768,    0,
    0,    0,    0,  431, 1844,    0, 1787,  387,    0, 1531,
    0, 1961, 1897,    0,    0,  592,    0,    0,  162, 1801,
    0,    0,    0, 1803,    0, 1851, 2205,    0, 1901, 2207,
  500,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2157,  151,    0,    0,    0,    0,  135,
 2213, 2221, 2207, 1397,    0, 1935,    0, 1993,    0, 1531,
 1531,    0, 4458,  730,    0, 1887,    0, 1864, 2224,    0,
 8212,    0, 1891,    0,    0, 1899, 2236, 8359, 2237,    0,
    0,    0, 7877, 2856,  654,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1105,13262,
  426,    0, 9906,13262, 1270, 1417, 1427, 2442,    0,    0,
13262, 8598, 2603, 1835,  597,13262,13262, 8598,  837,  837,
 1493, 1951, 1867, 2238,    0,    0,11862, 9906,13262,13262,
13262,13262,13262,13262,13262, 2999,    0,    0, 2008, 2223,
 9906,    0, 9906, 1991, 9906,    0,    0,11748, 1861,    0,
    0, 1834, 9272,    0, 1977,    0, 1894, 1493, 2253,    0,
 1977,11748, 1978, 1982, 9906,    0, 1951,    0, 2102,    0,
 9906, 2358, 1857,13262,    0, 1433,    0,    0,  121,    0,
 1865,    0,    0,    0, 9906, 1273, 9906,    0,    0, 9906,
 9906,    0,    0,13262, 2043, 4458,    0, 1817,13262,13262,
 1937, 1816,    0, 1531,    0,  720,    0,    0,    0, 2276,
    0,    0,    0,  158, 9906,    0,    0,    0,    0, 2045,
 2279,    0, 2023, 2042,    0,    0, 1985, 1986,  987, 2056,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  387, 9906,13967, 1955, 1956, 1963,    0,  387,    0, 1621,
    0,    0, 1928, 1531,    0, 3079,  100,    0, 1936,    0,
    0,    0,  387,    0, 2205,   97,    0,    0,    0, 2048,
   97, 1938,   97,    0, 2296,  387,  387,  387,    0, 1394,
 9906,    0, 1998,    0, 1915,    0,    0,    0, 1340, 1944,
 9906, 1939,   76,    0,    0, 2314,  594,    0, 3079, 9906,
 1946, 8836,    0,    0, 1946, 8359,    0,    0,    0, 8169,
    0,  190,  695,13262,13262, 8943, 2320, 1125, 9201,13262,
 9459, 1077, 9566, 9824,12094,10082,10189,10447, 8598, 8598,
13262,13262, 3414,13262, 2321,13262, 2322, 2217,13262,13262,
13262,13262,13262,13262,    0,    0,    0,    0,    0,    0,
    0,    0, 2047, 1402,  -57,13262,  831,13262,    0, 8598,
 2138, 3064,  422,   74,    0,    0,    0,    0, 1945,    0,
    0, 2088,  837,  837,  959,  959, 2603, 1142,    0,    0,
    0,    0,    0, 1958,    0, 1942, 2100, 1991,    0,    0,
 9906, 9906, 9906, 9906, 9906, 9906,  159, 2324,    0,    0,
    0,11748, 1997,    0,  -48, 2013, 9906,    0,   74,13262,
 2013, 2333, 2324,11748, 5785, 2061,    0,    0,    0,    0,
 1626, 2341, 2339, 1139,    0,    0,    0, 8598, 2060,    0,
    0,    0,    0,    0,    0,    0,  -67,    0,    0,    0,
 2603,    0, 1959, 1944,    0,    0,    0, 2603, 9906,    0,
    0,    0, 2000, 2121,    0,  720,    0, 2350, 8598, 4830,
    0,    0,    0,    0,    0,    0, 1991,    0,    0,    0,
 4830,    0,    0,    0,  987,  987, 1903, 2352,    0,    0,
    0,    0,    0,   -5,  359, 1001,    0, 2143,    0,    0,
    0, 9906,    0,    0,    0,    0,   70, 1976, 1979, 2008,
 2368,    0, 1787, 1981,    0,    0, 1389,    0, 2354, 9906,
 1444,  387, 2373, 2375, 2380, 2160,  730, 1991, 2000, 1860,
 4458,    0, 1844,   74, 1983, 1999,    0, 2481,    0,  387,
 2038, 2122, 8212,    0,    0,  387,  723,    0,    0, 2128,
    0,    0, 2386, 2392,    0, 8598, 8598,    0, 8598, 8598,
 2109, 2603, 2396,    0,    0, 2603, 2398,12339,    0,    0,
    0, 2093, 2400, 2603, 2402, 1741, 2404, 2603, 2405,13262,
13262, 2320,13262,12446, 2406, 2603, 2410, 2412, 2603, 2414,
13262, 2603, 2415, 2603, 2417, 2423, 2603, 2425, 2433, 2603,
 2434,    0, 2215, 2286, 2316,    0, 2359,    0, 2450,13262,
    0, 2468, 2514, 2532, 2724, 2988, 3149,13262,13262,13262,
 2603, 2052,13262,    0,13262, 2603, 2173,  789,13262,13262,
13262,13262,    0,    0, 9906,    0,    0, 9906,13785,    0,
    0,    0,    0,    0,    0,    0,    0, 2172,11748,    0,
11748,  159, 2324,    0,  287,  287,  205,    0,    0,11269,
 1990,    0, 2333, 2324, 2456, 5785, 2465, 1139, 2466,13262,
 2244, 2358, 2133,    0, 2135, 2136, 2179, 2239, 2192,    0,
    0,    0, 1991,    0, 1983, 1268, 2476, 1787, 2139, 1787,
    0, 8598,  245, 1436,    0,    0,    0,    0,    0,    0,
    0,    0, 4830, 4830,    0, 2100,13262,    0, 2240, 2123,
 1903,  792,    0, 8598,    0,    0,    0, 2147, 2148, 2151,
 2159, 2163, 2164,    0,    0,    0,    0,    0, 2488,   70,
    0, 2081,    0,    0,   97,    0, 4830, 2167,   97,    0,
   97,    0, 2008, 2503,    0,    0,    0,    0, 1340, 2100,
 1787,    0, 1860,    0,    0,11269, 2269,    0, 1469,    0,
 2476,    0, 2507,    0, 2509, 1531,  -56, 2510,    0,    0,
 1531,    0,    0,    0, 2215,    0, 2215,13262, 2145, 2145,
 1493, 1995, 2511,    0, 2145, 2145,13262, 2145, 2145, 2549,
 2603,    0, 1782,13262, 2603, 2145, 2145, 2145, 2145, 2603,
 2145, 2145, 2145, 2145, 2145, 2145,    0,    0, 9455, 2517,
    0,    0, 3158,    0,    0,    0,    0,    0,    0, 3165,
 1625,    0,13262, 2243, 2140,    0,13262, 3173, 1996, 3367,
 1559,    0, 9455, 2506,    0,    0, 2272,    0, 3679,  -91,
 2512, 2529, 2246,13262, 2513, 2524, 3979, 9906, 9906,    0,
 2525, 2526, 2251, 2536,    0,    0,    0, 2271, 9699, 2541,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2543,  220, 2530,    0,    0,    0,    0, 2225,
    0,    0,    0,    0,    0,    0,    0, 2176,    0,    0,
    0,    0, 1854,    0, 5785, 2456, 2244,    0, 2002, 9906,
 1139,    0,    0,    0, 2358,13262, 8598, 2220, 2100,    0,
    0,    0,    0, 1621,    0,    0,  465,    0,    0,    0,
    0,    0, 2603, 2329, 2240,    0, 1183, 9906,    0, 2281,
  631, 2182, 2283, 2184, 2323, 2285, 2326, 1991,    0,    0,
 2579,    0, 2581,    0, 2282, 2329, 2167,    0,    0,    0,
    0,    0,    0,    0,    0, 1991, 1844,    0, 1621, 2143,
 2310, 2254,    0, 1944, 1944,    0,    0, 2266,   -6,    0,
    0, 2008, 2273, 2603, 2593, 2599,   74,    0,    0, 2602,
 2610, 2603, 2611, 2612,13262,13262, 2603, 2616, 2619, 2622,
 2623, 2624, 2625, 2626, 2632, 2634, 2635,    0,    0,    0,
    0,13262, 2641,    0,    0,13262,    0,    0,    0,13262,
 1857,  186,    0, 2381,    0,12678, 9906,13262,    0, 8598,
 2603,    0,    0,    0, 2631,    0,    0,    0, 9906,    0,
    0, 3379,    0,    0,  691,    0, 2252,    0,    0,    0,
  367, 2643,    0,    0,  287, 1857, 9906,    0,    0,  932,
 2603, 2655,    0,  654,  -25,    0,    0,    0,    0,    0,
    0,    0,    0, 2659, 9906,    0,    0, 2353, 2363, 9906,
 1903,    0,    0,  987, 9906,    0,    0,    0,    0,    0,
    0,    0, 2100,    0,    0,    0,    0,    0, 2100,    0,
    0,    0,    0, 2658, 1857,    0,    0,    0,13262, 1531,
    0, 2248,    0, 1531,    0,    0,13262,    0,    0,    0,
    0, 2603, 2603,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2603,    0,    0, 3324, 2060, 4830,    0,
    0, 2664,    0, 3474,13030, 1474,  638, 2650,    0, 2374,
 1035,    0,    0, 2665,    0, 9906, 9906,   70,  157,    0,
    0, 2212, 2379, 8598,    0, 2060, 2676,    0, 8598,13262,
    0,    0, 2355,    0, 2678,    0,    0,    0,    0,    0,
 1903,  987,    0, 2343,    0,    0,    0,    0, 2143,    0,
 2658, 2677, 1139,    0,    0, 2060, 2603,    0,    0,    0,
 1559,    0, 2192,    0, 1946,    0, 2337, 3379,    0, 2295,
    0, 3379,    0, 3379, 2787,    0,    0,    0,    0,13785,
  691,13262, 3379,  759, 2192, 9906,    0,  654,    0,    0,
 7769,    0, 2343,  382, 1903, 2348, 2365,    0,    0, 1139,
 2685, 2658, 2407, 2192, 2708, 1857, 2220, 2712, 2177, 2697,
 1443,13785, 2713,    0,    0,    0,    0, 1482,  228, 2458,
 2220,    0, 2006, 9906,    0, 2722, 2729,  944, 1903,  958,
  958,    0, 2477,    0, 2342,    0,    0,    0, 1139, 2658,
    0,    0, 2060, 2397,    0, 3379, 2713,    0,    0, 2474,
 3379, 2491, 2482,13785, 2397, 9906,    0,    0,    0, 7769,
 2752,    0, 9906,    0,    0, 2338, 1054,    0,    0,    0,
 1076, 2192,  918, 2011,13785,  164, 9906,13785,    0,  918,
    0,    0, 9906, 2461,    0,    0,    0, 8598, 2220,    0,
    0,    0,13785,    0,    0,    0, 2751, 2758, 2761,    0,
  654, 2397,    0, 9906,    0, 9906,  918,    0, 2765,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  131, 1176,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  173,10685,    0,    0,    0,    0,    0,    0,
 2808,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  124,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  356,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2479,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   19, 2444,
    0,    0,    0,  263,    0,  263,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2448,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  947,    0,    0,12923,10792,    0,    0,
    0, 1156,    0,    0,    0, 4374,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2409,  819,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   37,    0,    0,    0,    0,  236,    0,    0,
 1185,    0,    0,    0,    0,    0,  204,    0, 9820,    0,
    0,    0,    0,    0,    0,    0,    0,   37,    0,  515,
    0,    0, 2785,    0,    0,    0, 2563,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1237,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1334,    0, 2564,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1487,    0,
    0,    0,    0,    0,    0,    0,10959,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1080,    0,    0,
 1404,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   37,    0, 2785,    0, 2563,    0, 1425,    0,    0,
    0,    0,    0,  133,    0,    0,    0,    0, 1472,    0,
 2554,    0,    0,    0,    0, 1512,    0,    0,    0,    0,
    0, 2564,  -20,14080,    0, 1534,    0,    0,    0,    0,
  212,    0,    0,    0, 2383, 2383,    0,    0,    0, 1539,
  341,    0,    0,    0,    0,    0,    0,    0, 2564,    0,
    0, 1632,    0,    0,    0,    0,    0,    0,    0, 5489,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1967,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2743,    0,    0,    0, 3043, 3343,
    0, 3643,    0,    0,    0,    0,    0,    0,  420,    0,
    0,    0,    0,    0,    0,    0,    0,  357, 3943,    0,
 2443,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1569,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1580,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3558,    0,
 2518,    0,    0,    0,    0,    0,    0,  254,    0,  254,
    0,    0, 1248,    0, 1333, 1440,    0,    0,    0,    0,
 1237,    0, 2564,    0,    0,    0,    0,  181,  179,    0,
    0,    0,  833,    0,  771,    0,    0,  875,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2554,    0,    0, 1535,    0, 2376,    0,    0,  781,
    0,    0,    0,    0,  764,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1614,    0,    0, 3334, 4726,
    0,    0,    0,10352,    0,    0, 6126,    0,    0,13760,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2033, 3034,    0,    0,    0,    0,13772,
11889,    0,13760,   37,    0,    0,    0,    0,    0,    0,
    0,    0,  -20, 1619,    0,    0,    0,    0, 2438,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 6530,
 6696, 6831,    0,    0,  839,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,13507,    0,
13507,    0,    0,    0,13507,13507,13507,    0,    0,    0,
    0,    0, 2394,    0,    0,    0,    0,    0, 5132, 5421,
11030, 2799,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2150,    0,
    0,    0,    0, 2574,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  305,    0,    0,10685,    0,    0,
  305,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  915,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1014,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -20,    0,   96,    0,    0,
  306,    0,    0,    0,    0,  101,    0,    0,    0,    0,
    0,    0,    0, 2025,    0,    0,    0,    0,    0, 2806,
    0,    0, 2256, 3878,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  773,  824,  848,    0,    0,    0,  835,
    0,    0,    0,    0,    0,    0,    0,    0, 3934,    0,
    0,    0,    0,    0, 6126,    0,    0,    0,    0,  949,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2564,
    0,    0,  669,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 8594,    0,    0,    0,  155,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,13507,    0,    0,    0,    0,    0,    0,
    0, 4243, 5710, 5999, 4543, 4843,  994,    0,    0,    0,
    0,    0,    0,    0,    0,11482,    0, 2577,    0,    0,
    0,    0,    0,    0,    0,    0, 1617, 1623,    0,    0,
    0,    0,    0,    0,    0, 1633,    0,    0,12923,    0,
 1633, 1634, 1635,    0,    0,    0,    0,    0,    0,    0,
    0, 6206,  886,  218,    0,    0,    0,    0,  279,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  185,    0,    0,    0,    0,    0,    0, 1030,    0,    0,
    0,    0, 2383, 2413,    0,  138,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2574,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1479,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1638,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 4234,
    0,    0,    0,    0,    0,    0,    0,    0, 1504,    0,
    0,    0,    0,    0,    0,    0, 1619, 2574, 2383, 1644,
  -20,    0, 2590,12923, 1686,   94,    0,    0,    0,    0,
 8594,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  572,    0,    0,    0, 1726,    0,    0,    0,    0,
    0,    0,    0, 1757,    0, 2210,    0, 2308,    0,    0,
    0,    0,    0,    0,    0, 6920,    0,    0, 6953,    0,
    0, 6995,    0, 7078,    0,    0, 7120,    0,    0, 7153,
    0,    0,  937,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2429,    0,    0,    0,    0,  877,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  366,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1617, 1696,    0, 2437,  362,    0,    0,    0,    0,
    0,    0, 1699, 1713, 1743,    0,    0, 2537, 1246,    0,
 1059,    0, 2538,    0, 2539, 2540,    0,    0,  187,    0,
    0,    0, 2574,    0, 1686,    0,  836,    0, 2418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1424,    0,
 1479, 2479,    0,    0,    0,    0,    0,  882,  982, 1039,
 1111, 1124, 1159,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  917,    0,    0,
    0,    0, 4382,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1760,    0,    0,    0,    0,    0, 2409,    0,
 1789,    0,    0,    0,    0,    0,  256,    0,    0,    0,
    0,    0,    0,    0,  174,    0,  243,    0,    0,    0,
11030,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 7242,    0, 7324,    0, 7444,    0,    0,    0,    0, 7472,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2816,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8885,    0,    0,
    0,    0,    0,    0,    0, 1831, 1848,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  847,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1896,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1794, 6281,    0,    0,    0,
  676,    0,    0,    0,    0,    0,    0,  573,    0,    0,
    0,    0,    0,  846,    0,    0,    0,    0,    0,    0,
    0,    0, 2828, 1566, 2004,    0, 2554,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2574,    0,    0,
    0,    0,    0,    0,    0, 1820,  983,    0,    0,    0,
    0,    0,    0,    0,    0, 2574, 2590,    0,  690,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  988,    0, 7548,    0,    0,13507,    0,    0,    0,
    0, 7576,    0,    0,    0,    0, 7652,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  534, 2822,    0,    0,    0,    0,    0,    0,    0,    0,
 2823,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2578,    0,    0,    0,
    0,    0,    0,    0,  362,  123,  604,    0,    0, 2537,
  241, 1023,    0,  869, 2848,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 4649,    0,    0,  211,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  316,    0,    0,    0,    0,    0,
    0,  258,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 7680, 7763,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2849,    0,    0,    0,  541,    0,    0,
    0,    0,    0,    0,   84,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  477, 5053,    0,    0,    0,
    0,    0, 2120,    0,    0,    0,    0,    0,    0,    0,
 4649, 1281,    0,  328,    0,    0,    0,    0,    0,    0,
    0,  788, 1046,    0,    0,  698, 2828,    0,    0,    0,
    0,    0,   73,    0,    0,    0, 2833,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  393,
    0,    0,    0,    0,   67,    0,    0, 5342,    0,    0,
 2848,    0, 1319,    0, 1479, 1583, 1588,    0,    0, 2537,
 1371,    0,    0,  221,    0,  534,  104,    0,    0,    0,
    0,  393, 2835,    0,    0,    0,    0,    0, 2595,    0,
  136,    0,    0, 1282,    0,    0, 2859,    0, 4649,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1062,    0,
    0,    0,  541,   28,    0,    0, 2843,    0,    0,13394,
    0,    0,    0,  393,  139,    0,    0,    0,    0, 2848,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2537,   73, 2863,    0,  393,    0,    0,  393,    0, 1454,
    0,    0,    0, 2026,    0,    0,    0,    0,  104,    0,
    0,    0,  393,    0,    0,    0, 2873,    0,    0,    0,
 2566,   28,    0,    0,    0,    0, 2863,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2915,    0,    0, 2919,
    0,    0,    0,    0,    0,    0,    0,    1,    0, 2921,
    6,  276, -148, -558, 1700,  685, -311,   13, 2911, 2642,
    0,   20,    0, 2651, -102,  -40,    0, -153, -976,  -34,
  -71,  -30, -172, 2653, 2197,    0,    0,    0, 1260,    0,
    0,  -52, 1818, -542,  -61, -914, 2292, 2058, -322,    0,
 1837,    0, 2853, 2070, 2114,    0,  -60,    0, -499,  -54,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -263, -334, 1845,
 -280, 1025, -893,    0,    0,    0,    0, -331, -133,  146,
 1855, -348,  -50,    0, -539,    0,    0, 1438, -949,    0,
 1447,    0,    0, 2620, -601,  -37,    0,    0,    0,    0,
 2589,    0, 2370, 1877,    0,    0,    0, 1933,    0, 2644,
 2194, 1876, 2627, 2633,    0, 1406,    0, -444, 2175,  -32,
 -571, 1352, -240,    0,    0, -921, -919, -889, -884, -882,
    0, 1496, -835,    0, 1199,-1211, 1497,    0, 2419,    0,
    0,    0, 1202, 1204, 1171,   61, -257, -820, -913,    0,
    0,    0, 2208,    0, -681,    0, 2036,    0,    0,    0,
    0,    0,-1168, -852, -728,    0,    0,    0,    0, -227,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1304,    0,    0,  -18,    0,    0,  144,
    0, -397,  315, 1594, 1408,    0, 1232,    0,    0,    0,
    0,    0,    0,    0, 2297, -130, 1992, 1762, 1541,    0,
  565, 1693, -891, -965,    0, -523, -589, -458,    0, -649,
 1272, 1113, -927, -551, -798, -993,-1240,    0,    0,  -49,
    0,    0,    0,    0,    0, 2661,    0,    0,    0,    0,
 2680, 2451,    0,    0,    0,    0,    0,   -9, 2693,    0,
    0,    0,  -10,    0,    0,    0, -355, 2171, -353, 1727,
 -480,    0,    0,    0,    0, 2439,    0, -229,    0,    0,
    0, 2411,    0, 2195,    0,    0,    0,    0,    0,    0,
   95,    0, 3024,    0,    0,    0,    0,    0, 2728,    0,
    0,    0,    0, 2533,    0, 2534,    0,    0,    0, 2280,
    0,    0,    0, -428,  -73,    0,    0, 1620,    0, 2267,
    0,    0,    0,    0,    0, -633, -686, -519, -251, 1405,
    0, 2902, 1283,    0, 2298, -980, -716,    0, -894, 2062,
    0,    0,    0,    0, -131,    0,    0,    0, 1325, 1428,
    0, 1196,    0, 1233,    0,  126,    0,    0,    0,    0,
 2528,    0,    0,    0, -391, 2325, 2071,-1032,    0,    0,
    0, -698, -419,  275,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2435, 1830, 1913,
    0,    0,    0,    0, -879,    0,    0,    0, 1805,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  107, 1872,    0,    0,
};
#define YYTABLESIZE 14596
static short yytable[] = {                                     134,
   38,  200,  134,  272,  223,  156,  155,  772,  161,   79,
  215,  291,  294,  219,  292,  295,  203,  961,  624,  232,
  625,  594,  562,  263,  321,  254,  717,  228, 1123,  802,
  260,  220, 1124,  758,  588,  258,  235,  703,  778, 1407,
  506,  965,  232,  240,  197,  199,  202,  205,  155,  210,
  259,  257, 1202, 1210,  221,  199,  226,  265,  205,  262,
  155,  210,  560,  239,  134,  264,  714,  944,  632, 1499,
 1103, 1019,  250,  156,  155,  252,  199,  206,  250,  226,
  202,  205,  261,  155,  210,  239,  134,  326,  205,  233,
  935,  529,  869,  368,  567,  642,  943, 1230,  756,  736,
  177, 1278,  485,  278,  989,  280, 1238,  714,  878, 1325,
  564,  485,  302,  714,   78, 1111,  322,  793, 1267,  794,
 1268,  638,  716,  632,  804,  714,  221,  155,  221, 1267,
  584, 1268,  647,  496,  386,  718,  642,  178,  632,  642,
  226, 1083,  327, 1077,  718,  348,  320,  205,  155,  239,
 1269, 1275,  368,  161,  642, 1270,  279, 1271, 1279,  177,
 1272, 1269,  771,  716,  632, 1303, 1270,  289, 1271, 1287,
  375, 1272,  584,  647,  856,  276,  718,  324,  631,  632,
  638,  716,  632,  992,  751,  743,  714,  530,  733,  584,
  290,  647, 1308, 1459,  718,   87,  178,  632,   87,  785,
 1781,  837, 1219,  159,  909, 1024,   78, 1774,  767,  293,
  765,  488, 1326,  624,  787,  625, 1539,  679, 1043,  631,
  714,  638, 1044,  535,  638, 1047, 1276,  714,  751, 1009,
 1128,  584,  290,  470, 1054,  390,  478,  631,  883,  638,
  712,  853, 1452,  751, 1454,  714, 1699,  594,  515, 1597,
 1598,  765,   61,  612,  765,  232,  221,  328,  679,  221,
 1470,  679,  159,  155, 1463,  487,  134,  290, 1225,  765,
  488, 1774,  155,  276,  487,  775,  679, 1320,  709,  714,
  546,  712,  362,  789,  712, 1488,  528,  140,  379, 1663,
 1098,  810,  481,  202,  390,  232,   95,  155,   97,  712,
  259,  481,   84, 1587,  756,  649,  765, 1172, 1173,  949,
  731,  561,  612, 1731,  589,  716,  591,  958,  470,  709,
  220,  910,  575, 1490, 1491,  577,  522,  247, 1288, 1250,
  642,   78,  646,  296,  221,  634, 1732,  709, 1198,  850,
  160,  221,  636,  221, 1671,  221,  649, 1471,  580,  290,
  744, 1267, 1267, 1268, 1268,  586,  325, 1514,  177,  530,
 1531,  535, 1492,  756,  649, 1084,  205,  210,  247, 1190,
  177,  247,  232,  297,  716,  155,  330,  202, 1061, 1226,
 1251,  655,  533, 1269, 1269, 1267,  247, 1268, 1270, 1270,
 1271, 1271, 1733, 1272, 1272,  178,  652,  177, 1524,  160,
  518, 1540, 1509,  177,  644,  177, 1528,  178, 1588,  817,
  642,  818,  232,  138,  586,  325,  331, 1269,  285,  890,
  535,  362, 1270, 1222, 1271,  852,  706, 1272, 1289, 1743,
 1710,  642, 1479,  996,  178, 1233, 1235, 1263,  632,  259,
  178,  730,  178, 1497,  177,  547,  714,  716,  284,  716,
 1253, 1126,  714,  830, 1042,  714,  730, 1780,  528,  632,
  890,  714, 1110,  890, 1893, 1201, 1125,  915,  647,  155,
 1273, 1274,  205,  487, 1057,  488,  709, 1672,  890,  766,
  155,  178, 1051,  718,  155,  327, 1617,  205, 1452,  155,
 1286, 1852,  161,  631,  390,  638,  997,  786,  727,  390,
  159,  714,  716,  288,  390, 1631,  909, 1038, 1799,  735,
 1040,  716,  890,  741,  631,  718,  638,  709,  481,   77,
 1527, 1050,  714, 1402, 1344, 1345,  831, 1346, 1347, 1792,
  755,  584,  679, 1660, 1611,  709,  806,  832,  232,  368,
  632,  642,  679, 1020,  679, 1319,  288,  159, 1068, 1076,
 1113,  520,  787,  679,  564,  712,  707,   87,  799,  221,
  485,   95,  392,   97,  799, 1627,  714,  712, 1127,  632,
  878,  838,  718,  855,  716,  714,  712,  288,  647,  714,
 1620,  709, 1725, 1832,  836,  714,  765,  863,  864,  962,
  732,  612, 1231,  709, 1632, 1229,  746,  679, 1633, 1634,
  584,  488, 1096,  686, 1218,  709,  679,  163,  642,  177,
  220,   77,  838,  718,  709,  838,  718, 1865,  714,  288,
  712,  789, 1012,  910,  631,  390,  638, 1673,  751,  712,
  838,  718,  714,  221, 1661,  716,  632,  160,  765,  530,
  716,  649,  716,  686,  686,  917,  178,  686,  718,  679,
  612,  632,  714,  612, 1076, 1677, 1290,  275,  709, 1497,
 1487,  679,  686,  679, 1612,  756,  714,  709,  133,  612,
  289, 1646,  712,  226,  638,  680, 1722, 1466, 1770, 1830,
  337,  164, 1501,  909,  160,  296,  712, 1653,  470,  186,
  470,  631,  970,  638,  860,  612,  141,  709,  951,  714,
 1606, 1258,  211,  765,  730, 1659,  221,  337, 1894,  836,
  709,  221,  159,  221, 1581,  221,  680, 1547,  487,  680,
 1280, 1281,  535,  487,  709,  914,  909,  133,  824,   98,
  679, 1032,  487, 1736,  680,  205,   77, 1737,  528, 1747,
 1352,  999,  205,  220,  227, 1748, 1526,  716,  186, 1764,
  150,  649, 1746,  712,  993,  155,  709,  155,  325, 1059,
  221,  716,  470,  410,  825, 1257,  221, 1321, 1277, 1750,
  651,  232,  396, 1332,  848,   14, 1616, 1267, 1067, 1268,
 1285, 1073, 1500,  761,  522,  221, 1018,  373, 1113, 1060,
 1062,  709, 1291, 1081, 1093, 1094, 1095,  130, 1212, 1820,
  910, 1215, 1399,  709, 1307,  247,  232,  232,  232, 1269,
  337,  651,  221,  221, 1270, 1831, 1271,  341,  645, 1272,
  247,  890,  410,  394, 1513, 1803,  655,  286,  380,  651,
 1523,  396,  643,  838,  186,  648,   77,  337,  715, 1568,
  849, 1074, 1469,  910, 1331,  186,  373,  395,   99,  160,
  151,  155,  762, 1379, 1798,  530,  709,  152,  716,  645,
  716,  155,  342, 1805,  826,  709, 1117,  709,  713,  655,
 1122,  165, 1803,  643,  481, 1624,  648,  645,  712,  715,
 1097,  401,  394,  713,  652,  678,  838,  718,  592,  838,
 1108,  643,  909,  186,  648,  136,  838,  715,  838,  909,
  938,  939,  838, 1216,  186, 1837,  395,  838,  718,  713,
  838, 1803, 1808,  716,  716,  909,  144,  245,  686,  381,
  709,  686,  716,  166,  255,  838,  678,  713,  686,  709,
  686, 1213,  884,  481,  686,  546,  783,  891, 1214,  686,
  401, 1118,  686, 1803,  678, 1217,  582,  838,  143, 1012,
  838,  838,  718, 1715,  528,  716, 1129,  686, 1582,  137,
  838,  487,  205,  210,  487,  239,  134,  130,  130,  343,
  349,  268,  487,  716,  785,  144,  838,  783,  266,  686,
  783,  402,  145,  686,  487,  487,  359,  344, 1707,  709,
  680,  205,  686,  891, 1223,  783,  478, 1461,  838, 1716,
  680, 1237,  680,  838,  718,  582, 1234,  143,  686,  910,
 1717,  680,  133, 1508,  707,  838,  910,  838,  718,  221,
  909,  345,  708,  624,  709,  625,  269,  410,  537,  644,
 1314,  537,  910,  186,  891,  686,  396,  891,  398,  940,
  402,  145,  232,  350, 1530,  374,  716,  686, 1334,  686,
  909, 1816,  891,  709, 1338,  680,  242, 1698,  694,  360,
  232,  375, 1298,  708,  680, 1451,  232, 1453,  220, 1822,
  644, 1033, 1512, 1130, 1806,  478,  961,  530, 1774,  607,
 1298,  708, 1299, 1850,  838,  718,  891,  394,  644,  374,
  243,  221, 1726, 1773, 1456, 1456, 1460,  398,  221,  694,
  593, 1800,  694,  186,  374,  375,  651,  680, 1763, 1718,
  397,  395,  373,  708,  373,  186,  686,  694,  709,  680,
  375,  680, 1784,  400, 1484, 1879, 1486, 1788,  154,  709,
  273,  186,  481,  645, 1193, 1194, 1785,  910,  607, 1881,
 1034, 1756,  186,  709,  481,  401, 1892,  643,  186, 1895,
  186,  287, 1839,  715,  645,  591,  884, 1403,  399,  518,
  884,  337,  396,  715, 1903,  715, 1804,  910,  643,  397,
  985,  648,  470,  274,  715,  220,  528,  481,  186,  481,
  967,  967,  400,  713,  588,  714,  186, 1529,  680,  186,
 1871,  807,  382,  144,  365,  221,  251,  186, 1408, 1449,
  678,  277,  712,  710,  713,  711, 1339,  713,  441,  487,
  678,  487,  678,  394,  591, 1397,  810,  399,  715,  373,
  186,  678,  186,  287,  143,  143,  487,  715,  366,  716,
  367,  373,  205,  373, 1204,  708,  716,  395, 1340,  716,
  709,  716,  985,  588,  186,  402,  641,  613,  713, 1243,
  716,  783,  368, 1872,  783, 1669,  519,  713,  382,  145,
  537,  783, 1244,  783,  645,  678,  186,  783,  808,  186,
  715,  401,  783, 1195,  678,  783, 1861,  369,  643,  281,
  765,  648, 1843, 1538,  715, 1245, 1901, 1844, 1543,  439,
  783,  337,  100,  537,  716,  716, 1392,  530, 1292, 1394,
  713, 1855,  398,  716, 1807, 1511,  613, 1729,  809,  282,
  520, 1862,  783,  504,  713,  783,  783,  678,  247,  967,
  143,  765,  509, 1863,  765,  783, 1821,  521, 1866,  678,
  101,  678,  623,  327, 1293,  537, 1246,  708, 1619,  765,
  283,  783,  102,  186,  644, 1841,  716, 1150, 1586,  708,
 1886,  715,  146,  538,  186,  213,  214,  543,  708,  247,
  716, 1151,  247,  783,  296,  644,  103, 1867,  783, 1902,
  374,  402,  374,  694,  397,  353,  694,  247,  354, 1749,
  783,  713,  783,  694,  707,  694,  375,  400,  375,  694,
  537,  623,  327, 1243,  694,  891,  528,  694,  678,  199,
  707, 1887,  708,  590,  911, 1152, 1244, 1596, 1122, 1122,
  616,  708,  694, 1889, 1868, 1873,  759,  607,  303, 1449,
  355,  319,  399,  220,  577,  329, 1880,  716,  398, 1245,
  333,  104, 1309, 1153,  694,  308,  351,  708,  694,  608,
  123,  105,  709,  712,  710,  537,  711,  694,  713,  783,
  607, 1888,  693,  645,  708,  487, 1243,  699,  700,  702,
  320,  616,  590,  694,  220,  205,  106,  220,  708, 1244,
 1714,  417, 1141, 1907,  123,  644,  607,  374,  241,  607,
 1246, 1310,  220,  577, 1456, 1728, 1774, 1309,  155,  374,
  694,  374, 1245,  375,  645,  607, 1142, 1154,  608, 1574,
  397, 1849,  694, 1577,  694,  375,  390,  375,  123,  101,
  677,  418,  645,  400,  395,  712,  710, 1644,  711,  241,
  713,  607,  241,  712,  710,  362,  711,  747,  713,  296,
  417,  749, 1769,  414,  475,  708, 1313,  241,  168,  677,
  677, 1744,  677, 1246,  364,  103, 1705,  512,  399,  677,
  677,  677,  677,  677, 1638,  677,  765,  765,  677,  677,
  677,  677,  677,  681,  765,  207,  677,  677,   44,  919,
  418,  694,  677,  677,  677, 1639,  681,  205,  475,   52,
  722, 1640,  243,  721,  681,  613,  373,  244,  780, 1122,
 1745,  558,  414,  475,  247,  247,  512,  168,  581,  681,
  712,  710,  247,  711,  559,  713,  207,  320,  582,  207,
  104, 1723,  383,  169,  729, 1641,   42, 1758,  613,  385,
  105, 1760,   25,  243,  207,  951,  243,   44,  244, 1630,
  155,  244,  761,   48,   50,  202, 1714,  490,   52, 1793,
 1714,  243, 1714,  135,  613,  106,  244,  613,  530,  729,
  681, 1714,  765,  156,  155, 1776,  765,  712,  710, 1742,
  711,  729,  713,  613,  387, 1001,  712,  710, 1015,  711,
  623,  713,  169,  765, 1662,   42,  296, 1714,  708,  765,
  530,   25, 1696,  709, 1016,  385,  296,  890,  376,  613,
  247,  761,   48,   50,  388,   26,  490,  507,   49,  232,
  220,  376,  135,  623,  913,  513,  199, 1778,  918,  376,
  391,  247,   51,  765, 1714,  934,  921,  247, 1189, 1714,
  941,  942,  530, 1779,  376,  807,  923,  304, 1534,  623,
  392,  439,  623,  952,  953,  954,  955,  956,  957,  439,
  156,  155,   46,  530,  385, 1712,  530,  528,  623, 1811,
  708,  247, 1481, 1813,   26,  709,  808,   49,  708,  136,
  305,  530, 1482,  709, 1819,  221,  807,  738,  331,  807,
 1449,   51,  376,  765,  623,  376,  999,  608,  509,  528,
 1824,  155,  712,  710,  807,  711,  306,  713,  124,  307,
 1847,  156,  155,   47,  739,  232,  475,  808, 1021, 1890,
  808,   46, 1449,  538, 1028,  308, 1896,  377, 1884, 1768,
  608,  247,  482, 1824,  155,  808,  232, 1851,  136,  207,
  490,  528,  232,  712,  710,  491,  711, 1874,  713,  156,
  155,  309, 1876, 1910,  494,  708,  608,  495,  726,  608,
  709,  725,  528,  157, 1449,  528,  999,  124,  796, 1824,
  155,  795,   47,  202,  497,  608,  232,  335,  243,  243,
  528,   53,  501,  244,  244, 1449,  243, 1122, 1449,  336,
  499,  244,   54,  202,  158,  752, 1007,  390,  207,   55,
  242,  608,  288, 1449,  411,  412,  413,  390, 1007,  159,
  500, 1007,  708, 1012,  202,  770,  202,  709,   81,   82,
  514,  708,  337,   56,  338, 1012,  709,  946, 1012, 1011,
  714, 1712,  421,  422,  243, 1712, 1713, 1814, 1131, 1132,
 1136,  517,  770, 1144, 1146, 1148, 1712, 1156, 1159, 1162,
 1164, 1167, 1170,  516,  988, 1174, 1175,  987, 1177, 1572,
 1179,  998,  526, 1182, 1183, 1184, 1185, 1186, 1187,  541,
  536,  542, 1712,  995,  243, 1102,  985,   14, 1101,  244,
 1191,  912, 1196,  916,  544,   57, 1005,  920,  922,  924,
  748,  712,  710,  551,  711,  243,  713,  145,  146, 1109,
  244,  243,  874,  213,  214, 1203,  244,  807,  714, 1712,
  553,  945,  712,  710, 1712,  711,   58,  713, 1206, 1254,
   59, 1205, 1101,  221, 1069, 1070,  981, 1005, 1005, 1005,
 1005, 1005, 1005, 1005,  439,  243, 1301,  708,  808, 1300,
  244, 1306,  709,  555, 1305, 1005, 1005, 1005, 1005, 1908,
 1464, 1909,  514,  714,   14, 1548, 1579,  552,  714,  714,
  807,  573, 1618,  807,  221,  714, 1857,  221, 1357, 1856,
  807, 1891,  807,  576, 1774,  590,  807, 1005,  708, 1005,
  586,  807,  221,  709,  807,  380,  737,  639,  380,  737,
  643,  808,  645,  514,  808,  243,  514,  107,  647,  807,
  244,  808, 1713,  808,  131,  131, 1713,  808, 1815, 1556,
  679,  514,  808,  680,  681,  808,  683, 1713, 1667, 1668,
  682,  807,  684,  685,  807,  807,  108,  109,  686,  110,
  808,  687,  688,  689,  807,  690,  111,  112,  113,  114,
  115,  691,  116, 1713,  696,  117,  118,  119,  120,  121,
  807,  697,  808,  122,  123,  808,  808,  698,  707,  124,
  125,  126,  998,  712,  710,  808,  711,  714,  713,  537,
  712,  710,  807,  711,  720,  713,  715,  807,  716,  723,
 1713,  808,  439,  724,  728, 1713,  742,  733,  745,  807,
  752,  807,  754,  765, 1360, 1361,  770, 1363, 1365,  712,
  710, 1199,  711,  808,  713, 1370,  769,  774,  808,  777,
  537,  776,  779,  537,  712,  710,  781,  711,  782,  713,
  808,  784,  808,  546, 1383,  787,  708,  788,  537,  832,
  537,  709, 1390, 1391, 1196,  790, 1846, 1196,  791, 1191,
  797,  821,  822, 1398,  439, 1400, 1401,  708, 1005,  823,
 1005, 1005,  709,  828,  436,  833,  288, 1005,  807,  840,
  834,  841,  842, 1005,  843,  845,  846,  851, 1005,  861,
  832, 1005,  857,  832,  509,  213,  862, 1181,  712,  710,
  858,  711,  871,  713,  439, 1005,  873,  874,  832,  808,
 1005, 1005,  879,  880, 1005,  881,  885,  936,  947,  232,
  221, 1005,  852, 1005, 1005,  290,  966, 1005,  982,  983,
  985, 1005,  990, 1005,  994, 1005,  213, 1005,  995,  213,
 1008, 1493, 1005, 1005, 1013, 1005, 1022,  811,  514,  514,
 1025, 1029, 1031,  514,  213, 1039,  514, 1048, 1049, 1051,
 1005, 1005,  800, 1005, 1055, 1056, 1377,  712,  710,  514,
  711, 1058,  713, 1064, 1065, 1092, 1005, 1072, 1005, 1005,
 1005, 1066, 1005, 1079, 1085, 1005, 1005, 1090,  811, 1099,
  509,  811, 1104, 1116,  738, 1005, 1378,  712,  710, 1138,
  711, 1178,  713, 1188,  708, 1180,  811, 1221, 1005, 1209,
 1348, 1005, 1544, 1227, 1224,  232, 1219, 1236,  708, 1207,
 1240, 1552, 1242,  709, 1248,  708, 1259, 1005, 1557, 1262,
  709, 1284, 1302, 1005, 1005, 1282,  232, 1001, 1005, 1381,
  712,  710,  232,  711,  514,  713, 1294,  537, 1304, 1005,
 1005, 1311, 1005, 1315,  708, 1316,  750, 1196, 1005,  709,
 1317, 1196,  537, 1318, 1110,  514, 1342, 1336, 1327,  708,
 1329,  514, 1343, 1341,  709, 1349,  232, 1350, 1591, 1355,
 1354, 1356,  985, 1358, 1359, 1366,  537, 1005, 1005, 1367,
 1005, 1368,  537, 1369, 1371,  537, 1372, 1005, 1005, 1005,
 1005, 1005, 1373, 1005, 1374,  514, 1005, 1005, 1005, 1005,
 1005,  832, 1375, 1376, 1005, 1005,  909, 1396, 1393, 1005,
 1005, 1005, 1005,  985,  985,  985,  985,  985,  998,  985,
 1382,  712,  710,  708,  711, 1450,  713,  537,  709, 1465,
 1621,  985,  985,  985,  985, 1467, 1468,  750, 1384,  712,
  710, 1476,  711, 1472,  713, 1473, 1474, 1475, 1477,  725,
  269, 1498, 1485,  568,  832, 1502, 1503,  832, 1510, 1504,
 1076,  213,  213,  985,  832,  985,  832, 1505, 1515,  213,
  832, 1506, 1507, 1525, 1532,  832, 1536, 1575,  832, 1537,
 1542, 1549,  537, 1351, 1385,  712,  710, 1569,  711,  537,
  713, 1584,  708,  832, 1583,  378, 1576,  709, 1590,  811,
 1589, 1592, 1386,  712,  710, 1604,  711,  537,  713, 1682,
 1683,  137, 1593, 1600, 1601,  832, 1602, 1613,  832,  832,
  712,  710,  708,  711, 1603,  713, 1694,  709,  832, 1609,
 1196, 1610, 1614, 1615, 1697, 1635,  378,  537, 1625,  378,
 1704,  537, 1706, 1645,  832, 1647, 1648, 1649, 1651, 1654,
 1650, 1655,  811, 1652,  378,  811, 1664,  213,  107, 1656,
 1670, 1666,  811, 1675,  811,  708,  832, 1674,  811, 1676,
  709,  832, 1678,  811,  712,  710,  811,  711,  213,  713,
 1679, 1680, 1681,  832,  213,  832, 1684,  108,  109, 1685,
  110,  811, 1686, 1687, 1688, 1689, 1690,  111,  112,  113,
  114,  115, 1691,  116, 1692, 1693,  117,  118,  119,  120,
  121, 1695, 1724,  811,  122,  123,  811,  811,  213, 1709,
  124,  125,  126, 1757, 1605, 1702,  811, 1751, 1730, 1738,
 1740, 1761, 1741, 1765,  985, 1759,  985,  985, 1771, 1772,
 1775, 1782,  811,  985, 1783, 1786, 1790, 1791, 1794,  985,
 1802, 1812,  832, 1833,  985, 1838,  708,  985, 1145,  439,
 1149,  709, 1157, 1160,  811, 1165, 1168, 1171, 1809,  811,
 1835,  985, 1009,  985,  708, 1840,  985,  985, 1842,  709,
  985,  811, 1845,  811, 1621, 1848, 1774,  985, 1854,  985,
  985,  800, 1859,  985, 1387,  712,  710,  985,  711,  985,
  713,  985, 1860,  985, 1830, 1831,  165, 1875,  985,  985,
 1877,  985, 1878, 1009, 1009, 1009, 1009, 1009, 1009, 1009,
  708, 1883, 1885, 1899, 1904,  709,  985,  985, 1905,  985,
 1906, 1009, 1009, 1009, 1009, 1911, 1818,    1,  708,  107,
 1555,  638,  985,  709,  985,  985,  985,  979,  985,   30,
  811,  985,  985,   32,  193,  708,  765,  261,  960,  382,
  709,  985,  156, 1009,  670, 1009,  966,  472,  108,  109,
   82,  110,  890,  272,  985,  203,  271,  985,  111,  112,
  113,  114,  115,  410,  116,  965,  946,  117,  118,  119,
  120,  121,  183,  985,  535,  122,  123,  184,  383,  985,
  985,  124,  125,  126,  985,  707,  705,  701,  703,  708,
  213,  322,  288,  378,  709,  985,  985,  724,  985,  945,
  378,  330,  378,  357,  985,  342,  378,  712,  710,  725,
  711,  328,  713,  645,  378,  925,  926,  927,  928,  929,
  930,  931,  932,  734,   27,  907,  906,  908,   30,  378,
   40, 1455,  142,  985,  985,  498,  985,  489,  488,  984,
 1337, 1115,  107,  985,  985,  985,  985,  985,  872,  985,
  256,  378,  985,  985,  985,  985,  985, 1335, 1120, 1075,
  985,  985, 1333, 1657, 1323,  985,  985,  985,  985,  598,
  557,  108,  109, 1658,  110,  835, 1312,  534, 1261, 1023,
  378,  111,  112,  113,  114,  115, 1324,  116, 1053,  570,
  117,  118,  119,  120,  121,  571, 1739, 1700,  122,  123,
 1637, 1829,  816, 1643,  124,  125,  126,  378, 1836, 1834,
  708, 1870, 1046, 1211, 1009,  709, 1009, 1009, 1767,  378,
 1595,  378, 1817, 1009, 1721, 1255, 1480,  963,  578, 1009,
 1628, 1535, 1801, 1091, 1009,  563,  820, 1009, 1388,  712,
  710,  554,  711,  537,  713, 1522,  144,  839,  527, 1082,
  960, 1009, 1014,  859, 1014, 1027, 1009, 1009,  760,  301,
 1009, 1010, 1734,  764, 1789, 1882, 1858, 1009,  773, 1009,
 1009, 1232, 1239, 1009, 1362,  991, 1395, 1009, 1489, 1009,
    0, 1009,    0, 1009,  537,    0,    0,  537, 1009, 1009,
    0, 1009,    0, 1014, 1014, 1014, 1014, 1014, 1014, 1014,
    0,    0,  537,    0,    0,    0, 1009, 1009,    0, 1009,
    0, 1014, 1014, 1014, 1014,  712,  710, 1200,  711,  107,
  713,    0, 1009,    0, 1009, 1009, 1009,    0, 1009,    0,
    0, 1009, 1009,   78,    0,    0,  894,    0,    0,    0,
    0, 1009,  708, 1014,    0, 1014,    0,  709,  108,  109,
  895,  110,    0,    0, 1009,    0,    0, 1009,  111,  112,
  113,  114,  115,    0,  116,    0,    0,  117,  118,  119,
  120,  121,    0, 1009,    0,  122,  123,    0,    0, 1009,
 1009,  124,  125,  126, 1009,    0,  896,    0,    0,    0,
    0,    0,    0,    0,  897, 1009, 1009,    0, 1009, 1389,
  712,  710,  898,  711, 1009,  713,    0,    0, 1570,  712,
  710,    0,  711,    0,  713, 1571,  712,  710,    0,  711,
  899,  713,  900, 1578,  712,  710,    0,  711,    0,  713,
    0,    0,    0, 1009, 1009,  901, 1009,  902,  903,  904,
    0,    0,    0, 1009, 1009, 1009, 1009, 1009,  107, 1009,
    0,    0, 1009, 1009, 1009, 1009, 1009,    0,    0,    0,
 1009, 1009,    0,    0,    0, 1009, 1009, 1009, 1009,    0,
    0,    0,    0,    0,  708,    0,    0,  108,  109,  709,
  110,    0,    0,    0,    0,    0,  905,  111,  112,  113,
  114,  115,    0,  116,    0,    0,  117,  118,  119,  120,
  121,  959,    0,    0,  122,  123,    0,    0,    0,    0,
  124,  125,  126,    0, 1014,    0, 1014, 1014,    0,  537,
  537,    0,    0, 1014,  537,    0,    0,  537,    0, 1014,
    0,    0,    0,    0, 1014,    0,    0, 1014,    0,    0,
  537,    0,    0,  535,    0,    0,    0,    0,    0,    0,
  708, 1014, 1010,    0,    0,  709, 1014, 1014,    0,    0,
 1014,    0,    0,    0,    0,    0,    0, 1014,    0, 1014,
 1014,    0,    0, 1014, 1762,  712,  710, 1014,  711, 1014,
  713, 1014,    0, 1014,  535,    0,    0,  535, 1014, 1014,
    0, 1014,    0, 1010, 1010, 1010, 1010, 1010, 1010, 1010,
    0,    0,  535,    0,  535,    0, 1014, 1014,    0, 1014,
    0, 1010, 1010, 1010, 1010,  537,    0,    0,  712,  710,
 1580,  711, 1014,  713, 1014, 1014, 1014,    0, 1014,    0,
    0, 1014, 1014,    0,    0,  708,  537,    0,    0,    0,
  709, 1014,  537, 1010,  708, 1010,  436,    0,    0,  709,
    0,  708,    0,    0, 1014,    0,  709, 1014,    0,  708,
  107,    0,    0,    0,  709,  712,  710,    0,  711,    0,
  713,    0,    0, 1014,    0,    0,  537,    0,    0, 1014,
 1014,    0,    0,    0, 1014,    0,    0,    0,    0,  108,
  109,    0,  110,    0,    0, 1014, 1014,    0, 1014,  111,
  112,  113,  114,  115, 1014,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,  125,  126,  712,  710,    0,  711,    0,
  713,    0,    0, 1014, 1014,    0, 1014,    0,   77,    0,
  107,    0, 1766, 1014, 1014, 1014, 1014, 1014,    0, 1014,
    0,    0, 1014, 1014, 1014, 1014, 1014,    0,    0,    0,
 1014, 1014,    0,    0,    0, 1014, 1014, 1014, 1014,  108,
  109,    0,  110,    0,    0,    0,    0,    0,    0,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,  535,  124,  125,  126,    0,    0,    0,    0,    0,
  708,  674,    0,    0, 1010,  709, 1010, 1010,  535,  535,
  535,    0,    0, 1010,  535,    0,    0,  535,    0, 1010,
    0,    0,    0,    0, 1010,    0,    0, 1010,    0,    0,
  535,    0,    0,    0,    0,    0,  535,    0,    0,    0,
    0, 1010, 1024,  708,    0,    0, 1010, 1010,  709,    0,
 1010,    0,    0,    0,    0,    0,    0, 1010,    0, 1010,
 1010,    0,    0, 1010,    0,    0,    0, 1010,    0, 1010,
    0, 1010,    0, 1010,    0,    0,    0, 1176, 1010, 1010,
    0, 1010,    0, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
  708,    0,    0,    0,    0,  709, 1010, 1010,    0, 1010,
    0, 1024, 1024, 1024, 1024,  535,    0,    0,    0,    0,
    0,    0, 1010,    0, 1010, 1010, 1010,    0, 1010,    0,
    0, 1010, 1010,    0,    0,    0,  535,    0,    0,    0,
    0, 1010,  535, 1024,    0, 1024,    0, 1585,    0,    0,
    0,  535,    0,  535, 1010,    0,    0, 1010,    0,    0,
  708,    0,    0,    0,    0,  709,    0,    0,    0,    0,
    0,  535,    0, 1010,    0,    0,  535,    0,    0, 1010,
 1010,    0,    0,    0, 1010,    0,    0,    0,    0,    0,
    0,    0,    0, 1545, 1546, 1010, 1010,    0, 1010, 1550,
 1551,    0, 1553, 1554, 1010,  535,    0,    0,    0,    0,
 1558, 1559, 1560, 1561,    0, 1562, 1563, 1564, 1565, 1566,
 1567,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1010, 1010,    0, 1010,    0,    0,    0,
  107,    0,    0, 1010, 1010, 1010, 1010, 1010,    0, 1010,
    0,    0, 1010, 1010, 1010, 1010, 1010,    0,    0,    0,
 1010, 1010,    0,    0,    0, 1010, 1010, 1010, 1010,  108,
  109,    0,  110,    0,    0,    0,    0,    0,    0,  111,
  112,  113,  114,  115,  674,  116,    0,  504,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,  125,  126,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1024,    0, 1024, 1024,    0,    0,
    0,    0,    0, 1024,    0,    0,    0,    0,  504, 1024,
    0,  504,    0,    0, 1024,    0,    0, 1024,    0,    0,
    0,    0,    0,  532,    0,    0,  504,    0,    0,    0,
    0, 1024,  859,    0,    0,    0, 1024, 1024,    0,    0,
 1024,    0,    0,    0,    0,    0,    0, 1024,    0, 1024,
 1024,    0,    0, 1024,    0,    0,    0, 1024,    0, 1024,
    0, 1024,    0, 1024,  532,    0,    0,  532, 1024, 1024,
    0, 1024,    0,  859,  859,  859,  859,  859,    0,  859,
    0,    0,  532,    0,  532,    0, 1024, 1024,    0, 1024,
    0,  859,  859,  859,  859,    0,    0,    0,    0,  677,
    0,    0, 1024,    0, 1024, 1024, 1024,    0, 1024,    0,
    0, 1024, 1024,    0,    0,    0,    0,    0,    0,    0,
    0, 1024,    0,    0,    0,  859,    0, 1594,  677,  677,
    0,  677,    0,    0, 1024,    0,    0, 1024,  677,  677,
  677,  677,  677,    0,  677,    0,    0,  677,  677,  677,
  677,  677,    0, 1024,    0,  677,  677,    0,    0, 1024,
 1024,  677,  677,  677, 1024,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1024, 1024,    0, 1024,    0,
    0,    0,    0,    0, 1024,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1024, 1024,    0, 1024,    0,    0,    0,
  107,    0,    0, 1024, 1024, 1024, 1024, 1024,    0, 1024,
    0,    0, 1024, 1024, 1024, 1024, 1024,    0,    0,    0,
 1024, 1024,    0,  504,  504, 1024, 1024, 1024, 1024,  108,
  109,  504,  110,    0,    0,    0,    0,    0,    0,  111,
  112,  113,  114,  115,  504,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,  532,  124,  125,  126,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  859,    0,  859,  859,  532,  532,
  532,    0,    0,  859,  532,    0,    0,  532,    0,  859,
    0,    0,    0,    0,  859,    0,    0,  859,    0,    0,
  532,    0,    0,  537,    0,    0,  532,   63,    0,    0,
    0,  859,  871,    0,    0,    0,  859,  859,   64,  504,
  859,    0,    0,    0,    0,   65,    0,  859,    0,  859,
  859,    0,    0,  859,    0,    0,    0,  859,   66,  859,
  504,  859,    0,  859,  537,    0,  504,  537,  859,  859,
    0,  859,    0,  871,  871,  871,  871,  871,    0,  871,
    0,    0,  537,    0,  537,    0,  859,  859,    0,  859,
    0,  871,  871,  871,  871,  532,    0,    0,    0,    0,
  504,    0,  859,    0,  859,  859,  859,    0,  859,    0,
    0,  859,  859,    0,    0,   67,  532,    0,    0,    0,
    0,  859,  532,    0,    0,  871,    0,    0,    0,    0,
    0,   68,    0,  532,  859,    0,    0,  859,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   69,    0,    0,
    0,  532,    0,  859,    0,    0,  532,    0,    0,  859,
  859,    0,   70,  699,  859,    0,   71,    0,    0,   72,
    0,  537,    0,    0,    0,  859,  859,   73,  859,    0,
    0,    0,    0,    0,  859,  532,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   74,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  699,    0,    0,
    0,    0,  537,  859,  859,  537,  859,    0,    0,    0,
  107,    0,  699,  859,  859,  859,  859,  859,    0,  859,
  537,    0,  859,  859,  859,  859,  859,    0,    0,    0,
  859,  859,    0,    0,    0,  859,  859,  859,  859,  108,
  109,    0,  110,    0,    0,    0,    0,    0,    0,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,  537,  124,  125,  126,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  871,    0,  871,  871,    0,  537,
  537,    0,    0,  871,  537,    0,    0,  537,    0,    0,
    0,    0,    0,    0,  871,    0,    0,  871,    0,    0,
  537,    0,    0,    0,    0,    0,  537,    0,    0,    0,
    0,  871,  874,    0,    0,    0,  871,  871,    0,    0,
  871,    0,    0,    0,    0,    0,    0,  871,    0,  871,
  871,    0,    0,  871,    0,    0,    0,  871,    0,  871,
    0,  871,    0,  871,    0,    0,    0,    0,  871,  871,
    0,  871,    0,  874,  874,  874,  874,  874,    0,  874,
    0,    0,    0,    0,    0,    0,  871,  871,    0,  871,
    0,  874,  874,  874,  874,  537,    0,    0,    0,    0,
    0,    0,  871,    0,  871,  871,  871,    0,  871,    0,
    0,  871,  871,    0,    0,    0,  537,    0,    0,    0,
    0,  871,  537,    0,    0,  874,    0,    0,    0,    0,
    0,    0,    0,  537,  871,    0,    0,  871,  241,    0,
    0,    0,    0,    0,    0,    0,    0,  537,  537,    0,
    0,  537,    0,  871,    0,  537,  537,    0,    0,  871,
  871,    0,    0,    0,  871,    0,    0,    0,  537,    0,
    0,    0,    0,    0,    0,  871,  871,    0,  871,  241,
    0,  699,  241,    0,  871,  537,    0,    0,  699,    0,
  699,    0,    0,    0,  699,    0,    0,  241,    0,    0,
    0,    0,  699,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  871,  871,  547,  871,  699,    0,    0,
    0,    0,    0,  871,  871,  871,  871,  871,    0,  871,
    0,  568,  871,  871,  871,  871,  871,    0,    0,  699,
  871,  871,    0,  537,    0,  871,  871,  871,  871,    0,
    0,    0,    0,    0,    0,  547,  547,    0,    0,  547,
    0,    0,    0,    0,  537,    0,    0,    0,  699,    0,
  537,    0,    0,  699,  547,    0,  547,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  874,  699,  874,  874,    0,    0,
    0,    0,    0,  874,  537,    0,  547,  699,    0,  699,
    0,    0,    0,    0,    0,    0,    0,  874,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  874,  875,    0,    0,    0,  874,  874,    0,    0,
  874,    0,    0,    0,    0,    0,    0,  874,    0,  874,
  874,    0,    0,  874,    0,    0,    0,  874,    0,  874,
    0,  874,    0,  874, 1264,    0,    0,    0,  874,  874,
    0,  874,    0,  875,  875,  875,  875,  875,    0,  875,
    0,    0,    0,    0,    0,    0,  874,  874,    0,  874,
    0,  875,  875,  875,  875,    0,    0,    0,    0,  107,
    0,    0,  874,    0,  874,  874,  874,    0,  874,    0,
    0,  874,  874,    0,  241,  241,    0,    0,    0,    0,
    0,  874,  241,    0,    0,  875,    0,    0,  108,  109,
    0,  110,    0,    0,  874,    0,    0,  874,  111,  112,
  113,  114,  115,    0,  116,    0,    0,  117,  118,  119,
  120,  121,    0,  874,    0,  122,  123,    0,    0,  874,
  874,  124,  125,  126,  874,    0,    0,    0,    0,    0,
    0,    0,    0,  547,    0,  874,  874,    0,  874,    0,
    0,    0,    0,    0,  874,    0,    0,    0,  547,    0,
  547,  547,  547,    0,    0,    0,  547,    0,    0,  547,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  241,    0,  547,  874,  874,    0,  874,    0,  547,    0,
    0,  547,    0,  874,  874,  874,  874,  874,    0,  874,
    0,  241,  874,  874,  874,  874,  874,  241,    0,    0,
  874,  874,  688,    0,    0,  874,  874,  874,  874,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  547,    0,    0,    0,    0,    0,    0,
    0,  241,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  688,    0,    0,  688,  547,    0,    0,
    0,    0,    0,    0,  875,    0,  875,  875,    0,    0,
    0,  688,    0,  875,    0,    0,    0,    0,  547,    0,
    0,  399,    0,    0,  547,    0,    0,  875,  547,    0,
    0,  869,    0,    0,    0,  547,    0,    0,    0,    0,
    0,  875,    0,    0,    0,    0,  875,  875,    0,  401,
  875,    0,    0,  547,    0,    0,    0,  875,  547,  875,
  875,    0,    0,  875,    0,    0,    0,  875,    0,  875,
    0,  875,  869,  875,  869,  869,  869,  403,  875,  875,
    0,  875,    0,  547,    0,    0,    0,  547,    0,    0,
  869,  869,  869,  869,    0,    0,  875,  875,    0,  875,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  875,    0,  875,  875,  875,    0,  875,    0,
    0,  875,  875,    0,  869,    0,    0,    0,    0,    0,
    0,  875,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  875,   85,    0,  875,  408,    0,
    0,    0,   86,    0,    0,    0,   87,   88,    0,    0,
    0,    0,    0,  875,  410,   89,    0,    0,    0,  875,
  875,    0,    0,    0,  875,    0,   90,    0,  411,  412,
  413,    0,  213,  214,    0,  875,  875,    0,  875,    0,
    0,    0,    0,    0,  875,    0,    0,    0,    0,    0,
    0,  416,  417,  418,  419,  420,  421,  422,  423,  424,
    0,    0,    0,    0,    0,    0,  426,  427,    0,    0,
    0,    0,    0,  875,  875,  431,  875,    0,    0,    0,
    0,    0,    0,  875,  875,  875,  875,  875,   91,  875,
    0,  691,  875,  875,  875,  875,  875,    0,    0,    0,
  875,  875,    0,    0,   92,  875,  875,  875,  875,    0,
    0,    0,    0,    0,    0,    0,    0,  688,    0,   93,
  688,  574,    0,   94,    0,    0,    0,  688,    0,  688,
    0,    0,  691,  688,   95,  691,    0,    0,  688,    0,
    0,  688,    0,  869,    0,  869,  869,    0,    0,    0,
  691,    0,  869,   96,    0,    0,  688,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  869,    0,    0,    0,
  868,    0,    0,    0,    0,    0,    0,    0,  688,    0,
  869,    0,  688,    0,    0,  869,  869,    0,    0,  869,
    0,  688,    0,    0,    0,    0,  869,    0,  869,  869,
    0,    0,  869,    0,    0,    0,  869,  688,  869,    0,
  869,  868,  869,  868,  868,  868,    0,  869,  869,    0,
  869,    0,    0,    0,    0,    0,    0,    0,    0,  868,
  868,  868,  868,    0,  688,  869,  869,    0,  869,    0,
    0,    0,    0,    0,    0,    0,  688,    0,  688,    0,
    0,  869,    0,  869,  869,  869,    0,  869,    0,    0,
  869,  869,    0,  868,    0,    0,    0,    0,    0,    0,
  869,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  869,    0,    0,  869,    0,    0,  107,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  869,    0,    0,    0,    0,    0,  869,  869,
    0,    0,    0,  869,    0,  688,    0,    0,  108,  109,
    0,  110,    0,    0,  869,  869,    0,  869,  111,  112,
  113,  114,  115,  869,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,    0,
    0,  124,  125,  126,    0,    0,    0,    0,    0,    0,
    0,    0,  869,  869,    0,  869,    0,    0,    0,    0,
    0,    0,  869,  869,  869,  869,  869,    0,  869,    0,
    0,  869,  869,  869,  869,  869,    0,    0,    0,  869,
  869,    0,    0,    0,  869,  869,  869,  869,    0,    0,
    0,    0,    0,    0,    0,    0,  691,    0,    0,  691,
    0,    0,    0,    0,    0,    0,  691,    0,  691,    0,
    0,    1,  691,    0,    0,    0,    0,  691,    0,    0,
  691,    0,  868,    0,  868,  868,    0,    0,    0,    2,
    0,  868,    0,    0,    0,  691,    0,    3,    0,    0,
    0,    0,    0,    0,    4,  868,    5,    0,    6,  870,
    0,    0,    0,    7,    0,    0,    0,  691,    0,  868,
    8,  691,    0,    0,  868,  868,    0,    0,  868,    0,
  691,    0,    0,    0,    9,  868,    0,  868,  868,    0,
    0,  868,    0,    0,   10,  868,  691,  868,    0,  868,
  870,  868,  870,  870,  870,    0,  868,  868,    0,  868,
  211,    0,    0,  211,    0,    0,    0,    0,  870,  870,
  870,  870,    0,  691,  868,  868,  211,  868,    0,    0,
  211,    0,    0,  211,    0,  691,    0,  691,    0,    0,
  868,    0,  868,  868,  868,    0,  868,    0,    0,  868,
  868,    0,  870,    0,    0,    0,    0,   11,    0,  868,
    0,    0,    0,    0,   12,    0,    0,   13,    0,   14,
   15,    0,  868,  211,    0,  868,    0,    0,    0,  211,
  211,  211,    0,  211,  211,  211,    0,  211,    0,    0,
    0,  868,    0,    0,   16,    0,    0,  868,  868,    0,
    0,    0,  868,    0,  691,  211,    0,  211,    0,    0,
    0,    0,    0,  868,  868,    0,  868,    0,    0,    0,
    0,    0,  868,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  211,    0,    0,    0,
    0,   81,    0,   17,    0,    0,    0,    0,    0,    0,
    0,  868,  868,  211,  868,    0,    0,    0,   18,   19,
    0,  868,  868,  868,  868,  868,    0,  868,    0,    0,
  868,  868,  868,  868,  868,  211,    0,    0,  868,  868,
    0,    0,    0,  868,  868,  868,  868,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  211,  211,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  870,    0,  870,  870,    0,    0,  211,    0,    0,
  870,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  870,    0,    0,    0,  873,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  870,    0,
    0,    0,    0,  870,  870,    0,    0,  870,    0,    0,
    0,    0,    0,    0,  870,    0,  870,  870,    0,    0,
  870,    0,    0,    0,  870,    0,  870,    0,  870,  873,
  870,  873,  873,  873,    0,  870,  870,    0,  870,    0,
    0,    0,    0,    0,    0,    0,    0,  873,  873,  873,
  873,    0,    0,  870,  870,    0,  870,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  870,
    0,  870,  870,  870,    0,  870,    0,    0,  870,  870,
    0,  873,    0,    0,    0,    0,    0,    0,  870,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  971,
    0,  870,    0,    0,  870,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  564,    0,    0,    0,    0,
  870,    0,    0,    0,    0,    0,  870,  870,    0,    0,
    0,  870,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  870,  870,    0,  870,    0,    0,    0,    0,
    0,  870,    0,    0,    0,    0,  564,    0,    0,  564,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  564,    0,  564,    0,    0,    0,
  870,  870,    0,  870,    0,    0,    0,    0,    0,    0,
  870,  870,  870,  870,  870,  696,  870,    0,    0,  870,
  870,  870,  870,  870,    0,    0,  564,  870,  870,  974,
    0,    0,  870,  870,  870,  870,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  107,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  696,    0,    0,  696,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  873,    0,  873,  873,  696,  108,  109,    0,  110,  873,
    0,    0,    0,    0,    0,  111,  112,  113,  114,  115,
  677,  116,    0,  873,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,  873,  124,  125,
  126,    0,  873,  873,    0,    0,  873,    0,    0,    0,
    0,    0,    0,  873,    0,  873,  873,    0,    0,  873,
  677,  677,    0,  873,  677,  873,    0,  873,    0,  873,
    0,    0,    0,    0,  873,  873,    0,  873,    0,  677,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  873,  873,    0,  873,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  873,    0,
  873,  873,  873,    0,  873,    0,    0,  873,  873,    0,
    0,    0,    0,  564,    0,    0,    0,  873,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  564,    0,
  873,  564,  564,  873,    0,    0,  564,    0,    0,  564,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  873,
    0,    0,  564,    0,    0,  873,  873,    0,  564,    0,
  873,  564,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  873,  873,    0,  873,    0,    0,    0,    0,    0,
  873,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  696,
    0,    0,    0,  564,    0,    0,    0,    0,    0,  873,
  873,    0,  873,    0,    0,    0,    0,    0,    0,  873,
  873,  873,  873,  873,    0,  873,    0,  564,  873,  873,
  873,  873,  873,    0,    0,    0,  873,  873,    0,    0,
    0,  873,  873,  873,  873,    0,    0,    0,  564,    0,
  696,    0,    0,  696,  564,    0,    0,    0,  564,  848,
  696,    0,  696,    0,    0,  564,  696,    0,    0,    0,
    0,  696,    0,    0,  696,    0,    0,    0,    0,    0,
    0,    0,    0,  564,    0,    0,    0,    0,  564,  696,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  848, 1018, 1018,  848, 1018, 1018, 1018,    0,    0,    0,
    0,  696,    0,  564,    0,  696,    0,  564,  848, 1018,
 1018, 1018,    0,    0,  696,  677,    0,    0,  677,    0,
    0,    0,    0,    0,    0,  677,    0,  677,    0,    0,
  696,  677,    0,    0,    0,    0,  677,    0,    0,  677,
 1018,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  677,    0,    0,  696,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  696,
    0,  696,    0,    0,    0,    0,  677,  696,    0,    0,
  677,    0,    0,    0,    0,    0,    0,    0,    0,  677,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  677,  696,  696,    0,  696,
    0,    0,    0,    0,    0,  849,  696,  696,  696,  696,
  696,    0,  696,    0,    0,  696,  696,  696,  696,  696,
    0,    0,  677,  696,  696,    0,    0,    0,  696,  696,
  696,  696,    0,    0,  677,    0,  677,    0,    0,    0,
    0,    0,  677,    0,    0,    0,  849, 1019, 1019,  849,
 1019, 1019, 1019,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  849, 1019, 1019, 1019,    0,    0,
    0,  677,  677,    0,  677,    0,    0,    0,    0,    0,
    0,  677,  677,  677,  677,  677,    0,  677,    0,    0,
  677,  677,  677,  677,  677,    0, 1019,    0,  677,  677,
    0,  848,    0,  677,  677,  677,  677,    0,    0,    0,
 1018,    0,    0,    0,    0,    0, 1018,    0,    0,    0,
    0, 1018,    0,    0, 1018,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  850,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  848,    0,    0,  848,    0,    0,
 1018,    0,    0,    0,  848,    0,  848,    0, 1018,    0,
  848,    0,    0,    0,    0,  848, 1018,    0,  848,    0,
    0,  850, 1020, 1020,  850, 1020, 1020, 1020,    0,    0,
    0,    0,    0,  848, 1018,    0, 1018,    0,    0,  850,
 1020, 1020, 1020,    0,    0,    0,    0,    0,    0, 1018,
    0, 1018, 1018, 1018,    0,  848,    0,    0,  848,  848,
    0,    0,    0,    0,    0,    0,    0,    0,  848,  809,
    0, 1020,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  848,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1018,    0,  810,    0,    0,    0,  848,  849,    0,    0,
  809,  848,    0,  809,    0,    0, 1019,    0,    0,    0,
    0,    0, 1019,  848,    0,  848,    0, 1019,  809,    0,
 1019,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  810,  840,    0,  810,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  849,  810,    0,  849,    0,    0, 1019,    0,    0,    0,
  849,    0,  849,    0, 1019,    0,  849,    0,    0,    0,
    0,  849, 1019,    0,  849,  840,    0,    0,  840,    0,
    0,    0,  848,    0,    0,    0,    0,    0,    0,  849,
 1019,    0, 1019,  840,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1019,    0, 1019, 1019, 1019,
    0,  849,    0,    0,  849,  849,    0,  804,    0,    0,
    0,    0,    0,    0,  849,    0,    0,    0,    0,    0,
    0,    0,  850,    0,    0,    0,    0,    0,    0,    0,
  849, 1020,    0,    0,    0,    0,    0, 1020,    0,    0,
    0,    0, 1020,    0,    0, 1020, 1019,    0,  804,  805,
    0,  804,  849,    0,    0,    0,    0,  849,    0,    0,
    0,    0,    0,    0,    0,    0,  804,    0,    0,  849,
    0,  849,    0,    0,    0,  850,    0,    0,  850,    0,
    0, 1020,  806,    0,    0,  850,    0,  850,    0, 1020,
  805,  850,    0,  805,    0,    0,  850, 1020,    0,  850,
    0,    0,    0,    0,    0,    0,    0,    0,  805,    0,
    0,  809,    0,    0,  850, 1020,    0, 1020,    0,    0,
    0,    0,    0,  806,    0,    0,  806,    0,    0,    0,
 1020,    0, 1020, 1020, 1020,    0,  850,    0,  849,  850,
  850,  806,    0,    0,  810,    0,    0,    0,    0,  850,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  809,  850,    0,  809,    0,    0,
    0,  839,    0,    0,  809,    0,  809,    0,    0,    0,
  809, 1020,    0,    0,    0,  809,  840,  850,  809,    0,
    0,    0,  850,    0,    0,    0,    0,  810,    0,    0,
  810,    0,    0,  809,  850,    0,  850,  810,    0,  810,
    0,    0,  839,  810,    0,  839,    0,    0,  810,    0,
    0,  810,    0,    0,    0,  809,    0,    0,  809,  809,
  839,    0,    0,    0,    0,    0,  810,    0,  809,  840,
    0,    0,  840,    0,    0,    0,    0,    0,    0,  840,
    0,  840,    0,  833,  809,  840,    0,    0,  810,    0,
  840,  810,  810,  840,    0,    0,    0,    0,    0,  804,
    0,  810,    0,  850,    0,    0,  809,    0,  840,    0,
    0,  809,    0,    0,    0,    0,    0,  810,    0,    0,
    0,    0,    0,  809,  833,  809,    0,  833,    0,    0,
  840,    0,    0,  840,  840,    0,    0,    0,    0,  810,
    0,  805,  833,  840,  810,    0,    0,    0,    0,    0,
    0,    0,  804,    0,    0,  804,  810,    0,  810,  840,
    0,    0,  804,    0,  804,    0,    0,    0,  804,    0,
    0,    0,    0,  804,  806,    0,  804,    0,    0,    0,
    0,  840,    0,    0,    0,    0,  840,    0,    0,    0,
    0,  804,  809,    0,  805,    0,    0,  805,  840,    0,
  840,    0,    0,  841,  805,    0,  805,    0,    0,    0,
  805,    0,    0,  804,    0,  805,  804,  804,  805,    0,
    0,    0,    0,    0,    0,  810,  804,  806,    0,    0,
  806,  842,    0,  805,    0,    0,    0,  806,    0,  806,
    0,    0,  804,  806,  841,    0,    0,  841,  806,    0,
    0,  806,    0,    0,    0,  805,    0,    0,  805,  805,
    0,    0,  841,  839,  804,    0,  806,  840,  805,  804,
    0,    0,  842,    0,    0,  842,    0,    0,    0,    0,
    0,  804,    0,  804,  805,    0,    0,    0,  806,    0,
  842,  806,  806,    0,    0,    0,    0,    0,    0,    0,
    0,  806,    0,    0,    0,    0,  805,  830,    0,    0,
    0,  805,    0,    0,    0,    0,  839,  806,    0,  839,
    0,    0,    0,  805,    0,  805,  839,    0,  839,    0,
    0,    0,  839,    0,    0,  834,    0,  839,    0,  806,
  839,    0,    0,    0,  806,  833,    0,    0,  830,    0,
  804,  830,    0,    0,    0,  839,  806,    0,  806,    0,
    0,    0,    0,    0,    0,    0,  830,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  834,  839,    0,  834,
  839,  839,    0,    0,    0,    0,    0,    0,    0,    0,
  839,    0,  805,    0,  834,    0,    0,    0,  833,    0,
    0,  833,    0,    0,    0,    0,  839,    0,  833,    0,
  833,  843,    0,    0,  833,    0,    0,    0,    0,  833,
    0,    0,  833,    0,    0,  806,    0,    0,  839,    0,
    0,    0,    0,  839,    0,    0,    0,  833,    0,  831,
    0,    0,    0,    0,    0,  839,    0,  839,    0,    0,
    0,    0,  843,    0,    0,  843,    0,    0,    0,  833,
    0,    0,  833,  833,    0,  841,    0,    0,    0,    0,
  843,    0,  833,    0,    0,    0,    0,    0,    0,    0,
  831,    0,    0,  831,    0,    0,    0,    0,  833,    0,
    0,    0,    0,  842,    0,    0,    0,    0,  831,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  833,    0,    0,    0,  839,  833,    0,    0,  841,    0,
    0,  841,  835,    0,    0,    0,    0,  833,  841,  833,
  841,    0,    0,    0,  841,    0,    0,    0,    0,  841,
    0,    0,  841,    0,    0,    0,  842,    0,    0,  842,
    0,    0,    0,    0,    0,    0,  842,  841,  842,    0,
    0,    0,  842,  835,    0,    0,  835,  842,    0,  830,
  842,    0,    0,    0,    0,    0,    0,    0,    0,  841,
    0,  835,  841,  841,    0,  842,    0,    0,    0,    0,
    0,    0,  841,    0,    0,    0,  833,  834,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  842,  841,    0,
  842,  842,    0,    0,    0,    0,    0,    0,    0,    0,
  842,    0,  830,    0,    0,  830,    0,    0,    0,    0,
  841,    0,  830,    0,  830,  841,  842,    0,  830,    0,
    0,    0,    0,  830,    0,    0,  830,  841,    0,  841,
  834,    0,    0,  834,    0,    0,    0,    0,  842,    0,
  834,  830,  834,  842,    0,    0,  834,    0,    0,    0,
    0,  834,    0,  843,  834,  842,  663,  842,    0,  433,
    0,  434,    0,  830,    0,    0,  830,  830,    0,  834,
    0,    0,    0,    0,  436,    0,  830,    0,    0,  290,
    0,  831,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  834,  830,    0,  834,  834,  841,    0,    0,    0,
    0,    0,    0,    0,  834,    0,  843,    0,    0,  843,
    0,    0,    0,    0,  830,    0,  843,    0,  843,  830,
  834,    0,  843,    0,  842,    0,    0,  843,    0,    0,
  843,  830,    0,  830,  831,    0,    0,  831,    0,    0,
    0,    0,  834,    0,  831,  843,  831,  834,    0,    0,
  831,    0,    0,    0,    0,  831,    0,    0,  831,  834,
    0,  834,    0,    0,  835,    0,    0,  843,    0,    0,
  843,  843,    0,  831,    0,    0,    0,    0,    0,    0,
  843,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  831,  843,    0,  831,  831,
  830,    0,    0,    0,    0,    0,    0,    0,  831,    0,
    0,    0,    0,    0,    0,    0,    0,  835,  843,    0,
  835,    0,    0,  843,  831,    0,    0,  835,  834,  835,
    0,    0,    0,  835,    0,  843,    0,  843,  835,    0,
    0,  835,    0,    0,    0,    0,  831, 1731,    0,    0,
    0,  831,    0,    0,    0,    0,  835,    0,    0,    0,
    0,    0,    0,  831,    0,  831,    0,    0,    0,    0,
 1732,    0,    0,    0,    0,    0,    0,    0,  835,    0,
    0,  835,  835,  396,    0,    0,    0,    0,    0,    0,
  397,  835,    0,    0,    0,    0,    0,    0,    0,    0,
  663,    0,  398,  433,  843,  434,    0,  835,  399,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  436,    0,
    0,    0,    0,  290,    0,    0, 1733,  657,    0,  835,
    0,    0,  831,    0,  835,    0,  401,    0,    0,  402,
    0,    0,    0,    0,    0,    0,  835,    0,  835,  945,
  712,  710,    0,  711,    0,  713,    0,    0,    0,    0,
  107,    0,    0,    0,  403,    0,  404,  405,  907,  906,
  908,    0,    0,    0,    0,    0,  406,    0,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,  886,  108,
  109,    0,  110,    0,    0,    0,    0,    0,    0,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,  835,  122,  123,    0,    0,
    0,    0,  124,  125,  126,  701,    0,    0,    0,  659,
    0,    0,    0,    0,    0,  408,    0,    0,    0,    0,
    0,  409,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  410,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  411,  412,  413,  414,  213,
  214,    0,    0,    0,    0,    0,    0,    0,    0,  415,
    0,    0,    0,    0,    0,    0,    0,    0,  416,  417,
  418,  419,  420,  421,  422,  423,  424,  108,  425,    0,
  110,    0,    0,  426,  427,    0,  428,  429,  430,  113,
  114,  115,  431,  116,    0,    0,  117,  118,  119,  120,
  121,  887,  888,  889,  122,  123,    0,  396,    0,    0,
  124,  432,  126,    0,  397,    0,    0,    0,  663,    0,
    0,  433,    0,  434,    0,    0,  398,    0,    0,  656,
    0,    0,  399,  400,    0,    0,  436,    0,    0,    0,
    0,  290,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  657,    0,    0,    0,    0,    0,    0,    0,  894,
  401,    0,    0,  402,    0,  708,    0,    0,    0,    0,
  709,    0,    0,  895,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  403,    0,
  404,  405,    0,    0,    0,    0,    0,    0,    0,    0,
  406,    0,    0,  875,  407,    0,  600,    0,    0,  896,
    0,    0,  658,    0,    0,    0,    0,  897,    0,  876,
    0,    0,    0,  601,    0,  898,  602,    0,    0,    0,
    0,    0,    0,  100,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  899,    0,  900,    0,    0,    0,    0,
    0,    0,    0,  659,    0,    0,    0,    0,  901,  408,
  902,  903,  904,    0,    0,  409,  603,    0,    0,    0,
    0,  101,  604,  605,  606,  410,  607,  608,  609,    0,
  610,    0,    0,  102,    0,    0,    0,    0,    0,  411,
  412,  413,  414,  213,  214,    0,    0,  139,  611,    0,
  612,    0,    0,  415,    0,    0,    0,  103,    0,  905,
    0,    0,  416,  417,  418,  419,  420,  421,  422,  423,
  424,  108,  425,    0,  110,    0,    0,  426,  427,  613,
  428,  429,  430,  113,  114,  115,  431,  116,    0,    0,
  117,  118,  660,  661,  662,  396,  614,    0,  122,  123,
    0,    0,  397,    0,  124,  432,  126,  663,    0,    0,
  433,    0,  434,    0,  398,    0,    0,    0,  615,    0,
  399,  400,  104,    0,    0,  436,    0,    0,    0,    0,
  290,    0,  105,    0,    0,    0,    0,    0,    0,  657,
    0,    0,    0,    0,    0,    0,    0,    0,  401,    0,
    0,  402,    0,  616,  617,    0,    0,  106,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  618,    0,    0,    0,    0,  107,  403,    0,  404,  405,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,    0,
  882,    0,    0,    0,  108,  109,    0,  110,    0,    0,
    0,    0,    0,    0,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117,  118,  119,  120,  121,    0,    0,
    0,  122,  123,    0,    0,    0,    0,  124,  125,  126,
    0,  659,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,  409,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,  412,  413,
  414,  213,  214,    0,    0,    0,    0,    0,    0,    0,
    0,  415,    0,    0,    0,    0,    0,    0,    0,    0,
  416,  417,  418,  419,  420,  421,  422,  423,  424,  108,
  425,    0,  110,    0,    0,  426,  427,    0,  428,  429,
  430,  113,  114,  115,  431,  116,    0,    0,  117,  118,
  660,  661,  662,    0,  396,  211,  122,  123,  211,    0,
    0,  397,  124,  432,  126,  663,    0,    0,  433,    0,
  434,  211,    0,  398,    0,  211,    0,    0,  211,  399,
  400,    0,    0,  436,    0,    0,    0,    0,  290,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  657,    0,
    0,    0,    0,    0,    0,    0,    0,  401,    0,    0,
  402,    0,    0,    0,    0,    0,    0,    0,  211,    0,
    0,    0,    0,    0,  211,  211,  211,    0,  211,  211,
  211,    0,  211,    0,    0,  403,    0,  404,  405,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
  211,  407,  211,    0,    0,    0,    0,    0,    0,  658,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  503,    0,    0,  433,    0,  434,    0,    0,
    0,  211,    0,    0,    0,    0,    0,    0,    0,    0,
  436,    0,    0,    0,    0,  290,    0,    0,  211,    0,
  659,    0,    0,    0,    0,    0,  408,    0,    0,    0,
    0,    0,  409,    0,    0,    0,    0,    0,    0,    0,
  211,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  411,  412,  413,  414,
  213,  214,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,    0,    0,    0,  211,  211,    0,    0,  416,
  417,  418,  419,  420,  421,  422,  423,  424,  108,  425,
    0,  110,  211,    0,  426,  427,    0,  428,  429,  430,
  113,  114,  115,  431,  116,    0,    0,  117,  118,  660,
  661,  662,  396,    0,    0,  122,  123,    0,    0,  397,
    0,  124,  432,  126,    0,    0,    0,    0,    0,    0,
    0,  398,    0,    0,    0,    0,    0,  399,  400,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  657,    0,    0,    0,
    0,    0,    0,    0,    0,  401,  985,    0,  402,  985,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  987,    0,  985,    0,    0,  985,
    0,    0,    0,  403,    0,  404,  405,    0,    0,    0,
    0,    0,    0,    0,    0,  406,    0,    0,    0,  407,
    0,    0, 1133,    0,    0, 1134,    0,  882,    0,  396,
    0,    0,    0,    0,    0,    0,  397,    0,    0,  985,
    0,    0,    0,    0,    0,  985,  985,  985,  398,  985,
  985,  985,    0,  985,  399,  400,    0,    0,    0,    0,
  503,    0,    0,  433,    0,  434,    0,    0,  659,    0,
    0,  985,    0,  985,  408,    0,    0,    0,  436,    0,
  409,    0,  401,  290,    0,  402,    0,    0,    0,    0,
  410,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  985,    0,  411,  412,  413,  414,  213,  214,
  403,    0,  404,  405,    0,    0,    0,    0,  415,  985,
    0,    0,  406,    0,    0,    0,  407,  416,  417,  418,
  419,  420,  421,  422,  423,  424,  108,  425,    0,  110,
    0,  985,  426,  427,    0,  428,  429,  430,  113,  114,
  115,  431,  116,    0,  100,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,    0,  124,
  432,  126,    0,    0,    0,    0,  985,  985,    0, 1135,
    0,  408,    0,    0,    0,    0,    0,  409,    0,    0,
    0,    0,  101,  985,    0,    0,    0,  410,    0,    0,
    0,    0,    0,    0,  102,    0,    0,    0,    0,    0,
    0,  411,  412,  413,  414,  213,  214,    0,    0,    0,
    0,    0,    0,    0,    0,  415,    0,    0,  103,    0,
    0,    0,    0,    0,  416,  417,  418,  419,  420,  421,
  422,  423,  424,  108,  425,    0,  110,    0,    0,  426,
  427,    0,  428,  429,  430,  113,  114,  115,  431,  116,
    0,    0,  117,  118,  119,  120,  121,    0,    0,    0,
  122,  123,    0,    0,    0,    0,  124,  432,  126,    0,
 1143,    0,    0, 1134,    0,    0,    0,  396,    0,    0,
    0,    0,    0,  104,  397,    0,    0,    0,    0,    0,
    0,    0,    0,  105,    0,    0,  398,    0,    0,    0,
    0,    0,  399,  400,    0,    0,    0,    0,  503,    0,
    0,  433,    0,  434,    0,    0,    0,    0,  106,    0,
    0,    0,    0,    0,    0,    0,  436,    0,    0,    0,
  401,  290,    0,  402,    0,    0,  107,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  403,    0,
  404,  405,    0,    0,    0,  108,  109,    0,  110,    0,
  406,    0,    0,    0,  407,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,  125,
  126,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  503,    0,    0,  433,    0,
  434,    0,    0,    0,    0,    0,    0, 1135,    0,  408,
    0,    0,    0,  436,    0,  409,    0,    0,  290,    0,
    0,    0,    0,    0,    0,  410,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  411,
  412,  413,  414,  213,  214,    0,    0,    0,    0,    0,
    0,    0,    0,  415,    0,    0,    0,    0,    0,    0,
    0,    0,  416,  417,  418,  419,  420,  421,  422,  423,
  424,  108,  425,    0,  110,    0,    0,  426,  427,    0,
  428,  429,  430,  113,  114,  115,  431,  116,    0,    0,
  117,  118,  119,  120,  121,    0,  483,    0,  122,  123,
    0,    0,    0,    0,  124,  432,  126,    0, 1147,    0,
    0, 1134,    0,  107,    0,  396,  599,    0,    0,  600,
    0,    0,  397,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  398,    0,  601,    0,    0,  602,
  399,  400,  108,  109,    0,  110,    0,    0,    0,    0,
    0,    0,  111,  112,  113,  114,  115,    0,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,  401,  122,
  123,  402,    0,    0,    0,  124,  125,  126,    0,  603,
    0,    0,    0,    0,    0,  604,  605,  606,    0,  607,
  608,  609,    0,  610,    0,    0,  403,    0,  404,  405,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,  611,  407,  612,    0, 1155,    0,    0, 1134,    0,
    0,    0,  396,    0,    0,    0,    0,    0,    0,  397,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  398,  613,    0,    0,    0,    0,  399,  400,    0,
    0,    0,    0,  503,    0,    0,  433,    0,  434,  614,
    0,    0,    0,    0,    0, 1135,    0,  408,    0,    0,
    0,  436,    0,  409,    0,  401,  290,    0,  402,    0,
    0,  615,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,  412,  413,
  414,  213,  214,  403,    0,  404,  405,    0,    0,    0,
    0,  415,    0,    0,    0,  406,  616,  617,    0,  407,
  416,  417,  418,  419,  420,  421,  422,  423,  424,  108,
  425,    0,  110,  618,    0,  426,  427,    0,  428,  429,
  430,  113,  114,  115,  431,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123, 1209,    0,
    0,    0,  124,  432,  126,    0,    0,    0,    0,    0,
    0,    0, 1135,    0,  408,    0,    0,    0,    0,    0,
  409,    0,    0,    0,    0,    0,    6,    0,    0,    0,
  410,    0,    0,    0,    0,    0,    0, 1409, 1410,    0,
 1411,    0,    0,    0,  411,  412,  413,  414,  213,  214,
    0,    0,    0,    0,    0,    0, 1412,    0,  415,    0,
    0,    0, 1413,    0,    0,    0,    0,  416,  417,  418,
  419,  420,  421,  422,  423,  424,  108,  425,    0,  110,
    0,    0,  426,  427,    0,  428,  429,  430,  113,  114,
  115,  431,  116,    0,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,    0,  124,
  432,  126,    0, 1158,    0,    0, 1134,    0,    0, 1414,
  396,  141,    0,    0,  141,    0,    0,  397,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   14,    0,  398,
    0,  141,    0,    0,  141,  399,  400,    0,    0,    0,
    0,  503, 1415,    0,  433,    0,  434,    0,    0,    0,
    0,    0,   16,    0,    0,    0,    0,    0,    0,  436,
    0, 1605,    0,  401,  290,    0,  402,    0,    0,    0,
  107,    0,    0,    0,  141,    0,    0,    0,    0,    0,
  141,  141,  141,    0,  141,  141,  141,    0,  141,    0,
    0,  403,    0,  404,  405,    0,    0,    0,    0, 1416,
  109,    0,  110,  406,    0,    0,  141,  407,  141,  111,
  112,  113,  114,  115,    0,  116,    0,    0,  117, 1417,
  119,  120,  121,    0,    0,    0,  122,  123, 1418, 1419,
 1420,    0,  124,  125,  126,    0,    0,  141,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  503,    0,
    0,  433,    0,  434,  141,    0,    0,    0,    0,    0,
 1135,    0,  408,    0,    0,    0,  436,    0,  409,    0,
    0,  290,    0,    0,    0,    0,  141,    0,  410,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,  412,  413,  414,  213,  214,    0,    0,
    0,    0,    0,    0,    0,    0,  415,    0,    0,    0,
    0,  141,  141,    0,    0,  416,  417,  418,  419,  420,
  421,  422,  423,  424,  108,  425,    0,  110,  141,    0,
  426,  427,    0,  428,  429,  430,  113,  114,  115,  431,
  116,    0,    0,  117,  118,  119,  120,  121,    0,    0,
    0,  122,  123,    0,    0,    0,    0,  124,  432,  126,
    0, 1163,    0,    0, 1134,    0,    0,    0,  396,    0,
    0,  548,    0,    0,    0,  397,    0,  107,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  398,    0,    0,
    0,    0,    0,  399,  400,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  108,  109,    0,  110,
    0,  548,  548,    0,    0,  548,  111,  112,  113,  114,
  115,  401,  116,    0,  402,  117,  118,  119,  120,  121,
  548,    0,  548,  122,  123,    0,    0,    0,    0,  124,
  125,  126,    0,    0,    0,    0,    0,    0,    0,  403,
    0,  404,  405,    0,    0,    0,    0,    0,    0,    0,
    0,  406,  548,    0,    0,  407,    0,    0, 1166,    0,
    0, 1134,    0,    0,    0,  396,    0,    0,    0,    0,
    0,    0,  397,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  398,    0,    0,    0,    0,    0,
  399,  400,    0,    0,    0,    0,  503,    0,    0,  433,
    0,  434,    0,    0,    0,    0,    0,    0, 1135,    0,
  408,    0,    0,    0,  436,    0,  409,    0,  401,  290,
    0,  402,    0,    0,    0,    0,  410,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  411,  412,  413,  414,  213,  214,  403,    0,  404,  405,
    0,    0,    0,    0,  415,    0,    0,    0,  406,    0,
    0,    0,  407,  416,  417,  418,  419,  420,  421,  422,
  423,  424,  108,  425,    0,  110,    0,    0,  426,  427,
    0,  428,  429,  430,  113,  114,  115,  431,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123,    0,    0,    0,    0,  124,  432,  126,    0,    0,
    0,    0,    0,    0,    0, 1135,    0,  408,    0,  548,
    0,    0,    0,  409,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  410,  548,    0,  548,  548,  548,    0,
    0,    0,  548,    0,    0,  548,    0,  411,  412,  413,
  414,  213,  214,    0,    0,    0,    0,    0,  548,    0,
    0,  415,    0,    0,  548,    0,    0,  548,    0,    0,
  416,  417,  418,  419,  420,  421,  422,  423,  424,  108,
  425,    0,  110,    0,    0,  426,  427,    0,  428,  429,
  430,  113,  114,  115,  431,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,  548,
    0,    0,  124,  432,  126,    0, 1169,    0,    0, 1134,
    0,    0,    0,  396,    0,    0,    0,    0,    0,    0,
  397,    0,    0,  548,  661,    0,  661,  661,    0,  661,
    0,    0,  398,    0,    0,    0,    0,    0,  399,  400,
    0,    0,  661,    0,  548,    0,    0,  661,    0,    0,
  548,    0,    0,    0,  548,    0,    0,    0,    0,    0,
    0,  548,    0,    0,    0,    0,  401,    0,    0,  402,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  548,
    0,    0,    0,    0,  548,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  403,    0,  404,  405,    0,    0,
    0,  479,    0,    0,    0,    0,  406,    0,    0,  548,
  407,    0,    0,  548,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  107,    0,    0,
    0,  655,    0,  655,  655,    0,  655,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  655,
    0,    0,    0,    0,  655,    0,  108,  109,    0,  110,
    0,    0,    0, 1135,    0,  408,  111,  112,  113,  114,
  115,  409,  116,    0,    0,  117,  118,  119,  120,  121,
    0,  410,    0,  122,  123,    0,    0,    0,    0,  124,
  125,  126,    0,    0,    0,  411,  412,  413,  414,  213,
  214,    0,    0,    0,    0,    0,    0,    0,    0,  415,
    0,    0,    0,    0,    0,    0,    0,    0,  416,  417,
  418,  419,  420,  421,  422,  423,  424,  108,  425,    0,
  110,    0,    0,  426,  427,    0,  428,  429,  430,  113,
  114,  115,  431,  116,  661,    0,  117,  118,  119,  120,
  121,  661,    0,    0,  122,  123,    0,    0,  661,    0,
  124,  432,  126,    0,    0,    0,    0,    0,    0,    0,
  661,    0,    0,    0,    0,    0,  661,  661,    0,    0,
    0,    0,    0,    0,  661,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  661,    0,    0,  661,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  661,    0,  661,  661,    0,    0,    0,    0,
    0,    0,    0,    0,  661,    0,    0,    0,  661,    0,
    0,  655,    0,    0,    0,    0,    0,    0,  655,    0,
    0,    0,    0,    0,    0,  655,    0,    0,    0,  661,
    0,    0,  661,    0,  661,    0,    0,  655,    0,    0,
    0,    0,    0,  655,  655,    0,    0,  661,    0,    0,
    0,  655,  661,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  661,    0,    0,    0,    0,    0,  661,
    0,  655,    0,    0,  655,    0,    0,    0,    0,  661,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  661,  661,  661,  661,  661,  661,  655,
    0,  655,  655,    0,    0,    0,    0,  661,    0,    0,
    0,  655,    0,    0,    0,  655,  661,  661,  661,  661,
  661,  661,  661,  661,  661,  661,  661,    0,  661,    0,
    0,  661,  661,    0,  661,  661,  661,  661,  661,  661,
  661,  661,    0,    0,  661,  661,  661,  661,  661,    0,
    0,    0,  661,  661,    0,    0,    0,    0,  661,  661,
  661,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  655,    0,    0,    0,    0,    0,  655,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  655,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  655,  655,  655,  655,  655,  655,    0,    0,    0,    0,
    0,    0,    0,    0,  655,    0,    0,    0,    0,    0,
    0,    0,    0,  655,  655,  655,  655,  655,  655,  655,
  655,  655,  655,  655,    0,  655,    0,    0,  655,  655,
    0,  655,  655,  655,  655,  655,  655,  655,  655,  661,
    0,  655,  655,  655,  655,  655,  661,    0,    0,  655,
  655,    0,    0,  661,    0,  655,  655,  655,  503,    0,
  508,  433,    0,  434,    0,  661,    0,    0,    0,    0,
    0,  661,  661,    0,    0,    0,  436,    0,    0,  661,
    0,  290,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  661,
    0,    0,  661,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  661,    0,  661,
  661,    0,    0,    0,    0,    0,    0,    0,    0,  661,
    0,    0,    0,  661,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  580,    0,    0,    0,    0,  503,    0,    0,  433,    0,
  434,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  436,    0,    0,    0,    0,  290,  580,
  580,    0,  580,    0,    0,    0,    0,    0,  661,  580,
  580,  580,  580,  580,  661,  580,    0,    0,  580,  580,
  580,  580,  580,    0,  661,    0,  580,  580,    0,    0,
    0,    0,  580,  580,  580,    0,    0,    0,  661,  661,
  661,  661,  661,  661,    0,    0,    0,    0,    0,    0,
    0,    0,  661,    0,    0,    0,    0,    0,    0,    0,
    0,  661,  661,  661,  661,  661,  661,  661,  661,  661,
  661,  661,    0,  661,    0,    0,  661,  661,    0,  661,
  661,  661,  661,  661,  661,  661,  661,    0,    0,  661,
  661,  661,  661,  661,    0,  396,    0,  661,  661,    0,
    0,    0,  397,  661,  661,  661,    0,    0,    0,    0,
    0,    0,    0,    0,  398,    0,    0,    0,    0,    0,
  399,  400,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  401,    0,
    0,  402,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  403,    0,  404,  405,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,    0,    0,    0,    0,  503,    0,  397,
  433,    0,  434,    0,    0,    0,    0,    0,    0,    0,
    0,  398,    0,    0,    0,  436,    0,  399,  400,    0,
  290,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,  409,    0,  401,    0,    0,  402,    0,
    0,    0,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,  412,  413,
  414,  213,  214,  403,    0,  404,  405,    0,    0,    0,
    0,  415,    0,    0,    0,  406,    0,    0,    0,  407,
  416,  417,  418,  419,  420,  421,  422,  423,  424,  108,
  425,    0,  110,    0,    0,  426,  427,    0,  428,  429,
  430,  113,  114,  115,  431,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,  432,  126,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,    0,    0,    0,
  409,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  410,    0,    0,    0,    0,    0,    0,    0,  692,    0,
    0,    0,    0,    0,  411,  412,  413,  414,  213,  214,
    0,    0,    0,    0,    0,    0,    0,    0,  415,    0,
    0,    0,    0,    0,    0,    0,    0,  416,  417,  418,
  419,  420,  421,  422,  423,  424,  108,  425,    0,  110,
    0,    0,  426,  427,    0,  428,  429,  430,  113,  114,
  115,  431,  116,    0,  396,  117,  118,  119,  120,  121,
    0,  397,    0,  122,  123,    0,    0,    0,  542,  124,
  432,  126,    0,  398,    0,    0,    0,    0,    0,  399,
  400,  503,  948,    0,  433,    0,  434,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  436,
    0,    0,    0,    0,  290,    0,    0,  401,    0,  542,
  402,    0,  542,  280,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  542,    0,  542,
    0,    0,    0,    0,    0,  403,    0,  404,  405,    0,
    0,    0,  280,  280,    0,  280,    0,  406,    0,    0,
    0,  407,  280,  280,  280,  280,  280,    0,  280,  542,
    0,  280,  280,  280,  280,  280,    0,    0,    0,  280,
  280,    0,    0,    0,    0,  280,  280,  280,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  701,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  408,    0,    0,    0,
    0,    0,  409,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  411,  412,  413,  414,
  213,  214,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,  971,    0,    0,    0,    0,    0,    0,  416,
  417,  418,  419,  420,  421,  422,  423,  424,  108,  425,
    0,  110,    0,    0,  426,  427,    0,  428,  429,  430,
  113,  114,  115,  431,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,    0,
    0,  124,  432,  126,    0,    0,    0,    0,  396,    0,
    0,    0,    0,  503,    0,  397,  433,    0,  434,    0,
    0,    0,  972,    0,    0,    0,  542,  398,    0,    0,
    0,  436,    0,  399,  400,    0,  290,    0,    0,    0,
    0,  542,    0,  542,  542,  542,    0,    0,    0,  542,
    0,    0,  542,    0,    0,    0,    0,  973,    0,    0,
    0,  401,  974,    0,  402,  542,    0,    0,  975,    0,
    0,  542,    0,    0,  542,    0,    0,    0,    0,  107,
    0,    0,    0,    0,    0,    0,    0,  976,    0,  403,
    0,  404,  405,    0,    0,    0,    0,    0,    0,    0,
    0,  406,    0,    0,    0,  407,    0,    0,  108,  109,
    0,  110,    0,    0,    0,    0,  542,    0,  111,  112,
  113,  114,  115,    0,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,    0,
  542,  124,  125,  126,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,  542,    0,    0,    0,    0,  409,  542,    0,    0,
    0,  542,    0,    0,    0,    0,  410,    0,  542,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  411,  412,  413,  414,  213,  214,  542,    0,    0,    0,
    0,  542,    0,    0,  415,    0,    0,    0,    0,    0,
    0,    0,    0,  416,  417,  418,  419,  420,  421,  422,
  423,  424,  108,  425,    0,  110,  542,    0,  426,  427,
  542,  428,  429,  430,  113,  114,  115,  431,  116,    0,
  396,  117,  118,  119,  120,  121,    0,  397,    0,  122,
  123,    0,    0,    0,    0,  124,  432,  126,  503,  398,
    0,  433,    0,  434,    0,  399,  400,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  436,    0,    0,    0,
    0,  290,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  401,    0,    0,  402,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  403,    0,  404,  405,    0,    0,    0,    0,    0,
    0,    0,    0,  406,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  503,    0,    0,  433,    0,
  434,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  436,    0,    0,    0,    0,  290,    0,
    0,    0,  408,    0,    0,    0,    0,    0,  409,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  410,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1161,
    0,    0,  411,  412,  413,  414,  213,  214,    0,    0,
    0,    0,    0,    0,    0,    0,  415,    0,    0,    0,
    0,    0,    0,    0,    0,  416,  417,  418,  419,  420,
  421,  422,  423,  424,  108,  425,    0,  110,    0,    0,
  426,  427,    0,  428,  429,  430,  113,  114,  115,  431,
  116,    0,    0,  117,  118,  119,  120,  121,    0,    0,
    0,  122,  123,    0,    0,  396,    0,  124,  432,  126,
    0,    0,  397,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  398,    0,    0,    0,    0,    0,
  399,  400,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  401,    0,
    0,  402,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  403,    0,  404,  405,
    0,    0,    0,    0,    0,    0,    0,    0,  406,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,    0,    0,    0,    0,  503,    0,  397,
  433,    0,  434,    0,    0,    0,    0,    0,    0,    0,
    0,  398,    0,    0,    0,  436, 1703,  399,  400,    0,
  290,    0,    0,    0,    0,    0,    0, 1351,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,  409,    0,  401,    0,    0,  402,    0,
    0,    0,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  411,  412,  413,
  414,  213,  214,  403,    0,  404,  405,    0,    0,    0,
    0,  415,    0,    0,    0,  406,    0,    0,    0,  407,
  416,  417,  418,  419,  420,  421,  422,  423,  424,  108,
  425,    0,  110,    0,    0,  426,  427,    0,  428,  429,
  430,  113,  114,  115,  431,  116,    0,    0,  117,  118,
  119,  120,  121,    0,    0,    0,  122,  123,    0,    0,
    0,    0,  124,  432,  126,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,    0,    0,    0,
  409,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  410,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1364,    0,    0,  411,  412,  413,  414,  213,  214,
    0,    0,    0,    0,    0,    0,    0,    0,  415,    0,
    0,    0,    0,    0,    0,    0,    0,  416,  417,  418,
  419,  420,  421,  422,  423,  424,  108,  425,    0,  110,
    0,    0,  426,  427,    0,  428,  429,  430,  113,  114,
  115,  431,  116,    0,  396,  117,  118,  119,  120,  121,
    0,  397,    0,  122,  123,    0,    0,    0,    0,  124,
  432,  126,  977,  398,  977,  977,    0,  977,    0,  399,
  400,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  977,    0,    0,    0,    0,  977,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  401,    0,    0,
  402,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  403,    0,  404,  405,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  435,
    0,    0,  433,    0,  434,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  436,    0,    0,
    0,    0,  290,    0,    0,    0,  408,    0,    0,    0,
    0,    0,  409,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  411,  412,  413,  414,
  213,  214,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,    0,    0,    0,    0,    0,    0,    0,  416,
  417,  418,  419,  420,  421,  422,  423,  424,  108,  425,
    0,  110,    0,    0,  426,  427,    0,  428,  429,  430,
  113,  114,  115,  431,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,  977,
    0,  124,  432,  126,    0,    0,  977,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  977,    0,
    0,    0,    0,    0,  977,  977,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  977,    0,    0,  977,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  977,    0,  977,  977,    0,    0,    0,    0,    0,    0,
    0,    0,  977,    0,    0,    0,  977,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  396,    0,    0,    0,
    0,  503,    0,  397,  433,    0,  434,    0,    0,    0,
    0,    0,    0,    0,    0,  398,    0,    0,    0,  436,
    0,  399,  400,    0,  290,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  977,    0,    0,    0,    0,    0,  977,    0,  401,
    0,    0,  402,    0,    0,    0,    0,  977,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  977,  977,  977,  977,  977,  977,  403,    0,  404,
  405,    0,    0,    0,    0,  977,    0,    0,    0,  406,
    0,    0,    0,  407,  977,  977,  977,  977,  977,  977,
  977,  977,  977,  977,  977,    0,  977,    0,    0,  977,
  977,    0,  977,  977,  977,  977,  977,  977,  977,  977,
    0,    0,  977,  977,  977,  977,  977,    0,    0,    0,
  977,  977,    0,    0,    0,    0,  977,  977,  977,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  408,    0,
    0,    0,    0,    0,  409,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  410,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  411,  412,
  413,  414,  213,  214,    0,    0,    0,    0,    0,    0,
    0,    0,  415,    0,    0,    0,    0,    0,    0,    0,
    0,  416,  417,  418,  419,  420,  421,  422,  423,  424,
  108,  425,    0,  110,    0,    0,  426,  427,    0,  428,
  429,  430,  113,  114,  115,  431,  116,    0,  396,  117,
  118,  119,  120,  121,    0,  397,    0,  122,  123,    0,
    0,    0,    0,  124,  432,  126,  977,  398,    0,  977,
    0,  977,    0,  399,  400,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  977,    0,    0,    0,    0,  977,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  401,    0,    0,  402,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  403,
    0,  404,  405,    0,    0,    0,    0,    0,    0,    0,
    0,  406,    0,    0,    0,  407,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  320,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,  409,    0,    0,    0,
    0,  320,    0,    0,    0,    0,  410,    0,  320,    0,
    0,    0,  320,  320,    0,  320,    0,    0,  320,    0,
  411,  412,  413,  414,  213,  214,    0,    0,    0,    0,
    0,  320,    0,    0,  415,    0,    0,  320,    0,    0,
    0,    0,    0,  416,  417,  418,  419,  420,  421,  422,
  423,  424,  108,  425,    0,  110,    0,    0,  426,  427,
    0,  428,  429,  430,  113,  114,  115,  431,  116,  554,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123,  539,    0,  977,    0,  124,  432,  126,    0,    0,
  977,    0,    0,    0,  320,    0,    0,    0,    0,    0,
    0,    0,  977,    0,    0,    0,    0,    0,  977,  977,
  554,    0,  320,  554,    0,    0,    0,    0,    0,    0,
    0,    0,  539,    0,    0,  539,    0,  320,  554,    0,
  554,    0,    0,    0,    0,    0,  977,  320,    0,  977,
  539,    0,  539,    0,    0,    0,  320,    0,  320,    0,
    0,    0,    0,    0,    0,  320,    0,    0,    0,    0,
  554,    0,    0,    0,  977,    0,  977,  977,    0,    0,
    0,    0,  539,    0,    0,    0,  977,    0,    0,    0,
  977,    0,    0,    0,  320,  320,    0,  320,    0,    0,
    0,    0,    0,    0,  320,  320,  320,  320,  320,    0,
  320,    0,    0,  320,  320,  320,  320,  320,    0,    0,
    0,  320,  320,  320,  320,  320,    0,  320,  320,  320,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  977,    0,    0,    0,    0,
    0,  977,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  977,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  977,  977,  977,  977,  977,
  977,    0,    0,    0,    0,    0,    0,    0,    0,  977,
    0,    0,    0,    0,    0,    0,    0,    0,  977,  977,
  977,  977,  977,  977,  977,  977,  977,  977,  977,    0,
  977,    0,    0,  977,  977,    0,  977,  977,  977,  977,
  977,  977,  977,  977,    0,    0,  977,  977,  977,  977,
  977,    0,    0,    0,  977,  977,    0,  554,    0,    0,
  977,  977,  977,    0,    0,    0,    0,    0,    0,  539,
    0,    0,  554,    0,    0,  554,  554,    0,    0,    0,
  554,    0,    0,  554,  539,    0,    0,  539,  539,    0,
    0,    0,  539,    0, 1209,  539,  554,    0,    0,    0,
    0,    0,  554,    0,    0,  554,    0,    0,  539,    0,
    0,    0,    0,    0,  539,    0,    0,  539,    0,    0,
    0,    0,    6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1409, 1410,    0, 1411,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  554,    0,    0,
    0,    0, 1412,    0,    0,    0,    0,    0, 1413,  539,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  554,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  539,    0,    0,    0,    0,    0,    0,
    0,    0,  554,    0,    0,    0,    0,    0,  554,    0,
    0,    0,  554,    0,  539,    0,    0,    0,    0,  554,
  539,    0,    0,    0,  539, 1414,    0,  599,    0,    0,
  600,  539,    0,    0,    0,    0,  800,  554,    0,    0,
    0,    0,  554,   14,    0,    0,    0,  601,    0,  539,
  602,    0,    0,    0,  539,    0,    0,    0, 1415,    0,
    0,    0,    0,    0,    0,    0,    0,  554,   16,    0,
    0,  554,    0,    0,    0,    0,    0,    0,    0,  539,
    0,    0,    0,  539,    0,    0,  107,    0,  599,    0,
  603,  600,    0,    0,    0,    0,  604,  605,  606,    0,
  607,  608,  609,    0,  610,    0,    0,    0,  601,    0,
    0,  602,    0,    0,    0, 1416,  109,    0,  110,    0,
    0,    0,  611,    0,  612,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117, 1417,  119,  120,  121,    0,
    0,    0,  122,  123, 1418, 1419, 1420,    0,  124,  125,
  126,  603,    0,  613,    0,    0,    0,  604,  605,  606,
    0,  607,  608,  609,    0,  610,    0,    0,    0,    0,
  614,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  611,    0,  612,    0,    0,    0,    0,
    0,    0,  615,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  107,    0,    0,
    0,    0,    0,    0,  613,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  616,  617,    0,
    0,  614,    0,    0,    0,    0,  108,  109,    0,  110,
    0,    0,    0,    0,  618,    0,  111,  112,  113,  114,
  115,    0,  116,  615,    0,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,  107,  124,
  125,  126,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  616,  617,
    0,    0,    0,    0,    0,    0,    0,  108,  109,    0,
  110,    0,    0,    0,    0,  618,    0,  111,  112,  113,
  114,  115,    0,  116,    0,    0,  117,  118,  119,  120,
  121,    0,    0,    0,  122,  123,  107,    0,    0,    0,
  124,  125,  126,    0,    0,  169,  170,  171,  172,  173,
  174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
  184,    0,    0,  185,  186,  108,  109,    0,  110,    0,
    0,  187,  188,    0,    0,  111,  112,  113,  114,  115,
  189,  116,  190,    0,  117,  118,  119,  120,  121,    0,
    0,  468,  122,  123,  191,  192,  193,  194,  124,  125,
  126,  468,  468,  468,  468,  468,  468,  468,  468,  468,
  468,  468,  468,  468,  468,  468,    0,    0,  468,  468,
  468,  468,    0,  468,    0,    0,  468,  468,    0,    0,
  468,  468,  468,  468,  468,  468,  468,  468,    0,  468,
  468,  468,  468,  468,    0,    0,    0,  468,  468,  468,
  468,  468,  468,  468,  468,  468,
};
static short yycheck[] = {                                       9,
    0,   56,   12,  106,   65,   16,   16,  531,   18,    4,
   63,  145,  146,   64,  145,  146,   57,  716,  374,   69,
  374,  370,  345,   95,  156,   86,  471,   68,  881,  569,
   92,   64,  885,  514,  366,   90,   71,  435,  538, 1208,
  292,  723,   92,   74,   54,   55,   56,   57,   58,   59,
   91,   89,  944,  967,   64,   65,   66,   98,   68,   94,
   70,   71,  343,   73,   74,   96,    0,  701,   41, 1281,
  869,  770,   82,   84,   84,   85,   86,   58,   88,   89,
   90,   91,   93,   93,   94,   95,   96,  159,   98,   70,
  692,  319,  644,    0,  352,    0,  698,  989,   45,  491,
    0, 1051,  275,  136,  738,  138, 1001,   41,  651, 1103,
  351,  284,  150,   41,   45,   40,  157,  562, 1040,  564,
 1040,  379,    0,    0,  569,   59,  136,  137,  138, 1051,
    0, 1051,    0,  282,  237,    0,   41,    0,    0,   44,
  150,   45,   59,   44,   41,  198,  156,  157,  158,  159,
 1040, 1045,   59,  163,   59, 1040,  137, 1040, 1052,   59,
 1040, 1051,   44,   41,   41, 1080, 1051,   40, 1051, 1063,
  223, 1051,    0,   41,   40,   44,   41,  158,    0,   41,
    0,   59,   59,  742,    0,  497,    0,  319,   44,   59,
   63,   59, 1086, 1226,   59,   41,   59,   59,   44,  548,
   44,   40,   44,    0,  262,  777,   45,   44,   44,   40,
    0,    0, 1104,  569,   41,  569,  273,    0,   61,   41,
    0,   41,  794,  326,   44,  797, 1047,   41,   44,  753,
   41,   59,   63,  266,  806,    0,  269,   59,  658,   59,
    0,   91, 1219,   59, 1221,   59,   61,  596,  303, 1418,
 1419,   41,  447,    0,   44,  276,  266,  163,   41,  269,
 1241,   44,   59,  273, 1230,  275,  276,   63,  317,   59,
   59,   44,  282,   44,  284,  533,   59, 1098,    0,   59,
  344,   41,  264,   41,   44,   41,  319,   12,  228, 1530,
  862,  572,  273,  303,   59,  316,   41,  307,   41,   59,
  264,  282,  317,  395,    0,    0,   44,  909,  910,  707,
  483,  344,   59,  339,  367,    0,  369,  715,  351,   41,
  353,  379,  355, 1273, 1274,  358,  307,    0,  334,  397,
  383,   45,  387,  260,  344,  376,  362,   59,  940,  620,
    0,  351,  377,  353,  351,  355,   41, 1242,  358,   63,
  499, 1273, 1274, 1273, 1274,    0,    0, 1307,  258,  491,
 1326,    0, 1276,   59,   59,  846,  376,  377,   41,  427,
  270,   44,  393,  300,   59,  385,  260,  387,  823,  428,
  448,  391,  322, 1273, 1274, 1307,   59, 1307, 1273, 1274,
 1273, 1274,  418, 1273, 1274,  258,  391,  297, 1313,   59,
  280,  458, 1296,  303,  385,  305, 1320,  270,  500,  457,
  315,  459,  433,  272,   59,   59,  300, 1307,  143,    0,
   59,  403, 1307,  982, 1307,  275,  436, 1307,  434, 1641,
 1599,  336, 1253,  745,  297,  994,  995, 1039,  315,  403,
  303,  482,  305, 1279,  344,  509,  380,  325,  317,  327,
 1022,  262,  380,  587,  297,  389,  497,  301,  491,  336,
   41,  389,  387,   44,  301,   44,  886,   42,  336,  479,
 1042, 1043,  482,  483,  809,  264,    0,  484,   59,  315,
  490,  344,  297,  380,  494,  402, 1467,  497, 1465,  499,
 1062,  264,  502,  315,  259,  315,  748,  550,  479,  264,
  297,  315,  380,  450,  269,   41,  262,  788, 1749,  490,
  791,  389,   93,  494,  336,  380,  336,   41,  499,  450,
 1319,  802,  336, 1205, 1126, 1127,  588, 1129, 1130, 1741,
  477,  401,  315, 1527,  315,   59,  569,  590,  588,  446,
  513,  446,  325,  771,  327, 1097,  450,  344,  829,  450,
  873,  431,  379,  336,  795,  315,  339,  403,  568,  569,
  733,  306,  444,  306,  574, 1479,  380,  327,  379,  446,
 1113,    0,    0,  439,   41,  389,  336,  450,  446,  513,
 1475,   41, 1615, 1795,  423,  513,  376,  640,  641,  720,
  446,  338,  990,  315, 1488,  987,  502,  380, 1492, 1493,
  428,  390,  860,    0,  446,  327,  389,  498,  513,  509,
  643,  450,   41,   41,  336,   44,  513, 1829,  432,  450,
  380,  379,  756,  379,  446,  390,  446, 1542,  444,  389,
   59,   59,  446,  643, 1528,  513,  513,  297,  376,  771,
  325,  336,  327,   40,   41,  683,  509,   44,  513,  432,
  397,  513,  432,  400,  450, 1547,  298,  428,  380, 1495,
 1262,  444,   59,  446,  445,  361,  446,  389,    0,  416,
  305,   41,  432,  683,  494,    0,  310, 1236,   41,  298,
  315,   59, 1284,  262,  344,  260,  446, 1508,  721,    0,
  723,  513,  725,  513,  634,  442,   12,    0,  708,  513,
 1429, 1033,  401,  493,  745, 1526,  716,  315, 1877,  423,
  432,  721,  509,  723, 1401,  725,   41, 1351,  728,   44,
 1055, 1056,  361,  733,  446,  300,  262,   59,  298,  395,
  513,  784,  742, 1627,   59,  745,  450, 1631,  771, 1653,
 1138,  751,  752,  776,  261, 1659, 1318,  432,   59, 1699,
  322,  446, 1646,  513,  742,  765,   59,  767,  402,  821,
  770,  446,  795,    0,  334, 1029,  776, 1099, 1049, 1663,
    0,  821,    0, 1108,  275,  409, 1463, 1699,  828, 1699,
 1061,  834, 1282,  371,  765,  795,  767,    0, 1111,  822,
  823,  513,  434,  843,  856,  857,  858,  283,  971,   41,
  379,  974, 1200,  327, 1085,  319,  856,  857,  858, 1699,
  445,   41,  822,  823, 1699,  434, 1699,  258,    0, 1699,
  493,  402,   59,    0, 1305, 1753,  836,  143,  268,   59,
 1311,   59,    0,  262,    0,    0,  450,  445,    0, 1379,
  341,  836, 1240,  379, 1108,    0,   59,    0,  514,  509,
  422,  861,  440, 1176, 1748,  987,  380,  429,  325,   41,
  327,  871,  303, 1757,  434,  389,  273,  327,    0,  879,
  880,  380, 1800,   41,    0, 1477,   41,   59,   42,   41,
  861,    0,   59,   47,  879,    0,  315,  315,  297,  318,
  871,   59,  262,   59,   59,  272,  325,   59,  327,  262,
  304,  305,  331,  975,   59, 1799,   59,  336,  336,   41,
  339, 1839, 1765,  380,    0,  262,    0,  450,  315,  359,
  380,  318,  389,  432,  319,  354,   41,   59,  325,  389,
  327,  972,  658,   59,  331,  344,    0,  663,  973,  336,
   59,  348,  339, 1871,   59,  976,    0,  376,    0, 1083,
  379,  380,  380,  263,  987,   41,  262,  354, 1403,  336,
  389,  971,  972,  973,  974,  975,  976,  453,  454,  410,
  257,  394,  982,   59, 1323,   59,  405,   41,   40,  376,
   44,    0,    0,  380,  994,  995,  257,  428, 1590,  513,
  315, 1001,  389,    0,  982,   59, 1029, 1227,  427,  309,
  325, 1001,  327,  432,  432,   59,  994,   59,  405,  379,
  320,  336,  344, 1294,  339,  444,  379,  446,  446, 1029,
  262,  462,    0, 1379,  327, 1379,   40,  264,   41,    0,
 1092,   44,  379,  344,   41,  432,  264,   44,    0,  443,
   59,   59, 1092,  330, 1325,    0,  513,  444, 1110,  446,
  262, 1780,   59,  513, 1116,  380,  265, 1581,    0,  330,
 1110,    0, 1072,   41,  389, 1219, 1116, 1221, 1101, 1786,
   41,  352, 1302,  379, 1761, 1108, 1775, 1209,   44,    0,
 1090,   59, 1077, 1812,  513,  513,   93,  264,   59,   44,
  299, 1101, 1616,   59, 1225, 1226, 1227,   59, 1108,   41,
  509, 1751,   44,  258,   59,   44,  336,  432, 1698,  419,
    0,  264,  325,  277,  327,  270,  513,   59,  282,  444,
   59,  446, 1724,    0, 1258, 1854, 1260, 1729,   16,  432,
  376,  297,  258,  315,  304,  305, 1726,  379,   59, 1856,
  421, 1665,  297,  446,  270,  264, 1875,  315,  303, 1878,
  305,  305, 1802,  315,  336,    0,  882, 1208,    0,  280,
  886,  315,  390,  325, 1893,  327, 1756,  379,  336,   59,
  390,  336, 1205,  376,  336, 1208, 1209,  303,  344,  305,
  304,  305,   59,  315,    0,   44,  352, 1321,  513,  344,
 1840,  316,  276,  277,  352, 1205,   84,  352, 1208, 1209,
  315,   44,   42,   43,  336,   45,  484,   47,  428, 1219,
  325, 1221,  327,  390,   59,  427, 1497,   59,  380,  432,
  386,  336,  388,  428,  276,  277, 1236,  389,  386,  315,
  388,  444, 1242,  446,   93,  277,    0,  390,  516,  325,
  282,  327,  462,   59,  410,  264,  344,    0,  380,  318,
  336,  315,  410, 1843,  318, 1536,  377,  389,  276,  277,
  273,  325,  331,  327,  446,  380,  421,  331,  393,  435,
  432,  390,  336,  443,  389,  339,  333,  435,  446,  378,
    0,  446, 1806, 1336,  446,  354, 1888, 1807, 1341,  265,
  354,  445,  260,  306,  380,   59, 1190, 1429,  298, 1193,
  432, 1821,  264,  389, 1763, 1300,   59,  376,  433,  376,
  431,  368,  376,  289,  446,  379,  380,  432,    0,  443,
  372,   41,  298,  380,   44,  389, 1785,  448,  371,  444,
  298,  446,    0,    0,  334,  348,  405,  315, 1470,   59,
  376,  405,  310,  509,  315, 1804,  432,  271, 1409,  327,
  297,  513,  486,  329,  509,  453,  454,  333,  336,   41,
  446,  285,   44,  427,  260,  336,  334,  410,  432, 1889,
  325,  390,  327,  315,  264,  258,  318,   59,  261, 1660,
  444,  513,  446,  325,  339,  327,  325,  264,  327,  331,
  403,   59,   59,  318,  336,  402, 1429,  339,  513, 1409,
  339,  348,  380,    0,  300,  329,  331, 1417, 1418, 1419,
  397,  389,  354, 1872,  457, 1844,  397,  338,  333, 1429,
  303,  410,  264,    0,    0,  273, 1855,  513,  390,  354,
  513,  399,   44,  357,  376,  416,   40,  277,  380,    0,
  265,  409,  282,   42,   43,  458,   45,  389,   47,  513,
  371,  376,  428,    0,  432, 1465,  318,  433,  434,  435,
 1470,  448,   59,  405,   41, 1475,  434,   44,  446,  331,
 1602,    0,  348, 1902,  299,  446,  397,  432,    0,  400,
  405,   93,   59,   59, 1615, 1617,   44,   44, 1498,  444,
  432,  446,  354,  432,   41,  416,  372,  421,   59, 1393,
  390,   59,  444, 1397,  446,  444,  247,  446,  333,  298,
  452,    0,   59,  390,  255,   42,   43, 1498,   45,   41,
   47,  442,   44,   42,   43,  416,   45,  503,   47,  260,
   59,  507,   59,    0,    0,  513,   93,   59,    0,  481,
  482, 1644,  484,  405,  453,  334, 1587,   44,  390,  491,
  492,  493,  494,  495,  372,  497,  276,  277,  500,  501,
  502,  503,  504,  318,  284,    0,  508,  509,    0,  300,
   59,  513,  514,  515,  516,  393,  331, 1587,   44,    0,
   41,  399,    0,   44,  339,  338,  264,    0,  428, 1599,
 1645,  284,   59,   59,  276,  277,   93,   59,  259,  354,
   42,   43,  284,   45,  297,   47,   41, 1617,  269,   44,
  399, 1611,  313,    0,  333,  433,    0, 1670,  371,  315,
  409, 1674,    0,   41,   59, 1635,   44,   59,   41, 1484,
 1640,   44,    0,    0,    0, 1645, 1768,    0,   59, 1742,
 1772,   59, 1774,    0,  397,  434,   59,  400, 1780,  368,
  405, 1783,  372, 1664, 1664, 1716,  376,   42,   43, 1640,
   45,  380,   47,  416,  333,   40,   42,   43,  396,   45,
  338,   47,   59,  393, 1529,   59,  260, 1809,  277,  399,
 1812,   59, 1576,  282,  412,    0,  260,  663,  318,  442,
  372,   59,   59,   59,  452,    0,   59,  293,    0,  276,
  277,  331,   59,  371,  680,  301, 1716, 1717,  684,  339,
  332,  393,    0,  433, 1846,  691,  300,  399,  317, 1851,
  696,  697, 1854, 1718,  354,    0,  300,  338,  260,  397,
  444,  707,  400,  709,  710,  711,  712,  713,  714,  715,
 1751, 1751,    0, 1875,   59, 1602, 1878, 1780,  416, 1768,
  277,  433,  485, 1772,   59,  282,    0,   59,  277,    0,
  371, 1893,  495,  282, 1783, 1775,   41,  409,  300,   44,
 1780,   59,  395,  493,  442,  405, 1786,  338,  754, 1812,
 1791, 1791,   42,   43,   59,   45,  397,   47,    0,  400,
 1809, 1802, 1802,    0,  436,  372,  403,   41,  774, 1873,
   44,   59, 1812,  779,  780,  416, 1880,  430, 1863,  336,
  371,  493,  395, 1824, 1824,   59,  393,  336,   59,    0,
  336, 1854,  399,   42,   43,   40,   45, 1846,   47, 1840,
 1840,  442, 1851, 1907,  317,  277,  397,  315,   41,  400,
  282,   44, 1875,  395, 1854, 1878, 1856,   59,   41, 1860,
 1860,   44,   59, 1863,  395,  416,  433,  258,  276,  277,
 1893,  291,  498,  276,  277, 1875,  284, 1877, 1878,  270,
  376,  284,  302, 1883,  426,  317,   46,  259,   59,  309,
  265,  442,  450, 1893,  449,  450,  451,  269,   58,  441,
  376,   61,  277,   46, 1904,  409, 1906,  282,  313,  314,
  428,  277,  303,  333,  305,   58,  282,   41,   61,  477,
   44, 1768,  477,  478,  299, 1772, 1602, 1774,  894,  895,
  896,  442,  436,  899,  900,  901, 1783,  903,  904,  905,
  906,  907,  908,  356,   41,  911,  912,   44,  914,  315,
  916,   46,  426,  919,  920,  921,  922,  923,  924,  330,
  377,  332, 1809,   58,  372,   41,   61,  409,   44,  372,
  936,  679,  938,  681,  315,  395,    0,  685,  686,  687,
   41,   42,   43,  508,   45,  393,   47,  485,  486,   41,
  393,  399,   44,  453,  454,   41,  399,  262,   44, 1846,
  508,   41,   42,   43, 1851,   45,  426,   47,   41,   41,
  430,   44,   44,    0,  384,  385,   40,   41,   42,   43,
   44,   45,   46,   47,  990,  433,   41,  277,  262,   44,
  433,   41,  282,  284,   44,   59,   60,   61,   62, 1904,
   41, 1906,    0,   44,  409,   41,   41,  509,   44,   44,
  315,  461,   41,  318,   41,   44,   41,   44,  308,   44,
  325,   41,  327,   44,   44,  366,  331,   91,  277,   93,
   61,  336,   59,  282,  339,   41,   41,  283,   44,   44,
   40,  315,  264,   41,  318,  493,   44,  452,   40,  354,
  493,  325, 1768,  327,  453,  454, 1772,  331, 1774,  308,
   40,   59,  336,   40,   40,  339,   40, 1783, 1534, 1535,
  453,  376,   40,   40,  379,  380,  481,  482,   40,  484,
  354,   40,   40,  453,  389,  453,  491,  492,  493,  494,
  495,   40,  497, 1809,   40,  500,  501,  502,  503,  504,
  405,   40,  376,  508,  509,  379,  380,   40,   40,  514,
  515,  516,   41,   42,   43,  389,   45,   44,   47,    0,
   42,   43,  427,   45,   61,   47,   91,  432,   46,   40,
 1846,  405, 1138,  264,  428, 1851,  317,   44,  395,  444,
  317,  446,   44,   44, 1150, 1151,   46, 1153, 1154,   42,
   43,   44,   45,  427,   47, 1161,  420,   61,  432,  264,
   41,   40,   44,   44,   42,   43,  434,   45,  446,   47,
  444,  344,  446,  344, 1180,  344,  277,  276,   59,    0,
   61,  282, 1188, 1189, 1190,  276,   40, 1193,  297, 1195,
  264,  390,  428, 1199, 1200, 1201, 1202,  277,  262,  462,
  264,  265,  282,  390,   58,  275,  450,  271,  513,  439,
  344,  439,  392,  277,   40,  345,   40,   91,  282,  315,
   41,  285,   40,   44, 1230,    0,  264,   41,   42,   43,
   40,   45,  376,   47, 1240,  299,  403,   44,   59,  513,
  304,  305,  382,  375,  308,   40,   40,  443,   41,  276,
  277,  315,  275,  317,  318,   63,  296,  321,  428,  456,
  314,  325,   40,  327,  317,  329,   41,  331,  317,   44,
  444, 1277,  336,  337,  440,  339,  264,    0,  276,  277,
  494,  375,  497,  281,   59,   40,  284,  273,   40,  297,
  354,  355,  281,  357,  340,  340,   41,   42,   43,  297,
   45,  276,   47,  379,  379,   40,  370,  410,  372,  373,
  374,  379,  376,  408,  297,  379,  380,  410,   41,  352,
 1326,   44,  409,   40,  409,  389,   41,   42,   43,   40,
   45,   41,   47,  317,  277,   44,   59,   44,  402,  270,
  262,  405, 1348,  361,  378,  372,   44,  317,  277,  438,
   40, 1357,   44,  282,  325,  277,  266,  421, 1364,   40,
  282,   40,  414,  427,  428,  493,  393,   40,  432,   41,
   42,   43,  399,   45,  372,   47,  264,  258,   41,  443,
  444,   58,  446,   41,  277,   41,  264, 1393,  452,  282,
   41, 1397,  273,  264,  387,  393,   41,  306,  446,  277,
  432,  399,   41,  306,  282,   40,  433,   40, 1414,   40,
  348,   40,    0,   40,   40,   40,  297,  481,  482,   40,
  484,   40,  303,   40,   40,  306,   40,  491,  492,  493,
  494,  495,   40,  497,   40,  433,  500,  501,  502,  503,
  504,  262,   40,   40,  508,  509,  262,  305,  427,  513,
  514,  515,  516,   41,   42,   43,   44,   45,   46,   47,
   41,   42,   43,  277,   45,  324,   47,  348,  282,   44,
 1476,   59,   60,   61,   62,   41,   41,  264,   41,   42,
   43,  273,   45,  381,   47,  381,  381,  339,  327,   44,
   40,  399,  384,  284,  315,  379,  379,  318,   41,  379,
  450,  276,  277,   91,  325,   93,  327,  379,  372,  284,
  331,  379,  379,   41,  276,  336,   40,  305,  339,   41,
   41,   41,  403,  409,   41,   42,   43,   41,   45,  410,
   47,  290,  277,  354,   59,    0,  427,  282,   40,  262,
   59,   59,   41,   42,   43,  305,   45,  428,   47, 1555,
 1556,  336,   59,   59,   59,  376,  336,   58,  379,  380,
   42,   43,  277,   45,   59,   47, 1572,  282,  389,   59,
 1576,   59,  378,  428, 1580,  277,   41,  458,  389,   44,
 1586,  462, 1588,  333,  405,  434,  334,  434,  334,   41,
  298,   41,  315,  298,   59,  318,  317,  372,  452,  348,
  365,  378,  325,   41,  327,  277,  427,  365,  331,   41,
  282,  432,   41,  336,   42,   43,  339,   45,  393,   47,
   41,   41,   41,  444,  399,  446,   41,  481,  482,   41,
  484,  354,   41,   41,   41,   41,   41,  491,  492,  493,
  494,  495,   41,  497,   41,   41,  500,  501,  502,  503,
  504,   41,   40,  376,  508,  509,  379,  380,  433,   59,
  514,  515,  516, 1669,  443,  315,  389,   40,   44,   41,
  348, 1677,  340,   40,  262,  458,  264,  265,   59,  336,
   46,  500,  405,  271,  336,   40,  362,   40,  376,  277,
   44,  427,  513,  376,  282,   41,  277,  285,  899, 1705,
  901,  282,  903,  904,  427,  906,  907,  908,  402,  432,
  376,  299,    0,  301,  277,  339,  304,  305,   41,  282,
  308,  444,   41,  446, 1730,   59,   44,  315,  301,  317,
  318,  281,   41,  321,   41,   42,   43,  325,   45,  327,
   47,  329,   44,  331,  298,  434,  380,  304,  336,  337,
  290,  339,  301,   41,   42,   43,   44,   45,   46,   47,
  277,   40,  455,  333,   44,  282,  354,  355,   41,  357,
   40,   59,   60,   61,   62,   41, 1782,    0,  277,  452,
  262,  333,  370,  282,  372,  373,  374,  409,  376,  376,
  513,  379,  380,  376,   40,  277,  264,  264,   42,  276,
  282,  389,  450,   91,  317,   93,  443,  462,  481,  482,
  403,  484,   44,  270,  402,   40,  270,  405,  491,  492,
  493,  494,  495,  264,  497,  427,   41,  500,  501,  502,
  503,  504,  450,  421,  428,  508,  509,  450,   41,  427,
  428,  514,  515,  516,  432,  339,  339,  339,  339,  277,
   59,   59,  305,  318,  282,  443,  444,   40,  446,   41,
  325,   59,  327,   59,  452,  301,  331,   42,   43,   41,
   45,   59,   47,   41,  339,  464,  465,  466,  467,  468,
  469,  470,  471,   41,    0,   60,   61,   62,    0,  354,
    0, 1222,   12,  481,  482,  284,  484,  277,  276,  733,
 1113,  874,  452,  491,  492,  493,  494,  495,  647,  497,
   88,  376,  500,  501,  502,  503,  504, 1111,  879,  836,
  508,  509, 1108, 1516, 1100,  513,  514,  515,  516,  371,
  341,  481,  482, 1517,  484,  596, 1090,  324, 1036,  776,
  405,  491,  492,  493,  494,  495, 1101,  497,  804,  353,
  500,  501,  502,  503,  504,  353, 1635, 1582,  508,  509,
 1495, 1793,  574, 1497,  514,  515,  516,  432, 1797, 1796,
  277, 1831,  795,  968,  262,  282,  264,  265, 1705,  444,
 1417,  446, 1781,  271, 1607, 1024, 1255,  721,  358,  277,
 1480, 1329, 1751,  853,  282,  346,  576,  285,   41,   42,
   43,  339,   45,    0,   47, 1309,   13,  599,  311,  845,
   42,  299,    0,  633,  765,  779,  304,  305,  516,  148,
  308,  754, 1625,  520, 1730, 1860, 1824,  315,  531,  317,
  318,  991, 1001,  321, 1152,  741, 1195,  325, 1264,  327,
   -1,  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   59,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   42,   43,   44,   45,  452,
   47,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   45,   -1,   -1,  271,   -1,   -1,   -1,
   -1,  389,  277,   91,   -1,   93,   -1,  282,  481,  482,
  285,  484,   -1,   -1,  402,   -1,   -1,  405,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  421,   -1,  508,  509,   -1,   -1,  427,
  428,  514,  515,  516,  432,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  329,  443,  444,   -1,  446,   41,
   42,   43,  337,   45,  452,   47,   -1,   -1,   41,   42,
   43,   -1,   45,   -1,   47,   41,   42,   43,   -1,   45,
  355,   47,  357,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,  481,  482,  370,  484,  372,  373,  374,
   -1,   -1,   -1,  491,  492,  493,  494,  495,  452,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,  277,   -1,   -1,  481,  482,  282,
  484,   -1,   -1,   -1,   -1,   -1,  421,  491,  492,  493,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,  293,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  262,   -1,  264,  265,   -1,  276,
  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
  297,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
  277,  299,    0,   -1,   -1,  282,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   41,   42,   43,  325,   45,  327,
   47,  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   42,   43,
   44,   45,  370,   47,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  277,  393,   -1,   -1,   -1,
  282,  389,  399,   91,  277,   93,   58,   -1,   -1,  282,
   -1,  277,   -1,   -1,  402,   -1,  282,  405,   -1,  277,
  452,   -1,   -1,   -1,  282,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,  421,   -1,   -1,  433,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,  443,  444,   -1,  446,  491,
  492,  493,  494,  495,  452,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,  481,  482,   -1,  484,   -1,  450,   -1,
  452,   -1,   59,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  258,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
  277,   44,   -1,   -1,  262,  282,  264,  265,  275,  276,
  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,  299,    0,  277,   -1,   -1,  304,  305,  282,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,  264,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
  277,   -1,   -1,   -1,   -1,  282,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,
   -1,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,  393,   -1,   -1,   -1,
   -1,  389,  399,   91,   -1,   93,   -1,   59,   -1,   -1,
   -1,  408,   -1,  410,  402,   -1,   -1,  405,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,   -1,
   -1,  428,   -1,  421,   -1,   -1,  433,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1, 1349, 1350,  443,  444,   -1,  446, 1355,
 1356,   -1, 1358, 1359,  452,  462,   -1,   -1,   -1,   -1,
 1366, 1367, 1368, 1369,   -1, 1371, 1372, 1373, 1374, 1375,
 1376,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
  452,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,  317,  497,   -1,    0,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   41,  277,
   -1,   44,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   59,   -1,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,  452,
   -1,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   93,   -1,   59,  481,  482,
   -1,  484,   -1,   -1,  402,   -1,   -1,  405,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  421,   -1,  508,  509,   -1,   -1,  427,
  428,  514,  515,  516,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
  452,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,  276,  277,  513,  514,  515,  516,  481,
  482,  284,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,  297,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  258,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,  275,  276,
  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
  297,   -1,   -1,    0,   -1,   -1,  303,  291,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,  302,  372,
  308,   -1,   -1,   -1,   -1,  309,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,  322,  327,
  393,  329,   -1,  331,   41,   -1,  399,   44,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,
  433,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  379,  393,   -1,   -1,   -1,
   -1,  389,  399,   -1,   -1,   93,   -1,   -1,   -1,   -1,
   -1,  395,   -1,  410,  402,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  411,   -1,   -1,
   -1,  428,   -1,  421,   -1,   -1,  433,   -1,   -1,  427,
  428,   -1,  426,    0,  432,   -1,  430,   -1,   -1,  433,
   -1,    0,   -1,   -1,   -1,  443,  444,  441,  446,   -1,
   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   41,  481,  482,   44,  484,   -1,   -1,   -1,
  452,   -1,   59,  491,  492,  493,  494,  495,   -1,  497,
   59,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  258,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,  276,
  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,   -1,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,
   -1,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,  393,   -1,   -1,   -1,
   -1,  389,  399,   -1,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  410,  402,   -1,   -1,  405,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,  277,   -1,
   -1,  428,   -1,  421,   -1,  284,  433,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,  297,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   41,
   -1,  318,   44,   -1,  452,  462,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   59,   -1,   -1,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,    0,  484,  354,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,  284,  500,  501,  502,  503,  504,   -1,   -1,  376,
  508,  509,   -1,  372,   -1,  513,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,  405,   -1,
  399,   -1,   -1,  410,   59,   -1,   61,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,  432,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,  433,   -1,   91,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   45,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,  452,
   -1,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,  276,  277,   -1,   -1,   -1,   -1,
   -1,  389,  284,   -1,   -1,   93,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,  402,   -1,   -1,  405,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  421,   -1,  508,  509,   -1,   -1,  427,
  428,  514,  515,  516,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,  273,   -1,
  275,  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  372,   -1,  297,  481,  482,   -1,  484,   -1,  303,   -1,
   -1,  306,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,  393,  500,  501,  502,  503,  504,  399,   -1,   -1,
  508,  509,    0,   -1,   -1,  513,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,  372,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   59,   -1,  271,   -1,   -1,   -1,   -1,  393,   -1,
   -1,  292,   -1,   -1,  399,   -1,   -1,  285,  403,   -1,
   -1,    0,   -1,   -1,   -1,  410,   -1,   -1,   -1,   -1,
   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,  320,
  308,   -1,   -1,  428,   -1,   -1,   -1,  315,  433,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,  348,  336,  337,
   -1,  339,   -1,  458,   -1,   -1,   -1,  462,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  402,  302,   -1,  405,  419,   -1,
   -1,   -1,  309,   -1,   -1,   -1,  313,  314,   -1,   -1,
   -1,   -1,   -1,  421,  435,  322,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,  333,   -1,  449,  450,
  451,   -1,  453,  454,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
   -1,   -1,   -1,   -1,   -1,   -1,  487,  488,   -1,   -1,
   -1,   -1,   -1,  481,  482,  496,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,  395,  497,
   -1,    0,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,  411,  513,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  426,
  318,  284,   -1,  430,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,   41,  331,  441,   44,   -1,   -1,  336,   -1,
   -1,  339,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   59,   -1,  271,  460,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  299,   -1,  380,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,  389,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,  405,  327,   -1,
  329,   41,  331,   43,   44,   45,   -1,  336,  337,   -1,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,  432,  354,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,  513,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,  443,  444,   -1,  446,  491,  492,
  493,  494,  495,  452,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
   -1,  261,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,
  339,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,  279,
   -1,  271,   -1,   -1,   -1,  354,   -1,  287,   -1,   -1,
   -1,   -1,   -1,   -1,  294,  285,  296,   -1,  298,    0,
   -1,   -1,   -1,  303,   -1,   -1,   -1,  376,   -1,  299,
  310,  380,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
  389,   -1,   -1,   -1,  324,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,  334,  325,  405,  327,   -1,  329,
   41,  331,   43,   44,   45,   -1,  336,  337,   -1,  339,
  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,  432,  354,  355,  288,  357,   -1,   -1,
  292,   -1,   -1,  295,   -1,  444,   -1,  446,   -1,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   93,   -1,   -1,   -1,   -1,  397,   -1,  389,
   -1,   -1,   -1,   -1,  404,   -1,   -1,  407,   -1,  409,
  410,   -1,  402,  335,   -1,  405,   -1,   -1,   -1,  341,
  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,
   -1,  421,   -1,   -1,  434,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,  513,  367,   -1,  369,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,
   -1,  403,   -1,  483,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  415,  484,   -1,   -1,   -1,  498,  499,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,  437,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,  489,   -1,   -1,
  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   41,
  331,   43,   44,   45,   -1,  336,  337,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
  421,   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,    0,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   91,  508,  509,  435,
   -1,   -1,  513,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   59,  481,  482,   -1,  484,  271,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
    0,  497,   -1,  285,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  299,  514,  515,
  516,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   40,   41,   -1,  325,   44,  327,   -1,  329,   -1,  331,
   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,  258,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,
  402,  276,  277,  405,   -1,   -1,  281,   -1,   -1,  284,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,
   -1,   -1,  297,   -1,   -1,  427,  428,   -1,  303,   -1,
  432,  306,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  264,
   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,  372,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,   -1,   -1,   -1,  393,   -1,
  315,   -1,   -1,  318,  399,   -1,   -1,   -1,  403,    0,
  325,   -1,  327,   -1,   -1,  410,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,  433,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,  376,   -1,  458,   -1,  380,   -1,  462,   59,   60,
   61,   62,   -1,   -1,  389,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
  405,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,  376,  452,   -1,   -1,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  405,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,    0,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,  432,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   91,   -1,  508,  509,
   -1,  262,   -1,  513,  514,  515,  516,   -1,   -1,   -1,
  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,    0,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  421,   -1,    0,   -1,   -1,   -1,  427,  262,   -1,   -1,
   41,  432,   -1,   44,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,  444,   -1,  446,   -1,  282,   59,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,    0,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   59,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   41,   -1,   -1,   44,   -1,
   -1,   -1,  513,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  405,  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,  282,   -1,   -1,  285,  421,   -1,   41,    0,
   -1,   44,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,  321,    0,   -1,   -1,  325,   -1,  327,   -1,  329,
   41,  331,   -1,   44,   -1,   -1,  336,  337,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   -1,  262,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,  513,  379,
  380,   59,   -1,   -1,  262,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,  405,   -1,  318,   -1,   -1,
   -1,    0,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,  421,   -1,   -1,   -1,  336,  262,  427,  339,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,  354,  444,   -1,  446,  325,   -1,  327,
   -1,   -1,   41,  331,   -1,   44,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   59,   -1,   -1,   -1,   -1,   -1,  354,   -1,  389,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,    0,  405,  331,   -1,   -1,  376,   -1,
  336,  379,  380,  339,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  389,   -1,  513,   -1,   -1,  427,   -1,  354,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,  444,   41,  446,   -1,   44,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,  427,
   -1,  262,   59,  389,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,  444,   -1,  446,  405,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,  262,   -1,  339,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,  354,  513,   -1,  315,   -1,   -1,  318,  444,   -1,
  446,   -1,   -1,    0,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,  376,   -1,  336,  379,  380,  339,   -1,
   -1,   -1,   -1,   -1,   -1,  513,  389,  315,   -1,   -1,
  318,    0,   -1,  354,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,  405,  331,   41,   -1,   -1,   44,  336,   -1,
   -1,  339,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   59,  262,  427,   -1,  354,  513,  389,  432,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,  405,   -1,   -1,   -1,  376,   -1,
   59,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,  427,    0,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,  315,  405,   -1,  318,
   -1,   -1,   -1,  444,   -1,  446,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,   -1,    0,   -1,  336,   -1,  427,
  339,   -1,   -1,   -1,  432,  262,   -1,   -1,   41,   -1,
  513,   44,   -1,   -1,   -1,  354,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,  376,   -1,   44,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,  513,   -1,   59,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,  405,   -1,  325,   -1,
  327,    0,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   -1,   -1,  513,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,  354,   -1,    0,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,  262,   -1,   -1,   -1,   -1,
   59,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,  513,  432,   -1,   -1,  315,   -1,
   -1,  318,    0,   -1,   -1,   -1,   -1,  444,  325,  446,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,  354,  327,   -1,
   -1,   -1,  331,   41,   -1,   -1,   44,  336,   -1,  262,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   59,  379,  380,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,  513,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  405,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
  427,   -1,  325,   -1,  327,  432,  405,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,  444,   -1,  446,
  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,  427,   -1,
  325,  354,  327,  432,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,  262,  339,  444,   40,  446,   -1,   43,
   -1,   45,   -1,  376,   -1,   -1,  379,  380,   -1,  354,
   -1,   -1,   -1,   -1,   58,   -1,  389,   -1,   -1,   63,
   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,  405,   -1,  379,  380,  513,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,  427,   -1,  325,   -1,  327,  432,
  405,   -1,  331,   -1,  513,   -1,   -1,  336,   -1,   -1,
  339,  444,   -1,  446,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,  427,   -1,  325,  354,  327,  432,   -1,   -1,
  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,  444,
   -1,  446,   -1,   -1,  262,   -1,   -1,  376,   -1,   -1,
  379,  380,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,  405,   -1,  379,  380,
  513,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,  427,   -1,
  318,   -1,   -1,  432,  405,   -1,   -1,  325,  513,  327,
   -1,   -1,   -1,  331,   -1,  444,   -1,  446,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,  427,  339,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,
  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,  286,   43,  513,   45,   -1,  405,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,  418,  311,   -1,  427,
   -1,   -1,  513,   -1,  432,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
  452,   -1,   -1,   -1,  348,   -1,  350,  351,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,  513,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,  409,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,  493,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  505,  506,  507,  508,  509,   -1,  267,   -1,   -1,
  514,  515,  516,   -1,  274,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,  289,
   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  271,
  320,   -1,   -1,  323,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,  272,  364,   -1,  275,   -1,   -1,  321,
   -1,   -1,  372,   -1,   -1,   -1,   -1,  329,   -1,  288,
   -1,   -1,   -1,  292,   -1,  337,  295,   -1,   -1,   -1,
   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,  370,  419,
  372,  373,  374,   -1,   -1,  425,  335,   -1,   -1,   -1,
   -1,  298,  341,  342,  343,  435,  345,  346,  347,   -1,
  349,   -1,   -1,  310,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,  324,  367,   -1,
  369,   -1,   -1,  463,   -1,   -1,   -1,  334,   -1,  421,
   -1,   -1,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,  398,
  490,  491,  492,  493,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  267,  415,   -1,  508,  509,
   -1,   -1,  274,   -1,  514,  515,  516,   40,   -1,   -1,
   43,   -1,   45,   -1,  286,   -1,   -1,   -1,  437,   -1,
  292,  293,  399,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,  472,  473,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,   -1,   -1,   -1,   -1,  452,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  372,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,  493,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,  267,  272,  508,  509,  275,   -1,
   -1,  274,  514,  515,  516,   40,   -1,   -1,   43,   -1,
   45,  288,   -1,  286,   -1,  292,   -1,   -1,  295,  292,
  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
  367,  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  415,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,  492,
  493,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,  272,   -1,  323,  275,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,   -1,  292,   -1,   -1,  295,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,  260,   -1,   -1,  263,   -1,  372,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,  335,
   -1,   -1,   -1,   -1,   -1,  341,  342,  343,  286,  345,
  346,  347,   -1,  349,  292,  293,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  413,   -1,
   -1,  367,   -1,  369,  419,   -1,   -1,   -1,   58,   -1,
  425,   -1,  320,   63,   -1,  323,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  398,   -1,  449,  450,  451,  452,  453,  454,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  463,  415,
   -1,   -1,  360,   -1,   -1,   -1,  364,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,  437,  487,  488,   -1,  490,  491,  492,  493,  494,
  495,  496,  497,   -1,  260,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,  472,  473,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,  298,  489,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,  310,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,  334,   -1,
   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,  493,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,  399,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  409,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
  320,   63,   -1,  323,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,
   -1,   -1,   -1,   58,   -1,  425,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,  493,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,  435,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  260,   -1,
   -1,  263,   -1,  452,   -1,  267,  272,   -1,   -1,  275,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,  292,   -1,   -1,  295,
  292,  293,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,  320,  508,
  509,  323,   -1,   -1,   -1,  514,  515,  516,   -1,  335,
   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,  367,  364,  369,   -1,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,  398,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,  415,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,
   -1,   58,   -1,  425,   -1,  320,   63,   -1,  323,   -1,
   -1,  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,  360,  472,  473,   -1,  364,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,
  492,  493,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  270,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,  298,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,   -1,
  312,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  463,   -1,
   -1,   -1,  334,   -1,   -1,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,  493,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  260,   -1,   -1,  263,   -1,   -1,  391,
  267,  272,   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,  286,
   -1,  292,   -1,   -1,  295,  292,  293,   -1,   -1,   -1,
   -1,   40,  424,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,  443,   -1,  320,   63,   -1,  323,   -1,   -1,   -1,
  452,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,  360,   -1,   -1,  367,  364,  369,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  514,  515,  516,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,  415,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   58,   -1,  425,   -1,
   -1,   63,   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,  472,  473,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,  489,   -1,
  487,  488,   -1,  490,  491,  492,  493,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,    0,   -1,   -1,   -1,  274,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   40,   41,   -1,   -1,   44,  491,  492,  493,  494,
  495,  320,  497,   -1,  323,  500,  501,  502,  503,  504,
   59,   -1,   61,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   91,   -1,   -1,  364,   -1,   -1,  260,   -1,
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
   -1,  490,  491,  492,  493,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,  258,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,  273,   -1,  275,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,  297,   -1,
   -1,  463,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,  493,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,  348,
   -1,   -1,  514,  515,  516,   -1,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,  372,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   58,   -1,  393,   -1,   -1,   63,   -1,   -1,
  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  426,   -1,   -1,   -1,   -1,  360,   -1,   -1,  458,
  364,   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,  417,   -1,  419,  491,  492,  493,  494,
  495,  425,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  435,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,  493,
  494,  495,  496,  497,  260,   -1,  500,  501,  502,  503,
  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,
   -1,  300,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,  493,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,  493,  494,  495,  496,  497,  260,
   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,  508,
  509,   -1,   -1,  274,   -1,  514,  515,  516,   40,   -1,
   42,   43,   -1,   45,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   58,   -1,   -1,  300,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,  419,  491,
  492,  493,  494,  495,  425,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,  435,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,  493,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  267,   -1,  508,  509,   -1,
   -1,   -1,  274,  514,  515,  516,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   40,   -1,  274,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   58,   -1,  292,  293,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,  493,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,  493,  494,
  495,  496,  497,   -1,  267,  500,  501,  502,  503,  504,
   -1,  274,   -1,  508,  509,   -1,   -1,   -1,    0,  514,
  515,  516,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   40,   41,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,   41,
  323,   -1,   44,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,  493,  494,  495,   -1,  497,   91,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,  325,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
  493,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   40,   -1,  274,   43,   -1,   45,   -1,
   -1,   -1,  395,   -1,   -1,   -1,  258,  286,   -1,   -1,
   -1,   58,   -1,  292,  293,   -1,   63,   -1,   -1,   -1,
   -1,  273,   -1,  275,  276,  277,   -1,   -1,   -1,  281,
   -1,   -1,  284,   -1,   -1,   -1,   -1,  430,   -1,   -1,
   -1,  320,  435,   -1,  323,  297,   -1,   -1,  441,   -1,
   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,  348,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  372,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,  393,   -1,   -1,   -1,   -1,  425,  399,   -1,   -1,
   -1,  403,   -1,   -1,   -1,   -1,  435,   -1,  410,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  428,   -1,   -1,   -1,
   -1,  433,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,  458,   -1,  487,  488,
  462,  490,  491,  492,  493,  494,  495,  496,  497,   -1,
  267,  500,  501,  502,  503,  504,   -1,  274,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   40,  286,
   -1,   43,   -1,   45,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,  493,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,  267,   -1,  514,  515,  516,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   40,   -1,  274,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   58,   59,  292,  293,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,  493,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  446,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,  493,  494,
  495,  496,  497,   -1,  267,  500,  501,  502,  503,  504,
   -1,  274,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   40,  286,   42,   43,   -1,   45,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
  493,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  267,
   -1,  514,  515,  516,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   40,   -1,  274,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   58,
   -1,  292,  293,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,  493,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,  493,  494,  495,  496,  497,   -1,  267,  500,
  501,  502,  503,  504,   -1,  274,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   40,  286,   -1,   43,
   -1,   45,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,  298,   -1,   -1,   -1,   -1,  435,   -1,  305,   -1,
   -1,   -1,  309,  310,   -1,  312,   -1,   -1,  315,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  463,   -1,   -1,  334,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,  493,  494,  495,  496,  497,    0,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,    0,   -1,  267,   -1,  514,  515,  516,   -1,   -1,
  274,   -1,   -1,   -1,  391,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   41,   -1,  409,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,  424,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,  320,  434,   -1,  323,
   59,   -1,   61,   -1,   -1,   -1,  443,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   91,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,  493,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,  258,   -1,   -1,
  514,  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,  258,
   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,  273,   -1,   -1,  276,  277,   -1,
   -1,   -1,  281,   -1,  270,  284,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,  297,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  309,  310,   -1,  312,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,   -1,
   -1,   -1,  328,   -1,   -1,   -1,   -1,   -1,  334,  348,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  393,   -1,   -1,   -1,   -1,   -1,  399,   -1,
   -1,   -1,  403,   -1,  393,   -1,   -1,   -1,   -1,  410,
  399,   -1,   -1,   -1,  403,  391,   -1,  272,   -1,   -1,
  275,  410,   -1,   -1,   -1,   -1,  281,  428,   -1,   -1,
   -1,   -1,  433,  409,   -1,   -1,   -1,  292,   -1,  428,
  295,   -1,   -1,   -1,  433,   -1,   -1,   -1,  424,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  434,   -1,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,
   -1,   -1,   -1,  462,   -1,   -1,  452,   -1,  272,   -1,
  335,  275,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,   -1,  292,   -1,
   -1,  295,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,  367,   -1,  369,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  514,  515,
  516,  335,   -1,  398,   -1,   -1,   -1,  341,  342,  343,
   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,
  415,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,   -1,  369,   -1,   -1,   -1,   -1,
   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,
   -1,  415,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,  489,   -1,  491,  492,  493,  494,
  495,   -1,  497,  437,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  452,  514,
  515,  516,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,  489,   -1,  491,  492,  493,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,  452,   -1,   -1,   -1,
  514,  515,  516,   -1,   -1,  461,  462,  463,  464,  465,
  466,  467,  468,  469,  470,  471,  472,  473,  474,  475,
  476,   -1,   -1,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,   -1,  491,  492,  493,  494,  495,
  496,  497,  498,   -1,  500,  501,  502,  503,  504,   -1,
   -1,  452,  508,  509,  510,  511,  512,  513,  514,  515,
  516,  462,  463,  464,  465,  466,  467,  468,  469,  470,
  471,  472,  473,  474,  475,  476,   -1,   -1,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,   -1,
  491,  492,  493,  494,  495,  496,  497,  498,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 516
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
"IIF","SCALAR_ARRAY",
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
"union_view_expr : union_view_expr UNION distinct_noise select_view_expr",
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
"union_expr : union_expr UNION distinct_noise select_expr",
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
"non_reserved_word : USING",
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
		node->nod_type  = nod_list;
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
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 369:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 370:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 371:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 372:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
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
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 382:
{ lex.beginning = lex.last_token; }
break;
case 383:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
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
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 390:
{ yyval = NULL; }
break;
case 391:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 405:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 406:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 407:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 408:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 409:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
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
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
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
case 468:
{ yyval = NULL; }
break;
case 469:
{ yyval = NULL; }
break;
case 470:
{ yyval = NULL; }
break;
case 471:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 472:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 473:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 474:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 475:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 476:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 477:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 478:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 480:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 481:
{ yyval = yyvsp[0]; }
break;
case 482:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 484:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 485:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 486:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 488:
{ yyval = NULL; }
break;
case 490:
{ yyval = NULL; }
break;
case 491:
{ yyval = yyvsp[0]; }
break;
case 492:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 500:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 501:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 502:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 503:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 508:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 509:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 511:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 512:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 513:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 519:
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
case 520:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 522:
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
case 523:
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
case 524:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 531:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 532:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 533:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 534:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 535:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 536:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 538:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 539:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 540:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 552:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 553:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 554:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 555:
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
case 556:
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
case 559:
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
case 560:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 561:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 562:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 563:
{ yyval = yyvsp[-1]; }
break;
case 564:
{ yyval = 0; }
break;
case 568:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 569:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 570:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 574:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 575:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 576:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 577:
{ yyval = 0; }
break;
case 578:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 581:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 582:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 585:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 586:
{ yyval = NULL; }
break;
case 588:
{ yyval = NULL; }
break;
case 589:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 591:
{ yyval = NULL; }
break;
case 593:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 598:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 599:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 600:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 601:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 602:
{ yyval = yyvsp[0];}
break;
case 604:
{ yyval = yyvsp[0];}
break;
case 605:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 606:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 607:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 608:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 609:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 610:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 611:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 612:
{ yyval = 0; }
break;
case 613:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 614:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 615:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 616:
{ yyval = (dsql_nod*) 0; }
break;
case 617:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 618:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 620:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 621:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 622:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 623:
{ yyval = 0; }
break;
case 625:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 626:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 627:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 628:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 629:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 630:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 631:
{ yyval = make_list (yyvsp[0]); }
break;
case 632:
{ yyval = 0; }
break;
case 634:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 635:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 636:
{ yyval = 0; }
break;
case 637:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 638:
{ yyval = 0; }
break;
case 639:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 640:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 641:
{ yyval = yyvsp[-1]; }
break;
case 642:
{ yyval = 0; }
break;
case 643:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 644:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 645:
{ yyval = NULL; }
break;
case 646:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 647:
{ yyval = 0; }
break;
case 648:
{ yyval = yyvsp[0]; }
break;
case 649:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 650:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 651:
{ yyval = 0; }
break;
case 652:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 653:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 654:
{ lex.limit_clause = true; }
break;
case 655:
{ lex.limit_clause = false; }
break;
case 656:
{ lex.first_detection = true; }
break;
case 657:
{ lex.first_detection = false; }
break;
case 658:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 659:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 660:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 661:
{ yyval = 0; }
break;
case 662:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 663:
{ yyval = yyvsp[-2]; }
break;
case 664:
{ yyval = yyvsp[-1]; }
break;
case 665:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 666:
{ yyval = yyvsp[-1]; }
break;
case 667:
{ yyval = yyvsp[0]; }
break;
case 668:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 669:
{ yyval = 0; }
break;
case 670:
{ yyval = make_list (yyvsp[0]); }
break;
case 671:
{ yyval = 0; }
break;
case 673:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 675:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 678:
{ yyval = make_list (yyvsp[0]); }
break;
case 680:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 684:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 686:
{ yyval = NULL; }
break;
case 687:
{ yyval = make_list (yyvsp[-1]); }
break;
case 688:
{ yyval = NULL; }
break;
case 690:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 691:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 692:
{ yyval = yyvsp[-1]; }
break;
case 693:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 694:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 695:
{ yyval = make_list (yyvsp[-1]); }
break;
case 696:
{ yyval = NULL; }
break;
case 698:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 699:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 700:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 701:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 702:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 703:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 704:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 705:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 706:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 707:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 708:
{ yyval = make_list (yyvsp[0]); }
break;
case 709:
{ yyval = 0; }
break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 713:
{ yyval = yyvsp[0]; }
break;
case 714:
{ yyval = 0; }
break;
case 715:
{ yyval = yyvsp[0]; }
break;
case 716:
{ yyval = 0; }
break;
case 717:
{ yyval = yyvsp[0]; }
break;
case 718:
{ yyval = 0; }
break;
case 719:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 720:
{ yyval = 0; }
break;
case 721:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 722:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 723:
{ yyval = 0; }
break;
case 724:
{ yyval = 0; }
break;
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 727:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 732:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 733:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 736:
{ yyval = make_list (yyvsp[-1]); }
break;
case 737:
{ yyval = 0; }
break;
case 738:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 739:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 743:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 744:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 747:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 748:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 750:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 751:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 752:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 753:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 754:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 755:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 759:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 760:
{ yyval = yyvsp[0]; }
break;
case 762:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 765:
{ yyval = NULL; }
break;
case 766:
{ yyval = make_list (yyvsp[-1]); }
break;
case 768:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
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
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 776:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 777:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = yyvsp[-1]; }
break;
case 793:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 804:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 806:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 808:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 810:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 832:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 834:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 835:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 836:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 837:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 838:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 839:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 840:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 842:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 843:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 844:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 845:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 846:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 848:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 849:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 850:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
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
case 855:
{ yyval = make_list (yyvsp[-1]); }
break;
case 856:
{ yyval = yyvsp[-1]; }
break;
case 857:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 858:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
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
