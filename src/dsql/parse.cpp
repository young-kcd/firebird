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
  103,  104,  104,  108,  108,   81,  111,  111,  116,  113,
  113,  114,  114,  118,  118,  119,  120,  120,  121,  122,
   79,   83,   80,  105,  105,  125,  126,  126,  128,  128,
  129,  129,  129,  129,  129,  127,  127,  130,  130,  131,
  131,  131,  109,  133,  134,  134,  135,  135,  136,  136,
  137,  137,  137,  107,  107,  107,   76,   86,  138,  138,
  138,  139,  139,  140,  140,  141,  141,  141,  146,   97,
   97,  143,  143,  115,  115,  110,  150,  151,   62,  117,
  117,  117,  117,  117,  117,  144,  144,  158,  158,  159,
  161,  161,  161,  161,  161,  142,  160,  160,  164,  164,
  164,  164,  166,  167,  168,  163,  163,  162,  162,  162,
  162,  162,  169,  170,  171,  171,  171,  171,   75,   85,
   90,  176,  172,  172,  173,  173,  177,  177,  179,  178,
  178,  181,  180,  180,  180,  174,  174,  182,  182,  183,
  185,  185,  186,  184,  184,  188,  188,  188,  187,  190,
  190,  175,  192,  192,  192,  193,  193,  191,  191,  195,
  195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
  195,  195,  195,  195,  195,  196,  196,  196,  196,  202,
  202,  203,  199,  209,  210,  200,  207,  207,  204,  214,
  198,  215,  212,  212,  212,  212,  208,  211,  211,  206,
  206,  206,  213,  213,  194,  194,  219,  220,  220,  221,
  221,  221,  221,  205,  205,  205,  223,  225,  224,  226,
   12,  227,  227,  227,  228,  228,  228,   13,  230,  230,
  231,  231,  232,   78,   87,  148,   98,  100,  235,  235,
   77,   91,  236,  236,  236,  237,  240,  240,  241,  241,
  241,  241,  241,  241,  241,  241,  241,  241,  241,  241,
  241,  241,  241,  238,  238,  239,    2,  243,  243,  243,
  243,  243,  243,  243,  243,  243,  249,  249,  253,  253,
  253,  253,  253,  253,  253,  244,  244,  254,  254,  254,
  254,  254,  254,  254,  254,  254,  248,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  256,  256,
  258,  258,  257,  255,  255,  255,  250,  250,  251,  251,
  251,  252,  262,  262,  263,  263,  246,  247,  247,  264,
  264,  264,  264,  264,  245,  265,  265,  266,  266,    9,
  267,  267,  267,  267,  267,  267,  267,  267,  267,  267,
  267,  267,  267,  112,  112,  145,  145,  270,  270,  273,
  273,  275,  275,   57,   57,  272,  272,  272,  272,  272,
  272,  272,  272,  272,  279,  279,  271,  271,  271,  271,
  281,  281,  280,  280,  280,   59,   59,  276,  276,  276,
  274,  274,  274,  285,  285,  285,  284,  284,  283,  283,
  283,  277,  277,  286,  286,  286,  287,  287,  278,  278,
  278,  278,  288,  288,   20,   20,   20,  290,  290,  290,
   18,   18,   18,  292,  293,  296,  296,  294,  298,  298,
    5,   17,  297,  297,  299,  299,  300,  300,  289,  301,
  301,  302,  302,  303,  303,  303,  303,  304,  304,  305,
  305,  306,  306,  308,  308,  308,  309,  309,  309,  310,
  310,  310,  307,  312,  312,  312,  313,  313,  311,  311,
  314,  316,  316,  315,  315,  291,    4,    4,    4,  317,
  319,  319,  319,  319,  319,  319,  319,  319,  319,  319,
  319,  319,  319,  319,  321,  321,  322,  318,  318,   19,
  323,  323,  325,  325,  324,  324,  234,  329,  330,  326,
  326,  326,  331,  333,  342,  343,  344,  345,  334,  334,
  334,  334,  346,  346,  346,  347,  347,  347,  335,  335,
  336,  336,  349,  349,  350,  350,  351,  351,  337,  353,
  353,  354,  354,  356,  356,  356,  358,  359,  359,  360,
  360,  362,  362,  355,  355,  355,  363,  364,  365,  367,
  367,  368,  369,  357,  357,  370,  370,  371,  371,   25,
  366,  366,  366,  366,  366,  372,  372,  339,  339,  373,
  373,  374,  340,  340,  338,  338,  341,  341,  375,  376,
  376,  376,  376,  376,  377,  377,  378,  378,  379,  379,
  380,  380,  380,  381,  381,  382,  382,  327,  327,  383,
  383,  384,   72,   72,   72,  386,  386,  385,  385,  328,
  328,  328,   11,   11,    8,    8,  389,  390,   21,   21,
  392,  393,  388,  388,  388,  391,  394,  394,  197,  201,
    3,    3,  397,  397,  397,  399,  399,  400,  398,  398,
  401,  401,   35,   35,   96,   95,   95,  387,  387,  402,
  403,  403,  217,  217,  217,  147,  395,  395,  123,  123,
  123,  123,  123,  407,  407,  407,  407,  407,  407,  406,
  406,  406,  409,  409,  409,  409,  409,  409,  409,  409,
  409,  409,  409,  410,  410,  410,  410,  410,  410,  410,
  410,  416,  416,  416,  416,  416,  416,  416,  416,  416,
  416,  416,  416,  416,  416,  416,  416,  421,  421,  411,
  411,  412,  412,  413,  413,  413,  413,  414,  414,  418,
  418,  419,  419,  419,  419,  417,  420,  415,  415,  405,
  405,  405,  408,  408,  408,  422,  422,  423,   99,   99,
   99,   99,   99,   99,   99,   99,   99,   99,   99,   99,
   99,   99,   99,   99,   99,   99,   99,   99,   99,   99,
   99,   99,   99,   99,   99,   99,  157,  157,  157,  430,
  430,  424,  229,  229,  152,  152,  431,  431,  431,  431,
  431,  426,  426,  426,  426,  426,  233,  153,  153,  154,
  155,  155,  155,  155,  155,   55,   55,   22,   22,  242,
  432,   58,  282,  261,  261,  106,  425,  425,  425,  433,
  433,  433,  433,  433,  433,  433,  433,  433,  433,  433,
  434,  434,  436,  437,  437,  437,  439,  440,  440,  441,
  435,  435,  435,  435,  442,  444,  444,  443,  443,  443,
  443,  445,  445,  445,  396,  396,  427,  428,  428,  446,
  446,  446,  447,  447,  448,  448,  451,  451,  449,  449,
  454,  454,  453,  450,  452,  429,  429,  438,  438,  438,
  438,  438,  438,  438,  438,  348,  348,  332,  332,  156,
   52,   68,  132,  149,  404,  165,  189,  269,   93,   66,
  222,  124,   73,  352,  218,  320,   36,   45,  361,  268,
   43,   46,  216,   44,  295,  260,  260,  455,  455,  455,
  455,  455,  455,  455,  455,  455,  455,  455,  455,  455,
  455,  455,  455,  455,  455,  455,  455,  455,  455,  455,
  455,  455,  455,  455,  455,  455,  455,  455,
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
    4,    0,    1,    1,    2,    8,    1,    0,    3,    1,
    0,    1,    0,    1,    2,    1,    1,    1,    2,    6,
    1,    1,    3,    0,    1,    1,    0,    1,    1,    2,
    3,    4,    2,    2,    3,    0,    1,    1,    2,    1,
    4,    3,    3,    1,    0,    1,    1,    2,    3,    4,
    0,    1,    2,    0,    1,    1,    5,    5,    3,    2,
    0,    1,    3,    1,    1,    6,    3,    2,    6,    2,
    1,    2,    2,    2,    0,    1,    1,    2,    0,    1,
    1,    1,    1,    1,    1,    0,    1,    1,    2,    2,
    1,    1,    5,    2,    3,    2,    2,    0,    1,    1,
    1,    1,    3,    4,    8,    4,    0,    1,    1,    2,
    2,    0,    3,    3,    1,    2,    2,    2,    8,    8,
    8,    8,    3,    0,    4,    0,    1,    3,    5,    1,
    3,    2,    3,    3,    0,    1,    0,    1,    2,    4,
    1,    1,    3,    1,    0,    2,    2,    0,    6,    1,
    1,    3,    1,    2,    0,    1,    2,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    3,    1,    3,    8,    9,    5,    8,    6,    3,    0,
    3,    2,    1,    1,    3,    3,    7,    2,    0,    1,
    1,    2,    3,    0,    1,    2,    4,    1,    3,    2,
    2,    2,    1,    1,    1,    1,    2,    2,    5,    0,
    5,    1,    3,    0,    2,    4,    0,    7,    3,    0,
    1,    3,    3,    7,    7,    0,    0,    0,    3,    0,
    8,    8,    1,    1,    0,    2,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    5,    5,    5,
    5,    5,    5,    2,    0,    4,    2,    2,    3,    2,
    2,    3,    3,    2,    2,    3,    1,    2,    3,    3,
    2,    2,    2,    2,    3,    1,    3,    3,    3,    2,
    2,    4,    4,    4,    5,    4,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    1,    1,    1,    1,    1,    0,    2,    2,    4,    4,
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
    1,    1,    3,    1,    1,    1,    1,    2,    2,    3,
    0,    4,    1,    3,    1,    2,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    1,    2,    1,
    1,    1,    1,    1,    2,    1,    1,    1,    1,    4,
    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,
    1,    1,    6,    1,    1,    1,    4,    4,    4,    4,
    1,    1,    4,    4,    7,    2,    0,    7,    6,    6,
    4,    1,    1,    1,    4,    3,    6,    1,    1,    6,
    8,    6,    1,    1,    4,    6,    4,    5,    3,    5,
    4,    5,    1,    1,    1,    4,    6,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    0,    1,    0,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,
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
    0,  103,    0,  980,    0,   23,  978,    0,    0,   74,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  510,    0,    0,    0,   37,   34,
   36,    0,   35,    0, 1086, 1088, 1089, 1090, 1091, 1092,
 1093, 1112, 1113, 1094, 1096, 1095, 1101, 1097, 1102, 1098,
 1100, 1103, 1099, 1104, 1105, 1106, 1108, 1109, 1107, 1110,
 1111, 1115, 1114, 1116, 1117, 1118,    0,    0,    0,    0,
   32,   69,   67, 1078, 1087,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  691,    0,    0,
    0,  738, 1080,    0,    0,    0,    0,    0,    0,  116,
 1085,  594,    0,    0,    2,    0,    0,    0,    0,    0,
    0,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  452,  453,  454,  455,  456,  457,
  458,  459,  467,  461,  462,  460,  463,  464,  465,  466,
  468,  470,  473,  469,  471,  472,  474,  475,  476,  477,
  478,    0,  437,  438,    0,  408, 1069,    0,    0,  414,
 1073,    0,  411, 1077,    0,  740,    0,  410, 1081,    0,
  415, 1072,    0,  601,  976,    0,  166,  112,    0,  113,
    0,  216, 1065,  836,  104,    0,  110,  161,    0,    0,
  106,    0,  114,  982,    0,  107,    0,  108,    0,  109,
 1084,  162,  115,  111,  783,  784,    0,  981,  979,    0,
   75,    0, 1070,    0, 1068,  518,  511,    0,  517,  522,
  512,  513,  520,  514,  515,  516,  521,  523,    0,    0,
    0,   30,    0,   39,  823,   38,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  602,  986,    0,  967,  685,  685,  686,  696,  698, 1056,
  699,    0,  700,  690,  686,    0,    0,    0,    0,    0,
    0,    0,  620,  609,    0,  612,  614,  615,  616,  617,
  623,  624, 1079,  739,    0,  120,    0,    0,  117,    0,
  118,    0,  119,    0,    0,  650,  651,  658,    0,  657,
  659,  661,  666,  654,  652,  655,  653,  662,  665,  660,
  664,    0,    0,    0,    0,    0,  670,    0,    0, 1058,
    0,    0,  677,    0,    0,    0,    0,    0,  498,    0,
    0,    0,    0,  219,    0,  417,  128, 1061,    0,  416,
  487,  488,    0,    0,    0,    0,    0,    0,  426,    0,
  383,  384,    0,    0,  607,  605,  977,    0,    0,    0,
    0,    0,    0,    0,  169,  147,    0,  125,    0,    0,
    0,  121,    0,  136,  135,    0,    0,    0,    0,    0,
    0,   76,    0,    0,    0,    0,  798,  519,    0,    0,
    0,    0,  942,  973,    0, 1060,    0,  944,    0,    0,
  974,    0,  968,  959,  958,  957,    0,    0,    0,    0,
  947,    0,    0,  960,  961,  969,  970,    0,  971,  972,
    0,    0,    0,  975,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  963,    0,  833,
  939,  940,  941,  946,  945,  924,    0,    0,    0,  923,
    0,    0,  928,  920,  921,  922,  925,  926,  927,  962,
  987,  988,  989, 1001, 1002, 1004, 1005, 1006, 1011, 1012,
 1028, 1029, 1033, 1034,  217,    0,    0,    0,  371,    0,
    0,    0,  826,   27,    0,    0,    0,    0,   72,   70,
 1082,   68,   33,    0,    0,    0,  828,    0,   48,   50,
    0,    0,    0,  599,    0,    0,    0,  693,  695,    0,
  702,    0,    0,    0,  703,  689,    0,  646,    0,  621,
    0,  618,    0,  619,  644,    0,  639,    0,    0,  613,
  837,  807,    0,    0,    0,  126,    0,    0,    0,  596,
  595,  663,  656,    0, 1076,    0,    0,    0,  671,  675,
    0,    0,  780,  682,  681,    0,  184,    0,    0,  144,
    0,  503,    0,  504,  499,  377,    0,  421,  423,  422,
  377,  378,  424,    0,  418,    0,    0,    0,    0,  267,
    0,    0,    0,  430,  431,    0,  480,    0,    0,    0,
    0,    0,    0,    0,  388,  387,  506,    0,    0,    0,
  165,    0,    0,  174,    0,  173,    0,    0,  180,  163,
    0,  178,  170,    0,    0,  542,  577,  545,    0,  578,
    0,    0,  546,    0,    0,    0,  569,  581,  541,  564,
  543,  544,  539,  526,  377,  524,  525,  527,    0,    0,
  536,  537,  538,  540,    0,    0,    0,    0,  124,    0,
    0,  122,    0,  123,    0,  139,    0,    0,  200,    0,
    0,  376,    0,  219,    0,    0,   84,    0,  102,    0,
  101, 1062,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  839,  841,  850,  853,  854,  855,  856,  857,
  858,  859,  860,  861,  862,  863,    0,    0,    0,    0,
    0,  964,    0,    0,    0,    0,    0,    0,  965,  966,
    0,  948,  949,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  332, 1083,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  361,    0,    0,  272,  534,
    0,    0,    0,  369,    0,    0,    0,  825,    0,    0,
   44,   73,    0,    0,   26,    0,  831,    0,    0,  794,
    0,    0,    0,    0,  598,    0,  685,    0,  707,    0,
    0,    0,    0,  589,    0,  984,  588,    0,  622,    0,
  630,  626,  625,    0,    0,    0,  641,  629,    0,    0,
    0,  802,    0,    0,    0,  376,  669,  668,  647,    0,
    0,    0,    0,    0,    0,    0,    0,  145,    0,  502,
    0,  420,    0,  419,  425,  493,    0,  492,  377,    0,
  263,  376,  237, 1066,    0,    0,  377,    0,    0,  208,
  376,    0,    0,    0,  242,  236,  239,  240,  241,  429,
  485,  484,  428,  427,    0,    0,    0,    0,    0,    0,
    0,    0,  386,    0,  377,  490,    0,  489,    0,  171,
  175,    0,    0,  179,    0,    0,    0,  565,  566,  582,
    0,  579,    0,    0,  572,  570,  571,    0,    0,    0,
    0,  535,    0,    0,    0,    0,  573,  127,    0,    0,
  376,    0,  199,    0,  202,  204,  205,    0,    0,    0,
    0,    0,  219,   79,    0,    0,   78,    0,    0,    0,
    0,  840,  852,    0,    0,  913,  914,  915,    0,  844,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  760,
  762,    0,  757,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1048,
 1049, 1050, 1051, 1052, 1053, 1054, 1055,    0,    0,    0,
    0,    0,    0,    0, 1039,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1022, 1024, 1023,    0,    0,
  937,  363,    0,  938, 1026,    0,  933, 1064,    0,    0,
    0,    0,    0,    0,    0,    0,  333,  334,    0,    0,
  943,  835,  834,  373,  372,    0,  270,    0,    0,    0,
  278,  827,    0,    0,    0,   71,    0,    0,    0,  830,
    0,    0,    0,    0,  694,  697, 1074,  706,    0,    0,
    0,    0,  712,  713,  714,  715,  724,  725,  726,    0,
    0,  704,  590,  985,  631,  640,  635,  634,    0,  645,
  838,  808,    0,    0,  376,    0,    0,  648,  667,    0,
  687,  782,  781,    0,  501,    0,    0,  183,    0,  187,
    0,    0,  149,  220,  221,  222,  223,  224,  225,  955,
  495,    0,  268,    0,  210,  377,  212,  378,  150,  207,
  213,    0,    0,    0,  432,  433,  436,  378,  481,  482,
  434,    0,    0,    0,  404,    0,  491,  195,  196,  172,
    0,  182,  553,  554,  983,    0,    0,    0,    0,    0,
  580,    0,    0,  378,    0,    0,  530,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  197,
    0,    0,    0,   83,    0,    0,    0,    0,   91,   85,
    0,    0,   90,   86,    0,  806, 1067,    0,    0,  845,
    0,    0,  851,    0,    0,    0,    0,    0,  889,  888,
    0,    0,    0,  898,  916,    0,  908,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  843,    0,  761,
    0,  797,    0,    0,  219,    0,  990,    0,    0, 1013,
    0,    0,    0,    0,    0,    0,    0,  950,    0,    0,
    0,    0,    0, 1035,    0,    0,    0,    0,    0,    0,
    0, 1046, 1007, 1008, 1009, 1014, 1010,    0, 1021,    0,
    0,  679, 1025,  952,    0,    0,    0,    0,  265,  284,
    0,    0,  368,  279,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   54,   62,   64,    0,   42,    0,    0,
    0,  812,  832,    0,  811,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  741,    0,    0,    0,    0,    0,
    0,    0,  637,  638,  642,    0,    0,  198,    0,  649,
    0,    0,    0,    0,  188,    0,  956,  377,  377,  378,
    0,    0,    0,    0,    0,    0,  243,  435,    0,    0,
    0,    0,    0,    0,  377,  508,  378,  181, 1063,    0,
    0,  548,    0,  547,  583,    0,  575,    0,    0,    0,
  528,  556,    0,    0,  558,  561,  563,  376,    0,    0,
    0,  134,    0,  203,    0,    0,  378,  105,    0,  132,
    0,   94,    0,    0,    0,    0,   88,   89,    0,  906,
  907,  847,    0,  849,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  909,    0,    0,    0,    0,    0,    0,
    0,  899,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  768,    0,    0,
    0,  996,  995,    0,    0,  992,  991,    0, 1000,  999,
  998,  997,  994,  993,    0,    0, 1041,    0,    0,    0,
 1040,    0,    0,    0,    0,    0,    0,    0,  366,  336,
  335,  271,    0,    0,  281,  282,    0,    0,    0,    0,
  315,    0,    0,    0,  314,    0,    0,    0,    0,  360,
  303,  301,  305,    0,  302,  291,  296,  290,    0,    0,
    0,  299,  300,  304,  306,  307,  308,  309,  310,  311,
  312,  313,  316,  317,  318,  319,    0,    0,  354,  355,
  356,    0,  810,   66,   58,   59,   65,   60,   61,    0,
   24,    0,    0,   25,  818,  817,    0,  816,  814,  822,
  821,  819,    0,   45,   46,    0,  716,    0,    0,    0,
  746,  744,  742,    0,  743,    0,    0,    0,    0,    0,
  801,    0,  378,  786,  787,  688,    0,  193,  189,  378,
    0,    0,  269,  378,    0,    0,    0,  228,    0,    0,
  244,    0,    0,    0,    0,    0,    0,    0,    0,  505,
  550,    0,  551,    0,    0,    0,    0,  154,  156,  157,
  158,  533,  531,  529,  560,    0,    0,  378,    0,  378,
    0,  374,  377,   92,    0,    0,   98,   93,    0,    0,
    0,    0,    0,  917,    0,  918,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  770,  759,    0,    0,
  771,    0,  767,  218, 1027, 1047, 1003,    0,    0, 1037,
 1036,    0, 1042, 1030, 1032,    0, 1019, 1020,    0,    0,
  280,    0,    0,    0,    0,    0,  342, 1075,  357,  358,
    0,    0,  292,    0,  297,    0,  345,  298,    0,    0,
  338,    0,   55,   56,   57,   63,    0,    0,  793,    0,
  736,  734,    0,  727,    0,    0,    0,  750,    0,  684,
  378,  375,  788,  190,  160,  273,  274,  262,    0,    0,
  206,  229,    0,    0,    0,  231,  232,  230,    0,    0,
  402,  403,  399,  398,  401,  400,    0,  549,  576,  159,
  146,  155,    0,    0,  259,  141,  381,  379,    0,    0,
   96,    0,   80,    0,  875,  883,  678,    0,  876,  884,
  879,  887,    0,    0,  877,  885,  878,  886,  872,  880,
  873,  881,  874,  882,  766,    0,    0,    0, 1015, 1038,
    0, 1018,    0,    0,  283,    0,    0,    0,    0,  329,
    0,    0,  353,    0,    0,    0,    0,  348,  346,    0,
    0,    0,   40,  813,    0,    0,  718,  728,    0,    0,
  729,  730,  731,    0,  260,  209,  214,    0,    0,  234,
    0,  246,  406,  378,  378,    0,   77,   99,  100,    0,
    0,    0,  773, 1031,  286,  287,    0,    0,    0,    0,
  352,  351, 1071,  350,    0,    0,    0,    0,    0,    0,
    0,  717,    0,    0,  751,  235,    0,    0,    0,    0,
    0,  261,  382,    0,    0,  772,    0,    0,    0,    0,
    0,  347,  349,    0,    0,    0,    0,  722,    0,    0,
    0,    0,    0,  245,    0,  251,    0,  250,  133,  775,
    0,  289,    0,    0,    0,    0,    0,    0,  720,  733,
  233,    0,    0,  255,  254,  253,  776,    0,    0,    0,
    0,  337,  723,  258,  256,  257,  327,    0,  343,  324,
  325,
};
static short yydgoto[] = {                                      21,
   22,   23,   24,   25,   26,   27,   28, 1571,   30,   31,
 1572, 1573,   34,   35,   36,   37,   38,   39, 1574,   41,
 1575, 1615,  147,  565,  172, 1371, 1611,  148,  840,  149,
  568,  845,  150,  151,  314, 1130,  161,  162, 1372, 1373,
 1374, 1375,  247,  279,  152, 1376,  153,  570,   90,  291,
  462,  517,  745, 1247,  518,  996,  714,  272,  972,  746,
  747,  748, 1254, 1249, 1698,  292,  750,  751,   82,  265,
   83,  287, 1910, 1468,  271,  276,  278,  280,  267,  258,
  260,  273,  283,  180,  369,  371,  373,  366,  452,  732,
  734,  729,  235,  236,  562,  315,  916,  901,  761,  904,
  456,  737, 1238, 1462,  682,  866, 1210,  639,  640,  663,
  447,  715, 1188, 1676, 1801, 1189, 1173, 1677, 1678, 1679,
 1680, 1681,  762,  268,  259,  443,  690,  444,  445,  691,
  692, 1438,  641, 1168, 1169, 1170, 1414,  458,  984,  985,
  986,  987,  918, 1656,  716,  920,  520,  989, 1087,  556,
 1525, 1174,  521,  522,  523,  524,  525, 1657, 1658,  666,
 1808, 1939, 1427,  926,  913,  927,  928,  929, 1940, 1941,
 1985,  424,  561, 1109, 1576,  243,  659, 1106,  660, 1420,
  557, 1110, 1111, 1361, 1554, 1555, 1556, 1865, 1256, 1577,
 1578, 1579, 1580, 1766, 1581, 1582, 1583, 1584, 1585, 1586,
 1587, 1588, 1589, 1590, 1591, 1592, 1593, 1594, 1595, 1596,
 1597, 1096, 1976, 1870,  526,  815,  527, 1598, 1767, 1877,
 1878, 1922, 1599, 1600, 1601, 1761,  528,  826,  529,  311,
  558,  559,  530,   43, 1467,  433,  677,  945, 1436,  678,
  943,   87,   66,  428,  248,  191,  408,  232,  415,  240,
  251,  420,  416,  429,  933,  430,  673, 1201,  233,  531,
 1225,  657,  908,  409,  679, 1437,  105,  246,  296,  717,
  718,  830, 1226,  831, 1227,  721,  722,  723,  724,  957,
 1219, 1217,  725,  726,  727,  965,  728,  962,   44,   45,
   46,   47,   48,   49,  182,  621,   68,  585,  254,  436,
  354,  355,  356,  357,  358,  359,  360,  361,  362,  872,
  606, 1149, 1405,  607,  608,  877,  392,  889,  393,  626,
  394,  892,  187,  397,  629,   50,  190,  403, 1491,  814,
   51,  401,   52,  166,  342,  593,  862,  466, 1402, 1640,
  778,  588,  344, 1411, 1793,  167,  168,  343,  594,  595,
  860, 1128, 1131, 1132, 1133, 1134, 1135, 1136, 1886, 1932,
  532, 1959, 1137, 1138, 1139, 1400, 1891, 1892, 1893, 1392,
  175, 1632, 1787, 1788, 1518, 1034, 1519, 1520, 1521, 1733,
 1911, 1913,  632,  633, 1162, 1646,  576, 1779,   53,   54,
  467,   55,   56,  614,  615,  533, 1118, 1382, 1617, 1618,
 1622,  577,  848,  264,  763,  764, 1011, 1012,  765,  766,
  767,  768,  769,  770,  771,  772,  773,  774,  775,  776,
 1272, 1274, 1275,  534,  535,  536,  537,  538,  539,  792,
  540,  289,  541,  542,  543,  544,  545, 1058,  546,  547,
  548,  549,  550, 1739, 1080,  551,  552,  553,  554,  797,
 1063, 1337, 1332,  798,  155,
};
static short yysindex[] = {                                  16514,
 1212, -113, 3436,  100, 1099,  409, 3282,  428,16707,  -15,
  194,   33, -113, 1199,  742,12065, 1011,12065,  237,  368,
    0,  714,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  540,    0,    0,    0,    0,    0,    0,  721,
    0,    0,    0,    0,    0,    0,    0,17650,12065,  541,
12065,12065,12065,12065,12065,    0,    0,  552, 1356,12065,
12065,12065,  706,12065,  587,12065,12065,    0,12065,12065,
12065,    0,  851,    0,  633,    0,    0,  815,12065,    0,
12065,12065,12065,  872,12065,12065,12065,12065,  587,12065,
12065,12065,12065,12065,    0,12065, 1129,  829,    0,    0,
    0, 1202,    0, 1202,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  917,  924,  153, 1294,
    0,    0,    0,    0,    0,12065,12065,12065,  991, 1001,
 1363,12065,  523,  192,  221,  623,  897,    0,12065, 1088,
 1078,    0,    0,12065, 1016,12065,12065,12065,12065,    0,
    0,    0,12065, 7622,    0, 1009, 1035, 1184,  857,  950,
 1314,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  785,    0,    0, 1356,    0,    0, 1047,   43,    0,
    0, 1524,    0,    0,  959,    0,  585,    0,    0, 1068,
    0,    0, 1158,    0,    0, 1163,    0,    0, 1114,    0,
 1370,    0,    0,    0,    0, 1356,    0,    0,  -35, 1524,
    0,   64,    0,    0, 1339,    0, 1344,    0, 1202,    0,
    0,    0,    0,    0,    0,    0, 1357,    0,    0, 1047,
    0, 1366,    0, 1232,    0,    0,    0, 1047,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,15555,12065,
 1306,    0,12065,    0,    0,    0, 1286, 1319,11798,12065,
 1110, 1380, 1686, 1411, 1442, 1451, 1399, 1406,  111, 1304,
    0,    0,15811,    0,    0,    0,    0,    0,    0,    0,
    0,13507,    0,    0,    0, 1376,12065, 1428, 1364,  735,
12065, 1395,    0,    0, 1078,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12065,    0, 1356, 1524,    0, 1339,
    0, 1202,    0, 1436, 1408,    0,    0,    0, 1514,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1503,12065,12065, 1481, 1362,    0,15811, 1455,    0,
 1455,15811,    0, -108, 1361, 1369, 1365, 1314,    0,  819,
   61, 1574,12065,    0,  785,    0,    0,    0, 1575,    0,
    0,    0,12065, 1306,12600, 1419,12863, 1838,    0,17650,
    0,    0,  764, 1439,    0,    0,    0, 1826, 1826, 1356,
 1525, 1356,  455, 1114,    0,    0,11119,    0,12065,12065,
12065,    0, 1306,    0,    0, 1607,  682, 1855,12065, 1638,
12065,    0, 1861, 9973, 9606, 1515,    0,    0, 1863, 1868,
 1870, 1462,    0,    0, 1872,    0, 1880,    0, 1884, 1891,
    0, 1892,    0,    0,    0,    0,    0, 1894, 1476, 1482,
    0, 1899, 1899,    0,    0,    0,    0, 1903,    0,    0,
13763, 1909, 1913,    0, 1916, 1601, 1920, 1921, 1922, 1934,
 1938, 1940,15811,15811,14019,12065, 1944,    0, 1978,    0,
    0,    0,    0,    0,    0,    0, 1879, 1583, 1947,    0,
    0, 1948,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,11119, 1945,  284,    0, 1967,
 1731,  310,    0,    0,12065,12065,12065,  155,    0,    0,
    0,    0,    0,12065,12065,  801,    0,12065,    0,    0,
 1286, 1613,11798,    0,12065,14019, 1794,    0,    0,15811,
    0, 1851, 1693, 1971,    0,    0,  213,    0,  789,    0,
  -32,    0,  -32,    0,    0, 1973,    0,  225, 1593,    0,
    0,    0, 1972,  131, 1963,    0, 1306, 1986, 1763,    0,
    0,    0,    0,  919,    0, 1701, 1996,12065,    0,    0,
 1969, 1995,    0,    0,    0, 1387,    0, 1700, 1702,    0,
 1356,    0, 1709,    0,    0,    0, 1785,    0,    0,    0,
    0,    0,    0,11119,    0, 1356, 1697,11119, 1170,    0,
 1799,12065,17405,    0,    0,  816,    0,12065, 1005,  959,
 1674, 1641,  -63,    0,    0,    0,    0,   37, 1676,  219,
    0, 1623,  587,    0, 1356,    0, 1800, 1734,    0,    0,
  455,    0,    0,  124, 1650,    0,    0,    0, 1655,    0,
 1689, 2055,    0, 1751, 2057,  598,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2008,  103,
    0,    0,    0,    0,  114, 2063, 2065, 2057,    0, 1356,
 1524,    0, 1791,    0, 1847,    0, 1356, 1356,    0,12600,
  585,    0, 1737,    0, 1713, 2073,    0, 5334,    0, 1739,
    0,    0, 1743, 2084, 9825, 2085,    0,    0,    0, 9330,
10873,  543,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  559, 1746, 1044,15811,
  687,    0,12065,15811, 1087, 1188, 1210, 2183,    0,    0,
  587,    0,    0,15811,10099, 1978, 1684,  644,15811,15811,
10099, 1814,15811,15811,15811,15811,15811,10318,  845,  845,
 1864, 1491,  721, 2091,    0,    0,14275,12065,15811,15811,
15811,15811,15811,15811, 1992,    0,15811, 6761,    0,    0,
 1859, 2072,12065,    0,12065, 1843,12065,    0, 1717, 1719,
    0,    0, 1698,11798,    0, 1828,    0, 1502, 2109,    0,
 1828,12065,12065, 2108,    0, 1864,    0, 1930,    0,12065,
 6970, 1716,15811,    0, 1125,    0,    0,  -28,    0, 1718,
    0,    0,    0,12065,  922,12065,    0,    0,12065,12065,
 1515,    0,15811, 1900,12600,    0,    0,    0,    0,  919,
12065, 1830, 2124, 1675,15811,15811, 1356,    0,  489,    0,
 2131,    0, 4768,    0,    0,    0, 1356,    0,    0,12065,
    0,    0,    0,    0, 1901, 2135,    0, 1574, 1897,    0,
    0, 1839, 1840, 1202,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  587,12065, 1886, 1574,17503, 1802,
 1811, 1812,    0,  587,    0,    0, 1128,    0, 1415,    0,
    0, 1782, 1356,    0, 9973,   94, 1787,    0,    0,    0,
  587,    0, 2055,  105,    0,    0,    0, 1574,  105, 1786,
  105,    0, 2158,  587,  587,  587,    0,    0, 1306,12065,
    0, 1848,    0, 1551,    0,    0,    0,  764, 1455,12065,
 1591,  174,    0,    0, 2159,   21,    0, 9973,12065, 1455,
10589,    0,    0, 1455, 9825,    0,    0,    0,10817,    0,
  167,  554,15811,15811,10856, 2161, 1055,11123,15811,11390,
 1297,11657,11924,14531,12191,12458,12725,10099,10099,    0,
    0, 1841,    0, 2165,  950,15811,15811, 2687,15811, 2167,
15811, 2162, 2291,15811,15811,15811,15811,15811,15811,    0,
    0,    0,    0,    0,    0,    0,    0, 1896, 2168,  734,
    8,15811,  694,15811,    0,10099, 1955, 2474,  452,12065,
 2353, 2387, 2414, 2517, 2547,    0,    0,    0, 1602,14787,
    0,    0,  950,    0,    0, 1609,    0,    0, 1937,  845,
  845,  758,  758,  825,10203, 2180,    0,    0, 2179, 1978,
    0,    0,    0,    0,    0, 1626,    0, 1790, 1961, 1843,
    0,    0,11531,11531, 1858,    0,  -37, 1878,12065,    0,
15811, 1878, 1927, 1931,    0,    0,    0,    0, 6524, 2193,
 2203,  850,    0,    0,    0,    0,    0,    0,    0,10099,
 1924,    0,    0,    0,    0,    0,    0,    0, -130,    0,
    0,    0, 1746, 1978,    0, 1630, 1455,    0,    0,  919,
    0,    0,    0, 1978,    0, 1826, 1984,    0,  489,    0,
10099, 1256,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  365,    0, 1843,    0,    0,    0,    0,    0,    0,
    0, 1202, 1202, 1759,    0,    0,    0,    0,    0,    0,
    0,  -71,   19,  403,    0, 1994,    0,    0,    0,    0,
12065,    0,    0,    0,    0,  100, 1642, 1842, 1859, 2216,
    0, 1623, 1658,    0, 2201, 1077,    0,12065, 1138,  587,
 2221, 2222, 2230, 2009,  585, 1843, 1826, 1702,12600,    0,
 1676, 1833, 9419,    0,  587, 1885, 1975, 5334,    0,    0,
  587, -141,    0,    0, 1983,    0,    0, 2249, 2252,    0,
10099,10099,    0,10099,10099, 2535, 1978, 2254,    0,    0,
 1978, 2264,14019,    0,    0, 1999,    0,  407, 2277, 1978,
 2279, 1668, 2280, 1978, 2281,15811,15811, 2161,15811,15043,
 2283, 1978, 2289, 2297, 1978, 2300,15811, 1978, 2302, 1978,
 2304, 2306, 1978, 2307, 2309, 1978, 2310,    0, 2069,    0,
17572,    0, 2637, 2672,    0, 2697,    0, 2761,15811,    0,
 2835, 2847, 2890, 2988, 2999, 3050,15811,    0,15811,15811,
 1978, 1925,15811,    0,15811, 1978, 2048,  549,15811,15811,
15811,    0,    0,    0,    0,    0,    0,15811,    0,15811,
 1646,    0,    0,    0, 1681,10203, 9490,12065,    0,    0,
12065,17212,    0,    0,12065,12065,12065,12065,12065,12065,
 1908, 2311, 2312,    0,    0,    0, 1908,    0,   95,   95,
   73,    0,    0, 1683,    0,11531,11531, 2320,  850, 2321,
15811, 2099, 6970, 1987,    0, 1987,  637, 1987, 2028, 2031,
 2098, 2046,    0,    0,    0,  950, 1843,    0, 1833,    0,
  751, 1623, 1990, 1623,    0,  441,    0,    0,    0,    0,
 1961,15811, 2092, 1979, 1759,  851,    0,    0, 1998, 2002,
 2003, 2004, 2005, 2006,    0,    0,    0,    0,    0, 2348,
  100,    0, 1941,    0,    0,  105,    0,   22,  105,  105,
    0,    0, 1859, 2349,    0,    0,    0,    0,  764, 1961,
 1623,    0, 1702,    0, 1994, 2117,    0,    0, 2124,    0,
 2357,    0, 2358, 1356,  -38, 2361,    0,    0, 1356,    0,
    0,    0, 2069,    0, 2069,15811, 1455, 1455, 1691,  721,
 2362,15811, 2087,    0, 1455, 1455,15811, 1455, 1455, 3296,
 1978,    0, 1912,15811, 1978, 1455, 1455, 1455, 1455, 1978,
 1455, 1455, 1455, 1455, 1455, 1455,12065,    0, 2364, 2363,
 1193,    0,    0,11119, 2365,    0,    0, 3058,    0,    0,
    0,    0,    0,    0, 3135, 1808,    0,15811, 2103, 1988,
    0,15811, 3147, 1696, 3009, 3190, 3237,15811,    0,    0,
    0,    0,11119, 2351,    0,    0, 2121,    0,12065, -112,
    0, 2374, 2080,15811,    0,    0,12065,12065,12065,    0,
    0,    0,    0, 2082,    0,    0,    0,    0, 2116,17077,
 2366,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  426, 2373,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2111,
    0,11531,11531,    0,    0,    0, 2011,    0,    0,    0,
    0,    0, 1904,    0,    0, 2099,    0, 1714,12065,  850,
    0,    0,    0, 2094,    0, 6970, 6970,15811,10099, 1515,
    0, 1961,    0,    0,    0,    0, 1415,    0,    0,    0,
 4768, 4768,    0,    0, 1978, 2160, 2092,    0, 1043,12065,
    0, 2110, 2007, 2112, 2014, 2144, 2119, 2147, 1843,    0,
    0, 2417,    0, 2425, 2120, 2160,   22,    0,    0,    0,
    0,    0,    0,    0,    0, 1843, 1676,    0, 1415,    0,
 2093,    0,    0,    0, 2104,   10,    0,    0, 1859, 2105,
 1978, 2431, 2434,    0,  950,    0, 1978,15811, 2437, 2439,
 1978, 2440, 2444,15811,15811, 1978, 2447, 2449, 2450, 2456,
 2457, 2458, 2461, 2462, 2463, 2468,    0,    0,17572, 2473,
    0,12065,    0,    0,    0,    0,    0,15811, 2479,    0,
    0,15811,    0,    0,    0,15811,    0,    0, 3350,  673,
    0, 2207,15811,15811,10099, 1978,    0,    0,    0,    0,
12065,10203,    0,  643,    0, 2089,    0,    0,  -29, 2483,
    0, 2149,    0,    0,    0,    0,   95,15811,    0,12065,
    0,    0, 6970,    0, 2012, 1978, 2481,    0,  543,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2492,12065,
    0,    0, 2194,12065, 1759,    0,    0,    0, 1202,12065,
    0,    0,    0,    0,    0,    0, 1961,    0,    0,    0,
    0,    0, 1961, 1994,    0,    0,    0,    0,15811, 1356,
    0, 2077,    0, 1356,    0,    0,    0, 1978,    0,    0,
    0,    0, 1978, 1978,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,12065, 2205, 1978,    0,    0,
 3358,    0, 4768, 4768,    0, 2499, 1978, 1615,  471,    0,
 2204, 2180,    0,12065,12065,  100,  320,    0,    0, 2041,
 2208,10099,    0,    0,  147, 2505,    0,    0,10099, 2507,
    0,    0,    0,15811,    0,    0,    0, 1759, 1202,    0,
 2173,    0,    0,    0,    0, 1978,    0,    0,    0, 2508,
 2512, 2514,    0,    0,    0,    0, 1455,10203, 2128,10203,
    0,    0,    0,    0,17212,  643,15811,10203,  521,10203,
12065,    0,  543,12065,    0,    0, 2173,   28, 1759, 2185,
 2187,    0,    0, 2515,12065,    0,12065, 2526, 2180,17212,
 2180,    0,    0, 1825,  169, 2268, 2180,    0, 1724, 1735,
 1759,  737,  737,    0, 2274,    0, 2141,    0,    0,    0,
 2538,    0, 2282,10203, 2295, 2298,17212,12065,    0,    0,
    0, 2145,  697,    0,    0,    0,    0,17212,  524,12065,
17212,    0,    0,    0,    0,    0,    0,17212,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  228,  713,    0,    0,    0,    0,    0,    0,    0,
    0,16783,  296,12988,    0,    0,    0,    0,    0,    0,
    0, 2596,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  178,    0,    0,    0,    0,    0,    0,  157,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  771,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2271,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   27, 2238,    0,    0,
    0,  132,    0,  132,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2239,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  771,    0,    0,15299,13251,    0,    0,    0,
  875,    0,    0,  120,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  410,    0, 2197,  282,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   31,    0,    0,    0,    0,  716,    0,    0,    0,
    0,    0,  969,    0,    0,    0,    0,    0,  141,    0,
11386,    0,    0,    0,    0,    0,    0,    0,    0,   31,
    0,  578,    0,    0, 2577,    0,    0,    0, 2355,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  121,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  146,    0,
 2360,    0,    0,    0,    0,    0,12332,    0,    0,    0,
    0,    0, 1178,    0,    0,    0,    0,    0,    0,13125,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  780,    0,    0, 1017,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   31,    0, 2577,
    0, 2355,    0, 1022,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  307,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1038,    0,    0,
    0,    0,    0,    0, 1167,    0,    0,    0,  162,    0,
    0,    0,    0, 2360, 1006,17731,    0, 1225,    0,    0,
    0,    0,  183,    0,    0,    0,    0, 2172, 2172,    0,
    0,    0, 1278,  159,    0,    0,    0,    0,    0,    0,
    0,    0, 2360,    0,    0, 1378,    0,    0,    0,    0,
    0,    0, 9268,    0,    0,  151,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2266,    0,    0,    0,
    0, 1783, 1783,    0,    0,    0,    0, 2866,    0,    0,
    0, 3166, 3466,    0, 3766, 4066,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  356,    0,
    0,    0,    0,    0,    0,    0, 4366, 1292,  285,    0,
 2566,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1309,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12332,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 9709,    0, 2314,    0,    0,    0,    0,    0,    0,
  993,    0,  993,    0,    0,  965,    0, 1025, 1115,    0,
    0,    0,    0,  121,    0,    0, 2360,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  227,  161,    0,    0,    0,  470,    0,    0,  849,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1329,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  308,    0,
  -14,    0,    0,  788,    0,    0,    0,    0,  739,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1335,    0,    0, 2557,16332,    0,    0,    0,16344,    0,
    0,16553,    0,    0,16611,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1889, 2143,
    0,    0,    0,    0,16722,16381,    0,16611,    0,    0,
   31,    0,    0,    0,    0,    0,    0,    0,    0, 1006,
 1054,    0,    0,    0,    0, 2226,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 7356, 7553, 7705,    0,
    0,  432,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2592,  118,16067,    0,
16067,    0,    0,    0,16067,16067,16067,    0,    0,    0,
    0,    0,    0,    0,    0, 2190,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 5555, 5844,
 2593,    0,   74,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 3719,    0,    0,    0,    0, 2371,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  233,    0,    0,    0,    0,
  233,    0,    0, 1346,    0,    0,    0,    0,    0,    0,
    0, 1039,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  666,    0,    0,    0,    0,    0,
  151,    0,    0,    0, 1006,    0,    0,    0,    0,    0,
    0,    0,  474,  144,    0,    0,    0,    0,  457,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2603,    0,    0, 1946, 2423,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  256,
  377,  427,    0,    0,    0,    0,    0,    0,  988,    0,
    0,    0,    0,    0,    0,    0, 2857,    0,    0,    0,
    0,    0,16553,    0,    0,    0,    0,  899,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2360,    0,
    0,  405,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 5623,    0,    0,    0,  292,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2604,    0,    0, 1391,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2615,    0,    0,    0,    0,    0, 4666, 6133,
 6422, 4966, 5266,    0,    0, 1392,    0,    0,    0,  402,
    0,    0,    0,    0,    0,    0,    0,13381,    0, 2391,
    0,    0,    0,    0,    0,    0,    0, 1420,    0,    0,
    0, 1420,    0,    0,    0,    0,    0,    0,    0, 6631,
  152, 1341,    0,    0,    0,    0,    0,    0,    0,    0,
  324,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  118,  171,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  677,    0, 2172, 2220,    0,  635,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1736,    0, 2371,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1280,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1445,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3157,    0,
    0,    0,    0,    0, 1146,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1054, 2371, 2172, 1446, 1006,    0,
 2408, 1459,    0,    0,    0, 5623,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1704,    0,    0,    0,
 2033,    0,    0,    0,    0,    0,    0,    0,    0, 6343,
    0, 7803,    0, 7902,    0,    0,    0,    0,    0,    0,
    0, 7996,    0,    0, 8099,    0,    0, 8193,    0, 8221,
    0,    0, 8315,    0,    0, 8343,    0,    0, 9150,    0,
 2592,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2246,    0,    0,    0,    0,  805,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   15,    0,    0,    0,    0,    0,    0,    0,  142,
 1469,  125,  133,    0,    0,    0, 1469,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2335, 1079,
    0, 7055,    0, 2336,    0, 2336, 2335, 2336,    0,    0,
    0, 1102,    0,    0,    0, 1391, 2371,    0, 1459,    0,
    0,    0, 2227,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1942,    0, 1280, 2271,    0,    0,  818,  822,
  925,  936,  937,  943,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  229,    0,    0,
    0,    0, 3457,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1474,    0,    0,    0,    0,    0, 1496,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,    0,
    0,    0,  149,    0,  210,    0,    0,    0,    0,   74,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 8418,    0, 8512,    0, 8540,    0,    0,    0,    0, 8634,
    0,    0,    0,    0,    0,    0, 1229,    0,    0, 2635,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2640,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6184, 2624,    0,
    0,    0,    0,    0,    0, 1495, 1572,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  569,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1578,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1506,    0,    0, 6846,    0,    0,    0, 1414,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1454,
    0,    0,    0,    0,    0,    0, 1343,    0,    0,    0,
    0,    0,    0,    0, 2644, 1522, 1993,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2371,    0,
    0,    0,    0,    0,    0, 1511,  327,    0,    0,    0,
    0,    0,    0,    0,    0, 2371, 2408,    0,  425,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  638,    0,
 8709,    0,    0,    0, 2615,    0, 8737,    0,    0,    0,
 8831,    0,    0,    0,    0, 8859,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2592,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2627,
    0,    0, 2629,    0,    0, 2630,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2385,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 9253,
    0,    0,    0,    0, 2335,  163,  647,    0, 1431,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3872,    0,    0,    0,  206,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  676,    0,    0,    0,    0,    0, 8934,    0,    0,
    0,    0, 9028, 9056,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1745, 2651,    0,    0,
    0,    0,    0,    0,    0,    0, 2636, 2638,    0,    0,
    0, 2642,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1517, 1162,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3872,  309,    0,
  182,    0,    0,    0,    0, 2644,    0,    0,    0, 2661,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  472,    0,    0,    0,    0,    0,
    0,    0, 8953,    0,    0,    0, 1345,    0, 1280, 1424,
 1637,    0,    0,    0,    0,    0,    0,    0, 2645,  472,
 2646,    0,    0,    0, 2402,    0, 1527,    0,    0,    0,
 3872,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,16961,    0,    0,    0,  472,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  472,    0,    0,
  472,    0,    0,    0,    0,    0,    0,  472,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0, 2706,    0,    0,
 2708, 2711,    0,    0,    0,    0,    0,    0,    1,    0,
 2716,   -1, 2561, 2146,  -61, -868, 1354, 2572, 1882, 2579,
 2163,    0,    0, 2422, -106,    4,    0,    0,    0,    0,
  236,  249,  -59,  -81,  -64, -290, 2429, 1910,    0,    0,
    0, 1052,    0,    0,  -18, 1497, -616,  -72,-1063, 2018,
 1760, -384,    0, 1509,    0, 2657, 1766, 1810,    0,    0,
    0, -588,  -50,    0,    0,    0,    0,    0, 2690,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -67,    0, -618,  -44, 1529, -528,  250, -945,
    0,    0,    0,    0, -405, -133, -899, 1535, -415,  -56,
    0, -638, 1809,    0, 1100, -308, -798,    0, 1101,    0,
 1130, -393, -726,   20,    0,    0,    0,    0, 2343,    0,
 2100, 1562,    0,    0,    0, 1624,    0, 2426, 1907, 1561,
 2376, 2380,    0,    0, -532, 1888,  -51, -809, 1010, -293,
    0,    0, -863, -859, -857, -855, -844,    0, 1152,-1189,
    0,  874,-1361,    0, 2150,    0,    0,    0,  873,  879,
  858, -159, -318,-1070,-1051,    0,    0,    0, 1915,    0,
 -756,    0, 1703,    0,    0,    0,    0,    0,-1324, -753,
    0,    0,    0,    0,    0,    0, -252,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,-1062,    0,    0, -790,    0, -789, 1253, 1057,    0,
  900,    0,    0,    0,    0,    0,    0,    0, -495,    0,
    0, 2013, -152, -502, 1418, -622, -849,-1107,-1146,    0,
    0,  150,    0,    0,    0,    0,    0, 2405,    0,    0,
    0,    0, 2421, 2169,    0,    0,    0,    0,    0,   -9,
 -552,    0,    0, 2432,    0,    0,    0,  -10,    0,    0,
    0, -425, 1871, -424, 1394,    0,    0,    0,    0,    0,
    0, -486,    0,    0,    0, 2122,    0, 1890,    0,    0,
    0,    0,    0,    0,   29,    0, 2832,    0, 2684,    0,
    0,    0, 2494,    0,    0,    0,    0, 2253,    0, 2255,
    0,    0,    0, 1981,    0,    0,    0, -695,    0, -251,
    0,    0,    0,    0,    0, -255, -740, -975, 2546,    0,
 1211,    0,    0,    0,    0,    0,    0, -336,    0,    0,
 -840, -235,  980,    0,    0,    0, 2692, 1270,    0, 2000,
 -670,-1619,    0,-1000, 1732,-1364,    0,    0,    0,    0,
 -169,    0,    0,    0,    0, 1465,    0,    0,    0,    0,
 2773,  267,    0,  974, 2095,    0, 1140,    0, 1358,    0,
 -253,    0,    0, 1991,    0,    0,    0,    0,    0,    0,
 2259,    0,    0,    0, -475,-1305, 2034, 1757,-1243,    0,
    0,    0,    0, -752, -524,  154,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1640, 1608,    0,    0,    0, -804,    0,    0,    0, 2406,
 1728,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,-1017, 1569,    0,    0,
};
#define YYTABLESIZE 18263
static short yytable[] = {                                     154,
   40,  245,   86,  266,  364,  174,  173,  316,  181,  893,
  239,  336,  339,  261,  275,  282,  648,  277,  262,  812,
  306,  719,  720,  829,  917,  297,  303,  689,  569,  654,
  335,  338, 1355,  683, 1097, 1098, 1557, 1035,  307, 1175,
 1153,  305,  894, 1176,  867, 1177,  301, 1178,  234,  237,
  257,  241,  244,  173,  249,  252, 1603, 1363, 1179, 1312,
  263,  237,  252, 1661,  244,  242,  173,  249, 1061,  281,
  154,  252, 1083,  850, 1069, 1103, 1157,  270, 1107,  293,
  174,  173,  295,  237,  250,  293,  252,  241,  244,  304,
  173,  249,  281,  154,  252,  323,  244,  372, 1180,  847,
  589,  302, 1184,  652,  322,  661,  324, 1352,  367,  309,
  453, 1191,  612, 1421,  779,  300,  370,  779,  988,  740,
  756,  905,  903,  308,   52,  909, 1151,  948, 1389,  658,
  919,  997,   53, 1465,  735,  334, 1619, 1216, 1241,   85,
  167, 1092,  627,  789,   85,  364,  263,  173,  263, 1222,
  758,  709,  154,  974,  320, 1444,  779,  334,  168,  252,
  778,  494,  752,  956,  363, 1460,  237,  244,  173,  281,
  809, 1236,  460,  181,  880,  824,  779,  672,  740,  756,
  368,  252,  507,   52,  789, 1092,  968,  789,  346,  846,
  827,   53,  709,  971, 1158,  613,  320,  779,  844,  167,
 1092,  778,  789,  752,  364,  824,  752, 1263,  617,  758,
  709,  374, 1356, 1246,  809,  779,  417,  168,  672,  778,
  494,  752,  252,  898,  274,  252,  785,  604,  153,  809,
 1002,  333,  815, 1659, 1696,  637,  672,  719,  720,  937,
  252,  507, 1423, 1759, 1760, 1377,  824,  448,  274,  824,
  848,  601, 1428,  902,  334,  391,  156,  865,  555,  813,
  337,  563, 1429,  947,  824,  619, 1403,  785,  876, 1028,
  785, 1784,  925,  449, 1603,  689,  842,  881, 1448, 1379,
 1880,  792,  106,  334,  362,  785,  604,  153,  483,  605,
  370,  815,  569, 1252,  264,  604,  598,  646,  884,  421,
  263, 1308, 1309,  263, 1097, 1098,  674,  486,  824,  571,
  154, 1958, 1537,  611,  391, 1539, 1431, 1404, 1690,  295,
  157, 1086,  792,  749,  834, 1962,  152,  833, 1094,  339,
  813,  454,   87,   67,  940,   87, 1642,  241,  870, 1338,
  792,  173, 1477,  362,  649, 1407,  938,  674,  616,  824,
  838,  486,  824,  837,  604,  953,  159,  650, 1993,  450,
 1831,  653, 1430, 1926,  749,  674,  486,  824, 1253, 1654,
  941,  555,  422,  262, 1478,  669,  389,  970,  671,   14,
 1182,  730,  749,  625,  625,  152, 1029, 1754, 1187, 1684,
 1380,  733, 1630, 1675,  451,  483,  953,  741,  939,  953,
  638,  954,  603,  263,  140,  613, 1199,  871, 1688,  676,
  743, 1223, 1406,  263,  953,  263, 1206,  263, 1888, 1697,
  674,  684,  455,  686,  194, 1419,  390,  583, 1261,  370,
 1641,  755, 1975,  264, 1330,  389, 1871,  167,  739,  237,
  244,  249,  954, 1900, 1416,  954,  507,  483,  953,  173,
  676,  241, 1432,  731,  752,  168,  185,   95,  789,  339,
  954, 1963,  749,  140, 1410,  158,  709, 1659,  676,  790,
  942,  779,  755,  673, 1653,  778,  709,  752,  709,  789,
 1260, 1650, 1930,  194,  167,  390, 1242,  709,  160,  752,
  755, 1670,  779, 1832,  954, 1341,  778, 1258,  752,  740,
  756, 1259,  168,  839,  813,  153,  816,  824,  740,  756,
  790, 1919,  846,  672,  673,  185,  851, 1624, 1625,  391,
 1740, 1692, 1215,  611, 1743, 1141,  494,  846,  790,  740,
  758,  709,  673, 1884, 1482, 1483, 1936, 1484, 1485,  875,
  709,  785,  752, 1215,   84, 1262,  955,  364,  949,   84,
  809,  752,  973,  569,  332,  173,  244,  571,  519,  809,
 1245, 1956,  785,  740,  173, 1550, 1551, 1356,  173,  841,
   52,  979,  507,  571,  465,  181,  563, 1964,   53, 1824,
  319,  824,  587,  709,  824,  824,  779, 1092,  848,  789,
 1791,  592,  824,  815,  752,  709,  792,  709, 1817, 1981,
  843, 1552,  779,  152, 1103,  888,  778, 1248,  752, 1687,
  950,  921, 1459,  855,  809, 1823,  658,  792,  263, 1603,
 1925, 1125,  899,  672, 1469, 1384, 1388, 1152,  604, 1198,
  779,  997,  740,  756,  186, 1833, 1785,  906,  749, 1159,
  389,  332,  674, 1383, 1603,  391,  748,  631, 1686,  167,
  749,  636,  914,  263, 1409, 1099,  789, 1422,  914,  749,
 1234, 1418,  332,  758,  709, 1999,  951,  168,  332,  779,
  332, 1603,  785,  778,  252,  752,  791, 1905,  557, 1104,
  824,  557, 1603,  809,  824, 1603,  362,  748,  262,  864,
  390,  953, 1603,  186,   87,  946,  604, 1792,  824, 1872,
 1433,  824, 1028,  749, 1795,  748, 1493,  824, 1798, 1799,
  613,  978,  749, 1028,  185,  385,   97,  791,  982,  983,
  785, 1629,   95,  604, 1860,   91,  185,  792, 1040, 1837,
  263, 1144, 1028, 1864,  183,  791, 1434,  954,  405,  785,
 1769,  824, 1825,  184, 1827,  676,  755, 1794,  140, 1705,
  796,  687,  674,  185, 1494,  749,  755,  953,  755,  185,
 1412,  185,  809,  810,  811, 1903,  389,  755,  194,  749,
  606, 1904,  185,  252,  385,  822,  820, 1489,  821,  627,
  823,  555, 1028,  555,  790, 1112,  339, 1175, 1175, 1826,
 1123, 1176, 1176, 1177, 1177, 1178, 1178,  405,  637, 1790,
  185,  824, 1042,  954, 1028,  790, 1179, 1179, 1088,  673,
 1028,  755,  605, 1144, 1150, 1264,  390,  396,  263, 1029,
  755,  397,  106,  263, 1998,  263, 1765,  263,  611,  606,
 1029, 1461,  274,  262,  571,  856,  749, 1662,  627,  858,
 1166,  431,  173,  244, 1544, 1895, 1180, 1180,  500, 1029,
 1127,  244, 1796, 1797,  186, 1949,  841, 1951,  555,  238,
  137, 1248, 1195,  755,  173, 1955,  173, 1957,  827,  263,
 1770,  888,  966,  293,  611,  263,  396,  755, 1165, 1194,
  397,  625,  340,  339, 1196, 1734,  822, 1200, 1181, 1651,
 1652,  823,  186, 1674, 1621, 1628, 1682, 1030,  151, 1029,
  263, 1231, 1232, 1233,  186, 1873, 1669,  500, 1003, 1167,
  557, 1989, 1789, 1010,  432,  790,  339, 1354, 1235,  673,
 1031, 1029,  341,  253,  393, 1099,  263, 1029, 1243,  263,
 1524,  186, 1265,  611, 1212,  392,  395,  186,  967,  186,
 1059,  107,  394,  557,  755,  752,  340, 1064, 1065,  613,
  330, 1874,  253, 1213, 1394, 1780, 1673,  151, 1942, 1943,
 1944,  748, 1875,  688,  633,  185,  269, 1395,  608, 1863,
  173, 1097, 1098,  748,  385, 1542, 1032,  130,  186,  385,
  173,   97,  748,  393,  385,  557, 1039,  194,  752, 1257,
 1396,  791,  632, 1995,  392,  395,  749, 1333, 1334, 1175,
 1175,  394,  405, 1176, 1176, 1177, 1177, 1178, 1178, 1009,
  818,  130,  791,  339,  601,  819,  610, 1490, 1179, 1179,
 1750,  597,  675,  633,  643,  738,  748,  608, 1869, 1038,
  137,  137,  676, 1043,  818,  748,   84,  412,  756,  819,
  557, 1398,  410, 1060, 1996,  130,  194,  898, 1067, 1068,
 1329,  632, 1071, 1072, 1073, 1074, 1075, 1079, 1180, 1180,
  252, 1876,  636,  169, 1915, 1916,  519,  611, 1089, 1090,
 1091, 1092, 1093,  519, 1604,  610, 1100, 1607,  748,  756,
  597,  396,  288,  643,  274,  397, 1066,  411, 1144, 1342,
 1065,  646,  748, 1205,  646,  557,  412,  756,  719,  720,
  188,  754,  647,  571,  571,  385,  500, 1982, 1045, 1045,
 1220,  602,  592,  636,  628,  285,  399,  627,  500,  244,
 1450,  818,  791,  274,  274,  274,  819, 1097, 1098, 1097,
 1098,  922, 1154,  290,  255,  256, 1335, 1097, 1098, 1097,
 1098,  888,  754,  186,  631, 1164, 1983, 1424, 1425,  286,
  627,  500,  189,  500, 1003, 1929,  400, 1454, 1003,  748,
  754,  721, 1933,  170, 1829,  603,  413, 1394,  310, 1451,
  171, 1952, 1472,  628,  151,  151,  627, 1065, 1476,  627,
 1395, 1450,  604, 1097, 1098,  868, 1099,  262,  393,  532,
  298,  563,  613, 1984,  412,  627, 1973, 1065, 1470,  392,
  395, 1439,  721, 1396,  352,  721,  394,  396,  923,   14,
  911,  397,  413,  910, 1440,  438,  425, 1397, 1439,  426,
  721,  627,  312, 1992,  409,  413, 1616, 1616, 1620,  263,
 1453, 1490, 1490,  263, 1997, 1644,  849, 2000,  532, 1490,
 1490,  313, 1490, 1490, 2001, 1645,  414, 1045,  924, 1065,
 1490, 1490, 1490, 1490, 1398, 1490, 1490, 1490, 1490, 1490,
 1490,  427, 1266, 1267, 1271, 1807,  887, 1280, 1282, 1284,
  151, 1292, 1295, 1298, 1300, 1303, 1306,  176, 1647,  247,
 1649,  238, 1885,  409,  194, 1313, 1314, 1608, 1316,  419,
 1318,  367,  317, 1321, 1322, 1323, 1324, 1325, 1326,  318,
 1517,  173,  633,  340, 1553, 1609,  555, 1606,   43,  262,
  611, 1331,  385, 1336,  393, 1960,  590, 1147,  646,  176,
  247,  238,  385,  247,  596,  392,  395, 1689,  496, 1351,
  632,  194,  394, 1148,  177,  633,  176,  321,  247,  194,
  710,  463,  194, 1036,  252,   47,  340,  263,  263,  468,
  367, 1558, 1602,  756, 1276,  571,  244,  249,  571,  281,
  154,  633,  643,  756,  633,  756, 1399,   43,  325, 1605,
  519,  255,  256,  194,  756,  194,  571,  571,  326,  274,
  633,  710,  165,  244,  710,  252, 1044,  496,  252,  632,
  792,  365,  632,  177,  274,  643,  712,  194,  238,  710,
  274,  194, 1277,  252,   47,  177,  633,  109,  632,  712,
  613,   88,   89,  711, 1675,  348,  754,  712,  756,  820,
  347,  643,  194,  248,  643,  365, 1278,  756,  822,  820,
  753,  821,  712,  823,  632, 1803,  178,  754,  238, 1672,
  643, 1804,  395,  111,  509,  142,  712,  340,  349,  792,
  365,  179,  628,  758,  711, 1695,  398,  711,  380, 1782,
 1700,  931,  402,  932,  248,  438,  643,  248,   41,  340,
  756,  753,  711,  143,  350, 1805,  721,  351,  820,  721,
  396,  754,  248,  712,  756,  628,  721, 1046,  721,  753,
  754, 1753,  721,  352,  758,  131,  194,  721,  418,  439,
  721,  440,   57,  509,  142,  805, 1517,  173,  112, 1048,
  215,  628,  758,   58,  628,  721,  803,  380,  113,  353,
   59,  215,  519,  441,   60, 1730,  804,   41,  611,  721,
  628, 1082,  143,  754,  827, 1500, 1501,  721, 1503, 1505,
 1094,  721, 1120,  114,   61, 1119, 1510,  754,  442,  237,
  721,  756, 1094,  340,  131, 1094,  628, 1758, 1257, 1257,
 1731,  769,  215,  423,  805,  215,  721, 1286, 1528,  215,
 1602,  404, 1732,  435,  332,  803, 1535,  332, 1536, 1336,
  215, 1287, 1336,  405, 1331,  804,  829,  434, 1543,  519,
 1545, 1240, 1099,  721, 1239,  712,  769, 1546, 1809, 1547,
  194, 1143,  571,  571, 1207,  721,   62,  721,  769,  634,
 1887,  635,  194,  829,  754,  437,  406, 1099,  407,  363,
  252,  252,  108, 1079, 1616, 1288,  244,  244,  252, 1099,
  341, 1244, 1099,  446,  993, 1075,  249,   63, 1065,  194,
  519,   64, 1349,  822,  820,  194,  821,  194,  823, 1353,
  173,  457,  827, 1289,  721,  710,  822,  820,  459,  821,
  109,  823, 1633,  818, 1635,  710, 1359,  710,  819, 1358,
 1408, 1655,  110, 1239,  721,  465,  710,  249,  721,  745,
  249, 1857, 1442,  164,  165, 1441,  194,  822,  820,  461,
  821, 1970,  823, 1971,  194,  249,  111,  464, 1447,  248,
  248, 1446, 1901,  900,  484,  485,  486,  248,  560,  822,
  820,  564,  821,  566,  823,  574,  252, 1290, 1517,  173,
  710, 1549,  241, 1623, 1356,  575,  827,  578,  711,  710,
   65, 1704,  494,  495,  827, 1701, 1745,  252,  711,  827,
  711, 1707, 1899,  252,  900,  753, 1711,  900, 1099,  711,
 1099, 1257,  745, 1716, 1781,  613,  579,  827, 1099, 1902,
 1099,  112,  900,  194, 1979,  580,  753, 1978,  758, 1881,
  363,  113,  710,  244,  581, 1980,  275,  252,  837,  275,
  613,  582,  951,  599,  710,  777,  710, 1336,  777,  758,
 1088, 1336, 1937,  711,  173,  248,  114, 1749, 1208, 1209,
  241,  584,  711,  597, 1099,  600, 1921,  613,  255,  256,
  753, 1907,  620, 1756,  896, 1909,  248,  622,  613,  753,
  609,  613,  248,  951,  951,  951,  951,  951,  613,  951,
  138,  138,  623,  758,  857,  822,  820,  252,  821,  624,
  823,  951,  951,  951,  951,  711,  241, 1773, 1775,  822,
  820,  194,  821,  710,  823,  628,  248,  711,  630,  711,
 1774, 1776,  753,   14,  237, 1923,  822,  820,  642,  821,
  651,  823,  644,  611, 1924,  951,  753,  643,  818,  667,
  656,  670,  563,  819,  680,  758,  681, 1786,  534,  736,
  685,  818,  822,  820,  740,  821,  819,  823,  611,  758,
  744,  742,  779,  777, 1081,  822,  820,  780,  821,  781,
  823,  783,  249,  249,  782, 1602,  248, 1948, 1348,  784,
  249, 1127,  818,  785,  263,  611,  711,  819,  789,  534,
  786,  787,  534,  788,  790,  241,  611,  241,  791,  611,
 1602,  226,  794,  753,  818,  151,  611,  534,  799,  819,
 1918,  802,  800,  822,  820,  801,  821, 1838,  823,  803,
  804,  805, 1548, 1843, 1844,  900,  758, 1602, 1127,  824,
 1126,  822,  820,  806,  821, 1497,  823,  807, 1602,  808,
 1257, 1602,  226,  817,  825,  226,  151, 1858, 1602,  151,
  827, 1336,  227,  828,  836, 1861,  822,  820, 1339,  821,
  226,  823, 1867, 1868,  151,  832,  835,  853,  249,  861,
  822,  820,  878,  821,  863,  823,  874,  879,  900,  822,
  820,  900,  821,  883,  823,  885,  886,  519,  900,  249,
  900, 1095,  867,  227,  900,  249,  227,  890,  895,  900,
  951,  891,  900,  897,  951,  637,  951,  951, 1037,  516,
 1041,  227,  900,  951, 1045, 1047, 1049,  900,  951,  951,
  646,  907,  912,  935,  951,  944,  951,  951,  936,  249,
  818,  900,  332,  867,  952,  819,  867,  953, 1906,  900,
  960,  951,  900,  900,  818,  951,  951,  951,  958,  819,
  951,  867,  900,  959,  961,  963,  964,  951,  969,  951,
  951,  818,  975,  951,  976,  980,  819,  951,  900,  951,
  981,  951,  990,  951,  859,  992,  993,  999,  951,  951,
  998,  951, 1738, 1000, 1004,  188, 1062,  818, 1070,  249,
  900, 1084,  819,  970,  334,  900,  951,  951, 1108,  951,
  818, 1117,  557, 1786, 1113,  819, 1114,  900, 1121,  900,
  951,  844,  951, 1115,  951,  951,  951, 1145,  951, 1140,
 1974,  951,  951, 1155,  534,  534, 1160,  837, 1161,  534,
 1171,  951,  534, 1185, 1186,  951, 1954,  915, 1192, 1193,
 1202,  951, 1197,  557,  951,  534,  557,  951,  818, 1203,
 1204, 1211,  951,  819, 1218, 1228,  900, 1230, 1251, 1237,
 1273,  557, 1310,  951, 1311, 1319,  818, 1317, 1328,  951,
  951,  819, 1327,  818,  951,  951,  900,  238,  226, 1715,
  900,  151,  151, 1356, 1357,  951,  951, 1360,  951,  151,
 1362,  818, 1391,  285,  951, 1378,  819,  951, 1381,  951,
  951,  951,  951, 1386,  951,  818, 1393, 1387, 1401, 1413,
  819, 1426,  951,  951,  818, 1443, 1445, 1435, 1449,  819,
  534, 1455, 1456,  951,  951, 1086,  951,  286,  238,  227,
 1457, 1245, 1458,  951,  951,  951,  951,  951, 1466,  951,
 1474,  534,  951,  951,  951,  951,  951,  534, 1479, 1480,
  951,  951, 1481, 1487,  867,  951,  951,  951,  951,  951,
  951,  951,  951, 1488,  951, 1061, 1086, 1086, 1086, 1086,
 1086, 1086, 1086,  238,  951, 1492, 1495,  151, 1496, 1498,
 1499,  534, 1506, 1086, 1086, 1086, 1086, 1086, 1507, 1394,
 1028, 1320,  822,  820,  238,  821, 1508,  823,  151, 1509,
  238, 1511, 1395, 1512,  151, 1513, 1514,  867, 1515, 1516,
  867, 1538, 1541, 1610, 1612, 1613, 1086,  867, 1086,  867,
 1626, 1627,  859,  867,  238, 1396, 1636, 1631,  867, 1637,
 1638,  867, 1639, 1648,  238,  662, 1663, 1660,  151, 1397,
 1664, 1665, 1666, 1667, 1668,  238,  867, 1889, 1671, 1685,
 1215,  238, 1691, 1343,  822,  820, 1693,  821, 1694,  823,
  867, 1699, 1706, 1708, 1728, 1735, 1729, 1741,  867, 1751,
 1752,  867,  867, 1755, 1742,  157, 1398, 1762,  557,  557,
 1763,  867,  524,  557, 1768,  238,  557, 1344,  822,  820,
 1771,  821, 1783,  823, 1772, 1778, 1800,  867, 1777,  557,
 1811, 1814, 1810,  115, 1816, 1812,  116, 1813,  117,  118,
  119,  120, 1815,  121, 1345,  822,  820, 1818,  821,  867,
  823,  122,  123,  524,  867, 1819,  524, 1820, 1830, 1834,
 1828, 1835,  124,  125, 1836,  126,  867, 1839,  867, 1840,
 1841,  524,  127,  128, 1842,  129,  130, 1845,  131, 1846,
 1847,  132,  133,  134,  135,  136, 1848, 1849, 1850,  137,
  138, 1851, 1852, 1853, 1890,  139,  140,  141, 1854,  142,
  143,  144, 1856,  145,  557,  822,  820, 1340,  821, 1859,
  823, 1866, 1882,  146, 1894,  867, 1883, 1086, 1399, 1086,
 1086, 1764, 1896, 1898, 1908,  557, 1086, 1912, 1917, 1920,
 1927,  557, 1086, 1928, 1931,  867, 1934, 1086, 1938,  867,
 1086, 1945, 1946, 1947, 1950, 1969,  555, 1346,  822,  820,
 1965,  821, 1967,  823, 1086, 1065, 1972,  818, 1977, 1086,
 1086, 1962,  819, 1086, 1963,  557,  822,  820, 1987,  821,
 1086,  823, 1086, 1086, 1990, 1988, 1086, 1347,  822,  820,
 1086,  821, 1086,  823, 1086,    1, 1086,  555, 1991, 1994,
  555, 1086, 1086,  785, 1086, 1059, 1065, 1065, 1065, 1065,
 1065, 1079, 1065,   29,   31,  555,  201,  555,  824, 1086,
 1086,  164, 1086,  266, 1065, 1065, 1065, 1065,   82,  818,
  701,  764, 1044, 1086,  819, 1086,  953, 1086, 1086, 1086,
  277, 1086,  211,  763, 1086, 1086, 1050, 1051, 1052, 1053,
 1054, 1055, 1056, 1057, 1086,  792, 1065, 1281, 1065, 1285,
  276, 1293, 1296,  818, 1301, 1304, 1307, 1086,  819,  191,
 1086,  405, 1043,  745,  747,  765,  192, 1522,  822,  820,
 1017,  821,  322,  823,  378,  288, 1086,  320,  330,  294,
  818, 1016, 1086, 1086,  321,  819,  323, 1086,  524,  524,
  331,  774,  344,  326,  359,   29,  524,   32, 1086, 1086,
   33, 1086, 1523,  822,  820,   42,  821, 1086,  823,  524,
 1086,  327, 1086, 1086, 1086, 1086,  852, 1086,  822,  820,
 1614,  821,  328,  823, 1124, 1086, 1086, 1526,  822,  820,
  329,  821,  573,  823, 1475,  854, 1086, 1086,  572, 1086,
  818,  299, 1250, 1116, 1473,  819, 1086, 1086, 1086, 1086,
 1086,  991, 1086, 1255, 1214, 1086, 1086, 1086, 1086, 1086,
  284, 1471, 1463, 1086, 1086, 1821, 1224, 1822, 1086, 1086,
 1086, 1086, 1086, 1086, 1086, 1086,  693, 1086, 1806, 1452,
  954, 1156, 1415,  818,  524,  618, 1486, 1086,  819, 1464,
  664, 1527,  822,  820,  665,  821, 1190,  823, 1802, 1897,
 1961,  818, 1364, 1968,  555,  524,  819,  930, 1966, 1757,
 1986,  524, 1879,  818, 1183, 1953, 1643, 1065,  819, 1065,
 1065,  555,  555,  555,  672,  655, 1065,  555,  934,  645,
  555, 1229, 1065, 1683,  163, 1105,  331, 1065,  610,  977,
 1065,  869, 1221,  555, 1146,  524,  552,  873,  345,  555,
 1390, 1634, 1142,  294, 1065, 1096, 1065, 1935, 1855, 1065,
 1065, 1033,  882, 1065, 1727, 1529,  822,  820, 1385,  821,
 1065,  823, 1065, 1065, 1122, 1163, 1065, 1530,  822,  820,
 1065,  821, 1065,  823, 1065, 1502, 1065,  552,  793, 1417,
  552, 1065, 1065, 1540, 1065,    0, 1096, 1096, 1096, 1096,
 1096, 1096, 1096,  818,    0,  552,    0,  552,  819, 1065,
 1065,    0, 1065,    0, 1096, 1096, 1096, 1096,  555,    0,
 1531,  822,  820, 1065,  821, 1065,  823, 1065, 1065, 1065,
    0, 1065,    0,    0, 1065, 1065,    0,    0,  818,  555,
 1315,    0,    0,  819, 1065,  555, 1096,    0, 1096,    0,
    0,    0,    0,  818,  555,    0,  555, 1065,  819,    0,
 1065,    0,    0,  818,    0,    0,    0,    0,  819,    0,
    0,    0,    0,    0,  555,    0, 1065,    0,    0,  555,
    0,    0, 1065, 1065,    0,    0,    0, 1065,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1065, 1065,
    0, 1065,    0,    0,    0,    0,    0, 1065,  555,    0,
 1065,    0, 1065, 1065, 1065, 1065,    0, 1065, 1532,  822,
  820,    0,  821,    0,  823, 1065, 1065,  818,    0, 1533,
  822,  820,  819,  821,    0,  823, 1065, 1065,    0, 1065,
  822,  820, 1746,  821,    0,  823, 1065, 1065, 1065, 1065,
 1065,    0, 1065,    0,    0, 1065, 1065, 1065, 1065, 1065,
    0,    0,    0, 1065, 1065,    0,    0,    0, 1065, 1065,
 1065, 1065, 1065, 1065, 1065, 1065,    0, 1065,    0,    0,
 1534,  822,  820,    0,  821,    0,  823, 1065, 1736,  822,
  820,    0,  821,    0,  823,    0,    0,    0,    0,    0,
    0,  818,    0,    0,  552,    0,  819,    0,    0,    0,
    0,    0,    0,  818,    0,    0,    0, 1096,  819, 1096,
 1096,  552,  552,  552,    0,    0, 1096,  552,    0,    0,
  552,    0, 1096,    0,    0,    0,    0, 1096,    0,    0,
 1096,    0,    0,  552,    0,    0,  557,    0,    0,  552,
    0,    0,    0,    0, 1096, 1101,  818,    0,    0, 1096,
 1096,  819,    0, 1096,    0, 1737,  822,  820,    0,  821,
 1096,  823, 1096, 1096,    0,    0, 1096, 1744,  822,  820,
 1096,  821, 1096,  823, 1096,    0, 1096,  557,    0,    0,
  557, 1096, 1096,    0, 1096,    0, 1101, 1101, 1101, 1101,
 1101, 1101, 1101,    0,    0,  557,    0,  557,    0, 1096,
 1096,    0, 1096,    0, 1101, 1101, 1101, 1101,  552,    0,
 1747,  822,  820, 1096,  821, 1096,  823, 1096, 1096, 1096,
    0, 1096,    0,    0, 1096, 1096,    0,    0,    0,  552,
    0,    0,    0,    0, 1096,  552, 1101,    0, 1101,    0,
    0,    0,    0,    0,  818,    0,  552, 1096,    0,  819,
 1096,    0,    0,    0,    0,  818,    0, 1748,  822,  820,
  819,  821,    0,  823,  552,  818, 1096,    0,    0,  552,
  819,    0, 1096, 1096,    0,    0,    0, 1096,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1096, 1096,
    0, 1096,    0,    0,    0,    0,    0, 1096,  552,    0,
 1096,    0, 1096, 1096, 1096, 1096,  818, 1096,    0,    0,
    0,  819,    0,    0,  818, 1096, 1096,  822,  820,  819,
  821,    0,  823,    0,    0,    0, 1096, 1096,    0, 1096,
    0,    0,    0,    0,    0,    0, 1096, 1096, 1096, 1096,
 1096,    0, 1096,    0,    0, 1096, 1096, 1096, 1096, 1096,
    0,    0,    0, 1096, 1096,    0,    0,    0, 1096, 1096,
 1096, 1096, 1096, 1096, 1096, 1096,    0, 1096,    0,    0,
 1862,  822,  820,    0,  821,    0,  823, 1096, 1914,  822,
  820,    0,  821,    0,  823,    0,    0,    0,    0,    0,
    0,  818,    0,    0,  557,    0,  819,    0,    0,    0,
    0,    0,    0,  818,    0,    0,    0, 1101,  819, 1101,
 1101,    0,  557,  557,    0,    0, 1101,  557,    0,    0,
  557,    0, 1101,    0,    0,    0,    0, 1101,    0,    0,
 1101,    0,    0,  557,    0,    0,  557,    0,    0,  557,
    0,    0,    0,    0, 1101, 1097,  818,    0,    0, 1101,
 1101,  819,    0, 1101,    0,    0,    0,    0,    0,    0,
 1101,    0, 1101, 1101,    0,    0, 1101,    0,    0,    0,
 1101,    0, 1101,    0, 1101,    0, 1101,  557,    0,    0,
  557, 1101, 1101,    0, 1101,    0, 1097, 1097, 1097, 1097,
 1097, 1097, 1097,  818,    0,  557,    0,    0,  819, 1101,
 1101,    0, 1101,    0, 1097, 1097, 1097, 1097,  557,    0,
    0,    0,    0, 1101,    0, 1101,    0, 1101, 1101, 1101,
    0, 1101,    0,    0, 1101, 1101,    0,    0,    0,  557,
    0,    0,    0,    0, 1101,  557, 1097, 1714, 1097,    0,
    0,    0,    0,    0,    0,    0,  557, 1101,    0,    0,
 1101,    0,  818,    0,    0,    0,    0,  819,    0,    0,
    0,    0,    0,   92,  557,    0, 1101,    0,    0,  557,
   93,    0, 1101, 1101,   94,   95,    0, 1101,    0,    0,
    0,    0,    0,   96,    0,    0,    0,    0, 1101, 1101,
    0, 1101,    0,    0,   97,    0,    0, 1101,  557,    0,
 1101,    0, 1101, 1101, 1101, 1101,  818, 1101,    0,    0,
    0,  819,    0,    0,  818, 1101, 1101,    0,    0,  819,
    0,    0,    0,    0,    0,    0, 1101, 1101,    0, 1101,
    0,    0,    0,    0,    0,    0, 1101, 1101, 1101, 1101,
 1101,    0, 1101,    0,    0, 1101, 1101, 1101, 1101, 1101,
    0,    0,    0, 1101, 1101,    0,   98,    0, 1101, 1101,
 1101, 1101, 1101, 1101, 1101, 1101,    0, 1101,    0,    0,
    0,    0,   99,    0,    0,    0,    0, 1101,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  100,    0,    0,
    0,  101,    0,    0,    0,    0,    0,    0,  557,    0,
    0,    0,  102,    0,    0,    0,   69, 1097,    0, 1097,
 1097,    0,  557,  557,    0,    0, 1097,   70,    0,    0,
  557,  103, 1097,    0,   71,    0,    0, 1097,    0,    0,
 1097,    0,    0,  557,    0,    0,    0,   72,    0,  557,
    0,    0,  557,    0, 1097, 1110,    0,    0,    0, 1097,
 1097,    0,    0, 1097,    0,    0,    0,  557,    0,  557,
 1097,    0, 1097, 1097,    0,    0, 1097,    0,    0,    0,
 1097,    0, 1097,    0, 1097,    0, 1097,    0,    0,    0,
  104, 1097, 1097,    0, 1097,    0, 1110, 1110, 1110, 1110,
 1110, 1110, 1110,    0,   73,    0,    0,    0,    0, 1097,
 1097,    0, 1097,    0, 1110, 1110, 1110, 1110,  557,    0,
   74,    0,    0, 1097,    0, 1097,    0, 1097, 1097, 1097,
    0, 1097,    0,    0, 1097, 1097,   75,    0,    0,  557,
    0,    0,    0,    0, 1097,  557, 1110,    0, 1110,    0,
    0,   76,    0,    0,    0,   77,    0, 1097,   78,    0,
 1097,  247,    0,    0,    0,    0,   79,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1097,    0,    0,  557,
    0,    0, 1097, 1097,    0,   80,    0, 1097,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1097, 1097,
    0, 1097,  247,    0,    0,  247,    0, 1097,    0,    0,
 1097,    0, 1097, 1097, 1097, 1097,    0, 1097,    0,    0,
  247,    0,    0,    0,    0, 1097, 1097,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1097, 1097,    0, 1097,
    0,    0,    0,    0,   81,    0, 1097, 1097, 1097, 1097,
 1097,    0, 1097,    0,    0, 1097, 1097, 1097, 1097, 1097,
    0,    0,    0, 1097, 1097,    0,  557,    0, 1097, 1097,
 1097, 1097, 1097, 1097, 1097, 1097,    0, 1097,    0,    0,
    0,  557,    0,    0,    0,    0,    0, 1097,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  557,    0,    0,    0,    0,
    0,  557,    0,    0,  557,    0,    0, 1110,    0, 1110,
 1110,    0, 1702, 1703,    0,    0, 1110,    0,    0,    0,
 1709, 1710, 1110, 1712, 1713,    0,    0, 1110,    0,    0,
 1110, 1717, 1718, 1719, 1720,    0, 1721, 1722, 1723, 1724,
 1725, 1726,    0,    0, 1110, 1115,  557,    0,    0, 1110,
 1110,    0,    0, 1110,    0,    0,    0,    0,    0,    0,
 1110,    0, 1110, 1110,    0,    0, 1110,    0,    0,    0,
 1110,    0, 1110,    0, 1110,    0, 1110,    0,    0,    0,
    0, 1110, 1110,    0, 1110,    0, 1115, 1115, 1115, 1115,
 1115, 1115, 1115,    0,    0,    0,    0,    0,    0, 1110,
 1110,  557, 1110,    0, 1115, 1115, 1115, 1115,  557,    0,
    0,    0,    0, 1110,    0, 1110,    0, 1110, 1110, 1110,
    0, 1110,    0,    0, 1110, 1110,  557,  247,  247,    0,
    0,    0,    0,    0, 1110,  247, 1115,    0, 1115,    0,
    0,    0,    0,    0,    0,    0,    0, 1110,    0,    0,
 1110,    0,    0,    0,    0,    0,  557,    0,    0,    0,
  557,    0,    0,    0,    0,    0, 1110,    0,    0,    0,
    0,    0, 1110, 1110,    0,    0,    0, 1110,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1110, 1110,
    0, 1110,    0,    0,    0,    0,    0, 1110,    0,    0,
 1110,    0, 1110, 1110, 1110, 1110,    0, 1110,    0,    0,
    0,    0,    0,    0,    0, 1110, 1110,    0,    0,    0,
    0,    0,    0,  247,    0,    0, 1110, 1110,    0, 1110,
    0,    0,    0,    0,    0,    0, 1110, 1110, 1110, 1110,
 1110,    0, 1110,    0,  247, 1110, 1110, 1110, 1110, 1110,
  247,    0,    0, 1110, 1110,    0,    0,    0, 1110, 1110,
 1110, 1110, 1110, 1110, 1110, 1110,    0, 1110,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1110,    0,    0,
    0,    0,    0,    0,  247,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1115,    0, 1115,
 1115,    0,    0,    0,    0,    0, 1115,    0,    0,    0,
    0,    0, 1115,    0,    0,    0,    0, 1115,    0,    0,
 1115,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1115,  919,    0,    0,    0, 1115,
 1115,    0,    0, 1115,    0,    0,    0,    0,    0,    0,
 1115,    0, 1115, 1115,    0,    0, 1115,    0,    0,    0,
 1115,    0, 1115,    0, 1115,    0, 1115,    0,    0,    0,
    0, 1115, 1115,    0, 1115,    0,  919,  919,  919,  919,
  919,    0,  919,    0,    0,    0,    0,    0,    0, 1115,
 1115,    0, 1115,    0,  919,  919,  919,  919,    0,    0,
    0,    0,    0, 1115,    0, 1115,    0, 1115, 1115, 1115,
    0, 1115,    0,    0, 1115, 1115,    0,    0,    0,    0,
    0,    0,    0,    0, 1115,    0,    0,    0,  919,    0,
    0,    0,    0,    0,    0,    0,    0, 1115,    0,    0,
 1115,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1115,    0,    0,    0,
    0,    0, 1115, 1115,    0,    0,    0, 1115,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1115, 1115,
    0, 1115,    0,    0,    0,    0,    0, 1115,    0,    0,
 1115,    0, 1115, 1115, 1115, 1115,    0, 1115,    0,    0,
    0,    0,    0,    0,    0, 1115, 1115,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1115, 1115,    0, 1115,
    0,    0,    0,    0,    0,    0, 1115, 1115, 1115, 1115,
 1115,    0, 1115,    0,    0, 1115, 1115, 1115, 1115, 1115,
    0,    0,    0, 1115, 1115,    0,    0,    0, 1115, 1115,
 1115, 1115, 1115, 1115, 1115, 1115,    0, 1115,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1115,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  919,    0,  919,
  919,    0,    0,    0,    0,    0,  919,    0,    0,    0,
    0,    0,  919,    0,    0,    0,    0,  919,    0,    0,
  919,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  919,  932,    0,    0,    0,  919,
  919,    0,    0,  919,    0,    0,    0,    0,    0,    0,
  919,    0,  919,  919,    0,    0,  919,    0,    0,    0,
  919,    0,  919,    0,  919,    0,  919,    0,    0,    0,
    0,  919,  919,    0,  919,    0,  932,  932,  932,  932,
  932,    0,  932,    0,    0,    0,    0,    0,    0,  919,
  919,    0,  919,    0,  932,  932,  932,  932,    0,    0,
    0,    0,    0,  919,    0,  919,    0,  919,  919,  919,
    0,  919,    0,    0,  919,  919,    0,    0,    0,    0,
    0,    0,    0,    0,  919,    0,    0,    0,  932,    0,
    0,    0,    0,    0,    0,    0,    0,  919,    0,    0,
  919,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  919,    0,    0,    0,
    0,    0,  919,  919,    0,    0,    0,  919,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  919,  919,
    0,  919, 1172,    0,    0,    0,    0,  919,    0,    0,
  919,    0,  919,  919,  919,  919,    0,  919,    0,    0,
    0,    0,    0,    0,    0,  919,  919,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  919,  919,    0,  919,
    0,    0,    0,    0,    0,    0,  919,  919,  919,  919,
  919,    0,  919,    0,    0,  919,  919,  919,  919,  919,
    0,    0,    0,  919,  919,    0,    0,    0,  919,  919,
  919,  919,  919,  919,  919,  919,    0,  919,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  919,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  932,    0,  932,
  932,    0,    0,    0,    0,    0,  932,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  932,    0,    0,
  932,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  932,  935,    0,    0,    0,  932,
  932,    0,    0,  932,    0,    0,    0,    0,    0,    0,
  932,    0,  932,  932,    0,    0,  932,    0,    0,    0,
  932,    0,  932,    0,  932,    0,  932,    0,    0,    0,
    0,  932,  932,    0,  932,    0,  935,  935,  935,  935,
  935,    0,  935,    0,    0,    0,    0,    0,    0,  932,
  932,    0,  932,    0,  935,  935,  935,  935,    0,    0,
    0,    0,    0,  932,    0,  932,    0,  932,  932,  932,
    0,  932,    0,    0,  932,  932,    0,    0,    0,    0,
    0,    0,    0,    0,  932,    0,    0,    0,  935,  472,
    0,    0,    0,    0,    0,    0,    0,  932,    0,    0,
  932,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  932,  474,    0,    0,
    0,    0,  932,  932,    0,    0,    0,  932,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  932,  932,
    0,  932,    0,    0,    0,  476,    0,  932,    0,    0,
  932,    0,  932,  932,  932,  932,    0,  932,    0,    0,
    0,    0,    0,    0,    0,  932,  932,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  932,  932,    0,  932,
    0,    0,    0,    0,    0,    0,  932,  932,  932,  932,
  932,    0,  932,    0,    0,  932,  932,  932,  932,  932,
    0,    0,    0,  932,  932,    0,    0,    0,  932,  932,
  932,  932,  932,  932,  932,  932,  481,  932,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  932,    0,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,  485,  486,    0,
  255,  256,    0,    0,    0,    0,    0,  935,    0,  935,
  935,    0,    0,    0,    0,    0,  935,    0,    0,  489,
  490,  491,  492,  493,  494,  495,  496,  497,    0,    0,
  935,    0,    0,    0,  499,  500,    0,    0,    0,    0,
    0,    0,    0,  504,  935,  936,    0,    0,    0,  935,
  935,    0,    0,  935,    0,    0,    0,    0,    0,    0,
  935,    0,  935,  935,    0,    0,  935,    0,    0,    0,
  935,    0,  935,    0,  935,    0,  935,    0,    0,    0,
    0,  935,  935,    0,  935,    0,  936,  936,  936,  936,
  936,    0,  936,    0,    0,    0,    0,    0,    0,  935,
  935,    0,  935,    0,  936,  936,  936,  936,    0,    0,
    0,    0,    0,  935,    0,  935,    0,  935,  935,  935,
    0,  935,    0,    0,  935,  935,    0,    0,    0,    0,
    0,    0,    0,    0,  935,    0,    0,    0,  936,    0,
    0,    0,    0,    0,    0,    0,    0,  935,    0,    0,
  935,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  935,    0,    0,    0,
    0,    0,  935,  935,    0,    0,    0,  935,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  935,  935,
    0,  935,    0,    0,    0,    0,    0,  935,    0,    0,
  935,    0,  935,  935,  935,  935,    0,  935,    0,    0,
    0,    0,    0,    0,    0,  935,  935,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  935,  935,    0,  935,
    0,    0,    0,    0,    0,    0,  935,  935,  935,  935,
  935,    0,  935,    0,    0,  935,  935,  935,  935,  935,
    0,    0,    0,  935,  935,    0,    0,    0,  935,  935,
  935,  935,  935,  935,  935,  935,    0,  935,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  935,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  936,    0,  936,
  936,    0,    0,    0,    0,    0,  936,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  936,    0,    0,    0,  930,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,    0,    0,    0,    0,  936,
  936,    0,    0,  936,    0,    0,    0,    0,    0,    0,
  936,    0,  936,  936,    0,    0,  936,    0,    0,    0,
  936,    0,  936,    0,  936,  930,  936,  930,  930,  930,
    0,  936,  936,    0,  936,  994,    0,    0,  695,    0,
    0,    0,    0,  930,  930,  930,  930,    0,    0,  936,
  936,  995,  936,    0,    0,  696,    0,    0,  697,    0,
    0,    0,    0,  936,    0,  936,    0,  936,  936,  936,
    0,  936,    0,    0,  936,  936,    0,  930,    0,    0,
    0,    0,    0,    0,  936,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  936,  698,    0,
  936,    0,    0,    0,  699,  700,  701,    0,  702,  703,
  704,    0,  705,    0,    0,    0,  936,    0,    0,    0,
    0,    0,  936,  936,    0,    0,    0,  936,    0,    0,
  706,    0,  707,    0,    0,    0,    0,    0,  936,  936,
    0,  936,    0,    0,    0,    0,    0,  936,    0,    0,
  936,    0,  936,  936,  936,  936,    0,  936,    0,    0,
    0,  708,    0,    0,    0,  936,  936,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  936,  936,  709,  936,
    0,    0,    0,    0,    0,    0,  936,  936,  936,  936,
  936,    0,  936,    0,    0,  936,  936,  936,  936,  936,
  710,    0,    0,  936,  936,    0,    0,    0,  936,  936,
  936,  936,  936,  936,  936,  936,    0,  936,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  936,    0,    0,
    0,    0,    0,    0,    0,  711,  712,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  930,    0,  930,  930,
    0,    0,  713,    0,    0,  930,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  930,
    0,    0,    0,  929,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  930,    0,    0,    0,    0,  930,  930,
    0,    0,  930,    0,    0,    0,    0,    0,    0,  930,
    0,  930,  930,    0,    0,  930,    0,    0,    0,  930,
    0,  930,    0,  930,  929,  930,  929,  929,  929,    0,
  930,  930,    0,  930,  219,    0,    0,  219,    0,    0,
    0,    0,  929,  929,  929,  929,    0,    0,  930,  930,
  219,  930,    0,    0,  219,    0,    0,  219,    0,    0,
    0,    0,  930,    0,  930,    0,  930,  930,  930,    0,
  930,    0,    0,  930,  930,    0,  929,    0,    0,    0,
    0,    0,    0,  930,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  930,  219,    0,  930,
    0,    0,    0,  219,  219,  219,    0,  219,  219,  219,
    0,  219,    0,    0,    0,  930,    0,    0,    0,    0,
    0,  930,  930,    0,    0,    0,  930,    0,    0,  219,
    0,  219,    0,    0,    0,    0,    0,  930,  930,    0,
  930,    0,    0,    0,    0,    0,  930,    0,    0,  930,
    0,  930,  930,  930,  930,    0,  930,    0,    0,    0,
  219,    0,    0,    0,  930,  930,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  930,  930,  219,  930,    0,
    0,    0,    0,    0,    0,  930,  930,  930,  930,  930,
    0,  930,    0,    0,  930,  930,  930,  930,  930,  219,
    0,    0,  930,  930,    0,    0,    0,  930,  930,  930,
  930,  930,  930,  930,  930,    0,  930,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  930,    0,    0,    0,
    0,    0,    0,    0,  219,  219,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  929,    0,  929,  929,    0,
    0,  219,    0,    0,  929,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  929,    0,
    0,    0,  931,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  929,    0,    0,    0,    0,  929,  929,    0,
    0,  929,    0,    0,    0,    0,    0,    0,  929,    0,
  929,  929,    0,    0,  929,    0,    0,    0,  929,    0,
  929,    0,  929,  931,  929,  931,  931,  931,    0,  929,
  929,    0,  929,    0,    0,    0,    0,    0,    0,    0,
    0,  931,  931,  931,  931,    0,    0,  929,  929,    0,
  929,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  929,    0,  929,    0,  929,  929,  929,    0,  929,
    0,    0,  929,  929,    0,  931,    0,    0,    0,    0,
    0,    0,  929,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  929,    0,    0,  929,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  929,    0,    0,    0,    0,    0,
  929,  929,    0,    0,    0,  929,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  929,  929,    0,  929,
    0,    0,    0,    0,    0,  929,    0,    0,  929,    0,
  929,  929,  929,  929,    0,  929,    0,    0,    0,    0,
    0,    0,    0,  929,  929,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  929,  929,    0,  929,    0,    0,
    0,    0,    0,    0,  929,  929,  929,  929,  929,    0,
  929,    0,  868,  929,  929,  929,  929,  929,    0,    0,
    0,  929,  929,    0,    0,    0,  929,  929,  929,  929,
  929,  929,  929,  929,    0,  929,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  929,    0,    0,    0,    0,
    0,    0,    0,  868,    0,    0,  868,    0,    0,    0,
    0,    0,    0,    0,  931,    0,  931,  931,    0,    0,
    0,  868,    0,  931,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  931,    0,    0,
    0,  934,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  931,    0,    0,    0,    0,  931,  931,    0,    0,
  931,    0,    0,    0,    0,    0,    0,  931,    0,  931,
  931,    0,    0,  931,    0, 1065,    0,  931, 1065,  931,
    0,  931,  934,  931,  934,  934,  934,    0,  931,  931,
    0,  931,    0, 1067,    0, 1065,    0,    0, 1065,    0,
  934,  934,  934,  934,    0,    0,  931,  931,    0,  931,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  931,    0,  931,    0,  931,  931,  931,    0,  931,    0,
    0,  931,  931,    0,  934,    0,    0,    0, 1065,    0,
    0,  931,    0,    0, 1065, 1065, 1065,    0, 1065, 1065,
 1065,    0, 1065,    0,  931,    0,    0,  931,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1065,    0, 1065,  931,    0,    0,    0,    0,    0,  931,
  931,    0,    0, 1129,  931,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  931,  931,    0,  931,    0,
    0, 1065,    0,    0,  931,    0,    0,  931,    0,  931,
  931,  931,  931,    0,  931,    0,    0,    0, 1065,    0,
    0,    0,  931,  931,  868,    0,    0,    0,    0,    0,
    0,    0,    0,  931,  931,    0,  931,    0,    0,    0,
 1065,    0,    0,  931,  931,  931,  931,  931,    0,  931,
  737,    0,  931,  931,  931,  931,  931,    0,    0,    0,
  931,  931,    0,    0,    0,  931,  931,  931,  931,  931,
  931,  931,  931,    0,  931, 1065, 1065,  868,    0,    0,
  868,    0,    0,    0,  931,    0,    0,  868,    0,  868,
    0,  737, 1065,  868,  737,    0,    0,    0,  868,    0,
    0,  868,    0,  934,    0,  934,  934,    0,    0,  737,
    0,    0,  934,    0,    0,    0,  868,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  934,    0,    0,    0,
  868,    0,    0,    0,    0,    0,    0,    0,  868,    0,
  934,  868,  868,    0,    0,  934,  934,    0,    0,  934,
    0,  868,    0,    0,    0,    0,  934,    0,  934,  934,
    0,    0,  934,    0,    0,    0,  934,  868,  934,    0,
  934,    0,  934,    0,    0,    0,    0,  934,  934,    0,
  934,    0,    0,    0,    0,    0,    0,    0,    0,  868,
    0,    0,    0,    0,  868,  934,  934,    0,  934,    0,
    0,    0,    0,    0,    0,    0,  868,    0,  868,  934,
    0,  934,    0,  934,  934,  934,    0,  934,    0,    0,
  934,  934, 1102,    0,    0,    0,    0,    0,    0,    0,
  934,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  934,    0,    0,  934,    0,    0,    0,
    0,    0,    0,    0,    0,  868,    0,    0,    0,    0,
    0,    0,  934,    0,    0,  708,    0,    0,  934,  934,
    0,    0,    0,  934,    0,  868,    0,    0,    0,  868,
    0,    0,    0,    0,  934,  934,    0,  934,    0,    0,
    0,    0,    0,  934,    0,    0,  934,    0,  934,  934,
  934,  934,    0,  934,    0,  708,  708,    0,    0,  708,
    0,  934,  934,    0,  737,    0,    0,    0,    0,    0,
    0,    0,  934,  934,  708,  934,    0,    0,    0,    0,
    0,    0,  934,  934,  934,  934,  934,    0,  934,    0,
    0,  934,  934,  934,  934,  934,    0,    0,    0,  934,
  934,    0,   14,    0,  934,  934,  934,  934,  934,  934,
  934,  934,    0,  934,    0,  737,    0,    0,  737,    0,
    0,    0,    0,  934,    0,  737,    0,  737,    0,    0,
    0,  737,    0,    0,    0,    0,  737,    0,    0,  737,
    0,    0,    0,    0,    0,  115,    0,    0,  116,    0,
  117,  118,  119,  120,  737,  121,    0,    0,    0,    0,
    0,    0,    0,  122,  123,    0,    0,    0,  737,    0,
    0,    0,    0,    0,  124,  125,  737,  126,    0, 1129,
  737,    0,    0,    0,  127,  128,    0,  129,  130,  737,
  131,    0,    0,  132,  133,  134,  135,  136,    0,    0,
    0,  137,  138,    0,    0,  737,    0,  139,  140,  141,
    0,  142,  143,  144,    0,  145,    0,    0,    0,    0,
    0,    0,    0, 1101,  735,  146,    0,    0,    0,    0,
    0,    0,  737,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  737,    0,  737,    0,    0,    0,
    0,    0,  737,    0,    0,  737,    0,  737,  737,  737,
  737,    0,  737,    0,    0,  735,    0,    0,  735,    0,
  737,  737,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  737,  737,  735,  737,    0,    0,    0,    0,    0,
    0,  737,  737,  737,  737,  737,    0,  737,    0,    0,
  737,  737,  737,  737,  737,    0,    0,    0,  737,  737,
    0,    0,    0,  737,  737,  737,  737,  737,  737,  737,
  737,    0,  737,    0,    0,    0,    0,    0,    0,    0,
  708,    0,  737,  708,    0,    0,    0,    0,    0,    0,
  708,    0,  708,    0,    0,    0,  708,    0,    0,    0,
    0,  708,    0,    0,  708,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  708,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115,  708,    0,  116,    0,  117,  118,  119,
  120,  708,  121,    0,    0,  708,    0,    0,    0,    0,
  122,  123,    0,    0,  708,    0,    0,    0,    0,    0,
    0,  124,  125,    0,  126,    0,    0,    0,    0,    0,
  708,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,  708,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,  708,
    0,  708,  146,    0,    0,    0,    0,  708,    0,    0,
  708,    0,  708,  708,  708,  708,    0,  708,    0,    0,
    0,    0,    0,    0,    0,  708,  708,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  708,  708,    0,  708,
    0,    0,    0,    0,    0,    0,  708,  708,  708,  708,
  708,    0,  708,    0,    0,  708,  708,  708,  708,  708,
    0,    0,    0,  708,  708,  910,    0,    0,  708,  708,
  708,  708,  708,  708,  708,  708,    0,  708,    0,  735,
    0,    0,  735,    0,    0,    0,    0,  708,    0,  735,
    0,  735,    0,    0,    0,  735,    0,    0,    0,    0,
  735,    0,    0,  735,    0,    0,  910, 1104, 1104,  910,
 1104, 1104, 1104,    0,    0,    0,    0,    0,  735,    0,
    0,    0,    0,    0,  910, 1104, 1104, 1104,    0,    0,
    0,  115,  735,    0,  116,    0,  117,  118,  119,  120,
  735,  121,    0,    0,  735,    0,    0,    0,    0,  122,
  123,    0,    0,  735,    0,    0, 1104,    0,    0,    0,
  124,  125,    0,  126,    0,    0,    0,    0,    0,  735,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,  735,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,  735,    0,
  735,  146,    0,    0,    0,    0,  708,    0,    0,  708,
    0,  708,  708,  708,  708,    0,  708,    0,    0,    0,
    0,    0,    0,    0,  708,  708,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  708,  708,    0,  708,    0,
    0,    0,    0,    0,    0,  708,  708,  735,  708,  708,
    0,  708,  911,    0,  708,  708,  708,  708,  708,    0,
    0,    0,  708,  708,    0,    0,    0,  735,  708,  708,
  708,  735,  708,  708,  708,    0,  708,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  708,    0,    0,    0,
    0,    0,    0,  911, 1105, 1105,  911, 1105, 1105, 1105,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  911, 1105, 1105, 1105,    0,    0,  910,    0,    0,
    0,    0,    0,    0,    0,    0, 1104,    0,    0,    0,
    0,    0, 1104,    0,    0,    0,    0, 1104,    0,    0,
 1104,    0,    0, 1105,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  910,    0,    0,  910,    0,    0, 1104,    0,    0,    0,
  910,    0,  910,    0, 1104,    0,  910,    0,    0,    0,
    0,  910, 1104,    0,  910,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  912,    0,    0,    0,    0,  910,
 1104,    0, 1104,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  910,    0, 1104,    0, 1104, 1104, 1104,
    0,  910,    0,    0,  910,  910,    0,    0,    0,    0,
    0,    0,    0,    0,  910,  912, 1106, 1106,  912, 1106,
 1106, 1106,    0,    0,    0,    0,    0,    0,    0,    0,
  910,    0,    0,  912, 1106, 1106, 1106,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1104,    0,    0,    0,
    0,    0,  910,    0,    0,    0,    0,  910,    0,    0,
    0,    0,    0,    0,    0, 1106,    0,    0,    0,  910,
    0,  910,  894,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  911,    0,    0,    0,    0,    0,
    0,    0,    0, 1105,    0,    0,    0,    0,    0, 1105,
    0,    0,    0,    0, 1105,    0,    0, 1105,    0,    0,
    0,    0,    0,  894,    0,    0,  894,    0,  910,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  894,    0,    0,    0,    0,    0,  911,  910,    0,
  911,    0,  910, 1105,    0,    0,    0,  911,    0,  911,
    0, 1105,    0,  911,    0,    0,    0,    0,  911, 1105,
    0,  911,    0,    0,    0,    0,  375,    0,    0,    0,
    0,  871,    0,    0,    0,    0,  911, 1105,    0, 1105,
    0,    0,  376,    0,    0,    0,    0,    0,    0,    0,
  911,    0, 1105,  377, 1105, 1105, 1105,    0,  911,    0,
  378,  911,  911,    0,  379,  380,    0,    0,    0,    0,
    0,  911,  871,  381,    0,  871,    0,    0,    0,    0,
    0,    0,    0,    0,  382,    0,    0,  911,    0,    0,
  871,    0,    0,    0,    0,    0,  912,    0,    0,    0,
    0,    0,    0, 1105,    0, 1106,    0,    0,    0,  911,
    0, 1106,    0,    0,  911,    0, 1106,    0,    0, 1106,
    0,    0,    0,    0,    0,  869,  911,    0,  911,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  383,    0,
    0,    0,    0,    0,    0,    0,  384,    0,    0,  912,
    0,    0,  912,    0,    0, 1106,    0,    0,    0,  912,
    0,  912,    0, 1106,    0,  912,  869,    0,    0,  869,
  912, 1106,    0,  912,    0,  911,    0,  385,    0,    0,
    0,  386,    0,    0,  869,    0,    0,    0,  912, 1106,
    0, 1106,  387,    0,  894,  911,    0,    0,    0,  911,
    0,    0,  912,    0, 1106,    0, 1106, 1106, 1106,    0,
  912,  388,  389,  912,  912,    0,    0,    0,    0,    0,
    0,    0,    0,  912,    0,    0,    0,    0,  870,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  912,
    0,    0,    0,    0,    0,    0,    0,  894,    0,    0,
  894,    0,    0,    0,    0, 1106,    0,  894,    0,  894,
    0,  912,    0,  894,    0,    0,  912,    0,  894,  870,
  390,  894,  870,  391,    0,    0,    0,    0,  912,    0,
  912,    0,    0,    0,    0,    0,  894,  870,    0,    0,
    0,    0,    0,  871,    0,    0,    0,    0,    0,    0,
  894,    0,    0,    0,    0,    0,    0,    0,  894,    0,
    0,  894,  894,    0,    0,    0,    0,    0,    0,    0,
    0,  894,  902,    0,    0,    0,    0,  912,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  894,    0,    0,
    0,    0,    0,    0,    0,    0,  871,  912,    0,  871,
  864,  912,    0,    0,    0,    0,  871,    0,  871,  894,
    0,    0,  871,  902,  894,    0,  902,  871,    0,    0,
  871,    0,    0,    0,    0,    0,  894,    0,  894,    0,
    0,  902,    0,    0,    0,  871,    0,  869,    0,    0,
    0,  864,    0,    0,  864,    0,    0,    0,    0,  871,
    0,    0,    0,    0,    0,    0,    0,  871,    0,  864,
  871,  871,    0,    0,    0,    0,    0,    0,    0,    0,
  871,    0,    0,    0,    0,  894,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  871,    0,    0,    0,
  869,    0,    0,  869,  865,  894,    0,    0,    0,  894,
  869,    0,  869,    0,    0,    0,  869,    0,  871,    0,
    0,  869,    0,  871,  869,    0,    0,    0,    0,    0,
    0,    0,  866,    0,    0,  871,    0,  871,    0,  869,
    0,    0,    0,    0,    0,  865,    0,    0,  865,    0,
  870,    0,    0,  869,    0,    0,    0,    0,    0,    0,
    0,  869,    0,  865,  869,  869,    0,    0,    0,    0,
    0,    0,    0,  866,  869,    0,  866,    0,    0,    0,
    0,    0,    0,    0,  871,    0,    0,    0,    0,    0,
  869,  866,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  870,  871,    0,  870,  901,  871,    0,
    0,    0,  869,  870,    0,  870,    0,  869,    0,  870,
    0,    0,    0,    0,  870,    0,    0,  870,    0,  869,
    0,  869,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  870,    0,  902,    0,    0,    0,  901,    0,
    0,  901,    0,    0,    0,    0,  870,    0,    0,    0,
    0,    0,    0,    0,  870,    0,  901,  870,  870,    0,
    0,    0,  864,    0,    0,    0,    0,  870,  869,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  870,    0,    0,    0,  902,  869,    0,
  902,  895,  869,    0,    0,    0,    0,  902,    0,  902,
    0,    0,    0,  902,    0,  870,    0,    0,  902,    0,
  870,  902,    0,    0,    0,  864,    0,    0,  864,  903,
    0,    0,  870,    0,  870,  864,  902,  864,    0,    0,
    0,  864,  895,    0,    0,  895,  864,    0,    0,  864,
  902,    0,    0,    0,    0,    0,    0,    0,  902,    0,
  895,  902,  902,    0,  864,    0,  865,    0,    0,    0,
  903,  902,    0,  903,    0,    0,    0,    0,  864,    0,
    0,  870,    0,    0,    0,    0,  864,  902,  903,  864,
  864,    0,    0,    0,  866,    0,    0,    0,    0,  864,
    0,  870,    0,    0,    0,  870,    0,    0,    0,  902,
    0,    0,    0,    0,  902,  864,    0,    0,    0,  865,
    0,    0,  865,  904,    0,    0,  902,    0,  902,  865,
    0,  865,    0,    0,    0,  865,    0,  864,    0,    0,
  865,    0,  864,  865,    0,    0,    0,  866,    0,    0,
  866,    0,    0,    0,  864,    0,  864,  866,  865,  866,
    0,    0,    0,  866,  904,    0,    0,  904,  866,  901,
    0,  866,  865,    0,    0,  902,    0,    0,    0,    0,
  865,    0,  904,  865,  865,    0,  866,    0,    0,    0,
    0,    0,    0,  865,    0,  902,    0,    0,  892,  902,
  866,    0,    0,  864,    0,    0,    0,    0,  866,  865,
    0,  866,  866,    0,    0,    0,    0,    0,    0,    0,
    0,  866,  901,  864,    0,  901,  890,  864,    0,    0,
    0,  865,  901,    0,  901,    0,  865,  866,  901,  892,
    0,    0,  892,  901,    0,    0,  901,    0,  865,    0,
  865,    0,    0,    0,    0,    0,    0,  892,    0,  866,
    0,  901,    0,  895,  866,    0,    0,  890,    0,    0,
  890,    0,    0,    0,    0,  901,  866,    0,  866,    0,
    0,    0,    0,  901,    0,  890,  901,  901,    0,    0,
    0,  903,    0,    0,    0,    0,  901,  865,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  901,    0,    0,    0,  895,  865,    0,  895,
  896,  865,    0,    0,    0,  866,  895,    0,  895,    0,
    0,    0,  895,    0,  901,    0,    0,  895,    0,  901,
  895,    0,    0,    0,  903,  866,    0,  903,  905,  866,
    0,  901,    0,  901,  903,  895,  903,    0,    0,    0,
  903,  896,    0,    0,  896,  903,    0,    0,  903,  895,
    0,    0,    0,    0,    0,    0,    0,  895,    0,  896,
  895,  895,    0,  903,    0,  904,    0,    0,    0,  905,
  895,    0,  905,    0,    0,    0,    0,  903,    0,    0,
  901,    0,    0,    0,    0,  903,  895,  905,  903,  903,
    0,    0,    0,    0,    0,    0,    0,    0,  903,    0,
  901,    0,    0,  891,  901,    0,    0,    0,  895,    0,
    0,    0,    0,  895,  903,    0,    0,    0,  904,    0,
    0,  904,  732,    0,    0,  895,    0,  895,  904,    0,
  904,    0,    0,    0,  904,    0,  903,    0,    0,  904,
  892,  903,  904,    0,  891,    0,    0,  891,    0,    0,
    0,    0,    0,  903,    0,  903,    0,  904,    0,    0,
    0,    0,  891,  732,    0,    0,  732,    0,  890,    0,
    0,  904,    0,    0,  895,    0,    0,    0,    0,  904,
    0,  732,  904,  904,    0,    0,    0,    0,    0,    0,
    0,    0,  904,  892,  895,    0,  892,  893,  895,    0,
    0,    0,  903,  892,    0,  892,    0,    0,  904,  892,
    0,    0,    0,    0,  892,    0,    0,  892,    0,    0,
    0,  890,  903,    0,  890,  897,  903,    0,    0,    0,
  904,  890,  892,  890,    0,  904,    0,  890,  893,    0,
    0,  893,  890,    0,    0,  890,  892,  904,    0,  904,
    0,    0,    0,    0,  892,    0,  893,  892,  892,    0,
  890,    0,  896,    0,    0,    0,  897,  892,    0,  897,
    0,    0,    0,    0,  890,    0,    0,    0,    0,    0,
    0,    0,  890,  892,  897,  890,  890,    0,    0,    0,
  905,    0,    0,    0,    0,  890,  904,    0,    0,    0,
    0,    0,    0,    0,    0,  892,    0,    0,    0,    0,
  892,  890,    0,    0,    0,  896,  904,    0,  896,  842,
  904,    0,  892,    0,  892,  896,    0,  896,    0,    0,
    0,  896,    0,  890,    0,    0,  896,    0,  890,  896,
    0,    0,    0,  905,    0,    0,  905,    0,    0,    0,
  890,    0,  890,  905,  896,  905,    0,    0,    0,  905,
  842,    0,    0,  842,  905,  891,    0,  905,  896,    0,
    0,  892,    0,    0,    0,    0,  896,    0,  842,  896,
  896,    0,  905,    0,    0,    0,    0,    0,    0,  896,
    0,  892,    0,    0,    0,  892,  905,    0,    0,  890,
    0,    0,    0,    0,  905,  896,    0,  905,  905,    0,
    0,    0,    0,    0,    0,    0,    0,  905,  891,  890,
    0,  891,  719,  890,    0,    0,    0,  896,  891,    0,
  891,    0,  896,  905,  891,    0,    0,  732,    0,  891,
  732,    0,  891,    0,  896,    0,  896,  732,    0,  732,
    0,    0,    0,  732,    0,  905,    0,  891,  732,  893,
  905,  732,  719,  719,    0,    0,  719,    0,    0,    0,
    0,  891,  905,    0,  905,    0,  732,    0,    0,  891,
    0,  719,  891,  891,    0,    0,    0,  897,    0,    0,
  732,    0,  891,  896,    0,    0,    0,    0,  732,    0,
    0,    0,  732,    0,    0,    0,    0,    0,  891,    0,
    0,  732,  893,  896,    0,  893,    0,  896,    0,    0,
    0,  905,  893,    0,  893,    0,    0,  732,  893,    0,
  891,    0,    0,  893,    0,  891,  893,    0,    0,  760,
  897,  905,  513,  897,  514,  905,    0,  891,    0,  891,
  897,  893,  897,    0,  732,    0,  897,  516,    0,    0,
    0,  897,  334,    0,  897,  893,  732,    0,  732,    0,
    0,    0,    0,  893,    0,    0,  893,  893,    0,  897,
    0,    0,    0,    0,    0,    0,  893,    0,    0,    0,
    0,    0,    0,  897,    0,    0,  891,    0,    0,    0,
    0,  897,  893,    0,  897,  897,    0,    0,    0,    0,
    0,    0,    0,    0,  897,  732,  891,    0,    0,    0,
  891,    0,    0,    0,  893,    0,    0,    0,  313,  893,
  897,    0,    0,    0,  842,  732,    0,  842,    0,  732,
    0,  893,    0,  893,  842,    0,  842,    0,    0,    0,
  842,    0,  897,    0,    0,  842,    0,  897,  842,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  897,
    0,  897,    0,  842,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  842,    0,    0,
  893,    0,    0,    0,    0,  842,    0,    0,  842,  842,
    0, 1102,    0,    0,    0,    0,    0,    0,  842,  219,
  893,    0,  219,    0,  893,    0,    0,    0,  897,    0,
    0,    0,    0,    0,  842,  219,    0,    0,    0,  219,
    0,    0,  219,    0,    0,    0,    0,  719,  897,    0,
  719,    0,  897,    0,    0,    0,  842,  719,    0,  719,
    0,  842,    0,  719,    0,    0,    0,    0,  719,    0,
    0,  719,    0,  842,    0,  842,  469,    0,    0,    0,
    0,    0,  219,  470,    0,    0,  719,    0,  219,  219,
  219,    0,  219,  219,  219,  471,  219,    0,    0,    0,
  719,  472,  473,    0,    0,    0,    0,    0,  719,    0,
    0,    0,  719,    0,  219,    0,  219,    0,    0,    0,
  754,  719,  842,    0,    0,  760,    0,    0,  513,  474,
  514,    0,  475,    0,    0,    0,    0,  719,    0,    0,
    0,    0,  842,  516,    0,  219,  842,    0,  334,    0,
   81,    0,    0,    0,    0,    0,    0,  476,    0,  477,
  478,    0,  219,    0,  719,    0,    0,    0,    0,  479,
    0,    0,    0,  480,    0,    0,  719,    0,  719,  915,
    0, 1005,    0,    0,  219,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   14,  219,
  219,    0,  756,    0,    0,  719,    0,    0,  481,    0,
    0,    0,  705,    0,  482,    0,  219,    0,    0,    0,
    0,    0,    0,    0,  483,  719,    0,    0,    0,  719,
    0,    0,    0,    0,    0,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0,    0,    0,    0,    0,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,    0,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136, 1006, 1007, 1008,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,    0,  507,  508,  509,    0,  510,  511,    0,
  512,  146,    0,    0,  760,    0,    0,  513,    0,  514,
  115,    0,  469,  116,    0,  117,  118,  119,  120,  470,
  121,    0,  516,    0,    0,    0,    0,  334,  122,  123,
    0,  471,    0,    0,  753,    0,    0,  472,  473,  124,
  125,    0,  126,    0,    0,    0,    0,    0,    0,  127,
  128,    0,  129,  130,    0,  131,  754,    0,  132,  133,
  134,  135,  136,    0,    0,  474,  137,  138,  475,    0,
    0,    0,  139,  140,  141,    0,  142,  143,  144,    0,
  145,  115,    0,    0,  116,    0,  117,  118,  119,  120,
  146,  121,    0,  476,    0,  477,  478,    0,    0,  122,
  123,    0,    0,    0,    0,  479,    0,    0,    0,  480,
  124,  125,    0,  126,    0,    0,    0,  755,    0,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,   85,  756,    0,
    0,  146,    0,    0,  481,  705,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,  469,  499,  500,    0,  501,  502,  503,  470,  129,
  130,  504,  131,    0,    0,  132,  133,  757,  758,  759,
  471,    0,    0,  137,  138,    0,  472,  473,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,    0,  507,
  508,  509,    0,  510,  511,  754,  512,  146,  760,    0,
    0,  513,    0,  514,  474,    0,    0,  475,    0,    0,
    0,    0,    0,    0,    0,    0,  516,    0,    0,    0,
  708,  334,    0,  708,    0,  708,  708,  708,  708,    0,
  708,    0,  476,    0,  477,  478,    0,    0,  708,  708,
    0,    0,    0,    0,  479,    0,    0,    0,  480,  708,
  708,    0,  708,    0,    0,    0, 1001,    0,    0,  708,
  708,    0,  708,  708,    0,  708,    0,    0,  708,  708,
  708,  708,  708,    0,    0,    0,  708,  708,    0,    0,
    0,    0,  708,  708,  708,    0,  708,  708,  708,    0,
  708,    0,    0,    0,    0,    0,    0,  756,    0,    0,
  708,    0,    0,  481,    0,    0,    0,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  483,
  516,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  757,  758,  759,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,    0,  507,  508,
  509,    0,  510,  511,    0,  512,  146,  586,    0,    0,
  513,    0,  514,    0,    0,  469,    0,    0,    0,    0,
    0,    0,  470,    0,    0,  516,    0,    0,    0,    0,
  334,    0,    0,    0,  471,    0,    0,    0,    0,    0,
  472,  473,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  754,
    0,    0,    0,    0,    0,    0,    0,    0,  474,    0,
    0,  475,   84,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,    0,  476,    0,  477,  478,
    0,    0,    0,  124,  125,    0,  126,    0,  479,    0,
    0,    0,  480,  127,  128,    0,  129,  130,    0,  131,
  755,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,  756,    0,    0,    0,    0,    0,  481,    0,    0,
    0,    0,    0,  482,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  484,  485,  486,
  487,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  488,    0,    0,    0,    0,    0,    0,  122,  123,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  124,
  498,    0,  126,    0,  469,  499,  500,    0,  501,  502,
  503,  470,  129,  130,  504,  131,    0,    0,  132,  133,
  757,  758,  759,  471,    0,    0,  137,  138,    0,  472,
  473,    0,  139,  505,  141,    0,  506,  143,  144,    0,
  145,    0,  507,  508,  509,    0,  510,  511,  760,  512,
  146,  513,    0,  514,    0,    0,    0,  474,    0,    0,
  475,    0,    0,    0,    0,    0,  516,    0,    0,    0,
    0,  334,    0,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,  476,    0,  477,  478,    0,
    0,    0,  122,  123,    0,    0,    0,  479,    0,    0,
    0,  480,    0,  124,  125,    0,  126,    0,    0,    0,
    0,    0,    0,  127,  128,    0,  129,  130,    0,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,  481,    0,    0,    0,
    0,    0,  482,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,  485,  486,  487,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  488,    0,    0,    0,    0,    0,    0,  122,  123,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  124,  498,
    0,  126,    0,    0,  499,  500,    0,  501,  502,  503,
    0,  129,  130,  504,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  505,  141,    0,  506,  143,  144, 1076,  145,
    0,  507,  508,  509, 1077,  510,  511, 1078,  512,  146,
    0,    0,    0,    0,    0,  469,    0, 1081,  822,  820,
    0,  821,  470,  823,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  471,    0, 1026, 1025, 1027,    0,
  472,  473,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  586,    0,    0,  513,  754,
  514,    0,    0,    0,    0,    0,    0,    0,  474,    0,
    0,  475,    0,  516,  822,  820,    0,  821,  334,  823,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1026, 1025, 1027,    0,  476,    0,  477,  478,
    0,    0,    0,    0,    0,    0,    0,    0,  479,    0,
    0,    0,  480,    0,    0,    0,    0,    0,    0,    0,
 1001,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  756,    0,    0,    0,    0,    0,  481,    0,    0,
    0,    0,    0,  482,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  484,  485,  486,
  487,  255,  256,  116,    0,  117,  118,  119,  120,    0,
  121,  488,    0,    0,    0,    0,    0,    0,  122,  123,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  124,
  498,    0,  126,    0,    0,  499,  500,    0,  501,  502,
  503,    0,  129,  130,  504,  131,    0, 1013,  132,  133,
  134,  135,  136,  818,    0,    0,  137,  138,  819,    0,
    0, 1014,  139,  505,  141,    0,  506,  143,  144,    0,
  145,    0,  507,  508,  509, 1268,  510,  511, 1269,  512,
  146,    0,  469,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,    0,    0, 1015,    0,    0,
    0,  471,    0, 1013,    0, 1016,    0,  472,  473,  818,
    0,    0,    0, 1017,  819,    0,    0, 1014,    0,    0,
    0,    0,  586,    0,    0,  513,    0,  514,    0,    0,
    0, 1018,    0, 1019,    0,  474,    0,    0,  475,    0,
  516,    0,    0,    0,    0,  334, 1020,    0, 1021, 1022,
 1023,    0,    0, 1015,    0,    0,    0,    0,    0,    0,
    0, 1016,    0,  476,    0,  477,  478,    0,    0, 1017,
    0,    0,    0,    0,    0,  479,    0,    0,    0,  480,
    0,    0,    0,    0,    0,    0,    0, 1018,    0, 1019,
    0,    0,    0,    0,    0,    0,    0, 1024,    0,    0,
    0,    0, 1020,    0, 1021, 1022, 1023,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1270,    0,  481,    0,    0,    0,    0,    0,
  482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  483,    0,    0, 1024,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,  485,  486,  487,  255,  256,
  116,    0,  117,  118,  119,  120,    0,  121,  488,    0,
    0,    0,    0,    0,    0,  122,  123,  489,  490,  491,
  492,  493,  494,  495,  496,  497,  124,  498,    0,  126,
    0,    0,  499,  500,    0,  501,  502,  503,    0,  129,
  130,  504,  131,    0,    0,  132,  133,  134,  135,  136,
    0,    0,    0,  137,  138,    0,    0,    0,    0,  139,
  505,  141,    0,  506,  143,  144,    0,  145,    0,  507,
  508,  509, 1279,  510,  511, 1269,  512,  146,    0,  469,
  694,    0,    0,  695,    0,    0,  470,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  471,    0,
  696,    0,    0,  697,  472,  473,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  586,
    0,    0,  513,    0,  514,    0,    0,    0,    0,    0,
    0,    0,  474,    0,    0,  475,    0,  516,    0,    0,
    0,    0,  334,  698,    0,    0,    0,    0,    0,  699,
  700,  701,    0,  702,  703,  704,    0,  705,    0,    0,
  476,    0,  477,  478,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,    0,  706,  480,  707,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  708,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  709,    0,    0,    0,    0,    0, 1270,
    0,  481,    0,    0,    0,    0,    0,  482,    0,    0,
    0,    0,    0,    0,    0,  710,    0,  483,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,  485,  486,  487,  255,  256,  116,    0,  117,
  118,  119,  120,    0,  121,  488,    0,    0,    0,    0,
  711,  712,  122,  123,  489,  490,  491,  492,  493,  494,
  495,  496,  497,  124,  498,    0,  126,  713,    0,  499,
  500,    0,  501,  502,  503,    0,  129,  130,  504,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  505,  141,    0,
  506,  143,  144,    0,  145,    0,  507,  508,  509, 1283,
  510,  511, 1269,  512,  146,    0,  469,  148,    0,    0,
  148,    0,    0,  470,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,  148,    0,    0,
  148,  472,  473,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  586,    0,    0,  513,
    0,  514,    0,    0,    0,    0,    0,    0,    0,  474,
    0,    0,  475,    0,  516,    0,    0,    0,    0,  334,
  148,    0,    0,    0,    0,    0,  148,  148,  148,    0,
  148,  148,  148,    0,  148,    0,    0,  476,    0,  477,
  478,    0,    0,    0,    0,    0,    0,    0,    0,  479,
    0,    0,  148,  480,  148,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  148,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  148,    0,    0,    0,    0,    0, 1270,    0,  481,    0,
    0,    0,    0,    0,  482,    0,    0,    0,    0,    0,
    0,    0,  148,    0,  483,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  484,  485,
  486,  487,  255,  256,  116,    0,  117,  118,  119,  120,
    0,  121,  488,    0,    0, 1365,    0,  148,  148,  122,
  123,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  124,  498,    0,  126,  148,    0,  499,  500,    0,  501,
  502,  503,    0,  129,  130,  504,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  505,  141,    0,  506,  143,  144,
    0,  145,    0,  507,  508,  509, 1291,  510,  511, 1269,
  512,  146,    0,  469,    0, 1366,    0,    0,    0,    0,
  470,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,    0,    0,    0,  472,  473,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1367,    0,    0,  586,    0, 1368,  513,    0,  514,    0,
    0, 1369,    0,    0,    0,    0,  474,    0,    0,  475,
    0,  516,  115,    0,    0,  116,  334,  117,  118,  119,
 1370,    0,  121,    0,    0,    0,    0,    0,    0,    0,
  122,  123,    0,    0,  476,    0,  477,  478,    0,    0,
    0,  124,  125,    0,  126,    0,  479,    0,    0,    0,
  480,  127,  128,    0,  129,  130,    0,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  140,  141,    0,  142,  143,
  144,    0,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1270,    0,  481,    0,    0,    0,    0,
    0,  482,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  484,  485,  486,  487,  255,
  256,  116,    0,  117,  118,  119,  120,    0,  121,  488,
    0,    0,    0,    0,    0,    0,  122,  123,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  124,  498,    0,
  126,    0,    0,  499,  500,    0,  501,  502,  503,    0,
  129,  130,  504,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,    0,    0,    0,    0,
  139,  505,  141,    0,  506,  143,  144,    0,  145,    0,
  507,  508,  509, 1294,  510,  511, 1269,  512,  146,    0,
  469,    0,    0,    0,    0,    0,    0,  470,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  471,
    0,    0,    0,    0,    0,  472,  473,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  586,    0,  567,  513,    0,  514,    0,    0,    0,    0,
    0,    0,    0,  474,    0,    0,  475,    0,  516,  115,
    0,    0,  116,  334,  117,  118,  119,  120,    0,  121,
    0,    0,    0,    0,    0,    0,    0,  122,  123,    0,
    0,  476,    0,  477,  478,    0,    0,    0,  124,  125,
    0,  126,    0,  479,    0,    0,    0,  480,  127,  128,
    0,  129,  130,    0,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  140,  141,    0,  142,  143,  144,    0,  145,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1270,    0,  481,    0,    0,    0,    0,    0,  482,    0,
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
 1299,  510,  511, 1269,  512,  146,    0,  469,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0, 1270,    0,  481,
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
  144,    0,  145,    0,  507,  508,  509, 1302,  510,  511,
 1269,  512,  146,    0,  469,    0,    0,    0,    0,    0,
    0,  470,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  471,    0,    0,    0,    0,    0,  472,
  473,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  586,    0,    0,  513,    0,  514,
    0,    0,    0,    0,    0,    0,    0,  474,    0,    0,
  475,    0,  516,   28,    0,    0,   28,  334,   28,   28,
   28,   28,    0,   28,    0,    0,    0,    0,    0,    0,
    0,   28,   28,    0,    0,  476,    0,  477,  478,    0,
    0,    0,   28,   28,    0,   28,    0,  479,    0,    0,
    0,  480,   28,   28,    0,   28,   28,    0,   28,    0,
    0,   28,   28,   28,   28,   28,    0,    0,    0,   28,
   28,    0,    0,    0,    0,   28,   28,   28,    0,   28,
   28,   28,    0,   28,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   28,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1270,    0,  481,    0,    0,    0,
    0,    0,  482,  662,    0,    0,    0,    0,    0,    0,
    0,    0,  483,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  484,  485,  486,  487,
  255,  256,  116,    0,  117,  118,  119,  120,    0,  121,
  488,    0,    0,    0,    0,    0,    0,  122,  123,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  124,  498,
    0,  126,    0,    0,  499,  500,    0,  501,  502,  503,
    0,  129,  130,  504,  131,    0,    0,  132,  133,  134,
  135,  136,    0,    0,    0,  137,  138,    0,    0,    0,
    0,  139,  505,  141,    0,  506,  143,  144,    0,  145,
    0,  507,  508,  509, 1305,  510,  511, 1269,  512,  146,
    0,  469,    0,    0,    0,    0,    0,    0,  470,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  471,    0,    0,    0,    0,    0,  472,  473,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  692,    0,  692,
  692,    0,  692,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  474,  692,    0,  475,    0,    0,
  692,  115,    0,    0,  116,    0,  117,  118,  119,  120,
    0,  121,    0,    0,    0,    0,    0,    0,    0,  122,
  123,    0,  476,    0,  477,  478,    0,    0,    0,    0,
  124,  125,    0,  126,  479,    0,    0,    0,  480,    0,
  127,  128,    0,  129,  130,    0,  131,    0,    0,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,    0,
    0,    0,    0,  139,  140,  141,    0,  142,  143,  144,
    0,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  146,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1270,    0,  481,    0,    0,  668,    0,    0,  482,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  483,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  484,  485,  486,  487,  255,  256,  116,
    0,  117,  118,  119,  120,    0,  121,  488,    0,    0,
    0,    0,    0,    0,  122,  123,  489,  490,  491,  492,
  493,  494,  495,  496,  497,  124,  498,    0,  126,    0,
    0,  499,  500,    0,  501,  502,  503,    0,  129,  130,
  504,  131,    0,    0,  132,  133,  134,  135,  136,    0,
    0,    0,  137,  138,    0,    0,    0,    0,  139,  505,
  141,    0,  506,  143,  144,    0,  145,  692,  507,  508,
  509,    0,  510,  511,  692,  512,  146,    0,    0,    0,
    0,  692,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  692,    0,    0,    0,    0,    0,  692,
  692,    0,    0,    0,    0,    0,    0,  692,    0,    0,
  686,    0,  686,  686,    0,  686,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  692,  686,    0,
  692,    0,    0,  686,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,    0,  692,    0,  692,  692,    0,
    0,    0,    0,  124,  125,    0,  126,  692,    0,    0,
    0,  692,    0,  127,  128,    0,  129,  130,    0,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  140,  141,    0,
  142,  143,  144,    0,  145,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  692,    0,    0,    0,
    0,    0,  692,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  692,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  692,  692,  692,  692,
  692,  692,  692,    0,  692,  692,  692,  692,    0,  692,
  692,    0,    0,    0,    0,    0,    0,  692,  692,  692,
  692,  692,  692,  692,  692,  692,  692,  692,  692,  692,
    0,  692,    0,    0,  692,  692,    0,  692,  692,  692,
    0,  692,  692,  692,  692,    0,    0,  692,  692,  692,
  692,  692,    0,    0,    0,  692,  692,    0,    0,    0,
    0,  692,  692,  692,    0,  692,  692,  692,    0,  692,
  686,  692,  692,  692,    0,  692,  692,  686,  692,  692,
    0,    0,    0,    0,  686,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  686,    0,    0,    0,
    0,    0,  686,  686,    0,    0,  586,    0,  591,  513,
  686,  514,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  516,    0,    0,    0,    0,  334,
  686,    0,    0,  686,    0,    0,  600,    0,    0,  600,
    0,  600,  600,  600,  600,    0,  600,    0,    0,    0,
    0,    0,    0,    0,  600,  600,    0,    0,  686,    0,
  686,  686,    0,    0,    0,  600,  600,    0,  600,    0,
  686,    0,    0,    0,  686,  600,  600,    0,  600,  600,
    0,  600,    0,    0,  600,  600,  600,  600,  600,    0,
    0,    0,  600,  600,    0,    0,    0,    0,  600,  600,
  600,    0,  600,  600,  600,    0,  600,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  600,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  686,
    0,    0,    0,    0,    0,  686,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  686,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  686,
  686,  686,  686,  686,  686,  686,    0,  686,  686,  686,
  686,    0,  686,  686,    0,    0,    0,    0,    0,    0,
  686,  686,  686,  686,  686,  686,  686,  686,  686,  686,
  686,  686,  686,    0,  686,    0,    0,  686,  686,    0,
  686,  686,  686,    0,  686,  686,  686,  686,    0,    0,
  686,  686,  686,  686,  686,    0,    0,    0,  686,  686,
    0,    0,    0,    0,  686,  686,  686,    0,  686,  686,
  686,    0,  686,  469,  686,  686,  686,    0,  686,  686,
  470,  686,  686,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  471,    0,    0,    0,    0,    0,  472,  473,
    0,    0,  586,    0,    0,  513,    0,  514,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  516,    0,    0,    0,    0,  334,  474,    0,    0,  475,
    0,    0,  285,    0,    0,  285,    0,  285,  285,  285,
  285,    0,  285,    0,    0,    0,    0,    0,    0,    0,
  285,  285,    0,    0,  476,    0,  477,  478,    0,    0,
    0,  285,  285,    0,  285,    0,  479,    0,    0,    0,
  480,  285,  285,    0,  285,  285,    0,  285,    0,    0,
  285,  285,  285,  285,  285,    0,    0,    0,  285,  285,
    0,    0,    0,    0,  285,  285,  285,    0,  285,  285,
  285,    0,  285,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  285,    0,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,  795,    0,    0,    0,    0,
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
  472,  473,    0,    0,  586, 1085,    0,  513,    0,  514,
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
    0,    0,    0,    0,    0,    0,    0,   14,    0,    0,
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
    0,    0,    0,    0,    0,    0, 1297,    0,    0,  484,
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
  516,    0,    0, 1350,    0,  334,  474,    0,    0,  475,
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
    0,    0,    0,    0,  472,  473,    0,    0, 1057,    0,
 1057, 1057,    0, 1057,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1057,    0,    0,    0,
    0, 1057,  474,    0,    0,  475,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0, 1504,    0,
    0,  484,  485,  486,  487,  255,  256,  116,    0,  117,
  118,  119,  120,    0,  121,  488,    0,    0,    0,    0,
    0,    0,  122,  123,  489,  490,  491,  492,  493,  494,
  495,  496,  497,  124,  498,    0,  126,    0,    0,  499,
  500,    0,  501,  502,  503,    0,  129,  130,  504,  131,
    0,    0,  132,  133,  134,  135,  136,    0,    0,    0,
  137,  138,    0,    0,    0,    0,  139,  505,  141,    0,
  506,  143,  144,    0,  145, 1057,  507,  508,  509,    0,
  510,  511, 1057,  512,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1057,    0,    0,    0,    0,    0,
 1057, 1057,    0,    0,  515,    0,    0,  513,    0,  514,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  516,    0,    0,    0,    0,  334, 1057,    0,
    0, 1057,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1057,    0, 1057, 1057,
    0,    0,    0,    0,    0,    0,    0,    0, 1057,    0,
    0,    0, 1057,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1057,    0,    0,
    0,    0,    0, 1057,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1057,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1057, 1057, 1057,
 1057, 1057, 1057, 1057,    0, 1057, 1057, 1057, 1057,    0,
 1057, 1057,    0,    0,    0,    0,    0,    0, 1057, 1057,
 1057, 1057, 1057, 1057, 1057, 1057, 1057, 1057, 1057, 1057,
 1057,    0, 1057,    0,    0, 1057, 1057,    0, 1057, 1057,
 1057,    0, 1057, 1057, 1057, 1057,    0,    0, 1057, 1057,
 1057, 1057, 1057,    0,    0,    0, 1057, 1057,    0,    0,
    0,    0, 1057, 1057, 1057,    0, 1057, 1057, 1057,    0,
 1057,  469, 1057, 1057, 1057,    0, 1057, 1057,  470, 1057,
 1057,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
    0,    0,  472,  473,    0,    0, 1057,    0,    0, 1057,
    0, 1057,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1057,    0,    0,    0,    0, 1057,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,  484,
  485,  486,  487,  255,  256,  116,    0,  117,  118,  119,
  120,    0,  121,  488,    0,    0,    0,    0,    0,    0,
  122,  123,  489,  490,  491,  492,  493,  494,  495,  496,
  497,  124,  498,    0,  126,    0,    0,  499,  500,    0,
  501,  502,  503,    0,  129,  130,  504,  131,    0,    0,
  132,  133,  134,  135,  136,    0,    0,    0,  137,  138,
    0,    0,    0,    0,  139,  505,  141,    0,  506,  143,
  144,  567,  145, 1057,  507,  508,  509,    0,  510,  511,
 1057,  512,  146,  568,    0,    0,    0,    0,    0,    0,
    0,    0, 1057,    0,    0,    0,    0,    0, 1057, 1057,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  567,  567,    0,    0,  567,    0,    0,    0,    0,
  562,    0,    0,  568,  568,    0, 1057,  568,    0, 1057,
  567,    0,  567,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  568,    0,  568,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1057,    0, 1057, 1057,    0,    0,
    0,  562,  567,    0,  562,    0, 1057,    0,    0,    0,
 1057,    0,    0,    0,  568,    0,    0,    0,    0,  562,
    0,  562,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  562,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1057,    0,    0,    0,    0,
    0, 1057,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1057,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1057, 1057, 1057, 1057, 1057,
 1057, 1057,    0, 1057, 1057, 1057, 1057,    0, 1057, 1057,
    0,    0,    0,    0,    0,    0, 1057, 1057, 1057, 1057,
 1057, 1057, 1057, 1057, 1057, 1057, 1057, 1057, 1057,    0,
 1057,    0,  584, 1057, 1057,    0, 1057, 1057, 1057,    0,
 1057, 1057, 1057, 1057,    0,    0, 1057, 1057, 1057, 1057,
 1057,    0,    0,    0, 1057, 1057,    0,    0,    0,    0,
 1057, 1057, 1057,    0, 1057, 1057, 1057,    0, 1057,  567,
 1057, 1057, 1057,  584, 1057, 1057,  584, 1057, 1057,    0,
    0,  568,    0,    0,  567,    0,  567,  567,  567,    0,
  574,  584,  567,  584,    0,  567,  568,    0,  568,  568,
  568,    0,    0,    0,  568,    0,    0,  568,  567,    0,
    0,    0,    0,    0,  567,    0,    0,  567,  562,    0,
  568,    0,    0,  584,    0,    0,  568,    0,    0,  568,
    0,  574,    0,  562,  574,  562,  562,  562,    0,    0,
    0,  562,    0,    0,  562,    0,    0,    0,    0,  574,
    0,  574,    0,    0,    0,    0,    0,  562,    0,  567,
    0,    0,    0,  562,    0,    0,  562,    0,    0,    0,
    0,  568,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  574,    0,  567,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  568,    0,    0,    0,    0,
    0,  559,    0,    0,  567,    0,    0,    0,  562,    0,
  567,    0,    0,    0,  567,    0,  568,    0,    0,    0,
    0,  567,  568,    0,    0,    0,  568,    0,    0,    0,
    0,    0,  562,  568,    0,    0,    0,    0,    0,  567,
    0,    0,  559,    0,  567,  559,    0,    0,    0,    0,
    0,  568,    0,  562,    1,    0,  568,    0,    0,  562,
  559,    0,  559,  562,    0,    0,    0,    0,    0,  567,
  562,    0,    2,  567,    0,    0,    0,    0,    0,    0,
    3,  568,    0,    0,    0,  568,    0,    4,  562,    5,
  584,    6,  559,  562,    0,    0,    7,    0,    0,    0,
    0,    0,    0,    8,    0,  584,    0,    0,  584,  584,
    0,    0,    0,  584,    0,    0,  584,    9,  562,    0,
    0,    0,  562,    0,    0,    0,    0,   10,    0,  584,
    0,    0,    0,    0,    0,  584,    0,    0,  584,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  574,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  574,    0,    0,  574,  574,    0,    0,
    0,  574,    0,    0,  574,    0,    0,    0,    0,    0,
  584,    0,    0,    0,    0,    0,    0,  574,    0,    0,
   11,    0,    0,  574,    0,    0,  574,   12,    0,    0,
   13,    0,   14,   15,  584,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  584,    0,   16,    0,    0,
    0,  584,    0,    0,    0,  584,    0,    0,  574,    0,
    0,    0,  584,    0,    0,    0,  108,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  559,
  584,    0,  574,    0,    0,  584,    0,    0,    0,    0,
    0,    0,    0,    0,  559,    0,   17,  559,  559,    0,
    0,    0,  559,  574,  109,  559,    0,    0,    0,  574,
  584,   18,   19,  574,  584,    0,  110,    0,  559,    0,
  574,    0,    0,    0,  559,    0,    0,  559,    0,    0,
    0,    0,    0,    0,    0,    0,   20,    0,  574,    0,
  111,    0,   49,  574,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  574,  559,
    0,    0,  574,    0,    0,    0,    0,    0,    0,    0,
   49,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   49,  559,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  112,    0,    0,    0,    0,
    0,    0,    0,    0,  559,  113,   49,    0,    0,    0,
  559,    0,    0,    0,  559,    0,    0,    0,    0,    0,
    0,  559,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,    0,    0,    0,    0,    0,    0,  559,
    0,    0,    0,    0,  559,    0,    0,    0,  115,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121,    0,
    0,    0,    0,    0,    0,    0,  122,  123,    0,  559,
    0,   49,    0,  559,    0,    0,    0,  124,  125,    0,
  126,   49,    0,    0,    0,    0,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132,  133,  134,  135,
  136,    0,    0,    0,  137,  138,   49,    0,    0,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
  328,    0,    0,    0,   51,    0,    0,   51,  146,   51,
   51,   51,   51,    0,   51,    0,    0,    0,    0,    0,
    0,    0,   51,   51,    0,    0,    0,    0,  328,    0,
    0,    0,    0,   51,   51,  328,   51,    0,    0,  328,
  328,    0,  328,   51,   51,  328,   51,   51,    0,   51,
    0,    0,   51,   51,   51,   51,   51,    0,  328,    0,
   51,   51,    0,    0,  328,    0,   51,   51,   51,    0,
   51,   51,   51,    0,   51,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   51,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1362,    0,    0,    0,
    0,  328,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  328,
    0,    0,    0,    0,    6,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  328, 1559, 1560,    0, 1561,    0,
    0,    0,    0,    0,  328,    0,    0,    0,    0,    0,
    0,    0,    0,  328, 1562,  328,    0,    0,    0,    0,
 1563,    0,  328,    0,    0,  328,    0,  328,  328,  328,
  328,    0,  328,    0,    0,    0,    0,    0,    0,    0,
  328,  328,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  328,  328,    0,  328,    0,    0,    0,    0,    0,
    0,  328,  328,    0,  328,  328,    0,  328,    0,    0,
  328,  328,  328,  328,  328,    0,    0, 1564,  328,  328,
  328,  328,  328,    0,  328,  328,  328,    0,  328,  328,
  328, 1362,  328,    0,    0,   14,    0,    0,    0,    0,
    0,    0,  328,    0,    0,    0,    0,    0,    0,    0,
 1565,    0,    0,    0,    0,    0,    0,    0,    0,    6,
   16,    0,    0,    0,    0,    0,    0,    0,    0, 1764,
 1559, 1560,    0, 1561,    0,    0,    0,    0,  487,    0,
    0,  116,    0,  117,  118,  119,  120,    0,  121, 1562,
    0,    0,    0,    0,    0, 1563,  122,  123,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1566,  125,    0,
  126,    0,    0,    0,    0,    0,    0,  127,  128,    0,
  129,  130,    0,  131,    0,    0,  132, 1567,  134,  135,
  136,    0,    0,    0,  137,  138, 1568, 1569, 1570,    0,
  139,  140,  141,    0,  142,  143,  144,    0,  145,    0,
    0,    0, 1564,    0,    0,    0,    0,    0,  146,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1565,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  487,    0,    0,  116,    0,  117,  118,
  119,  120,    0,  121,    0,    0,  694,    0,    0,  695,
    0,  122,  123,    0,    0,  915,    0,    0,    0,    0,
    0,    0, 1566,  125,    0,  126,  696,    0,    0,  697,
    0,    0,  127,  128,    0,  129,  130,    0,  131,    0,
    0,  132, 1567,  134,  135,  136,    0,    0,    0,  137,
  138, 1568, 1569, 1570,    0,  139,  140,  141,    0,  142,
  143,  144,    0,  145,    0,    0,    0,    0,    0,  698,
    0,    0,    0,  146,    0,  699,  700,  701,    0,  702,
  703,  704,    0,  705,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  706,    0,  707,  694,    0,    0,  695,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  696,    0,    0,  697,    0,    0,
    0,    0,  708,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  709,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  698,    0,    0,
    0,  710,    0,  699,  700,  701,    0,  702,  703,  704,
    0,  705,    0,    0,    0,    0,  115,    0,    0,  116,
    0,  117,  118,  119,  120,    0,  121,    0,    0,  706,
    0,  707,    0,    0,  122,  123,  711,  712,    0,    0,
    0,    0,    0,    0,    0,  124,  125,    0,  126,    0,
    0,    0,    0,  713,    0,  127,  128,    0,  129,  130,
  708,  131,    0,    0,  132,  133,  134,  135,  136,    0,
 1030,    0,  137,  138,    0,    0,    0,  709,  139,  140,
  141,    0,  142,  143,  144,    0,  145,    0,    0,    0,
    0,    0,    0, 1031,    0,    0,  146,    0,    0,  710,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  115,    0,    0,  116,    0,  117,
  118,  119,  120,    0,  121,    0,    0,    0,    0,    0,
    0,    0,  122,  123,  711,  712,    0,    0,    0,    0,
    0,    0,    0,  124,  125,    0,  126,    0,    0, 1032,
    0,  713,    0,  127,  128,    0,  129,  130,    0,  131,
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
    0,  115,    0,  146,  116,  192,  117,  118,  119,  120,
  193,  121,  194,  195,  196,  197,  198,  199,  200,  122,
  123,  201,  202,  203,  204,  205,    0,    0,  206,  207,
  124,  125,  208,  126,    0,    0,  209,  210,  211,  212,
  127,  128,  213,  129,  130,  214,  131,  215,  216,  132,
  133,  134,  135,  136,    0,    0,    0,  137,  138,  217,
  218,  219,  220,  139,  140,  141,  221,  142,  143,  144,
  222,  145,  223,  224,  225,  226,  227,  228,  229,  230,
  231,  146,  479,    0,    0,  479,  479,  479,  479,  479,
  479,    0,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,    0,    0,  479,
  479,  479,  479,  479,  479,    0,    0,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,    0,    0,    0,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,  479,  479,
};
static short yycheck[] = {                                       9,
    0,   63,    4,   71,  174,   16,   16,  114,   18,  628,
   61,  164,  165,   70,   76,   80,  410,   77,   70,  515,
  102,  447,  447,  556,  663,   93,   99,  443,  319,  414,
  164,  165, 1095,  439,  825,  825, 1361,  778,  103,  903,
  881,  101,  631,  903,  597,  903,   97,  903,   58,   59,
   69,   61,   62,   63,   64,   65, 1362, 1109,  903, 1035,
   70,   71,   72, 1425,   74,   62,   76,   77,  795,   79,
   80,   81,  813,  576,  801,  828,  886,   74,  835,   89,
   91,   91,   92,   93,   65,   95,   96,   97,   98,  100,
  100,  101,  102,  103,  104,  157,  106,  179,  903,  575,
  336,   98,  912,  412,  156,  424,  158, 1083,  176,  106,
  270,  921,  365, 1184,   41,   96,  178,    0,  741,    0,
    0,  654,  651,  104,    0,  658,  879,  680, 1129,  423,
  663,  748,    0, 1241,  453,   63, 1380,   44,  988,   45,
    0,    0,  394,    0,   45,    0,  156,  157,  158,   45,
    0,    0,  162,   40,   44, 1219,    0,   63,    0,  169,
    0,    0,    0,   40,  174, 1236,  176,  177,  178,  179,
    0,  981,  279,  183,   44,   44,   59,    0,   59,   59,
  177,    0,    0,   59,   41,   44,  715,   44,  169,   41,
   44,   59,   41,   91,  890,  365,   44,   41,   44,   59,
   59,   41,   59,   41,   59,    0,   44,   41,  368,   59,
   59,  183,   44,   40,   44,   59,  235,   59,   41,   59,
   59,   59,   41,  639,   75,   44,    0,    0,    0,   59,
  755,   40,    0, 1423,  273,  344,   59,  663,  663,  303,
   59,   59, 1188, 1568, 1569, 1114,   41,  266,   99,   44,
   41,  280, 1198,  647,   63,    0,  272,   45,  310,  515,
   40,  313,  334,   45,   59,  372,  397,   41,   44,  262,
   44, 1636,  666,  309, 1580,  691,  567,  614, 1224,  317,
  310,    0,  395,   63,    0,   59,   59,   59,  303,  351,
  264,   59,  583,  273,  264,    0,  347,  276,  617,  257,
  310, 1028, 1029,  313, 1095, 1095,    0,    0,    0,  319,
  320, 1931, 1330,  365,   59, 1333,  298,  448, 1465,  305,
  336,  817,   41,    0,   41,  298,    0,   44,  824,  315,
  586,  268,   41,  447,  298,   44, 1407,  347,  371, 1066,
   59,  351,  484,   59,  284, 1155,  410,   41,  367,   41,
   41,   44,   44,   44,   59,    0,  324,  297, 1978,  395,
  351,  413,  434,   44,   41,   59,   59,   59,  348, 1421,
  334,  423,  330,  425,  516,  427,    0,  275,  430,  409,
  909,  449,   59,  393,  394,   59,  379,  500,  917, 1453,
  428,  451, 1393,  372,  430,  410,   41,  459,  462,   44,
  509,    0,  431,  413,    0,  575,  939,  440, 1460,    0,
  461,  964, 1153,  423,   59,  425,  945,  427, 1783,  458,
  430,  440,  359,  442,    0,   61,    0,  317,  262,  403,
 1406,    0,  264,  403,  427,   59, 1761,  297,  457,  449,
  450,  451,   41, 1805, 1171,   44,  264,  462,   93,  459,
   41,  461,  434,  450,  464,  297,    0,   41,  315,  445,
   59,  434,  464,   59, 1160,  272,  315, 1657,   59,    0,
  434,  315,   41,    0, 1420,  315,  325,  315,  327,  336,
 1005,   41,  336,   59,  344,   59,  989,  336,  456,  327,
   59, 1437,  336,  484,   93,   44,  336, 1000,  336,  380,
  380, 1004,  344,  565,  760,  277,  516,  376,  389,  389,
   41,   41,  574,  336,   41,   59,  578, 1386, 1387,  264,
 1538, 1467,  450,  575, 1542,  862,  365,  379,   59,  410,
  380,  380,   59, 1777, 1261, 1262, 1898, 1264, 1265,  315,
  389,  315,  380,  450,  450,  379,  423,  402,  682,  450,
  380,  389,  439,  844,  450,  565,  566,  567,  309,  389,
  387,   41,  336,  444,  574, 1356, 1356,   44,  578,  566,
  446,  731,  390,  583,  444,  585,  628, 1939,  446, 1687,
  428,  376,  333,  432,  276,  277,  513,  446,  379,  446,
 1642,  342,  284,  361,  432,  444,  315,  446, 1669, 1961,
  446, 1358,  446,  277, 1357,  624,  446,  992,  446, 1459,
  683,  663, 1235,  585,  444, 1686,  910,  336,  628, 1925,
  301,  857,  641,  446, 1243, 1121, 1129,  880,  401,  938,
  513, 1248,  513,  513,    0, 1699, 1637,  656,  315,  891,
  264,  450,  336, 1119, 1950,  390,    0,  398, 1458,  509,
  327,  402,  662,  663, 1157,  825,  513, 1186,  668,  336,
  979,  297,  450,  513,  513, 1990,  685,  509,  450,  513,
  450, 1977,  446,  513,  493,  513,    0, 1824,   41,  832,
  372,   44, 1988,  513,  376, 1991,  402,   41,  740,  477,
  264,  336, 1998,   59,  403,  477,  401, 1643,  493, 1762,
  298,  393,  262,  380, 1650,   59,  300,  399, 1654, 1655,
  880,  730,  389,  262,  258,    0,   41,   41,  737,  738,
  494, 1392,  306,  428, 1742,  317,  270,  446,   42, 1705,
  740,  865,  262,   61,  498,   59,  334,  336,    0,  513,
  315,  433, 1688,  376, 1690,  336,  315, 1647,  344, 1490,
  501,  297,  446,  297,  348,  432,  325,  402,  327,  303,
 1166,  305,  513,  514,  515, 1817,  390,  336,  344,  446,
    0, 1823,   59,  783,   59,   42,   43, 1273,   45,    0,
   47,  833,  262,  835,  315,  837,  315, 1651, 1652, 1689,
  852, 1651, 1652, 1651, 1652, 1651, 1652,   59,  344, 1640,
  344,  493,  783,  402,  262,  336, 1651, 1652,  818,  336,
  262,  380,  874,  947,  876,  262,  390,    0,  828,  379,
  389,    0,  395,  833,  301,  835, 1580,  837,  880,   59,
  379, 1237,  683,  885,  844,  586,  513, 1426,   59,  590,
  352,  257,  852,  853, 1340, 1791, 1651, 1652,    0,  379,
  860,  861, 1651, 1652,  315, 1918,  853, 1920,  910,  319,
  283, 1246,  935,  432,  874, 1928,  876, 1930,   44,  879,
  445,  890,  275,  305,    0,  885,   59,  446,  897,  924,
   59,  891,  260,  315,  936, 1524,   42,  939,  907, 1418,
 1419,   47,  258, 1446, 1381, 1391, 1449,  339,    0,  379,
  910,  974,  975,  976,  270,  263, 1435,   59,  755,  421,
  273, 1974, 1639,  760,  330,  446,  445,   93,  980,  446,
  362,  379,  300,  401,    0, 1095,  936,  379,  990,  939,
 1315,  297,  379,   59,  953,    0,    0,  303,  341,  305,
  791,  514,    0,  306,  513,  955,  260,  304,  305, 1119,
  428,  309,  401,  955,  318, 1626, 1443,   59, 1904, 1905,
 1906,  315,  320,  509,    0,  509,  261,  331,    0,  297,
  980, 1762, 1762,  327,  259,  427,  418,  265,  344,  264,
  990,  306,  336,   59,  269,  348,  300,    0,  998,  999,
  354,  315,    0,  297,   59,   59,  998,  304,  305, 1863,
 1864,   59,  264, 1863, 1864, 1863, 1864, 1863, 1864,  760,
  277,  299,  336,  445,  280,  282,    0, 1273, 1863, 1864,
 1553,    0,  259,   59,    0,  344,  380,   59, 1755,  780,
  453,  454,  269,  784,  277,  389,  450,    0,    0,  282,
  403,  405,  258,  794,  348,  333,   59, 1463,  799,  800,
  317,   59,  803,  804,  805,  806,  807,  808, 1863, 1864,
 1070,  419,  397,  322, 1863, 1864,  817, 1119,  819,  820,
  821,  822,  823,  824, 1365,   59,  827, 1368,  432,   41,
   59,  264,  450,   59,  935,  264,  443,  303, 1222, 1070,
  303,  276,  446,  944,  276,  458,   59,   59, 1524, 1524,
  380,    0,  284, 1113, 1114,  390,  258,  371,  304,  305,
  961,  377,  863,  448,    0,  265,  260,  338,  270, 1129,
   44,  277,  446,  974,  975,  976,  282, 1918, 1918, 1920,
 1920,  316,  883,  319,  453,  454,  443, 1928, 1928, 1930,
 1930, 1160,   41,  509,  895,  896,  410, 1192, 1193,  299,
  371,  303,  432,  305, 1001, 1882,  300, 1230, 1005,  513,
   59,    0, 1889,  422, 1693,  431,    0,  318,   40,   93,
  429, 1925, 1245,   59,  276,  277,  397,  390, 1251,  400,
  331,   44,  448, 1974, 1974,  397, 1356, 1239,  264,   44,
  319, 1243, 1362,  457,  410,  416, 1950,  410, 1243,  264,
  264, 1211,   41,  354,  416,   44,  264,  390,  393,  409,
   41,  390,  428,   44, 1216,  428,  258,  368, 1228,  261,
   59,  442,  394, 1977,    0,   59, 1379, 1380, 1381, 1239,
   93, 1487, 1488, 1243, 1988,  485,  436, 1991,   93, 1495,
 1496,   40, 1498, 1499, 1998,  495,  462,  443,  433,  462,
 1506, 1507, 1508, 1509,  405, 1511, 1512, 1513, 1514, 1515,
 1516,  303, 1013, 1014, 1015, 1659,  348, 1018, 1019, 1020,
  372, 1022, 1023, 1024, 1025, 1026, 1027,    0, 1412,    0,
 1414,  276, 1778,   59,  297, 1036, 1037, 1369, 1039,  238,
 1041,    0,  376, 1044, 1045, 1046, 1047, 1048, 1049,  376,
 1311, 1311,  338,  260, 1361, 1370, 1358, 1367,    0, 1361,
 1362, 1062,  259, 1064,  390, 1934,  337,  396,  276,  309,
   41,  316,  269,   44,  345,  390,  390, 1461,    0, 1080,
  338,  344,  390,  412,    0,  371,   59,   44,   59,  352,
    0,  290,    0,  300,    0,    0,  260, 1357, 1358,  298,
   59, 1361, 1362,  315,  300, 1365, 1366, 1367, 1368, 1369,
 1370,  397,  338,  325,  400,  327,  517,   59,  378, 1366,
 1121,  453,  454,  386,  336,  388, 1386, 1387,  378, 1230,
  416,   41,  486, 1393,   44,   41,  300,   59,   44,  397,
    0,    0,  400,   59, 1245,  371,  318,  410,  393,   59,
 1251,   59,  348,   59,   59,  395,  442,  298,  416,  331,
 1580,  313,  314,    0,  372,  338,  315,  339,  380,    0,
  333,  397,  435,    0,  400,  410,  372,  389,   42,   43,
    0,   45,  354,   47,  442,  393,  426,  336,  433, 1441,
  416,  399,  434,  334,    0,    0,  368,  260,  371,   59,
   59,  441,  338,    0,   41, 1474,  273,   44,    0, 1629,
 1479,  457,  513,  459,   41,  352,  442,   44,    0,  260,
  432,   41,   59,    0,  397,  433,  315,  400,   59,  318,
  446,  380,   59,  405,  446,  371,  325,  300,  327,   59,
  389, 1559,  331,  416,   41,    0,  509,  336,  452,  386,
  339,  388,  291,   59,   59,    0, 1517, 1517,  399,  300,
    0,  397,   59,  302,  400,  354,    0,   59,  409,  442,
  309,    0, 1273,  410,  313,  333,    0,   59, 1580,  368,
  416,   41,   59,  432,   44, 1286, 1287,  376, 1289, 1290,
   46,  380,   41,  434,  333,   44, 1297,  446,  435, 1559,
  389,  513,   58,   59,   59,   61,  442, 1567, 1568, 1569,
  368,  333,   41,   40,   59,   44,  405,  271, 1319,   59,
 1580,  258,  380,  416,  450,   59, 1327,  450, 1329, 1330,
   59,  285, 1333,  270, 1335,   59,  409,  520, 1339, 1340,
 1341,   41, 1762,  432,   44,  517,  368, 1348, 1660, 1350,
  258,  477, 1612, 1613,  477,  444,  395,  446,  380,  399,
 1780,  401,  270,  436,  513,  453,  303,   46,  305, 1629,
  276,  277,  260,   46, 1777,  329, 1636, 1637,  284,   58,
   59,   41,   61,  264,   44,   58,    0,  426,   61,  297,
 1391,  430,   41,   42,   43,  303,   45,  305,   47,   41,
 1660,  313,   44,  357,  493,  315,   42,   43,  315,   45,
  298,   47, 1396,  277, 1398,  325,   41,  327,  282,   44,
   41, 1422,  310,   44,  513,  444,  336,   41,  517,  339,
   44, 1732,   41,  485,  486,   44,  344,   42,   43,  333,
   45, 1945,   47, 1947,  352,   59,  334,  332,   41,  276,
  277,   44, 1809,    0,  449,  450,  451,  284,  403,   42,
   43,  426,   45,  395,   47,  336,  372,  421, 1729, 1729,
  380,   41, 1732,   41,   44,   40,   44,  317,  315,  389,
  519,   41,  477,  478,   44, 1486,   41,  393,  325,   44,
  327, 1492, 1804,  399,   41,  315, 1497,   44, 1918,  336,
 1920, 1761,  339, 1504,   41, 1925,  315,   44, 1928, 1810,
 1930,  399,   59,  421,   41,  315,  336,   44,  315, 1769,
 1780,  409,  432, 1783,  376,   41,   41,  433,   44,   44,
 1950,  376,    0,  356,  444,   41,  446, 1538,   44,  336,
 1800, 1542, 1899,  380, 1804,  372,  434, 1548,  384,  385,
 1810,  498,  389,  428, 1974,  442, 1874, 1977,  453,  454,
  380, 1830,  377, 1564,  428, 1834,  393,  410, 1988,  389,
  426, 1991,  399,   41,   42,   43,   44,   45, 1998,   47,
  453,  454,  319,  380,   41,   42,   43,  493,   45,  337,
   47,   59,   60,   61,   62,  432, 1856, 1612, 1613,   42,
   43,  509,   45,  513,   47,  375,  433,  444,  497,  446,
 1612, 1613,  432,  409, 1874, 1875,   42,   43,  508,   45,
  297,   47,  508, 1925, 1876,   93,  446,  509,  277,  461,
  306,   44, 1934,  282,  446,  432,   61, 1638,    0,  283,
  366,  277,   42,   43,   40,   45,  282,   47, 1950,  446,
   40,  264,   40,  389,   41,   42,   43,   40,   45,   40,
   47,   40,  276,  277,  453, 1925,  493, 1917,  317,   40,
  284, 1931,  277,   40, 1934, 1977,  513,  282,  453,   41,
   40,   40,   44,   40,  453, 1945, 1988, 1947,   40, 1991,
 1950,    0,   40,  513,  277,    0, 1998,   59,   40,  282,
  336,  351,   40,   42,   43,   40,   45, 1708,   47,   40,
   40,   40,  317, 1714, 1715,  262,  513, 1977, 1978,   91,
   41,   42,   43,   40,   45,  308,   47,   40, 1988,   40,
 1990, 1991,   41,   40,  402,   44,   41, 1738, 1998,   44,
   44, 1742,    0,   46,  264, 1746,   42,   43,   44,   45,
   59,   47, 1753, 1754,   59,   61,   40,  395,  372,  317,
   42,   43,  420,   45,   44,   47,   44,   46,  315,   42,
   43,  318,   45,   61,   47,   40,  264, 1778,  325,  393,
  327,   40,    0,   41,  331,  399,   44,  337,   44,  336,
  258,   46,  339,  344,  262,  344,  264,  265,  779,   58,
  781,   59,  344,  271,  785,  786,  787,  354,  276,  277,
  276,  365,  264,  390,  282,  390,  284,  285,  428,  433,
  277,  368,  450,   41,  275,  282,   44,  344, 1829,  376,
  392,  299,  379,  380,  277,  303,  304,  305,  439,  282,
  308,   59,  389,  439,   40,  345,   40,  315,   91,  317,
  318,  277,   40,  321,   40,  315,  282,  325,  405,  327,
  264,  329,  376,  331,  264,  403,   44,  375,  336,  337,
  382,  339,  315,   40,   40,  380,  443,  277,  315,  493,
  427,   41,  282,  275,   63,  432,  354,  355,  296,  357,
  277,  314,    0, 1894,  428,  282,  428,  444,   40,  446,
  368,   44,  370,  456,  372,  373,  374,  440,  376,  444,
  336,  379,  380,  264,  276,  277,  337,   44,  494,  281,
   40,  389,  284,  273,   40,  393, 1927,  281,  340,  340,
  379,  399,  297,   41,  402,  297,   44,  405,  277,  379,
  379,  410,  410,  282,  408,  410,  493,   40,   40,  352,
   40,   59,  362,  421,   40,   44,  277,   41,   41,  427,
  428,  282,  317,  277,  432,  433,  513,  276,  277,  308,
  517,  276,  277,   44,   46,  443,  444,  438,  446,  284,
  270,  277,   40,  265,  452,  378,  282,  455,  361,  457,
  458,  459,  460,  317,  462,  277,   44,  317,  325,  266,
  282,  493,  470,  471,  277,  414,   41,  264,   58,  282,
  372,   41,   41,  481,  482,    0,  484,  299,  276,  277,
   41,  387,  264,  491,  492,  493,  494,  495,  446,  497,
  306,  393,  500,  501,  502,  503,  504,  399,  306,   41,
  508,  509,   41,   40,  262,  513,  514,  515,  516,  517,
  518,  519,  520,   40,  522,   40,   41,   42,   43,   44,
   45,   46,   47,  372,  532,  317,   40,  372,   40,   40,
   40,  433,   40,   58,   59,   60,   61,   62,   40,  318,
  262,   41,   42,   43,  393,   45,   40,   47,  393,   40,
  399,   40,  331,   40,  399,   40,   40,  315,   40,   40,
  318,  427,  305,  446,   44,   44,   91,  325,   93,  327,
   41,   41,  264,  331,  372,  354,  339,  381,  336,  339,
  273,  339,  327,  384,  433,  284,  379,  399,  433,  368,
  379,  379,  379,  379,  379,  393,  354,  376,   41,   41,
  450,  399,  276,   41,   42,   43,   40,   45,   41,   47,
  368,   41,   41,  317,   41,   41,   44,  305,  376,   59,
  290,  379,  380,   40,  427,  336,  405,  336,  276,  277,
  305,  389,    0,  281,   59,  433,  284,   41,   42,   43,
   58,   45,  339,   47,  324,  532,  277,  405,  428,  297,
  434,  298,  333,  452,  298,  334,  455,  434,  457,  458,
  459,  460,  334,  462,   41,   42,   43,   41,   45,  427,
   47,  470,  471,   41,  432,   41,   44,  348,  365,  365,
  378,   41,  481,  482,   41,  484,  444,   41,  446,   41,
   41,   59,  491,  492,   41,  494,  495,   41,  497,   41,
   41,  500,  501,  502,  503,  504,   41,   41,   41,  508,
  509,   41,   41,   41,  493,  514,  515,  516,   41,  518,
  519,  520,   40,  522,  372,   42,   43,   44,   45,   41,
   47,  315,   40,  532,   44,  493,  378,  262,  517,  264,
  265,  443,   41,  340,  458,  393,  271,  333,   40,  336,
  500,  399,  277,  336,   40,  513,   40,  282,  376,  517,
  285,   44,   41,   40,  427,   41,    0,   41,   42,   43,
  376,   45,  376,   47,  299,    0,   41,  277,  301,  304,
  305,  298,  282,  308,  434,  433,   42,   43,   41,   45,
  315,   47,  317,  318,  290,  304,  321,   41,   42,   43,
  325,   45,  327,   47,  329,    0,  331,   41,  301,  455,
   44,  336,  337,  333,  339,  409,   41,   42,   43,   44,
   45,   46,   47,  376,  376,   59,   40,   61,  264,  354,
  355,  450,  357,  264,   59,   60,   61,   62,  403,  277,
  317,   40,  443,  368,  282,  370,   44,  372,  373,  374,
  270,  376,   40,   40,  379,  380,  464,  465,  466,  467,
  468,  469,  470,  471,  389,   41,   91, 1018,   93, 1020,
  270, 1022, 1023,  277, 1025, 1026, 1027,  402,  282,  450,
  405,  264,  427,  339,  339,   41,  450,   41,   42,   43,
   41,   45,   59,   47,   41,   59,  421,   59,   59,  305,
  277,   41,  427,  428,   59,  282,   59,  432,  276,  277,
   59,   41,  301,   59,   59,    0,  284,    0,  443,  444,
    0,  446,   41,   42,   43,    0,   45,  452,   47,  297,
  455,  161,  457,  458,  459,  460,  581,  462,   42,   43,
 1377,   45,  161,   47,  853,  470,  471,   41,   42,   43,
  162,   45,  321,   47, 1248,  583,  481,  482,  320,  484,
  277,   95,  993,  844, 1246,  282,  491,  492,  493,  494,
  495,  744,  497,  998,  955,  500,  501,  502,  503,  504,
   81, 1243, 1238,  508,  509, 1676,  968, 1677,  513,  514,
  515,  516,  517,  518,  519,  520,  444,  522, 1659, 1228,
  691,  885, 1169,  277,  372,  370,  262,  532,  282, 1239,
  425,   41,   42,   43,  425,   45,  919,   47, 1657, 1800,
 1937,  277, 1110, 1941,  258,  393,  282,  668, 1940, 1567,
 1963,  399, 1766,  277,  910, 1926, 1409,  262,  282,  264,
  265,  275,  276,  277,  430,  415,  271,  281,  670,  408,
  284,  971,  277, 1450,   13,  833,  163,  282,  355,  728,
  285,  599,  963,  297,  874,  433,    0,  603,  167,  303,
 1129, 1397,  863,   91,  299,    0,  301, 1894, 1729,  304,
  305,  777,  614,  308, 1517,   41,   42,   43, 1122,   45,
  315,   47,  317,  318,  851,  895,  321,   41,   42,   43,
  325,   45,  327,   47,  329, 1288,  331,   41,  493, 1172,
   44,  336,  337, 1335,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,  277,   -1,   59,   -1,   61,  282,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   41,   42,   43,  368,   45,  370,   47,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,  277,  393,
  264,   -1,   -1,  282,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,  277,  408,   -1,  410,  402,  282,   -1,
  405,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,
   -1,   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   41,   42,
   43,   -1,   45,   -1,   47,  470,  471,  277,   -1,   41,
   42,   43,  282,   45,   -1,   47,  481,  482,   -1,  484,
   42,   43,   44,   45,   -1,   47,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   41,   42,   43,   -1,   45,   -1,   47,  532,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,  258,   -1,  282,   -1,   -1,   -1,
   -1,   -1,   -1,  277,   -1,   -1,   -1,  262,  282,  264,
  265,  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,
   -1,   -1,   -1,   -1,  299,    0,  277,   -1,   -1,  304,
  305,  282,   -1,  308,   -1,   41,   42,   43,   -1,   45,
  315,   47,  317,  318,   -1,   -1,  321,   41,   42,   43,
  325,   45,  327,   47,  329,   -1,  331,   41,   -1,   -1,
   44,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   41,   42,   43,  368,   45,  370,   47,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,  277,   -1,  410,  402,   -1,  282,
  405,   -1,   -1,   -1,   -1,  277,   -1,   41,   42,   43,
  282,   45,   -1,   47,  428,  277,  421,   -1,   -1,  433,
  282,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,  277,  462,   -1,   -1,
   -1,  282,   -1,   -1,  277,  470,  471,   42,   43,  282,
   45,   -1,   47,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   41,   42,   43,   -1,   45,   -1,   47,  532,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,  258,   -1,  282,   -1,   -1,   -1,
   -1,   -1,   -1,  277,   -1,   -1,   -1,  262,  282,  264,
  265,   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,
   -1,   -1,   -1,   -1,  299,    0,  277,   -1,   -1,  304,
  305,  282,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,
   44,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,  277,   -1,   59,   -1,   -1,  282,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,  262,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,
  405,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
   -1,   -1,   -1,  302,  428,   -1,  421,   -1,   -1,  433,
  309,   -1,  427,  428,  313,  314,   -1,  432,   -1,   -1,
   -1,   -1,   -1,  322,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,  333,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,  277,  462,   -1,   -1,
   -1,  282,   -1,   -1,  277,  470,  471,   -1,   -1,  282,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,  395,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,  411,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,   -1,
   -1,  430,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,  441,   -1,   -1,   -1,  291,  262,   -1,  264,
  265,   -1,  276,  277,   -1,   -1,  271,  302,   -1,   -1,
  284,  460,  277,   -1,  309,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,   -1,  322,   -1,   41,
   -1,   -1,   44,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   59,   -1,   61,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
  519,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,  379,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
  395,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,  411,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,  426,   -1,   -1,   -1,  430,   -1,  402,  433,   -1,
  405,    0,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,  433,
   -1,   -1,  427,  428,   -1,  460,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   41,   -1,   -1,   44,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   59,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,  519,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,  258,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,  273,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,   -1,   -1,
   -1,  303,   -1,   -1,  306,   -1,   -1,  262,   -1,  264,
  265,   -1, 1487, 1488,   -1,   -1,  271,   -1,   -1,   -1,
 1495, 1496,  277, 1498, 1499,   -1,   -1,  282,   -1,   -1,
  285, 1506, 1507, 1508, 1509,   -1, 1511, 1512, 1513, 1514,
 1515, 1516,   -1,   -1,  299,    0,  348,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,  403,  357,   -1,   59,   60,   61,   62,  410,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,  428,  276,  277,   -1,
   -1,   -1,   -1,   -1,  389,  284,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,  458,   -1,   -1,   -1,
  462,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,  372,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,  393,  500,  501,  502,  503,  504,
  399,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
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
   -1,  446,   45,   -1,   -1,   -1,   -1,  452,   -1,   -1,
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
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,  292,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,  320,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,  348,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,  419,  522,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,   -1,
  453,  454,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,  472,
  473,  474,  475,  476,  477,  478,  479,  480,   -1,   -1,
  285,   -1,   -1,   -1,  487,  488,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  496,  299,    0,   -1,   -1,   -1,  304,
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
  379,  380,   42,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,  481,  482,  376,  484,   -1,   40,
  380,   -1,   -1,   -1,  491,  492,   -1,  494,  495,  389,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,  405,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  293,    0,  532,   -1,   -1,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   41,   -1,   -1,   44,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   59,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  532,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,  368,   -1,  455,   -1,  457,  458,  459,
  460,  376,  462,   -1,   -1,  380,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
  405,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,  432,  518,  519,
  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,  532,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,    0,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,  522,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,  532,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,  452,  368,   -1,  455,   -1,  457,  458,  459,  460,
  376,  462,   -1,   -1,  380,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  389,   -1,   -1,   91,   -1,   -1,   -1,
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
   -1,  497,    0,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,  522,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  532,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  405,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,  444,
   -1,  446,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  493,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,  315,  513,   -1,
  318,   -1,  517,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,
   -1,  339,   -1,   -1,   -1,   -1,  275,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,  291,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,  302,  372,  373,  374,   -1,  376,   -1,
  309,  379,  380,   -1,  313,  314,   -1,   -1,   -1,   -1,
   -1,  389,   41,  322,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  333,   -1,   -1,  405,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,   -1,  421,   -1,  271,   -1,   -1,   -1,  427,
   -1,  277,   -1,   -1,  432,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,    0,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  395,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,  493,   -1,  426,   -1,   -1,
   -1,  430,   -1,   -1,   59,   -1,   -1,   -1,  354,  355,
   -1,  357,  441,   -1,  262,  513,   -1,   -1,   -1,  517,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,  460,  461,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,   -1,   -1,  421,   -1,  325,   -1,  327,
   -1,  427,   -1,  331,   -1,   -1,  432,   -1,  336,   41,
  519,  339,   44,  522,   -1,   -1,   -1,   -1,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  354,   59,   -1,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,    0,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  315,  513,   -1,  318,
    0,  517,   -1,   -1,   -1,   -1,  325,   -1,  327,  427,
   -1,   -1,  331,   41,  432,   -1,   44,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,
   -1,   59,   -1,   -1,   -1,  354,   -1,  262,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   59,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,
  315,   -1,   -1,  318,    0,  513,   -1,   -1,   -1,  517,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  427,   -1,
   -1,  336,   -1,  432,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,  444,   -1,  446,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
  262,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,   59,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,  389,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  405,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  315,  513,   -1,  318,    0,  517,   -1,
   -1,   -1,  427,  325,   -1,  327,   -1,  432,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,   -1,  262,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   59,  379,  380,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,  389,  493,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,  315,  513,   -1,
  318,    0,  517,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  427,   -1,   -1,  336,   -1,
  432,  339,   -1,   -1,   -1,  315,   -1,   -1,  318,    0,
   -1,   -1,  444,   -1,  446,  325,  354,  327,   -1,   -1,
   -1,  331,   41,   -1,   -1,   44,  336,   -1,   -1,  339,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   59,  379,  380,   -1,  354,   -1,  262,   -1,   -1,   -1,
   41,  389,   -1,   44,   -1,   -1,   -1,   -1,  368,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  376,  405,   59,  379,
  380,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,  389,
   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,  405,   -1,   -1,   -1,  315,
   -1,   -1,  318,    0,   -1,   -1,  444,   -1,  446,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  427,   -1,   -1,
  336,   -1,  432,  339,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,   -1,  444,   -1,  446,  325,  354,  327,
   -1,   -1,   -1,  331,   41,   -1,   -1,   44,  336,  262,
   -1,  339,  368,   -1,   -1,  493,   -1,   -1,   -1,   -1,
  376,   -1,   59,  379,  380,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,  513,   -1,   -1,    0,  517,
  368,   -1,   -1,  493,   -1,   -1,   -1,   -1,  376,  405,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,  315,  513,   -1,  318,    0,  517,   -1,   -1,
   -1,  427,  325,   -1,  327,   -1,  432,  405,  331,   41,
   -1,   -1,   44,  336,   -1,   -1,  339,   -1,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,  427,
   -1,  354,   -1,  262,  432,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,  368,  444,   -1,  446,   -1,
   -1,   -1,   -1,  376,   -1,   59,  379,  380,   -1,   -1,
   -1,  262,   -1,   -1,   -1,   -1,  389,  493,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,  315,  513,   -1,  318,
    0,  517,   -1,   -1,   -1,  493,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,  427,   -1,   -1,  336,   -1,  432,
  339,   -1,   -1,   -1,  315,  513,   -1,  318,    0,  517,
   -1,  444,   -1,  446,  325,  354,  327,   -1,   -1,   -1,
  331,   41,   -1,   -1,   44,  336,   -1,   -1,  339,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   59,
  379,  380,   -1,  354,   -1,  262,   -1,   -1,   -1,   41,
  389,   -1,   44,   -1,   -1,   -1,   -1,  368,   -1,   -1,
  493,   -1,   -1,   -1,   -1,  376,  405,   59,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
  513,   -1,   -1,    0,  517,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  432,  405,   -1,   -1,   -1,  315,   -1,
   -1,  318,    0,   -1,   -1,  444,   -1,  446,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  427,   -1,   -1,  336,
  262,  432,  339,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  444,   -1,  446,   -1,  354,   -1,   -1,
   -1,   -1,   59,   41,   -1,   -1,   44,   -1,  262,   -1,
   -1,  368,   -1,   -1,  493,   -1,   -1,   -1,   -1,  376,
   -1,   59,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,  315,  513,   -1,  318,    0,  517,   -1,
   -1,   -1,  493,  325,   -1,  327,   -1,   -1,  405,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,  315,  513,   -1,  318,    0,  517,   -1,   -1,   -1,
  427,  325,  354,  327,   -1,  432,   -1,  331,   41,   -1,
   -1,   44,  336,   -1,   -1,  339,  368,  444,   -1,  446,
   -1,   -1,   -1,   -1,  376,   -1,   59,  379,  380,   -1,
  354,   -1,  262,   -1,   -1,   -1,   41,  389,   -1,   44,
   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,  405,   59,  379,  380,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,  389,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  432,  405,   -1,   -1,   -1,  315,  513,   -1,  318,    0,
  517,   -1,  444,   -1,  446,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,  427,   -1,   -1,  336,   -1,  432,  339,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,
  444,   -1,  446,  325,  354,  327,   -1,   -1,   -1,  331,
   41,   -1,   -1,   44,  336,  262,   -1,  339,  368,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  376,   -1,   59,  379,
  380,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,  513,   -1,   -1,   -1,  517,  368,   -1,   -1,  493,
   -1,   -1,   -1,   -1,  376,  405,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,  315,  513,
   -1,  318,    0,  517,   -1,   -1,   -1,  427,  325,   -1,
  327,   -1,  432,  405,  331,   -1,   -1,  315,   -1,  336,
  318,   -1,  339,   -1,  444,   -1,  446,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  427,   -1,  354,  336,  262,
  432,  339,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,  368,  444,   -1,  446,   -1,  354,   -1,   -1,  376,
   -1,   59,  379,  380,   -1,   -1,   -1,  262,   -1,   -1,
  368,   -1,  389,  493,   -1,   -1,   -1,   -1,  376,   -1,
   -1,   -1,  380,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,  389,  315,  513,   -1,  318,   -1,  517,   -1,   -1,
   -1,  493,  325,   -1,  327,   -1,   -1,  405,  331,   -1,
  427,   -1,   -1,  336,   -1,  432,  339,   -1,   -1,   40,
  315,  513,   43,  318,   45,  517,   -1,  444,   -1,  446,
  325,  354,  327,   -1,  432,   -1,  331,   58,   -1,   -1,
   -1,  336,   63,   -1,  339,  368,  444,   -1,  446,   -1,
   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,  493,   -1,   -1,   -1,
   -1,  376,  405,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,  493,  513,   -1,   -1,   -1,
  517,   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,  432,
  405,   -1,   -1,   -1,  315,  513,   -1,  318,   -1,  517,
   -1,  444,   -1,  446,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,  427,   -1,   -1,  336,   -1,  432,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,
  493,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,
   -1,   42,   -1,   -1,   -1,   -1,   -1,   -1,  389,  272,
  513,   -1,  275,   -1,  517,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,   -1,  405,  288,   -1,   -1,   -1,  292,
   -1,   -1,  295,   -1,   -1,   -1,   -1,  315,  513,   -1,
  318,   -1,  517,   -1,   -1,   -1,  427,  325,   -1,  327,
   -1,  432,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,  444,   -1,  446,  267,   -1,   -1,   -1,
   -1,   -1,  335,  274,   -1,   -1,  354,   -1,  341,  342,
  343,   -1,  345,  346,  347,  286,  349,   -1,   -1,   -1,
  368,  292,  293,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,   -1,  380,   -1,  367,   -1,  369,   -1,   -1,   -1,
  311,  389,  493,   -1,   -1,   40,   -1,   -1,   43,  320,
   45,   -1,  323,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,  513,   58,   -1,  398,  517,   -1,   63,   -1,
  403,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,  415,   -1,  432,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   -1,  444,   -1,  446,  281,
   -1,  372,   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,  472,
  473,   -1,  413,   -1,   -1,  493,   -1,   -1,  419,   -1,
   -1,   -1,   44,   -1,  425,   -1,  489,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,  513,   -1,   -1,   -1,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  505,  506,  507,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,  524,  525,  526,   -1,  528,  529,   -1,
  531,  532,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
  452,   -1,  267,  455,   -1,  457,  458,  459,  460,  274,
  462,   -1,   58,   -1,   -1,   -1,   -1,   63,  470,  471,
   -1,  286,   -1,   -1,  289,   -1,   -1,  292,  293,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,  311,   -1,  500,  501,
  502,  503,  504,   -1,   -1,  320,  508,  509,  323,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
  532,  462,   -1,  348,   -1,  350,  351,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  481,  482,   -1,  484,   -1,   -1,   -1,  372,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,   -1,   -1,   -1,   -1,   45,  413,   -1,
   -1,  532,   -1,   -1,  419,  317,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,  267,  487,  488,   -1,  490,  491,  492,  274,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
  286,   -1,   -1,  508,  509,   -1,  292,  293,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  522,   -1,  524,
  525,  526,   -1,  528,  529,  311,  531,  532,   40,   -1,
   -1,   43,   -1,   45,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
  452,   63,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,  348,   -1,  350,  351,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  481,
  482,   -1,  484,   -1,   -1,   -1,  372,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
  532,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   58,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,  522,   -1,  524,  525,
  526,   -1,  528,  529,   -1,  531,  532,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,  450,   -1,  452,   -1,   -1,  455,   -1,  457,
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
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,  267,  487,  488,   -1,  490,  491,
  492,  274,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  286,   -1,   -1,  508,  509,   -1,  292,
  293,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,  524,  525,  526,   -1,  528,  529,   40,  531,
  532,   43,   -1,   45,   -1,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,  348,   -1,  350,  351,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  522,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  532,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  521,  522,
   -1,  524,  525,  526,  527,  528,  529,  530,  531,  532,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   41,   42,   43,
   -1,   45,  274,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   60,   61,   62,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,  311,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   58,   42,   43,   -1,   45,   63,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   60,   61,   62,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  492,   -1,  494,  495,  496,  497,   -1,  271,  500,  501,
  502,  503,  504,  277,   -1,   -1,  508,  509,  282,   -1,
   -1,  285,  514,  515,  516,   -1,  518,  519,  520,   -1,
  522,   -1,  524,  525,  526,  260,  528,  529,  263,  531,
  532,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  321,   -1,   -1,
   -1,  286,   -1,  271,   -1,  329,   -1,  292,  293,  277,
   -1,   -1,   -1,  337,  282,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,  355,   -1,  357,   -1,  320,   -1,   -1,  323,   -1,
   58,   -1,   -1,   -1,   -1,   63,  370,   -1,  372,  373,
  374,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  329,   -1,  348,   -1,  350,  351,   -1,   -1,  337,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,
   -1,   -1,  370,   -1,  372,  373,  374,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,  462,  463,   -1,   -1,  325,   -1,  472,  473,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  522,   -1,  524,  525,  526,  260,  528,  529,  263,
  531,  532,   -1,  267,   -1,  395,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  430,   -1,   -1,   40,   -1,  435,   43,   -1,   45,   -1,
   -1,  441,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
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
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,  435,   43,   -1,   45,   -1,   -1,   -1,   -1,
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
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,
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
   -1,  428,   -1,  393,  261,   -1,  433,   -1,   -1,  399,
   59,   -1,   61,  403,   -1,   -1,   -1,   -1,   -1,  458,
  410,   -1,  279,  462,   -1,   -1,   -1,   -1,   -1,   -1,
  287,  458,   -1,   -1,   -1,  462,   -1,  294,  428,  296,
  258,  298,   91,  433,   -1,   -1,  303,   -1,   -1,   -1,
   -1,   -1,   -1,  310,   -1,  273,   -1,   -1,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,  324,  458,   -1,
   -1,   -1,  462,   -1,   -1,   -1,   -1,  334,   -1,  297,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,   -1,
  397,   -1,   -1,  303,   -1,   -1,  306,  404,   -1,   -1,
  407,   -1,  409,  410,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  393,   -1,  434,   -1,   -1,
   -1,  399,   -1,   -1,   -1,  403,   -1,   -1,  348,   -1,
   -1,   -1,  410,   -1,   -1,   -1,  260,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,
  428,   -1,  372,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  273,   -1,  483,  276,  277,   -1,
   -1,   -1,  281,  393,  298,  284,   -1,   -1,   -1,  399,
  458,  498,  499,  403,  462,   -1,  310,   -1,  297,   -1,
  410,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  523,   -1,  428,   -1,
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
"def_trigger_clause : symbol_trigger_name FOR simple_table_name trigger_active trigger_type trigger_position trigger_action end_trigger",
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
case 151:
{ yyval = NULL; }
break;
case 153:
{ yyval = NULL; }
break;
case 155:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 156:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 159:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 160:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 161:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
break;
case 162:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
break;
case 163:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 166:
{ yyval = (dsql_nod*) yyvsp[0]; }
break;
case 167:
{yyval = NULL;}
break;
case 170:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 171:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 172:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 173:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 174:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 175:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 176:
{yyval = NULL;}
break;
case 179:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 181:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 182:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 183:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 184:
{ lex.g_file  = make_file();}
break;
case 189:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 190:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 197:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 198:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 199:
{ yyval = yyvsp[0]; }
break;
case 200:
{ yyval = yyvsp[0]; }
break;
case 201:
{ yyval = NULL; }
break;
case 203:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 206:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 207:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 208:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 209:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 212:
{ yyval = NULL; }
break;
case 213:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 214:
{ yyval = yyvsp[0]; }
break;
case 215:
{ yyval = NULL; }
break;
case 216:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 217:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 218:
{ yyval = yyvsp[-1]; }
break;
case 219:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 226:
{ yyval = NULL; }
break;
case 229:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 230:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 233:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 234:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 235:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 236:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 237:
{ yyval = yyvsp[0]; }
break;
case 238:
{ yyval = NULL ;}
break;
case 243:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 244:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 245:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 246:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 247:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 248:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 249:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 250:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 251:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 252:
{ yyval = NULL;}
break;
case 253:
{ yyval = yyvsp[0];}
break;
case 254:
{ yyval = yyvsp[0];}
break;
case 255:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 256:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 257:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 258:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 259:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 260:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 261:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 262:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 263:
{ yyval = make_list (yyvsp[-1]); }
break;
case 264:
{ yyval = NULL; }
break;
case 265:
{ yyval = make_list (yyvsp[-1]); }
break;
case 266:
{ yyval = NULL; }
break;
case 268:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 269:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-4], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 271:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 272:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 273:
{ yyval = yyvsp[0]; }
break;
case 274:
{ yyval = yyvsp[0]; }
break;
case 275:
{ yyval = NULL; }
break;
case 276:
{ yyval = make_list (yyvsp[0]); }
break;
case 277:
{ yyval = NULL; }
break;
case 279:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 280:
{ yyval = yyvsp[-1]; }
break;
case 283:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 284:
{ yyval = NULL; }
break;
case 285:
{ yyval = NULL; }
break;
case 286:
{ yyval = yyvsp[0]; }
break;
case 287:
{ yyval = yyvsp[0]; }
break;
case 288:
{ yyval = NULL; }
break;
case 289:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 292:
{ yyval = yyvsp[-1]; }
break;
case 293:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 294:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 295:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 297:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 314:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 315:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 320:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
break;
case 321:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
break;
case 322:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 323:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
break;
case 324:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 325:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 326:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
break;
case 327:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 328:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 329:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
break;
case 330:
{ yyval = NULL; }
break;
case 331:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
break;
case 332:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 335:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 336:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 337:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 338:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 339:
{ yyval = NULL; }
break;
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 341:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 342:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
break;
case 343:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 344:
{ yyval = NULL; }
break;
case 346:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 357:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 358:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 359:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 360:
{ yyval = NULL; }
break;
case 361:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 362:
{ yyval = make_list (yyvsp[0]); }
break;
case 363:
{ yyval = make_list (yyvsp[-1]); }
break;
case 364:
{ yyval = NULL; }
break;
case 365:
{ yyval = make_list (yyvsp[0]); }
break;
case 366:
{ yyval = make_list (yyvsp[-1]); }
break;
case 367:
{ yyval = NULL; }
break;
case 368:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 369:
{ yyval = make_list (yyvsp[-1]); }
break;
case 370:
{ yyval = NULL; }
break;
case 372:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 373:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 374:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 375:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 376:
{ lex.beginning = lex_position(); }
break;
case 377:
{ lex.beginning = lex.last_token; }
break;
case 378:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					(lex.last_token == lex.last_token_bk) ?
					lex.last_token  - lex.beginning :
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
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 382:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
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
case 418:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 419:
{ yyval = make_node (nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0]); }
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = yyvsp[0]; }
break;
case 422:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 427:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 428:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 429:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 430:
{ yyval = yyvsp[0]; }
break;
case 431:
{ yyval = yyvsp[0]; }
break;
case 432:
{ yyval = make_node(nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 433:
{ yyval = make_node(nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 434:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], yyvsp[0], NULL); }
break;
case 435:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-3], NULL,
					make_node(nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0])); }
