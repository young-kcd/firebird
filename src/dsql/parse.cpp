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
     KW_DELETING = 516
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
 *            clashes with normal DEBUG macro.
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
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
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
 */

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <windows.h>
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

/* Can't include ../jrd/err_proto.h here because it pulls jrd.h. */
#if !defined(JRD_ERR_PROTO_H)
extern "C" TEXT *DLL_EXPORT ERR_string(const TEXT*, int);
#endif

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

#define MIN_CACHE_BUFFERS       250
#define DEF_CACHE_BUFFERS       1000

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE		2048
#define YYMAXDEPTH		2048

/* Make bison allocate static stack */
#define YYINITDEPTH 2048
/* #define YYSTACK_USE_ALLOCA 1 */

#define YYSTYPE		DSQL_NOD
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME [] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING [] = "";	

#define TRIGGER_TYPE_SUFFIX(slot1, slot2, slot3) \
	((slot1 << 1) | (slot2 << 3) | (slot3 << 5))

extern "C" {

#ifndef SHLIB_DEFS
DSQL_NOD		DSQL_parse;
#else
extern DSQL_NOD	DSQL_parse;
#endif

}	// extern "C"
static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

#define MAX_TOKEN_LEN   256
#define CHECK_BOUND(to)\
    {\
    if ((to - string) >= MAX_TOKEN_LEN)        \
	yyabandon (-104, isc_token_too_long); \
    }
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static BOOLEAN	long_int (DSQL_NOD, SLONG *);
#endif
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static BOOLEAN	short_int (DSQL_NOD, SLONG *, SSHORT);
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
#line 834 "y.tab.c"

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
#define YYFINAL  157
#define YYLAST   6797

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  278
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  416
/* YYNRULES -- Number of rules. */
#define YYNRULES  996
/* YYNRULES -- Number of states. */
#define YYNSTATES  1843

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   516

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     271,   272,   267,   265,   270,   266,   276,   268,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   273,   269,
     263,   262,   264,   277,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   274,     2,   275,     2,     2,     2,     2,     2,     2,
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
     255,   256,   257,   258,   259,   260,   261
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
     542,   546,   550,   554,   556,   558,   565,   569,   571,   575,
     577,   581,   584,   586,   587,   590,   594,   596,   597,   599,
     601,   604,   608,   613,   614,   616,   619,   620,   622,   624,
     630,   636,   640,   643,   644,   646,   650,   652,   654,   661,
     665,   668,   675,   678,   680,   683,   686,   689,   690,   692,
     694,   697,   698,   701,   702,   704,   706,   708,   710,   712,
     714,   715,   717,   719,   722,   725,   728,   734,   736,   739,
     743,   746,   749,   750,   752,   754,   756,   758,   762,   767,
     776,   781,   782,   789,   791,   793,   796,   799,   800,   804,
     808,   810,   813,   816,   819,   828,   837,   846,   855,   859,
     860,   863,   864,   866,   870,   873,   875,   876,   878,   881,
     888,   890,   891,   894,   896,   898,   900,   904,   906,   909,
     910,   912,   915,   918,   921,   923,   925,   927,   929,   931,
     933,   936,   938,   940,   943,   945,   947,   949,   952,   955,
     957,   961,   966,   969,   976,   981,   983,   985,   993,  1002,
    1009,  1018,  1025,  1030,  1031,  1036,  1039,  1041,  1045,  1046,
    1049,  1054,  1055,  1057,  1059,  1063,  1067,  1074,  1077,  1080,
    1084,  1085,  1087,  1090,  1095,  1097,  1101,  1104,  1107,  1110,
    1112,  1117,  1125,  1133,  1135,  1137,  1141,  1146,  1154,  1157,
    1159,  1163,  1165,  1167,  1174,  1178,  1179,  1180,  1181,  1182,
    1186,  1187,  1197,  1207,  1209,  1211,  1212,  1215,  1217,  1219,
    1221,  1223,  1225,  1229,  1233,  1237,  1241,  1245,  1249,  1255,
    1261,  1267,  1273,  1279,  1285,  1288,  1289,  1294,  1297,  1301,
    1305,  1308,  1311,  1315,  1319,  1322,  1326,  1333,  1335,  1338,
    1343,  1347,  1350,  1353,  1356,  1359,  1363,  1365,  1369,  1373,
    1377,  1380,  1383,  1388,  1393,  1399,  1401,  1403,  1405,  1407,
    1409,  1411,  1413,  1415,  1417,  1419,  1421,  1423,  1425,  1427,
    1429,  1431,  1433,  1435,  1437,  1439,  1441,  1443,  1445,  1447,
    1449,  1452,  1455,  1458,  1460,  1462,  1464,  1465,  1468,  1471,
    1472,  1474,  1477,  1480,  1483,  1486,  1489,  1491,  1495,  1503,
    1505,  1506,  1508,  1509,  1512,  1515,  1518,  1521,  1524,  1527,
    1530,  1533,  1536,  1540,  1543,  1546,  1549,  1551,  1553,  1555,
    1557,  1562,  1568,  1570,  1574,  1576,  1580,  1582,  1585,  1587,
    1589,  1591,  1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,
    1612,  1617,  1624,  1630,  1634,  1635,  1638,  1641,  1642,  1646,
    1647,  1652,  1654,  1660,  1665,  1667,  1672,  1674,  1677,  1680,
    1682,  1684,  1686,  1689,  1692,  1695,  1698,  1700,  1701,  1705,
    1711,  1713,  1715,  1718,  1722,  1724,  1727,  1731,  1732,  1734,
    1736,  1738,  1744,  1750,  1757,  1759,  1761,  1763,  1766,  1771,
    1773,  1774,  1780,  1782,  1783,  1787,  1790,  1792,  1793,  1796,
    1797,  1799,  1800,  1804,  1806,  1807,  1809,  1812,  1814,  1816,
    1818,  1820,  1823,  1826,  1828,  1831,  1835,  1837,  1839,  1843,
    1847,  1849,  1852,  1856,  1858,  1861,  1862,  1865,  1867,  1869,
    1870,  1872,  1874,  1876,  1880,  1883,  1887,  1888,  1890,  1894,
    1899,  1904,  1906,  1910,  1915,  1919,  1920,  1922,  1926,  1930,
    1932,  1934,  1935,  1937,  1939,  1944,  1945,  1949,  1950,  1953,
    1954,  1957,  1958,  1968,  1979,  1980,  1981,  1982,  1983,  1987,
    1990,  1992,  1993,  1997,  2003,  2007,  2010,  2016,  2019,  2021,
    2023,  2025,  2027,  2029,  2033,  2035,  2038,  2042,  2045,  2047,
    2051,  2053,  2055,  2062,  2066,  2070,  2073,  2077,  2078,  2080,
    2084,  2086,  2088,  2090,  2093,  2095,  2097,  2099,  2102,  2104,
    2107,  2109,  2112,  2113,  2117,  2118,  2120,  2124,  2126,  2128,
    2130,  2132,  2136,  2138,  2140,  2142,  2145,  2146,  2149,  2150,
    2153,  2154,  2159,  2161,  2164,  2166,  2168,  2169,  2171,  2175,
    2178,  2180,  2182,  2185,  2187,  2192,  2195,  2197,  2201,  2210,
    2216,  2218,  2222,  2224,  2226,  2231,  2236,  2241,  2243,  2245,
    2251,  2257,  2259,  2263,  2267,  2269,  2271,  2279,  2287,  2293,
    2297,  2298,  2300,  2302,  2304,  2307,  2308,  2310,  2312,  2314,
    2315,  2319,  2321,  2325,  2327,  2328,  2332,  2334,  2338,  2340,
    2344,  2348,  2350,  2352,  2356,  2358,  2361,  2363,  2367,  2371,
    2373,  2376,  2380,  2384,  2388,  2392,  2394,  2398,  2401,  2403,
    2405,  2407,  2409,  2411,  2413,  2415,  2417,  2419,  2421,  2425,
    2429,  2433,  2437,  2441,  2445,  2449,  2453,  2460,  2467,  2474,
    2481,  2488,  2495,  2502,  2509,  2516,  2523,  2530,  2537,  2544,
    2551,  2558,  2565,  2567,  2569,  2575,  2582,  2586,  2591,  2597,
    2604,  2608,  2613,  2617,  2622,  2626,  2631,  2636,  2642,  2647,
    2652,  2656,  2661,  2663,  2665,  2667,  2669,  2671,  2673,  2675,
    2679,  2683,  2694,  2705,  2707,  2709,  2711,  2713,  2715,  2717,
    2719,  2721,  2723,  2726,  2729,  2733,  2737,  2741,  2745,  2749,
    2753,  2757,  2761,  2763,  2765,  2767,  2769,  2773,  2775,  2777,
    2779,  2781,  2783,  2788,  2790,  2794,  2796,  2799,  2801,  2803,
    2805,  2807,  2809,  2811,  2813,  2816,  2819,  2822,  2824,  2826,
    2828,  2830,  2832,  2834,  2836,  2838,  2840,  2842,  2845,  2847,
    2850,  2852,  2854,  2856,  2858,  2860,  2863,  2865,  2867,  2869,
    2871,  2873,  2878,  2884,  2890,  2896,  2902,  2908,  2914,  2920,
    2926,  2932,  2938,  2945,  2952,  2959,  2968,  2973,  2978,  2982,
    2989,  2991,  2993,  3000,  3007,  3009,  3011,  3016,  3023,  3028,
    3034,  3038,  3044,  3049,  3055,  3057,  3059,  3061,  3063,  3065,
    3067,  3069,  3071,  3073,  3075,  3077,  3079,  3080,  3082,  3084,
    3086,  3088,  3090,  3092,  3094,  3096,  3098,  3100,  3102,  3104,
    3106,  3108,  3110,  3112,  3114,  3116,  3118,  3120,  3122,  3124,
    3126,  3128,  3130,  3132,  3134,  3136,  3138,  3140,  3142,  3144,
    3146,  3148,  3150,  3152,  3154,  3156,  3158
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     279,     0,    -1,   280,    -1,   280,   269,    -1,   456,    -1,
     595,    -1,   511,    -1,   310,    -1,   300,    -1,   585,    -1,
     478,    -1,   281,    -1,   583,    -1,   433,    -1,   312,    -1,
     314,    -1,   290,    -1,   512,    -1,   505,    -1,   534,    -1,
     503,    -1,   589,    -1,    43,   642,    -1,    73,   283,   129,
     282,   182,   294,   287,    -1,    73,   285,   129,   148,   289,
     182,   294,   287,    -1,    73,   283,   129,   282,   182,   292,
      -1,    73,   285,   129,   148,   289,   182,   292,    -1,    73,
     296,   182,   298,   288,    -1,   567,    -1,   180,   567,    -1,
       6,    -1,     6,   147,    -1,   284,    -1,   286,    -1,   284,
     270,   286,    -1,    59,    -1,   163,    -1,    84,    -1,    47,
      -1,   188,   600,    -1,   153,   600,    -1,   200,    73,   131,
      -1,    -1,   200,   210,   131,    -1,    -1,   683,    -1,   158,
     291,   283,   129,   282,    66,   294,    -1,   158,   291,   285,
     129,   148,   289,    66,   294,    -1,   158,   283,   129,   282,
      66,   294,    -1,   158,   285,   129,   148,   289,    66,   294,
      -1,   158,   283,   129,   282,    66,   292,    -1,   158,   285,
     129,   148,   289,    66,   292,    -1,   158,   296,    66,   298,
      -1,    73,   131,    64,    -1,   293,    -1,   292,   270,   293,
      -1,   292,   270,   295,    -1,   294,   270,   293,    -1,   148,
     683,    -1,   184,   687,    -1,   195,   690,    -1,   214,   684,
      -1,   295,    -1,   294,   270,   295,    -1,   688,    -1,   189,
     688,    -1,    74,   688,    -1,   297,    -1,   296,   270,   297,
      -1,   684,    -1,   299,    -1,   298,   270,   299,    -1,   688,
      -1,   189,   688,    -1,    45,   301,    -1,    63,   309,    -1,
      62,    68,   302,    -1,   669,   304,   157,   307,    55,   641,
     117,   641,    -1,   485,    -1,    19,    -1,    37,   271,   645,
     272,   491,    -1,    -1,   305,    -1,   271,   305,   272,    -1,
     306,    -1,   305,   270,   306,    -1,   373,   303,    -1,   373,
     303,    20,   238,    -1,   308,    -1,   271,   308,   272,    -1,
     373,   303,    -1,   373,   303,   212,    -1,   373,   303,    20,
     101,    -1,   373,   303,    20,   238,    -1,   140,   645,    -1,
     678,    82,   490,   135,   490,    55,   641,   117,   641,    -1,
      36,   311,    -1,    58,   677,   641,    -1,   316,   539,    83,
     681,   129,   567,   317,    -1,   148,   392,    -1,   180,   360,
      -1,   184,   448,    -1,   195,   434,    -1,    71,   333,    -1,
      40,   335,    -1,    51,   324,    -1,   165,   318,    -1,   214,
     334,    -1,   237,   313,    -1,   148,   393,    -1,   180,   361,
      -1,   195,   435,    -1,    36,   132,     7,   315,    -1,   148,
     394,    -1,   184,   449,    -1,   187,    -1,    -1,   602,    -1,
     601,    -1,   367,   271,   445,   630,   446,   272,    -1,   645,
     319,   320,   641,   321,   322,    -1,   111,    -1,    14,    -1,
      -1,    -1,    32,    -1,    -1,   102,   336,   648,   359,    -1,
      -1,   323,    -1,   348,    -1,   323,   348,    -1,   370,   325,
     480,   445,   326,   446,   327,   369,    -1,    10,    -1,    -1,
      46,   445,   375,    -1,    -1,    -1,   328,    -1,   329,    -1,
     328,   329,    -1,   330,    -1,   331,    -1,   332,    -1,   124,
      98,    -1,   445,    24,   271,   609,   272,   446,    -1,   680,
      -1,   684,    -1,   337,   338,   341,    -1,    -1,   262,    -1,
     641,    -1,    -1,   339,    -1,   340,    -1,   339,   340,    -1,
     139,   336,   645,    -1,   102,   336,   648,   359,    -1,   189,
     641,    -1,   141,   641,    -1,   164,   118,   641,    -1,    -1,
     342,    -1,   343,    -1,   342,   343,    -1,   348,    -1,   345,
      -1,   344,    -1,    46,    23,   164,   671,    -1,    75,   336,
     648,    -1,    25,   336,   648,    -1,   127,   336,   645,    -1,
     109,   336,   646,    -1,   347,    -1,   346,    -1,   103,   271,
     349,   272,   136,   350,    -1,   103,    15,   350,    -1,   103,
      -1,   354,   641,   355,    -1,   350,    -1,   349,   270,   350,
      -1,   351,   352,    -1,   641,    -1,    -1,   352,   353,    -1,
     168,   336,   648,    -1,    94,    -1,    -1,   356,    -1,   357,
      -1,   356,   357,    -1,   175,   358,   648,    -1,   102,   336,
     648,   359,    -1,    -1,    12,    -1,    12,   137,    -1,    -1,
     137,    -1,   138,    -1,   567,   362,   271,   363,   272,    -1,
     567,   362,   271,   363,   272,    -1,    62,    94,   641,    -1,
      62,   641,    -1,    -1,   364,    -1,   363,   270,   364,    -1,
     365,    -1,   380,    -1,   370,   368,   374,   446,   376,   369,
      -1,   370,   481,   366,    -1,   370,   366,    -1,   367,   271,
     445,   630,   446,   272,    -1,    30,    20,    -1,    30,    -1,
     480,   445,    -1,   607,   443,    -1,    26,   672,    -1,    -1,
     607,    -1,   607,    -1,   373,   480,    -1,    -1,    46,   375,
      -1,    -1,   634,    -1,   638,    -1,   639,    -1,   640,    -1,
     668,    -1,   631,    -1,    -1,   377,    -1,   378,    -1,   377,
     378,    -1,   381,   379,    -1,   124,    98,    -1,   153,   567,
     600,   388,   386,    -1,   387,    -1,   187,   386,    -1,   146,
      90,   386,    -1,   381,   382,    -1,    33,   674,    -1,    -1,
     383,    -1,   384,    -1,   385,    -1,   387,    -1,   187,   601,
     386,    -1,   146,    90,   601,   386,    -1,    65,    90,   601,
     153,   567,   600,   388,   386,    -1,   247,   539,    83,   681,
      -1,    -1,   445,    24,   271,   609,   272,   446,    -1,   389,
      -1,   390,    -1,   390,   389,    -1,   389,   390,    -1,    -1,
     129,   188,   391,    -1,   129,    47,   391,    -1,   211,    -1,
     164,    46,    -1,   164,    98,    -1,   123,   209,    -1,   683,
     396,   397,    10,   443,   400,   406,   446,    -1,   683,   396,
     397,    10,   443,   400,   406,   446,    -1,   683,   396,   397,
      10,   443,   400,   406,   446,    -1,   683,   396,   397,    10,
     443,   400,   406,   446,    -1,   271,   398,   272,    -1,    -1,
     157,   396,    -1,    -1,   399,    -1,   398,   270,   399,    -1,
     371,   481,    -1,   401,    -1,    -1,   402,    -1,   401,   402,
      -1,    45,   403,   370,   481,   404,   269,    -1,   192,    -1,
      -1,   262,   375,    -1,   374,    -1,   409,    -1,   406,    -1,
      17,   407,    54,    -1,   408,    -1,   408,   429,    -1,    -1,
     405,    -1,   408,   405,    -1,   593,   269,    -1,   585,   269,
      -1,   410,    -1,   411,    -1,   412,    -1,   413,    -1,   415,
      -1,   418,    -1,   583,   269,    -1,   419,    -1,   421,    -1,
     589,   269,    -1,   426,    -1,   416,    -1,   417,    -1,   178,
     269,    -1,    61,   269,    -1,   427,    -1,    58,   677,   269,
      -1,    58,   677,   630,   269,    -1,    58,   269,    -1,    59,
     148,   683,   423,   424,   269,    -1,    59,   414,   630,   269,
      -1,   191,    -1,   254,    -1,    64,   534,    86,   425,   428,
      50,   405,    -1,    64,    59,   414,   630,    86,   425,    50,
     405,    -1,    59,   414,   630,    86,   425,   269,    -1,    78,
     271,   609,   272,   181,   405,    53,   405,    -1,    78,   271,
     609,   272,   181,   405,    -1,   144,   630,   420,   269,    -1,
      -1,   534,    86,   425,   269,    -1,   273,   689,    -1,   564,
      -1,   271,   564,   272,    -1,    -1,   156,   425,    -1,   156,
     271,   425,   272,    -1,    -1,   422,    -1,   606,    -1,   425,
     270,   606,    -1,   425,   270,   422,    -1,   199,   271,   609,
     272,    50,   405,    -1,   235,   269,    -1,   255,   269,    -1,
      10,    39,   675,    -1,    -1,   430,    -1,   429,   430,    -1,
     197,   431,    50,   405,    -1,   432,    -1,   431,   270,   432,
      -1,   173,   642,    -1,    69,   679,    -1,    58,   677,    -1,
       9,    -1,    59,   148,   683,   423,    -1,   690,   600,    10,
     443,   436,   447,   444,    -1,   690,   600,    10,   443,   436,
     447,   444,    -1,   437,    -1,   438,    -1,   437,   186,   438,
      -1,   437,   186,     6,   438,    -1,   163,   554,   555,   439,
     574,   569,   573,    -1,    66,   440,    -1,   441,    -1,   440,
     270,   441,    -1,   442,    -1,   566,    -1,   441,   568,    89,
     441,   129,   609,    -1,   271,   442,   272,    -1,    -1,    -1,
      -1,    -1,   200,    24,   131,    -1,    -1,   687,    64,   567,
     450,   451,   454,   445,   455,   446,    -1,   687,    64,   567,
     450,   451,   454,   445,   455,   446,    -1,     3,    -1,    80,
      -1,    -1,   452,   453,    -1,    16,    -1,     5,    -1,    84,
      -1,   188,    -1,    47,    -1,    84,   132,   188,    -1,    84,
     132,    47,    -1,   188,   132,    84,    -1,   188,   132,    47,
      -1,    47,   132,    84,    -1,    47,   132,   188,    -1,    84,
     132,   188,   132,    47,    -1,    84,   132,    47,   132,   188,
      -1,   188,   132,    84,   132,    47,    -1,   188,   132,    47,
     132,    84,    -1,    47,   132,    84,   132,   188,    -1,    47,
     132,   188,   132,    84,    -1,   143,   643,    -1,    -1,    10,
     445,   400,   406,    -1,     7,   457,    -1,    58,   677,   641,
      -1,   180,   567,   462,    -1,   184,   475,    -1,   148,   395,
      -1,    40,   471,   472,    -1,    51,   464,   460,    -1,    83,
     470,    -1,    46,   445,   375,    -1,    24,   445,   271,   609,
     272,   446,    -1,   461,    -1,   460,   461,    -1,   164,   445,
     458,   446,    -1,     4,    33,   459,    -1,     4,   332,    -1,
      52,    46,    -1,    52,    33,    -1,   182,   607,    -1,   216,
     373,   481,    -1,   463,    -1,   462,   270,   463,    -1,    52,
     607,   469,    -1,    52,    33,   674,    -1,     4,   365,    -1,
       4,   380,    -1,   466,   607,   143,   645,    -1,   466,   464,
     182,   607,    -1,   466,   468,   216,   467,   446,    -1,   465,
      -1,   692,    -1,   215,    -1,   216,    -1,   217,    -1,   218,
      -1,   219,    -1,   220,    -1,   221,    -1,   222,    -1,   223,
      -1,   224,    -1,   225,    -1,   226,    -1,   227,    -1,   228,
      -1,   229,    -1,   230,    -1,   233,    -1,   234,    -1,   241,
      -1,   242,    -1,   250,    -1,   252,    -1,     7,    -1,     7,
     215,    -1,   481,   445,    -1,   607,   443,    -1,   607,    -1,
     213,    -1,   211,    -1,    -1,   681,     3,    -1,   681,    80,
      -1,    -1,   473,    -1,   472,   473,    -1,     4,   323,    -1,
      52,   103,    -1,   164,   474,    -1,     4,   345,    -1,   344,
      -1,   474,   270,   344,    -1,   687,   450,   476,   454,   445,
     477,   446,    -1,   451,    -1,    -1,   455,    -1,    -1,    52,
     479,    -1,    58,   677,    -1,    83,   681,    -1,   148,   683,
      -1,   180,   686,    -1,   184,   687,    -1,   195,   690,    -1,
      63,   678,    -1,    51,   676,    -1,    62,    68,   669,    -1,
     165,   645,    -1,   214,   684,    -1,    71,   680,    -1,   481,
      -1,   482,    -1,   485,    -1,   488,    -1,   486,   274,   483,
     275,    -1,   493,   274,   483,   275,   491,    -1,   484,    -1,
     483,   270,   484,    -1,   647,    -1,   647,   273,   647,    -1,
     486,    -1,   493,   491,    -1,   492,    -1,   497,    -1,   501,
      -1,   243,    -1,   487,    -1,   169,    -1,    41,    -1,   226,
      -1,   227,    -1,    85,    -1,    96,    -1,    19,   490,   489,
     491,    -1,    19,   271,   646,   272,    -1,    19,   271,   646,
     270,   642,   272,    -1,    19,   271,   270,   642,   272,    -1,
     162,   168,   646,    -1,    -1,   177,   642,    -1,   177,   670,
      -1,    -1,    23,   164,   671,    -1,    -1,   496,   271,   645,
     272,    -1,   496,    -1,   496,   193,   271,   645,   272,    -1,
     495,   271,   645,   272,    -1,   495,    -1,   494,   271,   645,
     272,    -1,   191,    -1,    23,   193,    -1,    91,   193,    -1,
      23,    -1,    91,    -1,   121,    -1,   119,    23,    -1,   119,
      91,    -1,    99,   499,    -1,   500,   499,    -1,   645,    -1,
      -1,   271,   647,   272,    -1,   271,   647,   270,   647,   272,
      -1,    44,    -1,    92,    -1,    95,   502,    -1,    97,    95,
     502,    -1,   152,    -1,    93,   145,    -1,   271,   643,   272,
      -1,    -1,   516,    -1,   504,    -1,   533,    -1,   164,    71,
     680,   182,   647,    -1,   164,    71,   680,   182,   231,    -1,
     164,    71,   680,   182,   266,   231,    -1,   506,    -1,   507,
      -1,   509,    -1,   252,   691,    -1,   253,   252,   691,   508,
      -1,   130,    -1,    -1,   161,   513,   182,   510,   691,    -1,
     252,    -1,    -1,    28,   513,   514,    -1,   161,   513,    -1,
     201,    -1,    -1,   155,   515,    -1,    -1,   170,    -1,    -1,
     164,   183,   517,    -1,   518,    -1,    -1,   519,    -1,   518,
     519,    -1,   520,    -1,   521,    -1,   522,    -1,   526,    -1,
     151,   130,    -1,   151,   202,    -1,   196,    -1,   123,   196,
      -1,    88,   107,   523,    -1,   523,    -1,   524,    -1,   151,
     185,   525,    -1,   151,    29,   525,    -1,   170,    -1,   170,
     180,    -1,   170,   180,   174,    -1,   194,    -1,   123,   194,
      -1,    -1,   154,   529,    -1,   166,    -1,   149,    -1,    -1,
     151,    -1,   202,    -1,   530,    -1,   529,   270,   530,    -1,
     532,   531,    -1,    64,   527,   528,    -1,    -1,   567,    -1,
     532,   270,   567,    -1,   164,   176,    83,   681,    -1,   535,
     536,   542,   544,    -1,   545,    -1,   535,   186,   545,    -1,
     535,   186,     6,   545,    -1,   133,    20,   537,    -1,    -1,
     538,    -1,   537,   270,   538,    -1,   630,   539,   541,    -1,
      11,    -1,    48,    -1,    -1,   239,    -1,   249,    -1,   248,
     549,   540,   550,    -1,    -1,    64,   188,   543,    -1,    -1,
     128,   602,    -1,    -1,   200,   251,    -1,    -1,   163,   551,
     554,   555,   558,   574,   569,   573,   575,    -1,   163,   551,
     554,   555,   558,   574,   569,   573,   575,   536,    -1,    -1,
      -1,    -1,    -1,   552,   553,   548,    -1,   552,   548,    -1,
     553,    -1,    -1,   239,   648,   547,    -1,   239,   271,   630,
     272,   547,    -1,   239,   637,   547,    -1,   240,   648,    -1,
     240,   271,   548,   630,   272,    -1,   240,   637,    -1,    49,
      -1,   667,    -1,   556,    -1,   267,    -1,   557,    -1,   556,
     270,   557,    -1,   594,    -1,   594,   682,    -1,   594,    10,
     682,    -1,    66,   559,    -1,   560,    -1,   559,   270,   560,
      -1,   561,    -1,   562,    -1,   560,   568,    89,   560,   129,
     609,    -1,   271,   561,   272,    -1,   683,   563,   685,    -1,
     683,   563,    -1,   271,   564,   272,    -1,    -1,   565,    -1,
     564,   270,   565,    -1,   668,    -1,   630,    -1,   567,    -1,
     686,   685,    -1,   686,    -1,    81,    -1,   105,    -1,   105,
     134,    -1,   159,    -1,   159,   134,    -1,    67,    -1,    67,
     134,    -1,    -1,    74,    20,   570,    -1,    -1,   571,    -1,
     570,   270,   571,    -1,   606,    -1,   498,    -1,   654,    -1,
     572,    -1,   606,    26,   672,    -1,   652,    -1,   653,    -1,
     656,    -1,    77,   609,    -1,    -1,   198,   609,    -1,    -1,
     142,   576,    -1,    -1,   577,   271,   578,   272,    -1,    89,
      -1,   172,   114,    -1,   114,    -1,   172,    -1,    -1,   579,
      -1,   579,   270,   578,    -1,   580,   581,    -1,   576,    -1,
     686,    -1,   686,   580,    -1,   120,    -1,    83,   271,   582,
     272,    -1,   133,   681,    -1,   681,    -1,   681,   270,   582,
      -1,    84,    86,   567,   603,   190,   271,   584,   272,    -1,
      84,    86,   567,   603,   546,    -1,   594,    -1,   584,   270,
     594,    -1,   586,    -1,   587,    -1,    47,    66,   566,   574,
      -1,    47,    66,   566,   588,    -1,   198,    38,   128,   675,
      -1,   590,    -1,   591,    -1,   188,   566,   164,   592,   574,
      -1,   188,   566,   164,   592,   588,    -1,   593,    -1,   592,
     270,   593,    -1,   608,   262,   594,    -1,   630,    -1,   668,
      -1,   151,    19,   607,    66,   567,   596,   598,    -1,    84,
      19,   607,    86,   567,   596,   598,    -1,    63,    66,   597,
     182,   597,    -1,    63,   182,   597,    -1,    -1,   490,    -1,
     637,    -1,   642,    -1,   113,   599,    -1,    -1,   646,    -1,
     637,    -1,   601,    -1,    -1,   271,   602,   272,    -1,   607,
      -1,   602,   270,   607,    -1,   604,    -1,    -1,   271,   605,
     272,    -1,   608,    -1,   605,   270,   608,    -1,   607,    -1,
     685,   276,   673,    -1,   685,   276,   267,    -1,   673,    -1,
     607,    -1,   685,   276,   673,    -1,   624,    -1,   124,   624,
      -1,   611,    -1,   609,   132,   609,    -1,   609,     8,   609,
      -1,   611,    -1,   124,   624,    -1,   610,   132,   609,    -1,
     610,     8,   609,    -1,   625,   132,   609,    -1,   625,     8,
     609,    -1,   612,    -1,   271,   610,   272,    -1,   124,   611,
      -1,   613,    -1,   616,    -1,   617,    -1,   618,    -1,   623,
      -1,   614,    -1,   621,    -1,   619,    -1,   620,    -1,   622,
      -1,   630,   262,   630,    -1,   630,   263,   630,    -1,   630,
     264,   630,    -1,   630,    70,   630,    -1,   630,   106,   630,
      -1,   630,   125,   630,    -1,   630,   126,   630,    -1,   630,
     122,   630,    -1,   630,   262,     6,   271,   628,   272,    -1,
     630,   263,     6,   271,   628,   272,    -1,   630,   264,     6,
     271,   628,   272,    -1,   630,    70,     6,   271,   628,   272,
      -1,   630,   106,     6,   271,   628,   272,    -1,   630,   125,
       6,   271,   628,   272,    -1,   630,   126,     6,   271,   628,
     272,    -1,   630,   122,     6,   271,   628,   272,    -1,   630,
     262,   615,   271,   628,   272,    -1,   630,   263,   615,   271,
     628,   272,    -1,   630,   264,   615,   271,   628,   272,    -1,
     630,    70,   615,   271,   628,   272,    -1,   630,   106,   615,
     271,   628,   272,    -1,   630,   125,   615,   271,   628,   272,
      -1,   630,   126,   615,   271,   628,   272,    -1,   630,   122,
     615,   271,   628,   272,    -1,   171,    -1,     9,    -1,   630,
      18,   630,     8,   630,    -1,   630,   124,    18,   630,     8,
     630,    -1,   630,   108,   630,    -1,   630,   124,   108,   630,
      -1,   630,   108,   630,    57,   630,    -1,   630,   124,   108,
     630,    57,   630,    -1,   630,    79,   626,    -1,   630,   124,
      79,   626,    -1,   630,    34,   630,    -1,   630,   124,    34,
     630,    -1,   630,   175,   630,    -1,   630,   124,   175,   630,
      -1,   630,   175,   200,   630,    -1,   630,   124,   175,   200,
     630,    -1,    60,   271,   546,   272,    -1,   167,   271,   546,
     272,    -1,   630,    87,    98,    -1,   630,    87,   124,    98,
      -1,   256,    -1,   257,    -1,   258,    -1,   259,    -1,   260,
      -1,   261,    -1,   627,    -1,   271,   633,   272,    -1,   271,
     628,   272,    -1,   163,   551,   554,   630,   558,   574,   569,
     573,   575,   536,    -1,   163,   551,   554,   630,   558,   574,
     569,   573,   575,   536,    -1,   606,    -1,   632,    -1,   649,
      -1,   636,    -1,   637,    -1,   422,    -1,   655,    -1,   656,
      -1,   654,    -1,   266,   630,    -1,   265,   630,    -1,   630,
     265,   630,    -1,   630,    31,   630,    -1,   630,    26,   672,
      -1,   630,   266,   630,    -1,   630,   267,   630,    -1,   630,
     268,   630,    -1,   271,   630,   272,    -1,   271,   629,   272,
      -1,   638,    -1,   639,    -1,   640,    -1,    42,    -1,   685,
     276,    42,    -1,   101,    -1,   631,    -1,   228,    -1,   229,
      -1,   230,    -1,   606,   274,   633,   275,    -1,   630,    -1,
     633,   270,   630,    -1,   636,    -1,   266,   635,    -1,   205,
      -1,   204,    -1,   203,    -1,   231,    -1,   232,    -1,   635,
      -1,   641,    -1,    41,   207,    -1,   226,   207,    -1,   227,
     207,    -1,   277,    -1,   189,    -1,   233,    -1,   234,    -1,
     241,    -1,   242,    -1,    69,    -1,   173,    -1,   250,    -1,
     207,    -1,   208,   207,    -1,   643,    -1,   266,   644,    -1,
     204,    -1,   204,    -1,   643,    -1,   204,    -1,   648,    -1,
     266,   648,    -1,   204,    -1,   650,    -1,   651,    -1,   652,
      -1,   653,    -1,    35,   271,   267,   272,    -1,    35,   271,
     667,   630,   272,    -1,    35,   271,    49,   630,   272,    -1,
     179,   271,   667,   630,   272,    -1,   179,   271,    49,   630,
     272,    -1,    13,   271,   667,   630,   272,    -1,    13,   271,
      49,   630,   272,    -1,   116,   271,   667,   630,   272,    -1,
     116,   271,    49,   630,   272,    -1,   112,   271,   667,   630,
     272,    -1,   112,   271,    49,   630,   272,    -1,    72,   271,
     680,   270,   630,   272,    -1,   217,   271,   666,    66,   630,
     272,    -1,   236,   271,   630,    66,   645,   272,    -1,   236,
     271,   630,    66,   645,    64,   643,   272,    -1,   100,   271,
     630,   272,    -1,   669,   271,   633,   272,    -1,   669,   271,
     272,    -1,    22,   271,   594,    10,   372,   272,    -1,   657,
      -1,   658,    -1,   245,   271,   630,   270,   630,   272,    -1,
     246,   271,   565,   270,   564,   272,    -1,   659,    -1,   661,
      -1,   244,   664,   660,    54,    -1,   244,   664,   660,    53,
     665,    54,    -1,   197,   663,   181,   665,    -1,   660,   197,
     663,   181,   665,    -1,   244,   662,    54,    -1,   244,   662,
      53,   665,    54,    -1,   197,   609,   181,   665,    -1,   662,
     197,   609,   181,   665,    -1,   630,    -1,   630,    -1,   565,
      -1,   218,    -1,   219,    -1,   220,    -1,   221,    -1,   222,
      -1,   223,    -1,   224,    -1,   225,    -1,     6,    -1,    -1,
      98,    -1,   206,    -1,   692,    -1,   692,    -1,   692,    -1,
     692,    -1,   692,    -1,   692,    -1,   692,    -1,   692,    -1,
     692,    -1,   692,    -1,   692,    -1,   692,    -1,   692,    -1,
     692,    -1,   692,    -1,   692,    -1,   692,    -1,   692,    -1,
     692,    -1,   692,    -1,   692,    -1,   692,    -1,   206,    -1,
     693,    -1,   235,    -1,   238,    -1,   236,    -1,   246,    -1,
     249,    -1,   255,    -1,   251,    -1,   245,    -1,   248,    -1,
     254,    -1,   247,    -1,   256,    -1,   257,    -1,   258,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   529,   529,   531,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   561,   565,   569,   573,   577,   582,   583,
     587,   589,   591,   595,   596,   600,   604,   606,   608,   610,
     612,   616,   618,   622,   624,   628,   635,   640,   645,   650,
     655,   660,   665,   671,   675,   676,   678,   680,   684,   686,
     688,   690,   694,   695,   702,   704,   706,   710,   711,   715,
     719,   720,   724,   726,   733,   737,   739,   744,   750,   751,
     753,   759,   761,   762,   766,   767,   773,   776,   782,   783,
     786,   789,   793,   797,   800,   805,   814,   818,   821,   824,
     826,   828,   830,   832,   834,   836,   838,   840,   845,   849,
     851,   853,   863,   867,   869,   880,   882,   886,   888,   889,
     895,   901,   903,   905,   909,   911,   915,   917,   921,   923,
     926,   927,   934,   952,   954,   958,   960,   964,   966,   969,
     970,   973,   978,   979,   982,   986,   995,  1003,  1012,  1017,
    1018,  1021,  1027,  1029,  1032,  1033,  1037,  1039,  1041,  1043,
    1045,  1049,  1051,  1054,  1055,  1059,  1061,  1062,  1063,  1067,
    1069,  1071,  1073,  1077,  1082,  1089,  1095,  1102,  1109,  1142,
    1143,  1146,  1152,  1156,  1157,  1160,  1168,  1172,  1173,  1176,
    1177,  1180,  1182,  1186,  1187,  1188,  1191,  1192,  1193,  1199,
    1204,  1209,  1211,  1213,  1217,  1218,  1222,  1223,  1230,  1234,
    1238,  1247,  1253,  1254,  1257,  1259,  1264,  1266,  1271,  1277,
    1283,  1287,  1293,  1295,  1299,  1304,  1305,  1306,  1308,  1310,
    1314,  1316,  1319,  1320,  1324,  1329,  1331,  1336,  1337,  1339,
    1347,  1351,  1353,  1356,  1357,  1358,  1359,  1362,  1366,  1370,
    1377,  1384,  1389,  1395,  1398,  1400,  1402,  1404,  1408,  1411,
    1415,  1418,  1421,  1424,  1433,  1445,  1456,  1467,  1478,  1480,
    1484,  1486,  1490,  1491,  1495,  1501,  1503,  1507,  1508,  1512,
    1517,  1519,  1523,  1525,  1529,  1530,  1533,  1537,  1539,  1541,
    1545,  1546,  1550,  1551,  1552,  1553,  1554,  1555,  1556,  1557,
    1558,  1559,  1560,  1561,  1562,  1563,  1564,  1565,  1567,  1572,
    1575,  1577,  1581,  1585,  1590,  1594,  1594,  1596,  1601,  1606,
    1611,  1613,  1617,  1621,  1627,  1632,  1637,  1639,  1641,  1645,
    1647,  1649,  1653,  1654,  1655,  1657,  1661,  1672,  1674,  1682,
    1684,  1687,  1688,  1692,  1697,  1698,  1702,  1704,  1706,  1708,
    1715,  1723,  1730,  1750,  1754,  1756,  1758,  1762,  1773,  1777,
    1778,  1782,  1783,  1786,  1789,  1795,  1799,  1805,  1809,  1815,
    1819,  1827,  1838,  1849,  1851,  1853,  1857,  1861,  1863,  1867,
    1869,  1871,  1873,  1875,  1877,  1879,  1881,  1883,  1885,  1887,
    1889,  1891,  1893,  1895,  1899,  1901,  1905,  1911,  1915,  1918,
    1925,  1927,  1929,  1932,  1935,  1940,  1944,  1950,  1951,  1955,
    1964,  1968,  1970,  1972,  1974,  1976,  1980,  1981,  1985,  1987,
    1989,  1991,  1997,  2000,  2002,  2006,  2014,  2015,  2016,  2017,
    2018,  2019,  2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,
    2028,  2029,  2030,  2031,  2032,  2033,  2034,  2035,  2036,  2039,
    2041,  2045,  2047,  2052,  2058,  2060,  2062,  2066,  2068,  2075,
    2081,  2082,  2086,  2094,  2096,  2098,  2102,  2103,  2110,  2120,
    2121,  2125,  2126,  2132,  2136,  2138,  2140,  2142,  2144,  2146,
    2148,  2150,  2152,  2154,  2156,  2158,  2165,  2166,  2169,  2170,
    2173,  2178,  2185,  2186,  2190,  2199,  2205,  2206,  2209,  2210,
    2211,  2212,  2229,  2234,  2239,  2258,  2275,  2282,  2283,  2290,
    2294,  2300,  2306,  2314,  2318,  2324,  2328,  2332,  2338,  2342,
    2349,  2355,  2361,  2369,  2374,  2379,  2386,  2387,  2388,  2391,
    2392,  2395,  2396,  2397,  2404,  2408,  2419,  2426,  2432,  2486,
    2548,  2549,  2556,  2569,  2574,  2579,  2586,  2588,  2595,  2596,
    2597,  2601,  2606,  2611,  2622,  2623,  2624,  2627,  2631,  2635,
    2637,  2641,  2645,  2646,  2649,  2653,  2657,  2658,  2661,  2663,
    2667,  2668,  2672,  2676,  2677,  2681,  2682,  2686,  2687,  2688,
    2689,  2692,  2694,  2698,  2700,  2704,  2706,  2709,  2711,  2713,
    2717,  2719,  2721,  2725,  2727,  2729,  2733,  2737,  2739,  2741,
    2745,  2747,  2751,  2752,  2756,  2760,  2762,  2766,  2767,  2772,
    2780,  2784,  2786,  2788,  2792,  2794,  2798,  2799,  2803,  2807,
    2809,  2811,  2815,  2817,  2821,  2823,  2827,  2829,  2833,  2835,
    2839,  2841,  2848,  2860,  2873,  2877,  2881,  2885,  2889,  2891,
    2893,  2895,  2899,  2901,  2903,  2907,  2909,  2911,  2915,  2917,
    2921,  2923,  2927,  2928,  2932,  2933,  2935,  2942,  2948,  2949,
    2953,  2954,  2957,  2959,  2963,  2966,  2971,  2973,  2977,  2978,
    2982,  2983,  2986,  2987,  2992,  2997,  2999,  3001,  3003,  3005,
    3007,  3009,  3011,  3018,  3020,  3024,  3025,  3029,  3030,  3031,
    3032,  3033,  3037,  3038,  3039,  3042,  3044,  3048,  3050,  3057,
    3059,  3063,  3067,  3069,  3071,  3077,  3079,  3083,  3084,  3088,
    3090,  3093,  3094,  3098,  3100,  3102,  3106,  3107,  3115,  3118,
    3122,  3123,  3130,  3131,  3134,  3138,  3142,  3149,  3150,  3153,
    3158,  3163,  3164,  3168,  3172,  3173,  3179,  3181,  3185,  3187,
    3189,  3192,  3193,  3196,  3200,  3202,  3205,  3207,  3213,  3214,
    3218,  3222,  3223,  3228,  3229,  3233,  3237,  3238,  3243,  3244,
    3247,  3252,  3257,  3260,  3267,  3268,  3270,  3271,  3273,  3277,
    3278,  3280,  3282,  3286,  3288,  3292,  3293,  3295,  3299,  3300,
    3301,  3302,  3303,  3304,  3305,  3306,  3307,  3308,  3313,  3315,
    3317,  3319,  3321,  3323,  3325,  3327,  3334,  3336,  3338,  3340,
    3342,  3344,  3346,  3348,  3350,  3352,  3354,  3356,  3358,  3360,
    3362,  3364,  3368,  3369,  3375,  3377,  3382,  3384,  3386,  3388,
    3393,  3395,  3399,  3401,  3405,  3407,  3409,  3411,  3415,  3419,
    3423,  3425,  3429,  3434,  3439,  3446,  3451,  3456,  3465,  3466,
    3470,  3474,  3487,  3504,  3505,  3506,  3507,  3508,  3509,  3510,
    3511,  3512,  3513,  3515,  3517,  3524,  3526,  3528,  3535,  3542,
    3549,  3551,  3553,  3554,  3555,  3556,  3558,  3560,  3564,  3568,
    3584,  3600,  3604,  3608,  3609,  3613,  3614,  3618,  3620,  3622,
    3624,  3626,  3630,  3631,  3633,  3649,  3665,  3669,  3673,  3675,
    3679,  3683,  3686,  3689,  3692,  3695,  3700,  3702,  3707,  3708,
    3712,  3719,  3726,  3733,  3740,  3741,  3745,  3751,  3752,  3753,
    3754,  3757,  3759,  3761,  3764,  3771,  3780,  3787,  3796,  3798,
    3800,  3802,  3808,  3817,  3821,  3828,  3832,  3836,  3838,  3842,
    3848,  3849,  3852,  3856,  3860,  3861,  3864,  3866,  3870,  3872,
    3876,  3878,  3882,  3884,  3888,  3891,  3894,  3897,  3899,  3901,
    3903,  3905,  3907,  3909,  3911,  3915,  3916,  3919,  3927,  3930,
    3933,  3936,  3939,  3942,  3945,  3948,  3951,  3954,  3957,  3960,
    3963,  3966,  3974,  3977,  3980,  3983,  3986,  3989,  3992,  3995,
    3998,  4003,  4004,  4009,  4011,  4012,  4013,  4014,  4015,  4016,
    4017,  4018,  4019,  4020,  4021,  4022,  4023
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
  "KW_INSERTING", "KW_UPDATING", "KW_DELETING", "'='", "'<'", "'>'", 
  "'+'", "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", 
  "']'", "'.'", "'?'", "$accept", "top", "statement", "grant", 
  "prot_table_name", "privileges", "privilege_list", "proc_privileges", 
  "privilege", "grant_option", "role_admin_option", "simple_proc_name", 
  "revoke", "rev_grant_option", "grantee_list", "grantee", 
  "user_grantee_list", "user_grantee", "role_name_list", "role_name", 
  "role_grantee_list", "role_grantee", "declare", "declare_clause", 
  "udf_decl_clause", "udf_data_type", "arg_desc_list1", "arg_desc_list", 
  "arg_desc", "return_value1", "return_value", "filter_decl_clause", 
  "create", "create_clause", "recreate", "recreate_clause", "replace", 
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
  "input_parameters", "output_parameters", "proc_parameters", 
  "proc_parameter", "var_declaration_list", "var_declarations", 
  "var_declaration", "var_decl_opt", "var_init_opt", "proc_block", 
  "full_proc_block", "full_proc_block_body", "proc_statements", 
  "proc_statement", "excp_statement", "raise_statement", "exec_procedure", 
  "exec_sql", "varstate", "for_select", "for_exec_into", "exec_into", 
  "if_then_else", "post_event", "event_argument_opt", "singleton_select", 
  "variable", "proc_inputs", "proc_outputs", "variable_list", "while", 
  "breakleave", "cursor_def", "excp_hndl_statements", 
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
  "numeric_type", "ordinal", "prec_scale", "decimal_keyword", 
  "float_type", "precision_opt", "set", "set_generator", "savepoint", 
  "set_savepoint", "release_savepoint", "release_only_opt", 
  "undo_savepoint", "optional_savepoint", "commit", "rollback", 
  "optional_work", "optional_retain", "opt_snapshot", "set_transaction", 
  "tran_opt_list_m", "tran_opt_list", "tran_opt", "access_mode", 
  "lock_wait", "isolation_mode", "iso_mode", "snap_shot", "version_mode", 
  "tbl_reserve_options", "lock_type", "lock_mode", "restr_list", 
  "restr_option", "table_lock", "table_list", "set_statistics", "select", 
  "union_expr", "order_clause", "order_list", "order_item", 
  "order_direction", "nulls_placement", "nulls_clause", 
  "for_update_clause", "for_update_list", "lock_clause", "select_expr", 
  "ordered_select_expr", "begin_limit", "end_limit", "begin_first", 
  "end_first", "limit_clause", "first_clause", "skip_clause", 
  "distinct_clause", "select_list", "select_items", "select_item", 
  "from_clause", "from_list", "table_reference", "joined_table", 
  "table_proc", "proc_table_inputs", "null_or_value_list", 
  "null_or_value", "table_name", "simple_table_name", "join_type", 
  "group_clause", "grp_column_list", "grp_column_elem", 
  "group_by_function", "having_clause", "where_clause", "plan_clause", 
  "plan_expression", "plan_type", "plan_item_list", "plan_item", 
  "table_or_alias_list", "access_type", "index_list", "insert", 
  "insert_value_list", "delete", "delete_searched", "delete_positioned", 
  "cursor_clause", "update", "update_searched", "update_positioned", 
  "assignments", "assignment", "rhs", "blob", "filter_clause", 
  "blob_subtype_value", "segment_clause", "segment_length", 
  "column_parens_opt", "column_parens", "column_list", 
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
  "numeric_value_function", "string_value_function", "udf", 
  "cast_specification", "case_expression", "case_abbreviation", 
  "case_specification", "simple_case", "simple_when_clause", 
  "searched_case", "searched_when_clause", "when_operand", "case_operand", 
  "case_result", "timestamp_part", "all_noise", "null_value", 
  "symbol_UDF_name", "symbol_blob_subtype_name", 
  "symbol_character_set_name", "symbol_collation_name", 
  "symbol_column_name", "symbol_constraint_name", "symbol_cursor_name", 
  "symbol_domain_name", "symbol_exception_name", "symbol_filter_name", 
  "symbol_gdscode_name", "symbol_generator_name", "symbol_index_name", 
  "symbol_item_alias_name", "symbol_procedure_name", "symbol_role_name", 
  "symbol_table_alias_name", "symbol_table_name", "symbol_trigger_name", 
  "symbol_user_name", "symbol_variable_name", "symbol_view_name", 
  "symbol_savepoint_name", "valid_symbol_name", "non_reserved_word", 0
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
     515,   516,    61,    60,    62,    43,    45,    42,    47,    59,
      44,    40,    41,    58,    91,    93,    46,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   278,   279,   279,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   281,   281,   281,   281,   281,   282,   282,
     283,   283,   283,   284,   284,   285,   286,   286,   286,   286,
     286,   287,   287,   288,   288,   289,   290,   290,   290,   290,
     290,   290,   290,   291,   292,   292,   292,   292,   293,   293,
     293,   293,   294,   294,   295,   295,   295,   296,   296,   297,
     298,   298,   299,   299,   300,   301,   301,   302,   303,   303,
     303,   304,   304,   304,   305,   305,   306,   306,   307,   307,
     308,   308,   308,   308,   308,   309,   310,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   312,   313,
     313,   313,   314,   315,   315,   316,   316,   317,   317,   317,
     318,   319,   319,   319,   320,   320,   321,   321,   322,   322,
     323,   323,   324,   325,   325,   326,   326,   327,   327,   328,
     328,   329,   330,   330,   331,   332,   333,   334,   335,   336,
     336,   337,   338,   338,   339,   339,   340,   340,   340,   340,
     340,   341,   341,   342,   342,   343,   343,   343,   343,   344,
     344,   344,   344,   345,   345,   346,   346,   347,   348,   349,
     349,   350,   351,   352,   352,   353,   354,   355,   355,   356,
     356,   357,   357,   358,   358,   358,   359,   359,   359,   360,
     361,   362,   362,   362,   363,   363,   364,   364,   365,   365,
     365,   366,   367,   367,   368,   368,   369,   369,   370,   371,
     372,   373,   374,   374,   375,   375,   375,   375,   375,   375,
     376,   376,   377,   377,   378,   379,   379,   379,   379,   379,
     380,   381,   381,   382,   382,   382,   382,   383,   384,   385,
     386,   386,   387,   388,   388,   388,   388,   388,   389,   390,
     391,   391,   391,   391,   392,   393,   394,   395,   396,   396,
     397,   397,   398,   398,   399,   400,   400,   401,   401,   402,
     403,   403,   404,   404,   405,   405,   406,   407,   407,   407,
     408,   408,   409,   409,   409,   409,   409,   409,   409,   409,
     409,   409,   409,   409,   409,   409,   409,   409,   409,   409,
     410,   410,   411,   412,   413,   414,   414,   415,   416,   417,
     418,   418,   419,   420,   421,   422,   423,   423,   423,   424,
     424,   424,   425,   425,   425,   425,   426,   427,   427,   428,
     428,   429,   429,   430,   431,   431,   432,   432,   432,   432,
     433,   434,   435,   436,   437,   437,   437,   438,   439,   440,
     440,   441,   441,   442,   442,   443,   444,   445,   446,   447,
     447,   448,   449,   450,   450,   450,   451,   452,   452,   453,
     453,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     453,   453,   453,   453,   454,   454,   455,   456,   457,   457,
     457,   457,   457,   457,   457,   458,   459,   460,   460,   461,
     461,   461,   461,   461,   461,   461,   462,   462,   463,   463,
     463,   463,   463,   463,   463,   464,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   466,
     466,   467,   467,   468,   469,   469,   469,   470,   470,   471,
     472,   472,   473,   473,   473,   473,   474,   474,   475,   476,
     476,   477,   477,   478,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   480,   480,   481,   481,
     482,   482,   483,   483,   484,   484,   485,   485,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   487,   487,   488,
     488,   488,   488,   489,   489,   490,   490,   490,   491,   491,
     492,   492,   492,   493,   493,   493,   494,   494,   494,   495,
     495,   496,   496,   496,   497,   497,   498,   499,   499,   499,
     500,   500,   501,   501,   501,   501,   502,   502,   503,   503,
     503,   504,   504,   504,   505,   505,   505,   506,   507,   508,
     508,   509,   510,   510,   511,   512,   513,   513,   514,   514,
     515,   515,   516,   517,   517,   518,   518,   519,   519,   519,
     519,   520,   520,   521,   521,   522,   522,   523,   523,   523,
     524,   524,   524,   525,   525,   525,   526,   527,   527,   527,
     528,   528,   529,   529,   530,   531,   531,   532,   532,   533,
     534,   535,   535,   535,   536,   536,   537,   537,   538,   539,
     539,   539,   540,   540,   541,   541,   542,   542,   543,   543,
     544,   544,   545,   546,   547,   548,   549,   550,   551,   551,
     551,   551,   552,   552,   552,   553,   553,   553,   554,   554,
     555,   555,   556,   556,   557,   557,   557,   558,   559,   559,
     560,   560,   561,   561,   562,   562,   563,   563,   564,   564,
     565,   565,   566,   566,   567,   568,   568,   568,   568,   568,
     568,   568,   568,   569,   569,   570,   570,   571,   571,   571,
     571,   571,   572,   572,   572,   573,   573,   574,   574,   575,
     575,   576,   577,   577,   577,   577,   577,   578,   578,   579,
     579,   580,   580,   581,   581,   581,   582,   582,   583,   583,
     584,   584,   585,   585,   586,   587,   588,   589,   589,   590,
     591,   592,   592,   593,   594,   594,   595,   595,   596,   596,
     596,   597,   597,   490,   598,   598,   599,   599,   600,   600,
     601,   602,   602,   603,   603,   604,   605,   605,   606,   606,
     606,   607,   608,   608,   609,   609,   609,   609,   609,   610,
     610,   610,   610,   610,   610,   611,   611,   611,   612,   612,
     612,   612,   612,   612,   612,   612,   612,   612,   613,   613,
     613,   613,   613,   613,   613,   613,   614,   614,   614,   614,
     614,   614,   614,   614,   614,   614,   614,   614,   614,   614,
     614,   614,   615,   615,   616,   616,   617,   617,   617,   617,
     618,   618,   619,   619,   620,   620,   620,   620,   621,   622,
     623,   623,   624,   624,   624,   625,   625,   625,   626,   626,
     627,   628,   629,   630,   630,   630,   630,   630,   630,   630,
     630,   630,   630,   630,   630,   630,   630,   630,   630,   630,
     630,   630,   630,   630,   630,   630,   630,   630,   630,   631,
     631,   631,   632,   633,   633,   634,   634,   635,   635,   635,
     635,   635,   636,   636,   636,   636,   636,   637,   638,   638,
     639,   640,   640,   640,   640,   640,   641,   641,   642,   642,
     643,   644,   645,   646,   647,   647,   648,   649,   649,   649,
     649,   650,   650,   650,   650,   650,   650,   650,   650,   650,
     650,   650,   651,   652,   653,   653,   653,   654,   654,   655,
     656,   656,   657,   657,   658,   658,   659,   659,   660,   660,
     661,   661,   662,   662,   663,   664,   665,   666,   666,   666,
     666,   666,   666,   666,   666,   667,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   692,   693,   693,   693,   693,   693,   693,   693,
     693,   693,   693,   693,   693,   693,   693
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
       3,     3,     3,     1,     1,     6,     3,     1,     3,     1,
       3,     2,     1,     0,     2,     3,     1,     0,     1,     1,
       2,     3,     4,     0,     1,     2,     0,     1,     1,     5,
       5,     3,     2,     0,     1,     3,     1,     1,     6,     3,
       2,     6,     2,     1,     2,     2,     2,     0,     1,     1,
       2,     0,     2,     0,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     2,     2,     2,     5,     1,     2,     3,
       2,     2,     0,     1,     1,     1,     1,     3,     4,     8,
       4,     0,     6,     1,     1,     2,     2,     0,     3,     3,
       1,     2,     2,     2,     8,     8,     8,     8,     3,     0,
       2,     0,     1,     3,     2,     1,     0,     1,     2,     6,
       1,     0,     2,     1,     1,     1,     3,     1,     2,     0,
       1,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     2,     1,     1,     1,     2,     2,     1,
       3,     4,     2,     6,     4,     1,     1,     7,     8,     6,
       8,     6,     4,     0,     4,     2,     1,     3,     0,     2,
       4,     0,     1,     1,     3,     3,     6,     2,     2,     3,
       0,     1,     2,     4,     1,     3,     2,     2,     2,     1,
       4,     7,     7,     1,     1,     3,     4,     7,     2,     1,
       3,     1,     1,     6,     3,     0,     0,     0,     0,     3,
       0,     9,     9,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     5,     5,
       5,     5,     5,     5,     2,     0,     4,     2,     3,     3,
       2,     2,     3,     3,     2,     3,     6,     1,     2,     4,
       3,     2,     2,     2,     2,     3,     1,     3,     3,     3,
       2,     2,     4,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     0,     2,     2,     0,
       1,     2,     2,     2,     2,     2,     1,     3,     7,     1,
       0,     1,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     2,     2,     2,     1,     1,     1,     1,
       4,     5,     1,     3,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     6,     5,     3,     0,     2,     2,     0,     3,     0,
       4,     1,     5,     4,     1,     4,     1,     2,     2,     1,
       1,     1,     2,     2,     2,     2,     1,     0,     3,     5,
       1,     1,     2,     3,     1,     2,     3,     0,     1,     1,
       1,     5,     5,     6,     1,     1,     1,     2,     4,     1,
       0,     5,     1,     0,     3,     2,     1,     0,     2,     0,
       1,     0,     3,     1,     0,     1,     2,     1,     1,     1,
       1,     2,     2,     1,     2,     3,     1,     1,     3,     3,
       1,     2,     3,     1,     2,     0,     2,     1,     1,     0,
       1,     1,     1,     3,     2,     3,     0,     1,     3,     4,
       4,     1,     3,     4,     3,     0,     1,     3,     3,     1,
       1,     0,     1,     1,     4,     0,     3,     0,     2,     0,
       2,     0,     9,    10,     0,     0,     0,     0,     3,     2,
       1,     0,     3,     5,     3,     2,     5,     2,     1,     1,
       1,     1,     1,     3,     1,     2,     3,     2,     1,     3,
       1,     1,     6,     3,     3,     2,     3,     0,     1,     3,
       1,     1,     1,     2,     1,     1,     1,     2,     1,     2,
       1,     2,     0,     3,     0,     1,     3,     1,     1,     1,
       1,     3,     1,     1,     1,     2,     0,     2,     0,     2,
       0,     4,     1,     2,     1,     1,     0,     1,     3,     2,
       1,     1,     2,     1,     4,     2,     1,     3,     8,     5,
       1,     3,     1,     1,     4,     4,     4,     1,     1,     5,
       5,     1,     3,     3,     1,     1,     7,     7,     5,     3,
       0,     1,     1,     1,     2,     0,     1,     1,     1,     0,
       3,     1,     3,     1,     0,     3,     1,     3,     1,     3,
       3,     1,     1,     3,     1,     2,     1,     3,     3,     1,
       2,     3,     3,     3,     3,     1,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     1,     1,     5,     6,     3,     4,     5,     6,
       3,     4,     3,     4,     3,     4,     4,     5,     4,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,    10,    10,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     4,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     4,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     6,     6,     6,     8,     4,     4,     3,     6,
       1,     1,     6,     6,     1,     1,     4,     6,     4,     5,
       3,     5,     4,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,   567,   116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   567,   641,     0,     0,     0,     0,     0,
       0,     2,    11,    16,     8,     7,    14,    15,    13,     4,
      10,    20,   549,    18,   554,   555,   556,     6,    17,   548,
     550,    19,   615,   611,    12,     9,   722,   723,    21,   727,
     728,     5,   459,     0,     0,     0,     0,     0,     0,   397,
     566,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   115,     0,     0,    96,   621,   900,     0,    22,   898,
       0,     0,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,    30,    38,
      35,    37,   749,    36,   749,   981,   983,   985,   984,   990,
     986,   993,   991,   987,   989,   992,   988,   994,   995,   996,
       0,    32,     0,    33,     0,    67,    69,   973,   982,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
     956,   635,   640,     0,     0,   574,     0,   672,   674,   975,
       0,     0,     0,   108,   557,   980,     0,     1,     3,     0,
       0,   627,     0,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,     0,   425,   426,     0,   966,
     404,     0,   970,   401,   269,   972,     0,   674,   400,   375,
     976,   571,   564,   896,     0,   104,   152,   151,   105,   134,
     218,   761,   962,     0,   103,   146,   969,     0,    99,   269,
     106,   902,   123,   100,   203,   101,     0,   102,   749,   979,
     107,   147,   619,   620,     0,   901,   899,     0,    75,     0,
     967,   698,   481,   965,   474,     0,   480,   485,   475,   476,
     483,   477,   478,   479,   484,   328,    31,     0,    40,   748,
      39,     0,     0,     0,     0,     0,     0,   754,     0,     0,
       0,     0,     0,     0,     0,   563,   906,     0,   887,   634,
     634,   635,   647,   645,   955,   648,     0,   649,   639,   635,
       0,     0,     0,     0,     0,     0,   590,   583,   572,   573,
     575,   577,   578,   579,   586,   587,   580,     0,   673,   974,
     109,   269,   110,   203,   111,   749,   560,     0,     0,   612,
       0,   631,     0,     0,     0,   402,   460,   367,     0,   367,
       0,   221,   403,   407,   398,   457,   458,     0,   271,   242,
     449,     0,   399,   416,     0,   373,   374,   470,   570,   568,
     897,   149,   149,     0,     0,     0,   161,   153,   154,   133,
       0,    97,     0,     0,   112,   271,   122,   121,   124,     0,
       0,     0,     0,     0,   958,    76,   221,   517,     0,   724,
     725,   482,     0,     0,     0,     0,   865,   893,     0,   957,
       0,   867,     0,     0,   894,     0,   888,   879,   878,   877,
     981,     0,     0,     0,   869,   870,   871,   880,   881,   889,
     890,   985,   891,   892,     0,   990,   986,   895,     0,     0,
       0,     0,   848,   350,   326,   668,   843,   758,   671,   868,
     844,   882,   846,   847,   862,   863,   864,   883,   845,   907,
     908,   909,   910,   851,   849,   850,   930,   931,   934,   935,
     670,     0,     0,   962,     0,   751,     0,     0,    28,    34,
       0,     0,    44,    70,    72,   977,    68,     0,     0,     0,
     753,     0,    53,     0,     0,     0,     0,    52,   562,     0,
       0,     0,   644,   642,     0,   651,     0,   650,   652,   654,
     734,   735,   638,     0,   609,     0,   584,   595,   581,   595,
     582,   596,   602,   606,   607,   591,   576,   698,   731,   762,
       0,     0,   271,     0,     0,   559,   558,   614,   616,   621,
     613,   629,     0,   610,   186,   177,   462,   465,   174,   173,
     130,     0,   463,   149,   149,   149,   149,   466,   464,   461,
       0,   411,     0,   413,   412,     0,   414,     0,   408,     0,
       0,   272,   219,   269,     0,     0,   420,     0,   421,   367,
     450,     0,   456,     0,     0,     0,   453,   962,   378,   377,
     469,     0,   395,   150,     0,     0,   159,     0,   158,     0,
     148,   162,   163,   167,   166,   165,   155,   517,   529,   504,
     540,   507,   530,   541,     0,   547,   508,     0,   537,     0,
     531,   544,   503,   526,   505,   506,   501,   367,   486,   487,
     488,   496,   502,   489,   498,   519,     0,   524,   521,   499,
     537,   500,   113,   269,   114,     0,     0,   125,     0,     0,
     202,   242,   375,   365,     0,   221,     0,    82,    84,     0,
       0,     0,   743,     0,     0,     0,     0,   994,   995,   996,
       0,   697,   766,   775,   778,   783,   779,   780,   781,   785,
     786,   784,   787,   782,   764,     0,   956,     0,   956,   884,
       0,     0,   956,   956,   956,     0,   885,   886,     0,     0,
     945,     0,     0,     0,     0,   853,   852,   641,     0,     0,
     671,   325,   978,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   750,    29,     0,     0,    45,    73,
       0,     0,    27,   740,     0,   756,   641,     0,   719,   740,
       0,     0,     0,     0,   561,     0,   634,     0,     0,   698,
       0,     0,   655,   971,   552,     0,   551,   904,     0,   585,
       0,   593,   589,   588,     0,   599,     0,   604,   592,     0,
     729,   730,     0,     0,     0,   242,   365,     0,   625,     0,
     626,   630,     0,     0,   131,   187,     0,     0,     0,     0,
       0,   367,   410,     0,   367,   368,   415,   496,   519,   274,
       0,   268,   270,   365,   241,   963,   213,   210,     0,   223,
     367,   486,   365,     0,     0,     0,   240,   243,   244,   245,
     246,     0,   419,   455,   454,   418,   417,     0,     0,     0,
     381,   379,   380,   376,     0,   367,   196,   156,   160,     0,
     164,     0,   514,   527,   528,   545,     0,   542,   547,     0,
     534,   532,   533,   136,     0,     0,     0,   497,     0,     0,
       0,     0,   535,   271,     0,   365,   126,   201,     0,   204,
     206,   207,     0,     0,     0,     0,   221,   221,    79,     0,
      86,    78,   515,   516,   959,   517,     0,     0,     0,   777,
     765,     0,     0,   835,   836,   837,     0,   769,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   947,   948,   949,   950,   951,   952,   953,   954,     0,
       0,     0,     0,   940,     0,     0,     0,     0,     0,   956,
     327,   861,   860,   669,   873,     0,   856,   961,   855,   854,
     857,   858,   859,   928,     0,   866,   760,   759,   752,     0,
       0,     0,     0,     0,     0,    25,    54,    42,    62,    64,
       0,     0,    71,     0,   745,     0,   755,   956,     0,   745,
      50,    48,     0,     0,     0,   643,   646,     0,   657,   658,
     660,   661,   667,     0,   684,   653,   656,   553,   905,   594,
     603,   598,   597,     0,   608,   732,   733,   763,   365,     0,
       0,   617,   636,   618,   628,   176,   183,   182,     0,   179,
     149,   193,   178,   188,   189,   170,   169,   903,   172,   171,
     467,     0,     0,     0,   409,   273,   276,   212,   367,     0,
     368,   214,   209,   215,     0,     0,   251,     0,   423,   368,
     367,   365,   422,     0,     0,     0,   394,   472,   197,   198,
     157,     0,     0,     0,     0,   519,     0,   543,     0,     0,
     367,   368,     0,   492,   494,     0,     0,     0,     0,     0,
       0,     0,   375,   276,   149,   128,   242,   199,   395,   956,
     370,   353,   354,     0,    83,     0,   221,     0,    88,     0,
      85,     0,     0,     0,   726,   964,     0,     0,   770,     0,
       0,   776,     0,     0,   768,   767,     0,   822,     0,   813,
     812,     0,   791,     0,   820,   838,   830,     0,     0,     0,
     792,   816,     0,     0,   795,     0,     0,     0,     0,     0,
       0,     0,   793,     0,     0,   794,     0,   824,     0,     0,
     788,     0,     0,   789,     0,     0,   790,     0,     0,   221,
       0,   911,     0,     0,   926,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   946,     0,     0,   944,     0,     0,
     936,     0,     0,     0,     0,     0,   872,   927,    66,    58,
      59,    65,    60,    61,     0,     0,     0,    23,    26,    42,
      43,   517,   517,     0,   737,   757,     0,     0,   720,   736,
      51,    49,    46,     0,   682,   660,     0,   680,   675,   676,
     678,     0,     0,   665,     0,   696,   600,   601,   605,   276,
     200,   370,     0,   181,     0,     0,     0,   194,     0,   190,
       0,     0,     0,   405,   229,   224,   875,   225,   226,   227,
     228,   281,     0,   275,   277,     0,   222,   230,     0,   251,
     621,   247,     0,   424,   451,   452,   386,   387,   383,   382,
     385,   384,   367,   471,   368,   168,   960,     0,     0,   510,
       0,   509,   546,     0,   538,     0,   137,     0,   490,     0,
     518,   519,   525,   523,     0,   520,   365,     0,     0,     0,
     120,   129,   205,   367,     0,     0,   366,     0,    98,     0,
     118,   117,    94,     0,     0,    90,     0,    87,     0,   828,
     829,   772,   771,   774,   773,     0,     0,     0,   641,     0,
       0,   831,     0,     0,     0,     0,     0,     0,   823,   821,
     817,     0,   825,     0,     0,     0,     0,   826,     0,     0,
       0,     0,     0,     0,   917,   916,     0,     0,   913,   912,
       0,   921,   920,   919,   918,   915,   914,     0,     0,   942,
     941,     0,     0,     0,     0,     0,     0,     0,   874,    55,
      56,     0,    57,    63,    24,   741,     0,   742,   739,   744,
     747,   746,     0,     0,   718,     0,    47,   663,   659,   681,
     677,   679,     0,     0,   664,     0,     0,   700,     0,   366,
     622,   623,   637,   149,   184,   180,     0,   196,   195,   191,
       0,   368,   876,   280,     0,   289,   368,   278,   368,   217,
     231,   232,   367,     0,   248,     0,     0,     0,     0,     0,
       0,     0,     0,   276,   468,   512,     0,   513,     0,   135,
       0,   217,   138,   139,   141,   142,   143,   493,   495,   491,
     522,   276,   395,   368,   196,     0,     0,     0,   351,     0,
     355,   367,    89,     0,     0,    91,   519,     0,   814,     0,
       0,   956,   840,   839,     0,     0,   818,     0,     0,     0,
       0,   827,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   929,   220,   922,   923,     0,   924,   943,   938,
     937,     0,   932,   933,   698,    41,   517,   698,   721,   682,
     666,   688,   683,   685,   690,   687,   536,   692,   693,   689,
     694,     0,   695,   706,   632,   368,   352,   624,     0,   175,
     192,   368,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   983,   988,   290,   285,     0,   287,   284,
     294,   295,   296,   297,   298,   305,   306,   299,   301,   302,
     304,   309,     0,     0,     0,     0,     0,   267,     0,     0,
     208,   233,     0,     0,     0,   251,   234,   237,   749,     0,
     368,   392,   393,   389,   388,   391,   390,     0,   511,   539,
     144,   132,   140,     0,   367,   264,   127,   368,     0,   698,
     369,   356,     0,     0,    92,    93,    80,     0,   799,   807,
       0,   800,   808,   803,   811,   815,   819,   801,   809,   802,
     810,   796,   804,   797,   805,   798,   806,     0,   939,   684,
     738,   684,     0,     0,     0,     0,   702,   704,   705,   699,
       0,   265,   185,   406,   223,   312,     0,     0,   315,   316,
       0,   308,     0,     0,     0,   323,   307,     0,   337,   338,
     286,     0,   291,   288,   341,     0,   300,   293,   303,   292,
     211,   216,   235,   251,   749,   238,   257,   250,   252,   396,
     368,     0,   371,     0,   358,   359,   361,   362,   684,   368,
      77,    95,     0,   925,   696,   696,   662,   686,   691,   703,
     706,     0,   283,     0,   310,     0,   328,     0,     0,     0,
       0,     0,     0,   349,     0,     0,     0,     0,   344,   342,
     332,     0,   333,   239,   257,     0,   251,   253,   254,   266,
     368,   682,   361,     0,     0,   696,     0,   698,   700,   700,
     710,     0,   707,     0,   711,   282,   279,   311,   331,     0,
     314,     0,   340,     0,   322,     0,   348,   347,   968,   346,
       0,     0,   324,     0,   251,     0,     0,   249,     0,   256,
       0,   255,   372,   364,   360,     0,   357,   119,   684,   615,
     615,   701,   706,     0,   713,     0,   709,   712,     0,     0,
       0,     0,     0,     0,     0,     0,   343,   345,   335,   334,
     236,     0,     0,   260,   259,   258,   682,   696,   842,   633,
     708,     0,   715,     0,   329,   313,   319,     0,     0,     0,
     321,   336,   263,   261,   262,     0,   700,     0,   716,     0,
       0,   339,   317,     0,   363,   615,   714,     0,   330,   318,
     320,   841,   717
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    20,    21,    22,   457,   120,   121,   122,   123,  1187,
     712,   707,    23,   135,   955,   956,   957,   958,   124,   125,
     462,   463,    24,    82,   375,   860,   636,   637,   638,  1087,
    1088,   238,    25,    74,    26,   153,    27,   364,    75,  1298,
     220,   368,   628,  1075,  1290,   526,   208,   360,  1061,  1441,
    1442,  1443,  1444,  1445,  1446,   214,   230,   205,   574,   206,
     356,   357,   358,   580,   581,   582,   583,   584,   528,   529,
     530,  1008,  1005,  1006,  1223,  1404,   531,  1012,  1013,  1014,
    1228,  1050,   223,   312,   370,   848,   849,   850,   787,   788,
     789,  1570,   557,   549,  1346,   639,  1030,  1233,  1419,  1420,
    1421,  1576,   851,   559,   796,   797,   798,   799,  1251,   800,
    1726,  1727,  1728,  1804,   218,   310,   622,   193,   338,   554,
     550,   551,  1242,  1243,  1244,  1414,  1703,  1545,  1546,  1547,
    1548,  1549,  1550,  1551,  1552,  1553,  1650,  1554,  1555,  1556,
    1557,  1558,  1711,  1559,   422,   423,  1789,  1721,  1560,  1561,
    1793,  1663,  1664,  1717,  1718,    28,   227,   314,  1080,  1081,
    1082,  1599,  1684,  1685,  1686,   853,  1458,   542,  1024,  1296,
     225,   624,   347,   570,   571,   813,   815,  1263,    29,    59,
     775,   772,   332,   333,   342,   343,   185,   186,   344,  1039,
     565,   805,   190,   162,   325,   326,   538,   198,   572,  1264,
      30,    96,   607,   608,   609,  1062,  1063,   610,   777,   612,
     613,  1055,  1375,   837,   614,   778,   616,   617,   618,   619,
    1511,   830,   620,   621,   827,    31,    32,    33,    34,    35,
     516,    36,   479,    37,    38,    61,   202,   349,    39,   298,
     299,   300,   301,   302,   303,   304,   305,   742,   306,   993,
    1218,   501,   502,   747,   503,    40,  1562,    42,   161,   517,
     518,   234,  1402,  1003,   321,   760,   523,    43,   718,   482,
     288,  1222,  1527,   140,   141,   142,   286,   486,   487,   488,
     729,   978,   979,   980,   981,  1213,   424,  1164,  1687,   147,
    1211,  1215,  1512,  1513,  1514,  1397,   379,  1524,  1740,  1640,
    1741,  1742,  1743,  1786,  1827,  1563,  1197,  1564,    46,    47,
     380,  1565,    49,    50,   507,  1566,   489,    51,   964,  1376,
    1194,  1379,   258,   259,   454,   469,   470,   714,   426,   427,
     510,   651,   876,   652,   653,   654,   655,  1111,   656,   657,
     658,   659,   660,   661,   662,   663,   664,   878,  1114,  1115,
    1319,   689,   665,   429,   430,   935,  1235,   431,   432,   433,
     434,   435,   436,   437,   642,   221,   236,  1516,  1018,  1064,
     737,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     447,   448,   926,   449,   681,  1168,   682,  1165,   919,   287,
     450,   451,   863,  1265,   936,   211,   784,  1094,   242,   188,
     239,  1757,   215,  1828,   732,   982,   126,   452,   197,   199,
     959,   691,   228,   154,   453,   128
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1404
static const short yypact[] =
{
     996,   835,    32,  1398,   236,   627,   209,  1456,    88,  1352,
     537,   232,  1629,    32,   662,   493,  3013,   424,  3013,    39,
     522,   259, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404,   396, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404,  4224,  3013,  3013,  3013,  3013,  3013, -1404,
   -1404,   548,   413,  3013,  3013,  3013,   714,  3013,   530,  3013,
    3013, -1404,  3013,  3013, -1404,   223, -1404,   543, -1404, -1404,
     687,  3013, -1404,  3013,  3013,  3013,   776,  3013,  3013,  3013,
    3013,   530,  3013,  3013,  3013,  3013, -1404,  3013,   664, -1404,
   -1404, -1404,   553, -1404,   553, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
     734,   599,   748, -1404,    42, -1404, -1404, -1404, -1404,  3013,
    3013,  3013,   800,   805,   808,   510,   106,   792,   391,   480,
     220,   718, -1404,  3013,   879,   725,   806, -1404,  3013, -1404,
    3013,  3013,  3013, -1404, -1404, -1404,  3013, -1404, -1404,   957,
     205,   935,   295, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404,   485, -1404, -1404,   413, -1404,
   -1404,   213, -1404, -1404,   735, -1404,   839, -1404, -1404,   506,
   -1404,   852, -1404, -1404,   822, -1404,   969, -1404, -1404,  1051,
   -1404, -1404, -1404,   413, -1404, -1404, -1404,   544, -1404,   735,
   -1404, -1404,   250, -1404,   976, -1404,  1002, -1404,   553, -1404,
   -1404, -1404, -1404, -1404,   999, -1404, -1404,   873, -1404,  1004,
   -1404,   892, -1404, -1404, -1404,   873, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404,  4631, -1404,  3013, -1404, -1404,
   -1404,  2030,   528,   956,  2508,  3013,  1021,   853,  1047,  1067,
    2030,   993,  1013,  1016,  2508,   896, -1404,  6021, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404,  4312, -1404, -1404, -1404,
     979,  3013,  1058,   955,   330,  3013,   984, -1404, -1404,   725,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404,  3013, -1404, -1404,
   -1404,   735, -1404,   976, -1404,   553,  1046,  6021,  1018, -1404,
     994,   983,   863,  1083,   551,   295, -1404,  1160,   684, -1404,
    3013, -1404,   485, -1404, -1404, -1404, -1404,  3013,  1039,  2188,
     990,  2311,   929, -1404,  4224, -1404, -1404,   780, -1404, -1404,
   -1404,   953,   953,   413,  1103,   413,  1345,   969, -1404, -1404,
    3599, -1404,  3013,  3013, -1404,  1039, -1404, -1404,  1192,   135,
     959,  3013,  1218,  3013, -1404, -1404,    85,    73,  3640, -1404,
   -1404, -1404,   960,   963,   966,  1031, -1404, -1404,   971, -1404,
     974, -1404,   975,   982, -1404,  1003, -1404, -1404, -1404, -1404,
    1010,  1011,  1043,  1060, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404,  1014, -1404, -1404,  4728,  1015,  1027, -1404,  6021,  6021,
    4392,  3013, -1404, -1404,  1042, -1404,  1045, -1404,  1177, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404,  1030,  1040,  1041,   647, -1404,  3013,  1131, -1404, -1404,
    3013,  3013,    19, -1404, -1404, -1404, -1404,  3013,  3013,    77,
   -1404,  3013, -1404,  1254,  3013,  2030,  1180,  1061, -1404,  3013,
    4967,   324, -1404, -1404,  6021, -1404,  1266,  1063, -1404,   604,
    1177, -1404, -1404,   511, -1404,   642, -1404,   107, -1404,   107,
   -1404,  1064, -1404,   113, -1404,  1161, -1404,     8, -1404, -1404,
    1080,  1068,  1039,  1077,  1339, -1404, -1404,  1081, -1404,   189,
   -1404,  1224,  1106, -1404, -1404,   144,  1260, -1404, -1404, -1404,
   -1404,   413, -1404,   953,   953,   953,   953, -1404,  1089, -1404,
    1336, -1404,  1347, -1404, -1404,  1326, -1404,  3599, -1404,  3599,
     689, -1404, -1404,   735,  1364,  3013, -1404,  6290, -1404,   452,
   -1404,  3013,   824,   839,  1193,  1162,  1234,  1197, -1404, -1404,
   -1404,   192,  1237, -1404,  1178,   530, -1404,   413, -1404,  1358,
   -1404,  1345, -1404, -1404, -1404, -1404, -1404,   331,  1191, -1404,
   -1404, -1404,  1194, -1404,  1241,  1121, -1404,  1298,  1123,   416,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404,  1124, -1404, -1404, -1404,   131,  1129,  1130,    21, -1404,
    1123, -1404, -1404,   735, -1404,  1331,  1392, -1404,   413,   413,
   -1404,  2188,   506, -1404,  1274, -1404,  1247,  1136, -1404,  6554,
    3232,  1272, -1404,  1280,  1138,  3737,  1139,   823,  1020,  1155,
    3400,   168, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404,  1221,   491,  5047,   152, -1404,
    3013,  6021,   788,   789,   905,  1528, -1404, -1404,  6021,  3976,
    1177,   287,  1215,  6021,  5047,   181,   181,   662,   721,  1141,
     341, -1404, -1404,  5047,  6021,  3013,  6021,  6021,  6021,  6021,
    6021,  5286,  1605,  3013, -1404, -1404,  3883,  1233, -1404, -1404,
    1208,  2508, -1404,  1356,   774, -1404,   662,  1150, -1404,  1356,
    3883,  1357,  1360,  3013, -1404,   341, -1404,   459,  2831,  1230,
    5047,  3013, -1404, -1404, -1404,    37, -1404, -1404,   173, -1404,
    1228, -1404, -1404, -1404,  3013,    96,  3013, -1404, -1404,  3013,
   -1404, -1404,  5047,  3013,  1424,  2188, -1404,  6021,  1187,  3013,
   -1404, -1404,   413,   413, -1404,   479,  1178,  1178,  1242,   530,
     551, -1404, -1404,  1166, -1404, -1404, -1404, -1404,  1427, -1404,
    3013, -1404, -1404, -1404, -1404, -1404,  1431, -1404,  1176,  1406,
   -1404,  1423, -1404,  1365,  1367,   553, -1404, -1404, -1404, -1404,
   -1404,  1435, -1404, -1404, -1404, -1404, -1404,  3013,  6380,   530,
    1328,  1329,  1333, -1404,   530, -1404,   847, -1404, -1404,  1303,
   -1404,   500,  1306, -1404, -1404, -1404,   530, -1404,  1121,   369,
   -1404, -1404, -1404,  1425,   369,  1309,   369, -1404,   530,   530,
    1199,   530, -1404,  1039,  3013, -1404,  1377, -1404,   775, -1404,
   -1404, -1404,   780,  1319,  3013,   779,    54, -1404, -1404,  1223,
    1477, -1404, -1404, -1404, -1404,    73,  3013,  1335,  4073, -1404,
   -1404,  1335,  3737, -1404, -1404, -1404,   141, -1404,   197,  1101,
    3976,  3976,  6021,  6021,  1797,  1229,   535,  2066,  6021,  2347,
     476,  2601,  2698,  5366,  2952,  3049,  3303,  6021,  6021,  1489,
    6021,  1231,  6021,  1232,   473,  6021,  6021,  6021,  6021,  6021,
    6021, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,  1438,
    1209,   170,  5047, -1404,  3976,  6021,   439,   640,  1238,   220,
   -1404, -1404, -1404, -1404,  1177,   505, -1404, -1404,  1483,   181,
     181,   791,   791, -1404,   790, -1404, -1404, -1404, -1404,  3013,
    3013,  3013,  3013,  3013,  3013,  1240, -1404,    46, -1404, -1404,
    3883,  1380, -1404,   142,  1399,  3013, -1404,   220,  5047,  1399,
    1240,  1243,  3883,  1905,  1450, -1404, -1404,  2831,  1251,   911,
   -1404, -1404,  1246,  3976,  1455, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404,   558, -1404, -1404, -1404, -1404, -1404,   819,
    1319, -1404, -1404, -1404,  1261, -1404, -1404, -1404,   842, -1404,
     953,  1520, -1404,   479, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404,  1263,  3976,  2249, -1404, -1404,  1490, -1404, -1404,  2249,
   -1404, -1404, -1404, -1404,   553,   553,  1290,  1267, -1404, -1404,
   -1404, -1404, -1404,   380,   243,   716, -1404,  1531, -1404, -1404,
   -1404,  3013,   236,   846,  1374,  1427,  1271, -1404,  1178,   851,
   -1404, -1404,   610, -1404,  1275,  3013,   737,  1282,  1283,   530,
    1287,  1534,   506,  1490,   953,  1260,  2188, -1404,  1237,   220,
    1361,  1370, -1404,  1874, -1404,   530,  1422,  1509, -1404,  6554,
   -1404,   530,  1330,  1511, -1404, -1404,  1295,  1297, -1404,  3976,
    3976, -1404,  3976,  3976, -1404,  1563,   362,  1177,  1301, -1404,
   -1404,  1302,  1177,  5605, -1404, -1404, -1404,  1476,  1304,  1308,
    1177,  1028,  1310,  1313,  1177,  6021,  6021,  1229,  6021,  5685,
    1315,  1320,  1177,  1321,  1323,  1177,  6021,  1177,  1324,  1325,
    1177,  1334,  1340,  1177,  1342,  1343,  1177,   536,   562, -1404,
     606, -1404,   616,  6021, -1404,   648,   657,   722,   730,   872,
     901,  6021,   530,  5047, -1404,  1526,   262,  1177,  1421,  5047,
   -1404,  6021,  6021,  5047,  6021,  6021, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404,  3883,  1542,  3883, -1404,  1240,    46,
   -1404,   555,   555,   381, -1404, -1404,  4312,   858, -1404, -1404,
    1240,  1243,  1346,  1905,   787,  1348,  2831,  1484, -1404,  1485,
    1488,  1535,  5047,  3013,  1607,  1552, -1404, -1404, -1404,  1490,
   -1404,  1361,   456,  1462,   413,  1496,  1178,  1497,  1178, -1404,
    3976,   148,  1075, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404,  1441,  1620,  1490, -1404,  6021, -1404,   743,  1486,  1290,
     223, -1404,  3976, -1404, -1404, -1404,  1510,  1512,  1513,  1514,
    1516,  1517, -1404, -1404, -1404, -1404, -1404,  1369,   236, -1404,
    1242, -1404, -1404,   369, -1404,  2249,   185,   369, -1404,   369,
   -1404,  1427, -1404, -1404,  1371, -1404, -1404,   780,  1620,  1178,
   -1404,  1260, -1404, -1404,  4312,  1626, -1404,   237, -1404,  1381,
   -1404,  1261, -1404,  1382,   413,   172,  1383, -1404,   413, -1404,
   -1404, -1404,  1563, -1404,  1563,  6021,  1494,  1494,   662,  1388,
     864, -1404,  1494,  1494,  6021,  1494,  1494,   378,  1177, -1404,
    1072,  6021,  1177,  1494,  1494,  1494,  1494,  1177,  1494,  1494,
    1494,  1494,  1494,  1494, -1404, -1404,  1389,  3599, -1404, -1404,
     945, -1404, -1404, -1404, -1404, -1404, -1404,  1005,   102, -1404,
   -1404,  5047,  5047,  1599,  1491,  1037,   900,  1284,  1177, -1404,
   -1404,  1533, -1404, -1404, -1404, -1404,  1487, -1404, -1404, -1404,
   -1404, -1404,  1266,  5047, -1404,  1905,  1346, -1404,   911, -1404,
   -1404, -1404,  2831,   917, -1404,  2431,  3976,  1523,  1620, -1404,
   -1404, -1404, -1404,   953, -1404, -1404,   413,   847, -1404, -1404,
     153, -1404, -1404, -1404,  3013,  6257, -1404, -1404,  1177,  1640,
     743, -1404,   814,  3013, -1404,  1584,   156,  1492,  1587,  1495,
    1628,  1589,  1630,  1490, -1404, -1404,  1407, -1404,  1410, -1404,
    1580,  1640,   185, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404,  1490,  1237, -1404,   847,  1531,  1618,  1555, -1404,  1319,
   -1404, -1404, -1404,  1572,    89, -1404,  1427,  1573,  1177,  1420,
    1428,   220, -1404, -1404,  1432,  1436,  1177,  1437,  1440,  6021,
    6021,  1177,  1442,  1447,  1448,  1449,  1451,  1453,  1454,  1458,
    1459,  1461, -1404, -1404, -1404, -1404,   530, -1404, -1404, -1404,
   -1404,  5047, -1404, -1404,  1230, -1404,   555,  1230, -1404,   991,
   -1404, -1404,  1426, -1404, -1404,  1667, -1404, -1404, -1404, -1404,
   -1404,  1419,   168,   398, -1404, -1404, -1404, -1404,  1178, -1404,
   -1404, -1404, -1404,  3599,  3104,   115,  1430,   154,  1439,  1611,
    6021,  1434,  1465,  1470,  1493, -1404, -1404,  1653,  6159, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404,  1668,  1501,  1502,  1503,  1506, -1404,  1505,  3013,
   -1404, -1404,  1657,  1666,  3013,  1290, -1404, -1404,   553,  3013,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404,  1620, -1404, -1404,
   -1404, -1404, -1404,  1620, -1404, -1404, -1404, -1404,  2859,  1230,
   -1404, -1404,  6021,   413, -1404, -1404, -1404,   413, -1404, -1404,
    6021, -1404, -1404, -1404, -1404,  1177,  1177, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404, -1404,  1507, -1404,  1455,
   -1404,  1455,  3976,  2431,  3013,  3208, -1404, -1404,  1643, -1404,
    1515, -1404, -1404, -1404,   125, -1404,  5924,  3013, -1404, -1404,
    6021, -1404,   489,  1672,  3976,  1177, -1404,  3976, -1404, -1404,
   -1404,   238, -1404,  1562, -1404,  2662, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404,  1290,   553, -1404,  1632, -1404, -1404, -1404,
   -1404,  1531, -1404,  2859,  1508,   911, -1404, -1404,  1455,  1177,
   -1404, -1404,  1284, -1404,  1552,  1552,   168, -1404, -1404, -1404,
    3151,  2249, -1404,  1518, -1404,  1164,  4631,  1057,  6021,  2662,
     162,  1519,   166, -1404,  3013,  3013,   236,   129, -1404, -1404,
   -1404,   756, -1404, -1404,  1632,   290,  1290,  1634,  1635, -1404,
   -1404,   787,  1522,  2859,  1676,  1552,  1525,  1230,  1523,  1523,
   -1404,  1527,  1521,   816,  3013, -1404, -1404, -1404,  1624,  2662,
   -1404,  1225,   140,  1600, -1404,  1733, -1404, -1404, -1404, -1404,
    6257,   238, -1404,  2662,  1290,   437,   437, -1404,  1737, -1404,
    1597, -1404, -1404, -1404,   911,  2859, -1404, -1404,  1455,  1656,
    1656, -1404,  3151,  1524, -1404,  3013, -1404, -1404,  2754,  1529,
     830,  2662,  1751,  1750,  6257,  6257, -1404, -1404, -1404, -1404,
   -1404,  1596,   436, -1404, -1404, -1404,   997,  1552, -1404, -1404,
   -1404,  3013, -1404,  2662,  1538, -1404, -1404,   138,  3013,  6257,
    1759, -1404, -1404, -1404, -1404,  3976,  1523,  1541,  1544,   930,
    6257, -1404, -1404,  6257,   168,  1656, -1404,  3013, -1404, -1404,
   -1404, -1404, -1404
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -1404, -1404, -1404, -1404,   -85,   179, -1404,   191,  1553,   629,
   -1404,  -306, -1404, -1404,  -558,    30,  -650,  -959,  1804,  1556,
    1546,  1111, -1404, -1404, -1404,   736, -1404,  1188,   967, -1404,
     740, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404,   752, -1404, -1404, -1404, -1404,
   -1404,   387, -1404, -1404,  1504, -1404, -1404, -1404,  -338, -1404,
   -1404, -1404,  1473, -1404, -1404,  1252,  -229,  1536, -1404, -1404,
    -322, -1404,  -706, -1404, -1404, -1404, -1404, -1404, -1404,   821,
   -1404,  -877, -1404, -1404,  1532,  1082,   760,  1540,  1053,   759,
   -1404,   405,   -52, -1404, -1404,  -313,   203,  -991, -1404, -1404,
     428, -1404,  1543, -1019, -1404, -1404, -1404, -1404, -1190,   427,
     133,   127,   143,   105, -1404, -1404, -1404, -1404,  -112,  -323,
   -1404,  1088,  -945, -1404,   630, -1404, -1404, -1403, -1119, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404,   229, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404,  -268,   182, -1404, -1138, -1404, -1404,
   -1404, -1404,   226, -1404,   130, -1404, -1404, -1404,   890, -1404,
   -1115, -1404, -1404, -1118,   210,  -652,   495,  -276,  -915,   671,
   -1404, -1404,  -518,  -736, -1404, -1404,  -966, -1274, -1404, -1404,
   -1404, -1404, -1404,  1564, -1404,  1332,  1557, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404,  1574, -1404, -1404, -1404, -1404,
   -1404, -1404,  -527,  -460, -1404,  1066,   623,  -540,  -339, -1404,
   -1404, -1404,  -315,  -903, -1404,  -336, -1404, -1404, -1404, -1404,
   -1404,  1285, -1404, -1404,  1078, -1404, -1404, -1404, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404,  1890, -1404, -1404, -1404, -1404,
   -1404,  1608, -1404, -1404, -1404,  1413, -1404,  1411, -1404, -1404,
   -1404, -1404,  1167, -1404, -1404, -1404,    10, -1404,  -988, -1404,
    1157,  -478, -1404, -1404, -1404, -1404, -1404,    27,    -1,  -184,
     -23, -1404, -1404,  -622, -1404,  1771,  -819,  -986, -1404,  1185,
   -1231, -1404,  -833,   939, -1404, -1404,  -394,  -240,   602,   -38,
    -965, -1108, -1404,   284, -1404, -1169,  -494, -1139,   395, -1404,
     137, -1404,   176, -1404,    84,  1922, -1404,  1923, -1404, -1404,
    1417,  1925, -1404, -1404, -1404,  -205,  -634, -1404,  1210, -1060,
     961, -1404,   -98,  -638,  -606, -1404, -1404, -1404, -1230,   -26,
    -418,  -627, -1404,  -401, -1404, -1404, -1404,   370, -1404, -1404,
   -1404, -1404, -1404, -1404, -1404, -1404,  -490, -1404,   807, -1404,
     655, -1404,  1044,  -923, -1404,  -600, -1404,   711,  -912,  -116,
    -904,  -896,  -894,   -50,     1,     0, -1404,   -51,  -770,  -454,
    -111, -1404, -1404, -1404, -1235, -1212, -1211, -1404, -1209, -1404,
   -1404, -1404, -1404, -1404, -1404,   777, -1404,  -996, -1404,   307,
    -261,   619, -1404,   880, -1347,  -682,  1386,   139, -1404,   -56,
    1868, -1404,    -6,   -53,  1227,   -24,    -4,  -132,   -15,    -3,
    -166, -1404,   -59,   -47,    -9, -1404
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -975
static const short yytable[] =
{
     127,   148,   191,   127,    79,    78,   260,   149,   213,   155,
      41,   209,   207,   750,   575,   425,   308,   222,   547,   196,
     947,   611,   279,   282,   615,   491,   688,   280,   283,   244,
     790,   224,   194,   899,   585,   253,   248,   210,  1246,   736,
     250,   758,   626,   219,   187,   189,   192,   195,   149,   200,
     715,  1053,   921,   545,   212,   189,   216,  1009,   195,  1424,
     149,   200,   641,   229,   127,   929,   249,   226,   148,   231,
     971,   997,   240,   255,   149,   243,   189,   251,   240,   216,
     192,   195,   247,   149,   200,   229,   127,   776,   195,   779,
     252,   254,   267,   315,   967,   537,   483,   791,   464,   861,
    1234,   944,   508,   266,  1000,   268,  1234,   365,   464,   316,
    1174,  1236,  1293,   313,   852,  1247,  1078,  1236,   996,  1237,
     212,   149,   212,  1416,  1253,  1237,   311,  1238,  1288,  1239,
     372,  1026,  1378,  1238,   216,  1239,  1504,   290,   334,   309,
    1033,   195,   149,   229,  1204,  1662,  1276,   155,  1196,  1099,
    1792,  1507,  1271,  1004,   835,   870,   880,  1036,   284,   762,
    1517,   880,   970,   361,   880,  1515,  1496,  1359,   721,  1453,
     880,  1029,   274,  1363,   880,   511,   880,   745,   880,  1760,
     425,  1597,  1460,  1518,  1519,   473,  1520,   319,  1830,   754,
    1604,   133,  1464,  1073,  1085,   766,   767,   768,   769,   512,
     232,   900,   497,   134,   764,  1102,   378,   695,  1191,  -367,
    1382,   318,   696,  1652,   840,   695,   335,   514,   611,   710,
     696,   615,  1671,   458,   264,  1370,   284,  1373,  1422,   629,
     740,   455,   458,    60,   232,   984,    97,   233,   494,   810,
     716,   276,   -81,  1459,   869,   991,  1185,  1713,   212,   877,
     640,   131,   149,  1104,  1105,   465,   127,   504,   484,   585,
    1294,   149,   992,  1647,   366,   465,   492,   717,   987,   285,
     880,   233,   822,  1100,  1398,    83,   811,    76,   749,  1525,
     881,   509,   192,   801,  1439,   881,   149,  1698,   881,   711,
    1258,   156,   841,   336,   881,   709,  1714,  1166,   881,   322,
     881,   741,   881,   576,   546,   578,  1648,  1715,  1456,  1440,
    1189,   552,   265,   210,   272,   562,  1186,    14,   566,   630,
     634,   212,  1201,  1202,  1192,  1086,   273,  1605,   212,  1103,
     212,   833,   212,   632,  1198,   567,   511,  1765,   623,    77,
     922,   923,   203,   204,  1601,   520,  1219,   323,  1040,  1434,
     695,  1163,  1234,   195,   200,   696,   635,  1796,   499,   497,
     625,   367,   149,  1236,   192,  1498,  1499,   695,    14,  1649,
    1315,  1237,   696,  1388,  1497,  1059,   265,    79,  1449,  1238,
     812,  1239,  1098,   746,  1465,  1675,  1479,  1701,   695,  1255,
     722,  1820,  1821,   696,   881,  1231,  1248,  1249,  1517,  1761,
    1079,  1422,  1188,  1515,   695,   836,   491,  1730,  1763,   696,
    1763,  1716,   692,  1101,  1200,   763,  1832,   974,   705,   901,
    1411,  1518,  1519,  1381,  1520,  1531,  1373,  1839,  1580,   713,
    1840,  1259,   724,   719,  1753,  1722,   708,   458,  1755,   831,
      76,   782,   509,  1361,   928,  1300,  1630,   149,   699,   700,
     708,   195,   465,   933,   697,   698,   699,   700,   149,   324,
     498,  1737,   149,   816,  1256,   195,   149,   869,  1679,   491,
     155,   869,  1311,  1312,  1680,  1313,  1314,  1301,  1766,  1722,
     733,   765,  1823,  1723,   924,   695,  1594,  1636,  1587,   327,
     696,   491,  1169,  1170,  1125,  1021,  1532,   284,  1023,   695,
    1437,  1567,    77,  1568,   696,  1628,  1593,   832,   640,   345,
    1126,   843,  1637,  1320,  1031,   499,    98,   793,  1405,  1722,
    1071,  1694,   157,  1695,   817,  1738,  1739,   818,   158,   159,
    1530,   792,   500,  1799,  1824,    76,  1767,   328,  1595,  1047,
     897,  1020,   975,  1089,   995,   464,   785,  1195,   212,   861,
    1093,  1452,   785,  1386,  1287,  1127,   129,    99,  1722,  1509,
    1801,  1722,   695,  1606,   143,  1731,  1776,   696,  1257,   100,
    1638,  1752,   150,   276,  1800,    99,   533,  1596,   846,   847,
    1735,  1010,   160,  1722,  1128,  1017,   346,    79,   695,   697,
     698,   699,   700,   696,   101,   276,   726,    77,   794,  1779,
    1780,  1802,   821,  1410,   151,   210,   697,   698,   699,   700,
    1641,  1790,   101,   932,   731,  1774,  1643,   511,   146,   152,
     203,   204,   212,   130,   988,  1426,   534,   697,   698,   699,
     700,   864,   695,  1116,  1451,  1058,  1171,   696,  1826,   795,
      79,   862,   695,   697,   698,   699,   700,   696,  1803,   329,
    1814,  1129,  1610,  1817,  1011,  1015,  1016,  1806,   278,  1117,
     535,   216,   277,   102,   903,  1678,   695,   330,   278,   144,
    1807,   696,  1226,   103,   695,  1829,   145,   948,   536,   696,
    1648,   102,  1682,   695,   276,   241,   937,  1835,   696,    80,
      81,   103,   362,   212,   212,  1400,  1471,   465,   104,   708,
    1529,   331,   465,   201,  1017,  1401,   504,   491,   994,  1216,
    1745,   465,  1007,  1007,   195,   276,   104,   543,  1019,   195,
    1734,   217,   733,   509,   697,   698,   699,   700,   363,   210,
     544,   976,   640,   455,    76,   149,  1289,   149,   697,   698,
     699,   700,   734,  1649,   212,  1154,   212,   235,   695,  1508,
     212,   281,  1245,   696,   552,   237,   695,   278,  1042,    76,
    1217,   696,  1240,  1260,  1254,  1729,  1734,  -242,  1240,  1522,
    1052,   212,  1425,  1089,  1736,  1175,   555,   735,  1234,  1366,
    1176,  1038,  1041,  1178,  1275,   568,  1181,  1067,  1068,  1236,
    1070,  1808,  1809,   738,   284,   284,   569,  1237,   212,   212,
    1261,   697,   698,   699,   700,  1238,  1072,  1239,  1344,  1734,
     105,   256,   296,   292,  1046,  1772,  1083,   695,  1393,  1438,
    1493,    77,   696,  -832,   257,  1448,  1056,   697,   698,   699,
     700,  -832,   278,   511,  1345,   149,  1347,   905,   907,   106,
     107,  1734,   108,   339,   245,   149,   340,  1841,   293,   109,
     110,   111,   112,   113,  1207,   114,   376,  1095,   115,   116,
     117,   118,   119,   261,   381,    79,  1096,  -242,  1208,   262,
    1097,   697,   698,   699,   700,    52,   294,   263,  1348,   295,
    1277,   697,   698,   699,   700,  1278,    53,  -832,  1349,  -242,
    -832,   341,  1209,    54,  1182,   296,  -242,  -832,   695,  1783,
    -832,   138,   139,   696,  -832,   697,   698,   699,   700,  -832,
    1172,   284,  -832,   697,   698,   699,   700,   703,    55,   704,
    1351,   297,   697,   698,   699,   700,  1179,   695,  -832,  1352,
    -242,   269,   696,   425,   270,   491,  1784,   271,  1572,   509,
     465,   195,   200,   465,   229,   127,  1210,   988,  1180,  1785,
    1183,   465,  -832,   947,   909,  -832,  -832,   524,   139,   780,
    1573,   781,   291,   465,   465,  -832,   525,  1574,   195,   764,
     307,   695,   425,   898,   275,   902,   696,   317,  1207,   906,
     908,   910,  -832,    56,  1048,  1049,  1433,   697,   698,   699,
     700,   693,  1208,   930,  1353,   697,   698,   699,   700,   320,
    -682,  1575,  1354,     1,  -832,  1696,   337,  1277,   611,  -832,
    1629,   615,  1281,  1631,  1240,    57,  1209,  1455,  1284,    58,
    -833,  -832,   348,  -832,     2,  1762,  1763,  1710,  -833,   350,
    1712,   695,     3,   491,  1302,   803,   696,   804,   369,     4,
    1306,     5,  1266,     6,   965,  1076,   966,  1077,     7,   857,
     210,  1084,    79,  1267,   695,     8,  1266,   455,  1207,   696,
    1175,   359,  1177,   695,  1207,  1528,   371,   212,   696,     9,
    1210,   351,  1208,  1644,   212,  1377,  1377,  1380,  1208,   374,
      10,  1394,   373,   695,  -833,  1324,   377,  -833,   696,  1076,
     378,  1220,  -832,  -832,  -833,  -832,  1209,  -833,   695,  1816,
    1763,  -833,  1209,   696,   460,  1688,  -833,   467,   352,  -833,
     353,  1358,  1224,   471,  1225,  1407,  1268,  1409,  1269,   882,
    1632,  1273,   491,  1274,   468,  -833,  1825,   695,  1383,  1480,
    1384,   472,   696,   354,  1175,   883,  1473,   697,   698,   699,
     700,   474,   475,  1749,  1355,   476,   801,    11,   478,  -833,
    1210,   496,  -833,  -833,    12,  -834,  1210,    13,   355,    14,
      15,   493,  -833,  -834,   505,   495,   697,   698,   699,   700,
     693,   884,  1503,  1356,  1007,   465,   515,   465,  1454,  -833,
     885,    14,   521,   522,    16,  1602,   532,   693,   886,  1510,
     695,    79,    79,   540,   465,   696,   553,   195,  1834,   563,
    1763,  -833,  1838,   695,   309,   560,  -833,   887,   696,   888,
     697,   698,   699,   700,  1369,   573,  1372,  1494,  -833,  -834,
    -833,   577,  -834,   889,   627,   890,   891,   892,   633,  -834,
     631,   666,  -834,    17,   667,   695,  -834,   668,   669,   882,
     696,  -834,   670,  1778,  -834,   671,   672,   695,    18,    19,
     676,   695,   696,   673,  1463,   883,   696,  1119,  1467,  1123,
    -834,  1131,  1134,  1521,  1139,  1142,  1145,   677,    79,  1436,
     697,   698,   699,   700,   674,  1162,   893,  1495,   397,   398,
     399,  -958,   675,   511,  -834,   678,   683,  -834,  -834,  -833,
    -833,   884,  -833,   697,   698,   699,   700,  -834,   684,   428,
     885,   701,   697,   698,   699,   700,   407,   408,   886,  1502,
     695,  1791,   693,   706,  -834,   696,   702,  -974,  1681,   694,
     720,   481,   697,   698,   699,   700,  1750,   887,   723,   888,
     490,   711,   728,   730,   744,   748,  -834,   697,   698,   699,
     700,  -834,   752,   889,   753,   890,   891,   892,   755,   756,
     728,   757,   759,  -834,   524,  -834,  1007,   761,    98,   770,
     771,   519,  1533,   894,   895,   896,   697,   698,   699,   700,
     533,   773,   774,   932,   783,   807,   465,   809,   808,  -426,
     814,   819,   276,   195,   823,  1578,   825,   824,   210,   509,
    1377,   579,   826,   828,   829,   844,   893,  1720,   834,    99,
     838,   839,   845,   854,   856,   212,   857,   865,   866,   867,
     871,   100,   925,   931,   149,   960,   511,  1642,   961,   963,
     534,   968,   989,   972,  -834,  -834,   973,  -834,   983,   697,
     698,   699,   700,  1747,   998,  1002,   101,  1022,    62,   524,
    1240,  1720,   697,   698,   699,   700,  1017,  1028,   525,    63,
     835,  1027,  1029,   786,   535,  1034,    64,  1035,   680,  1037,
    1043,  1044,   685,   686,   690,  1045,   425,  1051,  1054,    65,
    1069,  1060,   536,  1065,   697,   698,   699,   700,  1646,  1074,
    1676,  1720,  1079,   894,   895,   896,   697,   698,   699,   700,
     697,   698,   699,   700,  1091,  1798,  1627,  1092,   716,  1149,
    1113,  1521,  1153,  1151,  1161,   102,    79,    84,  1173,   695,
    1184,  1190,  1193,  1186,    85,   103,  1203,  1212,    86,    87,
    1720,  1206,   509,  1720,   725,   189,  1677,    88,   727,  1214,
      66,   703,  1227,  1521,  1230,  1241,  1674,  1250,  1252,    89,
     104,  1262,  1270,  1272,  1286,  1720,    67,  1653,  1279,   697,
     698,   699,   700,  1690,  1282,  1283,  1297,  1691,   105,  1285,
     937,  1295,  1085,    68,  1304,   149,  1308,  1309,  1307,  1310,
     192,   880,  1316,  1317,  1321,  1322,  1724,  1521,    69,  1323,
    1360,  1325,    70,   148,  1326,    71,  1333,   106,   107,   149,
     108,  1334,  1335,    72,  1336,  1338,  1339,   109,   110,   111,
     112,   113,  1362,   114,    90,  1340,   115,   116,   117,   118,
     119,  1341,    73,  1342,  1343,  1371,  1385,  1521,  1389,  1390,
    1387,    91,  1391,  1706,  1392,   937,   212,  1395,   511,  1396,
    1403,  1521,  1406,  1413,  1408,    98,    92,  1415,   195,  1423,
      93,  1435,  1427,  1450,  1428,  1429,  1430,   945,  1431,  1432,
    1457,    94,  1461,  1500,  1462,  1466,  1521,  1318,  1756,  1521,
    1472,  1492,   511,   511,  1505,  1523,  1569,  1579,   148,  1506,
      95,  1582,  1501,  1585,   149,  1584,    99,  1586,  1590,  1588,
    1581,  1521,  1589,  1583,  1598,  1744,  1600,   511,   100,  1603,
    1607,   149,  1608,  1634,   879,  1635,  1633,   130,   511,  1651,
    1609,   511,   132,  1656,  1611,   189,  1758,  1660,  1612,  1613,
    1654,   490,  1614,   101,  1617,   904,    79,  1759,   148,  1618,
    1619,  1620,   920,  1621,   149,  1622,  1623,   927,   428,  1744,
    1624,  1625,  1812,  1626,   509,   149,  1657,   428,   934,  1658,
     938,   939,   940,   941,   942,   934,   911,   912,   913,   914,
     915,   916,   917,   918,  1665,  1672,  1673,  1699,  1709,  1661,
     148,  1725,  1659,  1768,  1770,  1775,   149,  1744,   509,   509,
    1666,  1667,  1668,   149,   490,  1669,   192,  1670,  1733,  1693,
    1788,  1794,   102,  1795,  1765,  1766,  1700,  1746,  1754,   159,
    1818,  1782,   103,   509,  1773,  1811,   490,  1777,  1815,  1781,
    1819,   519,   192,  1108,   509,  1822,  1109,   509,  1763,  1095,
     382,   105,  1833,  1836,  1837,   459,   136,   104,  1374,   383,
     477,   466,   962,   855,  1090,  1305,  1303,  1291,   192,  1592,
     586,   541,   384,   820,  1229,   105,  1292,   999,   385,   386,
     106,   107,  1299,   108,  1032,   513,  1591,  1702,  1571,  1577,
     109,   110,   111,   112,   113,  1771,   114,  1764,   527,   115,
     116,   117,   118,   119,   106,   107,   387,   108,  1025,   388,
    1769,  1805,   946,  1417,   109,   110,   111,   112,   113,   556,
     114,  1708,   558,   115,   116,   117,   118,   119,  1748,  1719,
    1221,  1797,  1399,  1732,  1526,   806,   548,   390,   391,   539,
    1447,   564,  1066,   137,   786,   842,  1057,   506,   739,   392,
     743,   990,   289,   393,  1001,   985,  1205,  1697,  1639,  1810,
    1787,  1842,    44,    45,   751,    48,  1106,  1107,  1112,   969,
    1199,  1120,  1121,  1124,  1329,  1132,  1135,  1137,  1140,  1143,
    1146,  1147,  1148,  1412,  1150,  1280,  1152,   802,  1364,  1155,
    1156,  1157,  1158,  1159,  1160,   246,     0,  1831,   986,     0,
       0,     0,     0,     0,     0,     0,   428,     0,  1110,  1167,
     394,  1469,  1470,     0,     0,     0,   395,  1474,  1475,   949,
    1477,  1478,     0,     0,     0,     0,   396,     0,  1482,  1483,
    1484,  1485,     0,  1486,  1487,  1488,  1489,  1490,  1491,     0,
     397,   398,   399,   400,   203,   204,     0,     0,     0,     0,
       0,     0,   490,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   106,   411,     0,   108,     0,     0,   412,   413,
       0,   414,   415,   416,   111,   112,   113,   417,   114,     0,
       0,   115,   116,   117,   118,   119,     0,     0,     0,     0,
       0,     0,   418,   419,     0,     0,     0,     0,   480,     0,
     421,     0,  1118,     0,   278,  1109,     0,     0,     0,   382,
     105,     0,     0,     0,     0,     0,     0,     0,   383,     0,
       0,     0,     0,     0,   952,     0,     0,     0,     0,     0,
       0,   384,     0,     0,     0,     0,     0,   385,   386,   106,
     107,   105,   108,     0,     0,     0,     0,     0,     0,   109,
     110,   111,   112,   113,     0,   114,     0,     0,   115,   116,
     117,   118,   119,     0,     0,   387,     0,     0,   388,     0,
     106,   107,     0,   108,     0,   257,     0,     0,     0,     0,
     109,   110,   111,   112,   113,     0,   114,   934,     0,   115,
     116,   117,   118,   119,     0,     0,   390,   391,     0,  1327,
    1328,     0,  1330,  1332,     0,     0,     0,     0,   392,     0,
    1337,     0,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1350,     0,     0,
       0,     0,     0,     0,     0,  1357,     0,   428,     0,     0,
     456,     0,     0,   428,     0,  1167,  1365,   428,  1367,  1368,
       0,   555,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,  1110,     0,   394,
     490,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   396,   428,     0,     0,     0,
       0,     0,     0,     0,     0,   106,   107,     0,   108,   397,
     398,   399,   400,   203,   204,   109,   110,   111,   112,   113,
       0,   114,     0,   401,   115,   116,   117,   118,   119,  1418,
     385,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   106,   411,     0,   108,     0,     0,   412,   413,     0,
     414,   415,   416,   111,   112,   113,   417,   114,   387,     0,
     115,   116,   117,   118,   119,     0,     0,     0,     0,     0,
       0,   418,   419,     0,     0,     0,     0,   480,   490,   421,
       0,     0,     0,   278,   561,     0,     0,   389,     0,     0,
       0,     0,     0,  1122,     0,     0,  1109,     0,     0,  1468,
     382,     0,     0,     0,     0,     0,     0,     0,  1476,   383,
       0,     0,     0,     0,     0,  1481,     0,     0,     0,     0,
       0,     0,   384,     0,     0,     0,     0,     0,   385,   386,
       0,     0,     0,     0,   105,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   428,   428,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   387,     0,     0,   388,
       0,     0,   394,   106,   107,     0,   108,   490,     0,     0,
       0,     0,     0,   109,   110,   111,   112,   113,   396,   114,
       0,     0,   115,   116,   117,   118,   119,   390,   391,     0,
       0,     0,   397,   398,   399,     0,   203,   204,     0,   392,
       0,     0,     0,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,   404,   405,   406,
     407,   408,   409,   410,     0,     0,     0,     0,     0,     0,
     412,   413,     0,     0,     0,     0,     0,     0,     0,   417,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1232,     0,   105,  1110,     0,
     394,     0,     0,  1615,  1616,     0,   395,     0,     0,     0,
       0,   390,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,   428,   106,   107,     0,   108,
     397,   398,   399,   400,   203,   204,   109,   110,   111,   112,
     113,     0,   114,     0,   401,   115,   116,   117,   118,   119,
       0,     0,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   106,   411,  1655,   108,     0,     0,   412,   413,
       0,   414,   415,   416,   111,   112,   113,   417,   114,     0,
       0,   115,   116,   117,   118,   119,     0,  1130,     0,     0,
    1109,     0,   418,   419,   382,     0,     0,     0,   480,     0,
     421,     0,     0,   383,   278,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,   384,   400,     0,     0,
       0,     0,   385,   386,     0,     0,  1689,     0,   401,     0,
       0,     0,     0,     0,  1692,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,   411,     0,   108,
     387,     0,     0,   388,     0,   414,   415,   416,   111,   112,
     113,     0,   114,     0,     0,   115,   116,   117,   118,   119,
    1705,     0,     0,     0,  1707,     0,     0,   461,     0,     0,
       0,   390,   391,     0,  1133,     0,     0,  1109,     0,     0,
       0,   382,     0,   392,   105,     0,     0,   393,     0,     0,
     383,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,     0,     0,     0,     0,     0,   385,
     386,     0,     0,   106,   107,     0,   108,     0,     0,     0,
     428,     0,  1751,   109,   110,   111,   112,   113,     0,   114,
       0,     0,   115,   116,   117,   118,   119,   387,     0,     0,
     388,     0,  1110,     0,   394,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,   390,   391,
       0,     0,     0,     0,   397,   398,   399,   400,   203,   204,
     392,     0,     0,     0,   393,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   106,   411,     0,   108,
       0,     0,   412,   413,     0,   414,   415,   416,   111,   112,
     113,   417,   114,     0,     0,   115,   116,   117,   118,   119,
       0,     0,     0,     0,     0,     0,   418,   419,   105,  1110,
       0,   394,   480,     0,   421,     0,     0,   395,   278,     0,
       0,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,   107,     0,
     108,   397,   398,   399,   400,   203,   204,   109,   110,   111,
     112,   113,     0,   114,     0,   401,   115,   116,   117,   118,
     119,     0,     0,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   106,   411,   421,   108,     0,     0,   412,
     413,     0,   414,   415,   416,   111,   112,   113,   417,   114,
       0,     0,   115,   116,   117,   118,   119,     0,  1138,     0,
     105,  1109,     0,   418,   419,   382,     0,     0,     0,   480,
       0,   421,     0,     0,   383,   278,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   384,     0,   106,
     107,     0,   108,   385,   386,     0,     0,     0,     0,   109,
     110,   111,   112,   113,     0,   114,     0,     0,   115,   116,
     117,   118,   119,     0,     0,     0,     0,     0,     0,     0,
       0,   387,     0,     0,   388,  1813,     0,   421,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   390,   391,     0,  1141,     0,     0,  1109,     0,
       0,     0,   382,     0,   392,   105,   106,   107,   393,   108,
       0,   383,     0,     0,     0,     0,   109,   110,   111,   112,
     113,     0,   114,     0,   384,   115,   116,   117,   118,   119,
     385,   386,     0,     0,   106,   107,     0,   108,     0,     0,
       0,     0,   977,     0,   109,   110,   111,   112,   113,     0,
     114,     0,     0,   115,   116,   117,   118,   119,   387,     0,
       0,   388,     0,  1110,     0,   394,     0,     0,     0,     0,
    1683,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,   390,
     391,     0,     0,     0,     0,   397,   398,   399,   400,   203,
     204,   392,     0,     0,     0,   393,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   106,   411,     0,
     108,     0,     0,   412,   413,     0,   414,   415,   416,   111,
     112,   113,   417,   114,     0,     0,   115,   116,   117,   118,
     119,     0,     0,     0,     0,     0,     0,   418,   419,   105,
    1110,     0,   394,   480,     0,   421,     0,     0,   395,   278,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
    1636,     0,     0,     0,     0,     0,     0,     0,   106,   107,
       0,   108,   397,   398,   399,   400,   203,   204,   109,   110,
     111,   112,   113,     0,   114,  1637,   401,   115,   116,   117,
     118,   119,     0,     0,     0,   402,   403,   404,   405,   406,
     407,   408,   409,   410,   106,   411,     0,   108,     0,     0,
     412,   413,     0,   414,   415,   416,   111,   112,   113,   417,
     114,     0,     0,   115,   116,   117,   118,   119,     0,  1144,
     105,     0,  1109,     0,   418,   419,   382,     0,     0,     0,
     480,     0,   421,  1638,     0,   383,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   384,   106,
     107,     0,   108,     0,   385,   386,     0,     0,     0,   109,
     110,   111,   112,   113,     0,   114,     0,   105,   115,   116,
     117,   118,   119,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   387,  1645,     0,   388,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,   107,     0,   108,
       0,     0,     0,     0,     0,     0,   109,   110,   111,   112,
     113,     0,   114,   390,   391,   115,   116,   117,   118,   119,
       0,     0,     0,   382,   105,   392,     0,     0,     0,   393,
       0,     0,   383,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   384,    76,     0,   105,     0,
       0,   385,   386,   106,   107,     0,   108,     0,     0,     0,
       0,     0,     0,   109,   110,   111,   112,   113,     0,   114,
     644,     0,   115,   116,   117,   118,   119,   106,   107,   387,
     108,     0,   388,     0,  1110,   946,   394,   109,   110,   111,
     112,   113,   395,   114,     0,     0,   115,   116,   117,   118,
     119,     0,   396,     0,     0,     0,     0,     0,    77,     0,
     390,   391,     0,     0,     0,     0,   397,   398,   399,   400,
     203,   204,   392,     0,     0,     0,   393,     0,     0,     0,
     401,     0,     0,     0,   872,     0,     0,     0,     0,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   106,   411,
       0,   108,     0,     0,   412,   413,     0,   414,   415,   416,
     111,   112,   113,   417,   114,     0,     0,   115,   116,   117,
     118,   119,     0,   687,     0,     0,     0,   646,   418,   419,
       0,     0,     0,   394,   480,     0,   421,     0,     0,   395,
     278,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,   398,   399,   400,   203,   204,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   587,     0,
       0,     0,   588,     0,     0,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   106,   411,     0,   108,     0,
     589,   412,   413,   590,   414,   415,   416,   111,   112,   113,
     417,   114,     0,   382,   115,   116,   117,   118,   119,   873,
     874,   875,   383,     0,     0,   418,   419,     0,     0,     0,
       0,   650,     0,   421,     0,   384,     0,   278,   643,     0,
       0,   385,   386,     0,   591,     0,     0,     0,     0,     0,
     592,   593,   594,     0,   595,   596,   597,     0,   598,     0,
     644,     0,     0,     0,     0,     0,     0,     0,     0,   387,
       0,     0,   388,     0,     0,     0,     0,     0,   599,     0,
     600,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     390,   391,     0,     0,     0,     0,     0,     0,     0,     0,
     382,   601,   392,     0,     0,     0,   393,     0,     0,   383,
       0,     0,     0,     0,   645,     0,     0,     0,   602,     0,
       0,     0,   384,     0,     0,     0,     0,     0,   385,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     603,     0,     0,     0,     0,     0,     0,   644,     0,     0,
       0,     0,     0,     0,     0,     0,   387,   646,     0,   388,
       0,     0,     0,   394,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,   604,   605,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,   390,   391,     0,
       0,     0,   606,   397,   398,   399,   400,   203,   204,   392,
       0,     0,     0,   393,     0,     0,     0,   401,     0,     0,
       0,   868,     0,     0,     0,     0,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   106,   411,     0,   108,     0,
       0,   412,   413,     0,   414,   415,   416,   111,   112,   113,
     417,   114,     0,     0,   115,   116,   647,   648,   649,     0,
       0,     0,     0,     0,   646,   418,   419,     0,     0,     0,
     394,   650,     0,   421,     0,     0,   395,   278,     0,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,   398,   399,   400,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,   949,     0,     0,
       0,     0,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   106,   411,     0,   108,     0,     0,   412,   413,
       0,   414,   415,   416,   111,   112,   113,   417,   114,   382,
       0,   115,   116,   647,   648,   649,     0,     0,   383,     0,
       0,     0,   418,   419,     0,     0,     0,     0,   650,     0,
     421,   384,     0,     0,   278,     0,     0,   385,   386,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   950,     0,     0,     0,     0,   644,     0,     0,     0,
       0,     0,     0,     0,     0,   387,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   951,     0,     0,
       0,     0,   952,     0,     0,     0,   390,   391,   953,     0,
       0,     0,     0,     0,     0,     0,   382,     0,   392,   105,
       0,     0,   393,     0,     0,   383,     0,   954,     0,     0,
     645,     0,     0,     0,     0,     0,     0,     0,   384,     0,
       0,     0,     0,     0,   385,   386,     0,     0,   106,   107,
       0,   108,     0,     0,     0,     0,     0,     0,   109,   110,
     111,   112,   113,   644,   114,     0,     0,   115,   116,   117,
     118,   119,   387,   646,     0,   388,     0,     0,     0,   394,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,   390,   391,     0,     0,     0,     0,   397,
     398,   399,   400,   203,   204,   392,     0,     0,     0,   393,
       0,     0,     0,   401,     0,     0,     0,   868,     0,     0,
       0,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   106,   411,     0,   108,     0,     0,   412,   413,     0,
     414,   415,   416,   111,   112,   113,   417,   114,     0,     0,
     115,   116,   647,   648,   649,     0,     0,     0,     0,     0,
     646,   418,   419,     0,     0,     0,   394,   650,     0,   421,
       0,     0,   395,   278,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   398,   399,   400,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
     401,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   106,   411,
       0,   108,     0,     0,   412,   413,     0,   414,   415,   416,
     111,   112,   113,   417,   114,   382,     0,   115,   116,   117,
     118,   119,     0,     0,   383,     0,     0,     0,   418,   419,
       0,     0,     0,     0,   650,     0,   421,   384,     0,     0,
     278,     0,     0,   385,   386,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   387,     0,     0,   388,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   382,     0,     0,     0,     0,
     389,     0,   390,   391,   383,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   392,     0,     0,   384,   393,     0,
     105,     0,     0,   385,   386,     0,     0,     0,     0,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,     0,   179,   180,   106,
     107,   387,   108,     0,   388,   181,   182,     0,     0,   109,
     110,   111,   112,   113,   183,   114,   184,     0,   115,   116,
     117,   118,   119,     0,     0,   394,     0,     0,     0,     0,
     389,   395,   390,   391,     0,     0,     0,     0,     0,     0,
       0,   396,     0,     0,   392,     0,     0,     0,   393,     0,
       0,     0,     0,     0,     0,   397,   398,   399,   400,   203,
     204,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   106,   411,     0,
     108,     0,     0,   412,   413,   687,   414,   415,   416,   111,
     112,   113,   417,   114,     0,   394,   115,   116,   117,   118,
     119,   395,     0,     0,     0,     0,     0,   418,   419,   485,
       0,   396,     0,   480,     0,   421,     0,     0,     0,   278,
       0,     0,     0,     0,     0,   397,   398,   399,   400,   203,
     204,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   106,   411,     0,
     108,     0,     0,   412,   413,     0,   414,   415,   416,   111,
     112,   113,   417,   114,   382,     0,   115,   116,   117,   118,
     119,     0,     0,   383,     0,     0,     0,   418,   419,     0,
       0,     0,     0,   480,     0,   421,   384,     0,     0,   278,
       0,     0,   385,   386,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     387,     0,     0,   388,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   389,
       0,   390,   391,     0,     0,     0,     0,     0,     0,     0,
       0,   382,     0,   392,     0,     0,     0,   393,     0,     0,
     383,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   384,     0,     0,     0,     0,     0,   385,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   387,     0,     0,
     388,     0,     0,     0,   394,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,   390,   391,
       0,     0,     0,     0,   397,   398,   399,   400,   203,   204,
     392,     0,     0,     0,   393,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   106,   411,     0,   108,
       0,     0,   412,   413,     0,   414,   415,   416,   111,   112,
     113,   417,   114,     0,     0,   115,   116,   117,   118,   119,
       0,     0,     0,     0,     0,     0,   418,   419,     0,     0,
       0,   394,   420,     0,   421,     0,     0,   395,   278,     0,
       0,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,   679,     0,     0,     0,     0,
       0,   397,   398,   399,   400,   203,   204,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   106,   411,     0,   108,     0,     0,   412,
     413,     0,   414,   415,   416,   111,   112,   113,   417,   114,
     382,     0,   115,   116,   117,   118,   119,     0,     0,   383,
       0,     0,     0,   418,   419,     0,     0,     0,     0,   480,
       0,   421,   384,     0,     0,   278,     0,     0,   385,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   387,     0,     0,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     382,     0,     0,     0,     0,     0,     0,   390,   391,   383,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
       0,     0,   384,   393,     0,     0,     0,     0,   385,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   387,     0,     0,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     687,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     394,     0,     0,     0,     0,   389,   395,   390,   391,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,   392,
       0,     0,     0,   393,     0,     0,     0,     0,     0,     0,
     397,   398,   399,   400,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   106,   411,     0,   108,     0,     0,   412,   413,
       0,   414,   415,   416,   111,   112,   113,   417,   114,     0,
     394,   115,   116,   117,   118,   119,   395,     0,     0,     0,
       0,     0,   418,   419,     0,     0,   396,     0,   480,     0,
     421,     0,     0,     0,   278,     0,     0,     0,     0,     0,
     397,   398,   399,   400,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   106,   411,     0,   108,     0,     0,   412,   413,
       0,   414,   415,   416,   111,   112,   113,   417,   114,   382,
       0,   115,   116,   117,   118,   119,     0,     0,   383,     0,
       0,     0,   418,   419,     0,     0,     0,     0,   480,     0,
     421,   384,     0,     0,   278,     0,     0,   385,   386,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   387,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   382,
       0,     0,     0,     0,     0,     0,   390,   391,   383,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   392,     0,
       0,   384,   393,     0,     0,     0,     0,   385,   386,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   387,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,   395,   390,   391,     0,     0,
       0,     0,     0,     0,     0,   396,     0,     0,   392,     0,
       0,     0,   393,     0,     0,     0,     0,     0,     0,   397,
     398,   399,   400,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,     0,     0,     0,     0,     0,
       0,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   106,   411,     0,   108,     0,     0,   412,   413,     0,
     414,   415,   416,   111,   112,   113,   417,   114,     0,   394,
     115,   116,   117,   118,   119,   395,     0,     0,     0,     0,
       0,   418,   419,     0,     0,   396,     0,   480,   943,   421,
       0,     0,     0,   278,     0,     0,  1136,     0,     0,   397,
     398,   399,   400,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,     0,     0,     0,     0,     0,
       0,     0,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   106,   411,     0,   108,     0,     0,   412,   413,     0,
     414,   415,   416,   111,   112,   113,   417,   114,   382,     0,
     115,   116,   117,   118,   119,     0,     0,   383,     0,     0,
       0,   418,   419,     0,     0,     0,     0,   480,     0,   421,
     384,     0,     0,   278,     0,     0,   385,   386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   387,     0,     0,   388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   382,     0,
       0,     0,     0,     0,     0,   390,   391,   383,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,     0,     0,
     384,   393,     0,     0,     0,     0,   385,   386,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   387,     0,     0,   388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1318,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,   395,   390,   391,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,   392,     0,     0,
       0,   393,     0,     0,     0,     0,     0,     0,   397,   398,
     399,   400,   203,   204,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     106,   411,     0,   108,     0,     0,   412,   413,     0,   414,
     415,   416,   111,   112,   113,   417,   114,     0,   394,   115,
     116,   117,   118,   119,   395,     0,     0,     0,     0,     0,
     418,   419,     0,     0,   396,     0,   480,     0,   421,     0,
       0,     0,   278,     0,     0,  1331,     0,     0,   397,   398,
     399,   400,   203,   204,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     106,   411,     0,   108,     0,     0,   412,   413,     0,   414,
     415,   416,   111,   112,   113,   417,   114,   382,     0,   115,
     116,   117,   118,   119,     0,     0,   383,     0,     0,     0,
     418,   419,     0,     0,     0,     0,   480,     0,   421,   384,
       0,     0,   278,     0,     0,   385,   386,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   387,     0,     0,   388,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   390,   391,     0,     0,     0,     0,
       0,     0,     0,     0,   382,     0,   392,     0,     0,     0,
     393,     0,     0,   383,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   384,     0,     0,     0,
       0,     0,   385,   386,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     387,     0,     0,   388,     0,     0,     0,   394,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,   390,   391,     0,     0,     0,     0,   397,   398,   399,
     400,   203,   204,   392,     0,     0,     0,   393,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   106,
     411,     0,   108,     0,     0,   412,   413,     0,   414,   415,
     416,   111,   112,   113,   417,   114,  1415,     0,   115,   116,
     117,   118,   119,     0,     0,     0,     0,     0,     0,   418,
     419,     0,     0,  1704,   394,   480,     0,   421,     0,     0,
     395,   278,     0,     0,     0,     0,     6,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,  1534,  1535,     0,
    1536,     0,     0,  1537,   397,   398,   399,   400,   203,   204,
       0,     0,     0,     0,     0,     0,     0,  1538,   401,     0,
       0,     0,     0,  1539,     0,     0,     0,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   106,   411,     0,   108,
       0,     0,   412,   413,     0,   414,   415,   416,   111,   112,
     113,   417,   114,     0,  1415,   115,   116,   117,   118,   119,
       0,     0,     0,     0,     0,     0,   418,   419,     0,     0,
       0,     0,   480,     0,   421,     0,     0,     0,   278,     0,
       0,     0,     0,  1540,     6,     0,     0,     0,     0,   587,
       0,     0,     0,   588,     0,  1534,  1535,     0,  1536,     0,
     786,  1537,    14,     0,     0,     0,     0,     0,     0,     0,
       0,   589,     0,     0,   590,  1538,     0,  1541,     0,     0,
       0,  1539,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,     0,     0,     0,     0,  1661,     0,  1542,     0,
       0,     0,     0,     0,     0,   105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   591,     0,     0,     0,     0,
       0,   592,   593,   594,     0,   595,   596,   597,     0,   598,
       0,     0,     0,     0,  1543,   107,     0,   108,     0,   587,
       0,  1540,     0,   588,   109,   110,   111,   112,   113,   599,
     114,   600,     0,   115,  1544,   117,   118,   119,     0,     0,
      14,   589,     0,     0,   590,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1541,     0,     0,     0,     0,
       0,     0,   601,     0,     0,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1542,     0,     0,   602,
       0,     0,     0,   105,     0,   591,     0,     0,     0,     0,
       0,   592,   593,   594,     0,   595,   596,   597,     0,   598,
       0,   603,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1543,   107,     0,   108,   105,     0,     0,   599,
       0,   600,   109,   110,   111,   112,   113,     0,   114,     0,
       0,   115,  1544,   117,   118,   119,   604,   605,     0,     0,
       0,     0,     0,     0,     0,   106,   107,     0,   108,     0,
       0,     0,   601,   606,     0,   109,   110,   111,   112,   113,
       0,   114,     0,     0,   115,   116,   117,   118,   119,   602,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   603,     0,   858,     0,     0,     0,   588,     0,     0,
       0,     0,     0,     0,     0,     0,   105,     0,     0,     0,
       0,   859,     0,     0,     0,   589,     0,     0,   590,     0,
       0,     0,     0,     0,     0,     0,   604,   605,     0,     0,
       0,     0,     0,     0,     0,   106,   107,     0,   108,     0,
       0,     0,     0,   606,     0,   109,   110,   111,   112,   113,
       0,   114,     0,     0,   115,   116,   117,   118,   119,   591,
       0,     0,     0,     0,     0,   592,   593,   594,     0,   595,
     596,   597,     0,   598,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   599,     0,   600,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   601,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   602,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   603,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     604,   605,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   606
};

static const short yycheck[] =
{
       9,    16,    55,    12,     4,     4,   104,    16,    64,    18,
       0,    63,    62,   507,   352,   255,   148,    68,   331,    57,
     702,   360,   138,   139,   360,   286,   420,   138,   139,    85,
     557,    69,    56,   667,   356,    94,    89,    63,  1029,   493,
      91,   519,   365,    67,    53,    54,    55,    56,    57,    58,
     468,   821,   679,   329,    63,    64,    65,   763,    67,  1249,
      69,    70,   377,    72,    73,   687,    90,    70,    83,    73,
     720,   753,    81,    97,    83,    84,    85,    92,    87,    88,
      89,    90,    88,    92,    93,    94,    95,   547,    97,   549,
      93,    95,   130,   152,   716,   324,   280,   557,   264,   639,
    1023,   701,   307,   129,   756,   131,  1029,   219,   274,   156,
     929,  1023,  1078,   151,   632,  1030,   852,  1029,   752,  1023,
     129,   130,   131,  1242,  1039,  1029,   150,  1023,  1073,  1023,
     228,   783,  1192,  1029,   143,  1029,  1367,   143,   188,   148,
     792,   150,   151,   152,   977,  1548,  1061,   156,   967,     8,
      10,  1382,  1055,   759,    23,   645,     8,   795,     6,    15,
    1395,     8,   720,   213,     8,  1395,    64,  1163,   474,  1288,
       8,    46,    66,  1169,     8,   307,     8,    64,     8,    50,
     420,  1455,  1297,  1395,  1395,   270,  1395,   160,    50,   512,
     101,    12,    20,   845,   140,   533,   534,   535,   536,   311,
      11,    49,    29,    12,   526,     8,   198,    26,    66,    24,
    1196,     6,    31,    59,   193,    26,     3,   315,   557,   200,
      31,   557,  1569,   261,   182,  1184,     6,  1186,  1247,    94,
     123,   257,   270,   201,    11,   729,   148,    48,   291,    47,
     163,   204,   157,     6,   645,   149,   200,     9,   257,   650,
     177,    19,   261,   880,   881,   264,   265,   295,   281,   581,
    1079,   270,   166,   148,    14,   274,   289,   190,   231,    49,
       8,    48,   587,   132,  1219,    66,    84,   204,   270,  1398,
     132,   307,   291,   559,  1275,   132,   295,  1634,   132,   270,
      47,   252,   271,    80,   132,   461,    58,   924,   132,     4,
     132,   194,   132,   353,   330,   355,   191,    69,  1294,   124,
     960,   337,   270,   339,   135,   341,   270,   163,   344,   369,
     373,   330,   972,   973,   182,   271,   135,   238,   337,   132,
     339,   607,   341,   371,   968,   344,   468,    47,   362,   266,
      53,    54,   207,   208,  1459,   318,   998,    52,   808,  1264,
      26,   181,  1275,   362,   363,    31,   271,  1760,   185,    29,
     363,   111,   371,  1275,   373,  1361,  1362,    26,   163,   254,
       8,  1275,    31,  1206,   272,   829,   270,   377,  1281,  1275,
     188,  1275,   872,   270,   212,  1575,     8,   262,    26,  1041,
     475,  1794,  1795,    31,   132,  1022,  1034,  1035,  1633,   270,
     163,  1420,   960,  1633,    26,   274,   667,  1681,   270,    31,
     270,   173,   421,   272,   972,   271,  1819,   723,   456,   267,
     272,  1633,  1633,  1193,  1633,   272,  1385,  1830,   272,   467,
    1833,   188,   479,   471,   272,  1665,   460,   475,   272,    23,
     204,   553,   468,   181,   684,  1083,  1506,   456,   267,   268,
     474,   460,   461,   693,   265,   266,   267,   268,   467,   164,
     130,  1692,   471,   574,    84,   474,   475,   868,  1587,   730,
     479,   872,  1099,  1100,  1593,  1102,  1103,  1083,   188,  1709,
     489,   531,    46,  1673,   197,    26,  1452,    89,  1433,     4,
      31,   752,    53,    54,    18,   771,  1411,     6,   774,    26,
    1270,  1416,   266,  1418,    31,  1501,  1451,    91,   177,     3,
      34,   623,   114,  1113,   790,   185,     6,    65,  1224,  1749,
     843,  1629,     0,  1631,   575,  1694,  1695,   577,   269,   133,
    1407,   557,   202,  1763,    98,   204,  1726,    52,  1453,   815,
      49,   770,   726,   856,   749,   711,   555,   965,   557,  1089,
     865,  1287,   561,  1203,  1072,    79,    19,    47,  1788,  1392,
     123,  1791,    26,  1466,    71,  1683,  1735,    31,   188,    59,
     172,  1709,   148,   204,  1764,    47,    25,  1454,   628,   629,
    1688,   102,   186,  1813,   108,   204,    80,   587,    26,   265,
     266,   267,   268,    31,    84,   204,   272,   266,   146,  1738,
    1739,   164,   271,  1230,   180,   631,   265,   266,   267,   268,
    1525,  1749,    84,   272,    10,  1733,  1531,   749,    16,   195,
     207,   208,   631,    86,   735,  1252,    75,   265,   266,   267,
     268,   640,    26,    98,  1286,   266,   197,    31,  1807,   187,
     640,   640,    26,   265,   266,   267,   268,    31,   211,   164,
    1788,   175,  1471,  1791,   175,   766,   767,  1775,   277,   124,
     109,   670,   271,   153,   670,  1580,    26,   182,   277,   176,
    1778,    31,  1010,   163,    26,  1813,   183,   703,   127,    31,
     191,   153,  1597,    26,   204,    83,   695,  1826,    31,    62,
      63,   163,   148,   702,   703,   239,  1318,   706,   188,   723,
    1406,   216,   711,   155,   204,   249,   744,   968,   746,   151,
    1701,   720,   762,   763,   723,   204,   188,    33,   769,   728,
    1685,     7,   731,   749,   265,   266,   267,   268,   184,   755,
      46,   272,   177,   759,   204,   744,  1074,   746,   265,   266,
     267,   268,   231,   254,   753,   272,   755,   204,    26,  1383,
     759,   271,  1028,    31,   780,    68,    26,   277,   809,   204,
     202,    31,  1023,    47,  1040,  1680,  1731,    24,  1029,  1396,
     270,   780,  1250,  1086,  1689,   270,    33,   266,  1701,  1173,
     275,   807,   808,   949,  1060,     5,   952,   838,   839,  1701,
     841,  1779,  1780,   151,     6,     6,    16,  1701,   807,   808,
      84,   265,   266,   267,   268,  1701,   844,  1701,   272,  1774,
     206,   147,   170,    88,   814,  1730,   854,    26,  1212,  1273,
    1347,   266,    31,     0,   271,  1279,   826,   265,   266,   267,
     268,     8,   277,   965,   272,   844,  1149,    49,    49,   235,
     236,  1806,   238,     4,    68,   854,     7,  1835,   123,   245,
     246,   247,   248,   249,    67,   251,   237,   866,   254,   255,
     256,   257,   258,   129,   245,   865,   867,   124,    81,   270,
     871,   265,   266,   267,   268,    40,   151,   129,   272,   154,
     270,   265,   266,   267,   268,   275,    51,    64,   272,   146,
      67,    52,   105,    58,   953,   170,   153,    74,    26,    83,
      77,   239,   240,    31,    81,   265,   266,   267,   268,    86,
     270,     6,    89,   265,   266,   267,   268,   270,    83,   272,
     272,   196,   265,   266,   267,   268,   950,    26,   105,   272,
     187,   131,    31,  1173,   129,  1196,   120,   129,   124,   965,
     949,   950,   951,   952,   953,   954,   159,  1058,   951,   133,
     954,   960,   129,  1635,    49,   132,   133,    94,   240,   270,
     146,   272,    83,   972,   973,   142,   103,   153,   977,  1291,
     164,    26,  1212,   666,   182,   668,    31,    20,    67,   672,
     673,   674,   159,   148,   137,   138,  1262,   265,   266,   267,
     268,   270,    81,   272,   272,   265,   266,   267,   268,    64,
      89,   187,   272,     7,   181,  1632,   271,   270,  1347,   186,
    1504,  1347,   275,  1507,  1275,   180,   105,  1293,  1069,   184,
       0,   198,   170,   200,    28,   269,   270,  1654,     8,   207,
    1657,    26,    36,  1294,  1085,   211,    31,   213,    62,    43,
    1091,    45,  1051,    47,   270,   270,   272,   272,    52,   270,
    1076,   272,  1052,  1052,    26,    59,  1065,  1083,    67,    31,
     270,    10,   272,    26,    67,  1403,    64,  1076,    31,    73,
     159,   102,    81,  1533,  1083,  1191,  1192,  1193,    81,   206,
      84,  1213,    83,    26,    64,    57,    82,    67,    31,   270,
     198,   272,   269,   270,    74,   272,   105,    77,    26,   269,
     270,    81,   105,    31,   148,  1599,    86,    86,   139,    89,
     141,  1162,   270,    66,   272,  1226,   270,  1228,   272,    18,
     129,   270,  1383,   272,   271,   105,   129,    26,   270,    57,
     272,    64,    31,   164,   270,    34,   272,   265,   266,   267,
     268,   148,   129,    86,   272,   129,  1422,   151,   252,   129,
     159,   196,   132,   133,   158,     0,   159,   161,   189,   163,
     164,   182,   142,     8,   180,   107,   265,   266,   267,   268,
     270,    70,   272,   272,  1224,  1184,   130,  1186,  1289,   159,
      79,   163,   188,   200,   188,  1461,   103,   270,    87,   272,
      26,  1191,  1192,    33,  1203,    31,   157,  1206,  1825,   270,
     270,   181,   272,    26,  1213,   215,   186,   106,    31,   108,
     265,   266,   267,   268,  1184,   262,  1186,   272,   198,    64,
     200,   118,    67,   122,    32,   124,   125,   126,    10,    74,
     271,   271,    77,   237,   271,    26,    81,   271,   207,    18,
      31,    86,   271,  1737,    89,   271,   271,    26,   252,   253,
     207,    26,    31,   271,  1304,    34,    31,   887,  1308,   889,
     105,   891,   892,  1395,   894,   895,   896,   207,  1268,  1268,
     265,   266,   267,   268,   271,    66,   175,   272,   203,   204,
     205,   271,   271,  1415,   129,   271,   271,   132,   133,   269,
     270,    70,   272,   265,   266,   267,   268,   142,   271,   255,
      79,   271,   265,   266,   267,   268,   231,   232,    87,   272,
      26,    86,   270,   182,   159,    31,   276,   276,  1594,   274,
      66,   277,   265,   266,   267,   268,   269,   106,   148,   108,
     286,   270,    66,   270,   270,   174,   181,   265,   266,   267,
     268,   186,   262,   122,   276,   124,   125,   126,   271,    10,
      66,   270,   128,   198,    94,   200,  1406,   251,     6,   270,
      24,   317,  1414,   262,   263,   264,   265,   266,   267,   268,
      25,    24,    46,   272,    10,   182,  1385,   143,   216,   182,
     143,    23,   204,  1392,   193,  1423,   145,   193,  1414,  1415,
    1506,    46,   271,    95,   271,    64,   175,  1665,   274,    47,
     271,   271,    10,   129,   157,  1414,   270,   135,   128,   271,
     271,    59,   197,   272,  1423,   182,  1548,  1528,   210,    63,
      75,   271,   194,    66,   269,   270,    66,   272,   198,   265,
     266,   267,   268,   269,    10,   248,    84,   271,    40,    94,
    1701,  1709,   265,   266,   267,   268,   204,   271,   103,    51,
      23,    20,    46,    30,   109,    90,    58,    90,   414,    24,
     132,   132,   418,   419,   420,   132,  1706,   164,   162,    71,
     271,    46,   127,   164,   265,   266,   267,   268,  1534,   102,
    1578,  1749,   163,   262,   263,   264,   265,   266,   267,   268,
     265,   266,   267,   268,   271,  1763,  1496,    20,   163,    10,
     271,  1633,   270,   272,    66,   153,  1506,    51,   270,    26,
     270,   131,   113,   270,    58,   163,    66,   271,    62,    63,
    1788,   270,  1548,  1791,   480,  1534,  1579,    71,   484,    74,
     132,   270,    12,  1665,   271,    45,  1574,   247,   271,    83,
     188,    10,   168,   272,    10,  1813,   148,  1537,   273,   265,
     266,   267,   268,  1603,   272,   272,   186,  1607,   206,   272,
    1569,   200,   140,   165,    55,  1574,    55,   272,   238,   272,
    1579,     8,   271,   271,    98,   271,  1674,  1709,   180,   271,
      54,   271,   184,  1598,   271,   187,   271,   235,   236,  1598,
     238,   271,   271,   195,   271,   271,   271,   245,   246,   247,
     248,   249,   181,   251,   148,   271,   254,   255,   256,   257,
     258,   271,   214,   271,   271,    73,   270,  1749,   134,   134,
     272,   165,   134,  1647,    89,  1634,  1635,    20,  1760,    77,
     168,  1763,   136,   192,   137,     6,   180,    17,  1647,   153,
     184,   272,   132,   272,   132,   132,   132,    42,   132,   132,
      24,   195,   271,    54,   272,   272,  1788,   163,  1714,  1791,
     272,   272,  1794,  1795,   131,   142,    26,    83,  1683,   182,
     214,    84,   181,    84,  1683,    47,    47,    47,    98,   272,
     188,  1813,   272,   188,    66,  1700,   131,  1819,    59,   117,
     117,  1700,   272,    26,   650,   276,   270,    86,  1830,   269,
     272,  1833,    73,   269,   272,  1714,  1715,    54,   272,   272,
     271,   667,   272,    84,   272,   671,  1716,  1716,  1733,   272,
     272,   272,   678,   272,  1733,   272,   272,   683,   684,  1744,
     272,   272,  1785,   272,  1760,  1744,   271,   693,   694,   269,
     696,   697,   698,   699,   700,   701,   218,   219,   220,   221,
     222,   223,   224,   225,    86,    98,    90,   114,    86,   197,
    1775,   129,   269,   129,   129,    89,  1775,  1782,  1794,  1795,
     269,   269,   269,  1782,   730,   269,  1785,   272,   270,   272,
     156,   181,   153,    50,    47,   188,   271,   269,   269,   133,
      39,   270,   163,  1819,   272,   271,   752,   272,   269,   272,
      50,   757,  1811,     6,  1830,   209,     9,  1833,   270,  1818,
      13,   206,    53,   272,   270,   262,    12,   188,  1189,    22,
     274,   265,   711,   635,   857,  1089,  1086,  1075,  1837,  1442,
     357,   327,    35,   581,  1013,   206,  1076,   755,    41,    42,
     235,   236,  1083,   238,   791,   313,  1441,  1644,  1420,  1422,
     245,   246,   247,   248,   249,  1728,   251,  1724,   322,   254,
     255,   256,   257,   258,   235,   236,    69,   238,   780,    72,
    1727,  1766,   267,  1243,   245,   246,   247,   248,   249,   339,
     251,  1652,   339,   254,   255,   256,   257,   258,  1706,  1663,
    1000,  1761,  1221,  1683,  1399,   563,   332,   100,   101,   325,
    1277,   344,   836,    13,    30,   620,   828,   299,   495,   112,
     499,   744,   141,   116,   757,   730,   977,  1633,  1523,  1782,
    1744,  1837,     0,     0,   507,     0,   882,   883,   884,   719,
     969,   887,   888,   889,  1127,   891,   892,   893,   894,   895,
     896,   897,   898,  1232,   900,  1065,   902,   561,  1171,   905,
     906,   907,   908,   909,   910,    87,    -1,  1818,   731,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   922,    -1,   171,   925,
     173,  1316,  1317,    -1,    -1,    -1,   179,  1322,  1323,    74,
    1325,  1326,    -1,    -1,    -1,    -1,   189,    -1,  1333,  1334,
    1335,  1336,    -1,  1338,  1339,  1340,  1341,  1342,  1343,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,   968,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    -1,    -1,
      -1,    -1,   265,   266,    -1,    -1,    -1,    -1,   271,    -1,
     273,    -1,     6,    -1,   277,     9,    -1,    -1,    -1,    13,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,   235,
     236,   206,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    69,    -1,    -1,    72,    -1,
     235,   236,    -1,   238,    -1,   271,    -1,    -1,    -1,    -1,
     245,   246,   247,   248,   249,    -1,   251,  1113,    -1,   254,
     255,   256,   257,   258,    -1,    -1,   100,   101,    -1,  1125,
    1126,    -1,  1128,  1129,    -1,    -1,    -1,    -1,   112,    -1,
    1136,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1153,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1161,    -1,  1163,    -1,    -1,
     180,    -1,    -1,  1169,    -1,  1171,  1172,  1173,  1174,  1175,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   171,    -1,   173,
    1196,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,  1212,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,   203,
     204,   205,   206,   207,   208,   245,   246,   247,   248,   249,
      -1,   251,    -1,   217,   254,   255,   256,   257,   258,  1245,
      41,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    69,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,    -1,    -1,
      -1,   265,   266,    -1,    -1,    -1,    -1,   271,  1294,   273,
      -1,    -1,    -1,   277,    33,    -1,    -1,    98,    -1,    -1,
      -1,    -1,    -1,     6,    -1,    -1,     9,    -1,    -1,  1315,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1324,    22,
      -1,    -1,    -1,    -1,    -1,  1331,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1361,  1362,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,   173,   235,   236,    -1,   238,  1383,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,   189,   251,
      -1,    -1,   254,   255,   256,   257,   258,   100,   101,    -1,
      -1,    -1,   203,   204,   205,    -1,   207,   208,    -1,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    -1,    -1,    -1,    -1,    -1,    -1,
     241,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   250,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   266,    -1,   206,   171,    -1,
     173,    -1,    -1,  1479,  1480,    -1,   179,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1501,   235,   236,    -1,   238,
     203,   204,   205,   206,   207,   208,   245,   246,   247,   248,
     249,    -1,   251,    -1,   217,   254,   255,   256,   257,   258,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,  1540,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    -1,
      -1,   254,   255,   256,   257,   258,    -1,     6,    -1,    -1,
       9,    -1,   265,   266,    13,    -1,    -1,    -1,   271,    -1,
     273,    -1,    -1,    22,   277,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   204,    35,   206,    -1,    -1,
      -1,    -1,    41,    42,    -1,    -1,  1602,    -1,   217,    -1,
      -1,    -1,    -1,    -1,  1610,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,
      69,    -1,    -1,    72,    -1,   244,   245,   246,   247,   248,
     249,    -1,   251,    -1,    -1,   254,   255,   256,   257,   258,
    1646,    -1,    -1,    -1,  1650,    -1,    -1,   189,    -1,    -1,
      -1,   100,   101,    -1,     6,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,   112,   206,    -1,    -1,   116,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,    -1,
    1706,    -1,  1708,   245,   246,   247,   248,   249,    -1,   251,
      -1,    -1,   254,   255,   256,   257,   258,    69,    -1,    -1,
      72,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    -1,    -1,    -1,   265,   266,   206,   171,
      -1,   173,   271,    -1,   273,    -1,    -1,   179,   277,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,
     238,   203,   204,   205,   206,   207,   208,   245,   246,   247,
     248,   249,    -1,   251,    -1,   217,   254,   255,   256,   257,
     258,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   273,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      -1,    -1,   254,   255,   256,   257,   258,    -1,     6,    -1,
     206,     9,    -1,   265,   266,    13,    -1,    -1,    -1,   271,
      -1,   273,    -1,    -1,    22,   277,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,   235,
     236,    -1,   238,    41,    42,    -1,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,   271,    -1,   273,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,   101,    -1,     6,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,   112,   206,   235,   236,   116,   238,
      -1,    22,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,    -1,    35,   254,   255,   256,   257,   258,
      41,    42,    -1,    -1,   235,   236,    -1,   238,    -1,    -1,
      -1,    -1,   271,    -1,   245,   246,   247,   248,   249,    -1,
     251,    -1,    -1,   254,   255,   256,   257,   258,    69,    -1,
      -1,    72,    -1,   171,    -1,   173,    -1,    -1,    -1,    -1,
     271,   179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    -1,    -1,    -1,    -1,   265,   266,   206,
     171,    -1,   173,   271,    -1,   273,    -1,    -1,   179,   277,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,   236,
      -1,   238,   203,   204,   205,   206,   207,   208,   245,   246,
     247,   248,   249,    -1,   251,   114,   217,   254,   255,   256,
     257,   258,    -1,    -1,    -1,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,    -1,   238,    -1,    -1,
     241,   242,    -1,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,    -1,   254,   255,   256,   257,   258,    -1,     6,
     206,    -1,     9,    -1,   265,   266,    13,    -1,    -1,    -1,
     271,    -1,   273,   172,    -1,    22,   277,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,   235,
     236,    -1,   238,    -1,    41,    42,    -1,    -1,    -1,   245,
     246,   247,   248,   249,    -1,   251,    -1,   206,   254,   255,
     256,   257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    69,   269,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,
      -1,    -1,    -1,    -1,    -1,    -1,   245,   246,   247,   248,
     249,    -1,   251,   100,   101,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    13,   206,   112,    -1,    -1,    -1,   116,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    35,   204,    -1,   206,    -1,
      -1,    41,    42,   235,   236,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,   245,   246,   247,   248,   249,    -1,   251,
      60,    -1,   254,   255,   256,   257,   258,   235,   236,    69,
     238,    -1,    72,    -1,   171,   267,   173,   245,   246,   247,
     248,   249,   179,   251,    -1,    -1,   254,   255,   256,   257,
     258,    -1,   189,    -1,    -1,    -1,    -1,    -1,   266,    -1,
     100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     217,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    -1,    -1,   254,   255,   256,
     257,   258,    -1,   163,    -1,    -1,    -1,   167,   265,   266,
      -1,    -1,    -1,   173,   271,    -1,   273,    -1,    -1,   179,
     277,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   203,   204,   205,   206,   207,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,    19,    -1,
      -1,    -1,    23,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      41,   241,   242,    44,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    13,   254,   255,   256,   257,   258,   259,
     260,   261,    22,    -1,    -1,   265,   266,    -1,    -1,    -1,
      -1,   271,    -1,   273,    -1,    35,    -1,   277,    38,    -1,
      -1,    41,    42,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      91,    92,    93,    -1,    95,    96,    97,    -1,    99,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,   119,    -1,
     121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,   152,   112,    -1,    -1,    -1,   116,    -1,    -1,    22,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     191,    -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,   167,    -1,    72,
      -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    -1,
      -1,    -1,   243,   203,   204,   205,   206,   207,   208,   112,
      -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,    -1,   238,    -1,
      -1,   241,   242,    -1,   244,   245,   246,   247,   248,   249,
     250,   251,    -1,    -1,   254,   255,   256,   257,   258,    -1,
      -1,    -1,    -1,    -1,   167,   265,   266,    -1,    -1,    -1,
     173,   271,    -1,   273,    -1,    -1,   179,   277,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    74,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    13,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    22,    -1,
      -1,    -1,   265,   266,    -1,    -1,    -1,    -1,   271,    -1,
     273,    35,    -1,    -1,   277,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,   100,   101,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,   112,   206,
      -1,    -1,   116,    -1,    -1,    22,    -1,   214,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      -1,    -1,    -1,    -1,    41,    42,    -1,    -1,   235,   236,
      -1,   238,    -1,    -1,    -1,    -1,    -1,    -1,   245,   246,
     247,   248,   249,    60,   251,    -1,    -1,   254,   255,   256,
     257,   258,    69,   167,    -1,    72,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,   112,    -1,    -1,    -1,   116,
      -1,    -1,    -1,   217,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    -1,    -1,    -1,
     167,   265,   266,    -1,    -1,    -1,   173,   271,    -1,   273,
      -1,    -1,   179,   277,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,
     207,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
      -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,   246,
     247,   248,   249,   250,   251,    13,    -1,   254,   255,   256,
     257,   258,    -1,    -1,    22,    -1,    -1,    -1,   265,   266,
      -1,    -1,    -1,    -1,   271,    -1,   273,    35,    -1,    -1,
     277,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      98,    -1,   100,   101,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    35,   116,    -1,
     206,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,    -1,    -1,   233,   234,   235,
     236,    69,   238,    -1,    72,   241,   242,    -1,    -1,   245,
     246,   247,   248,   249,   250,   251,   252,    -1,   254,   255,
     256,   257,   258,    -1,    -1,   173,    -1,    -1,    -1,    -1,
      98,   179,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,   112,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,   163,   244,   245,   246,   247,
     248,   249,   250,   251,    -1,   173,   254,   255,   256,   257,
     258,   179,    -1,    -1,    -1,    -1,    -1,   265,   266,   267,
      -1,   189,    -1,   271,    -1,   273,    -1,    -1,    -1,   277,
      -1,    -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   217,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,    -1,
     238,    -1,    -1,   241,   242,    -1,   244,   245,   246,   247,
     248,   249,   250,   251,    13,    -1,   254,   255,   256,   257,
     258,    -1,    -1,    22,    -1,    -1,    -1,   265,   266,    -1,
      -1,    -1,    -1,   271,    -1,   273,    35,    -1,    -1,   277,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,   112,    -1,    -1,    -1,   116,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      72,    -1,    -1,    -1,   173,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,
      -1,    -1,    -1,    -1,   203,   204,   205,   206,   207,   208,
     112,    -1,    -1,    -1,   116,    -1,    -1,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    -1,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    -1,    -1,    -1,   265,   266,    -1,    -1,
      -1,   173,   271,    -1,   273,    -1,    -1,   179,   277,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,   203,   204,   205,   206,   207,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,    -1,   238,    -1,    -1,   241,
     242,    -1,   244,   245,   246,   247,   248,   249,   250,   251,
      13,    -1,   254,   255,   256,   257,   258,    -1,    -1,    22,
      -1,    -1,    -1,   265,   266,    -1,    -1,    -1,    -1,   271,
      -1,   273,    35,    -1,    -1,   277,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    -1,    35,   116,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
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
     173,   254,   255,   256,   257,   258,   179,    -1,    -1,    -1,
      -1,    -1,   265,   266,    -1,    -1,   189,    -1,   271,    -1,
     273,    -1,    -1,    -1,   277,    -1,    -1,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,    -1,   238,    -1,    -1,   241,   242,
      -1,   244,   245,   246,   247,   248,   249,   250,   251,    13,
      -1,   254,   255,   256,   257,   258,    -1,    -1,    22,    -1,
      -1,    -1,   265,   266,    -1,    -1,    -1,    -1,   271,    -1,
     273,    35,    -1,    -1,   277,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    35,   116,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,
      -1,    -1,    -1,    -1,    -1,   179,   100,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   112,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   173,
     254,   255,   256,   257,   258,   179,    -1,    -1,    -1,    -1,
      -1,   265,   266,    -1,    -1,   189,    -1,   271,   272,   273,
      -1,    -1,    -1,   277,    -1,    -1,   200,    -1,    -1,   203,
     204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,
     244,   245,   246,   247,   248,   249,   250,   251,    13,    -1,
     254,   255,   256,   257,   258,    -1,    -1,    22,    -1,    -1,
      -1,   265,   266,    -1,    -1,    -1,    -1,   271,    -1,   273,
      35,    -1,    -1,   277,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      35,   116,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,    -1,    -1,   179,   100,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,   112,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    -1,   173,   254,
     255,   256,   257,   258,   179,    -1,    -1,    -1,    -1,    -1,
     265,   266,    -1,    -1,   189,    -1,   271,    -1,   273,    -1,
      -1,    -1,   277,    -1,    -1,   200,    -1,    -1,   203,   204,
     205,   206,   207,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,
     245,   246,   247,   248,   249,   250,   251,    13,    -1,   254,
     255,   256,   257,   258,    -1,    -1,    22,    -1,    -1,    -1,
     265,   266,    -1,    -1,    -1,    -1,   271,    -1,   273,    35,
      -1,    -1,   277,    -1,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    13,    -1,   112,    -1,    -1,    -1,
     116,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,   173,    -1,    -1,
      -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,   101,    -1,    -1,    -1,    -1,   203,   204,   205,
     206,   207,   208,   112,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,    -1,   238,    -1,    -1,   241,   242,    -1,   244,   245,
     246,   247,   248,   249,   250,   251,    17,    -1,   254,   255,
     256,   257,   258,    -1,    -1,    -1,    -1,    -1,    -1,   265,
     266,    -1,    -1,   269,   173,   271,    -1,   273,    -1,    -1,
     179,   277,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,
      61,    -1,    -1,    64,   203,   204,   205,   206,   207,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,   217,    -1,
      -1,    -1,    -1,    84,    -1,    -1,    -1,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,    -1,   238,
      -1,    -1,   241,   242,    -1,   244,   245,   246,   247,   248,
     249,   250,   251,    -1,    17,   254,   255,   256,   257,   258,
      -1,    -1,    -1,    -1,    -1,    -1,   265,   266,    -1,    -1,
      -1,    -1,   271,    -1,   273,    -1,    -1,    -1,   277,    -1,
      -1,    -1,    -1,   144,    47,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    23,    -1,    58,    59,    -1,    61,    -1,
      30,    64,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    44,    78,    -1,   178,    -1,    -1,
      -1,    84,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,   199,    -1,
      -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,    19,
      -1,   144,    -1,    23,   245,   246,   247,   248,   249,   119,
     251,   121,    -1,   254,   255,   256,   257,   258,    -1,    -1,
     163,    41,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,    -1,   169,
      -1,    -1,    -1,   206,    -1,    85,    -1,    -1,    -1,    -1,
      -1,    91,    92,    93,    -1,    95,    96,    97,    -1,    99,
      -1,   191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   235,   236,    -1,   238,   206,    -1,    -1,   119,
      -1,   121,   245,   246,   247,   248,   249,    -1,   251,    -1,
      -1,   254,   255,   256,   257,   258,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,
      -1,    -1,   152,   243,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   191,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   227,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,   236,    -1,   238,    -1,
      -1,    -1,    -1,   243,    -1,   245,   246,   247,   248,   249,
      -1,   251,    -1,    -1,   254,   255,   256,   257,   258,    85,
      -1,    -1,    -1,    -1,    -1,    91,    92,    93,    -1,    95,
      96,    97,    -1,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   243
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     7,    28,    36,    43,    45,    47,    52,    59,    73,
      84,   151,   158,   161,   163,   164,   188,   237,   252,   253,
     279,   280,   281,   290,   300,   310,   312,   314,   433,   456,
     478,   503,   504,   505,   506,   507,   509,   511,   512,   516,
     533,   534,   535,   545,   583,   585,   586,   587,   589,   590,
     591,   595,    40,    51,    58,    83,   148,   180,   184,   457,
     201,   513,    40,    51,    58,    71,   132,   148,   165,   180,
     184,   187,   195,   214,   311,   316,   204,   266,   642,   643,
      62,    63,   301,    66,    51,    58,    62,    63,    71,    83,
     148,   165,   180,   184,   195,   214,   479,   148,     6,    47,
      59,    84,   153,   163,   188,   206,   235,   236,   238,   245,
     246,   247,   248,   249,   251,   254,   255,   256,   257,   258,
     283,   284,   285,   286,   296,   297,   684,   692,   693,    19,
      86,    19,    73,   283,   285,   291,   296,   513,   239,   240,
     551,   552,   553,    71,   176,   183,   566,   567,   686,   692,
     148,   180,   195,   313,   691,   692,   252,     0,   269,   133,
     186,   536,   471,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   233,
     234,   241,   242,   250,   252,   464,   465,   692,   677,   692,
     470,   681,   692,   395,   683,   692,   567,   686,   475,   687,
     692,   155,   514,   207,   208,   335,   337,   641,   324,   370,
     607,   673,   692,   677,   333,   680,   692,     7,   392,   683,
     318,   643,   645,   360,   567,   448,   687,   434,   690,   692,
     334,   684,    11,    48,   539,   204,   644,    68,   309,   678,
     692,   566,   676,   692,   677,    68,   678,   680,   681,   683,
     645,   686,   687,   690,   684,   683,   147,   271,   600,   601,
     600,   129,   270,   129,   182,   270,   607,   567,   607,   131,
     129,   129,   283,   285,    66,   182,   204,   271,   277,   637,
     648,   271,   637,   648,     6,    49,   554,   667,   548,   553,
     680,    83,    88,   123,   151,   154,   170,   196,   517,   518,
     519,   520,   521,   522,   523,   524,   526,   164,   685,   692,
     393,   683,   361,   567,   435,   690,   691,    20,     6,   545,
      64,   542,     4,    52,   164,   472,   473,     4,    52,   164,
     182,   216,   460,   461,   641,     3,    80,   271,   396,     4,
       7,    52,   462,   463,   466,     3,    80,   450,   170,   515,
     207,   102,   139,   141,   164,   189,   338,   339,   340,    10,
     325,   641,   148,   184,   315,   396,    14,   111,   319,    62,
     362,    64,   600,    83,   206,   302,   669,    82,   198,   574,
     588,   669,    13,    22,    35,    41,    42,    69,    72,    98,
     100,   101,   112,   116,   173,   179,   189,   203,   204,   205,
     206,   217,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   236,   241,   242,   244,   245,   246,   250,   265,   266,
     271,   273,   422,   423,   564,   565,   606,   607,   630,   631,
     632,   635,   636,   637,   638,   639,   640,   641,   649,   650,
     651,   652,   653,   654,   655,   656,   657,   658,   659,   661,
     668,   669,   685,   692,   602,   607,   180,   282,   567,   286,
     148,   189,   298,   299,   688,   692,   297,    86,   271,   603,
     604,    66,    64,   282,   148,   129,   129,   298,   252,   510,
     271,   630,   547,   547,   548,   267,   555,   556,   557,   594,
     630,   668,   548,   182,   681,   107,   196,    29,   130,   185,
     202,   529,   530,   532,   567,   180,   519,   592,   593,   607,
     608,   685,   396,   362,   600,   130,   508,   537,   538,   630,
     545,   188,   200,   544,    94,   103,   323,   345,   346,   347,
     348,   354,   103,    25,    75,   109,   127,   344,   474,   473,
      33,   332,   445,    33,    46,   445,   607,   373,   461,   371,
     398,   399,   607,   157,   397,    33,   365,   370,   380,   381,
     215,    33,   607,   270,   464,   468,   607,   692,     5,    16,
     451,   452,   476,   262,   336,   336,   641,   118,   641,    46,
     341,   342,   343,   344,   345,   348,   340,    19,    23,    41,
      44,    85,    91,    92,    93,    95,    96,    97,    99,   119,
     121,   152,   169,   191,   226,   227,   243,   480,   481,   482,
     485,   486,   487,   488,   492,   493,   494,   495,   496,   497,
     500,   501,   394,   683,   449,   687,   397,    32,   320,    94,
     641,   271,   567,    10,   681,   271,   304,   305,   306,   373,
     177,   490,   642,    38,    60,   124,   167,   256,   257,   258,
     271,   609,   611,   612,   613,   614,   616,   617,   618,   619,
     620,   621,   622,   623,   624,   630,   271,   271,   271,   207,
     271,   271,   271,   271,   271,   271,   207,   207,   271,   197,
     630,   662,   664,   271,   271,   630,   630,   163,   564,   629,
     630,   689,   692,   270,   274,    26,    31,   265,   266,   267,
     268,   271,   276,   270,   272,   567,   182,   289,   683,   688,
     200,   270,   288,   567,   605,   608,   163,   190,   546,   567,
      66,   289,   282,   148,   691,   630,   272,   630,    66,   558,
     270,    10,   682,   692,   231,   266,   647,   648,   151,   523,
     123,   194,   525,   525,   270,    64,   270,   531,   174,   270,
     574,   588,   262,   276,   397,   271,    10,   270,   539,   128,
     543,   251,    15,   271,   348,   641,   336,   336,   336,   336,
     270,    24,   459,    24,    46,   458,   481,   486,   493,   481,
     270,   272,   396,    10,   674,   692,    30,   366,   367,   368,
     480,   481,   607,    65,   146,   187,   382,   383,   384,   385,
     387,   445,   674,   211,   213,   469,   463,   182,   216,   143,
      47,    84,   188,   453,   143,   454,   648,   645,   641,    23,
     343,   271,   490,   193,   193,   145,   271,   502,    95,   271,
     499,    23,    91,   445,   274,    23,   274,   491,   271,   271,
     193,   271,   499,   396,    64,    10,   641,   641,   363,   364,
     365,   380,   450,   443,   129,   305,   157,   270,    19,    37,
     303,   485,   642,   670,   692,   135,   128,   271,   124,   611,
     624,   271,   124,   259,   260,   261,   610,   611,   625,   630,
       8,   132,    18,    34,    70,    79,    87,   106,   108,   122,
     124,   125,   126,   175,   262,   263,   264,    49,   667,   594,
      49,   267,   667,   680,   630,    49,   667,    49,   667,    49,
     667,   218,   219,   220,   221,   222,   223,   224,   225,   666,
     630,   609,    53,    54,   197,   197,   660,   630,   565,   551,
     272,   272,   272,   565,   630,   633,   672,   692,   630,   630,
     630,   630,   630,   272,   633,    42,   267,   673,   607,    74,
     148,   184,   189,   195,   214,   292,   293,   294,   295,   688,
     182,   210,   299,    63,   596,   270,   272,   551,   271,   596,
     292,   294,    66,    66,   289,   547,   272,   271,   559,   560,
     561,   562,   683,   198,   574,   557,   682,   231,   648,   194,
     530,   149,   166,   527,   567,   593,   594,   673,    10,   363,
     443,   538,   248,   541,   602,   350,   351,   641,   349,   350,
     102,   175,   355,   356,   357,   648,   648,   204,   646,   645,
     344,   445,   271,   445,   446,   399,   443,    20,   271,    46,
     374,   445,   366,   443,    90,    90,   601,    24,   607,   467,
     481,   607,   645,   132,   132,   132,   643,   445,   137,   138,
     359,   164,   270,   646,   162,   489,   643,   502,   266,   647,
      46,   326,   483,   484,   647,   164,   483,   645,   645,   271,
     645,   397,   567,   443,   102,   321,   270,   272,   451,   163,
     436,   437,   438,   567,   272,   140,   271,   307,   308,   373,
     306,   271,    20,   490,   675,   692,   546,   546,   624,     8,
     132,   272,     8,   132,   609,   609,   630,   630,     6,     9,
     171,   615,   630,   271,   626,   627,    98,   124,     6,   615,
     630,   630,     6,   615,   630,    18,    34,    79,   108,   175,
       6,   615,   630,     6,   615,   630,   200,   630,     6,   615,
     630,     6,   615,   630,     6,   615,   630,   630,   630,    10,
     630,   272,   630,   270,   272,   630,   630,   630,   630,   630,
     630,    66,    66,   181,   565,   665,   609,   630,   663,    53,
      54,   197,   270,   270,   554,   270,   275,   272,   688,   683,
     687,   688,   690,   684,   270,   200,   270,   287,   292,   294,
     131,    66,   182,   113,   598,   608,   554,   584,   594,   598,
     292,   294,   294,    66,   560,   561,   270,    67,    81,   105,
     159,   568,   271,   563,    74,   569,   151,   202,   528,   443,
     272,   436,   549,   352,   270,   272,   336,    12,   358,   357,
     271,   609,   266,   375,   631,   634,   636,   638,   639,   640,
     668,    45,   400,   401,   402,   445,   375,   446,   601,   601,
     247,   386,   271,   446,   445,   443,    84,   188,    47,   188,
      47,    84,    10,   455,   477,   671,   692,   642,   270,   272,
     168,   491,   272,   270,   272,   445,   446,   270,   275,   273,
     671,   275,   272,   272,   645,   272,    10,   450,   400,   336,
     322,   323,   364,   454,   554,   200,   447,   186,   317,   367,
     601,   602,   645,   308,    55,   303,   645,   238,    55,   272,
     272,   609,   609,   609,   609,     8,   271,   271,   163,   628,
     633,    98,   271,   271,    57,   271,   271,   630,   630,   626,
     630,   200,   630,   271,   271,   271,   271,   630,   271,   271,
     271,   271,   271,   271,   272,   272,   372,   373,   272,   272,
     630,   272,   272,   272,   272,   272,   272,   630,   645,   665,
      54,   181,   181,   665,   663,   630,   564,   630,   630,   293,
     295,    73,   293,   295,   287,   490,   597,   637,   597,   599,
     637,   646,   555,   270,   272,   270,   294,   272,   560,   134,
     134,   134,    89,   564,   685,    20,    77,   573,   400,   447,
     239,   249,   540,   168,   353,   350,   136,   648,   137,   648,
     609,   272,   635,   192,   403,    17,   406,   402,   630,   376,
     377,   378,   381,   153,   386,   539,   609,   132,   132,   132,
     132,   132,   132,   445,   446,   272,   642,   646,   647,   375,
     124,   327,   328,   329,   330,   331,   332,   484,   647,   491,
     272,   443,   451,   406,   648,   445,   555,    24,   444,     6,
     438,   271,   272,   641,    20,   212,   272,   641,   630,   628,
     628,   551,   272,   272,   628,   628,   630,   628,   628,     8,
      57,   630,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   272,   480,   272,   272,    64,   272,   665,   665,
      54,   181,   272,   272,   558,   131,   182,   558,   594,   560,
     272,   498,   570,   571,   572,   606,   645,   652,   653,   654,
     656,   685,   609,   142,   575,   406,   444,   550,   336,   350,
     359,   272,   446,   370,    58,    59,    61,    64,    78,    84,
     144,   178,   199,   235,   255,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   415,   416,   417,   418,   419,   421,
     426,   427,   534,   583,   585,   589,   593,   446,   446,    26,
     369,   378,   124,   146,   153,   187,   379,   387,   567,    83,
     272,   188,    84,   188,    47,    84,    47,   400,   272,   272,
      98,   369,   329,   400,   454,   446,   359,   455,    66,   439,
     131,   438,   445,   117,   101,   238,   491,   117,   272,   272,
     554,   272,   272,   272,   272,   630,   630,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   643,   665,   574,
     597,   574,   129,   270,    26,   276,    89,   114,   172,   576,
     577,   446,   648,   446,   481,   269,   677,   148,   191,   254,
     414,   269,    59,   534,   271,   630,   269,   271,   269,   269,
      54,   197,   405,   429,   430,    86,   269,   269,   269,   269,
     272,   672,    98,    90,   567,   386,   600,   681,   446,   406,
     406,   445,   446,   271,   440,   441,   442,   566,   574,   630,
     641,   641,   630,   272,   569,   569,   609,   571,   672,   114,
     271,   262,   374,   404,   269,   630,   683,   630,   414,    86,
     609,   420,   609,     9,    58,    69,   173,   431,   432,   430,
     422,   425,   606,   386,   600,   129,   388,   389,   390,   446,
     455,   441,   442,   270,   568,   569,   446,   558,   573,   573,
     576,   578,   579,   580,   686,   375,   269,   269,   423,    86,
     269,   630,   425,   272,   269,   272,   677,   679,   692,   642,
      50,   270,   269,   270,   388,    47,   188,   386,   129,   390,
     129,   389,   446,   272,   441,    89,   573,   272,   574,   575,
     575,   272,   270,    83,   120,   133,   581,   580,   156,   424,
     425,    86,    10,   428,   181,    50,   405,   432,   422,   606,
     386,   123,   164,   211,   391,   391,   441,   569,   536,   536,
     578,   271,   681,   271,   425,   269,   269,   425,    39,    50,
     405,   405,   209,    46,    98,   129,   573,   582,   681,   425,
      50,   675,   405,    53,   609,   575,   272,   270,   272,   405,
     405,   536,   582
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

#ifndef SHLIB_DEFS
int DSQL_yychar;
#endif


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
			lex.g_field->fld_character_length = (USHORT) yyvsp[-2]; }
    break;

  case 81:
    { yyval = (DSQL_NOD) NULL; }
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
		  		(DSQL_NOD) NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
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
    { yyval = (DSQL_NOD) NULL; }
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
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 137:
    { yyval = (DSQL_NOD) NULL; }
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
    {yyval = (DSQL_NOD) NULL;}
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
    {yyval = (DSQL_NOD) NULL;}
    break;

  case 164:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 168:
    { yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;

  case 169:
    { yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
    break;

  case 170:
    { yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
    break;

  case 171:
    { yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
    break;

  case 172:
    { yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
    break;

  case 173:
    { if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;

  case 174:
    { if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
    break;

  case 175:
    { lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 176:
    { lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
    break;

  case 177:
    { lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file);}
    break;

  case 178:
    { lex.g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
    break;

  case 180:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 181:
    { 
		         check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
                                                (DSQL_NOD) lex.g_file); }
    break;

  case 182:
    { lex.g_file = make_file();
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
    break;

  case 185:
    { lex.g_file->fil_length = (SLONG) yyvsp[0]; }
    break;

  case 186:
    { lex.g_file  = make_file ();}
    break;

  case 191:
    { lex.g_file->fil_start = (SLONG) yyvsp[0];}
    break;

  case 192:
    { lex.g_file->fil_length = (SLONG) yyvsp[-1];}
    break;

  case 199:
    { yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;

  case 200:
    { yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;

  case 201:
    { yyval = yyvsp[0]; }
    break;

  case 202:
    { yyval = yyvsp[0]; }
    break;

  case 203:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 205:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 208:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;

  case 209:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 210:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 211:
    { 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;

  case 214:
    { yyval = NULL; }
    break;

  case 215:
    { yyval = make_node (nod_def_domain, (int) e_dom_count,
                                            yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 216:
    { yyval = yyvsp[0]; }
    break;

  case 217:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 218:
    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;

  case 219:
    { lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
    break;

  case 220:
    { yyval = yyvsp[-1]; }
    break;

  case 221:
    { lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;

  case 222:
    { yyval = yyvsp[0]; }
    break;

  case 223:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 227:
    { yyval = yyvsp[0]; }
    break;

  case 228:
    { yyval = yyvsp[0]; }
    break;

  case 229:
    { yyval = yyvsp[0]; }
    break;

  case 230:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 233:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 234:
    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 235:
    { yyval = make_node (nod_null, (int) 1, NULL); }
    break;

  case 236:
    { yyval = make_node (nod_foreign, e_for_count,
                        make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 238:
    { yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;

  case 239:
    { yyval = make_node (nod_primary, e_pri_count, NULL, yyvsp[0]); }
    break;

  case 240:
    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 241:
    { yyval = yyvsp[0]; }
    break;

  case 242:
    { yyval = NULL ;}
    break;

  case 247:
    { yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 248:
    { yyval = make_node (nod_primary, e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 249:
    { yyval = make_node (nod_foreign, e_for_count, yyvsp[-5], yyvsp[-3], 
			         yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 250:
    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;

  case 251:
    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 252:
    { yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 253:
    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], NULL);}
    break;

  case 254:
    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;

  case 255:
    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;

  case 256:
    { yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;

  case 257:
    { yyval = NULL;}
    break;

  case 258:
    { yyval = yyvsp[0];}
    break;

  case 259:
    { yyval = yyvsp[0];}
    break;

  case 260:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, e_ref_trig_action_count, NULL);}
    break;

  case 261:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, e_ref_trig_action_count, NULL);}
    break;

  case 262:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, e_ref_trig_action_count, NULL);}
    break;

  case 263:
    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, e_ref_trig_action_count, NULL);}
    break;

  case 264:
    { yyval = make_node (nod_def_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 265:
    { yyval = make_node (nod_redef_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 266:
    { yyval = make_node (nod_replace_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 267:
    { yyval = make_node (nod_mod_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 268:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 269:
    { yyval = NULL; }
    break;

  case 270:
    { yyval = yyvsp[0]; }
    break;

  case 271:
    { yyval = NULL; }
    break;

  case 273:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 274:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;

  case 275:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 276:
    { yyval = NULL; }
    break;

  case 278:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 279:
    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], NULL, NULL, NULL, NULL, NULL); }
    break;

  case 280:
    { yyval = NULL; }
    break;

  case 281:
    { yyval = NULL; }
    break;

  case 282:
    { yyval = yyvsp[0]; }
    break;

  case 283:
    { yyval = yyvsp[0]; }
    break;

  case 286:
    { yyval = yyvsp[-1]; }
    break;

  case 287:
    { yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[0]), NULL); }
    break;

  case 288:
    { yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;

  case 289:
    { yyval = make_node (nod_block, e_blk_count, NULL, NULL);}
    break;

  case 291:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 307:
    { yyval = make_node (nod_return, e_rtn_count, NULL); }
    break;

  case 308:
    { yyval = make_node (nod_exit, 0, NULL); }
    break;

  case 310:
    { yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
    break;

  case 311:
    { yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 312:
    { yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
    break;

  case 313:
    { yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
    break;

  case 314:
    { yyval = make_node (nod_exec_sql, e_exec_sql_count, yyvsp[-1]); }
    break;

  case 317:
    { yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
    break;

  case 318:
    { 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
    break;

  case 319:
    { 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
    break;

  case 320:
    { yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;

  case 321:
    { yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;

  case 322:
    { yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
    break;

  case 323:
    { yyval = NULL; }
    break;

  case 324:
    { yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
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
    { yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
    break;

  case 337:
    { yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
    break;

  case 338:
    { yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
    break;

  case 339:
    { yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;

  case 340:
    { yyval = NULL; }
    break;

  case 342:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 343:
    { yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;

  case 345:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 346:
    { yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;

  case 347:
    { yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;

  case 348:
    { yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;

  case 349:
    { yyval = make_node (nod_default, 1, NULL); }
    break;

  case 350:
    { yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;

  case 351:
    { yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 352:
    { yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 353:
    { yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
    break;

  case 354:
    { yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
    break;

  case 355:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 356:
    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 357:
    { yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 358:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 360:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 363:
    { yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 364:
    { yyval = yyvsp[-1]; }
    break;

  case 365:
    { lex.beginning = lex_position(); }
    break;

  case 366:
    { yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
    break;

  case 367:
    { lex.beginning = lex.last_token; }
    break;

  case 368:
    { yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
    break;

  case 369:
    { yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 370:
    { yyval = 0; }
    break;

  case 371:
    { yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 372:
    { yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 373:
    { yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;

  case 374:
    { yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;

  case 375:
    { yyval = NULL; }
    break;

  case 376:
    { yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;

  case 377:
    { yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
    break;

  case 378:
    { yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
    break;

  case 379:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
    break;

  case 380:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
    break;

  case 381:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
    break;

  case 382:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
    break;

  case 383:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
    break;

  case 384:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
    break;

  case 385:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
    break;

  case 386:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
    break;

  case 387:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
    break;

  case 388:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
    break;

  case 389:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
    break;

  case 390:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
    break;

  case 391:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
    break;

  case 392:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
    break;

  case 393:
    { yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
    break;

  case 394:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 395:
    { yyval = NULL; }
    break;

  case 396:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 397:
    { yyval = yyvsp[0]; }
    break;

  case 398:
    { yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 399:
    { yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 400:
    { yyval = yyvsp[0]; }
    break;

  case 401:
    { yyval = yyvsp[0]; }
    break;

  case 402:
    { yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;

  case 403:
    { yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 404:
    { yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
    break;

  case 405:
    { yyval = yyvsp[0]; }
    break;

  case 406:
    { yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 408:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 409:
    { yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
    break;

  case 410:
    { yyval = yyvsp[0]; }
    break;

  case 411:
    { yyval = yyvsp[0]; }
    break;

  case 412:
    {yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;

  case 413:
    { yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;

  case 414:
    { yyval = yyvsp[0]; }
    break;

  case 415:
    { yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;

  case 417:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 418:
    { yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 419:
    { yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;

  case 420:
    { yyval = yyvsp[0]; }
    break;

  case 421:
    { yyval = yyvsp[0]; }
    break;

  case 422:
    { yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 423:
    { yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 424:
    { yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;

  case 425:
    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 449:
    { yyval = NULL; }
    break;

  case 450:
    { yyval = NULL; }
    break;

  case 451:
    { yyval = NULL; }
    break;

  case 452:
    { yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 453:
    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
    break;

  case 454:
    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 455:
    { yyval = make_node (nod_cascade, 0, NULL); }
    break;

  case 456:
    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 457:
    { yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;

  case 458:
    { yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;

  case 459:
    { lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
    break;

  case 461:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 462:
    { yyval = yyvsp[0]; }
    break;

  case 463:
    { yyval = make_node (nod_drop_log, (int) 0, NULL); }
    break;

  case 464:
    { yyval = yyvsp[0]; }
    break;

  case 465:
    { yyval = yyvsp[0]; }
    break;

  case 467:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 468:
    { yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 470:
    { yyval = NULL; }
    break;

  case 472:
    { yyval = NULL; }
    break;

  case 473:
    { yyval = yyvsp[0]; }
    break;

  case 474:
    { yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;

  case 475:
    { yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;

  case 476:
    { yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;

  case 477:
    { yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;

  case 478:
    { yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;

  case 479:
    { yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;

  case 480:
    { yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;

  case 481:
    { yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;

  case 482:
    { yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;

  case 483:
    { yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;

  case 484:
    { yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;

  case 485:
    { yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;

  case 490:
    { lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
    break;

  case 491:
    { lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
    break;

  case 493:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 494:
    { if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
    break;

  case 495:
    { yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 501:
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

  case 502:
    { 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;

  case 503:
    { 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;

  case 504:
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

  case 505:
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

  case 506:
    { 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;

  case 509:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			}
    break;

  case 510:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;

  case 511:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
    break;

  case 512:
    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
    break;

  case 513:
    {
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
    break;

  case 514:
    {
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;

  case 515:
    {
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
    break;

  case 516:
    {
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;

  case 517:
    {
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;

  case 518:
    {
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;

  case 520:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 521:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 522:
    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 523:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
    break;

  case 524:
    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;

  case 525:
    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
    break;

  case 534:
    { 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;

  case 535:
    {  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
    break;

  case 536:
    { yyval = make_node (nod_position, 1, yyvsp[0]); }
    break;

  case 537:
    {
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
    break;

  case 538:
    {         
			if ( ((SLONG) yyvsp[-1] < 1) || ((SLONG) yyvsp[-1] > 18) )
			    yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((SLONG) yyvsp[-1] > 9)
			    {
			    if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect     >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect     <= SQL_DIALECT_V5) ) )
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
			lex.g_field->fld_precision = (USHORT) yyvsp[-1];
			}
    break;

  case 539:
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
				   (db_dialect     >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect     <= SQL_DIALECT_V5) ) )
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
			lex.g_field->fld_precision = (USHORT) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT) yyvsp[-1];
			}
    break;

  case 542:
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

  case 543:
    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 544:
    { 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;

  case 545:
    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 546:
    { yyval = yyvsp[-1]; }
    break;

  case 547:
    { yyval = 0; }
    break;

  case 551:
    { 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
    break;

  case 552:
    {
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
    break;

  case 553:
    {
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
    break;

  case 557:
    { yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;

  case 558:
    { yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 559:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 560:
    { yyval = 0; }
    break;

  case 561:
    { yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;

  case 564:
    { yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;

  case 565:
    { yyval = make_node (nod_rollback, 0, NULL); }
    break;

  case 568:
    { yyval = make_node (nod_commit_retain, 0, NULL); }
    break;

  case 569:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 571:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 572:
    {yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;

  case 574:
    { yyval = (DSQL_NOD) NULL; }
    break;

  case 576:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 581:
    { yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;

  case 582:
    { yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;

  case 583:
    { yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;

  case 584:
    { yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;

  case 585:
    { yyval = yyvsp[0];}
    break;

  case 587:
    { yyval = yyvsp[0];}
    break;

  case 588:
    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 589:
    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 590:
    { yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;

  case 591:
    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 592:
    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 593:
    { yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;

  case 594:
    { yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;

  case 595:
    { yyval = 0; }
    break;

  case 596:
    { yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;

  case 597:
    { yyval = (DSQL_NOD) NOD_SHARED; }
    break;

  case 598:
    { yyval = (DSQL_NOD) NOD_PROTECTED ; }
    break;

  case 599:
    { yyval = (DSQL_NOD) 0; }
    break;

  case 600:
    { yyval = (DSQL_NOD) NOD_READ; }
    break;

  case 601:
    { yyval = (DSQL_NOD) NOD_WRITE; }
    break;

  case 603:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 604:
    { yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 605:
    { yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
    break;

  case 606:
    { yyval = 0; }
    break;

  case 608:
    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 609:
    {yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
    break;

  case 610:
    { yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 611:
    { yyval = make_node (nod_list, 1, yyvsp[0]); }
    break;

  case 612:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 613:
    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 614:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 615:
    { yyval = 0; }
    break;

  case 617:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 618:
    { yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 619:
    { yyval = 0; }
    break;

  case 620:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 621:
    { yyval = 0; }
    break;

  case 622:
    { yyval = MAKE_constant((STR)NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;

  case 623:
    { yyval = MAKE_constant((STR)NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;

  case 624:
    { yyval = yyvsp[-1]; }
    break;

  case 625:
    { yyval = 0; }
    break;

  case 626:
    { yyval = make_node (nod_for_update, 1, yyvsp[0]); }
    break;

  case 627:
    { yyval = 0; }
    break;

  case 628:
    { yyval = yyvsp[0]; }
    break;

  case 629:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 630:
    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 631:
    { yyval = 0; }
    break;

  case 632:
    { yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
    break;

  case 633:
    { yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
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
    { yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;

  case 639:
    { yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
    break;

  case 640:
    { yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
    break;

  case 641:
    { yyval = 0; }
    break;

  case 642:
    { yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
    break;

  case 643:
    { yyval = yyvsp[-2]; }
    break;

  case 644:
    { yyval = yyvsp[-1]; }
    break;

  case 645:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
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
    { yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 656:
    { yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 657:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 659:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 662:
    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 663:
    { yyval = yyvsp[-1]; }
    break;

  case 664:
    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
    break;

  case 665:
    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 666:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 667:
    { yyval = NULL; }
    break;

  case 669:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 673:
    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 674:
    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;

  case 675:
    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 676:
    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 677:
    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 678:
    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 679:
    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 680:
    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 681:
    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 682:
    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 683:
    { yyval = make_list (yyvsp[0]); }
    break;

  case 684:
    { yyval = 0; }
    break;

  case 686:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 691:
    { yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;

  case 695:
    { yyval = yyvsp[0]; }
    break;

  case 696:
    { yyval = 0; }
    break;

  case 697:
    { yyval = yyvsp[0]; }
    break;

  case 698:
    { yyval = 0; }
    break;

  case 699:
    { yyval = yyvsp[0]; }
    break;

  case 700:
    { yyval = 0; }
    break;

  case 701:
    { yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 702:
    { yyval = 0; }
    break;

  case 703:
    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 704:
    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 705:
    { yyval = 0; }
    break;

  case 706:
    { yyval = 0; }
    break;

  case 708:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 709:
    { yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 712:
    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 713:
    { yyval = make_node (nod_natural, (int) 0, NULL); }
    break;

  case 714:
    { yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;

  case 715:
    { yyval = make_node (nod_index_order, 1, yyvsp[0]); }
    break;

  case 717:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 718:
    { yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;

  case 719:
    { yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 721:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 724:
    { yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 725:
    { yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 726:
    { yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
    break;

  case 729:
    { yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
    break;

  case 730:
    { yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
    break;

  case 732:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 733:
    { yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;

  case 736:
    { yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 737:
    { yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 738:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 739:
    { yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;

  case 743:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 744:
    { yyval = yyvsp[0]; }
    break;

  case 746:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 749:
    { yyval = NULL; }
    break;

  case 750:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 752:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 754:
    { yyval = NULL; }
    break;

  case 755:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 757:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 759:
    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 760:
    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;

  case 761:
    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 763:
    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 765:
    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 767:
    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 768:
    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 770:
    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 771:
    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 772:
    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 773:
    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 774:
    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 776:
    { yyval = yyvsp[-1]; }
    break;

  case 777:
    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 788:
    { yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 789:
    { yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 790:
    { yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 791:
    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 792:
    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 793:
    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 794:
    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 795:
    { yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 796:
    { yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 797:
    { yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 798:
    { yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 799:
    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 800:
    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 801:
    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 802:
    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 803:
    { yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 804:
    { yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 805:
    { yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 806:
    { yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 807:
    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 808:
    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 809:
    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 810:
    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 811:
    { yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 814:
    { yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 815:
    { yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 816:
    { yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 817:
    { yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 818:
    { yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 819:
    { yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 820:
    { yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 821:
    { yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 822:
    { yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 823:
    { yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 824:
    { yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 825:
    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 826:
    { yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 827:
    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 828:
    { yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;

  case 829:
    { yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;

  case 830:
    { yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;

  case 831:
    { yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;

  case 832:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;

  case 833:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;

  case 834:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;

  case 835:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;

  case 836:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
    break;

  case 837:
    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
    break;

  case 839:
    { yyval = make_list (yyvsp[-1]); }
    break;

  case 840:
    { yyval = yyvsp[-1]; }
    break;

  case 841:
    { yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 842:
    { yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
    break;

  case 852:
    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 853:
    { yyval = yyvsp[0]; }
    break;

  case 854:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 855:
    { yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 856:
    { yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
    break;

  case 857:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 858:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 859:
    {
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 860:
    { yyval = yyvsp[-1]; }
    break;

  case 861:
    { yyval = yyvsp[-1]; }
    break;

  case 865:
    { yyval = make_node (nod_dbkey, 1, NULL); }
    break;

  case 866:
    { yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;

  case 867:
    { 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
    break;

  case 868:
    { yyval = yyvsp[0]; }
    break;

  case 869:
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

  case 870:
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

  case 871:
    { yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;

  case 872:
    { yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 874:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 876:
    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 877:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
    break;

  case 878:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 879:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
    break;

  case 880:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 881:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 883:
    { yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
    break;

  case 884:
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

  case 885:
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

  case 886:
    { yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;

  case 887:
    { yyval = make_parameter (); }
    break;

  case 888:
    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 889:
    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 890:
    { yyval = make_node (nod_current_role, 0, NULL); }
    break;

  case 891:
    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
    break;

  case 892:
    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
    break;

  case 893:
    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
    break;

  case 894:
    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
    break;

  case 895:
    { yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
    break;

  case 896:
    { yyval = yyvsp[0]; }
    break;

  case 897:
    { ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;

  case 899:
    { yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;

  case 900:
    { if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 901:
    { if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 902:
    { if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;

  case 903:
    { if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 905:
    { yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
    break;

  case 906:
    { yyval = yyvsp[0];}
    break;

  case 911:
    { yyval = make_node (nod_agg_count, 0, NULL); }
    break;

  case 912:
    { yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;

  case 913:
    { yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;

  case 914:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;

  case 915:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 916:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;

  case 917:
    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 918:
    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 919:
    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 920:
    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 921:
    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 922:
    { 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
    break;

  case 923:
    { yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;

  case 924:
    { yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
    break;

  case 925:
    { yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
    break;

  case 926:
    { yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;

  case 927:
    { yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 928:
    { yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;

  case 929:
    { yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;

  case 932:
    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;

  case 933:
    { yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 936:
    { yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 937:
    { yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 938:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 939:
    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 940:
    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 941:
    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 942:
    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 943:
    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 947:
    { yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
    break;

  case 948:
    { yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
    break;

  case 949:
    { yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
    break;

  case 950:
    { yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
    break;

  case 951:
    { yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
    break;

  case 952:
    { yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
    break;

  case 953:
    { yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
    break;

  case 954:
    { yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
    break;

  case 957:
    { yyval = make_node (nod_null, 0, NULL); }
    break;


    }

/* Line 1016 of /usr/local/share/bison/yacc.c.  */
#line 7897 "y.tab.c"

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
static BOOLEAN long_int (
    DSQL_NOD		string,
    SLONG	*long_value)
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
			return FALSE;
		}
	}

	*long_value = atol ((char *)((STR) string)->str_data);

	return TRUE;
}
#endif

static DSQL_FLD make_field (
    DSQL_NOD		field_name)
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
TSQL    tdsql;

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
TSQL    tdsql;

tdsql = GET_THREAD_DATA;
       
temp_file = FB_NEW(*tdsql->tsql_default) fil;

return temp_file;
}


static DSQL_NOD make_list (
    DSQL_NOD		node)
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
TSQL    tdsql;

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
TSQL    tdsql;

tdsql = GET_THREAD_DATA;

node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
node->nod_type = nod_parameter;
node->nod_line = (USHORT) lex.lines_bk;
node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
node->nod_count = 1;
node->nod_arg[0] = (DSQL_NOD)lex.param_number++;

return node;
}


static DSQL_NOD make_node (
    NOD_TYPE	type,
    int		count,
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
TSQL    tdsql;

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


static DSQL_NOD make_flag_node (
    NOD_TYPE	type,
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
TSQL    tdsql;

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
    DSQL_debug = level;
    if (level >> 8)
        *yydeb = level >> 8;
    /* CVC: I added this code form Mike Nordell to see the output from internal
       operations that's generated in DEV build when DEBUG <n> is typed into isql.exe.
       When n>0, the output console is activated; otherwise it's closed. */
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
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
static BOOLEAN short_int (
    DSQL_NOD		string,
    SLONG	*long_value,
    SSHORT	range)
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
		return FALSE;
	}

	for (char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return FALSE;
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

	BOOLEAN return_value;

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

static void stack_nodes (
    DSQL_NOD		node,
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
DSQL_NOD     curr_node, next_node, start_chain, end_chain, save_link;

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
end_chain = (DSQL_NOD) NULL;
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
    while ( TRUE)
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

STR	delimited_id_str;

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

    BOOLEAN have_error     = FALSE;	/* syntax error or value too large */
    BOOLEAN have_digit     = FALSE;	/* we've seen a digit              */
    BOOLEAN have_decimal   = FALSE;	/* we've seen a '.'                */
    BOOLEAN have_exp       = FALSE;	/* digit ... [eE]                  */
    BOOLEAN have_exp_sign  = FALSE; /* digit ... [eE] {+-]             */
    BOOLEAN have_exp_digit = FALSE; /* digit ... [eE] ... digit        */
    UINT64	number         = 0;
    UINT64	limit_by_10    = MAX_SINT64 / 10;

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
	      have_error = TRUE;
	      break;
	    }
	else if (have_exp)
	    {
	    /* We've seen e or E, but nothing beyond that. */
	    if ( ('-' == c) || ('+' == c) )
		have_exp_sign = TRUE;
	    else if ( classes [c]  & CHR_DIGIT )
		/* We have a digit: we haven't seen a sign yet,
		   but it's too late now. */
		have_exp_digit = have_exp_sign  = TRUE;
	    else
		{
		/* end of the token */
		have_error = TRUE;
		break;
		}
	    }
	else if ('.' == c)
	    {
	    if (!have_decimal)
		have_decimal = TRUE;
	    else
		{
		have_error = TRUE;
		break;
		}
	    }
	else if (classes [c] & CHR_DIGIT)
	  {
	    /* Before computing the next value, make sure there will be
	       no overflow.  */

	    have_digit = TRUE;

	    if (number >= limit_by_10)
		/* possibility of an overflow */
		if ((number > limit_by_10) || (c > '8'))
		    {
		    have_error = TRUE;
		    break;
		    }
	    number = number * 10 + (c - '0');
	  }
	else if ( (('E' == c) || ('e' == c)) && have_digit )
	    have_exp = TRUE;
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
		yylval = (DSQL_NOD) number;
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
    sym = HSHD_lookup (NULL_PTR, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
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
    sym = HSHD_lookup (NULL_PTR, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
	gds_arg_gds, gds_command_end_err,    /* Unexpected end of command */
	0);
else
    {
    ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
 	gds_arg_gds, gds_dsql_token_unk_err, 
	gds_arg_number, (SLONG) lex.lines, 
	gds_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
	    /* Token unknown - line %d, char %d */
 	gds_arg_gds, gds_random, 
	gds_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token,
 	0);
    }
}


static void yyabandon (
    SSHORT      sql_code,
    ISC_STATUS      error_symbol)
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

