/* A Bison parser, made from parse.y, by GNU bison 1.75.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ACTIVE = 258,
     ADD = 259,
     AFTER = 260,
     ALL = 261,
     ALTER = 262,
     AND = 263,
     ANY = 264,
     AS = 265,
     ASC = 266,
     AT = 267,
     AVG = 268,
     AUTO = 269,
     BASENAME = 270,
     BEFORE = 271,
     BEGIN = 272,
     BETWEEN = 273,
     BLOB = 274,
     BY = 275,
     CACHE = 276,
     CAST = 277,
     CHARACTER = 278,
     CHECK = 279,
     CHECK_POINT_LEN = 280,
     COLLATE = 281,
     COMMA = 282,
     COMMIT = 283,
     COMMITTED = 284,
     COMPUTED = 285,
     CONCATENATE = 286,
     CONDITIONAL = 287,
     CONSTRAINT = 288,
     CONTAINING = 289,
     COUNT = 290,
     CREATE = 291,
     CSTRING = 292,
     CURRENT = 293,
     CURSOR = 294,
     DATABASE = 295,
     DATE = 296,
     DB_KEY = 297,
     KW_DEBUG = 298,
     DECIMAL = 299,
     DECLARE = 300,
     DEFAULT = 301,
     KW_DELETE = 302,
     DESC = 303,
     DISTINCT = 304,
     DO = 305,
     DOMAIN = 306,
     DROP = 307,
     ELSE = 308,
     END = 309,
     ENTRY_POINT = 310,
     EQL = 311,
     ESCAPE = 312,
     EXCEPTION = 313,
     EXECUTE = 314,
     EXISTS = 315,
     EXIT = 316,
     EXTERNAL = 317,
     FILTER = 318,
     FOR = 319,
     FOREIGN = 320,
     FROM = 321,
     FULL = 322,
     FUNCTION = 323,
     GDSCODE = 324,
     GEQ = 325,
     GENERATOR = 326,
     GEN_ID = 327,
     GRANT = 328,
     GROUP = 329,
     GROUP_COMMIT_WAIT = 330,
     GTR = 331,
     HAVING = 332,
     IF = 333,
     KW_IN = 334,
     INACTIVE = 335,
     INNER = 336,
     INPUT_TYPE = 337,
     INDEX = 338,
     INSERT = 339,
     INTEGER = 340,
     INTO = 341,
     IS = 342,
     ISOLATION = 343,
     JOIN = 344,
     KEY = 345,
     KW_CHAR = 346,
     KW_DEC = 347,
     KW_DOUBLE = 348,
     KW_FILE = 349,
     KW_FLOAT = 350,
     KW_INT = 351,
     KW_LONG = 352,
     KW_NULL = 353,
     KW_NUMERIC = 354,
     KW_UPPER = 355,
     KW_VALUE = 356,
     LENGTH = 357,
     LOGFILE = 358,
     LPAREN = 359,
     LEFT = 360,
     LEQ = 361,
     LEVEL = 362,
     LIKE = 363,
     LOG_BUF_SIZE = 364,
     LSS = 365,
     MANUAL = 366,
     MAXIMUM = 367,
     MAX_SEGMENT = 368,
     MERGE = 369,
     MESSAGE = 370,
     MINIMUM = 371,
     MODULE_NAME = 372,
     NAMES = 373,
     NATIONAL = 374,
     NATURAL = 375,
     NCHAR = 376,
     NEQ = 377,
     NO = 378,
     NOT = 379,
     NOT_GTR = 380,
     NOT_LSS = 381,
     NUM_LOG_BUFS = 382,
     OF = 383,
     ON = 384,
     ONLY = 385,
     OPTION = 386,
     OR = 387,
     ORDER = 388,
     OUTER = 389,
     OUTPUT_TYPE = 390,
     OVERFLOW = 391,
     PAGE = 392,
     PAGES = 393,
     KW_PAGE_SIZE = 394,
     PARAMETER = 395,
     PASSWORD = 396,
     PLAN = 397,
     POSITION = 398,
     POST_EVENT = 399,
     PRECISION = 400,
     PRIMARY = 401,
     PRIVILEGES = 402,
     PROCEDURE = 403,
     PROTECTED = 404,
     RAW_PARTITIONS = 405,
     READ = 406,
     REAL = 407,
     REFERENCES = 408,
     RESERVING = 409,
     RETAIN = 410,
     RETURNING_VALUES = 411,
     RETURNS = 412,
     REVOKE = 413,
     RIGHT = 414,
     RPAREN = 415,
     ROLLBACK = 416,
     SEGMENT = 417,
     SELECT = 418,
     SET = 419,
     SHADOW = 420,
     KW_SHARED = 421,
     SINGULAR = 422,
     KW_SIZE = 423,
     SMALLINT = 424,
     SNAPSHOT = 425,
     SOME = 426,
     SORT = 427,
     SQLCODE = 428,
     STABILITY = 429,
     STARTING = 430,
     STATISTICS = 431,
     SUB_TYPE = 432,
     SUSPEND = 433,
     SUM = 434,
     TABLE = 435,
     THEN = 436,
     TO = 437,
     TRANSACTION = 438,
     TRIGGER = 439,
     UNCOMMITTED = 440,
     UNION = 441,
     UNIQUE = 442,
     UPDATE = 443,
     USER = 444,
     VALUES = 445,
     VARCHAR = 446,
     VARIABLE = 447,
     VARYING = 448,
     VERSION = 449,
     VIEW = 450,
     WAIT = 451,
     WHEN = 452,
     WHERE = 453,
     WHILE = 454,
     WITH = 455,
     WORK = 456,
     WRITE = 457,
     FLOAT_NUMBER = 458,
     NUMBER = 459,
     NUMERIC = 460,
     SYMBOL = 461,
     STRING = 462,
     INTRODUCER = 463,
     ACTION = 464,
     ADMIN = 465,
     CASCADE = 466,
     FREE_IT = 467,
     RESTRICT = 468,
     ROLE = 469,
     COLUMN = 470,
     TYPE = 471,
     EXTRACT = 472,
     YEAR = 473,
     MONTH = 474,
     DAY = 475,
     HOUR = 476,
     MINUTE = 477,
     SECOND = 478,
     WEEKDAY = 479,
     YEARDAY = 480,
     TIME = 481,
     TIMESTAMP = 482,
     CURRENT_DATE = 483,
     CURRENT_TIME = 484,
     CURRENT_TIMESTAMP = 485,
     NUMBER64BIT = 486,
     SCALEDINT = 487,
     CURRENT_USER = 488,
     CURRENT_ROLE = 489,
     KW_BREAK = 490,
     SUBSTRING = 491,
     RECREATE = 492,
     KW_DESCRIPTOR = 493,
     FIRST = 494,
     SKIP = 495,
     CURRENT_CONNECTION = 496,
     CURRENT_TRANSACTION = 497,
     BIGINT = 498,
     CASE = 499,
     NULLIF = 500,
     COALESCE = 501,
     USING = 502,
     NULLS = 503,
     LAST = 504,
     ROW_COUNT = 505,
     LOCK = 506,
     SAVEPOINT = 507,
     RELEASE = 508,
     STATEMENT = 509,
     LEAVE = 510,
     INSERTING = 511,
     UPDATING = 512,
     DELETING = 513,
     KW_INSERTING = 514,
     KW_UPDATING = 515,
     KW_DELETING = 516,
     BACKUP = 517,
     KW_DIFFERENCE = 518,
     OPEN = 519,
     CLOSE = 520,
     FETCH = 521,
     ROWS = 522,
     BLOCK = 523
   };
#endif
#define ACTIVE 258
#define ADD 259
#define AFTER 260
#define ALL 261
#define ALTER 262
#define AND 263
#define ANY 264
#define AS 265
#define ASC 266
#define AT 267
#define AVG 268
#define AUTO 269
#define BASENAME 270
#define BEFORE 271
#define BEGIN 272
#define BETWEEN 273
#define BLOB 274
#define BY 275
#define CACHE 276
#define CAST 277
#define CHARACTER 278
#define CHECK 279
#define CHECK_POINT_LEN 280
#define COLLATE 281
#define COMMA 282
#define COMMIT 283
#define COMMITTED 284
#define COMPUTED 285
#define CONCATENATE 286
#define CONDITIONAL 287
#define CONSTRAINT 288
#define CONTAINING 289
#define COUNT 290
#define CREATE 291
#define CSTRING 292
#define CURRENT 293
#define CURSOR 294
#define DATABASE 295
#define DATE 296
#define DB_KEY 297
#define KW_DEBUG 298
#define DECIMAL 299
#define DECLARE 300
#define DEFAULT 301
#define KW_DELETE 302
#define DESC 303
#define DISTINCT 304
#define DO 305
#define DOMAIN 306
#define DROP 307
#define ELSE 308
#define END 309
#define ENTRY_POINT 310
#define EQL 311
#define ESCAPE 312
#define EXCEPTION 313
#define EXECUTE 314
#define EXISTS 315
#define EXIT 316
#define EXTERNAL 317
#define FILTER 318
#define FOR 319
#define FOREIGN 320
#define FROM 321
#define FULL 322
#define FUNCTION 323
#define GDSCODE 324
#define GEQ 325
#define GENERATOR 326
#define GEN_ID 327
#define GRANT 328
#define GROUP 329
#define GROUP_COMMIT_WAIT 330
#define GTR 331
#define HAVING 332
#define IF 333
#define KW_IN 334
#define INACTIVE 335
#define INNER 336
#define INPUT_TYPE 337
#define INDEX 338
#define INSERT 339
#define INTEGER 340
#define INTO 341
#define IS 342
#define ISOLATION 343
#define JOIN 344
#define KEY 345
#define KW_CHAR 346
#define KW_DEC 347
#define KW_DOUBLE 348
#define KW_FILE 349
#define KW_FLOAT 350
#define KW_INT 351
#define KW_LONG 352
#define KW_NULL 353
#define KW_NUMERIC 354
#define KW_UPPER 355
#define KW_VALUE 356
#define LENGTH 357
#define LOGFILE 358
#define LPAREN 359
#define LEFT 360
#define LEQ 361
#define LEVEL 362
#define LIKE 363
#define LOG_BUF_SIZE 364
#define LSS 365
#define MANUAL 366
#define MAXIMUM 367
#define MAX_SEGMENT 368
#define MERGE 369
#define MESSAGE 370
#define MINIMUM 371
#define MODULE_NAME 372
#define NAMES 373
#define NATIONAL 374
#define NATURAL 375
#define NCHAR 376
#define NEQ 377
#define NO 378
#define NOT 379
#define NOT_GTR 380
#define NOT_LSS 381
#define NUM_LOG_BUFS 382
#define OF 383
#define ON 384
#define ONLY 385
#define OPTION 386
#define OR 387
#define ORDER 388
#define OUTER 389
#define OUTPUT_TYPE 390
#define OVERFLOW 391
#define PAGE 392
#define PAGES 393
#define KW_PAGE_SIZE 394
#define PARAMETER 395
#define PASSWORD 396
#define PLAN 397
#define POSITION 398
#define POST_EVENT 399
#define PRECISION 400
#define PRIMARY 401
#define PRIVILEGES 402
#define PROCEDURE 403
#define PROTECTED 404
#define RAW_PARTITIONS 405
#define READ 406
#define REAL 407
#define REFERENCES 408
#define RESERVING 409
#define RETAIN 410
#define RETURNING_VALUES 411
#define RETURNS 412
#define REVOKE 413
#define RIGHT 414
#define RPAREN 415
#define ROLLBACK 416
#define SEGMENT 417
#define SELECT 418
#define SET 419
#define SHADOW 420
#define KW_SHARED 421
#define SINGULAR 422
#define KW_SIZE 423
#define SMALLINT 424
#define SNAPSHOT 425
#define SOME 426
#define SORT 427
#define SQLCODE 428
#define STABILITY 429
#define STARTING 430
#define STATISTICS 431
#define SUB_TYPE 432
#define SUSPEND 433
#define SUM 434
#define TABLE 435
#define THEN 436
#define TO 437
#define TRANSACTION 438
#define TRIGGER 439
#define UNCOMMITTED 440
#define UNION 441
#define UNIQUE 442
#define UPDATE 443
#define USER 444
#define VALUES 445
#define VARCHAR 446
#define VARIABLE 447
#define VARYING 448
#define VERSION 449
#define VIEW 450
#define WAIT 451
#define WHEN 452
#define WHERE 453
#define WHILE 454
#define WITH 455
#define WORK 456
#define WRITE 457
#define FLOAT_NUMBER 458
#define NUMBER 459
#define NUMERIC 460
#define SYMBOL 461
#define STRING 462
#define INTRODUCER 463
#define ACTION 464
#define ADMIN 465
#define CASCADE 466
#define FREE_IT 467
#define RESTRICT 468
#define ROLE 469
#define COLUMN 470
#define TYPE 471
#define EXTRACT 472
#define YEAR 473
#define MONTH 474
#define DAY 475
#define HOUR 476
#define MINUTE 477
#define SECOND 478
#define WEEKDAY 479
#define YEARDAY 480
#define TIME 481
#define TIMESTAMP 482
#define CURRENT_DATE 483
#define CURRENT_TIME 484
#define CURRENT_TIMESTAMP 485
#define NUMBER64BIT 486
#define SCALEDINT 487
#define CURRENT_USER 488
#define CURRENT_ROLE 489
#define KW_BREAK 490
#define SUBSTRING 491
#define RECREATE 492
#define KW_DESCRIPTOR 493
#define FIRST 494
#define SKIP 495
#define CURRENT_CONNECTION 496
#define CURRENT_TRANSACTION 497
#define BIGINT 498
#define CASE 499
#define NULLIF 500
#define COALESCE 501
#define USING 502
#define NULLS 503
#define LAST 504
#define ROW_COUNT 505
#define LOCK 506
#define SAVEPOINT 507
#define RELEASE 508
#define STATEMENT 509
#define LEAVE 510
#define INSERTING 511
#define UPDATING 512
#define DELETING 513
#define KW_INSERTING 514
#define KW_UPDATING 515
#define KW_DELETING 516
#define BACKUP 517
#define KW_DIFFERENCE 518
#define OPEN 519
#define CLOSE 520
#define FETCH 521
#define ROWS 522
#define BLOCK 523




/* Copy the first part of user declarations.  */

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
 *   in rdb_source fields when altering domains plus one unexpected null pointer.
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

//static void	yyerror(const TEXT*); redeclaration.

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#ifndef YYSTYPE
typedef int yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Copy the second part of user declarations.  */