break;
case 436:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], NULL,
					make_node(nod_del_default, (int) 0, NULL)); }
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
					yyvsp[0], NULL, NULL, NULL, NULL); }
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
			yyval = make_node (nod_current_time, 1, yyvsp[0]);
			}
break;
case 949:
{ yyval = make_node (nod_current_timestamp, 1, yyvsp[0]); }
break;
case 950:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 951:
{ yyval = NULL; }
break;
case 952:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 954:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 956:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 957:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 958:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 959:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 960:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 961:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 963:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 964:
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
case 965:
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
case 966:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 967:
{ yyval = make_parameter (); }
break;
case 968:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 969:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 970:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 971:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 972:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 973:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 974:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 975:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 976:
{ yyval = yyvsp[0]; }
break;
case 977:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 979:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 980:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 981:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 982:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 983:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 985:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 986:
{ yyval = yyvsp[0];}
break;
case 990:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 991:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 992:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 993:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 994:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 995:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 996:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 997:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 998:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 999:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 1000:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 1003:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 1007:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1008:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1009:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1010:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
break;
case 1013:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 1014:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
break;
case 1015:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 1016:
{ yyval = yyvsp[0]; }
break;
case 1017:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 1018:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
break;
case 1019:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
break;
case 1020:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
break;
case 1021:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
break;
case 1022:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
break;
case 1023:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
break;
case 1024:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
break;
case 1025:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1026:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 1027:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 1030:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 1031:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 1032:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 1035:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1036:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1037:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1038:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1039:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 1040:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 1041:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 1042:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 1046:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			}
break;
case 1047:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 1048:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 1049:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 1050:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 1051:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 1052:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 1053:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 1054:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 1055:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 1060:
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
