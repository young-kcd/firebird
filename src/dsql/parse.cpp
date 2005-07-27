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
  192,  192,  192,  192,  192,  192,  192,  192,  192,  192,
  192,  192,  192,  192,  192,  192,  192,  192,  201,  201,
  202,  198,  208,  209,  199,  206,  206,  203,  213,  197,
  214,  211,  211,  211,  211,  207,  210,  210,  205,  205,
  205,  212,  212,  195,  195,  218,  219,  219,  220,  220,
  220,  220,  204,  204,  204,  222,  224,  223,  225,   12,
  226,  226,  226,  227,  227,  227,   13,  229,  229,  230,
  230,  231,   78,   87,  147,  235,   98,  100,  234,  234,
   77,   91,  236,  236,  236,  237,  240,  240,  241,  241,
  241,  241,  241,  241,  241,  241,  241,  241,  241,  241,
  241,  241,  241,  238,  238,  239,    2,  243,  243,  243,
  243,  243,  243,  243,  243,  243,  253,  254,  249,  249,
  255,  255,  255,  255,  255,  255,  255,  244,  244,  256,
  256,  256,  256,  256,  256,  256,  248,  261,  261,  261,
  261,  261,  261,  261,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  261,  261,  261,  261,  258,  258,
  260,  260,  259,  257,  257,  257,  250,  250,  251,  251,
  251,  252,  264,  264,  265,  265,  246,  247,  247,  266,
  266,  266,  266,  266,  245,  267,  267,  268,  268,    9,
  269,  269,  269,  269,  269,  269,  269,  269,  269,  269,
  269,  269,  269,  112,  112,  144,  144,  272,  272,  275,
  275,  277,  277,   57,   57,  274,  274,  274,  274,  274,
  274,  274,  274,  274,  281,  281,  273,  273,  273,  273,
  283,  283,  282,  282,  282,   59,   59,  278,  278,  278,
  276,  276,  276,  287,  287,  287,  286,  286,  285,  285,
  285,  279,  279,  288,  288,  288,  289,  289,  280,  280,
  280,  280,  290,  290,   20,   20,   20,  292,  292,  292,
   18,   18,   18,  294,  295,  298,  298,  296,  300,  300,
    5,   17,  299,  299,  301,  301,  302,  302,  291,  303,
  303,  304,  304,  305,  305,  305,  305,  306,  306,  307,
  307,  308,  308,  310,  310,  310,  311,  311,  311,  312,
  312,  312,  309,  314,  314,  314,  315,  315,  313,  313,
  316,  318,  318,  317,  317,  293,    4,    4,    4,  319,
  321,  321,  321,  321,  321,  321,  321,  321,  321,  321,
  321,  321,  321,  321,  323,  323,  324,  320,  320,   19,
  325,  325,  327,  327,  326,  326,  233,  331,  332,  328,
  328,  328,  333,  335,  344,  345,  346,  347,  336,  336,
  336,  336,  348,  348,  348,  349,  349,  349,  337,  337,
  338,  338,  351,  351,  352,  352,  353,  353,  339,  355,
  355,  356,  356,  358,  358,  358,  360,  361,  361,  362,
  362,  364,  364,  357,  357,  357,  365,  366,  367,  369,
  369,  370,  371,  359,  359,  372,  372,  373,  373,   25,
  368,  368,  368,  368,  368,  374,  374,  341,  341,  375,
  375,  376,  342,  342,  340,  340,  343,  343,  377,  378,
  378,  378,  378,  378,  379,  379,  380,  380,  381,  381,
  382,  382,  382,  383,  383,  384,  384,  329,  329,  385,
  385,  386,   72,   72,   72,  388,  388,  387,  387,  330,
  330,  330,   11,   11,    8,    8,  391,  392,   21,   21,
  394,  395,  390,  390,  390,  393,  396,  396,  196,  200,
    3,    3,  399,  399,  399,  401,  401,  402,  400,  400,
  403,  403,   35,   35,   96,   95,   95,  389,  389,  404,
  405,  405,  216,  216,  216,  146,  397,  397,  122,  122,
  122,  122,  122,  409,  409,  409,  409,  409,  409,  408,
  408,  408,  411,  411,  411,  411,  411,  411,  411,  411,
  411,  411,  411,  412,  412,  412,  412,  412,  412,  412,
  412,  418,  418,  418,  418,  418,  418,  418,  418,  418,
  418,  418,  418,  418,  418,  418,  418,  423,  423,  413,
  413,  414,  414,  415,  415,  415,  415,  416,  416,  420,
  420,  421,  421,  421,  421,  419,  422,  417,  417,  407,
  407,  407,  410,  410,  410,  424,  424,  425,   99,   99,
   99,   99,   99,   99,   99,   99,   99,   99,   99,   99,
   99,   99,   99,   99,   99,   99,   99,   99,   99,   99,
   99,   99,   99,   99,   99,   99,  157,  157,  157,  426,
  228,  228,  152,  152,  432,  432,  432,  432,  432,  428,
  428,  428,  428,  428,  232,  153,  153,  154,  155,  155,
  155,  155,  155,   55,   55,   22,   22,  242,  433,   58,
  284,  263,  263,  106,  427,  427,  427,  434,  434,  434,
  434,  434,  434,  434,  434,  434,  434,  434,  435,  435,
  437,  438,  438,  438,  440,  441,  441,  442,  436,  436,
  436,  436,  443,  445,  445,  444,  444,  444,  444,  446,
  446,  446,  398,  398,  429,  430,  430,  447,  447,  447,
  448,  448,  449,  449,  452,  452,  450,  450,  455,  455,
  454,  451,  453,  431,  431,  439,  439,  439,  439,  439,
  439,  439,  439,  350,  350,  334,  334,  156,   52,   68,
  131,  148,  406,  166,  190,  271,   93,   66,  221,  123,
   73,  354,  217,  322,   36,   45,  363,  270,   43,   46,
  215,   44,  297,  262,  262,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,
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
    1,    1,    3,    1,    2,    0,    1,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    1,    1,    1,    1,    2,    3,
    1,    3,    8,    9,    5,    8,    6,    3,    0,    3,
    2,    1,    1,    3,    3,    7,    2,    0,    1,    1,
    2,    3,    0,    1,    2,    4,    1,    3,    2,    2,
    2,    1,    1,    1,    1,    2,    2,    5,    0,    5,
    1,    3,    0,    2,    4,    0,    7,    3,    0,    1,
    3,    3,    7,    7,    0,    0,    0,    0,    3,    0,
    9,    9,    1,    1,    0,    2,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    5,    5,    5,
    5,    5,    5,    2,    0,    4,    2,    2,    3,    2,
    2,    3,    3,    2,    2,    3,    3,    6,    1,    2,
    4,    3,    2,    2,    2,    2,    3,    1,    3,    3,
    3,    2,    2,    4,    4,    5,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    2,    1,    1,    1,    0,    2,    2,    4,    4,
    5,    3,    2,    0,    2,    0,    0,    1,    2,    2,
    4,    3,    2,    2,    7,    1,    0,    1,    0,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    3,    2,
    2,    2,    2,    1,    1,    1,    1,    4,    5,    1,
    3,    1,    3,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    4,    4,    6,    5,
    3,    0,    2,    2,    0,    3,    0,    4,    1,    5,
    4,    1,    4,    1,    2,    2,    1,    1,    1,    2,
    2,    2,    2,    0,    3,    5,    1,    1,    2,    3,
    1,    2,    3,    0,    1,    1,    1,    5,    5,    6,
    1,    1,    1,    2,    4,    1,    0,    5,    1,    0,
    3,    3,    1,    0,    2,    0,    1,    0,    3,    1,
    0,    1,    2,    1,    1,    1,    1,    2,    2,    1,
    2,    3,    1,    1,    3,    3,    1,    2,    3,    1,
    2,    0,    2,    1,    1,    0,    1,    1,    1,    3,
    2,    3,    0,    1,    3,    4,    5,    6,    7,    1,
    1,    1,    1,    1,    1,    2,    1,    1,    1,    1,
    1,    1,    2,    1,    1,    1,    2,    1,    1,    3,
    3,    0,    2,    0,    2,    0,    3,    3,    3,    1,
    4,    4,    1,    9,    0,    0,    0,    0,    3,    2,
    1,    0,    3,    5,    3,    2,    5,    2,    1,    1,
    1,    1,    1,    3,    1,    3,    1,    0,    2,    1,
    3,    1,    1,    1,    1,    3,    6,    1,    0,    3,
    0,    1,    3,    1,    1,    1,    4,    5,    5,    1,
    1,    2,    4,    4,    2,    3,    0,    1,    2,    1,
    1,    2,    2,    2,    0,    1,    0,    3,    0,    1,
    3,    1,    2,    0,    2,    0,    2,    0,    4,    1,
    2,    1,    1,    0,    1,    3,    2,    1,    1,    2,
    1,    4,    3,    1,    3,    4,    0,    3,    0,    1,
    3,    3,    1,    1,    0,    1,    1,    4,    0,    2,
    4,    0,    9,    5,    1,    1,    7,    4,    1,    1,
    8,    5,    2,    4,    0,    4,    1,    3,    3,    1,
    7,    7,    5,    3,    0,    1,    1,    1,    2,    0,
    1,    1,    1,    0,    3,    1,    3,    1,    0,    3,
    1,    3,    1,    3,    3,    1,    1,    3,    1,    2,
    1,    3,    3,    1,    2,    3,    3,    3,    3,    1,
    3,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    3,    3,    3,    3,    3,    3,    3,
    3,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    1,    1,    5,
    6,    5,    6,    3,    4,    5,    6,    3,    4,    3,
    4,    3,    4,    4,    5,    4,    4,    3,    4,    1,
    1,    1,    1,    1,    1,    1,    3,    3,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    3,    3,    3,    3,    3,    3,    3,    3,    1,    1,
    1,    1,    3,    1,    1,    1,    1,    1,    1,    4,
    1,    3,    1,    2,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    1,    2,    1,    1,    1,
    1,    1,    2,    1,    1,    1,    1,    4,    5,    5,
    5,    5,    5,    5,    5,    5,    5,    5,    1,    1,
    6,    1,    1,    1,    4,    4,    4,    4,    1,    1,
    4,    4,    7,    2,    0,    7,    6,    6,    4,    1,
    1,    1,    4,    3,    6,    1,    1,    6,    8,    6,
    1,    1,    4,    6,    4,    5,    3,    5,    4,    5,
    1,    1,    1,    4,    6,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    0,    1,    0,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    3,    4,    5,    6,    7,    8,    9,   10,
   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
   21,   22,    0,  585,  586,  587,  591,  592,  593,    0,
  680,  683,  795,  796,  799,  800,  497,    0,    0,    0,
    0,    0,    0,    0,    0,  407,  603,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  129,    0,    0,
    0,  103,    0,  978,    0,   23,  976,    0,    0,   74,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  510,    0,    0,    0,   37,   34,
   36,    0,   35,    0, 1084, 1086, 1087, 1088, 1089, 1090,
 1091, 1110, 1111, 1092, 1094, 1093, 1099, 1095, 1100, 1096,
 1098, 1101, 1097, 1102, 1103, 1104, 1106, 1107, 1105, 1108,
 1109, 1113, 1112, 1114, 1115, 1116,    0,    0,    0,    0,
   32,   69,   67, 1076, 1085,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  691,    0,    0,
    0,  738, 1078,    0,    0,    0,    0,    0,    0,  116,
 1083,  594,    0,    0,    2,    0,    0,    0,    0,    0,
    0,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  452,  453,  454,  455,  456,  457,
  458,  459,  467,  461,  462,  460,  463,  464,  465,  466,
  468,  470,  473,  469,  471,  472,  474,  475,  476,  477,
  478,    0,  437,  438,    0,  408, 1067,    0,    0,  414,
 1071,    0,  411, 1075,    0,  740,    0,  410, 1079,    0,
  415, 1070,    0,  601,  974,    0,  165,  112,    0,  113,
    0,  215, 1063,  836,  104,    0,  110,  160,    0,    0,
  106,    0,  114,  980,    0,  107,    0,  108,    0,  109,
 1082,  161,  115,  111,  783,  784,    0,  979,  977,    0,
   75,    0, 1068,    0, 1066,  518,  511,    0,  517,  522,
  512,  513,  520,  514,  515,  516,  521,  523,    0,    0,
    0,   30,    0,   39,  823,   38,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  602,  984,    0,  965,  685,  685,  686,  696,  698, 1054,
  699,    0,  700,  690,  686,    0,    0,    0,    0,    0,
    0,    0,  620,  609,    0,  612,  614,  615,  616,  617,
  623,  624, 1077,  739,    0,  120,    0,    0,  117,    0,
  118,    0,  119,    0,    0,  650,  651,  658,    0,  657,
  659,  661,  666,  654,  652,  655,  653,  662,  665,  660,
  664,    0,    0,    0,    0,    0,  670,    0,    0, 1056,
    0,    0,  677,    0,    0,    0,    0,    0,  498,    0,
    0,  377,    0,  218,    0,  419,  128, 1059,    0,  416,
  487,  488,    0,    0,    0,    0,    0,    0,  428,    0,
  383,  384,    0,    0,  607,  605,  975,    0,    0,    0,
    0,    0,    0,    0,  168,  147,    0,  125,    0,    0,
    0,  121,    0,  136,  135,    0,    0,    0,    0,    0,
    0,   76,    0,    0,    0,    0,  798,  519,    0,    0,
    0,    0,  942,  971,    0, 1058,    0,  944,    0,    0,
  972,    0,  966,  957,  956,  955,    0,    0,    0,    0,
  947,  948,  949,  958,  959,  967,  968,    0,  969,  970,
    0,    0,    0,  973,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  961,    0,  833,
  939,  940,  941,  946,  945,  924,    0,    0,    0,  923,
    0,    0,  928,  920,  921,  922,  925,  926,  927,  960,
  985,  986,  987,  999, 1000, 1002, 1003, 1004, 1009, 1010,
 1026, 1027, 1031, 1032,  216,    0,    0,    0,  370,    0,
    0,    0,  826,   27,    0,    0,    0,    0,   72,   70,
 1080,   68,   33,    0,    0,    0,  828,    0,   48,   50,
    0,    0,    0,  599,    0,    0,    0,  693,  695,    0,
  702,    0,    0,    0,  703,  689,    0,  646,    0,  621,
    0,  618,    0,  619,  644,    0,  639,    0,    0,  613,
  837,  807,    0,    0,    0,  126,    0,    0,    0,  596,
  595,  663,  656,    0, 1074,    0,    0,    0,  671,  675,
    0,    0,  780,  682,  681,    0,  183,    0,    0,  144,
    0,  503,    0,  504,  499,    0,    0,  423,  425,  424,
    0,  426,    0,  420,    0,    0,    0,    0,  269,    0,
    0,    0,  432,  433,    0,  480,    0,    0,    0,    0,
    0,    0,    0,  388,  387,  506,    0,    0,    0,  164,
    0,    0,  173,    0,  172,    0,    0,  179,  162,    0,
  177,  169,    0,    0,  542,  577,  545,    0,  578,    0,
    0,  546,    0,    0,    0,  569,  581,  541,  564,  543,
  544,  539,  526,  377,  524,  525,  527,    0,    0,  536,
  537,  538,  540,    0,    0,    0,    0,  124,    0,    0,
  122,    0,  123,    0,  139,    0,    0,  199,    0,    0,
  375,    0,  218,    0,    0,   84,    0,  102,    0,  101,
 1060,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  839,  841,  850,  853,  854,  855,  856,  857,  858,
  859,  860,  861,  862,  863,    0,    0,    0,    0,    0,
  962,    0,    0,    0,    0,    0,    0,  963,  964,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  331, 1081,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  360,    0,    0,  274,  534,    0,    0,    0,  368,
    0,    0,    0,  825,    0,    0,   44,   73,    0,    0,
   26,    0,  831,    0,    0,  794,    0,    0,    0,    0,
  598,    0,  685,    0,  707,    0,    0,    0,    0,  589,
    0,  982,  588,    0,  622,    0,  630,  626,  625,    0,
    0,    0,  641,  629,    0,    0,    0,  802,    0,    0,
    0,  375,  669,  668,  647,    0,    0,    0,    0,    0,
    0,    0,    0,  145,    0,  502,  377,  422,    0,  377,
  378,  427,  493,    0,  492,    0,    0,  265,  375,  238,
 1064,    0,    0,  377,    0,    0,  207,  375,    0,    0,
    0,    0,  243,  237,  240,  241,  242,  431,  485,  484,
  430,  429,    0,    0,    0,    0,    0,    0,  386,    0,
  377,  490,    0,  489,    0,  170,  174,    0,    0,  178,
    0,    0,    0,  565,  566,  582,    0,  579,    0,    0,
  572,  570,  571,    0,    0,    0,    0,  535,    0,    0,
    0,    0,  573,  127,    0,    0,  375,    0,  198,    0,
  201,  203,  204,    0,    0,    0,    0,    0,  218,   79,
    0,    0,   78,    0,    0,    0,    0,  840,  852,    0,
    0,  913,  914,  915,    0,  844,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  760,  762,    0,  757,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1046, 1047, 1048, 1049, 1050,
 1051, 1052, 1053,    0,    0,    0,    0,    0,    0, 1037,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1020, 1022, 1021,    0,    0,  937,  362,    0,  938, 1024,
    0,  933, 1062,    0,    0,    0,    0,    0,    0,    0,
    0,  332,  333,    0,    0,  943,  835,  834,  372,  371,
    0,  272,    0,    0,    0,  280,  827,    0,    0,    0,
   71,    0,    0,    0,  830,    0,    0,    0,    0,  694,
  697, 1072,  706,    0,    0,    0,    0,  712,  713,  714,
  715,  724,  725,  726,    0,    0,  704,  590,  983,  631,
  640,  635,  634,    0,  645,  838,  808,    0,    0,  375,
    0,    0,  648,  667,    0,  687,  782,  781,    0,  501,
    0,    0,  182,    0,  186,    0,    0,    0,  421,  495,
  375,  375,  277,  378,  270,    0,  209,  377,  211,  377,
  378,  206,  212,    0,    0,    0,    0,  434,  435,  377,
  375,  378,    0,    0,    0,  404,    0,  491,  194,  195,
  171,    0,  181,  553,  554,  981,    0,    0,    0,    0,
    0,  580,    0,    0,  378,    0,    0,  530,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  196,    0,    0,    0,   83,    0,    0,    0,    0,   91,
   85,    0,    0,   90,   86,    0,  806, 1065,    0,    0,
  845,    0,    0,  851,    0,    0,    0,    0,    0,  889,
  888,    0,    0,    0,  898,  916,    0,  908,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  843,    0,
  761,    0,  797,    0,    0,  218,    0,  988,    0,    0,
 1011,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1033,    0,    0,    0,    0,    0,    0,
    0, 1044, 1005, 1006, 1007, 1012, 1008,    0, 1019,    0,
    0,  679, 1023,  950,    0,    0,    0,    0,  267,  286,
    0,    0,  367,  281,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   54,   62,   64,    0,   42,    0,    0,
    0,  812,  832,    0,  811,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  741,    0,    0,    0,    0,    0,
    0,    0,  637,  638,  642,    0,    0,  197,    0,  649,
    0,    0,    0,    0,  187,    0,    0,    0,  417,  221,
  222,  223,  224,  225,  226,  953,    0,    0,  271,    0,
    0,    0,    0,    0,    0,    0,  244,    0,  481,  482,
  436,    0,    0,    0,    0,    0,    0,  377,  508,  378,
  180, 1061,    0,    0,  548,    0,  547,  583,    0,  575,
    0,    0,    0,  528,  556,    0,    0,  558,  561,  563,
  375,    0,    0,    0,  134,    0,  202,  377,    0,  376,
  105,    0,  132,    0,   94,    0,    0,    0,    0,   88,
   89,    0,  906,  907,  847,    0,  849,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  909,    0,    0,    0,
    0,    0,    0,    0,  899,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  768,    0,    0,    0,  994,  993,    0,    0,  990,  989,
    0,  998,  997,  996,  995,  992,  991,    0,    0, 1039,
    0,    0,    0, 1038,    0,    0,    0,    0,    0,    0,
    0,  365,  335,  334,  273,    0,    0,  283,  284,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  359,    0,    0,    0,    0,    0,  292,  297,
  291,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  315,  316,  317,  318,    0,    0,  353,
  354,  355,    0,  810,   66,   58,   59,   65,   60,   61,
    0,   24,    0,    0,   25,  818,  817,    0,  816,  814,
  822,  821,  819,    0,   45,   46,    0,  716,    0,    0,
    0,  746,  744,  742,    0,  743,    0,    0,    0,    0,
    0,  801,    0,  376,  786,  787,  688,    0,  192,  188,
    0,  378,  954,  275,  276,  378,    0,  149,    0,    0,
  229,    0,    0,  245,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  505,  550,    0,  551,    0,    0,    0,
    0,  153,  155,  156,  157,  533,  531,  529,  560,    0,
    0,  378,    0,    0,    0,  373,  377,   92,    0,    0,
   98,   93,    0,    0,    0,    0,    0,  917,    0,  918,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  770,  759,    0,    0,  771,    0,  767,  217, 1025, 1045,
 1001,    0,    0, 1035, 1034,    0, 1040, 1028, 1030,    0,
 1017, 1018,    0,    0,  282,    0,    0,    0,  314,    0,
    0,  313,  341, 1073,  356,  357,    0,  302,  300,  304,
    0,  301,  293,    0,  298,    0,  344,  299,  303,  305,
  306,  307,  308,  309,  310,  311,  312,    0,    0,  337,
    0,   55,   56,   57,   63,    0,    0,  793,    0,  736,
  734,    0,  727,    0,    0,    0,  750,    0,  684,  378,
  374,  788,  189,  378,  159,  264,    0,    0,  205,  230,
    0,    0,    0,    0,  231,  234,    0,    0,  378,  402,
  403,  399,  398,  401,  400,    0,  549,  576,  158,  146,
  154,    0,  377,  261,  141,  378,  379,    0,    0,   96,
    0,   80,    0,  875,  883,  678,    0,  876,  884,  879,
  887,    0,    0,  877,  885,  878,  886,  872,  880,  873,
  881,  874,  882,  766,    0,    0,    0, 1013, 1036,    0,
 1016,    0,    0,  289,  285,    0,    0,    0,    0,  328,
    0,    0,  352,    0,    0,    0,    0,  347,  345,    0,
    0,    0,   40,  813,    0,    0,  718,  728,    0,    0,
  729,  730,  731,    0,  262,  418,  208,  213,  232,    0,
    0,  235,    0,  247,  249,  406,  378,    0,  381,    0,
   77,   99,  100,    0,    0,    0,  773, 1029,  219,  288,
    0,    0,    0,    0,  351,  350, 1069,  349,    0,    0,
    0,    0,    0,    0,    0,  717,    0,    0,  751,  236,
    0,    0,    0,    0,    0,  263,  378,    0,    0,  772,
    0,    0,    0,    0,    0,  346,  348,    0,    0,    0,
    0,  722,    0,    0,    0,    0,    0,  246,    0,  253,
    0,  252,  382,  133,  775,    0,  290,    0,    0,    0,
    0,    0,    0,  720,  733,  233,    0,    0,  257,  256,
  255,  776,    0,    0,    0,    0,  336,  723,  260,  258,
  259,  326,    0,  342,  323,  324,
};
static short yydgoto[] = {                                      21,
   22,   23,   24,   25,   26,   27,   28, 1574,   30,   31,
 1575, 1576,   34,   35,   36,   37,   38,   39, 1577,   41,
 1578, 1616,  147,  565,  172, 1361, 1612,  148,  836,  149,
  568,  841,  150,  151,  314, 1125,  161,  162, 1362, 1363,
 1364, 1365,  247,  279,  152, 1366,  153,  570,   90,  291,
  462,  517,  744, 1238,  518,  992,  713,  272,  968,  745,
  746,  747, 1245, 1240, 1702,  292,  749,  750,   82,  265,
   83,  287, 1934, 1471,  271,  276,  278,  280,  267,  258,
  260,  273,  283,  180,  369,  371,  373,  366,  452,  731,
  733,  728,  235,  236,  562,  315,  913,  647,  760, 1169,
  456,  736, 1229, 1465,  681,  862, 1201,  639,  640,  662,
  447,  714, 1181, 1680, 1819, 1409, 1681, 1682, 1683, 1684,
 1685,  761,  268,  259,  443,  689,  444,  445,  690,  691,
 1441,  641, 1163, 1164, 1165, 1404,  458,  980,  981,  982,
  983,  915, 1659,  715,  917,  520,  985, 1082,  556, 1528,
 1884, 1410,  521,  522,  523,  524,  525, 1660, 1661,  665,
 1825, 1963, 1427,  923,  924,  910,  925,  926,  927, 1964,
 1965, 2010,  424,  561, 1104, 1579,  243,  658, 1101,  659,
 1174,  557, 1105, 1106, 1351, 1557, 1558, 1559, 1885, 1247,
 1580, 1581, 1582, 1583, 1776, 1584, 1585, 1586, 1587, 1588,
 1589, 1590, 1591, 1592, 1593, 1594, 1595, 1596, 1597, 1598,
 1091, 2001, 1890,  526,  811,  527, 1599, 1777, 1897, 1898,
 1946, 1600, 1601, 1602, 1767,  528,  822,  529,  311,  558,
  559,  530,   43, 1470, 1696,  433,  676,  941, 1439,  677,
  939,   87,   66,  428,  248,  191,  408,  232,  415,  240,
  251,  420,  901,  898,  416,  429,  931,  430,  672, 1192,
  233,  531, 1216,  656,  905,  409,  678, 1440,  105,  246,
  296,  716,  717,  826, 1217,  827, 1218,  720,  721,  722,
  723,  953, 1210, 1208,  724,  725,  726,  961,  727,  958,
   44,   45,   46,   47,   48,   49,  182,  621,   68,  585,
  254,  436,  354,  355,  356,  357,  358,  359,  360,  361,
  362,  868,  606, 1144, 1395,  607,  608,  873,  392,  885,
  393,  626,  394,  888,  187,  397,  629,   50,  190,  403,
 1494,  810,   51,  401,   52,  166,  342,  593,  858,  466,
 1392, 1641,  777,  588,  344, 1401, 1812,  167,  168,  343,
  594,  595,  856, 1123, 1126, 1127, 1128, 1129, 1130, 1131,
 1906, 1956,  532, 1983, 1132, 1133, 1134, 1390, 1911, 1912,
 1913, 1382,  175, 1633, 1806, 1807, 1521, 1030, 1522, 1523,
 1524, 1737, 1935, 1937,  632,  633, 1157, 1647,  576, 1798,
   53,   54,  467,   55,   56,  614,  615,  533, 1113, 1372,
 1618, 1619, 1623,  577,  844,  264,  762,  763, 1007, 1008,
  764,  765,  766,  767,  768,  769,  770,  771,  772,  773,
  774,  775, 1263, 1265, 1266,  534,  535,  536,  537,  538,
  539,  540,  289,  541,  542,  543,  544,  545, 1054,  546,
  547,  548,  549,  550, 1743, 1075,  551,  552,  553,  554,
  793, 1058, 1327, 1322,  794,  155,
};
static short yysindex[] = {                                  16311,
 1466, -129, 3454,   89,  687,    5, 1951, -114,16504,   26,
   67,  -77, -129, 1092,  644,11862,  831,11862, -130,  117,
    0,  287,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  187,    0,    0,    0,    0,    0,    0,  433,
    0,    0,    0,    0,    0,    0,    0,17528,11862,  402,
11862,11862,11862,11862,11862,    0,    0,  341, 1228,11862,
11862,11862,  490,11862,  325,11862,11862,    0,11862,11862,
11862,    0, 1236,    0,  419,    0,    0,  610,11862,    0,
11862,11862,11862,  624,11862,11862,11862,11862,  325,11862,
11862,11862,11862,11862,    0,11862,  910,  588,    0,    0,
    0, 1013,    0, 1013,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  688,  693,  186, 1061,
    0,    0,    0,    0,    0,11862,11862,11862,  710,  776,
 1408,11862,  -80,   88,  196,  987,  633,    0,11862,  827,
 2045,    0,    0,11862,  762,11862,11862,11862,11862,    0,
    0,    0,11862, 7416,    0,  828,  865, 1000, 1103,  803,
 1340,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1001,    0,    0, 1228,    0,    0,  914,  413,    0,
    0, 1341,    0,    0, 1097,    0,  546,    0,    0,  866,
    0,    0,  979,    0,    0,  954,    0,    0, 1161,    0,
 1191,    0,    0,    0,    0, 1228,    0,    0,  700, 1341,
    0,   11,    0,    0, 1175,    0, 1178,    0, 1013,    0,
    0,    0,    0,    0,    0,    0, 1174,    0,    0,  914,
    0, 1194,    0, 1113,    0,    0,    0,  914,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,15352,11862,
 1169,    0,11862,    0,    0,    0, 1095, 1219,11595,11862,
  757, 1291, 1542, 1301, 1333, 1344, 1279, 1290,  112, 1190,
    0,    0,15608,    0,    0,    0,    0,    0,    0,    0,
    0,13304,    0,    0,    0, 1244,11862, 1337, 1267,  801,
11862, 1276,    0,    0, 2045,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11862,    0, 1228, 1341,    0, 1175,
    0, 1013,    0, 1349, 1304,    0,    0,    0, 1420,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1404,11862,11862, 1388, 1270,    0,15608, 1362,    0,
 1362,15608,    0, -111, 1275, 1278, 1287, 1340,    0, 1529,
 1328,    0,11862,    0, 1001,    0,    0,    0, 1520,    0,
    0,    0,11862, 1169,12397, 1357,12660, 1789,    0,17528,
    0,    0,  598, 1385,    0,    0,    0, 1776, 1776, 1228,
 1480, 1228,   99, 1161,    0,    0,10916,    0,11862,11862,
11862,    0, 1169,    0,    0, 1565,  766, 1811,11862, 1615,
11862,    0, 1850, 9995, 9352, 1506,    0,    0, 1863, 1865,
 1876, 1475,    0,    0, 1890,    0, 1899,    0, 1910, 1915,
    0, 1917,    0,    0,    0,    0,    0, 1919, 1490, 1508,
    0,    0,    0,    0,    0,    0,    0, 1923,    0,    0,
13560, 1930, 1931,    0, 1932, 1624, 1939, 1940, 1941, 1943,
 1944, 1946,15608,15608,13816,11862, 1948,    0, 2877,    0,
    0,    0,    0,    0,    0,    0, 1898, 1590, 1954,    0,
    0, 1949,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,10916, 1938, 1562,    0, 1963,
 1747, 1582,    0,    0,11862,11862,11862,   96,    0,    0,
    0,    0,    0,11862,11862,  518,    0,11862,    0,    0,
 1095, 1618,11595,    0,11862,13816, 2051,    0,    0,15608,
    0, 2677, 1698, 1972,    0,    0,   84,    0,  563,    0,
  390,    0,  390,    0,    0, 1974,    0,  113, 1600,    0,
    0,    0, 1975,  106, 1961,    0, 1169, 1984, 1761,    0,
    0,    0,    0,  673,    0, 1689, 1982,11862,    0,    0,
 1807, 1989,    0,    0,    0,  870,    0, 1690, 1693,    0,
 1228,    0, 1694,    0,    0, 1763, 1772,    0,    0,    0,
 1752,    0,10916,    0, 1228, 1686,10916, 1616,    0, 1790,
11862,17202,    0,    0,  449,    0,11862, 1079, 1097, 1663,
 1627, 1595,    0,    0,    0,    0,  640, 1668,   85,    0,
 1621,  325,    0, 1228,    0, 1791, 1721,    0,    0,   99,
    0,    0,  131, 1634,    0,    0,    0, 1635,    0, 1676,
 2036,    0, 1732, 2039,  671,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1990,  222,    0,
    0,    0,    0,  158, 2040, 2042, 2039,    0, 1228, 1341,
    0, 1770,    0, 1822,    0, 1228, 1228,    0,12397,  546,
    0, 1711,    0, 1685, 2046,    0, 5521,    0, 1722,    0,
    0, 1730, 2063, 9608, 2069,    0,    0,    0, 9096, 7733,
  101,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  962, 1733, 1173,15608,  151,
    0,11862,15608, 1184, 1245, 1246, 2443,    0,    0,15608,
 9864, 2877, 1667,  545,15608,15608, 9864, 1797,15608,15608,
15608,15608,15608,10120,  897,  897, 2131, 1629,  433, 2073,
    0,    0,14072,11862,15608,15608,15608,15608,15608,15608,
 6711,    0,15608, 6948,    0,    0, 1842, 2055,11862,    0,
11862, 1825,11862,    0, 1696, 1697,    0,    0, 1666,11595,
    0, 1816,    0, 1636, 2091,    0, 1816,11862,11862, 2098,
    0, 2131,    0, 2138,    0,11862, 9226, 1699,15608,    0,
  966,    0,    0,   75,    0, 1695,    0,    0,    0,11862,
  -81,11862,    0,    0,11862,11862, 1506,    0,15608, 1881,
12397,    0,    0,    0,    0,  673,11862, 1809, 2103, 1654,
15608,15608, 1228,    0,  385,    0,    0,    0, 2109,    0,
    0,    0,    0, 1228,    0,  142,11862,    0,    0,    0,
    0, 1879, 2114,    0, 1859, 1885,    0,    0, 1817, 1827,
 1013, 1893,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  325,11862,17300, 1792, 1805, 1808,    0,  325,
    0,    0, 1104,    0, 1369,    0,    0, 1778, 1228,    0,
 9995,   82, 1762,    0,    0,    0,  325,    0, 2036,   90,
    0,    0,    0, 1859,   90, 1779,   90,    0, 2150,  325,
  325,  325,    0,    0, 1169,11862,    0, 1840,    0, 1642,
    0,    0,    0,  598, 1362,11862, 1656,   80,    0,    0,
 2154,  387,    0, 9995,11862, 1362,10386,    0,    0, 1362,
 9608,    0,    0,    0, 8963,    0,  243,  485,15608,15608,
10653, 2157,  918,10920,15608,11187,   48,11454,11721,14328,
11988,12255,12522, 9864, 9864,    0,    0, 1845,    0, 2162,
  803,15608,15608, 2811,15608, 2167,15608, 2166, 2186,15608,
15608,15608,15608,15608,15608,    0,    0,    0,    0,    0,
    0,    0,    0, 1895, 1719,  631,15608,  894,15608,    0,
 9864, 2551, 2690,  467,11862, 2207, 2254, 2388, 2431, 2600,
    0,    0,    0, 1691,14584,    0,    0,  803,    0,    0,
 1675,    0,    0, 1936,  897,  897,   58,   58,  234,10270,
 2170,    0,    0, 2172, 2877,    0,    0,    0,    0,    0,
 1687,    0, 1781, 1950, 1825,    0,    0,11328,11328, 1852,
    0,  -27, 1860,11862,    0,15608, 1860, 1908, 1918,    0,
    0,    0,    0, 7157, 2197, 2195, 1293,    0,    0,    0,
    0,    0,    0,    0, 9864, 1920,    0,    0,    0,    0,
    0,    0,    0,  -89,    0,    0,    0, 1733, 2877,    0,
 1728, 1362,    0,    0,  673,    0,    0,    0, 2877,    0,
 1776, 1978,    0,  385,    0, 2200, 9864, 4955,    0,    0,
    0,    0,    0,    0,    0, 1825,    0,    0,    0,    0,
    0,    0,    0, 1013, 1013, 1753, 2211,    0,    0,    0,
    0,    0, -109,  -42,  400,    0, 1993,    0,    0,    0,
    0,11862,    0,    0,    0,    0,   89, 1760, 1841, 1842,
 2217,    0, 1621, 1815,    0, 2201,  896,    0,11862,  911,
  325, 2220, 2222, 2225, 2005,  546, 1825, 1776, 1693,12397,
    0, 1668, 1824, 8981,    0,  325, 1887, 1973, 5521,    0,
    0,  325,  664,    0,    0, 1977,    0,    0, 2237, 2239,
    0, 9864, 9864,    0, 9864, 9864, 2774, 2877, 2241,    0,
    0, 2877, 2246,13816,    0,    0, 1981,    0,  466, 2260,
 2877, 2262, 1793, 2263, 2877, 2264,15608,15608, 2157,15608,
14840, 2270, 2877, 2271, 2272, 2877, 2273,15608, 2877, 2274,
 2877, 2277, 2279, 2877, 2280, 2282, 2877, 2283,    0, 2062,
    0,17425,    0, 2653, 2729,    0, 2746,    0, 2760,15608,
    0, 2939, 2956, 3002, 3077, 3185, 3259,15608,15608,15608,
 2877, 1907,15608,    0,15608, 2877, 2021,  741,15608,15608,
15608,    0,    0,    0,    0,    0,    0,15608,    0,15608,
 1735,    0,    0,    0, 1819,10270, 9482,11862,    0,    0,
11862,17009,    0,    0,11862,11862,11862,11862,11862,11862,
 1884, 2291, 2297,    0,    0,    0, 1884,    0,  143,  143,
   62,    0,    0, 1837,    0,11328,11328, 2301, 1293, 2304,
15608, 2083, 9226, 1968,    0, 1968,  877, 1968, 2011, 2013,
 2085, 2029,    0,    0,    0,  803, 1825,    0, 1824,    0,
  760, 1621, 1976, 1621,    0, 9864,  393, 1296,    0,    0,
    0,    0,    0,    0,    0,    0, 4955, 4955,    0, 1950,
15608, 4955, 2075, 1962, 1753, 1236,    0, 9864,    0,    0,
    0, 1986, 1987, 1988, 1992, 1996, 1999,    0,    0,    0,
    0,    0, 2328,   89,    0, 1935,    0,    0,   90,    0,
 2014,   90,   90,    0,    0, 1842, 2346,    0,    0,    0,
    0,  598, 1950, 1621,    0, 1693,    0,    0, 2112,    0,
    0, 2103,    0, 2351,    0, 2352, 1228,  -68, 2359,    0,
    0, 1228,    0,    0,    0, 2062,    0, 2062,15608, 1362,
 1362, 1847,  433, 2360,15608, 2086,    0, 1362, 1362,15608,
 1362, 1362, 2824, 2877,    0, 1909,15608, 2877, 1362, 1362,
 1362, 1362, 2877, 1362, 1362, 1362, 1362, 1362, 1362,11862,
    0, 2361, 2363,  829,    0,    0,10916, 2368,    0,    0,
 3322,    0,    0,    0,    0,    0,    0, 3377, 1787,    0,
15608, 2099, 1983,    0,15608, 3409, 1871, 2895, 3459, 3475,
15608,    0,    0,    0,    0,10916, 2355,    0,    0, 2127,
    0,11862,  -90, 2364, 2378, 2089,15608, 2367,    0,11862,
11862,11862,    0, 2375, 2391, 2392, 2116, 2394,    0,    0,
    0, 2117,16874, 2395, 2398, 2400, 2403, 2405, 2406, 2408,
 2410, 2412, 2416,    0,    0,    0,    0,  318, 2419,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2095,    0,11328,11328,    0,    0,    0, 2053,    0,    0,
    0,    0,    0, 1959,    0,    0, 2083,    0, 1882,11862,
 1293,    0,    0,    0, 2143,    0, 9226, 9226,15608, 9864,
 1506,    0, 1950,    0,    0,    0,    0, 1369,    0,    0,
  641,    0,    0,    0,    0,    0, 2877,    0, 2209, 2075,
    0,  989,11862,    0, 2161,  683, 2061, 2163, 2064, 2204,
 2169, 2208, 1825,    0,    0, 2464,    0, 2466, 2160, 2209,
 2014,    0,    0,    0,    0,    0,    0,    0,    0, 1825,
 1668,    0, 1369, 1993, 2132,    0,    0,    0, 2149,  173,
    0,    0, 1842, 2155, 2877, 2474, 2480,    0,  803,    0,
 2877,15608, 2481, 2482, 2877, 2484, 2485,15608,15608, 2877,
 2486, 2487, 2493, 2494, 2496, 2497, 2499, 2501, 2502, 2503,
    0,    0,17425, 2508,    0,11862,    0,    0,    0,    0,
    0,15608, 2509,    0,    0,15608,    0,    0,    0,15608,
    0,    0, 3484,  239,    0, 2234,15608,15608,    0, 9864,
 2877,    0,    0,    0,    0,    0,11862,    0,    0,    0,
10270,    0,    0,  571,    0, 2110,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -34, 2518,    0,
 2178,    0,    0,    0,    0,  143,11862,    0,11862,    0,
    0, 9226,    0, 2972, 2877, 2515,    0,  101,    0,    0,
    0,    0,    0,    0,    0,    0, 2520,11862,    0,    0,
 2215, 2226,11862, 1753,    0,    0, 1013,11862,    0,    0,
    0,    0,    0,    0,    0, 1950,    0,    0,    0,    0,
    0, 1950,    0,    0,    0,    0,    0,15608, 1228,    0,
 2111,    0, 1228,    0,    0,    0, 2877,    0,    0,    0,
    0, 2877, 2877,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11862, 2238, 2877,    0,    0, 3537,
    0, 4955, 4955,    0,    0, 2527, 2877, 1193,  729,    0,
 2243, 2170,    0,11862,11862,   89,  175,    0,    0, 2077,
 2244, 9864,    0,    0,  147, 2533,    0,    0, 9864, 2541,
    0,    0,    0,15608,    0,    0,    0,    0,    0, 1753,
 1013,    0, 2206,    0,    0,    0,    0, 1993,    0, 2877,
    0,    0,    0, 2543, 2548, 2559,    0,    0,    0,    0,
 1362,10270, 2173,10270,    0,    0,    0,    0,17009,  571,
15608,10270,  735,10270,11862,    0,  101,11862,    0,    0,
 2206,  -15, 1753, 2214, 2227,    0,    0, 2564,11862,    0,
11862, 2567, 2170,17009, 2170,    0,    0, 1622,  259, 2311,
 2170,    0, 1883, 1888, 1753,  680,  680,    0, 2316,    0,
 2188,    0,    0,    0,    0, 2575,    0, 2320,10270, 2338,
 2330,17009,11862,    0,    0,    0, 2177,  609,    0,    0,
    0,    0,17009,  208,11862,17009,    0,    0,    0,    0,
    0,    0,17009,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  201,  976,    0,    0,    0,    0,    0,    0,    0,
    0,16580,  127,12785,    0,    0,    0,    0,    0,    0,
    0, 2633,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  431,    0,    0,    0,    0,    0,    0,  121,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  323,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2302,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -36, 2275,    0,    0,
    0,  145,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2278,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  323,    0,    0,15096,13048,    0,    0,    0,
  454,    0,    0,  255,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  748,    0, 2229,  271,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   -6,    0,    0,    0,    0,  964,    0,    0,    0,
    0,    0,  959,    0,    0,    0,    0,    0,  138,    0,
11183,    0,    0,    0,    0,    0,    0,    0,    0,   -6,
    0,  517,    0,    0, 2599,    0,    0,    0, 2382,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  184,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  218,    0,
 2384,    0,    0,    0,    0,    0,12129,    0,    0,    0,
    0,    0, 1213,    0,    0,    0,    0,    0,    0,12922,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  936,    0,    0, 1048,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   -6,    0, 2599,
    0, 2382,    0, 1183,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  686,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1187,    0, 2377,
    0,    0,    0,    0, 1204,    0,    0,    0,  302,    0,
    0,    0,    0, 2384,   44,17609,    0, 1260,    0,    0,
    0,    0,  285,    0,    0,    0,    0, 2205, 2205,    0,
    0,    0, 1281,  152,    0,    0,    0,    0,    0,    0,
    0,    0, 2384,    0,    0, 1306,    0,    0,    0,    0,
    0,    0,10649,    0,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2247,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3053,    0,    0,
    0, 3353, 3653,    0, 3953, 4253,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  427,    0,
    0,    0,    0,    0,    0,    0, 4553, 1330,  248,    0,
 2753,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1356,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12129,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 9738,    0, 2339,    0,    0,    0,    0,    0,    0,
 1098,    0, 1098,    0,    0, 1144,    0, 1237, 1405,    0,
    0,    0,    0,  184,    0,    0, 2384,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  155,  489,    0,    0,    0,  365,    0,    0, 1294,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1365,    0,    0,    0,    0,
    0,    0,    0,    0, 2377,    0,    0,  695,    0, 2198,
    0,    0,  409,    0,    0,    0,    0,  181,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1373,
    0,    0, 2917,16129,    0,    0,    0,16141,    0,    0,
16350,    0,    0,16408,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2067, 2353,    0,
    0,    0,    0,16519,16178,    0,16408,    0,    0,   -6,
    0,    0,    0,    0,    0,    0,    0,    0,   44,  995,
    0,    0,    0,    0, 2258,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2000, 7543, 7740,    0,    0,
  797,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2617,  118,15864,    0,15864,
    0,    0,    0,15864,15864,15864,    0,    0,    0,    0,
    0, 2219,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 5742, 6031, 2620,    0,   76,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2182,    0,    0,    0,
    0, 2396,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  265,    0,    0,    0,    0,  265,    0,    0, 1390,
    0,    0,    0,    0,    0,    0,    0, 1072,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  681,    0,    0,    0,    0,    0,  105,    0,    0,    0,
   44,    0,    0,    0,    0,    0,    0,    0,  875,  328,
    0,    0,    0,    0,  625,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1903,    0,    0,    0,    0,
    0, 2627,    0,    0, 1991, 2859,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  677,  712,  785,    0,    0,
    0,    0,    0,    0,  983,    0,    0,    0,    0,    0,
    0,    0, 3344,    0,    0,    0,    0,    0,16350,    0,
    0,    0,    0,  685,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2384,    0,    0,  329,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 5810,    0,    0,
    0,  213,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2631,    0,    0,
 1409,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2632,    0,    0,
    0,    0,    0, 4853, 6320, 6609, 5153, 5453,    0,    0,
 1421,    0,    0,    0, 1045,    0,    0,    0,    0,    0,
    0,    0,13178,    0, 2402,    0,    0,    0,    0,    0,
    0,    0, 1426,    0,    0,    0, 1426,    0,    0,    0,
    0,    0,    0,    0, 6818,  366,  389,    0,    0,    0,
    0,    0,    0,    0,    0, 1087,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  118,  282,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  553,    0,
 2205, 2230,    0,  981,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2396,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1285,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1428,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 3644,
    0,    0,    0,    0,    0, 1003,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  995, 2396, 2205, 1430,   44,
    0, 2413, 1458,    0,    0,    0, 5810,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  952,    0,    0,
    0, 1123,    0,    0,    0,    0,    0,    0,    0,    0,
 1212,    0, 1376,    0, 2516,    0,    0,    0,    0,    0,
    0,    0, 2547,    0,    0, 6530,    0,    0, 7812,    0,
 7884,    0,    0, 7944,    0,    0, 8039,    0,    0, 1651,
    0, 2617,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2249,    0,    0,    0,    0,  902,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   42,    0,    0,    0,    0,    0,    0,    0,  194,
 1483,  163,  273,    0,    0,    0, 1483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2342, 1345,
    0, 7242,    0, 2344,    0, 2344, 2342, 2344,    0,    0,
    0,  210,    0,    0,    0, 1409, 2396,    0, 1458,    0,
    0,    0, 2236,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1630,    0, 1285, 2302,    0,    0,    0,    0,
    0,  786,  980, 1044, 1142, 1189, 1230,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1163,    0,    0,    0,    0, 2887,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1497,    0,    0,    0,    0,
    0, 1500,    0,    0,    0,    0,    0,  167,    0,    0,
    0,    0,    0,    0,    0,  125,    0,  137,    0,    0,
    0,    0,   76,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8111,    0, 8183,    0, 8243,    0,    0,
    0,    0, 8315,    0,    0,    0,    0,    0,    0, 1282,
    0,    0, 2643,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2646,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
17493, 2629,    0,    0,    0,    0,    0,    0, 1739, 1766,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1512,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1525,    0,    0, 7033,    0,    0,    0,
  648,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1076,    0,    0,    0,    0,    0,    0,  749,    0,    0,
    0,    0,    0,    0,    0,    0, 2648,    0, 1524, 1843,
    0, 2377,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2396,    0,    0,    0,    0,    0,    0, 1576,
 1202,    0,    0,    0,    0,    0,    0,    0,    0, 2396,
 2413,    0,  334,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  810,    0, 8399,    0,    0,    0, 2632,    0,
 8482,    0,    0,    0, 8554,    0,    0,    0,    0, 8614,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2617,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2638,    0,    0, 2642,    0,    0,    0,
 2644,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2387,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  183,    0,
    0,    0,    0, 2342, 1077, 1360,    0,  679,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2906,    0,    0,  190,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  172,    0,    0,    0,    0,    0, 8725,    0,    0,    0,
    0, 8758, 8847,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1904, 2669,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2652, 2655,    0,    0,
    0, 2656,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1587, 2165,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2906,
 1823,    0,  176,    0,    0,    0,    0,    0,    0, 2648,
    0,    0,    0, 2671,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,    0,    0,    0, 8910,    0,    0,    0,
 1038,    0, 1285, 1318, 1839,    0,    0,    0,    0,    0,
    0,    0, 2657,  407, 2662,    0,    0,    0, 2422,    0,
 1593,    0,    0,    0, 2906,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,16758,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  407,    0,    0,  407,    0,    0,    0,    0,
    0,    0,  407,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, 2726,    0,    0,
 2727, 2730,    0,    0,    0,    0,    0,    0,    1,    0,
 2736,   -2, 2582, 2164,   22, -917, 1387, 2597, 1924, 2606,
 2176,    0,    0, 2454, -106,   38,    0,    0,    0,    0,
  258,  297,  -60,  -83,  -55, -295, 2457, 1942,    0,    0,
    0, 1085,    0,    0,    4, 1541, -668,  -71,-1170, 2038,
 1794, -382,    0, 1547,    0, 2695, 1798, 1834,    0,    0,
    0, -598,  -58,    0,    0,    0,    0,    0, 2723,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -66,    0, -624, -719, 1572, -397, 1118,-1020,
    0,    0,    0,    0, -405, -152, -424, 1579, -423,  -56,
    0, -631, 1846,    0, 1131,-1172,    0, 1137,    0,    0,
 2414, -675,  -30,    0,    0,    0,    0, 2376,    0, 2133,
 1603,    0,    0,    0, 1661,    0, 2456, 1957, 1597, 2404,
 2407,    0,    0, -520, 1926,  -49, -697, 1017, -309,    0,
    0,    0,-1125,-1123,-1066,-1064,-1023,    0, 1179,-1197,
    0,  879,-1368, 1186,    0, 2183,    0,    0,    0,  880,
  885,  864, -139, -321,-1052,-1040,    0,    0,    0, 1956,
    0, -780,    0, 1756,    0,    0,    0,    0,    0,-1309,
-1065,    0,    0,    0,    0, -252,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
-1021,    0,    0, -803,    0, -799, 1303, 1081,    0,  929,
    0,    0,    0,    0,    0,    0,    0, -477,    0,    0,
 2052, -154, -501, 1481, 1241, -621, -878,-1109,-1490,    0,
    0,  -52,    0,    0,    0,    0,    0, 2458,    0,    0,
    0,    0,    0,    0, 2475, 2224,    0,    0,    0,    0,
    0,   -9, -538,    0,    0, 2483,    0,    0,    0,  -10,
    0,    0,    0, -421, 1922, -418, 1441,    0,    0,    0,
    0,    0,    0, -684,    0,    0,    0, 2171,    0, 1945,
    0,    0,    0,    0,    0,    0,  -25,    0, 2884,    0,
 2739,    0,    0,    0, 2544,    0,    0,    0,    0, 2317,
    0, 2322,    0,    0,    0, 2059,    0,    0,    0, -743,
    0, -317,    0,    0,    0,    0,    0, -485, -701, -932,
 2550,    0, 1208,    0,    0,    0,    0,    0,    0, -432,
    0,    0, -840, -278, 1027,    0,    0,    0, 2765, 1090,
    0, 2074,-1195,-1167,    0,-1046, 1810,-1400,    0,    0,
    0,    0, -158,    0,    0,    0,    0, 1557,    0,    0,
    0,    0, 2854,  349,    0, 1035, 2175,    0, 1220,    0,
 1435,    0, -249,    0,    0, 2065,    0,    0,    0,    0,
    0,    0, 2343,    0,    0,    0, -529,-1308, 2121, 1853,
-1255,    0,    0,    0,    0, -764, -522,  316,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1182, 1684,    0,    0,    0,-1009,    0,    0,
    0, 1556,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,-1014, 1644,    0,    0,
};
#define YYTABLESIZE 18141
static short yytable[] = {                                     154,
   40,   86,  239,  889,  266,  174,  173,  316,  181,  336,
  339,  335,  338,  261,  651,  364,  277, 1092,  306,  688,
  262, 1093,  274,  569,  282,  718,  297,  303,  719,  809,
  914,  653,  890,  682,  250,  825, 1148,  808,  301, 1447,
  305, 1560, 1411, 1604, 1412,  843,  274,  307,  234,  237,
 1102,  241,  244,  173,  249,  252, 1664,  589,  863, 1098,
  263,  237,  252, 1353,  244,  300,  173,  249, 1345,  281,
  154,  252,  257,  308,  846, 1031,  627, 1379,  993,  293,
  174,  173,  295,  237,  245,  293,  252,  241,  244,  304,
  173,  249,  281,  154,  252,  372,  244,  275, 1303,  242,
  809, 1413,  660, 1414,  758, 1232,  322, 1078,  324,  367,
 1146,  270,  612,  657, 1620, 1056,  779,  779,  984, 1237,
  779, 1064, 1468, 1420,  334, 1207,  604,  333,  861,  943,
  453,  734,  902,   85, 1213,  302,  906,  166,  346,  840,
  944,  916, 1153,  309, 1415, 1342,  263,  173,  263,  876,
  334,  167,  154, 1419,  785,  320,  872,  374, 1416,  252,
 1423,  779,   52,  758,  363,  846,  237,  244,  173,  281,
  952, 1431,  460,  181, 1463,  254,  779,  848,  323,  779,
  405,  877,  719,  756, 1152,  604, 1630,   85,  824,  824,
  833, 1367, 1036, 1090, 1451,  785,  166,  970,  785,  370,
  604, 1186, 1172, 1846, 1700,  334,  613,   95, 1173,  754,
  167, 1176,   97,  785,  368,  894,  254,  363, 1950,  254,
 1183,   52,  719,  719, 1432, 1662,  719,  369,  617,  320,
  824,  998,  637,  824,  254,  337, 1803, 1090,  417,  405,
  718,  719,  756,  719, 1654, 1655,  159,  361,  824, 1658,
  754, 1346, 1090,   87,  740, 1434,   87,  266,  334,  604,
  555, 1765, 1766,  563,  815,  619,  688,  922,  754,  448,
  792,  838,   53,  809, 1604, 1900,  363,  823,  454, 1227,
  106,  809, 1986, 1254,  507, 1688, 1092,  569,  598, 1369,
 1093, 1411, 1411, 1412, 1412,  880, 1411,  156, 1412, 1883,
  263,  494, 1346,  263,  106, 1540,  361, 1393, 1542,  571,
  154,  792,  967,  740, 1142,  611,  964,   67, 1277,  239,
  253,   91,  606,  815, 1433,  809, 1344,  789,  140,  792,
 1143,   53, 1278,  193,  814, 1081, 1631,  241,  158,  815,
  809,  173, 1089,  507, 1643,  185,  296,  330, 1299, 1300,
 1413, 1413, 1414, 1414,  601, 1413,  338, 1414, 1394,  239,
  494,  157, 1024,  652,  790,  709,  369,  183,  789,  455,
  616,  789,  605,  555,   14,  262, 1279,  668,  160, 1656,
  670,  606,  729,  625,  625, 1328,  789,  140,  710, 1701,
  732, 1435,  193, 1415, 1415,  686,  266,  638, 1415,  107,
 1370, 1908,  742,  263, 1280,  790,  709, 1416, 1416, 1758,
  340, 1400, 1416,  263, 1190,  263,  613,  263, 1987, 1674,
  673, 1214, 1692,  790,  709, 1136,  951,  871,  583,  710,
  672, 1799,  710, 1652,  166,  779,  239, 1967, 1171,  237,
  244,  249,  637,  683,  405,  685, 1396,  710,  167,  173,
 1035,  241, 1397,  611,  751, 1922,  779, 1891, 1625, 1626,
  738,  748, 1662, 1642, 1424, 1425, 1236,  951, 1281,  785,
  951,  672,   95, 1417, 1418, 1949,  239,   97, 1251, 1025,
  740,  166, 1954, 1233,  758,  951,  338,  730,  778,  672,
  785, 1407,  184, 1430, 1249,  167,  966,  719, 1250, 1166,
  719,  186, 1168,  846, 1252,  603,  812,  719, 2023,  719,
 1331, 1206,  611,  719, 1473,  848, 1179, 1775,  719,  951,
  824,  719, 2000, 1850,  754,  611, 1744,  604,  945,  778,
 1747, 1206, 1852,  332,  332, 1882,  719,  332,   84,  332,
 1904,  839, 1553, 1197,  569,  754, 1554,  778,  507,  465,
  719, 1960,  791,  951,  604,  173,  244,  571,  719,  851,
  860,  942,  719,  756,  173,  824,  779, 1555,  173, 1154,
  993,  719,  756,  571, 1120,  181, 1485, 1486,  563, 1487,
 1488, 1843, 1098, 1691, 1373,  792,  835,  719,  779,  754,
  975, 1804,   84,  791, 1988,  842,  969,  657,  754,  847,
  785,  604, 1810,  837, 1462, 1239,  792,  687,   52, 1472,
  946,  791,  918,  319,  719,   87, 2006,  758,  263,  363,
 1836, 1253, 1378, 1147,  184,  815,  719,  884,  719,  274,
  779, 1815, 1788,  779,  740, 1816, 1817, 1842, 1374, 1090,
 1604,  754,  789,  740,  895,  332,  166,  711,  785,  361,
 1399,  911,  263, 1225, 1024,  754, 1851,  911,  903, 1243,
  167,  809, 1094,  789,  740, 1604,  494,  785,  254,  421,
  809, 1844,  140, 1099,  507,  719,  391,  193,  753,  790,
  709, 1814,  824,  184,  150,  674, 1622,  947,  711,  262,
  709,  711,  709, 1604,  486,  719,  756, 1436,  740,  719,
  790,  709,  294,  710, 1604, 2024,  711, 1604, 1139, 1939,
 1940,  389,  338,  710, 1604,  710,  792,  613,   53,  753,
  238,  338,  754, 1829,  710,  809,  674,  745, 1024,  263,
 1651, 1879,  974, 1437, 1244,  391, 1161,  753,  486,  978,
  979,  253,  422,  150,  674,  709, 1255,  676,  193, 1892,
  269, 1038, 1666,  486,  709, 1402, 1411, 1411, 1412, 1412,
  866, 1677, 1789, 1690,  919, 1496,  672,  740,  710, 1943,
  389, 1025,  252,  789,   84, 1980, 1856,  710, 1493,  555,
 1421,  555, 1422, 1107,  390,  396, 1492, 1982,  676, 1915,
 1139, 1709, 1429, 1916,  809, 1926,  755,  709, 1063,  137,
 1809, 1927,  431,  778, 1083, 1162,  676,  193, 1925,  709,
  790,  709,  188, 1497,  263, 1413, 1413, 1414, 1414,  263,
  710,  263, 1464,  263,  778, 1929,  611, 1665,  951,  867,
  571,  262,  710, 1893,  710, 2018,  438,  755,  173,  244,
  789,  920,  338,  390,  396, 1025, 1122,  244, 1059, 1060,
  557,  338, 1547,  557, 1239,  755,  674,  555, 1415, 1415,
  173, 1188,  173, 1256,  189,  263,  675,  791,  288, 1118,
 1063,  263, 1416, 1416,  673,  432,  672,  625,  709, 1894,
  274,  921,  184, 1976, 1189, 1191,  837, 1196,  791,  884,
 1895,  605, 1024, 1145,  184, 1738, 1160,  263, 1222, 1223,
 1224,  710, 1024, 1629, 1211, 2020, 1966, 1170, 1998, 1968,
 1678,  818,  816, 1686,  817,  673,  819,  274,  274,  274,
 1973,  184, 1975, 1527,  263,  263,   14,  184,  290,  184,
 1979, 1094, 1981,  673,  778,  627, 2017,  936,  818, 1453,
  391,  751,  298,  819, 1024,  962, 1993, 2022, 1204,  310,
 2025,  900, 1203,  845, 1453,  613, 2021, 2026,  608,  864,
  150,  150,  711,  385, 1808,  169,  173, 1092,  184,  137,
  137, 1093,  711,  937,  711,  389,  173, 2014,  352,  397,
  185,  312,  193,  711,  751, 1248,  745, 1061, 1454, 1896,
 1024,  748,  900,  753,  627,  900, 1024, 1226,  791,   88,
   89,  778, 1024, 1456, 1493, 1493,  193, 1234,  449, 1025,
  900,  963, 1493, 1493,  753, 1493, 1493,  608,  193, 1025,
  883,  674,  385, 1493, 1493, 1493, 1493,  711, 1493, 1493,
 1493, 1493, 1493, 1493, 1332, 1754,  711,  254,  397,  185,
 1673,  193,  894,  393,  952,  193,  532,  610,  390,  396,
 2007,  193,  313,  193,  109,  252,  150, 1320,  753, 1605,
 1139, 1025, 1608,  317,  611,  170,  391,  753,  318,  999,
 1694,  756,  171,  938, 1006,  758,  752,  636,  254,  711,
  601,  254,  557,  676, 1889,  952,  749,  325,  952, 2008,
  111,  711,  193,  711,  450,  532,  254,  632,  571,  571,
  193,  389,  393,  952,  321,  718,  610, 1025,  719,  737,
  753,  755,  756, 1025,  244,  557,  758,  752,  165, 1025,
  752,  755,  867,  755,  753,  255,  256,  749,  636,  451,
  756,  674,  755,  184,  758,  752, 2009,  952, 1092,  176,
 1092,  392, 1093,  633, 1093,  749,  814, 1480, 1092, 1457,
 1092,  815, 1093,  326, 1093,  112,  632,  557,  884,  347,
  711, 1734,  151,  867, 1475,  113,  867, 1545,  274,  193,
 1479,  365, 1905,  814,  390,  396,  755,  602,  815, 1481,
  262,  867,  597,  274,  563,  755,  412, 1094,  395,  274,
  114,  753, 1442,  613, 1384, 1092, 1735, 1323, 1324, 1093,
  392,  152,  633,  413, 1443, 1043, 1043, 1385, 1736, 1442,
  673,  868,  557,  900, 1617, 1617, 1621, 1267,  255,  256,
  263,  151,  385, 1813,  263,  177, 1953,  385,  755,  394,
 1386,  603,  385, 1957,  818,  816,  643,  817,  185,  819,
  130,  597,  755,  397, 1645,  412,  340,  395,  604, 1648,
  185, 1650,  868,  385, 1646,  868,  178,  193,  410,  409,
  152,  395,  413,  385,  922, 1268,  900,  557, 1845,  900,
  868,  179,  398,  627,  130, 1609,  900,  185,  900,  193,
  175, 1388,  900,  185,  248,  185,  341,  900,  394, 1269,
  900, 1520,  173,  500, 1556,  643, 1607,  892,  555, 1848,
 1026,  262,  611,  411, 1610,  900,  627,  393,  130,  755,
  396, 1693,  999,  254,  254,  402,  999,  250,  409,  900,
  673,  254,  419, 1027,  185,  248,  193,  900,  248,  366,
  900,  900,  627, 1984,  193,  627, 1325,  263,  263,  175,
  900, 1561, 1603,  248, 1043,  571,  244,  249,  571,  281,
  154,  627,  500,  385,  425,   43,  900,  426,  250,  748,
 1821,  250,  399,  590,  496,  418,  571,  571,  193,  397,
  193,  596,  176,  244,  463,  894,  250,  627,  900, 1028,
  423, 1822,  468,  900,  867,  434,  756, 1823,  366,   47,
  758,  752,  193, 1606,  435,  900,  756,  900,  756,  427,
  748,  749,  400,  752,  628,  392,  437,  756,  792,  254,
  412,  758,  752,  749,   43,  332,  894,  193,  748,  894,
  364, 1824,  749,  496,  613,  820,  519,  509,  413,  142,
  254,  176,  340,  393,  894,  632,  254,  867,  377,  151,
  867, 1676, 1138,  340,  900, 1928,  952,  867,   47,  867,
  587,  756,  395,  867,  446,  758,  752,  380,  867,  592,
  756,  867,  414,  628,  900,  752,  749,  792,  900,  814,
  254, 1801, 1032,  868,  815,  749,  867,  377,  152,  364,
 1699,  633,   41, 1040,  820, 1704,  509,  457,  142,  185,
  867,  193,  459,  394,  632, 1757,  143,  632,  867,  131,
  285,  867,  867,  756,  340,  340,  461,  758,  752, 1520,
  173,  867,  438,  632,  633,  631,  380,  756,  749,  636,
  564,  758,  752,  214,  805,  464,  868,  867, 1942,  868,
  254,  392,  749,  611,  286,  929,  868,  930,  868,  632,
  633,   41,  868,  633, 1042, 1044,  439,  868,  440,  867,
  868,  500,  237,  332,  867,  143,  465, 1077,  131,  633,
 1764, 1248, 1248,  500,  214,  868,  867,  214,  867, 1073,
  441,  560, 1063, 1603,  643,  214,  164,  165,  395,  868,
 1198,  575,  214,  805,  756,  633,  803,  868,  758,  752,
  868,  868,  804,  250,  250,  442,  500,  404,  500,  749,
  868,  250,  830,  571,  571,  829,  634,  643,  635,  405,
 1384,  649, 1094,  566,  769,  867,  868,  578,  792,  394,
  363,  829,  834, 1385,  650,  833,  574,  244,  244,  227,
  805,  806,  807,  643,  214,  867,  643,  894,  868,  867,
 1907, 1617,  406,  868,  407,  803, 1386,  579,  829,  769,
  842,  804,  643,  173,  581,  868,  908,  868,  580,  907,
 1387,  769,  712,  818,  816,  582,  817,  108,  819, 1077,
  227,  597,  823,  227,  748,  712, 1115, 1876,  643, 1114,
  255,  256, 1231,  712, 1827, 1230,  748,  584,  227,  250,
  894,  842,  599,  894,  842,  748, 1235, 1388,  712,  989,
  894,  609,  894,  852,  868,  109,  894,  854,  600,  842,
  250,  894,  712,  622,  894, 1343,  250,  110,  823, 1995,
 1923, 1996, 1520,  173,  868,  620,  241, 1349,  868,  894,
 1348, 1339,  818,  816, 1634,  817, 1636,  819,  623,  748,
  624,  111,  628,  894,  484,  485,  486,  563,  748,  712,
  250,  894, 1199, 1200,  894,  894,   57, 1248,  138,  138,
  818,  816,  628,  817,  894,  819,  630,   58, 1398, 1924,
   14, 1230,  494,  495,   59,  628,  818,  816,   60,  817,
  894,  819,  642, 1094, 1092, 1094,  643,  263, 1901,  363,
  613,  748,  244, 1094,  644, 1094, 1092,  339,   61, 1092,
 1445,  628,  894, 1444,  628,  748,  112,  894, 1083, 1389,
  250, 1097,  646,  173, 1961,  613,  113,  666,  241,  894,
  628,  894,  824, 1097,  340,  655, 1097, 1945,  818,  816,
  679,  817,  669,  819,  818,  816,  680,  817,  251,  819,
 1094,  114,  228,  613, 1921,  684,  628,  735,  818,  816,
  739,  817, 1931,  819,  613, 1450, 1933,  613, 1449, 1552,
   62,  712, 1346,  824,  613,  241,  824, 1033,  894, 1037,
 1792, 1794,  748, 1041, 1043, 1045, 1005, 1624,  741,  251,
  823,  824,  251,  228,  237, 1947,  228, 1708,  894,  743,
  823,   63,  894, 1948,  776,   64, 1034,  251,  814,  611,
 1039,  228,  778,  815,  779,  239,  227, 1055,  563, 1793,
 1795, 1749, 1062, 1063,  823,  780, 1066, 1067, 1068, 1069,
 1070, 1074, 1800, 2004,  611,  823, 2003,  781, 2005,  782,
  519,  833, 1084, 1085, 1086, 1087, 1088,  519,  783, 1603,
 1095, 1972,  788,  375,  777, 1122,  375,  777,  263,  784,
  818,  816,  611,  817,  785,  819,  786, 1999,  787,  241,
  789,  241,  790,  611, 1603,  842,  611,  814,  842,  795,
  796,  797,  815,  611,  798,  842,  592,  842,  799,  800,
  801,  842,  802,  803,   65,  804,  842,  813,  820,  842,
  150,  821, 1603, 1122,  824,  814, 1149,  823,  828,  910,
  815,  239,  831, 1603,  842, 1248, 1603, 1338,  631, 1159,
  832,  814,  849, 1603,  857,  859,  815,  870,  842,  874,
  875,  879,  239,  881,  882,  886,  842,  887,  239,  842,
  842,  150,  891,  893,  150, 1319,  637,  896,  897,  842,
  910, 1102, 1102,  910, 1102, 1102, 1102,  899,  900,  150,
  904, 1551,  933,  909,  934,  842,  935,  940,  910, 1102,
 1102, 1102,  239,  814,  949,  948,  534,  956,  815,  814,
  332,  285,  954,  955,  815,  957,  959,  842,  960,  971,
  965,  972,  842,  814,  976,  977,  986,  988,  815,  989,
 1102,  853,  818,  816,  842,  817,  842,  819,  824,  824,
 1500, 1742,  996,  994,  995,  286,  824,  534, 1000, 1057,
  534, 1065,  188, 1079,  251,  251,  966,  334,  239,  228,
 1103, 1110,  251, 1108, 1109,  534, 1257, 1258, 1262, 1112,
 1116, 1271, 1273, 1275, 1140, 1283, 1286, 1289, 1291, 1294,
 1297,  840, 1135,  842, 1150, 1155,  833, 1156, 1167, 1304,
 1305, 1177, 1307, 1178, 1309, 1180, 1184, 1312, 1313, 1314,
 1315, 1316, 1317,  842,  721,  912, 1185,  842, 1187, 1209,
 1193, 1076,  818,  816, 1321,  817, 1326,  819, 1121,  818,
  816,  557,  817, 1194,  819,  814, 1195, 1202, 1219, 1221,
  815, 1228, 1341, 1242,  824, 1272, 1264, 1276,  824, 1284,
 1287, 1302, 1292, 1295, 1298,  721, 1301, 1308,  721, 1310,
  251, 1318,  814, 1346,  239,  824, 1719, 1347, 1350, 1352,
 1371,  824,  557,  721, 1376,  557, 1311,  818,  816, 1368,
  817,  251,  819,  519, 1377,  239, 1381,  251, 1383, 1406,
  557,  239,  557, 1403, 1391, 1426, 1084, 1333,  818,  816,
 1428,  817,   92,  819, 1446,  824, 1438, 1448, 1452,   93,
 1458,  910, 1459,   94,   95, 1460,  150,  150, 1461, 1469,
 1102,  251,   96, 1236,  150,  239, 1102, 1483, 1477, 1484,
 1490, 1102, 1482,   97, 1102, 1491, 1059, 1084, 1084, 1084,
 1084, 1084, 1084, 1084, 1334,  818,  816, 1495,  817, 1498,
  819, 1499, 1501, 1502, 1084, 1084, 1084, 1084, 1084, 1509,
 1510, 1511, 1512, 1514,  910,  824, 1515,  910, 1516, 1517,
 1102, 1518, 1519, 1024,  910, 1544,  910,  814, 1102, 1611,
  910,  251,  815, 1541, 1613,  910, 1102, 1084,  910, 1084,
 1614, 1627,  534,  534, 1628,   98,  855,  534, 1632, 1637,
  534, 1638,  557,  910, 1102, 1640, 1102, 1639,  661, 1649,
 1663,   99,  150,  534, 1667, 1668, 1669,  910, 1675, 1102,
 1670, 1102, 1102, 1102, 1671,  910,  100, 1672,  910,  910,
  101,  519,  348,  150, 1206, 1679, 1689, 1695,  910,  150,
 1697,  102, 1698,  557, 1503, 1504,  557, 1506, 1508, 1703,
 1710, 1732, 1712, 1745,  910, 1513, 1733,  814, 1739, 1746,
  103,  557,  815, 1755,  814,  349, 1756, 1760, 1791,  815,
 1102, 1773, 1759,  150,  157, 1762,  910, 1531, 1335,  818,
  816,  910,  817, 1768,  819, 1538, 1539, 1326,  534,  557,
 1326,  350, 1321,  910,  351,  910, 1546,  519, 1548, 1769,
 1770, 1771, 1772, 1778,  557, 1549, 1779, 1550, 1780,  534,
  352, 1781,  814, 1782, 1783,  534, 1784,  815, 1785,  104,
 1786, 1336,  818,  816, 1787,  817, 1790,  819,  557,  721,
 1796, 1802,  721,  814,  557, 1818,  353,  557,  815,  721,
 1797,  721,  910, 1828, 1830,  721, 1831, 1832,  519,  534,
  721, 1833, 1834,  721, 1837, 1835, 1838, 1839, 1084, 1847,
 1084, 1084,  910, 1849, 1854,  871,  910, 1084,  721, 1853,
 1855, 1858, 1859, 1084, 1860, 1861, 1864, 1865, 1084,  557,
  814, 1084,  721, 1866, 1867,  815, 1868, 1869, 1657, 1870,
  721, 1871, 1872, 1873,  721, 1084,  869, 1875, 1886, 1878,
 1084, 1084, 1774,  721, 1084, 1903,  871, 1902, 1914,  871,
 1917, 1084, 1919, 1084, 1084, 1920, 1941, 1084, 1932,  721,
 1936, 1084, 1955, 1084,  871, 1084, 1951, 1084, 1944, 1952,
 1958, 1962, 1084, 1084,  557, 1084, 1969,  869, 1970, 1989,
  869,  557,  818,  816, 1329,  817,  721,  819, 1971, 1974,
 1084, 1084, 1991, 1084, 1994,  869, 1705, 1997,  721,  557,
  721, 2002, 1711, 1986, 1084, 2012, 1084, 1715, 1084, 1084,
 1084, 1987, 1084, 2013, 1720, 1084, 1084, 2015,  557,  557,
 2016, 2019,    1,  557,  785, 1084,  557, 1057,  200,  557,
 1337,  818,  816,  557,  817,  824,  819,  268, 1084,  557,
   29, 1084,  377,   31,  163,  701,  764,  721, 1326,  483,
   82, 1042, 1326,  951,  814,  279,  210, 1084, 1753,  815,
  763,  278,  792, 1084, 1084, 1041,  405,  721, 1084,  190,
  745,  721,  747,  765, 1761,  191, 1015,  321,  378, 1084,
 1084,  295, 1084, 1525,  818,  816,  220,  817, 1084,  819,
  319, 1084,  329, 1084, 1084, 1084, 1084,  814, 1084, 1014,
  320,  774,  815,  322,  330,  325, 1084, 1084,  818,  816,
  358,  817,  343,  819,  557,   29,   32, 1084, 1084,   33,
 1084,  818,  816, 1330,  817,   42,  819, 1084, 1084, 1084,
 1084, 1084,  327, 1084,  848,  557, 1084, 1084, 1084, 1084,
 1084,  557, 1063, 1615, 1084, 1084, 1805,  328,  850, 1084,
 1084, 1084, 1084, 1084, 1084, 1084, 1084,  329, 1084, 1526,
  818,  816, 1119,  817,  573,  819,  572,  871, 1084, 1478,
  987, 1111, 1241, 1476, 1205,  557, 1529,  818,  816,  299,
  817, 1246,  819, 1063, 1063, 1063, 1063, 1063, 1077, 1063,
 1530,  818,  816,  284,  817, 1474,  819, 1466,  869, 1215,
 1840, 1063, 1063, 1063, 1063,  818,  816, 1841,  817,  692,
  819, 1455,  950,  648, 1405,  618, 1467,  814,  663, 1857,
  871,  664,  815,  871, 1918, 1862, 1863, 1151, 1820, 1985,
  871, 1182,  871, 1063, 1992, 1063,  871, 1826, 1990,  928,
 2011,  871,  818,  816,  871,  817, 1899,  819,  524, 1877,
 1354,  869, 1175, 1326,  869,  818,  816, 1880,  817,  871,
  819,  869, 1763,  869, 1887, 1888,  814,  869, 1977, 1644,
 1100,  815,  869,  871, 1811,  869,  557,  671, 1220,  654,
  645,  871,  932, 1687,  871,  871,  163,  973,  610,  524,
  869,  331,  524, 1212,  871,  248, 1046, 1047, 1048, 1049,
 1050, 1051, 1052, 1053,  869,  865,  555,  524,  818,  816,
  871,  817,  869,  819,  869,  869,  869,  557, 1141,  814,
  557,  345, 1137, 1380,  815,  869,  818,  816, 1750,  817,
  855,  819,  871, 1635,  294,  557,  248,  871, 1959,  248,
 1029,  869, 1874,  814, 1731, 1158,  878,  555,  815,  871,
  555,  871, 1505, 1653,  248, 1930,  814, 1117, 1543, 1375,
    0,  815,    0,  869,    0,  555,    0,  555,  869, 1532,
  818,  816,    0,  817,    0,  819,    0,    0,    0,    0,
  869,    0,  869,    0,    0,    0, 1533,  818,  816,    0,
  817,    0,  819,    0,    0,  814,    0,    0,  871,    0,
  815,    0,    0,    0, 1063,    0, 1063, 1063,    0,    0,
    0,    0,  814, 1063,    0,    0,    0,  815,  871, 1063,
    0, 1805,  871,    0, 1063, 1489,  814, 1063,    0,  869,
    0,  815, 1534,  818,  816,    0,  817,    0,  819,    0,
  814, 1063, 1094, 1063,    0,  815, 1063, 1063,    0,  869,
 1063,    0,    0,  869,    0,    0,    0, 1063, 1978, 1063,
 1063,    0,    0, 1063, 1306,    0,    0, 1063,    0, 1063,
    0, 1063,    0, 1063,    0, 1718,    0,  814, 1063, 1063,
    0, 1063,  815, 1094, 1094, 1094, 1094, 1094, 1094, 1094,
  814,    0,    0,    0,    0,  815, 1063, 1063,    0, 1063,
    0, 1094, 1094, 1094, 1094,    0,    0, 1535,  818,  816,
 1063,  817, 1063,  819, 1063, 1063, 1063,    0, 1063,    0,
    0, 1063, 1063,    0,  524,  524,    0,    0,    0,    0,
    0, 1063,  524, 1094,    0, 1094,    0,    0,    0,    0,
    0,    0,    0,  814, 1063,  524,    0, 1063,  815,    0,
    0,    0,  557,  557,    0,    0,    0,    0,    0,    0,
  557,  814,    0, 1063,  555,    0,  815,    0,    0, 1063,
 1063,  248,  248,  557, 1063,    0,    0,    0,    0,  248,
    0,  555,  555,  555,    0, 1063, 1063,  555, 1063,    0,
  555,    0,    0,    0, 1063,    0,    0, 1063,    0, 1063,
 1063, 1063, 1063,  555, 1063,  814,    0,    0,    0,  555,
  815,    0, 1063, 1063,    0, 1536,  818,  816,    0,  817,
  524,  819,  814, 1063, 1063,    0, 1063,  815,    0,    0,
    0,    0,    0, 1063, 1063, 1063, 1063, 1063,    0, 1063,
    0,  524, 1063, 1063, 1063, 1063, 1063,  524,  557,    0,
 1063, 1063,    0,    0,    0, 1063, 1063, 1063, 1063, 1063,
 1063, 1063, 1063,    0, 1063,    0,    0,  248,  814,  557,
    0,    0,    0,  815, 1063,  557,    0,    0,  555, 1384,
    0,  524,    0,    0,    0,    0,    0,    0,  248, 1537,
  818,  816, 1385,  817,  248,  819,    0,    0,    0,  555,
    0,    0,    0,    0, 1094,  555, 1094, 1094,    0,  557,
    0,    0,    0, 1094,  555, 1386,  555,    0,    0, 1094,
    0,    0,    0,    0, 1094,    0,    0, 1094,  248, 1387,
    0,    0,    0,  552,  555,    0,    0, 1909,    0,  555,
    0, 1094, 1099,  814,    0,    0, 1094, 1094,  815,    0,
 1094,    0, 1740,  818,  816,    0,  817, 1094,  819, 1094,
 1094,    0,    0, 1094,    0,    0, 1388, 1094,  555, 1094,
    0, 1094,    0, 1094,  552,    0,    0,  552, 1094, 1094,
    0, 1094,    0, 1099, 1099, 1099, 1099, 1099, 1099, 1099,
    0,    0,  552,    0,  552,    0, 1094, 1094,    0, 1094,
    0, 1099, 1099, 1099, 1099,    0,    0, 1741,  818,  816,
 1094,  817, 1094,  819, 1094, 1094, 1094,    0, 1094,    0,
    0, 1094, 1094,    0,    0,    0,    0,    0,    0,    0,
    0, 1094,    0, 1099,    0, 1099,    0,    0,    0, 1748,
  818,  816,    0,  817, 1094,  819,    0, 1094,    0,    0,
    0,  814,    0,    0, 1910,    0,  815,    0,    0,    0,
    0,    0,    0, 1094,    0,    0,    0,    0,    0, 1094,
 1094,    0,    0,    0, 1094,    0,    0,    0, 1389,    0,
    0,    0,    0,    0,    0, 1094, 1094,    0, 1094, 1751,
  818,  816,    0,  817, 1094,  819,    0, 1094,    0, 1094,
 1094, 1094, 1094,    0, 1094, 1752,  818,  816,    0,  817,
    0,  819, 1094, 1094, 1881,  818,  816,    0,  817,    0,
  819,    0,    0, 1094, 1094,  814, 1094,    0,    0,    0,
  815,    0,    0, 1094, 1094, 1094, 1094, 1094,    0, 1094,
    0,    0, 1094, 1094, 1094, 1094, 1094,    0,    0,    0,
 1094, 1094,    0,    0,    0, 1094, 1094, 1094, 1094, 1094,
 1094, 1094, 1094,    0, 1094,    0,    0, 1938,  818,  816,
    0,  817,    0,  819, 1094,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  814,    0,
    0,  552,    0,  815,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1099,    0, 1099, 1099,  552,  552,
  552,    0,    0, 1099,  552,    0,    0,  552,    0, 1099,
    0,    0,    0,    0, 1099,    0,    0, 1099,    0,    0,
  552,    0,    0,  557,    0,    0,  552,    0,    0,    0,
    0, 1099, 1095,  814,    0,    0, 1099, 1099,  815,    0,
 1099,    0,    0,    0,    0,    0,    0, 1099,    0, 1099,
 1099,    0,    0, 1099,    0,    0,    0, 1099,    0, 1099,
    0, 1099,    0, 1099,  557,  814,    0,  557, 1099, 1099,
  815, 1099,    0, 1095, 1095, 1095, 1095, 1095, 1095, 1095,
    0,    0,  557,    0,  557,    0, 1099, 1099,    0, 1099,
    0, 1095, 1095, 1095, 1095,  552,    0,    0,    0,    0,
 1099,    0, 1099,    0, 1099, 1099, 1099,    0, 1099,    0,
    0, 1099, 1099,    0,    0,  814,  552,    0,    0,    0,
  815, 1099,  552, 1095,   69, 1095,    0,    0,    0,    0,
    0,  814,    0,  552, 1099,   70,  815, 1099,    0,    0,
  814,    0,   71,    0,    0,  815,    0,    0,    0,    0,
    0,  552,    0, 1099,    0,   72,  552,    0,    0, 1099,
 1099,    0,    0,    0, 1099,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1099, 1099,    0, 1099,    0,
    0,    0,    0,    0, 1099,  552,    0, 1099,    0, 1099,
 1099, 1099, 1099,  814, 1099,    0,    0,    0,  815,    0,
    0,    0, 1099, 1099,    0,    0,    0,    0,    0,    0,
    0,    0,   73, 1099, 1099,    0, 1099,    0,    0,    0,
    0,    0,    0, 1099, 1099, 1099, 1099, 1099,   74, 1099,
    0,    0, 1099, 1099, 1099, 1099, 1099,    0,    0,    0,
 1099, 1099,    0,    0,   75, 1099, 1099, 1099, 1099, 1099,
 1099, 1099, 1099,    0, 1099,    0,    0,    0,    0,   76,
    0,    0,    0,   77, 1099,    0,   78,    0,    0,    0,
    0,    0,    0,    0,   79,    0,    0,    0,    0,    0,
    0,  557,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   80, 1095,    0, 1095, 1095,    0,  557,
  557,    0,    0, 1095,  557,    0,    0,  557,    0, 1095,
    0,    0,    0,    0, 1095,    0,    0, 1095,    0,    0,
  557,    0,    0,    0,    0,    0,  557,    0,    0,    0,
    0, 1095, 1108,    0,    0,    0, 1095, 1095,    0,    0,
 1095,    0,    0,    0,    0,    0,    0, 1095,    0, 1095,
 1095,    0,   81, 1095,    0,    0,    0, 1095,    0, 1095,
    0, 1095,    0, 1095,    0,    0,    0,    0, 1095, 1095,
    0, 1095,    0, 1108, 1108, 1108, 1108, 1108, 1108, 1108,
    0,    0,    0,    0,    0,    0, 1095, 1095,    0, 1095,
    0, 1108, 1108, 1108, 1108,  557,    0,    0,    0,    0,
 1095,    0, 1095,    0, 1095, 1095, 1095,    0, 1095,    0,
    0, 1095, 1095,    0,    0,    0,  557,    0,    0, 1706,
 1707, 1095,  557, 1108,    0, 1108,    0, 1713, 1714,    0,
 1716, 1717,    0,  557, 1095,    0,    0, 1095, 1721, 1722,
 1723, 1724,    0, 1725, 1726, 1727, 1728, 1729, 1730,    0,
    0,  557,    0, 1095,    0,    0,  557,    0,    0, 1095,
 1095,    0,    0,    0, 1095,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1095, 1095,    0, 1095,    0,
    0,    0,    0,    0, 1095,  557,    0, 1095,    0, 1095,
 1095, 1095, 1095,    0, 1095,    0,    0,    0,    0,    0,
    0,    0, 1095, 1095,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1095, 1095,    0, 1095,    0,    0,    0,
    0,    0,    0, 1095, 1095, 1095, 1095, 1095,    0, 1095,
    0,    0, 1095, 1095, 1095, 1095, 1095,    0,    0,    0,
 1095, 1095,    0,    0,    0, 1095, 1095, 1095, 1095, 1095,
 1095, 1095, 1095,    0, 1095,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1095,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1108,    0, 1108, 1108,    0,    0,
    0,    0,    0, 1108,    0,    0,    0,    0,    0, 1108,
    0,    0,    0,    0, 1108,    0,    0, 1108,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1108, 1113,    0,    0,    0, 1108, 1108,    0,    0,
 1108,    0,    0,    0,    0,    0,    0, 1108,    0, 1108,
 1108,    0,    0, 1108,    0,    0,    0, 1108,    0, 1108,
    0, 1108,    0, 1108,    0,    0,    0,    0, 1108, 1108,
    0, 1108,    0, 1113, 1113, 1113, 1113, 1113, 1113, 1113,
    0,    0,    0,    0,    0,    0, 1108, 1108,    0, 1108,
    0, 1113, 1113, 1113, 1113,    0,    0,    0,    0,    0,
 1108,    0, 1108,    0, 1108, 1108, 1108,    0, 1108,    0,
    0, 1108, 1108,    0,    0,    0,    0,    0,    0,    0,
    0, 1108,    0, 1113,    0, 1113,    0,    0,    0,    0,
    0,    0,    0,    0, 1108,    0,    0, 1108,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1108,    0,    0,    0,    0,    0, 1108,
 1108,    0,    0,    0, 1108,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1108, 1108,    0, 1108,    0,
    0,    0,    0,    0, 1108,    0,    0, 1108,    0, 1108,
 1108, 1108, 1108,    0, 1108,    0,    0,    0,    0,    0,
    0,    0, 1108, 1108,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1108, 1108,    0, 1108,    0,    0,    0,
    0,    0,    0, 1108, 1108, 1108, 1108, 1108,    0, 1108,
    0,    0, 1108, 1108, 1108, 1108, 1108,    0,    0,    0,
 1108, 1108,    0,    0,    0, 1108, 1108, 1108, 1108, 1108,
 1108, 1108, 1108,    0, 1108,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1108,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1113,    0, 1113, 1113,    0,    0,
    0,    0,    0, 1113,    0,    0,    0,    0,    0, 1113,
    0,    0,    0,    0, 1113,    0,    0, 1113,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1113,  919,    0,    0,    0, 1113, 1113,    0,    0,
 1113,    0,    0,    0,    0,    0,    0, 1113,    0, 1113,
 1113,    0,    0, 1113,    0,    0,    0, 1113,    0, 1113,
    0, 1113,    0, 1113,    0,    0,    0,    0, 1113, 1113,
    0, 1113,    0,  919,  919,  919,  919,  919,    0,  919,
    0,    0,    0,    0,    0,    0, 1113, 1113,    0, 1113,
    0,  919,  919,  919,  919,    0,    0,    0,    0,    0,
 1113,    0, 1113,    0, 1113, 1113, 1113,    0, 1113,    0,
    0, 1113, 1113,    0,    0,    0,    0,    0,    0,    0,
    0, 1113,    0,    0,    0,  919,    0,    0,    0,    0,
    0,    0,    0,    0, 1113,    0,    0, 1113,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1113,    0,    0,    0,    0,    0, 1113,
 1113,    0,    0,    0, 1113,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1113, 1113,    0, 1113,    0,
    0,    0,    0,    0, 1113,    0,    0, 1113,    0, 1113,
 1113, 1113, 1113,    0, 1113,    0,    0,    0,    0,    0,
    0,    0, 1113, 1113,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1113, 1113,    0, 1113,    0,    0,    0,
    0,    0,    0, 1113, 1113, 1113, 1113, 1113,    0, 1113,
    0,    0, 1113, 1113, 1113, 1113, 1113,    0,    0,    0,
 1113, 1113,    0,    0,    0, 1113, 1113, 1113, 1113, 1113,
 1113, 1113, 1113,    0, 1113,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1113,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  919,    0,  919,  919,    0,    0,
    0,    0,    0,  919,    0,    0,    0,    0,    0,  919,
    0,    0,    0,    0,  919,    0,    0,  919,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  919,  932,    0,    0,    0,  919,  919,    0,    0,
  919,    0,    0,    0,    0,    0,    0,  919,    0,  919,
  919,    0,    0,  919,    0,    0,    0,  919,    0,  919,
    0,  919,    0,  919,    0,    0,    0,    0,  919,  919,
    0,  919,    0,  932,  932,  932,  932,  932,    0,  932,
    0,    0,    0,    0,    0,    0,  919,  919,    0,  919,
    0,  932,  932,  932,  932,    0,    0,    0,    0,    0,
  919,    0,  919,    0,  919,  919,  919,    0,  919,    0,
    0,  919,  919,    0,    0,    0,    0,    0,    0,    0,
    0,  919,    0,    0,    0,  932,    0,    0,    0,    0,
    0,    0,    0,    0,  919,    0,    0,  919,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  919,    0,    0,    0,    0,    0,  919,
  919,    0,    0,    0,  919,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  919,  919,    0,  919, 1408,
    0,    0,    0,    0,  919,    0,    0,  919,    0,  919,
  919,  919,  919,    0,  919,    0,    0,    0,    0,    0,
    0,    0,  919,  919,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  919,  919,    0,  919,    0,    0,    0,
    0,    0,    0,  919,  919,  919,  919,  919,    0,  919,
    0,    0,  919,  919,  919,  919,  919,    0,    0,    0,
  919,  919,    0,    0,    0,  919,  919,  919,  919,  919,
  919,  919,  919,    0,  919,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  919,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  932,    0,  932,  932,    0,    0,
    0,    0,    0,  932,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  932,    0,    0,  932,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  932,  935,    0,    0,    0,  932,  932,    0,    0,
  932,    0,    0,    0,    0,    0,    0,  932,    0,  932,
  932,    0,    0,  932,    0,    0,    0,  932,    0,  932,
    0,  932,    0,  932,    0,    0,    0,    0,  932,  932,
    0,  932,    0,  935,  935,  935,  935,  935,    0,  935,
    0,    0,    0,    0,    0,    0,  932,  932,    0,  932,
    0,  935,  935,  935,  935,    0,    0,    0,    0,    0,
  932,    0,  932,    0,  932,  932,  932,    0,  932,    0,
    0,  932,  932,    0,    0,    0,    0,    0,    0,    0,
    0,  932,    0,    0,    0,  935,  472,    0,    0,    0,
    0,    0,    0,    0,  932,    0,    0,  932,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  932,  474,    0,    0,    0,    0,  932,
  932,    0,    0,    0,  932,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  932,  932,    0,  932,    0,
    0,    0,  476,    0,  932,    0,    0,  932,    0,  932,
  932,  932,  932,    0,  932,    0,    0,    0,    0,    0,
    0,    0,  932,  932,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  932,  932,    0,  932,    0,    0,    0,
    0,    0,    0,  932,  932,  932,  932,  932,    0,  932,
    0,    0,  932,  932,  932,  932,  932,    0,    0,    0,
  932,  932,    0,    0,    0,  932,  932,  932,  932,  932,
  932,  932,  932,  481,  932,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  932,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,    0,  255,  256,    0,
    0,    0,    0,    0,  935,    0,  935,  935,    0,    0,
    0,    0,    0,  935,    0,    0,  489,  490,  491,  492,
  493,  494,  495,  496,  497,    0,    0,  935,    0,    0,
    0,  499,  500,    0,    0,    0,    0,    0,    0,    0,
  504,  935,  936,    0,    0,    0,  935,  935,    0,    0,
  935,    0,    0,    0,    0,    0,    0,  935,    0,  935,
  935,    0,    0,  935,    0,    0,    0,  935,    0,  935,
    0,  935,    0,  935,    0,    0,    0,    0,  935,  935,
    0,  935,    0,  936,  936,  936,  936,  936,    0,  936,
    0,    0,    0,    0,    0,    0,  935,  935,    0,  935,
    0,  936,  936,  936,  936,    0,    0,    0,    0,    0,
  935,    0,  935,    0,  935,  935,  935,    0,  935,    0,
    0,  935,  935,    0,    0,    0,    0,    0,    0,    0,
    0,  935,    0,    0,    0,  936,    0,    0,    0,    0,
    0,    0,    0,    0,  935,    0,    0,  935,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  935,    0,    0,    0,    0,    0,  935,
  935,    0,    0,    0,  935,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  935,  935,    0,  935,    0,
    0,    0,    0,    0,  935,    0,    0,  935,    0,  935,
  935,  935,  935,    0,  935,    0,    0,    0,    0,    0,
    0,    0,  935,  935,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  935,  935,    0,  935,    0,    0,    0,
    0,    0,    0,  935,  935,  935,  935,  935,    0,  935,
    0,    0,  935,  935,  935,  935,  935,    0,    0,    0,
  935,  935,    0,    0,    0,  935,  935,  935,  935,  935,
  935,  935,  935,    0,  935,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  935,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,    0,  936,  936,    0,    0,
    0,    0,    0,  936,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  936,    0,    0,
    0,  930,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  936,    0,    0,    0,    0,  936,  936,    0,    0,
  936,    0,    0,    0,    0,    0,    0,  936,    0,  936,
  936,    0,    0,  936,    0,    0,    0,  936,    0,  936,
    0,  936,  930,  936,  930,  930,  930,    0,  936,  936,
    0,  936,  990,    0,    0,  694,    0,    0,    0,    0,
  930,  930,  930,  930,    0,    0,  936,  936,  991,  936,
    0,    0,  695,    0,    0,  696,    0,    0,    0,    0,
  936,    0,  936,    0,  936,  936,  936,    0,  936,    0,
    0,  936,  936,    0,  930,    0,    0,    0,    0,    0,
    0,  936,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,  697,    0,  936,    0,    0,
    0,  698,  699,  700,    0,  701,  702,  703,    0,  704,
    0,    0,    0,  936,    0,    0,    0,    0,    0,  936,
  936,    0,    0,    0,  936,    0,    0,  705,    0,  706,
    0,    0,    0,    0,    0,  936,  936,    0,  936,    0,
    0,    0,    0,    0,  936,    0,    0,  936,    0,  936,
  936,  936,  936,    0,  936,    0,    0,    0,  707,    0,
    0,    0,  936,  936,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,  936,  708,  936,    0,    0,    0,
    0,    0,    0,  936,  936,  936,  936,  936,    0,  936,
    0,    0,  936,  936,  936,  936,  936,  709,    0,    0,
  936,  936,    0,    0,    0,  936,  936,  936,  936,  936,
  936,  936,  936,    0,  936,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,    0,    0,    0,    0,    0,
    0,    0,  710,  711,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  930,    0,  930,  930,    0,    0,  712,
    0,    0,  930,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  930,    0,    0,    0,
  929,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  930,    0,    0,    0,    0,  930,  930,    0,    0,  930,
    0,    0,    0,    0,    0,    0,  930,    0,  930,  930,
    0,    0,  930,    0,    0,    0,  930,    0,  930,    0,
  930,  929,  930,  929,  929,  929,    0,  930,  930,    0,
  930,  218,    0,    0,  218,    0,    0,    0,    0,  929,
  929,  929,  929,    0,    0,  930,  930,  218,  930,    0,
    0,  218,    0,    0,  218,    0,    0,    0,    0,  930,
    0,  930,    0,  930,  930,  930,    0,  930,    0,    0,
  930,  930,    0,  929,    0,    0,    0,    0,    0,    0,
  930,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  930,  218,    0,  930,    0,    0,    0,
  218,  218,  218,    0,  218,  218,  218,    0,  218,    0,
    0,    0,  930,    0,    0,    0,    0,    0,  930,  930,
    0,    0,    0,  930,    0,    0,  218,    0,  218,    0,
    0,    0,    0,    0,  930,  930,    0,  930,    0,    0,
    0,    0,    0,  930,    0,    0,  930,    0,  930,  930,
  930,  930,    0,  930,    0,    0,    0,  218,    0,    0,
    0,  930,  930,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  930,  930,  218,  930,    0,    0,    0,    0,
    0,    0,  930,  930,  930,  930,  930,    0,  930,    0,
    0,  930,  930,  930,  930,  930,  218,    0,    0,  930,
  930,    0,    0,    0,  930,  930,  930,  930,  930,  930,
  930,  930,    0,  930,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  930,    0,    0,    0,    0,    0,    0,
    0,  218,  218,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  929,    0,  929,  929,    0,    0,  218,    0,
    0,  929,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  929,    0,    0,    0,  931,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  929,
    0,    0,    0,    0,  929,  929,    0,    0,  929,    0,
    0,    0,    0,    0,    0,  929,    0,  929,  929,    0,
    0,  929,    0,    0,    0,  929,    0,  929,    0,  929,
  931,  929,  931,  931,  931,    0,  929,  929,    0,  929,
    0,    0,    0,    0,    0,    0,    0,    0,  931,  931,
  931,  931,    0,    0,  929,  929,    0,  929,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  929,    0,
  929,    0,  929,  929,  929,    0,  929,    0,    0,  929,
  929,    0,  931,    0,    0,    0,    0,    0,    0,  929,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  929,    0,    0,  929,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  929,    0,    0,    0,    0,    0,  929,  929,    0,
    0,    0,  929,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  929,  929,    0,  929,    0,    0,    0,
    0,    0,  929,    0,    0,  929,    0,  929,  929,  929,
  929,    0,  929,    0,    0,    0,    0,    0,    0,    0,
  929,  929,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  929,  929,    0,  929,    0,    0,    0,    0,    0,
    0,  929,  929,  929,  929,  929,    0,  929,    0,  870,
  929,  929,  929,  929,  929,    0,    0,    0,  929,  929,
    0,    0,    0,  929,  929,  929,  929,  929,  929,  929,
  929,    0,  929,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  929,    0,    0,    0,    0,    0,    0,    0,
  870,    0,    0,  870,    0,    0,    0,    0,    0,    0,
    0,  931,    0,  931,  931,    0,    0,    0,  870,    0,
  931,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  931,    0,    0,    0,  934,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  931,    0,
    0,    0,    0,  931,  931,    0,    0,  931,    0,    0,
    0,    0,    0,    0,  931,    0,  931,  931,    0,    0,
  931,    0,    0,    0,  931,    0,  931,    0,  931,  934,
  931,  934,  934,  934,    0,  931,  931,    0,  931,    0,
    0,    0,    0,    0,    0,    0,    0,  934,  934,  934,
  934,    0,    0,  931,  931,    0,  931,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  931,    0,  931,
    0,  931,  931,  931,    0,  931,    0,    0,  931,  931,
    0,  934,    0,    0,    0,    0,    0,    0,  931,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  931,    0,    0,  931,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  931,    0,    0,    0,    0,    0,  931,  931,    0,    0,
 1090,  931,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  931,  931,    0,  931,    0,    0,  516,    0,
    0,  931,    0,    0,  931,    0,  931,  931,  931,  931,
    0,  931,    0,    0,    0,    0,    0,    0,    0,  931,
  931,  870,    0,    0,    0,    0,    0,    0,    0,    0,
  931,  931,    0,  931,    0,    0,    0,    0,    0,    0,
  931,  931,  931,  931,  931,    0,  931,  737,    0,  931,
  931,  931,  931,  931,    0,    0,    0,  931,  931,    0,
    0,    0,  931,  931,  931,  931,  931,  931,  931,  931,
    0,  931,    0,    0,  870,    0,    0,  870,    0,    0,
    0,  931,    0,    0,  870,    0,  870,    0,  737,    0,
  870,  737,    0,    0,    0,  870,    0,    0,  870,    0,
  934,    0,  934,  934,    0,    0,  737,    0,    0,  934,
    0,    0,    0,  870,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  934,    0,    0,    0,  870,    0,    0,
    0,    0,    0,    0,    0,  870,    0,  934,  870,  870,
    0,    0,  934,  934,    0,    0,  934,    0,  870,    0,
    0,    0,    0,  934,    0,  934,  934,    0,    0,  934,
    0,    0,    0,  934,  870,  934,    0,  934,    0,  934,
    0,    0,    0,    0,  934,  934,    0,  934,    0,    0,
    0,    0,    0,    0,    0,    0,  870,    0,    0,    0,
    0,  870,  934,  934,    0,  934,    0,    0,    0,    0,
    0,    0,    0,  870,    0,  870,  934,    0,  934,    0,
  934,  934,  934,    0,  934,    0,    0,  934,  934, 1097,
    0,    0,    0,    0,    0,    0,    0,  934,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  934,    0,    0,  934,    0,    0,    0,    0,    0,    0,
    0,    0,  870,    0,    0,    0,    0,    0,    0,  934,
    0,    0,  708,    0,    0,  934,  934,    0,    0,    0,
  934,    0,  870,    0,    0,    0,  870,    0,    0,    0,
    0,  934,  934,    0,  934,    0,    0,    0,    0,    0,
  934,    0,    0,  934,    0,  934,  934,  934,  934,    0,
  934,    0,  708,  708,    0,    0,  708,    0,  934,  934,
    0,  737,    0,    0,    0,    0,    0,    0,    0,  934,
  934,  708,  934,    0,    0,    0,    0,    0,    0,  934,
  934,  934,  934,  934,    0,  934,    0,    0,  934,  934,
  934,  934,  934,    0,    0,    0,  934,  934,    0,    0,
    0,  934,  934,  934,  934,  934,  934,  934,  934,    0,
  934,    0,  737,    0,    0,  737,    0,    0,    0,    0,
  934,    0,  737,    0,  737,    0,    0,    0,  737,    0,
    0,    0,    0,  737,    0,    0,  737,    0,    0,    0,
    0,    0,  115,    0,    0,  116,    0,  117,  118,  119,
  120,  737,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,    0,    0,    0,  737,    0,    0,    0,    0,
    0,  124,  125,  737,  126,    0, 1124,  737,    0,    0,
    0,  127,  128,    0,  129,  130,  737,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,  737,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
 1096,  735,  146,    0,    0,    0,    0,    0,    0,  737,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  737,    0,  737,    0,    0,    0,    0,    0,  737,
    0,    0,  737,    0,  737,  737,  737,  737,    0,  737,
    0,    0,  735,    0,    0,  735,    0,  737,  737,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  737,  737,
  735,  737,    0,    0,    0,    0,    0,    0,  737,  737,
  737,  737,  737,    0,  737,    0,    0,  737,  737,  737,
  737,  737,    0,    0,    0,  737,  737,    0,    0,    0,
  737,  737,  737,  737,  737,  737,  737,  737,    0,  737,
    0,    0,    0,    0,    0,    0,    0,  708,    0,  737,
  708,    0,    0,    0,    0,    0,    0,  708,    0,  708,
    0,    0,    0,  708,    0,    0,    0,    0,  708,    0,
    0,  708,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  708,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  115,
  708,    0,  116,    0,  117,  118,  119,  120,  708,  121,
    0,    0,  708,    0,    0,    0,    0,  122,  123,    0,
    0,  708,    0,    0,    0,    0,    0,    0,  124,  125,
    0,  126,    0,    0,    0,    0,    0,  708,  127,  128,
    0,  129,  130,    0,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,  708,  142,  143,  144,    0,  145,
    0,    0,    0,    0,    0,    0,  708,    0,  708,  146,
    0,    0,    0,    0,  708,    0,    0,  708,    0,  708,
  708,  708,  708,    0,  708,    0,    0,    0,    0,    0,
    0,    0,  708,  708,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  708,  708,    0,  708,    0,    0,    0,
    0,    0,    0,  708,  708,  708,  708,  708,    0,  708,
    0,    0,  708,  708,  708,  708,  708,    0,    0,    0,
  708,  708,  911,    0,    0,  708,  708,  708,  708,  708,
  708,  708,  708,    0,  708,    0,  735,    0,    0,  735,
    0,    0,    0,    0,  708,   14,  735,    0,  735,    0,
    0,    0,  735,    0,    0,    0,    0,  735,    0,    0,
  735,    0,    0,  911, 1103, 1103,  911, 1103, 1103, 1103,
    0,    0,    0,    0,    0,  735,    0,    0,    0,    0,
    0,  911, 1103, 1103, 1103,    0,    0,    0,  115,  735,
    0,  116,    0,  117,  118,  119,  120,  735,  121,    0,
    0,  735,    0,    0,    0,    0,  122,  123,    0,    0,
  735,    0,    0, 1103,    0,    0,    0,  124,  125,    0,
  126,    0,    0,    0,    0,    0,  735,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,  735,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,  735,    0,  735,  146,    0,
  375,    0,    0,  708,    0,    0,  708,    0,  708,  708,
  708,  708,    0,  708,    0,    0,  376,    0,    0,    0,
    0,  708,  708,    0,    0,    0,    0,  377,    0,    0,
    0,    0,  708,  708,  378,  708,    0,    0,  379,  380,
    0,    0,  708,  708,  735,  708,  708,  381,  708,  912,
    0,  708,  708,  708,  708,  708,    0,    0,  382,  708,
  708,    0,    0,    0,  735,  708,  708,  708,  735,  708,
  708,  708,    0,  708,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  708,  818,  816,    0,  817,    0,  819,
  912, 1104, 1104,  912, 1104, 1104, 1104,    0,    0,    0,
    0,    0, 1022, 1021, 1023,    0,    0,    0,  912, 1104,
 1104, 1104,  383,    0,  911,    0,    0,    0,    0,    0,
  384,  902,    0, 1103,    0,    0,    0,    0,    0, 1103,
    0,    0,    0,    0, 1103,    0,    0, 1103,    0,    0,
 1104,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  385,    0,    0,    0,  386,    0,    0,    0,    0,
    0,    0,  902,    0,    0,  902,  387,  911,    0,    0,
  911,    0,    0, 1103,    0,    0,    0,  911,    0,  911,
  902, 1103,    0,  911,    0,  388,  389,    0,  911, 1103,
    0,  911,    0,  864,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  911, 1103,    0, 1103,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  911,    0, 1103,    0, 1103, 1103, 1103,    0,  911,    0,
    0,  911,  911,    0,  864,    0,    0,  864,    0,    0,
    0,  911,    0,    0,  390,    0,    0,  391,    0,    0,
    0,    0,  864,  865,    0,    0,    0,  911,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1103,    0,    0,    0,    0,    0,  911,
    0,    0,    0,    0,  911,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  865,    0,  911,  865,  911,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  912,  865, 1009,    0,    0,    0,    0,    0,  814,
 1104,    0,    0,    0,  815,    0, 1104, 1010,    0,    0,
    0, 1104,    0,    0, 1104,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  911,    0,    0,  866,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1011,  912,  911,    0,  912,    0,  911,
 1104, 1012,    0,    0,  912,    0,  912,    0, 1104, 1013,
  912,    0,    0,  902,    0,  912, 1104,    0,  912,  866,
    0,    0,  866,    0,    0,    0,    0, 1014,    0, 1015,
    0,    0,    0,  912, 1104,    0, 1104,  866,    0,    0,
    0,    0, 1016,    0, 1017, 1018, 1019,  912,    0, 1104,
  901, 1104, 1104, 1104,    0,  912,    0,    0,  912,  912,
    0,    0,    0,    0,    0,    0,  902,    0,  912,  902,
    0,    0,    0,    0,    0,    0,  902,    0,  902,    0,
    0,    0,  902,    0,  912,  864,    0,  902,    0,    0,
  902,  901,    0, 1020,  901,    0,    0,    0,    0,    0,
 1104,    0,    0,    0,    0,  902,  912,    0,    0,  901,
    0,  912,    0,    0,    0,    0,    0,    0,    0,  902,
    0,    0,  895,  912,    0,  912,    0,  902,    0,    0,
  902,  902,    0,    0,    0,    0,    0,    0,  864,    0,
  902,  864,    0,    0,    0,  865,    0,    0,  864,    0,
  864,    0,    0,    0,  864,    0,  902,    0,    0,  864,
    0,    0,  864,  895,    0,    0,  895,    0,    0,    0,
    0,    0,  912,    0,    0,    0,    0,  864,  902,    0,
    0,  895,  903,  902,    0,    0,    0,    0,    0,    0,
    0,  864,  912,    0,    0,  902,  912,  902,  865,  864,
    0,  865,  864,  864,    0,    0,    0,    0,  865,    0,
  865,    0,  864,    0,  865,    0,    0,    0,    0,  865,
    0,    0,  865,  903,    0,    0,  903,    0,  864,    0,
    0,    0,    0,    0,    0,    0,    0,  865,    0,    0,
  866,  903,    0,    0,  902,    0,    0,    0,    0,    0,
  864,  865,    0,    0,  904,  864,    0,    0,    0,  865,
    0,    0,  865,  865,  902,    0,    0,  864,  902,  864,
    0,    0,  865,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  865,    0,
    0,    0,    0,  866,    0,  904,  866,    0,  904,    0,
    0,    0,    0,  866,    0,  866,    0,    0,    0,  866,
  865,    0,  901,  904,  866,  865,  864,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  865,    0,  865,
    0,    0,  866,    0,    0,    0,  864,    0,  892,    0,
  864,    0,    0,    0,    0,    0,  866,    0,    0,    0,
    0,    0,    0,    0,  866,    0,    0,  866,  866,    0,
    0,    0,    0,    0,    0,  901,    0,  866,  901,    0,
    0,    0,    0,    0,    0,  901,  865,  901,    0,  892,
    0,  901,  892,  866,  895,    0,  901,    0,    0,  901,
    0,    0,    0,    0,    0,    0,  865,  892,    0,    0,
  865,    0,    0,    0,  901,  866,    0,    0,    0,    0,
  866,    0,    0,    0,    0,    0,    0,    0,  901,    0,
    0,  890,  866,    0,  866,    0,  901,    0,    0,  901,
  901,    0,    0,    0,    0,    0,    0,  895,    0,  901,
  895,    0,    0,    0,  903,    0,    0,  895,    0,  895,
    0,    0,    0,  895,    0,  901,    0,    0,  895,    0,
    0,  895,  890,    0,    0,  890,    0,    0,    0,    0,
    0,  866,    0,    0,    0,    0,  895,  901,    0,    0,
  890,    0,  901,    0,    0,    0,    0,    0,    0,    0,
  895,  866,    0,  896,  901,  866,  901,  903,  895,    0,
  903,  895,  895,    0,    0,    0,    0,  903,    0,  903,
    0,  895,    0,  903,    0,    0,  904,    0,  903,    0,
    0,  903,    0,    0,    0,    0,    0,  895,    0,    0,
    0,    0,    0,    0,  896,    0,  903,  896,    0,    0,
    0,    0,    0,  901,    0,    0,    0,    0,    0,  895,
  903,    0,  896,  905,  895,    0,    0,    0,  903,    0,
    0,  903,  903,  901,    0,    0,  895,  901,  895,  904,
    0,  903,  904,    0,    0,    0,    0,    0,    0,  904,
    0,  904,    0,    0,    0,  904,    0,  903,    0,    0,
  904,    0,    0,  904,  905,    0,    0,  905,    0,    0,
  892,    0,    0,    0,    0,    0,    0,    0,  904,  903,
    0,    0,  905,    0,  903,  895,    0,    0,    0,    0,
    0,    0,  904,    0,    0,    0,  903,    0,  903,    0,
  904,    0,    0,  904,  904,  895,    0,    0,    0,  895,
    0,    0,    0,  904,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  892,    0,    0,  892,    0,    0,  904,
    0,    0,    0,  892,  891,  892,    0,    0,    0,  892,
    0,    0,    0,    0,  892,  903,    0,  892,    0,    0,
    0,  904,    0,  890,    0,    0,  904,    0,    0,    0,
    0,    0,  892,    0,    0,  903,    0,  893,  904,  903,
  904,    0,    0,    0,    0,  891,  892,    0,  891,    0,
    0,    0,    0,    0,  892,    0,    0,  892,  892,    0,
    0,    0,    0,  891,    0,    0,    0,  892,    0,    0,
    0,    0,    0,    0,    0,    0,  890,    0,  893,  890,
    0,  893,    0,  892,    0,    0,  890,  904,  890,    0,
    0,    0,  890,    0,    0,  896,  893,  890,    0,    0,
  890,    0,    0,    0,    0,  892,    0,  904,    0,    0,
  892,  904,    0,    0,    0,  890,    0,    0,    0,    0,
    0,    0,  892,    0,  892,    0,  897,    0,    0,  890,
    0,    0,    0,    0,    0,    0,    0,  890,    0,    0,
  890,  890,    0,    0,    0,    0,    0,    0,  896,    0,
  890,  896,    0,    0,    0,  905,    0,    0,  896,    0,
  896,    0,    0,    0,  896,    0,  890,  897,    0,  896,
  897,  892,  896,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  897,    0,  896,  890,  732,
    0,  892,    0,  890,    0,  892,    0,    0,    0,    0,
    0,  896,    0,    0,    0,  890,    0,  890,  905,  896,
    0,  905,  896,  896,    0,    0,    0,    0,  905,    0,
  905,    0,  896,    0,  905,    0,    0,    0,    0,  905,
  732,    0,  905,  732,    0,    0,    0,    0,  896,    0,
    0,    0,    0,    0,    0,    0,    0,  905,  732,    0,
    0,    0,    0,    0,  890,    0,    0,    0,    0,    0,
  896,  905,    0,    0,    0,  896,  891,    0,    0,  905,
    0,    0,  905,  905,  890,    0,    0,  896,  890,  896,
    0,    0,  905, 1076,  818,  816,    0,  817,    0,  819,
    0,    0,    0,    0,    0,    0,    0,    0,  905,  893,
  313,    0, 1022, 1021, 1023,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  891,
  905,    0,  891,    0,    0,  905,  896,    0,    0,  891,
    0,  891,    0,    0,    0,  891,    0,  905,    0,  905,
  891,    0,    0,  891,    0,    0,  896,    0,    0,    0,
  896,    0,  893,    0,    0,  893,    0,    0,  891,    0,
    0,    0,  893,    0,  893,    0,    0,    0,  893,    0,
    0,    0,  891,  893,    0,    0,  893,    0,    0,    0,
  891,    0,    0,  891,  891,    0,  905,    0,  897,    0,
    0,  893,    0,  891,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  893,  905,    0,    0,  891,
  905,    0,    0,  893,    0,  759,  893,  893,  513,    0,
  514,    0,    0,    0,    0,    0,  893,    0,    0,    0,
    0,  891,    0,  516,    0,    0,  891,    0,  334,    0,
    0,  897,  893,    0,  897,    0,    0,    0,  891,    0,
  891,  897,    0,  897,    0,    0,    0,  897,    0,    0,
    0,    0,  897,    0,  893,  897,    0,    0,    0,  893,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  897,  893,    0,  893,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  897,    0,    0,  891,    0,    0,
    0,    0,  897,    0,  732,  897,  897,  732,    0,    0,
    0,    0,    0, 1009,  732,  897,  732,  891,    0,  814,
  732,  891,    0,    0,  815,  732,    0, 1010,  732,    0,
  893,  897,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  912,    0,  732,    0, 1124,    0,    0,    0,    0,
  893,    0,    0,  897,  893,    0,    0,  732,  897,    0,
    0,    0,    0, 1011,    0,  732,    0,    0,    0,  732,
  897, 1012,  897,    0,    0,    0,    0,    0,  732, 1013,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  732,    0,    0, 1014,    0, 1015,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1016,    0, 1017, 1018, 1019,    0,    0,  897,
    0,  732,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  732,    0,  732,    0,    0,    0,  897,
    0,    0,  469,  897,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0, 1020,    0,    0,    0,  472,  473,    0,
    0,  759,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,  732,    0,    0,    0,  753,    0,    0,  516,
    0,    0,    0,    0,  334,  474,    0,    0,  475,    0,
    0,    0,  732,    0,    0,    0,  732,    0,    0,    0,
    0,    0,  115,    0,    0,  116,    0,  117,  118,  119,
  120,    0,  121,  476,    0,  477,  478,    0,    0,    0,
  122,  123,    0,    0,    0,  479,    0,    0,    0,  480,
    0,  124,  125,    0,  126,    0,    0, 1001,    0,    0,
    0,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,   14,    0,    0,    0,  755,    0,
    0,    0,  146,    0,  481,    0,    0,    0,    0,    0,
  482,    0,    0, 1097,    0,    0,    0,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,    0,  499,  500,    0,  501,  502,  503,    0,  129,
  130,  504,  131,    0,    0,  132,  133,  134,  135,  136,
 1002, 1003, 1004,  137,  138,    0,    0,    0,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,  469,  507,
  508,  509,    0,  510,  511,  470,  512,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,    0,
  752,    0,    0,  472,  473,    0,    0,  759,    0,    0,
  513,    0,  514,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  753,    0,    0,  516,    0,    0,    0,    0,
  334,  474,    0,    0,  475,    0,    0,  115,    0,    0,
  116,    0,  117,  118,  119,  120,    0,  121,    0,    0,
    0,    0,    0,    0,    0,  122,  123,    0,    0,  476,
    0,  477,  478,    0,    0,    0,  124,  125,    0,  126,
    0,  479,    0,    0,    0,  480,  127,  128,    0,  129,
  130,    0,  131,  754,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  140,  141,    0,  142,  143,  144,    0,  145,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  146,    0,    0,
    0,    0,    0,    0,  755,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,  705,    0,    0,    0,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0,    0,    0,    0,
    0,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,    0,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  756,  757,  758,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144,    0,  145,  469,  507,  508,  509,    0,  510,
  511,  470,  512,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  471,    0,    0,    0,    0,    0,  472,
  473,    0,    0,  759,    0,    0,  513,    0,  514,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  753,    0,
    0,  516,    0,    0,    0,    0,  334,  474,    0,    0,
  475,    0,    0,  115,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,    0,    0,  476,    0,  477,  478,    0,
    0,    0,  124,  125,    0,  126,    0,  479,    0,    0,
    0,  480,  127,  128,    0,  129,  130,    0,  131,  997,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,    0,    0,    0,    0,    0,    0,
  755,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,   85,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  705,    0,  484,  485,  486,  487,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  488,    0,    0,    0,    0,    0,    0,  122,  123,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  124,  498,
    0,  126,    0,    0,  499,  500,    0,  501,  502,  503,
    0,  129,  130,  504,  131,    0,    0,  132,  133,  756,
  757,  758,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  505,  141,    0,  506,  143,  144,    0,  145,
  469,  507,  508,  509,    0,  510,  511,  470,  512,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,    0,    0,    0,  472,  473,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  753,    0,    0,  516,    0,    0,
    0,    0,  334,  474,    0,    0,  475,    0,    0,  708,
    0,    0,  708,    0,  708,  708,  708,  708,    0,  708,
    0,    0,    0,    0,    0,    0,    0,  708,  708,    0,
    0,  476,    0,  477,  478,    0,    0,    0,  708,  708,
    0,  708,    0,  479,    0,    0,    0,  480,  708,  708,
    0,  708,  708,    0,  708,  754,    0,  708,  708,  708,
  708,  708,    0,    0,    0,  708,  708,    0,    0,    0,
    0,  708,  708,  708,    0,  708,  708,  708,    0,  708,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  708,
    0,    0,    0,    0,    0,    0,  755,    0,    0,    0,
    0,    0,  481,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  483,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  484,  485,  486,  487,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  488,  516,    0,    0,
    0,    0,    0,  122,  123,  489,  490,  491,  492,  493,
  494,  495,  496,  497,  124,  498,    0,  126,    0,    0,
  499,  500,    0,  501,  502,  503,    0,  129,  130,  504,
  131,    0,    0,  132,  133,  756,  757,  758,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  505,  141,
    0,  506,  143,  144,    0,  145,  469,  507,  508,  509,
    0,  510,  511,  470,  512,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,    0,    0,    0,
    0,  472,  473,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  759,    0,    0,  513,    0,
  514,    0,    0,    0,    0,    0,    0,    0,    0,  474,
    0,    0,  475,  516,   84,    0,  115,    0,  334,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,    0,
    0,    0,    0,    0,  122,  123,    0,  476,    0,  477,
  478,    0,    0,    0,    0,  124,  125,    0,  126,  479,
    0,    0,    0,  480,    0,  127,  128,    0,  129,  130,
    0,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  146,    0,    0,    0,
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
 1071,  145,    0,  507,  508,  509, 1072,  510,  511, 1073,
  512,  146,  469,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,    0,  472,  473,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  586,    0,    0,  513,  753,  514,    0,    0,
    0,    0,    0,    0,    0,  474,    0,    0,  475,    0,
  516,    0,    0,    0,    0,  334,    0,    0,    0,    0,
    0,  115,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,  476,    0,  477,  478,    0,    0,  122,
  123,    0,    0,    0,    0,  479,    0,    0,    0,  480,
  124,  125,    0,  126,    0,    0,    0,  997,    0,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,  755,    0,
    0,  146,    0,    0,  481,    0,    0,    0,    0,    0,
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
  508,  509, 1259,  510,  511, 1260,  512,  146,    0,  469,
  218,    0,    0,  218,    0,    0,  470,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  218,    0,  471,    0,
  218,    0,    0,  218,  472,  473,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,  474,    0,    0,  475,    0,  516,    0,    0,
    0,    0,  334,  218,    0,    0,    0,    0,    0,  218,
  218,  218,    0,  218,  218,  218,    0,  218,    0,    0,
  476,    0,  477,  478,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,    0,  218,  480,  218,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  218,    0,    0,    0,
    0,   81,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  218,    0,    0,    0,    0,    0, 1261,
    0,  481,    0,    0,    0,    0,    0,  482,    0,    0,
    0,    0,    0,    0,    0,  218,    0,  483,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,  485,  486,  487,  255,  256,  116,    0,  117,
  118,  119,  120,    0,  121,  488,    0,    0,    0,    0,
  218,  218,  122,  123,  489,  490,  491,  492,  493,  494,
  495,  496,  497,  124,  498,    0,  126,  218,    0,  499,
  500,    0,  501,  502,  503,    0,  129,  130,  504,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  505,  141,    0,
  506,  143,  144,    0,  145,    0,  507,  508,  509, 1270,
  510,  511, 1260,  512,  146,    0,  469,  693,    0,    0,
  694,    0,    0,  470,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,  695,    0,    0,
  696,  472,  473,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  586,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,  474,
    0,    0,  475,    0,  516,    0,    0,    0,    0,  334,
  697,    0,    0,    0,    0,    0,  698,  699,  700,    0,
  701,  702,  703,    0,  704,    0,    0,  476,    0,  477,
  478,    0,    0,    0,    0,    0,    0,    0,    0,  479,
    0,    0,  705,  480,  706,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  707,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  708,    0,    0,    0,    0,    0, 1261,    0,  481,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,  709,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,  710,  711,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,  712,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,    0,  507,  508,  509, 1274,  510,  511, 1260,
  512,  146,    0,  469,  148,    0,    0,  148,    0,    0,
  470,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,  148,    0,    0,  148,  472,  473,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  586,    0,    0,  513,    0,  514,    0,
    0,    0,    0,    0,    0,    0,  474,    0,    0,  475,
    0,  516,    0,    0,    0,    0,  334,  148,    0,    0,
    0,    0,    0,  148,  148,  148,    0,  148,  148,  148,
    0,  148,    0,    0,  476,    0,  477,  478,    0,    0,
    0,    0,    0,    0,    0,    0,  479,    0,    0,  148,
  480,  148,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  148,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  148,    0,    0,
    0,    0,    0, 1261,    0,  481,    0,    0,    0,    0,
    0,  482,    0,    0,    0,    0,    0,    0,    0,  148,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,  487,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  488,
    0,    0, 1355,    0,  148,  148,  122,  123,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  124,  498,    0,
  126,  148,    0,  499,  500,    0,  501,  502,  503,    0,
  129,  130,  504,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  505,  141,    0,  506,  143,  144,    0,  145,    0,
  507,  508,  509, 1282,  510,  511, 1260,  512,  146,    0,
  469,    0, 1356,    0,    0,    0,    0,  470,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,    0,    0,    0,  472,  473,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1357,    0,    0,
  586,    0, 1358,  513,    0,  514,    0,    0, 1359,    0,
    0,    0,    0,  474,    0,    0,  475,    0,  516,  115,
    0,    0,  116,  334,  117,  118,  119, 1360,    0,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123,    0,
    0,  476,    0,  477,  478,    0,    0,    0,  124,  125,
    0,  126,    0,  479,    0,    0,    0,  480,  127,  128,
    0,  129,  130,    0,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,    0,  142,  143,  144,    0,  145,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1261,    0,  481,    0,    0,    0,    0,    0,  482,    0,
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
 1285,  510,  511, 1260,  512,  146,    0,  469,    0,    0,
    0,    0,    0,    0,  470,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  471,    0,    0,    0,
    0,    0,  472,  473,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  586,    0,  567,
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
    0,    0,    0,    0,    0,    0,    0, 1261,    0,  481,
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
  144,    0,  145,    0,  507,  508,  509, 1290,  510,  511,
 1260,  512,  146,    0,  469,    0,    0,    0,    0,    0,
    0,  470,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  471,    0,    0,    0,    0,    0,  472,
  473,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  586,    0,    0,  513,    0,  514,
    0,    0,    0,    0,    0,    0,    0,  474,    0,    0,
  475,    0,  516,  115,    0,    0,  116,  334,  117,  118,
  119,  120,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,    0,    0,  476,    0,  477,  478,    0,
    0,    0,  124,  125,    0,  126,    0,  479,    0,    0,
    0,  480,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1261,    0,  481,    0,    0,    0,
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
    0,  507,  508,  509, 1293,  510,  511, 1260,  512,  146,
    0,  469,    0,    0,    0,    0,    0,    0,  470,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,    0,    0,    0,    0,  472,  473,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  586,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,    0,    0,  474,    0,    0,  475,    0,  516,
   28,    0,    0,   28,  334,   28,   28,   28,   28,    0,
   28,    0,    0,    0,    0,    0,    0,    0,   28,   28,
    0,    0,  476,    0,  477,  478,    0,    0,    0,   28,
   28,    0,   28,    0,  479,    0,    0,    0,  480,   28,
   28,    0,   28,   28,    0,   28,    0,    0,   28,   28,
   28,   28,   28,    0,    0,    0,   28,   28,    0,    0,
    0,    0,   28,   28,   28,    0,   28,   28,   28,    0,
   28,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   28,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1261,    0,  481,    0,    0,    0,    0,    0,  482,
  661,    0,    0,    0,    0,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,    0,  507,  508,
  509, 1296,  510,  511, 1260,  512,  146,    0,  469,    0,
    0,    0,    0,    0,    0,  470,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  471,    0,    0,
    0,    0,    0,  472,  473,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  692,    0,  692,  692,    0,  692,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  474,  692,    0,  475,    0,    0,  692,  115,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,  476,
    0,  477,  478,    0,    0,    0,    0,  124,  125,    0,
  126,  479,    0,    0,    0,  480,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1261,    0,
  481,    0,    0,  667,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  483,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  484,  485,  486,  487,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  488,    0,    0,    0,    0,    0,
    0,  122,  123,  489,  490,  491,  492,  493,  494,  495,
  496,  497,  124,  498,    0,  126,    0,    0,  499,  500,
    0,  501,  502,  503,    0,  129,  130,  504,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  505,  141,    0,  506,
  143,  144,    0,  145,  692,  507,  508,  509,    0,  510,
  511,  692,  512,  146,    0,    0,    0,    0,  692,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  692,    0,    0,    0,    0,    0,  692,  692,    0,    0,
    0,    0,    0,    0,  692,    0,    0,  686,    0,  686,
  686,    0,  686,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  692,  686,    0,  692,    0,    0,
  686,  115,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,    0,    0,    0,    0,    0,    0,  122,
  123,    0,  692,    0,  692,  692,    0,    0,    0,    0,
  124,  125,    0,  126,  692,    0,    0,    0,  692,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  692,    0,    0,    0,    0,    0,  692,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  692,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  692,  692,  692,  692,  692,  692,  692,
    0,  692,  692,  692,  692,    0,  692,  692,    0,    0,
    0,    0,    0,    0,  692,  692,  692,  692,  692,  692,
  692,  692,  692,  692,  692,  692,  692,    0,  692,    0,
    0,  692,  692,    0,  692,  692,  692,    0,  692,  692,
  692,  692,    0,    0,  692,  692,  692,  692,  692,    0,
    0,    0,  692,  692,    0,    0,    0,    0,  692,  692,
  692,    0,  692,  692,  692,    0,  692,  686,  692,  692,
  692,    0,  692,  692,  686,  692,  692,    0,    0,    0,
    0,  686,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  686,    0,    0,    0,    0,    0,  686,
  686,    0,    0,  586,    0,  591,  513,  686,  514,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  516,    0,    0,    0,    0,  334,  686,    0,    0,
  686,    0,    0,  600,    0,    0,  600,    0,  600,  600,
  600,  600,    0,  600,    0,    0,    0,    0,    0,    0,
    0,  600,  600,    0,    0,  686,    0,  686,  686,    0,
    0,    0,  600,  600,    0,  600,    0,  686,    0,    0,
    0,  686,  600,  600,    0,  600,  600,    0,  600,    0,
    0,  600,  600,  600,  600,  600,    0,    0,    0,  600,
  600,    0,    0,    0,    0,  600,  600,  600,    0,  600,
  600,  600,    0,  600,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  600,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  686,    0,    0,    0,
    0,    0,  686,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  686,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  686,  686,  686,  686,
  686,  686,  686,    0,  686,  686,  686,  686,    0,  686,
  686,    0,    0,    0,    0,    0,    0,  686,  686,  686,
  686,  686,  686,  686,  686,  686,  686,  686,  686,  686,
    0,  686,    0,    0,  686,  686,    0,  686,  686,  686,
    0,  686,  686,  686,  686,    0,    0,  686,  686,  686,
  686,  686,    0,    0,    0,  686,  686,    0,    0,    0,
    0,  686,  686,  686,    0,  686,  686,  686,    0,  686,
  469,  686,  686,  686,    0,  686,  686,  470,  686,  686,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,    0,    0,    0,  472,  473,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  516,    0,    0,
    0,    0,  334,  474,    0,    0,  475,    0,    0,  287,
    0,    0,  287,    0,  287,  287,  287,  287,    0,  287,
    0,    0,    0,    0,    0,    0,    0,  287,  287,    0,
    0,  476,    0,  477,  478,    0,    0,    0,  287,  287,
    0,  287,    0,  479,    0,    0,    0,  480,  287,  287,
    0,  287,  287,    0,  287,    0,    0,  287,  287,  287,
  287,  287,    0,    0,    0,  287,  287,    0,    0,    0,
    0,  287,  287,  287,    0,  287,  287,  287,    0,  287,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  287,
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
    0,    0,  791,    0,    0,    0,    0,    0,  484,  485,
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
    0,  586, 1080,    0,  513,    0,  514,    0,    0,    0,
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
    0,    0,    0,    0,   14,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
  473,    0,    0,  586,    0,    0,  513,    0,  514,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1288,    0,    0,  484,  485,  486,  487,
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
 1340,    0,  334,  474,    0,    0,  475,    0,    0,    0,
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
    0,  472,  473,    0,    0, 1055,    0, 1055, 1055,    0,
 1055,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1055,    0,    0,    0,    0, 1055,  474,
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
    0,    0,    0,    0,    0, 1507,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145, 1055,  507,  508,  509,    0,  510,  511, 1055,
  512,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1055,    0,    0,    0,    0,    0, 1055, 1055,    0,
    0,  515,    0,    0,  513,    0,  514,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  516,
    0,    0,    0,    0,  334, 1055,    0,    0, 1055,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1055,    0, 1055, 1055,    0,    0,    0,
    0,    0,    0,    0,    0, 1055,    0,    0,    0, 1055,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1055,    0,    0,    0,    0,    0,
 1055,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1055,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1055, 1055, 1055, 1055, 1055, 1055,
 1055,    0, 1055, 1055, 1055, 1055,    0, 1055, 1055,    0,
    0,    0,    0,    0,    0, 1055, 1055, 1055, 1055, 1055,
 1055, 1055, 1055, 1055, 1055, 1055, 1055, 1055,    0, 1055,
    0,    0, 1055, 1055,    0, 1055, 1055, 1055,    0, 1055,
 1055, 1055, 1055,    0,    0, 1055, 1055, 1055, 1055, 1055,
    0,    0,    0, 1055, 1055,    0,    0,    0,    0, 1055,
 1055, 1055,    0, 1055, 1055, 1055,    0, 1055,  469, 1055,
 1055, 1055,    0, 1055, 1055,  470, 1055, 1055,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
  473,    0,    0, 1055,    0,    0, 1055,    0, 1055,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1055,    0,    0,    0,    0, 1055,  474,    0,    0,
  475,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  476,    0,  477,  478,    0,
    0,    0,    0,    0,    0,    0,    0,  479,    0,    0,
    0,  480,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
    0,  139,  505,  141,    0,  506,  143,  144,  567,  145,
 1055,  507,  508,  509,    0,  510,  511, 1055,  512,  146,
  568,    0,    0,    0,    0,    0,    0,    0,    0, 1055,
    0,    0,    0,    0,    0, 1055, 1055,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  567,  567,
    0,    0,  567,    0,    0,    0,    0,  562,    0,    0,
  568,  568,    0, 1055,  568,    0, 1055,  567,    0,  567,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  568,
    0,  568,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1055,    0, 1055, 1055,    0,    0,    0,  562,  567,
    0,  562,    0, 1055,    0,    0,    0, 1055,    0,    0,
    0,  568,    0,    0,    0,    0,  562,    0,  562,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  562,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1055,    0,    0,    0,    0,    0, 1055,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1055,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1055, 1055, 1055, 1055, 1055, 1055, 1055,    0,
 1055, 1055, 1055, 1055,    0, 1055, 1055,    0,    0,    0,
    0,    0,    0, 1055, 1055, 1055, 1055, 1055, 1055, 1055,
 1055, 1055, 1055, 1055, 1055, 1055,    0, 1055,    0,  584,
 1055, 1055,    0, 1055, 1055, 1055,    0, 1055, 1055, 1055,
 1055,    0,    0, 1055, 1055, 1055, 1055, 1055,    0,    0,
    0, 1055, 1055,    0,    0,    0,    0, 1055, 1055, 1055,
    0, 1055, 1055, 1055,    0, 1055,  567, 1055, 1055, 1055,
  584, 1055, 1055,  584, 1055, 1055,    0,    0,  568,    0,
    0,  567,    0,  567,  567,  567,    0,  574,  584,  567,
  584,    0,  567,  568,    0,  568,  568,  568,    0,    0,
    0,  568,    0,    0,  568,  567,    0,    0,    0,    0,
    0,  567,    0,    0,  567,  562,    0,  568,    0,    0,
  584,    0,    0,  568,    0,    0,  568,    0,  574,    0,
  562,  574,  562,  562,  562,    0,    0,    0,  562,    0,
    0,  562,    0,    0,    0,    0,  574,    0,  574,    0,
    0,    0,    0,    0,  562,    0,  567,    0,    0,    0,
  562,    0,    0,  562,    0,    0,    0,    0,  568,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  574,    0,
  567,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  568,    0,    0,    0,    0,    0,  559,    0,
    0,  567,    0,    0,    0,  562,    0,  567,    0,    0,
    0,  567,    0,  568,    0,    0,    0,    0,  567,  568,
    0,    0,    0,  568,    0,    0,    0,    0,    0,  562,
  568,    0,    0,    0,    0,    0,  567,    0,    0,  559,
    0,  567,  559,    0,    0,    0,    0,    0,  568,    0,
  562,    1,    0,  568,    0,    0,  562,  559,    0,  559,
  562,    0,    0,    0,    0,    0,  567,  562,    0,    2,
  567,    0,    0,    0,    0,    0,    0,    3,  568,    0,
    0,    0,  568,    0,    4,  562,    5,  584,    6,  559,
  562,    0,    0,    7,    0,    0,    0,    0,    0,    0,
    8,    0,  584,    0,    0,  584,  584,    0,    0,    0,
  584,    0,    0,  584,    9,  562,    0,    0,    0,  562,
    0,    0,    0,    0,   10,    0,  584,    0,    0,    0,
    0,    0,  584,    0,    0,  584,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  574,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  574,    0,    0,  574,  574,    0,    0,    0,  574,    0,
    0,  574,    0,    0,    0,    0,    0,  584,    0,    0,
    0,    0,    0,    0,  574,    0,    0,   11,    0,    0,
  574,    0,    0,  574,   12,    0,    0,   13,    0,   14,
   15,  584,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  584,    0,   16,    0,    0,    0,  584,    0,
    0,    0,  584,    0,    0,  574,    0,    0,    0,  584,
    0,    0,    0,  108,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  559,  584,    0,  574,
    0,    0,  584,    0,    0,    0,    0,    0,    0,    0,
    0,  559,    0,   17,  559,  559,    0,    0,    0,  559,
  574,  109,  559,    0,    0,    0,  574,  584,   18,   19,
  574,  584,    0,  110,    0,  559,    0,  574,    0,    0,
    0,  559,    0,    0,  559,    0,    0,    0,    0,    0,
    0,    0,    0,   20,    0,  574,    0,  111,    0,   49,
  574,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  574,  559,    0,    0,  574,
    0,    0,    0,    0,    0,    0,    0,   49,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   49,
  559,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  112,    0,    0,    0,    0,    0,    0,    0,
    0,  559,  113,   49,    0,    0,    0,  559,    0,    0,
    0,  559,    0,    0,    0,    0,    0,    0,  559,    0,
    0,    0,    0,    0,    0,    0,    0,  114,    0,    0,
    0,    0,    0,    0,    0,    0,  559,    0,    0,    0,
    0,  559,    0,    0,    0,  115,    0,    0,  116,    0,
  117,  118,  119,  120,    0,  121,    0,    0,    0,    0,
    0,    0,    0,  122,  123,    0,  559,    0,   49,    0,
  559,    0,    0,    0,  124,  125,    0,  126,   49,    0,
    0,    0,    0,    0,  127,  128,    0,  129,  130,    0,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,   49,    0,    0,    0,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,  327,    0,    0,
    0,   51,    0,    0,   51,  146,   51,   51,   51,   51,
    0,   51,    0,    0,    0,    0,    0,    0,    0,   51,
   51,    0,    0,    0,    0,  327,    0,    0,    0,    0,
   51,   51,  327,   51,    0,    0,  327,  327,    0,  327,
   51,   51,  327,   51,   51,    0,   51,    0,    0,   51,
   51,   51,   51,   51,    0,  327,    0,   51,   51,    0,
    0,  327,    0,   51,   51,   51,    0,   51,   51,   51,
    0,   51,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   51,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1352,    0,    0,    0,    0,  327,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  327,    0,    0,    0,
    0,    6,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  327, 1562, 1563,    0, 1564,    0,    0,    0,    0,
    0,  327,    0,    0,    0,    0,    0,    0,    0,    0,
  327, 1565,  327,    0,    0,    0,    0, 1566,    0,  327,
    0,    0,  327,    0,  327,  327,  327,  327,    0,  327,
    0,    0,    0,    0,    0,    0,    0,  327,  327,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  327,  327,
    0,  327,    0,    0,    0,    0,    0,    0,  327,  327,
    0,  327,  327,    0,  327,    0,    0,  327,  327,  327,
  327,  327,    0,    0, 1567,  327,  327,  327,  327,  327,
    0,  327,  327,  327,    0,  327,  327,  327, 1352,  327,
    0,    0,   14,    0,    0,    0,    0,    0,    0,  327,
    0,    0,    0,    0,    0,    0,    0, 1568,    0,    0,
    0,    0,    0,    0,    0,    0,    6,   16,    0,    0,
    0,    0,    0,    0,    0,    0, 1774, 1562, 1563,    0,
 1564,    0,    0,    0,    0,  487,    0,    0,  116,    0,
  117,  118,  119,  120,    0,  121, 1565,    0,    0,    0,
    0,    0, 1566,  122,  123,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1569,  125,    0,  126,    0,    0,
    0,    0,    0,    0,  127,  128,    0,  129,  130,    0,
  131,    0,    0,  132, 1570,  134,  135,  136,    0,    0,
    0,  137,  138, 1571, 1572, 1573,    0,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,    0,    0, 1567,
    0,    0,    0,    0,    0,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   14,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1568,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   16,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  487,    0,    0,  116,    0,  117,  118,  119,  120,    0,
  121,    0,    0,  693,    0,    0,  694,    0,  122,  123,
    0,    0,  912,    0,    0,    0,    0,    0,    0, 1569,
  125,    0,  126,  695,    0,    0,  696,    0,    0,  127,
  128,    0,  129,  130,    0,  131,    0,    0,  132, 1570,
  134,  135,  136,    0,    0,    0,  137,  138, 1571, 1572,
 1573,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,  697,    0,    0,    0,
  146,    0,  698,  699,  700,    0,  701,  702,  703,    0,
  704,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  705,    0,
  706,  693,    0,    0,  694,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  695,    0,    0,  696,    0,    0,    0,    0,  707,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  708,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  697,    0,    0,    0,  709,    0,
  698,  699,  700,    0,  701,  702,  703,    0,  704,    0,
    0,    0,    0,  115,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,  705,    0,  706,    0,
    0,  122,  123,  710,  711,    0,    0,    0,    0,    0,
    0,    0,  124,  125,    0,  126,    0,    0,    0,    0,
  712,    0,  127,  128,    0,  129,  130,  707,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,  708,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,    0,    0,  709,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0, 1026, 1063,    0,    0, 1063,    0,  122,
  123,  710,  711,    0,    0,    0,    0,    0,    0,    0,
  124,  125, 1065,  126, 1063,    0, 1027, 1063,  712,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0, 1063,    0,    0,
    0,  146,    0, 1063, 1063, 1063,    0, 1063, 1063, 1063,
    0, 1063, 1028,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1063,
    0, 1063,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  115,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,    0,
 1063,    0,    0,    0,  122,  123,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  124,  125, 1063,  126,    0,
    0,    0,    0,    0,    0,  127,  128,    0,  129,  130,
    0,  131,    0,    0,  132,  133,  134,  135,  136, 1063,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,    0, 1063, 1063,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  115,
    0, 1063,  116,  192,  117,  118,  119,  120,  193,  121,
  194,  195,  196,  197,  198,  199,  200,  122,  123,  201,
  202,  203,  204,  205,    0,    0,  206,  207,  124,  125,
  208,  126,    0,    0,  209,  210,  211,  212,  127,  128,
  213,  129,  130,  214,  131,  215,  216,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,  217,  218,  219,
  220,  139,  140,  141,  221,  142,  143,  144,  222,  145,
  223,  224,  225,  226,  227,  228,  229,  230,  231,  146,
  479,    0,    0,  479,  479,  479,  479,  479,  479,    0,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,    0,    0,  479,  479,  479,
  479,  479,  479,    0,    0,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,    0,    0,    0,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,
};
static short yycheck[] = {                                       9,
    0,    4,   61,  628,   71,   16,   16,  114,   18,  164,
  165,  164,  165,   70,  412,  174,   77,  821,  102,  443,
   70,  821,   75,  319,   80,  447,   93,   99,  447,  515,
  662,  414,  631,  439,   65,  556,  877,  515,   97, 1210,
  101, 1351, 1168, 1352, 1168,  575,   99,  103,   58,   59,
  831,   61,   62,   63,   64,   65, 1425,  336,  597,  824,
   70,   71,   72, 1104,   74,   96,   76,   77, 1090,   79,
   80,   81,   69,  104,  576,  777,  394, 1124,  747,   89,
   91,   91,   92,   93,   63,   95,   96,   97,   98,  100,
  100,  101,  102,  103,  104,  179,  106,   76, 1031,   62,
  586, 1168,  424, 1168,    0,  984,  156,  809,  158,  176,
  875,   74,  365,  423, 1370,  791,   41,    0,  740,   40,
    0,  797, 1232, 1176,   63,   44,    0,   40,   45,   45,
  270,  453,  653,   45,   45,   98,  657,    0,  169,   44,
  679,  662,  886,  106, 1168, 1078,  156,  157,  158,   44,
   63,    0,  162, 1174,    0,   44,   44,  183, 1168,  169,
 1181,   41,    0,   59,  174,   41,  176,  177,  178,  179,
   40, 1192,  279,  183, 1227,    0,   59,   41,  157,   59,
    0,  614,    0,    0,  882,   59, 1382,   45,   44,    0,
   44, 1109,   42,    0, 1215,   41,   59,   40,   44,  178,
    0,  921,   61, 1694,  273,   63,  365,   41,  906,    0,
   59,  909,   41,   59,  177,  639,   41,    0,   44,   44,
  918,   59,   40,   41,  334, 1423,   44,  264,  368,   44,
   41,  754,  344,   44,   59,   40, 1637,   44,  235,   59,
  662,   59,   59,  662, 1417, 1418,  324,    0,   59, 1422,
   41,   44,   59,   41,    0,  298,   44,  264,   63,   59,
  310, 1571, 1572,  313,    0,  372,  690,  665,   59,  266,
    0,  567,    0,  759, 1583,  310,   59,   44,  268,  977,
  395,    0,  298,   41,    0, 1456, 1090,  583,  347,  317,
 1090, 1417, 1418, 1417, 1418,  617, 1422,  272, 1422,   61,
  310,    0,   44,  313,  395, 1320,   59,  397, 1323,  319,
  320,   41,   91,   59,  396,  365,  714,  447,  271,  276,
  401,  317,    0,   59,  434,   44,   93,    0,    0,   59,
  412,   59,  285,    0,  277,  813, 1383,  347,  272,  282,
   59,  351,  820,   59, 1397,   59,  305,  428, 1024, 1025,
 1417, 1418, 1417, 1418,  280, 1422,  315, 1422,  448,  316,
   59,  336,  262,  413,    0,    0,  403,  498,   41,  359,
  367,   44,  351,  423,  409,  425,  329,  427,  456, 1420,
  430,   59,  449,  393,  394, 1061,   59,   59,    0,  458,
  451,  434,   59, 1417, 1418,  297,  403,  509, 1422,  514,
  428, 1802,  461,  413,  357,   41,   41, 1417, 1418,  500,
  260, 1155, 1422,  423,  935,  425,  575,  427,  434, 1440,
  430,  960, 1463,   59,   59,  858,    0,  315,  317,   41,
    0, 1627,   44,   41,  297,  315,  393, 1928,  297,  449,
  450,  451,  344,  440,  264,  442, 1148,   59,  297,  459,
  300,  461, 1150,    0,  464, 1824,  336, 1767, 1376, 1377,
  457,  464, 1660, 1396, 1184, 1185,  387,   41,  421,  315,
   44,   41,  306, 1171, 1172,  301,  433,  306, 1001,  379,
  459,  344,  336,  985,  380,   59,  445,  450,    0,   59,
  336, 1167,  376, 1191,  996,  344,  275,  315, 1000,  897,
  318,  315,  900,  379,  262,  431,  516,  325,  301,  327,
   44,  450,   59,  331, 1234,  379,  914, 1583,  336,   93,
  376,  339,  264,  351,  315,  575, 1541,  401,  681,   41,
 1545,  450, 1703,  450,  450,  297,  354,  450,  450,  450,
 1796,  446, 1346,  941,  840,  336, 1346,   59,  264,  444,
  368, 1920,    0,  423,  428,  565,  566,  567,  376,  585,
  477,  477,  380,  380,  574,  376,  446, 1348,  578,  887,
 1239,  389,  389,  583,  853,  585, 1252, 1253,  628, 1255,
 1256, 1691, 1347, 1462, 1114,  315,  565,  405,  513,  380,
  730, 1638,  450,   41, 1963,  574,  439,  907,  389,  578,
  446,  401, 1643,  566, 1226,  988,  336,  509,  446, 1234,
  682,   59,  662,  428,  432,  403, 1985,  513,  628,  402,
 1673,  379, 1124,  876,    0,  361,  444,  624,  446,  682,
  513, 1652,  315,  513,  380, 1656, 1657, 1690, 1116,  446,
 1949,  432,  315,  389,  641,  450,  509,    0,  494,  402,
 1152,  661,  662,  975,  262,  446,  484,  667,  655,  273,
  509,  380,  821,  336,  410, 1974,  365,  513,  493,  257,
  389, 1692,  344,  828,  390,  493,    0,  344,    0,  315,
  315,   41,  493,   59,    0,    0, 1371,  684,   41,  739,
  325,   44,  327, 2002,    0,  513,  513,  298,  444,  517,
  336,  336,  305,  315, 2013, 2015,   59, 2016,  861, 1882,
 1883,    0,  315,  325, 2023,  327,  446,  876,  446,   41,
  319,  315,  513,   41,  336,  444,   41,  339,  262,  739,
 1406, 1746,  729,  334,  348,   59,  352,   59,   44,  736,
  737,  401,  330,   59,   59,  380,  262,    0,    0, 1771,
  261,  782, 1428,   59,  389, 1161, 1882, 1883, 1882, 1883,
  371, 1446,  445, 1461,  316,  300,  336,  513,  380,   41,
   59,  379,  782,  446,  450,   41, 1709,  389, 1264,  829,
 1178,  831, 1180,  833,    0,    0, 1264, 1955,   41, 1810,
  943, 1493, 1190, 1814,  513, 1836,    0,  432,  390,  283,
 1641, 1842,  257,  315,  814,  421,   59,   59, 1829,  444,
  446,  446,  380,  348,  824, 1882, 1883, 1882, 1883,  829,
  432,  831, 1228,  833,  336, 1846,  876, 1426,  402,  440,
  840,  881,  444,  263,  446, 2003,  428,   41,  848,  849,
  513,  393,  445,   59,   59,  379,  856,  857,  304,  305,
   41,  445, 1330,   44, 1237,   59,  259,  907, 1882, 1883,
  870,  933,  872,  379,  432,  875,  269,  315,  450,  848,
  462,  881, 1882, 1883,    0,  330,  446,  887,  513,  309,
  933,  433,  258, 1949,  934,  935,  849,  940,  336,  886,
  320,  870,  262,  872,  270, 1527,  893,  907,  970,  971,
  972,  513,  262, 1381,  957,  297, 1927,  904, 1974, 1930,
 1449,   42,   43, 1452,   45,   41,   47,  970,  971,  972,
 1942,  297, 1944, 1306,  934,  935,  409,  303,  319,  305,
 1952, 1090, 1954,   59,  446,    0, 2002,  298,   42,   44,
  264,  951,  319,   47,  262,  275, 1967, 2013,  951,   40,
 2016,    0,  949,  436,   44, 1114,  348, 2023,    0,  397,
  276,  277,  315,    0, 1640,  322,  976, 1771,  344,  453,
  454, 1771,  325,  334,  327,  264,  986, 1999,  416,    0,
    0,  394,    0,  336,  994,  995,  339,  443,   93,  419,
  262,  994,   41,  315,   59,   44,  262,  976,  446,  313,
  314,  513,  262,   93, 1490, 1491,  258,  986,  309,  379,
   59,  341, 1498, 1499,  336, 1501, 1502,   59,  270,  379,
  348,  336,   59, 1509, 1510, 1511, 1512,  380, 1514, 1515,
 1516, 1517, 1518, 1519, 1065, 1556,  389,    0,   59,   59,
 1438,   59, 1466,    0,    0,  297,   44,    0,  264,  264,
  371,  303,   40,  305,  298, 1065,  372,  427,  380, 1355,
 1213,  379, 1358,  376, 1114,  422,  390,  389,  376,  754,
 1468,    0,  429,  434,  759,    0,    0,  397,   41,  432,
  280,   44,  273,  336, 1760,   41,    0,  378,   44,  410,
  334,  444,  344,  446,  395,   93,   59,    0, 1108, 1109,
  352,  390,   59,   59,   44, 1527,   59,  379, 1527,  344,
  432,  315,   41,  379, 1124,  306,   41,   41,  486,  379,
   44,  325,    0,  327,  446,  453,  454,   41,  448,  430,
   59,  446,  336,  509,   59,   59,  457,   93, 1942,  309,
 1944,    0, 1942,    0, 1944,   59,  277,  484, 1952, 1221,
 1954,  282, 1952,  378, 1954,  399,   59,  348, 1155,  333,
  513,  333,    0,   41, 1236,  409,   44,  427, 1221,  421,
 1242,  410, 1797,  277,  390,  390,  380,  377,  282,  516,
 1230,   59,    0, 1236, 1234,  389,    0, 1346,    0, 1242,
  434,  513, 1202, 1352,  318, 1999,  368,  304,  305, 1999,
   59,    0,   59,    0, 1207,  304,  305,  331,  380, 1219,
  336,    0,  403,  262, 1369, 1370, 1371,  300,  453,  454,
 1230,   59,  259, 1648, 1234,  395, 1902,  264,  432,    0,
  354,  431,  269, 1909,   42,   43,    0,   45,  258,   47,
  265,   59,  446,  264,  485,   59,  260,   59,  448, 1402,
  270, 1404,   41,  259,  495,   44,  426,  509,  258,    0,
   59,  434,   59,  269, 1662,  348,  315,  458, 1693,  318,
   59,  441,  273,  338,  299, 1359,  325,  297,  327,  297,
    0,  405,  331,  303,    0,  305,  300,  336,   59,  372,
  339, 1302, 1302,    0, 1351,   59, 1357,  428, 1348, 1697,
  339, 1351, 1352,  303, 1360,  354,  371,  264,  333,  513,
  446, 1464,  997,  276,  277,  513, 1001,    0,   59,  368,
  446,  284,  238,  362,  344,   41,  344,  376,   44,    0,
  379,  380,  397, 1958,  352,  400,  443, 1347, 1348,   59,
  389, 1351, 1352,   59,  443, 1355, 1356, 1357, 1358, 1359,
 1360,  416,   59,  390,  258,    0,  405,  261,   41,    0,
  372,   44,  260,  337,    0,  452, 1376, 1377,  386,  390,
  388,  345,    0, 1383,  290,    0,   59,  442,  427,  418,
   40,  393,  298,  432,  262,  520,  315,  399,   59,    0,
  315,  315,  410, 1356,  416,  444,  325,  446,  327,  303,
   41,  315,  300,  327,    0,  264,  453,  336,    0,  372,
  410,  336,  336,  327,   59,  450,   41,  435,   59,   44,
    0,  433,  336,   59, 1583,    0,  309,    0,  428,    0,
  393,   59,  260,  390,   59,  338,  399,  315,  276,  277,
  318, 1444,  477,  260,  493, 1843,  402,  325,   59,  327,
  333,  380,  264,  331,  264,  380,  380,    0,  336,  342,
  389,  339,  462,   59,  513,  389,  380,   59,  517,  277,
  433, 1630,  300,  262,  282,  389,  354,  276,  277,   59,
 1477,  338,    0,  300,   59, 1482,   59,  313,   59,  509,
  368,  509,  315,  264,  397, 1562,    0,  400,  376,    0,
  265,  379,  380,  432,  260,  260,  333,  432,  432, 1520,
 1520,  389,  352,  416,  371,  398,   59,  446,  432,  402,
  426,  446,  446,    0,    0,  332,  315,  405,  336,  318,
  493,  390,  446, 1583,  299,  457,  325,  459,  327,  442,
  397,   59,  331,  400,  300,  300,  386,  336,  388,  427,
  339,  258, 1562,  450,  432,   59,  444,   46,   59,  416,
 1570, 1571, 1572,  270,   41,  354,  444,   44,  446,   58,
  410,  403,   61, 1583,  338,    0,  485,  486,  390,  368,
  477,   40,   59,   59,  513,  442,    0,  376,  513,  513,
  379,  380,    0,  276,  277,  435,  303,  258,  305,  513,
  389,  284,   41, 1613, 1614,   44,  399,  371,  401,  270,
  318,  284, 1771,  395,  333,  493,  405,  317,  501,  390,
 1630,  409,   41,  331,  297,   44,  336, 1637, 1638,    0,
  513,  514,  515,  397,   59,  513,  400,  262,  427,  517,
 1799, 1796,  303,  432,  305,   59,  354,  315,  436,  368,
    0,   59,  416, 1663,  376,  444,   41,  446,  315,   44,
  368,  380,  318,   42,   43,  376,   45,  260,   47,   41,
   41,  428,   44,   44,  315,  331,   41, 1736,  442,   44,
  453,  454,   41,  339, 1663,   44,  327,  498,   59,  372,
  315,   41,  356,  318,   44,  336,   41,  405,  354,   44,
  325,  426,  327,  586,  493,  298,  331,  590,  442,   59,
  393,  336,  368,  410,  339,   41,  399,  310,   44, 1969,
 1827, 1971, 1733, 1733,  513,  377, 1736,   41,  517,  354,
   44,   41,   42,   43, 1386,   45, 1388,   47,  319,  380,
  337,  334,  338,  368,  449,  450,  451, 1797,  389,  405,
  433,  376,  384,  385,  379,  380,  291, 1767,  453,  454,
   42,   43,  375,   45,  389,   47,  497,  302,   41, 1828,
  409,   44,  477,  478,  309,  371,   42,   43,  313,   45,
  405,   47,  508, 1942,   46, 1944,  509, 1797, 1788, 1799,
 1949,  432, 1802, 1952,  508, 1954,   58,   59,  333,   61,
   41,  397,  427,   44,  400,  446,  399,  432, 1818,  517,
  493,   46,  284, 1823, 1921, 1974,  409,  461, 1828,  444,
  416,  446,    0,   58,   59,  306,   61, 1894,   42,   43,
  446,   45,   44,   47,   42,   43,   61,   45,    0,   47,
 1999,  434,    0, 2002, 1823,  366,  442,  283,   42,   43,
   40,   45, 1849,   47, 2013,   41, 1853, 2016,   44,   41,
  395,  517,   44,   41, 2023, 1875,   44,  778,  493,  780,
 1613, 1614,  513,  784,  785,  786,  759,   41,  264,   41,
   44,   59,   44,   41, 1894, 1895,   44,   41,  513,   40,
   44,  426,  517, 1896,  389,  430,  779,   59,  277, 1949,
  783,   59,   40,  282,   40,  276,  277,  790, 1958, 1613,
 1614,   41,  795,  796,   44,   40,  799,  800,  801,  802,
  803,  804,   41,   41, 1974,   44,   44,  453,   41,   40,
  813,   44,  815,  816,  817,  818,  819,  820,   40, 1949,
  823, 1941,  453,   41,   41, 1955,   44,   44, 1958,   40,
   42,   43, 2002,   45,   40,   47,   40,  336,   40, 1969,
  453, 1971,   40, 2013, 1974,  315, 2016,  277,  318,   40,
   40,   40,  282, 2023,  351,  325,  859,  327,   40,   40,
   40,  331,   40,   40,  519,   40,  336,   40,   91,  339,
    0,  402, 2002, 2003,   46,  277,  879,   44,   61,    0,
  282,  372,   40, 2013,  354, 2015, 2016,  317,  891,  892,
  264,  277,  395, 2023,  317,   44,  282,   44,  368,  420,
   46,   61,  393,   40,  264,  337,  376,   46,  399,  379,
  380,   41,   44,  344,   44,  317,  344,  344,  276,  389,
   41,   42,   43,   44,   45,   46,   47,  276,  297,   59,
  365,  317,  390,  264,  428,  405,  462,  390,   59,   60,
   61,   62,  433,  277,  344,  275,    0,  392,  282,  277,
  450,  265,  439,  439,  282,   40,  345,  427,   40,   40,
   91,   40,  432,  277,  315,  264,  376,  403,  282,   44,
   91,   41,   42,   43,  444,   45,  446,   47,  276,  277,
  308,  315,   40,  382,  375,  299,  284,   41,   40,  443,
   44,  315,  380,   41,  276,  277,  275,   63,  276,  277,
  296,  456,  284,  428,  428,   59, 1009, 1010, 1011,  314,
   40, 1014, 1015, 1016,  440, 1018, 1019, 1020, 1021, 1022,
 1023,   44,  444,  493,  264,  337,   44,  494,   40, 1032,
 1033,  273, 1035,   40, 1037,  297,  340, 1040, 1041, 1042,
 1043, 1044, 1045,  513,    0,  281,  340,  517,  276,  408,
  379,   41,   42,   43, 1057,   45, 1059,   47,   41,   42,
   43,    0,   45,  379,   47,  277,  379,  410,  410,   40,
  282,  352, 1075,   40,  372, 1014,   40, 1016,  376, 1018,
 1019,   40, 1021, 1022, 1023,   41,  362,   41,   44,   44,
  372,  317,  277,   44,  372,  393,  308,   46,  438,  270,
  361,  399,   41,   59,  317,   44,   41,   42,   43,  378,
   45,  393,   47, 1116,  317,  393,   40,  399,   44,   40,
   59,  399,   61,  266,  325,  493,    0,   41,   42,   43,
   40,   45,  302,   47,  414,  433,  264,   41,   58,  309,
   41,  262,   41,  313,  314,   41,  276,  277,  264,  446,
  271,  433,  322,  387,  284,  433,  277,   41,  306,   41,
   40,  282,  306,  333,  285,   40,   40,   41,   42,   43,
   44,   45,   46,   47,   41,   42,   43,  317,   45,   40,
   47,   40,   40,   40,   58,   59,   60,   61,   62,   40,
   40,   40,   40,   40,  315,  493,   40,  318,   40,   40,
  321,   40,   40,  262,  325,  305,  327,  277,  329,  446,
  331,  493,  282,  427,   44,  336,  337,   91,  339,   93,
   44,   41,  276,  277,   41,  395,  264,  281,  381,  339,
  284,  339,    0,  354,  355,  327,  357,  273,  284,  384,
  399,  411,  372,  297,  379,  379,  379,  368,   41,  370,
  379,  372,  373,  374,  379,  376,  426,  379,  379,  380,
  430, 1264,  338,  393,  450,  372,   41,  276,  389,  399,
   40,  441,   41,   41, 1277, 1278,   44, 1280, 1281,   41,
   41,   41,  317,  305,  405, 1288,   44,  277,   41,  427,
  460,   59,  282,   59,  277,  371,  290,   40,  324,  282,
  421,  305,   59,  433,  336,   59,  427, 1310,   41,   42,
   43,  432,   45,   59,   47, 1318, 1319, 1320,  372,  258,
 1323,  397, 1325,  444,  400,  446, 1329, 1330, 1331,   59,
   59,  336,   59,   59,  273, 1338,   59, 1340,   59,  393,
  416,   59,  277,   59,   59,  399,   59,  282,   59,  519,
   59,   41,   42,   43,   59,   45,   58,   47,  297,  315,
  428,  339,  318,  277,  303,  277,  442,  306,  282,  325,
  532,  327,  493,  333,  434,  331,  334,  434, 1381,  433,
  336,  298,  334,  339,   41,  298,   41,  348,  262,  378,
  264,  265,  513,  365,   41,    0,  517,  271,  354,  365,
   41,   41,   41,  277,   41,   41,   41,   41,  282,  348,
  277,  285,  368,   41,   41,  282,   41,   41, 1421,   41,
  376,   41,   41,   41,  380,  299,    0,   40,  315,   41,
  304,  305,  443,  389,  308,  378,   41,   40,   44,   44,
   41,  315,  348,  317,  318,  340,   40,  321,  458,  405,
  333,  325,   40,  327,   59,  329,  500,  331,  336,  336,
   40,  376,  336,  337,  403,  339,   44,   41,   41,  376,
   44,  410,   42,   43,   44,   45,  432,   47,   40,  427,
  354,  355,  376,  357,   41,   59, 1489,   41,  444,  428,
  446,  301, 1495,  298,  368,   41,  370, 1500,  372,  373,
  374,  434,  376,  304, 1507,  379,  380,  290,  276,  277,
  301,  455,    0,  281,  333,  389,  284,  409,   40,  458,
   41,   42,   43,  462,   45,  264,   47,  264,  402,  297,
  376,  405,  276,  376,  450,  317,   40,  493, 1541,  462,
  403,  443, 1545,   44,  277,  270,   40,  421, 1551,  282,
   40,  270,   41,  427,  428,  427,  264,  513,  432,  450,
  339,  517,  339,   41, 1567,  450,   41,   59,   41,  443,
  444,  305,  446,   41,   42,   43,   59,   45,  452,   47,
   59,  455,   59,  457,  458,  459,  460,  277,  462,   41,
   59,   41,  282,   59,   59,   59,  470,  471,   42,   43,
   59,   45,  301,   47,  372,    0,    0,  481,  482,    0,
  484,   42,   43,   44,   45,    0,   47,  491,  492,  493,
  494,  495,  161,  497,  581,  393,  500,  501,  502,  503,
  504,  399,    0, 1367,  508,  509, 1639,  161,  583,  513,
  514,  515,  516,  517,  518,  519,  520,  162,  522,   41,
   42,   43,  849,   45,  321,   47,  320,  262,  532, 1239,
  743,  840,  989, 1237,  951,  433,   41,   42,   43,   95,
   45,  994,   47,   41,   42,   43,   44,   45,   46,   47,
   41,   42,   43,   81,   45, 1234,   47, 1229,  262,  964,
 1680,   59,   60,   61,   62,   42,   43, 1681,   45,  444,
   47, 1219,  690,  410, 1164,  370, 1230,  277,  425, 1712,
  315,  425,  282,  318, 1818, 1718, 1719,  881, 1660, 1961,
  325,  916,  327,   91, 1965,   93,  331, 1662, 1964,  667,
 1987,  336,   42,   43,  339,   45, 1776,   47,    0, 1742,
 1105,  315,  907, 1746,  318,   42,   43, 1750,   45,  354,
   47,  325, 1570,  327, 1757, 1758,  277,  331, 1950, 1399,
  829,  282,  336,  368, 1644,  339,    0,  430,  967,  415,
  408,  376,  669, 1453,  379,  380,   13,  727,  355,   41,
  354,  163,   44,  959,  389,    0,  464,  465,  466,  467,
  468,  469,  470,  471,  368,  599,    0,   59,   42,   43,
  405,   45,  376,   47,  603,  379,  380,   41,  870,  277,
   44,  167,  859, 1124,  282,  389,   42,   43,   44,   45,
  264,   47,  427, 1387,   91,   59,   41,  432, 1914,   44,
  776,  405, 1733,  277, 1520,  891,  614,   41,  282,  444,
   44,  446, 1279, 1408,   59, 1848,  277,  847, 1325, 1117,
   -1,  282,   -1,  427,   -1,   59,   -1,   61,  432,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
  444,   -1,  446,   -1,   -1,   -1,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,  277,   -1,   -1,  493,   -1,
  282,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,  277,  271,   -1,   -1,   -1,  282,  513,  277,
   -1, 1914,  517,   -1,  282,  262,  277,  285,   -1,  493,
   -1,  282,   41,   42,   43,   -1,   45,   -1,   47,   -1,
  277,  299,    0,  301,   -1,  282,  304,  305,   -1,  513,
  308,   -1,   -1,  517,   -1,   -1,   -1,  315, 1951,  317,
  318,   -1,   -1,  321,  264,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,  262,   -1,  277,  336,  337,
   -1,  339,  282,   41,   42,   43,   44,   45,   46,   47,
  277,   -1,   -1,   -1,   -1,  282,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   41,   42,   43,
  368,   45,  370,   47,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,  276,  277,   -1,   -1,   -1,   -1,
   -1,  389,  284,   91,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  277,  402,  297,   -1,  405,  282,   -1,
   -1,   -1,  276,  277,   -1,   -1,   -1,   -1,   -1,   -1,
  284,  277,   -1,  421,  258,   -1,  282,   -1,   -1,  427,
  428,  276,  277,  297,  432,   -1,   -1,   -1,   -1,  284,
   -1,  275,  276,  277,   -1,  443,  444,  281,  446,   -1,
  284,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,  297,  462,  277,   -1,   -1,   -1,  303,
  282,   -1,  470,  471,   -1,   41,   42,   43,   -1,   45,
  372,   47,  277,  481,  482,   -1,  484,  282,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,  393,  500,  501,  502,  503,  504,  399,  372,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,  372,  277,  393,
   -1,   -1,   -1,  282,  532,  399,   -1,   -1,  372,  318,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  393,   41,
   42,   43,  331,   45,  399,   47,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  262,  399,  264,  265,   -1,  433,
   -1,   -1,   -1,  271,  408,  354,  410,   -1,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,  433,  368,
   -1,   -1,   -1,    0,  428,   -1,   -1,  376,   -1,  433,
   -1,  299,    0,  277,   -1,   -1,  304,  305,  282,   -1,
  308,   -1,   41,   42,   43,   -1,   45,  315,   47,  317,
  318,   -1,   -1,  321,   -1,   -1,  405,  325,  462,  327,
   -1,  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,
   -1,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,   -1,   -1,   41,   42,   43,
  368,   45,  370,   47,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   91,   -1,   93,   -1,   -1,   -1,   41,
   42,   43,   -1,   45,  402,   47,   -1,  405,   -1,   -1,
   -1,  277,   -1,   -1,  493,   -1,  282,   -1,   -1,   -1,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,  517,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   41,
   42,   43,   -1,   45,  452,   47,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   41,   42,   43,   -1,   45,
   -1,   47,  470,  471,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,  481,  482,  277,  484,   -1,   -1,   -1,
  282,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   41,   42,   43,
   -1,   45,   -1,   47,  532,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  277,   -1,
   -1,  258,   -1,  282,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,  275,  276,
  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
  297,   -1,   -1,    0,   -1,   -1,  303,   -1,   -1,   -1,
   -1,  299,    0,  277,   -1,   -1,  304,  305,  282,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   41,  277,   -1,   44,  336,  337,
  282,  339,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,
   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  277,  393,   -1,   -1,   -1,
  282,  389,  399,   91,  291,   93,   -1,   -1,   -1,   -1,
   -1,  277,   -1,  410,  402,  302,  282,  405,   -1,   -1,
  277,   -1,  309,   -1,   -1,  282,   -1,   -1,   -1,   -1,
   -1,  428,   -1,  421,   -1,  322,  433,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,  462,   -1,  455,   -1,  457,
  458,  459,  460,  277,  462,   -1,   -1,   -1,  282,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  379,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,  395,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,  411,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,  426,
   -1,   -1,   -1,  430,  532,   -1,  433,   -1,   -1,   -1,
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
   -1,  379,  380,   -1,   -1,   -1,  393,   -1,   -1, 1490,
 1491,  389,  399,   91,   -1,   93,   -1, 1498, 1499,   -1,
 1501, 1502,   -1,  410,  402,   -1,   -1,  405, 1509, 1510,
 1511, 1512,   -1, 1514, 1515, 1516, 1517, 1518, 1519,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,  532,   -1,   -1,  325,   -1,  327,   -1,   41,   -1,
  331,   44,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,
  262,   -1,  264,  265,   -1,   -1,   59,   -1,   -1,  271,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  285,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  299,  379,  380,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,  389,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,  405,  327,   -1,  329,   -1,  331,
   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  432,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   42,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,  421,
   -1,   -1,    0,   -1,   -1,  427,  428,   -1,   -1,   -1,
  432,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,
   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   40,   41,   -1,   -1,   44,   -1,  470,  471,
   -1,  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   59,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
  522,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
  532,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,  354,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,  481,  482,  376,  484,   -1,   40,  380,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,  389,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,  405,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  293,    0,  532,   -1,   -1,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   41,   -1,   -1,   44,   -1,  470,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   59,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  532,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
  368,   -1,  455,   -1,  457,  458,  459,  460,  376,  462,
   -1,   -1,  380,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,  405,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,  432,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,  532,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,    0,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  522,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,  532,  409,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,
  339,   -1,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,  452,  368,
   -1,  455,   -1,  457,  458,  459,  460,  376,  462,   -1,
   -1,  380,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  389,   -1,   -1,   91,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,  405,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,  432,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,  532,   -1,
  275,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,  291,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,  302,   -1,   -1,
   -1,   -1,  481,  482,  309,  484,   -1,   -1,  313,  314,
   -1,   -1,  491,  492,  493,  494,  495,  322,  497,    0,
   -1,  500,  501,  502,  503,  504,   -1,   -1,  333,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   42,   43,   -1,   45,   -1,   47,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   59,   60,
   61,   62,  387,   -1,  262,   -1,   -1,   -1,   -1,   -1,
  395,    0,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  426,   -1,   -1,   -1,  430,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,  441,  315,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   59,  329,   -1,  331,   -1,  460,  461,   -1,  336,  337,
   -1,  339,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,  389,   -1,   -1,  519,   -1,   -1,  522,   -1,   -1,
   -1,   -1,   59,    0,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,  444,   44,  446,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   59,  271,   -1,   -1,   -1,   -1,   -1,  277,
  271,   -1,   -1,   -1,  282,   -1,  277,  285,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  321,  315,  513,   -1,  318,   -1,  517,
  321,  329,   -1,   -1,  325,   -1,  327,   -1,  329,  337,
  331,   -1,   -1,  262,   -1,  336,  337,   -1,  339,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,  355,   -1,  357,
   -1,   -1,   -1,  354,  355,   -1,  357,   59,   -1,   -1,
   -1,   -1,  370,   -1,  372,  373,  374,  368,   -1,  370,
    0,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,  405,  262,   -1,  336,   -1,   -1,
  339,   41,   -1,  421,   44,   -1,   -1,   -1,   -1,   -1,
  421,   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,   59,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,   -1,    0,  444,   -1,  446,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   -1,   -1,  262,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,
   -1,   -1,  339,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,  354,  427,   -1,
   -1,   59,    0,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,  513,   -1,   -1,  444,  517,  446,  315,  376,
   -1,  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,  389,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   41,   -1,   -1,   44,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,
  262,   59,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  427,  368,   -1,   -1,    0,  432,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,  315,   -1,   41,  318,   -1,   44,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
  427,   -1,  262,   59,  336,  432,  493,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,  354,   -1,   -1,   -1,  513,   -1,    0,   -1,
  517,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,  493,  327,   -1,   41,
   -1,  331,   44,  405,  262,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,  513,   59,   -1,   -1,
  517,   -1,   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
   -1,    0,  444,   -1,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   -1,   -1,   -1,  262,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,   -1,
   -1,  339,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,
   59,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,  513,   -1,    0,  444,  517,  446,  315,  376,   -1,
  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,  389,   -1,  331,   -1,   -1,  262,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,  354,   44,   -1,   -1,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,  427,
  368,   -1,   59,    0,  432,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,  315,
   -1,  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,
  336,   -1,   -1,  339,   41,   -1,   -1,   44,   -1,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  427,
   -1,   -1,   59,   -1,  432,  493,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,   -1,   -1,  444,   -1,  446,   -1,
  376,   -1,   -1,  379,  380,  513,   -1,   -1,   -1,  517,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  405,
   -1,   -1,   -1,  325,    0,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,  493,   -1,  339,   -1,   -1,
   -1,  427,   -1,  262,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,  513,   -1,    0,  444,  517,
  446,   -1,   -1,   -1,   -1,   41,  368,   -1,   44,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,   41,  318,
   -1,   44,   -1,  405,   -1,   -1,  325,  493,  327,   -1,
   -1,   -1,  331,   -1,   -1,  262,   59,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,  427,   -1,  513,   -1,   -1,
  432,  517,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,  446,   -1,    0,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   -1,   -1,  262,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,   41,   -1,  336,
   44,  493,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,  354,  427,    0,
   -1,  513,   -1,  432,   -1,  517,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,  444,   -1,  446,  315,  376,
   -1,  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,  389,   -1,  331,   -1,   -1,   -1,   -1,  336,
   41,   -1,  339,   44,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,   59,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  427,  368,   -1,   -1,   -1,  432,  262,   -1,   -1,  376,
   -1,   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,
   -1,   -1,  389,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,  262,
   40,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,
  427,   -1,  318,   -1,   -1,  432,  493,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  444,   -1,  446,
  336,   -1,   -1,  339,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  315,   -1,   -1,  318,   -1,   -1,  354,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,  368,  336,   -1,   -1,  339,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,  493,   -1,  262,   -1,
   -1,  354,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,  513,   -1,   -1,  405,
  517,   -1,   -1,  376,   -1,   40,  379,  380,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,  427,   -1,   58,   -1,   -1,  432,   -1,   63,   -1,
   -1,  315,  405,   -1,  318,   -1,   -1,   -1,  444,   -1,
  446,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
   -1,   -1,  336,   -1,  427,  339,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,  493,   -1,   -1,
   -1,   -1,  376,   -1,  315,  379,  380,  318,   -1,   -1,
   -1,   -1,   -1,  271,  325,  389,  327,  513,   -1,  277,
  331,  517,   -1,   -1,  282,  336,   -1,  285,  339,   -1,
  493,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  281,   -1,  354,   -1,   40,   -1,   -1,   -1,   -1,
  513,   -1,   -1,  427,  517,   -1,   -1,  368,  432,   -1,
   -1,   -1,   -1,  321,   -1,  376,   -1,   -1,   -1,  380,
  444,  329,  446,   -1,   -1,   -1,   -1,   -1,  389,  337,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  405,   -1,   -1,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  370,   -1,  372,  373,  374,   -1,   -1,  493,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,  513,
   -1,   -1,  267,  517,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,  421,   -1,   -1,   -1,  292,  293,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,  311,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,   -1,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,  348,   -1,  350,  351,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,  481,  482,   -1,  484,   -1,   -1,  372,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,  409,   -1,   -1,   -1,  413,   -1,
   -1,   -1,  532,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   42,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
  505,  506,  507,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,  267,  524,
  525,  526,   -1,  528,  529,  274,  531,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
  289,   -1,   -1,  292,  293,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  311,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,
  495,   -1,  497,  372,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   44,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,
  323,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,  372,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   45,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  317,   -1,  449,  450,  451,  452,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   58,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,  267,  524,  525,  526,
   -1,  528,  529,  274,  531,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   58,  450,   -1,  452,   -1,   63,  455,
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
  521,  522,   -1,  524,  525,  526,  527,  528,  529,  530,
  531,  532,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,  311,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,  348,   -1,  350,  351,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  481,  482,   -1,  484,   -1,   -1,   -1,  372,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,  532,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
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
  272,   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,  286,   -1,
  292,   -1,   -1,  295,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   58,   -1,   -1,
   -1,   -1,   63,  335,   -1,   -1,   -1,   -1,   -1,  341,
  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,  367,  364,  369,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,
   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
  472,  473,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,  489,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,  524,  525,  526,  260,
  528,  529,  263,  531,  532,   -1,  267,  272,   -1,   -1,
  275,   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  286,   -1,  292,   -1,   -1,
  295,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   58,   -1,   -1,   -1,   -1,   63,
  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,  367,  364,  369,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  415,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,  472,  473,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,  524,  525,  526,  260,  528,  529,  263,
  531,  532,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
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
  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,  325,   -1,  472,  473,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
  524,  525,  526,  260,  528,  529,  263,  531,  532,   -1,
  267,   -1,  395,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  430,   -1,   -1,
   40,   -1,  435,   43,   -1,   45,   -1,   -1,  441,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   58,  452,
   -1,   -1,  455,   63,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,  435,
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
  263,  531,  532,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
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
   -1,  524,  525,  526,  260,  528,  529,  263,  531,  532,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   58,
  452,   -1,   -1,  455,   63,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,  524,  525,
  526,  260,  528,  529,  263,  531,  532,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   58,   -1,  323,   -1,   -1,   63,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,  360,   -1,   -1,   -1,  364,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,  284,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,  260,  524,  525,  526,   -1,  528,
  529,  267,  531,  532,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,  300,   -1,   -1,   40,   -1,   42,
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
  516,   -1,  518,  519,  520,   -1,  522,  260,  524,  525,
  526,   -1,  528,  529,  267,  531,  532,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   40,   -1,   42,   43,  300,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,
  323,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,  522,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,
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
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,  450,
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
   -1,   40,   41,   -1,   43,   -1,   45,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,  446,   -1,   -1,  449,  450,  451,  452,
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
  317,   -1,   63,  320,   -1,   -1,  323,   -1,   -1,   -1,
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
   -1,  292,  293,   -1,   -1,   40,   -1,   42,   43,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,  514,  515,  516,   -1,  518,  519,  520,    0,  522,
  267,  524,  525,  526,   -1,  528,  529,  274,  531,  532,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   40,   41,   -1,  320,   44,   -1,  323,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   41,   91,
   -1,   44,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   91,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,    0,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,  258,  524,  525,  526,
   41,  528,  529,   44,  531,  532,   -1,   -1,  258,   -1,
   -1,  273,   -1,  275,  276,  277,   -1,    0,   59,  281,
   61,   -1,  284,  273,   -1,  275,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,  297,   -1,   -1,   -1,   -1,
   -1,  303,   -1,   -1,  306,  258,   -1,  297,   -1,   -1,
   91,   -1,   -1,  303,   -1,   -1,  306,   -1,   41,   -1,
  273,   44,  275,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,  297,   -1,  348,   -1,   -1,   -1,
  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,  348,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,
  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,  393,   -1,   -1,   -1,  348,   -1,  399,   -1,   -1,
   -1,  403,   -1,  393,   -1,   -1,   -1,   -1,  410,  399,
   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,  372,
  410,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   41,
   -1,  433,   44,   -1,   -1,   -1,   -1,   -1,  428,   -1,
  393,  261,   -1,  433,   -1,   -1,  399,   59,   -1,   61,
  403,   -1,   -1,   -1,   -1,   -1,  458,  410,   -1,  279,
  462,   -1,   -1,   -1,   -1,   -1,   -1,  287,  458,   -1,
   -1,   -1,  462,   -1,  294,  428,  296,  258,  298,   91,
  433,   -1,   -1,  303,   -1,   -1,   -1,   -1,   -1,   -1,
  310,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,  324,  458,   -1,   -1,   -1,  462,
   -1,   -1,   -1,   -1,  334,   -1,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  273,   -1,   -1,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,   -1,   -1,   -1,   -1,  348,   -1,   -1,
   -1,   -1,   -1,   -1,  297,   -1,   -1,  397,   -1,   -1,
  303,   -1,   -1,  306,  404,   -1,   -1,  407,   -1,  409,
  410,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  393,   -1,  434,   -1,   -1,   -1,  399,   -1,
   -1,   -1,  403,   -1,   -1,  348,   -1,   -1,   -1,  410,
   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  258,  428,   -1,  372,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  273,   -1,  483,  276,  277,   -1,   -1,   -1,  281,
  393,  298,  284,   -1,   -1,   -1,  399,  458,  498,  499,
  403,  462,   -1,  310,   -1,  297,   -1,  410,   -1,   -1,
   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  523,   -1,  428,   -1,  334,   -1,  260,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  458,  348,   -1,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  310,
  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  399,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  393,  409,  334,   -1,   -1,   -1,  399,   -1,   -1,
   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,  458,   -1,  399,   -1,
  462,   -1,   -1,   -1,  481,  482,   -1,  484,  409,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  434,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,  270,   -1,   -1,
   -1,  452,   -1,   -1,  455,  532,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,  298,   -1,   -1,   -1,   -1,
  481,  482,  305,  484,   -1,   -1,  309,  310,   -1,  312,
  491,  492,  315,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  328,   -1,  508,  509,   -1,
   -1,  334,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,  391,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  424,  309,  310,   -1,  312,   -1,   -1,   -1,   -1,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,  328,  445,   -1,   -1,   -1,   -1,  334,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,  391,  508,  509,  510,  511,  512,
   -1,  514,  515,  516,   -1,  518,  519,  520,  270,  522,
   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,  532,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  298,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  309,  310,   -1,
  312,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,  328,   -1,   -1,   -1,
   -1,   -1,  334,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,  391,
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  424,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,  272,   -1,   -1,  275,   -1,  470,  471,
   -1,   -1,  281,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,  292,   -1,   -1,  295,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
  532,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,
  369,  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,  398,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,  437,   -1,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,  367,   -1,  369,   -1,
   -1,  470,  471,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
  489,   -1,  491,  492,   -1,  494,  495,  398,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  415,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,  437,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,  339,  272,   -1,   -1,  275,   -1,  470,
  471,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  290,  484,  292,   -1,  362,  295,  489,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
   -1,  532,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,
   -1,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
  398,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,  437,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  489,  455,  456,  457,  458,  459,  460,  461,  462,
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
"proc_statement : assignment ';'",
"proc_statement : insert ';'",
"proc_statement : update ';'",
"proc_statement : delete ';'",
"proc_statement : singleton_select ';'",
"proc_statement : exec_procedure ';'",
"proc_statement : exec_sql ';'",
"proc_statement : exec_into ';'",
"proc_statement : exec_udf ';'",
"proc_statement : excp_statement ';'",
"proc_statement : raise_statement ';'",
"proc_statement : post_event ';'",
"proc_statement : cursor_statement ';'",
"proc_statement : breakleave ';'",
"proc_statement : SUSPEND ';'",
"proc_statement : EXIT ';'",
"proc_statement : if_then_else",
"proc_statement : while",
"proc_statement : for_select",
"proc_statement : for_exec_into",
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
"alter_op : col_opt alter_col_name TYPE alter_data_type_or_domain end_trigger",
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
case 313:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 314:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 319:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
break;
case 320:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
break;
case 321:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 322:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
break;
case 323:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 324:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 325:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
break;
case 326:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 327:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 328:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
break;
case 329:
{ yyval = NULL; }
break;
case 330:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
break;
case 331:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
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
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
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
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 361:
{ yyval = make_list (yyvsp[0]); }
break;
case 362:
{ yyval = make_list (yyvsp[-1]); }
break;
case 363:
{ yyval = NULL; }
break;
case 364:
{ yyval = make_list (yyvsp[0]); }
break;
case 365:
{ yyval = make_list (yyvsp[-1]); }
break;
case 366:
{ yyval = NULL; }
break;
case 367:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 368:
{ yyval = make_list (yyvsp[-1]); }
break;
case 369:
{ yyval = NULL; }
break;
case 371:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 372:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 373:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 374:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 375:
{ lex.beginning = lex_position(); }
break;
case 376:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 377:
{ lex.beginning = lex.last_token; }
break;
case 378:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 379:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 380:
{ yyval = 0; }
break;
case 381:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 382:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 383:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 385:
{ yyval = NULL; }
break;
case 386:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 387:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 405:
{ yyval = NULL; }
break;
case 406:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 407:
{ yyval = yyvsp[0]; }
break;
case 408:
{ yyval = yyvsp[0]; }
break;
case 409:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 413:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 414:
{ yyval = make_node (nod_mod_index, (int) e_mod_idx_count, yyvsp[0]); }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = yyvsp[0]; }
break;
case 417:
{ yyval = yyvsp[0]; }
break;
case 418:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 420:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 421:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 435:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 436:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 437:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 479:
{ yyval = NULL; }
break;
case 480:
{ yyval = NULL; }
break;
case 481:
{ yyval = NULL; }
break;
case 482:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 483:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 484:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 485:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 486:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 487:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 488:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 489:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 490:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 491:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 492:
{ yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 493:
{ yyval = yyvsp[0]; }
break;
case 494:
{ yyval = NULL; }
break;
case 495:
{ yyval = yyvsp[0]; }
break;
case 496:
{ yyval = NULL; }
break;
case 497:
{ yyval = NULL; }
break;
case 499:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 500:
{ yyval = yyvsp[0]; }
break;
case 501:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 502:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 503:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 504:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 505:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 507:
{ yyval = NULL; }
break;
case 509:
{ yyval = NULL; }
break;
case 510:
{ yyval = yyvsp[0]; }
break;
case 511:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 512:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 513:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 514:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 515:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 516:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 517:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 518:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 519:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 520:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 521:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 522:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 523:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 528:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 529:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 531:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 532:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 533:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 539:
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
case 540:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 542:
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
case 543:
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
case 544:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 549:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 550:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 551:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 552:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 553:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 554:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 555:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 556:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 558:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 559:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 560:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 561:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 562:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 563:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 572:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 573:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 574:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 575:
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
case 576:
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
case 579:
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
case 580:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 581:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 582:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 583:
{ yyval = yyvsp[-1]; }
break;
case 584:
{ yyval = 0; }
break;
case 588:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 589:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 590:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 594:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 595:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 596:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 597:
{ yyval = 0; }
break;
case 598:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 601:
{ yyval = make_node (nod_commit, e_commit_count, yyvsp[0]); }
break;
case 602:
{ yyval = make_node (nod_rollback, e_rollback_count, yyvsp[0]); }
break;
case 605:
{ yyval = make_node (nod_retain, 0, NULL); }
break;
case 606:
{ yyval = NULL; }
break;
case 608:
{ yyval = NULL; }
break;
case 609:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 611:
{ yyval = NULL; }
break;
case 613:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 618:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 619:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 620:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 621:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 622:
{ yyval = yyvsp[0];}
break;
case 624:
{ yyval = yyvsp[0];}
break;
case 625:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 626:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 627:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 628:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 629:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 630:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 631:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 632:
{ yyval = 0; }
break;
case 633:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 634:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 635:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 636:
{ yyval = (dsql_nod*) 0; }
break;
case 637:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 638:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 640:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 641:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 642:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 643:
{ yyval = 0; }
break;
case 645:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 646:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
break;
case 647:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
break;
case 648:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
break;
case 649:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
break;
case 650:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
break;
case 651:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
break;
case 652:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
break;
case 653:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
break;
case 654:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
break;
case 655:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
break;
case 656:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
break;
case 657:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
break;
case 658:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
break;
case 659:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
break;
case 660:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
break;
case 661:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
break;
case 662:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
break;
case 663:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
break;
case 664:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
break;
case 665:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
break;
case 666:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
break;
case 667:
{ yyval = yyvsp[0]; }
break;
case 669:
{ yyval = NULL; }
break;
case 670:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 671:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
break;
case 672:
{ yyval = NULL; }
break;
case 673:
{ yyval = yyvsp[0]; }
break;
case 674:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 675:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 676:
{ yyval = NULL; }
break;
case 677:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 678:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 679:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 681:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 682:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 684:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 685:
{ lex.limit_clause = true; }
break;
case 686:
{ lex.limit_clause = false; }
break;
case 687:
{ lex.first_detection = true; }
break;
case 688:
{ lex.first_detection = false; }
break;
case 689:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 690:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 691:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 692:
{ yyval = 0; }
break;
case 693:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 694:
{ yyval = yyvsp[-2]; }
break;
case 695:
{ yyval = yyvsp[-1]; }
break;
case 696:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 697:
{ yyval = yyvsp[-1]; }
break;
case 698:
{ yyval = yyvsp[0]; }
break;
case 699:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 700:
{ yyval = 0; }
break;
case 701:
{ yyval = make_list (yyvsp[0]); }
break;
case 702:
{ yyval = 0; }
break;
case 704:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 706:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 709:
{ yyval = make_list (yyvsp[0]); }
break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 716:
{ yyval = yyvsp[-1]; }
break;
case 717:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 719:
{ yyval = NULL; }
break;
case 720:
{ yyval = make_list (yyvsp[-1]); }
break;
case 721:
{ yyval = NULL; }
break;
case 723:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 727:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 728:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 729:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 732:
{ yyval = yyvsp[0]; }
break;
case 733:
{ yyval = make_list (yyvsp[-1]); }
break;
case 734:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 735:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 736:
{ yyval = make_list (yyvsp[-1]); }
break;
case 737:
{ yyval = NULL; }
break;
case 739:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 740:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 741:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 742:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 743:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 744:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 745:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 748:
{ yyval = make_list (yyvsp[0]); }
break;
case 749:
{ yyval = NULL; }
break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 753:
{ yyval = yyvsp[0]; }
break;
case 754:
{ yyval = NULL; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 760:
{ yyval = 0; }
break;
case 761:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 762:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 763:
{ yyval = 0; }
break;
case 764:
{ yyval = 0; }
break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 772:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 773:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 775:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 776:
{ yyval = make_list (yyvsp[-1]); }
break;
case 777:
{ yyval = 0; }
break;
case 778:
{ yyval = make_list (yyvsp[0]); }
break;
case 779:
{ yyval = 0; }
break;
case 781:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 783:
{ yyval = 0; }
break;
case 784:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 785:
{ yyval = 0; }
break;
case 786:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 787:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 788:
{ yyval = yyvsp[-1]; }
break;
case 789:
{ yyval = 0; }
break;
case 790:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 792:
{ yyval = NULL; }
break;
case 793:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
				yyvsp[-6], make_list (yyvsp[-5]), make_list (yyvsp[-2]), NULL, yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_insert, (int) e_ins_count,
				yyvsp[-2], yyvsp[-1], NULL, yyvsp[0], NULL); }
