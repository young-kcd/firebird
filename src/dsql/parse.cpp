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
#include "../jrd/intlobj_new.h"

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
#define NEXT 518
#define SEQUENCE 519
#define RESTART 520
#define BOTH 521
#define COLLATION 522
#define COMMENT 523
#define BIT_LENGTH 524
#define CHAR_LENGTH 525
#define CHARACTER_LENGTH 526
#define LEADING 527
#define KW_LOWER 528
#define OCTET_LENGTH 529
#define TRAILING 530
#define TRIM 531
#define RETURNING 532
#define YYERRCODE 256
static short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,   10,   10,   10,   24,   24,   23,   23,
   23,   33,   33,   28,   34,   34,   34,   34,   34,   27,
   27,   32,   32,   29,   16,   16,   16,   37,   37,   38,
   38,   26,   26,   39,   39,   39,   39,   41,   41,   41,
   41,   40,   40,   42,   42,   42,   30,   30,   47,   31,
   31,   48,   48,    7,   49,   49,   51,   56,   56,   56,
   53,   53,   53,   60,   60,   61,   63,   63,   63,   63,
   54,   54,   64,   64,   65,   65,   65,   65,   65,   50,
   67,   67,    6,   69,   69,   69,   69,   69,   69,   69,
   69,   69,   69,   69,   69,   14,   84,   84,   84,   84,
   15,   89,   89,   89,   70,   88,   92,   94,   71,   71,
   74,   74,   74,   82,  101,  101,  101,  102,  102,  103,
  103,  104,  104,  108,  108,   81,  111,  111,  113,  113,
  114,  114,  117,  117,  118,  119,  119,  120,  121,   79,
   83,   80,  105,  105,  124,  125,  125,  127,  127,  128,
  128,  128,  128,  128,  126,  126,  129,  129,  130,  130,
  130,  109,  132,  133,  133,  134,  134,  135,  135,  136,
  136,  136,  107,  107,  107,   76,   86,  137,  137,  137,
  138,  138,  139,  139,  140,  140,  140,  145,   97,   97,
  142,  142,  115,  115,  110,  149,  150,   62,  151,  151,
  116,  116,  116,  116,  116,  116,  143,  143,  158,  158,
  159,  161,  161,  161,  161,  161,  141,  160,  160,  165,
  165,  165,  165,  167,  168,  169,  163,  163,  164,  162,
  162,  162,  162,  162,  170,  171,  172,  172,  172,  172,
   75,   85,   90,  177,  173,  173,  174,  174,  178,  178,
  180,  179,  179,  182,  181,  181,  181,  175,  175,  183,
  183,  184,  186,  186,  187,  185,  185,  189,  189,  188,
  191,  191,  176,  193,  193,  193,  194,  194,  192,  192,
  196,  196,  196,  196,  196,  196,  196,  196,  196,  196,
  196,  196,  196,  196,  196,  196,  197,  197,  197,  197,
  203,  203,  204,  200,  210,  211,  201,  208,  208,  205,
  215,  199,  216,  213,  213,  213,  213,  209,  212,  212,
  207,  207,  207,  214,  214,  195,  195,  220,  221,  221,
  222,  222,  222,  222,  206,  206,  206,  224,  226,  225,
  227,   12,  228,  228,  228,  229,  229,  229,   13,  231,
  231,  232,  232,  233,   78,   87,  147,  237,   98,  100,
  236,  236,   77,   91,  238,  238,  238,  239,  242,  242,
  243,  243,  243,  243,  243,  243,  243,  243,  243,  243,
  243,  243,  243,  243,  243,  240,  240,  241,    2,  245,
  245,  245,  245,  245,  245,  245,  245,  245,  255,  256,
  251,  251,  257,  257,  257,  257,  257,  257,  257,  246,
  246,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  250,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  260,  260,  262,  262,  261,  259,  259,  259,
  252,  252,  253,  253,  253,  254,  266,  266,  267,  267,
  248,  249,  249,  268,  268,  268,  268,  268,  247,  269,
  269,  270,  270,    9,  271,  271,  271,  271,  271,  271,
  271,  271,  271,  271,  271,  271,  271,  112,  112,  144,
  144,  274,  274,  277,  277,  279,  279,   57,   57,  276,
  276,  276,  276,  276,  276,  276,  276,  276,  283,  283,
  275,  275,  275,  275,  285,  285,  284,  284,  284,   59,
   59,  280,  280,  280,  278,  278,  278,  289,  289,  289,
  288,  288,  287,  287,  287,  281,  281,  290,  290,  290,
  291,  291,  282,  282,  282,  282,  292,  292,   20,   20,
   20,  294,  294,  294,   18,   18,   18,  296,  297,  300,
  300,  298,  302,  302,    5,   17,  301,  301,  303,  303,
  304,  304,  293,  305,  305,  306,  306,  307,  307,  307,
  307,  308,  308,  309,  309,  310,  310,  312,  312,  312,
  313,  313,  313,  314,  314,  314,  311,  316,  316,  316,
  317,  317,  315,  315,  318,  320,  320,  319,  319,  295,
    4,    4,    4,  321,  323,  323,  323,  323,  323,  323,
  323,  323,  323,  323,  323,  323,  323,  323,  325,  325,
  326,  322,  322,   19,  327,  327,  329,  329,  328,  328,
  235,  333,  334,  330,  330,  330,  335,  337,  346,  347,
  348,  349,  338,  338,  338,  338,  350,  350,  350,  351,
  351,  351,  339,  339,  340,  340,  353,  353,  354,  354,
  355,  355,  341,  357,  357,  358,  358,  360,  360,  360,
  362,  363,  363,  364,  364,  366,  366,  359,  359,  359,
  367,  368,  369,  371,  371,  372,  373,  361,  361,  374,
  374,  375,  375,   25,  370,  370,  370,  370,  370,  376,
  376,  343,  343,  377,  377,  378,  344,  344,  342,  342,
  345,  345,  379,  380,  380,  380,  380,  380,  381,  381,
  382,  382,  383,  383,  384,  384,  384,  385,  385,  386,
  386,  331,  331,  387,  387,  388,   72,   72,   72,  390,
  390,  389,  389,  332,  332,  332,   11,   11,    8,    8,
  393,  394,   21,   21,  396,  397,  392,  392,  392,  395,
  398,  398,  198,  202,    3,    3,  401,  401,  401,  403,
  403,  404,  402,  402,  405,  405,   35,   35,   96,   95,
   95,  391,  391,  406,  407,  407,  218,  218,  218,  146,
  399,  399,  122,  122,  122,  122,  122,  411,  411,  411,
  411,  411,  411,  410,  410,  410,  413,  413,  413,  413,
  413,  413,  413,  413,  413,  413,  413,  414,  414,  414,
  414,  414,  414,  414,  414,  420,  420,  420,  420,  420,
  420,  420,  420,  420,  420,  420,  420,  420,  420,  420,
  420,  425,  425,  415,  415,  416,  416,  417,  417,  417,
  417,  418,  418,  422,  422,  423,  423,  423,  423,  421,
  424,  419,  419,  409,  409,  409,  412,  412,  412,  426,
  426,  427,   99,   99,   99,   99,   99,   99,   99,   99,
   99,   99,   99,   99,   99,   99,   99,   99,   99,   99,
   99,   99,   99,   99,   99,   99,   99,   99,   99,   99,
  157,  157,  157,  434,  434,  428,  230,  230,  152,  152,
  435,  435,  435,  435,  435,  430,  430,  430,  430,  430,
  234,  153,  153,  154,  155,  155,  155,  155,  155,   55,
   55,   22,   22,  244,  436,   58,  286,  265,  265,  106,
  429,  429,  429,  437,  437,  437,  437,  437,  437,  437,
  437,  437,  437,  437,  438,  438,  440,  441,  441,  441,
  443,  444,  444,  445,  439,  439,  439,  439,  446,  448,
  448,  447,  447,  447,  447,  449,  449,  449,  400,  400,
  431,  432,  432,  450,  450,  450,  451,  451,  452,  452,
  455,  455,  453,  453,  458,  458,  457,  454,  456,  433,
  433,  442,  442,  442,  442,  442,  442,  442,  442,  352,
  352,  336,  336,  156,   52,   68,  131,  148,  408,  166,
  190,  273,   93,   66,  223,  123,   73,  356,  219,  324,
   36,   45,  365,  272,   43,   46,  217,   44,  299,  264,
  264,  459,  459,  459,  459,  459,  459,  459,  459,  459,
  459,  459,  459,  459,  459,  459,  459,  459,  459,  459,
  459,  459,  459,  459,  459,  459,  459,  459,  459,  459,
  459,  459,
};
static short yylen[] = {                                         2,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    8,    8,    5,    1,    0,    1,    2,
    1,    1,    3,    1,    1,    1,    1,    2,    2,    3,
    0,    3,    0,    1,    8,    8,    5,    3,    0,    3,
    0,    1,    1,    1,    3,    3,    3,    2,    2,    2,
    2,    1,    3,    1,    2,    2,    1,    3,    1,    1,
    3,    1,    2,    2,    2,    3,    8,    1,    1,    5,
    0,    1,    3,    1,    3,    3,    0,    2,    2,    1,
    1,    3,    3,    2,    0,    2,    2,    1,    3,    9,
    1,    1,    2,    2,    7,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    4,    2,    2,    2,    2,    2,    2,    2,    1,    0,
    1,    1,    6,    6,    1,    1,    0,    0,    1,    0,
    4,    0,    1,    1,    2,    8,    1,    0,    3,    0,
    0,    1,    1,    2,    1,    1,    1,    2,    6,    1,
    1,    3,    0,    1,    1,    0,    1,    1,    2,    3,
    4,    2,    2,    3,    0,    1,    1,    2,    1,    4,
    3,    3,    1,    0,    1,    1,    2,    3,    4,    0,
    1,    2,    0,    1,    1,    5,    5,    3,    2,    0,
    1,    3,    1,    1,    6,    3,    2,    6,    2,    1,
    2,    2,    2,    0,    1,    1,    2,    0,    2,    0,
    1,    1,    1,    1,    1,    1,    0,    1,    1,    2,
    2,    2,    5,    1,    2,    3,    2,    2,    0,    1,
    1,    1,    1,    3,    4,    8,    4,    0,    6,    1,
    1,    2,    2,    0,    3,    3,    1,    2,    2,    2,
    8,    8,    8,    8,    3,    0,    4,    0,    1,    3,
    4,    1,    3,    2,    3,    3,    1,    1,    0,    1,
    2,    4,    1,    1,    3,    1,    0,    2,    1,    6,
    1,    1,    3,    1,    2,    0,    1,    2,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    3,    1,    3,    8,    9,    5,    8,    6,    3,
    0,    3,    2,    1,    1,    3,    3,    7,    2,    0,
    1,    1,    2,    3,    0,    1,    2,    4,    1,    3,
    2,    2,    2,    1,    1,    1,    1,    2,    2,    5,
    0,    5,    1,    3,    0,    2,    4,    0,    7,    3,
    0,    1,    3,    3,    7,    7,    0,    0,    0,    0,
    3,    0,    9,    9,    1,    1,    0,    2,    1,    1,
    1,    1,    1,    3,    3,    3,    3,    3,    3,    5,
    5,    5,    5,    5,    5,    2,    0,    4,    2,    2,
    3,    2,    2,    3,    3,    2,    2,    3,    3,    6,
    1,    2,    4,    3,    2,    2,    2,    2,    3,    1,
    3,    3,    3,    2,    2,    4,    4,    4,    5,    4,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    0,
    2,    2,    4,    4,    5,    3,    2,    0,    2,    0,
    0,    1,    2,    2,    4,    3,    2,    2,    7,    1,
    0,    1,    0,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    3,    2,    2,    2,    2,    1,    1,    1,
    1,    4,    5,    1,    3,    1,    3,    1,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    4,    4,    6,    5,    3,    0,    2,    2,    0,    3,
    0,    4,    1,    5,    4,    1,    4,    1,    2,    2,
    1,    1,    1,    2,    2,    2,    2,    0,    3,    5,
    1,    1,    2,    3,    1,    2,    3,    0,    1,    1,
    1,    5,    5,    6,    1,    1,    1,    2,    4,    1,
    0,    5,    1,    0,    3,    3,    1,    0,    2,    0,
    1,    0,    3,    1,    0,    1,    2,    1,    1,    1,
    1,    2,    2,    1,    2,    3,    1,    1,    3,    3,
    1,    2,    3,    1,    2,    0,    2,    1,    1,    0,
    1,    1,    1,    3,    2,    3,    0,    1,    3,    4,
    5,    6,    7,    1,    1,    1,    1,    1,    1,    2,
    1,    1,    1,    1,    1,    1,    2,    1,    1,    1,
    2,    1,    1,    3,    3,    0,    2,    0,    2,    0,
    3,    3,    3,    1,    4,    4,    1,    9,    0,    0,
    0,    0,    3,    2,    1,    0,    3,    5,    3,    2,
    5,    2,    1,    1,    1,    1,    1,    3,    1,    3,
    1,    0,    2,    1,    3,    1,    1,    1,    1,    3,
    6,    1,    0,    3,    0,    1,    3,    1,    1,    1,
    4,    5,    5,    1,    1,    2,    4,    4,    2,    3,
    0,    1,    2,    1,    1,    2,    2,    2,    0,    1,
    0,    3,    0,    1,    3,    1,    2,    0,    2,    0,
    2,    0,    4,    1,    2,    1,    1,    0,    1,    3,
    2,    1,    1,    2,    1,    4,    3,    1,    3,    4,
    0,    3,    0,    1,    3,    3,    1,    1,    0,    1,
    1,    4,    0,    2,    4,    0,    9,    5,    1,    1,
    7,    4,    1,    1,    8,    5,    2,    4,    0,    4,
    1,    3,    3,    1,    7,    7,    5,    3,    0,    1,
    1,    1,    2,    0,    1,    1,    1,    0,    3,    1,
    3,    1,    0,    3,    1,    3,    1,    3,    3,    1,
    1,    3,    1,    2,    1,    3,    3,    1,    2,    3,
    3,    3,    3,    1,    3,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    3,    3,    3,
    3,    3,    3,    3,    3,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    1,    1,    5,    6,    5,    6,    3,    4,    5,
    6,    3,    4,    3,    4,    3,    4,    4,    5,    4,
    4,    3,    4,    1,    1,    1,    1,    1,    1,    1,
    3,    3,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    3,    3,    3,    3,    3,    3,
    3,    3,    1,    1,    1,    1,    3,    1,    1,    1,
    1,    2,    2,    3,    0,    4,    1,    3,    1,    2,
    1,    1,    1,    1,    1,    1,    1,    2,    2,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    1,    2,    1,    1,    1,    1,    1,    2,    1,
    1,    1,    1,    4,    5,    5,    5,    5,    5,    5,
    5,    5,    5,    5,    1,    1,    6,    1,    1,    1,
    4,    4,    4,    4,    1,    1,    4,    4,    7,    2,
    0,    7,    6,    6,    4,    1,    1,    1,    4,    3,
    6,    1,    1,    6,    8,    6,    1,    1,    4,    6,
    4,    5,    3,    5,    4,    5,    1,    1,    1,    4,
    6,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    0,    1,    0,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    3,    4,    5,    6,    7,    8,    9,   10,
   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
   21,   22,    0,  589,  590,  591,  595,  596,  597,    0,
  684,  687,  799,  800,  803,  804,  501,    0,    0,    0,
    0,    0,    0,    0,    0,  409,  607,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  129,    0,    0,
    0,  103,    0,  984,    0,   23,  982,    0,    0,   74,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  514,    0,    0,    0,   37,   34,
   36,    0,   35,    0, 1090, 1092, 1093, 1094, 1095, 1096,
 1097, 1116, 1117, 1098, 1100, 1099, 1105, 1101, 1106, 1102,
 1104, 1107, 1103, 1108, 1109, 1110, 1112, 1113, 1111, 1114,
 1115, 1119, 1118, 1120, 1121, 1122,    0,    0,    0,    0,
   32,   69,   67, 1082, 1091,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  695,    0,    0,
    0,  742, 1084,    0,    0,    0,    0,    0,    0,  116,
 1089,  598,    0,    0,    2,    0,    0,    0,    0,    0,
    0,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  452,  453,  454,  455,  456,  457,  458,  459,  460,  461,
  462,  463,  471,  465,  466,  464,  467,  468,  469,  470,
  472,  474,  477,  473,  475,  476,  478,  479,  480,  481,
  482,    0,  441,  442,    0,  410, 1073,    0,    0,  416,
 1077,    0,  413, 1081,    0,  744,    0,  412, 1085,    0,
  417, 1076,    0,  605,  980,    0,  165,  112,    0,  113,
    0,  215, 1069,  840,  104,    0,  110,  160,    0,    0,
  106,    0,  114,  986,    0,  107,    0,  108,    0,  109,
 1088,  161,  115,  111,  787,  788,    0,  985,  983,    0,
   75,    0, 1074,    0, 1072,  522,  515,    0,  521,  526,
  516,  517,  524,  518,  519,  520,  525,  527,    0,    0,
    0,   30,    0,   39,  827,   38,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  606,  990,    0,  971,  689,  689,  690,  700,  702, 1060,
  703,    0,  704,  694,  690,    0,    0,    0,    0,    0,
    0,    0,  624,  613,    0,  616,  618,  619,  620,  621,
  627,  628, 1083,  743,    0,  120,    0,    0,  117,    0,
  118,    0,  119,    0,    0,  654,  655,  662,    0,  661,
  663,  665,  670,  658,  656,  659,  657,  666,  669,  664,
  668,    0,    0,    0,    0,    0,  674,    0,    0, 1062,
    0,    0,  681,    0,    0,    0,    0,    0,  502,    0,
    0,  379,    0,  218,    0,  421,  128, 1065,    0,  418,
  491,  492,    0,    0,    0,    0,    0,    0,  430,    0,
  385,  386,    0,    0,  611,  609,  981,    0,    0,    0,
    0,    0,    0,    0,  168,  147,    0,  125,    0,    0,
    0,  121,    0,  136,  135,    0,    0,    0,    0,    0,
    0,   76,    0,    0,    0,    0,  802,  523,    0,    0,
    0,    0,  946,  977,    0, 1064,    0,  948,    0,    0,
  978,    0,  972,  963,  962,  961,    0,    0,    0,    0,
  951,    0,    0,  964,  965,  973,  974,    0,  975,  976,
    0,    0,    0,  979,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  967,    0,  837,
  943,  944,  945,  950,  949,  928,    0,    0,    0,  927,
    0,    0,  932,  924,  925,  926,  929,  930,  931,  966,
  991,  992,  993, 1005, 1006, 1008, 1009, 1010, 1015, 1016,
 1032, 1033, 1037, 1038,  216,    0,    0,    0,  372,    0,
    0,    0,  830,   27,    0,    0,    0,    0,   72,   70,
 1086,   68,   33,    0,    0,    0,  832,    0,   48,   50,
    0,    0,    0,  603,    0,    0,    0,  697,  699,    0,
  706,    0,    0,    0,  707,  693,    0,  650,    0,  625,
    0,  622,    0,  623,  648,    0,  643,    0,    0,  617,
  841,  811,    0,    0,    0,  126,    0,    0,    0,  600,
  599,  667,  660,    0, 1080,    0,    0,    0,  675,  679,
    0,    0,  784,  686,  685,    0,  183,    0,    0,  144,
    0,  507,    0,  508,  503,    0,    0,  425,  427,  426,
    0,  428,    0,  422,    0,    0,    0,    0,  269,    0,
    0,    0,  434,  435,    0,  484,    0,    0,    0,    0,
    0,    0,    0,  390,  389,  510,    0,    0,    0,  164,
    0,    0,  173,    0,  172,    0,    0,  179,  162,    0,
  177,  169,    0,    0,  546,  581,  549,    0,  582,    0,
    0,  550,    0,    0,    0,  573,  585,  545,  568,  547,
  548,  543,  530,  379,  528,  529,  531,    0,    0,  540,
  541,  542,  544,    0,    0,    0,    0,  124,    0,    0,
  122,    0,  123,    0,  139,    0,    0,  199,    0,    0,
  377,    0,  218,    0,    0,   84,    0,  102,    0,  101,
 1066,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  843,  845,  854,  857,  858,  859,  860,  861,  862,
  863,  864,  865,  866,  867,    0,    0,    0,    0,    0,
  968,    0,    0,    0,    0,    0,    0,  969,  970,    0,
  952,  953,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  333, 1087,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  362,    0,    0,  274,  538,    0,
    0,    0,  370,    0,    0,    0,  829,    0,    0,   44,
   73,    0,    0,   26,    0,  835,    0,    0,  798,    0,
    0,    0,    0,  602,    0,  689,    0,  711,    0,    0,
    0,    0,  593,    0,  988,  592,    0,  626,    0,  634,
  630,  629,    0,    0,    0,  645,  633,    0,    0,    0,
  806,    0,    0,    0,  377,  673,  672,  651,    0,    0,
    0,    0,    0,    0,    0,    0,  145,    0,  506,  379,
  424,    0,  379,  380,  429,  497,    0,  496,    0,    0,
  265,  377,  238, 1070,    0,    0,  379,    0,    0,  207,
  377,    0,    0,    0,    0,  243,  237,  240,  241,  242,
  433,  489,  488,  432,  431,    0,    0,    0,    0,    0,
    0,    0,    0,  388,    0,  379,  494,    0,  493,    0,
  170,  174,    0,    0,  178,    0,    0,    0,  569,  570,
  586,    0,  583,    0,    0,  576,  574,  575,    0,    0,
    0,    0,  539,    0,    0,    0,    0,  577,  127,    0,
    0,  377,    0,  198,    0,  201,  203,  204,    0,    0,
    0,    0,    0,  218,   79,    0,    0,   78,    0,    0,
    0,    0,  844,  856,    0,    0,  917,  918,  919,    0,
  848,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  764,  766,    0,  761,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059,    0,    0,
    0,    0,    0,    0,    0, 1043,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1026, 1028, 1027,    0,
    0,  941,  364,    0,  942, 1030,    0,  937, 1068,    0,
    0,    0,    0,    0,    0,    0,    0,  334,  335,    0,
    0,  947,  839,  838,  374,  373,    0,  272,    0,    0,
    0,  280,  831,    0,    0,    0,   71,    0,    0,    0,
  834,    0,    0,    0,    0,  698,  701, 1078,  710,    0,
    0,    0,    0,  716,  717,  718,  719,  728,  729,  730,
    0,    0,  708,  594,  989,  635,  644,  639,  638,    0,
  649,  842,  812,    0,    0,  377,    0,    0,  652,  671,
    0,  691,  786,  785,    0,  505,    0,    0,  182,    0,
  186,    0,    0,    0,  423,  499,  377,  377,  277,  380,
  270,    0,  209,  379,  211,  379,  380,  206,  212,    0,
    0,    0,    0,  436,  437,  440,  380,  379,  377,  438,
    0,    0,    0,  406,    0,  495,  194,  195,  171,    0,
  181,  557,  558,  987,    0,    0,    0,    0,    0,  584,
    0,    0,  380,    0,    0,  534,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  196,    0,
    0,    0,   83,    0,    0,    0,    0,   91,   85,    0,
    0,   90,   86,    0,  810, 1071,    0,    0,  849,    0,
    0,  855,    0,    0,    0,    0,    0,  893,  892,    0,
    0,    0,  902,  920,    0,  912,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  847,    0,  765,    0,
  801,    0,    0,  218,    0,  994,    0,    0, 1017,    0,
    0,    0,    0,    0,    0,    0,  954,    0,    0,    0,
    0,    0, 1039,    0,    0,    0,    0,    0,    0,    0,
 1050, 1011, 1012, 1013, 1018, 1014,    0, 1025,    0,    0,
  683, 1029,  956,    0,    0,    0,    0,  267,  286,    0,
    0,  369,  281,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   54,   62,   64,    0,   42,    0,    0,    0,
  816,  836,    0,  815,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  745,    0,    0,    0,    0,    0,    0,
    0,  641,  642,  646,    0,    0,  197,    0,  653,    0,
    0,    0,    0,  187,    0,    0,    0,  419,  221,  222,
  223,  224,  225,  226,  959,    0,    0,  271,    0,    0,
    0,    0,    0,    0,    0,  244,    0,  439,  485,  486,
    0,    0,    0,    0,    0,    0,  379,  512,  380,  180,
 1067,    0,    0,  552,    0,  551,  587,    0,  579,    0,
    0,    0,  532,  560,    0,    0,  562,  565,  567,  377,
    0,    0,    0,  134,    0,  202,  379,    0,  378,  105,
    0,  132,    0,   94,    0,    0,    0,    0,   88,   89,
    0,  910,  911,  851,    0,  853,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  913,    0,    0,    0,    0,
    0,    0,    0,  903,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  772,
    0,    0,    0, 1000,  999,    0,    0,  996,  995,    0,
 1004, 1003, 1002, 1001,  998,  997,    0,    0, 1045,    0,
    0,    0, 1044,    0,    0,    0,    0,    0,    0,    0,
  367,  337,  336,  273,    0,    0,  283,  284,    0,    0,
    0,    0,  316,    0,    0,    0,  315,    0,    0,    0,
    0,  361,  304,  302,  306,    0,  303,  292,  297,  291,
    0,    0,    0,  300,  301,  305,  307,  308,  309,  310,
  311,  312,  313,  314,  317,  318,  319,  320,    0,    0,
  355,  356,  357,    0,  814,   66,   58,   59,   65,   60,
   61,    0,   24,    0,    0,   25,  822,  821,    0,  820,
  818,  826,  825,  823,    0,   45,   46,    0,  720,    0,
    0,    0,  750,  748,  746,    0,  747,    0,    0,    0,
    0,    0,  805,    0,  378,  790,  791,  692,    0,  192,
  188,    0,  380,  960,  275,  276,  380,    0,  149,    0,
    0,  229,    0,    0,  245,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  509,  554,    0,  555,    0,    0,
    0,    0,  153,  155,  156,  157,  537,  535,  533,  564,
    0,    0,  380,    0,    0,    0,  375,  379,   92,    0,
    0,   98,   93,    0,    0,    0,    0,    0,  921,    0,
  922,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  774,  763,    0,    0,  775,    0,  771,  217, 1031,
 1051, 1007,    0,    0, 1041, 1040,    0, 1046, 1034, 1036,
    0, 1023, 1024,    0,    0,  282,    0,    0,    0,    0,
    0,  343, 1079,  358,  359,    0,    0,  293,    0,  298,
    0,  346,  299,    0,    0,  339,    0,   55,   56,   57,
   63,    0,    0,  797,    0,  740,  738,    0,  731,    0,
    0,    0,  754,    0,  688,  380,  376,  792,  189,  380,
  159,  264,    0,    0,  205,  230,    0,    0,    0,    0,
  231,  234,    0,    0,  380,  404,  405,  401,  400,  403,
  402,    0,  553,  580,  158,  146,  154,    0,  379,  261,
  141,  380,  381,    0,    0,   96,    0,   80,    0,  879,
  887,  682,    0,  880,  888,  883,  891,    0,    0,  881,
  889,  882,  890,  876,  884,  877,  885,  878,  886,  770,
    0,    0,    0, 1019, 1042,    0, 1022,    0,    0,  289,
  285,    0,    0,    0,    0,  330,    0,    0,  354,    0,
    0,    0,    0,  349,  347,    0,    0,    0,   40,  817,
    0,    0,  722,  732,    0,    0,  733,  734,  735,    0,
  262,  420,  208,  213,  232,    0,    0,  235,    0,  247,
  249,  408,  380,    0,  383,    0,   77,   99,  100,    0,
    0,    0,  777, 1035,  219,  288,    0,    0,    0,    0,
  353,  352, 1075,  351,    0,    0,    0,    0,    0,    0,
    0,  721,    0,    0,  755,  236,    0,    0,    0,    0,
    0,  263,  380,    0,    0,  776,    0,    0,    0,    0,
    0,  348,  350,    0,    0,    0,    0,  726,    0,    0,
    0,    0,    0,  246,    0,  253,    0,  252,  384,  133,
  779,    0,  290,    0,    0,    0,    0,    0,    0,  724,
  737,  233,    0,    0,  257,  256,  255,  780,    0,    0,
    0,    0,  338,  727,  260,  258,  259,  328,    0,  344,
  325,  326,
};
static short yydgoto[] = {                                      21,
   22,   23,   24,   25,   26,   27,   28, 1583,   30,   31,
 1584, 1585,   34,   35,   36,   37,   38,   39, 1586,   41,
 1587, 1627,  147,  565,  172, 1370, 1623,  148,  839,  149,
  568,  844,  150,  151,  314, 1131,  161,  162, 1371, 1372,
 1373, 1374,  247,  279,  152, 1375,  153,  570,   90,  291,
  462,  517,  744, 1246,  518,  997,  713,  272,  973,  745,
  746,  747, 1253, 1248, 1713,  292,  749,  750,   82,  265,
   83,  287, 1930, 1480,  271,  276,  278,  280,  267,  258,
  260,  273,  283,  180,  369,  371,  373,  366,  452,  731,
  733,  728,  235,  236,  562,  315,  916,  647,  760, 1175,
  456,  736, 1237, 1474,  681,  865, 1209,  639,  640,  662,
  447,  714, 1187, 1691, 1815, 1418, 1692, 1693, 1694, 1695,
 1696,  761,  268,  259,  443,  689,  444,  445,  690,  691,
 1450,  641, 1169, 1170, 1171, 1413,  458,  985,  986,  987,
  988,  918, 1670,  715,  920,  520,  990, 1088,  556, 1537,
 1880, 1419,  521,  522,  523,  524,  525, 1671, 1672,  665,
 1821, 1959, 1436,  926,  927,  913,  928,  929,  930, 1960,
 1961, 2006,  424,  561, 1110, 1588,  243,  658, 1107,  659,
 1180,  557, 1111, 1112, 1360, 1566, 1567, 1568, 1881, 1255,
 1589, 1590, 1591, 1592, 1781, 1593, 1594, 1595, 1596, 1597,
 1598, 1599, 1600, 1601, 1602, 1603, 1604, 1605, 1606, 1607,
 1608, 1609, 1097, 1997, 1886,  526,  814,  527, 1610, 1782,
 1893, 1894, 1942, 1611, 1612, 1613, 1776,  528,  825,  529,
  311,  558,  559,  530,   43, 1479, 1707,  433,  676,  946,
 1448,  677,  944,   87,   66,  428,  248,  191,  408,  232,
  415,  240,  251,  420,  904,  901,  416,  429,  934,  430,
  672, 1200,  233,  531, 1224,  656,  908,  409,  678, 1449,
  105,  246,  296,  716,  717,  829, 1225,  830, 1226,  720,
  721,  722,  723,  958, 1218, 1216,  724,  725,  726,  966,
  727,  963,   44,   45,   46,   47,   48,   49,  182,  621,
   68,  585,  254,  436,  354,  355,  356,  357,  358,  359,
  360,  361,  362,  871,  606, 1150, 1404,  607,  608,  876,
  392,  888,  393,  626,  394,  891,  187,  397,  629,   50,
  190,  403, 1503,  813,   51,  401,   52,  166,  342,  593,
  861,  466, 1401, 1652,  777,  588,  344, 1410, 1808,  167,
  168,  343,  594,  595,  859, 1129, 1132, 1133, 1134, 1135,
 1136, 1137, 1902, 1952,  532, 1979, 1138, 1139, 1140, 1399,
 1907, 1908, 1909, 1391,  175, 1644, 1802, 1803, 1530, 1035,
 1531, 1532, 1533, 1748, 1931, 1933,  632,  633, 1163, 1658,
  576, 1794,   53,   54,  467,   55,   56,  614,  615,  533,
 1119, 1381, 1629, 1630, 1634,  577,  847,  264,  762,  763,
 1012, 1013,  764,  765,  766,  767,  768,  769,  770,  771,
  772,  773,  774,  775, 1271, 1273, 1274,  534,  535,  536,
  537,  538,  539,  791,  540,  289,  541,  542,  543,  544,
  545, 1059,  546,  547,  548,  549,  550, 1754, 1081,  551,
  552,  553,  554,  796, 1064, 1336, 1331,  797,  155,
};
static short yysindex[] = {                                  16918,
 2229, -100, 3126,   70, 1011,  374, 3426,  -39,17022,   23,
  436,  -57, -100, -134,  759,12227,  672,12227,  290,  531,
    0,  852,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  649,    0,    0,    0,    0,    0,    0,  366,
    0,    0,    0,    0,    0,    0,    0,17965,12227,  657,
12227,12227,12227,12227,12227,    0,    0,  619, 1297,12227,
12227,12227,  802,12227,  693,12227,12227,    0,12227,12227,
12227,    0,  511,    0,  715,    0,    0,  863,12227,    0,
12227,12227,12227,  881,12227,12227,12227,12227,  693,12227,
12227,12227,12227,12227,    0,12227, 1070,  842,    0,    0,
    0, 1200,    0, 1200,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  875,  916,  139, 1262,
    0,    0,    0,    0,    0,12227,12227,12227,  942,  975,
 1727,12227,  481,   93,  211,   62,  918,    0,12227, 1150,
 1230,    0,    0,12227, 1023,12227,12227,12227,12227,    0,
    0,    0,12227, 7688,    0, 1084, 1083, 1296,   69, 1022,
 1422,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1014,    0,    0, 1297,    0,    0, 1128,   33,    0,
    0, 1603,    0,    0, 1287,    0,  517,    0,    0, 1102,
    0,    0, 1239,    0,    0, 1204,    0,    0, 1178,    0,
 1395,    0,    0,    0,    0, 1297,    0,    0,  901, 1603,
    0,  490,    0,    0, 1356,    0, 1362,    0, 1200,    0,
    0,    0,    0,    0,    0,    0, 1346,    0,    0, 1128,
    0, 1351,    0, 1241,    0,    0,    0, 1128,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,15984,12227,
 1284,    0,12227,    0,    0,    0, 1272, 1298,11169,12227,
 1252, 1369, 1680, 1401, 1409, 1418, 1361, 1363,  157, 1243,
    0,    0,16240,    0,    0,    0,    0,    0,    0,    0,
    0,13936,    0,    0,    0, 1317,12227, 1404, 1323,  845,
12227, 1355,    0,    0, 1230,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12227,    0, 1297, 1603,    0, 1356,
    0, 1200,    0, 1410, 1393,    0,    0,    0, 1478,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1464,12227,12227, 1439, 1334,    0,16240, 1424,    0,
 1424,16240,    0,  -36, 1347, 1338, 1386, 1422,    0, 1593,
 1231,    0,12227,    0, 1014,    0,    0,    0, 1622,    0,
    0,    0,12227, 1284,10892, 1468,13029, 1887,    0,17965,
    0,    0, 1254, 1487,    0,    0,    0, 1874, 1874, 1297,
 1573, 1297,  -26, 1178,    0,    0,11548,    0,12227,12227,
12227,    0, 1284,    0,    0, 1657,  404, 1901,12227, 1679,
12227,    0, 1904,10370, 9984, 1557,    0,    0, 1910, 1911,
 1913, 1504,    0,    0, 1924,    0, 1925,    0, 1927, 1928,
    0, 1929,    0,    0,    0,    0,    0, 1931, 1519, 1521,
    0, 1936, 1936,    0,    0,    0,    0, 1938,    0,    0,
14192, 1940, 1943,    0, 1944, 1640, 1952, 1953, 1954, 1956,
 1964, 1965,16240,16240,14448,12227, 1967,    0, 1776,    0,
    0,    0,    0,    0,    0,    0, 1917, 1609, 1973,    0,
    0, 1974,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,11548, 1960,  473,    0, 1987,
 1765, 1060,    0,    0,12227,12227,12227,   56,    0,    0,
    0,    0,    0,12227,12227, 1046,    0,12227,    0,    0,
 1272, 1627,11169,    0,12227,14448, 1751,    0,    0,16240,
    0, 1446, 1713, 1988,    0,    0,  165,    0, 1120,    0,
  -92,    0,  -92,    0,    0, 1990,    0,  177, 1611,    0,
    0,    0, 1989,  117, 1975,    0, 1284, 2002, 1780,    0,
    0,    0,    0,  459,    0, 1708, 2005,12227,    0,    0,
 1552, 2004,    0,    0,    0, 1196,    0, 1710, 1711,    0,
 1297,    0, 1712,    0,    0, 1781, 1786,    0,    0,    0,
 1755,    0,11548,    0, 1297, 1700,11548, 1587,    0, 1805,
12227,17720,    0,    0,  -38,    0,12227,  960, 1287, 1676,
 1643,   84,    0,    0,    0,    0,  685, 1682,  179,    0,
 1618,  693,    0, 1297,    0, 1798, 1730,    0,    0,  -26,
    0,    0,  166, 1637,    0,    0,    0, 1638,    0, 1687,
 2040,    0, 1736, 2046,  714,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1997,  223,    0,
    0,    0,    0,  216, 2050, 2051, 2046,    0, 1297, 1603,
    0, 1777,    0, 1829,    0, 1297, 1297,    0,10892,  517,
    0, 1718,    0, 1692, 2052,    0, 5793,    0, 1723,    0,
    0, 1742, 2070,10240, 2081,    0,    0,    0, 9728, 8005,
   34,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  932, 1753,  454,16240,  700,
    0,12227,16240,  492,  937, 1061, 2515,    0,    0,  693,
    0,    0,16240,10496, 1776, 1691,  710,16240,16240,10496,
 1807,16240,16240,16240,16240,16240,10752,  825,  825, 1971,
 1678,  366, 2096,    0,    0,14704,12227,16240,16240,16240,
16240,16240,16240, 2344,    0,16240, 7220,    0,    0, 1863,
 2076,12227,    0,12227, 1844,12227,    0, 1715, 1719,    0,
    0, 1689,11169,    0, 1834,    0, 1702, 2118,    0, 1834,
12227,12227, 2115,    0, 1971,    0, 2042,    0,12227, 7429,
 1716,16240,    0, 1107,    0,    0,   40,    0, 1725,    0,
    0,    0,12227,  442,12227,    0,    0,12227,12227, 1557,
    0,16240, 1902,10892,    0,    0,    0,    0,  459,12227,
 1831, 2126, 1677,16240,16240, 1297,    0,  432,    0,    0,
    0, 2132,    0,    0,    0,    0, 1297,    0,  171,12227,
    0,    0,    0,    0, 1900, 2134,    0, 1878, 1896,    0,
    0, 1838, 1839, 1200, 1905,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  693,12227, 1883, 1755,17818,
 1803, 1810, 1811,    0,  693,    0,    0, 1129,    0, 1398,
    0,    0, 1778, 1297,    0,10370,   83, 1783,    0,    0,
    0,  693,    0, 2040,   85,    0,    0,    0, 1878,   85,
 1782,   85,    0, 2146,  693,  693,  693,    0,    0, 1284,
12227,    0, 1845,    0, 1717,    0,    0,    0, 1254, 1424,
12227, 1728,  251,    0,    0, 2164,  660,    0,10370,12227,
 1424,11018,    0,    0, 1424,10240,    0,    0,    0,11246,
    0,  174,  491,16240,16240,11285, 2166,  791,11552,16240,
11819,  961,12086,12353,14960,12620,12887,13154,10496,10496,
    0,    0, 1836,    0, 2167, 1022,16240,16240, 3455,16240,
 2168,16240, 2169, 2066,16240,16240,16240,16240,16240,16240,
    0,    0,    0,    0,    0,    0,    0,    0, 1897, 2175,
 1396,   -7,16240,  756,16240,    0,10496, 1531, 2056,  668,
12227, 2221, 2312, 2327, 2356, 2374,    0,    0,    0, 1467,
15216,    0,    0, 1022,    0,    0, 1733,    0,    0, 1933,
  825,  825,  772,  772,  289,10627, 2173,    0,    0, 2176,
 1776,    0,    0,    0,    0,    0, 1735,    0, 1785, 1957,
 1844,    0,    0,11960,11960, 1852,    0,  393, 1870,12227,
    0,16240, 1870, 1919, 1921,    0,    0,    0,    0, 2422,
 2194, 2191,  617,    0,    0,    0,    0,    0,    0,    0,
10496, 1914,    0,    0,    0,    0,    0,    0,    0,  667,
    0,    0,    0, 1753, 1776,    0, 1758, 1424,    0,    0,
  459,    0,    0,    0, 1776,    0, 1874, 1979,    0,  432,
    0, 2200,10496, 5227,    0,    0,    0,    0,    0,    0,
    0, 1844,    0,    0,    0,    0,    0,    0,    0, 1200,
 1200, 1748, 2206,    0,    0,    0,    0,    0,    0,    0,
  360,  104,  528,    0, 1985,    0,    0,    0,    0,12227,
    0,    0,    0,    0,   70, 1791, 1837, 1863, 2209,    0,
 1618, 1802,    0, 2196, 1186,    0,12227, 1271,  693, 2211,
 2214, 2216, 1994,  517, 1844, 1874, 1711,10892,    0, 1682,
 1814, 6983,    0,  693, 1872, 1959, 5793,    0,    0,  693,
 -141,    0,    0, 1969,    0,    0, 2235, 2237,    0,10496,
10496,    0,10496,10496, 2158, 1776, 2230,    0,    0, 1776,
 2239,14448,    0,    0, 1963,    0, 1044, 2241, 1776, 2251,
 1367, 2253, 1776, 2259,16240,16240, 2166,16240,15472, 2265,
 1776, 2266, 2268, 1776, 2270,16240, 1776, 2272, 1776, 2274,
 2275, 1776, 2276, 2282, 1776, 2283,    0, 2065,    0,17887,
    0, 2429, 2437,    0, 2445,    0, 2735,16240,    0, 2750,
 2844, 2904, 2916, 2923, 2931,16240,    0,16240,16240, 1776,
 1903,16240,    0,16240, 1776, 2026,   68,16240,16240,16240,
    0,    0,    0,    0,    0,    0,16240,    0,16240, 1513,
    0,    0,    0, 1804,10627, 9858,12227,    0,    0,12227,
17527,    0,    0,12227,12227,12227,12227,12227,12227, 1888,
 2296, 2297,    0,    0,    0, 1888,    0,  196,  196,   61,
    0,    0, 1813,    0,11960,11960, 2301,  617, 2303,16240,
 2085, 7429, 1983,    0, 1983, 1098, 1983, 2006, 2013, 2083,
 2033,    0,    0,    0, 1022, 1844,    0, 1814,    0, 1115,
 1618, 1981, 1618,    0,10496,  339, 1387,    0,    0,    0,
    0,    0,    0,    0,    0, 5227, 5227,    0, 1957,16240,
 5227, 2079, 1968, 1748,  511,    0,10496,    0,    0,    0,
 1996, 1998, 1999, 2000, 2003, 2009,    0,    0,    0,    0,
    0, 2325,   70,    0, 1923,    0,    0,   85,    0, 2011,
   85,   85,    0,    0, 1863, 2340,    0,    0,    0,    0,
 1254, 1957, 1618,    0, 1711,    0,    0, 2109,    0,    0,
 2126,    0, 2354,    0, 2355, 1297,    2, 2363,    0,    0,
 1297,    0,    0,    0, 2065,    0, 2065,16240, 1424, 1424,
 1847,  366, 2364,16240, 2078,    0, 1424, 1424,16240, 1424,
 1424, 2673, 1776,    0, 1454,16240, 1776, 1424, 1424, 1424,
 1424, 1776, 1424, 1424, 1424, 1424, 1424, 1424,12227,    0,
 2366, 2367,  815,    0,    0,11548, 2372,    0,    0, 2994,
    0,    0,    0,    0,    0,    0, 3096, 1427,    0,16240,
 2103, 2007,    0,16240, 3131, 1848, 3396, 3209, 3217,16240,
    0,    0,    0,    0,11548, 2365,    0,    0, 2133,    0,
12227,  527,    0, 2385, 2091,16240,    0,    0,12227,12227,
12227,    0,    0,    0,    0, 2093,    0,    0,    0,    0,
 2136,17392, 2378,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   52, 2373,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2121,    0,11960,11960,    0,    0,    0, 2018,    0,
    0,    0,    0,    0, 1907,    0,    0, 2085,    0, 1854,
12227,  617,    0,    0,    0, 2108,    0, 7429, 7429,16240,
10496, 1557,    0, 1957,    0,    0,    0,    0, 1398,    0,
    0,  443,    0,    0,    0,    0,    0, 1776,    0, 2171,
 2079,    0, 1154,12227,    0, 2116,  460, 2016, 2120, 2021,
 2159, 2122, 2160, 1844,    0,    0, 2418,    0, 2423, 2119,
 2171, 2011,    0,    0,    0,    0,    0,    0,    0,    0,
 1844, 1682,    0, 1398, 1985, 2088,    0,    0,    0, 2098,
  344,    0,    0, 1863, 2104, 1776, 2432, 2436,    0, 1022,
    0, 1776,16240, 2442, 2444, 1776, 2448, 2452,16240,16240,
 1776, 2455, 2456, 2460, 2461, 2464, 2470, 2471, 2474, 2475,
 2480,    0,    0,17887, 2482,    0,12227,    0,    0,    0,
    0,    0,16240, 2484,    0,    0,16240,    0,    0,    0,
16240,    0,    0, 3294,  189,    0, 2208,16240,16240,10496,
 1776,    0,    0,    0,    0,12227,10627,    0,   81,    0,
 2086,    0,    0,  -18, 2488,    0, 2154,    0,    0,    0,
    0,  196,16240,    0,12227,    0,    0, 7429,    0, 1209,
 1776, 2489,    0,   34,    0,    0,    0,    0,    0,    0,
    0,    0, 2494,12227,    0,    0, 2189, 2199,12227, 1748,
    0,    0, 1200,12227,    0,    0,    0,    0,    0,    0,
    0, 1957,    0,    0,    0,    0,    0, 1957,    0,    0,
    0,    0,    0,16240, 1297,    0, 2082,    0, 1297,    0,
    0,    0, 1776,    0,    0,    0,    0, 1776, 1776,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12227, 2210, 1776,    0,    0, 3306,    0, 5227, 5227,    0,
    0, 2504, 1776, 1210,  513,    0, 2212, 2173,    0,12227,
12227,   70,  241,    0,    0, 2049, 2215,10496,    0,    0,
  133, 2512,    0,    0,10496, 2513,    0,    0,    0,16240,
    0,    0,    0,    0,    0, 1748, 1200,    0, 2178,    0,
    0,    0,    0, 1985,    0, 1776,    0,    0,    0, 2511,
 2517, 2521,    0,    0,    0,    0, 1424,10627, 2139,10627,
    0,    0,    0,    0,17527,   81,16240,10627,  553,10627,
12227,    0,   34,12227,    0,    0, 2178,  445, 1748, 2192,
 2198,    0,    0, 2529,12227,    0,12227, 2534, 2173,17527,
 2173,    0,    0, 1385,  190, 2279, 2173,    0, 1860, 1871,
 1748,  506,  506,    0, 2278,    0, 2148,    0,    0,    0,
    0, 2543,    0, 2284,10627, 2300, 2285,17527,12227,    0,
    0,    0, 2137,  796,    0,    0,    0,    0,17527,  426,
12227,17527,    0,    0,    0,    0,    0,    0,17527,    0,
    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  265,  909,    0,    0,    0,    0,    0,    0,    0,
    0,17098,  247,13417,    0,    0,    0,    0,    0,    0,
    0, 2587,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  213,    0,    0,    0,    0,    0,    0,  282,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  755,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2258,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   -6, 2217,    0,    0,
    0,  205,    0,  205,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2219,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  755,    0,    0,15728,13680,    0,    0,    0,
  902,    0,    0,  126,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  945,    0, 2197,  371,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   12,    0,    0,    0,    0,  737,    0,    0,    0,
    0,    0,  943,    0,    0,    0,    0,    0,  128,    0,
11815,    0,    0,    0,    0,    0,    0,    0,    0,   12,
    0,  -31,    0,    0, 2558,    0,    0,    0, 2336,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  132,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  129,    0,
 2337,    0,    0,    0,    0,    0,12494,    0,    0,    0,
    0,    0, 1193,    0,    0,    0,    0,    0,    0,12761,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  770,    0,    0, 1033,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   12,    0, 2558,
    0, 2336,    0, 1131,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  370,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1137,    0, 2331,
    0,    0,    0,    0, 1168,    0,    0,    0,  307,    0,
    0,    0,    0, 2337,  -16,18046,    0, 1205,    0,    0,
    0,    0,  280,    0,    0,    0,    0, 2153, 2153,    0,
    0,    0, 1215,  138,    0,    0,    0,    0,    0,    0,
    0,    0, 2337,    0,    0, 1373,    0,    0,    0,    0,
    0,    0, 9435,    0,    0,  119,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2725,    0,    0,    0,
    0, 2242, 2242,    0,    0,    0,    0, 3325,    0,    0,
    0, 3625, 3925,    0, 4225, 4525,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1059,    0,
    0,    0,    0,    0,    0,    0, 4825, 1245,  225,    0,
 3025,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1267,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12494,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10114,    0, 2294,    0,    0,    0,    0,    0,    0,
  335,    0,  335,    0,    0,  778,    0,  965, 1065,    0,
    0,    0,    0,  132,    0,    0, 2337,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  198,  496,    0,    0,    0,  680,    0,    0, 1228,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1332,    0,    0,    0,    0,
    0,    0,    0,    0, 2331,    0,    0,  900,    0,  595,
    0,    0,  684,    0,    0,    0,    0,  286,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1339,
    0,    0, 1918, 9023,    0,    0,    0,16761,    0,    0,
16810,    0,    0,16982,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2184, 3316,    0,
    0,    0,    0,17040,16773,    0,16982,    0,    0,   12,
    0,    0,    0,    0,    0,    0,    0,    0,  -16, 1391,
    0,    0,    0,    0, 2220,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1327, 7815, 8012,    0,    0,
  152,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2579,  123,16496,    0,16496,
    0,    0,    0,16496,16496,16496,    0,    0,    0,    0,
    0,    0,    0,    0, 2182,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6014, 6303, 2583,
    0,   77,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1809,
    0,    0,    0,    0, 2358,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  327,    0,    0,    0,    0,  327,
    0,    0, 1349,    0,    0,    0,    0,    0,    0,    0,
  444,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  732,    0,    0,    0,    0,    0,  119,
    0,    0,    0,  -16,    0,    0,    0,    0,    0,    0,
    0,  398,  204,    0,    0,    0,    0,  951,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1873,    0,
    0,    0,    0,    0, 2589,    0,    0, 1275, 2288,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  281,  419,  504,    0,    0,    0,    0,    0,    0,  337,
    0,    0,    0,    0,    0,    0,    0, 2233,    0,    0,
    0,    0,    0,16810,    0,    0,    0,    0,  859,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2337,
    0,    0,  172,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 6082,    0,    0,    0,  168,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2590,    0,    0, 1397,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2593,    0,    0,    0,    0,    0, 5125,
 6592, 6881, 5425, 5725,    0,    0, 1405,    0,    0,    0,
 1206,    0,    0,    0,    0,    0,    0,    0,13292,    0,
 2369,    0,    0,    0,    0,    0,    0,    0, 1425,    0,
    0,    0, 1425,    0,    0,    0,    0,    0,    0,    0,
 7090,  287,  641,    0,    0,    0,    0,    0,    0,    0,
    0,  643,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  123,  184,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  720,    0, 2153, 2186,    0, 1040,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2358,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1390,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1426,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3016,    0,    0,
    0,    0,    0, 1352,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1391, 2358, 2153, 1435,  -16,    0, 2376,
 1457,    0,    0,    0, 6082,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1670,    0,    0,    0, 1893,
    0,    0,    0,    0,    0,    0,    0,    0, 1982,    0,
 6802,    0, 8084,    0,    0,    0,    0,    0,    0,    0,
 8156,    0,    0, 8216,    0,    0, 8311,    0, 8383,    0,
    0, 8455,    0,    0, 8515,    0,    0, 9420,    0, 2579,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2218,
    0,    0,    0,    0,  764,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  771,    0,    0,    0,    0,    0,    0,    0,  194, 1479,
  131,  140,    0,    0,    0, 1479,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2298, 1136,    0,
 7514,    0, 2307,    0, 2307, 2298, 2307,    0,    0,    0,
 1031,    0,    0,    0, 1397, 2358,    0, 1457,    0,    0,
    0, 2203,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2019,    0, 1390, 2258,    0,    0,    0,    0,    0,
  698,  786,  887,  896,  939,  940,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  801,
    0,    0,    0,    0, 3616,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1495,    0,    0,    0,    0,    0,
 1522,    0,    0,    0,    0,    0,  145,    0,    0,    0,
    0,    0,    0,    0,  130,    0,  295,    0,    0,    0,
    0,   77,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 8587,    0, 8671,    0, 8754,    0,    0,    0,
    0, 8826,    0,    0,    0,    0,    0,    0,  995,    0,
    0, 2601,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2607,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 6643,
 2591,    0,    0,    0,    0,    0,    0, 1575, 1694,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  818,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1630,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1532,    0,    0, 7305,    0,    0,
    0,  934,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  203,    0,    0,    0,    0,    0,    0,  914,    0,
    0,    0,    0,    0,    0,    0,    0, 2608,    0, 1747,
 2912,    0, 2331,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2358,    0,    0,    0,    0,    0,    0,
 1534,  819,    0,    0,    0,    0,    0,    0,    0,    0,
 2358, 2376,    0,  349,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  811,    0, 8886,    0,    0,    0, 2593,
    0, 8997,    0,    0,    0, 9030,    0,    0,    0,    0,
 9119,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2579,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2598,    0,    0, 2603,    0,    0,
 2606,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2362,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 9517,    0,    0,    0,    0, 2298,
  908,  953,    0, 1079,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1582,
    0,    0,  176,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  185,    0,    0,    0,
    0,    0, 9191,    0,    0,    0,    0, 9290, 9323,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1875, 2627,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2612, 2613,    0,    0,    0, 2614,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1537, 9550,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1582, 1480,    0,  164,    0,
    0,    0,    0,    0,    0, 2608,    0,    0,    0, 2635,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  348,    0,    0,    0,    0,    0,
    0,    0, 9639,    0,    0,    0, 1007,    0, 1390, 1671,
 1763,    0,    0,    0,    0,    0,    0,    0, 2618,  348,
 2619,    0,    0,    0, 2381,    0, 1561,    0,    0,    0,
 1582,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,17276,    0,    0,    0,  348,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  348,    0,
    0,  348,    0,    0,    0,    0,    0,    0,  348,    0,
    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, 2679,    0,    0,
 2683, 2684,    0,    0,    0,    0,    0,    0,    1,    0,
 2689,   -2, 2530, 2111,   22, -896, 1314, 2532, 1846, 2541,
 2113,    0,    0, 2384, -106,   37,    0,    0,    0,    0,
  236,  275,  -59,  -83,  -60, -284, 2387, 1865,    0,    0,
    0, 1103,    0,    0,  -33, 1462, -641,  -71,-1097, 1986,
 1734, -377,    0, 1465,    0, 2622, 1731, 1775,    0,    0,
    0, -599,  -58,    0,    0,    0,    0,    0, 2651,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -66,    0, -616, -732, 1496, -395, 1104,-1043,
    0,    0,    0,    0, -405, -151,-1354, 1503, -417,  -55,
    0, -633, 1772,    0, 1056,-1269,    0, 1057,    0,    0,
 2342, -736,   38,    0,    0,    0,    0, 2309,    0, 2073,
 1546,    0,    0,    0, 1605,    0, 2409, 1906, 1543, 2375,
 2380,    0,    0, -512, 1869,  -49, -746,  980, -311,    0,
    0,    0,-1129,-1114,-1073,-1066,-1038,    0, 1118,-1205,
    0,  841,-1393, 1134,    0, 2141,    0,    0,    0,  849,
  851,  826, -150, -302,-1080,-1072,    0,    0,    0, 1909,
    0, -765,    0, 1701,    0,    0,    0,    0,    0,-1312,
 -848,    0,    0,    0,    0,    0,    0, -319,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,-1021,    0,    0, -808,    0, -793, 1234, 1036,
    0,  874,    0,    0,    0,    0,    0,    0,    0, -491,
    0,    0, 1991, -154, -525, 1413, 1167, -666, -864,-1127,
-1475,    0,    0,  -52,    0,    0,    0,    0,    0, 2394,
    0,    0,    0,    0, 1890,    0, 2415, 2163,    0,    0,
    0,    0,    0,   -9, -520,    0,    0, 2434,    0,    0,
    0,  -10,    0,    0,    0, -425, 1868, -422, 1375,    0,
    0,    0,    0,    0,    0, -444,    0,    0,    0, 2123,
    0, 1879,    0,    0,    0,    0,    0,    0,  -37,    0,
 2836,    0, 2688,    0,    0,    0, 2499,    0,    0,    0,
    0, 2256,    0, 2264,    0,    0,    0, 1995,    0,    0,
    0, -700,    0, -318,    0,    0,    0,    0,    0, -482,
 -698, -979, 2054,    0, 1049,    0,    0,    0,    0,    0,
    0, -398,    0,    0, -850, -263, 1266,    0,    0,    0,
 2702, 1100,    0, 2008,-1145, -717,    0,-1071, 1726,-1419,
    0,    0,    0,    0, -170,    0,    0,    0,    0, 1475,
    0,    0,    0,    0, 2784,  170,    0,  968, 2107,    0,
 1144,    0, 1365,    0, -330,    0,    0, 2001,    0,    0,
    0,    0,    0,    0, 2286,    0,    0,    0, -535,-1283,
 2048, 1767,-1263,    0,    0,    0,    0, -761, -558,  -14,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  848, 1612,    0,    0,    0, -993,
    0,    0,    0, 2403, 1484,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -886, 1568,    0,    0,
};
#define YYTABLESIZE 18578
static short yytable[] = {                                     154,
   40,   86,  239,  364,  266,  174,  173,  316,  181,  336,
  339,  892,  335,  338,  261, 1098,  651,  277,  306,  282,
  262,  718,  274,  811,  719,  688,  297,  303,  917, 1154,
 1099,  893,  812,  682,  569,  257,  653, 1362,  301,  846,
 1675,  305,  307,  828, 1420,  612,  274, 1569,  234,  237,
  849,  241,  244,  173,  249,  252, 1311, 1062, 1388, 1421,
  263,  237,  252, 1070,  244, 1104,  173,  249, 1108,  281,
  154,  252,  589,  989, 1354,  627,  866, 1615, 1036,  293,
  174,  173,  295,  237,  245,  293,  252,  241,  244,  304,
  173,  249,  281,  154,  252,  372,  244,  275,  242,  843,
 1422, 1429,  250,  812, 1351,  998,  322, 1423,  324,  367,
  270,  657, 1477, 1084,   85, 1631, 1152,  783,  762,  453,
 1456,  660,  783,  334, 1240,  744, 1215,  166,  365, 1221,
   52,  760,  333,  300,  302, 1424, 1428,  167, 1158,   53,
  905,  308,  309, 1432,  909,  374,  263,  173,  263,  919,
  734,  759,  154, 1438, 1472,  334, 1665, 1666,  949,  252,
  879, 1669, 1179,  254,  363, 1182,  237,  244,  173,  281,
  850,  140,  460,  181, 1189,  828,  826,  762,  323, 1460,
 1425,  783,  320,  813,  744,   95,  166,  365, 1159,   52,
  760, 1192,  759, 1096,  613, 1003,  167,  789,   53,  370,
  320,  417,  762,  793,  254,  957,  346,  254,   87,  864,
  759,   87,  676,  368, 1262,  880,  828,  617, 1376,  828,
  875,  897,  254,  948,  363,   97, 1673,  813, 1799, 1842,
  140, 1178,  448, 1355,  828, 1235,  718, 1096,  789,  719,
   85,  789,  813,  762,  793, 1641,  608,  793,  828, 1879,
  337,  137, 1096,  676, 1029,  975,  789,  371,  334,  239,
  555,  762,  793,  563,  608,  619,  159, 1774, 1775,  925,
  686,  676,  688,  334, 1711,  266,  812,  922,  869,  511,
  393,  783,  841,  363, 1946,  407,  713, 1098,  598,  421,
 1245, 1896, 1307, 1308,  156, 1029, 1420, 1420,  569,  239,
  263, 1420, 1099,  263, 1809,  608,  498,  637, 1615,  571,
  154, 1421, 1421,  972,  883,  611, 1421,  637,  969,  601,
 1642,  340,  783,  608, 1087, 1654,  819,  713,  399, 1029,
 1337, 1095,  826,  616,  636,  852,  193,  241,  511,  393,
  783,  173, 1489, 1889,  407,  713,   67,  870,  193, 1841,
  164,  165, 1422, 1422,  923,  106, 1667, 1422,  157, 1423,
 1423,  341,  422,  652, 1423,  498, 1784, 1699,  400,  678,
  796, 1030,  605,  555, 1490,  262,  239,  668, 1904, 1663,
  670, 1353,  729,  625,  625,  819,  938, 1424, 1424, 1890,
   14,  732, 1424,  636,  924,  193,  371,  677,  160, 1703,
 1891, 1443,  742,  263,  613, 1685,  683,  193,  685, 1406,
  678,  796, 1030,  263,  266,  263,  239,  263,  391, 1329,
  673,  137,  137,  738,  166, 1653, 1918, 1198,  678,  796,
 1426, 1427, 1425, 1425,  167, 1260, 1416, 1425,  677,  237,
  244,  249, 1549,  760, 1222, 1551, 1030, 1259, 1963,  173,
   95,  241, 1440, 1996,  751, 1405,  677, 1433, 1434, 1712,
 1409,  748, 1142, 1887, 1241, 1673,  759, 1177, 1950, 1355,
  603,  166,  638,  583,  107, 1257,  759,  391,  759, 1258,
  740,  167,  687, 1810,  760, 1878,  730,  759, 1636, 1637,
   97,  874, 1795,  939, 1554,  782, 1785,  971,  762, 1892,
 1825,  842,  760,  392, 1172,  744,  815, 1174,  850, 1482,
 1214,  760,  789,  833,  744,  140,  832,  762,  793,   84,
  760, 1185, 1956, 1494, 1495,  611, 1496, 1497, 1900,  950,
  365,  759, 1214,  789,  332,  744,  782, 1444,  762,  793,
  759, 1945,  332,  511,  393,  940, 1562,  854,  676,  407,
 1205,  828, 1261, 1939,  782,  173,  244,  571,  569, 1153,
  465, 1563,  392,  813,  173, 1984,  319, 1471,  173,  744,
   87, 1160,  813,  571, 1839,  181,   52, 1800,  563,  980,
  828, 1806,  762,  759, 1382,   53,  838, 2002,  956,  783,
  887, 1564, 1126, 1976, 1104,  845,  783,  759,  657,  850,
 1029,  713,  840, 1832, 1387,  998, 1702,  898, 1935, 1936,
  951,  713,  921,  713,  332, 1247, 1848,  783,  263, 1811,
 1838,  906,  713, 1812, 1813, 1481,  363,  813,  332,  274,
 1383,  762, 1408,  193,  762,  783,  166, 1244,  744, 1096,
  714,  863,  753,  789,  760,   84,  167,  608,  762,  793,
  952,  914,  263, 1100,  974,  947,  254,  914,  676, 1840,
  332, 1615,  340, 1755,  759,  608,  713, 1758,  828,  511,
  393,  498,  636,  852,  608,  713, 1105, 1233, 1662,  794,
  193,  714,  391,  753,  714,  796, 1615,  819,  193,  262,
   91,  789,  193, 1441, 1846,  979,  813,  398, 2020,  714,
 1677,  753,  983,  984, 1029,  678,  796,  158,  613, 1378,
  789, 1340, 1145,  340, 1615,  762,  793, 1030,  713,  795,
  794, 1029,  193, 1701,  193, 1615, 2019,  783, 1615,  263,
  713,  636,  713,  677,  636, 1615,  387, 1060,  794, 1004,
 1852, 1041, 1982, 1780, 1011,  188,  193,  737, 1420, 1420,
  636,  340, 1263, 1037,  610, 1888,  398,  454,  760, 1922,
  795, 1411, 1911, 1421, 1421, 1923, 1912,  392,  760,  631,
  760,  193,  252,  431, 1029,  285,  636,  637,  795,  760,
 1501, 1921,  555, 1167,  555,  399, 1113,  183, 1430, 1502,
 1431, 1045,  340, 1442,  783,  387, 1145,  189, 1925,  713,
  151, 1805, 1439, 1720, 1422, 1422,  886, 1089,  391,  286,
  782, 1423, 1423,  610, 1029,  678,  796,  263,  152, 1043,
 1379, 1030,  263,  760,  263, 1445,  263, 1847,  631,  611,
 1473,  782,  760,  571,  262, 1676,  637, 1148, 1030, 1424,
 1424,  173,  244,  677,  399,  193,  432, 1556,  455, 1128,
  244,  561, 1168, 1149,  561,  887,  255,  256,  150,  151,
  555, 1446, 1166,  173, 1194,  173,  821, 1247,  263, 1264,
 1875,  822, 1124, 1176,  263,  760, 2003,  152, 1983, 1962,
  625,  253, 1964,  274, 1425, 1425,  395, 1195,  840,  760,
 1199, 1030, 1204,  392,  605,  394, 1151,  487, 1640,  490,
  263,  615, 1749, 1230, 1231, 1232,  184,  756,  330, 1219,
  185,  255,  256,  193, 1804, 2004, 1969,  150, 1971, 1989,
 1211,  106,  274,  274,  274, 1100, 1975,  263, 1977, 1029,
  263, 1030, 1251,  715, 1393, 1633, 1536, 1689,  397,  396,
 1697,  782,  612,  490,  680,  395,  751, 1394,  756,  613,
  184,  756,  752, 1212,  394,  714,  760,  753,  490,  340,
  615,  398, 2005,  186,  647,  714,  756,  714, 1098,  753,
 1395,  173,  193, 2010,  715,  238,  714,  715,  753,  749,
  176,  173,  941, 1099, 1396,  680, 1069, 1004,  967,  751,
 1256, 1004,  715,  752,  794,  387,  748,  397,  396, 1040,
  387,  612, 1234,  680,  487,  387,  254, 1252,  782,  184,
 1688,  752, 1242, 1065, 1066,  794, 1502, 1502,  942,  253,
  714, 1397,  753,  647, 1502, 1502, 1769, 1502, 1502,  714,
  758,  753,  614, 1885,  795, 1502, 1502, 1502, 1502,  185,
 1502, 1502, 1502, 1502, 1502, 1502, 1030,  254,  817,  399,
  254, 1684, 1765,  818,  968,  795,  487,  897,  957, 1332,
 1333,  252,  269, 1402,  632,  254,  177, 1049, 1049, 1145,
  611,  758,  714, 1069,  753,  296,  379,  151,  757, 1616,
  169, 1705, 1619,  561,  714,  340,  714,  398,  753,  758,
 1275,  614, 2016, 1069,  379,  152, 1972,  178,  185,  957,
  837,  817,  957,  836,  571,  571,  818,  631, 1341,  310,
  718,  442,  179,  719, 1403,  637,  561,  957,  943,  757,
  244, 1994,  294,  632,  601,  794,  387,  887,  640, 1098,
  601, 1098,  340, 1398,  150,  150,  414,  757, 1276, 1098,
  631, 1098,   84, 2017, 1099, 1069, 1099, 1745,  637, 2013,
  395,  957, 1067,  714, 1099,  753, 1099, 1466,  561,  394,
 2018, 1949, 1277, 2021,  288,  795,  631,  415, 1953,  631,
 2022,  193, 1484,  130,  637,  399,  274,  637, 1488,  640,
  170,  290, 1746,  193, 1100,  631, 1098,  171,  262,  601,
  613,  274,  563,  637, 1747,  414,  340,  274, 1334,  298,
 1451, 1099,  397,  396,  411,  958, 1049,  130,  184,  449,
  193,  631, 1452,  561,  175,  340,  193, 1451,  193,  637,
  184,  602,  756, 1628, 1628, 1632,  415,  504,  263, 1462,
  150, 1285,  263, 1978,  756,  312, 1047,  821,  819,  313,
  820,  130,  822,  756,  368, 1286,  958,  184,  715,  958,
  317,  821,  819,  184,  820,  184,  822,  193,  715, 1659,
  715, 1661,  340,  411,  958,  193,   43,  752,  561,  715,
 1031,  410,  749,  175,  150,  603,  395,  925, 1463,  752,
  680, 2014,  254,  254, 1620,  394,  504,  756,  752, 1287,
  254,  318,  604, 1032,  184,  450,  756,  185,  958, 1529,
  173, 1901,  647,  368, 1565,  321, 1618,  555, 1621,  185,
  262,  611, 1844,  715, 1462,  150,  411, 1288,  150,  325,
  340, 1704,  715,   88,   89,   43,  914,  773,  397,  396,
  451,  500,  752,  150,  193,  647,  185, 1980,  176,  756,
  419,  752,  185, 1505,  185,  758,  263,  263,   47, 1033,
 1570, 1614,  326,  756,  571,  244,  249,  571,  281,  154,
 1049,  647,  773, 1465,  647,  715,  758,  914, 1108, 1108,
  914, 1108, 1108, 1108,  773,  571,  571,  715,  254,  715,
  647, 1289,  244,  185,  752,  914, 1108, 1108, 1108,  248,
  500, 1506,  463,  757,  957,  536,  796,  176,  752,  254,
  468, 1617,  632,  165,  366,  254,  647,   47,  821,  819,
  758,  820,  519,  822,  757, 1393,  932, 1108,  933,  758,
  756,  613,  193,  412,  824,  513,  821,  819, 1394,  820,
  248,  822,  365,  248,  142,  632,  587,  821,  819,  254,
  820,  413,  822, 1924,  536,  592,  715,  634,  248,  635,
 1687, 1395, 1710,  716,   14,  796,  382, 1715,  757,  184,
  957,  632,  758,  366,  632,  752,  716,  757,  821,  819,
 1797,  820,  817,  822,  716,  414,  758,  818,   41,  828,
  632,  848,  347,  824,  513,  504,  817,  821,  819,  716,
  820,  818,  822,  142,  143,  821,  819,  504,  820,  254,
  822,  631, 1397,  716, 1768,  636,  632, 1348,  821,  819,
  757,  820,  674,  822,  649,  382,  867,  395, 1529,  173,
  828,  131,  675,  828,  757, 1817, 1393,  650,  396,  438,
  504,  809,  504,  214,  402,  352,  807,   41,  828, 1394,
  716,  958,  611,  758,  425, 1938, 1818,  426,  185,  109,
  150,  150, 1819,  143,  821,  819,  332,  820,  150,  822,
  808,  237, 1395,  439, 1645,  440, 1647,  348,  398, 1773,
 1256, 1256,  821,  819, 1338,  820, 1396,  822,  332,  418,
  131,  248, 1614, 1144, 1905,  111, 1820,  441,  914,  427,
  809,  757,  214,  821,  819,  807,  820, 1108,  822, 1656,
  349,  833,  590, 1108,  795, 1206, 1100,  958, 1108, 1657,
  596, 1108,  442, 1397,  571,  571,  808,  809,  810,  808,
 1098,  434,  248,  895, 1903,  248,  350,  911,  833,  351,
  910,  363, 1098,  341, 1991, 1098, 1992, 1628,  244,  244,
  248,  914,  423,  817,  914,  352,  150, 1108,  818,  387,
  112,  914,  716,  914,  435, 1108,  437,  914,  446,  387,
  113,  817,  914, 1108,  173,  914,  818,  150,  457,  904,
  250,  353,  817,  150, 1509, 1083,  459,  818,  461,  404,
  914, 1108,  464, 1108,  465,  114,  560, 1079, 1872,  855,
 1069,  405,  566,  857,  914, 1823, 1108,  564, 1108, 1108,
 1108, 1906,  914,  817,  574,  914,  914,  150,  818,  858,
  904,  250, 1328,  904,  250,  914, 1919,  578, 1083,  575,
 1995,  826,  817,  579,  406, 1398,  407,  818,  904,  250,
  817,  914,  580, 1529,  173,  818,  581,  241,  582, 1103,
  584, 1753, 1121,  817,  597, 1120,  214, 1108,  818,  255,
  256, 1103,  342,  914, 1103,  828,  828, 1239,  914,  599,
 1238, 1730,  251,  828,  600, 1920, 1256, 1100, 1243, 1100,
  914,  994,  914, 1352,  613, 1358,  826, 1100, 1357, 1100,
  609, 1207, 1208, 1347, 1897,  363,  620,  214,  244,  817,
  214,  856,  821,  819,  818,  820,  623,  822, 1407,  613,
  624, 1238,  622,  251, 1089,  214,  251,  817,  561,  173,
 1957, 1927,  818,  628,  241, 1929,  285,  821,  819,  914,
  820,  251,  822, 1941, 1100,  138,  138,  613,  817, 1560,
  630, 1454,   14,  818, 1453,  484,  485,  486,  613,  914,
 1917,  613, 1459,  914, 1561, 1458,  643, 1355,  613,  561,
  286,  828,  561, 1635,  642,  828,  826,  248,  248, 1788,
 1790,  241, 1010,  494,  495,  248, 1280,  561, 1284,  561,
 1292, 1295,  828, 1300, 1303, 1306,  646, 1038,  828, 1042,
  237, 1943, 1039, 1046, 1048, 1050, 1044, 1719, 1760, 1944,
  826,  826,  871,  644, 1796,  611, 1061,  826, 1789, 1791,
 2000, 1068, 1069, 1999,  563, 1072, 1073, 1074, 1075, 1076,
 1080, 2001,  828,  377,  836,  781,  377,  559,  781,  519,
  611, 1090, 1091, 1092, 1093, 1094,  519,  655,  666, 1101,
  669,  904,  679,  871,  680, 1614,  871, 1968,  684,  735,
  739, 1128,  741,  743,  263,  776,  250,  250,  611,  778,
  779,  871,  780,  248,  250,  241,  781,  241,  559,  611,
 1614,  559,  611,  782,  783,  592,  784,  785,  786,  611,
  787,  788,  828,  789,  248,  790,  559,  793,  559,  798,
  248,  872,  799,  800,  904, 1155,  108,  904, 1614, 1128,
  801,  802,  803,  804,  904,  805,  904,  631, 1165, 1614,
  904, 1256, 1614,  806,  807,  904,  816,  823,  904, 1614,
  824, 1082,  821,  819,  248,  820,  826,  822,  227,  827,
  831,  852,  872,  904,  109,  872,  834,  817,  835,  860,
  877,  862,  818,  873,  878,  882,  110,  904,  251,  251,
  872,  884,  250,  885,  889,  904,  251,  894,  904,  904,
  890,  903,  817,  896,  637,  899,  900,  818,  904,  227,
  111,  902,  227,  250,  907,  936,  561,  332,  912,  250,
  937,  945,  953,  954,  904,  959,  960,  227,  961,  962,
  964,  561, 1127,  821,  819,  965,  820,  970,  822,  976,
  977,  981,  982,  991,  993,  994,  904,  821,  819, 1339,
  820,  904,  822,  250,  999,  561, 1319,  821,  819, 1001,
  820,  561,  822,  904,  561,  904, 1000, 1265, 1266, 1270,
 1005, 1071, 1279, 1281, 1283,  112, 1291, 1294, 1297, 1299,
 1302, 1305,  188, 1063,  251,  113, 1085,  971,  334, 1109,
 1312, 1313, 1114, 1315, 1116, 1317, 1115, 1118, 1320, 1321,
 1322, 1323, 1324, 1325,  871,  251,  561, 1122,  843, 1141,
  114,  251,  904,  250, 1146, 1156, 1330, 1161, 1335,  836,
 1162, 1173, 1183, 1184, 1186,  559,  915, 1190, 1191, 1196,
 1193, 1201,  904,  538, 1350, 1229,  904, 1210, 1202, 1203,
 1217, 1227,  559,  559,  559,  251, 1236, 1309,  559,  821,
  819,  559,  820, 1250,  822, 1272, 1310,  871, 1316,  817,
  871,  561, 1318, 1326,  559, 1327, 1355,  871,  561,  871,
  559, 1356, 1359,  871,  538,  519, 1361,  538,  871, 1377,
 1380,  871,  556, 1390, 1392, 1385,  561, 1386, 1400, 1415,
 1435,  955,  538,  872, 1412, 1437,  871,  817, 1447, 1457,
 1455, 1467,  818, 1461, 1468,  251, 1469, 1470, 1244, 1478,
  871, 1342,  821,  819, 1486,  820,  561,  822,  871, 1499,
  561,  871,  871,  556, 1491, 1492,  556, 1493, 1500, 1504,
 1507,  871,  955,  955,  955,  955,  955,  528,  955,  559,
 1508,  556, 1510,  556,  239,  227,  872,  871, 1511,  872,
  955,  955,  955,  955, 1518, 1519,  872, 1520,  872, 1521,
  559, 1523,  872, 1524, 1525, 1526,  559,  872,  817,  871,
  872, 1527, 1528,  818,  871,  559, 1029,  559,  528, 1550,
 1553,  528,  817, 1622,  955,  872,  871,  818,  871, 1624,
 1625, 1638,  817, 1639, 1648,  559,  528,  818,  858,  872,
  559, 1649, 1343,  821,  819, 1650,  820,  872,  822, 1651,
  872,  872,  661, 1643, 1660, 1686, 1674, 1344,  821,  819,
  872,  820, 1214,  822, 1678,  519, 1679, 1680, 1681,  559,
 1700, 1682, 1690, 1096, 1706,  871,  872, 1683, 1512, 1513,
  239, 1515, 1517, 1708, 1723, 1709, 1345,  821,  819, 1522,
  820,  516,  822, 1714, 1721,  871, 1743, 1756,  872,  871,
 1744,  239, 1750,  872, 1346,  821,  819,  239,  820, 1498,
  822, 1540, 1767, 1766, 1770,  872,  157,  872, 1777, 1547,
 1786, 1548, 1335, 1757,  817, 1335, 1783, 1330, 1793,  818,
 1778, 1555,  519, 1557, 1787, 1792, 1798, 1814, 1824, 1826,
 1558,  239, 1559, 1827, 1828, 1830, 1829, 1831, 1833,  538,
  538, 1130, 1845, 1834,  538, 1843, 1835,  538, 1849, 1534,
  821,  819, 1850,  820,  872,  822, 1851, 1535,  821,  819,
  538,  820, 1854,  822, 1855, 1538,  821,  819, 1856,  820,
  556,  822, 1857,  519,  872, 1860, 1861,  817,  872,  955,
 1862, 1863,  818,  955, 1864,  955,  955,  556,  556,  556,
 1865, 1866,  955,  556, 1867, 1868,  556,  955,  955,   57,
 1869, 1871, 1882,  955, 1874,  955,  955, 1898, 1779,  556,
   58, 1899, 1910, 1668, 1913,  556, 1915,   59, 1916, 1928,
  955,   60, 1932, 1937,  955,  955,  955, 1940, 1947,  955,
 1948, 1951, 1954, 1958, 1965,  538,  955, 1966,  955,  955,
 1967,   61,  955,  528,  528, 1970,  955, 1985,  955, 1990,
  955,  528,  955, 1987, 1993, 1982,  538,  955,  955, 1998,
  955, 1983,  538, 2008,  528, 2012,    1, 2009,  817, 2011,
  789, 2015,   29,  818,   31,  955,  955,  200,  955,  828,
  268, 1716,  163,  817,  556, 1063,  379, 1722,  818,  955,
  705,  955, 1726,  955,  955,  955,  538,  955,  768, 1731,
  955,  955,   82,   62, 1048,  556,  957,  279,  210,  767,
  955,  556,  817,  796,  955,  190,  749,  818,  278,  407,
  955,  769,  556,  955, 1047,  751,  955, 1021,  380,  323,
  817,  955,  191, 1335,   63,  818,  220, 1335,   64,  528,
  556,  321,  955, 1764,  331,  556,  295, 1020,  955,  955,
  322,  324,  332,  955,  955,  778,  327,  360,   29, 1771,
  528,  345,   32,   33,  955,  955,  528,  955,   42, 1626,
  327,  851,  328,  955,  556,  853,  955, 1125,  955,  955,
  955,  955,  329,  955,  573,  817,  572, 1117, 1487, 1485,
  818,  955,  955,  817,  821,  819,  299,  820,  818,  822,
  528,  817,  955,  955, 1090,  955,  818, 1249,  992, 1254,
 1213,  284,  955,  955,  955,  955,  955, 1483,  955, 1475,
 1223,  955,  955,  955,  955,  955, 1836,   65, 1837,  955,
  955,  648,  692, 1801,  955,  955,  955,  955,  955,  955,
  955,  955,  955,  955, 1065, 1090, 1090, 1090, 1090, 1090,
 1090, 1090, 1464,  955, 1414, 1539,  821,  819,  618,  820,
 1476,  822, 1090, 1090, 1090, 1090, 1090, 1188, 1816, 1157,
 1541,  821,  819, 1914,  820,  115,  822, 1981,  116,  663,
  117,  118,  119,  120,  664,  121, 1822,  931, 2007, 1988,
 1986, 1363, 1772,  122,  123, 1090, 1895, 1090, 1181, 1973,
 1655, 1807, 1106,  671,  124,  125, 1853,  126, 1197,  654,
   14,  935, 1858, 1859,  127,  128, 1698,  129,  130, 1228,
  131,  645, 1220,  132,  133,  134,  135,  136,  163,  978,
  331,  137,  138,  610,  868, 1389, 1873,  139,  140,  141,
 1335,  142,  143,  144, 1876,  145,  872, 1147,  345, 1143,
 1646, 1883, 1884,  115,  294,  146,  116, 1955,  117,  118,
  119,  120, 1034,  121, 1542,  821,  819, 1870,  820, 1384,
  822,  122,  123, 1742, 1164,  792,  519, 1123, 1514,  881,
 1664, 1552,  124,  125,    0,  126,    0,    0,    0,    0,
    0,  228,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0, 1729,  139,  140,  141,    0,  142,
  143,  144,    0,  145, 1543,  821,  819, 1926,  820,  817,
  822,    0,  228,  146,  818,  228, 1544,  821,  819,    0,
  820,    0,  822, 1545,  821,  819,    0,  820,    0,  822,
  228, 1546,  821,  819,    0,  820,    0,  822, 1051, 1052,
 1053, 1054, 1055, 1056, 1057, 1058, 1090,    0, 1090, 1090,
    0,    0,    0,    0,    0, 1090,    0,    0,    0,    0,
    0, 1090,    0,    0,    0,    0, 1090,    0,    0, 1090,
    0,  817,    0, 1801,    0,  561,  818,    0,    0,    0,
    0,    0,    0, 1090, 1069,    0,  817,    0, 1090, 1090,
    0,  818, 1090,    0, 1751,  821,  819,    0,  820, 1090,
  822, 1090, 1090,    0,    0, 1090,    0,    0,    0, 1090,
 1974, 1090,    0, 1090,    0, 1090,  561,    0,    0,  561,
 1090, 1090,    0, 1090,    0, 1069, 1069, 1069, 1069, 1069,
 1083, 1069,    0,    0,  561,    0,  561,    0, 1090, 1090,
    0, 1090,    0, 1069, 1069, 1069, 1069,    0,    0,    0,
    0,    0, 1090,    0, 1090,    0, 1090, 1090, 1090,    0,
 1090,    0,    0, 1090, 1090,    0,    0,    0,    0,    0,
    0,    0,    0, 1090,    0, 1069,    0, 1069,    0,    0,
  817,    0,    0,    0,    0,  818, 1090,    0,    0, 1090,
    0,    0,    0,    0,    0,    0, 1752,  821,  819,    0,
  820,    0,  822,    0,    0, 1090,    0,    0,    0,    0,
    0, 1090, 1090,    0,    0,    0, 1090,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1090, 1090,    0,
 1090, 1759,  821,  819,    0,  820, 1090,  822,    0, 1090,
  817, 1090, 1090, 1090, 1090,  818, 1090,  239,  228,    0,
    0,    0,  817,    0, 1090, 1090,    0,  818,    0,  817,
    0,    0,    0,    0,  818, 1090, 1090,  817, 1090,    0,
    0,    0,  818,    0,    0, 1090, 1090, 1090, 1090, 1090,
    0, 1090,    0,    0, 1090, 1090, 1090, 1090, 1090,    0,
    0,    0, 1090, 1090,    0,    0,    0, 1090, 1090, 1090,
 1090, 1090, 1090, 1090, 1090,    0, 1090,    0,    0, 1762,
  821,  819,    0,  820,    0,  822, 1090, 1763,  821,  819,
    0,  820,    0,  822,    0,    0,    0,    0,    0,    0,
  817,    0,    0,  561,    0,  818,    0,    0,    0,    0,
    0,    0,    0,  239,    0,    0, 1069,    0, 1069, 1069,
    0,  561,  561,    0,    0, 1069,  561,    0,    0,  561,
    0, 1069,    0,    0,  239,    0, 1069,    0,    0, 1069,
  239,    0,  561,    0,    0,  561,    0,    0,  561,    0,
    0,    0,    0, 1069, 1100, 1069,    0,    0, 1069, 1069,
    0,    0, 1069,    0, 1877,  821,  819,    0,  820, 1069,
  822, 1069, 1069,    0,  239, 1069, 1934,  821,  819, 1069,
  820, 1069,  822, 1069,    0, 1069,  561,    0,    0,  561,
 1069, 1069,    0, 1069,    0, 1100, 1100, 1100, 1100, 1100,
 1100, 1100,  817,    0,  561,    0,    0,  818, 1069, 1069,
    0, 1069,    0, 1100, 1100, 1100, 1100,  561,    0,    0,
    0,    0, 1069,    0, 1069,    0, 1069, 1069, 1069,    0,
 1069,    0,    0, 1069, 1069,    0,    0,  817,  561,    0,
    0,    0,  818, 1069,  561, 1100,   69, 1100,    0,    0,
    0,    0,    0,    0,    0,  561, 1069,   70,    0, 1069,
    0,    0,    0,    0,   71,    0,    0,  821,  819, 1761,
  820,    0,  822,  561,    0, 1069,    0,   72,  561,    0,
    0, 1069, 1069,    0,    0,    0, 1069,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1069, 1069,    0,
 1069,    0,    0,    0,    0,    0, 1069,  561,    0, 1069,
    0, 1069, 1069, 1069, 1069,  817, 1069,    0,    0,    0,
  818,    0,    0,  817, 1069, 1069,  821,  819,  818,  820,
    0,  822,    0,    0,   73, 1069, 1069,    0, 1069,    0,
    0,    0,    0,    0,    0, 1069, 1069, 1069, 1069, 1069,
   74, 1069,    0,    0, 1069, 1069, 1069, 1069, 1069,    0,
    0,    0, 1069, 1069,    0,    0,   75, 1069, 1069, 1069,
 1069, 1069, 1069, 1069, 1069,    0, 1069,    0,    0,    0,
    0,   76, 1717, 1718,    0,   77, 1069,    0,   78,    0,
 1724, 1725,    0, 1727, 1728,    0,   79,    0,    0,    0,
  817, 1732, 1733, 1734, 1735,  818, 1736, 1737, 1738, 1739,
 1740, 1741,  817,    0,    0,   80, 1100,  818, 1100, 1100,
    0,  561,  561,    0,    0, 1100,  561,    0,    0,  561,
    0, 1100,    0,    0,    0,    0, 1100,    0,    0, 1100,
    0,    0,  561,    0,    0,  561,    0,    0,    0,    0,
    0,    0,    0, 1100, 1105,    0,    0,    0, 1100, 1100,
    0,    0, 1100,    0,    0,    0,    0,    0,    0, 1100,
    0, 1100, 1100,    0,   81, 1100,    0,    0,    0, 1100,
    0, 1100,    0, 1100,    0, 1100,  561,    0,    0,  561,
 1100, 1100,    0, 1100,    0, 1105, 1105, 1105, 1105, 1105,
 1105, 1105,  817,    0,  561,    0,    0,  818, 1100, 1100,
    0, 1100,    0, 1105, 1105, 1105, 1105,  561,    0,    0,
    0,    0, 1100,    0, 1100,    0, 1100, 1100, 1100,    0,
 1100,    0,    0, 1100, 1100,    0,    0,    0,  561,    0,
    0,    0,    0, 1100,  561, 1105,    0, 1105, 1314,    0,
    0,    0,    0,    0,    0,    0, 1100,   92,    0, 1100,
    0,  817,    0,    0,   93,    0,  818,    0,   94,   95,
    0,    0,    0,    0,    0, 1100,    0,   96,  561,    0,
    0, 1100, 1100,    0,    0,    0, 1100,    0,   97,    0,
    0,    0,    0,    0,    0,    0,    0, 1100, 1100,    0,
 1100,    0,    0,    0,    0,    0, 1100,    0,    0, 1100,
    0, 1100, 1100, 1100, 1100,    0, 1100,    0,    0,    0,
    0,    0,    0,    0, 1100, 1100,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1100, 1100,    0, 1100,    0,
    0,    0,    0,    0,    0, 1100, 1100, 1100, 1100, 1100,
   98, 1100,    0,    0, 1100, 1100, 1100, 1100, 1100,    0,
    0,    0, 1100, 1100,    0,    0,   99, 1100, 1100, 1100,
 1100, 1100, 1100, 1100, 1100,    0, 1100,    0,    0,    0,
    0,  100,    0,    0,    0,  101, 1100,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  102,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  103, 1105,    0, 1105, 1105,
    0,  561,  561,    0,    0, 1105,    0,    0,    0,  561,
    0, 1105,    0,    0,    0,    0, 1105,    0,    0, 1105,
    0,    0,  561,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1105, 1101,    0,    0,    0, 1105, 1105,
    0,    0, 1105,    0,    0,    0,    0,    0,    0, 1105,
    0, 1105, 1105,    0,  104, 1105,    0,    0,    0, 1105,
    0, 1105,    0, 1105,    0, 1105,    0,    0,    0,    0,
 1105, 1105,    0, 1105,    0, 1101, 1101, 1101, 1101, 1101,
 1101, 1101,    0,    0,    0,    0,    0,    0, 1105, 1105,
    0, 1105,    0, 1101, 1101, 1101, 1101,  561,    0,    0,
    0,    0, 1105,    0, 1105,    0, 1105, 1105, 1105,    0,
 1105,    0,    0, 1105, 1105,    0,    0,    0,  561,    0,
    0,    0,    0, 1105,  561, 1101,    0, 1101,    0,    0,
    0,    0,    0,    0,    0,    0, 1105,    0,    0, 1105,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1105,    0,    0,  561,    0,
    0, 1105, 1105,    0,    0,    0, 1105,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1105, 1105,    0,
 1105,    0,    0,    0,    0,    0, 1105,    0,    0, 1105,
    0, 1105, 1105, 1105, 1105,    0, 1105,    0,    0,    0,
    0,    0,    0,    0, 1105, 1105,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1105, 1105,    0, 1105,    0,
    0,    0,    0,    0,    0, 1105, 1105, 1105, 1105, 1105,
    0, 1105,    0,    0, 1105, 1105, 1105, 1105, 1105,    0,
    0,    0, 1105, 1105,    0,    0,    0, 1105, 1105, 1105,
 1105, 1105, 1105, 1105, 1105,    0, 1105,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1105,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1101,    0, 1101, 1101,
    0,    0,    0,    0,    0, 1101,    0,    0,    0,    0,
    0, 1101,    0,    0,    0,    0, 1101,    0,    0, 1101,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1101, 1114,    0,    0,    0, 1101, 1101,
    0,    0, 1101,    0,    0,    0,    0,    0,    0, 1101,
    0, 1101, 1101,    0,    0, 1101,    0,    0,    0, 1101,
    0, 1101,    0, 1101,    0, 1101,    0,    0,    0,    0,
 1101, 1101,    0, 1101,    0, 1114, 1114, 1114, 1114, 1114,
 1114, 1114,    0,    0,    0,    0,    0,    0, 1101, 1101,
    0, 1101,    0, 1114, 1114, 1114, 1114,    0,    0,    0,
    0,    0, 1101,    0, 1101,    0, 1101, 1101, 1101,    0,
 1101,    0,    0, 1101, 1101,    0,    0,    0,    0,    0,
    0,    0,    0, 1101,    0, 1114,    0, 1114,    0,    0,
    0,    0,    0,    0,    0,    0, 1101,    0,    0, 1101,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1101,    0,    0,    0,    0,
    0, 1101, 1101,    0,    0,    0, 1101,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1101, 1101,    0,
 1101,    0,    0,    0,    0,    0, 1101,    0,    0, 1101,
    0, 1101, 1101, 1101, 1101,    0, 1101,    0,    0,    0,
    0,    0,    0,    0, 1101, 1101,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1101, 1101,    0, 1101,    0,
    0,    0,    0,    0,    0, 1101, 1101, 1101, 1101, 1101,
    0, 1101,    0,    0, 1101, 1101, 1101, 1101, 1101,    0,
    0,    0, 1101, 1101,    0,    0,    0, 1101, 1101, 1101,
 1101, 1101, 1101, 1101, 1101,    0, 1101,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1101,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1114,    0, 1114, 1114,
    0,    0,    0,    0,    0, 1114,    0,    0,    0,    0,
    0, 1114,    0,    0,    0,    0, 1114,    0,    0, 1114,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1114, 1119,    0,    0,    0, 1114, 1114,
    0,    0, 1114,    0,    0,    0,    0,    0,    0, 1114,
    0, 1114, 1114,    0,    0, 1114,    0,    0,    0, 1114,
    0, 1114,    0, 1114,    0, 1114,    0,    0,    0,    0,
 1114, 1114,    0, 1114,    0, 1119, 1119, 1119, 1119, 1119,
 1119, 1119,    0,    0,    0,    0,    0,    0, 1114, 1114,
    0, 1114,    0, 1119, 1119, 1119, 1119,    0,    0,    0,
    0,    0, 1114,    0, 1114,    0, 1114, 1114, 1114,    0,
 1114,    0,    0, 1114, 1114,    0,    0,    0,    0,    0,
    0,    0,    0, 1114,    0, 1119,    0, 1119,    0,    0,
    0,    0,    0,    0,    0,    0, 1114,    0,    0, 1114,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1114,    0,    0,    0,    0,
    0, 1114, 1114,    0,    0,    0, 1114,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1114, 1114,    0,
 1114,    0,    0,    0,    0,    0, 1114,    0,    0, 1114,
    0, 1114, 1114, 1114, 1114,    0, 1114,    0,    0,    0,
    0,    0,    0,    0, 1114, 1114,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1114, 1114,    0, 1114,    0,
    0,    0,    0,    0,    0, 1114, 1114, 1114, 1114, 1114,
    0, 1114,    0,    0, 1114, 1114, 1114, 1114, 1114,    0,
    0,    0, 1114, 1114,    0,    0,    0, 1114, 1114, 1114,
 1114, 1114, 1114, 1114, 1114,    0, 1114,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1114,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1119,    0, 1119, 1119,
    0,    0,    0,    0,    0, 1119,    0,    0,    0,    0,
    0, 1119,    0,    0,    0,    0, 1119,    0,    0, 1119,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1119,  923,    0,    0,    0, 1119, 1119,
    0,    0, 1119,    0,    0,    0,    0,    0,    0, 1119,
    0, 1119, 1119,    0,    0, 1119,    0,    0,    0, 1119,
    0, 1119,    0, 1119,    0, 1119,    0,    0,    0,    0,
 1119, 1119,    0, 1119,    0,  923,  923,  923,  923,  923,
    0,  923,    0,    0,    0,    0,    0,    0, 1119, 1119,
    0, 1119,    0,  923,  923,  923,  923,    0,    0,    0,
    0,    0, 1119,    0, 1119,    0, 1119, 1119, 1119,    0,
 1119,    0,    0, 1119, 1119,    0,    0,    0,    0,    0,
    0,    0,    0, 1119,    0,    0,    0,  923,    0,    0,
    0,    0,    0,    0,    0,    0, 1119,    0,    0, 1119,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1119,    0,    0,    0,    0,
    0, 1119, 1119,    0,    0,    0, 1119,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1119, 1119,    0,
 1119,    0,    0,    0,    0,    0, 1119,    0,    0, 1119,
    0, 1119, 1119, 1119, 1119,    0, 1119,    0,    0,    0,
    0,    0,    0,    0, 1119, 1119,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1119, 1119,    0, 1119,    0,
    0,    0,    0,    0,    0, 1119, 1119, 1119, 1119, 1119,
    0, 1119,    0,    0, 1119, 1119, 1119, 1119, 1119,    0,
    0,    0, 1119, 1119,    0,    0,    0, 1119, 1119, 1119,
 1119, 1119, 1119, 1119, 1119,    0, 1119,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1119,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  923,    0,  923,  923,
    0,    0,    0,    0,    0,  923,    0,    0,    0,    0,
    0,  923,    0,    0,    0,    0,  923,    0,    0,  923,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  923,  936,    0,    0,    0,  923,  923,
    0,    0,  923,    0,    0,    0,    0,    0,    0,  923,
    0,  923,  923,    0,    0,  923,    0,    0,    0,  923,
    0,  923,    0,  923,    0,  923,    0,    0,    0,    0,
  923,  923,    0,  923,    0,  936,  936,  936,  936,  936,
    0,  936,    0,    0,    0,    0,    0,    0,  923,  923,
    0,  923,    0,  936,  936,  936,  936,    0,    0,    0,
    0,    0,  923,    0,  923,    0,  923,  923,  923,    0,
  923,    0,    0,  923,  923,    0,    0,    0,    0,    0,
    0,    0,    0,  923,    0,    0,    0,  936,    0,    0,
    0,    0,    0,    0,    0,    0,  923,    0,    0,  923,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  923,    0,    0,    0,    0,
    0,  923,  923,    0,    0,    0,  923,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  923,  923,    0,
  923, 1417,    0,    0,    0,    0,  923,    0,    0,  923,
    0,  923,  923,  923,  923,    0,  923,    0,    0,    0,
    0,    0,    0,    0,  923,  923,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  923,  923,    0,  923,    0,
    0,    0,    0,    0,    0,  923,  923,  923,  923,  923,
    0,  923,    0,    0,  923,  923,  923,  923,  923,    0,
    0,    0,  923,  923,    0,    0,    0,  923,  923,  923,
  923,  923,  923,  923,  923,    0,  923,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  923,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  936,    0,  936,  936,
    0,    0,    0,    0,    0,  936,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  936,    0,    0,  936,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,  939,    0,    0,    0,  936,  936,
    0,    0,  936,    0,    0,    0,    0,    0,    0,  936,
    0,  936,  936,    0,    0,  936,    0,    0,    0,  936,
    0,  936,    0,  936,    0,  936,    0,    0,    0,    0,
  936,  936,    0,  936,    0,  939,  939,  939,  939,  939,
    0,  939,    0,    0,    0,    0,    0,    0,  936,  936,
    0,  936,    0,  939,  939,  939,  939,    0,    0,    0,
    0,    0,  936,    0,  936,    0,  936,  936,  936,    0,
  936,    0,    0,  936,  936,    0,    0,    0,    0,    0,
    0,    0,    0,  936,    0,    0,    0,  939,  472,    0,
    0,    0,    0,    0,    0,    0,  936,    0,    0,  936,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  936,  474,    0,    0,    0,
    0,  936,  936,    0,    0,    0,  936,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  936,  936,    0,
  936,    0,    0,    0,  476,    0,  936,    0,    0,  936,
    0,  936,  936,  936,  936,    0,  936,    0,    0,    0,
    0,    0,    0,    0,  936,  936,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  936,  936,    0,  936,    0,
    0,    0,    0,    0,    0,  936,  936,  936,  936,  936,
    0,  936,    0,    0,  936,  936,  936,  936,  936,    0,
    0,    0,  936,  936,    0,    0,    0,  936,  936,  936,
  936,  936,  936,  936,  936,  481,  936,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  936,    0,    0,    0,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,    0,  255,
  256,    0,    0,    0,    0,    0,  939,    0,  939,  939,
    0,    0,    0,    0,    0,  939,    0,    0,  489,  490,
  491,  492,  493,  494,  495,  496,  497,    0,    0,  939,
    0,    0,    0,  499,  500,    0,    0,    0,    0,    0,
    0,    0,  504,  939,  940,    0,    0,    0,  939,  939,
    0,    0,  939,    0,    0,    0,    0,    0,    0,  939,
    0,  939,  939,    0,    0,  939,    0,    0,    0,  939,
    0,  939,    0,  939,    0,  939,    0,    0,    0,    0,
  939,  939,    0,  939,    0,  940,  940,  940,  940,  940,
    0,  940,    0,    0,    0,    0,    0,    0,  939,  939,
    0,  939,    0,  940,  940,  940,  940,    0,    0,    0,
    0,    0,  939,    0,  939,    0,  939,  939,  939,    0,
  939,    0,    0,  939,  939,    0,    0,    0,    0,    0,
    0,    0,    0,  939,    0,    0,    0,  940,    0,    0,
    0,    0,    0,    0,    0,    0,  939,    0,    0,  939,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  939,    0,    0,    0,    0,
    0,  939,  939,    0,    0,    0,  939,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  939,  939,    0,
  939,    0,    0,    0,    0,    0,  939,    0,    0,  939,
    0,  939,  939,  939,  939,    0,  939,    0,    0,    0,
    0,    0,    0,    0,  939,  939,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  939,  939,    0,  939,    0,
    0,    0,    0,    0,    0,  939,  939,  939,  939,  939,
    0,  939,    0,    0,  939,  939,  939,  939,  939,    0,
    0,    0,  939,  939,    0,    0,    0,  939,  939,  939,
  939,  939,  939,  939,  939,    0,  939,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  939,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  940,    0,  940,  940,
    0,    0,    0,    0,    0,  940,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  940,
    0,    0,    0,  934,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  940,    0,    0,    0,    0,  940,  940,
    0,    0,  940,    0,    0,    0,    0,    0,    0,  940,
    0,  940,  940,    0,    0,  940,    0,    0,    0,  940,
    0,  940,    0,  940,  934,  940,  934,  934,  934,    0,
  940,  940,    0,  940,  995,    0,    0,  694,    0,    0,
    0,    0,  934,  934,  934,  934,    0,    0,  940,  940,
  996,  940,    0,    0,  695,    0,    0,  696,    0,    0,
    0,    0,  940,    0,  940,    0,  940,  940,  940,    0,
  940,    0,    0,  940,  940,    0,  934,    0,    0,    0,
    0,    0,    0,  940,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  940,  697,    0,  940,
    0,    0,    0,  698,  699,  700,    0,  701,  702,  703,
    0,  704,    0,    0,    0,  940,    0,    0,    0,    0,
    0,  940,  940,    0,    0,    0,  940,    0,    0,  705,
    0,  706,    0,    0,    0,    0,    0,  940,  940,    0,
  940,    0,    0,    0,    0,    0,  940,    0,    0,  940,
    0,  940,  940,  940,  940,    0,  940,    0,    0,    0,
  707,    0,    0,    0,  940,  940,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  940,  940,  708,  940,    0,
    0,    0,    0,    0,    0,  940,  940,  940,  940,  940,
    0,  940,    0,    0,  940,  940,  940,  940,  940,  709,
    0,    0,  940,  940,    0,    0,    0,  940,  940,  940,
  940,  940,  940,  940,  940,    0,  940,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  940,    0,    0,    0,
    0,    0,    0,    0,  710,  711,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  934,    0,  934,  934,    0,
    0,  712,    0,    0,  934,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  934,    0,
    0,    0,  933,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  934,    0,    0,    0,    0,  934,  934,    0,
    0,  934,    0,    0,    0,    0,    0,    0,  934,    0,
  934,  934,    0,    0,  934,    0,    0,    0,  934,    0,
  934,    0,  934,  933,  934,  933,  933,  933,    0,  934,
  934,    0,  934,  218,    0,    0,  218,    0,    0,    0,
    0,  933,  933,  933,  933,    0,    0,  934,  934,  218,
  934,    0,    0,  218,    0,    0,  218,    0,    0,    0,
    0,  934,    0,  934,    0,  934,  934,  934,    0,  934,
    0,    0,  934,  934,    0,  933,    0,    0,    0,    0,
    0,    0,  934,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  934,  218,    0,  934,    0,
    0,    0,  218,  218,  218,    0,  218,  218,  218,    0,
  218,    0,    0,    0,  934,    0,    0,    0,    0,    0,
  934,  934,    0,    0,    0,  934,    0,    0,  218,    0,
  218,    0,    0,    0,    0,    0,  934,  934,    0,  934,
    0,    0,    0,    0,    0,  934,    0,    0,  934,    0,
  934,  934,  934,  934,    0,  934,    0,    0,    0,  218,
    0,    0,    0,  934,  934,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  934,  934,  218,  934,    0,    0,
    0,    0,    0,    0,  934,  934,  934,  934,  934,    0,
  934,    0,    0,  934,  934,  934,  934,  934,  218,    0,
    0,  934,  934,    0,    0,    0,  934,  934,  934,  934,
  934,  934,  934,  934,    0,  934,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  934,    0,    0,    0,    0,
    0,    0,    0,  218,  218,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  933,    0,  933,  933,    0,    0,
  218,    0,    0,  933,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  933,    0,    0,
    0,  935,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  933,    0,    0,    0,    0,  933,  933,    0,    0,
  933,    0,    0,    0,    0,    0,    0,  933,    0,  933,
  933,    0,    0,  933,    0,    0,    0,  933,    0,  933,
    0,  933,  935,  933,  935,  935,  935,    0,  933,  933,
    0,  933,    0,    0,    0,    0,    0,    0,    0,    0,
  935,  935,  935,  935,    0,    0,  933,  933,    0,  933,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  933,    0,  933,    0,  933,  933,  933,    0,  933,    0,
    0,  933,  933,    0,  935,    0,    0,    0,    0,    0,
    0,  933,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  933,    0,    0,  933,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  933,    0,    0,    0,    0,    0,  933,
  933,    0,    0,    0,  933,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  933,  933,    0,  933,    0,
    0,    0,    0,    0,  933,    0,    0,  933,    0,  933,
  933,  933,  933,    0,  933,    0,    0,    0,    0,    0,
    0,    0,  933,  933,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  933,  933,    0,  933,    0,    0,    0,
    0,    0,    0,  933,  933,  933,  933,  933,    0,  933,
    0,  898,  933,  933,  933,  933,  933,    0,    0,    0,
  933,  933,    0,    0,    0,  933,  933,  933,  933,  933,
  933,  933,  933,    0,  933,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  933,    0,    0,    0,    0,    0,
    0,    0,  898,    0,    0,  898,    0,    0,    0,    0,
    0,    0,    0,  935,    0,  935,  935,    0,    0,    0,
  898,    0,  935,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  935,    0,    0,    0,
  938,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  935,    0,    0,    0,    0,  935,  935,    0,    0,  935,
    0,    0,    0,    0,    0,    0,  935,    0,  935,  935,
    0,    0,  935,    0, 1069,    0,  935, 1069,  935,    0,
  935,  938,  935,  938,  938,  938,    0,  935,  935,    0,
  935,    0, 1071,    0, 1069,    0,    0, 1069,    0,  938,
  938,  938,  938,    0,    0,  935,  935,    0,  935,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  935,
    0,  935,    0,  935,  935,  935,    0,  935,    0,    0,
  935,  935,    0,  938,    0,    0,    0, 1069,    0,    0,
  935,    0,    0, 1069, 1069, 1069,    0, 1069, 1069, 1069,
    0, 1069,    0,  935,    0,    0,  935,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1069,
    0, 1069,  935,    0,    0,    0,    0,    0,  935,  935,
    0,    0,  313,  935,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  935,  935,    0,  935,    0,    0,
 1069,    0,    0,  935,    0,    0,  935,    0,  935,  935,
  935,  935,    0,  935,    0,    0,    0, 1069,    0,    0,
    0,  935,  935,  898,    0,    0,    0,    0,    0,    0,
    0,    0,  935,  935,    0,  935,    0,    0,    0, 1069,
    0,    0,  935,  935,  935,  935,  935,    0,  935,  741,
    0,  935,  935,  935,  935,  935,    0,    0,    0,  935,
  935,    0,    0,    0,  935,  935,  935,  935,  935,  935,
  935,  935,    0,  935, 1069, 1069,  898,    0,    0,  898,
    0,    0,    0,  935,    0,    0,  898,    0,  898,    0,
  741, 1069,  898,  741,    0,    0,    0,  898,    0,    0,
  898,    0,  938,    0,  938,  938,    0,    0,  741,    0,
    0,  938,    0,    0,    0,  898,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  938,    0,    0,    0,  898,
    0,    0,    0,    0,    0,    0,    0,  898,    0,  938,
  898,  898,    0,    0,  938,  938,    0,    0,  938,    0,
  898,    0,    0,    0,    0,  938,    0,  938,  938,    0,
    0,  938,    0,    0,    0,  938,  898,  938,    0,  938,
    0,  938,    0,    0,    0,    0,  938,  938,    0,  938,
    0,    0,    0,    0,    0,    0,    0,    0,  898,    0,
    0,    0,    0,  898,  938,  938,    0,  938,    0,    0,
    0,    0,    0,    0,    0,  898,    0,  898,  938,    0,
  938,    0,  938,  938,  938,    0,  938,    0,    0,  938,
  938, 1103,    0,  915,    0,    0,    0,    0,    0,  938,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  938,    0,    0,  938,    0,    0,    0,    0,
    0,    0,    0,    0,  898,    0,    0,    0,    0,    0,
    0,  938,    0,    0,  712,    0,    0,  938,  938,    0,
    0,    0,  938,    0,  898,    0,    0,    0,  898,    0,
    0,    0,    0,  938,  938,    0,  938,    0,    0,    0,
    0,    0,  938,    0,    0,  938,    0,  938,  938,  938,
  938,    0,  938,    0,  712,  712,    0,    0,  712,    0,
  938,  938,    0,  741,    0,    0,    0,    0,    0,    0,
    0,  938,  938,  712,  938,    0,    0,    0,    0,    0,
    0,  938,  938,  938,  938,  938,    0,  938,    0,    0,
  938,  938,  938,  938,  938,    0,    0,    0,  938,  938,
    0,    0,    0,  938,  938,  938,  938,  938,  938,  938,
  938,    0,  938,    0,  741,    0,    0,  741,    0,    0,
    0,    0,  938,    0,  741,    0,  741,    0,    0,    0,
  741,    0,    0,    0,    0,  741,    0,    0,  741,    0,
    0,    0,    0,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,  741,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,    0,    0,  741,    0,    0,
    0,    0,    0,  124,  125,  741,  126,    0, 1130,  741,
    0,    0,    0,  127,  128,    0,  129,  130,  741,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,  741,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0, 1102,  739,  146,    0,    0,    0,    0,    0,
    0,  741,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  741,    0,  741,    0,    0,    0,    0,
    0,  741,    0,    0,  741,    0,  741,  741,  741,  741,
    0,  741,    0,    0,  739,    0,    0,  739,    0,  741,
  741,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  741,  741,  739,  741,    0,    0,    0,    0,    0,    0,
  741,  741,  741,  741,  741,    0,  741,    0,    0,  741,
  741,  741,  741,  741,    0,    0,    0,  741,  741,    0,
    0,    0,  741,  741,  741,  741,  741,  741,  741,  741,
    0,  741,    0,    0,    0,    0,    0,    0,    0,  712,
    0,  741,  712,    0,    0,    0,    0,    0,    0,  712,
    0,  712,    0,    0,    0,  712,    0,    0,    0,    0,
  712,    0,    0,  712,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  712,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,  712,    0,  116,    0,  117,  118,  119,  120,
  712,  121,    0,    0,  712,    0,    0,    0,    0,  122,
  123,    0,    0,  712,    0,    0,    0,    0,    0,    0,
  124,  125,    0,  126,    0,    0,    0,    0,    0,  712,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,  712,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,  712,    0,
  712,  146,    0,    0,    0,    0,  712,    0,    0,  712,
    0,  712,  712,  712,  712,    0,  712,    0,    0,    0,
    0,    0,    0,    0,  712,  712,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  712,  712,    0,  712,    0,
    0,    0,    0,    0,    0,  712,  712,  712,  712,  712,
    0,  712,    0,    0,  712,  712,  712,  712,  712,    0,
    0,    0,  712,  712,  915,    0,    0,  712,  712,  712,
  712,  712,  712,  712,  712,    0,  712,    0,  739,    0,
    0,  739,    0,    0,    0,    0,  712,    0,  739,    0,
  739,    0,    0,    0,  739,    0,    0,    0,    0,  739,
    0,    0,  739,    0,    0,  915, 1109, 1109,  915, 1109,
 1109, 1109,    0,    0,    0,    0,    0,  739,    0,    0,
    0,    0,    0,  915, 1109, 1109, 1109,    0,    0,    0,
  115,  739,    0,  116,    0,  117,  118,  119,  120,  739,
  121,    0,    0,  739,    0,    0,    0,    0,  122,  123,
    0,    0,  739,    0,    0, 1109,    0,    0,    0,  124,
  125,    0,  126,    0,    0,    0,    0,    0,  739,  127,
  128,    0,  129,  130,    0,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  140,  141,  739,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,    0,  739,    0,  739,
  146,    0,  375,    0,    0,  712,    0,    0,  712,    0,
  712,  712,  712,  712,    0,  712,    0,    0,  376,    0,
    0,    0,    0,  712,  712,    0,    0,    0,    0,  377,
    0,    0,    0,    0,  712,  712,  378,  712,    0,    0,
  379,  380,    0,    0,  712,  712,  739,  712,  712,  381,
  712,  916,    0,  712,  712,  712,  712,  712,    0,    0,
  382,  712,  712,    0,    0,    0,  739,  712,  712,  712,
  739,  712,  712,  712,    0,  712,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  712,  821,  819,    0,  820,
    0,  822,  916, 1110, 1110,  916, 1110, 1110, 1110,    0,
    0,    0,    0,    0, 1027, 1026, 1028,    0,    0,    0,
  916, 1110, 1110, 1110,  383,    0,  915,    0,    0,    0,
    0,    0,  384,  875,    0, 1109,    0,    0,    0,    0,
    0, 1109,    0,    0,    0,    0, 1109,    0,    0, 1109,
    0,    0, 1110,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  385,    0,    0,    0,  386,    0,    0,
    0,    0,    0,    0,  875,    0,    0,  875,  387,  915,
    0,    0,  915,    0,    0, 1109,    0,    0,    0,  915,
    0,  915,  875, 1109,    0,  915,    0,  388,  389,    0,
  915, 1109,    0,  915,    0,  873,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  915, 1109,
    0, 1109,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  915,    0, 1109,    0, 1109, 1109, 1109,    0,
  915,    0,    0,  915,  915,    0,  873,    0,    0,  873,
    0,    0,    0,  915,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,  873,  874,    0,    0,    0,  915,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1109,    0,    0,    0,    0,
    0,  915,    0,    0,    0,    0,  915,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  874,    0,  915,  874,
  915,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  916,  874, 1014,    0,    0,    0,    0,
    0,  817, 1110,    0,    0,    0,  818,    0, 1110, 1015,
    0,    0,    0, 1110,    0,    0, 1110,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  915,    0,    0,
  906,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1016,  916,  915,    0,  916,
    0,  915, 1110, 1017,    0,    0,  916,    0,  916,    0,
 1110, 1018,  916,    0,    0,  875,    0,  916, 1110,    0,
  916,  906,    0,    0,  906,    0,    0,    0,    0, 1019,
    0, 1020,    0,    0,    0,  916, 1110,    0, 1110,  906,
    0,    0,    0,    0, 1021,    0, 1022, 1023, 1024,  916,
    0, 1110,  868, 1110, 1110, 1110,    0,  916,    0,    0,
  916,  916,    0,    0,    0,    0,    0,    0,  875,    0,
  916,  875,    0,    0,    0,    0,    0,    0,  875,    0,
  875,    0,    0,    0,  875,    0,  916,  873,    0,  875,
    0,    0,  875,  868,    0, 1025,  868,    0,    0,    0,
    0,    0, 1110,    0,    0,    0,    0,  875,  916,    0,
    0,  868,    0,  916,    0,    0,    0,    0,    0,    0,
    0,  875,    0,    0,  869,  916,    0,  916,    0,  875,
    0,    0,  875,  875,    0,    0,    0,    0,    0,    0,
  873,    0,  875,  873,    0,    0,    0,  874,    0,    0,
  873,    0,  873,    0,    0,    0,  873,    0,  875,    0,
    0,  873,    0,    0,  873,  869,    0,    0,  869,    0,
    0,    0,    0,    0,  916,    0,    0,    0,    0,  873,
  875,    0,    0,  869,  870,  875,    0,    0,    0,    0,
    0,    0,    0,  873,  916,    0,    0,  875,  916,  875,
  874,  873,    0,  874,  873,  873,    0,    0,    0,    0,
  874,    0,  874,    0,  873,    0,  874,    0,    0,    0,
    0,  874,    0,    0,  874,  870,    0,    0,  870,    0,
  873,    0,    0,    0,    0,    0,    0,    0,    0,  874,
    0,    0,  906,  870,    0,    0,  875,    0,    0,    0,
    0,    0,  873,  874,    0,    0,  905,  873,    0,    0,
    0,  874,    0,    0,  874,  874,  875,    0,    0,  873,
  875,  873,    0,    0,  874,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  874,    0,    0,    0,    0,  906,    0,  905,  906,    0,
  905,    0,    0,    0,    0,  906,    0,  906,    0,    0,
    0,  906,  874,    0,  868,  905,  906,  874,  873,  906,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  874,
    0,  874,    0,    0,  906,    0,    0,    0,  873,    0,
  899,    0,  873,    0,    0,    0,    0,    0,  906,    0,
    0,    0,    0,    0,    0,    0,  906,    0,    0,  906,
  906,    0,    0,    0,    0,    0,    0,  868,    0,  906,
  868,    0,    0,    0,    0,    0,    0,  868,  874,  868,
    0,  899,    0,  868,  899,  906,  869,    0,  868,    0,
    0,  868,    0,    0,    0,    0,    0,    0,  874,  899,
    0,    0,  874,    0,    0,    0,  868,  906,    0,    0,
    0,    0,  906,    0,    0,    0,    0,    0,    0,    0,
  868,    0,    0,  907,  906,    0,  906,    0,  868,    0,
    0,  868,  868,    0,    0,    0,    0,    0,    0,  869,
    0,  868,  869,    0,    0,    0,  870,    0,    0,  869,
    0,  869,    0,    0,    0,  869,    0,  868,    0,    0,
  869,    0,    0,  869,  907,    0,    0,  907,    0,    0,
    0,    0,    0,  906,    0,    0,    0,    0,  869,  868,
    0,    0,  907,    0,  868,    0,    0,    0,    0,    0,
    0,    0,  869,  906,    0,  908,  868,  906,  868,  870,
  869,    0,  870,  869,  869,    0,    0,    0,    0,  870,
    0,  870,    0,  869,    0,  870,    0,    0,  905,    0,
  870,    0,    0,  870,    0,    0,    0,    0,    0,  869,
    0,    0,    0,    0,    0,    0,  908,    0,  870,  908,
    0,    0,    0,    0,    0,  868,    0,    0,    0,    0,
    0,  869,  870,    0,  908,  896,  869,    0,    0,    0,
  870,    0,    0,  870,  870,  868,    0,    0,  869,  868,
  869,  905,    0,  870,  905,    0,    0,    0,    0,    0,
    0,  905,    0,  905,    0,    0,    0,  905,    0,  870,
    0,    0,  905,    0,    0,  905,  896,    0,    0,  896,
    0,    0,  899,    0,    0,    0,    0,    0,    0,    0,
  905,  870,    0,    0,  896,    0,  870,  869,    0,    0,
    0,    0,    0,    0,  905,    0,    0,    0,  870,    0,
  870,    0,  905,    0,    0,  905,  905,  869,    0,    0,
    0,  869,    0,    0,    0,  905,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  899,    0,    0,  899,    0,
    0,  905,    0,    0,    0,  899,  894,  899,    0,    0,
    0,  899,    0,    0,    0,    0,  899,  870,    0,  899,
    0,    0,    0,  905,    0,  907,    0,    0,  905,    0,
    0,    0,  571,    0,  899,    0,    0,  870,    0,  900,
  905,  870,  905,    0,    0,    0,    0,  894,  899,    0,
  894,    0,    0,    0,    0,    0,  899,    0,    0,  899,
  899,    0,    0,    0,    0,  894,    0,    0,    0,  899,
    0,    0,  571,  571,    0,    0,  571,    0,  907,    0,
  900,  907,    0,  900,    0,  899,    0,    0,  907,  905,
  907,  571,    0,  571,  907,    0,    0,  908,  900,  907,
    0,    0,  907,    0,    0,    0,    0,  899,    0,  905,
    0,    0,  899,  905,    0,    0,    0,  907,    0,    0,
    0,    0,    0,  571,  899,    0,  899,    0,  909,    0,
    0,  907,    0,    0,    0,    0,    0,    0,    0,  907,
    0,    0,  907,  907,    0,    0,    0,    0,    0,    0,
  908,    0,  907,  908,    0,    0,    0,  896,    0,    0,
  908,    0,  908,    0,    0,    0,  908,    0,  907,  909,
    0,  908,  909,  899,  908,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  909,    0,  908,
  907,    0,    0,  899,    0,  907,    0,  899,    0,    0,
  895,    0,    0,  908,    0,    0,    0,  907,    0,  907,
  896,  908,    0,  896,  908,  908,    0,    0,    0,    0,
  896,    0,  896,    0,  908,    0,  896,    0,    0,    0,
    0,  896,    0,    0,  896,    0,    0,    0,    0,    0,
  908,  895,    0,    0,  895,    0,    0,    0,    0,  896,
    0,    0,    0,    0,    0,    0,  907,    0,    0,  895,
    0,    0,  908,  896,    0,    0,    0,  908,  894,    0,
    0,  896,    0,    0,  896,  896,  907,    0,    0,  908,
  907,  908,    0,    0,  896,    0,    0,    0,    0,    0,
  571,    0,    0,    0,    0,    0,    0,    0,    0,  897,
  896,  900,    0,    0,    0,  571,    0,  571,  571,  571,
    0,    0,    0,  571,    0,    0,  571,    0,    0,    0,
    0,  894,  896,    0,  894,    0,    0,  896,  908,  571,
    0,  894,  901,  894,    0,  571,    0,  894,  571,  896,
  897,  896,  894,  897,    0,  894,    0,    0,  908,    0,
    0,    0,  908,    0,  900,    0,    0,  900,  897,    0,
  894,    0,    0,    0,  900,    0,  900,    0,    0,    0,
  900,    0,    0,  901,  894,  900,  901,    0,  900,    0,
  571,    0,  894,    0,    0,  894,  894,    0,  896,    0,
  909,  901,    0,  900,    0,  894,    0,    0,    0,    0,
    0,    0,    0,    0,  571,    0,    0,  900,  896,    0,
    0,  894,  896,    0,    0,  900,    0,    0,  900,  900,
    0,    0,    0,    0,    0,  571,    0,    0,  900,  846,
    0,  571,    0,  894,    0,  571,    0,    0,  894,    0,
    0,    0,  571,  909,  900,    0,  909,    0,    0,    0,
  894,    0,  894,  909,    0,  909,    0,    0,    0,  909,
  571,    0,  895,    0,  909,  571,  900,  909,    0,    0,
  846,  900,    0,  846,    0,    0,    0,    0,    0,    0,
    0,    0,  909,  900,    0,  900,    0,    0,  846,    0,
  571,    0,    0,    0,  571,    0,  909,    0,    0,  894,
    0,    0,    0,    0,  909,    0,    0,  909,  909,    0,
    0,    0,    0,    0,    0,  895,    0,  909,  895,  894,
    0,    0,    0,  894,    0,  895,  723,  895,    0,    0,
    0,  895,  900,  909,    0,    0,  895,    0,    0,  895,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  900,    0,  895,  909,  900,    0,    0,  725,
  909,  897,    0,    0,    0,    0,  723,  723,  895,    0,
  723,    0,  909,    0,  909,    0,  895,    0,    0,  895,
  895,    0,    0,    0,    0,  723,    0,    0,    0,  895,
    0,    0,    0,    0,  901,    0,    0,    0,    0,    0,
  725,    0,    0,  725,    0,  895,    0,    0,    0,    0,
    0,    0,    0,    0,  897,    0,    0,  897,  725,    0,
    0,  909,    0,    0,  897,    0,  897,  895,    0,    0,
  897,    0,  895,    0,    0,  897,    0,    0,  897,    0,
    0,  909,    0,    0,  895,  909,  895,  901,  736,    0,
  901,    0,    0,  897,    0,    0,    0,  901,    0,  901,
    0,    0,    0,  901,    0,    0,    0,  897,  901,    0,
    0,  901,    0,    0,    0,  897,    0,    0,  897,  897,
    0,    0,    0,    0,    0,    0,  901,    0,  897,  736,
    0,    0,  736,  895,    0,    0,    0,    0,    0,    0,
  901,    0,    0,    0,  897,    0,    0,  736,  901,    0,
    0,  901,  901,  895,    0,    0,  218,  895,    0,  218,
    0,  901,    0,    0,    0,    0,  897,    0,    0,    0,
    0,  897,  218,    0,    0,    0,  218,  901,    0,  218,
    0,    0,    0,  897,  846,  897,    0,  846,    0,    0,
    0,    0,    0,    0,  846,    0,  846,    0,    0,  901,
  846,    0,    0,    0,  901,  846,    0,    0,  846,    0,
    0,    0,    0,    0,    0,    0,  901,  759,  901,  218,
  513,    0,  514,  846,    0,  218,  218,  218,    0,  218,
  218,  218,  897,  218,    0,  516,    0,  846,    0,    0,
  334,    0,    0,    0,    0,  846,    0,    0,  846,  846,
    0,  218,  897,  218,    0,    0,  897,    0,  846,    0,
    0,    0,    0,    0,    0,  901,    0,    0,    0,    0,
    0,    0,    0,    0,  846,    0,    0,    0,    0,    0,
    0,  723,  218,    0,  723,  901,    0,   81,    0,  901,
    0,  723,    0,  723,    0,    0,  846,  723,    0,  218,
    0,  846,  723,    0,    0,  723,    0,    0,    0,    0,
    0,    0,    0,  846,  725,  846,    0,  725,    0,    0,
  723,  218,    0,    0,  725,    0,  725,    0,    0,    0,
  725,    0,    0,    0,  723,  725,    0,    0,  725,    0,
    0,    0,  723,    0,    0,    0,  723,    0,    0, 1103,
    0,    0,    0,  725,    0,  723,  218,  218,    0,    0,
    0,    0,  846,    0,    0,    0,    0,  725,    0,    0,
    0,  723,    0,  218,    0,  725,    0,    0,    0,  725,
    0,    0,  846,    0,    0,    0,  846,    0,  725,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  723,    0,
    0,    0,    0,  736,  725,    0,  736,    0,    0,    0,
  723,    0,  723,  736,    0,  736,    0,    0,    0,  736,
    0,    0,    0,    0,  736,    0,    0,  736,    0,    0,
    0,  725,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  736,  725,  469,  725,    0,    0,    0,    0,
    0,  470,    0,    0,    0,    0,  736,    0,    0,  723,
    0,    0,    0,  471,  736,    0,    0,    0,  736,  472,
  473,    0,    0,  759,    0,    0,  513,  736,  514,  723,
    0,    0,    0,  723,    0,    0,    0,    0,  753,    0,
    0,  516,  725,  736,    0,    0,  334,  474,    0,    0,
  475,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  725,    0,    0,    0,  725,    0,    0,    0,
  736,    0,    0,    0,    0,  476,    0,  477,  478,    0,
    0,    0,  736,    0,  736,    0,    0,  479,    0,    0,
    0,  480,    0,    0,    0,    0,    0,    0,    0, 1006,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  736,    0,    0,    0,    0,   14,    0,    0,    0,
  755,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,  736,  482,    0,    0,  736,    0,  709,    0,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,  485,  486,  487,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  488,    0,    0,    0,    0,    0,    0,  122,  123,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  124,  498,
    0,  126,    0,    0,  499,  500,    0,  501,  502,  503,
    0,  129,  130,  504,  131,    0,    0,  132,  133,  134,
  135,  136, 1007, 1008, 1009,  137,  138,    0,    0,    0,
    0,  139,  505,  141,    0,  506,  143,  144,    0,  145,
  469,  507,  508,  509,    0,  510,  511,  470,  512,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,  752,    0,    0,  472,  473,    0,    0,  759,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  753,    0,    0,  516,    0,    0,
    0,    0,  334,  474,    0,    0,  475,    0,    0,  115,
    0,    0,  116,    0,  117,  118,  119,  120,    0,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123,    0,
    0,  476,    0,  477,  478,    0,    0,    0,  124,  125,
    0,  126,    0,  479,    0,    0,    0,  480,  127,  128,
    0,  129,  130,    0,  131,  754,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,    0,  142,  143,  144,    0,  145,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  146,
    0,    0,    0,    0,    0,    0,  755,    0,    0,    0,
    0,    0,  481,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,   85,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  709,    0,  484,  485,  486,  487,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  488,    0,    0,    0,
    0,    0,    0,  122,  123,  489,  490,  491,  492,  493,
  494,  495,  496,  497,  124,  498,    0,  126,    0,    0,
  499,  500,    0,  501,  502,  503,    0,  129,  130,  504,
  131,    0,    0,  132,  133,  756,  757,  758,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  505,  141,
    0,  506,  143,  144,    0,  145,  469,  507,  508,  509,
    0,  510,  511,  470,  512,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,    0,    0,    0,
    0,  472,  473,    0,    0,  759,    0,    0,  513,    0,
  514,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  753,    0,    0,  516,    0,    0,    0,    0,  334,  474,
    0,    0,  475,    0,    0,  712,    0,    0,  712,    0,
  712,  712,  712,  712,    0,  712,    0,    0,    0,    0,
    0,    0,    0,  712,  712,    0,    0,  476,    0,  477,
  478,    0,    0,    0,  712,  712,    0,  712,    0,  479,
    0,    0,    0,  480,  712,  712,    0,  712,  712,    0,
  712, 1002,    0,  712,  712,  712,  712,  712,    0,    0,
    0,  712,  712,    0,    0,    0,    0,  712,  712,  712,
    0,  712,  712,  712,    0,  712,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  712,    0,    0,    0,    0,
    0,    0,  755,    0,    0,    0,    0,    0,  481,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  516,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  756,  757,  758,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,  469,  507,  508,  509,    0,  510,  511,  470,
  512,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,    0,  472,  473,    0,
    0,  586,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  753,    0,    0,  516,
    0,    0,    0,    0,  334,  474,    0,    0,  475,   84,
    0,  115,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,    0,    0,    0,    0,    0,    0,  122,
  123,    0,    0,  476,    0,  477,  478,    0,    0,    0,
  124,  125,    0,  126,    0,  479,    0,    0,    0,  480,
  127,  128,    0,  129,  130,    0,  131,  754,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  146,    0,    0,    0,    0,    0,    0,  755,    0,
    0,    0,    0,    0,  481,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,    0,  499,  500,    0,  501,  502,  503,    0,  129,
  130,  504,  131,    0,    0,  132,  133,  756,  757,  758,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,  469,  507,
  508,  509,    0,  510,  511,  470,  512,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,    0,
    0,    0,    0,  472,  473,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  759,    0,    0,
  513,    0,  514,    0,    0,    0,    0,    0,    0,    0,
    0,  474,    0,    0,  475,  516,    0,    0,  115,    0,
  334,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,  476,
    0,  477,  478,    0,    0,    0,    0,  124,  125,    0,
  126,  479,    0,    0,    0,  480,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,  661,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0,    0,    0,    0,
    0,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,    0,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144, 1077,  145,    0,  507,  508,  509, 1078,  510,
  511, 1079,  512,  146,  469,    0, 1082,  821,  819,    0,
  820,  470,  822,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  471,    0, 1027, 1026, 1028,    0,  472,
  473,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  586,    0,    0,  513,  753,  514,
    0,    0,    0,    0,    0,    0,    0,  474,    0,    0,
  475,    0,  516,  115,    0,    0,  116,  334,  117,  118,
  119,  120,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,    0,    0,  476,    0,  477,  478,    0,
    0,    0,  124,  125,    0,  126,    0,  479,    0,    0,
    0,  480,  127,  128,    0,  129,  130,    0,  131, 1002,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,    0,    0,    0,    0,    0,    0,
  755,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,  485,  486,  487,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  488,    0,    0,    0,    0,    0,    0,  122,  123,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  124,  498,
    0,  126,    0,    0,  499,  500,    0,  501,  502,  503,
    0,  129,  130,  504,  131,    0, 1014,  132,  133,  134,
  135,  136,  817,    0,    0,  137,  138,  818,    0,    0,
 1015,  139,  505,  141,    0,  506,  143,  144,    0,  145,
    0,  507,  508,  509, 1267,  510,  511, 1268,  512,  146,
    0,  469,    0,    0,    0,    0,    0,    0,  470,    0,
    0,    0,    0,    0,    0,    0, 1016,    0,    0,    0,
  471,    0,    0,    0, 1017,    0,  472,  473,    0,    0,
    0,    0, 1018,    0,    0,    0,    0,    0,    0,    0,
    0,  586,    0,    0,  513,    0,  514,    0,    0,    0,
 1019,    0, 1020,  567,  474,    0,    0,  475,    0,  516,
    0,    0,    0,    0,  334, 1021,    0, 1022, 1023, 1024,
  115,    0,    0,  116,    0,  117,  118,  119,  120,    0,
  121,    0,  476,    0,  477,  478,    0,    0,  122,  123,
    0,    0,    0,    0,  479,    0,    0,    0,  480,  124,
  125,    0,  126,    0,    0,    0,    0,    0,    0,  127,
  128,    0,  129,  130,    0,  131, 1025,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  146, 1269,    0,  481,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,    0,  507,  508,
  509, 1278,  510,  511, 1268,  512,  146,    0,  469,  693,
    0,    0,  694,    0,    0,  470,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,  695,
    0,    0,  696,  472,  473,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  586,    0,
    0,  513,    0,  514,    0,    0,    0,    0,    0,    0,
    0,  474,    0,    0,  475,    0,  516,    0,    0,    0,
    0,  334,  697,    0,    0,    0,    0,    0,  698,  699,
  700,    0,  701,  702,  703,    0,  704,    0,    0,  476,
    0,  477,  478,    0,    0,    0,    0,    0,    0,    0,
    0,  479,    0,    0,  705,  480,  706,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  707,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  708,    0,    0,    0,    0,    0, 1269,    0,
  481,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,  709,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0,    0,    0,  710,
  711,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,  712,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144,    0,  145,    0,  507,  508,  509, 1282,  510,
  511, 1268,  512,  146,    0,  469,  148,    0,    0,  148,
    0,    0,  470,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  471,    0,  148,    0,    0,  148,
  472,  473,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  586,    0,    0,  513,    0,
  514,    0,    0,    0,    0,    0,    0,    0,  474,    0,
    0,  475,    0,  516,    0,    0,    0,    0,  334,  148,
    0,    0,    0,    0,    0,  148,  148,  148,    0,  148,
  148,  148,    0,  148,    0,    0,  476,    0,  477,  478,
    0,    0,    0,    0,    0,    0,    0,    0,  479,    0,
    0,  148,  480,  148,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  148,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  148,
    0,    0,    0,    0,    0, 1269,    0,  481,    0,    0,
    0,    0,    0,  482,    0,    0,    0,    0,    0,    0,
    0,  148,    0,  483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  484,  485,  486,
  487,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  488,    0,    0, 1364,    0,  148,  148,  122,  123,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  124,
  498,    0,  126,  148,    0,  499,  500,    0,  501,  502,
  503,    0,  129,  130,  504,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  505,  141,    0,  506,  143,  144,    0,
  145,    0,  507,  508,  509, 1290,  510,  511, 1268,  512,
  146,    0,  469,    0, 1365,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,    0,  472,  473,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1366,
    0,    0,  586,    0, 1367,  513,    0,  514,    0,    0,
 1368,    0,    0,    0,    0,  474,    0,    0,  475,    0,
  516,  115,    0,    0,  116,  334,  117,  118,  119, 1369,
    0,  121,    0,    0,    0,    0,    0,    0,    0,  122,
  123,    0,    0,  476,    0,  477,  478,    0,    0,    0,
  124,  125,    0,  126,    0,  479,    0,    0,    0,  480,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1269,    0,  481,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,    0,  499,  500,    0,  501,  502,  503,    0,  129,
  130,  504,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,    0,  507,
  508,  509, 1293,  510,  511, 1268,  512,  146,    0,  469,
    0,    0,    0,    0,    0,    0,  470,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
    0,    0,    0,    0,  472,  473,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,  474,    0,    0,  475,    0,  516,  115,    0,
    0,  116,  334,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,    0,
  476,    0,  477,  478,    0,    0,    0,  124,  125,    0,
  126,    0,  479,    0,    0,    0,  480,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1269,
    0,  481,    0,    0,    0,    0,    0,  482,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  483,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,  485,  486,  487,  255,  256,  116,    0,  117,
  118,  119,  120,    0,  121,  488,    0,    0,    0,    0,
    0,    0,  122,  123,  489,  490,  491,  492,  493,  494,
  495,  496,  497,  124,  498,    0,  126,    0,    0,  499,
  500,    0,  501,  502,  503,    0,  129,  130,  504,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  505,  141,    0,
  506,  143,  144,    0,  145,    0,  507,  508,  509, 1298,
  510,  511, 1268,  512,  146,    0,  469,    0,    0,    0,
    0,    0,    0,  470,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,    0,    0,    0,
    0,  472,  473,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  586,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,  474,
    0,    0,  475,    0,  516,   28,    0,    0,   28,  334,
   28,   28,   28,   28,    0,   28,    0,    0,    0,    0,
    0,    0,    0,   28,   28,    0,    0,  476,    0,  477,
  478,    0,    0,    0,   28,   28,    0,   28,    0,  479,
    0,    0,    0,  480,   28,   28,    0,   28,   28,    0,
   28,    0,    0,   28,   28,   28,   28,   28,    0,    0,
    0,   28,   28,    0,    0,    0,    0,   28,   28,   28,
    0,   28,   28,   28,    0,   28,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   28,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1269,    0,  481,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,    0,  507,  508,  509, 1301,  510,  511, 1268,
  512,  146,    0,  469,    0,    0,    0,    0,    0,    0,
  470,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,    0,    0,    0,  472,  473,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  586,    0,    0,  513,    0,  514,    0,
    0,    0,    0,    0,    0,    0,  474,    0,    0,  475,
    0,  516,  604,    0,    0,  604,  334,  604,  604,  604,
  604,    0,  604,    0,    0,    0,    0,    0,    0,    0,
  604,  604,    0,    0,  476,    0,  477,  478,    0,    0,
    0,  604,  604,    0,  604,    0,  479,    0,    0,    0,
  480,  604,  604,    0,  604,  604,    0,  604,    0,    0,
  604,  604,  604,  604,  604,    0,    0,    0,  604,  604,
    0,    0,    0,    0,  604,  604,  604,    0,  604,  604,
  604,    0,  604,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  604,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1269,    0,  481,    0,    0,    0,    0,
    0,  482,  667,    0,    0,    0,    0,    0,    0,    0,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,  487,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  488,
    0,    0,    0,    0,    0,    0,  122,  123,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  124,  498,    0,
  126,    0,    0,  499,  500,    0,  501,  502,  503,    0,
  129,  130,  504,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  505,  141,    0,  506,  143,  144,    0,  145,    0,
  507,  508,  509, 1304,  510,  511, 1268,  512,  146,    0,
  469,    0,    0,    0,    0,    0,    0,  470,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,    0,    0,    0,  472,  473,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  696,    0,  696,  696,
    0,  696,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  474,  696,    0,  475,    0,    0,  696,
  115,    0,    0,  116,    0,  117,  118,  119,  120,    0,
  121,    0,    0,    0,    0,    0,    0,    0,  122,  123,
    0,  476,    0,  477,  478,    0,    0,    0,    0,  124,
  125,    0,  126,  479,    0,    0,    0,  480,    0,  127,
  128,    0,  129,  130,    0,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  146,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1269,    0,  481,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  484,  485,  486,  487,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  488,    0,    0,    0,
    0,    0,    0,  122,  123,  489,  490,  491,  492,  493,
  494,  495,  496,  497,  124,  498,    0,  126,    0,    0,
  499,  500,    0,  501,  502,  503,    0,  129,  130,  504,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  505,  141,
    0,  506,  143,  144,    0,  145,  696,  507,  508,  509,
    0,  510,  511,  696,  512,  146,    0,    0,    0,    0,
  696,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  696,    0,    0,    0,    0,    0,  696,  696,
    0,    0,    0,    0,    0,    0,  696,    0,    0,  690,
    0,  690,  690,    0,  690,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  696,  690,    0,  696,
    0,    0,  690,  287,    0,    0,  287,    0,  287,  287,
  287,  287,    0,  287,    0,    0,    0,    0,    0,    0,
    0,  287,  287,    0,  696,    0,  696,  696,    0,    0,
    0,    0,  287,  287,    0,  287,  696,    0,    0,    0,
  696,    0,  287,  287,    0,  287,  287,    0,  287,    0,
    0,  287,  287,  287,  287,  287,    0,    0,    0,  287,
  287,    0,    0,    0,    0,  287,  287,  287,    0,  287,
  287,  287,    0,  287,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  287,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  696,    0,    0,    0,    0,
    0,  696,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  696,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  696,  696,  696,  696,  696,
  696,  696,    0,  696,  696,  696,  696,    0,  696,  696,
    0,    0,    0,    0,    0,    0,  696,  696,  696,  696,
  696,  696,  696,  696,  696,  696,  696,  696,  696,    0,
  696,    0,    0,  696,  696,    0,  696,  696,  696,    0,
  696,  696,  696,  696,    0,    0,  696,  696,  696,  696,
  696,    0,    0,    0,  696,  696,    0,    0,    0,    0,
  696,  696,  696,    0,  696,  696,  696,    0,  696,  690,
  696,  696,  696,    0,  696,  696,  690,  696,  696,    0,
    0,    0,    0,  690,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  690,    0,    0,    0,    0,
    0,  690,  690,    0,    0,  586,    0,  591,  513,  690,
  514,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  516,    0,    0,    0,    0,  334,  690,
    0,    0,  690,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  690,    0,  690,
  690,    0,    0,    0,    0,    0,    0,    0,    0,  690,
    0,    0,    0,  690,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  690,    0,
    0,    0,    0,    0,  690,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  690,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  690,  690,
  690,  690,  690,  690,  690,    0,  690,  690,  690,  690,
    0,  690,  690,    0,    0,    0,    0,    0,    0,  690,
  690,  690,  690,  690,  690,  690,  690,  690,  690,  690,
  690,  690,    0,  690,    0,    0,  690,  690,    0,  690,
  690,  690,    0,  690,  690,  690,  690,    0,    0,  690,
  690,  690,  690,  690,    0,    0,    0,  690,  690,    0,
    0,    0,    0,  690,  690,  690,    0,  690,  690,  690,
    0,  690,  469,  690,  690,  690,    0,  690,  690,  470,
  690,  690,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,    0,  472,  473,    0,
    0,  586,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  516,
    0,    0,    0,    0,  334,  474,    0,    0,  475,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  476,    0,  477,  478,    0,    0,    0,
    0,    0,    0,    0,    0,  479,    0,    0,    0,  480,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  481,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,    0,  499,  500,    0,  501,  502,  503,    0,  129,
  130,  504,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,  469,  507,
  508,  509,    0,  510,  511,  470,  512,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,    0,
    0,    0,    0,  472,  473,    0,    0,  586,    0,    0,
  513,    0,  514,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  516,    0,    0,    0,    0,
  334,  474,    0,    0,  475,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  476,
    0,  477,  478,    0,    0,    0,    0,    0,    0,    0,
    0,  479,    0,    0,    0,  480,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  483,    0,    0,    0,
    0,    0,    0,    0,  794,    0,    0,    0,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0,    0,    0,    0,
    0,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,    0,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144,    0,  145,  469,  507,  508,  509,    0,  510,
  511,  470,  512,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  471,    0,    0,    0,    0,    0,  472,
  473,    0,    0,  586, 1086,    0,  513,    0,  514,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  516,    0,    0,    0,    0,  334,  474,    0,    0,
  475,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  476,    0,  477,  478,    0,
    0,    0,    0,    0,    0,    0,    0,  479,    0,    0,
    0,  480,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   14,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,  485,  486,  487,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  488,    0,    0,    0,    0,    0,    0,  122,  123,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  124,  498,
    0,  126,    0,    0,  499,  500,    0,  501,  502,  503,
    0,  129,  130,  504,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  505,  141,    0,  506,  143,  144,    0,  145,
  469,  507,  508,  509,    0,  510,  511,  470,  512,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,    0,    0,    0,  472,  473,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  516,    0,    0,
    0,    0,  334,  474,    0,    0,  475,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  476,    0,  477,  478,    0,    0,    0,    0,    0,
    0,    0,    0,  479,    0,    0,    0,  480,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  481,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  484,  485,  486,  487,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  488,    0,    0,    0,
    0,    0,    0,  122,  123,  489,  490,  491,  492,  493,
  494,  495,  496,  497,  124,  498,    0,  126,    0,    0,
  499,  500,    0,  501,  502,  503,    0,  129,  130,  504,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  505,  141,
    0,  506,  143,  144,    0,  145,  469,  507,  508,  509,
    0,  510,  511,  470,  512,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,    0,    0,    0,
    0,  472,  473,    0,    0,  586,    0,    0,  513,    0,
  514,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  516,    0,    0,    0,    0,  334,  474,
    0,    0,  475,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  476,    0,  477,
  478,    0,    0,    0,    0,    0,    0,    0,    0,  479,
    0,    0,    0,  480,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  481,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1296,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,  469,  507,  508,  509,    0,  510,  511,  470,
  512,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,    0,  472,  473,    0,
    0,  586,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  516,
    0,    0, 1349,    0,  334,  474,    0,    0,  475,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  476,    0,  477,  478,    0,    0,    0,
    0,    0,    0,    0,    0,  479,    0,    0,    0,  480,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  481,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,    0,  499,  500,    0,  501,  502,  503,    0,  129,
  130,  504,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,  469,  507,
  508,  509,    0,  510,  511,  470,  512,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,    0,
    0,    0,    0,  472,  473,    0,    0, 1061,    0, 1061,
 1061,    0, 1061,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1061,    0,    0,    0,    0,
 1061,  474,    0,    0,  475,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  476,
    0,  477,  478,    0,    0,    0,    0,    0,    0,    0,
    0,  479,    0,    0,    0,  480,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1516,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0,    0,    0,    0,
    0,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,    0,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144,    0,  145, 1061,  507,  508,  509,    0,  510,
  511, 1061,  512,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1061,    0,    0,    0,    0,    0, 1061,
 1061,    0,    0,  515,    0,    0,  513,    0,  514,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  516,    0,    0,    0,    0,  334, 1061,    0,    0,
 1061,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1061,    0, 1061, 1061,    0,
    0,    0,    0,    0,    0,    0,    0, 1061,    0,    0,
    0, 1061,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1061,    0,    0,    0,
    0,    0, 1061,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1061,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1061, 1061, 1061, 1061,
 1061, 1061, 1061,    0, 1061, 1061, 1061, 1061,    0, 1061,
 1061,    0,    0,    0,    0,    0,    0, 1061, 1061, 1061,
 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061,
    0, 1061,    0,    0, 1061, 1061,    0, 1061, 1061, 1061,
    0, 1061, 1061, 1061, 1061,    0,    0, 1061, 1061, 1061,
 1061, 1061,    0,    0,    0, 1061, 1061,    0,    0,    0,
    0, 1061, 1061, 1061,    0, 1061, 1061, 1061,    0, 1061,
  469, 1061, 1061, 1061,    0, 1061, 1061,  470, 1061, 1061,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,    0,    0,    0,  472,  473,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  516,    0,    0,
    0,    0,  334,  474,    0,    0,  475,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  476,    0,  477,  478,    0,    0,    0,    0,    0,
    0,    0,    0,  479,    0,    0,    0,  480,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  481,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  484,  485,  486,  487,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  488,    0,    0,    0,
    0,    0,    0,  122,  123,  489,  490,  491,  492,  493,
  494,  495,  496,  497,  124,  498,    0,  126,    0,    0,
  499,  500,    0,  501,  502,  503,    0,  129,  130,  504,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  505,  141,
    0,  506,  143,  144,    0,  145,  469,  507,  508,  509,
    0,  510,  511,  470,  512,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,    0,    0,    0,
    0,  472,  473,    0,    0, 1061,    0,    0, 1061,    0,
 1061,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1061,    0,    0,    0,    0, 1061,  474,
    0,    0,  475,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  476,    0,  477,
  478,    0,    0,    0,    0,    0,    0,    0,    0,  479,
    0,    0,    0,  480,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  481,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
  572,  145, 1061,  507,  508,  509,    0,  510,  511, 1061,
  512,  146,  566,    0,    0,    0,    0,    0,    0,    0,
    0, 1061,    0,    0,    0,    0,    0, 1061, 1061,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  572,  572,    0,    0,  572,    0,    0,    0,    0,  588,
    0,    0,    0,  566,    0, 1061,  566,    0, 1061,  572,
    0,  572,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  566,    0,  566,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1061,    0, 1061, 1061,    0,    0,    0,
  588,  572,    0,  588,    0, 1061,    0,    0,    0, 1061,
    0,    0,    0,  566,    0,    0,    0,    0,  588,    0,
  588,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  588,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1061,    0,    0,    0,    0,    0,
 1061,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1061,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1061, 1061, 1061, 1061, 1061, 1061,
 1061,    0, 1061, 1061, 1061, 1061,    0, 1061, 1061,    0,
    0,    0,    0,    0,    0, 1061, 1061, 1061, 1061, 1061,
 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061,    0, 1061,
    0,  578, 1061, 1061,    0, 1061, 1061, 1061,    0, 1061,
 1061, 1061, 1061,    0,    0, 1061, 1061, 1061, 1061, 1061,
    0,    0,    0, 1061, 1061,    0,    0,    0,    0, 1061,
 1061, 1061,    0, 1061, 1061, 1061,    0, 1061,  572, 1061,
 1061, 1061,  578, 1061, 1061,  578, 1061, 1061,    0,    0,
  566,    0,    0,  572,    0,  572,  572,  572,    0,  563,
  578,  572,  578,    0,  572,  566,    0,  566,  566,  566,
    0,    0,    0,  566,    0,    0,  566,  572,    0,    0,
    0,    0,    0,  572,    0,    0,  572,  588,    0,  566,
    0,    0,  578,    0,    0,  566,    0,    0,  566,    0,
  563,    0,  588,  563,    0,  588,  588,    0,    0,    0,
  588,    0,    0,  588,    0,    0,    0,    0,  563,    0,
  563,    0,    0,    0,    0,    0,  588,    0,  572,    0,
    0,    0,  588,    0,    0,  588,    0,    0,    0,    0,
  566,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  563,    0,  572,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  566,    0,    0,    0,    0,    0,
    0,    0,    0,  572,    0,    0,    0,  588,    0,  572,
    0,    0,    0,  572,    0,  566,    0,    0,    0,    0,
  572,  566,    0,    0,    0,  566,    0,    0,    1,    0,
    0,  588,  566,    0,    0,    0,    0,    0,  572,    0,
    0,    0,    0,  572,    0,    0,    2,    0,    0,    0,
  566,    0,  588,    0,    3,  566,    0,    0,  588,    0,
    0,    4,  588,    5,    0,    6,    0,    0,  572,  588,
    7,    0,  572,    0,    0,    0,    0,    8,    0,    0,
  566,    0,    0,    0,  566,    0,    0,  588,    0,  578,
    0,    9,  588,    0,    0,    0,    0,    0,    0,    0,
    0,   10,    0,    0,  578,    0,    0,  578,  578,    0,
    0,    0,  578,    0,    0,  578,    0,  588,    0,    0,
    0,  588,    0,    0,    0,    0,    0,    0,  578,    0,
    0,  108,    0,    0,  578,    0,    0,  578,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  563,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  563,    0,   11,  563,  563,    0,    0,  109,
  563,   12,    0,  563,   13,    0,   14,   15,    0,  578,
    0,  110,    0,    0,    0,    0,  563,    0,    0,    0,
    0,    0,  563,    0,    0,  563,    0,    0,    0,    0,
    0,   16,    0,  578,    0,  111,    0,   49,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  578,    0,    0,    0,    0,    0,
  578,    0,    0,    0,  578,    0,    0,  563,    0,    0,
    0,  578,    0,    0,    0,   49,    0,    0,    0,    0,
   17,    0,    0,    0,    0,    0,    0,   49,    0,  578,
    0,  563,    0,    0,  578,   18,   19,    0,    0,    0,
  112,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  113,   49,  563,    0,    0,    0,    0,    0,  563,  578,
   20,    0,  563,  578,    0,    0,    0,    0,    0,  563,
    0,    0,    0,    0,    0,  114,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  563,    0,    0,
    0,    0,  563,  115,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,    0,    0,    0,   49,  563,    0,    0,
    0,  563,  124,  125,    0,  126,   49,    0,    0,    0,
    0,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,   49,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,  329,    0,    0,    0,   51,
    0,    0,   51,  146,   51,   51,   51,   51,    0,   51,
    0,    0,    0,    0,    0,    0,    0,   51,   51,    0,
    0,    0,    0,  329,    0,    0,    0,    0,   51,   51,
  329,   51,    0,    0,  329,  329,    0,  329,   51,   51,
  329,   51,   51,    0,   51,    0,    0,   51,   51,   51,
   51,   51,    0,  329,    0,   51,   51,    0,    0,  329,
    0,   51,   51,   51,    0,   51,   51,   51,    0,   51,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   51,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1361,    0,    0,    0,    0,  329,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  329,    0,    0,    0,    0,    6,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  329,
 1571, 1572,    0, 1573,    0,    0,    0,    0,    0,  329,
    0,    0,    0,    0,    0,    0,    0,    0,  329, 1574,
  329,    0,    0,    0,    0, 1575,    0,  329,    0,    0,
  329,    0,  329,  329,  329,  329,    0,  329,    0,    0,
    0,    0,    0,    0,    0,  329,  329,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  329,  329,    0,  329,
    0,    0,    0,    0,    0,    0,  329,  329,    0,  329,
  329,    0,  329,    0,    0,  329,  329,  329,  329,  329,
    0,    0, 1576,  329,  329,  329,  329,  329,    0,  329,
  329,  329,    0,  329,  329,  329, 1361,  329,    0,    0,
   14,    0,    0,    0,    0,    0,    0,  329,    0,    0,
    0,    0,    0,    0,    0, 1577,    0,    0,    0,    0,
    0,    0,    0,    0,    6,   16,    0,    0,    0,    0,
    0,    0,    0,    0, 1779, 1571, 1572,    0, 1573,    0,
    0,    0,    0,  487,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121, 1574,    0,    0,    0,    0,    0,
 1575,  122,  123,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1578,  125,    0,  126,    0,    0,    0,    0,
    0,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132, 1579,  134,  135,  136,    0,    0,    0,  137,
  138, 1580, 1581, 1582,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0, 1576,    0,    0,
    0,    0,    0,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   14,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1577,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   16,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  487,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,  693,    0,    0,  694,    0,  122,  123,    0,    0,
  915,    0,    0,    0,    0,    0,    0, 1578,  125,    0,
  126,  695,    0,    0,  696,    0,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132, 1579,  134,  135,
  136,    0,    0,    0,  137,  138, 1580, 1581, 1582,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,  697,    0,    0,    0,  146,    0,
  698,  699,  700,    0,  701,  702,  703,    0,  704,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  705,    0,  706,  693,
    0,    0,  694,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  695,
    0,    0,  696,    0,    0,    0,    0,  707,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  708,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  697,    0,    0,    0,  709,    0,  698,  699,
  700,    0,  701,  702,  703,    0,  704,    0,    0,    0,
    0,  115,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,    0,  705,    0,  706,    0,    0,  122,
  123,  710,  711,    0,    0,    0,    0,    0,    0,    0,
  124,  125,    0,  126,    0,    0,    0,    0,  712,    0,
  127,  128,    0,  129,  130,  707,  131,    0,    0,  132,
  133,  134,  135,  136,    0, 1031,    0,  137,  138,    0,
    0,    0,  708,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0, 1032,    0,
    0,  146,    0,    0,  709,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  115,
    0,    0,  116,    0,  117,  118,  119,  120,    0,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123,  710,
  711,    0,    0,    0,    0,    0,    0,    0,  124,  125,
    0,  126,    0,    0, 1033,    0,  712,    0,  127,  128,
    0,  129,  130,    0,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,    0,  142,  143,  144,  115,  145,
    0,  116,    0,  117,  118,  119,  120,    0,  121,  146,
    0,    0,    0,    0,    0,    0,  122,  123,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  124,  125,    0,
  126,    0,    0,    0,    0,    0,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,  115,    0,  146,  116,
  192,  117,  118,  119,  120,  193,  121,  194,  195,  196,
  197,  198,  199,  200,  122,  123,  201,  202,  203,  204,
  205,    0,    0,  206,  207,  124,  125,  208,  126,    0,
    0,  209,  210,  211,  212,  127,  128,  213,  129,  130,
  214,  131,  215,  216,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,  217,  218,  219,  220,  139,  140,
  141,  221,  142,  143,  144,  222,  145,  223,  224,  225,
  226,  227,  228,  229,  230,  231,  146,  483,    0,    0,
  483,  483,  483,  483,  483,  483,    0,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,    0,    0,  483,  483,  483,  483,  483,  483,
    0,    0,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
    0,    0,    0,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,
};
static short yycheck[] = {                                       9,
    0,    4,   61,  174,   71,   16,   16,  114,   18,  164,
  165,  628,  164,  165,   70,  824,  412,   77,  102,   80,
   70,  447,   75,  515,  447,  443,   93,   99,  662,  880,
  824,  631,  515,  439,  319,   69,  414, 1110,   97,  575,
 1434,  101,  103,  556, 1174,  365,   99, 1360,   58,   59,
  576,   61,   62,   63,   64,   65, 1036,  794, 1130, 1174,
   70,   71,   72,  800,   74,  827,   76,   77,  834,   79,
   80,   81,  336,  740, 1096,  394,  597, 1361,  777,   89,
   91,   91,   92,   93,   63,   95,   96,   97,   98,  100,
  100,  101,  102,  103,  104,  179,  106,   76,   62,   44,
 1174, 1182,   65,  586, 1084,  747,  156, 1174,  158,  176,
   74,  423, 1240,  812,   45, 1379,  878,   41,    0,  270,
 1218,  424,    0,   63,  989,    0,   44,    0,    0,   45,
    0,    0,   40,   96,   98, 1174, 1180,    0,  885,    0,
  653,  104,  106, 1187,  657,  183,  156,  157,  158,  662,
  453,    0,  162, 1197, 1235,   63, 1426, 1427,  679,  169,
   44, 1431,  909,    0,  174,  912,  176,  177,  178,  179,
   41,    0,  279,  183,  921,    0,   44,   59,  157, 1223,
 1174,   59,   44,    0,   59,   41,   59,   59,  889,   59,
   59,  924,   41,    0,  365,  754,   59,    0,   59,  178,
   44,  235,    0,    0,   41,   40,  169,   44,   41,   45,
   59,   44,    0,  177,   41,  614,   41,  368, 1115,   44,
   44,  639,   59,   45,    0,   41, 1432,   44, 1648, 1705,
   59,   61,  266,   44,   59,  982,  662,   44,   41,  662,
   45,   44,   59,   41,   41, 1391,    0,   44,   44,   61,
   40,  283,   59,   41,  262,   40,   59,  264,   63,  276,
  310,   59,   59,  313,    0,  372,  324, 1580, 1581,  665,
  297,   59,  690,   63,  273,  264,  759,  316,  371,    0,
    0,    0,  567,   59,   44,    0,    0, 1096,  347,  257,
   40,  310, 1029, 1030,  272,  262, 1426, 1427,  583,  316,
  310, 1431, 1096,  313, 1659,   59,    0,  344, 1592,  319,
  320, 1426, 1427,   91,  617,  365, 1431,  344,  714,  280,
 1392,  260,   41,   59,  816, 1406,    0,   41,  260,  262,
 1067,  823,   44,  367,    0,   41,    0,  347,   59,   59,
   59,  351,  484,  263,   59,   59,  447,  440,    0, 1704,
  485,  486, 1426, 1427,  393,  395, 1429, 1431,  336, 1426,
 1427,  300,  330,  413, 1431,   59,  315, 1465,  300,    0,
    0,  379,  351,  423,  516,  425,  393,  427, 1798,   41,
  430,   93,  449,  393,  394,   59,  303, 1426, 1427,  309,
  409,  451, 1431,   59,  433,   59,  403,    0,  456, 1472,
  320,  298,  461,  413,  575, 1449,  440,   59,  442, 1156,
   41,   41,  379,  423,  403,  425,  433,  427,    0,  427,
  430,  453,  454,  457,  297, 1405, 1820,  940,   59,   59,
 1177, 1178, 1426, 1427,  297,  262, 1173, 1431,   41,  449,
  450,  451, 1329,    0,  965, 1332,  379, 1006, 1924,  459,
  306,  461, 1199,  264,  464, 1154,   59, 1190, 1191,  458,
 1161,  464,  861, 1776,  990, 1671,  315,  297,  336,   44,
  431,  344,  509,  317,  514, 1001,  325,   59,  327, 1005,
  459,  344,  509,   41,   41,  297,  450,  336, 1385, 1386,
  306,  315, 1638,  410,  427,    0,  445,  275,  380,  419,
   41,  446,   59,    0,  900,  380,  516,  903,  379, 1242,
  450,  380,  315,   41,  389,  344,   44,  315,  315,  450,
  389,  917, 1916, 1260, 1261,  575, 1263, 1264, 1792,  681,
  402,  380,  450,  336,  450,  410,   41,  434,  336,  336,
  389,  301,  450,  264,  264,  462, 1355,  585,  336,  264,
  946,  376,  379,   41,   59,  565,  566,  567,  843,  879,
  444, 1355,   59,  380,  574, 1959,  428, 1234,  578,  444,
  403,  890,  389,  583, 1702,  585,  446, 1649,  628,  730,
  376, 1654,  380,  432, 1120,  446,  565, 1981,  423,  513,
  624, 1357,  856,   41, 1356,  574,  315,  446,  910,  578,
  262,  315,  566, 1684, 1130, 1247, 1471,  641, 1878, 1879,
  682,  325,  662,  327,  450,  993, 1714,  336,  628, 1663,
 1701,  655,  336, 1667, 1668, 1242,  402,  444,  450,  682,
 1122,  513, 1158,  297,  432,  513,  509,  387,  513,  446,
    0,  477,    0,  446,  513,  450,  509,  401,  446,  446,
  684,  661,  662,  824,  439,  477,  493,  667,  446, 1703,
  450, 1945,  315, 1550,  513,  401,  380, 1554,  493,  390,
  390,  365,  338,  379,  428,  389,  831,  980, 1415,    0,
  344,   41,  264,   41,   44,  315, 1970,  361,  352,  739,
  317,  494,  344,  334,  351,  729,  513,    0, 2011,   59,
 1437,   59,  736,  737,  262,  336,  336,  272,  879,  317,
  513,   44,  864,  260, 1998,  513,  513,  379,  432,    0,
   41,  262,  386, 1470,  388, 2009,  301,  446, 2012,  739,
  444,  397,  446,  336,  400, 2019,    0,  790,   59,  754,
 1720,   42,  298, 1592,  759,  380,  410,  344, 1878, 1879,
  416,  260,  262,  300,    0, 1777,   59,  268,  315, 1832,
   41, 1167, 1806, 1878, 1879, 1838, 1810,  264,  325,    0,
  327,  435,  782,  257,  262,  265,  442,    0,   59,  336,
 1272, 1825,  832,  352,  834,    0,  836,  498, 1184, 1272,
 1186,  300,  445,  434,  513,   59,  948,  432, 1842,  513,
    0, 1652, 1198, 1502, 1878, 1879,  348,  817,  390,  299,
  315, 1878, 1879,   59,  262,  446,  446,  827,    0,  782,
  428,  379,  832,  380,  834,  298,  836,  484,   59,  879,
 1236,  336,  389,  843,  884, 1435,   59,  396,  379, 1878,
 1879,  851,  852,  446,   59,  509,  330, 1339,  359,  859,
  860,   41,  421,  412,   44,  889,  453,  454,    0,   59,
  910,  334,  896,  873,  936,  875,   42, 1245,  878,  379,
 1757,   47,  851,  907,  884,  432,  371,   59,  434, 1923,
  890,  401, 1926,  936, 1878, 1879,    0,  937,  852,  446,
  940,  379,  945,  390,  873,    0,  875,  303, 1390,    0,
  910,    0, 1536,  975,  976,  977,  376,    0,  428,  962,
   59,  453,  454,    0, 1651,  410, 1938,   59, 1940, 1963,
  954,  395,  975,  976,  977, 1096, 1948,  937, 1950,  262,
  940,  379,  273,    0,  318, 1380, 1314, 1458,    0,    0,
 1461,  446,    0,   44,    0,   59,  956,  331,   41, 1120,
    0,   44,    0,  956,   59,  315,  513,  315,   59,  260,
   59,  264,  457,  315,    0,  325,   59,  327, 1777,  327,
  354,  981,   59, 1995,   41,  319,  336,   44,  336,  339,
  309,  991,  298, 1777,  368,   41,  303, 1002,  275,  999,
 1000, 1006,   59,   41,  315,  259,  999,   59,   59,  300,
  264,   59,  981,   59,  410,  269,    0,  348,  513,   59,
 1455,   59,  991,  304,  305,  336, 1499, 1500,  334,  401,
  380,  405,  380,   59, 1507, 1508,  500, 1510, 1511,  389,
    0,  389,    0, 1770,  315, 1518, 1519, 1520, 1521,    0,
 1523, 1524, 1525, 1526, 1527, 1528,  379,   41,  277,  264,
   44, 1447, 1565,  282,  341,  336,  462, 1475,    0,  304,
  305, 1071,  261,  397,    0,   59,  395,  304,  305, 1221,
 1120,   41,  432,  390,  432,  305,  276,  277,    0, 1364,
  322, 1477, 1367,  273,  444,  315,  446,  390,  446,   59,
  300,   59,  297,  410,  276,  277, 1945,  426,   59,   41,
   41,  277,   44,   44, 1114, 1115,  282,  338, 1071,   40,
 1536,  428,  441, 1536,  448,  338,  306,   59,  434,   41,
 1130, 1970,  305,   59,  280,  446,  390, 1161,  397, 1938,
    0, 1940,  315,  517,  276,  277,    0,   59,  348, 1948,
  371, 1950,  450,  348, 1938,  462, 1940,  333,  371, 1998,
  264,   93,  443,  513, 1948,  513, 1950, 1229,  348,  264,
 2009, 1898,  372, 2012,  450,  446,  397,    0, 1905,  400,
 2019,  258, 1244,  265,  397,  390, 1229,  400, 1250,  448,
  422,  319,  368,  270, 1355,  416, 1995,  429, 1238,   59,
 1361, 1244, 1242,  416,  380,   59,  260, 1250,  443,  319,
 1210, 1995,  264,  264,    0,    0,  443,  299,  258,  309,
  297,  442, 1215,  403,    0,  445,  303, 1227,  305,  442,
  270,  377,  315, 1378, 1379, 1380,   59,    0, 1238,   44,
  372,  271, 1242, 1951,  327,  394,  300,   42,   43,   40,
   45,  333,   47,  336,    0,  285,   41,  297,  315,   44,
  376,   42,   43,  303,   45,  305,   47,  344,  325, 1411,
  327, 1413,  445,   59,   59,  352,    0,  315,  458,  336,
  339,  258,  339,   59,    0,  431,  390, 1673,   93,  327,
  336, 1999,  276,  277, 1368,  390,   59,  380,  336,  329,
  284,  376,  448,  362,  344,  395,  389,  258,   93, 1310,
 1310, 1793,  338,   59, 1360,   44, 1366, 1357, 1369,  270,
 1360, 1361, 1708,  380,   44,   41,  303,  357,   44,  378,
  260, 1473,  389,  313,  314,   59,    0,  333,  390,  390,
  430,    0,  380,   59,  421,  371,  297, 1954,    0,  432,
  238,  389,  303,  300,  305,  315, 1356, 1357,    0,  418,
 1360, 1361,  378,  446, 1364, 1365, 1366, 1367, 1368, 1369,
  300,  397,  368,   93,  400,  432,  336,   41,   42,   43,
   44,   45,   46,   47,  380, 1385, 1386,  444,  372,  446,
  416,  421, 1392,  344,  432,   59,   60,   61,   62,    0,
   59,  348,  290,  315,  336,   44,    0,   59,  446,  393,
  298, 1365,  338,  486,    0,  399,  442,   59,   42,   43,
  380,   45,  309,   47,  336,  318,  457,   91,  459,  389,
  513, 1592,  509,  410,    0,    0,   42,   43,  331,   45,
   41,   47,  410,   44,    0,  371,  333,   42,   43,  433,
   45,  428,   47, 1839,   93,  342,  513,  399,   59,  401,
 1453,  354, 1486,  318,  409,   59,    0, 1491,  380,  509,
  402,  397,  432,   59,  400,  513,  331,  389,   42,   43,
 1641,   45,  277,   47,  339,  462,  446,  282,    0,    0,
  416,  436,  333,   59,   59,  258,  277,   42,   43,  354,
   45,  282,   47,   59,    0,   42,   43,  270,   45,  493,
   47,  398,  405,  368, 1571,  402,  442,   41,   42,   43,
  432,   45,  259,   47,  284,   59,  397,  434, 1529, 1529,
   41,    0,  269,   44,  446,  372,  318,  297,  446,  352,
  303,    0,  305,    0,  513,  416,    0,   59,   59,  331,
  405,  336, 1592,  513,  258,  336,  393,  261,  509,  298,
  276,  277,  399,   59,   42,   43,  450,   45,  284,   47,
    0, 1571,  354,  386, 1395,  388, 1397,  338,  273, 1579,
 1580, 1581,   42,   43,   44,   45,  368,   47,  450,  452,
   59,    0, 1592,  477,  376,  334,  433,  410,  262,  303,
   59,  513,   59,   42,   43,   59,   45,  271,   47,  485,
  371,  409,  337,  277,  501,  477, 1777,  402,  282,  495,
  345,  285,  435,  405, 1624, 1625,  513,  514,  515,   59,
   46,  520,   41,  428, 1795,   44,  397,   41,  436,  400,
   44, 1641,   58,   59, 1965,   61, 1967, 1792, 1648, 1649,
   59,  315,   40,  277,  318,  416,  372,  321,  282,  259,
  399,  325,  517,  327,  416,  329,  453,  331,  264,  269,
  409,  277,  336,  337, 1674,  339,  282,  393,  313,    0,
    0,  442,  277,  399,  308,   46,  315,  282,  333,  258,
  354,  355,  332,  357,  444,  434,  403,   58, 1747,  586,
   61,  270,  395,  590,  368, 1674,  370,  426,  372,  373,
  374,  493,  376,  277,  336,  379,  380,  433,  282,  264,
   41,   41,  317,   44,   44,  389, 1823,  317,   41,   40,
  336,   44,  277,  315,  303,  517,  305,  282,   59,   59,
  277,  405,  315, 1744, 1744,  282,  376, 1747,  376,   46,
  498,  315,   41,  277,  428,   44,    0,  421,  282,  453,
  454,   58,   59,  427,   61,  276,  277,   41,  432,  356,
   44,  308,    0,  284,  442, 1824, 1776, 1938,   41, 1940,
  444,   44,  446,   41, 1945,   41,   44, 1948,   44, 1950,
  426,  384,  385,  317, 1784, 1795,  377,   41, 1798,  277,
   44,   41,   42,   43,  282,   45,  319,   47,   41, 1970,
  337,   44,  410,   41, 1814,   59,   44,  277,    0, 1819,
 1917, 1845,  282,  375, 1824, 1849,  265,   42,   43,  493,
   45,   59,   47, 1890, 1995,  453,  454, 1998,  277,  317,
  497,   41,  409,  282,   44,  449,  450,  451, 2009,  513,
 1819, 2012,   41,  517,   41,   44,  509,   44, 2019,   41,
  299,  372,   44,   41,  508,  376,   44,  276,  277, 1624,
 1625, 1871,  759,  477,  478,  284, 1019,   59, 1021,   61,
 1023, 1024,  393, 1026, 1027, 1028,  284,  778,  399,  780,
 1890, 1891,  779,  784,  785,  786,  783,   41,   41, 1892,
   44,   44,    0,  508,   41, 1945,  793,   44, 1624, 1625,
   41,  798,  799,   44, 1954,  802,  803,  804,  805,  806,
  807,   41,  433,   41,   44,   41,   44,    0,   44,  816,
 1970,  818,  819,  820,  821,  822,  823,  306,  461,  826,
   44,  262,  446,   41,   61, 1945,   44, 1937,  366,  283,
   40, 1951,  264,   40, 1954,  389,  276,  277, 1998,   40,
   40,   59,   40,  372,  284, 1965,  453, 1967,   41, 2009,
 1970,   44, 2012,   40,   40,  862,   40,   40,   40, 2019,
   40,  453,  493,  453,  393,   40,   59,   40,   61,   40,
  399,    0,   40,   40,  315,  882,  260,  318, 1998, 1999,
  351,   40,   40,   40,  325,   40,  327,  894,  895, 2009,
  331, 2011, 2012,   40,   40,  336,   40,   91,  339, 2019,
  402,   41,   42,   43,  433,   45,   44,   47,    0,   46,
   61,  395,   41,  354,  298,   44,   40,  277,  264,  317,
  420,   44,  282,   44,   46,   61,  310,  368,  276,  277,
   59,   40,  372,  264,  337,  376,  284,   44,  379,  380,
   46,  297,  277,  344,  344,  344,  276,  282,  389,   41,
  334,  276,   44,  393,  365,  390,  258,  450,  264,  399,
  428,  390,  275,  344,  405,  439,  439,   59,  392,   40,
  345,  273,   41,   42,   43,   40,   45,   91,   47,   40,
   40,  315,  264,  376,  403,   44,  427,   42,   43,   44,
   45,  432,   47,  433,  382,  297,   41,   42,   43,   40,
   45,  303,   47,  444,  306,  446,  375, 1014, 1015, 1016,
   40,  315, 1019, 1020, 1021,  399, 1023, 1024, 1025, 1026,
 1027, 1028,  380,  443,  372,  409,   41,  275,   63,  296,
 1037, 1038,  428, 1040,  456, 1042,  428,  314, 1045, 1046,
 1047, 1048, 1049, 1050,  262,  393,  348,   40,   44,  444,
  434,  399,  493,  493,  440,  264, 1063,  337, 1065,   44,
  494,   40,  273,   40,  297,  258,  281,  340,  340,  297,
  276,  379,  513,    0, 1081,   40,  517,  410,  379,  379,
  408,  410,  275,  276,  277,  433,  352,  362,  281,   42,
   43,  284,   45,   40,   47,   40,   40,  315,   41,  277,
  318,  403,   44,  317,  297,   41,   44,  325,  410,  327,
  303,   46,  438,  331,   41, 1122,  270,   44,  336,  378,
  361,  339,    0,   40,   44,  317,  428,  317,  325,   40,
  493,    0,   59,  262,  266,   40,  354,  277,  264,   41,
  414,   41,  282,   58,   41,  493,   41,  264,  387,  446,
  368,   41,   42,   43,  306,   45,  458,   47,  376,   40,
  462,  379,  380,   41,  306,   41,   44,   41,   40,  317,
   40,  389,   41,   42,   43,   44,   45,    0,   47,  372,
   40,   59,   40,   61,  276,  277,  315,  405,   40,  318,
   59,   60,   61,   62,   40,   40,  325,   40,  327,   40,
  393,   40,  331,   40,   40,   40,  399,  336,  277,  427,
  339,   40,   40,  282,  432,  408,  262,  410,   41,  427,
  305,   44,  277,  446,   93,  354,  444,  282,  446,   44,
   44,   41,  277,   41,  339,  428,   59,  282,  264,  368,
  433,  339,   41,   42,   43,  273,   45,  376,   47,  327,
  379,  380,  284,  381,  384,   41,  399,   41,   42,   43,
  389,   45,  450,   47,  379, 1272,  379,  379,  379,  462,
   41,  379,  372,   40,  276,  493,  405,  379, 1285, 1286,
  372, 1288, 1289,   40,  317,   41,   41,   42,   43, 1296,
   45,   58,   47,   41,   41,  513,   41,  305,  427,  517,
   44,  393,   41,  432,   41,   42,   43,  399,   45,  262,
   47, 1318,  290,   59,   40,  444,  336,  446,  336, 1326,
   58, 1328, 1329,  427,  277, 1332,   59, 1334,  532,  282,
  305, 1338, 1339, 1340,  324,  428,  339,  277,  333,  434,
 1347,  433, 1349,  334,  434,  334,  298,  298,   41,  276,
  277,   40,  365,   41,  281,  378,  348,  284,  365,   41,
   42,   43,   41,   45,  493,   47,   41,   41,   42,   43,
  297,   45,   41,   47,   41,   41,   42,   43,   41,   45,
  258,   47,   41, 1390,  513,   41,   41,  277,  517,  258,
   41,   41,  282,  262,   41,  264,  265,  275,  276,  277,
   41,   41,  271,  281,   41,   41,  284,  276,  277,  291,
   41,   40,  315,  282,   41,  284,  285,   40,  443,  297,
  302,  378,   44, 1430,   41,  303,  348,  309,  340,  458,
  299,  313,  333,   40,  303,  304,  305,  336,  500,  308,
  336,   40,   40,  376,   44,  372,  315,   41,  317,  318,
   40,  333,  321,  276,  277,  427,  325,  376,  327,   41,
  329,  284,  331,  376,   41,  298,  393,  336,  337,  301,
  339,  434,  399,   41,  297,  301,    0,  304,  277,  290,
  333,  455,  376,  282,  376,  354,  355,   40,  357,  264,
  264, 1498,  450,  277,  372,  409,  276, 1504,  282,  368,
  317,  370, 1509,  372,  373,  374,  433,  376,   40, 1516,
  379,  380,  403,  395,  443,  393,   44,  270,   40,   40,
  389,  399,  277,   41,  393,  450,  339,  282,  270,  264,
  399,   41,  410,  402,  427,  339,  405,   41,   41,   59,
  277,  410,  450, 1550,  426,  282,   59, 1554,  430,  372,
  428,   59,  421, 1560,   59,  433,  305,   41,  427,  428,
   59,   59,   59,  432,  433,   41,   59,   59,    0, 1576,
  393,  301,    0,    0,  443,  444,  399,  446,    0, 1376,
  161,  581,  161,  452,  462,  583,  455,  852,  457,  458,
  459,  460,  162,  462,  321,  277,  320,  843, 1247, 1245,
  282,  470,  471,  277,   42,   43,   95,   45,  282,   47,
  433,  277,  481,  482,    0,  484,  282,  994,  743,  999,
  956,   81,  491,  492,  493,  494,  495, 1242,  497, 1237,
  969,  500,  501,  502,  503,  504, 1691,  519, 1692,  508,
  509,  410,  444, 1650,  513,  514,  515,  516,  517,  518,
  519,  520,  690,  522,   40,   41,   42,   43,   44,   45,
   46,   47, 1227,  532, 1170,   41,   42,   43,  370,   45,
 1238,   47,   58,   59,   60,   61,   62,  919, 1671,  884,
   41,   42,   43, 1814,   45,  452,   47, 1957,  455,  425,
  457,  458,  459,  460,  425,  462, 1673,  667, 1983, 1961,
 1960, 1111, 1579,  470,  471,   91, 1781,   93,  910, 1946,
 1408, 1655,  832,  430,  481,  482, 1723,  484,  939,  415,
  409,  669, 1729, 1730,  491,  492, 1462,  494,  495,  972,
  497,  408,  964,  500,  501,  502,  503,  504,   13,  727,
  163,  508,  509,  355,  599, 1130, 1753,  514,  515,  516,
 1757,  518,  519,  520, 1761,  522,  603,  873,  167,  862,
 1396, 1768, 1769,  452,   91,  532,  455, 1910,  457,  458,
  459,  460,  776,  462,   41,   42,   43, 1744,   45, 1123,
   47,  470,  471, 1529,  894,  493, 1793,  850, 1287,  614,
 1417, 1334,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,    0,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  262,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   41,   42,   43, 1844,   45,  277,
   47,   -1,   41,  532,  282,   44,   41,   42,   43,   -1,
   45,   -1,   47,   41,   42,   43,   -1,   45,   -1,   47,
   59,   41,   42,   43,   -1,   45,   -1,   47,  464,  465,
  466,  467,  468,  469,  470,  471,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,  277,   -1, 1910,   -1,    0,  282,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,  277,   -1,  304,  305,
   -1,  282,  308,   -1,   41,   42,   43,   -1,   45,  315,
   47,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
 1947,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   41,   42,   43,   -1,   45,  452,   47,   -1,  455,
  277,  457,  458,  459,  460,  282,  462,  276,  277,   -1,
   -1,   -1,  277,   -1,  470,  471,   -1,  282,   -1,  277,
   -1,   -1,   -1,   -1,  282,  481,  482,  277,  484,   -1,
   -1,   -1,  282,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,  532,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,  258,   -1,  282,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  372,   -1,   -1,  262,   -1,  264,  265,
   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,  393,   -1,  282,   -1,   -1,  285,
  399,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,  301,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   41,   42,   43,   -1,   45,  315,
   47,  317,  318,   -1,  433,  321,   41,   42,   43,  325,
   45,  327,   47,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,  277,   -1,   59,   -1,   -1,  282,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,  277,  393,   -1,
   -1,   -1,  282,  389,  399,   91,  291,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  402,  302,   -1,  405,
   -1,   -1,   -1,   -1,  309,   -1,   -1,   42,   43,   44,
   45,   -1,   47,  428,   -1,  421,   -1,  322,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,  455,
   -1,  457,  458,  459,  460,  277,  462,   -1,   -1,   -1,
  282,   -1,   -1,  277,  470,  471,   42,   43,  282,   45,
   -1,   47,   -1,   -1,  379,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
  395,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,  411,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,  426, 1499, 1500,   -1,  430,  532,   -1,  433,   -1,
 1507, 1508,   -1, 1510, 1511,   -1,  441,   -1,   -1,   -1,
  277, 1518, 1519, 1520, 1521,  282, 1523, 1524, 1525, 1526,
 1527, 1528,  277,   -1,   -1,  460,  262,  282,  264,  265,
   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,  519,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,  277,   -1,   59,   -1,   -1,  282,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   91,   -1,   93,  264,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,  302,   -1,  405,
   -1,  277,   -1,   -1,  309,   -1,  282,   -1,  313,  314,
   -1,   -1,   -1,   -1,   -1,  421,   -1,  322,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,  333,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
  395,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,  411,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,  426,   -1,   -1,   -1,  430,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,  262,   -1,  264,  265,
   -1,  276,  277,   -1,   -1,  271,   -1,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,  519,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   45,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,  292,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,  320,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,  348,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,  419,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,   -1,  453,
  454,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,   -1,   -1,  285,
   -1,   -1,   -1,  487,  488,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  496,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
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
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
  398,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,  437,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,  489,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   41,  331,   43,   44,   45,   -1,  336,
  337,   -1,  339,  272,   -1,   -1,  275,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,  288,
  357,   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,  335,   -1,  405,   -1,
   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,  367,   -1,
  369,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,  398,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  415,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,  437,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
  489,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,    0,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   59,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,  272,   -1,  325,  275,  327,   -1,
  329,   41,  331,   43,   44,   45,   -1,  336,  337,   -1,
  339,   -1,  290,   -1,  292,   -1,   -1,  295,   -1,   59,
   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   93,   -1,   -1,   -1,  335,   -1,   -1,
  389,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,
   -1,  369,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   40,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
  398,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,  415,   -1,   -1,
   -1,  470,  471,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,  437,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,    0,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,  472,  473,  315,   -1,   -1,  318,
   -1,   -1,   -1,  532,   -1,   -1,  325,   -1,  327,   -1,
   41,  489,  331,   44,   -1,   -1,   -1,  336,   -1,   -1,
  339,   -1,  262,   -1,  264,  265,   -1,   -1,   59,   -1,
   -1,  271,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  299,
  379,  380,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
  389,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,  405,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   42,   -1,  281,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
   -1,  421,   -1,   -1,    0,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,  513,   -1,   -1,   -1,  517,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   40,   41,   -1,   -1,   44,   -1,
  470,  471,   -1,  264,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   59,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  522,   -1,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,  532,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,  354,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,  481,  482,  376,  484,   -1,   40,  380,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,  389,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,  405,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  293,    0,  532,   -1,   -1,   -1,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   41,   -1,   -1,   44,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   59,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  532,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,  368,   -1,  455,   -1,  457,  458,  459,  460,
  376,  462,   -1,   -1,  380,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,  405,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,  432,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,  532,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,    0,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,  532,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   -1,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
  452,  368,   -1,  455,   -1,  457,  458,  459,  460,  376,
  462,   -1,   -1,  380,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,  389,   -1,   -1,   91,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,  405,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,  432,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
  532,   -1,  275,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,  291,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,  302,
   -1,   -1,   -1,   -1,  481,  482,  309,  484,   -1,   -1,
  313,  314,   -1,   -1,  491,  492,  493,  494,  495,  322,
  497,    0,   -1,  500,  501,  502,  503,  504,   -1,   -1,
  333,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,   42,   43,   -1,   45,
   -1,   47,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,
   59,   60,   61,   62,  387,   -1,  262,   -1,   -1,   -1,
   -1,   -1,  395,    0,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  426,   -1,   -1,   -1,  430,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  441,  315,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   59,  329,   -1,  331,   -1,  460,  461,   -1,
  336,  337,   -1,  339,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,  389,   -1,   -1,  519,   -1,   -1,  522,
   -1,   -1,   -1,   -1,   59,    0,   -1,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,  444,   44,
  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   59,  271,   -1,   -1,   -1,   -1,
   -1,  277,  271,   -1,   -1,   -1,  282,   -1,  277,  285,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  321,  315,  513,   -1,  318,
   -1,  517,  321,  329,   -1,   -1,  325,   -1,  327,   -1,
  329,  337,  331,   -1,   -1,  262,   -1,  336,  337,   -1,
  339,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  355,
   -1,  357,   -1,   -1,   -1,  354,  355,   -1,  357,   59,
   -1,   -1,   -1,   -1,  370,   -1,  372,  373,  374,  368,
   -1,  370,    0,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,  262,   -1,  336,
   -1,   -1,  339,   41,   -1,  421,   44,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,  354,  427,   -1,
   -1,   59,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,    0,  444,   -1,  446,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  389,  318,   -1,   -1,   -1,  262,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  405,   -1,
   -1,  336,   -1,   -1,  339,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,  354,
  427,   -1,   -1,   59,    0,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,  513,   -1,   -1,  444,  517,  446,
  315,  376,   -1,  318,  379,  380,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,  389,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   41,   -1,   -1,   44,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,  262,   59,   -1,   -1,  493,   -1,   -1,   -1,
   -1,   -1,  427,  368,   -1,   -1,    0,  432,   -1,   -1,
   -1,  376,   -1,   -1,  379,  380,  513,   -1,   -1,  444,
  517,  446,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  405,   -1,   -1,   -1,   -1,  315,   -1,   41,  318,   -1,
   44,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,  427,   -1,  262,   59,  336,  432,  493,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,   -1,   -1,  354,   -1,   -1,   -1,  513,   -1,
    0,   -1,  517,   -1,   -1,   -1,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,  493,  327,
   -1,   41,   -1,  331,   44,  405,  262,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,  513,   59,
   -1,   -1,  517,   -1,   -1,   -1,  354,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,    0,  444,   -1,  446,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  389,  318,   -1,   -1,   -1,  262,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,
  336,   -1,   -1,  339,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,  354,  427,
   -1,   -1,   59,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,  513,   -1,    0,  444,  517,  446,  315,
  376,   -1,  318,  379,  380,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,  389,   -1,  331,   -1,   -1,  262,   -1,
  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,  354,   44,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,  427,  368,   -1,   59,    0,  432,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,  513,   -1,   -1,  444,  517,
  446,  315,   -1,  389,  318,   -1,   -1,   -1,   -1,   -1,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  405,
   -1,   -1,  336,   -1,   -1,  339,   41,   -1,   -1,   44,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  427,   -1,   -1,   59,   -1,  432,  493,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,  444,   -1,
  446,   -1,  376,   -1,   -1,  379,  380,  513,   -1,   -1,
   -1,  517,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,  405,   -1,   -1,   -1,  325,    0,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,  493,   -1,  339,
   -1,   -1,   -1,  427,   -1,  262,   -1,   -1,  432,   -1,
   -1,   -1,    0,   -1,  354,   -1,   -1,  513,   -1,    0,
  444,  517,  446,   -1,   -1,   -1,   -1,   41,  368,   -1,
   44,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,  389,
   -1,   -1,   40,   41,   -1,   -1,   44,   -1,  315,   -1,
   41,  318,   -1,   44,   -1,  405,   -1,   -1,  325,  493,
  327,   59,   -1,   61,  331,   -1,   -1,  262,   59,  336,
   -1,   -1,  339,   -1,   -1,   -1,   -1,  427,   -1,  513,
   -1,   -1,  432,  517,   -1,   -1,   -1,  354,   -1,   -1,
   -1,   -1,   -1,   91,  444,   -1,  446,   -1,    0,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  389,  318,   -1,   -1,   -1,  262,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  405,   41,
   -1,  336,   44,  493,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,  354,
  427,   -1,   -1,  513,   -1,  432,   -1,  517,   -1,   -1,
    0,   -1,   -1,  368,   -1,   -1,   -1,  444,   -1,  446,
  315,  376,   -1,  318,  379,  380,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,  389,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
  405,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   59,
   -1,   -1,  427,  368,   -1,   -1,   -1,  432,  262,   -1,
   -1,  376,   -1,   -1,  379,  380,  513,   -1,   -1,  444,
  517,  446,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
  405,  262,   -1,   -1,   -1,  273,   -1,  275,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,
   -1,  315,  427,   -1,  318,   -1,   -1,  432,  493,  297,
   -1,  325,    0,  327,   -1,  303,   -1,  331,  306,  444,
   41,  446,  336,   44,   -1,  339,   -1,   -1,  513,   -1,
   -1,   -1,  517,   -1,  315,   -1,   -1,  318,   59,   -1,
  354,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,   41,  368,  336,   44,   -1,  339,   -1,
  348,   -1,  376,   -1,   -1,  379,  380,   -1,  493,   -1,
  262,   59,   -1,  354,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,  368,  513,   -1,
   -1,  405,  517,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,  389,    0,
   -1,  399,   -1,  427,   -1,  403,   -1,   -1,  432,   -1,
   -1,   -1,  410,  315,  405,   -1,  318,   -1,   -1,   -1,
  444,   -1,  446,  325,   -1,  327,   -1,   -1,   -1,  331,
  428,   -1,  262,   -1,  336,  433,  427,  339,   -1,   -1,
   41,  432,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  444,   -1,  446,   -1,   -1,   59,   -1,
  458,   -1,   -1,   -1,  462,   -1,  368,   -1,   -1,  493,
   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,  513,
   -1,   -1,   -1,  517,   -1,  325,    0,  327,   -1,   -1,
   -1,  331,  493,  405,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  513,   -1,  354,  427,  517,   -1,   -1,    0,
  432,  262,   -1,   -1,   -1,   -1,   40,   41,  368,   -1,
   44,   -1,  444,   -1,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,   59,   -1,
   -1,  493,   -1,   -1,  325,   -1,  327,  427,   -1,   -1,
  331,   -1,  432,   -1,   -1,  336,   -1,   -1,  339,   -1,
   -1,  513,   -1,   -1,  444,  517,  446,  315,    0,   -1,
  318,   -1,   -1,  354,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,  368,  336,   -1,
   -1,  339,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,  389,   41,
   -1,   -1,   44,  493,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,   -1,  405,   -1,   -1,   59,  376,   -1,
   -1,  379,  380,  513,   -1,   -1,  272,  517,   -1,  275,
   -1,  389,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  432,  288,   -1,   -1,   -1,  292,  405,   -1,  295,
   -1,   -1,   -1,  444,  315,  446,   -1,  318,   -1,   -1,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,  427,
  331,   -1,   -1,   -1,  432,  336,   -1,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   40,  446,  335,
   43,   -1,   45,  354,   -1,  341,  342,  343,   -1,  345,
  346,  347,  493,  349,   -1,   58,   -1,  368,   -1,   -1,
   63,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,  367,  513,  369,   -1,   -1,  517,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,  315,  398,   -1,  318,  513,   -1,  403,   -1,  517,
   -1,  325,   -1,  327,   -1,   -1,  427,  331,   -1,  415,
   -1,  432,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,  315,  446,   -1,  318,   -1,   -1,
  354,  437,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,  368,  336,   -1,   -1,  339,   -1,
   -1,   -1,  376,   -1,   -1,   -1,  380,   -1,   -1,   42,
   -1,   -1,   -1,  354,   -1,  389,  472,  473,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,  368,   -1,   -1,
   -1,  405,   -1,  489,   -1,  376,   -1,   -1,   -1,  380,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,  315,  405,   -1,  318,   -1,   -1,   -1,
  444,   -1,  446,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  444,  267,  446,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,  368,   -1,   -1,  493,
   -1,   -1,   -1,  286,  376,   -1,   -1,   -1,  380,  292,
  293,   -1,   -1,   40,   -1,   -1,   43,  389,   45,  513,
   -1,   -1,   -1,  517,   -1,   -1,   -1,   -1,  311,   -1,
   -1,   58,  493,  405,   -1,   -1,   63,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  444,   -1,  446,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,  513,  425,   -1,   -1,  517,   -1,   44,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  505,  506,  507,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
  267,  524,  525,  526,   -1,  528,  529,  274,  531,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,  289,   -1,   -1,  292,  293,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  311,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,
   -1,  494,  495,   -1,  497,  372,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   45,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  317,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,  267,  524,  525,  526,
   -1,  528,  529,  274,  531,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  311,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,
   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,
   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,
  497,  372,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,  267,  524,  525,  526,   -1,  528,  529,  274,
  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,  450,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,
  491,  492,   -1,  494,  495,   -1,  497,  372,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,  267,  524,
  525,  526,   -1,  528,  529,  274,  531,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   58,   -1,   -1,  452,   -1,
   63,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,  360,   -1,   -1,   -1,  364,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,  284,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  521,  522,   -1,  524,  525,  526,  527,  528,
  529,  530,  531,  532,  267,   -1,   41,   42,   43,   -1,
   45,  274,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   60,   61,   62,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,  311,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   58,  452,   -1,   -1,  455,   63,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,  372,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,  271,  500,  501,  502,
  503,  504,  277,   -1,   -1,  508,  509,  282,   -1,   -1,
  285,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,  524,  525,  526,  260,  528,  529,  263,  531,  532,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  321,   -1,   -1,   -1,
  286,   -1,   -1,   -1,  329,   -1,  292,  293,   -1,   -1,
   -1,   -1,  337,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
  355,   -1,  357,  435,  320,   -1,   -1,  323,   -1,   58,
   -1,   -1,   -1,   -1,   63,  370,   -1,  372,  373,  374,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,  348,   -1,  350,  351,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,  421,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  532,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,  524,  525,
  526,  260,  528,  529,  263,  531,  532,   -1,  267,  272,
   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,  292,
   -1,   -1,  295,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   58,   -1,   -1,   -1,
   -1,   63,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,  367,  364,  369,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,  472,
  473,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,  489,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,  524,  525,  526,  260,  528,
  529,  263,  531,  532,   -1,  267,  272,   -1,   -1,  275,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,  292,   -1,   -1,  295,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   58,   -1,   -1,   -1,   -1,   63,  335,
   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,  367,  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,  325,   -1,  472,  473,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,  524,  525,  526,  260,  528,  529,  263,  531,
  532,   -1,  267,   -1,  395,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  430,
   -1,   -1,   40,   -1,  435,   43,   -1,   45,   -1,   -1,
  441,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   58,  452,   -1,   -1,  455,   63,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,  524,
  525,  526,  260,  528,  529,  263,  531,  532,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   58,  452,   -1,
   -1,  455,   63,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,  524,  525,  526,  260,
  528,  529,  263,  531,  532,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   58,  452,   -1,   -1,  455,   63,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,
   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,  524,  525,  526,  260,  528,  529,  263,
  531,  532,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   58,  452,   -1,   -1,  455,   63,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,
  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
  524,  525,  526,  260,  528,  529,  263,  531,  532,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   58,   -1,  323,   -1,   -1,   63,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,  360,   -1,   -1,   -1,  364,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,  260,  524,  525,  526,
   -1,  528,  529,  267,  531,  532,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   58,   -1,  323,
   -1,   -1,   63,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,  360,   -1,   -1,   -1,
  364,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,  260,
  524,  525,  526,   -1,  528,  529,  267,  531,  532,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   40,   -1,   42,   43,  300,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,  267,  524,  525,  526,   -1,  528,  529,  274,
  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,  267,  524,
  525,  526,   -1,  528,  529,  274,  531,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,  267,  524,  525,  526,   -1,  528,
  529,  274,  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   40,   41,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
  267,  524,  525,  526,   -1,  528,  529,  274,  531,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,   -1,
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
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,  267,  524,  525,  526,
   -1,  528,  529,  274,  531,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,
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
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,  267,  524,  525,  526,   -1,  528,  529,  274,
  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,  317,   -1,   63,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,  267,  524,
  525,  526,   -1,  528,  529,  274,  531,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,  267,  524,  525,  526,   -1,  528,
  529,  274,  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,
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
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
  267,  524,  525,  526,   -1,  528,  529,  274,  531,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,   -1,
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
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,  267,  524,  525,  526,
   -1,  528,  529,  274,  531,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
    0,  522,  267,  524,  525,  526,   -1,  528,  529,  274,
  531,  532,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   41,   -1,  320,   44,   -1,  323,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   41,   91,   -1,   44,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,    0,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,  258,  524,
  525,  526,   41,  528,  529,   44,  531,  532,   -1,   -1,
  258,   -1,   -1,  273,   -1,  275,  276,  277,   -1,    0,
   59,  281,   61,   -1,  284,  273,   -1,  275,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,  297,   -1,   -1,
   -1,   -1,   -1,  303,   -1,   -1,  306,  258,   -1,  297,
   -1,   -1,   91,   -1,   -1,  303,   -1,   -1,  306,   -1,
   41,   -1,  273,   44,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,  297,   -1,  348,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  393,   -1,   -1,   -1,  348,   -1,  399,
   -1,   -1,   -1,  403,   -1,  393,   -1,   -1,   -1,   -1,
  410,  399,   -1,   -1,   -1,  403,   -1,   -1,  261,   -1,
   -1,  372,  410,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,  433,   -1,   -1,  279,   -1,   -1,   -1,
  428,   -1,  393,   -1,  287,  433,   -1,   -1,  399,   -1,
   -1,  294,  403,  296,   -1,  298,   -1,   -1,  458,  410,
  303,   -1,  462,   -1,   -1,   -1,   -1,  310,   -1,   -1,
  458,   -1,   -1,   -1,  462,   -1,   -1,  428,   -1,  258,
   -1,  324,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  334,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,   -1,  458,   -1,   -1,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,
   -1,  260,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,  397,  276,  277,   -1,   -1,  298,
  281,  404,   -1,  284,  407,   -1,  409,  410,   -1,  348,
   -1,  310,   -1,   -1,   -1,   -1,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
   -1,  434,   -1,  372,   -1,  334,   -1,  260,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,   -1,
  399,   -1,   -1,   -1,  403,   -1,   -1,  348,   -1,   -1,
   -1,  410,   -1,   -1,   -1,  298,   -1,   -1,   -1,   -1,
  483,   -1,   -1,   -1,   -1,   -1,   -1,  310,   -1,  428,
   -1,  372,   -1,   -1,  433,  498,  499,   -1,   -1,   -1,
  399,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,  334,  393,   -1,   -1,   -1,   -1,   -1,  399,  458,
  523,   -1,  403,  462,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,  433,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,  399,  458,   -1,   -1,
   -1,  462,  481,  482,   -1,  484,  409,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  434,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,  270,   -1,   -1,   -1,  452,
   -1,   -1,  455,  532,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,  298,   -1,   -1,   -1,   -1,  481,  482,
  305,  484,   -1,   -1,  309,  310,   -1,  312,  491,  492,
  315,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  328,   -1,  508,  509,   -1,   -1,  334,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  270,   -1,   -1,   -1,   -1,  391,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,
  309,  310,   -1,  312,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  328,
  445,   -1,   -1,   -1,   -1,  334,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,  391,  508,  509,  510,  511,  512,   -1,  514,
  515,  516,   -1,  518,  519,  520,  270,  522,   -1,   -1,
  409,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  298,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  309,  310,   -1,  312,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,  328,   -1,   -1,   -1,   -1,   -1,
  334,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,  391,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  424,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,  272,   -1,   -1,  275,   -1,  470,  471,   -1,   -1,
  281,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,  292,   -1,   -1,  295,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,  532,   -1,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,  369,  272,
   -1,   -1,  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  292,
   -1,   -1,  295,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,  437,   -1,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,  367,   -1,  369,   -1,   -1,  470,
  471,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,  489,   -1,
  491,  492,   -1,  494,  495,  398,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  339,   -1,  508,  509,   -1,
   -1,   -1,  415,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,  362,   -1,
   -1,  532,   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,  418,   -1,  489,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  452,  522,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,  532,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  532,  455,
  456,  457,  458,  459,  460,  461,  462,  463,  464,  465,
  466,  467,  468,  469,  470,  471,  472,  473,  474,  475,
  476,   -1,   -1,  479,  480,  481,  482,  483,  484,   -1,
   -1,  487,  488,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  498,  499,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,  510,  511,  512,  513,  514,  515,
  516,  517,  518,  519,  520,  521,  522,  523,  524,  525,
  526,  527,  528,  529,  530,  531,  532,  452,   -1,   -1,
  455,  456,  457,  458,  459,  460,   -1,  462,  463,  464,
  465,  466,  467,  468,  469,  470,  471,  472,  473,  474,
  475,  476,   -1,   -1,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,  491,  492,  493,  494,
  495,  496,  497,  498,  499,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,  510,  511,  512,  513,  514,
  515,  516,  517,  518,  519,  520,  521,  522,  523,  524,
  525,  526,  527,  528,  529,  530,  531,  532,
};
#define YYFINAL 21
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 532
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
"IIF","SCALAR_ARRAY","CROSS","NEXT","SEQUENCE","RESTART","BOTH","COLLATION",
"COMMENT","BIT_LENGTH","CHAR_LENGTH","CHARACTER_LENGTH","LEADING","KW_LOWER",
"OCTET_LENGTH","TRAILING","TRIM","RETURNING",
};
char *yyrule[] = {
"$accept : top",
"top : statement",
"top : statement ';'",
"statement : alter",
"statement : blob_io",
"statement : comment",
"statement : commit",
"statement : create",
"statement : declare",
"statement : delete",
"statement : drop",
"statement : grant",
"statement : insert",
"statement : exec_procedure",
"statement : exec_block",
"statement : recreate",
"statement : replace",
"statement : revoke",
"statement : rollback",
"statement : savepoint",
"statement : select",
"statement : set",
"statement : update",
"statement : KW_DEBUG signed_short_integer",
"grant : GRANT privileges ON table_noise simple_table_name TO non_role_grantee_list grant_option",
"grant : GRANT proc_privileges ON PROCEDURE simple_proc_name TO non_role_grantee_list grant_option",
"grant : GRANT role_name_list TO role_grantee_list role_admin_option",
"table_noise : TABLE",
"table_noise :",
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
"revoke : REVOKE rev_grant_option privileges ON table_noise simple_table_name FROM non_role_grantee_list",
"revoke : REVOKE rev_grant_option proc_privileges ON PROCEDURE simple_proc_name FROM non_role_grantee_list",
"revoke : REVOKE rev_admin_option role_name_list FROM role_grantee_list",
"rev_grant_option : GRANT OPTION FOR",
"rev_grant_option :",
"rev_admin_option : ADMIN OPTION FOR",
"rev_admin_option :",
"non_role_grantee_list : grantee_list",
"non_role_grantee_list : user_grantee_list",
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
"create_clause : EXCEPTION exception_clause",
"create_clause : unique_opt order_direction INDEX symbol_index_name ON simple_table_name index_definition",
"create_clause : PROCEDURE procedure_clause",
"create_clause : TABLE table_clause",
"create_clause : TRIGGER def_trigger_clause",
"create_clause : VIEW view_clause",
"create_clause : GENERATOR generator_clause",
"create_clause : SEQUENCE generator_clause",
"create_clause : DATABASE db_clause",
"create_clause : DOMAIN domain_clause",
"create_clause : SHADOW shadow_clause",
"create_clause : ROLE role_clause",
"recreate : RECREATE recreate_clause",
"recreate_clause : PROCEDURE rprocedure_clause",
"recreate_clause : TABLE rtable_clause",
"recreate_clause : VIEW rview_clause",
"recreate_clause : EXCEPTION rexception_clause",
"replace : CREATE OR ALTER replace_clause",
"replace_clause : PROCEDURE replace_procedure_clause",
"replace_clause : TRIGGER replace_trigger_clause",
"replace_clause : EXCEPTION replace_exception_clause",
"exception_clause : symbol_exception_name sql_string",
"rexception_clause : symbol_exception_name sql_string",
"replace_exception_clause : symbol_exception_name sql_string",
"alter_exception_clause : symbol_exception_name sql_string",
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
"column_def : column_def_name data_type_or_domain domain_default_opt end_trigger column_constraint_clause collate_clause",
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
"proc_statement : simple_proc_statement ';'",
"proc_statement : complex_proc_statement",
"simple_proc_statement : assignment",
"simple_proc_statement : insert",
"simple_proc_statement : update",
"simple_proc_statement : delete",
"simple_proc_statement : singleton_select",
"simple_proc_statement : exec_procedure",
"simple_proc_statement : exec_sql",
"simple_proc_statement : exec_into",
"simple_proc_statement : exec_udf",
"simple_proc_statement : excp_statement",
"simple_proc_statement : raise_statement",
"simple_proc_statement : post_event",
"simple_proc_statement : cursor_statement",
"simple_proc_statement : breakleave",
"simple_proc_statement : SUSPEND",
"simple_proc_statement : EXIT",
"complex_proc_statement : if_then_else",
"complex_proc_statement : while",
"complex_proc_statement : for_select",
"complex_proc_statement : for_exec_into",
"excp_statement : EXCEPTION symbol_exception_name",
"excp_statement : EXCEPTION symbol_exception_name value",
"raise_statement : EXCEPTION",
"exec_sql : EXECUTE STATEMENT value",
"for_select : label_opt FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : label_opt FOR EXECUTE STATEMENT value INTO variable_list DO proc_block",
"exec_into : EXECUTE STATEMENT value INTO variable_list",
"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
"if_then_else : IF '(' search_condition ')' THEN proc_block",
"post_event : POST_EVENT value event_argument_opt",
"event_argument_opt :",
"singleton_select : select INTO variable_list",
"variable : ':' symbol_variable_name",
"variable_list : variable",
"variable_list : column_name",
"variable_list : variable_list ',' column_name",
"variable_list : variable_list ',' variable",
"while : label_opt WHILE '(' search_condition ')' DO proc_block",
"label_opt : symbol_label_name ':'",
"label_opt :",
"breakleave : KW_BREAK",
"breakleave : LEAVE",
"breakleave : LEAVE symbol_label_name",
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
"exec_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs_opt",
"proc_inputs : value_list",
"proc_inputs : '(' value_list ')'",
"proc_inputs :",
"proc_outputs_opt : RETURNING_VALUES variable_list",
"proc_outputs_opt : RETURNING_VALUES '(' variable_list ')'",
"proc_outputs_opt :",
"exec_block : EXECUTE BLOCK block_input_params output_parameters AS local_declaration_list full_proc_block",
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
"alter_clause : EXCEPTION alter_exception_clause",
"alter_clause : TABLE simple_table_name alter_ops",
"alter_clause : TRIGGER alter_trigger_clause",
"alter_clause : PROCEDURE alter_procedure_clause",
"alter_clause : DATABASE init_alter_db alter_db",
"alter_clause : DOMAIN alter_column_name alter_domain_ops",
"alter_clause : INDEX alter_index_clause",
"alter_clause : SEQUENCE alter_sequence_clause",
"alter_clause : EXTERNAL FUNCTION alter_udf_clause",
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
"alter_op : col_opt alter_col_name TYPE alter_data_type_or_domain",
"alter_op : col_opt alter_col_name SET domain_default_opt2 end_trigger",
"alter_op : col_opt alter_col_name DROP DEFAULT",
"alter_column_name : keyword_or_column",
"keyword_or_column : valid_symbol_name",
"keyword_or_column : ADMIN",
"keyword_or_column : COLUMN",
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
"keyword_or_column : RECREATE",
"keyword_or_column : CURRENT_CONNECTION",
"keyword_or_column : CURRENT_TRANSACTION",
"keyword_or_column : BIGINT",
"keyword_or_column : CASE",
"keyword_or_column : RELEASE",
"keyword_or_column : ROW_COUNT",
"keyword_or_column : SAVEPOINT",
"keyword_or_column : OPEN",
"keyword_or_column : CLOSE",
"keyword_or_column : FETCH",
"keyword_or_column : ROWS",
"keyword_or_column : USING",
"keyword_or_column : CROSS",
"keyword_or_column : BIT_LENGTH",
"keyword_or_column : BOTH",
"keyword_or_column : CHAR_LENGTH",
"keyword_or_column : CHARACTER_LENGTH",
"keyword_or_column : COMMENT",
"keyword_or_column : LEADING",
"keyword_or_column : KW_LOWER",
"keyword_or_column : OCTET_LENGTH",
"keyword_or_column : TRAILING",
"keyword_or_column : TRIM",
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
"alter_sequence_clause : symbol_generator_name RESTART WITH signed_long_integer",
"alter_sequence_clause : symbol_generator_name RESTART WITH NUMBER64BIT",
"alter_sequence_clause : symbol_generator_name RESTART WITH '-' NUMBER64BIT",
"alter_udf_clause : symbol_UDF_name entry_op module_op",
"entry_op : ENTRY_POINT sql_string",
"entry_op :",
"module_op : MODULE_NAME sql_string",
"module_op :",
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
"drop_clause : SEQUENCE symbol_generator_name",
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
"rollback : ROLLBACK optional_work optional_retain",
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
"comment : COMMENT ON ddl_type0 IS ddl_desc",
"comment : COMMENT ON ddl_type1 symbol_ddl_name IS ddl_desc",
"comment : COMMENT ON ddl_type2 symbol_ddl_name ddl_subname IS ddl_desc",
"ddl_type0 : DATABASE",
"ddl_type1 : DOMAIN",
"ddl_type1 : TABLE",
"ddl_type1 : VIEW",
"ddl_type1 : PROCEDURE",
"ddl_type1 : TRIGGER",
"ddl_type1 : EXTERNAL FUNCTION",
"ddl_type1 : FILTER",
"ddl_type1 : EXCEPTION",
"ddl_type1 : GENERATOR",
"ddl_type1 : SEQUENCE",
"ddl_type1 : INDEX",
"ddl_type1 : ROLE",
"ddl_type1 : CHARACTER SET",
"ddl_type1 : COLLATION",
"ddl_type2 : COLUMN",
"ddl_type2 : PARAMETER",
"ddl_subname : '.' symbol_ddl_name",
"ddl_desc : sql_string",
"ddl_desc : KW_NULL",
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
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' value_list ')' returning_clause",
"insert : INSERT INTO simple_table_name ins_column_parens_opt select_expr",
"delete : delete_searched",
"delete : delete_positioned",
"delete_searched : KW_DELETE FROM table_name where_clause plan_clause order_clause rows_clause",
"delete_positioned : KW_DELETE FROM table_name cursor_clause",
"update : update_searched",
"update : update_positioned",
"update_searched : UPDATE table_name SET assignments where_clause plan_clause order_clause rows_clause",
"update_positioned : UPDATE table_name SET assignments cursor_clause",
"returning_clause : RETURNING value_list",
"returning_clause : RETURNING value_list INTO variable_list",
"returning_clause :",
"cursor_clause : WHERE CURRENT OF symbol_cursor_name",
"assignments : assignment",
"assignments : assignments ',' assignment",
"assignment : update_column_name '=' value",
"exec_udf : udf",
"blob_io : READ BLOB simple_column_name FROM simple_table_name filter_clause_io segment_clause_io",
"blob_io : INSERT BLOB simple_column_name INTO simple_table_name filter_clause_io segment_clause_io",
"filter_clause_io : FILTER FROM blob_subtype_value_io TO blob_subtype_value_io",
"filter_clause_io : FILTER TO blob_subtype_value_io",
"filter_clause_io :",
"blob_subtype_value_io : blob_subtype_io",
"blob_subtype_value_io : parameter",
"blob_subtype_io : signed_short_integer",
"segment_clause_io : MAX_SEGMENT segment_length_io",
"segment_clause_io :",
"segment_length_io : unsigned_short_integer",
"segment_length_io : parameter",
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
"value : next_value_expression",
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
"datetime_value_expression : CURRENT_TIME sec_precision_opt",
"datetime_value_expression : CURRENT_TIMESTAMP sec_precision_opt",
"sec_precision_opt : '(' nonneg_short_integer ')'",
"sec_precision_opt :",
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
"numeric_value_function : extract_expression",
"numeric_value_function : length_expression",
"extract_expression : EXTRACT '(' timestamp_part FROM value ')'",
"length_expression : bit_length_expression",
"length_expression : char_length_expression",
"length_expression : octet_length_expression",
"bit_length_expression : BIT_LENGTH '(' value ')'",
"char_length_expression : CHAR_LENGTH '(' value ')'",
"char_length_expression : CHARACTER_LENGTH '(' value ')'",
"octet_length_expression : OCTET_LENGTH '(' value ')'",
"string_value_function : substring_function",
"string_value_function : trim_function",
"string_value_function : KW_UPPER '(' value ')'",
"string_value_function : KW_LOWER '(' value ')'",
"substring_function : SUBSTRING '(' value FROM value string_length_opt ')'",
"string_length_opt : FOR value",
"string_length_opt :",
"trim_function : TRIM '(' trim_specification value FROM value ')'",
"trim_function : TRIM '(' value FROM value ')'",
"trim_function : TRIM '(' trim_specification FROM value ')'",
"trim_function : TRIM '(' value ')'",
"trim_specification : BOTH",
"trim_specification : TRAILING",
"trim_specification : LEADING",
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
"next_value_expression : NEXT KW_VALUE FOR symbol_generator_name",
"next_value_expression : GEN_ID '(' symbol_generator_name ',' value ')'",
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
"symbol_ddl_name : valid_symbol_name",
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
"non_reserved_word : ACTION",
"non_reserved_word : CASCADE",
"non_reserved_word : FREE_IT",
"non_reserved_word : RESTRICT",
"non_reserved_word : ROLE",
"non_reserved_word : TYPE",
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
"non_reserved_word : SEQUENCE",
"non_reserved_word : NEXT",
"non_reserved_word : RESTART",
"non_reserved_word : COLLATION",
"non_reserved_word : RETURNING",
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

		const dsql_nod* old = node;
		node = FB_NEW_RPT(*tdsql->getDefaultPool(), l) dsql_nod;
		node->nod_count = l;
		node->nod_type = nod_list;
		node->nod_line = (USHORT) lex.lines_bk;
		node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
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
				const LexerState savedState = lex;
				const int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
					const LexerState savedState = lex;
					const int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
			else if (sym->sym_keyword != COMMENT || prev_keyword == -1)
			{
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
					const int token = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
			isc_arg_gds, isc_command_end_err2,	/* Unexpected end of command */
			isc_arg_number, (SLONG) lex.lines,
			isc_arg_number, (SLONG) (lex.last_token - lex.line_start + 1),
			0);
	else
	{
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			/* Token unknown - line %d, column %d */
			isc_arg_gds, isc_dsql_token_unk_err,
			isc_arg_number, (SLONG) lex.lines,
			isc_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Show the token */
			isc_arg_gds, isc_random,
			isc_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token,
			0);
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
case 23:
{ prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 26:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
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
{ yyval = NULL; }
break;
case 42:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 43:
{ yyval = NULL; }
break;
case 44:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
break;
case 45:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 46:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 47:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					make_list(yyvsp[-2]), make_list(yyvsp[0]), NULL, yyvsp[-3]); }
break;
case 48:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 49:
{ yyval = NULL; }
break;
case 50:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 51:
{ yyval = NULL; }
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
{ yyval = yyvsp[0]; }
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
{ yyval = yyvsp[0]; }
break;
case 123:
{ yyval = yyvsp[0]; }
break;
case 124:
{ yyval = yyvsp[0]; }
break;
case 125:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 126:
{ yyval = make_node (nod_redef_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 127:
{ yyval = make_node (nod_replace_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 128:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 129:
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 130:
{ yyval = NULL; }
break;
case 131:
{ yyval = make_list (yyvsp[0]); }
break;
case 133:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 134:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 135:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 136:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 137:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 138:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 139:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 140:
{ yyval = (dsql_nod*) 0;}
break;
case 141:
{ yyval = yyvsp[-1]; }
break;
case 142:
{ yyval = NULL; }
break;
case 145:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 146:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 147:
{ yyval = NULL; }
break;
case 148:
{ yyval = NULL; }
break;
case 149:
{ yyval = yyvsp[0]; }
break;
case 150:
{ yyval = NULL; }
break;
case 151:
{ yyval = NULL; }
break;
case 154:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 155:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 158:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 159:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 160:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
break;
case 161:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
break;
case 162:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 165:
{ yyval = (dsql_nod*) yyvsp[0]; }
break;
case 166:
{yyval = NULL;}
break;
case 169:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 170:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 171:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 172:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 173:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 174:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 175:
{yyval = NULL;}
break;
case 178:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 180:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 181:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 182:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 183:
{ lex.g_file  = make_file();}
break;
case 188:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 189:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 196:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 197:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 198:
{ yyval = yyvsp[0]; }
break;
case 199:
{ yyval = yyvsp[0]; }
break;
case 200:
{ yyval = NULL; }
break;
case 202:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 205:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 206:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 207:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 208:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 211:
{ yyval = NULL; }
break;
case 212:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 213:
{ yyval = yyvsp[0]; }
break;
case 214:
{ yyval = NULL; }
break;
case 215:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 216:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 217:
{ yyval = yyvsp[-1]; }
break;
case 218:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 219:
{ yyval = yyvsp[0]; }
break;
case 220:
{ yyval = NULL; }
break;
case 227:
{ yyval = NULL; }
break;
case 230:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 231:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 232:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 233:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 235:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 236:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 237:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 238:
{ yyval = yyvsp[0]; }
break;
case 239:
{ yyval = NULL ;}
break;
case 244:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 245:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 246:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 247:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 248:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 249:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 250:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 251:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 252:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 253:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 254:
{ yyval = NULL;}
break;
case 255:
{ yyval = yyvsp[0];}
break;
case 256:
{ yyval = yyvsp[0];}
break;
case 257:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 258:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 259:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 260:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 261:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 262:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 263:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 264:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 265:
{ yyval = make_list (yyvsp[-1]); }
break;
case 266:
{ yyval = NULL; }
break;
case 267:
{ yyval = make_list (yyvsp[-1]); }
break;
case 268:
{ yyval = NULL; }
break;
case 270:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 271:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 273:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 274:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 275:
{ yyval = yyvsp[0]; }
break;
case 276:
{ yyval = yyvsp[0]; }
break;
case 277:
{ yyval = (dsql_nod*) NULL; }
break;
case 278:
{ yyval = make_list (yyvsp[0]); }
break;
case 279:
{ yyval = NULL; }
break;
case 281:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 282:
{ yyval = yyvsp[-1]; }
break;
case 285:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 286:
{ yyval = NULL; }
break;
case 287:
{ yyval = NULL; }
break;
case 288:
{ yyval = yyvsp[0]; }
break;
case 289:
{ yyval = yyvsp[0]; }
break;
case 290:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 293:
{ yyval = yyvsp[-1]; }
break;
case 294:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 295:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 296:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 298:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 315:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 316:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 321:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
break;
case 322:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
break;
case 323:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 324:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
break;
case 325:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 326:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 327:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
break;
case 328:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 329:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 330:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
break;
case 331:
{ yyval = NULL; }
break;
case 332:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
break;
case 333:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 336:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 337:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 338:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 339:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 340:
{ yyval = NULL; }
break;
case 341:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 342:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 343:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
break;
case 344:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 345:
{ yyval = NULL; }
break;
case 347:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 358:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 359:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 360:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 361:
{ yyval = NULL; }
break;
case 362:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 363:
{ yyval = make_list (yyvsp[0]); }
break;
case 364:
{ yyval = make_list (yyvsp[-1]); }
break;
case 365:
{ yyval = NULL; }
break;
case 366:
{ yyval = make_list (yyvsp[0]); }
break;
case 367:
{ yyval = make_list (yyvsp[-1]); }
break;
case 368:
{ yyval = NULL; }
break;
case 369:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 370:
{ yyval = make_list (yyvsp[-1]); }
break;
case 371:
{ yyval = NULL; }
break;
case 373:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 374:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 375:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 376:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 377:
{ lex.beginning = lex_position(); }
break;
case 378:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 379:
{ lex.beginning = lex.last_token; }
break;
case 380:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 381:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 382:
{ yyval = 0; }
break;
case 383:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 384:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 385:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 387:
{ yyval = NULL; }
break;
case 388:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 405:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 406:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 407:
{ yyval = NULL; }
break;
case 408:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 412:
{ yyval = yyvsp[0]; }
break;
case 413:
{ yyval = yyvsp[0]; }
break;
case 414:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 415:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 416:
{ yyval = make_node (nod_mod_index, (int) e_mod_idx_count, yyvsp[0]); }
break;
case 417:
{ yyval = yyvsp[0]; }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = yyvsp[0]; }
break;
case 420:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 422:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 423:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 427:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 428:
{ yyval = yyvsp[0]; }
break;
case 429:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 431:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 432:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 433:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 434:
{ yyval = yyvsp[0]; }
break;
case 435:
{ yyval = yyvsp[0]; }
break;
case 436:
{ yyval = make_node(nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 437:
{ yyval = make_node(nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 438:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], yyvsp[0], NULL); }
break;
case 439:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-3], NULL,
					make_node(nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0])); }
