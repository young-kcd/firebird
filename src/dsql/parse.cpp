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
  157,  157,  157,  428,  230,  230,  152,  152,  434,  434,
  434,  434,  434,  430,  430,  430,  430,  430,  234,  153,
  153,  154,  155,  155,  155,  155,  155,   55,   55,   22,
   22,  244,  435,   58,  286,  265,  265,  106,  429,  429,
  429,  436,  436,  436,  436,  436,  436,  436,  436,  436,
  436,  436,  437,  437,  439,  440,  440,  440,  442,  443,
  443,  444,  438,  438,  438,  438,  445,  447,  447,  446,
  446,  446,  446,  448,  448,  448,  400,  400,  431,  432,
  432,  449,  449,  449,  450,  450,  451,  451,  454,  454,
  452,  452,  457,  457,  456,  453,  455,  433,  433,  441,
  441,  441,  441,  441,  441,  441,  441,  352,  352,  336,
  336,  156,   52,   68,  131,  148,  408,  166,  190,  273,
   93,   66,  223,  123,   73,  356,  219,  324,   36,   45,
  365,  272,   43,   46,  217,   44,  299,  264,  264,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
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
    1,    1,    1,    4,    1,    3,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    2,    2,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    1,
    2,    1,    1,    1,    1,    1,    2,    1,    1,    1,
    1,    4,    5,    5,    5,    5,    5,    5,    5,    5,
    5,    5,    1,    1,    6,    1,    1,    1,    4,    4,
    4,    4,    1,    1,    4,    4,    7,    2,    0,    7,
    6,    6,    4,    1,    1,    1,    4,    3,    6,    1,
    1,    6,    8,    6,    1,    1,    4,    6,    4,    5,
    3,    5,    4,    5,    1,    1,    1,    4,    6,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,
    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
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
    0,  103,    0,  982,    0,   23,  980,    0,    0,   74,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  514,    0,    0,    0,   37,   34,
   36,    0,   35,    0, 1088, 1090, 1091, 1092, 1093, 1094,
 1095, 1114, 1115, 1096, 1098, 1097, 1103, 1099, 1104, 1100,
 1102, 1105, 1101, 1106, 1107, 1108, 1110, 1111, 1109, 1112,
 1113, 1117, 1116, 1118, 1119, 1120,    0,    0,    0,    0,
   32,   69,   67, 1080, 1089,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  695,    0,    0,
    0,  742, 1082,    0,    0,    0,    0,    0,    0,  116,
 1087,  598,    0,    0,    2,    0,    0,    0,    0,    0,
    0,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  452,  453,  454,  455,  456,  457,  458,  459,  460,  461,
  462,  463,  471,  465,  466,  464,  467,  468,  469,  470,
  472,  474,  477,  473,  475,  476,  478,  479,  480,  481,
  482,    0,  441,  442,    0,  410, 1071,    0,    0,  416,
 1075,    0,  413, 1079,    0,  744,    0,  412, 1083,    0,
  417, 1074,    0,  605,  978,    0,  165,  112,    0,  113,
    0,  215, 1067,  840,  104,    0,  110,  160,    0,    0,
  106,    0,  114,  984,    0,  107,    0,  108,    0,  109,
 1086,  161,  115,  111,  787,  788,    0,  983,  981,    0,
   75,    0, 1072,    0, 1070,  522,  515,    0,  521,  526,
  516,  517,  524,  518,  519,  520,  525,  527,    0,    0,
    0,   30,    0,   39,  827,   38,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  606,  988,    0,  969,  689,  689,  690,  700,  702, 1058,
  703,    0,  704,  694,  690,    0,    0,    0,    0,    0,
    0,    0,  624,  613,    0,  616,  618,  619,  620,  621,
  627,  628, 1081,  743,    0,  120,    0,    0,  117,    0,
  118,    0,  119,    0,    0,  654,  655,  662,    0,  661,
  663,  665,  670,  658,  656,  659,  657,  666,  669,  664,
  668,    0,    0,    0,    0,    0,  674,    0,    0, 1060,
    0,    0,  681,    0,    0,    0,    0,    0,  502,    0,
    0,  379,    0,  218,    0,  421,  128, 1063,    0,  418,
  491,  492,    0,    0,    0,    0,    0,    0,  430,    0,
  385,  386,    0,    0,  611,  609,  979,    0,    0,    0,
    0,    0,    0,    0,  168,  147,    0,  125,    0,    0,
    0,  121,    0,  136,  135,    0,    0,    0,    0,    0,
    0,   76,    0,    0,    0,    0,  802,  523,    0,    0,
    0,    0,  946,  975,    0, 1062,    0,  948,    0,    0,
  976,    0,  970,  961,  960,  959,    0,    0,    0,    0,
  951,  952,  953,  962,  963,  971,  972,    0,  973,  974,
    0,    0,    0,  977,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  965,    0,  837,
  943,  944,  945,  950,  949,  928,    0,    0,    0,  927,
    0,    0,  932,  924,  925,  926,  929,  930,  931,  964,
  989,  990,  991, 1003, 1004, 1006, 1007, 1008, 1013, 1014,
 1030, 1031, 1035, 1036,  216,    0,    0,    0,  372,    0,
    0,    0,  830,   27,    0,    0,    0,    0,   72,   70,
 1084,   68,   33,    0,    0,    0,  832,    0,   48,   50,
    0,    0,    0,  603,    0,    0,    0,  697,  699,    0,
  706,    0,    0,    0,  707,  693,    0,  650,    0,  625,
    0,  622,    0,  623,  648,    0,  643,    0,    0,  617,
  841,  811,    0,    0,    0,  126,    0,    0,    0,  600,
  599,  667,  660,    0, 1078,    0,    0,    0,  675,  679,
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
 1064,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  843,  845,  854,  857,  858,  859,  860,  861,  862,
  863,  864,  865,  866,  867,    0,    0,    0,    0,    0,
  966,    0,    0,    0,    0,    0,    0,  967,  968,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  333, 1085,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  362,    0,    0,  274,  538,    0,    0,    0,  370,
    0,    0,    0,  829,    0,    0,   44,   73,    0,    0,
   26,    0,  835,    0,    0,  798,    0,    0,    0,    0,
  602,    0,  689,    0,  711,    0,    0,    0,    0,  593,
    0,  986,  592,    0,  626,    0,  634,  630,  629,    0,
    0,    0,  645,  633,    0,    0,    0,  806,    0,    0,
    0,  377,  673,  672,  651,    0,    0,    0,    0,    0,
    0,    0,    0,  145,    0,  506,  379,  424,    0,  379,
  380,  429,  497,    0,  496,    0,    0,  265,  377,  238,
 1068,    0,    0,  379,    0,    0,  207,  377,    0,    0,
    0,    0,  243,  237,  240,  241,  242,  433,  489,  488,
  432,  431,    0,    0,    0,    0,    0,    0,    0,    0,
  388,    0,  379,  494,    0,  493,    0,  170,  174,    0,
    0,  178,    0,    0,    0,  569,  570,  586,    0,  583,
    0,    0,  576,  574,  575,    0,    0,    0,    0,  539,
    0,    0,    0,    0,  577,  127,    0,    0,  377,    0,
  198,    0,  201,  203,  204,    0,    0,    0,    0,    0,
  218,   79,    0,    0,   78,    0,    0,    0,    0,  844,
  856,    0,    0,  917,  918,  919,    0,  848,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  764,  766,    0,
  761,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1050, 1051, 1052,
 1053, 1054, 1055, 1056, 1057,    0,    0,    0,    0,    0,
    0, 1041,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1024, 1026, 1025,    0,    0,  941,  364,    0,
  942, 1028,    0,  937, 1066,    0,    0,    0,    0,    0,
    0,    0,    0,  334,  335,    0,    0,  947,  839,  838,
  374,  373,    0,  272,    0,    0,    0,  280,  831,    0,
    0,    0,   71,    0,    0,    0,  834,    0,    0,    0,
    0,  698,  701, 1076,  710,    0,    0,    0,    0,  716,
  717,  718,  719,  728,  729,  730,    0,    0,  708,  594,
  987,  635,  644,  639,  638,    0,  649,  842,  812,    0,
    0,  377,    0,    0,  652,  671,    0,  691,  786,  785,
    0,  505,    0,    0,  182,    0,  186,    0,    0,    0,
  423,  499,  377,  377,  277,  380,  270,    0,  209,  379,
  211,  379,  380,  206,  212,    0,    0,    0,    0,  436,
  437,  440,  380,  379,  377,  438,    0,    0,    0,  406,
    0,  495,  194,  195,  171,    0,  181,  557,  558,  985,
    0,    0,    0,    0,    0,  584,    0,    0,  380,    0,
    0,  534,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  196,    0,    0,    0,   83,    0,
    0,    0,    0,   91,   85,    0,    0,   90,   86,    0,
  810, 1069,    0,    0,  849,    0,    0,  855,    0,    0,
    0,    0,    0,  893,  892,    0,    0,    0,  902,  920,
    0,  912,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  847,    0,  765,    0,  801,    0,    0,  218,
    0,  992,    0,    0, 1015,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1037,    0,    0,
    0,    0,    0,    0,    0, 1048, 1009, 1010, 1011, 1016,
 1012,    0, 1023,    0,    0,  683, 1027,  954,    0,    0,
    0,    0,  267,  286,    0,    0,  369,  281,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   54,   62,   64,
    0,   42,    0,    0,    0,  816,  836,    0,  815,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  745,    0,
    0,    0,    0,    0,    0,    0,  641,  642,  646,    0,
    0,  197,    0,  653,    0,    0,    0,    0,  187,    0,
    0,    0,  419,  221,  222,  223,  224,  225,  226,  957,
    0,    0,  271,    0,    0,    0,    0,    0,    0,    0,
  244,    0,  439,  485,  486,    0,    0,    0,    0,    0,
    0,  379,  512,  380,  180, 1065,    0,    0,  552,    0,
  551,  587,    0,  579,    0,    0,    0,  532,  560,    0,
    0,  562,  565,  567,  377,    0,    0,    0,  134,    0,
  202,  379,    0,  378,  105,    0,  132,    0,   94,    0,
    0,    0,    0,   88,   89,    0,  910,  911,  851,    0,
  853,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  913,    0,    0,    0,    0,    0,    0,    0,  903,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  772,    0,    0,    0,  998,  997,
    0,    0,  994,  993,    0, 1002, 1001, 1000,  999,  996,
  995,    0,    0, 1043,    0,    0,    0, 1042,    0,    0,
    0,    0,    0,    0,    0,  367,  337,  336,  273,    0,
    0,  283,  284,    0,    0,    0,    0,  316,    0,    0,
    0,  315,    0,    0,    0,    0,  361,  304,  302,  306,
    0,  303,  292,  297,  291,    0,    0,    0,  300,  301,
  305,  307,  308,  309,  310,  311,  312,  313,  314,  317,
  318,  319,  320,    0,    0,  355,  356,  357,    0,  814,
   66,   58,   59,   65,   60,   61,    0,   24,    0,    0,
   25,  822,  821,    0,  820,  818,  826,  825,  823,    0,
   45,   46,    0,  720,    0,    0,    0,  750,  748,  746,
    0,  747,    0,    0,    0,    0,    0,  805,    0,  378,
  790,  791,  692,    0,  192,  188,    0,  380,  958,  275,
  276,  380,    0,  149,    0,    0,  229,    0,    0,  245,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  509,
  554,    0,  555,    0,    0,    0,    0,  153,  155,  156,
  157,  537,  535,  533,  564,    0,    0,  380,    0,    0,
    0,  375,  379,   92,    0,    0,   98,   93,    0,    0,
    0,    0,    0,  921,    0,  922,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  774,  763,    0,    0,
  775,    0,  771,  217, 1029, 1049, 1005,    0,    0, 1039,
 1038,    0, 1044, 1032, 1034,    0, 1021, 1022,    0,    0,
  282,    0,    0,    0,    0,    0,  343, 1077,  358,  359,
    0,    0,  293,    0,  298,    0,  346,  299,    0,    0,
  339,    0,   55,   56,   57,   63,    0,    0,  797,    0,
  740,  738,    0,  731,    0,    0,    0,  754,    0,  688,
  380,  376,  792,  189,  380,  159,  264,    0,    0,  205,
  230,    0,    0,    0,    0,  231,  234,    0,    0,  380,
  404,  405,  401,  400,  403,  402,    0,  553,  580,  158,
  146,  154,    0,  379,  261,  141,  380,  381,    0,    0,
   96,    0,   80,    0,  879,  887,  682,    0,  880,  888,
  883,  891,    0,    0,  881,  889,  882,  890,  876,  884,
  877,  885,  878,  886,  770,    0,    0,    0, 1017, 1040,
    0, 1020,    0,    0,  289,  285,    0,    0,    0,    0,
  330,    0,    0,  354,    0,    0,    0,    0,  349,  347,
    0,    0,    0,   40,  817,    0,    0,  722,  732,    0,
    0,  733,  734,  735,    0,  262,  420,  208,  213,  232,
    0,    0,  235,    0,  247,  249,  408,  380,    0,  383,
    0,   77,   99,  100,    0,    0,    0,  777, 1033,  219,
  288,    0,    0,    0,    0,  353,  352, 1073,  351,    0,
    0,    0,    0,    0,    0,    0,  721,    0,    0,  755,
  236,    0,    0,    0,    0,    0,  263,  380,    0,    0,
  776,    0,    0,    0,    0,    0,  348,  350,    0,    0,
    0,    0,  726,    0,    0,    0,    0,    0,  246,    0,
  253,    0,  252,  384,  133,  779,    0,  290,    0,    0,
    0,    0,    0,    0,  724,  737,  233,    0,    0,  257,
  256,  255,  780,    0,    0,    0,    0,  338,  727,  260,
  258,  259,  328,    0,  344,  325,  326,
};
static short yydgoto[] = {                                      21,
   22,   23,   24,   25,   26,   27,   28, 1578,   30,   31,
 1579, 1580,   34,   35,   36,   37,   38,   39, 1581,   41,
 1582, 1622,  147,  565,  172, 1365, 1618,  148,  836,  149,
  568,  841,  150,  151,  314, 1127,  161,  162, 1366, 1367,
 1368, 1369,  247,  279,  152, 1370,  153,  570,   90,  291,
  462,  517,  744, 1242,  518,  994,  713,  272,  970,  745,
  746,  747, 1249, 1244, 1708,  292,  749,  750,   82,  265,
   83,  287, 1925, 1475,  271,  276,  278,  280,  267,  258,
  260,  273,  283,  180,  369,  371,  373,  366,  452,  731,
  733,  728,  235,  236,  562,  315,  913,  647,  760, 1171,
  456,  736, 1233, 1469,  681,  862, 1205,  639,  640,  662,
  447,  714, 1183, 1686, 1810, 1413, 1687, 1688, 1689, 1690,
 1691,  761,  268,  259,  443,  689,  444,  445,  690,  691,
 1445,  641, 1165, 1166, 1167, 1408,  458,  982,  983,  984,
  985,  915, 1665,  715,  917,  520,  987, 1084,  556, 1532,
 1875, 1414,  521,  522,  523,  524,  525, 1666, 1667,  665,
 1816, 1954, 1431,  923,  924,  910,  925,  926,  927, 1955,
 1956, 2001,  424,  561, 1106, 1583,  243,  658, 1103,  659,
 1176,  557, 1107, 1108, 1355, 1561, 1562, 1563, 1876, 1251,
 1584, 1585, 1586, 1587, 1776, 1588, 1589, 1590, 1591, 1592,
 1593, 1594, 1595, 1596, 1597, 1598, 1599, 1600, 1601, 1602,
 1603, 1604, 1093, 1992, 1881,  526,  811,  527, 1605, 1777,
 1888, 1889, 1937, 1606, 1607, 1608, 1771,  528,  822,  529,
  311,  558,  559,  530,   43, 1474, 1702,  433,  676,  943,
 1443,  677,  941,   87,   66,  428,  248,  191,  408,  232,
  415,  240,  251,  420,  901,  898,  416,  429,  931,  430,
  672, 1196,  233,  531, 1220,  656,  905,  409,  678, 1444,
  105,  246,  296,  716,  717,  826, 1221,  827, 1222,  720,
  721,  722,  723,  955, 1214, 1212,  724,  725,  726,  963,
  727,  960,   44,   45,   46,   47,   48,   49,  182,  621,
   68,  585,  254,  436,  354,  355,  356,  357,  358,  359,
  360,  361,  362,  868,  606, 1146, 1399,  607,  608,  873,
  392,  885,  393,  626,  394,  888,  187,  397,  629,   50,
  190,  403, 1498,  810,   51,  401,   52,  166,  342,  593,
  858,  466, 1396, 1647,  777,  588,  344, 1405, 1803,  167,
  168,  343,  594,  595,  856, 1125, 1128, 1129, 1130, 1131,
 1132, 1133, 1897, 1947,  532, 1974, 1134, 1135, 1136, 1394,
 1902, 1903, 1904, 1386,  175, 1639, 1797, 1798, 1525, 1032,
 1526, 1527, 1528, 1743, 1926, 1928,  632,  633, 1159, 1653,
  576, 1789,   53,   54,  467,   55,   56,  614,  615,  533,
 1115, 1376, 1624, 1625, 1629,  577,  844,  264,  762,  763,
 1009, 1010,  764,  765,  766,  767,  768,  769,  770,  771,
  772,  773,  774,  775, 1267, 1269, 1270,  534,  535,  536,
  537,  538,  539,  540,  289,  541,  542,  543,  544,  545,
 1056,  546,  547,  548,  549,  550, 1749, 1077,  551,  552,
  553,  554,  793, 1060, 1331, 1326,  794,  155,
};
static short yysindex[] = {                                   8028,
 1573,  -97, 3104,   92, 1083,  148, 3404,  434,16365,   58,
  272, -121,  -97,  942,  670,12783,  933,12783,  106,  249,
    0,  577,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  536,    0,    0,    0,    0,    0,    0,  752,
    0,    0,    0,    0,    0,    0,    0,17389,12783,  538,
12783,12783,12783,12783,12783,    0,    0,  617, 1087,12783,
12783,12783,  653,12783,  487,12783,12783,    0,12783,12783,
12783,    0,   72,    0,  610,    0,    0,  763,12783,    0,
12783,12783,12783,  799,12783,12783,12783,12783,  487,12783,
12783,12783,12783,12783,    0,12783, 1096,  758,    0,    0,
    0, 1166,    0, 1166,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  784,  869,  108, 1286,
    0,    0,    0,    0,    0,12783,12783,12783,  897,  900,
 2838,12783,  852,  147,  212,   80,  859,    0,12783, 1059,
 1271,    0,    0,12783,  993,12783,12783,12783,12783,    0,
    0,    0,12783,16094,    0,  999, 1042, 1236, 1116, 1007,
 1411,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  848,    0,    0, 1087,    0,    0, 1118,   42,    0,
    0, 1536,    0,    0, 1157,    0,  541,    0,    0, 1081,
    0,    0, 1179,    0,    0, 1197,    0,    0, 1145,    0,
 1348,    0,    0,    0,    0, 1087,    0,    0,  540, 1536,
    0,  495,    0,    0, 1306,    0, 1317,    0, 1166,    0,
    0,    0,    0,    0,    0,    0, 1334,    0,    0, 1118,
    0, 1323,    0, 1230,    0,    0,    0, 1118,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,15213,12783,
 1283,    0,12783,    0,    0,    0, 1256, 1320,11990,12783,
 1172, 1364, 1679, 1408, 1422, 1430, 1375, 1377,  136, 1260,
    0,    0,15469,    0,    0,    0,    0,    0,    0,    0,
    0,13165,    0,    0,    0, 1350,12783, 1420, 1352,  596,
12783, 1357,    0,    0, 1271,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12783,    0, 1087, 1536,    0, 1306,
    0, 1166,    0, 1413, 1398,    0,    0,    0, 1483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1479,12783,12783, 1450, 1336,    0,15469, 1453,    0,
 1453,15469,    0,  325, 1353, 1369, 1361, 1411,    0, 1589,
 1027,    0,12783,    0,  848,    0,    0,    0, 1574,    0,
    0,    0,12783, 1283,12258, 1418,12521, 1840,    0,17389,
    0,    0, 1072, 1442,    0,    0,    0, 1828, 1828, 1087,
 1528, 1087,  472, 1145,    0,    0,11311,    0,12783,12783,
12783,    0, 1283,    0,    0, 1628,  724, 1875,12783, 1653,
12783,    0, 1881,10121, 9213, 1537,    0,    0, 1885, 1887,
 1894, 1489,    0,    0, 1908,    0, 1919,    0, 1920, 1922,
    0, 1924,    0,    0,    0,    0,    0, 1927, 1517, 1519,
    0,    0,    0,    0,    0,    0,    0, 1929,    0,    0,
13421, 1933, 1934,    0, 1936, 1626, 1938, 1939, 1951, 1952,
 1954, 1956,15469,15469,13677,12783, 1961,    0, 1689,    0,
    0,    0,    0,    0,    0,    0, 1913, 1605, 1964,    0,
    0, 1963,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,11311, 1950,  975,    0, 1972,
 1752, 1481,    0,    0,12783,12783,12783,   83,    0,    0,
    0,    0,    0,12783,12783, 1043,    0,12783,    0,    0,
 1256, 1623,11990,    0,12783,13677, 2010,    0,    0,15469,
    0, 1896, 1702, 1981,    0,    0,  267,    0,  746,    0,
  -40,    0,  -40,    0,    0, 1988,    0,  154, 1613,    0,
    0,    0, 1989,  122, 1973,    0, 1283, 1997, 1774,    0,
    0,    0,    0,  -57,    0, 1704, 2012,12783,    0,    0,
 1825, 2015,    0,    0,    0, 1012,    0, 1712, 1716,    0,
 1087,    0, 1717,    0,    0, 1789, 1790,    0,    0,    0,
 1765,    0,11311,    0, 1087, 1707,11311, 1634,    0, 1803,
12783,17063,    0,    0,  691,    0,12783,  305, 1157, 1680,
 1648,  503,    0,    0,    0,    0,  530, 1684,  277,    0,
 1629,  487,    0, 1087,    0, 1813, 1734,    0,    0,  472,
    0,    0,  166, 1650,    0,    0,    0, 1652,    0, 1701,
 2054,    0, 1751, 2057,  546,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2007,  195,    0,
    0,    0,    0,   89, 2059, 2060, 2057,    0, 1087, 1536,
    0, 1793,    0, 1839,    0, 1087, 1087,    0,12258,  541,
    0, 1729,    0, 1706, 2066,    0, 5471,    0, 1733,    0,
    0, 1737, 2076, 9469, 2080,    0,    0,    0, 8950,10475,
  547,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  829, 1741, 1148,15469,  629,
    0,12783,15469, 1212, 1229, 1294, 2289,    0,    0,15469,
 9725, 1689, 1683,  548,15469,15469, 9725, 1808,15469,15469,
15469,15469,15469, 9981,  209,  209, 2040, 1663,  752, 2086,
    0,    0,13933,12783,15469,15469,15469,15469,15469,15469,
 6661,    0,15469, 9599,    0,    0, 1853, 2072,12783,    0,
12783, 1833,12783,    0, 1709, 1710,    0,    0, 1685,11990,
    0, 1831,    0, 1694, 2099,    0, 1831,12783,12783, 2103,
    0, 2040,    0, 2089,    0,12783, 9343, 1708,15469,    0,
  949,    0,    0,  -56,    0, 1711,    0,    0,    0,12783,
  -51,12783,    0,    0,12783,12783, 1537,    0,15469, 1886,
12258,    0,    0,    0,    0,  -57,12783, 1812, 2109, 1661,
15469,15469, 1087,    0,  607,    0,    0,    0, 2116,    0,
    0,    0,    0, 1087,    0,  619,12783,    0,    0,    0,
    0, 1884, 2118,    0, 1864, 1883,    0,    0, 1827, 1829,
 1166, 1892,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  487,12783, 1874, 1765,17161, 1791, 1797, 1800,
    0,  487,    0,    0, 1073,    0, 1321,    0,    0, 1762,
 1087,    0,10121,   77, 1773,    0,    0,    0,  487,    0,
 2054,   99,    0,    0,    0, 1864,   99, 1772,   99,    0,
 2143,  487,  487,  487,    0,    0, 1283,12783,    0, 1832,
    0, 1723,    0,    0,    0, 1072, 1453,12783, 1728,   93,
    0,    0, 2145,   19,    0,10121,12783, 1453,10247,    0,
    0, 1453, 9469,    0,    0,    0, 6626,    0,   94,  571,
15469,15469,10514, 2147,   14,10781,15469,11048, 1233,11315,
11582,14189,11849,12116,12383, 9725, 9725,    0,    0, 1826,
    0, 2149, 1007,15469,15469, 2672,15469, 2152,15469, 2146,
 2101,15469,15469,15469,15469,15469,15469,    0,    0,    0,
    0,    0,    0,    0,    0, 1877, 1532,  -26,15469,  569,
15469,    0, 9725, 2184, 2233,  379,12783, 2154, 2295, 2359,
 2409, 2533,    0,    0,    0, 1160,14445,    0,    0, 1007,
    0,    0, 1730,    0,    0, 1921,  209,  209,  806,  806,
  289,10655, 2156,    0,    0, 2165, 1689,    0,    0,    0,
    0,    0, 1744,    0, 1764, 1942, 1833,    0,    0,11723,
11723, 1836,    0,  -15, 1854,12783,    0,15469, 1854, 1899,
 1900,    0,    0,    0,    0, 1873, 2179, 2189, 1392,    0,
    0,    0,    0,    0,    0,    0, 9725, 1909,    0,    0,
    0,    0,    0,    0,    0,  378,    0,    0,    0, 1741,
 1689,    0, 1748, 1453,    0,    0,  -57,    0,    0,    0,
 1689,    0, 1828, 1969,    0,  607,    0, 2196, 9725, 4905,
    0,    0,    0,    0,    0,    0,    0, 1833,    0,    0,
    0,    0,    0,    0,    0, 1166, 1166, 1753, 2197,    0,
    0,    0,    0,    0,    0,    0,  389,  -41,  712,    0,
 1977,    0,    0,    0,    0,12783,    0,    0,    0,    0,
   92, 1755, 1841, 1853, 2212,    0, 1629, 1763,    0, 2199,
 1205,    0,12783, 1269,  487, 2217, 2218, 2219, 1999,  541,
 1833, 1828, 1716,12258,    0, 1684, 1821, 9087,    0,  487,
 1878, 1962, 5471,    0,    0,  487,  263,    0,    0, 1975,
    0,    0, 2242, 2243,    0, 9725, 9725,    0, 9725, 9725,
 2841, 1689, 2245,    0,    0, 1689, 2249,13677,    0,    0,
 1974,    0,  570, 2250, 1689, 2253, 1362, 2254, 1689, 2256,
15469,15469, 2147,15469,14701, 2257, 1689, 2260, 2261, 1689,
 2262,15469, 1689, 2266, 1689, 2268, 2269, 1689, 2270, 2274,
 1689, 2275,    0, 2056,    0,17286,    0, 2540, 2555,    0,
 2585,    0, 2647,15469,    0, 2657, 2664, 2728, 2804, 2954,
 2972,15469,15469,15469, 1689, 1897,15469,    0,15469, 1689,
 2016,  429,15469,15469,15469,    0,    0,    0,    0,    0,
    0,15469,    0,15469, 1646,    0,    0,    0, 1780,10655,
 9856,12783,    0,    0,12783,16870,    0,    0,12783,12783,
12783,12783,12783,12783, 1880, 2279, 2283,    0,    0,    0,
 1880,    0,   73,   73,   67,    0,    0, 1788,    0,11723,
11723, 2288, 1392, 2298,15469, 2077, 9343, 1965,    0, 1965,
 1147, 1965, 2008, 2011, 2078, 2022,    0,    0,    0, 1007,
 1833,    0, 1821,    0, 1161, 1629, 1968, 1629,    0, 9725,
  117, 1452,    0,    0,    0,    0,    0,    0,    0,    0,
 4905, 4905,    0, 1942,15469, 4905, 2069, 1959, 1753,   72,
    0, 9725,    0,    0,    0, 1982, 1983, 1984, 1993, 2000,
 2001,    0,    0,    0,    0,    0, 2343,   92,    0, 1940,
    0,    0,   99,    0, 2014,   99,   99,    0,    0, 1853,
 2353,    0,    0,    0,    0, 1072, 1942, 1629,    0, 1716,
    0,    0, 2120,    0,    0, 2109,    0, 2357,    0, 2358,
 1087, -101, 2362,    0,    0, 1087,    0,    0,    0, 2056,
    0, 2056,15469, 1453, 1453, 1799,  752, 2380,15469, 2105,
    0, 1453, 1453,15469, 1453, 1453, 3327, 1689,    0, 1705,
15469, 1689, 1453, 1453, 1453, 1453, 1689, 1453, 1453, 1453,
 1453, 1453, 1453,12783,    0, 2382, 2388, 1284,    0,    0,
11311, 2392,    0,    0, 3027,    0,    0,    0,    0,    0,
    0, 3059, 1675,    0,15469, 2132, 2017,    0,15469, 3074,
 1819, 3446, 3134, 3187,15469,    0,    0,    0,    0,11311,
 2379,    0,    0, 2151,    0,12783,  -22,    0, 2403, 2110,
15469,    0,    0,12783,12783,12783,    0,    0,    0,    0,
 2111,    0,    0,    0,    0, 2144,16735, 2394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  358, 2397,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2134,    0,11723,11723,
    0,    0,    0, 2031,    0,    0,    0,    0,    0, 1928,
    0,    0, 2077,    0, 1849,12783, 1392,    0,    0,    0,
 2124,    0, 9343, 9343,15469, 9725, 1537,    0, 1942,    0,
    0,    0,    0, 1321,    0,    0,  168,    0,    0,    0,
    0,    0, 1689,    0, 2187, 2069,    0, 1264,12783,    0,
 2137,  172, 2038, 2131, 2041, 2176, 2142, 2181, 1833,    0,
    0, 2436,    0, 2441, 2135, 2187, 2014,    0,    0,    0,
    0,    0,    0,    0,    0, 1833, 1684,    0, 1321, 1977,
 2107,    0,    0,    0, 2121,  -67,    0,    0, 1853, 2122,
 1689, 2448, 2450,    0, 1007,    0, 1689,15469, 2455, 2457,
 1689, 2460, 2462,15469,15469, 1689, 2465, 2466, 2472, 2473,
 2475, 2480, 2481, 2482, 2483, 2484,    0,    0,17286, 2487,
    0,12783,    0,    0,    0,    0,    0,15469, 2488,    0,
    0,15469,    0,    0,    0,15469,    0,    0, 3198,  652,
    0, 2213,15469,15469, 9725, 1689,    0,    0,    0,    0,
12783,10655,    0, 1003,    0, 2087,    0,    0,  384, 2494,
    0, 2160,    0,    0,    0,    0,   73,12783,    0,12783,
    0,    0, 9343,    0, 2505, 1689, 2500,    0,  547,    0,
    0,    0,    0,    0,    0,    0,    0, 2499,12783,    0,
    0, 2198, 2207,12783, 1753,    0,    0, 1166,12783,    0,
    0,    0,    0,    0,    0,    0, 1942,    0,    0,    0,
    0,    0, 1942,    0,    0,    0,    0,    0,15469, 1087,
    0, 2095,    0, 1087,    0,    0,    0, 1689,    0,    0,
    0,    0, 1689, 1689,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,12783, 2222, 1689,    0,    0,
 3272,    0, 4905, 4905,    0,    0, 2517, 1689, 1500,  259,
    0, 2223, 2156,    0,12783,12783,   92,  178,    0,    0,
 2058, 2228, 9725,    0,    0,  145, 2527,    0,    0, 9725,
 2530,    0,    0,    0,15469,    0,    0,    0,    0,    0,
 1753, 1166,    0, 2203,    0,    0,    0,    0, 1977,    0,
 1689,    0,    0,    0, 2529, 2545, 2548,    0,    0,    0,
    0, 1453,10655, 2162,10655,    0,    0,    0,    0,16870,
 1003,15469,10655,  291,10655,12783,    0,  547,12783,    0,
    0, 2203,   20, 1753, 2216, 2225,    0,    0, 2552,12783,
    0,12783, 2558, 2156,16870, 2156,    0,    0, 1516,  223,
 2302, 2156,    0, 1855, 1863, 1753,  861,  861,    0, 2306,
    0, 2171,    0,    0,    0,    0, 2567,    0, 2307,10655,
 2320, 2312,16870,12783,    0,    0,    0, 2159,   29,    0,
    0,    0,    0,16870,  302,12783,16870,    0,    0,    0,
    0,    0,    0,16870,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  185,  843,    0,    0,    0,    0,    0,    0,    0,
    0,16441,  449,12646,    0,    0,    0,    0,    0,    0,
    0, 2616,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  196,    0,    0,    0,    0,    0,    0,  199,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  919,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2284,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   12, 2248,    0,    0,
    0,  118,    0,  118,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2255,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  919,    0,    0,14957,12909,    0,    0,    0,
 1020,    0,    0,  183,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  447,    0, 2210,  368,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   43,    0,    0,    0,    0, 1234,    0,    0,    0,
    0,    0, 1032,    0,    0,    0,    0,    0,  138,    0,
11578,    0,    0,    0,    0,    0,    0,    0,    0,   43,
    0,  513,    0,    0, 2589,    0,    0,    0, 2373,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  142,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  219,    0,
 2375,    0,    0,    0,    0,    0,13039,    0,    0,    0,
    0,    0, 1137,    0,    0,    0,    0,    0,    0,13295,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  329,    0,    0, 1050,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   43,    0, 2589,
    0, 2373,    0, 1120,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  262,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1171,    0, 2364,
    0,    0,    0,    0, 1174,    0,    0,    0,  250,    0,
    0,    0,    0, 2375,  780,17470,    0, 1231,    0,    0,
    0,    0,  738,    0,    0,    0,    0, 2192, 2192,    0,
    0,    0, 1240,  215,    0,    0,    0,    0,    0,    0,
    0,    0, 2375,    0,    0, 1388,    0,    0,    0,    0,
    0,    0,11044,    0,    0,  131,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2163,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3003,    0,    0,
    0, 3303, 3603,    0, 3903, 4203,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  693,    0,
    0,    0,    0,    0,    0,    0, 4503, 1250,  254,    0,
 2703,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1262,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
13039,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10398,    0, 2326,    0,    0,    0,    0,    0,    0,
  339,    0,  339,    0,    0,  360,    0,  715, 1111,    0,
    0,    0,    0,  142,    0,    0, 2375,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  182,  648,    0,    0,    0,  475,    0,    0,  923,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1268,    0,    0,    0,    0,
    0,    0,    0,    0, 2364,    0,    0,  936,    0,  510,
    0,    0, 1293,    0,    0,    0,    0,  328,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1279,
    0,    0, 2994,15990,    0,    0,    0,16002,    0,    0,
16211,    0,    0,16269,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1787, 2133,    0,
    0,    0,    0,16380,16039,    0,16269,    0,    0,   43,
    0,    0,    0,    0,    0,    0,    0,    0,  780, 1440,
    0,    0,    0,    0, 2246,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1322, 7278, 7357,    0,    0,
  188,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2606,  104,15725,    0,15725,
    0,    0,    0,15725,15725,15725,    0,    0,    0,    0,
    0, 2205,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 5692, 5981, 2607,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 4156,    0,    0,    0,
    0, 2389,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  288,    0,    0,    0,    0,  288,    0,    0, 1360,
    0,    0,    0,    0,    0,    0,    0, 1476,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  867,    0,    0,    0,    0,    0,  131,    0,    0,    0,
  780,    0,    0,    0,    0,    0,    0,    0,  295,  184,
    0,    0,    0,    0,  938,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1905,    0,    0,    0,    0,
    0, 2610,    0,    0, 2036, 2504,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  846,  924,  990,
    0,    0,    0,    0,    0,    0,  777,    0,    0,    0,
    0,    0,    0,    0, 3294,    0,    0,    0,    0,    0,
16211,    0,    0,    0,    0,  466,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2375,    0,    0,  265,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 5760,
    0,    0,    0,  279,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2621,
    0,    0, 1379,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2627,
    0,    0,    0,    0,    0, 4803, 6270, 6559, 5103, 5403,
    0,    0, 1386,    0,    0,    0,  751,    0,    0,    0,
    0,    0,    0,    0,13551,    0, 2399,    0,    0,    0,
    0,    0,    0,    0, 1412,    0,    0,    0, 1412,    0,
    0,    0,    0,    0,    0,    0, 2398,  681,  912,    0,
    0,    0,    0,    0,    0,    0,    0, 1021,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  104,
  120,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  952,    0, 2192, 2220,    0,  941,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2389,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1329,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1424,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 3594,    0,    0,    0,    0,    0, 1341,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1440,
 2389, 2192, 1431,  780,    0, 2410, 1457,    0,    0,    0,
 5760,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1075,    0,    0,    0, 1556,    0,    0,    0,    0,
    0,    0,    0,    0, 1786,    0, 1980,    0, 6480,    0,
    0,    0,    0,    0,    0,    0, 7489,    0,    0, 7517,
    0,    0, 7600,    0, 7697,    0,    0, 7780,    0,    0,
 7808,    0,    0, 1105,    0, 2606,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2281,    0,    0,    0,    0,  638,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  592,    0,    0,    0,    0,
    0,    0,    0,  239, 1469,  134,  214,    0,    0,    0,
 1469,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2334, 2624,    0, 6979,    0, 2336,    0, 2336,
 2334, 2336,    0,    0,    0,  709,    0,    0,    0, 1379,
 2389,    0, 1457,    0,    0,    0, 2234,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1063,    0, 1329, 2284,
    0,    0,    0,    0,    0, 1005, 1031, 1071, 1085, 1097,
 1123,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  753,    0,    0,    0,    0, 2894,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1505,
    0,    0,    0,    0,    0, 1524,    0,    0,    0,    0,
    0,  130,    0,    0,    0,    0,    0,    0,    0,  222,
    0,  284,    0,    0,    0,    0,   74,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 7940,    0, 8019,
    0, 8174,    0,    0,    0,    0, 8204,    0,    0,    0,
    0,    0,    0, 1305,    0,    0, 2646,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2652,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,17354, 2637,    0,    0,    0,    0,
    0,    0, 1698, 1792,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  642,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  297,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1526,
    0,    0, 6768,    0,    0,    0,  945,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  159,    0,    0,    0,
    0,    0,    0,  657,    0,    0,    0,    0,    0,    0,
    0,    0, 2656,    0, 1572, 3109,    0, 2364,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2389,    0,
    0,    0,    0,    0,    0, 1530,  940,    0,    0,    0,
    0,    0,    0,    0,    0, 2389, 2410,    0,  426,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  917,    0,
 8246,    0,    0,    0, 2627,    0, 8368,    0,    0,    0,
 8401,    0,    0,    0,    0, 8443,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2606,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2642,
    0,    0, 2651,    0,    0, 2653,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2413,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 8776,
    0,    0,    0,    0, 2334,  246, 1159,    0,  778,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2594,    0,    0,  194,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  171,    0,    0,    0,    0,    0, 8537,    0,    0,
    0,    0, 8612, 8701,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1906, 2680,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2663, 2665,    0,
    0,    0, 2668,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1544, 8734,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2594,  161,    0,  186,    0,    0,    0,    0,    0,    0,
 2656,    0,    0,    0, 2687,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  390,
    0,    0,    0,    0,    0,    0,    0, 8836,    0,    0,
    0, 1215,    0, 1329, 1582, 1621,    0,    0,    0,    0,
    0,    0,    0, 2671,  390, 2673,    0,    0,    0, 2432,
    0, 1586,    0,    0,    0, 2594,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,16619,    0,
    0,    0,  390,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  390,    0,    0,  390,    0,    0,    0,
    0,    0,    0,  390,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, 2735,    0,    0,
 2736, 2739,    0,    0,    0,    0,    0,    0,    1,    0,
 2740,   -2, 2580, 2161,   -3, -920, 1372, 2590, 1912, 2605,
 2185,    0,    0, 2456, -106,   34,    0,    0,    0,    0,
  228,  235,  -59,  -80,  -60, -290, 2459, 1943,    0,    0,
    0,  775,    0,    0,  -18, 1541, -635,  -71,-1091, 2042,
 1795, -376,    0, 1548,    0, 2695, 1796, 1838,    0,    0,
    0, -600,  -58,    0,    0,    0,    0,    0, 2712,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -66,    0, -616, -438, 1557, -395, 1227,-1117,
    0,    0,    0,    0, -407, -151, -367, 1564, -418,  -55,
    0, -622, 1834,    0, 1113,-1141,    0, 1115,    0,    0,
 2395, -767,    9,    0,    0,    0,    0, 2360,    0, 2117,
 1583,    0,    0,    0, 1642,    0, 2439, 1930, 1578, 2391,
 2393,    0,    0, -512, 1898,  -49, -763, 1004, -323,    0,
    0,    0,-1056,-1031,-1014, -993, -978,    0, 1153,-1156,
    0,  868,-1344, 1156,    0, 2158,    0,    0,    0,  865,
  871,  849,  -62, -302,-1050,-1061,    0,    0,    0, 1926,
    0, -762,    0, 1721,    0,    0,    0,    0,    0,-1307,
 -688,    0,    0,    0,    0,    0,    0, -264,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,-1051,    0,    0, -798,    0, -795, 1255, 1055,
    0,  893,    0,    0,    0,    0,    0,    0,    0, -479,
    0,    0, 2006, -154, -530, 1435, 1189, -623, -883,-1112,
-1418,    0,    0,  -42,    0,    0,    0,    0,    0, 2411,
    0,    0,    0,    0, 1904,    0, 2428, 2183,    0,    0,
    0,    0,    0,   -9, -563,    0,    0, 2440,    0,    0,
    0,  -10,    0,    0,    0, -431, 1895, -428, 1397,    0,
    0,    0,    0,    0,    0, -399,    0,    0,    0, 2136,
    0, 1914,    0,    0,    0,    0,    0,    0,   -7,    0,
 2848,    0, 2702,    0,    0,    0, 2511,    0,    0,    0,
    0, 2278,    0, 2271,    0,    0,    0, 2026,    0,    0,
    0, -653,    0, -237,    0,    0,    0,    0,    0, -480,
 -730, -969, 2496,    0,  862,    0,    0,    0,    0,    0,
    0, -382,    0,    0, -840, -258,    7,    0,    0,    0,
 2718, 1134,    0, 2028,-1182, -991,    0,-1028, 1778,-1326,
    0,    0,    0,    0, -170,    0,    0,    0,    0, 1514,
    0,    0,    0,    0, 2817,  238,    0, 1014, 2150,    0,
 1170,    0, 1399,    0, -236,    0,    0, 2030,    0,    0,
    0,    0,    0,    0, 2308,    0,    0,    0, -517,-1279,
 2081, 1806,-1249,    0,    0,    0,    0, -749, -505,  -95,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1395, 1644,    0,    0,    0, -963,
    0,    0,    0, 1520,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -784, 1602,    0,    0,
};
#define YYTABLESIZE 18002
static short yytable[] = {                                     154,
   40,   86,  239,  364,  266,  174,  173,  316,  181,  336,
  339,  889,  335,  338,  261,  718,  651,  277,  719,  282,
  262,  306, 1094, 1058,  688, 1095,  297,  303,  569, 1066,
  890,  682,  274,  863,  809,  808, 1150,  653,  301,  914,
 1349,  305,  307,  825, 1357,  846, 1033, 1564,  234,  237,
  257,  241,  244,  173,  249,  252,  274,  843, 1423,  245,
  263,  237,  252, 1307,  244, 1427,  173,  249, 1104,  281,
  154,  252,  275,  250, 1100, 1433, 1610,  589, 1080,  293,
  174,  173,  295,  237, 1670,  293,  252,  241,  244,  304,
  173,  249,  281,  154,  252,  242,  244, 1383,  372,  657,
  612, 1455, 1236,  783,  300,  809,  322,  270,  324,  367,
 1346,  995,  308, 1415,  783,  946,  986,   85, 1154,  813,
 1211,  660, 1451, 1472, 1626, 1148,  840, 1424,  972,  334,
  762,  302, 1241,   52, 1258,  334,   85,  166, 1416,  309,
  902,  760, 1175, 1217,  906, 1178,  263,  173,  263,  916,
  734,  320,  154,  323, 1185, 1417,  627, 1658,  762,  252,
  828,  828,  783,  813,  363,  876,  237,  244,  173,  281,
   95, 1706,  460,  181,  370,  374, 1418,  346,  813,  320,
 1467,  789,  744,  793,  608,  254,  333,  759,  833,  762,
 1371, 1419,   52,  828,  613,  676,  166,  872,  783,  762,
  760,  828,  159, 1636,  828,  954, 1420,  453, 1805,  334,
  368,   97, 1820,   53,  167, 1231,  417,  762,  365,  828,
  894, 1941,  789,  601,  793,  789,  254,  793,  759,  254,
  718,  877, 1155,  719,  828, 1026,  676,  828, 1094,  783,
  789,  744,  793,  608,  254,  756,  759,  448, 1000,  498,
  818,  337,  828,  363,  676,  819, 1438,  783, 1303, 1304,
  555,  678,  850,  563,  140,  619, 1350, 1769, 1770,  922,
 1668,  688,   53,  167,  334,  371,  838,  365,  809, 1660,
 1661, 1837, 1094, 1841, 1664,  969,  756,  819,  598,  756,
  883, 1247,  569, 1094,  677, 1332, 1095, 1094,  421, 1934,
  263, 1373,  678,  263,  756,  617,  266, 1610,  498,  571,
  154,  861,  363, 1271,  880,  611, 1794, 1977,  966,   87,
  678,  945,   87,  140,  852, 2011, 1680,  407,  631,  156,
  866, 1971,  823, 1083,  160,  677,  285,  241,  636,  340,
 1091,  173, 1081,  590, 1144, 1350,  819,  605,  616,   67,
 1649,  596, 1027,  677, 1077, 1256, 1707, 1067, 1637,  637,
 1145, 1272, 1662,  652, 1415, 1415, 1248,  796, 1694, 1415,
  286,  422,  106,  555,  603,  262, 2012,  668, 1026,  341,
  670, 1348,  729,  625,  625, 1273,  407,  631, 1401, 1416,
 1416,  732, 1439,  157, 1416,  255,  256,  636, 1218,  867,
 1324, 1411,  742,  263,  613, 1698, 1417, 1417,  796, 1421,
 1422, 1417, 1374,  263,  371,  263, 1842,  263,  637, 1400,
  673,  683, 1335,  685, 1194,  193,  796, 1418, 1418, 1026,
 1648, 1435, 1418, 1026,  166,   95,  828,  828,  738,  237,
  244,  249, 1419, 1419,  828,  266,  680, 1419,  608,  173,
 1790,  241,  583, 1978,  751,  740, 1237, 1420, 1420, 1631,
 1632,  748, 1420, 1882,   91,  150, 1899, 1253,  871,  968,
 1913, 1254, 1257,  762,  794, 1138,   97, 1764, 1940, 1240,
 1945,  166, 1188,  730,  193,  814, 1991,  680, 1489, 1490,
  815, 1491, 1492,  828,  762, 1027,  789, 1255,  793,  813,
 1958, 1168,  759, 1404, 1170,  680,  812,  608,  813, 1668,
  762,  167,  759,  783,  759,  794, 1210,  789, 1181,  793,
 1026,  760,   84,  759,  150,  611, 1210,  971,  839,  947,
  760,  676,  828,  794,  783,  319,  828, 1895,  762, 1544,
 1806,   84, 1546,  158, 1807, 1808, 1027, 1201,  332,  569,
 1027, 1557, 1026,  828, 1558,  173,  244,  571,  167,  828,
  756,  835,  744,  813,  173,  465, 1951,  759,  173,  828,
  842,  744,  756,  571,  847,  181,  759,  851,  563,   52,
 1835,  756, 1697,  657, 1834,  608,  783, 1801,  953, 1559,
  762,  407,  744,  828, 1122, 1382,  332,  678, 1377,  837,
  850, 1100, 2014,  183,  762,  884, 1466,  995,  140, 1979,
  948, 1149,  918, 1243,  498, 1795,  783, 1843,  263,  759,
  365, 1476,  895, 1403,  184,  756,  744,  789, 1827,  793,
  677, 1997,  813,  759,  756,  185,  903, 1027, 1378,  274,
 1026,  676, 1657,  762,  783, 1833,  166,  782,  819, 1156,
 1096,  911,  263,  828,  760,  363,  193,  911, 1001,   53,
 1610,  332,  852, 1008, 1672,  949,  631,  977,  637, 1027,
 1038,  762, 1779, 1101, 1229,  789,  636,  756,  254, 1174,
  713,   87,  796, 1906, 1094, 1610,  828, 1907,  782,  262,
 1026,  756,  955, 1891,  789,  744,  793,  637, 2015,  631,
  759, 1696, 1916,  796,  340,  613,  782,  678,  758, 1141,
  976,  783, 1874, 1610,  647,  193,  332,  980,  981, 1920,
 1883,  713, 1436,  167, 1610,  631,  332, 1610,  631,  263,
  637, 1930, 1931,  955, 1610,  636,  955,  511,  636,  713,
  677,  150,  150,  860,  631, 1847, 1484, 1428, 1429,  758,
  956,  955,  151,  944,  636, 1406,  637, 1027,  756,  637,
 1750,  929,  454,  930, 1753, 1917, 1715,  758,  686,  193,
  631, 1918,  252,  647, 1397,  637,  193,  757, 1485,  555,
  636,  555,  680, 1109, 1425,  955, 1426, 1497, 1496,  794,
 1040,  956,   14, 1141,  956,  137,  511,  431, 1434, 1477,
 1957,  637, 1780, 1959, 1085,  935, 1800, 1027, 1026,  956,
  794,  151,  487,  796,  263,  637, 1415, 1415,  757,  263,
  964,  263, 1437,  263, 1468, 1398,  611,  938,  106, 1671,
  571,  262, 1259,  638,  340,  193,  757,  150,  173,  244,
 1984, 1416, 1416,  956, 1120,  393, 1124,  244,  449,  608,
  186, 1061, 1062,  455, 1551, 1549,  238,  555, 1417, 1417,
  173, 1190,  173,  939, 1243,  263,  605,  884, 1147, 1500,
  432,  263, 1327, 1328, 1162,  601,  608,  625, 1799, 1418,
 1418, 1964,  837, 1966, 1191, 1172,  965, 1195,  340, 1684,
  274, 1970, 1692, 1972, 1419, 1419,  296,  263, 1775, 1200,
 1226, 1227, 1228, 1001,  393, 1635,  340, 1001, 1744, 1420,
 1420,  714,  936,  269,  193, 1173, 1215, 1501,  610,  487,
  794, 1096,  504,  391,  263, 1027,  193,  263, 1037,  274,
  274,  274, 1207, 1531,  450,  490,   84,  184, 2005,  152,
  185, 1047, 1047,  751,  715,  613,  294,  107, 1873, 1260,
 1208,  795,  714,  193, 1973,  714,  340,  561, 1163,  193,
  561,  193,  782,  940,  937,  137,  137, 1870,  173,  451,
  714,  487,  602, 1094, 1230, 1628, 1095,  610,  173,  490,
  687,  504,  391,  782, 1238,  715,  751, 1252,  715,  392,
 1063,  169,  795,  748,  490,  713,  184, 1880,  152,  185,
  193,  511, 2009,  715,  398,  713,  919,  713,  193, 1440,
  795, 1329,  419, 1497, 1497,  830,  713,  253,  829,  615,
  753, 1497, 1497,  758, 1497, 1497,  603, 1164,  379,  151,
  399,  612, 1497, 1497, 1497, 1497,  340, 1497, 1497, 1497,
 1497, 1497, 1497,  604,  758, 1441, 1679, 1760,  392,  614,
 1683,  894,  647,  818,  816,  239,  817,  252,  819,  288,
  713,  753,  227,  398,  463, 1141,  611,  737, 1611,  713,
  395, 1614,  468,  193,  904, 1336, 1700,  193,  615,  753,
 1047,  290,  814,  920,  394,  647,  340,  815,  758,  399,
  612,  170,  757,  782,  955,  239,  397,  758,  171,  718,
  571,  571,  719,  227,  846,  410,  227,  130,  614,  393,
  632,  647,  713,  757,  647,  904,  244,  298,  904,  601,
  193,  227,  396,  921,  713, 1944,  713,  511,  193,  395,
  647,  188, 1948,  904, 1094,  310, 1094, 1095,  884, 1095,
  758,  130,  864,  394, 1094,  846, 1094, 1095,  846, 1095,
  411,  312,  956, 1461,  758,  397,  647,  757,  752,  317,
  782,  352,  193,  846,  193,  193,  757, 1028, 1479,  632,
  414, 1896,  239,  415, 1483,  130,  255,  256,  601, 1096,
  504,  396,  274,  189,  262,  613,  193,  391,  563,  561,
 1029, 1094,  504,  713, 1095,  184, 1446,  274,  185,  752,
 1343,  818,  816,  274,  817,  313,  819,  184, 1447,  757,
  185,  193,  239, 1446,  254,  379,  152,  752, 1623, 1623,
 1627,  758,  561,  757,  263,  504,  714,  504,  263,  414,
  411, 1998,  415,  387,  184,  393,  714,  185,  714,  175,
  184,  176,  184,  185,  318,  185, 1030,  714, 1457,  368,
  749, 1967,  253,  392, 1654,  254, 1656,  412,  254,  715,
  634,   43,  635,  640,  561, 1884,  795,  500,  398,  715,
 1999,  715,  922,  254,  325,  413, 1989,  326,  176,  330,
  715,  184, 1615,  749,  185,  193, 1804,  795,  814,  411,
  757,  714,  387,  815,  399, 1524,  173, 1458,  175, 1560,
  714, 1613,  555, 1616, 2008,  262,  611, 1839,  368,  414,
  649, 1885, 1457,  391,  640, 2013, 1699, 2000, 2016,  561,
   43,  914, 1886,  650,  715, 2017,  500,  177,  248,  321,
  674, 1836, 1975,  715,  395,  753,  904,  176,  239,  227,
  675,  263,  263,  714,  165, 1565, 1609,  753,  394,  571,
  244,  249,  571,  281,  154,  714,  753,  714,  178,   47,
  397, 1460,  914, 1106, 1106,  914, 1106, 1106, 1106,  248,
  571,  571,  248,  179,  561,  399,  715,  244,  796,  392,
  914, 1106, 1106, 1106,  536,  366,  396,  248,  715,  904,
  715,  347,  904, 1612,  398,   88,   89,  795,  332,  904,
  753,  904,  365,  818,  816,  904,  817,  340,  819,  753,
  904,  824, 1106,  904,  425,  400,  613,  426,   47,  846,
  399, 1887,  846,  513,  714, 1140,  164,  165,  904,  846,
  142,  846,  395,  536,  239,  846,  814,  796, 1919,  892,
  846,  815,  904,  846,  366, 1682,  184, 1034,  632,  185,
  904,   14,  753,  904,  904,  239,  382,  715,  846,  427,
  395,  239, 1705,  904, 1388, 1792,  753, 1710,   41,  109,
  824,  340,  846,  752,  394,  760, 1342, 1389,  845,  904,
  846,  632,  513,  846,  846,  752,  397,  396,  340,  142,
  254,  254,  387,  846,  752,  239,  438,  387,  254, 1763,
 1390,  904,  387, 1281,  143,  111,  904,  632,  398,  846,
  632, 1042,  396, 1524,  173,  382,  760, 1282,  904,  402,
  904,  834,  332,  131,  833,  809,  632,   41, 1044,  214,
  439,  846,  440,  753,  760,  519,  846,  611,  752,  255,
  256,  818,  816,  807,  817,  833,  819,  752,  846, 1202,
  846, 1392,  632,  340,  441,  871,  237,  818,  816,  587,
  817, 1283,  819,  143, 1768, 1252, 1252,  904,  592,  418,
  112,  214,  833,  818,  816,  423,  817, 1609,  819,  442,
  113,  250,  131,  914,  809,  808,  254,  904,  214, 1284,
  752,  904, 1106, 1046,  435, 1067,  871,  846, 1106,  871,
  434, 1096,  807, 1106,  752,  114, 1106,  254,  348,  571,
  571,  446,  214,  254,  871,  214, 1740,  846,  457, 1898,
  251,  846,  250,  387,  631,  250,  363, 1640,  636, 1642,
  214,  459, 1623,  244,  244, 1812,  914,  773,  814,  914,
  250,  349, 1106,  815,  808, 1651,  914,  254,  914,  437,
 1106, 1741,  914, 1285,  464, 1652, 1813,  914, 1106,  173,
  914,  251, 1814, 1742,  251, 1818,  461,  350,  404, 1504,
  351,  752,  773,  465,  908,  914, 1106,  907, 1106,  251,
  405,  564, 1067, 1867,  773,  560,  352,  818,  816,  914,
  817, 1106,  819, 1106, 1106, 1106, 1815,  914,  387,  574,
  914,  914, 1067, 1079, 1203, 1204,  823,  254,  387, 1388,
  914, 1914,  353,  406,  566,  407,  818,  816,  575,  817,
  442,  819, 1389, 1986,  578, 1987,  914,  792, 1524,  173,
  818,  816,  241,  817, 1117,  819,  579, 1116,  563,  805,
  806,  807, 1106, 1096,  580, 1390,  818,  816,  914,  817,
  581,  819,  582,  914, 1067, 1096,  341,  584, 1096, 1391,
 1915, 1252, 1096, 1235, 1096,  914, 1234,  914, 1239,  613,
 1347,  991, 1096,  823, 1096,  599,  814,  597,  263, 1892,
  363,  815,  609,  244, 1353,  872,  538, 1352, 1402,  620,
  760, 1234,  814,  600,  613, 1449, 1392,  815, 1448, 1085,
  760,  623,  760, 1454,  173, 1952, 1453,  622,  814,  241,
 1912,  760,  852,  815,  914,  624,  854,  871, 1936, 1096,
 1556, 1922,  613, 1350,  628, 1924,  872,  538, 1630,  872,
  538,  823,  630,  613,  914, 1933,  613, 1101,  914, 1714,
  138,  138,  823,  613,  872,  538, 1783, 1785, 1323, 1101,
  342, 1990, 1101, 1784, 1786,  760,  241,  250,  250, 1755,
  642,   14,  823,   57,  760,  250,  818,  816,  644,  817,
  871,  819,  646,  871,   58,  237, 1938,  643,  666,  655,
  871,   59,  871,  669, 1939,   60,  871,  679,  680, 1791,
  611,  871,  823,  684,  871, 1995,  251,  251, 1994,  563,
  484,  485,  486, 1996,  251,   61,  833,  760, 1393,  871,
  735, 1035, 1126, 1039,  739,  611,  741, 1043, 1045, 1047,
  743,  760,  814,  871,  778,  776,  779,  815,  494,  495,
 1609,  871, 1963,  780,  871,  871, 1124,  818,  816,  263,
  817,  781,  819,  611,  871,  377,  781,  782,  377,  781,
  241,  814,  241,  250,  611, 1609,  815,  611,  783,  784,
  871,  785, 1555,  786,  611,  814,  787,   62,  790,  788,
  815,  789,  795,  796,  250,  797,  798,  799,  800,  898,
  250,  814,  871, 1609, 1124, 1007,  815,  871,  760, 1748,
  801,  802,  251,  803, 1609,  804, 1252, 1609,   63,  871,
  813,  871,   64,  820, 1609, 1036,  821,  823,  824, 1041,
  828,  831, 1725,  251,  250,  832, 1057,  849,  857,  251,
  898, 1064, 1065,  898,  859, 1068, 1069, 1070, 1071, 1072,
 1076,  870,  874,  879,  875,  150,  881,  882,  898,  519,
  886, 1086, 1087, 1088, 1089, 1090,  519,  872,  871, 1097,
  853,  818,  816,  251,  817,  893,  819,  887,  891,  637,
  896,  900,  538,  538,  897,  899,  909,  538,  871,  933,
  538,  904,  871,  942,  250,  934,  150,  951,  332,  150,
 1078,  818,  816,  538,  817,  592,  819,  950,  956,  285,
  957,   65,  958,  959,  150,  961,  962,  967,  973,  974,
  872,  814,  979,  872,  988, 1151,  815,  978,  990,  991,
  872,  997,  872,  251,  996,  998,  872,  631, 1161, 1002,
  188,  872, 1067,  286,  872, 1059, 1081,  968, 1105, 1123,
  818,  816,  561,  817,  334,  819, 1110, 1111, 1118,  872,
 1112, 1315,  818,  816, 1114,  817,  840,  819, 1157, 1152,
 1142, 1137,  833,  872, 1158, 1169, 1179, 1180,  538,  855,
 1182,  872, 1088,  912,  872,  872, 1186, 1189, 1187, 1197,
 1192, 1206,  814,  561,  872, 1198,  561,  815, 1199,  538,
 1213, 1223, 1225, 1232, 1246,  538, 1268, 1305, 1306, 1314,
  872,  561, 1312, 1322, 1337,  818,  816,  814,  817, 1350,
  819, 1354, 1063, 1088, 1088, 1088, 1088, 1088, 1088, 1088,
 1351, 1356,  872, 1372, 1375, 1380, 1381,  872, 1385,  538,
 1088, 1088, 1088, 1088, 1088,  818,  816, 1333,  817,  872,
  819,  872, 1387, 1395, 1407, 1410, 1432, 1261, 1262, 1266,
 1442,  898, 1275, 1277, 1279, 1430, 1287, 1290, 1293, 1295,
 1298, 1301, 1452, 1088, 1450, 1088, 1456, 1462, 1463, 1464,
 1308, 1309, 1465, 1311, 1240, 1313, 1473, 1481, 1316, 1317,
 1318, 1319, 1320, 1321,  818,  816, 1334,  817,  872,  819,
 1486,   14, 1487, 1488, 1494, 1325,  814, 1330, 1495, 1502,
 1499,  815, 1503, 1505,  898, 1506, 1513,  898,  872, 1514,
 1515, 1516,  872, 1345,  898, 1518,  898, 1519, 1520, 1521,
  898,  150,  150, 1522, 1523,  898,  814, 1026,  898,  150,
 1548,  815, 1619, 1545,  115, 1617, 1620,  116, 1633,  117,
  118,  119,  120,  898,  121, 1338,  818,  816, 1634,  817,
  855,  819,  122,  123,  519, 1638, 1643,  898, 1646, 1644,
 1645, 1655,  661,  124,  125,  898,  126, 1669,  898,  898,
 1673, 1674, 1675,  127,  128,  814,  129,  130,  898,  131,
  815, 1676,  132,  133,  134,  135,  136,  814, 1677, 1678,
  137,  138,  815, 1681,  898, 1685,  139,  140,  141, 1210,
  142,  143,  144, 1695,  145, 1701, 1703,  741, 1704, 1339,
  818,  816, 1709,  817,  146,  819,  898,  150,  561,  561,
 1276,  898, 1280,  561, 1288, 1291,  561, 1296, 1299, 1302,
 1716, 1718, 1738,  898, 1088,  898, 1088, 1088,  150,  561,
  814, 1739, 1745, 1088,  150,  815, 1751, 1761,  741, 1088,
 1762,  741, 1765, 1752, 1088,  157, 1772, 1088, 1773, 1340,
  818,  816, 1778,  817, 1781,  819,  741, 1782, 1787, 1788,
  814, 1088, 1793, 1809, 1822,  815, 1088, 1088,  150, 1819,
 1088, 1821,  898, 1824, 1823, 1825, 1828, 1088, 1826, 1088,
 1088, 1829, 1830, 1088, 1838, 1840, 1844, 1088, 1845, 1088,
 1846, 1088,  898, 1088,  519, 1849,  898, 1850, 1088, 1088,
 1851, 1088, 1852,  528,  561, 1855, 1856, 1507, 1508,  814,
 1510, 1512, 1857, 1858,  815, 1859, 1088, 1088, 1517, 1088,
 1860, 1861, 1862, 1863, 1864,  561, 1866, 1877, 1869, 1774,
 1088,  561, 1088, 1893, 1088, 1088, 1088, 1894, 1088, 1908,
 1535, 1088, 1088, 1905,  528, 1910, 1911,  528, 1542, 1543,
 1330, 1088, 1923, 1330, 1927, 1325, 1932, 1942, 1935, 1550,
  519, 1552,  528, 1943, 1088,  561, 1946, 1088, 1553, 1949,
 1554,  814, 1960, 1341,  818,  816,  815,  817, 1953,  819,
 1529,  818,  816, 1088,  817, 1961,  819, 1962, 1965, 1088,
 1088, 1980, 1985,  248, 1088, 1530,  818,  816, 1988,  817,
 1982,  819, 1993, 1977, 1978, 1088, 1088, 2003, 1088, 2006,
 2004,  519, 2007, 2010, 1088,    1,  789, 1088, 1061, 1088,
 1088, 1088, 1088,   29, 1088, 1533,  818,  816,  200,  817,
   31,  819, 1088, 1088,  248,  814,  828,  248,  268,  379,
  815,  163,  705, 1088, 1088,  768, 1088, 1046,   82,  210,
  955, 1663,  248, 1088, 1088, 1088, 1088, 1088,  279, 1088,
  767,  741, 1088, 1088, 1088, 1088, 1088,  796,  278,  190,
 1088, 1088,  749,  407,  751, 1088, 1088, 1088, 1088, 1088,
 1088, 1088, 1088,  191, 1088,  814,  769, 1534,  818,  816,
  815,  817, 1019,  819, 1088,  323,  380, 1536,  818,  816,
  220,  817, 1067,  819, 1537,  818,  816, 1045,  817,  321,
  819,  331,  741,  818,  816,  741,  817,  295,  819, 1711,
 1018,  322,  741,  324,  741, 1717,  332,  778,  741,  327,
 1721,  360,  345,  741,   29,   32,  741, 1726,   33,   42,
  327,  848, 1621, 1067, 1067, 1067, 1067, 1067, 1081, 1067,
  328,  741, 1048, 1049, 1050, 1051, 1052, 1053, 1054, 1055,
 1121, 1067, 1067, 1067, 1067,  741,  329,  850, 1538,  818,
  816, 1330,  817,  741,  819, 1330,  573,  741,  572,  528,
  528, 1759, 1113, 1482,  989, 1245,  741,  528, 1480,  299,
 1209, 1250,  284, 1067, 1478, 1067, 1470, 1766, 1831, 1219,
  528, 1832,  741,  692,  648, 1459,  952, 1409,  618,  814,
 1153, 1471, 1909, 1184,  815,  663,  814,  664, 1811, 1976,
 1983,  815, 1388, 1817,  928, 1981, 2002, 1358, 1767,  741,
 1890,  814, 1177, 1968, 1102, 1389,  815, 1650, 1802, 1193,
  671,  741,  654,  741, 1539,  818,  816,  645,  817,  741,
  819,  932,  741, 1693,  741,  741,  741,  741, 1390,  741,
  163,  814,  975, 1224,  331,  610,  815,  741,  741,  248,
  248, 1796, 1391,  869, 1216,  528,  865,  248,  741,  741,
 1900,  741,  818,  816,  345,  817, 1139,  819,  741,  741,
  741,  741,  741,  561,  741, 1143,  528,  741,  741,  741,
  741,  741,  528, 1384, 1641,  741,  741,  294, 1865, 1392,
  741,  741,  741,  741,  741,  741,  741,  741, 1950,  741,
 1160,  878, 1737,  814, 1379, 1031, 1509, 1119,  815,  741,
 1547, 1659,    0,  814,  561, 1310,  528,  561,  815,    0,
  814,  716,    0,    0, 1848,  815,    0,    0,  814,    0,
 1853, 1854,  561,  815,  716,    0,    0,    0,    0,    0,
    0,    0,  716,    0, 1067,  248, 1067, 1067,    0,    0,
    0,    0,    0, 1067, 1868,    0,    0,  716, 1330, 1067,
    0,    0, 1871,    0, 1067,    0,  248, 1067,    0, 1878,
 1879,  716,  248,  559, 1540,  818,  816, 1901,  817,    0,
  819, 1067, 1098, 1067,  814,    0, 1067, 1067,    0,  815,
 1067,    0, 1541,  818,  816,    0,  817, 1067,  819, 1067,
 1067, 1393,    0, 1067,    0,    0,  248, 1067,  716, 1067,
    0, 1067,    0, 1067,  559,    0,    0,  559, 1067, 1067,
    0, 1067,    0, 1098, 1098, 1098, 1098, 1098, 1098, 1098,
    0,    0,  559,    0,  559,    0, 1067, 1067,    0, 1067,
    0, 1098, 1098, 1098, 1098, 1921,    0, 1746,  818,  816,
 1067,  817, 1067,  819, 1067, 1067, 1067,    0, 1067,    0,
  814, 1067, 1067,    0,    0,  815,    0,    0,    0,    0,
    0, 1067,    0, 1098,    0, 1098,    0,  108,    0, 1747,
  818,  816, 1493,  817, 1067,  819,    0, 1067,  228,    0,
    0,    0,    0,    0, 1754,  818,  816,  814,  817,    0,
  819,    0,  815, 1067,    0,    0,    0,    0,    0, 1067,
 1067, 1796,    0,    0, 1067,  109,    0,    0,    0,    0,
  716,    0,    0,    0,    0, 1067, 1067,  110, 1067,  228,
    0,    0,  228,    0, 1067,    0,    0, 1067,    0, 1067,
 1067, 1067, 1067,    0, 1067,    0,    0,  228, 1969,  561,
  561,  111, 1067, 1067, 1757,  818,  816,  561,  817,    0,
  819,    0,    0, 1067, 1067,    0, 1067,    0,    0,    0,
  561,    0,    0, 1067, 1067, 1067, 1067, 1067,    0, 1067,
    0,    0, 1067, 1067, 1067, 1067, 1067,    0,    0,    0,
 1067, 1067,    0,    0,    0, 1067, 1067, 1067, 1067, 1067,
 1067, 1067, 1067,    0, 1067,    0,    0, 1758,  818,  816,
  814,  817,    0,  819, 1067,  815,  112,    0, 1872,  818,
  816,    0,  817,    0,  819,    0,  113,    0,  814,    0,
    0,  559,    0,  815,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1098,  561, 1098, 1098,  559,  559,
  559,  114,    0, 1098,  559,    0,    0,  559,    0, 1098,
    0,    0,    0,    0, 1098,    0,  561, 1098,    0,    0,
  559,    0,  561,  556,    0,    0,  559,    0,    0,    0,
    0, 1098, 1103,  814,    0,    0, 1098, 1098,  815,    0,
 1098,    0, 1929,  818,  816,    0,  817, 1098,  819, 1098,
 1098,    0,    0, 1098,    0,    0,  561, 1098,    0, 1098,
    0, 1098,    0, 1098,  556,  814,    0,  556, 1098, 1098,
  815, 1098,    0, 1103, 1103, 1103, 1103, 1103, 1103, 1103,
  814,    0,  556,    0,  556,  815, 1098, 1098,    0, 1098,
    0, 1103, 1103, 1103, 1103,  559,    0,    0,  818,  816,
 1098,  817, 1098,  819, 1098, 1098, 1098,    0, 1098,    0,
    0, 1098, 1098,    0,  239,  228,  559,    0,    0,    0,
    0, 1098,  559, 1103,   69, 1103,    0,    0,    0,    0,
    0,  559,    0,  559, 1098,   70,    0, 1098,    0,    0,
  814,    0,   71,    0,    0,  815,    0,    0,    0,    0,
    0,  559,    0, 1098,    0,   72,  559,    0,    0, 1098,
 1098,    0,    0,    0, 1098,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1098, 1098,    0, 1098,    0,
    0,    0,    0,    0, 1098,  559,    0, 1098,    0, 1098,
 1098, 1098, 1098,  814, 1098,    0,    0,    0,  815,    0,
    0,    0, 1098, 1098,  814,    0,    0,    0,    0,  815,
  239,    0,   73, 1098, 1098,    0, 1098,  818,  816, 1756,
  817,    0,  819, 1098, 1098, 1098, 1098, 1098,   74, 1098,
    0,  239, 1098, 1098, 1098, 1098, 1098,  239,    0,    0,
 1098, 1098,    0,    0,   75, 1098, 1098, 1098, 1098, 1098,
 1098, 1098, 1098,    0, 1098,    0,    0,    0,    0,   76,
    0,    0,    0,   77, 1098,    0,   78,    0,    0,    0,
    0,  239,    0,    0,   79,    0,    0,    0,  814,    0,
    0,  556,    0,  815,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   80, 1103,    0, 1103, 1103,  556,  556,
  556,    0,    0, 1103,  556,    0,    0,  556,    0, 1103,
    0,    0,    0,    0, 1103,    0,    0, 1103, 1724,    0,
  556,    0,    0,  561,    0,    0,  556,    0,    0,    0,
    0, 1103, 1099,  814,    0,    0, 1103, 1103,  815,    0,
 1103,    0,    0,    0,    0,    0,    0, 1103,    0, 1103,
 1103,    0,   81, 1103,    0,    0,    0, 1103,    0, 1103,
    0, 1103,    0, 1103,  561,    0,    0,  561, 1103, 1103,
    0, 1103,    0, 1099, 1099, 1099, 1099, 1099, 1099, 1099,
    0,    0,  561,    0,  561,    0, 1103, 1103,    0, 1103,
    0, 1099, 1099, 1099, 1099,  556,    0,    0,    0,    0,
 1103,    0, 1103,    0, 1103, 1103, 1103,    0, 1103,    0,
    0, 1103, 1103,    0,    0,    0,  556,    0,    0,    0,
    0, 1103,  556, 1099,    0, 1099,    0,    0,    0,    0,
    0,    0,    0,  556, 1103,   92,    0, 1103,    0,    0,
    0,    0,   93,    0,    0,    0,   94,   95,    0,    0,
    0,  556,  814, 1103,    0,   96,  556,  815,    0, 1103,
 1103,    0,    0,    0, 1103,    0,   97,    0,    0,    0,
    0,    0,    0,    0,    0, 1103, 1103,    0, 1103,    0,
    0,    0,    0,    0, 1103,  556,    0, 1103,    0, 1103,
 1103, 1103, 1103,    0, 1103,    0,    0,    0,    0,    0,
    0,    0, 1103, 1103,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1103, 1103,    0, 1103,    0,    0,    0,
    0,    0,    0, 1103, 1103, 1103, 1103, 1103,   98, 1103,
    0,    0, 1103, 1103, 1103, 1103, 1103,    0,    0,    0,
 1103, 1103,    0,    0,   99, 1103, 1103, 1103, 1103, 1103,
 1103, 1103, 1103,    0, 1103,    0,    0,    0,    0,  100,
    0,    0,    0,  101, 1103,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  102,    0,    0,    0,    0,    0,
    0,  561,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  103, 1099,    0, 1099, 1099,    0,  561,
  561,    0,    0, 1099,  561,    0,    0,  561,    0, 1099,
    0,    0,    0,    0, 1099,    0,    0, 1099,    0,    0,
  561,    0,    0,    0,    0,    0,  561,    0,    0,    0,
    0, 1099, 1112,    0,    0,    0, 1099, 1099,    0,    0,
 1099,    0,    0,    0,    0,    0,    0, 1099,    0, 1099,
 1099,    0,  104, 1099,    0,    0,    0, 1099,    0, 1099,
    0, 1099,    0, 1099,    0,    0,    0,    0, 1099, 1099,
    0, 1099,    0, 1112, 1112, 1112, 1112, 1112, 1112, 1112,
    0,    0,    0,    0,    0,    0, 1099, 1099,    0, 1099,
    0, 1112, 1112, 1112, 1112,  561,    0,    0,    0,    0,
 1099,    0, 1099,    0, 1099, 1099, 1099,    0, 1099,    0,
    0, 1099, 1099,    0,    0,    0,  561,    0,    0, 1712,
 1713, 1099,  561, 1112,    0, 1112,    0, 1719, 1720,    0,
 1722, 1723,    0,  561, 1099,    0,    0, 1099, 1727, 1728,
 1729, 1730,    0, 1731, 1732, 1733, 1734, 1735, 1736,    0,
    0,  561,    0, 1099,    0,    0,  561,    0,    0, 1099,
 1099,    0,    0,    0, 1099,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1099, 1099,    0, 1099,    0,
    0,    0,    0,    0, 1099,  561,    0, 1099,    0, 1099,
 1099, 1099, 1099,    0, 1099,    0,    0,    0,    0,    0,
    0,    0, 1099, 1099,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1099, 1099,    0, 1099,    0,    0,    0,
    0,    0,    0, 1099, 1099, 1099, 1099, 1099,    0, 1099,
    0,    0, 1099, 1099, 1099, 1099, 1099,    0,    0,    0,
 1099, 1099,    0,    0,    0, 1099, 1099, 1099, 1099, 1099,
 1099, 1099, 1099,    0, 1099,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1099,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  561,    0,    0,    0,    0,
    0,    0,    0,    0, 1112,    0, 1112, 1112,    0,    0,
    0,    0,    0, 1112,    0,    0,    0,    0,    0, 1112,
    0,    0,    0,    0, 1112,    0,    0, 1112,    0,    0,
    0,    0,    0,    0,    0,    0,  561,    0,    0,  561,
    0, 1112, 1117,    0,    0,    0, 1112, 1112,    0,    0,
 1112,    0,    0,    0,  561,    0,  561, 1112,    0, 1112,
 1112,    0,    0, 1112,    0,    0,    0, 1112,    0, 1112,
    0, 1112,    0, 1112,    0,    0,    0,    0, 1112, 1112,
    0, 1112,    0, 1117, 1117, 1117, 1117, 1117, 1117, 1117,
    0,    0,    0,    0,    0,    0, 1112, 1112,    0, 1112,
    0, 1117, 1117, 1117, 1117,    0,    0,    0,    0,    0,
 1112,    0, 1112,    0, 1112, 1112, 1112,    0, 1112,    0,
    0, 1112, 1112,    0,    0,    0,    0,    0,    0,    0,
    0, 1112,    0, 1117,    0, 1117,    0,    0,    0,    0,
    0,    0,    0,    0, 1112,    0,    0, 1112,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1112,    0,    0,    0,    0,    0, 1112,
 1112,    0,    0,    0, 1112,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1112, 1112,    0, 1112,    0,
    0,    0,    0,    0, 1112,    0,    0, 1112,    0, 1112,
 1112, 1112, 1112,    0, 1112,    0,    0,    0,    0,    0,
    0,    0, 1112, 1112,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1112, 1112,    0, 1112,    0,    0,    0,
    0,    0,    0, 1112, 1112, 1112, 1112, 1112,    0, 1112,
    0,    0, 1112, 1112, 1112, 1112, 1112,    0,    0,    0,
 1112, 1112,    0,  561,    0, 1112, 1112, 1112, 1112, 1112,
 1112, 1112, 1112,    0, 1112,    0,    0,    0,  561,    0,
    0,    0,    0,    0, 1112,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  561,    0,    0,    0,    0,    0,  561,    0,
    0,  561,    0,    0, 1117,    0, 1117, 1117,    0,    0,
    0,    0,    0, 1117,    0,    0,    0,    0,    0, 1117,
    0,    0,    0,    0, 1117,    0,    0, 1117,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1117,  923,  561,    0,    0, 1117, 1117,    0,    0,
 1117,    0,    0,    0,    0,    0,    0, 1117,    0, 1117,
 1117,    0,    0, 1117,    0,    0,    0, 1117,    0, 1117,
    0, 1117,    0, 1117,    0,    0,    0,    0, 1117, 1117,
    0, 1117,    0,  923,  923,  923,  923,  923,    0,  923,
    0,    0,    0,    0,    0,    0, 1117, 1117,  561, 1117,
    0,  923,  923,  923,  923,  561,    0,    0,    0,    0,
 1117,    0, 1117,    0, 1117, 1117, 1117,    0, 1117,    0,
    0, 1117, 1117,  561,    0,    0,    0,    0,    0,    0,
    0, 1117,    0,    0,    0,  923,    0,    0,    0,    0,
    0,    0,    0,    0, 1117,    0,    0, 1117,    0,    0,
    0,    0,    0,  561,    0,    0,    0,  561,    0,    0,
    0,    0,    0, 1117,    0,    0,    0,    0,    0, 1117,
 1117,    0,    0,    0, 1117,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1117, 1117,    0, 1117,    0,
    0,    0,    0,    0, 1117,    0,    0, 1117,    0, 1117,
 1117, 1117, 1117,    0, 1117,    0,    0,    0,    0,    0,
    0,    0, 1117, 1117,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1117, 1117,    0, 1117,    0,    0,    0,
    0,    0,    0, 1117, 1117, 1117, 1117, 1117,    0, 1117,
    0,    0, 1117, 1117, 1117, 1117, 1117,    0,    0,    0,
 1117, 1117,    0,    0,    0, 1117, 1117, 1117, 1117, 1117,
 1117, 1117, 1117,    0, 1117,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1117,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  923,    0,  923,  923,    0,    0,
    0,    0,    0,  923,    0,    0,    0,    0,    0,  923,
    0,    0,    0,    0,  923,    0,    0,  923,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  923,  936,    0,    0,    0,  923,  923,    0,    0,
  923,    0,    0,    0,    0,    0,    0,  923,    0,  923,
  923,    0,    0,  923,    0,    0,    0,  923,    0,  923,
    0,  923,    0,  923,    0,    0,    0,    0,  923,  923,
    0,  923,    0,  936,  936,  936,  936,  936,    0,  936,
    0,    0,    0,    0,    0,    0,  923,  923,    0,  923,
    0,  936,  936,  936,  936,    0,    0,    0,    0,    0,
  923,    0,  923,    0,  923,  923,  923,    0,  923,    0,
    0,  923,  923,    0,    0,    0,    0,    0,    0,    0,
    0,  923,    0,    0,    0,  936,    0,    0,    0,    0,
    0,    0,    0,    0,  923,    0,    0,  923,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  923,    0,    0,    0,    0,    0,  923,
  923,    0,    0,    0,  923,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  923,  923,    0,  923, 1412,
    0,    0,    0,    0,  923,    0,    0,  923,    0,  923,
  923,  923,  923,    0,  923,    0,    0,    0,    0,    0,
    0,    0,  923,  923,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  923,  923,    0,  923,    0,    0,    0,
    0,    0,    0,  923,  923,  923,  923,  923,    0,  923,
    0,    0,  923,  923,  923,  923,  923,    0,    0,    0,
  923,  923,    0,    0,    0,  923,  923,  923,  923,  923,
  923,  923,  923,    0,  923,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  923,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,    0,  936,  936,    0,    0,
    0,    0,    0,  936,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,    0,    0,  936,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  936,  939,    0,    0,    0,  936,  936,    0,    0,
  936,    0,    0,    0,    0,    0,    0,  936,    0,  936,
  936,    0,    0,  936,    0,    0,    0,  936,    0,  936,
    0,  936,    0,  936,    0,    0,    0,    0,  936,  936,
    0,  936,    0,  939,  939,  939,  939,  939,    0,  939,
    0,    0,    0,    0,    0,    0,  936,  936,    0,  936,
    0,  939,  939,  939,  939,    0,    0,    0,    0,    0,
  936,    0,  936,    0,  936,  936,  936,    0,  936,    0,
    0,  936,  936,    0,    0,    0,    0,    0,    0,    0,
    0,  936,    0,    0,    0,  939,  472,    0,    0,    0,
    0,    0,    0,    0,  936,    0,    0,  936,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,  474,    0,    0,    0,    0,  936,
  936,    0,    0,    0,  936,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  936,  936,    0,  936,    0,
    0,    0,  476,    0,  936,    0,    0,  936,    0,  936,
  936,  936,  936,    0,  936,    0,    0,    0,    0,    0,
    0,    0,  936,  936,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,  936,    0,  936,    0,    0,    0,
    0,    0,    0,  936,  936,  936,  936,  936,    0,  936,
    0,    0,  936,  936,  936,  936,  936,    0,    0,    0,
  936,  936,    0,    0,    0,  936,  936,  936,  936,  936,
  936,  936,  936,  481,  936,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,    0,  255,  256,    0,
    0,    0,    0,    0,  939,    0,  939,  939,    0,    0,
    0,    0,    0,  939,    0,    0,  489,  490,  491,  492,
  493,  494,  495,  496,  497,    0,    0,  939,    0,    0,
    0,  499,  500,    0,    0,    0,    0,    0,    0,    0,
  504,  939,  940,    0,    0,    0,  939,  939,    0,    0,
  939,    0,    0,    0,    0,    0,    0,  939,    0,  939,
  939,    0,    0,  939,    0,    0,    0,  939,    0,  939,
    0,  939,    0,  939,    0,    0,    0,    0,  939,  939,
    0,  939,    0,  940,  940,  940,  940,  940,    0,  940,
    0,    0,    0,    0,    0,    0,  939,  939,    0,  939,
    0,  940,  940,  940,  940,    0,    0,    0,    0,    0,
  939,    0,  939,    0,  939,  939,  939,    0,  939,    0,
    0,  939,  939,    0,    0,    0,    0,    0,    0,    0,
    0,  939,    0,    0,    0,  940,    0,    0,    0,    0,
    0,    0,    0,    0,  939,    0,    0,  939,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  939,    0,    0,    0,    0,    0,  939,
  939,    0,    0,    0,  939,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  939,  939,    0,  939,    0,
    0,    0,    0,    0,  939,    0,    0,  939,    0,  939,
  939,  939,  939,    0,  939,    0,    0,    0,    0,    0,
    0,    0,  939,  939,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  939,  939,    0,  939,    0,    0,    0,
    0,    0,    0,  939,  939,  939,  939,  939,    0,  939,
    0,    0,  939,  939,  939,  939,  939,    0,    0,    0,
  939,  939,    0,    0,    0,  939,  939,  939,  939,  939,
  939,  939,  939,    0,  939,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  939,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  940,    0,  940,  940,    0,    0,
    0,    0,    0,  940,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  940,    0,    0,
    0,  934,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  940,    0,    0,    0,    0,  940,  940,    0,    0,
  940,    0,    0,    0,    0,    0,    0,  940,    0,  940,
  940,    0,    0,  940,    0,    0,    0,  940,    0,  940,
    0,  940,  934,  940,  934,  934,  934,    0,  940,  940,
    0,  940,  992,    0,    0,  694,    0,    0,    0,    0,
  934,  934,  934,  934,    0,    0,  940,  940,  993,  940,
    0,    0,  695,    0,    0,  696,    0,    0,    0,    0,
  940,    0,  940,    0,  940,  940,  940,    0,  940,    0,
    0,  940,  940,    0,  934,    0,    0,    0,    0,    0,
    0,  940,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  940,  697,    0,  940,    0,    0,
    0,  698,  699,  700,    0,  701,  702,  703,    0,  704,
    0,    0,    0,  940,    0,    0,    0,    0,    0,  940,
  940,    0,    0,    0,  940,    0,    0,  705,    0,  706,
    0,    0,    0,    0,    0,  940,  940,    0,  940,    0,
    0,    0,    0,    0,  940,    0,    0,  940,    0,  940,
  940,  940,  940,    0,  940,    0,    0,    0,  707,    0,
    0,    0,  940,  940,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  940,  940,  708,  940,    0,    0,    0,
    0,    0,    0,  940,  940,  940,  940,  940,    0,  940,
    0,    0,  940,  940,  940,  940,  940,  709,    0,    0,
  940,  940,    0,    0,    0,  940,  940,  940,  940,  940,
  940,  940,  940,    0,  940,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  940,    0,    0,    0,    0,    0,
    0,    0,  710,  711,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  934,    0,  934,  934,    0,    0,  712,
    0,    0,  934,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  934,    0,    0,    0,
  933,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  934,    0,    0,    0,    0,  934,  934,    0,    0,  934,
    0,    0,    0,    0,    0,    0,  934,    0,  934,  934,
    0,    0,  934,    0,    0,    0,  934,    0,  934,    0,
  934,  933,  934,  933,  933,  933,    0,  934,  934,    0,
  934,  218,    0,    0,  218,    0,    0,    0,    0,  933,
  933,  933,  933,    0,    0,  934,  934,  218,  934,    0,
    0,  218,    0,    0,  218,    0,    0,    0,    0,  934,
    0,  934,    0,  934,  934,  934,    0,  934,    0,    0,
  934,  934,    0,  933,    0,    0,    0,    0,    0,    0,
  934,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  934,  218,    0,  934,    0,    0,    0,
  218,  218,  218,    0,  218,  218,  218,    0,  218,    0,
    0,    0,  934,    0,    0,    0,    0,    0,  934,  934,
    0,    0,    0,  934,    0,    0,  218,    0,  218,    0,
    0,    0,    0,    0,  934,  934,    0,  934,    0,    0,
    0,    0,    0,  934,    0,    0,  934,    0,  934,  934,
  934,  934,    0,  934,    0,    0,    0,  218,    0,    0,
    0,  934,  934,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  934,  934,  218,  934,    0,    0,    0,    0,
    0,    0,  934,  934,  934,  934,  934,    0,  934,    0,
    0,  934,  934,  934,  934,  934,  218,    0,    0,  934,
  934,    0,    0,    0,  934,  934,  934,  934,  934,  934,
  934,  934,    0,  934,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  934,    0,    0,    0,    0,    0,    0,
    0,  218,  218,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  933,    0,  933,  933,    0,    0,  218,    0,
    0,  933,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  933,    0,    0,    0,  935,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  933,
    0,    0,    0,    0,  933,  933,    0,    0,  933,    0,
    0,    0,    0,    0,    0,  933,    0,  933,  933,    0,
    0,  933,    0,    0,    0,  933,    0,  933,    0,  933,
  935,  933,  935,  935,  935,    0,  933,  933,    0,  933,
    0,    0,    0,    0,    0,    0,    0,    0,  935,  935,
  935,  935,    0,    0,  933,  933,    0,  933,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  933,    0,
  933,    0,  933,  933,  933,    0,  933,    0,    0,  933,
  933,    0,  935,    0,    0,    0,    0,    0,    0,  933,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  933,    0,    0,  933,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  933,    0,    0,    0,    0,    0,  933,  933,    0,
    0,    0,  933,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  933,  933,    0,  933,    0,    0,    0,
    0,    0,  933,    0,    0,  933,    0,  933,  933,  933,
  933,    0,  933,    0,    0,    0,    0,    0,    0,    0,
  933,  933,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  933,  933,    0,  933,    0,    0,    0,    0,    0,
    0,  933,  933,  933,  933,  933,    0,  933,    0,  875,
  933,  933,  933,  933,  933,    0,    0,    0,  933,  933,
    0,    0,    0,  933,  933,  933,  933,  933,  933,  933,
  933,    0,  933,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  933,    0,    0,    0,    0,    0,    0,    0,
  875,    0,    0,  875,    0,    0,    0,    0,    0,    0,
    0,  935,    0,  935,  935,    0,    0,    0,  875,    0,
  935,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  935,    0,    0,    0,  938,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  935,    0,
    0,    0,    0,  935,  935,    0,    0,  935,    0,    0,
    0,    0,    0,    0,  935,    0,  935,  935,    0,    0,
  935,    0,    0,    0,  935,    0,  935,    0,  935,  938,
  935,  938,  938,  938,    0,  935,  935,    0,  935,    0,
    0,    0,    0,    0,    0,    0,    0,  938,  938,  938,
  938,    0,    0,  935,  935,    0,  935,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  935,    0,  935,
    0,  935,  935,  935,    0,  935,    0,    0,  935,  935,
    0,  938,    0,    0,    0,    0,    0,    0,  935,    0,
    0,    0,    0,    0,    0,    0, 1078,  818,  816,    0,
  817,  935,  819,    0,  935,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1024, 1023, 1025,    0,    0,
  935,    0,    0,    0,    0,    0,  935,  935,    0,    0,
 1092,  935,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  935,  935,    0,  935,    0,    0,  516,    0,
    0,  935,    0,    0,  935,    0,  935,  935,  935,  935,
    0,  935,    0,    0,    0,    0,    0,    0,    0,  935,
  935,  875,    0,    0,    0,    0,    0,    0,    0,    0,
  935,  935,    0,  935,    0,    0,    0,    0,    0,    0,
  935,  935,  935,  935,  935,    0,  935,  712,    0,  935,
  935,  935,  935,  935,    0,    0,    0,  935,  935,    0,
    0,    0,  935,  935,  935,  935,  935,  935,  935,  935,
    0,  935,    0,    0,  875,    0,    0,  875,    0,    0,
    0,  935,    0,    0,  875,    0,  875,  712,  712,    0,
  875,  712,    0,    0,    0,  875,    0,    0,  875,    0,
  938,    0,  938,  938,    0,    0,  712,    0,    0,  938,
    0,    0,    0,  875,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  938,    0,    0,    0,  875,    0,    0,
    0,    0,    0,    0,    0,  875,    0,  938,  875,  875,
    0,    0,  938,  938,    0,    0,  938,    0,  875,    0,
    0,    0,    0,  938,    0,  938,  938,    0,    0,  938,
    0,    0,    0,  938,  875,  938,    0,  938,    0,  938,
    0,    0,    0,    0,  938,  938, 1011,  938,    0,    0,
    0,    0,  814,    0,    0,    0,  875,  815,    0,    0,
 1012,  875,  938,  938,    0,  938,    0,    0,    0,    0,
    0,    0,    0,  875,    0,  875,  938,    0,  938,    0,
  938,  938,  938,    0,  938,    0,    0,  938,  938,    0,
    0,    0,    0,    0,    0,    0, 1013,  938,    0,    0,
    0,    0,    0,    0, 1014,    0,    0,    0,    0,    0,
  938,    0, 1015,  938,    0,    0,    0,    0,    0,    0,
    0,    0,  875,    0,    0,    0,    0,    0,  739,  938,
 1016,    0, 1017,    0,    0,  938,  938,    0,    0,    0,
  938,    0,  875,    0,    0, 1018,  875, 1019, 1020, 1021,
    0,  938,  938,    0,  938,    0,    0,    0,    0,    0,
  938,    0,    0,  938,    0,  938,  938,  938,  938,  739,
  938,    0,  739,    0,    0,    0,    0,    0,  938,  938,
    0,    0,    0,    0,    0,    0,    0,  739,    0,  938,
  938,    0,  938,    0,    0,    0, 1022,    0,    0,  938,
  938,  938,  938,  938,    0,  938,    0,    0,  938,  938,
  938,  938,  938,    0,    0,    0,  938,  938,    0,    0,
    0,  938,  938,  938,  938,  938,  938,  938,  938,    0,
  938,    0,  712,    0,    0,  712,    0,    0,    0,    0,
  938,    0,  712,    0,  712,    0,    0,    0,  712,    0,
    0,    0,    0,  712,    0,    0,  712,    0,    0,    0,
    0,    0,  115,    0,    0,  116,    0,  117,  118,  119,
  120,  712,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,    0,    0,    0,  712,    0,    0,    0,    0,
    0,  124,  125,  712,  126,    0,    0,  712,    0,    0,
    0,  127,  128,    0,  129,  130,  712,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,  712,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  146,    0,    0,    0,    0,    0,    0,  712,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  712,    0,  712,    0,    0,    0,    0,    0,  712,
    0,    0,  712,    0,  712,  712,  712,  712,    0,  712,
    0,    0,    0,    0,    0,    0,    0,  712,  712,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  712,  712,
    0,  712,    0,    0,    0,    0,    0,    0,  712,  712,
  712,  712,  712,    0,  712,    0,    0,  712,  712,  712,
  712,  712,    0,    0,    0,  712,  712,  915,    0,    0,
  712,  712,  712,  712,  712,  712,  712,  712,    0,  712,
    0,    0,    0,  739,    0,    0,  739,    0,    0,  712,
    0,    0,    0,  739,    0,  739,    0,    0,    0,  739,
    0,    0,    0,    0,  739,    0,    0,  739,  915, 1107,
 1107,  915, 1107, 1107, 1107,    0,    0,    0,    0,    0,
    0,    0,  739,    0,    0,    0,  915, 1107, 1107, 1107,
    0,    0,    0,    0,    0,    0,  739,    0,    0,    0,
    0,    0,    0,    0,  739,    0,  916,    0,  739,    0,
    0,    0,    0,    0,    0,    0,    0,  739, 1107,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  739,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  916, 1108, 1108,
  916, 1108, 1108, 1108,    0,    0,    0,    0,    0,    0,
  739,    0,    0,    0,    0,  916, 1108, 1108, 1108,    0,
    0,    0,  739,    0,  739,    0,    0,    0,    0,    0,
  712,    0,    0,  712,    0,  712,  712,  712,  712,    0,
  712,    0,    0,    0,    0,    0,    0, 1108,  712,  712,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  712,
  712,    0,  712,    0,    0,    0,    0,    0,    0,  712,
  712,  739,  712,  712,    0,  712,    0,    0,  712,  712,
  712,  712,  712,    0,    0,    0,  712,  712,  873,    0,
    0,  739,  712,  712,  712,  739,  712,  712,  712,    0,
  712,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  712,    0,    0,    0,    0,    0,  874,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  873,
    0,    0,  873,    0,    0,    0,    0,    0,    0,  915,
    0,    0,    0,    0,    0,    0,    0,  873, 1107,    0,
    0,    0,    0,    0, 1107,    0,    0,  874,    0, 1107,
  874,    0, 1107,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  874,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  915,    0,    0,  915,    0,    0, 1107,  906,
    0,    0,  915,    0,  915,    0, 1107,    0,  915,    0,
    0,    0,    0,  915, 1107,    0,  915,    0,  916,    0,
    0,    0,    0,    0,    0,    0,    0, 1108,    0,    0,
    0,  915, 1107, 1108, 1107,    0,    0,    0, 1108,    0,
  906, 1108,    0,  906,    0,  915,    0, 1107,    0, 1107,
 1107, 1107,    0,  915,    0,    0,  915,  915,  906,    0,
    0,    0,    0,    0,    0,    0,  915,    0,    0,    0,
    0,  916,    0,    0,  916,    0,    0, 1108,    0,    0,
    0,  916,  915,  916,    0, 1108,    0,  916,    0,    0,
    0,    0,  916, 1108,    0,  916,  868,    0, 1107,    0,
    0,    0,    0,    0,  915,    0,    0,    0,    0,  915,
  916, 1108,    0, 1108,    0,    0,    0,    0,    0,    0,
    0,  915,    0,  915,  916,    0, 1108,    0, 1108, 1108,
 1108,    0,  916,    0,    0,  916,  916,  868,    0,    0,
  868,    0,    0,    0,    0,  916,    0,    0,    0,    0,
  873,    0,    0,    0,    0,  868,    0,    0,    0,    0,
    0,  916,    0,    0,    0,    0,    0,    0,    0,    0,
  915,    0,    0,    0,    0,    0,    0, 1108,  874,  869,
    0,    0,    0,  916,    0,    0,    0,    0,  916,    0,
  915,    0,    0,    0,  915,    0,    0,    0,    0,    0,
  916,    0,  916,  873,    0,    0,  873,  870,    0,    0,
    0,    0,    0,  873,    0,  873,    0,    0,    0,  873,
  869,    0,    0,  869,  873,    0,    0,  873,    0,    0,
    0,  874,    0,    0,  874,    0,    0,    0,  869,    0,
    0,  874,  873,  874,    0,    0,    0,  874,  870,  916,
    0,  870,  874,    0,    0,  874,  873,    0,    0,    0,
    0,  906,    0,    0,  873,    0,  870,  873,  873,  916,
  874,    0,    0,  916,    0,    0,    0,  873,    0,    0,
    0,    0,    0,    0,  874,    0,    0,    0,    0,    0,
    0,    0,  874,  873,    0,  874,  874,    0,    0,    0,
    0,    0,    0,    0,    0,  874,    0,    0,    0,    0,
    0,    0,    0,    0,  906,  873,    0,  906,    0,    0,
  873,  874,    0,    0,  906,    0,  906,    0,    0,    0,
  906,    0,  873,    0,  873,  906,    0,    0,  906,  905,
    0,    0,    0,  874,    0,    0,    0,    0,  874,    0,
    0,    0,    0,  906,    0,    0,    0,    0,  868,    0,
  874,    0,  874,    0,    0,    0,    0,  906,    0,    0,
    0,    0,    0,    0,    0,  906,    0,    0,  906,  906,
  905,  873,    0,  905,    0,    0,    0,    0,  906,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  905,    0,
    0,  873,    0,    0,  906,  873,    0,    0,    0,  874,
    0,  868,    0,    0,  868,    0,    0,    0,  899,    0,
    0,  868,    0,  868,    0,    0,  906,  868,    0,  874,
    0,  906,  868,  874,    0,  868,    0,    0,    0,    0,
    0,  869,    0,  906,    0,  906,    0,    0,    0,    0,
  868,    0,    0,    0,    0,    0,    0,    0,    0,  899,
    0,    0,  899,    0,  868,    0,    0,    0,    0,  870,
    0,    0,  868,    0,    0,  868,  868,  899,    0,    0,
    0,    0,    0,    0,    0,  868,    0,    0,    0,    0,
    0,    0,  906,    0,  869,    0,    0,  869,    0,    0,
    0,  868,    0,    0,  869,    0,  869,    0,    0,    0,
  869,    0,  906,    0,    0,  869,  906,    0,  869,    0,
    0,    0,  870,  868,    0,  870,    0,    0,  868,    0,
    0,    0,  870,  869,  870,    0,    0,    0,  870,    0,
  868,    0,  868,  870,    0,    0,  870,  869,    0,    0,
    0,    0,    0,    0,    0,  869,    0,    0,  869,  869,
    0,  870,    0,    0,    0,    0,    0,    0,  869,    0,
    0,    0,    0,  907,    0,  870,    0,    0,    0,    0,
    0,    0,    0,  870,  869,    0,  870,  870,    0,  868,
    0,    0,    0,    0,    0,    0,  870,    0,    0,    0,
    0,  905,    0,  908,    0,    0,  869,    0,    0,  868,
    0,  869,  870,  868,  907,    0,    0,  907,    0,    0,
    0,    0,    0,  869,    0,  869,    0,    0,    0,    0,
    0,    0,  907,    0,  870,    0,    0,    0,    0,  870,
    0,    0,    0,    0,  908,  896,    0,  908,    0,    0,
    0,  870,    0,  870,  905,    0,    0,  905,    0,    0,
    0,    0,  908,    0,  905,    0,  905,    0,    0,    0,
  905,    0,  869,    0,    0,  905,    0,    0,  905,    0,
  899,    0,    0,    0,    0,    0,  896,    0,    1,  896,
    0,    0,  869,  905,    0,    0,  869,    0,    0,    0,
  870,    0,    0,    0,  896,    0,    2,  905,    0,    0,
    0,    0,    0,    0,    3,  905,    0,    0,  905,  905,
  870,    4,    0,    5,  870,    6,    0,    0,  905,    0,
    7,    0,    0,  899,    0,    0,  899,    8,    0,    0,
    0,    0,    0,  899,  905,  899,    0,    0,    0,  899,
    0,    9,    0,    0,  899,    0,    0,  899,    0,    0,
    0,   10,    0,    0,    0,    0,  905,  894,    0,    0,
    0,  905,  899,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  905,    0,  905,  899,    0,    0,    0,
    0,    0,    0,    0,  899,    0,    0,  899,  899,    0,
  900,    0,    0,    0,    0,    0,    0,  899,  894,    0,
    0,  894,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  899,   11,    0,  894,    0,    0,    0,
    0,   12,  905,    0,   13,  907,   14,   15,    0,    0,
    0,  900,  909,    0,  900,  899,    0,    0,    0,    0,
  899,    0,  905,    0,    0,    0,  905,    0,    0,  900,
    0,   16,  899,    0,  899,  908,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  909,    0,    0,  909,    0,  907,    0,
    0,  907,    0,    0,    0,    0,    0,    0,  907,    0,
  907,  909,    0,    0,  907,    0,    0,  896,    0,  907,
   17,  899,  907,    0,    0,    0,    0,    0,  908,    0,
    0,  908,    0,    0,    0,   18,   19,  907,  908,    0,
  908,  899,    0,    0,  908,  899,  895,    0,    0,  908,
    0,  907,  908,    0,    0,    0,    0,    0,    0,  907,
   20,    0,  907,  907,    0,    0,    0,  908,    0,    0,
  896,    0,  907,  896,    0,    0,    0,    0,    0,    0,
  896,  908,  896,    0,    0,    0,  896,  895,  907,  908,
  895,  896,  908,  908,  896,    0,    0,    0,    0,    0,
    0,    0,  908,    0,    0,  895,    0,    0,    0,  896,
  907,    0,    0,    0,    0,  907,    0,    0,  908,    0,
    0,  897,    0,  896,    0,    0,    0,  907,    0,  907,
    0,  896,    0,    0,  896,  896,    0,    0,    0,  894,
  908,    0,    0,    0,  896,  908,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  908,    0,  908,
  896,    0,  897,    0,    0,  897,    0,    0,    0,    0,
    0,    0,  900,    0,    0,    0,  907,    0,    0,    0,
  897,    0,  896,    0,    0,    0,    0,  896,    0,    0,
    0,    0,  894,    0,    0,  894,  907,    0,    0,  896,
  907,  896,  894,    0,  894,    0,  908,    0,  894,    0,
  901,    0,    0,  894,  909,    0,  894,    0,    0,    0,
    0,    0,    0,    0,    0,  900,  908,    0,  900,    0,
  908,  894,    0,    0,    0,  900,    0,  900,    0,    0,
    0,  900,    0,  725,    0,  894,  900,    0,  896,  900,
    0,  901,    0,  894,  901,    0,  894,  894,    0,    0,
    0,    0,    0,    0,  900,    0,  894,  909,  896,  901,
  909,    0,  896,    0,    0,    0,    0,  909,  900,  909,
    0,    0,  894,  909,  725,  723,  900,  725,  909,  900,
  900,  909,    0,    0,    0,    0,    0,    0,    0,  900,
    0,    0,  725,    0,  894,    0,  909,    0,  895,  894,
    0,    0,    0,    0,    0,  900,    0,    0,    0,    0,
  909,  894,    0,  894,    0,  723,  723,    0,  909,  723,
    0,  909,  909,    0,    0,    0,    0,  900,    0,    0,
    0,  909,  900,    0,  723,  736,    0,    0,    0,    0,
    0,    0,    0,    0,  900,    0,  900,  909,    0,    0,
    0,  895,    0,    0,  895,    0,    0,    0,    0,    0,
  894,  895,    0,  895,    0,    0,    0,  895,    0,  909,
    0,    0,  895,  897,  909,  895,  736,    0,    0,  736,
  894,    0,    0,    0,  894,    0,  909,    0,  909,    0,
  895,    0,    0,  900,  736,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  895,    0,    0,    0,    0,    0,
    0,    0,  895,  900,    0,  895,  895,  900,    0,    0,
    0,    0,    0,    0,    0,  895,  897,    0,    0,  897,
    0,    0,    0,    0,    0,  909,  897,    0,  897,    0,
    0,  895,  897,    0,    0,    0,    0,  897,    0,    0,
  897,    0,    0,    0,    0,  909,    0,    0,    0,  909,
    0,    0,  901,  895,    0,  897,    0,    0,  895,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  897,
  895,    0,  895,    0,    0,    0,    0,  897,    0,  759,
  897,  897,  513,    0,  514,    0,    0,    0,    0,    0,
  897,    0,    0,    0,    0,    0,    0,  516,    0,    0,
    0,    0,  334,    0,    0,  901,  897,    0,  901,    0,
    0,    0,    0,    0,    0,  901,    0,  901,    0,  895,
    0,  901,    0,    0,    0,    0,  901,    0,  897,  901,
    0,    0,    0,  897,    0,    0,    0,    0,  725,  895,
    0,  725,    0,  895,  901,  897,    0,  897,  725,    0,
  725,    0,    0,    0,  725,    0,    0,    0,  901,  725,
    0,    0,  725,    0,    0,    0,  901,    0,    0,  901,
  901,    0,    0,    0,    0,    0,    0,  725,    0,  901,
  723,    0,    0,  723,    0,    0,    0,    0,    0,    0,
  723,  725,  723,    0,  897,  901,  723,    0,    0,  725,
    0,  723,    0,  725,  723,    0,    0,    0,    0,    0,
    0,    0,  725,    0,  897,    0,  313,  901,  897,  723,
    0,    0,  901,    0,    0,    0,    0,    0,  725,    0,
    0,    0,    0,  723,  901,    0,  901,    0,    0,    0,
  736,  723,    0,  736,    0,  723,    0,    0,    0,    0,
  736,    0,  736,    0,  723,  725,  736,    0,    0,    0,
    0,  736,    0,    0,  736,    0,    0,  725,    0,  725,
  723,    0,    0,    0,    0,    0,    0,    0,    0,  736,
    0,    0,    0,  901,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  736,    0,    0,    0,  723,    0,    0,
    0,  736,    0,  901,    0,  736,  469,  901,    0,  723,
    0,  723,    0,  470,  736,    0,  725,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,    0,    0,    0,
  736,  472,  473,    0,    0,    0,  725,    0,    0,    0,
  725,    0,  759,    0,    0,  513,    0,  514,    0,    0,
  753,    0,    0,    0,    0,    0,    0,  736,  723,  474,
  516,    0,  475,    0,    0,  334,    0,    0,    0,  736,
    0,  736,    0,    0,    0,    0,    0,    0,  723,    0,
    0,    0,  723,    0,    0,    0,    0,  476,    0,  477,
  478,    0,    0,    0,    0,    0,    0,    0,    0,  479,
    0,    0,    0,  480,    0,    0,    0,    0,    0,    0,
    0, 1003,    0,    0,    0,    0,    0,    0,  736,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  736,    0,
    0,    0,  736,    0,    0,    0,    0,    0,   14,    0,
    0,    0,  755,    0,    0,    0,    0,  912,  481,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0, 1126,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136, 1004, 1005, 1006,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,    0,  507,  508,  509,    0,  510,  511,  469,
  512,  146,    0,    0,    0,    0,  470,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
    0,  752,    0,    0,  472,  473,    0,    0,  759,    0,
    0,  513,    0,  514,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  753,    0,    0,  516,    0,    0,    0,
    0,  334,  474,    0,    0,  475,    0,    0,  115,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,    0,
  476,    0,  477,  478,    0,    0,    0,  124,  125,    0,
  126,    0,  479,    0,    0,    0,  480,  127,  128,    0,
  129,  130,    0,  131,  754,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,  755,    0,    0,    0,    0,
    0,  481,    0,    0,    0,    0,    0,  482,    0,    0,
 1099,    0,    0,    0,    0,    0,    0,  483,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,  485,  486,  487,  255,  256,  116,    0,  117,
  118,  119,  120,    0,  121,  488,    0,    0,    0,    0,
    0,    0,  122,  123,  489,  490,  491,  492,  493,  494,
  495,  496,  497,  124,  498,    0,  126,    0,    0,  499,
  500,    0,  501,  502,  503,    0,  129,  130,  504,  131,
    0,    0,  132,  133,  756,  757,  758,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  505,  141,    0,
  506,  143,  144,    0,  145,  469,  507,  508,  509,    0,
  510,  511,  470,  512,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  471,    0,    0,    0,    0,    0,
  472,  473,    0,    0,  759,    0,    0,  513,    0,  514,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  753,
    0,    0,  516,    0,    0,    0,    0,  334,  474,    0,
    0,  475,    0,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,    0,  476,    0,  477,  478,
    0,    0,    0,  124,  125,    0,  126,    0,  479,    0,
    0,    0,  480,  127,  128,    0,  129,  130,    0,  131,
  999,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,  755,    0,    0,    0,    0,    0,  481,    0,    0,
    0, 1098,    0,  482,    0,    0,    0, 1099,    0,    0,
    0,    0,    0,  483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  484,  485,  486,
  487,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  488,    0,    0,    0,    0,    0,    0,  122,  123,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  124,
  498,    0,  126,    0,    0,  499,  500,    0,  501,  502,
  503,    0,  129,  130,  504,  131,    0,    0,  132,  133,
  756,  757,  758,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  505,  141,    0,  506,  143,  144,    0,
  145,  469,  507,  508,  509,    0,  510,  511,  470,  512,
  146,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,    0,    0,    0,    0,  472,  473,    0,    0,
  586,    0,    0,  513,    0,  514,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  753,    0,    0,  516,    0,
    0,    0,    0,  334,  474,    0,    0,  475,    0,    0,
  115,    0,    0,  116,    0,  117,  118,  119,  120,    0,
  121,    0,    0,    0,    0,    0,    0,    0,  122,  123,
    0,    0,  476,    0,  477,  478,    0,    0,    0,  124,
  125,    0,  126,    0,  479,    0,    0,    0,  480,  127,
  128,    0,  129,  130,    0,  131,  754,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  146,    0,    0,    0,    0,    0,    0,  755,    0,    0,
    0,    0,    0,  481,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,   85,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  756,  757,  758,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,  469,  507,  508,
  509,    0,  510,  511,  470,  512,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  471,    0,    0,    0,
    0,    0,  472,  473,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  759,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,    0,
  474,    0,    0,  475,  516,    0,    0,  115,    0,  334,
  116,    0,  117,  118,  119,  120,    0,  121,    0,    0,
    0,    0,    0,    0,    0,  122,  123,    0,  476,    0,
  477,  478,    0,    0,    0,    0,  124,  125,    0,  126,
  479,    0,    0,    0,  480,    0,  127,  128,    0,  129,
  130,    0,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  140,  141,    0,  142,  143,  144,    0,  145,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  481,
    0,    0,    0,    0,    0,  482,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  483,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  484,
  485,  486,  487,  255,  256,  116,    0,  117,  118,  119,
  120,  709,  121,  488,    0,    0,    0,    0,    0,    0,
  122,  123,  489,  490,  491,  492,  493,  494,  495,  496,
  497,  124,  498,    0,  126,    0,    0,  499,  500,    0,
  501,  502,  503,    0,  129,  130,  504,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  505,  141,    0,  506,  143,
  144, 1073,  145,    0,  507,  508,  509, 1074,  510,  511,
 1075,  512,  146,  469,    0,    0,  818,  816,    0,  817,
  470,  819,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0, 1024, 1023, 1025,    0,  472,  473,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  586,    0,    0,  513,  753,  514,    0,
    0,    0,    0,    0,    0,    0,  474,    0,    0,  475,
   84,  516,  115,    0,    0,  116,  334,  117,  118,  119,
  120,    0,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,    0,    0,  476,    0,  477,  478,    0,    0,
    0,  124,  125,    0,  126,    0,  479,    0,    0,    0,
  480,  127,  128,    0,  129,  130,    0,  131,  999,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  146,    0,    0,    0,    0,    0,    0,  755,
    0,    0,    0,    0,    0,  481,    0,    0,    0,    0,
    0,  482,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,  487,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  488,
    0,    0,  516,    0,  709,    0,  122,  123,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  124,  498,    0,
  126,    0,    0,  499,  500,    0,  501,  502,  503,    0,
  129,  130,  504,  131,    0, 1011,  132,  133,  134,  135,
  136,  814,    0,    0,  137,  138,  815,    0,    0, 1012,
  139,  505,  141,    0,  506,  143,  144,    0,  145,    0,
  507,  508,  509, 1263,  510,  511, 1264,  512,  146,    0,
  469,    0,    0,    0,    0,    0,    0,  470,    0,    0,
    0,    0,    0,    0,    0, 1013,    0,    0,    0,  471,
    0,    0,    0, 1014,    0,  472,  473,    0,    0,    0,
    0, 1015,    0,    0,    0,    0,    0,    0,    0,    0,
  586,    0,    0,  513,    0,  514,    0,    0,    0, 1016,
    0, 1017,    0,  474,    0,    0,  475,    0,  516,    0,
    0,    0,    0,  334, 1018,    0, 1019, 1020, 1021,  712,
    0,    0,  712,    0,  712,  712,  712,  712,    0,  712,
    0,  476,    0,  477,  478,    0,    0,  712,  712,    0,
    0,    0,    0,  479,    0,    0,    0,  480,  712,  712,
    0,  712,    0,    0,    0,    0,    0,    0,  712,  712,
    0,  712,  712,    0,  712, 1022,    0,  712,  712,  712,
  712,  712,    0,    0,    0,  712,  712,    0,    0,    0,
    0,  712,  712,  712,    0,  712,  712,  712,    0,  712,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  712,
 1265,    0,  481,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  484,  485,  486,  487,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  488,    0,    0,    0,
    0,    0,    0,  122,  123,  489,  490,  491,  492,  493,
  494,  495,  496,  497,  124,  498,    0,  126,    0,    0,
  499,  500,    0,  501,  502,  503,    0,  129,  130,  504,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  505,  141,
    0,  506,  143,  144,    0,  145,    0,  507,  508,  509,
 1274,  510,  511, 1264,  512,  146,    0,  469,    0,    0,
    0,    0,    0,    0,  470,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  471,    0,    0,    0,
    0,    0,  472,  473,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  586,    0,    0,
  513,    0,  514,    0,    0,    0,    0,    0,    0,    0,
  474,    0,    0,  475,    0,  516,  115,    0,    0,  116,
  334,  117,  118,  119,  120,    0,  121,    0,    0,    0,
    0,    0,    0,    0,  122,  123,    0,    0,  476,    0,
  477,  478,    0,    0,    0,  124,  125,    0,  126,    0,
  479,    0,    0,    0,  480,  127,  128,    0,  129,  130,
    0,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1265,    0,  481,
    0,    0,    0,    0,    0,  482,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  483,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  484,
  485,  486,  487,  255,  256,  116,    0,  117,  118,  119,
  120,    0,  121,  488,    0,    0,    0,    0,    0,    0,
  122,  123,  489,  490,  491,  492,  493,  494,  495,  496,
  497,  124,  498,    0,  126,    0,    0,  499,  500,    0,
  501,  502,  503,    0,  129,  130,  504,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  505,  141,    0,  506,  143,
  144,    0,  145,    0,  507,  508,  509, 1278,  510,  511,
 1264,  512,  146,    0,  469,  218,    0,    0,  218,    0,
    0,  470,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  218,    0,  471,    0,  218,    0,    0,  218,  472,
  473,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  586,    0,    0,  513,    0,  514,
    0,    0,    0,    0,    0,    0,    0,  474,    0,    0,
  475,    0,  516,    0,    0,    0,    0,  334,  218,    0,
    0,    0,    0,    0,  218,  218,  218,    0,  218,  218,
  218,    0,  218,    0,    0,  476,    0,  477,  478,    0,
    0,    0,    0,    0,    0,    0,    0,  479,    0,    0,
  218,  480,  218,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  218,    0,    0,    0,    0,   81,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  218,    0,
    0,    0,    0,    0, 1265,    0,  481,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
  218,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,  485,  486,  487,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  488,    0,    0,    0,    0,  218,  218,  122,  123,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  124,  498,
    0,  126,  218,    0,  499,  500,    0,  501,  502,  503,
    0,  129,  130,  504,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  505,  141,    0,  506,  143,  144,    0,  145,
    0,  507,  508,  509, 1286,  510,  511, 1264,  512,  146,
    0,  469,  693,    0,    0,  694,    0,    0,  470,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,  695,    0,    0,  696,  472,  473,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  586,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,    0,    0,  474,    0,    0,  475,    0,  516,
    0,    0,    0,    0,  334,  697,    0,    0,    0,    0,
    0,  698,  699,  700,    0,  701,  702,  703,    0,  704,
    0,    0,  476,    0,  477,  478,    0,    0,    0,    0,
    0,    0,    0,    0,  479,    0,    0,  705,  480,  706,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  707,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  708,    0,    0,    0,    0,
    0, 1265,    0,  481,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,  709,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,  710,  711,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,  712,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,    0,  507,  508,
  509, 1289,  510,  511, 1264,  512,  146,    0,  469,  148,
    0,    0,  148,    0,    0,  470,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,  148,
    0,    0,  148,  472,  473,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  586,    0,
    0,  513,    0,  514,    0,    0,    0,    0,    0,    0,
    0,  474,    0,    0,  475,    0,  516,    0,    0,    0,
    0,  334,  148,    0,    0,    0,    0,    0,  148,  148,
  148,    0,  148,  148,  148,    0,  148,    0,    0,  476,
    0,  477,  478,    0,    0,    0,    0,    0,    0,    0,
    0,  479,    0,    0,  148,  480,  148,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  148,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  148,    0,    0,    0,    0,    0, 1265,    0,
  481,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,  148,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0, 1359,    0,  148,
  148,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,  148,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144,    0,  145,    0,  507,  508,  509, 1294,  510,
  511, 1264,  512,  146,    0,  469,    0, 1360,    0,    0,
    0,    0,  470,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  471,    0,    0,    0,    0,    0,
  472,  473,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1361,    0,    0,  586,    0, 1362,  513,    0,
  514,    0,    0, 1363,    0,    0,    0,    0,  474,    0,
    0,  475,    0,  516,  115,    0,    0,  116,  334,  117,
  118,  119, 1364,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,    0,  476,    0,  477,  478,
    0,    0,    0,  124,  125,    0,  126,    0,  479,    0,
    0,    0,  480,  127,  128,    0,  129,  130,    0,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1265,    0,  481,    0,    0,
    0,    0,    0,  482,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  484,  485,  486,
  487,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  488,    0,    0,    0,    0,    0,    0,  122,  123,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  124,
  498,    0,  126,    0,    0,  499,  500,    0,  501,  502,
  503,    0,  129,  130,  504,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  505,  141,    0,  506,  143,  144,    0,
  145,    0,  507,  508,  509, 1297,  510,  511, 1264,  512,
  146,    0,  469,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,    0,  472,  473,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  586,    0,  567,  513,    0,  514,    0,    0,
    0,    0,    0,    0,    0,  474,    0,    0,  475,    0,
  516,  115,    0,    0,  116,  334,  117,  118,  119,  120,
    0,  121,    0,    0,    0,    0,    0,    0,    0,  122,
  123,    0,    0,  476,    0,  477,  478,    0,    0,    0,
  124,  125,    0,  126,    0,  479,    0,    0,    0,  480,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1265,    0,  481,    0,    0,    0,    0,    0,
  482,  661,    0,    0,    0,    0,    0,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,    0,  499,  500,    0,  501,  502,  503,    0,  129,
  130,  504,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,    0,  507,
  508,  509, 1300,  510,  511, 1264,  512,  146,    0,  469,
    0,    0,    0,    0,    0,    0,  470,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
    0,    0,    0,    0,  472,  473,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  696,    0,  696,  696,    0,
  696,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  474,  696,    0,  475,    0,    0,  696,  115,
    0,    0,  116,    0,  117,  118,  119,  120,    0,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123,    0,
  476,    0,  477,  478,    0,    0,    0,    0,  124,  125,
    0,  126,  479,    0,    0,    0,  480,    0,  127,  128,
    0,  129,  130,    0,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,    0,  142,  143,  144,    0,  145,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1265,
    0,  481,    0,    0,  667,    0,    0,  482,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  483,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,  485,  486,  487,  255,  256,  116,    0,  117,
  118,  119,  120,    0,  121,  488,    0,    0,    0,    0,
    0,    0,  122,  123,  489,  490,  491,  492,  493,  494,
  495,  496,  497,  124,  498,    0,  126,    0,    0,  499,
  500,    0,  501,  502,  503,    0,  129,  130,  504,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  505,  141,    0,
  506,  143,  144,    0,  145,  696,  507,  508,  509,    0,
  510,  511,  696,  512,  146,    0,    0,    0,    0,  696,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  696,    0,    0,    0,    0,    0,  696,  696,    0,
    0,    0,    0,    0,    0,  696,    0,    0,  690,    0,
  690,  690,    0,  690,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  696,  690,    0,  696,    0,
    0,  690,  115,    0,    0,  116,    0,  117,  118,  119,
  120,    0,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,    0,  696,    0,  696,  696,    0,    0,    0,
    0,  124,  125,    0,  126,  696,    0,    0,    0,  696,
    0,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  696,    0,    0,    0,    0,    0,
  696,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  696,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  696,  696,  696,  696,  696,  696,
  696,    0,  696,  696,  696,  696,    0,  696,  696,    0,
    0,    0,    0,    0,    0,  696,  696,  696,  696,  696,
  696,  696,  696,  696,  696,  696,  696,  696,    0,  696,
    0,    0,  696,  696,    0,  696,  696,  696,    0,  696,
  696,  696,  696,    0,    0,  696,  696,  696,  696,  696,
    0,    0,    0,  696,  696,    0,    0,    0,    0,  696,
  696,  696,    0,  696,  696,  696,    0,  696,  690,  696,
  696,  696,    0,  696,  696,  690,  696,  696,    0,    0,
    0,    0,  690,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  690,    0,    0,    0,    0,    0,
  690,  690,    0,    0,  586,    0,  591,  513,  690,  514,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  516,    0,    0,    0,    0,  334,  690,    0,
    0,  690,    0,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,    0,  690,    0,  690,  690,
    0,    0,    0,  124,  125,    0,  126,    0,  690,    0,
    0,    0,  690,  127,  128,    0,  129,  130,    0,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  690,    0,    0,
    0,    0,    0,  690,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  690,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  690,  690,  690,
  690,  690,  690,  690,    0,  690,  690,  690,  690,    0,
  690,  690,    0,    0,    0,    0,    0,    0,  690,  690,
  690,  690,  690,  690,  690,  690,  690,  690,  690,  690,
  690,    0,  690,    0,    0,  690,  690,    0,  690,  690,
  690,    0,  690,  690,  690,  690,    0,    0,  690,  690,
  690,  690,  690,    0,    0,    0,  690,  690,    0,    0,
    0,    0,  690,  690,  690,    0,  690,  690,  690,    0,
  690,  469,  690,  690,  690,    0,  690,  690,  470,  690,
  690,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,    0,    0,    0,    0,  472,  473,    0,    0,
  586,    0,    0,  513,    0,  514,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  516,    0,
    0,    0,    0,  334,  474,    0,    0,  475,    0,    0,
   28,    0,    0,   28,    0,   28,   28,   28,   28,    0,
   28,    0,    0,    0,    0,    0,    0,    0,   28,   28,
    0,    0,  476,    0,  477,  478,    0,    0,    0,   28,
   28,    0,   28,    0,  479,    0,    0,    0,  480,   28,
   28,    0,   28,   28,    0,   28,    0,    0,   28,   28,
   28,   28,   28,    0,    0,    0,   28,   28,    0,    0,
    0,    0,   28,   28,   28,    0,   28,   28,   28,    0,
   28,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   28,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  481,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,  469,  507,  508,
  509,    0,  510,  511,  470,  512,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  471,    0,    0,    0,
    0,    0,  472,  473,    0,    0,  586,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  516,    0,    0,    0,    0,  334,
  474,    0,    0,  475,    0,    0,  604,    0,    0,  604,
    0,  604,  604,  604,  604,    0,  604,    0,    0,    0,
    0,    0,    0,    0,  604,  604,    0,    0,  476,    0,
  477,  478,    0,    0,    0,  604,  604,    0,  604,    0,
  479,    0,    0,    0,  480,  604,  604,    0,  604,  604,
    0,  604,    0,    0,  604,  604,  604,  604,  604,    0,
    0,    0,  604,  604,    0,    0,    0,    0,  604,  604,
  604,    0,  604,  604,  604,    0,  604,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  604,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  481,
    0,    0,    0,    0,    0,  482,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  483,    0,    0,    0,    0,
    0,    0,    0,  791,    0,    0,    0,    0,    0,  484,
  485,  486,  487,  255,  256,  116,    0,  117,  118,  119,
  120,    0,  121,  488,    0,    0,    0,    0,    0,    0,
  122,  123,  489,  490,  491,  492,  493,  494,  495,  496,
  497,  124,  498,    0,  126,    0,    0,  499,  500,    0,
  501,  502,  503,    0,  129,  130,  504,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  505,  141,    0,  506,  143,
  144,    0,  145,  469,  507,  508,  509,    0,  510,  511,
  470,  512,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,    0,    0,    0,  472,  473,
    0,    0,  586, 1082,    0,  513,    0,  514,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  516,    0,    0,    0,    0,  334,  474,    0,    0,  475,
    0,    0,  287,    0,    0,  287,    0,  287,  287,  287,
  287,    0,  287,    0,    0,    0,    0,    0,    0,    0,
  287,  287,    0,    0,  476,    0,  477,  478,    0,    0,
    0,  287,  287,    0,  287,    0,  479,    0,    0,    0,
  480,  287,  287,    0,  287,  287,    0,  287,    0,    0,
  287,  287,  287,  287,  287,    0,    0,    0,  287,  287,
    0,    0,    0,    0,  287,  287,  287,    0,  287,  287,
  287,    0,  287,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  287,    0,    0,   14,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  481,    0,    0,    0,    0,
    0,  482,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,  487,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  488,
    0,    0,    0,    0,    0,    0,  122,  123,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  124,  498,    0,
  126,    0,    0,  499,  500,    0,  501,  502,  503,    0,
  129,  130,  504,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  505,  141,    0,  506,  143,  144,    0,  145,  469,
  507,  508,  509,    0,  510,  511,  470,  512,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
    0,    0,    0,    0,  472,  473,    0,    0,  586,    0,
    0,  513,    0,  514,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  516,    0,    0,    0,
    0,  334,  474,    0,    0,  475,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  476,    0,  477,  478,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,    0,    0,  480,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
  506,  143,  144,    0,  145,  469,  507,  508,  509,    0,
  510,  511,  470,  512,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  471,    0,    0,    0,    0,    0,
  472,  473,    0,    0,  586,    0,    0,  513,    0,  514,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  516,    0,    0,    0,    0,  334,  474,    0,
    0,  475,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  476,    0,  477,  478,
    0,    0,    0,    0,    0,    0,    0,    0,  479,    0,
    0,    0,  480,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  481,    0,    0,
    0,    0,    0,  482,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1292,    0,    0,  484,  485,  486,
  487,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  488,    0,    0,    0,    0,    0,    0,  122,  123,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  124,
  498,    0,  126,    0,    0,  499,  500,    0,  501,  502,
  503,    0,  129,  130,  504,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  505,  141,    0,  506,  143,  144,    0,
  145,  469,  507,  508,  509,    0,  510,  511,  470,  512,
  146,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,    0,    0,    0,    0,  472,  473,    0,    0,
  586,    0,    0,  513,    0,  514,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  516,    0,
    0, 1344,    0,  334,  474,    0,    0,  475,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  476,    0,  477,  478,    0,    0,    0,    0,
    0,    0,    0,    0,  479,    0,    0,    0,  480,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  481,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,  469,  507,  508,
  509,    0,  510,  511,  470,  512,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  471,    0,    0,    0,
    0,    0,  472,  473,    0,    0, 1059,    0, 1059, 1059,
    0, 1059,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1059,    0,    0,    0,    0, 1059,
  474,    0,    0,  475,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  476,    0,
  477,  478,    0,    0,    0,    0,    0,    0,    0,    0,
  479,    0,    0,    0,  480,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  481,
    0,    0,    0,    0,    0,  482,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  483,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1511,    0,    0,  484,
  485,  486,  487,  255,  256,  116,    0,  117,  118,  119,
  120,    0,  121,  488,    0,    0,    0,    0,    0,    0,
  122,  123,  489,  490,  491,  492,  493,  494,  495,  496,
  497,  124,  498,    0,  126,    0,    0,  499,  500,    0,
  501,  502,  503,    0,  129,  130,  504,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  505,  141,    0,  506,  143,
  144,    0,  145, 1059,  507,  508,  509,    0,  510,  511,
 1059,  512,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1059,    0,    0,    0,    0,    0, 1059, 1059,
    0,    0,  515,    0,    0,  513,    0,  514,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  516,    0,    0,    0,    0,  334, 1059,    0,    0, 1059,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1059,    0, 1059, 1059,    0,    0,
    0,    0,    0,    0,    0,    0, 1059,    0,    0,    0,
 1059,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1059,    0,    0,    0,    0,
    0, 1059,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1059,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1059, 1059, 1059, 1059, 1059,
 1059, 1059,    0, 1059, 1059, 1059, 1059,    0, 1059, 1059,
    0,    0,    0,    0,    0,    0, 1059, 1059, 1059, 1059,
 1059, 1059, 1059, 1059, 1059, 1059, 1059, 1059, 1059,    0,
 1059,    0,    0, 1059, 1059,    0, 1059, 1059, 1059,    0,
 1059, 1059, 1059, 1059,    0,    0, 1059, 1059, 1059, 1059,
 1059,    0,    0,    0, 1059, 1059,    0,    0,    0,    0,
 1059, 1059, 1059,    0, 1059, 1059, 1059,    0, 1059,  469,
 1059, 1059, 1059,    0, 1059, 1059,  470, 1059, 1059,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
    0,    0,    0,    0,  472,  473,    0,    0,  586,    0,
    0,  513,    0,  514,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  516,    0,    0,    0,
    0,  334,  474,    0,    0,  475,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  476,    0,  477,  478,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,    0,    0,  480,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
  506,  143,  144,    0,  145,  469,  507,  508,  509,    0,
  510,  511,  470,  512,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  471,    0,    0,    0,    0,    0,
  472,  473,    0,    0, 1059,    0,    0, 1059,    0, 1059,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1059,    0,    0,    0,    0, 1059,  474,    0,
    0,  475,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  476,    0,  477,  478,
    0,    0,    0,    0,    0,    0,    0,    0,  479,    0,
    0,    0,  480,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  481,    0,    0,
    0,    0,    0,  482,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  484,  485,  486,
  487,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  488,    0,    0,    0,    0,    0,    0,  122,  123,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  124,
  498,    0,  126,    0,    0,  499,  500,    0,  501,  502,
  503,    0,  129,  130,  504,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  505,  141,    0,  506,  143,  144,  571,
  145, 1059,  507,  508,  509,    0,  510,  511, 1059,  512,
  146,  572,    0,    0,    0,    0,    0,    0,    0,    0,
 1059,    0,    0,    0,    0,    0, 1059, 1059,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  571,
  571,    0,    0,  571,    0,    0,    0,    0,  566,    0,
    0,  572,  572,    0, 1059,  572,    0, 1059,  571,    0,
  571,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  572,    0,  572,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1059,    0, 1059, 1059,    0,    0,    0,  566,
  571,    0,  566,    0, 1059,    0,    0,    0, 1059,    0,
    0,    0,  572,    0,    0,    0,    0,  566,    0,  566,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  566,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1059,    0,    0,    0,    0,    0, 1059,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1059,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1059, 1059, 1059, 1059, 1059, 1059, 1059,
    0, 1059, 1059, 1059, 1059,    0, 1059, 1059,    0,    0,
    0,    0,    0,    0, 1059, 1059, 1059, 1059, 1059, 1059,
 1059, 1059, 1059, 1059, 1059, 1059, 1059,    0, 1059,    0,
  588, 1059, 1059,    0, 1059, 1059, 1059,    0, 1059, 1059,
 1059, 1059,    0,    0, 1059, 1059, 1059, 1059, 1059,    0,
    0,    0, 1059, 1059,    0,    0,    0,    0, 1059, 1059,
 1059,    0, 1059, 1059, 1059,    0, 1059,  571, 1059, 1059,
 1059,  588, 1059, 1059,  588, 1059, 1059,    0,    0,  572,
    0,    0,  571,    0,  571,  571,  571,    0,  578,  588,
  571,  588,    0,  571,  572,    0,  572,  572,  572,    0,
    0,    0,  572,    0,    0,  572,  571,    0,    0,    0,
    0,    0,  571,    0,    0,  571,  566,    0,  572,    0,
    0,  588,    0,    0,  572,    0,    0,  572,    0,  578,
    0,  566,  578,  566,  566,  566,    0,    0,    0,  566,
    0,    0,  566,    0,    0,    0,    0,  578,    0,  578,
    0,    0,    0,    0,    0,  566,    0,  571,    0,    0,
    0,  566,    0,    0,  566,    0,    0,    0,    0,  572,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  578,
    0,  571,    0,    0,    0,    0,    0,    0,  375,    0,
    0,    0,    0,  572,    0,    0,    0,    0,    0,  563,
    0,    0,  571,    0,  376,    0,  566,    0,  571,    0,
    0,    0,  571,    0,  572,  377,    0,    0,    0,  571,
  572,    0,  378,    0,  572,    0,  379,  380,    0,    0,
  566,  572,    0,    0,    0,  381,    0,  571,    0,    0,
  563,    0,  571,  563,    0,    0,  382,    0,    0,  572,
    0,  566,    0,    0,  572,    0,    0,  566,  563,    0,
  563,  566,    0,    0,    0,    0,    0,  571,  566,    0,
    0,  571,    0,    0,    0,    0,    0,    0,    0,  572,
    0,    0,    0,  572,    0,    0,  566,    0,  588,    0,
  563,  566,    0,    0,    0,    0,    0,    0,    0,    0,
  383,    0,    0,  588,    0,    0,  588,  588,  384,    0,
    0,  588,    0,    0,  588,    0,  566,    0,    0,    0,
  566,    0,    0,    0,    0,    0,    0,  588,    0,    0,
    0,    0,    0,  588,    0,    0,  588,    0,    0,  385,
    0,    0,    0,  386,    0,    0,  578,    0,    0,    0,
    0,    0,    0,    0,  387,    0,    0,    0,    0,    0,
    0,  578,    0,    0,  578,  578,    0,    0,    0,  578,
    0,    0,  578,  388,  389,    0,    0,    0,  588,    0,
    0,    0,    0,    0,    0,  578,    0,    0,    0,    0,
    0,  578,    0,    0,  578,    0,    0,    0,    0,    0,
    0,    0,  588,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  588,    0,    0,    0,    0,    0,  588,
    0,    0,  390,  588,    0,  391,  578,    0,    0,    0,
  588,    0,    0,    0,  108,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  563,  588,    0,
  578,    0,    0,  588,    0,    0,    0,    0,    0,    0,
    0,    0,  563,    0,    0,  563,  563,    0,    0,    0,
  563,  578,  109,  563,    0,    0,    0,  578,  588,    0,
    0,  578,  588,    0,  110,    0,  563,    0,  578,    0,
    0,    0,  563,    0,    0,  563,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  578,    0,  111,    0,
   49,  578,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  578,  563,    0,    0,
  578,    0,    0,    0,    0,    0,    0,    0,   49,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   49,  563,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  112,    0,    0,    0,    0,    0,    0,
    0,    0,  563,  113,   49,    0,    0,    0,  563,    0,
    0,    0,  563,    0,    0,    0,    0,    0,    0,  563,
    0,    0,    0,    0,    0,    0,    0,    0,  114,    0,
    0,    0,    0,    0,    0,    0,    0,  563,    0,    0,
    0,    0,  563,    0,    0,    0,  115,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,    0,
    0,    0,    0,    0,  122,  123,    0,  563,    0,   49,
    0,  563,    0,    0,    0,  124,  125,    0,  126,   49,
    0,    0,    0,    0,    0,  127,  128,    0,  129,  130,
    0,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,   49,    0,    0,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,  329,    0,
    0,    0,   51,    0,    0,   51,  146,   51,   51,   51,
   51,    0,   51,    0,    0,    0,    0,    0,    0,    0,
   51,   51,    0,    0,    0,    0,  329,    0,    0,    0,
    0,   51,   51,  329,   51,    0,    0,  329,  329,    0,
  329,   51,   51,  329,   51,   51,    0,   51,    0,    0,
   51,   51,   51,   51,   51,    0,  329,    0,   51,   51,
    0,    0,  329,    0,   51,   51,   51,    0,   51,   51,
   51,    0,   51,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   51,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1356,    0,    0,    0,    0,  329,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  329,    0,    0,
    0,    0,    6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  329, 1566, 1567,    0, 1568,    0,    0,    0,
    0,    0,  329,    0,    0,    0,    0,    0,    0,    0,
    0,  329, 1569,  329,    0,    0,    0,    0, 1570,    0,
  329,    0,    0,  329,    0,  329,  329,  329,  329,    0,
  329,    0,    0,    0,    0,    0,    0,    0,  329,  329,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  329,
  329,    0,  329,    0,    0,    0,    0,    0,    0,  329,
  329,    0,  329,  329,    0,  329,    0,    0,  329,  329,
  329,  329,  329,    0,    0, 1571,  329,  329,  329,  329,
  329,    0,  329,  329,  329,    0,  329,  329,  329, 1356,
  329,    0,    0,   14,    0,    0,    0,    0,    0,    0,
  329,    0,    0,    0,    0,    0,    0,    0, 1572,    0,
    0,    0,    0,    0,    0,    0,    0,    6,   16,    0,
    0,    0,    0,    0,    0,    0,    0, 1774, 1566, 1567,
    0, 1568,    0,    0,    0,    0,  487,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121, 1569,    0,    0,
    0,    0,    0, 1570,  122,  123,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1573,  125,    0,  126,    0,
    0,    0,    0,    0,    0,  127,  128,    0,  129,  130,
    0,  131,    0,    0,  132, 1574,  134,  135,  136,    0,
    0,    0,  137,  138, 1575, 1576, 1577,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
 1571,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   14,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1572,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   16,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  487,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,    0,  693,    0,    0,  694,    0,  122,
  123,    0,    0,  912,    0,    0,    0,    0,    0,    0,
 1573,  125,    0,  126,  695,    0,    0,  696,    0,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
 1574,  134,  135,  136,    0,    0,    0,  137,  138, 1575,
 1576, 1577,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,  697,    0,    0,
    0,  146,    0,  698,  699,  700,    0,  701,  702,  703,
    0,  704,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  705,
    0,  706,  693,    0,    0,  694,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  695,    0,    0,  696,    0,    0,    0,    0,
  707,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  708,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  697,    0,    0,    0,  709,
    0,  698,  699,  700,    0,  701,  702,  703,    0,  704,
    0,    0,    0,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,  705,    0,  706,
    0,    0,  122,  123,  710,  711,    0,    0,    0,    0,
    0,    0,    0,  124,  125,    0,  126,    0,    0,    0,
    0,  712,    0,  127,  128,    0,  129,  130,  707,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,  708,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,  709,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115,    0,    0,  116,    0,  117,  118,  119,
  120,    0,  121,    0, 1028, 1067,    0,    0, 1067,    0,
  122,  123,  710,  711,    0,    0,    0,    0,    0,    0,
    0,  124,  125, 1069,  126, 1067,    0, 1029, 1067,  712,
    0,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0, 1067,    0,
    0,    0,  146,    0, 1067, 1067, 1067,    0, 1067, 1067,
 1067,    0, 1067, 1030,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1067,    0, 1067,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
  116,    0,  117,  118,  119,  120,    0,  121,    0,    0,
    0, 1067,    0,    0,    0,  122,  123,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  124,  125, 1067,  126,
    0,    0,    0,    0,    0,    0,  127,  128,    0,  129,
  130,    0,  131,    0,    0,  132,  133,  134,  135,  136,
 1067,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  140,  141,    0,  142,  143,  144,    0,  145,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  146,    0,    0,
    0,    0,    0,    0,    0, 1067, 1067,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  115,    0, 1067,  116,  192,  117,  118,  119,  120,  193,
  121,  194,  195,  196,  197,  198,  199,  200,  122,  123,
  201,  202,  203,  204,  205,    0,    0,  206,  207,  124,
  125,  208,  126,    0,    0,  209,  210,  211,  212,  127,
  128,  213,  129,  130,  214,  131,  215,  216,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,  217,  218,
  219,  220,  139,  140,  141,  221,  142,  143,  144,  222,
  145,  223,  224,  225,  226,  227,  228,  229,  230,  231,
  146,  483,    0,    0,  483,  483,  483,  483,  483,  483,
    0,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,    0,    0,  483,  483,
  483,  483,  483,  483,    0,    0,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,    0,    0,    0,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,
};
static short yycheck[] = {                                       9,
    0,    4,   61,  174,   71,   16,   16,  114,   18,  164,
  165,  628,  164,  165,   70,  447,  412,   77,  447,   80,
   70,  102,  821,  791,  443,  821,   93,   99,  319,  797,
  631,  439,   75,  597,  515,  515,  877,  414,   97,  662,
 1092,  101,  103,  556, 1106,  576,  777, 1355,   58,   59,
   69,   61,   62,   63,   64,   65,   99,  575, 1176,   63,
   70,   71,   72, 1033,   74, 1183,   76,   77,  831,   79,
   80,   81,   76,   65,  824, 1193, 1356,  336,  809,   89,
   91,   91,   92,   93, 1429,   95,   96,   97,   98,  100,
  100,  101,  102,  103,  104,   62,  106, 1126,  179,  423,
  365, 1219,  986,    0,   96,  586,  156,   74,  158,  176,
 1080,  747,  104, 1170,   41,  679,  740,   45,  882,    0,
   44,  424, 1214, 1236, 1374,  875,   44, 1178,   40,   63,
    0,   98,   40,    0,   41,   63,   45,    0, 1170,  106,
  653,    0,  906,   45,  657,  909,  156,  157,  158,  662,
  453,   44,  162,  157,  918, 1170,  394,   41,    0,  169,
    0,   44,   59,   44,  174,   44,  176,  177,  178,  179,
   41,  273,  279,  183,  178,  183, 1170,  169,   59,   44,
 1231,    0,    0,    0,    0,    0,   40,    0,   44,   59,
 1111, 1170,   59,    0,  365,    0,   59,   44,    0,   41,
   59,   41,  324, 1386,   44,   40, 1170,  270,   41,   63,
  177,   41,   41,    0,    0,  979,  235,   59,    0,   59,
  639,   44,   41,  280,   41,   44,   41,   44,   41,   44,
  662,  614,  886,  662,   41,  262,   41,   44,    0,   41,
   59,   59,   59,   59,   59,    0,   59,  266,  754,    0,
   42,   40,   59,    0,   59,   47,  298,   59, 1026, 1027,
  310,    0,   41,  313,    0,  372,   44, 1575, 1576,  665,
 1427,  690,   59,   59,   63,  264,  567,   59,  759, 1421,
 1422, 1700,   44,  351, 1426,   91,   41,    0,  347,   44,
  348,  273,  583, 1092,    0, 1063, 1092,   59,  257,   41,
  310,  317,   41,  313,   59,  368,  264, 1587,   59,  319,
  320,   45,   59,  300,  617,  365, 1643,  298,  714,   41,
   59,   45,   44,   59,   41,  297, 1444,    0,    0,  272,
  371,   41,   44,  813,  456,   41,  265,  347,    0,  260,
  820,  351,   46,  337,  396,   44,   59,  351,  367,  447,
 1401,  345,  379,   59,   58,  262,  458,   61, 1387,    0,
  412,  348, 1424,  413, 1421, 1422,  348,    0, 1460, 1426,
  299,  330,  395,  423,  431,  425,  348,  427,  262,  300,
  430,   93,  449,  393,  394,  372,   59,   59, 1152, 1421,
 1422,  451,  434,  336, 1426,  453,  454,   59,  962,  440,
  427, 1169,  461,  413,  575, 1467, 1421, 1422,   41, 1173,
 1174, 1426,  428,  423,  403,  425,  484,  427,   59, 1150,
  430,  440,   44,  442,  937,    0,   59, 1421, 1422,  262,
 1400, 1195, 1426,  262,  297,  306,  276,  277,  457,  449,
  450,  451, 1421, 1422,  284,  403,    0, 1426,    0,  459,
 1633,  461,  317,  434,  464,  459,  987, 1421, 1422, 1380,
 1381,  464, 1426, 1771,  317,    0, 1793,  998,  315,  275,
 1815, 1002,  379,  315,    0,  858,  306,  500,  301,  387,
  336,  344,  921,  450,   59,  277,  264,   41, 1256, 1257,
  282, 1259, 1260,  376,  336,  379,  315, 1003,  315,  380,
 1919,  897,  315, 1157,  900,   59,  516,   59,  389, 1666,
  380,  297,  325,  315,  327,   41,  450,  336,  914,  336,
  262,  380,  450,  336,   59,  575,  450,  439,  446,  681,
  389,  336,  372,   59,  336,  428,  376, 1787,  380, 1324,
 1658,  450, 1327,  272, 1662, 1663,  379,  943,  450,  840,
  379, 1350,  262,  393, 1350,  565,  566,  567,  344,  399,
  315,  565,  380,  444,  574,  444, 1911,  380,  578,  376,
  574,  389,  327,  583,  578,  585,  389,  585,  628,  446,
 1698,  336, 1466,  907, 1697,  401,  513, 1649,  423, 1352,
  432,  264,  410,  433,  853, 1126,  450,  336, 1116,  566,
  379, 1351,  301,  498,  446,  624, 1230, 1243,  344, 1954,
  682,  876,  662,  990,  365, 1644,  513, 1709,  628,  432,
  402, 1238,  641, 1154,  376,  380,  444,  446, 1679,  446,
  336, 1976,  513,  446,  389,   59,  655,  379, 1118,  682,
  262,  446, 1410,  513,  446, 1696,  509,    0,  361,  887,
  821,  661,  662,  493,  513,  402,    0,  667,  754,  446,
 1940,  450,  379,  759, 1432,  684,  338,  730,  344,  379,
   42,  513,  315,  828,  977,  494,  338,  432,  493,   61,
    0,  403,  315, 1801,  446, 1965,  493, 1805,   41,  739,
  262,  446,    0,  310,  513,  513,  513,  338, 2006,  371,
  513, 1465, 1820,  336,  315,  876,   59,  446,    0,  861,
  729,  513,   61, 1993,    0,   59,  450,  736,  737, 1837,
 1772,   41,  334,  509, 2004,  397,  450, 2007,  400,  739,
  371, 1873, 1874,   41, 2014,  397,   44,    0,  400,   59,
  446,  276,  277,  477,  416, 1715,  484, 1186, 1187,   41,
    0,   59,    0,  477,  416, 1163,  397,  379,  513,  400,
 1545,  457,  268,  459, 1549, 1827, 1497,   59,  297,  344,
  442, 1833,  782,   59,  397,  416,    0,    0,  516,  829,
  442,  831,  336,  833, 1180,   93, 1182, 1268, 1268,  315,
  782,   41,  409,  945,   44,  283,   59,  257, 1194, 1238,
 1918,  442,  445, 1921,  814,  303, 1647,  379,  262,   59,
  336,   59,  303,  446,  824,  344, 1873, 1874,   41,  829,
  275,  831,  434,  833, 1232,  448,  876,  298,  395, 1430,
  840,  881,  262,  509,  445,   59,   59,  372,  848,  849,
 1958, 1873, 1874,   93,  848,    0,  856,  857,  309,  401,
  315,  304,  305,  359, 1334,  427,  319,  907, 1873, 1874,
  870,  933,  872,  334, 1241,  875,  870,  886,  872,  300,
  330,  881,  304,  305,  893,  280,  428,  887, 1646, 1873,
 1874, 1933,  849, 1935,  934,  904,  341,  937,  260, 1453,
  933, 1943, 1456, 1945, 1873, 1874,  305,  907, 1587,  942,
  972,  973,  974,  999,   59, 1385,  315, 1003, 1531, 1873,
 1874,    0,  410,  261,  258,  297,  959,  348,    0,  410,
  446, 1092,    0,    0,  934,  379,  270,  937,  300,  972,
  973,  974,  951, 1310,  395,    0,  450,    0, 1990,    0,
    0,  304,  305,  953,    0, 1116,  305,  514,  297,  379,
  953,    0,   41,  297, 1946,   44,  315,   41,  352,  303,
   44,  305,  315,  434,  462,  453,  454, 1752,  978,  430,
   59,  462,  377, 1772,  978, 1375, 1772,   59,  988,   44,
  509,   59,   59,  336,  988,   41,  996,  997,   44,    0,
  443,  322,   41,  996,   59,  315,   59, 1765,   59,   59,
  344,  264, 1994,   59,    0,  325,  316,  327,  352,  298,
   59,  443,  238, 1494, 1495,   41,  336,  401,   44,    0,
    0, 1502, 1503,  315, 1505, 1506,  431,  421,  276,  277,
    0,    0, 1513, 1514, 1515, 1516,  445, 1518, 1519, 1520,
 1521, 1522, 1523,  448,  336,  334, 1442, 1560,   59,    0,
 1450, 1470,  338,   42,   43,  276,   45, 1067,   47,  450,
  380,   41,    0,   59,  290, 1217, 1116,  344, 1359,  389,
    0, 1362,  298,  297,    0, 1067, 1472,  421,   59,   59,
  443,  319,  277,  393,    0,  371,  445,  282,  380,   59,
   59,  422,  315,  446,  402,  316,    0,  389,  429, 1531,
 1110, 1111, 1531,   41,    0,  258,   44,  265,   59,  264,
    0,  397,  432,  336,  400,   41, 1126,  319,   44,    0,
  344,   59,    0,  433,  444, 1893,  446,  390,  352,   59,
  416,  380, 1900,   59, 1933,   40, 1935, 1933, 1157, 1935,
  432,  299,  397,   59, 1943,   41, 1945, 1943,   44, 1945,
  303,  394,  402, 1225,  446,   59,  442,  380,    0,  376,
  513,  416,  386,   59,  388,  509,  389,  339, 1240,   59,
    0, 1788,  393,    0, 1246,  333,  453,  454,   59, 1350,
  258,   59, 1225,  432, 1234, 1356,  410,  264, 1238,  273,
  362, 1990,  270,  513, 1990,  258, 1206, 1240,  258,   41,
   41,   42,   43, 1246,   45,   40,   47,  270, 1211,  432,
  270,  435,  433, 1223,    0,  276,  277,   59, 1373, 1374,
 1375,  513,  306,  446, 1234,  303,  315,  305, 1238,   59,
    0,  371,   59,    0,  297,  390,  325,  297,  327,    0,
  303,  309,  305,  303,  376,  305,  418,  336,   44,    0,
  339, 1940,  401,  264, 1406,   41, 1408,  410,   44,  315,
  399,    0,  401,  397,  348,  263,  315,    0,  264,  325,
  410,  327, 1668,   59,  378,  428, 1965,  378,    0,  428,
  336,  344, 1363,  339,  344,  509, 1654,  336,  277,   59,
  513,  380,   59,  282,  264, 1306, 1306,   93,   59, 1355,
  389, 1361, 1352, 1364, 1993, 1355, 1356, 1703,   59,  462,
  284,  309,   44,  390,  448, 2004, 1468,  457, 2007,  403,
   59,    0,  320,  297,  380, 2014,   59,  395,    0,   44,
  259, 1699, 1949,  389,  264,  315,  262,   59,  276,  277,
  269, 1351, 1352,  432,  486, 1355, 1356,  327,  264, 1359,
 1360, 1361, 1362, 1363, 1364,  444,  336,  446,  426,    0,
  264,   93,   41,   42,   43,   44,   45,   46,   47,   41,
 1380, 1381,   44,  441,  458,  260,  432, 1387,    0,  390,
   59,   60,   61,   62,   44,    0,  264,   59,  444,  315,
  446,  333,  318, 1360,  390,  313,  314,  446,  450,  325,
  380,  327,  410,   42,   43,  331,   45,  260,   47,  389,
  336,    0,   91,  339,  258,  300, 1587,  261,   59,  315,
  390,  419,  318,    0,  513,  477,  485,  486,  354,  325,
    0,  327,  434,   93,  372,  331,  277,   59, 1834,  428,
  336,  282,  368,  339,   59, 1448,  509,  300,  338,  509,
  376,  409,  432,  379,  380,  393,    0,  513,  354,  303,
  390,  399, 1481,  389,  318, 1636,  446, 1486,    0,  298,
   59,  260,  368,  315,  390,    0,  317,  331,  436,  405,
  376,  371,   59,  379,  380,  327,  390,  446,  260,   59,
  276,  277,  259,  389,  336,  433,  352,  264,  284, 1566,
  354,  427,  269,  271,    0,  334,  432,  397,  273,  405,
  400,  300,  390, 1524, 1524,   59,   41,  285,  444,  513,
  446,   41,  450,    0,   44,    0,  416,   59,  300,    0,
  386,  427,  388,  513,   59,  309,  432, 1587,  380,  453,
  454,   42,   43,    0,   45,  409,   47,  389,  444,  477,
  446,  405,  442,  260,  410,    0, 1566,   42,   43,  333,
   45,  329,   47,   59, 1574, 1575, 1576,  493,  342,  452,
  399,    0,  436,   42,   43,   40,   45, 1587,   47,  435,
  409,    0,   59,  262,   59,    0,  372,  513,   59,  357,
  432,  517,  271,  300,  416,  303,   41,  493,  277,   44,
  520, 1772,   59,  282,  446,  434,  285,  393,  338, 1619,
 1620,  264,   41,  399,   59,   44,  333,  513,  313, 1790,
    0,  517,   41,  390,  398,   44, 1636, 1390,  402, 1392,
   59,  315, 1787, 1643, 1644,  372,  315,  333,  277,  318,
   59,  371,  321,  282,   59,  485,  325,  433,  327,  453,
  329,  368,  331,  421,  332,  495,  393,  336,  337, 1669,
  339,   41,  399,  380,   44, 1669,  333,  397,  258,  308,
  400,  513,  368,  444,   41,  354,  355,   44,  357,   59,
  270,  426,  390, 1742,  380,  403,  416,   42,   43,  368,
   45,  370,   47,  372,  373,  374,  433,  376,  259,  336,
  379,  380,  410,   41,  384,  385,   44,  493,  269,  318,
  389, 1818,  442,  303,  395,  305,   42,   43,   40,   45,
  428,   47,  331, 1960,  317, 1962,  405,  501, 1739, 1739,
   42,   43, 1742,   45,   41,   47,  315,   44, 1788,  513,
  514,  515,  421,   46,  315,  354,   42,   43,  427,   45,
  376,   47,  376,  432,  462,   58,   59,  498,   61,  368,
 1819, 1771, 1933,   41, 1935,  444,   44,  446,   41, 1940,
   41,   44, 1943,   44, 1945,  356,  277,  428, 1788, 1779,
 1790,  282,  426, 1793,   41,    0,    0,   44,   41,  377,
  315,   44,  277,  442, 1965,   41,  405,  282,   44, 1809,
  325,  319,  327,   41, 1814, 1912,   44,  410,  277, 1819,
 1814,  336,  586,  282,  493,  337,  590,  262, 1885, 1990,
   41, 1840, 1993,   44,  375, 1844,   41,   41,   41,   44,
   44,   44,  497, 2004,  513,  336, 2007,   46,  517,   41,
  453,  454,   44, 2014,   59,   59, 1619, 1620,  317,   58,
   59,  336,   61, 1619, 1620,  380, 1866,  276,  277,   41,
  508,  409,   44,  291,  389,  284,   42,   43,  508,   45,
  315,   47,  284,  318,  302, 1885, 1886,  509,  461,  306,
  325,  309,  327,   44, 1887,  313,  331,  446,   61,   41,
 1940,  336,   44,  366,  339,   41,  276,  277,   44, 1949,
  449,  450,  451,   41,  284,  333,   44,  432,  517,  354,
  283,  778,   40,  780,   40, 1965,  264,  784,  785,  786,
   40,  446,  277,  368,   40,  389,   40,  282,  477,  478,
 1940,  376, 1932,   40,  379,  380, 1946,   42,   43, 1949,
   45,  453,   47, 1993,  389,   41,   41,   40,   44,   44,
 1960,  277, 1962,  372, 2004, 1965,  282, 2007,   40,   40,
  405,   40,  317,   40, 2014,  277,   40,  395,   40,  453,
  282,  453,   40,   40,  393,   40,  351,   40,   40,    0,
  399,  277,  427, 1993, 1994,  759,  282,  432,  513,  315,
   40,   40,  372,   40, 2004,   40, 2006, 2007,  426,  444,
   40,  446,  430,   91, 2014,  779,  402,   44,   46,  783,
   61,   40,  308,  393,  433,  264,  790,  395,  317,  399,
   41,  795,  796,   44,   44,  799,  800,  801,  802,  803,
  804,   44,  420,   61,   46,    0,   40,  264,   59,  813,
  337,  815,  816,  817,  818,  819,  820,  262,  493,  823,
   41,   42,   43,  433,   45,  344,   47,   46,   44,  344,
  344,  297,  276,  277,  276,  276,  264,  281,  513,  390,
  284,  365,  517,  390,  493,  428,   41,  344,  450,   44,
   41,   42,   43,  297,   45,  859,   47,  275,  439,  265,
  439,  519,  392,   40,   59,  345,   40,   91,   40,   40,
  315,  277,  264,  318,  376,  879,  282,  315,  403,   44,
  325,  375,  327,  493,  382,   40,  331,  891,  892,   40,
  380,  336,  315,  299,  339,  443,   41,  275,  296,   41,
   42,   43,    0,   45,   63,   47,  428,  428,   40,  354,
  456,   41,   42,   43,  314,   45,   44,   47,  337,  264,
  440,  444,   44,  368,  494,   40,  273,   40,  372,  264,
  297,  376,    0,  281,  379,  380,  340,  276,  340,  379,
  297,  410,  277,   41,  389,  379,   44,  282,  379,  393,
  408,  410,   40,  352,   40,  399,   40,  362,   40,   44,
  405,   59,   41,  317,   41,   42,   43,  277,   45,   44,
   47,  438,   40,   41,   42,   43,   44,   45,   46,   47,
   46,  270,  427,  378,  361,  317,  317,  432,   40,  433,
   58,   59,   60,   61,   62,   42,   43,   44,   45,  444,
   47,  446,   44,  325,  266,   40,   40, 1011, 1012, 1013,
  264,  262, 1016, 1017, 1018,  493, 1020, 1021, 1022, 1023,
 1024, 1025,   41,   91,  414,   93,   58,   41,   41,   41,
 1034, 1035,  264, 1037,  387, 1039,  446,  306, 1042, 1043,
 1044, 1045, 1046, 1047,   42,   43,   44,   45,  493,   47,
  306,  409,   41,   41,   40, 1059,  277, 1061,   40,   40,
  317,  282,   40,   40,  315,   40,   40,  318,  513,   40,
   40,   40,  517, 1077,  325,   40,  327,   40,   40,   40,
  331,  276,  277,   40,   40,  336,  277,  262,  339,  284,
  305,  282,   44,  427,  452,  446,   44,  455,   41,  457,
  458,  459,  460,  354,  462,   41,   42,   43,   41,   45,
  264,   47,  470,  471, 1118,  381,  339,  368,  327,  339,
  273,  384,  284,  481,  482,  376,  484,  399,  379,  380,
  379,  379,  379,  491,  492,  277,  494,  495,  389,  497,
  282,  379,  500,  501,  502,  503,  504,  277,  379,  379,
  508,  509,  282,   41,  405,  372,  514,  515,  516,  450,
  518,  519,  520,   41,  522,  276,   40,    0,   41,   41,
   42,   43,   41,   45,  532,   47,  427,  372,  276,  277,
 1016,  432, 1018,  281, 1020, 1021,  284, 1023, 1024, 1025,
   41,  317,   41,  444,  262,  446,  264,  265,  393,  297,
  277,   44,   41,  271,  399,  282,  305,   59,   41,  277,
  290,   44,   40,  427,  282,  336,  336,  285,  305,   41,
   42,   43,   59,   45,   58,   47,   59,  324,  428,  532,
  277,  299,  339,  277,  334,  282,  304,  305,  433,  333,
  308,  434,  493,  298,  434,  334,   41,  315,  298,  317,
  318,   41,  348,  321,  378,  365,  365,  325,   41,  327,
   41,  329,  513,  331, 1268,   41,  517,   41,  336,  337,
   41,  339,   41,    0,  372,   41,   41, 1281, 1282,  277,
 1284, 1285,   41,   41,  282,   41,  354,  355, 1292,  357,
   41,   41,   41,   41,   41,  393,   40,  315,   41,  443,
  368,  399,  370,   40,  372,  373,  374,  378,  376,   41,
 1314,  379,  380,   44,   41,  348,  340,   44, 1322, 1323,
 1324,  389,  458, 1327,  333, 1329,   40,  500,  336, 1333,
 1334, 1335,   59,  336,  402,  433,   40,  405, 1342,   40,
 1344,  277,   44,   41,   42,   43,  282,   45,  376,   47,
   41,   42,   43,  421,   45,   41,   47,   40,  427,  427,
  428,  376,   41,    0,  432,   41,   42,   43,   41,   45,
  376,   47,  301,  298,  434,  443,  444,   41,  446,  290,
  304, 1385,  301,  455,  452,    0,  333,  455,  409,  457,
  458,  459,  460,  376,  462,   41,   42,   43,   40,   45,
  376,   47,  470,  471,   41,  277,  264,   44,  264,  276,
  282,  450,  317,  481,  482,   40,  484,  443,  403,   40,
   44, 1425,   59,  491,  492,  493,  494,  495,  270,  497,
   40,  264,  500,  501,  502,  503,  504,   41,  270,  450,
  508,  509,  339,  264,  339,  513,  514,  515,  516,  517,
  518,  519,  520,  450,  522,  277,   41,   41,   42,   43,
  282,   45,   41,   47,  532,   59,   41,   41,   42,   43,
   59,   45,    0,   47,   41,   42,   43,  427,   45,   59,
   47,   59,  315,   42,   43,  318,   45,  305,   47, 1493,
   41,   59,  325,   59,  327, 1499,   59,   41,  331,   59,
 1504,   59,  301,  336,    0,    0,  339, 1511,    0,    0,
  161,  581, 1371,   41,   42,   43,   44,   45,   46,   47,
  161,  354,  464,  465,  466,  467,  468,  469,  470,  471,
  849,   59,   60,   61,   62,  368,  162,  583,   41,   42,
   43, 1545,   45,  376,   47, 1549,  321,  380,  320,  276,
  277, 1555,  840, 1243,  743,  991,  389,  284, 1241,   95,
  953,  996,   81,   91, 1238,   93, 1233, 1571, 1686,  966,
  297, 1687,  405,  444,  410, 1223,  690, 1166,  370,  277,
  881, 1234, 1809,  916,  282,  425,  277,  425, 1666, 1952,
 1956,  282,  318, 1668,  667, 1955, 1978, 1107, 1574,  432,
 1776,  277,  907, 1941,  829,  331,  282, 1403, 1650,  936,
  430,  444,  415,  446,   41,   42,   43,  408,   45,  452,
   47,  669,  455, 1457,  457,  458,  459,  460,  354,  462,
   13,  277,  727,  969,  163,  355,  282,  470,  471,  276,
  277, 1645,  368,  603,  961,  372,  599,  284,  481,  482,
  376,  484,   42,   43,  167,   45,  859,   47,  491,  492,
  493,  494,  495,    0,  497,  870,  393,  500,  501,  502,
  503,  504,  399, 1126, 1391,  508,  509,   91, 1739,  405,
  513,  514,  515,  516,  517,  518,  519,  520, 1905,  522,
  891,  614, 1524,  277, 1119,  776, 1283,  847,  282,  532,
 1329, 1412,   -1,  277,   41,  264,  433,   44,  282,   -1,
  277,  318,   -1,   -1, 1718,  282,   -1,   -1,  277,   -1,
 1724, 1725,   59,  282,  331,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  339,   -1,  262,  372,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271, 1748,   -1,   -1,  354, 1752,  277,
   -1,   -1, 1756,   -1,  282,   -1,  393,  285,   -1, 1763,
 1764,  368,  399,    0,   41,   42,   43,  493,   45,   -1,
   47,  299,    0,  301,  277,   -1,  304,  305,   -1,  282,
  308,   -1,   41,   42,   43,   -1,   45,  315,   47,  317,
  318,  517,   -1,  321,   -1,   -1,  433,  325,  405,  327,
   -1,  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62, 1839,   -1,   41,   42,   43,
  368,   45,  370,   47,  372,  373,  374,   -1,  376,   -1,
  277,  379,  380,   -1,   -1,  282,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   91,   -1,   93,   -1,  260,   -1,   41,
   42,   43,  262,   45,  402,   47,   -1,  405,    0,   -1,
   -1,   -1,   -1,   -1,   41,   42,   43,  277,   45,   -1,
   47,   -1,  282,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428, 1905,   -1,   -1,  432,  298,   -1,   -1,   -1,   -1,
  517,   -1,   -1,   -1,   -1,  443,  444,  310,  446,   41,
   -1,   -1,   44,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   59, 1942,  276,
  277,  334,  470,  471,   41,   42,   43,  284,   45,   -1,
   47,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
  297,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   41,   42,   43,
  277,   45,   -1,   47,  532,  282,  399,   -1,   41,   42,
   43,   -1,   45,   -1,   47,   -1,  409,   -1,  277,   -1,
   -1,  258,   -1,  282,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,  372,  264,  265,  275,  276,
  277,  434,   -1,  271,  281,   -1,   -1,  284,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,  393,  285,   -1,   -1,
  297,   -1,  399,    0,   -1,   -1,  303,   -1,   -1,   -1,
   -1,  299,    0,  277,   -1,   -1,  304,  305,  282,   -1,
  308,   -1,   41,   42,   43,   -1,   45,  315,   47,  317,
  318,   -1,   -1,  321,   -1,   -1,  433,  325,   -1,  327,
   -1,  329,   -1,  331,   41,  277,   -1,   44,  336,  337,
  282,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
  277,   -1,   59,   -1,   61,  282,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   42,   43,
  368,   45,  370,   47,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,  276,  277,  393,   -1,   -1,   -1,
   -1,  389,  399,   91,  291,   93,   -1,   -1,   -1,   -1,
   -1,  408,   -1,  410,  402,  302,   -1,  405,   -1,   -1,
  277,   -1,  309,   -1,   -1,  282,   -1,   -1,   -1,   -1,
   -1,  428,   -1,  421,   -1,  322,  433,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,  462,   -1,  455,   -1,  457,
  458,  459,  460,  277,  462,   -1,   -1,   -1,  282,   -1,
   -1,   -1,  470,  471,  277,   -1,   -1,   -1,   -1,  282,
  372,   -1,  379,  481,  482,   -1,  484,   42,   43,   44,
   45,   -1,   47,  491,  492,  493,  494,  495,  395,  497,
   -1,  393,  500,  501,  502,  503,  504,  399,   -1,   -1,
  508,  509,   -1,   -1,  411,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,  426,
   -1,   -1,   -1,  430,  532,   -1,  433,   -1,   -1,   -1,
   -1,  433,   -1,   -1,  441,   -1,   -1,   -1,  277,   -1,
   -1,  258,   -1,  282,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,  262,   -1,  264,  265,  275,  276,
  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,  262,   -1,
  297,   -1,   -1,    0,   -1,   -1,  303,   -1,   -1,   -1,
   -1,  299,    0,  277,   -1,   -1,  304,  305,  282,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,  519,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,  393,   -1,   -1,   -1,
   -1,  389,  399,   91,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  410,  402,  302,   -1,  405,   -1,   -1,
   -1,   -1,  309,   -1,   -1,   -1,  313,  314,   -1,   -1,
   -1,  428,  277,  421,   -1,  322,  433,  282,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,  333,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,  462,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,  395,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,  411,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,  426,
   -1,   -1,   -1,  430,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,
   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,  262,   -1,  264,  265,   -1,  276,
  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,  519,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,  393,   -1,   -1, 1494,
 1495,  389,  399,   91,   -1,   93,   -1, 1502, 1503,   -1,
 1505, 1506,   -1,  410,  402,   -1,   -1,  405, 1513, 1514,
 1515, 1516,   -1, 1518, 1519, 1520, 1521, 1522, 1523,   -1,
   -1,  428,   -1,  421,   -1,   -1,  433,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,  462,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   59,   -1,   61,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   91,   -1,   93,   -1,   -1,   -1,   -1,
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
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,  258,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,  273,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,  306,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,    0,  348,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,  403,  357,
   -1,   59,   60,   61,   62,  410,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,  428,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,  458,   -1,   -1,   -1,  462,   -1,   -1,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   45,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   93,  292,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  421,  320,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,  348,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,  419,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,   -1,  453,  454,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,  472,  473,  474,  475,
  476,  477,  478,  479,  480,   -1,   -1,  285,   -1,   -1,
   -1,  487,  488,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  496,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,
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
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,  288,  357,
   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  402,  335,   -1,  405,   -1,   -1,
   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,  349,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,  367,   -1,  369,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,  398,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,  415,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,  437,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,  398,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  415,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,  437,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  489,   -1,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   41,  331,   43,   44,   45,   -1,  336,  337,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,    0,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   59,   -1,
  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   41,
  331,   43,   44,   45,   -1,  336,  337,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   -1,
   45,  402,   47,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   60,   61,   62,   -1,   -1,
  421,   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,   -1,
   40,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   58,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,    0,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,  522,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,
   -1,  532,   -1,   -1,  325,   -1,  327,   40,   41,   -1,
  331,   44,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,
  262,   -1,  264,  265,   -1,   -1,   59,   -1,   -1,  271,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  285,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  299,  379,  380,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,  389,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,  405,  327,   -1,  329,   -1,  331,
   -1,   -1,   -1,   -1,  336,  337,  271,  339,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,  427,  282,   -1,   -1,
  285,  432,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  321,  389,   -1,   -1,
   -1,   -1,   -1,   -1,  329,   -1,   -1,   -1,   -1,   -1,
  402,   -1,  337,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,    0,  421,
  355,   -1,  357,   -1,   -1,  427,  428,   -1,   -1,   -1,
  432,   -1,  513,   -1,   -1,  370,  517,  372,  373,  374,
   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   41,
  462,   -1,   44,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,  421,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
  522,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
  532,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,  354,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,  481,  482,  376,  484,   -1,   -1,  380,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,  389,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,  405,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,    0,   -1,   -1,
  513,  514,  515,  516,  517,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  532,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,    0,   -1,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   91,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   91,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,    0,   -1,
   -1,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  532,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,  271,   -1,
   -1,   -1,   -1,   -1,  277,   -1,   -1,   41,   -1,  282,
   44,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  321,    0,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,
   -1,   -1,   -1,  336,  337,   -1,  339,   -1,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,
   -1,  354,  355,  277,  357,   -1,   -1,   -1,  282,   -1,
   41,  285,   -1,   44,   -1,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,  315,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
   -1,  325,  405,  327,   -1,  329,   -1,  331,   -1,   -1,
   -1,   -1,  336,  337,   -1,  339,    0,   -1,  421,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,
  354,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,   -1,  379,  380,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   -1,   -1,   -1,  421,  262,    0,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,
  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,   -1,   -1,
  444,   -1,  446,  315,   -1,   -1,  318,    0,   -1,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   41,   -1,   -1,   44,  336,   -1,   -1,  339,   -1,   -1,
   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   59,   -1,
   -1,  325,  354,  327,   -1,   -1,   -1,  331,   41,  493,
   -1,   44,  336,   -1,   -1,  339,  368,   -1,   -1,   -1,
   -1,  262,   -1,   -1,  376,   -1,   59,  379,  380,  513,
  354,   -1,   -1,  517,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,  405,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,  427,   -1,  318,   -1,   -1,
  432,  405,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,  444,   -1,  446,  336,   -1,   -1,  339,    0,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,  262,   -1,
  444,   -1,  446,   -1,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   41,  493,   -1,   44,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   -1,  513,   -1,   -1,  405,  517,   -1,   -1,   -1,  493,
   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,    0,   -1,
   -1,  325,   -1,  327,   -1,   -1,  427,  331,   -1,  513,
   -1,  432,  336,  517,   -1,  339,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,
  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,  368,   -1,   -1,   -1,   -1,  262,
   -1,   -1,  376,   -1,   -1,  379,  380,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,  493,   -1,  315,   -1,   -1,  318,   -1,   -1,
   -1,  405,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,  513,   -1,   -1,  336,  517,   -1,  339,   -1,
   -1,   -1,  315,  427,   -1,  318,   -1,   -1,  432,   -1,
   -1,   -1,  325,  354,  327,   -1,   -1,   -1,  331,   -1,
  444,   -1,  446,  336,   -1,   -1,  339,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,    0,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,  405,   -1,  379,  380,   -1,  493,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,  262,   -1,    0,   -1,   -1,  427,   -1,   -1,  513,
   -1,  432,  405,  517,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,  427,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   41,    0,   -1,   44,   -1,   -1,
   -1,  444,   -1,  446,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,   59,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,  493,   -1,   -1,  336,   -1,   -1,  339,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   41,   -1,  261,   44,
   -1,   -1,  513,  354,   -1,   -1,  517,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   59,   -1,  279,  368,   -1,   -1,
   -1,   -1,   -1,   -1,  287,  376,   -1,   -1,  379,  380,
  513,  294,   -1,  296,  517,  298,   -1,   -1,  389,   -1,
  303,   -1,   -1,  315,   -1,   -1,  318,  310,   -1,   -1,
   -1,   -1,   -1,  325,  405,  327,   -1,   -1,   -1,  331,
   -1,  324,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,  334,   -1,   -1,   -1,   -1,  427,    0,   -1,   -1,
   -1,  432,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,  389,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  405,  397,   -1,   59,   -1,   -1,   -1,
   -1,  404,  493,   -1,  407,  262,  409,  410,   -1,   -1,
   -1,   41,    0,   -1,   44,  427,   -1,   -1,   -1,   -1,
  432,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   59,
   -1,  434,  444,   -1,  446,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   59,   -1,   -1,  331,   -1,   -1,  262,   -1,  336,
  483,  493,  339,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,  498,  499,  354,  325,   -1,
  327,  513,   -1,   -1,  331,  517,    0,   -1,   -1,  336,
   -1,  368,  339,   -1,   -1,   -1,   -1,   -1,   -1,  376,
  523,   -1,  379,  380,   -1,   -1,   -1,  354,   -1,   -1,
  315,   -1,  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  325,  368,  327,   -1,   -1,   -1,  331,   41,  405,  376,
   44,  336,  379,  380,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   59,   -1,   -1,   -1,  354,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,  405,   -1,
   -1,    0,   -1,  368,   -1,   -1,   -1,  444,   -1,  446,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  262,
  427,   -1,   -1,   -1,  389,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
  405,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,  493,   -1,   -1,   -1,
   59,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,  513,   -1,   -1,  444,
  517,  446,  325,   -1,  327,   -1,  493,   -1,  331,   -1,
    0,   -1,   -1,  336,  262,   -1,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  315,  513,   -1,  318,   -1,
  517,  354,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,    0,   -1,  368,  336,   -1,  493,  339,
   -1,   41,   -1,  376,   44,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,  354,   -1,  389,  315,  513,   59,
  318,   -1,  517,   -1,   -1,   -1,   -1,  325,  368,  327,
   -1,   -1,  405,  331,   41,    0,  376,   44,  336,  379,
  380,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   59,   -1,  427,   -1,  354,   -1,  262,  432,
   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,
  368,  444,   -1,  446,   -1,   40,   41,   -1,  376,   44,
   -1,  379,  380,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,  389,  432,   -1,   59,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  446,  405,   -1,   -1,
   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,
  493,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  427,
   -1,   -1,  336,  262,  432,  339,   41,   -1,   -1,   44,
  513,   -1,   -1,   -1,  517,   -1,  444,   -1,  446,   -1,
  354,   -1,   -1,  493,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,  513,   -1,  379,  380,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   -1,  493,  325,   -1,  327,   -1,
   -1,  405,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,  513,   -1,   -1,   -1,  517,
   -1,   -1,  262,  427,   -1,  354,   -1,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
  444,   -1,  446,   -1,   -1,   -1,   -1,  376,   -1,   40,
  379,  380,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  315,  405,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,  493,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,  427,  339,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,  315,  513,
   -1,  318,   -1,  517,  354,  444,   -1,  446,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,  368,  336,
   -1,   -1,  339,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,  389,
  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  325,  368,  327,   -1,  493,  405,  331,   -1,   -1,  376,
   -1,  336,   -1,  380,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,  513,   -1,   40,  427,  517,  354,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,  368,  444,   -1,  446,   -1,   -1,   -1,
  315,  376,   -1,  318,   -1,  380,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,  389,  432,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,  444,   -1,  446,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,   -1,  432,   -1,   -1,
   -1,  376,   -1,  513,   -1,  380,  267,  517,   -1,  444,
   -1,  446,   -1,  274,  389,   -1,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
  405,  292,  293,   -1,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,  432,  493,  320,
   58,   -1,  323,   -1,   -1,   63,   -1,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,  513,   -1,
   -1,   -1,  517,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  513,   -1,
   -1,   -1,  517,   -1,   -1,   -1,   -1,   -1,  409,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,  281,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  505,  506,  507,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,  524,  525,  526,   -1,  528,  529,  267,
  531,  532,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,  289,   -1,   -1,  292,  293,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  311,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,  372,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   42,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,  267,  524,  525,  526,   -1,
  528,  529,  274,  531,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,
   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
  372,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,  293,   -1,  425,   -1,   -1,   -1,   42,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  267,  524,  525,  526,   -1,  528,  529,  274,  531,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,
  492,   -1,  494,  495,   -1,  497,  372,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  532,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,  267,  524,  525,
  526,   -1,  528,  529,  274,  531,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   58,   -1,   -1,  452,   -1,   63,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
  360,   -1,   -1,   -1,  364,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   44,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,  521,  522,   -1,  524,  525,  526,  527,  528,  529,
  530,  531,  532,  267,   -1,   -1,   42,   43,   -1,   45,
  274,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   60,   61,   62,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,  311,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
  450,   58,  452,   -1,   -1,  455,   63,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,
  364,  491,  492,   -1,  494,  495,   -1,  497,  372,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   58,   -1,  317,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,  271,  500,  501,  502,  503,
  504,  277,   -1,   -1,  508,  509,  282,   -1,   -1,  285,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
  524,  525,  526,  260,  528,  529,  263,  531,  532,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  321,   -1,   -1,   -1,  286,
   -1,   -1,   -1,  329,   -1,  292,  293,   -1,   -1,   -1,
   -1,  337,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,  355,
   -1,  357,   -1,  320,   -1,   -1,  323,   -1,   58,   -1,
   -1,   -1,   -1,   63,  370,   -1,  372,  373,  374,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,  348,   -1,  350,  351,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,  421,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,
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
   -1,  518,  519,  520,   -1,  522,   -1,  524,  525,  526,
  260,  528,  529,  263,  531,  532,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   58,  452,   -1,   -1,  455,
   63,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,  524,  525,  526,  260,  528,  529,
  263,  531,  532,   -1,  267,  272,   -1,   -1,  275,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  288,   -1,  286,   -1,  292,   -1,   -1,  295,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   58,   -1,   -1,   -1,   -1,   63,  335,   -1,
   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
  367,  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,  403,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,  472,  473,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,  524,  525,  526,  260,  528,  529,  263,  531,  532,
   -1,  267,  272,   -1,   -1,  275,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,  292,   -1,   -1,  295,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   58,
   -1,   -1,   -1,   -1,   63,  335,   -1,   -1,   -1,   -1,
   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,  349,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,  364,  369,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,  472,  473,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,  489,
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
  459,  460,   -1,  462,  463,   -1,   -1,  325,   -1,  472,
  473,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,  489,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,  524,  525,  526,  260,  528,
  529,  263,  531,  532,   -1,  267,   -1,  395,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  430,   -1,   -1,   40,   -1,  435,   43,   -1,
   45,   -1,   -1,  441,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   58,  452,   -1,   -1,  455,   63,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,  524,  525,  526,  260,  528,  529,  263,  531,
  532,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,  435,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
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
  425,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   58,   -1,  323,   -1,   -1,   63,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,  360,   -1,   -1,   -1,  364,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,  284,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,  260,  524,  525,  526,   -1,
  528,  529,  267,  531,  532,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,   40,   -1,
   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   58,   -1,  323,   -1,
   -1,   63,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,  360,   -1,   -1,   -1,  364,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  515,  516,   -1,  518,  519,  520,   -1,  522,  260,  524,
  525,  526,   -1,  528,  529,  267,  531,  532,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   40,   -1,   42,   43,  300,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,
   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  267,  524,  525,  526,   -1,  528,  529,  274,  531,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,  267,  524,  525,
  526,   -1,  528,  529,  274,  531,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
  320,   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,  267,  524,  525,  526,   -1,  528,  529,
  274,  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   40,   41,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,
  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,  409,   -1,   -1,   -1,   -1,
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
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,  267,
  524,  525,  526,   -1,  528,  529,  274,  531,  532,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  518,  519,  520,   -1,  522,  267,  524,  525,  526,   -1,
  528,  529,  274,  531,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,
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
   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  267,  524,  525,  526,   -1,  528,  529,  274,  531,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,  317,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,
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
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,  267,  524,  525,
  526,   -1,  528,  529,  274,  531,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
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
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,  267,  524,  525,  526,   -1,  528,  529,
  274,  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,
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
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,  267,
  524,  525,  526,   -1,  528,  529,  274,  531,  532,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  518,  519,  520,   -1,  522,  267,  524,  525,  526,   -1,
  528,  529,  274,  531,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,
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
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,    0,
  522,  267,  524,  525,  526,   -1,  528,  529,  274,  531,
  532,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   40,   41,   -1,  320,   44,   -1,  323,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   41,
   91,   -1,   44,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   91,   -1,   -1,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
    0,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,  258,  524,  525,
  526,   41,  528,  529,   44,  531,  532,   -1,   -1,  258,
   -1,   -1,  273,   -1,  275,  276,  277,   -1,    0,   59,
  281,   61,   -1,  284,  273,   -1,  275,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  306,  258,   -1,  297,   -1,
   -1,   91,   -1,   -1,  303,   -1,   -1,  306,   -1,   41,
   -1,  273,   44,  275,  276,  277,   -1,   -1,   -1,  281,
   -1,   -1,  284,   -1,   -1,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,  297,   -1,  348,   -1,   -1,
   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,  348,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,
   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,  275,   -1,
   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,  393,   -1,  291,   -1,  348,   -1,  399,   -1,
   -1,   -1,  403,   -1,  393,  302,   -1,   -1,   -1,  410,
  399,   -1,  309,   -1,  403,   -1,  313,  314,   -1,   -1,
  372,  410,   -1,   -1,   -1,  322,   -1,  428,   -1,   -1,
   41,   -1,  433,   44,   -1,   -1,  333,   -1,   -1,  428,
   -1,  393,   -1,   -1,  433,   -1,   -1,  399,   59,   -1,
   61,  403,   -1,   -1,   -1,   -1,   -1,  458,  410,   -1,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,
   -1,   -1,   -1,  462,   -1,   -1,  428,   -1,  258,   -1,
   91,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  387,   -1,   -1,  273,   -1,   -1,  276,  277,  395,   -1,
   -1,  281,   -1,   -1,  284,   -1,  458,   -1,   -1,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,
   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,  426,
   -1,   -1,   -1,  430,   -1,   -1,  258,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,
   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,  281,
   -1,   -1,  284,  460,  461,   -1,   -1,   -1,  348,   -1,
   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,   -1,   -1,
   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,   -1,  399,
   -1,   -1,  519,  403,   -1,  522,  348,   -1,   -1,   -1,
  410,   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  428,   -1,
  372,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,
  281,  393,  298,  284,   -1,   -1,   -1,  399,  458,   -1,
   -1,  403,  462,   -1,  310,   -1,  297,   -1,  410,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,  334,   -1,
  260,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  348,   -1,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  310,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  399,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  393,  409,  334,   -1,   -1,   -1,  399,   -1,
   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,  458,   -1,  399,
   -1,  462,   -1,   -1,   -1,  481,  482,   -1,  484,  409,
   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,  434,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,  270,   -1,
   -1,   -1,  452,   -1,   -1,  455,  532,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,  298,   -1,   -1,   -1,
   -1,  481,  482,  305,  484,   -1,   -1,  309,  310,   -1,
  312,  491,  492,  315,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,  328,   -1,  508,  509,
   -1,   -1,  334,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,  391,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,
   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  424,  309,  310,   -1,  312,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,  328,  445,   -1,   -1,   -1,   -1,  334,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,  391,  508,  509,  510,  511,
  512,   -1,  514,  515,  516,   -1,  518,  519,  520,  270,
  522,   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  309,  310,
   -1,  312,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,  328,   -1,   -1,
   -1,   -1,   -1,  334,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
  391,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  424,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,  272,   -1,   -1,  275,   -1,  470,
  471,   -1,   -1,  281,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,  292,   -1,   -1,  295,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,
  511,  512,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
   -1,  532,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,
   -1,  369,  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,
  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,  437,
   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,  349,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,  367,   -1,  369,
   -1,   -1,  470,  471,  472,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,  489,   -1,  491,  492,   -1,  494,  495,  398,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  415,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,  437,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,  339,  272,   -1,   -1,  275,   -1,
  470,  471,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  290,  484,  292,   -1,  362,  295,  489,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,  335,   -1,
   -1,   -1,  532,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,  418,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  367,   -1,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,  398,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
  437,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  489,  455,  456,  457,  458,  459,  460,  461,
  462,  463,  464,  465,  466,  467,  468,  469,  470,  471,
  472,  473,  474,  475,  476,   -1,   -1,  479,  480,  481,
  482,  483,  484,   -1,   -1,  487,  488,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  498,  499,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,
  512,  513,  514,  515,  516,  517,  518,  519,  520,  521,
  522,  523,  524,  525,  526,  527,  528,  529,  530,  531,
  532,  452,   -1,   -1,  455,  456,  457,  458,  459,  460,
   -1,  462,  463,  464,  465,  466,  467,  468,  469,  470,
  471,  472,  473,  474,  475,  476,   -1,   -1,  479,  480,
  481,  482,  483,  484,   -1,   -1,  487,  488,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  498,  499,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,  517,  518,  519,  520,
  521,  522,  523,  524,  525,  526,  527,  528,  529,  530,
  531,  532,
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
"returning_clause : RETURNING column_list",
"returning_clause : RETURNING column_list INTO variable_list",
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
case 299:
{ yyval = make_node (nod_proc_stmt, 1, yyvsp[-1]); }
break;
case 300:
{ yyval = make_node (nod_proc_stmt, 1, yyvsp[0]); }
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
			yyval = make_node (nod_current_time, 0, NULL);
			}
break;
case 953:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 954:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 956:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 958:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 959:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 960:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 961:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 962:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 963:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 965:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 966:
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
case 967:
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
case 968:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 969:
{ yyval = make_parameter (); }
break;
case 970:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 971:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 972:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 973:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 974:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 975:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 976:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 977:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 978:
{ yyval = yyvsp[0]; }
break;
case 979:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 981:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 982:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 983:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 984:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 985:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 987:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 988:
{ yyval = yyvsp[0];}
break;
case 992:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 993:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 994:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 995:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 996:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 997:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 998:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 999:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 1000:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 1001:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 1002:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 1005:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 1009:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1010:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1011:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1012:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1015:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 1016:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
break;
case 1017:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 1018:
{ yyval = yyvsp[0]; }
break;
case 1019:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 1020:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
break;
case 1021:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
break;
case 1022:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
break;
case 1023:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
break;
case 1024:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
break;
case 1025:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
break;
case 1026:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
break;
case 1027:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1028:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 1029:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 1032:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 1033:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 1034:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1037:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1038:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1039:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1040:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1041:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1042:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1043:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1044:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1048:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			}
break;
case 1049:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 1050:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 1051:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 1052:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 1053:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 1054:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 1055:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 1056:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 1057:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 1062:
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