break;
case 797:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 798:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 802:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[0]), NULL); }
break;
case 804:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[-2]), make_list (yyvsp[0])); }
break;
case 805:
{ yyval = NULL; }
break;
case 806:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 808:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 810:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 811:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 815:
{ yyval = NULL; }
break;
case 818:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 819:
{ yyval = yyvsp[0]; }
break;
case 820:
{ yyval = NULL; }
break;
case 821:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 824:
{ yyval = NULL; }
break;
case 825:
{ yyval = make_list (yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 829:
{ yyval = NULL; }
break;
case 830:
{ yyval = make_list (yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 834:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 835:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 836:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 838:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 840:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 842:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 843:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 845:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 846:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 847:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 848:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 851:
{ yyval = yyvsp[-1]; }
break;
case 852:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 864:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 865:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 866:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 867:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 868:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 869:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 870:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 871:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 872:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 873:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 874:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 875:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 876:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 877:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 878:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 879:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 880:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 881:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 882:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 883:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 884:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 885:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 886:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 887:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 890:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 891:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 892:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 893:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 894:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 895:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 896:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 897:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 898:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 899:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 900:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 901:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 902:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 903:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 904:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 905:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 906:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 907:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 908:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 909:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 910:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 911:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 912:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 913:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 914:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 915:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 917:
{ yyval = make_list (yyvsp[-1]); }
break;
case 918:
{ yyval = yyvsp[-1]; }
break;
case 929:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 930:
{ yyval = yyvsp[0]; }
break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 932:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 933:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 934:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 936:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 937:
{ yyval = yyvsp[-1]; }
break;
case 938:
{ yyval = yyvsp[-1]; }
break;
case 942:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 943:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 944:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 947:
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
case 948:
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
case 949:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 950:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 952:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 954:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 955:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 956:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 957:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 958:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 959:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 961:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 962:
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
case 963:
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
case 964:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 965:
{ yyval = make_parameter (); }
break;
case 966:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 967:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 968:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 969:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 970:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 971:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 972:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 973:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 974:
{ yyval = yyvsp[0]; }
break;
case 975:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 977:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 978:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 979:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 980:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 981:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 983:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 984:
{ yyval = yyvsp[0];}
break;
case 988:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 989:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 990:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 991:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 992:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 993:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 994:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 995:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 996:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 997:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 998:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 1001:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 1005:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1006:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1007:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1008:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1011:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 1012:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
break;
case 1013:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 1014:
{ yyval = yyvsp[0]; }
break;
case 1015:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 1016:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
break;
case 1017:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
break;
case 1018:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
break;
case 1019:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
break;
case 1020:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
break;
case 1021:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
break;
case 1022:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
break;
case 1023:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1024:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 1025:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 1028:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 1029:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 1030:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1033:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1034:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1035:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1036:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1037:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1038:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1039:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1040:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1044:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			}
break;
case 1045:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 1046:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 1047:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 1048:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 1049:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 1050:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 1051:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 1052:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 1053:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 1058:
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
