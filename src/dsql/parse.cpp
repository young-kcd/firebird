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
    3,    2,    1,    0,    2,    0,    1,    0,    3,    1,
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
  984,    0,  965,  685,  685,  686,  696,  698, 1054,  699,
    0,  700,  690,  686,    0,    0,    0,    0,    0,    0,
    0,  620,  609,    0,  612,  614,  615,  616,  617,  623,
  624, 1077,  739,    0,  120,    0,    0,  117,    0,  118,
    0,  119,    0,    0,  650,  651,  658,    0,  657,  659,
  661,  666,  654,  652,  655,  653,  662,  665,  660,  664,
    0,    0,    0,    0,    0,  670,    0,    0, 1056,    0,
    0,  677,    0,    0,    0,    0,    0,  498,    0,    0,
  377,    0,  218,    0,  419,  128, 1059,    0,  416,  487,
  488,    0,    0,    0,    0,    0,    0,  428,    0,  383,
  384,    0,    0,  607,  605,  975,    0,    0,    0,    0,
    0,    0,    0,  168,  147,    0,  125,    0,    0,    0,
  121,    0,  136,  135,    0,    0,    0,    0,    0,    0,
   76,    0,    0,    0,    0,  798,  519,    0,    0,    0,
    0,  942,  971,    0, 1058,    0,  944,    0,    0,  972,
    0,  966,  957,  956,  955,    0,    0,    0,    0,  947,
  948,  949,  958,  959,  967,  968,    0,  969,  970,    0,
    0,    0,  973,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  961,    0,  833,  939,
  940,  941,  946,  945,  924,    0,    0,    0,  923,    0,
    0,  928,  920,  921,  922,  925,  926,  927,  960,  985,
  986,  987,  999, 1000, 1002, 1003, 1004, 1009, 1010, 1026,
 1027, 1031, 1032,  216,    0,    0,    0,  370,    0,    0,
    0,  826,   27,    0,    0,    0,    0,   72,   70, 1080,
   68,   33,    0,    0,    0,  828,    0,   48,   50,    0,
    0,    0,  599,    0,    0,    0,  693,  695,    0,  702,
    0,    0,    0,  703,  689,    0,  646,    0,  621,    0,
  618,    0,  619,  644,    0,  639,    0,    0,  613,  837,
  807,    0,    0,    0,  126,    0,    0,    0,  596,  595,
  663,  656,    0, 1074,    0,    0,    0,  671,  675,    0,
    0,  780,  682,  681,    0,  183,    0,    0,  144,    0,
  503,    0,  504,  499,    0,    0,  423,  425,  424,    0,
  426,    0,  420,    0,    0,    0,    0,  269,    0,    0,
    0,  432,  433,    0,  480,    0,    0,    0,    0,    0,
    0,    0,  388,  387,  506,    0,    0,    0,  164,    0,
    0,  173,    0,  172,    0,    0,  179,  162,    0,  177,
  169,    0,    0,  542,  577,  545,    0,  578,    0,    0,
  546,    0,    0,    0,  569,  581,  541,  564,  543,  544,
  539,  526,  377,  524,  525,  527,    0,    0,  536,  537,
  538,  540,    0,    0,    0,    0,  124,    0,    0,  122,
    0,  123,    0,  139,    0,    0,  199,    0,    0,  375,
    0,  218,    0,    0,   84,    0,  102,    0,  101, 1060,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  839,  841,  850,  853,  854,  855,  856,  857,  858,  859,
  860,  861,  862,  863,    0,    0,    0,    0,    0,  962,
    0,    0,    0,    0,    0,    0,  963,  964,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  331,
 1081,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  360,    0,    0,  274,  534,    0,    0,    0,  368,    0,
    0,    0,  825,    0,    0,   44,   73,    0,    0,   26,
    0,  831,    0,    0,  794,    0,    0,    0,    0,  598,
    0,  685,    0,  707,    0,    0,    0,    0,  589,    0,
  982,  588,    0,  622,    0,  630,  626,  625,    0,    0,
    0,  641,  629,    0,    0,    0,  802,    0,    0,    0,
  375,  669,  668,  647,    0,    0,    0,    0,    0,    0,
    0,    0,  145,    0,  502,  377,  422,    0,  377,  378,
  427,  493,    0,  492,    0,    0,  265,  375,  238, 1064,
    0,    0,  377,    0,    0,  207,  375,    0,    0,    0,
    0,  243,  237,  240,  241,  242,  431,  485,  484,  430,
  429,    0,    0,    0,    0,    0,    0,  386,    0,  377,
  490,    0,  489,    0,  170,  174,    0,    0,  178,    0,
    0,    0,  565,  566,  582,    0,  579,    0,    0,  572,
  570,  571,    0,    0,    0,    0,  535,    0,    0,    0,
    0,  573,  127,    0,    0,  375,    0,  198,    0,  201,
  203,  204,    0,    0,    0,    0,    0,  218,   79,    0,
    0,   78,    0,    0,    0,    0,  840,  852,    0,    0,
  913,  914,  915,    0,  844,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  760,  762,    0,  757,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1046, 1047, 1048, 1049, 1050, 1051,
 1052, 1053,    0,    0,    0,    0,    0,    0, 1037,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1020,
 1022, 1021,    0,    0,  937,  362,    0,  938, 1024,    0,
  933, 1062,    0,    0,    0,    0,    0,    0,    0,    0,
  332,  333,    0,    0,  943,  835,  834,  372,  371,    0,
  272,    0,    0,    0,  280,  827,    0,    0,    0,   71,
    0,    0,    0,  830,    0,    0,    0,    0,  694,  697,
 1072,  706,    0,    0,    0,    0,  712,  713,  714,  715,
  724,  725,  726,    0,    0,  704,  590,  983,  631,  640,
  635,  634,    0,  645,  838,  808,    0,    0,  375,    0,
    0,  648,  667,    0,  687,  782,  781,    0,  501,    0,
    0,  182,    0,  186,    0,    0,    0,  421,  495,  375,
  375,  277,  378,  270,    0,  209,  377,  211,  377,  378,
  206,  212,    0,    0,    0,    0,  434,  435,  377,  375,
  378,    0,    0,    0,  404,    0,  491,  194,  195,  171,
    0,  181,  553,  554,  981,    0,    0,    0,    0,    0,
  580,    0,    0,  378,    0,    0,  530,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  196,
    0,    0,    0,   83,    0,    0,    0,    0,   91,   85,
    0,    0,   90,   86,    0,  806, 1065,    0,    0,  845,
    0,    0,  851,    0,    0,    0,    0,    0,  889,  888,
    0,    0,    0,  898,  916,    0,  908,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  843,    0,  761,
    0,  797,    0,    0,  218,    0,  988,    0,    0, 1011,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1033,    0,    0,    0,    0,    0,    0,    0,
 1044, 1005, 1006, 1007, 1012, 1008,    0, 1019,    0,    0,
  679, 1023,  950,    0,    0,    0,    0,  267,  286,    0,
    0,  367,  281,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   54,   62,   64,    0,   42,    0,    0,    0,
  812,  832,    0,  811,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  741,    0,    0,    0,    0,    0,    0,
    0,  637,  638,  642,    0,    0,  197,    0,  649,    0,
    0,    0,    0,  187,    0,    0,    0,  417,  221,  222,
  223,  224,  225,  226,  953,    0,    0,  271,    0,    0,
    0,    0,    0,    0,    0,  244,    0,  481,  482,  436,
    0,    0,    0,    0,    0,    0,  377,  508,  378,  180,
 1061,    0,    0,  548,    0,  547,  583,    0,  575,    0,
    0,    0,  528,  556,    0,    0,  558,  561,  563,  375,
    0,    0,    0,  134,    0,  202,  377,    0,  376,  105,
    0,  132,    0,   94,    0,    0,    0,    0,   88,   89,
    0,  906,  907,  847,    0,  849,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  909,    0,    0,    0,    0,
    0,    0,    0,  899,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  768,
    0,    0,    0,  994,  993,    0,    0,  990,  989,    0,
  998,  997,  996,  995,  992,  991,    0,    0, 1039,    0,
    0,    0, 1038,    0,    0,    0,    0,    0,    0,    0,
  365,  335,  334,  273,    0,    0,  283,  284,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  359,    0,    0,    0,    0,    0,  292,  297,  291,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  315,  316,  317,  318,    0,    0,  353,  354,
  355,    0,  810,   66,   58,   59,   65,   60,   61,    0,
   24,    0,    0,   25,  818,  817,    0,  816,  814,  822,
  821,  819,    0,   45,   46,    0,  716,    0,    0,    0,
  746,  744,  742,    0,  743,    0,    0,    0,    0,    0,
  801,    0,  376,  786,  787,  688,    0,  192,  188,    0,
  378,  954,  275,  276,  378,    0,  149,    0,    0,  229,
    0,    0,  245,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  505,  550,    0,  551,    0,    0,    0,    0,
  153,  155,  156,  157,  533,  531,  529,  560,    0,    0,
  378,    0,    0,    0,  373,  377,   92,    0,    0,   98,
   93,    0,    0,    0,    0,    0,  917,    0,  918,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  770,
  759,    0,    0,  771,    0,  767,  217, 1025, 1045, 1001,
    0,    0, 1035, 1034,    0, 1040, 1028, 1030,    0, 1017,
 1018,    0,    0,  282,    0,    0,    0,  314,    0,    0,
  313,  341, 1073,  356,  357,    0,  302,  300,  304,    0,
  301,  293,    0,  298,    0,  344,  299,  303,  305,  306,
  307,  308,  309,  310,  311,  312,    0,    0,  337,    0,
   55,   56,   57,   63,    0,    0,  793,    0,  736,  734,
    0,  727,    0,    0,    0,  750,    0,  684,  378,  374,
  788,  189,  378,  159,  264,    0,    0,  205,  230,    0,
    0,    0,    0,  231,  234,    0,    0,  378,  402,  403,
  399,  398,  401,  400,    0,  549,  576,  158,  146,  154,
    0,  377,  261,  141,  378,  379,    0,    0,   96,    0,
   80,    0,  875,  883,  678,    0,  876,  884,  879,  887,
    0,    0,  877,  885,  878,  886,  872,  880,  873,  881,
  874,  882,  766,    0,    0,    0, 1013, 1036,    0, 1016,
    0,    0,  289,  285,    0,    0,    0,    0,  328,    0,
    0,  352,    0,    0,    0,    0,  347,  345,    0,    0,
    0,   40,  813,    0,    0,  718,  728,    0,    0,  729,
  730,  731,    0,  262,  418,  208,  213,  232,    0,    0,
  235,    0,  247,  249,  406,  378,    0,  381,    0,   77,
   99,  100,    0,    0,    0,  773, 1029,  219,  288,    0,
    0,    0,    0,  351,  350, 1069,  349,    0,    0,    0,
    0,    0,    0,    0,  717,    0,    0,  751,  236,    0,
    0,    0,    0,    0,  263,  378,    0,    0,  772,    0,
    0,    0,    0,    0,  346,  348,    0,    0,    0,    0,
  722,    0,    0,    0,    0,    0,  246,    0,  253,    0,
  252,  382,  133,  775,    0,  290,    0,    0,    0,    0,
    0,    0,  720,  733,  233,    0,    0,  257,  256,  255,
  776,    0,    0,    0,    0,  336,  723,  260,  258,  259,
  326,    0,  342,  323,  324,
};
static short yydgoto[] = {                                      21,
   22,   23,   24,   25,   26,   27,   28, 1573,   30,   31,
 1574, 1575,   34,   35,   36,   37,   38,   39, 1576,   41,
 1577, 1615,  147,  564,  172, 1360, 1611,  148,  835,  149,
  567,  840,  150,  151,  314, 1124,  161,  162, 1361, 1362,
 1363, 1364,  247,  279,  152, 1365,  153,  569,   90,  291,
  461,  516,  743, 1237,  517,  991,  712,  272,  967,  744,
  745,  746, 1244, 1239, 1701,  292,  748,  749,   82,  265,
   83,  287, 1933, 1470,  271,  276,  278,  280,  267,  258,
  260,  273,  283,  180,  368,  370,  372,  365,  451,  730,
  732,  727,  235,  236,  561,  315,  912,  646,  759, 1168,
  455,  735, 1228, 1464,  680,  861, 1200,  638,  639,  661,
  446,  713, 1180, 1679, 1818, 1408, 1680, 1681, 1682, 1683,
 1684,  760,  268,  259,  442,  688,  443,  444,  689,  690,
 1440,  640, 1162, 1163, 1164, 1403,  457,  979,  980,  981,
  982,  914, 1658,  714,  916,  519,  984, 1081,  555, 1527,
 1883, 1409,  520,  521,  522,  523,  524, 1659, 1660,  664,
 1824, 1962, 1426,  922,  923,  909,  924,  925,  926, 1963,
 1964, 2009,  423,  560, 1103, 1578,  243,  657, 1100,  658,
 1173,  556, 1104, 1105, 1350, 1556, 1557, 1558, 1884, 1246,
 1579, 1580, 1581, 1582, 1775, 1583, 1584, 1585, 1586, 1587,
 1588, 1589, 1590, 1591, 1592, 1593, 1594, 1595, 1596, 1597,
 1090, 2000, 1889,  525,  810,  526, 1598, 1776, 1896, 1897,
 1945, 1599, 1600, 1601, 1766,  527,  821,  528,  311,  557,
  558,  529,   43, 1469, 1695,  432,  675,  940, 1438,  676,
  938,   87,   66,  427,  248,  191,  407,  232,  414,  240,
  251,  419,  900,  897,  415,  428,  930,  429,  671, 1191,
  233,  530, 1215,  655,  904,  408,  677, 1439,  105,  246,
  296,  715,  716,  825, 1216,  826, 1217,  719,  720,  721,
  722,  952, 1209, 1207,  723,  724,  725,  960,  726,  957,
   44,   45,   46,   47,   48,   49,  182,  620,   68,  584,
  254,  435,  353,  354,  355,  356,  357,  358,  359,  360,
  361,  867,  605, 1143, 1394,  606,  607,  872,  391,  884,
  392,  625,  393,  887,  187,  396,  628,   50,  190,  402,
 1493,  809,   51,  400,   52,  166,  341,  592,  857,  465,
 1391, 1640,  776,  587,  343, 1400, 1811,  167,  168,  342,
  593,  594,  855, 1122, 1125, 1126, 1127, 1128, 1129, 1130,
 1905, 1955,  531, 1982, 1131, 1132, 1133, 1389, 1910, 1911,
 1912, 1381,  175, 1632, 1805, 1806, 1520, 1029, 1521, 1522,
 1523, 1736, 1934, 1936,  631,  632, 1156, 1646,  575, 1797,
   53,   54,  466,   55,   56,  613,  614,  532, 1112, 1371,
 1617, 1618, 1622,  576,  843,  264,  761,  762, 1006, 1007,
  763,  764,  765,  766,  767,  768,  769,  770,  771,  772,
  773,  774, 1262, 1264, 1265,  533,  534,  535,  536,  537,
  538,  539,  289,  540,  541,  542,  543,  544, 1053,  545,
  546,  547,  548,  549, 1742, 1074,  550,  551,  552,  553,
  792, 1057, 1326, 1321,  793,  155,
};
static short yysindex[] = {                                   9142,
 2915, -123, 3147,   67,  822,   11, 3153, -148,16626,   21,
   82,  343, -123, 1109,  690,11984,  800,11984, -107,   90,
    0,  375,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  439,    0,    0,    0,    0,    0,    0,  396,
    0,    0,    0,    0,    0,    0,    0,17569,11984,  493,
11984,11984,11984,11984,11984,    0,    0,  520, 1172,11984,
11984,11984,  274,11984,  540,11984,11984,    0,11984,11984,
11984,    0,   79,    0,  663,    0,    0,  828,11984,    0,
11984,11984,11984,  924,11984,11984,11984,11984,  540,11984,
11984,11984,11984,11984,    0,11984, 1250,  885,    0,    0,
    0, 1336,    0, 1336,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1017, 1028,  102, 1261,
    0,    0,    0,    0,    0,11984,11984,11984, 1007, 1079,
 1156,11984,  984,  197,  211,  903,  992,    0,11984, 1149,
  955,    0,    0,11984, 1094,11984,11984,11984,11984,    0,
    0,    0,11984, 7415,    0, 1055, 1060, 1248, 1151,  995,
 1412,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  526,    0,    0, 1172,    0,    0, 1124,   78,    0,
    0, 1505,    0,    0,  579,    0,  490,    0,    0, 1095,
    0,    0, 1183,    0,    0, 1160,    0,    0,  -46,    0,
 1355,    0,    0,    0,    0, 1172,    0,    0,  545, 1505,
    0,   71,    0,    0, 1325,    0, 1319,    0, 1336,    0,
    0,    0,    0,    0,    0,    0, 1314,    0,    0, 1124,
    0, 1366,    0, 1223,    0,    0,    0, 1124,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,15474,11984,
 1301,    0,11984,    0,    0,    0, 1286, 1333,10659,11984,
  852, 1399, 1699, 1428, 1441, 1447, 1392, 1396,  115, 1256,
    0,15730,    0,    0,    0,    0,    0,    0,    0,    0,
13426,    0,    0,    0, 1346,11984, 1458, 1367,  858,11984,
 1371,    0,    0,  955,    0,    0,    0,    0,    0,    0,
    0,    0,    0,11984,    0, 1172, 1505,    0, 1325,    0,
 1336,    0, 1454, 1406,    0,    0,    0, 1518,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1487,11984,11984, 1467, 1361,    0,15730, 1442,    0, 1442,
15730,    0,  381, 1360, 1373, 1362, 1412,    0, 1603,  873,
    0,11984,    0,  526,    0,    0,    0, 1585,    0,    0,
    0,11984, 1301,12519, 1436,12782, 1851,    0,17569,    0,
    0, 1013, 1469,    0,    0,    0, 1864, 1864, 1172, 1556,
 1172,  712,  -46,    0,    0,11305,    0,11984,11984,11984,
    0, 1301,    0,    0, 1640,  706, 1886,11984, 1663,11984,
    0, 1888,10117, 9474, 1540,    0,    0, 1891, 1898, 1903,
 1504,    0,    0, 1922,    0, 1925,    0, 1927, 1928,    0,
 1929,    0,    0,    0,    0,    0, 1930, 1522, 1530,    0,
    0,    0,    0,    0,    0,    0, 1931,    0,    0,13682,
 1938, 1941,    0, 1945, 1635, 1960, 1962, 1964, 1967, 1971,
 1977,15730,15730,13938,11984, 1984,    0, 2876,    0,    0,
    0,    0,    0,    0,    0, 1935, 1618, 1985,    0,    0,
 1991,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11305, 1978,  987,    0, 1998, 1783,
 1601,    0,    0,11984,11984,11984,   87,    0,    0,    0,
    0,    0,11984,11984,  -86,    0,11984,    0,    0, 1286,
 1654,10659,    0,11984,13938, 1493,    0,    0,15730,    0,
 2372, 1731, 2006,    0,    0,  144,    0,  897,    0,  -39,
    0,  -39,    0,    0, 2007,    0,  188, 1636,    0,    0,
    0, 2009,  101, 2001,    0, 1301, 2025, 1804,    0,    0,
    0,    0,  737,    0, 1730, 2032,11984,    0,    0, 2067,
 2035,    0,    0,    0,  436,    0, 1738, 1739,    0, 1172,
    0, 1743,    0,    0, 1812, 1813,    0,    0,    0, 1795,
    0,11305,    0, 1172, 1729,11305, 1622,    0, 1832,11984,
17324,    0,    0,  958,    0,11984,  460,  579, 1710, 1673,
 1642,    0,    0,    0,    0,  808, 1728,  173,    0, 1671,
  540,    0, 1172,    0, 1850, 1778,    0,    0,  712,    0,
    0,  146, 1702,    0,    0,    0, 1704,    0, 1752, 2111,
    0, 1811, 2125,  469,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2075,  606,    0,    0,
    0,    0,  121, 2128, 2132, 2125,    0, 1172, 1505,    0,
 1858,    0, 1916,    0, 1172, 1172,    0,12519,  490,    0,
 1806,    0, 1781, 2141,    0, 5520,    0, 1805,    0,    0,
 1814, 2146, 9730, 2151,    0,    0,    0, 9218, 7732,  527,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  855, 1817, 1293,15730,  689,    0,
11984,15730, 1309, 1311, 1321, 1606,    0,    0,15730, 9986,
 2876, 1753,   42,15730,15730, 9986, 1884,15730,15730,15730,
15730,15730,10242,  890,  890, 1787, 1639,  396, 2159,    0,
    0,14194,11984,15730,15730,15730,15730,15730,15730, 6710,
    0,15730, 6947,    0,    0, 1926, 2139,11984,    0,11984,
 1907,11984,    0, 1785, 1788,    0,    0, 1764,10659,    0,
 1892,    0, 1653, 2177,    0, 1892,11984,11984, 2180,    0,
 1787,    0, 1847,    0,11984, 9348, 1784,15730,    0,  537,
    0,    0,  -21,    0, 1786,    0,    0,    0,11984, 1087,
11984,    0,    0,11984,11984, 1540,    0,15730, 1961,12519,
    0,    0,    0,    0,  737,11984, 1890, 2187, 1740,15730,
15730, 1172,    0,  561,    0,    0,    0, 2195,    0,    0,
    0,    0, 1172,    0,  242,11984,    0,    0,    0,    0,
 1963, 2199,    0, 1943, 1973,    0,    0, 1901, 1902, 1336,
 1968,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  540,11984,17422, 1873, 1877, 1879,    0,  540,    0,
    0,  595,    0, 1265,    0,    0, 1833, 1172,    0,10117,
   86, 1852,    0,    0,    0,  540,    0, 2111,  111,    0,
    0,    0, 1943,  111, 1863,  111,    0, 2228,  540,  540,
  540,    0,    0, 1301,11984,    0, 1923,    0, 1658,    0,
    0,    0, 1013, 1442,11984, 1665,  137,    0,    0, 2234,
   23,    0,10117,11984, 1442,10508,    0,    0, 1442, 9730,
    0,    0,    0,10736,    0,  171,  551,15730,15730,10775,
 2237,   -3,11042,15730,11309, 1043,11576,11843,14450,12110,
12377,12644, 9986, 9986,    0,    0, 1918,    0, 2238,  995,
15730,15730, 2676,15730, 2241,15730, 2255, 1913,15730,15730,
15730,15730,15730,15730,    0,    0,    0,    0,    0,    0,
    0,    0, 1986, 1383,  572,15730,  575,15730,    0, 9986,
 2689, 2773,  457,11984, 1980, 1989, 2136, 2206, 2222,    0,
    0,    0, 1737,14706,    0,    0,  995,    0,    0, 1700,
    0,    0, 2008,  890,  890, 1133, 1133, 1009,10392, 2257,
    0,    0, 2251, 2876,    0,    0,    0,    0,    0, 1705,
    0, 1871, 2040, 1907,    0,    0,11717,11717, 1924,    0,
  -31, 1950,11984,    0,15730, 1950, 1995, 1996,    0,    0,
    0,    0, 7156, 2275, 2274, 1799,    0,    0,    0,    0,
    0,    0,    0, 9986, 1994,    0,    0,    0,    0,    0,
    0,    0,  -37,    0,    0,    0, 1817, 2876,    0, 1750,
 1442,    0,    0,  737,    0,    0,    0, 2876,    0, 1864,
 2055,    0,  561,    0, 2282, 9986, 4954,    0,    0,    0,
    0,    0,    0,    0, 1907,    0,    0,    0,    0,    0,
    0,    0, 1336, 1336, 1830, 2287,    0,    0,    0,    0,
    0,  422,  -25,  511,    0, 2065,    0,    0,    0,    0,
11984,    0,    0,    0,    0,   67, 1755, 1917, 1926, 2292,
    0, 1671, 1761,    0, 2276, 1034,    0,11984, 1039,  540,
 2299, 2300, 2301, 2079,  490, 1907, 1864, 1739,12519,    0,
 1728, 1899, 9102,    0,  540, 1975, 2041, 5520,    0,    0,
  540, -135,    0,    0, 2042,    0,    0, 2309, 2310,    0,
 9986, 9986,    0, 9986, 9986, 2810, 2876, 2315,    0,    0,
 2876, 2320,13938,    0,    0, 2047,    0,  553, 2325, 2876,
 2328, 1803, 2330, 2876, 2334,15730,15730, 2237,15730,14962,
 2336, 2876, 2337, 2340, 2876, 2342,15730, 2876, 2343, 2876,
 2344, 2345, 2876, 2347, 2349, 2876, 2350,    0, 2129,    0,
17491,    0, 2253, 2316,    0, 2365,    0, 2387,15730,    0,
 2409, 2417, 2425, 2433, 2728, 2738,15730,15730,15730, 2876,
 1965,15730,    0,15730, 2876, 2088,  754,15730,15730,15730,
    0,    0,    0,    0,    0,    0,15730,    0,15730, 1434,
    0,    0,    0, 1762,10392, 9604,11984,    0,    0,11984,
17131,    0,    0,11984,11984,11984,11984,11984,11984, 1954,
 2352, 2355,    0,    0,    0, 1954,    0,  235,  235,  262,
    0,    0, 1782,    0,11717,11717, 2360, 1799, 2361,15730,
 2145, 9348, 2022,    0, 2022, 1170, 2022, 2072, 2077, 2148,
 2095,    0,    0,    0,  995, 1907,    0, 1899,    0, 1201,
 1671, 2039, 1671,    0, 9986,  424, 1538,    0,    0,    0,
    0,    0,    0,    0,    0, 4954, 4954,    0, 2040,15730,
 4954, 2140, 2026, 1830,   79,    0, 9986,    0,    0,    0,
 2054, 2059, 2060, 2062, 2070, 2074,    0,    0,    0,    0,
    0, 2403,   67,    0, 2011,    0,    0,  111,    0, 2091,
  111,  111,    0,    0, 1926, 2424,    0,    0,    0,    0,
 1013, 2040, 1671,    0, 1739,    0,    0, 2193,    0,    0,
 2187,    0, 2431,    0, 2432, 1172,  -91, 2436,    0,    0,
 1172,    0,    0,    0, 2129,    0, 2129,15730, 1442, 1442,
 1792,  396, 2440,15730, 2167,    0, 1442, 1442,15730, 1442,
 1442, 2823, 2876,    0, 1937,15730, 2876, 1442, 1442, 1442,
 1442, 2876, 1442, 1442, 1442, 1442, 1442, 1442,11984,    0,
 2444, 2442, 1296,    0,    0,11305, 2446,    0,    0, 2745,
    0,    0,    0,    0,    0,    0, 2759, 1695,    0,15730,
 2185, 2066,    0,15730, 2860, 1797, 2944, 2870, 2938,15730,
    0,    0,    0,    0,11305, 2435,    0,    0, 2207,    0,
11984, -105, 2437, 2461, 2166,15730, 2447,    0,11984,11984,
11984,    0, 2448, 2450, 2454, 2169, 2455,    0,    0,    0,
 2214,16996, 2462, 2463, 2465, 2466, 2467, 2468, 2470, 2474,
 2475, 2477,    0,    0,    0,    0,  418, 2479,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2196,
    0,11717,11717,    0,    0,    0, 2113,    0,    0,    0,
    0,    0, 2010,    0,    0, 2145,    0, 1828,11984, 1799,
    0,    0,    0, 2200,    0, 9348, 9348,15730, 9986, 1540,
    0, 2040,    0,    0,    0,    0, 1265,    0,    0,  453,
    0,    0,    0,    0,    0, 2876,    0, 2270, 2140,    0,
  988,11984,    0, 2215,  667, 2115, 2218, 2121, 2259, 2224,
 2262, 1907,    0,    0, 2521,    0, 2524, 2220, 2270, 2091,
    0,    0,    0,    0,    0,    0,    0,    0, 1907, 1728,
    0, 1265, 2065, 2188,    0,    0,    0, 2205,  -85,    0,
    0, 1926, 2211, 2876, 2531, 2537,    0,  995,    0, 2876,
15730, 2538, 2539, 2876, 2540, 2543,15730,15730, 2876, 2545,
 2547, 2548, 2551, 2553, 2554, 2556, 2558, 2563, 2566,    0,
    0,17491, 2569,    0,11984,    0,    0,    0,    0,    0,
15730, 2570,    0,    0,15730,    0,    0,    0,15730,    0,
    0, 2955,  257,    0, 2298,15730,15730,    0, 9986, 2876,
    0,    0,    0,    0,    0,11984,    0,    0,    0,10392,
    0,    0,  914,    0, 2172,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   -5, 2581,    0, 2245,
    0,    0,    0,    0,  235,11984,    0,11984,    0,    0,
 9348,    0, 1356, 2876, 2583,    0,  527,    0,    0,    0,
    0,    0,    0,    0,    0, 2587,11984,    0,    0, 2281,
 2290,11984, 1830,    0,    0, 1336,11984,    0,    0,    0,
    0,    0,    0,    0, 2040,    0,    0,    0,    0,    0,
 2040,    0,    0,    0,    0,    0,15730, 1172,    0, 2173,
    0, 1172,    0,    0,    0, 2876,    0,    0,    0,    0,
 2876, 2876,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,11984, 2304, 2876,    0,    0, 3001,    0,
 4954, 4954,    0,    0, 2592, 2876, 1186,  673,    0, 2297,
 2257,    0,11984,11984,   67,  208,    0,    0, 2134, 2302,
 9986,    0,    0,  184, 2599,    0,    0, 9986, 2600,    0,
    0,    0,15730,    0,    0,    0,    0,    0, 1830, 1336,
    0, 2265,    0,    0,    0,    0, 2065,    0, 2876,    0,
    0,    0, 2601, 2603, 2610,    0,    0,    0,    0, 1442,
10392, 2225,10392,    0,    0,    0,    0,17131,  914,15730,
10392,  697,10392,11984,    0,  527,11984,    0,    0, 2265,
  401, 1830, 2277, 2279,    0,    0, 2615,11984,    0,11984,
 2618, 2257,17131, 2257,    0,    0, 1391,  225, 2359, 2257,
    0, 1860, 1861, 1830,  633,  633,    0, 2367,    0, 2229,
    0,    0,    0,    0, 2625,    0, 2366,10392, 2381, 2371,
17131,11984,    0,    0,    0, 2221,  470,    0,    0,    0,
    0,17131,  227,11984,17131,    0,    0,    0,    0,    0,
    0,17131,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  253,  795,    0,    0,    0,    0,    0,    0,    0,
    0,16702,  220,12907,    0,    0,    0,    0,    0,    0,
    0, 2675,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  134,    0,    0,    0,    0,    0,    0,  302,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1033,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2346,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  464, 2306,    0,    0,
    0,  189,    0,  189,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2307,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1146,    0,    0,15218,13170,    0,    0,    0,
 1179,    0,    0,  236,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  333,    0, 2271,  507,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  471,    0,    0,    0,    0,  645,    0,    0,    0,
    0,    0, 1258,    0,    0,    0,    0,    0,  151,    0,
11572,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,  485,    0,    0, 2645,    0,    0,    0, 2423,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  119,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  138,    0,
 2429,    0,    0,    0,    0,    0,12251,    0,    0,    0,
    0,    0,  871,    0,    0,    0,    0,    0,    0,13044,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  355,    0,    0, 1270,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  471,    0, 2645,    0,
 2423,    0, 1299,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  317,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1304,    0, 2412,    0,
    0,    0,    0, 1316,    0,    0,    0,  326,    0,    0,
    0,    0, 2429,  775,17650,    0, 1323,    0,    0,    0,
    0,  203,    0,    0,    0,    0, 2247, 2247,    0,    0,
    0, 1335,  241,    0,    0,    0,    0,    0,    0,    0,
    0, 2429,    0,    0, 1235,    0,    0,    0,    0,    0,
    0,11038,    0,    0,  143,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2246,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 3052,    0,    0,    0,
 3352, 3652,    0, 3952, 4252,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  150,    0,    0,
    0,    0,    0,    0,    0, 4552, 1340,  222,    0, 2752,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1401,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,12251,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 9860,    0, 2378,    0,    0,    0,    0,    0,    0,  802,
    0,  802,    0,    0,  642,    0,  868, 1236,    0,    0,
    0,    0,  119,    0,    0, 2429,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  183,
  359,    0,    0,    0,  683,    0,    0, 1006,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1408,    0,    0,    0,    0,    0,
    0,    0,    0, 2412,    0,    0, 1188,    0, 2249,    0,
    0,  421,    0,    0,    0,    0,  240,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1439,    0,
    0, 2916,16251,    0,    0,    0,16263,    0,    0,16472,
    0,    0,16530,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1378, 1822,    0,    0,
    0,    0,16641,16300,    0,16530,    0,    0,  471,    0,
    0,    0,    0,    0,    0,    0,    0,  775, 1313,    0,
    0,    0,    0, 2293,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1999, 7542, 7739,    0,    0,  217,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2660,   58,15986,    0,15986,    0,
    0,    0,15986,15986,15986,    0,    0,    0,    0,    0,
 2266,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 5741, 6030, 2668,    0,  100,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1760,    0,    0,    0,    0,
 2443,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  320,    0,    0,    0,    0,  320,    0,    0, 1448,    0,
    0,    0,    0,    0,    0,    0, 1000,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  444,
    0,    0,    0,    0,    0,  143,    0,    0,    0,  775,
    0,    0,    0,    0,    0,    0,    0,  387,  190,    0,
    0,    0,    0,  943,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1862,    0,    0,    0,    0,    0,
 2674,    0,    0, 1839, 1953,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  452,  705,  741,    0,    0,    0,
    0,    0,    0,  933,    0,    0,    0,    0,    0,    0,
    0, 3343,    0,    0,    0,    0,    0,16472,    0,    0,
    0,    0,  668,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2429,    0,    0,  289,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 5809,    0,    0,    0,
  155,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2680,    0,    0, 1453,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2681,    0,    0,    0,
    0,    0, 4852, 6319, 6608, 5152, 5452,    0,    0, 1472,
    0,    0,    0, 1166,    0,    0,    0,    0,    0,    0,
    0,13300,    0, 2456,    0,    0,    0,    0,    0,    0,
    0, 1491,    0,    0,    0, 1491,    0,    0,    0,    0,
    0,    0,    0, 6817,  157,  635,    0,    0,    0,    0,
    0,    0,    0,    0,  643,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   58,  248,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  765,    0, 2247,
 2285,    0, 1126,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2443,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1403,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1497,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3643,    0,
    0,    0,    0,    0, 1239,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1313, 2443, 2247, 1511,  775,    0,
 2460, 1520,    0,    0,    0, 5809,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1178,    0,    0,    0,
 1212,    0,    0,    0,    0,    0,    0,    0,    0, 1354,
    0, 1766,    0, 2515,    0,    0,    0,    0,    0,    0,
    0, 2546,    0,    0, 6529,    0,    0, 7811,    0, 7883,
    0,    0, 7943,    0,    0, 8038,    0,    0, 2164,    0,
 2660,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2317,
    0,    0,    0,    0,  806,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -38,    0,    0,    0,    0,    0,    0,    0,  136, 1525,
  103,  142,    0,    0,    0, 1525,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2386, 1246,    0,
 7241,    0, 2390,    0, 2390, 2386, 2390,    0,    0,    0,
  275,    0,    0,    0, 1453, 2443,    0, 1520,    0,    0,
    0, 2295,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1320,    0, 1403, 2346,    0,    0,    0,    0,    0,
  785,  793,  807,  934,  963, 1027,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  888,
    0,    0,    0,    0, 2602,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1528,    0,    0,    0,    0,    0,
 1547,    0,    0,    0,    0,    0,  215,    0,    0,    0,
    0,    0,    0,    0,  113,    0,  123,    0,    0,    0,
    0,  100,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 8110,    0, 8182,    0, 8242,    0,    0,    0,
    0, 8314,    0,    0,    0,    0,    0,    0, 1368,    0,
    0, 2701,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2710,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,16448,
 2694,    0,    0,    0,    0,    0,    0, 1706, 1816,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   14,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1344,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1549,    0,    0, 7032,    0,    0,    0,  942,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  631,
    0,    0,    0,    0,    0,    0,  166,    0,    0,    0,
    0,    0,    0,    0,    0, 2716,    0, 1500, 1776,    0,
 2412,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2443,    0,    0,    0,    0,    0,    0, 1578,  978,
    0,    0,    0,    0,    0,    0,    0,    0, 2443, 2460,
    0,  327,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  447,    0, 8398,    0,    0,    0, 2681,    0, 8481,
    0,    0,    0, 8553,    0,    0,    0,    0, 8613,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2660,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2699,    0,    0, 2708,    0,    0,    0, 2713,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2469,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8964,    0,    0,
    0,    0, 2386, 1003, 1029,    0,  641,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2887,    0,    0,  114,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  216,
    0,    0,    0,    0,    0, 8724,    0,    0,    0,    0,
 8757, 8846,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1870, 2735,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2723, 2725,    0,    0,    0,
 2730,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1589, 8918,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2887, 1093,
    0,  185,    0,    0,    0,    0,    0,    0, 2716,    0,
    0,    0, 2750,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  431,    0,    0,
    0,    0,    0,    0,    0, 9050,    0,    0,    0, 1130,
    0, 1403, 1602, 1714,    0,    0,    0,    0,    0,    0,
    0, 2744,  431, 2746,    0,    0,    0, 2506,    0, 1598,
    0,    0,    0, 2887,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,16880,    0,    0,    0,
  431,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  431,    0,    0,  431,    0,    0,    0,    0,    0,
    0,  431,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, 2809,    0,    0,
 2819, 2821,    0,    0,    0,    0,    0,    0,    1,    0,
 2822,   -2, 2649, 2243,   22, -889, 1459, 2663, 1979, 2664,
 2252,    0,    0, 2507, -108,    7,    0,    0,    0,    0,
  108,  297,  -60,  -83,  -56, -288, 2511, 1993,    0,    0,
    0,  727,    0,    0,    4, 1599, -668,  -59,-1093, 2096,
 1853, -385,    0, 1608,    0, 2753, 1846, 1897,    0,    0,
    0, -600,  -58,    0,    0,    0,    0,    0, 2768,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -66,    0, -623, -447, 1617, -399, 1118,-1051,
    0,    0,    0,    0, -400, -151, -384, 1628, -417,  -62,
    0, -632, 1895,    0, 1181,-1171,    0, 1182,    0,    0,
 2471, -739,  -30,    0,    0,    0,    0, 2438,    0, 2183,
 1666,    0,    0,    0, 1713,    0, 2519, 2012, 1660, 2472,
 2473,    0,    0, -529, 1983,  -49, -694, 1073, -287,    0,
    0,    0,-1108,-1065,-1023, -995, -979,    0, 1234,-1092,
    0,  946,-1388, 1247,    0, 2244,    0,    0,    0,  945,
  961,  941, -152, -348,-1074,-1066,    0,    0,    0, 2023,
    0, -785,    0, 1826,    0,    0,    0,    0,    0,-1316,
 -528,    0,    0,    0,    0, -316,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
-1025,    0,    0, -802,    0, -798, 1363, 1158,    0,  985,
    0,    0,    0,    0,    0,    0,    0, -481,    0,    0,
 2108, -154, -531, 1539, 1295, -611, -877,-1111,-1262,    0,
    0,   25,    0,    0,    0,    0,    0, 2510,    0,    0,
    0,    0,    0,    0, 2527, 2280,    0,    0,    0,    0,
    0,   -9, -553,    0,    0, 2536,    0,    0,    0,   -1,
    0,    0,    0, -426, 1988, -423, 1492,    0,    0,    0,
    0,    0,    0, -409,    0,    0,    0, 2219,    0, 1992,
    0,    0,    0,    0,    0,    0,  -50,    0, 2936,    0,
    0,    0,    0,    0, 2598,    0,    0,    0,    0, 2357,
    0, 2354,    0,    0,    0, 2093,    0,    0,    0, -680,
    0, -278,    0,    0,    0,    0,    0, -393, -700, -951,
 2250,    0,  610,    0,    0,    0,    0,    0,    0, -330,
    0,    0, -844, -289,  738,    0,    0,    0, 2796, 1076,
    0, 2106,-1151,-1232,    0,-1046, 1844, -893,    0,    0,
    0,    0, -158,    0,    0,    0,    0, 1582,    0,    0,
    0,    0, 2878,  -28,    0, 1057, 2197,    0, 1244,    0,
 1463,    0, -512,    0,    0, 2094,    0,    0,    0,    0,
    0,    0, 2380,    0,    0,    0, -532,-1214, 2153, 1878,
-1156,    0,    0,    0,    0, -763, -341, -577,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  929, 1725,    0,    0,    0, -959,    0,    0,
    0, 1597,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,-1115, 1682,    0,    0,
};
#define YYTABLESIZE 18182
static short yytable[] = {                                     154,
   40,   86,  239,  888,  266,  316,  173,  261,  181,  335,
  338,  650,  334,  337,  174,  363,  277, 1091,  306,  717,
  262, 1092,  718,  282,  687,  824,  297,  652,  913,  889,
  568, 1147,  807, 1559,  250, 1663, 1352,  681,  301,  303,
  305,  842,  862,  845, 1101,  588,  307,  611,  234,  237,
 1055,  241,  244,  173,  249,  252, 1063,  779, 1410, 1097,
  263,  237,  252, 1344,  244,  300,  173,  249,  242,  281,
  154,  252,  257,  308,  659, 1030, 1378,  992, 1302,  293,
  270,  173,  295,  237,  245,  293,  252,  241,  244,  174,
  173,  249,  281,  154,  252,  371,  244,  275,  304,  274,
 1419, 1411,   52,  733,  302, 1231,  322, 1077,  324,  366,
 1145,   85,  309,  824,  626, 1446,  779,  452,  756, 1467,
  808, 1418,  901,  274,  943, 1341,  905,  983, 1422, 1206,
  839,  915,  373,  672,  656, 1090, 1603,  363,  345, 1430,
  779,   53,  758, 1412,  875,  320,  263,  173,  263,  951,
  166, 1462,  154,  846,  824, 1212,  709,  824,  320,  252,
  969,   52, 1450,  848,  362,  193,  237,  244,  173,  281,
  459, 1413,  824,  181,  672,  998, 1236,  756,  323, 1090,
 1005, 1699,  785,  367,  254,  951, 1151, 1414,  860,  789,
  951,  808,  672,  951, 1090,   87,  363,  709,   87,  369,
   53,  758,  507, 1539, 1152,  612, 1541, 1415,  951,  166,
 1172, 1253, 1619, 1175,  616,  709,  755,  942, 1366,  604,
  893,  361, 1182,  785,  193,  254,  785,  832,  254, 1629,
  789,  871,  824,  789,  717,  740,  332,  718,  416,  405,
  167,  785,  951,  254, 1653, 1654,  106,  809,  789, 1657,
  336, 1949,  604, 1764, 1765,   95,   97,  755,  600,  333,
  554,  507,  618,  562,  921, 1849,  296,  879, 1345,  447,
 1345,  687, 1433,  333,  754,  755,  338,  837,  604,   85,
  361, 1226,  876, 1298, 1299, 1368, 1091,  597,  140,  106,
 1092,  809,  156,  568,  740, 1242, 1266,  333,  405,  167,
  263,  779, 1171,  263, 1899,  437,  809, 1410, 1410,  570,
  154,  604, 1410,  963,  610,  754,  674, 1882,  294,  815,
 1327, 1642,   14,   67,  333,  494,  193,   91,  338, 1661,
 1080,  865,  676,  754,  420, 1630,  241, 1088,  453,  438,
  173,  439,  779,  285, 1267, 1058, 1059,  140, 1479,  844,
 1411, 1411, 1655,  158,  627, 1411,  157,  674,  778, 1392,
  779, 1687,  651,  440,  808,  107, 1700, 1603, 1268,  615,
 1243,  604,  554,  676,  262,  674,  667,  286,  815,  669,
 1480,  728,  624,  624,  494,  193,  673, 1673,  441,  731,
  183,  676, 1412, 1412, 1757, 1691, 1369, 1412, 1850,  778,
  866,  741,  263,   14, 1189, 1213,  338,  421, 1434,  602,
 1393,  997,  263,  627,  263,  612,  263,  778,  998,  672,
 1413, 1413,  998,  193, 1743, 1413, 1406,  673, 1746,  454,
 1845,  582, 1251,  185, 1921,  193, 1414, 1414,  237,  244,
  249, 1414,  682, 1641,  684,  673, 1395,  166,  173, 1890,
  241,  391, 1232,  750, 1396,  729, 1415, 1415,  338,  737,
  747, 1415,  193, 1248, 1651,  184,  507, 1249,  193,  672,
  193,  709, 1185, 1399, 1798, 1416, 1417,  817,  815,  739,
  816,  709,  818,  709, 1060, 1624, 1625,  557, 1999,  824,
  557,  846,  709, 1813,  166, 1429, 1165,  785,  756, 1167,
 1330,  848,  870,  405,  789,  811,  792,  756, 1948,  193,
  391, 1484, 1485, 1178, 1486, 1487,   84,  193,  785, 1953,
   95,   97,  758, 1235,  610,  789, 1135, 2022,  944,  319,
 1959,  755,  838,  850,  269, 1205,  709,  167, 1170,  363,
 1196,  755, 1552,  755,  464,  709, 1553,  792,   52, 1252,
  568,  951,  755, 1881,  173,  244,  570,   87, 1146,  968,
  331, 1554, 1119,  173,  824,  792, 1661,  173,  950,  992,
  779,  836,  570, 1987,  181, 1809,  974,  562, 1842,  672,
 1372, 1090, 1097, 1690,  167,  834,  193,   53,  709,  754,
 1803, 1377,  507,  331,  841, 2005,  755, 1835,  846, 1814,
  709, 1238,  709, 1815, 1816,  755,  824, 1153, 1851, 1471,
  754,  917,  779, 1461, 1841,  740,  779,  263,  656, 1398,
  859,  945,  331,  361,  740, 1224,  883,  809,  785, 1878,
  758,  756,  140, 1373,  710,  789,  809,  779, 1903, 1843,
  753,  633,  749,  894,  385,  740,  331,  604,  755,  941,
  910,  263,  674,  604,  754,  758,  910,  902, 1250,  166,
  331, 1093,  755,  754, 1966, 1650,  159,  150,  676,  709,
  193,  758, 1098,  778,  193,  710,  785,  254,  710,  740,
  815,  753,  790,  749,   84, 1023,  946, 1665,  262,  758,
  494,  809,  627,  710,  778,  785,  966, 2023, 1985,  753,
  633,  749,  789,  385,  389,  274,  754, 1828, 1138, 1938,
 1939, 1205,  813, 1942, 1023,  391,  612,  814, 1023,  557,
  754, 1981,  673,  790,  636,  627,  150,  369,  263,  755,
 1035,  973, 1787, 1603,  266, 1423, 1424, 1979,  977,  978,
  390,  790, 1802,  961, 1891,  338,  430,  779,  740,  167,
 1037,  627,  557,  186,  627, 1431, 1855, 1914, 1603, 1401,
  809, 1915,  674,  389,  791, 1689, 2019,  137, 1925, 2017,
  627,  252, 1410, 1410, 1926,  188, 1924, 1420,  554, 1421,
  554, 1491, 1106,  409,  396, 1472, 1603,  754, 1023, 1428,
 1138, 1708,  397, 1928,  557, 1808,  627, 1603,  160,  390,
 1603,  632, 1024, 1082,  778,  791,  393, 1603, 1435,  962,
 1063,  238, 1254,  263,  779, 1411, 1411, 2020,  263,  431,
  263,  792,  263,  791, 1664,  610, 1463,  189,  410,  570,
  262, 1024,  673, 1023, 1986, 1024,  424,  173,  244,  425,
  636,  391,  792,  396, 1436, 1121,  244, 1546,  438,  557,
 1238,  397, 1495,  448,  836, 1432,  554, 1412, 1412,  173,
  632,  173, 1788,  891,  263,  393,  369,  643, 1117, 1492,
  263,  778, 1187,  266, 1965,  338,  624, 1967, 1322, 1323,
  965,  426, 1063, 1188, 1190, 1413, 1413,  151,  883,  637,
  604,  636, 1144, 1737, 1677, 1159,  263, 1685, 1628, 1807,
 1496, 1414, 1414,  385,  557, 1024, 1169, 1907,  385, 1221,
 1222, 1223, 1160,  385, 1992, 1972,  928, 1974,  929, 1526,
  253, 1415, 1415,  263,  263, 1978,  643, 1980, 1023, 1255,
 1093,  817,  193,  392, 1023,  411,  818,  137,  137,  449,
  750,  711,  184,  150,  150,  758,  151, 1203,  339,  710,
 1024, 1202,  792,  412,  612,  753,  274,  749, 1023,  710,
 1621,  710,  395, 1195,  418,  173,  758, 1091,  389,  749,
  710, 1092, 2013,  745,  450,  173,  753,  152,  749,  633,
 1210, 1161,  711,  750, 1247,  711,  331,  413, 1034,   84,
  747,  193,  392,  274,  274,  274, 1225,  790, 1319,  756,
  711,  184,  752, 2006,  390,  500, 1233,  633,  685,  634,
  758,  169,  633, 1137,  710, 1023,  462, 1324,  790, 1888,
  753,  395,  749,  710,  467, 1753,  394,  829,  748,  753,
  828,  749,  606, 1331,  385, 1676,  152, 1672,  633,  150,
  756,  633, 2007,  752,  331, 1024,  752,  893,  396,  736,
  239, 1024,  822, 1774,  252,  636,  397,  633,  756,  130,
 1138,  752,  758,  610,  500, 1604,  710, 1693, 1607,  748,
  393, 1197,  753,  589,  749, 1024,  758, 1452,  710,  791,
  710,  595, 1452,  633,  882,  394,  753,  748,  749, 2008,
  239,  606,  824,  130,  389, 1492, 1492,  570,  570,  717,
  791, 1343,  718, 1492, 1492,  935, 1492, 1492,  176, 1043,
 1043,  170,  288,  244, 1492, 1492, 1492, 1492,  171, 1492,
 1492, 1492, 1492, 1492, 1492,  185, 1453,  130,  790,  254,
  390, 1455, 1024,  824,   88,   89,  824,  600, 1091,  632,
 1091,  936, 1092,  758, 1092,  602,  290,  710, 1091,  109,
 1091,  824, 1092,  753, 1092,  749,  648,  883,  255,  256,
 1456, 1952,  339,  377,  151,  952,  813,  239, 1956,  649,
  254,  814, 1904,  254,  396, 1474, 1892,  900,  611,  262,
 1544, 1478,  397,  562,  185,  111, 1093,  486,  254,  255,
  256, 1441,  612, 1025,  177, 1091,  393,  392,  632, 1092,
  184,  632,  340, 1442,  602,  643,  952,  239, 1441,  952,
  791,  867,  184, 1616, 1616, 1620, 1026,  632,  900,  263,
  686,  900, 1893,  263,  952,  178,  395,  817,  815,  193,
  816,  486,  818, 1894,  601,  628,  900,  611,  643,  184,
  179,  937,  298,  632,  274,  184,  486,  184, 1043, 1647,
  112, 1649,  867,  377,  152,  867,  711,  608,  952,  274,
  113,  921, 1812,  500,  643,  274,  711,  643,  711,  610,
  867,  673, 1027,  918, 1608,  500,  193,  711,  312,  829,
  745,  674,  532,  643,  193,  114,  184, 1555,  602,  310,
  394,  173,  347,  863,  628, 1606, 1847,  554,  597, 1519,
  262,  610, 1609,  412,  321,  603,  829, 1844,  500,  643,
  500, 1692,  351, 1276,  756,  413,  608,  752,  193,  227,
  193,  711,  409,  392,  756,  348,  756, 1277,  610,  752,
  711,  532, 1895, 1983,  175,  756,  263,  263,  752,  366,
 1560, 1602,  193,  748,  570,  244,  249,  570,  281,  154,
  919,  349,  395,  868,  350,  748, 1633,  597, 1635, 1820,
  227, 1605,  412,  227,  748,  570,  570,  193,  824,  824,
  351, 1278,  244,  711,  413,  313,  824,  534,  227,  756,
 1821,  409,  752,  185,  325,  711, 1822,  711,  756, 1077,
  920,  752,  317,  175,  868,  185,  352,  868,  366, 1279,
   43, 1073,  248,  318, 1063,  254,  254,  496,  748,  813,
  398,  330,  868,  254,  814,  108,  394,  748,  534, 1975,
 1823,  534,  185,  612,  817,  815,  518,  816,  185,  818,
  185,  756,  817,  815,  752,  816,  534,  818,  176,  900,
 1675,  193, 1927,  248, 1997,  756,  248,   47,  752,  586,
  399,  184,  792,  109,  711, 1994,  326, 1995,  591,   43,
  748,  248,  813, 1280,  824,  110,  496,  814,  824,  185,
 1800,  364, 2016,  867,  748,  817,  815,  165,  816, 1698,
  818,  346, 1141, 2021, 1703,  824, 2024, 1383,  394,  111,
  820,  824,  900, 2025, 1756,  900,  509,  176, 1142,  214,
 1384,  254,  900,  364,  900,  395,   47,  401,  900,  173,
  142,  792,  756,  900,  630,  752,  900, 1519,  635,  380,
  397, 1941,  254, 1385,   41,  824,  867,  143,  254,  867,
  364,  900,  610,  852,  817,  815,  867,  816,  867,  818,
  214,  748,  867,  214,  422,  900,  131,  867,  805,  820,
  867,  237,  339,  900,  112,  509,  900,  900,  214, 1763,
 1247, 1247,  254,  712,  113,  867,  900,  952,  339,  142,
  339,  385, 1602,  628, 1387,  417,  712,  214,  380,  867,
  339,  385,  900,   41,  712,  824,  143,  867,  803,  114,
  867,  867, 1031,  164,  165,  239,  227,  804,  434,  712,
  867,  250,  570,  570,  900,  131,  628,  805, 1039,  900,
 1041, 1093,  436,  712,  433,  868,  867,  791,  445,  362,
 1043,  900,  254,  900,  255,  256,  244,  244, 1733,  804,
  805,  806,  628,  458,  185,  628,  214,  456,  867, 1906,
 1616,  833,  250,  867,  832,  250,  460,  803, 1198, 1199,
  712,  628,  173,  534,  534,  867,  804,  867,  534,  813,
  250,  534,  907, 1734,  814,  906,  464,  813,  868,  403,
  900,  868,  814, 1383,  534, 1735, 1875,  628,  868, 1076,
  868,  404,  822, 1826,  868, 1644, 1384,  138,  138,  868,
  900,  239,  868, 1114,  900, 1645, 1113,  463, 1230, 1318,
  769, 1229,  851,  559,  867, 1234,  853,  868,  988, 1385,
  813,  563,  239,  251,  405,  814,  406, 1922,  239, 1791,
 1793,  868,  173, 1386,  867,  241, 1998,  565,  867,  868,
 1519, 1908,  868,  868,  573,  769,  817,  815,  574,  816,
 1342,  818,  868,  822,  577, 1348,  562,  769, 1347,  534,
 1550, 1092,  239,  583,  251,  578, 1247,  251,  868,  557,
 1387,  579,  712, 1092,  339,  894, 1092,  580, 1923,  813,
  534,  581,  251,  596,  814,  228,  534, 1338,  817,  815,
  868,  816, 1093,  818, 1093,  868,  263, 1900,  362,  612,
 1397,  244, 1093, 1229, 1093, 1444,  608,  868, 1443,  868,
  557, 1449, 1551,  557, 1448, 1345,  894, 1082,  599,  894,
  534, 1960,  173,  598,  612,  621,  228,  241,  557,  228,
  557,  557, 1623,  623,  894,  822, 1944, 1075,  817,  815,
  619,  816, 1707,  818,  228,  822,  622, 1748,  150, 1093,
  822,  627,  612, 1920,  817,  815,  868,  816, 1909,  818,
   14, 1930, 1032,  612, 1036, 1932,  612,  629, 1040, 1042,
 1044, 1097,  557,  612,  241,  557,  868,  641, 1799,  643,
  868,  822, 1388, 1097,  340, 1004, 1097,  250,  250,  150,
  557,  642,  150,  237, 1946,  250,  645, 1120,  817,  815,
  654,  816, 1947,  818,  668, 1033,  665,  150,  610, 1038,
 2003, 2004,  375, 2002,  832,  375, 1054,  562, 1792, 1794,
  777, 1061, 1062,  777,  678, 1065, 1066, 1067, 1068, 1069,
 1073,  683,  734,  610,  679,  738,  740,  742,  775,  518,
  777, 1083, 1084, 1085, 1086, 1087,  518,  778, 1602, 1094,
 1971, 1271,  779, 1275, 1121, 1283, 1286,  263, 1291, 1294,
 1297,  610,  524, 1310,  817,  815,  780,  816,  241,  818,
  241,  781,  610, 1602,  782,  610,  783,  784,  785,  786,
  789,  813,  610,  250,  787,  591,  814,  794,  817,  815,
  795,  816,  788,  818,  796,  797,  483,  484,  485,  251,
  251, 1602, 1121,  524,  250, 1148,  524,  251,  910,  798,
  250,  799, 1602,  800, 1247, 1602,  801,  630, 1158, 1741,
  802,  524, 1602,  813,  493,  494,  803,  557,  814,  820,
 1332,  817,  815,  812,  816,  819,  818,  894,  822, 1333,
  817,  815,  557,  816,  250,  818,  823,  830,  827,  910,
 1102, 1102,  910, 1102, 1102, 1102,  831,  856,  848,  858,
  869,  239,  228, 1337,  874,  873,  557,  910, 1102, 1102,
 1102,  878,  557,  813,  880,  557,  885,  881,  814, 1045,
 1046, 1047, 1048, 1049, 1050, 1051, 1052,  886,  890,  813,
  894,  892,  636,  894,  814,  251,  895,  896,  898, 1102,
  894,  899,  894,  903,  250,  908,  894,  557,  557,  932,
  933,  894,  557,  934,  894,  557,  251,  557,  817,  815,
 1499,  816,  251,  818,  150,  150, 1383,  939,  557,  894,
  331,  948,  150,  813,  947, 1256, 1257, 1261,  814, 1384,
 1270, 1272, 1274,  894, 1282, 1285, 1288, 1290, 1293, 1296,
  953,  894,  954,  955,  894,  894,  251,  239, 1303, 1304,
  956, 1306, 1385, 1308,  894,  958, 1311, 1312, 1313, 1314,
 1315, 1316,  557,  842,  959,  964, 1386,  970,  239,  557,
  894,  971,  975, 1320,  239, 1325, 1334,  817,  815,  976,
  816,  985,  818,  987,  988,  995,  993,  557,  994,  813,
  999, 1340,  894,  557,  814, 1056,  188,  894, 1064, 1078,
  965,  333, 1102, 1387,  842, 1111,  251,  842,  239,  894,
  150,  894, 1107,  813,  557, 1108, 1115,  557,  814, 1109,
  557,  557,  842,  839, 1149, 1139, 1154, 1134,  524,  524,
  832,  150,  518, 1155, 1166, 1176,  524,  150, 1177, 1179,
 1183, 1184, 1201, 1186, 1718, 1084, 1335,  817,  815,  524,
  816, 1192,  818,  911,  557, 1193,  813, 1194,  894, 1208,
  910,  814, 1336,  817,  815,  813,  816, 1220,  818, 1102,
  814,  150, 1218, 1241, 1227, 1102, 1263, 1301,  894, 1300,
 1102, 1307,  894, 1102,  813, 1059, 1084, 1084, 1084, 1084,
 1084, 1084, 1084, 1524,  817,  815, 1346,  816, 1309,  818,
 1345, 1367, 1317, 1084, 1084, 1084, 1084, 1084, 1349, 1351,
 1370, 1375, 1376,  910, 1380, 1388,  910, 1382, 1390, 1102,
 1402, 1405, 1425,  910,  524,  910, 1427, 1102, 1437,  910,
 1445,  285, 1447, 1451,  910, 1102, 1084,  910, 1084, 1457,
 1458, 1459, 1460,  813, 1468,  524, 1476, 1481,  814, 1482,
 1483,  524,  910, 1102, 1489, 1102, 1525,  817,  815, 1490,
  816, 1235,  818, 1494, 1497,  286,  910, 1498, 1102, 1500,
 1102, 1102, 1102, 1501,  910, 1508, 1509,  910,  910, 1510,
  518, 1511, 1513, 1514, 1515,  524, 1516,  910, 1517, 1518,
 1023, 1540, 1543, 1502, 1503, 1612, 1505, 1507, 1613, 1610,
 1626, 1627, 1631,  910, 1512, 1528,  817,  815,  854,  816,
 1636,  818,  813,  817,  815, 1637,  816,  814,  818, 1102,
 1638, 1639, 1648,  660, 1662,  910, 1530, 1529,  817,  815,
  910,  816, 1666,  818, 1537, 1538, 1325, 1667, 1668, 1325,
 1669, 1320,  910, 1674,  910, 1545,  518, 1547, 1670, 1531,
  817,  815, 1671,  816, 1548,  818, 1549, 1532,  817,  815,
 1205,  816, 1678,  818, 1688, 1533,  817,  815, 1694,  816,
 1696,  818, 1697, 1534,  817,  815, 1702,  816,  842,  818,
 1709,  842,  813, 1711, 1731, 1732, 1738,  814,  842, 1744,
  842,  910, 1745, 1754,  842, 1758, 1755,  518,  813,  842,
 1759,  157,  842,  814, 1770, 1761, 1767, 1084, 1768, 1084,
 1084,  910, 1769, 1771,  871,  910, 1084,  842, 1772, 1790,
 1777, 1778, 1084, 1779, 1780, 1781, 1782, 1084, 1783,  813,
 1084,  842, 1784, 1785,  814, 1786, 1789, 1656, 1801,  842,
 1795, 1796,  842,  842, 1084,  869, 1817, 1827, 1829, 1084,
 1084, 1830,  842, 1084, 1831,  871, 1832, 1833,  871, 1834,
 1084, 1836, 1084, 1084, 1837, 1846, 1084, 1838,  842, 1848,
 1084, 1853, 1084,  871, 1084, 1852, 1084, 1854, 1857, 1858,
 1859, 1084, 1084, 1860, 1084, 1863,  869, 1864, 1865,  869,
  842, 1866,  813, 1867, 1868,  842, 1869,  814, 1870, 1084,
 1084,  557, 1084, 1871,  869, 1704, 1872,  842, 1874,  842,
 1877, 1710, 1885, 1084, 1773, 1084, 1714, 1084, 1084, 1084,
 1901, 1084, 1902, 1719, 1084, 1084, 1913, 1916, 1918, 1919,
 1931, 1940, 1943, 1950, 1084,  854, 1935, 1951, 1954, 1957,
 1961,  813,  557, 1969, 1968,  557,  814, 1084,  813, 1970,
 1084, 1973, 1988,  814, 1990, 1993,  842, 1325, 1996, 2001,
  557, 1325, 1986,  813, 1985, 2011, 1084, 1752,  814, 2012,
 2014, 2015, 1084, 1084,    1, 2018,  842, 1084,  785, 1057,
  842,   29,   31, 1760,  200,  813,  824,  377, 1084, 1084,
  814, 1084,  268,  813,  701,   82,  163, 1084,  814,  764,
 1084,  813, 1084, 1084, 1084, 1084,  814, 1084, 1042,  813,
  483,  951,  279,  210,  814, 1084, 1084,  817,  815,  763,
  816,  792,  818,  405,  745,  278, 1084, 1084,  747, 1084,
  817,  815, 1328,  816,  190,  818, 1084, 1084, 1084, 1084,
 1084,  765, 1084, 1041,  191, 1084, 1084, 1084, 1084, 1084,
 1015, 1063,  321, 1084, 1084, 1804,  378,  220, 1084, 1084,
 1084, 1084, 1084, 1084, 1084, 1084,  319, 1084, 1535,  817,
  815,  329,  816,  295,  818, 1014,  871, 1084, 1536,  817,
  815,  320,  816,  322,  818, 1739,  817,  815,  330,  816,
  774,  818, 1063, 1063, 1063, 1063, 1063, 1077, 1063, 1740,
  817,  815,  325,  816,  358,  818,  343,  869,   29,  327,
 1063, 1063, 1063, 1063,  817,  815, 1329,  816,   32,  818,
   33,   42,  847,  328, 1614,  329, 1118,  572, 1856,  871,
  571, 1110,  871,  849, 1861, 1862, 1477,  986, 1245,  871,
 1240,  871, 1063, 1475, 1063,  871, 1204,  299,  284, 1473,
  871,  817,  815,  871,  816, 1465,  818, 1214, 1876, 1839,
  869, 1840, 1325,  869,  817,  815, 1879,  816,  871,  818,
  869,  949,  869, 1886, 1887, 1404,  869,  557,  557,  647,
  691,  869,  871, 1454,  869,  557,  248,  617, 1466, 1917,
  871, 1150, 1819,  871,  871,  662,  663, 1181,  557,  869,
 1747,  817,  815,  871,  816, 1984,  818, 1825, 1991,  927,
 1750,  817,  815,  869,  816,  555,  818,  817,  815,  871,
  816,  869,  818, 1989,  869,  869, 2010,  248, 1174, 1353,
  248, 1762, 1898, 1976,  869, 1099, 1643, 1810,  670, 1305,
  653,  871,  644, 1686,  972,  248,  871,  931,  163, 1211,
  869,  609,  813, 1219,  864,  868,  555,  814,  871,  555,
  871, 1140,  344, 1136, 1929,  813, 1379, 1634,  294, 1958,
  814, 1028,  869,  557,  555, 1873,  555,  869, 1751,  817,
  815, 1730,  816, 1157,  818,  817,  815, 1749,  816,  869,
  818,  869,  877, 1374,  557, 1880,  817,  815, 1116,  816,
  557,  818, 1504, 1652,  813, 1542,    0,  871,    0,  814,
    0,    0,    0, 1063,  813, 1063, 1063,    0,    0,  814,
    0,  813, 1063,    0,    0,    0,  814,  871, 1063,    0,
 1804,  871,    0, 1063,  557,  813, 1063,    0,  869,    0,
  814, 1937,  817,  815,    0,  816,    0,  818,    0,  813,
 1063, 1094, 1063,    0,  814, 1063, 1063,    0,  869, 1063,
    0,    0,  869,    0,    0,    0, 1063, 1977, 1063, 1063,
    0, 1488, 1063,    0,    0,    0, 1063,    0, 1063,    0,
 1063,    0, 1063,    0, 1717,    0,  813, 1063, 1063,    0,
 1063,  814, 1094, 1094, 1094, 1094, 1094, 1094, 1094,  813,
    0,    0,    0,    0,  814, 1063, 1063,    0, 1063,    0,
 1094, 1094, 1094, 1094,    0,    0,    0,    0,    0, 1063,
    0, 1063,    0, 1063, 1063, 1063,    0, 1063,    0,    0,
 1063, 1063,    0,    0,    0,    0,  813,    0,    0,    0,
 1063,  814, 1094,    0, 1094,    0,  813,    0,    0,    0,
    0,  814,  813, 1063,    0,    0, 1063,  814,    0,    0,
    0,    0,  248,  248,    0,    0,    0,    0,    0,    0,
  248,    0, 1063,  555,    0,    0,    0,    0, 1063, 1063,
    0,    0,    0, 1063,    0,    0,    0,    0,    0,    0,
  555,  555,  555,    0, 1063, 1063,  555, 1063,    0,  555,
    0,    0,    0, 1063,    0,   57, 1063,    0, 1063, 1063,
 1063, 1063,  555, 1063,  813,    0,   58,    0,  555,  814,
  813, 1063, 1063,   59,    0,  814,    0,   60,    0,    0,
    0,  813, 1063, 1063,    0, 1063,  814,    0,    0,    0,
    0,    0, 1063, 1063, 1063, 1063, 1063,   61, 1063,    0,
    0, 1063, 1063, 1063, 1063, 1063,    0,    0,  248, 1063,
 1063,    0,    0,    0, 1063, 1063, 1063, 1063, 1063, 1063,
 1063, 1063,    0, 1063,    0,    0,    0,  813,    0,  248,
    0,    0,  814, 1063,    0,  248,    0,  555,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  555,   62,
    0,    0,    0, 1094,  555, 1094, 1094,    0,    0,  248,
    0,    0, 1094,  555,    0,  555,    0,    0, 1094,    0,
    0,    0,    0, 1094,    0,    0, 1094,    0,    0,    0,
   63,    0,  552,  555,   64,    0,    0,    0,  555,    0,
 1094, 1099,    0,    0,    0, 1094, 1094,    0,    0, 1094,
    0,    0,    0,    0,    0,    0, 1094,    0, 1094, 1094,
    0,    0, 1094,    0,    0,    0, 1094,  555, 1094,    0,
 1094,    0, 1094,  552,    0,    0,  552, 1094, 1094,    0,
 1094,    0, 1099, 1099, 1099, 1099, 1099, 1099, 1099,    0,
    0,  552,    0,  552,    0, 1094, 1094,    0, 1094,    0,
 1099, 1099, 1099, 1099,    0,    0,    0,    0,    0, 1094,
    0, 1094,    0, 1094, 1094, 1094,    0, 1094,    0,    0,
 1094, 1094,    0,   65,    0,    0,    0,   69,    0,    0,
 1094,    0, 1099,    0, 1099,    0,    0,    0,   70,    0,
    0,    0,    0, 1094,   92,   71, 1094,    0,    0,    0,
    0,   93,    0,    0,    0,   94,   95,    0,   72,    0,
    0,    0, 1094,    0,   96,    0,    0,    0, 1094, 1094,
    0,    0,    0, 1094,    0,   97,    0,    0,    0,    0,
    0,    0,    0,    0, 1094, 1094,    0, 1094,    0,    0,
    0,    0,    0, 1094,    0,    0, 1094,    0, 1094, 1094,
 1094, 1094,    0, 1094,    0,    0,    0,    0,    0,    0,
    0, 1094, 1094,    0,    0,   73,    0,    0,    0,    0,
    0,    0, 1094, 1094,    0, 1094,    0,    0,    0,    0,
    0,   74, 1094, 1094, 1094, 1094, 1094,   98, 1094,    0,
    0, 1094, 1094, 1094, 1094, 1094,    0,   75,    0, 1094,
 1094,    0,    0,   99, 1094, 1094, 1094, 1094, 1094, 1094,
 1094, 1094,   76, 1094,    0,    0,   77,    0,  100,   78,
    0,    0,  101, 1094,    0,    0,    0,   79,    0,    0,
    0,    0,    0,  102,    0,    0,    0,    0,    0,    0,
  552,    0,    0,    0,    0,    0,   80,    0,    0,    0,
    0,    0,  103, 1099,    0, 1099, 1099,  552,  552,  552,
    0,    0, 1099,  552,    0,    0,  552,    0, 1099,    0,
    0,    0,    0, 1099,    0,    0, 1099,    0,    0,  552,
    0,    0,  557,    0,    0,  552,    0,    0,    0,    0,
 1099, 1095,    0,    0,    0, 1099, 1099,    0,    0, 1099,
    0,    0,    0,    0,    0,   81, 1099,    0, 1099, 1099,
    0,  104, 1099,    0,    0,    0, 1099,    0, 1099,    0,
 1099,    0, 1099,  557,    0,    0,  557, 1099, 1099,    0,
 1099,    0, 1095, 1095, 1095, 1095, 1095, 1095, 1095,    0,
    0,  557,    0,  557,    0, 1099, 1099,    0, 1099,    0,
 1095, 1095, 1095, 1095,  552,    0,    0,    0,    0, 1099,
    0, 1099,    0, 1099, 1099, 1099,    0, 1099,    0,    0,
 1099, 1099,    0,    0,    0,  552,    0,    0, 1705, 1706,
 1099,  552, 1095,    0, 1095,    0, 1712, 1713,    0, 1715,
 1716,    0,  552, 1099,    0,    0, 1099, 1720, 1721, 1722,
 1723,    0, 1724, 1725, 1726, 1727, 1728, 1729,    0,    0,
  552,    0, 1099,    0,    0,  552,    0,    0, 1099, 1099,
    0,    0,    0, 1099,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1099, 1099,    0, 1099,    0,    0,
    0,    0,    0, 1099,  552,    0, 1099,    0, 1099, 1099,
 1099, 1099,    0, 1099,    0,    0,    0,    0,    0,    0,
    0, 1099, 1099,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1099, 1099,    0, 1099,    0,    0,    0,    0,
    0,    0, 1099, 1099, 1099, 1099, 1099,    0, 1099,    0,
    0, 1099, 1099, 1099, 1099, 1099,    0,    0,    0, 1099,
 1099,    0,    0,    0, 1099, 1099, 1099, 1099, 1099, 1099,
 1099, 1099,    0, 1099,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1099,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  557,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1095,    0, 1095, 1095,    0,  557,  557,
    0,    0, 1095,  557,    0,    0,  557,    0, 1095,    0,
    0,    0,    0, 1095,    0,    0, 1095,    0,    0,  557,
    0,    0,    0,    0,    0,  557,    0,    0,    0,    0,
 1095, 1108,    0,    0,    0, 1095, 1095,    0,    0, 1095,
    0,    0,    0,    0,    0,    0, 1095,    0, 1095, 1095,
    0,    0, 1095,    0,    0,    0, 1095,    0, 1095,    0,
 1095,    0, 1095,    0,    0,    0,    0, 1095, 1095,    0,
 1095,    0, 1108, 1108, 1108, 1108, 1108, 1108, 1108,    0,
    0,    0,    0,    0,    0, 1095, 1095,    0, 1095,    0,
 1108, 1108, 1108, 1108,  557,    0,    0,    0,    0, 1095,
    0, 1095,    0, 1095, 1095, 1095,    0, 1095,    0,    0,
 1095, 1095,    0,    0,    0,  557,    0,    0,    0,    0,
 1095,  557, 1108,    0, 1108,    0,    0,    0,    0,    0,
    0,    0,  557, 1095,    0,    0, 1095,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  557,    0, 1095,    0,    0,  557,    0,    0, 1095, 1095,
    0,    0,    0, 1095,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1095, 1095,    0, 1095,    0,    0,
    0,    0,    0, 1095,  557,    0, 1095,    0, 1095, 1095,
 1095, 1095,    0, 1095,    0,    0,    0,    0,    0,    0,
    0, 1095, 1095,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1095, 1095,    0, 1095,    0,    0,    0,    0,
    0,    0, 1095, 1095, 1095, 1095, 1095,    0, 1095,    0,
    0, 1095, 1095, 1095, 1095, 1095,    0,    0,    0, 1095,
 1095,    0,    0,    0, 1095, 1095, 1095, 1095, 1095, 1095,
 1095, 1095,    0, 1095,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1095,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1108,    0, 1108, 1108,    0,    0,    0,
    0,    0, 1108,    0,    0,    0,    0,    0, 1108,    0,
    0,    0,    0, 1108,    0,    0, 1108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1108, 1113,    0,    0,    0, 1108, 1108,    0,    0, 1108,
    0,    0,    0,    0,    0,    0, 1108,    0, 1108, 1108,
    0,    0, 1108,    0,    0,    0, 1108,    0, 1108,    0,
 1108,    0, 1108,    0,    0,    0,    0, 1108, 1108,    0,
 1108,    0, 1113, 1113, 1113, 1113, 1113, 1113, 1113,    0,
    0,    0,    0,    0,    0, 1108, 1108,    0, 1108,    0,
 1113, 1113, 1113, 1113,    0,    0,    0,    0,    0, 1108,
    0, 1108,    0, 1108, 1108, 1108,    0, 1108,    0,    0,
 1108, 1108,    0,    0,    0,    0,    0,    0,    0,    0,
 1108,    0, 1113,    0, 1113,    0,    0,    0,    0,    0,
    0,    0,    0, 1108,    0,    0, 1108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1108,    0,    0,    0,    0,    0, 1108, 1108,
    0,    0,    0, 1108,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1108, 1108,    0, 1108,    0,    0,
    0,    0,    0, 1108,    0,    0, 1108,    0, 1108, 1108,
 1108, 1108,    0, 1108,    0,    0,    0,    0,    0,    0,
    0, 1108, 1108,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1108, 1108,    0, 1108,    0,    0,    0,    0,
    0,    0, 1108, 1108, 1108, 1108, 1108,    0, 1108,    0,
    0, 1108, 1108, 1108, 1108, 1108,    0,    0,    0, 1108,
 1108,    0,    0,    0, 1108, 1108, 1108, 1108, 1108, 1108,
 1108, 1108,    0, 1108,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1108,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1113,    0, 1113, 1113,    0,    0,    0,
    0,    0, 1113,    0,    0,    0,    0,    0, 1113,    0,
    0,    0,    0, 1113,    0,    0, 1113,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1113,  919,    0,    0,    0, 1113, 1113,    0,    0, 1113,
    0,    0,    0,    0,    0,    0, 1113,    0, 1113, 1113,
    0,    0, 1113,    0,    0,    0, 1113,    0, 1113,    0,
 1113,    0, 1113,    0,    0,    0,    0, 1113, 1113,    0,
 1113,    0,  919,  919,  919,  919,  919,    0,  919,    0,
    0,    0,    0,    0,    0, 1113, 1113,    0, 1113,    0,
  919,  919,  919,  919,    0,    0,    0,    0,    0, 1113,
    0, 1113,    0, 1113, 1113, 1113,    0, 1113,    0,    0,
 1113, 1113,    0,    0,    0,    0,    0,    0,    0,    0,
 1113,    0,    0,    0,  919,    0,    0,    0,    0,    0,
    0,    0,    0, 1113,    0,    0, 1113,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1113,    0,    0,    0,    0,    0, 1113, 1113,
    0,    0,    0, 1113,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1113, 1113,    0, 1113,    0,    0,
    0,    0,    0, 1113,    0,    0, 1113,    0, 1113, 1113,
 1113, 1113,    0, 1113,    0,    0,    0,    0,    0,    0,
    0, 1113, 1113,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1113, 1113,    0, 1113,    0,    0,    0,    0,
    0,    0, 1113, 1113, 1113, 1113, 1113,    0, 1113,    0,
    0, 1113, 1113, 1113, 1113, 1113,    0,    0,    0, 1113,
 1113,    0,    0,    0, 1113, 1113, 1113, 1113, 1113, 1113,
 1113, 1113,    0, 1113,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1113,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  919,    0,  919,  919,    0,    0,    0,
    0,    0,  919,    0,    0,    0,    0,    0,  919,    0,
    0,    0,    0,  919,    0,    0,  919,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  919,  932,    0,    0,    0,  919,  919,    0,    0,  919,
    0,    0,    0,    0,    0,    0,  919,    0,  919,  919,
    0,    0,  919,    0,    0,    0,  919,    0,  919,    0,
  919,    0,  919,    0,    0,    0,    0,  919,  919,    0,
  919,    0,  932,  932,  932,  932,  932,    0,  932,    0,
    0,    0,    0,    0,    0,  919,  919,    0,  919,    0,
  932,  932,  932,  932,    0,    0,    0,    0,    0,  919,
    0,  919,    0,  919,  919,  919,    0,  919,    0,    0,
  919,  919,    0,    0,    0,    0,    0,    0,    0,    0,
  919,    0,    0,    0,  932,    0,    0,    0,    0,    0,
    0,    0,    0,  919,    0,    0,  919,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  919,    0,    0,    0,    0,    0,  919,  919,
    0,    0,    0,  919,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  919,  919,    0,  919, 1407,    0,
    0,    0,    0,  919,    0,    0,  919,    0,  919,  919,
  919,  919,    0,  919,    0,    0,    0,    0,    0,    0,
    0,  919,  919,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  919,  919,    0,  919,    0,    0,    0,    0,
    0,    0,  919,  919,  919,  919,  919,    0,  919,    0,
    0,  919,  919,  919,  919,  919,    0,    0,    0,  919,
  919,    0,    0,    0,  919,  919,  919,  919,  919,  919,
  919,  919,    0,  919,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  919,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  932,    0,  932,  932,    0,    0,    0,
    0,    0,  932,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  932,    0,    0,  932,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  932,  935,    0,    0,    0,  932,  932,    0,    0,  932,
    0,    0,    0,    0,    0,    0,  932,    0,  932,  932,
    0,    0,  932,    0,    0,    0,  932,    0,  932,    0,
  932,    0,  932,    0,    0,    0,    0,  932,  932,    0,
  932,    0,  935,  935,  935,  935,  935,    0,  935,    0,
    0,    0,    0,    0,    0,  932,  932,    0,  932,    0,
  935,  935,  935,  935,    0,    0,    0,    0,    0,  932,
    0,  932,    0,  932,  932,  932,    0,  932,    0,    0,
  932,  932,    0,    0,    0,    0,    0,    0,    0,    0,
  932,    0,    0,    0,  935,  471,    0,    0,    0,    0,
    0,    0,    0,  932,    0,    0,  932,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  932,  473,    0,    0,    0,    0,  932,  932,
    0,    0,    0,  932,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  932,  932,    0,  932,    0,    0,
    0,  475,    0,  932,    0,    0,  932,    0,  932,  932,
  932,  932,    0,  932,    0,    0,    0,    0,    0,    0,
    0,  932,  932,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  932,  932,    0,  932,    0,    0,    0,    0,
    0,    0,  932,  932,  932,  932,  932,    0,  932,    0,
    0,  932,  932,  932,  932,  932,    0,    0,    0,  932,
  932,    0,    0,    0,  932,  932,  932,  932,  932,  932,
  932,  932,  480,  932,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  932,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,  484,  485,    0,  255,  256,    0,    0,
    0,    0,    0,  935,    0,  935,  935,    0,    0,    0,
    0,    0,  935,    0,    0,  488,  489,  490,  491,  492,
  493,  494,  495,  496,    0,    0,  935,    0,    0,    0,
  498,  499,    0,    0,    0,    0,    0,    0,    0,  503,
  935,  936,    0,    0,    0,  935,  935,    0,    0,  935,
    0,    0,    0,    0,    0,    0,  935,    0,  935,  935,
    0,    0,  935,    0,    0,    0,  935,    0,  935,    0,
  935,    0,  935,    0,    0,    0,    0,  935,  935,    0,
  935,    0,  936,  936,  936,  936,  936,    0,  936,    0,
    0,    0,    0,    0,    0,  935,  935,    0,  935,    0,
  936,  936,  936,  936,    0,    0,    0,    0,    0,  935,
    0,  935,    0,  935,  935,  935,    0,  935,    0,    0,
  935,  935,    0,    0,    0,    0,    0,    0,    0,    0,
  935,    0,    0,    0,  936,    0,    0,    0,    0,    0,
    0,    0,    0,  935,    0,    0,  935,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  935,    0,    0,    0,    0,    0,  935,  935,
    0,    0,    0,  935,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  935,  935,    0,  935,    0,    0,
    0,    0,    0,  935,    0,    0,  935,    0,  935,  935,
  935,  935,    0,  935,    0,    0,    0,    0,    0,    0,
    0,  935,  935,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  935,  935,    0,  935,    0,    0,    0,    0,
    0,    0,  935,  935,  935,  935,  935,    0,  935,    0,
    0,  935,  935,  935,  935,  935,    0,    0,    0,  935,
  935,    0,    0,    0,  935,  935,  935,  935,  935,  935,
  935,  935,    0,  935,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  935,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,    0,  936,  936,    0,    0,    0,
    0,    0,  936,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  936,    0,    0,    0,
  930,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  936,    0,    0,    0,    0,  936,  936,    0,    0,  936,
    0,    0,    0,    0,    0,    0,  936,    0,  936,  936,
    0,    0,  936,    0,    0,    0,  936,    0,  936,    0,
  936,  930,  936,  930,  930,  930,    0,  936,  936,    0,
  936,  989,    0,    0,  693,    0,    0,    0,    0,  930,
  930,  930,  930,    0,    0,  936,  936,  990,  936,    0,
    0,  694,    0,    0,  695,    0,    0,    0,    0,  936,
    0,  936,    0,  936,  936,  936,    0,  936,    0,    0,
  936,  936,    0,  930,    0,    0,    0,    0,    0,    0,
  936,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,  696,    0,  936,    0,    0,    0,
  697,  698,  699,    0,  700,  701,  702,    0,  703,    0,
    0,    0,  936,    0,    0,    0,    0,    0,  936,  936,
    0,    0,    0,  936,    0,    0,  704,    0,  705,    0,
    0,    0,    0,    0,  936,  936,    0,  936,    0,    0,
    0,    0,    0,  936,    0,    0,  936,    0,  936,  936,
  936,  936,    0,  936,    0,    0,    0,  706,    0,    0,
    0,  936,  936,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  936,  936,  707,  936,    0,    0,    0,    0,
    0,    0,  936,  936,  936,  936,  936,    0,  936,    0,
    0,  936,  936,  936,  936,  936,  708,    0,    0,  936,
  936,    0,    0,    0,  936,  936,  936,  936,  936,  936,
  936,  936,    0,  936,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  936,    0,    0,    0,    0,    0,    0,
    0,  709,  710,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  930,    0,  930,  930,    0,    0,  711,    0,
    0,  930,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  930,    0,    0,    0,  929,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  930,
    0,    0,    0,    0,  930,  930,    0,    0,  930,    0,
    0,    0,    0,    0,    0,  930,    0,  930,  930,    0,
    0,  930,    0,    0,    0,  930,    0,  930,    0,  930,
  929,  930,  929,  929,  929,    0,  930,  930,    0,  930,
  218,    0,    0,  218,    0,    0,    0,    0,  929,  929,
  929,  929,    0,    0,  930,  930,  218,  930,    0,    0,
  218,    0,    0,  218,    0,    0,    0,    0,  930,    0,
  930,    0,  930,  930,  930,    0,  930,    0,    0,  930,
  930,    0,  929,    0,    0,    0,    0,    0,    0,  930,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  930,  218,    0,  930,    0,    0,    0,  218,
  218,  218,    0,  218,  218,  218,    0,  218,    0,    0,
    0,  930,    0,    0,    0,    0,    0,  930,  930,    0,
    0,    0,  930,    0,    0,  218,    0,  218,    0,    0,
    0,    0,    0,  930,  930,    0,  930,    0,    0,    0,
    0,    0,  930,    0,    0,  930,    0,  930,  930,  930,
  930,    0,  930,    0,    0,    0,  218,    0,    0,    0,
  930,  930,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  930,  930,  218,  930,    0,    0,    0,    0,    0,
    0,  930,  930,  930,  930,  930,    0,  930,    0,    0,
  930,  930,  930,  930,  930,  218,    0,    0,  930,  930,
    0,    0,    0,  930,  930,  930,  930,  930,  930,  930,
  930,    0,  930,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  930,    0,    0,    0,    0,    0,    0,    0,
  218,  218,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  929,    0,  929,  929,    0,    0,  218,    0,    0,
  929,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  929,    0,    0,    0,  931,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  929,    0,
    0,    0,    0,  929,  929,    0,    0,  929,    0,    0,
    0,    0,    0,    0,  929,    0,  929,  929,    0,    0,
  929,    0,    0,    0,  929,    0,  929,    0,  929,  931,
  929,  931,  931,  931,    0,  929,  929,    0,  929,    0,
    0,    0,    0,    0,    0,    0,    0,  931,  931,  931,
  931,    0,    0,  929,  929,    0,  929,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  929,    0,  929,
    0,  929,  929,  929,    0,  929,    0,    0,  929,  929,
    0,  931,    0,    0,    0,    0,    0,    0,  929,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  929,    0,    0,  929,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  929,    0,    0,    0,    0,    0,  929,  929,    0,    0,
    0,  929,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  929,  929,    0,  929,    0,    0,    0,    0,
    0,  929,    0,    0,  929,    0,  929,  929,  929,  929,
    0,  929,    0,    0,    0,    0,    0,    0,    0,  929,
  929,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  929,  929,    0,  929,    0,    0,    0,    0,    0,    0,
  929,  929,  929,  929,  929,    0,  929,    0,  870,  929,
  929,  929,  929,  929,    0,    0,    0,  929,  929,    0,
    0,    0,  929,  929,  929,  929,  929,  929,  929,  929,
    0,  929,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  929,    0,    0,    0,    0,    0,    0,    0,  870,
    0,    0,  870,    0,    0,    0,    0,    0,    0,    0,
  931,    0,  931,  931,    0,    0,    0,  870,    0,  931,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  931,    0,    0,    0,  934,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  931,    0,    0,
    0,    0,  931,  931,    0,    0,  931,    0,    0,    0,
    0,    0,    0,  931,    0,  931,  931,    0,    0,  931,
    0,    0,    0,  931,    0,  931,    0,  931,  934,  931,
  934,  934,  934,    0,  931,  931,    0,  931,    0,    0,
    0,    0,    0,    0,    0,    0,  934,  934,  934,  934,
    0,    0,  931,  931,    0,  931,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  931,    0,  931,    0,
  931,  931,  931,    0,  931,    0,    0,  931,  931,    0,
  934,    0,    0,    0,    0,    0,    0,  931,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  931,    0,    0,  931,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  931,
    0,    0,    0,    0,    0,  931,  931,    0,    0, 1089,
  931,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  931,  931,    0,  931,    0,    0,  515,    0,    0,
  931,    0,    0,  931,    0,  931,  931,  931,  931,    0,
  931,    0,    0,    0,    0,    0,    0,    0,  931,  931,
  870,    0,    0,    0,    0,    0,    0,    0,    0,  931,
  931,    0,  931,    0,    0,    0,    0,    0,    0,  931,
  931,  931,  931,  931,    0,  931,  737,    0,  931,  931,
  931,  931,  931,    0,    0,    0,  931,  931,    0,    0,
    0,  931,  931,  931,  931,  931,  931,  931,  931,    0,
  931,    0,    0,  870,    0,    0,  870,    0,    0,    0,
  931,    0,    0,  870,    0,  870,    0,  737,    0,  870,
  737,    0,    0,    0,  870,    0,    0,  870,    0,  934,
    0,  934,  934,    0,    0,  737,    0,    0,  934,    0,
    0,    0,  870,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  934,    0,    0,    0,  870,    0,    0,    0,
    0,    0,    0,    0,  870,    0,  934,  870,  870,    0,
    0,  934,  934,    0,    0,  934,    0,  870,    0,    0,
    0,    0,  934,    0,  934,  934,    0,    0,  934,    0,
    0,    0,  934,  870,  934,    0,  934,    0,  934,    0,
    0,    0,    0,  934,  934,    0,  934,    0,    0,    0,
    0,    0,    0,    0,    0,  870,    0,    0,    0,    0,
  870,  934,  934,    0,  934,    0,    0,    0,    0,    0,
    0,    0,  870,    0,  870,  934,    0,  934,    0,  934,
  934,  934,    0,  934,    0,    0,  934,  934, 1096,    0,
    0,    0,    0,    0,    0,    0,  934,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  934,
    0,    0,  934,    0,    0,    0,    0,    0,    0,    0,
    0,  870,    0,    0,    0,    0,    0,    0,  934,    0,
    0,  708,    0,    0,  934,  934,    0,    0,    0,  934,
    0,  870,    0,    0,    0,  870,    0,    0,    0,    0,
  934,  934,    0,  934,    0,    0,    0,    0,    0,  934,
    0,    0,  934,    0,  934,  934,  934,  934,    0,  934,
    0,  708,  708,    0,    0,  708,    0,  934,  934,    0,
  737,    0,    0,    0,    0,    0,    0,    0,  934,  934,
  708,  934,    0,    0,    0,    0,    0,    0,  934,  934,
  934,  934,  934,    0,  934,    0,    0,  934,  934,  934,
  934,  934,    0,    0,    0,  934,  934,    0,    0,    0,
  934,  934,  934,  934,  934,  934,  934,  934,    0,  934,
    0,  737,    0,    0,  737,    0,    0,    0,    0,  934,
    0,  737,    0,  737,    0,    0,    0,  737,    0,    0,
    0,    0,  737,    0,    0,  737,    0,    0,    0,    0,
    0,  115,    0,    0,  116,    0,  117,  118,  119,  120,
  737,  121,    0,    0,    0,    0,    0,    0,    0,  122,
  123,    0,    0,    0,  737,    0,    0,    0,    0,    0,
  124,  125,  737,  126,    0, 1123,  737,    0,    0,    0,
  127,  128,    0,  129,  130,  737,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,  737,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0, 1095,
  735,  146,    0,    0,    0,    0,    0,    0,  737,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  737,    0,  737,    0,    0,    0,    0,    0,  737,    0,
    0,  737,    0,  737,  737,  737,  737,    0,  737,    0,
    0,  735,    0,    0,  735,    0,  737,  737,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  737,  737,  735,
  737,    0,    0,    0,    0,    0,    0,  737,  737,  737,
  737,  737,    0,  737,    0,    0,  737,  737,  737,  737,
  737,    0,    0,    0,  737,  737,    0,    0,    0,  737,
  737,  737,  737,  737,  737,  737,  737,    0,  737,    0,
    0,    0,    0,    0,    0,    0,  708,    0,  737,  708,
    0,    0,    0,    0,    0,    0,  708,    0,  708,    0,
    0,    0,  708,    0,    0,    0,    0,  708,    0,    0,
  708,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  708,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  115,  708,
    0,  116,    0,  117,  118,  119,  120,  708,  121,    0,
    0,  708,    0,    0,    0,    0,  122,  123,    0,    0,
  708,    0,    0,    0,    0,    0,    0,  124,  125,    0,
  126,    0,    0,    0,    0,    0,  708,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,  708,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,  708,    0,  708,  146,    0,
    0,    0,    0,  708,    0,    0,  708,    0,  708,  708,
  708,  708,    0,  708,    0,    0,    0,    0,    0,    0,
    0,  708,  708,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  708,  708,    0,  708,    0,    0,    0,    0,
    0,    0,  708,  708,  708,  708,  708,    0,  708,    0,
    0,  708,  708,  708,  708,  708,    0,    0,    0,  708,
  708,  911,    0,    0,  708,  708,  708,  708,  708,  708,
  708,  708,    0,  708,    0,  735,    0,    0,  735,    0,
    0,    0,    0,  708,   14,  735,    0,  735,    0,    0,
    0,  735,    0,    0,    0,    0,  735,    0,    0,  735,
    0,    0,  911, 1103, 1103,  911, 1103, 1103, 1103,    0,
    0,    0,    0,    0,  735,    0,    0,    0,    0,    0,
  911, 1103, 1103, 1103,    0,    0,    0,  115,  735,    0,
  116,    0,  117,  118,  119,  120,  735,  121,    0,    0,
  735,    0,    0,    0,    0,  122,  123,    0,    0,  735,
    0,    0, 1103,    0,    0,    0,  124,  125,    0,  126,
    0,    0,    0,    0,    0,  735,  127,  128,    0,  129,
  130,    0,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  140,  141,  735,  142,  143,  144,    0,  145,    0,    0,
    0,    0,    0,    0,  735,    0,  735,  146,    0,  374,
    0,    0,  708,    0,    0,  708,    0,  708,  708,  708,
  708,    0,  708,    0,    0,  375,    0,    0,    0,    0,
  708,  708,    0,    0,    0,    0,  376,    0,    0,    0,
    0,  708,  708,  377,  708,    0,    0,  378,  379,    0,
    0,  708,  708,  735,  708,  708,  380,  708,  912,    0,
  708,  708,  708,  708,  708,    0,    0,  381,  708,  708,
    0,    0,    0,  735,  708,  708,  708,  735,  708,  708,
  708,    0,  708,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  708,  817,  815,    0,  816,    0,  818,  912,
 1104, 1104,  912, 1104, 1104, 1104,    0,    0,    0,    0,
    0, 1021, 1020, 1022,    0,    0,    0,  912, 1104, 1104,
 1104,  382,    0,  911,    0,    0,    0,    0,    0,  383,
  902,    0, 1103,    0,    0,    0,    0,    0, 1103,    0,
    0,    0,    0, 1103,    0,    0, 1103,    0,    0, 1104,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  384,    0,    0,    0,  385,    0,    0,    0,    0,    0,
    0,  902,    0,    0,  902,  386,  911,    0,    0,  911,
    0,    0, 1103,    0,    0,    0,  911,    0,  911,  902,
 1103,    0,  911,    0,  387,  388,    0,  911, 1103,    0,
  911,    0,  864,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  911, 1103,    0, 1103,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  911,
    0, 1103,    0, 1103, 1103, 1103,    0,  911,    0,    0,
  911,  911,    0,  864,    0,    0,  864,    0,    0,    0,
  911,    0,    0,  389,    0,    0,  390,    0,    0,    0,
    0,  864,  865,    0,    0,    0,  911,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1103,    0,    0,    0,    0,    0,  911,    0,
    0,    0,    0,  911,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  865,    0,  911,  865,  911,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  912,  865, 1008,    0,    0,    0,    0,    0,  813, 1104,
    0,    0,    0,  814,    0, 1104, 1009,    0,    0,    0,
 1104,    0,    0, 1104,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  911,    0,    0,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1010,  912,  911,    0,  912,    0,  911, 1104,
 1011,    0,    0,  912,    0,  912,    0, 1104, 1012,  912,
    0,    0,  902,    0,  912, 1104,    0,  912,  866,    0,
    0,  866,    0,    0,    0,    0, 1013,    0, 1014,    0,
    0,    0,  912, 1104,    0, 1104,  866,    0,    0,    0,
    0, 1015,    0, 1016, 1017, 1018,  912,    0, 1104,  901,
 1104, 1104, 1104,    0,  912,    0,    0,  912,  912,    0,
    0,    0,    0,    0,    0,  902,    0,  912,  902,    0,
    0,    0,    0,    0,    0,  902,    0,  902,    0,    0,
    0,  902,    0,  912,  864,    0,  902,    0,    0,  902,
  901,    0, 1019,  901,    0,    0,    0,    0,    0, 1104,
    0,    0,    0,    0,  902,  912,    0,    0,  901,    0,
  912,    0,    0,    0,    0,    0,    0,    0,  902,    0,
    0,  895,  912,    0,  912,    0,  902,    0,    0,  902,
  902,    0,    0,    0,    0,    0,    0,  864,    0,  902,
  864,    0,    0,    0,  865,    0,    0,  864,    0,  864,
    0,    0,    0,  864,    0,  902,    0,    0,  864,    0,
    0,  864,  895,    0,    0,  895,    0,    0,    0,    0,
    0,  912,    0,    0,    0,    0,  864,  902,    0,    0,
  895,  903,  902,    0,    0,    0,    0,    0,    0,    0,
  864,  912,    0,    0,  902,  912,  902,  865,  864,    0,
  865,  864,  864,    0,    0,    0,    0,  865,    0,  865,
    0,  864,    0,  865,    0,    0,    0,    0,  865,    0,
    0,  865,  903,    0,    0,  903,    0,  864,    0,    0,
    0,    0,    0,    0,    0,    0,  865,    0,    0,  866,
  903,    0,    0,  902,    0,    0,    0,    0,    0,  864,
  865,    0,    0,  904,  864,    0,    0,    0,  865,    0,
    0,  865,  865,  902,    0,    0,  864,  902,  864,    0,
    0,  865,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  865,    0,    0,
    0,    0,  866,    0,  904,  866,    0,  904,    0,    0,
    0,    0,  866,    0,  866,    0,    0,    0,  866,  865,
    0,  901,  904,  866,  865,  864,  866,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  865,    0,  865,    0,
    0,  866,    0,    0,    0,  864,    0,  892,    0,  864,
    0,    0,    0,    0,    0,  866,    0,    0,    0,    0,
    0,    0,    0,  866,    0,    0,  866,  866,    0,    0,
    0,    0,    0,    0,  901,    0,  866,  901,    0,    0,
    0,    0,    0,    0,  901,  865,  901,    0,  892,    0,
  901,  892,  866,  895,    0,  901,    0,    0,  901,    0,
    0,    0,    0,    0,    0,  865,  892,    0,    0,  865,
    0,    0,    0,  901,  866,    0,    0,    0,    0,  866,
    0,    0,    0,    0,    0,    0,    0,  901,    0,    0,
  890,  866,    0,  866,    0,  901,    0,    0,  901,  901,
    0,    0,    0,    0,    0,    0,  895,    0,  901,  895,
    0,    0,    0,  903,    0,    0,  895,    0,  895,    0,
    0,    0,  895,    0,  901,    0,    0,  895,    0,    0,
  895,  890,    0,    0,  890,    0,    0,    0,    0,    0,
  866,    0,    0,    0,    0,  895,  901,    0,    0,  890,
    0,  901,    0,    0,    0,    0,    0,    0,    0,  895,
  866,    0,  896,  901,  866,  901,  903,  895,    0,  903,
  895,  895,    0,    0,    0,    0,  903,    0,  903,    0,
  895,    0,  903,    0,    0,  904,    0,  903,    0,    0,
  903,    0,    0,    0,    0,    0,  895,    0,    0,    0,
    0,    0,    0,  896,    0,  903,  896,    0,    0,    0,
    0,    0,  901,    0,    0,    0,    0,    0,  895,  903,
    0,  896,  905,  895,    0,    0,    0,  903,    0,    0,
  903,  903,  901,    0,    0,  895,  901,  895,  904,    0,
  903,  904,    0,    0,    0,    0,    0,    0,  904,    0,
  904,    0,    0,    0,  904,    0,  903,    0,    0,  904,
    0,    0,  904,  905,    0,    0,  905,    0,    0,  892,
    0,    0,    0,    0,    0,    0,    0,  904,  903,    0,
    0,  905,    0,  903,  895,    0,    0,    0,    0,    0,
    0,  904,    0,    0,    0,  903,    0,  903,    0,  904,
    0,    0,  904,  904,  895,    0,    0,    0,  895,    0,
    0,    0,  904,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  892,    0,    0,  892,    0,    0,  904,    0,
    0,    0,  892,  891,  892,    0,    0,    0,  892,    0,
    0,    0,    0,  892,  903,    0,  892,    0,    0,    0,
  904,    0,  890,    0,    0,  904,    0,    0,    0,    0,
    0,  892,    0,    0,  903,    0,  893,  904,  903,  904,
    0,    0,    0,    0,  891,  892,    0,  891,    0,    0,
    0,    0,    0,  892,    0,    0,  892,  892,    0,    0,
    0,    0,  891,    0,    0,    0,  892,    0,    0,    0,
    0,    0,    0,    0,    0,  890,    0,  893,  890,    0,
  893,    0,  892,    0,    0,  890,  904,  890,    0,    0,
    0,  890,    0,    0,  896,  893,  890,    0,    0,  890,
    0,    0,    0,    0,  892,    0,  904,    0,    0,  892,
  904,    0,    0,    0,  890,    0,    0,    0,    0,    0,
    0,  892,    0,  892,    0,  897,    0,    0,  890,    0,
    0,    0,    0,    0,    0,    0,  890,    0,    0,  890,
  890,    0,    0,    0,    0,    0,    0,  896,    0,  890,
  896,    0,    0,    0,  905,    0,    0,  896,    0,  896,
    0,    0,    0,  896,    0,  890,  897,    0,  896,  897,
  892,  896,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  897,    0,  896,  890,    0,    0,
  892,    0,  890,    0,  892,    0,    0,  721,    0,    0,
  896,    0,    0,    0,  890,    0,  890,  905,  896,    0,
  905,  896,  896,    0,    0,    0,    0,  905,    0,  905,
    0,  896,    0,  905,    0,    0,    0,    0,  905,    0,
    0,  905,    0,    0,    0,    0,    0,  896,  721,    0,
    0,  721,    0,  719,    0,    0,  905,    0,    0,    0,
    0,    0,    0,  890,    0,    0,  721,    0,    0,  896,
  905,    0,    0,    0,  896,  891,    0,    0,  905,    0,
    0,  905,  905,  890,    0,    0,  896,  890,  896,    0,
    0,  905,    0,  719,  719,    0,    0,  719,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  905,  893,    0,
    0,    0,  719,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  891,  905,
    0,  891,    0,    0,  905,  896,    0,    0,  891,  732,
  891,    0,    0,    0,  891,    0,  905,    0,  905,  891,
    0,    0,  891,    0,    0,  896,    0,    0,    0,  896,
    0,  893,    0,    0,  893,    0,    0,  891,    0,    0,
    0,  893,    0,  893,    0,    0,    0,  893,    0,    0,
  732,  891,  893,  732,    0,  893,    0,    0,    0,  891,
    0,    0,  891,  891,    0,  905,    0,  897,  732,    0,
  893,    0,  891,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  893,  905,    0,    0,  891,  905,
    0,    0,  893,    0,    0,  893,  893,    0,    0,    0,
    0,  313,    0,    0,    0,  893,    0,    0,    0,    0,
  891,    0,    0,    0,    0,  891,    0,    0,    0,    0,
  897,  893,    0,  897,    0,    0,    0,  891,    0,  891,
  897,    0,  897,    0,    0,    0,  897,    0,    0,    0,
    0,  897,    0,  893,  897,    0,    0,    0,  893,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  897,
  893,    0,  893,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  897,    0,    0,  891,    0,    0,    0,
    0,  897,    0,    0,  897,  897,    0,    0,    0,    0,
    0,    0,  721,    0,  897,  721,  891,    0,    0,    0,
  891,    0,  721,    0,  721,    0,    0,    0,  721,  893,
  897,    0,    0,  721,    0,    0,  721,  758,    0,    0,
  512,    0,  513,    0,    0,    0,    0,    0,    0,  893,
    0,  721,  897,  893,    0,  515,    0,  897,  719,    0,
  333,  719,    0,    0,    0,  721,    0,    0,  719,  897,
  719,  897,    0,  721,  719,    0,    0,  721,    0,  719,
    0,    0,  719,    0,    0,    0,  721,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  719,    0,    0,
    0,    0,  721,    0,    0,    0,    0,    0,    0,    0,
    0,  719,    0,    0,    0,    0,    0,    0,  897,  719,
    0,    0,    0,  719,    0,    0,    0,    0,    0,  721,
    0,    0,  719,    0,    0,    0,    0,    0,  897,    0,
    0,  721,  897,  721,  732,    0,    0,  732,  719,    0,
    0,    0,    0,    0,  732,    0,  732,    0,    0,    0,
  732,    0,  911,    0,    0,  732,    0, 1123,  732,    0,
    0,    0,    0,    0,    0,  719,    0,    0,    0,    0,
    0,    0,    1,  732,    0,    0,    0,  719,    0,  719,
  721,    0,    0,    0,    0,    0,    0,  732,    0,    0,
    2,    0,    0,    0,    0,  732,    0,    0,    3,  732,
  721,    0,    0,    0,  721,    4,    0,    5,  732,    6,
    0,    0,    0,    0,    7,    0,    0,    0,    0,    0,
    0,    8,    0,    0,  732,    0,  719,    0,    0,    0,
    0,    0,    0,    0,    0,    9,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   10,  719,    0,    0,    0,
  719,  732,    0,    0,  468,    0,    0,    0,    0,    0,
    0,  469,    0,  732,    0,  732,    0,    0,    0,    0,
    0,    0,    0,  470,    0,    0,    0,    0,    0,  471,
  472,    0,    0,  758,    0,    0,  512,    0,  513,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  752,    0,
    0,  515,    0,    0,    0,    0,  333,  473,   11,    0,
  474,    0,  732,    0,    0,   12,    0,    0,   13,    0,
   14,   15,    0,  115,    0,    0,  116,    0,  117,  118,
  119,  120,  732,  121,    0,  475,  732,  476,  477,    0,
    0,  122,  123,    0,    0,   16,    0,  478,    0,    0,
    0,  479,  124,  125,    0,  126,    0,    0,    0, 1000,
    0,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,   17,    0,   14,    0,    0,    0,
  754,    0,    0,  146,    0,    0,  480,    0,    0,   18,
   19,    0,  481,    0,    0, 1096,    0,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   20,    0,  483,  484,  485,  486,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  487,    0,    0,    0,    0,    0,    0,  122,  123,  488,
  489,  490,  491,  492,  493,  494,  495,  496,  124,  497,
    0,  126,    0,    0,  498,  499,    0,  500,  501,  502,
    0,  129,  130,  503,  131,    0,    0,  132,  133,  134,
  135,  136, 1001, 1002, 1003,  137,  138,    0,    0,    0,
    0,  139,  504,  141,    0,  505,  143,  144,    0,  145,
  468,  506,  507,  508,    0,  509,  510,  469,  511,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  470,
    0,    0,  751,    0,    0,  471,  472,    0,    0,  758,
    0,    0,  512,    0,  513,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  752,    0,    0,  515,    0,    0,
    0,    0,  333,  473,    0,    0,  474,    0,    0,  115,
    0,    0,  116,    0,  117,  118,  119,  120,    0,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123,    0,
    0,  475,    0,  476,  477,    0,    0,    0,  124,  125,
    0,  126,    0,  478,    0,    0,    0,  479,  127,  128,
    0,  129,  130,    0,  131,  753,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,    0,  142,  143,  144,    0,  145,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  146,
    0,    0,    0,    0,    0,    0,  754,    0,    0,    0,
    0,    0,  480,    0,    0,    0,    0,    0,  481,    0,
    0,    0,    0,  705,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,  484,  485,  486,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  487,    0,    0,    0,
    0,    0,    0,  122,  123,  488,  489,  490,  491,  492,
  493,  494,  495,  496,  124,  497,    0,  126,    0,    0,
  498,  499,    0,  500,  501,  502,    0,  129,  130,  503,
  131,    0,    0,  132,  133,  755,  756,  757,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  504,  141,
    0,  505,  143,  144,    0,  145,  468,  506,  507,  508,
    0,  509,  510,  469,  511,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  470,    0,    0,    0,    0,
    0,  471,  472,    0,    0,  758,    0,    0,  512,    0,
  513,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  752,    0,    0,  515,    0,    0,    0,    0,  333,  473,
    0,    0,  474,    0,    0,  115,    0,    0,  116,    0,
  117,  118,  119,  120,    0,  121,    0,    0,    0,    0,
    0,    0,    0,  122,  123,    0,    0,  475,    0,  476,
  477,    0,    0,    0,  124,  125,    0,  126,    0,  478,
    0,    0,    0,  479,  127,  128,    0,  129,  130,    0,
  131,  996,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  146,    0,    0,    0,    0,
    0,    0,  754,    0,    0,    0,    0,    0,  480,    0,
    0,    0,    0,    0,  481,    0,    0,    0,    0,    0,
    0,   85,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  705,    0,  483,  484,
  485,  486,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  487,    0,    0,    0,    0,    0,    0,  122,
  123,  488,  489,  490,  491,  492,  493,  494,  495,  496,
  124,  497,    0,  126,    0,    0,  498,  499,    0,  500,
  501,  502,    0,  129,  130,  503,  131,    0,    0,  132,
  133,  755,  756,  757,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  504,  141,    0,  505,  143,  144,
    0,  145,  468,  506,  507,  508,    0,  509,  510,  469,
  511,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  470,    0,    0,    0,    0,    0,  471,  472,    0,
    0,  585,    0,    0,  512,    0,  513,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  752,    0,    0,  515,
    0,    0,    0,    0,  333,  473,    0,    0,  474,    0,
    0,  708,    0,    0,  708,    0,  708,  708,  708,  708,
    0,  708,    0,    0,    0,    0,    0,    0,    0,  708,
  708,    0,    0,  475,    0,  476,  477,    0,    0,    0,
  708,  708,    0,  708,    0,  478,    0,    0,    0,  479,
  708,  708,    0,  708,  708,    0,  708,  753,    0,  708,
  708,  708,  708,  708,    0,    0,    0,  708,  708,    0,
    0,    0,    0,  708,  708,  708,    0,  708,  708,  708,
    0,  708,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  708,    0,    0,    0,    0,    0,    0,  754,    0,
    0,    0,    0,    0,  480,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,  484,  485,  486,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  487,  515,
    0,    0,    0,    0,    0,  122,  123,  488,  489,  490,
  491,  492,  493,  494,  495,  496,  124,  497,    0,  126,
    0,    0,  498,  499,    0,  500,  501,  502,    0,  129,
  130,  503,  131,    0,    0,  132,  133,  755,  756,  757,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  504,  141,    0,  505,  143,  144,    0,  145,  468,  506,
  507,  508,    0,  509,  510,  469,  511,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  470,    0,    0,
    0,    0,    0,  471,  472,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  758,    0,    0,
  512,    0,  513,    0,    0,    0,    0,    0,    0,    0,
    0,  473,    0,    0,  474,  515,   84,    0,  115,    0,
  333,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,  475,
    0,  476,  477,    0,    0,    0,    0,  124,  125,    0,
  126,  478,    0,    0,    0,  479,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  480,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,  484,  485,  486,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  487,    0,    0,    0,    0,    0,
    0,  122,  123,  488,  489,  490,  491,  492,  493,  494,
  495,  496,  124,  497,    0,  126,    0,    0,  498,  499,
    0,  500,  501,  502,    0,  129,  130,  503,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  504,  141,    0,  505,
  143,  144, 1070,  145,    0,  506,  507,  508, 1071,  509,
  510, 1072,  511,  146,  468,    0, 1075,  817,  815,    0,
  816,  469,  818,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  470,    0, 1021, 1020, 1022,    0,  471,
  472,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  585,    0,    0,  512,  752,  513,
    0,    0,    0,    0,    0,    0,    0,  473,    0,    0,
  474,    0,  515,    0,    0,    0,    0,  333,    0,    0,
    0,    0,    0,  115,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,  475,    0,  476,  477,    0,
    0,  122,  123,    0,    0,    0,    0,  478,    0,    0,
    0,  479,  124,  125,    0,  126,    0,    0,    0,  996,
    0,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
  754,    0,    0,  146,    0,    0,  480,    0,    0,    0,
    0,    0,  481,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  483,  484,  485,  486,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  487,    0,    0,    0,    0,    0,    0,  122,  123,  488,
  489,  490,  491,  492,  493,  494,  495,  496,  124,  497,
    0,  126,    0,    0,  498,  499,    0,  500,  501,  502,
    0,  129,  130,  503,  131,    0, 1008,  132,  133,  134,
  135,  136,  813,    0,    0,  137,  138,  814,    0,    0,
 1009,  139,  504,  141,    0,  505,  143,  144,    0,  145,
    0,  506,  507,  508, 1258,  509,  510, 1259,  511,  146,
    0,  468,    0,    0,    0,    0,    0,    0,  469,    0,
    0,    0,    0,    0,    0,    0, 1010,    0,    0,    0,
  470,    0,    0,    0, 1011,    0,  471,  472,    0,    0,
    0,    0, 1012,    0,    0,    0,    0,    0,    0,    0,
    0,  585,    0,    0,  512,    0,  513,    0,    0,    0,
 1013,    0, 1014,  566,  473,    0,    0,  474,    0,  515,
    0,    0,    0,    0,  333, 1015,    0, 1016, 1017, 1018,
  115,    0,    0,  116,    0,  117,  118,  119,  120,    0,
  121,    0,  475,    0,  476,  477,    0,    0,  122,  123,
    0,    0,    0,    0,  478,    0,    0,    0,  479,  124,
  125,    0,  126,    0,    0,    0,    0,    0,    0,  127,
  128,    0,  129,  130,    0,  131, 1019,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  146, 1260,    0,  480,    0,    0,    0,    0,    0,  481,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  483,  484,  485,  486,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  487,    0,    0,
    0,    0,    0,    0,  122,  123,  488,  489,  490,  491,
  492,  493,  494,  495,  496,  124,  497,    0,  126,    0,
    0,  498,  499,    0,  500,  501,  502,    0,  129,  130,
  503,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  504,
  141,    0,  505,  143,  144,    0,  145,    0,  506,  507,
  508, 1269,  509,  510, 1259,  511,  146,    0,  468,  218,
    0,    0,  218,    0,    0,  469,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  218,    0,  470,    0,  218,
    0,    0,  218,  471,  472,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  585,    0,
    0,  512,    0,  513,    0,    0,    0,    0,    0,    0,
    0,  473,    0,    0,  474,    0,  515,    0,    0,    0,
    0,  333,  218,    0,    0,    0,    0,    0,  218,  218,
  218,    0,  218,  218,  218,    0,  218,    0,    0,  475,
    0,  476,  477,    0,    0,    0,    0,    0,    0,    0,
    0,  478,    0,    0,  218,  479,  218,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  218,    0,    0,    0,    0,
   81,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  218,    0,    0,    0,    0,    0, 1260,    0,
  480,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,    0,    0,  218,    0,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,  484,  485,  486,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  487,    0,    0,    0,    0,  218,
  218,  122,  123,  488,  489,  490,  491,  492,  493,  494,
  495,  496,  124,  497,    0,  126,  218,    0,  498,  499,
    0,  500,  501,  502,    0,  129,  130,  503,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  504,  141,    0,  505,
  143,  144,    0,  145,    0,  506,  507,  508, 1273,  509,
  510, 1259,  511,  146,    0,  468,  692,    0,    0,  693,
    0,    0,  469,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  470,    0,  694,    0,    0,  695,
  471,  472,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  585,    0,    0,  512,    0,
  513,    0,    0,    0,    0,    0,    0,    0,  473,    0,
    0,  474,    0,  515,    0,    0,    0,    0,  333,  696,
    0,    0,    0,    0,    0,  697,  698,  699,    0,  700,
  701,  702,    0,  703,    0,    0,  475,    0,  476,  477,
    0,    0,    0,    0,    0,    0,    0,    0,  478,    0,
    0,  704,  479,  705,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  706,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  707,
    0,    0,    0,    0,    0, 1260,    0,  480,    0,    0,
    0,    0,    0,  481,    0,    0,    0,    0,    0,    0,
    0,  708,    0,  482,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  483,  484,  485,
  486,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  487,    0,    0,    0,    0,  709,  710,  122,  123,
  488,  489,  490,  491,  492,  493,  494,  495,  496,  124,
  497,    0,  126,  711,    0,  498,  499,    0,  500,  501,
  502,    0,  129,  130,  503,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  504,  141,    0,  505,  143,  144,    0,
  145,    0,  506,  507,  508, 1281,  509,  510, 1259,  511,
  146,    0,  468,  148,    0,    0,  148,    0,    0,  469,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  470,    0,  148,    0,    0,  148,  471,  472,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  585,    0,    0,  512,    0,  513,    0,    0,
    0,    0,    0,    0,    0,  473,    0,    0,  474,    0,
  515,    0,    0,    0,    0,  333,  148,    0,    0,    0,
    0,    0,  148,  148,  148,    0,  148,  148,  148,    0,
  148,    0,    0,  475,    0,  476,  477,    0,    0,    0,
    0,    0,    0,    0,    0,  478,    0,    0,  148,  479,
  148,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  148,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  148,    0,    0,    0,
    0,    0, 1260,    0,  480,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,    0,    0,    0,  148,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,  484,  485,  486,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  487,    0,
    0, 1354,    0,  148,  148,  122,  123,  488,  489,  490,
  491,  492,  493,  494,  495,  496,  124,  497,    0,  126,
  148,    0,  498,  499,    0,  500,  501,  502,    0,  129,
  130,  503,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  504,  141,    0,  505,  143,  144,    0,  145,    0,  506,
  507,  508, 1284,  509,  510, 1259,  511,  146,    0,  468,
    0, 1355,    0,    0,    0,    0,  469,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  470,    0,
    0,    0,    0,    0,  471,  472,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1356,    0,    0,  585,
    0, 1357,  512,    0,  513,    0,    0, 1358,    0,    0,
    0,    0,  473,    0,    0,  474,    0,  515,  115,    0,
    0,  116,  333,  117,  118,  119, 1359,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,    0,
  475,    0,  476,  477,    0,    0,    0,  124,  125,    0,
  126,    0,  478,    0,    0,    0,  479,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1260,
    0,  480,    0,    0,    0,    0,    0,  481,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  482,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  483,  484,  485,  486,  255,  256,  116,    0,  117,
  118,  119,  120,    0,  121,  487,    0,    0,    0,    0,
    0,    0,  122,  123,  488,  489,  490,  491,  492,  493,
  494,  495,  496,  124,  497,    0,  126,    0,    0,  498,
  499,    0,  500,  501,  502,    0,  129,  130,  503,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  504,  141,    0,
  505,  143,  144,    0,  145,    0,  506,  507,  508, 1289,
  509,  510, 1259,  511,  146,    0,  468,    0,    0,    0,
    0,    0,    0,  469,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  470,    0,    0,    0,    0,
    0,  471,  472,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  585,    0,    0,  512,
    0,  513,    0,    0,    0,    0,    0,    0,    0,  473,
    0,    0,  474,    0,  515,  115,    0,    0,  116,  333,
  117,  118,  119,  120,    0,  121,    0,    0,    0,    0,
    0,    0,    0,  122,  123,    0,    0,  475,    0,  476,
  477,    0,    0,    0,  124,  125,    0,  126,    0,  478,
    0,    0,    0,  479,  127,  128,    0,  129,  130,    0,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1260,    0,  480,    0,
    0,    0,    0,    0,  481,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  483,  484,
  485,  486,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  487,    0,    0,    0,    0,    0,    0,  122,
  123,  488,  489,  490,  491,  492,  493,  494,  495,  496,
  124,  497,    0,  126,    0,    0,  498,  499,    0,  500,
  501,  502,    0,  129,  130,  503,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  504,  141,    0,  505,  143,  144,
    0,  145,    0,  506,  507,  508, 1292,  509,  510, 1259,
  511,  146,    0,  468,    0,    0,    0,    0,    0,    0,
  469,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  470,    0,    0,    0,    0,    0,  471,  472,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  585,    0,    0,  512,    0,  513,    0,
    0,    0,    0,    0,    0,    0,  473,    0,    0,  474,
    0,  515,   28,    0,    0,   28,  333,   28,   28,   28,
   28,    0,   28,    0,    0,    0,    0,    0,    0,    0,
   28,   28,    0,    0,  475,    0,  476,  477,    0,    0,
    0,   28,   28,    0,   28,    0,  478,    0,    0,    0,
  479,   28,   28,    0,   28,   28,    0,   28,    0,    0,
   28,   28,   28,   28,   28,    0,    0,    0,   28,   28,
    0,    0,    0,    0,   28,   28,   28,    0,   28,   28,
   28,    0,   28,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   28,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1260,    0,  480,    0,    0,    0,    0,
    0,  481,  660,    0,    0,    0,    0,    0,    0,    0,
    0,  482,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  483,  484,  485,  486,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  487,
    0,    0,    0,    0,    0,    0,  122,  123,  488,  489,
  490,  491,  492,  493,  494,  495,  496,  124,  497,    0,
  126,    0,    0,  498,  499,    0,  500,  501,  502,    0,
  129,  130,  503,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  504,  141,    0,  505,  143,  144,    0,  145,    0,
  506,  507,  508, 1295,  509,  510, 1259,  511,  146,    0,
  468,    0,    0,    0,    0,    0,    0,  469,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,  471,  472,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  692,    0,  692,  692,
    0,  692,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  473,  692,    0,  474,    0,    0,  692,
  115,    0,    0,  116,    0,  117,  118,  119,  120,    0,
  121,    0,    0,    0,    0,    0,    0,    0,  122,  123,
    0,  475,    0,  476,  477,    0,    0,    0,    0,  124,
  125,    0,  126,  478,    0,    0,    0,  479,    0,  127,
  128,    0,  129,  130,    0,  131,    0,    0,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,    0,    0,
    0,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  146,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1260,    0,  480,    0,    0,  666,    0,    0,  481,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,  484,  485,  486,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  487,    0,    0,    0,
    0,    0,    0,  122,  123,  488,  489,  490,  491,  492,
  493,  494,  495,  496,  124,  497,    0,  126,    0,    0,
  498,  499,    0,  500,  501,  502,    0,  129,  130,  503,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  504,  141,
    0,  505,  143,  144,    0,  145,  692,  506,  507,  508,
    0,  509,  510,  692,  511,  146,    0,    0,    0,    0,
  692,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  692,    0,    0,    0,    0,    0,  692,  692,
    0,    0,    0,    0,    0,    0,  692,    0,    0,  686,
    0,  686,  686,    0,  686,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  692,  686,    0,  692,
    0,    0,  686,  115,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,    0,  692,    0,  692,  692,    0,    0,
    0,    0,  124,  125,    0,  126,  692,    0,    0,    0,
  692,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  692,    0,    0,    0,    0,
    0,  692,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  692,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  692,  692,  692,  692,  692,
  692,  692,    0,  692,  692,  692,  692,    0,  692,  692,
    0,    0,    0,    0,    0,    0,  692,  692,  692,  692,
  692,  692,  692,  692,  692,  692,  692,  692,  692,    0,
  692,    0,    0,  692,  692,    0,  692,  692,  692,    0,
  692,  692,  692,  692,    0,    0,  692,  692,  692,  692,
  692,    0,    0,    0,  692,  692,    0,    0,    0,    0,
  692,  692,  692,    0,  692,  692,  692,    0,  692,  686,
  692,  692,  692,    0,  692,  692,  686,  692,  692,    0,
    0,    0,    0,  686,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  686,    0,    0,    0,    0,
    0,  686,  686,    0,    0,  585,    0,  590,  512,  686,
  513,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  515,    0,    0,    0,    0,  333,  686,
    0,    0,  686,    0,    0,  600,    0,    0,  600,    0,
  600,  600,  600,  600,    0,  600,    0,    0,    0,    0,
    0,    0,    0,  600,  600,    0,    0,  686,    0,  686,
  686,    0,    0,    0,  600,  600,    0,  600,    0,  686,
    0,    0,    0,  686,  600,  600,    0,  600,  600,    0,
  600,    0,    0,  600,  600,  600,  600,  600,    0,    0,
    0,  600,  600,    0,    0,    0,    0,  600,  600,  600,
    0,  600,  600,  600,    0,  600,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  600,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  686,    0,
    0,    0,    0,    0,  686,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  686,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  686,  686,
  686,  686,  686,  686,  686,    0,  686,  686,  686,  686,
    0,  686,  686,    0,    0,    0,    0,    0,    0,  686,
  686,  686,  686,  686,  686,  686,  686,  686,  686,  686,
  686,  686,    0,  686,    0,    0,  686,  686,    0,  686,
  686,  686,    0,  686,  686,  686,  686,    0,    0,  686,
  686,  686,  686,  686,    0,    0,    0,  686,  686,    0,
    0,    0,    0,  686,  686,  686,    0,  686,  686,  686,
    0,  686,  468,  686,  686,  686,    0,  686,  686,  469,
  686,  686,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  470,    0,    0,    0,    0,    0,  471,  472,    0,
    0,  585,    0,    0,  512,    0,  513,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  515,
    0,    0,    0,    0,  333,  473,    0,    0,  474,    0,
    0,  287,    0,    0,  287,    0,  287,  287,  287,  287,
    0,  287,    0,    0,    0,    0,    0,    0,    0,  287,
  287,    0,    0,  475,    0,  476,  477,    0,    0,    0,
  287,  287,    0,  287,    0,  478,    0,    0,    0,  479,
  287,  287,    0,  287,  287,    0,  287,    0,    0,  287,
  287,  287,  287,  287,    0,    0,    0,  287,  287,    0,
    0,    0,    0,  287,  287,  287,    0,  287,  287,  287,
    0,  287,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  287,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  480,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,  484,  485,  486,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  487,    0,
    0,    0,    0,    0,    0,  122,  123,  488,  489,  490,
  491,  492,  493,  494,  495,  496,  124,  497,    0,  126,
    0,    0,  498,  499,    0,  500,  501,  502,    0,  129,
  130,  503,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  504,  141,    0,  505,  143,  144,    0,  145,  468,  506,
  507,  508,    0,  509,  510,  469,  511,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  470,    0,    0,
    0,    0,    0,  471,  472,    0,    0,  585,    0,    0,
  512,    0,  513,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  515,    0,    0,    0,    0,
  333,  473,    0,    0,  474,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  475,
    0,  476,  477,    0,    0,    0,    0,    0,    0,    0,
    0,  478,    0,    0,    0,  479,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  480,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,  790,    0,    0,    0,    0,    0,
  483,  484,  485,  486,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  487,    0,    0,    0,    0,    0,
    0,  122,  123,  488,  489,  490,  491,  492,  493,  494,
  495,  496,  124,  497,    0,  126,    0,    0,  498,  499,
    0,  500,  501,  502,    0,  129,  130,  503,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  504,  141,    0,  505,
  143,  144,    0,  145,  468,  506,  507,  508,    0,  509,
  510,  469,  511,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  470,    0,    0,    0,    0,    0,  471,
  472,    0,    0,  585, 1079,    0,  512,    0,  513,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  515,    0,    0,    0,    0,  333,  473,    0,    0,
  474,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  475,    0,  476,  477,    0,
    0,    0,    0,    0,    0,    0,    0,  478,    0,    0,
    0,  479,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   14,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  480,    0,    0,    0,
    0,    0,  481,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  483,  484,  485,  486,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  487,    0,    0,    0,    0,    0,    0,  122,  123,  488,
  489,  490,  491,  492,  493,  494,  495,  496,  124,  497,
    0,  126,    0,    0,  498,  499,    0,  500,  501,  502,
    0,  129,  130,  503,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  504,  141,    0,  505,  143,  144,    0,  145,
  468,  506,  507,  508,    0,  509,  510,  469,  511,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,  471,  472,    0,    0,  585,
    0,    0,  512,    0,  513,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  515,    0,    0,
    0,    0,  333,  473,    0,    0,  474,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  475,    0,  476,  477,    0,    0,    0,    0,    0,
    0,    0,    0,  478,    0,    0,    0,  479,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  480,    0,    0,    0,    0,    0,  481,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,  484,  485,  486,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  487,    0,    0,    0,
    0,    0,    0,  122,  123,  488,  489,  490,  491,  492,
  493,  494,  495,  496,  124,  497,    0,  126,    0,    0,
  498,  499,    0,  500,  501,  502,    0,  129,  130,  503,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  504,  141,
    0,  505,  143,  144,    0,  145,  468,  506,  507,  508,
    0,  509,  510,  469,  511,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  470,    0,    0,    0,    0,
    0,  471,  472,    0,    0,  585,    0,    0,  512,    0,
  513,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  515,    0,    0,    0,    0,  333,  473,
    0,    0,  474,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  475,    0,  476,
  477,    0,    0,    0,    0,    0,    0,    0,    0,  478,
    0,    0,    0,  479,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  480,    0,
    0,    0,    0,    0,  481,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1287,    0,    0,  483,  484,
  485,  486,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  487,    0,    0,    0,    0,    0,    0,  122,
  123,  488,  489,  490,  491,  492,  493,  494,  495,  496,
  124,  497,    0,  126,    0,    0,  498,  499,    0,  500,
  501,  502,    0,  129,  130,  503,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  504,  141,    0,  505,  143,  144,
    0,  145,  468,  506,  507,  508,    0,  509,  510,  469,
  511,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  470,    0,    0,    0,    0,    0,  471,  472,    0,
    0,  585,    0,    0,  512,    0,  513,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  515,
    0,    0, 1339,    0,  333,  473,    0,    0,  474,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  475,    0,  476,  477,    0,    0,    0,
    0,    0,    0,    0,    0,  478,    0,    0,    0,  479,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  480,    0,    0,    0,    0,    0,
  481,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  483,  484,  485,  486,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  487,    0,
    0,    0,    0,    0,    0,  122,  123,  488,  489,  490,
  491,  492,  493,  494,  495,  496,  124,  497,    0,  126,
    0,    0,  498,  499,    0,  500,  501,  502,    0,  129,
  130,  503,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  504,  141,    0,  505,  143,  144,    0,  145,  468,  506,
  507,  508,    0,  509,  510,  469,  511,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  470,    0,    0,
    0,    0,    0,  471,  472,    0,    0, 1055,    0, 1055,
 1055,    0, 1055,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1055,    0,    0,    0,    0,
 1055,  473,    0,    0,  474,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  475,
    0,  476,  477,    0,    0,    0,    0,    0,    0,    0,
    0,  478,    0,    0,    0,  479,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  480,    0,    0,    0,    0,    0,  481,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  482,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1506,    0,    0,
  483,  484,  485,  486,  255,  256,  116,    0,  117,  118,
  119,  120,    0,  121,  487,    0,    0,    0,    0,    0,
    0,  122,  123,  488,  489,  490,  491,  492,  493,  494,
  495,  496,  124,  497,    0,  126,    0,    0,  498,  499,
    0,  500,  501,  502,    0,  129,  130,  503,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  504,  141,    0,  505,
  143,  144,    0,  145, 1055,  506,  507,  508,    0,  509,
  510, 1055,  511,  146,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1055,    0,    0,    0,    0,    0, 1055,
 1055,    0,    0,  514,    0,    0,  512,    0,  513,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  515,    0,    0,    0,    0,  333, 1055,    0,    0,
 1055,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1055,    0, 1055, 1055,    0,
    0,    0,    0,    0,    0,    0,    0, 1055,    0,    0,
    0, 1055,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1055,    0,    0,    0,
    0,    0, 1055,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1055,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1055, 1055, 1055, 1055,
 1055, 1055, 1055,    0, 1055, 1055, 1055, 1055,    0, 1055,
 1055,    0,    0,    0,    0,    0,    0, 1055, 1055, 1055,
 1055, 1055, 1055, 1055, 1055, 1055, 1055, 1055, 1055, 1055,
    0, 1055,    0,    0, 1055, 1055,    0, 1055, 1055, 1055,
    0, 1055, 1055, 1055, 1055,    0,    0, 1055, 1055, 1055,
 1055, 1055,    0,    0,    0, 1055, 1055,    0,    0,    0,
    0, 1055, 1055, 1055,    0, 1055, 1055, 1055,    0, 1055,
  468, 1055, 1055, 1055,    0, 1055, 1055,  469, 1055, 1055,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,  471,  472,    0,    0,  585,
    0,    0,  512,    0,  513,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  515,    0,    0,
    0,    0,  333,  473,    0,    0,  474,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  475,    0,  476,  477,    0,    0,    0,    0,    0,
    0,    0,    0,  478,    0,    0,    0,  479,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  480,    0,    0,    0,    0,    0,  481,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  482,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,  484,  485,  486,  255,  256,  116,    0,
  117,  118,  119,  120,    0,  121,  487,    0,    0,    0,
    0,    0,    0,  122,  123,  488,  489,  490,  491,  492,
  493,  494,  495,  496,  124,  497,    0,  126,    0,    0,
  498,  499,    0,  500,  501,  502,    0,  129,  130,  503,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,    0,    0,  139,  504,  141,
    0,  505,  143,  144,    0,  145,  468,  506,  507,  508,
    0,  509,  510,  469,  511,  146,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  470,    0,    0,    0,    0,
    0,  471,  472,    0,    0, 1055,    0,    0, 1055,    0,
 1055,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1055,    0,    0,    0,    0, 1055,  473,
    0,    0,  474,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  475,    0,  476,
  477,    0,    0,    0,    0,    0,    0,    0,    0,  478,
    0,    0,    0,  479,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  480,    0,
    0,    0,    0,    0,  481,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  483,  484,
  485,  486,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  487,    0,    0,    0,    0,    0,    0,  122,
  123,  488,  489,  490,  491,  492,  493,  494,  495,  496,
  124,  497,    0,  126,    0,    0,  498,  499,    0,  500,
  501,  502,    0,  129,  130,  503,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  504,  141,    0,  505,  143,  144,
  567,  145, 1055,  506,  507,  508,    0,  509,  510, 1055,
  511,  146,  568,    0,    0,    0,    0,    0,    0,    0,
    0, 1055,    0,    0,    0,    0,    0, 1055, 1055,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  567,  567,    0,    0,  567,    0,    0,    0,    0,  562,
    0,    0,  568,  568,    0, 1055,  568,    0, 1055,  567,
    0,  567,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  568,    0,  568,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1055,    0, 1055, 1055,    0,    0,    0,
  562,  567,    0,  562,    0, 1055,    0,    0,    0, 1055,
    0,    0,    0,  568,    0,    0,    0,    0,  562,    0,
  562,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  562,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1055,    0,    0,    0,    0,    0,
 1055,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1055,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1055, 1055, 1055, 1055, 1055, 1055,
 1055,    0, 1055, 1055, 1055, 1055,    0, 1055, 1055,    0,
    0,    0,    0,    0,    0, 1055, 1055, 1055, 1055, 1055,
 1055, 1055, 1055, 1055, 1055, 1055, 1055, 1055,    0, 1055,
    0,  584, 1055, 1055,    0, 1055, 1055, 1055,    0, 1055,
 1055, 1055, 1055,    0,    0, 1055, 1055, 1055, 1055, 1055,
    0,    0,    0, 1055, 1055,    0,    0,    0,    0, 1055,
 1055, 1055,    0, 1055, 1055, 1055,    0, 1055,  567, 1055,
 1055, 1055,  584, 1055, 1055,  584, 1055, 1055,    0,    0,
  568,    0,    0,  567,    0,  567,  567,  567,    0,  574,
  584,  567,  584,    0,  567,  568,    0,  568,  568,  568,
    0,    0,    0,  568,    0,    0,  568,  567,    0,    0,
    0,    0,    0,  567,    0,    0,  567,  562,    0,  568,
    0,    0,  584,    0,    0,  568,    0,    0,  568,    0,
  574,    0,  562,  574,  562,  562,  562,    0,    0,    0,
  562,    0,    0,  562,    0,    0,    0,    0,  574,    0,
  574,    0,    0,    0,    0,    0,  562,    0,  567,    0,
    0,    0,  562,    0,    0,  562,    0,    0,    0,    0,
  568,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  574,    0,  567,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  568,    0,    0,    0,    0,    0,
  559,    0,    0,  567,    0,    0,    0,  562,    0,  567,
    0,    0,    0,  567,    0,  568,    0,    0,    0,    0,
  567,  568,    0,    0,    0,  568,    0,    0,    0,    0,
    0,  562,  568,    0,    0,    0,    0,    0,  567,    0,
    0,  559,    0,  567,  559,    0,    0,    0,    0,    0,
  568,    0,  562,    0,    0,  568,    0,    0,  562,  559,
    0,  559,  562,    0,    0,    0,    0,    0,  567,  562,
    0,    0,  567,    0,    0,    0,    0,    0,    0, 1063,
  568,    0, 1063,    0,  568,    0,    0,  562,    0,  584,
    0,  559,  562,    0,    0,    0,    0, 1065,    0, 1063,
    0,    0, 1063,    0,  584,    0,    0,  584,  584,    0,
    0,    0,  584,    0,    0,  584,    0,  562,    0,    0,
    0,  562,    0,    0,    0,    0,    0,    0,  584,    0,
    0,    0,    0,    0,  584,    0,    0,  584,    0,    0,
    0,    0, 1063,    0,    0,    0,    0,  574, 1063, 1063,
 1063,    0, 1063, 1063, 1063,    0, 1063,    0,    0,    0,
    0,    0,  574,    0,    0,  574,  574,    0,    0,    0,
  574,    0,    0,  574, 1063,    0, 1063,    0,    0,  584,
    0,    0,    0,    0,    0,    0,  574,    0,    0,    0,
    0,    0,  574,    0,    0,  574,    0,    0,    0,    0,
    0,    0,    0,  584,    0, 1063,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1063,    0,  584,    0,    0,    0,    0,    0,
  584,    0,    0,    0,  584,    0,    0,  574,    0,    0,
    0,  584,    0,    0, 1063,  108,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  559,  584,
    0,  574,    0,    0,  584,    0,    0,    0,    0,    0,
    0,    0,    0,  559,    0,    0,  559,  559,    0, 1063,
 1063,  559,  574,  109,  559,    0,    0,    0,  574,  584,
    0,    0,  574,  584,    0,  110, 1063,  559,    0,  574,
    0,    0,    0,  559,    0,    0,  559,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  574,    0,  111,
    0,   49,  574,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  574,  559,    0,
    0,  574,    0,    0,    0,    0,    0,    0,    0,   49,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   49,  559,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  112,    0,    0,    0,    0,    0,
    0,    0,    0,  559,  113,   49,    0,    0,    0,  559,
    0,    0,    0,  559,    0,    0,    0,    0,    0,    0,
  559,    0,    0,    0,    0,    0,    0,    0,    0,  114,
    0,    0,    0,    0,    0,    0,    0,    0,  559,    0,
    0,    0,    0,  559,    0,    0,    0,  115,    0,    0,
  116,    0,  117,  118,  119,  120,    0,  121,    0,    0,
    0,    0,    0,    0,    0,  122,  123,    0,  559,    0,
   49,    0,  559,    0,    0,    0,  124,  125,    0,  126,
   49,    0,    0,    0,    0,    0,  127,  128,    0,  129,
  130,    0,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,   49,    0,    0,    0,  139,
  140,  141,    0,  142,  143,  144,    0,  145,    0,  327,
    0,    0,    0,   51,    0,    0,   51,  146,   51,   51,
   51,   51,    0,   51,    0,    0,    0,    0,    0,    0,
    0,   51,   51,    0,    0,    0,    0,  327,    0,    0,
    0,    0,   51,   51,  327,   51,    0,    0,  327,  327,
    0,  327,   51,   51,  327,   51,   51,    0,   51,    0,
    0,   51,   51,   51,   51,   51,    0,  327,    0,   51,
   51,    0,    0,  327,    0,   51,   51,   51,    0,   51,
   51,   51,    0,   51,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   51,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1351,    0,    0,    0,    0,
  327,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  327,    0,
    0,    0,    0,    6,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  327, 1561, 1562,    0, 1563,    0,    0,
    0,    0,    0,  327,    0,    0,    0,    0,    0,    0,
    0,    0,  327, 1564,  327,    0,    0,    0,    0, 1565,
    0,  327,    0,    0,  327,    0,  327,  327,  327,  327,
    0,  327,    0,    0,    0,    0,    0,    0,    0,  327,
  327,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  327,  327,    0,  327,    0,    0,    0,    0,    0,    0,
  327,  327,    0,  327,  327,    0,  327,    0,    0,  327,
  327,  327,  327,  327,    0,    0, 1566,  327,  327,  327,
  327,  327,    0,  327,  327,  327,    0,  327,  327,  327,
 1351,  327,    0,    0,   14,    0,    0,    0,    0,    0,
    0,  327,    0,    0,    0,    0,    0,    0,    0, 1567,
    0,    0,    0,    0,    0,    0,    0,    0,    6,   16,
    0,    0,    0,    0,    0,    0,    0,    0, 1773, 1561,
 1562,    0, 1563,    0,    0,    0,    0,  486,    0,    0,
  116,    0,  117,  118,  119,  120,    0,  121, 1564,    0,
    0,    0,    0,    0, 1565,  122,  123,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1568,  125,    0,  126,
    0,    0,    0,    0,    0,    0,  127,  128,    0,  129,
  130,    0,  131,    0,    0,  132, 1569,  134,  135,  136,
    0,    0,    0,  137,  138, 1570, 1571, 1572,    0,  139,
  140,  141,    0,  142,  143,  144,    0,  145,    0,    0,
    0, 1566,    0,    0,    0,    0,    0,  146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   14,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1567,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   16,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  486,    0,    0,  116,    0,  117,  118,  119,
  120,    0,  121,    0,    0,  692,    0,    0,  693,    0,
  122,  123,    0,    0,  911,    0,    0,    0,    0,    0,
    0, 1568,  125,    0,  126,  694,    0,    0,  695,    0,
    0,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132, 1569,  134,  135,  136,    0,    0,    0,  137,  138,
 1570, 1571, 1572,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,  696,    0,
    0,    0,  146,    0,  697,  698,  699,    0,  700,  701,
  702,    0,  703,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  704,    0,  705,  692,    0,    0,  693,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  694,    0,    0,  695,    0,    0,    0,
    0,  706,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  707,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  696,    0,    0,    0,
  708,    0,  697,  698,  699,    0,  700,  701,  702,    0,
  703,    0,    0,    0,    0,  115,    0,    0,  116,    0,
  117,  118,  119,  120,    0,  121,    0,    0,  704,    0,
  705,    0,    0,  122,  123,  709,  710,    0,    0,    0,
    0,    0,    0,    0,  124,  125,    0,  126,    0,    0,
    0,    0,  711,    0,  127,  128,    0,  129,  130,  706,
  131,    0,    0,  132,  133,  134,  135,  136,    0, 1025,
    0,  137,  138,    0,    0,    0,  707,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,    0,    0,    0,
    0,    0, 1026,    0,    0,  146,    0,    0,  708,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  115,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,    0,    0,    0,    0,
    0,  122,  123,  709,  710,    0,    0,    0,    0,    0,
    0,    0,  124,  125,    0,  126,    0,    0, 1027,    0,
  711,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132,  133,  134,  135,  136,    0,    0,    0,  137,
  138,    0,    0,    0,    0,  139,  140,  141,    0,  142,
  143,  144,  115,  145,    0,  116,    0,  117,  118,  119,
  120,    0,  121,  146,    0,    0,    0,    0,    0,    0,
  122,  123,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  124,  125,    0,  126,    0,    0,    0,    0,    0,
    0,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
  115,    0,  146,  116,  192,  117,  118,  119,  120,  193,
  121,  194,  195,  196,  197,  198,  199,  200,  122,  123,
  201,  202,  203,  204,  205,    0,    0,  206,  207,  124,
  125,  208,  126,    0,    0,  209,  210,  211,  212,  127,
  128,  213,  129,  130,  214,  131,  215,  216,  132,  133,
  134,  135,  136,    0,    0,    0,  137,  138,  217,  218,
  219,  220,  139,  140,  141,  221,  142,  143,  144,  222,
  145,  223,  224,  225,  226,  227,  228,  229,  230,  231,
  146,  479,    0,    0,  479,  479,  479,  479,  479,  479,
    0,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,    0,    0,  479,  479,
  479,  479,  479,  479,    0,    0,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,    0,    0,    0,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,
};
static short yycheck[] = {                                       9,
    0,    4,   61,  627,   71,  114,   16,   70,   18,  164,
  165,  411,  164,  165,   16,  174,   77,  820,  102,  446,
   70,  820,  446,   80,  442,  555,   93,  413,  661,  630,
  319,  876,  514, 1350,   65, 1424, 1103,  438,   97,   99,
  101,  574,  596,  575,  830,  335,  103,  364,   58,   59,
  790,   61,   62,   63,   64,   65,  796,    0, 1167,  823,
   70,   71,   72, 1089,   74,   96,   76,   77,   62,   79,
   80,   81,   69,  104,  423,  776, 1123,  746, 1030,   89,
   74,   91,   92,   93,   63,   95,   96,   97,   98,   91,
  100,  101,  102,  103,  104,  179,  106,   76,  100,   75,
 1175, 1167,    0,  452,   98,  983,  156,  808,  158,  176,
  874,   45,  106,    0,  393, 1209,   59,  270,    0, 1231,
  514, 1173,  652,   99,  678, 1077,  656,  739, 1180,   44,
   44,  661,  183,    0,  422,    0, 1351,    0,  169, 1191,
   41,    0,    0, 1167,   44,   44,  156,  157,  158,    0,
    0, 1226,  162,   41,   41,   45,    0,   44,   44,  169,
   40,   59, 1214,   41,  174,    0,  176,  177,  178,  179,
  279, 1167,   59,  183,   41,  753,   40,   59,  157,   44,
  758,  273,    0,  177,    0,   40,  881, 1167,   45,    0,
   41,  585,   59,   44,   59,   41,   59,   41,   44,  178,
   59,   59,    0, 1319,  885,  364, 1322, 1167,   59,   59,
  905,   41, 1369,  908,  367,   59,    0,   45, 1108,    0,
  638,    0,  917,   41,   59,   41,   44,   44,   44, 1381,
   41,   44,   44,   44,  661,    0,   40,  661,  235,    0,
    0,   59,   93,   59, 1416, 1417,  395,    0,   59, 1421,
   40,   44,    0, 1570, 1571,   41,   41,   41,  280,   63,
  310,   59,  371,  313,  664,  351,  305,  616,   44,  266,
   44,  689,  298,   63,    0,   59,  315,  566,   59,   45,
   59,  976,  613, 1023, 1024,  317, 1089,  346,    0,  395,
 1089,   44,  272,  582,   59,  273,  300,   63,   59,   59,
  310,    0,   61,  313,  310,  352,   59, 1416, 1417,  319,
  320,   59, 1421,  713,  364,   41,    0,   61,  305,    0,
 1060, 1396,  409,  447,   63,    0,    0,  317,  315, 1422,
  812,  371,    0,   59,  257, 1382,  346,  819,  268,  386,
  350,  388,   41,  265,  348,  304,  305,   59,  484,  436,
 1416, 1417, 1419,  272,    0, 1421,  336,   41,    0,  397,
   59, 1455,  412,  410,  758,  514,  458, 1582,  372,  366,
  348,  350,  422,   41,  424,   59,  426,  299,   59,  429,
  516,  448,  392,  393,   59,   59,    0, 1439,  435,  450,
  498,   59, 1416, 1417,  500, 1462,  428, 1421,  484,   41,
  440,  460,  412,  409,  934,  959,  445,  330,  434,  431,
  448,  753,  422,   59,  424,  574,  426,   59,  996,  429,
 1416, 1417, 1000,  258, 1540, 1421, 1166,   41, 1544,  359,
 1693,  317,  262,   59, 1823,  270, 1416, 1417,  448,  449,
  450, 1421,  439, 1395,  441,   59, 1147,  297,  458, 1766,
  460,    0,  984,  463, 1149,  449, 1416, 1417,  445,  456,
  463, 1421,  297,  995,   41,  376,  264,  999,  303,  336,
  305,  315,  920, 1154, 1626, 1170, 1171,   42,   43,  458,
   45,  325,   47,  327,  443, 1375, 1376,   41,  264,  376,
   44,  379,  336,   41,  344, 1190,  896,  315,  380,  899,
   44,  379,  315,  264,  315,  515,    0,  389,  301,  344,
   59, 1251, 1252,  913, 1254, 1255,  450,  352,  336,  336,
  306,  306,  380,  387,  574,  336,  857,  301,  680,  428,
 1919,  315,  446,  584,  261,  450,  380,  297,  297,  402,
  940,  325, 1345,  327,  444,  389, 1345,   41,  446,  379,
  839,  402,  336,  297,  564,  565,  566,  403,  875,  439,
  450, 1347,  852,  573,  376,   59, 1659,  577,  423, 1238,
  513,  565,  582, 1962,  584, 1642,  729,  627, 1690,  446,
 1113,  446, 1346, 1461,  344,  564,  421,  446,  432,  315,
 1637, 1123,  390,  450,  573, 1984,  380, 1672,  577, 1651,
  444,  987,  446, 1655, 1656,  389,  493,  886, 1702, 1233,
  336,  661,  513, 1225, 1689,  380,  315,  627,  906, 1151,
  477,  681,  450,  402,  389,  974,  623,  380,  446, 1745,
    0,  513,  344, 1115,    0,  446,  389,  336, 1795, 1691,
    0,    0,    0,  640,    0,  410,  450,  428,  432,  477,
  660,  661,  336,  401,  380,  513,  666,  654, 1000,  509,
  450,  820,  446,  389, 1927, 1405,  324,    0,  336,  513,
  344,   41,  827,  315,  509,   41,  494,  493,   44,  444,
  361,   41,    0,   41,  450,  262,  683, 1427,  738,   59,
  365,  444,  338,   59,  336,  513,   91, 2014,  298,   59,
   59,   59,  513,   59,    0,  681,  432,   41,  860, 1881,
 1882,  450,  277,   41,  262,  264,  875,  282,  262,  273,
  446, 1954,  336,   41,  344,  371,   59,  264,  738,  513,
   42,  728,  315, 1948,  264, 1183, 1184,   41,  735,  736,
    0,   59, 1636,  275, 1770,  315,  257,  446,  513,  509,
  781,  397,  306,  315,  400,  334, 1708, 1809, 1973, 1160,
  513, 1813,  446,   59,    0, 1460,  297,  283, 1835, 2002,
  416,  781, 1881, 1882, 1841,  380, 1828, 1177,  828, 1179,
  830, 1263,  832,  258,    0, 1233, 2001,  513,  262, 1189,
  942, 1492,    0, 1845,  348, 1640,  442, 2012,  456,   59,
 2015,    0,  379,  813,  446,   41,    0, 2022,  298,  341,
  390,  319,  262,  823,  513, 1881, 1882,  348,  828,  330,
  830,  315,  832,   59, 1425,  875, 1227,  432,  303,  839,
  880,  379,  446,  262,  434,  379,  258,  847,  848,  261,
  397,  390,  336,   59,  334,  855,  856, 1329,  428,  403,
 1236,   59,  300,  309,  848,  434,  906, 1881, 1882,  869,
   59,  871,  445,  428,  874,   59,  403,    0,  847, 1263,
  880,  513,  932,  403, 1926,  445,  886, 1929,  304,  305,
  275,  303,  462,  933,  934, 1881, 1882,    0,  885,  509,
  869,  448,  871, 1526, 1448,  892,  906, 1451, 1380, 1639,
  348, 1881, 1882,  259,  458,  379,  903, 1801,  264,  969,
  970,  971,  352,  269, 1966, 1941,  457, 1943,  459, 1305,
  401, 1881, 1882,  933,  934, 1951,   59, 1953,  262,  379,
 1089,   42,    0,    0,  262,  410,   47,  453,  454,  395,
  950,    0,    0,  276,  277,  315,   59,  950,  260,  315,
  379,  948,  446,  428, 1113,  315,  932,  315,  262,  325,
 1370,  327,    0,  939,  238,  975,  336, 1770,  264,  327,
  336, 1770, 1998,  339,  430,  985,  336,    0,  336,  338,
  956,  421,   41,  993,  994,   44,  450,  462,  300,  450,
  993,   59,   59,  969,  970,  971,  975,  315,  427,    0,
   59,   59,    0,  371,  264,    0,  985,  398,  297,  400,
  380,  322,  371,  477,  380,  262,  290,  443,  336, 1759,
  380,   59,  380,  389,  298, 1555,    0,   41,    0,  389,
   44,  389,    0, 1064,  390, 1445,   59, 1437,  397,  372,
   41,  400,  410,   41,  450,  379,   44, 1465,  264,  344,
  276,  379,   44, 1582, 1064,  344,  264,  416,   59,  265,
 1212,   59,  432, 1113,   59, 1354,  432, 1467, 1357,   41,
  264,  477,  432,  336,  432,  379,  446,   44,  444,  315,
  446,  344,   44,  442,  348,   59,  446,   59,  446,  457,
  316,   59,    0,  299,  390, 1489, 1490, 1107, 1108, 1526,
  336,   93, 1526, 1497, 1498,  298, 1500, 1501,  309,  304,
  305,  422,  450, 1123, 1508, 1509, 1510, 1511,  429, 1513,
 1514, 1515, 1516, 1517, 1518,    0,   93,  333,  446,    0,
  390,   93,  379,   41,  313,  314,   44,  280, 1941,  338,
 1943,  334, 1941,  513, 1943,    0,  319,  513, 1951,  298,
 1953,   59, 1951,  513, 1953,  513,  284, 1154,  453,  454,
 1220, 1901,  260,  276,  277,    0,  277,  393, 1908,  297,
   41,  282, 1796,   44,  390, 1235,  263,    0,    0, 1229,
  427, 1241,  390, 1233,   59,  334, 1345,    0,   59,  453,
  454, 1201, 1351,  339,  395, 1998,  390,  264,  397, 1998,
  258,  400,  300, 1206,   59,  338,   41,  433, 1218,   44,
  446,    0,  270, 1368, 1369, 1370,  362,  416,   41, 1229,
  509,   44,  309, 1233,   59,  426,  264,   42,   43,  297,
   45,   44,   47,  320,  377,    0,   59,   59,  371,  297,
  441,  434,  319,  442, 1220,  303,   59,  305,  443, 1401,
  399, 1403,   41,  276,  277,   44,  315,    0,   93, 1235,
  409, 1661, 1647,  258,  397, 1241,  325,  400,  327,    0,
   59,  259,  418,  316, 1358,  270,  344,  336,  394,  409,
  339,  269,   44,  416,  352,  434,  344, 1350,  431,   40,
  264, 1301,  338,  397,   59, 1356, 1696, 1347,    0, 1301,
 1350, 1351, 1359,    0,   44,  448,  436, 1692,  303,  442,
  305, 1463,  416,  271,  315,    0,   59,  315,  386,    0,
  388,  380,    0,  390,  325,  371,  327,  285,   59,  327,
  389,   93,  419, 1957,    0,  336, 1346, 1347,  336,    0,
 1350, 1351,  410,  315, 1354, 1355, 1356, 1357, 1358, 1359,
  393,  397,  390,    0,  400,  327, 1385,   59, 1387,  372,
   41, 1355,   59,   44,  336, 1375, 1376,  435,  276,  277,
  416,  329, 1382,  432,   59,   40,  284,    0,   59,  380,
  393,   59,  380,  258,  378,  444,  399,  446,  389,   46,
  433,  389,  376,   59,   41,  270,  442,   44,   59,  357,
    0,   58,    0,  376,   61,  276,  277,    0,  380,  277,
  260,  428,   59,  284,  282,  260,  390,  389,   41, 1948,
  433,   44,  297, 1582,   42,   43,  309,   45,  303,   47,
  305,  432,   42,   43,  432,   45,   59,   47,    0,  262,
 1443,  509, 1842,   41, 1973,  446,   44,    0,  446,  332,
  300,  509,    0,  298,  513, 1968,  378, 1970,  341,   59,
  432,   59,  277,  421,  372,  310,   59,  282,  376,  344,
 1629,    0, 2001,  262,  446,   42,   43,  486,   45, 1476,
   47,  333,  396, 2012, 1481,  393, 2015,  318,  434,  334,
    0,  399,  315, 2022, 1561,  318,    0,   59,  412,    0,
  331,  372,  325,  410,  327,  446,   59,  513,  331, 1519,
    0,   59,  513,  336,  397,  513,  339, 1519,  401,    0,
  273,  336,  393,  354,    0,  433,  315,    0,  399,  318,
   59,  354, 1582,   41,   42,   43,  325,   45,  327,   47,
   41,  513,  331,   44,   40,  368,    0,  336,    0,   59,
  339, 1561,  260,  376,  399,   59,  379,  380,   59, 1569,
 1570, 1571,  433,  318,  409,  354,  389,  402,  260,   59,
  260,  259, 1582,  338,  405,  452,  331,    0,   59,  368,
  260,  269,  405,   59,  339,  493,   59,  376,    0,  434,
  379,  380,  300,  485,  486,  276,  277,    0,  416,  354,
  389,    0, 1612, 1613,  427,   59,  371,   59,  300,  432,
  300, 1770,  453,  368,  520,  262,  405,  500,  264, 1629,
  300,  444,  493,  446,  453,  454, 1636, 1637,  333,  512,
  513,  514,  397,  315,  509,  400,   59,  313,  427, 1798,
 1795,   41,   41,  432,   44,   44,  333,   59,  384,  385,
  405,  416, 1662,  276,  277,  444,   59,  446,  281,  277,
   59,  284,   41,  368,  282,   44,  444,  277,  315,  258,
  493,  318,  282,  318,  297,  380, 1735,  442,  325,   41,
  327,  270,   44, 1662,  331,  485,  331,  453,  454,  336,
  513,  372,  339,   41,  517,  495,   44,  332,   41,  317,
  333,   44,  585,  403,  493,   41,  589,  354,   44,  354,
  277,  426,  393,    0,  303,  282,  305, 1826,  399, 1612,
 1613,  368, 1732,  368,  513, 1735,  336,  395,  517,  376,
 1732,  376,  379,  380,  336,  368,   42,   43,   40,   45,
   41,   47,  389,   44,  317,   41, 1796,  380,   44,  372,
  317,   46,  433,  498,   41,  315, 1766,   44,  405,    0,
  405,  315,  517,   58,   59,    0,   61,  376, 1827,  277,
  393,  376,   59,  428,  282,    0,  399,   41,   42,   43,
  427,   45, 1941,   47, 1943,  432, 1796, 1787, 1798, 1948,
   41, 1801, 1951,   44, 1953,   41,  426,  444,   44,  446,
   41,   41,   41,   44,   44,   44,   41, 1817,  442,   44,
  433, 1920, 1822,  356, 1973,  410,   41, 1827,   59,   44,
   61,    0,   41,  337,   59,   44, 1893,   41,   42,   43,
  377,   45,   41,   47,   59,   44,  319,   41,    0, 1998,
   44,  375, 2001, 1822,   42,   43,  493,   45,  493,   47,
  409, 1848,  777, 2012,  779, 1852, 2015,  497,  783,  784,
  785,   46,   41, 2022, 1874,   44,  513,  508,   41,  508,
  517,   44,  517,   58,   59,  758,   61,  276,  277,   41,
   59,  509,   44, 1893, 1894,  284,  284,   41,   42,   43,
  306,   45, 1895,   47,   44,  778,  461,   59, 1948,  782,
   41,   41,   41,   44,   44,   44,  789, 1957, 1612, 1613,
   41,  794,  795,   44,  446,  798,  799,  800,  801,  802,
  803,  366,  283, 1973,   61,   40,  264,   40,  389,  812,
   40,  814,  815,  816,  817,  818,  819,   40, 1948,  822,
 1940, 1013,   40, 1015, 1954, 1017, 1018, 1957, 1020, 1021,
 1022, 2001,    0,   41,   42,   43,  453,   45, 1968,   47,
 1970,   40, 2012, 1973,   40, 2015,   40,   40,   40,   40,
   40,  277, 2022,  372,  453,  858,  282,   40,   42,   43,
   40,   45,  453,   47,   40,  351,  449,  450,  451,  276,
  277, 2001, 2002,   41,  393,  878,   44,  284,    0,   40,
  399,   40, 2012,   40, 2014, 2015,   40,  890,  891,  315,
   40,   59, 2022,  277,  477,  478,   40,  258,  282,  402,
   41,   42,   43,   40,   45,   91,   47,  262,   44,   41,
   42,   43,  273,   45,  433,   47,   46,   40,   61,   41,
   42,   43,   44,   45,   46,   47,  264,  317,  395,   44,
   44,  276,  277,  317,   46,  420,  297,   59,   60,   61,
   62,   61,  303,  277,   40,  306,  337,  264,  282,  464,
  465,  466,  467,  468,  469,  470,  471,   46,   44,  277,
  315,  344,  344,  318,  282,  372,  344,  276,  276,   91,
  325,  297,  327,  365,  493,  264,  331,  276,  277,  390,
  428,  336,  281,  462,  339,  284,  393,  348,   42,   43,
  308,   45,  399,   47,  276,  277,  318,  390,  297,  354,
  450,  344,  284,  277,  275, 1008, 1009, 1010,  282,  331,
 1013, 1014, 1015,  368, 1017, 1018, 1019, 1020, 1021, 1022,
  439,  376,  439,  392,  379,  380,  433,  372, 1031, 1032,
   40, 1034,  354, 1036,  389,  345, 1039, 1040, 1041, 1042,
 1043, 1044,  403,    0,   40,   91,  368,   40,  393,  410,
  405,   40,  315, 1056,  399, 1058,   41,   42,   43,  264,
   45,  376,   47,  403,   44,   40,  382,  428,  375,  277,
   40, 1074,  427,  372,  282,  443,  380,  432,  315,   41,
  275,   63,  296,  405,   41,  314,  493,   44,  433,  444,
  372,  446,  428,  277,  393,  428,   40,  458,  282,  456,
  399,  462,   59,   44,  264,  440,  337,  444,  276,  277,
   44,  393, 1115,  494,   40,  273,  284,  399,   40,  297,
  340,  340,  410,  276,  308,    0,   41,   42,   43,  297,
   45,  379,   47,  281,  433,  379,  277,  379,  493,  408,
  262,  282,   41,   42,   43,  277,   45,   40,   47,  271,
  282,  433,  410,   40,  352,  277,   40,   40,  513,  362,
  282,   41,  517,  285,  277,   40,   41,   42,   43,   44,
   45,   46,   47,   41,   42,   43,   46,   45,   44,   47,
   44,  378,  317,   58,   59,   60,   61,   62,  438,  270,
  361,  317,  317,  315,   40,  517,  318,   44,  325,  321,
  266,   40,  493,  325,  372,  327,   40,  329,  264,  331,
  414,  265,   41,   58,  336,  337,   91,  339,   93,   41,
   41,   41,  264,  277,  446,  393,  306,  306,  282,   41,
   41,  399,  354,  355,   40,  357,   41,   42,   43,   40,
   45,  387,   47,  317,   40,  299,  368,   40,  370,   40,
  372,  373,  374,   40,  376,   40,   40,  379,  380,   40,
 1263,   40,   40,   40,   40,  433,   40,  389,   40,   40,
  262,  427,  305, 1276, 1277,   44, 1279, 1280,   44,  446,
   41,   41,  381,  405, 1287,   41,   42,   43,  264,   45,
  339,   47,  277,   42,   43,  339,   45,  282,   47,  421,
  273,  327,  384,  284,  399,  427, 1309,   41,   42,   43,
  432,   45,  379,   47, 1317, 1318, 1319,  379,  379, 1322,
  379, 1324,  444,   41,  446, 1328, 1329, 1330,  379,   41,
   42,   43,  379,   45, 1337,   47, 1339,   41,   42,   43,
  450,   45,  372,   47,   41,   41,   42,   43,  276,   45,
   40,   47,   41,   41,   42,   43,   41,   45,  315,   47,
   41,  318,  277,  317,   41,   44,   41,  282,  325,  305,
  327,  493,  427,   59,  331,   59,  290, 1380,  277,  336,
   40,  336,  339,  282,  336,   59,   59,  262,   59,  264,
  265,  513,   59,   59,    0,  517,  271,  354,  305,  324,
   59,   59,  277,   59,   59,   59,   59,  282,   59,  277,
  285,  368,   59,   59,  282,   59,   58, 1420,  339,  376,
  428,  532,  379,  380,  299,    0,  277,  333,  434,  304,
  305,  334,  389,  308,  434,   41,  298,  334,   44,  298,
  315,   41,  317,  318,   41,  378,  321,  348,  405,  365,
  325,   41,  327,   59,  329,  365,  331,   41,   41,   41,
   41,  336,  337,   41,  339,   41,   41,   41,   41,   44,
  427,   41,  277,   41,   41,  432,   41,  282,   41,  354,
  355,    0,  357,   41,   59, 1488,   41,  444,   40,  446,
   41, 1494,  315,  368,  443,  370, 1499,  372,  373,  374,
   40,  376,  378, 1506,  379,  380,   44,   41,  348,  340,
  458,   40,  336,  500,  389,  264,  333,  336,   40,   40,
  376,  277,   41,   41,   44,   44,  282,  402,  277,   40,
  405,  427,  376,  282,  376,   41,  493, 1540,   41,  301,
   59, 1544,  434,  277,  298,   41,  421, 1550,  282,  304,
  290,  301,  427,  428,    0,  455,  513,  432,  333,  409,
  517,  376,  376, 1566,   40,  277,  264,  276,  443,  444,
  282,  446,  264,  277,  317,  403,  450,  452,  282,   40,
  455,  277,  457,  458,  459,  460,  282,  462,  443,  277,
  462,   44,  270,   40,  282,  470,  471,   42,   43,   40,
   45,   41,   47,  264,  339,  270,  481,  482,  339,  484,
   42,   43,   44,   45,  450,   47,  491,  492,  493,  494,
  495,   41,  497,  427,  450,  500,  501,  502,  503,  504,
   41,    0,   59,  508,  509, 1638,   41,   59,  513,  514,
  515,  516,  517,  518,  519,  520,   59,  522,   41,   42,
   43,   59,   45,  305,   47,   41,  262,  532,   41,   42,
   43,   59,   45,   59,   47,   41,   42,   43,   59,   45,
   41,   47,   41,   42,   43,   44,   45,   46,   47,   41,
   42,   43,   59,   45,   59,   47,  301,  262,    0,  161,
   59,   60,   61,   62,   42,   43,   44,   45,    0,   47,
    0,    0,  580,  161, 1366,  162,  848,  321, 1711,  315,
  320,  839,  318,  582, 1717, 1718, 1238,  742,  993,  325,
  988,  327,   91, 1236,   93,  331,  950,   95,   81, 1233,
  336,   42,   43,  339,   45, 1228,   47,  963, 1741, 1679,
  315, 1680, 1745,  318,   42,   43, 1749,   45,  354,   47,
  325,  689,  327, 1756, 1757, 1163,  331,  276,  277,  409,
  443,  336,  368, 1218,  339,  284,    0,  369, 1229, 1817,
  376,  880, 1659,  379,  380,  424,  424,  915,  297,  354,
   41,   42,   43,  389,   45, 1960,   47, 1661, 1964,  666,
   41,   42,   43,  368,   45,    0,   47,   42,   43,  405,
   45,  376,   47, 1963,  379,  380, 1986,   41,  906, 1104,
   44, 1569, 1775, 1949,  389,  828, 1398, 1643,  429,  264,
  414,  427,  407, 1452,  726,   59,  432,  668,   13,  958,
  405,  354,  277,  966,  598,  602,   41,  282,  444,   44,
  446,  869,  167,  858, 1847,  277, 1123, 1386,   91, 1913,
  282,  775,  427,  372,   59, 1732,   61,  432,   41,   42,
   43, 1519,   45,  890,   47,   42,   43,   44,   45,  444,
   47,  446,  613, 1116,  393,   41,   42,   43,  846,   45,
  399,   47, 1278, 1407,  277, 1324,   -1,  493,   -1,  282,
   -1,   -1,   -1,  262,  277,  264,  265,   -1,   -1,  282,
   -1,  277,  271,   -1,   -1,   -1,  282,  513,  277,   -1,
 1913,  517,   -1,  282,  433,  277,  285,   -1,  493,   -1,
  282,   41,   42,   43,   -1,   45,   -1,   47,   -1,  277,
  299,    0,  301,   -1,  282,  304,  305,   -1,  513,  308,
   -1,   -1,  517,   -1,   -1,   -1,  315, 1950,  317,  318,
   -1,  262,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,  262,   -1,  277,  336,  337,   -1,
  339,  282,   41,   42,   43,   44,   45,   46,   47,  277,
   -1,   -1,   -1,   -1,  282,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,
  389,  282,   91,   -1,   93,   -1,  277,   -1,   -1,   -1,
   -1,  282,  277,  402,   -1,   -1,  405,  282,   -1,   -1,
   -1,   -1,  276,  277,   -1,   -1,   -1,   -1,   -1,   -1,
  284,   -1,  421,  258,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
  275,  276,  277,   -1,  443,  444,  281,  446,   -1,  284,
   -1,   -1,   -1,  452,   -1,  291,  455,   -1,  457,  458,
  459,  460,  297,  462,  277,   -1,  302,   -1,  303,  282,
  277,  470,  471,  309,   -1,  282,   -1,  313,   -1,   -1,
   -1,  277,  481,  482,   -1,  484,  282,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,  333,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,  372,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,  277,   -1,  393,
   -1,   -1,  282,  532,   -1,  399,   -1,  372,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  393,  395,
   -1,   -1,   -1,  262,  399,  264,  265,   -1,   -1,  433,
   -1,   -1,  271,  408,   -1,  410,   -1,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,
  426,   -1,    0,  428,  430,   -1,   -1,   -1,  433,   -1,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,  462,  327,   -1,
  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,  519,   -1,   -1,   -1,  291,   -1,   -1,
  389,   -1,   91,   -1,   93,   -1,   -1,   -1,  302,   -1,
   -1,   -1,   -1,  402,  302,  309,  405,   -1,   -1,   -1,
   -1,  309,   -1,   -1,   -1,  313,  314,   -1,  322,   -1,
   -1,   -1,  421,   -1,  322,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,  333,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  379,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,  395,  491,  492,  493,  494,  495,  395,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  411,   -1,  508,
  509,   -1,   -1,  411,  513,  514,  515,  516,  517,  518,
  519,  520,  426,  522,   -1,   -1,  430,   -1,  426,  433,
   -1,   -1,  430,  532,   -1,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,
  258,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,  460,  262,   -1,  264,  265,  275,  276,  277,
   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,  297,
   -1,   -1,    0,   -1,   -1,  303,   -1,   -1,   -1,   -1,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,  519,  315,   -1,  317,  318,
   -1,  519,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   41,   -1,   -1,   44,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   59,   -1,   61,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,  393,   -1,   -1, 1489, 1490,
  389,  399,   91,   -1,   93,   -1, 1497, 1498,   -1, 1500,
 1501,   -1,  410,  402,   -1,   -1,  405, 1508, 1509, 1510,
 1511,   -1, 1513, 1514, 1515, 1516, 1517, 1518,   -1,   -1,
  428,   -1,  421,   -1,   -1,  433,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,  462,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,  276,  277,
   -1,   -1,  271,  281,   -1,   -1,  284,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,  297,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,  372,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,
  389,  399,   91,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  410,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  428,   -1,  421,   -1,   -1,  433,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,  462,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   91,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   45,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   93,  292,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,  320,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,  348,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,  419,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,   -1,  453,  454,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,  271,   -1,   -1,  472,  473,  474,  475,  476,
  477,  478,  479,  480,   -1,   -1,  285,   -1,   -1,   -1,
  487,  488,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  496,
  299,    0,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
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
  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,  354,  355,  288,  357,   -1,   -1,
  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  402,  335,   -1,  405,   -1,   -1,   -1,  341,
  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,
   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,  367,   -1,  369,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,  398,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  415,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,  437,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  421,   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,    0,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   59,   -1,  271,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   41,  331,
   43,   44,   45,   -1,  336,  337,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,
   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,   -1,   40,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,  444,   -1,  446,   -1,   -1,   58,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,    0,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
  522,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,
  532,   -1,   -1,  325,   -1,  327,   -1,   41,   -1,  331,
   44,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,  262,
   -1,  264,  265,   -1,   -1,   59,   -1,   -1,  271,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  285,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  299,  379,  380,   -1,
   -1,  304,  305,   -1,   -1,  308,   -1,  389,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,  405,  327,   -1,  329,   -1,  331,   -1,
   -1,   -1,   -1,  336,  337,   -1,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  432,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,  446,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   42,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,
   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,
   -1,    0,   -1,   -1,  427,  428,   -1,   -1,   -1,  432,
   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,   -1,
  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   40,   41,   -1,   -1,   44,   -1,  470,  471,   -1,
  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   59,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,  518,  519,  520,   -1,  522,
   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,  532,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
  354,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,
  481,  482,  376,  484,   -1,   40,  380,   -1,   -1,   -1,
  491,  492,   -1,  494,  495,  389,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,  405,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  293,
    0,  532,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   41,   -1,   -1,   44,   -1,  470,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   59,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,
  514,  515,  516,  517,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  532,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,  368,
   -1,  455,   -1,  457,  458,  459,  460,  376,  462,   -1,
   -1,  380,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,  405,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,  432,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,  532,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,    0,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  522,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,  532,  409,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,  452,  368,   -1,
  455,   -1,  457,  458,  459,  460,  376,  462,   -1,   -1,
  380,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,
   -1,   -1,   91,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,  405,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,  432,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  446,  532,   -1,  275,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,  291,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,  302,   -1,   -1,   -1,
   -1,  481,  482,  309,  484,   -1,   -1,  313,  314,   -1,
   -1,  491,  492,  493,  494,  495,  322,  497,    0,   -1,
  500,  501,  502,  503,  504,   -1,   -1,  333,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  532,   42,   43,   -1,   45,   -1,   47,   41,
   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,
   -1,   60,   61,   62,   -1,   -1,   -1,   59,   60,   61,
   62,  387,   -1,  262,   -1,   -1,   -1,   -1,   -1,  395,
    0,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,   91,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  426,   -1,   -1,   -1,  430,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,  441,  315,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   59,
  329,   -1,  331,   -1,  460,  461,   -1,  336,  337,   -1,
  339,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
  389,   -1,   -1,  519,   -1,   -1,  522,   -1,   -1,   -1,
   -1,   59,    0,   -1,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,  444,   44,  446,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   59,  271,   -1,   -1,   -1,   -1,   -1,  277,  271,
   -1,   -1,   -1,  282,   -1,  277,  285,   -1,   -1,   -1,
  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  321,  315,  513,   -1,  318,   -1,  517,  321,
  329,   -1,   -1,  325,   -1,  327,   -1,  329,  337,  331,
   -1,   -1,  262,   -1,  336,  337,   -1,  339,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,  355,   -1,  357,   -1,
   -1,   -1,  354,  355,   -1,  357,   59,   -1,   -1,   -1,
   -1,  370,   -1,  372,  373,  374,  368,   -1,  370,    0,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,  405,  262,   -1,  336,   -1,   -1,  339,
   41,   -1,  421,   44,   -1,   -1,   -1,   -1,   -1,  421,
   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,   59,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
   -1,    0,  444,   -1,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   -1,   -1,   -1,  262,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,   -1,
   -1,  339,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,
   59,    0,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,  513,   -1,   -1,  444,  517,  446,  315,  376,   -1,
  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,  389,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   41,   -1,   -1,   44,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,  262,
   59,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,  427,
  368,   -1,   -1,    0,  432,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,  315,   -1,   41,  318,   -1,   44,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,  427,
   -1,  262,   59,  336,  432,  493,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,
   -1,  354,   -1,   -1,   -1,  513,   -1,    0,   -1,  517,
   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,   -1,
   -1,   -1,   -1,   -1,  325,  493,  327,   -1,   41,   -1,
  331,   44,  405,  262,   -1,  336,   -1,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,  513,   59,   -1,   -1,  517,
   -1,   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,
    0,  444,   -1,  446,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,
   -1,   -1,   -1,  262,   -1,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,  405,   -1,   -1,  336,   -1,   -1,
  339,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,   59,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
  513,   -1,    0,  444,  517,  446,  315,  376,   -1,  318,
  379,  380,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
  389,   -1,  331,   -1,   -1,  262,   -1,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,  354,   44,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,  427,  368,
   -1,   59,    0,  432,   -1,   -1,   -1,  376,   -1,   -1,
  379,  380,  513,   -1,   -1,  444,  517,  446,  315,   -1,
  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,
   -1,   -1,  339,   41,   -1,   -1,   44,   -1,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  427,   -1,
   -1,   59,   -1,  432,  493,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,  444,   -1,  446,   -1,  376,
   -1,   -1,  379,  380,  513,   -1,   -1,   -1,  517,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  405,   -1,
   -1,   -1,  325,    0,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,  493,   -1,  339,   -1,   -1,   -1,
  427,   -1,  262,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,  354,   -1,   -1,  513,   -1,    0,  444,  517,  446,
   -1,   -1,   -1,   -1,   41,  368,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,   41,  318,   -1,
   44,   -1,  405,   -1,   -1,  325,  493,  327,   -1,   -1,
   -1,  331,   -1,   -1,  262,   59,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,  427,   -1,  513,   -1,   -1,  432,
  517,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  446,   -1,    0,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   -1,   -1,   -1,  262,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  405,   41,   -1,  336,   44,
  493,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,  354,  427,   -1,   -1,
  513,   -1,  432,   -1,  517,   -1,   -1,    0,   -1,   -1,
  368,   -1,   -1,   -1,  444,   -1,  446,  315,  376,   -1,
  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,  389,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,   41,   -1,
   -1,   44,   -1,    0,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,  493,   -1,   -1,   59,   -1,   -1,  427,
  368,   -1,   -1,   -1,  432,  262,   -1,   -1,  376,   -1,
   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,   -1,
   -1,  389,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,  262,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,  427,
   -1,  318,   -1,   -1,  432,  493,   -1,   -1,  325,    0,
  327,   -1,   -1,   -1,  331,   -1,  444,   -1,  446,  336,
   -1,   -1,  339,   -1,   -1,  513,   -1,   -1,   -1,  517,
   -1,  315,   -1,   -1,  318,   -1,   -1,  354,   -1,   -1,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
   41,  368,  336,   44,   -1,  339,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,  493,   -1,  262,   59,   -1,
  354,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,  513,   -1,   -1,  405,  517,
   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
  315,  405,   -1,  318,   -1,   -1,   -1,  444,   -1,  446,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,  427,  339,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,  493,   -1,   -1,   -1,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  389,  318,  513,   -1,   -1,   -1,
  517,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,  493,
  405,   -1,   -1,  336,   -1,   -1,  339,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  513,
   -1,  354,  427,  517,   -1,   58,   -1,  432,  315,   -1,
   63,  318,   -1,   -1,   -1,  368,   -1,   -1,  325,  444,
  327,  446,   -1,  376,  331,   -1,   -1,  380,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,  493,  376,
   -1,   -1,   -1,  380,   -1,   -1,   -1,   -1,   -1,  432,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,  513,   -1,
   -1,  444,  517,  446,  315,   -1,   -1,  318,  405,   -1,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,  281,   -1,   -1,  336,   -1,   40,  339,   -1,
   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,  261,  354,   -1,   -1,   -1,  444,   -1,  446,
  493,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,
  279,   -1,   -1,   -1,   -1,  376,   -1,   -1,  287,  380,
  513,   -1,   -1,   -1,  517,  294,   -1,  296,  389,  298,
   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,   -1,
   -1,  310,   -1,   -1,  405,   -1,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  334,  513,   -1,   -1,   -1,
  517,  432,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  320,  397,   -1,
  323,   -1,  493,   -1,   -1,  404,   -1,   -1,  407,   -1,
  409,  410,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,  513,  462,   -1,  348,  517,  350,  351,   -1,
   -1,  470,  471,   -1,   -1,  434,   -1,  360,   -1,   -1,
   -1,  364,  481,  482,   -1,  484,   -1,   -1,   -1,  372,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,  483,   -1,  409,   -1,   -1,   -1,
  413,   -1,   -1,  532,   -1,   -1,  419,   -1,   -1,  498,
  499,   -1,  425,   -1,   -1,   42,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  523,   -1,  449,  450,  451,  452,
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
   -1,   -1,   -1,   44,   -1,   -1,   -1,   -1,  435,   -1,
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
   -1,   45,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  317,   -1,  449,  450,
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
   -1,   -1,   -1,   -1,   63,  320,   -1,   -1,  323,   -1,
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
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   58,
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
   -1,  320,   -1,   -1,  323,   58,  450,   -1,  452,   -1,
   63,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,  360,   -1,   -1,   -1,  364,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,
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
  519,  520,  521,  522,   -1,  524,  525,  526,  527,  528,
  529,  530,  531,  532,  267,   -1,   41,   42,   43,   -1,
   45,  274,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   60,   61,   62,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,  311,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,  348,   -1,  350,  351,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,  481,  482,   -1,  484,   -1,   -1,   -1,  372,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,   -1,  532,   -1,   -1,  419,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,  288,   -1,  286,   -1,  292,
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
  403,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  462,  463,   -1,   -1,   -1,   -1,  472,  473,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,  524,  525,  526,  260,  528,  529,  263,  531,
  532,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   58,   -1,   -1,   -1,   -1,   63,  335,   -1,   -1,   -1,
   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,  364,
  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,  325,   -1,  472,  473,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,  524,
  525,  526,  260,  528,  529,  263,  531,  532,   -1,  267,
   -1,  395,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  430,   -1,   -1,   40,
   -1,  435,   43,   -1,   45,   -1,   -1,  441,   -1,   -1,
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
  417,   -1,  419,   -1,   -1,  284,   -1,   -1,  425,   -1,
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
   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,
   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,
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
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   40,   41,   -1,  320,   44,   -1,  323,   59,
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
   41,   -1,  273,   44,  275,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,  297,   -1,  348,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,  393,   -1,   -1,   -1,  348,   -1,  399,
   -1,   -1,   -1,  403,   -1,  393,   -1,   -1,   -1,   -1,
  410,  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,
   -1,  372,  410,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   41,   -1,  433,   44,   -1,   -1,   -1,   -1,   -1,
  428,   -1,  393,   -1,   -1,  433,   -1,   -1,  399,   59,
   -1,   61,  403,   -1,   -1,   -1,   -1,   -1,  458,  410,
   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,  272,
  458,   -1,  275,   -1,  462,   -1,   -1,  428,   -1,  258,
   -1,   91,  433,   -1,   -1,   -1,   -1,  290,   -1,  292,
   -1,   -1,  295,   -1,  273,   -1,   -1,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,   -1,  458,   -1,   -1,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  258,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,  367,   -1,  369,   -1,   -1,  348,
   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  372,   -1,  398,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  415,   -1,  393,   -1,   -1,   -1,   -1,   -1,
  399,   -1,   -1,   -1,  403,   -1,   -1,  348,   -1,   -1,
   -1,  410,   -1,   -1,  437,  260,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  428,
   -1,  372,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,  472,
  473,  281,  393,  298,  284,   -1,   -1,   -1,  399,  458,
   -1,   -1,  403,  462,   -1,  310,  489,  297,   -1,  410,
   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,  334,
   -1,  260,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  348,   -1,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  310,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  399,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  393,  409,  334,   -1,   -1,   -1,  399,
   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  458,   -1,
  399,   -1,  462,   -1,   -1,   -1,  481,  482,   -1,  484,
  409,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,  434,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,  270,
   -1,   -1,   -1,  452,   -1,   -1,  455,  532,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,  298,   -1,   -1,
   -1,   -1,  481,  482,  305,  484,   -1,   -1,  309,  310,
   -1,  312,  491,  492,  315,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  328,   -1,  508,
  509,   -1,   -1,  334,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,
  391,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,
   -1,   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  424,  309,  310,   -1,  312,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  328,  445,   -1,   -1,   -1,   -1,  334,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,  391,  508,  509,  510,
  511,  512,   -1,  514,  515,  516,   -1,  518,  519,  520,
  270,  522,   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,
   -1,  532,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  309,
  310,   -1,  312,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,  328,   -1,
   -1,   -1,   -1,   -1,  334,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,  510,  511,  512,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,  391,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  424,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,  272,   -1,   -1,  275,   -1,
  470,  471,   -1,   -1,  281,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,  292,   -1,   -1,  295,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,  335,   -1,
   -1,   -1,  532,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  367,   -1,  369,  272,   -1,   -1,  275,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
  437,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,  367,   -1,
  369,   -1,   -1,  470,  471,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,  489,   -1,  491,  492,   -1,  494,  495,  398,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  339,
   -1,  508,  509,   -1,   -1,   -1,  415,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,  362,   -1,   -1,  532,   -1,   -1,  437,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,  418,   -1,
  489,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  452,  522,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,  532,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  532,  455,  456,  457,  458,  459,  460,  461,
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
			isc_arg_number, lex.lines,
			isc_arg_number, lex.last_token - lex.line_start + 1,
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
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 602:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 605:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
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
case 948:
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
case 963:
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
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
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
