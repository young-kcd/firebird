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
     PAGE_SIZE = 394,
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
     KW_DIFFERENCE = 518
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
#define PAGE_SIZE 394
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
 */

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <windows.h>
#include <stdio.h>
/*#include <wincon.h>*/
#endif

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>

#include "gen/iberror.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/sym.h"
#include "../jrd/gds.h"
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
#include "../wal/wal.h"
#include "../jrd/err_proto.h"

ASSERT_FILENAME

static void	yyerror (TEXT *);

/* since UNIX isn't standard, we have to define
   stuff which is in <limits.h> (which isn't available
   on all UNIXes... */

#define SHRT_POS_MAX		32767
#define SHRT_UNSIGNED_MAX	65535
#define SHRT_NEG_MAX		32768
#define LONG_POS_MAX		2147483647
#define POSITIVE		0
#define NEGATIVE		1
#define UNSIGNED		2

#define MIN_CACHE_BUFFERS	   250
#define DEF_CACHE_BUFFERS	   1000

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE		2048
#define YYMAXDEPTH		2048

#define YYSTYPE		DSQL_NOD
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME [] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING [] = "";	

#define TRIGGER_TYPE_SUFFIX(slot1, slot2, slot3) \
	((slot1 << 1) | (slot2 << 3) | (slot3 << 5))

DSQL_NOD		DSQL_parse;

static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

#define MAX_TOKEN_LEN   256
#define CHECK_BOUND(to)\
	{\
	if ((to - string) >= MAX_TOKEN_LEN)		\
	yyabandon (-104, isc_token_too_long); \
	}
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static bool		long_int(DSQL_NOD, SLONG *);
#endif
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(DSQL_NOD, SLONG *, SSHORT);
#endif
static void	stack_nodes (DSQL_NOD, DLLS *);
inline static int	yylex (USHORT, USHORT, USHORT, BOOLEAN *);
static void	yyabandon (SSHORT, ISC_STATUS);
static void	check_log_file_attrs (void);

struct LexerState {
	/* This is, in fact, parser state. Not used in lexer itself */
	DSQL_FLD g_field;
	FIL	g_file;
	DSQL_NOD g_field_name;
	SSHORT log_defined, cache_defined;
	int dsql_debug;
	
	/* Actual lexer state begins from here */
	TEXT *beginning;
	TEXT	*ptr, *end, *last_token, *line_start;
	TEXT	*last_token_bk, *line_start_bk;
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
		BOOLEAN	*stmt_ambiguous);
};

/* Get ready for thread-safety. Move this to BISON object pointer when we 
   switch to generating "pure" reenterant parser. */