/* Line 213 of /usr/local/share/bison/yacc.c.  */
#line 845 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short DSQL_yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];	\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  162
#define YYLAST   7179

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  285
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  439
/* YYNRULES -- Number of rules. */
#define YYNRULES  1042
/* YYNRULES -- Number of states. */
#define YYNSTATES  1940

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   523

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     278,   279,   274,   272,   277,   273,   283,   275,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   280,   276,
     270,   269,   271,   284,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   281,     2,   282,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    49,    57,    66,    73,    81,    87,
      89,    92,    94,    97,    99,   101,   105,   107,   109,   111,
     113,   116,   119,   123,   124,   128,   129,   131,   139,   148,
     155,   163,   170,   178,   183,   187,   189,   193,   197,   201,
     204,   207,   210,   213,   215,   219,   221,   224,   227,   229,
     233,   235,   237,   241,   243,   246,   249,   252,   256,   265,
     267,   269,   275,   276,   278,   282,   284,   288,   291,   296,
     298,   302,   305,   309,   314,   319,   322,   332,   335,   339,
     347,   350,   353,   356,   359,   362,   365,   368,   371,   374,
     377,   380,   383,   386,   391,   394,   397,   399,   400,   402,
     404,   411,   418,   420,   422,   423,   424,   426,   427,   432,
     433,   435,   437,   440,   449,   451,   452,   456,   457,   458,
     460,   462,   465,   467,   469,   471,   474,   481,   483,   485,
     489,   490,   492,   494,   495,   497,   499,   502,   506,   511,
     514,   517,   521,   522,   524,   526,   529,   531,   533,   535,
     540,   544,   548,   552,   556,   560,   562,   564,   571,   575,
     577,   581,   583,   587,   590,   592,   593,   596,   600,   602,
     603,   605,   607,   610,   614,   619,   620,   622,   625,   626,
     628,   630,   636,   642,   646,   649,   650,   652,   656,   658,
     660,   667,   671,   674,   681,   684,   686,   689,   692,   695,
     696,   698,   700,   703,   704,   707,   708,   710,   712,   714,
     716,   718,   720,   721,   723,   725,   728,   731,   734,   740,
     742,   745,   749,   752,   755,   756,   758,   760,   762,   764,
     768,   773,   782,   787,   788,   795,   797,   799,   802,   805,
     806,   810,   814,   816,   819,   822,   825,   834,   843,   852,
     861,   865,   866,   871,   872,   874,   878,   883,   885,   889,
     892,   896,   900,   902,   904,   905,   907,   910,   915,   917,
     919,   923,   925,   926,   929,   931,   938,   940,   942,   946,
     948,   951,   952,   954,   957,   960,   963,   965,   967,   969,
     971,   973,   975,   978,   980,   982,   985,   987,   989,   991,
     994,   997,   999,  1002,  1006,  1011,  1014,  1021,  1026,  1028,
    1030,  1039,  1049,  1056,  1065,  1072,  1077,  1078,  1083,  1086,
    1088,  1092,  1093,  1096,  1101,  1102,  1104,  1106,  1110,  1114,
    1122,  1125,  1126,  1129,  1132,  1136,  1140,  1141,  1143,  1146,
    1151,  1153,  1157,  1160,  1163,  1166,  1168,  1170,  1172,  1174,
    1177,  1180,  1186,  1187,  1192,  1200,  1204,  1205,  1207,  1211,
    1215,  1223,  1231,  1233,  1235,  1239,  1244,  1252,  1255,  1257,
    1261,  1263,  1265,  1272,  1276,  1277,  1278,  1279,  1280,  1284,
    1285,  1295,  1305,  1307,  1309,  1310,  1313,  1315,  1317,  1319,
    1321,  1323,  1327,  1331,  1335,  1339,  1343,  1347,  1353,  1359,
    1365,  1371,  1377,  1383,  1386,  1387,  1392,  1395,  1399,  1403,
    1406,  1409,  1413,  1417,  1420,  1424,  1431,  1433,  1436,  1441,
    1445,  1448,  1451,  1454,  1457,  1461,  1463,  1467,  1471,  1475,
    1478,  1481,  1486,  1491,  1497,  1499,  1501,  1503,  1505,  1507,
    1509,  1511,  1513,  1515,  1517,  1519,  1521,  1523,  1525,  1527,
    1529,  1531,  1533,  1535,  1537,  1539,  1541,  1543,  1545,  1547,
    1549,  1551,  1553,  1555,  1558,  1561,  1564,  1566,  1568,  1570,
    1571,  1574,  1577,  1578,  1580,  1583,  1586,  1589,  1592,  1595,
    1600,  1604,  1607,  1610,  1612,  1616,  1624,  1626,  1627,  1629,
    1630,  1633,  1636,  1639,  1642,  1645,  1648,  1651,  1654,  1657,
    1661,  1664,  1667,  1670,  1672,  1674,  1676,  1678,  1683,  1689,
    1691,  1695,  1697,  1701,  1703,  1706,  1708,  1710,  1712,  1714,
    1716,  1718,  1720,  1722,  1724,  1726,  1728,  1733,  1738,  1745,
    1751,  1755,  1756,  1759,  1762,  1763,  1767,  1768,  1773,  1775,
    1781,  1786,  1788,  1793,  1795,  1798,  1801,  1803,  1805,  1807,
    1810,  1813,  1816,  1819,  1820,  1824,  1830,  1832,  1834,  1837,
    1841,  1843,  1846,  1850,  1851,  1853,  1855,  1857,  1863,  1869,
    1876,  1878,  1880,  1882,  1885,  1890,  1892,  1893,  1899,  1901,
    1902,  1906,  1909,  1911,  1912,  1915,  1916,  1918,  1919,  1923,
    1925,  1926,  1928,  1931,  1933,  1935,  1937,  1939,  1942,  1945,
    1947,  1950,  1954,  1956,  1958,  1962,  1966,  1968,  1971,  1975,
    1977,  1980,  1981,  1984,  1986,  1988,  1989,  1991,  1993,  1995,
    1999,  2002,  2006,  2007,  2009,  2013,  2018,  2024,  2026,  2030,
    2035,  2039,  2040,  2042,  2046,  2050,  2052,  2054,  2055,  2057,
    2059,  2064,  2065,  2068,  2073,  2074,  2078,  2079,  2082,  2083,
    2086,  2087,  2097,  2109,  2110,  2111,  2112,  2113,  2117,  2120,
    2122,  2123,  2127,  2133,  2137,  2140,  2146,  2149,  2151,  2153,
    2155,  2157,  2159,  2163,  2165,  2169,  2171,  2172,  2175,  2177,
    2181,  2183,  2185,  2187,  2194,  2196,  2197,  2201,  2202,  2204,
    2208,  2215,  2219,  2224,  2227,  2231,  2232,  2234,  2237,  2239,
    2241,  2243,  2246,  2248,  2251,  2253,  2256,  2257,  2261,  2262,
    2264,  2268,  2270,  2273,  2274,  2277,  2278,  2281,  2282,  2287,
    2289,  2292,  2294,  2296,  2297,  2299,  2303,  2306,  2308,  2310,
    2313,  2315,  2320,  2324,  2326,  2330,  2335,  2336,  2345,  2351,
    2353,  2355,  2360,  2365,  2370,  2372,  2374,  2380,  2386,  2388,
    2392,  2396,  2398,  2400,  2402,  2406,  2414,  2422,  2428,  2432,
    2433,  2435,  2437,  2439,  2442,  2443,  2445,  2447,  2449,  2450,
    2454,  2456,  2460,  2462,  2463,  2467,  2469,  2473,  2475,  2479,
    2483,  2485,  2487,  2491,  2493,  2496,  2498,  2502,  2506,  2508,
    2511,  2515,  2519,  2523,  2527,  2529,  2533,  2536,  2538,  2540,
    2542,  2544,  2546,  2548,  2550,  2552,  2554,  2556,  2560,  2564,
    2568,  2572,  2576,  2580,  2584,  2588,  2595,  2602,  2609,  2616,
    2623,  2630,  2637,  2644,  2651,  2658,  2665,  2672,  2679,  2686,
    2693,  2700,  2702,  2704,  2710,  2717,  2721,  2726,  2732,  2739,
    2743,  2748,  2752,  2757,  2761,  2766,  2771,  2777,  2782,  2787,
    2791,  2796,  2798,  2800,  2802,  2804,  2806,  2808,  2810,  2814,
    2818,  2830,  2842,  2844,  2846,  2848,  2850,  2852,  2854,  2856,
    2858,  2860,  2863,  2866,  2870,  2874,  2878,  2882,  2886,  2890,
    2894,  2898,  2900,  2902,  2904,  2906,  2910,  2912,  2914,  2916,
    2918,  2920,  2925,  2927,  2931,  2933,  2936,  2938,  2940,  2942,
    2944,  2946,  2948,  2950,  2953,  2956,  2959,  2961,  2963,  2965,
    2967,  2969,  2971,  2973,  2975,  2977,  2979,  2982,  2984,  2987,
    2989,  2991,  2993,  2995,  2997,  3000,  3002,  3004,  3006,  3008,
    3010,  3015,  3021,  3027,  3033,  3039,  3045,  3051,  3057,  3063,
    3069,  3075,  3082,  3084,  3091,  3093,  3098,  3106,  3109,  3110,
    3115,  3119,  3126,  3128,  3130,  3137,  3144,  3146,  3148,  3153,
    3160,  3165,  3171,  3175,  3181,  3186,  3192,  3194,  3196,  3198,
    3200,  3202,  3204,  3206,  3208,  3210,  3212,  3214,  3216,  3217,
    3219,  3221,  3223,  3225,  3227,  3229,  3231,  3233,  3235,  3237,
    3239,  3241,  3243,  3245,  3247,  3249,  3251,  3253,  3255,  3257,
    3259,  3261,  3263,  3265,  3267,  3269,  3271,  3273,  3275,  3277,
    3279,  3281,  3283,  3285,  3287,  3289,  3291,  3293,  3295,  3297,
    3299,  3301,  3303
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     286,     0,    -1,   287,    -1,   287,   276,    -1,   479,    -1,
     621,    -1,   533,    -1,   317,    -1,   307,    -1,   610,    -1,
     501,    -1,   288,    -1,   609,    -1,   452,    -1,   453,    -1,
     319,    -1,   321,    -1,   297,    -1,   534,    -1,   527,    -1,
     556,    -1,   525,    -1,   614,    -1,    43,   668,    -1,    73,
     290,   129,   289,   182,   301,   294,    -1,    73,   292,   129,
     148,   296,   182,   301,   294,    -1,    73,   290,   129,   289,
     182,   299,    -1,    73,   292,   129,   148,   296,   182,   299,
      -1,    73,   303,   182,   305,   295,    -1,   593,    -1,   180,
     593,    -1,     6,    -1,     6,   147,    -1,   291,    -1,   293,
      -1,   291,   277,   293,    -1,    59,    -1,   163,    -1,    84,
      -1,    47,    -1,   188,   626,    -1,   153,   626,    -1,   200,
      73,   131,    -1,    -1,   200,   210,   131,    -1,    -1,   713,
      -1,   158,   298,   290,   129,   289,    66,   301,    -1,   158,
     298,   292,   129,   148,   296,    66,   301,    -1,   158,   290,
     129,   289,    66,   301,    -1,   158,   292,   129,   148,   296,
      66,   301,    -1,   158,   290,   129,   289,    66,   299,    -1,
     158,   292,   129,   148,   296,    66,   299,    -1,   158,   303,
      66,   305,    -1,    73,   131,    64,    -1,   300,    -1,   299,
     277,   300,    -1,   299,   277,   302,    -1,   301,   277,   300,
      -1,   148,   713,    -1,   184,   717,    -1,   195,   720,    -1,
     214,   714,    -1,   302,    -1,   301,   277,   302,    -1,   718,
      -1,   189,   718,    -1,    74,   718,    -1,   304,    -1,   303,
     277,   304,    -1,   714,    -1,   306,    -1,   305,   277,   306,
      -1,   718,    -1,   189,   718,    -1,    45,   308,    -1,    63,
     316,    -1,    62,    68,   309,    -1,   698,   311,   157,   314,
      55,   667,   117,   667,    -1,   508,    -1,    19,    -1,    37,
     278,   671,   279,   514,    -1,    -1,   312,    -1,   278,   312,
     279,    -1,   313,    -1,   312,   277,   313,    -1,   380,   310,
      -1,   380,   310,    20,   238,    -1,   315,    -1,   278,   315,
     279,    -1,   380,   310,    -1,   380,   310,   212,    -1,   380,
     310,    20,   101,    -1,   380,   310,    20,   238,    -1,   140,
     671,    -1,   707,    82,   513,   135,   513,    55,   667,   117,
     667,    -1,    36,   318,    -1,    58,   706,   667,    -1,   323,
     561,    83,   710,   129,   593,   324,    -1,   148,   399,    -1,
     180,   367,    -1,   184,   471,    -1,   195,   457,    -1,    71,
     340,    -1,    40,   342,    -1,    51,   331,    -1,   165,   325,
      -1,   214,   341,    -1,   237,   320,    -1,   148,   400,    -1,
     180,   368,    -1,   195,   458,    -1,    36,   132,     7,   322,
      -1,   148,   401,    -1,   184,   472,    -1,   187,    -1,    -1,
     628,    -1,   627,    -1,   374,   278,   468,   656,   469,   279,
      -1,   671,   326,   327,   667,   328,   329,    -1,   111,    -1,
      14,    -1,    -1,    -1,    32,    -1,    -1,   102,   343,   674,
     366,    -1,    -1,   330,    -1,   355,    -1,   330,   355,    -1,
     377,   332,   503,   468,   333,   469,   334,   376,    -1,    10,
      -1,    -1,    46,   468,   382,    -1,    -1,    -1,   335,    -1,
     336,    -1,   335,   336,    -1,   337,    -1,   338,    -1,   339,
      -1,   124,    98,    -1,   468,    24,   278,   635,   279,   469,
      -1,   709,    -1,   714,    -1,   344,   345,   348,    -1,    -1,
     269,    -1,   667,    -1,    -1,   346,    -1,   347,    -1,   346,
     347,    -1,   139,   343,   671,    -1,   102,   343,   674,   366,
      -1,   189,   667,    -1,   141,   667,    -1,   164,   118,   667,
      -1,    -1,   349,    -1,   350,    -1,   349,   350,    -1,   355,
      -1,   352,    -1,   351,    -1,    46,    23,   164,   700,    -1,
     263,    94,   667,    -1,    75,   343,   674,    -1,    25,   343,
     674,    -1,   127,   343,   671,    -1,   109,   343,   672,    -1,
     354,    -1,   353,    -1,   103,   278,   356,   279,   136,   357,
      -1,   103,    15,   357,    -1,   103,    -1,   361,   667,   362,
      -1,   357,    -1,   356,   277,   357,    -1,   358,   359,    -1,
     667,    -1,    -1,   359,   360,    -1,   168,   343,   674,    -1,
      94,    -1,    -1,   363,    -1,   364,    -1,   363,   364,    -1,
     175,   365,   674,    -1,   102,   343,   674,   366,    -1,    -1,
      12,    -1,    12,   137,    -1,    -1,   137,    -1,   138,    -1,
     593,   369,   278,   370,   279,    -1,   593,   369,   278,   370,
     279,    -1,    62,    94,   667,    -1,    62,   667,    -1,    -1,
     371,    -1,   370,   277,   371,    -1,   372,    -1,   387,    -1,
     377,   375,   381,   469,   383,   376,    -1,   377,   504,   373,
      -1,   377,   373,    -1,   374,   278,   468,   656,   469,   279,
      -1,    30,    20,    -1,    30,    -1,   503,   468,    -1,   633,
     466,    -1,    26,   701,    -1,    -1,   633,    -1,   633,    -1,
     380,   503,    -1,    -1,    46,   382,    -1,    -1,   660,    -1,
     664,    -1,   665,    -1,   666,    -1,   697,    -1,   657,    -1,
      -1,   384,    -1,   385,    -1,   384,   385,    -1,   388,   386,
      -1,   124,    98,    -1,   153,   593,   626,   395,   393,    -1,
     394,    -1,   187,   393,    -1,   146,    90,   393,    -1,   388,
     389,    -1,    33,   703,    -1,    -1,   390,    -1,   391,    -1,
     392,    -1,   394,    -1,   187,   627,   393,    -1,   146,    90,
     627,   393,    -1,    65,    90,   627,   153,   593,   626,   395,
     393,    -1,   247,   561,    83,   710,    -1,    -1,   468,    24,
     278,   635,   279,   469,    -1,   396,    -1,   397,    -1,   397,
     396,    -1,   396,   397,    -1,    -1,   129,   188,   398,    -1,
     129,    47,   398,    -1,   211,    -1,   164,    46,    -1,   164,
      98,    -1,   123,   209,    -1,   713,   403,   404,    10,   466,
     410,   419,   469,    -1,   713,   403,   404,    10,   466,   410,
     419,   469,    -1,   713,   403,   404,    10,   466,   410,   419,
     469,    -1,   713,   403,   404,    10,   466,   410,   419,   469,
      -1,   278,   405,   279,    -1,    -1,   157,   278,   407,   279,
      -1,    -1,   406,    -1,   405,   277,   406,    -1,   378,   504,
     409,   469,    -1,   408,    -1,   407,   277,   408,    -1,   378,
     504,    -1,    46,   466,   382,    -1,   269,   466,   382,    -1,
     466,    -1,   411,    -1,    -1,   412,    -1,   411,   412,    -1,
      45,   415,   413,   276,    -1,   414,    -1,   417,    -1,   377,
     504,   416,    -1,   192,    -1,    -1,   269,   382,    -1,   381,
      -1,   704,    39,    64,   278,   569,   279,    -1,   422,    -1,
     419,    -1,    17,   420,    54,    -1,   421,    -1,   421,   443,
      -1,    -1,   418,    -1,   421,   418,    -1,   618,   276,    -1,
     610,   276,    -1,   423,    -1,   424,    -1,   425,    -1,   426,
      -1,   428,    -1,   431,    -1,   609,   276,    -1,   432,    -1,
     434,    -1,   614,   276,    -1,   439,    -1,   429,    -1,   430,
      -1,   178,   276,    -1,    61,   276,    -1,   441,    -1,   447,
     276,    -1,    58,   706,   276,    -1,    58,   706,   656,   276,
      -1,    58,   276,    -1,    59,   148,   713,   436,   437,   276,
      -1,    59,   427,   656,   276,    -1,   191,    -1,   254,    -1,
     440,    64,   556,    86,   438,   442,    50,   418,    -1,   440,
      64,    59,   427,   656,    86,   438,    50,   418,    -1,    59,
     427,   656,    86,   438,   276,    -1,    78,   278,   635,   279,
     181,   418,    53,   418,    -1,    78,   278,   635,   279,   181,
     418,    -1,   144,   656,   433,   276,    -1,    -1,   556,    86,
     438,   276,    -1,   280,   719,    -1,   620,    -1,   278,   620,
     279,    -1,    -1,   156,   438,    -1,   156,   278,   438,   279,
      -1,    -1,   435,    -1,   632,    -1,   438,   277,   632,    -1,
     438,   277,   435,    -1,   440,   199,   278,   635,   279,    50,
     418,    -1,   712,   280,    -1,    -1,   235,   276,    -1,   255,
     276,    -1,   255,   712,   276,    -1,    10,    39,   704,    -1,
      -1,   444,    -1,   443,   444,    -1,   197,   445,    50,   418,
      -1,   446,    -1,   445,   277,   446,    -1,   173,   668,    -1,
      69,   708,    -1,    58,   706,    -1,     9,    -1,   448,    -1,
     450,    -1,   449,    -1,   264,   704,    -1,   265,   704,    -1,
     266,   451,   704,    86,   438,    -1,    -1,    59,   148,   713,
     436,    -1,    59,   268,   454,   404,    10,   410,   419,    -1,
     278,   455,   279,    -1,    -1,   456,    -1,   455,   277,   456,
      -1,   408,   269,   663,    -1,   720,   626,    10,   466,   459,
     470,   467,    -1,   720,   626,    10,   466,   459,   470,   467,
      -1,   460,    -1,   461,    -1,   460,   186,   461,    -1,   460,
     186,     6,   461,    -1,   163,   577,   578,   462,   599,   595,
     598,    -1,    66,   463,    -1,   464,    -1,   463,   277,   464,
      -1,   465,    -1,   592,    -1,   464,   594,    89,   464,   129,
     635,    -1,   278,   465,   279,    -1,    -1,    -1,    -1,    -1,
     200,    24,   131,    -1,    -1,   717,    64,   593,   473,   474,
     477,   468,   478,   469,    -1,   717,    64,   593,   473,   474,
     477,   468,   478,   469,    -1,     3,    -1,    80,    -1,    -1,
     475,   476,    -1,    16,    -1,     5,    -1,    84,    -1,   188,
      -1,    47,    -1,    84,   132,   188,    -1,    84,   132,    47,
      -1,   188,   132,    84,    -1,   188,   132,    47,    -1,    47,
     132,    84,    -1,    47,   132,   188,    -1,    84,   132,   188,
     132,    47,    -1,    84,   132,    47,   132,   188,    -1,   188,
     132,    84,   132,    47,    -1,   188,   132,    47,   132,    84,
      -1,    47,   132,    84,   132,   188,    -1,    47,   132,   188,
     132,    84,    -1,   143,   669,    -1,    -1,    10,   468,   410,
     419,    -1,     7,   480,    -1,    58,   706,   667,    -1,   180,
     593,   485,    -1,   184,   498,    -1,   148,   402,    -1,    40,
     494,   495,    -1,    51,   487,   483,    -1,    83,   493,    -1,
      46,   468,   382,    -1,    24,   468,   278,   635,   279,   469,
      -1,   484,    -1,   483,   484,    -1,   164,   468,   481,   469,
      -1,     4,    33,   482,    -1,     4,   339,    -1,    52,    46,
      -1,    52,    33,    -1,   182,   633,    -1,   216,   380,   504,
      -1,   486,    -1,   485,   277,   486,    -1,    52,   633,   492,
      -1,    52,    33,   703,    -1,     4,   372,    -1,     4,   387,
      -1,   489,   633,   143,   671,    -1,   489,   487,   182,   633,
      -1,   489,   491,   216,   490,   469,    -1,   488,    -1,   722,
      -1,   215,    -1,   216,    -1,   217,    -1,   218,    -1,   219,
      -1,   220,    -1,   221,    -1,   222,    -1,   223,    -1,   224,
      -1,   225,    -1,   226,    -1,   227,    -1,   228,    -1,   229,
      -1,   230,    -1,   233,    -1,   234,    -1,   241,    -1,   242,
      -1,   250,    -1,   252,    -1,   264,    -1,   265,    -1,   266,
      -1,   267,    -1,     7,    -1,     7,   215,    -1,   504,   468,
      -1,   633,   466,    -1,   633,    -1,   213,    -1,   211,    -1,
      -1,   710,     3,    -1,   710,    80,    -1,    -1,   496,    -1,
     495,   496,    -1,     4,   330,    -1,    52,   103,    -1,   164,
     497,    -1,     4,   352,    -1,     4,   263,    94,   667,    -1,
      52,   263,    94,    -1,    17,   262,    -1,    54,   262,    -1,
     351,    -1,   497,   277,   351,    -1,   717,   473,   499,   477,
     468,   500,   469,    -1,   474,    -1,    -1,   478,    -1,    -1,
      52,   502,    -1,    58,   706,    -1,    83,   710,    -1,   148,
     713,    -1,   180,   716,    -1,   184,   717,    -1,   195,   720,
      -1,    63,   707,    -1,    51,   705,    -1,    62,    68,   698,
      -1,   165,   671,    -1,   214,   714,    -1,    71,   709,    -1,
     504,    -1,   505,    -1,   508,    -1,   511,    -1,   509,   281,
     506,   282,    -1,   516,   281,   506,   282,   514,    -1,   507,
      -1,   506,   277,   507,    -1,   673,    -1,   673,   280,   673,
      -1,   509,    -1,   516,   514,    -1,   515,    -1,   520,    -1,
     523,    -1,   243,    -1,   510,    -1,   169,    -1,    41,    -1,
     226,    -1,   227,    -1,    85,    -1,    96,    -1,    19,   513,
     512,   514,    -1,    19,   278,   672,   279,    -1,    19,   278,
     672,   277,   668,   279,    -1,    19,   278,   277,   668,   279,
      -1,   162,   168,   672,    -1,    -1,   177,   668,    -1,   177,
     699,    -1,    -1,    23,   164,   700,    -1,    -1,   519,   278,
     671,   279,    -1,   519,    -1,   519,   193,   278,   671,   279,
      -1,   518,   278,   671,   279,    -1,   518,    -1,   517,   278,
     671,   279,    -1,   191,    -1,    23,   193,    -1,    91,   193,
      -1,    23,    -1,    91,    -1,   121,    -1,   119,    23,    -1,
     119,    91,    -1,    99,   521,    -1,   522,   521,    -1,    -1,
     278,   673,   279,    -1,   278,   673,   277,   673,   279,    -1,
      44,    -1,    92,    -1,    95,   524,    -1,    97,    95,   524,
      -1,   152,    -1,    93,   145,    -1,   278,   669,   279,    -1,
      -1,   538,    -1,   526,    -1,   555,    -1,   164,    71,   709,
     182,   673,    -1,   164,    71,   709,   182,   231,    -1,   164,
      71,   709,   182,   273,   231,    -1,   528,    -1,   529,    -1,
     531,    -1,   252,   721,    -1,   253,   252,   721,   530,    -1,
     130,    -1,    -1,   161,   535,   182,   532,   721,    -1,   252,
      -1,    -1,    28,   535,   536,    -1,   161,   535,    -1,   201,
      -1,    -1,   155,   537,    -1,    -1,   170,    -1,    -1,   164,
     183,   539,    -1,   540,    -1,    -1,   541,    -1,   540,   541,
      -1,   542,    -1,   543,    -1,   544,    -1,   548,    -1,   151,
     130,    -1,   151,   202,    -1,   196,    -1,   123,   196,    -1,
      88,   107,   545,    -1,   545,    -1,   546,    -1,   151,   185,
     547,    -1,   151,    29,   547,    -1,   170,    -1,   170,   180,
      -1,   170,   180,   174,    -1,   194,    -1,   123,   194,    -1,
      -1,   154,   551,    -1,   166,    -1,   149,    -1,    -1,   151,
      -1,   202,    -1,   552,    -1,   551,   277,   552,    -1,   554,
     553,    -1,    64,   549,   550,    -1,    -1,   593,    -1,   554,
     277,   593,    -1,   164,   176,    83,   710,    -1,   557,   558,
     564,   565,   567,    -1,   568,    -1,   557,   186,   568,    -1,
     557,   186,     6,   568,    -1,   133,    20,   559,    -1,    -1,
     560,    -1,   559,   277,   560,    -1,   656,   561,   563,    -1,
      11,    -1,    48,    -1,    -1,   239,    -1,   249,    -1,   248,
     572,   562,   573,    -1,    -1,   267,   656,    -1,   267,   656,
     182,   656,    -1,    -1,    64,   188,   566,    -1,    -1,   128,
     628,    -1,    -1,   200,   251,    -1,    -1,   163,   574,   577,
     578,   582,   599,   595,   598,   600,    -1,   163,   574,   577,
     578,   582,   599,   595,   598,   600,   558,   564,    -1,    -1,
      -1,    -1,    -1,   575,   576,   571,    -1,   575,   571,    -1,
     576,    -1,    -1,   239,   674,   570,    -1,   239,   278,   656,
     279,   570,    -1,   239,   663,   570,    -1,   240,   674,    -1,
     240,   278,   571,   656,   279,    -1,   240,   663,    -1,    49,
      -1,   696,    -1,   579,    -1,   274,    -1,   580,    -1,   579,
     277,   580,    -1,   619,    -1,   619,   581,   711,    -1,    10,
      -1,    -1,    66,   583,    -1,   584,    -1,   583,   277,   584,
      -1,   589,    -1,   590,    -1,   585,    -1,   278,   556,   279,
     581,   586,   587,    -1,   715,    -1,    -1,   278,   588,   279,
      -1,    -1,   711,    -1,   588,   277,   711,    -1,   584,   594,
      89,   584,   129,   635,    -1,   278,   589,   279,    -1,   713,
     591,   581,   715,    -1,   713,   591,    -1,   278,   620,   279,
      -1,    -1,   593,    -1,   716,   715,    -1,   716,    -1,    81,
      -1,   105,    -1,   105,   134,    -1,   159,    -1,   159,   134,
      -1,    67,    -1,    67,   134,    -1,    -1,    74,    20,   596,
      -1,    -1,   597,    -1,   596,   277,   597,    -1,   656,    -1,
      77,   635,    -1,    -1,   198,   635,    -1,    -1,   142,   601,
      -1,    -1,   602,   278,   603,   279,    -1,    89,    -1,   172,
     114,    -1,   114,    -1,   172,    -1,    -1,   604,    -1,   604,
     277,   603,    -1,   605,   606,    -1,   601,    -1,   716,    -1,
     716,   605,    -1,   120,    -1,    83,   278,   607,   279,    -1,
     133,   710,   608,    -1,   710,    -1,   710,   277,   607,    -1,
      83,   278,   607,   279,    -1,    -1,    84,    86,   593,   629,
     190,   278,   620,   279,    -1,    84,    86,   593,   629,   569,
      -1,   611,    -1,   612,    -1,    47,    66,   592,   599,    -1,
      47,    66,   592,   613,    -1,   198,    38,   128,   704,    -1,
     615,    -1,   616,    -1,   188,   592,   164,   617,   599,    -1,
     188,   592,   164,   617,   613,    -1,   618,    -1,   617,   277,
     618,    -1,   634,   269,   619,    -1,   656,    -1,   697,    -1,
     619,    -1,   620,   277,   619,    -1,   151,    19,   633,    66,
     593,   622,   624,    -1,    84,    19,   633,    86,   593,   622,
     624,    -1,    63,    66,   623,   182,   623,    -1,    63,   182,
     623,    -1,    -1,   513,    -1,   663,    -1,   668,    -1,   113,
     625,    -1,    -1,   672,    -1,   663,    -1,   627,    -1,    -1,
     278,   628,   279,    -1,   633,    -1,   628,   277,   633,    -1,
     630,    -1,    -1,   278,   631,   279,    -1,   634,    -1,   631,
     277,   634,    -1,   633,    -1,   715,   283,   702,    -1,   715,
     283,   274,    -1,   702,    -1,   633,    -1,   715,   283,   702,
      -1,   650,    -1,   124,   650,    -1,   637,    -1,   635,   132,
     635,    -1,   635,     8,   635,    -1,   637,    -1,   124,   650,
      -1,   636,   132,   635,    -1,   636,     8,   635,    -1,   651,
     132,   635,    -1,   651,     8,   635,    -1,   638,    -1,   278,
     636,   279,    -1,   124,   637,    -1,   639,    -1,   642,    -1,
     643,    -1,   644,    -1,   649,    -1,   640,    -1,   647,    -1,
     645,    -1,   646,    -1,   648,    -1,   656,   269,   656,    -1,
     656,   270,   656,    -1,   656,   271,   656,    -1,   656,    70,
     656,    -1,   656,   106,   656,    -1,   656,   125,   656,    -1,
     656,   126,   656,    -1,   656,   122,   656,    -1,   656,   269,
       6,   278,   654,   279,    -1,   656,   270,     6,   278,   654,
     279,    -1,   656,   271,     6,   278,   654,   279,    -1,   656,
      70,     6,   278,   654,   279,    -1,   656,   106,     6,   278,
     654,   279,    -1,   656,   125,     6,   278,   654,   279,    -1,
     656,   126,     6,   278,   654,   279,    -1,   656,   122,     6,
     278,   654,   279,    -1,   656,   269,   641,   278,   654,   279,
      -1,   656,   270,   641,   278,   654,   279,    -1,   656,   271,
     641,   278,   654,   279,    -1,   656,    70,   641,   278,   654,
     279,    -1,   656,   106,   641,   278,   654,   279,    -1,   656,
     125,   641,   278,   654,   279,    -1,   656,   126,   641,   278,
     654,   279,    -1,   656,   122,   641,   278,   654,   279,    -1,
     171,    -1,     9,    -1,   656,    18,   656,     8,   656,    -1,
     656,   124,    18,   656,     8,   656,    -1,   656,   108,   656,
      -1,   656,   124,   108,   656,    -1,   656,   108,   656,    57,
     656,    -1,   656,   124,   108,   656,    57,   656,    -1,   656,
      79,   652,    -1,   656,   124,    79,   652,    -1,   656,    34,
     656,    -1,   656,   124,    34,   656,    -1,   656,   175,   656,
      -1,   656,   124,   175,   656,    -1,   656,   175,   200,   656,
      -1,   656,   124,   175,   200,   656,    -1,    60,   278,   569,
     279,    -1,   167,   278,   569,   279,    -1,   656,    87,    98,
      -1,   656,    87,   124,    98,    -1,   256,    -1,   257,    -1,
     258,    -1,   259,    -1,   260,    -1,   261,    -1,   653,    -1,
     278,   659,   279,    -1,   278,   654,   279,    -1,   163,   574,
     577,   656,   582,   599,   595,   598,   600,   558,   564,    -1,
     163,   574,   577,   656,   582,   599,   595,   598,   600,   558,
     564,    -1,   632,    -1,   658,    -1,   675,    -1,   662,    -1,
     663,    -1,   435,    -1,   684,    -1,   685,    -1,   683,    -1,
     273,   656,    -1,   272,   656,    -1,   656,   272,   656,    -1,
     656,    31,   656,    -1,   656,    26,   701,    -1,   656,   273,
     656,    -1,   656,   274,   656,    -1,   656,   275,   656,    -1,
     278,   656,   279,    -1,   278,   655,   279,    -1,   664,    -1,
     665,    -1,   666,    -1,    42,    -1,   715,   283,    42,    -1,
     101,    -1,   657,    -1,   228,    -1,   229,    -1,   230,    -1,
     632,   281,   659,   282,    -1,   656,    -1,   659,   277,   656,
      -1,   662,    -1,   273,   661,    -1,   205,    -1,   204,    -1,
     203,    -1,   231,    -1,   232,    -1,   661,    -1,   667,    -1,
      41,   207,    -1,   226,   207,    -1,   227,   207,    -1,   284,
      -1,   189,    -1,   233,    -1,   234,    -1,   241,    -1,   242,
      -1,    69,    -1,   173,    -1,   250,    -1,   207,    -1,   208,
     207,    -1,   669,    -1,   273,   670,    -1,   204,    -1,   204,
      -1,   669,    -1,   204,    -1,   674,    -1,   273,   674,    -1,
     204,    -1,   676,    -1,   677,    -1,   678,    -1,   680,    -1,
      35,   278,   274,   279,    -1,    35,   278,   696,   656,   279,
      -1,    35,   278,    49,   656,   279,    -1,   179,   278,   696,
     656,   279,    -1,   179,   278,    49,   656,   279,    -1,    13,
     278,   696,   656,   279,    -1,    13,   278,    49,   656,   279,
      -1,   116,   278,   696,   656,   279,    -1,   116,   278,    49,
     656,   279,    -1,   112,   278,   696,   656,   279,    -1,   112,
     278,    49,   656,   279,    -1,    72,   278,   709,   277,   656,
     279,    -1,   679,    -1,   217,   278,   695,    66,   656,   279,
      -1,   681,    -1,   100,   278,   656,   279,    -1,   236,   278,
     656,    66,   656,   682,   279,    -1,    64,   656,    -1,    -1,
     698,   278,   659,   279,    -1,   698,   278,   279,    -1,    22,
     278,   619,    10,   379,   279,    -1,   686,    -1,   687,    -1,
     245,   278,   656,   277,   656,   279,    -1,   246,   278,   619,
     277,   620,   279,    -1,   688,    -1,   690,    -1,   244,   693,
     689,    54,    -1,   244,   693,   689,    53,   694,    54,    -1,
     197,   692,   181,   694,    -1,   689,   197,   692,   181,   694,
      -1,   244,   691,    54,    -1,   244,   691,    53,   694,    54,
      -1,   197,   635,   181,   694,    -1,   691,   197,   635,   181,
     694,    -1,   656,    -1,   656,    -1,   619,    -1,   218,    -1,
     219,    -1,   220,    -1,   221,    -1,   222,    -1,   223,    -1,
     224,    -1,   225,    -1,     6,    -1,    -1,    98,    -1,   206,
      -1,   722,    -1,   722,    -1,   722,    -1,   722,    -1,   722,
      -1,   722,    -1,   722,    -1,   722,    -1,   722,    -1,   722,
      -1,   722,    -1,   722,    -1,   722,    -1,   722,    -1,   722,
      -1,   722,    -1,   722,    -1,   722,    -1,   722,    -1,   722,
      -1,   722,    -1,   722,    -1,   722,    -1,   206,    -1,   723,
      -1,   235,    -1,   238,    -1,   236,    -1,   246,    -1,   249,
      -1,   255,    -1,   251,    -1,   245,    -1,   248,    -1,   254,
      -1,   247,    -1,   256,    -1,   257,    -1,   258,    -1,   268,
      -1,   262,    -1,   263,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   536,   536,   538,   542,   543,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   569,   573,   577,   581,   585,   590,
     591,   595,   597,   599,   603,   604,   608,   612,   614,   616,
     618,   620,   624,   626,   630,   632,   636,   643,   648,   653,
     658,   663,   668,   673,   679,   683,   684,   686,   688,   692,
     694,   696,   698,   702,   703,   710,   712,   714,   718,   719,
     723,   727,   728,   732,   734,   741,   745,   747,   752,   758,
     759,   761,   767,   769,   770,   774,   775,   781,   784,   790,
     791,   794,   797,   801,   805,   808,   813,   822,   826,   829,
     832,   834,   836,   838,   840,   842,   844,   846,   848,   853,
     857,   859,   861,   871,   875,   877,   888,   890,   894,   896,
     897,   903,   909,   911,   913,   917,   919,   923,   925,   929,
     931,   934,   935,   942,   960,   962,   966,   968,   972,   974,
     977,   978,   981,   986,   987,   990,   994,  1003,  1011,  1020,
    1025,  1026,  1029,  1035,  1037,  1040,  1041,  1045,  1047,  1049,
    1051,  1053,  1057,  1059,  1062,  1063,  1067,  1069,  1070,  1071,
    1073,  1077,  1079,  1081,  1083,  1087,  1092,  1099,  1105,  1112,
    1119,  1152,  1153,  1156,  1161,  1165,  1166,  1169,  1177,  1181,
    1182,  1185,  1186,  1189,  1191,  1195,  1196,  1197,  1200,  1201,
    1202,  1208,  1213,  1218,  1220,  1222,  1226,  1227,  1231,  1232,
    1239,  1243,  1247,  1256,  1262,  1263,  1266,  1268,  1273,  1275,
    1280,  1286,  1292,  1296,  1302,  1304,  1308,  1313,  1314,  1315,
    1317,  1319,  1323,  1325,  1328,  1329,  1333,  1338,  1340,  1345,
    1346,  1348,  1356,  1360,  1362,  1365,  1366,  1367,  1368,  1371,
    1375,  1379,  1386,  1393,  1398,  1404,  1407,  1409,  1411,  1413,
    1417,  1420,  1424,  1427,  1430,  1433,  1442,  1453,  1463,  1473,
    1483,  1485,  1489,  1491,  1495,  1496,  1500,  1506,  1507,  1511,
    1516,  1518,  1520,  1524,  1526,  1530,  1531,  1535,  1539,  1540,
    1543,  1548,  1550,  1554,  1556,  1560,  1565,  1566,  1569,  1573,
    1575,  1577,  1581,  1582,  1586,  1587,  1588,  1589,  1590,  1591,
    1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,
    1603,  1605,  1606,  1609,  1611,  1615,  1619,  1624,  1628,  1628,
    1630,  1635,  1639,  1643,  1645,  1649,  1653,  1659,  1664,  1669,
    1671,  1673,  1677,  1679,  1681,  1685,  1686,  1687,  1689,  1693,
    1697,  1699,  1703,  1705,  1707,  1712,  1714,  1718,  1719,  1723,
    1728,  1729,  1733,  1735,  1737,  1739,  1743,  1744,  1745,  1748,
    1752,  1756,  1760,  1802,  1809,  1817,  1819,  1823,  1824,  1828,
    1834,  1841,  1861,  1865,  1867,  1869,  1873,  1884,  1888,  1889,
    1893,  1894,  1902,  1905,  1911,  1915,  1921,  1925,  1931,  1935,
    1943,  1954,  1965,  1967,  1969,  1973,  1977,  1979,  1983,  1985,
    1987,  1989,  1991,  1993,  1995,  1997,  1999,  2001,  2003,  2005,
    2007,  2009,  2011,  2015,  2017,  2021,  2027,  2031,  2034,  2041,
    2043,  2045,  2048,  2051,  2056,  2060,  2066,  2067,  2071,  2080,
    2084,  2086,  2088,  2090,  2092,  2096,  2097,  2101,  2103,  2105,
    2107,  2113,  2116,  2118,  2122,  2130,  2131,  2132,  2133,  2134,
    2135,  2136,  2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,
    2145,  2146,  2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,
    2155,  2156,  2159,  2161,  2165,  2167,  2172,  2178,  2180,  2182,
    2186,  2188,  2195,  2201,  2202,  2206,  2214,  2216,  2218,  2220,
    2222,  2224,  2226,  2230,  2231,  2238,  2248,  2249,  2253,  2254,
    2260,  2264,  2266,  2268,  2270,  2272,  2274,  2276,  2278,  2280,
    2282,  2284,  2286,  2293,  2294,  2297,  2298,  2301,  2306,  2313,
    2314,  2318,  2327,  2333,  2334,  2337,  2338,  2339,  2340,  2357,
    2362,  2367,  2386,  2403,  2410,  2411,  2418,  2422,  2428,  2434,
    2442,  2446,  2452,  2456,  2460,  2466,  2470,  2477,  2483,  2489,
    2497,  2502,  2507,  2514,  2515,  2516,  2519,  2520,  2523,  2524,
    2525,  2532,  2536,  2547,  2553,  2607,  2669,  2670,  2677,  2690,
    2695,  2700,  2707,  2709,  2716,  2717,  2718,  2722,  2727,  2732,
    2743,  2744,  2745,  2748,  2752,  2756,  2758,  2762,  2766,  2767,
    2770,  2774,  2778,  2779,  2782,  2784,  2788,  2789,  2793,  2797,
    2798,  2802,  2803,  2807,  2808,  2809,  2810,  2813,  2815,  2819,
    2821,  2825,  2827,  2830,  2832,  2834,  2838,  2840,  2842,  2846,
    2848,  2850,  2854,  2858,  2860,  2862,  2866,  2868,  2872,  2873,
    2877,  2881,  2883,  2887,  2888,  2893,  2901,  2905,  2907,  2909,
    2913,  2915,  2919,  2920,  2924,  2928,  2930,  2932,  2936,  2938,
    2942,  2944,  2948,  2951,  2959,  2963,  2965,  2969,  2971,  2975,
    2977,  2984,  2996,  3010,  3014,  3018,  3022,  3026,  3028,  3030,
    3032,  3036,  3038,  3040,  3044,  3046,  3048,  3052,  3054,  3058,
    3060,  3064,  3065,  3069,  3070,  3074,  3075,  3080,  3084,  3085,
    3089,  3090,  3091,  3095,  3100,  3101,  3105,  3107,  3111,  3112,
    3116,  3118,  3122,  3125,  3130,  3132,  3136,  3137,  3142,  3147,
    3149,  3151,  3153,  3155,  3157,  3159,  3161,  3168,  3170,  3174,
    3175,  3181,  3184,  3186,  3190,  3192,  3199,  3201,  3205,  3209,
    3211,  3213,  3219,  3221,  3225,  3226,  3230,  3232,  3235,  3236,
    3240,  3242,  3244,  3248,  3249,  3253,  3255,  3262,  3265,  3272,
    3273,  3276,  3280,  3284,  3291,  3292,  3295,  3300,  3305,  3306,
    3310,  3314,  3315,  3318,  3319,  3326,  3328,  3332,  3334,  3336,
    3339,  3340,  3343,  3347,  3349,  3352,  3354,  3360,  3361,  3365,
    3369,  3370,  3375,  3376,  3380,  3384,  3385,  3390,  3391,  3394,
    3399,  3404,  3407,  3414,  3415,  3417,  3418,  3420,  3424,  3425,
    3427,  3429,  3433,  3435,  3439,  3440,  3442,  3446,  3447,  3448,
    3449,  3450,  3451,  3452,  3453,  3454,  3455,  3460,  3462,  3464,
    3466,  3468,  3470,  3472,  3474,  3481,  3483,  3485,  3487,  3489,
    3491,  3493,  3495,  3497,  3499,  3501,  3503,  3505,  3507,  3509,
    3511,  3515,  3516,  3522,  3524,  3529,  3531,  3533,  3535,  3540,
    3542,  3546,  3548,  3552,  3554,  3556,  3558,  3562,  3566,  3570,
    3572,  3576,  3581,  3586,  3593,  3598,  3603,  3612,  3613,  3617,
    3621,  3635,  3653,  3654,  3655,  3656,  3657,  3658,  3659,  3660,
    3661,  3662,  3664,  3666,  3673,  3675,  3677,  3684,  3691,  3698,
    3700,  3702,  3703,  3704,  3705,  3707,  3709,  3713,  3717,  3733,
    3749,  3753,  3757,  3758,  3762,  3763,  3767,  3769,  3771,  3773,
    3775,  3779,  3780,  3782,  3798,  3814,  3818,  3822,  3824,  3828,
    3832,  3835,  3838,  3841,  3844,  3849,  3851,  3856,  3857,  3861,
    3868,  3875,  3882,  3889,  3890,  3894,  3900,  3901,  3902,  3903,
    3906,  3908,  3910,  3913,  3920,  3929,  3936,  3945,  3947,  3949,
    3951,  3957,  3966,  3969,  3973,  3974,  3978,  3987,  3989,  3993,
    3995,  3999,  4005,  4006,  4009,  4013,  4017,  4018,  4021,  4023,
    4027,  4029,  4033,  4035,  4039,  4041,  4045,  4048,  4051,  4054,
    4056,  4058,  4060,  4062,  4064,  4066,  4068,  4072,  4073,  4076,
    4084,  4087,  4090,  4093,  4096,  4099,  4102,  4105,  4108,  4111,
    4114,  4117,  4120,  4123,  4126,  4129,  4132,  4135,  4138,  4141,
    4144,  4147,  4150,  4153,  4158,  4159,  4164,  4166,  4167,  4168,
    4169,  4170,  4171,  4172,  4173,  4174,  4175,  4176,  4177,  4178,
    4180,  4181,  4182
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ACTIVE", "ADD", "AFTER", "ALL", "ALTER", 
  "AND", "ANY", "AS", "ASC", "AT", "AVG", "AUTO", "BASENAME", "BEFORE", 
  "BEGIN", "BETWEEN", "BLOB", "BY", "CACHE", "CAST", "CHARACTER", "CHECK", 
  "CHECK_POINT_LEN", "COLLATE", "COMMA", "COMMIT", "COMMITTED", 
  "COMPUTED", "CONCATENATE", "CONDITIONAL", "CONSTRAINT", "CONTAINING", 
  "COUNT", "CREATE", "CSTRING", "CURRENT", "CURSOR", "DATABASE", "DATE", 
  "DB_KEY", "KW_DEBUG", "DECIMAL", "DECLARE", "DEFAULT", "KW_DELETE", 
  "DESC", "DISTINCT", "DO", "DOMAIN", "DROP", "ELSE", "END", 
  "ENTRY_POINT", "EQL", "ESCAPE", "EXCEPTION", "EXECUTE", "EXISTS", 
  "EXIT", "EXTERNAL", "FILTER", "FOR", "FOREIGN", "FROM", "FULL", 
  "FUNCTION", "GDSCODE", "GEQ", "GENERATOR", "GEN_ID", "GRANT", "GROUP", 
  "GROUP_COMMIT_WAIT", "GTR", "HAVING", "IF", "KW_IN", "INACTIVE", 
  "INNER", "INPUT_TYPE", "INDEX", "INSERT", "INTEGER", "INTO", "IS", 
  "ISOLATION", "JOIN", "KEY", "KW_CHAR", "KW_DEC", "KW_DOUBLE", "KW_FILE", 
  "KW_FLOAT", "KW_INT", "KW_LONG", "KW_NULL", "KW_NUMERIC", "KW_UPPER", 
  "KW_VALUE", "LENGTH", "LOGFILE", "LPAREN", "LEFT", "LEQ", "LEVEL", 
  "LIKE", "LOG_BUF_SIZE", "LSS", "MANUAL", "MAXIMUM", "MAX_SEGMENT", 
  "MERGE", "MESSAGE", "MINIMUM", "MODULE_NAME", "NAMES", "NATIONAL", 
  "NATURAL", "NCHAR", "NEQ", "NO", "NOT", "NOT_GTR", "NOT_LSS", 
  "NUM_LOG_BUFS", "OF", "ON", "ONLY", "OPTION", "OR", "ORDER", "OUTER", 
  "OUTPUT_TYPE", "OVERFLOW", "PAGE", "PAGES", "KW_PAGE_SIZE", "PARAMETER", 
  "PASSWORD", "PLAN", "POSITION", "POST_EVENT", "PRECISION", "PRIMARY", 
  "PRIVILEGES", "PROCEDURE", "PROTECTED", "RAW_PARTITIONS", "READ", 
  "REAL", "REFERENCES", "RESERVING", "RETAIN", "RETURNING_VALUES", 
  "RETURNS", "REVOKE", "RIGHT", "RPAREN", "ROLLBACK", "SEGMENT", "SELECT", 
  "SET", "SHADOW", "KW_SHARED", "SINGULAR", "KW_SIZE", "SMALLINT", 
  "SNAPSHOT", "SOME", "SORT", "SQLCODE", "STABILITY", "STARTING", 
  "STATISTICS", "SUB_TYPE", "SUSPEND", "SUM", "TABLE", "THEN", "TO", 
  "TRANSACTION", "TRIGGER", "UNCOMMITTED", "UNION", "UNIQUE", "UPDATE", 
  "USER", "VALUES", "VARCHAR", "VARIABLE", "VARYING", "VERSION", "VIEW", 
  "WAIT", "WHEN", "WHERE", "WHILE", "WITH", "WORK", "WRITE", 
  "FLOAT_NUMBER", "NUMBER", "NUMERIC", "SYMBOL", "STRING", "INTRODUCER", 
  "ACTION", "ADMIN", "CASCADE", "FREE_IT", "RESTRICT", "ROLE", "COLUMN", 
  "TYPE", "EXTRACT", "YEAR", "MONTH", "DAY", "HOUR", "MINUTE", "SECOND", 
  "WEEKDAY", "YEARDAY", "TIME", "TIMESTAMP", "CURRENT_DATE", 
  "CURRENT_TIME", "CURRENT_TIMESTAMP", "NUMBER64BIT", "SCALEDINT", 
  "CURRENT_USER", "CURRENT_ROLE", "KW_BREAK", "SUBSTRING", "RECREATE", 
  "KW_DESCRIPTOR", "FIRST", "SKIP", "CURRENT_CONNECTION", 
  "CURRENT_TRANSACTION", "BIGINT", "CASE", "NULLIF", "COALESCE", "USING", 
  "NULLS", "LAST", "ROW_COUNT", "LOCK", "SAVEPOINT", "RELEASE", 
  "STATEMENT", "LEAVE", "INSERTING", "UPDATING", "DELETING", 
  "KW_INSERTING", "KW_UPDATING", "KW_DELETING", "BACKUP", "KW_DIFFERENCE", 
  "OPEN", "CLOSE", "FETCH", "ROWS", "BLOCK", "'='", "'<'", "'>'", "'+'", 
  "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", "']'", 
  "'.'", "'?'", "$accept", "top", "statement", "grant", "prot_table_name", 
  "privileges", "privilege_list", "proc_privileges", "privilege", 
  "grant_option", "role_admin_option", "simple_proc_name", "revoke", 
  "rev_grant_option", "grantee_list", "grantee", "user_grantee_list", 
  "user_grantee", "role_name_list", "role_name", "role_grantee_list", 
  "role_grantee", "declare", "declare_clause", "udf_decl_clause", 
  "udf_data_type", "arg_desc_list1", "arg_desc_list", "arg_desc", 
  "return_value1", "return_value", "filter_decl_clause", "create", 
  "create_clause", "recreate", "recreate_clause", "replace", 
  "replace_clause", "unique_opt", "index_definition", "shadow_clause", 
  "manual_auto", "conditional", "first_file_length", "sec_shadow_files", 
  "db_file_list", "domain_clause", "as_opt", "domain_default_opt", 
  "domain_constraint_clause", "domain_constraint_list", 
  "domain_constraint_def", "domain_constraint", "null_constraint", 
  "domain_check_constraint", "generator_clause", "role_clause", 
  "db_clause", "equals", "db_name", "db_initial_desc1", "db_initial_desc", 
  "db_initial_option", "db_rem_desc1", "db_rem_desc", "db_rem_option", 
  "db_log_option", "db_log", "db_rem_log_spec", "db_default_log_spec", 
  "db_file", "logfiles", "logfile_desc", "logfile_name", "logfile_attrs", 
  "logfile_attr", "file1", "file_desc1", "file_desc", "file_clause", 
  "file_clause_noise", "page_noise", "table_clause", "rtable_clause", 
  "external_file", "table_elements", "table_element", "column_def", 
  "def_computed", "computed_by", "data_type_or_domain", "collate_clause", 
  "column_def_name", "simple_column_def_name", "data_type_descriptor", 
  "init_data_type", "default_opt", "default_value", 
  "column_constraint_clause", "column_constraint_list", 
  "column_constraint_def", "column_constraint", 
  "table_constraint_definition", "constraint_name_opt", 
  "table_constraint", "unique_constraint", "primary_constraint", 
  "referential_constraint", "constraint_index_opt", "check_constraint", 
  "referential_trigger_action", "update_rule", "delete_rule", 
  "referential_action", "procedure_clause", "rprocedure_clause", 
  "replace_procedure_clause", "alter_procedure_clause", 
  "input_parameters", "output_parameters", "input_proc_parameters", 
  "input_proc_parameter", "output_proc_parameters", "proc_parameter", 
  "default_par_opt", "local_declaration_list", "local_declarations", 
  "local_declaration", "local_declaration_item", "var_declaration_item", 
  "var_decl_opt", "var_init_opt", "cursor_declaration_item", "proc_block", 
  "full_proc_block", "full_proc_block_body", "proc_statements", 
  "proc_statement", "excp_statement", "raise_statement", "exec_procedure", 
  "exec_sql", "varstate", "for_select", "for_exec_into", "exec_into", 
  "if_then_else", "post_event", "event_argument_opt", "singleton_select", 
  "variable", "proc_inputs", "proc_outputs", "variable_list", "while", 
  "label_opt", "breakleave", "cursor_def", "excp_hndl_statements", 
  "excp_hndl_statement", "errors", "err", "cursor_statement", 
  "open_cursor", "close_cursor", "fetch_cursor", "fetch_opt", 
  "invoke_procedure", "invoke_block", "block_input_params", 
  "block_parameters", "block_parameter", "view_clause", "rview_clause", 
  "union_view", "union_view_expr", "select_view_expr", "from_view_clause", 
  "from_view_list", "view_table", "joined_view_table", "begin_string", 
  "end_string", "begin_trigger", "end_trigger", "check_opt", 
  "def_trigger_clause", "replace_trigger_clause", "trigger_active", 
  "trigger_type", "trigger_type_prefix", "trigger_type_suffix", 
  "trigger_position", "trigger_action", "alter", "alter_clause", 
  "domain_default_opt2", "domain_check_constraint2", "alter_domain_ops", 
  "alter_domain_op", "alter_ops", "alter_op", "alter_column_name", 
  "keyword_or_column", "col_opt", "alter_data_type_or_domain", 
  "alter_col_name", "drop_behaviour", "alter_index_clause", 
  "init_alter_db", "alter_db", "db_alter_clause", "db_log_option_list", 
  "alter_trigger_clause", "new_trigger_type", "new_trigger_action", 
  "drop", "drop_clause", "data_type", "non_array_type", "array_type", 
  "array_spec", "array_range", "simple_type", "non_charset_simple_type", 
  "integer_keyword", "blob_type", "blob_segsize", "blob_subtype", 
  "charset_clause", "national_character_type", "character_type", 
  "varying_keyword", "character_keyword", "national_character_keyword", 
  "numeric_type", "prec_scale", "decimal_keyword", "float_type", 
  "precision_opt", "set", "set_generator", "savepoint", "set_savepoint", 
  "release_savepoint", "release_only_opt", "undo_savepoint", 
  "optional_savepoint", "commit", "rollback", "optional_work", 
  "optional_retain", "opt_snapshot", "set_transaction", "tran_opt_list_m", 
  "tran_opt_list", "tran_opt", "access_mode", "lock_wait", 
  "isolation_mode", "iso_mode", "snap_shot", "version_mode", 
  "tbl_reserve_options", "lock_type", "lock_mode", "restr_list", 
  "restr_option", "table_lock", "table_list", "set_statistics", "select", 
  "union_expr", "order_clause", "order_list", "order_item", 
  "order_direction", "nulls_placement", "nulls_clause", "rows_clause", 
  "for_update_clause", "for_update_list", "lock_clause", "select_expr", 
  "ordered_select_expr", "begin_limit", "end_limit", "begin_first", 
  "end_first", "limit_clause", "first_clause", "skip_clause", 
  "distinct_clause", "select_list", "select_items", "select_item", 
  "as_noise", "from_clause", "from_list", "table_reference", 
  "derived_table", "correlation_name", "derived_column_list", 
  "alias_list", "joined_table", "table_proc", "table_proc_inputs", 
  "table_name", "simple_table_name", "join_type", "group_clause", 
  "group_by_list", "group_by_item", "having_clause", "where_clause", 
  "plan_clause", "plan_expression", "plan_type", "plan_item_list", 
  "plan_item", "table_or_alias_list", "access_type", "index_list", 
  "extra_indices_opt", "insert", "delete", "delete_searched", 
  "delete_positioned", "cursor_clause", "update", "update_searched", 
  "update_positioned", "assignments", "assignment", "rhs", "rhs_list", 
  "blob", "filter_clause", "blob_subtype_value", "segment_clause", 
  "segment_length", "column_parens_opt", "column_parens", "column_list", 
  "ins_column_parens_opt", "ins_column_parens", "ins_column_list", 
  "column_name", "simple_column_name", "update_column_name", 
  "search_condition", "bracable_search_condition", 
  "simple_search_condition", "predicate", "comparison_predicate", 
  "quantified_predicate", "some", "between_predicate", "like_predicate", 
  "in_predicate", "containing_predicate", "starting_predicate", 
  "exists_predicate", "unique_predicate", "null_predicate", 
  "trigger_action_predicate", "special_trigger_action_predicate", 
  "in_predicate_value", "table_subquery", "column_select", 
  "column_singleton", "value", "datetime_value_expression", 
  "array_element", "value_list", "constant", "u_numeric_constant", 
  "u_constant", "parameter", "current_user", "current_role", 
  "internal_info", "sql_string", "signed_short_integer", 
  "nonneg_short_integer", "neg_short_integer", "pos_short_integer", 
  "unsigned_short_integer", "signed_long_integer", "long_integer", 
  "function", "aggregate_function", "generate_value_function", 
  "numeric_value_function", "extract_expression", "string_value_function", 
  "substring_function", "string_length_opt", "udf", "cast_specification", 
  "case_expression", "case_abbreviation", "case_specification", 
  "simple_case", "simple_when_clause", "searched_case", 
  "searched_when_clause", "when_operand", "case_operand", "case_result", 
  "timestamp_part", "all_noise", "null_value", "symbol_UDF_name", 
  "symbol_blob_subtype_name", "symbol_character_set_name", 
  "symbol_collation_name", "symbol_column_name", "symbol_constraint_name", 
  "symbol_cursor_name", "symbol_domain_name", "symbol_exception_name", 
  "symbol_filter_name", "symbol_gdscode_name", "symbol_generator_name", 
  "symbol_index_name", "symbol_item_alias_name", "symbol_label_name", 
  "symbol_procedure_name", "symbol_role_name", "symbol_table_alias_name", 
  "symbol_table_name", "symbol_trigger_name", "symbol_user_name", 
  "symbol_variable_name", "symbol_view_name", "symbol_savepoint_name", 
  "valid_symbol_name", "non_reserved_word", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,    61,
      60,    62,    43,    45,    42,    47,    59,    44,    40,    41,
      58,    91,    93,    46,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   285,   286,   286,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   287,   287,   287,   287,   287,
     287,   287,   287,   287,   288,   288,   288,   288,   288,   289,
     289,   290,   290,   290,   291,   291,   292,   293,   293,   293,
     293,   293,   294,   294,   295,   295,   296,   297,   297,   297,
     297,   297,   297,   297,   298,   299,   299,   299,   299,   300,
     300,   300,   300,   301,   301,   302,   302,   302,   303,   303,
     304,   305,   305,   306,   306,   307,   308,   308,   309,   310,
     310,   310,   311,   311,   311,   312,   312,   313,   313,   314,
     314,   315,   315,   315,   315,   315,   316,   317,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   319,
     320,   320,   320,   321,   322,   322,   323,   323,   324,   324,
     324,   325,   326,   326,   326,   327,   327,   328,   328,   329,
     329,   330,   330,   331,   332,   332,   333,   333,   334,   334,
     335,   335,   336,   337,   337,   338,   339,   340,   341,   342,
     343,   343,   344,   345,   345,   346,   346,   347,   347,   347,
     347,   347,   348,   348,   349,   349,   350,   350,   350,   350,
     350,   351,   351,   351,   351,   352,   352,   353,   353,   354,
     355,   356,   356,   357,   358,   359,   359,   360,   361,   362,
     362,   363,   363,   364,   364,   365,   365,   365,   366,   366,
     366,   367,   368,   369,   369,   369,   370,   370,   371,   371,
     372,   372,   372,   373,   374,   374,   375,   375,   376,   376,
     377,   378,   379,   380,   381,   381,   382,   382,   382,   382,
     382,   382,   383,   383,   384,   384,   385,   386,   386,   386,
     386,   386,   387,   388,   388,   389,   389,   389,   389,   390,
     391,   392,   393,   393,   394,   395,   395,   395,   395,   395,
     396,   397,   398,   398,   398,   398,   399,   400,   401,   402,
     403,   403,   404,   404,   405,   405,   406,   407,   407,   408,
     409,   409,   409,   410,   410,   411,   411,   412,   413,   413,
     414,   415,   415,   416,   416,   417,   418,   418,   419,   420,
     420,   420,   421,   421,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   423,   423,   424,   425,   426,   427,   427,
     428,   429,   430,   431,   431,   432,   433,   434,   435,   436,
     436,   436,   437,   437,   437,   438,   438,   438,   438,   439,
     440,   440,   441,   441,   441,   442,   442,   443,   443,   444,
     445,   445,   446,   446,   446,   446,   447,   447,   447,   448,
     449,   450,   451,   452,   453,   454,   454,   455,   455,   456,
     457,   458,   459,   460,   460,   460,   461,   462,   463,   463,
     464,   464,   465,   465,   466,   467,   468,   469,   470,   470,
     471,   472,   473,   473,   473,   474,   475,   475,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   477,   477,   478,   479,   480,   480,   480,
     480,   480,   480,   480,   481,   482,   483,   483,   484,   484,
     484,   484,   484,   484,   484,   485,   485,   486,   486,   486,
     486,   486,   486,   486,   487,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   489,   489,   490,   490,   491,   492,   492,   492,
     493,   493,   494,   495,   495,   496,   496,   496,   496,   496,
     496,   496,   496,   497,   497,   498,   499,   499,   500,   500,
     501,   502,   502,   502,   502,   502,   502,   502,   502,   502,
     502,   502,   502,   503,   503,   504,   504,   505,   505,   506,
     506,   507,   507,   508,   508,   509,   509,   509,   509,   509,
     509,   509,   509,   509,   510,   510,   511,   511,   511,   511,
     512,   512,   513,   513,   513,   514,   514,   515,   515,   515,
     516,   516,   516,   517,   517,   517,   518,   518,   519,   519,
     519,   520,   520,   521,   521,   521,   522,   522,   523,   523,
     523,   523,   524,   524,   525,   525,   525,   526,   526,   526,
     527,   527,   527,   528,   529,   530,   530,   531,   532,   532,
     533,   534,   535,   535,   536,   536,   537,   537,   538,   539,
     539,   540,   540,   541,   541,   541,   541,   542,   542,   543,
     543,   544,   544,   545,   545,   545,   546,   546,   546,   547,
     547,   547,   548,   549,   549,   549,   550,   550,   551,   551,
     552,   553,   553,   554,   554,   555,   556,   557,   557,   557,
     558,   558,   559,   559,   560,   561,   561,   561,   562,   562,
     563,   563,   564,   564,   564,   565,   565,   566,   566,   567,
     567,   568,   569,   570,   571,   572,   573,   574,   574,   574,
     574,   575,   575,   575,   576,   576,   576,   577,   577,   578,
     578,   579,   579,   580,   580,   581,   581,   582,   583,   583,
     584,   584,   584,   585,   586,   586,   587,   587,   588,   588,
     589,   589,   590,   590,   591,   591,   592,   592,   593,   594,
     594,   594,   594,   594,   594,   594,   594,   595,   595,   596,
     596,   597,   598,   598,   599,   599,   600,   600,   601,   602,
     602,   602,   602,   602,   603,   603,   604,   604,   605,   605,
     606,   606,   606,   607,   607,   608,   608,   609,   609,   610,
     610,   611,   612,   613,   614,   614,   615,   616,   617,   617,
     618,   619,   619,   620,   620,   621,   621,   622,   622,   622,
     623,   623,   513,   624,   624,   625,   625,   626,   626,   627,
     628,   628,   629,   629,   630,   631,   631,   632,   632,   632,
     633,   634,   634,   635,   635,   635,   635,   635,   636,   636,
     636,   636,   636,   636,   637,   637,   637,   638,   638,   638,
     638,   638,   638,   638,   638,   638,   638,   639,   639,   639,
     639,   639,   639,   639,   639,   640,   640,   640,   640,   640,
     640,   640,   640,   640,   640,   640,   640,   640,   640,   640,
     640,   641,   641,   642,   642,   643,   643,   643,   643,   644,
     644,   645,   645,   646,   646,   646,   646,   647,   648,   649,
     649,   650,   650,   650,   651,   651,   651,   652,   652,   653,
     654,   655,   656,   656,   656,   656,   656,   656,   656,   656,
     656,   656,   656,   656,   656,   656,   656,   656,   656,   656,
     656,   656,   656,   656,   656,   656,   656,   656,   657,   657,
     657,   658,   659,   659,   660,   660,   661,   661,   661,   661,
     661,   662,   662,   662,   662,   662,   663,   664,   664,   665,
     666,   666,   666,   666,   666,   667,   667,   668,   668,   669,
     670,   671,   672,   673,   673,   674,   675,   675,   675,   675,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   677,   678,   679,   680,   680,   681,   682,   682,   683,
     683,   684,   685,   685,   686,   686,   687,   687,   688,   688,
     689,   689,   690,   690,   691,   691,   692,   693,   694,   695,
     695,   695,   695,   695,   695,   695,   695,   696,   696,   697,
     698,   699,   700,   701,   702,   703,   704,   705,   706,   707,
     708,   709,   710,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   722,   723,   723,   723,   723,
     723,   723,   723,   723,   723,   723,   723,   723,   723,   723,
     723,   723,   723
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     7,     8,     6,     7,     5,     1,
       2,     1,     2,     1,     1,     3,     1,     1,     1,     1,
       2,     2,     3,     0,     3,     0,     1,     7,     8,     6,
       7,     6,     7,     4,     3,     1,     3,     3,     3,     2,
       2,     2,     2,     1,     3,     1,     2,     2,     1,     3,
       1,     1,     3,     1,     2,     2,     2,     3,     8,     1,
       1,     5,     0,     1,     3,     1,     3,     2,     4,     1,
       3,     2,     3,     4,     4,     2,     9,     2,     3,     7,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     4,     2,     2,     1,     0,     1,     1,
       6,     6,     1,     1,     0,     0,     1,     0,     4,     0,
       1,     1,     2,     8,     1,     0,     3,     0,     0,     1,
       1,     2,     1,     1,     1,     2,     6,     1,     1,     3,
       0,     1,     1,     0,     1,     1,     2,     3,     4,     2,
       2,     3,     0,     1,     1,     2,     1,     1,     1,     4,
       3,     3,     3,     3,     3,     1,     1,     6,     3,     1,
       3,     1,     3,     2,     1,     0,     2,     3,     1,     0,
       1,     1,     2,     3,     4,     0,     1,     2,     0,     1,
       1,     5,     5,     3,     2,     0,     1,     3,     1,     1,
       6,     3,     2,     6,     2,     1,     2,     2,     2,     0,
       1,     1,     2,     0,     2,     0,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     2,     2,     2,     5,     1,
       2,     3,     2,     2,     0,     1,     1,     1,     1,     3,
       4,     8,     4,     0,     6,     1,     1,     2,     2,     0,
       3,     3,     1,     2,     2,     2,     8,     8,     8,     8,
       3,     0,     4,     0,     1,     3,     4,     1,     3,     2,
       3,     3,     1,     1,     0,     1,     2,     4,     1,     1,
       3,     1,     0,     2,     1,     6,     1,     1,     3,     1,
       2,     0,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     2,     1,     1,     1,     2,
       2,     1,     2,     3,     4,     2,     6,     4,     1,     1,
       8,     9,     6,     8,     6,     4,     0,     4,     2,     1,
       3,     0,     2,     4,     0,     1,     1,     3,     3,     7,
       2,     0,     2,     2,     3,     3,     0,     1,     2,     4,
       1,     3,     2,     2,     2,     1,     1,     1,     1,     2,
       2,     5,     0,     4,     7,     3,     0,     1,     3,     3,
       7,     7,     1,     1,     3,     4,     7,     2,     1,     3,
       1,     1,     6,     3,     0,     0,     0,     0,     3,     0,
       9,     9,     1,     1,     0,     2,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     5,     5,     5,
       5,     5,     5,     2,     0,     4,     2,     3,     3,     2,
       2,     3,     3,     2,     3,     6,     1,     2,     4,     3,
       2,     2,     2,     2,     3,     1,     3,     3,     3,     2,
       2,     4,     4,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     0,
       2,     2,     0,     1,     2,     2,     2,     2,     2,     4,
       3,     2,     2,     1,     3,     7,     1,     0,     1,     0,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       2,     2,     2,     1,     1,     1,     1,     4,     5,     1,
       3,     1,     3,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     6,     5,
       3,     0,     2,     2,     0,     3,     0,     4,     1,     5,
       4,     1,     4,     1,     2,     2,     1,     1,     1,     2,
       2,     2,     2,     0,     3,     5,     1,     1,     2,     3,
       1,     2,     3,     0,     1,     1,     1,     5,     5,     6,
       1,     1,     1,     2,     4,     1,     0,     5,     1,     0,
       3,     2,     1,     0,     2,     0,     1,     0,     3,     1,
       0,     1,     2,     1,     1,     1,     1,     2,     2,     1,
       2,     3,     1,     1,     3,     3,     1,     2,     3,     1,
       2,     0,     2,     1,     1,     0,     1,     1,     1,     3,
       2,     3,     0,     1,     3,     4,     5,     1,     3,     4,
       3,     0,     1,     3,     3,     1,     1,     0,     1,     1,
       4,     0,     2,     4,     0,     3,     0,     2,     0,     2,
       0,     9,    11,     0,     0,     0,     0,     3,     2,     1,
       0,     3,     5,     3,     2,     5,     2,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     0,     2,     1,     3,
       1,     1,     1,     6,     1,     0,     3,     0,     1,     3,
       6,     3,     4,     2,     3,     0,     1,     2,     1,     1,
       1,     2,     1,     2,     1,     2,     0,     3,     0,     1,
       3,     1,     2,     0,     2,     0,     2,     0,     4,     1,
       2,     1,     1,     0,     1,     3,     2,     1,     1,     2,
       1,     4,     3,     1,     3,     4,     0,     8,     5,     1,
       1,     4,     4,     4,     1,     1,     5,     5,     1,     3,
       3,     1,     1,     1,     3,     7,     7,     5,     3,     0,
       1,     1,     1,     2,     0,     1,     1,     1,     0,     3,
       1,     3,     1,     0,     3,     1,     3,     1,     3,     3,
       1,     1,     3,     1,     2,     1,     3,     3,     1,     2,
       3,     3,     3,     3,     1,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     1,     1,     5,     6,     3,     4,     5,     6,     3,
       4,     3,     4,     3,     4,     4,     5,     4,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     3,     3,
      11,    11,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     4,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       4,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     6,     1,     6,     1,     4,     7,     2,     0,     4,
       3,     6,     1,     1,     6,     6,     1,     1,     4,     6,
       4,     5,     3,     5,     4,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,   603,   117,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   603,   680,     0,     0,     0,     0,     0,
       0,     2,    11,    17,     8,     7,    15,    16,    13,    14,
       4,    10,    21,   585,    19,   590,   591,   592,     6,    18,
     584,   586,    20,   651,   647,    12,     9,   759,   760,    22,
     764,   765,     5,   492,     0,     0,     0,     0,     0,     0,
     426,   602,   605,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,     0,     0,    97,   657,   939,     0,    23,
     937,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   510,     0,   376,
      31,    39,    36,    38,   788,    37,   788,  1024,  1026,  1028,
    1027,  1033,  1029,  1036,  1034,  1030,  1032,  1035,  1031,  1037,
    1038,  1039,  1041,  1042,  1040,     0,    33,     0,    34,     0,
      68,    70,  1016,  1025,     0,     0,     0,     0,     0,     0,
       0,     0,   601,     0,     0,   998,   674,   679,     0,     0,
     610,     0,   716,   718,  1018,     0,     0,     0,   109,   593,
    1023,     0,     1,     3,     0,     0,   664,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,     0,   454,   455,     0,  1008,   433,
       0,  1012,   430,   271,  1015,     0,   718,   429,   404,  1019,
     607,   600,   935,     0,   105,   153,   152,   106,   135,   220,
     800,  1004,     0,   104,   147,  1011,     0,   100,   271,   107,
     941,   124,   101,   205,   102,     0,   103,   788,  1022,   108,
     148,   655,   656,     0,   940,   938,     0,    76,     0,  1009,
     735,   518,  1007,   511,     0,   517,   522,   512,   513,   520,
     514,   515,   516,   521,   341,     0,   273,    32,     0,    41,
     787,    40,     0,     0,     0,     0,     0,     0,   793,     0,
       0,     0,     0,     0,     0,     0,   599,   945,     0,   926,
     673,   673,   674,   686,   684,   997,   687,     0,   688,   678,
     674,     0,     0,     0,     0,     0,     0,   626,   619,   608,
     609,   611,   613,   614,   615,   622,   623,   616,     0,   717,
    1017,   110,   271,   111,   205,   112,   788,   596,     0,     0,
     648,     0,   666,     0,     0,     0,     0,     0,   431,   493,
     396,     0,   396,     0,   223,   432,   436,   427,   490,   491,
       0,   273,   244,   482,     0,   428,   445,     0,   402,   403,
     507,   606,   604,   936,   150,   150,     0,     0,     0,   162,
     154,   155,   134,     0,    98,     0,     0,   113,   273,   123,
     122,   125,     0,     0,     0,     0,     0,  1000,    77,   223,
     554,     0,   761,   762,   519,     0,     0,     0,     0,   904,
     932,     0,   999,     0,   906,     0,     0,   933,     0,   927,
     918,   917,   916,  1024,     0,     0,     0,   908,   909,   910,
     919,   920,   928,   929,  1028,   930,   931,     0,  1033,  1029,
     934,     0,     0,     0,     0,   887,   373,   773,   339,   882,
     797,   771,   907,   883,   921,   885,   886,   901,   902,   903,
     922,   884,   946,   947,   948,   962,   949,   964,   890,   888,
     889,   972,   973,   976,   977,   772,     0,     0,  1004,     0,
       0,     0,   377,   221,     0,     0,     0,   790,     0,     0,
      29,    35,     0,     0,    45,    71,    73,  1020,    69,     0,
       0,     0,   792,     0,    54,     0,     0,     0,     0,    53,
     598,     0,     0,     0,   683,   681,     0,   690,     0,   689,
     691,   696,   677,     0,   645,     0,   620,   631,   617,   631,
     618,   632,   638,   642,   643,   627,   612,   735,   768,   801,
       0,     0,   273,     0,     0,   595,   594,   650,   652,   657,
     649,   662,     0,   670,   188,   179,     0,   495,   498,   176,
     175,   131,     0,   501,   496,     0,   502,   150,   150,   150,
     150,   503,   497,   494,     0,   440,     0,   442,   441,     0,
     443,     0,   437,     0,     0,   274,     0,     0,   449,     0,
     450,   396,   483,     0,   489,     0,     0,     0,   486,  1004,
     407,   406,   506,     0,   424,   151,     0,     0,   160,     0,
     159,     0,     0,   149,   163,   164,   168,   167,   166,   156,
     554,   566,   541,   576,   544,   567,   577,     0,   583,   545,
       0,   573,     0,   568,   580,   540,   563,   542,   543,   538,
     396,   523,   524,   525,   533,   539,   526,   535,   556,     0,
     561,   558,   536,   573,   537,   114,   271,   115,     0,     0,
     126,     0,     0,   204,   244,   404,   394,     0,   223,     0,
      83,    85,     0,     0,     0,   782,     0,     0,     0,     0,
     871,   872,   873,     0,   734,   805,   814,   817,   822,   818,
     819,   820,   824,   825,   823,   826,   821,   803,     0,   998,
       0,   998,   923,     0,     0,   998,   998,   998,     0,   924,
     925,     0,     0,   987,     0,     0,     0,     0,   892,   891,
     680,     0,     0,   771,   338,  1021,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   279,   533,   556,     0,
       0,   375,     0,   284,     0,   789,    30,     0,     0,    46,
      74,     0,     0,    28,   779,     0,   795,   680,     0,   758,
     779,     0,     0,     0,     0,   597,     0,   673,     0,     0,
     735,     0,   695,     0,   588,     0,   587,   943,     0,   621,
       0,   629,   625,   624,     0,   635,     0,   640,   628,     0,
     766,   767,     0,     0,     0,   244,   394,     0,   661,     0,
     668,     0,   646,     0,     0,     0,   132,   189,   500,     0,
       0,     0,     0,     0,   396,   439,     0,   396,   397,   444,
     394,     0,   270,   394,   243,  1005,   215,   212,     0,   225,
     396,   523,   394,     0,     0,     0,   242,   245,   246,   247,
     248,     0,   448,   488,   487,   447,   446,     0,     0,     0,
     410,   408,   409,   405,     0,   396,   198,   157,   161,     0,
       0,   165,     0,   551,   564,   565,   581,     0,   578,   583,
       0,   571,   569,   570,   137,     0,     0,     0,   534,     0,
       0,     0,     0,   572,   273,     0,   394,   127,   203,     0,
     206,   208,   209,     0,     0,     0,     0,   223,   223,    80,
       0,    87,    79,   552,   553,  1001,   554,     0,     0,     0,
     816,   804,     0,     0,   874,   875,   876,     0,   808,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   989,   990,   991,   992,   993,   994,   995,   996,
       0,     0,     0,     0,   982,     0,     0,     0,     0,     0,
     998,   340,   900,   899,   774,   912,     0,   895,  1003,   894,
     893,   896,   897,   898,   970,     0,   905,   799,   798,   379,
     378,     0,   277,   292,     0,   283,   285,   791,     0,     0,
       0,     0,     0,     0,    26,    55,    43,    63,    65,     0,
       0,    72,     0,   784,     0,   794,   998,     0,   784,    51,
      49,     0,     0,     0,   682,   685,     0,   697,   698,   702,
     700,   701,   715,     0,   728,   692,   694,  1013,   589,   944,
     630,   639,   634,   633,     0,   644,   769,   770,   802,   394,
       0,     0,   653,   675,   654,   663,     0,   665,   669,   178,
     185,   184,     0,   181,   499,   150,   195,   180,   190,   191,
     172,   171,   942,   174,   173,   504,     0,     0,     0,   438,
     394,   394,   397,   282,   275,   284,   214,   396,     0,   397,
     216,   211,   217,     0,     0,   253,     0,   452,   397,   396,
     394,   451,     0,     0,     0,   423,   509,   199,   200,   158,
       0,   170,     0,     0,     0,   556,     0,   579,     0,     0,
     396,   397,     0,   529,   531,     0,     0,     0,     0,     0,
       0,     0,   404,   284,   150,   129,   244,   201,   424,   998,
     399,   382,   383,     0,    84,     0,   223,     0,    89,     0,
      86,     0,     0,     0,   763,  1006,     0,     0,   809,     0,
       0,   815,     0,     0,   807,   806,     0,   861,     0,   852,
     851,     0,   830,     0,   859,   877,   869,     0,     0,     0,
     831,   855,     0,     0,   834,     0,     0,     0,     0,     0,
       0,     0,   832,     0,     0,   833,     0,   863,     0,     0,
     827,     0,     0,   828,     0,     0,   829,     0,     0,   223,
       0,   950,     0,     0,   965,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   988,     0,     0,   986,     0,     0,
     978,     0,     0,     0,     0,     0,   911,   969,     0,   272,
     291,     0,   351,   374,   286,    67,    59,    60,    66,    61,
      62,     0,     0,     0,    24,    27,    43,    44,   554,   554,
       0,   776,   796,     0,     0,   775,    52,    50,    47,     0,
       0,   726,   700,     0,   724,   719,   720,   722,     0,     0,
     713,     0,   733,   636,   637,   641,   284,   202,   399,     0,
     667,   183,     0,     0,     0,   196,     0,   192,     0,     0,
       0,   434,   231,   226,   914,   227,   228,   229,   230,     0,
       0,   276,     0,     0,   224,   232,     0,   253,   657,   249,
       0,   453,   484,   485,   415,   416,   412,   411,   414,   413,
     396,   508,   397,   169,  1002,     0,     0,   547,     0,   546,
     582,     0,   574,     0,   138,     0,   527,     0,   555,   556,
     562,   560,     0,   557,   394,     0,     0,     0,   121,   130,
     207,   396,     0,     0,   395,     0,    99,     0,   119,   118,
      95,     0,     0,    91,     0,    88,     0,   867,   868,   811,
     810,   813,   812,     0,     0,     0,   680,     0,     0,   870,
       0,     0,     0,     0,     0,     0,   862,   860,   856,     0,
     864,     0,     0,     0,     0,   865,     0,     0,     0,     0,
       0,     0,   956,   955,     0,     0,   952,   951,     0,   960,
     959,   958,   957,   954,   953,     0,   968,   984,   983,     0,
       0,     0,     0,     0,     0,     0,   913,   278,     0,     0,
     288,   289,     0,  1004,     0,     0,     0,     0,     0,     0,
       0,  1026,  1031,     0,     0,   372,   302,   297,     0,   351,
     296,   306,   307,   308,   309,   310,   317,   318,   311,   313,
     314,   316,     0,   321,     0,   366,   368,   367,     0,     0,
       0,     0,     0,     0,  1004,    56,    57,     0,    58,    64,
      25,   780,     0,   781,   778,   783,   786,   785,     0,   757,
       0,    48,   696,   711,   699,   725,   721,   723,     0,     0,
       0,     0,     0,   737,     0,   395,   658,   659,   676,   150,
     186,   182,     0,   198,   197,   193,     0,   397,   915,   280,
     281,   397,   397,   219,   233,   234,   396,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,   284,   505,   549,
       0,   550,     0,   136,     0,   219,   139,   140,   142,   143,
     144,   530,   532,   528,   559,   284,   424,   397,   198,     0,
       0,     0,   380,     0,   384,   396,    90,     0,     0,    92,
     556,     0,   853,     0,     0,   998,   879,   878,     0,     0,
     857,     0,     0,     0,     0,   866,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   971,   222,   961,   963,
       0,     0,   985,   980,   979,     0,   974,   975,   735,   225,
     287,     0,   325,     0,     0,   328,   329,     0,   320,     0,
     336,   319,   352,   353,     0,  1014,   369,   370,     0,   298,
       0,   303,   300,   357,     0,     0,   322,     0,   312,   305,
     315,   304,   350,    42,   554,   735,   705,   726,   714,   712,
     727,   729,   731,   732,   743,   671,   397,   381,   660,     0,
     177,   194,   397,   146,   269,     0,     0,   210,   235,     0,
       0,     0,   253,   236,   239,   788,     0,   397,   421,   422,
     418,   417,   420,   419,     0,   548,   575,   145,   133,   141,
       0,   396,   266,   128,   397,     0,   735,   398,   385,     0,
       0,    93,    94,    81,     0,   838,   846,     0,   839,   847,
     842,   850,   854,   858,   840,   848,   841,   849,   835,   843,
     836,   844,   837,   845,   967,   966,   981,   728,     0,   294,
     290,     0,   323,     0,   341,     0,     0,     0,   354,     0,
     365,     0,     0,     0,     0,   360,   358,     0,     0,     0,
     345,     0,   346,     0,   777,   728,   707,   704,     0,     0,
     739,   741,   742,   736,     0,   267,   187,   435,   213,   218,
     237,   253,   788,   240,   259,   252,   254,   425,   397,     0,
     400,     0,   387,   388,   390,   391,   728,   397,    78,    96,
       0,   733,   293,     0,   324,   344,     0,   327,     0,   335,
       0,   364,   363,  1010,   362,   351,     0,     0,     0,     0,
     337,     0,     0,   733,     0,   703,   710,   730,   740,   743,
     241,   259,     0,   253,   255,   256,   268,   397,   726,   390,
       0,     0,   733,     0,   735,   737,     0,     0,     0,     0,
     351,   371,   359,   361,     0,   356,     0,   348,   347,   737,
       0,   708,   747,     0,   744,     0,   748,   253,     0,     0,
     251,     0,   258,     0,   257,   401,   393,   389,     0,   386,
     120,   728,   651,   295,     0,   342,   326,   332,   334,     0,
       0,     0,   351,   651,     0,   706,   738,   743,     0,   750,
       0,   746,   749,   238,     0,     0,   262,   261,   260,   726,
     733,   664,     0,   351,     0,     0,   351,   349,   664,   709,
     745,     0,   756,   265,   263,   264,     0,   737,   881,   343,
     333,   351,   355,   330,   672,     0,   753,     0,   752,   392,
     651,   331,   751,     0,     0,   664,   754,     0,   880,   755
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    20,    21,    22,   479,   125,   126,   127,   128,  1244,
     743,   738,    23,   140,   994,   995,   996,   997,   129,   130,
     484,   485,    24,    83,   388,   891,   659,   660,   661,  1137,
    1138,   247,    25,    75,    26,   158,    27,   377,    76,  1356,
     229,   381,   651,  1125,  1348,   547,   217,   373,  1111,  1545,
    1546,  1547,  1548,  1549,  1550,   223,   239,   214,   596,   215,
     369,   370,   371,   603,   604,   605,   606,   607,   549,   550,
     551,  1052,  1049,  1050,  1281,  1510,   552,  1057,  1058,  1059,
    1286,  1099,   232,   323,   383,   879,   880,   881,   817,   818,
     819,  1667,   579,   469,  1404,   662,  1079,  1291,  1523,  1524,
    1525,  1673,   882,   581,   826,   827,   828,   829,  1309,   830,
    1823,  1824,  1825,  1897,   227,   321,   645,   202,   351,   475,
     574,   575,   981,   470,  1072,   984,   985,   986,  1429,  1430,
    1231,  1730,  1431,  1446,  1447,  1448,  1449,  1450,  1451,  1452,
    1453,  1454,  1617,  1455,  1456,  1457,  1458,  1459,  1737,  1460,
     435,   436,  1838,  1751,  1461,  1462,  1463,  1881,  1632,  1633,
    1744,  1745,  1464,  1465,  1466,  1467,  1628,    28,    29,   266,
     471,   472,   236,   325,  1130,  1131,  1132,  1696,  1782,  1783,
    1784,   884,  1562,   566,  1069,  1354,   234,   647,   360,   592,
     593,   843,   845,  1321,    30,    60,   808,   805,   345,   346,
     355,   356,   194,   195,   357,  1088,   587,   835,   199,   167,
     338,   339,   562,   207,   594,  1322,    31,    97,   630,   631,
     632,  1112,  1113,   633,   727,   635,   636,  1105,  1481,   868,
     637,   728,   639,   640,   641,   642,   861,   643,   644,   858,
      32,    33,    34,    35,    36,   536,    37,   501,    38,    39,
      62,   211,   362,    40,   309,   310,   311,   312,   313,   314,
     315,   316,   772,   317,  1034,  1275,   521,   522,   777,   523,
      41,  1468,    43,   166,   537,   538,   243,  1508,  1044,   332,
     543,  1047,   792,    44,   749,   504,   299,  1279,  1658,   145,
     146,   147,   297,   508,   509,   510,   763,   760,  1017,  1018,
    1019,  1756,  1815,  1850,  1020,  1021,  1270,  1785,   152,  1268,
    1272,  1650,  1651,  1503,   392,  1655,  1852,  1764,  1853,  1854,
    1855,  1891,  1925,  1928,  1469,  1470,    47,    48,   393,  1471,
      50,    51,   527,  1472,   437,   438,    52,  1003,  1482,  1251,
    1485,   269,   270,   476,   491,   492,   745,   439,   440,   530,
     674,   907,   675,   676,   677,   678,  1161,   679,   680,   681,
     682,   683,   684,   685,   686,   687,   909,  1164,  1165,  1377,
     712,   688,   442,   443,   966,  1293,   444,   445,   446,   447,
     448,   449,   450,   665,    80,   245,   231,  1063,  1114,   767,
     451,   452,   453,   454,   455,   456,   457,  1601,   458,   459,
     460,   461,   462,   463,   957,   464,   704,  1218,   705,  1215,
     950,   298,   465,   466,   894,  1323,   967,   220,   814,  1144,
     251,   197,   248,  1802,   224,  1926,  1026,  1473,  1022,   131,
     467,   206,   208,   998,   714,   237,   159,   468,   133
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1578
static const short yypact[] =
{
    3170,   807,    53,  1339,   451,   926,   447,  1494,   305,  2101,
     590,   249,  1921,    53,   613,   163,  5741,   797,  5741,   279,
     517,   259, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578,   117, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,  6911,  5741,  5741,  5741,  5741,  5741,
   -1578, -1578,   410,   828,  5741,  5741,  5741,   633,  5741,   466,
    5741,  5741, -1578,  5741,  5741, -1578,   241, -1578,   481, -1578,
   -1578,   669,  5741, -1578,  5741,  5741,  5741,   803,  5741,  5741,
    5741,  5741,   466,  5741,  5741,  5741,  5741, -1578,  5741,   514,
     742, -1578, -1578, -1578,   634, -1578,   634, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578,   808,   695,   869, -1578,    63,
   -1578, -1578, -1578, -1578,  5741,  5741,  5741,   832,   875,   882,
    1167,   118,   846,   421,   468,   712,   826, -1578,  5741,   964,
    1107,   914, -1578,  5741, -1578,  5741,  5741,  5741, -1578, -1578,
   -1578,  5741, -1578, -1578,  1039,   215,   822,   185, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,   459, -1578, -1578,   828, -1578, -1578,
     491, -1578, -1578,   837, -1578,   242, -1578, -1578,   580, -1578,
     916, -1578, -1578,   915, -1578,   992, -1578, -1578,  1106, -1578,
   -1578, -1578,   828, -1578, -1578, -1578,   495, -1578,   837, -1578,
   -1578,   484, -1578,  1063, -1578,  1062, -1578,   634, -1578, -1578,
   -1578, -1578, -1578,  1049, -1578, -1578,   930, -1578,  1055, -1578,
     945, -1578, -1578, -1578,   930, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,  4858,  5741,   993, -1578,  5741, -1578,
   -1578, -1578,  3762,   469,  1005,  4009,  5741,  1090,   919,  1121,
    1128,  3762,  1054,  1074,  1083,  4009,   965, -1578,  6276, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578,  4514, -1578, -1578,
   -1578,  1034,  5741,  1114,  1024,   341,  5741,  1033, -1578, -1578,
    1107, -1578, -1578, -1578, -1578, -1578, -1578, -1578,  5741, -1578,
   -1578, -1578,   837, -1578,  1063, -1578,   634,  1094,  6276,  1065,
   -1578,  6276,  1161,   226,   973,    94,   979,   704,   185, -1578,
    1203,   750, -1578,  5741, -1578,   459, -1578, -1578, -1578, -1578,
    5741,   993,  2066,  1030,  2297,   997, -1578,  6911, -1578, -1578,
     253, -1578, -1578, -1578,  1006,  1006,   828,  1175,   828,  1234,
     992, -1578, -1578,  6882, -1578,  5741,  5741, -1578,   993, -1578,
   -1578,  1262,   382,  1018,  5741,  1291,  5741, -1578, -1578,    87,
     482,  3848, -1578, -1578, -1578,  1027,  1035,  1041,  1095, -1578,
   -1578,  1043, -1578,  1044, -1578,  1045,  1046, -1578,  1051, -1578,
   -1578, -1578, -1578,  1053,  1056,  1125,  1126, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,  1058, -1578, -1578,  4944,  1060,  1068,
   -1578,  6276,  6276,  4611,  5741, -1578, -1578, -1578,  1072,  1059,
   -1578,   998, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578,  1079,  1073,  1075,  6882,
    1091,   790, -1578, -1578,  1081,  1352,   800, -1578,  5741,  1181,
   -1578, -1578,  5741,  5741,   285, -1578, -1578, -1578, -1578,  5741,
    5741,   764, -1578,  5741, -1578,  1301,  5741,  3762,  1226,  1100,
   -1578,  5741,  5191,   231, -1578, -1578,  6276, -1578,  1314,  1104,
   -1578,   154, -1578,   610, -1578,   331, -1578,   504, -1578,   504,
   -1578,  1105, -1578,   129, -1578,  1210, -1578,   406, -1578, -1578,
    1116,  1103,   993,  1109,  1378, -1578, -1578,  1112, -1578,   193,
   -1578,   905,  1204,  1191, -1578,   134,  1299,  1300, -1578, -1578,
   -1578, -1578,   828, -1578, -1578,  1302, -1578,  1006,  1006,  1006,
    1006, -1578,  1118, -1578,  1374, -1578,  1375, -1578, -1578,  1361,
   -1578,  6882, -1578,  6882,   819, -1578,  1398,  5741, -1578,  2600,
   -1578,   405, -1578,  5741,   892,   242,  1227,  1199,  1273,  1235,
   -1578, -1578, -1578,   159,  1276, -1578,  1216,   466, -1578,   828,
   -1578,  1399,  1327, -1578,  1234, -1578, -1578, -1578, -1578, -1578,
     417,  1230, -1578, -1578, -1578,  1231, -1578,  1281,  1149, -1578,
    1333,  1151,   596, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,  1150, -1578, -1578, -1578,   164,  1152,
    1154,   337, -1578,  1151, -1578, -1578,   837, -1578,  1372,  1427,
   -1578,   828,   828, -1578,  2066,   580, -1578,  1309, -1578,  1282,
    1164, -1578,  6844,  5408,  1315, -1578,  1321,  1174,  3934,  1177,
    1131,  1173,  1442,  3596,   224, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,  2121,   781,
    5277,   139, -1578,  5741,  6276,   811,   864,   946,  1317, -1578,
   -1578,  6276,  4181,   998,   162,  1254,  6276,  5277,   216,   216,
     613,   831,  1179,   275, -1578, -1578,  5277,  6276,  5741,  6276,
    6276,  6276,  6276,  6276,  5524,  2681, -1578, -1578,  1430,  1178,
    5741, -1578,  5741,  1414,  5741, -1578, -1578,  4393,  1283, -1578,
   -1578,  1253,  4009, -1578,  1401,   834, -1578,   613,  1188, -1578,
    1401,  4393,  1403,  1404,  5741, -1578,   275, -1578,   363,  4742,
    1269,  5277, -1578,  5741, -1578,   818, -1578, -1578,   211, -1578,
    1278, -1578, -1578, -1578,  5741,   791,  5741, -1578, -1578,  5741,
   -1578, -1578,  5277,  5741,  1464,  2066, -1578,  6276,  1229,  6276,
    1347,  1228, -1578,   828,   828,   828, -1578,   569, -1578,  1216,
    1216,  1274,   466,   704, -1578, -1578,  1202, -1578, -1578, -1578,
     116,  5741, -1578, -1578, -1578, -1578,  1462, -1578,  1205,  1438,
   -1578,  1456, -1578,  1400,  1402,   634, -1578, -1578, -1578, -1578,
   -1578,  1469, -1578, -1578, -1578, -1578, -1578,  5741,  6801,   466,
    1356,  1363,  1364, -1578,   466, -1578,   933, -1578, -1578,  1334,
     828, -1578,   500,  1337, -1578, -1578, -1578,   466, -1578,  1149,
     461, -1578, -1578, -1578,  1454,   461,  1341,   461, -1578,   466,
     466,  1223,   466, -1578,   993,  5741, -1578,  1405, -1578,   844,
   -1578, -1578, -1578,   253,  1340,  5741,   884,   121, -1578, -1578,
    1232,  1486, -1578, -1578, -1578, -1578,   482,  5741,  1345,  4267,
   -1578, -1578,  1345,  3934, -1578, -1578, -1578,   142, -1578,   287,
    1667,  4181,  4181,  6276,  6276,  1810,  1237,   683,  2226,  6276,
    2499,   199,  2781,  2879,  5610,  3140,  3238,  3499,  6276,  6276,
    1499,  6276,  1238,  6276,  1236,   377,  6276,  6276,  6276,  6276,
    6276,  6276, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
    1450,   990,   383,  5277, -1578,  4181,  6276,   326,   600,  1243,
     712, -1578, -1578, -1578, -1578,   998,   449, -1578, -1578,  1496,
     216,   216,   925,   925, -1578,   889, -1578, -1578, -1578, -1578,
   -1578,   895, -1578,  1332,  1514,  1414, -1578, -1578,  5741,  5741,
    5741,  5741,  5741,  5741,  1255, -1578,   403, -1578, -1578,  4393,
    1412, -1578,   429,  1420,  5741, -1578,   712,  5277,  1420,  1255,
    1267,  4393,  3318,  1480, -1578, -1578,  3036,  1270,   681, -1578,
   -1578, -1578,  1271,  4181,  1477, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,   100, -1578, -1578, -1578, -1578, -1578,
     906,  1340, -1578, -1578, -1578,   998,  5741, -1578, -1578, -1578,
   -1578, -1578,   909, -1578, -1578,  1006,  1542, -1578,   569, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578,  1277,  4181,  3421, -1578,
   -1578, -1578, -1578, -1578, -1578,  1414, -1578, -1578,  3421, -1578,
   -1578, -1578, -1578,   634,   634,  1311,  1298, -1578, -1578, -1578,
   -1578, -1578,   412,   156,   631, -1578,  1551, -1578, -1578, -1578,
    5741, -1578,   451,   921,  1415,  1430,  1305, -1578,  1216,   929,
   -1578, -1578,   724, -1578,  1308,  5741,   741,  1306,  1310,   466,
    1312,  1583,   580,  1414,  1006,  1300,  2066, -1578,  1276,   712,
    1394,  1409, -1578,  1324, -1578,   466,  1457,  1541, -1578,  6844,
   -1578,   466,  1360,  1544, -1578, -1578,  1322,  1325, -1578,  4181,
    4181, -1578,  4181,  4181, -1578,  1592,   168,   998,  1328, -1578,
   -1578,  1329,   998,  5857, -1578, -1578, -1578,  1505,  1330,  1331,
     998,   265,  1335,  1336,   998,  6276,  6276,  1237,  6276,  5943,
    1338,  1342,   998,  1343,  1348,   998,  6276,   998,  1349,  1353,
     998,  1354,  1355,   998,  1357,  1358,   998,   428,   627, -1578,
     635, -1578,   643,  6276, -1578,   651,   696,   740,   759,   767,
     778,  6276,  6276,  5277, -1578,  1556,   427,   998,  1431,  5277,
   -1578,  6276,  6276,  5277,  6276,  6276, -1578, -1578,  5741, -1578,
   -1578,  5741,  6546, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578,  4393,  1538,  4393, -1578,  1255,   403, -1578,   435,   435,
     271, -1578, -1578,  4514,   932, -1578,  1255,  1267,  1346,  3318,
    1359,   782,  1362,  4742,  1481, -1578,  1484,  1491,  1545,  5277,
     588,  1619,  1563, -1578, -1578, -1578,  1414, -1578,  1394,   699,
    1366,  1478,   828,  1511,  1216,  1513,  1216, -1578,  4181,   145,
    1113, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,  3421,
    3421, -1578,  1514,  6276, -1578,   961,  1495,  1311,   241, -1578,
    4181, -1578, -1578, -1578,  1519,  1520,  1521,  1522,  1523,  1524,
   -1578, -1578, -1578, -1578, -1578,  1379,   451, -1578,  1274, -1578,
   -1578,   461, -1578,  3421,   483,   461, -1578,   461, -1578,  1430,
   -1578, -1578,  1381, -1578, -1578,   253,  1514,  1216, -1578,  1300,
   -1578, -1578,  4514,  1639, -1578,   293, -1578,  1386, -1578,  1366,
   -1578,  1387,   828,   202,  1389, -1578,   828, -1578, -1578, -1578,
    1592, -1578,  1592,  6276,  1506,  1506,   613,  1393,   952, -1578,
    1506,  1506,  6276,  1506,  1506,   174,   998, -1578,  1078,  6276,
     998,  1506,  1506,  1506,  1506,   998,  1506,  1506,  1506,  1506,
    1506,  1506, -1578, -1578,  1396,  6882, -1578, -1578,   801, -1578,
   -1578, -1578, -1578, -1578, -1578,   855,   994, -1578, -1578,  5277,
    5277,  1623,  1498,   872,   963,  1017,   998, -1578,  6882,  1407,
   -1578, -1578,  1642,  1645,  5075,   457,  1411,  1410,  1604,  6276,
    1416,  1418,  5332,  5741,  5741, -1578, -1578, -1578,  1637,  6511,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578,   262, -1578,  1419, -1578, -1578, -1578,  1610,  1421,
    1424,  1428,  1433,  1426,   670, -1578, -1578,  1579, -1578, -1578,
   -1578, -1578,  1536, -1578, -1578, -1578, -1578, -1578,  1314, -1578,
    3318,  1346,  1709, -1578,   681, -1578, -1578, -1578,  4742,  1008,
    5741,  6276,  4181,  1578,  1514, -1578, -1578, -1578, -1578,  1006,
   -1578, -1578,   828,   933, -1578, -1578,   147, -1578, -1578, -1578,
   -1578, -1578,   998,  1695,   961, -1578,   840,  5741, -1578,  1641,
     151,  1534,  1643,  1540,  1679,  1646,  1682,  1414, -1578, -1578,
    1452, -1578,  1455, -1578,  1638,  1695,   483, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578,  1414,  1276, -1578,   933,  1551,
    1669,  1607, -1578,  1340, -1578, -1578, -1578,  1624,   130, -1578,
    1430,  1625,   998,  1461,  1466,   712, -1578, -1578,  1470,  1471,
     998,  1472,  1473,  6276,  6276,   998,  1474,  1476,  1479,  1482,
    1483,  1485,  1490,  1497,  1500,  1501, -1578, -1578, -1578, -1578,
    6276,  1502, -1578, -1578, -1578,  5277, -1578, -1578,  1269,   128,
   -1578,  1684, -1578,  6190,  5741, -1578, -1578,  6276, -1578,  4181,
     998, -1578, -1578, -1578,  1487, -1578, -1578, -1578,  5741, -1578,
     217, -1578,  1559, -1578,   450,  1492, -1578,  4666, -1578, -1578,
   -1578, -1578, -1578, -1578,   435,  1269,  5741,   815, -1578, -1578,
    1488, -1578,   998,   224,   770, -1578, -1578, -1578, -1578,  1216,
   -1578, -1578, -1578, -1578, -1578,  1503,  5741, -1578, -1578,  1659,
    1670,  5741,  1311, -1578, -1578,   634,  5741, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,  1514, -1578, -1578, -1578, -1578, -1578,
    1514, -1578, -1578, -1578, -1578,  4999,  1269, -1578, -1578,  6276,
     828, -1578, -1578, -1578,   828, -1578, -1578,  6276, -1578, -1578,
   -1578, -1578,   998,   998, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578,   998, -1578, -1578,  1477,  3421, -1578,
   -1578,  1507, -1578,   789,  4858,   686,   155,  1508, -1578,  1673,
   -1578,  5741,  5741,   451,   115, -1578, -1578,   506,  1688,  4181,
   -1578,   805, -1578,  1504, -1578,  1477,  1510, -1578,  4181,  6276,
   -1578, -1578,  1663, -1578,  1512, -1578, -1578, -1578, -1578, -1578,
   -1578,  1311,   634, -1578,  1649, -1578, -1578, -1578, -1578,  1551,
   -1578,  4999,  1517,   681, -1578, -1578,  1477,   998, -1578, -1578,
    1017,  1563, -1578,  1345, -1578,  1627,  4666, -1578,  1614, -1578,
    4666, -1578, -1578, -1578, -1578,  6592,   217,  6276,  4666,   160,
   -1578,  4666,  5665,  1563,  5741, -1578,   224, -1578, -1578,  3676,
   -1578,  1649,   274,  1311,  1668,  1671, -1578, -1578,   782,  1527,
    4999,  1710,  1563,  1528,  1269,  1578,  1529,  4095,  1525,   816,
    6592,  1532, -1578, -1578,   974,   146,  1748, -1578, -1578,  1578,
    1009, -1578, -1578,  1533,  1537,   771,  5741,  1311,   524,   524,
   -1578,  1764, -1578,  1629, -1578, -1578, -1578,   681,  4999, -1578,
   -1578,  1477,  1680, -1578,  4666,  1532, -1578, -1578,  1765,  4666,
    1781,  1772,  6592,  1680,  5741, -1578, -1578,  3676,  1546, -1578,
    5741, -1578, -1578, -1578,  1616,   192, -1578, -1578, -1578,   838,
    1563,   822,  1031,  6592,   127,  5741,  6592, -1578,   822, -1578,
   -1578,  5741,  1743, -1578, -1578, -1578,  4181,  1578, -1578, -1578,
   -1578,  6592, -1578, -1578, -1578,  1548,  1552,  1550, -1578,   224,
    1680, -1578, -1578,  5741,  5741,   822, -1578,  1554, -1578, -1578
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -1578, -1578, -1578, -1578,   -90,   292, -1578,   448,  1557,   589,
   -1578,  -326, -1578, -1578,  -561,   -72,  -649, -1068,  1822,  1561,
    1553,  1097, -1578, -1578, -1578,   701, -1578,  1185,   958, -1578,
     713, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578,   723, -1578, -1578, -1578, -1578,
   -1578,   304, -1578, -1578,  1515, -1578, -1578, -1578,  -349, -1578,
   -1578, -1578,  1493, -1578, -1578,  1250,  -226,  1531, -1578, -1578,
    -340, -1578,  -754, -1578, -1578, -1578, -1578, -1578, -1578,   798,
   -1578, -1248, -1578, -1578,  1543,  1076,   731,  1547,  1037,   727,
   -1578,   317,   -49,  -220, -1578,  -320,   257, -1018, -1578, -1578,
     345, -1578,  1549, -1124, -1578, -1578, -1578, -1578, -1239,   348,
      50,    51,    56,    18, -1578, -1578, -1578, -1578,  -110,  -244,
   -1578,  1077, -1578,  -662, -1578, -1003, -1578,   899, -1578, -1578,
   -1578, -1578, -1578, -1361,  -941, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578,   138, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
    -641,   152, -1578,  -461, -1578, -1578, -1578, -1578, -1578,   258,
   -1578,    83, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578,  1162, -1578, -1578,   852, -1578, -1154, -1578, -1578,  -933,
     114,  -653,   395,  -321,  -893,   628, -1578, -1578,  -542,  -764,
   -1578, -1578,  -987, -1381, -1578, -1578, -1578, -1578, -1578,  1560,
   -1578,  1323,  1558, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578,  1569, -1578, -1578, -1578, -1578, -1578, -1578,  -548,  -450,
   -1578,  1042,   593,  -567,  -351, -1578, -1578, -1578,  -282,  -968,
   -1578,  -350, -1578, -1578, -1578, -1578,  1280, -1578, -1578,  1070,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
    1917, -1578, -1578, -1578, -1578, -1578,  1621, -1578, -1578, -1578,
    1417, -1578,  1425, -1578, -1578, -1578, -1578,  1159, -1578, -1578,
   -1578,    10, -1578, -1092, -1578,  1147,  -500, -1578, -1578,  -891,
   -1578, -1578, -1578,    31,  -864,  -175,   220, -1578, -1578,  -656,
   -1578,  1789,  -894,  -735, -1578,  1182, -1116, -1290, -1578,  -911,
   -1578, -1578, -1578, -1578,   934, -1578, -1578,   673,    36, -1306,
   -1093, -1578,   186, -1577,  -507,  -971,   295, -1578,    64, -1578,
      96, -1578,  -834, -1578,  1955,  1959, -1578, -1578,  1434,  1960,
   -1578, -1578, -1578,  -178,  -280,  -401, -1578,  1212, -1125,   955,
   -1578,  -101,  -682,  -833, -1578, -1578, -1578,  -496,   -38,  -419,
    -574, -1578,  -488, -1578, -1578, -1578,   446, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578,  -510, -1578,   799, -1578,   573,
   -1578,  1197, -1015, -1578,  -607, -1578,   685,  -975,  -130,  -969,
    -964,  -946,   -55,     0,    23, -1578,   -51,  -742,  -476,  -132,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578, -1578,
   -1578, -1578, -1578, -1578, -1578, -1578, -1578,   756, -1578, -1047,
   -1578,   587,  -917,   551, -1578,   863,   316,  -700,  1408, -1173,
   -1578,   -59,  1897, -1578,   -47,   -54, -1357,   545,   -24,   -44,
    -135,   -15,   -43,  -185, -1578,   -60,   -23,    -9, -1578
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -1040
static const short yytable[] =
{
     132,   153,   200,   132,    79,   271,   222,   154,   216,   160,
      42,   291,   294,   290,   293,   218,   597,   511,   319,   726,
     780,   569,   634,   638,   571,   978,   219,   253,   235,   608,
     240,   820,   711,   203,  1146,   262,   257,   766,  1147,   788,
    1053,   259,   256,  1233,   228,   196,   198,   201,   204,   154,
     209,   261,   263,  1292,   960,   221,   198,   225,  1432,   204,
    1304,   154,   209,  1292,   238,   132,  1224,   258,  1528,   153,
     982,   746,  1302,   249,   264,   154,   252,   198,   260,   249,
     225,   201,   204,  1038,   154,   209,   238,   132,  1631,   204,
     486,  1006,   230,  1294,   205,   892,   277,   326,   279,  1295,
     486,   301,  1010,  1294,  1296,  1261,   233,   576,   664,  1295,
    1103,   561,  1253,   883,  1296,   230,   505,   975,   378,  1128,
    1346,   809,  1297,   810,  1484,   221,   154,   221,   952,   821,
     573,   322,  1297,  1041,   649,  1608,   385,  1329,   327,   225,
     528,  1351,   347,  1085,   320,   295,   204,   154,   238,   793,
    1149,  1298,   160,   911,  1500,   911,  1880,  1073,   901,   911,
    1075,  1298,  1070,   911,   762,  1805,  1417,   374,   911,  1082,
     752,   278,  1421,  1476,  1078,  1479,  1373,  1921,  1694,  1301,
     900,  1526,  1583,   531,   285,   908,  1305,   866,   931,   333,
    1009,   495,   324,   775,   718,  1311,   330,   554,  1645,   719,
     718,  1564,   334,  1316,   241,   719,   840,   796,   799,   800,
     801,   802,   532,  1280,  1835,   953,   954,  1175,  1334,   718,
    -693,   329,  1568,  1123,   719,   534,  1740,   473,   634,   638,
     477,  1701,   911,  1176,   148,  1352,  1849,   335,  1914,   336,
     517,   242,   718,   841,   -82,   275,   352,   719,   514,   353,
     164,  1273,   241,  1024,    61,  1869,   221,   718,   590,   221,
     831,  1135,   719,   154,   608,  1661,   487,   132,   136,   591,
    1626,  1627,   154,  1504,  1150,  1741,   487,   912,  1177,   912,
     529,  1519,  1520,   912,  1292,  1292,  1742,   912,   784,   242,
    1915,   718,   912,   201,   354,  1152,   719,   154,   740,  1563,
    1359,   718,  1274,   165,   138,   570,   719,  1178,   480,   864,
    1693,   598,   473,   600,   219,  1543,   584,   480,  1292,   588,
     544,  1858,  1382,  1917,  1294,  1294,  1634,   653,   853,   545,
    1295,  1295,   657,   648,   221,  1296,  1296,  1154,  1155,   149,
     276,   221,   524,   221,  1317,   221,   150,   842,   589,   337,
    1246,   646,  1494,  1297,  1297,   531,   912,   555,  1294,   955,
     540,  1521,  1257,  1258,  1295,   658,   204,   209,  1702,  1296,
     517,  1553,  1602,  1603,  1179,   154,  1646,   201,    14,  1219,
    1220,  1216,  1298,  1298,  1109,  1071,  1276,  1297,  1089,   718,
    1743,   911,  1806,  1148,   719,   276,   519,  1728,  1827,  1136,
    1526,  1306,  1307,   718,  1811,  1557,   776,   753,   719,  1698,
     930,   900,   794,   932,  1569,   900,  1298,  1299,  1300,  1153,
     655,  1151,  1479,  1811,  1517,   715,  1662,   959,  1013,  1538,
    1677,  -693,   283,  1773,  1798,   911,   964,  1313,  1245,  1846,
     720,   721,   722,   723,  1842,   867,   720,   721,   722,   723,
    1256,  1358,   529,    98,   718,  1739,  1129,  1851,   739,   719,
     139,  1635,  1859,   340,   846,   720,   721,   722,   723,   154,
     823,   518,   739,   204,   487,  1062,   652,  1831,   755,  1878,
     154,   511,   768,  1066,   154,   741,  1068,   204,   154,   546,
     722,   723,   160,  1289,   348,  1248,  1314,   797,   379,  1080,
    1834,   307,  1037,   720,   721,   722,   723,  -396,  1487,  1747,
     757,   341,   506,    84,   736,   912,   101,   162,  1488,  1754,
     512,  1907,  1831,  1221,  1096,   744,   519,  1909,  1511,   750,
     871,   161,  1820,   480,  1684,   163,   874,   720,   721,   722,
     723,   822,  1920,   520,   848,  1923,   847,   720,   721,   722,
     723,   824,  1690,   103,   963,   289,  1378,   486,  1726,   912,
    1931,  1831,   742,  1656,  1213,   210,  1427,  1139,   815,  1691,
     221,   349,   892,    99,   815,  1369,  1370,  1065,  1371,  1372,
    1345,  1556,  1014,   358,  1860,  1252,  1541,  1647,   284,   212,
     213,   573,   825,  1831,   663,   380,   877,   878,   762,   979,
    1315,  1036,  1703,  1242,   391,  1614,  1254,  1544,  1419,   134,
    1491,  1249,   663,    14,  1143,   872,   219,  1560,  1893,   862,
     230,    77,   104,   342,  1663,   287,   718,   770,  1664,  1665,
    1121,   719,   105,  1029,  1791,   720,   721,   722,   723,    77,
     226,   343,  1015,   375,   531,   221,   934,  1894,  1615,   720,
     721,   722,   723,   718,   895,    77,  1204,   106,   719,   663,
     359,   718,  1813,   893,  1692,   287,   719,  1060,  1061,   718,
      77,  1055,   287,  1214,   719,   344,   135,   718,  1318,   376,
    1243,  1707,   719,   779,   225,   244,    77,   863,  1895,   151,
      78,  1555,   473,  1832,   473,   852,   987,  1615,   771,   288,
     720,   721,   722,   723,  1062,   289,  1284,  1402,    78,   968,
    1792,  1616,   718,  1292,  1516,  1319,   221,   719,   295,   289,
    1575,   221,   718,   221,    78,   221,  1225,   719,   487,   557,
     739,  1226,  1922,   487,  1108,  1896,  1530,   246,  1051,  1051,
    1054,   529,   487,  1777,  1056,   204,   292,   219,  1264,  1778,
     204,  1064,   289,  1294,  1027,    78,  1303,   250,  1660,  1295,
    1616,   296,  1265,  1765,  1296,   154,   718,   154,  1312,  1767,
    -726,   719,  1796,   473,   221,  1347,   221,  1102,  1900,   558,
    1901,  1166,  1297,   567,  1776,   718,  1266,   295,  1091,  1333,
     719,  1908,   265,   718,  -696,  1101,   568,   389,   719,  1087,
    1090,  1780,   221,  1235,   718,   394,  1238,  1167,  1529,   719,
     524,  1298,  1035,   559,   287,   718,  1139,   295,  1117,  1118,
     719,  1120,  1424,  -696,  -696,   230,  -696,   718,   221,   221,
     928,   560,   719,  -696,  -696,  -696,  -696,  -696,  1935,  -696,
    1267,   764,  -696,  -696,  -696,  -696,  -696,    53,  1828,  1264,
    -696,  -696,   143,   144,  1888,  1542,  -696,  1597,    54,  1760,
     936,  1552,   230,  1265,  1872,    55,   154,  1095,  1499,   531,
     295,   254,   720,   721,   722,   723,   154,  1222,  1883,  1405,
    1106,   718,  1264,   765,  1761,  1826,   719,  1266,  1145,   267,
      56,  1889,   230,   230,  1833,   230,  1265,  1867,   718,   720,
     721,   722,   723,   719,  1890,  1264,  1403,   720,   721,   722,
     723,  1122,   268,   938,  1406,   720,   721,   722,   723,  1265,
    1266,  1133,  1407,   720,   721,   722,   723,   747,  1653,  1836,
    1409,   718,  1239,  1214,  1865,  1899,   719,   272,  1506,  1214,
    1032,  1267,  1762,  1266,  1758,   155,  1930,  1237,  1507,  1240,
   -1014,   718,   295, -1017,   748,    57,   719,  1033,   720,   721,
     722,   723,  1797,   280,  1669,  1236,   529,  1916,   720,   721,
     722,   723,   273,   511,  1267,  1410,  1029,   156,  1609,   487,
     204,   209,   487,   238,   132,  -244,  1670,    58,    81,    82,
     487,    59,   157,  1671,   577,   940,  1750,  1267,   274,  1537,
     718,  1335,   487,   487,   281,   719,  1336,   204,   477,   796,
    1918,   282,   720,   721,   722,   723,   718,  1924,  1335,  1411,
     718,   719,   287,  1339,   718,   719,  1260,  1672,   286,   719,
    1559,   720,   721,   722,   723,   212,   213,   221,  1412,   720,
     721,   722,   723,   718,  1938,  1736,  1413,   302,   719,  1028,
     720,   721,   722,   723,   634,   638,  1212,  1414,  1600,   328,
    1879,   720,   721,   722,   723,  1794,   144,   730,  1342,   731,
    1097,  1098,   511,   720,   721,   722,   723,   734,   318,   735,
    1598,  1810,  1811,   759,  1360,  -244,   361,   789,   219,   331,
    1364,  1324,  1877,  1811,   364,   477,   811,   531,   812,  1936,
    1937,  1727,  1325,   833,   718,   834,  1324,  -244,   716,   719,
     961,  1004,   978,  1005,  -244,   350,   372,   221,  1483,  1483,
    1486,  1126,   363,  1127,   221,   382,   384,   720,   721,   722,
     723,   365,   386,   366,  1599,  1584,   387,   390,  1755,  1214,
    1214,  1752,   230,   391,   720,   721,   722,   723,  -244, -1037,
     474,  1606,  1513,   482,  1515,  1750,   367, -1037,   230,  1750,
    1659,   888, -1037,  1134,   230, -1037,  1225,  1750,  1227,  1475,
    1847,  1478,  1228,   100,  1229,  1809,   489,   720,   721,   722,
     723,   368,  1428,  1126,  1816,  1277,  1282,   493,  1283,  1786,
     473, -1038,   494,   219,   529,   303,  1750,   490,  1326, -1038,
    1327, -1037,   496,   497, -1038,   831,  1331, -1038,  1332,   716,
   -1037,  1489,   498,   525,   101,  1558,   513,   500, -1037,   221,
     516,   515,  1433,  1474,   535,   542,   102,  1051,    14,  1225,
     304,  1577,   487,  1750,   487,   553,   564, -1037,  1750, -1037,
     716,   556,  1607, -1038,  1699,   582,   720,   721,   722,   723,
     487,   103, -1038, -1037,   204, -1037, -1037, -1037,   305,   557,
   -1038,   306,   720,   721,   722,   723,   720,   721,   722,   723,
     720,   721,   722,   723,   585,   595,   929,   307,   933, -1038,
     601, -1038,   937,   939,   941,   716,  1884,  1648,  1885,   720,
     721,   722,   723,   599,   650, -1038,   654, -1038, -1038, -1038,
    1752,   656,   692,   308,  1752,   689, -1037,  1567,  1811,   558,
    1919,  1571,  1752,   690,   531,  1848,   410,   411,   412,   691,
     104,   693,   694,   695,   696,  1214,  1540,  1871,   544,   697,
     105, -1000,   699,   700,   698,  1839,   701,   545,   706,  1841,
     717,  1752,  1929,   559,   420,   421,   707,  1845, -1038,   716,
     720,   721,   722,   723,   816,   106,   725,   724, -1017,   732,
     729,   560,   733,   737,  1169,  1649,  1173,   751,  1181,  1184,
    1779,  1189,  1192,  1195,   754,  1613,  1875,   742,  1752,    63,
     759,   761,   774,  1752,   778,   782,   783,   785,   786,   787,
      64,   791,   790,   795,   544,   803,   798,    65,   804,   806,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037,   807,   813,   837,
      66,   529, -1037,  1902, -1037,   838,   839,  -455,  1904,   844,
     287,   850,   849,   854,   855,   198,   856,   857,   859,   860,
     869,   865,   870,  1625,  1145,  1145,   875,   876,   885,   887,
    1474,   888, -1038, -1038, -1038, -1038, -1038, -1038, -1038,   897,
     896,   956,   898,   866, -1038,   902, -1038,  1051,   962,   983,
   -1039,   441,   289,  1000,  1002,   999,  1007,  1023, -1039,  1011,
    1012,    67,  1030, -1039,  1039,  1046, -1039,  1043,  1062,  1048,
    1067,   487,  1076,  1077,  1078,   503,   816,    68,  1092,   204,
    1083,   320,  1084,  1086,   441,  1093,  1094,   602,  1100,  1104,
    1110,  1119,  1753,  1129,    69,  1115,  1142,  1124,   747,  1199,
    1141,  1757, -1039,  1203,  1483,  1163,  1211,  1201,   154,    70,
    1223, -1039,   718,    71,  1230,   539,    72,  1766,   541, -1039,
     107,  1232,  1241,  1250,    73,   942,   943,   944,   945,   946,
     947,   948,   949,  1247,  1243,    85,  1259,  1263, -1039,  1269,
   -1039,  1271,    86,    74,  1285,  1288,    87,    88,  1308,   108,
     109,  1320,   110,  1675, -1039,    89, -1039, -1039, -1039,   111,
     112,   113,   114,   115,  1774,   116,  1310,    90,   117,   118,
     119,   120,   121,  1328,  1330,  1340,   122,   123,  1337,  1341,
    1734,  1343,   124,  1344,  1353,  1355,  1362,  1135,  1365,  1366,
     911,  1367,   268,  1379,  1368,   204,  1374,  1375,  1380,  1381,
    1418,  1477,  1420,  1383,  1384,  1495,  1391, -1039,  1496,  1145,
    1392,  1393,  1775,  1490,   703,  1497,  1394,  1396,   708,   709,
     713,  1397,  1398,  1399,  1498,  1400,  1401,   320,  1492,  1501,
    1502,  1493,    91,   734,  1748,  1788,  1509,  1512,  1527,  1789,
    1514,  1531,  1532,  1533,  1534,  1535,  1536,   968,  1539,    92,
    1554,  1753,   154,  1561,  1565,  1753,  1566,   201,  1570,  1376,
     531,  1821,  1576,  1753,    93,  1596,  1753,  1604,    94,  1605,
     153,  1611,  1801,  1610, -1006,   913,   154,  1618,  1619,    95,
     135,  1629,  1621,   718,  1622,  1636,  1637,  1638,   719,   756,
    1639,   914,  1753,   758,  1640,   531,  1642,  1772,    96,  1641,
    1643, -1039, -1039, -1039, -1039, -1039, -1039, -1039,  1644,   762,
    1654,  1666,  1678, -1039,  1676, -1039,  1681,  1679,  1680,  1683,
    1682,  1685,   198,  1803,  1686,  1695,  1687,   915,  1697,  1753,
    1705,  1700,  1704,  1804,  1753,  1706,   916,   531,  1731,  1708,
    1709,  1710,  1711,  1714,   917,  1715,  1630,  1770,  1716,  1800,
    1771,  1717,  1718,  1738,  1719,  1759,   153,   529,   531,  1720,
    1749,   531,   154,   918,  1808,   919,  1721,  1818,  1822,  1722,
    1723,  1725,  1768,  1837,  1799,  1793,   531,  1812,  1814,   920,
    1819,   921,   922,   923,  1830,  1840,  1474,  1861,  1882,  1868,
    1863,  1876,   529,   221,  1856,  1027,  1866,  1870,  1873,  1811,
     154,  1858,  1886,   164,  1887,   153,  1158,  1859,  1903,  1159,
    1905,   154,  1906,   395,  1911,  1913,  1927,  1932,  1934,  1933,
     481,  1474,   396,  1939,   141,  1480,  1912,   488,   499,  1001,
    1363,  1856,   924,   886,   529,   397,  1140,   154,  1349,  1361,
    1689,   398,   399,   153,   851,   565,  1287,  1350,  1081,   154,
    1357,  1040,  1688,   609,   548,   529,  1729,   533,   529,  1668,
     910,  1857,  1856,  1474,  1674,  1027,  1864,  1898,   154,   400,
    1862,   201,   401,   529,  1234,  1807,  1795,   441,  1074,  1843,
    1746,   935,   980,  1278,  1474,  1829,  1145,  1474,   951,   578,
    1657,   580,   201,   958,   441,   572,  1505,   563,   836,  1116,
     403,   404,  1474,   441,   965,   586,   969,   970,   971,   972,
     973,   965,   405,   873,   201,   201,   406,   100,  1551,  1107,
     142,   526,   769,  1031,  1042,   300,   925,   926,   927,   720,
     721,   722,   723,  1025,   773,  1817,   963,  1573,  1574,  1763,
    1262,  1910,  1892,  1578,  1579,    45,  1581,  1582,   441,    46,
      49,   781,  1008,  1255,  1586,  1587,  1588,  1589,   101,  1590,
    1591,  1592,  1593,  1594,  1595,  1518,  1387,  1422,  1338,   441,
     102,  1160,  1769,   407,   539,   255,  1045,  1624,     0,   408,
       0,   832,     0,     0,   137,     0,     0,     0,     0,   409,
       0,     0,     0,     0,     0,   103,     0,     0,     0,     0,
       0,     0,     0,   410,   411,   412,   413,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
       0,     0,     0,     0,     0,     0,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   108,   424,     0,   110,     0,
       0,   425,   426,     0,   427,   428,   429,   113,   114,   115,
     430,   116,     0,     0,   117,   118,   119,   120,   121,     0,
       0,     0,   122,   123,   104,     0,     0,     0,   124,     0,
       0,     0,   431,   432,   105,     0,     0,     0,   502,     0,
     434,     0,     0,     0,   289,     0,     0,     0,     0,   577,
       0,     0,     0,     0,     0,     0,     0,   100,     0,   106,
    1156,  1157,  1162,     0,     0,  1170,  1171,  1174,     0,  1182,
    1185,  1187,  1190,  1193,  1196,  1197,  1198,   107,  1200,     0,
    1202,     0,     0,  1205,  1206,  1207,  1208,  1209,  1210,   913,
       0,     0,     0,     0,     0,     0,     0,   718,   101,     0,
     441,     0,   719,  1217,     0,   914,   108,   109,     0,   110,
     102,     0,     0,     0,     0,     0,   111,   112,   113,   114,
     115,     0,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,   103,     0,     0,     0,   124,
       0,   915,     0,     0,     0,     0,     0,     0,     0,     0,
     916,     0,     0,     0,   441,     0,     0,     0,   917,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   918,     0,   919,
       0,     0,  1168,     0,     0,  1159,     0,     0,     0,   395,
       0,     0,     0,   920,     0,   921,   922,   923,   396,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,   397,     0,     0,   105,     0,     0,   398,   399,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   106,
       0,     0,     0,     0,     0,   400,   924,     0,   401,     0,
       0,   108,   109,     0,   110,     0,     0,   107,     0,     0,
       0,   111,   112,   113,   114,   115,     0,   116,     0,     0,
     117,   118,   119,   120,   121,     0,   403,   404,   122,   123,
     583,     0,     0,     0,   124,     0,   108,   109,   405,   110,
       0,     0,   406,     0,     0,     0,   111,   112,   113,   114,
     115,     0,   116,     0,     0,   117,   118,   119,   120,   121,
     965,     0,     0,   122,   123,     0,     0,     0,     0,   124,
       0,     0,  1385,  1386,     0,  1388,  1390,     0,     0,     0,
       0,     0,     0,  1395,     0,     0,     0,     0,     0,     0,
     925,   926,   927,   720,   721,   722,   723,  1160,     0,   407,
    1408,     0,     0,     0,     0,   408,     0,     0,  1415,  1416,
     441,     0,     0,     0,     0,   409,   441,     0,  1217,  1423,
     441,  1425,  1426,     0,     0,     0,     0,     0,     0,   410,
     411,   412,   413,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,     0,     0,     0,     0,
     441,     0,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   108,   424,     0,   110,     0,   441,   425,   426,     0,
     427,   428,   429,   113,   114,   115,   430,   116,     0,     0,
     117,   118,   119,   120,   121,     0,     0,     0,   122,   123,
       0,     0,     0,     0,   124,     0,     0,     0,   431,   432,
    1522,     0,     0,   107,   502,  1172,   434,     0,  1159,     0,
     289,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   108,   109,   397,   110,     0,     0,     0,     0,
     398,   399,   111,   112,   113,   114,   115,     0,   116,   441,
       0,   117,   118,   119,   120,   121,     0,     0,     0,   122,
     123,     0,     0,     0,     0,   124,     0,     0,   400,     0,
    1572,   401,     0,     0,     0,     0,     0,     0,     0,  1580,
       0,     0,     0,     0,     0,     0,  1585,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   405,     0,     0,     0,   406,   441,   441,     0,   610,
       0,     0,     0,   611,     0,     0,     0,     0,     0,     0,
     816,     0,     0,     0,     0,     0,  1620,     0,     0,     0,
       0,   612,     0,     0,   613,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1160,     0,   407,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,   614,     0,     0,   409,     0,
       0,   615,   616,   617,     0,   618,   619,   620,  1652,   621,
       0,     0,   410,   411,   412,   413,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,   622,
       0,   623,     0,   976,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   108,   424,     0,   110,     0,     0,
     425,   426,     0,   427,   428,   429,   113,   114,   115,   430,
     116,     0,   624,   117,   118,   119,   120,   121,     0,     0,
       0,   122,   123,     0,     0,     0,     0,   124,     0,   625,
       0,   431,   432,     0,     0,     0,     0,   502,     0,   434,
    1712,  1713,     0,   289,     0,     0,     0,  1180,     0,     0,
    1159,   626,     0,     0,   395,     0,     0,  1724,     0,     0,
       0,     0,   441,   396,     0,     0,   107,     0,     0,     0,
    1733,     0,     0,     0,  1735,     0,   397,     0,     0,     0,
       0,     0,   398,   399,     0,     0,   627,   628,     0,     0,
       0,     0,     0,     0,     0,   108,   109,     0,   110,     0,
       0,     0,     0,   629,     0,   111,   112,   113,   114,   115,
     400,   116,     0,   401,   117,   118,   119,   120,   121,     0,
       0,     0,   122,   123,     0,     0,     0,     0,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   404,     0,     0,  1183,     0,   107,  1159,     0,
       0,     0,   395,   405,     0,     0,  1787,   406,     0,     0,
       0,   396,     0,     0,  1790,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,     0,   108,   109,     0,   110,
     398,   399,     0,     0,     0,     0,   111,   112,   113,   114,
     115,   441,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,     0,     0,     0,   400,   124,
       0,   401,  1160,     0,   407,   977,  1652,     0,     0,     0,
     408,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     409,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,     0,     0,   410,   411,   412,   413,   212,   213,
       0,   405,     0,     0,     0,   406,     0,     0,   414,     0,
       0,     0,     0,     0,  1844,     0,     0,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   108,   424,     0,   110,
       0,     0,   425,   426,     0,   427,   428,   429,   113,   114,
     115,   430,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,     0,     0,     0,     0,   124,
    1160,     0,   407,   431,   432,     0,     0,     0,   408,   502,
       0,   434,     0,     0,     0,   289,     0,     0,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   410,   411,   412,   413,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   414,     0,     0,     0,
       0,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   108,   424,     0,   110,     0,     0,
     425,   426,     0,   427,   428,   429,   113,   114,   115,   430,
     116,     0,     0,   117,   118,   119,   120,   121,     0,     0,
       0,   122,   123,     0,     0,     0,  1188,   124,     0,  1159,
       0,   431,   432,   395,     0,     0,     0,   502,     0,   434,
       0,     0,   396,   289,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   397,     0,     1,     0,     0,
       0,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,    14,
       0,     0,     0,     0,     0,     0,     3,     0,     0,   400,
       0,     0,   401,     4,     0,     5,     0,     6,     0,     0,
       0,     0,     7,     0,     0,     0,     0,     0,     0,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,   404,   107,     9,  1191,     0,     0,  1159,     0,     0,
       0,   395,   405,     0,    10,     0,   406,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   108,   109,   397,   110,     0,     0,     0,     0,   398,
     399,   111,   112,   113,   114,   115,     0,   116,     0,     0,
     117,   118,   119,   120,   121,     0,     0,     0,   122,   123,
       0,     0,     0,     0,   124,     0,     0,   400,     0,     0,
     401,  1160,     0,   407,  1016,     0,     0,     0,     0,   408,
       0,    11,     0,     0,     0,     0,     0,     0,    12,   409,
       0,    13,     0,    14,    15,     0,     0,     0,   403,   404,
       0,     0,     0,   410,   411,   412,   413,   212,   213,     0,
     405,     0,     0,     0,   406,     0,     0,   414,    16,     0,
       0,     0,     0,     0,     0,     0,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   108,   424,     0,   110,     0,
       0,   425,   426,     0,   427,   428,   429,   113,   114,   115,
     430,   116,   988,     0,   117,   118,   119,   120,   121,     0,
       0,     0,   122,   123,     0,     0,     0,    17,   124,  1160,
       0,   407,   431,   432,     0,     0,     0,   408,   502,     0,
     434,     0,    18,    19,   289,     0,     0,   409,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   410,   411,   412,   413,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,   398,     0,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   108,   424,     0,   110,     0,     0,   425,
     426,     0,   427,   428,   429,   113,   114,   115,   430,   116,
     400,     0,   117,   118,   119,   120,   121,     0,     0,     0,
     122,   123,     0,     0,     0,  1194,   124,   991,  1159,     0,
     431,   432,   395,     0,     0,     0,   502,     0,   434,   402,
       0,   396,   289,     0,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
     398,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   108,   109,     0,   110,     0,     0,     0,
       0,     0,     0,   111,   112,   113,   114,   115,   400,   116,
       0,   401,   117,   118,   119,   120,   121,     0,     0,     0,
     122,   123,     0,     0,     0,     0,   124,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,   403,
     404,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     409,   405,     0,     0,     0,   406,     0,     0,   396,     0,
       0,     0,     0,     0,   410,   411,   412,     0,   212,   213,
       0,   397,     0,     0,     0,     0,     0,   398,   399,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   667,     0,     0,     0,
       0,     0,   425,   426,     0,   400,     0,     0,   401,     0,
    1160,   430,   407,     0,     0,     0,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   409,     0,
       0,     0,     0,     0,  1290,     0,   403,   404,     0,     0,
       0,     0,   410,   411,   412,   413,   212,   213,   405,     0,
       0,     0,   406,     0,     0,     0,   414,     0,     0,     0,
     903,     0,     0,     0,     0,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   108,   424,     0,   110,     0,     0,
     425,   426,     0,   427,   428,   429,   113,   114,   115,   430,
     116,     0,     0,   117,   118,   119,   120,   121,     0,   710,
       0,   122,   123,   669,     0,  1760,     0,   124,     0,   407,
       0,   431,   432,     0,     0,   408,     0,   502,     0,   434,
       0,     0,     0,   289,     0,   409,     0,     0,     0,     0,
    1761,     0,     0,     0,     0,     0,     0,     0,     0,   410,
     411,   412,   413,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,     0,     0,     0,     0,
       0,     0,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   108,   424,     0,   110,     0,     0,   425,   426,     0,
     427,   428,   429,   113,   114,   115,   430,   116,  1762,     0,
     117,   118,   119,   120,   121,   904,   905,   906,   122,   123,
       0,   395,     0,     0,   124,     0,     0,     0,   431,   432,
     396,     0,     0,     0,   673,     0,   434,     0,     0,     0,
     289,     0,   107,   397,     0,     0,   666,     0,     0,   398,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   667,     0,
       0,   108,   109,     0,   110,     0,     0,   400,     0,     0,
     401,   111,   112,   113,   114,   115,     0,   116,     0,     0,
     117,   118,   119,   120,   121,     0,     0,     0,   122,   123,
       0,     0,   478,     0,   124,     0,     0,   395,   403,   404,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
     405,     0,     0,     0,   406,     0,     0,     0,   107,   397,
       0,     0,   668,     0,     0,   398,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   667,     0,     0,   108,   109,     0,
     110,     0,     0,   400,     0,     0,   401,   111,   112,   113,
     114,   115,     0,   116,     0,   669,   117,   118,   119,   120,
     121,   407,     0,     0,   122,   123,     0,   408,     0,     0,
     124,     0,     0,     0,   403,   404,     0,   409,     0,     0,
       0,     0,     0,     0,     0,     0,   405,     0,     0,     0,
     406,   410,   411,   412,   413,   212,   213,     0,   899,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,     0,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   108,   424,     0,   110,     0,     0,   425,
     426,     0,   427,   428,   429,   113,   114,   115,   430,   116,
       0,   669,   117,   118,   670,   671,   672,   407,     0,     0,
     122,   123,     0,   408,     0,     0,   124,     0,     0,     0,
     431,   432,     0,   409,     0,     0,   673,     0,   434,     0,
       0,     0,   289,     0,     0,     0,     0,   410,   411,   412,
     413,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,     0,     0,     0,     0,     0,     0,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   108,
     424,     0,   110,     0,     0,   425,   426,     0,   427,   428,
     429,   113,   114,   115,   430,   116,     0,     0,   117,   118,
     670,   671,   672,     0,   395,     0,   122,   123,   483,     0,
       0,     0,   124,   396,     0,     0,   431,   432,     0,     0,
       0,     0,   673,     0,   434,   107,   397,     0,   289,     0,
       0,     0,   398,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   667,     0,     0,   108,   109,     0,   110,     0,     0,
     400,     0,     0,   401,   111,   112,   113,   114,   115,     0,
     116,     0,     0,   117,   118,   119,   120,   121,     0,     0,
       0,   122,   123,     0,     0,     0,     0,   124,     0,     0,
     395,   403,   404,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,   405,     0,     0,     0,   406,     0,     0,
       0,   107,   397,     0,     0,   668,     0,     0,   398,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   667,     0,     0,
     108,   109,     0,   110,     0,     0,   400,     0,     0,   401,
     111,   112,   113,   114,   115,     0,   116,     0,   669,   117,
     118,   119,   120,   121,   407,     0,     0,   122,   123,     0,
     408,     0,     0,   124,     0,     0,     0,   403,   404,     0,
     409,     0,     0,  1874,     0,   434,     0,     0,     0,   405,
       0,     0,     0,   406,   410,   411,   412,   413,   212,   213,
       0,   899,     0,     0,     0,     0,     0,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   108,   424,     0,   110,
       0,     0,   425,   426,     0,   427,   428,   429,   113,   114,
     115,   430,   116,     0,   669,   117,   118,   670,   671,   672,
     407,     0,     0,   122,   123,     0,   408,     0,     0,   124,
       0,     0,     0,   431,   432,     0,   409,     0,     0,   673,
       0,   434,     0,     0,     0,   289,     0,   988,     0,     0,
     410,   411,   412,   413,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,   414,     0,     0,     0,     0,     0,
       0,     0,     0,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   108,   424,     0,   110,     0,     0,   425,   426,
       0,   427,   428,   429,   113,   114,   115,   430,   116,     0,
       0,   117,   118,   119,   120,   121,     0,   395,     0,   122,
     123,     0,     0,     0,     0,   124,   396,     0,     0,   431,
     432,   989,     0,     0,     0,   673,     0,   434,     0,   397,
       0,   289,     0,     0,     0,   398,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   990,     0,     0,
       0,     0,   991,   400,     0,     0,   401,     0,   992,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,     0,     0,     0,     0,     0,   993,     0,     0,
       0,     0,   402,     0,   403,   404,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,   405,     0,   108,   109,
     406,   110,     0,   396,     0,     0,     0,     0,   111,   112,
     113,   114,   115,     0,   116,     0,   397,   117,   118,   119,
     120,   121,   398,   399,     0,   122,   123,     0,     0,     0,
       0,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,   401,     0,     0,     0,   407,     0,     0,
       0,     0,     0,   408,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,     0,     0,     0,     0,     0,   402,
       0,   403,   404,     0,     0,     0,     0,   410,   411,   412,
     413,   212,   213,   405,     0,     0,     0,   406,     0,     0,
       0,   414,     0,     0,     0,     0,     0,     0,     0,     0,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   108,
     424,     0,   110,     0,     0,   425,   426,     0,   427,   428,
     429,   113,   114,   115,   430,   116,     0,     0,   117,   118,
     119,   120,   121,     0,   710,     0,   122,   123,     0,     0,
       0,     0,   124,     0,   407,     0,   431,   432,   507,     0,
     408,     0,   502,     0,   434,     0,     0,     0,   289,     0,
     409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   410,   411,   412,   413,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   108,   424,     0,   110,
       0,     0,   425,   426,     0,   427,   428,   429,   113,   114,
     115,   430,   116,     0,     0,   117,   118,   119,   120,   121,
       0,   395,   107,   122,   123,     0,     0,     0,     0,   124,
     396,     0,     0,   431,   432,     0,     0,     0,     0,   502,
       0,   434,     0,   397,     0,   289,     0,     0,     0,   398,
     399,   108,   109,     0,   110,     0,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,     0,   116,     0,     0,
     117,   118,   119,   120,   121,     0,     0,   400,   122,   123,
     401,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,   107,     0,
       0,     0,     0,     0,     0,     0,   402,   395,   403,   404,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
     405,     0,     0,     0,   406,     0,     0,   108,   109,   397,
     110,     0,     0,     0,     0,   398,   399,   111,   112,   113,
     114,   115,     0,   116,     0,     0,   117,   118,   119,   120,
     121,     0,     0,     0,   122,   123,     0,     0,     0,     0,
     124,     0,     0,   400,     0,     0,   401,     0,     0,     0,
    1016,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   407,     0,     0,     0,     0,     0,   408,     0,     0,
       0,     0,     0,     0,   403,   404,     0,   409,     0,     0,
       0,     0,     0,     0,     0,     0,   405,     0,     0,     0,
     406,   410,   411,   412,   413,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,   414,     0,     0,     0,     0,
       0,     0,     0,     0,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   108,   424,     0,   110,     0,     0,   425,
     426,     0,   427,   428,   429,   113,   114,   115,   430,   116,
       0,     0,   117,   118,   119,   120,   121,   407,     0,     0,
     122,   123,     0,   408,     0,     0,   124,     0,     0,     0,
     431,   432,     0,   409,     0,     0,   433,     0,   434,     0,
       0,   702,   289,     0,     0,     0,     0,   410,   411,   412,
     413,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,     0,     0,     0,     0,     0,     0,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   108,
     424,     0,   110,     0,     0,   425,   426,     0,   427,   428,
     429,   113,   114,   115,   430,   116,     0,     0,   117,   118,
     119,   120,   121,     0,   395,   107,   122,   123,     0,     0,
       0,     0,   124,   396,     0,     0,   431,   432,     0,     0,
       0,     0,   502,     0,   434,     0,   397,     0,   289,     0,
       0,     0,   398,   399,   108,   109,     0,   110,     0,     0,
       0,     0,     0,     0,   111,   112,   113,   114,   115,     0,
     116,     0,     0,   117,   118,   119,   120,   121,     0,     0,
     400,   122,   123,   401,     0,     0,     0,   124,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1781,     0,     0,
       0,   107,     0,     0,     0,     0,     0,     0,     0,     0,
     395,   403,   404,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,   405,     0,     0,     0,   406,     0,     0,
     108,   109,   397,   110,     0,     0,     0,     0,   398,   399,
     111,   112,   113,   114,   115,     0,   116,     0,     0,   117,
     118,   119,   120,   121,     0,     0,     0,   122,   123,     0,
       0,     0,     0,   124,     0,     0,   400,     0,     0,   401,
       0,  1612,     0,     0,   710,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,     0,
     408,     0,     0,     0,     0,   402,     0,   403,   404,     0,
     409,     0,     0,     0,     0,     0,     0,     0,     0,   405,
       0,     0,     0,   406,   410,   411,   412,   413,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
       0,     0,     0,     0,     0,     0,     0,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   108,   424,     0,   110,
       0,     0,   425,   426,     0,   427,   428,   429,   113,   114,
     115,   430,   116,     0,     0,   117,   118,   119,   120,   121,
     407,     0,     0,   122,   123,     0,   408,     0,     0,   124,
       0,     0,     0,   431,   432,     0,   409,     0,     0,   502,
       0,   434,     0,     0,     0,   289,     0,     0,     0,     0,
     410,   411,   412,   413,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,   414,     0,     0,     0,     0,     0,
       0,     0,     0,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   108,   424,     0,   110,     0,     0,   425,   426,
       0,   427,   428,   429,   113,   114,   115,   430,   116,     0,
       0,   117,   118,   119,   120,   121,     0,   395,   107,   122,
     123,     0,     0,     0,     0,   124,   396,     0,     0,   431,
     432,     0,     0,     0,     0,   502,     0,   434,     0,   397,
       0,   289,     0,     0,     0,   398,   399,   108,   109,     0,
     110,     0,     0,     0,     0,     0,     0,   111,   112,   113,
     114,   115,     0,   116,     0,     0,   117,   118,   119,   120,
     121,     0,     0,   400,   122,   123,   401,     0,     0,     0,
     124,     0,     0,     0,     0,     0,     0,     0,  1623,     0,
       0,     0,    77,     0,   107,     0,     0,     0,     0,     0,
       0,     0,     0,   395,   403,   404,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,   405,     0,     0,     0,
     406,     0,     0,   108,   109,   397,   110,     0,     0,     0,
       0,   398,   399,   111,   112,   113,   114,   115,     0,   116,
       0,     0,   117,   118,   119,   120,   121,     0,     0,     0,
     122,   123,     0,     0,     0,     0,   124,     0,     0,   400,
       0,    78,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,   408,     0,     0,     0,     0,     0,     0,
     403,   404,     0,   409,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,   406,   410,   411,   412,
     413,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,   414,     0,     0,     0,     0,     0,     0,     0,     0,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   108,
     424,     0,   110,     0,     0,   425,   426,     0,   427,   428,
     429,   113,   114,   115,   430,   116,     0,     0,   117,   118,
     119,   120,   121,   407,     0,     0,   122,   123,     0,   408,
       0,     0,   124,     0,     0,     0,   431,   432,     0,   409,
       0,     0,   502,   974,   434,     0,     0,     0,   289,     0,
    1186,     0,     0,   410,   411,   412,   413,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
       0,     0,     0,     0,     0,     0,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   108,   424,     0,   110,     0,
       0,   425,   426,     0,   427,   428,   429,   113,   114,   115,
     430,   116,     0,     0,   117,   118,   119,   120,   121,     0,
     395,   107,   122,   123,     0,     0,     0,     0,   124,   396,
       0,     0,   431,   432,     0,     0,     0,     0,   502,     0,
     434,     0,   397,     0,   289,     0,     0,     0,   398,   399,
     108,   109,     0,   110,     0,     0,     0,     0,     0,     0,
     111,   112,   113,   114,   115,     0,   116,     0,     0,   117,
     118,   119,   120,   121,     0,     0,   400,   122,   123,   401,
       0,     0,     0,   124,     0,     0,     0,     0,     0,   977,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,     0,     0,   395,   403,   404,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,   405,
       0,     0,     0,   406,     0,     0,   108,   109,   397,   110,
       0,     0,     0,     0,   398,   399,   111,   112,   113,   114,
     115,     0,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,     0,     0,     0,     0,   124,
       0,     0,   400,     0,     0,   401,     0,     0,     0,     0,
    1376,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,   408,     0,     0,     0,
       0,     0,     0,   403,   404,     0,   409,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,   406,
     410,   411,   412,   413,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,   414,     0,     0,     0,     0,     0,
       0,     0,     0,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   108,   424,     0,   110,     0,     0,   425,   426,
       0,   427,   428,   429,   113,   114,   115,   430,   116,     0,
       0,   117,   118,   119,   120,   121,   407,     0,     0,   122,
     123,     0,   408,     0,     0,   124,     0,     0,     0,   431,
     432,     0,   409,     0,     0,   502,     0,   434,     0,     0,
       0,   289,     0,  1389,     0,     0,   410,   411,   412,   413,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
     414,     0,     0,     0,     0,     0,     0,     0,     0,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   108,   424,
       0,   110,     0,     0,   425,   426,     0,   427,   428,   429,
     113,   114,   115,   430,   116,     0,     0,   117,   118,   119,
     120,   121,     0,   395,     0,   122,   123,     0,     0,     0,
       0,   124,   396,     0,     0,   431,   432,     0,     0,     0,
       0,   502,     0,   434,     0,   397,     0,   289,     0,     0,
       0,   398,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     403,   404,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,   405,     0,     0,     0,   406,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,   398,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,   408,
       0,     0,     0,     0,     0,     0,   403,   404,     0,   409,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,   406,   410,   411,   412,   413,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,   414,     0,     0,
       0,     0,     0,     0,     0,     0,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   108,   424,     0,   110,     0,
       0,   425,   426,     0,   427,   428,   429,   113,   114,   115,
     430,   116,     0,     0,   117,   118,   119,   120,   121,   407,
       0,     0,   122,   123,     0,   408,     0,     0,   124,     0,
       0,     0,   431,   432,     0,   409,  1732,     0,   502,     0,
     434,     0,     0,     0,   289,     0,     0,     0,     0,   410,
     411,   412,   413,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,   414,     0,     0,     0,     0,     0,     0,
       0,     0,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   108,   424,     0,   110,     0,     0,   425,   426,     0,
     427,   428,   429,   113,   114,   115,   430,   116,  1232,     0,
     117,   118,   119,   120,   121,     0,     0,     0,   122,   123,
       0,     0,     0,     0,   124,     0,     0,     0,   431,   432,
       0,     0,     0,     0,   502,     0,   434,     0,     6,     0,
     289,     0,     0,  1232,     0,  -299,     0,     0,     0,  1434,
    1435,     0,  1436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1437,
       0,     0,     0,     6,     0,  1438,     0,     0,     0,     0,
    -301,     0,     0,     0,  1434,  1435,     0,  1436,     0,  1232,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1437,     0,     0,     0,     0,     0,
    1438,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1434,  1435,     0,  1436,     0,  1439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1437,     0,     0,     0,    14,     0,  1438,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1440,
    1439,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,  1630,    14,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,  1440,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    16,     0,  1439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1441,   109,     0,   110,
       0,     0,   107,     0,     0,    14,   111,   112,   113,   114,
     115,     0,   116,     0,     0,   117,  1442,   119,   120,   121,
    1440,     0,     0,   122,   123,  1443,  1444,  1445,     0,   124,
      16,  1441,   109,     0,   110,     0,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,     0,   116,   107,     0,
     117,  1442,   119,   120,   121,     0,     0,     0,   122,   123,
    1443,  1444,  1445,     0,   124,     0,     0,     0,     0,     0,
     610,     0,     0,     0,   611,     0,     0,  1441,   109,     0,
     110,     0,     0,     0,     0,     0,     0,   111,   112,   113,
     114,   115,   612,   116,     0,   613,   117,  1442,   119,   120,
     121,     0,     0,     0,   122,   123,  1443,  1444,  1445,     0,
     124,     0,     0,   889,     0,     0,     0,   611,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   890,     0,     0,     0,   612,   614,     0,   613,     0,
       0,     0,   615,   616,   617,     0,   618,   619,   620,     0,
     621,   610,     0,     0,     0,   611,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     622,     0,   623,   612,     0,     0,   613,     0,     0,   614,
       0,     0,     0,     0,     0,   615,   616,   617,     0,   618,
     619,   620,     0,   621,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   624,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   622,     0,   623,     0,   614,     0,     0,
     625,     0,     0,   615,   616,   617,     0,   618,   619,   620,
       0,   621,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   626,     0,     0,     0,   624,     0,     0,     0,
       0,   622,     0,   623,     0,     0,     0,   107,     0,     0,
       0,     0,     0,   625,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   627,   628,     0,
       0,     0,     0,     0,   624,   626,   108,   109,     0,   110,
       0,     0,     0,     0,   629,     0,   111,   112,   113,   114,
     115,   625,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,     0,     0,     0,     0,   124,
     627,   628,     0,   626,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   627,   628,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,     0,     0,     0,   629,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,     0,     0,   184,   185,   108,   109,     0,   110,
       0,     0,   186,   187,     0,     0,   111,   112,   113,   114,
     115,   188,   116,   189,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,   190,   191,   192,   193,   124
};

static const short yycheck[] =
{
       9,    16,    56,    12,     4,   106,    65,    16,    63,    18,
       0,   143,   144,   143,   144,    64,   365,   297,   153,   469,
     527,   342,   373,   373,   344,   725,    64,    86,    71,   369,
      74,   579,   433,    57,   898,    95,    90,   513,   902,   539,
     794,    92,    89,   984,    68,    54,    55,    56,    57,    58,
      59,    94,    96,  1068,   710,    64,    65,    66,  1231,    68,
    1078,    70,    71,  1078,    73,    74,   960,    91,  1307,    84,
     732,   490,  1075,    82,    98,    84,    85,    86,    93,    88,
      89,    90,    91,   783,    93,    94,    95,    96,  1449,    98,
     275,   747,    69,  1068,    58,   662,   134,   157,   136,  1068,
     285,   148,   751,  1078,  1068,  1016,    70,   351,   390,  1078,
     852,   337,  1006,   655,  1078,    92,   291,   724,   228,   883,
    1123,   571,  1068,   573,  1249,   134,   135,   136,   702,   579,
     350,   155,  1078,   786,   378,  1425,   237,  1105,   161,   148,
     318,  1128,   197,   825,   153,     6,   155,   156,   157,    15,
       8,  1068,   161,     8,  1270,     8,    10,   810,   668,     8,
     813,  1078,    46,     8,    10,    50,  1213,   222,     8,   822,
     496,   135,  1219,  1241,    46,  1243,     8,    50,  1559,  1072,
     668,  1305,     8,   318,    66,   673,  1079,    23,    49,     4,
     751,   281,   156,    64,    26,  1088,   165,   103,  1488,    31,
      26,  1355,    17,    47,    11,    31,    47,   547,   557,   558,
     559,   560,   322,  1046,  1791,    53,    54,    18,  1111,    26,
      66,     6,    20,   876,    31,   326,     9,   265,   579,   579,
     268,   101,     8,    34,    71,  1129,  1813,    52,    46,    54,
      29,    48,    26,    84,   157,   182,     4,    31,   302,     7,
     133,   151,    11,   760,   201,  1832,   265,    26,     5,   268,
     581,   140,    31,   272,   604,  1513,   275,   276,    19,    16,
    1443,  1444,   281,  1276,   132,    58,   285,   132,    79,   132,
     318,  1299,  1300,   132,  1299,  1300,    69,   132,   532,    48,
      98,    26,   132,   302,    52,     8,    31,   306,   483,     6,
    1133,    26,   202,   186,    12,   343,    31,   108,   272,   630,
    1558,   366,   350,   368,   352,  1333,   354,   281,  1333,   357,
      94,    47,    57,  1900,  1299,  1300,    64,   382,   610,   103,
    1299,  1300,   386,   376,   343,  1299,  1300,   911,   912,   176,
     277,   350,   306,   352,   188,   354,   183,   188,   357,   164,
     999,   375,  1263,  1299,  1300,   490,   132,   263,  1333,   197,
     329,  1302,  1011,  1012,  1333,   278,   375,   376,   238,  1333,
      29,  1339,  1419,  1420,   175,   384,  1492,   386,   163,    53,
      54,   955,  1299,  1300,   860,   269,  1039,  1333,   838,    26,
     173,     8,   277,   903,    31,   277,   185,   269,  1779,   278,
    1524,  1083,  1084,    26,   277,  1346,   277,   497,    31,  1563,
     690,   899,   278,   274,   212,   903,  1333,  1070,  1071,   132,
     384,   279,  1490,   277,   279,   434,   279,   707,   754,  1322,
     279,   277,   140,  1672,   279,     8,   716,  1090,   999,   279,
     272,   273,   274,   275,  1805,   281,   272,   273,   274,   275,
    1011,  1133,   490,   148,    26,  1628,   163,  1814,   482,    31,
      12,   199,   188,     4,   596,   272,   273,   274,   275,   478,
      65,   130,   496,   482,   483,   204,    94,  1783,   501,  1840,
     489,   761,   151,   804,   493,   200,   807,   496,   497,   263,
     274,   275,   501,  1067,     3,    66,    84,   552,    14,   820,
    1790,   170,   782,   272,   273,   274,   275,    24,  1250,    59,
     279,    52,   292,    66,   478,   132,    47,     0,  1253,  1644,
     300,  1882,  1828,   197,   845,   489,   185,  1884,  1282,   493,
     193,   252,  1771,   497,  1537,   276,   646,   272,   273,   274,
     275,   579,  1903,   202,   599,  1906,   597,   272,   273,   274,
     275,   146,  1555,    84,   279,   284,  1163,   742,  1605,   132,
    1921,  1867,   277,  1504,   181,   155,  1228,   887,   577,  1556,
     579,    80,  1139,   268,   583,  1149,  1150,   803,  1152,  1153,
    1122,  1345,   757,     3,  1823,  1004,  1328,  1498,   140,   207,
     208,   811,   187,  1899,   177,   111,   651,   652,    10,   729,
     188,   779,  1570,   200,   198,   148,  1007,   124,   181,    19,
    1259,   182,   177,   163,   896,   278,   654,  1352,  1857,    23,
     597,   204,   153,   164,  1517,   204,    26,   123,  1521,  1522,
     874,    31,   163,   765,  1727,   272,   273,   274,   275,   204,
       7,   182,   279,   148,   779,   654,   693,   123,   191,   272,
     273,   274,   275,    26,   663,   204,   279,   188,    31,   177,
      80,    26,  1755,   663,  1557,   204,    31,   799,   800,    26,
     204,   102,   204,   953,    31,   216,    86,    26,    47,   184,
     277,  1575,    31,   277,   693,   204,   204,    91,   164,    16,
     273,  1344,   730,  1786,   732,   278,   734,   191,   194,   278,
     272,   273,   274,   275,   204,   284,  1055,   279,   273,   718,
    1728,   254,    26,  1728,  1288,    84,   725,    31,     6,   284,
    1376,   730,    26,   732,   273,   734,   277,    31,   737,    25,
     754,   282,  1905,   742,   273,   211,  1310,    68,   793,   794,
     795,   779,   751,  1684,   175,   754,   278,   785,    67,  1690,
     759,   802,   284,  1728,   763,   273,  1077,    84,  1512,  1728,
     254,    49,    81,  1656,  1728,   774,    26,   776,  1089,  1662,
      89,    31,    86,   811,   783,  1124,   785,   277,  1871,    75,
    1872,    98,  1728,    33,  1677,    26,   105,     6,   839,  1110,
      31,  1883,   278,    26,   206,   850,    46,   246,    31,   837,
     838,  1694,   811,   988,    26,   254,   991,   124,  1308,    31,
     774,  1728,   776,   109,   204,    26,  1136,     6,   869,   870,
      31,   872,  1223,   235,   236,   802,   238,    26,   837,   838,
      49,   127,    31,   245,   246,   247,   248,   249,  1930,   251,
     159,   231,   254,   255,   256,   257,   258,    40,  1781,    67,
     262,   263,   239,   240,    83,  1331,   268,  1405,    51,    89,
      49,  1337,   839,    81,  1835,    58,   875,   844,  1269,  1004,
       6,    68,   272,   273,   274,   275,   885,   277,  1849,  1199,
     857,    26,    67,   273,   114,  1778,    31,   105,   897,   147,
      83,   120,   869,   870,  1787,   872,    81,  1830,    26,   272,
     273,   274,   275,    31,   133,    67,   279,   272,   273,   274,
     275,   875,   278,    49,   279,   272,   273,   274,   275,    81,
     105,   885,   279,   272,   273,   274,   275,   163,  1502,  1793,
     279,    26,   992,  1213,  1827,  1868,    31,   129,   239,  1219,
     149,   159,   172,   105,   129,   148,  1917,   990,   249,   993,
     280,    26,     6,   283,   190,   148,    31,   166,   272,   273,
     274,   275,   276,   131,   124,   989,  1004,   129,   272,   273,
     274,   275,   277,  1253,   159,   279,  1108,   180,  1428,   988,
     989,   990,   991,   992,   993,    24,   146,   180,    62,    63,
     999,   184,   195,   153,    33,    49,  1637,   159,   129,  1320,
      26,   277,  1011,  1012,   129,    31,   282,  1016,  1046,  1349,
    1901,   129,   272,   273,   274,   275,    26,  1908,   277,   279,
      26,    31,   204,   282,    26,    31,  1016,   187,   182,    31,
    1351,   272,   273,   274,   275,   207,   208,  1046,   279,   272,
     273,   274,   275,    26,  1935,  1619,   279,    83,    31,   231,
     272,   273,   274,   275,  1405,  1405,    66,   279,    64,    20,
      86,   272,   273,   274,   275,   276,   240,   277,  1119,   279,
     137,   138,  1352,   272,   273,   274,   275,   277,   164,   279,
     279,   276,   277,    66,  1135,   124,   170,   182,  1126,   267,
    1141,  1100,   276,   277,   102,  1133,   277,  1232,   279,  1933,
    1934,  1608,  1102,   211,    26,   213,  1115,   146,   277,    31,
     279,   277,  1812,   279,   153,   278,    10,  1126,  1248,  1249,
    1250,   277,   207,   279,  1133,    62,    64,   272,   273,   274,
     275,   139,    83,   141,   279,    57,   206,    82,  1645,  1419,
    1420,  1637,  1119,   198,   272,   273,   274,   275,   187,    18,
     157,   279,  1284,   148,  1286,  1796,   164,    26,  1135,  1800,
    1509,   277,    31,   279,  1141,    34,   277,  1808,   279,  1241,
    1811,  1243,   277,     6,   279,  1749,    86,   272,   273,   274,
     275,   189,  1231,   277,  1758,   279,   277,    66,   279,  1696,
    1228,    18,    64,  1231,  1232,    88,  1837,   278,   277,    26,
     279,    70,   148,   129,    31,  1526,   277,    34,   279,   277,
      79,   279,   129,   180,    47,  1347,   182,   252,    87,  1228,
     196,   107,  1231,  1232,   130,    64,    59,  1282,   163,   277,
     123,   279,  1241,  1874,  1243,   262,    33,   106,  1879,   108,
     277,   262,   279,    70,  1565,   215,   272,   273,   274,   275,
    1259,    84,    79,   122,  1263,   124,   125,   126,   151,    25,
      87,   154,   272,   273,   274,   275,   272,   273,   274,   275,
     272,   273,   274,   275,   277,   269,   689,   170,   691,   106,
      46,   108,   695,   696,   697,   277,   277,   279,   279,   272,
     273,   274,   275,   118,    32,   122,   278,   124,   125,   126,
    1796,    10,   207,   196,  1800,   278,   175,  1362,   277,    75,
     279,  1366,  1808,   278,  1449,  1811,   203,   204,   205,   278,
     153,   278,   278,   278,   278,  1605,  1326,  1834,    94,   278,
     163,   278,   207,   207,   278,  1796,   278,   103,   278,  1800,
     281,  1837,  1916,   109,   231,   232,   278,  1808,   175,   277,
     272,   273,   274,   275,    30,   188,   283,   278,   283,   278,
     269,   127,    10,   182,   918,  1500,   920,    66,   922,   923,
    1691,   925,   926,   927,   148,  1434,  1837,   277,  1874,    40,
      66,   277,   277,  1879,   174,   269,   283,   278,    10,   277,
      51,   200,   188,    94,    94,   277,    94,    58,    24,    24,
     269,   270,   271,   272,   273,   274,   275,    46,    10,   182,
      71,  1449,   281,  1874,   283,   216,   143,   182,  1879,   143,
     204,    94,    23,   193,   193,  1434,   145,   278,    95,   278,
     278,   281,   278,  1442,  1443,  1444,    64,    10,   129,   157,
    1449,   277,   269,   270,   271,   272,   273,   274,   275,   128,
     135,   197,   278,    23,   281,   278,   283,  1512,   279,    45,
      18,   264,   284,   210,    63,   182,   278,   198,    26,    66,
      66,   132,   194,    31,    10,   128,    34,   248,   204,   251,
     278,  1490,    20,   278,    46,   288,    30,   148,   132,  1498,
      90,  1500,    90,    24,   297,   132,   132,   263,   164,   162,
      46,   278,  1637,   163,   165,   164,    20,   102,   163,    10,
     278,  1646,    70,   277,  1644,   278,    66,   279,  1527,   180,
     277,    79,    26,   184,   192,   328,   187,  1659,   331,    87,
     206,    17,   277,   113,   195,   218,   219,   220,   221,   222,
     223,   224,   225,   131,   277,    51,    66,   277,   106,   278,
     108,    74,    58,   214,    12,   278,    62,    63,   247,   235,
     236,    10,   238,  1527,   122,    71,   124,   125,   126,   245,
     246,   247,   248,   249,  1675,   251,   278,    83,   254,   255,
     256,   257,   258,   168,   279,   279,   262,   263,   280,   279,
    1614,   279,   268,    10,   200,   186,    55,   140,   238,    55,
       8,   279,   278,    98,   279,  1614,   278,   278,   278,   278,
      54,    73,   181,   278,   278,   134,   278,   175,   134,  1628,
     278,   278,  1676,   277,   427,   134,   278,   278,   431,   432,
     433,   278,   278,   278,    89,   278,   278,  1646,   279,    20,
      77,   279,   148,   277,  1634,  1700,   168,   136,   153,  1704,
     137,   132,   132,   132,   132,   132,   132,  1666,   279,   165,
     279,  1796,  1671,    24,   278,  1800,   279,  1676,   279,   163,
    1805,  1772,   279,  1808,   180,   279,  1811,    54,   184,   181,
    1695,    39,  1741,   276,    39,    18,  1695,   276,   278,   195,
      86,    54,   276,    26,   276,   276,    86,   276,    31,   502,
     276,    34,  1837,   506,   276,  1840,   280,  1671,   214,   276,
     131,   269,   270,   271,   272,   273,   274,   275,   182,    10,
     142,    26,   188,   281,    83,   283,    47,    84,   188,    47,
      84,   279,  1741,  1742,   279,    66,    98,    70,   131,  1874,
     279,   117,   117,  1743,  1879,   279,    79,  1882,    64,   279,
     279,   279,   279,   279,    87,   279,   197,    98,   279,    86,
      90,   279,   279,   276,   279,   277,  1781,  1805,  1903,   279,
     278,  1906,  1781,   106,    86,   108,   279,   114,   129,   279,
     279,   279,   279,   156,   276,   278,  1921,   283,   278,   122,
     278,   124,   125,   126,   277,   181,  1805,   129,    50,    89,
     129,   276,  1840,  1812,  1819,  1814,   279,   279,   279,   277,
    1819,    47,   279,   133,   277,  1830,     6,   188,    53,     9,
      39,  1830,    50,    13,   278,   209,    83,   279,   278,   277,
     273,  1840,    22,   279,    12,  1246,  1890,   276,   285,   742,
    1139,  1856,   175,   658,  1882,    35,   888,  1856,  1125,  1136,
    1546,    41,    42,  1868,   604,   340,  1058,  1126,   821,  1868,
    1133,   785,  1545,   370,   333,  1903,  1609,   324,  1906,  1524,
     673,  1821,  1887,  1882,  1526,  1884,  1825,  1859,  1887,    69,
    1824,  1890,    72,  1921,   985,  1747,  1734,   690,   811,  1806,
    1632,   694,   730,  1041,  1903,  1781,  1905,  1906,   701,   352,
    1505,   352,  1911,   706,   707,   345,  1278,   338,   585,   867,
     100,   101,  1921,   716,   717,   357,   719,   720,   721,   722,
     723,   724,   112,   643,  1933,  1934,   116,     6,  1335,   859,
      13,   310,   515,   774,   787,   146,   269,   270,   271,   272,
     273,   274,   275,   761,   519,  1759,   279,  1374,  1375,  1654,
    1016,  1887,  1856,  1380,  1381,     0,  1383,  1384,   761,     0,
       0,   527,   750,  1008,  1391,  1392,  1393,  1394,    47,  1396,
    1397,  1398,  1399,  1400,  1401,  1290,  1177,  1221,  1115,   782,
      59,   171,  1666,   173,   787,    88,   789,  1442,    -1,   179,
      -1,   583,    -1,    -1,    73,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   153,    -1,    -1,    -1,   268,    -1,
      -1,    -1,   272,   273,   163,    -1,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,   188,
     913,   914,   915,    -1,    -1,   918,   919,   920,    -1,   922,
     923,   924,   925,   926,   927,   928,   929,   206,   931,    -1,
     933,    -1,    -1,   936,   937,   938,   939,   940,   941,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    47,    -1,
     953,    -1,    31,   956,    -1,    34,   235,   236,    -1,   238,
      59,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    84,    -1,    -1,    -1,   268,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,  1007,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   108,
      -1,    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,    -1,    -1,   122,    -1,   124,   125,   126,    22,    -1,
      -1,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,   163,    -1,    -1,    41,    42,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    69,   175,    -1,    72,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,   206,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   100,   101,   262,   263,
      33,    -1,    -1,    -1,   268,    -1,   235,   236,   112,   238,
      -1,    -1,   116,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
    1163,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
      -1,    -1,  1175,  1176,    -1,  1178,  1179,    -1,    -1,    -1,
      -1,    -1,    -1,  1186,    -1,    -1,    -1,    -1,    -1,    -1,
     269,   270,   271,   272,   273,   274,   275,   171,    -1,   173,
    1203,    -1,    -1,    -1,    -1,   179,    -1,    -1,  1211,  1212,
    1213,    -1,    -1,    -1,    -1,   189,  1219,    -1,  1221,  1222,
    1223,  1224,  1225,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
    1253,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,  1269,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,
    1303,    -1,    -1,   206,   278,     6,   280,    -1,     9,    -1,
     284,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,
      41,    42,   245,   246,   247,   248,   249,    -1,   251,  1352,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,
    1373,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1382,
      -1,    -1,    -1,    -1,    -1,    -1,  1389,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,  1419,  1420,    -1,    19,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      30,    -1,    -1,    -1,    -1,    -1,  1439,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     171,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,   189,    -1,
      -1,    91,    92,    93,    -1,    95,    96,    97,  1501,    99,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,   119,
      -1,   121,    -1,    42,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,   152,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,   169,
      -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,   280,
    1583,  1584,    -1,   284,    -1,    -1,    -1,     6,    -1,    -1,
       9,   191,    -1,    -1,    13,    -1,    -1,  1600,    -1,    -1,
      -1,    -1,  1605,    22,    -1,    -1,   206,    -1,    -1,    -1,
    1613,    -1,    -1,    -1,  1617,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    41,    42,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,
      -1,    -1,    -1,   243,    -1,   245,   246,   247,   248,   249,
      69,   251,    -1,    72,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,     6,    -1,   206,     9,    -1,
      -1,    -1,    13,   112,    -1,    -1,  1699,   116,    -1,    -1,
      -1,    22,    -1,    -1,  1707,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,   235,   236,    -1,   238,
      41,    42,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,  1734,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    69,   268,
      -1,    72,   171,    -1,   173,   274,  1759,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,   112,    -1,    -1,    -1,   116,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,  1807,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
     171,    -1,   173,   272,   273,    -1,    -1,    -1,   179,   278,
      -1,   280,    -1,    -1,    -1,   284,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,     6,   268,    -1,     9,
      -1,   272,   273,    13,    -1,    -1,    -1,   278,    -1,   280,
      -1,    -1,    22,   284,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    -1,     7,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    69,
      -1,    -1,    72,    43,    -1,    45,    -1,    47,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,   206,    73,     6,    -1,    -1,     9,    -1,    -1,
      -1,    13,   112,    -1,    84,    -1,   116,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,    41,
      42,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,    -1,
      72,   171,    -1,   173,   278,    -1,    -1,    -1,    -1,   179,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   189,
      -1,   161,    -1,   163,   164,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
     112,    -1,    -1,    -1,   116,    -1,    -1,   217,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    74,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,   237,   268,   171,
      -1,   173,   272,   273,    -1,    -1,    -1,   179,   278,    -1,
     280,    -1,   252,   253,   284,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      69,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,     6,   268,   189,     9,    -1,
     272,   273,    13,    -1,    -1,    -1,   278,    -1,   280,    98,
      -1,    22,   284,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    69,   251,
      -1,    72,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
     189,   112,    -1,    -1,    -1,   116,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,    -1,   207,   208,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,    60,    -1,    -1,    -1,
      -1,    -1,   241,   242,    -1,    69,    -1,    -1,    72,    -1,
     171,   250,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,   273,    -1,   100,   101,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,   217,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,   163,
      -1,   262,   263,   167,    -1,    89,    -1,   268,    -1,   173,
      -1,   272,   273,    -1,    -1,   179,    -1,   278,    -1,   280,
      -1,    -1,    -1,   284,    -1,   189,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,   172,    -1,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
      -1,    13,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,
      22,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,
     284,    -1,   206,    35,    -1,    -1,    38,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,
      -1,   235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,
      72,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,   180,    -1,   268,    -1,    -1,    13,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   206,    35,
      -1,    -1,   124,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    69,    -1,    -1,    72,   245,   246,   247,
     248,   249,    -1,   251,    -1,   167,   254,   255,   256,   257,
     258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,
     268,    -1,    -1,    -1,   100,   101,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
     116,   203,   204,   205,   206,   207,   208,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,   167,   254,   255,   256,   257,   258,   173,    -1,    -1,
     262,   263,    -1,   179,    -1,    -1,   268,    -1,    -1,    -1,
     272,   273,    -1,   189,    -1,    -1,   278,    -1,   280,    -1,
      -1,    -1,   284,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    13,    -1,   262,   263,   189,    -1,
      -1,    -1,   268,    22,    -1,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,    -1,   280,   206,    35,    -1,   284,    -1,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      69,    -1,    -1,    72,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,
      13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   206,    35,    -1,    -1,   124,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,   167,   254,
     255,   256,   257,   258,   173,    -1,    -1,   262,   263,    -1,
     179,    -1,    -1,   268,    -1,    -1,    -1,   100,   101,    -1,
     189,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,   203,   204,   205,   206,   207,   208,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,   167,   254,   255,   256,   257,   258,
     173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,
      -1,    -1,    -1,   272,   273,    -1,   189,    -1,    -1,   278,
      -1,   280,    -1,    -1,    -1,   284,    -1,    74,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    13,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,
     273,   148,    -1,    -1,    -1,   278,    -1,   280,    -1,    35,
      -1,   284,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
      -1,    -1,   189,    69,    -1,    -1,    72,    -1,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,
      -1,    -1,    98,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,   112,    -1,   235,   236,
     116,   238,    -1,    22,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    35,   254,   255,   256,
     257,   258,    41,    42,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,   163,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,   173,    -1,   272,   273,   274,    -1,
     179,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    13,   206,   262,   263,    -1,    -1,    -1,    -1,   268,
      22,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    35,    -1,   284,    -1,    -1,    -1,    41,
      42,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    69,   262,   263,
      72,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   280,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    13,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,    -1,    -1,   235,   236,    35,
     238,    -1,    -1,    -1,    -1,    41,    42,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
     268,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,
     278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
     116,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,   173,    -1,    -1,
     262,   263,    -1,   179,    -1,    -1,   268,    -1,    -1,    -1,
     272,   273,    -1,   189,    -1,    -1,   278,    -1,   280,    -1,
      -1,   197,   284,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    13,   206,   262,   263,    -1,    -1,
      -1,    -1,   268,    22,    -1,    -1,   272,   273,    -1,    -1,
      -1,    -1,   278,    -1,   280,    -1,    35,    -1,   284,    -1,
      -1,    -1,    41,    42,   235,   236,    -1,   238,    -1,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      69,   262,   263,    72,    -1,    -1,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
     235,   236,    35,   238,    -1,    -1,    -1,    -1,    41,    42,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,    -1,    -1,    69,    -1,    -1,    72,
      -1,   276,    -1,    -1,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
     173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,
      -1,    -1,    -1,   272,   273,    -1,   189,    -1,    -1,   278,
      -1,   280,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    13,   206,   262,
     263,    -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,
     273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    35,
      -1,   284,    -1,    -1,    -1,    41,    42,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    69,   262,   263,    72,    -1,    -1,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   276,    -1,
      -1,    -1,   204,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    13,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,   112,    -1,    -1,    -1,
     116,    -1,    -1,   235,   236,    35,   238,    -1,    -1,    -1,
      -1,    41,    42,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    69,
      -1,   273,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    -1,   116,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,   173,    -1,    -1,   262,   263,    -1,   179,
      -1,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,   189,
      -1,    -1,   278,   279,   280,    -1,    -1,    -1,   284,    -1,
     200,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      13,   206,   262,   263,    -1,    -1,    -1,    -1,   268,    22,
      -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    35,    -1,   284,    -1,    -1,    -1,    41,    42,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    69,   262,   263,    72,
      -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,   274,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,   235,   236,    35,   238,
      -1,    -1,    -1,    -1,    41,    42,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,   173,    -1,    -1,   262,
     263,    -1,   179,    -1,    -1,   268,    -1,    -1,    -1,   272,
     273,    -1,   189,    -1,    -1,   278,    -1,   280,    -1,    -1,
      -1,   284,    -1,   200,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    13,    -1,   262,   263,    -1,    -1,    -1,
      -1,   268,    22,    -1,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,    35,    -1,   284,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,   116,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,   173,
      -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,    -1,
      -1,    -1,   272,   273,    -1,   189,   276,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    17,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,
      -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    47,    -1,
     284,    -1,    -1,    17,    -1,    54,    -1,    -1,    -1,    58,
      59,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    47,    -1,    84,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    -1,    58,    59,    -1,    61,    -1,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    -1,    61,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,   163,    -1,    84,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   197,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,   206,    -1,    -1,   163,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
     178,    -1,    -1,   262,   263,   264,   265,   266,    -1,   268,
     188,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,   206,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    23,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    41,   251,    -1,    44,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,   264,   265,   266,    -1,
     268,    -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    85,    -1,    44,    -1,
      -1,    -1,    91,    92,    93,    -1,    95,    96,    97,    -1,
      99,    19,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,   121,    41,    -1,    -1,    44,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,    85,    -1,    -1,
     169,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   191,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,   119,    -1,   121,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,
      -1,    -1,    -1,    -1,   152,   191,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,   243,    -1,   245,   246,   247,   248,
     249,   169,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
     226,   227,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   243,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,    -1,    -1,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,    -1,   245,   246,   247,   248,
     249,   250,   251,   252,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,   264,   265,   266,   267,   268
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     7,    28,    36,    43,    45,    47,    52,    59,    73,
      84,   151,   158,   161,   163,   164,   188,   237,   252,   253,
     286,   287,   288,   297,   307,   317,   319,   321,   452,   453,
     479,   501,   525,   526,   527,   528,   529,   531,   533,   534,
     538,   555,   556,   557,   568,   609,   610,   611,   612,   614,
     615,   616,   621,    40,    51,    58,    83,   148,   180,   184,
     480,   201,   535,    40,    51,    58,    71,   132,   148,   165,
     180,   184,   187,   195,   214,   318,   323,   204,   273,   668,
     669,    62,    63,   308,    66,    51,    58,    62,    63,    71,
      83,   148,   165,   180,   184,   195,   214,   502,   148,   268,
       6,    47,    59,    84,   153,   163,   188,   206,   235,   236,
     238,   245,   246,   247,   248,   249,   251,   254,   255,   256,
     257,   258,   262,   263,   268,   290,   291,   292,   293,   303,
     304,   714,   722,   723,    19,    86,    19,    73,   290,   292,
     298,   303,   535,   239,   240,   574,   575,   576,    71,   176,
     183,   592,   593,   716,   722,   148,   180,   195,   320,   721,
     722,   252,     0,   276,   133,   186,   558,   494,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   233,   234,   241,   242,   250,   252,
     264,   265,   266,   267,   487,   488,   722,   706,   722,   493,
     710,   722,   402,   713,   722,   593,   716,   498,   717,   722,
     155,   536,   207,   208,   342,   344,   667,   331,   377,   633,
     702,   722,   706,   340,   709,   722,     7,   399,   713,   325,
     669,   671,   367,   593,   471,   717,   457,   720,   722,   341,
     714,    11,    48,   561,   204,   670,    68,   316,   707,   722,
     592,   705,   722,   706,    68,   707,   709,   710,   713,   671,
     716,   717,   720,   714,   713,   278,   454,   147,   278,   626,
     627,   626,   129,   277,   129,   182,   277,   633,   593,   633,
     131,   129,   129,   290,   292,    66,   182,   204,   278,   284,
     663,   674,   278,   663,   674,     6,    49,   577,   696,   571,
     576,   709,    83,    88,   123,   151,   154,   170,   196,   539,
     540,   541,   542,   543,   544,   545,   546,   548,   164,   715,
     722,   400,   713,   368,   593,   458,   720,   721,    20,     6,
     568,   267,   564,     4,    17,    52,    54,   164,   495,   496,
       4,    52,   164,   182,   216,   483,   484,   667,     3,    80,
     278,   403,     4,     7,    52,   485,   486,   489,     3,    80,
     473,   170,   537,   207,   102,   139,   141,   164,   189,   345,
     346,   347,    10,   332,   667,   148,   184,   322,   403,    14,
     111,   326,    62,   369,    64,   626,    83,   206,   309,   698,
      82,   198,   599,   613,   698,    13,    22,    35,    41,    42,
      69,    72,    98,   100,   101,   112,   116,   173,   179,   189,
     203,   204,   205,   206,   217,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   236,   241,   242,   244,   245,   246,
     250,   272,   273,   278,   280,   435,   436,   619,   620,   632,
     633,   656,   657,   658,   661,   662,   663,   664,   665,   666,
     667,   675,   676,   677,   678,   679,   680,   681,   683,   684,
     685,   686,   687,   688,   690,   697,   698,   715,   722,   378,
     408,   455,   456,   633,   157,   404,   628,   633,   180,   289,
     593,   293,   148,   189,   305,   306,   718,   722,   304,    86,
     278,   629,   630,    66,    64,   289,   148,   129,   129,   305,
     252,   532,   278,   656,   570,   570,   571,   274,   578,   579,
     580,   619,   571,   182,   710,   107,   196,    29,   130,   185,
     202,   551,   552,   554,   593,   180,   541,   617,   618,   633,
     634,   715,   403,   369,   626,   130,   530,   559,   560,   656,
     568,   656,    64,   565,    94,   103,   263,   330,   352,   353,
     354,   355,   361,   262,   103,   263,   262,    25,    75,   109,
     127,   351,   497,   496,    33,   339,   468,    33,    46,   468,
     633,   380,   484,   378,   405,   406,   404,    33,   372,   377,
     387,   388,   215,    33,   633,   277,   487,   491,   633,   722,
       5,    16,   474,   475,   499,   269,   343,   343,   667,   118,
     667,    46,   263,   348,   349,   350,   351,   352,   355,   347,
      19,    23,    41,    44,    85,    91,    92,    93,    95,    96,
      97,    99,   119,   121,   152,   169,   191,   226,   227,   243,
     503,   504,   505,   508,   509,   510,   511,   515,   516,   517,
     518,   519,   520,   522,   523,   401,   713,   472,   717,   404,
      32,   327,    94,   667,   278,   593,    10,   710,   278,   311,
     312,   313,   380,   177,   513,   668,    38,    60,   124,   167,
     256,   257,   258,   278,   635,   637,   638,   639,   640,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   656,   278,
     278,   278,   207,   278,   278,   278,   278,   278,   278,   207,
     207,   278,   197,   656,   691,   693,   278,   278,   656,   656,
     163,   620,   655,   656,   719,   722,   277,   281,    26,    31,
     272,   273,   274,   275,   278,   283,   504,   509,   516,   269,
     277,   279,   278,    10,   277,   279,   593,   182,   296,   713,
     718,   200,   277,   295,   593,   631,   634,   163,   190,   569,
     593,    66,   296,   289,   148,   721,   656,   279,   656,    66,
     582,   277,    10,   581,   231,   273,   673,   674,   151,   545,
     123,   194,   547,   547,   277,    64,   277,   553,   174,   277,
     599,   613,   269,   283,   404,   278,    10,   277,   561,   182,
     188,   200,   567,    15,   278,    94,   355,   667,    94,   343,
     343,   343,   343,   277,    24,   482,    24,    46,   481,   504,
     504,   277,   279,    10,   703,   722,    30,   373,   374,   375,
     503,   504,   633,    65,   146,   187,   389,   390,   391,   392,
     394,   468,   703,   211,   213,   492,   486,   182,   216,   143,
      47,    84,   188,   476,   143,   477,   674,   671,   667,    23,
      94,   350,   278,   513,   193,   193,   145,   278,   524,    95,
     278,   521,    23,    91,   468,   281,    23,   281,   514,   278,
     278,   193,   278,   521,   403,    64,    10,   667,   667,   370,
     371,   372,   387,   473,   466,   129,   312,   157,   277,    19,
      37,   310,   508,   668,   699,   722,   135,   128,   278,   124,
     637,   650,   278,   124,   259,   260,   261,   636,   637,   651,
     656,     8,   132,    18,    34,    70,    79,    87,   106,   108,
     122,   124,   125,   126,   175,   269,   270,   271,    49,   696,
     619,    49,   274,   696,   709,   656,    49,   696,    49,   696,
      49,   696,   218,   219,   220,   221,   222,   223,   224,   225,
     695,   656,   635,    53,    54,   197,   197,   689,   656,   619,
     574,   279,   279,   279,   619,   656,   659,   701,   722,   656,
     656,   656,   656,   656,   279,   659,    42,   274,   702,   663,
     456,   407,   408,    45,   410,   411,   412,   633,    74,   148,
     184,   189,   195,   214,   299,   300,   301,   302,   718,   182,
     210,   306,    63,   622,   277,   279,   574,   278,   622,   299,
     301,    66,    66,   296,   570,   279,   278,   583,   584,   585,
     589,   590,   713,   198,   599,   580,   711,   722,   231,   674,
     194,   552,   149,   166,   549,   593,   618,   619,   702,    10,
     370,   466,   560,   248,   563,   656,   128,   566,   251,   357,
     358,   667,   356,   357,   667,   102,   175,   362,   363,   364,
     674,   674,   204,   672,   671,   351,   468,   278,   468,   469,
      46,   269,   409,   466,   406,   466,    20,   278,    46,   381,
     468,   373,   466,    90,    90,   627,    24,   633,   490,   504,
     633,   671,   132,   132,   132,   669,   468,   137,   138,   366,
     164,   667,   277,   672,   162,   512,   669,   524,   273,   673,
      46,   333,   506,   507,   673,   164,   506,   671,   671,   278,
     671,   404,   593,   466,   102,   328,   277,   279,   474,   163,
     459,   460,   461,   593,   279,   140,   278,   314,   315,   380,
     313,   278,    20,   513,   704,   722,   569,   569,   650,     8,
     132,   279,     8,   132,   635,   635,   656,   656,     6,     9,
     171,   641,   656,   278,   652,   653,    98,   124,     6,   641,
     656,   656,     6,   641,   656,    18,    34,    79,   108,   175,
       6,   641,   656,     6,   641,   656,   200,   656,     6,   641,
     656,     6,   641,   656,     6,   641,   656,   656,   656,    10,
     656,   279,   656,   277,   279,   656,   656,   656,   656,   656,
     656,    66,    66,   181,   619,   694,   635,   656,   692,    53,
      54,   197,   277,   277,   577,   277,   282,   279,   277,   279,
     192,   415,    17,   419,   412,   718,   713,   717,   718,   720,
     714,   277,   200,   277,   294,   299,   301,   131,    66,   182,
     113,   624,   634,   577,   620,   624,   299,   301,   301,    66,
     556,   584,   589,   277,    67,    81,   105,   159,   594,   278,
     591,    74,   595,   151,   202,   550,   466,   279,   459,   572,
     628,   359,   277,   279,   343,    12,   365,   364,   278,   635,
     273,   382,   657,   660,   662,   664,   665,   666,   697,   466,
     466,   469,   410,   468,   382,   469,   627,   627,   247,   393,
     278,   469,   468,   466,    84,   188,    47,   188,    47,    84,
      10,   478,   500,   700,   722,   668,   277,   279,   168,   514,
     279,   277,   279,   468,   469,   277,   282,   280,   700,   282,
     279,   279,   671,   279,    10,   473,   410,   343,   329,   330,
     371,   477,   577,   200,   470,   186,   324,   374,   627,   628,
     671,   315,    55,   310,   671,   238,    55,   279,   279,   635,
     635,   635,   635,     8,   278,   278,   163,   654,   659,    98,
     278,   278,    57,   278,   278,   656,   656,   652,   656,   200,
     656,   278,   278,   278,   278,   656,   278,   278,   278,   278,
     278,   278,   279,   279,   379,   380,   279,   279,   656,   279,
     279,   279,   279,   279,   279,   656,   656,   694,    54,   181,
     181,   694,   692,   656,   620,   656,   656,   408,   377,   413,
     414,   417,   704,   722,    58,    59,    61,    78,    84,   144,
     178,   235,   255,   264,   265,   266,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   428,   429,   430,   431,   432,
     434,   439,   440,   441,   447,   448,   449,   450,   556,   609,
     610,   614,   618,   712,   722,   300,   302,    73,   300,   302,
     294,   513,   623,   663,   623,   625,   663,   672,   578,   279,
     277,   301,   279,   279,   584,   134,   134,   134,    89,   620,
     581,    20,    77,   598,   410,   470,   239,   249,   562,   168,
     360,   357,   136,   674,   137,   674,   635,   279,   661,   382,
     382,   419,   656,   383,   384,   385,   388,   153,   393,   561,
     635,   132,   132,   132,   132,   132,   132,   468,   469,   279,
     668,   672,   673,   382,   124,   334,   335,   336,   337,   338,
     339,   507,   673,   514,   279,   466,   474,   419,   674,   468,
     578,    24,   467,     6,   461,   278,   279,   667,    20,   212,
     279,   667,   656,   654,   654,   574,   279,   279,   654,   654,
     656,   654,   654,     8,    57,   656,   654,   654,   654,   654,
     654,   654,   654,   654,   654,   654,   279,   503,   279,   279,
      64,   682,   694,   694,    54,   181,   279,   279,   582,   504,
     276,    39,   276,   706,   148,   191,   254,   427,   276,   278,
     656,   276,   276,   276,   712,   722,   704,   704,   451,    54,
     197,   418,   443,   444,    64,   199,   276,    86,   276,   276,
     276,   276,   280,   131,   182,   582,   581,   584,   279,   715,
     596,   597,   656,   635,   142,   600,   419,   467,   573,   343,
     357,   366,   279,   469,   469,   469,    26,   376,   385,   124,
     146,   153,   187,   386,   394,   593,    83,   279,   188,    84,
     188,    47,    84,    47,   410,   279,   279,    98,   376,   336,
     410,   477,   469,   366,   478,    66,   462,   131,   461,   468,
     117,   101,   238,   514,   117,   279,   279,   577,   279,   279,
     279,   279,   656,   656,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   656,   279,   694,   599,   269,   381,
     416,    64,   276,   656,   713,   656,   635,   433,   276,   704,
       9,    58,    69,   173,   445,   446,   444,    59,   556,   278,
     435,   438,   632,   715,   623,   599,   586,   715,   129,   277,
      89,   114,   172,   601,   602,   469,   674,   469,   279,   701,
      98,    90,   593,   393,   626,   710,   469,   419,   419,   468,
     469,   278,   463,   464,   465,   592,   599,   656,   667,   667,
     656,   595,   382,   278,   276,   436,    86,   276,   279,   276,
      86,   706,   708,   722,   668,    50,   277,   427,    86,   635,
     276,   277,   283,   595,   278,   587,   635,   597,   114,   278,
     393,   626,   129,   395,   396,   397,   469,   478,   464,   465,
     277,   594,   595,   469,   582,   598,   569,   156,   437,   438,
     181,   438,   418,   446,   656,   438,   279,   435,   632,   598,
     588,   711,   601,   603,   604,   605,   716,   395,    47,   188,
     393,   129,   397,   129,   396,   469,   279,   464,    89,   598,
     279,   599,   600,   279,   278,   438,   276,   276,   418,    86,
      10,   442,    50,   600,   277,   279,   279,   277,    83,   120,
     133,   606,   605,   393,   123,   164,   211,   398,   398,   464,
     595,   558,   438,    53,   438,    39,    50,   418,   558,   711,
     603,   278,   710,   209,    46,    98,   129,   598,   564,   279,
     418,    50,   704,   418,   564,   607,   710,    83,   608,   635,
     600,   418,   279,   277,   278,   558,   607,   607,   564,   279
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(DSQL_yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (DSQL_yychar == YYEMPTY && yylen == 1)				\
    {								\
      DSQL_yychar = (Token);						\
      yylval = (Value);						\
      DSQL_yychar1 = YYTRANSLATE (DSQL_yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#define YYLEX	yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (lex.dsql_debug)					\
    YYFPRINTF Args;				\
} while (0)
# define YYDSYMPRINT(Args)			\
do {						\
  if (lex.dsql_debug)					\
    yysymprint Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yysymprint (FILE* yyout, int yytype, YYSTYPE yyvalue)
#else
yysymprint (yyout, yytype, yyvalue)
    FILE* yyout;
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[yytype], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yydestruct (int yytype, YYSTYPE yyvalue)
#else
yydestruct (yytype, yyvalue)
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  switch (yytype)
    {
      default:
        break;
    }
}



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into dsql_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int dsql_yyparse (void *);
# else
int dsql_yyparse (void);
# endif
#endif


/* The lookahead symbol.  */

int DSQL_yychar;


/* The semantic value of the lookahead symbol.  */
static YYSTYPE yylval;

/* Number of parse errors so far.  */
static int yynerrs;


int
dsql_yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int DSQL_yychar1 = 0;

  /* Three stacks and their tools:
     `DSQL_yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	DSQL_yyssa[YYINITDEPTH];
  short *DSQL_yyss = DSQL_yyssa;
  register short *DSQL_DSQL_yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, DSQL_DSQL_yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  DSQL_yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  DSQL_DSQL_yyssp = DSQL_yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  DSQL_DSQL_yyssp++;

 yysetstate:
  *DSQL_DSQL_yyssp = yystate;

  if (DSQL_DSQL_yyssp >= DSQL_yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = DSQL_DSQL_yyssp - DSQL_yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *DSQL_yyss1 = DSQL_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &DSQL_yyss1, yysize * sizeof (*DSQL_DSQL_yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	DSQL_yyss = DSQL_yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *DSQL_yyss1 = DSQL_yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (DSQL_yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (DSQL_yyss1 != DSQL_yyssa)
	  YYSTACK_FREE (DSQL_yyss1);
      }
# endif
#endif /* no yyoverflow */

      DSQL_DSQL_yyssp = DSQL_yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (DSQL_DSQL_yyssp >= DSQL_yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* DSQL_yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (DSQL_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      DSQL_yychar = YYLEX;
    }

  /* Convert token to internal form (in DSQL_yychar1) for indexing tables with.  */

  if (DSQL_yychar <= 0)		/* This means end of input.  */
    {
      DSQL_yychar1 = 0;
      DSQL_yychar = YYEOF;		/* Don't call YYLEX any more.  */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      DSQL_yychar1 = YYTRANSLATE (DSQL_yychar);

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, DSQL_yychar1, yylval));
      YYDPRINTF ((stderr, "\n"));
    }

  /* If the proper action on seeing token YYCHAR1 is to reduce or to
     detect an error, take that action.  */
  yyn += DSQL_yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != DSQL_yychar1)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      DSQL_yychar, yytname[DSQL_yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (DSQL_yychar != YYEOF)
    DSQL_yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];



#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (lex.dsql_debug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn - 1, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] >= 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    {
        case 2:
    { DSQL_parse = yyvsp[0]; }
    break;

  case 3:
    { DSQL_parse = yyvsp[-1]; }
    break;

  case 23:
    { prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
    break;

  case 24:
    { yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;

  case 25:
    { yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;

  case 26:
    { yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-4], yyvsp[-2], make_list(yyvsp[0]), NULL); }
    break;

  case 27:
    { yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), NULL); }
    break;

  case 28:
    { yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
    break;

  case 30:
    { yyval = yyvsp[0]; }
    break;

  case 31:
    { yyval = make_node (nod_all, (int) 0, NULL); }
    break;

  case 32:
    { yyval = make_node (nod_all, (int) 0, NULL); }
    break;

  case 33:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 35:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 36:
    { yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
    break;

  case 37:
    { yyval = make_node (nod_select, (int) 0, NULL); }
    break;

  case 38:
    { yyval = make_node (nod_insert, (int) 0, NULL); }
    break;

  case 39:
    { yyval = make_node (nod_delete, (int) 0, NULL); }
    break;

  case 40:
    { yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
    break;

  case 41:
    { yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
    break;

  case 42:
    { yyval = make_node (nod_grant, (int) 0, NULL); }
    break;

  case 43:
    { yyval = 0; }
    break;

  case 44:
    { yyval = make_node (nod_grant_admin, (int) 0, NULL); }
    break;

  case 45:
    { yyval = 0; }
    break;

  case 46:
    { yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
    break;

  case 47:
    { yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-5]); }
    break;

  case 48:
    { yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-6]); }
    break;

  case 49:
    { yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;

  case 50:
    { yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;

  case 51:
    { yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;

  case 52:
    { yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
    break;

  case 53:
    { yyval = make_node (nod_revoke, 
				(int) e_grant_count, make_list(yyvsp[-2]), make_list(yyvsp[0]),
				NULL, NULL); }
    break;

  case 54:
    { yyval = make_node (nod_grant, (int) 0, NULL); }
    break;

  case 56:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 57:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 58:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 59:
    { yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); }
    break;

  case 60:
    { yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); }
    break;

  case 61:
    { yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); }
    break;

  case 62:
    { yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
    break;

  case 64:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 65:
    { yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
    break;

  case 66:
    { yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); }
    break;

  case 67:
    { yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); }
    break;

  case 69:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 70:
    { yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
    break;

  case 72:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 73:
    { yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
    break;

  case 74:
    { yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
    break;

  case 75:
    { yyval = yyvsp[0];}
    break;

  case 76:
    { yyval = yyvsp[0]; }
    break;

  case 77:
    { yyval = yyvsp[0]; }
    break;

  case 78:
    { yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); }
    break;

  case 80:
    { lex.g_field->fld_dtype = dtype_blob; }
    break;

  case 81:
    { 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-2]; }
    break;

  case 82:
    { yyval = NULL; }
    break;

  case 84:
    { yyval = yyvsp[-1]; }
    break;

  case 86:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 87:
    { yyval = make_node (nod_udf_param, (int) e_udf_param_count,
							  yyvsp[-1], NULL); }
    break;

  case 88:
    { yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				yyvsp[-3], MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG)); }
    break;

  case 90:
    { yyval = yyvsp[-1]; }
    break;

  case 91:
    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((dsql_str*) FUN_reference, CONSTANT_SLONG));}
    break;

  case 92:
    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((dsql_str*) (-1 * FUN_reference), CONSTANT_SLONG));}
    break;

  case 93:
    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((dsql_str*) FUN_value, CONSTANT_SLONG));}
    break;

  case 94:
    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((dsql_str*) FUN_descriptor, CONSTANT_SLONG));}
    break;

  case 95:
    { yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
    break;

  case 96:
    { yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 97:
    { yyval = yyvsp[0]; }
    break;

  case 98:
    { yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 99:
    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
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
    { yyval = yyvsp[0]; }
    break;

  case 116:
    { yyval = make_node (nod_unique, 0, NULL); }
    break;

  case 117:
    { yyval = NULL; }
    break;

  case 118:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 120:
    { yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;

  case 121:
    { yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 122:
    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 123:
    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 124:
    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 125:
    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 126:
    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 127:
    { yyval = (dsql_nod*) 0;}
    break;

  case 128:
    { yyval = yyvsp[-1]; }
    break;

  case 129:
    { yyval = NULL; }
    break;

  case 132:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 133:
    { yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 134:
    { yyval = NULL; }
    break;

  case 135:
    { yyval = NULL; }
    break;

  case 136:
    { yyval = yyvsp[0]; }
    break;

  case 137:
    { yyval = NULL; }
    break;

  case 138:
    { yyval = NULL; }
    break;

  case 141:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 142:
    { yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;

  case 145:
    { yyval = make_node (nod_null, (int) 0, NULL); }
    break;

  case 146:
    { yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 147:
    { yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
    break;

  case 148:
    { yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
    break;

  case 149:
    { yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;

  case 152:
    { lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (dsql_nod*) yyvsp[0]; }
    break;

  case 153:
    {yyval = NULL;}
    break;

  case 156:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 157:
    { yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;

  case 158:
    { yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;

  case 159:
    { yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;

  case 160:
    { yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;

  case 161:
    { yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;

  case 162:
    {yyval = NULL;}
    break;

  case 165:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 169:
    { yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;

  case 170:
    { yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;

  case 171:
    { yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
    break;

  case 172:
    { yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
    break;

  case 173:
    { yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
    break;

  case 174:
    { yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
    break;

  case 175:
    { if (lex.log_defined)
				yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;

  case 176:
    { if (lex.log_defined)
				yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;

  case 177:
    { lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 178:
    { lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
    break;

  case 179:
    { lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(dsql_nod*) lex.g_file);}
    break;

  case 180:
    { lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;

  case 182:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 183:
    { 
			 yyval = (dsql_nod*) make_node (nod_log_file_desc, (int) 1,
												(dsql_nod*) lex.g_file); }
    break;

  case 184:
    { lex.g_file = make_file();
			  lex.g_file->fil_name = (dsql_str*) yyvsp[0]; }
    break;

  case 187:
    { lex.g_file->fil_length = (IPTR) yyvsp[0]; }
    break;

  case 188:
    { lex.g_file  = make_file ();}
    break;

  case 193:
    { lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;

  case 194:
    { lex.g_file->fil_length = (IPTR) yyvsp[-1];}
    break;

  case 201:
    { yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;

  case 202:
    { yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;

  case 203:
    { yyval = yyvsp[0]; }
    break;

  case 204:
    { yyval = yyvsp[0]; }
    break;

  case 205:
    { yyval = NULL; }
    break;

  case 207:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 210:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;

  case 211:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 212:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 213:
    { 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;

  case 216:
    { yyval = NULL; }
    break;

  case 217:
    { yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 218:
    { yyval = yyvsp[0]; }
    break;

  case 219:
    { yyval = NULL; }
    break;

  case 220:
    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 221:
    { lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 222:
    { yyval = yyvsp[-1]; }
    break;

  case 223:
    { lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 224:
    { yyval = yyvsp[0]; }
    break;

  case 225:
    { yyval = NULL; }
    break;

  case 229:
    { yyval = yyvsp[0]; }
    break;

  case 230:
    { yyval = yyvsp[0]; }
    break;

  case 231:
    { yyval = yyvsp[0]; }
    break;

  case 232:
    { yyval = NULL; }
    break;

  case 235:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 236:
    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 237:
    { yyval = make_node (nod_null, (int) 1, NULL); }
    break;

  case 238:
    { yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 240:
    { yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;

  case 241:
    { yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;

  case 242:
    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 243:
    { yyval = yyvsp[0]; }
    break;

  case 244:
    { yyval = NULL ;}
    break;

  case 249:
    { yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 250:
    { yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 251:
    { yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 252:
    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;

  case 253:
    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 254:
    { yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 255:
    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;

  case 256:
    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;

  case 257:
    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;

  case 258:
    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;

  case 259:
    { yyval = NULL;}
    break;

  case 260:
    { yyval = yyvsp[0];}
    break;

  case 261:
    { yyval = yyvsp[0];}
    break;

  case 262:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;

  case 263:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;

  case 264:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;

  case 265:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;

  case 266:
    { yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 267:
    { yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 268:
    { yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 269:
    { yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 270:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 271:
    { yyval = NULL; }
    break;

  case 272:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 273:
    { yyval = NULL; }
    break;

  case 275:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 276:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;

  case 278:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 279:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;

  case 280:
    { yyval = yyvsp[0]; }
    break;

  case 281:
    { yyval = yyvsp[0]; }
    break;

  case 282:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 283:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 284:
    { yyval = NULL; }
    break;

  case 286:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 287:
    { yyval = yyvsp[-1]; }
    break;

  case 290:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;

  case 291:
    { yyval = NULL; }
    break;

  case 292:
    { yyval = NULL; }
    break;

  case 293:
    { yyval = yyvsp[0]; }
    break;

  case 294:
    { yyval = yyvsp[0]; }
    break;

  case 295:
    { yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;

  case 298:
    { yyval = yyvsp[-1]; }
    break;

  case 299:
    { yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;

  case 300:
    { yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;

  case 301:
    { yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;

  case 303:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 319:
    { yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;

  case 320:
    { yyval = make_node (nod_exit, 0, NULL); }
    break;

  case 323:
    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;

  case 324:
    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 325:
    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;

  case 326:
    { yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;

  case 327:
    { yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;

  case 330:
    { yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;

  case 331:
    { yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;

  case 332:
    { yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;

  case 333:
    { yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;

  case 334:
    { yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;

  case 335:
    { yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 336:
    { yyval = NULL; }
    break;

  case 337:
    { yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;

  case 338:
    { yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
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

  case 342:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 343:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 344:
    { yyval = NULL; }
    break;

  case 347:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 348:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 349:
    { yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;

  case 350:
    { yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;

  case 351:
    { yyval = NULL; }
    break;

  case 352:
    { yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;

  case 353:
    { yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;

  case 354:
    { yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;

  case 355:
    { yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 356:
    { yyval = NULL; }
    break;

  case 358:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 359:
    { yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;

  case 361:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 362:
    { yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;

  case 363:
    { yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;

  case 364:
    { yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;

  case 365:
    { yyval = make_node (nod_default, 1, NULL); }
    break;

  case 369:
    { yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;

  case 370:
    { yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;

  case 371:
    { yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;

  case 372:
    { yyval = NULL; }
    break;

  case 373:
    { yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;

  case 374:
    { yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;

  case 375:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 376:
    { yyval = NULL; }
    break;

  case 378:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 379:
    { yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;

  case 380:
    { yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 381:
    { yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 382:
    { yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
    break;

  case 383:
    { yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;

  case 384:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 385:
    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 386:
    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;

  case 387:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 389:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 392:
    { yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 393:
    { yyval = yyvsp[-1]; }
    break;

  case 394:
    { lex.beginning = lex_position(); }
    break;

  case 395:
    { yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;

  case 396:
    { lex.beginning = lex.last_token; }
    break;

  case 397:
    { yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;

  case 398:
    { yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 399:
    { yyval = 0; }
    break;

  case 400:
    { yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 401:
    { yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 402:
    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 403:
    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 404:
    { yyval = NULL; }
    break;

  case 405:
    { yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;

  case 406:
    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 407:
    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 408:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;

  case 409:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;

  case 410:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;

  case 411:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;

  case 412:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;

  case 413:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;

  case 414:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;

  case 415:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;

  case 416:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;

  case 417:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;

  case 418:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;

  case 419:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;

  case 420:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;

  case 421:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;

  case 422:
    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;

  case 423:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 424:
    { yyval = NULL; }
    break;

  case 425:
    { yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 426:
    { yyval = yyvsp[0]; }
    break;

  case 427:
    { yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 428:
    { yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 429:
    { yyval = yyvsp[0]; }
    break;

  case 430:
    { yyval = yyvsp[0]; }
    break;

  case 431:
    { yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;

  case 432:
    { yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 433:
    { yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
    break;

  case 434:
    { yyval = yyvsp[0]; }
    break;

  case 435:
    { yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 437:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 438:
    { yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;

  case 439:
    { yyval = yyvsp[0]; }
    break;

  case 440:
    { yyval = yyvsp[0]; }
    break;

  case 441:
    {yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;

  case 442:
    { yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;

  case 443:
    { yyval = yyvsp[0]; }
    break;

  case 444:
    { yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;

  case 446:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 447:
    { yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 448:
    { yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;

  case 449:
    { yyval = yyvsp[0]; }
    break;

  case 450:
    { yyval = yyvsp[0]; }
    break;

  case 451:
    { yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 452:
    { yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 453:
    { yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;

  case 454:
    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 482:
    { yyval = NULL; }
    break;

  case 483:
    { yyval = NULL; }
    break;

  case 484:
    { yyval = NULL; }
    break;

  case 485:
    { yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 486:
    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 487:
    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 488:
    { yyval = make_node (nod_cascade, 0, NULL); }
    break;

  case 489:
    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 490:
    { yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;

  case 491:
    { yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;

  case 492:
    { lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
    break;

  case 494:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 495:
    { yyval = yyvsp[0]; }
    break;

  case 496:
    { yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;

  case 497:
    { yyval = yyvsp[0]; }
    break;

  case 498:
    { yyval = yyvsp[0]; }
    break;

  case 499:
    { yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;

  case 500:
    { yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;

  case 501:
    { yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;

  case 502:
    { yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;

  case 504:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
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

  case 527:
    { lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;

  case 528:
    { lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;

  case 530:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 531:
    { if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;

  case 532:
    { yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 538:
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

  case 539:
    { 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;

  case 540:
    { 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;

  case 541:
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

  case 542:
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

  case 543:
    { 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;

  case 546:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;

  case 547:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;

  case 548:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 549:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 550:
    {
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;

  case 551:
    {
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;

  case 552:
    {
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;

  case 553:
    {
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;

  case 554:
    {
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;

  case 555:
    {
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;

  case 557:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 558:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 559:
    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 560:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 561:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;

  case 562:
    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 571:
    { 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;

  case 572:
    {  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;

  case 573:
    {
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;

  case 574:
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

  case 575:
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

  case 578:
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

  case 579:
    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 580:
    { 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;

  case 581:
    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 582:
    { yyval = yyvsp[-1]; }
    break;

  case 583:
    { yyval = 0; }
    break;

  case 587:
    { 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;

  case 588:
    {
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;

  case 589:
    {
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
    break;

  case 593:
    { yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;

  case 594:
    { yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 595:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 596:
    { yyval = 0; }
    break;

  case 597:
    { yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;

  case 600:
    { yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;

  case 601:
    { yyval = make_node (nod_rollback, 0, NULL); }
    break;

  case 604:
    { yyval = make_node (nod_commit_retain, 0, NULL); }
    break;

  case 605:
    { yyval = NULL; }
    break;

  case 607:
    { yyval = NULL; }
    break;

  case 608:
    {yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;

  case 610:
    { yyval = NULL; }
    break;

  case 612:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 617:
    { yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;

  case 618:
    { yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;

  case 619:
    { yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;

  case 620:
    { yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;

  case 621:
    { yyval = yyvsp[0];}
    break;

  case 623:
    { yyval = yyvsp[0];}
    break;

  case 624:
    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 625:
    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 626:
    { yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;

  case 627:
    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 628:
    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 629:
    { yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;

  case 630:
    { yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;

  case 631:
    { yyval = 0; }
    break;

  case 632:
    { yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;

  case 633:
    { yyval = (dsql_nod*) NOD_SHARED; }
    break;

  case 634:
    { yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;

  case 635:
    { yyval = (dsql_nod*) 0; }
    break;

  case 636:
    { yyval = (dsql_nod*) NOD_READ; }
    break;

  case 637:
    { yyval = (dsql_nod*) NOD_WRITE; }
    break;

  case 639:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 640:
    { yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 641:
    { yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;

  case 642:
    { yyval = 0; }
    break;

  case 644:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 645:
    {yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;

  case 646:
    { yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 647:
    { yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;

  case 648:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 649:
    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
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

  case 654:
    { yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 655:
    { yyval = 0; }
    break;

  case 656:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 657:
    { yyval = 0; }
    break;

  case 658:
    { yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;

  case 659:
    { yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;

  case 660:
    { yyval = yyvsp[-1]; }
    break;

  case 661:
    { yyval = 0; }
    break;

  case 662:
    { yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;

  case 663:
    { yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;

  case 664:
    { yyval = NULL; }
    break;

  case 665:
    { yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;

  case 666:
    { yyval = 0; }
    break;

  case 667:
    { yyval = yyvsp[0]; }
    break;

  case 668:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 669:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 670:
    { yyval = 0; }
    break;

  case 671:
    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;

  case 672:
    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 673:
    { lex.limit_clause = true; }
    break;

  case 674:
    { lex.limit_clause = false; }
    break;

  case 675:
    { lex.first_detection = true; }
    break;

  case 676:
    { lex.first_detection = false; }
    break;

  case 677:
    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;

  case 678:
    { yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;

  case 679:
    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;

  case 680:
    { yyval = 0; }
    break;

  case 681:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;

  case 682:
    { yyval = yyvsp[-2]; }
    break;

  case 683:
    { yyval = yyvsp[-1]; }
    break;

  case 684:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 685:
    { yyval = yyvsp[-1]; }
    break;

  case 686:
    { yyval = yyvsp[0]; }
    break;

  case 687:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 688:
    { yyval = 0; }
    break;

  case 689:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 690:
    { yyval = 0; }
    break;

  case 692:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 694:
    { yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 697:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 699:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 703:
    { yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;

  case 705:
    { yyval = NULL; }
    break;

  case 706:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 707:
    { yyval = NULL; }
    break;

  case 709:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 710:
    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 711:
    { yyval = yyvsp[-1]; }
    break;

  case 712:
    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;

  case 713:
    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 714:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 715:
    { yyval = NULL; }
    break;

  case 717:
    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 718:
    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;

  case 719:
    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 720:
    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 721:
    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 722:
    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 723:
    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 724:
    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 725:
    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 726:
    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
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

  case 732:
    { yyval = yyvsp[0]; }
    break;

  case 733:
    { yyval = 0; }
    break;

  case 734:
    { yyval = yyvsp[0]; }
    break;

  case 735:
    { yyval = 0; }
    break;

  case 736:
    { yyval = yyvsp[0]; }
    break;

  case 737:
    { yyval = 0; }
    break;

  case 738:
    { yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 739:
    { yyval = 0; }
    break;

  case 740:
    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 741:
    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 742:
    { yyval = 0; }
    break;

  case 743:
    { yyval = 0; }
    break;

  case 745:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 746:
    { yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 749:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 750:
    { yyval = make_node (nod_natural, (int) 0, NULL); }
    break;

  case 751:
    { yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;

  case 752:
    { yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 754:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 755:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 756:
    { yyval = 0; }
    break;

  case 757:
    { yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;

  case 758:
    { yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 761:
    { yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 762:
    { yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 763:
    { yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 766:
    { yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;

  case 767:
    { yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;

  case 769:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 770:
    { yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;

  case 774:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 775:
    { yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 776:
    { yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 777:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 778:
    { yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;

  case 782:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 783:
    { yyval = yyvsp[0]; }
    break;

  case 785:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 788:
    { yyval = NULL; }
    break;

  case 789:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 791:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 793:
    { yyval = NULL; }
    break;

  case 794:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 796:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 798:
    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 799:
    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;

  case 800:
    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 802:
    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 804:
    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 806:
    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 807:
    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 809:
    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 810:
    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 811:
    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 812:
    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 813:
    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 815:
    { yyval = yyvsp[-1]; }
    break;

  case 816:
    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 827:
    { yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 828:
    { yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 829:
    { yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 830:
    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 831:
    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 832:
    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 833:
    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 834:
    { yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 835:
    { yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 836:
    { yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 837:
    { yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 838:
    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 839:
    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 840:
    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 841:
    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 842:
    { yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 843:
    { yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 844:
    { yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 845:
    { yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 846:
    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 847:
    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 848:
    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 849:
    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 850:
    { yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 853:
    { yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 854:
    { yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 855:
    { yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 856:
    { yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 857:
    { yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 858:
    { yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 859:
    { yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 860:
    { yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 861:
    { yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 862:
    { yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 863:
    { yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 864:
    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 865:
    { yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 866:
    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 867:
    { yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;

  case 868:
    { yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;

  case 869:
    { yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;

  case 870:
    { yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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

  case 874:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 875:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;

  case 876:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;

  case 878:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 879:
    { yyval = yyvsp[-1]; }
    break;

  case 880:
    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 881:
    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 891:
    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 892:
    { yyval = yyvsp[0]; }
    break;

  case 893:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 894:
    { yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 895:
    { yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;

  case 896:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 897:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 898:
    {
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 899:
    { yyval = yyvsp[-1]; }
    break;

  case 900:
    { yyval = yyvsp[-1]; }
    break;

  case 904:
    { yyval = make_node (nod_dbkey, 1, NULL); }
    break;

  case 905:
    { yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;

  case 906:
    { 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;

  case 907:
    { yyval = yyvsp[0]; }
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
			yyval = make_node (nod_current_date, 0, NULL);
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
			yyval = make_node (nod_current_time, 0, NULL);
			}
    break;

  case 910:
    { yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;

  case 911:
    { yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 913:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 915:
    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 916:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;

  case 917:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 918:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;

  case 919:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 920:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 922:
    { yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;

  case 923:
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

  case 924:
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

  case 925:
    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;

  case 926:
    { yyval = make_parameter (); }
    break;

  case 927:
    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 928:
    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 929:
    { yyval = make_node (nod_current_role, 0, NULL); }
    break;

  case 930:
    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;

  case 931:
    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;

  case 932:
    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;

  case 933:
    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;

  case 934:
    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;

  case 935:
    { yyval = yyvsp[0]; }
    break;

  case 936:
    { ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;

  case 938:
    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 939:
    { if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 940:
    { if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 941:
    { if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;

  case 942:
    { if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 944:
    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 945:
    { yyval = yyvsp[0];}
    break;

  case 950:
    { yyval = make_node (nod_agg_count, 0, NULL); }
    break;

  case 951:
    { yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;

  case 952:
    { yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;

  case 953:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;

  case 954:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 955:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;

  case 956:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 957:
    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 958:
    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 959:
    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 960:
    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 961:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;

  case 963:
    { yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;

  case 965:
    { yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;

  case 966:
    { yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;

  case 967:
    { yyval = yyvsp[0]; }
    break;

  case 968:
    { yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;

  case 969:
    { yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 970:
    { yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;

  case 971:
    { yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;

  case 974:
    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;

  case 975:
    { yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 978:
    { yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 979:
    { yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 980:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 981:
    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 982:
    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 983:
    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 984:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 985:
    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 989:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;

  case 990:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;

  case 991:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;

  case 992:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;

  case 993:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;

  case 994:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;

  case 995:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;

  case 996:
    { yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;

  case 999:
    { yyval = make_node (nod_null, 0, NULL); }
    break;


    }

/* Line 1016 of /usr/local/share/bison/yacc.c.  */
#line 8182 "y.tab.c"

  yyvsp -= yylen;
  DSQL_DSQL_yyssp -= yylen;


#if YYDEBUG
  if (lex.dsql_debug)
    {
      short *DSQL_DSQL_yyssp1 = DSQL_yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (DSQL_DSQL_yyssp1 != DSQL_DSQL_yyssp)
	YYFPRINTF (stderr, " %d", *++DSQL_DSQL_yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *DSQL_DSQL_yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *DSQL_DSQL_yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (DSQL_yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (DSQL_yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (DSQL_DSQL_yyssp > DSQL_yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*DSQL_DSQL_yyssp],
			    *yyvsp));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*DSQL_DSQL_yyssp], *yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  DSQL_yychar, yytname[DSQL_yychar1]));
      yydestruct (DSQL_yychar1, yylval);
      DSQL_yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (DSQL_DSQL_yyssp == DSQL_yyss)
	YYABORT;

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*DSQL_DSQL_yyssp], *yyvsp));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp);
      yyvsp--;
      yystate = *--DSQL_DSQL_yyssp;


#if YYDEBUG
      if (lex.dsql_debug)
	{
	  short *DSQL_DSQL_yyssp1 = DSQL_yyss - 1;
	  YYFPRINTF (stderr, "Error: state stack now");
	  while (DSQL_DSQL_yyssp1 != DSQL_DSQL_yyssp)
	    YYFPRINTF (stderr, " %d", *++DSQL_DSQL_yyssp1);
	  YYFPRINTF (stderr, "\n");
	}
#endif
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (DSQL_yyss != DSQL_yyssa)
    YYSTACK_FREE (DSQL_yyss);
#endif
  return yyresult;
}





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


