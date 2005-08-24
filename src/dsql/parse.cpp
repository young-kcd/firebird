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
static short yysindex[] = {                                  16911,
 2695, -130, 3420,  100,  685,    7, 3426, -139,17015,  448,
   76,  -24, -130,  881,  672,12220, 1034,12220,  -97,   54,
    0,  636,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  411,    0,    0,    0,    0,    0,    0,  -72,
    0,    0,    0,    0,    0,    0,    0,17958,12220,  503,
12220,12220,12220,12220,12220,    0,    0,  489, 1182,12220,
12220,12220,  781,12220,  614,12220,12220,    0,12220,12220,
12220,    0,  935,    0,  653,    0,    0,  838,12220,    0,
12220,12220,12220,  879,12220,12220,12220,12220,  614,12220,
12220,12220,12220,12220,    0,12220, 1224,  742,    0,    0,
    0, 1247,    0, 1247,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  922,  953,  147, 1357,
    0,    0,    0,    0,    0,12220,12220,12220, 1042, 1055,
 1194,12220,  771,   98,  179,  994,  976,    0,12220, 1143,
  774,    0,    0,12220, 1113,12220,12220,12220,12220,    0,
    0,    0,12220, 7688,    0, 1097, 1093, 1272, 1145, 1044,
 1154,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  861,    0,    0, 1182,    0,    0, 1109,   70,    0,
    0, 1536,    0,    0, 1191,    0,   99,    0,    0, 1061,
    0,    0, 1190,    0,    0, 1167,    0,    0,  880,    0,
 1385,    0,    0,    0,    0, 1182,    0,    0,  692, 1536,
    0,  450,    0,    0, 1342,    0, 1346,    0, 1247,    0,
    0,    0,    0,    0,    0,    0, 1338,    0,    0, 1109,
    0, 1354,    0, 1254,    0,    0,    0, 1109,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,15977,12220,
 1301,    0,12220,    0,    0,    0, 1252, 1326,11953,12220,
 1343, 1379, 1691, 1416, 1434, 1440, 1369, 1384,  163, 1264,
    0,    0,16233,    0,    0,    0,    0,    0,    0,    0,
    0,13929,    0,    0,    0, 1353,12220, 1408, 1332,  682,
12220, 1356,    0,    0,  774,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12220,    0, 1182, 1536,    0, 1342,
    0, 1247,    0, 1406, 1377,    0,    0,    0, 1476,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1472,12220,12220, 1448, 1316,    0,16233, 1423,    0,
 1423,16233,    0, -121, 1351, 1359, 1352, 1154,    0, 1588,
  912,    0,12220,    0,  861,    0,    0,    0, 1572,    0,
    0,    0,12220, 1301,10885, 1425,13022, 1844,    0,17958,
    0,    0, 1341, 1443,    0,    0,    0, 1830, 1830, 1182,
 1529, 1182,  427,  880,    0,    0, 7886,    0,12220,12220,
12220,    0, 1301,    0,    0, 1621,  578, 1861,12220, 1648,
12220,    0, 1873, 9851, 9977, 1526,    0,    0, 1876, 1877,
 1878, 1466,    0,    0, 1888,    0, 1891,    0, 1895, 1900,
    0, 1901,    0,    0,    0,    0,    0, 1903, 1480, 1491,
    0, 1906, 1906,    0,    0,    0,    0, 1907,    0,    0,
14185, 1908, 1910,    0, 1911, 1611, 1924, 1925, 1927, 1928,
 1929, 1932,16233,16233,14441,12220, 1933,    0, 2130,    0,
    0,    0,    0,    0,    0,    0, 1886, 1576, 1935,    0,
    0, 1934,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 7886, 1920,  919,    0, 1943,
 1720, 1459,    0,    0,12220,12220,12220,  115,    0,    0,
    0,    0,    0,12220,12220,  856,    0,12220,    0,    0,
 1252, 1592,11953,    0,12220,14441, 1912,    0,    0,16233,
    0, 1828, 1674, 1948,    0,    0,   95,    0,  569,    0,
  -93,    0,  -93,    0,    0, 1949,    0,  177, 1574,    0,
    0,    0, 1950,  141, 1944,    0, 1301, 1964, 1744,    0,
    0,    0,    0,  639,    0, 1675, 1967,12220,    0,    0,
 2011, 1970,    0,    0,    0,  429,    0, 1671, 1673,    0,
 1182,    0, 1677,    0,    0, 1742, 1746,    0,    0,    0,
 1732,    0, 7886,    0, 1182, 1660, 7886, 1534,    0, 1769,
12220,17713,    0,    0,  -31,    0,12220,  622, 1191, 1645,
 1609,  483,    0,    0,    0,    0,  402, 1652,  235,    0,
 1594,  614,    0, 1182,    0, 1770, 1704,    0,    0,  427,
    0,    0,  112, 1612,    0,    0,    0, 1613,    0, 1663,
 2020,    0, 1716, 2023,  748,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1976,  592,    0,
    0,    0,    0,  204, 2026, 2028, 2023,    0, 1182, 1536,
    0, 1758,    0, 1805,    0, 1182, 1182,    0,10885,   99,
    0, 1701,    0, 1681, 2036,    0, 5793,    0, 1703,    0,
    0, 1721, 2055,10233, 2058,    0,    0,    0, 9721, 2046,
  -28,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  665, 1724, 1255,16233,  175,
    0,12220,16233, 1273, 1277, 1287, 2642,    0,    0,  614,
    0,    0,16233,10489, 2130, 1657,    8,16233,16233,10489,
 1786,16233,16233,16233,16233,16233,10745,  883,  883, 2029,
 1602,  -72, 2070,    0,    0,14697,12220,16233,16233,16233,
16233,16233,16233, 2344,    0,16233, 7220,    0,    0, 1837,
 2050,12220,    0,12220, 1819,12220,    0, 1693, 1698,    0,
    0, 1666,11953,    0, 1803,    0, 1618, 2093,    0, 1803,
12220,12220, 2090,    0, 2029,    0, 2310,    0,12220, 7429,
 1692,16233,    0,  989,    0,    0,  -44,    0, 1697,    0,
    0,    0,12220, 1036,12220,    0,    0,12220,12220, 1526,
    0,16233, 1874,10885,    0,    0,    0,    0,  639,12220,
 1808, 2095, 1646,16233,16233, 1182,    0,  527,    0,    0,
    0, 2103,    0,    0,    0,    0, 1182,    0,  370,12220,
    0,    0,    0,    0, 1875, 2107,    0, 1860, 1879,    0,
    0, 1821, 1824, 1247, 1882,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  614,12220, 1862, 1732,17811,
 1787, 1791, 1797,    0,  614,    0,    0, 1147,    0, 1030,
    0,    0, 1755, 1182,    0, 9851,  133, 1760,    0,    0,
    0,  614,    0, 2020,  109,    0,    0,    0, 1860,  109,
 1771,  109,    0, 2139,  614,  614,  614,    0,    0, 1301,
12220,    0, 1834,    0, 1635,    0,    0,    0, 1341, 1423,
12220, 1639,  159,    0,    0, 2140,   42,    0, 9851,12220,
 1423,11011,    0,    0, 1423,10233,    0,    0,    0,10817,
    0,  165,   47,16233,16233,11278, 2148,  820,11545,16233,
11812,  996,12079,12346,14953,12613,12880,13147,10489,10489,
    0,    0, 1829,    0, 2150, 1044,16233,16233, 1985,16233,
 2151,16233, 2149, 2429,16233,16233,16233,16233,16233,16233,
    0,    0,    0,    0,    0,    0,    0,    0, 1880, 2157,
 1521,   12,16233,  535,16233,    0,10489, 2298, 2352,  183,
12220, 2437, 2445, 2608, 2735, 2750,    0,    0,    0, 1549,
15209,    0,    0, 1044,    0,    0, 1644,    0,    0, 1923,
  883,  883,  601,  601,  650,10107, 2158,    0,    0, 2161,
 2130,    0,    0,    0,    0,    0, 1661,    0, 1765, 1939,
 1819,    0,    0,11686,11686, 1826,    0,    6, 1849,12220,
    0,16233, 1849, 1896, 1902,    0,    0,    0,    0, 2422,
 2177, 2178, 1319,    0,    0,    0,    0,    0,    0,    0,
10489, 1898,    0,    0,    0,    0,    0,    0,    0,  -52,
    0,    0,    0, 1724, 2130,    0, 1678, 1423,    0,    0,
  639,    0,    0,    0, 2130,    0, 1830, 1955,    0,  527,
    0, 2187,10489, 5227,    0,    0,    0,    0,    0,    0,
    0, 1819,    0,    0,    0,    0,    0,    0,    0, 1247,
 1247, 1738, 2196,    0,    0,    0,    0,    0,    0,    0,
  -12,  531,  763,    0, 1973,    0,    0,    0,    0,12220,
    0,    0,    0,    0,  100, 1684, 1827, 1837, 2197,    0,
 1594, 1702,    0, 2182,  793,    0,12220, 1089,  614, 2204,
 2205, 2207, 1986,   99, 1819, 1830, 1673,10885,    0, 1652,
 1807, 6983,    0,  614, 1867, 1951, 5793,    0,    0,  614,
  312,    0,    0, 1952,    0,    0, 2214, 2219,    0,10489,
10489,    0,10489,10489, 2673, 2130, 2223,    0,    0, 2130,
 2224,14441,    0,    0, 1953,    0,  498, 2225, 2130, 2226,
 1511, 2231, 2130, 2235,16233,16233, 2148,16233,15465, 2238,
 2130, 2239, 2240, 2130, 2241,16233, 2130, 2250, 2130, 2251,
 2255, 2130, 2256, 2259, 2130, 2265,    0, 2052,    0,17880,
    0, 2854, 2904,    0, 2915,    0, 2922,16233,    0, 2931,
 2994, 3081, 3096, 3131, 3209,16233,    0,16233,16233, 2130,
 1881,16233,    0,16233, 2130, 2007,  513,16233,16233,16233,
    0,    0,    0,    0,    0,    0,16233,    0,16233, 1694,
    0,    0,    0, 1706,10107, 9483,12220,    0,    0,12220,
17520,    0,    0,12220,12220,12220,12220,12220,12220, 1869,
 2272, 2275,    0,    0,    0, 1869,    0,  113,  113,  192,
    0,    0, 1728,    0,11686,11686, 2281, 1319, 2283,16233,
 2062, 7429, 1946,    0, 1946,  941, 1946, 1990, 1991, 2059,
 2006,    0,    0,    0, 1044, 1819,    0, 1807,    0, 1128,
 1594, 1954, 1594,    0,10489,  445, 1407,    0,    0,    0,
    0,    0,    0,    0,    0, 5227, 5227,    0, 1939,16233,
 5227, 2060, 1947, 1738,  935,    0,10489,    0,    0,    0,
 1969, 1977, 1980, 1981, 1984, 1987,    0,    0,    0,    0,
    0, 2293,  100,    0, 1899,    0,    0,  109,    0, 1992,
  109,  109,    0,    0, 1837, 2313,    0,    0,    0,    0,
 1341, 1939, 1594,    0, 1673,    0,    0, 2089,    0,    0,
 2095,    0, 2329,    0, 2331, 1182,  -35, 2332,    0,    0,
 1182,    0,    0,    0, 2052,    0, 2052,16233, 1423, 1423,
 1773,  -72, 2336,16233, 2053,    0, 1423, 1423,16233, 1423,
 1423, 3455, 2130,    0, 1754,16233, 2130, 1423, 1423, 1423,
 1423, 2130, 1423, 1423, 1423, 1423, 1423, 1423,12220,    0,
 2337, 2335, 1168,    0,    0, 7886, 2340,    0,    0, 3217,
    0,    0,    0,    0,    0,    0, 3294, 1441,    0,16233,
 2077, 1958,    0,16233, 3306, 1777, 2865, 3349, 3378,16233,
    0,    0,    0,    0, 7886, 2339,    0,    0, 2098,    0,
12220, -107,    0, 2351, 2068,16233,    0,    0,12220,12220,
12220,    0,    0,    0,    0, 2069,    0,    0,    0,    0,
 2106,17385, 2354,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   71, 2357,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2099,    0,11686,11686,    0,    0,    0, 1989,    0,
    0,    0,    0,    0, 1897,    0,    0, 2062,    0, 1789,
12220, 1319,    0,    0,    0, 2086,    0, 7429, 7429,16233,
10489, 1526,    0, 1939,    0,    0,    0,    0, 1030,    0,
    0,  470,    0,    0,    0,    0,    0, 2130,    0, 2154,
 2060,    0, 1032,12220,    0, 2094,  676, 2000, 2101, 2003,
 2142, 2111, 2143, 1819,    0,    0, 2405,    0, 2408, 2102,
 2154, 1992,    0,    0,    0,    0,    0,    0,    0,    0,
 1819, 1652,    0, 1030, 1973, 2076,    0,    0,    0, 2091,
 -102,    0,    0, 1837, 2092, 2130, 2417, 2418,    0, 1044,
    0, 2130,16233, 2423, 2424, 2130, 2425, 2428,16233,16233,
 2130, 2432, 2436, 2440, 2442, 2444, 2448, 2452, 2455, 2456,
 2457,    0,    0,17880, 2461,    0,12220,    0,    0,    0,
    0,    0,16233, 2462,    0,    0,16233,    0,    0,    0,
16233,    0,    0, 3396,  610,    0, 2190,16233,16233,10489,
 2130,    0,    0,    0,    0,12220,10107,    0,  609,    0,
 2072,    0,    0,  392, 2471,    0, 2085,    0,    0,    0,
    0,  113,12220,    0,12220,    0,    0, 7429,    0, 1689,
 2130, 2458,    0,  -28,    0,    0,    0,    0,    0,    0,
    0,    0, 2475,12220,    0,    0, 2164, 2180,12220, 1738,
    0,    0, 1247,12220,    0,    0,    0,    0,    0,    0,
    0, 1939,    0,    0,    0,    0,    0, 1939,    0,    0,
    0,    0,    0,16233, 1182,    0, 2063,    0, 1182,    0,
    0,    0, 2130,    0,    0,    0,    0, 2130, 2130,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12220, 2189, 2130,    0,    0, 3509,    0, 5227, 5227,    0,
    0, 2483, 2130, 1208,  687,    0, 2192, 2158,    0,12220,
12220,  100,  182,    0,    0, 2025, 2193,10489,    0,    0,
  160, 2491,    0,    0,10489, 2492,    0,    0,    0,16233,
    0,    0,    0,    0,    0, 1738, 1247,    0, 2159,    0,
    0,    0,    0, 1973,    0, 2130,    0,    0,    0, 2489,
 2496, 2498,    0,    0,    0,    0, 1423,10107, 2112,10107,
    0,    0,    0,    0,17520,  609,16233,10107,  719,10107,
12220,    0,  -28,12220,    0,    0, 2159,  534, 1738, 2166,
 2168,    0,    0, 2499,12220,    0,12220, 2507, 2158,17520,
 2158,    0,    0, 1297,  169, 2248, 2158,    0, 1793, 1804,
 1738,  647,  647,    0, 2253,    0, 2118,    0,    0,    0,
    0, 2512,    0, 2254,10107, 2264, 2260,17520,12220,    0,
    0,    0, 2100,  787,    0,    0,    0,    0,17520,  208,
12220,17520,    0,    0,    0,    0,    0,    0,17520,    0,
    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  198,   38,    0,    0,    0,    0,    0,    0,    0,
    0,17091,  307,13410,    0,    0,    0,    0,    0,    0,
    0, 2562,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  131,    0,    0,    0,    0,    0,    0,  406,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  320,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2232,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   -6, 2198,    0,    0,
    0,  145,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2200,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  320,    0,    0,15721,13673,    0,    0,    0,
  979,    0,    0,  105,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  454,    0, 2163,  228,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  452,    0,    0,    0,    0,  794,    0,    0,    0,
    0,    0, 1100,    0,    0,    0,    0,    0,  146,    0,
11541,    0,    0,    0,    0,    0,    0,    0,    0,  452,
    0,  456,    0,    0, 2526,    0,    0,    0, 2304,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  123,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  259,    0,
 2318,    0,    0,    0,    0,    0,12487,    0,    0,    0,
    0,    0, 1186,    0,    0,    0,    0,    0,    0,12754,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  573,    0,    0, 1117,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  452,    0, 2526,
    0, 2304,    0, 1124,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  184,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1127,    0, 2308,
    0,    0,    0,    0, 1146,    0,    0,    0,  321,    0,
    0,    0,    0, 2318,  698,18039,    0, 1237,    0,    0,
    0,    0,  458,    0,    0,    0,    0, 2135, 2135,    0,
    0,    0, 1269,  156,    0,    0,    0,    0,    0,    0,
    0,    0, 2318,    0,    0, 1239,    0,    0,    0,    0,
    0,    0,11274,    0,    0,  124,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2725,    0,    0,    0,
    0, 2242, 2242,    0,    0,    0,    0, 3325,    0,    0,
    0, 3625, 3925,    0, 4225, 4525,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  723,    0,
    0,    0,    0,    0,    0,    0, 4825, 1286,  291,    0,
 3025,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1303,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12487,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 9595,    0, 2284,    0,    0,    0,    0,    0,    0,
  542,    0,  542,    0,    0, 1026,    0, 1096, 1211,    0,
    0,    0,    0,  123,    0,    0, 2318,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  299,  419,    0,    0,    0,  425,    0,    0, 1158,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1304,    0,    0,    0,    0,
    0,    0,    0,    0, 2308,    0,    0, 1163,    0,  507,
    0,    0, 1088,    0,    0,    0,    0,  779,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1320,
    0,    0, 1806, 9023,    0,    0,    0,16754,    0,    0,
16803,    0,    0,16975,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2171, 3316,    0,
    0,    0,    0,17033,16766,    0,16975,    0,    0,  452,
    0,    0,    0,    0,    0,    0,    0,    0,  698, 1362,
    0,    0,    0,    0, 2186,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1327, 7815, 8012,    0,    0,
  121,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2558,  150,16489,    0,16489,
    0,    0,    0,16489,16489,16489,    0,    0,    0,    0,
    0,    0,    0,    0, 2160,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6014, 6303, 2556,
    0,   79,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 3578,
    0,    0,    0,    0, 2341,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  171,    0,    0,    0,    0,  171,
    0,    0, 1321,    0,    0,    0,    0,    0,    0,    0,
  697,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  885,    0,    0,    0,    0,    0,  124,
    0,    0,    0,  698,    0,    0,    0,    0,    0,    0,
    0,  273,  369,    0,    0,    0,    0, 1021,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1810,    0,
    0,    0,    0,    0, 2566,    0,    0, 1275, 1802,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  887,  916,  924,    0,    0,    0,    0,    0,    0,  826,
    0,    0,    0,    0,    0,    0,    0, 2233,    0,    0,
    0,    0,    0,16803,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2318,
    0,    0,  187,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 6082,    0,    0,    0,  167,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2567,    0,    0, 1336,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2568,    0,    0,    0,    0,    0, 5125,
 6592, 6881, 5425, 5725,    0,    0, 1337,    0,    0,    0,
  952,    0,    0,    0,    0,    0,    0,    0,13285,    0,
 2349,    0,    0,    0,    0,    0,    0,    0, 1349,    0,
    0,    0, 1349,    0,    0,    0,    0,    0,    0,    0,
 7090,  134,  628,    0,    0,    0,    0,    0,    0,    0,
    0,  729,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  150,  231,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  477,    0, 2135, 2173,    0, 1221,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2341,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1473,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1381,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3016,    0,    0,
    0,    0,    0, 1122,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1362, 2341, 2135, 1410,  698,    0, 2360,
 1430,    0,    0,    0, 6082,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1670,    0,    0,    0, 1893,
    0,    0,    0,    0,    0,    0,    0,    0, 1982,    0,
 6802,    0, 8084,    0,    0,    0,    0,    0,    0,    0,
 8156,    0,    0, 8216,    0,    0, 8311,    0, 8383,    0,
    0, 8455,    0,    0, 8515,    0,    0, 9420,    0, 2558,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2201,
    0,    0,    0,    0,  652,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -34,    0,    0,    0,    0,    0,    0,    0,  218, 1438,
  151,  220,    0,    0,    0, 1438,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2286, 1422,    0,
 7514,    0, 2288,    0, 2288, 2286, 2288,    0,    0,    0,
  995,    0,    0,    0, 1336, 2341,    0, 1430,    0,    0,
    0, 2183,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2184,    0, 1473, 2232,    0,    0,    0,    0,    0,
  971,  988, 1009, 1010, 1019, 1029,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  927,
    0,    0,    0,    0, 3731,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1450,    0,    0,    0,    0,    0,
 1470,    0,    0,    0,    0,    0,  213,    0,    0,    0,
    0,    0,    0,    0,  294,    0,  303,    0,    0,    0,
    0,   79,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 8587,    0, 8671,    0, 8754,    0,    0,    0,
    0, 8826,    0,    0,    0,    0,    0,    0, 1234,    0,
    0, 2589,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2595,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 6643,
 2578,    0,    0,    0,    0,    0,    0, 1733, 1818,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  555,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1614,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1482,    0,    0, 7305,    0,    0,
    0,  710,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  224,    0,    0,    0,    0,    0,    0,  336,    0,
    0,    0,    0,    0,    0,    0,    0, 2597,    0, 1585,
 2380,    0, 2308,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2341,    0,    0,    0,    0,    0,    0,
 1487,  980,    0,    0,    0,    0,    0,    0,    0,    0,
 2341, 2360,    0,  194,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  493,    0, 8886,    0,    0,    0, 2568,
    0, 8997,    0,    0,    0, 9030,    0,    0,    0,    0,
 9119,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2558,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2580,    0,    0, 2581,    0,    0,
 2583,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2343,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1067,    0,    0,    0,    0, 2286,
  961,  817,    0, 1075,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1975,
    0,    0,  122,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  241,    0,    0,    0,
    0,    0, 9191,    0,    0,    0,    0, 9290, 9323,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1811, 2604,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2587, 2600,    0,    0,    0, 2601,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1510, 9517,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1975, 1258,    0,  188,    0,
    0,    0,    0,    0,    0, 2597,    0,    0,    0, 2621,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  562,    0,    0,    0,    0,    0,
    0,    0, 9550,    0,    0,    0, 1436,    0, 1473, 1481,
 1763,    0,    0,    0,    0,    0,    0,    0, 2606,  562,
 2609,    0,    0,    0, 2366,    0, 1527,    0,    0,    0,
 1975,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,17269,    0,    0,    0,  562,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  562,    0,
    0,  562,    0,    0,    0,    0,    0,    0,  562,    0,
    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, 2671,    0,    0,
 2672, 2676,    0,    0,    0,    0,    0,    0,    1,    0,
 2677,   -2, 2517, 2108,   22, -671, 1305, 2518, 1831, 2511,
 2104,    0,    0, 2361, -106,   37,    0,    0,    0,    0,
   85,  102,  -59,  -83,  -60, -291, 2364, 1847,    0,    0,
    0,  812,    0,    0,  -33, 1444, -590,  -77,-1104, 1960,
 1699, -377,    0, 1447,    0, 2603, 1708, 1740,    0,    0,
    0, -602,  -58,    0,    0,    0,    0,    0, 2624,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -66,    0, -616, -702, 1467, -395, 1104,-1079,
    0,    0,    0,    0, -405, -151, -653, 1471, -427,  -55,
    0, -630, 1741,    0, 1037,-1230,    0, 1025,    0,    0,
 2311, -736,  -30,    0,    0,    0,    0, 2285,    0, 2040,
 1504,    0,    0,    0, 1562,    0, 2368, 1856, 1503, 2322,
 2323,    0,    0, -525, 1870,  -49, -779,  939, -287,    0,
    0,    0,-1129,-1101,-1098,-1074,-1072,    0, 1078,-1069,
    0,  806,-1321, 1102,    0, 2114,    0,    0,    0,  827,
  830,  811,  -84, -312,-1057,-1064,    0,    0,    0, 1890,
    0, -775,    0, 1687,    0,    0,    0,    0,    0,-1313,
-1198,    0,    0,    0,    0,    0,    0, -262,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,-1036,    0,    0, -801,    0, -800, 1226, 1027,
    0,  863,    0,    0,    0,    0,    0,    0,    0, -477,
    0,    0, 1978, -154, -499, 1399, 1156, -613, -885,-1121,
-1468,    0,    0,  -42,    0,    0,    0,    0,    0, 2382,
    0,    0,    0,    0, 1883,    0, 2402, 2152,    0,    0,
    0,    0,    0,   -9, -553,    0,    0, 2411,    0,    0,
    0,  -10,    0,    0,    0, -422, 1848, -421, 1361,    0,
    0,    0,    0,    0,    0, -439,    0,    0,    0, 2097,
    0, 1865,    0,    0,    0,    0,    0,    0,   77,    0,
 2817,    0, 2669,    0,    0,    0, 2482,    0,    0,    0,
    0, 2243,    0, 2237,    0,    0,    0, 1983,    0,    0,
    0, -697,    0, -278,    0,    0,    0,    0,    0, -280,
 -737, -967, 2393,    0,  780,    0,    0,    0,    0,    0,
    0, -328,    0,    0, -850, -285,  388,    0,    0,    0,
 2682, 1114,    0, 1988, -862, -667,    0,-1051, 1713,-1409,
    0,    0,    0,    0, -170,    0,    0,    0,    0, 1455,
    0,    0,    0,    0, 2763,  272,    0,  945, 2110,    0,
 1123,    0, 1339,    0, -366,    0,    0, 1993,    0,    0,
    0,    0,    0,    0, 2257,    0,    0,    0, -527,-1320,
 2019, 1747,-1224,    0,    0,    0,    0, -749, -538, -509,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1567, 1591,    0,    0,    0,-1059,
    0,    0,    0, 2390, 1458,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,-1081, 1554,    0,    0,
};
#define YYTABLESIZE 18571
static short yytable[] = {                                     154,
   40,   86,  239,  364,  266,  174,  173,  316,  181,  336,
  339,  892,  335,  338,  261,  688,  651,  277,  306,  282,
  262,  303, 1098, 1099,  718,  719,  297,  569,  893, 1154,
  828,  917,  274,  682,  250,  257,  653,  811,  301, 1036,
 1615,  305,  307,  866, 1420, 1362, 1569,  846,  234,  237,
  589,  241,  244,  173,  249,  252,  274, 1062, 1108, 1354,
  263,  237,  252, 1070,  244,  300,  173,  249, 1311,  281,
  154,  252, 1421,  308, 1084, 1422,  849, 1104, 1388,  293,
  174,  173,  295,  237,  245,  293,  252,  241,  244,  304,
  173,  249,  281,  154,  252,  372,  244,  275,  242, 1423,
 1428, 1424,  612, 1240,  744, 1158,  322, 1432,  324,  367,
  270,  660, 1675, 1456, 1425,  627, 1351, 1438, 1477,  783,
  759,  828,  760,  762, 1429,  949,  989,  905, 1152, 1179,
  676,  909, 1182,  713,  302,  657,  919,  333,  346,  864,
  734, 1189,  309, 1460,   85,  166,  263,  173,  263,  783,
   52,  957,  154, 1221, 1631,  167,  998,   85,  843,  252,
  334,  759,  828,  744,  363,  828,  237,  244,  173,  281,
  819,  676,  460,  181,  713,  334, 1215, 1472,  323,  759,
  828,  760,  762,  678,  879,  453,  140,  254,  828,  676,
  320, 1159,  713,  193,  613, 1665, 1666,  608, 1245,  370,
 1669,  417, 1235,  836,  166, 1262,  320,   87,  783,   52,
   87,  897, 1355,  368,  167, 1003, 1041, 1096,  337,   53,
  875, 1192,  637,  762,  678, 1946, 1340,  796,  254,  819,
  813,  254,  448, 1029,  812,  601, 1842, 1711, 1799,  718,
  719,  334,  678,  975, 1004,  140,  254, 1549, 1846, 1011,
 1551, 1355,  193,   95,  334,  106,  608,  371,  365,  374,
  555, 1096,  688,  563,  762,  619, 1774, 1775,  796,  925,
  296, 1615,  677, 1029,  813,  841, 1096,  869,   53,  948,
  340,   97,  762,  617,  922,  880,  796,  106,  598,  813,
  363,  569, 1307, 1308, 1098, 1099, 1420, 1420,  789,  159,
  263, 1420,  130,  263,  883,  812,  608,  188, 1263,  571,
  154, 1065, 1066,  677, 1251,  611,   67,  365,  969,  610,
  498, 1441, 1378,   91, 1421, 1421,  421, 1422, 1422, 1421,
 1337,  677, 1422,  616,  850,  193,  130,  241, 1087,  789,
 1642,  173,  789,  852, 1402, 1095,  870,  158, 1654,  363,
 1030, 1423, 1423, 1424, 1424,  431, 1423,  789, 1424,  189,
 1699,  923, 1673,  652, 1667,  608, 1425, 1425,  793, 1685,
  130, 1425,  605,  555,  107,  262, 1406,  668,  610,  498,
  670, 1847,  729,  625,  625, 1784,  603,  638, 1904, 1252,
 1030,  732, 1769, 1780,  193, 1403,  371, 1426, 1427,  422,
  183,  924,  742,  263,  613,  783,  683, 1703,  685,  793,
  340, 1222,  793,  263, 1198,  263, 1405,  263,  782, 1440,
  673, 1442, 1712,  738,  794, 1264, 1260,  793,  432,  184,
 1178,  160, 1996, 1379,  340,  759, 1416, 1653, 1329,  237,
  244,  249,  166, 1376, 1029,  759,  783,  759,  713,  173,
 1067,  241,  167,  680,  751, 1963,  759,  511,  713,  782,
  713,  748, 1887, 1409,  783,  794,  676, 1259, 1755,  713,
  821,  819, 1758,  820, 1040,  822,  795,  782,  812,  583,
  740,  150, 1945,  794,  744, 1663,  730, 1433, 1434,  166,
 1241,  874, 1004,  744,  680, 1950, 1004,  828, 1918,  167,
  759, 1257,  760,  762, 1172, 1258,  815, 1174, 2019,  759,
 1810,  760,  680,  713,  744, 1785,  511,  795,   95,  678,
  828, 1185,  713, 1494, 1495,  611, 1496, 1497, 1641,  950,
  140,  819, 1142,  561,  956,  795,  561,  193,  762, 1482,
  150,  636,  796, 1261,  332, 1244,   97,  332,  744,   84,
 1205,  569,  759, 1562, 1563,  173,  244,  571,  332,  762,
  842, 1030,   84,  796,  173,  713,  759, 1900,  173,   87,
 1126,  863,  631,  571,  319,  181,  676,  713,  563,  713,
 1839, 1564, 1214, 1811,  465, 1702,  838, 1812, 1813, 1806,
  887,  783, 1382,  193, 1956,  845,   52, 1800,  608,  850,
  636, 1673,  840,  762,  951,  193, 1104,  898,  677, 1848,
  813, 1160,  921,  789,  828, 1247, 1153,  744,  263,  813,
 1471,  906,  657, 1840, 1615, 1481, 1832,  714,  332,  678,
 1387,  631,  193,  759,  789,  760,  762, 1984,  193,  274,
  193, 1214,  974, 1838, 1383,  980,  713, 1935, 1936, 1615,
  952,  914,  263, 1100,  166,  762,  998,  914, 1408, 2002,
  365,  854,  783, 1096,  167,   53, 1177, 1233,  714,  762,
 1879,  714,  850,  796,  813, 1875, 1105, 1615, 1662,  193,
  254,  852,  972,  793,  332,  498,  714,  193, 1615,  262,
 1701, 1615,  363,  826,  185,  979,  760, 2020, 1615,  941,
 1677, 1896,  983,  984,  793,  817, 1029,  608,  613,  715,
  818,  947, 1145, 1636, 1637,  266, 1825,  454,  677,  156,
  783,  511,  957,  686,  590,  186, 1911, 1939,  753,  263,
 1912, 1029,  596,  782,  608,  942,  762,  760,  137,  794,
 1888,  783, 1353,  813,  789, 1921, 1972, 1060, 1420, 1420,
  715, 1043, 1852,  715,  782,  760,  193,  150,  150, 1976,
  794, 1411, 1925,  957, 1720,  561,  957, 1922,  715,  753,
  637, 1994,  252, 1923, 1029, 1795, 1421, 1421,  407, 1422,
 1422,  957,  555,  157,  555,  938, 1113,  753, 1430,  680,
 1431,  795,  789,  387, 1501, 1489, 1145, 1505,  561, 2013,
   14, 1805, 1439, 1423, 1423, 1424, 1424, 1089,  455,  487,
 2018,  789,  795, 2021,  793,  957,  752,  263, 1425, 1425,
 2022,  238,  263, 1030,  263,  193,  263, 1490, 1443,  611,
 1473, 1982, 1676,  571,  262,  943, 1462,  407, 1332, 1333,
  561,  173,  244, 1962,  193, 1506, 1964,  511, 1030, 1128,
  244,  783,  387,  150,  266,  887,  895,  752, 1194,  294,
  555, 1556, 1166,  173,  782,  173,  971, 1247,  263,  340,
  794, 1889, 1124, 1176,  263,  752,  340,  817, 1167,  636,
  625,  793,  818, 1989,  193, 1463,  393, 1195,  840,  253,
 1199, 1030,  939,  274,  605,  561, 1151, 1230, 1231, 1232,
  263, 1969, 1204, 1971, 1689, 1749, 1878, 1697,  137,  137,
  631, 1975, 1640, 1977, 1804,  391,  487, 1890,  783, 1219,
 1211,  737,  795,  392,  821, 1100,  151,  263, 1891,  822,
  263,  782,  274,  274,  274,  687, 1536, 1029,  636, 1554,
 1633,  636,  714,  631,  940,  393,  751, 1168, 1029,  613,
  561,  958,  714, 1212,  714, 1049, 1049,  636, 2010,  833,
  756,  601,  832,  714, 1444,  867,  749, 1983,  487,  631,
  398,  173,  631,  239,  391, 1098, 1099, 1334,  615,  152,
 1029,  173,  392,  636,  352,  151,  886,  399,  631,  751,
 1256, 1502,  958,  169,  758,  958,  748,   88,   89,  340,
  449,  756, 1234, 1031,  756, 1809,  340,  714,  395,  394,
  958,  760, 1242,  239,  631, 1688,  714, 2003,  397,  756,
  184,  760,  967,  760,  715,  637, 1032, 1892,  396,  398,
  255,  256,  760, 1885,  715,  758,  715,  615,  152, 1765,
 1341,  269,  407,  753,  958,  715,  399,  897,  749,  419,
 1841, 1684,  387,  758, 1030,  753, 2004,  387,  602,  714,
 1445,  252,  387,   84,  753, 1030,  723,  395,  394, 1145,
  611,  714, 1616,  714,  757, 1619,  760,  397,  932,  184,
  933, 1705, 1033, 2016,  637,  760,  450,  396,  968,  715,
  239,  255,  256,  170, 1049,  647, 1446, 1030,  715,  612,
  171,  463,  288, 2005,  571,  571,  723,  723,  753,  468,
  723,  348,  603,  718,  719,  757,  614,  753,  410, 1275,
  244,  451,  193,  601,  957,  723,  414,  887,  760,  604,
  239,  752, 1462,  757, 2017,  312, 1098, 1099, 1098, 1099,
  714,  715,  760,  752,  349,  415, 1098, 1099, 1098, 1099,
  393, 1466,  752,  715,  647,  715,  290,  504,  612,  817,
  753, 1949,  490,  411,  818,  536, 1484, 1276, 1953,  193,
  350,  253, 1488,  351,  753,  614, 1901,  193,  634,  391,
  635, 1465,  601,  387, 1100,  414,  274,  392,  262,  352,
  613, 1277,  563, 1098, 1099,  649,  752,  298,  330,  285,
 1451,  274,  379,  151,  415,  752,  490,  274,  650,  760,
  632,  193, 1452,  193,  536,  353,  504, 1451, 1502, 1502,
  185,  490,  715, 1628, 1628, 1632, 1502, 1502,  263, 1502,
 1502,  438,  263,  286,  398,  193,  411, 1502, 1502, 1502,
 1502,  753, 1502, 1502, 1502, 1502, 1502, 1502,  752,  821,
  819,  399,  820,  340,  822,  379,  152,  828, 1393, 1659,
  193, 1661,  752,  310,   14,  439, 1285,  440,  175,  632,
  412, 1394,  395,  394,  150,  756,  393,  925,  184,  185,
 1286,  640,  397, 1978, 1620,  368,  313,  756,  413,  441,
  184,  848,  396,  341, 1395,  411,  756,  317,  828, 1529,
  173,  828,   43,  500, 1565,  391, 1618,  555, 1621,  758,
  262,  611, 1844,  392,  442,  150,  828,  184,  150,  176,
   47, 1704,  414,  184, 1287,  184,  914,  175,  318,  752,
  758, 2014,  640,  150,  193,  796,  366, 1980,  821,  819,
  756,  820,  176,  822,  368, 1397,  263,  263,  824,  756,
 1570, 1614, 1288,  958,  571,  244,  249,  571,  281,  154,
  398,   43,  500,  637,  184,  164,  165,  914, 1108, 1108,
  914, 1108, 1108, 1108,  758,  571,  571,  399,  176,   47,
  513,  723,  244,  758,  723,  914, 1108, 1108, 1108,  757,
 1069,  723,  756,  723,  796,  366,  637,  723,  395,  394,
  321, 1617,  723, 1817,  399,  723,  756,  824,  397,  142,
  757,  404,  519, 1207, 1208,  504, 1289, 1108,  396,  325,
  723,  613,  637,  405, 1818,  637,  758,  504,  177,  382,
 1819, 1148,  326,  647,  723,  254,  587,   41,  332,  513,
  758,  637,  723, 1924,  400,  592,  723, 1149,  425,  143,
 1687,  426, 1710,  108,  757,  723,  406, 1715,  407,  178,
  504,  165,  504,  757, 1820, 1144,  647,  637,  142,  131,
 1797,  723,  248,  756,  179,  347,  254, 1069,  185,  254,
  250,  809,  821,  819,  817,  820,  214,  822,  382,  818,
  185,  109,  647,  427,  254,  647,   41, 1069,  723,  837,
 1745,  631,  836,  110, 1768,  636,  757,  758,  143,  807,
  723,  647,  723,  248,  340,  442,  248,  185, 1529,  173,
  757,  250,  365,  185,  250,  185,  808,  111,  131,  184,
  395,  248,  340,  828,  828, 1746,  340,  647,  396,  250,
  809,  828,  611, 1938,  398,  214,  340, 1747,  632, 1069,
  150,  150,  821,  819, 1037,  820,  402,  822,  150,  723,
  418,  237,  821,  819,  185,  820,  773,  822,  807, 1773,
 1256, 1256, 1045,  817,  911,  423, 1047,  910,  818,  723,
  434,  632, 1614,  723,  214,  808, 1049,  757,  914, 1348,
  821,  819,  112,  820,  833,  822,  332, 1108, 1991,  674,
 1992,  773,  113, 1108,  795,  435, 1100,  632, 1108,  675,
  632, 1108, 1656,  773,  571,  571,  808,  809,  810,  437,
  387,  833, 1657, 1206, 1903,  214,  632,  114,  214,  828,
  387,  363, 1995,  828,  255,  256, 1393, 1628,  244,  244,
  109,  914, 1083,  214,  914,  826,  150, 1108,  446, 1394,
  828,  914,  632,  914,  457, 1108,  828,  914, 1121, 1083,
  459, 1120,  914, 1108,  173,  914, 1645,  150, 1647,  904,
  461, 1079, 1395,  150, 1069, 1239,  111,  564, 1238, 1243,
  914, 1108,  994, 1108, 1352,  464, 1396,  826, 1872,  855,
  828,  138,  138,  857,  914, 1823, 1108,  465, 1108, 1108,
 1108, 1358,  914,  560, 1357,  914,  914,  150, 1788, 1790,
  904,  254,  254,  904,  574,  914, 1919,  817, 1407,  254,
  566, 1238,  818, 1397, 1454, 1789, 1791, 1453,  904,  185,
  575,  914,  578, 1529,  173,  821,  819,  241,  820,  716,
  822,  112, 1459,  563,  581, 1458, 1561, 1108,  579, 1355,
  828,  113,  716,  914,  580, 1753,  250,  250,  914,  582,
  716,  584,  251,  599,  250, 1920, 1256, 1100, 1635, 1100,
  914,  826,  914,  600,  613,  716,  114, 1100, 1098, 1100,
  597,  609,  620,  263, 1897,  363,  622,  817,  244,  716,
 1098,  341,  818, 1098,  623,  821,  819,  817,  820,  613,
  822,  528,  818,  251, 1089,  559,  251,  254,  624,  173,
 1957, 1927,  630, 1719,  241, 1929,  826, 1760, 1509,  914,
  826,  251,  628, 1941, 1100,  817,  716,  613,  254, 1796,
  818,   14,  826, 2000,  254, 1398, 1999, 1328,  613,  914,
 1917,  613,  528,  914, 2001,  528,  559,  836,  613,  559,
  377,  781,  250,  377,  781,  484,  485,  486,  642,  644,
  528,  241, 1010, 1103,  559, 1347,  559,  643,  254,  821,
  819,  646,  820,  250,  822, 1103,  342,  655, 1103,  250,
  237, 1943, 1039,  494,  495,  666, 1044,  669,  679, 1944,
  680, 1038,  871, 1042,  684,  611, 1061, 1046, 1048, 1050,
  739, 1068, 1069,  735,  563, 1072, 1073, 1074, 1075, 1076,
 1080,  741,  743,  250,  776,  778,  779,  780,  781,  519,
  611, 1090, 1091, 1092, 1093, 1094,  519,  782,  254, 1101,
  783,  904,  788,  871,  784, 1614,  871, 1968,  716,  785,
  786, 1128,  787,  789,  263,  790,  793,  798,  611,  799,
  800,  871,  856,  821,  819,  241,  820,  241,  822,  611,
 1614,  801,  611,  802,  803,  592,  804,  805,  806,  611,
  817,  807,  816,  250,  248,  818,  823,  824,  826,  827,
  831,  872,  834,  835,  904, 1155,  852,  904, 1614, 1128,
  860,  862,  873,  877,  904,  878,  904,  631, 1165, 1614,
  904, 1256, 1614,  884,  882,  904, 1393,  885,  904, 1614,
 1560,  889,  890,  894,  896,  248,  637,  900,  248, 1394,
  899,  902,  872,  904,  907,  872,  821,  819,  903,  820,
  817,  822,  912,  248,  936,  818,  937,  904,  251,  251,
  872,  945, 1395,  332,  953,  904,  251,  954,  904,  904,
  959,  960,  821,  819,  961,  820, 1396,  822,  904,  962,
  964, 1730,  965,  559, 1905,  976,  970,  977,  982, 1082,
  821,  819,  981,  820,  904,  822,  991,  528,  528,  994,
  559,  559,  559,  993,  999,  528,  559,  821,  819,  559,
  820,  858,  822, 1397, 1001, 1000,  904, 1005,  528, 1063,
 1071,  904,  559,  188,  817, 1027, 1026, 1028,  559,  818,
 1085,  971,  334,  904, 1109,  904, 1118, 1265, 1266, 1270,
 1114, 1116, 1279, 1281, 1283, 1115, 1291, 1294, 1297, 1299,
 1302, 1305, 1122,  843,  251, 1141, 1146, 1156,  836, 1162,
 1312, 1313, 1173, 1315, 1161, 1317, 1184, 1183, 1320, 1321,
 1322, 1323, 1324, 1325,  871,  251, 1186, 1193, 1196,  915,
 1190,  251,  904, 1191, 1210, 1201, 1330, 1217, 1335, 1202,
  538,  821,  819,  528,  820, 1203,  822,  559, 1229, 1250,
 1227, 1906,  904,  227, 1350, 1236,  904, 1272,  817, 1310,
 1309, 1316, 1318,  818,  528,  251, 1326, 1327,  559,  817,
  528, 1355, 1359, 1377,  559, 1398, 1356,  871, 1361, 1380,
  871,  538, 1385,  559,  538,  559, 1390,  871, 1386,  871,
 1412, 1392, 1400,  871,  227,  519, 1415,  227,  871,  538,
 1435,  871,  556,  559,  528, 1437, 1447, 1457,  559, 1461,
 1455,  955,  227,  872, 1467, 1468,  871, 1469, 1314, 1470,
  248,  248, 1478, 1244, 1492,  251, 1486, 1491,  248, 1493,
  871,  817, 1499, 1500, 1507, 1508,  818,  559,  871, 1504,
 1510,  871,  871,  556, 1511,  285,  556, 1518, 1519, 1520,
 1521,  871,  955,  955,  955,  955,  955,  817,  955, 1523,
 1524,  556,  818,  556, 1525, 1526,  872,  871, 1527,  872,
  955,  955,  955,  955, 1528,  817,  872, 1550,  872,  286,
  818, 1553,  872, 1029, 1622, 1624, 1014,  872, 1625,  871,
  872, 1638,  817, 1639,  871,  858, 1643,  818, 1648, 1649,
 1015, 1650, 1651, 1686,  955,  872,  871, 1660,  871,  821,
  819, 1338,  820,  661,  822, 1674,  248, 1678, 1214,  872,
 1127,  821,  819, 1700,  820, 1679,  822,  872, 1680, 1681,
  872,  872, 1682, 1690, 1706, 1683, 1016,  248, 1708, 1723,
  872, 1709, 1714,  248, 1017,  519, 1721, 1743, 1744,  228,
 1750, 1756, 1018, 1096, 1757,  871,  872, 1767, 1512, 1513,
 1770, 1515, 1517,  821,  819, 1339,  820, 1766,  822, 1522,
 1019,  516, 1020,  157, 1777,  871,  817,  248,  872,  871,
 1778,  818, 1783,  872, 1786, 1021, 1792, 1022, 1023, 1024,
  228, 1540, 1787,  228, 1798,  872, 1824,  872, 1793, 1547,
 1814, 1548, 1335, 1826, 1827, 1335, 1828, 1330,  228, 1829,
 1831, 1555,  519, 1557, 1830, 1833,  538,  538, 1834, 1835,
 1558,  538, 1559, 1843,  538, 1845, 1849, 1850, 1851,  239,
  227, 1130, 1899, 1854, 1855, 1856, 1025,  538, 1857, 1319,
  821,  819, 1860,  820,  872,  822, 1861, 1342,  821,  819,
 1862,  820, 1863,  822, 1864, 1343,  821,  819, 1865,  820,
  556,  822, 1866,  519,  872, 1867, 1868, 1869,  872,  955,
 1871, 1910, 1874,  955, 1882,  955,  955,  556,  556,  556,
 1898, 1915,  955,  556, 1779, 1913,  556,  955,  955, 1916,
 1928, 1932, 1937,  955, 1947,  955,  955, 1940, 1948,  556,
 1951, 1954, 1965, 1668, 1958,  556, 1966, 1967, 1970, 1990,
  955, 1985,  538, 1987,  955,  955,  955, 1993, 1998,  955,
 1982, 1983, 2008, 2011, 2015,  239,  955, 2009,  955,  955,
 2012,    1,  955,  538,  789,  200,  955,  828,  955,  538,
  955, 1063,  955,   29,  817,   31,  239,  955,  955,  818,
  955,  268,  239,  379,  163, 1280,  817, 1284,   82, 1292,
 1295,  818, 1300, 1303, 1306,  955,  955,  768,  955,  957,
  705, 1716, 1048,  538,  556,  210,  767, 1722,  796,  955,
  279,  955, 1726,  955,  955,  955,  239,  955,  278, 1731,
  955,  955,  190,  407,  749,  556,  751, 1047,  817,  769,
  955,  556,  191,  818,  955, 1021,  323,  380,  220,  321,
  955,  331,  556,  955, 1020,  322,  955,  295, 1344,  821,
  819,  955,  820, 1335,  822,  239,  228, 1335,  324,  332,
  556,  778,  955, 1764,  327,  556,  345,  360,  955,  955,
   29,   32,  329,  955,  955,   33,   42,  327,  328, 1771,
 1626,  573, 1125,  572,  955,  955,  853,  955,  851, 1117,
 1487, 1485, 1249,  955,  556, 1213,  955,  299,  955,  955,
  955,  955,  992,  955,  284,  817, 1254, 1475, 1483, 1223,
  818,  955,  955,  817,  821,  819, 1837,  820,  818,  822,
  648,  817,  955,  955, 1090,  955,  818, 1836,  692,  955,
 1464, 1414,  955,  955,  955,  955,  955,  618,  955, 1157,
 1476,  955,  955,  955,  955,  955,  663,  664, 1816,  955,
  955,  239, 1914, 1801,  955,  955,  955,  955,  955,  955,
  955,  955, 1981,  955, 1065, 1090, 1090, 1090, 1090, 1090,
 1090, 1090,  239,  955, 1822, 1345,  821,  819,  239,  820,
  931,  822, 1090, 1090, 1090, 1090, 1090, 1988, 1188, 1986,
 1346,  821,  819, 2007,  820,  115,  822, 1363,  116, 1181,
  117,  118,  119,  120, 1772,  121, 1655, 1895, 1973, 1106,
 1807,  671,  239,  122,  123, 1090,  654, 1090,  645, 1228,
  935, 1197, 1698,  978,  124,  125, 1853,  126, 1220,  163,
   14,  331, 1858, 1859,  127,  128,  610,  129,  130,  872,
  131,  868, 1389,  132,  133,  134,  135,  136,  345, 1143,
 1646,  137,  138,  294, 1955, 1147, 1873,  139,  140,  141,
 1335,  142,  143,  144, 1876,  145, 1870, 1742, 1123, 1384,
  881, 1883, 1884,  115, 1664,  146,  116, 1514,  117,  118,
  119,  120,  792,  121,  817, 1034, 1164, 1552,    0,  818,
    0,  122,  123,    0, 1534,  821,  819,    0,  820,    0,
  822,    0,  124,  125,    0,  126,  821,  819, 1761,  820,
    0,  822,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0, 1498,  139,  140,  141,    0,  142,
  143,  144,    0,  145, 1535,  821,  819, 1926,  820,  817,
  822,    0,    0,  146,  818, 1538,  821,  819,    0,  820,
    0,  822, 1539,  821,  819,    0,  820,    0,  822,    0,
    0, 1541,  821,  819,    0,  820,    0,  822,    0,    0,
    0,    0,    0,    0,    0,   57, 1090,    0, 1090, 1090,
    0,    0,    0,    0,    0, 1090,   58,    0,    0,    0,
    0, 1090,    0,   59,    0,    0, 1090,   60,    0, 1090,
    0,  817,    0, 1801,    0,  561,  818,    0,    0,    0,
    0,    0,    0, 1090, 1069,    0,  817,   61, 1090, 1090,
    0,  818, 1090,    0, 1542,  821,  819,    0,  820, 1090,
  822, 1090, 1090,    0,    0, 1090,    0,    0,    0, 1090,
 1974, 1090,    0, 1090,    0, 1090,  561,    0,    0,  561,
 1090, 1090,    0, 1090,    0, 1069, 1069, 1069, 1069, 1069,
 1083, 1069,    0,    0,  561,    0,  561,    0, 1090, 1090,
    0, 1090,    0, 1069, 1069, 1069, 1069,    0,    0,   62,
    0,    0, 1090,    0, 1090,    0, 1090, 1090, 1090,    0,
 1090,    0,    0, 1090, 1090, 1051, 1052, 1053, 1054, 1055,
 1056, 1057, 1058, 1090,    0, 1069,    0, 1069,    0,    0,
   63, 1543,  821,  819,   64,  820, 1090,  822,    0, 1090,
  817,    0,    0,    0,    0,  818, 1544,  821,  819,    0,
  820,  817,  822,    0,    0, 1090,  818,    0,    0,    0,
    0, 1090, 1090,    0,    0,    0, 1090,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1090, 1090,    0,
 1090, 1545,  821,  819,    0,  820, 1090,  822,    0, 1090,
  817, 1090, 1090, 1090, 1090,  818, 1090,    0,    0,    0,
    0,  817,    0,    0, 1090, 1090,  818,    0,  817,    0,
    0,    0,    0,  818,    0, 1090, 1090,  817, 1090,    0,
    0,    0,  818,   65,    0, 1090, 1090, 1090, 1090, 1090,
    0, 1090,    0,    0, 1090, 1090, 1090, 1090, 1090,    0,
    0,    0, 1090, 1090,    0,    0,    0, 1090, 1090, 1090,
 1090, 1090, 1090, 1090, 1090,    0, 1090,    0,    0, 1546,
  821,  819,    0,  820,    0,  822, 1090, 1751,  821,  819,
    0,  820,    0,  822,    0,    0,    0,    0,    0,    0,
  817,    0,    0,  561,    0,  818,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1069,    0, 1069, 1069,
    0,  561,  561,    0,    0, 1069,  561,    0,    0,  561,
    0, 1069,    0,    0,    0,    0, 1069,    0,    0, 1069,
    0,    0,  561,    0,    0,  561,    0,    0,  561,    0,
    0,    0,    0, 1069, 1100, 1069,    0,    0, 1069, 1069,
    0,    0, 1069,    0, 1752,  821,  819,    0,  820, 1069,
  822, 1069, 1069,    0,    0, 1069, 1759,  821,  819, 1069,
  820, 1069,  822, 1069,    0, 1069,  561,  817,    0,  561,
 1069, 1069,  818, 1069,    0, 1100, 1100, 1100, 1100, 1100,
 1100, 1100,  817,    0,  561,    0,    0,  818, 1069, 1069,
    0, 1069,    0, 1100, 1100, 1100, 1100,  561,    0, 1762,
  821,  819, 1069,  820, 1069,  822, 1069, 1069, 1069,    0,
 1069,    0,    0, 1069, 1069,    0,    0,  817,  561,    0,
    0,    0,  818, 1069,  561, 1100,    0, 1100, 1763,  821,
  819,    0,  820,    0,  822,  561, 1069,    0,    0, 1069,
    0,    0,    0,    0,    0,    0, 1877,  821,  819,    0,
  820,    0,  822,  561,    0, 1069,    0,    0,  561,    0,
    0, 1069, 1069,    0,    0,    0, 1069,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1069, 1069,    0,
 1069,    0,    0,    0,    0,    0, 1069,  561,    0, 1069,
    0, 1069, 1069, 1069, 1069,  817, 1069,    0,    0,    0,
  818,    0,    0,  817, 1069, 1069,  821,  819,  818,  820,
    0,  822,    0,    0,    0, 1069, 1069,    0, 1069,    0,
    0,    0,    0,    0,    0, 1069, 1069, 1069, 1069, 1069,
    0, 1069,    0,    0, 1069, 1069, 1069, 1069, 1069,    0,
    0,    0, 1069, 1069,    0,    0,    0, 1069, 1069, 1069,
 1069, 1069, 1069, 1069, 1069,    0, 1069,    0,    0, 1934,
  821,  819,    0,  820,    0,  822, 1069,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  817,    0,    0,    0,    0,  818,    0,  561,    0,    0,
    0,    0,  817,    0,    0,    0, 1100,  818, 1100, 1100,
    0,  561,  561,    0,    0, 1100,  561,    0,    0,  561,
    0, 1100,    0,    0,    0,    0, 1100,    0,    0, 1100,
    0,    0,  561,    0,    0,    0,    0,    0,  561,    0,
    0,  561,    0, 1100, 1105,  817,    0,    0, 1100, 1100,
  818,    0, 1100,    0,    0,    0,  561,    0,  561, 1100,
    0, 1100, 1100,    0,    0, 1100,    0,    0,    0, 1100,
    0, 1100,    0, 1100,  817, 1100,    0,    0,    0,  818,
 1100, 1100,    0, 1100,    0, 1105, 1105, 1105, 1105, 1105,
 1105, 1105,  817,    0,    0,    0,    0,  818, 1100, 1100,
    0, 1100,    0, 1105, 1105, 1105, 1105,  561,    0,    0,
    0,    0, 1100,    0, 1100,    0, 1100, 1100, 1100,    0,
 1100,    0,    0, 1100, 1100,    0,    0,    0,  561,    0,
   69,    0,    0, 1100,  561, 1105, 1729, 1105,    0,    0,
    0,   70,    0,    0,    0,    0, 1100,   92,   71, 1100,
  561,  817,    0,    0,   93,    0,  818,    0,   94,   95,
    0,   72,    0,    0,    0, 1100,    0,   96,  561,    0,
    0, 1100, 1100,    0,    0,    0, 1100,    0,   97,    0,
    0,    0,    0,    0,    0,    0,    0, 1100, 1100,    0,
 1100,  561,    0,    0,  561,    0, 1100,    0,    0, 1100,
    0, 1100, 1100, 1100, 1100,  817, 1100,    0,    0,  561,
  818,    0,    0,    0, 1100, 1100,    0,    0,   73,    0,
    0,    0,    0,    0,    0, 1100, 1100,    0, 1100,    0,
    0,    0,    0,    0,   74, 1100, 1100, 1100, 1100, 1100,
   98, 1100,    0,    0, 1100, 1100, 1100, 1100, 1100,    0,
   75,    0, 1100, 1100,    0,  561,   99, 1100, 1100, 1100,
 1100, 1100, 1100, 1100, 1100,   76, 1100,    0,    0,   77,
  561,  100,   78,    0,    0,  101, 1100,    0,    0,    0,
   79,    0,    0,    0,    0,    0,  102,    0,    0,    0,
    0,    0,    0,    0,  561,    0,    0,    0,    0,   80,
  561,    0,    0,  561,    0,  103, 1105,    0, 1105, 1105,
    0, 1717, 1718,    0,    0, 1105,    0,    0,    0, 1724,
 1725, 1105, 1727, 1728,    0,    0, 1105,    0,    0, 1105,
 1732, 1733, 1734, 1735,    0, 1736, 1737, 1738, 1739, 1740,
 1741,    0,    0, 1105, 1101,  561,    0,    0, 1105, 1105,
    0,    0, 1105,    0,    0,    0,    0,    0,   81, 1105,
    0, 1105, 1105,    0,  104, 1105,    0,    0,    0, 1105,
    0, 1105,    0, 1105,    0, 1105,    0,    0,    0,    0,
 1105, 1105,    0, 1105,    0, 1101, 1101, 1101, 1101, 1101,
 1101, 1101,    0,    0,    0,    0,    0,    0, 1105, 1105,
  561, 1105,    0, 1101, 1101, 1101, 1101,  561,    0,    0,
    0,    0, 1105,    0, 1105,    0, 1105, 1105, 1105,    0,
 1105,    0,    0, 1105, 1105,  561,  561,  561,    0,    0,
    0,    0,    0, 1105,  561, 1101,    0, 1101,    0,    0,
    0,    0,    0,    0,    0,    0, 1105,  561,    0, 1105,
    0,    0,    0,    0,    0,  561,    0,    0,    0,  561,
    0,    0,    0,    0,    0, 1105,    0,    0,    0,    0,
    0, 1105, 1105,    0,    0,    0, 1105,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1105, 1105,    0,
 1105,    0,    0,    0,    0,    0, 1105,    0,    0, 1105,
    0, 1105, 1105, 1105, 1105,    0, 1105,    0,    0,    0,
    0,    0,    0,    0, 1105, 1105,    0,    0,    0,    0,
    0,    0,  561,    0,    0, 1105, 1105,    0, 1105,    0,
    0,    0,    0,    0,    0, 1105, 1105, 1105, 1105, 1105,
    0, 1105,    0,  561, 1105, 1105, 1105, 1105, 1105,  561,
    0,    0, 1105, 1105,    0,    0,    0, 1105, 1105, 1105,
 1105, 1105, 1105, 1105, 1105,    0, 1105,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1105,    0,    0,    0,
    0,    0,    0,  561,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,  712,    0,    0,    0,    0,
    0,    0,  916, 1110, 1110,  916, 1110, 1110, 1110,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  916, 1110, 1110, 1110,  383,    0,  915,    0,    0,    0,
    0,    0,  384,  875,    0, 1109,    0,    0,    0,    0,
    0, 1109,    0,    0,    0,    0, 1109,    0,    0, 1109,
    0,    0, 1110,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  385,    0,    0,    0,  386,    0,    0,
    0,    0,    0,    0,  875,    0,    0,  875,  387,  915,
    0,    0,  915,    0,    0, 1109,    0,    0,    0,  915,
    0,  915,  875, 1109,    0,  915,    0,  388,  389,    0,
  915, 1109,    0,  915,    0,  873,    0,  693,    0,    0,
  694,    0,    0,    0,    0,    0,    0,    0,  915, 1109,
    0, 1109,    0,    0,    0,    0,    0,  695,    0,    0,
  696,    0,  915,    0, 1109,    0, 1109, 1109, 1109,    0,
  915,    0,    0,  915,  915,    0,  873,    0,    0,  873,
    0,    0,    0,  915,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,  873,  874,    0,    0,    0,  915,
  697,    0,    0,    0,    0,    0,  698,  699,  700,    0,
  701,  702,  703,    0,  704, 1109,    0,    0,    0,    0,
    0,  915,    0,    0,    0,    0,  915,    0,    0,    0,
    0,    0,  705,    0,  706,    0,  874,    0,  915,  874,
  915,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  916,  874,    0,    0,    0,    0,    0,
    0,    0, 1110,  707,    0,    0,    0,    0, 1110,    0,
    0,    0,    0, 1110,    0,    0, 1110,    0,    0,    0,
  708,    0,    0,    0,    0,    0,    0,  915,    0,    0,
  906,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  709,    0,    0,    0,  916,  915,    0,  916,
    0,  915, 1110,    0,    0,    0,  916,    0,  916,    0,
 1110,    0,  916,    0,    0,  875,    0,  916, 1110,    0,
  916,  906,    0,    0,  906,    0,    0,  710,  711,    0,
    0,    0,    0,    0,    0,  916, 1110,    0, 1110,  906,
    0,    0,    0,    0,  712,    0,    0,    0,    0,  916,
    0, 1110,  868, 1110, 1110, 1110,    0,  916,    0,    0,
  916,  916,    0,    0,    0,    0,    0,    0,  875,    0,
  916,  875,    0,    0,    0,    0,    0,    0,  875,    0,
  875,    0,    0,    0,  875,    0,  916,  873,    0,  875,
    0,    0,  875,  868,    0,    0,  868,    0,    0,    0,
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
    0,    0,    0,  894,    0,  895,  725,  895,    0,    0,
    0,  895,  900,  909, 1103,    0,  895,    0,    0,  895,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  900,    0,  895,  909,  900,    0,    0,  736,
  909,  897,    0,    0,    0,    0,    0,  725,  895,    0,
  725,    0,  909,    0,  909,    0,  895,    0,    0,  895,
  895,    0,    0,    0,    0,  725,    0,    0,    0,  895,
    0,    0,    0,    0,  901,    0,    0,    0,    0,    0,
  736,    0,    0,  736,    0,  895,    0,    0,    0,    0,
    0,    0,    0,    0,  897,    0,    0,  897,  736,    0,
    0,  909,    0,    0,  897,    0,  897,  895,    0,    0,
  897,    0,  895,    0,    0,  897,    0,    0,  897,    0,
    0,  909,    0,    0,  895,  909,  895,  901,  709,    0,
  901,    0,    0,  897,    0,    0,    0,  901,    0,  901,
    0,    0,    0,  901,    0,    0,    0,  897,  901,    0,
    0,  901,    0,    0,    0,  897,    0,    0,  897,  897,
    0,    0,    0,    0,    0,    0,  901,    0,  897,    0,
    0,    0,    0,  895,    0,    0,    0,    0,    0,    0,
  901,    0,    0,    0,  897,    0,    0,    0,  901,    0,
    0,  901,  901,  895,    0,    0,    0,  895,    0,    0,
    0,  901,    0,    0,    0,    0,  897,    0,    0,    0,
    0,  897,    0,    0,    0,    0,    0,  901,    0,    0,
    0,    0,    0,  897,  846,  897,    0,  846,    0,    0,
    0,    0,    0,    0,  846,    0,  846,    0,    0,  901,
  846,    0,    0,    0,  901,  846,    0,    0,  846,    0,
  759,    0,    0,  513,    0,  514,  901,    0,  901,    0,
    0,    0,    0,  846,    0,    0,    0,    0,  516,    0,
    0,    0,  897,  334,    0,    0,    0,  846,    0,    0,
    0,    0,    0,    0,    0,  846,    0,    0,  846,  846,
    0,    0,  897,    0,    0,    0,  897,    0,  846,    0,
    0,    0,    0,    0,    0,  901,    0,    0,    0,    0,
    0,    0,    0,    0,  846,    0,    0,    0,    0,    0,
    0,  725,    0,    0,  725,  901,    0,    0,    0,  901,
    0,  725,    0,  725,    0,    0,  846,  725,    0,    0,
    0,  846,  725,    0,    0,  725,    0,    0,    0,    0,
    0,    0,    0,  846,  736,  846,    0,  736,    0,    0,
  725,    0,    0,    0,  736,    0,  736,    0,    0,    0,
  736,    0,    0,    0,  725,  736,    0,    0,  736,    0,
    0,    0,  725,    0,    0,   85,  725,    0,    0,    0,
    0,    0,    0,  736,    0,  725,    0,    0,    0,    0,
    0,  709,  846,    0,    0,    0,    0,  736,    0,    0,
    0,  725,    0,    0,    0,  736,    0,    0,    0,  736,
    0,    0,  846,    0,  115,    0,  846,  116,  736,  117,
  118,  119,  120,    0,  121,    0,    0,    0,  725,    0,
    0,    0,  122,  123,  736,    0,    0,    0,    0,    0,
  725,    0,  725,  124,  125,    0,  126,    0,    0,    0,
    0,    0,    0,  127,  128,    0,  129,  130,    0,  131,
    0,  736,  132,  133,  134,  135,  136,  469,    0,    0,
  137,  138,    0,  736,  470,  736,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,  471,    0,    0,  725,
    0,    0,  472,  473,  146,    0,  759,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,  725,
    0,  753,    0,  725,  516,    0,    0,    0,    0,  334,
  474,    0,  736,  475,    0,    0,  712,    0,    0,  712,
    0,  712,  712,  712,  712,    0,  712,    0,    0,    0,
    0,    0,  736,    0,  712,  712,  736,    0,  476,    0,
  477,  478,    0,    0,    0,  712,  712,    0,  712,    0,
  479,    0,    0,    0,  480,  712,  712,    0,  712,  712,
    0,  712, 1006,    0,  712,  712,  712,  712,  712,    0,
    0,    0,  712,  712,    0,    0,    0,    0,  712,  712,
  712,    0,  712,  712,  712,    0,  712,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  712,    0,    0,   14,
    0,    0,    0,  755,    0,    0,    0,    0,    0,  481,
    0,    0,    0,    0,    0,  482,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  483,    0,    0,    0,    0,
    0,    0,    0,    0,  516,    0,    0,    0,    0,  484,
  485,  486,  487,  255,  256,  116,    0,  117,  118,  119,
  120,    0,  121,  488,    0,    0,    0,    0,    0,    0,
  122,  123,  489,  490,  491,  492,  493,  494,  495,  496,
  497,  124,  498,    0,  126,    0,    0,  499,  500,    0,
  501,  502,  503,    0,  129,  130,  504,  131,    0,    0,
  132,  133,  134,  135,  136, 1007, 1008, 1009,  137,  138,
    0,    0,    0,    0,  139,  505,  141,    0,  506,  143,
  144,    0,  145,  469,  507,  508,  509,    0,  510,  511,
  470,  512,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,  752,    0,    0,  472,  473,
    0,    0,  759,    0,    0,  513,    0,  514,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  753,    0,    0,
  516,    0,    0,    0,    0,  334,  474,    0,    0,  475,
   84,    0,  115,    0,    0,  116,    0,  117,  118,  119,
  120,    0,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,    0,    0,  476,    0,  477,  478,    0,    0,
    0,  124,  125,    0,  126,    0,  479,    0,    0,    0,
  480,  127,  128,    0,  129,  130,    0,  131,  754,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  146,    0,    0,    0,    0,    0,    0,  755,
    0,    0,    0,    0,    0,  481,    0,    0,    0,    0,
    0,  482,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,  487,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  488,
    0,    0,    0,    0,    0,    0,  122,  123,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  124,  498,    0,
  126,    0,    0,  499,  500,    0,  501,  502,  503,    0,
  129,  130,  504,  131,    0,    0,  132,  133,  756,  757,
  758,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  505,  141,    0,  506,  143,  144,    0,  145,  469,
  507,  508,  509,    0,  510,  511,  470,  512,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
    0,    0,    0,    0,  472,  473,    0,    0,  759,    0,
    0,  513,    0,  514,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  753,    0,    0,  516,    0,    0,    0,
    0,  334,  474,    0,    0,  475,    0,    0,  115,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,    0,
  476,    0,  477,  478,    0,    0,    0,  124,  125,    0,
  126,    0,  479,    0,    0,    0,  480,  127,  128,    0,
  129,  130,    0,  131, 1002,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,  755,    0,    0,    0,    0,
    0,  481,    0,    0,    0,    0,    0,  482,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  483,    0,    0,
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
  472,  473,    0,    0,  586,    0,    0,  513,    0,  514,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  753,
    0,    0,  516,    0,    0,    0,    0,  334,  474,    0,
    0,  475,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  476,    0,  477,  478,
    0,    0,    0,    0,    0,    0,    0,    0,  479,    0,
    0,    0,  480,    0,    0,    0,    0, 1082,  821,  819,
  754,  820,    0,  822,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1027, 1026, 1028,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  755,    0,    0,    0,    0,    0,  481,    0,    0,
    0,    0,    0,  482,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  759,    0,    0,  513,    0,  514,    0,    0,    0,    0,
    0,    0,    0,    0,  474,    0,    0,  475,  516,    0,
    0,    0,    0,  334,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1014,    0,    0,
    0,    0,  476,  817,  477,  478,    0,    0,  818,    0,
    0, 1015,    0,    0,  479,    0,    0,    0,  480,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1016,    0,    0,
    0,    0,    0,    0,    0, 1017,    0,    0,    0,    0,
    0,    0,    0, 1018,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  481,    0,    0,    0,    0,  661,  482,
    0, 1019,    0, 1020,    0,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0, 1021,    0, 1022, 1023,
 1024,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503, 1025,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144, 1077,  145,    0,  507,  508,
  509, 1078,  510,  511, 1079,  512,  146,  469,    0,    0,
    0,    0,    0,    0,  470,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  471,    0,    0,    0,
    0,    0,  472,  473,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  586,    0,    0,
  513,  753,  514,    0,    0,    0,    0,    0,    0,    0,
  474,    0,    0,  475,    0,  516,  115,    0,    0,  116,
  334,  117,  118,  119,  120,    0,  121,    0,    0,    0,
    0,    0,    0,    0,  122,  123,    0,    0,  476,    0,
  477,  478,    0,    0,    0,  124,  125,    0,  126,    0,
  479,    0,    0,    0,  480,  127,  128,    0,  129,  130,
    0,  131, 1002,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,  755,    0,    0,    0,    0,    0,  481,
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
  144,    0,  145,    0,  507,  508,  509, 1267,  510,  511,
 1268,  512,  146,    0,  469,  218,    0,    0,  218,    0,
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
    0,    0,    0,    0, 1269,    0,  481,    0,    0,    0,
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
    0,  507,  508,  509, 1278,  510,  511, 1268,  512,  146,
    0,  469,  148,    0,    0,  148,    0,    0,  470,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,  148,    0,    0,  148,  472,  473,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  586,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,    0,    0,  474,    0,    0,  475,    0,  516,
    0,    0,    0,    0,  334,  148,    0,    0,    0,    0,
    0,  148,  148,  148,    0,  148,  148,  148,    0,  148,
    0,    0,  476,    0,  477,  478,    0,    0,    0,    0,
    0,    0,    0,    0,  479,    0,    0,  148,  480,  148,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  148,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  148,    0,    0,    0,    0,
    0, 1269,    0,  481,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,  148,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
 1364,    0,  148,  148,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,  148,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,    0,  507,  508,
  509, 1282,  510,  511, 1268,  512,  146,    0,  469,    0,
 1365,    0,    0,    0,    0,  470,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,    0,
    0,    0,    0,  472,  473,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1366,    0,    0,  586,    0,
 1367,  513,    0,  514,    0,    0, 1368,    0,    0,    0,
    0,  474,    0,    0,  475,    0,  516,  115,    0,    0,
  116,  334,  117,  118,  119, 1369,    0,  121,    0,    0,
    0,    0,    0,    0,    0,  122,  123,    0,    0,  476,
    0,  477,  478,    0,    0,    0,  124,  125,    0,  126,
    0,  479,    0,    0,    0,  480,  127,  128,    0,  129,
  130,    0,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  140,  141,    0,  142,  143,  144,    0,  145,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1269,    0,
  481,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0,    0,    0,    0,
    0,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,    0,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144,    0,  145,    0,  507,  508,  509, 1290,  510,
  511, 1268,  512,  146,    0,  469,    0,    0,    0,    0,
    0,    0,  470,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  471,    0,    0,    0,    0,    0,
  472,  473,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  586,    0,  567,  513,    0,
  514,    0,    0,    0,    0,    0,    0,    0,  474,    0,
    0,  475,    0,  516,  115,    0,    0,  116,  334,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,    0,  476,    0,  477,  478,
    0,    0,    0,  124,  125,    0,  126,    0,  479,    0,
    0,    0,  480,  127,  128,    0,  129,  130,    0,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1269,    0,  481,    0,    0,
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
  145,    0,  507,  508,  509, 1293,  510,  511, 1268,  512,
  146,    0,  469,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,    0,  472,  473,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  586,    0,    0,  513,    0,  514,    0,    0,
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
  508,  509, 1298,  510,  511, 1268,  512,  146,    0,  469,
    0,    0,    0,    0,    0,    0,  470,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
    0,    0,    0,    0,  472,  473,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,  474,    0,    0,  475,    0,  516,   28,    0,
    0,   28,  334,   28,   28,   28,   28,    0,   28,    0,
    0,    0,    0,    0,    0,    0,   28,   28,    0,    0,
  476,    0,  477,  478,    0,    0,    0,   28,   28,    0,
   28,    0,  479,    0,    0,    0,  480,   28,   28,    0,
   28,   28,    0,   28,    0,    0,   28,   28,   28,   28,
   28,    0,    0,    0,   28,   28,    0,    0,    0,    0,
   28,   28,   28,    0,   28,   28,   28,    0,   28,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   28,    0,
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
  506,  143,  144,    0,  145,    0,  507,  508,  509, 1301,
  510,  511, 1268,  512,  146,    0,  469,    0,    0,    0,
    0,    0,    0,  470,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,    0,    0,    0,
    0,  472,  473,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  586,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,  474,
    0,    0,  475,    0,  516,  604,    0,    0,  604,  334,
  604,  604,  604,  604,    0,  604,    0,    0,    0,    0,
    0,    0,    0,  604,  604,    0,    0,  476,    0,  477,
  478,    0,    0,    0,  604,  604,    0,  604,    0,  479,
    0,    0,    0,  480,  604,  604,    0,  604,  604,    0,
  604,    0,    0,  604,  604,  604,  604,  604,    0,    0,
    0,  604,  604,    0,    0,    0,    0,  604,  604,  604,
    0,  604,  604,  604,    0,  604,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  604,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1269,    0,  481,    0,
    0,    0,    0,    0,  482,  667,    0,    0,    0,    0,
    0,    0,    0,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,    0,  507,  508,  509, 1304,  510,  511, 1268,
  512,  146,    0,  469,    0,    0,    0,    0,    0,    0,
  470,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,    0,    0,    0,  472,  473,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  696,
    0,  696,  696,    0,  696,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  474,  696,    0,  475,
    0,    0,  696,  115,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,    0,  476,    0,  477,  478,    0,    0,
    0,    0,  124,  125,    0,  126,  479,    0,    0,    0,
  480,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1269,    0,  481,    0,    0,    0,    0,
    0,  482,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,  487,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  488,
    0,    0,    0,    0,    0,    0,  122,  123,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  124,  498,    0,
  126,    0,    0,  499,  500,    0,  501,  502,  503,    0,
  129,  130,  504,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  505,  141,    0,  506,  143,  144,    0,  145,  696,
  507,  508,  509,    0,  510,  511,  696,  512,  146,    0,
    0,    0,    0,  696,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  696,    0,    0,    0,    0,
    0,  696,  696,    0,    0,    0,    0,    0,    0,  696,
    0,    0,  690,    0,  690,  690,    0,  690,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  696,
  690,    0,  696,    0,    0,  690,  287,    0,    0,  287,
    0,  287,  287,  287,  287,    0,  287,    0,    0,    0,
    0,    0,    0,    0,  287,  287,    0,  696,    0,  696,
  696,    0,    0,    0,    0,  287,  287,    0,  287,  696,
    0,    0,    0,  696,    0,  287,  287,    0,  287,  287,
    0,  287,    0,    0,  287,  287,  287,  287,  287,    0,
    0,    0,  287,  287,    0,    0,    0,    0,  287,  287,
  287,    0,  287,  287,  287,    0,  287,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  287,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  696,    0,
    0,    0,    0,    0,  696,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  696,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  696,  696,
  696,  696,  696,  696,  696,    0,  696,  696,  696,  696,
    0,  696,  696,    0,    0,    0,    0,    0,    0,  696,
  696,  696,  696,  696,  696,  696,  696,  696,  696,  696,
  696,  696,    0,  696,    0,    0,  696,  696,    0,  696,
  696,  696,    0,  696,  696,  696,  696,    0,    0,  696,
  696,  696,  696,  696,    0,    0,    0,  696,  696,    0,
    0,    0,    0,  696,  696,  696,    0,  696,  696,  696,
    0,  696,  690,  696,  696,  696,    0,  696,  696,  690,
  696,  696,    0,    0,    0,    0,  690,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  690,    0,
    0,    0,    0,    0,  690,  690,    0,    0,  586,    0,
  591,  513,  690,  514,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  516,    0,    0,    0,
    0,  334,  690,    0,    0,  690,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  690,    0,  690,  690,    0,    0,    0,    0,    0,    0,
    0,    0,  690,    0,    0,    0,  690,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  690,    0,    0,    0,    0,    0,  690,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  690,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  690,  690,  690,  690,  690,  690,  690,    0,  690,
  690,  690,  690,    0,  690,  690,    0,    0,    0,    0,
    0,    0,  690,  690,  690,  690,  690,  690,  690,  690,
  690,  690,  690,  690,  690,    0,  690,    0,    0,  690,
  690,    0,  690,  690,  690,    0,  690,  690,  690,  690,
    0,    0,  690,  690,  690,  690,  690,    0,    0,    0,
  690,  690,    0,    0,    0,    0,  690,  690,  690,    0,
  690,  690,  690,    0,  690,  469,  690,  690,  690,    0,
  690,  690,  470,  690,  690,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,  484,  485,  486,
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
    0,    0,    0,  334,  474,    0,    0,  475,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,  794,    0,    0,
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
    0,    0,  472,  473,    0,    0,  586, 1086,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  516,    0,    0,    0,    0,  334,
  474,    0,    0,  475,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  476,    0,
  477,  478,    0,    0,    0,    0,    0,    0,    0,    0,
  479,    0,    0,    0,  480,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   14,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  481,
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
  144,    0,  145,  469,  507,  508,  509,    0,  510,  511,
  470,  512,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,    0,    0,    0,  472,  473,
    0,    0,  586,    0,    0,  513,    0,  514,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  516,    0,    0,    0,    0,  334,  474,    0,    0,  475,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  476,    0,  477,  478,    0,    0,
    0,    0,    0,    0,    0,    0,  479,    0,    0,    0,
  480,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0, 1296,    0,
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
    0,    0,  516,    0,    0, 1349,    0,  334,  474,    0,
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
    0,    0,  139,  505,  141,    0,  506,  143,  144,    0,
  145,  469,  507,  508,  509,    0,  510,  511,  470,  512,
  146,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,    0,    0,    0,    0,  472,  473,    0,    0,
 1061,    0, 1061, 1061,    0, 1061,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1061,    0,
    0,    0,    0, 1061,  474,    0,    0,  475,    0,    0,
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
 1516,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145, 1061,  507,  508,
  509,    0,  510,  511, 1061,  512,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1061,    0,    0,    0,
    0,    0, 1061, 1061,    0,    0,  515,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  516,    0,    0,    0,    0,  334,
 1061,    0,    0, 1061,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1061,    0,
 1061, 1061,    0,    0,    0,    0,    0,    0,    0,    0,
 1061,    0,    0,    0, 1061,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1061,
    0,    0,    0,    0,    0, 1061,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1061,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1061,
 1061, 1061, 1061, 1061, 1061, 1061,    0, 1061, 1061, 1061,
 1061,    0, 1061, 1061,    0,    0,    0,    0,    0,    0,
 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061,
 1061, 1061, 1061,    0, 1061,    0,    0, 1061, 1061,    0,
 1061, 1061, 1061,    0, 1061, 1061, 1061, 1061,    0,    0,
 1061, 1061, 1061, 1061, 1061,    0,    0,    0, 1061, 1061,
    0,    0,    0,    0, 1061, 1061, 1061,    0, 1061, 1061,
 1061,    0, 1061,  469, 1061, 1061, 1061,    0, 1061, 1061,
  470, 1061, 1061,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,    0,    0,    0,  472,  473,
    0,    0,  586,    0,    0,  513,    0,  514,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  516,    0,    0,    0,    0,  334,  474,    0,    0,  475,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  476,    0,  477,  478,    0,    0,
    0,    0,    0,    0,    0,    0,  479,    0,    0,    0,
  480,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,  472,  473,    0,    0, 1061,    0,
    0, 1061,    0, 1061,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1061,    0,    0,    0,
    0, 1061,  474,    0,    0,  475,    0,    0,    0,    0,
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
  506,  143,  144,  572,  145, 1061,  507,  508,  509,    0,
  510,  511, 1061,  512,  146,  566,    0,    0,    0,    0,
    0,    0,    0,    0, 1061,    0,    0,    0,    0,    0,
 1061, 1061,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  572,  572,    0,    0,  572,    0,    0,
    0,    0,  588,    0,    0,    0,  566,    0, 1061,  566,
    0, 1061,  572,    0,  572,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  566,    0,  566,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1061,    0, 1061, 1061,
    0,    0,    0,  588,  572,    0,  588,    0, 1061,    0,
    0,    0, 1061,    0,    0,    0,  566,    0,    0,    0,
    0,  588,    0,  588,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  588,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1061,    0,    0,
    0,    0,    0, 1061,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1061,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1061, 1061, 1061,
 1061, 1061, 1061, 1061,    0, 1061, 1061, 1061, 1061,    0,
 1061, 1061,    0,    0,    0,    0,    0,    0, 1061, 1061,
 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061, 1061,
 1061,    0, 1061,    0,  578, 1061, 1061,    0, 1061, 1061,
 1061,    0, 1061, 1061, 1061, 1061,    0,    0, 1061, 1061,
 1061, 1061, 1061,    0,    0,    0, 1061, 1061,    0,    0,
    0,    0, 1061, 1061, 1061,    0, 1061, 1061, 1061,    0,
 1061,  572, 1061, 1061, 1061,  578, 1061, 1061,  578, 1061,
 1061,    0,    0,  566,    0,    0,  572,    0,  572,  572,
  572,    0,  563,  578,  572,  578,    0,  572,  566,    0,
  566,  566,  566,    0,    0,    0,  566,    0,    0,  566,
  572,    0,    0,    0,    0,    0,  572,    0,    0,  572,
  588,    0,  566,    0,    0,  578,    0,    0,  566,    0,
    0,  566,    0,  563,    0,  588,  563,    0,  588,  588,
    0,    0,    0,  588,    0,    0,  588,    0,    0,    0,
    0,  563,    0,  563,    0,    0,    0,    0,    0,  588,
    0,  572,    0,    0,    0,  588,    0,    0,  588,    0,
    0,    0,    0,  566,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  563,    0,  572,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  566,    0,    0,
    0,    0,    0,    0,    0,    0,  572,    0,    0,    0,
  588,    0,  572,    0,    0,    0,  572,    0,  566,    0,
    0,    0,    0,  572,  566,    0,    0,    0,  566,    0,
    0,    1,    0,    0,  588,  566,    0,    0,    0,    0,
    0,  572,    0,    0,    0,    0,  572,    0,    0,    2,
    0,    0,    0,  566,    0,  588,    0,    3,  566,    0,
    0,  588,    0,    0,    4,  588,    5,    0,    6,    0,
    0,  572,  588,    7,    0,  572,    0,    0,    0,    0,
    8,    0,    0,  566,    0,    0,    0,  566,    0,    0,
  588,    0,  578,    0,    9,  588,    0,    0,    0,    0,
    0,    0,    0,    0,   10,    0,    0,  578,    0,    0,
  578,  578,    0,    0,    0,  578,    0,    0,  578,    0,
  588,    0,    0,    0,  588,    0,    0,    0,    0,    0,
    0,  578,    0,    0,  108,    0,    0,  578,    0,    0,
  578,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  563,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  563,    0,   11,  563,  563,
    0,    0,  109,  563,   12,    0,  563,   13,    0,   14,
   15,    0,  578,    0,  110,    0,    0,    0,    0,  563,
    0,    0,    0,    0,    0,  563,    0,    0,  563,    0,
    0,    0,    0,    0,   16,    0,  578,    0,  111,    0,
   49,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  578,    0,    0,
    0,    0,    0,  578,    0,    0,    0,  578,    0,    0,
  563,    0,    0,    0,  578,    0,    0,    0,   49,    0,
    0,    0,    0,   17,    0,    0,    0,    0,    0,    0,
   49,    0,  578,    0,  563,    0,    0,  578,   18,   19,
    0,    0,    0,  112,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  113,   49,  563,    0,    0,    0,    0,
    0,  563,  578,   20,    0,  563,  578,    0,    0,    0,
    0,    0,  563,    0,    0,    0,    0,    0,  114,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  563,    0,    0,    0,    0,  563,  115,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,    0,
    0,    0,    0,    0,  122,  123,    0,    0,    0,   49,
  563,    0,    0,    0,  563,  124,  125,    0,  126,   49,
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
    0,    0,    0,    0, 1361,    0,    0,    0,    0,  329,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  329,    0,    0,
    0,    0,    6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  329, 1571, 1572,    0, 1573,    0,    0,    0,
    0,    0,  329,    0,    0,    0,    0,    0,    0,    0,
    0,  329, 1574,  329,    0,    0,    0,    0, 1575,    0,
  329,    0,    0,  329,    0,  329,  329,  329,  329,    0,
  329,    0,    0,    0,    0,    0,    0,    0,  329,  329,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  329,
  329,    0,  329,    0,    0,    0,    0,    0,    0,  329,
  329,    0,  329,  329,    0,  329,    0,    0,  329,  329,
  329,  329,  329,    0,    0, 1576,  329,  329,  329,  329,
  329,    0,  329,  329,  329,    0,  329,  329,  329, 1361,
  329,    0,    0,   14,    0,    0,    0,    0,    0,    0,
  329,    0,    0,    0,    0,    0,    0,    0, 1577,    0,
    0,    0,    0,    0,    0,    0,    0,    6,   16,    0,
    0,    0,    0,    0,    0,    0,    0, 1779, 1571, 1572,
    0, 1573,    0,    0,    0,    0,  487,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121, 1574,    0,    0,
    0,    0,    0, 1575,  122,  123,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1578,  125,    0,  126,    0,
    0,    0,    0,    0,    0,  127,  128,    0,  129,  130,
    0,  131,    0,    0,  132, 1579,  134,  135,  136,    0,
    0,    0,  137,  138, 1580, 1581, 1582,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
 1576,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   14,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1577,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   16,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  487,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,    0,  693,    0,    0,  694,    0,  122,
  123,    0,    0,  915,    0,    0,    0,    0,    0,    0,
 1578,  125,    0,  126,  695,    0,    0,  696,    0,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
 1579,  134,  135,  136,    0,    0,    0,  137,  138, 1580,
 1581, 1582,    0,  139,  140,  141,    0,  142,  143,  144,
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
    0,    0,  132,  133,  134,  135,  136,    0, 1031,    0,
  137,  138,    0,    0,    0,  708,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0, 1032,    0,    0,  146,    0,    0,  709,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115,    0,    0,  116,    0,  117,  118,  119,
  120,    0,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,  710,  711,    0,    0,    0,    0,    0,    0,
    0,  124,  125,    0,  126,    0,    0, 1033,    0,  712,
    0,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,  115,  145,    0,  116,    0,  117,  118,  119,  120,
    0,  121,  146,    0,    0,    0,    0,    0,    0,  122,
  123,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  124,  125,    0,  126,    0,    0,    0,    0,    0,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0,  115,
    0,  146,  116,  192,  117,  118,  119,  120,  193,  121,
  194,  195,  196,  197,  198,  199,  200,  122,  123,  201,
  202,  203,  204,  205,    0,    0,  206,  207,  124,  125,
  208,  126,    0,    0,  209,  210,  211,  212,  127,  128,
  213,  129,  130,  214,  131,  215,  216,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,  217,  218,  219,
  220,  139,  140,  141,  221,  142,  143,  144,  222,  145,
  223,  224,  225,  226,  227,  228,  229,  230,  231,  146,
  483,    0,    0,  483,  483,  483,  483,  483,  483,    0,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,    0,    0,  483,  483,  483,
  483,  483,  483,    0,    0,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,    0,    0,    0,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,  483,  483,  483,  483,  483,  483,  483,  483,  483,
  483,
};
static short yycheck[] = {                                       9,
    0,    4,   61,  174,   71,   16,   16,  114,   18,  164,
  165,  628,  164,  165,   70,  443,  412,   77,  102,   80,
   70,   99,  824,  824,  447,  447,   93,  319,  631,  880,
  556,  662,   75,  439,   65,   69,  414,  515,   97,  777,
 1361,  101,  103,  597, 1174, 1110, 1360,  575,   58,   59,
  336,   61,   62,   63,   64,   65,   99,  794,  834, 1096,
   70,   71,   72,  800,   74,   96,   76,   77, 1036,   79,
   80,   81, 1174,  104,  812, 1174,  576,  827, 1130,   89,
   91,   91,   92,   93,   63,   95,   96,   97,   98,  100,
  100,  101,  102,  103,  104,  179,  106,   76,   62, 1174,
 1180, 1174,  365,  989,    0,  885,  156, 1187,  158,  176,
   74,  424, 1434, 1218, 1174,  394, 1084, 1197, 1240,   41,
    0,    0,    0,    0, 1182,  679,  740,  653,  878,  909,
    0,  657,  912,    0,   98,  423,  662,   40,  169,   45,
  453,  921,  106, 1223,   45,    0,  156,  157,  158,    0,
    0,   40,  162,   45, 1379,    0,  747,   45,   44,  169,
   63,   41,   41,   59,  174,   44,  176,  177,  178,  179,
    0,   41,  279,  183,   41,   63,   44, 1235,  157,   59,
   59,   59,   59,    0,   44,  270,    0,    0,   44,   59,
   44,  889,   59,    0,  365, 1426, 1427,    0,   40,  178,
 1431,  235,  982,   44,   59,   41,   44,   41,   59,   59,
   44,  639,   44,  177,   59,  754,   42,    0,   40,    0,
   44,  924,  344,    0,   41,   44,   44,    0,   41,   59,
    0,   44,  266,  262,  515,  280, 1705,  273, 1648,  662,
  662,   63,   59,   40,  754,   59,   59, 1329,  351,  759,
 1332,   44,   59,   41,   63,  395,   59,  264,    0,  183,
  310,   44,  690,  313,   41,  372, 1580, 1581,   41,  665,
  305, 1592,    0,  262,   44,  567,   59,  371,   59,   45,
  315,   41,   59,  368,  316,  614,   59,  395,  347,   59,
    0,  583, 1029, 1030, 1096, 1096, 1426, 1427,    0,  324,
  310, 1431,  265,  313,  617,  586,    0,  380,  262,  319,
  320,  304,  305,   41,  273,  365,  447,   59,  714,    0,
    0,  334,  317,  317, 1426, 1427,  257, 1426, 1427, 1431,
 1067,   59, 1431,  367,   41,    0,  299,  347,  816,   41,
 1392,  351,   44,   41,  397,  823,  440,  272, 1406,   59,
  379, 1426, 1427, 1426, 1427,  257, 1431,   59, 1431,  432,
 1465,  393, 1432,  413, 1429,   59, 1426, 1427,    0, 1449,
  333, 1431,  351,  423,  514,  425, 1156,  427,   59,   59,
  430,  484,  449,  393,  394,  315,  431,  509, 1798,  348,
  379,  451,  500, 1592,   59,  448,  403, 1177, 1178,  330,
  498,  433,  461,  413,  575,    0,  440, 1472,  442,   41,
  445,  965,   44,  423,  940,  425, 1154,  427,    0, 1199,
  430,  434,  458,  457,    0,  379,  262,   59,  330,  376,
   61,  456,  264,  428,  260,  315, 1173, 1405,  427,  449,
  450,  451,  297, 1115,  262,  325,   41,  327,  315,  459,
  443,  461,  297,    0,  464, 1924,  336,    0,  325,   41,
  327,  464, 1776, 1161,   59,   41,  336, 1006, 1550,  336,
   42,   43, 1554,   45,  300,   47,    0,   59,  759,  317,
  459,    0,  301,   59,  380,   41,  450, 1190, 1191,  344,
  990,  315, 1002,  389,   41,  336, 1006,  376, 1820,  344,
  380, 1001,  380,  380,  900, 1005,  516,  903,  301,  389,
   41,  389,   59,  380,  410,  445,   59,   41,  306,  336,
  376,  917,  389, 1260, 1261,  575, 1263, 1264, 1391,  681,
  344,  361,  861,   41,  423,   59,   44,  344,  315, 1242,
   59,    0,  315,  379,  450,  387,  306,  450,  444,  450,
  946,  843,  432, 1355, 1355,  565,  566,  567,  450,  336,
  446,  379,  450,  336,  574,  432,  446, 1792,  578,  403,
  856,  477,    0,  583,  428,  585,  446,  444,  628,  446,
 1702, 1357,  450, 1663,  444, 1471,  565, 1667, 1668, 1654,
  624,  513, 1120,  258, 1916,  574,  446, 1649,  401,  578,
   59, 1671,  566,  380,  682,  270, 1356,  641,  336, 1714,
  380,  890,  662,  315,  493,  993,  879,  513,  628,  389,
 1234,  655,  910, 1703, 1945, 1242, 1684,    0,  450,  446,
 1130,   59,  297,  513,  336,  513,  513, 1959,  303,  682,
  305,  450,  439, 1701, 1122,  730,  513, 1878, 1879, 1970,
  684,  661,  662,  824,  509,  432, 1247,  667, 1158, 1981,
  402,  585,  513,  446,  509,  446,  297,  980,   41,  446,
   61,   44,  379,  446,  444, 1757,  831, 1998, 1415,  344,
  493,  379,   91,  315,  450,  365,   59,  352, 2009,  739,
 1470, 2012,  402,   44,   59,  729,    0, 2011, 2019,  298,
 1437,  310,  736,  737,  336,  277,  262,  401,  879,    0,
  282,  477,  864, 1385, 1386,  264,   41,  268,  446,  272,
  315,  264,    0,  297,  337,  315, 1806,   41,    0,  739,
 1810,  262,  345,  315,  428,  334,  513,   41,  283,  315,
 1777,  336,   93,  513,  446, 1825, 1945,  790, 1878, 1879,
   41,  782, 1720,   44,  336,   59,  421,  276,  277,   41,
  336, 1167, 1842,   41, 1502,  273,   44, 1832,   59,   41,
  344, 1970,  782, 1838,  262, 1638, 1878, 1879,    0, 1878,
 1879,   59,  832,  336,  834,  303,  836,   59, 1184,  336,
 1186,  315,  494,    0, 1272,  484,  948,  300,  306, 1998,
  409, 1652, 1198, 1878, 1879, 1878, 1879,  817,  359,  303,
 2009,  513,  336, 2012,  446,   93,    0,  827, 1878, 1879,
 2019,  319,  832,  379,  834,    0,  836,  516,  298,  879,
 1236,  298, 1435,  843,  884,  434,   44,   59,  304,  305,
  348,  851,  852, 1923,  509,  348, 1926,  390,  379,  859,
  860,  446,   59,  372,  403,  889,  428,   41,  936,  305,
  910, 1339,  896,  873,  446,  875,  275, 1245,  878,  315,
  446,  263,  851,  907,  884,   59,  315,  277,  352,  338,
  890,  513,  282, 1963,   59,   93,    0,  937,  852,  401,
  940,  379,  410,  936,  873,  403,  875,  975,  976,  977,
  910, 1938,  945, 1940, 1458, 1536,  297, 1461,  453,  454,
  338, 1948, 1390, 1950, 1651,    0,  410,  309,  513,  962,
  954,  344,  446,    0,   42, 1096,    0,  937,  320,   47,
  940,  513,  975,  976,  977,  509, 1314,  262,  397,  427,
 1380,  400,  315,  371,  462,   59,  956,  421,  262, 1120,
  458,    0,  325,  956,  327,  304,  305,  416, 1995,   41,
    0,  280,   44,  336,  434,  397,  339,  434,  462,  397,
    0,  981,  400,  276,   59, 1777, 1777,  443,    0,    0,
  262,  991,   59,  442,  416,   59,  348,    0,  416,  999,
 1000, 1272,   41,  322,    0,   44,  999,  313,  314,  445,
  309,   41,  981,  339,   44, 1659,  445,  380,    0,    0,
   59,  315,  991,  316,  442, 1455,  389,  371,    0,   59,
    0,  325,  275,  327,  315,    0,  362,  419,    0,   59,
  453,  454,  336, 1770,  325,   41,  327,   59,   59, 1565,
 1071,  261,  264,  315,   93,  336,   59, 1475,  339,  238,
 1704, 1447,  259,   59,  379,  327,  410,  264,  377,  432,
  298, 1071,  269,  450,  336,  379,    0,   59,   59, 1221,
 1120,  444, 1364,  446,    0, 1367,  380,   59,  457,   59,
  459, 1477,  418,  297,   59,  389,  395,   59,  341,  380,
  393,  453,  454,  422,  443,    0,  334,  379,  389,    0,
  429,  290,  450,  457, 1114, 1115,   40,   41,  380,  298,
   44,  338,  431, 1536, 1536,   41,    0,  389,  258,  300,
 1130,  430,  297,    0,  402,   59,    0, 1161,  432,  448,
  433,  315,   44,   59,  348,  394, 1938, 1938, 1940, 1940,
  513,  432,  446,  327,  371,    0, 1948, 1948, 1950, 1950,
  264, 1229,  336,  444,   59,  446,  319,    0,   59,  277,
  432, 1898,    0,  303,  282,   44, 1244,  348, 1905,  344,
  397,  401, 1250,  400,  446,   59, 1793,  352,  399,  264,
  401,   93,   59,  390, 1355,   59, 1229,  264, 1238,  416,
 1361,  372, 1242, 1995, 1995,  284,  380,  319,  428,  265,
 1210, 1244,  276,  277,   59,  389,   44, 1250,  297,  513,
    0,  386, 1215,  388,   93,  442,   59, 1227, 1499, 1500,
    0,   59,  513, 1378, 1379, 1380, 1507, 1508, 1238, 1510,
 1511,  352, 1242,  299,  264,  410,    0, 1518, 1519, 1520,
 1521,  513, 1523, 1524, 1525, 1526, 1527, 1528,  432,   42,
   43,  264,   45,  260,   47,  276,  277,    0,  318, 1411,
  435, 1413,  446,   40,  409,  386,  271,  388,    0,   59,
  410,  331,  264,  264,    0,  315,  390, 1673,  258,   59,
  285,  397,  264, 1951, 1368,    0,   40,  327,  428,  410,
  270,  436,  264,  300,  354,   59,  336,  376,   41, 1310,
 1310,   44,    0,    0, 1360,  390, 1366, 1357, 1369,  315,
 1360, 1361, 1708,  390,  435,   41,   59,  297,   44,    0,
    0, 1473,  462,  303,  329,  305,    0,   59,  376,  513,
  336, 1999,  448,   59,  509,    0,    0, 1954,   42,   43,
  380,   45,  309,   47,   59,  405, 1356, 1357,    0,  389,
 1360, 1361,  357,  402, 1364, 1365, 1366, 1367, 1368, 1369,
  390,   59,   59,  338,  344,  485,  486,   41,   42,   43,
   44,   45,   46,   47,  380, 1385, 1386,  390,   59,   59,
    0,  315, 1392,  389,  318,   59,   60,   61,   62,  315,
  303,  325,  432,  327,   59,   59,  371,  331,  390,  390,
   44, 1365,  336,  372,  260,  339,  446,   59,  390,    0,
  336,  258,  309,  384,  385,  258,  421,   91,  390,  378,
  354, 1592,  397,  270,  393,  400,  432,  270,  395,    0,
  399,  396,  378,  338,  368,    0,  333,    0,  450,   59,
  446,  416,  376, 1839,  300,  342,  380,  412,  258,    0,
 1453,  261, 1486,  260,  380,  389,  303, 1491,  305,  426,
  303,  486,  305,  389,  433,  477,  371,  442,   59,    0,
 1641,  405,    0,  513,  441,  333,   41,  390,  258,   44,
    0,    0,   42,   43,  277,   45,    0,   47,   59,  282,
  270,  298,  397,  303,   59,  400,   59,  410,  432,   41,
  333,  398,   44,  310, 1571,  402,  432,  513,   59,    0,
  444,  416,  446,   41,  260,  428,   44,  297, 1529, 1529,
  446,   41,  410,  303,   44,  305,    0,  334,   59,  509,
  434,   59,  260,  276,  277,  368,  260,  442,  446,   59,
   59,  284, 1592,  336,  273,   59,  260,  380,  338,  462,
  276,  277,   42,   43,  300,   45,  513,   47,  284,  493,
  452, 1571,   42,   43,  344,   45,  333,   47,   59, 1579,
 1580, 1581,  300,  277,   41,   40,  300,   44,  282,  513,
  520,  371, 1592,  517,    0,   59,  300,  513,  262,   41,
   42,   43,  399,   45,  409,   47,  450,  271, 1965,  259,
 1967,  368,  409,  277,  501,  416, 1777,  397,  282,  269,
  400,  285,  485,  380, 1624, 1625,  513,  514,  515,  453,
  259,  436,  495,  477, 1795,   41,  416,  434,   44,  372,
  269, 1641,  336,  376,  453,  454,  318, 1792, 1648, 1649,
  298,  315,   41,   59,  318,   44,  372,  321,  264,  331,
  393,  325,  442,  327,  313,  329,  399,  331,   41,   46,
  315,   44,  336,  337, 1674,  339, 1395,  393, 1397,    0,
  333,   58,  354,  399,   61,   41,  334,  426,   44,   41,
  354,  355,   44,  357,   41,  332,  368,   44, 1747,  586,
  433,  453,  454,  590,  368, 1674,  370,  444,  372,  373,
  374,   41,  376,  403,   44,  379,  380,  433, 1624, 1625,
   41,  276,  277,   44,  336,  389, 1823,  277,   41,  284,
  395,   44,  282,  405,   41, 1624, 1625,   44,   59,  509,
   40,  405,  317, 1744, 1744,   42,   43, 1747,   45,  318,
   47,  399,   41, 1793,  376,   44,   41,  421,  315,   44,
  493,  409,  331,  427,  315,  315,  276,  277,  432,  376,
  339,  498,    0,  356,  284, 1824, 1776, 1938,   41, 1940,
  444,   44,  446,  442, 1945,  354,  434, 1948,   46, 1950,
  428,  426,  377, 1793, 1784, 1795,  410,  277, 1798,  368,
   58,   59,  282,   61,  319,   42,   43,  277,   45, 1970,
   47,    0,  282,   41, 1814,    0,   44,  372,  337, 1819,
 1917, 1845,  497,   41, 1824, 1849,   44,   41,  308,  493,
   44,   59,  375, 1890, 1995,  277,  405, 1998,  393,   41,
  282,  409,   44,   41,  399,  517,   44,  317, 2009,  513,
 1819, 2012,   41,  517,   41,   44,   41,   44, 2019,   44,
   41,   41,  372,   44,   44,  449,  450,  451,  508,  508,
   59, 1871,  759,   46,   59,  317,   61,  509,  433,   42,
   43,  284,   45,  393,   47,   58,   59,  306,   61,  399,
 1890, 1891,  779,  477,  478,  461,  783,   44,  446, 1892,
   61,  778,    0,  780,  366, 1945,  793,  784,  785,  786,
   40,  798,  799,  283, 1954,  802,  803,  804,  805,  806,
  807,  264,   40,  433,  389,   40,   40,   40,  453,  816,
 1970,  818,  819,  820,  821,  822,  823,   40,  493,  826,
   40,  262,  453,   41,   40, 1945,   44, 1937,  517,   40,
   40, 1951,   40,  453, 1954,   40,   40,   40, 1998,   40,
   40,   59,   41,   42,   43, 1965,   45, 1967,   47, 2009,
 1970,  351, 2012,   40,   40,  862,   40,   40,   40, 2019,
  277,   40,   40,  493,    0,  282,   91,  402,   44,   46,
   61,    0,   40,  264,  315,  882,  395,  318, 1998, 1999,
  317,   44,   44,  420,  325,   46,  327,  894,  895, 2009,
  331, 2011, 2012,   40,   61,  336,  318,  264,  339, 2019,
  317,  337,   46,   44,  344,   41,  344,  276,   44,  331,
  344,  276,   41,  354,  365,   44,   42,   43,  297,   45,
  277,   47,  264,   59,  390,  282,  428,  368,  276,  277,
   59,  390,  354,  450,  275,  376,  284,  344,  379,  380,
  439,  439,   42,   43,  392,   45,  368,   47,  389,   40,
  345,  308,   40,  258,  376,   40,   91,   40,  264,   41,
   42,   43,  315,   45,  405,   47,  376,  276,  277,   44,
  275,  276,  277,  403,  382,  284,  281,   42,   43,  284,
   45,  264,   47,  405,   40,  375,  427,   40,  297,  443,
  315,  432,  297,  380,  277,   60,   61,   62,  303,  282,
   41,  275,   63,  444,  296,  446,  314, 1014, 1015, 1016,
  428,  456, 1019, 1020, 1021,  428, 1023, 1024, 1025, 1026,
 1027, 1028,   40,   44,  372,  444,  440,  264,   44,  494,
 1037, 1038,   40, 1040,  337, 1042,   40,  273, 1045, 1046,
 1047, 1048, 1049, 1050,  262,  393,  297,  276,  297,  281,
  340,  399,  493,  340,  410,  379, 1063,  408, 1065,  379,
    0,   42,   43,  372,   45,  379,   47,  372,   40,   40,
  410,  493,  513,    0, 1081,  352,  517,   40,  277,   40,
  362,   41,   44,  282,  393,  433,  317,   41,  393,  277,
  399,   44,  438,  378,  399,  517,   46,  315,  270,  361,
  318,   41,  317,  408,   44,  410,   40,  325,  317,  327,
  266,   44,  325,  331,   41, 1122,   40,   44,  336,   59,
  493,  339,    0,  428,  433,   40,  264,   41,  433,   58,
  414,    0,   59,  262,   41,   41,  354,   41,  264,  264,
  276,  277,  446,  387,   41,  493,  306,  306,  284,   41,
  368,  277,   40,   40,   40,   40,  282,  462,  376,  317,
   40,  379,  380,   41,   40,  265,   44,   40,   40,   40,
   40,  389,   41,   42,   43,   44,   45,  277,   47,   40,
   40,   59,  282,   61,   40,   40,  315,  405,   40,  318,
   59,   60,   61,   62,   40,  277,  325,  427,  327,  299,
  282,  305,  331,  262,  446,   44,  271,  336,   44,  427,
  339,   41,  277,   41,  432,  264,  381,  282,  339,  339,
  285,  273,  327,   41,   93,  354,  444,  384,  446,   42,
   43,   44,   45,  284,   47,  399,  372,  379,  450,  368,
   41,   42,   43,   41,   45,  379,   47,  376,  379,  379,
  379,  380,  379,  372,  276,  379,  321,  393,   40,  317,
  389,   41,   41,  399,  329, 1272,   41,   41,   44,    0,
   41,  305,  337,   40,  427,  493,  405,  290, 1285, 1286,
   40, 1288, 1289,   42,   43,   44,   45,   59,   47, 1296,
  355,   58,  357,  336,  336,  513,  277,  433,  427,  517,
  305,  282,   59,  432,   58,  370,  428,  372,  373,  374,
   41, 1318,  324,   44,  339,  444,  333,  446,  532, 1326,
  277, 1328, 1329,  434,  334, 1332,  434, 1334,   59,  298,
  298, 1338, 1339, 1340,  334,   41,  276,  277,   41,  348,
 1347,  281, 1349,  378,  284,  365,  365,   41,   41,  276,
  277,   40,  378,   41,   41,   41,  421,  297,   41,   41,
   42,   43,   41,   45,  493,   47,   41,   41,   42,   43,
   41,   45,   41,   47,   41,   41,   42,   43,   41,   45,
  258,   47,   41, 1390,  513,   41,   41,   41,  517,  258,
   40,   44,   41,  262,  315,  264,  265,  275,  276,  277,
   40,  348,  271,  281,  443,   41,  284,  276,  277,  340,
  458,  333,   40,  282,  500,  284,  285,  336,  336,  297,
   40,   40,   44, 1430,  376,  303,   41,   40,  427,   41,
  299,  376,  372,  376,  303,  304,  305,   41,  301,  308,
  298,  434,   41,  290,  455,  372,  315,  304,  317,  318,
  301,    0,  321,  393,  333,   40,  325,  264,  327,  399,
  329,  409,  331,  376,  277,  376,  393,  336,  337,  282,
  339,  264,  399,  276,  450, 1019,  277, 1021,  403, 1023,
 1024,  282, 1026, 1027, 1028,  354,  355,   40,  357,   44,
  317, 1498,  443,  433,  372,   40,   40, 1504,   41,  368,
  270,  370, 1509,  372,  373,  374,  433,  376,  270, 1516,
  379,  380,  450,  264,  339,  393,  339,  427,  277,   41,
  389,  399,  450,  282,  393,   41,   59,   41,   59,   59,
  399,   59,  410,  402,   41,   59,  405,  305,   41,   42,
   43,  410,   45, 1550,   47,  276,  277, 1554,   59,   59,
  428,   41,  421, 1560,   59,  433,  301,   59,  427,  428,
    0,    0,  162,  432,  433,    0,    0,  161,  161, 1576,
 1376,  321,  852,  320,  443,  444,  583,  446,  581,  843,
 1247, 1245,  994,  452,  462,  956,  455,   95,  457,  458,
  459,  460,  743,  462,   81,  277,  999, 1237, 1242,  969,
  282,  470,  471,  277,   42,   43, 1692,   45,  282,   47,
  410,  277,  481,  482,    0,  484,  282, 1691,  444,  690,
 1227, 1170,  491,  492,  493,  494,  495,  370,  497,  884,
 1238,  500,  501,  502,  503,  504,  425,  425, 1671,  508,
  509,  372, 1814, 1650,  513,  514,  515,  516,  517,  518,
  519,  520, 1957,  522,   40,   41,   42,   43,   44,   45,
   46,   47,  393,  532, 1673,   41,   42,   43,  399,   45,
  667,   47,   58,   59,   60,   61,   62, 1961,  919, 1960,
   41,   42,   43, 1983,   45,  452,   47, 1111,  455,  910,
  457,  458,  459,  460, 1579,  462, 1408, 1781, 1946,  832,
 1655,  430,  433,  470,  471,   91,  415,   93,  408,  972,
  669,  939, 1462,  727,  481,  482, 1723,  484,  964,   13,
  409,  163, 1729, 1730,  491,  492,  355,  494,  495,  603,
  497,  599, 1130,  500,  501,  502,  503,  504,  167,  862,
 1396,  508,  509,   91, 1910,  873, 1753,  514,  515,  516,
 1757,  518,  519,  520, 1761,  522, 1744, 1529,  850, 1123,
  614, 1768, 1769,  452, 1417,  532,  455, 1287,  457,  458,
  459,  460,  493,  462,  277,  776,  894, 1334,   -1,  282,
   -1,  470,  471,   -1,   41,   42,   43,   -1,   45,   -1,
   47,   -1,  481,  482,   -1,  484,   42,   43,   44,   45,
   -1,   47,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  262,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   41,   42,   43, 1844,   45,  277,
   47,   -1,   -1,  532,  282,   41,   42,   43,   -1,   45,
   -1,   47,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  291,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,  302,   -1,   -1,   -1,
   -1,  277,   -1,  309,   -1,   -1,  282,  313,   -1,  285,
   -1,  277,   -1, 1910,   -1,    0,  282,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,  277,  333,  304,  305,
   -1,  282,  308,   -1,   41,   42,   43,   -1,   45,  315,
   47,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
 1947,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,  395,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,  464,  465,  466,  467,  468,
  469,  470,  471,  389,   -1,   91,   -1,   93,   -1,   -1,
  426,   41,   42,   43,  430,   45,  402,   47,   -1,  405,
  277,   -1,   -1,   -1,   -1,  282,   41,   42,   43,   -1,
   45,  277,   47,   -1,   -1,  421,  282,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   41,   42,   43,   -1,   45,  452,   47,   -1,  455,
  277,  457,  458,  459,  460,  282,  462,   -1,   -1,   -1,
   -1,  277,   -1,   -1,  470,  471,  282,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,  481,  482,  277,  484,   -1,
   -1,   -1,  282,  519,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,  532,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,  258,   -1,  282,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,  301,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   41,   42,   43,   -1,   45,  315,
   47,  317,  318,   -1,   -1,  321,   41,   42,   43,  325,
   45,  327,   47,  329,   -1,  331,   41,  277,   -1,   44,
  336,  337,  282,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,  277,   -1,   59,   -1,   -1,  282,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   41,
   42,   43,  368,   45,  370,   47,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,  277,  393,   -1,
   -1,   -1,  282,  389,  399,   91,   -1,   93,   41,   42,
   43,   -1,   45,   -1,   47,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   -1,
   45,   -1,   47,  428,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,  455,
   -1,  457,  458,  459,  460,  277,  462,   -1,   -1,   -1,
  282,   -1,   -1,  277,  470,  471,   42,   43,  282,   45,
   -1,   47,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,    0,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,  262,  282,  264,  265,
   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,  299,    0,  277,   -1,   -1,  304,  305,
  282,   -1,  308,   -1,   -1,   -1,   59,   -1,   61,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,  277,  331,   -1,   -1,   -1,  282,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,  277,   -1,   -1,   -1,   -1,  282,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
  291,   -1,   -1,  389,  399,   91,  262,   93,   -1,   -1,
   -1,  302,   -1,   -1,   -1,   -1,  402,  302,  309,  405,
    0,  277,   -1,   -1,  309,   -1,  282,   -1,  313,  314,
   -1,  322,   -1,   -1,   -1,  421,   -1,  322,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,  333,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   41,   -1,   -1,   44,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,  277,  462,   -1,   -1,   59,
  282,   -1,   -1,   -1,  470,  471,   -1,   -1,  379,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,  395,  491,  492,  493,  494,  495,
  395,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  411,   -1,  508,  509,   -1,  258,  411,  513,  514,  515,
  516,  517,  518,  519,  520,  426,  522,   -1,   -1,  430,
  273,  426,  433,   -1,   -1,  430,  532,   -1,   -1,   -1,
  441,   -1,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  297,   -1,   -1,   -1,   -1,  460,
  303,   -1,   -1,  306,   -1,  460,  262,   -1,  264,  265,
   -1, 1499, 1500,   -1,   -1,  271,   -1,   -1,   -1, 1507,
 1508,  277, 1510, 1511,   -1,   -1,  282,   -1,   -1,  285,
 1518, 1519, 1520, 1521,   -1, 1523, 1524, 1525, 1526, 1527,
 1528,   -1,   -1,  299,    0,  348,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,  519,  315,
   -1,  317,  318,   -1,  519,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
  403,  357,   -1,   59,   60,   61,   62,  410,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,  428,  276,  277,   -1,   -1,
   -1,   -1,   -1,  389,  284,   91,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,  297,   -1,  405,
   -1,   -1,   -1,   -1,   -1,  458,   -1,   -1,   -1,  462,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,  372,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,  393,  500,  501,  502,  503,  504,  399,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,  387,   -1,  262,   -1,   -1,   -1,
   -1,   -1,  395,    0,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  426,   -1,   -1,   -1,  430,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  441,  315,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   59,  329,   -1,  331,   -1,  460,  461,   -1,
  336,  337,   -1,  339,   -1,    0,   -1,  272,   -1,   -1,
  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,   -1,  292,   -1,   -1,
  295,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,  389,   -1,   -1,  519,   -1,   -1,  522,
   -1,   -1,   -1,   -1,   59,    0,   -1,   -1,   -1,  405,
  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,  421,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,  367,   -1,  369,   -1,   41,   -1,  444,   44,
  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  398,   -1,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,
  415,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  437,   -1,   -1,   -1,  315,  513,   -1,  318,
   -1,  517,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,  262,   -1,  336,  337,   -1,
  339,   41,   -1,   -1,   44,   -1,   -1,  472,  473,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   59,
   -1,   -1,   -1,   -1,  489,   -1,   -1,   -1,   -1,  368,
   -1,  370,    0,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,  262,   -1,  336,
   -1,   -1,  339,   41,   -1,   -1,   44,   -1,   -1,   -1,
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
   -1,  331,  493,  405,   42,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  513,   -1,  354,  427,  517,   -1,   -1,    0,
  432,  262,   -1,   -1,   -1,   -1,   -1,   41,  368,   -1,
   44,   -1,  444,   -1,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,   59,   -1,
   -1,  493,   -1,   -1,  325,   -1,  327,  427,   -1,   -1,
  331,   -1,  432,   -1,   -1,  336,   -1,   -1,  339,   -1,
   -1,  513,   -1,   -1,  444,  517,  446,  315,   44,   -1,
  318,   -1,   -1,  354,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,  368,  336,   -1,
   -1,  339,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,  389,   -1,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,   -1,  405,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,  513,   -1,   -1,   -1,  517,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,  444,  315,  446,   -1,  318,   -1,   -1,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,  427,
  331,   -1,   -1,   -1,  432,  336,   -1,   -1,  339,   -1,
   40,   -1,   -1,   43,   -1,   45,  444,   -1,  446,   -1,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,  493,   63,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,   -1,  318,  513,   -1,   -1,   -1,  517,
   -1,  325,   -1,  327,   -1,   -1,  427,  331,   -1,   -1,
   -1,  432,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,  315,  446,   -1,  318,   -1,   -1,
  354,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,  368,  336,   -1,   -1,  339,   -1,
   -1,   -1,  376,   -1,   -1,   45,  380,   -1,   -1,   -1,
   -1,   -1,   -1,  354,   -1,  389,   -1,   -1,   -1,   -1,
   -1,  317,  493,   -1,   -1,   -1,   -1,  368,   -1,   -1,
   -1,  405,   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,
   -1,   -1,  513,   -1,  452,   -1,  517,  455,  389,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,  432,   -1,
   -1,   -1,  470,  471,  405,   -1,   -1,   -1,   -1,   -1,
  444,   -1,  446,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,  432,  500,  501,  502,  503,  504,  267,   -1,   -1,
  508,  509,   -1,  444,  274,  446,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,  286,   -1,   -1,  493,
   -1,   -1,  292,  293,  532,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  513,
   -1,  311,   -1,  517,   58,   -1,   -1,   -1,   -1,   63,
  320,   -1,  493,  323,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,  513,   -1,  470,  471,  517,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,  372,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,  409,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  505,  506,  507,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,  267,  524,  525,  526,   -1,  528,  529,
  274,  531,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,  289,   -1,   -1,  292,  293,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,
  450,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   41,   42,   43,
  372,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,
   -1,   -1,  348,  277,  350,  351,   -1,   -1,  282,   -1,
   -1,  285,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  321,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  337,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,  284,  425,
   -1,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,  370,   -1,  372,  373,
  374,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,  421,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,  521,  522,   -1,  524,  525,
  526,  527,  528,  529,  530,  531,  532,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,  311,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   58,  452,   -1,   -1,  455,
   63,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,  372,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,
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
  325,   -1,  472,  473,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,  489,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,  524,  525,
  526,  260,  528,  529,  263,  531,  532,   -1,  267,   -1,
  395,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  430,   -1,   -1,   40,   -1,
  435,   43,   -1,   45,   -1,   -1,  441,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   58,  452,   -1,   -1,
  455,   63,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,  524,  525,  526,  260,  528,
  529,  263,  531,  532,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,  435,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
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
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
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
   -1,   -1,   -1,   -1,  425,  284,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
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
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,
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
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,  300,
   -1,   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   58,   -1,  323,   -1,   -1,   63,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,  360,
   -1,   -1,   -1,  364,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
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
   -1,  522,  260,  524,  525,  526,   -1,  528,  529,  267,
  531,  532,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   40,   -1,
   42,   43,  300,   45,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  267,  524,  525,  526,   -1,  528,  529,  274,  531,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
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
   -1,   -1,  292,  293,   -1,   -1,   40,   41,   -1,   43,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,
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
   -1,   -1,   58,   -1,   -1,  317,   -1,   63,  320,   -1,
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
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  267,  524,  525,  526,   -1,  528,  529,  274,  531,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,
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
  446,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
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
  518,  519,  520,    0,  522,  267,  524,  525,  526,   -1,
  528,  529,  274,  531,  532,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   41,   -1,  320,   44,
   -1,  323,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   41,   91,   -1,   44,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   91,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,    0,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  258,  524,  525,  526,   41,  528,  529,   44,  531,
  532,   -1,   -1,  258,   -1,   -1,  273,   -1,  275,  276,
  277,   -1,    0,   59,  281,   61,   -1,  284,  273,   -1,
  275,  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,
  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,
  258,   -1,  297,   -1,   -1,   91,   -1,   -1,  303,   -1,
   -1,  306,   -1,   41,   -1,  273,   44,   -1,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,  297,
   -1,  348,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   91,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,
  348,   -1,  399,   -1,   -1,   -1,  403,   -1,  393,   -1,
   -1,   -1,   -1,  410,  399,   -1,   -1,   -1,  403,   -1,
   -1,  261,   -1,   -1,  372,  410,   -1,   -1,   -1,   -1,
   -1,  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,  279,
   -1,   -1,   -1,  428,   -1,  393,   -1,  287,  433,   -1,
   -1,  399,   -1,   -1,  294,  403,  296,   -1,  298,   -1,
   -1,  458,  410,  303,   -1,  462,   -1,   -1,   -1,   -1,
  310,   -1,   -1,  458,   -1,   -1,   -1,  462,   -1,   -1,
  428,   -1,  258,   -1,  324,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  334,   -1,   -1,  273,   -1,   -1,
  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,
  458,   -1,   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,  297,   -1,   -1,  260,   -1,   -1,  303,   -1,   -1,
  306,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  273,   -1,  397,  276,  277,
   -1,   -1,  298,  281,  404,   -1,  284,  407,   -1,  409,
  410,   -1,  348,   -1,  310,   -1,   -1,   -1,   -1,  297,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
   -1,   -1,   -1,   -1,  434,   -1,  372,   -1,  334,   -1,
  260,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,
   -1,   -1,   -1,  399,   -1,   -1,   -1,  403,   -1,   -1,
  348,   -1,   -1,   -1,  410,   -1,   -1,   -1,  298,   -1,
   -1,   -1,   -1,  483,   -1,   -1,   -1,   -1,   -1,   -1,
  310,   -1,  428,   -1,  372,   -1,   -1,  433,  498,  499,
   -1,   -1,   -1,  399,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  409,  334,  393,   -1,   -1,   -1,   -1,
   -1,  399,  458,  523,   -1,  403,  462,   -1,   -1,   -1,
   -1,   -1,  410,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  428,   -1,   -1,   -1,   -1,  433,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,  399,
  458,   -1,   -1,   -1,  462,  481,  482,   -1,  484,  409,
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
   -1,   -1,  500,  501,  502,  503,  504,   -1,  339,   -1,
  508,  509,   -1,   -1,   -1,  415,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,  362,   -1,   -1,  532,   -1,   -1,  437,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  418,   -1,  489,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,  452,  522,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,  532,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  532,  455,  456,  457,  458,  459,  460,  461,  462,
  463,  464,  465,  466,  467,  468,  469,  470,  471,  472,
  473,  474,  475,  476,   -1,   -1,  479,  480,  481,  482,
  483,  484,   -1,   -1,  487,  488,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  498,  499,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,
  513,  514,  515,  516,  517,  518,  519,  520,  521,  522,
  523,  524,  525,  526,  527,  528,  529,  530,  531,  532,
  452,   -1,   -1,  455,  456,  457,  458,  459,  460,   -1,
  462,  463,  464,  465,  466,  467,  468,  469,  470,  471,
  472,  473,  474,  475,  476,   -1,   -1,  479,  480,  481,
  482,  483,  484,   -1,   -1,  487,  488,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  498,  499,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,
  512,  513,  514,  515,  516,  517,  518,  519,  520,  521,
  522,  523,  524,  525,  526,  527,  528,  529,  530,  531,
  532,
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
