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
   84,   15,   90,   90,   90,   70,   89,   93,   95,   71,
   71,   74,   74,   74,   82,  102,  102,  102,  103,  103,
  104,  104,  105,  105,  109,  109,   81,  112,  112,  117,
  114,  114,  115,  115,  119,  119,  120,  121,  121,  122,
  123,   79,   83,   80,  106,  106,  126,  127,  127,  129,
  129,  130,  130,  130,  130,  130,  128,  128,  131,  131,
  132,  132,  132,  110,  134,  135,  135,  136,  136,  137,
  137,  138,  138,  138,  108,  108,  108,   76,   86,  139,
  139,  139,  140,  140,  141,  141,  142,  142,  142,  147,
   98,   98,  144,  144,  116,  116,  111,  151,  152,   62,
  118,  118,  118,  118,  118,  118,  145,  145,  159,  159,
  160,  162,  162,  162,  162,  162,  143,  161,  161,  165,
  165,  165,  165,  167,  168,  169,  164,  164,  163,  163,
  163,  163,  163,  170,  171,  172,  172,  172,  172,   75,
   85,   91,  177,  173,  173,  174,  174,  178,  178,  180,
  179,  179,  182,  181,  181,  181,  175,  175,  183,  183,
  184,  186,  186,  187,  185,  185,  189,  189,  189,  188,
  191,  191,  176,  193,  193,  193,  194,  194,  192,  192,
  196,  196,  196,  196,  196,  196,  196,  196,  196,  196,
  196,  196,  196,  196,  196,  196,  197,  197,  197,  197,
  203,  203,  204,  200,  210,  211,  201,  208,  208,  205,
  215,  199,  216,  213,  213,  213,  213,  209,  212,  212,
  207,  207,  207,  214,  214,  195,  195,  220,  221,  221,
  222,  222,  222,  222,  206,  206,  206,  224,  226,  225,
  227,   12,  228,  228,  228,  229,  229,  229,   13,  231,
  231,  232,  232,  233,   78,   87,  149,   99,  101,  236,
  236,   77,   88,   92,  237,  237,  237,  238,  241,  241,
  242,  242,  242,  242,  242,  242,  242,  242,  242,  242,
  242,  242,  242,  242,  242,  239,  239,  240,    2,  244,
  244,  244,  244,  244,  244,  244,  244,  244,  250,  250,
  254,  254,  254,  254,  254,  254,  254,  245,  245,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  249,  260,
  260,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  260,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  260,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  260,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  257,  257,  259,  259,  258,  256,  256,  256,  251,  251,
  252,  252,  252,  253,  263,  263,  264,  264,  247,  248,
  248,  265,  265,  265,  265,  265,  246,  266,  266,  267,
  267,    9,  268,  268,  268,  268,  268,  268,  268,  268,
  268,  268,  268,  268,  268,  113,  113,  146,  146,  271,
  271,  274,  274,  276,  276,   57,   57,  273,  273,  273,
  273,  273,  273,  273,  273,  273,  280,  280,  272,  272,
  272,  272,  282,  282,  281,  281,  281,   59,   59,  277,
  277,  277,  275,  275,  275,  286,  286,  286,  285,  285,
  284,  284,  284,  278,  278,  287,  287,  287,  288,  288,
  279,  279,  279,  279,  289,  289,   20,   20,   20,  291,
  291,  291,   18,   18,   18,  293,  294,  297,  297,  295,
  299,  299,    5,   17,  298,  298,  300,  300,  301,  301,
  290,  302,  302,  303,  303,  304,  304,  304,  304,  305,
  305,  306,  306,  307,  307,  309,  309,  309,  310,  310,
  310,  311,  311,  311,  308,  313,  313,  313,  314,  314,
  312,  312,  315,  317,  317,  316,  316,  292,    4,    4,
    4,  318,  320,  320,  320,  320,  320,  320,  320,  320,
  320,  320,  320,  320,  320,  320,  322,  322,  323,  319,
  319,   19,  324,  324,  326,  326,  325,  325,  235,  330,
  331,  327,  327,  327,  332,  334,  343,  344,  345,  346,
  335,  335,  335,  335,  347,  347,  347,  348,  348,  348,
  336,  336,  337,  337,  350,  350,  351,  351,  352,  352,
  338,  354,  354,  355,  355,  357,  357,  357,  359,  360,
  360,  361,  361,  363,  363,  356,  356,  356,  364,  365,
  366,  368,  368,  369,  370,  358,  358,  371,  371,  372,
  372,   25,  367,  367,  367,  367,  367,  373,  373,  340,
  340,  374,  374,  375,  341,  341,  339,  339,  342,  342,
  376,  377,  377,  377,  377,  377,  378,  378,  379,  379,
  380,  380,  381,  381,  381,  382,  382,  383,  383,  328,
  328,  384,  384,  385,   72,   72,   72,  387,  387,  386,
  386,  329,  329,  329,   11,   11,    8,    8,  390,  391,
   21,   21,  393,  394,  389,  389,  389,  392,  395,  395,
  198,  202,    3,    3,  398,  398,  398,  400,  400,  401,
  399,  399,  402,  402,   35,   35,   97,   96,   96,  388,
  388,  403,  404,  404,  218,  218,  218,  148,  396,  396,
  124,  124,  124,  124,  124,  408,  408,  408,  408,  408,
  408,  407,  407,  407,  410,  410,  410,  410,  410,  410,
  410,  410,  410,  410,  410,  411,  411,  411,  411,  411,
  411,  411,  411,  417,  417,  417,  417,  417,  417,  417,
  417,  417,  417,  417,  417,  417,  417,  417,  417,  422,
  422,  412,  412,  413,  413,  414,  414,  414,  414,  415,
  415,  419,  419,  420,  420,  420,  420,  418,  421,  416,
  416,  406,  406,  406,  409,  409,  409,  423,  423,  424,
  100,  100,  100,  100,  100,  100,  100,  100,  100,  100,
  100,  100,  100,  100,  100,  100,  100,  100,  100,  100,
  100,  100,  100,  100,  100,  100,  100,  100,  158,  158,
  158,  431,  431,  425,  230,  230,  153,  153,  432,  432,
  432,  432,  432,  427,  427,  427,  427,  427,  234,  154,
  154,  155,  156,  156,  156,  156,  156,   55,   55,   22,
   22,  243,  433,   58,  283,  262,  262,  107,  426,  426,
  426,  434,  434,  434,  434,  434,  434,  434,  434,  434,
  434,  434,  435,  435,  437,  438,  438,  438,  440,  441,
  441,  442,  436,  436,  436,  436,  443,  445,  445,  444,
  444,  444,  444,  446,  446,  446,  397,  397,  428,  429,
  429,  447,  447,  447,  448,  448,  449,  449,  452,  452,
  450,  450,  455,  455,  454,  451,  453,  430,  430,  439,
  439,  439,  439,  439,  439,  439,  439,  349,  349,  333,
  333,  157,   52,   68,  133,  150,  405,  166,  190,  270,
   94,   66,  223,  125,   73,  353,  219,  321,   36,   45,
  362,  269,   43,   46,  217,   44,  296,  261,  261,  456,
  456,  456,  456,  456,  456,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,  456,  456,  456,  456,
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
    2,    4,    2,    2,    2,    2,    2,    2,    2,    1,
    0,    1,    1,    6,    6,    1,    1,    0,    0,    1,
    0,    4,    0,    1,    1,    2,    8,    1,    0,    3,
    1,    0,    1,    0,    1,    2,    1,    1,    1,    2,
    6,    1,    1,    3,    0,    1,    1,    0,    1,    1,
    2,    3,    4,    2,    2,    3,    0,    1,    1,    2,
    1,    4,    3,    3,    1,    0,    1,    1,    2,    3,
    4,    0,    1,    2,    0,    1,    1,    5,    5,    3,
    2,    0,    1,    3,    1,    1,    6,    3,    2,    6,
    2,    1,    2,    2,    2,    0,    1,    1,    2,    0,
    1,    1,    1,    1,    1,    1,    0,    1,    1,    2,
    2,    1,    1,    5,    2,    3,    2,    2,    0,    1,
    1,    1,    1,    3,    4,    8,    4,    0,    1,    1,
    2,    2,    0,    3,    3,    1,    2,    2,    2,    8,
    8,    8,    8,    3,    0,    4,    0,    1,    3,    5,
    1,    3,    2,    3,    3,    0,    1,    0,    1,    2,
    4,    1,    1,    3,    1,    0,    2,    2,    0,    6,
    1,    1,    3,    1,    2,    0,    1,    2,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    3,    1,    3,    8,    9,    5,    8,    6,    3,
    0,    3,    2,    1,    1,    3,    3,    7,    2,    0,
    1,    1,    2,    3,    0,    1,    2,    4,    1,    3,
    2,    2,    2,    1,    1,    1,    1,    2,    2,    5,
    0,    5,    1,    3,    0,    2,    4,    0,    7,    3,
    0,    1,    3,    3,    7,    7,    0,    0,    0,    3,
    0,    8,    8,    8,    1,    1,    0,    2,    1,    1,
    1,    1,    1,    3,    3,    3,    3,    3,    3,    5,
    5,    5,    5,    5,    5,    2,    0,    4,    2,    2,
    3,    2,    2,    3,    3,    2,    2,    3,    1,    2,
    3,    3,    2,    2,    2,    2,    3,    1,    3,    3,
    3,    2,    2,    4,    4,    4,    5,    4,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    1,    1,    1,    1,    1,    0,    2,    2,
    4,    4,    5,    3,    2,    0,    2,    0,    0,    1,
    2,    2,    4,    3,    2,    2,    7,    1,    0,    1,
    0,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    3,    2,    2,    2,    2,    1,    1,    1,    1,    4,
    5,    1,    3,    1,    3,    1,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    4,    4,
    6,    5,    3,    0,    2,    2,    0,    3,    0,    4,
    1,    5,    4,    1,    4,    1,    2,    2,    1,    1,
    1,    2,    2,    2,    2,    0,    3,    5,    1,    1,
    2,    3,    1,    2,    3,    0,    1,    1,    1,    5,
    5,    6,    1,    1,    1,    2,    4,    1,    0,    5,
    1,    0,    3,    3,    1,    0,    2,    0,    1,    0,
    3,    1,    0,    1,    2,    1,    1,    1,    1,    2,
    2,    1,    2,    3,    1,    1,    3,    3,    1,    2,
    3,    1,    2,    0,    2,    1,    1,    0,    1,    1,
    1,    3,    2,    3,    0,    1,    3,    4,    5,    6,
    7,    1,    1,    1,    1,    1,    1,    2,    1,    1,
    1,    1,    1,    1,    2,    1,    1,    1,    2,    1,
    1,    3,    3,    0,    2,    0,    2,    0,    3,    3,
    3,    1,    4,    4,    1,    9,    0,    0,    0,    0,
    3,    2,    1,    0,    3,    5,    3,    2,    5,    2,
    1,    1,    1,    1,    1,    3,    1,    3,    1,    0,
    2,    1,    3,    1,    1,    1,    1,    3,    6,    1,
    0,    3,    0,    1,    3,    1,    1,    1,    4,    5,
    5,    1,    1,    2,    4,    4,    2,    3,    0,    1,
    2,    1,    1,    2,    2,    2,    0,    1,    0,    3,
    0,    1,    3,    1,    2,    0,    2,    0,    2,    0,
    4,    1,    2,    1,    1,    0,    1,    3,    2,    1,
    1,    2,    1,    4,    3,    1,    3,    4,    0,    3,
    0,    1,    3,    3,    1,    1,    0,    1,    1,    4,
    0,    2,    4,    0,    9,    5,    1,    1,    7,    4,
    1,    1,    8,    5,    2,    4,    0,    4,    1,    3,
    3,    1,    7,    7,    5,    3,    0,    1,    1,    1,
    2,    0,    1,    1,    1,    0,    3,    1,    3,    1,
    0,    3,    1,    3,    1,    3,    3,    1,    1,    3,
    1,    2,    1,    3,    3,    1,    2,    3,    3,    3,
    3,    1,    3,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    3,    3,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    1,
    1,    5,    6,    5,    6,    3,    4,    5,    6,    3,
    4,    3,    4,    3,    4,    4,    5,    4,    4,    3,
    4,    1,    1,    1,    1,    1,    1,    1,    3,    3,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    3,    3,    3,    3,    3,    3,    3,    3,
    1,    1,    1,    1,    3,    1,    1,    1,    1,    2,
    2,    3,    0,    4,    1,    3,    1,    2,    1,    1,
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
   21,   22,    0,  587,  588,  589,  593,  594,  595,    0,
  682,  685,  797,  798,  801,  802,  499,    0,    0,    0,
    0,    0,    0,    0,    0,  409,  605,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  130,    0,    0,
    0,  103,    0,  982,    0,   23,  980,    0,    0,   74,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  512,    0,    0,    0,   37,   34,
   36,    0,   35,    0, 1088, 1090, 1091, 1092, 1093, 1094,
 1095, 1114, 1115, 1096, 1098, 1097, 1103, 1099, 1104, 1100,
 1102, 1105, 1101, 1106, 1107, 1108, 1110, 1111, 1109, 1112,
 1113, 1117, 1116, 1118, 1119, 1120,    0,    0,    0,    0,
   32,   69,   67, 1080, 1089,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  693,    0,    0,
    0,  740, 1082,    0,    0,    0,    0,    0,    0,    0,
  116, 1087,  596,    0,    0,    2,    0,    0,    0,    0,
    0,    0,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
  459,  460,  461,  469,  463,  464,  462,  465,  466,  467,
  468,  470,  472,  475,  471,  473,  474,  476,  477,  478,
  479,  480,    0,  439,  440,    0,  410, 1071,    0,    0,
  416, 1075,    0,  413, 1079,    0,  742,    0,  412, 1083,
    0,  417, 1074,    0,  603,  978,    0,  167,  112,    0,
  113,    0,  217, 1067,  838,  104,    0,  110,  162,    0,
    0,  106,    0,  114,  984,    0,  107,    0,  108,    0,
  109, 1086,  163,  115,  111,  785,  786,    0,  983,  981,
    0,   75,    0, 1072,    0, 1070,  520,  513,    0,  519,
  524,  514,  515,  522,  516,  517,  518,  523,  525,    0,
    0,    0,   30,    0,   39,  825,   38,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  604,  988,    0,  969,  687,  687,  688,  698,  700,
 1058,  701,    0,  702,  692,  688,    0,    0,    0,    0,
    0,    0,    0,  622,  611,    0,  614,  616,  617,  618,
  619,  625,  626, 1081,  741,    0,  121,    0,    0,  117,
    0,  118,    0,  120,    0,  119,    0,    0,  652,  653,
  660,    0,  659,  661,  663,  668,  656,  654,  657,  655,
  664,  667,  662,  666,    0,    0,    0,    0,    0,  672,
    0,    0, 1060,    0,    0,  679,    0,    0,    0,    0,
    0,  500,    0,    0,    0,    0,  220,    0,  419,  129,
 1063,    0,  418,  489,  490,    0,    0,    0,    0,    0,
    0,  428,    0,  385,  386,    0,    0,  609,  607,  979,
    0,    0,    0,    0,    0,    0,    0,  170,  148,    0,
  126,    0,    0,    0,  122,    0,  137,  136,    0,    0,
    0,    0,    0,    0,   76,    0,    0,    0,    0,  800,
  521,    0,    0,    0,    0,  944,  975,    0, 1062,    0,
  946,    0,    0,  976,    0,  970,  961,  960,  959,    0,
    0,    0,    0,  949,    0,    0,  962,  963,  971,  972,
    0,  973,  974,    0,    0,    0,  977,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  965,    0,  835,  941,  942,  943,  948,  947,  926,    0,
    0,    0,  925,    0,    0,  930,  922,  923,  924,  927,
  928,  929,  964,  989,  990,  991, 1003, 1004, 1006, 1007,
 1008, 1013, 1014, 1030, 1031, 1035, 1036,  218,    0,    0,
    0,  372,    0,    0,    0,  828,   27,    0,    0,    0,
    0,   72,   70, 1084,   68,   33,    0,    0,    0,  830,
    0,   48,   50,    0,    0,    0,  601,    0,    0,    0,
  695,  697,    0,  704,    0,    0,    0,  705,  691,    0,
  648,    0,  623,    0,  620,    0,  621,  646,    0,  641,
    0,    0,  615,  839,  809,    0,    0,    0,  127,    0,
    0,    0,    0,  598,  597,  665,  658,    0, 1078,    0,
    0,    0,  673,  677,    0,    0,  782,  684,  683,    0,
  185,    0,    0,  145,    0,  505,    0,  506,  501,  378,
    0,  423,  425,  424,  378,  379,  426,    0,  420,    0,
    0,    0,    0,  268,    0,    0,    0,  432,  433,    0,
  482,    0,    0,    0,    0,    0,    0,    0,  390,  389,
  508,    0,    0,    0,  166,    0,    0,  175,    0,  174,
    0,    0,  181,  164,    0,  179,  171,    0,    0,  544,
  579,  547,    0,  580,    0,    0,  548,    0,    0,    0,
  571,  583,  543,  566,  545,  546,  541,  528,  378,  526,
  527,  529,    0,    0,  538,  539,  540,  542,    0,    0,
    0,    0,  125,    0,    0,  123,    0,  124,    0,  140,
    0,    0,  201,    0,    0,  377,    0,  220,    0,    0,
   84,    0,  102,    0,  101, 1064,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  841,  843,  852,  855,
  856,  857,  858,  859,  860,  861,  862,  863,  864,  865,
    0,    0,    0,    0,    0,  966,    0,    0,    0,    0,
    0,    0,  967,  968,    0,  950,  951,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  333, 1085,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  362,
    0,    0,  273,  536,    0,    0,    0,  370,    0,    0,
    0,  827,    0,    0,   44,   73,    0,    0,   26,    0,
  833,    0,    0,  796,    0,    0,    0,    0,  600,    0,
  687,    0,  709,    0,    0,    0,    0,  591,    0,  986,
  590,    0,  624,    0,  632,  628,  627,    0,    0,    0,
  643,  631,    0,    0,    0,  804,    0,    0,    0,    0,
  377,  671,  670,  649,    0,    0,    0,    0,    0,    0,
    0,    0,  146,    0,  504,    0,  422,    0,  421,  427,
  495,    0,  494,  378,    0,  264,  377,  238, 1068,    0,
    0,  378,    0,    0,  209,  377,    0,    0,    0,  243,
  237,  240,  241,  242,  431,  487,  486,  430,  429,    0,
    0,    0,    0,    0,    0,    0,    0,  388,    0,  378,
  492,    0,  491,    0,  172,  176,    0,    0,  180,    0,
    0,    0,  567,  568,  584,    0,  581,    0,    0,  574,
  572,  573,    0,    0,    0,    0,  537,    0,    0,    0,
    0,  575,  128,    0,    0,  377,    0,  200,    0,  203,
  205,  206,    0,    0,    0,    0,    0,  220,   79,    0,
    0,   78,    0,    0,    0,    0,  842,  854,    0,    0,
  915,  916,  917,    0,  846,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  762,  764,    0,  759,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1050, 1051, 1052, 1053, 1054, 1055,
 1056, 1057,    0,    0,    0,    0,    0,    0,    0, 1041,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1024, 1026, 1025,    0,    0,  939,  364,    0,  940, 1028,
    0,  935, 1066,    0,    0,    0,    0,    0,    0,    0,
    0,  334,  335,    0,    0,  945,  837,  836,  374,  373,
    0,  271,    0,    0,    0,  279,  829,    0,    0,    0,
   71,    0,    0,    0,  832,    0,    0,    0,    0,  696,
  699, 1076,  708,    0,    0,    0,    0,  714,  715,  716,
  717,  726,  727,  728,    0,    0,  706,  592,  987,  633,
  642,  637,  636,    0,  647,  840,  810,    0,    0,  377,
    0,    0,    0,  650,  669,    0,  689,  784,  783,    0,
  503,    0,    0,  184,    0,  188,    0,    0,  150,  221,
  222,  223,  224,  225,  226,  957,  497,    0,  269,    0,
  211,  378,  213,  379,  151,  208,  214,    0,    0,    0,
  434,  435,  438,  379,  483,  484,  436,    0,    0,    0,
  406,    0,  493,  196,  197,  173,    0,  183,  555,  556,
  985,    0,    0,    0,    0,    0,  582,    0,    0,  379,
    0,    0,  532,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  198,    0,    0,    0,   83,
    0,    0,    0,    0,   91,   85,    0,    0,   90,   86,
    0,  808, 1069,    0,    0,  847,    0,    0,  853,    0,
    0,    0,    0,    0,  891,  890,    0,    0,    0,  900,
  918,    0,  910,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  845,    0,  763,    0,  799,    0,    0,
  220,    0,  992,    0,    0, 1015,    0,    0,    0,    0,
    0,    0,    0,  952,    0,    0,    0,    0,    0, 1037,
    0,    0,    0,    0,    0,    0,    0, 1048, 1009, 1010,
 1011, 1016, 1012,    0, 1023,    0,    0,  681, 1027,  954,
    0,    0,    0,    0,  266,  285,    0,    0,  369,  280,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   54,
   62,   64,    0,   42,    0,    0,    0,  814,  834,    0,
  813,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  743,    0,    0,    0,    0,    0,    0,    0,  639,  640,
  644,    0,    0,  199,    0,    0,  651,    0,    0,    0,
    0,  189,    0,  958,  378,  378,  379,    0,    0,    0,
    0,    0,    0,  244,  437,    0,    0,    0,    0,    0,
    0,  378,  510,  379,  182, 1065,    0,    0,  550,    0,
  549,  585,    0,  577,    0,    0,    0,  530,  558,    0,
    0,  560,  563,  565,  377,    0,    0,    0,  135,    0,
  204,    0,    0,  379,  105,    0,  133,    0,   94,    0,
    0,    0,    0,   88,   89,    0,  908,  909,  849,    0,
  851,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  911,    0,    0,    0,    0,    0,    0,    0,  901,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  770,    0,    0,    0,  998,  997,
    0,    0,  994,  993,    0, 1002, 1001, 1000,  999,  996,
  995,    0,    0, 1043,    0,    0,    0, 1042,    0,    0,
    0,    0,    0,    0,    0,  367,  337,  336,  272,    0,
    0,  282,  283,    0,    0,    0,    0,  316,    0,    0,
    0,  315,    0,    0,    0,    0,  361,  304,  302,  306,
    0,  303,  292,  297,  291,    0,    0,    0,  300,  301,
  305,  307,  308,  309,  310,  311,  312,  313,  314,  317,
  318,  319,  320,    0,    0,  355,  356,  357,    0,  812,
   66,   58,   59,   65,   60,   61,    0,   24,    0,    0,
   25,  820,  819,    0,  818,  816,  824,  823,  821,    0,
   45,   46,    0,  718,    0,    0,    0,  748,  746,  744,
    0,  745,    0,    0,    0,    0,    0,  803,    0,    0,
  379,  788,  789,  690,    0,  194,  190,  379,    0,    0,
  270,  379,    0,    0,    0,  229,    0,    0,  245,    0,
    0,    0,    0,    0,    0,    0,    0,  507,  552,    0,
  553,    0,    0,    0,    0,  155,  157,  158,  159,  535,
  533,  531,  562,    0,    0,  379,    0,  379,    0,  375,
  378,   92,    0,    0,   98,   93,    0,    0,    0,    0,
    0,  919,    0,  920,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  772,  761,    0,    0,  773,    0,
  769,  219, 1029, 1049, 1005,    0,    0, 1039, 1038,    0,
 1044, 1032, 1034,    0, 1021, 1022,    0,    0,  281,    0,
    0,    0,    0,    0,  343, 1077,  358,  359,    0,    0,
  293,    0,  298,    0,  346,  299,    0,    0,  339,    0,
   55,   56,   57,   63,    0,    0,  795,    0,  738,  736,
    0,  729,    0,    0,    0,  752,    0,  686,  379,  379,
  376,  790,  191,  161,  274,  275,  263,    0,    0,  207,
  230,    0,    0,    0,  232,  233,  231,    0,    0,  404,
  405,  401,  400,  403,  402,    0,  551,  578,  160,  147,
  156,    0,    0,  260,  142,  382,  380,    0,    0,   96,
    0,   80,    0,  877,  885,  680,    0,  878,  886,  881,
  889,    0,    0,  879,  887,  880,  888,  874,  882,  875,
  883,  876,  884,  768,    0,    0,    0, 1017, 1040,    0,
 1020,    0,    0,  284,    0,    0,    0,    0,  330,    0,
    0,  354,    0,    0,    0,    0,  349,  347,    0,    0,
    0,   40,  815,    0,    0,  720,  730,    0,    0,  731,
  732,  733,    0,  261,  383,  210,  215,    0,    0,  235,
    0,  247,  408,  379,  379,    0,   77,   99,  100,    0,
    0,    0,  775, 1033,  287,  288,    0,    0,    0,    0,
  353,  352, 1073,  351,    0,    0,    0,    0,    0,    0,
    0,  719,    0,    0,  753,  236,    0,    0,    0,    0,
    0,  262,  384,    0,    0,  774,    0,    0,    0,    0,
    0,  348,  350,    0,    0,    0,    0,  724,    0,    0,
    0,    0,    0,  246,    0,  252,    0,  251,  134,  777,
    0,  290,    0,    0,    0,    0,    0,    0,  722,  735,
  234,    0,    0,  256,  255,  254,  778,    0,    0,    0,
    0,  338,  725,  259,  257,  258,  328,    0,  344,  325,
  326,
};
static short yydgoto[] = {                                      21,
   22,   23,   24,   25,   26,   27,   28, 1578,   30,   31,
 1579, 1580,   34,   35,   36,   37,   38,   39, 1581,   41,
 1582, 1622,  147,  568,  172, 1377, 1618,  148,  844,  149,
  571,  849,  150,  151,  315, 1135,  161,  162, 1378, 1379,
 1380, 1381,  248,  280,  152, 1382,  153,  573,   90,  292,
  465,  520,  749, 1253,  521, 1001,  718,  273,  977,  750,
  751,  752, 1260, 1255, 1706,  293,  754,  755,   82,  266,
   83,  288, 1920, 1475,  272,  277,  279,  281,  268,  259,
  261,  274,  284,  181,  370,  372,  376,  374,  367,  455,
  736,  738,  733,  236,  237,  565,  316,  921,  906,  765,
  909,  459,  741, 1244, 1469,  686,  870, 1216,  643,  644,
  667,  450,  719, 1194, 1684, 1810, 1195, 1179, 1685, 1686,
 1687, 1688, 1689,  766,  269,  260,  446,  694,  447,  448,
  695,  696, 1445,  645, 1174, 1175, 1176, 1421,  461,  989,
  990,  991,  992,  923, 1664,  720,  925,  523,  994, 1092,
  559, 1532, 1180,  524,  525,  526,  527,  528, 1665, 1666,
  670, 1817, 1949, 1434,  931,  918,  932,  933,  934, 1950,
 1951, 1995,  427,  564, 1114, 1583,  244,  663, 1111,  664,
 1427,  560, 1115, 1116, 1367, 1561, 1562, 1563, 1874, 1262,
 1584, 1585, 1586, 1587, 1774, 1588, 1589, 1590, 1591, 1592,
 1593, 1594, 1595, 1596, 1597, 1598, 1599, 1600, 1601, 1602,
 1603, 1604, 1101, 1986, 1879,  529,  819,  530, 1605, 1775,
 1886, 1887, 1932, 1606, 1607, 1608, 1769,  531,  830,  532,
  312,  561,  562,  533,   43, 1474,  436,  681,  950, 1443,
  682,  948,   87,   66,  431,  249,  192,  411,  233,  418,
  241,  252,  423,  419,  432,  938,  433,  677, 1207,  234,
  534, 1231,  661,  913,  412,  683, 1444,  105,  247,  297,
  721,  722,  834, 1232,  835, 1233,  725,  726,  727,  728,
  962, 1225, 1223,  729,  730,  731,  970,  732,  967,   44,
   45,   46,   47,   48,   49,  183,  625,   68,  588,  255,
  439,  355,  356,  357,  358,  359,  360,  361,  362,  363,
  876,  609, 1154, 1411,  610,  611,  881,  395,  894,  396,
  630,  397,  897,  188,  400,  633,   50,  191,  406, 1498,
  818,   51,  404,   52,  166,  343,  596,  866,  469, 1408,
 1647,  782,  591,  345, 1418, 1802,  167,  168,  344,  597,
  598,  864, 1133, 1136, 1137, 1138, 1139, 1140, 1141, 1895,
 1942,  535, 1969, 1142, 1143, 1144, 1406, 1900, 1901, 1902,
 1398,  175, 1639, 1795, 1796, 1525, 1039, 1526, 1527, 1528,
 1741, 1921, 1923,  636,  637, 1168, 1654,  579, 1787,   53,
   54,  470,   55,   56,  617,  618,  536, 1123, 1388, 1624,
 1625, 1629,  580,  852,  265,  767,  768, 1016, 1017,  769,
  770,  771,  772,  773,  774,  775,  776,  777,  778,  779,
  780, 1278, 1280, 1281,  537,  538,  539,  540,  541,  542,
  796,  543,  290,  544,  545,  546,  547,  548, 1063,  549,
  550,  551,  552,  553, 1747, 1085,  554,  555,  556,  557,
  801, 1068, 1343, 1338,  802,  155,
};
static short yysindex[] = {                                   7938,
  898,  -95, 3237,   99,  609,   51, 3373,  -77,17077,  667,
  100,   18,  -95, 1176,  539,13495, 1314,13495,  256,  207,
    0,  635,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  530,    0,    0,    0,    0,    0,    0,  643,
    0,    0,    0,    0,    0,    0,    0,18020,13495,  579,
13495,13495,13495,13495,13495,    0,    0,  376, 1110,13495,
13495,13495,  696,13495,  631,13495,13495,    0,13495,13495,
13495,    0,   92,    0,  647,    0,    0,  852,13495,    0,
13495,13495,13495,  882,13495,13495,13495,13495,  631,13495,
13495,13495,13495,13495,    0,13495, 1152,  851,    0,    0,
    0, 1218,    0, 1218,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  903,  936,  157, 1223,
    0,    0,    0,    0,    0,13495,13495,13495,  951,  965,
 1354,13495,  -45,   83,   95,  495,  864,    0,13495, 1105,
 1367,    0,    0,13495, 1056,13495,13495,13495,13495,13495,
    0,    0,    0,13495, 8802,    0, 1048, 1045, 1256,  863,
 1066, 1469,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  840,    0,    0, 1110,    0,    0, 1145,   69,
    0,    0, 1591,    0,    0, 1342,    0,  105,    0,    0,
 1120,    0,    0, 1220,    0,    0, 1195,    0,    0, 1137,
    0, 1390,    0,    0,    0,    0, 1110,    0,    0,  473,
 1591,    0,   57,    0,    0, 1374,    0, 1368,    0, 1218,
    0,    0,    0,    0,    0,    0,    0, 1324,    0,    0,
 1145,    0, 1362,    0, 1280,    0,    0,    0, 1145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,15925,
13495, 1327,    0,13495,    0,    0,    0, 1299, 1361,11644,
13495,  807, 1432, 1735, 1470, 1474, 1479, 1429, 1431,  165,
 1325,    0,    0,16181,    0,    0,    0,    0,    0,    0,
    0,    0,13877,    0,    0,    0, 1400,13495, 1478, 1396,
   -8,13495, 1414,    0,    0, 1367,    0,    0,    0,    0,
    0,    0,    0,    0,    0,13495,    0, 1110, 1591,    0,
 1374,    0, 1535,    0, 1218,    0, 1477, 1441,    0,    0,
    0, 1536,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1521,13495,13495, 1484, 1365,    0,
16181, 1455,    0, 1455,16181,    0,  -83, 1357, 1371, 1380,
 1469,    0,  113,  792, 1579,13495,    0,  840,    0,    0,
    0, 1573,    0,    0,    0,13495, 1327,12970, 1428,13233,
 1847,    0,18020,    0,    0,  975, 1446,    0,    0,    0,
 1836, 1836, 1110, 1532, 1110,   64, 1137,    0,    0,12290,
    0,13495,13495,13495,    0, 1327,    0,    0, 1620,  354,
 1874,13495, 1641,13495,    0, 1875,10833, 9925, 1534,    0,
    0, 1881, 1885, 1887, 1481,    0,    0, 1898,    0, 1904,
    0, 1909, 1913,    0, 1914,    0,    0,    0,    0,    0,
 1916, 1509, 1520,    0, 1935, 1935,    0,    0,    0,    0,
 1937,    0,    0,14133, 1940, 1944,    0, 1948, 1643, 1957,
 1958, 1962, 1972, 1973, 1975,16181,16181,14389,13495, 1976,
    0, 2985,    0,    0,    0,    0,    0,    0,    0, 1926,
 1617, 1982,    0,    0, 1983,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,12290, 1970,
 1051,    0, 1992, 1769, 1418,    0,    0,13495,13495,13495,
  147,    0,    0,    0,    0,    0,13495,13495,  755,    0,
13495,    0,    0, 1299, 1639,11644,    0,13495,14389, 1865,
    0,    0,16181,    0, 2026, 1718, 1993,    0,    0,  107,
    0, 1106,    0,  372,    0,  372,    0,    0, 1996,    0,
  233, 1616,    0,    0,    0, 1995,  106, 1981,    0, 1327,
 2004,13495, 1783,    0,    0,    0,    0,  615,    0, 1711,
 2012,13495,    0,    0, 1634, 2006,    0,    0,    0,  449,
    0, 1716, 1717,    0, 1110,    0, 1719,    0,    0,    0,
 1789,    0,    0,    0,    0,    0,    0,12290,    0, 1110,
 1697,12290, 1483,    0, 1802,13495,17775,    0,    0,  458,
    0,13495,  838, 1342, 1684, 1644,  -50,    0,    0,    0,
    0,   15, 1686,  137,    0, 1632,  631,    0, 1110,    0,
 1810, 1742,    0,    0,   64,    0,    0,  218, 1648,    0,
    0,    0, 1650,    0, 1702, 2055,    0, 1751, 2060,  679,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2011,  152,    0,    0,    0,    0,  149, 2061,
 2063, 2060,    0, 1110, 1591,    0, 1792,    0, 1840,    0,
 1110, 1110,    0,12970,  105,    0, 1733,    0, 1708, 2066,
    0, 5904,    0, 1730,    0,    0, 1738, 2074,10181, 2076,
    0,    0,    0, 9669,11721,  141,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  556, 1739,  905,16181,  457,    0,13495,16181,  996, 1108,
 1198, 3083,    0,    0,  631,    0,    0,16181,10437, 2985,
 1680,  688,16181,16181,10437, 1812,16181,16181,16181,16181,
16181,10693,  891,  891, 1877, 1581,  643, 2088,    0,    0,
14645,13495,16181,16181,16181,16181,16181,16181, 2858,    0,
16181,10311,    0,    0, 1855, 2072,13495,    0,13495, 1844,
13495,    0, 1709, 1721,    0,    0, 1694,11644,    0, 1838,
    0, 1605, 2116,    0, 1838,13495,13495, 2114,    0, 1877,
    0, 1922,    0,13495,10055, 1723,16181,    0,  995,    0,
    0,   42,    0, 1720,    0,    0,    0,13495,  -32,13495,
    0,    0,13495,13495, 1534,    0,16181, 1899,12970,  105,
    0,    0,    0,    0,  615,13495, 1825, 2125, 1677,16181,
16181, 1110,    0,  568,    0, 2134,    0, 2546,    0,    0,
    0, 1110,    0,    0,13495,    0,    0,    0,    0, 1903,
 2141,    0, 1579, 1902,    0,    0, 1842, 1846, 1218,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  631,
13495, 1890, 1579,17873, 1818, 1828, 1829,    0,  631,    0,
    0, 1037,    0, 1289,    0,    0, 1781, 1110,    0,10833,
   97, 1801,    0,    0,    0,  631,    0, 2055,  121,    0,
    0,    0, 1579,  121, 1800,  121,    0, 2172,  631,  631,
  631,    0,    0, 1327,13495,    0, 1862,    0, 1609,    0,
    0,    0,  975, 1455,13495, 1625,  148,    0,    0, 2178,
   85,    0,10833,13495, 1455,10959,    0,    0, 1455,10181,
    0,    0,    0,11187,    0,  389,  708,16181,16181,11226,
 2179,  929,11493,16181,11760, 1064,12027,12294,14901,12561,
12828,13095,10437,10437,    0,    0, 1861,    0, 2190, 1066,
16181,16181, 2079,16181, 2191,16181, 2187, 1929,16181,16181,
16181,16181,16181,16181,    0,    0,    0,    0,    0,    0,
    0,    0, 1918, 2192, 1570,   14,16181,  726,16181,    0,
10437, 2101, 2158,  415,13495, 2010, 2123, 2147, 2268, 2295,
    0,    0,    0, 1543,15157,    0,    0, 1066,    0,    0,
 1626,    0,    0, 1959,  891,  891,  479,  479, 1173,11367,
 2193,    0,    0, 2196, 2985,    0,    0,    0,    0,    0,
 1631,    0, 1806, 1985, 1844,    0,    0,12702,12702, 1893,
    0,  413, 1892,13495,    0,16181, 1892, 1955, 1961,    0,
    0,    0,    0, 7094, 2235, 2232,  -38,    0,    0,    0,
    0,    0,    0,    0,10437, 1956,    0,    0,    0,    0,
    0,    0,    0,  327,    0,    0,    0, 1739, 2985,    0,
 1654,  975, 1455,    0,    0,  615,    0,    0,    0, 2985,
    0, 1836, 2014,    0,  568,    0,10437, 1394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  150,    0, 1844,
    0,    0,    0,    0,    0,    0,    0, 1218, 1218, 1791,
    0,    0,    0,    0,    0,    0,    0,   37,   56, 1042,
    0, 2021,    0,    0,    0,    0,13495,    0,    0,    0,
    0,   99, 1658, 1872, 1855, 2252,    0, 1632, 1659,    0,
 2238, 1182,    0,13495, 1331,  631, 2256, 2257, 2260, 2041,
  105, 1844, 1836, 1717,12970,    0, 1686, 1866, 9799,    0,
  631, 1920, 2008, 5904,    0,    0,  631,  694,    0,    0,
 2013,    0,    0, 2275, 2277,    0,10437,10437,    0,10437,
10437, 2817, 2985, 2281,    0,    0, 2985, 2282,14389,    0,
    0, 2007,    0,   44, 2287, 2985, 2290, 1549, 2291, 2985,
 2299,16181,16181, 2179,16181,15413, 2312, 2985, 2313, 2314,
 2985, 2315,16181, 2985, 2319, 2985, 2320, 2322, 2985, 2330,
 2331, 2985, 2332,    0, 2073,    0,17942,    0, 2385, 2436,
    0, 2456,    0, 2467,16181,    0, 2476, 2623, 2657, 2679,
 2735, 2811,16181,    0,16181,16181, 2985, 1917,16181,    0,
16181, 2985, 2069,   25,16181,16181,16181,    0,    0,    0,
    0,    0,    0,16181,    0,16181, 1613,    0,    0,    0,
 1667,11367,10568,13495,    0,    0,13495,17582,    0,    0,
13495,13495,13495,13495,13495,13495, 1931, 2336, 2337,    0,
    0,    0, 1931,    0,  219,  219,   88,    0,    0, 1678,
    0,12702,12702, 2334,  -38, 2341,16181, 2122,10055, 2009,
    0, 2009, 1020, 2009, 2048, 2052, 2115, 2065,    0,    0,
    0, 1066, 1844,    0, 1686, 1866,    0,  887, 1632, 2015,
 1632,    0,  440,    0,    0,    0,    0, 1985,16181, 2109,
 2002, 1791,   92,    0,    0, 2023, 2024, 2027, 2029, 2034,
 2046,    0,    0,    0,    0,    0, 2353,   99,    0, 1987,
    0,    0,  121,    0,   53,  121,  121,    0,    0, 1855,
 2390,    0,    0,    0,    0,  975, 1985, 1632,    0, 1717,
    0, 2021, 2162,    0,    0, 2125,    0, 2399,    0, 2400,
 1110,   -7, 2409,    0,    0, 1110,    0,    0,    0, 2073,
    0, 2073,16181, 1455, 1455, 1705,  643, 2410,16181, 2140,
    0, 1455, 1455,16181, 1455, 1455, 3157, 2985,    0, 1596,
16181, 2985, 1455, 1455, 1455, 1455, 2985, 1455, 1455, 1455,
 1455, 1455, 1455,13495,    0, 2417, 2415, 1140,    0,    0,
12290, 2421,    0,    0, 2928,    0,    0,    0,    0,    0,
    0, 2935, 1204,    0,16181, 2168, 2036,    0,16181, 3088,
 1707, 2373, 3104, 3111,16181,    0,    0,    0,    0,12290,
 2416,    0,    0, 2198,    0,13495, -106,    0, 2440, 2150,
16181,    0,    0,13495,13495,13495,    0,    0,    0,    0,
 2159,    0,    0,    0,    0, 2189,17447, 2437,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  377, 2446,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2181,    0,12702,12702,
    0,    0,    0, 2078,    0,    0,    0,    0,    0, 1979,
    0,    0, 2122,    0, 1713,13495,  -38,    0,    0,    0,
 2174,    0,10055,10055,16181,10437, 1534,    0, 1985, 2021,
    0,    0,    0,    0, 1289,    0,    0,    0, 2546, 2546,
    0,    0, 2985, 2230, 2109,    0,  482,13495,    0, 2182,
 2082, 2195, 2090, 2228, 2203, 2229, 1844,    0,    0, 2498,
    0, 2499, 2199, 2230,   53,    0,    0,    0,    0,    0,
    0,    0,    0, 1844, 1686,    0, 1289,    0, 2163,    0,
    0,    0, 2184,  -78,    0,    0, 1855, 2194, 2985, 2511,
 2514,    0, 1066,    0, 2985,16181, 2515, 2517, 2985, 2519,
 2520,16181,16181, 2985, 2522, 2523, 2524, 2526, 2529, 2530,
 2532, 2533, 2535, 2537,    0,    0,17942, 2539,    0,13495,
    0,    0,    0,    0,    0,16181, 2541,    0,    0,16181,
    0,    0,    0,16181,    0,    0, 3289,  170,    0, 2269,
16181,16181,10437, 2985,    0,    0,    0,    0,13495,11367,
    0,  577,    0, 2142,    0,    0,   13, 2543,    0, 2212,
    0,    0,    0,    0,  219,16181,    0,13495,    0,    0,
10055,    0, 1563, 2985, 2548,    0,  141,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2552,13495,    0,
    0, 2247,13495, 1791,    0,    0,    0, 1218,13495,    0,
    0,    0,    0,    0,    0, 1985,    0,    0,    0,    0,
    0, 1985, 2021,    0,    0,    0,    0,16181, 1110,    0,
 2137,    0, 1110,    0,    0,    0, 2985,    0,    0,    0,
    0, 2985, 2985,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,13495, 2263, 2985,    0,    0, 3384,
    0, 2546, 2546,    0, 2559, 2985,  971,  487,    0, 2261,
 2193,    0,13495,13495,   99,  194,    0,    0, 2100, 2266,
10437,    0,    0,  112, 2564,    0,    0,10437, 2565,    0,
    0,    0,16181,    0,    0,    0,    0, 1791, 1218,    0,
 2234,    0,    0,    0,    0, 2985,    0,    0,    0, 2562,
 2570, 2573,    0,    0,    0,    0, 1455,11367, 2188,11367,
    0,    0,    0,    0,17582,  577,16181,11367,  686,11367,
13495,    0,  141,13495,    0,    0, 2234,  416, 1791, 2241,
 2242,    0,    0, 2579,13495,    0,13495, 2582, 2193,17582,
 2193,    0,    0, 1294,  168, 2323, 2193,    0, 1732, 1754,
 1791,  833,  833,    0, 2316,    0, 2200,    0,    0,    0,
 2586,    0, 2324,11367, 2342, 2335,17582,13495,    0,    0,
    0, 2183,   50,    0,    0,    0,    0,17582,  266,13495,
17582,    0,    0,    0,    0,    0,    0,17582,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  174, 1015,    0,    0,    0,    0,    0,    0,    0,
    0,17153,  164,13358,    0,    0,    0,    0,    0,    0,
    0, 2637,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  279,    0,    0,    0,    0,    0,    0,  187,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  346,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2306,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  386, 2270,    0,    0,
    0,  111,    0,  111,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2271,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  346,    0,    0,15669,13621,    0,    0,    0,
  849,    0,    0,  136,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  370,    0, 2233,
  704,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  469,    0,    0,    0,    0, 1127,    0,    0,
    0,    0,    0, 1027,    0,    0,    0,    0,    0,  145,
    0,12557,    0,    0,    0,    0,    0,    0,    0,    0,
  469,    0,  496,    0,    0, 2604,    0,    0,    0, 2381,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  126,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  176,
    0, 2384,    0,    0,    0,    0,    0,13751,    0,    0,
    0,    0,    0, 1092,    0,    0,    0,    0,    0,    0,
14007,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  463,    0,    0, 1100,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  469,    0,
 2604,    0,    0,    0, 2381,    0, 1102,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  304,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1155,    0,    0,    0,    0,    0,    0, 1174,    0,    0,
    0,  336,    0,    0,    0,    0, 2384,  566,18101,    0,
 1183,    0,    0,    0,    0,  480,    0,    0,    0,    0,
 2202, 2202,    0,    0,    0, 1213,  159,    0,    0,    0,
    0,    0,    0,    0,    0, 2384,    0,    0, 1264,    0,
    0,    0,    0,    0,    0,12023,    0,    0,  133,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2787,
    0,    0,    0,    0, 2304, 2304,    0,    0,    0,    0,
 3436,    0,    0,    0, 3736, 4036,    0, 4336, 4636,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  928,    0,    0,    0,    0,    0,    0,    0, 4936,
 1215,  262,    0, 3136,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1262,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,13751,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11110,    0, 2339,    0,    0,    0,
    0,    0,    0,  945,    0,  945,    0,    0,  857,    0,
  884, 1035,    0,    0,    0,    0,  126,    0,    0, 2384,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  178,  216,    0,    0,    0,  707,
    0,    0, 1230,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1287,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1047,    0,   24,    0,    0,  842,    0,    0,
    0,    0,  803,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1310,    0,    0, 3127,16702,    0,
    0,    0,16714,    0,    0,16923,    0,    0,16981,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1706, 1762,    0,    0,    0,    0,17092,16751,
    0,16981,    0,    0,  469,    0,    0,    0,    0,    0,
    0,    0,    0,  566, 1247,    0,    0,    0,    0, 2246,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 7756, 7939, 8055,    0,    0,  215,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2613,  124,16437,    0,16437,    0,    0,    0,16437,16437,
16437,    0,    0,    0,    0,    0,    0,    0,    0, 2211,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 6125, 6414, 2616,    0,  140,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2487,    0,    0,    0,    0, 2393,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  241,
    0,    0,    0,    0,  241,    0,    0, 1320,    0,    0,
    0,    0,    0,    0,    0, 1080,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  666,    0,
    0,    0,    0,    0,  133,    0,    0,    0,  566, 1247,
    0,    0,    0,    0,    0,    0,    0,  337,  161,    0,
    0,    0,    0,  180,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2629,
    0,    0, 1901, 1841,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  625,  713,  787,    0,    0,    0,
    0,    0,    0,  752,    0,    0,    0,    0,    0,    0,
    0, 2092,    0,    0,    0,    0,    0,16923,    0,    0,
    0,    0,  705,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2384,    0,    0,  341,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 6193,    0,    0,    0,
  273,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2631,    0,    0, 1322,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2632,    0,    0,
    0,    0,    0, 5236, 6703, 6992, 5536, 5836,    0,    0,
 1333,    0,    0,    0, 1074,    0,    0,    0,    0,    0,
    0,    0,14263,    0, 2405,    0,    0,    0,    0,    0,
    0,    0, 1360,    0,    0,    0, 1360,    0,    0,    0,
    0,    0,    0,    0, 7201,  967,  173,    0,    0,    0,
    0,    0,    0,    0,    0, 1113,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  124,  154,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  888,
    0, 2202, 2231,    0,  462,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1773,    0, 2393,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1389,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1370,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2710,    0,    0,    0,    0,    0,
 1382,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1247, 2393, 2202, 1377,  566,    0, 2418, 1404,    0,    0,
    0, 6193,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1116,    0,    0,    0, 1752,    0,    0,    0,
    0,    0,    0,    0,    0, 6913,    0, 7640,    0, 8152,
    0,    0,    0,    0,    0,    0,    0, 8185,    0,    0,
 8305,    0,    0, 8389,    0, 8449,    0,    0, 8544,    0,
    0, 8616,    0,    0, 2557,    0, 2613,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2259,    0,    0,    0,
    0,  766,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,  196, 1415,  162,  210,    0,
    0,    0, 1415,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2340,  634,    0, 1952,    0, 2346,
    0, 2346, 2340, 2346,    0,    0,    0,  710,    0,    0,
    0, 1322, 2393,    0, 2418, 1404,    0,    0,    0, 2237,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1986,
    0, 1389, 2306,    0,    0,  800,  837,  921, 1024, 1088,
 1093,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  224,    0,    0,    0,    0, 1896,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1454,
    0,    0,    0,    0,    0, 1471,    0,    0,    0,    0,
    0,  166,    0,    0,    0,    0,    0,    0,    0,  153,
    0,  195,    0,    0,    0,    0,  140,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8695,    0, 8845,
    0, 8891,    0,    0,    0,    0, 9014,    0,    0,    0,
    0,    0,    0, 1209,    0,    0, 2647,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 6754, 2633,    0,    0,    0,    0,
    0,    0, 1493, 1522,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  431,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1586,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1490,
    0,    0, 7416,    0,    0,    0, 1497,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  291,    0,    0,    0,
    0,    0,    0,    0,  814,    0,    0,    0,    0,    0,
    0,    0, 2653, 1398, 2258,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2393,    0,    0,    0,
    0,    0,    0, 1494,  242,    0,    0,    0,    0,    0,
    0,    0,    0, 2393, 2418,    0,  359,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  873,    0, 9042,    0,
    0,    0, 2632,    0, 9136,    0,    0,    0, 9256,    0,
    0,    0,    0, 9284,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2613,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2636,    0,    0,
 2646,    0,    0, 2652,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2402,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1219,    0,    0,
    0,    0, 2340,  307, 1129,    0,  991,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2049,    0,    0,    0,  206,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  211,    0,    0,    0,    0,    0, 9379,    0,    0,    0,
    0, 9439, 9473,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1786, 2671,    0,    0,    0,
    0,    0,    0,    0,    0, 2656, 2658,    0,    0,    0,
 2660,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1510, 1353,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2049, 1344,    0,
  186,    0,    0,    0,    0, 2653,    0,    0,    0, 2675,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  447,    0,    0,    0,    0,    0,
    0,    0, 9562,    0,    0,    0, 1409,    0, 1389, 1420,
 1682,    0,    0,    0,    0,    0,    0,    0, 2664,  447,
 2668,    0,    0,    0, 2427,    0, 1511,    0,    0,    0,
 2049,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,17331,    0,    0,    0,  447,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  447,    0,    0,
  447,    0,    0,    0,    0,    0,    0,  447,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, 2729,    0,    0,
 2730, 2734,    0,    0,    0,    0,    0,    0,    1,    0,
 2739,   -2, 2574, 2156,   22, -651, 1358, 2594, 1886, 2595,
 2186,    0,    0, 2443, -108,  -40,    0,    0,    0,    0,
  115,  229,  -64,  -84,  -57, -292, 2449, 1919,    0,    0,
    0,  628,    0,    0,  -59, 1519, -633,  -39,-1107, 2031,
 1785, -342,    0, 1529,    0, 2694, 1788, 1832,    0,    0,
    0, -606,  -49,    0,    0,    0,    0,    0, 2713,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -67,    0, -629, -418, 1551, -616, 1197,
 -514,    0,    0,    0,    0, -399, -150, -790, 1558, -414,
  -62,    0, -627, 1830,    0, 1125, -314, -889,    0, 1126,
    0, 1143, -402, -763,  -27,    0,    0,    0,    0, 2367,
    0, 2120, 1582,    0,    0,    0, 1662,    0, 2454, 1950,
 1595, 2413, 2422,    0,    0, -524, 1927,  -51, -787, 1034,
 -306,    0,    0, -875, -867, -863, -861, -844,    0, 1179,
-1237,    0,  908,-1353,    0, 2185,    0,    0,    0,  910,
  913,  892, -144, -325, -956, -997,    0,    0,    0, 1953,
    0, -729,    0, 1755,    0,    0,    0,    0,    0,-1291,
-1023,    0,    0,    0,    0,    0,    0, -253,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -994,    0,    0, -808,    0, -804, 1293, 1095,
    0,  935,    0,    0,    0,    0,    0,    0,    0, -488,
    0,    0, 2037, -148, -468, 1457, -631, -872,-1084,-1315,
    0,    0,  -55,    0,    0,    0,    0,    0, 2444,    0,
    0,    0,    0, 2458, 2205,    0,    0,    0,    0,    0,
   -9, -500,    0,    0, 2470,    0,    0,    0,    8,    0,
    0,    0, -423, 1907, -419, 1430,    0,    0,    0,    0,
    0,    0,-1011,    0,    0,    0, 2153,    0, 1921,    0,
    0,    0,    0,    0,    0,  -44,    0, 2873,    0, 2728,
    0,    0,    0, 2536,    0,    0,    0,    0, 2293,    0,
 2296,    0,    0,    0, 2025,    0,    0,    0, -485,    0,
 -268,    0,    0,    0,    0,    0, -461, -692, -966, 2209,
    0, 1202,    0,    0,    0,    0,    0,    0, -391,    0,
    0, -826, -225, 1236,    0,    0,    0, 2732, 1078,    0,
 2039,-1070,-1586,    0,-1018, 1767,-1355,    0,    0,    0,
    0, -169,    0,    0,    0,    0, 1501,    0,    0,    0,
    0, 2816,  203,    0, 1005, 2128,    0, 1175,    0, 1386,
    0, -339,    0,    0, 2017,    0,    0,    0,    0,    0,
    0, 2297,    0,    0,    0, -527,-1290, 2067, 1794,-1196,
    0,    0,    0,    0, -751, -556, -440,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1229, 1624,    0,    0,    0, -835,    0,    0,    0,
 2428, 1745,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,-1085, 1585,    0,    0,
};
#define YYTABLESIZE 18633
static short yytable[] = {                                     154,
   40,   86,  898,  267,  365,  317,  173,  262,  182,  258,
  652,  240,  278,  336,  339,  337,  340,  307,  263,  275,
 1102,  243,  283,  174, 1103,  298,  723,  572,  899,  816,
  724,  693, 1181,  271,  833, 1066,  306,  251,  908,  922,
 1182, 1074,  687,  275, 1183,  308, 1184,  302,  235,  238,
  851,  242,  245,  173,  250,  253,  817,  303, 1158,  304,
  264,  238,  253, 1185,  245,  310,  173,  250,  301,  282,
  154,  253, 1186, 1318,  658, 1564,  309, 1610, 1669,  294,
 1108,  173,  296,  238,  246,  294,  253,  242,  245, 1040,
  173,  250,  282,  154,  253,  375,  245,  276,  174,  871,
  656,  665,  973, 1163,  323, 1361,  325,  305,  368, 1112,
  854,  592,  615,  993,  373, 1395, 1369, 1451, 1002,  662,
 1247, 1358,  334,  781, 1088,  758,  456,  817,  631, 1190,
  739, 1156,  760,  910,  338,  742,  369,  914, 1197,  377,
 1222,  347,  924,   85,  168,  335,  264,  173,  264,  884,
  335,  869,  154,  811,  826,  831, 1698,  335,  169,  253,
  791,   52, 1472,  606,  364, 1228,  238,  245,  173,  250,
  282,  463,  712,  606,  182,  365,  420,  787,  324,  186,
  781,  952,  781,  953,  758,  253,  781, 1252,  979, 1626,
  848,  760, 1667,  848,  742, 1094,  616,  811, 1242,  371,
  321,  791, 1007,  168,  791,  826,   95,  451,  321,   53,
 1426, 1362,  811,  712,  757,  780,  712,  169,  787,  791,
   52,  787,  606,  154,  620,  885,  253,  781,  903,  253,
 1873,  712,  606, 1428,  365,  850,  787, 1936,  186, 1094,
  817,  153,  976,  723,  253,  781,  826,  724,  907,  826,
 1544,   97,  942, 1546, 1094,  757,  780,  961, 1162,  558,
  641,  363,  566,   85,  826, 1704,  623,  930,   53, 1314,
 1315,  604, 1840,  757,  780, 1033,  880,  846,  674, 1400,
  693,  335,  154, 1767, 1768, 1467, 1033, 1792,  106, 1415,
  760, 1102, 1401,  572,  888, 1103, 1610, 1188,  601,  817,
  153,  264,  817,  676,  264, 1193,  754, 1344,  619, 1362,
  574,  154,  945,   87,  614, 1402,   87,  106, 1008,  674,
  363,  604, 1889, 1015,  457,  424,  485, 1636,  650, 1403,
 1650,  760, 1091, 1212, 1800,  496,  675,  674,  242, 1099,
  141,  159,  173, 1500,  676,  608, 2005,  754,  946,  760,
  754,   67, 1692, 1438, 1968,  254,  286, 1258,  195,  943,
  691,  434,  676, 1152,  657,  754, 1404,   91,  605,  678,
 1436,  158, 1413,  608,  558, 1628,  263,  675,  673, 1153,
 1637,  675,  331,  688,  734,  690,  629,  629,  650,  737,
  287, 1501, 1034, 1762,  496,  675,  651, 2006,  425,  141,
  743, 2003, 1033, 1034,  608, 1841,  264,  641,  616, 1164,
  678,  944,  735, 1423,  747,  458,  264,  195,  264, 1205,
  264,   14,  606,  678, 1683,  642,  975, 1667,  678, 1269,
 1662, 1985, 1259,  485,  435, 1897,  107,  186, 1681,  607,
 1336,  168,  238,  245,  250, 1648, 1425, 1940,  947,  186,
 1705, 1549,  173, 1266,  242,  169, 1649,  756, 1347, 1748,
 1910,  187,  629, 1751,  753, 1412, 1872, 1383, 1229, 1696,
 1437,   95,  606,  160, 1146,  791,  186, 1880, 1405,  509,
 1658,  586,  186,  745,  186,  485,  826,  712,  168, 1439,
  826,  824,  787,  825, 1935,  827,  791,  712, 1045,  712,
  154,  781,  169, 1489, 1490,  758, 1491, 1492,  712,  820,
 1200,  747,  760,  787,  758,  742,   97, 1915,  153, 1034,
  187,  629,  781,  186,  742, 1248,  614, 1929,  845,  757,
  780,  848,  333,  811, 1251,  954, 1264, 1221,  509,  757,
 1265,  757,  811,  859,  333,  742, 1221,  879,   84,  468,
  757,  780,  712, 1557, 1946,  572,  333, 1558,  173,  245,
  574,  712, 1788, 1773,  606, 1008, 2008,  173,  893, 1008,
  333,  173,  692,  850,  606, 1429,  574,  365,  182,  742,
  566,  826,  185,  868,  320,  904,  333,  978, 1893,  843,
  984,  606,  847, 1695,  757, 1974, 1389,  811,  850, 1842,
  911,  817,  855,  757,  712,  760,  791,   52,  662, 1466,
 1833, 1108,  173,  951,  674,  926,  712, 1991,  712, 1476,
 1002,  754,  264,  787,  393, 1793,  760, 1165, 1204,  956,
 1157,  275,  781,  754, 1559, 1130,  781, 1390,  758,  676,
  960, 1094,  754,  890, 1610,  760,  757,  955,  742,  371,
 1267, 1799,  781,  168, 1254,   53,  919,  264, 1240, 1104,
  757,  780,  919,  363, 1869, 1394,  811,  169,   84, 1610,
  760,  787,  675,  791,  983,   87, 1033, 1694,  253, 1430,
 1417,  987,  988,  393,  141,  712,  754, 1109,  186, 1435,
  787, 1777,  263,  186, 1416,  754, 1610,  742,  826,  781,
  496, 1033,  195,  794,  152,  678,  792, 1610, 2009,  756,
 1610,  296,  391, 1972,  616, 1455,  341, 1610, 1149,  187,
 1826,  340,  760, 1409,  674,  822, 1966,  757,  780, 1385,
  823,  187,  265,  650,  264,  294,  760, 1832,  754, 1064,
 1631, 1632,  874,  509,  794,  340, 1846,  792, 1033,  676,
  756,  195,  754,  184,  341,  822, 1044,  650,  187, 1047,
  823,  340,  794,  152,  187,  792,  187, 1268,  756, 1805,
 1806,  391, 1419,  927, 1410, 1881,  254,  253,  138, 1431,
 1432,  452,  675, 1181, 1181,  558,  392,  558,  371, 1117,
 1496, 1182, 1182, 1034,  342, 1183, 1183, 1184, 1184,  398,
  629, 1149,  407,  760, 1713,  187,  256,  257, 1659, 1660,
  195,  875, 1093,  195, 1185, 1185,  845, 1497, 1034,  754,
 1798, 1778,  264, 1186, 1186, 1677, 1670,  264, 1913,  264,
 1477,  264,  614,  629, 1914,  893,  399,  263,  574, 1882,
 1386,  239, 1171, 1468,  187,  392,  173,  245,  613, 1973,
  928,  340, 1187, 1683, 1132,  245,  635, 1551,  398,  629,
  169,  407,  629,  558, 1803, 1034,  422,  453,  173,  509,
  173,  265,  195,  264, 1812,  340,  901, 1128,  629,  264,
 1813,  239, 1797,  645,  275, 1883,  629,  793,  393, 1202,
  929,  340, 1206, 1211, 1035,  399, 1884,  239, 1218,  608,
 1201, 1155,  454, 1742,  629,  264, 1835,  613, 1635, 1254,
 1226, 1962, 1661,  559, 1814,  635,  559, 1036,  466, 1172,
  395,   88,   89,  275,  275,  275,  471,  955,  793, 1678,
 1104,  264,  826, 1959,  264, 1961, 1983,  827,  156, 1237,
 1238, 1239,  645, 1965,  634, 1967,  793, 1033,  138,  138,
  756,  714, 1682,  971,  616, 1690,  270, 1219,  239, 1700,
  170, 1102,  892, 2002,  714, 1103,  711,  171,  955, 1270,
  187,  955,  714, 1037, 2007,  173,  391, 2010, 1531,  395,
  152,  152, 1925, 1926, 2011,  173,  955,  714, 1173, 1999,
  755, 1069, 1070,  756, 1263, 1885, 1181, 1181,  239, 1878,
  753,  714,  157,  634, 1182, 1182, 1241,  711, 1183, 1183,
 1184, 1184,  826,  824,  393,  825, 1249,  827,  794,  972,
  955,  792,  189,  394,  756,  711,  610, 1185, 1185, 1339,
 1340,  755, 1497, 1497,  630, 1758, 1186, 1186,  714,  794,
 1497, 1497,  792, 1497, 1497,  756,  488, 1348,  195,  755,
  392, 1497, 1497, 1497, 1497,  903, 1497, 1497, 1497, 1497,
 1497, 1497,  638,  398, 1034,  253,  407,  256,  257, 1047,
 1047,  195,  614,  956,  190,  653,  152, 1149, 1611,  758,
   84, 1614,  394,  195, 1838,  610, 1271,  397,  654,  756,
  488,  838,  396,  630,  837,  195,  289,  413,  756,  612,
  399,  599,  391,  195,  109,  488,  893,  723,  574,  574,
  195,  724,  751,  638,  956,  902,  195,  956,  195, 1102,
  758, 1102,  402, 1103,  245, 1103,  387, 1939,  750, 1102,
 1071, 1102,  956, 1103, 1943, 1103, 1801,  195,  758,  195,
  111,  756,  414, 1804, 1067,  559,  397, 1807, 1808,  794,
  714,  396,  792,  751,  414,  756,  902,  195,  612,  902,
  599,  195,  403,   14,  341,  195,  956,  822, 1341,  750,
  291,  751,  823,  415,  902, 1102,  392, 1484,  559, 1103,
  275, 1834,  411, 1836,  395,  387,  195,  750,   57,  398,
  853,  311, 1104,  263,  635,  275, 1461,  566,  616,   58,
  299,  275,  793, 1992, 1041,  112,   59, 1446, 1047, 1485,
   60, 1479,  177,  414,  368,  113,  831, 1483,  721, 1447,
  559,  645,  756,  793, 1446, 1457,  399,  635, 1282,  502,
   61, 1067,  415,  679,  195,  264, 1623, 1623, 1627,  264,
  114,  411, 1993,  680,  313,  826,  824,  822,  825,  415,
  827, 1067,  823,  635,  645,  341,  635,  314,  721,  721,
  195,   43,  721,  955, 1816, 1360,  322,  416, 1655,  440,
 1657,  177,  635,  368, 1458,  559, 1283,  721,  318,  131,
  645,  711,  634,  645, 1904, 1905,  498,  394,  502, 1994,
 1615,  711,   62,  711,  936, 1049,  937, 1894,  635,  645,
 1284,  417,  711, 1067, 1560,  755, 1928,  173, 1613,  178,
  395,  319,  558,  131, 1970,  263,  614, 1697, 1616,   47,
   43,  794,  195,   63, 1524,  645,  755,   64,  326,  955,
  559, 1612,  366,  793, 1292,  826,  824, 1400,  825, 1440,
  827,  634,  327,  826,  634,  498,  711,  131, 1293,  165,
 1401,  397,  723,  264,  264,  711,  396, 1565, 1609,  822,
  634,  574,  245,  250,  574,  282,  154,  341,  178,  511,
  755, 1652,  630, 1402, 1457, 1441,  143,  902,   47,  755,
  794, 1653,  574,  574,  826,  387,  634,  826,  248,  245,
  387,  366, 1294,  723,  758,  387,  723,  216,  711, 1952,
 1953, 1954,  826,  381,  758,  630,  758, 1051,  253,  956,
  711,  723,  711,  394,   41,  758,   65,  616,  822,  249,
 1295, 1703,  755, 1460, 1404,  534, 1708,  751,  511,  248,
  902,  630,  248,  902,  630,  143,  755,  348,  216,  751,
  902,  216,  902,  750,  333, 1680,  902,  248,  751,  253,
  630,  902,  253,  144,  902,  750,  216,  341,  842,  758,
  249,  841,  381,  249,  750,  366, 1790,  253,  758,  902,
  132, 1148, 1738,   41,  534,  956,  630,  397,  249,  711,
  822,  398,  396,  902, 1296,  823,  333,  502,  441,  807,
  399,  902,  751,  216,  902,  902,  713, 1053, 1761,  502,
  831,  751,  872,  755,  902,  387,  522, 1739,  750,  805,
  806,  758,  144, 1213,  173,  387,  387,  750, 1746, 1740,
  902,  353,  442,  916,  443,  758,  915,  831,  401,  132,
  590, 1524,  502,  721,  502,  614,  721,  713, 1096,  595,
  713,  771,  902,  721,  751,  721,  444,  902,  807,  721,
 1096,  341,  216, 1096,  721,  713,  238,  721,  751,  902,
  750,  902,  256,  257, 1766, 1263, 1263, 1101,  805,  806,
  822,  445,  721,  593,  750,  823,  771, 1609,  405, 1101,
  342,  599, 1101, 1355,  826,  824,  721,  825,  771,  827,
  826,  824,  758,  825,  721,  827,  421,  635,  721,  428,
 1104,  640,  429,  638, 1640,  639, 1642,  721,  902,  574,
  574,  826,  824,  108,  825, 1980,  827, 1981, 1896,  826,
  826, 1087,  176,  721,  831,  751,  364,  826,  902, 1984,
  426, 1081,  902,  245,  245,  438, 1623,  826,  824,  437,
  825,  750,  827, 1077,  430, 1125, 1067,  440, 1124, 1246,
  721,  109, 1245,  449,  826,  824,  464,  825,  173,  827,
  164,  165,  721,  110,  721, 1250, 1359,  723,  998,  831,
  723, 1365, 1214, 1215, 1364,  826,  824,  723,  825,  723,
  827,  250,  462,  723,  253,  253,  460,  111,  723, 1818,
 1866,  723,  253,  467, 1414,  249,  249, 1245, 1449, 1454,
  800, 1448, 1453,  249,  349,  536,  723, 1556,  177, 1911,
 1362,  721,  813,  814,  815,  826,  139,  139, 1630,  826,
  723,  831,  250,  468,  567,  250,  407,  173,  723,  563,
  242,  721,  723, 1781, 1783,  721,  826,  350,  408,  178,
  250,  723,  826,  179, 1524, 1712,  536, 1753,  831,  536,
  831,  869,  112, 1789,  180,  569,  831,  723, 1104, 1263,
 1104,  559,  113,  351,  536,  616,  352,  577, 1104, 1912,
 1104,  409, 1989,  410,  578, 1988,  826, 1890,  364, 1917,
  253,  245,  353, 1919,  723,  860,  581,  114,  582,  862,
  616,  249,  869,  583, 1990,  869,  723,  841,  723, 1093,
 1947,  253,  559,  173,  584,  559,  585,  253,  354,  242,
  869,  713,  249,  276, 1104, 1931,  276,  616,  249,  822,
  559,  713,  587,  713,  823,  822,  779,  600,  616,  779,
  823,  616,  713,  602, 1909,  747,  826,  603,  616,  612,
  526,  253,  487,  488,  489,  723,  822, 1782, 1784,  622,
  626,  823,  249,  624,  627,  242, 1504,  628,  632, 1354,
 1042,  634, 1046,   14,  646,  723, 1050, 1052, 1054,  723,
  497,  498,  822,  238, 1933,  655,  713,  823,  660,  647,
 1400,  526, 1934,  614,  526,  713, 1335,  648,  671,  822,
  674,  684,  566, 1401,  823,  559,  685,  689,  286,  526,
  152,  253,  740, 1723,  746,  861,  826,  824,  614,  825,
  822,  827,  249,  744,  748,  823, 1402, 1086,  826,  824,
  783,  825,  781,  827,  784, 1609,  785, 1958,  713, 1555,
 1403, 1132,  287,  786,  264,  614,  559,  787, 1898,  559,
  713,  152,  713,  788,  152,  242,  614,  242,  789,  614,
 1609,  737,  790,  791,  559,  792,  614,  250,  250,  152,
 1014,  793, 1131,  826,  824,  250,  825, 1404,  827, 1326,
  826,  824,  794,  825,  795,  827,  798, 1609, 1132,  803,
 1043,  536,  536,  804, 1048,  227,  536,  805, 1609,  536,
 1263, 1609,  737,  806, 1065,  737,  807,  808, 1609, 1072,
 1073,  809,  536, 1076, 1077, 1078, 1079, 1080, 1084,  713,
  737,  810,  811,  869,  812,  821,  828,  522,  829, 1094,
 1095, 1096, 1097, 1098,  522,  831,  227, 1105,  832,  227,
  836,  839,  840,  857,  865,  882,  867,  559,  559,  878,
  883,  887,  559,  889,  227,  559,  891,  895,  248,  900,
 1349,  826,  824,  250,  825, 1899,  827,  896,  559,  902,
  641,  912,  905,  595,  650,  917,  869,  826,  824,  869,
  825,  941,  827,  940,  250,  949,  869,  536,  869, 1405,
  250,  333,  869, 1159,  957,  958,  963,  869,  964,  248,
  869,  554,  248,  965,  966,  968,  635, 1170,  536,  969,
  980,  974,  981,  986,  536,  869,  985,  248,  995,  998,
  997, 1003, 1004, 1005,  250, 1009,  526,  526,  189,  869,
  826,  824, 1067,  825,  526,  827, 1075,  869, 1089,  975,
  869,  869,  554,  559,  335,  554, 1118,  526,  536, 1113,
  869,  822,  826,  824, 1345,  825,  823,  827, 1119, 1120,
  554, 1122,  554,  822,  559, 1126,  869,  848,  823, 1150,
  559, 1166, 1160, 1350,  826,  824, 1145,  825,  841,  827,
 1167,  559,  559, 1177,  250, 1191,  152,  152,  869,  559,
 1192, 1198,  920,  869,  152, 1199, 1203, 1351,  826,  824,
 1217,  825,  559,  827,  559,  869, 1208,  869,  822,  826,
  824, 1346,  825,  823,  827,  822, 1209, 1210, 1224, 1234,
  823, 1236,  526, 1243, 1272, 1273, 1277, 1257, 1279, 1286,
 1288, 1290, 1316, 1298, 1301, 1304, 1306, 1309, 1312, 1317,
 1325, 1323, 1334,  526, 1333,  822, 1362, 1319, 1320,  526,
 1322, 1363, 1324, 1366,  869, 1327, 1328, 1329, 1330, 1331,
 1332, 1287, 1387, 1291, 1368, 1299, 1302,  228, 1307, 1310,
 1313,  239,  227, 1337,  869, 1342,  737,  559,  869,  737,
 1384, 1392,  152,  526, 1397, 1399,  737, 1393,  737, 1420,
 1407, 1357,  737, 1433, 1442, 1450,  822,  737,  559,  863,
  737,  823, 1452,  152,  559, 1456, 1462, 1463,  228,  152,
 1464,  228,  822,  953, 1465,  737, 1251,  823, 1352,  826,
  824, 1473,  825, 1481,  827, 1487,  228, 1488, 1486,  737,
 1494, 1495,  522, 1499,  248,  248, 1502,  737,  559, 1503,
 1505,  737,  248,  152, 1033, 1353,  826,  824, 1506,  825,
  737,  827, 1321, 1545,  953,  953,  953,  953,  953,  554,
  953, 1513, 1514, 1515, 1516,  822,  737,  239, 1518, 1519,
  823, 1520,  953,  953,  953,  953,  554,  554,  554, 1521,
 1522, 1523,  554, 1548, 1633,  554, 1617,  822,  239, 1619,
 1620, 1634,  823,  737,  239,  863, 1643, 1645,  554, 1638,
 1644, 1646,  666, 1679,  554,  737,  953,  737, 1656,  822,
 1668, 1671, 1672,  710,  823, 1673,  710, 1674,  710,  710,
  710,  710, 1675,  710,  826,  824, 1754,  825,  239,  827,
  248,  710,  710,  822, 1676, 1529,  826,  824,  823,  825,
 1693,  827,  710,  710,  822,  710, 1221, 1699, 1701,  823,
 1702,  248,  710,  710,  737,  710,  710,  248,  710, 1707,
 1714,  710,  710,  710,  710,  710, 1716, 1736, 1737,  710,
  710, 1743, 1750,  554,  737,  710,  710,  710,  737,  710,
  710,  710, 1749,  710, 1759,  522, 1530,  826,  824, 1763,
  825,  248,  827,  710,  554,  157,  559, 1760, 1507, 1508,
  554, 1510, 1512, 1771, 1770, 1776, 1533,  826,  824, 1517,
  825,  554,  827, 1779, 1780, 1785, 1809, 1534,  826,  824,
 1786,  825, 1791,  827, 1819, 1820, 1536,  826,  824,  554,
  825, 1535,  827, 1822,  554, 1823, 1825,  559, 1821, 1542,
  559, 1543, 1342,  239,  228, 1342, 1824, 1337, 1827, 1828,
 1837, 1550,  522, 1552,  822,  559, 1829,  559, 1839,  823,
 1553, 1844, 1554,  554, 1845, 1848,  844, 1849, 1843, 1850,
 1851,  953, 1854, 1855, 1856,  953, 1857,  953,  953, 1858,
 1859,  822, 1860, 1861,  953, 1862,  823, 1863, 1865,  953,
  953, 1868, 1891, 1875, 1772,  953, 1908,  953,  953, 1892,
 1178, 1903, 1906,  522, 1918, 1922, 1930,  844, 1927, 1937,
  844, 1938,  953, 1941, 1944, 1955,  953,  953,  953, 1948,
 1956,  953, 1957, 1972, 1960,  844, 1975, 1977,  953, 1979,
  953,  953, 1982, 1987,  953, 1663, 1997, 1998,  953,  239,
  953, 2000,  953, 1973,  953, 2001,    1, 2004,  787,  953,
  953, 1061,  953,  202,  826,   29,   31,  267,   82,  822,
  239,  165,  766, 1046,  823,  703,  239,  953,  953,  955,
  953,  822,  278, 1537,  826,  824,  823,  825,  212,  827,
  765,  953,  794,  953,  277,  953,  953,  953,  747,  953,
  192,  407,  953,  953,  749, 1045,  193,  767, 1019, 1709,
  239,  323,  953,  379,  289, 1715,  953, 1538,  826,  824,
 1719,  825,  953,  827,  321,  953,  295, 1724,  953,  559,
  331, 1018,  822,  953,  322,  776,  324,  823,  332, 1539,
  826,  824,  327,  825,  953,  827,  360,  345,   29,   32,
  953,  953,  822,   33,  328,  953,  953,  823,   42,  856,
 1621, 1342, 1129,  822,  559, 1342,  953,  953,  823,  953,
  559, 1757,  822,  559,  329,  953,  330,  823,  953,  559,
  953,  953,  953,  953,  576,  953, 1121, 1764,  559,  575,
  559,  858, 1482,  953,  953, 1540,  826,  824,  996,  825,
 1480,  827, 1256,  559,  953,  953, 1088,  953,  300,  559,
 1261, 1220,  559,  285,  953,  953,  953,  953,  953, 1478,
  953, 1470, 1230,  953,  953,  953,  953,  953, 1830, 1815,
 1831,  953,  953,  697,  959, 1459,  953,  953,  953,  953,
  953,  953,  953,  953,  621,  953, 1063, 1088, 1088, 1088,
 1088, 1088, 1088, 1088,  559,  953, 1422,  475, 1161, 1471,
  668, 1794, 1907, 1811, 1088, 1088, 1088, 1088, 1088,  669,
 1196, 1541,  826,  824, 1971,  825,  935,  827,  826,  824,
 1978,  825, 1976,  827, 1996,  477, 1765, 1189, 1888, 1370,
 1963,  844, 1651, 1110,  844,  659,  676, 1088,  939, 1088,
  649,  844, 1235,  844,  982,  163, 1691,  844, 1227,  559,
  332,  613,  844,  479,  873,  844,  559, 1100,  346,  822,
 1396,  877, 1151, 1641,  823, 1147,  295, 1945, 1038, 1735,
  844, 1864, 1847,  886,  559,  519, 1169, 1509, 1852, 1853,
 1391, 1127, 1424,  797,  844, 1547,    0,    0,    0,    0,
    0,    0,  844,  822,    0,  844,  844,    0,  823,    0,
    0,    0, 1867,    0,  559,  844, 1342,    0,  559,    0,
 1870,    0,    0,    0,    0,  822,    0, 1876, 1877,    0,
  823,  844,    0,    0,  484,    0,    0,  559, 1744,  826,
  824,    0,  825,    0,  827, 1745,  826,  824,    0,  825,
  486,  827,  522,  844,    0,  559,  559,    0,  844,    0,
  559,    0,    0,  559,  487,  488,  489,    0,  256,  257,
  844,    0,  844,    0,    0,    0,  559,    0,    0,    0,
    0,  822,  559,    0,    0,    0,  823,  492,  493,  494,
  495,  496,  497,  498,  499,  500,  826,  824,    0,  825,
    0,  827,  502,  503, 1916,    0,    0,    0,    0,    0,
    0,  507,    0,    0,    0,    0,    0,    0, 1088,  844,
 1088, 1088,    0,    0,    0,    0,    0, 1088,    0,    0,
    0,    0,    0, 1088,    0,    0,    0,    0, 1088,  844,
    0, 1088,    0,  844,    0,    0,    0,    0, 1493,    0,
    0,  559,    0,    0,    0, 1088,    0,  822,    0,    0,
 1088, 1088,  823,  822, 1088,    0,    0,    0,  823, 1794,
    0, 1088,  559, 1088, 1088,    0,    0, 1088,  559,    0,
    0, 1088,    0, 1088,    0, 1088,    0, 1088,    0,  559,
    0,    0, 1088, 1088,    0, 1088,  557,    0, 1752,  826,
  824,    0,  825, 1964,  827, 1067,    0,  559,    0,    0,
 1088, 1088,  559, 1088, 1755,  826,  824,    0,  825,    0,
  827, 1756,  826,  824, 1088,  825, 1088,  827, 1088, 1088,
 1088,    0, 1088,    0,    0, 1088, 1088,  557,    0,    0,
  557,  559,    0,    0,    0, 1088, 1067, 1067, 1067, 1067,
 1067, 1081, 1067,    0,    0,  557,    0,  557, 1088,    0,
    0, 1088,    0,    0, 1067, 1067, 1067, 1067,  826,  824,
    0,  825,    0,  827,  822,    0,    0, 1088,    0,  823,
    0,  822,    0, 1088, 1088,    0,  823,    0, 1088,    0,
    0,    0,    0,    0,    0,    0, 1067,    0, 1067, 1088,
 1088,    0, 1088,    0,    0,    0,    0,    0, 1088,    0,
    0, 1088,    0, 1088, 1088, 1088, 1088,    0, 1088,    0,
    0,    0,    0,    0,    0,    0, 1088, 1088,    0,    0,
    0,  822,    0,    0,    0,    0,  823, 1088, 1088,    0,
 1088,    0,    0,    0,    0,    0,    0, 1088, 1088, 1088,
 1088, 1088,    0, 1088,    0,    0, 1088, 1088, 1088, 1088,
 1088,    0,    0,    0, 1088, 1088,    0,    0,    0, 1088,
 1088, 1088, 1088, 1088, 1088, 1088, 1088,    0, 1088,  115,
    0,    0,  116,    0,  117,  118,  119,  120, 1088,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123, 1871,
  826,  824,    0,  825,    0,  827,    0,    0,  124,  125,
    0,  126,    0,    0,    0,    0,    0,    0,  127,  128,
    0,  129,  130,    0,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,  822,  137,  138,    0,    0,  823,
    0,  139,  140,  141,    0,  142,  143,  144,    0,  145,
  822,    0,    0,    0,  557,  823,    0,  822,    0,  146,
    0,    0,  823,    0,    0,    0,    0, 1067,    0, 1067,
 1067,  557,  557,  557,    0,    0, 1067,  557,    0,    0,
  557,    0, 1067,    0,    0,    0,    0, 1067, 1722,    0,
 1067,    0,    0,  557, 1924,  826,  824,    0,  825,  557,
  827,    0,    0,  822, 1067, 1098, 1067,    0,  823, 1067,
 1067,    0,    0, 1067,    0,    0,    0,    0,    0,    0,
 1067,    0, 1067, 1067,    0,    0, 1067,    0,    0,    0,
 1067,    0, 1067,    0, 1067,    0, 1067,    0,    0,    0,
    0, 1067, 1067,    0, 1067,    0, 1098, 1098, 1098, 1098,
 1098, 1098, 1098,    0,    0,    0,    0,    0,    0, 1067,
 1067,    0, 1067,    0, 1098, 1098, 1098, 1098,  557,    0,
    0,    0,    0, 1067,    0, 1067,    0, 1067, 1067, 1067,
    0, 1067,    0,    0, 1067, 1067,    0,    0,    0,  557,
    0,    0,    0,    0, 1067,  557, 1098,   69, 1098,    0,
    0,    0,    0,    0,  557,    0,  557, 1067,   70,    0,
 1067,    0,    0,    0,    0,   71, 1055, 1056, 1057, 1058,
 1059, 1060, 1061, 1062,  557,    0, 1067,    0,   72,  557,
    0,    0, 1067, 1067,    0,  822,    0, 1067,    0,    0,
  823,    0,    0,    0,    0,    0,    0,    0, 1067, 1067,
    0, 1067,    0,    0,    0,    0,    0, 1067,  557,    0,
 1067,    0, 1067, 1067, 1067, 1067,    0, 1067,    0,    0,
    0,    0,    0,    0,    0, 1067, 1067,    0,    0,    0,
    0,    0,    0,    0,    0,   73, 1067, 1067,    0, 1067,
    0,    0,    0,    0,    0,    0, 1067, 1067, 1067, 1067,
 1067,   74, 1067,    0,    0, 1067, 1067, 1067, 1067, 1067,
    0,    0,    0, 1067, 1067,    0,    0,   75, 1067, 1067,
 1067, 1067, 1067, 1067, 1067, 1067,    0, 1067,    0,    0,
  822,    0,   76,    0,    0,  823,   77, 1067,    0,   78,
    0,    0,    0,    0,   92,    0,    0,   79,    0,    0,
    0,   93,    0,    0,    0,   94,   95,    0,    0,    0,
    0,    0,    0,    0,   96,    0,   80, 1098,    0, 1098,
 1098,    0, 1710, 1711,    0,   97, 1098,    0,    0,    0,
 1717, 1718, 1098, 1720, 1721,    0,    0, 1098,    0,    0,
 1098, 1725, 1726, 1727, 1728,    0, 1729, 1730, 1731, 1732,
 1733, 1734,    0,    0, 1098, 1103,    0,    0,    0, 1098,
 1098,    0,    0, 1098,    0,    0,    0,    0,    0,    0,
 1098,    0, 1098, 1098,    0,   81, 1098,    0,    0,    0,
 1098,    0, 1098,    0, 1098,    0, 1098,   98,    0,    0,
    0, 1098, 1098,    0, 1098,    0, 1103, 1103, 1103, 1103,
 1103, 1103, 1103,   99,    0,    0,    0,    0,    0, 1098,
 1098,    0, 1098,    0, 1103, 1103, 1103, 1103,  100,    0,
    0,    0,  101, 1098,    0, 1098,    0, 1098, 1098, 1098,
    0, 1098,    0,  102, 1098, 1098,    0,    0,    0,    0,
    0,    0,    0,    0, 1098,    0, 1103,    0, 1103,    0,
    0,    0,  103,    0,    0,    0,    0, 1098,    0,    0,
 1098,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1098,    0,    0,    0,
    0,    0, 1098, 1098,    0,    0,    0, 1098,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1098, 1098,
    0, 1098,    0,    0,    0,    0,    0, 1098,    0,    0,
 1098,  104, 1098, 1098, 1098, 1098,    0, 1098,    0,    0,
    0,    0,    0,    0,    0, 1098, 1098,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1098, 1098,    0, 1098,
    0,    0,    0,    0,    0,    0, 1098, 1098, 1098, 1098,
 1098,    0, 1098,    0,    0, 1098, 1098, 1098, 1098, 1098,
    0,    0,    0, 1098, 1098,    0,    0,    0, 1098, 1098,
 1098, 1098, 1098, 1098, 1098, 1098,    0, 1098,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1098,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1103,    0, 1103,
 1103,    0,    0,    0,    0,    0, 1103,    0,    0,    0,
    0,    0, 1103,    0,    0,    0,    0, 1103,    0,    0,
 1103,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1103, 1099,    0,    0,    0, 1103,
 1103,    0,    0, 1103,    0,    0,    0,    0,    0,    0,
 1103,    0, 1103, 1103,    0,    0, 1103,    0,    0,    0,
 1103,    0, 1103,    0, 1103,    0, 1103,    0,    0,    0,
    0, 1103, 1103,    0, 1103,    0, 1099, 1099, 1099, 1099,
 1099, 1099, 1099,    0,    0,    0,    0,    0,    0, 1103,
 1103,    0, 1103,    0, 1099, 1099, 1099, 1099,    0,    0,
    0,    0,    0, 1103,    0, 1103,    0, 1103, 1103, 1103,
    0, 1103,    0,    0, 1103, 1103,    0,    0,    0,    0,
    0,    0,    0,    0, 1103,    0, 1099,    0, 1099,    0,
    0,    0,    0,    0,    0,    0,    0, 1103,    0,    0,
 1103,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1103,    0,    0,    0,
    0,    0, 1103, 1103,    0,    0,    0, 1103,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1103, 1103,
    0, 1103,    0,    0,    0,    0,    0, 1103,    0,    0,
 1103,    0, 1103, 1103, 1103, 1103,    0, 1103,    0,    0,
    0,    0,    0,    0,    0, 1103, 1103,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1103, 1103,    0, 1103,
    0,    0,    0,    0,    0,    0, 1103, 1103, 1103, 1103,
 1103,    0, 1103,    0,    0, 1103, 1103, 1103, 1103, 1103,
    0,    0,    0, 1103, 1103,    0,    0,    0, 1103, 1103,
 1103, 1103, 1103, 1103, 1103, 1103,    0, 1103,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1103,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1099,    0, 1099,
 1099,    0,    0,    0,    0,    0, 1099,    0,    0,    0,
    0,    0, 1099,    0,    0,    0,    0, 1099,    0,    0,
 1099,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1099, 1112,    0,    0,    0, 1099,
 1099,    0,    0, 1099,    0,    0,    0,    0,    0,    0,
 1099,    0, 1099, 1099,    0,    0, 1099,    0,    0,    0,
 1099,    0, 1099,    0, 1099,    0, 1099,    0,    0,    0,
    0, 1099, 1099,    0, 1099,    0, 1112, 1112, 1112, 1112,
 1112, 1112, 1112,    0,    0,    0,    0,    0,    0, 1099,
 1099,    0, 1099,    0, 1112, 1112, 1112, 1112,    0,    0,
    0,    0,    0, 1099,    0, 1099,    0, 1099, 1099, 1099,
    0, 1099,    0,    0, 1099, 1099,    0,    0,    0,    0,
    0,    0,    0,    0, 1099,    0, 1112,    0, 1112,    0,
    0,    0,    0,    0,    0,    0,    0, 1099,    0,    0,
 1099,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1099,    0,    0,    0,
    0,    0, 1099, 1099,    0,    0,    0, 1099,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1099, 1099,
    0, 1099,    0,    0,    0,    0,    0, 1099,    0,    0,
 1099,    0, 1099, 1099, 1099, 1099,    0, 1099,    0,    0,
    0,    0,    0,    0,    0, 1099, 1099,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1099, 1099,    0, 1099,
    0,    0,    0,    0,    0,    0, 1099, 1099, 1099, 1099,
 1099,    0, 1099,    0,    0, 1099, 1099, 1099, 1099, 1099,
    0,    0,    0, 1099, 1099,    0,    0,    0, 1099, 1099,
 1099, 1099, 1099, 1099, 1099, 1099,    0, 1099,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1099,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1112,    0, 1112,
 1112,    0,    0,    0,    0,    0, 1112,    0,    0,    0,
    0,    0, 1112,    0,    0,    0,    0, 1112,    0,    0,
 1112,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1112, 1117,    0,    0,    0, 1112,
 1112,    0,    0, 1112,    0,    0,    0,    0,    0,    0,
 1112,    0, 1112, 1112,    0,    0, 1112,    0,    0,    0,
 1112,    0, 1112,    0, 1112,    0, 1112,    0,    0,    0,
    0, 1112, 1112,    0, 1112,    0, 1117, 1117, 1117, 1117,
 1117, 1117, 1117,    0,    0,    0,    0,    0,    0, 1112,
 1112,    0, 1112,    0, 1117, 1117, 1117, 1117,    0,    0,
    0,    0,    0, 1112,    0, 1112,    0, 1112, 1112, 1112,
    0, 1112,    0,    0, 1112, 1112,    0,    0,    0,    0,
    0,    0,    0,    0, 1112,    0, 1117,    0, 1117,    0,
    0,    0,    0,    0,    0,    0,    0, 1112,    0,    0,
 1112,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1112,    0,    0,    0,
    0,    0, 1112, 1112,    0,    0,    0, 1112,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1112, 1112,
    0, 1112,    0,    0,    0,    0,    0, 1112,    0,    0,
 1112,    0, 1112, 1112, 1112, 1112,    0, 1112,    0,    0,
    0,    0,    0,    0,    0, 1112, 1112,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1112, 1112,    0, 1112,
    0,    0,    0,    0,    0,    0, 1112, 1112, 1112, 1112,
 1112,    0, 1112,    0,    0, 1112, 1112, 1112, 1112, 1112,
    0,    0,    0, 1112, 1112,    0,    0,    0, 1112, 1112,
 1112, 1112, 1112, 1112, 1112, 1112,    0, 1112,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1112,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1117,    0, 1117,
 1117,    0,    0,    0,    0,    0, 1117,    0,    0,    0,
    0,    0, 1117,    0,    0,    0,    0, 1117,    0,    0,
 1117,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1117,  921,    0,    0,    0, 1117,
 1117,    0,    0, 1117,    0,    0,    0,    0,    0,    0,
 1117,    0, 1117, 1117,    0,    0, 1117,    0,    0,    0,
 1117,    0, 1117,    0, 1117,    0, 1117,    0,    0,    0,
    0, 1117, 1117,    0, 1117,    0,  921,  921,  921,  921,
  921,    0,  921,    0,    0,    0,    0,    0,    0, 1117,
 1117,    0, 1117,    0,  921,  921,  921,  921,    0,    0,
    0,    0,    0, 1117,    0, 1117,    0, 1117, 1117, 1117,
    0, 1117,    0,    0, 1117, 1117,    0,    0,    0,    0,
    0,    0,    0,    0, 1117,    0,    0,    0,  921,    0,
    0,    0,    0,    0,    0,    0,    0, 1117,    0,    0,
 1117,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1117,    0,    0,    0,
    0,    0, 1117, 1117,    0,    0,    0, 1117,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1117, 1117,
    0, 1117,    0,    0,    0,    0,    0, 1117,    0,    0,
 1117,    0, 1117, 1117, 1117, 1117,    0, 1117,    0,    0,
    0,    0,    0,    0,    0, 1117, 1117,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1117, 1117,    0, 1117,
    0,    0,    0,    0,    0,    0, 1117, 1117, 1117, 1117,
 1117,    0, 1117,    0,    0, 1117, 1117, 1117, 1117, 1117,
    0,    0,    0, 1117, 1117,    0,    0,    0, 1117, 1117,
 1117, 1117, 1117, 1117, 1117, 1117,    0, 1117,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1117,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  921,    0,  921,
  921,    0,    0,    0,    0,    0,  921,    0,    0,    0,
    0,    0,  921,    0,    0,    0,    0,  921,    0,    0,
  921,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  921,  934,    0,    0,    0,  921,
  921,    0,    0,  921,    0,    0,    0,    0,    0,    0,
  921,    0,  921,  921,    0,    0,  921,    0,    0,    0,
  921,    0,  921,    0,  921,    0,  921,    0,    0,    0,
    0,  921,  921,    0,  921,    0,  934,  934,  934,  934,
  934,    0,  934,    0,    0,    0,    0,    0,    0,  921,
  921,    0,  921,    0,  934,  934,  934,  934,    0,    0,
    0,    0,    0,  921,    0,  921,    0,  921,  921,  921,
    0,  921,    0,    0,  921,  921,    0,    0,    0,    0,
    0,    0,    0,    0,  921,    0,    0,    0,  934,    0,
    0,    0,    0,    0,    0,    0,    0,  921,    0,    0,
  921,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  921,    0,    0,    0,
    0,    0,  921,  921,    0,    0,    0,  921,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  921,  921,
    0,  921,    0,    0,    0,    0,    0,  921,    0,    0,
  921,    0,  921,  921,  921,  921,    0,  921,    0,    0,
    0,    0,    0,    0,    0,  921,  921,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  921,  921,    0,  921,
    0,    0,    0,    0,    0,    0,  921,  921,  921,  921,
  921,    0,  921,    0,    0,  921,  921,  921,  921,  921,
    0,    0,    0,  921,  921,    0,    0,    0,  921,  921,
  921,  921,  921,  921,  921,  921,    0,  921,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  921,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  934,    0,  934,
  934,    0,    0,    0,    0,    0,  934,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  934,    0,    0,
  934,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  934,  937,    0,    0,    0,  934,
  934,    0,    0,  934,    0,    0,    0,    0,    0,    0,
  934,    0,  934,  934,    0,    0,  934,    0,    0,    0,
  934,    0,  934,    0,  934,    0,  934,    0,    0,    0,
    0,  934,  934,    0,  934,    0,  937,  937,  937,  937,
  937,    0,  937,    0,    0,    0,    0,    0,    0,  934,
  934,    0,  934,    0,  937,  937,  937,  937,    0,    0,
    0,    0,    0,  934,    0,  934,    0,  934,  934,  934,
    0,  934,    0,    0,  934,  934,    0,    0,    0,    0,
    0,    0,    0,    0,  934,    0,    0,    0,  937,    0,
    0,    0,    0,    0,    0,    0,    0,  934,    0,    0,
  934,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  934,    0,    0,    0,
    0,    0,  934,  934,    0,    0,    0,  934,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  934,  934,
    0,  934,    0,    0,    0,    0,    0,  934,    0,    0,
  934,    0,  934,  934,  934,  934,    0,  934,    0,    0,
    0,    0,    0,    0,    0,  934,  934,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  934,  934,    0,  934,
    0,    0,    0,    0,    0,    0,  934,  934,  934,  934,
  934,    0,  934,    0,    0,  934,  934,  934,  934,  934,
    0,    0,    0,  934,  934,    0,    0,    0,  934,  934,
  934,  934,  934,  934,  934,  934,    0,  934,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  934,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  937,    0,  937,
  937,    0,    0,    0,    0,    0,  937,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  937,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  937,  938,    0,    0,    0,  937,
  937,    0,    0,  937,    0,    0,    0,    0,    0,    0,
  937,    0,  937,  937,    0,    0,  937,    0,    0,    0,
  937,    0,  937,    0,  937,    0,  937,    0,    0,    0,
    0,  937,  937,    0,  937,    0,  938,  938,  938,  938,
  938,    0,  938,    0,    0,    0,    0,    0,    0,  937,
  937,    0,  937,    0,  938,  938,  938,  938,    0,    0,
    0,    0,    0,  937,    0,  937,    0,  937,  937,  937,
    0,  937,    0,    0,  937,  937,    0,    0,    0,    0,
    0,    0,    0,    0,  937,    0,    0,    0,  938,    0,
    0,    0,    0,    0,    0,    0,    0,  937,    0,    0,
  937,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  937,    0,    0,    0,
    0,    0,  937,  937,    0,    0,    0,  937,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  937,  937,
    0,  937,    0,    0,    0,    0,    0,  937,    0,    0,
  937,    0,  937,  937,  937,  937,    0,  937,    0,    0,
    0,    0,    0,    0,    0,  937,  937,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  937,  937,    0,  937,
    0,    0,    0,    0,    0,    0,  937,  937,  937,  937,
  937,    0,  937,    0,    0,  937,  937,  937,  937,  937,
    0,    0,    0,  937,  937,    0,    0,    0,  937,  937,
  937,  937,  937,  937,  937,  937,    0,  937,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  937,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  938,    0,  938,
  938,    0,    0,    0,    0,    0,  938,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  938,    0,    0,    0,  932,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  938,    0,    0,    0,    0,  938,
  938,    0,    0,  938,    0,    0,    0,    0,    0,    0,
  938,    0,  938,  938,    0,    0,  938,    0,    0,    0,
  938,    0,  938,    0,  938,  932,  938,  932,  932,  932,
    0,  938,  938,    0,  938,  999,    0,    0,  699,    0,
    0,    0,    0,  932,  932,  932,  932,    0,    0,  938,
  938, 1000,  938,    0,    0,  700,    0,    0,  701,    0,
    0,    0,    0,  938,    0,  938,    0,  938,  938,  938,
    0,  938,    0,    0,  938,  938,    0,  932,    0,    0,
    0,    0,    0,    0,  938,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  938,  702,    0,
  938,    0,    0,    0,  703,  704,  705,    0,  706,  707,
  708,    0,  709,    0,    0,    0,  938,    0,    0,    0,
    0,    0,  938,  938,    0,    0,    0,  938,    0,    0,
  710,    0,  711,    0,    0,    0,    0,    0,  938,  938,
    0,  938,    0,    0,    0,    0,    0,  938,    0,    0,
  938,    0,  938,  938,  938,  938,    0,  938,    0,    0,
    0,  712,    0,    0,    0,  938,  938,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  938,  938,  713,  938,
    0,    0,    0,    0,    0,    0,  938,  938,  938,  938,
  938,    0,  938,    0,    0,  938,  938,  938,  938,  938,
  714,    0,    0,  938,  938,    0,    0,    0,  938,  938,
  938,  938,  938,  938,  938,  938,    0,  938,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  938,    0,    0,
    0,    0,    0,    0,    0,  715,  716,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  932,    0,  932,  932,
    0,    0,  717,    0,    0,  932,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  932,
    0,    0,    0,  931,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  932,    0,    0,    0,    0,  932,  932,
    0,    0,  932,    0,    0,    0,    0,    0,    0,  932,
    0,  932,  932,    0,    0,  932,    0,    0,    0,  932,
    0,  932,    0,  932,  931,  932,  931,  931,  931,    0,
  932,  932,    0,  932,  220,    0,    0,  220,    0,    0,
    0,    0,  931,  931,  931,  931,    0,    0,  932,  932,
  220,  932,    0,    0,  220,    0,    0,  220,    0,    0,
    0,    0,  932,    0,  932,    0,  932,  932,  932,    0,
  932,    0,    0,  932,  932,    0,  931,    0,    0,    0,
    0,    0,    0,  932,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  932,  220,    0,  932,
    0,    0,    0,  220,  220,  220,    0,  220,  220,  220,
    0,  220,    0,    0,    0,  932,    0,    0,    0,    0,
    0,  932,  932,    0,    0,    0,  932,    0,    0,  220,
    0,  220,    0,    0,    0,    0,    0,  932,  932,    0,
  932,    0,    0,    0,    0,    0,  932,    0,    0,  932,
    0,  932,  932,  932,  932,    0,  932,    0,    0,    0,
  220,    0,    0,    0,  932,  932,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  932,  932,  220,  932,    0,
    0,    0,    0,    0,    0,  932,  932,  932,  932,  932,
    0,  932,    0,    0,  932,  932,  932,  932,  932,  220,
    0,    0,  932,  932,    0,    0,    0,  932,  932,  932,
  932,  932,  932,  932,  932,    0,  932,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  932,    0,    0,    0,
    0,    0,    0,    0,  220,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  931,    0,  931,  931,    0,
    0,  220,    0,    0,  931,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  931,    0,
    0,    0,  933,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  931,    0,    0,    0,    0,  931,  931,    0,
    0,  931,    0,    0,    0,    0,    0,    0,  931,    0,
  931,  931,    0,    0,  931,    0,    0,    0,  931,    0,
  931,    0,  931,  933,  931,  933,  933,  933,    0,  931,
  931,    0,  931,    0,    0,    0,    0,    0,    0,    0,
    0,  933,  933,  933,  933,    0,    0,  931,  931,    0,
  931,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  931,    0,  931,    0,  931,  931,  931,    0,  931,
    0,    0,  931,  931,    0,  933,    0,    0,    0,    0,
    0,    0,  931,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  931,    0,    0,  931,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  931,    0,    0,    0,    0,    0,
  931,  931,    0,    0,    0,  931,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  931,  931,    0,  931,
    0,    0,    0,    0,    0,  931,    0,    0,  931,    0,
  931,  931,  931,  931,    0,  931,    0,    0,    0,    0,
    0,    0,    0,  931,  931,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  931,  931,    0,  931,    0,    0,
    0,    0,    0,    0,  931,  931,  931,  931,  931,    0,
  931,    0,  870,  931,  931,  931,  931,  931,    0,    0,
    0,  931,  931,    0,    0,    0,  931,  931,  931,  931,
  931,  931,  931,  931,    0,  931,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  931,    0,    0,    0,    0,
    0,    0,    0,  870,    0,    0,  870,    0,    0,    0,
    0,    0,    0,    0,  933,    0,  933,  933,    0,    0,
    0,  870,    0,  933,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  933,    0,    0,
    0,  936,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  933,    0,    0,    0,    0,  933,  933,    0,    0,
  933,    0,    0,    0,    0,    0,    0,  933,    0,  933,
  933,    0,    0,  933,    0, 1067,    0,  933, 1067,  933,
    0,  933,  936,  933,  936,  936,  936,    0,  933,  933,
    0,  933,    0, 1069,    0, 1067,    0,    0, 1067,    0,
  936,  936,  936,  936,    0,    0,  933,  933,    0,  933,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  933,    0,  933,    0,  933,  933,  933,    0,  933,    0,
    0,  933,  933,    0,  936,    0,    0,    0, 1067,    0,
    0,  933,    0,    0, 1067, 1067, 1067,    0, 1067, 1067,
 1067,    0, 1067,    0,  933,    0,    0,  933,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1067,    0, 1067,  933,    0,    0,    0,    0,    0,  933,
  933,    0,    0, 1134,  933,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  933,  933,    0,  933,    0,
    0, 1067,    0,    0,  933,    0,    0,  933,    0,  933,
  933,  933,  933,    0,  933,    0,    0,    0, 1067,    0,
    0,    0,  933,  933,  870,    0,    0,    0,    0,    0,
    0,    0,    0,  933,  933,    0,  933,    0,    0,    0,
 1067,    0,    0,  933,  933,  933,  933,  933,    0,  933,
  739,    0,  933,  933,  933,  933,  933,    0,    0,    0,
  933,  933,    0,    0,    0,  933,  933,  933,  933,  933,
  933,  933,  933,    0,  933, 1067, 1067,  870,    0,    0,
  870,    0,    0,    0,  933,    0,    0,  870,    0,  870,
    0,  739, 1067,  870,  739,    0,    0,    0,  870,    0,
    0,  870,    0,  936,    0,  936,  936,    0,    0,  739,
    0,    0,  936,    0,    0,    0,  870,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  936,    0,    0,    0,
  870,    0,    0,    0,    0,    0,    0,    0,  870,    0,
  936,  870,  870,    0,    0,  936,  936,    0,    0,  936,
    0,  870,    0,    0,    0,    0,  936,    0,  936,  936,
    0,    0,  936,    0,    0,    0,  936,  870,  936,    0,
  936,    0,  936,    0,    0,    0,    0,  936,  936,    0,
  936,    0,    0,    0,    0,    0,    0,    0,    0,  870,
    0,    0,    0,    0,  870,  936,  936,    0,  936,    0,
    0,    0,    0,    0,    0,    0,  870,    0,  870,  936,
    0,  936,    0,  936,  936,  936,    0,  936,    0,    0,
  936,  936,    0,    0,    0,    0,    0,    0,    0,    0,
  936,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,    0,    0,  936,    0,    0,    0,
    0,    0,    0,    0,    0,  870,    0,    0,    0,    0,
    0,    0,  936,    0,    0,  710,    0,    0,  936,  936,
    0,    0,    0,  936,    0,  870,    0,    0,    0,  870,
    0,    0,    0,    0,  936,  936,    0,  936,    0,    0,
    0,    0,    0,  936,    0,    0,  936,    0,  936,  936,
  936,  936,    0,  936,    0,  710,  710,    0,    0,  710,
    0,  936,  936,    0,  739,    0,    0,    0,    0,    0,
    0,    0,  936,  936,  710,  936,    0,    0,    0,    0,
    0,    0,  936,  936,  936,  936,  936,    0,  936,    0,
    0,  936,  936,  936,  936,  936,    0,    0,    0,  936,
  936,    0,   14,    0,  936,  936,  936,  936,  936,  936,
  936,  936,    0,  936,    0,  739,    0,    0,  739,    0,
    0,    0,    0,  936,    0,  739,    0,  739,    0,    0,
    0,  739,    0,    0,    0,    0,  739,    0,    0,  739,
    0,    0,    0,    0,    0,  115,    0,    0,  116,    0,
  117,  118,  119,  120,  739,  121,    0,    0,    0,    0,
    0,    0,    0,  122,  123,    0,    0,    0,  739,    0,
    0,    0,    0,    0,  124,  125,  739,  126,    0,    0,
  739,    0,    0,    0,  127,  128,    0,  129,  130,  739,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,  739,    0,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  146,    0,    0,    0,    0,
    0,    0,  739,    0,    0,    0,    0,    0,    0,  896,
    0,    0,    0,    0,  739,    0,  739,    0,    0,    0,
    0,    0,  739,    0,    0,  739,    0,  739,  739,  739,
  739,    0,  739,    0,    0,    0,    0,    0,    0,    0,
  739,  739,    0,    0,    0,    0,    0,    0,    0,    0,
  896,  739,  739,  896,  739,    0,    0,    0,    0,    0,
    0,  739,  739,  739,  739,  739,    0,  739,  896,    0,
  739,  739,  739,  739,  739,    0,    0,    0,  739,  739,
    0,    0,    0,  739,  739,  739,  739,  739,  739,  739,
  739,    0,  739,    0,    0,    0,    0,    0,    0,    0,
  710,    0,  739,  710,    0,    0,    0,    0,    0,    0,
  710,    0,  710,    0,    0,    0,  710,    0,    0,    0,
    0,  710,    0,    0,  710,  912,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  710,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  710,    0,    0,    0,    0,    0,    0,
    0,  710,    0,    0,    0,  710,  912, 1106, 1106,  912,
 1106, 1106, 1106,    0,  710,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  912, 1106, 1106, 1106,    0,    0,
  710,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1106,  710,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  710,
    0,  710,    0,    0,    0,    0,    0,  710,    0,    0,
  710,    0,  710,  710,  710,  710,    0,  710,    0,    0,
    0,    0,    0,    0,    0,  710,  710,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  710,  710,    0,  710,
    0,  896,    0,    0,    0,    0,  710,  710,  710,  710,
  710,    0,  710,    0,    0,  710,  710,  710,  710,  710,
    0,    0,    0,  710,  710,    0,    0,    0,  710,  710,
  710,  710,  710,  710,  710,  710,    0,  710,  913,    0,
    0,    0,    0,    0,    0,    0,    0,  710,    0,    0,
    0,    0,    0,    0,  896,    0,    0,  896,    0,    0,
    0,    0,    0,    0,  896,    0,  896,    0,    0,    0,
  896,    0,    0,    0,    0,  896,    0,    0,  896,  913,
 1107, 1107,  913, 1107, 1107, 1107,    0,    0,    0,    0,
    0,    0,    0,  896,    0,    0,    0,  913, 1107, 1107,
 1107,    0,    0,    0,    0,    0,    0,  896,    0,    0,
    0,    0,    0,    0,    0,  896,    0,  912,  896,  896,
    0,    0,    0,    0,    0,    0, 1106,    0,  896, 1107,
    0,    0, 1106,    0,    0,    0,    0, 1106,    0,    0,
 1106,    0,    0,    0,  896,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  914,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  896,    0,    0,    0,
  912,  896,    0,  912,    0,    0, 1106,    0,    0,    0,
  912,    0,  912,  896, 1106,  896,  912,    0,    0,    0,
    0,  912, 1106,    0,  912,  914, 1108, 1108,  914, 1108,
 1108, 1108,    0,    0,    0,    0,    0,    0,    0,  912,
 1106,    0, 1106,  914, 1108, 1108, 1108,    0,    0,    0,
    0,    0,    0,  912,    0, 1106,    0, 1106, 1106, 1106,
    0,  912,  896,    0,  912,  912,    0,    0,    0,    0,
    0,    0,    0,    0,  912, 1108,    0,    0,    0,    0,
    0,  873,  896,    0,    0,    0,  896,    0,    0,    0,
  912,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1106,    0,    0,    0,
    0,    0,  912,    0,  871,    0,    0,  912,    0,    0,
    0,    0,  873,    0,    0,  873,    0,    0,    1,  912,
  913,  912,    0,    0,    0,    0,    0,    0,    0, 1107,
  873,    0,    0,    0,    0, 1107,    2,    0,    0,    0,
 1107,    0,    0, 1107,    3,  871,    0,    0,  871,    0,
    0,    4,    0,    5,    0,    6,    0,    0,    0,    0,
    7,    0,    0,  871,    0,    0,    0,    8,  912,    0,
    0,    0,    0,  913,    0,    0,  913,    0,    0, 1107,
    0,    9,    0,  913,    0,  913,    0, 1107,  912,  913,
    0,   10,  912,    0,  913, 1107,    0,  913,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  913, 1107,    0, 1107,    0,    0,    0,    0,
    0,    0,    0,    0,  872,    0,  913,    0, 1107,    0,
 1107, 1107, 1107,    0,  913,    0,  914,  913,  913,    0,
    0,    0,    0,    0,    0, 1108,    0,  913,    0,    0,
    0, 1108,    0,    0,   11,    0, 1108,    0,    0, 1108,
    0,   12,    0,  913,   13,  872,   14,   15,  872,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1107,
    0,    0,    0,  872,    0,  913,    0,    0,    0,  914,
  913,   16,  914,    0,    0, 1108,    0,    0,    0,  914,
    0,  914,  913, 1108,  913,  914,    0,    0,  904,    0,
  914, 1108,    0,  914,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  914, 1108,
    0, 1108,    0,  873,    0,    0,    0,    0,    0,    0,
   17,    0,  914,    0, 1108,    0, 1108, 1108, 1108,  904,
  914,  913,  904,  914,  914,   18,   19,    0,    0,    0,
    0,    0,    0,  914,    0,    0,  871,  904,  866,    0,
    0,  913,    0,    0,    0,  913,    0,    0,    0,  914,
   20,    0,    0,    0,    0,    0,  873,    0,    0,  873,
    0,    0,    0,    0,    0, 1108,  873,    0,  873,    0,
    0,  914,  873,    0,    0,    0,  914,  873,    0,  866,
  873,    0,  866,    0,    0,    0,    0,    0,  914,  871,
  914,    0,  871,    0,    0,  873,    0,  866,    0,  871,
    0,  871,    0,    0,    0,  871,    0,    0,    0,  873,
  871,    0,    0,  871,    0,    0,    0,  873,    0,    0,
  873,  873,    0,    0,    0,    0,    0,    0,  871,    0,
  873,    0,    0,  867,    0,    0,    0,  914,    0,    0,
    0,    0,  871,    0,    0,    0,  873,    0,    0,    0,
  871,    0,    0,  871,  871,    0,  872,  914,    0,    0,
    0,  914,    0,  871,    0,    0,    0,    0,  873,    0,
    0,    0,    0,  873,  867,    0,    0,  867,    0,  871,
    0,    0,    0,    0,    0,  873,    0,  873,    0,    0,
    0,    0,  867,    0,    0,    0,    0,    0,    0,    0,
    0,  871,    0,    0,    0,  868,  871,    0,    0,  872,
    0,    0,  872,    0,    0,    0,    0,    0,  871,  872,
  871,  872,    0,    0,    0,  872,    0,    0,    0,    0,
  872,    0,    0,  872,  873,    0,    0,    0,    0,    0,
  904,    0,    0,    0,    0,    0,  868,    0,  872,  868,
    0,    0,    0,    0,  873,    0,    0,    0,  873,    0,
    0,    0,  872,    0,  868,    0,    0,  871,    0,    0,
  872,    0,    0,  872,  872,    0,    0,    0,    0,    0,
    0,    0,    0,  872,  903,    0,    0,  871,    0,    0,
    0,  871,    0,  904,    0,    0,  904,    0,    0,  872,
  866,    0,    0,  904,    0,  904,    0,    0,    0,  904,
    0,    0,    0,    0,  904,    0,    0,  904,    0,    0,
    0,  872,    0,    0,    0,  903,  872,    0,  903,    0,
    0,    0,  904,    0,    0,    0,    0,    0,  872,    0,
  872,    0,    0,  903,    0,    0,  904,    0,    0,    0,
    0,    0,    0,  866,  904,    0,  866,  904,  904,    0,
    0,    0,    0,  866,    0,  866,    0,  904,    0,  866,
    0,    0,    0,    0,  866,    0,    0,  866,    0,    0,
    0,    0,    0,  904,    0,    0,    0,  872,    0,    0,
    0,    0,  866,    0,    0,  867,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  904,  866,  872,    0,    0,
  904,  872,    0,    0,  866,    0,    0,  866,  866,    0,
    0,    0,  904,    0,  904,    0,    0,  866,    0,    0,
    0,    0,    0,    0,  897,    0,    0,    0,    0,    0,
    0,    0,    0,  866,    0,    0,    0,    0,  867,    0,
    0,  867,    0,    0,    0,    0,    0,    0,  867,    0,
  867,    0,    0,    0,  867,  866,    0,  868,    0,  867,
  866,  904,  867,    0,    0,  897,    0,    0,  897,    0,
  905,    0,  866,    0,  866,    0,    0,  867,    0,    0,
    0,  904,    0,  897,    0,  904,    0,    0,    0,    0,
    0,  867,    0,    0,    0,    0,    0,    0,    0,  867,
    0,    0,  867,  867,    0,    0,    0,    0,    0,    0,
  868,  905,  867,  868,  905,    0,    0,    0,    0,    0,
  868,  866,  868,    0,    0,    0,  868,    0,  867,  905,
    0,  868,    0,    0,  868,    0,  903,    0,    0,    0,
    0,  866,    0,    0,    0,  866,    0,    0,    0,  868,
  867,    0,    0,    0,    0,  867,    0,    0,    0,    0,
    0,    0,    0,  868,    0,    0,    0,  867,    0,  867,
    0,  868,    0,    0,  868,  868,    0,    0,    0,    0,
    0,    0,    0,    0,  868,    0,    0,    0,    0,  903,
    0,    0,  903,  906,    0,    0,    0,    0,    0,  903,
  868,  903,    0,    0,    0,  903,    0,    0,    0,    0,
  903,    0,    0,  903,    0,    0,  867,    0,    0,    0,
    0,  894,  868,    0,    0,    0,    0,  868,  903,    0,
    0,    0,    0,    0,  906,    0,  867,  906,    0,  868,
  867,  868,  903,    0,    0,    0,    0,    0,    0,    0,
  903,    0,  906,  903,  903,    0,  378,    0,    0,    0,
    0,    0,  894,  903,    0,  894,    0,    0,    0,    0,
    0,    0,  379,    0,    0,    0,    0,    0,    0,  903,
  894,    0,    0,  380,    0,    0,  897,    0,  868,    0,
  381,    0,    0,    0,  382,  383,    0,    0,    0,    0,
    0,  903,    0,  384,    0,    0,  903,    0,  868,    0,
    0,    0,  868,    0,  385,  892,    0,    0,  903,    0,
  903,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  905,    0,    0,    0,    0,    0,    0,  897,
    0,    0,  897,    0,    0,    0,    0,    0,    0,  897,
    0,  897,    0,    0,    0,  897,  892,    0,    0,  892,
  897,    0,    0,  897,    0,    0,    0,  903,  386,    0,
    0,    0,    0,    0,  892,    0,  387,    0,  897,    0,
    0,    0,    0,    0,    0,  905,    0,  903,  905,    0,
    0,  903,  897,    0,    0,  905,    0,  905,    0,    0,
  897,  905,    0,  897,  897,    0,  905,  388,    0,  905,
    0,  389,    0,  897,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,  905,    0,    0,    0,    0,  897,
    0,    0,    0,    0,    0,  898,    0,    0,  905,    0,
    0,  391,  392,    0,    0,    0,  905,    0,    0,  905,
  905,  897,    0,    0,    0,  906,  897,    0,    0,  905,
    0,    0,    0,  907,    0,    0,    0,    0,  897,    0,
  897,    0,    0,    0,    0,  905,  898,    0,    0,  898,
    0,    0,    0,  894,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  898,    0,    0,  905,    0,    0,
  393,    0,  905,  394,  907,    0,    0,  907,  906,    0,
    0,  906,    0,    0,  905,    0,  905,  897,  906,    0,
  906,    0,  907,    0,  906,    0,    0,    0,    0,  906,
    0,    0,  906,    0,    0,    0,  894,  897,    0,  894,
    0,  897,    0,    0,    0,    0,  894,  906,  894,    0,
    0,    0,  894,    0,    0,    0,    0,  894,  893,    0,
  894,  906,    0,  905,    0,    0,    0,    0,    0,  906,
    0,    0,  906,  906,    0,  894,    0,  892,    0,    0,
    0,    0,  906,  905,    0,    0,    0,  905,    0,  894,
    0,    0,    0,    0,    0,    0,    0,  894,  906,  893,
  894,  894,  893,    0,    0,    0,    0,    0,    0,    0,
  894,    0,    0,    0,    0,    0,    0,  893,  895,    0,
  906,    0,    0,    0,    0,  906,  894,    0,    0,    0,
  892,    0,    0,  892,    0,    0,    0,  906,    0,  906,
  892,    0,  892,    0,    0,    0,  892,    0,  894,    0,
    0,  892,  899,  894,  892,    0,    0,    0,    0,  895,
    0,    0,  895,    0,    0,  894,    0,  894,    0,  892,
    0,    0,    0,    0,    0,    0,    0,  895,    0,    0,
    0,    0,    0,  892,    0,    0,  906,    0,    0,    0,
    0,  892,    0,  899,  892,  892,  899,  898,    0,    0,
    0,    0,    0,    0,  892,    0,  906,    0,    0,    0,
  906,  899,    0,    0,  894,    0,    0,    0,    0,    0,
  892,    0,    0,    0,    0,  907,    0,    0,    0,    0,
    0,    0,    0,    0,  894,    0,    0,    0,  894,    0,
    0,  734,  892,    0,    0,    0,    0,  892,    0,    0,
  898,    0,    0,  898,    0,    0,    0,    0,    0,  892,
  898,  892,  898,    0,    0,    0,  898,    0,    0,    0,
    0,  898,    0,    0,  898,    0,    0,    0,  907,    0,
    0,  907,  734,    0,    0,  734,    0,    0,  907,  898,
  907,    0,    0,    0,  907,    0,    0,    0,    0,  907,
  734,    0,  907,  898,    0,    0,    0,    0,  892,    0,
    0,  898,    0,    0,  898,  898,    0,  907,    0,    0,
  893,    0,    0,    0,  898,    0,    0,    0,  892,    0,
    0,  907,  892,    0,    0,    0,    0,    0,    0,  907,
  898,    0,  907,  907,    0,    0,    0,    0,    0,    0,
    0,    0,  907,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  898,    0,    0,    0,    0,  898,  907,    0,
    0,    0,    0,  893,    0,    0,  893,    0,    0,  898,
  895,  898,    0,  893,    0,  893,    0,    0,  764,  893,
  907,  516,    0,  517,  893,  907,    0,  893,    0,    0,
    0,    0,    0,    0,    0,    0,  519,  907,    0,  907,
    0,  335,  893,    0,  899,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  893,    0,  898,    0,
    0,    0,    0,  895,  893,    0,  895,  893,  893,    0,
    0,    0,    0,  895,    0,  895,    0,  893,  898,  895,
    0,    0,  898,    0,  895,    0,  907,  895,    0,    0,
    0,    0,    0,  893,    0,    0,    0,  899,    0,    0,
  899,    0,  895,    0,    0,    0,  907,  899,    0,  899,
  907,    0,    0,  899,    0,  893,  895,    0,  899,    0,
  893,  899,    0,    0,  895,    0,    0,  895,  895,    0,
    0,    0,  893,    0,  893,    0,  899,  895,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  314,    0,
  899,    0,    0,  895,    0,    0,    0,    0,  899,    0,
    0,  899,  899,    0,    0,    0,    0,    0,    0,    0,
    0,  899,    0,    0,    0,  895,    0,    0,    0,    0,
  895,  893,    0,    0,    0,    0,  734,  899,    0,  734,
    0,    0,  895,    0,  895,    0,  734,    0,  734,    0,
    0,  893,  734,    0,    0,  893,    0,  734,    0,  899,
  734,    0,    0,    0,  899,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  734,  899,    0,  899,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  734,
    0,  895,    0,    0,    0,  472,    0,  734,    0,    0,
    0,  734,  473,    0,    0,    0,    0,    0,    0,    0,
  734,  895,    0,    0,  474,  895,    0,    0,    0,    0,
  475,  476,    0,    0,  764,  899,  734,  516,    0,  517,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  758,
    0,    0,  519,    0,    0,  899,    0,  335,  477,  899,
    0,  478,    0,  734,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  734,    0,  734,    0,    0,
    0,    0,    0,    0,    0,    0,  479,    0,  480,  481,
    0,    0,    0,    0,    0,    0,    0,    0,  482,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
 1010,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  734,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  734,    0,    0,   14,  734,  920,
    0,  760,    0,    0,    0,    0,    0,  484,    0,    0,
    0,    0,    0,  485, 1134,    0,    0,    0,    0,    0,
    0,    0,    0,  486,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  487,  488,  489,
  490,  256,  257,  116,    0,  117,  118,  119,  120,    0,
  121,  491,    0,    0,    0,    0,    0,    0,  122,  123,
  492,  493,  494,  495,  496,  497,  498,  499,  500,  124,
  501,    0,  126,    0,    0,  502,  503,    0,  504,  505,
  506,    0,  129,  130,  507,  131,    0,    0,  132,  133,
  134,  135,  136, 1011, 1012, 1013,  137,  138,    0,    0,
    0,    0,  139,  508,  141,    0,  509,  143,  144,    0,
  145,  472,  510,  511,  512,    0,  513,  514,  473,  515,
  146,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  474,    0,    0,  757,    0,    0,  475,  476,    0,    0,
  764,    0,    0,  516,    0,  517,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  758,    0,    0,  519,    0,
    0,    0,    0,  335,  477,    0,    0,  478,    0,    0,
  115,    0,    0,  116,    0,  117,  118,  119,  120,    0,
  121,    0,    0,    0,    0,    0,    0,    0,  122,  123,
    0,    0,  479,    0,  480,  481,    0,    0,    0,  124,
  125,    0,  126,    0,  482,    0,    0,    0,  483,  127,
  128,    0,  129,  130,    0,  131,  759,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  146,    0,    0,    0,    0,    0,    0,  760,    0,    0,
    0,    0,    0,  484,    0,    0,    0,    0,    0,  485,
    0,    0, 1107,    0,    0,    0,    0,    0,    0,  486,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  487,  488,  489,  490,  256,  257,  116,
    0,  117,  118,  119,  120,    0,  121,  491,    0,    0,
    0,    0,    0,    0,  122,  123,  492,  493,  494,  495,
  496,  497,  498,  499,  500,  124,  501,    0,  126,    0,
    0,  502,  503,    0,  504,  505,  506,    0,  129,  130,
  507,  131,    0,    0,  132,  133,  761,  762,  763,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  508,
  141,    0,  509,  143,  144,    0,  145,  472,  510,  511,
  512,    0,  513,  514,  473,  515,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  474,    0,    0,    0,
    0,    0,  475,  476,    0,    0,  764,    0,    0,  516,
    0,  517,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  758,    0,    0,  519,    0,    0,    0,    0,  335,
  477,    0,    0,  478,    0,    0,  115,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,    0,
    0,    0,    0,    0,  122,  123,    0,    0,  479,    0,
  480,  481,    0,    0,    0,  124,  125,    0,  126,    0,
  482,    0,    0,    0,  483,  127,  128,    0,  129,  130,
    0,  131, 1006,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,  760,    0,    0,    0,    0,    0,  484,
    0,    0,    0, 1106,    0,  485,    0,    0,    0, 1107,
    0,    0,    0,    0,    0,  486,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  487,
  488,  489,  490,  256,  257,  116,    0,  117,  118,  119,
  120,    0,  121,  491,    0,    0,    0,    0,    0,    0,
  122,  123,  492,  493,  494,  495,  496,  497,  498,  499,
  500,  124,  501,    0,  126,    0,    0,  502,  503,    0,
  504,  505,  506,    0,  129,  130,  507,  131,    0,    0,
  132,  133,  761,  762,  763,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  508,  141,    0,  509,  143,
  144,    0,  145,  472,  510,  511,  512,    0,  513,  514,
  473,  515,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  474,    0,    0,    0,    0,    0,  475,  476,
    0,    0,  589,    0,    0,  516,    0,  517,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  758,    0,    0,
  519,    0,    0,    0,    0,  335,  477,    0,    0,  478,
    0,    0,  115,    0,    0,  116,    0,  117,  118,  119,
  120,    0,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,    0,    0,  479,    0,  480,  481,    0,    0,
    0,  124,  125,    0,  126,    0,  482,    0,    0,    0,
  483,  127,  128,    0,  129,  130,    0,  131,  759,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  146,    0,    0,    0,    0,    0,    0,  760,
    0,    0,    0,    0,    0,  484,    0,    0,    0,    0,
    0,  485,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  486,    0,    0,    0,    0,    0,   85,    0,    0,
    0,    0,    0,    0,    0,  487,  488,  489,  490,  256,
  257,  116,    0,  117,  118,  119,  120,    0,  121,  491,
    0,    0,    0,    0,    0,    0,  122,  123,  492,  493,
  494,  495,  496,  497,  498,  499,  500,  124,  501,    0,
  126,    0,    0,  502,  503,    0,  504,  505,  506,    0,
  129,  130,  507,  131,    0,    0,  132,  133,  761,  762,
  763,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  508,  141,    0,  509,  143,  144,    0,  145,  472,
  510,  511,  512,    0,  513,  514,  473,  515,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  474,    0,
    0,    0,    0,    0,  475,  476,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  764,    0,
    0,  516,    0,  517,    0,    0,    0,    0,    0,    0,
    0,    0,  477,    0,    0,  478,  519,    0,    0,  115,
    0,  335,  116,    0,  117,  118,  119,  120,    0,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123,    0,
  479,    0,  480,  481,    0,    0,    0,    0,  124,  125,
    0,  126,  482,    0,    0,    0,  483,    0,  127,  128,
    0,  129,  130,    0,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,    0,  142,  143,  144,    0,  145,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,    0,    0,    0,    0,    0,  485,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  486,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  487,  488,  489,  490,  256,  257,  116,    0,  117,
  118,  119,  120,  707,  121,  491,    0,    0,    0,    0,
    0,    0,  122,  123,  492,  493,  494,  495,  496,  497,
  498,  499,  500,  124,  501,    0,  126,    0,    0,  502,
  503,    0,  504,  505,  506,    0,  129,  130,  507,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  508,  141,    0,
  509,  143,  144, 1081,  145,    0,  510,  511,  512, 1082,
  513,  514, 1083,  515,  146,  472,    0, 1086,  826,  824,
    0,  825,  473,  827,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  474,    0, 1031, 1030, 1032,    0,
  475,  476,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  589,    0,    0,  516,  758,
  517,    0,    0,    0,    0,    0,    0,    0,  477,    0,
    0,  478,   84,  519,  115,    0,    0,  116,  335,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,    0,  479,    0,  480,  481,
    0,    0,    0,  124,  125,    0,  126,    0,  482,    0,
    0,    0,  483,  127,  128,    0,  129,  130,    0,  131,
 1006,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,  760,    0,    0,    0,    0,    0,  484,    0,    0,
    0,    0,    0,  485,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  486,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  487,  488,  489,
  490,  256,  257,  116,    0,  117,  118,  119,  120,    0,
  121,  491,    0,    0,  519,    0,  707,    0,  122,  123,
  492,  493,  494,  495,  496,  497,  498,  499,  500,  124,
  501,    0,  126,    0,    0,  502,  503,    0,  504,  505,
  506,    0,  129,  130,  507,  131,    0, 1018,  132,  133,
  134,  135,  136,  822,    0,    0,  137,  138,  823,    0,
    0, 1019,  139,  508,  141,    0,  509,  143,  144,    0,
  145,    0,  510,  511,  512, 1274,  513,  514, 1275,  515,
  146,    0,  472,    0,    0,    0,    0,    0,    0,  473,
    0,    0,    0,    0,    0,    0,    0, 1020,    0,    0,
    0,  474,    0,    0,    0, 1021,    0,  475,  476,    0,
    0,    0,    0, 1022,    0,    0,    0,    0,    0,    0,
    0,    0,  589,    0,    0,  516,    0,  517,    0,    0,
    0, 1023,    0, 1024,    0,  477,    0,    0,  478,    0,
  519,    0,    0,    0,    0,  335, 1025,    0, 1026, 1027,
 1028,  710,    0,    0,  710,    0,  710,  710,  710,  710,
    0,  710,    0,  479,    0,  480,  481,    0,    0,  710,
  710,    0,    0,    0,    0,  482,    0,    0,    0,  483,
  710,  710,    0,  710,    0,    0,    0,    0,    0,    0,
  710,  710,    0,  710,  710,    0,  710, 1029,    0,  710,
  710,  710,  710,  710,    0,    0,    0,  710,  710,    0,
    0,    0,    0,  710,  710,  710,    0,  710,  710,  710,
    0,  710,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  710, 1276,    0,  484,    0,    0,    0,    0,    0,
  485,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  486,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  487,  488,  489,  490,  256,  257,
  116,    0,  117,  118,  119,  120,    0,  121,  491,    0,
    0,    0,    0,    0,    0,  122,  123,  492,  493,  494,
  495,  496,  497,  498,  499,  500,  124,  501,    0,  126,
    0,    0,  502,  503,    0,  504,  505,  506,    0,  129,
  130,  507,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  508,  141,    0,  509,  143,  144,    0,  145,    0,  510,
  511,  512, 1285,  513,  514, 1275,  515,  146,    0,  472,
    0,    0,  826,  824,    0,  825,  473,  827,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  474,    0,
 1031, 1030, 1032,    0,  475,  476,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  589,
    0,    0,  516,    0,  517,    0,    0,    0,    0,    0,
    0,    0,  477,    0,    0,  478,    0,  519,  115,    0,
    0,  116,  335,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,    0,
  479,    0,  480,  481,    0,    0,    0,  124,  125,    0,
  126,    0,  482,    0,    0,    0,  483,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1276,
    0,  484,    0,    0,    0,    0,    0,  485,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  486,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  487,  488,  489,  490,  256,  257,  116,    0,  117,
  118,  119,  120,    0,  121,  491,    0,    0,    0,    0,
    0,    0,  122,  123,  492,  493,  494,  495,  496,  497,
  498,  499,  500,  124,  501,    0,  126,    0,    0,  502,
  503,    0,  504,  505,  506,    0,  129,  130,  507,  131,
    0, 1018,  132,  133,  134,  135,  136,  822,    0,    0,
  137,  138,  823,    0,    0, 1019,  139,  508,  141,    0,
  509,  143,  144,    0,  145,    0,  510,  511,  512, 1289,
  513,  514, 1275,  515,  146,    0,  472,    0,    0,    0,
    0,    0,    0,  473,    0,    0,    0,    0,    0,    0,
    0, 1020,    0,    0,    0,  474,    0,    0,    0, 1021,
    0,  475,  476,    0,    0,    0,    0, 1022,    0,    0,
    0,    0,    0,    0,    0,    0,  589,    0,    0,  516,
    0,  517,    0,    0,    0, 1023,    0, 1024,  570,  477,
    0,    0,  478,    0,  519,    0,    0,    0,    0,  335,
 1025,    0, 1026, 1027, 1028,  115,    0,    0,  116,    0,
  117,  118,  119,  120,    0,  121,    0,  479,    0,  480,
  481,    0,    0,  122,  123,    0,    0,    0,    0,  482,
    0,    0,    0,  483,  124,  125,    0,  126,    0,    0,
    0,    0,    0,    0,  127,  128,    0,  129,  130,    0,
  131, 1029,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  146, 1276,    0,  484,    0,
    0,    0,    0,    0,  485,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  486,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  487,  488,
  489,  490,  256,  257,  116,    0,  117,  118,  119,  120,
    0,  121,  491,    0,    0,    0,    0,    0,    0,  122,
  123,  492,  493,  494,  495,  496,  497,  498,  499,  500,
  124,  501,    0,  126,    0,    0,  502,  503,    0,  504,
  505,  506,    0,  129,  130,  507,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  508,  141,    0,  509,  143,  144,
    0,  145,    0,  510,  511,  512, 1297,  513,  514, 1275,
  515,  146,    0,  472,  220,    0,    0,  220,    0,    0,
  473,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,    0,  474,    0,  220,    0,    0,  220,  475,  476,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  589,    0,    0,  516,    0,  517,    0,
    0,    0,    0,    0,    0,    0,  477,    0,    0,  478,
    0,  519,    0,    0,    0,    0,  335,  220,    0,    0,
    0,    0,    0,  220,  220,  220,    0,  220,  220,  220,
    0,  220,    0,    0,  479,    0,  480,  481,    0,    0,
    0,    0,    0,    0,    0,    0,  482,    0,    0,  220,
  483,  220,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,    0,    0,    0,    0,   81,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  220,    0,    0,
    0,    0,    0, 1276,    0,  484,    0,    0,    0,    0,
    0,  485,    0,    0,    0,    0,    0,    0,    0,  220,
    0,  486,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  487,  488,  489,  490,  256,
  257,  116,    0,  117,  118,  119,  120,    0,  121,  491,
    0,    0,    0,    0,  220,  220,  122,  123,  492,  493,
  494,  495,  496,  497,  498,  499,  500,  124,  501,    0,
  126,  220,    0,  502,  503,    0,  504,  505,  506,    0,
  129,  130,  507,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  508,  141,    0,  509,  143,  144,    0,  145,    0,
  510,  511,  512, 1300,  513,  514, 1275,  515,  146,    0,
  472,  698,    0,    0,  699,    0,    0,  473,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  474,
    0,  700,    0,    0,  701,  475,  476,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  589,    0,    0,  516,    0,  517,    0,    0,    0,    0,
    0,    0,    0,  477,    0,    0,  478,    0,  519,    0,
    0,    0,    0,  335,  702,    0,    0,    0,    0,    0,
  703,  704,  705,    0,  706,  707,  708,    0,  709,    0,
    0,  479,    0,  480,  481,    0,    0,    0,    0,    0,
    0,    0,    0,  482,    0,    0,  710,  483,  711,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  712,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  713,    0,    0,    0,    0,    0,
 1276,    0,  484,    0,    0,    0,    0,    0,  485,    0,
    0,    0,    0,    0,    0,    0,  714,    0,  486,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  487,  488,  489,  490,  256,  257,  116,    0,
  117,  118,  119,  120,    0,  121,  491,    0,    0,    0,
    0,  715,  716,  122,  123,  492,  493,  494,  495,  496,
  497,  498,  499,  500,  124,  501,    0,  126,  717,    0,
  502,  503,    0,  504,  505,  506,    0,  129,  130,  507,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  508,  141,
    0,  509,  143,  144,    0,  145,    0,  510,  511,  512,
 1305,  513,  514, 1275,  515,  146,    0,  472,  149,    0,
    0,  149,    0,    0,  473,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  474,    0,  149,    0,
    0,  149,  475,  476,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  589,    0,    0,
  516,    0,  517,    0,    0,    0,    0,    0,    0,    0,
  477,    0,    0,  478,    0,  519,    0,    0,    0,    0,
  335,  149,    0,    0,    0,    0,    0,  149,  149,  149,
    0,  149,  149,  149,    0,  149,    0,    0,  479,    0,
  480,  481,    0,    0,    0,    0,    0,    0,    0,    0,
  482,    0,    0,  149,  483,  149,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  149,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  149,    0,    0,    0,    0,    0, 1276,    0,  484,
    0,    0,    0,    0,    0,  485,    0,    0,    0,    0,
    0,    0,    0,  149,    0,  486,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  487,
  488,  489,  490,  256,  257,  116,    0,  117,  118,  119,
  120,    0,  121,  491,    0,    0, 1371,    0,  149,  149,
  122,  123,  492,  493,  494,  495,  496,  497,  498,  499,
  500,  124,  501,    0,  126,  149,    0,  502,  503,    0,
  504,  505,  506,    0,  129,  130,  507,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  508,  141,    0,  509,  143,
  144,    0,  145,    0,  510,  511,  512, 1308,  513,  514,
 1275,  515,  146,    0,  472,    0, 1372,    0,    0,    0,
    0,  473,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  474,    0,    0,    0,    0,    0,  475,
  476,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1373,    0,    0,  589,    0, 1374,  516,    0,  517,
    0,    0, 1375,    0,    0,    0,    0,  477,    0,    0,
  478,    0,  519,  115,    0,    0,  116,  335,  117,  118,
  119, 1376,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,    0,    0,  479,    0,  480,  481,    0,
    0,    0,  124,  125,    0,  126,    0,  482,    0,    0,
    0,  483,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1276,    0,  484,    0,    0,    0,
    0,    0,  485,  666,    0,    0,    0,    0,    0,    0,
    0,    0,  486,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  487,  488,  489,  490,
  256,  257,  116,    0,  117,  118,  119,  120,    0,  121,
  491,    0,    0,    0,    0,    0,    0,  122,  123,  492,
  493,  494,  495,  496,  497,  498,  499,  500,  124,  501,
    0,  126,    0,    0,  502,  503,    0,  504,  505,  506,
    0,  129,  130,  507,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  508,  141,    0,  509,  143,  144,    0,  145,
    0,  510,  511,  512, 1311,  513,  514, 1275,  515,  146,
    0,  472,    0,    0,    0,    0,    0,    0,  473,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  474,    0,    0,    0,    0,    0,  475,  476,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  694,    0,  694,
  694,    0,  694,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  477,  694,    0,  478,    0,    0,
  694,  115,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,    0,    0,    0,    0,    0,    0,  122,
  123,    0,  479,    0,  480,  481,    0,    0,    0,    0,
  124,  125,    0,  126,  482,    0,    0,    0,  483,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1276,    0,  484,    0,    0,  672,    0,    0,  485,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  486,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  487,  488,  489,  490,  256,  257,  116,
    0,  117,  118,  119,  120,    0,  121,  491,    0,    0,
    0,    0,    0,    0,  122,  123,  492,  493,  494,  495,
  496,  497,  498,  499,  500,  124,  501,    0,  126,    0,
    0,  502,  503,    0,  504,  505,  506,    0,  129,  130,
  507,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  508,
  141,    0,  509,  143,  144,    0,  145,  694,  510,  511,
  512,    0,  513,  514,  694,  515,  146,    0,    0,    0,
    0,  694,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  694,    0,    0,    0,    0,    0,  694,
  694,    0,    0,    0,    0,    0,    0,  694,    0,    0,
  688,    0,  688,  688,    0,  688,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  694,  688,    0,
  694,    0,    0,  688,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,  694,    0,  694,  694,    0,
    0,    0,    0,  124,  125,    0,  126,  694,    0,    0,
    0,  694,    0,  127,  128,    0,  129,  130,    0,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  694,    0,    0,    0,
    0,    0,  694,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  694,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  694,  694,  694,  694,
  694,  694,  694,    0,  694,  694,  694,  694,    0,  694,
  694,    0,    0,    0,    0,    0,    0,  694,  694,  694,
  694,  694,  694,  694,  694,  694,  694,  694,  694,  694,
    0,  694,    0,    0,  694,  694,    0,  694,  694,  694,
    0,  694,  694,  694,  694,    0,    0,  694,  694,  694,
  694,  694,    0,    0,    0,  694,  694,    0,    0,    0,
    0,  694,  694,  694,    0,  694,  694,  694,    0,  694,
  688,  694,  694,  694,    0,  694,  694,  688,  694,  694,
    0,    0,    0,    0,  688,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  688,    0,    0,    0,
    0,    0,  688,  688,    0,    0,  589,    0,  594,  516,
  688,  517,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  519,    0,    0,    0,    0,  335,
  688,    0,    0,  688,    0,    0,  115,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,    0,
    0,    0,    0,    0,  122,  123,    0,    0,  688,    0,
  688,  688,    0,    0,    0,  124,  125,    0,  126,    0,
  688,    0,    0,    0,  688,  127,  128,    0,  129,  130,
    0,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  688,
    0,    0,    0,    0,    0,  688,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  688,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  688,
  688,  688,  688,  688,  688,  688,    0,  688,  688,  688,
  688,    0,  688,  688,    0,    0,    0,    0,    0,    0,
  688,  688,  688,  688,  688,  688,  688,  688,  688,  688,
  688,  688,  688,    0,  688,    0,    0,  688,  688,    0,
  688,  688,  688,    0,  688,  688,  688,  688,    0,    0,
  688,  688,  688,  688,  688,    0,    0,    0,  688,  688,
    0,    0,    0,    0,  688,  688,  688,    0,  688,  688,
  688,    0,  688,  472,  688,  688,  688,    0,  688,  688,
  473,  688,  688,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  474,    0,    0,    0,    0,    0,  475,  476,
    0,    0,  589,    0,    0,  516,    0,  517,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  519,    0,    0,    0,    0,  335,  477,    0,    0,  478,
    0,    0,   28,    0,    0,   28,    0,   28,   28,   28,
   28,    0,   28,    0,    0,    0,    0,    0,    0,    0,
   28,   28,    0,    0,  479,    0,  480,  481,    0,    0,
    0,   28,   28,    0,   28,    0,  482,    0,    0,    0,
  483,   28,   28,    0,   28,   28,    0,   28,    0,    0,
   28,   28,   28,   28,   28,    0,    0,    0,   28,   28,
    0,    0,    0,    0,   28,   28,   28,    0,   28,   28,
   28,    0,   28,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   28,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,    0,    0,    0,    0,
    0,  485,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  486,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  487,  488,  489,  490,  256,
  257,  116,    0,  117,  118,  119,  120,    0,  121,  491,
    0,    0,    0,    0,    0,    0,  122,  123,  492,  493,
  494,  495,  496,  497,  498,  499,  500,  124,  501,    0,
  126,    0,    0,  502,  503,    0,  504,  505,  506,    0,
  129,  130,  507,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  508,  141,    0,  509,  143,  144,    0,  145,  472,
  510,  511,  512,    0,  513,  514,  473,  515,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  474,    0,
    0,    0,    0,    0,  475,  476,    0,    0,  589,    0,
    0,  516,    0,  517,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  519,    0,    0,    0,
    0,  335,  477,    0,    0,  478,    0,    0,  602,    0,
    0,  602,    0,  602,  602,  602,  602,    0,  602,    0,
    0,    0,    0,    0,    0,    0,  602,  602,    0,    0,
  479,    0,  480,  481,    0,    0,    0,  602,  602,    0,
  602,    0,  482,    0,    0,    0,  483,  602,  602,    0,
  602,  602,    0,  602,    0,    0,  602,  602,  602,  602,
  602,    0,    0,    0,  602,  602,    0,    0,    0,    0,
  602,  602,  602,    0,  602,  602,  602,    0,  602,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  602,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,    0,    0,    0,    0,    0,  485,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  486,    0,    0,
    0,    0,    0,    0,    0,  799,    0,    0,    0,    0,
    0,  487,  488,  489,  490,  256,  257,  116,    0,  117,
  118,  119,  120,    0,  121,  491,    0,    0,    0,    0,
    0,    0,  122,  123,  492,  493,  494,  495,  496,  497,
  498,  499,  500,  124,  501,    0,  126,    0,    0,  502,
  503,    0,  504,  505,  506,    0,  129,  130,  507,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  508,  141,    0,
  509,  143,  144,    0,  145,  472,  510,  511,  512,    0,
  513,  514,  473,  515,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  474,    0,    0,    0,    0,    0,
  475,  476,    0,    0,  589, 1090,    0,  516,    0,  517,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  519,    0,    0,    0,    0,  335,  477,    0,
    0,  478,    0,    0,  286,    0,    0,  286,    0,  286,
  286,  286,  286,    0,  286,    0,    0,    0,    0,    0,
    0,    0,  286,  286,    0,    0,  479,    0,  480,  481,
    0,    0,    0,  286,  286,    0,  286,    0,  482,    0,
    0,    0,  483,  286,  286,    0,  286,  286,    0,  286,
    0,    0,  286,  286,  286,  286,  286,    0,    0,    0,
  286,  286,    0,    0,    0,    0,  286,  286,  286,    0,
  286,  286,  286,    0,  286,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  286,    0,    0,   14,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  484,    0,    0,
    0,    0,    0,  485,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  486,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  487,  488,  489,
  490,  256,  257,  116,    0,  117,  118,  119,  120,    0,
  121,  491,    0,    0,    0,    0,    0,    0,  122,  123,
  492,  493,  494,  495,  496,  497,  498,  499,  500,  124,
  501,    0,  126,    0,    0,  502,  503,    0,  504,  505,
  506,    0,  129,  130,  507,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  508,  141,    0,  509,  143,  144,    0,
  145,  472,  510,  511,  512,    0,  513,  514,  473,  515,
  146,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  474,    0,    0,    0,    0,    0,  475,  476,    0,    0,
  589,    0,    0,  516,    0,  517,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  519,    0,
    0,    0,    0,  335,  477,    0,    0,  478,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,  480,  481,    0,    0,    0,    0,
    0,    0,    0,    0,  482,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,    0,    0,    0,    0,    0,  485,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  486,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  487,  488,  489,  490,  256,  257,  116,
    0,  117,  118,  119,  120,    0,  121,  491,    0,    0,
    0,    0,    0,    0,  122,  123,  492,  493,  494,  495,
  496,  497,  498,  499,  500,  124,  501,    0,  126,    0,
    0,  502,  503,    0,  504,  505,  506,    0,  129,  130,
  507,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  508,
  141,    0,  509,  143,  144,    0,  145,  472,  510,  511,
  512,    0,  513,  514,  473,  515,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  474,    0,    0,    0,
    0,    0,  475,  476,    0,    0,  589,    0,    0,  516,
    0,  517,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  519,    0,    0,    0,    0,  335,
  477,    0,    0,  478,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  479,    0,
  480,  481,    0,    0,    0,    0,    0,    0,    0,    0,
  482,    0,    0,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  484,
    0,    0,    0,    0,    0,  485,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  486,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1303,    0,    0,  487,
  488,  489,  490,  256,  257,  116,    0,  117,  118,  119,
  120,    0,  121,  491,    0,    0,    0,    0,    0,    0,
  122,  123,  492,  493,  494,  495,  496,  497,  498,  499,
  500,  124,  501,    0,  126,    0,    0,  502,  503,    0,
  504,  505,  506,    0,  129,  130,  507,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  508,  141,    0,  509,  143,
  144,    0,  145,  472,  510,  511,  512,    0,  513,  514,
  473,  515,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  474,    0,    0,    0,    0,    0,  475,  476,
    0,    0,  589,    0,    0,  516,    0,  517,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  519,    0,    0, 1356,    0,  335,  477,    0,    0,  478,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  479,    0,  480,  481,    0,    0,
    0,    0,    0,    0,    0,    0,  482,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,    0,    0,    0,    0,
    0,  485,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  486,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  487,  488,  489,  490,  256,
  257,  116,    0,  117,  118,  119,  120,    0,  121,  491,
    0,    0,    0,    0,    0,    0,  122,  123,  492,  493,
  494,  495,  496,  497,  498,  499,  500,  124,  501,    0,
  126,    0,    0,  502,  503,    0,  504,  505,  506,    0,
  129,  130,  507,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  508,  141,    0,  509,  143,  144,    0,  145,  472,
  510,  511,  512,    0,  513,  514,  473,  515,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  474,    0,
    0,    0,    0,    0,  475,  476,    0,    0, 1059,    0,
 1059, 1059,    0, 1059,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1059,    0,    0,    0,
    0, 1059,  477,    0,    0,  478,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  479,    0,  480,  481,    0,    0,    0,    0,    0,    0,
    0,    0,  482,    0,    0,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,    0,    0,    0,    0,    0,  485,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  486,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1511,    0,
    0,  487,  488,  489,  490,  256,  257,  116,    0,  117,
  118,  119,  120,    0,  121,  491,    0,    0,    0,    0,
    0,    0,  122,  123,  492,  493,  494,  495,  496,  497,
  498,  499,  500,  124,  501,    0,  126,    0,    0,  502,
  503,    0,  504,  505,  506,    0,  129,  130,  507,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  508,  141,    0,
  509,  143,  144,    0,  145, 1059,  510,  511,  512,    0,
  513,  514, 1059,  515,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1059,    0,    0,    0,    0,    0,
 1059, 1059,    0,    0,  518,    0,    0,  516,    0,  517,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  519,    0,    0,    0,    0,  335, 1059,    0,
    0, 1059,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1059,    0, 1059, 1059,
    0,    0,    0,    0,    0,    0,    0,    0, 1059,    0,
    0,    0, 1059,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1059,    0,    0,
    0,    0,    0, 1059,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1059,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1059, 1059, 1059,
 1059, 1059, 1059, 1059,    0, 1059, 1059, 1059, 1059,    0,
 1059, 1059,    0,    0,    0,    0,    0,    0, 1059, 1059,
 1059, 1059, 1059, 1059, 1059, 1059, 1059, 1059, 1059, 1059,
 1059,    0, 1059,    0,    0, 1059, 1059,    0, 1059, 1059,
 1059,    0, 1059, 1059, 1059, 1059,    0,    0, 1059, 1059,
 1059, 1059, 1059,    0,    0,    0, 1059, 1059,    0,    0,
    0,    0, 1059, 1059, 1059,    0, 1059, 1059, 1059,    0,
 1059,  472, 1059, 1059, 1059,    0, 1059, 1059,  473, 1059,
 1059,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  474,    0,    0,    0,    0,    0,  475,  476,    0,    0,
  589,    0,    0,  516,    0,  517,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  519,    0,
    0,    0,    0,  335,  477,    0,    0,  478,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,  480,  481,    0,    0,    0,    0,
    0,    0,    0,    0,  482,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,    0,    0,    0,    0,    0,  485,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  486,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  487,  488,  489,  490,  256,  257,  116,
    0,  117,  118,  119,  120,    0,  121,  491,    0,    0,
    0,    0,    0,    0,  122,  123,  492,  493,  494,  495,
  496,  497,  498,  499,  500,  124,  501,    0,  126,    0,
    0,  502,  503,    0,  504,  505,  506,    0,  129,  130,
  507,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  508,
  141,    0,  509,  143,  144,    0,  145,  472,  510,  511,
  512,    0,  513,  514,  473,  515,  146,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  474,    0,    0,    0,
    0,    0,  475,  476,    0,    0, 1059,    0,    0, 1059,
    0, 1059,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1059,    0,    0,    0,    0, 1059,
  477,    0,    0,  478,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  479,    0,
  480,  481,    0,    0,    0,    0,    0,    0,    0,    0,
  482,    0,    0,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  484,
    0,    0,    0,    0,    0,  485,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  486,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  487,
  488,  489,  490,  256,  257,  116,    0,  117,  118,  119,
  120,    0,  121,  491,    0,    0,    0,    0,    0,    0,
  122,  123,  492,  493,  494,  495,  496,  497,  498,  499,
  500,  124,  501,    0,  126,    0,    0,  502,  503,    0,
  504,  505,  506,    0,  129,  130,  507,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  508,  141,    0,  509,  143,
  144,  569,  145, 1059,  510,  511,  512,    0,  513,  514,
 1059,  515,  146,  570,    0,    0,    0,    0,    0,    0,
    0,    0, 1059,    0,    0,    0,    0,    0, 1059, 1059,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  569,  569,    0,    0,  569,    0,    0,    0,    0,
  564,    0,    0,  570,  570,    0, 1059,  570,    0, 1059,
  569,    0,  569,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  570,    0,  570,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1059,    0, 1059, 1059,    0,    0,
    0,  564,  569,    0,  564,    0, 1059,    0,    0,    0,
 1059,    0,    0,    0,  570,    0,    0,    0,    0,  564,
    0,  564,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  564,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1059,    0,    0,    0,    0,
    0, 1059,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1059,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1059, 1059, 1059, 1059, 1059,
 1059, 1059,    0, 1059, 1059, 1059, 1059,    0, 1059, 1059,
    0,    0,    0,    0,    0,    0, 1059, 1059, 1059, 1059,
 1059, 1059, 1059, 1059, 1059, 1059, 1059, 1059, 1059,    0,
 1059,    0,  586, 1059, 1059,    0, 1059, 1059, 1059,    0,
 1059, 1059, 1059, 1059,    0,    0, 1059, 1059, 1059, 1059,
 1059,    0,    0,    0, 1059, 1059,    0,    0,    0,    0,
 1059, 1059, 1059,    0, 1059, 1059, 1059,    0, 1059,  569,
 1059, 1059, 1059,  586, 1059, 1059,  586, 1059, 1059,    0,
    0,  570,    0,    0,  569,    0,  569,  569,  569,    0,
  576,  586,  569,  586,    0,  569,  570,    0,  570,  570,
  570,    0,    0,    0,  570,    0,    0,  570,  569,    0,
    0,    0,    0,    0,  569,    0,    0,  569,  564,    0,
  570,    0,    0,  586,    0,    0,  570,    0,    0,  570,
    0,  576,    0,  564,  576,  564,  564,  564,    0,    0,
    0,  564,    0,    0,  564,    0,    0,    0,    0,  576,
    0,  576,    0,    0,    0,    0,    0,  564,    0,  569,
    0,    0,    0,  564,    0,    0,  564,    0,    0,    0,
    0,  570,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  576,    0,  569,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  570,    0,    0,    0,    0,
    0,  561,    0,    0,  569,    0,    0,    0,  564,    0,
  569,    0,    0,    0,  569,    0,  570,    0,    0,    0,
    0,  569,  570,    0,    0,    0,  570,    0,    0,    0,
    0,    0,  564,  570,    0,    0,    0,    0,    0,  569,
    0,    0,  561,    0,  569,  561,    0,    0,    0,    0,
    0,  570,    0,  564,    0,    0,  570,    0,    0,  564,
  561,    0,  561,  564,    0,    0,    0,    0,    0,  569,
  564,    0,    0,  569,    0,    0,    0,    0,    0,    0,
    0,  570,    0,    0,    0,  570,    0,    0,  564,    0,
  586,    0,  561,  564,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  586,    0,    0,  586,  586,
    0,    0,    0,  586,    0,    0,  586,    0,  564,    0,
    0,    0,  564,    0,    0,    0,    0,    0,    0,  586,
    0,    0,    0,    0,    0,  586,    0,    0,  586,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  576,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  576,    0,    0,  576,  576,    0,    0,
    0,  576,    0,    0,  576,    0,    0,    0,    0,    0,
  586,    0,    0,    0,    0,    0,    0,  576,    0,    0,
    0,    0,    0,  576,    0,    0,  576,    0,    0,    0,
    0,    0,    0,    0,  586,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  586,    0,    0,    0,    0,
    0,  586,    0,    0,    0,  586,    0,    0,  576,    0,
    0,    0,  586,    0,    0,    0,  108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  561,
  586,    0,  576,    0,    0,  586,    0,    0,    0,    0,
    0,    0,    0,    0,  561,    0,    0,  561,  561,    0,
    0,    0,  561,  576,  109,  561,    0,    0,    0,  576,
  586,    0,    0,  576,  586,    0,  110,    0,  561,    0,
  576,    0,    0,    0,  561,    0,    0,  561,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  576,    0,
  111,    0,   49,  576,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  576,  561,
    0,    0,  576,    0,    0,    0,    0,    0,    0,    0,
   49,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   49,  561,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  112,    0,    0,    0,    0,
    0,    0,    0,    0,  561,  113,   49,    0,    0,    0,
  561,    0,    0,    0,  561,    0,    0,    0,    0,    0,
    0,  561,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,    0,    0,    0,    0,    0,    0,  561,
    0,    0,    0,    0,  561,    0,    0,    0,  115,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,  561,
    0,   49,    0,  561,    0,    0,    0,  124,  125,    0,
  126,   49,    0,    0,    0,    0,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,   49,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
  329,    0,    0,    0,   51,    0,    0,   51,  146,   51,
   51,   51,   51,    0,   51,    0,    0,    0,    0,    0,
    0,    0,   51,   51,    0,    0,    0,    0,  329,    0,
    0,    0,    0,   51,   51,  329,   51,    0,    0,  329,
  329,    0,  329,   51,   51,  329,   51,   51,    0,   51,
    0,    0,   51,   51,   51,   51,   51,    0,  329,    0,
   51,   51,    0,    0,  329,    0,   51,   51,   51,    0,
   51,   51,   51,    0,   51,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   51,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1368,    0,    0,    0,
    0,  329,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  329,
    0,    0,    0,    0,    6,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  329, 1566, 1567,    0, 1568,    0,
    0,    0,    0,    0,  329,    0,    0,    0,    0,    0,
    0,    0,    0,  329, 1569,  329,    0,    0,    0,    0,
 1570,    0,  329,    0,    0,  329,    0,  329,  329,  329,
  329,    0,  329,    0,    0,    0,    0,    0,    0,    0,
  329,  329,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  329,  329,    0,  329,    0,    0,    0,    0,    0,
    0,  329,  329,    0,  329,  329,    0,  329,    0,    0,
  329,  329,  329,  329,  329,    0,    0, 1571,  329,  329,
  329,  329,  329,    0,  329,  329,  329,    0,  329,  329,
  329, 1368,  329,    0,    0,   14,    0,    0,    0,    0,
    0,    0,  329,    0,    0,    0,    0,    0,    0,    0,
 1572,    0,    0,    0,    0,    0,    0,    0,    0,    6,
   16,    0,    0,    0,    0,    0,    0,    0,    0, 1772,
 1566, 1567,    0, 1568,    0,    0,    0,    0,  490,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121, 1569,
    0,    0,    0,    0,    0, 1570,  122,  123,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1573,  125,    0,
  126,    0,    0,    0,    0,    0,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132, 1574,  134,  135,
  136,    0,    0,    0,  137,  138, 1575, 1576, 1577,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0, 1571,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1572,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  490,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,  698,    0,    0,  699,
    0,  122,  123,    0,    0,  920,    0,    0,    0,    0,
    0,    0, 1573,  125,    0,  126,  700,    0,    0,  701,
    0,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132, 1574,  134,  135,  136,    0,    0,    0,  137,
  138, 1575, 1576, 1577,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,  702,
    0,    0,    0,  146,    0,  703,  704,  705,    0,  706,
  707,  708,    0,  709,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  710,    0,  711,  698,    0,    0,  699,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  700,    0,    0,  701,    0,    0,
    0,    0,  712,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  713,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  702,    0,    0,
    0,  714,    0,  703,  704,  705,    0,  706,  707,  708,
    0,  709,    0,    0,    0,    0,  115,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,  710,
    0,  711,    0,    0,  122,  123,  715,  716,    0,    0,
    0,    0,    0,    0,    0,  124,  125,    0,  126,    0,
    0,    0,    0,  717,    0,  127,  128,    0,  129,  130,
  712,  131,    0,    0,  132,  133,  134,  135,  136,    0,
 1035,    0,  137,  138,    0,    0,    0,  713,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0, 1036,    0,    0,  146,    0,    0,  714,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,  715,  716,    0,    0,    0,    0,
    0,    0,    0,  124,  125,    0,  126,    0,    0, 1037,
    0,  717,    0,  127,  128,    0,  129,  130,    0,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,  115,  145,    0,  116,    0,  117,  118,
  119,  120,    0,  121,  146,    0,    0,    0,    0,    0,
    0,  122,  123,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  124,  125,    0,  126,    0,    0,    0,    0,
    0,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,  115,    0,  146,  116,  193,  117,  118,  119,  120,
  194,  121,  195,  196,  197,  198,  199,  200,  201,  122,
  123,  202,  203,  204,  205,  206,    0,    0,  207,  208,
  124,  125,  209,  126,    0,    0,  210,  211,  212,  213,
  127,  128,  214,  129,  130,  215,  131,  216,  217,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,  218,
  219,  220,  221,  139,  140,  141,  222,  142,  143,  144,
  223,  145,  224,  225,  226,  227,  228,  229,  230,  231,
  232,  146,  481,    0,    0,  481,  481,  481,  481,  481,
  481,    0,  481,  481,  481,  481,  481,  481,  481,  481,
  481,  481,  481,  481,  481,  481,  481,    0,    0,  481,
  481,  481,  481,  481,  481,    0,    0,  481,  481,  481,
  481,  481,  481,  481,  481,  481,  481,  481,  481,  481,
  481,  481,  481,  481,  481,    0,    0,    0,  481,  481,
  481,  481,  481,  481,  481,  481,  481,  481,  481,  481,
  481,  481,  481,  481,  481,  481,  481,  481,  481,  481,
  481,  481,  481,
};
static short yycheck[] = {                                       9,
    0,    4,  632,   71,  174,  114,   16,   70,   18,   69,
  413,   61,   77,  164,  165,  164,  165,  102,   70,   75,
  829,   62,   80,   16,  829,   93,  450,  320,  635,  518,
  450,  446,  908,   74,  559,  799,  101,   65,  655,  667,
  908,  805,  442,   99,  908,  103,  908,   97,   58,   59,
  578,   61,   62,   63,   64,   65,  518,   98,  885,   99,
   70,   71,   72,  908,   74,  106,   76,   77,   96,   79,
   80,   81,  908, 1040,  417, 1367,  104, 1368, 1432,   89,
  832,   91,   92,   93,   63,   95,   96,   97,   98,  782,
  100,  101,  102,  103,  104,  180,  106,   76,   91,  600,
  415,  427,  719,  891,  156, 1100,  158,  100,  176,  839,
  579,  337,  366,  745,  179, 1134, 1114, 1225,  752,  426,
  993, 1088,   40,    0,  817,    0,  271,  589,  397,  917,
  456,  883,    0,  658,   40,    0,  177,  662,  926,  184,
   44,  169,  667,   45,    0,   63,  156,  157,  158,   44,
   63,   45,  162,    0,   44,   44, 1472,   63,    0,  169,
    0,    0, 1247,    0,  174,   45,  176,  177,  178,  179,
  180,  280,    0,    0,  184,    0,  236,    0,  157,    0,
   41,   45,   59,  684,   59,    0,    0,   40,   40, 1386,
   44,   59, 1430,   41,   59,    0,  366,   44,  986,  178,
   44,   41,  759,   59,   44,    0,   41,  267,   44,    0,
   61,   44,   59,   41,    0,    0,   44,   59,   41,   59,
   59,   44,   59,    0,  369,  617,   41,   41,  643,   44,
   61,   59,   59, 1190,   59,   41,   59,   44,   59,   44,
    0,    0,   91,  667,   59,   59,   41,  667,  651,   44,
 1336,   41,  303, 1339,   59,   41,   41,   40,  890,  311,
  344,    0,  314,   45,   59,  273,  375,  670,   59, 1033,
 1034,  280,  351,   59,   59,  262,   44,  570,    0,  318,
  695,   63,   59, 1575, 1576, 1242,  262, 1643,  395, 1162,
    0, 1100,  331,  586,  620, 1100, 1587,  914,  348,   59,
   59,  311,  764,    0,  314,  922,    0, 1071,  368,   44,
  320,  321,  298,   41,  366,  354,   44,  395,  759,   41,
   59,  280,  310,  764,  268,  257,  303, 1398,  276,  368,
 1415,   41,  821,  950, 1650,    0,    0,   59,  348,  828,
    0,  324,  352,  300,   41,    0,  297,   41,  334,   59,
   44,  447, 1460,  298, 1941,  401,  265,  273,    0,  410,
  297,  257,   59,  396,  416,   59,  405,  317,  377,    0,
  334,  272, 1160,  352,  426, 1387,  428,   41,  430,  412,
 1399,  433,  428,  443,  452,  445,  396,  397,  276,  454,
  299,  348,  379,  500,   59,   59,  284,  348,  330,   59,
  460, 1988,  262,  379,   59,  484,  416,  344,  578,  895,
   41,  462,  453, 1177,  464,  359,  426,   59,  428,  944,
  430,  409,  431,  433,  372,  509,  275, 1665,   59,   41,
 1428,  264,  348,  410,  330, 1791,  514,  258, 1450,  448,
  427,  297,  452,  453,  454, 1412,  297,  336,  434,  270,
  458,  427,  462, 1010,  464,  297, 1413,  467,   44, 1545,
 1814,    0,    0, 1549,  467, 1158,  297, 1119,  969, 1467,
  434,  306,  431,  456,  866,  315,  297, 1769,  517,    0,
   41,  317,  303,  462,  305,  462,  376,  315,  344,  434,
   42,   43,  315,   45,  301,   47,  336,  325,   42,  327,
  277,  315,  344, 1267, 1268,  380, 1270, 1271,  336,  519,
  929,  339,  380,  336,  389,  380,  306, 1833,  277,  379,
   59,   59,  336,  344,  389,  994,  578,   41,  569,  315,
  315,  379,  450,  380,  387,  686, 1005,  450,   59,  325,
 1009,  327,  389,  588,  450,  410,  450,  315,  450,  444,
  336,  336,  380, 1362, 1908,  848,  450, 1362,  568,  569,
  570,  389, 1633, 1587,  401, 1006,  301,  577,  628, 1010,
  450,  581,  509,  379,  401, 1192,  586,  402,  588,  444,
  632,  376,  376,  477,  428,  645,  450,  439, 1785,  568,
  735,  428,  446, 1466,  380, 1949, 1124,  444,  577, 1707,
  660,  361,  581,  389,  432,  315,  446,  446,  915, 1241,
 1695, 1363,  622,  477,  336,  667,  444, 1971,  446, 1249,
 1254,  315,  632,  446,    0, 1644,  336,  896,  943,  689,
  884,  687,  446,  327, 1364,  861,  513, 1126,  513,  336,
  423,  446,  336,  622, 1935,  513,  432,  687,  513,  264,
  262, 1649,  513,  509,  997,  446,  666,  667,  984,  829,
  446,  446,  672,  402, 1750, 1134,  513,  509,  450, 1960,
  380,  494,  336,  513,  734,  403,  262, 1465,  493, 1194,
 1166,  741,  742,   59,  344,  513,  380,  836,  509, 1204,
  513,  315,  744,   59, 1163,  389, 1987,  344,  493,  513,
  365,  262,  344,    0,    0,  336,    0, 1998, 2000,    0,
 2001,  305,    0,  298,  884, 1230,  260, 2008,  869,  258,
 1677,  315,  432,  397,  446,  277,   41,  513,  513,  317,
  282,  270,  264,  276,  744,  305,  446, 1694,  432,  795,
 1392, 1393,  371,  264,   41,  315, 1713,   41,  262,  446,
   41,    0,  446,  498,  260,  277,  300,  276,  297,  787,
  282,  315,   59,   59,  303,   59,  305,  379,   59, 1659,
 1660,   59, 1172,  316,  448, 1770,  401,  787,  283, 1198,
 1199,  309,  446, 1659, 1660,  837,    0,  839,  403,  841,
 1279, 1659, 1660,  379,  300, 1659, 1660, 1659, 1660,    0,
  338,  952,    0,  513, 1497,  344,  453,  454, 1425, 1426,
   59,  440,  822,    0, 1659, 1660,  857, 1279,  379,  513,
 1647,  445,  832, 1659, 1660, 1442, 1433,  837, 1826,  839,
 1249,  841,  884,  371, 1832,  895,    0,  889,  848,  263,
  428,  276,  902, 1243,  315,   59,  856,  857,    0,  434,
  393,  445,  912,  372,  864,  865,    0, 1346,   59,  397,
  322,   59,  400,  915, 1655,  379,  239,  395,  878,  390,
  880,  403,   59,  883,  393,  445,  428,  856,  416,  889,
  399,  316, 1646,    0,  940,  309,  896,    0,  264,  941,
  433,  445,  944,  949,  339,   59,  320,  319,  958,  878,
  940,  880,  430, 1531,  442,  915, 1697,   59, 1397, 1252,
  966, 1935, 1427,   41,  433,   59,   44,  362,  291,  352,
    0,  313,  314,  979,  980,  981,  299,    0,   41, 1444,
 1100,  941,   42, 1928,  944, 1930, 1960,   47,  272,  979,
  980,  981,   59, 1938,    0, 1940,   59,  262,  453,  454,
  960,  318, 1453,  275, 1124, 1456,  261,  960,  393, 1474,
  422, 1770,  348, 1987,  331, 1770,    0,  429,   41,  262,
  509,   44,  339,  418, 1998,  985,  264, 2001, 1321,   59,
  276,  277, 1872, 1873, 2008,  995,   59,  354,  421, 1984,
    0,  304,  305, 1003, 1004,  419, 1872, 1873,  433, 1763,
 1003,  368,  336,   59, 1872, 1873,  985,   41, 1872, 1873,
 1872, 1873,   42,   43,  390,   45,  995,   47,  315,  341,
   93,  315,  380,    0,  315,   59,    0, 1872, 1873,  304,
  305,   41, 1494, 1495,    0, 1560, 1872, 1873,  405,  336,
 1502, 1503,  336, 1505, 1506,  336,    0, 1075,  297,   59,
  264, 1513, 1514, 1515, 1516, 1470, 1518, 1519, 1520, 1521,
 1522, 1523,  397,  264,  379, 1075,  264,  453,  454,  304,
  305,  258, 1124,    0,  432,  284,  372, 1228, 1371,    0,
  450, 1374,   59,  270, 1701,   59,  379,    0,  297,  380,
   44,   41,    0,   59,   44,  344,  450,  258,  389,    0,
  264,    0,  390,  352,  298,   59, 1166, 1531, 1118, 1119,
  297, 1531,    0,  448,   41,    0,  303,   44,  305, 1928,
   41, 1930,  260, 1928, 1134, 1930,    0, 1891,    0, 1938,
  443, 1940,   59, 1938, 1898, 1940, 1651,  386,   59,  388,
  334,  432,  303, 1658,  303,  273,   59, 1662, 1663,  446,
  517,   59,  446,   41,    0,  446,   41,  344,   59,   44,
   59,  410,  300,  409,  260,  352,   93,  277,  443,   41,
  319,   59,  282,    0,   59, 1984,  390,  484,  306, 1984,
 1236, 1696,    0, 1698,  264,   59,  435,   59,  291,  390,
  436,   40, 1362, 1245,  338, 1251, 1236, 1249, 1368,  302,
  319, 1257,  315,  371,  300,  399,  309, 1217,  443,  516,
  313, 1251,    0,   59,    0,  409,   44, 1257,    0, 1222,
  348,  338,  513,  336, 1234,   44,  390,  371,  300,    0,
  333,  390,   59,  259,  421, 1245, 1385, 1386, 1387, 1249,
  434,   59,  410,  269,  394,   42,   43,  277,   45,  410,
   47,  410,  282,  397,  371,  260,  400,   40,   40,   41,
  509,    0,   44,  336, 1667,   93,   44,  428, 1419,  428,
 1421,   59,  416,   59,   93,  403,  348,   59,  376,  265,
  397,  315,  338,  400, 1799, 1800,    0,  264,   59,  457,
 1375,  325,  395,  327,  457,  300,  459, 1786,  442,  416,
  372,  462,  336,  462, 1367,  315,  336, 1317, 1373,    0,
  390,  376, 1364,  299, 1944, 1367, 1368, 1468, 1376,    0,
   59,    0,  509,  426, 1317,  442,  336,  430,  378,  402,
  458, 1372,    0,  446,  271,   42,   43,  318,   45,  298,
   47,  397,  378,    0,  400,   59,  380,  333,  285,  486,
  331,  264,    0, 1363, 1364,  389,  264, 1367, 1368,    0,
  416, 1371, 1372, 1373, 1374, 1375, 1376,  260,   59,    0,
  380,  485,  338,  354,   44,  334,    0,  262,   59,  389,
   59,  495, 1392, 1393,   41,  259,  442,   44,    0, 1399,
  264,   59,  329,   41,  315,  269,   44,    0,  432, 1914,
 1915, 1916,   59,    0,  325,  371,  327,  300,    0,  336,
  444,   59,  446,  390,    0,  336,  519, 1587,   59,    0,
  357, 1481,  432,   93,  405,   44, 1486,  315,   59,   41,
  315,  397,   44,  318,  400,   59,  446,  333,   41,  327,
  325,   44,  327,  315,  450, 1448,  331,   59,  336,   41,
  416,  336,   44,    0,  339,  327,   59,  260,   41,  380,
   41,   44,   59,   44,  336,  410, 1636,   59,  389,  354,
    0,  477,  333,   59,   93,  402,  442,  390,   59,  513,
  277,  434,  390,  368,  421,  282,  450,  258,  352,    0,
  446,  376,  380,    0,  379,  380,    0,  300, 1566,  270,
  409,  389,  397,  513,  389,  259,  310,  368,  380,    0,
    0,  432,   59,  477, 1524,  269,  390,  389,  315,  380,
  405,  416,  386,   41,  388,  446,   44,  436,  273,   59,
  334, 1524,  303,  315,  305, 1587,  318,   41,   46,  343,
   44,  333,  427,  325,  432,  327,  410,  432,   59,  331,
   58,   59,   59,   61,  336,   59, 1566,  339,  446,  444,
  432,  446,  453,  454, 1574, 1575, 1576,   46,   59,   59,
  277,  435,  354,  338,  446,  282,  368, 1587,  513,   58,
   59,  346,   61,   41,   42,   43,  368,   45,  380,   47,
   42,   43,  513,   45,  376,   47,  452,  401,  380,  258,
 1770,  405,  261,  402, 1402,  404, 1404,  389,  493, 1619,
 1620,   42,   43,  260,   45, 1955,   47, 1957, 1788,  276,
  277,   41,  309,  405,   44,  513, 1636,  284,  513,  336,
   40,   46,  517, 1643, 1644,  416, 1785,   42,   43,  520,
   45,  513,   47,   58,  303,   41,   61,  453,   44,   41,
  432,  298,   44,  264,   42,   43,  333,   45, 1668,   47,
  485,  486,  444,  310,  446,   41,   41,  315,   44,   44,
  318,   41,  384,  385,   44,   42,   43,  325,   45,  327,
   47,    0,  315,  331,  276,  277,  313,  334,  336, 1668,
 1740,  339,  284,  332,   41,  276,  277,   44,   41,   41,
  504,   44,   44,  284,  338,    0,  354,   41,  395, 1818,
   44,  493,  516,  517,  518,  372,  453,  454,   41,  376,
  368,   44,   41,  444,  426,   44,  258, 1737,  376,  403,
 1740,  513,  380, 1619, 1620,  517,  393,  371,  270,  426,
   59,  389,  399,  430, 1737,   41,   41,   41,   44,   44,
   44,    0,  399,   41,  441,  395,   44,  405, 1928, 1769,
 1930,    0,  409,  397,   59, 1935,  400,  336, 1938, 1819,
 1940,  303,   41,  305,   40,   44,  433, 1777, 1788, 1839,
  372, 1791,  416, 1843,  432,  589,  317,  434,  315,  593,
 1960,  372,   41,  315,   41,   44,  444,   44,  446, 1809,
 1909,  393,   41, 1813,  376,   44,  376,  399,  442, 1819,
   59,  315,  393,   41, 1984, 1883,   44, 1987,  399,  277,
   59,  325,  498,  327,  282,  277,   41,  428, 1998,   44,
  282, 2001,  336,  356, 1813,  339,  493,  442, 2008,  426,
    0,  433,  449,  450,  451,  493,  277, 1619, 1620,  315,
  410,  282,  433,  377,  319, 1865,  308,  337,  375,  317,
  783,  497,  785,  409,  508,  513,  789,  790,  791,  517,
  477,  478,  277, 1883, 1884,  297,  380,  282,  306,  509,
  318,   41, 1885, 1935,   44,  389,  317,  508,  461,  277,
   44,  446, 1944,  331,  282,    0,   61,  366,  265,   59,
    0,  493,  283,  308,  264,   41,   42,   43, 1960,   45,
  277,   47,  493,   40,   40,  282,  354,   41,   42,   43,
   40,   45,  389,   47,   40, 1935,   40, 1927,  432,  317,
  368, 1941,  299,  453, 1944, 1987,   41,   40,  376,   44,
  444,   41,  446,   40,   44, 1955, 1998, 1957,   40, 2001,
 1960,    0,   40,   40,   59,   40, 2008,  276,  277,   59,
  764,  453,   41,   42,   43,  284,   45,  405,   47,   41,
   42,   43,  453,   45,   40,   47,   40, 1987, 1988,   40,
  784,  276,  277,   40,  788,    0,  281,   40, 1998,  284,
 2000, 2001,   41,  351,  798,   44,   40,   40, 2008,  803,
  804,   40,  297,  807,  808,  809,  810,  811,  812,  513,
   59,   40,   40,  262,   40,   40,   91,  821,  402,  823,
  824,  825,  826,  827,  828,   44,   41,  831,   46,   44,
   61,   40,  264,  395,  317,  420,   44,  276,  277,   44,
   46,   61,  281,   40,   59,  284,  264,  337,    0,   44,
   41,   42,   43,  372,   45,  493,   47,   46,  297,  344,
  344,  365,  344,  867,  276,  264,  315,   42,   43,  318,
   45,  428,   47,  390,  393,  390,  325,  372,  327,  517,
  399,  450,  331,  887,  275,  344,  439,  336,  439,   41,
  339,    0,   44,  392,   40,  345,  900,  901,  393,   40,
   40,   91,   40,  264,  399,  354,  315,   59,  376,   44,
  403,  382,  375,   40,  433,   40,  276,  277,  380,  368,
   42,   43,  443,   45,  284,   47,  315,  376,   41,  275,
  379,  380,   41,  372,   63,   44,  428,  297,  433,  296,
  389,  277,   42,   43,   44,   45,  282,   47,  428,  456,
   59,  314,   61,  277,  393,   40,  405,   44,  282,  440,
  399,  337,  264,   41,   42,   43,  444,   45,   44,   47,
  494,  276,  277,   40,  493,  273,  276,  277,  427,  284,
   40,  340,  281,  432,  284,  340,  297,   41,   42,   43,
  410,   45,  297,   47,  433,  444,  379,  446,  277,   42,
   43,   44,   45,  282,   47,  277,  379,  379,  408,  410,
  282,   40,  372,  352, 1018, 1019, 1020,   40,   40, 1023,
 1024, 1025,  362, 1027, 1028, 1029, 1030, 1031, 1032,   40,
   44,   41,   41,  393,  317,  277,   44, 1041, 1042,  399,
 1044,   46, 1046,  438,  493, 1049, 1050, 1051, 1052, 1053,
 1054, 1023,  361, 1025,  270, 1027, 1028,    0, 1030, 1031,
 1032,  276,  277, 1067,  513, 1069,  315,  372,  517,  318,
  378,  317,  372,  433,   40,   44,  325,  317,  327,  266,
  325, 1085,  331,  493,  264,  414,  277,  336,  393,  264,
  339,  282,   41,  393,  399,   58,   41,   41,   41,  399,
   41,   44,  277,    0,  264,  354,  387,  282,   41,   42,
   43,  446,   45,  306,   47,   41,   59,   41,  306,  368,
   40,   40, 1126,  317,  276,  277,   40,  376,  433,   40,
   40,  380,  284,  433,  262,   41,   42,   43,   40,   45,
  389,   47,  264,  427,   41,   42,   43,   44,   45,  258,
   47,   40,   40,   40,   40,  277,  405,  372,   40,   40,
  282,   40,   59,   60,   61,   62,  275,  276,  277,   40,
   40,   40,  281,  305,   41,  284,  446,  277,  393,   44,
   44,   41,  282,  432,  399,  264,  339,  273,  297,  381,
  339,  327,  284,   41,  303,  444,   93,  446,  384,  277,
  399,  379,  379,  452,  282,  379,  455,  379,  457,  458,
  459,  460,  379,  462,   42,   43,   44,   45,  433,   47,
  372,  470,  471,  277,  379,   41,   42,   43,  282,   45,
   41,   47,  481,  482,  277,  484,  450,  276,   40,  282,
   41,  393,  491,  492,  493,  494,  495,  399,  497,   41,
   41,  500,  501,  502,  503,  504,  317,   41,   44,  508,
  509,   41,  427,  372,  513,  514,  515,  516,  517,  518,
  519,  520,  305,  522,   59, 1279,   41,   42,   43,   40,
   45,  433,   47,  532,  393,  336,    0,  290, 1292, 1293,
  399, 1295, 1296,  305,  336,   59,   41,   42,   43, 1303,
   45,  410,   47,   58,  324,  428,  277,   41,   42,   43,
  532,   45,  339,   47,  333,  434,   41,   42,   43,  428,
   45, 1325,   47,  434,  433,  298,  298,   41,  334, 1333,
   44, 1335, 1336,  276,  277, 1339,  334, 1341,   41,   41,
  378, 1345, 1346, 1347,  277,   59,  348,   61,  365,  282,
 1354,   41, 1356,  462,   41,   41,    0,   41,  365,   41,
   41,  258,   41,   41,   41,  262,   41,  264,  265,   41,
   41,  277,   41,   41,  271,   41,  282,   41,   40,  276,
  277,   41,   40,  315,  443,  282,  340,  284,  285,  378,
   45,   44,   41, 1397,  458,  333,  336,   41,   40,  500,
   44,  336,  299,   40,   40,   44,  303,  304,  305,  376,
   41,  308,   40,  298,  427,   59,  376,  376,  315,   41,
  317,  318,   41,  301,  321, 1429,   41,  304,  325,  372,
  327,  290,  329,  434,  331,  301,    0,  455,  333,  336,
  337,  409,  339,   40,  264,  376,  376,  264,  403,  277,
  393,  450,   40,  443,  282,  317,  399,  354,  355,   44,
  357,  277,  270,   41,   42,   43,  282,   45,   40,   47,
   40,  368,   41,  370,  270,  372,  373,  374,  339,  376,
  450,  264,  379,  380,  339,  427,  450,   41,   41, 1493,
  433,   59,  389,   41,   59, 1499,  393,   41,   42,   43,
 1504,   45,  399,   47,   59,  402,  305, 1511,  405,    0,
   59,   41,  277,  410,   59,   41,   59,  282,   59,   41,
   42,   43,   59,   45,  421,   47,   59,  301,    0,    0,
  427,  428,  277,    0,  161,  432,  433,  282,    0,  584,
 1383, 1545,  857,  277,  258, 1549,  443,  444,  282,  446,
   41, 1555,  277,   44,  161,  452,  162,  282,  455,  273,
  457,  458,  459,  460,  322,  462,  848, 1571,   59,  321,
   61,  586, 1254,  470,  471,   41,   42,   43,  748,   45,
 1252,   47,  998,  297,  481,  482,    0,  484,   95,  303,
 1003,  960,  306,   81,  491,  492,  493,  494,  495, 1249,
  497, 1244,  973,  500,  501,  502,  503,  504, 1684, 1667,
 1685,  508,  509,  447,  695, 1234,  513,  514,  515,  516,
  517,  518,  519,  520,  371,  522,   40,   41,   42,   43,
   44,   45,   46,   47,  348,  532, 1175,  292,  889, 1245,
  428, 1645, 1809, 1665,   58,   59,   60,   61,   62,  428,
  924,   41,   42,   43, 1947,   45,  672,   47,   42,   43,
 1951,   45, 1950,   47, 1973,  320, 1574,  915, 1774, 1115,
 1936,  315, 1416,  837,  318,  418,  433,   91,  674,   93,
  411,  325,  976,  327,  732,   13, 1457,  331,  968,  403,
  163,  356,  336,  348,  602,  339,  410,   40,  167,  277,
 1134,  606,  878, 1403,  282,  867,   91, 1903,  781, 1524,
  354, 1737, 1716,  617,  428,   58,  900, 1294, 1722, 1723,
 1127,  855, 1178,  496,  368, 1341,   -1,   -1,   -1,   -1,
   -1,   -1,  376,  277,   -1,  379,  380,   -1,  282,   -1,
   -1,   -1, 1746,   -1,  458,  389, 1750,   -1,  462,   -1,
 1754,   -1,   -1,   -1,   -1,  277,   -1, 1761, 1762,   -1,
  282,  405,   -1,   -1,  419,   -1,   -1,  258,   41,   42,
   43,   -1,   45,   -1,   47,   41,   42,   43,   -1,   45,
  435,   47, 1786,  427,   -1,  276,  277,   -1,  432,   -1,
  281,   -1,   -1,  284,  449,  450,  451,   -1,  453,  454,
  444,   -1,  446,   -1,   -1,   -1,  297,   -1,   -1,   -1,
   -1,  277,  303,   -1,   -1,   -1,  282,  472,  473,  474,
  475,  476,  477,  478,  479,  480,   42,   43,   -1,   45,
   -1,   47,  487,  488, 1838,   -1,   -1,   -1,   -1,   -1,
   -1,  496,   -1,   -1,   -1,   -1,   -1,   -1,  262,  493,
  264,  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,
   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,  513,
   -1,  285,   -1,  517,   -1,   -1,   -1,   -1,  262,   -1,
   -1,  372,   -1,   -1,   -1,  299,   -1,  277,   -1,   -1,
  304,  305,  282,  277,  308,   -1,   -1,   -1,  282, 1903,
   -1,  315,  393,  317,  318,   -1,   -1,  321,  399,   -1,
   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,  410,
   -1,   -1,  336,  337,   -1,  339,    0,   -1,   41,   42,
   43,   -1,   45, 1937,   47,    0,   -1,  428,   -1,   -1,
  354,  355,  433,  357,   41,   42,   43,   -1,   45,   -1,
   47,   41,   42,   43,  368,   45,  370,   47,  372,  373,
  374,   -1,  376,   -1,   -1,  379,  380,   41,   -1,   -1,
   44,  462,   -1,   -1,   -1,  389,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   61,  402,   -1,
   -1,  405,   -1,   -1,   59,   60,   61,   62,   42,   43,
   -1,   45,   -1,   47,  277,   -1,   -1,  421,   -1,  282,
   -1,  277,   -1,  427,  428,   -1,  282,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   93,  443,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,
  514,  515,  516,  517,  518,  519,  520,   -1,  522,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,  532,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,  277,  508,  509,   -1,   -1,  282,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
  277,   -1,   -1,   -1,  258,  282,   -1,  277,   -1,  532,
   -1,   -1,  282,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,  262,   -1,
  285,   -1,   -1,  297,   41,   42,   43,   -1,   45,  303,
   47,   -1,   -1,  277,  299,    0,  301,   -1,  282,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,  291,   93,   -1,
   -1,   -1,   -1,   -1,  408,   -1,  410,  402,  302,   -1,
  405,   -1,   -1,   -1,   -1,  309,  464,  465,  466,  467,
  468,  469,  470,  471,  428,   -1,  421,   -1,  322,  433,
   -1,   -1,  427,  428,   -1,  277,   -1,  432,   -1,   -1,
  282,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  379,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,  395,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,  411,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
  277,   -1,  426,   -1,   -1,  282,  430,  532,   -1,  433,
   -1,   -1,   -1,   -1,  302,   -1,   -1,  441,   -1,   -1,
   -1,  309,   -1,   -1,   -1,  313,  314,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  322,   -1,  460,  262,   -1,  264,
  265,   -1, 1494, 1495,   -1,  333,  271,   -1,   -1,   -1,
 1502, 1503,  277, 1505, 1506,   -1,   -1,  282,   -1,   -1,
  285, 1513, 1514, 1515, 1516,   -1, 1518, 1519, 1520, 1521,
 1522, 1523,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,  519,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,  395,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,  411,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  426,   -1,
   -1,   -1,  430,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,  441,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,
   -1,   -1,  460,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,  519,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
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
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,  398,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
  437,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
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
  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,    0,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   59,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,  272,   -1,  325,  275,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,   -1,  290,   -1,  292,   -1,   -1,  295,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,  335,   -1,
   -1,  389,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,  402,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  367,   -1,  369,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   40,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,  398,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,  415,   -1,
   -1,   -1,  470,  471,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
  437,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
    0,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,  472,  473,  315,   -1,   -1,
  318,   -1,   -1,   -1,  532,   -1,   -1,  325,   -1,  327,
   -1,   41,  489,  331,   44,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,  262,   -1,  264,  265,   -1,   -1,   59,
   -1,   -1,  271,   -1,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  299,  379,  380,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,  389,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,  405,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,  354,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,    0,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,  513,   -1,   -1,   -1,  517,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   40,   41,   -1,   -1,   44,
   -1,  470,  471,   -1,  264,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   59,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,  409,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,  532,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,  354,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,  481,  482,  376,  484,   -1,   -1,
  380,   -1,   -1,   -1,  491,  492,   -1,  494,  495,  389,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,  405,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,  481,  482,   44,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   59,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  532,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,   -1,   -1,  380,   41,   42,   43,   44,
   45,   46,   47,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   91,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  262,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   41,
   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  262,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,  389,   91,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
  315,  432,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,  444,  329,  446,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,  493,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   91,   -1,   -1,   -1,   -1,
   -1,    0,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,   -1,    0,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,  261,  444,
  262,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  271,
   59,   -1,   -1,   -1,   -1,  277,  279,   -1,   -1,   -1,
  282,   -1,   -1,  285,  287,   41,   -1,   -1,   44,   -1,
   -1,  294,   -1,  296,   -1,  298,   -1,   -1,   -1,   -1,
  303,   -1,   -1,   59,   -1,   -1,   -1,  310,  493,   -1,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  321,
   -1,  324,   -1,  325,   -1,  327,   -1,  329,  513,  331,
   -1,  334,  517,   -1,  336,  337,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,  262,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,  271,   -1,  389,   -1,   -1,
   -1,  277,   -1,   -1,  397,   -1,  282,   -1,   -1,  285,
   -1,  404,   -1,  405,  407,   41,  409,  410,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,
   -1,   -1,   -1,   59,   -1,  427,   -1,   -1,   -1,  315,
  432,  434,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,  444,  329,  446,  331,   -1,   -1,    0,   -1,
  336,  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
  483,   -1,  368,   -1,  370,   -1,  372,  373,  374,   41,
  376,  493,   44,  379,  380,  498,  499,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,  262,   59,    0,   -1,
   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,  405,
  523,   -1,   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   -1,  421,  325,   -1,  327,   -1,
   -1,  427,  331,   -1,   -1,   -1,  432,  336,   -1,   41,
  339,   -1,   44,   -1,   -1,   -1,   -1,   -1,  444,  315,
  446,   -1,  318,   -1,   -1,  354,   -1,   59,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,  368,
  336,   -1,   -1,  339,   -1,   -1,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,
  389,   -1,   -1,    0,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,  368,   -1,   -1,   -1,  405,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,  262,  513,   -1,   -1,
   -1,  517,   -1,  389,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,   41,   -1,   -1,   44,   -1,  405,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,    0,  432,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,  444,  325,
  446,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,   -1,   -1,  339,  493,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   41,   -1,  354,   44,
   -1,   -1,   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,
   -1,   -1,  368,   -1,   59,   -1,   -1,  493,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,    0,   -1,   -1,  513,   -1,   -1,
   -1,  517,   -1,  315,   -1,   -1,  318,   -1,   -1,  405,
  262,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   41,  432,   -1,   44,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,   -1,   -1,   59,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,  315,  376,   -1,  318,  379,  380,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,  389,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,  354,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,  368,  513,   -1,   -1,
  432,  517,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,  444,   -1,  446,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,  427,   -1,  262,   -1,  336,
  432,  493,  339,   -1,   -1,   41,   -1,   -1,   44,   -1,
    0,   -1,  444,   -1,  446,   -1,   -1,  354,   -1,   -1,
   -1,  513,   -1,   59,   -1,  517,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   41,  389,  318,   44,   -1,   -1,   -1,   -1,   -1,
  325,  493,  327,   -1,   -1,   -1,  331,   -1,  405,   59,
   -1,  336,   -1,   -1,  339,   -1,  262,   -1,   -1,   -1,
   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,  354,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,   -1,  444,   -1,  446,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,  315,
   -1,   -1,  318,    0,   -1,   -1,   -1,   -1,   -1,  325,
  405,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,  493,   -1,   -1,   -1,
   -1,    0,  427,   -1,   -1,   -1,   -1,  432,  354,   -1,
   -1,   -1,   -1,   -1,   41,   -1,  513,   44,   -1,  444,
  517,  446,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,   59,  379,  380,   -1,  275,   -1,   -1,   -1,
   -1,   -1,   41,  389,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,  291,   -1,   -1,   -1,   -1,   -1,   -1,  405,
   59,   -1,   -1,  302,   -1,   -1,  262,   -1,  493,   -1,
  309,   -1,   -1,   -1,  313,  314,   -1,   -1,   -1,   -1,
   -1,  427,   -1,  322,   -1,   -1,  432,   -1,  513,   -1,
   -1,   -1,  517,   -1,  333,    0,   -1,   -1,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   41,   -1,   -1,   44,
  336,   -1,   -1,  339,   -1,   -1,   -1,  493,  387,   -1,
   -1,   -1,   -1,   -1,   59,   -1,  395,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  513,  318,   -1,
   -1,  517,  368,   -1,   -1,  325,   -1,  327,   -1,   -1,
  376,  331,   -1,  379,  380,   -1,  336,  426,   -1,  339,
   -1,  430,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  441,   -1,  354,   -1,   -1,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,  368,   -1,
   -1,  460,  461,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,  427,   -1,   -1,   -1,  262,  432,   -1,   -1,  389,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  444,   -1,
  446,   -1,   -1,   -1,   -1,  405,   41,   -1,   -1,   44,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,  427,   -1,   -1,
  519,   -1,  432,  522,   41,   -1,   -1,   44,  315,   -1,
   -1,  318,   -1,   -1,  444,   -1,  446,  493,  325,   -1,
  327,   -1,   59,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,  315,  513,   -1,  318,
   -1,  517,   -1,   -1,   -1,   -1,  325,  354,  327,   -1,
   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,    0,   -1,
  339,  368,   -1,  493,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,  354,   -1,  262,   -1,   -1,
   -1,   -1,  389,  513,   -1,   -1,   -1,  517,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  405,   41,
  379,  380,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   59,    0,   -1,
  427,   -1,   -1,   -1,   -1,  432,  405,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,  444,   -1,  446,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  427,   -1,
   -1,  336,    0,  432,  339,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,  444,   -1,  446,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,  493,   -1,   -1,   -1,
   -1,  376,   -1,   41,  379,  380,   44,  262,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,  513,   -1,   -1,   -1,
  517,   59,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  405,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,
   -1,    0,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,  444,
  325,  446,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,  315,   -1,
   -1,  318,   41,   -1,   -1,   44,   -1,   -1,  325,  354,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   59,   -1,  339,  368,   -1,   -1,   -1,   -1,  493,   -1,
   -1,  376,   -1,   -1,  379,  380,   -1,  354,   -1,   -1,
  262,   -1,   -1,   -1,  389,   -1,   -1,   -1,  513,   -1,
   -1,  368,  517,   -1,   -1,   -1,   -1,   -1,   -1,  376,
  405,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,  405,   -1,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  444,
  262,  446,   -1,  325,   -1,  327,   -1,   -1,   40,  331,
  427,   43,   -1,   45,  336,  432,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  444,   -1,  446,
   -1,   63,  354,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  493,   -1,
   -1,   -1,   -1,  315,  376,   -1,  318,  379,  380,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,  389,  513,  331,
   -1,   -1,  517,   -1,  336,   -1,  493,  339,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,  354,   -1,   -1,   -1,  513,  325,   -1,  327,
  517,   -1,   -1,  331,   -1,  427,  368,   -1,  336,   -1,
  432,  339,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,  444,   -1,  446,   -1,  354,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
  368,   -1,   -1,  405,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  432,  493,   -1,   -1,   -1,   -1,  315,  405,   -1,  318,
   -1,   -1,  444,   -1,  446,   -1,  325,   -1,  327,   -1,
   -1,  513,  331,   -1,   -1,  517,   -1,  336,   -1,  427,
  339,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  493,   -1,   -1,   -1,  267,   -1,  376,   -1,   -1,
   -1,  380,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,  513,   -1,   -1,  286,  517,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   40,  493,  405,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   -1,   -1,   58,   -1,   -1,  513,   -1,   63,  320,  517,
   -1,  323,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  513,   -1,   -1,  409,  517,  281,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   40,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  505,  506,  507,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  267,  524,  525,  526,   -1,  528,  529,  274,  531,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,  289,   -1,   -1,  292,  293,   -1,   -1,
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
   -1,   -1,   42,   -1,   -1,   -1,   -1,   -1,   -1,  435,
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
   -1,  311,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
  320,   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,  372,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,  293,   -1,  425,   -1,   -1,   -1,   42,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
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
   -1,  435,   -1,   -1,   -1,   -1,   -1,   45,   -1,   -1,
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
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   58,   -1,   -1,  452,
   -1,   63,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,  360,   -1,   -1,   -1,  364,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   44,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  521,  522,   -1,  524,  525,  526,  527,
  528,  529,  530,  531,  532,  267,   -1,   41,   42,   43,
   -1,   45,  274,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   60,   61,   62,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,  311,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,  450,   58,  452,   -1,   -1,  455,   63,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
  372,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   58,   -1,  317,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,  271,  500,  501,
  502,  503,  504,  277,   -1,   -1,  508,  509,  282,   -1,
   -1,  285,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,  524,  525,  526,  260,  528,  529,  263,  531,
  532,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  321,   -1,   -1,
   -1,  286,   -1,   -1,   -1,  329,   -1,  292,  293,   -1,
   -1,   -1,   -1,  337,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,  355,   -1,  357,   -1,  320,   -1,   -1,  323,   -1,
   58,   -1,   -1,   -1,   -1,   63,  370,   -1,  372,  373,
  374,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,  348,   -1,  350,  351,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,  421,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  532,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   42,   43,   -1,   45,  274,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   60,   61,   62,   -1,  292,  293,   -1,   -1,   -1,   -1,
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
   -1,  271,  500,  501,  502,  503,  504,  277,   -1,   -1,
  508,  509,  282,   -1,   -1,  285,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,  524,  525,  526,  260,
  528,  529,  263,  531,  532,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  321,   -1,   -1,   -1,  286,   -1,   -1,   -1,  329,
   -1,  292,  293,   -1,   -1,   -1,   -1,  337,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,  355,   -1,  357,  435,  320,
   -1,   -1,  323,   -1,   58,   -1,   -1,   -1,   -1,   63,
  370,   -1,  372,  373,  374,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,  348,   -1,  350,
  351,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,  421,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,  417,   -1,  419,   -1,
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
  531,  532,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  288,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   58,   -1,   -1,   -1,   -1,   63,  335,   -1,   -1,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,
  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  398,   -1,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,  472,  473,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
  524,  525,  526,  260,  528,  529,  263,  531,  532,   -1,
  267,  272,   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,  292,   -1,   -1,  295,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   58,   -1,
   -1,   -1,   -1,   63,  335,   -1,   -1,   -1,   -1,   -1,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,  367,  364,  369,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,  472,  473,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,  489,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,  524,  525,  526,
  260,  528,  529,  263,  531,  532,   -1,  267,  272,   -1,
   -1,  275,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,  292,   -1,
   -1,  295,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   58,   -1,   -1,   -1,   -1,
   63,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,
   -1,  345,  346,  347,   -1,  349,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,  367,  364,  369,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  415,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  437,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,  325,   -1,  472,  473,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,  489,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,  524,  525,  526,  260,  528,  529,
  263,  531,  532,   -1,  267,   -1,  395,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  430,   -1,   -1,   40,   -1,  435,   43,   -1,   45,
   -1,   -1,  441,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   58,  452,   -1,   -1,  455,   63,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,  284,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,  524,  525,  526,  260,  528,  529,  263,  531,  532,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   58,   -1,  323,   -1,   -1,
   63,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,  360,   -1,   -1,   -1,  364,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,  284,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,  260,  524,  525,
  526,   -1,  528,  529,  267,  531,  532,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,
   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   58,   -1,
  323,   -1,   -1,   63,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,  360,   -1,   -1,
   -1,  364,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
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
  260,  524,  525,  526,   -1,  528,  529,  267,  531,  532,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   40,   -1,   42,   43,
  300,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,
  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   63,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
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
  292,  293,   -1,   -1,   40,   41,   -1,   43,   -1,   45,
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
   -1,   -1,   -1,   -1,  532,   -1,   -1,  409,   -1,   -1,
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
   58,   -1,   -1,  317,   -1,   63,  320,   -1,   -1,  323,
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
   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
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
  520,    0,  522,  267,  524,  525,  526,   -1,  528,  529,
  274,  531,  532,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   40,   41,   -1,  320,   44,   -1,  323,
   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   41,   91,   -1,   44,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,    0,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,  258,
  524,  525,  526,   41,  528,  529,   44,  531,  532,   -1,
   -1,  258,   -1,   -1,  273,   -1,  275,  276,  277,   -1,
    0,   59,  281,   61,   -1,  284,  273,   -1,  275,  276,
  277,   -1,   -1,   -1,  281,   -1,   -1,  284,  297,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,  258,   -1,
  297,   -1,   -1,   91,   -1,   -1,  303,   -1,   -1,  306,
   -1,   41,   -1,  273,   44,  275,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,  297,   -1,  348,
   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,
   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,  393,   -1,   -1,   -1,  348,   -1,
  399,   -1,   -1,   -1,  403,   -1,  393,   -1,   -1,   -1,
   -1,  410,  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,
   -1,   -1,  372,  410,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   41,   -1,  433,   44,   -1,   -1,   -1,   -1,
   -1,  428,   -1,  393,   -1,   -1,  433,   -1,   -1,  399,
   59,   -1,   61,  403,   -1,   -1,   -1,   -1,   -1,  458,
  410,   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,   -1,   -1,   -1,  462,   -1,   -1,  428,   -1,
  258,   -1,   91,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,  458,   -1,
   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,  297,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,
   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,
   -1,  399,   -1,   -1,   -1,  403,   -1,   -1,  348,   -1,
   -1,   -1,  410,   -1,   -1,   -1,  260,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,
  428,   -1,  372,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,
   -1,   -1,  281,  393,  298,  284,   -1,   -1,   -1,  399,
  458,   -1,   -1,  403,  462,   -1,  310,   -1,  297,   -1,
  410,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
  334,   -1,  260,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  348,
   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  310,  372,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  399,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  393,  409,  334,   -1,   -1,   -1,
  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  458,
   -1,  399,   -1,  462,   -1,   -1,   -1,  481,  482,   -1,
  484,  409,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,  434,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
  270,   -1,   -1,   -1,  452,   -1,   -1,  455,  532,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,  298,   -1,
   -1,   -1,   -1,  481,  482,  305,  484,   -1,   -1,  309,
  310,   -1,  312,  491,  492,  315,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,  328,   -1,
  508,  509,   -1,   -1,  334,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  270,   -1,   -1,   -1,
   -1,  391,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,
   -1,   -1,   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  424,  309,  310,   -1,  312,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  328,  445,   -1,   -1,   -1,   -1,
  334,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,  391,  508,  509,
  510,  511,  512,   -1,  514,  515,  516,   -1,  518,  519,
  520,  270,  522,   -1,   -1,  409,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  424,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
  309,  310,   -1,  312,   -1,   -1,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,  328,
   -1,   -1,   -1,   -1,   -1,  334,  470,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,  391,   -1,   -1,   -1,   -1,   -1,  532,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,  272,   -1,   -1,  275,
   -1,  470,  471,   -1,   -1,  281,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,  292,   -1,   -1,  295,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,  335,
   -1,   -1,   -1,  532,   -1,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  367,   -1,  369,  272,   -1,   -1,  275,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,
   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
   -1,  437,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,  367,
   -1,  369,   -1,   -1,  470,  471,  472,  473,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,  489,   -1,  491,  492,   -1,  494,  495,
  398,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  339,   -1,  508,  509,   -1,   -1,   -1,  415,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,  362,   -1,   -1,  532,   -1,   -1,  437,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,  418,
   -1,  489,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  452,  522,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,  532,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  532,  455,  456,  457,  458,  459,  460,
  461,  462,  463,  464,  465,  466,  467,  468,  469,  470,
  471,  472,  473,  474,  475,  476,   -1,   -1,  479,  480,
  481,  482,  483,  484,   -1,   -1,  487,  488,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  498,  499,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,  517,  518,  519,  520,
  521,  522,  523,  524,  525,  526,  527,  528,  529,  530,
  531,  532,  452,   -1,   -1,  455,  456,  457,  458,  459,
  460,   -1,  462,  463,  464,  465,  466,  467,  468,  469,
  470,  471,  472,  473,  474,  475,  476,   -1,   -1,  479,
  480,  481,  482,  483,  484,   -1,   -1,  487,  488,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  498,  499,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  510,  511,  512,  513,  514,  515,  516,  517,  518,  519,
  520,  521,  522,  523,  524,  525,  526,  527,  528,  529,
  530,  531,  532,
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
"create_clause : TRIGGER trigger_clause",
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
"recreate_clause : TRIGGER rtrigger_clause",
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
"domain_default : DEFAULT begin_trigger default_value",
"domain_default_opt : domain_default",
"domain_default_opt :",
"domain_constraint_clause : domain_constraint_list",
"domain_constraint_clause :",
"domain_constraint_list : domain_constraint_def",
"domain_constraint_list : domain_constraint_list domain_constraint_def",
"domain_constraint_def : domain_constraint",
"domain_constraint : null_constraint",
"domain_constraint : check_constraint",
"null_constraint : NOT KW_NULL",
"check_constraint : CHECK begin_trigger '(' search_condition ')' end_trigger",
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
"column_constraint : null_constraint",
"column_constraint : check_constraint",
"column_constraint : REFERENCES simple_table_name column_parens_opt referential_trigger_action constraint_index_opt",
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
"input_proc_parameter : simple_column_def_name non_array_type begin_trigger default_par_opt end_trigger",
"output_proc_parameters : proc_parameter",
"output_proc_parameters : output_proc_parameters ',' proc_parameter",
"proc_parameter : simple_column_def_name non_array_type",
"default_par_opt : DEFAULT begin_trigger default_value",
"default_par_opt : '=' begin_trigger default_value",
"default_par_opt :",
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
"var_init_opt : DEFAULT default_value",
"var_init_opt : '=' default_value",
"var_init_opt :",
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
"view_clause : symbol_view_name column_parens_opt AS begin_string select_expr check_opt end_trigger",
"rview_clause : symbol_view_name column_parens_opt AS begin_string select_expr check_opt end_trigger",
"begin_string :",
"begin_trigger :",
"end_trigger :",
"check_opt : WITH CHECK OPTION",
"check_opt :",
"trigger_clause : symbol_trigger_name FOR simple_table_name trigger_active trigger_type trigger_position trigger_action end_trigger",
"rtrigger_clause : symbol_trigger_name FOR simple_table_name trigger_active trigger_type trigger_position trigger_action end_trigger",
"replace_trigger_clause : symbol_trigger_name FOR simple_table_name trigger_active trigger_type trigger_position trigger_action end_trigger",
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
"alter_domain_ops : alter_domain_op",
"alter_domain_ops : alter_domain_ops alter_domain_op",
"alter_domain_op : SET domain_default end_trigger",
"alter_domain_op : ADD CONSTRAINT check_constraint",
"alter_domain_op : ADD check_constraint",
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
"alter_op : col_opt alter_col_name SET domain_default end_trigger",
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
"alter_data_type_or_domain : non_array_type",
"alter_data_type_or_domain : simple_column_name",
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
{ yyval = yyvsp[0]; }
break;
case 126:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 127:
{ yyval = make_node (nod_redef_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 128:
{ yyval = make_node (nod_replace_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 129:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 130:
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 131:
{ yyval = NULL; }
break;
case 132:
{ yyval = make_list (yyvsp[0]); }
break;
case 134:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 135:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 136:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 137:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 138:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 139:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 140:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 141:
{ yyval = (dsql_nod*) 0;}
break;
case 142:
{ yyval = yyvsp[-1]; }
break;
case 143:
{ yyval = NULL; }
break;
case 146:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 147:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 148:
{ yyval = NULL; }
break;
case 149:
{ yyval = NULL; }
break;
case 150:
{ yyval = yyvsp[0]; }
break;
case 152:
{ yyval = NULL; }
break;
case 154:
{ yyval = NULL; }
break;
case 156:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 157:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 160:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 161:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count,
					NULL, NULL, yyvsp[-2], NULL, yyvsp[0]); }
break;
case 162:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
break;
case 163:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
break;
case 164:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 167:
{ yyval = (dsql_nod*) yyvsp[0]; }
break;
case 168:
{yyval = NULL;}
break;
case 171:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 172:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 173:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 174:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 175:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 176:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 177:
{yyval = NULL;}
break;
case 180:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 182:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 183:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 184:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 185:
{ lex.g_file  = make_file();}
break;
case 190:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 191:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 198:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 199:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 200:
{ yyval = yyvsp[0]; }
break;
case 201:
{ yyval = yyvsp[0]; }
break;
case 202:
{ yyval = NULL; }
break;
case 204:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 207:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 208:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 209:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 210:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 213:
{ yyval = NULL; }
break;
case 214:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 215:
{ yyval = yyvsp[0]; }
break;
case 216:
{ yyval = NULL; }
break;
case 217:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 218:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 219:
{ yyval = yyvsp[-1]; }
break;
case 220:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
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
case 234:
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
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 250:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 251:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 252:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 253:
{ yyval = NULL;}
break;
case 254:
{ yyval = yyvsp[0];}
break;
case 255:
{ yyval = yyvsp[0];}
break;
case 256:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 257:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 258:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 259:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 260:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 261:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 262:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 263:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 264:
{ yyval = make_list (yyvsp[-1]); }
break;
case 265:
{ yyval = NULL; }
break;
case 266:
{ yyval = make_list (yyvsp[-1]); }
break;
case 267:
{ yyval = NULL; }
break;
case 269:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 270:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-4], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 272:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 273:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 274:
{ yyval = yyvsp[0]; }
break;
case 275:
{ yyval = yyvsp[0]; }
break;
case 276:
{ yyval = NULL; }
break;
case 277:
{ yyval = make_list (yyvsp[0]); }
break;
case 278:
{ yyval = NULL; }
break;
case 280:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 281:
{ yyval = yyvsp[-1]; }
break;
case 284:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 285:
{ yyval = NULL; }
break;
case 286:
{ yyval = NULL; }
break;
case 287:
{ yyval = yyvsp[0]; }
break;
case 288:
{ yyval = yyvsp[0]; }
break;
case 289:
{ yyval = NULL; }
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
{ lex.beginning = lex.last_token; }
break;
case 379:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 380:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 381:
{ yyval = 0; }
break;
case 382:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 383:
{ yyval = make_node (nod_redef_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 384:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
case 420:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 421:
{ yyval = make_node (nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0]); }
break;
case 422:
{ yyval = yyvsp[0]; }
break;
case 423:
{ yyval = yyvsp[0]; }
break;
case 424:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 425:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 426:
{ yyval = yyvsp[0]; }
break;
case 427:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 429:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 430:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 431:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 432:
{ yyval = yyvsp[0]; }
break;
case 433:
{ yyval = yyvsp[0]; }
break;
case 434:
{ yyval = make_node(nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 435:
{ yyval = make_node(nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 436:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], yyvsp[0], NULL); }
break;
case 437:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-3], NULL,
					make_node(nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0])); }
