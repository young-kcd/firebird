/* A Bison parser, made by GNU Bison 1.875.  */

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
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

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

// Using this option causes build problems on Win32 with bison 1.28
//#define YYSTACK_USE_ALLOCA 1

#define YYSTYPE  dsql_nod*
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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */


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
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short DSQL_yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

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
	    (To)[yyi] = (From)[yyi];		\
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
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
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   6944

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  285
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  428
/* YYNRULES -- Number of rules. */
#define YYNRULES  1017
/* YYNRULES -- Number of states. */
#define YYNSTATES  1897

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   523

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
     514,   517,   521,   522,   524,   526,   529,   531,   536,   540,
     544,   546,   547,   549,   551,   554,   558,   563,   564,   566,
     569,   570,   572,   574,   580,   586,   590,   593,   594,   596,
     600,   602,   604,   611,   615,   618,   625,   628,   630,   633,
     636,   639,   640,   642,   644,   647,   648,   651,   652,   654,
     656,   658,   660,   662,   664,   665,   667,   669,   672,   675,
     678,   684,   686,   689,   693,   696,   699,   700,   702,   704,
     706,   708,   712,   717,   726,   731,   732,   739,   741,   743,
     746,   749,   750,   754,   758,   760,   763,   766,   769,   778,
     787,   796,   805,   809,   810,   815,   816,   818,   822,   827,
     829,   833,   836,   840,   844,   846,   848,   849,   851,   854,
     859,   861,   863,   867,   869,   870,   873,   875,   882,   884,
     886,   890,   892,   895,   896,   898,   901,   904,   907,   909,
     911,   913,   915,   917,   919,   922,   924,   926,   929,   931,
     933,   935,   938,   941,   943,   946,   950,   955,   958,   965,
     970,   979,   989,   996,  1005,  1012,  1017,  1018,  1023,  1026,
    1028,  1032,  1033,  1036,  1041,  1042,  1044,  1046,  1050,  1054,
    1062,  1065,  1066,  1069,  1072,  1076,  1080,  1081,  1083,  1086,
    1091,  1093,  1097,  1100,  1103,  1106,  1108,  1110,  1112,  1114,
    1117,  1120,  1126,  1127,  1132,  1140,  1144,  1145,  1147,  1151,
    1155,  1163,  1171,  1173,  1175,  1179,  1184,  1192,  1195,  1197,
    1201,  1203,  1205,  1212,  1216,  1217,  1218,  1219,  1220,  1224,
    1225,  1235,  1245,  1247,  1249,  1250,  1253,  1255,  1257,  1259,
    1261,  1263,  1267,  1271,  1275,  1279,  1283,  1287,  1293,  1299,
    1305,  1311,  1317,  1323,  1326,  1327,  1332,  1335,  1339,  1343,
    1346,  1349,  1353,  1357,  1360,  1364,  1371,  1373,  1376,  1381,
    1385,  1388,  1391,  1394,  1397,  1401,  1403,  1407,  1411,  1415,
    1418,  1421,  1426,  1431,  1437,  1439,  1441,  1443,  1445,  1447,
    1449,  1451,  1453,  1455,  1457,  1459,  1461,  1463,  1465,  1467,
    1469,  1471,  1473,  1475,  1477,  1479,  1481,  1483,  1485,  1487,
    1489,  1491,  1493,  1495,  1498,  1501,  1504,  1506,  1508,  1510,
    1511,  1514,  1517,  1518,  1520,  1523,  1526,  1531,  1535,  1538,
    1541,  1549,  1551,  1552,  1554,  1555,  1558,  1561,  1564,  1567,
    1570,  1573,  1576,  1579,  1582,  1586,  1589,  1592,  1595,  1597,
    1599,  1601,  1603,  1608,  1614,  1616,  1620,  1622,  1626,  1628,
    1631,  1633,  1635,  1637,  1639,  1641,  1643,  1645,  1647,  1649,
    1651,  1653,  1658,  1663,  1670,  1676,  1680,  1681,  1684,  1687,
    1688,  1692,  1693,  1698,  1700,  1706,  1711,  1713,  1718,  1720,
    1723,  1726,  1728,  1730,  1732,  1735,  1738,  1741,  1744,  1745,
    1749,  1755,  1757,  1759,  1762,  1766,  1768,  1771,  1775,  1776,
    1778,  1780,  1782,  1788,  1794,  1801,  1803,  1805,  1807,  1810,
    1815,  1817,  1818,  1824,  1826,  1827,  1831,  1834,  1836,  1837,
    1840,  1841,  1843,  1844,  1848,  1850,  1851,  1853,  1856,  1858,
    1860,  1862,  1864,  1867,  1870,  1872,  1875,  1879,  1881,  1883,
    1887,  1891,  1893,  1896,  1900,  1902,  1905,  1906,  1909,  1911,
    1913,  1914,  1916,  1918,  1920,  1924,  1927,  1931,  1932,  1934,
    1938,  1943,  1949,  1951,  1955,  1960,  1964,  1965,  1967,  1971,
    1975,  1977,  1979,  1980,  1982,  1984,  1989,  1990,  1993,  1998,
    1999,  2003,  2004,  2007,  2008,  2011,  2012,  2022,  2034,  2035,
    2036,  2037,  2038,  2042,  2045,  2047,  2048,  2052,  2058,  2062,
    2065,  2071,  2074,  2076,  2078,  2080,  2082,  2084,  2088,  2090,
    2094,  2096,  2097,  2100,  2102,  2106,  2108,  2110,  2112,  2119,
    2121,  2122,  2126,  2127,  2129,  2133,  2140,  2144,  2149,  2152,
    2156,  2157,  2159,  2162,  2164,  2166,  2168,  2171,  2173,  2176,
    2178,  2181,  2182,  2186,  2187,  2189,  2193,  2195,  2198,  2199,
    2202,  2203,  2206,  2207,  2212,  2214,  2217,  2219,  2221,  2222,
    2224,  2228,  2231,  2233,  2235,  2238,  2240,  2245,  2249,  2251,
    2255,  2260,  2261,  2270,  2276,  2278,  2280,  2285,  2290,  2295,
    2297,  2299,  2305,  2311,  2313,  2317,  2321,  2323,  2325,  2327,
    2331,  2339,  2347,  2353,  2357,  2358,  2360,  2362,  2364,  2367,
    2368,  2370,  2372,  2374,  2375,  2379,  2381,  2385,  2387,  2388,
    2392,  2394,  2398,  2400,  2404,  2408,  2410,  2412,  2416,  2418,
    2421,  2423,  2427,  2431,  2433,  2436,  2440,  2444,  2448,  2452,
    2454,  2458,  2461,  2463,  2465,  2467,  2469,  2471,  2473,  2475,
    2477,  2479,  2481,  2485,  2489,  2493,  2497,  2501,  2505,  2509,
    2513,  2520,  2527,  2534,  2541,  2548,  2555,  2562,  2569,  2576,
    2583,  2590,  2597,  2604,  2611,  2618,  2625,  2627,  2629,  2635,
    2642,  2646,  2651,  2657,  2664,  2668,  2673,  2677,  2682,  2686,
    2691,  2696,  2702,  2707,  2712,  2716,  2721,  2723,  2725,  2727,
    2729,  2731,  2733,  2735,  2739,  2743,  2755,  2767,  2769,  2771,
    2773,  2775,  2777,  2779,  2781,  2783,  2785,  2788,  2791,  2795,
    2799,  2803,  2807,  2811,  2815,  2819,  2823,  2825,  2827,  2829,
    2831,  2835,  2837,  2839,  2841,  2843,  2845,  2850,  2852,  2856,
    2858,  2861,  2863,  2865,  2867,  2869,  2871,  2873,  2875,  2878,
    2881,  2884,  2886,  2888,  2890,  2892,  2894,  2896,  2898,  2900,
    2902,  2904,  2907,  2909,  2912,  2914,  2916,  2918,  2920,  2922,
    2925,  2927,  2929,  2931,  2933,  2935,  2940,  2946,  2952,  2958,
    2964,  2970,  2976,  2982,  2988,  2994,  3000,  3007,  3009,  3016,
    3018,  3023,  3031,  3034,  3035,  3040,  3044,  3051,  3053,  3055,
    3062,  3069,  3071,  3073,  3078,  3085,  3090,  3096,  3100,  3106,
    3111,  3117,  3119,  3121,  3123,  3125,  3127,  3129,  3131,  3133,
    3135,  3137,  3139,  3141,  3142,  3144,  3146,  3148,  3150,  3152,
    3154,  3156,  3158,  3160,  3162,  3164,  3166,  3168,  3170,  3172,
    3174,  3176,  3178,  3180,  3182,  3184,  3186,  3188,  3190,  3192,
    3194,  3196,  3198,  3200,  3202,  3204,  3206,  3208,  3210,  3212,
    3214,  3216,  3218,  3220,  3222,  3224,  3226,  3228
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     286,     0,    -1,   287,    -1,   287,   276,    -1,   469,    -1,
     610,    -1,   522,    -1,   317,    -1,   307,    -1,   599,    -1,
     490,    -1,   288,    -1,   598,    -1,   442,    -1,   443,    -1,
     319,    -1,   321,    -1,   297,    -1,   523,    -1,   516,    -1,
     545,    -1,   514,    -1,   603,    -1,    43,   657,    -1,    73,
     290,   129,   289,   182,   301,   294,    -1,    73,   292,   129,
     148,   296,   182,   301,   294,    -1,    73,   290,   129,   289,
     182,   299,    -1,    73,   292,   129,   148,   296,   182,   299,
      -1,    73,   303,   182,   305,   295,    -1,   582,    -1,   180,
     582,    -1,     6,    -1,     6,   147,    -1,   291,    -1,   293,
      -1,   291,   277,   293,    -1,    59,    -1,   163,    -1,    84,
      -1,    47,    -1,   188,   615,    -1,   153,   615,    -1,   200,
      73,   131,    -1,    -1,   200,   210,   131,    -1,    -1,   702,
      -1,   158,   298,   290,   129,   289,    66,   301,    -1,   158,
     298,   292,   129,   148,   296,    66,   301,    -1,   158,   290,
     129,   289,    66,   301,    -1,   158,   292,   129,   148,   296,
      66,   301,    -1,   158,   290,   129,   289,    66,   299,    -1,
     158,   292,   129,   148,   296,    66,   299,    -1,   158,   303,
      66,   305,    -1,    73,   131,    64,    -1,   300,    -1,   299,
     277,   300,    -1,   299,   277,   302,    -1,   301,   277,   300,
      -1,   148,   702,    -1,   184,   706,    -1,   195,   709,    -1,
     214,   703,    -1,   302,    -1,   301,   277,   302,    -1,   707,
      -1,   189,   707,    -1,    74,   707,    -1,   304,    -1,   303,
     277,   304,    -1,   703,    -1,   306,    -1,   305,   277,   306,
      -1,   707,    -1,   189,   707,    -1,    45,   308,    -1,    63,
     316,    -1,    62,    68,   309,    -1,   687,   311,   157,   314,
      55,   656,   117,   656,    -1,   497,    -1,    19,    -1,    37,
     278,   660,   279,   503,    -1,    -1,   312,    -1,   278,   312,
     279,    -1,   313,    -1,   312,   277,   313,    -1,   371,   310,
      -1,   371,   310,    20,   238,    -1,   315,    -1,   278,   315,
     279,    -1,   371,   310,    -1,   371,   310,   212,    -1,   371,
     310,    20,   101,    -1,   371,   310,    20,   238,    -1,   140,
     660,    -1,   696,    82,   502,   135,   502,    55,   656,   117,
     656,    -1,    36,   318,    -1,    58,   695,   656,    -1,   323,
     550,    83,   699,   129,   582,   324,    -1,   148,   390,    -1,
     180,   358,    -1,   184,   461,    -1,   195,   447,    -1,    71,
     340,    -1,    40,   342,    -1,    51,   331,    -1,   165,   325,
      -1,   214,   341,    -1,   237,   320,    -1,   148,   391,    -1,
     180,   359,    -1,   195,   448,    -1,    36,   132,     7,   322,
      -1,   148,   392,    -1,   184,   462,    -1,   187,    -1,    -1,
     617,    -1,   616,    -1,   365,   278,   458,   645,   459,   279,
      -1,   660,   326,   327,   656,   328,   329,    -1,   111,    -1,
      14,    -1,    -1,    -1,    32,    -1,    -1,   102,   343,   663,
     357,    -1,    -1,   330,    -1,   351,    -1,   330,   351,    -1,
     368,   332,   492,   458,   333,   459,   334,   367,    -1,    10,
      -1,    -1,    46,   458,   373,    -1,    -1,    -1,   335,    -1,
     336,    -1,   335,   336,    -1,   337,    -1,   338,    -1,   339,
      -1,   124,    98,    -1,   458,    24,   278,   624,   279,   459,
      -1,   698,    -1,   703,    -1,   344,   345,   348,    -1,    -1,
     269,    -1,   656,    -1,    -1,   346,    -1,   347,    -1,   346,
     347,    -1,   139,   343,   660,    -1,   102,   343,   663,   357,
      -1,   189,   656,    -1,   141,   656,    -1,   164,   118,   656,
      -1,    -1,   349,    -1,   350,    -1,   349,   350,    -1,   351,
      -1,    46,    23,   164,   689,    -1,   263,    94,   656,    -1,
     352,   656,   353,    -1,    94,    -1,    -1,   354,    -1,   355,
      -1,   354,   355,    -1,   175,   356,   663,    -1,   102,   343,
     663,   357,    -1,    -1,    12,    -1,    12,   137,    -1,    -1,
     137,    -1,   138,    -1,   582,   360,   278,   361,   279,    -1,
     582,   360,   278,   361,   279,    -1,    62,    94,   656,    -1,
      62,   656,    -1,    -1,   362,    -1,   361,   277,   362,    -1,
     363,    -1,   378,    -1,   368,   366,   372,   459,   374,   367,
      -1,   368,   493,   364,    -1,   368,   364,    -1,   365,   278,
     458,   645,   459,   279,    -1,    30,    20,    -1,    30,    -1,
     492,   458,    -1,   622,   456,    -1,    26,   690,    -1,    -1,
     622,    -1,   622,    -1,   371,   492,    -1,    -1,    46,   373,
      -1,    -1,   649,    -1,   653,    -1,   654,    -1,   655,    -1,
     686,    -1,   646,    -1,    -1,   375,    -1,   376,    -1,   375,
     376,    -1,   379,   377,    -1,   124,    98,    -1,   153,   582,
     615,   386,   384,    -1,   385,    -1,   187,   384,    -1,   146,
      90,   384,    -1,   379,   380,    -1,    33,   692,    -1,    -1,
     381,    -1,   382,    -1,   383,    -1,   385,    -1,   187,   616,
     384,    -1,   146,    90,   616,   384,    -1,    65,    90,   616,
     153,   582,   615,   386,   384,    -1,   247,   550,    83,   699,
      -1,    -1,   458,    24,   278,   624,   279,   459,    -1,   387,
      -1,   388,    -1,   388,   387,    -1,   387,   388,    -1,    -1,
     129,   188,   389,    -1,   129,    47,   389,    -1,   211,    -1,
     164,    46,    -1,   164,    98,    -1,   123,   209,    -1,   702,
     394,   395,    10,   456,   401,   410,   459,    -1,   702,   394,
     395,    10,   456,   401,   410,   459,    -1,   702,   394,   395,
      10,   456,   401,   410,   459,    -1,   702,   394,   395,    10,
     456,   401,   410,   459,    -1,   278,   396,   279,    -1,    -1,
     157,   278,   398,   279,    -1,    -1,   397,    -1,   396,   277,
     397,    -1,   369,   493,   400,   459,    -1,   399,    -1,   398,
     277,   399,    -1,   369,   493,    -1,    46,   456,   373,    -1,
     269,   456,   373,    -1,   456,    -1,   402,    -1,    -1,   403,
      -1,   402,   403,    -1,    45,   406,   404,   276,    -1,   405,
      -1,   408,    -1,   368,   493,   407,    -1,   192,    -1,    -1,
     269,   373,    -1,   372,    -1,   693,    39,    64,   278,   558,
     279,    -1,   413,    -1,   410,    -1,    17,   411,    54,    -1,
     412,    -1,   412,   433,    -1,    -1,   409,    -1,   412,   409,
      -1,   607,   276,    -1,   599,   276,    -1,   414,    -1,   415,
      -1,   416,    -1,   417,    -1,   418,    -1,   421,    -1,   598,
     276,    -1,   422,    -1,   424,    -1,   603,   276,    -1,   429,
      -1,   419,    -1,   420,    -1,   178,   276,    -1,    61,   276,
      -1,   431,    -1,   437,   276,    -1,    58,   695,   276,    -1,
      58,   695,   645,   276,    -1,    58,   276,    -1,    59,   148,
     702,   426,   427,   276,    -1,    59,   254,   645,   276,    -1,
     430,    64,   545,    86,   428,   432,    50,   409,    -1,   430,
      64,    59,   254,   645,    86,   428,    50,   409,    -1,    59,
     254,   645,    86,   428,   276,    -1,    78,   278,   624,   279,
     181,   409,    53,   409,    -1,    78,   278,   624,   279,   181,
     409,    -1,   144,   645,   423,   276,    -1,    -1,   545,    86,
     428,   276,    -1,   280,   708,    -1,   609,    -1,   278,   609,
     279,    -1,    -1,   156,   428,    -1,   156,   278,   428,   279,
      -1,    -1,   425,    -1,   621,    -1,   428,   277,   621,    -1,
     428,   277,   425,    -1,   430,   199,   278,   624,   279,    50,
     409,    -1,   701,   280,    -1,    -1,   235,   276,    -1,   255,
     276,    -1,   255,   701,   276,    -1,    10,    39,   693,    -1,
      -1,   434,    -1,   433,   434,    -1,   197,   435,    50,   409,
      -1,   436,    -1,   435,   277,   436,    -1,   173,   657,    -1,
      69,   697,    -1,    58,   695,    -1,     9,    -1,   438,    -1,
     440,    -1,   439,    -1,   264,   693,    -1,   265,   693,    -1,
     266,   441,   693,    86,   428,    -1,    -1,    59,   148,   702,
     426,    -1,    59,   268,   444,   395,    10,   401,   410,    -1,
     278,   445,   279,    -1,    -1,   446,    -1,   445,   277,   446,
      -1,   399,   269,   652,    -1,   709,   615,    10,   456,   449,
     460,   457,    -1,   709,   615,    10,   456,   449,   460,   457,
      -1,   450,    -1,   451,    -1,   450,   186,   451,    -1,   450,
     186,     6,   451,    -1,   163,   566,   567,   452,   588,   584,
     587,    -1,    66,   453,    -1,   454,    -1,   453,   277,   454,
      -1,   455,    -1,   581,    -1,   454,   583,    89,   454,   129,
     624,    -1,   278,   455,   279,    -1,    -1,    -1,    -1,    -1,
     200,    24,   131,    -1,    -1,   706,    64,   582,   463,   464,
     467,   458,   468,   459,    -1,   706,    64,   582,   463,   464,
     467,   458,   468,   459,    -1,     3,    -1,    80,    -1,    -1,
     465,   466,    -1,    16,    -1,     5,    -1,    84,    -1,   188,
      -1,    47,    -1,    84,   132,   188,    -1,    84,   132,    47,
      -1,   188,   132,    84,    -1,   188,   132,    47,    -1,    47,
     132,    84,    -1,    47,   132,   188,    -1,    84,   132,   188,
     132,    47,    -1,    84,   132,    47,   132,   188,    -1,   188,
     132,    84,   132,    47,    -1,   188,   132,    47,   132,    84,
      -1,    47,   132,    84,   132,   188,    -1,    47,   132,   188,
     132,    84,    -1,   143,   658,    -1,    -1,    10,   458,   401,
     410,    -1,     7,   470,    -1,    58,   695,   656,    -1,   180,
     582,   475,    -1,   184,   487,    -1,   148,   393,    -1,    40,
     484,   485,    -1,    51,   477,   473,    -1,    83,   483,    -1,
      46,   458,   373,    -1,    24,   458,   278,   624,   279,   459,
      -1,   474,    -1,   473,   474,    -1,   164,   458,   471,   459,
      -1,     4,    33,   472,    -1,     4,   339,    -1,    52,    46,
      -1,    52,    33,    -1,   182,   622,    -1,   216,   371,   493,
      -1,   476,    -1,   475,   277,   476,    -1,    52,   622,   482,
      -1,    52,    33,   692,    -1,     4,   363,    -1,     4,   378,
      -1,   479,   622,   143,   660,    -1,   479,   477,   182,   622,
      -1,   479,   481,   216,   480,   459,    -1,   478,    -1,   711,
      -1,   215,    -1,   216,    -1,   217,    -1,   218,    -1,   219,
      -1,   220,    -1,   221,    -1,   222,    -1,   223,    -1,   224,
      -1,   225,    -1,   226,    -1,   227,    -1,   228,    -1,   229,
      -1,   230,    -1,   233,    -1,   234,    -1,   241,    -1,   242,
      -1,   250,    -1,   252,    -1,   264,    -1,   265,    -1,   266,
      -1,   267,    -1,     7,    -1,     7,   215,    -1,   493,   458,
      -1,   622,   456,    -1,   622,    -1,   213,    -1,   211,    -1,
      -1,   699,     3,    -1,   699,    80,    -1,    -1,   486,    -1,
     485,   486,    -1,     4,   330,    -1,     4,   263,    94,   656,
      -1,    52,   263,    94,    -1,    17,   262,    -1,    54,   262,
      -1,   706,   463,   488,   467,   458,   489,   459,    -1,   464,
      -1,    -1,   468,    -1,    -1,    52,   491,    -1,    58,   695,
      -1,    83,   699,    -1,   148,   702,    -1,   180,   705,    -1,
     184,   706,    -1,   195,   709,    -1,    63,   696,    -1,    51,
     694,    -1,    62,    68,   687,    -1,   165,   660,    -1,   214,
     703,    -1,    71,   698,    -1,   493,    -1,   494,    -1,   497,
      -1,   500,    -1,   498,   281,   495,   282,    -1,   505,   281,
     495,   282,   503,    -1,   496,    -1,   495,   277,   496,    -1,
     662,    -1,   662,   280,   662,    -1,   498,    -1,   505,   503,
      -1,   504,    -1,   509,    -1,   512,    -1,   243,    -1,   499,
      -1,   169,    -1,    41,    -1,   226,    -1,   227,    -1,    85,
      -1,    96,    -1,    19,   502,   501,   503,    -1,    19,   278,
     661,   279,    -1,    19,   278,   661,   277,   657,   279,    -1,
      19,   278,   277,   657,   279,    -1,   162,   168,   661,    -1,
      -1,   177,   657,    -1,   177,   688,    -1,    -1,    23,   164,
     689,    -1,    -1,   508,   278,   660,   279,    -1,   508,    -1,
     508,   193,   278,   660,   279,    -1,   507,   278,   660,   279,
      -1,   507,    -1,   506,   278,   660,   279,    -1,   191,    -1,
      23,   193,    -1,    91,   193,    -1,    23,    -1,    91,    -1,
     121,    -1,   119,    23,    -1,   119,    91,    -1,    99,   510,
      -1,   511,   510,    -1,    -1,   278,   662,   279,    -1,   278,
     662,   277,   662,   279,    -1,    44,    -1,    92,    -1,    95,
     513,    -1,    97,    95,   513,    -1,   152,    -1,    93,   145,
      -1,   278,   658,   279,    -1,    -1,   527,    -1,   515,    -1,
     544,    -1,   164,    71,   698,   182,   662,    -1,   164,    71,
     698,   182,   231,    -1,   164,    71,   698,   182,   273,   231,
      -1,   517,    -1,   518,    -1,   520,    -1,   252,   710,    -1,
     253,   252,   710,   519,    -1,   130,    -1,    -1,   161,   524,
     182,   521,   710,    -1,   252,    -1,    -1,    28,   524,   525,
      -1,   161,   524,    -1,   201,    -1,    -1,   155,   526,    -1,
      -1,   170,    -1,    -1,   164,   183,   528,    -1,   529,    -1,
      -1,   530,    -1,   529,   530,    -1,   531,    -1,   532,    -1,
     533,    -1,   537,    -1,   151,   130,    -1,   151,   202,    -1,
     196,    -1,   123,   196,    -1,    88,   107,   534,    -1,   534,
      -1,   535,    -1,   151,   185,   536,    -1,   151,    29,   536,
      -1,   170,    -1,   170,   180,    -1,   170,   180,   174,    -1,
     194,    -1,   123,   194,    -1,    -1,   154,   540,    -1,   166,
      -1,   149,    -1,    -1,   151,    -1,   202,    -1,   541,    -1,
     540,   277,   541,    -1,   543,   542,    -1,    64,   538,   539,
      -1,    -1,   582,    -1,   543,   277,   582,    -1,   164,   176,
      83,   699,    -1,   546,   547,   553,   554,   556,    -1,   557,
      -1,   546,   186,   557,    -1,   546,   186,     6,   557,    -1,
     133,    20,   548,    -1,    -1,   549,    -1,   548,   277,   549,
      -1,   645,   550,   552,    -1,    11,    -1,    48,    -1,    -1,
     239,    -1,   249,    -1,   248,   561,   551,   562,    -1,    -1,
     267,   645,    -1,   267,   645,   182,   645,    -1,    -1,    64,
     188,   555,    -1,    -1,   128,   617,    -1,    -1,   200,   251,
      -1,    -1,   163,   563,   566,   567,   571,   588,   584,   587,
     589,    -1,   163,   563,   566,   567,   571,   588,   584,   587,
     589,   547,   553,    -1,    -1,    -1,    -1,    -1,   564,   565,
     560,    -1,   564,   560,    -1,   565,    -1,    -1,   239,   663,
     559,    -1,   239,   278,   645,   279,   559,    -1,   239,   652,
     559,    -1,   240,   663,    -1,   240,   278,   560,   645,   279,
      -1,   240,   652,    -1,    49,    -1,   685,    -1,   568,    -1,
     274,    -1,   569,    -1,   568,   277,   569,    -1,   608,    -1,
     608,   570,   700,    -1,    10,    -1,    -1,    66,   572,    -1,
     573,    -1,   572,   277,   573,    -1,   578,    -1,   579,    -1,
     574,    -1,   278,   545,   279,   570,   575,   576,    -1,   704,
      -1,    -1,   278,   577,   279,    -1,    -1,   700,    -1,   577,
     277,   700,    -1,   573,   583,    89,   573,   129,   624,    -1,
     278,   578,   279,    -1,   702,   580,   570,   704,    -1,   702,
     580,    -1,   278,   609,   279,    -1,    -1,   582,    -1,   705,
     704,    -1,   705,    -1,    81,    -1,   105,    -1,   105,   134,
      -1,   159,    -1,   159,   134,    -1,    67,    -1,    67,   134,
      -1,    -1,    74,    20,   585,    -1,    -1,   586,    -1,   585,
     277,   586,    -1,   645,    -1,    77,   624,    -1,    -1,   198,
     624,    -1,    -1,   142,   590,    -1,    -1,   591,   278,   592,
     279,    -1,    89,    -1,   172,   114,    -1,   114,    -1,   172,
      -1,    -1,   593,    -1,   593,   277,   592,    -1,   594,   595,
      -1,   590,    -1,   705,    -1,   705,   594,    -1,   120,    -1,
      83,   278,   596,   279,    -1,   133,   699,   597,    -1,   699,
      -1,   699,   277,   596,    -1,    83,   278,   596,   279,    -1,
      -1,    84,    86,   582,   618,   190,   278,   609,   279,    -1,
      84,    86,   582,   618,   558,    -1,   600,    -1,   601,    -1,
      47,    66,   581,   588,    -1,    47,    66,   581,   602,    -1,
     198,    38,   128,   693,    -1,   604,    -1,   605,    -1,   188,
     581,   164,   606,   588,    -1,   188,   581,   164,   606,   602,
      -1,   607,    -1,   606,   277,   607,    -1,   623,   269,   608,
      -1,   645,    -1,   686,    -1,   608,    -1,   609,   277,   608,
      -1,   151,    19,   622,    66,   582,   611,   613,    -1,    84,
      19,   622,    86,   582,   611,   613,    -1,    63,    66,   612,
     182,   612,    -1,    63,   182,   612,    -1,    -1,   502,    -1,
     652,    -1,   657,    -1,   113,   614,    -1,    -1,   661,    -1,
     652,    -1,   616,    -1,    -1,   278,   617,   279,    -1,   622,
      -1,   617,   277,   622,    -1,   619,    -1,    -1,   278,   620,
     279,    -1,   623,    -1,   620,   277,   623,    -1,   622,    -1,
     704,   283,   691,    -1,   704,   283,   274,    -1,   691,    -1,
     622,    -1,   704,   283,   691,    -1,   639,    -1,   124,   639,
      -1,   626,    -1,   624,   132,   624,    -1,   624,     8,   624,
      -1,   626,    -1,   124,   639,    -1,   625,   132,   624,    -1,
     625,     8,   624,    -1,   640,   132,   624,    -1,   640,     8,
     624,    -1,   627,    -1,   278,   625,   279,    -1,   124,   626,
      -1,   628,    -1,   631,    -1,   632,    -1,   633,    -1,   638,
      -1,   629,    -1,   636,    -1,   634,    -1,   635,    -1,   637,
      -1,   645,   269,   645,    -1,   645,   270,   645,    -1,   645,
     271,   645,    -1,   645,    70,   645,    -1,   645,   106,   645,
      -1,   645,   125,   645,    -1,   645,   126,   645,    -1,   645,
     122,   645,    -1,   645,   269,     6,   278,   643,   279,    -1,
     645,   270,     6,   278,   643,   279,    -1,   645,   271,     6,
     278,   643,   279,    -1,   645,    70,     6,   278,   643,   279,
      -1,   645,   106,     6,   278,   643,   279,    -1,   645,   125,
       6,   278,   643,   279,    -1,   645,   126,     6,   278,   643,
     279,    -1,   645,   122,     6,   278,   643,   279,    -1,   645,
     269,   630,   278,   643,   279,    -1,   645,   270,   630,   278,
     643,   279,    -1,   645,   271,   630,   278,   643,   279,    -1,
     645,    70,   630,   278,   643,   279,    -1,   645,   106,   630,
     278,   643,   279,    -1,   645,   125,   630,   278,   643,   279,
      -1,   645,   126,   630,   278,   643,   279,    -1,   645,   122,
     630,   278,   643,   279,    -1,   171,    -1,     9,    -1,   645,
      18,   645,     8,   645,    -1,   645,   124,    18,   645,     8,
     645,    -1,   645,   108,   645,    -1,   645,   124,   108,   645,
      -1,   645,   108,   645,    57,   645,    -1,   645,   124,   108,
     645,    57,   645,    -1,   645,    79,   641,    -1,   645,   124,
      79,   641,    -1,   645,    34,   645,    -1,   645,   124,    34,
     645,    -1,   645,   175,   645,    -1,   645,   124,   175,   645,
      -1,   645,   175,   200,   645,    -1,   645,   124,   175,   200,
     645,    -1,    60,   278,   558,   279,    -1,   167,   278,   558,
     279,    -1,   645,    87,    98,    -1,   645,    87,   124,    98,
      -1,   256,    -1,   257,    -1,   258,    -1,   259,    -1,   260,
      -1,   261,    -1,   642,    -1,   278,   648,   279,    -1,   278,
     643,   279,    -1,   163,   563,   566,   645,   571,   588,   584,
     587,   589,   547,   553,    -1,   163,   563,   566,   645,   571,
     588,   584,   587,   589,   547,   553,    -1,   621,    -1,   647,
      -1,   664,    -1,   651,    -1,   652,    -1,   425,    -1,   673,
      -1,   674,    -1,   672,    -1,   273,   645,    -1,   272,   645,
      -1,   645,   272,   645,    -1,   645,    31,   645,    -1,   645,
      26,   690,    -1,   645,   273,   645,    -1,   645,   274,   645,
      -1,   645,   275,   645,    -1,   278,   645,   279,    -1,   278,
     644,   279,    -1,   653,    -1,   654,    -1,   655,    -1,    42,
      -1,   704,   283,    42,    -1,   101,    -1,   646,    -1,   228,
      -1,   229,    -1,   230,    -1,   621,   281,   648,   282,    -1,
     645,    -1,   648,   277,   645,    -1,   651,    -1,   273,   650,
      -1,   205,    -1,   204,    -1,   203,    -1,   231,    -1,   232,
      -1,   650,    -1,   656,    -1,    41,   207,    -1,   226,   207,
      -1,   227,   207,    -1,   284,    -1,   189,    -1,   233,    -1,
     234,    -1,   241,    -1,   242,    -1,    69,    -1,   173,    -1,
     250,    -1,   207,    -1,   208,   207,    -1,   658,    -1,   273,
     659,    -1,   204,    -1,   204,    -1,   658,    -1,   204,    -1,
     663,    -1,   273,   663,    -1,   204,    -1,   665,    -1,   666,
      -1,   667,    -1,   669,    -1,    35,   278,   274,   279,    -1,
      35,   278,   685,   645,   279,    -1,    35,   278,    49,   645,
     279,    -1,   179,   278,   685,   645,   279,    -1,   179,   278,
      49,   645,   279,    -1,    13,   278,   685,   645,   279,    -1,
      13,   278,    49,   645,   279,    -1,   116,   278,   685,   645,
     279,    -1,   116,   278,    49,   645,   279,    -1,   112,   278,
     685,   645,   279,    -1,   112,   278,    49,   645,   279,    -1,
      72,   278,   698,   277,   645,   279,    -1,   668,    -1,   217,
     278,   684,    66,   645,   279,    -1,   670,    -1,   100,   278,
     645,   279,    -1,   236,   278,   645,    66,   645,   671,   279,
      -1,    64,   645,    -1,    -1,   687,   278,   648,   279,    -1,
     687,   278,   279,    -1,    22,   278,   608,    10,   370,   279,
      -1,   675,    -1,   676,    -1,   245,   278,   645,   277,   645,
     279,    -1,   246,   278,   608,   277,   609,   279,    -1,   677,
      -1,   679,    -1,   244,   682,   678,    54,    -1,   244,   682,
     678,    53,   683,    54,    -1,   197,   681,   181,   683,    -1,
     678,   197,   681,   181,   683,    -1,   244,   680,    54,    -1,
     244,   680,    53,   683,    54,    -1,   197,   624,   181,   683,
      -1,   680,   197,   624,   181,   683,    -1,   645,    -1,   645,
      -1,   608,    -1,   218,    -1,   219,    -1,   220,    -1,   221,
      -1,   222,    -1,   223,    -1,   224,    -1,   225,    -1,     6,
      -1,    -1,    98,    -1,   206,    -1,   711,    -1,   711,    -1,
     711,    -1,   711,    -1,   711,    -1,   711,    -1,   711,    -1,
     711,    -1,   711,    -1,   711,    -1,   711,    -1,   711,    -1,
     711,    -1,   711,    -1,   711,    -1,   711,    -1,   711,    -1,
     711,    -1,   711,    -1,   711,    -1,   711,    -1,   711,    -1,
     711,    -1,   206,    -1,   712,    -1,   235,    -1,   238,    -1,
     236,    -1,   246,    -1,   249,    -1,   255,    -1,   251,    -1,
     245,    -1,   248,    -1,   254,    -1,   247,    -1,   256,    -1,
     257,    -1,   258,    -1,   268,    -1,   262,    -1,   263,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   537,   537,   539,   543,   544,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,   561,   562,   570,   574,   578,   582,   586,   591,
     592,   596,   598,   600,   604,   605,   609,   613,   615,   617,
     619,   621,   625,   628,   631,   634,   637,   644,   649,   654,
     659,   664,   669,   674,   680,   684,   685,   687,   689,   693,
     695,   697,   699,   703,   704,   711,   713,   715,   719,   720,
     724,   728,   729,   733,   735,   742,   746,   748,   753,   759,
     760,   762,   769,   770,   771,   775,   776,   782,   785,   791,
     792,   795,   798,   802,   806,   809,   814,   823,   827,   830,
     833,   835,   837,   839,   841,   843,   845,   847,   849,   854,
     858,   860,   862,   872,   876,   878,   889,   892,   895,   897,
     898,   904,   910,   912,   915,   919,   920,   925,   926,   931,
     932,   935,   936,   943,   961,   964,   967,   970,   974,   975,
     978,   979,   982,   987,   988,   991,   995,  1004,  1012,  1021,
    1026,  1027,  1030,  1037,  1038,  1041,  1042,  1046,  1048,  1050,
    1052,  1054,  1059,  1060,  1063,  1064,  1068,  1073,  1075,  1124,
    1181,  1185,  1186,  1189,  1190,  1193,  1195,  1199,  1200,  1201,
    1204,  1205,  1206,  1212,  1217,  1222,  1224,  1227,  1230,  1231,
    1235,  1236,  1243,  1247,  1251,  1260,  1266,  1267,  1270,  1272,
    1277,  1280,  1284,  1290,  1296,  1301,  1306,  1309,  1312,  1317,
    1318,  1319,  1321,  1323,  1328,  1329,  1332,  1333,  1337,  1342,
    1344,  1349,  1350,  1352,  1360,  1364,  1366,  1369,  1370,  1371,
    1372,  1375,  1379,  1383,  1390,  1398,  1402,  1409,  1411,  1413,
    1415,  1418,  1421,  1424,  1428,  1431,  1434,  1437,  1446,  1457,
    1467,  1477,  1487,  1490,  1493,  1496,  1499,  1500,  1504,  1510,
    1511,  1515,  1520,  1522,  1524,  1528,  1531,  1534,  1535,  1539,
    1543,  1544,  1547,  1552,  1555,  1558,  1560,  1564,  1569,  1570,
    1573,  1577,  1579,  1582,  1585,  1586,  1590,  1591,  1592,  1593,
    1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,
    1604,  1605,  1607,  1609,  1610,  1613,  1615,  1619,  1623,  1628,
    1632,  1637,  1641,  1645,  1647,  1651,  1658,  1661,  1666,  1671,
    1673,  1676,  1679,  1681,  1684,  1687,  1688,  1689,  1691,  1695,
    1699,  1702,  1705,  1707,  1709,  1714,  1717,  1720,  1721,  1725,
    1730,  1731,  1735,  1737,  1739,  1741,  1745,  1746,  1747,  1750,
    1754,  1758,  1763,  1804,  1811,  1819,  1822,  1825,  1826,  1830,
    1836,  1843,  1863,  1867,  1869,  1871,  1875,  1886,  1890,  1891,
    1895,  1896,  1904,  1907,  1914,  1918,  1924,  1928,  1933,  1938,
    1945,  1956,  1967,  1969,  1972,  1975,  1979,  1981,  1985,  1987,
    1989,  1991,  1993,  1995,  1997,  1999,  2001,  2003,  2005,  2007,
    2009,  2011,  2013,  2017,  2020,  2023,  2029,  2033,  2036,  2043,
    2045,  2047,  2050,  2053,  2058,  2062,  2068,  2069,  2073,  2082,
    2086,  2088,  2090,  2092,  2094,  2098,  2099,  2103,  2105,  2107,
    2109,  2115,  2118,  2120,  2124,  2132,  2133,  2134,  2135,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,
    2147,  2148,  2149,  2150,  2151,  2152,  2153,  2154,  2155,  2156,
    2157,  2158,  2161,  2163,  2167,  2169,  2174,  2180,  2182,  2185,
    2188,  2190,  2198,  2203,  2204,  2208,  2222,  2224,  2226,  2228,
    2242,  2252,  2254,  2257,  2259,  2264,  2268,  2270,  2272,  2274,
    2276,  2278,  2280,  2282,  2284,  2286,  2288,  2290,  2297,  2298,
    2301,  2302,  2305,  2310,  2317,  2318,  2322,  2331,  2337,  2338,
    2341,  2342,  2343,  2344,  2361,  2366,  2371,  2390,  2407,  2414,
    2415,  2422,  2426,  2432,  2438,  2446,  2451,  2456,  2460,  2465,
    2470,  2474,  2481,  2487,  2493,  2501,  2506,  2511,  2518,  2519,
    2520,  2523,  2524,  2527,  2528,  2529,  2536,  2540,  2552,  2557,
    2611,  2673,  2674,  2681,  2694,  2699,  2704,  2711,  2714,  2720,
    2721,  2722,  2726,  2731,  2736,  2747,  2748,  2749,  2752,  2756,
    2760,  2763,  2766,  2770,  2771,  2774,  2778,  2782,  2783,  2786,
    2789,  2792,  2794,  2797,  2801,  2803,  2806,  2807,  2811,  2812,
    2813,  2814,  2817,  2819,  2823,  2825,  2829,  2831,  2834,  2836,
    2838,  2842,  2844,  2846,  2850,  2852,  2855,  2858,  2862,  2864,
    2867,  2870,  2872,  2876,  2877,  2881,  2885,  2888,  2891,  2892,
    2897,  2905,  2909,  2911,  2913,  2917,  2920,  2923,  2924,  2928,
    2932,  2934,  2937,  2940,  2942,  2946,  2949,  2952,  2955,  2964,
    2967,  2970,  2973,  2976,  2979,  2982,  2988,  3000,  3015,  3019,
    3023,  3027,  3030,  3032,  3034,  3037,  3040,  3042,  3044,  3048,
    3050,  3052,  3056,  3058,  3062,  3064,  3068,  3069,  3073,  3074,
    3078,  3079,  3084,  3088,  3089,  3093,  3094,  3095,  3100,  3104,
    3106,  3109,  3112,  3115,  3116,  3120,  3122,  3126,  3129,  3134,
    3137,  3140,  3141,  3146,  3151,  3153,  3155,  3157,  3159,  3161,
    3163,  3166,  3172,  3175,  3178,  3179,  3185,  3188,  3191,  3194,
    3197,  3203,  3206,  3209,  3213,  3215,  3217,  3223,  3226,  3229,
    3230,  3234,  3236,  3239,  3240,  3244,  3246,  3248,  3252,  3253,
    3257,  3260,  3266,  3269,  3276,  3277,  3280,  3284,  3288,  3295,
    3296,  3299,  3304,  3309,  3310,  3314,  3318,  3319,  3322,  3323,
    3330,  3332,  3336,  3338,  3340,  3343,  3344,  3347,  3351,  3353,
    3356,  3358,  3364,  3366,  3369,  3373,  3374,  3379,  3381,  3384,
    3388,  3389,  3394,  3395,  3398,  3403,  3408,  3411,  3418,  3419,
    3421,  3422,  3424,  3428,  3429,  3431,  3433,  3437,  3439,  3443,
    3444,  3446,  3450,  3451,  3452,  3453,  3454,  3455,  3456,  3457,
    3458,  3459,  3464,  3466,  3468,  3470,  3472,  3474,  3476,  3478,
    3485,  3487,  3489,  3491,  3493,  3495,  3497,  3499,  3501,  3503,
    3505,  3507,  3509,  3511,  3513,  3515,  3519,  3520,  3526,  3528,
    3533,  3535,  3537,  3539,  3544,  3546,  3550,  3552,  3556,  3558,
    3560,  3562,  3566,  3570,  3574,  3576,  3580,  3585,  3590,  3597,
    3602,  3607,  3616,  3617,  3621,  3625,  3639,  3657,  3658,  3659,
    3660,  3661,  3662,  3663,  3664,  3665,  3666,  3668,  3670,  3677,
    3679,  3681,  3688,  3695,  3702,  3704,  3706,  3707,  3708,  3709,
    3711,  3713,  3717,  3721,  3737,  3753,  3757,  3761,  3762,  3766,
    3767,  3771,  3773,  3775,  3777,  3779,  3783,  3784,  3786,  3802,
    3818,  3822,  3826,  3828,  3832,  3836,  3839,  3842,  3845,  3848,
    3853,  3855,  3860,  3861,  3865,  3872,  3879,  3886,  3893,  3894,
    3898,  3904,  3905,  3906,  3907,  3910,  3912,  3914,  3917,  3924,
    3933,  3940,  3949,  3951,  3953,  3955,  3961,  3970,  3973,  3977,
    3978,  3982,  3991,  3994,  3997,  3999,  4003,  4009,  4010,  4013,
    4017,  4021,  4022,  4025,  4027,  4031,  4033,  4037,  4039,  4043,
    4045,  4049,  4052,  4055,  4058,  4060,  4062,  4064,  4066,  4068,
    4070,  4072,  4076,  4077,  4080,  4088,  4091,  4094,  4097,  4100,
    4103,  4106,  4109,  4112,  4115,  4118,  4121,  4124,  4127,  4130,
    4133,  4136,  4139,  4142,  4145,  4148,  4151,  4154,  4157,  4162,
    4163,  4169,  4170,  4171,  4172,  4173,  4174,  4175,  4176,  4177,
    4178,  4179,  4180,  4181,  4182,  4184,  4185,  4186
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
  "db_file", "file1", "file_desc1", "file_desc", "file_clause", 
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
  "exec_sql", "for_select", "for_exec_into", "exec_into", "if_then_else", 
  "post_event", "event_argument_opt", "singleton_select", "variable", 
  "proc_inputs", "proc_outputs", "variable_list", "while", "label_opt", 
  "breakleave", "cursor_def", "excp_hndl_statements", 
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
  "init_alter_db", "alter_db", "db_alter_clause", "alter_trigger_clause", 
  "new_trigger_type", "new_trigger_action", "drop", "drop_clause", 
  "data_type", "non_array_type", "array_type", "array_spec", 
  "array_range", "simple_type", "non_charset_simple_type", 
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
     347,   347,   348,   348,   349,   349,   350,   350,   350,   351,
     352,   353,   353,   354,   354,   355,   355,   356,   356,   356,
     357,   357,   357,   358,   359,   360,   360,   360,   361,   361,
     362,   362,   363,   363,   363,   364,   365,   365,   366,   366,
     367,   367,   368,   369,   370,   371,   372,   372,   373,   373,
     373,   373,   373,   373,   374,   374,   375,   375,   376,   377,
     377,   377,   377,   377,   378,   379,   379,   380,   380,   380,
     380,   381,   382,   383,   384,   384,   385,   386,   386,   386,
     386,   386,   387,   388,   389,   389,   389,   389,   390,   391,
     392,   393,   394,   394,   395,   395,   396,   396,   397,   398,
     398,   399,   400,   400,   400,   401,   401,   402,   402,   403,
     404,   404,   405,   406,   406,   407,   407,   408,   409,   409,
     410,   411,   411,   411,   412,   412,   413,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   413,   413,   413,
     413,   413,   413,   413,   413,   414,   414,   415,   416,   417,
     418,   419,   420,   421,   421,   422,   423,   424,   425,   426,
     426,   426,   427,   427,   427,   428,   428,   428,   428,   429,
     430,   430,   431,   431,   431,   432,   432,   433,   433,   434,
     435,   435,   436,   436,   436,   436,   437,   437,   437,   438,
     439,   440,   441,   442,   443,   444,   444,   445,   445,   446,
     447,   448,   449,   450,   450,   450,   451,   452,   453,   453,
     454,   454,   455,   455,   456,   457,   458,   459,   460,   460,
     461,   462,   463,   463,   463,   464,   465,   465,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   467,   467,   468,   469,   470,   470,   470,
     470,   470,   470,   470,   471,   472,   473,   473,   474,   474,
     474,   474,   474,   474,   474,   475,   475,   476,   476,   476,
     476,   476,   476,   476,   477,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,   478,   479,   479,   480,   480,   481,   482,   482,   482,
     483,   483,   484,   485,   485,   486,   486,   486,   486,   486,
     487,   488,   488,   489,   489,   490,   491,   491,   491,   491,
     491,   491,   491,   491,   491,   491,   491,   491,   492,   492,
     493,   493,   494,   494,   495,   495,   496,   496,   497,   497,
     498,   498,   498,   498,   498,   498,   498,   498,   498,   499,
     499,   500,   500,   500,   500,   501,   501,   502,   502,   502,
     503,   503,   504,   504,   504,   505,   505,   505,   506,   506,
     506,   507,   507,   508,   508,   508,   509,   509,   510,   510,
     510,   511,   511,   512,   512,   512,   512,   513,   513,   514,
     514,   514,   515,   515,   515,   516,   516,   516,   517,   518,
     519,   519,   520,   521,   521,   522,   523,   524,   524,   525,
     525,   526,   526,   527,   528,   528,   529,   529,   530,   530,
     530,   530,   531,   531,   532,   532,   533,   533,   534,   534,
     534,   535,   535,   535,   536,   536,   536,   537,   538,   538,
     538,   539,   539,   540,   540,   541,   542,   542,   543,   543,
     544,   545,   546,   546,   546,   547,   547,   548,   548,   549,
     550,   550,   550,   551,   551,   552,   552,   553,   553,   553,
     554,   554,   555,   555,   556,   556,   557,   558,   559,   560,
     561,   562,   563,   563,   563,   563,   564,   564,   564,   565,
     565,   565,   566,   566,   567,   567,   568,   568,   569,   569,
     570,   570,   571,   572,   572,   573,   573,   573,   574,   575,
     575,   576,   576,   577,   577,   578,   578,   579,   579,   580,
     580,   581,   581,   582,   583,   583,   583,   583,   583,   583,
     583,   583,   584,   584,   585,   585,   586,   587,   587,   588,
     588,   589,   589,   590,   591,   591,   591,   591,   591,   592,
     592,   593,   593,   594,   594,   595,   595,   595,   596,   596,
     597,   597,   598,   598,   599,   599,   600,   601,   602,   603,
     603,   604,   605,   606,   606,   607,   608,   608,   609,   609,
     610,   610,   611,   611,   611,   612,   612,   502,   613,   613,
     614,   614,   615,   615,   616,   617,   617,   618,   618,   619,
     620,   620,   621,   621,   621,   622,   623,   623,   624,   624,
     624,   624,   624,   625,   625,   625,   625,   625,   625,   626,
     626,   626,   627,   627,   627,   627,   627,   627,   627,   627,
     627,   627,   628,   628,   628,   628,   628,   628,   628,   628,
     629,   629,   629,   629,   629,   629,   629,   629,   629,   629,
     629,   629,   629,   629,   629,   629,   630,   630,   631,   631,
     632,   632,   632,   632,   633,   633,   634,   634,   635,   635,
     635,   635,   636,   637,   638,   638,   639,   639,   639,   640,
     640,   640,   641,   641,   642,   643,   644,   645,   645,   645,
     645,   645,   645,   645,   645,   645,   645,   645,   645,   645,
     645,   645,   645,   645,   645,   645,   645,   645,   645,   645,
     645,   645,   645,   646,   646,   646,   647,   648,   648,   649,
     649,   650,   650,   650,   650,   650,   651,   651,   651,   651,
     651,   652,   653,   653,   654,   655,   655,   655,   655,   655,
     656,   656,   657,   657,   658,   659,   660,   661,   662,   662,
     663,   664,   664,   664,   664,   665,   665,   665,   665,   665,
     665,   665,   665,   665,   665,   665,   666,   667,   668,   669,
     669,   670,   671,   671,   672,   672,   673,   674,   674,   675,
     675,   676,   676,   677,   677,   678,   678,   679,   679,   680,
     680,   681,   682,   683,   684,   684,   684,   684,   684,   684,
     684,   684,   685,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   705,   706,   707,   708,   709,   710,   711,
     711,   712,   712,   712,   712,   712,   712,   712,   712,   712,
     712,   712,   712,   712,   712,   712,   712,   712
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
       2,     3,     0,     1,     1,     2,     1,     4,     3,     3,
       1,     0,     1,     1,     2,     3,     4,     0,     1,     2,
       0,     1,     1,     5,     5,     3,     2,     0,     1,     3,
       1,     1,     6,     3,     2,     6,     2,     1,     2,     2,
       2,     0,     1,     1,     2,     0,     2,     0,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     2,     2,     2,
       5,     1,     2,     3,     2,     2,     0,     1,     1,     1,
       1,     3,     4,     8,     4,     0,     6,     1,     1,     2,
       2,     0,     3,     3,     1,     2,     2,     2,     8,     8,
       8,     8,     3,     0,     4,     0,     1,     3,     4,     1,
       3,     2,     3,     3,     1,     1,     0,     1,     2,     4,
       1,     1,     3,     1,     0,     2,     1,     6,     1,     1,
       3,     1,     2,     0,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     2,     1,     1,
       1,     2,     2,     1,     2,     3,     4,     2,     6,     4,
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
       2,     2,     0,     1,     2,     2,     4,     3,     2,     2,
       7,     1,     0,     1,     0,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,     2,     2,     1,     1,
       1,     1,     4,     5,     1,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     6,     5,     3,     0,     2,     2,     0,
       3,     0,     4,     1,     5,     4,     1,     4,     1,     2,
       2,     1,     1,     1,     2,     2,     2,     2,     0,     3,
       5,     1,     1,     2,     3,     1,     2,     3,     0,     1,
       1,     1,     5,     5,     6,     1,     1,     1,     2,     4,
       1,     0,     5,     1,     0,     3,     2,     1,     0,     2,
       0,     1,     0,     3,     1,     0,     1,     2,     1,     1,
       1,     1,     2,     2,     1,     2,     3,     1,     1,     3,
       3,     1,     2,     3,     1,     2,     0,     2,     1,     1,
       0,     1,     1,     1,     3,     2,     3,     0,     1,     3,
       4,     5,     1,     3,     4,     3,     0,     1,     3,     3,
       1,     1,     0,     1,     1,     4,     0,     2,     4,     0,
       3,     0,     2,     0,     2,     0,     9,    11,     0,     0,
       0,     0,     3,     2,     1,     0,     3,     5,     3,     2,
       5,     2,     1,     1,     1,     1,     1,     3,     1,     3,
       1,     0,     2,     1,     3,     1,     1,     1,     6,     1,
       0,     3,     0,     1,     3,     6,     3,     4,     2,     3,
       0,     1,     2,     1,     1,     1,     2,     1,     2,     1,
       2,     0,     3,     0,     1,     3,     1,     2,     0,     2,
       0,     2,     0,     4,     1,     2,     1,     1,     0,     1,
       3,     2,     1,     1,     2,     1,     4,     3,     1,     3,
       4,     0,     8,     5,     1,     1,     4,     4,     4,     1,
       1,     5,     5,     1,     3,     3,     1,     1,     1,     3,
       7,     7,     5,     3,     0,     1,     1,     1,     2,     0,
       1,     1,     1,     0,     3,     1,     3,     1,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     2,
       1,     3,     3,     1,     2,     3,     3,     3,     3,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     1,     1,     5,     6,
       3,     4,     5,     6,     3,     4,     3,     4,     3,     4,
       4,     5,     4,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     3,     3,    11,    11,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     4,     1,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     4,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     6,     1,     6,     1,
       4,     7,     2,     0,     4,     3,     6,     1,     1,     6,
       6,     1,     1,     4,     6,     4,     5,     3,     5,     4,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,   578,   117,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   578,   655,     0,     0,     0,     0,     0,
       0,     2,    11,    17,     8,     7,    15,    16,    13,    14,
       4,    10,    21,   560,    19,   565,   566,   567,     6,    18,
     559,   561,    20,   626,   622,    12,     9,   734,   735,    22,
     739,   740,     5,   472,     0,     0,     0,     0,     0,     0,
     406,   577,   580,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,     0,     0,    97,   632,   914,     0,    23,
     912,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   485,     0,   356,
      31,    39,    36,    38,   763,    37,   763,   999,  1001,  1003,
    1002,  1008,  1004,  1011,  1009,  1005,  1007,  1010,  1006,  1012,
    1013,  1014,  1016,  1017,  1015,     0,    33,     0,    34,     0,
      68,    70,   991,  1000,     0,     0,     0,     0,     0,     0,
       0,     0,   576,     0,     0,   973,   649,   654,     0,     0,
     585,     0,   691,   693,   993,     0,     0,     0,   109,   568,
     998,     0,     1,     3,     0,     0,   639,     0,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,     0,   434,   435,     0,   983,   413,
       0,   987,   410,   253,   990,     0,   693,   409,   384,   994,
     582,   575,   910,     0,   105,   153,   152,   106,   135,   202,
     775,   979,     0,   104,   147,   986,     0,   100,   253,   107,
     916,   124,   101,   187,   102,     0,   103,   763,   997,   108,
     148,   630,   631,     0,   915,   913,     0,    76,     0,   984,
     710,   493,   982,   486,     0,   492,   497,   487,   488,   495,
     489,   490,   491,   496,   321,     0,   255,    32,     0,    41,
     762,    40,     0,     0,     0,     0,     0,     0,   768,     0,
       0,     0,     0,     0,     0,     0,   574,   920,     0,   901,
     648,   648,   649,   661,   659,   972,   662,     0,   663,   653,
     649,     0,     0,     0,     0,     0,     0,   601,   594,   583,
     584,   586,   588,   589,   590,   597,   598,   591,     0,   692,
     992,   110,   253,   111,   187,   112,   763,   571,     0,     0,
     623,     0,   641,     0,     0,     0,     0,   411,   473,   376,
       0,   376,     0,   205,   412,   416,   407,   470,   471,     0,
     255,   226,   462,     0,   408,   425,     0,   382,   383,   482,
     581,   579,   911,   150,   150,     0,     0,     0,   162,   154,
     155,   134,     0,    98,     0,     0,   113,   255,   123,   122,
     125,     0,     0,     0,     0,     0,   975,    77,   205,   529,
       0,   736,   737,   494,     0,     0,     0,     0,   879,   907,
       0,   974,     0,   881,     0,     0,   908,     0,   902,   893,
     892,   891,   999,     0,     0,     0,   883,   884,   885,   894,
     895,   903,   904,  1003,   905,   906,     0,  1008,  1004,   909,
       0,     0,     0,     0,   862,   353,   748,   319,   857,   772,
     746,   882,   858,   896,   860,   861,   876,   877,   878,   897,
     859,   921,   922,   923,   937,   924,   939,   865,   863,   864,
     947,   948,   951,   952,   747,     0,     0,   979,     0,     0,
       0,   357,   203,     0,     0,     0,   765,     0,     0,    29,
      35,     0,     0,    45,    71,    73,   995,    69,     0,     0,
       0,   767,     0,    54,     0,     0,     0,     0,    53,   573,
       0,     0,     0,   658,   656,     0,   665,     0,   664,   666,
     671,   652,     0,   620,     0,   595,   606,   592,   606,   593,
     607,   613,   617,   618,   602,   587,   710,   743,   776,     0,
       0,   255,     0,     0,   570,   569,   625,   627,   632,   624,
     637,     0,   645,   170,     0,   475,   131,     0,   478,     0,
     479,   474,     0,   420,     0,   422,   421,     0,   423,     0,
     417,     0,     0,   256,     0,     0,   429,     0,   430,   376,
     463,     0,   469,     0,     0,     0,   466,   979,   387,   386,
     481,     0,   404,   151,     0,     0,   160,     0,   159,     0,
       0,   149,   163,   164,   166,   156,   529,   541,   516,   551,
     519,   542,   552,     0,   558,   520,     0,   548,     0,   543,
     555,   515,   538,   517,   518,   513,   376,   498,   499,   500,
     508,   514,   501,   510,   531,     0,   536,   533,   511,   548,
     512,   114,   253,   115,     0,     0,   126,     0,     0,   186,
     226,   384,   374,     0,   205,     0,    83,    85,     0,     0,
       0,   757,     0,     0,     0,     0,   846,   847,   848,     0,
     709,   780,   789,   792,   797,   793,   794,   795,   799,   800,
     798,   801,   796,   778,     0,   973,     0,   973,   898,     0,
       0,   973,   973,   973,     0,   899,   900,     0,     0,   962,
       0,     0,     0,     0,   867,   866,   655,     0,     0,   746,
     318,   996,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   261,   508,   531,     0,     0,   355,     0,   266,
       0,   764,    30,     0,     0,    46,    74,     0,     0,    28,
     754,     0,   770,   655,     0,   733,   754,     0,     0,     0,
       0,   572,     0,   648,     0,     0,   710,     0,   670,     0,
     563,     0,   562,   918,     0,   596,     0,   604,   600,   599,
       0,   610,     0,   615,   603,     0,   741,   742,     0,     0,
       0,   226,   374,     0,   636,     0,   643,     0,   621,     0,
     132,   171,   477,   376,   419,     0,   376,   377,   424,   374,
       0,   252,   374,   225,   980,   197,   194,     0,   207,   376,
     498,   374,     0,     0,     0,   224,   227,   228,   229,   230,
       0,   428,   468,   467,   427,   426,     0,     0,     0,   390,
     388,   389,   385,     0,   376,   180,   157,   161,     0,     0,
     165,     0,   526,   539,   540,   556,     0,   553,   558,     0,
     546,   544,   545,   137,     0,     0,     0,   509,     0,     0,
       0,     0,   547,   255,     0,   374,   127,   185,     0,   188,
     190,   191,     0,     0,     0,     0,   205,   205,    80,     0,
      87,    79,   527,   528,   976,   529,     0,     0,     0,   791,
     779,     0,     0,   849,   850,   851,     0,   783,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   964,   965,   966,   967,   968,   969,   970,   971,     0,
       0,     0,     0,   957,     0,     0,     0,     0,     0,   973,
     320,   875,   874,   749,   887,     0,   870,   978,   869,   868,
     871,   872,   873,   945,     0,   880,   774,   773,   359,   358,
       0,   259,   274,     0,   265,   267,   766,     0,     0,     0,
       0,     0,     0,    26,    55,    43,    63,    65,     0,     0,
      72,     0,   759,     0,   769,   973,     0,   759,    51,    49,
       0,     0,     0,   657,   660,     0,   672,   673,   677,   675,
     676,   690,     0,   703,   667,   669,   988,   564,   919,   605,
     614,   609,   608,     0,   619,   744,   745,   777,   374,     0,
       0,   628,   650,   629,   638,     0,   640,   644,   476,   150,
     177,   169,   172,   173,     0,     0,     0,   418,   374,   374,
     377,   264,   257,   266,   196,   376,     0,   377,   198,   193,
     199,     0,     0,   235,     0,   432,   377,   376,   374,   431,
       0,     0,     0,   403,   484,   181,   182,   158,     0,   168,
     917,     0,     0,     0,   531,     0,   554,     0,     0,   376,
     377,     0,   504,   506,     0,     0,     0,     0,     0,     0,
       0,   384,   266,   150,   129,   226,   183,   404,   973,   379,
     362,   363,     0,    84,     0,   205,     0,    89,     0,    86,
       0,     0,     0,   738,   981,     0,     0,   784,     0,     0,
     790,     0,     0,   782,   781,     0,   836,     0,   827,   826,
       0,   805,     0,   834,   852,   844,     0,     0,     0,   806,
     830,     0,     0,   809,     0,     0,     0,     0,     0,     0,
       0,   807,     0,     0,   808,     0,   838,     0,     0,   802,
       0,     0,   803,     0,     0,   804,     0,     0,   205,     0,
     925,     0,     0,   940,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   963,     0,     0,   961,     0,     0,   953,
       0,     0,     0,     0,     0,   886,   944,     0,   254,   273,
       0,   331,   354,   268,    67,    59,    60,    66,    61,    62,
       0,     0,     0,    24,    27,    43,    44,   529,   529,     0,
     751,   771,     0,     0,   750,    52,    50,    47,     0,     0,
     701,   675,     0,   699,   694,   695,   697,     0,     0,   688,
       0,   708,   611,   612,   616,   266,   184,   379,     0,   642,
       0,   178,     0,   174,     0,     0,     0,   414,   213,   208,
     889,   209,   210,   211,   212,     0,     0,   258,     0,     0,
     206,   214,     0,   235,   632,   231,     0,   433,   464,   465,
     395,   396,   392,   391,   394,   393,   376,   483,   377,   167,
     977,     0,     0,   522,     0,   521,   557,     0,   549,     0,
     138,     0,   502,     0,   530,   531,   537,   535,     0,   532,
     374,     0,     0,     0,   121,   130,   189,   376,     0,     0,
     375,     0,    99,     0,   119,   118,    95,     0,     0,    91,
       0,    88,     0,   842,   843,   786,   785,   788,   787,     0,
       0,     0,   655,     0,     0,   845,     0,     0,     0,     0,
       0,     0,   837,   835,   831,     0,   839,     0,     0,     0,
       0,   840,     0,     0,     0,     0,     0,     0,   931,   930,
       0,     0,   927,   926,     0,   935,   934,   933,   932,   929,
     928,     0,   943,   959,   958,     0,     0,     0,     0,     0,
       0,     0,   888,   260,     0,     0,   270,   271,     0,   979,
       0,     0,     0,     0,     0,     0,     0,  1001,  1006,     0,
       0,   352,   284,   279,     0,   331,   278,   288,   289,   290,
     291,   292,   299,   300,   293,   295,   296,   298,     0,   303,
       0,   346,   348,   347,     0,     0,     0,     0,     0,     0,
     979,    56,    57,     0,    58,    64,    25,   755,     0,   756,
     753,   758,   761,   760,     0,   732,     0,    48,   671,   686,
     674,   700,   696,   698,     0,     0,     0,     0,     0,   712,
       0,   375,   633,   634,   651,   180,   179,   175,     0,   377,
     890,   262,   263,   377,   377,   201,   215,   216,   376,     0,
     232,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     480,   524,     0,   525,     0,   136,     0,   201,   139,   140,
     142,   143,   144,   505,   507,   503,   534,   266,   404,   377,
     180,     0,     0,     0,   360,     0,   364,   376,    90,     0,
       0,    92,   531,     0,   828,     0,     0,   973,   854,   853,
       0,     0,   832,     0,     0,     0,     0,   841,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   946,   204,
     936,   938,     0,     0,   960,   955,   954,     0,   949,   950,
     710,   207,   269,     0,   307,     0,     0,     0,   302,     0,
     316,   301,   332,   333,     0,   989,   349,   350,     0,   280,
       0,   285,   282,   337,     0,     0,   304,     0,   294,   287,
     297,   286,   330,    42,   529,   710,   680,   701,   689,   687,
     702,   704,   706,   707,   718,   646,   377,   361,   635,   176,
     377,   146,   251,     0,     0,   192,   217,     0,     0,     0,
     235,   218,   221,   763,     0,   377,   401,   402,   398,   397,
     400,   399,     0,   523,   550,   145,   133,   141,     0,   376,
     248,   128,   377,     0,   710,   378,   365,     0,     0,    93,
      94,    81,     0,   813,   821,     0,   814,   822,   817,   825,
     829,   833,   815,   823,   816,   824,   810,   818,   811,   819,
     812,   820,   942,   941,   956,   703,     0,   276,   272,     0,
     305,     0,   321,     0,     0,     0,   334,     0,   345,     0,
       0,     0,     0,   340,   338,     0,     0,     0,   325,     0,
     326,     0,   752,   703,   682,   679,     0,     0,   714,   716,
     717,   711,     0,   249,   415,   195,   200,   219,   235,   763,
     222,   241,   234,   236,   405,   377,     0,   380,     0,   367,
     368,   370,   371,   703,   377,    78,    96,     0,   708,   275,
       0,   306,   324,     0,   309,     0,   315,     0,   344,   343,
     985,   342,   331,     0,     0,     0,     0,   317,     0,     0,
     708,     0,   678,   685,   705,   715,   718,   223,   241,     0,
     235,   237,   238,   250,   377,   701,   370,     0,     0,   708,
       0,   710,   712,     0,     0,     0,     0,   331,   351,   339,
     341,     0,   336,     0,   328,   327,   712,     0,   683,   722,
       0,   719,     0,   723,   235,     0,     0,   233,     0,   240,
       0,   239,   381,   373,   369,     0,   366,   120,   703,   626,
     277,     0,   322,   308,   312,   314,     0,     0,     0,   331,
     626,     0,   681,   713,   718,     0,   725,     0,   721,   724,
     220,     0,     0,   244,   243,   242,   701,   708,   639,     0,
     331,     0,     0,   331,   329,   639,   684,   720,     0,   731,
     247,   245,   246,     0,   712,   856,   323,   313,   331,   335,
     310,   647,     0,   728,     0,   727,   372,   626,   311,   726,
       0,     0,   639,   729,     0,   855,   730
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    20,    21,    22,   478,   125,   126,   127,   128,  1213,
     729,   724,    23,   140,   973,   974,   975,   976,   129,   130,
     483,   484,    24,    83,   387,   870,   645,   646,   647,  1106,
    1107,   247,    25,    75,    26,   158,    27,   376,    76,  1322,
     229,   380,   637,  1094,  1314,   545,   217,   372,  1080,  1507,
    1508,  1509,  1510,  1511,  1512,   223,   239,   214,   584,   215,
     368,   369,   370,   591,   592,   593,   546,   547,  1031,  1032,
    1033,  1252,  1067,   232,   323,   382,   858,   859,   860,   796,
     797,   798,  1625,   567,   468,  1370,   648,  1047,  1257,  1485,
    1486,  1487,  1631,   861,   569,   805,   806,   807,   808,  1275,
     809,  1780,  1781,  1782,  1854,   227,   321,   631,   202,   350,
     474,   562,   563,   960,   469,  1040,   963,   964,   965,  1395,
    1396,  1200,  1688,  1397,  1412,  1413,  1414,  1415,  1416,  1417,
    1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1695,  1426,
     434,   435,  1795,  1709,  1427,  1428,  1429,  1838,  1592,  1593,
    1702,  1703,  1430,  1431,  1432,  1433,  1588,    28,    29,   266,
     470,   471,   236,   325,  1099,  1100,  1101,  1654,  1739,  1740,
    1741,   863,  1524,   554,  1037,  1320,   234,   633,   359,   580,
     581,   822,   824,  1287,    30,    60,   787,   784,   344,   345,
     354,   355,   194,   195,   356,  1056,   575,   814,   199,   167,
     337,   338,   207,   582,  1288,    31,    97,   616,   617,   618,
    1081,  1082,   619,   713,   621,   622,  1074,  1447,   847,   623,
     714,   625,   626,   627,   628,   840,   629,   630,   837,    32,
      33,    34,    35,    36,   535,    37,   500,    38,    39,    62,
     211,   361,    40,   309,   310,   311,   312,   313,   314,   315,
     316,   758,   317,  1013,  1244,   520,   521,   763,   522,    41,
    1434,    43,   166,   536,   537,   243,  1474,  1023,   332,   542,
    1026,   778,    44,   735,   503,   299,  1248,  1618,   145,   146,
     147,   297,   507,   508,   509,   749,   746,   996,   997,   998,
    1714,  1772,  1807,   999,  1000,  1239,  1742,   152,  1237,  1241,
    1610,  1611,  1469,   391,  1615,  1809,  1722,  1810,  1811,  1812,
    1848,  1882,  1885,  1435,  1436,    47,    48,   392,  1437,    50,
      51,   526,  1438,   436,   437,    52,   982,  1448,  1220,  1451,
     269,   270,   475,   490,   491,   731,   438,   439,   529,   660,
     886,   661,   662,   663,   664,  1130,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   888,  1133,  1134,  1343,   698,
     674,   441,   442,   945,  1259,   443,   444,   445,   446,   447,
     448,   449,   651,    80,   245,   231,  1072,  1083,   753,   450,
     451,   452,   453,   454,   455,   456,  1563,   457,   458,   459,
     460,   461,   462,   936,   463,   690,  1187,   691,  1184,   929,
     298,   464,   465,   873,  1289,   946,   220,   793,  1113,   251,
     197,   248,  1759,   224,  1883,  1005,  1439,  1001,   131,   466,
     206,   208,   977,   700,   237,   159,   467,   133
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1486
static const short yypact[] =
{
    3619,   538,    44,  2486,   481,   765,   175,  1622,    28,  2200,
     687,   251,  6050,    44,   667,   429,  5107,   737,  5107,    49,
     535,   284, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486,   635, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486,  6676,  5107,  5107,  5107,  5107,  5107,
   -1486, -1486,   505,   777,  5107,  5107,  5107,   691,  5107,   546,
    5107,  5107, -1486,  5107,  5107, -1486,   781, -1486,   590, -1486,
   -1486,   823,  5107, -1486,  5107,  5107,  5107,   857,  5107,  5107,
    5107,  5107,   546,  5107,  5107,  5107,  5107, -1486,  5107,   531,
     677, -1486, -1486, -1486,   574, -1486,   574, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486,   720,   654,   812, -1486,   327,
   -1486, -1486, -1486, -1486,  5107,  5107,  5107,   820,   851,   861,
     536,   113,   828,   377,   480,   229,   818, -1486,  5107,   964,
    1028,   885, -1486,  5107, -1486,  5107,  5107,  5107, -1486, -1486,
   -1486,  5107, -1486, -1486,  1045,   248,   804,   862, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486,   809, -1486, -1486,   777, -1486, -1486,
     209, -1486, -1486,   799, -1486,   232, -1486, -1486,   486, -1486,
     922, -1486, -1486,   888, -1486,   905, -1486, -1486,  1093, -1486,
   -1486, -1486,   777, -1486, -1486, -1486,    95, -1486,   799, -1486,
   -1486,   476, -1486,  1051, -1486,  1055, -1486,   574, -1486, -1486,
   -1486, -1486, -1486,  1053, -1486, -1486,   919, -1486,  1057, -1486,
     944, -1486, -1486, -1486,   919, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486,  4341,  5107,   988, -1486,  5107, -1486,
   -1486, -1486,  3503,   427,  1008,  3806,  5107,  1071,   882,  1095,
    1100,  3503,  1019,  1044,  1058,  3806,   938, -1486,  5943, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486,  4008, -1486, -1486,
   -1486,  1015,  5107,  1114,  1006,   134,  5107,  1054, -1486, -1486,
    1028, -1486, -1486, -1486, -1486, -1486, -1486, -1486,  5107, -1486,
   -1486, -1486,   799, -1486,  1051, -1486,   574,  1105,  5943,  1062,
   -1486,  5943,  1186,    66,   991,   999,  1001,   862, -1486,  1236,
     840, -1486,  5107, -1486,   809, -1486, -1486, -1486, -1486,  5107,
     988,  2266,  1060,  3059,   993, -1486,  6676, -1486, -1486,   510,
   -1486, -1486, -1486,  1012,  1012,   777,  1161,   777,   139,   905,
   -1486, -1486,  6647, -1486,  5107,  5107, -1486,   988, -1486, -1486,
    1257,    84,  1013,  5107,  1282,  5107, -1486, -1486,   323,   437,
    1010, -1486, -1486, -1486,  1018,  1021,  1022,  1090, -1486, -1486,
    1023, -1486,  1025, -1486,  1027,  1029, -1486,  1030, -1486, -1486,
   -1486, -1486,  1031,  1032,  1099,  1104, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486,  1040, -1486, -1486,  4588,  1041,  1047, -1486,
    5943,  5943,  4255,  5107, -1486, -1486, -1486,  1043,  1048, -1486,
    1150, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486,  1052,  1039,  1049,  6647,  1064,
     641, -1486, -1486,  1056,  1313,   729, -1486,  5107,  1144, -1486,
   -1486,  5107,  5107,   336, -1486, -1486, -1486, -1486,  5107,  5107,
      83, -1486,  5107, -1486,  1270,  5107,  3503,  1191,  1063, -1486,
    5107,  4686,   372, -1486, -1486,  5943, -1486,  1280,  1074, -1486,
     118, -1486,   599, -1486,   355, -1486,   496, -1486,   496, -1486,
    1076, -1486,    87, -1486,  1173, -1486,    40, -1486, -1486,  1079,
    1072,   988,  1080,  1344, -1486, -1486,  1082, -1486,   473, -1486,
     623,  1178,  1167, -1486,  1284,  1289, -1486,   777, -1486,  1291,
   -1486, -1486,  1362, -1486,  1364, -1486, -1486,  1343, -1486,  6647,
   -1486,  6647,   736, -1486,  1380,  5107, -1486,  6383, -1486,   436,
   -1486,  5107,   813,   232,  1210,  1177,  1251,  1213, -1486, -1486,
   -1486,   160,  1253, -1486,  1193,   546, -1486,   777, -1486,  1375,
    1308, -1486,   139, -1486, -1486, -1486,   414,  1212, -1486, -1486,
   -1486,  1214, -1486,  1258,  1130, -1486,  1314,  1132,   518, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
    1131, -1486, -1486, -1486,   120,  1133,  1136,   346, -1486,  1132,
   -1486, -1486,   799, -1486,  1353,  1408, -1486,   777,   777, -1486,
    2266,   486, -1486,  1290, -1486,  1263,  1149, -1486,  6638,  5494,
    1293, -1486,  1301,  1152,  3589,  1154,  2149,  2656,  2688,  3337,
     338, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486,  3258,   236,  4933,   152, -1486,  5107,
    5943,   504,   784,   794,  1245, -1486, -1486,  5943,  3675,  1150,
     249,  1237,  5943,  4933,   191,   191,   667,   760,  1156,   384,
   -1486, -1486,  4933,  5943,  5107,  5943,  5943,  5943,  5943,  5943,
    5019,  1879, -1486, -1486,  1413,  1157,  5107, -1486,  5107,  1398,
    5107, -1486, -1486,  5145,  1262, -1486, -1486,  1235,  3806, -1486,
    1383,   763, -1486,   667,  1170, -1486,  1383,  5145,  1384,  1385,
    5107, -1486,   384, -1486,   402,  4396,  1254,  4933, -1486,  5107,
   -1486,   592, -1486, -1486,   145, -1486,  1255, -1486, -1486, -1486,
    5107,   348,  5107, -1486, -1486,  5107, -1486, -1486,  4933,  5107,
    1443,  2266, -1486,  5943,  1206,  5943,  1333,  1221, -1486,   777,
   -1486,   601, -1486, -1486, -1486,  1195, -1486, -1486, -1486,   136,
    5107, -1486, -1486, -1486, -1486,  1455, -1486,  1198,  1431, -1486,
    1448, -1486,  1389,  1391,   574, -1486, -1486, -1486, -1486, -1486,
    1458, -1486, -1486, -1486, -1486, -1486,  5107,  6557,   546,  1351,
    1352,  1355, -1486,   546, -1486,   859, -1486, -1486,  1321,   777,
   -1486,   534,  1327, -1486, -1486, -1486,   546, -1486,  1130,   512,
   -1486, -1486, -1486,  1444,   512,  1328,   512, -1486,   546,   546,
    1215,   546, -1486,   988,  5107, -1486,  1393, -1486,   830, -1486,
   -1486, -1486,   510,  1335,  5107,   835,   216, -1486, -1486,  1218,
    1471, -1486, -1486, -1486, -1486,   437,  5107,  1336,  3922, -1486,
   -1486,  1336,  3589, -1486, -1486, -1486,   125, -1486,   468,  1334,
    3675,  3675,  5943,  5943,  1315,  1219,   157,  1777,  5943,  2060,
     498,  2359,  2620,  5266,  2881,  2979,  3240,  5943,  5943,  1490,
    5943,  1222,  5943,  1225,   543,  5943,  5943,  5943,  5943,  5943,
    5943, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,  1437,
    1042,   151,  4933, -1486,  3675,  5943,   252,   185,  1228,   229,
   -1486, -1486, -1486, -1486,  1150,   642, -1486, -1486,  1480,   191,
     191,   487,   487, -1486,   858, -1486, -1486, -1486, -1486, -1486,
     873, -1486,  1318,  1494,  1398, -1486, -1486,  5107,  5107,  5107,
    5107,  5107,  5107,  1230, -1486,   394, -1486, -1486,  5145,  1382,
   -1486,   138,  1401,  5107, -1486,   229,  4933,  1401,  1230,  1238,
    5145,  3752,  1450, -1486, -1486,  2777,  1240,   931, -1486, -1486,
   -1486,  1247,  3675,  1452, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486,   575, -1486, -1486, -1486, -1486, -1486,   895,
    1335, -1486, -1486, -1486,  1150,  5107, -1486, -1486, -1486,  1012,
    1515, -1486,   601, -1486,  1250,  3675,  4509, -1486, -1486, -1486,
   -1486, -1486, -1486,  1398, -1486, -1486,  4509, -1486, -1486, -1486,
   -1486,   574,   574,  1283,  1256, -1486, -1486, -1486, -1486, -1486,
     102,   121,   193, -1486,  1521, -1486, -1486, -1486,  5107, -1486,
   -1486,   481,   917,  1365,  1413,  1260, -1486,  1193,   929, -1486,
   -1486,   652, -1486,  1272,  5107,   688,  1261,  1275,   546,  1276,
    1525,   486,  1398,  1012,  1289,  2266, -1486,  1253,   229,  1337,
    1350, -1486,  2231, -1486,   546,  1418,  1513, -1486,  6638, -1486,
     546,  1338,  1519, -1486, -1486,  1296,  1302, -1486,  3675,  3675,
   -1486,  3675,  3675, -1486,  1574,   167,  1150,  1306, -1486, -1486,
    1307,  1150,  5363, -1486, -1486, -1486,  1488,  1311,  1312,  1150,
     682,  1316,  1322,  1150,  5943,  5943,  1219,  5943,  5610,  1323,
    1324,  1150,  1332,  1339,  1150,  5943,  1150,  1341,  1345,  1150,
    1346,  1348,  1150,  1357,  1358,  1150,   567,   585, -1486,   596,
   -1486,   608,  5943, -1486,   665,   674,   730,   756,   788,   825,
    5943,  5943,  4933, -1486,  1537,   180,  1150,  1411,  4933, -1486,
    5943,  5943,  4933,  5943,  5943, -1486, -1486,  5107, -1486, -1486,
    5107,  6187, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
    5145,  1523,  5145, -1486,  1230,   394, -1486,   399,   399,   246,
   -1486, -1486,  4008,   932, -1486,  1230,  1238,  1354,  3752,  1342,
     562,  1360,  4396,  1477, -1486,  1478,  1482,  1540,  4933,  1842,
    1594,  1560, -1486, -1486, -1486,  1398, -1486,  1337,   571,  1366,
    1193,  1504,  1193, -1486,  3675,   129,  1081, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486,  4509,  4509, -1486,  1494,  5943,
   -1486,   868,  1489,  1283,   781, -1486,  3675, -1486, -1486, -1486,
    1514,  1516,  1517,  1518,  1520,  1522, -1486, -1486, -1486, -1486,
   -1486,  1368,   481, -1486,  1441, -1486, -1486,   512, -1486,  4509,
     171,   512, -1486,   512, -1486,  1413, -1486, -1486,  1372, -1486,
   -1486,   510,  1494,  1193, -1486,  1289, -1486, -1486,  4008,  1629,
   -1486,   282, -1486,  1377, -1486,  1366, -1486,  1378,   777,   194,
    1379, -1486,   777, -1486, -1486, -1486,  1574, -1486,  1574,  5943,
    1493,  1493,   667,  1381,   933, -1486,  1493,  1493,  5943,  1493,
    1493,   363,  1150, -1486,  1097,  5943,  1150,  1493,  1493,  1493,
    1493,  1150,  1493,  1493,  1493,  1493,  1493,  1493, -1486, -1486,
    1386,  6647, -1486, -1486,   874, -1486, -1486, -1486, -1486, -1486,
   -1486,   896,  1101, -1486, -1486,  4933,  4933,  1607,  1486,  1070,
     943,  1089,  1150, -1486,  6647,  1387, -1486, -1486,  1630,  1631,
    4817,    33,  1392,  1394,  1585,  5943,  1399,  1400,  5418,  5107,
    5107, -1486, -1486, -1486,  1620,  6081, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,   101, -1486,
    1402, -1486, -1486, -1486,  1591,  1403,  1405,  1406,  1410,  1409,
     386, -1486, -1486,  1557, -1486, -1486, -1486, -1486,  1510, -1486,
   -1486, -1486, -1486, -1486,  1280, -1486,  3752,  1354,  1685, -1486,
     931, -1486, -1486, -1486,  4396,   949,  5107,  5943,  3675,  1555,
    1494, -1486, -1486, -1486, -1486,   859, -1486, -1486,   130, -1486,
   -1486, -1486, -1486, -1486,  1150,  1672,   868, -1486,   847,  5107,
   -1486,  1623,   133,  1530,  1619,  1531,  1660,  1624,  1664,  1398,
   -1486, -1486,  1442, -1486,  1447, -1486,  1632,  1672,   171, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486,  1398,  1253, -1486,
     859,  1521,  1654,  1596, -1486,  1335, -1486, -1486, -1486,  1605,
     257, -1486,  1413,  1614,  1150,  1453,  1454,   229, -1486, -1486,
    1456,  1457,  1150,  1459,  1460,  5943,  5943,  1150,  1461,  1462,
    1463,  1465,  1466,  1467,  1468,  1469,  1470,  1472, -1486, -1486,
   -1486, -1486,  5943,  1473, -1486, -1486, -1486,  4933, -1486, -1486,
    1254,   163, -1486,  1670, -1486,  5696,  5107,  5943, -1486,  3675,
    1150, -1486, -1486, -1486,  1474, -1486, -1486, -1486,  5107, -1486,
     241, -1486,  1558, -1486,   410,  1479, -1486,  4139, -1486, -1486,
   -1486, -1486, -1486, -1486,   399,  1254,  5107,   438, -1486, -1486,
    1481, -1486,  1150,   338,   389, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486,  1484,  5107, -1486, -1486,  1639,  1666,  5107,
    1283, -1486, -1486,   574,  5107, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486,  1494, -1486, -1486, -1486, -1486, -1486,  1494, -1486,
   -1486, -1486, -1486,  4741,  1254, -1486, -1486,  5943,   777, -1486,
   -1486, -1486,   777, -1486, -1486,  5943, -1486, -1486, -1486, -1486,
    1150,  1150, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486,  1150, -1486, -1486,  1452,  4509, -1486, -1486,  1487,
   -1486,   703,  4341,   957,   137,  1492, -1486,  1678, -1486,  5107,
    5107,   481,   104, -1486, -1486,  1512,  1683,  3675, -1486,   741,
   -1486,  1491, -1486,  1452,  1495, -1486,  3675,  5943, -1486, -1486,
    1657, -1486,  1498, -1486, -1486, -1486, -1486, -1486,  1283,   574,
   -1486,  1648, -1486, -1486, -1486, -1486,  1521, -1486,  4741,  1502,
     931, -1486, -1486,  1452,  1150, -1486, -1486,  1089,  1560, -1486,
    1336, -1486,  1625,  4139, -1486,  1603, -1486,  4139, -1486, -1486,
   -1486, -1486,  6293,   241,  5943,  4139,   142, -1486,  4139,  5751,
    1560,  5107, -1486,   338, -1486, -1486,  5822, -1486,  1648,   140,
    1283,  1656,  1662, -1486, -1486,   562,  1524,  4741,  1700,  1560,
    1526,  1254,  1555,  1528,  4063,  1532,   854,  6293,  1534, -1486,
   -1486,  1107,   143,  1742, -1486, -1486,  1555,   970, -1486, -1486,
    1535,  1536,   770,  5107,  1283,   415,   415, -1486,  1747, -1486,
    1608, -1486, -1486, -1486,   931,  4741, -1486, -1486,  1452,  1667,
   -1486,  4139,  1534, -1486, -1486,  1744,  4139,  1756,  1759,  6293,
    1667,  5107, -1486, -1486,  5822,  1542, -1486,  5107, -1486, -1486,
   -1486,  1606,   173, -1486, -1486, -1486,   807,  1560,   804,   997,
    6293,   107,  5107,  6293, -1486,   804, -1486, -1486,  5107,  1738,
   -1486, -1486, -1486,  3675,  1555, -1486, -1486, -1486,  6293, -1486,
   -1486, -1486,  1544,  1547,  1548, -1486,   338,  1667, -1486, -1486,
    5107,  5107,   804, -1486,  1549, -1486, -1486
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -1486, -1486, -1486, -1486,   -61,   222, -1486,   314,  1554,   616,
   -1486,  -360, -1486, -1486,  -608,  -334,  -603, -1039,  1821,  1561,
    1556,  1106, -1486, -1486, -1486,   731, -1486,  1199,   975, -1486,
     739, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486,   751, -1486, -1486, -1486, -1486,
   -1486,   339, -1486, -1486,  1509, -1486, -1486, -1486,  -304, -1486,
   -1486, -1486,  1496, -1486, -1486,  1264,  -343, -1486, -1486, -1486,
     826, -1486, -1238, -1486, -1486,  1533,  1084,   755,  1511,  1061,
     758, -1486,   356,   -56,  -239, -1486,  -314,   293,  -906, -1486,
   -1486,   380, -1486,  1529, -1050, -1486, -1486, -1486, -1486, -1230,
     379,    90,    88,    91,    55, -1486, -1486, -1486, -1486,  -112,
    -278, -1486,  1083, -1486,  -648, -1486,  -931, -1486,   910, -1486,
   -1486, -1486, -1486, -1486, -1315,  -905, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1272,   183, -1486,  -636, -1486, -1486, -1486, -1486, -1486,   287,
   -1486,   122, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486,  1160, -1486, -1486,   863, -1486, -1157, -1486, -1486, -1021,
     146,  -595,   416,  -322,  -851,   639, -1486, -1486,  -544,  -774,
   -1486, -1486,  -990, -1298, -1486, -1486, -1486, -1486, -1486,  1546,
   -1486,  1319,  1538, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486,  1551, -1486, -1486, -1486, -1486, -1486,  -536,  -444, -1486,
    1050,   594,  -553,  -352, -1486, -1486, -1486,  -321,  -960, -1486,
    -349, -1486, -1486, -1486, -1486,  1268, -1486, -1486,  1065, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,  1878,
   -1486, -1486, -1486, -1486, -1486,  1588, -1486, -1486, -1486,  1388,
   -1486,  1412, -1486, -1486, -1486, -1486,  1147, -1486, -1486, -1486,
       4, -1486,  -993, -1486,  1143,  -499, -1486, -1486,  -891, -1486,
   -1486, -1486,    48,  -840,  -173,   643, -1486, -1486,  -630, -1486,
    1772,  -872, -1032, -1486,  1181, -1059, -1255, -1486,  -924, -1486,
   -1486, -1486, -1486,   925, -1486, -1486,   648,    35,  -947, -1485,
   -1486,   212, -1069,  -514, -1144,   317, -1486,    89, -1486,   119,
   -1486,  -837, -1486,  1934,  1935, -1486, -1486,  1414,  1936, -1486,
   -1486, -1486,  -194,  -280,  -394, -1486,  1201, -1097,   951, -1486,
     -95,  -673,  -408, -1486, -1486, -1486,  -842,   -38,  -437,  -666,
   -1486,  -451, -1486, -1486, -1486,   179, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486,  -485, -1486,   795, -1486,   605, -1486,
    1007,  -944, -1486,  -599, -1486,   693,  -942,  -130,  -927,  -916,
    -880,   -30,     1,    14, -1486,   -77,  -531,  -472,  -109, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486, -1486,
   -1486, -1486, -1486, -1486, -1486, -1486,   753, -1486, -1033, -1486,
     503,  -874,   580, -1486,   860,   329,  -695,  1376, -1158, -1486,
     -59,  1869, -1486,   -26,   -54, -1074,   550,   -47,   -42,  -135,
     -15,   -41,  -184, -1486,   -67,   -29,    -9, -1486
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1015
static const short yytable[] =
{
     132,   153,   200,   132,    42,    79,   222,   154,   218,   160,
     203,   271,   766,   290,   293,   259,   957,   510,   319,   557,
     620,   228,   931,   624,   712,   594,   219,   253,   262,   559,
     235,   799,   240,   216,   291,   294,   257,  1115,   697,   774,
     752,  1116,  1398,  1490,   258,   196,   198,   201,   204,   154,
     209,   264,   732,   261,   263,   221,   198,   225,  1202,   204,
     585,   154,   209,   256,   238,   132,   939,  1193,   650,   153,
     961,  1230,   564,   249,  1017,   154,   252,   198,   260,   249,
     225,   201,   204,   230,   154,   209,   238,   132,  1097,   204,
     326,   485,  1258,   205,  1260,   871,   277,   862,   279,   635,
    1591,   485,  1258,   985,  1260,   233,   230,  1317,   322,  1261,
     561,   954,  1268,  1222,  1295,   788,   377,   789,   504,  1261,
    1262,  1450,   301,   800,   527,   221,   154,   221,   748,   988,
    1262,  1053,   327,  1118,   989,   738,  1570,   890,   890,   225,
    1270,   890,   384,   845,   320,   890,   204,   154,   238,  1383,
     890,   761,   160,  1837,  1762,  1387,  1263,  1878,   295,   890,
     543,  1312,  1264,   516,  1526,  1594,  1263,   346,  1282,   880,
     278,  1442,  1264,  1445,   516,  1339,    98,  1020,   638,   285,
    1466,  1576,  1038,   530,  -668,   589,  1280,  1815,   890,  1267,
    1454,   324,   373,   704,  1041,  -376,  1271,  1043,   705,  1605,
    1748,   910,   780,   879,  1217,  1277,  1050,   819,   887,  1046,
     531,   704,   347,   330,  1530,   620,   705,   704,   624,  1871,
     494,  1488,   705,  1652,  1123,  1124,  1318,   472,  1770,  1300,
     476,   533,  1003,   543,   138,   295,   351,  1619,   390,   352,
    1284,    84,   295,   374,   820,    61,   733,   810,   513,   594,
    1698,  1586,  1587,   770,   329,  1135,   221,  1119,  1789,   221,
    1092,   891,   891,   154,   517,   891,   486,   132,  1185,   891,
     136,  1872,   154,   734,   891,   832,   486,  1285,   296,   375,
     528,  1136,  1651,   891,   353,   907,  1522,  1577,  1525,   348,
    1281,   212,   213,   201,   843,  1506,    99,   154,   726,  1699,
    1595,   161,   932,   933,   558,  1188,  1189,   479,  1460,  1283,
    1700,   472,   891,   219,  1470,   572,   479,   765,   576,   518,
    1218,  1258,  1258,  1260,  1260,  1708,   139,   632,  1816,   544,
     518,   643,  1182,   221,   634,   586,   519,   588,  1261,  1261,
     221,   523,   221,  1857,   221,  1515,   890,   577,   821,  1262,
    1262,   639,  1564,  1565,   530,  1258,  1104,  1260,  1659,  1481,
    1482,  1385,   283,  1483,   762,   204,   209,  1078,  1656,  1255,
    1214,  1545,  1261,  1057,   154,  1215,   201,   539,  1272,  1273,
     992,  1763,  1225,  1262,  1768,  1263,  1263,  1226,  1227,   704,
     276,  1264,  1264,  1505,   705,  -668,   909,  1117,   704,  1606,
    1730,   846,   590,   705,  1120,  1039,  1531,  1519,  1479,  1620,
     704,    14,  1635,   938,  1701,   705,  1755,  1445,   641,  1263,
    1768,  1803,   943,  1245,   701,  1264,   911,   879,   704,  1324,
    1697,   879,  1686,   705,   725,   739,  1488,  1500,  1784,   706,
     707,   708,   709,  1265,  1266,  1098,   934,  1799,   725,  1190,
    1070,   528,  1335,  1336,   284,  1337,  1338,   706,   707,   708,
     709,  1034,  1191,  1279,  1036,   708,   709,   510,   154,  1705,
     891,   741,   204,   486,   101,   825,  1121,  1048,  1718,   154,
     -82,  1708,  1835,   154,   241,  1708,   204,   154,  1016,   357,
     378,   160,  1791,  1708,  1105,  1660,  1804,  1011,  1777,   704,
     148,   802,  1064,  1719,   705,  1233,   754,  1712,   826,   275,
     295,   103,   722,   704,  1012,   578,  1144,   781,   705,  1234,
     853,   242,  1708,   730,  1864,   307,   579,   736,  1649,   801,
     289,   479,  1145,  1344,  1684,   162,   727,  1518,  1851,   850,
    1607,   841,   100,  1235,   485,  1877,  1221,  1311,  1880,  1393,
    1817,   561,  1108,   915,  1112,   871,   794,   827,   221,  1708,
     163,  1720,   794,  1888,  1708,  1616,   358,  1716,  1642,   704,
     993,  1015,  1661,    14,   705,  1090,   649,  1146,    53,  1852,
     104,   287,   803,   101,  1850,   958,  1648,   379,  1478,    54,
     105,   649,  1223,   704,  1211,   102,    55,  1236,   705,   230,
    1122,   644,   219,    77,   276,   149,  1147,   856,   857,   842,
    1492,   704,   150,   728,   649,   106,   705,  1249,    77,   756,
     103,    56,   704,   804,   851,  1457,  1853,   705,  1621,  1233,
     530,   221,  1622,  1623,   704,   706,   707,   708,   709,   705,
     874,    77,  1008,  1234,   706,   707,   708,   709,  1829,   704,
     872,   743,  1183,   913,   705,   288,   706,   707,   708,   709,
     210,   289,  1840,   942,   151,  1665,  -989,  1235,  1650,  -992,
     225,  1212,    78,  1148,   706,   707,   708,   709,   472,  1792,
     472,   994,   966,   289,   287,    77,    57,    78,  1453,   104,
     757,   704,   831,   725,  1325,   947,   705,  1808,   226,   105,
     704,  1806,   221,  1029,  1879,   705,   134,   221,   704,   221,
      78,   221,  1537,   705,   486,  1517,   287,  1785,    58,   486,
    1826,  1236,    59,  1269,   106,  1250,  1242,   528,   486,   704,
    1887,   204,   250,   219,   705,  1278,   204,  1734,  1070,  1348,
    1006,  1059,  1258,  1735,  1260,   706,   707,   708,   709,  1028,
      77,   154,   472,   154,    78,  1710,   704,  1299,   292,  1261,
     221,   705,   221,  1503,   289,  1723,  1824,  1866,   164,  1724,
    1262,  1086,  1087,   135,  1089,  1491,  1030,  1243,  1055,  1058,
    1749,   221,   704,  1204,  1733,  1077,  1207,   705,  1874,  1313,
     295,  1108,   241,  1788,   244,   523,   287,  1014,  1390,  1069,
     295,  1737,  1613,   287,  1856,   775,  1263,   221,   221,   265,
    1472,  1071,  1264,   339,   704,   706,   707,   708,   709,   705,
    1473,   165,  1173,  1007,   267,  1504,   388,    81,    82,   242,
     750,  1514,   230,   917,   393,  1559,  1858,  1063,  1788,   706,
     707,   708,   709,   919,  1465,   154,  1368,  1865,   530,   272,
    1075,   704,   268,  1845,  1371,   154,   705,   706,   707,   708,
     709,   340,   230,   230,  1369,   230,   333,  1114,   706,   707,
     708,   709,   751,   555,  1233,  1372,  1441,  1788,  1444,   334,
     706,   707,   708,   709,  1783,   155,   556,  1373,  1234,  1091,
    1846,   246,  -226,  1790,  1892,   706,   707,   708,   709,  1102,
     704,   565,  1183,  1847,  1208,   705,   143,   144,  1183,  1788,
    1793,  1710,  1235,  1694,   335,  1710,   336,   156,   716,  1194,
     717,  1205,   704,  1710,  1195,   254,  1805,   705,  1206,  1301,
    1209,   273,   157,  1822,  1302,   505,  1873,   706,   707,   708,
     709,   274,   510,   511,  1375,   528,   706,   707,   708,   709,
    1571,   280,  1710,  1376,   706,   707,   708,   709,   486,   204,
     209,   486,   238,   132,  1499,  1301,  1236,  1875,  1008,   486,
    1305,  1627,   780,   341,  1881,   706,   707,   708,   709,  1751,
     281,   486,   486,   704,   212,   213,   204,   476,   705,  1710,
     282,   342,  -226,  1628,  1710,  1521,  1065,  1066,  1233,  1229,
    1629,  1895,   706,   707,   708,   709,   720,   363,   721,  1377,
     286,  1308,  1234,   790,  -226,   791,   221,  1767,  1768,   620,
    -701,  -226,   624,   394,   812,   343,   813,  1326,   706,   707,
     708,   709,   395,  1330,  1630,  1378,  1235,   702,   510,   940,
     983,  1766,   984,  1753,   364,   396,   365,   302,   652,   318,
    1773,   397,   398,  1893,  1894,  -226,  1685,   219,   144,  1290,
     706,   707,   708,   709,   476,   328,   530,  1379,   704,   366,
     653,   331,  1291,   705,   957,  1290,  1138,   349,  1142,   399,
    1150,  1153,   400,  1158,  1161,  1164,   221,  1449,  1449,  1452,
    1236,  1713,   360,   221,   367,   362,   704,   706,   707,   708,
     709,   705,   230,   371,  1380,  1183,  1183,  1095,  1181,  1096,
     402,   403,   867,   381,  1103,   704,   303,  1796,   230,   383,
     705,  1798,   404,   704,   230,   386,   405,   704,   705,  1802,
    1834,  1768,   705,   704,   654,  1194,   385,  1196,   705,   389,
    1743,  1475,   390,  1477,  1394,   473,   706,   707,   708,   709,
    1197,   304,  1198,  1560,  1546,   745,   481,   488,  1832,   472,
     489,   492,   219,   528,   493,  1562,   810,   495,   706,   707,
     708,   709,  1095,   496,  1246,  1561,   704,   655,   908,   305,
     912,   705,   306,   406,   916,   918,   920,   497,   221,   407,
     499,  1399,  1440,  1836,  1292,  1859,  1293,   512,   307,   408,
    1861,   486,   515,   486,  1520,  1657,  1297,  1886,  1298,   702,
    1194,  1455,  1539,   409,   410,   411,   412,   212,   213,   486,
     702,   514,  1569,   204,   308,    14,   702,   413,  1608,   706,
     707,   708,   709,  1754,   524,   534,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   108,   423,  1841,   110,  1842,
     541,   424,   425,   548,   426,   427,   428,   113,   114,   115,
     429,   116,   549,   550,   117,   118,   656,   657,   658,   552,
     573,   440,   122,   123,  1768,   570,  1876,  1828,   124,   587,
     530,   583,   430,   431,   409,   410,   411,  1183,   659,   636,
     433,   640,   642,  1502,   289,   502,   675,   678,  1529,   676,
     677,   679,  1533,   680,   440,   681,   685,   682,   683,  -975,
     684,   686,   419,   420,   706,   707,   708,   709,   687,   692,
     702,  1127,   711,   719,  1128,   693,   723,  1736,   394,   703,
     710,  1609,  -992,   715,   718,   538,   737,   395,   540,   740,
     728,  1575,   706,   707,   708,   709,   745,   764,   768,  1568,
     396,   747,   892,   760,   772,   769,   397,   398,   771,   773,
     704,   706,   707,   708,   709,   705,   776,   777,   893,   706,
     707,   708,   709,   706,   707,   708,   709,   528,   779,   706,
     707,   708,   709,   543,   399,   782,   783,   400,   785,   786,
     792,   198,   816,   817,   818,  -435,   823,   287,   828,  1585,
    1114,  1114,   829,   835,   894,   833,  1440,   834,   836,   838,
     839,   848,   844,   895,   849,   402,   403,   854,   855,   864,
     866,   896,   706,   707,   708,   709,   867,   404,   875,   876,
     877,   405,   881,   689,   935,   941,   845,   694,   695,   699,
     897,   289,   898,   962,   978,   979,   981,   486,   986,  1009,
     990,   991,  1002,  1018,  1022,   204,   899,   320,   900,   901,
     902,  1025,  1711,   921,   922,   923,   924,   925,   926,   927,
     928,  1715,  1027,  1035,  1449,  1044,  1045,  1046,   795,  1051,
     154,  1052,  1054,  1060,  1061,  1068,  1129,  1062,   406,  1073,
    1079,  1111,  1084,  1088,   407,  1093,  1110,  1132,  1098,   733,
    1168,  1170,  1172,  1180,   408,  1192,   704,  1210,   742,   903,
    1199,  1201,   744,  1216,  1219,  1212,  1228,  1232,   409,   410,
     411,   412,   212,   213,  1633,  1238,  1240,  1251,  1254,  1692,
    1274,  1286,   413,  1294,  1276,  1310,  1321,  1319,  1731,  1296,
    1306,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     108,   423,  1303,   110,  1307,  1309,   424,   425,  1104,   426,
     427,   428,   113,   114,   115,   429,   116,   204,  1328,   117,
     118,   119,   120,   121,  1332,  1333,  1331,   122,   123,  1114,
    1732,  1334,   890,   124,  1340,  1341,  1345,   430,   431,  1346,
    1347,  1384,  1386,   501,  1349,   433,  1443,   320,  1706,   289,
    1350,  1357,  1358,   904,   905,   906,   706,   707,   708,   709,
    1359,  1461,  1462,   942,  1467,   947,  1463,  1360,  1711,  1362,
     154,  1458,  1711,  1363,  1364,   201,  1365,   530,  1745,  1464,
    1711,  1456,  1746,  1711,  1778,  1366,  1367,  1468,   153,  1459,
    1758,  1476,  1489,   720,   154,  1070,  1493,  1501,  1494,  1495,
    1496,  1516,  1497,  1523,  1498,  1527,  1342,  1528,  1532,  1711,
    1538,  1566,   530,  1572,  1729,  1558,   889,  1567,  1578,  1573,
    -981,   135,  1579,    85,  1589,  1581,  1582,  1597,  1596,  1598,
      86,  1599,  1600,   440,    87,    88,  1601,   914,  1603,  1602,
     198,  1760,  1604,    89,   930,   748,  1711,  1614,  1624,   937,
     440,  1711,  1761,  1637,   530,    90,  1634,  1639,  1640,   440,
     944,  1641,   948,   949,   950,   951,   952,   944,  1636,  1638,
    1653,  1643,  1658,   153,   528,   530,  1644,  1655,   530,   154,
    1645,  1662,  1663,  1664,  1689,  1666,  1667,  1727,  1668,  1669,
    1672,  1673,  1674,   530,  1675,  1676,  1677,  1678,  1679,  1680,
    1696,  1681,  1683,  1440,   440,  1590,  1728,  1707,  1717,   528,
     221,  1813,  1006,  1725,  1757,  1750,  1764,   154,  1756,  1765,
      91,  1775,   153,  1771,  1769,   440,  1776,  1779,   154,  1787,
     538,  1794,  1024,  1137,  1797,  1818,  1128,    92,  1440,  1825,
     394,  1820,  1839,  1869,  1815,  1862,  1816,  1860,  1813,   395,
     164,   528,    93,  1823,   154,  1827,    94,  1830,  1833,  1863,
     153,  1768,   396,  1844,  1843,  1870,   154,    95,   397,   398,
    1868,  1884,   528,  1889,  1890,   528,  1891,   480,  1896,  1813,
    1440,  1446,  1006,   141,   980,   154,    96,   487,   201,  1329,
     528,   498,  1109,   865,  1327,  1315,   399,  1647,   553,   400,
    1316,  1440,   748,  1114,  1440,  1019,   830,   532,  1253,   201,
    1323,  1049,   566,  1646,  1687,   595,  1626,  1632,  1814,  1440,
    1821,  1855,  1819,  1042,  1203,  1752,   959,   402,   403,  1704,
     568,   201,   201,  1247,  1786,  1800,  1471,  1617,   551,   404,
     560,   142,   815,   405,   574,  1513,  1085,   852,   525,  1125,
    1126,  1131,   755,  1076,  1139,  1140,  1143,  1010,  1151,  1154,
    1156,  1159,  1162,  1165,  1166,  1167,  1021,  1169,   300,  1171,
    1231,   955,  1174,  1175,  1176,  1177,  1178,  1179,  1004,  1774,
     759,  1721,  1849,  1867,    45,    46,    49,   987,  1224,   440,
     767,  1353,  1186,  1388,  1304,  1535,  1536,   811,  1129,  1480,
     406,  1540,  1541,  1726,  1543,  1544,   407,   255,  1584,     0,
       0,     0,  1548,  1549,  1550,  1551,   408,  1552,  1553,  1554,
    1555,  1556,  1557,     0,     0,     0,     0,     0,     0,     0,
     409,   410,   411,   412,   212,   213,     0,     0,     0,     0,
       0,     0,     0,   440,   413,     0,     0,     0,     0,     0,
       0,     0,     0,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   108,   423,     0,   110,     0,     0,   424,   425,
       0,   426,   427,   428,   113,   114,   115,   429,   116,     0,
       0,   117,   118,   119,   120,   121,     0,     0,     0,   122,
     123,     0,     0,     0,     0,   124,     0,     0,  -671,   430,
     431,     0,     0,     0,     0,   501,     0,   433,     0,     0,
       0,   289,     0,     0,     0,     0,  1141,     0,     0,  1128,
       0,     0,     0,   394,     0,     0,     0,  -671,  -671,     0,
    -671,     0,   395,     0,     0,   107,     0,  -671,  -671,  -671,
    -671,  -671,     0,  -671,     0,   396,  -671,  -671,  -671,  -671,
    -671,   397,   398,     0,  -671,  -671,     0,     0,     0,     0,
    -671,     0,     0,     0,   108,   109,     0,   110,     0,     0,
       0,     0,     0,     0,   111,   112,   113,   114,   115,   399,
     116,     0,   400,   117,   118,   119,   120,   121,     0,   944,
       0,   122,   123,     0,     0,     0,     0,   124,     0,     0,
       0,  1351,  1352,   956,  1354,  1356,     0,     0,     0,     0,
     402,   403,  1361,     0,     0,     0,     0, -1012,     0,     0,
       0,     0,   404,     0,     0, -1012,   405,     0,     0,  1374,
   -1012,     0,     0, -1012,     0,     0,     0,  1381,  1382,   440,
       0,     0,     0,     0,     0,   440,     0,  1186,  1389,   440,
    1391,  1392,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0, -1012,
       0,     0,     0,     0,     0,     0,     0,     0, -1012,   440,
       0,  1129,     0,   406,     0,     0, -1012,     0,     0,   407,
       0,     0,     0,     0,     0,   440,     0,   101,     0,   408,
       0,     0,     0,     0,     0, -1012,     0, -1012,     0,   102,
       0,   795,     0,   409,   410,   411,   412,   212,   213,     0,
       0, -1012,     0, -1012, -1012, -1012,  1484,   413,     0,     0,
       0,     0,     0,     0,   103,     0,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   108,   423,     0,   110,   565,
       0,   424,   425,     0,   426,   427,   428,   113,   114,   115,
     429,   116,     0,     0,   117,   118,   119,   120,   121,     0,
       0,     0,   122,   123, -1012,   440,     0,     0,   124,     0,
       0,     0,   430,   431,     0,     0,     0,     0,   501,     0,
     433,     0,     0,     0,   289,     0,  1534,     0,     0,     0,
       0,     0,     0,   104,     0,  1542,     0,     0,     0,     0,
       0,     0,  1547,   105,     0,  1149,     0,     0,  1128,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,   106,     0,
       0,     0,   440,   440,   396,     0,     0,     0,     0,     0,
     397,   398,     0,     0,     0,     0,   107,     0,     0,     0,
       0,     0,  1580,     0,     0,     0,     0,     0, -1012, -1012,
   -1012, -1012, -1012, -1012, -1012,     0,     0,     0,   399,     0,
   -1012,   400, -1012,     0,     0,   108,   109,   107,   110,     0,
       0,     0,     0,     0,     0,   111,   112,   113,   114,   115,
       0,   116,     0,     0,   117,   118,   119,   120,   121,   402,
     403,     0,   122,   123,     0,     0,   108,   109,   124,   110,
       0,   404,   107,     0,  1612,   405,   111,   112,   113,   114,
     115,     0,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,     0,     0,     0,     0,   124,
       0,   108,   109,     0,   110,     0,     0,     0,     0,   268,
       0,   111,   112,   113,   114,   115,     0,   116,     0,     0,
     117,   118,   119,   120,   121,     0,    63,     0,   122,   123,
    1129,     0,   406,     0,   124,     0,     0,    64,   407,     0,
       0,     0,     0,     0,    65,     0,     0,     0,   408,     0,
       0,     0,  1670,  1671,     0,     0,     0,    66,     0,     0,
       0,     0,   409,   410,   411,   412,   212,   213,     0,  1682,
       0,     0,     0,     0,   440,     0,   413,     0,     0,     0,
       0,     0,  1691,     0,  1693,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   108,   423,     0,   110,     0,     0,
     424,   425,     0,   426,   427,   428,   113,   114,   115,   429,
     116,     0,     0,   117,   118,   119,   120,   121,    67,     0,
       0,   122,   123,     0,     0,     0,  1152,   124,     0,  1128,
       0,   430,   431,   394,    68,     0,     0,   501,     0,   433,
       0,     0,   395,   289,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,   396,     0,     0,     0,     0,
       0,   397,   398,     0,  1744,     0,    70,     0,     0,     0,
      71,     0,  1747,    72, -1013,     0,     0,     0,     0,     0,
       0,    73, -1013,     0,     0,     0,     0, -1013,     0,   399,
   -1013,     0,   400,     0,     0,     0,     0,     0,     0,   440,
      74,     0,     0,     0,     0,     0, -1014,     0,     0,     0,
       0,     0,     0,     0, -1014,     0,     0,     0,     0, -1014,
     402,   403, -1014,     0,  1612,     0, -1013,     0,     0,     0,
       0,     0,   404,     0,     0, -1013,   405,     0,     0,     0,
       0,     0,     0, -1013,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0, -1014,     0,
       0,     0, -1013,     0, -1013,     0,     0, -1014,     0,     0,
       0,  1801,     0,     0,     0, -1014,     0,     0, -1013,     0,
   -1013, -1013, -1013,     0,     0,     0,     0,     0,     0,     0,
       0,  1129,     0,   406, -1014,     0, -1014,     0,     0,   407,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   408,
   -1014,     0, -1014, -1014, -1014,     0,     0,     0,     0,     0,
       0,     0,     0,   409,   410,   411,   412,   212,   213,     0,
       0, -1013,     0,     0,     0,     0,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   108,   423,     0,   110,     0,
       0,   424,   425, -1014,   426,   427,   428,   113,   114,   115,
     429,   116,     0,     0,   117,   118,   119,   120,   121,     0,
       0,     0,   122,   123,     0,     0,     0,  1157,   124,     0,
    1128,     0,   430,   431,   394,     0,     0,     0,   501,     0,
     433,     0,     0,   395,   289,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,   397,   398,     0, -1013, -1013, -1013, -1013, -1013,
   -1013, -1013,     0,     0,     0,     0,     0, -1013,     0, -1013,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,   400,     0,     0,     0, -1014, -1014, -1014,
   -1014, -1014, -1014, -1014,     0,     0,     0,     0,     0, -1014,
       0, -1014,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,   107,     0,  1160,     0,     0,  1128,     0,
       0,     0,   394,   404,     0,     0,     0,   405,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   108,   109,   396,   110,     0,     0,     0,     0,
     397,   398,   111,   112,   113,   114,   115,     0,   116,     0,
       0,   117,   118,   119,   120,   121,     0,     0,     0,   122,
     123,     0,     0,     0,     0,   124,     0,     0,   399,     0,
       0,   400,  1129,     0,   406,   995,     0,     0,     0,     0,
     407,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     408,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,     0,     0,     0,   409,   410,   411,   412,   212,   213,
       0,   404,   571,     0,     0,   405,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   108,   423,     0,   110,
       0,     0,   424,   425,     0,   426,   427,   428,   113,   114,
     115,   429,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,     0,   122,   123,     0,     0,     0,     0,   124,
    1129,     0,   406,   430,   431,     0,     0,     0,   407,   501,
       0,   433,     0,     0,     0,   289,     0,     0,   408,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   108,   423,     0,   110,     0,     0,
     424,   425,     0,   426,   427,   428,   113,   114,   115,   429,
     116,     0,     0,   117,   118,   119,   120,   121,     0,     0,
       0,   122,   123,     0,     0,     0,  1163,   124,     0,  1128,
       0,   430,   431,   394,     0,     0,     0,   501,     0,   433,
       0,     0,   395,   289,     0,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   396,   892,     0,     0,     0,
       0,   397,   398,     0,   704,     0,     0,     0,     0,   705,
       0,     0,   893,     0,   108,   109,     0,   110,     0,     0,
       0,     0,     0,     0,   111,   112,   113,   114,   115,   399,
     116,     0,   400,   117,   118,   119,   120,   121,     0,     0,
       0,   122,   123,     0,     0,     0,     0,   124,   894,     0,
       0,     0,     0,     0,     0,     0,     0,   895,     0,     0,
     402,   403,     0,     0,     0,   896,     0,     0,     0,     0,
     394,     0,   404,     0,     0,     0,   405,     0,     0,   395,
       0,     0,     0,     0,   897,     0,   898,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,   397,   398,
     899,     0,   900,   901,   902,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   653,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,   400,
       0,  1129,     0,   406,     0,     0,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   408,
       0,     0,     0,   903,     0,     0,     0,   402,   403,     0,
       0,     0,     0,   409,   410,   411,   412,   212,   213,   404,
       0,     0,     0,   405,     0,     0,     0,   413,     0,     0,
       0,   882,     0,     0,     0,     0,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   108,   423,     0,   110,     0,
       0,   424,   425,     0,   426,   427,   428,   113,   114,   115,
     429,   116,     0,     0,   117,   118,   119,   120,   121,     0,
     696,     0,   122,   123,   655,     0,     0,     0,   124,     0,
     406,     0,   430,   431,     0,     0,   407,     0,   501,     0,
     433,     0,     0,     0,   289,     0,   408,   904,   905,   906,
     706,   707,   708,   709,     0,     0,     0,     0,     0,     0,
     409,   410,   411,   412,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,   413,     0,     0,     0,     0,     0,
       0,     0,     0,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   108,   423,     0,   110,     0,     0,   424,   425,
       0,   426,   427,   428,   113,   114,   115,   429,   116,     0,
       0,   117,   118,   119,   120,   121,   883,   884,   885,   122,
     123,     0,   394,     0,     0,   124,     0,     0,     0,   430,
     431,   395,     0,     0,     0,   659,     0,   433,     0,     0,
       0,   289,     0,     0,   396,     0,     1,     0,     0,     0,
     397,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     0,   653,
       0,     0,     0,     0,     0,     3,     0,     0,   399,     0,
       0,   400,     4,     0,     5,     0,     6,     0,     0,     0,
       0,     7,     0,     0,     0,     0,     0,     0,     8,     0,
       0,     0,     0,   477,     0,     0,     0,     0,   394,   402,
     403,     0,     9,     0,     0,     0,     0,   395,     0,     0,
       0,   404,     0,    10,     0,   405,     0,     0,     0,   107,
     396,     0,     0,   878,     0,     0,   397,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   653,     0,     0,   108,   109,
       0,   110,     0,     0,   399,     0,     0,   400,   111,   112,
     113,   114,   115,     0,   116,     0,   655,   117,   118,   119,
     120,   121,   406,     0,     0,   122,   123,     0,   407,     0,
      11,   124,     0,     0,     0,   402,   403,    12,   408,     0,
      13,     0,    14,    15,     0,     0,     0,   404,     0,     0,
       0,   405,   409,   410,   411,   412,   212,   213,     0,   654,
       0,     0,     0,     0,     0,     0,   413,    16,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   108,   423,   967,   110,     0,     0,
     424,   425,     0,   426,   427,   428,   113,   114,   115,   429,
     116,     0,   655,   117,   118,   656,   657,   658,   406,     0,
       0,   122,   123,     0,   407,     0,    17,   124,     0,     0,
       0,   430,   431,     0,   408,     0,     0,   659,     0,   433,
       0,    18,    19,   289,     0,     0,     0,     0,   409,   410,
     411,   412,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,   413,     0,     0,     0,     0,     0,     0,     0,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     108,   423,     0,   110,     0,     0,   424,   425,     0,   426,
     427,   428,   113,   114,   115,   429,   116,     0,     0,   117,
     118,   656,   657,   658,     0,   394,     0,   122,   123,     0,
       0,   970,     0,   124,   395,     0,     0,   430,   431,     0,
       0,     0,     0,   659,     0,   433,     0,   396,   107,   289,
       0,     0,     0,   397,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   653,     0,     0,     0,     0,   108,   109,     0,
     110,   399,     0,     0,   400,   482,     0,   111,   112,   113,
     114,   115,     0,   116,     0,     0,   117,   118,   119,   120,
     121,     0,   107,     0,   122,   123,     0,     0,     0,     0,
     124,   394,   402,   403,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,   404,     0,     0,     0,   405,     0,
       0,   108,   109,   396,   110,     0,   878,     0,     0,   397,
     398,   111,   112,   113,   114,   115,     0,   116,     0,     0,
     117,   118,   119,   120,   121,     0,     0,     0,   122,   123,
       0,     0,     0,     0,   124,     0,     0,   399,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,   655,
       0,     0,     0,     0,     0,   406,     0,     0,     0,     0,
       0,   407,     0,     0,     0,     0,   401,     0,   402,   403,
       0,   408,     0,     0,     0,     0,     0,     0,     0,     0,
     404,     0,     0,     0,   405,   409,   410,   411,   412,   212,
     213,     0,     0,     0,     0,     0,     0,     0,     0,   413,
       0,     0,     0,     0,     0,     0,     0,     0,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   108,   423,     0,
     110,     0,     0,   424,   425,     0,   426,   427,   428,   113,
     114,   115,   429,   116,     0,     0,   117,   118,   119,   120,
     121,   406,     0,     0,   122,   123,     0,   407,     0,     0,
     124,     0,     0,     0,   430,   431,     0,   408,     0,     0,
     659,     0,   433,     0,     0,     0,   289,     0,     0,     0,
       0,   409,   410,   411,   412,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,   413,     0,     0,     0,     0,
       0,     0,     0,     0,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   108,   423,     0,   110,     0,     0,   424,
     425,     0,   426,   427,   428,   113,   114,   115,   429,   116,
       0,     0,   117,   118,   119,   120,   121,     0,   394,   107,
     122,   123,     0,     0,     0,     0,   124,   395,     0,     0,
     430,   431,   506,     0,     0,     0,   501,     0,   433,     0,
     396,     0,   289,     0,     0,     0,   397,   398,   108,   109,
       0,   110,     0,     0,     0,     0,     0,     0,   111,   112,
     113,   114,   115,     0,   116,     0,     0,   117,   118,   119,
     120,   121,     0,     0,   399,   122,   123,   400,     0,     0,
       0,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1831,     0,   433,     0,   107,     0,     0,     0,     0,
       0,     0,     0,   401,   394,   402,   403,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,   404,     0,     0,
       0,   405,     0,     0,   108,   109,   396,   110,     0,     0,
       0,     0,   397,   398,   111,   112,   113,   114,   115,     0,
     116,     0,     0,   117,   118,   119,   120,   121,     0,     0,
       0,   122,   123,     0,     0,     0,     0,   124,     0,     0,
     399,     0,     0,   400,     0,     0,     0,     0,   696,   433,
       0,     0,     0,     0,     0,     0,     0,     0,   406,     0,
       0,     0,     0,     0,   407,     0,     0,     0,     0,   401,
       0,   402,   403,     0,   408,     0,     0,     0,     0,     0,
       0,     0,     0,   404,     0,     0,     0,   405,   409,   410,
     411,   412,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,   413,     0,     0,     0,     0,     0,     0,     0,
       0,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     108,   423,     0,   110,     0,     0,   424,   425,     0,   426,
     427,   428,   113,   114,   115,   429,   116,     0,     0,   117,
     118,   119,   120,   121,   406,     0,     0,   122,   123,     0,
     407,     0,     0,   124,     0,     0,     0,   430,   431,     0,
     408,     0,     0,   501,     0,   433,     0,     0,     0,   289,
       0,     0,     0,     0,   409,   410,   411,   412,   212,   213,
     397,     0,     0,     0,     0,     0,     0,     0,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   108,   423,   399,   110,
       0,     0,   424,   425,     0,   426,   427,   428,   113,   114,
     115,   429,   116,     0,     0,   117,   118,   119,   120,   121,
       0,   394,   107,   122,   123,     0,     0,   401,     0,   124,
     395,     0,     0,   430,   431,     0,     0,     0,     0,   432,
       0,   433,     0,   396,     0,   289,     0,     0,     0,   397,
     398,   108,   109,     0,   110,     0,     0,     0,     0,     0,
       0,   111,   112,   113,   114,   115,     0,   116,     0,     0,
     117,   118,   119,   120,   121,     0,     0,   399,   122,   123,
     400,     0,     0,     0,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   995,     0,     0,     0,     0,     0,
       0,     0,   406,     0,     0,     0,     0,     0,   402,   403,
       0,     0,     0,     0,     0,     0,     0,     0,   408,   394,
     404,     0,     0,     0,   405,     0,     0,     0,   395,     0,
       0,     0,   409,   410,   411,     0,   212,   213,     0,     0,
       0,   396,     0,     0,     0,     0,     0,   397,   398,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,     0,     0,     0,     0,     0,     0,
     424,   425,     0,     0,     0,   399,     0,     0,   400,   429,
       0,   406,     0,     0,     0,     0,     0,   407,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   408,     0,     0,
       0,     0,  1256,     0,     0,   688,   402,   403,     0,     0,
       0,   409,   410,   411,   412,   212,   213,     0,   404,     0,
       0,     0,   405,     0,     0,   413,     0,     0,     0,     0,
       0,     0,     0,     0,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   108,   423,     0,   110,     0,     0,   424,
     425,     0,   426,   427,   428,   113,   114,   115,   429,   116,
       0,     0,   117,   118,   119,   120,   121,     0,     0,   696,
     122,   123,     0,     0,     0,     0,   124,     0,     0,   406,
     430,   431,     0,     0,     0,   407,   501,     0,   433,     0,
       0,     0,   289,     0,     0,   408,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   409,
     410,   411,   412,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,   413,     0,     0,     0,     0,     0,     0,
       0,     0,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   108,   423,     0,   110,     0,     0,   424,   425,     0,
     426,   427,   428,   113,   114,   115,   429,   116,     0,     0,
     117,   118,   119,   120,   121,     0,   394,   107,   122,   123,
       0,     0,     0,     0,   124,   395,     0,     0,   430,   431,
       0,     0,     0,     0,   501,     0,   433,     0,   396,     0,
     289,     0,     0,     0,   397,   398,   108,   109,     0,   110,
       0,     0,     0,     0,     0,     0,   111,   112,   113,   114,
     115,     0,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,   399,   122,   123,   400,     0,     0,     0,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1738,
       0,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,   401,   394,   402,   403,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,   404,     0,     0,     0,   405,
       0,     0,   108,   109,   396,   110,     0,     0,     0,     0,
     397,   398,   111,   112,   113,   114,   115,     0,   116,     0,
       0,   117,   118,   119,   120,   121,     0,     0,     0,   122,
     123,     0,     0,     0,     0,   124,     0,     0,   399,     0,
       0,   400,     0,  1574,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,     0,     0,     0,
       0,     0,   407,     0,     0,     0,     0,     0,     0,   402,
     403,     0,   408,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,     0,     0,   405,   409,   410,   411,   412,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
     413,     0,     0,     0,     0,     0,     0,     0,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   108,   423,
       0,   110,     0,     0,   424,   425,     0,   426,   427,   428,
     113,   114,   115,   429,   116,     0,     0,   117,   118,   119,
     120,   121,   406,     0,     0,   122,   123,     0,   407,     0,
       0,   124,     0,     0,     0,   430,   431,     0,   408,     0,
       0,   501,     0,   433,     0,     0,     0,   289,     0,   967,
       0,     0,   409,   410,   411,   412,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,   413,     0,     0,     0,
       0,     0,     0,     0,     0,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   108,   423,     0,   110,     0,     0,
     424,   425,     0,   426,   427,   428,   113,   114,   115,   429,
     116,     0,     0,   117,   118,   119,   120,   121,     0,   394,
       0,   122,   123,     0,     0,     0,     0,   124,   395,     0,
       0,   430,   431,   968,     0,     0,     0,   501,   953,   433,
       0,   396,     0,   289,     0,     0,     0,   397,   398,     0,
       0,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   969,
       0,     0,     0,     0,   970,   399,     0,     0,   400,     0,
     971,     0,   108,   109,     0,   110,     0,     0,     0,     0,
       0,   107,   111,   112,   113,   114,   115,     0,   116,   972,
       0,   117,   118,   119,   120,   121,   402,   403,     0,   122,
     123,     0,     0,     0,     0,   124,   394,     0,   404,     0,
     108,   109,   405,   110,     0,   395,     0,     0,     0,     0,
     111,   112,   113,   114,   115,     0,   116,     0,   396,   117,
     118,   119,   120,   121,   397,   398,     0,   122,   123,     0,
       0,     0,     0,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,   400,     0,     0,     0,   406,
       0,     0,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   408,     0,     0,     0,     0,
       0,     0,     0,   402,   403,     0,  1155,     0,     0,   409,
     410,   411,   412,   212,   213,   404,     0,     0,     0,   405,
       0,     0,     0,   413,     0,     0,     0,     0,     0,     0,
       0,     0,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   108,   423,     0,   110,     0,     0,   424,   425,     0,
     426,   427,   428,   113,   114,   115,   429,   116,     0,     0,
     117,   118,   119,   120,   121,     0,  1342,     0,   122,   123,
       0,     0,     0,     0,   124,     0,   406,     0,   430,   431,
       0,     0,   407,     0,   501,     0,   433,     0,     0,     0,
     289,     0,   408,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   409,   410,   411,   412,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
     413,     0,     0,     0,     0,     0,     0,     0,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   108,   423,
       0,   110,     0,     0,   424,   425,     0,   426,   427,   428,
     113,   114,   115,   429,   116,     0,     0,   117,   118,   119,
     120,   121,     0,   394,   107,   122,   123,     0,     0,     0,
       0,   124,   395,     0,     0,   430,   431,     0,     0,     0,
       0,   501,     0,   433,     0,   396,     0,   289,     0,     0,
       0,   397,   398,   108,   109,     0,   110,     0,     0,     0,
       0,     0,     0,   111,   112,   113,   114,   115,     0,   116,
       0,     0,   117,   118,   119,   120,   121,     0,     0,   399,
     122,   123,   400,     0,     0,     0,   124,     0,     0,     0,
       0,     0,     0,     0,  1583,     0,     0,     0,    77,     0,
     107,     0,     0,     0,     0,     0,     0,     0,     0,   394,
     402,   403,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,   404,     0,     0,     0,   405,     0,     0,   108,
     109,   396,   110,     0,     0,     0,     0,   397,   398,   111,
     112,   113,   114,   115,     0,   116,     0,     0,   117,   118,
     119,   120,   121,     0,     0,     0,   122,   123,     0,     0,
       0,     0,   124,     0,     0,   399,     0,    78,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,     0,     0,     0,     0,     0,   407,
       0,     0,     0,     0,     0,     0,   402,   403,     0,   408,
       0,     0,     0,     0,     0,     0,     0,     0,   404,     0,
    1355,     0,   405,   409,   410,   411,   412,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   108,   423,     0,   110,     0,
       0,   424,   425,     0,   426,   427,   428,   113,   114,   115,
     429,   116,     0,     0,   117,   118,   119,   120,   121,   406,
       0,     0,   122,   123,     0,   407,     0,     0,   124,     0,
       0,     0,   430,   431,     0,   408,     0,     0,   501,     0,
     433,     0,     0,     0,   289,     0,     0,     0,     0,   409,
     410,   411,   412,   212,   213,     0,     0,     0,     0,     0,
       0,  1718,     0,   413,     0,     0,     0,     0,     0,     0,
       0,     0,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   108,   423,     0,   110,     0,  1719,   424,   425,     0,
     426,   427,   428,   113,   114,   115,   429,   116,     0,     0,
     117,   118,   119,   120,   121,     0,   394,   107,   122,   123,
       0,     0,     0,     0,   124,   395,     0,     0,   430,   431,
       0,     0,  1690,     0,   501,     0,   433,     0,   396,     0,
     289,     0,     0,     0,   397,   398,   108,   109,     0,   110,
       0,     0,     0,     0,  1720,     0,   111,   112,   113,   114,
     115,     0,   116,     0,     0,   117,   118,   119,   120,   121,
       0,     0,   399,   122,   123,   400,     0,     0,     0,   124,
       0,     0,     0,     0,     0,   956,     0,     0,   107,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   100,   108,   109,   405,
     110,     0,     0,     0,     0,     0,     0,   111,   112,   113,
     114,   115,     0,   116,     0,     0,   117,   118,   119,   120,
     121,     0,     0,     0,   122,   123,     0,     0,     0,     0,
     124,     0,     0,     0,     0,     0,     0,   101,  1201,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,     0,     0,     0,     0,   406,     0,     0,     0,
       0,     0,   407,   137,     0,     0,     0,     0,     6,     0,
       0,     0,   408,     0,   103,  -281,     0,     0,     0,  1400,
    1401,     0,  1402,     0,     0,     0,   409,   410,   411,   412,
     212,   213,     0,     0,     0,     0,     0,     0,     0,  1403,
     413,     0,     0,     0,     0,  1404,     0,     0,     0,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   108,   423,
       0,   110,     0,     0,   424,   425,     0,   426,   427,   428,
     113,   114,   115,   429,   116,     0,     0,   117,   118,   119,
     120,   121,     0,   104,  1201,   122,   123,     0,     0,     0,
       0,   124,     0,   105,     0,   430,   431,     0,     0,     0,
       0,   501,     0,   433,     0,  1405,     0,   289,     0,     0,
       0,     0,     0,     0,     6,     0,     0,     0,   106,     0,
       0,  -283,     0,     0,    14,  1400,  1401,     0,  1402,     0,
       0,     0,     0,     0,     0,     0,   107,     0,     0,  1406,
       0,     0,     0,     0,     0,  1403,     0,     0,     0,    16,
       0,  1404,     0,     0,     0,     0,     0,     0,  1590,     0,
       0,     0,     0,     0,     0,   108,   109,   107,   110,     0,
       0,     0,     0,     0,     0,   111,   112,   113,   114,   115,
       0,   116,     0,     0,   117,   118,   119,   120,   121,     0,
    1201,     0,   122,   123,     0,     0,  1407,   109,   124,   110,
       0,     0,     0,     0,     0,     0,   111,   112,   113,   114,
     115,  1405,   116,     0,     0,   117,  1408,   119,   120,   121,
       6,     0,     0,   122,   123,  1409,  1410,  1411,     0,   124,
      14,  1400,  1401,     0,  1402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1406,     0,     0,     0,     0,
       0,  1403,     0,     0,     0,    16,     0,  1404,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,   596,     0,     0,     0,   597,     0,     0,     0,
       0,     0,     0,   795,     0,     0,     0,     0,     0,     0,
       0,     0,  1407,   109,   598,   110,     0,   599,     0,     0,
       0,     0,   111,   112,   113,   114,   115,  1405,   116,     0,
       0,   117,  1408,   119,   120,   121,     0,     0,     0,   122,
     123,  1409,  1410,  1411,     0,   124,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   600,     0,
       0,  1406,     0,     0,   601,   602,   603,     0,   604,   605,
     606,    16,   607,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,   608,     0,   609,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1407,   109,
       0,   110,     0,     0,     0,   610,     0,     0,   111,   112,
     113,   114,   115,     0,   116,     0,     0,   117,  1408,   119,
     120,   121,   611,     0,     0,   122,   123,  1409,  1410,  1411,
       0,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   612,     0,   596,     0,     0,     0,
     597,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,     0,     0,     0,     0,     0,     0,   598,     0,
       0,   599,     0,     0,     0,     0,     0,     0,     0,   613,
     614,     0,     0,     0,     0,     0,     0,     0,   108,   109,
       0,   110,     0,     0,     0,     0,   615,     0,   111,   112,
     113,   114,   115,     0,   116,     0,     0,   117,   118,   119,
     120,   121,   600,     0,     0,   122,   123,     0,   601,   602,
     603,   124,   604,   605,   606,     0,   607,   868,     0,     0,
       0,   597,     0,     0,     0,     0,   596,     0,     0,     0,
     597,     0,     0,     0,     0,   869,   608,     0,   609,   598,
       0,     0,   599,     0,     0,     0,     0,     0,   598,     0,
       0,   599,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   610,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   611,     0,     0,   601,
     602,   603,   600,   604,   605,   606,     0,   607,   601,   602,
     603,     0,   604,   605,   606,     0,   607,     0,   612,     0,
       0,     0,     0,     0,     0,     0,     0,   608,     0,   609,
       0,     0,     0,   107,     0,     0,   608,     0,   609,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   613,   614,     0,     0,     0,     0,     0,
     610,     0,   108,   109,     0,   110,     0,     0,     0,   610,
     615,     0,   111,   112,   113,   114,   115,   611,   116,     0,
       0,   117,   118,   119,   120,   121,   611,     0,     0,   122,
     123,     0,     0,     0,     0,   124,     0,     0,     0,   612,
       0,     0,     0,     0,     0,     0,     0,     0,   612,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   613,   614,     0,     0,     0,     0,
       0,     0,     0,   613,   614,     0,     0,     0,     0,     0,
       0,   615,   107,     0,     0,     0,     0,     0,     0,     0,
     615,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,     0,     0,   184,
     185,   108,   109,     0,   110,     0,     0,   186,   187,     0,
       0,   111,   112,   113,   114,   115,   188,   116,   189,     0,
     117,   118,   119,   120,   121,     0,     0,     0,   122,   123,
     190,   191,   192,   193,   124
};

static const short yycheck[] =
{
       9,    16,    56,    12,     0,     4,    65,    16,    64,    18,
      57,   106,   526,   143,   144,    92,   711,   297,   153,   341,
     372,    68,   688,   372,   468,   368,    64,    86,    95,   343,
      71,   567,    74,    63,   143,   144,    90,   877,   432,   538,
     512,   881,  1200,  1273,    91,    54,    55,    56,    57,    58,
      59,    98,   489,    94,    96,    64,    65,    66,   963,    68,
     364,    70,    71,    89,    73,    74,   696,   939,   389,    84,
     718,   995,   350,    82,   769,    84,    85,    86,    93,    88,
      89,    90,    91,    69,    93,    94,    95,    96,   862,    98,
     157,   275,  1036,    58,  1036,   648,   134,   641,   136,   377,
    1415,   285,  1046,   733,  1046,    70,    92,  1097,   155,  1036,
     349,   710,  1043,   985,  1074,   559,   228,   561,   291,  1046,
    1036,  1218,   148,   567,   318,   134,   135,   136,    10,   737,
    1046,   804,   161,     8,   737,   495,  1391,     8,     8,   148,
    1046,     8,   237,    23,   153,     8,   155,   156,   157,  1182,
       8,    64,   161,    10,    50,  1188,  1036,    50,     6,     8,
      94,  1092,  1036,    29,  1321,    64,  1046,   197,    47,   654,
     135,  1210,  1046,  1212,    29,     8,   148,   772,    94,    66,
    1239,   148,    46,   318,    66,    46,    84,    47,     8,  1040,
    1222,   156,   222,    26,   789,    24,  1047,   792,    31,  1454,
    1685,    49,   545,   654,    66,  1056,   801,    47,   659,    46,
     322,    26,     3,   165,    20,   567,    31,    26,   567,    46,
     281,  1271,    31,  1521,   890,   891,  1098,   265,  1713,  1080,
     268,   326,   746,    94,    12,     6,     4,  1475,   198,     7,
      47,    66,     6,   148,    84,   201,   163,   569,   302,   592,
       9,  1409,  1410,   531,     6,    98,   265,   132,  1743,   268,
     855,   132,   132,   272,   130,   132,   275,   276,   934,   132,
      19,    98,   281,   190,   132,   596,   285,    84,    49,   184,
     318,   124,  1520,   132,    52,    49,  1318,   254,     6,    80,
     188,   207,   208,   302,   616,   124,   268,   306,   482,    58,
     199,   252,    53,    54,   342,    53,    54,   272,  1232,   188,
      69,   349,   132,   351,  1245,   353,   281,   277,   356,   185,
     182,  1265,  1266,  1265,  1266,  1597,    12,   374,   188,   263,
     185,   385,   181,   342,   375,   365,   202,   367,  1265,  1266,
     349,   306,   351,  1828,   353,  1305,     8,   356,   188,  1265,
    1266,   381,  1385,  1386,   489,  1299,   140,  1299,   101,  1265,
    1266,   181,   140,  1268,   277,   374,   375,   839,  1525,  1035,
     978,     8,  1299,   817,   383,   978,   385,   329,  1051,  1052,
     740,   277,   990,  1299,   277,  1265,  1266,   990,   991,    26,
     277,  1265,  1266,  1299,    31,   277,   676,   882,    26,  1458,
    1630,   281,   263,    31,   279,   269,   212,  1312,   279,   279,
      26,   163,   279,   693,   173,    31,   279,  1456,   383,  1299,
     277,   279,   702,  1018,   433,  1299,   274,   878,    26,  1102,
    1588,   882,   269,    31,   481,   496,  1486,  1288,  1736,   272,
     273,   274,   275,  1038,  1039,   163,   197,  1762,   495,   197,
     204,   489,  1118,  1119,   140,  1121,  1122,   272,   273,   274,
     275,   783,   277,  1058,   786,   274,   275,   747,   477,    59,
     132,   500,   481,   482,    47,   584,     8,   799,    89,   488,
     157,  1753,  1797,   492,    11,  1757,   495,   496,   768,     3,
      14,   500,  1747,  1765,   278,   238,  1768,   149,  1728,    26,
      71,    65,   824,   114,    31,    67,   151,  1604,   585,   182,
       6,    84,   477,    26,   166,     5,    18,   547,    31,    81,
     632,    48,  1794,   488,  1839,   170,    16,   492,  1518,   567,
     284,   496,    34,  1132,  1567,     0,   200,  1311,   123,   193,
    1464,    23,     6,   105,   728,  1860,   983,  1091,  1863,  1197,
    1780,   790,   866,    49,   875,  1108,   565,   587,   567,  1831,
     276,   172,   571,  1878,  1836,  1470,    80,   129,  1499,    26,
     743,   765,  1532,   163,    31,   853,   177,    79,    40,   164,
     153,   204,   146,    47,  1814,   715,  1517,   111,  1254,    51,
     163,   177,   986,    26,   200,    59,    58,   159,    31,   585,
     132,   278,   640,   204,   277,   176,   108,   637,   638,    91,
    1276,    26,   183,   277,   177,   188,    31,  1025,   204,   123,
      84,    83,    26,   187,   278,  1228,   211,    31,  1479,    67,
     765,   640,  1483,  1484,    26,   272,   273,   274,   275,    31,
     649,   204,   751,    81,   272,   273,   274,   275,  1792,    26,
     649,   279,   932,   679,    31,   278,   272,   273,   274,   275,
     155,   284,  1806,   279,    16,  1537,   280,   105,  1519,   283,
     679,   277,   273,   175,   272,   273,   274,   275,   716,  1748,
     718,   279,   720,   284,   204,   204,   148,   273,  1219,   153,
     194,    26,   278,   740,  1102,   704,    31,  1771,     7,   163,
      26,  1770,   711,   102,  1862,    31,    19,   716,    26,   718,
     273,   720,  1342,    31,   723,  1310,   204,  1738,   180,   728,
    1789,   159,   184,  1045,   188,  1029,   151,   765,   737,    26,
    1874,   740,    84,   771,    31,  1057,   745,  1642,   204,    57,
     749,   818,  1686,  1648,  1686,   272,   273,   274,   275,   779,
     204,   760,   790,   762,   273,  1597,    26,  1079,   278,  1686,
     769,    31,   771,  1294,   284,  1616,  1787,  1841,   133,  1620,
    1686,   848,   849,    86,   851,  1274,   175,   202,   816,   817,
    1686,   790,    26,   967,  1635,   273,   970,    31,  1857,  1093,
       6,  1105,    11,  1740,   204,   760,   204,   762,  1192,   829,
       6,  1652,  1468,   204,  1825,   182,  1686,   816,   817,   278,
     239,   277,  1686,     4,    26,   272,   273,   274,   275,    31,
     249,   186,   279,   231,   147,  1297,   246,    62,    63,    48,
     231,  1303,   818,    49,   254,  1371,  1829,   823,  1785,   272,
     273,   274,   275,    49,  1238,   854,   279,  1840,   983,   129,
     836,    26,   278,    83,  1168,   864,    31,   272,   273,   274,
     275,    52,   848,   849,   279,   851,     4,   876,   272,   273,
     274,   275,   273,    33,    67,   279,  1210,  1824,  1212,    17,
     272,   273,   274,   275,  1735,   148,    46,   279,    81,   854,
     120,    68,    24,  1744,  1887,   272,   273,   274,   275,   864,
      26,    33,  1182,   133,   971,    31,   239,   240,  1188,  1856,
    1750,  1753,   105,  1579,    52,  1757,    54,   180,   277,   277,
     279,   968,    26,  1765,   282,    68,  1768,    31,   969,   277,
     972,   277,   195,  1784,   282,   292,   129,   272,   273,   274,
     275,   129,  1222,   300,   279,   983,   272,   273,   274,   275,
    1394,   131,  1794,   279,   272,   273,   274,   275,   967,   968,
     969,   970,   971,   972,  1286,   277,   159,  1858,  1077,   978,
     282,   124,  1315,   164,  1865,   272,   273,   274,   275,   276,
     129,   990,   991,    26,   207,   208,   995,  1025,    31,  1831,
     129,   182,   124,   146,  1836,  1317,   137,   138,    67,   995,
     153,  1892,   272,   273,   274,   275,   277,   102,   279,   279,
     182,  1088,    81,   277,   146,   279,  1025,   276,   277,  1371,
      89,   153,  1371,    13,   211,   216,   213,  1104,   272,   273,
     274,   275,    22,  1110,   187,   279,   105,   277,  1318,   279,
     277,  1707,   279,    86,   139,    35,   141,    83,    38,   164,
    1716,    41,    42,  1890,  1891,   187,  1570,  1095,   240,  1068,
     272,   273,   274,   275,  1102,    20,  1201,   279,    26,   164,
      60,   267,  1071,    31,  1769,  1084,   897,   278,   899,    69,
     901,   902,    72,   904,   905,   906,  1095,  1217,  1218,  1219,
     159,  1605,   170,  1102,   189,   207,    26,   272,   273,   274,
     275,    31,  1088,    10,   279,  1385,  1386,   277,    66,   279,
     100,   101,   277,    62,   279,    26,    88,  1753,  1104,    64,
      31,  1757,   112,    26,  1110,   206,   116,    26,    31,  1765,
     276,   277,    31,    26,   124,   277,    83,   279,    31,    82,
    1654,  1250,   198,  1252,  1200,   157,   272,   273,   274,   275,
     277,   123,   279,   279,    57,    66,   148,    86,  1794,  1197,
     278,    66,  1200,  1201,    64,    64,  1488,   148,   272,   273,
     274,   275,   277,   129,   279,   279,    26,   167,   675,   151,
     677,    31,   154,   173,   681,   682,   683,   129,  1197,   179,
     252,  1200,  1201,    86,   277,  1831,   279,   182,   170,   189,
    1836,  1210,   196,  1212,  1313,  1527,   277,  1873,   279,   277,
     277,   279,   279,   203,   204,   205,   206,   207,   208,  1228,
     277,   107,   279,  1232,   196,   163,   277,   217,   279,   272,
     273,   274,   275,   276,   180,   130,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   277,   238,   279,
      64,   241,   242,   262,   244,   245,   246,   247,   248,   249,
     250,   251,   263,   262,   254,   255,   256,   257,   258,    33,
     277,   264,   262,   263,   277,   215,   279,  1791,   268,   118,
    1415,   269,   272,   273,   203,   204,   205,  1567,   278,    32,
     280,   278,    10,  1292,   284,   288,   278,   207,  1328,   278,
     278,   278,  1332,   278,   297,   278,   207,   278,   278,   278,
     278,   207,   231,   232,   272,   273,   274,   275,   278,   278,
     277,     6,   283,    10,     9,   278,   182,  1649,    13,   281,
     278,  1466,   283,   269,   278,   328,    66,    22,   331,   148,
     277,  1400,   272,   273,   274,   275,    66,   174,   269,   279,
      35,   277,    18,   277,    10,   283,    41,    42,   278,   277,
      26,   272,   273,   274,   275,    31,   188,   200,    34,   272,
     273,   274,   275,   272,   273,   274,   275,  1415,    94,   272,
     273,   274,   275,    94,    69,    94,    24,    72,    24,    46,
      10,  1400,   182,   216,   143,   182,   143,   204,    23,  1408,
    1409,  1410,    94,   145,    70,   193,  1415,   193,   278,    95,
     278,   278,   281,    79,   278,   100,   101,    64,    10,   129,
     157,    87,   272,   273,   274,   275,   277,   112,   135,   128,
     278,   116,   278,   426,   197,   279,    23,   430,   431,   432,
     106,   284,   108,    45,   182,   210,    63,  1456,   278,   194,
      66,    66,   198,    10,   248,  1464,   122,  1466,   124,   125,
     126,   128,  1597,   218,   219,   220,   221,   222,   223,   224,
     225,  1606,   251,   278,  1604,    20,   278,    46,    30,    90,
    1489,    90,    24,   132,   132,   164,   171,   132,   173,   162,
      46,    20,   164,   278,   179,   102,   278,   278,   163,   163,
      10,   279,   277,    66,   189,   277,    26,   277,   501,   175,
     192,    17,   505,   131,   113,   277,    66,   277,   203,   204,
     205,   206,   207,   208,  1489,   278,    74,    12,   278,  1576,
     247,    10,   217,   168,   278,    10,   186,   200,  1633,   279,
     279,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   280,   238,   279,   279,   241,   242,   140,   244,
     245,   246,   247,   248,   249,   250,   251,  1576,    55,   254,
     255,   256,   257,   258,    55,   279,   238,   262,   263,  1588,
    1634,   279,     8,   268,   278,   278,    98,   272,   273,   278,
     278,    54,   181,   278,   278,   280,    73,  1606,  1594,   284,
     278,   278,   278,   269,   270,   271,   272,   273,   274,   275,
     278,   134,   134,   279,    20,  1624,   134,   278,  1753,   278,
    1629,   279,  1757,   278,   278,  1634,   278,  1762,  1658,    89,
    1765,   277,  1662,  1768,  1729,   278,   278,    77,  1653,   279,
    1699,   137,   153,   277,  1653,   204,   132,   279,   132,   132,
     132,   279,   132,    24,   132,   278,   163,   279,   279,  1794,
     279,    54,  1797,   276,  1629,   279,   659,   181,   276,    39,
      39,    86,   278,    51,    54,   276,   276,    86,   276,   276,
      58,   276,   276,   676,    62,    63,   276,   680,   131,   280,
    1699,  1700,   182,    71,   687,    10,  1831,   142,    26,   692,
     693,  1836,  1701,    84,  1839,    83,    83,    47,    84,   702,
     703,    47,   705,   706,   707,   708,   709,   710,   188,   188,
      66,   279,   117,  1738,  1762,  1860,   279,   131,  1863,  1738,
      98,   117,   279,   279,    64,   279,   279,    98,   279,   279,
     279,   279,   279,  1878,   279,   279,   279,   279,   279,   279,
     276,   279,   279,  1762,   747,   197,    90,   278,   277,  1797,
    1769,  1776,  1771,   279,    86,   278,   254,  1776,   276,    86,
     148,   114,  1787,   278,   283,   768,   278,   129,  1787,   277,
     773,   156,   775,     6,   181,   129,     9,   165,  1797,    89,
      13,   129,    50,  1847,    47,    39,   188,    53,  1813,    22,
     133,  1839,   180,   279,  1813,   279,   184,   279,   276,    50,
    1825,   277,    35,   277,   279,   209,  1825,   195,    41,    42,
     278,    83,  1860,   279,   277,  1863,   278,   273,   279,  1844,
    1839,  1215,  1841,    12,   728,  1844,   214,   276,  1847,  1108,
    1878,   285,   867,   644,  1105,  1094,    69,  1508,   339,    72,
    1095,  1860,    10,  1862,  1863,   771,   592,   324,  1032,  1868,
    1102,   800,   351,  1507,  1571,   369,  1486,  1488,  1778,  1878,
    1782,  1816,  1781,   790,   964,  1692,   716,   100,   101,  1592,
     351,  1890,  1891,  1020,  1738,  1763,  1247,  1471,   337,   112,
     344,    13,   573,   116,   356,  1301,   846,   629,   310,   892,
     893,   894,   514,   838,   897,   898,   899,   760,   901,   902,
     903,   904,   905,   906,   907,   908,   773,   910,   146,   912,
     995,    42,   915,   916,   917,   918,   919,   920,   747,  1717,
     518,  1614,  1813,  1844,     0,     0,     0,   736,   987,   932,
     526,  1146,   935,  1190,  1084,  1340,  1341,   571,   171,  1256,
     173,  1346,  1347,  1624,  1349,  1350,   179,    88,  1408,    -1,
      -1,    -1,  1357,  1358,  1359,  1360,   189,  1362,  1363,  1364,
    1365,  1366,  1367,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   986,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,   206,   272,
     273,    -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,
      -1,   284,    -1,    -1,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    22,    -1,    -1,   206,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    35,   254,   255,   256,   257,
     258,    41,    42,    -1,   262,   263,    -1,    -1,    -1,    -1,
     268,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    69,
     251,    -1,    72,   254,   255,   256,   257,   258,    -1,  1132,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,
      -1,  1144,  1145,   274,  1147,  1148,    -1,    -1,    -1,    -1,
     100,   101,  1155,    -1,    -1,    -1,    -1,    18,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    26,   116,    -1,    -1,  1172,
      31,    -1,    -1,    34,    -1,    -1,    -1,  1180,  1181,  1182,
      -1,    -1,    -1,    -1,    -1,  1188,    -1,  1190,  1191,  1192,
    1193,  1194,    -1,    -1,    -1,    -1,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,  1222,
      -1,   171,    -1,   173,    -1,    -1,    87,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,  1238,    -1,    47,    -1,   189,
      -1,    -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    59,
      -1,    30,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,   122,    -1,   124,   125,   126,  1269,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    33,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   175,  1318,    -1,    -1,   268,    -1,
      -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    -1,  1339,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,  1348,    -1,    -1,    -1,    -1,
      -1,    -1,  1355,   163,    -1,     6,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,  1385,  1386,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,  1405,    -1,    -1,    -1,    -1,    -1,   269,   270,
     271,   272,   273,   274,   275,    -1,    -1,    -1,    69,    -1,
     281,    72,   283,    -1,    -1,   235,   236,   206,   238,    -1,
      -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,   100,
     101,    -1,   262,   263,    -1,    -1,   235,   236,   268,   238,
      -1,   112,   206,    -1,  1467,   116,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,   268,
      -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,   278,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    40,    -1,   262,   263,
     171,    -1,   173,    -1,   268,    -1,    -1,    51,   179,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,   189,    -1,
      -1,    -1,  1545,  1546,    -1,    -1,    -1,    71,    -1,    -1,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,  1562,
      -1,    -1,    -1,    -1,  1567,    -1,   217,    -1,    -1,    -1,
      -1,    -1,  1575,    -1,  1577,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,   132,    -1,
      -1,   262,   263,    -1,    -1,    -1,     6,   268,    -1,     9,
      -1,   272,   273,    13,   148,    -1,    -1,   278,    -1,   280,
      -1,    -1,    22,   284,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    41,    42,    -1,  1657,    -1,   180,    -1,    -1,    -1,
     184,    -1,  1665,   187,    18,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    26,    -1,    -1,    -1,    -1,    31,    -1,    69,
      34,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,  1692,
     214,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    31,
     100,   101,    34,    -1,  1717,    -1,    70,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    79,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,   106,    -1,   108,    -1,    -1,    79,    -1,    -1,
      -1,  1764,    -1,    -1,    -1,    87,    -1,    -1,   122,    -1,
     124,   125,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   171,    -1,   173,   106,    -1,   108,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
     122,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,   175,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,     6,   268,    -1,
       9,    -1,   272,   273,    13,    -1,    -1,    -1,   278,    -1,
     280,    -1,    -1,    22,   284,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    41,    42,    -1,   269,   270,   271,   272,   273,
     274,   275,    -1,    -1,    -1,    -1,    -1,   281,    -1,   283,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,   269,   270,   271,
     272,   273,   274,   275,    -1,    -1,    -1,    -1,    -1,   281,
      -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,   206,    -1,     6,    -1,    -1,     9,    -1,
      -1,    -1,    13,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,
      41,    42,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,
      -1,    72,   171,    -1,   173,   278,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      -1,   112,    33,    -1,    -1,   116,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
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
      -1,    -1,    22,   284,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,    18,    -1,    -1,    -1,
      -1,    41,    42,    -1,    26,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,   235,   236,    -1,   238,    -1,    -1,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    69,
     251,    -1,    72,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,
     100,   101,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      13,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,    22,
      -1,    -1,    -1,    -1,   106,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
     122,    -1,   124,   125,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,   171,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,   175,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
     163,    -1,   262,   263,   167,    -1,    -1,    -1,   268,    -1,
     173,    -1,   272,   273,    -1,    -1,   179,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    -1,   189,   269,   270,   271,
     272,   273,   274,   275,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,    -1,    13,    -1,    -1,   268,    -1,    -1,    -1,   272,
     273,    22,    -1,    -1,    -1,   278,    -1,   280,    -1,    -1,
      -1,   284,    -1,    -1,    35,    -1,     7,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    69,    -1,
      -1,    72,    43,    -1,    45,    -1,    47,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,    13,   100,
     101,    -1,    73,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,   112,    -1,    84,    -1,   116,    -1,    -1,    -1,   206,
      35,    -1,    -1,   124,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,   246,
     247,   248,   249,    -1,   251,    -1,   167,   254,   255,   256,
     257,   258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,
     151,   268,    -1,    -1,    -1,   100,   101,   158,   189,    -1,
     161,    -1,   163,   164,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,   203,   204,   205,   206,   207,   208,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   217,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    74,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,   167,   254,   255,   256,   257,   258,   173,    -1,
      -1,   262,   263,    -1,   179,    -1,   237,   268,    -1,    -1,
      -1,   272,   273,    -1,   189,    -1,    -1,   278,    -1,   280,
      -1,   252,   253,   284,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    13,    -1,   262,   263,    -1,
      -1,   189,    -1,   268,    22,    -1,    -1,   272,   273,    -1,
      -1,    -1,    -1,   278,    -1,   280,    -1,    35,   206,   284,
      -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    69,    -1,    -1,    72,   189,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,   206,    -1,   262,   263,    -1,    -1,    -1,    -1,
     268,    13,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,   235,   236,    35,   238,    -1,   124,    -1,    -1,    41,
      42,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,
      -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   179,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    -1,   116,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,    -1,
     268,    -1,    -1,    -1,   272,   273,    -1,   189,    -1,    -1,
     278,    -1,   280,    -1,    -1,    -1,   284,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    13,   206,
     262,   263,    -1,    -1,    -1,    -1,   268,    22,    -1,    -1,
     272,   273,   274,    -1,    -1,    -1,   278,    -1,   280,    -1,
      35,    -1,   284,    -1,    -1,    -1,    41,    42,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    69,   262,   263,    72,    -1,    -1,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    13,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,   235,   236,    35,   238,    -1,    -1,
      -1,    -1,    41,    42,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    -1,   163,   280,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    -1,    -1,   116,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,   173,    -1,    -1,   262,   263,    -1,
     179,    -1,    -1,   268,    -1,    -1,    -1,   272,   273,    -1,
     189,    -1,    -1,   278,    -1,   280,    -1,    -1,    -1,   284,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    69,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    13,   206,   262,   263,    -1,    -1,    98,    -1,   268,
      22,    -1,    -1,   272,   273,    -1,    -1,    -1,    -1,   278,
      -1,   280,    -1,    35,    -1,   284,    -1,    -1,    -1,    41,
      42,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    69,   262,   263,
      72,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    13,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,    22,    -1,
      -1,    -1,   203,   204,   205,    -1,   207,   208,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    -1,    -1,    -1,    -1,    -1,    -1,
     241,   242,    -1,    -1,    -1,    69,    -1,    -1,    72,   250,
      -1,   173,    -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,   273,    -1,    -1,   197,   100,   101,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,   163,
     262,   263,    -1,    -1,    -1,    -1,   268,    -1,    -1,   173,
     272,   273,    -1,    -1,    -1,   179,   278,    -1,   280,    -1,
      -1,    -1,   284,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    13,   206,   262,   263,
      -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,   273,
      -1,    -1,    -1,    -1,   278,    -1,   280,    -1,    35,    -1,
     284,    -1,    -1,    -1,    41,    42,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    69,   262,   263,    72,    -1,    -1,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    13,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   235,   236,    35,   238,    -1,    -1,    -1,    -1,
      41,    42,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    69,    -1,
      -1,    72,    -1,   276,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    -1,   116,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,   173,    -1,    -1,   262,   263,    -1,   179,    -1,
      -1,   268,    -1,    -1,    -1,   272,   273,    -1,   189,    -1,
      -1,   278,    -1,   280,    -1,    -1,    -1,   284,    -1,    74,
      -1,    -1,   203,   204,   205,   206,   207,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    13,
      -1,   262,   263,    -1,    -1,    -1,    -1,   268,    22,    -1,
      -1,   272,   273,   148,    -1,    -1,    -1,   278,   279,   280,
      -1,    35,    -1,   284,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,    -1,    -1,    -1,   189,    69,    -1,    -1,    72,    -1,
     195,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,    -1,
      -1,   206,   245,   246,   247,   248,   249,    -1,   251,   214,
      -1,   254,   255,   256,   257,   258,   100,   101,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    13,    -1,   112,    -1,
     235,   236,   116,   238,    -1,    22,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    35,   254,
     255,   256,   257,   258,    41,    42,    -1,   262,   263,    -1,
      -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,   200,    -1,    -1,   203,
     204,   205,   206,   207,   208,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   163,    -1,   262,   263,
      -1,    -1,    -1,    -1,   268,    -1,   173,    -1,   272,   273,
      -1,    -1,   179,    -1,   278,    -1,   280,    -1,    -1,    -1,
     284,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    13,   206,   262,   263,    -1,    -1,    -1,
      -1,   268,    22,    -1,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,    35,    -1,   284,    -1,    -1,
      -1,    41,    42,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    69,
     262,   263,    72,    -1,    -1,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   276,    -1,    -1,    -1,   204,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,   235,
     236,    35,   238,    -1,    -1,    -1,    -1,    41,    42,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,    -1,   268,    -1,    -1,    69,    -1,   273,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
     200,    -1,   116,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,   173,
      -1,    -1,   262,   263,    -1,   179,    -1,    -1,   268,    -1,
      -1,    -1,   272,   273,    -1,   189,    -1,    -1,   278,    -1,
     280,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,   114,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    13,   206,   262,   263,
      -1,    -1,    -1,    -1,   268,    22,    -1,    -1,   272,   273,
      -1,    -1,   276,    -1,   278,    -1,   280,    -1,    35,    -1,
     284,    -1,    -1,    -1,    41,    42,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,   172,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    69,   262,   263,    72,    -1,    -1,    -1,   268,
      -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,     6,   235,   236,   116,
     238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    47,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    73,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,   189,    -1,    84,    54,    -1,    -1,    -1,    58,
      59,    -1,    61,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
     217,    -1,    -1,    -1,    -1,    84,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,   153,    17,   262,   263,    -1,    -1,    -1,
      -1,   268,    -1,   163,    -1,   272,   273,    -1,    -1,    -1,
      -1,   278,    -1,   280,    -1,   144,    -1,   284,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,   188,    -1,
      -1,    54,    -1,    -1,   163,    58,    59,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,   188,
      -1,    84,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,   206,   238,    -1,
      -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      17,    -1,   262,   263,    -1,    -1,   235,   236,   268,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,   144,   251,    -1,    -1,   254,   255,   256,   257,   258,
      47,    -1,    -1,   262,   263,   264,   265,   266,    -1,   268,
     163,    58,    59,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,   188,    -1,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    41,   238,    -1,    44,    -1,    -1,
      -1,    -1,   245,   246,   247,   248,   249,   144,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,   264,   265,   266,    -1,   268,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,   178,    -1,    -1,    91,    92,    93,    -1,    95,    96,
      97,   188,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    -1,   152,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,   169,    -1,    -1,   262,   263,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,   243,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    85,    -1,    -1,   262,   263,    -1,    91,    92,
      93,   268,    95,    96,    97,    -1,    99,    19,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    37,   119,    -1,   121,    41,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    -1,    -1,   169,    -1,    -1,    91,
      92,    93,    85,    95,    96,    97,    -1,    99,    91,    92,
      93,    -1,    95,    96,    97,    -1,    99,    -1,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,   121,
      -1,    -1,    -1,   206,    -1,    -1,   119,    -1,   121,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,   152,
     243,    -1,   245,   246,   247,   248,   249,   169,   251,    -1,
      -1,   254,   255,   256,   257,   258,   169,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,   268,    -1,    -1,    -1,   191,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,    -1,    -1,    -1,    -1,    -1,
      -1,   243,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     243,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,    -1,    -1,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
      -1,   245,   246,   247,   248,   249,   250,   251,   252,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
     264,   265,   266,   267,   268
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     7,    28,    36,    43,    45,    47,    52,    59,    73,
      84,   151,   158,   161,   163,   164,   188,   237,   252,   253,
     286,   287,   288,   297,   307,   317,   319,   321,   442,   443,
     469,   490,   514,   515,   516,   517,   518,   520,   522,   523,
     527,   544,   545,   546,   557,   598,   599,   600,   601,   603,
     604,   605,   610,    40,    51,    58,    83,   148,   180,   184,
     470,   201,   524,    40,    51,    58,    71,   132,   148,   165,
     180,   184,   187,   195,   214,   318,   323,   204,   273,   657,
     658,    62,    63,   308,    66,    51,    58,    62,    63,    71,
      83,   148,   165,   180,   184,   195,   214,   491,   148,   268,
       6,    47,    59,    84,   153,   163,   188,   206,   235,   236,
     238,   245,   246,   247,   248,   249,   251,   254,   255,   256,
     257,   258,   262,   263,   268,   290,   291,   292,   293,   303,
     304,   703,   711,   712,    19,    86,    19,    73,   290,   292,
     298,   303,   524,   239,   240,   563,   564,   565,    71,   176,
     183,   581,   582,   705,   711,   148,   180,   195,   320,   710,
     711,   252,     0,   276,   133,   186,   547,   484,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   233,   234,   241,   242,   250,   252,
     264,   265,   266,   267,   477,   478,   711,   695,   711,   483,
     699,   711,   393,   702,   711,   582,   705,   487,   706,   711,
     155,   525,   207,   208,   342,   344,   656,   331,   368,   622,
     691,   711,   695,   340,   698,   711,     7,   390,   702,   325,
     658,   660,   358,   582,   461,   706,   447,   709,   711,   341,
     703,    11,    48,   550,   204,   659,    68,   316,   696,   711,
     581,   694,   711,   695,    68,   696,   698,   699,   702,   660,
     705,   706,   709,   703,   702,   278,   444,   147,   278,   615,
     616,   615,   129,   277,   129,   182,   277,   622,   582,   622,
     131,   129,   129,   290,   292,    66,   182,   204,   278,   284,
     652,   663,   278,   652,   663,     6,    49,   566,   685,   560,
     565,   698,    83,    88,   123,   151,   154,   170,   196,   528,
     529,   530,   531,   532,   533,   534,   535,   537,   164,   704,
     711,   391,   702,   359,   582,   448,   709,   710,    20,     6,
     557,   267,   553,     4,    17,    52,    54,   485,   486,     4,
      52,   164,   182,   216,   473,   474,   656,     3,    80,   278,
     394,     4,     7,    52,   475,   476,   479,     3,    80,   463,
     170,   526,   207,   102,   139,   141,   164,   189,   345,   346,
     347,    10,   332,   656,   148,   184,   322,   394,    14,   111,
     326,    62,   360,    64,   615,    83,   206,   309,   687,    82,
     198,   588,   602,   687,    13,    22,    35,    41,    42,    69,
      72,    98,   100,   101,   112,   116,   173,   179,   189,   203,
     204,   205,   206,   217,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   236,   241,   242,   244,   245,   246,   250,
     272,   273,   278,   280,   425,   426,   608,   609,   621,   622,
     645,   646,   647,   650,   651,   652,   653,   654,   655,   656,
     664,   665,   666,   667,   668,   669,   670,   672,   673,   674,
     675,   676,   677,   679,   686,   687,   704,   711,   369,   399,
     445,   446,   622,   157,   395,   617,   622,   180,   289,   582,
     293,   148,   189,   305,   306,   707,   711,   304,    86,   278,
     618,   619,    66,    64,   289,   148,   129,   129,   305,   252,
     521,   278,   645,   559,   559,   560,   274,   567,   568,   569,
     608,   560,   182,   699,   107,   196,    29,   130,   185,   202,
     540,   541,   543,   582,   180,   530,   606,   607,   622,   623,
     704,   394,   360,   615,   130,   519,   548,   549,   645,   557,
     645,    64,   554,    94,   263,   330,   351,   352,   262,   263,
     262,   486,    33,   339,   458,    33,    46,   458,   622,   371,
     474,   369,   396,   397,   395,    33,   363,   368,   378,   379,
     215,    33,   622,   277,   477,   481,   622,   711,     5,    16,
     464,   465,   488,   269,   343,   343,   656,   118,   656,    46,
     263,   348,   349,   350,   351,   347,    19,    23,    41,    44,
      85,    91,    92,    93,    95,    96,    97,    99,   119,   121,
     152,   169,   191,   226,   227,   243,   492,   493,   494,   497,
     498,   499,   500,   504,   505,   506,   507,   508,   509,   511,
     512,   392,   702,   462,   706,   395,    32,   327,    94,   656,
     278,   582,    10,   699,   278,   311,   312,   313,   371,   177,
     502,   657,    38,    60,   124,   167,   256,   257,   258,   278,
     624,   626,   627,   628,   629,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   645,   278,   278,   278,   207,   278,
     278,   278,   278,   278,   278,   207,   207,   278,   197,   645,
     680,   682,   278,   278,   645,   645,   163,   609,   644,   645,
     708,   711,   277,   281,    26,    31,   272,   273,   274,   275,
     278,   283,   493,   498,   505,   269,   277,   279,   278,    10,
     277,   279,   582,   182,   296,   702,   707,   200,   277,   295,
     582,   620,   623,   163,   190,   558,   582,    66,   296,   289,
     148,   710,   645,   279,   645,    66,   571,   277,    10,   570,
     231,   273,   662,   663,   151,   534,   123,   194,   536,   536,
     277,    64,   277,   542,   174,   277,   588,   602,   269,   283,
     395,   278,    10,   277,   550,   182,   188,   200,   556,    94,
     351,   656,    94,    24,   472,    24,    46,   471,   493,   493,
     277,   279,    10,   692,   711,    30,   364,   365,   366,   492,
     493,   622,    65,   146,   187,   380,   381,   382,   383,   385,
     458,   692,   211,   213,   482,   476,   182,   216,   143,    47,
      84,   188,   466,   143,   467,   663,   660,   656,    23,    94,
     350,   278,   502,   193,   193,   145,   278,   513,    95,   278,
     510,    23,    91,   458,   281,    23,   281,   503,   278,   278,
     193,   278,   510,   394,    64,    10,   656,   656,   361,   362,
     363,   378,   463,   456,   129,   312,   157,   277,    19,    37,
     310,   497,   657,   688,   711,   135,   128,   278,   124,   626,
     639,   278,   124,   259,   260,   261,   625,   626,   640,   645,
       8,   132,    18,    34,    70,    79,    87,   106,   108,   122,
     124,   125,   126,   175,   269,   270,   271,    49,   685,   608,
      49,   274,   685,   698,   645,    49,   685,    49,   685,    49,
     685,   218,   219,   220,   221,   222,   223,   224,   225,   684,
     645,   624,    53,    54,   197,   197,   678,   645,   608,   563,
     279,   279,   279,   608,   645,   648,   690,   711,   645,   645,
     645,   645,   645,   279,   648,    42,   274,   691,   652,   446,
     398,   399,    45,   401,   402,   403,   622,    74,   148,   184,
     189,   195,   214,   299,   300,   301,   302,   707,   182,   210,
     306,    63,   611,   277,   279,   563,   278,   611,   299,   301,
      66,    66,   296,   559,   279,   278,   572,   573,   574,   578,
     579,   702,   198,   588,   569,   700,   711,   231,   663,   194,
     541,   149,   166,   538,   582,   607,   608,   691,    10,   361,
     456,   549,   248,   552,   645,   128,   555,   251,   656,   102,
     175,   353,   354,   355,   458,   278,   458,   459,    46,   269,
     400,   456,   397,   456,    20,   278,    46,   372,   458,   364,
     456,    90,    90,   616,    24,   622,   480,   493,   622,   660,
     132,   132,   132,   658,   458,   137,   138,   357,   164,   656,
     204,   277,   661,   162,   501,   658,   513,   273,   662,    46,
     333,   495,   496,   662,   164,   495,   660,   660,   278,   660,
     395,   582,   456,   102,   328,   277,   279,   464,   163,   449,
     450,   451,   582,   279,   140,   278,   314,   315,   371,   313,
     278,    20,   502,   693,   711,   558,   558,   639,     8,   132,
     279,     8,   132,   624,   624,   645,   645,     6,     9,   171,
     630,   645,   278,   641,   642,    98,   124,     6,   630,   645,
     645,     6,   630,   645,    18,    34,    79,   108,   175,     6,
     630,   645,     6,   630,   645,   200,   645,     6,   630,   645,
       6,   630,   645,     6,   630,   645,   645,   645,    10,   645,
     279,   645,   277,   279,   645,   645,   645,   645,   645,   645,
      66,    66,   181,   608,   683,   624,   645,   681,    53,    54,
     197,   277,   277,   566,   277,   282,   279,   277,   279,   192,
     406,    17,   410,   403,   707,   702,   706,   707,   709,   703,
     277,   200,   277,   294,   299,   301,   131,    66,   182,   113,
     613,   623,   566,   609,   613,   299,   301,   301,    66,   545,
     573,   578,   277,    67,    81,   105,   159,   583,   278,   580,
      74,   584,   151,   202,   539,   456,   279,   449,   561,   617,
     343,    12,   356,   355,   278,   624,   273,   373,   646,   649,
     651,   653,   654,   655,   686,   456,   456,   459,   401,   458,
     373,   459,   616,   616,   247,   384,   278,   459,   458,   456,
      84,   188,    47,   188,    47,    84,    10,   468,   489,   689,
     711,   657,   277,   279,   168,   503,   279,   277,   279,   458,
     459,   277,   282,   280,   689,   282,   279,   279,   660,   279,
      10,   463,   401,   343,   329,   330,   362,   467,   566,   200,
     460,   186,   324,   365,   616,   617,   660,   315,    55,   310,
     660,   238,    55,   279,   279,   624,   624,   624,   624,     8,
     278,   278,   163,   643,   648,    98,   278,   278,    57,   278,
     278,   645,   645,   641,   645,   200,   645,   278,   278,   278,
     278,   645,   278,   278,   278,   278,   278,   278,   279,   279,
     370,   371,   279,   279,   645,   279,   279,   279,   279,   279,
     279,   645,   645,   683,    54,   181,   181,   683,   681,   645,
     609,   645,   645,   399,   368,   404,   405,   408,   693,   711,
      58,    59,    61,    78,    84,   144,   178,   235,   255,   264,
     265,   266,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   424,   429,   430,   431,
     437,   438,   439,   440,   545,   598,   599,   603,   607,   701,
     711,   300,   302,    73,   300,   302,   294,   502,   612,   652,
     612,   614,   652,   661,   567,   279,   277,   301,   279,   279,
     573,   134,   134,   134,    89,   609,   570,    20,    77,   587,
     401,   460,   239,   249,   551,   663,   137,   663,   624,   279,
     650,   373,   373,   410,   645,   374,   375,   376,   379,   153,
     384,   550,   624,   132,   132,   132,   132,   132,   132,   458,
     459,   279,   657,   661,   662,   373,   124,   334,   335,   336,
     337,   338,   339,   496,   662,   503,   279,   456,   464,   410,
     663,   458,   567,    24,   457,     6,   451,   278,   279,   656,
      20,   212,   279,   656,   645,   643,   643,   563,   279,   279,
     643,   643,   645,   643,   643,     8,    57,   645,   643,   643,
     643,   643,   643,   643,   643,   643,   643,   643,   279,   492,
     279,   279,    64,   671,   683,   683,    54,   181,   279,   279,
     571,   493,   276,    39,   276,   695,   148,   254,   276,   278,
     645,   276,   276,   276,   701,   711,   693,   693,   441,    54,
     197,   409,   433,   434,    64,   199,   276,    86,   276,   276,
     276,   276,   280,   131,   182,   571,   570,   573,   279,   704,
     585,   586,   645,   624,   142,   589,   410,   457,   562,   357,
     279,   459,   459,   459,    26,   367,   376,   124,   146,   153,
     187,   377,   385,   582,    83,   279,   188,    84,   188,    47,
      84,    47,   401,   279,   279,    98,   367,   336,   401,   467,
     459,   357,   468,    66,   452,   131,   451,   458,   117,   101,
     238,   503,   117,   279,   279,   566,   279,   279,   279,   279,
     645,   645,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   645,   279,   683,   588,   269,   372,   407,    64,
     276,   645,   702,   645,   624,   423,   276,   693,     9,    58,
      69,   173,   435,   436,   434,    59,   545,   278,   425,   428,
     621,   704,   612,   588,   575,   704,   129,   277,    89,   114,
     172,   590,   591,   459,   459,   279,   690,    98,    90,   582,
     384,   615,   699,   459,   410,   410,   458,   459,   278,   453,
     454,   455,   581,   588,   645,   656,   656,   645,   584,   373,
     278,   276,   426,    86,   276,   279,   276,    86,   695,   697,
     711,   657,    50,   277,   254,    86,   624,   276,   277,   283,
     584,   278,   576,   624,   586,   114,   278,   384,   615,   129,
     386,   387,   388,   459,   468,   454,   455,   277,   583,   584,
     459,   571,   587,   558,   156,   427,   428,   181,   428,   409,
     436,   645,   428,   279,   425,   621,   587,   577,   700,   590,
     592,   593,   594,   705,   386,    47,   188,   384,   129,   388,
     129,   387,   459,   279,   454,    89,   587,   279,   588,   589,
     279,   278,   428,   276,   276,   409,    86,    10,   432,    50,
     589,   277,   279,   279,   277,    83,   120,   133,   595,   594,
     384,   123,   164,   211,   389,   389,   454,   584,   547,   428,
      53,   428,    39,    50,   409,   547,   700,   592,   278,   699,
     209,    46,    98,   129,   587,   553,   279,   409,    50,   693,
     409,   553,   596,   699,    83,   597,   624,   589,   409,   279,
     277,   278,   547,   596,   596,   553,   279
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
#define YYEMPTY		(-2)
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
      yytoken = YYTRANSLATE (DSQL_yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)
#endif

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

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (lex.dsql_debug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (lex.dsql_debug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (lex.dsql_debug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
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
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int dsql_yyparse (YYPARSE_PARAM);
# else
int dsql_yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int dsql_yyparse (void);
#else
int dsql_yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */

int DSQL_yychar;


/* The semantic value of the lookahead symbol.  */
static YYSTYPE yylval;

/* Number of syntax errors so far.  */
static int yynerrs;



/*----------.
| dsql_yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int dsql_yyparse (YYPARSE_PARAM)
# else
int dsql_yyparse (YYPARSE_PARAM)
  YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
dsql_yyparse (void)
#else
int
dsql_yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

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

  if (DSQL_yyss + yystacksize - 1 <= DSQL_DSQL_yyssp)
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
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
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

      if (DSQL_yyss + yystacksize - 1 <= DSQL_DSQL_yyssp)
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (DSQL_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      DSQL_yychar = YYLEX;
    }

  if (DSQL_yychar <= YYEOF)
    {
      DSQL_yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (DSQL_yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
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
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

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


  YY_REDUCE_PRINT (yyn);
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

    { lex.log_defined = false;
			  lex.cache_defined = false;
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

  case 167:

    { yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;

  case 168:

    { yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;

  case 169:

    { lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;

  case 170:

    { lex.g_file  = make_file();}
    break;

  case 175:

    { lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;

  case 176:

    { lex.g_file->fil_length = (IPTR) yyvsp[-1];}
    break;

  case 183:

    { yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;

  case 184:

    { yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;

  case 185:

    { yyval = yyvsp[0]; }
    break;

  case 186:

    { yyval = yyvsp[0]; }
    break;

  case 187:

    { yyval = NULL; }
    break;

  case 189:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 192:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;

  case 193:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 194:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 195:

    { 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;

  case 198:

    { yyval = NULL; }
    break;

  case 199:

    { yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 200:

    { yyval = yyvsp[0]; }
    break;

  case 201:

    { yyval = NULL; }
    break;

  case 202:

    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 203:

    { lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 204:

    { yyval = yyvsp[-1]; }
    break;

  case 205:

    { lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 206:

    { yyval = yyvsp[0]; }
    break;

  case 207:

    { yyval = NULL; }
    break;

  case 211:

    { yyval = yyvsp[0]; }
    break;

  case 212:

    { yyval = yyvsp[0]; }
    break;

  case 213:

    { yyval = yyvsp[0]; }
    break;

  case 214:

    { yyval = NULL; }
    break;

  case 217:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 218:

    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 219:

    { yyval = make_node (nod_null, (int) 1, NULL); }
    break;

  case 220:

    { yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 222:

    { yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;

  case 223:

    { yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;

  case 224:

    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 225:

    { yyval = yyvsp[0]; }
    break;

  case 226:

    { yyval = NULL ;}
    break;

  case 231:

    { yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 232:

    { yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 233:

    { yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 234:

    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;

  case 235:

    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 236:

    { yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 237:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;

  case 238:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;

  case 239:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;

  case 240:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;

  case 241:

    { yyval = NULL;}
    break;

  case 242:

    { yyval = yyvsp[0];}
    break;

  case 243:

    { yyval = yyvsp[0];}
    break;

  case 244:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;

  case 245:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;

  case 246:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;

  case 247:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;

  case 248:

    { yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 249:

    { yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 250:

    { yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 251:

    { yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 252:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 253:

    { yyval = NULL; }
    break;

  case 254:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 255:

    { yyval = NULL; }
    break;

  case 257:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 258:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;

  case 260:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 261:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;

  case 262:

    { yyval = yyvsp[0]; }
    break;

  case 263:

    { yyval = yyvsp[0]; }
    break;

  case 264:

    { yyval = (dsql_nod*) NULL; }
    break;

  case 265:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 266:

    { yyval = NULL; }
    break;

  case 268:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 269:

    { yyval = yyvsp[-1]; }
    break;

  case 272:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;

  case 273:

    { yyval = NULL; }
    break;

  case 274:

    { yyval = NULL; }
    break;

  case 275:

    { yyval = yyvsp[0]; }
    break;

  case 276:

    { yyval = yyvsp[0]; }
    break;

  case 277:

    { yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;

  case 280:

    { yyval = yyvsp[-1]; }
    break;

  case 281:

    { yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;

  case 282:

    { yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;

  case 283:

    { yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;

  case 285:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 301:

    { yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;

  case 302:

    { yyval = make_node (nod_exit, 0, NULL); }
    break;

  case 305:

    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
    break;

  case 306:

    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 307:

    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;

  case 308:

    { yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;

  case 309:

    { yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
    break;

  case 310:

    { yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;

  case 311:

    { yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;

  case 312:

    { yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;

  case 313:

    { yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;

  case 314:

    { yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;

  case 315:

    { yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 316:

    { yyval = NULL; }
    break;

  case 317:

    { yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;

  case 318:

    { yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;

  case 319:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 320:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 321:

    { yyval = NULL; }
    break;

  case 322:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 323:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 324:

    { yyval = NULL; }
    break;

  case 327:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 328:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 329:

    { yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;

  case 330:

    { yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;

  case 331:

    { yyval = NULL; }
    break;

  case 332:

    { yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;

  case 333:

    { yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;

  case 334:

    { yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
    break;

  case 335:

    { yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 336:

    { yyval = NULL; }
    break;

  case 338:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 339:

    { yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;

  case 341:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 342:

    { yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;

  case 343:

    { yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;

  case 344:

    { yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;

  case 345:

    { yyval = make_node (nod_default, 1, NULL); }
    break;

  case 349:

    { yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;

  case 350:

    { yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;

  case 351:

    { yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;

  case 352:

    { yyval = NULL; }
    break;

  case 353:

    { yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;

  case 354:

    { yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;

  case 355:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 356:

    { yyval = NULL; }
    break;

  case 358:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 359:

    { yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;

  case 360:

    { yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 361:

    { yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 362:

    { yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
    break;

  case 363:

    { yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;

  case 364:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 365:

    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 366:

    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;

  case 367:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 369:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 372:

    { yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 373:

    { yyval = yyvsp[-1]; }
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

    { yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
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

    { yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 417:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 418:

    { yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;

  case 419:

    { yyval = yyvsp[0]; }
    break;

  case 420:

    { yyval = yyvsp[0]; }
    break;

  case 421:

    {yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;

  case 422:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;

  case 423:

    { yyval = yyvsp[0]; }
    break;

  case 424:

    { yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;

  case 426:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 427:

    { yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 428:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;

  case 429:

    { yyval = yyvsp[0]; }
    break;

  case 430:

    { yyval = yyvsp[0]; }
    break;

  case 431:

    { yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 432:

    { yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 433:

    { yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;

  case 434:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 462:

    { yyval = NULL; }
    break;

  case 463:

    { yyval = NULL; }
    break;

  case 464:

    { yyval = NULL; }
    break;

  case 465:

    { yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 466:

    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 467:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 468:

    { yyval = make_node (nod_cascade, 0, NULL); }
    break;

  case 469:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 470:

    { yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;

  case 471:

    { yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;

  case 472:

    { lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
    break;

  case 474:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 475:

    { yyval = yyvsp[0]; }
    break;

  case 476:

    { yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;

  case 477:

    { yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;

  case 478:

    { yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;

  case 479:

    { yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;

  case 480:

    { yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 482:

    { yyval = NULL; }
    break;

  case 484:

    { yyval = NULL; }
    break;

  case 485:

    { yyval = yyvsp[0]; }
    break;

  case 486:

    { yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;

  case 487:

    { yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;

  case 488:

    { yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;

  case 489:

    { yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;

  case 490:

    { yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;

  case 491:

    { yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;

  case 492:

    { yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;

  case 493:

    { yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;

  case 494:

    { yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;

  case 495:

    { yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;

  case 496:

    { yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;

  case 497:

    { yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;

  case 502:

    { lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;

  case 503:

    { lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;

  case 505:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 506:

    { if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;

  case 507:

    { yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 513:

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

  case 514:

    { 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;

  case 515:

    { 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;

  case 516:

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

  case 517:

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

  case 518:

    { 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;

  case 521:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;

  case 522:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;

  case 523:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 524:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 525:

    {
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;

  case 526:

    {
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;

  case 527:

    {
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;

  case 528:

    {
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;

  case 529:

    {
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;

  case 530:

    {
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;

  case 532:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 533:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 534:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 535:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 536:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;

  case 537:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 546:

    { 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;

  case 547:

    {  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;

  case 548:

    {
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;

  case 549:

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

  case 550:

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

  case 553:

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

  case 554:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 555:

    { 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;

  case 556:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 557:

    { yyval = yyvsp[-1]; }
    break;

  case 558:

    { yyval = 0; }
    break;

  case 562:

    { 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));
			}
    break;

  case 563:

    {
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64));
			}
    break;

  case 564:

    {
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((dsql_str*)yyvsp[0], CONSTANT_SINT64)));
			}
    break;

  case 568:

    { yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;

  case 569:

    { yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 570:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 571:

    { yyval = 0; }
    break;

  case 572:

    { yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;

  case 575:

    { yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;

  case 576:

    { yyval = make_node (nod_rollback, 0, NULL); }
    break;

  case 579:

    { yyval = make_node (nod_commit_retain, 0, NULL); }
    break;

  case 580:

    { yyval = NULL; }
    break;

  case 582:

    { yyval = NULL; }
    break;

  case 583:

    {yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;

  case 585:

    { yyval = NULL; }
    break;

  case 587:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 592:

    { yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;

  case 593:

    { yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;

  case 594:

    { yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;

  case 595:

    { yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;

  case 596:

    { yyval = yyvsp[0];}
    break;

  case 598:

    { yyval = yyvsp[0];}
    break;

  case 599:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 600:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 601:

    { yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;

  case 602:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 603:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 604:

    { yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;

  case 605:

    { yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;

  case 606:

    { yyval = 0; }
    break;

  case 607:

    { yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;

  case 608:

    { yyval = (dsql_nod*) NOD_SHARED; }
    break;

  case 609:

    { yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;

  case 610:

    { yyval = (dsql_nod*) 0; }
    break;

  case 611:

    { yyval = (dsql_nod*) NOD_READ; }
    break;

  case 612:

    { yyval = (dsql_nod*) NOD_WRITE; }
    break;

  case 614:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 615:

    { yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 616:

    { yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;

  case 617:

    { yyval = 0; }
    break;

  case 619:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 620:

    {yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;

  case 621:

    { yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 622:

    { yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;

  case 623:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 624:

    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 625:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 626:

    { yyval = 0; }
    break;

  case 628:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 629:

    { yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 630:

    { yyval = 0; }
    break;

  case 631:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 632:

    { yyval = 0; }
    break;

  case 633:

    { yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;

  case 634:

    { yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;

  case 635:

    { yyval = yyvsp[-1]; }
    break;

  case 636:

    { yyval = 0; }
    break;

  case 637:

    { yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;

  case 638:

    { yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;

  case 639:

    { yyval = NULL; }
    break;

  case 640:

    { yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;

  case 641:

    { yyval = 0; }
    break;

  case 642:

    { yyval = yyvsp[0]; }
    break;

  case 643:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 644:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 645:

    { yyval = 0; }
    break;

  case 646:

    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
    break;

  case 647:

    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 648:

    { lex.limit_clause = true; }
    break;

  case 649:

    { lex.limit_clause = false; }
    break;

  case 650:

    { lex.first_detection = true; }
    break;

  case 651:

    { lex.first_detection = false; }
    break;

  case 652:

    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;

  case 653:

    { yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;

  case 654:

    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;

  case 655:

    { yyval = 0; }
    break;

  case 656:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;

  case 657:

    { yyval = yyvsp[-2]; }
    break;

  case 658:

    { yyval = yyvsp[-1]; }
    break;

  case 659:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 660:

    { yyval = yyvsp[-1]; }
    break;

  case 661:

    { yyval = yyvsp[0]; }
    break;

  case 662:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 663:

    { yyval = 0; }
    break;

  case 664:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 665:

    { yyval = 0; }
    break;

  case 667:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 669:

    { yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 672:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 674:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 678:

    { yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;

  case 680:

    { yyval = NULL; }
    break;

  case 681:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 682:

    { yyval = NULL; }
    break;

  case 684:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 685:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 686:

    { yyval = yyvsp[-1]; }
    break;

  case 687:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;

  case 688:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 689:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 690:

    { yyval = NULL; }
    break;

  case 692:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 693:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;

  case 694:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 695:

    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 696:

    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 697:

    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 698:

    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 699:

    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 700:

    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 701:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 702:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 703:

    { yyval = 0; }
    break;

  case 705:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 707:

    { yyval = yyvsp[0]; }
    break;

  case 708:

    { yyval = 0; }
    break;

  case 709:

    { yyval = yyvsp[0]; }
    break;

  case 710:

    { yyval = 0; }
    break;

  case 711:

    { yyval = yyvsp[0]; }
    break;

  case 712:

    { yyval = 0; }
    break;

  case 713:

    { yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 714:

    { yyval = 0; }
    break;

  case 715:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 716:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 717:

    { yyval = 0; }
    break;

  case 718:

    { yyval = 0; }
    break;

  case 720:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 721:

    { yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 724:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 725:

    { yyval = make_node (nod_natural, (int) 0, NULL); }
    break;

  case 726:

    { yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;

  case 727:

    { yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 729:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 730:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 731:

    { yyval = 0; }
    break;

  case 732:

    { yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;

  case 733:

    { yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 736:

    { yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 737:

    { yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 738:

    { yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 741:

    { yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;

  case 742:

    { yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;

  case 744:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 745:

    { yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;

  case 749:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 750:

    { yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 751:

    { yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 752:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 753:

    { yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;

  case 757:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 758:

    { yyval = yyvsp[0]; }
    break;

  case 760:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 763:

    { yyval = NULL; }
    break;

  case 764:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 766:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 768:

    { yyval = NULL; }
    break;

  case 769:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 771:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 773:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 774:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;

  case 775:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 777:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 779:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 781:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 782:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 784:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 785:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 786:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 787:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 788:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 790:

    { yyval = yyvsp[-1]; }
    break;

  case 791:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 802:

    { yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 803:

    { yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 804:

    { yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 805:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 806:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 807:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 808:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 809:

    { yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 810:

    { yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 811:

    { yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 812:

    { yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 813:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 814:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 815:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 816:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 817:

    { yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 818:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 819:

    { yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 820:

    { yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 821:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 822:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 823:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 824:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 825:

    { yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 828:

    { yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 829:

    { yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 830:

    { yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 831:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 832:

    { yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 833:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 834:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 835:

    { yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 836:

    { yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 837:

    { yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 838:

    { yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 839:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 840:

    { yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 841:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 842:

    { yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;

  case 843:

    { yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;

  case 844:

    { yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;

  case 845:

    { yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;

  case 846:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 847:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;

  case 848:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;

  case 849:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 850:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;

  case 851:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;

  case 853:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 854:

    { yyval = yyvsp[-1]; }
    break;

  case 855:

    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 856:

    { yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 866:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 867:

    { yyval = yyvsp[0]; }
    break;

  case 868:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 869:

    { yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 870:

    { yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;

  case 871:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 872:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 873:

    {
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 874:

    { yyval = yyvsp[-1]; }
    break;

  case 875:

    { yyval = yyvsp[-1]; }
    break;

  case 879:

    { yyval = make_node (nod_dbkey, 1, NULL); }
    break;

  case 880:

    { yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;

  case 881:

    { 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;

  case 882:

    { yyval = yyvsp[0]; }
    break;

  case 883:

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

  case 884:

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

  case 885:

    { yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;

  case 886:

    { yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 888:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 890:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 891:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;

  case 892:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 893:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;

  case 894:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 895:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 897:

    { yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;

  case 898:

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

  case 899:

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

  case 900:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;

  case 901:

    { yyval = make_parameter (); }
    break;

  case 902:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 903:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 904:

    { yyval = make_node (nod_current_role, 0, NULL); }
    break;

  case 905:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;

  case 906:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;

  case 907:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;

  case 908:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;

  case 909:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;

  case 910:

    { yyval = yyvsp[0]; }
    break;

  case 911:

    { ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;

  case 913:

    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 914:

    { if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 915:

    { if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 916:

    { if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;

  case 917:

    { if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 919:

    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 920:

    { yyval = yyvsp[0];}
    break;

  case 925:

    { yyval = make_node (nod_agg_count, 0, NULL); }
    break;

  case 926:

    { yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;

  case 927:

    { yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;

  case 928:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;

  case 929:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 930:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;

  case 931:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 932:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 933:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 934:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 935:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 936:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;

  case 938:

    { yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;

  case 940:

    { yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;

  case 941:

    { yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;

  case 942:

    { yyval = yyvsp[0]; }
    break;

  case 943:

    { yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;

  case 944:

    { yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 945:

    { yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;

  case 946:

    { yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;

  case 949:

    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;

  case 950:

    { yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 953:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 954:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 955:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 956:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 957:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 958:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 959:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 960:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 964:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;

  case 965:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;

  case 966:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;

  case 967:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;

  case 968:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;

  case 969:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;

  case 970:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;

  case 971:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;

  case 974:

    { yyval = make_node (nod_null, 0, NULL); }
    break;


    }

/* Line 999 of yacc.c.  */


  yyvsp -= yylen;
  DSQL_DSQL_yyssp -= yylen;


  YY_STACK_PRINT (DSQL_yyss, DSQL_DSQL_yyssp);

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
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
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
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



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
	  while (DSQL_yyss < DSQL_DSQL_yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*DSQL_DSQL_yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*DSQL_DSQL_yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      DSQL_yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
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

      YYDSYMPRINTF ("Error: popping", yystos[*DSQL_DSQL_yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--DSQL_DSQL_yyssp;

      YY_STACK_PRINT (DSQL_yyss, DSQL_DSQL_yyssp);
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
	tsql* tdsql = GET_THREAD_DATA;

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
	tsql* tdsql = GET_THREAD_DATA;
		   
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
	tsql* tdsql = GET_THREAD_DATA;

	if (!node)
		return node;

	DsqlNodStack stack;
	stack_nodes (node, stack);
	USHORT l = stack.getCount();

	dsql_nod* old  = node;
	node = FB_NEW_RPT(*tdsql->tsql_default, l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	dsql_nod** ptr = node->nod_arg + node->nod_count;

	while (stack.notEmpty())
		*--ptr = stack.pop();

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
	tsql* tdsql = GET_THREAD_DATA;

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
	tsql* tdsql = GET_THREAD_DATA;

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
	tsql* tdsql = GET_THREAD_DATA;

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


