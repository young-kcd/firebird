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
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include "../jrd/flags.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/parse_proto.h"
#include "../dsql/keywords.h"
#include "../dsql/misc_func.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"
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

const int MIN_CACHE_BUFFERS	= 250;
const int DEF_CACHE_BUFFERS	= 1000;

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE	2048
#define YYMAXDEPTH	2048

/* Make bison allocate static stack */
#define YYINITDEPTH 2048
#define YYSTACK_USE_ALLOCA 1

#define YYSTYPE		DSQL_NOD
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
static DSQL_NOD	make_list (dsql_nod*);
static dsql_nod*	make_node (NOD_TYPE, int, ...);
static dsql_nod*	make_parameter (void);
static dsql_nod*	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(dsql_nod*, SLONG*, SSHORT);
#endif
static void	stack_nodes (dsql_nod*, dsql_lls**);
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
	SSHORT log_defined, cache_defined;
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
#define BASENAME 269
#define BEFORE 270
#define BEGIN 271
#define BETWEEN 272
#define BLOB 273
#define BY 274
#define CACHE 275
#define CAST 276
#define CHARACTER 277
#define CHECK 278
#define CHECK_POINT_LEN 279
#define COLLATE 280
#define COMMA 281
#define COMMIT 282
#define COMMITTED 283
#define COMPUTED 284
#define CONCATENATE 285
#define CONDITIONAL 286
#define CONSTRAINT 287
#define CONTAINING 288
#define COUNT 289
#define CREATE 290
#define CSTRING 291
#define CURRENT 292
#define CURSOR 293
#define DATABASE 294
#define DATE 295
#define DB_KEY 296
#define KW_DEBUG 297
#define DECIMAL 298
#define DECLARE 299
#define DEFAULT 300
#define KW_DELETE 301
#define DESC 302
#define DISTINCT 303
#define DO 304
#define DOMAIN 305
#define DROP 306
#define ELSE 307
#define END 308
#define ENTRY_POINT 309
#define EQL 310
#define ESCAPE 311
#define EXCEPTION 312
#define EXECUTE 313
#define EXISTS 314
#define EXIT 315
#define EXTERNAL 316
#define FILTER 317
#define FOR 318
#define FOREIGN 319
#define FROM 320
#define FULL 321
#define FUNCTION 322
#define GDSCODE 323
#define GEQ 324
#define GENERATOR 325
#define GEN_ID 326
#define GRANT 327
#define GROUP 328
#define GROUP_COMMIT_WAIT 329
#define GTR 330
#define HAVING 331
#define IF 332
#define KW_IN 333
#define INACTIVE 334
#define INNER 335
#define INPUT_TYPE 336
#define INDEX 337
#define INSERT 338
#define INTEGER 339
#define INTO 340
#define IS 341
#define ISOLATION 342
#define JOIN 343
#define KEY 344
#define KW_CHAR 345
#define KW_DEC 346
#define KW_DOUBLE 347
#define KW_FILE 348
#define KW_FLOAT 349
#define KW_INT 350
#define KW_LONG 351
#define KW_NULL 352
#define KW_NUMERIC 353
#define KW_UPPER 354
#define KW_VALUE 355
#define LENGTH 356
#define LOGFILE 357
#define LPAREN 358
#define LEFT 359
#define LEQ 360
#define LEVEL 361
#define LIKE 362
#define LOG_BUF_SIZE 363
#define LSS 364
#define MANUAL 365
#define MAXIMUM 366
#define MAX_SEGMENT 367
#define MERGE 368
#define MESSAGE 369
#define MINIMUM 370
#define MODULE_NAME 371
#define NAMES 372
#define NATIONAL 373
#define NATURAL 374
#define NCHAR 375
#define NEQ 376
#define NO 377
#define NOT 378
#define NOT_GTR 379
#define NOT_LSS 380
#define NUM_LOG_BUFS 381
#define OF 382
#define ON 383
#define ONLY 384
#define OPTION 385
#define OR 386
#define ORDER 387
#define OUTER 388
#define OUTPUT_TYPE 389
#define OVERFLOW 390
#define PAGE 391
#define PAGES 392
#define KW_PAGE_SIZE 393
#define PARAMETER 394
#define PASSWORD 395
#define PLAN 396
#define POSITION 397
#define POST_EVENT 398
#define PRECISION 399
#define PRIMARY 400
#define PRIVILEGES 401
#define PROCEDURE 402
#define PROTECTED 403
#define RAW_PARTITIONS 404
#define READ 405
#define REAL 406
#define REFERENCES 407
#define RESERVING 408
#define RETAIN 409
#define RETURNING_VALUES 410
#define RETURNS 411
#define REVOKE 412
#define RIGHT 413
#define RPAREN 414
#define ROLLBACK 415
#define SEGMENT 416
#define SELECT 417
#define SET 418
#define SHADOW 419
#define KW_SHARED 420
#define SINGULAR 421
#define KW_SIZE 422
#define SMALLINT 423
#define SNAPSHOT 424
#define SOME 425
#define SORT 426
#define SQLCODE 427
#define STABILITY 428
#define STARTING 429
#define STATISTICS 430
#define SUB_TYPE 431
#define SUSPEND 432
#define SUM 433
#define TABLE 434
#define THEN 435
#define TO 436
#define TRANSACTION 437
#define TRIGGER 438
#define UNCOMMITTED 439
#define UNION 440
#define UNIQUE 441
#define UPDATE 442
#define USER 443
#define VALUES 444
#define VARCHAR 445
#define VARIABLE 446
#define VARYING 447
#define VERSION 448
#define VIEW 449
#define WAIT 450
#define WHEN 451
#define WHERE 452
#define WHILE 453
#define WITH 454
#define WORK 455
#define WRITE 456
#define FLOAT_NUMBER 457
#define NUMBER 458
#define NUMERIC 459
#define SYMBOL 460
#define STRING 461
#define INTRODUCER 462
#define ACTION 463
#define ADMIN 464
#define CASCADE 465
#define FREE_IT 466
#define RESTRICT 467
#define ROLE 468
#define COLUMN 469
#define TYPE 470
#define EXTRACT 471
#define YEAR 472
#define MONTH 473
#define DAY 474
#define HOUR 475
#define MINUTE 476
#define SECOND 477
#define WEEKDAY 478
#define YEARDAY 479
#define TIME 480
#define TIMESTAMP 481
#define CURRENT_DATE 482
#define CURRENT_TIME 483
#define CURRENT_TIMESTAMP 484
#define NUMBER64BIT 485
#define SCALEDINT 486
#define CURRENT_USER 487
#define CURRENT_ROLE 488
#define KW_BREAK 489
#define SUBSTRING 490
#define RECREATE 491
#define KW_DESCRIPTOR 492
#define FIRST 493
#define SKIP 494
#define CURRENT_CONNECTION 495
#define CURRENT_TRANSACTION 496
#define BIGINT 497
#define CASE 498
#define NULLIF 499
#define COALESCE 500
#define USING 501
#define NULLS 502
#define LAST 503
#define ROW_COUNT 504
#define LOCK 505
#define SAVEPOINT 506
#define RELEASE 507
#define STATEMENT 508
#define LEAVE 509
#define INSERTING 510
#define UPDATING 511
#define DELETING 512
#define KW_INSERTING 513
#define KW_UPDATING 514
#define KW_DELETING 515
#define BACKUP 516
#define KW_DIFFERENCE 517
#define OPEN 518
#define CLOSE 519
#define FETCH 520
#define ROWS 521
#define BLOCK 522
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
   50,   50,   50,   57,   57,   58,   58,   51,   51,   60,
   60,   60,   60,   60,   47,    5,   63,   63,   63,   63,
   63,   63,   63,   63,   63,   63,   63,   13,   78,   78,
   78,   14,   82,   82,   65,   65,   68,   68,   68,   76,
   91,   91,   91,   92,   92,   93,   93,   94,   94,   98,
   98,   75,  101,  101,  103,  103,  104,  104,  107,  107,
  108,  109,  109,  110,  111,   73,   77,   74,   95,   95,
  114,  115,  115,  117,  117,  118,  118,  118,  118,  118,
  116,  116,  119,  119,  120,  120,  120,  120,  120,  122,
  122,  122,  122,  121,  121,  126,  126,  125,   99,  127,
  127,  128,  131,  132,  132,  133,  129,  130,  130,  134,
  134,  135,  135,  136,  136,  136,   97,   97,   97,   70,
   80,  137,  137,  137,  138,  138,  139,  139,  140,  140,
  140,  146,   87,   87,  142,  142,  105,  105,  100,  150,
  151,   59,  143,  143,  106,  106,  106,  106,  106,  106,
  144,  144,  158,  158,  159,  161,  161,  161,  161,  161,
  141,  160,  160,  165,  165,  165,  165,  167,  168,  169,
  163,  163,  164,  162,  162,  162,  162,  162,  170,  171,
  172,  172,  172,  172,   69,   79,   83,  177,  173,  173,
  174,  174,  178,  178,  180,  179,  179,  182,  181,  181,
  181,  175,  175,  183,  183,  184,  186,  186,  187,  185,
  185,  189,  189,  188,  192,  192,  176,  194,  194,  194,
  195,  195,  193,  193,  193,  193,  193,  193,  193,  193,
  193,  193,  193,  193,  193,  193,  193,  193,  193,  193,
  193,  198,  198,  199,  200,  201,  202,  207,  208,  203,
  203,  204,  216,  205,  217,  211,  211,  211,  212,  212,
  212,  214,  214,  214,  214,  206,  213,  213,  209,  209,
  209,  215,  215,  196,  196,  222,  223,  223,  224,  224,
  224,  224,  210,  210,  210,  226,  228,  227,  229,   11,
   12,  230,  230,  231,  231,  232,   72,   81,  234,  237,
  237,  237,  238,  241,  245,  245,  246,  246,  247,  247,
  148,  236,   88,   90,  235,  235,   71,   84,  250,  250,
  250,  251,  254,  254,  255,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  255,  252,
  252,  253,    2,  257,  257,  257,  257,  257,  257,  257,
  265,  266,  263,  263,  267,  267,  267,  267,  267,  267,
  267,  258,  258,  268,  268,  268,  268,  268,  268,  268,
  262,  273,  273,  273,  273,  273,  273,  273,  273,  273,
  273,  273,  273,  273,  273,  273,  273,  273,  273,  273,
  273,  273,  273,  273,  273,  273,  273,  273,  270,  270,
  272,  272,  271,  269,  269,  269,  264,  264,  260,  261,
  261,  275,  275,  275,  275,  275,  275,  275,  275,  276,
  276,  259,  277,  277,  278,  278,    8,  279,  279,  279,
  279,  279,  279,  279,  279,  279,  279,  279,  279,  102,
  102,  145,  145,  282,  282,  285,  285,  287,  287,   54,
   54,  284,  284,  284,  284,  284,  284,  284,  284,  284,
  292,  292,  283,  283,  283,  283,  293,  293,   62,   62,
   62,   56,   56,  289,  289,  289,  286,  286,  286,  297,
  297,  297,  296,  296,  295,  295,  295,  290,  290,  298,
  298,  298,  299,  299,  291,  291,  291,  291,  300,  300,
   19,   19,   19,  302,  302,  302,   17,   17,   17,  304,
  305,  308,  308,  306,  310,  310,    4,   16,  309,  309,
  311,  311,  312,  312,  301,  313,  313,  314,  314,  315,
  315,  315,  315,  316,  316,  317,  317,  318,  318,  320,
  320,  320,  321,  321,  321,  322,  322,  322,  319,  324,
  324,  324,  325,  325,  323,  323,  326,  328,  328,  327,
  327,  303,   18,  329,  329,  329,  330,  330,  335,  335,
  336,   66,   66,   66,  338,  338,  337,  337,  331,  331,
  331,  332,  332,  341,  341,  333,  333,  334,  191,  345,
  346,  339,  340,  342,  342,  342,  342,  347,  347,  347,
  348,  348,  348,  239,  239,  240,  240,  350,  350,  351,
  351,  353,  353,  343,  355,  355,  356,  356,  356,  359,
  360,  360,  361,  361,  363,  363,  357,  357,  358,  358,
  364,  364,  248,  248,   32,  249,  249,  249,  249,  249,
  249,  249,  249,  243,  243,  365,  365,  366,  244,  244,
  242,  242,  344,  344,  367,  368,  368,  368,  368,  368,
  369,  369,  370,  370,  371,  371,  372,  372,  372,  373,
  373,  374,  374,   10,   10,    7,    7,  376,  377,  378,
   20,   20,  379,  380,  381,  381,  197,  352,  352,  219,
  219,    3,    3,  383,  383,  383,  385,  385,   62,  384,
  384,  386,  386,   35,   35,   86,   85,   85,  375,  375,
  387,  388,  388,  220,  220,  220,  147,  382,  382,  112,
  112,  112,  112,  112,  392,  392,  392,  392,  392,  392,
  391,  391,  391,  394,  394,  394,  394,  394,  394,  394,
  394,  394,  394,  395,  395,  395,  395,  395,  395,  395,
  395,  400,  400,  400,  400,  400,  400,  400,  400,  400,
  400,  400,  400,  400,  400,  400,  400,  406,  406,  396,
  396,  397,  397,  397,  397,  398,  398,  402,  402,  403,
  403,  403,  403,  401,  404,  399,  399,  390,  390,  390,
  393,  393,  393,  407,  407,  408,  405,  410,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,  157,  157,  157,  411,  409,  409,
  152,  152,  417,  417,  417,  417,  417,  413,  413,  413,
  413,  413,  233,  153,  153,  154,  155,  155,  155,  155,
  155,   52,   52,   21,   21,  256,  418,   55,  124,  288,
  288,   96,  412,  412,  412,  412,  419,  419,  419,  419,
  419,  419,  419,  419,  419,  419,  419,  420,  421,  423,
  422,  422,  425,  426,  426,  416,  416,  414,  415,  415,
  427,  427,  428,  428,  429,  429,  432,  432,  430,  430,
  435,  435,  434,  431,  433,  424,  424,  424,  424,  424,
  424,  424,  424,  349,  349,  156,   49,  294,  123,  149,
  389,  166,  190,  281,   64,   61,  225,  113,   67,  354,
  221,   36,   42,  362,  280,   40,   43,  218,   41,  307,
  274,  274,  436,  436,  436,  436,  436,  436,  436,  436,
  436,  436,  436,  436,  436,  436,  436,  436,  436,
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
    0,    1,    3,    1,    3,    2,    4,    1,    3,    2,
    3,    4,    4,    2,    9,    2,    3,    7,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    4,    2,    2,    1,    0,    1,    1,    6,    6,
    1,    1,    0,    0,    1,    0,    4,    0,    1,    1,
    2,    8,    1,    0,    3,    0,    0,    1,    1,    2,
    1,    1,    1,    2,    6,    1,    1,    3,    0,    1,
    1,    0,    1,    1,    2,    3,    4,    2,    2,    3,
    0,    1,    1,    2,    1,    1,    1,    4,    3,    3,
    3,    3,    3,    1,    1,    6,    3,    1,    3,    1,
    3,    2,    1,    0,    2,    3,    1,    0,    1,    1,
    2,    3,    4,    0,    1,    2,    0,    1,    1,    5,
    5,    3,    2,    0,    1,    3,    1,    1,    6,    3,
    2,    6,    2,    1,    2,    2,    2,    0,    1,    1,
    2,    0,    2,    0,    1,    1,    1,    1,    1,    1,
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
    7,    3,    0,    1,    3,    3,    7,    7,    1,    1,
    3,    4,    7,    2,    1,    3,    1,    1,    6,    3,
    0,    0,    0,    0,    3,    0,    9,    9,    1,    1,
    0,    2,    1,    1,    1,    1,    1,    3,    3,    3,
    3,    3,    3,    5,    5,    5,    5,    5,    5,    2,
    0,    4,    2,    3,    3,    2,    2,    3,    3,    2,
    3,    6,    1,    2,    4,    3,    2,    2,    2,    2,
    3,    1,    3,    3,    3,    2,    2,    4,    4,    5,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    2,    1,    1,    1,    0,    2,    2,    0,    1,
    2,    2,    2,    2,    2,    4,    3,    2,    2,    1,
    3,    7,    1,    0,    1,    0,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    3,    2,    2,    2,    1,
    1,    1,    1,    4,    5,    1,    3,    1,    3,    1,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    4,    4,    6,    5,    3,    0,    2,    2,
    0,    3,    0,    4,    1,    5,    4,    1,    4,    1,
    2,    2,    1,    1,    1,    2,    2,    2,    2,    0,
    3,    5,    1,    1,    2,    3,    1,    2,    3,    0,
    1,    1,    1,    5,    5,    6,    1,    1,    1,    2,
    4,    1,    0,    5,    1,    0,    3,    2,    1,    0,
    2,    0,    1,    0,    3,    1,    0,    1,    2,    1,
    1,    1,    1,    2,    2,    1,    2,    3,    1,    1,
    3,    3,    1,    2,    3,    1,    2,    0,    2,    1,
    1,    0,    1,    1,    1,    3,    2,    3,    0,    1,
    3,    4,    5,    1,    3,    4,    3,    0,    1,    3,
    3,    1,    1,    0,    1,    1,    4,    0,    2,    4,
    0,    3,    0,    2,    0,    2,    0,    9,   11,    0,
    0,    0,    0,    3,    2,    1,    0,    3,    5,    3,
    2,    5,    2,    1,    1,    1,    1,    1,    3,    1,
    3,    1,    0,    2,    1,    3,    1,    1,    1,    6,
    1,    0,    3,    0,    1,    3,    6,    3,    4,    2,
    3,    0,    1,    2,    1,    1,    1,    2,    1,    2,
    1,    2,    0,    3,    0,    1,    3,    1,    2,    0,
    2,    0,    2,    0,    4,    1,    2,    1,    1,    0,
    1,    3,    2,    1,    1,    2,    1,    4,    3,    1,
    3,    4,    0,    8,    5,    1,    1,    4,    4,    4,
    1,    1,    5,    5,    1,    3,    3,    1,    1,    1,
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
    1,    3,    1,    1,    1,    1,    1,    4,    1,    3,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    1,    2,    1,    1,    1,    1,    1,
    2,    1,    1,    1,    1,    1,    4,    5,    5,    5,
    5,    5,    5,    5,    5,    5,    5,    6,    1,    6,
    1,    4,    7,    2,    0,    4,    3,    6,    1,    1,
    6,    6,    1,    1,    4,    6,    4,    5,    3,    5,
    4,    5,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    0,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
  581,  582,  583,  587,  588,  589,    0,  644,  756,  757,
  761,  762,  489,    0,    0,    0,    0,    0,    0,  423,
  599,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,    0,    0,   96,    0,  936,    0,   22,  934,
    0,    0,   74,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  507,    0,    0,    0,
   38,   35,   37,    0,   36,    0, 1021, 1023, 1025, 1024,
 1030, 1026, 1033, 1031, 1027, 1029, 1032, 1028, 1034, 1035,
 1036, 1038, 1039, 1037,    0,    0,    0,    0,   33,   69,
   67, 1013, 1022,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  676,    0,    0,    0,
  713,    0, 1015,    0,    0,    0,    0,  108, 1020,  590,
    0,    2,    0,    0,    0,    0,  453,  454,  455,  456,
  457,  458,  459,  460,  461,  462,  463,  464,  465,  466,
  467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
  477,  478,    0,  451,  452,    0, 1005,    0,  430, 1009,
    0,  427, 1012,    0,  715,    0,  426, 1016,    0,  597,
  932,    0,  151,  104,    0,  105,    0,  219, 1001,  797,
    0,  103,  146, 1008,    0,    0,   99,    0,  106,  938,
    0,  100,    0,  101,    0,  102, 1019,  147,  107,  652,
  653,    0,  937,  935,    0,   75,    0, 1006,    0, 1004,
  515,  508,    0,  514,  519,  509,  510,  517,  511,  512,
  513,  518,    0,    0,    0,   31,    0,   40,  784,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  942,    0,  923,  670,  670,
  671,  681,  683,  994,  684,    0,  685,  675,  671,    0,
    0,    0,    0,    0,    0,    0,  616,  605,    0,  608,
  610,  611,  612,  613,  619,  620,    0, 1014,  714,    0,
  109,    0,  110,    0,  111,    0,    0,    0,  645,    0,
    0,    0,    0,    0,    0,    0,    0,  490,    0,    0,
  393,    0,  222,    0,  433,  424,  487,  488,    0,    0,
    0,    0,    0,    0,  442,    0,  399,  400,    0,  603,
  601,  933,    0,    0,    0,    0,    0,    0,    0,  154,
  133,    0,   97,    0,    0,  112,    0,  122,  121,    0,
    0,    0,    0,    0,    0,  997,   76,    0,    0,    0,
  758,  759,  516,    0,    0,    0,    0,  901,  929,    0,
  996,    0,  903,    0,    0,  930,    0,  924,  915,  914,
  913,    0,    0,    0,    0,  905,  906,  907,  916,  917,
  925,  926,    0,  927,  928,    0,    0,    0,  931,    0,
    0,    0,    0,    0,  919,    0,  794,  898,  899,  900,
  769,  904,  370,  884,    0,    0,  883,    0,  770,    0,
  880,  881,  882,  885,  886,  887,  918,  943,  944,  945,
  946,  959,  961,  969,  970,  973,  974,  220,    0,    0,
    0,  374,    0,    0,    0,  787,    0,    0,   28,    0,
    0,    0,   72,   70, 1017,   68,   34,    0,    0,    0,
  789,    0,   53,    0,    0,    0,    0,    0,  595,    0,
    0,    0,  678,  680,    0,  687,    0,    0,  688,    0,
  674,    0,  642,    0,  617,    0,  614,    0,  615,  640,
    0,  635,    0,    0,  609,  798,  765,    0,    0,    0,
    0,    0,    0,  592,  591,    0,    0,  649,  646,    0,
    0,    0,  187,    0,    0,    0,  130,  495,  174,  175,
    0,  498,  493,    0,  499,    0,    0,    0,    0,  500,
    0,  491,    0,    0,  437,  439,  438,    0,  440,    0,
  434,    0,    0,  273,    0,    0,    0,  446,  447,    0,
  480,    0,    0,    0,    0,    0,    0,    0,  404,  403,
  503,    0,    0,  150,    0,    0,  159,    0,  158,    0,
    0,  165,  148,    0,  163,  166,  167,  155,    0,    0,
  538,  573,  541,    0,  574,    0,    0,  542,    0,    0,
    0,  565,  577,  537,  560,  539,  540,  535,  522,  393,
  520,  521,  523,    0,    0,  532,  533,  534,  536,    0,
    0,    0,    0,    0,  113,    0,  114,    0,  125,    0,
    0,  203,    0,    0,  391,    0,  222,    0,    0,   84,
    0,    0,  779,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  800,  802,  811,  814,  815,  816,
  817,  818,  819,  820,  821,  822,  823,    0,    0,    0,
  920,    0,    0,    0,    0,    0,    0,  921,  922,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  335, 1018,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  278,  530,    0,    0,    0,  372,
    0,    0,    0,  786,   29,    0,    0,   45,   73,    0,
    0,   27,    0,  792,    0,    0,    0,  755,    0,    0,
    0,    0,    0,  594,    0,  670,    0,    0,    0,    0,
  692,    0,  585,    0,  940,  584,    0,  618,    0,  626,
  622,  621,    0,    0,    0,  637,  625,    0,    0,  763,
  764,    0,    0,    0,  391,    0,    0,    0,    0,    0,
  643,    0,    0,    0,  131,    0,  497,    0,    0,    0,
    0,    0,  393,  436,    0,  393,  394,  441,    0,    0,
  269,  391,  242, 1002,    0,    0,  393,    0,    0,  211,
  391,    0,    0,    0,    0,  247,  241,  244,  245,  246,
  445,  485,  484,  444,  443,    0,    0,    0,    0,    0,
    0,  402,    0,  393,    0,  156,  160,    0,    0,  164,
    0,    0,  561,  562,  578,    0,  575,    0,    0,  568,
  566,  567,    0,    0,    0,    0,  531,    0,    0,    0,
    0,  569,    0,    0,  391,    0,  202,    0,  205,  207,
  208,    0,    0,    0,    0,    0,  222,   79,    0,    0,
   78,  549,  998,  550,    0,    0,    0,    0,  801,  813,
    0,    0,  871,  872,  873,    0,  805,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  986,  987,  988,  989,  990,  991,  992,  993,    0,    0,
    0,    0,    0,    0,  979,    0,    0,    0,    0,  896,
  337,  897,  967,    0,    0,  892, 1000,    0,    0,    0,
    0,    0,  771,    0,  902,  796,  795,  376,  375,    0,
  276,    0,    0,    0,  284,  788,    0,    0,    0,    0,
    0,    0,    0,    0,   54,   62,   64,    0,    0,   71,
    0,    0,    0,  791,    0,    0,    0,    0,    0,    0,
    0,    0,  679,  682,    0,    0,    0,    0,  697,  698,
  699,    0,    0,  689, 1010,  691,  586,  941,  627,  636,
  631,  630,    0,  641,  799,  766,  767,  391,    0,    0,
  672,  651,  650,    0,    0,  662,  666,  183,  177,  184,
    0,  180,  496,    0,    0,  179,    0,  190,  171,  170,
  939,  173,  172,  501,    0,    0,    0,  435,  391,  391,
  281,  394,  274,    0,  213,  393,  215,    0,  394,  210,
  216,    0,    0,    0,    0,  448,  449,  393,  391,  394,
    0,    0,    0,  420,    0,  198,  199,  157,    0,  169,
    0,    0,    0,    0,    0,  576,    0,    0,  393,  394,
    0,  526,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  200,    0,    0,    0,    0,
  380,    0,   83,    0,    0,    0,    0,   88,   85,    0,
    0,    0,  760, 1003,    0,    0,  806,    0,    0,  812,
    0,    0,    0,    0,    0,  849,  848,    0,    0,    0,
  856,  874,  866,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  804,    0,    0,    0,  222,    0,  947,
    0,    0,  962,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  975,    0,  985,    0,    0,
    0,    0,    0,    0,  966,  908,    0,  271,  290,    0,
    0,  371,  285,   66,   58,   59,   65,   60,   61,    0,
    0,   23,    0,    0,    0,   43,    0,    0,    0,  773,
  793,    0,    0,  772,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  716,    0,    0,    0,    0,
    0,  633,  634,  638,    0,  201,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  191,    0,    0,    0,  431,
  225,  226,  227,  228,  229,  230,  911,    0,    0,  275,
    0,    0,  223,    0,    0,    0,    0,  248,    0,  481,
  482,  450,    0,    0,    0,    0,    0,    0,  393,  505,
  394,  168,  999,    0,    0,  544,    0,  543,  579,    0,
  571,    0,    0,    0,  524,    0,  552,    0,    0,  554,
  557,  559,  391,    0,    0,    0,  120,    0,  206,  393,
    0,    0,  392,    0,   98,    0,  118,    0,   94,    0,
    0,    0,    0,   87,    0,  864,  865,  808,    0,  810,
    0,    0,    0,    0,    0,    0,    0,  867,    0,    0,
    0,    0,    0,    0,    0,  857,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  953,  952,    0,    0,  949,  948,    0,  957,  956,  955,
  954,  951,  950,    0,    0,  981,    0,    0,    0,  980,
    0,    0,    0,    0,    0,  277,    0,    0,  287,  288,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  369,    0,    0,    0,    0,  296,  301,
  295,    0,    0,    0,  305,  306,  307,  308,  309,  310,
  312,  313,  315,  316,  317,  320,    0,    0,    0,  363,
  364,  365,    0,    0,   57,   63,   55,   56,   24,  777,
  778,    0,  775,  782,  783,  780,    0,  754,    0,    0,
    0,  708,    0,    0,    0,  722,  718,  720,    0,    0,
    0,    0,    0,  392,  655,  656,  673,    0,  185,  181,
    0,    0,  196,  192,    0,  394,  912,  279,  280,  394,
    0,    0,    0,  233,    0,    0,  249,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  502,  546,    0,  547,
    0,  135,    0,    0,    0,  139,  141,  142,  143,  527,
  529,  525,  556,    0,    0,  394,    0,    0,    0,    0,
  377,    0,  381,  393,   89,    0,    0,   91,    0,    0,
    0,    0,    0,    0,  876,  875,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  221,  968,  958,  960,    0,    0,
  977,  976,    0,  982,  971,  972,    0,    0,  286,    0,
  324,    0,    0,    0,  319,    0,    0,  318,  349,  350,
    0, 1011,  366,  367,    0,  304,  311,    0,  314,  297,
    0,  302,    0,  354,  303,  321,    0,    0,  347,   41,
    0,    0,    0,  711,  709,    0,    0,    0,  726,    0,
    0,  668,  394,  378,  657,    0,  176,  193,  394,  145,
  268,    0,    0,  209,  234,    0,    0,    0,    0,  235,
  238,    0,    0,  394,  418,  419,  415,  414,  417,  416,
    0,  545,  572,  144,  132,  140,    0,  393,  265,  127,
  394,    0,    0,  395,  382,    0,    0,   92,   93,   80,
    0,  835,  843,    0,  836,  844,  839,  847,    0,    0,
  837,  845,  838,  846,  832,  840,  833,  841,  834,  842,
    0,  963,  978,    0,    0,  293,  289,    0,  322,    0,
    0,    0,    0,    0,  351,    0,    0,  342,  343,    0,
  362,    0,    0,    0,    0,  357,  355,    0,    0,    0,
  774,    0,    0,  701,    0,    0,  736,  738,    0,  733,
    0,  266,  186,  432,  212,  217,  236,    0,    0,  239,
    0,  251,  253,  422,  394,    0,  397,    0,    0,    0,
  387,  388,    0,    0,   77,   95,    0,    0,  292,    0,
  323,    0,    0,  326,    0,  332,    0,  334,    0,    0,
  361,  360, 1007,  359,    0,    0,    0,    0,    0,    0,
    0,  700,    0,  727,  737,    0,  240,    0,    0,    0,
    0,    0,  267,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  345,  344,
  356,  358,    0,    0,    0,    0,  705,    0,    0,  744,
    0,    0,    0,    0,    0,    0,  250,    0,  257,    0,
  256,  398,  390,    0,    0,  383,  119,    0,    0,  294,
    0,    0,  325,  329,    0,    0,    0,    0,    0,    0,
    0,  703,  746,  735,    0,    0,  747,    0,  743,  237,
    0,    0,  261,  260,  259,    0,    0,    0,    0,    0,
    0,    0,    0,  346,    0,  706,  742,    0,    0,  264,
  262,  263,    0,    0,  878,  340,  330,    0,  352,  327,
  669,    0,    0,    0,  749,    0,    0,  328,    0,  748,
    0,    0,  751,    0,  877,  752,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1445,   28,   29, 1446,
   31,   32,   33,   34,   35,   36,   37, 1447,   39, 1448,
  663,  125,  478,  993, 1242,  126,  737,  994,  127,  482,
  742,  151,  128,  129,  268, 1016,  141,  995,  996,  206,
  235,  130,  997,  131,  484,   83,  246,  387,  434,  658,
 1136,  435,  890,  629,  228,  867,  659,  660,  661, 1138,
  247, 1480,   75,  196,   76,  242, 1922, 1355,  227,  232,
  234,  236,  222,  214,  216,  229,  239,  158,  321,  323,
  325,  376,  645,  647,  475,  269,  816,  564,  673, 1068,
  380,  650, 1124, 1347,  595,  765, 1098,  546,  547,  577,
  372,  630, 1110, 1544, 1664, 1290, 1545, 1546, 1547, 1548,
 1549,  674,  223,  215,  368,  603,  369,  370,  604,  605,
  606,  607, 1322, 1062,  549,  550, 1051, 1049,  551, 1056,
 1050, 1280, 1509, 1057, 1058, 1285,  382,  878,  879,  880,
  881,  818, 1079, 1522,  631,  820,  437,  883,  966,  469,
 1404, 1291,  438,  439,  440,  441,  442, 1523, 1524,  580,
 1670, 1820, 1308,  826,  827,  813,  828,  829,  830, 1821,
 1822, 1894,  350,  474,  983, 1449,  202,  573,  980,  574,
 1072,  470,  984,  985, 1230, 1428, 1429, 1430, 1727, 1143,
  748, 1450, 1451, 1452, 1453, 1633, 1454, 1455, 1456, 1457,
 1458, 1459, 1460, 1461, 1462, 1463, 1464, 1465, 1466, 1467,
  443, 1835, 1468, 1737, 1878, 1734,  444,  713,  445,  446,
 1469, 1634, 1745, 1746, 1802, 1470, 1471, 1472, 1625,  265,
  471,  472,  447, 1129, 1353, 1561, 1130, 1131,  296,  507,
 1693,  391, 1271, 1502, 1779, 1780, 1781, 1782, 1269,  359,
  591,  844, 1320,  592,  842,   80,   60,  354,  207,  166,
  337,  193,  344,  199,  807,  804,  345,  355,  834,  356,
  587, 1090,  194,  448,  338,  561,  593, 1321,   97,  205,
  251,  632,  633,  726, 1111,  727, 1112, 1113,  636,  637,
  638,  639, 1104,  894,  640,  641,  642,  860,  643,  857,
   41,   42,   43,   44,   45,   46,  160,  535,   62,  500,
  210,  361,  308,  309,  310,  311,  312,  313,  314,  315,
  316,  771,  521, 1033, 1274,  522,  523,  776,   47,  165,
  331,  542,  791,   48,  537,  538, 1042, 1507, 1278, 1655,
 1046,  145,  759, 1652,  503,  298,  146,  147,  297,  508,
  509,  449,  762, 1026, 1017, 1018, 1019, 1020, 1021, 1753,
 1812,  450, 1848, 1263, 1648, 1649, 1850, 1761, 1851, 1852,
 1853, 1889, 1923, 1925,  490,   49,   50,  392,   51,   52,
  529,  530, 1002, 1250, 1482, 1486,  491,  745,  220,  675,
  676,  908,  909,  677,  678,  679,  680,  681,  682,  683,
  684,  685,  686,  687, 1376, 1159, 1161, 1162,  965,  712,
  451,  452,  453,  454,  455,  456,  457,  244,  458,  459,
  460,  461,  462,  949,  463, 1600,  464,  465,  466,  467,
  703,  953, 1219, 1214,  704,  133,
};
static short yysindex[] = {                                   5676,
 2001, -181, 3516,   74, 1113,  101, 4142,  375, 8397,  -30,
   45,10219, -181, 1216,  733,10138, 1425,10138,  172,    0,
  425,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  942,    0,    0,    0,
    0,    0,    0,11369,10138,10138,10138,10138,10138,    0,
    0,  245, 1363,10138,10138,10138,  450,10138,  276,10138,
10138,    0,10138,10138,    0,  686,    0,  310,    0,    0,
  502,10138,    0,10138,10138,10138,  514,10138,10138,10138,
10138,  276,10138,10138,10138,10138,    0,10138,  842,  485,
    0,    0,    0,  951,    0,  951,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  524,  612,  201, 1056,    0,    0,
    0,    0,    0,10138,10138,10138,  724,  637,  761,  150,
 1679,  731,  100,  103,  552,  705,    0,10138,  914, 1201,
    0,  859,    0,10138,10138,10138,10138,    0,    0,    0,
10138,    0, 1019,   72,  786,  900,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  616,    0,    0, 1363,    0,  564,    0,    0,
 1305,    0,    0, 1442,    0,  596,    0,    0,  967,    0,
    0,  936,    0,    0, 1141,    0, 1147,    0,    0,    0,
 1363,    0,    0,    0, 1023, 1305,    0,  115,    0,    0,
 1132,    0, 1137,    0,  951,    0,    0,    0,    0,    0,
    0, 1146,    0,    0, 1035,    0, 1199,    0, 1095,    0,
    0,    0, 1035,    0,    0,    0,    0,    0,    0,    0,
    0,    0,10652,10138, 1182,    0,10138,    0,    0,    0,
11290, 1200, 7993,10138, 1097, 1351, 1656, 1421, 1481,11290,
 1358, 7993, 1429, 1443, 1335,    0,12326,    0,    0,    0,
    0,    0,    0,    0,    0, 9982,    0,    0,    0, 1406,
10138, 1501, 1423, 1093,10138, 1453,    0,    0, 1201,    0,
    0,    0,    0,    0,    0,    0,10138,    0,    0, 1305,
    0, 1132,    0,  951,    0, 1502,12326, 1479,    0,12326,
 1584,  139, 1387,  -23, 1430, 1055,  900,    0, 1647, 1353,
    0,10138,    0,  616,    0,    0,    0,    0,10138, 1182,
 6710, 1468, 7756, 1897,    0,11369,    0,    0, 1418,    0,
    0,    0, 1884, 1884, 1363, 1578, 1363, 1336, 1141,    0,
    0,13325,    0,10138,10138,    0, 1182,    0,    0, 1671,
  840, 1918,10138, 1699,10138,    0,    0, 1925,   87, 7079,
    0,    0,    0, 1935, 1939, 1941, 1522,    0,    0, 1945,
    0, 1956,    0, 1960, 1962,    0, 1967,    0,    0,    0,
    0,    0, 1986, 1528, 1557,    0,    0,    0,    0,    0,
    0,    0, 1987,    0,    0,10882, 2000, 2004,    0,12326,
12326,10080,10138, 2012,    0, 1427,    0,    0,    0,    0,
    0,    0,    0,    0, 2010, 1974,    0,    0,    0, 2029,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,13325, 2023,
  702,    0, 2064, 1848, 1545,    0,10138, 1677,    0,10138,
10138,  110,    0,    0,    0,    0,    0,10138,10138,  -42,
    0,10138,    0, 1794,10138, 2073,11290, 1717,    0,10138,
11130, 1160,    0,    0,12326,    0, 1805, 2085,    0, 1866,
    0,  145,    0,  867,    0,  487,    0,  487,    0,    0,
 2088,    0,  148, 1724,    0,    0,    0, 2097,   71, 2087,
 1182, 2116, 1896,    0,    0, 1893, 2126,    0,    0,  995,
 1732, 1721,    0,  149, 1824, 1838,    0,    0,    0,    0,
 1363,    0,    0, 1839,    0, 1884, 1884, 1884, 1884,    0,
 2139,    0, 1907, 1915,    0,    0,    0, 1899,    0,13325,
    0,13325, 1769,    0, 1933,10138,13070,    0,    0,  740,
    0,10138, 1014, 1442, 1804, 1764, 1733,    0,    0,    0,
    0,  582, 1809,    0, 1755,  276,    0, 1363,    0, 1934,
 1868,    0,    0, 1336,    0,    0,    0,    0,  258, 1771,
    0,    0,    0, 1781,    0, 1831, 2192,    0, 1888, 2198,
   34,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2148,  163,    0,    0,    0,    0,  116,
 2200, 2201, 2198, 1305,    0, 1927,    0, 1982,    0, 1363,
 1363,    0, 6710,  596,    0, 1867,    0, 1840, 2215,    0,
 9229, 4384,    0, 1876, 1886, 2229, 7685, 2230,    0,    0,
    0, 6979, 8175,   42,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  897,11210,  117,
    0,10138,12326,  926, 1087, 1302, 2684,    0,    0,12326,
 7915, 1427, 1821,  847,12326,11210, 1002, 1002, 1216, 1978,
 1820, 2232,    0,    0,11449,10138,12326,12326,12326,12326,
12326,11210,12326, 5727,    0,    0, 1997, 2212,10138,    0,
10138, 1979,10138,    0,    0,10971, 1841,    0,    0, 1815,
 7993,    0, 1964,    0, 1856, 1216, 2243,    0, 1964,10971,
 1972, 1973,10138,    0, 1978,    0, 2094, 4103, 1833,11210,
    0,10138,    0,  426,    0,    0,   17,    0, 1842,    0,
    0,    0,10138, 1152,10138,    0,    0,10138,10138,    0,
    0,11210, 2027, 6710,    0, 1795,12326,12326, 1914, 1793,
    0, 1363, 1363, 1363,    0,  -20,    0, 1755, 1755, 1844,
  276, 1055,    0,    0, 2259,    0,    0,    0,  126,10138,
    0,    0,    0,    0, 2040, 2263,    0, 2015, 2033,    0,
    0, 1976, 1977,  951, 2048,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  276,10138,13244, 1942, 1943,
 1944,    0,  276,    0, 1445,    0,    0, 1913, 1363,    0,
  111, 1928,    0,    0,    0,  276,    0, 2192,  135,    0,
    0,    0, 2039,  135, 1929,  135,    0, 2303,  276,  276,
  276,    0, 1182,10138,    0, 1989,    0, 1883,    0,    0,
    0, 1418, 1931,10138, 1889,  143,    0,    0, 2306, 2076,
    0,    0,    0,    0,   87,10138, 1936, 8163,    0,    0,
 1936, 7685,    0,    0,    0, 7638,    0,  401,  155,12326,
12326, 8263, 2312,  782, 8524,12326, 8782,  720, 8880, 9138,
11688, 9396, 9494, 9752, 7915, 7915,12326,12326, 2092,12326,
 2316,12326, 2325, 2319,12326,12326,12326,12326,12326,12326,
    0,    0,    0,    0,    0,    0,    0,    0, 2043, 1407,
  -28,12326,  902,11210,    0, 7915, 1869, 2326,  552,    0,
    0,    0,    0, 1427, 1932,    0,    0, 2093, 1002, 1002,
 1348, 1348,    0,  704,    0,    0,    0,    0,    0, 1949,
    0, 1926, 2105, 1979,    0,    0,10138,10138,10138,10138,
10138,10138,  152, 2337,    0,    0,    0,10971, 1999,    0,
  370, 2019,10138,    0,  552,11210, 2019, 2341, 2337,10971,
 8612, 2069,    0,    0, 3706, 2351, 2348,  976,    0,    0,
    0, 7915, 2070,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  335,    0,    0,    0,    0,    0, 1953, 1931,
    0,    0,    0, 1427,10138,    0,    0,    0,    0,    0,
 1965,    0,    0, 1884, 2134,    0,  -20,    0,    0,    0,
    0,    0,    0,    0, 2362, 7915, 7275,    0,    0,    0,
    0,    0,    0, 1979,    0,    0,    0, 7275,    0,    0,
    0,  951,  951, 1904, 2368,    0,    0,    0,    0,    0,
  501,  -93,  946,    0, 2146,    0,    0,    0,10138,    0,
   74, 1970, 1993, 1997, 2370,    0, 1755, 1971,    0,    0,
 1022,    0, 2354,10138, 1080,  276, 2375, 2377, 2380, 2160,
  596, 1979, 1884, 1838, 6710,    0, 1809,  552, 1983, 1988,
    0, 2722,    0,  276, 2037, 2123, 9229,    0,    0,  276,
 1946, 2133,    0,    0, 2403, 2404,    0, 7915, 7915,    0,
 7915, 7915, 2167, 1427, 2406,    0,    0, 1427, 2408,11768,
    0,    0,    0, 2099, 2415, 1427, 2418, 1211, 2421, 1427,
 2425,12326,12326, 2312,12326,12007, 2426, 1427, 2427, 2428,
 1427, 2429,12326, 1427, 2431, 1427, 2432, 2433, 1427, 2434,
 2435, 1427, 2437,    0, 2216, 2352, 2475,    0, 2484,    0,
 2579,12326,    0, 2613, 2639, 2647, 2703, 2864, 3132,12326,
12326,11210, 1427, 2044,11210,    0,12326,    0, 2172,  555,
12326,11210,12326,12326,    0,    0,10138,    0,    0,10138,
10786,    0,    0,    0,    0,    0,    0,    0,    0, 2154,
10971,    0,10971,  152, 2337,    0,  243,  243,  206,    0,
    0, 9982, 1990,    0, 2341, 2337, 2440, 8612, 2444,  976,
 2445,11210, 1866, 4103, 2100,    0, 2101, 2102, 2157, 2228,
 2173,    0,    0,    0, 1979,    0, 1983,  892, 2461, 2086,
 1363, 2124, 1755, 2122, 1755,    0, 7915,  406, 1803,    0,
    0,    0,    0,    0,    0,    0,    0, 7275, 7275,    0,
 2105,12326,    0, 2236, 2108, 1904,  686,    0, 7915,    0,
    0,    0, 2138, 2147, 2149, 2152, 2159, 2174,    0,    0,
    0,    0,    0, 2478,   74,    0, 1844,    0,    0,  135,
    0, 7275, 2156,  135,    0,  135,    0, 1997, 2499,    0,
    0,    0,    0, 1418, 2105, 1755,    0, 1838,    0,    0,
 9982, 2275,    0,  424,    0, 2461,    0, 2521,    0, 2524,
 1363,  -65, 2525,    0, 1363,    0,    0,    0, 2216,    0,
 2216,12326, 2145, 2145, 1216, 2526, 1991,    0, 2145, 2145,
12326, 2145, 2145, 2179, 1427,    0, 1835,12326, 1427, 2145,
 2145, 2145, 2145, 1427, 2145, 2145, 2145, 2145, 2145, 2145,
    0,    0,13325, 2529,    0,    0, 3255,    0,    0,    0,
    0,    0,    0, 3598, 1681,    0,11210, 2265, 2140,    0,
11210, 3612, 1998, 1605, 1427,    0,13325, 2513,    0,    0,
 2284,    0, 6113,  353, 2520, 2540, 2241,12326, 2523, 2527,
 6444,10138,10138,    0, 2531, 2532, 2253, 2536,    0,    0,
    0, 2288,10454, 2539,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2543,  -16, 2545,    0,
    0,    0,    0, 2220,    0,    0,    0,    0,    0,    0,
    0, 2171,    0,    0,    0,    0, 1805,    0, 8612, 2440,
 1866,    0, 2007,10138,  976,    0,    0,    0, 4103,12326,
 7915, 2214, 2105,    0,    0,    0,    0, 1884,    0,    0,
 1363, 1445,    0,    0,  480,    0,    0,    0,    0,    0,
 1427, 2328, 2236,    0, 1180,10138,    0, 2274,  614, 2170,
 2280, 2177, 2322, 2289, 2327, 1979,    0,    0, 2572,    0,
 2588,    0, 2278, 2328, 2156,    0,    0,    0,    0,    0,
    0,    0,    0, 1979, 1809,    0, 1445, 2146, 2313, 2249,
    0, 1931,    0,    0,    0, 2267,   18,    0, 1997, 2268,
 1427, 2595, 2600,  552,    0,    0, 2602, 2604, 1427, 2607,
 2609,12326,12326, 1427, 2610, 2616, 2618, 2620, 2622, 2624,
 2625, 2626, 2627, 2628,    0,    0,    0,    0,12326, 2632,
    0,    0,11210,    0,    0,    0, 1833,  141,    0, 2356,
    0,12246,10138,12326,    0, 7915, 1427,    0,    0,    0,
 2617,    0,    0,    0,10138,    0,    0, 5787,    0,    0,
  691,    0, 2227,    0,    0,    0,  567, 2643,    0,    0,
  243, 1833,10138,    0,    0, 1181, 1427, 2641,    0,   42,
  546,    0,    0,    0,    0, 1755,    0,    0,    0,    0,
    0, 2646,10138,    0,    0, 2339, 2349,10138, 1904,    0,
    0,  951,10138,    0,    0,    0,    0,    0,    0,    0,
 2105,    0,    0,    0,    0,    0, 2105,    0,    0,    0,
    0, 4623, 1833,    0,    0,12326, 1363,    0,    0,    0,
 1363,    0,    0,12326,    0,    0,    0,    0, 1427, 1427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1427,    0,    0, 2070, 7275,    0,    0, 2655,    0, 2137,
10652, 1318,  664, 2637,    0, 2358, 1497,    0,    0, 2653,
    0,10138,10138,   74,  142,    0,    0, 2194, 2360, 7915,
    0, 2070, 2663,    0, 7915,12326,    0,    0, 2336,    0,
 2667,    0,    0,    0,    0,    0,    0, 1904,  951,    0,
 2334,    0,    0,    0,    0, 2146,    0, 4623, 2664,  976,
    0,    0, 2070, 1427,    0,    0, 1605, 2173,    0, 1936,
    0, 2300, 5787,    0, 2286,    0, 5787,    0, 5787, 5265,
    0,    0,    0,    0,10786,  691,12326, 5787,  695, 2173,
10138,    0,   42,    0,    0, 9592,    0, 2334,  347, 1904,
 2340, 2342,    0,    0,  976, 2678, 4623, 2381, 2173, 2685,
 1833, 2214, 2686, 3097, 2670, 1563,10786, 2688,    0,    0,
    0,    0,  565,  159, 2430, 2214,    0, 2018,10138,    0,
 2692, 2693, 1464, 1904, 1148, 1148,    0, 2438,    0, 2299,
    0,    0,    0,  976, 4623,    0,    0, 2070, 2364,    0,
 5787, 2688,    0,    0, 2436, 5787, 2454, 2449,10786, 2364,
10138,    0,    0,    0, 9592, 2719,    0,10138,    0,    0,
 2304,  760,    0,    0,    0, 1260, 2173,  786, 2036,10786,
  178,10138,10786,    0,  786,    0,    0,10138, 2439,    0,
    0,    0, 7915, 2214,    0,    0,    0,10786,    0,    0,
    0, 2728, 2725, 2733,    0,   42, 2364,    0,10138,    0,
10138,  786,    0, 2734,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  240,  811,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  266, 7340,    0,    0,    0,    0,    0,    0,
 2777,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   94,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1012,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2442,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   96, 2397,
    0,    0,    0,  114,    0,  114,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2399,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1028,    0,    0,10319, 7599,    0,    0,    0, 1092,
    0,    0,    0, 2257,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  465,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  588,    0,    0,    0,    0, 1074,    0,    0, 1131,    0,
    0,    0,    0,    0, 1263,    0,13334,    0,    0,    0,
    0,    0,    0,    0,    0,  588,    0,  323,    0,    0,
 2744,    0,    0,    0, 1896,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1215,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1267,    0, 2522,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1144,    0,    0,    0,
    0,    0,    0,    0,10722,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  891,    0,    0, 1320,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  588,
    0, 2744,    0, 1896,    0, 1340,    0,    0,    0,    0,
  454,    0,    0,    0,    0,    0, 1378,    0, 2511,    0,
    0,    0,    0, 1392,    0,    0,    0,    0,    0, 2522,
  459,11927,    0, 1408,    0,    0,    0,    0,  289,    0,
    0,    0, 2338, 2338,    0,    0,    0, 1447, 1486,    0,
    0,    0,    0,    0,    0,    0, 2522,    0,    0, 1467,
    0,    0,    0,    0,    0,    0,    0, 9429, 2405,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2047,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2756,    0,    0,    0, 3060, 3364,    0,    0,
    0,    0,    0,    0,    0,  904,    0,    0,    0,    0,
    0,    0,    0,    0,  182, 3668,    0, 2450,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1493,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1551,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2485,    0, 3753,
    0,    0,    0,    0,    0,  901,    0,  901,    0,    0,
 1380,    0, 1718, 1739,    0,    0,    0,    0, 1215,    0,
 2522,    0,    0,    0,    0,  160,  305,    0,    0,  647,
    0,  416,    0, 1770,    0,  822,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2338, 2338, 2338, 2338,    0,
  868,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2511,
    0,    0, 1383,    0, 2343,    0,    0,  534,    0,    0,
    0,    0,  188,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1570,    0,    0,    0,    0, 5038,12833,
    0,    0,    0,12845,    0,    0,12857,    0,    0,12910,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 3484, 3998,    0,    0,    0,    0,12922,
 9755,    0,12910,  588,    0,    0,    0,    0,    0,    0,
    0,    0,  459, 1436,    0,    0,    0,    0, 2384,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 6221, 6348,
 6652,    0,    0,  231,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,12565,    0,12565,
    0,    0,    0,12565,12565,12565,    0,    0,    0,    0,
    0, 2353,    0,    0,    0,    0, 4708, 5012,10566, 2762,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1220,    0,    0,    0,
    0, 2537,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  331,    0,    0, 7340,    0,    0,  331,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  854,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  638,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  459,    0,  226,    0,    0,  819,    0,
    0,    0,    0,    0,    0, 1919,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2041,    0,
    0,    0,    0,    0, 2767,    0,    0, 5182, 4091,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  493,  659,
  934,    0,    0,    0, 1219,    0,    0,    0,    0,    0,
    0, 9048,    0,    0,    0,    0,    0,12857,    0,    0,
    0,    0,  959,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2522,    0,    0,  293,    0,    0,    0,    0,
    0,    0,    0,    0,    0,11623,    0,    0,    0,  280,
    0,    0,    0,    0, 2502,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,12565,    0,
    0,    0,    0, 1753,    0,    0,    0, 3972, 5228, 5532,
 4188, 4492,    0,    0,    0,    0,    0,    0,    0,    0,
    0,11835,    0, 2548,    0,    0,    0,    0,    0,    0,
    0,    0, 1573, 1590,    0,    0,    0,    0,    0,    0,
    0, 1596,    0,    0,10319,    0, 1596, 1608, 1617,    0,
    0,    0,    0,    0,    0, 5612,  575,  492,    0,    0,
    0,    0,  896,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  692,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2338, 2363,    0, 1923,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2537,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1136,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1635,    0,    0,    0,    0,    0,
    0,    0,    0, 9902,    0,    0,    0,    0,    0,    0,
    0,    0, 1173,    0,    0,    0,    0,    0,    0,    0,
 1436, 2537, 2338, 1685,  459,    0, 2550,10319, 1702,  185,
    0,    0,    0,    0,11623,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  581,    0,    0,    0,  917,    0,    0,
    0,    0,    0,    0,    0, 1125,    0, 1670,    0, 1715,
    0,    0,    0,    0,    0,    0,    0, 2266,    0,    0,
 2530,    0,    0, 2964,    0, 3140,    0,    0, 3474,    0,
    0, 4396,    0,    0, 5376,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2385,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  739,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1573, 1713,    0, 2386,  337,    0,    0,
    0,    0,    0,    0, 1719, 1720, 1730,    0,    0, 2480,
 1441,    0, 6015,    0, 2481,    0, 2482, 2488,    0,    0,
  198,    0,    0,    0, 2537,    0, 1702,    0,  851, 1487,
    0,    0,    0, 2369,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2935,    0, 1136, 2442,    0,    0,    0,
    0,    0, 1005, 1025, 1026, 1078, 1103, 1106,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1068,    0,    0,    0,    0, 4662,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1731,    0,    0,
    0,    0,    0,    0,    0, 1746,    0,    0,    0,    0,
    0,  136,    0,    0,    0,    0,    0,    0,  128,    0,
  134,    0,    0,    0,10566,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 4916,    0, 5365,    0, 6038,    0,
    0,    0,    0, 6125,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2791,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1767,    0,    0,    0,    0,    0,
    0,12181,    0,    0,    0,    0,    0,    0,    0, 1837,
 1846,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  835,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1860,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1763,
 5818,    0,    0,    0, 1187,    0,    0,    0,    0,    0,
    0,  721,    0,    0,    0,    0,    0, 2338,    0,    0,
    0, 1716,    0,    0,    0,    0,    0,    0,    0,    0,
 2793, 1509, 3693,    0, 2511,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2537,    0,    0,    0,    0,
    0,    0,    0, 1773, 1088,    0,    0,    0,    0,    0,
    0,    0,    0, 2537, 2550,    0,  380,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  805,    0,
 6420,    0,    0,12565,    0,    0,    0,    0, 6454,    0,
    0,    0,    0, 6715,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   63, 2776,    0,    0,
    0,    0,    0,    0,    0,    0, 2779,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2533,    0,    0,    0,    0,    0,    0,    0,
  337,  191, 3662,    0,    0, 2480,  635, 1070,    0,  498,
 2802,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 5202,    0,
    0,  253,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  611,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 6795, 6818,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2803,    0,    0,   80,    0,    0,    0,    0,    0,    0,
  236,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  327, 2672,    0,    0,    0,    0,    0, 2805,    0,
    0,    0,    0,    0,    0,    0,    0, 5202, 2264,    0,
  176,    0,    0,    0,    0,    0,    0,    0,  341, 1145,
    0,    0,  120, 2793,    0,    0,    0,  130,    0,    0,
    0, 2784,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   49,    0,    0,    0,    0,  313,
    0,    0, 3966,    0,    0, 2802,    0, 1630,    0, 1136,
 2176, 2462,    0,    0, 2480, 1488,    0,    0,  242,    0,
   63,   62,    0,    0,    0,    0,   49, 2794,    0,    0,
    0,    0,    0, 2546,    0,  218,    0,    0, 1484,    0,
    0, 2811,    0, 5202,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1226,    0,    0,    0,   80,   75,    0,
    0, 2795,    0,    0,12947,    0,    0,    0,   49,   92,
    0,    0,    0,    0, 2802,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2480,  130, 2814,    0,   49,
    0,    0,   49,    0, 1601,    0,    0,    0, 2059,    0,
    0,    0,    0,   62,    0,    0,    0,   49,    0,    0,
    0, 2815,    0,    0,    0, 3054,   75,    0,    0,    0,
    0, 2814,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2857,    0,    0, 2866,
    0,    0,    0,    0,    0,    0,    0,    8,    0, 2867,
   -2,  272,    2, -660, 1624,  295, -103, -525, 2859, 2590,
    0,  -44,    0, 2599, -101,   10,    0,  540, -962,  -40,
  -62,  -45, -229, 2601, 2135,    0,    0,    0, 1452,    0,
    0,  -22, 1740, -533,  -71, -981, 2221, 1994, -321, 1745,
 2796, -293,    0,  -61,    0, -496,  -55,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -342,  -86, 1750, -302, 1032, -966,
    0,    0,    0,    0, -344, -133, -527, 1759, -341,  -52,
    0, -540,    0,    0, 1343,-1012,    0, 1346,    0,    0,
 2549, -688,  -31,    0,    0,    0,    0, 2528,    0, 2290,
 2560, -225, 1782, -780,    0,    0,    0, -751,    0,    0,
    0,    0,    0,    0, 1843,    0, 2573, 2115, 1783, 2561,
 2563,    0, 1307,    0, -441, 2103,  -34, -561, 1255, -231,
    0,    0, -937, -929, -917, -910, -900,    0, 1397,-1104,
    0, 1105,-1234, 1396,    0, 2346,    0,    0,    0, 1107,
 1104, 1075, -104, -240, -912, -914,    0,    0,    0, 2120,
    0, -632,    0, 1950,    0,    0,    0,    0,    0,-1167,
 -865, -796,    0,    0,    0,    0, -153,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1202,    0,    0,   55,    0,    0,  525,    0, -389,  543,
 1495, 1304,    0, 1135,    0,    0,    0,    0,    0,    0,
    0, 2209, -128, 1905, 1667, 1444,    0,-1148, -907, -943,
    0, -510, -324, -456,    0, -909, 1168,  390, -515, -549,
 -712, -985,-1347,    0,    0,    1,    0,    0,    0,    0,
    0, 2591,    0,    0,    0,    0, 2612, 2373,    0,    0,
    0,    0,    0,   -9, 2631,    0,    0,    0,    0,  -10,
    0,    0,    0, -349, 2107, -348, 1620, -474,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2331,    0, 2111,
    0,    0,    0,    0,    0,    0,   11,    0, 2962,    0,
    0,    0,    0,    0, 2668,    0,    0,    0,    0, 2464,
    0, 2463,    0,    0,    0, 2207,    0,    0,    0, -553,
  -17,    0,    0,   43,    0, 2199,    0,    0,    0,    0,
    0, -649,-1231, -602, -176, 1566,    0, 2839, 1373,    0,
 2231, -279,-1066,-1488,    0, -908, 1980,    0,    0,    0,
    0, -136,    0,    0,    0, 1231, 1338,    0, 1111,    0,
 1143,    0, -430,    0,    0,    0,    0, 2468,    0,    0,
    0, -421, 2250, 1995, -968,    0,    0,    0, -690, -472,
  175,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2452, 1242, 1827,    0, -614,    0,
    0,    0, -894,    0,    0,    0, 1709,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -886, 1787,    0,    0,
};
#define YYTABLESIZE 13831
static short yytable[] = {                                     132,
  198,   79,  132,  221,  270,  154,  153,   38,  159,  289,
  292,  217,  951,  204,  290,  293,  510,  319,  780,  596,
  258,  570,  634,  635,  252,  231,  602,  725,  238,  218,
  233, 1145,  261,  977,  256, 1146,  817,  766,  568,  786,
  213, 1052,  711,  483,  195,  197,  200,  203,  153,  208,
  262, 1223,  483,  260,  219,  197,  224,  255,  203,  959,
  153,  208, 1431,  237,  132, 1303,  201,  744, 1232,  230,
 1102, 1527,  248,  154,  153,  250,  197,  226,  248,  224,
  200,  203,  259,  153,  208,  237,  132, 1035,  203, 1008,
  277,  648,  230,  648,  324,  664, 1005, 1252,  981,  276,
  257,  278,  734,  732,  882, 1300, 1260,  263,  974,  575,
  560,  322, 1304,  504,  779,  648,  300,  572,   78,  725,
  725,  377, 1328, 1312,  219,  153,  219,  891,  808, 1292,
  809,   78,  648,  384,  648,  819,  648, 1293,  224,  287,
 1292, 1350,  291, 1333,  318,  203,  153,  237, 1293, 1294,
  648,  159,  648,  741, 1101,  869, 1295,  785,  931,  654,
 1294, 1301,  288,  527,  320,  288, 1296, 1295,  807, 1127,
  730,  326, 1297,  346,  809,  258,   90, 1296,  725, 1107,
  528,  336, 1135, 1297,  379, 1806, 1070,  421,  793,  764,
  732,  775, 1607,  274,  899, 1241, 1494,  730,  373, 1525,
  654, 1725, 1799,  654,  795, 1563,  329, 1315, 1567, 1345,
 1691,  798,  799,  800,  801,  531,  258,  734,  654,  258,
 1351, 1799,  533, 1040, 1009,  658,  479,  634,  635,  468,
  731,  732,  476,  925,  258,  479, 1194, 1195,  730,  600,
  336,  730,  134,  379,  274,  513,  421, 1071, 1023,  732,
 1074,  739,  785,  866,  219, 1642,  730,  219,  734, 1081,
  520,  153,  602,  485,  132,  600,  658, 1220,  288,  658,
  153,  731,  485,   61, 1623, 1624,  734,  825, 1476, 1483,
 1478,  494,  526,  138,  658, 1518, 1519,   78,  504,  731,
  783,  200,  126,  785,  338,  153,  785,  851,  600,  516,
  730, 1637,   78,  925,  647,  288,  139,  569, 1487,  135,
  861,  785,  730, 1122,  468,  852,  218,  136,  583, 1542,
   86,  585, 1847,   86,  600, 1416,  725,  863, 1418,  656,
  776,  328,  219,  553,  646, 1054,  551, 1244,  654,  219,
  384,  219,  597,  219,  599,  647,  588,  504, 1316, 1255,
 1257,  126,  528,  730, 1537, 1495, 1552,  926,  652,  373,
 1292, 1292, 1503,  647,  203,  208,  348,  725, 1293, 1293,
  539,  730, 1698,  153,  746,  200,  294, 1288,  862,  197,
 1294, 1294,  378,  644, 1108,  725, 1520, 1295, 1295,  776,
  732,  751, 1906,  732, 1292,  551, 1088, 1296, 1296,  384,
 1568,  747, 1293, 1297, 1297,  152, 1212, 1559, 1055,  929,
  725,  648,  283, 1695, 1294,  667, 1151,  792, 1525,  930,
   84, 1295, 1877,  714, 1643, 1069,  958,  926, 1824, 1147,
 1556, 1296,  735,  648, 1770,  284, 1638, 1297,  197,  865,
 1078, 1150,  973,  743,   90, 1805, 1516,  749,  734,  732,
  725,  421,  479,  663,  526,  518,  667, 1736,  732, 1368,
 1369,  845, 1370, 1371,  661,  774,  725,  153, 1484,  282,
  203,  485, 1245,  249,  667,  725, 1275,  654,  153,  379,
  510, 1918,  153,  162, 1256,  203,  543,  153,   14,  738,
  159,  695,  407,  554,  663,  544,  785,  729,  752,  654,
 1065,  348, 1037, 1067,  738,  661,  373, 1298, 1299, 1699,
  754,  483,  663,  807, 1077,  730,  730,  662,  732,  809,
 1659,  732,  390,  661,  846,  730, 1476, 1311,  796, 1510,
 1601,   77,  695, 1817, 1604,  695, 1134,  730,  729,  873,
 1152, 1095,  821,  658,   77, 1377, 1540,  648,  731, 1660,
  695,  407,  504, 1661, 1662, 1831,  729,  286,  731,  725,
  286,  731,  868,  740, 1137,  658,  814,  219, 1061, 1688,
  731, 1344,  814,  725,  694,  847, 1064,  732,  572, 1013,
  858, 1251,  734,  732,  730, 1857,  732, 1700, 1653, 1689,
 1646,  336,  286,  730, 1426,  648,  230, 1490, 1515,  978,
  725, 1142,  286,  891,  734, 1240,  720,  718,  123,  719,
  732,  721,  648,  654,  648,  694, 1253,  731,  218, 1890,
 1529,  858,  647, 1681,  858, 1036,  731,  876,  877,  763,
 1028, 1555, 1120,  694,  728,  785,  273,  730,  379,  858,
  126, 1687,  528,  219,  647,  338,  659, 1855,  600, 1012,
  730,  730,  893,  209, 1674,  545, 1632,  725,  405,  892,
  933,  654, 1148, 1061, 1059, 1060, 1704,  925,  384,  732,
  731,  384, 1751,  662, 1218,  728,  258,  161,  728,  658,
  654,  730,  224, 1562,  731,  504, 1762,  659,  662, 1247,
   86,  660, 1764,  728,  468,  730,  468,  776,  986,  730,
   77,  600, 1279,  551, 1795,  659,  967, 1773,  730, 1283,
  225,  732, 1789,  725,  219,   77, 1723,  405,  730,  219,
  734,  219,  725,  219, 1777, 1574,  485,  197,  520, 1063,
 1034,  485,  660,   77, 1919, 1845,  243, 1084,  734, 1272,
  485,  925,  730,  203,  526,  729,  658, 1224,  203,  218,
  660,  731, 1025,  785, 1613,  667,  407, 1234,  647, 1657,
 1237,  734,  738,  153, 1086,  153, 1774,  243,  219, 1048,
 1048, 1053, 1775, 1302,  219,  468,   98,  243, 1346,  734,
  384, 1554,  661,  123,  123, 1310, 1149, 1292, 1856, 1356,
 1273,  926,  384,  663,  384, 1293, 1226, 1117, 1118, 1119,
  219,  230, 1087, 1089,  661, 1248, 1332, 1294, 1823,  695,
 1528,  294, 1650, 1137, 1295,  729,  925, 1830,  665,  695,
  347,  492,  695,  245, 1296,  647, 1100,  219,  219, 1121,
 1297,  695, 1423,  730,  723,  253,  230,  729, 1313, 1132,
 1128,  900,  858, 1094,  716,  553,  907,  725,  553,  717,
  664,  270,  357,  732,  295, 1541, 1105, 1862,  243,  665,
 1614, 1551, 1595,  769,  153,  926,  528,  494, 1825,  230,
  230,  230, 1493,  339,  153,  925, 1403,  665,  695, 1748,
  492,  264,  839,  286,  729,  266, 1144,  695, 1757,  407,
  623,  664,  694,  729,  732,  725,   99,  348,  858,  243,
  628,  858,  694,  768, 1876,  694,  271,  663,  858,  664,
 1027,  858,  732, 1758,  694,  858,  827, 1864,  661,  840,
  858,  340,  405,  858, 1833,  925,  494, 1733, 1238,  358,
 1001,  695, 1218,  406,  770, 1218,  725,  729,  732,  858,
  926,  732, 1314,  695,  768,  695, 1239,  768, 1236,  623,
  240,  729,  728, 1741,  725, 1896,  925,  827,  136,  628,
  827,  694,  768,  858,  659,  728,  858,  858,  526,  452,
  694, 1759,  510, 1028,  728,  827,  858,  485,  203,  208,
  485,  237,  132,   14, 1658, 1608,  659,  241,  485, 1421,
  267, 1172,  406,  858,  272, 1305, 1306, 1235,  270,  926,
  485,  485, 1742, 1001,  412,  203,  795, 1173, 1841,  660,
  476,  602,  695, 1743,  694,  858, 1536,  136,  729,  280,
  858,  728, 1259,  841,  413,  409,  694,  598,  694, 1690,
  728,  660,  858,  341,  858,  219,  720,  718,  734,  719,
 1875,  721,  632,  720, 1339, 1357,  300, 1558,  721,  926,
  732,  342, 1174,  634,  635,  405,  348,  148,  822, 1911,
  734, 1809, 1359,  412,  732, 1334, 1813,  137, 1363,  724,
  602,  510,  900,  401,  728,  116,  900,  408,  553,  492,
  926, 1175, 1904,  413,  409,  343,  598,  138,  728, 1323,
  218,  607,  492,  632,  528,  694, 1724,  476, 1324,  275,
  659,  858,  411, 1917, 1323,  410, 1920,  734,  279,  977,
  724, 1912,  116,  553, 1335,  219,  230, 1744, 1481, 1481,
 1485, 1928,  219, 1334,  828,  494,  137,  492,  724,  492,
  604, 1752,  401, 1163,  230,  252,  408, 1218,  494,  823,
  230, 1218,  298,  281,  385,  660,  138,  116, 1176, 1512,
  607, 1514,  348,  954,  955,  728,  294,  332,  665, 1164,
  734,  411,  149, 1656,  410,  828,  285,  768,  828,  150,
  333,  732, 1338,  494,  734,  494,  252, 1427,  827,  252,
  824,  732, 1783,  828,  732,  294,  696,  651,  385,  604,
  664,  348,  468,  732,  252,  218,  526,  406,  144,  927,
  756,  720,  718,  385,  719,  334,  721,  335, 1215, 1216,
  768,  768, 1557,  725,  732,  553,  528,  219,  197,  553,
 1432, 1473,  825,  768, 1926,  386,  725,  696,  935, 1869,
  696,  485,  623,  485,  827,  725,  136,  827,  136,  492,
  732,  734,  628, 1880,  827,  696, 1317,  827,  485,  732,
  301,  827,  720,  718,  203,  719,  827,  721, 1048,  827,
  553, 1696,  152,  553, 1828,  528,  338,  623,  412,  386,
  553,  767,  665,  732,  716,  827,  317,  197,  553,  717,
  553,  716,  725, 1318,  386,  494,  717,  348,  413,  409,
  306,  725,  327,  732,  436,  623, 1265,  956,  623,  827,
  211,  212,  827,  827,  664,  628,  330,  732,  628, 1828,
 1266, 1927,  827,  768,  623, 1898,  528,  336,  502,  606,
 1868,  152, 1539, 1218,  628,  338, 1905,  436,  163,  827,
  406, 1832,  401,  556, 1267,  725,  136,  401, 1566,  593,
  623,  408, 1570,  401,  349,  393,  294,  137, 1828,  725,
  628,  827, 1217, 1846,  768,  768,  827, 1645,  536,  720,
  718,  540,  719,  768,  721,  393,  411,  138,  827,  410,
  827, 1612, 1866, 1932,  732,  516, 1794,  428,  606,  629,
 1828,  164,  486,  557, 1505, 1776,  828,  724, 1268,  937,
  360,  429,  768,  768, 1506,  768,  362,  101,  593, 1788,
  724,  412,  768,  768,  768,  768,  768,  425,  768,  724,
  371,  768,  768,  768,  768,  768,  725,  558,  526,  768,
  768,  413,  409,  197,  374,  768,  486, 1810,   81,   82,
  788, 1622, 1144, 1144,  103,  559,  428,  827,  629,  716,
 1914,  486,  828, 1473,  717,  828,  161,  381,  720,  718,
  429,  719,  828,  721,  383,  828,  724,  702, 1829,  828,
  375,  707,  708,  710,  828,  724,  425,  828,  720,  718,
  401,  719,  385,  721,  408,  385,  517,  553,  832,  485,
  833, 1672,  385,  828,  318,  153,  182,  723, 1048,  203,
  716, 1740,   44,  553,  386,  717,  363,  197, 1933,  411,
 1934, 1265,  410,  104,  696,  161, 1754,  828,  218,  724,
  828,  828, 1481,  105,  696, 1266,  153,  696,  197,  553,
  828, 1381, 1763,  724, 1891,  553,  696,  182,  553,  723,
  182,  518,  755,  364,  389,  365,  757,  828,  106, 1267,
 1799,  152,  302, 1897,  153,  182,  390,  197,  519,  218,
   52,   44,  218,  386, 1031, 1798,  386, 1666,  366,  828,
  790,  294,  152, 1755,  828, 1892,  197,  218,  723,  162,
 1771, 1032,   42,  696,  197,  197,  828,  303,  828, 1667,
 1265,  197,  696,  367,  385,  734, 1668,  790,  733,   25,
  724,  152,  473, 1268, 1266,  781,  385,  716,  385,  197,
  661,  480,  717,  203,  939,  304, 1799,   48,  305,   52,
  152,  197, 1893,  197,  556, 1144,   50, 1772, 1267,  152,
 1669, 1874, 1731, 1769,  306,  152,  696,  716,  162,  258,
  553,   42,  717,  318,  506,  600,  197,  553,  696,  566,
  696,  661, 1913,  152, 1749,  828,  720,  718,   25,  719,
  307,  721,  567,  967,  781,  553, 1740, 1793,  153,  661,
 1740,  197, 1740,  200,  557,  386,   48, 1818,  528,  852,
  258, 1740, 1268,  258, 1785,   50,  589,  386, 1786,  386,
 1801,  154,  153,  543,  128,  553,  716,  590,  258,  553,
  488,  717,  544,  506,  401,  489,  388, 1740,  558,  351,
  528,  396,  352,  906,  393,  401,  716,  696,  143,  144,
  852,  717,   26,  852,  831,  197,  559,  639,   49,   51,
  436,  629,  720,  718,  934,  719, 1211,  721,  852,   46,
  129,  950,  197, 1803, 1740,  197,  957,  436,  624, 1740,
  492, 1804,  528,  128,  182,  117,  964,  353,  968,  969,
  970,  971,  972,  436,  964,  831,  629,  182,  831,  495,
  396,  697,   47,  528,  153,  182,  528,  154,  153,  178,
  526,   26,  218,  831,  197,  697,  639,   49,   51,  152,
 1475,  528, 1477,  697,  629,  153,  182,  629,   46,  129,
  219,  436,  182,  909,  182, 1473,  909,  624,  493,  697,
 1886, 1025,  526,  629,  117, 1849,  153,  910,  387,  811,
  910,  497,  810,  436,  153,  182,  154,  153,  536, 1044,
  745,   47,  387,  211,  212,  498,  155, 1473,  178,  629,
  387,  218, 1909,  153,  182, 1096, 1097, 1887, 1849,  153,
  499,  512,  153,  182,  526,  909,  387, 1836,  153,  182,
 1888, 1838,  601,  697,  154,  153,  505,  745,  156,  910,
  961,  514, 1844,  722,  511,  526,  153,  182,  526, 1473,
  745, 1025,  515,  157, 1849,  153,  720,  718,  200,  719,
 1915,  721, 1023,  526,  716,  534,  524, 1921, 1872,  717,
 1473, 1028, 1144, 1473, 1023,   14, 1004, 1023,  200, 1003,
  387,  541,  552, 1028,  182, 1014, 1028,  258, 1473,  258,
  720,  718, 1221,  719, 1935,  721,  258, 1011,  188,  200,
 1001,  200,  189, 1126,  758, 1899, 1125,  124,  124, 1133,
 1901,  852,  887,  563,  720,  718,  581,  719,  100,  721,
  584, 1153, 1154, 1158,  594,  555, 1166, 1168, 1170,  598,
 1178, 1181, 1184, 1186, 1189, 1192,  649,  653, 1196, 1197,
  716, 1199,  655, 1201,  657,  717, 1204, 1205, 1206, 1207,
 1208, 1209, 1225,  197,  688, 1224,  831,  188,  689,  101,
  690,  189,  691, 1213,  692,  436,  197,  852,  698, 1228,
  852,  102, 1227, 1276,  197,  693, 1125,  852, 1599,  694,
  852,  695,  153,  182,  852, 1282,  696,  258, 1281,  852,
 1326, 1331,  852, 1325, 1330,  197,  103,  699,  960,  720,
  718,  197,  719,  197,  721,  697,  700,  178,  852,  258,
 1488, 1576,  831,  722, 1224,  831,  258,  436, 1606,  705,
  178,  722,  831,  706,  197,  831, 1021, 1644,  178,  831,
  722,  715,  852,  722,  831,  852,  852,  831, 1882,  639,
  928, 1881,  932,  197,  723,  852,  936,  938,  940,  178,
  258,  197,  197,  831,  724,  178, 1916,  178,  197, 1799,
  624,  391,  852,  728,  391,  104,  997, 1021, 1021, 1021,
 1021, 1021, 1021, 1021,  639,  105,  197,  831,  178,  753,
  831,  831,  753,  731,  852, 1021, 1021, 1021, 1021,  852,
  831,  732,  736,  750,  716,  624,  741,  178,  753,  717,
  106,  852,  639,  852,  758,  639,  178,  831,  760,  761,
  258,  773,  178,  197, 1014,  720,  718, 1021,  719, 1021,
  721,  639,  778,  624,  197, 1583,  624,  782,  716,  831,
  178,  777, 1738,  717,  831,  784, 1167,  240, 1171,  785,
 1179, 1182,  624, 1187, 1190, 1193,  831,  639,  831,  787,
 1739,  794,  716,  789,  790,  254,  188,  717,  720,  718,
  189,  719,  802,  721,  803,  543,  797,  178,  624,  188,
  852,  964,  805,  189,  241, 1791,  812,  188,  806,  837,
  836,  189,  838, 1384, 1385,  843, 1387, 1389,  720,  718,
  848,  719,  286,  721, 1394,  849,  254,  853,  188,  254,
  720,  718,  189,  719,  188,  721,  188,  854,  189,  855,
  189,  856,  197, 1407,  254,  831,  858,  859,  864,  870,
  871, 1414, 1415,  436,  874,  875,  436,  188, 1213,  884,
  886,  189, 1422,  436, 1424, 1425,  715,  716,  887,  409,
  410,  411,  717,  785,  895,  829,  188,  896,  897,  901,
  189,  952,  962,  865,  288,  188,  998,  982,  999,  189,
 1001,  188, 1006,  436, 1022,  189,  178,  419,  420, 1029,
 1038, 1010, 1011,  436,   53, 1045, 1041, 1047, 1066,  188,
  715, 1061, 1076,  189,  785,   54,  829,  785, 1021,  829,
 1021, 1021,   55, 1075, 1078,  715,  815, 1738, 1021, 1082,
 1083, 1738,  785, 1839,  829, 1085, 1021, 1091, 1092, 1093,
 1099, 1021, 1738, 1521, 1021, 1739,  188,   56, 1109, 1739,
  189, 1840, 1116, 1103, 1123, 1140, 1114, 1128, 1021, 1141,
 1739, 1160,  746, 1021, 1021, 1198, 1200, 1021, 1738, 1203,
  720,  718, 1210,  719, 1021,  721, 1021, 1021, 1202, 1222,
 1021, 1229,  716,  716, 1021, 1231, 1739, 1021,  717, 1021,
 1243, 1021,  436, 1246, 1241, 1249, 1021, 1021, 1258, 1021,
 1262, 1264, 1401,  720,  718, 1738,  719, 1270,  721, 1284,
 1738, 1287,   57, 1571, 1307, 1021, 1021, 1309, 1021, 1319,
 1329, 1336, 1579, 1739, 1327, 1340,  716, 1341, 1739, 1584,
 1342,  717, 1021, 1343, 1021, 1021, 1021, 1354, 1372, 1021,
 1134, 1361, 1021, 1021,   58,  188, 1352, 1364,   59,  189,
 1582, 1365, 1021, 1366, 1367, 1373,  716, 1374,  436, 1001,
 1378,  717,  436,  254, 1379,  254, 1021, 1380,  716, 1021,
 1382,  255,  254,  717, 1383, 1390, 1391, 1392, 1393, 1617,
 1395, 1396, 1397, 1398, 1399, 1021, 1400,  925, 1417, 1420,
 1474, 1021, 1021, 1489, 1491, 1492, 1021, 1496, 1497, 1498,
 1001, 1001, 1001, 1001, 1001, 1014, 1001, 1021, 1021, 1499,
 1021, 1500,  255, 1501,  733,  255, 1021, 1508, 1001, 1001,
 1001, 1001, 1513, 1511, 1526, 1402,  720,  718, 1538,  719,
  255,  721,  576, 1530, 1405,  720,  718,  829,  719,  830,
  721, 1647, 1531, 1543, 1532, 1021, 1021, 1533, 1021, 1553,
 1001,  785, 1001,  785, 1534, 1021, 1021, 1021, 1021, 1021,
  785, 1021, 1560,  254, 1021, 1021, 1021, 1021, 1021, 1535,
 1564, 1375, 1021, 1021, 1565, 1569, 1575, 1021, 1021, 1596,
  830, 1609, 1602,  830, 1603,  254, 1610,  715, 1615, 1616,
  135, 1618,  254,  829,  715, 1619,  829,  715,  830, 1626,
 1627,  715, 1628,  829, 1629, 1630,  829, 1635,  716,  715,
  829, 1636, 1639,  717, 1640,  829, 1641, 1663,  829, 1651,
 1673, 1675, 1682, 1709, 1710,  715,  254, 1676, 1677, 1406,
  720,  718, 1678,  719,  829,  721, 1679, 1680, 1683, 1684,
 1721,  716, 1692, 1694,  436, 1702,  717, 1697, 1701,  715,
 1703,  785, 1705, 1730, 1706, 1732,  785, 1707,  829, 1708,
 1711,  829,  829, 1408,  720,  718, 1712,  719, 1713,  721,
 1714,  829, 1715,  785, 1716, 1717, 1718, 1719, 1720,  715,
  785,  704, 1722, 1728,  715, 1735,  254, 1631,  829, 1409,
  720,  718, 1750,  719, 1756,  721, 1765, 1410,  720,  718,
 1767,  719, 1768,  721, 1790, 1796,  715, 1797, 1800, 1808,
  829, 1807, 1811, 1815,  785,  829, 1816, 1827,  715, 1834,
  715, 1001,  704, 1001, 1001,  704, 1819,  829, 1863,  829,
 1837, 1001, 1858, 1865, 1860, 1867, 1870, 1784, 1873, 1001,
  704, 1799, 1884, 1879, 1001, 1787, 1885, 1001, 1855,  255,
 1856,  255, 1900, 1411,  720,  718, 1902,  719,  255,  721,
  163, 1001, 1903, 1001,  716, 1025, 1001, 1001, 1908,  717,
 1001,  267,  436,  716,  785, 1930, 1910, 1001,  717, 1001,
 1001, 1929, 1931, 1001, 1936, 1924,    1, 1001,  654,   30,
 1001,   32, 1001,  204, 1001,  272,  829, 1647,  393, 1001,
 1001,  830, 1001,  551,   82,  149, 1025, 1025, 1025, 1025,
 1025, 1025, 1025,  984,  686,  768,  214,  283, 1001, 1001,
  551, 1001,  483,  421, 1025, 1025, 1025, 1025,  282,  983,
  194,  551,  723,  721,  717, 1001,  195, 1001, 1001, 1001,
  719,  965, 1001,  394,  224, 1001, 1001,  333, 1843,  255,
  299,  740,  341,  964,  739, 1001, 1025,  830, 1025,  353,
  830,  741,  368,  339,  661,  750,   27,  830,  716, 1001,
  830,  255, 1001,  717,  830,   30,   40, 1479,  255,  830,
  140,  496,  830,  487,  486, 1000, 1362,  885, 1001, 1360,
 1139, 1358, 1348,  254, 1001, 1001, 1685,  565,  830, 1001,
 1686,  548,  716,  850,  532, 1337,  608,  717, 1039, 1286,
 1001, 1001,  255, 1001, 1412,  720,  718, 1349,  719, 1001,
  721,  578,  830,  579, 1726,  830,  830, 1766,  716, 1665,
 1671, 1080, 1854,  717, 1859,  830,  716,  831, 1861, 1073,
 1895,  717, 1792, 1233,  231, 1621, 1747,  979, 1001, 1001,
 1842, 1001,  830, 1504, 1277, 1826,  586, 1654, 1001, 1001,
 1001, 1001, 1001, 1550, 1001,  571,  835, 1001, 1001, 1001,
 1001, 1001,  255,  860,  830, 1001, 1001,  562, 1106,  830,
 1001, 1001, 1115,  872,  142,  231,  525,  768,  231, 1030,
  772,  830,  716,  830,  299, 1043, 1814,  717, 1760,  704,
 1024, 1883,  704,  231, 1261, 1907,  781, 1517, 1007,  704,
 1386, 1254,  704, 1419,  860,  815,  704,  860,    0,    0,
    0,  704,    0,    0,  704,    0,    0, 1025,    0, 1025,
 1025,    0,  860,    0,    0,    0,    0, 1025,    0,    0,
  704,    0,    0,    0,    0, 1025,    0,    0,    0,    0,
 1025,    0,    0, 1025,    0,    0,    0,    0,    0,    0,
  830,    0,    0,  389,  704,    0,    0, 1025,  704, 1030,
    0,    0, 1025, 1025,    0,    0, 1025,  704,    0,    0,
    0,    0,    0, 1025,    0, 1025, 1025,    0,    0, 1025,
    0,    0,    0, 1025,  704,    0, 1025,    0, 1025,    0,
 1025,    0,    0,    0,  389, 1025, 1025,  389, 1025,    0,
 1030, 1030, 1030, 1030, 1030, 1030, 1030,    0,    0,    0,
    0,  704,  389,    0, 1025, 1025,    0, 1025, 1030, 1030,
 1030, 1030,    0,  704,    0,  704,    0,    0,    0,    0,
    0, 1025,    0, 1025, 1025, 1025, 1871,    0, 1025,  824,
    0, 1025, 1025,  716,    0,    0,    0,    0,  717,    0,
 1030, 1025, 1030,    0,  433,  941,  942,  943,  944,  945,
  946,  947,  948,    0,    0, 1025,    0,    0, 1025,    0,
    0,    0, 1413,  720,  718,    0,  719,    0,  721,    0,
  824,  107,    0,  824, 1025,    0,    0,    0,    0,    0,
 1025, 1025,  704,    0,    0, 1025,    0,    0,  824,    0,
    0,    0,    0,    0,    0,    0, 1025, 1025,    0, 1025,
  108,  109,  243,  110,  231, 1025,    0,    0,    0,    0,
  111,  112,  113,  114,  115,  860,  116,    0,    0,  117,
  118,  119,  120,  121,    0,    0,    0,  122,  123,    0,
    0,    0,    0,  124, 1025, 1025,    0, 1025,    0,    0,
    0,    0,    0,    0, 1025, 1025, 1025, 1025, 1025,    0,
 1025,    0,    0, 1025, 1025, 1025, 1025, 1025,    0,    0,
    0, 1025, 1025,    0,    0,    0, 1025, 1025,    0,    0,
    0,  860,    0,    0,  860,    0,    0,    0,    0,    0,
    0,  860,    0,    0,  860, 1597,  720,  718,  860,  719,
    0,  721,    0,  860,    0,    0,  860,    0,    0,    0,
    0,    0,  243,    0,    0,    0,    0,    0,    0,    0,
    0, 1030,  860, 1030, 1030,    0,    0,    0,    0,    0,
    0, 1030,    0,    0,  243,    0,    0,    0,    0, 1030,
    0,  243,    0,    0, 1030,    0,  860, 1030,    0,  860,
  860,    0,    0,    0,    0,    0,    0,    0,    0,  860,
    0, 1030,    0, 1026,    0,    0, 1030, 1030,    0,    0,
 1030,    0,    0,    0,  389,  243,  860, 1030,    0, 1030,
 1030,  389,    0, 1030,  389,    0,    0, 1030,  389,    0,
 1030,    0, 1030,    0, 1030,    0,  389,    0,  860, 1030,
 1030,  824, 1030,  860, 1026, 1026, 1026, 1026, 1026, 1026,
 1026,  716,  389,    0,    0,  860,  717,  860, 1030, 1030,
    0, 1030, 1026, 1026, 1026, 1026,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1030,  389, 1030, 1030, 1030,
    0,    0, 1030,    0,    0, 1030, 1030,    0,    0,    0,
    0,    0,    0,    0, 1026, 1030, 1026,  824,    0,    0,
  824,    0,    0,    0,    0,    0,  389,  824,    0, 1030,
  824,    0, 1030,  825,  824,    0,    0,    0,    0,  824,
    0,    0,  824,  530,  860,    0,    0,    0, 1030,    0,
    0,    0,    0,  389, 1030, 1030,    0,    0,  824, 1030,
    0,    0,    0,    0,    0,  389,    0,  389,    0,    0,
 1030, 1030,    0, 1030,  825,    0,    0,  825,    0, 1030,
    0,    0,  824,    0,  530,  824,  824,  530,    0,    0,
    0,    0,  825,    0,  716,  824,    0,    0,    0,  717,
    0,    0,  530,    0,    0,    0,    0,    0, 1030, 1030,
    0, 1030,  824,    0,    0,    0,  107,    0, 1030, 1030,
 1030, 1030, 1030,    0, 1030,    0,    0, 1030, 1030, 1030,
 1030, 1030,    0,    0,  824, 1030, 1030,    0,    0,  824,
 1030, 1030,    0,    0,    0,  108,  109,    0,  110,    0,
    0,  824,    0,  824,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,    0,
    0,    0,    0,    0,    0, 1026,    0, 1026, 1026,    0,
    0,    0,    0,    0,    0, 1026,    0,    0, 1598,  720,
  718,    0,  719, 1026,  721,    0,    0,    0, 1026,    0,
    0, 1026, 1605,  720,  718,    0,  719,    0,  721,    0,
  824,  702,    0,    0,    0, 1026,    0,  879,    0,    0,
 1026, 1026,    0,    0, 1026,    0,    0,    0,    0,    0,
    0, 1026,    0, 1026, 1026,    0,    0, 1026,    0,    0,
    0, 1026,  232,    0, 1026,    0, 1026,    0, 1026,    0,
    0,  702,  702, 1026, 1026,  702, 1026,    0,  879,  879,
  879,  879,  879,    0,  879,    0,    0,    0,    0,    0,
  702,    0, 1026, 1026,    0, 1026,  879,  879,  879,  879,
    0,    0,    0,  232,    0,  825,  232,    0,    0, 1026,
    0, 1026, 1026, 1026,    0, 1015, 1026,    0,    0, 1026,
 1026,  232,    0,    0,    0,    0,    0,    0,    0, 1026,
  879,  530,    0,  530,    0,    0,    0,  530,    0,    0,
  530,    0,    0, 1026,    0,    0, 1026,    0,    0,    0,
    0,    0,    0,  530,    0,    0,    0,    0,    0,    0,
    0,  825, 1026,    0,  825,    0,  690,    0, 1026, 1026,
    0,  825,    0, 1026,  825,    0,    0,    0,  825,   63,
    0,    0,    0,  825, 1026, 1026,  825, 1026,    0,    0,
   64,    0,    0, 1026, 1572, 1573,    0,   65,    0,    0,
 1577, 1578,  825, 1580, 1581,    0,    0,    0,    0,    0,
   66, 1585, 1586, 1587, 1588,    0, 1589, 1590, 1591, 1592,
 1593, 1594, 1026, 1026,    0, 1026,  825,    0,    0,  825,
  825,  530, 1026, 1026, 1026, 1026, 1026,    0, 1026,  825,
    0, 1026, 1026, 1026, 1026, 1026,    0,  716,    0, 1026,
 1026,    0,  717,  530, 1026, 1026,  825,    0,    0,    0,
  530,  716,    0,    0,    0,    0,  717,    0,    0,    0,
    0,   67,    0,    0,    0,    0,    0,    0,  825,    0,
    0,    0,    0,  825,    0,    0,    0,   68,    0,    0,
    0,    0,    0,    0,  530,  825,    0,  825,    0,  879,
    0,  879,  879,    0,   69,    0,    0,    0,    0,  879,
    0,    0,    0,    0,    0,    0,    0,  879,    0,   70,
    0,    0,  879,   71,    0,  879,   72,    0,    0,    0,
    0,    0,    0,    0,   73,  707,    0,    0,    0,  879,
  243,  891,  232,    0,  879,  879,    0,    0,  879,  702,
    0,    0,  702,   74,    0,  879,    0,  879,  879,  702,
    0,  879,  702,    0,  825,  879,  702,  553,  879,    0,
  879,  702,  879,    0,  702,    0,  707,  879,  879,  707,
  879,    0,  891,  891,  891,  891,  891,    0,  891,    0,
  702,    0,    0,    0,  707,    0,  879,  879,    0,  879,
  891,  891,  891,  891,    0,    0,    0,    0,  553,    0,
    0,  553,    0,  879,  702,  879,  879,  879,  702,    0,
  879,    0,    0,  879,  879,    0,  553,  702,    0,    0,
    0,    0,    0,  879,  891,    0,    0,    0,    0,    0,
  243,    0,  690,    0,  702,    0,    0,  879,    0,    0,
  879,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  520,    0,  243,    0,    0,    0,  879,    0,    0,  243,
    0,  702,  879,  879,    0,    0,    0,  879,    0,    0,
    0,    0,    0,  702,    0,  702,    0,    0,  879,  879,
    0,  879,   14,    0,    0,    0,    0,  879,    0,    0,
    0,  520,    0,  243,  520,    0,    0,    0,    0,    0,
    0,    0, 1015,    0,    0,    0,    0,    0,    0,  520,
    0,    0,    0,    0,    0,    0,  879,  879,    0,  879,
    0,    0,    0,    0,    0,  107,  879,  879,  879,  879,
  879,    0,  879,    0,    0,  879,  879,  879,  879,  879,
    0,    0,  702,  879,  879,    0,    0,  894,  879,  879,
    0,    0,    0,    0,  108,  109,    0,  110,    0,    0,
    0,    0,    0,    0,  111,  112,  113,  114,  115,    0,
  116,    0,  693,  117,  118,  119,  120,  121,    0,    0,
    0,  122,  123,    0,    0,    0,    0,  124,  894,  894,
  894,  894,  894,  891,  894,  891,  891,    0,    0,    0,
    0,  693,  693,  891,  693,    0,  894,  894,  894,  894,
    0,  693,  693,  693,  693,  693,  891,  693,    0,  891,
  693,  693,  693,  693,  693,    0,    0,    0,  693,  693,
    0,    0,    0,  891,  693,  553,    0,  553,  891,  891,
  894,  553,  891,  707,  553,    0,  707,    0,    0,  891,
    0,  891,  891,  707,    0,  891,  707,  553,    0,  891,
  707,    0,  891,    0,  891,  707,  891,    0,  707,    0,
    0,  891,  891,    0,  891,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  707,    0,    0,    0,    0,    0,
  891,  891,    0,  891,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  891,  707,  891,
  891,  891,  707,    0,  891,    0,    0,  891,  891,    0,
    0,  707,    0,    0,    0,    0,    0,  891,  520,    0,
  520,    0,    0,    0,    0,  553,    0,  520,  707,    0,
    0,  891,    0,    0,  891,    0,    0,    0,    0,    0,
  520,    0,    0,    0,    0,  826,    0,  553,    0,    0,
  891,    0,    0,    0,  553,  707,  891,  891,    0,    0,
    0,  891,    0,    0,    0,    0,    0,  707,    0,  707,
    0,    0,  891,  891,    0,  891,    0,    0,   78,    0,
    0,  891,    0,    0,    0,    0,  826,    0,  553,  826,
    0,    0,    0,    0,    0,    0,   85,    0,    0,  894,
    0,  894,  894,   86,  826,    0,    0,   87,   88,  894,
  891,  891,    0,  891,    0,    0,   89,    0,  520,    0,
  891,  891,  891,  891,  891,  894,  891,    0,   90,  891,
  891,  891,  891,  891,    0,    0,  707,  891,  891,  894,
  520,  895,  891,  891,  894,  894,    0,  520,  894,    0,
    0,    0,    0,    0,    0,  894,    0,  894,  894,    0,
    0,  894,    0,    0,    0,  894,    0,    0,  894,    0,
  894,    0,  894,    0,    0,    0,    0,  894,  894,    0,
  894,  520,  895,  895,  895,  895,  895,    0,  895,    0,
    0,    0,    0,   91,    0,    0,  894,  894,    0,  894,
  895,  895,  895,  895,    0,    0,    0,    0,    0,    0,
   92,    0,  107,  894,    0,  894,  894,  894,    0,    0,
  894,    0,    0,  894,  894,   93,    0,    0,    0,   94,
    0,    0,    0,  894,  895,    0,    0,    0,    0,    0,
   95,  108,  109,    0,  110,    0,    0,  894,    0,    0,
  894,  111,  112,  113,  114,  115,    0,  116,    0,   96,
  117,  118,  119,  120,  121,    0,  894,    0,  122,  123,
    0,    0,  894,  894,  124,    0,    0,  894,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  894,  894,
    0,  894,    0,    0,    0,    0,    0,  894,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  826,    0,    0,
    0,  553, 1778,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  894,  894,    0,  894,
    0,    0,    0,    0,    0,    0,  894,  894,  894,  894,
  894,    0,  894,    0,    0,  894,  894,  894,  894,  894,
    0,    0,  553,  894,  894,  553,    0,  889,  894,  894,
    0,    0,    0,  826,    0,    0,  826,    0,    0,    0,
  553,    0,    0,  826,    0,    0,  826,    0,    0,    0,
  826,    0,    0,    0,    0,  826,    0,    0,  826,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  889,    0,
  889,  889,  889,  895,  826,  895,  895,    0,    0,    0,
    0,    0,    0,  895,    0,    0,  889,  889,  889,  889,
    0,    0,    0,    0,    0,    0,    0,    0,  826,  895,
    0,  826,  826,    0,    0,    0,    0,    0,    0,    0,
    0,  826,    0,  895,    0,    0,    0,    0,  895,  895,
  889,    0,  895,    0,    0,    0,    0,    0,  826,  895,
    0,  895,  895,    0,    0,  895,    0,    0,    0,  895,
    0,    0,  895,    0,  895,    0,  895,    0,    0,    0,
  826,  895,  895,    0,  895,  826,    0,    0,    0,    0,
    0,   77,    0,  107,    0,    0,    0,  826,    0,  826,
  895,  895,    0,  895,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  895,    0,  895,
  895,  895,  108,  109,  895,  110,    0,  895,  895,    0,
    0,    0,  111,  112,  113,  114,  115,  895,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123,  895,    0,    0,  895,  124,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  859,  826,    0,    0,    0,
  895,    0,    0,    0,    0,    0,  895,  895,    0,    0,
    0,  895,    0,    0,    0,    0,    0,    0,    0,  553,
    0,  553,  895,  895,    0,  895,    0,    0,  553,    0,
    0,  895,    0,    0,    0,    0,  859,    0,    0,  859,
    0,  553,    0,    0,    0,    0,    0,    0,    0,  889,
    0,  889,  889,    0,  859,    0,    0,    0,    0,  889,
  895,  895,    0,  895,    0,    0,    0,    0,    0,    0,
  895,  895,  895,  895,  895,  889,  895,    0,    0,  895,
  895,  895,  895,  895,    0,    0,    0,  895,  895,  889,
    0,  888,  895,  895,  889,  889,    0,    0,  889,    0,
    0,    0,    0,    0,    0,  889,    0,  889,  889,    0,
    0,  889,    0,    0,    0,  889,    0,  551,  889,  553,
  889,    0,  889,    0,    0,    0,    0,  889,  889,    0,
  889,    0,  888,    0,  888,  888,  888,    0,    0,    0,
    0,  553,    0,    0,    0,    0,  889,  889,  553,  889,
  888,  888,  888,  888,    0,    0,    0,    0,  551,    0,
    0,  551,  107,  889,    0,  889,  889,  889,    0,    0,
  889,    0,    0,  889,  889,    0,  551,    0,  551,    0,
    0,    0,  553,  889,  888,    0,    0,    0,    0,    0,
    0,  108,  109,    0,  110,    0,    0,  889,    0,    0,
  889,  111,  112,  113,  114,  115,    0,  116,    0,    0,
  117,  118,  119,  120,  121,    0,  889,    0,  122,  123,
    0,    0,  889,  889,  124,    0,    0,  889,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  889,  889,
    0,  889,    0,    0,    0,    0,    0,  889,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  859,    0,    0,
    0,  224,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  889,  889,    0,  889,
    0,  252,    0,    0,    0,    0,  889,  889,  889,  889,
  889,    0,  889,    0,    0,  889,  889,  889,  889,  889,
    0,    0,  224,  889,  889,  224,    0,  890,  889,  889,
    0,    0,    0,  859,    0,    0,  859,    0,    0,    0,
  224,    0,  252,  859,    0,  252,  859,    0,    0,    0,
  859,    0,    0,    0,    0,  859,    0,    0,  859,    0,
  252,    0,    0,    0,    0,    0,    0,    0,  890,    0,
  890,  890,  890,  888,  859,  888,  888,    0,    0,    0,
    0,    0,    0,  888,    0,    0,  890,  890,  890,  890,
    0,    0,    0,    0,    0,  551,    0,    0,  859,  888,
    0,  859,  859,    0,    0,    0,  976,    0,    0,    0,
    0,  859,    0,  888,  551,  551,    0,  551,  888,  888,
  890,  551,  888,    0,  551,    0,    0,    0,  859,  888,
    0,  888,  888,    0,    0,  888,    0,  551,    0,  888,
    0,    0,  888,  551,  888,    0,  888,    0,    0,    0,
  859,  888,  888,    0,  888,  859,    0,    0,    0,    0,
    0,    0,    0,    0,  853,    0,    0,  859,    0,  859,
  888,  888,    0,  888,    0,  803,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  888,    0,  888,
  888,  888,    0,    0,  888,    0,    0,  888,  888,    0,
    0,    0,    0,    0,    0,  853,    0,  888,  853,    0,
    0,    0,    0,    0,    0,  551,  803,    0,    0,  803,
    0,  888,    0,  853,  888,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  803,    0,  859,  551,    0,    0,
  888,    0,    0,    0,  551,    0,  888,  888,    0,    0,
    0,  888,    0,  551,    0,  551,    0,    0,    0,  224,
    0,  224,  888,  888,    0,  888,    0,    0,  224,    0,
    0,  888,    0,  551,    0,    0,    0,    0,  551,  252,
    0,  252,    0,    0,    0,    0,    0,    0,  252,  890,
    0,  890,  890,    0,    0,    0,    0,    0,    0,  890,
  888,  888,    0,  888,    0,    0,    0,  551,    0,    0,
  888,  888,  888,  888,  888,  890,  888,    0,    0,  888,
  888,  888,  888,  888,    0,    0,    0,  888,  888,  890,
    0,  893,  888,  888,  890,  890,    0,    0,  890,    0,
    0,    0,    0,    0,    0,  890,    0,  890,  890,    0,
    0,  890,    0,    0,    0,  890,    0,    0,  890,  224,
  890,    0,  890,    0,    0,    0,    0,  890,  890,    0,
  890,    0,  893,    0,  893,  893,  893,    0,    0,  252,
    0,  224,    0,    0,    0,    0,  890,  890,  224,  890,
  893,  893,  893,  893,    0,    0,    0,    0,    0,    0,
    0,  252,    0,  890,    0,  890,  890,  890,  252,    0,
  890,  712,    0,  890,  890,    0,    0,    0,    0,    0,
    0,    0,  224,  890,  893,    0,  853,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  890,    0,    0,
  890,    0,  252,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  712,    0,    0,  712,  890,    0,    0,    0,
    0,    0,  890,  890,    0,    0,    0,  890,    0,    0,
  712,    0,    0,    0,    0,    0,    0,    0,  890,  890,
    0,  890,  853,    0,    0,  853,    0,  890,    0,    0,
    0,    0,  853,  803,    0,  853,  803,    0,    0,  853,
    0,    0,    0,  803,  853,    0,  803,  853,    0,    0,
  803,    0,    0,    0,    0,  803,  890,  890,  803,  890,
    0,    0,    0,  853,  107,    0,  890,  890,  890,  890,
  890,    0,  890,    0,  803,  890,  890,  890,  890,  890,
    0,    0,    0,  890,  890,    0,    0,  853,  890,  890,
  853,  853,    0,  108,  109,    0,  110,    0,  803,    0,
  853,  803,  803,  111,  112,  113,  114,  115,  976,  116,
    0,  803,  117,  118,  119,  120,  121,  853,    0,    0,
  122,  123,    0,    0,    0,    0,  124,    0,  803,    0,
    0,    0,    0,  893,    0,  893,  893,    0,    0,  853,
    0,    0,    0,  893,  853,    0,    0,    0,    0,    0,
  803,    0,    0,    0,    0,  803,  853,  693,  853,  893,
    0,    0,    0,    0,    0,    0,    0,  803,    0,  803,
    0,    0,    0,  893,    0,    0,    0,    0,  893,  893,
    0,    0,  893,    0,  433,    0,    0,    0,    0,  893,
    0,  893,  893,    0,    0,  893,    0,  693,  693,  893,
    0,  693,  893,    0,  893,    0,  893,    0,    0,    0,
    0,  893,  893,    0,  893,  712,  693,    0,    0,    0,
    0,    0,    0,    0,    0,  853,    0,    0,    0,    0,
  893,  893,    0,  893,    0,    0,  803,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  893,    0,  893,
  893,  893,    0,    0,  893,    0,    0,  893,  893,    0,
    0,    0,    0,    0,    0,    0,    0,  893,    0,  712,
    0,    0,  712,    0,    0,    0,    1,    0,    0,  712,
    0,  893,  712,    0,  893,    0,  712,    0,    0,    0,
    0,  712,    0,    0,  712,    0,    0,    2,    0,    0,
  893,    0,    0,    0,    0,    3,  893,  893,    0,    0,
  712,  893,    4,    0,    5,    0,    6,    0,    0,    0,
    0,    7,  893,  893,    0,  893,    0,    0,    8,    0,
    0,  893,    0,    0,  712,    0,    0,    0,  712,    0,
    0,    0,    9,    0,    0,    0,    0,  712,    0,    0,
    0,    0,    0,   10,  710,    0,    0,    0,    0,    0,
  893,  893,  975,  893,  712,    0,    0,    0,    0,    0,
  893,  893,  893,  893,  893,    0,  893,  861,    0,  893,
  893,  893,  893,  893,    0,    0,    0,  893,  893,    0,
    0,  712,  893,  893,    0,  710,    0,    0,  710,    0,
    0,    0,    0,  712,    0,  712,    0,    0,    0,    0,
    0,  712,    0,  710,    0,    0,    0,    0,  861,    0,
   11,  861,    0,    0,    0,    0,    0,   12,    0,    0,
   13,    0,   14,   15,    0,    0,  861,    0,    0,    0,
  712,  712,    0,  712,    0,    0,    0,    0,    0,    0,
  712,  712,  712,  712,  712,    0,  712,   16,    0,  712,
  712,  712,  712,  712,  862,    0,    0,  712,  712,    0,
    0,    0,  712,  712,    0,  693,    0,    0,  693,    0,
    0,    0,    0,    0,    0,  693,    0,    0,  693,    0,
    0,    0,  693,    0,    0,    0,    0,  693,    0,    0,
  693,    0,    0,    0,    0,  862,   17,    0,  862,    0,
    0, 1611,    0,    0,    0,    0,  693,    0,    0,    0,
    0,   18,   19,  862,    0,    0,  107,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  693,    0,    0,    0,  693,    0,    0,    0,    0,    0,
    0,    0,    0,  693,    0,  108,  109,    0,  110,    0,
  868,    0,    0,    0,    0,  111,  112,  113,  114,  115,
  693,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,  107,    0,  124,    0,
    0,    0,    0,    0,    0,    0,    0,  693,    0,    0,
    0,  868, 1034, 1034,  868, 1034, 1034, 1034,    0,  693,
    0,  693,    0,    0,    0,  108,  109,  693,  110,  868,
 1034, 1034, 1034,    0,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,  861,
    0,    0,  122,  123,    0,    0,  693,  693,  124,  693,
    0, 1034,    0,    0,    0,    0,  693,  693,  693,  693,
  693,    0,  693,    0,    0,  693,  693,  693,  693,  693,
    0,    0,  710,  693,  693,  710,    0,    0,  693,  693,
    0,    0,  710,    0,    0,  710,    0,  869,    0,  710,
    0,    0,    0,    0,  710,  861,    0,  710,  861,    0,
    0,    0,    0,    0,    0,  861,    0,    0,  861,    0,
    0,    0,  861,  710,    0,    0,    0,  861,    0,    0,
  861,    0,    0,    0,    0,    0,  862,    0,  869, 1035,
 1035,  869, 1035, 1035, 1035,    0,  861,  710,    0,    0,
    0,  710,    0,    0,    0,    0,  869, 1035, 1035, 1035,
  710,    0,    0,    0,    0,    0,    0,    0,    0,  850,
  861,    0,    0,  861,  861,    0,    0,  710,    0,    0,
    0,    0,    0,  861,    0,    0,    0,    0, 1035,    0,
    0,    0,  862,    0,    0,  862,    0,    0,    0,    0,
  861,    0,  862,  854,  710,  862,    0,    0,    0,  862,
  850,    0,    0,  850,  862,    0,  710,  862,  710,    0,
    0,    0,  861,    0,  693,    0,    0,  861,  850,    0,
    0,    0,  868,  862,    0,    0,    0,    0,    0,  861,
    0,  861, 1034,    0,  854,    0,    0,  854,    0,    0,
 1034,    0, 1620,  693,  693, 1034,  693,  862, 1034,    0,
  862,  862,  854,  693,  693,  693,  693,  693,    0,  693,
  862,    0,  693,  693,  693,  693,  693,    0,    0,    0,
  693,  693,    0,    0,    0,  710,  693,  862,  868,    0,
    0,  868,    0,    0, 1034,    0,    0,    0,  868,    0,
    0,  868,    0, 1034,    0,  868,    0,    0,  861,  862,
  868, 1034,    0,  868,  862,    0,    0,    0,    0,    0,
    0,    0,  107,    0,    0,    0,  862,    0,  862,  868,
 1034,    0, 1034,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1034,    0, 1034, 1034,
 1034,  108,  109,  868,  110,    0,  868,  868,    0,  869,
    0,  111,  112,  113,  114,  115,  868,  116,    0, 1035,
  117,  118,  119,  120,  121,    0,    0, 1035,  122,  123,
    0,    0, 1035,  868,  124, 1035,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  862,    0,    0,    0, 1034,
    0,  870,    0,    0,    0,  868,    0,    0,    0,    0,
  868,    0,    0,    0,    0,  869,    0,    0,  869,    0,
    0, 1035,  868,    0,  868,  869,    0,    0,  869,    0,
 1035,  850,  869,    0,    0,    0,    0,  869, 1035,    0,
  869,    0,  870, 1036, 1036,  870, 1036, 1036, 1036,    0,
    0,    0,    0,    0,    0,    0,  869, 1035,    0, 1035,
  870, 1036, 1036, 1036,  863,  854,    0,    0,    0,    0,
    0,    0,    0, 1035,    0, 1035, 1035, 1035,    0,    0,
  869,    0,    0,  869,  869,    0,    0,  850,    0,    0,
  850,  868, 1036,  869,    0,    0,    0,  850,    0,    0,
  850,    0,    0,    0,  850,  863,    0,    0,  863,  850,
  869,    0,  850,    0,    0,    0,    0,    0,    0,    0,
    0,  854,    0,  863,  854,    0, 1035,    0,  850,    0,
    0,  854,  869,    0,  854,    0,    0,  869,  854,    0,
    0,    0,    0,  854,  851,    0,  854,    0,    0,  869,
    0,  869,  850,    0,    0,  850,  850,    0,    0,    0,
    0,    0,  854,    0,    0,  850,    0,  855,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  850,    0,    0,  851,  854,    0,  851,  854,
  854,    0,    0,    0,    0,    0,    0,    0,    0,  854,
    0,    0,    0,  851,  850,    0,    0,    0,  855,  850,
    0,  855,    0,    0,    0,    0,  854,    0,  869,    0,
    0,  850,    0,  850,    0,    0,  855,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  854,    0,
    0,    0,    0,  854,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  107,    0,  854,    0,  854,    0,    0,
    0,    0,    0,  870,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1036,    0,    0,    0,    0,    0,    0,
    0, 1036,  108,  109,    0,  110, 1036,    0,    0, 1036,
  850,    0,  111,  112,  113,  114,  115,    0,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123,    0,    0,    0,    0,  124,    0,    0,    0,  870,
    0,    0,  870,    0,  854, 1036,  863,    0,    0,  870,
    0,    0,  870,    0, 1036,    0,  870,    0,    0,    0,
    0,  870, 1036,    0,  870,    0,  576,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  870, 1036,    0, 1036,    0,    0,    0,    0,  672,    0,
    0,  430,    0,  431,    0,    0,    0, 1036,    0, 1036,
 1036, 1036,  863,    0,  870,  863,  433,  870,  870,    0,
    0,  288,  863,    0,    0,  863,    0,  870,    0,  863,
    0,    0,    0,    0,  863,    0,  851,  863,    0,    0,
    0,    0,    0,    0,  870,    0,    0,    0,    0,    0,
    0,    0,    0,  863,    0,    0,    0,    0,    0,  855,
 1036,    0,    0,    0,    0,    0,  870,    0,    0,    0,
    0,  870,    0,    0,    0,    0,    0,  863,    0,    0,
  863,  863,    0,  870,    0,  870,    0,    0,    0,    0,
  863,    0,  851,    0,    0,  851,    0,    0,  672,    0,
    0,  430,  851,  431,    0,  851,    0,  863,    0,  851,
    0,    0,    0,    0,  851,  855,  433,  851,  855,    0,
    0,  288,    0,    0,    0,  855,    0,    0,  855,  863,
    0,    0,  855,  851,  863,    0,    0,  855,    0,    0,
  855,    0,    0,    0,    0,    0,  863,    0,  863,  107,
    0,    0,  870,    0,    0,    0,  855,  851,    0,    0,
  851,  851,    0,    0,    0,    0,    0,    0,    0,    0,
  851,    0,    0,    0,    0,    0,    0,    0,  108,  109,
  855,  110,    0,  855,  855,    0,    0,  851,  111,  112,
  113,  114,  115,  855,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123,    0,    0,  851,
  855,  124,    0,    0,  851,  863,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  394,  851,    0,  851,    0,
    0,    0,  855,    0,  395,    0,    0,  855,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  396,    0,  855,
    0,  855,    0,  397,  398,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  666,    0,    0,    0,    0,    0,    0,    0,
    0,  399,    0,    0,  400,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  851,    0,    0,    0, 1289,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  402,  403,    0,    0,    0,    0,  855,    0,
    0,    0,    0,    0,  404,  394,    0,    0,  405,    0,
    0,    0,    0,    0,  395,    0,  902,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  396,    0,    0,
  665,    0,    0,  397,  398,    0,    0,    0,    0,  677,
    0,  677,  677,    0,  677,    0,    0,    0,    0,    0,
    0,    0,  666,    0,    0,  709,    0,  677,    0,  668,
    0,  399,  677,    0,  400,  406,    0,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  402,  403,    0,  409,  410,  411,  412,  211,
  212,    0,    0,    0,  404,    0,    0,    0,  405,  413,
    0,    0,    0,    0,    0,    0,  667,    0,  414,  415,
  416,  417,  418,  419,  420,  421,  422,  108,  423,    0,
  110,    0,    0,  424,  425,    0,  426,  427,  428,  113,
  114,  115,  429,  116,    0,    0,  117,  118,  119,  120,
  121,  903,  904,  905,  122,  123,    0,    0,    0,  668,
  124,    0,    0,    0,    0,  406,    0,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  409,  410,  411,  412,  211,
  212,    0,    0,    0,    0,    0,    0,    0,    0,  413,
    0,    0,    0,    0,    0,    0,    0,    0,  414,  415,
  416,  417,  418,  419,  420,  421,  422,  108,  423,  397,
  110,    0,    0,  424,  425,    0,  426,  427,  428,  113,
  114,  115,  429,  116,    0,    0,  117,  118,  669,  670,
  671,    0,    0,    0,  122,  123,    0,  399,    0,  677,
  124,    0,    0,    0,    0,    0,  677,    0,    0,    0,
    0,    0,    0,    0,    0,  677,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  401,    0,  677,    0,
    0,    0,    0,    0,  677,  677,    0,    0,  671,    0,
  671,  671,  677,  671,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  671,    0,    0,    0,
    0,  671,  677,    0,    0,  677,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  960,  720,
  718,    0,  719,    0,  721,    0,    0,    0,    0,    0,
    0,  677,    0,  677,  677,    0,    0,  923,  922,  924,
    0,  406,    0,    0,    0,  677,    0,    0,    0,  677,
    0,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,    0,  672,    0,    0,  430,    0,  431,
    0,  409,  410,  411,    0,  211,  212,    0,    0,    0,
    0,    0,  433,    0,    0,    0,    0,  288,    0,    0,
    0,    0,    0,    0,  414,  415,  416,  417,  418,  419,
  420,  421,  422,    0,    0,    0,  677,    0,    0,  424,
  425,    0,  677,    0,    0,    0,    0,    0,  429,    0,
    0,    0,  677,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  677,  677,  677,  677,
  677,  677,    0,    0,    0,    0,    0,    0,    0,    0,
  677,    0,    0,    0,    0,    0,    0,    0,    0,  677,
  677,  677,  677,  677,  677,  677,  677,  677,  677,  677,
    0,  677,    0,    0,  677,  677,    0,  677,  677,  677,
  677,  677,  677,  677,  677,    0,    0,  677,  677,  677,
  677,  677,    0,    0,    0,  677,  677,    0,  671,    0,
    0,  677,    0,    0,    0,  671,    0,    0,    0,    0,
    0,    0,    0,    0,  671,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  671,    0,    0,
    0,    0,    0,  671,  671,    0,    0,    0,    0,    0,
    0,  671,    0,    0,    0,    0,    0,    0,    0,  910,
    0,    0,    0,    0,    0,    0,    0,  716,    0,    0,
    0,  671,  717,    0,  671,  911,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  671,  394,  671,  671,  672,    0,    0,  430,    0,  431,
  395,  912,    0,    0,  671,    0,    0,    0,  671,    0,
  913,    0,  433,  396,    0,    0,    0,  288,  914,  397,
  398,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  915,  666,  916,
    0,    0,    0,    0,    0,    0,    0,  399,    0,    0,
  400,    0,    0,  917,    0,  918,  919,  920,    0,    0,
    0,    0,    0,    0,    0,  671,    0,    0,    0,    0,
    0,  671,    0,    0,    0,    0,    0,    0,  402,  403,
    0,  671,  582,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,  405,  671,  671,  671,  671,  671,
  671,    0,  898,    0,    0,    0,  921,    0,    0,  671,
    0,    0,    0,    0,    0,    0,    0,    0,  671,  671,
  671,  671,  671,  671,  671,  671,  671,  671,  671,    0,
  671,    0,    0,  671,  671,    0,  671,  671,  671,  671,
  671,  671,  671,  671,    0,  668,  671,  671,  671,  671,
  671,  406,    0,    0,  671,  671,    0,  407,    0,    0,
  671,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  409,  410,  411,  412,  211,  212,    0,    0,    0,
    0,    0,    0,    0,    0,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,  415,  416,  417,  418,  419,
  420,  421,  422,  108,  423,    0,  110,    0,    0,  424,
  425,  394,  426,  427,  428,  113,  114,  115,  429,  116,
  395,    0,  117,  118,  669,  670,  671,    0,    0,    0,
  122,  123,  672,  396,    0,  430,  124,  431,    0,  397,
  398,    0,    0,    0,    0,  107,  720,  718,    0,  719,
  433,  721,    0,    0,    0,  288,    0,    0,  666,    0,
    0,    0,    0,    0,  923,  922,  924,  399,    0,    0,
  400,    0,    0,    0,  108,  109,    0,  110,    0,    0,
    0,    0,    0,    0,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117,  118,  119,  120,  121,  402,  403,
    0,  122,  123,    0,    0,    0,    0,  124,    0,    0,
  404,    0,    0,    0,  405,    0,    0,    0,    0,    0,
    0,    0,  667,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  501,    0,    0,  430,    0,  431,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  433,    0,    0,    0,    0,  288,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  668,    0,    0,    0,    0,
    0,  406,    0,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  409,  410,  411,  412,  211,  212,    0,    0,    0,
    0,    0,    0,    0,    0,  413,    0,    0,    0,    0,
    0,    0,    0,    0,  414,  415,  416,  417,  418,  419,
  420,  421,  422,  108,  423,    0,  110,    0,    0,  424,
  425,    0,  426,  427,  428,  113,  114,  115,  429,  116,
    0,    0,  117,  118,  669,  670,  671,    0,    0,  394,
  122,  123,    0,    0,    0,  481,  124,    0,  395,    0,
    0,    0,    0,    0,    0,    0,  910,    0,    0,    0,
    0,  396,  107,    0,  716,    0,    0,  397,  398,  717,
    0,    0,  911,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  666,    0,    0,    0,
    0,  108,  109,    0,  110,  399,    0,    0,  400,    0,
    0,  111,  112,  113,  114,  115,    0,  116,  912,    0,
  117,  118,  119,  120,  121,    0,    0,  913,  122,  123,
    0,    0,    0,    0,  124,  914,  402,  403,    0,    0,
    0,    0, 1155,    0,    0, 1156,    0,    0,  404,  394,
    0,    0,  405,    0,  915,    0,  916,    0,  395,    0,
  898,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  917,  396,  918,  919,  920,    0,    0,  397,  398,    0,
    0,    0,    0,  501,    0,    0,  430,    0,  431,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  433,    0,  668,    0,  399,  288,    0,  400,  406,
    0,    0,    0,    0,    0,  407,    0,    0,    0,    0,
    0,    0,    0,  921,    0,  408,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  402,  403,    0,  409,
  410,  411,  412,  211,  212,    0,    0,    0,  404,    0,
    0,    0,  405,  413,    0,    0,    0,    0,    0,    0,
    0,    0,  414,  415,  416,  417,  418,  419,  420,  421,
  422,  108,  423,    0,  110,    0,  100,  424,  425,    0,
  426,  427,  428,  113,  114,  115,  429,  116,    0,    0,
  117,  118,  119,  120,  121,    0,    0,    0,  122,  123,
    0,    0,    0,    0,  124,    0,    0, 1157,    0,  406,
    0,    0,    0,    0,    0,  407,    0,  101,    0,    0,
    0,    0,    0,    0,    0,  408,    0,    0,    0,  102,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  409,
  410,  411,  412,  211,  212,    0,    0,    0,    0,    0,
    0,    0,    0,  413,  103,    0,    0,    0,    0,    0,
    0,    0,  414,  415,  416,  417,  418,  419,  420,  421,
  422,  108,  423,    0,  110,    0,    0,  424,  425,    0,
  426,  427,  428,  113,  114,  115,  429,  116,    0,    0,
  117,  118,  119,  120,  121,    0,    0,    0,  122,  123,
    0,    0,    0, 1165,  124,    0, 1156,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,  395,
    0,    0,    0,  104,    0,    0,    0,    0,    0,    0,
    0,    0,  396,  105,    0,    0,    0,    0,  397,  398,
    0,  501,    0,    0,  430,    0,  431,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  106,  433,
    0,    0,    0,    0,  288,    0,  399,    0,    0,  400,
    0,    0,    0,    0,    0,    0,  107,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  402,  403,    0,
    0,    0,    0,    0,    0,  108,  109,    0,  110,  404,
    0,    0,    0,  405,    0,  111,  112,  113,  114,  115,
    0,  116,    0,    0,  117,  118,  119,  120,  121,    0,
    0,    0,  122,  123,    0,    0,    0,    0,  124,  501,
    0,    0,  430,    0,  431,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  433,    0,  987,
    0,    0,  288,    0,    0,    0,    0,    0, 1157,    0,
  406,    0,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  408,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  409,  410,  411,  412,  211,  212,    0,    0,    0,    0,
    0,    0,    0,    0,  413,    0,    0,    0,    0,    0,
    0,    0,    0,  414,  415,  416,  417,  418,  419,  420,
  421,  422,  108,  423,    0,  110,    0,    0,  424,  425,
    0,  426,  427,  428,  113,  114,  115,  429,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123, 1169,    0,    0, 1156,  124,    0,  548,  394,    0,
    0,    0,    0,    0,  990,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  396,  107,    0,    0,    0,    0,  397,  398,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  548,    0,
    0,  548,    0,    0,    0,    0,    0,    0,    0,    0,
  108,  109,    0,  110,  399,    0,  548,  400,  548,    0,
  111,  112,  113,  114,  115,    0,  116,    0,    0,  117,
  118,  119,  120,  121,    0,    0,    0,  122,  123,    0,
    0,    0,    0,  124,    0,  402,  403,    0,    0, 1177,
    0,    0, 1156,    0,    0,    0,  394,  404,    0,    0,
    0,  405,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  396,    0,
    0,    0,    0,    0,  397,  398,    0,  501,    0,    0,
  430,    0,  431,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  433,    0,    0,    0,    0,
  288,    0,  399,    0,    0,  400, 1157,    0,  406,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,    0,    0,    0,
    0,    0,    0,  402,  403,    0,    0,    0,  409,  410,
  411,  412,  211,  212,    0,  404,    0,    0,    0,  405,
    0,    0,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,    0,  110,    0,    0,  424,  425,    0,  426,
  427,  428,  113,  114,  115,  429,  116,    0,    0,  117,
  118,  119,  120,  121,    0,    0,    0,  122,  123,    0,
    0,    0,    0,  124, 1157,  548,  406,    0,    0,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  408,    0,  548,  548,    0,  548,    0,    0,
    0,  548,    0,    0,  548,    0,  409,  410,  411,  412,
  211,  212,    0,    0,    0,    0,    0,  548,    0,    0,
  413,    0,    0,  548,    0,    0,    0,    0,    0,  414,
  415,  416,  417,  418,  419,  420,  421,  422,  108,  423,
    0,  110,    0,    0,  424,  425,    0,  426,  427,  428,
  113,  114,  115,  429,  116,    0,    0,  117,  118,  119,
  120,  121,    0,    0,    0,  122,  123, 1180,    0,    0,
 1156,  124,    0,    0,  394,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  548,  396,    0,    0,    0,
    0,    0,  397,  398,    0,  501,    0,    0,  430,    0,
  431,    0,    0,    0,    0,    0,    0,  548,    0,    0,
    0,    0,    0,  433,  548,    0,    0,    0,  288,    0,
  399,    0,    0,  400,    0,  548,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  548,    0,    0,    0,    0,  548,    0,
    0,  402,  403,    0,    0,    0,    0,    0,    0,    0,
    0,  888,    0,  404,    0,  610,    0,  405,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  548,    0,  889,
    0,    0,    0,  611,    0,    0,  612,    0,    0,    0,
    0,    0,    0,  501,    0,    0,  430,    0,  431,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  433,    0,    0,    0,    0,  288,    0,    0,    0,
    0,    0, 1157,    0,  406,    0,    0,  613,    0,    0,
  407,    0,    0,  614,  615,  616,    0,  617,  618,  619,
  408,  620,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  409,  410,  411,  412,  211,  212,
    0,  621,    0,  622,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,  623,  426,  427,  428,  113,  114,
  115,  429,  116,    0,    0,  117,  118,  119,  120,  121,
    0,  624,    0,  122,  123, 1185,    0,    0, 1156,  124,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,  625,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  396,    0,    0,    0,    0,    0,
  397,  398,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  222,    0,    0,    0,  222,    0,    0,  626,  627,
    0,    0,    0,    0,    0,    0,    0,    0,  399,  222,
    0,  400,    0,  222,    0,  628,  222,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  402,
  403,    0,    0, 1188,  558,    0, 1156,    0,    0,    0,
  394,  404,    0,    0,    0,  405,    0,  222,    0,  395,
    0,    0,    0,  222,  222,  222,    0,  222,  222,  222,
    0,  222,  396,    0,    0,    0,    0,    0,  397,  398,
    0,  501,    0,    0,  430,  558,  431,    0,  558,    0,
    0,  222,    0,  222,    0,    0,    0,    0,    0,  433,
    0,    0,    0,  558,  288,  558,  399,    0,    0,  400,
 1157,    0,  406,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,    0,  222,    0,    0,    0,  408,   81,
    0,    0,    0,    0,    0,  558,    0,  402,  403,    0,
    0,  222,  409,  410,  411,  412,  211,  212,    0,  404,
    0,    0,    0,  405,    0,    0,  413,    0,    0,    0,
    0,    0,    0,  222,    0,  414,  415,  416,  417,  418,
  419,  420,  421,  422,  108,  423,    0,  110,    0,    0,
  424,  425,    0,  426,  427,  428,  113,  114,  115,  429,
  116,  553,    0,  117,  118,  119,  120,  121,  222,  222,
    0,  122,  123,    0,    0,    0,    0,  124, 1157,    0,
  406,    0,    0,    0,    0,  222,  407,    0,    0,    0,
    0,    0,    0,    0, 1757,    0,  408,    0,    0,    0,
    0,    0,  553,    0,    0,  553,    0,    0,    0,    0,
  409,  410,  411,  412,  211,  212,    0,    0,    0, 1758,
  553,    0,  553,    0,  413,    0,    0,    0,    0,    0,
    0,    0,    0,  414,  415,  416,  417,  418,  419,  420,
  421,  422,  108,  423,    0,  110,    0,    0,  424,  425,
    0,  426,  427,  428,  113,  114,  115,  429,  116,    0,
    0,  117,  118,  119,  120,  121,    0,    0,    0,  122,
  123, 1191,  558,    0, 1156,  124,    0, 1759,  394,    0,
    0,  501,    0,  506,  430,    0,  431,  395,  558,    0,
    0,  558,  558,    0,  558,    0,    0,    0,  558,  433,
  396,  558,    0,    0,  288,    0,  397,  398,    0,    0,
    0,  107,    0,    0,  558,    0,    0,    0,    0,    0,
  558,    0,    0,  558,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,  400,    0,    0,
  108,  109,    0,  110,    0,    0,    0,    0,    0,    0,
  111,  112,  113,  114,  115,    0,  116,    0,    0,  117,
  118,  119,  120,  121,    0,  402,  403,  122,  123,    0,
    0,    0,    0,  124,    0,    0,    0,  404,    0,  501,
    0,  405,  430,    0,  431,    0,    0,    0,    0,    0,
    0,    0,  558,    0,    0,    0,    0,  433,    0,    0,
    0,    0,  288,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  558,    0,    0,    0,    0,  553,
    0,  558,    0,    0,    0,  558,    0,    0,    0,    0,
    0,    0,  558,    0,    0,    0, 1157,    0,  406,  553,
    0,  553,    0,    0,  407,  553,    0,    0,  553,    0,
  558,    0,    0,    0,  408,  558,    0,    0,    0,    0,
    0,  553,    0,    0,    0,    0,    0,  553,  409,  410,
  411,  412,  211,  212,    0,    0,    0,    0,    0,    0,
  558,    0,  413,    0,  558,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,    0,  110,    0,    0,  424,  425,  394,  426,
  427,  428,  113,  114,  115,  429,  116,  395,    0,  117,
  118,  119,  120,  121,    0,    0,    0,  122,  123,    0,
  396,    0,    0,  124,    0,    0,  397,  398,    0,  553,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  553,    0,    0,  399,    0,    0,  400,  553,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  553,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  401,    0,  402,  403,  553,    0,    0,
    0,    0,  553,    0,    0,    0,  394,  404,    0,    0,
    0,  405,    0,    0,    0,  395,    0,    0,  995,    0,
  995,  995,    0,  995,    0,    0,    0,    0,  396,    0,
    0,  553,    0,    0,  397,  398,  995,    0,    0,    0,
    0,  995,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,  400,    0,    0,  406,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,    0,    0,    0,
    0,  401,    0,  402,  403,    0,    0,    0,  409,  410,
  411,  412,  211,  212,    0,  404,    0,    0,    0,  405,
    0,    0,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,    0,  110,    0,    0,  424,  425,  100,  426,
  427,  428,  113,  114,  115,  429,  116,    0,    0,  117,
  118,  119,  120,  121,    0,    0,  709,  122,  123,    0,
    0,    0,    0,  124,    0,    0,  406,    0,    0,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,  101,
    0,    0,  408,    0,    0,    0,    0,    0,    0,    0,
    0,  102,    0,    0,    0,    0,  409,  410,  411,  412,
  211,  212,    0,    0,    0,  137,    0,    0,    0,    0,
  413,    0,    0,    0,    0,    0,  103,    0,    0,  414,
  415,  416,  417,  418,  419,  420,  421,  422,  108,  423,
    0,  110,    0,    0,  424,  425,    0,  426,  427,  428,
  113,  114,  115,  429,  116,  995,    0,  117,  118,  119,
  120,  121,    0,    0,  995,  122,  123,  107,    0,    0,
    0,  124,    0,    0,    0,  677,    0,  995,  677,    0,
  677,    0,    0,  995,  995,    0,    0,    0,    0,    0,
    0,    0,    0,  677,    0,  104,  108,  109,  677,  110,
    0,    0,    0,    0,    0,  105,  111,  112,  113,  114,
  115,  995,  116,    0,  995,  117,  118,  119,  120,  121,
    0,    0,    0,  122,  123,    0,    0,    0,    0,  124,
  106,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  995,    0,  995,  995,    0,    0,    0,    0,  107,    0,
    0,    0,    0,    0,  995,    0,    0,    0,  995,    0,
    0,  432,    0,    0,  430,    0,  431,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  108,  109,  433,
  110,    0,    0,    0,  288,    0,    0,  111,  112,  113,
  114,  115,    0,  116, 1231,    0,  117,  118,  119,  120,
  121,    0,    0,    0,  122,  123,    0,    0,    0,    0,
  124,    0,    0,    0,    0,  995,    0,    0,    0,    0,
    0,  995,    0,    0,    6,    0,    0,    0,    0,    0,
    0,  995,    0,    0,    0, 1433, 1434,    0, 1435,    0,
    0,    0,    0,    0,    0,  995,  995,  995,  995,  995,
  995,    0,    0,    0,    0, 1436,    0,    0,    0,  995,
    0, 1437,    0,    0,    0,    0,    0,    0,  995,  995,
  995,  995,  995,  995,  995,  995,  995,  995,  995,    0,
  995,    0,    0,  995,  995,    0,  995,  995,  995,  995,
  995,  995,  995,  995,    0,  677,  995,  995,  995,  995,
  995,    0,  677,    0,  995,  995,    0,    0,    0,    0,
  995,  677,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1438,    0,    0,  677,    0,    0,    0,    0,    0,
  677,  677,    0,    0,    0,    0,    0,    0,  677,    0,
   14,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1439,    0,    0,  677,    0,
    0,  677,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0, 1631,    0,    0,    0,    0,    0,
    0,    0,    0,  107,    0,    0,    0,    0,  394,  677,
  677,  501,    0,    0,  430,    0,  431,  395,    0,    0,
    0,  677,    0,    0,    0,  677,    0,    0,    0,  433,
  396,    0, 1440,  109,  288,  110,  397,  398,    0,    0,
    0,    0,  111,  112,  113,  114,  115,    0,  116,    0,
    0,  117, 1441,  119,  120,  121,    0,    0,    0,  122,
  123, 1442, 1443, 1444,  399,  124,    0,  400,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  677,    0,    0,    0,    0,    0,  677,    0,
    0,    0,    0,  401,    0,  402,  403,    0,  677,    0,
    0,    0,    0,    0,    0,    0,    0,  404,    0,    0,
    0,  405,  677,  677,  677,  677,  677,  677,    0,    0,
    0,    0,    0,    0,    0,    0,  677,    0,    0,    0,
    0,    0,    0,    0,    0,  677,  677,  677,  677,  677,
  677,  677,  677,  677,  677,  677, 1231,  677,    0,    0,
  677,  677,    0,  677,  677,  677,  677,  677,  677,  677,
  677,    0,    0,  677,  677,  677,  677,  677,  406,    0,
    0,  677,  677,    0,  407,    0,    6,  677,    0,    0,
    0,    0,    0,    0,  408,    0,    0, 1433, 1434,    0,
 1435,    0,    0,    0,    0,    0,    0,    0,  409,  410,
  411,  412,  211,  212,    0,    0,    0, 1436,    0,    0,
    0,    0,  413, 1437,    0,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,    0,  110,    0,    0,  424,  425,  394,  426,
  427,  428,  113,  114,  115,  429,  116,  395,    0,  117,
  118,  119,  120,  121,    0,    0,    0,  122,  123,  501,
  396,    0,  430,  124,  431,    0,  397,  398,    0,    0,
    0,  596,    0, 1438,    0,    0,    0,  433,    0,    0,
    0,    0,  288,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   14,    0,  399,    0,    0,  400,    0,    0,
  596,  596,    0,  596,    0,    0,    0, 1439,    0,    0,
  596,  596,  596,  596,  596,    0,  596,   16,    0,  596,
  596,  596,  596,  596,    0,  402,  403,  596,  596,    0,
    0,    0,    0,  596,    0,  107,    0,  404,    0,  501,
    0,  405,  430,    0,  431,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  433,    0,    0,
    0,    0,  288,    0, 1440,  109,    0,  110,    0,    0,
    0,    0,    0,    0,  111,  112,  113,  114,  115,    0,
  116,    0,    0,  117, 1441,  119,  120,  121,  987,    0,
    0,  122,  123, 1442, 1443, 1444,    0,  124,  406,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,    0,    0,    0,
    0,    0,  701,    0,    0,    0,    0,    0,  409,  410,
  411,  412,  211,  212,    0,    0,    0,    0,    0,    0,
    0,    0,  413,    0,    0,    0,    0,    0,    0,    0,
    0,  414,  415,  416,  417,  418,  419,  420,  421,  422,
  108,  423,  988,  110,    0,    0,  424,  425,    0,  426,
  427,  428,  113,  114,  115,  429,  116,    0,    0,  117,
  118,  119,  120,  121,    0,    0,  394,  122,  123,    0,
    0,    0,    0,  124,    0,  395,    0,    0,  989,    0,
    0,    0,    0,  990,    0,    0,    0,    0,  396,  991,
    0,    0,    0,    0,  397,  398,    0,    0,    0,    0,
  107,    0,    0,    0,    0,    0,    0,    0,  992,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,  400,    0,    0,    0,  108,
  109,    0,  110,    0,    0,    0,    0,    0,    0,  111,
  112,  113,  114,  115,    0,  116,  394,    0,  117,  118,
  119,  120,  121,  402,  403,  395,  122,  123,  501,  963,
    0,  430,  124,  431,    0,  404,    0,    0,  396,  405,
    0,    0,    0,    0,  397,  398,  433,    0,    0,    0,
    0,  288,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,  400,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  709,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  406,    0,    0,    0,
    0,  401,  407,  402,  403,    0,    0,    0,    0,    0,
    0,    0,  408,    0,    0,  404,    0,    0,    0,  405,
    0,    0,    0,    0,    0,    0,  409,  410,  411,  412,
  211,  212,    0,    0,    0,    0,    0,    0,    0,    0,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
  415,  416,  417,  418,  419,  420,  421,  422,  108,  423,
    0,  110,    0,    0,  424,  425,    0,  426,  427,  428,
  113,  114,  115,  429,  116,    0,  406,  117,  118,  119,
  120,  121,  407,    0,    0,  122,  123,    0,    0,    0,
    0,  124,  408,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  409,  410,  411,  412,
  211,  212,    0,    0,    0,    0,    0,    0,    0,    0,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  414,
  415,  416,  417,  418,  419,  420,  421,  422,  108,  423,
    0,  110,    0,    0,  424,  425,    0,  426,  427,  428,
  113,  114,  115,  429,  116,  394,    0,  117,  118,  119,
  120,  121,    0,  477,  395,  122,  123,  501,    0,    0,
  430,  124,  431,    0,    0,    0,    0,  396,    0,    0,
    0,    0,    0,  397,  398,  433,    0,    0,    0,  107,
  288,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  399,    0,    0,  400,    0,    0,    0,  108,  109,
    0,  110,    0,    0,    0,    0,    0,    0,  111,  112,
  113,  114,  115,    0,  116,    0,    0,  117,  118,  119,
  120,  121,  402,  403,    0,  122,  123,  501,    0,    0,
  430,  124,  431,    0,  404,    0,    0,    0,  405,    0,
    0,    0,    0,    0,    0,  433,    0,    0,  107,    0,
  288,    0,    0,    0,    0,    0,    0,  167,  168,  169,
  170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
  180,  181,  182,    0,    0,  183,  184,  108,  109,    0,
  110,    0,    0,  185,  186,    0,    0,  111,  112,  113,
  114,  115,  187,  116,  188,  406,  117,  118,  119,  120,
  121,  407,    0,    0,  122,  123,  189,  190,  191,  192,
  124,  408,    0,    0,    0,  222,    0,    0,    0,  222,
    0,    0,    0,    0,    0,  409,  410,  411,  412,  211,
  212,    0,    0,  222,    0,    0,    0,  222,    0,  413,
  222,    0,    0,    0,    0,    0,    0,    0,  414,  415,
  416,  417,  418,  419,  420,  421,  422,  108,  423,    0,
  110,    0,    0,  424,  425,    0,  426,  427,  428,  113,
  114,  115,  429,  116,  394,    0,  117,  118,  119,  120,
  121,  222,    0,  395,  122,  123,    0,  222,  222,  222,
  124,  222,  222,  222,    0,  222,  396,    0,    0,    0,
    0,    0,  397,  398,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  222,    0,  222,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  222,    0,
    0,    0,    0,    0,  394,    0,    0,    0,    0,    0,
    0,  402,  403,  395,    0,  222,  501,    0,    0,  430,
    0,  431,    0,  404,    0,    0,  396,  405,    0,    0,
    0,    0,  397,  398,  433,    0,    0,  222,    0,  288,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  222,  222,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  406,    0,    0,    0,    0,  222,
  407,  402,  403,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,  404,    0,    0,    0,  405,    0,    0,
    0, 1183,    0,    0,  409,  410,  411,  412,  211,  212,
    0,    0,    0,    0,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425, 1375,  426,  427,  428,  113,  114,
  115,  429,  116,    0,  406,  117,  118,  119,  120,  121,
  407,    0,    0,  122,  123,    0,    0,    0,    0,  124,
  408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  409,  410,  411,  412,  211,  212,
    0,    0,    0,    0,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,    0,    0,  414,  415,  416,
  417,  418,  419,  420,  421,  422,  108,  423,    0,  110,
    0,    0,  424,  425,    0,  426,  427,  428,  113,  114,
  115,  429,  116,  394,    0,  117,  118,  119,  120,  121,
    0,    0,  395,  122,  123,  501,    0,    0,  430,  124,
  431,    0,    0,    0,  291,  396,    0,    0,    0,    0,
    0,  397,  398,  433, 1729,    0,    0,    0,  288,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  291,  291,    0,  291,    0,    0,  399,
    0,    0,  400,  291,  291,  291,  291,  291,    0,  291,
    0,    0,  291,  291,  291,  291,  291,    0,    0,    0,
  291,  291,    0,    0,    0,    0,  291,    0,    0,    0,
  402,  403,    0,    0,    0,  501,    0,    0,  430,    0,
  431,    0,  404,    0,    0,    0,  405,    0,    0,    0,
    0,    0,    0,  433,    0,    0,  479,    0,  288,    0,
    0,    0,    0,    0,    0,    0,  479,  479,  479,  479,
  479,  479,  479,  479,  479,  479,  479,  479,  479,  479,
  479,    0,    0,  479,  479,  479,  479,    0,  479,    0,
    0,  479,  479,    0,    0,  479,  479,  479,  479,  479,
  479,  479,  479,  406,  479,  479,  479,  479,  479,  407,
    0,    0,  479,  479,  479,  479,  479,  479,  479,  408,
    0,    0,    0, 1001,    0,    0,    0, 1001,    0,    0,
 1388,    0,    0,  409,  410,  411,  412,  211,  212,    0,
    0,    0,    0, 1003,    0, 1001,    0,  413, 1001,    0,
    0,    0,    0,    0,    0,    0,  414,  415,  416,  417,
  418,  419,  420,  421,  422,  108,  423,    0,  110,    0,
    0,  424,  425,    0,  426,  427,  428,  113,  114,  115,
  429,  116,  394,    0,  117,  118,  119,  120,  121, 1001,
    0,  395,  122,  123,    0, 1001, 1001, 1001,  124, 1001,
 1001, 1001,    0, 1001,  396,    0,    0,    0,    0,    0,
  397,  398,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1001,    0, 1001,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1001,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,  402,
  403,  395,    0, 1001,  995,    0,    0,  995,    0,  995,
    0,  404,    0,    0,  396,  405,    0,    0,    0,    0,
  397,  398,  995,    0,    0, 1001,    0,  995,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
 1001, 1001,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  406,    0,    0,    0,    0, 1001,  407,  402,
  403,    0,    0,    0,    0,    0,    0,    0,  408,    0,
    0,  404,    0,    0,    0,  405,    0,    0,    0,    0,
    0,    0,  409,  410,  411,  412,  211,  212,    0,    0,
    0,    0,    0,    0,    0,    0,  413,    0,    0,    0,
    0,    0,    0,    0,    0,  414,  415,  416,  417,  418,
  419,  420,  421,  422,  108,  423,    0,  110,    0,    0,
  424,  425,    0,  426,  427,  428,  113,  114,  115,  429,
  116,    0,  406,  117,  118,  119,  120,  121,  407,    0,
    0,  122,  123,    0,    0,    0,    0,  124,  408,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  409,  410,  411,  412,  211,  212,    0,    0,
    0,    0,    0,    0,    0,    0,  413,    0,    0,    0,
    0,    0,    0,    0,    0,  414,  415,  416,  417,  418,
  419,  420,  421,  422,  108,  423,    0,  110,    0,    0,
  424,  425,    0,  426,  427,  428,  113,  114,  115,  429,
  116,  995,  563,  117,  118,  119,  120,  121,    0,    0,
  995,  122,  123,    0,  564,    0,    0,  124,    0,    0,
    0,    0,    0,  995,    0,    0,  580,    0,    0,  995,
  995,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  563,  563,    0,    0,  563,    0,    0,    0,
    0,    0,    0,    0,  564,  564,    0,  995,  564,    0,
  995,  563,    0,  563,    0,    0,    0,  580,    0,    0,
  580,    0,    0,  564,    0,  564,    0,    0,    0,  570,
    0,    0,    0,    0,    0,  580,    0,  580,  995,  995,
    0,  555,    0,  563,    0,    0,    0,    0,    0,    0,
  995,    0,    0,    0,  995,  564,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  580,    0,    0,
  570,    0,    0,  570,    0,    0,    0,    0,    0,    0,
    0,    0,  555,    0,    0,  555,    0,    0,  570,    0,
  570,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  555,    0,  555,    0,    0,    0,    0,    0,    0,    0,
    0,  995,    0,    0,    0,    0,    0,  995,    0,    0,
  570,    0,    0,    0,    0,    0,    0,  995,    0,    0,
    0,    0,  555,    0,    0,    0,    0,    0,    0,    0,
    0,  995,  995,  995,  995,  995,  995,    0,    0,    0,
    0,    0,    0,    0,    0,  995,    0,    0,    0,    0,
    0,    0,    0,    0,  995,  995,  995,  995,  995,  995,
  995,  995,  995,  995,  995,    0,  995,    0,    0,  995,
  995,    0,  995,  995,  995,  995,  995,  995,  995,  995,
    0,    0,  995,  995,  995,  995,  995,    0,    0,    0,
  995,  995,    0,    0,    0,    0,  995,    0,    0,    0,
  563,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  564,    0,    0,    0,  563,    0,    0,  563,
  563,    0,  563,    0,  580,    0,  563,    0,  564,  563,
    0,  564,  564,    0,  564,    0,    0,    0,  564,    0,
  580,  564,  563,    0,  580,    0,  580,    0,  563,    0,
  580,  563,    0,  580,  564,    0,    0,    0,    0,    0,
  564,    0,    0,  564,    0,    0,  580,    0,    0,    0,
    0,    0,  580,    0,    0,  580,    0,  570,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  555,
    0,    0,    0,  570,    0,    0,    0,  570,    0,  570,
    0,    0,    0,  570,    0,  555,  570,    0,    0,  555,
    0,  555,    0,    0,    0,  555,    0,    0,  555,  570,
  563,    0,    0,    0,    0,  570,    0,  331,  570,    0,
    0,  555,  564,    0,    0,    0,    0,  555,    0,    0,
  555,    0,  563,    0,  580,    0,    0,    0,    0,  563,
    0,    0,    0,  563,  564,    0,    0,  331,    0,    0,
  563,  564,    0,    0,  331,  564,  580,    0,  331,  331,
    0,  331,  564,  580,  331,    0,    0,  580,  563,    0,
    0,    0,    0,  563,  580,    0,    0,    0,  331,    0,
  564,    0,    0,    0,  331,  564,    0,  570,    0,    0,
    0,    0,  580,    0,    0,    0,    0,  580,  563,  555,
    0,    0,  563,    0,    0,    0,    0,    0,    0,  570,
  564,    0,    0,    0,  564,    0,  570,    0,    0,    0,
  570,  555,  580,    0,    0,    0,  580,  570,  555,    0,
    0,    0,  555,    0,    0,    0,    0,    0,    0,  555,
    0,    0,  609,    0,  331,  570,  610,    0,    0,    0,
  570,    0,    0,  815,    0,    0,    0,  555,    0,    0,
    0,    0,  555,  331,  611,    0,    0,  612,    0,    0,
    0,    0,    0,    0,    0,  570,    0,    0,  331,  570,
    0,    0,    0,    0,    0,    0,    0,  555,  331,    0,
    0,  555,    0,    0,    0,    0,    0,  331,    0,  331,
    0,    0,    0,    0,    0,    0,  331,    0,  613,    0,
    0,    0,    0,    0,  614,  615,  616,    0,  617,  618,
  619,    0,  620,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  331,  331,    0,  331,    0,
    0,    0,  621,    0,  622,  331,  331,  331,  331,  331,
    0,  331,    0,    0,  331,  331,  331,  331,  331,    0,
    0,    0,  331,  331,  331,  331,  331,    0,  331,    0,
    0,    0,    0,    0,    0,  623,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  624,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  625,    0,  609,    0,    0,    0,
  610,    0,    0,    0,    0,    0,    0,    0,    0,  107,
    0,    0,    0,    0,    0,    0,    0,    0,  611,    0,
    0,  612,    0,    0,    0,    0,    0,    0,    0,  626,
  627,    0,    0,    0,    0,    0,    0,    0,  108,  109,
    0,  110,    0,    0,    0,    0,  628,    0,  111,  112,
  113,  114,  115,    0,  116,    0,    0,  117,  118,  119,
  120,  121,  613,    0,    0,  122,  123,    0,  614,  615,
  616,  124,  617,  618,  619,    0,  620,  609,    0,    0,
    0,  610,    0,    0,    0,    0,  134,    0,    0,    0,
  134,    0,    0,    0,    0,    0,  621,    0,  622,  611,
    0,    0,  612,    0,    0,    0,    0,    0,  134,    0,
    0,  134,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  623,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  613,    0,    0,  624,    0,    0,  614,
  615,  616,  134,  617,  618,  619,    0,  620,  134,  134,
  134,    0,  134,  134,  134,    0,  134,    0,  625,    0,
    0,    0,    0,    0,    0,    0,    0,  621,    0,  622,
    0,    0,    0,  107,    0,    0,  134,    0,  134,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  626,  627,    0,    0,    0,    0,    0,
  623,    0,  108,  109,    0,  110,    0,    0,    0,  134,
  628,    0,  111,  112,  113,  114,  115,  624,  116,    0,
    0,  117,  118,  119,  120,  121,  134,    0,    0,  122,
  123,    0,    0,    0,    0,  124,    0,    0,    0,  625,
    0,    0,    0,    0,    0,    0,    0,    0,  134,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  626,  627,    0,    0,    0,    0,
    0,    0,    0,  134,  134,    0,    0,    0,    0,    0,
    0,  628,    0,    0,    0,    0,    0,    0,    0,    0,
  134,
};
static short yycheck[] = {                                       9,
   56,    4,   12,   65,  106,   16,   16,    0,   18,  143,
  144,   64,  701,   58,  143,  144,  296,  154,  529,  364,
   92,  343,  372,  372,   86,   70,  368,  469,   74,   64,
   71,  897,   95,  724,   90,  901,  577,  512,  341,  536,
   63,  793,  432,  273,   54,   55,   56,   57,   58,   59,
   96,  959,  282,   94,   64,   65,   66,   89,   68,  709,
   70,   71, 1230,   73,   74, 1078,   57,  489,  983,   69,
  851, 1306,   82,   84,   84,   85,   86,   68,   88,   89,
   90,   91,   93,   93,   94,   95,   96,  778,   98,  750,
  135,    0,   92,    0,  157,  389,  746, 1005,  731,  134,
   91,  136,   41,   41,  654, 1072, 1015,   98,  723,  350,
  336,  156, 1079,  290,   44,   41,  148,  349,   45,    0,
   41,  226, 1104, 1090,  134,  135,  136,  661,  570, 1067,
  572,   45,   41,  235,   41,  577,  377, 1067,  148,   40,
 1078, 1127,   40, 1110,  154,  155,  156,  157, 1078, 1067,
   59,  161,   59,   44,   44,   40, 1067,   44,   42,    0,
 1078, 1074,   63,  317,  155,   63, 1067, 1078,   41,  882,
   41,  161, 1067,  196,   41,    0,   41, 1078,   59,   45,
  317,    0,   40, 1078,    0,   44,   61,    0,   40,   45,
    0,   44, 1424,   44,  667,   44, 1263,    0,  221, 1304,
   41,   61,   44,   44,  546, 1354,  164,  301,  274, 1122,
 1558,  556,  557,  558,  559,  320,   41,    0,   59,   44,
 1128,   44,  324,  785,  750,    0,  271,  577,  577,  264,
    0,   41,  267,  262,   59,  280,  925,  926,   41,    0,
   59,    0,  273,   59,   44,  301,   59,  809,  759,   59,
  812,  481,    0,   91,  264, 1487,   59,  267,   41,  821,
  305,  271,  604,  273,  274,    0,   41,  956,   63,   44,
  280,   41,  282,  455, 1442, 1443,   59,  580, 1241, 1248,
 1243,  280,  317,   12,   59, 1298, 1299,   45,    0,   59,
  531,  301,    0,   41,   59,  305,   44,   40,   59,  283,
   59,  318,   45,  262,    0,   63,   12,  342, 1252,  340,
  277,   59,    0,  875,  349,  609,  351,  273,  353, 1332,
   41,  356, 1811,   44,   59, 1212,    0,  630, 1215,  385,
    0,  260,  342,  357,  375,  356,    0,  998,  383,  349,
    0,  351,  365,  353,  367,   41,  356,   59,  442, 1010,
 1011,   59,  489,   41, 1321, 1264, 1338,  386,  381,  264,
 1298, 1299, 1275,   59,  374,  375,  318,   41, 1298, 1299,
  328,   59,  355,  383,  417,  385,  260, 1066,  345,    0,
 1298, 1299,  268,  374,  859,   59, 1301, 1298, 1299,   59,
  328,  495, 1881,  331, 1332,   59,  838, 1298, 1299,   59,
  466,  444, 1332, 1298, 1299,   16,  435, 1351,  429,  689,
  331,  318,  141, 1562, 1332,    0,  262,  269, 1523,  303,
  320, 1332,  264,  433, 1491,  300,  706,  386, 1776,  902,
 1345, 1332,  477,  340, 1669,  141,  453, 1332,   59,  277,
  300,   41,  722,  488,  309,  304,   41,  492,  387,  387,
  331,  264,  497,    0,  489,  439,   41, 1625,  396, 1148,
 1149,  595, 1151, 1152,    0,  318,  387,  477, 1249,  320,
  480,  481,  998,   84,   59,  396, 1038,  318,  488,  365,
  760,  304,  492,   59, 1010,  495,  348,  497,  417,  480,
  500,    0,    0,  517,   41,  357,  383,    0,  497,  340,
  803,  453,  782,  806,  495,   41,  411, 1069, 1070,  492,
  500,  741,   59,  386,  817,  318,  387,  431,  328,  386,
   41,  331,  452,   59,  596,  396, 1489, 1089,  551, 1281,
 1417,  458,   41, 1768, 1421,   44,  394,  340,   41,  644,
  386,  844,  577,  318,  458, 1160, 1327,  454,  318, 1516,
   59,   59,  264, 1520, 1521, 1787,   59,  458,  328,  440,
  458,  331,  447,  454,  886,  340,  576,  577,  458, 1555,
  340, 1121,  582,  454,    0,  598,  802,  387,  810,  756,
    0, 1003,  521,  521,  387, 1820,  396, 1569, 1503, 1556,
 1499,  410,  458,  396, 1227,  521,  596, 1258, 1287,  728,
  521,  895,  458, 1137,  387,  454,   42,   43,  286,   45,
    0,   47,  521,  454,  521,   41, 1006,  387,  653, 1854,
 1309,   41,  318, 1536,   44,  779,  396,  650,  651,  485,
  764, 1344,  873,   59,    0,  383,  436,  440,  454,   59,
  348, 1554,  779,  653,  340,  410,    0,  301,  409,  753,
  521,  454,  662,  409,   41,  517, 1453,  331,    0,  662,
  692,  502,  262,  458,  798,  799, 1574,  262,  328,   59,
  440,  331, 1641,  431,  954,   41,  501,  506,   44,  454,
  521,  440,  692,  260,  454,  397, 1653,   41,  431,  320,
  411,    0, 1659,   59,  729,  454,  731,  367,  733,  387,
  458,  436, 1045,  367,   41,   59,  716, 1674,  396, 1054,
  261,  521, 1725,  387,  724,  458, 1603,   59,  521,  729,
    0,  731,  396,  733, 1691, 1375,  736,  348,  773,  801,
  775,  741,   41,  458, 1902,   41,  278,  824,  521,  405,
  750,  262,   41,  753,  779,   44,  521,   44,  758,  784,
   59,  521,  762,  501,  402,  340,  264,  987,  454, 1511,
  990,   41,  753,  773,  836,  775, 1681,  458,  778,  792,
  793,  794, 1687, 1076,  784,  810,  402,  319, 1123,   59,
  440, 1343,  318,  461,  462, 1088,  386, 1725,  442, 1132,
  456,  386,  452,  340,  454, 1725,   93,  869,  870,  871,
  810,  801,  837,  838,  340,  436, 1109, 1725, 1775,  318,
 1307,  260, 1501, 1135, 1725,  318,  262, 1784,    0,  328,
  257,    0,  331,  322, 1725,  521,  849,  837,  838,  874,
 1725,  340, 1222,  521,  343,  322,  836,  340,  338,  884,
  417,  667,  262,  843,  280,   41,  672,  521,   44,  285,
    0,  264,  257,    0,  303, 1330,  856, 1824,  400,   41,
  508, 1336, 1403,  377,  874,  386, 1003,    0, 1778,  869,
  870,  871, 1262,  258,  884,  262, 1198,   59,  387,  313,
   59,   40,  301,  458,  387,  401,  896,  396,  343,  397,
    0,   41,  318,  396,   41,    0,  522,  334,  318,  441,
    0,  321,  328,    0,  340,  331,  383,  454,  328,   59,
  485,  331,   59,  368,  340,  335,    0, 1827,  454,  338,
  340,  306,  264,  343, 1790,  262,   59, 1616,  991,  334,
  397,  440, 1212,    0,  448, 1215,   41,  440,  328,  359,
  386,  331,  442,  452,   41,  454,  992,   44,  989,   59,
  265,  454,  318,  263,   59, 1865,  262,   41,    0,   59,
   44,  387,   59,  383,  318,  331,  386,  387, 1003,  436,
  396,  426, 1252, 1107,  340,   59,  396,  987,  988,  989,
  990,  991,  992,  417, 1512, 1427,  340,  302,  998,  435,
   40,  272,   59,  413,  383, 1082, 1083,  988,  411,  386,
 1010, 1011,  312,  470,    0, 1015, 1348,  288, 1805,  318,
 1045,    0,  521,  323,  440,  435, 1319,   59,  521,  383,
  440,  387, 1015,  442,    0,    0,  452,    0,  454, 1557,
  396,  340,  452,  418,  454, 1045,   42,   43,  318,   45,
 1837,   47,  405,   42, 1116, 1132,  308, 1350,   47,  386,
  440,  436,  333, 1403, 1403,  397,  318,  325,  319,  300,
  340, 1750, 1134,   59,  454,   44, 1755,    0, 1140,    0,
   59, 1351,  898,    0,  440,  265,  902,    0,  274,  258,
  386,  362, 1879,   59,   59,  470,   59,    0,  454, 1099,
 1125,    0,  271,  456, 1231,  521, 1607, 1132, 1101,   44,
  454,  521,    0, 1900, 1114,    0, 1903,  387,  385, 1800,
   41,  352,  302,  309,   93, 1125, 1116,  427, 1247, 1248,
 1249, 1918, 1132,   44,    0,  258,   59,  306,   59,  308,
    0, 1642,   59,  352, 1134,    0,   59, 1417,  271,  400,
 1140, 1421,  308,  383,    0,  454,   59,  337,  429, 1283,
   59, 1285,  318,  307,  308,  521,  260,  258,  340,  378,
  440,   59,  430, 1508,   59,   41,  436,  264,   44,  437,
  271,  318,   93,  306,  454,  308,   41, 1230,  262,   44,
  441,  328, 1693,   59,  331,  260,    0,  348,   44,   59,
  340,  453, 1227,  340,   59, 1230, 1231,  264,  494,  303,
   41,   42,   43,   59,   45,  306,   47,  308,  307,  308,
  307,  308, 1346,  318,    0,  411,   44, 1227,    0,    0,
 1230, 1231, 1525,  320, 1913,    0,  331,   41,  303, 1832,
   44, 1241,  342, 1243,  318,  340,  278,  321,  280,  418,
  387,  521,  342, 1846,  328,   59,  301,  331, 1258,  396,
  337,  335,   42,   43, 1264,   45,  340,   47, 1281,  343,
   41, 1564,    0,   44, 1780,   93,    0,  377,  264,   44,
  466,  405,  454,   59,  280,  359,  418,   59,   59,  285,
   61,  280,  387,  338,   59,  418,  285,  453,  264,  264,
  424,  396,  274,  440,  263,  405,  321,  451,  408,  383,
  461,  462,  386,  387,  454,  405,  521,  454,  408, 1825,
  335, 1914,  396,  410,  424, 1869, 1453,  418,  287,    0,
 1831,   59, 1325, 1603,  424,   59, 1880,  296,  387,  413,
  397, 1788,  259,  279,  359,  440,  378,  264, 1361,    0,
  450,  264, 1365,  270,   40,  278,  260,  280, 1864,  454,
  450,  435,  451, 1810,  451,  452,  440, 1494,  327,   42,
   43,  330,   45,  460,   47,  278,  264,  280,  452,  264,
  454, 1433, 1829, 1927,  521,  283,   59,    0,   59,    0,
 1896,  440,    0,  329,  493, 1688,  262,  318,  413,  303,
  424,    0,  489,  490,  503,  492,  461,  301,   59, 1724,
  331,  397,  499,  500,  501,  502,  503,    0,  505,  340,
  264,  508,  509,  510,  511,  512,  521,  363, 1453,  516,
  517,  397,  397, 1433,  402,  522,   44, 1752,  316,  317,
  436, 1441, 1442, 1443,  338,  381,   59,  521,   59,  280,
 1897,   59,  318, 1453,  285,  321,    0,  316,   42,   43,
   59,   45,  328,   47,  318,  331,  387,  426, 1783,  335,
  438,  430,  431,  432,  340,  396,   59,  343,   42,   43,
  397,   45,  328,   47,  397,  331,  384,  258,  465, 1489,
  467, 1526,  337,  359, 1494,    0,    0,  343, 1511, 1499,
  280, 1628,    0,  274,  460,  285,  356,  279, 1929,  397,
 1931,  321,  397,  407,  318,   59, 1643,  383,    0,  440,
  386,  387, 1641,  417,  328,  335, 1526,  331,  300,  300,
  396,  311, 1656,  454,  377,  306,  340,   41,  309,  343,
   44,  439,  501,  393,  336,  395,  505,  413,  442,  359,
   44,  279,  342, 1868,   59,   59,  452,  329,  456,   41,
    0,   59,   44,  328,  403,   59,  331,  378,  418,  435,
  417,  260,  300,  383,  440,  418,  348,   59,  343,    0,
 1672,  420,    0,  387,  356,  357,  452,  377,  454,  400,
  321,  363,  396,  443,  440,   41,  407,  444,   44,    0,
  521,  329,  411,  413,  335,    0,  452,  280,  454,  381,
    0,  402,  285, 1613,  303,  405,   44,    0,  408,   59,
  348,  393,  465,  395,  279, 1625,    0, 1673,  359,  357,
  441,   59, 1613, 1668,  424,  363,  440,  280,   59,    0,
  411,   59,  285, 1643,    0,  300,  418,  418,  452,  287,
  454,   41,  383,  381, 1637,  521,   42,   43,   59,   45,
  450,   47,  300, 1663,   59,  436, 1793,  340, 1668,   59,
 1797,  443, 1799, 1673,  329,  440,   59, 1769, 1805,    0,
   41, 1808,  413,   44, 1697,   59,  259,  452, 1701,  454,
 1742, 1692, 1692,  348,    0,  466,  280,  270,   59,  470,
  340,  285,  357,   59,  259,   40,  245, 1834,  363,  258,
 1837,    0,  261,  672,  253,  270,  280,  521,  493,  494,
   41,  285,    0,   44,    0,    0,  381,    0,    0,    0,
  689,  342,   42,   43,  693,   45,  320,   47,   59,    0,
    0,  700, 1742, 1743, 1871,  517,  705,  706,    0, 1876,
  320, 1744, 1879,   59,  258,    0,  715,  306,  717,  718,
  719,  720,  721,  722,  723,   41,  377,  271,   44,  402,
   59,  321,    0, 1900,  279,  279, 1903, 1778, 1778,    0,
 1805,   59,    0,   59,   59,  335,   59,   59,   59,  517,
 1241, 1918, 1243,  343,  405,  300,  300,  408,   59,   59,
 1800,  760,  306,   41,  308, 1805,   44,   59,  318,  359,
  337, 1811, 1837,  424,   59, 1816, 1816,   41,  321,   41,
   44,  383,   44,  782,  329,  329, 1827, 1827,  787,  788,
  337,   59,  335,  461,  462,  383,  402, 1837,   59,  450,
  343,   59, 1888,  348,  348,  391,  392,  374, 1849, 1849,
  506,  436,  357,  357, 1879,   93,  359, 1793,  363,  363,
  387, 1797,  517,  413, 1865, 1865,  291,  374,  434,   93,
   41,  361, 1808,   44,  299, 1900,  381,  381, 1903, 1879,
  387, 1881,  450,  449, 1885, 1885,   42,   43, 1888,   45,
 1898,   47,   46, 1918,  280,  384,  434, 1905, 1834,  285,
 1900,   46, 1902, 1903,   58,  417,   41,   61, 1908,   44,
  413,  318,  516,   58,  418,   46,   61,  278, 1918,  280,
   42,   43,   44,   45, 1932,   47,  287,   58,    0, 1929,
   61, 1931,    0,   41,  320, 1871,   44,  461,  462,   41,
 1876,  262,   44,  287,   42,   43,  469,   45,  260,   47,
   44,  910,  911,  912,   61,  516,  915,  916,  917,  372,
  919,  920,  921,  922,  923,  924,  286,   40,  927,  928,
  280,  930,  264,  932,   40,  285,  935,  936,  937,  938,
  939,  940,   41,  258,   40,   44,  262,   59,   40,  301,
   40,   59,  461,  952,   40,  954,  271,  318,  461,   41,
  321,  313,   44,   41,  279,   40,   44,  328,  318,   40,
  331,   40,  517,  517,  335,   41,   40,  378,   44,  340,
   41,   41,  343,   44,   44,  300,  338,  461,   41,   42,
   43,  306,   45,  308,   47,   40,   40,  258,  359,  400,
   41,   41,  318,   44,   44,  321,  407, 1006,   41,   40,
  271,   44,  328,   40,  329,  331,    0,   41,  279,  335,
   44,   40,  383,   44,  340,  386,  387,  343,   41,  342,
  688,   44,  690,  348,   91,  396,  694,  695,  696,  300,
  441,  356,  357,  359,   46,  306,   41,  308,  363,   44,
  342,   41,  413,   61,   44,  407,   40,   41,   42,   43,
   44,   45,   46,   47,  377,  417,  381,  383,  329,   41,
  386,  387,   44,   40,  435,   59,   60,   61,   62,  440,
  396,  264,  436,  320,  280,  377,   44,  348,  402,  285,
  442,  452,  405,  454,  320,  408,  357,  413,   44,  264,
  501,   44,  363,  418,   41,   42,   43,   91,   45,   93,
   47,  424,   46,  405,  429,  311,  408,   61,  280,  435,
  381,  428, 1628,  285,  440,   40,  915,  265,  917,  264,
  919,  920,  424,  922,  923,  924,  452,  450,  454,   44,
 1628,  348,  280,  442,  454,    0,  258,  285,   42,   43,
  258,   45,   44,   47,  278,  348,  348,  418,  450,  271,
  521, 1160,  278,  271,  302,   59,  264,  279,  300,  436,
  397,  279,  470, 1172, 1173,  397, 1175, 1176,   42,   43,
  277,   45,  458,   47, 1183,  348,   41,  447,  300,   44,
   42,   43,  300,   45,  306,   47,  308,  447,  306,  399,
  308,   40,  517, 1202,   59,  521,  349,   40,   91,   40,
   40, 1210, 1211, 1212,  318,  264, 1215,  329, 1217,  383,
  411,  329, 1221, 1222, 1223, 1224,    0,  280,   44,  457,
  458,  459,  285,    0,  389,    0,  348,  382,   40,   40,
  348,  451,   41,  277,   63,  357,  436,  299,  464,  357,
  317,  363,   40, 1252,  452,  363,  517,  485,  486,  448,
  264,  320,  320, 1262,  294,  382,  502,  505,   40,  381,
   44,  458,   40,  381,   41,  305,   41,   44,  262,   44,
  264,  265,  312,  274,  300,   59,  284, 1793,  272,  344,
  344, 1797,   59, 1799,   59,  278,  280,  386,  386,  386,
  418,  285, 1808, 1302,  288, 1793,  418,  337,  300, 1797,
  418, 1799,   40,  416,  356,   40,  418,  417,  302,  274,
 1808,   40,  417,  307,  308,  264,   41,  311, 1834,   41,
   42,   43,  320,   45,  318,   47,  320,  321,   44,   44,
  324,  446,  280,  280,  328,  271, 1834,  331,  285,  333,
   44,  335, 1351,  385,   44,  367,  340,  341,  320,  343,
   40,   44,   41,   42,   43, 1871,   45,  328,   47,  266,
 1876,   40,  402, 1372,  501,  359,  360,   40,  362,  264,
   41,   58, 1381, 1871,  422,   41,  280,   41, 1876, 1388,
   41,  285,  376,  264,  378,  379,  380,  440,  262,  383,
  394,  309,  386,  387,  434,  517,  454,  492,  438,  517,
  262,  309,  396,   41,   41,   40,  280,   40, 1417,    0,
  352,  285, 1421,  278,   40,  280,  410,   40,  280,  413,
   40,    0,  287,  285,   40,   40,   40,   40,   40, 1438,
   40,   40,   40,   40,   40,  429,   40,  262,  435,  308,
  327,  435,  436,   44,   41,   41,  440,  388,  388,  388,
   41,   42,   43,   44,   45,   46,   47,  451,  452,  343,
  454,  274,   41,  331,   44,   44,  460,  422,   59,   60,
   61,   62,  391,  390,  407,   41,   42,   43,   41,   45,
   59,   47,  287,  386,   41,   42,   43,  262,   45,    0,
   47, 1500,  386,  378,  386,  489,  490,  386,  492,   41,
   91,  278,   93,  280,  386,  499,  500,  501,  502,  503,
  287,  505,  278,  378,  508,  509,  510,  511,  512,  386,
   40,  417,  516,  517,   41,   41,   41,  521,  522,   41,
   41,   59,  308,   44,  435,  400,  293,  321,   59,   40,
  340,   59,  407,  318,  328,   59,  321,  331,   59,   59,
   59,  335,  340,  328,   59,  308,  331,   59,  280,  343,
  335,   59,   58,  285,  385,  340,  436,  280,  343,  396,
  337,  442,   41, 1582, 1583,  359,  441,  338,  442,   41,
   42,   43,  301,   45,  359,   47,  338,  301,   41,  352,
 1599,  280,  320,  385, 1603,   41,  285,  371,  371,  383,
   41,  378,   41, 1612,   41, 1614,  383,   41,  383,   41,
   41,  386,  387,   41,   42,   43,   41,   45,   41,   47,
   41,  396,   41,  400,   41,   41,   41,   41,   41,  413,
  407,    0,   41,  318,  418,   59,  501,  451,  413,   41,
   42,   43,   40,   45,   44,   47,   41,   41,   42,   43,
  352,   45,  344,   47,   40,   59,  440,  340,   46,  340,
  435,  508,   40,  368,  441,  440,   40,   44,  452,  410,
  454,  262,   41,  264,  265,   44,  383,  452,   41,  454,
  435,  272,  383,  343,  383,   41,   41, 1696,   59,  280,
   59,   44,   41,  304,  285, 1704,   44,  288,  301,  278,
  442,  280,  307,   41,   42,   43,  293,   45,  287,   47,
  387,  302,  304,  304,  280,    0,  307,  308,   40,  285,
  311,   40, 1731,  280,  501,   41,  463,  318,  285,  320,
  321,   44,   40,  324,   41,  337,    0,  328,  337,  383,
  331,  383,  333,   40,  335,  264,  521, 1756,  278,  340,
  341,  262,  343,  389,  411,  458,   41,   42,   43,   44,
   45,   46,   47,  451,  320,   44,   40,  271,  359,  360,
  309,  362,  470,  264,   59,   60,   61,   62,  271,  435,
  458,  436,  343,  343,  343,  376,  458,  378,  379,  380,
  343,   41,  383,   41,   59,  386,  387,   59, 1807,  378,
  308,   40,   59,   41,   40,  396,   91,  318,   93,  304,
  321,   41,   59,   59,   41,   41,    0,  328,  280,  410,
  331,  400,  413,  285,  335,    0,    0, 1244,  407,  340,
   12,  282,  343,  275,  274,  741, 1137,  657,  429, 1135,
  887, 1132, 1124,   88,  435,  436, 1544,  339,  359,  440,
 1545,  332,  280,  604,  322, 1114,  369,  285,  784, 1057,
  451,  452,  441,  454,   41,   42,   43, 1125,   45,  460,
   47,  351,  383,  351, 1608,  386,  387, 1663,  280, 1523,
 1525,  819, 1818,  285, 1821,  396,  280,  582, 1822,  810,
 1856,  285, 1731,  984,    0, 1441, 1633,  729,  489,  490,
 1806,  492,  413, 1277, 1040, 1778,  356, 1504,  499,  500,
  501,  502,  503, 1334,  505,  344,  584,  508,  509,  510,
  511,  512,  501,    0,  435,  516,  517,  337,  858,  440,
  521,  522,  866,  643,   13,   41,  309,  514,   44,  773,
  518,  452,  280,  454,  146,  787, 1756,  285, 1651,  318,
  760, 1849,  321,   59, 1015, 1885,  529, 1289,  749,  328,
 1174, 1007,  331, 1217,   41,  284,  335,   44,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,  262,   -1,  264,
  265,   -1,   59,   -1,   -1,   -1,   -1,  272,   -1,   -1,
  359,   -1,   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,
  521,   -1,   -1,    0,  383,   -1,   -1,  302,  387,    0,
   -1,   -1,  307,  308,   -1,   -1,  311,  396,   -1,   -1,
   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,  413,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   41,  340,  341,   44,  343,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,  440,   59,   -1,  359,  360,   -1,  362,   59,   60,
   61,   62,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   40,   -1,  383,    0,
   -1,  386,  387,  280,   -1,   -1,   -1,   -1,  285,   -1,
   91,  396,   93,   -1,   58,  472,  473,  474,  475,  476,
  477,  478,  479,   -1,   -1,  410,   -1,   -1,  413,   -1,
   -1,   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   41,  460,   -1,   44,  429,   -1,   -1,   -1,   -1,   -1,
  435,  436,  521,   -1,   -1,  440,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
  489,  490,  278,  492,  280,  460,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,  262,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,  522,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,  521,  522,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   41,   42,   43,  335,   45,
   -1,   47,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,  359,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   -1,   -1,  400,   -1,   -1,   -1,   -1,  280,
   -1,  407,   -1,   -1,  285,   -1,  383,  288,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,   -1,
  311,   -1,   -1,   -1,  321,  441,  413,  318,   -1,  320,
  321,  328,   -1,  324,  331,   -1,   -1,  328,  335,   -1,
  331,   -1,  333,   -1,  335,   -1,  343,   -1,  435,  340,
  341,  262,  343,  440,   41,   42,   43,   44,   45,   46,
   47,  280,  359,   -1,   -1,  452,  285,  454,  359,  360,
   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,  383,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   91,  396,   93,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,   -1,  413,  328,   -1,  410,
  331,   -1,  413,    0,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,    0,  521,   -1,   -1,   -1,  429,   -1,
   -1,   -1,   -1,  440,  435,  436,   -1,   -1,  359,  440,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
  451,  452,   -1,  454,   41,   -1,   -1,   44,   -1,  460,
   -1,   -1,  383,   -1,   41,  386,  387,   44,   -1,   -1,
   -1,   -1,   59,   -1,  280,  396,   -1,   -1,   -1,  285,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,  413,   -1,   -1,   -1,  460,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  435,  516,  517,   -1,   -1,  440,
  521,  522,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,  452,   -1,  454,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   41,   42,
   43,   -1,   45,  280,   47,   -1,   -1,   -1,  285,   -1,
   -1,  288,   41,   42,   43,   -1,   45,   -1,   47,   -1,
  521,    0,   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,
  307,  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,    0,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   40,   41,  340,  341,   44,  343,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   59,   -1,  359,  360,   -1,  362,   59,   60,   61,   62,
   -1,   -1,   -1,   41,   -1,  262,   44,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   40,  383,   -1,   -1,  386,
  387,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   93,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,
  287,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,  429,   -1,  321,   -1,   44,   -1,  435,  436,
   -1,  328,   -1,  440,  331,   -1,   -1,   -1,  335,  294,
   -1,   -1,   -1,  340,  451,  452,  343,  454,   -1,   -1,
  305,   -1,   -1,  460, 1373, 1374,   -1,  312,   -1,   -1,
 1379, 1380,  359, 1382, 1383,   -1,   -1,   -1,   -1,   -1,
  325, 1390, 1391, 1392, 1393,   -1, 1395, 1396, 1397, 1398,
 1399, 1400,  489,  490,   -1,  492,  383,   -1,   -1,  386,
  387,  378,  499,  500,  501,  502,  503,   -1,  505,  396,
   -1,  508,  509,  510,  511,  512,   -1,  280,   -1,  516,
  517,   -1,  285,  400,  521,  522,  413,   -1,   -1,   -1,
  407,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
   -1,  386,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,  402,   -1,   -1,
   -1,   -1,   -1,   -1,  441,  452,   -1,  454,   -1,  262,
   -1,  264,  265,   -1,  419,   -1,   -1,   -1,   -1,  272,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,  434,
   -1,   -1,  285,  438,   -1,  288,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,    0,   -1,   -1,   -1,  302,
  278,    0,  280,   -1,  307,  308,   -1,   -1,  311,  318,
   -1,   -1,  321,  468,   -1,  318,   -1,  320,  321,  328,
   -1,  324,  331,   -1,  521,  328,  335,    0,  331,   -1,
  333,  340,  335,   -1,  343,   -1,   41,  340,  341,   44,
  343,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
  359,   -1,   -1,   -1,   59,   -1,  359,  360,   -1,  362,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,  376,  383,  378,  379,  380,  387,   -1,
  383,   -1,   -1,  386,  387,   -1,   59,  396,   -1,   -1,
   -1,   -1,   -1,  396,   93,   -1,   -1,   -1,   -1,   -1,
  378,   -1,  320,   -1,  413,   -1,   -1,  410,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,  400,   -1,   -1,   -1,  429,   -1,   -1,  407,
   -1,  440,  435,  436,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,  451,  452,
   -1,  454,  417,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   41,   -1,  441,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,   -1,  460,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  521,  516,  517,   -1,   -1,    0,  521,  522,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  460,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   41,   42,
   43,   44,   45,  262,   47,  264,  265,   -1,   -1,   -1,
   -1,  489,  490,  272,  492,   -1,   59,   60,   61,   62,
   -1,  499,  500,  501,  502,  503,  285,  505,   -1,  288,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,  302,  522,  278,   -1,  280,  307,  308,
   93,  284,  311,  318,  287,   -1,  321,   -1,   -1,  318,
   -1,  320,  321,  328,   -1,  324,  331,  300,   -1,  328,
  335,   -1,  331,   -1,  333,  340,  335,   -1,  343,   -1,
   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  383,  378,
  379,  380,  387,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,  396,  278,   -1,
  280,   -1,   -1,   -1,   -1,  378,   -1,  287,  413,   -1,
   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  300,   -1,   -1,   -1,   -1,    0,   -1,  400,   -1,   -1,
  429,   -1,   -1,   -1,  407,  440,  435,  436,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   45,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   41,   -1,  441,   44,
   -1,   -1,   -1,   -1,   -1,   -1,  305,   -1,   -1,  262,
   -1,  264,  265,  312,   59,   -1,   -1,  316,  317,  272,
  489,  490,   -1,  492,   -1,   -1,  325,   -1,  378,   -1,
  499,  500,  501,  502,  503,  288,  505,   -1,  337,  508,
  509,  510,  511,  512,   -1,   -1,  521,  516,  517,  302,
  400,    0,  521,  522,  307,  308,   -1,  407,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,  441,   41,   42,   43,   44,   45,   -1,   47,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  359,  360,   -1,  362,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,  460,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,  434,   -1,   -1,   -1,  438,
   -1,   -1,   -1,  396,   93,   -1,   -1,   -1,   -1,   -1,
  449,  489,  490,   -1,  492,   -1,   -1,  410,   -1,   -1,
  413,  499,  500,  501,  502,  503,   -1,  505,   -1,  468,
  508,  509,  510,  511,  512,   -1,  429,   -1,  516,  517,
   -1,   -1,  435,  436,  522,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,    0,   40,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   41,  516,  517,   44,   -1,    0,  521,  522,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   59,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   43,   44,   45,  262,  359,  264,  265,   -1,   -1,   -1,
   -1,   -1,   -1,  272,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,  288,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,
   93,   -1,  311,   -1,   -1,   -1,   -1,   -1,  413,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
  435,  340,  341,   -1,  343,  440,   -1,   -1,   -1,   -1,
   -1,  458,   -1,  460,   -1,   -1,   -1,  452,   -1,  454,
  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,  489,  490,  383,  492,   -1,  386,  387,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  396,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  410,   -1,   -1,  413,  522,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,  521,   -1,   -1,   -1,
  429,   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  278,
   -1,  280,  451,  452,   -1,  454,   -1,   -1,  287,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,  300,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  264,  265,   -1,   59,   -1,   -1,   -1,   -1,  272,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,  288,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,  302,
   -1,    0,  521,  522,  307,  308,   -1,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,    0,  331,  378,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,
   -1,  400,   -1,   -1,   -1,   -1,  359,  360,  407,  362,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,  460,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   59,   -1,   61,   -1,
   -1,   -1,  441,  396,   93,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,  410,   -1,   -1,
  413,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  429,   -1,  516,  517,
   -1,   -1,  435,  436,  522,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,    0,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   41,  516,  517,   44,   -1,    0,  521,  522,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   59,   -1,   41,  328,   -1,   44,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   43,   44,   45,  262,  359,  264,  265,   -1,   -1,   -1,
   -1,   -1,   -1,  272,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,  383,  288,
   -1,  386,  387,   -1,   -1,   -1,   42,   -1,   -1,   -1,
   -1,  396,   -1,  302,  277,  278,   -1,  280,  307,  308,
   93,  284,  311,   -1,  287,   -1,   -1,   -1,  413,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,  300,   -1,  328,
   -1,   -1,  331,  306,  333,   -1,  335,   -1,   -1,   -1,
  435,  340,  341,   -1,  343,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,  452,   -1,  454,
  359,  360,   -1,  362,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,  396,   44,   -1,
   -1,   -1,   -1,   -1,   -1,  378,   41,   -1,   -1,   44,
   -1,  410,   -1,   59,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,  521,  400,   -1,   -1,
  429,   -1,   -1,   -1,  407,   -1,  435,  436,   -1,   -1,
   -1,  440,   -1,  416,   -1,  418,   -1,   -1,   -1,  278,
   -1,  280,  451,  452,   -1,  454,   -1,   -1,  287,   -1,
   -1,  460,   -1,  436,   -1,   -1,   -1,   -1,  441,  278,
   -1,  280,   -1,   -1,   -1,   -1,   -1,   -1,  287,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,
  489,  490,   -1,  492,   -1,   -1,   -1,  470,   -1,   -1,
  499,  500,  501,  502,  503,  288,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,  302,
   -1,    0,  521,  522,  307,  308,   -1,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,  378,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   41,   -1,   43,   44,   45,   -1,   -1,  378,
   -1,  400,   -1,   -1,   -1,   -1,  359,  360,  407,  362,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  400,   -1,  376,   -1,  378,  379,  380,  407,   -1,
  383,    0,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  441,  396,   93,   -1,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,
  413,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,  429,   -1,   -1,   -1,
   -1,   -1,  435,  436,   -1,   -1,   -1,  440,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,
   -1,  454,  318,   -1,   -1,  321,   -1,  460,   -1,   -1,
   -1,   -1,  328,  318,   -1,  331,  321,   -1,   -1,  335,
   -1,   -1,   -1,  328,  340,   -1,  331,  343,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,  489,  490,  343,  492,
   -1,   -1,   -1,  359,  460,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,  359,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,  383,  521,  522,
  386,  387,   -1,  489,  490,   -1,  492,   -1,  383,   -1,
  396,  386,  387,  499,  500,  501,  502,  503,   42,  505,
   -1,  396,  508,  509,  510,  511,  512,  413,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,   -1,  413,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  435,
   -1,   -1,   -1,  272,  440,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,  452,    0,  454,  288,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,
   -1,   -1,  311,   -1,   58,   -1,   -1,   -1,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   40,   41,  328,
   -1,   44,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,  264,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  521,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   -1,   -1,  521,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,  318,
   -1,   -1,  321,   -1,   -1,   -1,  261,   -1,   -1,  328,
   -1,  410,  331,   -1,  413,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,  282,   -1,   -1,
  429,   -1,   -1,   -1,   -1,  290,  435,  436,   -1,   -1,
  359,  440,  297,   -1,  299,   -1,  301,   -1,   -1,   -1,
   -1,  306,  451,  452,   -1,  454,   -1,   -1,  313,   -1,
   -1,  460,   -1,   -1,  383,   -1,   -1,   -1,  387,   -1,
   -1,   -1,  327,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,  338,    0,   -1,   -1,   -1,   -1,   -1,
  489,  490,  296,  492,  413,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,    0,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,  440,  521,  522,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   59,   -1,   -1,   -1,   -1,   41,   -1,
  405,   44,   -1,   -1,   -1,   -1,   -1,  412,   -1,   -1,
  415,   -1,  417,  418,   -1,   -1,   59,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,  442,   -1,  508,
  509,  510,  511,  512,    0,   -1,   -1,  516,  517,   -1,
   -1,   -1,  521,  522,   -1,  318,   -1,   -1,  321,   -1,
   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   41,  491,   -1,   44,   -1,
   -1,   59,   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,
   -1,  506,  507,   59,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   -1,  489,  490,   -1,  492,   -1,
    0,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
  413,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,  460,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,  452,
   -1,  454,   -1,   -1,   -1,  489,  490,  460,  492,   59,
   60,   61,   62,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,  262,
   -1,   -1,  516,  517,   -1,   -1,  489,  490,  522,  492,
   -1,   91,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  318,  516,  517,  321,   -1,   -1,  521,  522,
   -1,   -1,  328,   -1,   -1,  331,   -1,    0,   -1,  335,
   -1,   -1,   -1,   -1,  340,  318,   -1,  343,  321,   -1,
   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,  359,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,  262,   -1,   41,   42,
   43,   44,   45,   46,   47,   -1,  359,  383,   -1,   -1,
   -1,  387,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
  383,   -1,   -1,  386,  387,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   91,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
  413,   -1,  328,    0,  440,  331,   -1,   -1,   -1,  335,
   41,   -1,   -1,   44,  340,   -1,  452,  343,  454,   -1,
   -1,   -1,  435,   -1,  460,   -1,   -1,  440,   59,   -1,
   -1,   -1,  262,  359,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,  272,   -1,   41,   -1,   -1,   44,   -1,   -1,
  280,   -1,   59,  489,  490,  285,  492,  383,  288,   -1,
  386,  387,   59,  499,  500,  501,  502,  503,   -1,  505,
  396,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,  521,  522,  413,  318,   -1,
   -1,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,  521,  435,
  340,  341,   -1,  343,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,  452,   -1,  454,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,  489,  490,  383,  492,   -1,  386,  387,   -1,  262,
   -1,  499,  500,  501,  502,  503,  396,  505,   -1,  272,
  508,  509,  510,  511,  512,   -1,   -1,  280,  516,  517,
   -1,   -1,  285,  413,  522,  288,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  521,   -1,   -1,   -1,  429,
   -1,    0,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,  324,  452,   -1,  454,  328,   -1,   -1,  331,   -1,
  333,  262,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   59,   60,   61,   62,    0,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,  318,   -1,   -1,
  321,  521,   91,  396,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   41,   -1,   -1,   44,  340,
  413,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   59,  321,   -1,  429,   -1,  359,   -1,
   -1,  328,  435,   -1,  331,   -1,   -1,  440,  335,   -1,
   -1,   -1,   -1,  340,    0,   -1,  343,   -1,   -1,  452,
   -1,  454,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,  396,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   41,  383,   -1,   44,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   59,  435,   -1,   -1,   -1,   41,  440,
   -1,   44,   -1,   -1,   -1,   -1,  413,   -1,  521,   -1,
   -1,  452,   -1,  454,   -1,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  280,  489,  490,   -1,  492,  285,   -1,   -1,  288,
  521,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,  318,
   -1,   -1,  321,   -1,  521,  324,  262,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,  287,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,  318,   -1,  383,  321,   58,  386,  387,   -1,
   -1,   63,  328,   -1,   -1,  331,   -1,  396,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,  262,  343,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,  262,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
  396,   -1,  318,   -1,   -1,  321,   -1,   -1,   40,   -1,
   -1,   43,  328,   45,   -1,  331,   -1,  413,   -1,  335,
   -1,   -1,   -1,   -1,  340,  318,   58,  343,  321,   -1,
   -1,   63,   -1,   -1,   -1,  328,   -1,   -1,  331,  435,
   -1,   -1,  335,  359,  440,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,  460,
   -1,   -1,  521,   -1,   -1,   -1,  359,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
  383,  492,   -1,  386,  387,   -1,   -1,  413,  499,  500,
  501,  502,  503,  396,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,  435,
  413,  522,   -1,   -1,  440,  521,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,  452,   -1,  454,   -1,
   -1,   -1,  435,   -1,  276,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,  452,
   -1,  454,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  521,   -1,   -1,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,  521,   -1,
   -1,   -1,   -1,   -1,  366,  267,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,  276,   -1,  378,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
  292,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  314,   -1,   -1,  417,   -1,   58,   -1,  421,
   -1,  323,   63,   -1,  326,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  471,
   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,  513,  514,  515,  516,  517,   -1,   -1,   -1,  421,
  522,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,  295,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,  323,   -1,  260,
  522,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   40,   -1,
   42,   43,  303,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,  352,   -1,  354,  355,   -1,   -1,   60,   61,   62,
   -1,  427,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  457,  458,  459,   -1,  461,  462,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,   -1,   -1,   -1,  427,   -1,   -1,  495,
  496,   -1,  433,   -1,   -1,   -1,   -1,   -1,  504,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,  260,   -1,
   -1,  522,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,  303,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,
   -1,  323,  285,   -1,  326,  288,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  352,  267,  354,  355,   40,   -1,   -1,   43,   -1,   45,
  276,  324,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
  333,   -1,   58,  289,   -1,   -1,   -1,   63,  341,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,  314,  362,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  443,  287,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,  457,  458,  459,  460,  461,
  462,   -1,  378,   -1,   -1,   -1,  429,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  421,  508,  509,  510,  511,
  512,  427,   -1,   -1,  516,  517,   -1,  433,   -1,   -1,
  522,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,  267,  498,  499,  500,  501,  502,  503,  504,  505,
  276,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   40,  289,   -1,   43,  522,   45,   -1,  295,
  296,   -1,   -1,   -1,   -1,  460,   42,   43,   -1,   45,
   58,   47,   -1,   -1,   -1,   63,   -1,   -1,  314,   -1,
   -1,   -1,   -1,   -1,   60,   61,   62,  323,   -1,   -1,
  326,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  354,  355,
   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,  267,
  516,  517,   -1,   -1,   -1,  443,  522,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,
   -1,  289,  460,   -1,  280,   -1,   -1,  295,  296,  285,
   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,  323,   -1,   -1,  326,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,  324,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  333,  516,  517,
   -1,   -1,   -1,   -1,  522,  341,  354,  355,   -1,   -1,
   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,  366,  267,
   -1,   -1,  370,   -1,  360,   -1,  362,   -1,  276,   -1,
  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,  289,  378,  379,  380,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,  421,   -1,  323,   63,   -1,  326,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,  260,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,  522,   -1,   -1,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,  301,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,  313,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,  338,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,  260,  522,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,  407,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,  417,   -1,   -1,   -1,   -1,  295,  296,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   58,
   -1,   -1,   -1,   -1,   63,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  366,
   -1,   -1,   -1,  370,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  328,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  260,   -1,   -1,  263,  522,   -1,    0,  267,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,  460,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,  323,   -1,   59,  326,   61,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,  522,   -1,  354,  355,   -1,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,  323,   -1,   -1,  326,  425,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,  522,  425,  258,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,  277,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,
  471,   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  260,   -1,   -1,
  263,  522,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  378,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,   -1,
   -1,   -1,   -1,   58,  407,   -1,   -1,   -1,   63,   -1,
  323,   -1,   -1,  326,   -1,  418,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  273,   -1,  366,   -1,  277,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,   -1,  291,
   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  427,   -1,   -1,  339,   -1,   -1,
  433,   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,
  443,  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,  406,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,  423,   -1,  516,  517,  260,   -1,   -1,  263,  522,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,  480,  481,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,  291,
   -1,  326,   -1,  295,   -1,  497,  298,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,  260,    0,   -1,  263,   -1,   -1,   -1,
  267,  366,   -1,   -1,   -1,  370,   -1,  339,   -1,  276,
   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   40,   -1,   -1,   43,   41,   45,   -1,   44,   -1,
   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   59,   63,   61,  323,   -1,   -1,  326,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,  443,  411,
   -1,   -1,   -1,   -1,   -1,   91,   -1,  354,  355,   -1,
   -1,  423,  457,  458,  459,  460,  461,  462,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,  445,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,    0,   -1,  508,  509,  510,  511,  512,  480,  481,
   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  425,   -1,
  427,   -1,   -1,   -1,   -1,  497,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  343,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,  368,
   59,   -1,   61,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  260,  258,   -1,  263,  522,   -1,  426,  267,   -1,
   -1,   40,   -1,   42,   43,   -1,   45,  276,  274,   -1,
   -1,  277,  278,   -1,  280,   -1,   -1,   -1,  284,   58,
  289,  287,   -1,   -1,   63,   -1,  295,  296,   -1,   -1,
   -1,  460,   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,
  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  354,  355,  516,  517,   -1,
   -1,   -1,   -1,  522,   -1,   -1,   -1,  366,   -1,   40,
   -1,  370,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  400,   -1,   -1,   -1,   -1,  258,
   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,
   -1,   -1,  418,   -1,   -1,   -1,  425,   -1,  427,  278,
   -1,  280,   -1,   -1,  433,  284,   -1,   -1,  287,   -1,
  436,   -1,   -1,   -1,  443,  441,   -1,   -1,   -1,   -1,
   -1,  300,   -1,   -1,   -1,   -1,   -1,  306,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
  466,   -1,  471,   -1,  470,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,  267,  498,
  499,  500,  501,  502,  503,  504,  505,  276,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
  289,   -1,   -1,  522,   -1,   -1,  295,  296,   -1,  378,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  400,   -1,   -1,  323,   -1,   -1,  326,  407,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  418,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,  436,   -1,   -1,
   -1,   -1,  441,   -1,   -1,   -1,  267,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,  276,   -1,   -1,   40,   -1,
   42,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,
   -1,  470,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,  352,   -1,  354,  355,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,  260,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  417,  516,  517,   -1,
   -1,   -1,   -1,  522,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  301,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  313,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,  327,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,  338,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,  516,  517,  460,   -1,   -1,
   -1,  522,   -1,   -1,   -1,   40,   -1,  289,   43,   -1,
   45,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,  407,  489,  490,   63,  492,
   -1,   -1,   -1,   -1,   -1,  417,  499,  500,  501,  502,
  503,  323,  505,   -1,  326,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,
  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   58,
  492,   -1,   -1,   -1,   63,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,  271,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
  522,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,  301,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,  312,  313,   -1,  315,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,  332,   -1,   -1,   -1,  471,
   -1,  338,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  260,  508,  509,  510,  511,
  512,   -1,  267,   -1,  516,  517,   -1,   -1,   -1,   -1,
  522,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,
  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,  267,  354,
  355,   40,   -1,   -1,   43,   -1,   45,  276,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   58,
  289,   -1,  489,  490,   63,  492,  295,  296,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  518,  519,  520,  323,  522,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,  271,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,  427,   -1,
   -1,  516,  517,   -1,  433,   -1,  301,  522,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,  312,  313,   -1,
  315,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,  332,   -1,   -1,
   -1,   -1,  471,  338,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,  267,  498,
  499,  500,  501,  502,  503,  504,  505,  276,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   40,
  289,   -1,   43,  522,   45,   -1,  295,  296,   -1,   -1,
   -1,  460,   -1,  398,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  323,   -1,   -1,  326,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,  432,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,  442,   -1,  508,
  509,  510,  511,  512,   -1,  354,  355,  516,  517,   -1,
   -1,   -1,   -1,  522,   -1,  460,   -1,  366,   -1,   40,
   -1,  370,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  328,   -1,
   -1,  516,  517,  518,  519,  520,   -1,  522,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  451,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,  402,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  267,  516,  517,   -1,
   -1,   -1,   -1,  522,   -1,  276,   -1,   -1,  438,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,  289,  449,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  468,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,  267,   -1,  508,  509,
  510,  511,  512,  354,  355,  276,  516,  517,   40,   41,
   -1,   43,  522,   45,   -1,  366,   -1,   -1,  289,  370,
   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  352,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  427,  508,  509,  510,
  511,  512,  433,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,  522,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,  434,  276,  516,  517,   40,   -1,   -1,
   43,  522,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,  460,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,  354,  355,   -1,  516,  517,   40,   -1,   -1,
   43,  522,   45,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  460,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  469,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,  483,  484,   -1,   -1,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,   -1,  499,  500,  501,
  502,  503,  504,  505,  506,  427,  508,  509,  510,  511,
  512,  433,   -1,   -1,  516,  517,  518,  519,  520,  521,
  522,  443,   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,  471,
  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,  339,   -1,  276,  516,  517,   -1,  345,  346,  347,
  522,  349,  350,  351,   -1,  353,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,  276,   -1,  423,   40,   -1,   -1,   43,
   -1,   45,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,  445,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  497,
  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,  417,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  427,  508,  509,  510,  511,  512,
  433,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  522,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  516,  517,   40,   -1,   -1,   43,  522,
   45,   -1,   -1,   -1,  460,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   58,   59,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  323,
   -1,   -1,  326,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,  460,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
  484,   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,   -1,  499,  500,  501,  502,  503,
  504,  505,  506,  427,  508,  509,  510,  511,  512,  433,
   -1,   -1,  516,  517,  518,  519,  520,  521,  522,  443,
   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
  454,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,  293,   -1,  295,   -1,  471,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,  339,
   -1,  276,  516,  517,   -1,  345,  346,  347,  522,  349,
  350,  351,   -1,  353,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,  276,   -1,  423,   40,   -1,   -1,   43,   -1,   45,
   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,
  295,  296,   58,   -1,   -1,  445,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,  497,  433,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  427,  508,  509,  510,  511,  512,  433,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,  522,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,    0,  508,  509,  510,  511,  512,   -1,   -1,
  276,  516,  517,   -1,    0,   -1,   -1,  522,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,    0,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   41,   -1,  323,   44,   -1,
  326,   59,   -1,   61,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,  354,  355,
   -1,    0,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,
  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  258,   -1,   -1,   -1,  274,   -1,   -1,  277,
  278,   -1,  280,   -1,  258,   -1,  284,   -1,  274,  287,
   -1,  277,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,
  274,  287,  300,   -1,  278,   -1,  280,   -1,  306,   -1,
  284,  309,   -1,  287,  300,   -1,   -1,   -1,   -1,   -1,
  306,   -1,   -1,  309,   -1,   -1,  300,   -1,   -1,   -1,
   -1,   -1,  306,   -1,   -1,  309,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,
   -1,   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,  274,  287,   -1,   -1,  278,
   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,  300,
  378,   -1,   -1,   -1,   -1,  306,   -1,  271,  309,   -1,
   -1,  300,  378,   -1,   -1,   -1,   -1,  306,   -1,   -1,
  309,   -1,  400,   -1,  378,   -1,   -1,   -1,   -1,  407,
   -1,   -1,   -1,  411,  400,   -1,   -1,  301,   -1,   -1,
  418,  407,   -1,   -1,  308,  411,  400,   -1,  312,  313,
   -1,  315,  418,  407,  318,   -1,   -1,  411,  436,   -1,
   -1,   -1,   -1,  441,  418,   -1,   -1,   -1,  332,   -1,
  436,   -1,   -1,   -1,  338,  441,   -1,  378,   -1,   -1,
   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,  466,  378,
   -1,   -1,  470,   -1,   -1,   -1,   -1,   -1,   -1,  400,
  466,   -1,   -1,   -1,  470,   -1,  407,   -1,   -1,   -1,
  411,  400,  466,   -1,   -1,   -1,  470,  418,  407,   -1,
   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,
   -1,   -1,  273,   -1,  398,  436,  277,   -1,   -1,   -1,
  441,   -1,   -1,  284,   -1,   -1,   -1,  436,   -1,   -1,
   -1,   -1,  441,  417,  295,   -1,   -1,  298,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  466,   -1,   -1,  432,  470,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  466,  442,   -1,
   -1,  470,   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,
   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,  339,   -1,
   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,  373,   -1,  375,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  518,  519,  520,   -1,  522,   -1,
   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   -1,  273,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  295,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,  339,   -1,   -1,  516,  517,   -1,  345,  346,
  347,  522,  349,  350,  351,   -1,  353,  273,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  273,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,  295,
   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,  295,   -1,
   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  339,   -1,   -1,  423,   -1,   -1,  345,
  346,  347,  339,  349,  350,  351,   -1,  353,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,   -1,   -1,  460,   -1,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
  406,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,  406,
  497,   -1,  499,  500,  501,  502,  503,  423,  505,   -1,
   -1,  508,  509,  510,  511,  512,  423,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  522,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
   -1,  497,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  497,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 522
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
"AS","ASC","AT","AVG","AUTO","BASENAME","BEFORE","BEGIN","BETWEEN","BLOB","BY",
"CACHE","CAST","CHARACTER","CHECK","CHECK_POINT_LEN","COLLATE","COMMA","COMMIT",
"COMMITTED","COMPUTED","CONCATENATE","CONDITIONAL","CONSTRAINT","CONTAINING",
"COUNT","CREATE","CSTRING","CURRENT","CURSOR","DATABASE","DATE","DB_KEY",
"KW_DEBUG","DECIMAL","DECLARE","DEFAULT","KW_DELETE","DESC","DISTINCT","DO",
"DOMAIN","DROP","ELSE","END","ENTRY_POINT","EQL","ESCAPE","EXCEPTION","EXECUTE",
"EXISTS","EXIT","EXTERNAL","FILTER","FOR","FOREIGN","FROM","FULL","FUNCTION",
"GDSCODE","GEQ","GENERATOR","GEN_ID","GRANT","GROUP","GROUP_COMMIT_WAIT","GTR",
"HAVING","IF","KW_IN","INACTIVE","INNER","INPUT_TYPE","INDEX","INSERT",
"INTEGER","INTO","IS","ISOLATION","JOIN","KEY","KW_CHAR","KW_DEC","KW_DOUBLE",
"KW_FILE","KW_FLOAT","KW_INT","KW_LONG","KW_NULL","KW_NUMERIC","KW_UPPER",
"KW_VALUE","LENGTH","LOGFILE","LPAREN","LEFT","LEQ","LEVEL","LIKE",
"LOG_BUF_SIZE","LSS","MANUAL","MAXIMUM","MAX_SEGMENT","MERGE","MESSAGE",
"MINIMUM","MODULE_NAME","NAMES","NATIONAL","NATURAL","NCHAR","NEQ","NO","NOT",
"NOT_GTR","NOT_LSS","NUM_LOG_BUFS","OF","ON","ONLY","OPTION","OR","ORDER",
"OUTER","OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","KW_PAGE_SIZE","PARAMETER",
"PASSWORD","PLAN","POSITION","POST_EVENT","PRECISION","PRIMARY","PRIVILEGES",
"PROCEDURE","PROTECTED","RAW_PARTITIONS","READ","REAL","REFERENCES","RESERVING",
"RETAIN","RETURNING_VALUES","RETURNS","REVOKE","RIGHT","RPAREN","ROLLBACK",
"SEGMENT","SELECT","SET","SHADOW","KW_SHARED","SINGULAR","KW_SIZE","SMALLINT",
"SNAPSHOT","SOME","SORT","SQLCODE","STABILITY","STARTING","STATISTICS",
"SUB_TYPE","SUSPEND","SUM","TABLE","THEN","TO","TRANSACTION","TRIGGER",
"UNCOMMITTED","UNION","UNIQUE","UPDATE","USER","VALUES","VARCHAR","VARIABLE",
"VARYING","VERSION","VIEW","WAIT","WHEN","WHERE","WHILE","WITH","WORK","WRITE",
"FLOAT_NUMBER","NUMBER","NUMERIC","SYMBOL","STRING","INTRODUCER","ACTION",
"ADMIN","CASCADE","FREE_IT","RESTRICT","ROLE","COLUMN","TYPE","EXTRACT","YEAR",
"MONTH","DAY","HOUR","MINUTE","SECOND","WEEKDAY","YEARDAY","TIME","TIMESTAMP",
"CURRENT_DATE","CURRENT_TIME","CURRENT_TIMESTAMP","NUMBER64BIT","SCALEDINT",
"CURRENT_USER","CURRENT_ROLE","KW_BREAK","SUBSTRING","RECREATE","KW_DESCRIPTOR",
"FIRST","SKIP","CURRENT_CONNECTION","CURRENT_TRANSACTION","BIGINT","CASE",
"NULLIF","COALESCE","USING","NULLS","LAST","ROW_COUNT","LOCK","SAVEPOINT",
"RELEASE","STATEMENT","LEAVE","INSERTING","UPDATING","DELETING","KW_INSERTING",
"KW_UPDATING","KW_DELETING","BACKUP","KW_DIFFERENCE","OPEN","CLOSE","FETCH",
"ROWS","BLOCK",
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
"arg_desc : init_data_type udf_data_type",
"arg_desc : init_data_type udf_data_type BY KW_DESCRIPTOR",
"return_value1 : return_value",
"return_value1 : '(' return_value ')'",
"return_value : init_data_type udf_data_type",
"return_value : init_data_type udf_data_type FREE_IT",
"return_value : init_data_type udf_data_type BY KW_VALUE",
"return_value : init_data_type udf_data_type BY KW_DESCRIPTOR",
"return_value : PARAMETER pos_short_integer",
"filter_decl_clause : symbol_filter_name INPUT_TYPE blob_subtype OUTPUT_TYPE blob_subtype ENTRY_POINT sql_string MODULE_NAME sql_string",
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
"db_rem_option : db_log",
"db_rem_option : db_log_option",
"db_rem_option : DEFAULT CHARACTER SET symbol_character_set_name",
"db_rem_option : KW_DIFFERENCE KW_FILE sql_string",
"db_log_option : GROUP_COMMIT_WAIT equals long_integer",
"db_log_option : CHECK_POINT_LEN equals long_integer",
"db_log_option : NUM_LOG_BUFS equals pos_short_integer",
"db_log_option : LOG_BUF_SIZE equals unsigned_short_integer",
"db_log : db_default_log_spec",
"db_log : db_rem_log_spec",
"db_rem_log_spec : LOGFILE '(' logfiles ')' OVERFLOW logfile_desc",
"db_rem_log_spec : LOGFILE BASENAME logfile_desc",
"db_default_log_spec : LOGFILE",
"db_file : file1 sql_string file_desc1",
"logfiles : logfile_desc",
"logfiles : logfiles ',' logfile_desc",
"logfile_desc : logfile_name logfile_attrs",
"logfile_name : sql_string",
"logfile_attrs :",
"logfile_attrs : logfile_attrs logfile_attr",
"logfile_attr : KW_SIZE equals long_integer",
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
"proc_inputs : rhs_list",
"proc_inputs : '(' rhs_list ')'",
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
"union_view_expr : union_view_expr UNION select_view_expr",
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
"db_alter_clause : DROP LOGFILE",
"db_alter_clause : SET db_log_option_list",
"db_alter_clause : ADD db_log",
"db_alter_clause : ADD KW_DIFFERENCE KW_FILE sql_string",
"db_alter_clause : DROP KW_DIFFERENCE KW_FILE",
"db_alter_clause : BEGIN BACKUP",
"db_alter_clause : END BACKUP",
"db_log_option_list : db_log_option",
"db_log_option_list : db_log_option_list ',' db_log_option",
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
"union_expr : union_expr UNION select_expr",
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
"select_item : rhs",
"select_item : rhs as_noise symbol_item_alias_name",
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
"table_proc_inputs : '(' rhs_list ')'",
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
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' rhs_list ')'",
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
"assignment : update_column_name '=' rhs",
"rhs : value",
"rhs : null_value",
"rhs_list : rhs",
"rhs_list : rhs_list ',' rhs",
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
"cast_specification : CAST '(' rhs AS data_type_descriptor ')'",
"case_expression : case_abbreviation",
"case_expression : case_specification",
"case_abbreviation : NULLIF '(' value ',' value ')'",
"case_abbreviation : COALESCE '(' rhs ',' rhs_list ')'",
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
"case_result : rhs",
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
		dsql_str* str_ = FB_NEW_RPT(*DSQL_permanent_pool, symbol->sym_length) dsql_str;
		str_->str_length = symbol->sym_length;
		strncpy((char*)str_->str_data, (char*)symbol->sym_string, symbol->sym_length);
		symbol->sym_object = (void *) str_;
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

	for (const char* p = ((dsql_str*) string)->str_data; classes[*p] & CHR_DIGIT; p++)
	{
		if (!(classes[*p] & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol ((char *)((dsql_str*) string)->str_data);

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
	TSQL tdsql = GET_THREAD_DATA;

	if (field_name == NULL)
	{
		dsql_fld* field =
			FB_NEW_RPT(*tdsql->tsql_default, sizeof (INTERNAL_FIELD_NAME)) dsql_fld;
		strcpy (field->fld_name, (TEXT*) INTERNAL_FIELD_NAME);
		return field;
	}
	const dsql_str* string = (dsql_str*) field_name->nod_arg[1];
	dsql_fld* field =
		FB_NEW_RPT(*tdsql->tsql_default, strlen ((SCHAR*) string->str_data)) dsql_fld;
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
	TSQL tdsql = GET_THREAD_DATA;
		   
	dsql_fil* temp_file = FB_NEW(*tdsql->tsql_default) dsql_fil;

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
	TSQL tdsql = GET_THREAD_DATA;

	if (!node)
		return node;

	dsql_lls* stack = 0;
	stack_nodes (node, &stack);
	USHORT l = 0;
	{for (const dsql_lls* temp = stack; temp; temp = temp->lls_next)
		l++;
	}

	dsql_nod* old  = node;
	node = FB_NEW_RPT(*tdsql->tsql_default, l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	dsql_nod** ptr = node->nod_arg + node->nod_count;

	while (stack)
		*--ptr = (dsql_nod*) LLS_POP (&stack);

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
	TSQL tdsql = GET_THREAD_DATA;

	dsql_nod* node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
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
	TSQL tdsql = GET_THREAD_DATA;

	dsql_nod* node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	VA_START (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

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
	TSQL tdsql = GET_THREAD_DATA;

	dsql_nod* node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	VA_START (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

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

	for (char* p = ((dsql_str*) string)->str_data; classes[*p] & CHR_DIGIT; p++)
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
						 dsql_lls** stack)
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
		LLS_PUSH (node, stack);
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

		stack_nodes( next_node, stack);

		/* stack the non-list nodes and reverse the chain on the way back */
		
		curr_node = end_chain;
		while (true)
		{
			LLS_PUSH( curr_node->nod_arg[1], stack);
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
		for (; ptr < end && classes[*ptr] & CHR_IDENT; ptr++)
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
		((c == '.') && (ptr < end) && (classes[*ptr] & CHR_DIGIT)))
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
		for (; ptr < end && classes[*ptr] & CHR_IDENT; ptr++)
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
					switch(sym->sym_keyword) {
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
						switch(nextToken) {
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
							  yyvsp[-1], NULL); }
break;
case 87:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				yyvsp[-3], MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG)); }
break;
case 89:
{ yyval = yyvsp[-1]; }
break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((dsql_str*) FUN_reference, CONSTANT_SLONG));}
break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((dsql_str*) (-1 * FUN_reference), CONSTANT_SLONG));}
break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((dsql_str*) FUN_value, CONSTANT_SLONG));}
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG));}
break;
case 94:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
break;
case 95:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 96:
{ yyval = yyvsp[0]; }
break;
case 97:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 98:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 99:
{ yyval = yyvsp[0]; }
break;
case 100:
{ yyval = yyvsp[0]; }
break;
case 101:
{ yyval = yyvsp[0]; }
break;
case 102:
{ yyval = yyvsp[0]; }
break;
case 103:
{ yyval = yyvsp[0]; }
break;
case 104:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 116:
{ yyval = NULL; }
break;
case 117:
{ yyval = make_list (yyvsp[0]); }
break;
case 119:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 120:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 121:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 122:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 123:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 124:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 125:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 126:
{ yyval = (dsql_nod*) 0;}
break;
case 127:
{ yyval = yyvsp[-1]; }
break;
case 128:
{ yyval = NULL; }
break;
case 131:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 132:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 133:
{ yyval = NULL; }
break;
case 134:
{ yyval = NULL; }
break;
case 135:
{ yyval = yyvsp[0]; }
break;
case 136:
{ yyval = NULL; }
break;
case 137:
{ yyval = NULL; }
break;
case 140:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 141:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 144:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 145:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 146:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
break;
case 147:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
break;
case 148:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 151:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (dsql_nod*) yyvsp[0]; }
break;
case 152:
{yyval = NULL;}
break;
case 155:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 156:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 157:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 158:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 159:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 160:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 161:
{yyval = NULL;}
break;
case 164:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 168:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 169:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 170:
{ yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
break;
case 171:
{ yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
break;
case 172:
{ yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
break;
case 173:
{ yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
break;
case 174:
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 175:
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 176:
{ lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 177:
{ lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
break;
case 178:
{ lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(dsql_nod*) lex.g_file);}
break;
case 179:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 181:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 182:
{ 
			 yyval = (dsql_nod*) make_node (nod_log_file_desc, (int) 1,
												(dsql_nod*) lex.g_file); }
break;
case 183:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (dsql_str*) yyvsp[0]; }
break;
case 186:
{ lex.g_file->fil_length = (IPTR) yyvsp[0]; }
break;
case 187:
{ lex.g_file  = make_file ();}
break;
case 192:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 193:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 200:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 201:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 202:
{ yyval = yyvsp[0]; }
break;
case 203:
{ yyval = yyvsp[0]; }
break;
case 204:
{ yyval = NULL; }
break;
case 206:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 209:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 210:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 211:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 212:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 215:
{ yyval = NULL; }
break;
case 216:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 217:
{ yyval = yyvsp[0]; }
break;
case 218:
{ yyval = NULL; }
break;
case 219:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 220:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 221:
{ yyval = yyvsp[-1]; }
break;
case 222:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 223:
{ yyval = yyvsp[0]; }
break;
case 224:
{ yyval = NULL; }
break;
case 228:
{ yyval = yyvsp[0]; }
break;
case 229:
{ yyval = yyvsp[0]; }
break;
case 230:
{ yyval = yyvsp[0]; }
break;
case 231:
{ yyval = NULL; }
break;
case 234:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 235:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 236:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 237:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 240:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 241:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 242:
{ yyval = yyvsp[0]; }
break;
case 243:
{ yyval = NULL ;}
break;
case 248:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 249:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 250:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 251:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 252:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 253:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 254:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 255:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 256:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 257:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 258:
{ yyval = NULL;}
break;
case 259:
{ yyval = yyvsp[0];}
break;
case 260:
{ yyval = yyvsp[0];}
break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 263:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 264:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 265:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 266:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 267:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 268:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 269:
{ yyval = make_list (yyvsp[-1]); }
break;
case 270:
{ yyval = NULL; }
break;
case 271:
{ yyval = make_list (yyvsp[-1]); }
break;
case 272:
{ yyval = NULL; }
break;
case 274:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 275:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 277:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 279:
{ yyval = yyvsp[0]; }
break;
case 280:
{ yyval = yyvsp[0]; }
break;
case 281:
{ yyval = (DSQL_NOD) NULL; }
break;
case 282:
{ yyval = make_list (yyvsp[0]); }
break;
case 283:
{ yyval = NULL; }
break;
case 285:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 286:
{ yyval = yyvsp[-1]; }
break;
case 289:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 290:
{ yyval = NULL; }
break;
case 291:
{ yyval = NULL; }
break;
case 292:
{ yyval = yyvsp[0]; }
break;
case 293:
{ yyval = yyvsp[0]; }
break;
case 294:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 297:
{ yyval = yyvsp[-1]; }
break;
case 298:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 299:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 300:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 302:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 318:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 319:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 322:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 323:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 324:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 325:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 326:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 327:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 328:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 329:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 330:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 331:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 332:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 333:
{ yyval = NULL; }
break;
case 334:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 335:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 336:
{ yyval = make_list (yyvsp[0]); }
break;
case 337:
{ yyval = make_list (yyvsp[-1]); }
break;
case 338:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_list (yyvsp[0]); }
break;
case 340:
{ yyval = make_list (yyvsp[-1]); }
break;
case 341:
{ yyval = NULL; }
break;
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 347:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 348:
{ yyval = NULL; }
break;
case 349:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 350:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 351:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 352:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 353:
{ yyval = NULL; }
break;
case 355:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 358:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 359:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 361:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 362:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 366:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 367:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 368:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 369:
{ yyval = NULL; }
break;
case 370:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 371:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 372:
{ yyval = make_list (yyvsp[-1]); }
break;
case 373:
{ yyval = NULL; }
break;
case 375:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 376:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 377:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 378:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 379:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 380:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 381:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 382:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 383:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 384:
{ yyval = make_list (yyvsp[0]); }
break;
case 386:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 389:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 390:
{ yyval = yyvsp[-1]; }
break;
case 391:
{ lex.beginning = lex_position(); }
break;
case 392:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 393:
{ lex.beginning = lex.last_token; }
break;
case 394:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 395:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 396:
{ yyval = 0; }
break;
case 397:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 398:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 399:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 401:
{ yyval = NULL; }
break;
case 402:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 403:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 405:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 406:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 407:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 408:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 409:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 410:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 411:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 412:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 413:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 414:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 415:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 416:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 417:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 418:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 419:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 420:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 421:
{ yyval = NULL; }
break;
case 422:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 423:
{ yyval = yyvsp[0]; }
break;
case 424:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 425:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 426:
{ yyval = yyvsp[0]; }
break;
case 427:
{ yyval = yyvsp[0]; }
break;
case 428:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 429:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 430:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 431:
{ yyval = yyvsp[0]; }
break;
case 432:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 434:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 435:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 436:
{ yyval = yyvsp[0]; }
break;
case 437:
{ yyval = yyvsp[0]; }
break;
case 438:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 439:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 440:
{ yyval = yyvsp[0]; }
break;
case 441:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 443:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 444:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 445:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 446:
{ yyval = yyvsp[0]; }
break;
case 447:
{ yyval = yyvsp[0]; }
break;
case 448:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 449:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 450:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 451:
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
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
break;
case 491:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 492:
{ yyval = yyvsp[0]; }
break;
case 493:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 494:
{ yyval = yyvsp[0]; }
break;
case 495:
{ yyval = yyvsp[0]; }
break;
case 496:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 498:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 499:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 501:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 502:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 504:
{ yyval = NULL; }
break;
case 506:
{ yyval = NULL; }
break;
case 507:
{ yyval = yyvsp[0]; }
break;
case 508:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 509:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 510:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 511:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 512:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 513:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 514:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 515:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 516:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 517:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 518:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 519:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 524:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 525:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 527:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 528:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 529:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 535:
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
case 536:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 538:
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
case 539:
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
case 540:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 547:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 548:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 549:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 550:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 551:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 552:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 554:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 555:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 556:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 557:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 558:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 559:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 568:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 569:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 570:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 571:
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
case 572:
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
case 575:
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
case 576:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 577:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 578:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 579:
{ yyval = yyvsp[-1]; }
break;
case 580:
{ yyval = 0; }
break;
case 584:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 585:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 586:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 590:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 591:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 592:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 593:
{ yyval = 0; }
break;
case 594:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 597:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 598:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 601:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 602:
{ yyval = NULL; }
break;
case 604:
{ yyval = NULL; }
break;
case 605:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 607:
{ yyval = NULL; }
break;
case 609:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 614:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 615:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 616:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 617:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 618:
{ yyval = yyvsp[0];}
break;
case 620:
{ yyval = yyvsp[0];}
break;
case 621:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 622:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 623:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 624:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 625:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 626:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 627:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 628:
{ yyval = 0; }
break;
case 629:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 630:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 631:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 632:
{ yyval = (dsql_nod*) 0; }
break;
case 633:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 634:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 636:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 637:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 638:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 639:
{ yyval = 0; }
break;
case 641:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 642:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 643:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 644:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 645:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 646:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 647:
{ yyval = make_list (yyvsp[0]); }
break;
case 648:
{ yyval = 0; }
break;
case 650:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 651:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 652:
{ yyval = 0; }
break;
case 653:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 654:
{ yyval = 0; }
break;
case 655:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 656:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 657:
{ yyval = yyvsp[-1]; }
break;
case 658:
{ yyval = 0; }
break;
case 659:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 660:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 661:
{ yyval = NULL; }
break;
case 662:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 663:
{ yyval = 0; }
break;
case 664:
{ yyval = yyvsp[0]; }
break;
case 665:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 666:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 667:
{ yyval = 0; }
break;
case 668:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 669:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 670:
{ lex.limit_clause = true; }
break;
case 671:
{ lex.limit_clause = false; }
break;
case 672:
{ lex.first_detection = true; }
break;
case 673:
{ lex.first_detection = false; }
break;
case 674:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 675:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 676:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 677:
{ yyval = 0; }
break;
case 678:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 679:
{ yyval = yyvsp[-2]; }
break;
case 680:
{ yyval = yyvsp[-1]; }
break;
case 681:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 682:
{ yyval = yyvsp[-1]; }
break;
case 683:
{ yyval = yyvsp[0]; }
break;
case 684:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 685:
{ yyval = 0; }
break;
case 686:
{ yyval = make_list (yyvsp[0]); }
break;
case 687:
{ yyval = 0; }
break;
case 689:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 691:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 694:
{ yyval = make_list (yyvsp[0]); }
break;
case 696:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 700:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 702:
{ yyval = NULL; }
break;
case 703:
{ yyval = make_list (yyvsp[-1]); }
break;
case 704:
{ yyval = NULL; }
break;
case 706:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 707:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 708:
{ yyval = yyvsp[-1]; }
break;
case 709:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 710:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 711:
{ yyval = make_list (yyvsp[-1]); }
break;
case 712:
{ yyval = NULL; }
break;
case 714:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 715:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 716:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 717:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 718:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 719:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 720:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 721:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 722:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 723:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 724:
{ yyval = make_list (yyvsp[0]); }
break;
case 725:
{ yyval = 0; }
break;
case 727:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 729:
{ yyval = yyvsp[0]; }
break;
case 730:
{ yyval = 0; }
break;
case 731:
{ yyval = yyvsp[0]; }
break;
case 732:
{ yyval = 0; }
break;
case 733:
{ yyval = yyvsp[0]; }
break;
case 734:
{ yyval = 0; }
break;
case 735:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 736:
{ yyval = 0; }
break;
case 737:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 738:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 739:
{ yyval = 0; }
break;
case 740:
{ yyval = 0; }
break;
case 742:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 743:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 747:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 748:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 749:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 752:
{ yyval = make_list (yyvsp[-1]); }
break;
case 753:
{ yyval = 0; }
break;
case 754:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 755:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 758:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 759:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 763:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 764:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 771:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 775:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 779:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 780:
{ yyval = yyvsp[0]; }
break;
case 782:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 785:
{ yyval = NULL; }
break;
case 786:
{ yyval = make_list (yyvsp[-1]); }
break;
case 788:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = NULL; }
break;
case 791:
{ yyval = make_list (yyvsp[-1]); }
break;
case 793:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 797:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 804:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 806:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 808:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 810:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = yyvsp[-1]; }
break;
case 813:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 826:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 827:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 828:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 829:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 830:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 831:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 832:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 834:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 837:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 838:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 839:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 840:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 841:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 842:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 843:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 844:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 845:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 846:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 847:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 850:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 851:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 852:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 853:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 854:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 855:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 856:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 857:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 858:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 859:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 860:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 861:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 862:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 863:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 864:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 865:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 866:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 867:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 868:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 869:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 870:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 871:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 872:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 873:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 875:
{ yyval = make_list (yyvsp[-1]); }
break;
case 876:
{ yyval = yyvsp[-1]; }
break;
case 877:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 878:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 888:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 889:
{ yyval = yyvsp[0]; }
break;
case 890:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 891:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 892:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 893:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 894:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 895:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 896:
{ yyval = yyvsp[-1]; }
break;
case 897:
{ yyval = yyvsp[-1]; }
break;
case 901:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 902:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 903:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 904:
{ yyval = yyvsp[0]; }
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
			yyval = make_node (nod_current_date, 0, NULL);
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
			yyval = make_node (nod_current_time, 0, NULL);
			}
break;
case 907:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 908:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 910:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 912:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 913:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 914:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 915:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 916:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 917:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 919:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 920:
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
case 921:
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
case 922:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 923:
{ yyval = make_parameter (); }
break;
case 924:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 925:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 926:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 927:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 928:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 929:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 930:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 931:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 932:
{ yyval = yyvsp[0]; }
break;
case 933:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 935:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 936:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 937:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 938:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 939:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 941:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 942:
{ yyval = yyvsp[0];}
break;
case 947:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 948:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 949:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 950:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 951:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 952:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 953:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 954:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 955:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 956:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 957:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 958:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 960:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 962:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 963:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 964:
{ yyval = yyvsp[0]; }
break;
case 965:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 966:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 967:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 968:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 971:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 972:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 975:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 976:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 977:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 978:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 979:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 980:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 981:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 982:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 986:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 987:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 988:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 989:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 990:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 991:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 992:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 993:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 996:
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
