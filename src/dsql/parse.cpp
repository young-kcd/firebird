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
   68,   68,   68,   68,   13,   83,   83,   83,   83,   14,
   88,   88,   88,   69,   87,   91,   93,   70,   70,   73,
   73,   73,   81,  100,  100,  100,  101,  101,  102,  102,
  103,  103,  107,  107,   80,  110,  110,  112,  112,  113,
  113,  116,  116,  117,  118,  118,  119,  120,   78,   82,
   79,  104,  104,  123,  124,  124,  126,  126,  127,  127,
  127,  127,  127,  125,  125,  128,  128,  129,  129,  129,
  108,  131,  132,  132,  133,  133,  134,  134,  135,  135,
  135,  106,  106,  106,   75,   85,  136,  136,  136,  137,
  137,  138,  138,  139,  139,  139,  144,   96,   96,  141,
  141,  114,  114,  109,  148,  149,   61,  150,  150,  115,
  115,  115,  115,  115,  115,  142,  142,  157,  157,  158,
  160,  160,  160,  160,  160,  140,  159,  159,  164,  164,
  164,  164,  166,  167,  168,  162,  162,  163,  161,  161,
  161,  161,  161,  169,  170,  171,  171,  171,  171,   74,
   84,   89,  176,  172,  172,  173,  173,  177,  177,  179,
  178,  178,  181,  180,  180,  180,  174,  174,  182,  182,
  183,  185,  185,  186,  184,  184,  188,  188,  187,  190,
  190,  175,  192,  192,  192,  193,  193,  191,  191,  191,
  191,  191,  191,  191,  191,  191,  191,  191,  191,  191,
  191,  191,  191,  191,  191,  191,  191,  200,  200,  201,
  197,  207,  208,  198,  205,  205,  202,  212,  196,  213,
  210,  210,  210,  210,  206,  209,  209,  204,  204,  204,
  211,  211,  194,  194,  217,  218,  218,  219,  219,  219,
  219,  203,  203,  203,  221,  223,  222,  224,   11,  225,
  225,  225,  226,  226,  226,   12,  228,  228,  229,  229,
  230,   77,   86,  146,  234,   97,   99,  233,  233,   76,
   90,  235,  235,  235,  236,  239,  239,  240,  240,  240,
  240,  240,  240,  240,  240,  240,  240,  240,  240,  240,
  240,  240,  237,  237,  238,    2,  242,  242,  242,  242,
  242,  242,  242,  242,  251,  252,  248,  248,  253,  253,
  253,  253,  253,  253,  253,  243,  243,  254,  254,  254,
  254,  254,  254,  254,  247,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  256,  256,  258,  258,
  257,  255,  255,  255,  249,  249,  250,  250,  250,  245,
  246,  246,  262,  262,  262,  262,  262,  244,  263,  263,
  264,  264,    8,  265,  265,  265,  265,  265,  265,  265,
  265,  265,  265,  265,  265,  265,  111,  111,  143,  143,
  268,  268,  271,  271,  273,  273,   56,   56,  270,  270,
  270,  270,  270,  270,  270,  270,  270,  277,  277,  269,
  269,  269,  269,  279,  279,  278,  278,  278,   58,   58,
  274,  274,  274,  272,  272,  272,  283,  283,  283,  282,
  282,  281,  281,  281,  275,  275,  284,  284,  284,  285,
  285,  276,  276,  276,  276,  286,  286,   19,   19,   19,
  288,  288,  288,   17,   17,   17,  290,  291,  294,  294,
  292,  296,  296,    4,   16,  295,  295,  297,  297,  298,
  298,  287,  299,  299,  300,  300,  301,  301,  301,  301,
  302,  302,  303,  303,  304,  304,  306,  306,  306,  307,
  307,  307,  308,  308,  308,  305,  310,  310,  310,  311,
  311,  309,  309,  312,  314,  314,  313,  313,  289,   18,
  315,  315,  317,  317,  316,  316,  232,  321,  322,  318,
  318,  318,  323,  325,  334,  335,  336,  337,  326,  326,
  326,  326,  338,  338,  338,  339,  339,  339,  327,  327,
  328,  328,  341,  341,  342,  342,  343,  343,  329,  345,
  345,  346,  346,  348,  348,  348,  350,  351,  351,  352,
  352,  354,  354,  347,  347,  347,  355,  356,  357,  359,
  359,  360,  361,  349,  349,  362,  362,  363,  363,   24,
  358,  358,  358,  358,  358,  364,  364,  331,  331,  365,
  365,  366,  332,  332,  330,  330,  333,  333,  367,  368,
  368,  368,  368,  368,  369,  369,  370,  370,  371,  371,
  372,  372,  372,  373,  373,  374,  374,  319,  319,  375,
  375,  376,   71,   71,   71,  378,  378,  377,  377,  320,
  320,  320,   10,   10,    7,    7,  380,  381,   20,   20,
  383,  384,  382,  385,  385,  195,  199,    3,    3,  388,
  388,  388,  390,  390,  278,  389,  389,  391,  391,   34,
   34,   95,   94,   94,  379,  379,  392,  393,  393,  215,
  215,  215,  145,  386,  386,  121,  121,  121,  121,  121,
  397,  397,  397,  397,  397,  397,  396,  396,  396,  399,
  399,  399,  399,  399,  399,  399,  399,  399,  399,  399,
  400,  400,  400,  400,  400,  400,  400,  400,  406,  406,
  406,  406,  406,  406,  406,  406,  406,  406,  406,  406,
  406,  406,  406,  406,  411,  411,  401,  401,  402,  402,
  403,  403,  403,  403,  404,  404,  408,  408,  409,  409,
  409,  409,  407,  410,  405,  405,  395,  395,  395,  398,
  398,  398,  412,  412,  413,   98,   98,   98,   98,   98,
   98,   98,   98,   98,   98,   98,   98,   98,   98,   98,
   98,   98,   98,   98,   98,   98,   98,   98,   98,   98,
   98,   98,   98,  156,  156,  156,  414,  227,  227,  151,
  151,  420,  420,  420,  420,  420,  416,  416,  416,  416,
  416,  231,  152,  152,  153,  154,  154,  154,  154,  154,
   54,   54,   21,   21,  241,  421,   57,  280,  261,  261,
  105,  415,  415,  415,  422,  422,  422,  422,  422,  422,
  422,  422,  422,  422,  422,  423,  425,  424,  424,  427,
  428,  428,  387,  387,  417,  418,  418,  429,  429,  429,
  430,  430,  431,  431,  434,  434,  432,  432,  437,  437,
  436,  433,  435,  419,  419,  426,  426,  426,  426,  426,
  426,  426,  426,  340,  340,  324,  324,  155,   51,   67,
  130,  147,  394,  165,  189,  267,   92,   65,  220,  122,
   72,  344,  216,   35,   44,  353,  266,   42,   45,  214,
   43,  293,  260,  260,  438,  438,  438,  438,  438,  438,
  438,  438,  438,  438,  438,  438,  438,  438,  438,  438,
  438,  438,  438,  438,  438,  438,  438,  438,  438,  438,
  438,  438,  438,
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
    1,    2,    2,    7,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    4,
    2,    2,    2,    2,    2,    2,    2,    1,    0,    1,
    1,    6,    6,    1,    1,    0,    0,    1,    0,    4,
    0,    1,    1,    2,    8,    1,    0,    3,    0,    0,
    1,    1,    2,    1,    1,    1,    2,    6,    1,    1,
    3,    0,    1,    1,    0,    1,    1,    2,    3,    4,
    2,    2,    3,    0,    1,    1,    2,    1,    4,    3,
    3,    1,    0,    1,    1,    2,    3,    4,    0,    1,
    2,    0,    1,    1,    5,    5,    3,    2,    0,    1,
    3,    1,    1,    6,    3,    2,    6,    2,    1,    2,
    2,    2,    0,    1,    1,    2,    0,    2,    0,    1,
    1,    1,    1,    1,    1,    0,    1,    1,    2,    2,
    2,    5,    1,    2,    3,    2,    2,    0,    1,    1,
    1,    1,    3,    4,    8,    4,    0,    6,    1,    1,
    2,    2,    0,    3,    3,    1,    2,    2,    2,    8,
    8,    8,    8,    3,    0,    4,    0,    1,    3,    4,
    1,    3,    2,    3,    3,    1,    1,    0,    1,    2,
    4,    1,    1,    3,    1,    0,    2,    1,    6,    1,
    1,    3,    1,    2,    0,    1,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    1,    1,    1,    1,    2,    3,    1,
    3,    8,    9,    5,    8,    6,    3,    0,    3,    2,
    1,    1,    3,    3,    7,    2,    0,    1,    1,    2,
    3,    0,    1,    2,    4,    1,    3,    2,    2,    2,
    1,    1,    1,    1,    2,    2,    5,    0,    5,    1,
    3,    0,    2,    4,    0,    7,    3,    0,    1,    3,
    3,    7,    7,    0,    0,    0,    0,    3,    0,    9,
    9,    1,    1,    0,    2,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    5,    5,    5,    5,
    5,    5,    2,    0,    4,    2,    2,    3,    2,    2,
    3,    3,    2,    2,    3,    6,    1,    2,    4,    3,
    2,    2,    2,    2,    3,    1,    3,    3,    3,    2,
    2,    4,    4,    5,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    2,    2,
    1,    1,    1,    0,    2,    2,    4,    4,    5,    0,
    1,    2,    2,    4,    3,    2,    2,    7,    1,    0,
    1,    0,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    3,    2,    2,    2,    2,    1,    1,    1,    1,
    4,    5,    1,    3,    1,    3,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    4,
    4,    6,    5,    3,    0,    2,    2,    0,    3,    0,
    4,    1,    5,    4,    1,    4,    1,    2,    2,    1,
    1,    1,    2,    2,    2,    2,    0,    3,    5,    1,
    1,    2,    3,    1,    2,    3,    0,    1,    1,    1,
    5,    5,    6,    1,    1,    1,    2,    4,    1,    0,
    5,    1,    0,    3,    2,    1,    0,    2,    0,    1,
    0,    3,    1,    0,    1,    2,    1,    1,    1,    1,
    2,    2,    1,    2,    3,    1,    1,    3,    3,    1,
    2,    3,    1,    2,    0,    2,    1,    1,    0,    1,
    1,    1,    3,    2,    3,    0,    1,    3,    4,    3,
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
    4,    0,    8,    5,    1,    1,    7,    4,    1,    1,
    8,    5,    4,    1,    3,    3,    1,    7,    7,    5,
    3,    0,    1,    1,    1,    2,    0,    1,    1,    1,
    0,    3,    1,    3,    1,    0,    3,    1,    3,    1,
    3,    3,    1,    1,    3,    1,    2,    1,    3,    3,
    1,    2,    3,    3,    3,    3,    1,    3,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    3,    3,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    1,    1,    5,    6,    5,    6,
    3,    4,    5,    6,    3,    4,    3,    4,    3,    4,
    4,    5,    4,    4,    3,    4,    1,    1,    1,    1,
    1,    1,    1,    3,    3,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    2,    2,    3,    3,    3,
    3,    3,    3,    3,    3,    1,    1,    1,    1,    3,
    1,    1,    1,    1,    1,    1,    4,    1,    3,    1,
    2,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    1,    2,    1,    1,    1,    1,    1,    2,
    1,    1,    1,    1,    4,    5,    5,    5,    5,    5,
    5,    5,    5,    5,    5,    1,    6,    1,    4,    7,
    2,    0,    4,    3,    6,    1,    1,    6,    8,    6,
    1,    1,    4,    6,    4,    5,    3,    5,    4,    5,
    1,    1,    1,    4,    6,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    0,    1,    0,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    0,  568,  569,  570,  574,  575,  576,    0,  640,  643,
  755,  756,  759,  760,  480,    0,    0,    0,    0,    0,
    0,    0,  406,  586,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  128,    0,    0,    0,  102,    0,
  935,    0,   22,  933,    0,    0,   73,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  493,    0,    0,    0,   36,   33,   35,    0,   34,
    0, 1023, 1025, 1026, 1027, 1028, 1029, 1030, 1049, 1050,
 1031, 1033, 1032, 1038, 1034, 1039, 1035, 1037, 1040, 1036,
 1041, 1042, 1043, 1045, 1046, 1044, 1047, 1048, 1052, 1051,
 1053,    0,    0,    0,    0,   31,   68,   66, 1015, 1024,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  651,    0,    0,    0,  698, 1017,    0,    0,
    0,    0,    0,    0,  115, 1022,  577,    0,    2,    0,
    0,    0,    0,    0,    0,  437,  438,  439,  440,  441,
  442,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  452,  453,  454,  455,  456,  457,  465,  459,  460,  458,
  461,  462,  463,  464,  466,    0,  435,  436,    0,  407,
 1007,    0,  413, 1011,    0,  410, 1014,    0,  700,    0,
  409, 1018,    0,  414, 1010,    0,  584,  931,    0,  164,
  111,    0,  112,    0,  214, 1003,  793,  103,    0,  109,
  159,    0,    0,  105,    0,  113,  937,    0,  106,    0,
  107,    0,  108, 1021,  160,  114,  110,  743,  744,    0,
  936,  934,    0,   74,    0, 1008,    0, 1006,  501,  494,
    0,  500,  505,  495,  496,  503,  497,  498,  499,  504,
  506,    0,    0,    0,   29,    0,   38,  780,   37,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  941,    0,  922,  645,  645,  646,  656,
  658,  994,  659,    0,  660,  650,  646,    0,    0,    0,
    0,    0,    0,    0,  603,  592,    0,  595,  597,  598,
  599,  600,  606,  607, 1016,  699,    0,  119,    0,    0,
  116,    0,  117,    0,  118,    0,    0,    0,  630,    0,
    0,  996,    0,    0,  637,    0,    0,    0,    0,    0,
  481,    0,    0,  376,    0,  217,    0,  417,  127,  475,
  476,    0,    0,    0,    0,    0,    0,  426,    0,  382,
  383,    0,    0,  590,  588,  932,    0,    0,    0,    0,
    0,    0,    0,  167,  146,    0,  124,    0,    0,    0,
  120,    0,  135,  134,    0,    0,    0,    0,    0,    0,
  999,   75,    0,    0,    0,    0,  758,  502,    0,    0,
    0,    0,  899,  928,    0,  998,    0,  901,    0,    0,
  929,    0,  923,  914,  913,  912,    0,    0,    0,    0,
  904,  905,  906,  915,  916,  924,  925,    0,  926,  927,
    0,    0,    0,  930,    0,    0,    0,    0,    0,    0,
    0,  918,    0,  790,  896,  897,  898,  903,  902,  881,
    0,    0,    0,  880,    0,    0,  885,  877,  878,  879,
  882,  883,  884,  917,  942,  943,  944,  956,  958,  966,
  967,  971,  972,  215,    0,    0,    0,  369,    0,    0,
    0,  783,   26,    0,    0,    0,    0,   71,   69, 1019,
   67,   32,    0,    0,    0,  785,    0,   47,   49,    0,
    0,    0,  582,    0,    0,    0,  653,  655,    0,  662,
    0,    0,    0,  663,  649,    0,  629,    0,  604,    0,
  601,    0,  602,  627,    0,  622,    0,    0,  596,  794,
  764,    0,    0,    0,  125,    0,    0,    0,  579,  578,
    0,  631,  635,    0,    0,  740,  642,  641,    0,  182,
    0,    0,  143,    0,  486,    0,  487,  482,    0,    0,
  421,  423,  422,    0,  424,    0,  418,    0,    0,  268,
    0,    0,    0,  430,  431,    0,  468,    0,    0,    0,
    0,    0,    0,    0,  387,  386,  489,    0,    0,    0,
  163,    0,    0,  172,    0,  171,    0,    0,  178,  161,
    0,  176,  168,    0,    0,  525,  560,  528,    0,  561,
    0,    0,  529,    0,    0,    0,  552,  564,  524,  547,
  526,  527,  522,  509,  376,  507,  508,  510,    0,    0,
  519,  520,  521,  523,    0,    0,    0,    0,  123,    0,
    0,  121,    0,  122,    0,  138,    0,    0,  198,    0,
    0,  374,    0,  217,    0,    0,   83,    0,  101,    0,
  100, 1000,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  796,  798,  807,  810,  811,  812,  813,  814,
  815,  816,  817,  818,  819,  820,    0,    0,    0,    0,
    0,  919,    0,    0,    0,    0,    0,    0,  920,  921,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  330, 1020,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  359,    0,    0,  273,
  517,    0,    0,    0,  367,    0,    0,    0,  782,    0,
    0,   43,   72,    0,    0,   25,    0,  788,    0,    0,
  754,    0,    0,    0,    0,  581,    0,  645,    0,  667,
    0,    0,    0,    0,  572,    0,  939,  571,    0,  605,
    0,  613,  609,  608,    0,    0,    0,  624,  612,    0,
    0,    0,  762,    0,    0,    0,  374,    0,    0,    0,
    0,    0,  144,    0,  485,  376,  420,    0,  376,  377,
  425,    0,    0,  264,  374,  237, 1004,    0,    0,  376,
    0,    0,  206,  374,    0,    0,    0,    0,  242,  236,
  239,  240,  241,  429,  473,  472,  428,  427,    0,    0,
    0,    0,    0,    0,  385,    0,  376,  478,    0,  477,
    0,  169,  173,    0,    0,  177,    0,    0,  775,    0,
  548,  549,  565,    0,  562,    0,    0,  555,  553,  554,
    0,    0,    0,    0,  518,    0,    0,    0,    0,  556,
  126,    0,    0,  374,    0,  197,    0,  200,  202,  203,
    0,    0,    0,    0,    0,  217,   78,    0,    0,   77,
    0,    0,    0,    0,  797,  809,    0,    0,  870,  871,
  872,    0,  801,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  720,  722,    0,  717,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  986,  987,  988,  989,  990,  991,  992,  993,
    0,    0,    0,    0,    0,    0,  977,    0,    0,    0,
    0,    0,  894,  361,    0,  895,  964,    0,  890, 1002,
    0,    0,    0,    0,    0,    0,    0,    0,  331,  332,
    0,    0,  900,  792,  791,  371,  370,    0,  271,    0,
    0,    0,  279,  784,    0,    0,    0,   70,    0,    0,
    0,  787,    0,    0,    0,    0,  654,  657, 1012,  666,
    0,    0,    0,    0,  672,  673,  674,  675,  684,  685,
  686,    0,    0,  664,  573,  940,  614,  623,  618,  617,
    0,  628,  795,  765,    0,    0,  374,    0,    0,  647,
  742,  741,    0,  484,    0,    0,  181,    0,  185,    0,
    0,    0,  419,  374,  374,  276,  377,  269,    0,  208,
  376,  210,  376,  377,  205,  211,    0,    0,    0,    0,
  432,  433,  376,  374,  377,    0,    0,    0,  403,    0,
  479,  193,  194,  170,    0,  180,  536,  537,  938,    0,
    0,    0,    0,    0,  563,    0,    0,  377,    0,    0,
  513,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  195,    0,    0,    0,   82,    0,    0,
    0,    0,   90,   84,    0,    0,   89,   85,    0,  763,
 1005,    0,    0,  802,    0,    0,  808,    0,    0,    0,
    0,    0,  846,  845,    0,    0,    0,  855,  873,    0,
  865,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  800,    0,  721,    0,  757,    0,    0,  217,    0,
  945,    0,    0,  959,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  973,    0,    0,    0,
    0,    0,    0,    0,  984,  639,  963,  907,    0,    0,
    0,    0,  266,  285,    0,    0,  366,  280,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   53,   61,   63,
    0,   41,    0,    0,    0,  769,  789,    0,  768,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  701,    0,
    0,    0,    0,    0,    0,    0,  620,  621,  625,    0,
    0,  196,    0,    0,    0,    0,    0,  186,    0,    0,
    0,  415,  220,  221,  222,  223,  224,  225,  910,    0,
    0,  270,    0,    0,    0,    0,    0,    0,    0,  243,
    0,  469,  470,  434,    0,    0,    0,    0,    0,    0,
  376,  491,  377,  179, 1001,    0,    0,  531,    0,  530,
  566,    0,  558,    0,    0,    0,  511,  539,    0,    0,
  541,  544,  546,  374,    0,    0,    0,  133,    0,  201,
  376,    0,  375,  104,    0,  131,    0,   93,    0,    0,
    0,    0,   87,   88,    0,  863,  864,  804,    0,  806,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  866,
    0,    0,    0,    0,    0,    0,    0,  856,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  728,    0,    0,    0,  951,  950,    0,
    0,  947,  946,    0,  955,  954,  953,  952,  949,  948,
    0,    0,  979,    0,    0,    0,  978,    0,    0,    0,
    0,  364,  334,  333,  272,    0,    0,  282,  283,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  358,    0,    0,    0,    0,    0,  291,  296,
  290,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  314,  315,  316,  317,    0,    0,  352,
  353,  354,    0,  767,   65,   57,   58,   64,   59,   60,
    0,   23,    0,    0,   24,  774,  773,    0,  771,  779,
  778,  776,  753,   44,   45,    0,  676,    0,    0,    0,
  706,  704,  702,    0,  703,    0,    0,    0,    0,    0,
  761,    0,  375,  746,  747,  648,    0,  191,  187,    0,
  377,  911,  274,  275,  377,    0,  148,    0,    0,  228,
    0,    0,  244,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  488,  533,    0,  534,    0,    0,    0,    0,
  152,  154,  155,  156,  516,  514,  512,  543,    0,    0,
  377,    0,    0,    0,  372,  376,   91,    0,    0,   97,
   92,    0,    0,    0,    0,    0,  874,    0,  875,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  730,
  719,    0,    0,  731,    0,  727,  216,  965,  985,  957,
    0,    0,  975,  974,    0,  980,  968,  970,    0,    0,
  281,    0,    0,    0,  313,    0,    0,  312,  340, 1013,
  355,  356,    0,  301,  299,  303,    0,  300,  292,    0,
  297,    0,  343,  298,  302,  304,  305,  306,  307,  308,
  309,  310,  311,    0,    0,  336,    0,   54,   55,   56,
   62,    0,    0,  696,  694,    0,  687,    0,    0,    0,
  710,    0,  644,  377,  373,  748,  188,  377,  158,  263,
    0,    0,  204,  229,    0,    0,    0,    0,  230,  233,
    0,    0,  377,  401,  402,  398,  397,  400,  399,    0,
  532,  559,  157,  145,  153,    0,  376,  260,  140,  377,
  378,    0,    0,   95,    0,   79,    0,  832,  840,  638,
    0,  833,  841,  836,  844,    0,    0,  834,  842,  835,
  843,  829,  837,  830,  838,  831,  839,  726,    0,    0,
    0,  960,  976,    0,    0,    0,  288,  284,    0,    0,
    0,    0,  327,    0,    0,  351,    0,    0,    0,    0,
  346,  344,    0,    0,    0,   39,  770,    0,  678,  688,
    0,    0,  689,  690,  691,    0,  261,  416,  207,  212,
  231,    0,    0,  234,    0,  246,  248,  405,  377,    0,
  380,    0,   76,   98,   99,    0,    0,    0,  733,  969,
  218,  287,    0,    0,    0,    0,  350,  349, 1009,  348,
    0,    0,    0,    0,    0,    0,  677,    0,    0,  711,
  235,    0,    0,    0,    0,    0,  262,  377,    0,    0,
  732,    0,    0,    0,    0,    0,  345,  347,    0,    0,
    0,  682,    0,    0,    0,    0,    0,  245,    0,  252,
    0,  251,  381,  132,  735,    0,  289,    0,    0,    0,
    0,    0,    0,  680,  693,  232,    0,    0,  256,  255,
  254,  736,    0,    0,    0,    0,  335,  683,  259,  257,
  258,  325,    0,  341,  322,  323,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1474,   28,   29, 1475,
 1476,   32,   33,   34,   35,   36,   37, 1477,   39, 1478,
  879,  142,  514,  167, 1265, 1512,  143,  771,  144,  517,
  776,  145,  146,  297, 1042,  156,  157, 1266, 1267, 1268,
 1269,  230,  262,  147, 1270,  148,  519,   87,  274,  422,
  471,  685, 1151,  472,  919,  654,  255,  895,  686,  687,
  688, 1158, 1153, 1601,  275,  690,  691,   79,  248,   80,
  270, 1826, 1374,  254,  259,  261,  263,  250,  241,  243,
  256,  266,  175,  351,  353,  355,  348,  411,  672,  674,
  669,  219,  220,  511,  298,  839,  590,  701, 1083,  415,
  677, 1142, 1368,  622,  797, 1114,  582,  583,  603,  406,
  655, 1094, 1579, 1713, 1312, 1580, 1581, 1582, 1583, 1584,
  702,  251,  242,  402,  630,  403,  404,  631,  632, 1344,
  584, 1077, 1078, 1079, 1307,  417,  907,  908,  909,  910,
  841, 1558,  656,  843,  474,  912,  999,  505, 1431, 1777,
 1313,  475,  476,  477,  478,  479, 1559, 1560,  606, 1719,
 1854, 1330,  849,  850,  836,  851,  852,  853, 1855, 1856,
 1900,  383,  510, 1021, 1479,  226,  599, 1018,  600, 1087,
  506, 1022, 1023, 1255, 1457, 1458, 1459, 1778, 1160, 1480,
 1481, 1482, 1483, 1672, 1484, 1485, 1486, 1487, 1488, 1489,
 1490, 1491, 1492, 1493, 1494, 1495, 1496, 1497, 1498, 1008,
 1891, 1783,  480,  746,  481, 1499, 1673, 1790, 1791, 1838,
 1500, 1501, 1502, 1663,  482,  757,  483,  294,  507,  508,
  484,   41, 1373, 1595,  392,  617,  867, 1342,  618,  865,
   84,   63,  387,  231,  185,  370,  216,  377,  223,  234,
  830,  827,  378,  388,  857,  389,  613, 1105,  217,  485,
 1129,  371,  619, 1343,  102,  229,  279,  657,  658,  761,
 1130,  762, 1131,  661,  662,  663,  664, 1517, 1123, 1121,
  665,  666,  667,  888,  668,  885,   42,   43,   44,   45,
   46,   47,  177,  570,   65,  534,  237,  395,  336,  337,
  338,  339,  340,  341,  342,  343,  344,  803,  555, 1061,
 1299,  556,  557,  808,  181,  359,  572,   48,  184,  365,
 1397,  745,   49,  363,   50,  161,  324,  542,  793,  426,
 1296, 1540,  718,  537,  326, 1304, 1706,  162,  163,  325,
  543,  544,  791, 1040, 1043, 1044, 1045, 1046, 1047, 1048,
 1798, 1847,  486, 1873, 1049, 1050, 1051, 1294, 1803, 1804,
 1805, 1286,  170, 1532, 1700, 1701, 1424,  957, 1425, 1426,
 1427, 1636, 1827, 1829,  575,  576, 1071, 1546,  525,   51,
   52,  427,   53,   54,  563,  564,  487, 1030, 1276, 1518,
 1522,  526,  779,  247,  703,  704,  934,  935,  705,  706,
  707,  708,  709,  710,  711,  712,  713,  714,  715,  716,
 1176, 1178, 1179,  488,  489,  490,  491,  492,  493,  494,
  272,  495,  496,  497,  498,  981,  499, 1642,  500,  501,
  502,  503,  734,  985, 1240, 1235,  735,  150,
};
static short yysindex[] = {                                   5236,
  675, -202, 2941,   87,  784,   68, 2816, -121,10467,  638,
  151,   28, -202, 1197,  420,13031,  952,13031,  178,    0,
  738,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  473,    0,    0,    0,    0,    0,    0, -105,    0,    0,
    0,    0,    0,    0,    0,16492,13031,13031,13031,13031,
13031,13031,    0,    0,  425, 1322,13031,13031,13031,  636,
13031,  481,13031,13031,    0,13031,13031,13031,    0,  400,
    0,  554,    0,    0,  615,13031,    0,13031,13031,13031,
  785,13031,13031,13031,13031,  481,13031,13031,13031,13031,
13031,    0,13031, 1073,  741,    0,    0,    0, 1110,    0,
 1110,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  816,  846,  204, 1264,    0,    0,    0,    0,    0,
13031,13031,13031,  934,  947, 1898,13031,  948,  194,  207,
  870,  906,    0,13031, 1071, 1539,    0,    0,13031, 1034,
13031,13031,13031,13031,    0,    0,    0,13031,    0, 1026,
 1045, 1204, 1042, 1008, 1292,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  895,    0,    0, 1322,    0,
    0,  597,    0,    0, 1469,    0,    0, 1306,    0,  609,
    0,    0, 1027,    0,    0, 1119,    0,    0, 1137,    0,
    0, 1066,    0, 1308,    0,    0,    0,    0, 1322,    0,
    0,  626, 1469,    0,   30,    0,    0, 1323,    0, 1297,
    0, 1110,    0,    0,    0,    0,    0,    0,    0, 1307,
    0,    0, 1190,    0, 1317,    0, 1207,    0,    0,    0,
 1190,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,14887,13031, 1252,    0,13031,    0,    0,    0, 1233,
 1271,12532,13031, 1001, 1335, 1633, 1390, 1384, 1400, 1329,
 1343,  138, 1226,    0,15125,    0,    0,    0,    0,    0,
    0,    0,    0,13157,    0,    0,    0, 1309,13031, 1391,
 1319,  484,13031, 1373,    0,    0, 1539,    0,    0,    0,
    0,    0,    0,    0,    0,    0,13031,    0, 1322, 1469,
    0, 1323,    0, 1110,    0, 1375, 1431, 1269,    0,15125,
 1403,    0, 1403,15125,    0,  -47, 1321, 1291, 1331, 1292,
    0, 1532,   71,    0,13031,    0,  895,    0,    0,    0,
    0,13031, 1252,10206, 1370,10990, 1791,    0,16492,    0,
    0, 1031, 1392,    0,    0,    0, 1780, 1780, 1322, 1477,
 1322,   20, 1066,    0,    0,11894,    0,13031,13031,13031,
    0, 1252,    0,    0, 1561,  828, 1805,13031, 1582,13031,
    0,    0, 1807, 8774, 9602, 1461,    0,    0, 1812, 1814,
 1817, 1405,    0,    0, 1820,    0, 1821,    0, 1824, 1825,
    0, 1832,    0,    0,    0,    0,    0, 1833, 1421, 1422,
    0,    0,    0,    0,    0,    0,    0, 1836,    0,    0,
13411, 1840, 1841,    0, 1842, 1536,15125,15125,13649,13031,
 1848,    0, 2183,    0,    0,    0,    0,    0,    0,    0,
 1799, 1489, 1855,    0,    0, 1856,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11894, 1847,  751,    0, 1863, 1645,
  851,    0,    0,13031,13031,13031,  112,    0,    0,    0,
    0,    0,13031,13031,  679,    0,13031,    0,    0, 1233,
 1523,12532,    0,13031,13649, 1188,    0,    0,15125,    0,
 1669, 1604, 1878,    0,    0,   90,    0, 1114,    0,  -20,
    0,  -20,    0,    0, 1879,    0,   89, 1504,    0,    0,
    0, 1881,   82, 1864,    0, 1252, 1888, 1667,    0,    0,
13031,    0,    0, 1768, 1894,    0,    0,    0,  883,    0,
 1598, 1599,    0, 1322,    0, 1601,    0,    0, 1671, 1674,
    0,    0,    0, 1656,    0,11894,    0,11894, 1280,    0,
 1688,13031,16325,    0,    0,  -72,    0,13031,  682, 1306,
 1564, 1529, 1496,    0,    0,    0,    0,  552, 1572,  162,
    0, 1515,  481,    0, 1322,    0, 1692, 1624,    0,    0,
   20,    0,    0,  218, 1533,    0,    0,    0, 1534,    0,
 1591, 1944,    0, 1641, 1948,  548,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1899,  163,
    0,    0,    0,    0,  106, 1952, 1954, 1948,    0, 1322,
 1469,    0, 1680,    0, 1732,    0, 1322, 1322,    0,10206,
  609,    0, 1621,    0, 1595, 1957,    0, 5603,    0, 1620,
    0,    0, 1628, 1965, 9840, 1966,    0,    0,    0, 9347,
10166,    3,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  446, 1631, 1046,15125,
  441,    0,13031,15125, 1169, 1189, 1299, 2208,    0,    0,
15125,10094, 2183, 1573,  433,15125,15125,10094, 1708,  729,
  729, 1722, 1396, -105, 1984,    0,    0,13903,13031,15125,
15125,15125,15125,15125,15125, 1683,    0,15125, 6719,    0,
    0, 1753, 1967,13031,    0,13031, 1739,13031,    0, 1609,
 1610,    0,    0, 1596,12532,    0, 1715,    0, 1537, 2013,
    0, 1715,13031,13031, 2010,    0, 1722,    0, 1935,    0,
13031, 4421, 1611,15125,    0,  961,    0,    0,  413,    0,
 1622,    0,    0,    0,13031,  616,13031,    0,    0,13031,
13031, 1461,    0,15125, 1796,10206,    0, 2017, 1569,15125,
15125, 1322,    0,  379,    0,    0,    0, 2024,    0,    0,
    0,   93,13031,    0,    0,    0,    0, 1795, 2025,    0,
 1772, 1797,    0,    0, 1745, 1746, 1110, 1813,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  481,13031,
16423, 1702, 1709, 1711,    0,  481,    0,    0, 1025,    0,
 1361,    0,    0, 1681, 1322,    0, 8774,   66,    0, 1684,
    0,    0,    0,  481,    0, 1944,   91,    0,    0,    0,
 1772,   91, 1685,   91,    0, 2054,  481,  481,  481,    0,
    0, 1252,13031,    0, 1744,    0, 1541,    0,    0,    0,
 1031, 1403,13031, 1603,  111,    0,    0, 2062,  -33,    0,
 8774,13031, 1403,10332,    0,    0, 1403, 9840,    0,    0,
    0, 9299,    0,  157,  467,15125,15125,10593, 2064,  848,
10854,15125,11115,  670,11376,11637,14141,11898,12159,12420,
10094,10094,    0,    0, 1737,    0, 2074, 1008,15125,15125,
 1870,15125, 2076,15125, 2071, 2029,15125,15125,15125,15125,
15125,15125,    0,    0,    0,    0,    0,    0,    0,    0,
 1802, 1286,   97,15125,  725,15125,    0,10094, 2004, 2372,
  229,13031,    0,    0, 1008,    0,    0, 1616,    0,    0,
 1843,  729,  729,   48,   48, 1015, 9486, 2077,    0,    0,
 2083, 2183,    0,    0,    0,    0,    0, 1634,    0, 1693,
 1852, 1739,    0,    0,12295,12295, 1752,    0,  -41, 1762,
13031,    0,15125, 1762, 1815, 1816,    0,    0,    0,    0,
 2200, 2096, 2093, 1040,    0,    0,    0,    0,    0,    0,
    0,10094, 1819,    0,    0,    0,    0,    0,    0,    0,
  435,    0,    0,    0, 1631, 2183,    0, 1652, 1403,    0,
    0,    0, 2183,    0, 1780, 1873,    0,  379,    0, 2106,
10094, 5339,    0,    0,    0,    0,    0,    0, 1739,    0,
    0,    0,    0,    0,    0,    0, 1110, 1110, 1657, 2108,
    0,    0,    0,    0,    0,  439,   12,   24,    0, 1887,
    0,    0,    0,    0,13031,    0,    0,    0,    0,   87,
 1665, 1735, 1753, 2114,    0, 1515, 1686,    0, 2098, 1088,
    0,13031, 1177,  481, 2119, 2122, 2125, 1904,  609, 1739,
 1780, 1599,10206,    0, 1572, 1724, 2272,    0,  481, 1784,
 1866, 5603,    0,    0,  481,  890,    0,    0, 1867,    0,
    0, 2135, 2138,    0,10094,10094,    0,10094,10094, 2571,
 2183, 2141,    0,    0, 2183, 2149,13649,    0,    0, 1876,
    0,  996, 2150, 2183, 2154, 1217, 2155, 2183, 2164,15125,
15125, 2064,15125,14395, 2169, 2183, 2170, 2175, 2183, 2176,
15125, 2183, 2181, 2183, 2189, 2191, 2183, 2194, 2197, 2183,
 2199,    0, 1961,    0,13324,    0, 2177, 2201,    0, 2335,
    0, 2365,15125,    0, 2488, 2555, 2578, 2588, 2622, 2752,
15125,15125,15125, 2183, 1826,15125,    0,15125, 2183, 1936,
  522,15125,15125,15125,    0,    0,    0,    0, 1690, 9486,
 4721,13031,    0,    0,13031,16134,    0,    0,13031,13031,
13031,13031,13031,13031, 1806, 2210, 2211,    0,    0,    0,
 1806,    0,  246,  246,  156,    0,    0, 1694,    0,12295,
12295, 2219, 1040, 2220,15125, 2000, 4421, 1886,    0, 1886,
 1225, 1886, 1929, 1931, 1996, 1945,    0,    0,    0, 1008,
 1739,    0, 1724,  878, 1515, 1891, 1515,    0,10094,  357,
 1142,    0,    0,    0,    0,    0,    0,    0,    0, 5339,
 5339,    0, 1852,15125, 5339, 1987, 1877, 1657,  400,    0,
10094,    0,    0,    0, 1900, 1901, 1903, 1906, 1910, 1911,
    0,    0,    0,    0,    0, 2236,   87,    0, 1844,    0,
    0,   91,    0, 1919,   91,   91,    0,    0, 1753, 2242,
    0,    0,    0,    0, 1031, 1852, 1515,    0, 1599,    0,
    0, 2016,    0,    0, 2017,    0, 2253,    0, 2257, 1322,
  -13, 2260,    0,    0, 1322,    0,    0,    0, 1961,    0,
 1961,15125, 1403, 1403, 1710, -105, 2276,15125, 2002,    0,
 1403, 1403,15125, 1403, 1403, 2703, 2183,    0, 1655,15125,
 2183, 1403, 1403, 1403, 1403, 2183, 1403, 1403, 1403, 1403,
 1403, 1403,13031,    0, 2279, 2269, 1018,    0,    0,11894,
 2280,    0,    0, 2797,    0,    0,    0,    0,    0,    0,
 2902, 1586,    0,15125, 2019, 1895,    0,15125, 2917, 1733,
 3025,    0,    0,    0,    0,11894, 2267,    0,    0, 2037,
    0,13031, -133, 2283, 2299, 2008,15125, 2286,    0,13031,
13031,13031,    0, 2287, 2290, 2291, 2020, 2294,    0,    0,
    0, 2050,15999, 2300, 2301, 2302, 2303, 2308, 2309, 2310,
 2313, 2314, 2320,    0,    0,    0,    0,   11, 2327,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2065,    0,12295,12295,    0,    0,    0, 1958,    0,    0,
    0,    0,    0,    0,    0, 2000,    0, 1741,13031, 1040,
    0,    0,    0, 2049,    0, 4421, 4421,15125,10094, 1461,
    0, 1852,    0,    0,    0,    0, 1361,    0,    0,  623,
    0,    0,    0,    0,    0, 2183,    0, 2113, 1987,    0,
  950,13031,    0, 2061,  758, 1964, 2067, 1970, 2102, 2075,
 2120, 1739,    0,    0, 2370,    0, 2381, 2078, 2113, 1919,
    0,    0,    0,    0,    0,    0,    0,    0, 1739, 1572,
    0, 1361, 1887, 2046,    0,    0,    0, 2060,  -15,    0,
    0, 1753, 2063, 2183, 2386, 2389,    0, 1008,    0, 2183,
15125, 2391, 2394, 2183, 2400, 2402,15125,15125, 2183, 2403,
 2406, 2407, 2408, 2410, 2412, 2416, 2420, 2421, 2425,    0,
    0,13324, 2427,    0,13031,    0,    0,    0,    0,    0,
15125, 2428,    0,    0,15125,    0,    0,    0,15125,  378,
    0, 2157,15125,15125,    0,10094, 2183,    0,    0,    0,
    0,    0,13031,    0,    0,    0, 9486,    0,    0,   -7,
    0, 2030,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  538, 2434,    0, 2099,    0,    0,    0,
    0,  246,13031,    0,    0, 4421,    0, 1882, 2183, 2432,
    0,    3,    0,    0,    0,    0,    0,    0,    0,    0,
 2438,13031,    0,    0, 2132, 2144,13031, 1657,    0,    0,
 1110,13031,    0,    0,    0,    0,    0,    0,    0, 1852,
    0,    0,    0,    0,    0, 1852,    0,    0,    0,    0,
    0,15125, 1322,    0, 2023,    0, 1322,    0,    0,    0,
 2183,    0,    0,    0,    0, 2183, 2183,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,13031, 2152,
 2183,    0,    0, 2977, 5339, 5339,    0,    0, 2446, 2183,
 1146,  778,    0, 2153, 2077,    0,13031,13031,   87,  189,
    0,    0, 1988, 2159,10094,    0,    0, 2451,    0,    0,
10094, 2456,    0,    0,    0,15125,    0,    0,    0,    0,
    0, 1657, 1110,    0, 2121,    0,    0,    0,    0, 1887,
    0, 2183,    0,    0,    0, 2455, 2459, 2461,    0,    0,
    0,    0, 1403, 9486, 2079, 9486,    0,    0,    0,    0,
16134,   -7,15125, 9486,  800,13031,    0,    3,13031,    0,
    0, 2121,  480, 1657, 2126, 2127,    0,    0, 2464,13031,
    0,13031, 2467, 2077,16134, 2077,    0,    0, 1471,  131,
 2212,    0, 1749, 1760, 1657,  506,  506,    0, 2213,    0,
 2080,    0,    0,    0,    0, 2474,    0, 2215, 9486, 2227,
 2226,16134,13031,    0,    0,    0, 2085,  641,    0,    0,
    0,    0,16134,  193,13031,16134,    0,    0,    0,    0,
    0,    0,16134,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  280,  825,    0,    0,    0,    0,    0,    0,    0,
    0,10728,  170,12658,    0,    0,    0,    0,    0,    0,
 2532,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  227,    0,    0,    0,    0,    0,    0,   98,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  903,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2205,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   47, 2158,    0,    0,    0,  124,    0,
  124,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2165,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  933,    0,    0,
14633,12919,    0,    0,    0,  942,    0,    0,  122,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  407,    0, 2133,  312,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  504,    0,    0,    0,    0,  721,
    0,    0,    0,    0,    0,  976,    0,    0,    0,    0,
    0,  152,    0,12155,    0,    0,    0,    0,    0,    0,
    0,    0,  504,    0,  445,    0,    0, 2503,    0,    0,
    0, 2281,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  115,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  171,    0, 2285,    0,    0,    0,    0,    0,13523,
    0,    0,    0,    0,    0, 1101,    0,    0,    0,    0,
    0,    0,13777,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  660,    0,    0, 1061,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  504,
    0, 2503,    0, 2281,    0, 1117,  241,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1144,
    0, 2268,    0,    0,    0,    0, 1159,    0,    0,    0,
    0,    0, 2285,  747,16561,    0, 1166,    0,    0,    0,
    0,  255,    0,    0,    0,    0, 2101, 2101,    0,    0,
    0, 1178,  173,    0,    0,    0,    0,    0,    0,    0,
    0, 2285,    0,    0, 1327,    0,    0,    0,    0,    0,
    0,    0,11372,    0,    0,   72,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2066,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2846,    0,    0,
    0, 3146, 3446,    0, 3746, 4046,    0,    0,    0,    0,
    0,    0,  902,    0,    0,    0,    0,    0,    0,    0,
 4346, 1182,  187,    0, 2546,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1228,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,13523,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 6950,    0, 2235,    0,    0,    0,    0,    0,    0,  667,
    0,  667,    0,    0, 1096,    0, 1279, 1320,    0,    0,
    0,    0,  115,    0,    0, 2285,    0,    0,    0,    0,
    0,    0,    0,  149,  264,    0,    0,    0,  369,    0,
    0, 1145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2268,    0,    0, 1131,    0,
 2092,    0,    0,  547,    0,    0,    0,    0,  620,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1238,    0,    0, 1930,15642,    0,    0,    0,15654,    0,
    0,15691,    0,    0,15845,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2463, 2952,
    0,    0,    0,    0,15902, 7746,    0,15845,    0,    0,
  504,    0,    0,    0,    0,    0,    0,    0,    0,  747,
 1296,    0,    0,    0,    0, 2161,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 7336, 7515, 7637,    0,
    0,  753,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1821,   75,15379,    0,
15379,    0,    0,    0,15379,15379,15379,    0,    0,    0,
    0,    0, 2112,    0,    0,    0,    0,    0,    0, 5535,
 5824, 2512,    0,   78,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1340,    0,    0,    0,    0, 2292,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  242,    0,    0,    0,
    0,  242,    0,    0, 1258,    0,    0,    0,    0,    0,
    0,    0,  837,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  509,    0,    0,    0,    0,
    0,   72,    0,    0,    0,  747,    0,  347,  177,    0,
    0,    0,    0,  442,    0,    0,    0,    0,    0,    0,
    0, 1773,    0,    0,    0,    0,    0, 2525,    0,    0,
 1885, 2057,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  290,  414,  515,    0,    0,    0,    0,    0,    0,
 1136,    0,    0,    0,    0,    0,    0,    0,    0, 3137,
    0,    0,    0,    0,    0,15691,    0,    0,    0,    0,
  702,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2285,    0,    0,  269,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11633,    0,    0,    0,  410,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1820,    0,    0, 1301,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2530,    0,    0,    0,    0,    0,
 4646, 6113, 6402, 4946, 5246,    0,    0, 1310,    0,    0,
    0, 1014,    0,    0,    0,    0,    0,    0,    0,14015,
    0, 2306,    0,    0,    0,    0,    0,    0,    0, 1330,
    0,    0,    0, 1330,    0,    0,    0,    0,    0,    0,
    0, 6611,  172,  633,    0,    0,    0,    0,    0,    0,
    0,    0,  929,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   75,  153,    0,    0,    0,    0,
    0,    0,  394,    0, 2101, 2123,    0, 1154,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2292,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1232,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1382,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 3437,    0,    0,    0,    0,    0, 1223,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1296, 2292,
 2101, 1388,  747,    0, 2330, 1407,    0,    0,    0,11633,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  909,    0,    0,    0, 1041,    0,    0,    0,    0,    0,
    0,    0,    0, 1580,    0, 1920,    0, 6320,    0,    0,
    0,    0,    0,    0,    0, 7720,    0,    0, 7804,    0,
    0, 7926,    0, 7956,    0,    0, 8079,    0,    0, 8161,
    0,    0, 6653,    0, 1821,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2145,    0,    0,    0,    0,  767,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  455,    0,    0,    0,    0,
    0,    0,    0,  180, 1419,  176,  184,    0,    0,    0,
 1419,    0, 2167,  337,    0,    0,    0,    0,    0,    0,
    0,    0, 2238, 1350,    0, 7035,    0, 2239,    0, 2239,
 2238, 2239,    0,    0,    0, 1143,    0,    0,    0, 1301,
 2292,    0, 1407,    0,    0, 2151,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1542,    0, 1232, 2205,    0,
    0,    0,    0,    0,  546,  575,  629,  658,  703,  811,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0, 3585,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1425,    0,
    0,    0,    0,    0, 1427,    0,    0,    0,    0,    0,
  154,    0,    0,    0,    0,    0,    0,    0,  230,    0,
  231,    0,    0,    0,    0,   78,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 8255,    0, 8337,    0,
 8426,    0,    0,    0,    0, 8460,    0,    0,    0,    0,
    0,    0, 1193,    0,    0, 2558,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2562,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12968, 2545,    0,    0,    0,    0,    0,    0, 1698, 1712,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  639,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1606,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 6826,    0,    0,    0, 1156,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  150,
    0,    0,    0,    0,    0,    0, 1010,    0,    0,    0,
    0,    0,    0,    0,    0, 2570,    0, 1326, 2059,    0,
 2268,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2292,    0,    0,    0,    0,    0,    0, 1490,  430,
    0,    0,    0,    0,    0,    0,    0,    0, 2292, 2330,
    0,  306,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  691,    0, 8520,    0,    0,    0, 2530,    0, 8602,
    0,    0,    0, 8636,    0,    0,    0,    0, 8696,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1821,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2556,
    0,    0, 2563,    0,    0,    0, 2565,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2321,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  337, 9118,    0,    0,    0,    0, 2238,  959, 1128,
    0, 1216,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3737,    0,    0,
  158,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  169,    0,    0,    0,    0,    0,
 8816,    0,    0,    0,    0, 8995, 9055,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1792,
 2586,    0,    0,    0,    0,    0,    0,    0,    0, 2569,
 2573,    0,    0,    0, 2575,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 9166,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 3737, 1594,    0,  179,    0,    0,    0,    0,    0,
    0, 2570,    0,    0,    0, 2595,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   29,    0,    0,    0,    0,    0,    0, 9220,    0,    0,
    0, 1293,    0, 1232, 1456, 1507,    0,    0,    0,    0,
    0,    0,    0, 2579,   29, 2581,    0,    0,    0, 2340,
    0,    0,    0,    0, 3737,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,15891,    0,    0,
    0,   29,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   29,    0,    0,   29,    0,    0,    0,    0,
    0,    0,   29,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2643,    0,    0, 2644,
 2645,    0,    0,    0,    0,    0,    0,    1,    0, 2646,
   -2, 2492, 2131,  -56, -759, 1379, 2495, 1869, 2499, 2134,
    0,    0, 2364, -103,   34,    0,    0,    0,    0,  275,
  281,  -54,  -77,  -45, -266, 2377, 1912,    0,    0,    0,
  734,    0,    0,  -53, 1544, -588,  -68,-1024, 2001, 1770,
 -345,    0, 1538,    0, 2601, 1777, 1822,    0,    0,    0,
 -539,  -48,    0,    0,    0,    0,    0, 2627,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -65,    0, -557, -207, 1559, -347, 1072, -902,    0,
    0,    0,    0, -360, -144, -308, 1565, -378,  -46,    0,
 -574, 1831,    0, 1132,-1120,    0, 1130,    0,    0, 2351,
 -618,  -19,    0,    0,    0,    0, 2325,    0, 2081, 1605,
    0,    0,    0, 1658,    0, 2383, 1917, 1608, 2354, 2357,
    0,    0, -475, 1907,  -44, -528, 1043, -264,    0,    0,
    0,-1042, -942, -904, -896, -894,    0, 1198,-1112,    0,
  900,-1274, 1200,    0, 2160,    0,    0,    0,  915,  904,
  881, -128, -271, -951, -982,    0,    0,    0, 1946,    0,
 -650,    0, 1740,    0,    0,    0,    0,    0,-1153,-1078,
    0,    0,    0,    0, -243,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -981,
    0,    0, -722,    0, -715, 1312, 1105,    0,  936,    0,
    0,    0,    0,    0,    0,    0, -374,    0,    0, 2021,
 -148, -455, 1480, 1241, -568, -784,-1037,-1332,    0,    0,
  -39,    0,    0,    0,    0,    0, 2409,    0,    0,    0,
    0,    0, 2419, 2179,    0,    0,    0,    0,    0,   -9,
 -490, 2430,    0,    0,    0,  -10,    0,    0,    0, -388,
 1908, -387, 1445,    0,    0,    0,    0, 2171,    0, -946,
    0,    0,    0, 2136,    0, 1921,    0,    0,    0,    0,
    0,    0,  -31,    0, 2790,    0,    0,    0,    0,    0,
 2469,    0,    0,    0,    0, 2261,    0, 2263,    0,    0,
    0, 2007,    0,    0,    0,    0,    0, -368, -673, -892,
 1682,    0, 1243,    0,    0,    0,    0,    0,    0, -402,
    0,    0, -775, -276, 1257,    0,    0,    0, 2651,  889,
    0, 2022, -884, -922,    0, -930, 1778,-1237,    0,    0,
    0,    0, -164,    0,    0,    0,    0, 1527,    0,    0,
    0,    0, 2732,  335,    0, 1016, 2104,    0, 1192,    0,
 1402,    0, -228,    0,    0, 2006,    0,    0,    0,    0,
    0, 2264,    0,    0,    0, -448,-1195, 2047, 1800,-1211,
    0,    0,    0, -686, -341,  223,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1134, 1638,    0,    0,    0, -888,    0,    0,    0, 1525,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,-1008, 1614,    0,    0,
};
#define YYTABLESIZE 17081
static short yytable[] = {                                     149,
   38,   83,  249,  228,  346,  169,  168,  299,  176,  222,
  318,  321,  240,  818,  317,  320,  258,  659,  660,  260,
  244,  289,  245,  629,  280, 1249,  594,  286,  840,  760,
  596,  265,  257, 1009,  819,  518, 1065,  623, 1257, 1314,
 1010,  538,  233,  288,  958,  284,  218,  221,  224,  227,
  168,  232,  235, 1563,  290,  798,  257,  246,  221,  235,
 1504,  227, 1519,  168,  232, 1216,  264,  149,  235,  781,
  995,  718, 1015,  283,  739,  778,  276,  169,  168,  278,
  221,  291,  276,  235,  224,  227,  287,  168,  232,  264,
  149,  235,  225,  227,  743,  306,  354,  739, 1350,  920,
  744, 1460, 1246,  561,  253,  349,  305, 1371,  307, 1120,
 1283,  601,  911,  983,  716, 1019,  352,  598,  739,  991,
  831,  700,  832, 1063,  412,  811, 1145,  842,  285,  870,
  718,   82,  807,  739,  796, 1126,  292, 1323,  739, 1315,
  675,  246,  168,  246,  328,  897,  356,  149,  745,  718,
 1150,  165,  766, 1085,  235,  775,  739,  781,  419,  345,
  812,  221,  227,  168,  264,  379,  744,  781,  176,  587,
  362,  669,  166,  716, 1250,   51,  749, 1316,  253, 1029,
  700,  303,  562,   52, 1322, 1317,  360, 1318, 1366,  745,
  718, 1326,  745, 1319,   94,  407,  766, 1167,  781, 1553,
 1554,  781, 1334,  823, 1557,  350,  869,  745,  718,   96,
  165,  766,  669, 1561,  659,  660,  781,  749,  316,  253,
  749,  566,  253, 1029, 1443, 1354,  632, 1445,  587,  362,
  669,  166, 1842,  315,   51,  749, 1250,  253, 1029, 1156,
  634,  772,   52,  845,   64,  360,  319,  303,  504,  773,
  568,  512,  629,  894,  490, 1786,  316,  878,  848, 1599,
 1740,  103,   82,  738,  951,  518, 1271,  632,  139,  316,
  803,  805, 1244,  103,  182, 1273,  554, 1314, 1314,  587,
  547,  634, 1314,  246, 1009,  632,  246, 1504, 1069,  390,
   82, 1010,  520,  149,  815,  565,  580,  413, 1697,  634,
  772, 1787,  560, 1086,  738,  192, 1089,  891,  316, 1337,
  368,  752, 1788,  490, 1157, 1096,  627, 1661, 1662,  224,
  846, 1339,  738,  168,  749, 1684,  183,  139, 1521,  750,
  595,  744, 1212, 1213, 1587, 1744,  538,  504,  587,  245,
 1555,  609,  670,  337,  611,  624,  633,  626,  390, 1542,
  801,  154,  752,  925,  592,  673, 1530, 1340,  951,  562,
  847,  681,  679,  580,  192,  246, 1654,  593,  750, 1241,
  752,  683,  246,  998,  246, 1140,  246, 1315, 1315,  614,
 1006,  952, 1315, 1591,   88, 1103, 1274,  633,  414, 1084,
 1053, 1300,  104,  751, 1890,  538, 1127, 1551,  221,  227,
  232, 1529, 1576,  806, 1671,  633,  636, 1541,  168,  750,
  224, 1789,  739,  388,  692, 1316, 1316,  150, 1165,  802,
 1316,  689,  153, 1317, 1317, 1318, 1318,  750, 1317,  151,
 1318, 1319, 1319,  739,  751, 1643, 1319,  893, 1776, 1646,
 1573,  183,  671, 1814, 1600, 1338, 1561,  636,  165,  368,
   86,  718,  751,   86,  532, 1685, 1146,  770, 1800,   94,
  747,  581, 1310,  745,  718,  636,  777, 1162, 1745,  166,
  782, 1163,  388,  337,   96,  952,  150,  871, 1080,  560,
 1797, 1082,  963,  155,  745,  718,  669, 1858,  151, 1841,
  951,  749, 1092, 1913,  716,  165,  669, 1149,  669,  781,
  183,  700,  786,  716,  168,  227,  520,  669,  518, 1784,
  700, 1037,  749,  168,  389, 1119,  166,  168,  490, 1110,
 1524, 1525,  520, 1233,  176,  425,  512, 1453,  628,  718,
  824,  700,  766,  781, 1454, 1166,   81, 1851, 1301,  314,
  314,  766,  902,  739,  896,  395, 1388, 1389,  772, 1390,
 1391,  669, 1737,  390,  872, 1320, 1321,  774,  844, 1704,
  669,  246,  632,  920, 1015,  700,  795, 1064,  598, 1152,
 1365,  873,  362,  389,  396, 1333,  634, 1746,  738, 1878,
 1590,  718, 1277,  257,  718, 1282, 1164,  739,  360, 1375,
  739, 1011,  837,  246,  745,  718,  766,  587,  837,  738,
 1896, 1455,  772,  669,  395, 1119, 1698,  952,  803,  805,
  739,  314,  139, 1303, 1016,  669,  901,  669,  951,  404,
 1730,   51,  749,  905,  906, 1029,  752,  716,  392,   52,
 1138,  302,  670,  396,  700,  245, 1773, 1736,  868, 1099,
  877, 1693,  745,  314,  490, 1504,  562,  752, 1709,  192,
  781, 1056, 1710, 1711, 1831, 1832,  314,  391, 1278,  610,
  165,  745,  718, 1708,  268,  766,  615,   81,  877, 1504,
  246,  253,  632,  670, 1775,  178,  670,  388,  404,  390,
  587,  166,  633,  750,  669, 1785,  634,  392, 1738,  749,
 1550,  670,  550,  376,  150,   81, 1504,  538,  269,  183,
  322,  149,  394,  965,  750,  376,  151, 1504,  751,  738,
 1504,  183, 1565,  235, 1305, 1750,  391, 1504,  610,  504,
  384,  504, 1608, 1024, 1056,  615, 1035,  136, 1168,  751,
 1075,  540, 1314, 1314,  540,  952,  986,  987,  183, 1000,
  962,  164,  636, 1324,  183, 1325,  183, 1818,  554,  246,
 1062, 1914,  715, 1819,  246, 1332,  246,  752,  246,  295,
  149,  394, 1867,  550, 1703,  520,  560,  265, 1074,  337,
  753,  245, 1335,  168,  227,  754,  738, 1876,  389,  384,
 1367, 1039,  227,  951,  953,  183, 1888,  180,  504, 1564,
 1101,  765,  633,  715,  764,  168,  179,  168, 1723, 1076,
  246, 1807, 1395,  388, 1152, 1808,  246,  954, 1396,  395,
  393,  715,   86, 1907,  750, 1102, 1104,  772, 1835,  257,
 1817, 1116,  889,  246, 1912,  236, 1109, 1915, 1135, 1136,
 1137, 1297, 1315, 1315, 1916, 1589,  716, 1821,  396,  751,
 1871,  165, 1011,  552, 1124, 1169, 1139, 1793,  166,  862,
  246,  246, 1864,  380, 1866, 1637, 1147,  257,  257,  257,
  551, 1577, 1870,  955, 1585,  390,  562,  692, 1450,  393,
 1316, 1316, 1336, 1430, 1117,  988, 1897,  716, 1317, 1317,
 1318, 1318, 1298,  404,  951,  863, 1319, 1319,  890, 1327,
 1328,  769,  392,  168,  768,  716,  252,  136,  136,  337,
  952,  908,  589,  168,  389,  619,  265, 1904,  857,  151,
 1528,  692, 1161, 1877,  552, 1898, 1857,  926,  689, 1859,
 1702,  391,  933, 1872,  753,  751,  381,  752,  709,  754,
   81,  553,  585,  273,  408,  395, 1003, 1910,  391, 1376,
 1190,  594,  908,  293, 1009,  908,   14,  670, 1448,  857,
  183, 1010,  857,  337, 1191, 1883,  619,  670,  712,  670,
  908,  589, 1899,  540,  396,   55,  394,  857,  670,  709,
 1908,  705, 1245,  152,  436,  591,   56,  149,  149,  384,
 1650, 1056,  235,   57,  384,  864,  560,  709, 1911,  384,
  823,  585, 1505, 1572,  908, 1508,  540,  610, 1192,  712,
  594,  952,  712,  271,  615,  749,  423,   58, 1003,  192,
  750, 1059,  670,  909,  428,  520,  520,  712,  392,  951,
  409,  670,  238, 1593, 1396, 1396, 1193, 1060, 1236, 1237,
  610,  227, 1396, 1396,  591, 1396, 1396, 1782,  540,  951,
  824,  659,  660, 1396, 1396, 1396, 1396,  391, 1396, 1396,
 1396, 1396, 1396, 1396,  909,  410,  610,  909,  758,  610,
  593,  951,  238,  615,  670, 1360,  615,  715,  192,   59,
  983,  983,  909,  149,  393,  610,  670,  715,  670,  715,
 1378,  824,  615,  337,  824, 1011, 1382,   14,  715,  129,
 1194,  562,  394,  540,  257,  616,   85,   86,  245,  824,
   60,  610,  512,  281,   61, 1345,  909, 1248,  615,  257,
  384, 1009,  293, 1009,  780,  257,  580, 1346, 1010,  593,
 1010, 1009, 1345,  129, 1516, 1516, 1520,  708, 1010,  322,
  474, 1356,  715,  246,  295,  192,  952,  246,  855,  238,
  856,  715,  714,  411,  483,  670,  926, 1180,  540,  296,
  926,  716,  372,  184,  616,  671,  952,  129,  412,  749,
 1547,  716, 1549,  716,  750,  408, 1009, 1238,  708,  323,
  857,  678,  716, 1010,  474,  580, 1845,  174,  952,  238,
 1357,  365, 1848,  714,  715, 1509,  708,  753,  751,  474,
  752,  300,  754,   62,  192, 1181,  671,  373,  715,  671,
  393,  714,  411,  483, 1423,  168, 1507,  504, 1456,  983,
  245,  560,  184,  848,  671,  713,  716,  412, 1510, 1182,
 1356,  301, 1592,  857,  408,  716,  857,   42,  788,  753,
  751,  247,  752,  857,  754,  857,  174,  175, 1707,  857,
  365,  246,  246,  709,  857, 1461, 1503,  857, 1742,  520,
  227,  232,  520,  264,  149,  709,  713,   46,  753,  751,
  171,  752,  857,  754,  709,  715,  515,  192,  716, 1359,
  520,  520,  247,  712,  713,  247,  857,  227,  626,  192,
  238,  239,  716, 1739,  857,  712,   42,  857,  857,  615,
  247, 1874,  253, 1506,  712, 1399,  175,  857,  106,  616,
  752,  361,  824,  908,  374,  322,  192,  304,  709,  363,
  821,  308,  192,  857,  192,  515,   46,  709,  562,  611,
  834, 1715,  375,  833,  309,  213, 1598,  753,  751,  777,
  752, 1603,  754,  253,  108,  857,  253,  626,  712,  540,
  857,  362, 1716, 1400, 1575,  959,  172,  712, 1717,  716,
 1633,  253,  857,  192,  857,  824,  376, 1288,  824,  752,
  709,  192, 1544,  473, 1695,  824,  213,  824,  363,  213,
 1289,  824, 1545, 1383,  709,  313,  824,  173,  611,  824,
  540,  492, 1718,  540,  213, 1634,  536,  141,  777, 1820,
  712,  160,  174, 1290,  824,  541, 1653, 1635,  540,  109,
  540,  857,  483,  329,  712, 1384,  379, 1291,  824,  110,
  314,  184, 1423,  168,  483,  909,  824,  397,   40,  824,
  824,  857,  749,  184,  142,  857,  130,  750,  322,  824,
  192,  574,  192,  616,  111,  579,  994, 1055,  560,  758,
  492,  709,  708,  347, 1292,  824,  141,  483,  322,  483,
  184,  398,  221,  399,  708,  249,  184,  714,  184,  357,
 1660, 1161, 1161,  708,  749,  379,  616,  824,  967,  750,
  671,  712,  824, 1503,  314,  400,  360,   40,  714,  192,
  671, 1834,  671,  142,  824,  130,  824,  192,  969,  213,
  358,  671,  616,  749,  705,  616,  249,  184,  750,  249,
  401, 1111, 1011,  520,  520, 1721,  250,  708,  382,  786,
  799,  616,  753,  751,  249,  752,  708,  754,  192,  345,
  364,  192,  714,  192, 1403,  729,  227,  227, 1799,  334,
  713,  714,  733,  824,  394,  671,  786,  616,  740,  741,
  742,  226, 1288, 1516,  671,  192,  393,  250,  213,  366,
  250,  713,  168,  824,  384, 1289, 1293,  824,  322,  708,
  729,  367,  749,  384,  384,  250,  385,  750,  253,  253,
  192,  405,  729,  708,  714,  539,  253, 1032, 1290,  825,
 1031, 1144,  226,  545, 1143,  226, 1770,  671,  714,  396,
  444,  445,  446,  781,  368,  713,  369,  540,  971,  671,
  226,  671, 1232,  577,  713,  578,  787,  960,  386,  964,
  789,  418,  540,  968,  970,  972,  626, 1815,  454,  455,
  825, 1423,  168,  825, 1533,  224, 1535,  753,  751, 1292,
  752, 1885,  754, 1886,  781,  416,  540,  781,  825,  420,
  708,  421,  540, 1148,  192,  540,  916,  713,  424,  626,
  425, 1016,  781, 1161,  509,  714, 1247,  611,  513,  758,
 1813,  713,  184, 1013,  253,  515, 1003,  672,  671, 1011,
  523, 1011,  524, 1816, 1253,  626,  562, 1252,  626, 1011,
  672,  159,  160,  345, 1794,  253,  227,  540,  672, 1823,
  611,  253, 1302, 1825,  626, 1143,  753,  751,  528,  752,
  562,  754, 1000,  672,  530, 1348,  527,  168, 1347, 1852,
  753,  751,  224,  752,  529,  754,  611,  672,  531,  611,
  626, 1837, 1007,  533, 1011,  253, 1353,  562,  713, 1352,
 1452,  249,  249, 1250, 1523,  611,  546,  758,  562,  249,
  470,  562,  540, 1031, 1112, 1113,  548,  749,  562,  540,
 1607,  569,  750,  758,  672, 1031,  338, 1036, 1031,  224,
  549,  611,  993,  753,  751,  573,  752,  540,  754, 1036,
  339,  932, 1036, 1648,  238,  239,  758,  221, 1839,  137,
  137, 1694,  250,  250,  758,  253, 1840, 1688, 1690, 1894,
  250,  961, 1893, 1689, 1691,  966,  560,  540,  558,  586,
 1895,  540,  982,  768,  512,  571, 1889,  989,  990,  753,
  751,   14,  752,  374,  754,  589,  374,  238,  226,  473,
  560, 1001, 1002, 1003, 1004, 1005,  473,  249,  585, 1012,
  607, 1503,  737, 1863,  610,  737, 1039,  620,  587,  246,
  621,  825,  625,  676,  680,  682,  684,  560,  249,  717,
  224,  719,  224,  720,  249, 1503,  721,  722,  560,  723,
  724,  560,  749,  725,  726,  541,  672,  750,  560,  781,
  781,  727,  728,  729,  730,  731,  330,  781,  250,  736,
  737,  738, 1503, 1039,  149, 1066,  739,  748,  249,  755,
  756,  574, 1073, 1503,  825, 1161, 1503,  825,  758,  250,
 1641,  759,  766, 1503,  825,  250,  825,  763,  767,  331,
  825,  753,  751,  238,  752,  825,  754,  784,  825,  851,
  792,  794,  805,  809,  814,  149,  810,  816,  149,  538,
  817,  749,  790,  825,  238,  332,  750,  820,  333,  250,
  238,  822,  580,  149,  825,  749,  826,  825,  249,  828,
  750,  835,  829,  859,  334,  825,  860,  861,  825,  825,
  851,  866, 1618,  851,  314,  781,  874,  875,  825,  781,
  538,  881,  882,  538,  238, 1038,  753,  751,  851,  752,
  335,  754,  883,  884,  825,  886,  781,  887,  538,  892,
  538,  898,  781,  899,  903,  904,  913,  915,  749,  250,
  916,  921,  922,  750,  923,  927,  825, 1170, 1171, 1175,
  182,  825, 1184, 1186, 1188,  984, 1196, 1199, 1202, 1204,
 1207, 1210,  992,  825,  996,  825,  781,  893, 1029,  316,
 1217, 1218,  268, 1220, 1020, 1222, 1025, 1026, 1225, 1226,
 1227, 1228, 1229, 1230,  749,  753,  751, 1242,  752,  750,
  754, 1027, 1033,  775, 1052, 1234,  507, 1239,  227, 1067,
  768, 1057, 1070, 1081, 1091, 1023,  269, 1090, 1093, 1224,
  753,  751,  825,  752, 1185,  754, 1189,  838, 1197, 1200,
 1106, 1205, 1208, 1211, 1097, 1098,  781, 1107, 1100, 1108,
 1115, 1122,  825, 1134, 1132, 1141,  825,  507, 1214,  227,
  507, 1155,  227, 1177,  473,  999, 1023, 1023, 1023, 1023,
 1023, 1023, 1023, 1215, 1223,  507, 1221,  227, 1231,  749,
 1250, 1256, 1275, 1023, 1023, 1023, 1023, 1023, 1251, 1272,
 1254, 1280, 1281, 1219,  112, 1285, 1287,  113, 1306,  114,
  115,  116,  117, 1295,  118, 1309,  749, 1331, 1349, 1329,
 1341,  750,  119,  120, 1351, 1355, 1023,  105, 1023, 1361,
  149,  149, 1362,  121,  122, 1363,  123, 1364,  149, 1372,
 1149, 1380, 1385,  124,  125, 1386,  126,  127, 1387,  128,
 1393,  851,  129,  130,  131,  132,  133,  538, 1394, 1401,
  134,  135, 1398, 1402, 1404,  106,  136,  137,  138, 1288,
  139,  140,  141, 1405,  538,  538,  538,  107, 1412, 1413,
  538,  749, 1289,  538, 1414, 1415,  750, 1428,  753,  751,
 1417,  752,  951,  754,  753,  751,  538,  752, 1418,  754,
 1419,  108,  538, 1420,  851, 1290, 1421,  851, 1422, 1041,
 1447, 1429,  753,  751,  851,  752,  851,  754,  473, 1291,
  851, 1511, 1444, 1513, 1514,  851,  149, 1801,  851, 1526,
 1527, 1406, 1407,  790, 1409, 1411, 1531, 1536, 1538, 1537,
  602, 1539, 1416,  851, 1548, 1562, 1574,  149, 1566, 1567,
  749, 1568, 1588,  149, 1569,  750, 1292,  851, 1570, 1571,
 1578, 1594, 1596, 1119, 1434,  851,  109, 1597,  851,  851,
 1602,  538, 1441, 1442, 1239,  749,  110, 1239,  851, 1234,
  750,  296, 1632, 1449,  473, 1451, 1609,  149, 1611, 1631,
 1638, 1645,  538, 1644,  851, 1651, 1652, 1023,  538, 1023,
 1023,  111,  507,  507,  238,  227, 1023,  538, 1656,  538,
  507, 1655, 1023,  152, 1658, 1664,  851, 1023, 1665, 1666,
 1023,  851, 1668,  507, 1669, 1667,  473,  538, 1674, 1675,
 1676, 1677,  538,  851, 1023,  851, 1678, 1679, 1680, 1023,
 1023, 1681, 1682, 1023, 1802, 1432,  753,  751, 1683,  752,
 1023,  754, 1023, 1023, 1686, 1692, 1023, 1696, 1687, 1712,
 1023,  538, 1023, 1722, 1023, 1556, 1023, 1724, 1293, 1727,
 1725, 1023, 1023, 1726, 1023, 1433,  753,  751, 1728,  752,
 1731,  754,  851,  753,  751, 1243,  752, 1729,  754, 1023,
 1023, 1732, 1023, 1741, 1743, 1733, 1748, 1747,  507, 1749,
  238, 1752,  851, 1023, 1753, 1023,  851, 1023, 1023, 1023,
 1754, 1023, 1755, 1758, 1023, 1023, 1759, 1760, 1761,  507,
 1762,  238, 1763,  749, 1023,  507, 1764,  238,  750,  749,
 1765, 1766,  517, 1604,  750, 1767, 1769, 1023, 1772, 1610,
 1023, 1779, 1670, 1795, 1614, 1806, 1796,  749, 1809, 1811,
 1824, 1619,  750, 1812, 1828, 1833, 1023, 1843, 1836,  507,
 1846,  238, 1023, 1023, 1844, 1849, 1853, 1023, 1860, 1861,
 1862, 1879, 1881,  517, 1884, 1865,  517, 1887, 1023, 1023,
 1876, 1023, 1892, 1877, 1902, 1239, 1905, 1023, 1903, 1239,
 1023,  517, 1023, 1023, 1023, 1023, 1906, 1023, 1435,  753,
  751,    1,  752,   28,  754, 1023, 1023,  745, 1657, 1909,
   30,  997,  199,  376,  781, 1003, 1023, 1023,  267, 1023,
  162,  661,  838,  471,  982,  908, 1023, 1023, 1023, 1023,
 1023,  278, 1023,   81,  209, 1023, 1023, 1023, 1023, 1023,
  752,  981,  189, 1023, 1023,  277,  705,  707, 1023, 1023,
 1023, 1023, 1023, 1023, 1023, 1023, 1003, 1003, 1003, 1003,
 1003, 1016, 1003,  404,  538, 1436,  753,  751,  725,  752,
  190,  754,  962,  320, 1003, 1003, 1003, 1003,   14, 1699,
  377,  749,  753,  751,  219,  752,  750,  754, 1437,  753,
  751,  318,  752,  328,  754,  294,  961,  319, 1438,  753,
  751,  321,  752,  329,  754,  734, 1003,  324, 1003,  357,
  342,  749,   27,   30,   31,   40,  750,  310,  749, 1515,
  311,  112, 1036,  750,  113,  312,  114,  115,  116,  117,
  783,  118, 1439,  753,  751,  785,  752,  522,  754,  119,
  120,  973,  974,  975,  976,  977,  978,  979,  980,  521,
  121,  122, 1751,  123,  914, 1154, 1028, 1379, 1756, 1757,
  124,  125,  282,  126,  127, 1381,  128, 1159, 1118,  129,
  130,  131,  132,  133,  267, 1377, 1369,  134,  135, 1735,
 1734,  876, 1771,  136,  137,  138, 1239,  139,  140,  141,
 1774, 1128,  591,  112, 1780, 1781,  113,  633,  114,  115,
  116,  117, 1068,  118,  567, 1308, 1358,  604,  517,  517,
  605,  119,  120,  517,  753,  751,  517,  752, 1095,  754,
 1370, 1875,  121,  122, 1810,  123, 1714, 1901, 1880,  517,
 1720, 1258,  124,  125,  749,  126,  127,  854,  128,  750,
 1882,  129,  130,  131,  132,  133, 1792, 1868, 1088,  134,
  135, 1659, 1543, 1705, 1017,  136,  137,  138,  858,  139,
  140,  141, 1440,  753,  751,  597,  752,  612,  754,  588,
 1586, 1133,  158,  900,  880,  559, 1125, 1003,  800, 1003,
 1003, 1058,  327, 1822,  804, 1054, 1003, 1534, 1284,  277,
  956, 1850, 1003, 1768, 1630, 1072,  813, 1003, 1034, 1408,
 1003,  749, 1392, 1279,  517, 1552,  750, 1639,  753,  751,
    0,  752,    0,  754, 1003, 1033, 1003,  749,    0, 1003,
 1003, 1446,  750, 1003,  749,  517,    0,    0,    0,  750,
 1003,  517, 1003, 1003,  749,    0, 1003,    0,    0,  750,
 1003,    0, 1003,    0, 1003,    0, 1003, 1699,    0,    0,
    0, 1003, 1003,    0, 1003,    0, 1033, 1033, 1033, 1033,
 1033, 1033, 1033,    0,    0,  517,    0,    0,  749, 1003,
 1003,    0, 1003,  750, 1033, 1033, 1033, 1033,    0,    0,
    0,    0,    0, 1003, 1869, 1003,    0, 1003, 1003, 1003,
    0, 1003,    0,    0, 1003, 1003,    0,    0,    0,    0,
    0,    0,    0,    0, 1003,    0, 1033,    0, 1033,    0,
    0,    0, 1640,  753,  751,    0,  752, 1003,  754,    0,
 1003,  540,    0,    0,    0,    0,    0, 1647,  753,  751,
    0,  752,    0,  754, 1617,    0, 1003,    0,    0,    0,
    0,    0, 1003, 1003,    0,    0,    0, 1003,    0,  749,
    0,    0,    0,    0,  750,    0,    0,    0, 1003, 1003,
    0, 1003,  540,    0,    0,  540,    0, 1003,    0,    0,
 1003,    0, 1003, 1003, 1003, 1003,    0, 1003,    0,    0,
  540,    0,    0,    0,    0, 1003, 1003, 1830,  753,  751,
    0,  752,    0,  754,    0,    0, 1003, 1003,  749, 1003,
    0,    0,    0,  750,    0,    0, 1003, 1003, 1003, 1003,
 1003,    0, 1003,    0,    0, 1003, 1003, 1003, 1003, 1003,
    0,    0,    0, 1003, 1003,    0,    0,    0, 1003, 1003,
 1003, 1003, 1003, 1003, 1003, 1003,  753,  751, 1649,  752,
    0,  754,    0,  749, 1605, 1606,    0,    0,  750,    0,
    0,    0, 1612, 1613,    0, 1615, 1616,    0,    0,    0,
    0,    0,    0, 1620, 1621, 1622, 1623,    0, 1624, 1625,
 1626, 1627, 1628, 1629,    0,    0,    0, 1033,    0, 1033,
 1033,    0,    0,    0,    0,    0, 1033,   89,    0,    0,
    0,    0, 1033,    0,   90,    0,    0, 1033,   91,   92,
 1033,    0,    0,    0,    0,    0,  535,   93,    0,    0,
    0,    0,    0,    0, 1033, 1038,    0,    0,   94, 1033,
 1033,    0,    0, 1033,    0,    0,    0,    0,    0,    0,
 1033,    0, 1033, 1033,    0,    0, 1033,    0,    0,    0,
 1033,    0, 1033,    0, 1033,    0, 1033,  535,  749,    0,
  535, 1033, 1033,  750, 1033,    0, 1038, 1038, 1038, 1038,
 1038, 1038, 1038,  749,    0,  535,    0,  535,  750, 1033,
 1033,    0, 1033,    0, 1038, 1038, 1038, 1038,    0,    0,
   95,    0,    0, 1033,    0, 1033,    0, 1033, 1033, 1033,
    0, 1033,    0,    0, 1033, 1033,   96,  540,  540,    0,
    0,   66,  540,    0, 1033,  540, 1038,    0, 1038,    0,
    0,   97,   67,    0,    0,   98,    0, 1033,  540,   68,
 1033,    0,    0,  749,    0,    0,   99,    0,  750,    0,
    0,    0,   69,    0,    0,    0, 1033,    0,    0,    0,
    0,    0, 1033, 1033,    0,  100,    0, 1033,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1033, 1033,
    0, 1033,    0,    0,    0,    0,    0, 1033,    0,    0,
 1033,  749, 1033, 1033, 1033, 1033,  750, 1033,    0,    0,
    0,    0,    0,    0,    0, 1033, 1033,    0,    0,   70,
    0,    0,    0,  540,    0,    0, 1033, 1033,    0, 1033,
    0,    0,    0,    0,  101,   71, 1033, 1033, 1033, 1033,
 1033,    0, 1033,    0,  540, 1033, 1033, 1033, 1033, 1033,
  540,   72,    0, 1033, 1033,    0,    0,    0, 1033, 1033,
 1033, 1033, 1033, 1033, 1033, 1033,   73,    0,    0,    0,
   74,    0,    0,   75,    0,    0,    0,    0,    0,    0,
    0,   76,    0,    0,  540,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  535,    0,    0,    0,    0,    0,
   77,    0,    0,    0,    0,    0,    0, 1038,    0, 1038,
 1038,  535,  535,  535,    0,    0, 1038,  535,    0,    0,
  535,    0, 1038,    0,    0,    0,    0, 1038,    0,    0,
 1038,    0,    0,  535,    0,    0,  540,    0,    0,  535,
    0,    0,    0,    0, 1038, 1034,    0,    0,    0, 1038,
 1038,    0,    0, 1038,    0,    0,    0,    0,    0,   78,
 1038,    0, 1038, 1038,    0,    0, 1038,    0,    0,    0,
 1038,    0, 1038,    0, 1038,    0, 1038,  540,    0,    0,
  540, 1038, 1038,    0, 1038,    0, 1034, 1034, 1034, 1034,
 1034, 1034, 1034,    0,    0,  540,    0,  540,    0, 1038,
 1038,    0, 1038,    0, 1034, 1034, 1034, 1034,  535,    0,
    0,    0,    0, 1038,    0, 1038,    0, 1038, 1038, 1038,
    0, 1038,    0,    0, 1038, 1038,    0,    0,    0,  535,
    0,    0,    0,    0, 1038,  535, 1034,    0, 1034,    0,
    0,    0,    0,    0,    0,    0,  535, 1038,    0,    0,
 1038,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  535,    0, 1038,    0,    0,  535,
    0,    0, 1038, 1038,    0,    0,    0, 1038,    0,    0,
    0,    0,    0,    0,  540,    0,    0,    0, 1038, 1038,
    0, 1038,    0,    0,    0,    0,    0, 1038,  535,    0,
 1038,    0, 1038, 1038, 1038, 1038,    0, 1038,    0,    0,
    0,    0,    0,    0,    0, 1038, 1038,    0,    0,    0,
    0,    0,    0,    0,    0,  540, 1038, 1038,  540, 1038,
    0,    0,    0,    0,    0,    0, 1038, 1038, 1038, 1038,
 1038,    0, 1038,  540,    0, 1038, 1038, 1038, 1038, 1038,
    0,    0,    0, 1038, 1038,    0,    0,    0, 1038, 1038,
 1038, 1038, 1038, 1038, 1038, 1038,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  540,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1034,    0, 1034,
 1034,    0,  540,  540,    0,    0, 1034,  540,    0,    0,
  540,    0, 1034,    0,    0,    0,    0, 1034,    0,    0,
 1034,    0,    0,  540,    0,    0,  247,    0,    0,  540,
    0,    0,    0,    0, 1034, 1047,    0,    0,    0, 1034,
 1034,    0,    0, 1034,    0,    0,    0,    0,    0,    0,
 1034,    0, 1034, 1034,    0,    0, 1034,    0,    0,    0,
 1034,    0, 1034,    0, 1034,    0, 1034,  247,    0,    0,
  247, 1034, 1034,    0, 1034,    0, 1047, 1047, 1047, 1047,
 1047, 1047, 1047,    0,    0,  247,    0,    0,    0, 1034,
 1034,    0, 1034,    0, 1047, 1047, 1047, 1047,  540,    0,
    0,    0,    0, 1034,    0, 1034,    0, 1034, 1034, 1034,
    0, 1034,    0,    0, 1034, 1034,    0,    0,    0,  540,
    0,    0,    0,    0, 1034,  540, 1047,    0, 1047,    0,
    0,    0,    0,    0,    0,    0,  540, 1034,    0,    0,
 1034,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  540,  540,    0,    0,  540,    0, 1034,    0,  540,  540,
    0,    0, 1034, 1034,    0,    0,    0, 1034,    0,    0,
    0,  540,    0,    0,    0,    0,    0,    0, 1034, 1034,
    0, 1034,    0,    0,    0,    0,    0, 1034,  540,    0,
 1034,    0, 1034, 1034, 1034, 1034,    0, 1034,    0,    0,
    0,    0,    0,    0,    0, 1034, 1034,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1034, 1034,    0, 1034,
    0,    0,    0,    0,    0,    0, 1034, 1034, 1034, 1034,
 1034,    0, 1034,    0,    0, 1034, 1034, 1034, 1034, 1034,
    0,    0,    0, 1034, 1034,    0,  540,    0, 1034, 1034,
 1034, 1034, 1034, 1034, 1034, 1034,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  540,    0,    0,
    0,    0,    0,  540,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1047,    0, 1047,
 1047,    0,  247,  247,    0,    0, 1047,  540,    0,    0,
  247,    0, 1047,    0,    0,    0,    0, 1047,    0,    0,
 1047,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1047, 1052,    0,    0,    0, 1047,
 1047,    0,    0, 1047,    0,    0,    0,    0,    0,    0,
 1047,    0, 1047, 1047,    0,    0, 1047,    0,    0,    0,
 1047,    0, 1047,    0, 1047,    0, 1047,    0,    0,    0,
    0, 1047, 1047,    0, 1047,    0, 1052, 1052, 1052, 1052,
 1052, 1052, 1052,    0,    0,    0,    0,    0,    0, 1047,
 1047,    0, 1047,    0, 1052, 1052, 1052, 1052,  247,    0,
    0,    0,    0, 1047,    0, 1047,    0, 1047, 1047, 1047,
    0, 1047,    0,    0, 1047, 1047,    0,    0,    0,  247,
    0,    0,    0,    0, 1047,  247, 1052,    0, 1052,    0,
    0,    0,    0,    0,    0,    0,    0, 1047,    0,    0,
 1047,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1047,    0,    0,  247,
    0,    0, 1047, 1047,    0,    0,    0, 1047,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1047, 1047,
    0, 1047,    0,    0,    0,    0,    0, 1047,    0,    0,
 1047,    0, 1047, 1047, 1047, 1047,    0, 1047,    0,    0,
    0,    0,    0,    0,    0, 1047, 1047,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1047, 1047,    0, 1047,
    0,    0,    0,    0,    0,    0, 1047, 1047, 1047, 1047,
 1047,    0, 1047,    0,    0, 1047, 1047, 1047, 1047, 1047,
    0,    0,    0, 1047, 1047,    0,    0,    0, 1047, 1047,
 1047, 1047, 1047, 1047, 1047, 1047,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1052,    0, 1052,
 1052,    0,    0,    0,    0,    0, 1052,    0,    0,    0,
    0,    0, 1052,    0,    0,    0,    0, 1052,    0,    0,
 1052,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1052,  876,    0,    0,    0, 1052,
 1052,    0,    0, 1052,    0,    0,    0,    0,    0,    0,
 1052,    0, 1052, 1052,    0,    0, 1052,    0,    0,    0,
 1052,    0, 1052,    0, 1052,    0, 1052,    0,    0,    0,
    0, 1052, 1052,    0, 1052,    0,  876,  876,  876,  876,
  876,    0,  876,    0,    0,    0,    0,    0,    0, 1052,
 1052,    0, 1052,    0,  876,  876,  876,  876,    0,    0,
    0,    0,    0, 1052,    0, 1052,    0, 1052, 1052, 1052,
    0, 1052,    0,    0, 1052, 1052,    0,    0,    0,    0,
    0,    0,    0,    0, 1052,    0,    0,    0,  876,    0,
    0,    0,    0,    0,    0,    0,    0, 1052,    0,    0,
 1052,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1041,    0,    0,    0,    0,    0, 1052,    0,    0,    0,
    0,    0, 1052, 1052,    0,    0,    0, 1052,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1052, 1052,
    0, 1052,    0,    0,    0,    0,    0, 1052,    0,    0,
 1052,    0, 1052, 1052, 1052, 1052,    0, 1052,    0,    0,
    0,    0,    0,    0,    0, 1052, 1052,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1052, 1052,    0, 1052,
    0,    0,    0,    0,    0,    0, 1052, 1052, 1052, 1052,
 1052,    0, 1052,    0,    0, 1052, 1052, 1052, 1052, 1052,
    0,    0,    0, 1052, 1052,    0,    0,    0, 1052, 1052,
 1052, 1052, 1052, 1052, 1052, 1052,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  876,    0,  876,
  876,    0,    0,    0,    0,    0,  876,    0,    0,    0,
    0,    0,  876,    0,    0,    0,    0,  876,    0,    0,
  876,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  876,  889,    0,    0,    0,  876,
  876,    0,    0,  876,    0,    0,    0,    0,    0,    0,
  876,    0,  876,  876,    0,    0,  876,    0,    0,    0,
  876,    0,  876,    0,  876,    0,  876,    0,    0,    0,
    0,  876,  876,    0,  876,    0,  889,  889,  889,  889,
  889,    0,  889,    0,    0,    0,    0,    0,    0,  876,
  876,    0,  876,    0,  889,  889,  889,  889,    0,    0,
    0,    0,    0,  876,    0,  876,    0,  876,  876,  876,
    0,  876,    0,    0,  876,  876,    0,    0,    0,    0,
    0,    0,    0,    0,  876,    0,    0,    0,  889,    0,
    0,    0,    0,    0,    0,    0,    0,  876,    0,    0,
  876,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1014,    0,    0,    0,  876,    0,    0,    0,
    0,    0,  876,  876,    0,    0,    0,  876,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  876,  876,
    0,  876,    0,    0,    0,    0,    0,  876,    0,    0,
  876,    0,  876,  876,  876,  876,    0,  876,    0,    0,
    0,    0,    0,    0,    0,  876,  876,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  876,  876,    0,  876,
    0,    0,    0,    0,    0,    0,  876,  876,  876,  876,
  876,    0,  876,    0,    0,  876,  876,  876,  876,  876,
    0,    0,    0,  876,  876,    0,    0,    0,  876,  876,
  876,  876,  876,  876,  876,  876,    0,    0,    0,    0,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  121,  122,    0,  123,    0,    0,  889,    0,  889,
  889,  124,  125,    0,  126,  127,  889,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,  889,  134,  135,
  889,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,  889,  892,    0,    0,    0,  889,
  889,    0,    0,  889,    0,    0,    0,    0,    0,    0,
  889,    0,  889,  889,    0,    0,  889,    0,    0,    0,
  889,    0,  889,    0,  889,    0,  889,    0,    0,    0,
    0,  889,  889,    0,  889,    0,  892,  892,  892,  892,
  892,    0,  892,    0,    0,    0,    0,    0,    0,  889,
  889,    0,  889,    0,  892,  892,  892,  892,    0,    0,
    0,    0,    0,  889,    0,  889,    0,  889,  889,  889,
    0,  889,    0,    0,  889,  889,    0,    0,    0,    0,
    0,    0,    0,    0,  889,    0,    0,    0,  892,    0,
    0,    0,    0,    0,    0,    0,    0,  889,    0,    0,
  889,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  889,    0,    0,    0,
    0,    0,  889,  889,    0,    0,    0,  889,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  889,  889,
    0,  889,    0,    0,    0,    0,    0,  889,    0,    0,
  889,    0,  889,  889,  889,  889,    0,  889,    0,    0,
    0,    0,    0,    0,    0,  889,  889,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  889,  889,    0,  889,
    0,    0,    0,    0,    0,    0,  889,  889,  889,  889,
  889,    0,  889,    0,    0,  889,  889,  889,  889,  889,
    0,    0,    0,  889,  889,    0,    0,    0,  889,  889,
  889,  889,  889,  889,  889,  889,    0,    0,    0,    0,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  121,  122,    0,  123,    0,    0,  892,    0,  892,
  892,  124,  125,    0,  126,  127,  892,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
  892,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,  892,  893,    0,    0,    0,  892,
  892,    0,    0,  892,    0,    0,    0,    0,    0,    0,
  892,    0,  892,  892,    0,    0,  892,    0,    0,    0,
  892,    0,  892,    0,  892,    0,  892,    0,    0,    0,
    0,  892,  892,    0,  892,    0,  893,  893,  893,  893,
  893,    0,  893,    0,    0,    0,    0,    0,    0,  892,
  892,    0,  892,    0,  893,  893,  893,  893,    0,    0,
    0,    0,    0,  892,    0,  892,    0,  892,  892,  892,
    0,  892,    0,    0,  892,  892,    0,    0,    0,    0,
    0,    0,    0,    0,  892,    0,    0,    0,  893,    0,
    0,    0,    0,    0,    0,    0,    0,  892,    0,    0,
  892,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  892,    0,    0,    0,
    0,    0,  892,  892,    0,    0,    0,  892,    0,    0,
    0,    0,    0, 1311,    0,    0,    0,    0,  892,  892,
    0,  892,    0,    0,    0,    0,    0,  892,    0,    0,
  892,    0,  892,  892,  892,  892,    0,  892,    0,    0,
    0,    0,    0,    0,    0,  892,  892,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  892,  892,    0,  892,
    0,    0,    0,    0,    0,    0,  892,  892,  892,  892,
  892,    0,  892,    0,    0,  892,  892,  892,  892,  892,
    0,    0,    0,  892,  892,    0,    0,    0,  892,  892,
  892,  892,  892,  892,  892,  892,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    1,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  893,    0,  893,
  893,    0,    0,    0,    2,    0,  893,    0,    0,    0,
    0,    0,    3,    0,    0,    0,    0,    0,    0,    4,
  893,    5,    0,    6,  887,    0,    0,    0,    7,    0,
    0,    0,    0,    0,  893,    8,    0,    0,    0,  893,
  893,    0,    0,  893,    0,    0,    0,    0,    0,    9,
  893,    0,  893,  893,    0,    0,  893,    0,    0,   10,
  893,    0,  893,    0,  893,  887,  893,  887,  887,  887,
    0,  893,  893,    0,  893,    0,    0,    0,    0,    0,
    0,    0,    0,  887,  887,  887,  887,    0,    0,  893,
  893,    0,  893,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  893,    0,  893,    0,  893,  893,  893,
    0,  893,    0,    0,  893,  893,    0,  887,    0,    0,
  432,    0,   11,    0,  893,    0,    0,    0,    0,   12,
    0,    0,   13,    0,   14,   15,    0,  893,    0,    0,
  893,    0,    0,    0,    0,    0,    0,    0,  434,    0,
    0,    0,    0,    0,    0,    0,  893,    0,    0,   16,
    0,    0,  893,  893,    0,    0,    0,  893,    0,    0,
    0,    0,    0,    0,    0,    0,  436,    0,  893,  893,
    0,  893,    0,    0,    0,    0,    0,  893,    0,    0,
  893,    0,  893,  893,  893,  893,    0,  893,    0,    0,
    0,    0,    0,    0,    0,  893,  893,    0,   17,    0,
    0,    0,    0,    0,    0,    0,  893,  893,    0,  893,
    0,    0,    0,   18,   19,    0,  893,  893,  893,  893,
  893,    0,  893,    0,    0,  893,  893,  893,  893,  893,
    0,    0,    0,  893,  893,    0,    0,  441,  893,  893,
  893,  893,  893,  893,  893,  893,    0,    0,    0,    0,
    0,    0,    0,  443,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  444,  445,  446,
    0,  238,  239,    0,    0,    0,  887,    0,  887,  887,
    0,    0,    0,    0,    0,  887,    0,    0,    0,    0,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  887,
    0,    0,    0,  886,    0,  459,  460,    0,    0,    0,
    0,    0,    0,  887,  464,    0,    0,    0,  887,  887,
    0,    0,  887,    0,    0,    0,    0,    0,    0,  887,
    0,  887,  887,    0,    0,  887,    0,    0,    0,  887,
    0,  887,    0,  887,  886,  887,  886,  886,  886,    0,
  887,  887,    0,  887,  917,    0,    0,  635,    0,    0,
    0,    0,  886,  886,  886,  886,    0,    0,  887,  887,
  918,  887,    0,    0,  636,    0,    0,  637,    0,    0,
    0,    0,  887,    0,  887,    0,  887,  887,  887,    0,
  887,    0,    0,  887,  887,    0,  886,    0,    0,    0,
    0,    0,    0,  887,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  887,  638,    0,  887,
    0,    0,    0,  639,  640,  641,    0,  642,  643,  644,
    0,  645,    0,    0,    0,  887,    0,    0,    0,    0,
    0,  887,  887,    0,    0,    0,  887,    0,    0,  646,
    0,  647,    0,    0,    0,    0,    0,  887,  887,    0,
  887,    0,    0,    0,    0,    0,  887,    0,    0,  887,
    0,  887,  887,  887,  887,    0,  887,    0,    0,    0,
  648,    0,    0,    0,  887,  887,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  887,  887,  649,  887,    0,
    0,    0,    0,    0,    0,  887,  887,  887,  887,  887,
    0,  887,    0,    0,  887,  887,  887,  887,  887,  650,
    0,    0,  887,  887,    0,    0,    0,  887,  887,  887,
  887,  887,  887,  887,  887,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  651,  652,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  886,    0,  886,  886,    0,
    0,  653,    0,    0,  886,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  886,    0,
    0,    0,  888,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  886,    0,    0,    0,    0,  886,  886,    0,
    0,  886,    0,    0,    0,    0,    0,    0,  886,    0,
  886,  886,    0,    0,  886,    0,    0,    0,  886,    0,
  886,    0,  886,  888,  886,  888,  888,  888,    0,  886,
  886,    0,  886,    0,    0,    0,    0,    0,    0,    0,
    0,  888,  888,  888,  888,    0,    0,  886,  886,    0,
  886,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  886,    0,  886,    0,  886,  886,  886,    0,  886,
    0,    0,  886,  886,    0,  888,    0,    0,    0,    0,
    0,    0,  886,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  886,    0,    0,  886,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  886,    0,    0,    0,    0,    0,
  886,  886,    0,    0,    0,  886,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  886,  886,    0,  886,
    0,    0,    0,    0,    0,  886,    0,    0,  886,    0,
  886,  886,  886,  886,    0,  886,    0,    0,    0,    0,
    0,    0,    0,  886,  886,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  886,  886,    0,  886,    0,    0,
    0,    0,    0,    0,  886,  886,  886,  886,  886,  828,
  886,    0,    0,  886,  886,  886,  886,  886,    0,    0,
    0,  886,  886,    0,    0,    0,  886,  886,  886,  886,
  886,  886,  886,  886,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  828,    0,    0,  828,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  888,    0,  888,  888,  828,    0,
    0,    0,    0,  888,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  888,    0,    0,
    0,  891,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  888,    0,    0,    0,    0,  888,  888,    0,    0,
  888,    0,    0,    0,    0,    0,    0,  888,    0,  888,
  888,    0,    0,  888,    0,    0,    0,  888,    0,  888,
    0,  888,  891,  888,  891,  891,  891,    0,  888,  888,
    0,  888,    0,    0,    0,    0,    0,    0,    0,    0,
  891,  891,  891,  891,    0,    0,  888,  888,    0,  888,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  888,    0,  888,    0,  888,  888,  888,    0,  888,    0,
    0,  888,  888,    0,  891,    0,    0,    0,    0,    0,
    0,  888,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  888,    0,    0,  888,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  888,    0,    0,    0,    0,    0,  888,
  888,    0,    0,    0,  888,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  888,  888,    0,  888,    0,
    0,    0,    0,    0,  888,    0,    0,  888,    0,  888,
  888,  888,  888,    0,  888,    0,    0,    0,    0,    0,
    0,  828,  888,  888,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  888,  888,    0,  888,    0,    0,    0,
    0,    0,    0,  888,  888,  888,  888,  888,    0,  888,
  697,    0,  888,  888,  888,  888,  888,    0,    0,    0,
  888,  888,    0,    0,    0,  888,  888,  888,  888,  888,
  888,  888,  888,    0,  828,    0,    0,  828,    0,    0,
    0,    0,    0,    0,  828,    0,  828,    0,    0,    0,
  828,  697,  799,    0,  697,  828,    0,    0,  828,    0,
    0,    0,    0,  891,    0,  891,  891,    0,    0,  697,
    0,    0,  891,  828,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  891,  828,    0,    0,
    0,    0,    0,  799,    0,  828,  799,    0,  828,  828,
  891,    0,    0,    0,    0,  891,  891,    0,  828,  891,
    0,  799,    0,    0,    0,    0,  891,    0,  891,  891,
    0,    0,  891,    0,  828,    0,  891,    0,  891,    0,
  891,    0,  891,    0,    0,    0,    0,  891,  891,    0,
  891,    0,    0,    0,    0,    0,  828,    0,    0,    0,
    0,  828,    0,    0,    0,  891,  891,    0,  891,    0,
 1014,    0,    0,  828,    0,  828,    0,    0,    0,  891,
    0,  891,    0,  891,  891,  891,    0,  891,    0,    0,
  891,  891,    0,    0,    0,    0,    0,    0,    0,    0,
  891,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  891,    0,    0,  891,    0,    0,    0,
    0,    0,  828,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  891,    0,    0,  668,    0,    0,  891,  891,
    0,    0,  828,  891,    0,    0,  828,    0,    0,    0,
    0,    0,    0,    0,  891,  891,    0,  891,    0,    0,
    0,    0,    0,  891,    0,    0,  891,    0,  891,  891,
  891,  891,    0,  891,    0,  668,  668,    0,    0,  668,
    0,  891,  891,    0,  697,    0,    0,    0,    0,    0,
    0,    0,  891,  891,  668,  891,    0,    0,    0,    0,
    0,    0,  891,  891,  891,  891,  891,    0,  891,    0,
    0,  891,  891,  891,  891,  891,    0,    0,    0,  891,
  891,    0,    0,    0,  891,  891,  891,  891,  891,  891,
  891,  891,    0,    0,    0,  697,    0,    0,  697,    0,
    0,    0,    0,    0,    0,  697,    0,  697,    0,    0,
    0,  697,    0,    0,    0,    0,  697,    0,    0,  697,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  697,    0,    0,  799,    0,    0,
  799,    0,    0,    0,    0,    0,    0,  799,  697,  799,
    0,    0,    0,  799,    0,    0,  697,    0,  799,    0,
  697,  799,    0,  665,    0,    0,    0,    0,    0,  697,
    0,    0,    0,    0,    0,    0,  799,    0,    0,    0,
    0, 1013,    0,    0,    0,  697,    0,    0,    0,    0,
  799,    0,    0,    0,    0,    0,    0,    0,  799,    0,
    0,  799,  799,    0,  695,    0,    0,    0,    0,    0,
    0,  799,  697,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  697,    0,  697,  799,    0,    0,
    0,    0,  697,    0,    0,  697,    0,  697,  697,  697,
  697,    0,  697,    0,    0,  695,    0,    0,  695,  799,
  697,  697,    0,    0,  799,    0,    0,    0,    0,    0,
    0,  697,  697,  695,  697,    0,  799,    0,  799,    0,
    0,  697,  697,  697,  697,  697,    0,  697,    0,    0,
  697,  697,  697,  697,  697,    0,    0,    0,  697,  697,
    0,    0,    0,  697,  697,  697,  697,  697,  697,  697,
  697,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  668,    0,    0,  668,    0,  799,    0,    0,    0,    0,
  668,    0,  668,    0,    0,    0,  668,    0,    0,    0,
    0,  668,    0,    0,  668,  799,    0,    0,    0,  799,
  112,    0,    0,  113,    0,  114,  115,  116,  117,  668,
  118,    0,    0,    0,    0,    0,    0,    0,  119,  120,
    0,    0,    0,  668,    0,    0,    0,    0,    0,  121,
  122,  668,  123,    0,    0,  668,    0,    0,    0,  124,
  125,    0,  126,  127,  668,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,    0,  134,  135,    0,    0,
  668,    0,  136,  137,  138,    0,  139,  140,  141,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  668,    0,    0,
    0,    0,    0,    0,    0,    0,  665,    0,    0,  668,
    0,  668,    0,    0,    0,    0,    0,  668,    0,    0,
  668,    0,  668,  668,  668,  668,    0,  668,    0,    0,
    0,    0,    0,    0,    0,  668,  668,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  668,  668,    0,  668,
    0,    0,    0,    0,    0,    0,  668,  668,  668,  668,
  668,    0,  668,    0,    0,  668,  668,  668,  668,  668,
    0,    0,    0,  668,  668,  867,    0,    0,  668,  668,
  668,  668,  668,  668,  668,  668,    0,    0,    0,  695,
    0,    0,  695,    0,    0,    0,    0,    0,    0,  695,
    0,  695,    0,    0,    0,  695,    0,    0,    0,    0,
  695,    0,    0,  695,    0,    0,  867, 1041, 1041,  867,
 1041, 1041, 1041,    0,    0,    0,    0,    0,  695,    0,
    0,    0,    0,    0,  867, 1041, 1041, 1041,    0,    0,
    0,  668,  695,    0,  668,    0,  668,  668,  668,  668,
  695,  668,    0,    0,  695,    0,    0,    0,    0,  668,
  668,    0,    0,  695,    0,    0, 1041,    0,    0,    0,
  668,  668,    0,  668,    0,    0,    0,    0,    0,  695,
  668,  668,    0,  668,  668,    0,  668,    0,    0,  668,
  668,  668,  668,  668,    0,    0,    0,  668,  668,    0,
    0,    0,    0,  668,  668,  668,  695,  668,  668,  668,
    0,    0,    0,    0,    0,    0,    0,    0,  695,    0,
  695,    0,    0,    0,    0,    0,  668,    0,    0,  668,
    0,  668,  668,  668,  668,    0,  668,    0,    0,    0,
    0,    0,    0,    0,  668,  668,    0,    0,    0,    0,
    0,    0,    0,    0,  868,  668,  668,    0,  668,    0,
    0,    0,    0,    0,    0,  668,  668,  695,  668,  668,
    0,  668,    0,    0,  668,  668,  668,  668,  668,    0,
    0,    0,  668,  668,    0,    0,    0,  695,  668,  668,
  668,  695,  668,  668,  668,  868, 1042, 1042,  868, 1042,
 1042, 1042,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  868, 1042, 1042, 1042,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  867,    0,    0,
    0,    0,    0,    0,    0, 1042, 1041,    0,    0,    0,
    0,    0, 1041,    0,    0,    0,    0, 1041,    0,    0,
 1041,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  869,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  867,    0,    0,  867,    0,    0, 1041,    0,    0,    0,
  867,    0,  867,    0, 1041,    0,  867,    0,    0,    0,
    0,  867, 1041,    0,  867,    0,    0,  869, 1043, 1043,
  869, 1043, 1043, 1043,    0,    0,    0,    0,    0,  867,
 1041,    0, 1041,    0,    0,  869, 1043, 1043, 1043,    0,
    0,    0,    0,  867,    0, 1041,    0, 1041, 1041, 1041,
    0,  867,    0,    0,  867,  867,    0,    0,    0,  826,
    0,    0,    0,    0,  867,    0,    0, 1043,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  867,    0,    0,    0,    0,  545,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1041,    0,    0,    0,
  826,    0,  867,  826,    0,    0,    0,  867,    0,    0,
    0,    0,    0,    0,    0,    0,  868,    0,  826,  867,
    0,  867,    0,    0,    0, 1042,  545,    0,    0,  545,
    0, 1042,    0,    0,    0,    0, 1042,    0,    0, 1042,
    0,    0,    0,  827,  545,    0,  545,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  867,  868,
    0,    0,  868,    0,    0, 1042,  545,    0,    0,  868,
    0,  868,    0, 1042,  827,  868,    0,  827,  867,    0,
  868, 1042,  867,  868,    0,    0,    0,    0,    0,    0,
    0,    0,  827,    0,    0,    0,    0,    0,  868, 1042,
    0, 1042,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  868,    0, 1042,    0, 1042, 1042, 1042,    0,
  868,    0,    0,  868,  868,    0,    0,    0,  869,    0,
    0,    0,    0,  868,    0,    0,    0, 1043,    0,    0,
    0,    0,    0, 1043,    0,    0,    0,    0, 1043,  868,
    0, 1043,    0,    0,    0,  859,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1042,    0,    0,    0,    0,
    0,  868,    0,    0,    0,    0,  868,    0,    0,    0,
    0,  869,    0,    0,  869,  821,    0, 1043,  868,    0,
  868,  869,    0,  869,    0, 1043,  859,  869,    0,  859,
    0,    0,  869, 1043,    0,  869,    0,    0,    0,    0,
    0,  826,    0,    0,  859,    0,    0,    0,    0,    0,
  869, 1043,    0, 1043,    0,    0,  821,    0,    0,  821,
    0,    0,    0,  545,  869,    0, 1043,  868, 1043, 1043,
 1043,    0,  869,    0,  821,  869,  869,    0,  545,    0,
  545,  545,  545,    0,    0,  869,  545,  868,    0,  545,
    0,  868,    0,    0,  826,    0,    0,  826,    0,    0,
    0,  869,  545,    0,  826,    0,  826,    0,  545,    0,
  826,  545,    0,    0,    0,  826,    0, 1043,  826,    0,
    0,    0,    0,  869,    0,  827,    0,    0,  869,    0,
    0,    0,    0,  826,    0,    0,    0,    0,  822,    0,
  869,    0,  869,    0,    0,    0,    0,  826,    0,    0,
    0,    0,    0,  545,    0,  826,    0,    0,  826,  826,
    0,    0,    0,    0,    0,    0,    0,    0,  826,    0,
    0,    0,    0,    0,    0,    0,    0,  545,  827,  822,
    0,  827,  822,    0,  826,    0,    0,    0,  827,  869,
  827,    0,    0,    0,  827,    0,    0,  822,  545,  827,
    0,    0,  827,    0,  545,    0,  826,    0,  545,  869,
    0,  826,    0,  869,    0,  545,    0,  827,    0,    0,
  823,    0,    0,  826,    0,  826,    0,    0,    0,    0,
    0,  827,    0,  545,    0,    0,    0,    0,  545,  827,
    0,    0,  827,  827,    0,    0,    0,  859,    0,    0,
    0,    0,  827,    0,    0,    0,    0,    0,    0,    0,
    0,  823,    0,  545,  823,    0,    0,  545,  827,    0,
    0,    0,  826,    0,    0,    0,    0,  821,    0,  823,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  827,    0,  826,    0,    0,  827,  826,    0,    0,    0,
  859,    0,    0,  859,    0,    0,    0,  827,    0,  827,
  859,    0,  859,    0,  858,    0,  859,    0,    0,    0,
    0,  859,    0,    0,  859,    0,    0,    0,    0,    0,
  821,    0,    0,  821,    0,    0,    0,    0,    0,  859,
  821,    0,  821,    0,    0,    0,  821,    0,    0,    0,
    0,  821,    0,  859,  821,  858,  827,    0,  858,    0,
    0,  859,    0,    0,  859,  859,    0,    0,    0,  821,
    0,    0,    0,  858,  859,    0,  827,    0,    0,    0,
  827,    0,    0,  821,    0,    0,    0,    0,    0,    0,
  859,  821,    0,    0,  821,  821,  852,    0,    0,    0,
  822,    0,    0,    0,  821,    0,    0,    0,    0,    0,
    0,    0,  859,    0,    0,    0,    0,  859,    0,    0,
  821,    0,    0,    0,    0,    0,    0,    0,    0,  859,
    0,  859,    0,    0,    0,    0,    0,  852,    0,    0,
  852,    0,  821,    0,    0,    0,    0,  821,    0,    0,
    0,    0,    0,  822,    0,  852,  822,    0,    0,  821,
    0,  821,    0,  822,    0,  822,    0,    0,    0,  822,
    0,    0,    0,    0,  822,    0,    0,  822,  859,    0,
    0,    0,  823,    0,    0,  860,    0,    0,    0,    0,
    0,    0,  822,    0,    0,    0,    0,    0,  859,    0,
    0,    0,  859,    0,    0,    0,  822,    0,  821,    0,
    0,    0,    0,    0,  822,    0,    0,  822,  822,  861,
    0,    0,    0,    0,    0,    0,  860,  822,  821,  860,
    0,    0,  821,    0,    0,  823,    0,    0,  823,    0,
    0,    0,    0,  822,  860,  823,    0,  823,    0,    0,
    0,  823,    0,    0,    0,    0,  823,    0,    0,  823,
  861,    0,    0,  861,    0,  822,    0,    0,    0,    0,
  822,    0,    0,    0,  823,    0,  858,    0,  861,  849,
    0,    0,  822,    0,  822,    0,    0,    0,  823,    0,
    0,    0,    0,    0,    0,    0,  823,    0,    0,  823,
  823,    0,    0,    0,    0,    0,    0,    0,    0,  823,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  849,    0,    0,  849,    0,  823,    0,    0,    0,  858,
    0,  822,  858,    0,    0,    0,    0,    0,  849,  858,
    0,  858,    0,    0,    0,  858,    0,  823,    0,    0,
  858,  822,  823,  858,    0,  822,    0,    0,  852,    0,
    0,  847,    0,    0,  823,    0,  823,    0,  858,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  858,    0,    0,    0,    0,    0,    0,    0,
  858,    0,    0,  858,  858,  853,    0,    0,    0,    0,
    0,    0,  847,  858,    0,  847,    0,    0,    0,    0,
    0,  852,    0,  823,  852,    0,    0,    0,    0,  858,
  847,  852,    0,  852,    0,    0,    0,  852,    0,    0,
    0,    0,  852,  823,    0,  852,  853,  823,    0,  853,
    0,  858,    0,    0,    0,    0,  858,  860,    0,    0,
  852,    0,    0,    0,  853,  862,    0,    0,  858,    0,
  858,    0,    0,    0,  852,    0,    0,    0,    0,    0,
    0,    0,  852,    0,    0,  852,  852,    0,    0,    0,
    0,  861,    0,    0,    0,  852,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  862,    0,    0,  862,
  860,  852,    0,  860,    0,    0,    0,  858,    0,    0,
  860,    0,  860,    0,  862,    0,  860,    0,    0,    0,
    0,  860,    0,  852,  860,    0,    0,  858,  852,    0,
    0,  858,    0,    0,  861,    0,    0,  861,    0,  860,
  852,  849,  852,    0,  861,    0,  861,    0,    0,    0,
  861,    0,    0,  860,    0,  861,    0,    0,  861,    0,
    0,  860,    0,    0,  860,  860,    0,    0,    0,    0,
    0,    0,    0,  861,  860,  848,    0,    0,   82,    0,
    0,    0,    0,    0,    0,    0,    0,  861,    0,  852,
  860,    0,    0,    0,  849,  861,    0,  849,  861,  861,
    0,    0,    0,    0,  849,    0,  849,    0,  861,  852,
  849,    0,  860,  852,    0,  849,  848,  860,  849,  848,
    0,    0,    0,  847,  861,    0,    0,    0,    0,  860,
    0,  860,    0,  849,  848,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  861,  849,    0,    0,
    0,  861,    0,    0,    0,  849,    0,  853,  849,  849,
    0,    0,    0,  861,    0,  861,    0,    0,  849,    0,
    0,    0,    0,    0,    0,    0,  847,    0,  860,  847,
    0,    0,    0,    0,  849,    0,  847,    0,  847,    0,
    0,    0,  847,    0,    0,    0,    0,  847,  860,    0,
  847,    0,  860,    0,    0,    0,  849,    0,    0,    0,
  853,  849,  861,  853,    0,  847,    0,  862,    0,    0,
  853,    0,  853,  849,    0,  849,  853,    0,    0,  847,
    0,  853,  861,    0,  853,    0,  861,  847,    0,    0,
  847,  847,    0,    0,    0,    0,    0,    0,    0,  853,
  847,    0,    0,    0,  850,    0,    0,    0,    0,    0,
    0,    0,    0,  853,    0,    0,  847,    0,    0,    0,
  862,  853,  849,  862,  853,  853,    0,    0,    0,    0,
  862,    0,  862,    0,  853,    0,  862,    0,  847,    0,
    0,  862,  849,  847,  862,  850,  849,    0,  850,    0,
  853,    0,    0,    0,    0,  847,    0,  847,    0,  862,
    0,    0,    0,  850,  854,    0,    0,    0,    0,    0,
    0,    0,  853,  862,    0,    0,    0,  853,    0,    0,
    0,  862,    0,    0,  862,  862,    0,  848,    0,  853,
    0,  853,    0,    0,  862,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  847,  854,    0,    0,  854,    0,
  862,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  854,  847,    0,    0,  679,  847,    0,
    0,    0,  862,    0,    0,    0,    0,  862,  853,    0,
  848,    0,    0,  848,    0,    0,    0,    0,    0,  862,
  848,  862,  848,    0,    0,    0,  848,    0,  853,    0,
    0,  848,  853,    0,  848,    0,    0,  679,  679,    0,
    0,  679,    0,    0,    0,  681,    0,    0,    0,  848,
    0,    0,    0,    0,    0,    0,  679,    0,    0,    0,
    0,    0,    0,  848,    0,    0,    0,    0,  862,    0,
    0,  848,    0,    0,  848,  848,    0,    0,    0,    0,
    0,    0,    0,    0,  848,    0,  681,    0,  862,  681,
    0,    0,  862,    0,    0,    0,    0,    0,    0,  692,
  848,    0,    0,   81,  681,  112,    0,    0,  113,    0,
  114,  115,  116,  117,    0,  118,    0,    0,    0,    0,
    0,    0,  848,  119,  120,    0,    0,  848,    0,    0,
    0,    0,    0,    0,  121,  122,  850,  123,    0,  848,
  692,  848,    0,  692,  124,  125,    0,  126,  127,    0,
  128,    0,    0,  129,  130,  131,  132,  133,  692,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  137,  138,
    0,  139,  140,  141,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  848,  850,
    0,    0,  850,    0,    0,    0,  854,    0,    0,  850,
    0,  850,    0,    0,    0,  850,    0,    0,  848,    0,
  850,    0,  848,  850,    0,    0,    0,    0,    0,  993,
  753,  751,    0,  752,    0,  754,    0,    0,  850,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  949,  948,
  950,    0,  850,    0,    0,    0,    0,    0,    0,  854,
  850,    0,  854,  850,  850,    0,    0,    0,    0,  854,
    0,  854,    0,  850,    0,  854,  700,    0,    0,  467,
  854,  468,    0,  854,    0,    0,    0,    0,    0,  850,
    0,    0,    0,    0,  470,    0,    0,    0,  854,  316,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  850,  854,    0,    0,    0,  850,    0,    0,    0,
  854,    0,  679,  854,  854,  679,    0,    0,  850,    0,
  850,    0,  679,  854,  679,    0,    0,    0,  679,    0,
    0,    0,    0,  679,    0,    0,  679,    0,    0,  854,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  679,    0,    0,    0,    0,    0,    0,    0,    0,
  681,  854,    0,  681,    0,  679,  854,  850,    0,    0,
  681,    0,  681,  679,    0,    0,  681,  679,  854,    0,
  854,  681,    0,    0,  681,    0,  679,  850,    0,    0,
    0,  850,    0,    0,    0,    0,    0,    0,    0,  681,
    0,    0,  679,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  681,  692,    0,    0,  692,    0,    0,
    0,  681,    0,  470,  692,  681,  692,  854,    0,  679,
  692,    0,    0,    0,  681,  692,    0,    0,  692,    0,
    0,  679,    0,  679,    0,    0,    0,  854,    0,  936,
  681,  854,    0,  692,    0,  749,    0,    0,    0,    0,
  750,    0,    0,  937,    0,    0,    0,  692,    0,    0,
    0,    0,    0,    0,    0,  692,    0,  681,    0,  692,
    0,    0,    0,    0,    0,    0,    0,    0,  692,  681,
  679,  681,    0,  429,    0,    0,    0,    0,    0,  938,
  430,    0,    0,    0,  692,    0,    0,  939,    0,    0,
  679,    0,  431,    0,  679,  940,    0,    0,  432,  433,
    0,  700,    0,    0,  467,    0,  468,    0,    0,    0,
    0,  692,    0,  941,    0,  942,    0,  694,  681,  470,
    0,    0,    0,  692,  316,  692,  434,    0,  943,  435,
  944,  945,  946,    0,    0,    0,    0,    0,  681,    0,
    0,    0,  681,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  436,    0,  437,  438,    0,    0,
    0,    0,    0,    0,    0,    0,  439,    0,    0,    0,
  440,    0,  692,    0,    0,    0,    0,    0,  928,  947,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  692,    0,    0,    0,  692,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   14,    0,    0,    0,  696,
    0,    0,    0,    0,    0,  441,    0,    0,    0,    0,
    0,  442,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  443,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  444,  445,  446,  447,  238,
  239,  113,    0,  114,  115,  116,  117,    0,  118,  448,
    0,    0,    0,    0,    0,    0,  119,  120,  449,  450,
  451,  452,  453,  454,  455,  456,  457,  121,  458,    0,
  123,    0,    0,  459,  460,    0,  461,  462,  463,    0,
  126,  127,  464,  128,    0,    0,  129,  130,  131,  132,
  133,  929,  930,  931,  134,  135,    0,    0,    0,    0,
  136,  465,  138,    0,  466,  140,  141,    0,  429,    0,
    0,    0,    0,    0,    0,  430,    0,    0,    0,  700,
    0,    0,  467,    0,  468,    0,    0,  431,    0,    0,
  693,    0,    0,  432,  433,    0,    0,  470,    0,    0,
    0,    0,  316,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  694,    0,    0,    0,    0,    0,    0,    0,
    0,  434,    0,    0,  435,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  112,    0,    0,
  113,    0,  114,  115,  116,  117,    0,  118,    0,  436,
    0,  437,  438,    0,    0,  119,  120,    0,    0,    0,
    0,  439,    0,    0,    0,  440,  121,  122,    0,  123,
    0,    0,    0,  695,    0,    0,  124,  125,    0,  126,
  127,    0,  128,    0,    0,  129,  130,  131,  132,  133,
    0,    0,    0,  134,  135,    0,    0,    0,    0,  136,
  137,  138,    0,  139,  140,  141,    0,    0,    0,    0,
    0,    0,    0,    0,  696,    0,    0,    0,    0,    0,
  441,    0,    0,    0,    0,    0,  442,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  443,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  444,  445,  446,  447,  238,  239,  113,    0,  114,  115,
  116,  117,    0,  118,  448,    0,    0,    0,    0,    0,
    0,  119,  120,  449,  450,  451,  452,  453,  454,  455,
  456,  457,  121,  458,    0,  123,    0,    0,  459,  460,
    0,  461,  462,  463,    0,  126,  127,  464,  128,    0,
    0,  129,  130,  697,  698,  699,  429,    0,    0,  134,
  135,    0,    0,  430,    0,  136,  465,  138,    0,  466,
  140,  141,    0,    0,    0,  431,    0,    0,    0,    0,
    0,  432,  433,  700,    0,    0,  467,    0,  468,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  694,  470,    0,    0,    0,    0,  316,    0,    0,  434,
    0,    0,  435,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  436,    0,  437,
  438,    0,    0,    0,    0,    0,    0,    0,    0,  439,
    0,    0,    0,  440,    0,    0,    0,  753,  751,    0,
  752,  924,  754,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  949,  948,  950,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  696,    0,    0,    0,    0,    0,  441,    0,
    0,    0,    0,    0,  442,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  443,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  444,  445,
  446,  447,  238,  239,  113,    0,  114,  115,  116,  117,
    0,  118,  448,    0,    0,    0,    0,    0,    0,  119,
  120,  449,  450,  451,  452,  453,  454,  455,  456,  457,
  121,  458,    0,  123,    0,    0,  459,  460,    0,  461,
  462,  463,    0,  126,  127,  464,  128,    0,    0,  129,
  130,  697,  698,  699,    0,    0,    0,  134,  135,    0,
    0,    0,    0,  136,  465,  138,    0,  466,  140,  141,
  429,    0,    0,    0,    0,    0,    0,  430,    0,    0,
    0,  700,    0,    0,  467,    0,  468,    0,    0,  431,
    0,    0,    0,    0,    0,  432,  433,    0,    0,  470,
    0,    0,    0,    0,  316,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  694,    0,    0,    0,    0,    0,
    0,    0,    0,  434,    0,    0,  435,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  936,    0,    0,    0,
    0,  436,  749,  437,  438,    0,    0,  750,    0,    0,
  937,    0,    0,  439,    0,    0,    0,  440,    0,    0,
    0,    0,    0,    0,    0,  695,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  938,    0,    0,  602,
    0,    0,    0,    0,  939,    0,    0,    0,    0,    0,
    0,    0,  940,    0,    0,    0,  696,    0,    0,    0,
    0,    0,  441,    0,    0,    0,    0,    0,  442,    0,
  941,    0,  942,    0,    0,    0,    0,    0,  443,    0,
    0,    0,    0,    0,    0,  943,    0,  944,  945,  946,
    0,    0,  444,  445,  446,  447,  238,  239,  113,    0,
  114,  115,  116,  117,    0,  118,  448,    0,    0,    0,
    0,    0,    0,  119,  120,  449,  450,  451,  452,  453,
  454,  455,  456,  457,  121,  458,    0,  123,    0,    0,
  459,  460,    0,  461,  462,  463,  947,  126,  127,  464,
  128,    0,    0,  129,  130,  697,  698,  699,  429,    0,
    0,  134,  135,    0,    0,  430,    0,  136,  465,  138,
    0,  466,  140,  141,    0,    0,    0,  431,    0,    0,
    0,    0,    0,  432,  433,    0,    0,    0,    0,    0,
    0,    0,  535,    0,    0,  467,    0,  468,    0,    0,
    0,    0,  694,    0,    0,    0,    0,    0,    0,    0,
  470,  434,    0,    0,  435,  316,    0,  112,    0,    0,
  113,    0,  114,  115,  116,  117,    0,  118,    0,    0,
    0,    0,    0,    0,    0,  119,  120,    0,    0,  436,
    0,  437,  438,    0,    0,    0,  121,  122,    0,  123,
    0,  439,    0,    0,    0,  440,  124,  125,    0,  126,
  127,    0,  128,  924,    0,  129,  130,  131,  132,  133,
    0,    0,    0,  134,  135,    0,    0,    0,    0,  136,
  137,  138,    0,  139,  140,  141,  105,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  696,    0,    0,    0,    0,    0,
  441,    0,    0,    0,    0,    0,  442,    0,    0,    0,
    0,    0,    0,    0,  106,    0,  443,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  107,    0,    0,    0,
  444,  445,  446,  447,  238,  239,  113,    0,  114,  115,
  116,  117,    0,  118,  448,    0,    0,    0,    0,    0,
  108,  119,  120,  449,  450,  451,  452,  453,  454,  455,
  456,  457,  121,  458,    0,  123,    0,    0,  459,  460,
    0,  461,  462,  463,    0,  126,  127,  464,  128,    0,
    0,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,    0,    0,    0,    0,  136,  465,  138,    0,  466,
  140,  141, 1172,    0,    0, 1173,    0,    0,    0,  429,
    0,    0,    0,    0,    0,  109,  430,    0,    0,    0,
    0,    0,    0,    0,    0,  110,    0,    0,  431,    0,
    0,    0,    0,    0,  432,  433,    0,    0,    0,    0,
    0,    0,    0,  535,    0,    0,  467,    0,  468,    0,
  111,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  470,  434,    0,    0,  435,  316,    0,  112,    0,
    0,  113,    0,  114,  115,  116,  117,    0,  118,    0,
    0,    0,    0,    0,    0,    0,  119,  120,    0,    0,
  436,    0,  437,  438,    0,    0,    0,  121,  122,    0,
  123,    0,  439,    0,    0,    0,  440,  124,  125,    0,
  126,  127,    0,  128,    0,    0,  129,  130,  131,  132,
  133,    0,    0,    0,  134,  135,    0,    0,    0,    0,
  136,  137,  138,    0,  139,  140,  141,   48,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1174,
    0,  441,    0,    0,    0,    0,    0,  442,    0,    0,
    0,    0,    0,    0,    0,   48,    0,  443,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   48,    0,    0,
    0,  444,  445,  446,  447,  238,  239,  113,    0,  114,
  115,  116,  117,    0,  118,  448,    0,    0,    0,    0,
    0,   48,  119,  120,  449,  450,  451,  452,  453,  454,
  455,  456,  457,  121,  458,    0,  123,    0,    0,  459,
  460,    0,  461,  462,  463,    0,  126,  127,  464,  128,
    0,    0,  129,  130,  131,  132,  133,    0,    0,    0,
  134,  135,    0,    0,    0,    0,  136,  465,  138,    0,
  466,  140,  141, 1183,    0,    0, 1173,    0,    0,    0,
  429,    0,    0,    0,    0,    0,   48,  430,    0,    0,
    0,    0,    0,    0,    0,    0,   48,    0,    0,  431,
    0,    0,    0,    0,    0,  432,  433,    0,    0,    0,
    0,    0,    0,    0,  535,    0,    0,  467,    0,  468,
    0,   48,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  470,  434,    0,    0,  435,  316,    0,   50,
    0,    0,   50,    0,   50,   50,   50,   50,    0,   50,
    0,    0,    0,    0,    0,    0,    0,   50,   50,    0,
    0,  436,    0,  437,  438,    0,    0,    0,   50,   50,
    0,   50,    0,  439,    0,    0,    0,  440,   50,   50,
    0,   50,   50,    0,   50,    0,    0,   50,   50,   50,
   50,   50,    0,    0,    0,   50,   50,    0,    0,    0,
    0,   50,   50,   50,    0,   50,   50,   50,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1174,    0,  441,  608,    0,    0,    0,    0,  442,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  443,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  444,  445,  446,  447,  238,  239,  113,    0,
  114,  115,  116,  117,    0,  118,  448,    0,    0,    0,
    0,    0,    0,  119,  120,  449,  450,  451,  452,  453,
  454,  455,  456,  457,  121,  458,    0,  123,    0,    0,
  459,  460,    0,  461,  462,  463,    0,  126,  127,  464,
  128,    0,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  465,  138,
    0,  466,  140,  141, 1187,    0,    0, 1173,    0,    0,
    0,  429,    0,    0,    0,    0,    0,    0,  430,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  431,    0,    0,    0,    0,    0,  432,  433,    0,    0,
    0,    0,    0,    0,    0,  535,    0,    0,  467,    0,
  468,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  470,  434,    0,    0,  435,  316,    0,
    0,  112,    0,    0,  113,    0,  114,  115,  116,  117,
    0,  118,    0,    0,    0,    0,    0,    0,    0,  119,
  120,    0,  436,    0,  437,  438,    0,    0,    0,    0,
  121,  122,    0,  123,  439,    0,    0,    0,  440,    0,
  124,  125,    0,  126,  127,    0,  128,    0,    0,  129,
  130,  131,  132,  133,    0,    0,    0,  134,  135,    0,
    0,    0,    0,  136,  137,  138,    0,  139,  140,  141,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1174,    0,  441,    0,    0,    0,    0,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  443,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  444,  445,  446,  447,  238,  239,  113,
    0,  114,  115,  116,  117,    0,  118,  448,    0,    0,
    0,    0,    0,    0,  119,  120,  449,  450,  451,  452,
  453,  454,  455,  456,  457,  121,  458,    0,  123,    0,
    0,  459,  460,    0,  461,  462,  463,    0,  126,  127,
  464,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  465,
  138,    0,  466,  140,  141, 1195,    0,    0, 1173,    0,
    0,    0,  429,  217,    0,    0,  217,    0,    0,  430,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  217,
    0,  431,    0,  217,    0,    0,  217,  432,  433,    0,
    0,    0,    0,    0,    0,    0,  535,    0,    0,  467,
    0,  468,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  470,  434,    0,    0,  435,  316,
    0,    0,    0,    0,    0,    0,  217,    0,    0,    0,
    0,    0,  217,  217,  217,    0,  217,  217,  217,    0,
  217,    0,    0,  436,    0,  437,  438,    0,    0,    0,
    0,    0,    0,    0,    0,  439,    0,    0,  217,  440,
  217,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  217,
    0,    0,    0,    0,   80,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  217,    0,    0,    0,
    0,    0, 1174,    0,  441,    0,    0,    0,    0,    0,
  442,    0,    0,    0,    0,    0,    0,    0,  217,    0,
  443,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  444,  445,  446,  447,  238,  239,
  113,    0,  114,  115,  116,  117,    0,  118,  448,    0,
    0,    0,    0,  217,  217,  119,  120,  449,  450,  451,
  452,  453,  454,  455,  456,  457,  121,  458,    0,  123,
  217,    0,  459,  460,    0,  461,  462,  463,    0,  126,
  127,  464,  128,    0,    0,  129,  130,  131,  132,  133,
    0,    0,    0,  134,  135,    0,    0,    0,    0,  136,
  465,  138,    0,  466,  140,  141, 1198,    0,    0, 1173,
    0,    0,    0,  429,  217,    0,    0,  217,    0,    0,
  430,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  217,    0,  431,    0,  217,    0,    0,  217,  432,  433,
    0,    0,    0,    0,    0,    0,    0,  535,    0,    0,
  467,    0,  468,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  470,  434,    0,    0,  435,
  316,    0,    0,    0,    0,    0,    0,  217,    0,    0,
    0,    0,    0,  217,  217,  217,    0,  217,  217,  217,
    0,  217,    0,    0,  436,    0,  437,  438,    0,    0,
    0,    0,    0,    0,    0,    0,  439,    0,    0,  217,
  440,  217,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  217,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  217,    0,    0,
    0,    0,    0, 1174,    0,  441,    0,    0,    0,    0,
    0,  442,    0,    0,    0,    0,    0,    0,    0,  217,
    0,  443,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  444,  445,  446,  447,  238,
  239,  113,    0,  114,  115,  116,  117,    0,  118,  448,
    0,    0,    0,    0,  217,  217,  119,  120,  449,  450,
  451,  452,  453,  454,  455,  456,  457,  121,  458,    0,
  123,  217,    0,  459,  460,    0,  461,  462,  463,    0,
  126,  127,  464,  128,    0,    0,  129,  130,  131,  132,
  133,    0,    0,    0,  134,  135,    0,    0,    0,    0,
  136,  465,  138,    0,  466,  140,  141, 1203,    0,    0,
 1173,    0,    0,    0,  429,  634,    0,    0,  635,    0,
    0,  430,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  431,    0,  636,    0,    0,  637,  432,
  433,    0,    0,    0,    0,    0,    0,    0,  535,    0,
    0,  467,    0,  468,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  470,  434,    0,    0,
  435,  316,    0,    0,    0,    0,    0,    0,  638,    0,
    0,    0,    0,    0,  639,  640,  641,    0,  642,  643,
  644,    0,  645,    0,    0,  436,    0,  437,  438,    0,
    0,    0,    0,    0,    0,    0,    0,  439,    0,    0,
  646,  440,  647,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  648,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  649,    0,
    0,    0,    0,    0, 1174,    0,  441,    0,    0,    0,
    0,    0,  442,    0,    0,    0,    0,    0,    0,    0,
  650,    0,  443,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  444,  445,  446,  447,
  238,  239,  113,    0,  114,  115,  116,  117,    0,  118,
  448,    0,    0,    0,    0,  651,  652,  119,  120,  449,
  450,  451,  452,  453,  454,  455,  456,  457,  121,  458,
    0,  123,  653,    0,  459,  460,    0,  461,  462,  463,
    0,  126,  127,  464,  128,    0,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  465,  138,    0,  466,  140,  141, 1206,    0,
    0, 1173,    0,    0,    0,  429,  147,    0,    0,  147,
    0,    0,  430,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  431,    0,  147,    0,    0,  147,
  432,  433,    0,    0,    0,    0,    0,    0,    0,  535,
    0,    0,  467,    0,  468,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  470,  434,    0,
    0,  435,  316,    0,    0,    0,    0,    0,    0,  147,
    0,    0,    0,    0,    0,  147,  147,  147,    0,  147,
  147,  147,    0,  147,    0,    0,  436,    0,  437,  438,
    0,    0,    0,    0,    0,    0,    0,    0,  439,    0,
    0,  147,  440,  147,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  147,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  147,
    0,    0,    0,    0,    0, 1174,    0,  441,    0,    0,
    0,    0,    0,  442,    0,    0,    0,    0,    0,    0,
    0,  147,    0,  443,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  444,  445,  446,
  447,  238,  239,  113,    0,  114,  115,  116,  117, 1259,
  118,  448,    0,    0,    0,    0,  147,  147,  119,  120,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  121,
  458,    0,  123,  147,    0,  459,  460,    0,  461,  462,
  463,    0,  126,  127,  464,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,    0,  134,  135,    0,    0,
    0,    0,  136,  465,  138,    0,  466,  140,  141, 1209,
    0,    0, 1173,    0,    0,    0,  429,    0,    0, 1260,
    0,    0,    0,  430,    0,    0,    0,  652,    0,  652,
  652,    0,  652,    0,    0,  431,    0,    0,    0,    0,
    0,  432,  433,    0,    0,  652,    0,    0,    0,    0,
  652,    0,    0,    0, 1261,    0,    0,    0,    0, 1262,
    0,    0,    0,    0,    0, 1263,    0,    0,    0,  434,
    0,    0,  435,    0,    0,    0,  112,    0,    0,  113,
    0,  114,  115,  116, 1264,    0,  118,    0,    0,    0,
    0,    0,    0,    0,  119,  120,    0,  436,    0,  437,
  438,    0,    0,    0,    0,  121,  122,    0,  123,  439,
    0,    0,    0,  440,    0,  124,  125,    0,  126,  127,
    0,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  137,
  138,    0,  139,  140,  141,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1174,    0,  441,    0,
    0,    0,    0,    0,  442,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  443,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  444,  445,
  446,  447,  238,  239,  113,    0,  114,  115,  116,  117,
    0,  118,  448,    0,    0,    0,    0,    0,    0,  119,
  120,  449,  450,  451,  452,  453,  454,  455,  456,  457,
  121,  458,    0,  123,    0,    0,  459,  460,    0,  461,
  462,  463,    0,  126,  127,  464,  128,  652,    0,  129,
  130,  131,  132,  133,  652,    0,    0,  134,  135,    0,
    0,  652,    0,  136,  465,  138,    0,  466,  140,  141,
    0,    0,    0,  652,    0,    0,    0,    0,    0,  652,
  652,    0,    0,    0,    0,    0,    0,  652,  646,    0,
  646,  646,    0,  646,    0,    0,  516,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  646,  652,    0,    0,
  652,  646,    0,  112,    0,    0,  113,    0,  114,  115,
  116,  117,    0,  118,    0,    0,    0,    0,    0,    0,
    0,  119,  120,    0,    0,  652,    0,  652,  652,    0,
    0,    0,  121,  122,    0,  123,    0,  652,    0,    0,
    0,  652,  124,  125,    0,  126,  127,    0,  128,    0,
    0,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,    0,    0,    0,    0,  136,  137,  138,    0,  139,
  140,  141,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  652,    0,    0,    0,
    0,    0,  652,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  652,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  652,  652,  652,  652,
  652,  652,  652,    0,  652,  652,  652,  652,    0,  652,
  652,    0,    0,    0,    0,    0,    0,  652,  652,  652,
  652,  652,  652,  652,  652,  652,  652,  652,  652,  652,
    0,  652,    0,    0,  652,  652,    0,  652,  652,  652,
    0,  652,  652,  652,  652,    0,    0,  652,  652,  652,
  652,  652,    0,    0,    0,  652,  652,    0,    0,    0,
    0,  652,  652,  652,    0,  652,  652,  652,  646,    0,
    0,    0,    0,    0,    0,  646,    0,    0,    0,    0,
    0,    0,  646,    0,    0,    0,  535,    0,  540,  467,
    0,  468,    0,    0,  646,    0,    0,    0,    0,    0,
  646,  646,    0,    0,  470,    0,    0,    0,  646,  316,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  646, 1003,
    0,  646, 1003,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1005,    0, 1003,
    0,    0, 1003,    0,    0,    0,  646,    0,  646,  646,
    0,    0,    0,    0,    0,    0,    0,    0,  646,    0,
    0,    0,  646,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1003,    0,    0,    0,    0,    0, 1003, 1003,
 1003,    0, 1003, 1003, 1003,    0, 1003,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1003,    0, 1003,  646,    0,    0,
    0,    0,    0,  646,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  646,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1003,    0,  646,  646,  646,
  646,  646,  646,  646,    0,  646,  646,  646,  646,    0,
  646,  646, 1003,    0,    0,    0,    0,    0,  646,  646,
  646,  646,  646,  646,  646,  646,  646,  646,  646,  646,
  646,    0,  646,    0, 1003,  646,  646,    0,  646,  646,
  646,    0,  646,  646,  646,  646,    0,    0,  646,  646,
  646,  646,  646,  429,    0,    0,  646,  646,    0,    0,
  430,    0,  646,  646,  646,    0,  646,  646,  646, 1003,
 1003,    0,  431,    0,    0,    0,    0,    0,  432,  433,
  535,    0,    0,  467,    0,  468, 1003,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  470,    0,
    0,    0,    0,  316,    0,    0,  434,    0,    0,  435,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,    0,  436,    0,  437,  438,    0,    0,
    0,  121,  122,    0,  123,    0,  439,    0,    0,    0,
  440,  124,  125,    0,  126,  127,    0,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  441,    0,    0,    0,    0,
    0,  442,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  443,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  444,  445,  446,  447,  238,
  239,  113,    0,  114,  115,  116,  117,    0,  118,  448,
    0,    0,    0,    0,    0,    0,  119,  120,  449,  450,
  451,  452,  453,  454,  455,  456,  457,  121,  458,    0,
  123,    0,    0,  459,  460,    0,  461,  462,  463,    0,
  126,  127,  464,  128,    0,    0,  129,  130,  131,  132,
  133,    0,  953,    0,  134,  135,    0,    0,    0,    0,
  136,  465,  138,    0,  466,  140,  141,  429,    0,    0,
    0,    0,    0,    0,  430,  954,    0,    0,  535,    0,
    0,  467,    0,  468,    0,    0,  431,    0,    0,    0,
    0,    0,  432,  433,    0,    0,  470,    0,    0,    0,
    0,  316,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  434,    0,    0,  435,    0,    0,    0,    0,    0,    0,
    0,  955,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  436,    0,
  437,  438,    0,    0,    0,    0,    0,    0,    0,    0,
  439,    0,    0,    0,  440,  112,    0,    0,  113,    0,
  114,  115,  116,  117,    0,  118,    0,    0,    0,    0,
    0,    0,    0,  119,  120,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  121,  122,    0,  123,    0,    0,
    0,    0,    0,    0,  124,  125,    0,  126,  127,    0,
  128,    0,    0,  129,  130,  131,  132,  133,    0,  441,
    0,  134,  135,    0,    0,  442,    0,  136,  137,  138,
    0,  139,  140,  141,    0,  443,    0,    0,    0,    0,
    0,    0,    0,  732,    0,    0,    0,    0,    0,  444,
  445,  446,  447,  238,  239,  113,    0,  114,  115,  116,
  117,    0,  118,  448,    0,    0,    0,    0,    0,    0,
  119,  120,  449,  450,  451,  452,  453,  454,  455,  456,
  457,  121,  458,    0,  123,    0,    0,  459,  460,    0,
  461,  462,  463,    0,  126,  127,  464,  128,    0,    0,
  129,  130,  131,  132,  133,  429,    0,    0,  134,  135,
    0,    0,  430,    0,  136,  465,  138,    0,  466,  140,
  141,    0,    0,    0,  431,    0,    0,    0,    0,    0,
  432,  433,  535,  997,    0,  467,    0,  468,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  470,    0,    0,    0,    0,  316,    0,    0,  434,    0,
    0,  435,    0,    0,   27,    0,    0,   27,    0,   27,
   27,   27,   27,    0,   27,    0,    0,    0,    0,    0,
    0,    0,   27,   27,    0,    0,  436,    0,  437,  438,
    0,    0,    0,   27,   27,    0,   27,    0,  439,    0,
    0,    0,  440,   27,   27,    0,   27,   27,    0,   27,
    0,    0,   27,   27,   27,   27,   27,    0,    0,    0,
   27,   27,    0,    0,    0,    0,   27,   27,   27,    0,
   27,   27,   27,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   14,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  441,    0,    0,
    0,    0,    0,  442,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  443,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  444,  445,  446,
  447,  238,  239,  113,    0,  114,  115,  116,  117,    0,
  118,  448,    0,    0,    0,    0,    0,    0,  119,  120,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  121,
  458,    0,  123,    0,    0,  459,  460,    0,  461,  462,
  463,    0,  126,  127,  464,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,    0,  134,  135,    0,    0,
    0,    0,  136,  465,  138,    0,  466,  140,  141,  429,
    0,    0,    0,    0,    0,    0,  430,    0,    0,    0,
  535,    0,    0,  467,    0,  468,    0,    0,  431,    0,
    0,    0,    0,    0,  432,  433,    0,    0,  470,    0,
    0,    0,    0,  316,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  434,    0,    0,  435,    0,    0,  583,    0,
    0,  583,    0,  583,  583,  583,  583,    0,  583,    0,
    0,    0,    0,    0,    0,    0,  583,  583,    0,    0,
  436,    0,  437,  438,    0,    0,    0,  583,  583,    0,
  583,    0,  439,    0,    0,    0,  440,  583,  583,    0,
  583,  583,    0,  583,    0,    0,  583,  583,  583,  583,
  583,    0,    0,    0,  583,  583,    0,    0,    0,    0,
  583,  583,  583,    0,  583,  583,  583,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  441,    0,    0,    0,    0,    0,  442,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  443,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  444,  445,  446,  447,  238,  239,  113,    0,  114,
  115,  116,  117,    0,  118,  448,    0,    0,    0,    0,
    0,    0,  119,  120,  449,  450,  451,  452,  453,  454,
  455,  456,  457,  121,  458,    0,  123,    0,    0,  459,
  460,    0,  461,  462,  463,    0,  126,  127,  464,  128,
    0,    0,  129,  130,  131,  132,  133,  429,    0,    0,
  134,  135,    0,    0,  430,    0,  136,  465,  138,    0,
  466,  140,  141,    0,    0,    0,  431,    0,    0,    0,
    0,    0,  432,  433,  535,    0,    0,  467,    0,  468,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  470,    0,    0,    0,    0,  316,    0,    0,
  434,    0,    0,  435,    0,    0,  286,    0,    0,  286,
    0,  286,  286,  286,  286,    0,  286,    0,    0,    0,
    0,    0,    0,    0,  286,  286,    0,    0,  436,    0,
  437,  438,    0,    0,    0,  286,  286,    0,  286,    0,
  439,    0,    0,    0,  440,  286,  286,    0,  286,  286,
    0,  286,    0,    0,  286,  286,  286,  286,  286,    0,
    0,    0,  286,  286,    0,    0,    0,    0,  286,  286,
  286,    0,  286,  286,  286,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  441,
    0,    0,    0,    0,    0,  442,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  443,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1201,    0,    0,  444,
  445,  446,  447,  238,  239,  113,    0,  114,  115,  116,
  117,    0,  118,  448,    0,    0,    0,    0,    0,    0,
  119,  120,  449,  450,  451,  452,  453,  454,  455,  456,
  457,  121,  458,    0,  123,    0,    0,  459,  460,    0,
  461,  462,  463,    0,  126,  127,  464,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,    0,    0,    0,  136,  465,  138,    0,  466,  140,
  141,  429,    0,    0,    0,    0,    0,    0,  430,    0,
    0,    0,  995,    0,  995,  995,    0,  995,    0,    0,
  431,    0,    0,    0,    0,    0,  432,  433,    0,    0,
  995,    0,    0,    0,    0,  995,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  434,    0,    0,  435,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  436,    0,  437,  438,    0,    0,    0,    0,
    0,    0,    0,    0,  439,    0,    0,    0,  440,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  441,    0,    0,    0,    0,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  443,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1410,    0,    0,  444,  445,  446,  447,  238,  239,  113,
    0,  114,  115,  116,  117,    0,  118,  448,    0,    0,
    0,    0,    0,    0,  119,  120,  449,  450,  451,  452,
  453,  454,  455,  456,  457,  121,  458,    0,  123,    0,
    0,  459,  460,    0,  461,  462,  463,    0,  126,  127,
  464,  128,    0,    0,  129,  130,  131,  132,  133,  995,
    0,    0,  134,  135,    0,    0,  995,    0,  136,  465,
  138,    0,  466,  140,  141,    0,    0,    0,  995,    0,
    0,    0,    0,    0,  995,  995,  469,    0,    0,  467,
    0,  468,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  470,    0,    0,    0,    0,  316,
    0,    0,  995,    0,    0,  995,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  995,    0,  995,  995,    0,    0,    0,    0,    0,    0,
    0,    0,  995,    0,    0,    0,  995,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  995,    0,    0,    0,    0,    0,  995,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  995,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  995,  995,  995,  995,  995,  995,  995,    0,  995,
  995,  995,  995,    0,  995,  995,    0,    0,    0,    0,
    0,    0,  995,  995,  995,  995,  995,  995,  995,  995,
  995,  995,  995,  995,  995,    0,  995,    0,    0,  995,
  995,    0,  995,  995,  995,    0,  995,  995,  995,  995,
    0,    0,  995,  995,  995,  995,  995,    0,    0,    0,
  995,  995,    0,    0,    0,    0,  995,  995,  995,    0,
  995,  995,  995,  429,    0,    0,    0,    0,    0,    0,
  430,    0,    0,    0,  535,    0,    0,  467,    0,  468,
    0,    0,  431,    0,    0,    0,    0,    0,  432,  433,
    0,    0,  470,    0,    0,    0,    0,  316,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  434,    0,    0,  435,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  436,    0,  437,  438,    0,    0,
    0,    0,    0,    0,    0,    0,  439,    0,    0,    0,
  440,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  441,    0,    0,    0,    0,
    0,  442,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  443,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  444,  445,  446,  447,  238,
  239,  113,    0,  114,  115,  116,  117,    0,  118,  448,
    0,    0,    0,    0,    0,    0,  119,  120,  449,  450,
  451,  452,  453,  454,  455,  456,  457,  121,  458,    0,
  123,    0,    0,  459,  460,    0,  461,  462,  463,    0,
  126,  127,  464,  128,    0,    0,  129,  130,  131,  132,
  133,  429,    0,    0,  134,  135,    0,    0,  430,    0,
  136,  465,  138,    0,  466,  140,  141,    0,    0,    0,
  431,    0,    0,    0,    0,    0,  432,  433,  995,    0,
    0,  995,    0,  995,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  995,    0,    0,    0,
    0,  995,    0,    0,  434,    0,    0,  435,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  436,    0,  437,  438,    0,    0,    0,    0,
    0,    0,    0,    0,  439,    0,    0,    0,  440,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  441,    0,    0,    0,    0,    0,  442,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  443,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  444,  445,  446,  447,  238,  239,  113,
    0,  114,  115,  116,  117,    0,  118,  448,    0,    0,
    0,    0,    0,    0,  119,  120,  449,  450,  451,  452,
  453,  454,  455,  456,  457,  121,  458,    0,  123,    0,
    0,  459,  460,    0,  461,  462,  463,    0,  126,  127,
  464,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  465,
  138,  550,  466,  140,  141,  995,    0,    0,    0,    0,
    0,    0,  995,  551,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  995,    0,    0,    0,    0,    0,
  995,  995,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  550,  550,    0,    0,  550,    0,    0,    0,    0,
  567,    0,    0,  551,  551,    0,    0,  551,  995,    0,
  550,  995,  550,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  551,    0,  551,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  995,    0,  995,  995,
    0,  567,  550,    0,  567,    0,    0,    0,  995,    0,
    0,    0,  995,    0,  551,    0,    0,    0,    0,  567,
    0,  567,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  567,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  995,    0,    0,
    0,    0,    0,  995,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  995,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  995,  995,  995,
  995,  995,  995,  995,    0,  995,  995,  995,  995,    0,
  995,  995,    0,    0,  557,    0,    0,    0,  995,  995,
  995,  995,  995,  995,  995,  995,  995,  995,  995,  995,
  995,    0,  995,    0,    0,  995,  995,    0,  995,  995,
  995,    0,  995,  995,  995,  995,    0,    0,  995,  995,
  995,  995,  995,    0,    0,  557,  995,  995,  557,    0,
    0,    0,  995,  995,  995,    0,  995,  995,  995,  550,
    0,  542,    0,  557,    0,  557,    0,    0,    0,    0,
    0,  551,    0,    0,  550,    0,  550,  550,  550,    0,
    0,    0,  550,    0,    0,  550,  551,    0,  551,  551,
  551,    0,    0,    0,  551,  557,    0,  551,  550,    0,
    0,    0,  542,    0,  550,  542,    0,  550,  567,    0,
  551,    0,    0,    0,    0,    0,  551,    0,    0,  551,
  542,    0,  542,  567,    0,    0,  567,  567,    0,    0,
    0,  567,    0,    0,  567,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  567,    0,  550,
    0,    0,  542,  567,    0,    0,  567,    0,    0,    0,
    0,  551,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  550,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  551,    0,    0,    0,    0,
    0,    0,    0,    0,  550,    0,    0,    0,  567,    0,
  550,    0,    0,    0,  550,    0,  551,    0,    0,    0,
    0,  550,  551,    0,    0,    0,  551,    0,    0,    0,
    0,    0,  567,  551,    0,    0,    0,    0,    0,  550,
    0,    0,    0,    0,  550,    0,    0,    0,    0,    0,
    0,  551,    0,  567,    0,    0,  551,    0,    0,  567,
    0,    0,    0,  567,    0,    0,    0,    0,    0,  550,
  567,    0,  557,  550,    0,    0,    0,    0,    0,    0,
    0,  551,    0,    0,    0,  551,    0,  557,  567,    0,
  557,  557,    0,  567,    0,  557,    0,    0,  557,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  557,    0,    0,    0,    0,    0,  557,  567,    0,
  557,    0,  567,    0,    0,    0,    0,    0,    0,  542,
  326,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  542,    0,    0,  542,  542,    0,
    0,    0,  542,    0,    0,  542,    0,    0,  326,    0,
    0,    0,  557,    0,    0,  326,    0,    0,  542,  326,
  326,    0,  326,    0,  542,  326,    0,  542,    0,    0,
    0,    0,    0,    0,    0,    0,  557,    0,  326,    0,
    0,    0,    0,    0,  326,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  557,    0,    0,
    0,    0,    0,  557,    0,    0,    0,  557,    0,  542,
    0,    0,    0,    0,  557,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1256,    0,
    0,    0,  557,  542,    0,    0,    0,  557,    0,    0,
    0,  326,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  542,    0,    6,    0,    0,  326,
  542,    0,  557,    0,  542,    0,  557, 1462, 1463,    0,
 1464,  542,    0,    0,  326,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  326,    0, 1465,    0,    0,  542,
    0,    0, 1466,  326,  542,  326,    0,    0,    0,    0,
    0,    0,  326,    0,    0,  326,    0,  326,  326,  326,
  326,    0,  326,    0,    0,    0,    0,    0,    0,  542,
  326,  326,    0,  542,    0,    0,    0,    0,    0,    0,
    0,  326,  326,    0,  326,    0,    0,    0,    0,    0,
    0,  326,  326,    0,  326,  326,    0,  326,    0, 1467,
  326,  326,  326,  326,  326,    0,    0,    0,  326,  326,
  326,  326,  326, 1256,  326,  326,  326,   14,  326,  326,
  326,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1468,    0,    0,    0,    0,    0,    0,    0,
    0,    6,   16,    0,    0,    0,    0,    0,    0,    0,
    0, 1670, 1462, 1463,    0, 1464,    0,    0,    0,    0,
  447,    0,    0,  113,    0,  114,  115,  116,  117,    0,
  118, 1465,    0,    0,    0,    0,    0, 1466,  119,  120,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1469,
  122,    0,  123,    0,    0,    0,    0,    0,    0,  124,
  125,    0,  126,  127,    0,  128,    0,    0,  129, 1470,
  131,  132,  133,    0,    0,    0,  134,  135, 1471, 1472,
 1473,    0,  136,  137,  138,    0,  139,  140,  141,    0,
    0,    0,    0,    0, 1467,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   14,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1468,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  447,    0,    0,  113,    0,
  114,  115,  116,  117,    0,  118,  634,    0,    0,  635,
    0,    0,    0,  119,  120,  838,    0,    0,    0,    0,
    0,    0,    0,    0, 1469,  122,  636,  123,    0,  637,
    0,    0,    0,    0,  124,  125,    0,  126,  127,    0,
  128,    0,    0,  129, 1470,  131,  132,  133,    0,    0,
    0,  134,  135, 1471, 1472, 1473,    0,  136,  137,  138,
    0,  139,  140,  141,    0,    0,    0,    0,    0,  638,
    0,    0,    0,    0,    0,  639,  640,  641,    0,  642,
  643,  644,    0,  645,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  646,    0,  647,  634,    0,    0,  635,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  636,    0,    0,  637,    0,    0,
    0,    0,  648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  649,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  638,    0,    0,
    0,  650,    0,  639,  640,  641,    0,  642,  643,  644,
    0,  645,    0,    0,    0,    0,  112,    0,    0,  113,
    0,  114,  115,  116,  117,    0,  118,    0,    0,  646,
    0,  647,    0,    0,  119,  120,  651,  652,    0,    0,
    0,    0,    0,    0,    0,  121,  122,    0,  123,    0,
    0,    0,    0,  653,    0,  124,  125,    0,  126,  127,
  648,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,  649,  136,  137,
  138,    0,  139,  140,  141,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  650,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  112,    0,    0,  113,    0,  114,
  115,  116,  117,    0,  118,    0,    0,    0,    0,    0,
    0,    0,  119,  120,  651,  652,    0,    0,    0,    0,
    0,    0,    0,  121,  122,    0,  123,    0,    0,    0,
    0,  653,    0,  124,  125,    0,  126,  127,    0,  128,
    0,    0,  129,  130,  131,  132,  133,    0,    0,    0,
  134,  135,    0,    0,    0,    0,  136,  137,  138,    0,
  139,  140,  141,  112,    0,    0,  113,  186,  114,  115,
  116,  117,  187,  118,  188,  189,  190,  191,  192,  193,
  194,  119,  120,  195,  196,  197,  198,  199,    0,    0,
  200,  201,  121,  122,  202,  123,    0,    0,  203,  204,
  205,  206,  124,  125,  207,  126,  127,  208,  128,  209,
  210,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,  211,  212,  213,  214,  136,  137,  138,  215,  139,
  140,  141,  467,    0,    0,  467,  467,  467,  467,  467,
  467,    0,  467,  467,  467,  467,  467,  467,  467,  467,
  467,  467,  467,  467,  467,  467,  467,    0,    0,  467,
  467,  467,  467,  467,  467,    0,    0,  467,  467,  467,
  467,  467,  467,  467,  467,  467,  467,  467,  467,  467,
  467,  467,  467,  467,  467,    0,    0,    0,  467,  467,
  467,  467,  467,  467,  467,  467,  467,  467,  467,  467,
  467,
};
static short yycheck[] = {                                       9,
    0,    4,   68,   60,  169,   16,   16,  111,   18,   58,
  159,  160,   66,  571,  159,  160,   73,  406,  406,   74,
   67,   99,   67,  402,   90, 1007,  374,   96,  603,  505,
  376,   77,   72,  756,  574,  302,  812,  398, 1021, 1082,
  756,  318,   62,   98,  718,   94,   56,   57,   58,   59,
   60,   61,   62, 1328,  100,  546,   96,   67,   68,   69,
 1256,   71, 1274,   73,   74,  958,   76,   77,   78,  525,
  744,    0,  759,   93,    0,  524,   86,   88,   88,   89,
   90,  101,   92,   93,   94,   95,   97,   97,   98,   99,
  100,  101,   59,  103,  469,  152,  174,    0, 1123,  688,
  469, 1255,  995,  347,   71,  171,  151, 1145,  153,   44,
 1041,  383,  681,  732,    0,  766,  173,  382,   41,  738,
  596,    0,  598,  810,  253,   44,  911,  603,   95,  620,
   59,   45,   44,   59,   45,   45,  103, 1089,   41, 1082,
  412,  151,  152,  153,  164,   40,  178,  157,    0,    0,
   40,    0,    0,   61,  164,   44,   59,    0,  262,  169,
  563,  171,  172,  173,  174,  219,  535,   44,  178,    0,
    0,    0,    0,   59,   44,    0,    0, 1082,    0,    0,
   59,   44,  347,    0, 1087, 1082,    0, 1082, 1140,   41,
   41, 1094,   44, 1082,   41,  249,   44,   41,   41, 1320,
 1321,   44, 1105,  582, 1325,  172,   45,   59,   59,   41,
   59,   59,   41, 1326,  603,  603,   59,   41,   63,   41,
   44,  350,   44,   44, 1233, 1128,    0, 1236,   59,   59,
   59,   59,   44,   40,   59,   59,   44,   59,   59,  273,
    0,    0,   59,  316,  447,   59,   40,   44,  293,  516,
  354,  296,  631,   91,    0,  263,   63,   40,  606,  273,
 1593,  395,   45,    0,  262,  532, 1026,   41,    0,   63,
   41,   41,   44,  395,  380,  317,  333, 1320, 1321,    0,
  329,   41, 1325,  293, 1007,   59,  296, 1483,  817,    0,
   45, 1007,  302,  303,  566,  349,  344,  268, 1536,   59,
   59,  309,  347,  832,   41,    0,  835,  655,   63,  298,
  264,    0,  320,   59,  348,  844,  297, 1471, 1472,  329,
  393,  298,   59,  333,  277,  315,  432,   59, 1275,  282,
  375,  700,  951,  952, 1359,  351,    0,  382,   59,  384,
 1323,  386,  408,  315,  389,  399,    0,  401,   59, 1301,
  371,  324,   41,  695,  284,  410, 1287,  334,  262,  524,
  433,  418,  416,  344,   59,  375,  500,  297,    0,  988,
   59,  420,  382,  748,  384,  904,  386, 1320, 1321,  389,
  755,  379, 1325, 1366,  317,  861,  428,   41,  359,  297,
  793, 1065,  514,    0,  264,   59,  887,   41,  408,  409,
  410, 1286, 1349,  315, 1483,   59,    0, 1300,  418,   41,
  420,  419,  315,    0,  424, 1320, 1321,    0,  262,  440,
 1325,  424,  272, 1320, 1321, 1320, 1321,   59, 1325,    0,
 1325, 1320, 1321,  336,   41, 1444, 1325,  275,   61, 1448,
 1343,    0,  409, 1718,  458,  434, 1559,   41,  297,  403,
   41,  380,   59,   44,  317,  445,  912,  514, 1696,  306,
  470,  509, 1081,  315,  315,   59,  523,  923,  484,  297,
  527,  927,   59,  445,  306,  379,   59,  622,  826,  524,
 1692,  829,   42,  456,  336,  336,  315, 1820,   59,  301,
  262,  315,  840,  301,  380,  344,  325,  387,  327,  376,
   59,  380,  534,  389,  514,  515,  516,  336,  775, 1663,
  389,  788,  336,  523,    0,  450,  344,  527,  264,  867,
 1280, 1281,  532,  427,  534,  444,  571, 1250,  509,  380,
  584,  410,  380,  376, 1250,  379,  450, 1812, 1067,  450,
  450,  389,  671,  446,  439,    0, 1165, 1166,  515, 1168,
 1169,  380, 1590,  264,  623, 1084, 1085,  446,  603, 1542,
  389,  571,  336, 1152, 1251,  444,  477,  811,  833,  915,
 1139,  625,  402,   59,    0, 1104,  336, 1602,  315, 1854,
 1365,  432, 1031,  623,  513, 1041,  928,  513,  402, 1147,
  513,  756,  602,  603,  446,  446,  444,  428,  608,  336,
 1875, 1252,  361,  432,   59,  450, 1537,  379,  379,  379,
  513,  450,  344, 1069,  763,  444,  670,  446,  262,    0,
 1572,  446,  446,  677,  678,  446,  315,  513,    0,  446,
  902,  428,    0,   59,  513,  680, 1645, 1589,  477,  847,
  423, 1526,  494,  450,  390, 1841,  811,  336, 1551,  344,
  493,  796, 1555, 1556, 1775, 1776,  450,    0, 1033,    0,
  509,  513,  513,   41,  265,  513,    0,  450,  423, 1865,
  680,  493,  446,   41,  297,  498,   44,  264,   59,  390,
  401,  509,  336,  315,  513, 1667,  446,   59, 1591,  513,
 1309,   59,  280,  276,  277,  450, 1892,  361,  299,  258,
  260,    0,    0,  723,  336,  276,  277, 1903,  315,  446,
 1906,  270, 1331,  723, 1075, 1608,   59, 1913,   59,  764,
    0,  766, 1396,  768,  869,   59,  783,  283,  262,  336,
  352,   41, 1775, 1776,   44,  379,  304,  305,  297,  749,
  300,  322,  336, 1091,  303, 1093,  305, 1730,  805,  759,
  807, 1905,    0, 1736,  764, 1103,  766,  446,  768,  305,
   59,   59, 1841,  280, 1540,  775,  811,  264,  822,  315,
   42,  816,  334,  783,  784,   47,  513,  298,  264,   59,
 1141,  791,  792,  262,  339,  344, 1865,  315,  833, 1329,
  859,   41,  446,   41,   44,  805,   59,  807,   41,  421,
  810, 1704, 1177,  390, 1150, 1708,  816,  362, 1177,  264,
    0,   59,  403, 1892,  446,  860,  861,  784,   41,  859,
 1723,  875,  275,  833, 1903,  401,  866, 1906,  897,  898,
  899,  397, 1775, 1776, 1913, 1364,    0, 1740,  264,  446,
   41,  422, 1007,  431,  884,  379,  903,  310,  429,  298,
  860,  861, 1834,  257, 1836, 1430,  913,  897,  898,  899,
  377, 1352, 1844,  418, 1355,  257, 1031,  877, 1243,   59,
 1775, 1776,  434, 1219,  877,  443,  371,   41, 1775, 1776,
 1775, 1776,  448,  264,  262,  334, 1775, 1776,  341, 1097,
 1098,   41,  264,  903,   44,   59,  261,  453,  454,  445,
  379,    0,    0,  913,  390,  397,  403, 1889,    0,  272,
 1285,  921,  922,  434,  431,  410, 1819,  695,  921, 1822,
 1539,  264,  700, 1846,   42,   43,  330,   45,    0,   47,
  450,  448,    0,  319,  309,  390,  390,  297,  330, 1147,
  271,    0,   41,  305, 1667,   44,  409,  315,  427,   41,
  509, 1667,   44,  315,  285, 1858,  448,  325,    0,  327,
   59,   59,  457,  273,  390,  291,  264,   59,  336,   41,
 1893,  339,  992,  336,  428,    0,  302,  276,  277,  259,
 1456, 1126,  992,  309,  264,  434, 1031,   59,  348,  269,
 1369,   59, 1259, 1341,   93, 1262,  306,  338,  329,   41,
   59,  379,   44,  450,  338,  277,  273,  333,  462,    0,
  282,  396,  380,    0,  281, 1025, 1026,   59,  390,  262,
  395,  389,  276, 1371, 1393, 1394,  357,  412,  304,  305,
  371, 1041, 1401, 1402,   59, 1404, 1405, 1656,  348,  262,
    0, 1430, 1430, 1412, 1413, 1414, 1415,  390, 1417, 1418,
 1419, 1420, 1421, 1422,   41,  430,  397,   44,   44,  400,
    0,  262,  316,  397,  432, 1134,  400,  315,   59,  395,
  304,  305,   59,  372,  264,  416,  444,  325,  446,  327,
 1149,   41,  416,  445,   44, 1250, 1155,  409,  336,  265,
  421, 1256,  390,  403, 1134,    0,  313,  314, 1143,   59,
  426,  442, 1147,  319,  430, 1115,   93,   93,  442, 1149,
  390, 1834,   40, 1836,  436, 1155,    0, 1120, 1834,   59,
 1836, 1844, 1132,  299, 1273, 1274, 1275,    0, 1844,  260,
    0,   44,  380, 1143,  394,    0,  379, 1147,  457,  393,
  459,  389,    0,    0,    0,  513,  924,  300,  458,   40,
  928,  315,  258,    0,   59,    0,  379,  333,    0,  277,
 1305,  325, 1307,  327,  282,    0, 1889,  443,   41,  300,
  262,  344,  336, 1889,   44,   59, 1795,    0,  379,  433,
   93,    0, 1801,   41,  432, 1263,   59,   42,   43,   59,
   45,  376,   47,  519,   59,  348,   41,  303,  446,   44,
  390,   59,   59,   59, 1215, 1215, 1261, 1252, 1255,  443,
 1255, 1256,   59, 1561,   59,    0,  380,   59, 1264,  372,
   44,  376, 1367,  315,   59,  389,  318,    0,   41,   42,
   43,    0,   45,  325,   47,  327,   59,    0, 1547,  331,
   59, 1251, 1252,  315,  336, 1255, 1256,  339, 1596, 1259,
 1260, 1261, 1262, 1263, 1264,  327,   41,    0,   42,   43,
  309,   45,  354,   47,  336,  513,   44,  258,  432,   93,
 1280, 1281,   41,  315,   59,   44,  368, 1287,    0,  270,
  453,  454,  446, 1592,  376,  327,   59,  379,  380,  259,
   59, 1849,    0, 1260,  336,  300,   59,  389,  298,  269,
    0,  260,  262,  402,  410,  260,  297,   44,  380,    0,
  428,  378,  303,  405,  305,   93,   59,  389, 1483,    0,
   41,  372,  428,   44,  378,    0, 1380,   42,   43,    0,
   45, 1385,   47,   41,  334,  427,   44,   59,  380,    0,
  432,  300,  393,  348, 1347,  300,  395,  389,  399,  513,
  333,   59,  444,  344,  446,  315,  462,  318,  318,   59,
  432,  352,  485,  292, 1529,  325,   41,  327,   59,   44,
  331,  331,  495,  484,  446,  428,  336,  426,   59,  339,
   41,    0,  433,   44,   59,  368,  315,    0,   59, 1737,
  432,  486,  441,  354,  354,  324, 1462,  380,   59,  399,
   61,  493,  258,  333,  446,  516,    0,  368,  368,  409,
  450,  258, 1423, 1423,  270,  402,  376,  352,    0,  379,
  380,  513,  277,  270,    0,  517,    0,  282,  260,  389,
  421,  360,  297,  338,  434,  364,   41,  477, 1483,   44,
   59,  513,  315,  410,  405,  405,   59,  303,  260,  305,
  297,  386, 1462,  388,  327,    0,  303,  315,  305,  434,
 1470, 1471, 1472,  336,  277,   59,  371,  427,  300,  282,
  315,  513,  432, 1483,  450,  410,  273,   59,  336,  344,
  325,  336,  327,   59,  444,   59,  446,  352,  300,    0,
  446,  336,  397,  277,  339,  400,   41,  344,  282,   44,
  435,  477, 1667, 1513, 1514, 1562,    0,  380,   40,  409,
  397,  416,   42,   43,   59,   45,  389,   47,  509, 1529,
  513,  386,  380,  388,  308,  333, 1536, 1537, 1693,  416,
  315,  389,  461,  493,  416,  380,  436,  442,  467,  468,
  469,    0,  318, 1692,  389,  410,  520,   41,   59,  258,
   44,  336, 1562,  513,  259,  331,  517,  517,  260,  432,
  368,  270,  277,  258,  269,   59,  261,  282,  276,  277,
  435,  264,  380,  446,  432,  319,  284,   41,  354,    0,
   44,   41,   41,  327,   44,   44, 1635,  432,  446,  453,
  449,  450,  451,    0,  303,  380,  305,  258,  300,  444,
   59,  446,  317,  361,  389,  363,  535,  719,  303,  721,
  539,  315,  273,  725,  726,  727,  338, 1721,  477,  478,
   41, 1632, 1632,   44, 1290, 1635, 1292,   42,   43,  405,
   45, 1860,   47, 1862,   41,  313,  297,   44,   59,  333,
  513,  452,  303,   41,  509,  306,   44,  432,  332,  371,
  444,   46,   59, 1663,  403,  513,   41,  338,  426,   44,
 1717,  446,  509,   58,  372,  395,   61,  318,  513, 1834,
  336, 1836,   40, 1722,   41,  397, 1841,   44,  400, 1844,
  331,  485,  486, 1693, 1684,  393, 1696,  348,  339, 1743,
  371,  399,   41, 1747,  416,   44,   42,   43,  315,   45,
 1865,   47, 1712,  354,  376,   41,  317, 1717,   44, 1813,
   42,   43, 1722,   45,  315,   47,  397,  368,  376,  400,
  442, 1787,   40,  498, 1889,  433,   41, 1892,  513,   44,
   41,  276,  277,   44,   41,  416,  428,   44, 1903,  284,
   58, 1906,  403,   46,  384,  385,  356,  277, 1913,  410,
   41,  377,  282,   44,  405,   58,   59,   46,   61, 1769,
  442,  442,   41,   42,   43,  497,   45,  428,   47,   58,
   59,  700,   61,   41,  453,  454,   44, 1787, 1788,  453,
  454,   41,  276,  277,   44,  493, 1789, 1513, 1514,   41,
  284,  720,   44, 1513, 1514,  724, 1841,  458,  426,  509,
   41,  462,  731,   44, 1849,  375,  336,  736,  737,   42,
   43,  409,   45,   41,   47,  284,   44,  276,  277,  748,
 1865,  750,  751,  752,  753,  754,  755,  372,  508,  758,
  461, 1841,   41, 1833,   44,   44, 1846,  446,  508, 1849,
   61,  262,  366,  283,   40,  264,   40, 1892,  393,  389,
 1860,   40, 1862,   40,  399, 1865,   40,  453, 1903,   40,
   40, 1906,  277,   40,   40,  794,  517,  282, 1913,  276,
  277,   40,   40,  453,  453,   40,  338,  284,  372,   40,
   40,   40, 1892, 1893,    0,  814,  351,   40,  433,   91,
  402,  820,  821, 1903,  315, 1905, 1906,  318,   44,  393,
  315,   46,   40, 1913,  325,  399,  327,   61,  264,  371,
  331,   42,   43,  372,   45,  336,   47,  395,  339,    0,
  317,   44,   44,  420,   61,   41,   46,   40,   44,    0,
  264,  277,  264,  354,  393,  397,  282,   44,  400,  433,
  399,  344,  344,   59,  344,  277,  276,  368,  493,  276,
  282,  264,  297,  390,  416,  376,  428,  462,  379,  380,
   41,  390,  308,   44,  450,  372,  275,  344,  389,  376,
   41,  439,  439,   44,  433,   41,   42,   43,   59,   45,
  442,   47,  392,   40,  405,  345,  393,   40,   59,   91,
   61,   40,  399,   40,  315,  264,  376,  403,  277,  493,
   44,  382,  375,  282,   40,   40,  427,  936,  937,  938,
  380,  432,  941,  942,  943,  443,  945,  946,  947,  948,
  949,  950,  315,  444,   41,  446,  433,  275,  314,   63,
  959,  960,  265,  962,  296,  964,  428,  428,  967,  968,
  969,  970,  971,  972,  277,   42,   43,   44,   45,  282,
   47,  456,   40,   44,  444,  984,    0,  986,    0,  264,
   44,  440,  494,   40,   40,    0,  299,  273,  297,   41,
   42,   43,  493,   45,  941,   47,  943,  281,  945,  946,
  379,  948,  949,  950,  340,  340,  493,  379,  276,  379,
  410,  408,  513,   40,  410,  352,  517,   41,  362,   41,
   44,   40,   44,   40, 1033,   40,   41,   42,   43,   44,
   45,   46,   47,   40,   44,   59,   41,   59,  317,  277,
   44,  270,  361,   58,   59,   60,   61,   62,   46,  378,
  438,  317,  317,  264,  452,   40,   44,  455,  266,  457,
  458,  459,  460,  325,  462,   40,  277,   40,  414,  493,
  264,  282,  470,  471,   41,   58,   91,  260,   93,   41,
  276,  277,   41,  481,  482,   41,  484,  264,  284,  446,
  387,  306,  306,  491,  492,   41,  494,  495,   41,  497,
   40,  262,  500,  501,  502,  503,  504,  258,   40,   40,
  508,  509,  317,   40,   40,  298,  514,  515,  516,  318,
  518,  519,  520,   40,  275,  276,  277,  310,   40,   40,
  281,  277,  331,  284,   40,   40,  282,   41,   42,   43,
   40,   45,  262,   47,   42,   43,  297,   45,   40,   47,
   40,  334,  303,   40,  315,  354,   40,  318,   40,   40,
  305,   41,   42,   43,  325,   45,  327,   47, 1177,  368,
  331,  446,  427,   44,   44,  336,  372,  376,  339,   41,
   41, 1190, 1191,  264, 1193, 1194,  381,  339,  273,  339,
  284,  327, 1201,  354,  384,  399,   41,  393,  379,  379,
  277,  379,   41,  399,  379,  282,  405,  368,  379,  379,
  372,  276,   40,  450, 1223,  376,  399,   41,  379,  380,
   41,  372, 1231, 1232, 1233,  277,  409, 1236,  389, 1238,
  282,   40,   44, 1242, 1243, 1244,   41,  433,  317,   41,
   41,  427,  393,  305,  405,   59,  290,  262,  399,  264,
  265,  434,  276,  277,  276,  277,  271,  408,   40,  410,
  284,   59,  277,  336,   59,   59,  427,  282,   59,   59,
  285,  432,   59,  297,  305,  336, 1285,  428,   59,   59,
   59,   59,  433,  444,  299,  446,   59,   59,   59,  304,
  305,   59,   59,  308,  493,   41,   42,   43,   59,   45,
  315,   47,  317,  318,   58,  428,  321,  339,  324,  277,
  325,  462,  327,  333,  329, 1324,  331,  434,  517,  298,
  334,  336,  337,  434,  339,   41,   42,   43,  334,   45,
   41,   47,  493,   42,   43,   44,   45,  298,   47,  354,
  355,   41,  357,  378,  365,  348,   41,  365,  372,   41,
  372,   41,  513,  368,   41,  370,  517,  372,  373,  374,
   41,  376,   41,   41,  379,  380,   41,   41,   41,  393,
   41,  393,   41,  277,  389,  399,   41,  399,  282,  277,
   41,   41,    0, 1392,  282,   41,   40,  402,   41, 1398,
  405,  315,  443,   40, 1403,   44,  378,  277,   41,  348,
  458, 1410,  282,  340,  333,   40,  421,  500,  336,  433,
   40,  433,  427,  428,  336,   40,  376,  432,   44,   41,
   40,  376,  376,   41,   41,  427,   44,   41,  443,  444,
  298,  446,  301,  434,   41, 1444,  290,  452,  304, 1448,
  455,   59,  457,  458,  459,  460,  301,  462,   41,   42,
   43,    0,   45,  376,   47,  470,  471,  333, 1467,  455,
  376,  409,   40,  276,  264,    0,  481,  482,  264,  484,
  450,  317,  281,  462,  443,   44,  491,  492,  493,  494,
  495,  270,  497,  403,   40,  500,  501,  502,  503,  504,
   41,  427,  450,  508,  509,  270,  339,  339,  513,  514,
  515,  516,  517,  518,  519,  520,   41,   42,   43,   44,
   45,   46,   47,  264,  428,   41,   42,   43,   41,   45,
  450,   47,   41,   59,   59,   60,   61,   62,  409, 1538,
   41,  277,   42,   43,   59,   45,  282,   47,   41,   42,
   43,   59,   45,   59,   47,  305,   41,   59,   41,   42,
   43,   59,   45,   59,   47,   41,   91,   59,   93,   59,
  301,  277,    0,    0,    0,    0,  282,  156,  277, 1271,
  156,  452,  784,  282,  455,  157,  457,  458,  459,  460,
  530,  462,   41,   42,   43,  532,   45,  304,   47,  470,
  471,  464,  465,  466,  467,  468,  469,  470,  471,  303,
  481,  482, 1611,  484,  684,  916,  775, 1150, 1617, 1618,
  491,  492,   92,  494,  495, 1152,  497,  921,  877,  500,
  501,  502,  503,  504,   78, 1147, 1142,  508,  509, 1580,
 1579,  631, 1641,  514,  515,  516, 1645,  518,  519,  520,
 1649,  891,  372,  452, 1653, 1654,  455,  403,  457,  458,
  459,  460,  816,  462,  352, 1078, 1132,  384,  276,  277,
  384,  470,  471,  281,   42,   43,  284,   45,  842,   47,
 1143, 1852,  481,  482, 1712,  484, 1559, 1877, 1855,  297,
 1561, 1022,  491,  492,  277,  494,  495,  608,  497,  282,
 1856,  500,  501,  502,  503,  504, 1672, 1842,  833,  508,
  509, 1470, 1303, 1543,  764,  514,  515,  516,  610,  518,
  519,  520,   41,   42,   43,  377,   45,  389,   47,  370,
 1356,  894,   13,  668,  634,  337,  886,  262,  548,  264,
  265,  805,  162, 1742,  552,  794,  271, 1291, 1041,   88,
  717, 1806,  277, 1632, 1423,  820,  563,  282,  782, 1192,
  285,  277,  262, 1034,  372, 1311,  282,   41,   42,   43,
   -1,   45,   -1,   47,  299,    0,  301,  277,   -1,  304,
  305, 1238,  282,  308,  277,  393,   -1,   -1,   -1,  282,
  315,  399,  317,  318,  277,   -1,  321,   -1,   -1,  282,
  325,   -1,  327,   -1,  329,   -1,  331, 1806,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,  433,   -1,   -1,  277,  354,
  355,   -1,  357,  282,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368, 1843,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,
   -1,   -1,   41,   42,   43,   -1,   45,  402,   47,   -1,
  405,    0,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,
   -1,   45,   -1,   47,  262,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,  443,  444,
   -1,  446,   41,   -1,   -1,   44,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   59,   -1,   -1,   -1,   -1,  470,  471,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,  481,  482,  277,  484,
   -1,   -1,   -1,  282,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   42,   43,   44,   45,
   -1,   47,   -1,  277, 1393, 1394,   -1,   -1,  282,   -1,
   -1,   -1, 1401, 1402,   -1, 1404, 1405,   -1,   -1,   -1,
   -1,   -1,   -1, 1412, 1413, 1414, 1415,   -1, 1417, 1418,
 1419, 1420, 1421, 1422,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,  302,   -1,   -1,
   -1,   -1,  277,   -1,  309,   -1,   -1,  282,  313,  314,
  285,   -1,   -1,   -1,   -1,   -1,    0,  322,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,  333,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   41,  277,   -1,
   44,  336,  337,  282,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,  277,   -1,   59,   -1,   61,  282,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
  395,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,  411,  276,  277,   -1,
   -1,  291,  281,   -1,  389,  284,   91,   -1,   93,   -1,
   -1,  426,  302,   -1,   -1,  430,   -1,  402,  297,  309,
  405,   -1,   -1,  277,   -1,   -1,  441,   -1,  282,   -1,
   -1,   -1,  322,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,  460,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,  277,  457,  458,  459,  460,  282,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  379,
   -1,   -1,   -1,  372,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,  519,  395,  491,  492,  493,  494,
  495,   -1,  497,   -1,  393,  500,  501,  502,  503,  504,
  399,  411,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,  426,   -1,   -1,   -1,
  430,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  441,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,  519,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,
   44,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,  481,  482,   44,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   59,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,
   44,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,  277,   -1,   -1,  428,   -1,  421,   -1,  284,  433,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,  297,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,  372,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,
   -1,   -1,   -1,  399,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,  276,  277,   -1,   -1,  271,  433,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,  433,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   40,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   42,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  262,   -1,  264,
  265,  491,  492,   -1,  494,  495,  271,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,  282,  508,  509,
  285,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
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
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  262,   -1,  264,
  265,  491,  492,   -1,  494,  495,  271,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  285,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
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
   -1,   -1,   -1,   45,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  261,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,  279,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,  294,
  285,  296,   -1,  298,    0,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,   -1,  299,  310,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,  324,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,  334,
  325,   -1,  327,   -1,  329,   41,  331,   43,   44,   45,
   -1,  336,  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,
  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,
  292,   -1,  397,   -1,  389,   -1,   -1,   -1,   -1,  404,
   -1,   -1,  407,   -1,  409,  410,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,  434,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  483,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,  498,  499,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,  419,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
   -1,  453,  454,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  285,
   -1,   -1,   -1,    0,   -1,  487,  488,   -1,   -1,   -1,
   -1,   -1,   -1,  299,  496,   -1,   -1,   -1,  304,  305,
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
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,    0,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   59,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,  262,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
    0,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   41,    0,   -1,   44,  336,   -1,   -1,  339,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   59,
   -1,   -1,  271,  354,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,  368,   -1,   -1,
   -1,   -1,   -1,   41,   -1,  376,   44,   -1,  379,  380,
  299,   -1,   -1,   -1,   -1,  304,  305,   -1,  389,  308,
   -1,   59,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,  405,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  432,   -1,   -1,   -1,  354,  355,   -1,  357,   -1,
   42,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,    0,   -1,   -1,  427,  428,
   -1,   -1,  513,  432,   -1,   -1,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   40,   41,   -1,   -1,   44,
   -1,  470,  471,   -1,  264,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   59,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,  368,  327,
   -1,   -1,   -1,  331,   -1,   -1,  376,   -1,  336,   -1,
  380,  339,   -1,   44,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,
   -1,  293,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,  389,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  446,  405,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   41,   -1,   -1,   44,  427,
  470,  471,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   59,  484,   -1,  444,   -1,  446,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,  493,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,  513,   -1,   -1,   -1,  517,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,  354,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,  481,
  482,  376,  484,   -1,   -1,  380,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,  389,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
  405,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  317,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,    0,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   91,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   91,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  405,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   41,   -1,  427,   44,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   59,  444,
   -1,  446,   -1,   -1,   -1,  271,   41,   -1,   -1,   44,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,    0,   59,   -1,   61,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,  315,
   -1,   -1,  318,   -1,   -1,  321,   91,   -1,   -1,  325,
   -1,  327,   -1,  329,   41,  331,   -1,   44,  513,   -1,
  336,  337,  517,  339,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  262,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,  271,   -1,   -1,
   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,  405,
   -1,  285,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,  315,   -1,   -1,  318,    0,   -1,  321,  444,   -1,
  446,  325,   -1,  327,   -1,  329,   41,  331,   -1,   44,
   -1,   -1,  336,  337,   -1,  339,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,  357,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,  258,  368,   -1,  370,  493,  372,  373,
  374,   -1,  376,   -1,   59,  379,  380,   -1,  273,   -1,
  275,  276,  277,   -1,   -1,  389,  281,  513,   -1,  284,
   -1,  517,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,
   -1,  405,  297,   -1,  325,   -1,  327,   -1,  303,   -1,
  331,  306,   -1,   -1,   -1,  336,   -1,  421,  339,   -1,
   -1,   -1,   -1,  427,   -1,  262,   -1,   -1,  432,   -1,
   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,    0,   -1,
  444,   -1,  446,   -1,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,  315,   41,
   -1,  318,   44,   -1,  405,   -1,   -1,   -1,  325,  493,
  327,   -1,   -1,   -1,  331,   -1,   -1,   59,  393,  336,
   -1,   -1,  339,   -1,  399,   -1,  427,   -1,  403,  513,
   -1,  432,   -1,  517,   -1,  410,   -1,  354,   -1,   -1,
    0,   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  428,   -1,   -1,   -1,   -1,  433,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,  458,   44,   -1,   -1,  462,  405,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,  262,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,  513,   -1,   -1,  432,  517,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,  444,   -1,  446,
  325,   -1,  327,   -1,    0,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,  354,
  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,  368,  339,   41,  493,   -1,   44,   -1,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  354,
   -1,   -1,   -1,   59,  389,   -1,  513,   -1,   -1,   -1,
  517,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,
  405,  376,   -1,   -1,  379,  380,    0,   -1,   -1,   -1,
  262,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,  315,   -1,   59,  318,   -1,   -1,  444,
   -1,  446,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,  493,   -1,
   -1,   -1,  262,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,  513,   -1,
   -1,   -1,  517,   -1,   -1,   -1,  368,   -1,  493,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   41,  389,  513,   44,
   -1,   -1,  517,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,  405,   59,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   41,   -1,   -1,   44,   -1,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,  354,   -1,  262,   -1,   59,    0,
   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,  405,   -1,   -1,   -1,  315,
   -1,  493,  318,   -1,   -1,   -1,   -1,   -1,   59,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  427,   -1,   -1,
  336,  513,  432,  339,   -1,  517,   -1,   -1,  262,   -1,
   -1,    0,   -1,   -1,  444,   -1,  446,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   41,  389,   -1,   44,   -1,   -1,   -1,   -1,
   -1,  315,   -1,  493,  318,   -1,   -1,   -1,   -1,  405,
   59,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
   -1,   -1,  336,  513,   -1,  339,   41,  517,   -1,   44,
   -1,  427,   -1,   -1,   -1,   -1,  432,  262,   -1,   -1,
  354,   -1,   -1,   -1,   59,    0,   -1,   -1,  444,   -1,
  446,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
  315,  405,   -1,  318,   -1,   -1,   -1,  493,   -1,   -1,
  325,   -1,  327,   -1,   59,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,  427,  339,   -1,   -1,  513,  432,   -1,
   -1,  517,   -1,   -1,  315,   -1,   -1,  318,   -1,  354,
  444,  262,  446,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,  368,   -1,  336,   -1,   -1,  339,   -1,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  389,    0,   -1,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  493,
  405,   -1,   -1,   -1,  315,  376,   -1,  318,  379,  380,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,  389,  513,
  331,   -1,  427,  517,   -1,  336,   41,  432,  339,   44,
   -1,   -1,   -1,  262,  405,   -1,   -1,   -1,   -1,  444,
   -1,  446,   -1,  354,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,  368,   -1,   -1,
   -1,  432,   -1,   -1,   -1,  376,   -1,  262,  379,  380,
   -1,   -1,   -1,  444,   -1,  446,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  493,  318,
   -1,   -1,   -1,   -1,  405,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,  513,   -1,
  339,   -1,  517,   -1,   -1,   -1,  427,   -1,   -1,   -1,
  315,  432,  493,  318,   -1,  354,   -1,  262,   -1,   -1,
  325,   -1,  327,  444,   -1,  446,  331,   -1,   -1,  368,
   -1,  336,  513,   -1,  339,   -1,  517,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  389,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,  405,   -1,   -1,   -1,
  315,  376,  493,  318,  379,  380,   -1,   -1,   -1,   -1,
  325,   -1,  327,   -1,  389,   -1,  331,   -1,  427,   -1,
   -1,  336,  513,  432,  339,   41,  517,   -1,   44,   -1,
  405,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,  354,
   -1,   -1,   -1,   59,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,  368,   -1,   -1,   -1,  432,   -1,   -1,
   -1,  376,   -1,   -1,  379,  380,   -1,  262,   -1,  444,
   -1,  446,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   41,   -1,   -1,   44,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,  513,   -1,   -1,    0,  517,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,  493,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,  444,
  325,  446,  327,   -1,   -1,   -1,  331,   -1,  513,   -1,
   -1,  336,  517,   -1,  339,   -1,   -1,   40,   41,   -1,
   -1,   44,   -1,   -1,   -1,    0,   -1,   -1,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,  493,   -1,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   41,   -1,  513,   44,
   -1,   -1,  517,   -1,   -1,   -1,   -1,   -1,   -1,    0,
  405,   -1,   -1,  450,   59,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,  427,  470,  471,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  262,  484,   -1,  444,
   41,  446,   -1,   44,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   59,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,  315,
   -1,   -1,  318,   -1,   -1,   -1,  262,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,  513,   -1,
  336,   -1,  517,  339,   -1,   -1,   -1,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,
   62,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,  315,
  376,   -1,  318,  379,  380,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,  389,   -1,  331,   40,   -1,   -1,   43,
  336,   45,   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  354,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,  368,   -1,   -1,   -1,  432,   -1,   -1,   -1,
  376,   -1,  315,  379,  380,  318,   -1,   -1,  444,   -1,
  446,   -1,  325,  389,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  315,  427,   -1,  318,   -1,  368,  432,  493,   -1,   -1,
  325,   -1,  327,  376,   -1,   -1,  331,  380,  444,   -1,
  446,  336,   -1,   -1,  339,   -1,  389,  513,   -1,   -1,
   -1,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,  315,   -1,   -1,  318,   -1,   -1,
   -1,  376,   -1,   58,  325,  380,  327,  493,   -1,  432,
  331,   -1,   -1,   -1,  389,  336,   -1,   -1,  339,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,  513,   -1,  271,
  405,  517,   -1,  354,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  285,   -1,   -1,   -1,  368,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  432,   -1,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,  444,
  493,  446,   -1,  267,   -1,   -1,   -1,   -1,   -1,  321,
  274,   -1,   -1,   -1,  405,   -1,   -1,  329,   -1,   -1,
  513,   -1,  286,   -1,  517,  337,   -1,   -1,  292,  293,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  432,   -1,  355,   -1,  357,   -1,  311,  493,   58,
   -1,   -1,   -1,  444,   63,  446,  320,   -1,  370,  323,
  372,  373,  374,   -1,   -1,   -1,   -1,   -1,  513,   -1,
   -1,   -1,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,  493,   -1,   -1,   -1,   -1,   -1,  372,  421,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  505,  506,  507,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,
  289,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,  348,
   -1,  350,  351,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,  481,  482,   -1,  484,
   -1,   -1,   -1,  372,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,  508,
  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  311,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,   -1,   -1,   -1,   42,   43,   -1,
   45,  372,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,
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
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,  348,  277,  350,  351,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  321,   -1,   -1,  284,
   -1,   -1,   -1,   -1,  329,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  337,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,  370,   -1,  372,  373,  374,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,  421,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,
   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  320,   -1,   -1,  323,   63,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,
  495,   -1,  497,  372,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  298,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  310,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
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
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  298,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  310,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,  334,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,  399,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,  284,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,  360,   -1,   -1,   -1,  364,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
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
   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,  364,
  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,
   -1,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,  472,  473,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  288,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
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
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,  472,  473,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,  272,   -1,   -1,  275,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,  292,   -1,   -1,  295,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,
  323,   63,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
  367,  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,  514,  515,  516,   -1,  518,  519,  520,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,  272,   -1,   -1,  275,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,  292,   -1,   -1,  295,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,
   -1,  323,   63,   -1,   -1,   -1,   -1,   -1,   -1,  335,
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
  452,  453,  454,  455,   -1,  457,  458,  459,  460,  325,
  462,  463,   -1,   -1,   -1,   -1,  472,  473,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,  395,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,  430,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,  360,
   -1,   -1,   -1,  364,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,  260,   -1,  500,
  501,  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,
   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,  300,   40,   -1,
   42,   43,   -1,   45,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,
  323,   63,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,  514,  515,  516,   -1,  518,  519,  520,  260,   -1,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   58,   -1,   -1,   -1,  300,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,  272,
   -1,  323,  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,  292,
   -1,   -1,  295,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  367,   -1,  369,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,  415,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,  437,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,
  274,   -1,  514,  515,  516,   -1,  518,  519,  520,  472,
  473,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   40,   -1,   -1,   43,   -1,   45,  489,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,
  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  504,   -1,  339,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,  362,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  419,
   -1,  508,  509,   -1,   -1,  425,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  267,   -1,   -1,  508,  509,
   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   40,   41,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
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
   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,
  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
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
  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
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
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  267,
   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
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
  518,  519,  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
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
  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
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
  516,    0,  518,  519,  520,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   40,   41,   -1,   -1,   44,  320,   -1,
   59,  323,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   41,   91,   -1,   44,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   91,   -1,   -1,   -1,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,    0,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   41,  508,  509,   44,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  258,
   -1,    0,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,
   -1,  258,   -1,   -1,  273,   -1,  275,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,  273,   -1,  275,  276,
  277,   -1,   -1,   -1,  281,   91,   -1,  284,  297,   -1,
   -1,   -1,   41,   -1,  303,   44,   -1,  306,  258,   -1,
  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,
   59,   -1,   61,  273,   -1,   -1,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,  348,
   -1,   -1,   91,  303,   -1,   -1,  306,   -1,   -1,   -1,
   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,  348,   -1,
  399,   -1,   -1,   -1,  403,   -1,  393,   -1,   -1,   -1,
   -1,  410,  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,
   -1,   -1,  372,  410,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,  428,   -1,  393,   -1,   -1,  433,   -1,   -1,  399,
   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,  458,
  410,   -1,  258,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,   -1,   -1,   -1,  462,   -1,  273,  428,   -1,
  276,  277,   -1,  433,   -1,  281,   -1,   -1,  284,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,  458,   -1,
  306,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,  258,
  270,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,  298,   -1,
   -1,   -1,  348,   -1,   -1,  305,   -1,   -1,  297,  309,
  310,   -1,  312,   -1,  303,  315,   -1,  306,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,  328,   -1,
   -1,   -1,   -1,   -1,  334,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,
   -1,   -1,   -1,  399,   -1,   -1,   -1,  403,   -1,  348,
   -1,   -1,   -1,   -1,  410,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  270,   -1,
   -1,   -1,  428,  372,   -1,   -1,   -1,  433,   -1,   -1,
   -1,  391,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  393,   -1,  298,   -1,   -1,  409,
  399,   -1,  458,   -1,  403,   -1,  462,  309,  310,   -1,
  312,  410,   -1,   -1,  424,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  434,   -1,  328,   -1,   -1,  428,
   -1,   -1,  334,  443,  433,  445,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,  458,
  470,  471,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,  391,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  510,  511,  512,  270,  514,  515,  516,  409,  518,  519,
  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  424,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  298,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,  309,  310,   -1,  312,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,  328,   -1,   -1,   -1,   -1,   -1,  334,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,  391,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,  272,   -1,   -1,  275,
   -1,   -1,   -1,  470,  471,  281,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  292,  484,   -1,  295,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,
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
   -1,   -1,  508,  509,   -1,   -1,   -1,  415,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,  489,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  452,   -1,   -1,  455,  456,  457,  458,
  459,  460,  461,  462,  463,  464,  465,  466,  467,  468,
  469,  470,  471,  472,  473,  474,  475,  476,   -1,   -1,
  479,  480,  481,  482,  483,  484,   -1,   -1,  487,  488,
  489,  490,  491,  492,  493,  494,  495,  496,  497,  498,
  499,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,  510,  511,  512,  513,  514,  515,  516,  517,  518,
  519,  520,  452,   -1,   -1,  455,  456,  457,  458,  459,
  460,   -1,  462,  463,  464,  465,  466,  467,  468,  469,
  470,  471,  472,  473,  474,  475,  476,   -1,   -1,  479,
  480,  481,  482,  483,  484,   -1,   -1,  487,  488,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  498,  499,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  510,  511,  512,  513,  514,  515,  516,  517,  518,  519,
  520,
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
"update : update_searched",
"update : update_positioned",
"update_searched : UPDATE table_name SET assignments where_clause plan_clause order_clause rows_clause",
"update_positioned : UPDATE table_name SET assignments cursor_clause",
"cursor_clause : WHERE CURRENT OF symbol_cursor_name",
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
{ yyval = yyvsp[0]; }
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
{ yyval = yyvsp[0]; }
break;
case 123:
{ yyval = yyvsp[0]; }
break;
case 124:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 125:
{ yyval = make_node (nod_redef_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 126:
{ yyval = make_node (nod_replace_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 127:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 128:
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 129:
{ yyval = NULL; }
break;
case 130:
{ yyval = make_list (yyvsp[0]); }
break;
case 132:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 133:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 134:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 135:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 136:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 137:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 138:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 139:
{ yyval = (dsql_nod*) 0;}
break;
case 140:
{ yyval = yyvsp[-1]; }
break;
case 141:
{ yyval = NULL; }
break;
case 144:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 145:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 146:
{ yyval = NULL; }
break;
case 147:
{ yyval = NULL; }
break;
case 148:
{ yyval = yyvsp[0]; }
break;
case 149:
{ yyval = NULL; }
break;
case 150:
{ yyval = NULL; }
break;
case 153:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 154:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 157:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 158:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 159:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
break;
case 160:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
break;
case 161:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 164:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
break;
case 165:
{yyval = NULL;}
break;
case 168:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 169:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 170:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 171:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 172:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 173:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 174:
{yyval = NULL;}
break;
case 177:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 179:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 180:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 181:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 182:
{ lex.g_file  = make_file();}
break;
case 187:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 188:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 195:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 196:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 197:
{ yyval = yyvsp[0]; }
break;
case 198:
{ yyval = yyvsp[0]; }
break;
case 199:
{ yyval = NULL; }
break;
case 201:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 204:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 205:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 206:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 207:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 210:
{ yyval = NULL; }
break;
case 211:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 212:
{ yyval = yyvsp[0]; }
break;
case 213:
{ yyval = NULL; }
break;
case 214:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 215:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 216:
{ yyval = yyvsp[-1]; }
break;
case 217:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 218:
{ yyval = yyvsp[0]; }
break;
case 219:
{ yyval = NULL; }
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
case 231:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 232:
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
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
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
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
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
{ yyval = (dsql_nod*) NULL; }
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
case 312:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 313:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 318:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
break;
case 319:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
break;
case 320:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 321:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
break;
case 322:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 323:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 324:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
break;
case 325:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 326:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 327:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
break;
case 328:
{ yyval = NULL; }
break;
case 329:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
break;
case 330:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
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
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
break;
case 341:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
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
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 360:
{ yyval = make_list (yyvsp[0]); }
break;
case 361:
{ yyval = make_list (yyvsp[-1]); }
break;
case 362:
{ yyval = NULL; }
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
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 367:
{ yyval = make_list (yyvsp[-1]); }
break;
case 368:
{ yyval = NULL; }
break;
case 370:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 371:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 372:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 373:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 374:
{ lex.beginning = lex_position(); }
break;
case 375:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 376:
{ lex.beginning = lex.last_token; }
break;
case 377:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 378:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 379:
{ yyval = 0; }
break;
case 380:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 381:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 382:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 384:
{ yyval = NULL; }
break;
case 385:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 386:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 404:
{ yyval = NULL; }
break;
case 405:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 406:
{ yyval = yyvsp[0]; }
break;
case 407:
{ yyval = yyvsp[0]; }
break;
case 408:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 412:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 413:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 418:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 419:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 433:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 434:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 435:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 467:
{ yyval = NULL; }
break;
case 468:
{ yyval = NULL; }
break;
case 469:
{ yyval = NULL; }
break;
case 470:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 471:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 472:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 473:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 474:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 475:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 476:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 477:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 478:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 479:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 480:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 482:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 483:
{ yyval = yyvsp[0]; }
break;
case 484:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 486:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 487:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 488:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 490:
{ yyval = NULL; }
break;
case 492:
{ yyval = NULL; }
break;
case 493:
{ yyval = yyvsp[0]; }
break;
case 494:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 500:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 501:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 502:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 503:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 504:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 505:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 506:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 511:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 512:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 514:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 515:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 516:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 522:
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
case 523:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 525:
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
case 526:
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
case 527:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 534:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 535:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 536:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 537:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 538:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 539:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 555:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 556:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 557:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 558:
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
case 559:
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
case 562:
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
case 563:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 564:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 565:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 566:
{ yyval = yyvsp[-1]; }
break;
case 567:
{ yyval = 0; }
break;
case 571:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 572:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 573:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 577:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 578:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 579:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 580:
{ yyval = 0; }
break;
case 581:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 584:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 585:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 588:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 589:
{ yyval = NULL; }
break;
case 591:
{ yyval = NULL; }
break;
case 592:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 594:
{ yyval = NULL; }
break;
case 596:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 601:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 602:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 603:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 604:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 605:
{ yyval = yyvsp[0];}
break;
case 607:
{ yyval = yyvsp[0];}
break;
case 608:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 609:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 610:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 611:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 612:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 613:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 614:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 615:
{ yyval = 0; }
break;
case 616:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 617:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 618:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 619:
{ yyval = (dsql_nod*) 0; }
break;
case 620:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 621:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 623:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 624:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 625:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 626:
{ yyval = 0; }
break;
case 628:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 629:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 630:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 631:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 632:
{ yyval = NULL; }
break;
case 633:
{ yyval = yyvsp[0]; }
break;
case 634:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 635:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 636:
{ yyval = NULL; }
break;
case 637:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 638:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 639:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 641:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 642:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 644:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 645:
{ lex.limit_clause = true; }
break;
case 646:
{ lex.limit_clause = false; }
break;
case 647:
{ lex.first_detection = true; }
break;
case 648:
{ lex.first_detection = false; }
break;
case 649:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 650:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 651:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 652:
{ yyval = 0; }
break;
case 653:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 654:
{ yyval = yyvsp[-2]; }
break;
case 655:
{ yyval = yyvsp[-1]; }
break;
case 656:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 657:
{ yyval = yyvsp[-1]; }
break;
case 658:
{ yyval = yyvsp[0]; }
break;
case 659:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 660:
{ yyval = 0; }
break;
case 661:
{ yyval = make_list (yyvsp[0]); }
break;
case 662:
{ yyval = 0; }
break;
case 664:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 666:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 669:
{ yyval = make_list (yyvsp[0]); }
break;
case 671:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 676:
{ yyval = yyvsp[-1]; }
break;
case 677:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 679:
{ yyval = NULL; }
break;
case 680:
{ yyval = make_list (yyvsp[-1]); }
break;
case 681:
{ yyval = NULL; }
break;
case 683:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 687:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 688:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 689:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 692:
{ yyval = yyvsp[0]; }
break;
case 693:
{ yyval = make_list (yyvsp[-1]); }
break;
case 694:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 695:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 696:
{ yyval = make_list (yyvsp[-1]); }
break;
case 697:
{ yyval = NULL; }
break;
case 699:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 700:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 701:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 702:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 703:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 704:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 705:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 708:
{ yyval = make_list (yyvsp[0]); }
break;
case 709:
{ yyval = NULL; }
break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 713:
{ yyval = yyvsp[0]; }
break;
case 714:
{ yyval = NULL; }
break;
case 715:
{ yyval = yyvsp[0]; }
break;
case 716:
{ yyval = NULL; }
break;
case 717:
{ yyval = yyvsp[0]; }
break;
case 718:
{ yyval = NULL; }
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
{ yyval = make_list (yyvsp[0]); }
break;
case 739:
{ yyval = 0; }
break;
case 741:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 743:
{ yyval = 0; }
break;
case 744:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 745:
{ yyval = 0; }
break;
case 746:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 747:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 748:
{ yyval = yyvsp[-1]; }
break;
case 749:
{ yyval = 0; }
break;
case 750:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 751:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 752:
{ yyval = NULL; }
break;
case 753:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 754:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 757:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 758:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 761:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 762:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 765:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 767:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 768:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 775:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 776:
{ yyval = yyvsp[0]; }
break;
case 778:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 781:
{ yyval = NULL; }
break;
case 782:
{ yyval = make_list (yyvsp[-1]); }
break;
case 784:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = NULL; }
break;
case 787:
{ yyval = make_list (yyvsp[-1]); }
break;
case 789:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 793:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 804:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 806:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 808:
{ yyval = yyvsp[-1]; }
break;
case 809:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 822:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 826:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 827:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 828:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 829:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 834:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 837:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 838:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 839:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 840:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 841:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 842:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 843:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 844:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 848:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 850:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 851:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 852:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 853:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 854:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 855:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 856:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 857:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 858:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 859:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 860:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 861:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 862:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 863:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 864:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 865:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 866:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 867:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 868:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 869:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 870:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 871:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 872:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 874:
{ yyval = make_list (yyvsp[-1]); }
break;
case 875:
{ yyval = yyvsp[-1]; }
break;
case 886:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 887:
{ yyval = yyvsp[0]; }
break;
case 888:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 889:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 890:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 891:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 892:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 893:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 894:
{ yyval = yyvsp[-1]; }
break;
case 895:
{ yyval = yyvsp[-1]; }
break;
case 899:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 900:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 901:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 904:
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
case 905:
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
case 906:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 907:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 909:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 911:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 912:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 913:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 914:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 915:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 916:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 918:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 919:
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
case 920:
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
case 921:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 922:
{ yyval = make_parameter (); }
break;
case 923:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 924:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 925:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 926:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 927:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 928:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 929:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 930:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 931:
{ yyval = yyvsp[0]; }
break;
case 932:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 934:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 935:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 936:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 937:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 938:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 940:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 941:
{ yyval = yyvsp[0];}
break;
case 945:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 946:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 947:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 948:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 949:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 950:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 951:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 952:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 953:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 954:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 955:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 957:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 959:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 960:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 961:
{ yyval = yyvsp[0]; }
break;
case 962:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 963:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 964:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 965:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 968:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 969:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 970:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 973:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 974:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 975:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 976:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 977:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 978:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 979:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 980:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 984:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 985:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
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
case 998:
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