break;
case 440:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], NULL,
					make_node(nod_del_default, (int) 0, NULL)); }
break;
case 441:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 483:
{ yyval = NULL; }
break;
case 484:
{ yyval = NULL; }
break;
case 485:
{ yyval = NULL; }
break;
case 486:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 487:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 488:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 489:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 490:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 491:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 492:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 493:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 494:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 495:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 496:
{ yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 497:
{ yyval = yyvsp[0]; }
break;
case 498:
{ yyval = NULL; }
break;
case 499:
{ yyval = yyvsp[0]; }
break;
case 500:
{ yyval = NULL; }
break;
case 501:
{ yyval = NULL; }
break;
case 503:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 504:
{ yyval = yyvsp[0]; }
break;
case 505:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 506:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 507:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 508:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 509:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 511:
{ yyval = NULL; }
break;
case 513:
{ yyval = NULL; }
break;
case 514:
{ yyval = yyvsp[0]; }
break;
case 515:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 516:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 517:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 518:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 519:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 520:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 521:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 522:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 523:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 524:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 525:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 526:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 527:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 532:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 533:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 535:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 536:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 537:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 543:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 546:
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
case 547:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 553:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 554:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 555:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 556:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 557:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 558:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 559:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 560:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 562:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 563:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 564:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 565:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 566:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 567:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 576:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 577:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 578:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 579:
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
case 580:
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
case 583:
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
case 584:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 585:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 586:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 587:
{ yyval = yyvsp[-1]; }
break;
case 588:
{ yyval = 0; }
break;
case 592:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 593:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 594:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 598:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 599:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 600:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 601:
{ yyval = 0; }
break;
case 602:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 605:
{ yyval = make_node (nod_commit, e_commit_count, yyvsp[0]); }
break;
case 606:
{ yyval = make_node (nod_rollback, e_rollback_count, yyvsp[0]); }
break;
case 609:
{ yyval = make_node (nod_retain, 0, NULL); }
break;
case 610:
{ yyval = NULL; }
break;
case 612:
{ yyval = NULL; }
break;
case 613:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 615:
{ yyval = NULL; }
break;
case 617:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 622:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 623:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 624:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 625:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 626:
{ yyval = yyvsp[0];}
break;
case 628:
{ yyval = yyvsp[0];}
break;
case 629:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 630:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 631:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 632:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 633:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 634:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 635:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 636:
{ yyval = 0; }
break;
case 637:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 638:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 639:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 640:
{ yyval = (dsql_nod*) 0; }
break;
case 641:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 642:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 644:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 645:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 646:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 647:
{ yyval = 0; }
break;
case 649:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 650:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
break;
case 651:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
break;
case 652:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
break;
case 653:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
break;
case 654:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
break;
case 655:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
break;
case 656:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
break;
case 657:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
break;
case 658:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
break;
case 659:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
break;
case 660:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
break;
case 661:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
break;
case 662:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
break;
case 663:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
break;
case 664:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
break;
case 665:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
break;
case 666:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
break;
case 667:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
break;
case 668:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
break;
case 669:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
break;
case 670:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
break;
case 671:
{ yyval = yyvsp[0]; }
break;
case 673:
{ yyval = NULL; }
break;
case 674:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 675:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
break;
case 676:
{ yyval = NULL; }
break;
case 677:
{ yyval = yyvsp[0]; }
break;
case 678:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 679:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 680:
{ yyval = NULL; }
break;
case 681:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 682:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 683:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 686:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 688:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 689:
{ lex.limit_clause = true; }
break;
case 690:
{ lex.limit_clause = false; }
break;
case 691:
{ lex.first_detection = true; }
break;
case 692:
{ lex.first_detection = false; }
break;
case 693:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 694:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 695:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 696:
{ yyval = 0; }
break;
case 697:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 698:
{ yyval = yyvsp[-2]; }
break;
case 699:
{ yyval = yyvsp[-1]; }
break;
case 700:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 701:
{ yyval = yyvsp[-1]; }
break;
case 702:
{ yyval = yyvsp[0]; }
break;
case 703:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 704:
{ yyval = 0; }
break;
case 705:
{ yyval = make_list (yyvsp[0]); }
break;
case 706:
{ yyval = 0; }
break;
case 708:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 710:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 713:
{ yyval = make_list (yyvsp[0]); }
break;
case 715:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 720:
{ yyval = yyvsp[-1]; }
break;
case 721:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 723:
{ yyval = NULL; }
break;
case 724:
{ yyval = make_list (yyvsp[-1]); }
break;
case 725:
{ yyval = NULL; }
break;
case 727:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 732:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 733:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 736:
{ yyval = yyvsp[0]; }
break;
case 737:
{ yyval = make_list (yyvsp[-1]); }
break;
case 738:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 739:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 740:
{ yyval = make_list (yyvsp[-1]); }
break;
case 741:
{ yyval = NULL; }
break;
case 743:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 744:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 745:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 746:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 747:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 748:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 749:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 752:
{ yyval = make_list (yyvsp[0]); }
break;
case 753:
{ yyval = NULL; }
break;
case 755:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 757:
{ yyval = yyvsp[0]; }
break;
case 758:
{ yyval = NULL; }
break;
case 759:
{ yyval = yyvsp[0]; }
break;
case 760:
{ yyval = NULL; }
break;
case 761:
{ yyval = yyvsp[0]; }
break;
case 762:
{ yyval = NULL; }
break;
case 763:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 764:
{ yyval = 0; }
break;
case 765:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 766:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 767:
{ yyval = 0; }
break;
case 768:
{ yyval = 0; }
break;
case 770:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 775:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 776:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 777:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_list (yyvsp[-1]); }
break;
case 781:
{ yyval = 0; }
break;
case 782:
{ yyval = make_list (yyvsp[0]); }
break;
case 783:
{ yyval = 0; }
break;
case 785:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 787:
{ yyval = 0; }
break;
case 788:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 789:
{ yyval = 0; }
break;
case 790:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 791:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 792:
{ yyval = yyvsp[-1]; }
break;
case 793:
{ yyval = 0; }
break;
case 794:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 796:
{ yyval = NULL; }
break;
case 797:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
				yyvsp[-6], make_list (yyvsp[-5]), make_list (yyvsp[-2]), NULL, yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_insert, (int) e_ins_count,
				yyvsp[-2], yyvsp[-1], NULL, yyvsp[0], NULL); }