static struct LexerState lex;



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
#define YYFINAL  159
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   6668

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  280
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  425
/* YYNRULES -- Number of rules. */
#define YYNRULES  1013
/* YYNRULES -- Number of states. */
#define YYNSTATES  1876

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   518

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     273,   274,   269,   267,   272,   268,   278,   270,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   275,   271,
     265,   264,   266,   279,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   276,     2,   277,     2,     2,     2,     2,     2,     2,
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
     255,   256,   257,   258,   259,   260,   261,   262,   263
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    47,    55,    64,    71,    79,    85,    87,
      90,    92,    95,    97,    99,   103,   105,   107,   109,   111,
     114,   117,   121,   122,   126,   127,   129,   137,   146,   153,
     161,   168,   176,   181,   185,   187,   191,   195,   199,   202,
     205,   208,   211,   213,   217,   219,   222,   225,   227,   231,
     233,   235,   239,   241,   244,   247,   250,   254,   263,   265,
     267,   273,   274,   276,   280,   282,   286,   289,   294,   296,
     300,   303,   307,   312,   317,   320,   330,   333,   337,   345,
     348,   351,   354,   357,   360,   363,   366,   369,   372,   375,
     378,   381,   384,   389,   392,   395,   397,   398,   400,   402,
     409,   416,   418,   420,   421,   422,   424,   425,   430,   431,
     433,   435,   438,   447,   449,   450,   454,   455,   456,   458,
     460,   463,   465,   467,   469,   472,   479,   481,   483,   487,
     488,   490,   492,   493,   495,   497,   500,   504,   509,   512,
     515,   519,   520,   522,   524,   527,   529,   531,   533,   538,
     542,   546,   550,   554,   558,   560,   562,   569,   573,   575,
     579,   581,   585,   588,   590,   591,   594,   598,   600,   601,
     603,   605,   608,   612,   617,   618,   620,   623,   624,   626,
     628,   634,   640,   644,   647,   648,   650,   654,   656,   658,
     665,   669,   672,   679,   682,   684,   687,   690,   693,   694,
     696,   698,   701,   702,   705,   706,   708,   710,   712,   714,
     716,   718,   719,   721,   723,   726,   729,   732,   738,   740,
     743,   747,   750,   753,   754,   756,   758,   760,   762,   766,
     771,   780,   785,   786,   793,   795,   797,   800,   803,   804,
     808,   812,   814,   817,   820,   823,   832,   841,   850,   859,
     863,   864,   867,   868,   870,   874,   877,   879,   880,   882,
     885,   892,   894,   895,   898,   900,   902,   904,   908,   910,
     913,   914,   916,   919,   922,   925,   927,   929,   931,   933,
     935,   937,   940,   942,   944,   947,   949,   951,   953,   956,
     959,   961,   965,   970,   973,   980,   985,   987,   989,   998,
    1008,  1015,  1024,  1031,  1036,  1037,  1042,  1045,  1047,  1051,
    1052,  1055,  1060,  1061,  1063,  1065,  1069,  1073,  1081,  1084,
    1085,  1088,  1091,  1095,  1099,  1100,  1102,  1105,  1110,  1112,
    1116,  1119,  1122,  1125,  1127,  1132,  1140,  1148,  1150,  1152,
    1156,  1161,  1169,  1172,  1174,  1178,  1180,  1182,  1189,  1193,
    1194,  1195,  1196,  1197,  1201,  1202,  1212,  1222,  1224,  1226,
    1227,  1230,  1232,  1234,  1236,  1238,  1240,  1244,  1248,  1252,
    1256,  1260,  1264,  1270,  1276,  1282,  1288,  1294,  1300,  1303,
    1304,  1309,  1312,  1316,  1320,  1323,  1326,  1330,  1334,  1337,
    1341,  1348,  1350,  1353,  1358,  1362,  1365,  1368,  1371,  1374,
    1378,  1380,  1384,  1388,  1392,  1395,  1398,  1403,  1408,  1414,
    1416,  1418,  1420,  1422,  1424,  1426,  1428,  1430,  1432,  1434,
    1436,  1438,  1440,  1442,  1444,  1446,  1448,  1450,  1452,  1454,
    1456,  1458,  1460,  1462,  1464,  1467,  1470,  1473,  1475,  1477,
    1479,  1480,  1483,  1486,  1487,  1489,  1492,  1495,  1498,  1501,
    1504,  1509,  1513,  1516,  1519,  1521,  1525,  1533,  1535,  1536,
    1538,  1539,  1542,  1545,  1548,  1551,  1554,  1557,  1560,  1563,
    1566,  1570,  1573,  1576,  1579,  1581,  1583,  1585,  1587,  1592,
    1598,  1600,  1604,  1606,  1610,  1612,  1615,  1617,  1619,  1621,
    1623,  1625,  1627,  1629,  1631,  1633,  1635,  1637,  1642,  1647,
    1654,  1660,  1664,  1665,  1668,  1671,  1672,  1676,  1677,  1682,
    1684,  1690,  1695,  1697,  1702,  1704,  1707,  1710,  1712,  1714,
    1716,  1719,  1722,  1725,  1728,  1729,  1733,  1739,  1741,  1743,
    1746,  1750,  1752,  1755,  1759,  1760,  1762,  1764,  1766,  1772,
    1778,  1785,  1787,  1789,  1791,  1794,  1799,  1801,  1802,  1808,
    1810,  1811,  1815,  1818,  1820,  1821,  1824,  1825,  1827,  1828,
    1832,  1834,  1835,  1837,  1840,  1842,  1844,  1846,  1848,  1851,
    1854,  1856,  1859,  1863,  1865,  1867,  1871,  1875,  1877,  1880,
    1884,  1886,  1889,  1890,  1893,  1895,  1897,  1898,  1900,  1902,
    1904,  1908,  1911,  1915,  1916,  1918,  1922,  1927,  1932,  1934,
    1938,  1943,  1947,  1948,  1950,  1954,  1958,  1960,  1962,  1963,
    1965,  1967,  1972,  1973,  1977,  1978,  1981,  1982,  1985,  1986,
    1996,  2007,  2008,  2009,  2010,  2011,  2015,  2018,  2020,  2021,
    2025,  2031,  2035,  2038,  2044,  2047,  2049,  2051,  2053,  2055,
    2057,  2061,  2063,  2067,  2069,  2070,  2073,  2075,  2079,  2081,
    2083,  2085,  2092,  2094,  2095,  2099,  2100,  2102,  2106,  2113,
    2117,  2122,  2125,  2129,  2130,  2132,  2136,  2138,  2140,  2142,
    2145,  2147,  2149,  2151,  2154,  2156,  2159,  2161,  2164,  2165,
    2169,  2170,  2172,  2176,  2178,  2181,  2182,  2185,  2186,  2189,
    2190,  2195,  2197,  2200,  2202,  2204,  2205,  2207,  2211,  2214,
    2216,  2218,  2221,  2223,  2228,  2232,  2234,  2238,  2243,  2244,
    2253,  2259,  2261,  2265,  2267,  2269,  2274,  2279,  2284,  2286,
    2288,  2294,  2300,  2302,  2306,  2310,  2312,  2314,  2322,  2330,
    2336,  2340,  2341,  2343,  2345,  2347,  2350,  2351,  2353,  2355,
    2357,  2358,  2362,  2364,  2368,  2370,  2371,  2375,  2377,  2381,
    2383,  2387,  2391,  2393,  2395,  2399,  2401,  2404,  2406,  2410,
    2414,  2416,  2419,  2423,  2427,  2431,  2435,  2437,  2441,  2444,
    2446,  2448,  2450,  2452,  2454,  2456,  2458,  2460,  2462,  2464,
    2468,  2472,  2476,  2480,  2484,  2488,  2492,  2496,  2503,  2510,
    2517,  2524,  2531,  2538,  2545,  2552,  2559,  2566,  2573,  2580,
    2587,  2594,  2601,  2608,  2610,  2612,  2618,  2625,  2629,  2634,
    2640,  2647,  2651,  2656,  2660,  2665,  2669,  2674,  2679,  2685,
    2690,  2695,  2699,  2704,  2706,  2708,  2710,  2712,  2714,  2716,
    2718,  2722,  2726,  2737,  2748,  2750,  2752,  2754,  2756,  2758,
    2760,  2762,  2764,  2766,  2769,  2772,  2776,  2780,  2784,  2788,
    2792,  2796,  2800,  2804,  2806,  2808,  2810,  2812,  2816,  2818,
    2820,  2822,  2824,  2826,  2831,  2833,  2837,  2839,  2842,  2844,
    2846,  2848,  2850,  2852,  2854,  2856,  2859,  2862,  2865,  2867,
    2869,  2871,  2873,  2875,  2877,  2879,  2881,  2883,  2885,  2888,
    2890,  2893,  2895,  2897,  2899,  2901,  2903,  2906,  2908,  2910,
    2912,  2914,  2916,  2921,  2927,  2933,  2939,  2945,  2951,  2957,
    2963,  2969,  2975,  2981,  2988,  2990,  2997,  2999,  3004,  3012,
    3015,  3016,  3021,  3025,  3032,  3034,  3036,  3043,  3050,  3052,
    3054,  3059,  3066,  3071,  3077,  3081,  3087,  3092,  3098,  3100,
    3102,  3104,  3106,  3108,  3110,  3112,  3114,  3116,  3118,  3120,
    3122,  3123,  3125,  3127,  3129,  3131,  3133,  3135,  3137,  3139,
    3141,  3143,  3145,  3147,  3149,  3151,  3153,  3155,  3157,  3159,
    3161,  3163,  3165,  3167,  3169,  3171,  3173,  3175,  3177,  3179,
    3181,  3183,  3185,  3187,  3189,  3191,  3193,  3195,  3197,  3199,
    3201,  3203,  3205,  3207
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     281,     0,    -1,   282,    -1,   282,   271,    -1,   459,    -1,
     602,    -1,   513,    -1,   312,    -1,   302,    -1,   592,    -1,
     481,    -1,   283,    -1,   590,    -1,   436,    -1,   314,    -1,
     316,    -1,   292,    -1,   514,    -1,   507,    -1,   536,    -1,
     505,    -1,   596,    -1,    43,   649,    -1,    73,   285,   129,
     284,   182,   296,   289,    -1,    73,   287,   129,   148,   291,
     182,   296,   289,    -1,    73,   285,   129,   284,   182,   294,
      -1,    73,   287,   129,   148,   291,   182,   294,    -1,    73,
     298,   182,   300,   290,    -1,   574,    -1,   180,   574,    -1,
       6,    -1,     6,   147,    -1,   286,    -1,   288,    -1,   286,
     272,   288,    -1,    59,    -1,   163,    -1,    84,    -1,    47,
      -1,   188,   607,    -1,   153,   607,    -1,   200,    73,   131,
      -1,    -1,   200,   210,   131,    -1,    -1,   694,    -1,   158,
     293,   285,   129,   284,    66,   296,    -1,   158,   293,   287,
     129,   148,   291,    66,   296,    -1,   158,   285,   129,   284,
      66,   296,    -1,   158,   287,   129,   148,   291,    66,   296,
      -1,   158,   285,   129,   284,    66,   294,    -1,   158,   287,
     129,   148,   291,    66,   294,    -1,   158,   298,    66,   300,
      -1,    73,   131,    64,    -1,   295,    -1,   294,   272,   295,
      -1,   294,   272,   297,    -1,   296,   272,   295,    -1,   148,
     694,    -1,   184,   698,    -1,   195,   701,    -1,   214,   695,
      -1,   297,    -1,   296,   272,   297,    -1,   699,    -1,   189,
     699,    -1,    74,   699,    -1,   299,    -1,   298,   272,   299,
      -1,   695,    -1,   301,    -1,   300,   272,   301,    -1,   699,
      -1,   189,   699,    -1,    45,   303,    -1,    63,   311,    -1,
      62,    68,   304,    -1,   679,   306,   157,   309,    55,   648,
     117,   648,    -1,   488,    -1,    19,    -1,    37,   273,   652,
     274,   494,    -1,    -1,   307,    -1,   273,   307,   274,    -1,
     308,    -1,   307,   272,   308,    -1,   375,   305,    -1,   375,
     305,    20,   238,    -1,   310,    -1,   273,   310,   274,    -1,
     375,   305,    -1,   375,   305,   212,    -1,   375,   305,    20,
     101,    -1,   375,   305,    20,   238,    -1,   140,   652,    -1,
     688,    82,   493,   135,   493,    55,   648,   117,   648,    -1,
      36,   313,    -1,    58,   687,   648,    -1,   318,   541,    83,
     691,   129,   574,   319,    -1,   148,   394,    -1,   180,   362,
      -1,   184,   451,    -1,   195,   437,    -1,    71,   335,    -1,
      40,   337,    -1,    51,   326,    -1,   165,   320,    -1,   214,
     336,    -1,   237,   315,    -1,   148,   395,    -1,   180,   363,
      -1,   195,   438,    -1,    36,   132,     7,   317,    -1,   148,
     396,    -1,   184,   452,    -1,   187,    -1,    -1,   609,    -1,
     608,    -1,   369,   273,   448,   637,   449,   274,    -1,   652,
     321,   322,   648,   323,   324,    -1,   111,    -1,    14,    -1,
      -1,    -1,    32,    -1,    -1,   102,   338,   655,   361,    -1,
      -1,   325,    -1,   350,    -1,   325,   350,    -1,   372,   327,
     483,   448,   328,   449,   329,   371,    -1,    10,    -1,    -1,
      46,   448,   377,    -1,    -1,    -1,   330,    -1,   331,    -1,
     330,   331,    -1,   332,    -1,   333,    -1,   334,    -1,   124,
      98,    -1,   448,    24,   273,   616,   274,   449,    -1,   690,
      -1,   695,    -1,   339,   340,   343,    -1,    -1,   264,    -1,
     648,    -1,    -1,   341,    -1,   342,    -1,   341,   342,    -1,
     139,   338,   652,    -1,   102,   338,   655,   361,    -1,   189,
     648,    -1,   141,   648,    -1,   164,   118,   648,    -1,    -1,
     344,    -1,   345,    -1,   344,   345,    -1,   350,    -1,   347,
      -1,   346,    -1,    46,    23,   164,   681,    -1,   263,    94,
     648,    -1,    75,   338,   655,    -1,    25,   338,   655,    -1,
     127,   338,   652,    -1,   109,   338,   653,    -1,   349,    -1,
     348,    -1,   103,   273,   351,   274,   136,   352,    -1,   103,
      15,   352,    -1,   103,    -1,   356,   648,   357,    -1,   352,
      -1,   351,   272,   352,    -1,   353,   354,    -1,   648,    -1,
      -1,   354,   355,    -1,   168,   338,   655,    -1,    94,    -1,
      -1,   358,    -1,   359,    -1,   358,   359,    -1,   175,   360,
     655,    -1,   102,   338,   655,   361,    -1,    -1,    12,    -1,
      12,   137,    -1,    -1,   137,    -1,   138,    -1,   574,   364,
     273,   365,   274,    -1,   574,   364,   273,   365,   274,    -1,
      62,    94,   648,    -1,    62,   648,    -1,    -1,   366,    -1,
     365,   272,   366,    -1,   367,    -1,   382,    -1,   372,   370,
     376,   449,   378,   371,    -1,   372,   484,   368,    -1,   372,
     368,    -1,   369,   273,   448,   637,   449,   274,    -1,    30,
      20,    -1,    30,    -1,   483,   448,    -1,   614,   446,    -1,
      26,   682,    -1,    -1,   614,    -1,   614,    -1,   375,   483,
      -1,    -1,    46,   377,    -1,    -1,   641,    -1,   645,    -1,
     646,    -1,   647,    -1,   678,    -1,   638,    -1,    -1,   379,
      -1,   380,    -1,   379,   380,    -1,   383,   381,    -1,   124,
      98,    -1,   153,   574,   607,   390,   388,    -1,   389,    -1,
     187,   388,    -1,   146,    90,   388,    -1,   383,   384,    -1,
      33,   684,    -1,    -1,   385,    -1,   386,    -1,   387,    -1,
     389,    -1,   187,   608,   388,    -1,   146,    90,   608,   388,
      -1,    65,    90,   608,   153,   574,   607,   390,   388,    -1,
     247,   541,    83,   691,    -1,    -1,   448,    24,   273,   616,
     274,   449,    -1,   391,    -1,   392,    -1,   392,   391,    -1,
     391,   392,    -1,    -1,   129,   188,   393,    -1,   129,    47,
     393,    -1,   211,    -1,   164,    46,    -1,   164,    98,    -1,
     123,   209,    -1,   694,   398,   399,    10,   446,   402,   408,
     449,    -1,   694,   398,   399,    10,   446,   402,   408,   449,
      -1,   694,   398,   399,    10,   446,   402,   408,   449,    -1,
     694,   398,   399,    10,   446,   402,   408,   449,    -1,   273,
     400,   274,    -1,    -1,   157,   398,    -1,    -1,   401,    -1,
     400,   272,   401,    -1,   373,   484,    -1,   403,    -1,    -1,
     404,    -1,   403,   404,    -1,    45,   405,   372,   484,   406,
     271,    -1,   192,    -1,    -1,   264,   377,    -1,   376,    -1,
     411,    -1,   408,    -1,    17,   409,    54,    -1,   410,    -1,
     410,   432,    -1,    -1,   407,    -1,   410,   407,    -1,   600,
     271,    -1,   592,   271,    -1,   412,    -1,   413,    -1,   414,
      -1,   415,    -1,   417,    -1,   420,    -1,   590,   271,    -1,
     421,    -1,   423,    -1,   596,   271,    -1,   428,    -1,   418,
      -1,   419,    -1,   178,   271,    -1,    61,   271,    -1,   430,
      -1,    58,   687,   271,    -1,    58,   687,   637,   271,    -1,
      58,   271,    -1,    59,   148,   694,   425,   426,   271,    -1,
      59,   416,   637,   271,    -1,   191,    -1,   254,    -1,   429,
      64,   536,    86,   427,   431,    50,   407,    -1,   429,    64,
      59,   416,   637,    86,   427,    50,   407,    -1,    59,   416,
     637,    86,   427,   271,    -1,    78,   273,   616,   274,   181,
     407,    53,   407,    -1,    78,   273,   616,   274,   181,   407,
      -1,   144,   637,   422,   271,    -1,    -1,   536,    86,   427,
     271,    -1,   275,   700,    -1,   571,    -1,   273,   571,   274,
      -1,    -1,   156,   427,    -1,   156,   273,   427,   274,    -1,
      -1,   424,    -1,   613,    -1,   427,   272,   613,    -1,   427,
     272,   424,    -1,   429,   199,   273,   616,   274,    50,   407,
      -1,   693,   275,    -1,    -1,   235,   271,    -1,   255,   271,
      -1,   255,   693,   271,    -1,    10,    39,   685,    -1,    -1,
     433,    -1,   432,   433,    -1,   197,   434,    50,   407,    -1,
     435,    -1,   434,   272,   435,    -1,   173,   649,    -1,    69,
     689,    -1,    58,   687,    -1,     9,    -1,    59,   148,   694,
     425,    -1,   701,   607,    10,   446,   439,   450,   447,    -1,
     701,   607,    10,   446,   439,   450,   447,    -1,   440,    -1,
     441,    -1,   440,   186,   441,    -1,   440,   186,     6,   441,
      -1,   163,   556,   557,   442,   580,   576,   579,    -1,    66,
     443,    -1,   444,    -1,   443,   272,   444,    -1,   445,    -1,
     573,    -1,   444,   575,    89,   444,   129,   616,    -1,   273,
     445,   274,    -1,    -1,    -1,    -1,    -1,   200,    24,   131,
      -1,    -1,   698,    64,   574,   453,   454,   457,   448,   458,
     449,    -1,   698,    64,   574,   453,   454,   457,   448,   458,
     449,    -1,     3,    -1,    80,    -1,    -1,   455,   456,    -1,
      16,    -1,     5,    -1,    84,    -1,   188,    -1,    47,    -1,
      84,   132,   188,    -1,    84,   132,    47,    -1,   188,   132,
      84,    -1,   188,   132,    47,    -1,    47,   132,    84,    -1,
      47,   132,   188,    -1,    84,   132,   188,   132,    47,    -1,
      84,   132,    47,   132,   188,    -1,   188,   132,    84,   132,
      47,    -1,   188,   132,    47,   132,    84,    -1,    47,   132,
      84,   132,   188,    -1,    47,   132,   188,   132,    84,    -1,
     143,   650,    -1,    -1,    10,   448,   402,   408,    -1,     7,
     460,    -1,    58,   687,   648,    -1,   180,   574,   465,    -1,
     184,   478,    -1,   148,   397,    -1,    40,   474,   475,    -1,
      51,   467,   463,    -1,    83,   473,    -1,    46,   448,   377,
      -1,    24,   448,   273,   616,   274,   449,    -1,   464,    -1,
     463,   464,    -1,   164,   448,   461,   449,    -1,     4,    33,
     462,    -1,     4,   334,    -1,    52,    46,    -1,    52,    33,
      -1,   182,   614,    -1,   216,   375,   484,    -1,   466,    -1,
     465,   272,   466,    -1,    52,   614,   472,    -1,    52,    33,
     684,    -1,     4,   367,    -1,     4,   382,    -1,   469,   614,
     143,   652,    -1,   469,   467,   182,   614,    -1,   469,   471,
     216,   470,   449,    -1,   468,    -1,   703,    -1,   215,    -1,
     216,    -1,   217,    -1,   218,    -1,   219,    -1,   220,    -1,
     221,    -1,   222,    -1,   223,    -1,   224,    -1,   225,    -1,
     226,    -1,   227,    -1,   228,    -1,   229,    -1,   230,    -1,
     233,    -1,   234,    -1,   241,    -1,   242,    -1,   250,    -1,
     252,    -1,     7,    -1,     7,   215,    -1,   484,   448,    -1,
     614,   446,    -1,   614,    -1,   213,    -1,   211,    -1,    -1,
     691,     3,    -1,   691,    80,    -1,    -1,   476,    -1,   475,
     476,    -1,     4,   325,    -1,    52,   103,    -1,   164,   477,
      -1,     4,   347,    -1,     4,   263,    94,   648,    -1,    52,
     263,    94,    -1,    17,   262,    -1,    54,   262,    -1,   346,
      -1,   477,   272,   346,    -1,   698,   453,   479,   457,   448,
     480,   449,    -1,   454,    -1,    -1,   458,    -1,    -1,    52,
     482,    -1,    58,   687,    -1,    83,   691,    -1,   148,   694,
      -1,   180,   697,    -1,   184,   698,    -1,   195,   701,    -1,
      63,   688,    -1,    51,   686,    -1,    62,    68,   679,    -1,
     165,   652,    -1,   214,   695,    -1,    71,   690,    -1,   484,
      -1,   485,    -1,   488,    -1,   491,    -1,   489,   276,   486,
     277,    -1,   496,   276,   486,   277,   494,    -1,   487,    -1,
     486,   272,   487,    -1,   654,    -1,   654,   275,   654,    -1,
     489,    -1,   496,   494,    -1,   495,    -1,   500,    -1,   503,
      -1,   243,    -1,   490,    -1,   169,    -1,    41,    -1,   226,
      -1,   227,    -1,    85,    -1,    96,    -1,    19,   493,   492,
     494,    -1,    19,   273,   653,   274,    -1,    19,   273,   653,
     272,   649,   274,    -1,    19,   273,   272,   649,   274,    -1,
     162,   168,   653,    -1,    -1,   177,   649,    -1,   177,   680,
      -1,    -1,    23,   164,   681,    -1,    -1,   499,   273,   652,
     274,    -1,   499,    -1,   499,   193,   273,   652,   274,    -1,
     498,   273,   652,   274,    -1,   498,    -1,   497,   273,   652,
     274,    -1,   191,    -1,    23,   193,    -1,    91,   193,    -1,
      23,    -1,    91,    -1,   121,    -1,   119,    23,    -1,   119,
      91,    -1,    99,   501,    -1,   502,   501,    -1,    -1,   273,
     654,   274,    -1,   273,   654,   272,   654,   274,    -1,    44,
      -1,    92,    -1,    95,   504,    -1,    97,    95,   504,    -1,
     152,    -1,    93,   145,    -1,   273,   650,   274,    -1,    -1,
     518,    -1,   506,    -1,   535,    -1,   164,    71,   690,   182,
     654,    -1,   164,    71,   690,   182,   231,    -1,   164,    71,
     690,   182,   268,   231,    -1,   508,    -1,   509,    -1,   511,
      -1,   252,   702,    -1,   253,   252,   702,   510,    -1,   130,
      -1,    -1,   161,   515,   182,   512,   702,    -1,   252,    -1,
      -1,    28,   515,   516,    -1,   161,   515,    -1,   201,    -1,
      -1,   155,   517,    -1,    -1,   170,    -1,    -1,   164,   183,
     519,    -1,   520,    -1,    -1,   521,    -1,   520,   521,    -1,
     522,    -1,   523,    -1,   524,    -1,   528,    -1,   151,   130,
      -1,   151,   202,    -1,   196,    -1,   123,   196,    -1,    88,
     107,   525,    -1,   525,    -1,   526,    -1,   151,   185,   527,
      -1,   151,    29,   527,    -1,   170,    -1,   170,   180,    -1,
     170,   180,   174,    -1,   194,    -1,   123,   194,    -1,    -1,
     154,   531,    -1,   166,    -1,   149,    -1,    -1,   151,    -1,
     202,    -1,   532,    -1,   531,   272,   532,    -1,   534,   533,
      -1,    64,   529,   530,    -1,    -1,   574,    -1,   534,   272,
     574,    -1,   164,   176,    83,   691,    -1,   537,   538,   544,
     546,    -1,   547,    -1,   537,   186,   547,    -1,   537,   186,
       6,   547,    -1,   133,    20,   539,    -1,    -1,   540,    -1,
     539,   272,   540,    -1,   637,   541,   543,    -1,    11,    -1,
      48,    -1,    -1,   239,    -1,   249,    -1,   248,   551,   542,
     552,    -1,    -1,    64,   188,   545,    -1,    -1,   128,   609,
      -1,    -1,   200,   251,    -1,    -1,   163,   553,   556,   557,
     561,   580,   576,   579,   581,    -1,   163,   553,   556,   557,
     561,   580,   576,   579,   581,   538,    -1,    -1,    -1,    -1,
      -1,   554,   555,   550,    -1,   554,   550,    -1,   555,    -1,
      -1,   239,   655,   549,    -1,   239,   273,   637,   274,   549,
      -1,   239,   644,   549,    -1,   240,   655,    -1,   240,   273,
     550,   637,   274,    -1,   240,   644,    -1,    49,    -1,   677,
      -1,   558,    -1,   269,    -1,   559,    -1,   558,   272,   559,
      -1,   601,    -1,   601,   560,   692,    -1,    10,    -1,    -1,
      66,   562,    -1,   563,    -1,   562,   272,   563,    -1,   568,
      -1,   569,    -1,   564,    -1,   273,   536,   274,   560,   565,
     566,    -1,   696,    -1,    -1,   273,   567,   274,    -1,    -1,
     692,    -1,   567,   272,   692,    -1,   563,   575,    89,   563,
     129,   616,    -1,   273,   568,   274,    -1,   694,   570,   560,
     696,    -1,   694,   570,    -1,   273,   571,   274,    -1,    -1,
     572,    -1,   571,   272,   572,    -1,   678,    -1,   637,    -1,
     574,    -1,   697,   696,    -1,   697,    -1,    81,    -1,   105,
      -1,   105,   134,    -1,   159,    -1,   159,   134,    -1,    67,
      -1,    67,   134,    -1,    -1,    74,    20,   577,    -1,    -1,
     578,    -1,   577,   272,   578,    -1,   637,    -1,    77,   616,
      -1,    -1,   198,   616,    -1,    -1,   142,   582,    -1,    -1,
     583,   273,   584,   274,    -1,    89,    -1,   172,   114,    -1,
     114,    -1,   172,    -1,    -1,   585,    -1,   585,   272,   584,
      -1,   586,   587,    -1,   582,    -1,   697,    -1,   697,   586,
      -1,   120,    -1,    83,   273,   588,   274,    -1,   133,   691,
     589,    -1,   691,    -1,   691,   272,   588,    -1,    83,   273,
     588,   274,    -1,    -1,    84,    86,   574,   610,   190,   273,
     591,   274,    -1,    84,    86,   574,   610,   548,    -1,   601,
      -1,   591,   272,   601,    -1,   593,    -1,   594,    -1,    47,
      66,   573,   580,    -1,    47,    66,   573,   595,    -1,   198,
      38,   128,   685,    -1,   597,    -1,   598,    -1,   188,   573,
     164,   599,   580,    -1,   188,   573,   164,   599,   595,    -1,
     600,    -1,   599,   272,   600,    -1,   615,   264,   601,    -1,
     637,    -1,   678,    -1,   151,    19,   614,    66,   574,   603,
     605,    -1,    84,    19,   614,    86,   574,   603,   605,    -1,
      63,    66,   604,   182,   604,    -1,    63,   182,   604,    -1,
      -1,   493,    -1,   644,    -1,   649,    -1,   113,   606,    -1,
      -1,   653,    -1,   644,    -1,   608,    -1,    -1,   273,   609,
     274,    -1,   614,    -1,   609,   272,   614,    -1,   611,    -1,
      -1,   273,   612,   274,    -1,   615,    -1,   612,   272,   615,
      -1,   614,    -1,   696,   278,   683,    -1,   696,   278,   269,
      -1,   683,    -1,   614,    -1,   696,   278,   683,    -1,   631,
      -1,   124,   631,    -1,   618,    -1,   616,   132,   616,    -1,
     616,     8,   616,    -1,   618,    -1,   124,   631,    -1,   617,
     132,   616,    -1,   617,     8,   616,    -1,   632,   132,   616,
      -1,   632,     8,   616,    -1,   619,    -1,   273,   617,   274,
      -1,   124,   618,    -1,   620,    -1,   623,    -1,   624,    -1,
     625,    -1,   630,    -1,   621,    -1,   628,    -1,   626,    -1,
     627,    -1,   629,    -1,   637,   264,   637,    -1,   637,   265,
     637,    -1,   637,   266,   637,    -1,   637,    70,   637,    -1,
     637,   106,   637,    -1,   637,   125,   637,    -1,   637,   126,
     637,    -1,   637,   122,   637,    -1,   637,   264,     6,   273,
     635,   274,    -1,   637,   265,     6,   273,   635,   274,    -1,
     637,   266,     6,   273,   635,   274,    -1,   637,    70,     6,
     273,   635,   274,    -1,   637,   106,     6,   273,   635,   274,
      -1,   637,   125,     6,   273,   635,   274,    -1,   637,   126,
       6,   273,   635,   274,    -1,   637,   122,     6,   273,   635,
     274,    -1,   637,   264,   622,   273,   635,   274,    -1,   637,
     265,   622,   273,   635,   274,    -1,   637,   266,   622,   273,
     635,   274,    -1,   637,    70,   622,   273,   635,   274,    -1,
     637,   106,   622,   273,   635,   274,    -1,   637,   125,   622,
     273,   635,   274,    -1,   637,   126,   622,   273,   635,   274,
      -1,   637,   122,   622,   273,   635,   274,    -1,   171,    -1,
       9,    -1,   637,    18,   637,     8,   637,    -1,   637,   124,
      18,   637,     8,   637,    -1,   637,   108,   637,    -1,   637,
     124,   108,   637,    -1,   637,   108,   637,    57,   637,    -1,
     637,   124,   108,   637,    57,   637,    -1,   637,    79,   633,
      -1,   637,   124,    79,   633,    -1,   637,    34,   637,    -1,
     637,   124,    34,   637,    -1,   637,   175,   637,    -1,   637,
     124,   175,   637,    -1,   637,   175,   200,   637,    -1,   637,
     124,   175,   200,   637,    -1,    60,   273,   548,   274,    -1,
     167,   273,   548,   274,    -1,   637,    87,    98,    -1,   637,
      87,   124,    98,    -1,   256,    -1,   257,    -1,   258,    -1,
     259,    -1,   260,    -1,   261,    -1,   634,    -1,   273,   640,
     274,    -1,   273,   635,   274,    -1,   163,   553,   556,   637,
     561,   580,   576,   579,   581,   538,    -1,   163,   553,   556,
     637,   561,   580,   576,   579,   581,   538,    -1,   613,    -1,
     639,    -1,   656,    -1,   643,    -1,   644,    -1,   424,    -1,
     665,    -1,   666,    -1,   664,    -1,   268,   637,    -1,   267,
     637,    -1,   637,   267,   637,    -1,   637,    31,   637,    -1,
     637,    26,   682,    -1,   637,   268,   637,    -1,   637,   269,
     637,    -1,   637,   270,   637,    -1,   273,   637,   274,    -1,
     273,   636,   274,    -1,   645,    -1,   646,    -1,   647,    -1,
      42,    -1,   696,   278,    42,    -1,   101,    -1,   638,    -1,
     228,    -1,   229,    -1,   230,    -1,   613,   276,   640,   277,
      -1,   637,    -1,   640,   272,   637,    -1,   643,    -1,   268,
     642,    -1,   205,    -1,   204,    -1,   203,    -1,   231,    -1,
     232,    -1,   642,    -1,   648,    -1,    41,   207,    -1,   226,
     207,    -1,   227,   207,    -1,   279,    -1,   189,    -1,   233,
      -1,   234,    -1,   241,    -1,   242,    -1,    69,    -1,   173,
      -1,   250,    -1,   207,    -1,   208,   207,    -1,   650,    -1,
     268,   651,    -1,   204,    -1,   204,    -1,   650,    -1,   204,
      -1,   655,    -1,   268,   655,    -1,   204,    -1,   657,    -1,
     658,    -1,   659,    -1,   661,    -1,    35,   273,   269,   274,
      -1,    35,   273,   677,   637,   274,    -1,    35,   273,    49,
     637,   274,    -1,   179,   273,   677,   637,   274,    -1,   179,
     273,    49,   637,   274,    -1,    13,   273,   677,   637,   274,
      -1,    13,   273,    49,   637,   274,    -1,   116,   273,   677,
     637,   274,    -1,   116,   273,    49,   637,   274,    -1,   112,
     273,   677,   637,   274,    -1,   112,   273,    49,   637,   274,
      -1,    72,   273,   690,   272,   637,   274,    -1,   660,    -1,
     217,   273,   676,    66,   637,   274,    -1,   662,    -1,   100,
     273,   637,   274,    -1,   236,   273,   637,    66,   637,   663,
     274,    -1,    64,   637,    -1,    -1,   679,   273,   640,   274,
      -1,   679,   273,   274,    -1,    22,   273,   601,    10,   374,
     274,    -1,   667,    -1,   668,    -1,   245,   273,   637,   272,
     637,   274,    -1,   246,   273,   572,   272,   571,   274,    -1,
     669,    -1,   671,    -1,   244,   674,   670,    54,    -1,   244,
     674,   670,    53,   675,    54,    -1,   197,   673,   181,   675,
      -1,   670,   197,   673,   181,   675,    -1,   244,   672,    54,
      -1,   244,   672,    53,   675,    54,    -1,   197,   616,   181,
     675,    -1,   672,   197,   616,   181,   675,    -1,   637,    -1,
     637,    -1,   572,    -1,   218,    -1,   219,    -1,   220,    -1,
     221,    -1,   222,    -1,   223,    -1,   224,    -1,   225,    -1,
       6,    -1,    -1,    98,    -1,   206,    -1,   703,    -1,   703,
      -1,   703,    -1,   703,    -1,   703,    -1,   703,    -1,   703,
      -1,   703,    -1,   703,    -1,   703,    -1,   703,    -1,   703,
      -1,   703,    -1,   703,    -1,   703,    -1,   703,    -1,   703,
      -1,   703,    -1,   703,    -1,   703,    -1,   703,    -1,   703,
      -1,   703,    -1,   206,    -1,   704,    -1,   235,    -1,   238,
      -1,   236,    -1,   246,    -1,   249,    -1,   255,    -1,   251,
      -1,   245,    -1,   248,    -1,   254,    -1,   247,    -1,   256,
      -1,   257,    -1,   258,    -1,   262,    -1,   263,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   522,   522,   524,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   554,   558,   562,   566,   570,   575,   576,
     580,   582,   584,   588,   589,   593,   597,   599,   601,   603,
     605,   609,   612,   615,   618,   621,   628,   633,   638,   643,
     648,   653,   658,   664,   668,   669,   671,   673,   677,   679,
     681,   683,   687,   688,   695,   697,   699,   703,   704,   708,
     712,   713,   717,   719,   726,   730,   732,   737,   743,   744,
     746,   753,   754,   755,   759,   760,   766,   769,   775,   776,
     779,   782,   786,   790,   793,   798,   807,   811,   814,   817,
     819,   821,   823,   825,   827,   829,   831,   833,   838,   842,
     844,   846,   856,   860,   862,   873,   876,   879,   881,   882,
     888,   894,   896,   899,   903,   904,   909,   910,   915,   916,
     919,   920,   927,   945,   948,   951,   954,   958,   959,   962,
     963,   966,   971,   972,   975,   979,   988,   996,  1005,  1010,
    1011,  1014,  1021,  1022,  1025,  1026,  1030,  1032,  1034,  1036,
    1038,  1043,  1044,  1047,  1048,  1052,  1054,  1055,  1056,  1058,
    1062,  1064,  1066,  1068,  1072,  1077,  1084,  1090,  1097,  1104,
    1137,  1138,  1141,  1147,  1151,  1152,  1155,  1163,  1167,  1168,
    1171,  1172,  1175,  1177,  1181,  1182,  1183,  1186,  1187,  1188,
    1194,  1199,  1204,  1206,  1209,  1212,  1213,  1217,  1218,  1225,
    1229,  1233,  1242,  1248,  1249,  1252,  1254,  1259,  1262,  1266,
    1272,  1278,  1283,  1288,  1291,  1294,  1299,  1300,  1301,  1303,
    1305,  1310,  1311,  1314,  1315,  1319,  1324,  1326,  1331,  1332,
    1334,  1342,  1346,  1348,  1351,  1352,  1353,  1354,  1357,  1361,
    1365,  1372,  1380,  1384,  1391,  1393,  1395,  1397,  1400,  1403,
    1406,  1410,  1413,  1416,  1419,  1428,  1440,  1451,  1462,  1473,
    1476,  1479,  1482,  1485,  1486,  1490,  1496,  1499,  1502,  1503,
    1507,  1512,  1515,  1518,  1520,  1524,  1525,  1528,  1532,  1534,
    1537,  1540,  1541,  1545,  1546,  1547,  1548,  1549,  1550,  1551,
    1552,  1553,  1554,  1555,  1556,  1557,  1558,  1559,  1560,  1562,
    1567,  1570,  1572,  1576,  1580,  1585,  1589,  1589,  1591,  1596,
    1600,  1604,  1606,  1610,  1617,  1620,  1625,  1630,  1632,  1635,
    1638,  1640,  1643,  1646,  1647,  1648,  1650,  1654,  1658,  1661,
    1664,  1666,  1668,  1673,  1676,  1678,  1679,  1683,  1688,  1689,
    1693,  1695,  1697,  1699,  1706,  1714,  1721,  1741,  1745,  1747,
    1749,  1753,  1764,  1768,  1769,  1773,  1774,  1782,  1785,  1792,
    1796,  1802,  1806,  1811,  1816,  1823,  1834,  1845,  1847,  1850,
    1853,  1857,  1859,  1863,  1865,  1867,  1869,  1871,  1873,  1875,
    1877,  1879,  1881,  1883,  1885,  1887,  1889,  1891,  1895,  1898,
    1901,  1907,  1911,  1914,  1921,  1923,  1925,  1928,  1931,  1936,
    1940,  1946,  1947,  1951,  1960,  1964,  1966,  1968,  1970,  1972,
    1976,  1977,  1981,  1983,  1985,  1987,  1993,  1996,  1998,  2002,
    2010,  2011,  2012,  2013,  2014,  2015,  2016,  2017,  2018,  2019,
    2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,  2029,
    2030,  2031,  2032,  2035,  2037,  2041,  2043,  2048,  2054,  2056,
    2059,  2062,  2064,  2072,  2077,  2078,  2082,  2090,  2092,  2094,
    2096,  2098,  2100,  2102,  2106,  2107,  2114,  2124,  2126,  2129,
    2131,  2136,  2140,  2142,  2144,  2146,  2148,  2150,  2152,  2154,
    2156,  2158,  2160,  2162,  2169,  2170,  2173,  2174,  2177,  2182,
    2189,  2190,  2194,  2203,  2209,  2210,  2213,  2214,  2215,  2216,
    2233,  2238,  2243,  2262,  2279,  2286,  2287,  2294,  2298,  2304,
    2310,  2318,  2323,  2328,  2332,  2337,  2342,  2346,  2353,  2359,
    2365,  2373,  2378,  2383,  2390,  2391,  2392,  2395,  2396,  2399,
    2400,  2401,  2408,  2412,  2424,  2429,  2483,  2545,  2546,  2553,
    2566,  2571,  2576,  2583,  2586,  2592,  2593,  2594,  2598,  2603,
    2608,  2619,  2620,  2621,  2624,  2628,  2632,  2635,  2638,  2642,
    2643,  2646,  2650,  2654,  2655,  2658,  2661,  2664,  2666,  2669,
    2673,  2675,  2678,  2679,  2683,  2684,  2685,  2686,  2689,  2691,
    2695,  2697,  2701,  2703,  2706,  2708,  2710,  2714,  2716,  2718,
    2722,  2724,  2727,  2730,  2734,  2736,  2739,  2742,  2744,  2748,
    2749,  2753,  2757,  2760,  2763,  2764,  2769,  2777,  2781,  2783,
    2785,  2789,  2792,  2795,  2796,  2800,  2804,  2806,  2809,  2812,
    2814,  2818,  2821,  2824,  2827,  2830,  2833,  2836,  2839,  2845,
    2857,  2871,  2875,  2879,  2883,  2886,  2888,  2890,  2893,  2896,
    2898,  2900,  2904,  2906,  2908,  2912,  2914,  2918,  2920,  2924,
    2925,  2929,  2930,  2934,  2935,  2940,  2944,  2945,  2949,  2950,
    2951,  2956,  2960,  2962,  2965,  2968,  2971,  2972,  2976,  2978,
    2982,  2985,  2990,  2993,  2996,  2997,  3001,  3002,  3005,  3006,
    3011,  3016,  3018,  3020,  3022,  3024,  3026,  3028,  3031,  3037,
    3040,  3043,  3044,  3050,  3053,  3056,  3059,  3062,  3068,  3071,
    3074,  3078,  3080,  3082,  3088,  3091,  3094,  3095,  3099,  3101,
    3104,  3105,  3109,  3111,  3113,  3117,  3118,  3122,  3125,  3131,
    3134,  3138,  3139,  3146,  3147,  3150,  3154,  3158,  3165,  3166,
    3169,  3174,  3179,  3180,  3184,  3188,  3189,  3195,  3197,  3201,
    3203,  3205,  3208,  3209,  3212,  3216,  3218,  3221,  3223,  3229,
    3231,  3234,  3238,  3239,  3244,  3246,  3249,  3253,  3254,  3259,
    3260,  3263,  3268,  3273,  3276,  3283,  3284,  3286,  3287,  3289,
    3293,  3294,  3296,  3298,  3302,  3304,  3308,  3309,  3311,  3315,
    3316,  3317,  3318,  3319,  3320,  3321,  3322,  3323,  3324,  3329,
    3331,  3333,  3335,  3337,  3339,  3341,  3343,  3350,  3352,  3354,
    3356,  3358,  3360,  3362,  3364,  3366,  3368,  3370,  3372,  3374,
    3376,  3378,  3380,  3384,  3385,  3391,  3393,  3398,  3400,  3402,
    3404,  3409,  3411,  3415,  3417,  3421,  3423,  3425,  3427,  3431,
    3435,  3439,  3441,  3445,  3450,  3455,  3462,  3467,  3472,  3481,
    3482,  3486,  3490,  3503,  3520,  3521,  3522,  3523,  3524,  3525,
    3526,  3527,  3528,  3529,  3531,  3533,  3540,  3542,  3544,  3551,
    3558,  3565,  3567,  3569,  3570,  3571,  3572,  3574,  3576,  3580,
    3584,  3600,  3616,  3620,  3624,  3625,  3629,  3630,  3634,  3636,
    3638,  3640,  3642,  3646,  3647,  3649,  3665,  3681,  3685,  3689,
    3691,  3695,  3699,  3702,  3705,  3708,  3711,  3716,  3718,  3723,
    3724,  3728,  3735,  3742,  3749,  3756,  3757,  3761,  3767,  3768,
    3769,  3770,  3773,  3775,  3777,  3780,  3787,  3796,  3803,  3812,
    3814,  3816,  3818,  3824,  3833,  3836,  3840,  3841,  3845,  3854,
    3857,  3860,  3862,  3866,  3872,  3873,  3876,  3880,  3884,  3885,
    3888,  3890,  3894,  3896,  3900,  3902,  3906,  3908,  3912,  3915,
    3918,  3921,  3923,  3925,  3927,  3929,  3931,  3933,  3935,  3939,
    3940,  3943,  3951,  3954,  3957,  3960,  3963,  3966,  3969,  3972,
    3975,  3978,  3981,  3984,  3987,  3990,  3993,  3996,  3999,  4002,
    4005,  4008,  4011,  4014,  4017,  4020,  4025,  4026,  4032,  4033,
    4034,  4035,  4036,  4037,  4038,  4039,  4040,  4041,  4042,  4043,
    4044,  4045,  4047,  4048
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
  "OUTPUT_TYPE", "OVERFLOW", "PAGE", "PAGES", "PAGE_SIZE", "PARAMETER", 
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
  "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "';'", "','", "'('", 
  "')'", "':'", "'['", "']'", "'.'", "'?'", "$accept", "top", "statement", 
  "grant", "prot_table_name", "privileges", "privilege_list", 
  "proc_privileges", "privilege", "grant_option", "role_admin_option", 
  "simple_proc_name", "revoke", "rev_grant_option", "grantee_list", 
  "grantee", "user_grantee_list", "user_grantee", "role_name_list", 
  "role_name", "role_grantee_list", "role_grantee", "declare", 
  "declare_clause", "udf_decl_clause", "udf_data_type", "arg_desc_list1", 
  "arg_desc_list", "arg_desc", "return_value1", "return_value", 
  "filter_decl_clause", "create", "create_clause", "recreate", 
  "recreate_clause", "replace", "replace_clause", "unique_opt", 
  "index_definition", "shadow_clause", "manual_auto", "conditional", 
  "first_file_length", "sec_shadow_files", "db_file_list", 
  "domain_clause", "as_opt", "domain_default_opt", 
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
  "input_parameters", "output_parameters", "proc_parameters", 
  "proc_parameter", "var_declaration_list", "var_declarations", 
  "var_declaration", "var_decl_opt", "var_init_opt", "proc_block", 
  "full_proc_block", "full_proc_block_body", "proc_statements", 
  "proc_statement", "excp_statement", "raise_statement", "exec_procedure", 
  "exec_sql", "varstate", "for_select", "for_exec_into", "exec_into", 
  "if_then_else", "post_event", "event_argument_opt", "singleton_select", 
  "variable", "proc_inputs", "proc_outputs", "variable_list", "while", 
  "label_opt", "breakleave", "cursor_def", "excp_hndl_statements", 
  "excp_hndl_statement", "errors", "err", "invoke_procedure", 
  "view_clause", "rview_clause", "union_view", "union_view_expr", 
  "select_view_expr", "from_view_clause", "from_view_list", "view_table", 
  "joined_view_table", "begin_string", "end_string", "begin_trigger", 
  "end_trigger", "check_opt", "def_trigger_clause", 
  "replace_trigger_clause", "trigger_active", "trigger_type", 
  "trigger_type_prefix", "trigger_type_suffix", "trigger_position", 
  "trigger_action", "alter", "alter_clause", "domain_default_opt2", 
  "domain_check_constraint2", "alter_domain_ops", "alter_domain_op", 
  "alter_ops", "alter_op", "alter_column_name", "keyword_or_column", 
  "col_opt", "alter_data_type_or_domain", "alter_col_name", 
  "drop_behaviour", "alter_index_clause", "init_alter_db", "alter_db", 
  "db_alter_clause", "db_log_option_list", "alter_trigger_clause", 
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
  "order_direction", "nulls_placement", "nulls_clause", 
  "for_update_clause", "for_update_list", "lock_clause", "select_expr", 
  "ordered_select_expr", "begin_limit", "end_limit", "begin_first", 
  "end_first", "limit_clause", "first_clause", "skip_clause", 
  "distinct_clause", "select_list", "select_items", "select_item", 
  "as_noise", "from_clause", "from_list", "table_reference", 
  "derived_table", "correlation_name", "derived_column_list", 
  "alias_list", "joined_table", "table_proc", "proc_table_inputs", 
  "null_or_value_list", "null_or_value", "table_name", 
  "simple_table_name", "join_type", "group_clause", "group_by_list", 
  "group_by_item", "having_clause", "where_clause", "plan_clause", 
  "plan_expression", "plan_type", "plan_item_list", "plan_item", 
  "table_or_alias_list", "access_type", "index_list", "extra_indices_opt", 
  "insert", "insert_value_list", "delete", "delete_searched", 
  "delete_positioned", "cursor_clause", "update", "update_searched", 
  "update_positioned", "assignments", "assignment", "rhs", "blob", 
  "filter_clause", "blob_subtype_value", "segment_clause", 
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
     515,   516,   517,   518,    61,    60,    62,    43,    45,    42,
      47,    59,    44,    40,    41,    58,    91,    93,    46,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   280,   281,   281,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   283,   283,   283,   283,   283,   284,   284,
     285,   285,   285,   286,   286,   287,   288,   288,   288,   288,
     288,   289,   289,   290,   290,   291,   292,   292,   292,   292,
     292,   292,   292,   293,   294,   294,   294,   294,   295,   295,
     295,   295,   296,   296,   297,   297,   297,   298,   298,   299,
     300,   300,   301,   301,   302,   303,   303,   304,   305,   305,
     305,   306,   306,   306,   307,   307,   308,   308,   309,   309,
     310,   310,   310,   310,   310,   311,   312,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   314,   315,
     315,   315,   316,   317,   317,   318,   318,   319,   319,   319,
     320,   321,   321,   321,   322,   322,   323,   323,   324,   324,
     325,   325,   326,   327,   327,   328,   328,   329,   329,   330,
     330,   331,   332,   332,   333,   334,   335,   336,   337,   338,
     338,   339,   340,   340,   341,   341,   342,   342,   342,   342,
     342,   343,   343,   344,   344,   345,   345,   345,   345,   345,
     346,   346,   346,   346,   347,   347,   348,   348,   349,   350,
     351,   351,   352,   353,   354,   354,   355,   356,   357,   357,
     358,   358,   359,   359,   360,   360,   360,   361,   361,   361,
     362,   363,   364,   364,   364,   365,   365,   366,   366,   367,
     367,   367,   368,   369,   369,   370,   370,   371,   371,   372,
     373,   374,   375,   376,   376,   377,   377,   377,   377,   377,
     377,   378,   378,   379,   379,   380,   381,   381,   381,   381,
     381,   382,   383,   383,   384,   384,   384,   384,   385,   386,
     387,   388,   388,   389,   390,   390,   390,   390,   390,   391,
     392,   393,   393,   393,   393,   394,   395,   396,   397,   398,
     398,   399,   399,   400,   400,   401,   402,   402,   403,   403,
     404,   405,   405,   406,   406,   407,   407,   408,   409,   409,
     409,   410,   410,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   412,   412,   413,   414,   415,   416,   416,   417,   418,
     419,   420,   420,   421,   422,   423,   424,   425,   425,   425,
     426,   426,   426,   427,   427,   427,   427,   428,   429,   429,
     430,   430,   430,   431,   431,   432,   432,   433,   434,   434,
     435,   435,   435,   435,   436,   437,   438,   439,   440,   440,
     440,   441,   442,   443,   443,   444,   444,   445,   445,   446,
     447,   448,   449,   450,   450,   451,   452,   453,   453,   453,
     454,   455,   455,   456,   456,   456,   456,   456,   456,   456,
     456,   456,   456,   456,   456,   456,   456,   456,   457,   457,
     458,   459,   460,   460,   460,   460,   460,   460,   460,   461,
     462,   463,   463,   464,   464,   464,   464,   464,   464,   464,
     465,   465,   466,   466,   466,   466,   466,   466,   466,   467,
     468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   469,   469,   470,   470,   471,   472,   472,
     472,   473,   473,   474,   475,   475,   476,   476,   476,   476,
     476,   476,   476,   476,   477,   477,   478,   479,   479,   480,
     480,   481,   482,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   482,   482,   483,   483,   484,   484,   485,   485,
     486,   486,   487,   487,   488,   488,   489,   489,   489,   489,
     489,   489,   489,   489,   489,   490,   490,   491,   491,   491,
     491,   492,   492,   493,   493,   493,   494,   494,   495,   495,
     495,   496,   496,   496,   497,   497,   497,   498,   498,   499,
     499,   499,   500,   500,   501,   501,   501,   502,   502,   503,
     503,   503,   503,   504,   504,   505,   505,   505,   506,   506,
     506,   507,   507,   507,   508,   509,   510,   510,   511,   512,
     512,   513,   514,   515,   515,   516,   516,   517,   517,   518,
     519,   519,   520,   520,   521,   521,   521,   521,   522,   522,
     523,   523,   524,   524,   525,   525,   525,   526,   526,   526,
     527,   527,   527,   528,   529,   529,   529,   530,   530,   531,
     531,   532,   533,   533,   534,   534,   535,   536,   537,   537,
     537,   538,   538,   539,   539,   540,   541,   541,   541,   542,
     542,   543,   543,   544,   544,   545,   545,   546,   546,   547,
     548,   549,   550,   551,   552,   553,   553,   553,   553,   554,
     554,   554,   555,   555,   555,   556,   556,   557,   557,   558,
     558,   559,   559,   560,   560,   561,   562,   562,   563,   563,
     563,   564,   565,   565,   566,   566,   567,   567,   568,   568,
     569,   569,   570,   570,   571,   571,   572,   572,   573,   573,
     574,   575,   575,   575,   575,   575,   575,   575,   575,   576,
     576,   577,   577,   578,   579,   579,   580,   580,   581,   581,
     582,   583,   583,   583,   583,   583,   584,   584,   585,   585,
     586,   586,   587,   587,   587,   588,   588,   589,   589,   590,
     590,   591,   591,   592,   592,   593,   594,   595,   596,   596,
     597,   598,   599,   599,   600,   601,   601,   602,   602,   603,
     603,   603,   604,   604,   493,   605,   605,   606,   606,   607,
     607,   608,   609,   609,   610,   610,   611,   612,   612,   613,
     613,   613,   614,   615,   615,   616,   616,   616,   616,   616,
     617,   617,   617,   617,   617,   617,   618,   618,   618,   619,
     619,   619,   619,   619,   619,   619,   619,   619,   619,   620,
     620,   620,   620,   620,   620,   620,   620,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,   621,   621,   622,   622,   623,   623,   624,   624,   624,
     624,   625,   625,   626,   626,   627,   627,   627,   627,   628,
     629,   630,   630,   631,   631,   631,   632,   632,   632,   633,
     633,   634,   635,   636,   637,   637,   637,   637,   637,   637,
     637,   637,   637,   637,   637,   637,   637,   637,   637,   637,
     637,   637,   637,   637,   637,   637,   637,   637,   637,   637,
     638,   638,   638,   639,   640,   640,   641,   641,   642,   642,
     642,   642,   642,   643,   643,   643,   643,   643,   644,   645,
     645,   646,   647,   647,   647,   647,   647,   648,   648,   649,
     649,   650,   651,   652,   653,   654,   654,   655,   656,   656,
     656,   656,   657,   657,   657,   657,   657,   657,   657,   657,
     657,   657,   657,   658,   659,   660,   661,   661,   662,   663,
     663,   664,   664,   665,   666,   666,   667,   667,   668,   668,
     669,   669,   670,   670,   671,   671,   672,   672,   673,   674,
     675,   676,   676,   676,   676,   676,   676,   676,   676,   677,
     677,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   688,   689,   690,   691,   692,   693,   694,   695,   696,
     697,   698,   699,   700,   701,   702,   703,   703,   704,   704,
     704,   704,   704,   704,   704,   704,   704,   704,   704,   704,
     704,   704,   704,   704
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     7,     8,     6,     7,     5,     1,     2,
       1,     2,     1,     1,     3,     1,     1,     1,     1,     2,
       2,     3,     0,     3,     0,     1,     7,     8,     6,     7,
       6,     7,     4,     3,     1,     3,     3,     3,     2,     2,
       2,     2,     1,     3,     1,     2,     2,     1,     3,     1,
       1,     3,     1,     2,     2,     2,     3,     8,     1,     1,
       5,     0,     1,     3,     1,     3,     2,     4,     1,     3,
       2,     3,     4,     4,     2,     9,     2,     3,     7,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     4,     2,     2,     1,     0,     1,     1,     6,
       6,     1,     1,     0,     0,     1,     0,     4,     0,     1,
       1,     2,     8,     1,     0,     3,     0,     0,     1,     1,
       2,     1,     1,     1,     2,     6,     1,     1,     3,     0,
       1,     1,     0,     1,     1,     2,     3,     4,     2,     2,
       3,     0,     1,     1,     2,     1,     1,     1,     4,     3,
       3,     3,     3,     3,     1,     1,     6,     3,     1,     3,
       1,     3,     2,     1,     0,     2,     3,     1,     0,     1,
       1,     2,     3,     4,     0,     1,     2,     0,     1,     1,
       5,     5,     3,     2,     0,     1,     3,     1,     1,     6,
       3,     2,     6,     2,     1,     2,     2,     2,     0,     1,
       1,     2,     0,     2,     0,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     2,     2,     2,     5,     1,     2,
       3,     2,     2,     0,     1,     1,     1,     1,     3,     4,
       8,     4,     0,     6,     1,     1,     2,     2,     0,     3,
       3,     1,     2,     2,     2,     8,     8,     8,     8,     3,
       0,     2,     0,     1,     3,     2,     1,     0,     1,     2,
       6,     1,     0,     2,     1,     1,     1,     3,     1,     2,
       0,     1,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     2,     1,     1,     1,     2,     2,
       1,     3,     4,     2,     6,     4,     1,     1,     8,     9,
       6,     8,     6,     4,     0,     4,     2,     1,     3,     0,
       2,     4,     0,     1,     1,     3,     3,     7,     2,     0,
       2,     2,     3,     3,     0,     1,     2,     4,     1,     3,
       2,     2,     2,     1,     4,     7,     7,     1,     1,     3,
       4,     7,     2,     1,     3,     1,     1,     6,     3,     0,
       0,     0,     0,     3,     0,     9,     9,     1,     1,     0,
       2,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     5,     5,     5,     5,     5,     5,     2,     0,
       4,     2,     3,     3,     2,     2,     3,     3,     2,     3,
       6,     1,     2,     4,     3,     2,     2,     2,     2,     3,
       1,     3,     3,     3,     2,     2,     4,     4,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       0,     2,     2,     0,     1,     2,     2,     2,     2,     2,
       4,     3,     2,     2,     1,     3,     7,     1,     0,     1,
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     2,     2,     2,     1,     1,     1,     1,     4,     5,
       1,     3,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     6,
       5,     3,     0,     2,     2,     0,     3,     0,     4,     1,
       5,     4,     1,     4,     1,     2,     2,     1,     1,     1,
       2,     2,     2,     2,     0,     3,     5,     1,     1,     2,
       3,     1,     2,     3,     0,     1,     1,     1,     5,     5,
       6,     1,     1,     1,     2,     4,     1,     0,     5,     1,
       0,     3,     2,     1,     0,     2,     0,     1,     0,     3,
       1,     0,     1,     2,     1,     1,     1,     1,     2,     2,
       1,     2,     3,     1,     1,     3,     3,     1,     2,     3,
       1,     2,     0,     2,     1,     1,     0,     1,     1,     1,
       3,     2,     3,     0,     1,     3,     4,     4,     1,     3,
       4,     3,     0,     1,     3,     3,     1,     1,     0,     1,
       1,     4,     0,     3,     0,     2,     0,     2,     0,     9,
      10,     0,     0,     0,     0,     3,     2,     1,     0,     3,
       5,     3,     2,     5,     2,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     0,     2,     1,     3,     1,     1,
       1,     6,     1,     0,     3,     0,     1,     3,     6,     3,
       4,     2,     3,     0,     1,     3,     1,     1,     1,     2,
       1,     1,     1,     2,     1,     2,     1,     2,     0,     3,
       0,     1,     3,     1,     2,     0,     2,     0,     2,     0,
       4,     1,     2,     1,     1,     0,     1,     3,     2,     1,
       1,     2,     1,     4,     3,     1,     3,     4,     0,     8,
       5,     1,     3,     1,     1,     4,     4,     4,     1,     1,
       5,     5,     1,     3,     3,     1,     1,     7,     7,     5,
       3,     0,     1,     1,     1,     2,     0,     1,     1,     1,
       0,     3,     1,     3,     1,     0,     3,     1,     3,     1,
       3,     3,     1,     1,     3,     1,     2,     1,     3,     3,
       1,     2,     3,     3,     3,     3,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     1,     1,     5,     6,     3,     4,     5,
       6,     3,     4,     3,     4,     3,     4,     4,     5,     4,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     3,    10,    10,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     4,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     4,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     6,     1,     6,     1,     4,     7,     2,
       0,     4,     3,     6,     1,     1,     6,     6,     1,     1,
       4,     6,     4,     5,     3,     5,     4,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,   574,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   574,   648,     0,     0,     0,     0,     0,
       0,     2,    11,    16,     8,     7,    14,    15,    13,     4,
      10,    20,   556,    18,   561,   562,   563,     6,    17,   555,
     557,    19,   622,   618,    12,     9,   733,   734,    21,   738,
     739,     5,   463,     0,     0,     0,     0,     0,     0,   401,
     573,   576,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,     0,     0,    96,   628,   911,     0,    22,   909,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   481,     0,    30,    38,
      35,    37,   760,    36,   760,   996,   998,  1000,   999,  1005,
    1001,  1008,  1006,  1002,  1004,  1007,  1003,  1009,  1010,  1011,
    1012,  1013,     0,    32,     0,    33,     0,    67,    69,   988,
     997,     0,     0,     0,     0,     0,     0,     0,     0,   572,
       0,     0,   970,   642,   647,     0,     0,   581,     0,   688,
     690,   990,     0,     0,     0,   108,   564,   995,     0,     1,
       3,     0,     0,   634,     0,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,     0,   429,   430,
       0,   980,   408,     0,   984,   405,   270,   987,     0,   690,
     404,   379,   991,   578,   571,   907,     0,   104,   152,   151,
     105,   134,   219,   772,   976,     0,   103,   146,   983,     0,
      99,   270,   106,   913,   123,   100,   204,   101,     0,   102,
     760,   994,   107,   147,   626,   627,     0,   912,   910,     0,
      75,     0,   981,   707,   489,   979,   482,     0,   488,   493,
     483,   484,   491,   485,   486,   487,   492,   329,    31,     0,
      40,   759,    39,     0,     0,     0,     0,     0,     0,   765,
       0,     0,     0,     0,     0,     0,     0,   570,   917,     0,
     898,   641,   641,   642,   654,   652,   969,   655,     0,   656,
     646,   642,     0,     0,     0,     0,     0,     0,   597,   590,
     579,   580,   582,   584,   585,   586,   593,   594,   587,     0,
     689,   989,   109,   270,   110,   204,   111,   760,   567,     0,
       0,   619,     0,   638,     0,     0,     0,     0,     0,   406,
     464,   371,     0,   371,     0,   222,   407,   411,   402,   461,
     462,     0,   272,   243,   453,     0,   403,   420,     0,   377,
     378,   478,   577,   575,   908,   149,   149,     0,     0,     0,
     161,   153,   154,   133,     0,    97,     0,     0,   112,   272,
     122,   121,   124,     0,     0,     0,     0,     0,   972,    76,
     222,   525,     0,   735,   736,   490,     0,     0,     0,     0,
     876,   904,     0,   971,     0,   878,     0,     0,   905,     0,
     899,   890,   889,   888,   996,     0,     0,     0,   880,   881,
     882,   891,   892,   900,   901,  1000,   902,   903,     0,  1005,
    1001,   906,     0,     0,     0,     0,   859,   354,   327,   684,
     854,   769,   687,   879,   855,   893,   857,   858,   873,   874,
     875,   894,   856,   918,   919,   920,   934,   921,   936,   862,
     860,   861,   944,   945,   948,   949,   686,     0,     0,   976,
       0,   762,     0,     0,    28,    34,     0,     0,    44,    70,
      72,   992,    68,     0,     0,     0,   764,     0,    53,     0,
       0,     0,     0,    52,   569,     0,     0,     0,   651,   649,
       0,   658,     0,   657,   659,   664,   745,   746,   645,     0,
     616,     0,   591,   602,   588,   602,   589,   603,   609,   613,
     614,   598,   583,   707,   742,   773,     0,     0,   272,     0,
       0,   566,   565,   621,   623,   628,   620,   636,     0,   617,
     187,   178,     0,   466,   469,   175,   174,   130,     0,   472,
     467,     0,   473,   149,   149,   149,   149,   474,   468,   465,
       0,   415,     0,   417,   416,     0,   418,     0,   412,     0,
       0,   273,   220,   270,     0,     0,   424,     0,   425,   371,
     454,     0,   460,     0,     0,     0,   457,   976,   382,   381,
     477,     0,   399,   150,     0,     0,   159,     0,   158,     0,
       0,   148,   162,   163,   167,   166,   165,   155,   525,   537,
     512,   547,   515,   538,   548,     0,   554,   516,     0,   544,
       0,   539,   551,   511,   534,   513,   514,   509,   371,   494,
     495,   496,   504,   510,   497,   506,   527,     0,   532,   529,
     507,   544,   508,   113,   270,   114,     0,     0,   125,     0,
       0,   203,   243,   379,   369,     0,   222,     0,    82,    84,
       0,     0,     0,   754,     0,     0,     0,     0,  1009,  1010,
    1011,     0,   706,   777,   786,   789,   794,   790,   791,   792,
     796,   797,   795,   798,   793,   775,     0,   970,     0,   970,
     895,     0,     0,   970,   970,   970,     0,   896,   897,     0,
       0,   959,     0,     0,     0,     0,   864,   863,   648,     0,
       0,   687,   326,   993,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   761,    29,     0,     0,    45,
      73,     0,     0,    27,   751,     0,   767,   648,     0,   730,
     751,     0,     0,     0,     0,   568,     0,   641,     0,     0,
     707,     0,   663,     0,   559,     0,   558,   915,     0,   592,
       0,   600,   596,   595,     0,   606,     0,   611,   599,     0,
     740,   741,     0,     0,     0,   243,   369,     0,   632,     0,
     633,   637,     0,     0,     0,   131,   188,   471,     0,     0,
       0,     0,     0,   371,   414,     0,   371,   372,   419,   504,
     527,   275,     0,   269,   271,   369,   242,   977,   214,   211,
       0,   224,   371,   494,   369,     0,     0,     0,   241,   244,
     245,   246,   247,     0,   423,   459,   458,   422,   421,     0,
       0,     0,   385,   383,   384,   380,     0,   371,   197,   156,
     160,     0,     0,   164,     0,   522,   535,   536,   552,     0,
     549,   554,     0,   542,   540,   541,   136,     0,     0,     0,
     505,     0,     0,     0,     0,   543,   272,     0,   369,   126,
     202,     0,   205,   207,   208,     0,     0,     0,     0,   222,
     222,    79,     0,    86,    78,   523,   524,   973,   525,     0,
       0,     0,   788,   776,     0,     0,   846,   847,   848,     0,
     780,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   961,   962,   963,   964,   965,   966,
     967,   968,     0,     0,     0,     0,   954,     0,     0,     0,
       0,     0,   970,   328,   872,   871,   685,   884,     0,   867,
     975,   866,   865,   868,   869,   870,   942,     0,   877,   771,
     770,   763,     0,     0,     0,     0,     0,     0,    25,    54,
      42,    62,    64,     0,     0,    71,     0,   756,     0,   766,
     970,     0,   756,    50,    48,     0,     0,     0,   650,   653,
       0,   665,   666,   670,   668,   669,   683,     0,   700,   660,
     662,   985,   560,   916,   601,   610,   605,   604,     0,   615,
     743,   744,   774,   369,     0,     0,   624,   643,   625,   635,
     177,   184,   183,     0,   180,   470,   149,   194,   179,   189,
     190,   171,   170,   914,   173,   172,   475,     0,     0,     0,
     413,   274,   277,   213,   371,     0,   372,   215,   210,   216,
       0,     0,   252,     0,   427,   372,   371,   369,   426,     0,
       0,     0,   398,   480,   198,   199,   157,     0,   169,     0,
       0,     0,   527,     0,   550,     0,     0,   371,   372,     0,
     500,   502,     0,     0,     0,     0,     0,     0,     0,   379,
     277,   149,   128,   243,   200,   399,   970,   374,   357,   358,
       0,    83,     0,   222,     0,    88,     0,    85,     0,     0,
       0,   737,   978,     0,     0,   781,     0,     0,   787,     0,
       0,   779,   778,     0,   833,     0,   824,   823,     0,   802,
       0,   831,   849,   841,     0,     0,     0,   803,   827,     0,
       0,   806,     0,     0,     0,     0,     0,     0,     0,   804,
       0,     0,   805,     0,   835,     0,     0,   799,     0,     0,
     800,     0,     0,   801,     0,     0,   222,     0,   922,     0,
       0,   937,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   960,     0,     0,   958,     0,     0,   950,     0,     0,
       0,     0,     0,   883,   941,    66,    58,    59,    65,    60,
      61,     0,     0,     0,    23,    26,    42,    43,   525,   525,
       0,   748,   768,     0,     0,   731,   747,    51,    49,    46,
       0,     0,   698,   668,     0,   696,   691,   692,   694,     0,
       0,   681,     0,   705,   607,   608,   612,   277,   201,   374,
       0,   182,     0,     0,     0,   195,     0,   191,     0,     0,
       0,   409,   230,   225,   886,   226,   227,   228,   229,   282,
       0,   276,   278,     0,   223,   231,     0,   252,   628,   248,
       0,   428,   455,   456,   390,   391,   387,   386,   389,   388,
     371,   479,   372,   168,   974,     0,     0,   518,     0,   517,
     553,     0,   545,     0,   137,     0,   498,     0,   526,   527,
     533,   531,     0,   528,   369,     0,     0,     0,   120,   129,
     206,   371,     0,     0,   370,     0,    98,     0,   118,   117,
      94,     0,     0,    90,     0,    87,     0,   839,   840,   783,
     782,   785,   784,     0,     0,     0,   648,     0,     0,   842,
       0,     0,     0,     0,     0,     0,   834,   832,   828,     0,
     836,     0,     0,     0,     0,   837,     0,     0,     0,     0,
       0,     0,   928,   927,     0,     0,   924,   923,     0,   932,
     931,   930,   929,   926,   925,     0,   940,   956,   955,     0,
       0,     0,     0,     0,     0,     0,   885,    55,    56,     0,
      57,    63,    24,   752,     0,   753,   750,   755,   758,   757,
       0,     0,   729,     0,    47,   664,   679,   667,   697,   693,
     695,     0,     0,     0,     0,     0,   709,     0,   370,   629,
     630,   644,   149,   185,   181,     0,   197,   196,   192,     0,
     372,   887,   281,     0,   339,   372,   279,   372,   218,   232,
     233,   371,     0,   249,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   476,   520,     0,   521,     0,   135,     0,
     218,   138,   139,   141,   142,   143,   501,   503,   499,   530,
     277,   399,   372,   197,     0,     0,     0,   355,     0,   359,
     371,    89,     0,     0,    91,   527,     0,   825,     0,     0,
     970,   851,   850,     0,     0,   829,     0,     0,     0,     0,
     838,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   943,   221,   933,   935,     0,     0,   957,   952,   951,
       0,   946,   947,   707,    41,   525,   707,   732,   673,   698,
     682,   680,   699,   701,   703,   704,   715,   639,   372,   356,
     631,     0,   176,   193,   372,   145,     0,     0,     0,     0,
       0,     0,     0,     0,   998,  1003,   291,   286,     0,   339,
     285,   295,   296,   297,   298,   299,   306,   307,   300,   302,
     303,   305,     0,   310,     0,     0,     0,     0,     0,     0,
     976,   268,     0,     0,   209,   234,     0,     0,     0,   252,
     235,   238,   760,     0,   372,   396,   397,   393,   392,   395,
     394,     0,   519,   546,   144,   132,   140,     0,   371,   265,
     127,   372,     0,   707,   373,   360,     0,     0,    92,    93,
      80,     0,   810,   818,     0,   811,   819,   814,   822,   826,
     830,   812,   820,   813,   821,   807,   815,   808,   816,   809,
     817,   939,   938,   953,   700,   749,   700,   675,   672,     0,
       0,   711,   713,   714,   708,     0,   266,   186,   410,   224,
     313,     0,     0,   316,   317,     0,   309,     0,   324,   308,
     340,   341,     0,   986,   287,     0,   292,   289,   345,     0,
       0,     0,   301,   294,   304,   293,   338,   212,   217,   236,
     252,   760,   239,   258,   251,   253,   400,   372,     0,   375,
       0,   362,   363,   365,   366,   700,   372,    77,    95,     0,
     705,   705,     0,   671,   678,   702,   712,   715,     0,   284,
       0,   311,     0,   329,     0,     0,     0,   342,   353,     0,
       0,     0,     0,   348,   346,     0,     0,     0,   333,     0,
     334,     0,   240,   258,     0,   252,   254,   255,   267,   372,
     698,   365,     0,     0,   705,     0,   707,   709,   709,     0,
     676,   719,     0,   716,     0,   720,   283,   280,   312,   332,
       0,   315,     0,   323,   352,   351,   982,   350,   339,     0,
       0,     0,     0,   325,     0,     0,   252,     0,     0,   250,
       0,   257,     0,   256,   376,   368,   364,     0,   361,   119,
     700,   622,   622,     0,   674,   710,   715,     0,   722,     0,
     718,   721,     0,     0,     0,   339,   347,   349,     0,   344,
       0,   336,   335,   237,     0,     0,   261,   260,   259,   698,
     705,   853,   640,   677,   717,     0,   728,     0,   330,   314,
     320,   322,     0,     0,     0,   339,   264,   262,   263,     0,
     709,     0,   725,     0,   724,     0,   339,     0,     0,   339,
     337,   367,   622,   723,     0,     0,   331,   321,   339,   343,
     318,   852,   726,     0,   319,   727
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    20,    21,    22,   463,   122,   123,   124,   125,  1204,
     723,   718,    23,   137,   968,   969,   970,   971,   126,   127,
     468,   469,    24,    82,   379,   873,   647,   648,   649,  1104,
    1105,   240,    25,    74,    26,   155,    27,   368,    75,  1316,
     222,   372,   639,  1092,  1308,   533,   210,   364,  1078,  1460,
    1461,  1462,  1463,  1464,  1465,   216,   232,   207,   584,   208,
     360,   361,   362,   591,   592,   593,   594,   595,   535,   536,
     537,  1023,  1020,  1021,  1241,  1423,   538,  1028,  1029,  1030,
    1246,  1066,   225,   314,   374,   861,   862,   863,   799,   800,
     801,  1584,   567,   559,  1364,   650,  1046,  1251,  1438,  1439,
    1440,  1590,   864,   569,   808,   809,   810,   811,  1269,   812,
    1745,  1746,  1747,  1827,   220,   312,   633,   195,   342,   564,
     560,   561,  1260,  1261,  1262,  1433,  1720,  1556,  1557,  1558,
    1559,  1560,  1561,  1562,  1563,  1564,  1665,  1565,  1566,  1567,
    1568,  1569,  1726,  1570,   426,   427,  1813,  1739,  1571,  1572,
    1573,  1844,  1677,  1678,  1732,  1733,    28,   229,   316,  1097,
    1098,  1099,  1613,  1701,  1702,  1703,   866,  1477,   552,  1040,
    1314,   227,   635,   351,   580,   581,   825,   827,  1281,    29,
      59,   787,   784,   336,   337,   346,   347,   187,   188,   348,
    1055,   575,   817,   192,   164,   329,   330,   548,   200,   582,
    1282,    30,    96,   618,   619,   620,  1079,  1080,   621,   789,
     623,   624,  1072,  1393,   850,   625,   790,   627,   628,   629,
     630,   843,   631,   632,   840,    31,    32,    33,    34,    35,
     522,    36,   485,    37,    38,    61,   204,   353,    39,   300,
     301,   302,   303,   304,   305,   306,   307,   752,   308,  1008,
    1236,   507,   508,   757,   509,    40,  1574,    42,   163,   523,
     524,   236,  1421,  1018,   323,   770,   529,    43,   729,   488,
     290,  1240,  1540,   142,   143,   144,   288,   492,   493,   494,
     743,   740,   991,   992,   993,  1647,  1713,  1759,   994,   995,
    1231,   428,  1181,  1704,   149,  1229,  1233,  1532,  1533,  1416,
     383,  1537,  1761,  1655,  1762,  1763,  1764,  1810,  1851,  1854,
    1575,  1214,  1576,    46,    47,   384,  1577,    49,    50,   513,
    1578,   495,    51,   977,  1394,  1211,  1397,   260,   261,   460,
     475,   476,   725,   430,   431,   516,   662,   889,   663,   664,
     665,   666,  1128,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   891,  1131,  1132,  1337,   700,   676,   433,   434,
     948,  1253,   435,   436,   437,   438,   439,   440,   441,   653,
      79,   238,   224,  1034,  1081,   747,   442,   443,   444,   445,
     446,   447,   448,  1516,   449,   450,   451,   452,   453,   454,
     939,   455,   692,  1185,   693,  1182,   932,   289,   456,   457,
     876,  1283,   949,   213,   796,  1111,   244,   190,   241,  1775,
     217,  1852,  1000,  1579,   996,   128,   458,   199,   201,   972,
     702,   230,   156,   459,   130
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1517
static const short yypact[] =
{
    2207,  1287,   448,  3667,   431,   885,   625,  1759,   646,  1910,
     203,   754,  1359,   448,   626,   476,  6164,   681,  6164,   551,
     824,   557, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517,    86, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517,  6405,  6164,  6164,  6164,  6164,  6164, -1517,
   -1517,   686,   766,  6164,  6164,  6164,   876,  6164,   698,  6164,
    6164, -1517,  6164,  6164, -1517,   685, -1517,   709, -1517, -1517,
     857,  6164, -1517,  6164,  6164,  6164,   866,  6164,  6164,  6164,
    6164,   698,  6164,  6164,  6164,  6164, -1517,  6164,   793, -1517,
   -1517, -1517,   679, -1517,   679, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517,   831,   695,   849, -1517,    39, -1517, -1517, -1517,
   -1517,  6164,  6164,  6164,   856,   869,   888,   444,   110,   813,
      13,   391,   228,   784, -1517,  6164,   938,   588,   863, -1517,
    6164, -1517,  6164,  6164,  6164, -1517, -1517, -1517,  6164, -1517,
   -1517,  1014,   207,   966,   496, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517,   196, -1517, -1517,
     766, -1517, -1517,   217, -1517, -1517,   783, -1517,   810, -1517,
   -1517,   475, -1517,   880, -1517, -1517,   832, -1517,   762, -1517,
   -1517,  1036, -1517, -1517, -1517,   766, -1517, -1517, -1517,   566,
   -1517,   783, -1517, -1517,   188, -1517,   999, -1517,  1004, -1517,
     679, -1517, -1517, -1517, -1517, -1517,   995, -1517, -1517,   875,
   -1517,  1003, -1517,   898, -1517, -1517, -1517,   875, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517,  4687, -1517,  6164,
   -1517, -1517, -1517,  4190,   430,   952,  4265,  6164,  1024,   847,
    1058,  1064,  4190,   982,  1009,  1012,  4265,   890, -1517,  6007,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,  4360, -1517,
   -1517, -1517,   961,  6164,  1037,   951,   281,  6164,   970, -1517,
   -1517,   588, -1517, -1517, -1517, -1517, -1517, -1517, -1517,  6164,
   -1517, -1517, -1517,   783, -1517,   999, -1517,   679,  1015,  6007,
     988, -1517,   967,   954,   135,   894,    82,   895,   541,   496,
   -1517,  1127,   219, -1517,  6164, -1517,   196, -1517, -1517, -1517,
   -1517,  6164,  1011,  2242,   955,  2797,   903, -1517,  6405, -1517,
   -1517,   790, -1517, -1517, -1517,   916,   916,   766,  1068,   766,
    1363,   762, -1517, -1517,  3290, -1517,  6164,  6164, -1517,  1011,
   -1517, -1517,  1155,   371,   915,  6164,  1179,  6164, -1517, -1517,
      44,   377,  3706, -1517, -1517, -1517,   922,   923,   924,  1006,
   -1517, -1517,   926, -1517,   929, -1517,   947,   948, -1517,   957,
   -1517, -1517, -1517, -1517,   968,   969,  1041,  1042, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517,   983, -1517, -1517,  4767,   990,
     996, -1517,  6007,  6007,  4457,  6164, -1517, -1517,   950, -1517,
     974, -1517,  1226, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517,  1000,   997,  1007,
     345, -1517,  6164,  1092, -1517, -1517,  6164,  6164,   280, -1517,
   -1517, -1517, -1517,  6164,  6164,   569, -1517,  6164, -1517,  1216,
    6164,  4190,  1139,  1016, -1517,  6164,  4997,   318, -1517, -1517,
    6007, -1517,  1225,  1020, -1517,   162,  1226, -1517, -1517,   489,
   -1517,   507, -1517,   400, -1517,   400, -1517,  1022, -1517,   107,
   -1517,  1124, -1517,   343, -1517, -1517,  1035,  1032,  1011,  1027,
    1301, -1517, -1517,  1046, -1517,   360, -1517,  1189,  1069, -1517,
   -1517,   136,  1227,  1228, -1517, -1517, -1517, -1517,   766, -1517,
   -1517,  1229, -1517,   916,   916,   916,   916, -1517,  1052, -1517,
    1302, -1517,  1305, -1517, -1517,  1279, -1517,  3290, -1517,  3290,
     585, -1517, -1517,   783,  1325,  6164, -1517,  6262, -1517,   340,
   -1517,  6164,   720,   810,  1154,  1121,  1196,  1160, -1517, -1517,
   -1517,   355,  1198, -1517,  1143,   698, -1517,   766, -1517,  1330,
    1254, -1517,  1363, -1517, -1517, -1517, -1517, -1517,   422,  1161,
   -1517, -1517, -1517,  1162, -1517,  1211,  1085, -1517,  1262,  1087,
     425, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517,  1086, -1517, -1517, -1517,   166,  1088,  1090,    17,
   -1517,  1087, -1517, -1517,   783, -1517,  1300,  1358, -1517,   766,
     766, -1517,  2242,   475, -1517,  1242, -1517,  1215,  1105, -1517,
    3353,  5815,  1243, -1517,  1251,  1108,  3803,  1109,   993,  1072,
    1095,  3454,   201, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517,  2661,   645,  5077,   141,
   -1517,  6164,  6007,   661,   738,   776,  1084, -1517, -1517,  6007,
    4033,  1226,   191,  1187,  6007,  5077,   168,   168,   626,   687,
    1111,   370, -1517, -1517,  5077,  6007,  6164,  6007,  6007,  6007,
    6007,  6007,  5307,  2273,  6164, -1517, -1517,  4879,  1204, -1517,
   -1517,  1177,  4265, -1517,  1327,   691, -1517,   626,  1118, -1517,
    1327,  4879,  1329,  1331,  6164, -1517,   370, -1517,   415,  3931,
    1200,  5077, -1517,  6164, -1517,   604, -1517, -1517,   214, -1517,
    1202, -1517, -1517, -1517,  6164,   620,  6164, -1517, -1517,  6164,
   -1517, -1517,  5077,  6164,  1390,  2242, -1517,  6007,  1153,  6164,
   -1517, -1517,   766,   766,   766, -1517,   480, -1517,  1143,  1143,
    1206,   698,   541, -1517, -1517,  1135, -1517, -1517, -1517, -1517,
    1388, -1517,  6164, -1517, -1517, -1517, -1517, -1517,  1393, -1517,
    1144,  1377, -1517,  1400, -1517,  1341,  1344,   679, -1517, -1517,
   -1517, -1517, -1517,  1412, -1517, -1517, -1517, -1517, -1517,  6164,
    6352,   698,  1313,  1314,  1315, -1517,   698, -1517,   872, -1517,
   -1517,  1273,   766, -1517,   265,  1286, -1517, -1517, -1517,   698,
   -1517,  1085,   461, -1517, -1517, -1517,  1403,   461,  1288,   461,
   -1517,   698,   698,  1180,   698, -1517,  1011,  6164, -1517,  1348,
   -1517,   696, -1517, -1517, -1517,   790,  1291,  6164,   716,   172,
   -1517, -1517,  1182,  1436, -1517, -1517, -1517, -1517,   377,  6164,
    1296,  4130, -1517, -1517,  1296,  3803, -1517, -1517, -1517,   132,
   -1517,   251,  1381,  4033,  4033,  6007,  6007,  1803,  1188,   637,
    2078,  6007,  2370,   578,  2635,  2733,  5387,  2996,  3094,  3357,
    6007,  6007,  1455,  6007,  1195,  6007,  1201,   582,  6007,  6007,
    6007,  6007,  6007,  6007, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517,  1404,  1082,   173,  5077, -1517,  4033,  6007,   266,
     252,  1203,   228, -1517, -1517, -1517, -1517,  1226,   -16, -1517,
   -1517,  1448,   168,   168,   801,   801, -1517,   728, -1517, -1517,
   -1517, -1517,  6164,  6164,  6164,  6164,  6164,  6164,  1205, -1517,
     333, -1517, -1517,  4879,  1345, -1517,   142,  1365,  6164, -1517,
     228,  5077,  1365,  1205,  1208,  4879,  2896,  1415, -1517, -1517,
    3863,  1212,   611, -1517, -1517, -1517,  1209,  4033,  1409, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,   330, -1517,
   -1517, -1517, -1517, -1517,   741,  1291, -1517, -1517, -1517,  1213,
   -1517, -1517, -1517,   742, -1517, -1517,   916,  1474, -1517,   480,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517,  1219,  4033,  2196,
   -1517, -1517,  1443, -1517, -1517,  2196, -1517, -1517, -1517, -1517,
     679,   679,  1250,  1231, -1517, -1517, -1517, -1517, -1517,   139,
     226,   190, -1517,  1488, -1517, -1517, -1517,  6164, -1517,   431,
     759,  1332,  1388,  1234, -1517,  1143,   763, -1517, -1517,   647,
   -1517,  1235,  6164,   664,  1237,  1239,   698,  1240,  1491,   475,
    1443,   916,  1228,  2242, -1517,  1198,   228,  1309,  1316, -1517,
    1879, -1517,   698,  1375,  1461, -1517,  3353, -1517,   698,  1282,
    1462, -1517, -1517,  1247,  1249, -1517,  4033,  4033, -1517,  4033,
    4033, -1517,  1516,   160,  1226,  1252, -1517, -1517,  1255,  1226,
    5617, -1517, -1517, -1517,  1429,  1256,  1258,  1226,   940,  1259,
    1260,  1226,  6007,  6007,  1188,  6007,  5697,  1261,  1264,  1226,
    1266,  1267,  1226,  6007,  1226,  1268,  1270,  1226,  1271,  1272,
    1226,  1275,  1276,  1226,   630,   640, -1517,   648, -1517,   675,
    6007, -1517,   715,   737,   795,   897,   949,   965,  6007,  6007,
    5077, -1517,  1481,   245,  1226,  1371,  5077, -1517,  6007,  6007,
    5077,  6007,  6007, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517,  4879,  1480,  4879, -1517,  1205,   333, -1517,   332,   332,
     297, -1517, -1517,  4360,   768, -1517, -1517,  1205,  1208,  1283,
    2896,  1280,   659,  1284,  3931,  1423, -1517,  1425,  1426,  1472,
    5077,   633,  1542,  1486, -1517, -1517, -1517,  1443, -1517,  1309,
     597,  1396,   766,  1430,  1143,  1431,  1143, -1517,  4033,   144,
     689, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,  1378,
    1550,  1443, -1517,  6007, -1517,   556,  1416,  1250,   685, -1517,
    4033, -1517, -1517, -1517,  1439,  1440,  1441,  1444,  1445,  1446,
   -1517, -1517, -1517, -1517, -1517,  1306,   431, -1517,  1206, -1517,
   -1517,   461, -1517,  2196,   449,   461, -1517,   461, -1517,  1388,
   -1517, -1517,  1308, -1517, -1517,   790,  1550,  1143, -1517,  1228,
   -1517, -1517,  4360,  1551, -1517,   234, -1517,  1310, -1517,  1213,
   -1517,  1312,   766,   157,  1317, -1517,   766, -1517, -1517, -1517,
    1516, -1517,  1516,  6007,  1424,  1424,   626,  1318,   773, -1517,
    1424,  1424,  6007,  1424,  1424,   354,  1226, -1517,  1106,  6007,
    1226,  1424,  1424,  1424,  1424,  1226,  1424,  1424,  1424,  1424,
    1424,  1424, -1517, -1517,  1319,  3290, -1517, -1517,   977, -1517,
   -1517, -1517, -1517, -1517, -1517,   992,  1172, -1517, -1517,  5077,
    5077,  1534,  1408,  1010,   777,  1159,  1226, -1517, -1517,  1459,
   -1517, -1517, -1517, -1517,  1414, -1517, -1517, -1517, -1517, -1517,
    1225,  5077, -1517,  2896,  1283,  1589, -1517,   611, -1517, -1517,
   -1517,  3931,   781,  6164,  6007,  4033,  1458,  1550, -1517, -1517,
   -1517, -1517,   916, -1517, -1517,   766,   872, -1517, -1517,   147,
   -1517, -1517, -1517,  6164,  4595, -1517, -1517,  1226,  1575,   556,
   -1517,   666,  6164, -1517,  1526,   148,  1432,  1527,  1435,  1565,
    1535,  1571,  1443, -1517, -1517,  1350, -1517,  1353, -1517,  1530,
    1575,   449, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
    1443,  1198, -1517,   872,  1488,  1563,  1499, -1517,  1291, -1517,
   -1517, -1517,  1515,    77, -1517,  1388,  1517,  1226,  1361,  1364,
     228, -1517, -1517,  1366,  1367,  1226,  1368,  1369,  6007,  6007,
    1226,  1370,  1380,  1382,  1383,  1384,  1385,  1386,  1387,  1389,
    1391, -1517, -1517, -1517, -1517,  6007,  1392, -1517, -1517, -1517,
    5077, -1517, -1517,  1200, -1517,   332,  1200, -1517,  6164,   708,
   -1517, -1517,  1397, -1517,  1226,   201,   403, -1517, -1517, -1517,
   -1517,  1143, -1517, -1517, -1517, -1517,  3290,  5505,   118,  1401,
    1395,  1576,  6007,  1405,  1406,  5755, -1517, -1517,  1583,  1635,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517,   131, -1517,  1578,  1407,  1413,  1417,  1427,  1399,
     285, -1517,  1411,  6164, -1517, -1517,  1573,  1585,  6164,  1250,
   -1517, -1517,   679,  6164, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517,  1550, -1517, -1517, -1517, -1517, -1517,  1550, -1517, -1517,
   -1517, -1517,  5458,  1200, -1517, -1517,  6007,   766, -1517, -1517,
   -1517,   766, -1517, -1517,  6007, -1517, -1517, -1517, -1517,  1226,
    1226, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517,  1226, -1517, -1517,  1409, -1517,  1409,  1410, -1517,  4033,
    6007, -1517, -1517,  1566, -1517,  1422, -1517, -1517, -1517,   117,
   -1517,  5927,  6164, -1517, -1517,  6007, -1517,  4033,  1226, -1517,
   -1517, -1517,  1428, -1517, -1517,   227, -1517,  1482, -1517,   257,
    1433,  5135, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
    1250,   679, -1517,  1557, -1517, -1517, -1517, -1517,  1488, -1517,
    5458,  1437,   611, -1517, -1517,  1409,  1226, -1517, -1517,  1159,
    1486,  1486,  6164, -1517,   201, -1517, -1517,  3534,  2196, -1517,
    1434, -1517,  1063,  4687,  1045,   149,  1453, -1517, -1517,  6164,
    6164,   431,   112, -1517, -1517,   348,  1601,  4033, -1517,   844,
   -1517,  1438, -1517,  1557,   244,  1250,  1581,  1586, -1517, -1517,
     659,  1451,  5458,  1613,  1486,  1452,  1200,  1458,  1458,   803,
   -1517, -1517,  1456,  1460,   629,  6164, -1517, -1517, -1517,  1548,
    5135, -1517,  1531, -1517, -1517, -1517, -1517, -1517,  6229,   227,
    6007,  5135,   150, -1517,  5135,  6073,  1250,   366,   366, -1517,
    1670, -1517,  1539, -1517, -1517, -1517,   611,  5458, -1517, -1517,
    1409,  1598,  1598,  6164, -1517, -1517,  3534,  1463, -1517,  6164,
   -1517, -1517,  5189,  1467,   846,  6229, -1517, -1517,  1152,   159,
    1683, -1517, -1517, -1517,  1532,   205, -1517, -1517, -1517,   726,
    1486, -1517, -1517, -1517, -1517,  6164,  1656,  5135,  1468, -1517,
   -1517,  1693,  5135,  1714,  1704,  6229, -1517, -1517, -1517,  4033,
    1458,  1483,  1487,  1489, -1517,   818,  6229,   120,  6164,  6229,
   -1517,   201,  1598, -1517,  6164,  6164, -1517, -1517,  6229, -1517,
   -1517, -1517, -1517,  1490, -1517, -1517
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -1517, -1517, -1517, -1517,  -105,   181, -1517,   185,  1496,   555,
   -1517,  -321, -1517, -1517,  -578,   -92,  -622, -1028,  1751,  1498,
    1492,  1044, -1517, -1517, -1517,   665, -1517,  1126,   900, -1517,
     670, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517,   683, -1517, -1517, -1517, -1517,
   -1517,   306, -1517, -1517,  1447, -1517, -1517, -1517,  -339, -1517,
   -1517, -1517,  1419, -1517, -1517,  1185,  -214,  1465, -1517, -1517,
    -330, -1517,  -704, -1517, -1517, -1517, -1517, -1517, -1517,   753,
   -1517,  -785, -1517, -1517,  1469,  1018,   692,  1449,   987,   693,
   -1517,   326,   -53, -1517, -1517,  -312,   140,  -995, -1517, -1517,
     352, -1517,  1471, -1091, -1517, -1517, -1517, -1517, -1216,   362,
      52,    54,    59,    23, -1517, -1517, -1517, -1517,   -88,  -272,
   -1517,  1028,  -962, -1517,   547, -1517, -1517, -1361, -1122, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517,    84, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517,  -907,   101, -1517,  -843, -1517, -1517,
   -1517, -1517, -1517,   151, -1517,    50, -1517, -1517, -1517,   816,
   -1517, -1150, -1517, -1517, -1253,   133,  -670,   416,  -268,  -863,
     596, -1517, -1517,  -525,  -734, -1517, -1517,  -966, -1308, -1517,
   -1517, -1517, -1517, -1517,  1501, -1517,  1278,  1495, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517,  1510, -1517, -1517, -1517,
   -1517, -1517, -1517,  -527,  -526, -1517,   991,   553,  -537,  -337,
   -1517, -1517, -1517,  -329,  -940, -1517,  -336, -1517, -1517, -1517,
   -1517,  1221, -1517, -1517,  1005, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517,  1840, -1517, -1517, -1517, -1517,
   -1517,  1553, -1517, -1517, -1517,  1356, -1517,  1355, -1517, -1517,
   -1517, -1517,  1104, -1517, -1517, -1517,     2, -1517,  -890, -1517,
    1094,  -483, -1517, -1517, -1517, -1517, -1517,    20,   -40,  -212,
     -50, -1517, -1517,  -621, -1517,  1719,  -880,  -999, -1517,  1122,
   -1044, -1239, -1517,  -871, -1517, -1517, -1517, -1517,   874, -1517,
   -1517,  -385,  -244,   615,   -37, -1160, -1397, -1517,   215, -1499,
    -501, -1095,   331, -1517,    60, -1517,   103, -1517,  -731, -1517,
    1869, -1517,  1874, -1517, -1517,  1372,  1876, -1517, -1517, -1517,
    -206,  -635, -1517,  1147, -1070,   896, -1517,   -93,  -657,  -639,
   -1517, -1517, -1517,  -541,   -41,  -387,  -623, -1517,  -429, -1517,
   -1517, -1517,   197, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517,  -496, -1517,   735, -1517,  1210, -1517,  1040,  -946, -1517,
    -595, -1517,   638,  -941,  -122,  -938,  -937,  -919,   -36,    -3,
      21, -1517,   -54,  -723,  -475,  -125, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517, -1517,
   -1517, -1517, -1517,   699, -1517, -1038, -1517,  -173,  -274,   576,
   -1517,   812,   312,  -692,  1328,    38, -1517,   -56,  1813, -1517,
     -35,   -51, -1516,   346,   -31,   -38,  -145,   -10,    -2,  -161,
   -1517,   -60,   -23,    -9, -1517
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -990
static const short yytable[] =
{
     129,    78,    41,   129,   193,   310,   150,   151,   215,   157,
     211,   262,   760,   429,   497,   282,   285,   585,   281,   284,
     198,   960,   212,   557,   746,   196,   209,   622,   626,   246,
     596,   788,   226,   791,   255,   233,   221,   252,   250,   699,
     802,   803,   768,   912,   189,   191,   194,   197,   151,   202,
    1264,  1443,   652,   249,   214,   191,   218,   256,   197,   251,
     151,   202,  1191,   231,   129,   555,   257,   934,   228,  1024,
     489,  1012,   242,   150,   151,   245,   191,   942,   242,   218,
     194,   197,   253,   151,   202,   231,   129,   726,   197,   223,
     268,   254,   270,  1252,   317,   269,  1015,   637,  1254,  1252,
    1213,  1255,  1256,   514,  1254,   470,   980,  1255,  1256,   984,
     292,  1070,   223,   874,   547,   470,   315,   957,   865,  1222,
    1257,   313,   214,   151,   214,  1042,  1257,  1011,  1306,  1311,
    1019,  1095,  1289,   369,  1049,   318,   218,   376,  1435,  1396,
    1116,   311,  1377,   197,   151,   231,  1523,   286,  1381,   157,
    1052,   772,   893,   983,   338,   893,   893,   893,   893,   732,
     883,  1526,  1778,  1045,   517,  1479,  1611,   479,  1333,  1843,
    1868,   755,   742,  1388,  1441,  1391,   276,  1483,  1618,   365,
     429,   893,   321,  1265,  1472,   540,   706,  1413,  1090,   848,
     913,   707,  1271,   135,   706,  1679,  1760,   136,  1676,   707,
     331,   -81,   370,   775,   778,   779,   780,   781,  1208,   893,
     853,  1757,  1758,   320,  1400,  1294,  1312,   278,   461,   161,
     339,   266,   131,  1274,   520,   518,   464,   882,  -661,   530,
     622,   626,   890,   490,   286,   464,  1728,  1278,   531,   998,
    1478,   498,   500,   503,   935,   936,   764,  1710,   332,  1711,
     214,  1847,   553,   893,   151,  1798,  1192,   471,   129,  1119,
     510,  1193,   596,   151,  1117,   554,  1662,   471,   515,   835,
    1121,  1122,   162,  1276,  1279,  1417,   894,   287,   706,   894,
     894,   894,   894,   707,   194,  1729,   279,  1833,   151,   132,
     854,  1787,   280,   556,  1056,  1538,  1730,   340,  1458,   371,
     562,   813,   212,  1848,   572,   894,   720,   576,  1754,  1663,
     503,   267,  1102,  1475,  1183,  1619,  1735,   646,   274,  1186,
    1187,   586,   275,   588,  1209,   214,   645,  1275,  1615,   517,
    1680,  1850,   214,   894,   214,   634,   214,   641,   643,   577,
     526,  1517,  1518,  1237,   706,   541,  1215,  1252,  1441,   707,
     846,  1206,  1254,  1407,  1180,  1255,  1256,   197,   202,  1468,
     333,  1528,  1498,  1218,  1219,   636,   151,  1076,   194,  1484,
      14,   234,  1664,  1692,  1257,  1391,   733,   894,   334,   756,
     706,  1718,   267,  1120,  1779,   707,   706,  1273,   937,  1115,
    1749,   707,  1784,  1266,  1267,  1205,   706,  1096,   532,   505,
    1731,   707,   822,  1830,   497,   805,  1118,  1217,   235,   773,
     914,   504,   335,   987,  1277,  1249,   703,  1816,  1430,  1453,
      14,  1544,  1594,  1772,  1820,   716,  1379,   708,   709,   710,
     711,  1784,  1788,   515,  -661,   719,   724,   710,   711,   823,
     730,   706,   849,  1318,   464,  1103,   707,  1750,   844,   719,
      98,   941,   882,   151,  1841,  1645,   882,   197,   471,   828,
     946,  1319,   735,  1188,   151,   640,   505,   497,   151,  1033,
    1756,   197,   151,  -371,  1742,   794,   157,    99,   349,  1696,
     721,  1234,  1643,   506,  1860,  1697,   806,  1399,   497,  1824,
    1601,    99,  1651,  1329,  1330,  1867,  1331,  1332,  1870,  1796,
     324,  1033,   776,   100,   911,  1608,   915,  1874,  1607,   651,
     919,   921,   923,   325,   101,  1037,   845,  1652,  1039,   708,
     709,   710,   711,   750,  1189,   988,   804,   807,   101,  1789,
    1825,   829,  1235,  1202,  1047,  1338,    76,  1069,  1424,  1663,
    1529,   382,  1753,   824,  1829,  1620,   856,   145,   326,  1110,
     327,   830,   722,  1010,   651,   350,   797,  1106,   214,  1063,
    -986,   470,   797,  -989,  1305,  1456,   543,  1545,  1036,   874,
    1823,  1471,  1581,  1459,  1582,  1653,   280,  1826,   205,   206,
    -243,    76,  1026,   102,  1088,   708,   709,   710,   711,   565,
    1753,  1212,   737,   103,   751,   278,  1142,   102,  1404,   651,
      77,   212,  1664,   859,   860,  1203,   223,   103,   706,  1609,
    1624,   280,  1143,   707,   517,   759,   544,   714,   104,   715,
    1003,   708,   709,   710,   711,  1429,    76,   708,   709,   710,
     711,   148,   104,   214,  1470,    76,  1753,   708,   709,   710,
     711,  1543,   877,   742,   945,    77,   916,  1445,   875,    60,
     545,   286,   146,  1031,  1032,  1027,   706,  1144,   748,   147,
     328,   707,  1801,  1802,   283,   278,   706,   286,   546,  1753,
     280,   707,   218,   961,   706,  1656,   294,   298,  1225,   707,
    -243,  1658,   708,   709,   710,   711,  1145,  1244,  1610,   989,
      77,    83,  1226,   278,   910,   834,   234,   950,   243,    77,
    -698,   706,  -243,   719,   214,   214,   707,   497,   471,  -243,
     918,   295,  1807,   471,   366,  1490,  1227,   510,   515,  1009,
     744,  1542,   471,  1766,   212,   197,  1225,  1035,   461,  1075,
     197,  1695,   727,   235,  1001,  1133,  1022,  1022,  1025,   296,
    1226,   706,   297,  -243,   286,   151,   707,   151,  1699,  1808,
     367,   562,  1307,  1146,   214,  1862,   214,   745,   298,   728,
     214,  1134,  1809,   706,  1227,  1258,  1527,  1058,   707,  1006,
    1228,  1258,  1252,   133,  1738,  1225,  1263,  1254,  1054,  1057,
    1255,  1256,   286,   214,   299,  1444,  1007,   920,  1272,  1226,
    1586,  1106,  1535,  1225,    97,   578,  1068,  1084,  1085,  1257,
    1087,  1195,   223,   158,  1198,  1384,   579,  1226,   278,  1293,
     214,   214,  1587,  1227,   343,   380,  1457,   344,  1228,  1588,
    1089,   706,  1467,   385,   159,   922,   707,   706,   160,   152,
    1100,  1227,   707,   517,  1748,  1002,  1419,  1649,  1512,  -664,
    1113,   203,   223,  1755,  1114,  1412,  1420,  1062,   151,   708,
     709,   710,   711,  1589,  1365,  1849,  1171,   792,   151,   793,
    1073,   153,   345,  1738,   355,   140,   141,  1228,  -664,  -664,
    1112,  -664,   223,   223,  1738,   223,   154,  1821,  -664,  -664,
    -664,  -664,  -664,   219,  -664,  1228,  1794,  -664,  -664,  -664,
    -664,  -664,   401,   402,   403,  -664,  -664,   708,   709,   710,
     711,   356,    76,   357,  1362,  1738,  1199,   708,   709,   710,
     711,  1831,  1832,   237,  1363,   708,   709,   710,   711,  1295,
     411,   412,  1366,   706,  1296,   239,   358,  1814,   707,  1200,
    1738,   815,  1196,   816,   247,  1738,  1295,   515,  1819,   497,
     258,  1299,   708,   709,   710,   711,   429,    80,    81,  1367,
    1003,   359,   259,   471,   197,   202,   471,   231,   129,   704,
     263,   943,  1197,   978,   471,   979,   706,   264,  1093,  1838,
    1094,   707,  1871,   205,   206,   706,   471,   471,   265,   775,
     707,   197,   708,   709,   710,   711,   429,   271,   870,  1369,
    1101,   706,  1221,  -843,  1855,   277,   707,  1342,   272,  1857,
    1192,  -843,  1194,   706,   708,   709,   710,   711,   707,  1064,
    1065,  1370,  1452,  1093,  1242,  1238,  1243,   273,   706,  1258,
    1659,   293,  1644,   707,   141,  1646,  1714,   309,   622,   626,
     322,  1286,  1302,  1287,   319,  1291,   706,  1292,   497,   354,
    1401,   707,  1402,  1474,  1725,  1192,   363,  1492,  1320,   704,
     352,  1522,   212,   704,  1324,  1530,   341,  -843,  1284,   461,
    -843,   373,   708,   709,   710,   711,  1285,  -843,   375,  1371,
    -843,   706,  -844,  1284,  -843,  1803,   707,  1804,   377,  -843,
    -844,   378,  -843,  1541,   214,   381,  1395,  1395,  1398,   706,
    1784,   214,  1866,   960,   707,  -845,   382,  1136,  -843,  1140,
     466,  1148,  1151,  -845,  1156,  1159,  1162,   223,   706,  1387,
     473,  1390,  1705,   707,  1782,  1783,  1784,  1840,  1784,  1426,
     474,  1428,  -843,   223,   477,  -843,  -843,   497,   478,   223,
     480,  1770,   706,  1872,  1873,  -843,  -844,   707,   481,  -844,
    1740,   482,   484,   499,   501,   521,  -844,   502,  1179,  -844,
     511,    14,  -843,  -844,   528,   527,   539,   542,  -844,  -845,
     550,  -844,  -845,  1499,   708,   709,   710,   711,   563,  -845,
     570,  1372,  -845,   813,  -843,   573,  -845,  -844,   706,  -843,
     583,  -845,  1473,   707,  -845,   706,   587,   638,   642,   644,
     707,  -843,   471,  -843,   471,   677,   678,   679,   706,   681,
    -845,  -844,   682,   707,  -844,  -844,  1022,   708,   709,   710,
     711,   471,  1616,   680,  -844,   197,   708,   709,   710,   711,
     683,   684,   704,  1373,  -845,   739,  1861,  -845,  -845,  1740,
     685,  -844,   708,   709,   710,   711,  1515,  -845,  1842,  1374,
    1740,  -972,   686,  1822,   708,   709,   710,   711,   687,   688,
     705,  1513,   706,  -844,  -845,  1800,   689,   707,  -844,   708,
     709,   710,   711,   694,  -843,  -843,  1514,  -843,  1531,   695,
    -844,  1740,  -844,   712,   717,   713,  -845,   708,   709,   710,
     711,  -845,   731,  1455,  1521,  -989,  1482,   734,   722,   517,
    1486,   739,   741,  -845,   754,  -845,  1740,   432,   758,   762,
     765,  1740,   924,   925,   926,   927,   928,   929,   930,   931,
     763,   766,   708,   709,   710,   711,  1771,   769,   767,   487,
     771,   774,   530,   777,   782,   786,   783,    52,   496,   785,
     708,   709,   710,   711,  1768,   795,   819,   820,    53,   821,
    1698,   826,  -430,  -844,  -844,    54,  -844,   278,   832,   708,
     709,   710,   711,   831,   836,   837,   838,   841,   839,   525,
     842,   851,   847,   852,   857,    98,  -845,  -845,   858,  -845,
      55,   867,   869,   708,   709,   710,   711,   870,   878,   879,
    1546,   880,   884,  1648,   938,   944,   973,   974,   543,  1022,
     976,   981,   212,   515,   471,   985,  1004,   986,   997,   895,
    1013,  1017,   197,  1395,   311,  1592,    99,   706,  1038,   589,
    1033,   848,   707,  1043,   517,   896,  1657,  1044,   100,   708,
     709,   710,   711,  1045,   214,  1580,   708,   709,   710,   711,
     798,  1050,   134,   151,  1051,    56,  1053,  1067,   544,   708,
     709,   710,   711,   101,  1258,  1059,  1060,  1061,  1071,  1077,
    1091,   897,  1082,  1086,  1096,  1108,  1109,   530,   691,   727,
     898,  1130,   696,   697,   701,  1166,   531,    57,   899,  1168,
    1178,    58,   545,  1170,   706,  1190,  1207,  1201,  1210,   429,
    1203,  1220,  1230,  1232,  1224,   714,  1245,   900,  1259,   901,
     546,  1661,  1248,   708,   709,   710,   711,  1268,  1280,  1693,
    1288,  1304,  1315,   902,  1270,   903,   904,   905,  1290,  1313,
    1297,  1300,   102,  1301,  1303,  1102,  1322,  1326,   515,   311,
    1325,  1327,   103,  1328,   893,  1334,   736,  1339,  1335,  1340,
     738,  1341,  1343,  1344,  1351,  1378,  1741,  1352,   191,  1353,
    1354,  1356,  1694,  1357,  1358,  1359,  1673,   104,  1360,  1361,
    1580,  1691,  1380,  1389,  1405,  1403,   906,  1408,  1406,  1409,
    1410,  1411,  1414,  1415,  1422,   105,  1425,  1434,  1427,  1442,
    1432,  1446,  1447,  1448,   950,  1476,  1449,  1450,  1451,   151,
    1454,  1707,  1469,  1480,   194,  1708,  1481,  1336,  1519,  1520,
    1524,  1485,  1491,  1511,   106,   107,  1525,   108,  1743,   742,
    1536,  1583,   150,   151,   109,   110,   111,   112,   113,  1593,
     114,  1596,  1598,   115,   116,   117,   118,   119,  1600,  1599,
    1595,   120,   121,  1597,  1602,  1741,   590,  1603,  1604,  1612,
    1614,  1723,  1617,   517,  1621,  1622,  1741,  1674,  1623,  1741,
    1625,  1626,  1627,  1628,  1631,   907,   908,   909,   708,   709,
     710,   711,  1434,   197,  1632,   945,  1633,  1634,  1635,  1636,
    1637,  1638,   132,  1639,  1681,  1640,  1642,  1741,  1667,  1650,
     517,  1689,  1666,  1774,  1686,  1690,  1669,  1670,  1682,  1675,
    1716,  1736,     6,  1712,  1683,  1687,  1744,  1781,  1684,  -288,
     150,   151,  1741,  1547,  1548,  1717,  1549,  1741,  1685,  1727,
     517,   892,  1797,  1001,  1812,  1767,  1737,  1765,   151,  1752,
    1790,   517,  1815,  1550,   517,  1792,  1785,  1787,   496,  1551,
     191,  1776,   917,   517,  1773,  1795,  1799,  1788,  1777,   933,
    1805,   161,  1806,  1845,   940,   432,  1835,   515,  1839,  1853,
    1784,  1846,   150,   151,   432,   947,  1856,   951,   952,   953,
     954,   955,   947,  1858,  1859,  1765,   151,  1863,  1836,  1864,
     465,  1392,  1865,   138,  1875,   472,   975,  1606,   483,  1580,
    1107,  1323,   868,  1321,   515,  1309,   214,   833,   551,  1552,
     597,   496,  1247,  1014,   519,  1310,  1605,   150,   151,   534,
    1048,  1585,   566,  1317,  1001,  1786,  1765,   151,    14,  1719,
     194,  1793,   496,  1591,   515,  1791,  1580,   525,  1436,  1125,
      84,  1828,  1126,  1553,   568,   515,   386,    85,   515,  1780,
    1041,    86,    87,    16,  1769,   387,   194,   515,  1734,  1817,
      88,  1239,  1675,  1751,  1539,  1418,  1580,   558,   388,   549,
    1083,   105,    89,   574,   389,   390,  1074,  1580,  1466,  1112,
    1580,   818,   855,   139,   512,   194,   194,   749,  1005,  1580,
     753,  1016,   291,   999,  1223,  1715,  1834,  1654,  1811,    44,
    1554,   107,   391,   108,    45,   392,    48,   982,  1216,  1347,
     109,   110,   111,   112,   113,   761,   114,  1382,  1431,   115,
    1555,   117,   118,   119,  1298,  1688,  1869,   120,   121,   814,
     248,  1672,     0,   394,   395,     0,     0,    90,     0,   798,
       0,     0,     0,     0,     0,   396,    98,     0,     0,   397,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1123,  1124,  1129,     0,    92,
    1137,  1138,  1141,    93,  1149,  1152,  1154,  1157,  1160,  1163,
    1164,  1165,     0,  1167,    94,  1169,     0,    99,  1172,  1173,
    1174,  1175,  1176,  1177,     0,     0,     0,     0,     0,   100,
       0,     0,     0,    95,  1127,   432,   398,     0,  1184,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,   402,   403,   404,
     205,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     405,   496,     0,     0,     0,     0,     0,     0,     0,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   106,   415,
       0,   108,     0,     0,   416,   417,     0,   418,   419,   420,
     111,   112,   113,   421,   114,     0,     0,   115,   116,   117,
     118,   119,     0,   102,     0,   120,   121,     0,     0,     0,
     422,   423,     0,   103,     0,     0,   486,     0,   425,     0,
       0,     0,   280,     0,  1135,   105,     0,  1126,     0,     0,
       0,   386,     0,     0,     0,     0,     0,     0,   104,     0,
     387,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   388,   106,   107,   105,   108,     0,   389,
     390,     0,     0,     0,   109,   110,   111,   112,   113,     0,
     114,     0,     0,   115,   116,   117,   118,   119,     0,     0,
       0,   120,   121,     0,     0,   106,   107,   391,   108,     0,
     392,     0,   259,     0,     0,   109,   110,   111,   112,   113,
       0,   114,     0,     0,   115,   116,   117,   118,   119,     0,
     947,     0,   120,   121,     0,     0,     0,     0,   394,   395,
       0,     0,  1345,  1346,     0,  1348,  1350,     0,     0,     0,
     396,     0,     0,  1355,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1368,     0,     0,     0,     1,     0,     0,     0,  1375,  1376,
     432,     0,     0,     0,     0,     0,   432,     0,  1184,  1383,
     432,  1385,  1386,     0,     0,     2,     0,   389,     0,     0,
       0,     0,     0,     3,     0,     0,     0,     0,     0,  1127,
       4,   398,     5,   496,     6,     0,     0,   399,     0,     7,
       0,     0,     0,     0,     0,   391,     8,   400,     0,     0,
     432,     0,     0,     0,     0,   565,     0,     0,     0,     0,
       9,   401,   402,   403,   404,   205,   206,     0,     0,     0,
       0,    10,     0,     0,   393,   405,     0,     0,     0,     0,
       0,     0,     0,  1437,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   106,   415,   958,   108,     0,     0,   416,
     417,     0,   418,   419,   420,   111,   112,   113,   421,   114,
       0,     0,   115,   116,   117,   118,   119,     0,     0,     0,
     120,   121,     0,     0,     0,   422,   423,     0,     0,     0,
       0,   486,   496,   425,     0,     0,     0,   280,    11,     0,
       0,     0,     0,     0,     0,    12,     0,     0,    13,   398,
      14,    15,     0,  1487,     0,     0,  1139,     0,     0,  1126,
       0,     0,  1495,   386,     0,   400,     0,     0,     0,  1500,
       0,     0,   387,     0,     0,    16,     0,     0,     0,   401,
     402,   403,     0,   205,   206,   388,     0,     0,     0,     0,
       0,   389,   390,     0,     0,     0,     0,     0,     0,   432,
     432,     0,   406,   407,   408,   409,   410,   411,   412,   413,
     414,     0,     0,     0,     0,     0,     0,   416,   417,   391,
       0,   496,   392,     0,    17,     0,   421,     0,   105,     0,
       0,     0,     0,     0,  1534,     0,     0,     0,     0,    18,
      19,     0,     0,     0,  1250,     0,     0,     0,     0,     0,
     394,   395,     0,     0,     0,     0,     0,   106,   107,   105,
     108,     0,   396,     0,     0,     0,   397,   109,   110,   111,
     112,   113,     0,   114,     0,     0,   115,   116,   117,   118,
     119,     0,     0,     0,   120,   121,     0,     0,   106,   107,
       0,   108,     0,     0,     0,     0,     0,     0,   109,   110,
     111,   112,   113,     0,   114,     0,     0,   115,   116,   117,
     118,   119,     0,     0,     0,   120,   121,     0,  1629,  1630,
       0,  1127,   959,   398,  1488,  1489,     0,     0,     0,   399,
    1493,  1494,     0,  1496,  1497,  1641,     0,     0,     0,   400,
     432,  1501,  1502,  1503,  1504,     0,  1505,  1506,  1507,  1508,
    1509,  1510,     0,   401,   402,   403,   404,   205,   206,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,  1668,     0,     0,     0,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   106,   415,     0,   108,     0,
       0,   416,   417,     0,   418,   419,   420,   111,   112,   113,
     421,   114,     0,     0,   115,   116,   117,   118,   119,     0,
       0,     0,   120,   121,     0,     0,     0,   422,   423,     0,
       0,  1147,     0,   486,  1126,   425,     0,     0,   386,   280,
       0,     0,     0,     0,     0,     0,  1706,   387,     0,     0,
       0,     0,     0,     0,  1709,     0,     0,     0,     0,     0,
     388,     0,     0,     0,     0,     0,   389,   390,     0,   895,
       0,     0,     0,     0,     0,     0,     0,   706,     0,     0,
    1534,     0,   707,     0,     0,   896,     0,     0,     0,     0,
       0,  1722,     0,     0,   391,  1724,     0,   392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   897,     0,     0,     0,   394,   395,     0,     0,  1150,
     898,     0,  1126,     0,     0,     0,   386,   396,   899,     0,
       0,   397,     0,     0,     0,   387,     0,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,   900,   388,   901,
       0,     0,     0,     0,   389,   390,     0,     0,     0,     0,
       0,     0,     0,   902,     0,   903,   904,   905,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   391,     0,     0,   392,  1127,     0,   398,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
    1818,     0,     0,     0,   400,     0,     0,     0,     0,     0,
     571,     0,     0,   394,   395,     0,   906,     0,   401,   402,
     403,   404,   205,   206,     0,   396,     0,     0,     0,   397,
       0,     0,   405,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     106,   415,     0,   108,     0,     0,   416,   417,     0,   418,
     419,   420,   111,   112,   113,   421,   114,     0,     0,   115,
     116,   117,   118,   119,     0,     0,     0,   120,   121,     0,
       0,     0,   422,   423,  1127,     0,   398,     0,   486,     0,
     425,     0,   399,     0,   280,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,   907,   908,   909,   708,   709,
     710,   711,     0,     0,     0,     0,   401,   402,   403,   404,
     205,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,     0,     0,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   106,   415,
     962,   108,     0,     0,   416,   417,     0,   418,   419,   420,
     111,   112,   113,   421,   114,     0,     0,   115,   116,   117,
     118,   119,     0,     0,     0,   120,   121,     0,     0,     0,
     422,   423,  1155,   105,     0,  1126,   486,     0,   425,   386,
       0,     0,   280,     0,     0,     0,     0,     0,   387,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   388,   106,   107,     0,   108,     0,   389,   390,     0,
       0,     0,   109,   110,   111,   112,   113,     0,   114,     0,
       0,   115,   116,   117,   118,   119,     0,     0,     0,   120,
     121,     0,     0,     0,     0,   391,     0,     0,   392,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   965,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   394,   395,     0,     0,
    1158,     0,   105,  1126,     0,     0,     0,   386,   396,     0,
       0,     0,   397,     0,     0,     0,   387,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   388,
       0,   106,   107,     0,   108,   389,   390,     0,     0,     0,
       0,   109,   110,   111,   112,   113,     0,   114,     0,     0,
     115,   116,   117,   118,   119,     0,     0,     0,   120,   121,
       0,     0,     0,   391,     0,     0,   392,  1127,     0,   398,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,   394,   395,     0,     0,     0,   401,
     402,   403,   404,   205,   206,     0,   396,     0,     0,     0,
     397,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   106,   415,     0,   108,     0,     0,   416,   417,     0,
     418,   419,   420,   111,   112,   113,   421,   114,     0,     0,
     115,   116,   117,   118,   119,     0,     0,     0,   120,   121,
       0,     0,     0,   422,   423,  1127,     0,   398,     0,   486,
       0,   425,     0,   399,     0,   280,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   402,   403,
     404,   205,   206,     0,     0,     0,     0,     0,     0,   598,
       0,   405,     0,   599,     0,     0,     0,     0,     0,     0,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   106,
     415,   600,   108,     0,   601,   416,   417,     0,   418,   419,
     420,   111,   112,   113,   421,   114,     0,     0,   115,   116,
     117,   118,   119,     0,     0,     0,   120,   121,     0,     0,
       0,   422,   423,  1161,     0,     0,  1126,   486,     0,   425,
     386,     0,   871,   280,     0,   602,   599,     0,     0,   387,
       0,   603,   604,   605,     0,   606,   607,   608,     0,   609,
     872,     0,   388,     0,   600,     0,     0,   601,   389,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   610,
       0,   611,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   391,     0,     0,   392,
       0,     0,     0,     0,     0,     0,     0,     0,   602,     0,
       0,     0,   612,     0,   603,   604,   605,     0,   606,   607,
     608,     0,   609,     0,     0,     0,     0,   394,   395,   613,
       0,     0,     0,     0,     0,     0,     0,   386,     0,   396,
       0,     0,   610,   397,   611,     0,   387,     0,     0,     0,
       0,   614,     0,     0,     0,     0,     0,     0,     0,   388,
       0,     0,     0,     0,     0,   389,   390,     0,     0,     0,
       0,     0,     0,     0,     0,   612,     0,     0,     0,     0,
       0,     0,     0,     0,   655,     0,   615,   616,     0,     0,
       0,     0,   613,   391,     0,     0,   392,     0,  1127,     0,
     398,     0,     0,   617,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,   614,     0,   400,     0,     0,     0,
       0,     0,     0,     0,   394,   395,     0,     0,     0,     0,
     401,   402,   403,   404,   205,   206,   396,     0,     0,     0,
     397,     0,     0,     0,   405,     0,     0,     0,   885,   615,
     616,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,   617,     0,   416,   417,
       0,   418,   419,   420,   111,   112,   113,   421,   114,     0,
       0,   115,   116,   117,   118,   119,     0,   698,     0,   120,
     121,   657,     0,  1651,   422,   423,     0,   398,     0,     0,
     486,     0,   425,   399,     0,     0,   280,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,  1652,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   402,   403,
     404,   205,   206,     0,     0,     0,     0,     0,     0,     0,
       0,   405,     0,     0,     0,     0,     0,     0,     0,     0,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   106,
     415,     0,   108,     0,     0,   416,   417,     0,   418,   419,
     420,   111,   112,   113,   421,   114,  1653,    62,   115,   116,
     117,   118,   119,   886,   887,   888,   120,   121,    63,   386,
       0,   422,   423,     0,     0,    64,     0,   661,   387,   425,
       0,     0,     0,   280,     0,     0,     0,     0,    65,     0,
     105,   388,     0,     0,   654,     0,     0,   389,   390,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   655,     0,     0,   106,
     107,     0,   108,     0,     0,   391,     0,     0,   392,   109,
     110,   111,   112,   113,     0,   114,     0,     0,   115,   116,
     117,   118,   119,     0,     0,     0,   120,   121,     0,    66,
       0,     0,     0,     0,     0,     0,   394,   395,     0,     0,
       0,     0,     0,     0,     0,    67,   386,     0,   396,     0,
       0,     0,   397,     0,     0,   387,     0,     0,     0,     0,
     656,     0,    68,     0,     0,     0,     0,     0,   388,     0,
       0,     0,     0,     0,   389,   390,     0,    69,     0,     0,
       0,    70,     0,     0,    71,     0,     0,     0,     0,     0,
       0,     0,    72,   655,     0,     0,     0,     0,     0,     0,
       0,     0,   391,   657,     0,   392,     0,     0,     0,   398,
       0,    73,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,   394,   395,     0,     0,     0,     0,   401,
     402,   403,   404,   205,   206,   396,     0,     0,     0,   397,
       0,     0,     0,   405,     0,     0,     0,   881,     0,     0,
       0,     0,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   106,   415,     0,   108,     0,     0,   416,   417,     0,
     418,   419,   420,   111,   112,   113,   421,   114,     0,     0,
     115,   116,   658,   659,   660,     0,     0,     0,   120,   121,
     657,     0,     0,   422,   423,     0,   398,     0,     0,   661,
       0,   425,   399,     0,     0,   280,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,   402,   403,   404,
     205,   206,     0,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,    14,     0,     0,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   106,   415,
       0,   108,     0,     0,   416,   417,   386,   418,   419,   420,
     111,   112,   113,   421,   114,   387,     0,   115,   116,   658,
     659,   660,     0,     0,     0,   120,   121,     0,   388,   105,
     422,   423,     0,     0,   389,   390,   661,     0,   425,     0,
       0,     0,   280,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   655,     0,     0,     0,     0,   106,   107,
       0,   108,   391,     0,     0,   392,     0,     0,   109,   110,
     111,   112,   113,     0,   114,     0,     0,   115,   116,   117,
     118,   119,     0,     0,     0,   120,   121,     0,     0,     0,
       0,     0,     0,   394,   395,     0,   990,   105,     0,     0,
       0,     0,     0,   386,     0,   396,     0,     0,     0,   397,
       0,     0,   387,     0,     0,     0,     0,   656,     0,     0,
       0,     0,     0,     0,     0,   388,   106,   107,     0,   108,
       0,   389,   390,     0,     0,     0,   109,   110,   111,   112,
     113,     0,   114,     0,     0,   115,   116,   117,   118,   119,
     655,     0,     0,   120,   121,     0,     0,     0,     0,   391,
     657,     0,   392,     0,   990,     0,   398,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
     394,   395,     0,     0,     0,     0,   401,   402,   403,   404,
     205,   206,   396,     0,     0,     0,   397,     0,     0,     0,
     405,     0,     0,     0,   881,     0,     0,     0,     0,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   106,   415,
       0,   108,     0,     0,   416,   417,     0,   418,   419,   420,
     111,   112,   113,   421,   114,     0,     0,   115,   116,   658,
     659,   660,     0,     0,     0,   120,   121,   657,     0,     0,
     422,   423,     0,   398,     0,     0,   661,     0,   425,   399,
       0,     0,   280,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,   402,   403,   404,   205,   206,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   106,   415,     0,   108,     0,
     462,   416,   417,   386,   418,   419,   420,   111,   112,   113,
     421,   114,   387,     0,   115,   116,   117,   118,   119,     0,
       0,     0,   120,   121,     0,   388,   105,   422,   423,     0,
       0,   389,   390,   661,     0,   425,     0,     0,     0,   280,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,   107,     0,   108,   391,
       0,     0,   392,     0,     0,   109,   110,   111,   112,   113,
       0,   114,     0,     0,   115,   116,   117,   118,   119,     0,
       0,     0,   120,   121,   467,     0,     0,     0,   393,     0,
     394,   395,     0,     0,     0,     0,     0,     0,     0,     0,
     386,   105,   396,     0,     0,     0,   397,     0,     0,   387,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   388,     0,     0,     0,     0,     0,   389,   390,
     106,   107,     0,   108,     0,     0,     0,     0,     0,     0,
     109,   110,   111,   112,   113,     0,   114,     0,     0,   115,
     116,   117,   118,   119,     0,     0,   391,   120,   121,   392,
       0,     0,     0,   398,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,   393,     0,   394,   395,     0,
       0,     0,     0,   401,   402,   403,   404,   205,   206,   396,
       0,     0,     0,   397,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   106,   415,     0,   108,     0,
       0,   416,   417,     0,   418,   419,   420,   111,   112,   113,
     421,   114,  1434,     0,   115,   116,   117,   118,   119,     0,
     698,     0,   120,   121,     0,     0,     0,   422,   423,   491,
     398,     0,     0,   486,     0,   425,   399,     0,     0,   280,
       0,     0,     6,     0,     0,     0,   400,     0,     0,  -290,
       0,     0,     0,  1547,  1548,     0,  1549,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,  1550,   405,     0,     0,     0,     0,  1551,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
     386,   418,   419,   420,   111,   112,   113,   421,   114,   387,
       0,   115,   116,   117,   118,   119,     0,     0,     0,   120,
     121,     0,   388,     0,   422,   423,     0,     0,   389,   390,
     486,     0,   425,     0,     0,     0,   280,     0,     0,  1552,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   391,     0,    14,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1553,     0,     0,     0,     0,     0,     0,
     386,     0,     0,    16,     0,   393,     0,   394,   395,   387,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,   105,   388,   397,     0,     0,     0,     0,   389,   390,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1554,   107,     0,   108,     0,     0,   391,     0,     0,   392,
     109,   110,   111,   112,   113,     0,   114,     0,     0,   115,
    1555,   117,   118,   119,     0,     0,     0,   120,   121,     0,
     398,     0,     0,     0,     0,     0,   399,   394,   395,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,   396,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
       0,   418,   419,   420,   111,   112,   113,   421,   114,     0,
     398,   115,   116,   117,   118,   119,   399,     0,     0,   120,
     121,     0,     0,   962,   422,   423,   400,     0,     0,     0,
     424,     0,   425,     0,   690,     0,   280,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
     386,   418,   419,   420,   111,   112,   113,   421,   114,   387,
       0,   115,   116,   117,   118,   119,     0,   963,     0,   120,
     121,     0,   388,     0,   422,   423,     0,     0,   389,   390,
     486,     0,   425,     0,     0,     0,   280,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   964,     0,     0,   391,     0,   965,   392,
       0,     0,     0,     0,   966,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,     0,     0,     0,
     386,     0,     0,   967,     0,     0,     0,   394,   395,   387,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,   388,   397,   106,   107,     0,   108,   389,   390,
       0,     0,     0,     0,   109,   110,   111,   112,   113,     0,
     114,     0,     0,   115,   116,   117,   118,   119,     0,     0,
       0,   120,   121,     0,     0,     0,   391,     0,     0,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     698,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,   393,   399,   394,   395,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,   396,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
       0,   418,   419,   420,   111,   112,   113,   421,   114,     0,
     398,   115,   116,   117,   118,   119,   399,     0,     0,   120,
     121,     0,     0,     0,   422,   423,   400,     0,     0,     0,
     486,     0,   425,     0,     0,     0,   280,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
     386,   418,   419,   420,   111,   112,   113,   421,   114,   387,
       0,   115,   116,   117,   118,   119,     0,     0,     0,   120,
     121,   105,   388,     0,   422,   423,     0,     0,   389,   390,
     486,     0,   425,     0,     0,     0,   280,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,   107,     0,   108,     0,     0,   391,     0,     0,   392,
     109,   110,   111,   112,   113,     0,   114,     0,     0,   115,
     116,   117,   118,   119,     0,   105,     0,   120,   121,     0,
     386,     0,     0,     0,     0,     0,     0,   394,   395,   387,
     425,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,   388,   397,   106,   107,     0,   108,   389,   390,
       0,     0,     0,     0,   109,   110,   111,   112,   113,     0,
     114,     0,     0,   115,   116,   117,   118,   119,     0,     0,
       0,   120,   121,     0,     0,     0,   391,     0,     0,   392,
       0,     0,  1837,     0,   425,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,   399,   394,   395,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,   396,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
       0,   418,   419,   420,   111,   112,   113,   421,   114,     0,
     398,   115,   116,   117,   118,   119,   399,     0,     0,   120,
     121,     0,     0,     0,   422,   423,   400,     0,     0,     0,
     486,   956,   425,     0,     0,     0,   280,  1153,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
     386,   418,   419,   420,   111,   112,   113,   421,   114,   387,
       0,   115,   116,   117,   118,   119,     0,     0,     0,   120,
     121,     0,   388,     0,   422,   423,     0,     0,   389,   390,
     486,     0,   425,     0,   105,     0,   280,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   391,     0,     0,   392,
       0,     0,     0,   106,   107,     0,   108,     0,     0,     0,
       0,     0,     0,   109,   110,   111,   112,   113,     0,   114,
     386,   105,   115,   116,   117,   118,   119,   394,   395,   387,
     120,   121,     0,     0,     0,     0,     0,     0,     0,   396,
       0,  1700,   388,   397,     0,     0,     0,     0,   389,   390,
     106,   107,     0,   108,     0,     0,     0,     0,     0,     0,
     109,   110,   111,   112,   113,     0,   114,     0,     0,   115,
     116,   117,   118,   119,     0,     0,   391,   120,   121,   392,
       0,     0,     0,     0,     0,     0,  1660,     0,     0,     0,
    1336,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,   399,   394,   395,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,   396,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
       0,   418,   419,   420,   111,   112,   113,   421,   114,     0,
     398,   115,   116,   117,   118,   119,   399,     0,     0,   120,
     121,     0,     0,     0,   422,   423,   400,     0,     0,     0,
     486,     0,   425,     0,     0,     0,   280,  1349,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
     386,   418,   419,   420,   111,   112,   113,   421,   114,   387,
       0,   115,   116,   117,   118,   119,     0,     0,     0,   120,
     121,   105,   388,     0,   422,   423,     0,     0,   389,   390,
     486,     0,   425,     0,     0,     0,   280,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     106,   107,     0,   108,     0,     0,   391,     0,     0,   392,
     109,   110,   111,   112,   113,     0,   114,     0,     0,   115,
     116,   117,   118,   119,     0,     0,     0,   120,   121,    76,
     386,   105,     0,     0,     0,     0,  1671,   394,   395,   387,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,   388,   397,     0,     0,     0,     0,   389,   390,
     106,   107,     0,   108,     0,     0,     0,     0,     0,     0,
     109,   110,   111,   112,   113,     0,   114,     0,     0,   115,
     116,   117,   118,   119,     0,     0,   391,   120,   121,   392,
       0,     0,     0,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,   399,   394,   395,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,   396,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,     0,     0,   416,   417,
       0,   418,   419,   420,   111,   112,   113,   421,   114,     0,
     398,   115,   116,   117,   118,   119,   399,     0,     0,   120,
     121,     0,     0,     0,   422,   423,   400,     0,  1721,     0,
     486,     0,   425,     0,     0,     0,   280,     0,     0,     0,
     401,   402,   403,   404,   205,   206,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   106,   415,     0,   108,  1434,     0,   416,   417,
       0,   418,   419,   420,   111,   112,   113,   421,   114,     0,
       0,   115,   116,   117,   118,   119,     0,     0,     0,   120,
     121,     0,     0,     0,   422,   423,     6,     0,     0,   105,
     486,   598,   425,     0,     0,   599,   280,  1547,  1548,     0,
    1549,     0,   798,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   600,     0,     0,   601,  1550,   106,   107,
       0,   108,     0,  1551,     0,     0,     0,     0,   109,   110,
     111,   112,   113,     0,   114,     0,     0,   115,   116,   117,
     118,   119,     0,     0,     0,   120,   121,     0,     0,     0,
       0,     0,   959,     0,     0,     0,     0,   602,     0,     0,
       0,     0,     0,   603,   604,   605,     0,   606,   607,   608,
       0,   609,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   598,     0,  1552,     0,   599,     0,     0,     0,     0,
       0,   610,     0,   611,     0,     0,     0,     0,     0,     0,
       0,     0,    14,   600,     0,     0,   601,     0,     0,   106,
     107,     0,   108,     0,     0,     0,     0,  1553,     0,   109,
     110,   111,   112,   113,   612,   114,     0,    16,   115,   116,
     117,   118,   119,     0,     0,     0,   120,   121,     0,     0,
       0,   613,     0,     0,     0,   105,     0,   602,     0,     0,
       0,     0,     0,   603,   604,   605,     0,   606,   607,   608,
       0,   609,     0,   614,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1554,   107,     0,   108,   105,     0,
       0,   610,     0,   611,   109,   110,   111,   112,   113,     0,
     114,     0,     0,   115,  1555,   117,   118,   119,   615,   616,
       0,   120,   121,     0,     0,     0,     0,   106,   107,     0,
     108,     0,     0,     0,   612,   617,     0,   109,   110,   111,
     112,   113,     0,   114,     0,     0,   115,   116,   117,   118,
     119,   613,     0,     0,   120,   121,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   614,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   105,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   615,   616,
       0,     0,     0,     0,     0,     0,     0,   106,   107,     0,
     108,     0,     0,     0,     0,   617,     0,   109,   110,   111,
     112,   113,     0,   114,     0,     0,   115,   116,   117,   118,
     119,   105,     0,     0,   120,   121,     0,     0,     0,     0,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,     0,     0,   181,   182,
     106,   107,     0,   108,     0,     0,   183,   184,     0,     0,
     109,   110,   111,   112,   113,   185,   114,   186,     0,   115,
     116,   117,   118,   119,     0,     0,     0,   120,   121
};

static const short yycheck[] =
{
       9,     4,     0,    12,    55,   150,    16,    16,    64,    18,
      63,   104,   513,   257,   288,   140,   141,   356,   140,   141,
      57,   713,    63,   335,   499,    56,    62,   364,   364,    85,
     360,   557,    69,   559,    94,    73,    67,    91,    89,   424,
     567,   567,   525,   678,    53,    54,    55,    56,    57,    58,
    1045,  1267,   381,    88,    63,    64,    65,    95,    67,    90,
      69,    70,   942,    72,    73,   333,    97,   690,    70,   773,
     282,   763,    81,    83,    83,    84,    85,   698,    87,    88,
      89,    90,    92,    92,    93,    94,    95,   474,    97,    68,
     131,    93,   133,  1039,   154,   132,   766,   369,  1039,  1045,
     980,  1039,  1039,   309,  1045,   266,   727,  1045,  1045,   731,
     145,   834,    91,   650,   328,   276,   153,   712,   643,   990,
    1039,   152,   131,   132,   133,   795,  1045,   762,  1090,  1095,
     769,   865,  1072,   221,   804,   158,   145,   230,  1260,  1209,
       8,   150,  1180,   152,   153,   154,  1385,     6,  1186,   158,
     807,    15,     8,   731,   190,     8,     8,     8,     8,   480,
     656,  1400,    50,    46,   309,  1315,  1474,   272,     8,    10,
      50,    64,    10,  1201,  1265,  1203,    66,    20,   101,   215,
     424,     8,   162,  1046,  1306,   103,    26,  1231,   858,    23,
      49,    31,  1055,    12,    26,    64,  1712,    12,  1559,    31,
       4,   157,    14,   533,   543,   544,   545,   546,    66,     8,
     193,  1710,  1711,     6,  1213,  1078,  1096,   204,   259,   133,
       3,   182,    19,    84,   317,   313,   263,   656,    66,    94,
     567,   567,   661,   283,     6,   272,     9,    47,   103,   740,
       6,   291,   293,    29,    53,    54,   518,  1644,    52,  1646,
     259,    46,    33,     8,   263,  1754,   272,   266,   267,     8,
     297,   277,   592,   272,   132,    46,   148,   276,   309,   598,
     893,   894,   186,    47,    84,  1237,   132,    49,    26,   132,
     132,   132,   132,    31,   293,    58,   273,  1803,   297,    86,
     273,    47,   279,   334,   820,  1417,    69,    80,  1293,   111,
     341,   569,   343,    98,   345,   132,   467,   348,  1705,   191,
      29,   272,   140,  1312,   937,   238,    59,   273,   137,    53,
      54,   357,   137,   359,   182,   334,   377,   188,  1478,   474,
     199,  1830,   341,   132,   343,   366,   345,   373,   375,   348,
     320,  1379,  1380,  1013,    26,   263,   981,  1293,  1439,    31,
     618,   973,  1293,  1224,   181,  1293,  1293,   366,   367,  1299,
     164,  1405,     8,   985,   986,   367,   375,   842,   377,   212,
     163,    11,   254,  1589,  1293,  1403,   481,   132,   182,   272,
      26,   264,   272,   132,   272,    31,    26,  1057,   197,   885,
    1698,    31,   272,  1050,  1051,   973,    26,   163,   263,   185,
     173,    31,    47,  1800,   678,    65,   274,   985,    48,   273,
     269,   130,   216,   734,   188,  1038,   425,  1778,   274,  1282,
     163,   274,   274,   274,   274,   462,   181,   267,   268,   269,
     270,   272,   188,   474,   272,   466,   473,   269,   270,    84,
     477,    26,   276,  1100,   481,   273,    31,  1700,    23,   480,
       6,   695,   881,   462,  1815,  1525,   885,   466,   467,   584,
     704,  1100,   485,   197,   473,    94,   185,   741,   477,   204,
    1709,   480,   481,    24,  1690,   563,   485,    47,     3,  1601,
     200,   151,  1520,   202,  1845,  1607,   146,  1210,   762,   123,
    1452,    47,    89,  1116,  1117,  1856,  1119,  1120,  1859,  1752,
       4,   204,   538,    59,   677,  1471,   679,  1868,  1470,   177,
     683,   684,   685,    17,    84,   783,    91,   114,   786,   267,
     268,   269,   270,   123,   272,   737,   567,   187,    84,  1745,
     164,   585,   202,   200,   802,  1130,   204,   272,  1242,   191,
    1411,   198,  1702,   188,  1797,  1485,   634,    71,    52,   878,
      54,   587,   272,   759,   177,    80,   565,   869,   567,   827,
     275,   722,   571,   278,  1089,  1288,    25,  1430,   782,  1106,
    1786,  1305,  1435,   124,  1437,   172,   279,   211,   207,   208,
      24,   204,   102,   153,   856,   267,   268,   269,   270,    33,
    1750,   978,   274,   163,   194,   204,    18,   153,  1220,   177,
     268,   642,   254,   639,   640,   272,   585,   163,    26,  1472,
    1490,   279,    34,    31,   759,   272,    75,   272,   188,   274,
     745,   267,   268,   269,   270,  1248,   204,   267,   268,   269,
     270,    16,   188,   642,  1304,   204,  1796,   267,   268,   269,
     270,  1426,   651,    10,   274,   268,   681,  1270,   651,   201,
     109,     6,   176,   778,   779,   175,    26,    79,   151,   183,
     164,    31,  1757,  1758,   273,   204,    26,     6,   127,  1829,
     279,    31,   681,   714,    26,  1538,    88,   170,    67,    31,
     124,  1544,   267,   268,   269,   270,   108,  1026,  1473,   274,
     268,    66,    81,   204,    49,   273,    11,   706,    83,   268,
      89,    26,   146,   734,   713,   714,    31,   981,   717,   153,
      49,   123,    83,   722,   148,  1336,   105,   754,   759,   756,
     231,  1425,   731,  1718,   765,   734,    67,   781,   769,   268,
     739,  1594,   163,    48,   743,    98,   772,   773,   774,   151,
      81,    26,   154,   187,     6,   754,    31,   756,  1611,   120,
     184,   792,  1091,   175,   763,  1850,   765,   268,   170,   190,
     769,   124,   133,    26,   105,  1039,  1401,   821,    31,   149,
     159,  1045,  1718,    19,  1681,    67,  1044,  1718,   819,   820,
    1718,  1718,     6,   792,   196,  1268,   166,    49,  1056,    81,
     124,  1103,  1415,    67,   148,     5,   832,   851,   852,  1718,
     854,   962,   781,   252,   965,  1190,    16,    81,   204,  1077,
     819,   820,   146,   105,     4,   239,  1291,     7,   159,   153,
     857,    26,  1297,   247,     0,    49,    31,    26,   271,   148,
     867,   105,    31,   978,  1697,   231,   239,   129,  1365,   206,
     880,   155,   821,  1706,   884,  1230,   249,   826,   857,   267,
     268,   269,   270,   187,  1166,   129,   274,   272,   867,   274,
     839,   180,    52,  1770,   102,   239,   240,   159,   235,   236,
     879,   238,   851,   852,  1781,   854,   195,  1784,   245,   246,
     247,   248,   249,     7,   251,   159,  1749,   254,   255,   256,
     257,   258,   203,   204,   205,   262,   263,   267,   268,   269,
     270,   139,   204,   141,   274,  1812,   966,   267,   268,   269,
     270,  1801,  1802,   204,   274,   267,   268,   269,   270,   272,
     231,   232,   274,    26,   277,    68,   164,  1770,    31,   967,
    1837,   211,   963,   213,    68,  1842,   272,   978,  1781,  1213,
     147,   277,   267,   268,   269,   270,  1190,    62,    63,   274,
    1075,   189,   273,   962,   963,   964,   965,   966,   967,   272,
     129,   274,   964,   272,   973,   274,    26,   272,   272,  1812,
     274,    31,  1862,   207,   208,    26,   985,   986,   129,  1309,
      31,   990,   267,   268,   269,   270,  1230,   131,   272,   274,
     274,    26,   990,     0,  1837,   182,    31,    57,   129,  1842,
     272,     8,   274,    26,   267,   268,   269,   270,    31,   137,
     138,   274,  1280,   272,   272,   274,   274,   129,    26,  1293,
    1546,    83,  1523,    31,   240,  1526,  1649,   164,  1365,  1365,
      64,   272,  1086,   274,    20,   272,    26,   274,  1312,   207,
     272,    31,   274,  1311,  1667,   272,    10,   274,  1102,   272,
     170,   274,  1093,   272,  1108,   274,   273,    64,  1067,  1100,
      67,    62,   267,   268,   269,   270,  1069,    74,    64,   274,
      77,    26,     0,  1082,    81,   272,    31,   274,    83,    86,
       8,   206,    89,  1422,  1093,    82,  1208,  1209,  1210,    26,
     272,  1100,   274,  1785,    31,     0,   198,   900,   105,   902,
     148,   904,   905,     8,   907,   908,   909,  1086,    26,  1201,
      86,  1203,  1613,    31,  1737,   271,   272,   271,   272,  1244,
     273,  1246,   129,  1102,    66,   132,   133,  1401,    64,  1108,
     148,    86,    26,  1864,  1865,   142,    64,    31,   129,    67,
    1681,   129,   252,   182,   107,   130,    74,   196,    66,    77,
     180,   163,   159,    81,   200,   188,   262,   262,    86,    64,
      33,    89,    67,    57,   267,   268,   269,   270,   157,    74,
     215,   274,    77,  1441,   181,   272,    81,   105,    26,   186,
     264,    86,  1307,    31,    89,    26,   118,    32,   273,    10,
      31,   198,  1201,   200,  1203,   273,   273,   273,    26,   273,
     105,   129,   273,    31,   132,   133,  1242,   267,   268,   269,
     270,  1220,  1480,   207,   142,  1224,   267,   268,   269,   270,
     273,   273,   272,   274,   129,    66,  1849,   132,   133,  1770,
     273,   159,   267,   268,   269,   270,    64,   142,    86,   274,
    1781,   273,   273,  1784,   267,   268,   269,   270,   207,   207,
     276,   274,    26,   181,   159,  1756,   273,    31,   186,   267,
     268,   269,   270,   273,   271,   272,   274,   274,  1413,   273,
     198,  1812,   200,   273,   182,   278,   181,   267,   268,   269,
     270,   186,    66,  1286,   274,   278,  1322,   148,   272,  1434,
    1326,    66,   272,   198,   272,   200,  1837,   257,   174,   264,
     273,  1842,   218,   219,   220,   221,   222,   223,   224,   225,
     278,    10,   267,   268,   269,   270,   271,   128,   272,   279,
     251,    94,    94,    94,   272,    46,    24,    40,   288,    24,
     267,   268,   269,   270,   271,    10,   182,   216,    51,   143,
    1608,   143,   182,   271,   272,    58,   274,   204,    94,   267,
     268,   269,   270,    23,   193,   193,   145,    95,   273,   319,
     273,   273,   276,   273,    64,     6,   271,   272,    10,   274,
      83,   129,   157,   267,   268,   269,   270,   272,   135,   128,
    1433,   273,   273,  1528,   197,   274,   182,   210,    25,  1425,
      63,   273,  1433,  1434,  1403,    66,   194,    66,   198,    18,
      10,   248,  1411,  1525,  1413,  1442,    47,    26,   273,    46,
     204,    23,    31,    20,  1559,    34,  1541,   273,    59,   267,
     268,   269,   270,    46,  1433,  1434,   267,   268,   269,   270,
      30,    90,    73,  1442,    90,   148,    24,   164,    75,   267,
     268,   269,   270,    84,  1718,   132,   132,   132,   162,    46,
     102,    70,   164,   273,   163,   273,    20,    94,   418,   163,
      79,   273,   422,   423,   424,    10,   103,   180,    87,   274,
      66,   184,   109,   272,    26,   272,   131,   272,   113,  1723,
     272,    66,   273,    74,   272,   272,    12,   106,    45,   108,
     127,  1547,   273,   267,   268,   269,   270,   247,    10,  1592,
     168,    10,   186,   122,   273,   124,   125,   126,   274,   200,
     275,   274,   153,   274,   274,   140,    55,    55,  1559,  1528,
     238,   274,   163,   274,     8,   273,   486,    98,   273,   273,
     490,   273,   273,   273,   273,    54,  1681,   273,  1547,   273,
     273,   273,  1593,   273,   273,   273,  1555,   188,   273,   273,
    1559,  1588,   181,    73,   274,   272,   175,   134,   274,   134,
     134,    89,    20,    77,   168,   206,   136,    17,   137,   153,
     192,   132,   132,   132,  1583,    24,   132,   132,   132,  1588,
     274,  1617,   274,   273,  1593,  1621,   274,   163,    54,   181,
     131,   274,   274,   274,   235,   236,   182,   238,  1691,    10,
     142,    26,  1612,  1612,   245,   246,   247,   248,   249,    83,
     251,    84,    47,   254,   255,   256,   257,   258,    47,    84,
     188,   262,   263,   188,   274,  1770,   263,   274,    98,    66,
     131,  1662,   117,  1778,   117,   274,  1781,    54,   274,  1784,
     274,   274,   274,   274,   274,   264,   265,   266,   267,   268,
     269,   270,    17,  1662,   274,   274,   274,   274,   274,   274,
     274,   274,    86,   274,    86,   274,   274,  1812,   273,   272,
    1815,    98,   271,  1729,   275,    90,   271,   271,   271,   197,
     114,  1679,    47,   273,   271,   274,   129,    86,   271,    54,
    1700,  1700,  1837,    58,    59,   273,    61,  1842,   271,   271,
    1845,   661,    89,  1712,   156,   271,   273,  1717,  1717,   272,
     129,  1856,   181,    78,  1859,   129,   278,    47,   678,    84,
    1729,  1730,   682,  1868,   271,   274,   274,   188,  1731,   689,
     274,   133,   272,    50,   694,   695,   273,  1778,   271,    83,
     272,   209,  1752,  1752,   704,   705,    53,   707,   708,   709,
     710,   711,   712,    39,    50,  1765,  1765,   274,  1809,   272,
     264,  1206,   273,    12,   274,   267,   722,  1461,   276,  1778,
     870,  1106,   646,  1103,  1815,  1092,  1785,   592,   331,   144,
     361,   741,  1029,   765,   315,  1093,  1460,  1797,  1797,   324,
     803,  1439,   343,  1100,  1803,  1743,  1806,  1806,   163,  1659,
    1809,  1747,   762,  1441,  1845,  1746,  1815,   767,  1261,     6,
      51,  1788,     9,   178,   343,  1856,    13,    58,  1859,  1735,
     792,    62,    63,   188,  1723,    22,  1835,  1868,  1677,  1779,
      71,  1015,   197,  1700,  1418,  1239,  1845,   336,    35,   329,
     849,   206,    83,   348,    41,    42,   841,  1856,  1295,  1858,
    1859,   573,   631,    13,   301,  1864,  1865,   501,   754,  1868,
     505,   767,   143,   741,   990,  1650,  1806,  1536,  1765,     0,
     235,   236,    69,   238,     0,    72,     0,   730,   982,  1144,
     245,   246,   247,   248,   249,   513,   251,  1188,  1250,   254,
     255,   256,   257,   258,  1082,  1583,  1858,   262,   263,   571,
      87,  1555,    -1,   100,   101,    -1,    -1,   148,    -1,    30,
      -1,    -1,    -1,    -1,    -1,   112,     6,    -1,    -1,   116,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   895,   896,   897,    -1,   180,
     900,   901,   902,   184,   904,   905,   906,   907,   908,   909,
     910,   911,    -1,   913,   195,   915,    -1,    47,   918,   919,
     920,   921,   922,   923,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,   214,   171,   935,   173,    -1,   938,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    84,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,   981,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,   153,    -1,   262,   263,    -1,    -1,    -1,
     267,   268,    -1,   163,    -1,    -1,   273,    -1,   275,    -1,
      -1,    -1,   279,    -1,     6,   206,    -1,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,   235,   236,   206,   238,    -1,    41,
      42,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,   235,   236,    69,   238,    -1,
      72,    -1,   273,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
    1130,    -1,   262,   263,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,  1142,  1143,    -1,  1145,  1146,    -1,    -1,    -1,
     112,    -1,    -1,  1153,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1170,    -1,    -1,    -1,     7,    -1,    -1,    -1,  1178,  1179,
    1180,    -1,    -1,    -1,    -1,    -1,  1186,    -1,  1188,  1189,
    1190,  1191,  1192,    -1,    -1,    28,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,   171,
      43,   173,    45,  1213,    47,    -1,    -1,   179,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    69,    59,   189,    -1,    -1,
    1230,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      73,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    84,    -1,    -1,    98,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1263,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    42,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,
     262,   263,    -1,    -1,    -1,   267,   268,    -1,    -1,    -1,
      -1,   273,  1312,   275,    -1,    -1,    -1,   279,   151,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   173,
     163,   164,    -1,  1333,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,  1342,    13,    -1,   189,    -1,    -1,    -1,  1349,
      -1,    -1,    22,    -1,    -1,   188,    -1,    -1,    -1,   203,
     204,   205,    -1,   207,   208,    35,    -1,    -1,    -1,    -1,
      -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,  1379,
    1380,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,    -1,    -1,    -1,    -1,    -1,    -1,   241,   242,    69,
      -1,  1401,    72,    -1,   237,    -1,   250,    -1,   206,    -1,
      -1,    -1,    -1,    -1,  1414,    -1,    -1,    -1,    -1,   252,
     253,    -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,   235,   236,   206,
     238,    -1,   112,    -1,    -1,    -1,   116,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   262,   263,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,  1498,  1499,
      -1,   171,   269,   173,  1334,  1335,    -1,    -1,    -1,   179,
    1340,  1341,    -1,  1343,  1344,  1515,    -1,    -1,    -1,   189,
    1520,  1351,  1352,  1353,  1354,    -1,  1356,  1357,  1358,  1359,
    1360,  1361,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,  1552,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    -1,    -1,   267,   268,    -1,
      -1,     6,    -1,   273,     9,   275,    -1,    -1,    13,   279,
      -1,    -1,    -1,    -1,    -1,    -1,  1616,    22,    -1,    -1,
      -1,    -1,    -1,    -1,  1624,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,
    1650,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,  1661,    -1,    -1,    69,  1665,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,   100,   101,    -1,    -1,     6,
      79,    -1,     9,    -1,    -1,    -1,    13,   112,    87,    -1,
      -1,   116,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1723,    -1,    -1,    -1,   106,    35,   108,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,   125,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    72,   171,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
    1780,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,   100,   101,    -1,   175,    -1,   203,   204,
     205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
      -1,    -1,   267,   268,   171,    -1,   173,    -1,   273,    -1,
     275,    -1,   179,    -1,   279,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,   264,   265,   266,   267,   268,
     269,   270,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      74,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
     267,   268,     6,   206,    -1,     9,   273,    -1,   275,    13,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,   235,   236,    -1,   238,    -1,    41,    42,    -1,
      -1,    -1,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
       6,    -1,   206,     9,    -1,    -1,    -1,    13,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,   235,   236,    -1,   238,    41,    42,    -1,    -1,    -1,
      -1,   245,   246,   247,   248,   249,    -1,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,    69,    -1,    -1,    72,   171,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,   112,    -1,    -1,    -1,
     116,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
      -1,    -1,    -1,   267,   268,   171,    -1,   173,    -1,   273,
      -1,   275,    -1,   179,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,   217,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    41,   238,    -1,    44,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,   267,   268,     6,    -1,    -1,     9,   273,    -1,   275,
      13,    -1,    19,   279,    -1,    85,    23,    -1,    -1,    22,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      37,    -1,    35,    -1,    41,    -1,    -1,    44,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   119,
      -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,   152,    -1,    91,    92,    93,    -1,    95,    96,
      97,    -1,    99,    -1,    -1,    -1,    -1,   100,   101,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,   112,
      -1,    -1,   119,   116,   121,    -1,    22,    -1,    -1,    -1,
      -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,   226,   227,    -1,    -1,
      -1,    -1,   169,    69,    -1,    -1,    72,    -1,   171,    -1,
     173,    -1,    -1,   243,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   191,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,   112,    -1,    -1,    -1,
     116,    -1,    -1,    -1,   217,    -1,    -1,    -1,   124,   226,
     227,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,   243,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,   163,    -1,   262,
     263,   167,    -1,    89,   267,   268,    -1,   173,    -1,    -1,
     273,    -1,   275,   179,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,   172,    40,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,    51,    13,
      -1,   267,   268,    -1,    -1,    58,    -1,   273,    22,   275,
      -1,    -1,    -1,   279,    -1,    -1,    -1,    -1,    71,    -1,
     206,    35,    -1,    -1,    38,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,   132,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   148,    13,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    22,    -1,    -1,    -1,    -1,
     124,    -1,   165,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,   180,    -1,    -1,
      -1,   184,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,   167,    -1,    72,    -1,    -1,    -1,   173,
      -1,   214,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,   262,   263,
     167,    -1,    -1,   267,   268,    -1,   173,    -1,    -1,   273,
      -1,   275,   179,    -1,    -1,   279,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    13,   244,   245,   246,
     247,   248,   249,   250,   251,    22,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    35,   206,
     267,   268,    -1,    -1,    41,    42,   273,    -1,   275,    -1,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,    69,    -1,    -1,    72,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,   273,   206,    -1,    -1,
      -1,    -1,    -1,    13,    -1,   112,    -1,    -1,    -1,   116,
      -1,    -1,    22,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,   235,   236,    -1,   238,
      -1,    41,    42,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
      60,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,    69,
     167,    -1,    72,    -1,   273,    -1,   173,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     217,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,   167,    -1,    -1,
     267,   268,    -1,   173,    -1,    -1,   273,    -1,   275,   179,
      -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
     180,   241,   242,    13,   244,   245,   246,   247,   248,   249,
     250,   251,    22,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,    -1,    35,   206,   267,   268,    -1,
      -1,    41,    42,   273,    -1,   275,    -1,    -1,    -1,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,    69,
      -1,    -1,    72,    -1,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,   262,   263,   189,    -1,    -1,    -1,    98,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,   206,   112,    -1,    -1,    -1,   116,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    69,   262,   263,    72,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    98,    -1,   100,   101,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    17,    -1,   254,   255,   256,   257,   258,    -1,
     163,    -1,   262,   263,    -1,    -1,    -1,   267,   268,   269,
     173,    -1,    -1,   273,    -1,   275,   179,    -1,    -1,   279,
      -1,    -1,    47,    -1,    -1,    -1,   189,    -1,    -1,    54,
      -1,    -1,    -1,    58,    59,    -1,    61,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,   217,    -1,    -1,    -1,    -1,    84,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,   163,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,   188,    -1,    98,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   206,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    74,   267,   268,   189,    -1,    -1,    -1,
     273,    -1,   275,    -1,   197,    -1,   279,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,   148,    -1,   262,
     263,    -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   184,    -1,    -1,    69,    -1,   189,    72,
      -1,    -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      13,    -1,    -1,   214,    -1,    -1,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,   235,   236,    -1,   238,    41,    42,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    98,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,   206,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,   206,    -1,   262,   263,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,
     275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,   235,   236,    -1,   238,    41,    42,
      -1,    -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,    -1,
      -1,   262,   263,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,   273,    -1,   275,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,   274,   275,    -1,    -1,    -1,   279,   200,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,   206,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      13,   206,   254,   255,   256,   257,   258,   100,   101,    22,
     262,   263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,   273,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    69,   262,   263,    72,
      -1,    -1,    -1,    -1,    -1,    -1,   271,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,    -1,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,   200,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      13,   244,   245,   246,   247,   248,   249,   250,   251,    22,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,   206,    35,    -1,   267,   268,    -1,    -1,    41,    42,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   236,    -1,   238,    -1,    -1,    69,    -1,    -1,    72,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263,   204,
      13,   206,    -1,    -1,    -1,    -1,   271,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
     235,   236,    -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,    -1,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    69,   262,   263,    72,
      -1,    -1,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,   189,    -1,   271,    -1,
     273,    -1,   275,    -1,    -1,    -1,   279,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    17,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,   262,
     263,    -1,    -1,    -1,   267,   268,    47,    -1,    -1,   206,
     273,    19,   275,    -1,    -1,    23,   279,    58,    59,    -1,
      61,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    -1,    -1,    44,    78,   235,   236,
      -1,   238,    -1,    84,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    -1,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,    -1,
      -1,    -1,   269,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,    19,    -1,   144,    -1,    23,    -1,    -1,    -1,    -1,
      -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    41,    -1,    -1,    44,    -1,    -1,   235,
     236,    -1,   238,    -1,    -1,    -1,    -1,   178,    -1,   245,
     246,   247,   248,   249,   152,   251,    -1,   188,   254,   255,
     256,   257,   258,    -1,    -1,    -1,   262,   263,    -1,    -1,
      -1,   169,    -1,    -1,    -1,   206,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    91,    92,    93,    -1,    95,    96,    97,
      -1,    99,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   235,   236,    -1,   238,   206,    -1,
      -1,   119,    -1,   121,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,   226,   227,
      -1,   262,   263,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,   152,   243,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,   169,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,    -1,    -1,    -1,    -1,   243,    -1,   245,   246,   247,
     248,   249,    -1,   251,    -1,    -1,   254,   255,   256,   257,
     258,   206,    -1,    -1,   262,   263,    -1,    -1,    -1,    -1,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,    -1,    -1,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,    -1,
     245,   246,   247,   248,   249,   250,   251,   252,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    -1,   262,   263
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     7,    28,    36,    43,    45,    47,    52,    59,    73,
      84,   151,   158,   161,   163,   164,   188,   237,   252,   253,
     281,   282,   283,   292,   302,   312,   314,   316,   436,   459,
     481,   505,   506,   507,   508,   509,   511,   513,   514,   518,
     535,   536,   537,   547,   590,   592,   593,   594,   596,   597,
     598,   602,    40,    51,    58,    83,   148,   180,   184,   460,
     201,   515,    40,    51,    58,    71,   132,   148,   165,   180,
     184,   187,   195,   214,   313,   318,   204,   268,   649,   650,
      62,    63,   303,    66,    51,    58,    62,    63,    71,    83,
     148,   165,   180,   184,   195,   214,   482,   148,     6,    47,
      59,    84,   153,   163,   188,   206,   235,   236,   238,   245,
     246,   247,   248,   249,   251,   254,   255,   256,   257,   258,
     262,   263,   285,   286,   287,   288,   298,   299,   695,   703,
     704,    19,    86,    19,    73,   285,   287,   293,   298,   515,
     239,   240,   553,   554,   555,    71,   176,   183,   573,   574,
     697,   703,   148,   180,   195,   315,   702,   703,   252,     0,
     271,   133,   186,   538,   474,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   233,   234,   241,   242,   250,   252,   467,   468,   703,
     687,   703,   473,   691,   703,   397,   694,   703,   574,   697,
     478,   698,   703,   155,   516,   207,   208,   337,   339,   648,
     326,   372,   614,   683,   703,   687,   335,   690,   703,     7,
     394,   694,   320,   650,   652,   362,   574,   451,   698,   437,
     701,   703,   336,   695,    11,    48,   541,   204,   651,    68,
     311,   688,   703,   573,   686,   703,   687,    68,   688,   690,
     691,   694,   652,   697,   698,   701,   695,   694,   147,   273,
     607,   608,   607,   129,   272,   129,   182,   272,   614,   574,
     614,   131,   129,   129,   285,   287,    66,   182,   204,   273,
     279,   644,   655,   273,   644,   655,     6,    49,   556,   677,
     550,   555,   690,    83,    88,   123,   151,   154,   170,   196,
     519,   520,   521,   522,   523,   524,   525,   526,   528,   164,
     696,   703,   395,   694,   363,   574,   438,   701,   702,    20,
       6,   547,    64,   544,     4,    17,    52,    54,   164,   475,
     476,     4,    52,   164,   182,   216,   463,   464,   648,     3,
      80,   273,   398,     4,     7,    52,   465,   466,   469,     3,
      80,   453,   170,   517,   207,   102,   139,   141,   164,   189,
     340,   341,   342,    10,   327,   648,   148,   184,   317,   398,
      14,   111,   321,    62,   364,    64,   607,    83,   206,   304,
     679,    82,   198,   580,   595,   679,    13,    22,    35,    41,
      42,    69,    72,    98,   100,   101,   112,   116,   173,   179,
     189,   203,   204,   205,   206,   217,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   236,   241,   242,   244,   245,
     246,   250,   267,   268,   273,   275,   424,   425,   571,   572,
     613,   614,   637,   638,   639,   642,   643,   644,   645,   646,
     647,   648,   656,   657,   658,   659,   660,   661,   662,   664,
     665,   666,   667,   668,   669,   671,   678,   679,   696,   703,
     609,   614,   180,   284,   574,   288,   148,   189,   300,   301,
     699,   703,   299,    86,   273,   610,   611,    66,    64,   284,
     148,   129,   129,   300,   252,   512,   273,   637,   549,   549,
     550,   269,   557,   558,   559,   601,   637,   678,   550,   182,
     691,   107,   196,    29,   130,   185,   202,   531,   532,   534,
     574,   180,   521,   599,   600,   614,   615,   696,   398,   364,
     607,   130,   510,   539,   540,   637,   547,   188,   200,   546,
      94,   103,   263,   325,   347,   348,   349,   350,   356,   262,
     103,   263,   262,    25,    75,   109,   127,   346,   477,   476,
      33,   334,   448,    33,    46,   448,   614,   375,   464,   373,
     400,   401,   614,   157,   399,    33,   367,   372,   382,   383,
     215,    33,   614,   272,   467,   471,   614,   703,     5,    16,
     454,   455,   479,   264,   338,   338,   648,   118,   648,    46,
     263,   343,   344,   345,   346,   347,   350,   342,    19,    23,
      41,    44,    85,    91,    92,    93,    95,    96,    97,    99,
     119,   121,   152,   169,   191,   226,   227,   243,   483,   484,
     485,   488,   489,   490,   491,   495,   496,   497,   498,   499,
     500,   502,   503,   396,   694,   452,   698,   399,    32,   322,
      94,   648,   273,   574,    10,   691,   273,   306,   307,   308,
     375,   177,   493,   649,    38,    60,   124,   167,   256,   257,
     258,   273,   616,   618,   619,   620,   621,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   637,   273,   273,   273,
     207,   273,   273,   273,   273,   273,   273,   207,   207,   273,
     197,   637,   672,   674,   273,   273,   637,   637,   163,   571,
     636,   637,   700,   703,   272,   276,    26,    31,   267,   268,
     269,   270,   273,   278,   272,   274,   574,   182,   291,   694,
     699,   200,   272,   290,   574,   612,   615,   163,   190,   548,
     574,    66,   291,   284,   148,   702,   637,   274,   637,    66,
     561,   272,    10,   560,   231,   268,   654,   655,   151,   525,
     123,   194,   527,   527,   272,    64,   272,   533,   174,   272,
     580,   595,   264,   278,   399,   273,    10,   272,   541,   128,
     545,   251,    15,   273,    94,   350,   648,    94,   338,   338,
     338,   338,   272,    24,   462,    24,    46,   461,   484,   489,
     496,   484,   272,   274,   398,    10,   684,   703,    30,   368,
     369,   370,   483,   484,   614,    65,   146,   187,   384,   385,
     386,   387,   389,   448,   684,   211,   213,   472,   466,   182,
     216,   143,    47,    84,   188,   456,   143,   457,   655,   652,
     648,    23,    94,   345,   273,   493,   193,   193,   145,   273,
     504,    95,   273,   501,    23,    91,   448,   276,    23,   276,
     494,   273,   273,   193,   273,   501,   398,    64,    10,   648,
     648,   365,   366,   367,   382,   453,   446,   129,   307,   157,
     272,    19,    37,   305,   488,   649,   680,   703,   135,   128,
     273,   124,   618,   631,   273,   124,   259,   260,   261,   617,
     618,   632,   637,     8,   132,    18,    34,    70,    79,    87,
     106,   108,   122,   124,   125,   126,   175,   264,   265,   266,
      49,   677,   601,    49,   269,   677,   690,   637,    49,   677,
      49,   677,    49,   677,   218,   219,   220,   221,   222,   223,
     224,   225,   676,   637,   616,    53,    54,   197,   197,   670,
     637,   572,   553,   274,   274,   274,   572,   637,   640,   682,
     703,   637,   637,   637,   637,   637,   274,   640,    42,   269,
     683,   614,    74,   148,   184,   189,   195,   214,   294,   295,
     296,   297,   699,   182,   210,   301,    63,   603,   272,   274,
     553,   273,   603,   294,   296,    66,    66,   291,   549,   274,
     273,   562,   563,   564,   568,   569,   694,   198,   580,   559,
     692,   703,   231,   655,   194,   532,   149,   166,   529,   574,
     600,   601,   683,    10,   365,   446,   540,   248,   543,   609,
     352,   353,   648,   351,   352,   648,   102,   175,   357,   358,
     359,   655,   655,   204,   653,   652,   346,   448,   273,   448,
     449,   401,   446,    20,   273,    46,   376,   448,   368,   446,
      90,    90,   608,    24,   614,   470,   484,   614,   652,   132,
     132,   132,   650,   448,   137,   138,   361,   164,   648,   272,
     653,   162,   492,   650,   504,   268,   654,    46,   328,   486,
     487,   654,   164,   486,   652,   652,   273,   652,   399,   574,
     446,   102,   323,   272,   274,   454,   163,   439,   440,   441,
     574,   274,   140,   273,   309,   310,   375,   308,   273,    20,
     493,   685,   703,   548,   548,   631,     8,   132,   274,     8,
     132,   616,   616,   637,   637,     6,     9,   171,   622,   637,
     273,   633,   634,    98,   124,     6,   622,   637,   637,     6,
     622,   637,    18,    34,    79,   108,   175,     6,   622,   637,
       6,   622,   637,   200,   637,     6,   622,   637,     6,   622,
     637,     6,   622,   637,   637,   637,    10,   637,   274,   637,
     272,   274,   637,   637,   637,   637,   637,   637,    66,    66,
     181,   572,   675,   616,   637,   673,    53,    54,   197,   272,
     272,   556,   272,   277,   274,   699,   694,   698,   699,   701,
     695,   272,   200,   272,   289,   294,   296,   131,    66,   182,
     113,   605,   615,   556,   591,   601,   605,   294,   296,   296,
      66,   536,   563,   568,   272,    67,    81,   105,   159,   575,
     273,   570,    74,   576,   151,   202,   530,   446,   274,   439,
     551,   354,   272,   274,   338,    12,   360,   359,   273,   616,
     268,   377,   638,   641,   643,   645,   646,   647,   678,    45,
     402,   403,   404,   448,   377,   449,   608,   608,   247,   388,
     273,   449,   448,   446,    84,   188,    47,   188,    47,    84,
      10,   458,   480,   681,   703,   649,   272,   274,   168,   494,
     274,   272,   274,   448,   449,   272,   277,   275,   681,   277,
     274,   274,   652,   274,    10,   453,   402,   338,   324,   325,
     366,   457,   556,   200,   450,   186,   319,   369,   608,   609,
     652,   310,    55,   305,   652,   238,    55,   274,   274,   616,
     616,   616,   616,     8,   273,   273,   163,   635,   640,    98,
     273,   273,    57,   273,   273,   637,   637,   633,   637,   200,
     637,   273,   273,   273,   273,   637,   273,   273,   273,   273,
     273,   273,   274,   274,   374,   375,   274,   274,   637,   274,
     274,   274,   274,   274,   274,   637,   637,   675,    54,   181,
     181,   675,   673,   637,   571,   637,   637,   295,   297,    73,
     295,   297,   289,   493,   604,   644,   604,   606,   644,   653,
     557,   272,   274,   272,   296,   274,   274,   563,   134,   134,
     134,    89,   571,   560,    20,    77,   579,   402,   450,   239,
     249,   542,   168,   355,   352,   136,   655,   137,   655,   616,
     274,   642,   192,   405,    17,   408,   404,   637,   378,   379,
     380,   383,   153,   388,   541,   616,   132,   132,   132,   132,
     132,   132,   448,   449,   274,   649,   653,   654,   377,   124,
     329,   330,   331,   332,   333,   334,   487,   654,   494,   274,
     446,   454,   408,   655,   448,   557,    24,   447,     6,   441,
     273,   274,   648,    20,   212,   274,   648,   637,   635,   635,
     553,   274,   274,   635,   635,   637,   635,   635,     8,    57,
     637,   635,   635,   635,   635,   635,   635,   635,   635,   635,
     635,   274,   483,   274,   274,    64,   663,   675,   675,    54,
     181,   274,   274,   561,   131,   182,   561,   601,   560,   563,
     274,   696,   577,   578,   637,   616,   142,   581,   408,   447,
     552,   338,   352,   361,   274,   449,   372,    58,    59,    61,
      78,    84,   144,   178,   235,   255,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   417,   418,   419,   420,   421,
     423,   428,   429,   430,   536,   590,   592,   596,   600,   693,
     703,   449,   449,    26,   371,   380,   124,   146,   153,   187,
     381,   389,   574,    83,   274,   188,    84,   188,    47,    84,
      47,   402,   274,   274,    98,   371,   331,   402,   457,   449,
     361,   458,    66,   442,   131,   441,   448,   117,   101,   238,
     494,   117,   274,   274,   556,   274,   274,   274,   274,   637,
     637,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   637,   274,   675,   580,   604,   580,   565,   696,   129,
     272,    89,   114,   172,   582,   583,   449,   655,   449,   484,
     271,   687,   148,   191,   254,   416,   271,   273,   637,   271,
     271,   271,   693,   703,    54,   197,   407,   432,   433,    64,
     199,    86,   271,   271,   271,   271,   275,   274,   682,    98,
      90,   574,   388,   607,   691,   449,   408,   408,   448,   449,
     273,   443,   444,   445,   573,   580,   637,   648,   648,   637,
     576,   576,   273,   566,   616,   578,   114,   273,   264,   376,
     406,   271,   637,   694,   637,   616,   422,   271,     9,    58,
      69,   173,   434,   435,   433,    59,   536,   273,   424,   427,
     613,   696,   388,   607,   129,   390,   391,   392,   449,   458,
     444,   445,   272,   575,   576,   449,   561,   579,   579,   567,
     692,   582,   584,   585,   586,   697,   377,   271,   271,   425,
      86,   271,   274,   271,   687,   689,   703,   649,    50,   272,
     416,    86,   616,   271,   272,   278,   390,    47,   188,   388,
     129,   392,   129,   391,   449,   274,   444,    89,   579,   274,
     580,   581,   581,   272,   274,   274,   272,    83,   120,   133,
     587,   586,   156,   426,   427,   181,   407,   435,   637,   427,
     274,   424,   613,   388,   123,   164,   211,   393,   393,   444,
     576,   538,   538,   692,   584,   273,   691,   273,   427,   271,
     271,   407,    86,    10,   431,    50,   209,    46,    98,   129,
     579,   588,   691,    83,   589,   427,    53,   427,    39,    50,
     407,   616,   581,   274,   272,   273,   274,   407,    50,   685,
     407,   538,   588,   588,   407,   274
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

  case 22:

    { prepare_console_debug ((int) yyvsp[0], &lex.dsql_debug);
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
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-2]; }
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
				yyvsp[-3], MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG)); }
    break;

  case 89:

    { yyval = yyvsp[-1]; }
    break;

  case 90:

    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((STR) FUN_reference, CONSTANT_SLONG));}
    break;

  case 91:

    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((STR) (-1 * FUN_reference), CONSTANT_SLONG));}
    break;

  case 92:

    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((STR) FUN_value, CONSTANT_SLONG));}
    break;

  case 93:

    { yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG));}
    break;

  case 94:

    { yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
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

    { yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;

  case 122:

    { yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;

  case 123:

    { yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;

  case 124:

    { yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;

  case 125:

    { yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;

  case 126:

    { yyval = (DSQL_NOD) 0;}
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
			  yyval = (DSQL_NOD) yyvsp[0]; }
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
						(DSQL_NOD) lex.g_file);}
    break;

  case 179:

    { lex.g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
    break;

  case 181:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 182:

    { 
				 check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
												(DSQL_NOD) lex.g_file); }
    break;

  case 183:

    { lex.g_file = make_file();
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
    break;

  case 186:

    { lex.g_file->fil_length = (SLONG) yyvsp[0]; }
    break;

  case 187:

    { lex.g_file  = make_file ();}
    break;

  case 192:

    { lex.g_file->fil_start = (SLONG) yyvsp[0];}
    break;

  case 193:

    { lex.g_file->fil_length = (SLONG) yyvsp[-1];}
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
			  yyval = (DSQL_NOD) lex.g_field; }
    break;

  case 220:

    { lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
    break;

  case 221:

    { yyval = yyvsp[-1]; }
    break;

  case 222:

    { lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
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

    { yyval = make_node (nod_foreign, e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 239:

    { yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;

  case 240:

    { yyval = make_node (nod_primary, e_pri_count, NULL, yyvsp[0]); }
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

    { yyval = make_node (nod_primary, e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 250:

    { yyval = make_node (nod_foreign, e_for_count, yyvsp[-5], yyvsp[-3], 
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

    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], NULL);}
    break;

  case 255:

    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;

  case 256:

    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;

  case 257:

    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
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
			 REF_ACTION_CASCADE, e_ref_trig_action_count, NULL);}
    break;

  case 262:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, e_ref_trig_action_count, NULL);}
    break;

  case 263:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, e_ref_trig_action_count, NULL);}
    break;

  case 264:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, e_ref_trig_action_count, NULL);}
    break;

  case 265:

    { yyval = make_node (nod_def_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 266:

    { yyval = make_node (nod_redef_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 267:

    { yyval = make_node (nod_replace_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 268:

    { yyval = make_node (nod_mod_procedure,
						  (int) e_prc_count, 
							  yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 269:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 270:

    { yyval = NULL; }
    break;

  case 271:

    { yyval = yyvsp[0]; }
    break;

  case 272:

    { yyval = NULL; }
    break;

  case 274:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 275:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
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

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], NULL, NULL, NULL, NULL, NULL); }
    break;

  case 281:

    { yyval = NULL; }
    break;

  case 282:

    { yyval = NULL; }
    break;

  case 283:

    { yyval = yyvsp[0]; }
    break;

  case 284:

    { yyval = yyvsp[0]; }
    break;

  case 287:

    { yyval = yyvsp[-1]; }
    break;

  case 288:

    { yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[0]), NULL); }
    break;

  case 289:

    { yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;

  case 290:

    { yyval = make_node (nod_block, e_blk_count, NULL, NULL);}
    break;

  case 292:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 308:

    { yyval = make_node (nod_return, e_rtn_count, NULL); }
    break;

  case 309:

    { yyval = make_node (nod_exit, 0, NULL); }
    break;

  case 311:

    { yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
    break;

  case 312:

    { yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 313:

    { yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
    break;

  case 314:

    { yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;

  case 315:

    { yyval = make_node (nod_exec_sql, e_exec_sql_count, yyvsp[-1]); }
    break;

  case 318:

    { yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;

  case 319:

    { yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;

  case 320:

    { yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
    break;

  case 321:

    { yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;

  case 322:

    { yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;

  case 323:

    { yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 324:

    { yyval = NULL; }
    break;

  case 325:

    { yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
    break;

  case 326:

    { yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;

  case 327:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 328:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 329:

    { yyval = NULL; }
    break;

  case 330:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 331:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 332:

    { yyval = NULL; }
    break;

  case 335:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 336:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 337:

    { yyval = make_node (nod_while, e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;

  case 338:

    { yyval = make_node (nod_label, 1, yyvsp[-1]); }
    break;

  case 339:

    { yyval = NULL; }
    break;

  case 340:

    { yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
    break;

  case 341:

    { yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
    break;

  case 342:

    { yyval = make_node (nod_breakleave, e_breakleave_count,
				make_node (nod_label, 1, yyvsp[-1])); }
    break;

  case 343:

    { yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;

  case 344:

    { yyval = NULL; }
    break;

  case 346:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 347:

    { yyval = make_node (nod_on_error, e_err_count,
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

  case 354:

    { yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;

  case 355:

    { yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 356:

    { yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 357:

    { yyval = make_node (nod_select, e_select_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 358:

    { yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;

  case 359:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 360:

    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 361:

    { yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 362:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 364:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 367:

    { yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 368:

    { yyval = yyvsp[-1]; }
    break;

  case 369:

    { lex.beginning = lex_position(); }
    break;

  case 370:

    { yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
				   (lex_position() == lex.end) ?
				   lex_position()-lex.beginning : lex.last_token-lex.beginning);}
    break;

  case 371:

    { lex.beginning = lex.last_token; }
    break;

  case 372:

    { yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
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
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 376:

    { yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 377:

    { yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;

  case 378:

    { yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;

  case 379:

    { yyval = NULL; }
    break;

  case 380:

    { yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;

  case 381:

    { yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;

  case 382:

    { yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;

  case 383:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
    break;

  case 384:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
    break;

  case 385:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
    break;

  case 386:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
    break;

  case 387:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
    break;

  case 388:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
    break;

  case 389:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
    break;

  case 390:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
    break;

  case 391:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
    break;

  case 392:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
    break;

  case 393:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
    break;

  case 394:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
    break;

  case 395:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
    break;

  case 396:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
    break;

  case 397:

    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
    break;

  case 398:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 399:

    { yyval = NULL; }
    break;

  case 400:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
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

    { yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 412:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 413:

    { yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;

  case 414:

    { yyval = yyvsp[0]; }
    break;

  case 415:

    { yyval = yyvsp[0]; }
    break;

  case 416:

    {yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;

  case 417:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;

  case 418:

    { yyval = yyvsp[0]; }
    break;

  case 419:

    { yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;

  case 421:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 422:

    { yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 423:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;

  case 424:

    { yyval = yyvsp[0]; }
    break;

  case 425:

    { yyval = yyvsp[0]; }
    break;

  case 426:

    { yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 427:

    { yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 428:

    { yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;

  case 429:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 453:

    { yyval = NULL; }
    break;

  case 454:

    { yyval = NULL; }
    break;

  case 455:

    { yyval = NULL; }
    break;

  case 456:

    { yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 457:

    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;

  case 458:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 459:

    { yyval = make_node (nod_cascade, 0, NULL); }
    break;

  case 460:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 461:

    { yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;

  case 462:

    { yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;

  case 463:

    { lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
    break;

  case 465:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 466:

    { yyval = yyvsp[0]; }
    break;

  case 467:

    { yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;

  case 468:

    { yyval = yyvsp[0]; }
    break;

  case 469:

    { yyval = yyvsp[0]; }
    break;

  case 470:

    { yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;

  case 471:

    { yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;

  case 472:

    { yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;

  case 473:

    { yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;

  case 475:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 476:

    { yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 478:

    { yyval = NULL; }
    break;

  case 480:

    { yyval = NULL; }
    break;

  case 481:

    { yyval = yyvsp[0]; }
    break;

  case 482:

    { yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;

  case 483:

    { yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;

  case 484:

    { yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;

  case 485:

    { yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;

  case 486:

    { yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;

  case 487:

    { yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;

  case 488:

    { yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;

  case 489:

    { yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;

  case 490:

    { yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;

  case 491:

    { yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;

  case 492:

    { yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;

  case 493:

    { yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;

  case 498:

    { lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;

  case 499:

    { lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;

  case 501:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 502:

    { if ((SLONG) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
    break;

  case 503:

    { yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 509:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
    break;

  case 510:

    { 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;

  case 511:

    { 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;

  case 512:

    { 
			*stmt_ambiguous = TRUE;
			if (client_dialect <= SQL_DIALECT_V5)
				{
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
					ERRD_post_warning (isc_sqlwarn, gds_arg_number, (SLONG) 301, 
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

  case 513:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
    break;

  case 514:

    { 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;

  case 517:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;

  case 518:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;

  case 519:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;

  case 520:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
    break;

  case 521:

    {
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
    break;

  case 522:

    {
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;

  case 523:

    {
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
    break;

  case 524:

    {
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;

  case 525:

    {
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;

  case 526:

    {
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;

  case 528:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 529:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 530:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 531:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;

  case 532:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;

  case 533:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
    break;

  case 542:

    { 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;

  case 543:

    {  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;

  case 544:

    {
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;

  case 545:

    {		 
			if ( ((SLONG) yyvsp[-1] < 1) || ((SLONG) yyvsp[-1] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((SLONG) yyvsp[-1] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (gds_sqlerr,
					   gds_arg_number, (SLONG) -817,
					   gds_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   gds_arg_number, (SLONG) db_dialect,
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
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					gds_arg_end );

					}
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else 
				if ((SLONG) yyvsp[-1] < 5)
					{
					lex.g_field->fld_dtype = dtype_short; 
					lex.g_field->fld_length = sizeof (SSHORT); 
					}
				else
					{
					lex.g_field->fld_dtype = dtype_long; 
					lex.g_field->fld_length = sizeof (SLONG); 
					}
			lex.g_field->fld_precision = (USHORT)(ULONG) yyvsp[-1];
			}
    break;

  case 546:

    { 
			if ( ((SLONG) yyvsp[-3] < 1) || ((SLONG) yyvsp[-3] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision should be between 1 and 18 */ 
			if (((SLONG) yyvsp[-1] > (SLONG) yyvsp[-3]) || ((SLONG) yyvsp[-1] < 0))
				yyabandon (-842, isc_scale_nogt);
				/* Scale must be between 0 and precision */
			if ((SLONG) yyvsp[-3] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (gds_sqlerr,
					   gds_arg_number, (SLONG) -817,
					   gds_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   gds_arg_number, (SLONG) db_dialect,
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
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					gds_arg_end );
				  }
				  /* client_dialect >= SQL_DIALECT_V6 */
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else
				{
				if ((SLONG) yyvsp[-3] < 5)
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
			lex.g_field->fld_precision = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT)(SLONG) yyvsp[-1];
			}
    break;

  case 549:

    { 
			if ((SLONG) yyvsp[0] > 7)
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

  case 550:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 551:

    { 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;

  case 552:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 553:

    { yyval = yyvsp[-1]; }
    break;

  case 554:

    { yyval = 0; }
    break;

  case 558:

    { 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
    break;

  case 559:

    {
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
    break;

  case 560:

    {
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
    break;

  case 564:

    { yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;

  case 565:

    { yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 566:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 567:

    { yyval = 0; }
    break;

  case 568:

    { yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;

  case 571:

    { yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;

  case 572:

    { yyval = make_node (nod_rollback, 0, NULL); }
    break;

  case 575:

    { yyval = make_node (nod_commit_retain, 0, NULL); }
    break;

  case 576:

    { yyval = NULL; }
    break;

  case 578:

    { yyval = NULL; }
    break;

  case 579:

    {yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;

  case 581:

    { yyval = NULL; }
    break;

  case 583:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 588:

    { yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;

  case 589:

    { yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;

  case 590:

    { yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;

  case 591:

    { yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;

  case 592:

    { yyval = yyvsp[0];}
    break;

  case 594:

    { yyval = yyvsp[0];}
    break;

  case 595:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 596:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 597:

    { yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;

  case 598:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 599:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 600:

    { yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;

  case 601:

    { yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;

  case 602:

    { yyval = 0; }
    break;

  case 603:

    { yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;

  case 604:

    { yyval = (DSQL_NOD) NOD_SHARED; }
    break;

  case 605:

    { yyval = (DSQL_NOD) NOD_PROTECTED ; }
    break;

  case 606:

    { yyval = (DSQL_NOD) 0; }
    break;

  case 607:

    { yyval = (DSQL_NOD) NOD_READ; }
    break;

  case 608:

    { yyval = (DSQL_NOD) NOD_WRITE; }
    break;

  case 610:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 611:

    { yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 612:

    { yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
    break;

  case 613:

    { yyval = 0; }
    break;

  case 615:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 616:

    {yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;

  case 617:

    { yyval = make_node (nod_select, e_select_count, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 618:

    { yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;

  case 619:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 620:

    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 621:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 622:

    { yyval = 0; }
    break;

  case 624:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 625:

    { yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 626:

    { yyval = 0; }
    break;

  case 627:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 628:

    { yyval = 0; }
    break;

  case 629:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 630:

    { yyval = 0; }
    break;

  case 631:

    { yyval = yyvsp[-1]; }
    break;

  case 632:

    { yyval = 0; }
    break;

  case 633:

    { yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;

  case 634:

    { yyval = 0; }
    break;

  case 635:

    { yyval = yyvsp[0]; }
    break;

  case 636:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 637:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 638:

    { yyval = 0; }
    break;

  case 639:

    { yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 640:

    { yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 641:

    { lex.limit_clause = true; }
    break;

  case 642:

    { lex.limit_clause = false; }
    break;

  case 643:

    { lex.first_detection = true; }
    break;

  case 644:

    { lex.first_detection = false; }
    break;

  case 645:

    { yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;

  case 646:

    { yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
    break;

  case 647:

    { yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
    break;

  case 648:

    { yyval = 0; }
    break;

  case 649:

    { yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
    break;

  case 650:

    { yyval = yyvsp[-2]; }
    break;

  case 651:

    { yyval = yyvsp[-1]; }
    break;

  case 652:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 653:

    { yyval = yyvsp[-1]; }
    break;

  case 654:

    { yyval = yyvsp[0]; }
    break;

  case 655:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 656:

    { yyval = 0; }
    break;

  case 657:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 658:

    { yyval = 0; }
    break;

  case 660:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 662:

    { yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 665:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 667:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 671:

    { yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;

  case 673:

    { yyval = NULL; }
    break;

  case 674:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 675:

    { yyval = NULL; }
    break;

  case 677:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 678:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 679:

    { yyval = yyvsp[-1]; }
    break;

  case 680:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;

  case 681:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 682:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 683:

    { yyval = NULL; }
    break;

  case 685:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 689:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 690:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;

  case 691:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 692:

    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 693:

    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 694:

    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 695:

    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 696:

    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 697:

    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 698:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 699:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 700:

    { yyval = 0; }
    break;

  case 702:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 704:

    { yyval = yyvsp[0]; }
    break;

  case 705:

    { yyval = 0; }
    break;

  case 706:

    { yyval = yyvsp[0]; }
    break;

  case 707:

    { yyval = 0; }
    break;

  case 708:

    { yyval = yyvsp[0]; }
    break;

  case 709:

    { yyval = 0; }
    break;

  case 710:

    { yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 711:

    { yyval = 0; }
    break;

  case 712:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 713:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 714:

    { yyval = 0; }
    break;

  case 715:

    { yyval = 0; }
    break;

  case 717:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 718:

    { yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 721:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 722:

    { yyval = make_node (nod_natural, (int) 0, NULL); }
    break;

  case 723:

    { yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;

  case 724:

    { yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 726:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 727:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 728:

    { yyval = 0; }
    break;

  case 729:

    { yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;

  case 730:

    { yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 732:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 735:

    { yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 736:

    { yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 737:

    { yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;

  case 740:

    { yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;

  case 741:

    { yyval = make_node (nod_update, e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;

  case 743:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 744:

    { yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;

  case 747:

    { yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 748:

    { yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 749:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 750:

    { yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;

  case 754:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 755:

    { yyval = yyvsp[0]; }
    break;

  case 757:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 760:

    { yyval = NULL; }
    break;

  case 761:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 763:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 765:

    { yyval = NULL; }
    break;

  case 766:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 768:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 770:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 771:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;

  case 772:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 774:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 776:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 778:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 779:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 781:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 782:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 783:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 784:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 785:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 787:

    { yyval = yyvsp[-1]; }
    break;

  case 788:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 799:

    { yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 800:

    { yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 801:

    { yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 802:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 803:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 804:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 805:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 806:

    { yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 807:

    { yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 808:

    { yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 809:

    { yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 810:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 811:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 812:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 813:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 814:

    { yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 815:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 816:

    { yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 817:

    { yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 818:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 819:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 820:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 821:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 822:

    { yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 825:

    { yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 826:

    { yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 827:

    { yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 828:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 829:

    { yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 830:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 831:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 832:

    { yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 833:

    { yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 834:

    { yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 835:

    { yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 836:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 837:

    { yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 838:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 839:

    { yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;

  case 840:

    { yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;

  case 841:

    { yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;

  case 842:

    { yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;

  case 843:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;

  case 844:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;

  case 845:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;

  case 846:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;

  case 847:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;

  case 848:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;

  case 850:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 851:

    { yyval = yyvsp[-1]; }
    break;

  case 852:

    { yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 853:

    { yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;

  case 863:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 864:

    { yyval = yyvsp[0]; }
    break;

  case 865:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 866:

    { yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 867:

    { yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;

  case 868:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 869:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 870:

    {
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 871:

    { yyval = yyvsp[-1]; }
    break;

  case 872:

    { yyval = yyvsp[-1]; }
    break;

  case 876:

    { yyval = make_node (nod_dbkey, 1, NULL); }
    break;

  case 877:

    { yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;

  case 878:

    { 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;

  case 879:

    { yyval = yyvsp[0]; }
    break;

  case 880:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
    break;

  case 881:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 0, NULL);
			}
    break;

  case 882:

    { yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;

  case 883:

    { yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 885:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 887:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 888:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
    break;

  case 889:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 890:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
    break;

  case 891:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 892:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 894:

    { yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
    break;

  case 895:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DATE);
			}
    break;

  case 896:

    {
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIME); 
			}
    break;

  case 897:

    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;

  case 898:

    { yyval = make_parameter (); }
    break;

  case 899:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 900:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 901:

    { yyval = make_node (nod_current_role, 0, NULL); }
    break;

  case 902:

    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
    break;

  case 903:

    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
    break;

  case 904:

    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
    break;

  case 905:

    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
    break;

  case 906:

    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
    break;

  case 907:

    { yyval = yyvsp[0]; }
    break;

  case 908:

    { ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;

  case 910:

    { yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;

  case 911:

    { if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 912:

    { if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 913:

    { if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;

  case 914:

    { if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 916:

    { yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;

  case 917:

    { yyval = yyvsp[0];}
    break;

  case 922:

    { yyval = make_node (nod_agg_count, 0, NULL); }
    break;

  case 923:

    { yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;

  case 924:

    { yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;

  case 925:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;

  case 926:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 927:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;

  case 928:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 929:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 930:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 931:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 932:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 933:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;

  case 935:

    { yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;

  case 937:

    { yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;

  case 938:

    { yyval = make_node (nod_substr, e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;

  case 939:

    { yyval = yyvsp[0]; }
    break;

  case 940:

    { yyval = MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;

  case 941:

    { yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 942:

    { yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;

  case 943:

    { yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;

  case 946:

    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;

  case 947:

    { yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 950:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 951:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 952:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 953:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 954:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 955:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 956:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 957:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 961:

    { yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
    break;

  case 962:

    { yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
    break;

  case 963:

    { yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
    break;

  case 964:

    { yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
    break;

  case 965:

    { yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
    break;

  case 966:

    { yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
    break;

  case 967:

    { yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
    break;

  case 968:

    { yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
    break;

  case 971:

    { yyval = make_node (nod_null, 0, NULL); }
    break;


    }

/* Line 991 of yacc.c.  */


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
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:

  /* Suppress GCC warning that yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__));
#endif


  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
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
	for (const TOK *token = KEYWORD_getTokens(); token->tok_string; ++token)
	{
		SYM symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) sym;
		symbol->sym_string = (TEXT *) token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		STR str_ = FB_NEW_RPT(*DSQL_permanent_pool, symbol->sym_length) str;
		str_->str_length = symbol->sym_length;
		strncpy((char*)str_->str_data, (char*)symbol->sym_string, symbol->sym_length);
		symbol->sym_object = (void *) str_;
		HSHD_insert(symbol);
	}
}


void LEX_string (
	TEXT	*string,
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
#ifdef DEV_BUILD
	if (DSQL_debug & 32)
		printf("%.*s\n", (int)length, string);
#endif
}


static void check_log_file_attrs (void)
{
/**********************************************
 *
 *	c h e c k _ l o g _ f i l e _ a t t r s
 *
 **********************************************
 *
 * Functional description
 *	Check if log file attributes are valid
 *
 *********************************************/

	if (lex.g_file->fil_partitions) {
		if (!lex.g_file->fil_length) {
			yyabandon (-261, isc_log_length_spec);
			/* Total length of a partitioned log must be specified */
		}
		
		if (PARTITION_SIZE (OneK * lex.g_file->fil_length, lex.g_file->fil_partitions) <
			(OneK*MIN_LOG_LENGTH)) {
			yyabandon (-239, isc_partition_too_small);
			/* Log partition size too small */
		}
	}
	else {
		if ((lex.g_file->fil_length) && (lex.g_file->fil_length < MIN_LOG_LENGTH)) {
			yyabandon (-239, isc_log_too_small);   /* Log size too small */
		}
	}	 
}


static TEXT *lex_position (void)
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
static bool long_int(DSQL_NOD string,
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

	for (const char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol ((char *)((STR) string)->str_data);

	return true;
}
#endif

static DSQL_FLD make_field (DSQL_NOD field_name)
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
	DSQL_FLD	field;
	STR	string;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	if (field_name == NULL)
	{
		field = FB_NEW_RPT(*tdsql->tsql_default, sizeof (INTERNAL_FIELD_NAME)) dsql_fld;
		strcpy (field->fld_name, (TEXT*) INTERNAL_FIELD_NAME);
		return field;
	}
	string = (STR) field_name->nod_arg [1];
	field = FB_NEW_RPT(*tdsql->tsql_default, strlen ((SCHAR*) string->str_data)) dsql_fld;
	strcpy (field->fld_name, (TEXT*) string->str_data);

	return field;
}


static FIL make_file (void)
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
	FIL	temp_file;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;
		   
	temp_file = FB_NEW(*tdsql->tsql_default) fil;

	return temp_file;
}


static DSQL_NOD make_list (DSQL_NOD node)
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
	DSQL_NOD	*ptr;
	DLLS	stack, temp;
	USHORT	l;
	DSQL_NOD	old;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	if (!node)
		return node;

	stack = 0;
	stack_nodes (node, &stack);
	for (l = 0, temp = stack; temp; temp = temp->lls_next)
		l++;

	old  = node;
	node = FB_NEW_RPT(*tdsql->tsql_default, l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	ptr = node->nod_arg + node->nod_count;

	while (stack)
		*--ptr = (DSQL_NOD) LLS_POP (&stack);

	return node;
}


static DSQL_NOD make_parameter (void)
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
	DSQL_NOD	node;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
	node->nod_type = nod_parameter;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = 1;
	node->nod_arg[0] = (DSQL_NOD)(ULONG) lex.param_number++;

	return node;
}


static DSQL_NOD make_node (NOD_TYPE	type,
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
	DSQL_NOD	node, *p;
	va_list	ptr;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	p = node->nod_arg;
	VA_START (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, DSQL_NOD);

	return node;
}


static DSQL_NOD make_flag_node (NOD_TYPE	type,
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
	DSQL_NOD	node, *p;
	va_list	ptr;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	p = node->nod_arg;
	VA_START (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, DSQL_NOD);

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
#if defined(DEV_BUILD)
	DSQL_debug = level;
#endif
	if (level >> 8)
		*yydeb = level >> 8;
	/* CVC: I added this code form Mike Nordell to see the output from internal
	   operations that's generated in DEV build when DEBUG <n> is typed into isql.exe.
	   When n>0, the output console is activated; otherwise it's closed. */
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
	static FILE* redirected_output;
	if (level > 0) {
		/* Console debug code inside this scope */
		if (AllocConsole()) {
			redirected_output = freopen("CONOUT$", "wt", stdout);
			printf("DebugConsole - Yes, it's working.\n");
		}
	}
	else if (level <= 0 && redirected_output) {
		fclose (redirected_output);
		redirected_output = 0;
		FreeConsole();
	}
#endif
}

#ifdef NOT_USED_OR_REPLACED
static bool short_int(DSQL_NOD string,
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

	if (((STR) string)->str_length > 5) {
		return false;
	}

	for (char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return false;
		}
	}

	/* there are 5 or fewer digits, it's value may still be greater
	 * than 32767... */

	SCHAR buf[10];	
	buf [0] = ((STR) string)->str_data[0];
	buf [1] = ((STR) string)->str_data[1];
	buf [2] = ((STR) string)->str_data[2];
	buf [3] = ((STR) string)->str_data[3];
	buf [4] = ((STR) string)->str_data[4];
	buf [5] = '\0';

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

static void stack_nodes (DSQL_NOD	node,
						 DLLS		*stack)
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
	DSQL_NOD	*ptr, *end;
	DSQL_NOD	 curr_node, next_node, start_chain, end_chain, save_link;

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

	start_chain = node;
	end_chain = NULL;
	curr_node = node;
	next_node = node->nod_arg[0];
	while ( curr_node->nod_count == 2 &&
			curr_node->nod_arg[0]->nod_type == nod_list &&
			curr_node->nod_arg[1]->nod_type != nod_list &&
			next_node->nod_arg[0]->nod_type == nod_list &&
			next_node->nod_arg[1]->nod_type != nod_list)
		{

		/* pattern was found so reverse the links and go to next node */

		save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
		}

	/* see if any chain was found */

	if ( end_chain)
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
			save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
			}
		return;
		}

	for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes (*ptr, stack);
}

inline static int yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	BOOLEAN	*stmt_ambiguous)
{
	int temp = lex.yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
	lex.prev_prev_keyword = lex.prev_keyword;
	lex.prev_keyword = temp;
	return temp;
}

int LexerState::yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	BOOLEAN	*stmt_ambiguous)
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
	char* p;
	char* buffer;
	char* buffer_end;
	char* new_buffer;
	SYM	sym;
	SSHORT	c;
	USHORT	buffer_len;

	STR delimited_id_str;

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

		tok_class = classes [c];

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
		p = string;
		for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
		{
		if (ptr >= end)
			return -1;
		CHECK_COPY_INCR(p, UPPER7(*ptr));
		}
		
		CHECK_BOUND(p);
		*p = 0;

		/* make a string value to hold the name, the name 
		 * is resolved in pass1_constant */

		yylval = (DSQL_NOD) (MAKE_string(string, p - string))->str_data;

		return INTRODUCER;
		}

	/* parse a quoted string, being sure to look for double quotes */

	if (tok_class & CHR_QUOTE)
		{
		buffer = string;
		buffer_len = sizeof (string);
		buffer_end = buffer + buffer_len - 1;
		for (p = buffer; ; p++)
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
			new_buffer = (char*)gds__alloc (2 * buffer_len);
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
		*stmt_ambiguous = TRUE; /* string delimited by double quotes could be
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
			yylval = (DSQL_NOD) MAKE_string(buffer, p - buffer);
			delimited_id_str = (STR) yylval;
			delimited_id_str->str_flags |= STR_delimited_id;
			if (buffer != string)
			gds__free (buffer);
			return SYMBOL;
			}
		}
		yylval = (DSQL_NOD) MAKE_string(buffer, p - buffer);
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

	assert(ptr <= end);

	if ((tok_class & CHR_DIGIT) ||
		((c == '.') && (ptr < end) && (classes [*ptr] & CHR_DIGIT)))
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
			if (have_exp_digit && (! (classes [c]  & CHR_DIGIT)))
				/* First non-digit after exponent and digit terminates
				 the token. */
				break;
			else if (have_exp_sign && (! (classes [c]  & CHR_DIGIT)))
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
				else if ( classes [c]  & CHR_DIGIT )
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
			else if (classes [c] & CHR_DIGIT)
			{
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = true;

				if (number >= limit_by_10)
				/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
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
			assert(have_digit);

			if (have_exp_digit)
				{
				yylval = (DSQL_NOD) MAKE_string(last_token, ptr - last_token);
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
				yylval = (DSQL_NOD) (ULONG) number;
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
							   gds_arg_string,
							   ERR_string( last_token,
								   ptr - last_token ),
							   gds_arg_end );
					ERRD_post_warning( isc_dsql_warning_number_ambiguous1,
							   gds_arg_end );
					}

				yylval = (DSQL_NOD) MAKE_string(last_token, ptr - last_token);

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
		p = string;
		CHECK_COPY_INCR(p, UPPER (c));
		for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			CHECK_COPY_INCR(p, UPPER (*ptr));
		}

		CHECK_BOUND(p);
		*p = 0;
		sym = HSHD_lookup (NULL, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
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
				(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
				 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
				 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
			{			
				LexerState savedState = lex;
				int nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
				lex = savedState;
				if (nextToken==OR || nextToken==AND) {
					switch(sym->sym_keyword) {
					case INSERTING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_INSERTING;
					case UPDATING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_UPDATING;
					case DELETING:
						yylval = (DSQL_NOD) sym->sym_object;
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
			if ((sym->sym_keyword == FIRST && !first_detection) || sym->sym_keyword == SKIP) {
				if (prev_keyword == SELECT || limit_clause) {
					LexerState savedState = lex;
					int nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
					lex = savedState;
					if (nextToken != NUMBER && nextToken != '?' && nextToken != '(') {
						yylval = (DSQL_NOD) MAKE_string(string, p - string);
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return SYMBOL;
					} else {
						yylval = (DSQL_NOD) sym->sym_object;
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return sym->sym_keyword;
					}
				} /* else fall down and return token as SYMBOL */
			} else {
				yylval = (DSQL_NOD) sym->sym_object;
				last_token_bk = last_token;
				line_start_bk = line_start;
				lines_bk = lines;
				return sym->sym_keyword;
			}
		}
		yylval = (DSQL_NOD) MAKE_string(string, p - string);
		last_token_bk = last_token;
		line_start_bk = line_start;
		lines_bk = lines;
		return SYMBOL;
	}

	/* Must be punctuation -- test for double character punctuation */

	if (last_token + 1 < end)
	{
		sym = HSHD_lookup (NULL, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
		(prev_keyword=='(' || prev_keyword==NOT || prev_keyword==AND || prev_keyword==OR ||
		 prev_keyword==ON || prev_keyword==HAVING || prev_keyword==WHERE || prev_keyword==WHEN) ) 
	{
		LexerState savedState = lex;	
		brace_analysis = true;
		int openCount = 0;
		int nextToken;
		do {
			openCount++;
			nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
		} while (nextToken == '(');
		DSQL_NOD temp_val = yylval;
		if (nextToken == INSERTING || nextToken == UPDATING || nextToken == DELETING)
		{
			/* Skip closing braces. */
			while ( openCount &&
					yylex(client_dialect,db_dialect,
						  parser_version,stmt_ambiguous) == ')')
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
					(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
					 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
					 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
				{			
					savedState = lex;
					int token = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
					lex = savedState;
					if (token==OR || token==AND) {
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
		} else {
			/* Restore status quo. */
			lex = savedState;
		}
	}

	/* Single character punctuation are simply passed on */

	return c;
}


static void yyerror (
	TEXT	*error_string)
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
		ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
		gds_arg_gds, gds_command_end_err,	/* Unexpected end of command */
		0);
	else
	{
		ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
		gds_arg_gds, gds_dsql_token_unk_err, 
		gds_arg_number, (SLONG) lex.lines, 
		gds_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Token unknown - line %d, char %d */
		gds_arg_gds, gds_random, 
		gds_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token, 0);
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

	ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) sql_code, 
		gds_arg_gds, error_symbol, 0);
}

