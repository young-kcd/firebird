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
#define NEXT 518
#define SEQUENCE 519
#define RESTART 520
#define YYERRCODE 256
static short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    9,    9,    9,   23,   23,   22,   22,   22,
   32,   32,   27,   33,   33,   33,   33,   33,   26,   26,
   31,   31,   28,   15,   15,   15,   36,   36,   37,   37,
   25,   25,   38,   38,   38,   38,   40,   40,   40,   40,
   39,   39,   41,   41,   41,   29,   29,   46,   30,   30,
   47,   47,    6,   48,   48,   50,   55,   55,   55,   52,
   52,   52,   59,   59,   60,   62,   62,   62,   62,   53,
   53,   63,   63,   64,   64,   64,   64,   64,   49,   66,
   66,    5,   68,   68,   68,   68,   68,   68,   68,   68,
   68,   68,   68,   68,   13,   83,   83,   83,   14,   87,
   87,   70,   70,   73,   73,   73,   81,   96,   96,   96,
   97,   97,   98,   98,   99,   99,  103,  103,   80,  106,
  106,  108,  108,  109,  109,  112,  112,  113,  114,  114,
  115,  116,   78,   82,   79,  100,  100,  119,  120,  120,
  122,  122,  123,  123,  123,  123,  123,  121,  121,  124,
  124,  125,  125,  125,  104,  127,  128,  128,  129,  129,
  130,  130,  131,  131,  131,  102,  102,  102,   75,   85,
  132,  132,  132,  133,  133,  134,  134,  135,  135,  135,
  141,   92,   92,  137,  137,  110,  110,  105,  145,  146,
   61,  138,  138,  111,  111,  111,  111,  111,  111,  139,
  139,  153,  153,  154,  156,  156,  156,  156,  156,  136,
  155,  155,  160,  160,  160,  160,  162,  163,  164,  158,
  158,  159,  157,  157,  157,  157,  157,  165,  166,  167,
  167,  167,  167,   74,   84,   88,  172,  168,  168,  169,
  169,  173,  173,  175,  174,  174,  177,  176,  176,  176,
  170,  170,  178,  178,  179,  181,  181,  182,  180,  180,
  184,  184,  183,  186,  186,  171,  188,  188,  188,  189,
  189,  187,  187,  187,  187,  187,  187,  187,  187,  187,
  187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
  187,  192,  192,  193,  194,  195,  196,  201,  202,  197,
  197,  198,  211,  199,  212,  206,  206,  206,  207,  207,
  207,  209,  209,  209,  209,  200,  208,  208,  203,  203,
  203,  210,  210,  190,  190,  217,  218,  218,  219,  219,
  219,  219,  204,  204,  204,  221,  223,  222,  224,   11,
   12,  225,  225,  226,  226,  227,   77,   86,  143,  231,
   93,   95,  230,  230,   76,   89,  232,  232,  232,  233,
  236,  236,  237,  237,  237,  237,  237,  237,  237,  237,
  237,  237,  237,  237,  237,  237,  237,  234,  234,  235,
    2,  239,  239,  239,  239,  239,  239,  239,  239,  248,
  249,  245,  245,  250,  250,  250,  250,  250,  250,  250,
  240,  240,  251,  251,  251,  251,  251,  251,  251,  244,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  253,  253,  255,
  255,  254,  252,  252,  252,  246,  246,  247,  247,  242,
  243,  243,  260,  260,  260,  260,  260,  241,  261,  261,
  262,  262,    8,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  263,  263,  107,  107,  140,  140,
  266,  266,  269,  269,  271,  271,   56,   56,  268,  268,
  268,  268,  268,  268,  268,  268,  268,  275,  275,  267,
  267,  267,  267,  277,  277,  276,  276,  276,   58,   58,
  272,  272,  272,  270,  270,  270,  281,  281,  281,  280,
  280,  279,  279,  279,  273,  273,  282,  282,  282,  283,
  283,  274,  274,  274,  274,  284,  284,   19,   19,   19,
  286,  286,   17,   17,   17,  288,  289,  292,  292,  290,
  294,  294,    4,   16,  293,  293,  295,  295,  296,  296,
  285,  297,  297,  298,  298,  299,  299,  299,  299,  300,
  300,  301,  301,  302,  302,  304,  304,  304,  305,  305,
  305,  306,  306,  306,  303,  308,  308,  308,  309,  309,
  307,  307,  310,  312,  312,  311,  311,  287,   18,  313,
  313,  315,  315,  314,  314,  229,  319,  320,  316,  316,
  316,  321,  323,  332,  333,  334,  335,  324,  324,  324,
  324,  336,  336,  336,  337,  337,  337,  325,  325,  326,
  326,  339,  339,  340,  340,  341,  341,  327,  343,  343,
  344,  344,  346,  346,  346,  348,  349,  349,  350,  350,
  352,  352,  345,  345,  345,  353,  354,  355,  357,  357,
  358,  359,  347,  347,  360,  360,  361,  361,   24,  356,
  356,  356,  356,  356,  362,  362,  329,  329,  363,  363,
  364,  330,  330,  328,  328,  331,  331,  365,  366,  366,
  366,  366,  366,  367,  367,  368,  368,  369,  369,  370,
  370,  370,  371,  371,  372,  372,  317,  317,  373,  373,
  374,   71,   71,   71,  376,  376,  375,  375,  318,  318,
  318,   10,   10,    7,    7,  378,  379,  380,   20,   20,
  381,  382,  383,  383,  191,  205,    3,    3,  386,  386,
  386,  388,  388,  276,  387,  387,  389,  389,   34,   34,
   91,   90,   90,  377,  377,  390,  391,  391,  215,  215,
  215,  142,  384,  384,  117,  117,  117,  117,  117,  395,
  395,  395,  395,  395,  395,  394,  394,  394,  397,  397,
  397,  397,  397,  397,  397,  397,  397,  397,  397,  398,
  398,  398,  398,  398,  398,  398,  398,  404,  404,  404,
  404,  404,  404,  404,  404,  404,  404,  404,  404,  404,
  404,  404,  404,  409,  409,  399,  399,  400,  400,  401,
  401,  401,  401,  402,  402,  406,  406,  407,  407,  407,
  407,  405,  408,  403,  403,  393,  393,  393,  396,  396,
  396,  410,  410,  411,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,  152,  152,  152,  412,  214,  214,  147,  147,
  418,  418,  418,  418,  418,  414,  414,  414,  414,  414,
  228,  148,  148,  149,  150,  150,  150,  150,  150,   54,
   54,   21,   21,  238,  419,   57,  278,  258,  258,  101,
  259,  259,  413,  413,  413,  420,  420,  420,  420,  420,
  420,  420,  420,  420,  420,  420,  421,  423,  422,  422,
  425,  426,  426,  385,  385,  415,  416,  416,  427,  427,
  427,  428,  428,  429,  429,  432,  432,  430,  430,  435,
  435,  434,  431,  433,  417,  417,  424,  424,  424,  424,
  424,  424,  424,  424,  338,  338,  322,  322,  151,   51,
   67,  126,  144,  392,  161,  185,  265,   69,   65,  220,
  118,   72,  342,  216,   35,   44,  351,  264,   42,   45,
  213,   43,  291,  257,  257,  436,  436,  436,  436,  436,
  436,  436,  436,  436,  436,  436,  436,  436,  436,  436,
  436,  436,  436,  436,  436,  436,  436,  436,
};
static short yylen[] = {                                         2,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    8,    8,    5,    1,    0,    1,    2,    1,
    1,    3,    1,    1,    1,    1,    2,    2,    3,    0,
    3,    0,    1,    8,    8,    5,    3,    0,    3,    0,
    1,    1,    1,    3,    3,    3,    2,    2,    2,    2,
    1,    3,    1,    2,    2,    1,    3,    1,    1,    3,
    1,    2,    2,    2,    3,    8,    1,    1,    5,    0,
    1,    3,    1,    3,    3,    0,    2,    2,    1,    1,
    3,    3,    2,    0,    2,    2,    1,    3,    9,    1,
    1,    2,    3,    7,    2,    2,    2,    2,    2,    2,
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
    2,    3,    4,    2,    6,    4,    8,    9,    6,    8,
    6,    4,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    7,    2,    0,    2,    2,
    3,    3,    0,    1,    2,    4,    1,    3,    2,    2,
    2,    1,    1,    1,    1,    2,    2,    5,    0,    4,
    7,    3,    0,    1,    3,    3,    7,    7,    0,    0,
    0,    0,    3,    0,    9,    9,    1,    1,    0,    2,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    5,    5,    5,    5,    5,    5,    2,    0,    4,
    2,    3,    3,    2,    2,    3,    3,    2,    2,    3,
    6,    1,    2,    4,    3,    2,    2,    2,    2,    3,
    1,    3,    3,    3,    2,    2,    4,    4,    5,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    0,    2,    2,    4,    4,    0,
    1,    2,    2,    4,    3,    2,    2,    7,    1,    0,
    1,    0,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    3,    2,    2,    2,    2,    1,    1,    1,    1,
    4,    5,    1,    3,    1,    3,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    4,
    4,    6,    5,    3,    0,    2,    2,    0,    3,    0,
    4,    1,    5,    4,    1,    4,    1,    2,    2,    1,
    1,    1,    2,    2,    2,    2,    0,    3,    5,    1,
    1,    2,    3,    1,    2,    3,    0,    1,    1,    1,
    5,    5,    1,    1,    1,    2,    4,    1,    0,    5,
    1,    0,    3,    2,    1,    0,    2,    0,    1,    0,
    3,    1,    0,    1,    2,    1,    1,    1,    1,    2,
    2,    1,    2,    3,    1,    1,    3,    3,    1,    2,
    3,    1,    2,    0,    2,    1,    1,    0,    1,    1,
    1,    3,    2,    3,    0,    1,    3,    4,    3,    3,
    0,    2,    0,    2,    0,    3,    3,    3,    1,    4,
    4,    1,    9,    0,    0,    0,    0,    3,    2,    1,
    0,    3,    5,    3,    2,    5,    2,    1,    1,    1,
    1,    1,    3,    1,    3,    1,    0,    2,    1,    3,
    1,    1,    1,    1,    3,    6,    1,    0,    3,    0,
    1,    3,    1,    1,    1,    4,    5,    5,    1,    1,
    2,    4,    4,    2,    3,    0,    1,    2,    1,    1,
    2,    2,    2,    0,    1,    0,    3,    0,    1,    3,
    1,    2,    0,    2,    0,    2,    0,    4,    1,    2,
    1,    1,    0,    1,    3,    2,    1,    1,    2,    1,
    4,    3,    1,    3,    4,    0,    3,    0,    1,    3,
    3,    1,    1,    0,    1,    1,    4,    0,    2,    4,
    0,    8,    5,    1,    1,    7,    4,    4,    1,    1,
    8,    5,    1,    3,    3,    1,    7,    7,    5,    3,
    0,    1,    1,    1,    2,    0,    1,    1,    1,    0,
    3,    1,    3,    1,    0,    3,    1,    3,    1,    3,
    3,    1,    1,    3,    1,    2,    1,    3,    3,    1,
    2,    3,    3,    3,    3,    1,    3,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    3,    3,    3,    3,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    1,    1,    5,    6,    5,    6,    3,
    4,    5,    6,    3,    4,    3,    4,    3,    4,    4,
    5,    4,    4,    3,    4,    1,    1,    1,    1,    1,
    1,    1,    3,    3,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    2,    3,    3,    3,    3,
    3,    3,    3,    3,    1,    1,    1,    1,    3,    1,
    1,    1,    1,    1,    1,    4,    1,    3,    1,    2,
    1,    1,    1,    1,    1,    1,    1,    2,    2,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    1,    2,    1,    1,    1,    1,    1,    2,    1,
    1,    2,    1,    1,    1,    4,    5,    5,    5,    5,
    5,    5,    5,    5,    5,    5,    1,    6,    1,    4,
    7,    2,    0,    4,    3,    6,    1,    1,    6,    8,
    6,    1,    1,    4,    6,    4,    5,    3,    5,    4,
    5,    1,    1,    1,    4,    6,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    0,    1,    0,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    0,  558,  559,  560,  563,  564,  565,    0,  629,  632,
  744,  745,  749,  750,  470,    0,    0,    0,    0,    0,
    0,    0,  401,  575,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  122,    0,    0,    0,  102,    0,
  924,    0,   22,  922,    0,    0,   73,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  483,    0,    0,    0,   36,   33,   35,    0,   34,
    0, 1014, 1034, 1035, 1016, 1018, 1017, 1023, 1019, 1024,
 1020, 1022, 1025, 1021, 1026, 1027, 1028, 1030, 1031, 1029,
 1032, 1033, 1037, 1036, 1038,    0,    0,    0,    0,   31,
   68,   66, 1006, 1015,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  640,    0,    0,    0,
  687, 1008,    0,    0,    0,    0,    0,  115, 1013,  566,
    0,    2,    0,    0,    0,    0,    0,    0,  432,  433,
  434,  435,  436,  437,  438,  439,  440,  441,  442,  443,
  444,  445,  446,  447,  448,  449,  456,  450,  451,  452,
  453,  454,  455,  457,    0,  430,  431,    0,  998,    0,
  408, 1002,    0,  405, 1005,    0,  689,    0,  404, 1009,
    0,  409, 1001,    0,  573,  920,    0,  158,  111,    0,
  112,    0,  208,  994,  782,    0,  109,  153,    0,    0,
  105,    0,  113,  926,    0,  106,    0,  107,    0,  108,
 1012,  154,  114,  110,  732,  733,    0,  925,  923,    0,
   74,    0,  999,    0,  997,  491,  484,    0,  490,  495,
  485,  486,  493,  487,  488,  489,  494,  496,    0,    0,
    0,   29,    0,   38,  769,   37,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  930,    0,  911,  634,  634,  635,  645,  647,  985,  648,
    0,  649,  639,  635,    0,    0,    0,    0,    0,    0,
    0,  592,  581,    0,  584,  586,  587,  588,  589,  595,
  596, 1007,  688,    0,    0,  116,    0,  117,    0,  118,
    0,    0,    0,  619,    0,    0,  987,    0,    0,  626,
    0,    0,    0,    0,    0,  471,    0,    0,  371,    0,
  211,    0,  412,  402,  466,  467,    0,    0,    0,    0,
    0,    0,  421,    0,  377,  378,    0,    0,  579,  577,
  921,    0,    0,    0,    0,    0,    0,    0,  161,  140,
    0,  103,    0,    0,  119,    0,  129,  128,    0,    0,
    0,    0,    0,    0,  990,   75,    0,    0,    0,    0,
  747,  492,    0,    0,    0,    0,  888,  917,    0,  989,
    0,  890,    0,    0,  918,    0,  912,  903,  902,  901,
    0,    0,    0,    0,  893,  894,  895,  904,  905,  913,
  914,    0,  915,  916,    0,    0,    0,  919,    0,    0,
    0,    0,    0,    0,    0,  907,    0,  779,  885,  886,
  887,  892,  891,  360,  870,    0,    0,  869,    0,    0,
  874,  866,  867,  868,  871,  872,  873,  906,  933,  934,
  935,  947,  949,  957,  958,  962,  963,  209,    0,    0,
    0,  364,    0,    0,    0,  772,   26,    0,    0,    0,
    0,   71,   69, 1010,   67,   32,    0,    0,    0,  774,
    0,   47,   49,    0,    0,    0,  571,    0,    0,    0,
  642,  644,    0,  651,    0,    0,    0,  652,  638,    0,
  618,    0,  593,    0,  590,    0,  591,  616,    0,  611,
    0,    0,  585,  783,  753,    0,    0,    0,    0,    0,
    0,  568,  567,    0,  620,  624,    0,    0,  729,  631,
  630,    0,  176,    0,    0,  137,    0,  476,    0,  477,
  472,    0,    0,  416,  418,  417,    0,  419,    0,  413,
    0,    0,  262,    0,    0,    0,  425,  426,    0,  459,
    0,    0,    0,    0,    0,    0,    0,  382,  381,  479,
    0,    0,    0,  157,    0,    0,  166,    0,  165,    0,
    0,  172,  155,    0,  170,  162,    0,    0,  515,  550,
  518,    0,  551,    0,    0,  519,    0,    0,    0,  542,
  554,  514,  537,  516,  517,  512,  499,  371,  497,  498,
  500,    0,    0,  509,  510,  511,  513,    0,    0,    0,
    0,    0,  120,    0,  121,    0,  132,    0,    0,  192,
    0,    0,  369,    0,  211,    0,    0,   83,    0,  101,
    0,  100,  991,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  785,  787,  796,  799,  800,  801,  802,
  803,  804,  805,  806,  807,  808,  809,    0,    0,    0,
    0,    0,  908,    0,    0,    0,    0,    0,    0,  909,
  910,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  325, 1011,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  267,  507,
    0,    0,    0,  362,    0,    0,    0,  771,    0,    0,
   43,   72,    0,    0,   25,    0,  777,    0,    0,  743,
    0,    0,    0,    0,  570,    0,  634,    0,  656,    0,
    0,    0,    0,  931,    0,  928,  561,  562,    0,  594,
    0,  602,  598,  597,    0,    0,    0,  613,  601,    0,
    0,    0,  752,    0,    0,    0,  369,    0,    0,    0,
    0,    0,  138,    0,  475,  371,  415,    0,  371,  372,
  420,    0,    0,  258,  369,  231,  995,    0,    0,  371,
    0,    0,  200,  369,    0,    0,    0,    0,  236,  230,
  233,  234,  235,  424,  464,  463,  423,  422,    0,    0,
    0,    0,    0,    0,  380,    0,  371,  468,  469,    0,
  163,  167,    0,    0,  171,    0,    0,  764,    0,  538,
  539,  555,    0,  552,    0,    0,  545,  543,  544,    0,
    0,    0,    0,  508,    0,    0,    0,    0,  546,    0,
    0,  369,    0,  191,    0,  194,  196,  197,    0,    0,
    0,    0,    0,  211,   78,    0,    0,   77,    0,    0,
    0,    0,  786,  798,    0,    0,  859,  860,  861,    0,
  790,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  709,  711,    0,  706,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  977,  978,  979,  980,  981,  982,  983,  984,    0,    0,
    0,    0,    0,    0,  968,    0,    0,    0,    0,    0,
  883,  327,    0,  884,  955,    0,  879,  993,    0,    0,
    0,    0,    0,    0,    0,  889,  781,  780,  366,  365,
    0,  265,    0,    0,    0,  273,  773,    0,    0,    0,
   70,    0,    0,    0,  776,    0,    0,    0,    0,  643,
  646, 1003,  655,    0,    0,    0,    0,  661,  662,  663,
  664,  673,  674,  675,    0,    0,  653,  932,  929,  603,
  612,  607,  606,    0,  617,  784,  754,    0,    0,  369,
    0,    0,  636,  731,  730,    0,  474,    0,    0,  175,
    0,  179,    0,    0,    0,  414,  369,  369,  270,  372,
  263,    0,  202,  371,  204,    0,  372,  199,  205,    0,
    0,    0,    0,  427,  428,  371,  369,  372,    0,    0,
    0,  398,    0,  187,  188,  164,    0,  174,  526,  527,
  927,    0,    0,    0,    0,    0,  553,    0,    0,  371,
  372,    0,    0,  503,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  189,    0,    0,    0,
   82,    0,    0,    0,    0,   90,   84,    0,    0,   89,
   85,    0,  748,  996,    0,    0,  791,    0,    0,  797,
    0,    0,    0,    0,    0,  835,  834,    0,    0,    0,
  844,  862,    0,  854,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  789,    0,  710,    0,  746,    0,
    0,  211,    0,  936,    0,    0,  950,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  964,
    0,    0,    0,    0,    0,    0,    0,  975,  628,  954,
  896,    0,  260,  279,    0,    0,  361,  274,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   53,   61,   63,
    0,   41,    0,    0,    0,  758,  778,    0,  757,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  690,    0,
    0,    0,    0,    0,    0,    0,  609,  610,  614,    0,
    0,  190,    0,    0,    0,    0,    0,  180,    0,    0,
    0,  410,  214,  215,  216,  217,  218,  219,  899,    0,
    0,  264,    0,    0,  212,    0,    0,    0,    0,  237,
    0,  460,  461,  429,    0,    0,    0,    0,    0,    0,
  371,  481,  372,  173,  992,    0,    0,  521,    0,  520,
  556,    0,  548,    0,    0,    0,    0,  501,  529,    0,
    0,  531,  534,  536,  369,    0,    0,    0,  127,    0,
  195,  371,    0,  370,  104,    0,  125,    0,   93,    0,
    0,    0,    0,   87,   88,    0,  852,  853,  793,    0,
  795,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  855,    0,    0,    0,    0,    0,    0,    0,  845,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  717,    0,    0,    0,  942,  941,
    0,    0,  938,  937,    0,  946,  945,  944,  943,  940,
  939,    0,    0,  970,    0,    0,    0,  969,    0,    0,
    0,    0,  266,    0,    0,  276,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  359,    0,    0,    0,    0,  285,  290,  284,    0,    0,
    0,  294,  295,  296,  297,  298,  299,  301,  302,  304,
  305,  306,  309,    0,    0,    0,    0,  353,  354,  355,
    0,  756,   65,   57,   58,   64,   59,   60,    0,   23,
    0,    0,   24,  763,  762,    0,  760,  768,  767,  765,
  742,   44,   45,    0,  665,    0,    0,    0,  695,  693,
  691,    0,  692,    0,    0,    0,    0,    0,  751,    0,
  370,  735,  736,  637,    0,  185,  181,    0,  372,  900,
  268,  269,  372,    0,    0,    0,  222,    0,    0,  238,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  478,
  523,    0,  524,    0,  142,    0,    0,    0,  146,  148,
  149,  150,  506,  504,  502,  533,    0,    0,  372,    0,
    0,    0,  367,  371,   91,    0,    0,   97,   92,    0,
    0,    0,    0,    0,  863,    0,  864,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  719,  708,    0,
    0,  720,    0,  716,  210,  956,  976,  948,    0,    0,
  966,  965,    0,  971,  959,  961,    0,    0,  275,    0,
  314,    0,    0,    0,  308,    0,    0,  307,  339,  340,
    0, 1004,  356,  357,    0,  293,  300,    0,  303,  286,
    0,  291,    0,  344,  292,  310,  311,    0,    0,  337,
    0,   54,   55,   56,   62,    0,    0,  685,  683,    0,
  676,    0,    0,    0,  699,    0,  633,  372,  368,  737,
  182,  372,  152,  257,    0,    0,  198,  223,    0,    0,
    0,    0,  224,  227,    0,    0,  372,  396,  397,  393,
  392,  395,  394,    0,  522,  549,  151,  139,  147,    0,
  371,  254,  134,  372,  373,    0,    0,   95,    0,   79,
    0,  821,  829,  627,    0,  822,  830,  825,  833,    0,
    0,  823,  831,  824,  832,  818,  826,  819,  827,  820,
  828,  715,    0,    0,    0,  951,  967,    0,    0,  282,
  278,    0,  312,    0,    0,    0,    0,    0,  341,    0,
    0,  332,  333,    0,  352,    0,    0,    0,    0,  347,
  345,    0,    0,    0,   39,  759,    0,  667,  677,    0,
    0,  678,  679,  680,    0,  255,  411,  201,  206,  225,
    0,    0,  228,    0,  240,  242,  400,  372,    0,  375,
    0,   76,   98,   99,    0,    0,    0,  722,  960,  281,
    0,  313,    0,    0,  316,    0,  322,    0,  324,    0,
    0,  351,  350, 1000,  349,    0,    0,    0,    0,    0,
    0,  666,    0,    0,  700,  229,    0,    0,    0,    0,
    0,  256,  372,    0,    0,  721,    0,    0,    0,    0,
    0,    0,    0,  335,  334,  346,  348,    0,    0,    0,
  671,    0,    0,    0,    0,    0,  239,    0,  246,    0,
  245,  376,  126,  724,    0,  283,    0,    0,  315,  319,
    0,    0,    0,    0,    0,    0,  669,  682,  226,    0,
    0,  250,  249,  248,  725,    0,    0,    0,    0,    0,
  336,  672,  253,  251,  252,  330,  320,    0,  342,  317,
  318,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1442,   28,   29, 1443,
   31,   32,   33,   34,   35,   36,   37, 1444,   39, 1445,
  858,  136,  498,  161, 1235, 1480,  137,  750,  138,  501,
  755,  139,  140,  284, 1015,  150,  151, 1236, 1237, 1238,
 1239,  218,  249,  141, 1240,  142,  503,   87,  261,  406,
  455,  666, 1124,  456,  897,  637,  242,  874,  667,  668,
  669, 1131, 1126, 1569,  262,  671,  672,   79,  208,   80,
  257, 1795, 1345,  241,  246,  248,  250,  237,  229,  231,
  243,  253,  168,  336,  338,  340,  395,  653,  655,  495,
  285,  819,  573,  682, 1056,  399,  658, 1115, 1339,  605,
  776, 1086,  565,  566,  586,  391,  638, 1101, 1547, 1677,
 1282, 1548, 1549, 1550, 1551, 1552,  683,  238,  230,  387,
  613,  388,  389,  614,  615, 1314,  567, 1050, 1051, 1052,
 1277,  401,  885,  886,  887,  888,  821, 1067, 1525,  639,
  823,  458,  890,  977,  489, 1402, 1283,  459,  460,  461,
  462,  463, 1526, 1527,  589, 1683, 1829, 1300,  829,  830,
  816,  831,  832,  833, 1830, 1831, 1883,  368,  494,  994,
 1446,  214,  582,  991,  583, 1060,  490,  995,  996, 1225,
 1425, 1426, 1427, 1741, 1133, 1447, 1448, 1449, 1450, 1643,
 1451, 1452, 1453, 1454, 1455, 1456, 1457, 1458, 1459, 1460,
 1461, 1462, 1463, 1464, 1465,  464, 1840, 1466, 1751, 1874,
 1748,  465,  727,  466,  467, 1467, 1644, 1759, 1760, 1813,
 1468, 1469, 1470, 1635,  281,  491,  492,  468,   41, 1344,
 1563,  377,  600,  847, 1312,  601,  845,   84,   63,  372,
  219,  178,  355,  205,  362,  211,  222,  810,  807,  363,
  373,  837,  374,  596, 1078,  206,  469, 1102,  778,  356,
  602, 1313,  102,  217,  266,  640,  641,  740, 1103,  741,
 1104,  644,  645,  646,  647, 1485, 1095, 1093,  648,  649,
  650,  867,  651,  864,   42,   43,   44,   45,   46,   47,
  170,  553,   65,  518,  225,  380,  323,  324,  325,  326,
  327,  328,  329,  330,  331,  783,  539, 1034, 1269,  540,
  541,  788,  174,  344,  555,   48,  177,  350, 1368,  726,
   49,  348,   50,  155,  311,  526,  772,  410, 1266, 1508,
  699,  521,  313, 1274, 1670,  156,  157,  312,  527,  528,
  770, 1013, 1016, 1017, 1018, 1019, 1020, 1021, 1767, 1822,
  470, 1852, 1022, 1023, 1024, 1264, 1772, 1773, 1774, 1256,
  164, 1500, 1664, 1665, 1395,  935, 1396, 1397, 1398, 1604,
 1796, 1798,  558,  559, 1044, 1514,  509,   51,   52,  411,
   53,   54,  547,  548,  471, 1003, 1246, 1486, 1490,  510,
  758,  235,  684,  685,  912,  913,  686,  687,  688,  689,
  690,  691,  692,  693,  694,  695,  696,  697, 1149, 1151,
 1152,  472,  473,  474,  475,  476,  477,  478,  259,  479,
  480,  481,  482,  959,  483, 1610,  484,  485,  486,  487,
  715,  963, 1213, 1208,  716,  144,
};
static short yysindex[] = {                                   5315,
 2533, -210, 3017,   68, 1381,  -39, 3116,  380,10236,  588,
   12,   -7, -210, 1203,  -50, 9737, 1051, 9737, -183,    0,
  263,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   44,    0,    0,    0,    0,    0,    0,  -52,    0,    0,
    0,    0,    0,    0,    0,15824, 9737, 9737, 9737, 9737,
 9737, 9737,    0,    0,   11, 1284, 9737, 9737, 9737,  181,
 9737,   55, 9737, 9737,    0, 9737, 9737, 9737,    0,  915,
    0,  229,    0,    0,  364, 9737,    0, 9737, 9737, 9737,
  470, 9737, 9737, 9737, 9737,   55, 9737, 9737, 9737, 9737,
 9737,    0, 9737,  680,  433,    0,    0,    0,  866,    0,
  866,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  582,  635,  116, 1006,    0,
    0,    0,    0,    0, 9737, 9737, 9737,  773,  884,  852,
 9737,  747,  158,  261,  574,  811,    0, 9737,  968, 1698,
    0,    0, 9737,  921, 9737, 9737, 9737,    0,    0,    0,
 9737,    0,  849,  911, 1091, 1046,  861, 1416,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1047,    0,    0, 1284,    0,   62,
    0,    0, 1341,    0,    0, 1309,    0,  401,    0,    0,
  960,    0,    0, 1021,    0,    0, 1036,    0,    0, 1258,
    0, 1280,    0,    0,    0, 1284,    0,    0,  370, 1341,
    0,  647,    0,    0, 1180,    0, 1204,    0,  866,    0,
    0,    0,    0,    0,    0,    0, 1189,    0,    0, 1080,
    0, 1249,    0, 1117,    0,    0,    0, 1080,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,14894, 9737,
 1186,    0, 9737,    0,    0,    0, 1171, 1198, 9495, 9737,
 1264, 1279, 1579, 1317, 1330, 1355, 1299, 1303,   71, 1210,
    0,15148,    0,    0,    0,    0,    0,    0,    0,    0,
12926,    0,    0,    0, 1290, 9737, 1329, 1285,  564, 9737,
 1324,    0,    0, 1698,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 9737, 1341,    0, 1180,    0,  866,    0,
 1352, 1364, 1260,    0,15148, 1401,    0, 1401,15148,    0,
  -31, 1274, 1275, 1315, 1416,    0, 1537,  653,    0, 9737,
    0, 1047,    0,    0,    0,    0, 9737, 1186, 9195, 1388,
 9255, 1781,    0,15824,    0,    0,  859, 1383,    0,    0,
    0, 1773, 1773, 1284, 1485, 1284,  581, 1258,    0,    0,
 5553,    0, 9737, 9737,    0, 1186,    0,    0, 1572,  677,
 1823, 9737, 1609, 9737,    0,    0, 1835, 3785, 9371, 1488,
    0,    0, 1842, 1846, 1848, 1449,    0,    0, 1880,    0,
 1895,    0, 1902, 1904,    0, 1905,    0,    0,    0,    0,
    0, 1906, 1477, 1483,    0,    0,    0,    0,    0,    0,
    0, 1907,    0,    0,13180, 1910, 1911,    0, 1914, 1561,
15148,15148,13418, 9737, 1916,    0, 2141,    0,    0,    0,
    0,    0,    0,    0,    0, 1919, 1874,    0,    0, 1920,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 5553, 1909,
 1536,    0, 1927, 1707, 1599,    0,    0, 9737, 9737, 9737,
  142,    0,    0,    0,    0,    0, 9737, 9737,  824,    0,
 9737,    0,    0, 1171, 1577, 9495,    0, 9737,13418, 1588,
    0,    0,15148,    0, 1896, 1660, 1946,    0,    0,   77,
    0,  966,    0,  -74,    0,  -74,    0,    0, 1947,    0,
  119, 1578,    0,    0,    0, 1954,  123, 1931, 1186, 1962,
 1739,    0,    0, 9737,    0,    0, 1575, 1966,    0,    0,
    0,  771,    0, 1668, 1671,    0, 1284,    0, 1672,    0,
    0, 1735, 1742,    0,    0,    0, 1722,    0, 5553,    0,
 5553, 1625,    0, 1758, 9737,15666,    0,    0,  807,    0,
 9737,  941, 1309, 1630, 1598, 1567,    0,    0,    0,    0,
  670, 1640,   77,    0, 1582,   55,    0, 1284,    0, 1764,
 1706,    0,    0,  581,    0,    0,  178, 1617,    0,    0,
    0, 1621,    0, 1674, 2027,    0, 1729, 2030,    8,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1984,  138,    0,    0,    0,    0,  112, 2039, 2041,
 2030, 1341,    0, 1785,    0, 1837,    0, 1284, 1284,    0,
 9195,  401,    0, 1731,    0, 1710, 2064,    0,11141,    0,
 1727,    0,    0, 1744, 2083, 9609, 2084,    0,    0,    0,
 9117, 6954,   67,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  698, 1751, 1166,
15148,  722,    0, 9737,15148, 1240, 1255, 1256, 2287,    0,
    0,15148, 9863, 2141, 1691,  521,15148,15148, 9863, 1821,
  204,  204, 1711, 1690,  -52, 2098,    0,    0,13672, 9737,
15148,15148,15148,15148,15148,15148,15148, 6561,    0,    0,
 1867, 2080, 9737,    0, 9737, 1849, 9737,    0, 1716, 1719,
    0,    0, 1694, 9495,    0, 1834,    0, 1701, 2114,    0,
 1834, 9737, 9737, 2112,    0, 1711,    0, 1850,    0, 9737,
 2885, 1715,15148,    0,  992,    0,    0,    0,  -30,    0,
 1730,    0,    0,    0, 9737, 1043, 9737,    0,    0, 9737,
 9737, 1488,    0,15148, 1898, 9195,    0, 2121, 1677,15148,
15148, 1284,    0,  -34,    0,    0,    0, 2135,    0,    0,
    0,  173, 9737,    0,    0,    0,    0, 1903, 2137,    0,
 1893, 1915,    0,    0, 1852, 1857,  866, 1928,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   55, 9737,
15764, 1829, 1831, 1832,    0,   55,    0,    0,    0, 1338,
    0,    0, 1796, 1284,    0, 3785,   80,    0, 1799,    0,
    0,    0,   55,    0, 2027,   85,    0,    0,    0, 1918,
   85, 1802,   85,    0, 2174,   55,   55,   55,    0, 1186,
 9737,    0, 1865,    0, 1705,    0,    0,    0,  859, 1401,
 9737, 1725,  135,    0,    0, 2176,   -8,    0, 3785, 9737,
 1401,10101,    0,    0, 1401, 9609,    0,    0,    0, 6293,
    0,  269,  134,15148,15148,10362, 2184,  786,10623,15148,
10884,  797,11145,11406,13910,11667,11928,12189, 9863, 9863,
    0,    0, 1868,    0, 2189,  861,15148,15148, 2045,15148,
 2186,15148, 2191, 1917,15148,15148,15148,15148,15148,15148,
    0,    0,    0,    0,    0,    0,    0,    0, 1922, 1247,
  504,15148,  721,15148,    0, 9863, 2554, 2726,  400, 9737,
    0,    0,  861,    0,    0, 1734,    0,    0, 1964,  204,
  204, 1286, 1286, 2141, 1076,    0,    0,    0,    0,    0,
 1756,    0, 1807, 1972, 1849,    0,    0,12322,12322, 1869,
    0,  717, 1887, 9737,    0,15148, 1887, 1932, 1933,    0,
    0,    0,    0, 1543, 2211, 2208, 1663,    0,    0,    0,
    0,    0,    0,    0, 9863, 1934,    0,    0,    0,    0,
    0,    0,    0,  741,    0,    0,    0, 1751, 2141,    0,
 1767, 1401,    0,    0,    0, 2141,    0, 1773, 1990,    0,
  -34,    0, 2218, 9863, 5057,    0,    0,    0,    0,    0,
    0, 1849,    0,    0,    0, 5057,    0,    0,    0,  866,
  866, 1768, 2220,    0,    0,    0,    0,    0,  417,  609,
  977,    0, 2003,    0,    0,    0, 9737,    0,    0,    0,
    0,   68, 1776, 1859, 1867, 2230,    0, 1582, 1778,    0,
    0, 2219, 1099,    0, 9737, 1105,   55, 2237, 2238, 2246,
 2024,  401, 1849, 1773, 1671, 9195,    0, 1640, 1844, 1602,
    0,   55, 1913, 1992,11141,    0,    0,   55, -175,    0,
    0, 1995,    0,    0, 2261, 2262,    0, 9863, 9863,    0,
 9863, 9863, 1594, 2141, 2264,    0,    0, 2141, 2265,13418,
    0,    0, 1989,    0,   -2, 2271, 2141, 2273, 1491, 2275,
 2141, 2278,15148,15148, 2184,15148,14164, 2279, 2141, 2280,
 2281, 2141, 2284,15148, 2141, 2285, 2141, 2286, 2288, 2141,
 2289, 2290, 2141, 2291,    0, 2070,    0, 9986,    0, 2035,
 2110,    0, 2239,    0, 2252,15148,    0, 2295, 2412, 2426,
 2441, 2497, 2548,15148,15148,15148, 2141, 1912,15148,    0,
15148, 2141, 2036,  606,15148,15148,15148,    0,    0,    0,
    0, 9737,    0,    0, 9737,14294,    0,    0, 9737, 9737,
 9737, 9737, 9737, 9737, 1897, 2300, 2303,    0,    0,    0,
 1897,    0,  230,  230,  169,    0,    0, 1786,    0,12322,
12322, 2308, 1663, 2309,15148, 2087, 2885, 1971,    0, 1971,
  946, 1971, 2015, 2021, 2089, 2042,    0,    0,    0,  861,
 1849,    0, 1844,  742, 1582, 1979, 1582,    0, 9863,  340,
 1232,    0,    0,    0,    0,    0,    0,    0,    0, 5057,
 5057,    0, 1972,15148,    0, 2090, 1976, 1768,  915,    0,
 9863,    0,    0,    0, 1997, 2006, 2009, 2011, 2012, 2014,
    0,    0,    0,    0,    0, 2330,   68,    0, 1944,    0,
    0,   85,    0, 5057, 2023,   85,   85,    0,    0, 1867,
 2356,    0,    0,    0,    0,  859, 1972, 1582,    0, 1671,
    0,    0, 2123,    0,    0, 2121,    0, 2360,    0, 2362,
 1284,  -69, 2363,    0,    0, 1284,    0,    0,    0, 2070,
    0, 2070,15148, 1401, 1401, 1794,  -52, 2364,15148, 2091,
    0, 1401, 1401,15148, 1401, 1401, 1771, 2141,    0, 1560,
15148, 2141, 1401, 1401, 1401, 1401, 2141, 1401, 1401, 1401,
 1401, 1401, 1401, 9737,    0, 2365, 2357,  725,    0,    0,
 5553, 2368,    0,    0, 2566,    0,    0,    0,    0,    0,
    0, 2615, 1313,    0,15148, 2115, 1994,    0,15148, 2776,
 1795, 2862,    0, 5553, 2367,    0,    0, 2129,    0, 5274,
  451, 2375, 2384, 2099,15148, 2377, 2382, 5563, 9737, 9737,
    0, 2385, 2387, 2107, 2389,    0,    0,    0, 2122,12818,
 2393,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2397, 2403,   30, 2406,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2142,    0,
12322,12322,    0,    0,    0, 2037,    0,    0,    0,    0,
    0,    0,    0, 2087,    0, 1800, 9737, 1663,    0,    0,
    0, 2131,    0, 2885, 2885,15148, 9863, 1488,    0, 1972,
    0,    0,    0,    0, 1338,    0,    0,  409,    0,    0,
    0,    0,    0, 2141, 2195, 2090,    0, 1017, 9737,    0,
 2144,  412, 2046, 2145, 2047, 2177, 2151, 2193, 1849,    0,
    0, 2446,    0, 2451,    0, 2147, 2195, 2023,    0,    0,
    0,    0,    0,    0,    0,    0, 1849, 1640,    0, 1338,
 2003, 2118,    0,    0,    0, 2128,  -57,    0,    0, 1867,
 2134, 2141, 2459, 2460,    0,  861,    0, 2141,15148, 2463,
 2465, 2141, 2466, 2467,15148,15148, 2141, 2470, 2472, 2478,
 2479, 2481, 2483, 2484, 2486, 2487, 2489,    0,    0, 9986,
 2491,    0, 9737,    0,    0,    0,    0,    0,15148, 2494,
    0,    0,15148,    0,    0,    0,15148,  260,    0, 2217,
    0,14402, 9737,15148,    0, 9863, 2141,    0,    0,    0,
 2477,    0,    0,    0, 9737,    0,    0, 4085,    0,    0,
  626,    0, 2094,    0,    0,    0,    0,   16, 2503,    0,
 2167,    0,    0,    0,    0,  230, 9737,    0,    0, 2885,
    0, 1130, 2141, 2502,    0,   67,    0,    0,    0,    0,
    0,    0,    0,    0, 2506, 9737,    0,    0, 2203, 2212,
 9737, 1768,    0,    0,  866, 9737,    0,    0,    0,    0,
    0,    0,    0, 1972,    0,    0,    0,    0,    0, 1972,
    0,    0,    0,    0,    0,15148, 1284,    0, 2100,    0,
 1284,    0,    0,    0, 2141,    0,    0,    0,    0, 2141,
 2141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 9737, 2215, 2141,    0,    0, 2853, 5057,    0,
    0, 2516,    0, 1856,14894, 1277,  463, 2498,    0, 2223,
 1459,    0,    0, 2523,    0, 9737, 9737,   68,  233,    0,
    0, 2062, 2235, 9863,    0,    0, 2534,    0,    0, 9863,
 2535,    0,    0,    0,15148,    0,    0,    0,    0,    0,
 1768,  866,    0, 2197,    0,    0,    0,    0, 2003,    0,
 2141,    0,    0,    0, 2536, 2537, 2539,    0,    0,    0,
 1401,    0, 2179, 4085,    0, 2155,    0, 4085,    0, 4085,
 3185,    0,    0,    0,    0,14294,  626,15148, 4085,  657,
 9737,    0,   67, 9737,    0,    0, 2197,  640, 1768, 2207,
 2210,    0,    0, 2546, 9737,    0, 9737, 2562, 2585, 2545,
 1542,14294, 2571,    0,    0,    0,    0, 1375,  386, 2315,
    0, 1804, 1806, 1768,  737,  737,    0, 2312,    0, 2183,
    0,    0,    0,    0, 2577,    0, 4085, 2571,    0,    0,
 2317, 4085, 2332, 2322,14294, 9737,    0,    0,    0, 2169,
  817,    0,    0,    0,    0, 1828,14294,  247, 9737,14294,
    0,    0,    0,    0,    0,    0,    0,14294,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  197,  792,    0,    0,    0,    0,    0,    0,    0,
    0,10497,  128,12427,    0,    0,    0,    0,    0,    0,
 2626,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  227,    0,    0,    0,    0,    0,    0,  100,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  917,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2294,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   28, 2253,    0,    0,    0,  264,    0,
  264,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2254,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1040,    0,    0,14656,12688,    0,    0,    0, 1176,
    0,    0,  465,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  687,    0, 2227,  357,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  612,    0,    0,    0,    0,  873,    0,    0,
    0,    0,    0, 1184,    0,    0,    0,    0,    0,  151,
    0,11663,    0,    0,    0,    0,    0,    0,    0,  612,
    0,  431,    0,    0, 2588,    0,    0,    0, 2373,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  117,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1188,    0,
 2376,    0,    0,    0,    0,    0,10772,    0,    0,    0,
    0,    0, 1137,    0,    0,    0,    0,    0,    0,11033,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  713,    0,    0, 1190,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  612,    0, 2588,    0, 2373,    0,
 1229,  681,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1245,    0, 2366,    0,    0,    0,
    0, 1271,    0,    0,    0,    0,    0, 2376,  819,15893,
    0, 1282,    0,    0,    0,    0,  153,    0,    0,    0,
    0, 2198, 2198,    0,    0,    0, 1318,  195,    0,    0,
    0,    0,    0,    0,    0, 2376,    0,    0, 1408,    0,
    0,    0,    0,    0,    0,    0,10880,    0,    0,  102,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1864,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2670,    0,    0,    0, 2970, 3270,    0, 3570, 3870,
    0,    0,    0,    0,    0,    0,  378,    0,    0,    0,
    0,    0,    0,    0,    0,  240, 4170,    0, 2370,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1337,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10772,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3485,    0, 2333,    0,    0,    0,
    0,    0,    0,  839,    0,  839,    0,    0,  857,    0,
 1063, 1065,    0,    0,    0,    0,  117,    0, 2376,    0,
    0,    0,    0,    0,    0,    0,  165,  420,    0,    0,
    0,  629,    0,    0, 1144,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2366,    0,
    0, 1269,    0, 2192,    0,    0,  950,    0,    0,    0,
    0,  806,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1348,    0,    0, 1486, 2509,    0,    0,
    0, 4953,    0,    0,12216,    0,    0,12953,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1656, 2161,    0,    0,    0,    0,13703, 9894,    0,
12953,  612,    0,    0,    0,    0,    0,    0,    0,    0,
  819, 1084,    0,    0,    0,    0, 2236,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 7147, 7226, 7566,
    0,    0,  132,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2609,  111,15386,
    0,15386,    0,    0,    0,15386,15386,15386,    0,    0,
    0,    0,    0, 2216,    0,    0,    0,    0,    0,    0,
 5359, 5648, 2607,    0,   57,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1268,    0,    0,    0,    0, 2383,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  304,    0,    0,    0,    0,
  304,    0,    0, 1368,    0,    0,    0,    0,    0,    0,
    0,  627,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  756,    0,    0,    0,    0,
    0,  102,    0,    0,    0,  819,    0,  869,  382,    0,
    0,    0,    0,  214,    0,    0,    0,    0,    0,    0,
    0, 1843,    0,    0,    0,    0,    0, 2621,    0,    0,
 1890, 1692,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  352,  403,  623,    0,    0,    0,    0,    0,  886,
    0,    0,    0,    0,    0,    0,    0,    0, 4461,    0,
    0,    0,    0,    0,12216,    0,    0,    0,    0,  414,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2376,
    0,    0,  302,    0,    0,    0,    0,    0,    0,    0,
    0,    0,11402,    0,    0,    0,  241,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1907,    0,    0, 1373,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2622,    0,    0,    0,    0,    0, 4470, 5937,
 6226, 4770, 5070,  877,    0,    0,    0,    0,    0,    0,
    0,    0,11294,    0, 2394,    0,    0,    0,    0,    0,
    0,    0, 1379,    0,    0,    0, 1379,    0,    0,    0,
    0,    0,    0,    0, 6435,  821,  211,    0,    0,    0,
    0,    0,    0,    0,    0,  870,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  111,  194,    0,
    0,    0,    0,    0,    0,  651,    0, 2198, 2229,    0,
  988,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2383,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1415,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1386,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 4761,    0,    0,    0,    0,    0,
    0, 1187,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1084, 2383, 2198, 1409,  819,    0, 2401, 1411,    0,
    0,    0,11402,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1008,    0,    0,    0, 2071,    0,    0,
    0,    0,    0,    0,    0,    0, 2187,    0, 6144,    0,
 7372,    0,    0,    0,    0,    0,    0,    0, 7645,    0,
    0, 7724,    0,    0, 7803,    0, 7933,    0,    0, 7961,
    0,    0, 8081,    0,    0, 6477,    0, 2609,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2240,    0,    0,    0,
    0,  750,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  349,    0,    0,    0,    0,
    0,    0,    0,    0, 1478,  166,  201,    0,    0,    0,
 1478,    0, 2245,  305,    0,    0,    0,    0,    0,    0,
    0,    0, 2327, 1257,    0, 6848,    0, 2338,    0, 2338,
 2327, 2338,    0,    0,    0,  172,    0,    0,    0, 1373,
 2383,    0, 1411,    0,    0, 2232,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1528,    0, 1415, 2294,    0,
    0,    0,    0,    0,  689,  782,  888,  897,  927,  955,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  726,    0,    0,    0,    0, 2213,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1494,
    0,    0,    0,    0,    0, 1499,    0,    0,    0,    0,
    0,  161,    0,    0,    0,    0,    0,    0,    0,  149,
    0,  207,    0,    0,    0,    0,   57,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8165,    0, 8211,
    0, 8295,    0,    0,    0,    0, 8378,    0,    0,    0,
    0,    0,    0,  876,    0,    0, 2643,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2645,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,13229,    0,
    0,    0,    0,    0,    0,    0, 1563, 1644,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  587,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1667,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 6641,    0,    0,    0,  295,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  217,    0,    0,
    0,    0,    0,    0,  863,    0,    0,    0,    0,    0,
    0,    0,    0, 2649, 1423, 1765,    0, 2366,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2383,    0,
    0,    0,    0,    0,    0,    0, 1517,  743,    0,    0,
    0,    0,    0,    0,    0,    0, 2383, 2401,    0,  332,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  661,
    0, 8453,    0,    0,    0, 2622,    0, 8583,    0,    0,
    0, 8666,    0,    0,    0,    0, 8694,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2609,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2633,    0,    0,
    0,    0,    0,    0,    0,    0, 2634,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  305, 7466,    0,    0,    0,
    0, 2327, 1044, 1039,    0,  435,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2561,    0,    0,  140,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  187,    0,
    0,    0,    0,    0, 8816,    0,    0,    0,    0, 8846,
 8888,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1872, 2657,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  185,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  525,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2561, 1115,    0,  148,    0,    0,    0,    0,    0,    0,
 2649,    0,    0,    0, 2661,    0,    0,    0,    0,    0,
    0,    0, 2646,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  385,    0,    0,    0,    0,
    0,    0, 8999,    0,    0,    0, 1167,    0, 1415, 1266,
 1308,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  385, 2660,    0,    0,    0,    0,    0, 2419,    0,
    0,    0,    0, 2561,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2662,    0,    0,
15484,    0,    0,    0,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  385,    0,    0,  385,
    0,    0,    0,    0,    0,    0,    0,  385,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2722,    0,    0, 2728,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2733,
   -3, 2576, 2221,   22, -222, 1493, 2586, 1974, 2590, 2231,
    0,    0, 2448, -106,    4,    0,    0,    0,    0,  436,
  447,  -60,  -70,  -61, -255, 2455, 2008,    0,    0,    0,
  652,    0,    0,  -53, 1635, -546,  -66, -975, 2101, 1870,
 -333,    0, 1653,    0, 2685, 1879, 1924,    0,  -65,    0,
 -516,  -54,    0,    0,    0,    0,    0, 2703,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -548,
 -171, 1669, -332, 1059, -764,    0,    0,    0,    0, -340,
 -136, -935, 1673, -365,  -55,    0, -553,    0,    0, 1236,
-1024,    0, 1239,    0,    0, 2435, -687,  -27,    0,    0,
    0,    0, 2407,    0, 2180, 1695,    0,    0,    0, 1745,
    0, 2462, 2005, 1687, 2436, 2438,    0, 1191,    0, -465,
 1986,  -44, -619, 1134, -220,    0,    0, -947, -922, -920,
 -916, -884,    0, 1294,-1132,    0,  984,-1222, 1298,    0,
 2241,    0,    0,    0,  996,  998,  973, -115, -290, -936,
 -937,    0,    0,    0, 2020,    0, -699,    0, 1851,    0,
    0,    0,    0,    0,-1152, -781,    0,    0,    0,    0,
 -216,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1094,    0,    0, -414,    0,
    0, 1072,    0, -417, 1110, 1406, 1206,    0, 1030,    0,
    0,    0,    0,    0,    0,    0, 2113, -143, -405, 1580,
 1339, -550, -755, -973,-1326,    0,    0,  -41,    0,    0,
    0,    0,    0, 2485,    0,    0,    0,    0,    0, 2493,
 2267,    0,    0,    0,    0,    0,   -9, -493, 2255, 2513,
    0,    0,    0,   -1,    0,    0,    0, -371, 1996, -370,
 1548,    0,    0,    0,    0, 2276,    0, -981,    0,    0,
    0, 2248,    0, 2038,    0,    0,    0,    0,    0,    0,
   76,    0, 2888,    0,    0,    0,    0,    0, 2589,    0,
    0,    0,    0, 2378,    0, 2379,    0,    0,    0, 2132,
    0,    0,    0,    0,    0, -392, -655, -880, 1827,    0,
 1358,    0,    0,    0,    0,    0,    0, -306,    0,    0,
 -738, -196, 1217,    0,    0,    0, 2760, 1079,    0, 2148,
-1076, -950,    0, -909, 1908,-1274,    0,    0,    0,    0,
 -155,    0,    0,    0,    0, 1662,    0,    0,    0,    0,
 2836,  344,    0, 1151, 2233,    0, 1327,    0, 1544,    0,
 -121,    0,    0, 2130,    0,    0,    0,    0,    0, 2396,
    0,    0,    0, -436,-1181, 2175, 1938,-1115,    0,    0,
    0, -719, -450, -473,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2200, 1775,
    0,    0,    0, -881,    0,    0,    0, 1665,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -987, 1737,    0,    0,
};
#define YYTABLESIZE 16413
static short yytable[] = {                                     143,
   83,   38,  236,  210,  286,  798,  162,  333,  169,  305,
  308,  232,  228,  247,  163,  252,  304,  307,  988,  642,
  643,  612,  233,  739,  267,  961,  577,  579,  276,  273,
  244,  969,  820,  502,  221,  724,  777,  275,  277,  271,
  799, 1295,  606,  936, 1472,  992,  207,  209,  212,  215,
  162,  220,  223, 1038,  244, 1189, 1227,  234,  209,  223,
  725,  215,  213,  162,  220,  270,  251,  143,  223,  973,
 1036,  757, 1428,  278,  240, 1530,  263,  584,  162,  265,
  209,  216,  263,  223,  212,  215,  163,  162,  220,  251,
  143,  223, 1219,  215,  245,  274,  339,  728,  272,  728,
  292,  707,  294,  760, 1253,  656,  279, 1284,  522,  848,
  728,  889,   82,  811,  290,  812,  705,  545, 1284, 1320,
  822,  775,  898, 1092,  396, 1293,  725,  576, 1487, 1098,
  315,  704, 1285, 1118, 1286,  234,  162,  234, 1287,  770,
  728,  143,  403, 1285, 1342, 1286,  581,  247,  223, 1287,
  159,  876,  480,  332,  364,  215,  162,  251,  728,  290,
  707,  169,  787, 1528,  734,   51,  791,  293,  335,  728,
 1288,  703,  704, 1289, 1123,  705, 1337, 1042,  546, 1497,
  770, 1288,  392,  770, 1289,  754,  576,  337,  247,  792,
  704,  247, 1059,  755,  160, 1062,  576,  302,  770,  803,
   52,   94,  904, 1567, 1069,  734,  247,  911,  734,  159,
  659,  480,  703,  177,  642,  643,  707,  857, 1414,  549,
  303, 1416,   82,  734,   51,  903,  621,   96,  873, 1661,
  703,  303,  551, 1058, 1704,  488,   64,  755,  496,  326,
  792, 1185, 1186,  328,  752,  734,  341,  794,  612,  534,
  735,  659,  755,  160,  659,  576,  828,  707,  795,   52,
  502,  531, 1113, 1489, 1129, 1521, 1522,  621, 1472,  659,
  234,  158,  177,  234,   82,  707, 1817,   88, 1214,  504,
  143,   86,  868,  147,   86,  621, 1633, 1634,  725,  544,
 1810,  363,  303, 1708,  660, 1292,  781, 1370,  326, 1545,
  306,  133, 1296,  761,  528,  870,  212,  770, 1354, 1140,
  162,  976,  563, 1304,  171,  578,  148, 1048,  365,  985,
 1739,  172,  488,  303,  233, 1762,  592,  175,  929,  594,
  607,  186,  609,  654, 1510,  660, 1325, 1543,  660, 1130,
 1355,  538, 1284, 1284, 1648, 1371,  660, 1498,  869,  664,
  234,  385,  546,  660, 1555, 1523,  741,  234,  173,  234,
  133,  234,  761,  528,  597,  782, 1280, 1285, 1285, 1286,
 1286,  159, 1099, 1287, 1287, 1076, 1284,  897,  160,  176,
 1519,  738, 1270,  215,  220, 1769, 1049,  516, 1568, 1509,
  186,  366,  162, 1528,  212, 1141,  652,  741,  673, 1559,
  536, 1285,  383, 1286,  670, 1288, 1288, 1287, 1289, 1289,
  385,  224,  872,  143,  728,  741,  480, 1657,  897,  727,
 1271,  897,  738,  662,   14,  738, 1709, 1611,  904, 1810,
  363, 1614,  904,  786,  702,  728,  897, 1290, 1291, 1288,
  738,  239, 1289, 1217,  728,  930,  704,  159,  149, 1672,
 1359, 1360, 1687, 1361, 1362, 1137,  704, 1303,  704, 1783,
  727,  383, 1833,  544,  689, 1026,   94,  704,  850, 1057,
  897,  177,  143, 1053, 1649,  702, 1055,  564,  727,  734,
  730,  707, 1750,  177, 1119,  731,  703, 1065,  162,  215,
  504,  160,   96,  702,  159, 1135,  705,  162,  502, 1136,
  734,  162,  751, 1806,   81,  705,  504,  703,  169,  496,
  177,  704, 1142,  804, 1083,  770,  177,   81,  177,  749,
  704, 1122, 1423,  689,  670,  659,  301,  792,  756, 1091,
 1138,  707,  761, 1816,  301,  659,  880,  659,  160,  851,
 1766,  824,  480,  289,  234,  728,  659, 1898, 1540,  694,
  875,  703,  707,  774,  852,  576, 1066,  177, 1826, 1125,
  703, 1336,  621,  704,  244,  670,  409, 1247,  670,  728,
 1010, 1346, 1668,  755, 1037,  817,  234,  704,  898, 1671,
 1558,  817,  755,  670, 1701,  794,  328,  753, 1248, 1111,
  659, 1518,  581,  765, 1710, 1662,  707,  576,  989,  659,
  856,  929, 1694,  703,  883,  884, 1857,  301, 1252,  660,
  734,   51,  728, 1532,  707,  385,  233,  703, 1091,  660,
 1700,  660,  384,  728, 1703, 1737,  705,   81,  739,  705,
  660, 1879,  770,  694, 1472,  546, 1273,  755, 1029,  770,
  247,  326,  659,   86,  704,  133,   52, 1139,  707, 1873,
  740,  234,  856,  289,  659, 1072,  659,  375,  734,  159,
 1472,  929,  707,  338,  761,  528,  383,  705, 1642,  739,
  929,  741,  621,  929,  660,  186,  943,  734,  258,   81,
  623,  384,  260,  660,  703,  705,  625,  739,  390,  143,
  143,  740,  741, 1472,  223, 1714,  738, 1850,  488,  338,
  488,  530,  997,  160,  530, 1472,  755, 1275, 1472,  740,
  301, 1576,  599,  130, 1800, 1557, 1472,  738,  930,  280,
  978,  623,  177,  659,  929,  144,  660,  625,  234,  707,
  376, 1294, 1366,  234,  727,  234, 1899,  234,  660,  623,
  660,  385,  145, 1302,  504,  625,  544,  390, 1047,  702,
 1305,  233,  162,  215, 1673,  727, 1787, 1367, 1674, 1675,
 1012,  215, 1788,  941,  393,  929,  751, 1324,  488, 1667,
  702,  599, 1074, 1338,  103,  162, 1241,  162,  930,  897,
  234,  391, 1531, 1008,  144,  143,  234,  930,  268, 1125,
  930, 1284,  383,  338, 1702, 1075, 1077,  244, 1421,  394,
 1088,  145,  741,  234, 1082,  399,  538,  660, 1035, 1108,
 1109, 1110,  734,  732,  702,  733, 1285,  735, 1286, 1666,
  658, 1096, 1287,  702,  964,  965,  282,  738, 1544,  338,
  234,  234, 1553,  309,  244,  244,  244, 1496,  604,  670,
  391,  930,  670,  534,  689, 1623,  673, 1605,  546,  670,
 1306,  670, 1089,  689, 1288,  670,  605, 1289, 1401,  145,
  670,  658,  186,  670,  399,  727,  702,  929,  622,  698,
 1851,  162,  379,  310,  689,  259,  898,  610,  670,  658,
  702,  162,  930,  130,  130,  186,  384,  387, 1755,  673,
 1134,  287,  670,  104,  738,  670,  386,  604, 1297, 1298,
  670,  338, 1112, 1776,  670,  283, 1307, 1777,  689,  622,
  698,  407, 1120,  670,  397,  605,  578,  898,  929,  412,
  898,  186, 1786,  146,  563, 1892,  389,  622,  698,  670,
 1206,  379,  727,  530, 1756,  898,  575, 1855, 1747, 1790,
  535,  705, 1218,  739,  186, 1757,  387,  702, 1347,  576,
 1624,  705,  390,  705,  388,  386,  670,  287, 1618,  544,
  223, 1029,  705,  966,  739,  740,  530,  842,  670,  898,
  670, 1367, 1367, 1473,  803,  578, 1476,  689, 1539, 1367,
 1367,  309, 1367, 1367,  930,  389,  740,  178,  504,  504,
 1367, 1367, 1367, 1367,  536, 1367, 1367, 1367, 1367, 1367,
 1367,  371,  144,  843,  215,  398,  705,  846,  530, 1561,
  288,  537,  384,  388,  259,  705,  623,  670,  371,  145,
  659,  940,  625, 1832, 1209, 1210, 1834, 1492, 1493,  642,
  643,  338, 1419, 1243, 1846,  930,  931,  670,  697,  574,
 1331,  670, 1308,  701, 1758,  391,  178,  730,  846,  291,
  599,  846,  731,  974,  974, 1349,  123, 1601,  705,  932,
 1871, 1353,  615,  530,  600,  244,  846, 1163, 1862,  399,
  546,  233,  705, 1856,  739,  496, 1820, 1315,  390,  697,
  244, 1164, 1823,  599,  701, 1153,  244,  701, 1316,  611,
  123,  988, 1602, 1891,  232, 1315,  740,  697,  574, 1484,
 1484, 1488,  701,  844, 1603, 1897,  234, 1880, 1900,  599,
  234,  105,  599, 1894,  770,  933, 1901,  598,  530,  736,
  186,  615,  825,  600,  123, 1165,  623,  599,  599,  226,
  227,  379,  186, 1154,  232,  658,  379, 1267, 1515,  705,
 1517,  379, 1327,  473, 1244,  658, 1881,  658, 1327,  106,
  295,  387,  608, 1166,  599,  770,  658, 1155,  770,  186,
  386,  107, 1477, 1211, 1895,  186,  247,  186, 1221, 1424,
 1475,  391, 1478,  770,  300,  583,  604,  488,  162,  255,
  233,  544,  186,  580,  698,  108, 1394,  328, 1268,  582,
  389, 1328,  974, 1882,  605,  828,  698, 1330,  801,  826,
  658, 1560,  473,  608,  622,  698,  186,  247,  718,  658,
  247,  232,  234,  256,  186, 1429, 1471, 1167,  388,  504,
  215,  220,  504,  251,  143,  247, 1512,  605,  569,  186,
  505, 1706,   14, 1474,  583,  604, 1513,  186,  604,  827,
  504,  504,  580,  718,  406,  178,  328,  215,  582,  698,
  109,  232,  658,  605,  604,  718,  605,  178,  698,  759,
  110,  296,  379, 1258,  658,  243,  658,  530,  465,  846,
  407,  186,  605,  186, 1309, 1853, 1259,  387,  898,  505,
  604,  403,  342,  186,  178,  111,  386,  569,  734,  732,
  178,  733,  178,  735,  546,  186,  154, 1566,  605, 1260,
  316,  698, 1571,  406,  357,  346,  243,  244,  530,  243,
 1310,  530,  465, 1542,  622,  698,  389,  168,  734,  732,
  186,  733,  846,  735,  243,  846,  530,  465,  530,  407,
  334,  178,  846,  658,  846, 1805,   42,  457,  846,  994,
  403, 1659,  379,  846,  388,  347,  846,  169,  244,  358,
 1262,  244,  379,  697,  734,  732,  343,  733,  701,  735,
  520,  846,  779,  345, 1622,  697,  244,   46, 1789,  525,
  701,  186,  741,  349,  697,  846,  168,  431,  766,  701,
  367,  321,  698,  846,  162,  482,  846,  846, 1679, 1841,
  770,  770, 1394, 1843,  186,   42,  846,  835,  770,  836,
  615,  473,  600,  557, 1849,  544,  169,  562,  135, 1680,
  374,  994,  846,  473,  241, 1681,  734,  732,  697,  733,
  209,  735,  207,  701, 1868,  309,   46,  697, 1632, 1134,
 1134,  741,  701,  615,  846,  600,  379,  766, 1032,  846,
 1471,  301,  247,  247,  482,  165,  473, 1258,  473, 1682,
  247,  846, 1886,  846, 1033,  241,  359, 1888,  241,  615,
 1259,  600,  615,  207,  600,  937,  207,  135, 1028,  374,
  697,  504,  504,  241,  360,  701,  166,   40,  615,  378,
  600,  207, 1754, 1260,  697,  528,  770,  332,  381,  701,
  770,  167,  400,  136,  215,  215,  178, 1261,  124,  309,
  846, 1768, 1810,  714,  615, 1770,  600,  770,  361,  721,
  722,  723, 1484,  770,  309,  309,  207, 1809,  402,  162,
  846,  404,  523,  730,  846,  530,  528,  220,  731,  528,
  529,  405,  734,  732, 1262,  733,   40,  735,  247,  945,
  530,  243,  243,  390,  528,  775,  528,  770, 1734,  243,
 1685,  697,  136,  730,  947,  949,  701,  124,  731,  247,
  409,  106,  730, 1205,  530,  247,  369,  731,  220,  370,
  530,  220,  775,  530,  661,  207,  744,  766, 1784,  743,
  408,  768, 1014,  244,  244, 1810,  220,  661,  493,  730,
  162,  244,  499,  212,  731,  661,  497,  108, 1394,  247,
 1870,  734,  732, 1501,  733, 1503,  735,  770, 1016,  382,
  661,  371, 1804,  215,  507,  530,  734,  732,  508,  733,
 1016,  735, 1771, 1016,  661, 1134, 1745, 1609,  767,  734,
  732, 1785,  733,  511,  735,  734,  732,  243,  733,  748,
  735,  283,  747,  383,  512,  384, 1263,  332, 1754, 1763,
  215,  730, 1754, 1792, 1754,  507,  731, 1794,  243,  247,
  546,  661,  109, 1754,  243,  814,  978,  385,  813,  513,
  530,  162,  110,  351,  514, 1827,  212,  530,  515,  244,
  428,  429,  430, 1754,  532,  352,  546,  153,  154, 1021,
 1812,  497,  386,   85,   86,  530,  507,  111,  243,  507,
  244, 1021, 1782,  560, 1021,  561,  244,  517,  438,  439,
 1872, 1754, 1007, 1864,  507, 1865, 1754,  530,  353,  546,
  354, 1084, 1085,  212, 1004,  530,  533,  994,  552,  530,
  972,  546,  497,  736,  546,  497,  226,  227,  554,  910,
  244, 1005,  546,  528, 1004, 1117,  209, 1814, 1116,  542,
  497,  971,  734,  732, 1815,  733,  556,  735,  243,  939,
  528,  528,  528,  944,  221, 1121,  528,  730,  894,  528,
  960,  544,  731,  661, 1220,  967,  968,  736,  938,  496,
  942,  568,  528,  569,  946,  948,  950,  457,  528,  979,
  980,  981,  982,  983,  984,  457, 1223,  544, 1374, 1222,
  244,  234, 1838,  232,  220,  221, 1471, 1272,  221,   14,
 1116, 1012,  734,  732,  234,  733, 1318,  735, 1323, 1317,
  572, 1322,  570,  221,  593,  212, 1491,  212,  603,  736,
  544,  525, 1471,  604, 1575, 1616,  730,  736,  736,  255,
 1658,  731,  544,  736, 1877,  544, 1878, 1876,  590,  747,
  608,  730, 1039,  544,  657, 1363,  731,  528,  557, 1046,
  131,  131,  661, 1014,  730, 1471, 1012, 1586, 1896,  731,
  730, 1810,  663,  256,  665,  731,  698, 1471,  528, 1134,
 1471,  700,  818,  369,  528,  701,  369,  702, 1471,  213,
 1011,  734,  732,  528,  733,  528,  735,  734,  732,  232,
  733,  703,  735,  990, 1014, 1014, 1014, 1014, 1014, 1014,
 1014,  720,  726,  528, 1802,  726, 1652, 1654,  528,  704,
  232, 1014, 1014, 1014, 1014, 1014,  232, 1653, 1655,  710,
  213,  507,  507,  213,  705,  711,  507,  734,  732,  507,
  733,  706,  735,  707,  708,  709,  712,  528,  213,  717,
  718,   14,  507,  719, 1014,  729, 1014, 1197,  734,  732,
  232,  733,  736,  735,  737,  738,  745,  497,  497,  742,
  746,  763, 1143, 1144, 1148,  497,  771, 1157, 1159, 1161,
 1258, 1169, 1172, 1175, 1177, 1180, 1183,  730,  497,  773,
  785,  794,  731, 1259,  112, 1190, 1191,  789, 1193,  790,
 1195,  796,  797, 1198, 1199, 1200, 1201, 1202, 1203,  800,
  806,  802,  113,  114,  563,  805, 1260,  808,  809,  839,
 1207,  815, 1212,  115,  116,  840,  117,  507,  841,  846,
 1261,  301, 1585,  118,  119,  317,  120,  121,  853,  122,
  232,  221,  123,  124,  125,  126,  127,  730,  507,  854,
  128,  129,  731,  112,  507,  860,  130,  131,  132,  861,
  133,  134,  135,  497,  457,  862,  863, 1262,  318,  866,
  813,  113,  114,  865,  871, 1399,  734,  732,  877,  733,
  878,  735,  115,  116,  497,  117,  734,  732,  507,  733,
  497,  735,  118,  119,  319,  120,  121,  320,  122,  881,
  882,  123,  124,  125,  126,  127,  891,  894,  899,  128,
  129,  813,  893,  321,  813,  130,  131,  132,  900,  133,
  134,  135,  901,  905,  497, 1014,  730, 1014, 1014,  813,
  175,  731,  730,  962, 1014,  970,  232,  731,  974,  322,
 1014,  872,  303,  998,  993, 1014,  999, 1002, 1014, 1000,
 1400,  734,  732, 1006,  733,  754,  735,  232, 1025,  769,
  530, 1040, 1014,  232,  747,  213,  213, 1014, 1014, 1030,
 1043, 1014,  730,  213, 1054, 1063, 1064,  731, 1014, 1263,
 1014, 1014,  734,  732, 1014,  733,  814,  735, 1014, 1066,
 1014, 1070, 1014,  730, 1014,  818, 1071,  232,  731, 1014,
 1014,  530, 1014, 1073,  530, 1087, 1094, 1079,  457, 1080,
 1081, 1105,  530, 1107, 1100, 1128, 1114, 1014, 1014,  530,
 1014, 1377, 1378, 1150, 1380, 1382, 1194,  814, 1188, 1187,
  814, 1014, 1387, 1014, 1196, 1014, 1014, 1014, 1204, 1014,
  730, 1226, 1014, 1014, 1224,  814, 1242, 1245, 1250, 1251,
 1255, 1257, 1014,  530, 1405, 1276,  530, 1279, 1265, 1301,
 1299,  213, 1412, 1413, 1212, 1014, 1311, 1212, 1014, 1207,
 1321,  530, 1319, 1420,  457, 1422, 1326, 1332, 1333, 1403,
  734,  732,  213,  733, 1014,  735, 1334, 1335,  213, 1343,
 1014, 1014, 1404,  734,  732, 1014,  733, 1351,  735, 1122,
 1356, 1357, 1358, 1364, 1365, 1369, 1014, 1014, 1192, 1014,
 1372,  730, 1373,  457, 1375, 1014,  731, 1376, 1383, 1384,
 1385,  730,  213, 1386, 1388, 1389,  731, 1390, 1391, 1392,
 1393,  929,  813, 1014, 1014, 1406,  734,  732, 1415,  733,
 1418,  735, 1479, 1481, 1014, 1014, 1482, 1014, 1494, 1495,
  769, 1499, 1524, 1504, 1014, 1014, 1014, 1014, 1014, 1505,
 1014, 1506, 1516, 1014, 1014, 1014, 1014, 1014, 1507,  994,
 1541, 1014, 1014,  585, 1529, 1533, 1014, 1014, 1014, 1014,
 1014, 1014, 1014, 1014, 1534,  813,  730, 1535,  813, 1536,
 1537,  731, 1538, 1091, 1546,  813, 1556,  813, 1562, 1564,
 1600,  813, 1565, 1570, 1577, 1599,  813, 1579, 1606,  813,
  994,  994,  994,  994,  994, 1007,  994,  730, 1620, 1612,
 1613, 1572,  731, 1626,  813, 1619, 1640, 1578,  994,  994,
  994,  994, 1582, 1625,  146, 1628,  530,  530,  813, 1587,
 1629,  530, 1638, 1636,  530, 1637,  813, 1639,  814,  813,
  813, 1645, 1407,  734,  732, 1646,  733,  530,  735,  813,
  994, 1647,  994, 1650, 1656, 1651, 1408,  734,  732, 1660,
  733, 1676,  735, 1212, 1691,  813, 1686, 1212, 1689, 1688,
 1690, 1409,  734,  732, 1692,  733, 1695,  735,  530,  530,
 1693, 1696, 1707, 1627, 1697, 1705,  530,  813, 1711, 1712,
 1713,  814,  813, 1716,  814, 1717, 1718, 1719,  540,  530,
 1722,  814, 1723,  814,  813,  730,  813,  814, 1724, 1725,
  731, 1726,  814, 1727, 1728,  814, 1729, 1730,  730, 1731,
 1733, 1742,  530,  731, 1736, 1749, 1641, 1410,  734,  732,
  814,  733, 1764,  735, 1765, 1775, 1778, 1797,  540,  540,
 1780, 1781,  540,  530,  814, 1801, 1807, 1793, 1808,  530,
  241, 1818,  814,  813, 1663,  814,  814,  540, 1811,  540,
 1819,  730, 1828, 1821, 1824,  814,  731, 1836, 1837, 1835,
 1839, 1842, 1858,  813,  530, 1860, 1863,  813, 1411,  734,
  732,  814,  733,  530,  735,  734,  732, 1215,  733,  540,
  735,  241, 1866, 1869,  241,  530, 1607,  734,  732, 1855,
  733,  530,  735,  814, 1810, 1875, 1856, 1885,  814,  241,
 1887, 1889, 1890, 1893, 1867,    1,  734,  193,   28,   30,
  814,  994,  814,  994,  994,  988,  770, 1715,   81,  261,
  994,  371,  454, 1720, 1721,  530,  994,  156,  713,  650,
  897,  994,  272,  462,  994, 1608,  734,  732,  973,  733,
  203,  735,  741,  271,  399,  694,  972, 1735,  994, 1018,
  994, 1212,  528,  994,  994, 1738,  696,  994,  183,  814,
 1744,  184, 1746,  714,  994,  953,  994,  994,  730,  372,
  994,  213,  323,  731,  994,  288,  994,  952,  994,  814,
  994,  723,  730,  814,  331,  994,  994,  731,  994, 1752,
 1018, 1018, 1018, 1018, 1018, 1018, 1018,  730,  358,  343,
  329,   27,  731,  994,  994,  297,  994,   30, 1018, 1018,
 1018, 1018,   40, 1483,  762,  298, 1009,  994,  506,  994,
  299,  994,  994,  994,  505,  994,  764, 1753,  994,  994,
  951,  952,  953,  954,  955,  956,  957,  958,  994, 1352,
 1018, 1001, 1018, 1127, 1791,  892,  540,  734,  732, 1216,
  733,  994,  735,  730,  994, 1350,  269, 1132,  731, 1090,
  254,  540, 1698,  540,  540,  540, 1699, 1340, 1348,  540,
  994,  574,  540,  855,  616, 1278,  994,  994,  550, 1329,
 1041,  994, 1341,  457,  587,  540,  588, 1068, 1740, 1779,
 1854,  540,  994,  994,  540,  994, 1615,  734,  732, 1678,
  733,  994,  735,   55,  730, 1684, 1861, 1859, 1884,  731,
  730,  834, 1061, 1663,   56,  731,  241,  241, 1803,  994,
  994,   57,  730, 1631,  241, 1228, 1847,  731, 1761, 1669,
  994,  994, 1511,  994,  580,  990,  540,  849,  595,  838,
  994,  994,  994,  994,  994,   58,  994,  571, 1106,  994,
  994,  994,  994,  994, 1554, 1752, 1848,  994,  994, 1752,
  540, 1844,  994,  994,  994,  994,  994,  994,  994,  994,
 1752,  730,  859, 1799,  734,  732,  731,  733,  879,  735,
  152,  540, 1097,  734,  732, 1617,  733,  540,  735,  780,
 1752,  540,  543, 1753,  784,  314, 1031, 1753,  540, 1845,
 1027, 1254, 1502,  264, 1014, 1825, 1732,   59, 1753, 1045,
  934, 1018,  241, 1018, 1018, 1007,  540, 1598, 1752, 1379,
 1018,  540,  793, 1752, 1249, 1520, 1018, 1417, 1753,    0,
    0, 1018,    0,  241, 1018,    0,    0,    0,   60,  241,
    0,    0,   61,    0,    0,    0,  540,    0, 1018, 1023,
  540,    0,    0, 1018, 1018,    0, 1753, 1018,    0,    0,
    0, 1753,    0,    0, 1018,    0, 1018, 1018,    0,    0,
 1018,    0,    0,  241, 1018,    0, 1018,    0, 1018,    0,
 1018,    0,  730,    0,    0, 1018, 1018,  731, 1018,    0,
 1023, 1023, 1023, 1023, 1023, 1023, 1023,    0,    0,    0,
    0,    0,    0, 1018, 1018,    0, 1018,    0, 1023, 1023,
 1023, 1023,    0,    0,    0,    0,  112, 1018,    0, 1018,
    0, 1018, 1018, 1018,    0, 1018,    0,    0, 1018, 1018,
    0,   62,  730,    0,  113,  114,    0,  731, 1018,    0,
 1023,    0, 1023,    0,    0,  115,  116,    0,  117,    0,
    0, 1018,    0,    0, 1018,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123,  124,  125,  126,  127,    0,
 1018,    0,  128,  129,    0,    0, 1018, 1018,  130,  131,
  132, 1018,  133,  134,  135,    0,    0,    0,    0,    0,
    0,    0, 1018, 1018,    0, 1018,    0,    0, 1158,    0,
 1162, 1018, 1170, 1173,    0, 1178, 1181, 1184,    0,  730,
    0,    0,    0,    0,  731,    0,    0,    0,  730, 1018,
 1018,    0,    0,  731,    0,    0,    0,    0,    0,    0,
 1018, 1018,    0, 1018,    0,    0,    0,    0,    0,    0,
 1018, 1018, 1018, 1018, 1018,    0, 1018,    0,    0, 1018,
 1018, 1018, 1018, 1018,    0,    0,    0, 1018, 1018,    0,
    0,    0, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018,
 1573, 1574,    0,    0,    0,    0,    0,    0, 1580, 1581,
    0, 1583, 1584,    0,    0,    0,    0,    0,    0, 1588,
 1589, 1590, 1591,    0, 1592, 1593, 1594, 1595, 1596, 1597,
    0,    0,    0,    0,    0,    0,  987,    0,    0,    0,
    0, 1023,    0, 1023, 1023,    0,    0,    0,    0,    0,
 1023,    0,    0,    0,    0,    0, 1023,    0,    0,    0,
    0, 1023,    0,    0, 1023,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1023, 1019,
    0,    0,    0, 1023, 1023,    0,    0, 1023,    0,    0,
    0,    0,    0,    0, 1023,    0, 1023, 1023,    0,    0,
 1023,    0,    0,    0, 1023,    0, 1023,    0, 1023,    0,
 1023,    0,    0,    0,    0, 1023, 1023,   66, 1023,    0,
 1019, 1019, 1019, 1019, 1019, 1019, 1019,    0,   67,    0,
    0,    0,    0, 1023, 1023,   68, 1023,    0, 1019, 1019,
 1019, 1019,    0,    0,    0,    0,  112, 1023,   69, 1023,
    0, 1023, 1023, 1023,    0, 1023,    0,    0, 1023, 1023,
    0,    0,    0,    0,  113,  114,    0,    0, 1023,    0,
 1019,    0, 1019,    0,    0,  115,  116,    0,  117,    0,
    0, 1023,    0,    0, 1023,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123,  124,  125,  126,  127,    0,
 1023,    0,  128,  129,    0,   70, 1023, 1023,  130,  131,
  132, 1023,  133,  134,  135,    0,    0,    0,    0,    0,
    0,   71, 1023, 1023,    0, 1023,    0,   89,    0,    0,
    0, 1023,    0,    0,   90,    0,    0,   72,   91,   92,
    0,    0,    0,    0,    0,    0,    0,   93,    0, 1023,
 1023,    0,   73,    0,    0,    0,   74,    0,   94,   75,
 1023, 1023,    0, 1023,    0,    0,    0,   76,    0,    0,
 1023, 1023, 1023, 1023, 1023,    0, 1023,    0,    0, 1023,
 1023, 1023, 1023, 1023,    0,    0,   77, 1023, 1023,    0,
    0,    0, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   95,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   96,    0,  654,    0,
    0, 1019,    0, 1019, 1019,   78,    0,    0,    0,    0,
 1019,   97,    0,    0,    0,   98, 1019,    0,    0,    0,
    0, 1019,    0,    0, 1019,    0,   99,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1019, 1032,
    0,    0,    0, 1019, 1019,  100,    0, 1019,    0,    0,
    0,    0,    0,    0, 1019,    0, 1019, 1019,    0,    0,
 1019,    0,    0,    0, 1019,    0, 1019,    0, 1019,    0,
 1019,    0,    0,    0,    0, 1019, 1019,    0, 1019,    0,
 1032, 1032, 1032, 1032, 1032, 1032, 1032,    0,    0,    0,
    0,    0,    0, 1019, 1019,    0, 1019,    0, 1032, 1032,
 1032, 1032,    0,    0,  101,    0,  112, 1019,    0, 1019,
    0, 1019, 1019, 1019,    0, 1019,    0,    0, 1019, 1019,
    0,    0,    0,    0,  113,  114,    0,    0, 1019,    0,
 1032,    0, 1032,    0,    0,  115,  116,    0,  117,    0,
    0, 1019,    0,    0, 1019,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123,  124,  125,  126,  127,    0,
 1019,    0,  128,  129,    0,    0, 1019, 1019,  130,  131,
  132, 1019,  133,  134,  135,    0,    0,    0,    0,    0,
    0,    0, 1019, 1019,    0, 1019,    0,    0,    0,    0,
    0, 1019,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1019,
 1019,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1019, 1019,    0, 1019,    0,    0,    0,    0,    0,    0,
 1019, 1019, 1019, 1019, 1019,    0, 1019,    0,    0, 1019,
 1019, 1019, 1019, 1019,    0,    0,    0, 1019, 1019,    0,
    0,    0, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  654,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   82,
    0, 1032,    0, 1032, 1032,    0,    0,    0,    0,    0,
 1032,    0,    0,    0,    0,    0, 1032,    0,    0,    0,
    0, 1032,    0,    0, 1032,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1032, 1037,
    0,    0,    0, 1032, 1032,    0,    0, 1032,    0,    0,
    0,    0,    0,    0, 1032,    0, 1032, 1032,    0,    0,
 1032,    0,    0,    0, 1032,    0, 1032,    0, 1032,    0,
 1032,    0,    0,    0,    0, 1032, 1032,    0, 1032,    0,
 1037, 1037, 1037, 1037, 1037, 1037, 1037,    0,    0,    0,
    0,    0,    0, 1032, 1032,    0, 1032,    0, 1037, 1037,
 1037, 1037,    0,    0,    0,    0,  657, 1032,    0, 1032,
    0, 1032, 1032, 1032,    0, 1032,    0,    0, 1032, 1032,
    0,    0,    0,    0,  657,  657,    0,    0, 1032,    0,
 1037,    0, 1037,    0,    0,  657,  657,    0,  657,    0,
    0, 1032,    0,    0, 1032,  657,  657,    0,  657,  657,
    0,  657,    0,    0,  657,  657,  657,  657,  657,    0,
 1032,    0,  657,  657,    0,    0, 1032, 1032,  657,  657,
  657, 1032,  657,  657,  657,    0,    0,    0,    0,    0,
    0,    0, 1032, 1032,    0, 1032,    0,    0,    0,    0,
    0, 1032,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1032,
 1032,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1032, 1032,    0, 1032,    0,    0,    0,    0,    0,    0,
 1032, 1032, 1032, 1032, 1032,    0, 1032,    0,    0, 1032,
 1032, 1032, 1032, 1032,    0,    0,    0, 1032, 1032,    0,
    0,    0, 1032, 1032, 1032, 1032, 1032, 1032, 1032, 1032,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1037,    0, 1037, 1037,    0,    0,    0,    0,    0,
 1037,    0,  454,    0,    0,    0, 1037,    0,    0,    0,
    0, 1037,    0,    0, 1037,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1037,  865,
    0,    0,    0, 1037, 1037,    0,    0, 1037,    0,    0,
    0,    0,    0,    0, 1037,    0, 1037, 1037,    0,    0,
 1037,    0,    0,    0, 1037,    0, 1037,    0, 1037,    0,
 1037,    0,    0,    0,    0, 1037, 1037,    0, 1037,    0,
  865,  865,  865,  865,  865,    0,  865,    0,    0,    0,
    0,    0,    0, 1037, 1037,    0, 1037,    0,  865,  865,
  865,  865,    0,    0,   81,    0,  112, 1037,    0, 1037,
    0, 1037, 1037, 1037,    0, 1037,    0,    0, 1037, 1037,
    0,    0,    0,    0,  113,  114,    0,    0, 1037,    0,
    0,    0,  865,    0,    0,  115,  116,    0,  117,    0,
    0, 1037,    0,    0, 1037,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123,  124,  125,  126,  127,    0,
 1037,    0,  128,  129,    0,    0, 1037, 1037,  130,  131,
  132, 1037,  133,  134,  135,    0,    0,    0,    0,    0,
    0,    0, 1037, 1037,    0, 1037,    0,    0,    0,    0,
    0, 1037,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1037,
 1037,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1037, 1037,    0, 1037,    0,    0,    0,    0,    0,    0,
 1037, 1037, 1037, 1037, 1037,    0, 1037,    0,    0, 1037,
 1037, 1037, 1037, 1037,    0,    0,    0, 1037, 1037,    0,
    0,    0, 1037, 1037, 1037, 1037, 1037, 1037, 1037, 1037,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  865,    0,  865,  865,    0,    0,    0,    0,    0,
  865,    0,    0,    0,    0,    0,  865,    0,    0,    0,
    0,  865,    0,    0,  865,    0,    0,    0,    0,    0,
  525,    0,    0,    0,    0,    0,    0,    0,  865,  878,
    0,    0,    0,  865,  865,    0,    0,  865,    0,    0,
    0,    0,    0,    0,  865,    0,  865,  865,    0,    0,
  865,    0,    0,    0,  865,    0,  865,    0,  865,    0,
  865,  525,    0,    0,  525,  865,  865,    0,  865,    0,
  878,  878,  878,  878,  878,    0,  878,    0,    0,  525,
    0,  525,    0,  865,  865,    0,  865,    0,  878,  878,
  878,  878,    0,    0,    0,    0,  112,  865,    0,  865,
    0,  865,  865,  865,    0,  865,    0,    0,  865,  865,
    0,    0,    0,    0,  113,  114,    0,    0,  865,    0,
    0,    0,  878,    0,    0,  115,  116,    0,  117,    0,
    0,  865,    0,    0,  865,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123,  124,  125,  126,  127,    0,
  865,    0,  128,  129,    0,    0,  865,  865,  130,  131,
  132,  865,  133,  134,  135,    0,    0,    0,    0,    0,
    0,    0,  865,  865,    0,  865,    0,    0,    0,    0,
    0,  865,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  865,
  865,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  865,  865,    0,  865,    0,    0,    0,    0,    0,    0,
  865,  865,  865,  865,  865,    0,  865,    0,    0,  865,
  865,  865,  865,  865,    0,    0,    0,  865,  865,    0,
    0,    0,  865,  865,  865,  865,  865,  865,  865,  865,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  525,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  878,    0,  878,  878,  525,  525,  525,    0,    0,
  878,  525,    0,    0,  525,    0,    0,    0,    0,    0,
    0,  878,    0,    0,  878,    0,    0,  525,    0,    0,
  530,    0,    0,  525,    0,    0,    0,    0,  878,  881,
    0,    0,    0,  878,  878,    0,    0,  878,    0,    0,
    0,    0,    0,    0,  878,    0,  878,  878,    0,    0,
  878,    0,    0,    0,  878,    0,  878,    0,  878,    0,
  878,  530,    0,    0,  530,  878,  878,    0,  878,    0,
  881,  881,  881,  881,  881,    0,  881,    0,    0,  530,
    0,  530,    0,  878,  878,    0,  878,    0,  881,  881,
  881,  881,  525,    0,    0,    0,    0,  878,    0,  878,
    0,  878,  878,  878,    0,  878,    0,    0,  878,  878,
    0,    0,    0,  525,    0,    0,    0,    0,  878,  525,
    0,    0,  881,    0,    0,    0,    0,    0,    0,    0,
  525,  878,    0,    0,  878,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  525,    0,
  878,    0,    0,  525,    0,    0,  878,  878,    0,    0,
    0,  878,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  878,  878,    0,  878,    0,    0,    0,    0,
    0,  878,  525,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  878,
  878,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  878,  878,  541,  878,    0,    0,    0,    0,    0,    0,
  878,  878,  878,  878,  878,    0,  878,    0,    0,  878,
  878,  878,  878,  878,    0,    0,    0,  878,  878,    0,
    0,    0,  878,  878,  878,  878,  878,  878,  878,  878,
    0,    0,  541,  541,    0,    0,  541,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  541,    0,  541,    0,    0,    0,    0,  530,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  881,    0,  881,  881,    0,  530,  530,    0,    0,
  881,  530,    0,  541,  530,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  881,    0,    0,  530,    0,    0,
    0,    0,    0,  530,    0,    0,    0,    0,  881,  882,
    0,    0,    0,  881,  881,    0,    0,  881,    0,    0,
    0,    0,    0,    0,  881,    0,  881,  881,    0,    0,
  881,    0,    0,    0,  881,    0,  881,    0,  881,    0,
  881, 1281,    0,    0,    0,  881,  881,    0,  881,    0,
  882,  882,  882,  882,  882,    0,  882,    0,    0,    0,
    0,    0,    0,  881,  881,    0,  881,    0,  882,  882,
  882,  882,  530,    0,    0,    0,    0,  881,    0,  881,
    0,  881,  881,  881,    0,  881,    0,    0,  881,  881,
    0,    0,    0,  530,    0,    0,    0,    0,  881,  530,
    0,    0,  882,    0,    0,    0,    0,    0,    0,    0,
  530,  881,    0,    0,  881,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  530,    0,
  881,    0,    0,  530,    0,    0,  881,  881,    0,    0,
    0,  881,    0,    0,    0,    0,    0,    0,    0,    0,
  541,    0,  881,  881,    0,  881,    0,    0,    0,    0,
    0,  881,  530,    0,    0,  541,    0,  541,  541,  541,
    0,    0,    0,  541,    0,    0,  541,    0,    0,  881,
  881,    0,    0,    0,    0,    0,    0,    0,    0,  541,
  881,  881,    0,  881,    0,  541,    0,    0,  541,    0,
  881,  881,  881,  881,  881,    0,  881,    0,    0,  881,
  881,  881,  881,  881,    0,    0,    0,  881,  881,    0,
    0,    0,  881,  881,  881,  881,  881,  881,  881,  881,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  541,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  541,    0,    0,    0,    0,    0,
    0,  882, 1621,  882,  882,    0,    0,    0,    0,    0,
  882,    0,    0,    0,    0,  541,    0,    0,  416,    0,
    0,  541,    0,    0,  882,  541,    0,    0,  876,    0,
    0,    0,  541,    0,    0,    0,    0,    0,  882,    0,
    0,    0,    0,  882,  882,    0,  418,  882,    0,    0,
  541,    0,    0,    0,  882,  541,  882,  882,    0,    0,
  882,    0,    0,    0,  882,    0,  882,    0,  882,  876,
  882,  876,  876,  876,  420,  882,  882,    0,  882,    0,
  541,    0,    0,    0,  541,    0,    0,  876,  876,  876,
  876,    0,    0,  882,  882,    0,  882,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  882,    0,  882,
    0,  882,  882,  882,    0,  882,    0,    0,  882,  882,
    0,  876,    0,    0,    0,    0,    0,    0,  882,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  882,    0,    0,  882,  425,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  882,  427,    0,    0,    0,    0,  882,  882,    0,    0,
    0,  882,    0,    0,    0,  428,  429,  430,    0,  226,
  227,    0,  882,  882,    0,  882,    0,    0,    0,    0,
    0,  882,    0,    0,    0,    0,    0,    0,  433,  434,
  435,  436,  437,  438,  439,  440,  441,    0,    0,  882,
  882,    0,    0,  443,  444,    0,    0,    0,    0,    0,
  882,  882,  448,  882,    0,    0,    0,    0,    0,    0,
  882,  882,  882,  882,  882,    0,  882,    0,    0,  882,
  882,  882,  882,  882,    0,    1,    0,  882,  882,    0,
    0,    0,  882,  882,  882,  882,  882,  882,  882,  882,
    0,    0,    0,    2,    0,    0,    0,    0,    0,    0,
    0,    3,    0,    0,    0,    0,    0,    0,    4,    0,
    5,    0,    6,    0,    0,    0,    0,    7,    0,    0,
  876, 1630,  876,  876,    8,    0,    0,    0,    0,  876,
    0,    0,    0,    0,    0,    0,    0,    0,    9,    0,
    0,    0,    0,  876,    0,    0,    0,  875,   10,    0,
    0,    0,    0,    0,    0,    0,    0,  876,    0,    0,
    0,    0,  876,  876,    0,    0,  876,    0,    0,    0,
    0,    0,    0,  876,    0,  876,  876,    0,    0,  876,
    0,    0,    0,  876,    0,  876,    0,  876,  875,  876,
  875,  875,  875,    0,  876,  876,    0,  876,    0,    0,
    0,    0,    0,    0,    0,    0,  875,  875,  875,  875,
    0,   11,  876,  876,    0,  876,    0,    0,   12,    0,
    0,   13,    0,   14,   15,  112,  876,    0,  876,    0,
  876,  876,  876,    0,  876,    0,    0,  876,  876,    0,
  875,    0,    0,  113,  114,    0,    0,  876,   16,    0,
    0,    0,    0,    0,  115,  116,    0,  117,    0,    0,
  876,    0,    0,  876,  118,  119,    0,  120,  121,    0,
  122,    0,    0,  123,  124,  125,  126,  127,    0,  876,
    0,  128,  129,    0,    0,  876,  876,  130,  131,  132,
  876,  133,  134,  135,    0,    0,    0,   17,    0,    0,
    0,  876,  876,    0,  876,    0,    0,    0,    0,    0,
  876,    0,   18,   19,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  617,    0,    0,  618,  876,  876,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  876,
  876,    0,  876,    0,  619,    0,    0,  620,    0,  876,
  876,  876,  876,  876,    0,  876,    0,    0,  876,  876,
  876,  876,  876,    0,    0,    0,  876,  876,    0,    0,
    0,  876,  876,  876,  876,  876,  876,  876,  876,    0,
    0,    0,    0,    0,    0,    0,    0,  621,    0,    0,
    0,    0,    0,  622,  623,  624,    0,  625,  626,  627,
    0,  628,    0,    0,    0,    0,    0,    0,    0,  875,
    0,  875,  875,    0,    0,    0,    0,    0,  875,  629,
    0,  630,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  875,    0,    0,    0,  877,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  875,    0,    0,    0,
  631,  875,  875,    0,    0,  875,    0,    0,    0,    0,
    0,    0,  875,    0,  875,  875,    0,  632,  875,    0,
    0,    0,  875,    0,  875,    0,  875,  877,  875,  877,
  877,  877,    0,  875,  875,    0,  875,    0,    0,  633,
    0,    0,    0,    0,    0,  877,  877,  877,  877,    0,
    0,  875,  875,    0,  875,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  112,  875,    0,  875,    0,  875,
  875,  875,    0,  875,  634,  635,  875,  875,    0,  877,
    0,    0,  113,  114,    0,    0,  875,    0,    0,    0,
    0,  636,    0,  115,  116,    0,  117,    0,    0,  875,
    0,    0,  875,  118,  119,    0,  120,  121,    0,  122,
    0,    0,  123,  124,  125,  126,  127,    0,  875,    0,
  128,  129,    0,    0,  875,  875,  130,  131,  132,  875,
  133,  134,  135,    0,    0,    0,    0,    0,    0,    0,
  875,  875,    0,  875,    0,    0,    0,    0,    0,  875,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  875,  875,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  875,  875,
    0,  875,    0,    0,    0,    0,    0,    0,  875,  875,
  875,  875,  875,  840,  875,    0,    0,  875,  875,  875,
  875,  875,    0,    0,    0,  875,  875,    0,    0,    0,
  875,  875,  875,  875,  875,  875,  875,  875,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  840,    0,    0,  840,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  877,    0,
  877,  877,  840,    0,    0,    0,    0,  877,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  877,    0,    0,    0,  880,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  877,    0,    0,    0,    0,
  877,  877,    0,    0,  877,    0,    0,    0,    0,    0,
    0,  877,    0,  877,  877,    0,    0,  877,    0,    0,
    0,  877,    0,  877,    0,  877,  880,  877,  880,  880,
  880,    0,  877,  877,    0,  877,    0,    0,    0,    0,
    0,    0,    0,    0,  880,  880,  880,  880,    0,    0,
  877,  877,    0,  877,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  877,    0,  877,    0,  877,  877,
  877,    0,  877,    0,    0,  877,  877,    0,  880,    0,
    0,    0,    0,    0,    0,  877,    0,    0,    0,    0,
    0,    0,    0,  971,  734,  732,    0,  733,  877,  735,
    0,  877,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  927,  926,  928,    0,    0,  877,    0,    0,
    0,    0,    0,  877,  877,    0,    0,    0,  877,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  877,
  877,    0,  877,    0,    0,    0,    0,    0,  877,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  840,  877,  877,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  877,  877,    0,
  877,    0,    0,    0,    0,    0,    0,  877,  877,  877,
  877,  877,    0,  877,  686,    0,  877,  877,  877,  877,
  877,    0,    0,    0,  877,  877,    0,    0,    0,  877,
  877,  877,  877,  877,  877,  877,  877,    0,  840,    0,
    0,  840,    0,    0,    0,    0,    0,    0,  840,    0,
  840,    0,    0,    0,  840,  686,  788,    0,  686,  840,
    0,    0,  840,    0,    0,    0,    0,  880,    0,  880,
  880,    0,    0,  686,    0,    0,  880,  840,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  880,  840,    0,    0,    0,    0,    0,  788,    0,  840,
  788,    0,  840,  840,  880,    0,    0,    0,    0,  880,
  880,    0,  840,  880,    0,  788,    0,    0,    0,    0,
  880,    0,  880,  880,    0,    0,  880,    0,  840,    0,
  880,    0,  880,    0,  880,    0,  880,    0,    0,    0,
    0,  880,  880,  914,  880,    0,    0,    0,    0,  730,
  840,    0,    0,    0,  731,  840,    0,  915,    0,  880,
  880,    0,  880,    0,    0,    0,    0,  840,    0,  840,
    0,    0,    0,  880,    0,  880,    0,  880,  880,  880,
    0,  880,  987,    0,  880,  880,    0,    0,    0,    0,
    0,    0,    0,  916,  880,    0,    0,    0,    0,    0,
    0,  917,    0,    0,    0,    0,    0,  880,    0,  918,
  880,    0,    0,    0,    0,    0,  840,    0,    0,    0,
  657,    0,    0,    0,    0,    0,  880,  919,    0,  920,
    0,    0,  880,  880,    0,    0,  840,  880,    0,    0,
  840,    0,  921,    0,  922,  923,  924,    0,  880,  880,
    0,  880,    0,    0,    0,    0,    0,  880,    0,    0,
  657,  657,    0,    0,  657,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  880,  880,    0,  686,  657,
    0,    0,    0,    0,    0,    0,  880,  880,    0,  880,
    0,    0,    0,  925,    0,    0,  880,  880,  880,  880,
  880,    0,  880,    0,    0,  880,  880,  880,  880,  880,
    0,    0,    0,  880,  880,    0,    0,    0,  880,  880,
  880,  880,  880,  880,  880,  880,    0,    0,    0,  686,
    0,    0,  686,    0,    0,    0,    0,    0,    0,  686,
    0,  686,    0,    0,    0,  686,    0,    0,    0,    0,
  686,    0,    0,  686,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  686,    0,
    0,  788,    0,    0,  788,    0,    0,    0,    0,    0,
    0,  788,  686,  788,    0,    0,    0,  788,    0,    0,
  686,    0,  788,    0,  686,  788,    0,    0,    0,    0,
    0,    0,    0,  686,    0,    0,    0,    0,    0,    0,
  788,    0,    0,    0,    0,    0,    0,    0,    0,  686,
    0,    0,    0,    0,  788,    0,    0,  684,    0,    0,
    0,    0,  788,  986,    0,  788,  788,    0,    0,    0,
    0,    0,    0,    0,    0,  788,  686,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  686,    0,
  686,  788,    0,    0,    0,    0,  686,    0,  684,    0,
    0,  684,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  788,  686,  686,  684,    0,  788,    0,
    0,    0,    0,    0,    0,  686,  686,    0,  686,    0,
  788,    0,  788,    0,    0,  686,  686,  686,  686,  686,
    0,  686,    0,    0,  686,  686,  686,  686,  686,    0,
    0,    0,  686,  686,    0,    0,    0,  686,  686,  686,
  686,  686,  686,  686,  686,  657,    0,    0,  657,    0,
    0,    0,    0,    0,    0,  657,    0,  657,    0,  788,
    0,  657,    0,    0,    0,    0,  657,    0,    0,  657,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  788,
    0,    0,    0,  788,  657,  734,  732,    0,  733,    0,
  735,    0,    0,    0,    0,    0,    0,    0,  657,    0,
    0,    0,  112,  927,  926,  928,  657,    0,    0,    0,
  657,    0,    0,    0,    0,    0,    0,    0,    0,  657,
  113,  114,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,  116,    0,  117,  657,    0,    0,    0,    0,
    0,  118,  119,    0,  120,  121,    0,  122,    0,    0,
  123,  124,  125,  126,  127,    0,    0,    0,  128,  129,
    0,    0,  657,    0,  130,  131,  132,    0,  133,  134,
  135,    0,    0,    0,  657,    0,  657,    0,    0,    0,
    0,    0,  657,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  657,  657,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  657,  657,    0,  657,    0,    0,    0,    0,    0,
    0,  657,  657,  657,  657,  657,    0,  657,    0,    0,
  657,  657,  657,  657,  657,    0,  856,    0,  657,  657,
    0,    0,    0,  657,  657,  657,  657,  657,  657,  657,
  657,    0,  684,    0,    0,  684,    0,    0,    0,    0,
    0,    0,  684,    0,  684,    0,    0,    0,  684,    0,
    0,    0,    0,  684,    0,    0,  684,  856, 1026, 1026,
  856, 1026, 1026, 1026,    0,    0,    0,    0,    0,    0,
    0,  684,    0,    0,    0,  856, 1026, 1026, 1026,    0,
    0,    0,    0,    0,    0,  684,    0,    0,    0,    0,
    0,    0,    0,  684,  914,  857,    0,  684,    0,    0,
  730,    0,    0,    0,    0,  731,  684, 1026,  915,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  684,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  857, 1027, 1027,  857,
 1027, 1027, 1027,    0,  916,    0,    0,    0,    0,  684,
    0,    0,  917,    0,  857, 1027, 1027, 1027,    0,    0,
  918,  684,    0,  684,    0,    0,    0,    0,    0,  657,
    0,    0,    0,    0,    0,    0,    0,    0,  919,    0,
  920,    0,    0,    0,    0,    0, 1027,  657,  657,    0,
    0,    0,    0,  921,    0,  922,  923,  924,  657,  657,
    0,  657,    0,    0,    0,    0,    0,    0,  657,  657,
  684,  657,  657,    0,  657,    0,    0,  657,  657,  657,
  657,  657,    0,    0,    0,  657,  657,    0,    0,    0,
  684,  657,  657,  657,  684,  657,  657,  657,    0,    0,
    0,  817,    0,    0,  925,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  856,    0,
    0,    0,  817,    0,    0,  817,    0, 1026,    0,    0,
    0,    0,    0, 1026,    0,    0,    0,    0, 1026,    0,
  817, 1026,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  856,    0,    0,  856,  668,    0, 1026,    0,    0,
    0,  856,    0,  856,    0, 1026,    0,  856,    0,    0,
    0,    0,  856, 1026,    0,  856,    0,  857,    0,    0,
    0,    0,    0,    0,    0,    0, 1027,    0,    0,    0,
  856, 1026, 1027, 1026,    0,  668,  668, 1027,    0,  668,
 1027,    0,    0,    0,  856,    0, 1026,    0, 1026, 1026,
 1026,    0,  856,    0,  668,  856,  856,    0,    0,    0,
    0,    0,    0,    0,    0,  856,    0,    0,    0,    0,
  857,    0,    0,  857,    0,    0, 1027,    0,    0,    0,
  857,  856,  857,    0, 1027,    0,  857,    0,    0,    0,
    0,  857, 1027,    0,  857,  858,    0, 1026,    0,    0,
    0,    0,    0,  856,    0,    0,    0,    0,  856,  857,
 1027,    0, 1027,    0,    0,    0,    0,    0,    0,    0,
  856,    0,  856,  857,    0, 1027,    0, 1027, 1027, 1027,
    0,  857,    0,    0,  857,  857,  858, 1028, 1028,  858,
 1028, 1028, 1028,    0,  857,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  858, 1028, 1028, 1028,    0,    0,
  857,    0,    0,  817,    0,    0,    0,    0,    0,  856,
    0,    0,    0,    0,  815,    0, 1027,    0,    0,    0,
    0,    0,  857,    0,    0,    0, 1028,  857,    0,  856,
    0,    0,    0,  856,    0,    0,    0,    0,    0,  857,
    0,  857,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  815,  817,    0,  815,  817,
    0,    0,    0,    0,    0,    0,  817,    0,  817,    0,
    0,    0,  817,  815,    0,    0,    0,  817,    0,    0,
  817,    0,    0,    0,    0,    0,    0,    0,  857,    0,
    0,    0,    0,  816,    0,  817,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  857,  817,
    0,    0,  857,    0,    0,    0,    0,  817,    0,    0,
  817,  817,    0,    0,    0,    0,    0,    0,    0,    0,
  817,    0,    0,    0,  816,    0,    0,  816,    0,    0,
    0,    0,    0,    0,    0,    0,  817,    0,    0,    0,
  668,    0,  816,  668,    0,    0,    0,    0,    0,    0,
  668,    0,  668,    0,    0,    0,  668,    0,  817,    0,
    0,  668,  848,  817,  668,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  817,    0,  817,    0,  668,
    0,    0,    0,    0,    0,    0,    0,  858,    0,    0,
    0,    0,    0,  668,    0,    0, 1028,    0,    0,    0,
    0,  668, 1028,  848,    0,  668,  848, 1028,    0,    0,
 1028,    0,    0,    0,  668,    0,    0,    0,    0,    0,
    0,  848,    0,    0,  817,    0,    0,    0,    0,    0,
  668,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  858,    0,    0,  858,  817,    0, 1028,    0,  817,    0,
  858,    0,  858,    0, 1028,    0,  858,  668,    0,    0,
    0,  858, 1028,    0,  858,    0,  815,    0,    0,  668,
    0,  668,    0,    0,    0,    0,    0,    0,    0,  858,
 1028,    0, 1028,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  810,  858,    0, 1028,    0, 1028, 1028, 1028,
    0,  858,    0,    0,  858,  858,    0,    0,    0,    0,
    0,    0,    0,    0,  858,    0,    0,    0,  668,  815,
  811,    0,  815,    0,    0,    0,    0,    0,    0,  815,
  858,  815,    0,  810,    0,  815,  810,    0,  668,    0,
  815,    0,  668,  815,    0,  816, 1028,    0,    0,    0,
    0,  810,  858,    0,    0,    0,    0,  858,  815,    0,
    0,  811,    0,    0,  811,    0,    0,    0,    0,  858,
    0,  858,  815,    0,    0,    0,    0,    0,    0,  811,
  815,    0,    0,  815,  815,    0,    0,    0,    0,    0,
    0,    0,    0,  815,    0,    0,    0,    0,  816,    0,
    0,  816,    0,    0,    0,    0,    0,    0,  816,  815,
  816,    0,    0,    0,  816,    0,    0,    0,  858,  816,
    0,    0,  816,    0,  848,    0,    0,    0,    0,    0,
    0,  815,    0,    0,    0,    0,  815,  816,  858,    0,
  812,    0,  858,    0,    0,    0,    0,    0,  815,    0,
  815,  816,    0,    0,    0,    0,    0,    0,    0,  816,
    0,    0,  816,  816,    0,    0,    0,    0,    0,    0,
    0,    0,  816,    0,    0,    0,    0,  848,    0,    0,
  848,  812,    0,    0,  812,    0,    0,  848,  816,  848,
    0,    0,    0,  848,    0,    0,    0,  815,  848,  812,
    0,  848,    0,    0,    0,    0,    0,    0,    0,    0,
  816,    0,    0,    0,    0,  816,  848,  815,    0,    0,
    0,  815,    0,    0,  847,    0,    0,  816,    0,  816,
  848,    0,    0,    0,    0,    0,    0,    0,  848,    0,
    0,  848,  848,    0,    0,    0,    0,    0,    0,    0,
    0,  848,    0,    0,  810,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  847,    0,  848,  847,    0,
  841,    0,    0,    0,    0,    0,  816,    0,    0,    0,
    0,    0,  811,  847,    0,    0,    0,    0,    0,  848,
    0,    0,    0,    0,  848,    0,  816,    0,    0,    0,
  816,    0,    0,    0,    0,    0,  848,  810,  848,    0,
  810,  841,    0,    0,  841,    0,    0,  810,    0,  810,
    0,    0,    0,  810,    0,    0,    0,    0,  810,  841,
    0,  810,    0,    0,    0,  811,    0,    0,  811,    0,
    0,    0,    0,    0,    0,  811,  810,  811,    0,    0,
    0,  811,    0,    0,  849,  848,  811,    0,    0,  811,
  810,    0,    0,    0,    0,    0,    0,    0,  810,    0,
    0,  810,  810,    0,  811,  848,    0,    0,    0,  848,
    0,  810,    0,    0,    0,    0,    0,    0,  811,    0,
    0,    0,    0,    0,    0,  849,  811,  810,  849,  811,
  811,    0,  812,    0,    0,    0,    0,    0,    0,  811,
    0,    0,    0,  849,    0,    0,    0,    0,    0,  810,
    0,    0,    0,    0,  810,  811,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  810,  850,  810,    0,
    0,    0,    0,    0,    0,    0,    0,  811,    0,    0,
    0,    0,  811,    0,    0,  812,    0,    0,  812,    0,
    0,    0,    0,    0,  811,  812,  811,  812,    0,    0,
    0,  812,    0,    0,    0,    0,  812,    0,  850,  812,
    0,  850,    0,    0,    0,  810,  847,    0,    0,    0,
    0,    0,    0,    0,  812,    0,  850,    0,    0,    0,
    0,    0,    0,    0,    0,  810,    0,    0,  812,  810,
    0,    0,  838,  811,    0,    0,  812,    0,    0,  812,
  812,    0,    0,    0,    0,    0,    0,    0,    0,  812,
    0,    0,  841,  811,    0,    0,    0,  811,    0,  847,
    0,    0,  847,    0,    0,  812,    0,    0,    0,  847,
    0,  847,    0,  838,    0,  847,  838,    0,    0,    0,
  847,    0,    0,  847,    0,    0,    0,  812,    0,    0,
    0,  838,  812,    0,    0,    0,    0,    0,  847,    0,
    0,    0,    0,    0,  812,  841,  812,    0,  841,    0,
    0,    0,  847,    0,    0,  841,    0,  841,    0,    0,
  847,  841,    0,  847,  847,    0,  841,    0,    0,  841,
    0,    0,    0,  847,    0,    0,  849,    0,    0,    0,
    0,    0,    0,    0,  841,    0,    0,    0,    0,  847,
    0,    0,    0,  812,    0,    0,    0,    0,  841,    0,
    0,    0,  836,    0,    0,    0,  841,    0,    0,  841,
  841,  847,    0,  812,    0,    0,  847,  812,    0,  841,
    0,    0,    0,    0,    0,    0,    0,    0,  847,  849,
  847,    0,  849,    0,    0,  841,    0,    0,    0,  849,
    0,  849,    0,  836,    0,  849,  836,    0,    0,    0,
  849,    0,    0,  849,    0,    0,    0,  841,    0,  850,
    0,  836,  841,    0,    0,    0,    0,    0,  849,    0,
    0,    0,    0,    0,  841,    0,  841,  847,    0,    0,
    0,    0,  849,    0,    0,  842,    0,    0,    0,    0,
  849,    0,    0,  849,  849,    0,    0,  847,    0,    0,
    0,  847,    0,  849,    0,    0,    0,    0,    0,    0,
    0,    0,  850,  851,    0,  850,    0,    0,    0,  849,
    0,    0,  850,  841,  850,    0,  842,    0,  850,  842,
    0,    0,    0,  850,  838,    0,  850,    0,    0,    0,
    0,  849,    0,  841,  842,    0,  849,  841,    0,    0,
    0,  850,    0,    0,  851,    0,    0,  851,  849,    0,
  849,    0,    0,    0,    0,  850,    0,    0,    0,    0,
    0,    0,  851,  850,    0,    0,  850,  850,    0,    0,
    0,    0,    0,    0,    0,    0,  850,  838,    0,    0,
  838,    0,    0,    0,    0,    0,    0,  838,    0,  838,
    0,    0,  850,  838,    0,    0,    0,  849,  838,    0,
    0,  838,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  850,    0,  838,  849,    0,  850,
    0,  849,    0,    0,    0,  837,    0,    0,    0,    0,
  838,  850,    0,  850,    0,    0,    0,    0,  838,    0,
    0,  838,  838,    0,    0,    0,    0,    0,    0,    0,
    0,  838,    0,    0,  836,  839,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  837,  838,    0,  837,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  850,    0,    0,    0,  837,    0,    0,    0,    0,  838,
    0,    0,    0,    0,  838,    0,  839,  843,    0,  839,
  850,    0,    0,    0,  850,    0,  838,  836,  838,    0,
  836,    0,    0,    0,  839,    0,    0,  836,    0,  836,
    0,    0,    0,  836,    0,    0,    0,    0,  836,    0,
    0,  836,    0,    0,    0,    0,    0,  842,  843,    0,
    0,  843,    0,    0,    0,    0,  836,    0,    0,    0,
    0,    0,    0,    0,    0,  838,  843,    0,    0,    0,
  836,    0,    0,    0,    0,  851,    0,    0,  836,    0,
    0,  836,  836,    0,    0,  838,    0,    0,    0,  838,
    0,  836,    0,    0,    0,    0,    0,    0,    0,    0,
  842,    0,    0,  842,    0,    0,    0,  836,    0,    0,
  842,    0,  842,    0,    0,    0,  842,    0,  681,    0,
    0,  842,    0,    0,  842,    0,    0,    0,  851,  836,
    0,  851,    0,    0,  836,    0,    0,    0,  851,  842,
  851,    0,    0,    0,  851,    0,  836,    0,  836,  851,
    0,    0,  851,  842,    0,    0,    0,    0,    0,  681,
    0,  842,  681,    0,  842,  842,    0,  851,    0,    0,
    0,    0,    0,    0,  842,    0,    0,  681,    0,    0,
    0,  851,    0,    0,    0,    0,    0,    0,    0,  851,
  842,    0,  851,  851,    0,  836,    0,  837,    0,    0,
    0,    0,  851,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  842,    0,    0,  836,    0,  842,  851,  836,
    0,    0,    0,    0,    0,    0,    0,  839,    0,  842,
    0,  842,    0,    0,    0,    0,    0,    0,    0,    0,
  851,    0,    0,    0,    0,  851,    0,    0,    0,    0,
  837,    0,    0,  837,    0,    0,    0,  851,    0,  851,
  837,    0,  837,    0,    0,    0,  837,    0,    0,  843,
    0,  837,    0,    0,  837,    0,  681,    0,  842,  451,
  839,  452,    0,  839,    0,    0,    0,    0,    0,  837,
  839,    0,  839,    0,  454,    0,  839,    0,  842,  303,
    0,  839,  842,  837,  839,    0,  851,    0,    0,    0,
    0,  837,    0,    0,  837,  837,    0,    0,    0,  839,
    0,    0,  843,    0,  837,  843,  851,    0,    0,    0,
  851,    0,  843,  839,  843,    0,    0,    0,  843,    0,
  837,  839,    0,  843,  839,  839,  843,    0,    0,    0,
    0,    0,    0,    0,  839,    0,    0,    0,    0,    0,
    0,  843,  837,    0,    0,    0,    0,  837,    0,    0,
  839,    0,    0,    0,    0,  843,    0,    0,    0,  837,
    0,  837,    0,  843,    0,    0,  843,  843,    0,    0,
    0,    0,  839,    0,    0,    0,  843,  839,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  839,
    0,  839,  843,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  837,    0,
    0,    0,    0,  681,  843,    0,  681,    0,    0,  843,
    0,    0,    0,  681,    0,  681,    0,    0,  837,  681,
    0,  843,  837,  843,  681,    0,    0,  681,  839,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  681,    0,    0,    0,    0,    0,  839,    0,
    0,    0,  839,    0,    0,    0,  681,    0,    0,    0,
    0,    0,    0,    0,  681,    0,    0,    0,  681,    0,
  843,    0,    0,  413,    0,    0,    0,  681,    0,    0,
  414,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  843,    0,  415,  681,  843,    0,    0,    0,  416,  417,
  681,    0,    0,  451,    0,  452,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  675,  454,    0,
  681,    0,    0,  303,    0,    0,  418,    0,    0,  419,
    0,    0,  681,    0,  681,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  420,    0,  421,  422,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,  585,    0,
  424,    0,    0,    0,    0,    0,    0,    0,  906,    0,
    0,  681,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  681,    0,    0,    0,  681,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   14,    0,    0,    0,  677,
    0,    0,    0,    0,    0,  425,    0,    0,  591,    0,
    0,  426,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  427,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  428,  429,  430,  431,  226,
  227,    0,    0,    0,    0,    0,    0,    0,    0,  432,
    0,    0,    0,    0,    0,    0,  113,  114,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  115,  442,    0,
  117,    0,    0,  443,  444,    0,  445,  446,  447,    0,
  120,  121,  448,  122,    0,    0,  123,  124,  125,  126,
  127,  907,  908,  909,  128,  129,    0,    0,    0,    0,
  130,  449,  132,    0,  450,  134,  135,  413,    0,    0,
    0,    0,    0,    0,  414,    0,  112,    0,  681,    0,
    0,  451,    0,  452,    0,    0,  415,    0,    0,  674,
    0,    0,  416,  417,  113,  114,  454,    0,    0,    0,
    0,  303,    0,    0,    0,  115,  116,    0,  117,    0,
    0,  675,    0,    0,    0,  118,  119,    0,  120,  121,
  418,  122,    0,  419,  123,  124,  125,  126,  127,    0,
    0,    0,  128,  129,    0,    0,  112,    0,  130,  131,
  132,    0,  133,  134,  135,    0,    0,    0,  420,    0,
  421,  422,    0,    0,  113,  114,    0,    0,    0,    0,
  423,    0,    0,    0,  424,  115,  116,    0,  117,    0,
    0,    0,  676,    0,    0,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123,  124,  125,  126,  127,    0,
    0,    0,  128,  129,    0,    0,    0,    0,  130,  131,
  132,    0,  133,  134,  135,    0,    0,    0,    0,    0,
    0,    0,    0,  677,    0,    0,    0,    0,    0,  425,
    0,    0,    0,    0,    0,  426,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  427,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  428,
  429,  430,  431,  226,  227,    0,    0,    0,    0,    0,
    0,    0,    0,  432,    0,    0,    0,    0,    0,    0,
  113,  114,  433,  434,  435,  436,  437,  438,  439,  440,
  441,  115,  442,    0,  117,    0,    0,  443,  444,    0,
  445,  446,  447,    0,  120,  121,  448,  122,    0,    0,
  123,  124,  678,  679,  680,  413,    0,    0,  128,  129,
    0,    0,  414,    0,  130,  449,  132,    0,  450,  134,
  135,    0,    0,  535,  415,    0,    0,    0,    0,    0,
  416,  417,  681,    0,    0,  451,    0,  452,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  675,
  454,    0,    0,    0,    0,  303,    0,    0,  418,  500,
    0,  419,    0,    0,  535,    0,    0,  535,    0,    0,
    0,    0,    0,    0,    0,    0,  112,    0,    0,    0,
    0,    0,  535,    0,  535,    0,  420,    0,  421,  422,
    0,    0,    0,    0,  113,  114,    0,    0,  423,    0,
    0,    0,  424,    0,    0,  115,  116,    0,  117,    0,
  902,    0,    0,    0,  535,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123,  124,  125,  126,  127,    0,
    0,    0,  128,  129,    0,    0,    0,    0,  130,  131,
  132,    0,  133,  134,  135,    0,    0,    0,    0,    0,
    0,  677,    0,    0,    0,    0,    0,  425,    0,    0,
    0,    0,    0,  426,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  427,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  428,  429,  430,
  431,  226,  227,    0,    0,    0,    0,    0,    0,    0,
    0,  432,    0,    0,    0,    0,    0,    0,  113,  114,
  433,  434,  435,  436,  437,  438,  439,  440,  441,  115,
  442,    0,  117,    0,    0,  443,  444,    0,  445,  446,
  447,    0,  120,  121,  448,  122,    0,    0,  123,  124,
  678,  679,  680,    0,    0,    0,  128,  129,    0,    0,
    0,    0,  130,  449,  132,    0,  450,  134,  135,  413,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  681,    0,    0,  451,    0,  452,    0,    0,  415,    0,
    0,  535,    0,    0,  416,  417,    0,    0,  454,    0,
    0,    0,    0,  303,    0,    0,  535,    0,  535,  535,
  535,    0,    0,  675,  535,    0,    0,  535,    0,    0,
    0,    0,  418,    0,    0,  419,    0,    0,  112,    0,
  535,    0,    0,    0,    0,    0,  535,    0,    0,  535,
    0,    0,    0,    0,    0,    0,  113,  114,    0,    0,
  420,    0,  421,  422,    0,    0,    0,  115,  116,    0,
  117,    0,  423,    0,    0,    0,  424,  118,  119,    0,
  120,  121,    0,  122,  676,    0,  123,  124,  125,  126,
  127,  535,    0,    0,  128,  129,    0,    0,    0,    0,
  130,  131,  132,    0,  133,  134,  135,    0,    0,    0,
    0,    0,    0,    0,    0,  535,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  677,    0,    0,    0,    0,
    0,  425,    0,    0,    0,    0,  535,  426,    0,    0,
    0,    0,  535,    0,    0,    0,  535,  427,    0,    0,
    0,    0,    0,  535,    0,    0,    0,    0,    0,    0,
    0,  428,  429,  430,  431,  226,  227,    0,    0,    0,
    0,  535,    0,    0,  931,  432,  535,    0,    0,    0,
    0,    0,  113,  114,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  115,  442,    0,  117,  932,    0,  443,
  444,  535,  445,  446,  447,  535,  120,  121,  448,  122,
    0,    0,  123,  124,  678,  679,  680,  413,    0,    0,
  128,  129,    0,    0,  414,    0,  130,  449,  132,    0,
  450,  134,  135,    0,    0,    0,  415,    0,    0,    0,
    0,    0,  416,  417,    0,    0,    0,    0,    0,    0,
    0,  519,    0,  933,  451,    0,  452,    0,    0,    0,
    0,  675,    0,    0,    0,    0,    0,    0,    0,  454,
  418,    0,    0,  419,  303,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  112,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  420,    0,
  421,  422,    0,    0,    0,  113,  114,    0,    0,    0,
  423,    0,    0,    0,  424,    0,  115,  116,    0,  117,
    0,    0,  902,    0,    0,    0,  118,  119,    0,  120,
  121,    0,  122,    0,    0,  123,  124,  125,  126,  127,
    0,    0,    0,  128,  129,  105,    0,    0,    0,  130,
  131,  132,    0,  133,  134,  135,    0,    0,    0,    0,
    0,    0,    0,  677,    0,    0,    0,    0,    0,  425,
    0,    0,    0,    0,    0,  426,    0,    0,    0,    0,
    0,    0,    0,  106,    0,  427,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  107,    0,    0,    0,  428,
  429,  430,  431,  226,  227,    0,    0,    0,    0,    0,
    0,    0,    0,  432,    0,    0,    0,    0,    0,  108,
  113,  114,  433,  434,  435,  436,  437,  438,  439,  440,
  441,  115,  442,    0,  117,    0,    0,  443,  444,    0,
  445,  446,  447,    0,  120,  121,  448,  122,    0,    0,
  123,  124,  125,  126,  127,    0,    0,    0,  128,  129,
    0,    0,    0,    0,  130,  449,  132,    0,  450,  134,
  135, 1145,    0,    0, 1146,    0,    0,    0,  413,    0,
    0,    0,    0,    0,  109,  414,    0,    0,    0,    0,
    0,    0,    0,    0,  110,    0,    0,  415,    0,    0,
    0,    0,    0,  416,  417,    0,    0,    0,    0,    0,
    0,    0,  519,    0,    0,  451,    0,  452,    0,  111,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  454,  418,    0,    0,  419,  303,    0,  112,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  113,  114,    0,    0,  420,
    0,  421,  422,    0,    0,    0,  115,  116,    0,  117,
    0,  423,    0,    0,    0,  424,  118,  119,    0,  120,
  121,    0,  122,    0,    0,  123,  124,  125,  126,  127,
    0,    0,    0,  128,  129,    0,    0,    0,    0,  130,
  131,  132,    0,  133,  134,  135,   48,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1147,    0,
  425,    0,    0,    0,    0,    0,  426,    0,    0,    0,
    0,    0,    0,    0,   48,    0,  427,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   48,    0,    0,    0,
  428,  429,  430,  431,  226,  227,    0,    0,    0,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,    0,
   48,  113,  114,  433,  434,  435,  436,  437,  438,  439,
  440,  441,  115,  442,    0,  117,    0,    0,  443,  444,
    0,  445,  446,  447,    0,  120,  121,  448,  122,    0,
    0,  123,  124,  125,  126,  127,    0,    0,    0,  128,
  129,    0,    0,    0,    0,  130,  449,  132,    0,  450,
  134,  135, 1156,    0,    0, 1146,    0,    0,    0,  413,
    0,    0,    0,    0,    0,   48,  414,    0,    0,    0,
    0,    0,    0,    0,    0,   48,    0,    0,  415,    0,
    0,    0,    0,    0,  416,  417,    0,    0,    0,    0,
    0,    0,    0,  519,    0,    0,  451,    0,  452,    0,
   48,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  454,  418,    0,    0,  419,  303,    0,   50,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   50,   50,    0,    0,
  420,    0,  421,  422,    0,    0,    0,   50,   50,    0,
   50,    0,  423,    0,    0,    0,  424,   50,   50,    0,
   50,   50,    0,   50,    0,    0,   50,   50,   50,   50,
   50,    0,    0,    0,   50,   50,    0,    0,    0,    0,
   50,   50,   50,    0,   50,   50,   50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1147,
    0,  425,    0,    0,    0,    0,    0,  426,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  427,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  428,  429,  430,  431,  226,  227,    0,    0,    0,
    0,    0,    0,    0,    0,  432,    0,    0,    0,    0,
    0,    0,  113,  114,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  115,  442,    0,  117,    0,    0,  443,
  444,    0,  445,  446,  447,    0,  120,  121,  448,  122,
    0,    0,  123,  124,  125,  126,  127,    0,    0,    0,
  128,  129,    0,    0,    0,    0,  130,  449,  132,    0,
  450,  134,  135, 1160,    0,    0, 1146,    0,    0,    0,
  413,  211,    0,    0,  211,    0,    0,  414,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  211,    0,  415,
    0,  211,    0,    0,  211,  416,  417,    0,    0,    0,
    0,    0,    0,    0,  519,    0,    0,  451,    0,  452,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  454,  418,    0,    0,  419,  303,    0,    0,
    0,    0,    0,    0,  211,    0,    0,    0,    0,    0,
  211,  211,  211,   27,  211,  211,  211,    0,  211,    0,
    0,  420,    0,  421,  422,    0,    0,    0,    0,    0,
    0,   27,   27,  423,    0,    0,  211,  424,  211,    0,
    0,    0,   27,   27,    0,   27,    0,    0,    0,    0,
    0,    0,   27,   27,    0,   27,   27,    0,   27,    0,
    0,   27,   27,   27,   27,   27,    0,  211,    0,   27,
   27,    0,   80,    0,    0,   27,   27,   27,    0,   27,
   27,   27,    0,    0,  211,    0,    0,    0,    0,    0,
 1147,    0,  425,    0,    0,    0,    0,    0,  426,    0,
    0,    0,    0,    0,    0,    0,  211,    0,  427,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  428,  429,  430,  431,  226,  227,    0,    0,
    0,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,  211,  211,  113,  114,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  115,  442,    0,  117,  211,    0,
  443,  444,    0,  445,  446,  447,    0,  120,  121,  448,
  122,    0,    0,  123,  124,  125,  126,  127,    0,    0,
    0,  128,  129,    0,    0,    0,    0,  130,  449,  132,
    0,  450,  134,  135, 1168,    0,    0, 1146,    0,    0,
    0,  413,  895,    0,    0,  618,    0,    0,  414,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  896,    0,
  415,    0,  619,    0,    0,  620,  416,  417,    0,    0,
    0,    0,    0,    0,    0,  519,    0,    0,  451,    0,
  452,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  454,  418,    0,    0,  419,  303,    0,
    0,    0,    0,    0,    0,  621,    0,    0,    0,    0,
    0,  622,  623,  624,  572,  625,  626,  627,    0,  628,
    0,    0,  420,    0,  421,  422,    0,    0,    0,    0,
    0,    0,  572,  572,  423,    0,    0,  629,  424,  630,
    0,    0,    0,  572,  572,    0,  572,    0,    0,    0,
    0,    0,    0,  572,  572,    0,  572,  572,    0,  572,
    0,    0,  572,  572,  572,  572,  572,    0,  631,    0,
  572,  572,    0,    0,    0,    0,  572,  572,  572,    0,
  572,  572,  572,    0,    0,  632,    0,    0,    0,    0,
    0, 1147,    0,  425,    0,    0,    0,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,  633,    0,  427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  428,  429,  430,  431,  226,  227,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,  634,  635,  113,  114,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  115,  442,    0,  117,  636,
    0,  443,  444,    0,  445,  446,  447,    0,  120,  121,
  448,  122,    0,    0,  123,  124,  125,  126,  127,    0,
    0,    0,  128,  129,    0,    0,    0,    0,  130,  449,
  132,    0,  450,  134,  135, 1171,    0,    0, 1146,    0,
    0,    0,  413,  211,    0,    0,  211,    0,    0,  414,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  211,
    0,  415,    0,  211,    0,    0,  211,  416,  417,    0,
    0,    0,    0,    0,    0,    0,  519,    0,    0,  451,
    0,  452,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  454,  418,    0,    0,  419,  303,
    0,    0,    0,    0,    0,    0,  211,    0,    0,    0,
    0,    0,  211,  211,  211,  280,  211,  211,  211,    0,
  211,    0,    0,  420,    0,  421,  422,    0,    0,    0,
    0,    0,    0,  280,  280,  423,    0,    0,  211,  424,
  211,    0,    0,    0,  280,  280,    0,  280,    0,    0,
    0,    0,    0,    0,  280,  280,    0,  280,  280,    0,
  280,    0,    0,  280,  280,  280,  280,  280,    0,  211,
    0,  280,  280,    0,    0,    0,    0,  280,  280,  280,
    0,  280,  280,  280,    0,    0,  211,    0,    0,    0,
    0,    0, 1147,    0,  425,    0,    0,    0,    0,    0,
  426,    0,    0,    0,    0,    0,    0,    0,  211,    0,
  427,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  428,  429,  430,  431,  226,  227,
    0,    0,    0,    0,    0,    0,    0,    0,  432,    0,
    0,    0,    0,  211,  211,  113,  114,  433,  434,  435,
  436,  437,  438,  439,  440,  441,  115,  442,    0,  117,
  211,    0,  443,  444,    0,  445,  446,  447,    0,  120,
  121,  448,  122,    0,    0,  123,  124,  125,  126,  127,
    0,    0,    0,  128,  129,    0,    0,    0,    0,  130,
  449,  132,    0,  450,  134,  135, 1176,    0,    0, 1146,
    0,    0,    0,  413,  141,    0,    0,  141,    0,    0,
  414,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  415,    0,  141,    0,    0,  141,  416,  417,
    0,    0,    0,    0,    0,    0,    0,  519,    0,    0,
  451,    0,  452,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  454,  418,    0,    0,  419,
  303,    0,    0,    0,    0,    0,    0,  141,    0,    0,
    0,    0,    0,  141,  141,  141,    0,  141,  141,  141,
    0,  141,    0,    0,  420,    0,  421,  422,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,  141,
  424,  141,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  141,    0,    0,
    0,    0,    0, 1147,    0,  425,    0,    0,    0,    0,
    0,  426,    0,    0,    0,    0,    0,    0,    0,  141,
    0,  427,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  428,  429,  430,  431,  226,
  227,    0,    0,    0,    0,    0,    0,    0,    0,  432,
    0,    0,    0,    0,  141,  141,  113,  114,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  115,  442,    0,
  117,  141,    0,  443,  444,    0,  445,  446,  447,    0,
  120,  121,  448,  122,    0,    0,  123,  124,  125,  126,
  127,    0,    0,    0,  128,  129,    0,    0,    0,    0,
  130,  449,  132,    0,  450,  134,  135, 1179,    0,    0,
 1146,    0,    0,    0,  413,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  415,    0,  557,    0,    0,    0,  416,
  417,    0,    0,    0,    0,    0,    0,    0,  519,    0,
    0,  451,    0,  452,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  454,  418,    0,    0,
  419,  303,    0,    0,    0,    0,  557,    0,    0,  557,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  557,  420,  557,  421,  422,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,  424,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  557,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1147,    0,  425,    0,    0,    0,
    0,    0,  426,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  427,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  428,  429,  430,  431,
  226,  227,    0,    0,    0,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,    0,    0,  113,  114,  433,
  434,  435,  436,  437,  438,  439,  440,  441,  115,  442,
    0,  117,    0,    0,  443,  444,    0,  445,  446,  447,
    0,  120,  121,  448,  122,    0,    0,  123,  124,  125,
  126,  127,    0,    0,    0,  128,  129,    0,    0,    0,
    0,  130,  449,  132,    0,  450,  134,  135, 1182,    0,
    0, 1146,    0,    0,    0,  413,    0,    0,    0,    0,
    0,    0,  414,    0,    0,    0,  641,    0,  641,  641,
    0,  641,    0,  557,  415,    0,    0,    0,    0,    0,
  416,  417,    0,    0,  641,    0,    0,    0,  557,  641,
    0,  557,  557,    0,    0,    0,  557,    0,    0,  557,
    0,    0,    0,    0,    0,    0,    0,    0,  418,    0,
    0,  419,  557,    0,    0,    0,    0,    0,  557,    0,
    0,  557,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,    0,  421,  422,
    0,    0,    0,    0,    0,    0,    0,    0,  423,    0,
    0,    0,  424,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  557,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  557,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1147,    0,  425,  557,    0,
    0,    0,    0,  426,  557,    0,    0,    0,  557,    0,
    0,    0,    0,  427,    0,  557,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  428,  429,  430,
  431,  226,  227,  557,    0,    0, 1229,    0,  557,    0,
    0,  432,    0,    0,    0,    0,    0,    0,  113,  114,
  433,  434,  435,  436,  437,  438,  439,  440,  441,  115,
  442,    0,  117,  557,    0,  443,  444,  557,  445,  446,
  447,    0,  120,  121,  448,  122,  641,    0,  123,  124,
  125,  126,  127,  641,    0,    0,  128,  129,    0,    0,
  641,    0,  130,  449,  132,    0,  450,  134,  135,    0,
    0,    0,  641,    0,    0,    0, 1230,    0,  641,  641,
    0,    0,    0,    0,    0,    0,  641,  635,    0,  635,
  635,    0,  635,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  635,  641,    0,    0,  641,
  635, 1231,    0,    0,    0,    0, 1232,    0,    0,    0,
    0,    0, 1233,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  112,  641,    0,  641,  641,    0,    0,
    0, 1234,    0,    0,    0,    0,  641,    0,    0,    0,
  641,  113,  114,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115,  116,    0,  117,    0,    0,    0,    0,
    0,    0,  118,  119,    0,  120,  121,    0,  122,    0,
    0,  123,  124,  125,  126,  127,    0,    0,    0,  128,
  129,    0,    0,    0,    0,  130,  131,  132,    0,  133,
  134,  135,    0,    0,    0,  641,    0,    0,    0,    0,
    0,  641,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  641,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  641,  641,  641,  641,  641,
  641,    0,    0,    0,    0,    0,    0,    0,    0,  641,
    0,    0,    0,    0,    0,    0,  641,  641,  641,  641,
  641,  641,  641,  641,  641,  641,  641,  641,  641,    0,
  641,    0,    0,  641,  641,    0,  641,  641,  641,    0,
  641,  641,  641,  641,    0,    0,  641,  641,  641,  641,
  641,    0,    0,    0,  641,  641,    0,    0,    0,    0,
  641,  641,  641,    0,  641,  641,  641,  635,    0,    0,
    0,    0,  547,    0,  635,    0,    0,    0,    0,    0,
    0,  635,    0,    0,    0,  519,    0,  524,  451,    0,
  452,    0,    0,  635,    0,    0,    0,    0,    0,  635,
  635,    0,    0,  454,    0,    0,    0,  635,  303,    0,
    0,    0,    0,  547,    0,    0,  547,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  635,    0,    0,
  635,  547,    0,  547,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  635,    0,  635,  635,    0,
    0,    0,    0,  547,    0,    0,    0,  635,    0,    0,
    0,  635,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1226,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  635,    0,    0,    0,
    0,    0,  635,    0,    0,    6,    0,    0,    0,    0,
    0,    0,  635,    0,    0,    0, 1430, 1431,    0, 1432,
    0,    0,    0,    0,    0,    0,  635,  635,  635,  635,
  635,  635,    0,    0,    0, 1433,    0,    0,    0,    0,
  635, 1434,    0,    0,    0,    0,    0,  635,  635,  635,
  635,  635,  635,  635,  635,  635,  635,  635,  635,  635,
    0,  635,    0,    0,  635,  635,    0,  635,  635,  635,
    0,  635,  635,  635,  635,    0,    0,  635,  635,  635,
  635,  635,  413,    0,    0,  635,  635,    0,    0,  414,
    0,  635,  635,  635,    0,  635,  635,  635, 1435,    0,
  547,  415,    0,    0,    0,    0,    0,  416,  417,  519,
    0,    0,  451,    0,  452,  547,   14,    0,  547,  547,
    0,    0,    0,  547,    0,    0,  547,  454,    0,    0,
    0, 1436,  303,    0,    0,  418,    0,    0,  419,  547,
    0,   16,    0,    0,    0,  547,    0,    0,  547,    0,
 1641,    0,    0,    0,    0,    0,    0,    0,    0,  431,
    0,    0,    0,  420,    0,  421,  422,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,  113,  114,  424,
    0,    0,    0,    0,    0,    0,    0,    0, 1437,  116,
  547,  117,    0,    0,    0,    0,    0,    0,  118,  119,
    0,  120,  121,    0,  122,    0,    0,  123, 1438,  125,
  126,  127,    0,    0,  547,  128,  129, 1439, 1440, 1441,
    0,  130,  131,  132,    0,  133,  134,  135,    0,    0,
    0,    0,    0,    0,  425,  547,    0,    0,    0,    0,
  426,  547,    0,    0,    0,  547,    0,    0,    0,    0,
  427,    0,  547,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  428,  429,  430,  431,  226,  227,
  547,    0,    0,    0,    0,  547,    0,    0,  432,    0,
    0,    0,    0,    0,    0,  113,  114,  433,  434,  435,
  436,  437,  438,  439,  440,  441,  115,  442,    0,  117,
  547,    0,  443,  444,  547,  445,  446,  447,    0,  120,
  121,  448,  122,    0,    0,  123,  124,  125,  126,  127,
    0,    0,    0,  128,  129,    0,    0,    0,    0,  130,
  449,  132,    0,  450,  134,  135,  413,    0,    0,    0,
    0,    0,    0,  414,    0,    0,    0,  519,    0,    0,
  451,    0,  452,    0,    0,  415,    0,    0,    0,    0,
    0,  416,  417,    0,    0,  454,    0,    0,    0,    0,
  303,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
  994,    0,  419,  994,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  996,    0,
  994,    0,    0,  994,    0,    0,    0,  420,    0,  421,
  422,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,  424,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  994,    0,    0,    0,    0,    0,  994,
  994,  994,    0,  994,  994,  994,    0,  994,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  994,    0,  994,  425,    0,
    0,    0,    0,    0,  426,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  427,    0,    0,    0,    0,    0,
    0,    0,  713,    0,    0,    0,  994,    0,  428,  429,
  430,  431,  226,  227,    0,    0,    0,    0,    0,    0,
    0,    0,  432,  994,    0,    0,    0,    0,    0,  113,
  114,  433,  434,  435,  436,  437,  438,  439,  440,  441,
  115,  442,    0,  117,    0,  994,  443,  444,    0,  445,
  446,  447,    0,  120,  121,  448,  122,    0,    0,  123,
  124,  125,  126,  127,  413,    0,    0,  128,  129,    0,
    0,  414,    0,  130,  449,  132,    0,  450,  134,  135,
  994,  994,  532,  415,    0,    0,    0,    0,    0,  416,
  417,  519,  975,    0,  451,    0,  452,  994,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  454,
    0,    0,    0,    0,  303,    0,    0,  418,    0,    0,
  419,    0,    0,  532,    0,    0,  532,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  532,    0,  532,    0,  420,    0,  421,  422,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,  424,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  532,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   14,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  425,    0,    0,    0,
    0,    0,  426,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  427,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  428,  429,  430,  431,
  226,  227,    0,    0,    0,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,    0,    0,  113,  114,  433,
  434,  435,  436,  437,  438,  439,  440,  441,  115,  442,
    0,  117,    0,    0,  443,  444,    0,  445,  446,  447,
    0,  120,  121,  448,  122,    0,    0,  123,  124,  125,
  126,  127,    0,    0,    0,  128,  129,    0,    0,    0,
    0,  130,  449,  132,    0,  450,  134,  135,  413,    0,
    0,    0,    0,    0,    0,  414,    0,    0,    0,  519,
    0,    0,  451,    0,  452,    0,    0,  415,    0,    0,
  532,    0,    0,  416,  417,    0,    0,  454,    0,    0,
    0,    0,  303,    0,    0,  532,    0,    0,  532,  532,
    0,    0,    0,  532,    0,    0,  532,    0,    0,    0,
    0,  418,    0,    0,  419,    0,    0,    0,    0,  532,
    0,    0,    0,    0,    0,  532,    0,    0,  532,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  420,
    0,  421,  422,    0,    0,    0,    0,    0,    0,    0,
    0,  423,    0,    0,    0,  424,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  532,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  532,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  425,    0,    0,    0,    0,  532,  426,    0,    0,    0,
    0,  532,    0,    0,    0,  532,  427,    0,    0,    0,
    0,    0,  532,    0,    0,    0,    0,    0,    0,    0,
  428,  429,  430,  431,  226,  227,    0,    0,    0,    0,
  532,    0,    0,    0,  432,  532,    0,    0,    0,    0,
    0,  113,  114,  433,  434,  435,  436,  437,  438,  439,
  440,  441,  115,  442,    0,  117,    0,    0,  443,  444,
  532,  445,  446,  447,  532,  120,  121,  448,  122,    0,
    0,  123,  124,  125,  126,  127,  413,    0,    0,  128,
  129,    0,    0,  414,    0,  130,  449,  132,    0,  450,
  134,  135,    0,    0,    0,  415,    0,    0,    0,    0,
    0,  416,  417,  519,    0,    0,  451,    0,  452,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  454,    0,    0,    0,    0,  303,    0,    0,  418,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  420,    0,  421,
  422,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,  424,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  425,    0,
    0,    0,    0,    0,  426,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  427,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1174,    0,    0,  428,  429,
  430,  431,  226,  227,    0,    0,    0,    0,    0,    0,
    0,    0,  432,    0,    0,    0,    0,    0,    0,  113,
  114,  433,  434,  435,  436,  437,  438,  439,  440,  441,
  115,  442,    0,  117,    0,    0,  443,  444,    0,  445,
  446,  447,    0,  120,  121,  448,  122,    0,    0,  123,
  124,  125,  126,  127,    0,    0,    0,  128,  129,    0,
    0,    0,    0,  130,  449,  132,    0,  450,  134,  135,
  413,    0,    0,    0,    0,    0,    0,  414,    0,    0,
    0,  519,    0,    0,  451,    0,  452,    0,    0,  415,
    0,    0,    0,    0,    0,  416,  417,    0,    0,  454,
 1743,    0,    0,    0,  303,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,  421,  422,    0,    0,    0,    0,    0,
    0,    0,    0,  423,    0,    0,    0,  424,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1226,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  425,    0,    0,    0,    0,    0,  426,    0,
    0,    6,    0,    0,    0,    0,    0,    0,  427,    0,
    0,    0, 1430, 1431,    0, 1432,    0,    0,    0, 1381,
    0,    0,  428,  429,  430,  431,  226,  227,    0,    0,
    0, 1433,    0,    0,    0,    0,  432, 1434,    0,    0,
    0,    0,    0,  113,  114,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  115,  442,    0,  117,    0,    0,
  443,  444,    0,  445,  446,  447,    0,  120,  121,  448,
  122,    0,    0,  123,  124,  125,  126,  127,  413,    0,
    0,  128,  129,    0,    0,  414,    0,  130,  449,  132,
    0,  450,  134,  135, 1435,    0,    0,  415,    0,    0,
    0,    0,    0,  416,  417,  986,    0,  986,  986,    0,
  986,    0,   14,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  986,    0,    0,    0, 1436,  986,    0,
    0,  418,    0,    0,  419,    0,    0,   16,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  431,    0,    0,    0,  420,
    0,  421,  422,    0,    0,    0,    0,    0,    0,    0,
    0,  423,    0,  113,  114,  424,    0,    0,    0,    0,
    0,    0,    0,    0, 1437,  116,    0,  117,    0,    0,
    0,    0,    0,    0,  118,  119,    0,  120,  121,    0,
  122,    0,    0,  123, 1438,  125,  126,  127,    0,    0,
    0,  128,  129, 1439, 1440, 1441,    0,  130,  131,  132,
    0,  133,  134,  135,    0,    0,    0,    0,    0,    0,
  425,    0,    0,    0,    0,    0,  426,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  427,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  428,  429,  430,  431,  226,  227,    0,    0,    0,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,    0,
    0,  113,  114,  433,  434,  435,  436,  437,  438,  439,
  440,  441,  115,  442,    0,  117,    0,    0,  443,  444,
    0,  445,  446,  447,    0,  120,  121,  448,  122,    0,
    0,  123,  124,  125,  126,  127,    0,    0,    0,  128,
  129,    0,    0,    0,    0,  130,  449,  132,    0,  450,
  134,  135,  986,    0,    0,    0,    0,    0,    0,  986,
    0,    0,    0,  453,    0,    0,  451,    0,  452,    0,
    0,  986,    0,    0,    0,    0,    0,  986,  986,    0,
    0,  454,    0,    0,    0,    0,  303,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  986,    0,    0,  986,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  986,    0,  986,  986,    0,    0,    0,
    0,    0,    0,    0,    0,  986,    0,    0,    0,  986,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  986,    0,    0,    0,    0,    0,
  986,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  986,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  986,  986,  986,  986,  986,  986,
    0,    0,    0,    0,    0,    0,    0,    0,  986,    0,
    0,    0,    0,    0,    0,  986,  986,  986,  986,  986,
  986,  986,  986,  986,  986,  986,  986,  986,    0,  986,
    0,    0,  986,  986,    0,  986,  986,  986,    0,  986,
  986,  986,  986,    0,    0,  986,  986,  986,  986,  986,
  413,    0,    0,  986,  986,    0,    0,  414,    0,  986,
  986,  986,    0,  986,  986,  986,    0,    0,    0,  415,
    0,    0,    0,    0,    0,  416,  417,  519,    0,    0,
  451,    0,  452,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  454,    0,    0,    0,    0,
  303,    0,    0,  418,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,  421,  422,    0,    0,    0,    0,    0,
    0,    0,    0,  423,    0,    0,    0,  424,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  425,    0,    0,    0,    0,    0,  426,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  427,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  428,  429,  430,  431,  226,  227,    0,    0,
    0,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,    0,    0,  113,  114,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  115,  442,    0,  117,    0,    0,
  443,  444,    0,  445,  446,  447,    0,  120,  121,  448,
  122,    0,    0,  123,  124,  125,  126,  127,    0,    0,
    0,  128,  129,    0,    0,    0,    0,  130,  449,  132,
    0,  450,  134,  135,  413,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  986,    0,    0,  986,    0,
  986,    0,    0,  415,    0,    0,    0,    0,    0,  416,
  417,    0,    0,  986,    0,    0,    0,    0,  986,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  418,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  420,    0,  421,  422,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,  424,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  425,    0,    0,    0,
    0,    0,  426,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  427,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  428,  429,  430,  431,
  226,  227,    0,    0,    0,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,    0,    0,  113,  114,  433,
  434,  435,  436,  437,  438,  439,  440,  441,  115,  442,
    0,  117,    0,    0,  443,  444,    0,  445,  446,  447,
    0,  120,  121,  448,  122,    0,    0,  123,  124,  125,
  126,  127,  986,    0,    0,  128,  129,    0,    0,  986,
    0,  130,  449,  132,    0,  450,  134,  135,    0,    0,
    0,  986,    0,    0,    0,    0,    0,  986,  986,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  986,    0,    0,  986,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  986,    0,  986,  986,    0,    0,    0,
    0,    0,    0,    0,    0,  986,    0,    0,    0,  986,
    0,    0,    0,  321,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  321,    0,    0,    0,    0,    0,    0,  321,    0,
    0,    0,  321,  321,    0,  321,    0,    0,  321,    0,
    0,    0,    0,    0,  986,    0,    0,    0,    0,    0,
  986,  321,    0,    0,    0,    0,    0,  321,    0,    0,
  986,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  986,  986,  986,  986,  986,  986,
    0,    0,    0,    0,    0,    0,    0,    0,  986,    0,
    0,    0,    0,    0,    0,  986,  986,  986,  986,  986,
  986,  986,  986,  986,  986,  986,  986,  986,    0,  986,
    0,    0,  986,  986,  321,  986,  986,  986,    0,  986,
  986,  986,  986,    0,    0,  986,  986,  986,  986,  986,
    0,    0,  321,  986,  986,    0,    0,    0,    0,  986,
  986,  986,    0,  986,  986,  986,    0,  321,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  321,    0,    0,
    0,    0,    0,    0,    0,    0,  321,    0,  321,    0,
    0,    0,    0,    0,    0,  321,    0,  617,    0,    0,
  618,    0,    0,    0,    0,    0,  818,    0,    0,    0,
    0,    0,    0,  321,  321,    0,    0,  619,    0,    0,
  620,    0,    0,    0,  321,  321,    0,  321,    0,    0,
    0,    0,    0,    0,  321,  321,    0,  321,  321,    0,
  321,    0,    0,  321,  321,  321,  321,  321,    0,    0,
    0,  321,  321,  321,  321,  321,    0,  321,  321,  321,
  621,  321,  321,  321,    0,    0,  622,  623,  624,    0,
  625,  626,  627,    0,  628,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  629,    0,  630,  617,    0,    0,  618,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  619,    0,    0,  620,    0,
    0,    0,    0,  631,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  632,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  621,    0,
    0,    0,  633,    0,  622,  623,  624,    0,  625,  626,
  627,    0,  628,    0,    0,    0,    0,  112,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  629,    0,  630,    0,    0,  113,  114,  634,  635,    0,
    0,    0,    0,    0,    0,    0,  115,  116,    0,  117,
    0,    0,    0,    0,  636,    0,  118,  119,    0,  120,
  121,  631,  122,    0,    0,  123,  124,  125,  126,  127,
    0,    0,    0,  128,  129,    0,    0,    0,  632,  130,
  131,  132,    0,  133,  134,  135,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  633,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  112,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  113,  114,  634,  635,    0,    0,    0,
    0,    0,    0,    0,  115,  116,    0,  117,    0,    0,
    0,    0,  636,    0,  118,  119,    0,  120,  121,    0,
  122,    0,    0,  123,  124,  125,  126,  127,    0,    0,
    0,  128,  129,    0,    0,  112,    0,  130,  131,  132,
    0,  133,  134,  135,  179,  180,  181,  182,  183,  184,
  185,  186,  187,  113,  114,  188,  189,  190,  191,  192,
    0,    0,  193,  194,  115,  116,    0,  117,    0,    0,
  195,  196,    0,    0,  118,  119,  197,  120,  121,  198,
  122,  199,    0,  123,  124,  125,  126,  127,    0,    0,
    0,  128,  129,  200,  201,  202,  203,  130,  131,  132,
  204,  133,  134,  135,  458,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  458,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,    0,
    0,  458,  458,  458,  458,    0,  458,    0,    0,  458,
  458,    0,    0,  458,  458,  458,  458,  458,  458,  458,
  458,    0,  458,  458,  458,  458,  458,    0,    0,    0,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,
};
static short yycheck[] = {                                       9,
    4,    0,   68,   58,  111,  554,   16,  163,   18,  153,
  154,   67,   66,   74,   16,   77,  153,  154,  738,  391,
  391,  387,   67,  489,   90,  713,  359,  361,   99,   96,
   72,  719,  586,  289,   62,  453,  530,   98,  100,   94,
  557, 1066,  383,  699, 1226,  745,   56,   57,   58,   59,
   60,   61,   62,  792,   96,  936,  994,   67,   68,   69,
  453,   71,   59,   73,   74,   93,   76,   77,   78,  725,
  790,  508, 1225,  101,   71, 1298,   86,  368,   88,   89,
   90,   60,   92,   93,   94,   95,   88,   97,   98,   99,
  100,  101,  973,  103,   73,   97,  167,   41,   95,    0,
  145,    0,  147,  509, 1014,  396,  103, 1055,  305,  603,
    0,  662,   45,  579,   44,  581,    0,  334, 1066, 1095,
  586,   45,  669,   44,  240, 1062,  519,    0, 1244,   45,
  158,    0, 1055,  889, 1055,  145,  146,  147, 1055,    0,
   41,  151,  249, 1066, 1118, 1066,  367,    0,  158, 1066,
    0,   40,    0,  163,  208,  165,  166,  167,   59,   44,
   59,  171,   44, 1296,    0,    0,   44,  146,  165,   59,
 1055,    0,   41, 1055,   40,   59, 1113,  797,  334, 1256,
   41, 1066,  236,   44, 1066,   44,   59,  166,   41,   41,
   59,   44,  812,    0,    0,  815,    0,   40,   59,  565,
    0,   41,  676,  273,  824,   41,   59,  681,   44,   59,
    0,   59,   41,    0,  586,  586,    0,   40, 1206,  335,
   63, 1209,   45,   59,   59,  676,    0,   41,   91, 1504,
   59,   63,  339,   61, 1561,  280,  447,   44,  283,    0,
  547,  929,  930,   59,  500,   42,  171,   41,  614,  280,
   47,   41,   59,   59,   44,   59,  589,   41,  549,   59,
  516,  316,  882, 1245,  273, 1290, 1291,   41, 1450,   59,
  280,  322,   59,  283,   45,   59,   44,  317,  966,  289,
  290,   41,  275,  272,   44,   59, 1439, 1440,  681,  334,
   44,  264,   63,  351,    0, 1060,  371,  300,   59, 1324,
   40,    0, 1067,    0,    0,  638,  316,   44,  484,   41,
  320,  729,  344, 1078,  498,  360,  324,  352,  257,  737,
   61,   59,  367,   63,  369,  310,  371,  380,  262,  374,
  384,    0,  386,  394, 1271,   41, 1101, 1319,   44,  348,
  516,  320, 1290, 1291,  315,  348,  400, 1257,  341,  404,
  360,    0,  508,   59, 1330, 1293,    0,  367,  315,  369,
   59,  371,   59,   59,  374,  440, 1054, 1290, 1291, 1290,
 1291,  422,  866, 1290, 1291,  841, 1324,    0,  429,  432,
   41,    0, 1038,  393,  394, 1660,  421,  317,  458, 1270,
   59,  330,  402, 1526,  404,  262,  393,   41,  408, 1337,
  431, 1324,    0, 1324,  408, 1290, 1291, 1324, 1290, 1291,
   59,  401,  275,    0,  315,   59,  264, 1494,   41,    0,
 1040,   44,   41,  402,  409,   44,  484, 1415,  902,   44,
  403, 1419,  906,  315,    0,  336,   59, 1057, 1058, 1324,
   59,  261, 1324,   44,  454,  379,  315,  297,  456,   41,
 1138, 1139,   41, 1141, 1142,  906,  325, 1077,  327, 1682,
   41,   59, 1789,  508,    0,  772,  306,  336,  605,  297,
   93,  258,   59,  806,  445,   41,  809,  509,   59,  315,
  277,  380, 1635,  270,  890,  282,  315,  820,  498,  499,
  500,  297,  306,   59,  344,  901,  380,  507,  754,  905,
  336,  511,  499,   41,  450,  389,  516,  336,  518,  554,
  297,  380,  379,  567,  847,  376,  303,  450,  305,  498,
  389,  387, 1222,   59,    0,  315,  450,  379,  507,  450,
  262,  315,  511,  301,  450,  325,  652,  327,  344,  606,
 1656,  586,  390,  428,  554,  446,  336,  301, 1313,  339,
  439,  380,  336,  477,  608,  428,  297,  344, 1781,  893,
  389, 1112,  336,  432,  606,   41,  444, 1004,   44,  513,
  767, 1120, 1510,  380,  791,  585,  586,  446, 1125, 1515,
 1336,  591,  389,   59, 1558,  379,  402,  446, 1006,  880,
  380, 1279,  813,  518, 1570, 1505,  380,  401,  742,  389,
  423,  262, 1539,  432,  658,  659, 1829,  450, 1014,  315,
  446,  446,  513, 1301,  513,  264,  661,  446,  450,  325,
 1557,  327,    0,  513, 1560, 1613,    0,  450,    0,  513,
  336, 1854,  493,  339, 1816,  791, 1042,  444,  775,  376,
  493,  402,  432,  403,  513,  344,  446,  379,  432,  264,
    0,  661,  423,  305,  444,  827,  446,  257,  494,  509,
 1842,  262,  446,  315,  361,  361,  264,   41, 1450,   41,
  262,  315,  446,  262,  380,  344,  704,  513,  450,  450,
    0,   59,  319,  389,  513,   59,    0,   59,    0,  276,
  277,   41,  336, 1875,  704, 1576,  315,   41,  743,  315,
  745,   41,  747,  509,   44, 1887,  513, 1048, 1890,   59,
  450, 1367,    0,  283, 1739, 1335, 1898,  336,  379,   40,
  730,   41,  509,  513,  262,    0,  432,   41,  738,  513,
  330, 1064, 1150,  743,  315,  745, 1889,  747,  444,   59,
  446,  390,    0, 1076,  754,   59,  791,   59,  802,  315,
  334,  796,  762,  763, 1519,  336, 1694, 1150, 1523, 1524,
  770,  771, 1700,   42,  395,  262,  763, 1100,  813, 1508,
  336,   59,  839, 1114,  395,  785,  999,  787,  379,  402,
  790,    0, 1299,  762,   59,  372,  796,  379,  319, 1123,
  379, 1739,  390,  445, 1559,  840,  841,  839, 1216,  430,
  854,   59,  446,  813,  846,    0,  785,  513,  787,  876,
  877,  878,   42,   43,  380,   45, 1739,   47, 1739, 1507,
    0,  863, 1739,  389,  304,  305,  394,  446, 1322,  445,
  840,  841, 1326,  260,  876,  877,  878, 1255,    0,  315,
   59,  379,  318,  280,  380,  395,  856, 1401, 1004,  325,
  434,  327,  856,  389, 1739,  331,    0, 1739, 1192,  272,
  336,   41,    0,  339,   59,  446,  432,  262,    0,    0,
 1821,  881,    0,  300,  410,  264,    0,  297,  354,   59,
  446,  891,  379,  453,  454,    0,  264,    0,  263,  899,
  900,  305,  368,  514,  513,  899,    0,   59, 1070, 1071,
  376,  315,  881, 1668,  380,   40,  298, 1672,  444,   41,
   41,  260,  891,  389,  268,   59,    0,   41,  262,  268,
   44,   59, 1687,  336,  344, 1876,    0,   59,   59,  405,
  427,   59,  513,  273,  309,   59,  284,  298, 1626, 1704,
  377,  315,  970,  315,   59,  320,   59,  513, 1120,  297,
  500,  325,  264,  327,    0,   59,  432,  376, 1424, 1004,
  970, 1098,  336,  443,  336,  315,  306,  298,  444,   93,
  446, 1364, 1365, 1229, 1340,   59, 1232,  513, 1311, 1372,
 1373,  260, 1375, 1376,  379,   59,  336,    0,  998,  999,
 1383, 1384, 1385, 1386,  431, 1388, 1389, 1390, 1391, 1392,
 1393,  276,  277,  334, 1014,  359,  380,    0,  348, 1342,
  376,  448,  390,   59,  403,  389,  336,  493,  276,  277,
  344,  300,  336, 1788,  304,  305, 1791, 1250, 1251, 1401,
 1401,  445,  427,  317, 1816,  379,  339,  513,    0,    0,
 1107,  517,  434,    0,  419,  264,   59,  277,   41,   44,
  338,   44,  282,  304,  305, 1122,  265,  333,  432,  362,
 1842, 1128,    0,  403,    0, 1107,   59,  271, 1833,  264,
 1226, 1116,  446,  434,  446, 1120, 1764, 1087,  390,   41,
 1122,  285, 1770,  371,   41,  300, 1128,   44, 1092,  509,
  299, 1811,  368, 1875,  276, 1105,  446,   59,   59, 1243,
 1244, 1245,   59,  434,  380, 1887, 1116,  371, 1890,  397,
 1120,  260,  400,  297,    0,  418, 1898,  259,  458,   44,
  258,   59,  316,   59,  333,  329,  446,  269,  416,  453,
  454,  259,  270,  348,  316,  315,  264,  397, 1275,  513,
 1277,  269,   44,    0,  428,  325,  410,  327,   44,  298,
  378,  264,  397,  357,  442,   41,  336,  372,   44,  297,
  264,  310, 1233,  443,  348,  303,    0,  305,   93, 1225,
 1231,  390, 1234,   59,  428,    0,  338, 1222, 1188,  265,
 1225, 1226,  297,    0,  315,  334, 1188,    0,  448,    0,
  264,   93,  443,  457,  338, 1528,  327,   93,  428,  393,
  380, 1338,   59,  448,  336,  336,  344,   41,  333,  389,
   44,  393, 1222,  299,  352, 1225, 1226,  421,  264, 1229,
 1230, 1231, 1232, 1233, 1234,   59,  485,  371,    0,  344,
   44, 1564,  409, 1230,   59,  397,  495,  352,  400,  433,
 1250, 1251,   59,  368,    0,  258,   59, 1257,   59,  380,
  399,  433,  432,  397,  416,  380,  400,  270,  389,  436,
  409,  378,  390,  318,  444,    0,  446,    0,    0,  262,
    0,  386,  416,  388,  298, 1824,  331,  390,  402,   93,
  442,    0,  434,  421,  297,  434,  390,   59,   42,   43,
  303,   45,  305,   47, 1450,  410,  486, 1351,  442,  354,
  333,  432, 1356,   59,  258,  260,   41,    0,   41,   44,
  334,   44,   44, 1317,  446,  446,  390,    0,   42,   43,
  435,   45,  315,   47,   59,  318,   59,   59,   61,   59,
  410,  344,  325,  513,  327,   59,    0,  279,  331,  390,
   59, 1497,  259,  336,  390,  300,  339,    0,   41,  303,
  405,   44,  269,  315,   42,   43,  446,   45,  315,   47,
  302,  354,  397,  273, 1430,  327,   59,    0, 1701,  311,
  327,  509,    0,  513,  336,  368,   59,  428,    0,  336,
   40,  416,  513,  376, 1394,    0,  379,  380,  372, 1804,
  276,  277, 1394, 1808,  509,   59,  389,  457,  284,  459,
  338,  258,  338,  345, 1819, 1450,   59,  349,    0,  393,
    0,  462,  405,  270,    0,  399,   42,   43,  380,   45,
 1430,   47,    0,  380, 1839,  260,   59,  389, 1438, 1439,
 1440,   59,  389,  371,  427,  371,  416,   59,  396,  432,
 1450,  450,  276,  277,   59,  395,  303,  318,  305,  433,
  284,  444, 1867,  446,  412,   41,  410, 1872,   44,  397,
  331,  397,  400,   41,  400,  300,   44,   59,  477,   59,
  432, 1481, 1482,   59,  428,  432,  426,    0,  416,  520,
  416,   59, 1638,  354,  446,    0,  372, 1497,  453,  446,
  376,  441,  313,    0, 1504, 1505,  509,  368,    0,  260,
  493, 1657,   44,  445,  442,  376,  442,  393,  462,  451,
  452,  453, 1656,  399,  260,  260,    0,   59,  315, 1529,
  513,  333,  306,  277,  517,  258,   41,    0,  282,   44,
  314,  452,   42,   43,  405,   45,   59,   47,  372,  300,
  273,  276,  277,  264,   59,  409,   61,  433, 1603,  284,
 1529,  513,   59,  277,  300,  300,  513,   59,  282,  393,
  444,  298,  277,  317,  297,  399,  258,  282,   41,  261,
  303,   44,  436,  306,  318,   59,   41,  519, 1685,   44,
  332,  523,   40,  276,  277,   44,   59,  331,  403,  277,
 1600,  284,  395, 1603,  282,  339,  426,  334, 1600,  433,
   59,   42,   43, 1260,   45, 1262,   47,  493,   46,  352,
  354,  303,  336, 1623,  336,  348,   42,   43,   40,   45,
   58,   47,  493,   61,  368, 1635, 1623,  315,   41,   42,
   43, 1686,   45,  317,   47,   42,   43,  372,   45,   41,
   47,   40,   44,  386,  315,  388,  517, 1657, 1804, 1648,
 1660,  277, 1808, 1707, 1810,    0,  282, 1711,  393,  493,
 1816,  405,  399, 1819,  399,   41, 1676,  410,   44,  315,
  403, 1681,  409,  258,  376, 1782, 1686,  410,  376,  372,
  449,  450,  451, 1839,  356,  270, 1842,  485,  486,   46,
 1756,    0,  435,  313,  314,  428,   41,  434,  433,   44,
  393,   58, 1681,  346,   61,  348,  399,  498,  477,  478,
  336, 1867,   46, 1835,   59, 1837, 1872,  428,  303, 1875,
  305,  384,  385, 1733,   58,  458,  442,   61,  377,  462,
   41, 1887,   41,   44, 1890,   44,  453,  454,  375,  681,
  433,   41, 1898,  258,   44,   41, 1756, 1757,   44,  426,
   59,   41,   42,   43, 1758,   45,  497,   47,  493,  701,
  275,  276,  277,  705,    0,   41,  281,  277,   44,  284,
  712, 1816,  282,  517,   41,  717,  718,   44,  700, 1824,
  702,  508,  297,  509,  706,  707,  708,  729,  303,  731,
  732,  733,  734,  735,  736,  737,   41, 1842,  308,   44,
  493, 1811, 1801,  276,  277,   41, 1816,   41,   44,  409,
   44, 1821,   42,   43, 1824,   45,   41,   47,   41,   44,
  284,   44,  508,   59,   44, 1835,   41, 1837,  446,   44,
 1875,  773, 1842,   61,   41,   41,  277,   44,   44,  265,
   41,  282, 1887,   44,   41, 1890,   41,   44,  461,   44,
  366,  277,  794, 1898,  283,  262,  282,  372,  800,  801,
  453,  454,   40,    0,  277, 1875, 1876,  308,   41,  282,
  277,   44,  264,  299,   40,  282,  389, 1887,  393, 1889,
 1890,   40,  281,   41,  399,   40,   44,   40, 1898,    0,
   41,   42,   43,  408,   45,  410,   47,   42,   43,  372,
   45,  453,   47,   40,   41,   42,   43,   44,   45,   46,
   47,  351,   41,  428,   59,   44, 1481, 1482,  433,   40,
  393,   58,   59,   60,   61,   62,  399, 1481, 1482,  453,
   41,  276,  277,   44,   40,  453,  281,   42,   43,  284,
   45,   40,   47,   40,   40,   40,   40,  462,   59,   40,
   40,  409,  297,   40,   91,   40,   93,   41,   42,   43,
  433,   45,   44,   47,   91,   46,   40,  276,  277,   61,
  264,  395,  914,  915,  916,  284,  317,  919,  920,  921,
  318,  923,  924,  925,  926,  927,  928,  277,  297,   44,
   44,   61,  282,  331,  452,  937,  938,  420,  940,   46,
  942,   40,  264,  945,  946,  947,  948,  949,  950,   44,
  276,  344,  470,  471,  344,  344,  354,  276,  297,  390,
  962,  264,  964,  481,  482,  428,  484,  372,  462,  390,
  368,  450,  262,  491,  492,  338,  494,  495,  275,  497,
  276,  277,  500,  501,  502,  503,  504,  277,  393,  344,
  508,  509,  282,  452,  399,  439,  514,  515,  516,  439,
  518,  519,  520,  372, 1006,  392,   40,  405,  371,   40,
    0,  470,  471,  345,   91,   41,   42,   43,   40,   45,
   40,   47,  481,  482,  393,  484,   42,   43,  433,   45,
  399,   47,  491,  492,  397,  494,  495,  400,  497,  315,
  264,  500,  501,  502,  503,  504,  376,   44,  382,  508,
  509,   41,  403,  416,   44,  514,  515,  516,  375,  518,
  519,  520,   40,   40,  433,  262,  277,  264,  265,   59,
  380,  282,  277,  443,  271,  315,  372,  282,   41,  442,
  277,  275,   63,  428,  296,  282,  428,  314,  285,  456,
   41,   42,   43,   40,   45,   44,   47,  393,  444,  264,
    0,  264,  299,  399,   44,  276,  277,  304,  305,  440,
  494,  308,  277,  284,   40,  273,   40,  282,  315,  517,
  317,  318,   42,   43,  321,   45,    0,   47,  325,  297,
  327,  340,  329,  277,  331,  281,  340,  433,  282,  336,
  337,   41,  339,  276,   44,  410,  408,  379, 1150,  379,
  379,  410,    0,   40,  297,   40,  352,  354,  355,   59,
  357, 1163, 1164,   40, 1166, 1167,   41,   41,   40,  362,
   44,  368, 1174,  370,   44,  372,  373,  374,  317,  376,
  277,  270,  379,  380,  438,   59,  378,  361,  317,  317,
   40,   44,  389,   41, 1196,  266,   44,   40,  325,   40,
  493,  372, 1204, 1205, 1206,  402,  264, 1209,  405, 1211,
   41,   59,  414, 1215, 1216, 1217,   58,   41,   41,   41,
   42,   43,  393,   45,  421,   47,   41,  264,  399,  446,
  427,  428,   41,   42,   43,  432,   45,  306,   47,  387,
  306,   41,   41,   40,   40,  317,  443,  444,  264,  446,
   40,  277,   40, 1255,   40,  452,  282,   40,   40,   40,
   40,  277,  433,   40,   40,   40,  282,   40,   40,   40,
   40,  262,  262,  470,  471,   41,   42,   43,  427,   45,
  305,   47,  446,   44,  481,  482,   44,  484,   41,   41,
  264,  381, 1294,  339,  491,  492,  493,  494,  495,  339,
  497,  273,  384,  500,  501,  502,  503,  504,  327,    0,
   41,  508,  509,  284,  399,  379,  513,  514,  515,  516,
  517,  518,  519,  520,  379,  315,  277,  379,  318,  379,
  379,  282,  379,  450,  372,  325,   41,  327,  276,   40,
   44,  331,   41,   41,   41,   41,  336,  317,   41,  339,
   41,   42,   43,   44,   45,   46,   47,  277,  290,  305,
  427, 1363,  282,   40,  354,   59,  305, 1369,   59,   60,
   61,   62, 1374,   59,  336,   59,  276,  277,  368, 1381,
   59,  281,  336,   59,  284,   59,  376,   59,  262,  379,
  380,   59,   41,   42,   43,   59,   45,  297,   47,  389,
   91,   59,   93,   58,  428,  324,   41,   42,   43,  339,
   45,  277,   47, 1415,  298,  405,  333, 1419,  334,  434,
  434,   41,   42,   43,  334,   45,   41,   47,  276,  277,
  298,   41,  365, 1435,  348,  378,  284,  427,  365,   41,
   41,  315,  432,   41,  318,   41,   41,   41,    0,  297,
   41,  325,   41,  327,  444,  277,  446,  331,   41,   41,
  282,   41,  336,   41,   41,  339,   41,   41,  277,   41,
   40,  315,  372,  282,   41,   59,  443,   41,   42,   43,
  354,   45,   40,   47,  378,   44,   41,  333,   40,   41,
  348,  340,   44,  393,  368,   40,   59,  458,  336,  399,
    0,  500,  376,  493, 1506,  379,  380,   59,   46,   61,
  336,  277,  376,   40,   40,  389,  282,   41,   40,   44,
  402,  427,  376,  513,  372,  376,   41,  517,   41,   42,
   43,  405,   45,  433,   47,   42,   43,   44,   45,   91,
   47,   41,   41,   59,   44,  393,   41,   42,   43,  298,
   45,  399,   47,  427,   44,  301,  434,   41,  432,   59,
  304,  290,  301,  455,   40,    0,  333,   40,  376,  376,
  444,  262,  446,  264,  265,  409,  264, 1579,  403,  264,
  271,  276,   58, 1585, 1586,  433,  277,  450,   40,  317,
   44,  282,  270,  462,  285,   41,   42,   43,  443,   45,
   40,   47,   41,  270,  264,  339,  427, 1609,  299,    0,
  301, 1613,  428,  304,  305, 1617,  339,  308,  450,  493,
 1622,  450, 1624,   41,  315,   41,  317,  318,  277,   41,
  321,   59,   59,  282,  325,  305,  327,   41,  329,  513,
  331,   41,  277,  517,   59,  336,  337,  282,  339, 1638,
   41,   42,   43,   44,   45,   46,   47,  277,   59,  301,
   59,    0,  282,  354,  355,  150,  357,    0,   59,   60,
   61,   62,    0, 1241,  514,  150,  763,  368,  291,  370,
  151,  372,  373,  374,  290,  376,  516, 1638,  379,  380,
  464,  465,  466,  467,  468,  469,  470,  471,  389, 1125,
   91,  754,   93,  894, 1706,  665,  258,   42,   43,   44,
   45,  402,   47,  277,  405, 1123,   92,  899,  282,  856,
   78,  273, 1547,  275,  276,  277, 1548, 1115, 1120,  281,
  421,  357,  284,  614,  388, 1051,  427,  428,  337, 1105,
  796,  432, 1116, 1745,  369,  297,  369,  822, 1618, 1676,
 1827,  303,  443,  444,  306,  446,   41,   42,   43, 1526,
   45,  452,   47,  291,  277, 1528, 1831, 1830, 1856,  282,
  277,  591,  813, 1775,  302,  282,  276,  277, 1745,  470,
  471,  309,  277, 1438,  284,  995, 1817,  282, 1643, 1511,
  481,  482, 1273,  484,  362,  743,  348,  603,  374,  593,
  491,  492,  493,  494,  495,  333,  497,  355,  873,  500,
  501,  502,  503,  504, 1327, 1804, 1818,  508,  509, 1808,
  372, 1810,  513,  514,  515,  516,  517,  518,  519,  520,
 1819,  277,  617,   41,   42,   43,  282,   45,  651,   47,
   13,  393,  865,   42,   43,   44,   45,  399,   47,  532,
 1839,  403,  324, 1804,  536,  156,  785, 1808,  410, 1810,
  773, 1014, 1261,   88,   40, 1775, 1600,  395, 1819,  800,
  698,  262,  372,  264,  265,  761,  428, 1394, 1867, 1165,
  271,  433,  547, 1872, 1007, 1281,  277, 1211, 1839,   -1,
   -1,  282,   -1,  393,  285,   -1,   -1,   -1,  426,  399,
   -1,   -1,  430,   -1,   -1,   -1,  458,   -1,  299,    0,
  462,   -1,   -1,  304,  305,   -1, 1867,  308,   -1,   -1,
   -1, 1872,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,  433,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,  277,   -1,   -1,  336,  337,  282,  339,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,  519,  277,   -1,  470,  471,   -1,  282,  389,   -1,
   91,   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,
  516,  432,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,  919,   -1,
  921,  452,  923,  924,   -1,  926,  927,  928,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,  277,  470,
  471,   -1,   -1,  282,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
 1364, 1365,   -1,   -1,   -1,   -1,   -1,   -1, 1372, 1373,
   -1, 1375, 1376,   -1,   -1,   -1,   -1,   -1,   -1, 1383,
 1384, 1385, 1386,   -1, 1388, 1389, 1390, 1391, 1392, 1393,
   -1,   -1,   -1,   -1,   -1,   -1,   42,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,  291,  339,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,  302,   -1,
   -1,   -1,   -1,  354,  355,  309,  357,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,  452,  368,  322,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,   -1,
   91,   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  421,   -1,  508,  509,   -1,  379,  427,  428,  514,  515,
  516,  432,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,  395,  443,  444,   -1,  446,   -1,  302,   -1,   -1,
   -1,  452,   -1,   -1,  309,   -1,   -1,  411,  313,  314,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  322,   -1,  470,
  471,   -1,  426,   -1,   -1,   -1,  430,   -1,  333,  433,
  481,  482,   -1,  484,   -1,   -1,   -1,  441,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,  460,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  395,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  411,   -1,   44,   -1,
   -1,  262,   -1,  264,  265,  519,   -1,   -1,   -1,   -1,
  271,  426,   -1,   -1,   -1,  430,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,
   -1,   -1,   -1,  304,  305,  460,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,
   61,   62,   -1,   -1,  519,   -1,  452,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,   -1,
   91,   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,
  516,  432,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  317,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   45,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,   -1,
   91,   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,
  516,  432,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
  271,   -1,   58,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,
   61,   62,   -1,   -1,  450,   -1,  452,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,   -1,
   -1,   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,
  516,  432,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
  271,   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,    0,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   41,   -1,   -1,   44,  336,  337,   -1,  339,   -1,
   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,   59,
   -1,   61,   -1,  354,  355,   -1,  357,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,  452,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  389,   -1,
   -1,   -1,   93,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  421,   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,
  516,  432,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,  275,  276,  277,   -1,   -1,
  271,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  285,   -1,   -1,  297,   -1,   -1,
    0,   -1,   -1,  303,   -1,   -1,   -1,   -1,  299,    0,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   41,   -1,   -1,   44,  336,  337,   -1,  339,   -1,
   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,   59,
   -1,   61,   -1,  354,  355,   -1,  357,   -1,   59,   60,
   61,   62,  372,   -1,   -1,   -1,   -1,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,  389,  399,
   -1,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
  421,   -1,   -1,  433,   -1,   -1,  427,  428,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,    0,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,  276,  277,   -1,   -1,
  271,  281,   -1,   91,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  285,   -1,   -1,  297,   -1,   -1,
   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,  299,    0,
   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   45,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   59,   60,
   61,   62,  372,   -1,   -1,   -1,   -1,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,  393,   -1,   -1,   -1,   -1,  389,  399,
   -1,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
  421,   -1,   -1,  433,   -1,   -1,  427,  428,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,  462,   -1,   -1,  273,   -1,  275,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,
  481,  482,   -1,  484,   -1,  303,   -1,   -1,  306,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   59,  264,  265,   -1,   -1,   -1,   -1,   -1,
  271,   -1,   -1,   -1,   -1,  393,   -1,   -1,  292,   -1,
   -1,  399,   -1,   -1,  285,  403,   -1,   -1,    0,   -1,
   -1,   -1,  410,   -1,   -1,   -1,   -1,   -1,  299,   -1,
   -1,   -1,   -1,  304,  305,   -1,  320,  308,   -1,   -1,
  428,   -1,   -1,   -1,  315,  433,  317,  318,   -1,   -1,
  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   41,
  331,   43,   44,   45,  348,  336,  337,   -1,  339,   -1,
  458,   -1,   -1,   -1,  462,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  402,   -1,   -1,  405,  419,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  421,  435,   -1,   -1,   -1,   -1,  427,  428,   -1,   -1,
   -1,  432,   -1,   -1,   -1,  449,  450,  451,   -1,  453,
  454,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,   -1,   -1,  470,
  471,   -1,   -1,  487,  488,   -1,   -1,   -1,   -1,   -1,
  481,  482,  496,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  261,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,  279,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,  294,   -1,
  296,   -1,  298,   -1,   -1,   -1,   -1,  303,   -1,   -1,
  262,   59,  264,  265,  310,   -1,   -1,   -1,   -1,  271,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,
   -1,   -1,   -1,  285,   -1,   -1,   -1,    0,  334,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,
   -1,   -1,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,  321,
   -1,   -1,   -1,  325,   -1,  327,   -1,  329,   41,  331,
   43,   44,   45,   -1,  336,  337,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,  397,  354,  355,   -1,  357,   -1,   -1,  404,   -1,
   -1,  407,   -1,  409,  410,  452,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   93,   -1,   -1,  470,  471,   -1,   -1,  389,  434,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
  402,   -1,   -1,  405,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,
   -1,  508,  509,   -1,   -1,  427,  428,  514,  515,  516,
  432,  518,  519,  520,   -1,   -1,   -1,  483,   -1,   -1,
   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  498,  499,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,  275,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,  292,   -1,   -1,  295,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  271,  367,
   -1,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,
  398,  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,  317,  318,   -1,  415,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   41,  331,   43,
   44,   45,   -1,  336,  337,   -1,  339,   -1,   -1,  437,
   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,  472,  473,  379,  380,   -1,   93,
   -1,   -1,  470,  471,   -1,   -1,  389,   -1,   -1,   -1,
   -1,  489,   -1,  481,  482,   -1,  484,   -1,   -1,  402,
   -1,   -1,  405,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,  421,   -1,
  508,  509,   -1,   -1,  427,  428,  514,  515,  516,  432,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,    0,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
  264,  265,   59,   -1,   -1,   -1,   -1,  271,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,
  304,  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,
   -1,  325,   -1,  327,   -1,  329,   41,  331,   43,   44,
   45,   -1,  336,  337,   -1,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  354,  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,   -1,  379,  380,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   42,   43,   -1,   45,  402,   47,
   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   60,   61,   62,   -1,   -1,  421,   -1,   -1,
   -1,   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,  470,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,
  494,  495,   -1,  497,    0,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,
  514,  515,  516,  517,  518,  519,  520,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   41,    0,   -1,   44,  336,
   -1,   -1,  339,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   59,   -1,   -1,  271,  354,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  285,  368,   -1,   -1,   -1,   -1,   -1,   41,   -1,  376,
   44,   -1,  379,  380,  299,   -1,   -1,   -1,   -1,  304,
  305,   -1,  389,  308,   -1,   59,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,  405,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,  271,  339,   -1,   -1,   -1,   -1,  277,
  427,   -1,   -1,   -1,  282,  432,   -1,  285,   -1,  354,
  355,   -1,  357,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   42,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  321,  389,   -1,   -1,   -1,   -1,   -1,
   -1,  329,   -1,   -1,   -1,   -1,   -1,  402,   -1,  337,
  405,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,  421,  355,   -1,  357,
   -1,   -1,  427,  428,   -1,   -1,  513,  432,   -1,   -1,
  517,   -1,  370,   -1,  372,  373,  374,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  264,   59,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,  421,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,
   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,
   -1,  325,  368,  327,   -1,   -1,   -1,  331,   -1,   -1,
  376,   -1,  336,   -1,  380,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,
   -1,   -1,   -1,   -1,  368,   -1,   -1,    0,   -1,   -1,
   -1,   -1,  376,  293,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,  405,   -1,   -1,   -1,   -1,  452,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,  470,  471,   59,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  444,   -1,  446,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,  493,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  513,
   -1,   -1,   -1,  517,  354,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
   -1,   -1,  452,   60,   61,   62,  376,   -1,   -1,   -1,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,  405,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,  432,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,    0,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,   -1,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,  271,    0,   -1,  380,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,  389,   91,  285,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,  321,   -1,   -1,   -1,   -1,  432,
   -1,   -1,  329,   -1,   59,   60,   61,   62,   -1,   -1,
  337,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  355,   -1,
  357,   -1,   -1,   -1,   -1,   -1,   91,  470,  471,   -1,
   -1,   -1,   -1,  370,   -1,  372,  373,  374,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,  518,  519,  520,   -1,   -1,
   -1,    0,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,  271,   -1,   -1,
   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,
   59,  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,   -1,  318,    0,   -1,  321,   -1,   -1,
   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,
   -1,   -1,  336,  337,   -1,  339,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
  354,  355,  277,  357,   -1,   40,   41,  282,   -1,   44,
  285,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,   59,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,  405,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,    0,   -1,  421,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,  354,
  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,  446,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   41,   42,   43,   44,
   45,   46,   47,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  405,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,  493,
   -1,   -1,   -1,   -1,    0,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   91,  432,   -1,  513,
   -1,   -1,   -1,  517,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,  315,   -1,   44,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   59,   -1,   -1,   -1,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,    0,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  513,  368,
   -1,   -1,  517,   -1,   -1,   -1,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,   -1,
  315,   -1,   59,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  427,   -1,
   -1,  336,    0,  432,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,  271,   -1,   -1,   -1,
   -1,  376,  277,   41,   -1,  380,   44,  282,   -1,   -1,
  285,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,  513,   -1,  321,   -1,  517,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,  432,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,  262,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,  493,  315,
    0,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
  405,  327,   -1,   41,   -1,  331,   44,   -1,  513,   -1,
  336,   -1,  517,  339,   -1,  262,  421,   -1,   -1,   -1,
   -1,   59,  427,   -1,   -1,   -1,   -1,  432,  354,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  444,
   -1,  446,  368,   -1,   -1,   -1,   -1,   -1,   -1,   59,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,  405,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,  493,  336,
   -1,   -1,  339,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,  354,  513,   -1,
    0,   -1,  517,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,  315,   -1,   -1,
  318,   41,   -1,   -1,   44,   -1,   -1,  325,  405,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,  493,  336,   59,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,  354,  513,   -1,   -1,
   -1,  517,   -1,   -1,    0,   -1,   -1,  444,   -1,  446,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,  405,   44,   -1,
    0,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,
   -1,   -1,  262,   59,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,   -1,  513,   -1,   -1,   -1,
  517,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,   -1,
  318,   41,   -1,   -1,   44,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,   59,
   -1,  339,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,  354,  327,   -1,   -1,
   -1,  331,   -1,   -1,    0,  493,  336,   -1,   -1,  339,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,  354,  513,   -1,   -1,   -1,  517,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,   -1,   41,  376,  405,   44,  379,
  380,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,    0,  446,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,  444,  325,  446,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   41,  339,
   -1,   44,   -1,   -1,   -1,  493,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,  368,  517,
   -1,   -1,    0,  493,   -1,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,  262,  513,   -1,   -1,   -1,  517,   -1,  315,
   -1,   -1,  318,   -1,   -1,  405,   -1,   -1,   -1,  325,
   -1,  327,   -1,   41,   -1,  331,   44,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   59,  432,   -1,   -1,   -1,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,  444,  315,  446,   -1,  318,   -1,
   -1,   -1,  368,   -1,   -1,  325,   -1,  327,   -1,   -1,
  376,  331,   -1,  379,  380,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,  389,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,  405,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,  368,   -1,
   -1,   -1,    0,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,  427,   -1,  513,   -1,   -1,  432,  517,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,  315,
  446,   -1,  318,   -1,   -1,  405,   -1,   -1,   -1,  325,
   -1,  327,   -1,   41,   -1,  331,   44,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,   -1,  427,   -1,  262,
   -1,   59,  432,   -1,   -1,   -1,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  446,  493,   -1,   -1,
   -1,   -1,  368,   -1,   -1,    0,   -1,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,  513,   -1,   -1,
   -1,  517,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  315,    0,   -1,  318,   -1,   -1,   -1,  405,
   -1,   -1,  325,  493,  327,   -1,   41,   -1,  331,   44,
   -1,   -1,   -1,  336,  262,   -1,  339,   -1,   -1,   -1,
   -1,  427,   -1,  513,   59,   -1,  432,  517,   -1,   -1,
   -1,  354,   -1,   -1,   41,   -1,   -1,   44,  444,   -1,
  446,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,  315,   -1,   -1,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,  405,  331,   -1,   -1,   -1,  493,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,  354,  513,   -1,  432,
   -1,  517,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
  368,  444,   -1,  446,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,  262,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,  405,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,   -1,   41,    0,   -1,   44,
  513,   -1,   -1,   -1,  517,   -1,  444,  315,  446,   -1,
  318,   -1,   -1,   -1,   59,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  262,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   59,   -1,   -1,   -1,
  368,   -1,   -1,   -1,   -1,  262,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,  513,   -1,   -1,   -1,  517,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,  405,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,    0,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,  315,  427,
   -1,  318,   -1,   -1,  432,   -1,   -1,   -1,  325,  354,
  327,   -1,   -1,   -1,  331,   -1,  444,   -1,  446,  336,
   -1,   -1,  339,  368,   -1,   -1,   -1,   -1,   -1,   41,
   -1,  376,   44,   -1,  379,  380,   -1,  354,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   59,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
  405,   -1,  379,  380,   -1,  493,   -1,  262,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,  513,   -1,  432,  405,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,  444,   -1,  446,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,  262,
   -1,  336,   -1,   -1,  339,   -1,   40,   -1,  493,   43,
  315,   45,   -1,  318,   -1,   -1,   -1,   -1,   -1,  354,
  325,   -1,  327,   -1,   58,   -1,  331,   -1,  513,   63,
   -1,  336,  517,  368,  339,   -1,  493,   -1,   -1,   -1,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  354,
   -1,   -1,  315,   -1,  389,  318,  513,   -1,   -1,   -1,
  517,   -1,  325,  368,  327,   -1,   -1,   -1,  331,   -1,
  405,  376,   -1,  336,  379,  380,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
  405,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,  444,
   -1,  446,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,  389,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,  315,  427,   -1,  318,   -1,   -1,  432,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,  513,  331,
   -1,  444,  517,  446,  336,   -1,   -1,  339,  493,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,  513,   -1,
   -1,   -1,  517,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,   -1,
  493,   -1,   -1,  267,   -1,   -1,   -1,  389,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  513,   -1,  286,  405,  517,   -1,   -1,   -1,  292,  293,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   58,   -1,
  432,   -1,   -1,   63,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,  284,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,
   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,  284,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  505,  506,  507,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,  452,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,  289,
   -1,   -1,  292,  293,  470,  471,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,  311,   -1,   -1,   -1,  491,  492,   -1,  494,  495,
  320,  497,   -1,  323,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,  452,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,  481,  482,   -1,  484,   -1,
   -1,   -1,  372,   -1,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  267,   -1,   -1,  508,  509,
   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,    0,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,  435,
   -1,  323,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   61,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,  481,  482,   -1,  484,   -1,
  372,   -1,   -1,   -1,   91,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,
   -1,  258,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,  273,   -1,  275,  276,
  277,   -1,   -1,  311,  281,   -1,   -1,  284,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,
  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,  372,   -1,  500,  501,  502,  503,
  504,  348,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,  393,  425,   -1,   -1,
   -1,   -1,  399,   -1,   -1,   -1,  403,  435,   -1,   -1,
   -1,   -1,   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,  428,   -1,   -1,  339,  463,  433,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,  362,   -1,  487,
  488,  458,  490,  491,  492,  462,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,
  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,  418,   43,   -1,   45,   -1,   -1,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
  320,   -1,   -1,  323,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,  481,  482,   -1,  484,
   -1,   -1,  372,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,  260,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  298,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  310,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,  334,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,  399,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  409,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  320,   -1,   -1,  323,   63,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  298,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  310,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
  334,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,  399,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,  320,   -1,   -1,  323,   63,   -1,  452,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,  272,   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,  286,
   -1,  292,   -1,   -1,  295,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,   -1,
   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,
  341,  342,  343,  452,  345,  346,  347,   -1,  349,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  360,   -1,   -1,  367,  364,  369,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  398,   -1,  508,
  509,   -1,  403,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,
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
   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,  272,   -1,   -1,  275,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,
  286,   -1,  292,   -1,   -1,  295,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,
   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,
   -1,  341,  342,  343,  452,  345,  346,  347,   -1,  349,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  360,   -1,   -1,  367,  364,  369,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,  398,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,  415,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,  472,  473,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,  489,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,
   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,  323,   63,
   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,   -1,  341,  342,  343,  452,  345,  346,  347,   -1,
  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  360,   -1,   -1,  367,  364,
  369,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  398,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,  415,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,  472,  473,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,  323,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
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
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,  472,  473,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,    0,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,
  323,   63,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,  348,   61,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,  258,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   58,   -1,   -1,   -1,  273,   63,
   -1,  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,  306,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,  393,   -1,
   -1,   -1,   -1,  425,  399,   -1,   -1,   -1,  403,   -1,
   -1,   -1,   -1,  435,   -1,  410,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  428,   -1,   -1,  325,   -1,  433,   -1,
   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,  458,   -1,  487,  488,  462,  490,  491,
  492,   -1,  494,  495,  496,  497,  260,   -1,  500,  501,
  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,
  274,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,  286,   -1,   -1,   -1,  395,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,  300,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,  323,
   63,  430,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,  348,   -1,  350,  351,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,
   -1,   -1,    0,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   58,   -1,   -1,   -1,  300,   63,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,
  323,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   91,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  270,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,  298,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,  309,  310,   -1,  312,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,  328,   -1,   -1,   -1,   -1,
  463,  334,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,
   -1,  514,  515,  516,   -1,  518,  519,  520,  391,   -1,
  258,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   40,
   -1,   -1,   43,   -1,   45,  273,  409,   -1,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,   58,   -1,   -1,
   -1,  424,   63,   -1,   -1,  320,   -1,   -1,  323,  297,
   -1,  434,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,  470,  471,  364,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
  348,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,  372,  508,  509,  510,  511,  512,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,  419,  393,   -1,   -1,   -1,   -1,
  425,  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,
  435,   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
  458,   -1,  487,  488,  462,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
  272,   -1,  323,  275,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,
  292,   -1,   -1,  295,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,
  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  367,   -1,  369,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,  398,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,  415,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,  437,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,
   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,  520,
  472,  473,    0,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   40,   41,   -1,   43,   -1,   45,  489,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,
  258,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  273,   -1,   -1,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,  297,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,  393,  425,   -1,   -1,   -1,
   -1,  399,   -1,   -1,   -1,  403,  435,   -1,   -1,   -1,
   -1,   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
  428,   -1,   -1,   -1,  463,  433,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
  458,  490,  491,  492,  462,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,  508,
  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,
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
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,
   59,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,  309,  310,   -1,  312,   -1,   -1,   -1,  446,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,  328,   -1,   -1,   -1,   -1,  463,  334,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,
   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,
   -1,  518,  519,  520,  391,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   40,   -1,   42,   43,   -1,
   45,   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,  424,   63,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,  470,  471,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
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
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
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
  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,  305,   -1,
   -1,   -1,  309,  310,   -1,  312,   -1,   -1,  315,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,  328,   -1,   -1,   -1,   -1,   -1,  334,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,  391,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,  409,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,  424,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  272,   -1,   -1,
  275,   -1,   -1,   -1,   -1,   -1,  281,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  292,   -1,   -1,
  295,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
  335,  518,  519,  520,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  367,   -1,  369,  272,   -1,   -1,  275,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  292,   -1,   -1,  295,   -1,
   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  415,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,
   -1,   -1,  437,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,   -1,   -1,  452,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  367,   -1,  369,   -1,   -1,  470,  471,  472,  473,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,  489,   -1,  491,  492,   -1,  494,
  495,  398,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  415,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  437,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,  489,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,  452,   -1,  514,  515,  516,
   -1,  518,  519,  520,  461,  462,  463,  464,  465,  466,
  467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
   -1,   -1,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,   -1,  491,  492,  493,  494,  495,  496,
  497,  498,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,  513,  514,  515,  516,
  517,  518,  519,  520,  452,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  462,  463,  464,  465,  466,  467,
  468,  469,  470,  471,  472,  473,  474,  475,  476,   -1,
   -1,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,   -1,  491,  492,  493,  494,  495,  496,  497,
  498,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,  510,  511,  512,  513,  514,  515,  516,  517,
  518,  519,  520,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 520
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
"IIF","SCALAR_ARRAY","CROSS","NEXT","SEQUENCE","RESTART",
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
"create_clause : EXCEPTION symbol_exception_name sql_string",
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
"proc_statement : exec_udf ';'",
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
"alter_clause : SEQUENCE alter_sequence_clause",
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
"alter_sequence_clause : symbol_generator_name RESTART WITH signed_64bit_integer",
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
"set_generator : SET GENERATOR symbol_generator_name TO signed_64bit_integer",
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
"exec_udf : udf",
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
"signed_64bit_integer : NUMBER64BIT",
"signed_64bit_integer : '-' NUMBER64BIT",
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
"non_reserved_word : SEQUENCE",
"non_reserved_word : NEXT",
"non_reserved_word : RESTART",
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
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 28:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 29:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 30:
{ yyval = make_list (yyvsp[0]); }
break;
case 32:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 33:
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
break;
case 34:
{ yyval = make_node (nod_select, (int) 0, NULL); }
break;
case 35:
{ yyval = make_node (nod_insert, (int) 0, NULL); }
break;
case 36:
{ yyval = make_node (nod_delete, (int) 0, NULL); }
break;
case 37:
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
break;
case 38:
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
break;
case 39:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 40:
{ yyval = NULL; }
break;
case 41:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 42:
{ yyval = NULL; }
break;
case 43:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
break;
case 44:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 45:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 46:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					make_list(yyvsp[-2]), make_list(yyvsp[0]), NULL, yyvsp[-3]); }
break;
case 47:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 48:
{ yyval = NULL; }
break;
case 49:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 50:
{ yyval = NULL; }
break;
case 54:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 55:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 56:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 57:
{ yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); }
break;
case 58:
{ yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); }
break;
case 59:
{ yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); }
break;
case 60:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 62:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 63:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 64:
{ yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); }
break;
case 65:
{ yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); }
break;
case 67:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 68:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 70:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 71:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 72:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 73:
{ yyval = yyvsp[0];}
break;
case 74:
{ yyval = yyvsp[0]; }
break;
case 75:
{ yyval = yyvsp[0]; }
break;
case 76:
{ yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); }
break;
case 78:
{ lex.g_field->fld_dtype = dtype_blob; }
break;
case 79:
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-2]; }
break;
case 80:
{ yyval = NULL; }
break;
case 82:
{ yyval = yyvsp[-1]; }
break;
case 84:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 85:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); }
break;
case 86:
{ yyval = NULL; }
break;
case 87:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
break;
case 88:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_scalar_array, CONSTANT_SLONG); }
break;
case 89:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_ref_with_null, CONSTANT_SLONG); }
break;
case 91:
{ yyval = yyvsp[-1]; }
break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); }
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
break;
case 94:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_reference, CONSTANT_SLONG); }
break;
case 95:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_value, CONSTANT_SLONG); }
break;
case 96:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
break;
case 97:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_reference), CONSTANT_SLONG); }
break;
case 98:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_descriptor), CONSTANT_SLONG); }
break;
case 99:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 100:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 101:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 102:
{ yyval = yyvsp[0]; }
break;
case 103:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 104:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 105:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
break;
case 154:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
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
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
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
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 413:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 414:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = yyvsp[0]; }
break;
case 417:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 418:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 419:
{ yyval = yyvsp[0]; }
break;
case 420:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 422:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 423:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 424:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{ yyval = yyvsp[0]; }
break;
case 427:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 428:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 429:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 430:
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
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 469:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 470:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 472:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 473:
{ yyval = yyvsp[0]; }
break;
case 474:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 476:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 477:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 478:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 480:
{ yyval = NULL; }
break;
case 482:
{ yyval = NULL; }
break;
case 483:
{ yyval = yyvsp[0]; }
break;
case 484:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 501:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 502:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 504:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 505:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 506:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 512:
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
case 513:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 514:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 515:
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
case 516:
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
case 517:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 524:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 525:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 526:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 527:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 528:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 529:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 545:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 546:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 547:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 548:
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
case 549:
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
case 552:
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
case 553:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 554:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 555:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 556:
{ yyval = yyvsp[-1]; }
break;
case 557:
{ yyval = 0; }
break;
case 561:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 562:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)); }
break;
case 566:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 567:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 568:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 569:
{ yyval = 0; }
break;
case 570:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 573:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 574:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 577:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 578:
{ yyval = NULL; }
break;
case 580:
{ yyval = NULL; }
break;
case 581:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 583:
{ yyval = NULL; }
break;
case 585:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 590:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 593:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 594:
{ yyval = yyvsp[0];}
break;
case 596:
{ yyval = yyvsp[0];}
break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 600:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 601:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 602:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 603:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 604:
{ yyval = 0; }
break;
case 605:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 606:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 607:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 608:
{ yyval = (dsql_nod*) 0; }
break;
case 609:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 610:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 612:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 613:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 614:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 615:
{ yyval = 0; }
break;
case 617:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 618:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 619:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 620:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 621:
{ yyval = NULL; }
break;
case 622:
{ yyval = yyvsp[0]; }
break;
case 623:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 624:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 625:
{ yyval = NULL; }
break;
case 626:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 627:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 628:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 629:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 630:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 631:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 633:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 634:
{ lex.limit_clause = true; }
break;
case 635:
{ lex.limit_clause = false; }
break;
case 636:
{ lex.first_detection = true; }
break;
case 637:
{ lex.first_detection = false; }
break;
case 638:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 639:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 640:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 641:
{ yyval = 0; }
break;
case 642:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 643:
{ yyval = yyvsp[-2]; }
break;
case 644:
{ yyval = yyvsp[-1]; }
break;
case 645:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 646:
{ yyval = yyvsp[-1]; }
break;
case 647:
{ yyval = yyvsp[0]; }
break;
case 648:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 649:
{ yyval = 0; }
break;
case 650:
{ yyval = make_list (yyvsp[0]); }
break;
case 651:
{ yyval = 0; }
break;
case 653:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 655:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 658:
{ yyval = make_list (yyvsp[0]); }
break;
case 660:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 665:
{ yyval = yyvsp[-1]; }
break;
case 666:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 668:
{ yyval = NULL; }
break;
case 669:
{ yyval = make_list (yyvsp[-1]); }
break;
case 670:
{ yyval = NULL; }
break;
case 672:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 676:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 677:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 678:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 681:
{ yyval = yyvsp[0]; }
break;
case 682:
{ yyval = make_list (yyvsp[-1]); }
break;
case 683:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 684:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 685:
{ yyval = make_list (yyvsp[-1]); }
break;
case 686:
{ yyval = NULL; }
break;
case 688:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 689:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 690:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 691:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 693:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 694:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 697:
{ yyval = make_list (yyvsp[0]); }
break;
case 698:
{ yyval = NULL; }
break;
case 700:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
break;
case 707:
{ yyval = NULL; }
break;
case 708:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 709:
{ yyval = 0; }
break;
case 710:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 711:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 712:
{ yyval = 0; }
break;
case 713:
{ yyval = 0; }
break;
case 715:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 716:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 719:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 720:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 721:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 722:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 725:
{ yyval = make_list (yyvsp[-1]); }
break;
case 726:
{ yyval = 0; }
break;
case 727:
{ yyval = make_list (yyvsp[0]); }
break;
case 728:
{ yyval = 0; }
break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 732:
{ yyval = 0; }
break;
case 733:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 734:
{ yyval = 0; }
break;
case 735:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 736:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 737:
{ yyval = yyvsp[-1]; }
break;
case 738:
{ yyval = 0; }
break;
case 739:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 740:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 741:
{ yyval = NULL; }
break;
case 742:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 743:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 747:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 748:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 751:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 752:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 754:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 755:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 756:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 757:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 758:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 759:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 764:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 765:
{ yyval = yyvsp[0]; }
break;
case 767:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
break;
case 776:
{ yyval = make_list (yyvsp[-1]); }
break;
case 778:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 782:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = yyvsp[-1]; }
break;
case 798:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 810:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 837:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 838:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 839:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 840:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 842:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 843:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 844:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 845:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 846:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 848:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 850:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 851:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 852:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 853:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 854:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 855:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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
case 859:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 860:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 861:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 863:
{ yyval = make_list (yyvsp[-1]); }
break;
case 864:
{ yyval = yyvsp[-1]; }
break;
case 875:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 876:
{ yyval = yyvsp[0]; }
break;
case 877:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 878:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 879:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 880:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 881:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 882:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 883:
{ yyval = yyvsp[-1]; }
break;
case 884:
{ yyval = yyvsp[-1]; }
break;
case 888:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 889:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 890:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 893:
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
case 894:
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
case 895:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 896:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 898:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 900:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 901:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 902:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 903:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 904:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 905:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 907:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 908:
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
case 909:
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
case 910:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 911:
{ yyval = make_parameter (); }
break;
case 912:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 913:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 914:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 915:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 916:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 917:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 918:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 919:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 920:
{ yyval = yyvsp[0]; }
break;
case 921:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 923:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 924:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 925:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 926:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 927:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 929:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 930:
{ yyval = yyvsp[0];}
break;
case 932:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 936:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 937:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 938:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 939:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 940:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 941:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 942:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 943:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 944:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 945:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 946:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 950:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 951:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 952:
{ yyval = yyvsp[0]; }
break;
case 953:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 954:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 955:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 956:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 959:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 960:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 961:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 964:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 965:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 966:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 967:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 968:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 969:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 970:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 971:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 975:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 976:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 977:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 978:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 979:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 980:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 981:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 982:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 983:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 984:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 989:
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