break;
case 438:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], NULL,
					make_node(nod_del_default, (int) 0, NULL)); }
break;
case 439:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 481:
{ yyval = NULL; }
break;
case 482:
{ yyval = NULL; }
break;
case 483:
{ yyval = NULL; }
break;
case 484:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
					yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 485:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 486:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 487:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 488:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 489:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 490:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 491:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 492:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 493:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 494:
{ yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 495:
{ yyval = yyvsp[0]; }
break;
case 496:
{ yyval = NULL; }
break;
case 497:
{ yyval = yyvsp[0]; }
break;
case 498:
{ yyval = NULL; }
break;
case 499:
{ yyval = NULL; }
break;
case 501:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 502:
{ yyval = yyvsp[0]; }
break;
case 503:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 504:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 505:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 506:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 507:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 509:
{ yyval = NULL; }
break;
case 511:
{ yyval = NULL; }
break;
case 512:
{ yyval = yyvsp[0]; }
break;
case 513:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 514:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 515:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 516:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 517:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 518:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 519:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 520:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 521:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 522:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 523:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 524:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 525:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 530:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 531:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 533:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 534:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 535:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 541:
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
case 542:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 544:
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
case 545:
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
case 546:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 549:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
break;
case 550:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 553:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 554:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 555:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 556:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 557:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 558:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 560:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 561:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 562:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 563:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 564:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 565:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 574:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 575:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 576:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 577:
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
case 578:
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
case 581:
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
case 582:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 583:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 584:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 585:
{ yyval = yyvsp[-1]; }
break;
case 586:
{ yyval = 0; }
break;
case 590:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 591:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 592:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 596:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 597:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 598:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 599:
{ yyval = 0; }
break;
case 600:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 603:
{ yyval = make_node (nod_commit, e_commit_count, yyvsp[0]); }
break;
case 604:
{ yyval = make_node (nod_rollback, e_rollback_count, yyvsp[0]); }
break;
case 607:
{ yyval = make_node (nod_retain, 0, NULL); }
break;
case 608:
{ yyval = NULL; }
break;
case 610:
{ yyval = NULL; }
break;
case 611:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 613:
{ yyval = NULL; }
break;
case 615:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 620:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 621:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 622:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 623:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 624:
{ yyval = yyvsp[0];}
break;
case 626:
{ yyval = yyvsp[0];}
break;
case 627:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 628:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 629:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 630:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 631:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 632:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 633:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 634:
{ yyval = 0; }
break;
case 635:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 636:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 637:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 638:
{ yyval = (dsql_nod*) 0; }
break;
case 639:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 640:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 642:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 643:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 644:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 645:
{ yyval = 0; }
break;
case 647:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 648:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
break;
case 649:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
break;
case 650:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
break;
case 651:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
break;
case 652:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
break;
case 653:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
break;
case 654:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
break;
case 655:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
break;
case 656:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
break;
case 657:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
break;
case 658:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
break;
case 659:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
break;
case 660:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
break;
case 661:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
break;
case 662:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
break;
case 663:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
break;
case 664:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
break;
case 665:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
break;
case 666:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
break;
case 667:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
break;
case 668:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
break;
case 669:
{ yyval = yyvsp[0]; }
break;
case 671:
{ yyval = NULL; }
break;
case 672:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 673:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
break;
case 674:
{ yyval = NULL; }
break;
case 675:
{ yyval = yyvsp[0]; }
break;
case 676:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 677:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 678:
{ yyval = NULL; }
break;
case 679:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 680:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 681:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 683:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 684:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 686:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 687:
{ lex.limit_clause = true; }
break;
case 688:
{ lex.limit_clause = false; }
break;
case 689:
{ lex.first_detection = true; }
break;
case 690:
{ lex.first_detection = false; }
break;
case 691:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 692:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 693:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 694:
{ yyval = 0; }
break;
case 695:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 696:
{ yyval = yyvsp[-2]; }
break;
case 697:
{ yyval = yyvsp[-1]; }
break;
case 698:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 699:
{ yyval = yyvsp[-1]; }
break;
case 700:
{ yyval = yyvsp[0]; }
break;
case 701:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 702:
{ yyval = 0; }
break;
case 703:
{ yyval = make_list (yyvsp[0]); }
break;
case 704:
{ yyval = 0; }
break;
case 706:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 708:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 711:
{ yyval = make_list (yyvsp[0]); }
break;
case 713:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 718:
{ yyval = yyvsp[-1]; }
break;
case 719:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 721:
{ yyval = NULL; }
break;
case 722:
{ yyval = make_list (yyvsp[-1]); }
break;
case 723:
{ yyval = NULL; }
break;
case 725:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 729:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 730:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 731:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 734:
{ yyval = yyvsp[0]; }
break;
case 735:
{ yyval = make_list (yyvsp[-1]); }
break;
case 736:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 737:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 738:
{ yyval = make_list (yyvsp[-1]); }
break;
case 739:
{ yyval = NULL; }
break;
case 741:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 743:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 744:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 745:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 746:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 747:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 750:
{ yyval = make_list (yyvsp[0]); }
break;
case 751:
{ yyval = NULL; }
break;
case 753:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 755:
{ yyval = yyvsp[0]; }
break;
case 756:
{ yyval = NULL; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 762:
{ yyval = 0; }
break;
case 763:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 764:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 765:
{ yyval = 0; }
break;
case 766:
{ yyval = 0; }
break;
case 768:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 774:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 775:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 778:
{ yyval = make_list (yyvsp[-1]); }
break;
case 779:
{ yyval = 0; }
break;
case 780:
{ yyval = make_list (yyvsp[0]); }
break;
case 781:
{ yyval = 0; }
break;
case 783:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 785:
{ yyval = 0; }
break;
case 786:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 787:
{ yyval = 0; }
break;
case 788:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 789:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 790:
{ yyval = yyvsp[-1]; }
break;
case 791:
{ yyval = 0; }
break;
case 792:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 794:
{ yyval = NULL; }
break;
case 795:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
				yyvsp[-6], make_list (yyvsp[-5]), make_list (yyvsp[-2]), NULL, yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_insert, (int) e_ins_count,
				yyvsp[-2], yyvsp[-1], NULL, yyvsp[0], NULL); }