break;
case 801:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 802:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 806:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[0]), NULL); }
break;
case 808:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[-2]), make_list (yyvsp[0])); }
break;
case 809:
{ yyval = NULL; }
break;
case 810:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 812:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 814:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 815:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 819:
{ yyval = NULL; }
break;
case 822:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 823:
{ yyval = yyvsp[0]; }
break;
case 824:
{ yyval = NULL; }
break;
case 825:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 828:
{ yyval = NULL; }
break;
case 829:
{ yyval = make_list (yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 833:
{ yyval = NULL; }
break;
case 834:
{ yyval = make_list (yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 838:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 839:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 840:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 842:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 844:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 846:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 847:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 850:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 851:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 852:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 853:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 855:
{ yyval = yyvsp[-1]; }
break;
case 856:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 868:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 869:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 870:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 871:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 872:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 873:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 874:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 875:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 876:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 877:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 878:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 879:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 880:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 881:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 882:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 883:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 884:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 885:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 886:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 887:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 888:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 889:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 890:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 891:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 894:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 895:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 896:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 897:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 898:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 899:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 900:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 901:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 902:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 903:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 904:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 905:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 906:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 907:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 908:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 909:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 910:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 911:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 912:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 913:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 914:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 915:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 916:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 917:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 918:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 919:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 921:
{ yyval = make_list (yyvsp[-1]); }
break;
case 922:
{ yyval = yyvsp[-1]; }
break;
case 933:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 934:
{ yyval = yyvsp[0]; }
break;
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 936:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 937:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 938:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 939:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 940:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 941:
{ yyval = yyvsp[-1]; }
break;
case 942:
{ yyval = yyvsp[-1]; }
break;
case 946:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 947:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 948:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 951:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
break;
case 952:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 1, yyvsp[0]);
			}
break;
case 953:
{ yyval = make_node (nod_current_timestamp, 1, yyvsp[0]); }
break;
case 954:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 955:
{ yyval = NULL; }
break;
case 956:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 958:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 960:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 961:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 962:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 963:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 964:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 965:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 967:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 968:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
			}
break;
case 969:
{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
break;
case 970:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 971:
{ yyval = make_parameter (); }
break;
case 972:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 973:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 974:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 975:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 976:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 977:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 978:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 979:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 980:
{ yyval = yyvsp[0]; }
break;
case 981:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 983:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 984:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 985:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 986:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 987:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 989:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 990:
{ yyval = yyvsp[0];}
break;
case 994:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 995:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 996:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 997:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 998:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 999:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 1000:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 1001:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 1002:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 1003:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 1004:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 1007:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 1011:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1012:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1013:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1014:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1017:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 1018:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
break;
case 1019:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 1020:
{ yyval = yyvsp[0]; }
break;
case 1021:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 1022:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
break;
case 1023:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
break;
case 1024:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
break;
case 1025:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
break;
case 1026:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
break;
case 1027:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
break;
case 1028:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
break;
case 1029:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1030:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 1031:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 1034:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 1035:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 1036:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1039:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1040:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1041:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1042:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1043:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1044:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1045:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1046:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1050:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			}
break;
case 1051:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 1052:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 1053:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 1054:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 1055:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 1056:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 1057:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 1058:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 1059:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 1064:
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