break;
case 799:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 800:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 804:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[0]), NULL); }
break;
case 806:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[-2]), make_list (yyvsp[0])); }
break;
case 807:
{ yyval = NULL; }
break;
case 808:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 810:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 812:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 813:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 817:
{ yyval = NULL; }
break;
case 820:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 821:
{ yyval = yyvsp[0]; }
break;
case 822:
{ yyval = NULL; }
break;
case 823:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 826:
{ yyval = NULL; }
break;
case 827:
{ yyval = make_list (yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 831:
{ yyval = NULL; }
break;
case 832:
{ yyval = make_list (yyvsp[-1]); }
break;
case 834:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 836:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 837:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 838:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 840:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 842:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 844:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 845:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 847:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 848:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 850:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 851:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 853:
{ yyval = yyvsp[-1]; }
break;
case 854:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 866:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 867:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 868:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 869:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 870:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 871:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 872:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 873:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 874:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 875:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 876:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 877:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 878:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 879:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 880:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 881:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 882:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 883:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 884:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 885:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 886:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 887:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 888:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 889:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 892:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 893:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 894:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 895:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 896:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 897:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 898:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 899:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 900:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 901:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 902:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 903:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 904:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 905:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 906:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 907:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 908:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 909:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 910:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 911:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 912:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 913:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 914:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 915:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 916:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 917:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 919:
{ yyval = make_list (yyvsp[-1]); }
break;
case 920:
{ yyval = yyvsp[-1]; }
break;
case 931:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 932:
{ yyval = yyvsp[0]; }
break;
case 933:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 934:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 935:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 936:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 937:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 938:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 939:
{ yyval = yyvsp[-1]; }
break;
case 940:
{ yyval = yyvsp[-1]; }
break;
case 944:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 945:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 946:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 949:
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
case 950:
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
case 951:
{ yyval = make_node (nod_current_timestamp, 1, yyvsp[0]); }
break;
case 952:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 953:
{ yyval = NULL; }
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
