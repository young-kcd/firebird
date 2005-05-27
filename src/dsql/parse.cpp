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
     BEFORE = 270,
     BEGIN = 271,
     BETWEEN = 272,
     BLOB = 273,
     BY = 274,
     CAST = 275,
     CHARACTER = 276,
     CHECK = 277,
     COLLATE = 278,
     COMMA = 279,
     COMMIT = 280,
     COMMITTED = 281,
     COMPUTED = 282,
     CONCATENATE = 283,
     CONDITIONAL = 284,
     CONSTRAINT = 285,
     CONTAINING = 286,
     COUNT = 287,
     CREATE = 288,
     CSTRING = 289,
     CURRENT = 290,
     CURSOR = 291,
     DATABASE = 292,
     DATE = 293,
     DB_KEY = 294,
     KW_DEBUG = 295,
     DECIMAL = 296,
     DECLARE = 297,
     DEFAULT = 298,
     KW_DELETE = 299,
     DESC = 300,
     DISTINCT = 301,
     DO = 302,
     DOMAIN = 303,
     DROP = 304,
     ELSE = 305,
     END = 306,
     ENTRY_POINT = 307,
     EQL = 308,
     ESCAPE = 309,
     EXCEPTION = 310,
     EXECUTE = 311,
     EXISTS = 312,
     EXIT = 313,
     EXTERNAL = 314,
     FILTER = 315,
     FOR = 316,
     FOREIGN = 317,
     FROM = 318,
     FULL = 319,
     FUNCTION = 320,
     GDSCODE = 321,
     GEQ = 322,
     GENERATOR = 323,
     GEN_ID = 324,
     GRANT = 325,
     GROUP = 326,
     GTR = 327,
     HAVING = 328,
     IF = 329,
     KW_IN = 330,
     INACTIVE = 331,
     INNER = 332,
     INPUT_TYPE = 333,
     INDEX = 334,
     INSERT = 335,
     INTEGER = 336,
     INTO = 337,
     IS = 338,
     ISOLATION = 339,
     JOIN = 340,
     KEY = 341,
     KW_CHAR = 342,
     KW_DEC = 343,
     KW_DOUBLE = 344,
     KW_FILE = 345,
     KW_FLOAT = 346,
     KW_INT = 347,
     KW_LONG = 348,
     KW_NULL = 349,
     KW_NUMERIC = 350,
     KW_UPPER = 351,
     KW_VALUE = 352,
     LENGTH = 353,
     LPAREN = 354,
     LEFT = 355,
     LEQ = 356,
     LEVEL = 357,
     LIKE = 358,
     LSS = 359,
     MANUAL = 360,
     MAXIMUM = 361,
     MAX_SEGMENT = 362,
     MERGE = 363,
     MESSAGE = 364,
     MINIMUM = 365,
     MODULE_NAME = 366,
     NAMES = 367,
     NATIONAL = 368,
     NATURAL = 369,
     NCHAR = 370,
     NEQ = 371,
     NO = 372,
     NOT = 373,
     NOT_GTR = 374,
     NOT_LSS = 375,
     OF = 376,
     ON = 377,
     ONLY = 378,
     OPTION = 379,
     OR = 380,
     ORDER = 381,
     OUTER = 382,
     OUTPUT_TYPE = 383,
     OVERFLOW = 384,
     PAGE = 385,
     PAGES = 386,
     KW_PAGE_SIZE = 387,
     PARAMETER = 388,
     PASSWORD = 389,
     PLAN = 390,
     POSITION = 391,
     POST_EVENT = 392,
     PRECISION = 393,
     PRIMARY = 394,
     PRIVILEGES = 395,
     PROCEDURE = 396,
     PROTECTED = 397,
     READ = 398,
     REAL = 399,
     REFERENCES = 400,
     RESERVING = 401,
     RETAIN = 402,
     RETURNING_VALUES = 403,
     RETURNS = 404,
     REVOKE = 405,
     RIGHT = 406,
     RPAREN = 407,
     ROLLBACK = 408,
     SEGMENT = 409,
     SELECT = 410,
     SET = 411,
     SHADOW = 412,
     KW_SHARED = 413,
     SINGULAR = 414,
     KW_SIZE = 415,
     SMALLINT = 416,
     SNAPSHOT = 417,
     SOME = 418,
     SORT = 419,
     SQLCODE = 420,
     STABILITY = 421,
     STARTING = 422,
     STATISTICS = 423,
     SUB_TYPE = 424,
     SUSPEND = 425,
     SUM = 426,
     TABLE = 427,
     THEN = 428,
     TO = 429,
     TRANSACTION = 430,
     TRIGGER = 431,
     UNCOMMITTED = 432,
     UNION = 433,
     UNIQUE = 434,
     UPDATE = 435,
     USER = 436,
     VALUES = 437,
     VARCHAR = 438,
     VARIABLE = 439,
     VARYING = 440,
     VERSION = 441,
     VIEW = 442,
     WAIT = 443,
     WHEN = 444,
     WHERE = 445,
     WHILE = 446,
     WITH = 447,
     WORK = 448,
     WRITE = 449,
     FLOAT_NUMBER = 450,
     NUMBER = 451,
     NUMERIC = 452,
     SYMBOL = 453,
     STRING = 454,
     INTRODUCER = 455,
     ACTION = 456,
     ADMIN = 457,
     CASCADE = 458,
     FREE_IT = 459,
     RESTRICT = 460,
     ROLE = 461,
     COLUMN = 462,
     TYPE = 463,
     EXTRACT = 464,
     YEAR = 465,
     MONTH = 466,
     DAY = 467,
     HOUR = 468,
     MINUTE = 469,
     SECOND = 470,
     WEEKDAY = 471,
     YEARDAY = 472,
     TIME = 473,
     TIMESTAMP = 474,
     CURRENT_DATE = 475,
     CURRENT_TIME = 476,
     CURRENT_TIMESTAMP = 477,
     NUMBER64BIT = 478,
     SCALEDINT = 479,
     CURRENT_USER = 480,
     CURRENT_ROLE = 481,
     KW_BREAK = 482,
     SUBSTRING = 483,
     RECREATE = 484,
     KW_DESCRIPTOR = 485,
     FIRST = 486,
     SKIP = 487,
     CURRENT_CONNECTION = 488,
     CURRENT_TRANSACTION = 489,
     BIGINT = 490,
     CASE = 491,
     NULLIF = 492,
     COALESCE = 493,
     USING = 494,
     NULLS = 495,
     LAST = 496,
     ROW_COUNT = 497,
     LOCK = 498,
     SAVEPOINT = 499,
     RELEASE = 500,
     STATEMENT = 501,
     LEAVE = 502,
     INSERTING = 503,
     UPDATING = 504,
     DELETING = 505,
     KW_INSERTING = 506,
     KW_UPDATING = 507,
     KW_DELETING = 508,
     BACKUP = 509,
     KW_DIFFERENCE = 510,
     OPEN = 511,
     CLOSE = 512,
     FETCH = 513,
     ROWS = 514,
     BLOCK = 515,
     IIF = 516,
     SCALAR_ARRAY = 517,
     CROSS = 518,
     NEXT = 519,
     SEQUENCE = 520,
     RESTART = 521,
     BOTH = 522,
     COLLATION = 523,
     COMMENT = 524,
     BIT_LENGTH = 525,
     CHAR_LENGTH = 526,
     CHARACTER_LENGTH = 527,
     LEADING = 528,
     KW_LOWER = 529,
     OCTET_LENGTH = 530,
     TRAILING = 531,
     TRIM = 532
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
#define BEFORE 270
#define BEGIN 271
#define BETWEEN 272
#define BLOB 273
#define BY 274
#define CAST 275
#define CHARACTER 276
#define CHECK 277
#define COLLATE 278
#define COMMA 279
#define COMMIT 280
#define COMMITTED 281
#define COMPUTED 282
#define CONCATENATE 283
#define CONDITIONAL 284
#define CONSTRAINT 285
#define CONTAINING 286
#define COUNT 287
#define CREATE 288
#define CSTRING 289
#define CURRENT 290
#define CURSOR 291
#define DATABASE 292
#define DATE 293
#define DB_KEY 294
#define KW_DEBUG 295
#define DECIMAL 296
#define DECLARE 297
#define DEFAULT 298
#define KW_DELETE 299
#define DESC 300
#define DISTINCT 301
#define DO 302
#define DOMAIN 303
#define DROP 304
#define ELSE 305
#define END 306
#define ENTRY_POINT 307
#define EQL 308
#define ESCAPE 309
#define EXCEPTION 310
#define EXECUTE 311
#define EXISTS 312
#define EXIT 313
#define EXTERNAL 314
#define FILTER 315
#define FOR 316
#define FOREIGN 317
#define FROM 318
#define FULL 319
#define FUNCTION 320
#define GDSCODE 321
#define GEQ 322
#define GENERATOR 323
#define GEN_ID 324
#define GRANT 325
#define GROUP 326
#define GTR 327
#define HAVING 328
#define IF 329
#define KW_IN 330
#define INACTIVE 331
#define INNER 332
#define INPUT_TYPE 333
#define INDEX 334
#define INSERT 335
#define INTEGER 336
#define INTO 337
#define IS 338
#define ISOLATION 339
#define JOIN 340
#define KEY 341
#define KW_CHAR 342
#define KW_DEC 343
#define KW_DOUBLE 344
#define KW_FILE 345
#define KW_FLOAT 346
#define KW_INT 347
#define KW_LONG 348
#define KW_NULL 349
#define KW_NUMERIC 350
#define KW_UPPER 351
#define KW_VALUE 352
#define LENGTH 353
#define LPAREN 354
#define LEFT 355
#define LEQ 356
#define LEVEL 357
#define LIKE 358
#define LSS 359
#define MANUAL 360
#define MAXIMUM 361
#define MAX_SEGMENT 362
#define MERGE 363
#define MESSAGE 364
#define MINIMUM 365
#define MODULE_NAME 366
#define NAMES 367
#define NATIONAL 368
#define NATURAL 369
#define NCHAR 370
#define NEQ 371
#define NO 372
#define NOT 373
#define NOT_GTR 374
#define NOT_LSS 375
#define OF 376
#define ON 377
#define ONLY 378
#define OPTION 379
#define OR 380
#define ORDER 381
#define OUTER 382
#define OUTPUT_TYPE 383
#define OVERFLOW 384
#define PAGE 385
#define PAGES 386
#define KW_PAGE_SIZE 387
#define PARAMETER 388
#define PASSWORD 389
#define PLAN 390
#define POSITION 391
#define POST_EVENT 392
#define PRECISION 393
#define PRIMARY 394
#define PRIVILEGES 395
#define PROCEDURE 396
#define PROTECTED 397
#define READ 398
#define REAL 399
#define REFERENCES 400
#define RESERVING 401
#define RETAIN 402
#define RETURNING_VALUES 403
#define RETURNS 404
#define REVOKE 405
#define RIGHT 406
#define RPAREN 407
#define ROLLBACK 408
#define SEGMENT 409
#define SELECT 410
#define SET 411
#define SHADOW 412
#define KW_SHARED 413
#define SINGULAR 414
#define KW_SIZE 415
#define SMALLINT 416
#define SNAPSHOT 417
#define SOME 418
#define SORT 419
#define SQLCODE 420
#define STABILITY 421
#define STARTING 422
#define STATISTICS 423
#define SUB_TYPE 424
#define SUSPEND 425
#define SUM 426
#define TABLE 427
#define THEN 428
#define TO 429
#define TRANSACTION 430
#define TRIGGER 431
#define UNCOMMITTED 432
#define UNION 433
#define UNIQUE 434
#define UPDATE 435
#define USER 436
#define VALUES 437
#define VARCHAR 438
#define VARIABLE 439
#define VARYING 440
#define VERSION 441
#define VIEW 442
#define WAIT 443
#define WHEN 444
#define WHERE 445
#define WHILE 446
#define WITH 447
#define WORK 448
#define WRITE 449
#define FLOAT_NUMBER 450
#define NUMBER 451
#define NUMERIC 452
#define SYMBOL 453
#define STRING 454
#define INTRODUCER 455
#define ACTION 456
#define ADMIN 457
#define CASCADE 458
#define FREE_IT 459
#define RESTRICT 460
#define ROLE 461
#define COLUMN 462
#define TYPE 463
#define EXTRACT 464
#define YEAR 465
#define MONTH 466
#define DAY 467
#define HOUR 468
#define MINUTE 469
#define SECOND 470
#define WEEKDAY 471
#define YEARDAY 472
#define TIME 473
#define TIMESTAMP 474
#define CURRENT_DATE 475
#define CURRENT_TIME 476
#define CURRENT_TIMESTAMP 477
#define NUMBER64BIT 478
#define SCALEDINT 479
#define CURRENT_USER 480
#define CURRENT_ROLE 481
#define KW_BREAK 482
#define SUBSTRING 483
#define RECREATE 484
#define KW_DESCRIPTOR 485
#define FIRST 486
#define SKIP 487
#define CURRENT_CONNECTION 488
#define CURRENT_TRANSACTION 489
#define BIGINT 490
#define CASE 491
#define NULLIF 492
#define COALESCE 493
#define USING 494
#define NULLS 495
#define LAST 496
#define ROW_COUNT 497
#define LOCK 498
#define SAVEPOINT 499
#define RELEASE 500
#define STATEMENT 501
#define LEAVE 502
#define INSERTING 503
#define UPDATING 504
#define DELETING 505
#define KW_INSERTING 506
#define KW_UPDATING 507
#define KW_DELETING 508
#define BACKUP 509
#define KW_DIFFERENCE 510
#define OPEN 511
#define CLOSE 512
#define FETCH 513
#define ROWS 514
#define BLOCK 515
#define IIF 516
#define SCALAR_ARRAY 517
#define CROSS 518
#define NEXT 519
#define SEQUENCE 520
#define RESTART 521
#define BOTH 522
#define COLLATION 523
#define COMMENT 524
#define BIT_LENGTH 525
#define CHAR_LENGTH 526
#define CHARACTER_LENGTH 527
#define LEADING 528
#define KW_LOWER 529
#define OCTET_LENGTH 530
#define TRAILING 531
#define TRIM 532




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

//const int MIN_CACHE_BUFFERS	= 250;
//const int DEF_CACHE_BUFFERS	= 1000;

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE	2048
#define YYMAXDEPTH	2048

/* Make bison allocate static stack */
#define YYINITDEPTH 2048

// Using this option causes build problems on Win32 with bison 1.28
//#define YYSTACK_USE_ALLOCA 1

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
#define YYFINAL  184
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   8953

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  294
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  457
/* YYNRULES -- Number of rules. */
#define YYNRULES  1113
/* YYNRULES -- Number of states. */
#define YYNSTATES  2021

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   532

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     287,   288,   283,   281,   286,   282,   292,   284,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   289,   285,
     279,   278,   280,   293,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   290,     2,   291,     2,     2,     2,     2,     2,     2,
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
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    48,    51,    60,    69,    75,    77,
      78,    80,    83,    85,    87,    91,    93,    95,    97,    99,
     102,   105,   109,   110,   114,   115,   117,   126,   135,   141,
     145,   146,   150,   151,   153,   155,   157,   161,   165,   169,
     172,   175,   178,   181,   183,   187,   189,   192,   195,   197,
     201,   203,   205,   209,   211,   214,   217,   220,   224,   233,
     235,   237,   243,   244,   246,   250,   252,   256,   260,   261,
     264,   267,   269,   271,   275,   279,   282,   283,   286,   289,
     291,   295,   305,   307,   309,   312,   315,   323,   326,   329,
     332,   335,   338,   341,   344,   347,   350,   353,   356,   359,
     362,   365,   368,   373,   376,   379,   382,   385,   388,   391,
     394,   396,   397,   399,   401,   408,   415,   417,   419,   420,
     421,   423,   424,   429,   430,   432,   434,   437,   446,   448,
     449,   453,   454,   455,   457,   459,   462,   464,   466,   468,
     471,   478,   480,   482,   486,   487,   489,   491,   492,   494,
     496,   499,   503,   508,   511,   514,   518,   519,   521,   523,
     526,   528,   533,   537,   541,   543,   544,   546,   548,   551,
     555,   560,   561,   563,   566,   567,   569,   571,   577,   583,
     587,   590,   591,   593,   597,   599,   601,   608,   612,   615,
     622,   625,   627,   630,   633,   636,   637,   639,   641,   644,
     645,   648,   649,   651,   653,   655,   657,   659,   661,   662,
     664,   666,   669,   672,   675,   681,   683,   686,   690,   693,
     696,   697,   699,   701,   703,   705,   709,   714,   723,   728,
     729,   736,   738,   740,   743,   746,   747,   751,   755,   757,
     760,   763,   766,   775,   784,   793,   802,   806,   807,   812,
     813,   815,   819,   824,   826,   830,   833,   837,   841,   843,
     845,   846,   848,   851,   856,   858,   860,   864,   866,   867,
     870,   872,   879,   881,   883,   887,   889,   892,   893,   895,
     898,   901,   904,   907,   910,   913,   916,   919,   922,   925,
     928,   931,   934,   937,   940,   943,   946,   948,   950,   952,
     954,   957,   961,   963,   967,   976,   986,   992,  1001,  1008,
    1012,  1013,  1017,  1020,  1022,  1024,  1028,  1032,  1040,  1043,
    1044,  1046,  1048,  1051,  1055,  1056,  1058,  1061,  1066,  1068,
    1072,  1075,  1078,  1081,  1083,  1085,  1087,  1089,  1092,  1095,
    1101,  1102,  1108,  1110,  1114,  1115,  1118,  1123,  1124,  1132,
    1136,  1137,  1139,  1143,  1147,  1155,  1163,  1164,  1165,  1166,
    1167,  1171,  1172,  1182,  1192,  1194,  1196,  1197,  1200,  1202,
    1204,  1206,  1208,  1210,  1214,  1218,  1222,  1226,  1230,  1234,
    1240,  1246,  1252,  1258,  1264,  1270,  1273,  1274,  1279,  1282,
    1285,  1289,  1292,  1295,  1299,  1303,  1306,  1309,  1313,  1317,
    1324,  1326,  1329,  1334,  1338,  1341,  1344,  1347,  1350,  1354,
    1356,  1360,  1364,  1368,  1371,  1374,  1379,  1384,  1390,  1392,
    1394,  1396,  1398,  1400,  1402,  1404,  1406,  1408,  1410,  1412,
    1414,  1416,  1418,  1420,  1422,  1424,  1426,  1428,  1430,  1432,
    1434,  1436,  1438,  1440,  1442,  1444,  1446,  1448,  1450,  1452,
    1454,  1456,  1458,  1460,  1462,  1464,  1466,  1468,  1470,  1472,
    1474,  1476,  1479,  1482,  1485,  1487,  1489,  1491,  1492,  1495,
    1498,  1503,  1508,  1514,  1518,  1521,  1522,  1525,  1526,  1527,
    1529,  1532,  1535,  1540,  1544,  1547,  1550,  1558,  1560,  1561,
    1563,  1564,  1567,  1570,  1573,  1576,  1579,  1582,  1585,  1588,
    1591,  1595,  1598,  1601,  1604,  1607,  1609,  1611,  1613,  1615,
    1620,  1626,  1628,  1632,  1634,  1638,  1640,  1643,  1645,  1647,
    1649,  1651,  1653,  1655,  1657,  1659,  1661,  1663,  1665,  1670,
    1675,  1682,  1688,  1692,  1693,  1696,  1699,  1700,  1704,  1705,
    1710,  1712,  1718,  1723,  1725,  1730,  1732,  1735,  1738,  1740,
    1742,  1744,  1747,  1750,  1753,  1756,  1757,  1761,  1767,  1769,
    1771,  1774,  1778,  1780,  1783,  1787,  1788,  1790,  1792,  1794,
    1800,  1806,  1813,  1815,  1817,  1819,  1822,  1827,  1829,  1830,
    1836,  1838,  1839,  1843,  1846,  1848,  1849,  1852,  1853,  1855,
    1856,  1860,  1862,  1863,  1865,  1868,  1870,  1872,  1874,  1876,
    1879,  1882,  1884,  1887,  1891,  1893,  1895,  1899,  1903,  1905,
    1908,  1912,  1914,  1917,  1918,  1921,  1923,  1925,  1926,  1928,
    1930,  1932,  1936,  1939,  1943,  1944,  1946,  1950,  1955,  1961,
    1968,  1976,  1978,  1980,  1982,  1984,  1986,  1988,  1991,  1993,
    1995,  1997,  1999,  2001,  2003,  2006,  2008,  2010,  2012,  2015,
    2017,  2019,  2023,  2027,  2028,  2031,  2032,  2035,  2036,  2040,
    2044,  2048,  2050,  2055,  2060,  2062,  2072,  2073,  2074,  2075,
    2076,  2080,  2083,  2085,  2086,  2090,  2096,  2100,  2103,  2109,
    2112,  2114,  2116,  2118,  2120,  2122,  2126,  2128,  2132,  2134,
    2135,  2138,  2140,  2144,  2146,  2148,  2150,  2152,  2156,  2163,
    2165,  2166,  2170,  2171,  2173,  2177,  2179,  2181,  2183,  2188,
    2194,  2200,  2202,  2204,  2207,  2212,  2217,  2220,  2224,  2225,
    2227,  2230,  2232,  2234,  2237,  2240,  2243,  2244,  2246,  2247,
    2251,  2252,  2254,  2258,  2260,  2263,  2264,  2267,  2268,  2271,
    2272,  2277,  2279,  2282,  2284,  2286,  2287,  2289,  2293,  2296,
    2298,  2300,  2303,  2305,  2310,  2314,  2316,  2320,  2325,  2326,
    2330,  2331,  2333,  2337,  2341,  2343,  2345,  2346,  2348,  2350,
    2355,  2356,  2359,  2364,  2365,  2374,  2380,  2382,  2384,  2392,
    2397,  2399,  2401,  2410,  2416,  2421,  2423,  2427,  2431,  2433,
    2441,  2449,  2455,  2459,  2460,  2462,  2464,  2466,  2469,  2470,
    2472,  2474,  2476,  2477,  2481,  2483,  2487,  2489,  2490,  2494,
    2496,  2500,  2502,  2506,  2510,  2512,  2514,  2518,  2520,  2523,
    2525,  2529,  2533,  2535,  2538,  2542,  2546,  2550,  2554,  2556,
    2560,  2563,  2565,  2567,  2569,  2571,  2573,  2575,  2577,  2579,
    2581,  2583,  2585,  2589,  2593,  2597,  2601,  2605,  2609,  2613,
    2617,  2624,  2631,  2638,  2645,  2652,  2659,  2666,  2673,  2680,
    2687,  2694,  2701,  2708,  2715,  2722,  2729,  2731,  2733,  2739,
    2746,  2752,  2759,  2763,  2768,  2774,  2781,  2785,  2790,  2794,
    2799,  2803,  2808,  2813,  2819,  2824,  2829,  2833,  2838,  2840,
    2842,  2844,  2846,  2848,  2850,  2852,  2856,  2860,  2862,  2864,
    2866,  2868,  2870,  2872,  2874,  2876,  2878,  2880,  2883,  2886,
    2890,  2894,  2898,  2902,  2906,  2910,  2914,  2918,  2920,  2922,
    2924,  2926,  2930,  2932,  2934,  2936,  2938,  2940,  2942,  2947,
    2949,  2953,  2955,  2958,  2960,  2962,  2964,  2966,  2968,  2970,
    2972,  2975,  2978,  2981,  2983,  2985,  2987,  2989,  2991,  2993,
    2995,  2997,  2999,  3001,  3004,  3006,  3009,  3011,  3013,  3015,
    3017,  3019,  3022,  3024,  3026,  3028,  3030,  3035,  3041,  3047,
    3053,  3059,  3065,  3071,  3077,  3083,  3089,  3095,  3097,  3099,
    3106,  3108,  3110,  3112,  3117,  3122,  3127,  3132,  3134,  3136,
    3141,  3146,  3154,  3162,  3169,  3176,  3181,  3183,  3185,  3187,
    3190,  3191,  3196,  3200,  3207,  3209,  3211,  3218,  3227,  3234,
    3236,  3238,  3243,  3250,  3255,  3261,  3265,  3271,  3276,  3282,
    3284,  3286,  3288,  3293,  3300,  3302,  3304,  3306,  3308,  3310,
    3312,  3314,  3316,  3318,  3319,  3321,  3322,  3324,  3326,  3328,
    3330,  3332,  3334,  3336,  3338,  3340,  3342,  3344,  3346,  3348,
    3350,  3352,  3354,  3356,  3358,  3360,  3362,  3364,  3366,  3368,
    3370,  3372,  3374,  3376,  3378,  3380,  3382,  3384,  3386,  3388,
    3390,  3392,  3394,  3396,  3398,  3400,  3402,  3404,  3406,  3408,
    3410,  3412,  3414,  3416,  3418,  3420,  3422,  3424,  3426,  3428,
    3430,  3432,  3434,  3436
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     295,     0,    -1,   296,    -1,   296,   285,    -1,   479,    -1,
     640,    -1,   559,    -1,   536,    -1,   331,    -1,   318,    -1,
     630,    -1,   504,    -1,   297,    -1,   629,    -1,   457,    -1,
     460,    -1,   333,    -1,   335,    -1,   306,    -1,   537,    -1,
     530,    -1,   565,    -1,   528,    -1,   633,    -1,    40,   687,
      -1,    70,   299,   122,   298,   605,   174,   309,   303,    -1,
      70,   301,   122,   141,   305,   174,   309,   303,    -1,    70,
     314,   174,   316,   304,    -1,   172,    -1,    -1,     6,    -1,
       6,   140,    -1,   300,    -1,   302,    -1,   300,   286,   302,
      -1,    56,    -1,   155,    -1,    80,    -1,    44,    -1,   180,
     646,    -1,   145,   646,    -1,   192,    70,   124,    -1,    -1,
     192,   202,   124,    -1,    -1,   740,    -1,   150,   307,   299,
     122,   298,   605,    63,   309,    -1,   150,   307,   301,   122,
     141,   305,    63,   309,    -1,   150,   308,   314,    63,   316,
      -1,    70,   124,    61,    -1,    -1,   202,   124,    61,    -1,
      -1,   310,    -1,   312,    -1,   311,    -1,   310,   286,   311,
      -1,   310,   286,   313,    -1,   312,   286,   311,    -1,   141,
     740,    -1,   176,   744,    -1,   187,   747,    -1,   206,   741,
      -1,   313,    -1,   312,   286,   313,    -1,   745,    -1,   181,
     745,    -1,    71,   745,    -1,   315,    -1,   314,   286,   315,
      -1,   741,    -1,   317,    -1,   316,   286,   317,    -1,   745,
      -1,   181,   745,    -1,    42,   319,    -1,    60,   329,    -1,
      59,    65,   320,    -1,   724,   322,   149,   326,    52,   686,
     111,   686,    -1,   511,    -1,    18,    -1,    34,   287,   690,
     288,   517,    -1,    -1,   323,    -1,   287,   323,   288,    -1,
     324,    -1,   323,   286,   324,    -1,   389,   321,   325,    -1,
      -1,    19,   230,    -1,    19,   262,    -1,    94,    -1,   327,
      -1,   287,   327,   288,    -1,   389,   321,   328,    -1,   133,
     690,    -1,    -1,    19,    97,    -1,    19,   230,    -1,   204,
      -1,    19,   230,   204,    -1,   733,    78,   330,   128,   330,
      52,   686,   111,   686,    -1,   725,    -1,   687,    -1,    33,
     332,    -1,    55,   337,    -1,   341,   625,    79,   736,   122,
     605,   342,    -1,   141,   408,    -1,   172,   376,    -1,   176,
     471,    -1,   187,   464,    -1,    68,   358,    -1,   265,   358,
      -1,    37,   360,    -1,    48,   349,    -1,   157,   343,    -1,
     206,   359,    -1,   229,   334,    -1,   141,   409,    -1,   172,
     377,    -1,   187,   465,    -1,    55,   338,    -1,    33,   125,
       7,   336,    -1,   141,   410,    -1,   176,   472,    -1,    55,
     339,    -1,   732,   686,    -1,   732,   686,    -1,   732,   686,
      -1,   732,   686,    -1,   179,    -1,    -1,   648,    -1,   647,
      -1,   383,   287,   468,   675,   469,   288,    -1,   690,   344,
     345,   686,   346,   347,    -1,   105,    -1,    14,    -1,    -1,
      -1,    29,    -1,    -1,    98,   361,   693,   375,    -1,    -1,
     348,    -1,   369,    -1,   348,   369,    -1,   386,   350,   506,
     468,   351,   469,   352,   385,    -1,    10,    -1,    -1,    43,
     468,   391,    -1,    -1,    -1,   353,    -1,   354,    -1,   353,
     354,    -1,   355,    -1,   356,    -1,   357,    -1,   118,    94,
      -1,   468,    22,   287,   655,   288,   469,    -1,   735,    -1,
     741,    -1,   362,   363,   366,    -1,    -1,   278,    -1,   686,
      -1,    -1,   364,    -1,   365,    -1,   364,   365,    -1,   132,
     361,   690,    -1,    98,   361,   693,   375,    -1,   181,   686,
      -1,   134,   686,    -1,   156,   112,   686,    -1,    -1,   367,
      -1,   368,    -1,   367,   368,    -1,   369,    -1,    43,    21,
     156,   726,    -1,   255,    90,   686,    -1,   370,   686,   371,
      -1,    90,    -1,    -1,   372,    -1,   373,    -1,   372,   373,
      -1,   167,   374,   693,    -1,    98,   361,   693,   375,    -1,
      -1,    12,    -1,    12,   130,    -1,    -1,   130,    -1,   131,
      -1,   605,   378,   287,   379,   288,    -1,   605,   378,   287,
     379,   288,    -1,    59,    90,   686,    -1,    59,   686,    -1,
      -1,   380,    -1,   379,   286,   380,    -1,   381,    -1,   396,
      -1,   386,   384,   351,   469,   392,   385,    -1,   386,   507,
     382,    -1,   386,   382,    -1,   383,   287,   468,   675,   469,
     288,    -1,    27,    19,    -1,    27,    -1,   506,   468,    -1,
     653,   466,    -1,    23,   727,    -1,    -1,   653,    -1,   653,
      -1,   389,   506,    -1,    -1,    43,   391,    -1,    -1,   679,
      -1,   683,    -1,   684,    -1,   685,    -1,   723,    -1,   676,
      -1,    -1,   393,    -1,   394,    -1,   393,   394,    -1,   397,
     395,    -1,   118,    94,    -1,   145,   605,   646,   404,   402,
      -1,   403,    -1,   179,   402,    -1,   139,    86,   402,    -1,
     397,   398,    -1,    30,   729,    -1,    -1,   399,    -1,   400,
      -1,   401,    -1,   403,    -1,   179,   647,   402,    -1,   139,
      86,   647,   402,    -1,    62,    86,   647,   145,   605,   646,
     404,   402,    -1,   239,   625,    79,   736,    -1,    -1,   468,
      22,   287,   655,   288,   469,    -1,   405,    -1,   406,    -1,
     406,   405,    -1,   405,   406,    -1,    -1,   122,   180,   407,
      -1,   122,    44,   407,    -1,   203,    -1,   156,    43,    -1,
     156,    94,    -1,   117,   201,    -1,   740,   412,   413,    10,
     466,   419,   428,   469,    -1,   740,   412,   413,    10,   466,
     419,   428,   469,    -1,   740,   412,   413,    10,   466,   419,
     428,   469,    -1,   740,   412,   413,    10,   466,   419,   428,
     469,    -1,   287,   414,   288,    -1,    -1,   149,   287,   416,
     288,    -1,    -1,   415,    -1,   414,   286,   415,    -1,   387,
     507,   418,   469,    -1,   417,    -1,   416,   286,   417,    -1,
     387,   507,    -1,    43,   466,   391,    -1,   278,   466,   391,
      -1,   466,    -1,   420,    -1,    -1,   421,    -1,   420,   421,
      -1,    42,   424,   422,   285,    -1,   423,    -1,   426,    -1,
     386,   507,   425,    -1,   184,    -1,    -1,   278,   391,    -1,
     390,    -1,   730,    36,    61,   287,   565,   288,    -1,   431,
      -1,   428,    -1,    16,   429,    51,    -1,   430,    -1,   430,
     448,    -1,    -1,   427,    -1,   430,   427,    -1,   638,   285,
      -1,   629,   285,    -1,   633,   285,    -1,   630,   285,    -1,
     441,   285,    -1,   457,   285,    -1,   434,   285,    -1,   437,
     285,    -1,   639,   285,    -1,   432,   285,    -1,   433,   285,
      -1,   439,   285,    -1,   452,   285,    -1,   446,   285,    -1,
     170,   285,    -1,    58,   285,    -1,   438,    -1,   444,    -1,
     435,    -1,   436,    -1,    55,   732,    -1,    55,   732,   675,
      -1,    55,    -1,    56,   246,   675,    -1,   445,    61,   565,
      82,   443,   447,    47,   427,    -1,   445,    61,    56,   246,
     675,    82,   443,    47,   427,    -1,    56,   246,   675,    82,
     443,    -1,    74,   287,   655,   288,   173,   427,    50,   427,
      -1,    74,   287,   655,   288,   173,   427,    -1,   137,   675,
     440,    -1,    -1,   565,    82,   443,    -1,   289,   746,    -1,
     442,    -1,   652,    -1,   443,   286,   652,    -1,   443,   286,
     442,    -1,   445,   191,   287,   655,   288,    47,   427,    -1,
     738,   289,    -1,    -1,   227,    -1,   247,    -1,   247,   738,
      -1,    10,    36,   730,    -1,    -1,   449,    -1,   448,   449,
      -1,   189,   450,    47,   427,    -1,   451,    -1,   450,   286,
     451,    -1,   165,   687,    -1,    66,   734,    -1,    55,   732,
      -1,     9,    -1,   453,    -1,   455,    -1,   454,    -1,   256,
     730,    -1,   257,   730,    -1,   258,   456,   730,    82,   443,
      -1,    -1,    56,   141,   740,   458,   459,    -1,   678,    -1,
     287,   678,   288,    -1,    -1,   148,   443,    -1,   148,   287,
     443,   288,    -1,    -1,    56,   260,   461,   413,    10,   419,
     428,    -1,   287,   462,   288,    -1,    -1,   463,    -1,   462,
     286,   463,    -1,   417,   278,   682,    -1,   747,   646,    10,
     466,   569,   470,   467,    -1,   747,   646,    10,   466,   569,
     470,   467,    -1,    -1,    -1,    -1,    -1,   192,    22,   124,
      -1,    -1,   744,    61,   605,   473,   474,   477,   468,   478,
     469,    -1,   744,    61,   605,   473,   474,   477,   468,   478,
     469,    -1,     3,    -1,    76,    -1,    -1,   475,   476,    -1,
      15,    -1,     5,    -1,    80,    -1,   180,    -1,    44,    -1,
      80,   125,   180,    -1,    80,   125,    44,    -1,   180,   125,
      80,    -1,   180,   125,    44,    -1,    44,   125,    80,    -1,
      44,   125,   180,    -1,    80,   125,   180,   125,    44,    -1,
      80,   125,    44,   125,   180,    -1,   180,   125,    80,   125,
      44,    -1,   180,   125,    44,   125,    80,    -1,    44,   125,
      80,   125,   180,    -1,    44,   125,   180,   125,    80,    -1,
     136,   688,    -1,    -1,    10,   468,   419,   428,    -1,     7,
     480,    -1,    55,   340,    -1,   172,   605,   485,    -1,   176,
     501,    -1,   141,   411,    -1,    37,   498,   499,    -1,    48,
     487,   483,    -1,    79,   493,    -1,   265,   494,    -1,    59,
      65,   495,    -1,    43,   468,   391,    -1,    22,   468,   287,
     655,   288,   469,    -1,   484,    -1,   483,   484,    -1,   156,
     468,   481,   469,    -1,     4,    30,   482,    -1,     4,   357,
      -1,    49,    43,    -1,    49,    30,    -1,   174,   653,    -1,
     208,   389,   507,    -1,   486,    -1,   485,   286,   486,    -1,
      49,   653,   492,    -1,    49,    30,   729,    -1,     4,   381,
      -1,     4,   396,    -1,   489,   653,   136,   690,    -1,   489,
     487,   174,   653,    -1,   489,   491,   208,   490,   469,    -1,
     488,    -1,   749,    -1,   202,    -1,   207,    -1,   209,    -1,
     210,    -1,   211,    -1,   212,    -1,   213,    -1,   214,    -1,
     215,    -1,   218,    -1,   219,    -1,   220,    -1,   221,    -1,
     222,    -1,   225,    -1,   226,    -1,   229,    -1,   233,    -1,
     234,    -1,   235,    -1,   236,    -1,   245,    -1,   242,    -1,
     244,    -1,   256,    -1,   257,    -1,   258,    -1,   259,    -1,
     239,    -1,   263,    -1,   270,    -1,   267,    -1,   271,    -1,
     272,    -1,   269,    -1,   273,    -1,   274,    -1,   275,    -1,
     276,    -1,   277,    -1,     7,    -1,     7,   207,    -1,   507,
     468,    -1,   653,   466,    -1,   653,    -1,   205,    -1,   203,
      -1,    -1,   736,     3,    -1,   736,    76,    -1,   735,   266,
     192,   692,    -1,   735,   266,   192,   223,    -1,   735,   266,
     192,   282,   223,    -1,   724,   496,   497,    -1,    52,   686,
      -1,    -1,   111,   686,    -1,    -1,    -1,   500,    -1,   499,
     500,    -1,     4,   348,    -1,     4,   255,    90,   686,    -1,
      49,   255,    90,    -1,    16,   254,    -1,    51,   254,    -1,
     744,   473,   502,   477,   468,   503,   469,    -1,   474,    -1,
      -1,   478,    -1,    -1,    49,   505,    -1,    55,   732,    -1,
      79,   736,    -1,   141,   740,    -1,   172,   743,    -1,   176,
     744,    -1,   187,   747,    -1,    60,   733,    -1,    48,   731,
      -1,    59,    65,   724,    -1,   157,   690,    -1,   206,   741,
      -1,    68,   735,    -1,   265,   735,    -1,   507,    -1,   508,
      -1,   511,    -1,   514,    -1,   512,   290,   509,   291,    -1,
     519,   290,   509,   291,   517,    -1,   510,    -1,   509,   286,
     510,    -1,   692,    -1,   692,   289,   692,    -1,   512,    -1,
     519,   517,    -1,   518,    -1,   523,    -1,   526,    -1,   235,
      -1,   513,    -1,   161,    -1,    38,    -1,   218,    -1,   219,
      -1,    81,    -1,    92,    -1,    18,   516,   515,   517,    -1,
      18,   287,   691,   288,    -1,    18,   287,   691,   286,   687,
     288,    -1,    18,   287,   286,   687,   288,    -1,   154,   160,
     691,    -1,    -1,   169,   687,    -1,   169,   725,    -1,    -1,
      21,   156,   726,    -1,    -1,   522,   287,   690,   288,    -1,
     522,    -1,   522,   185,   287,   690,   288,    -1,   521,   287,
     690,   288,    -1,   521,    -1,   520,   287,   690,   288,    -1,
     183,    -1,    21,   185,    -1,    87,   185,    -1,    21,    -1,
      87,    -1,   115,    -1,   113,    21,    -1,   113,    87,    -1,
      95,   524,    -1,   525,   524,    -1,    -1,   287,   692,   288,
      -1,   287,   692,   286,   692,   288,    -1,    41,    -1,    88,
      -1,    91,   527,    -1,    93,    91,   527,    -1,   144,    -1,
      89,   138,    -1,   287,   688,   288,    -1,    -1,   541,    -1,
     529,    -1,   558,    -1,   156,    68,   735,   174,   692,    -1,
     156,    68,   735,   174,   223,    -1,   156,    68,   735,   174,
     282,   223,    -1,   531,    -1,   532,    -1,   534,    -1,   244,
     748,    -1,   245,   244,   748,   533,    -1,   123,    -1,    -1,
     153,   538,   174,   535,   748,    -1,   244,    -1,    -1,    25,
     538,   539,    -1,   153,   538,    -1,   193,    -1,    -1,   147,
     540,    -1,    -1,   162,    -1,    -1,   156,   175,   542,    -1,
     543,    -1,    -1,   544,    -1,   543,   544,    -1,   545,    -1,
     546,    -1,   547,    -1,   551,    -1,   143,   123,    -1,   143,
     194,    -1,   188,    -1,   117,   188,    -1,    84,   102,   548,
      -1,   548,    -1,   549,    -1,   143,   177,   550,    -1,   143,
      26,   550,    -1,   162,    -1,   162,   172,    -1,   162,   172,
     166,    -1,   186,    -1,   117,   186,    -1,    -1,   146,   554,
      -1,   158,    -1,   142,    -1,    -1,   143,    -1,   194,    -1,
     555,    -1,   554,   286,   555,    -1,   557,   556,    -1,    61,
     552,   553,    -1,    -1,   605,    -1,   557,   286,   605,    -1,
     156,   168,    79,   736,    -1,   269,   122,   560,    83,   564,
      -1,   269,   122,   561,   739,    83,   564,    -1,   269,   122,
     562,   739,   563,    83,   564,    -1,    37,    -1,    48,    -1,
     172,    -1,   187,    -1,   141,    -1,   176,    -1,    59,    65,
      -1,    60,    -1,    55,    -1,    68,    -1,   265,    -1,    79,
      -1,   206,    -1,    21,   156,    -1,   268,    -1,   207,    -1,
     133,    -1,   292,   739,    -1,   686,    -1,    94,    -1,   569,
     566,   568,    -1,    61,   180,   567,    -1,    -1,   121,   648,
      -1,    -1,   192,   243,    -1,    -1,   572,   622,   628,    -1,
     572,   622,   628,    -1,   572,   622,   628,    -1,   573,    -1,
     572,   178,   722,   573,    -1,   572,   178,     6,   573,    -1,
     574,    -1,   155,   579,   582,   583,   587,   612,   608,   611,
     613,    -1,    -1,    -1,    -1,    -1,   580,   581,   576,    -1,
     580,   576,    -1,   581,    -1,    -1,   231,   693,   575,    -1,
     231,   287,   675,   288,   575,    -1,   231,   682,   575,    -1,
     232,   693,    -1,   232,   287,   576,   675,   288,    -1,   232,
     682,    -1,    46,    -1,   721,    -1,   584,    -1,   283,    -1,
     585,    -1,   584,   286,   585,    -1,   675,    -1,   675,   586,
     737,    -1,    10,    -1,    -1,    63,   588,    -1,   589,    -1,
     588,   286,   589,    -1,   595,    -1,   590,    -1,   602,    -1,
     591,    -1,   287,   595,   288,    -1,   287,   569,   288,   586,
     592,   593,    -1,   742,    -1,    -1,   287,   594,   288,    -1,
      -1,   737,    -1,   594,   286,   737,    -1,   596,    -1,   597,
      -1,   598,    -1,   589,   263,    85,   590,    -1,   589,   114,
     606,    85,   590,    -1,   589,   606,    85,   589,   599,    -1,
     600,    -1,   601,    -1,   122,   655,    -1,   239,   287,   648,
     288,    -1,   740,   603,   586,   742,    -1,   740,   603,    -1,
     287,   678,   288,    -1,    -1,   605,    -1,   743,   742,    -1,
     743,    -1,    77,    -1,   100,   607,    -1,   151,   607,    -1,
      64,   607,    -1,    -1,   127,    -1,    -1,    71,    19,   609,
      -1,    -1,   610,    -1,   609,   286,   610,    -1,   675,    -1,
      73,   655,    -1,    -1,   190,   655,    -1,    -1,   135,   614,
      -1,    -1,   615,   287,   616,   288,    -1,    85,    -1,   164,
     108,    -1,   108,    -1,   164,    -1,    -1,   617,    -1,   617,
     286,   616,    -1,   618,   619,    -1,   614,    -1,   743,    -1,
     743,   618,    -1,   114,    -1,    79,   287,   620,   288,    -1,
     126,   736,   621,    -1,   736,    -1,   736,   286,   620,    -1,
      79,   287,   620,   288,    -1,    -1,   126,    19,   623,    -1,
      -1,   624,    -1,   623,   286,   624,    -1,   675,   625,   627,
      -1,    11,    -1,    45,    -1,    -1,   231,    -1,   241,    -1,
     240,   577,   626,   578,    -1,    -1,   259,   675,    -1,   259,
     675,   174,   675,    -1,    -1,    80,    82,   605,   649,   182,
     287,   678,   288,    -1,    80,    82,   605,   649,   569,    -1,
     631,    -1,   632,    -1,    44,    63,   604,   612,   613,   622,
     628,    -1,    44,    63,   604,   636,    -1,   634,    -1,   635,
      -1,   180,   604,   156,   637,   612,   613,   622,   628,    -1,
     180,   604,   156,   637,   636,    -1,   190,    35,   121,   730,
      -1,   638,    -1,   637,   286,   638,    -1,   654,   278,   675,
      -1,   707,    -1,   143,    18,   653,    63,   605,   641,   644,
      -1,    80,    18,   653,    82,   605,   641,   644,    -1,    60,
      63,   642,   174,   642,    -1,    60,   174,   642,    -1,    -1,
     643,    -1,   682,    -1,   687,    -1,   107,   645,    -1,    -1,
     691,    -1,   682,    -1,   647,    -1,    -1,   287,   648,   288,
      -1,   653,    -1,   648,   286,   653,    -1,   650,    -1,    -1,
     287,   651,   288,    -1,   654,    -1,   651,   286,   654,    -1,
     653,    -1,   742,   292,   728,    -1,   742,   292,   283,    -1,
     728,    -1,   653,    -1,   742,   292,   728,    -1,   671,    -1,
     118,   671,    -1,   657,    -1,   655,   125,   655,    -1,   655,
       8,   655,    -1,   657,    -1,   118,   671,    -1,   656,   125,
     655,    -1,   656,     8,   655,    -1,   672,   125,   655,    -1,
     672,     8,   655,    -1,   658,    -1,   287,   656,   288,    -1,
     118,   657,    -1,   659,    -1,   662,    -1,   663,    -1,   664,
      -1,   665,    -1,   670,    -1,   660,    -1,   668,    -1,   666,
      -1,   667,    -1,   669,    -1,   675,   278,   675,    -1,   675,
     279,   675,    -1,   675,   280,   675,    -1,   675,    67,   675,
      -1,   675,   101,   675,    -1,   675,   119,   675,    -1,   675,
     120,   675,    -1,   675,   116,   675,    -1,   675,   278,     6,
     287,   570,   288,    -1,   675,   279,     6,   287,   570,   288,
      -1,   675,   280,     6,   287,   570,   288,    -1,   675,    67,
       6,   287,   570,   288,    -1,   675,   101,     6,   287,   570,
     288,    -1,   675,   119,     6,   287,   570,   288,    -1,   675,
     120,     6,   287,   570,   288,    -1,   675,   116,     6,   287,
     570,   288,    -1,   675,   278,   661,   287,   570,   288,    -1,
     675,   279,   661,   287,   570,   288,    -1,   675,   280,   661,
     287,   570,   288,    -1,   675,    67,   661,   287,   570,   288,
      -1,   675,   101,   661,   287,   570,   288,    -1,   675,   119,
     661,   287,   570,   288,    -1,   675,   120,   661,   287,   570,
     288,    -1,   675,   116,   661,   287,   570,   288,    -1,   163,
      -1,     9,    -1,   675,    83,    46,    63,   675,    -1,   675,
      83,   118,    46,    63,   675,    -1,   675,    17,   675,     8,
     675,    -1,   675,   118,    17,   675,     8,   675,    -1,   675,
     103,   675,    -1,   675,   118,   103,   675,    -1,   675,   103,
     675,    54,   675,    -1,   675,   118,   103,   675,    54,   675,
      -1,   675,    75,   673,    -1,   675,   118,    75,   673,    -1,
     675,    31,   675,    -1,   675,   118,    31,   675,    -1,   675,
     167,   675,    -1,   675,   118,   167,   675,    -1,   675,   167,
     192,   675,    -1,   675,   118,   167,   192,   675,    -1,    57,
     287,   569,   288,    -1,   159,   287,   569,   288,    -1,   675,
      83,    94,    -1,   675,    83,   118,    94,    -1,   248,    -1,
     249,    -1,   250,    -1,   251,    -1,   252,    -1,   253,    -1,
     674,    -1,   287,   678,   288,    -1,   287,   570,   288,    -1,
     652,    -1,   677,    -1,   694,    -1,   681,    -1,   682,    -1,
     442,    -1,   708,    -1,   709,    -1,   719,    -1,   707,    -1,
     282,   675,    -1,   281,   675,    -1,   675,   281,   675,    -1,
     675,    28,   675,    -1,   675,    23,   727,    -1,   675,   282,
     675,    -1,   675,   283,   675,    -1,   675,   284,   675,    -1,
     287,   675,   288,    -1,   287,   571,   288,    -1,   683,    -1,
     684,    -1,   685,    -1,    39,    -1,   742,   292,    39,    -1,
      97,    -1,   676,    -1,   723,    -1,   220,    -1,   221,    -1,
     222,    -1,   652,   290,   678,   291,    -1,   675,    -1,   678,
     286,   675,    -1,   681,    -1,   282,   680,    -1,   197,    -1,
     196,    -1,   195,    -1,   223,    -1,   224,    -1,   680,    -1,
     686,    -1,    38,   199,    -1,   218,   199,    -1,   219,   199,
      -1,   293,    -1,   181,    -1,   225,    -1,   226,    -1,   233,
      -1,   234,    -1,    66,    -1,   165,    -1,   242,    -1,   199,
      -1,   200,   199,    -1,   688,    -1,   282,   689,    -1,   196,
      -1,   196,    -1,   688,    -1,   196,    -1,   693,    -1,   282,
     693,    -1,   196,    -1,   695,    -1,   696,    -1,   702,    -1,
      32,   287,   283,   288,    -1,    32,   287,   721,   675,   288,
      -1,    32,   287,    46,   675,   288,    -1,   171,   287,   721,
     675,   288,    -1,   171,   287,    46,   675,   288,    -1,    13,
     287,   721,   675,   288,    -1,    13,   287,    46,   675,   288,
      -1,   110,   287,   721,   675,   288,    -1,   110,   287,    46,
     675,   288,    -1,   106,   287,   721,   675,   288,    -1,   106,
     287,    46,   675,   288,    -1,   697,    -1,   698,    -1,   209,
     287,   720,    63,   675,   288,    -1,   699,    -1,   700,    -1,
     701,    -1,   270,   287,   675,   288,    -1,   271,   287,   675,
     288,    -1,   272,   287,   675,   288,    -1,   275,   287,   675,
     288,    -1,   703,    -1,   704,    -1,    96,   287,   675,   288,
      -1,   274,   287,   675,   288,    -1,   228,   287,   675,    63,
     675,   706,   288,    -1,   277,   287,   705,   675,    63,   675,
     288,    -1,   277,   287,   675,    63,   675,   288,    -1,   277,
     287,   705,    63,   675,   288,    -1,   277,   287,   675,   288,
      -1,   267,    -1,   276,    -1,   273,    -1,    61,   675,    -1,
      -1,   724,   287,   678,   288,    -1,   724,   287,   288,    -1,
      20,   287,   675,    10,   388,   288,    -1,   710,    -1,   711,
      -1,   237,   287,   675,   286,   675,   288,    -1,   261,   287,
     655,   286,   675,   286,   675,   288,    -1,   238,   287,   675,
     286,   678,   288,    -1,   712,    -1,   714,    -1,   236,   717,
     713,    51,    -1,   236,   717,   713,    50,   718,    51,    -1,
     189,   716,   173,   718,    -1,   713,   189,   716,   173,   718,
      -1,   236,   715,    51,    -1,   236,   715,    50,   718,    51,
      -1,   189,   655,   173,   718,    -1,   715,   189,   655,   173,
     718,    -1,   675,    -1,   675,    -1,   675,    -1,   264,    97,
      61,   735,    -1,    69,   287,   735,   286,   675,   288,    -1,
     210,    -1,   211,    -1,   212,    -1,   213,    -1,   214,    -1,
     215,    -1,   216,    -1,   217,    -1,     6,    -1,    -1,    46,
      -1,    -1,    94,    -1,   198,    -1,   749,    -1,   749,    -1,
     749,    -1,   749,    -1,   749,    -1,   749,    -1,   749,    -1,
     749,    -1,   749,    -1,   749,    -1,   749,    -1,   749,    -1,
     749,    -1,   749,    -1,   749,    -1,   749,    -1,   749,    -1,
     749,    -1,   749,    -1,   749,    -1,   749,    -1,   749,    -1,
     749,    -1,   749,    -1,   198,    -1,   750,    -1,   201,    -1,
     203,    -1,   204,    -1,   205,    -1,   206,    -1,   208,    -1,
     227,    -1,   230,    -1,   228,    -1,   238,    -1,   241,    -1,
     247,    -1,   243,    -1,   237,    -1,   240,    -1,   246,    -1,
     248,    -1,   249,    -1,   250,    -1,   260,    -1,   254,    -1,
     268,    -1,   255,    -1,   261,    -1,   262,    -1,   216,    -1,
     217,    -1,   265,    -1,   264,    -1,   266,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   550,   550,   552,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   575,   576,   584,   588,   592,   597,   598,
     601,   603,   605,   609,   610,   614,   618,   620,   622,   624,
     626,   630,   633,   636,   639,   642,   649,   653,   657,   662,
     665,   668,   671,   674,   675,   678,   679,   681,   683,   687,
     689,   691,   693,   697,   698,   705,   707,   709,   713,   714,
     718,   722,   723,   727,   729,   736,   740,   742,   747,   753,
     754,   756,   763,   764,   765,   769,   770,   776,   782,   783,
     785,   787,   791,   792,   796,   799,   805,   806,   808,   810,
     813,   818,   824,   827,   833,   837,   839,   842,   844,   846,
     848,   850,   852,   854,   856,   858,   860,   869,   873,   875,
     877,   885,   889,   893,   895,   901,   908,   913,   918,   923,
     931,   934,   937,   939,   940,   946,   952,   954,   957,   961,
     962,   967,   968,   973,   974,   977,   978,   985,  1003,  1006,
    1009,  1012,  1016,  1017,  1020,  1021,  1024,  1029,  1030,  1033,
    1037,  1046,  1053,  1106,  1111,  1112,  1115,  1120,  1121,  1124,
    1125,  1129,  1131,  1133,  1135,  1137,  1142,  1143,  1146,  1147,
    1151,  1152,  1154,  1158,  1164,  1168,  1169,  1172,  1173,  1176,
    1178,  1182,  1183,  1184,  1187,  1188,  1189,  1195,  1200,  1205,
    1207,  1210,  1213,  1214,  1218,  1219,  1226,  1230,  1234,  1243,
    1249,  1250,  1253,  1255,  1260,  1263,  1267,  1273,  1279,  1284,
    1289,  1292,  1295,  1296,  1297,  1298,  1299,  1300,  1304,  1305,
    1308,  1309,  1313,  1318,  1320,  1325,  1326,  1328,  1336,  1340,
    1342,  1345,  1346,  1347,  1348,  1351,  1355,  1359,  1366,  1374,
    1378,  1385,  1387,  1389,  1391,  1394,  1397,  1400,  1404,  1407,
    1410,  1413,  1422,  1433,  1443,  1453,  1463,  1466,  1469,  1472,
    1475,  1476,  1480,  1486,  1487,  1491,  1496,  1498,  1500,  1504,
    1507,  1510,  1511,  1515,  1519,  1520,  1523,  1528,  1531,  1534,
    1536,  1540,  1545,  1546,  1549,  1553,  1555,  1558,  1561,  1562,
    1566,  1567,  1568,  1569,  1570,  1571,  1572,  1573,  1574,  1575,
    1576,  1577,  1578,  1579,  1580,  1582,  1584,  1585,  1586,  1587,
    1590,  1592,  1596,  1600,  1604,  1609,  1613,  1617,  1619,  1623,
    1630,  1633,  1638,  1643,  1644,  1645,  1647,  1651,  1655,  1658,
    1661,  1663,  1665,  1670,  1674,  1677,  1678,  1682,  1687,  1688,
    1692,  1694,  1696,  1698,  1702,  1703,  1704,  1707,  1711,  1715,
    1720,  1762,  1767,  1769,  1772,  1775,  1777,  1780,  1785,  1793,
    1796,  1799,  1800,  1804,  1810,  1817,  1841,  1845,  1851,  1855,
    1860,  1865,  1872,  1883,  1894,  1896,  1899,  1902,  1906,  1908,
    1912,  1914,  1916,  1918,  1920,  1922,  1924,  1926,  1928,  1930,
    1932,  1934,  1936,  1938,  1940,  1944,  1947,  1950,  1956,  1960,
    1962,  1969,  1971,  1973,  1976,  1979,  1981,  1983,  1987,  1991,
    1997,  1998,  2002,  2011,  2015,  2017,  2019,  2021,  2023,  2027,
    2028,  2032,  2034,  2036,  2038,  2044,  2047,  2049,  2053,  2061,
    2062,  2063,  2064,  2065,  2066,  2067,  2068,  2069,  2070,  2071,
    2072,  2073,  2074,  2075,  2076,  2077,  2078,  2079,  2080,  2081,
    2082,  2083,  2084,  2085,  2086,  2087,  2088,  2089,  2090,  2091,
    2092,  2093,  2094,  2095,  2096,  2098,  2099,  2100,  2102,  2103,
    2107,  2109,  2113,  2115,  2120,  2126,  2128,  2131,  2134,  2136,
    2140,  2143,  2146,  2151,  2155,  2158,  2161,  2164,  2171,  2174,
    2175,  2179,  2181,  2183,  2185,  2187,  2194,  2204,  2206,  2209,
    2211,  2216,  2220,  2222,  2224,  2226,  2228,  2230,  2232,  2234,
    2236,  2238,  2240,  2242,  2244,  2251,  2252,  2255,  2256,  2259,
    2264,  2271,  2272,  2276,  2285,  2291,  2292,  2295,  2296,  2297,
    2298,  2315,  2320,  2325,  2344,  2361,  2368,  2369,  2376,  2381,
    2388,  2395,  2404,  2409,  2414,  2418,  2423,  2428,  2432,  2439,
    2445,  2451,  2459,  2464,  2469,  2476,  2477,  2478,  2481,  2482,
    2485,  2486,  2487,  2494,  2498,  2510,  2515,  2569,  2631,  2632,
    2639,  2652,  2657,  2662,  2669,  2672,  2678,  2679,  2680,  2684,
    2687,  2690,  2698,  2699,  2700,  2703,  2707,  2711,  2714,  2717,
    2721,  2722,  2725,  2729,  2733,  2734,  2737,  2740,  2743,  2745,
    2748,  2752,  2754,  2757,  2758,  2762,  2763,  2764,  2765,  2768,
    2770,  2774,  2776,  2780,  2782,  2785,  2787,  2789,  2793,  2795,
    2797,  2801,  2803,  2806,  2809,  2813,  2815,  2818,  2821,  2823,
    2827,  2828,  2832,  2836,  2839,  2842,  2843,  2848,  2852,  2854,
    2856,  2860,  2864,  2866,  2868,  2870,  2872,  2874,  2876,  2878,
    2880,  2882,  2884,  2886,  2888,  2890,  2898,  2900,  2904,  2908,
    2909,  2916,  2920,  2923,  2926,  2929,  2932,  2935,  2941,  2945,
    2950,  2955,  2956,  2958,  2962,  2965,  2978,  2982,  2986,  2990,
    2993,  2995,  2997,  3000,  3003,  3005,  3007,  3011,  3013,  3015,
    3019,  3021,  3025,  3027,  3031,  3032,  3036,  3037,  3041,  3042,
    3047,  3051,  3052,  3056,  3057,  3060,  3061,  3062,  3068,  3072,
    3074,  3077,  3080,  3083,  3084,  3088,  3089,  3090,  3093,  3098,
    3103,  3107,  3108,  3111,  3115,  3119,  3122,  3127,  3130,  3133,
    3134,  3139,  3144,  3146,  3148,  3150,  3153,  3156,  3157,  3163,
    3166,  3169,  3170,  3176,  3179,  3182,  3185,  3188,  3194,  3197,
    3200,  3204,  3206,  3208,  3214,  3217,  3220,  3221,  3225,  3227,
    3230,  3231,  3235,  3237,  3239,  3243,  3244,  3248,  3251,  3256,
    3259,  3262,  3263,  3267,  3271,  3273,  3276,  3279,  3281,  3285,
    3288,  3293,  3296,  3305,  3311,  3314,  3321,  3322,  3325,  3329,
    3336,  3337,  3340,  3345,  3351,  3358,  3359,  3363,  3367,  3374,
    3376,  3380,  3382,  3385,  3388,  3389,  3392,  3396,  3399,  3402,
    3404,  3410,  3412,  3415,  3419,  3420,  3425,  3427,  3430,  3434,
    3435,  3440,  3441,  3444,  3449,  3454,  3457,  3464,  3465,  3467,
    3468,  3470,  3474,  3475,  3477,  3479,  3483,  3485,  3489,  3490,
    3492,  3496,  3497,  3498,  3499,  3500,  3501,  3502,  3503,  3504,
    3505,  3506,  3511,  3513,  3515,  3517,  3519,  3521,  3523,  3525,
    3531,  3533,  3535,  3537,  3539,  3541,  3543,  3545,  3547,  3549,
    3551,  3553,  3555,  3557,  3559,  3561,  3565,  3566,  3572,  3574,
    3578,  3580,  3585,  3587,  3589,  3591,  3596,  3598,  3602,  3604,
    3608,  3610,  3612,  3614,  3618,  3622,  3626,  3628,  3632,  3637,
    3642,  3649,  3654,  3659,  3668,  3669,  3673,  3680,  3681,  3682,
    3683,  3684,  3685,  3686,  3687,  3688,  3689,  3690,  3692,  3694,
    3701,  3703,  3705,  3712,  3719,  3726,  3728,  3730,  3731,  3732,
    3733,  3735,  3737,  3741,  3742,  3745,  3761,  3777,  3781,  3785,
    3786,  3790,  3791,  3795,  3797,  3799,  3801,  3803,  3807,  3808,
    3810,  3826,  3842,  3846,  3850,  3852,  3856,  3860,  3863,  3866,
    3869,  3872,  3877,  3879,  3884,  3885,  3889,  3896,  3903,  3910,
    3917,  3918,  3922,  3929,  3930,  3931,  3934,  3936,  3938,  3941,
    3948,  3957,  3964,  3973,  3975,  3977,  3979,  3983,  3984,  3987,
    3991,  3992,  3993,  3996,  4001,  4004,  4009,  4014,  4015,  4016,
    4018,  4022,  4031,  4033,  4036,  4038,  4043,  4045,  4047,  4051,
    4054,  4057,  4059,  4063,  4069,  4070,  4073,  4077,  4080,  4084,
    4085,  4088,  4090,  4094,  4096,  4100,  4102,  4106,  4108,  4112,
    4115,  4118,  4123,  4126,  4136,  4138,  4140,  4142,  4144,  4146,
    4148,  4150,  4154,  4155,  4158,  4159,  4162,  4170,  4173,  4176,
    4179,  4182,  4185,  4188,  4191,  4194,  4197,  4200,  4203,  4206,
    4209,  4212,  4215,  4218,  4221,  4224,  4227,  4230,  4233,  4236,
    4239,  4242,  4247,  4248,  4254,  4255,  4256,  4257,  4258,  4259,
    4260,  4261,  4262,  4263,  4264,  4265,  4266,  4267,  4268,  4269,
    4270,  4271,  4272,  4274,  4276,  4277,  4278,  4279,  4281,  4283,
    4284,  4285,  4286,  4287
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ACTIVE", "ADD", "AFTER", "ALL", "ALTER", 
  "AND", "ANY", "AS", "ASC", "AT", "AVG", "AUTO", "BEFORE", "BEGIN", 
  "BETWEEN", "BLOB", "BY", "CAST", "CHARACTER", "CHECK", "COLLATE", 
  "COMMA", "COMMIT", "COMMITTED", "COMPUTED", "CONCATENATE", 
  "CONDITIONAL", "CONSTRAINT", "CONTAINING", "COUNT", "CREATE", "CSTRING", 
  "CURRENT", "CURSOR", "DATABASE", "DATE", "DB_KEY", "KW_DEBUG", 
  "DECIMAL", "DECLARE", "DEFAULT", "KW_DELETE", "DESC", "DISTINCT", "DO", 
  "DOMAIN", "DROP", "ELSE", "END", "ENTRY_POINT", "EQL", "ESCAPE", 
  "EXCEPTION", "EXECUTE", "EXISTS", "EXIT", "EXTERNAL", "FILTER", "FOR", 
  "FOREIGN", "FROM", "FULL", "FUNCTION", "GDSCODE", "GEQ", "GENERATOR", 
  "GEN_ID", "GRANT", "GROUP", "GTR", "HAVING", "IF", "KW_IN", "INACTIVE", 
  "INNER", "INPUT_TYPE", "INDEX", "INSERT", "INTEGER", "INTO", "IS", 
  "ISOLATION", "JOIN", "KEY", "KW_CHAR", "KW_DEC", "KW_DOUBLE", "KW_FILE", 
  "KW_FLOAT", "KW_INT", "KW_LONG", "KW_NULL", "KW_NUMERIC", "KW_UPPER", 
  "KW_VALUE", "LENGTH", "LPAREN", "LEFT", "LEQ", "LEVEL", "LIKE", "LSS", 
  "MANUAL", "MAXIMUM", "MAX_SEGMENT", "MERGE", "MESSAGE", "MINIMUM", 
  "MODULE_NAME", "NAMES", "NATIONAL", "NATURAL", "NCHAR", "NEQ", "NO", 
  "NOT", "NOT_GTR", "NOT_LSS", "OF", "ON", "ONLY", "OPTION", "OR", 
  "ORDER", "OUTER", "OUTPUT_TYPE", "OVERFLOW", "PAGE", "PAGES", 
  "KW_PAGE_SIZE", "PARAMETER", "PASSWORD", "PLAN", "POSITION", 
  "POST_EVENT", "PRECISION", "PRIMARY", "PRIVILEGES", "PROCEDURE", 
  "PROTECTED", "READ", "REAL", "REFERENCES", "RESERVING", "RETAIN", 
  "RETURNING_VALUES", "RETURNS", "REVOKE", "RIGHT", "RPAREN", "ROLLBACK", 
  "SEGMENT", "SELECT", "SET", "SHADOW", "KW_SHARED", "SINGULAR", 
  "KW_SIZE", "SMALLINT", "SNAPSHOT", "SOME", "SORT", "SQLCODE", 
  "STABILITY", "STARTING", "STATISTICS", "SUB_TYPE", "SUSPEND", "SUM", 
  "TABLE", "THEN", "TO", "TRANSACTION", "TRIGGER", "UNCOMMITTED", "UNION", 
  "UNIQUE", "UPDATE", "USER", "VALUES", "VARCHAR", "VARIABLE", "VARYING", 
  "VERSION", "VIEW", "WAIT", "WHEN", "WHERE", "WHILE", "WITH", "WORK", 
  "WRITE", "FLOAT_NUMBER", "NUMBER", "NUMERIC", "SYMBOL", "STRING", 
  "INTRODUCER", "ACTION", "ADMIN", "CASCADE", "FREE_IT", "RESTRICT", 
  "ROLE", "COLUMN", "TYPE", "EXTRACT", "YEAR", "MONTH", "DAY", "HOUR", 
  "MINUTE", "SECOND", "WEEKDAY", "YEARDAY", "TIME", "TIMESTAMP", 
  "CURRENT_DATE", "CURRENT_TIME", "CURRENT_TIMESTAMP", "NUMBER64BIT", 
  "SCALEDINT", "CURRENT_USER", "CURRENT_ROLE", "KW_BREAK", "SUBSTRING", 
  "RECREATE", "KW_DESCRIPTOR", "FIRST", "SKIP", "CURRENT_CONNECTION", 
  "CURRENT_TRANSACTION", "BIGINT", "CASE", "NULLIF", "COALESCE", "USING", 
  "NULLS", "LAST", "ROW_COUNT", "LOCK", "SAVEPOINT", "RELEASE", 
  "STATEMENT", "LEAVE", "INSERTING", "UPDATING", "DELETING", 
  "KW_INSERTING", "KW_UPDATING", "KW_DELETING", "BACKUP", "KW_DIFFERENCE", 
  "OPEN", "CLOSE", "FETCH", "ROWS", "BLOCK", "IIF", "SCALAR_ARRAY", 
  "CROSS", "NEXT", "SEQUENCE", "RESTART", "BOTH", "COLLATION", "COMMENT", 
  "BIT_LENGTH", "CHAR_LENGTH", "CHARACTER_LENGTH", "LEADING", "KW_LOWER", 
  "OCTET_LENGTH", "TRAILING", "TRIM", "'='", "'<'", "'>'", "'+'", "'-'", 
  "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", "']'", "'.'", 
  "'?'", "$accept", "top", "statement", "grant", "table_noise", 
  "privileges", "privilege_list", "proc_privileges", "privilege", 
  "grant_option", "role_admin_option", "simple_proc_name", "revoke", 
  "rev_grant_option", "rev_admin_option", "non_role_grantee_list", 
  "grantee_list", "grantee", "user_grantee_list", "user_grantee", 
  "role_name_list", "role_name", "role_grantee_list", "role_grantee", 
  "declare", "declare_clause", "udf_decl_clause", "udf_data_type", 
  "arg_desc_list1", "arg_desc_list", "arg_desc", "param_mechanism", 
  "return_value1", "return_value", "return_mechanism", 
  "filter_decl_clause", "blob_filter_subtype", "create", "create_clause", 
  "recreate", "recreate_clause", "replace", "replace_clause", 
  "exception_clause", "rexception_clause", "replace_exception_clause", 
  "alter_exception_clause", "unique_opt", "index_definition", 
  "shadow_clause", "manual_auto", "conditional", "first_file_length", 
  "sec_shadow_files", "db_file_list", "domain_clause", "as_opt", 
  "domain_default_opt", "domain_constraint_clause", 
  "domain_constraint_list", "domain_constraint_def", "domain_constraint", 
  "null_constraint", "domain_check_constraint", "generator_clause", 
  "role_clause", "db_clause", "equals", "db_name", "db_initial_desc1", 
  "db_initial_desc", "db_initial_option", "db_rem_desc1", "db_rem_desc", 
  "db_rem_option", "db_file", "file1", "file_desc1", "file_desc", 
  "file_clause", "file_clause_noise", "page_noise", "table_clause", 
  "rtable_clause", "external_file", "table_elements", "table_element", 
  "column_def", "def_computed", "computed_by", "data_type_or_domain", 
  "collate_clause", "column_def_name", "simple_column_def_name", 
  "data_type_descriptor", "init_data_type", "default_opt", 
  "default_value", "column_constraint_clause", "column_constraint_list", 
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
  "proc_statement", "excp_statement", "raise_statement", "exec_sql", 
  "for_select", "for_exec_into", "exec_into", "if_then_else", 
  "post_event", "event_argument_opt", "singleton_select", "variable", 
  "variable_list", "while", "label_opt", "breakleave", "cursor_def", 
  "excp_hndl_statements", "excp_hndl_statement", "errors", "err", 
  "cursor_statement", "open_cursor", "close_cursor", "fetch_cursor", 
  "fetch_opt", "exec_procedure", "proc_inputs", "proc_outputs_opt", 
  "exec_block", "block_input_params", "block_parameters", 
  "block_parameter", "view_clause", "rview_clause", "begin_string", 
  "end_string", "begin_trigger", "end_trigger", "check_opt", 
  "def_trigger_clause", "replace_trigger_clause", "trigger_active", 
  "trigger_type", "trigger_type_prefix", "trigger_type_suffix", 
  "trigger_position", "trigger_action", "alter", "alter_clause", 
  "domain_default_opt2", "domain_check_constraint2", "alter_domain_ops", 
  "alter_domain_op", "alter_ops", "alter_op", "alter_column_name", 
  "keyword_or_column", "col_opt", "alter_data_type_or_domain", 
  "alter_col_name", "drop_behaviour", "alter_index_clause", 
  "alter_sequence_clause", "alter_udf_clause", "entry_op", "module_op", 
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
  "restr_option", "table_lock", "table_list", "set_statistics", "comment", 
  "ddl_type0", "ddl_type1", "ddl_type2", "ddl_subname", "ddl_desc", 
  "select", "for_update_clause", "for_update_list", "lock_clause", 
  "select_expr", "column_select", "column_singleton", "select_expr_body", 
  "query_term", "query_spec", "begin_limit", "end_limit", "begin_first", 
  "end_first", "limit_clause", "first_clause", "skip_clause", 
  "distinct_clause", "select_list", "select_items", "select_item", 
  "as_noise", "from_clause", "from_list", "table_reference", 
  "table_primary", "derived_table", "correlation_name", 
  "derived_column_list", "alias_list", "joined_table", "cross_join", 
  "natural_join", "qualified_join", "join_specification", 
  "join_condition", "named_columns_join", "table_proc", 
  "table_proc_inputs", "table_name", "simple_table_name", "join_type", 
  "outer_noise", "group_clause", "group_by_list", "group_by_item", 
  "having_clause", "where_clause", "plan_clause", "plan_expression", 
  "plan_type", "plan_item_list", "plan_item", "table_or_alias_list", 
  "access_type", "index_list", "extra_indices_opt", "order_clause", 
  "order_list", "order_item", "order_direction", "nulls_placement", 
  "nulls_clause", "rows_clause", "insert", "delete", "delete_searched", 
  "delete_positioned", "update", "update_searched", "update_positioned", 
  "cursor_clause", "assignments", "assignment", "exec_udf", "blob_io", 
  "filter_clause_io", "blob_subtype_value_io", "blob_subtype_io", 
  "segment_clause_io", "segment_length_io", "column_parens_opt", 
  "column_parens", "column_list", "ins_column_parens_opt", 
  "ins_column_parens", "ins_column_list", "column_name", 
  "simple_column_name", "update_column_name", "search_condition", 
  "bracable_search_condition", "simple_search_condition", "predicate", 
  "comparison_predicate", "quantified_predicate", "some", 
  "distinct_predicate", "between_predicate", "like_predicate", 
  "in_predicate", "containing_predicate", "starting_predicate", 
  "exists_predicate", "singular_predicate", "null_predicate", 
  "trigger_action_predicate", "special_trigger_action_predicate", 
  "in_predicate_value", "table_subquery", "value", 
  "datetime_value_expression", "array_element", "value_list", "constant", 
  "u_numeric_constant", "u_constant", "parameter", "current_user", 
  "current_role", "internal_info", "sql_string", "signed_short_integer", 
  "nonneg_short_integer", "neg_short_integer", "pos_short_integer", 
  "unsigned_short_integer", "signed_long_integer", "long_integer", 
  "function", "aggregate_function", "numeric_value_function", 
  "extract_expression", "length_expression", "bit_length_expression", 
  "char_length_expression", "octet_length_expression", 
  "string_value_function", "substring_function", "trim_function", 
  "trim_specification", "string_length_opt", "udf", "cast_specification", 
  "case_expression", "case_abbreviation", "case_specification", 
  "simple_case", "simple_when_clause", "searched_case", 
  "searched_when_clause", "when_operand", "case_operand", "case_result", 
  "next_value_expression", "timestamp_part", "all_noise", 
  "distinct_noise", "null_value", "symbol_UDF_name", 
  "symbol_blob_subtype_name", "symbol_character_set_name", 
  "symbol_collation_name", "symbol_column_name", "symbol_constraint_name", 
  "symbol_cursor_name", "symbol_domain_name", "symbol_exception_name", 
  "symbol_filter_name", "symbol_gdscode_name", "symbol_generator_name", 
  "symbol_index_name", "symbol_item_alias_name", "symbol_label_name", 
  "symbol_ddl_name", "symbol_procedure_name", "symbol_role_name", 
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
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,    61,    60,
      62,    43,    45,    42,    47,    59,    44,    40,    41,    58,
      91,    93,    46,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   294,   295,   295,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   297,   297,   297,   298,   298,
     299,   299,   299,   300,   300,   301,   302,   302,   302,   302,
     302,   303,   303,   304,   304,   305,   306,   306,   306,   307,
     307,   308,   308,   309,   309,   310,   310,   310,   310,   311,
     311,   311,   311,   312,   312,   313,   313,   313,   314,   314,
     315,   316,   316,   317,   317,   318,   319,   319,   320,   321,
     321,   321,   322,   322,   322,   323,   323,   324,   325,   325,
     325,   325,   326,   326,   327,   327,   328,   328,   328,   328,
     328,   329,   330,   330,   331,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   333,   334,   334,
     334,   334,   335,   336,   336,   336,   337,   338,   339,   340,
     341,   341,   342,   342,   342,   343,   344,   344,   344,   345,
     345,   346,   346,   347,   347,   348,   348,   349,   350,   350,
     351,   351,   352,   352,   353,   353,   354,   355,   355,   356,
     357,   358,   359,   360,   361,   361,   362,   363,   363,   364,
     364,   365,   365,   365,   365,   365,   366,   366,   367,   367,
     368,   368,   368,   369,   370,   371,   371,   372,   372,   373,
     373,   374,   374,   374,   375,   375,   375,   376,   377,   378,
     378,   378,   379,   379,   380,   380,   381,   381,   381,   382,
     383,   383,   384,   384,   385,   385,   386,   387,   388,   389,
     390,   390,   391,   391,   391,   391,   391,   391,   392,   392,
     393,   393,   394,   395,   395,   395,   395,   395,   396,   397,
     397,   398,   398,   398,   398,   399,   400,   401,   402,   402,
     403,   404,   404,   404,   404,   404,   405,   406,   407,   407,
     407,   407,   408,   409,   410,   411,   412,   412,   413,   413,
     414,   414,   415,   416,   416,   417,   418,   418,   418,   419,
     419,   420,   420,   421,   422,   422,   423,   424,   424,   425,
     425,   426,   427,   427,   428,   429,   429,   429,   430,   430,
     431,   431,   431,   431,   431,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   431,   431,   431,   431,   431,
     432,   432,   433,   434,   435,   436,   437,   438,   438,   439,
     440,   441,   442,   443,   443,   443,   443,   444,   445,   445,
     446,   446,   446,   447,   447,   448,   448,   449,   450,   450,
     451,   451,   451,   451,   452,   452,   452,   453,   454,   455,
     456,   457,   458,   458,   458,   459,   459,   459,   460,   461,
     461,   462,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   470,   471,   472,   473,   473,   473,   474,   475,   475,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   477,   477,   478,   479,   480,
     480,   480,   480,   480,   480,   480,   480,   480,   481,   482,
     483,   483,   484,   484,   484,   484,   484,   484,   484,   485,
     485,   486,   486,   486,   486,   486,   486,   486,   487,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     489,   489,   490,   490,   491,   492,   492,   492,   493,   493,
     494,   494,   494,   495,   496,   496,   497,   497,   498,   499,
     499,   500,   500,   500,   500,   500,   501,   502,   502,   503,
     503,   504,   505,   505,   505,   505,   505,   505,   505,   505,
     505,   505,   505,   505,   505,   506,   506,   507,   507,   508,
     508,   509,   509,   510,   510,   511,   511,   512,   512,   512,
     512,   512,   512,   512,   512,   512,   513,   513,   514,   514,
     514,   514,   515,   515,   516,   516,   516,   517,   517,   518,
     518,   518,   519,   519,   519,   520,   520,   520,   521,   521,
     522,   522,   522,   523,   523,   524,   524,   524,   525,   525,
     526,   526,   526,   526,   527,   527,   528,   528,   528,   529,
     529,   529,   530,   530,   530,   531,   532,   533,   533,   534,
     535,   535,   536,   537,   538,   538,   539,   539,   540,   540,
     541,   542,   542,   543,   543,   544,   544,   544,   544,   545,
     545,   546,   546,   547,   547,   548,   548,   548,   549,   549,
     549,   550,   550,   550,   551,   552,   552,   552,   553,   553,
     554,   554,   555,   556,   556,   557,   557,   558,   559,   559,
     559,   560,   561,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   562,   562,   563,   564,
     564,   565,   566,   566,   567,   567,   568,   568,   569,   570,
     571,   572,   572,   572,   573,   574,   575,   576,   577,   578,
     579,   579,   579,   579,   580,   580,   580,   581,   581,   581,
     582,   582,   583,   583,   584,   584,   585,   585,   586,   586,
     587,   588,   588,   589,   589,   590,   590,   590,   591,   592,
     592,   593,   593,   594,   594,   595,   595,   595,   596,   597,
     598,   599,   599,   600,   601,   602,   602,   603,   603,   604,
     604,   605,   606,   606,   606,   606,   606,   607,   607,   608,
     608,   609,   609,   610,   611,   611,   612,   612,   613,   613,
     614,   615,   615,   615,   615,   615,   616,   616,   617,   617,
     618,   618,   619,   619,   619,   620,   620,   621,   621,   622,
     622,   623,   623,   624,   625,   625,   625,   626,   626,   627,
     627,   628,   628,   628,   629,   629,   630,   630,   631,   632,
     633,   633,   634,   635,   636,   637,   637,   638,   639,   640,
     640,   641,   641,   641,   642,   642,   643,   644,   644,   645,
     645,   646,   646,   647,   648,   648,   649,   649,   650,   651,
     651,   652,   652,   652,   653,   654,   654,   655,   655,   655,
     655,   655,   656,   656,   656,   656,   656,   656,   657,   657,
     657,   658,   658,   658,   658,   658,   658,   658,   658,   658,
     658,   658,   659,   659,   659,   659,   659,   659,   659,   659,
     660,   660,   660,   660,   660,   660,   660,   660,   660,   660,
     660,   660,   660,   660,   660,   660,   661,   661,   662,   662,
     663,   663,   664,   664,   664,   664,   665,   665,   666,   666,
     667,   667,   667,   667,   668,   669,   670,   670,   671,   671,
     671,   672,   672,   672,   673,   673,   674,   675,   675,   675,
     675,   675,   675,   675,   675,   675,   675,   675,   675,   675,
     675,   675,   675,   675,   675,   675,   675,   675,   675,   675,
     675,   675,   675,   675,   675,   676,   676,   676,   677,   678,
     678,   679,   679,   680,   680,   680,   680,   680,   681,   681,
     681,   681,   681,   682,   683,   683,   684,   685,   685,   685,
     685,   685,   686,   686,   687,   687,   688,   689,   690,   691,
     692,   692,   693,   694,   694,   694,   695,   695,   695,   695,
     695,   695,   695,   695,   695,   695,   695,   696,   696,   697,
     698,   698,   698,   699,   700,   700,   701,   702,   702,   702,
     702,   703,   704,   704,   704,   704,   705,   705,   705,   706,
     706,   707,   707,   708,   709,   709,   710,   710,   710,   711,
     711,   712,   712,   713,   713,   714,   714,   715,   715,   716,
     717,   718,   719,   719,   720,   720,   720,   720,   720,   720,
     720,   720,   721,   721,   722,   722,   723,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   749,   749,   750,   750,   750,   750,   750,   750,
     750,   750,   750,   750,   750,   750,   750,   750,   750,   750,
     750,   750,   750,   750,   750,   750,   750,   750,   750,   750,
     750,   750,   750,   750
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     8,     8,     5,     1,     0,
       1,     2,     1,     1,     3,     1,     1,     1,     1,     2,
       2,     3,     0,     3,     0,     1,     8,     8,     5,     3,
       0,     3,     0,     1,     1,     1,     3,     3,     3,     2,
       2,     2,     2,     1,     3,     1,     2,     2,     1,     3,
       1,     1,     3,     1,     2,     2,     2,     3,     8,     1,
       1,     5,     0,     1,     3,     1,     3,     3,     0,     2,
       2,     1,     1,     3,     3,     2,     0,     2,     2,     1,
       3,     9,     1,     1,     2,     2,     7,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     4,     2,     2,     2,     2,     2,     2,     2,
       1,     0,     1,     1,     6,     6,     1,     1,     0,     0,
       1,     0,     4,     0,     1,     1,     2,     8,     1,     0,
       3,     0,     0,     1,     1,     2,     1,     1,     1,     2,
       6,     1,     1,     3,     0,     1,     1,     0,     1,     1,
       2,     3,     4,     2,     2,     3,     0,     1,     1,     2,
       1,     4,     3,     3,     1,     0,     1,     1,     2,     3,
       4,     0,     1,     2,     0,     1,     1,     5,     5,     3,
       2,     0,     1,     3,     1,     1,     6,     3,     2,     6,
       2,     1,     2,     2,     2,     0,     1,     1,     2,     0,
       2,     0,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     2,     2,     2,     5,     1,     2,     3,     2,     2,
       0,     1,     1,     1,     1,     3,     4,     8,     4,     0,
       6,     1,     1,     2,     2,     0,     3,     3,     1,     2,
       2,     2,     8,     8,     8,     8,     3,     0,     4,     0,
       1,     3,     4,     1,     3,     2,     3,     3,     1,     1,
       0,     1,     2,     4,     1,     1,     3,     1,     0,     2,
       1,     6,     1,     1,     3,     1,     2,     0,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       2,     3,     1,     3,     8,     9,     5,     8,     6,     3,
       0,     3,     2,     1,     1,     3,     3,     7,     2,     0,
       1,     1,     2,     3,     0,     1,     2,     4,     1,     3,
       2,     2,     2,     1,     1,     1,     1,     2,     2,     5,
       0,     5,     1,     3,     0,     2,     4,     0,     7,     3,
       0,     1,     3,     3,     7,     7,     0,     0,     0,     0,
       3,     0,     9,     9,     1,     1,     0,     2,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     5,
       5,     5,     5,     5,     5,     2,     0,     4,     2,     2,
       3,     2,     2,     3,     3,     2,     2,     3,     3,     6,
       1,     2,     4,     3,     2,     2,     2,     2,     3,     1,
       3,     3,     3,     2,     2,     4,     4,     5,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     1,     1,     1,     0,     2,     2,
       4,     4,     5,     3,     2,     0,     2,     0,     0,     1,
       2,     2,     4,     3,     2,     2,     7,     1,     0,     1,
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       3,     2,     2,     2,     2,     1,     1,     1,     1,     4,
       5,     1,     3,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       6,     5,     3,     0,     2,     2,     0,     3,     0,     4,
       1,     5,     4,     1,     4,     1,     2,     2,     1,     1,
       1,     2,     2,     2,     2,     0,     3,     5,     1,     1,
       2,     3,     1,     2,     3,     0,     1,     1,     1,     5,
       5,     6,     1,     1,     1,     2,     4,     1,     0,     5,
       1,     0,     3,     2,     1,     0,     2,     0,     1,     0,
       3,     1,     0,     1,     2,     1,     1,     1,     1,     2,
       2,     1,     2,     3,     1,     1,     3,     3,     1,     2,
       3,     1,     2,     0,     2,     1,     1,     0,     1,     1,
       1,     3,     2,     3,     0,     1,     3,     4,     5,     6,
       7,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     2,     1,
       1,     3,     3,     0,     2,     0,     2,     0,     3,     3,
       3,     1,     4,     4,     1,     9,     0,     0,     0,     0,
       3,     2,     1,     0,     3,     5,     3,     2,     5,     2,
       1,     1,     1,     1,     1,     3,     1,     3,     1,     0,
       2,     1,     3,     1,     1,     1,     1,     3,     6,     1,
       0,     3,     0,     1,     3,     1,     1,     1,     4,     5,
       5,     1,     1,     2,     4,     4,     2,     3,     0,     1,
       2,     1,     1,     2,     2,     2,     0,     1,     0,     3,
       0,     1,     3,     1,     2,     0,     2,     0,     2,     0,
       4,     1,     2,     1,     1,     0,     1,     3,     2,     1,
       1,     2,     1,     4,     3,     1,     3,     4,     0,     3,
       0,     1,     3,     3,     1,     1,     0,     1,     1,     4,
       0,     2,     4,     0,     8,     5,     1,     1,     7,     4,
       1,     1,     8,     5,     4,     1,     3,     3,     1,     7,
       7,     5,     3,     0,     1,     1,     1,     2,     0,     1,
       1,     1,     0,     3,     1,     3,     1,     0,     3,     1,
       3,     1,     3,     3,     1,     1,     3,     1,     2,     1,
       3,     3,     1,     2,     3,     3,     3,     3,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     1,     1,     5,     6,
       5,     6,     3,     4,     5,     6,     3,     4,     3,     4,
       3,     4,     4,     5,     4,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     4,     1,
       3,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     4,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     1,     1,     6,
       1,     1,     1,     4,     4,     4,     4,     1,     1,     4,
       4,     7,     7,     6,     6,     4,     1,     1,     1,     2,
       0,     4,     3,     6,     1,     1,     6,     8,     6,     1,
       1,     4,     6,     4,     5,     3,     5,     4,     5,     1,
       1,     1,     4,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
       0,     0,   605,   131,     0,     0,     0,     0,     0,     0,
       0,     0,    52,   605,   693,     0,     0,     0,     0,     0,
       0,     0,     2,    12,    18,     9,     8,    16,    17,    14,
      15,     4,    11,    22,   587,    20,   592,   593,   594,     7,
      19,   586,   588,     6,    21,   673,   780,   681,   684,    13,
      10,   796,   797,    23,   800,   801,     5,   498,     0,     0,
       0,     0,     0,     0,     0,     0,   408,   604,   607,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   130,     0,
       0,     0,   104,   786,   976,     0,    24,   974,     0,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   511,     0,   370,    30,    38,
      35,    37,   822,    36,   822,  1082,  1084,  1085,  1086,  1087,
    1088,  1089,  1109,  1110,  1090,  1092,  1091,  1097,  1093,  1098,
    1094,  1096,  1099,  1095,  1100,  1101,  1102,  1104,  1106,  1103,
    1107,  1108,  1112,  1111,  1113,  1105,     0,    32,     0,    33,
       0,    68,    70,  1074,  1083,     0,     0,     0,     0,     0,
       0,     0,   603,     0,     0,  1053,   687,   692,     0,     0,
     612,     0,   739,   741,  1076,     0,     0,     0,     0,   117,
     595,  1081,     0,     0,     1,     3,     0,   677,     0,  1055,
     793,     0,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   468,   462,   463,   461,   464,   465,   466,
     467,   469,   471,   474,   470,   472,   473,   475,   476,   477,
     478,   479,     0,   438,   439,   409,     0,  1065,     0,   415,
       0,  1069,   412,   267,  1073,     0,   741,   411,   386,  1077,
     416,     0,  1068,   609,   602,   972,     0,   113,   167,   166,
     114,   149,   216,   834,  1061,   105,     0,   111,   161,     0,
     107,   267,   115,   978,   138,   108,   201,   109,     0,   110,
     822,  1080,   116,   162,   112,   784,   785,     0,   977,   975,
       0,    76,     0,  1066,   757,   519,  1064,   512,     0,   518,
     523,   513,   514,   521,   515,   516,   517,   522,   524,   364,
       0,   269,    31,     0,    40,   821,    39,    29,     0,     0,
       0,     0,     0,   827,     0,     0,     0,     0,     0,     0,
     601,   982,     0,   963,   686,   686,   687,   699,   697,  1052,
     700,     0,   701,   691,   687,     0,     0,     0,     0,     0,
       0,   628,   621,   610,   611,   613,   615,   616,   617,   624,
     625,   618,     0,   740,  1075,   121,     0,   118,   267,   119,
     201,   120,   822,   598,     0,   651,   652,   659,     0,   658,
     660,   662,   667,   655,   653,   656,   654,   663,   666,   661,
     665,     0,     0,     0,   675,     0,   671,     0,     0,  1054,
       0,     0,   678,     0,     0,     0,     0,   413,   499,   378,
       0,   378,     0,   219,   414,   420,   129,  1057,   417,   495,
     488,   489,     0,   269,   240,   480,     0,   410,   429,     0,
     384,   385,   508,     0,   608,   606,   973,   164,   164,     0,
       0,     0,   176,   168,   169,   148,     0,   126,     0,     0,
       0,   122,   269,   137,   136,   139,     0,     0,     0,     0,
       0,    77,   219,     0,     0,   759,   799,   520,     0,     0,
       0,     0,   940,   969,     0,  1056,     0,   942,     0,     0,
     970,     0,   964,   955,   954,   953,  1082,     0,     0,     0,
     945,   946,   947,   956,   957,   965,   966,  1092,   967,   968,
       0,  1097,  1093,   971,  1107,  1112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   922,   367,   917,   831,
     949,   943,   918,   362,   958,   920,   921,   937,   938,   939,
     959,   919,   983,   984,   997,   998,  1000,  1001,  1002,   985,
    1007,  1008,   926,   923,   924,  1024,  1025,  1029,  1030,   925,
     944,     0,     0,  1061,     0,     0,     0,   371,   217,     0,
       0,     0,   824,    28,     0,    34,     0,     0,    44,    71,
      73,  1078,    69,     0,     0,     0,   826,     0,    49,    51,
      29,     0,     0,   600,     0,     0,     0,   696,   694,     0,
     703,     0,   702,   704,   709,   690,     0,   647,     0,   622,
     633,   619,   633,   620,   634,   640,   644,   645,   629,   614,
     757,   805,   835,     0,     0,   127,   269,     0,     0,   597,
     596,   664,   657,     0,     0,  1072,     0,     0,   672,   676,
     779,   781,   786,   683,   682,   791,   184,     0,   501,   145,
       0,   504,     0,   505,   500,     0,   424,     0,   426,   425,
       0,   427,     0,   421,     0,   497,     0,     0,   270,     0,
       0,   433,     0,   434,   378,   481,     0,   487,     0,     0,
       0,   484,  1061,   389,   388,   507,     0,   406,     0,   165,
       0,     0,   174,     0,   173,     0,     0,   163,   177,   178,
     180,   170,   556,   568,   543,   578,   546,   569,   579,     0,
     585,   547,     0,   575,     0,   570,   582,   542,   565,   544,
     545,   540,   378,   525,   526,   527,   535,   541,   528,   537,
     558,     0,   563,   560,   538,   575,   539,   125,     0,   123,
     267,   124,     0,     0,   140,     0,     0,   200,   240,   386,
     376,     0,   219,     0,    83,    85,     0,     0,   103,   102,
    1058,     0,     0,     0,     0,   908,   909,   910,     0,   756,
     839,   848,   851,   857,   852,   853,   854,   855,   859,   860,
     858,   861,   856,   837,     0,   765,   780,  1053,     0,  1053,
     960,     0,     0,  1053,  1053,  1053,     0,   961,   962,     0,
       0,  1040,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   928,   927,     0,   780,   949,     0,
     332,  1079,     0,   361,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   275,   535,   558,     0,     0,   369,
       0,   280,     0,   823,     0,     0,    45,    74,     0,     0,
      27,   813,     0,   829,     0,   795,   813,     0,     0,    48,
     599,     0,   686,     0,     0,   757,     0,   708,     0,   590,
       0,   589,   980,     0,   623,     0,   631,   627,   626,     0,
     637,     0,   642,   630,     0,   759,   803,     0,     0,     0,
     240,   376,   670,   648,   669,     0,     0,     0,   674,     0,
     790,     0,     0,   146,   185,   503,   378,   423,     0,   378,
     379,   428,   494,     0,   493,   376,     0,   266,   376,   239,
    1062,   211,   208,     0,   151,   378,   525,   376,     0,     0,
       0,   238,   241,   242,   243,   244,     0,   432,   486,   485,
     431,   430,     0,     0,     0,   392,   390,   391,   387,     0,
     378,   491,     0,   490,   194,   171,   175,     0,     0,   179,
       0,     0,   553,   566,   567,   583,     0,   580,   585,     0,
     573,   571,   572,   151,     0,     0,     0,   536,     0,     0,
       0,     0,   574,   128,   269,     0,   376,   141,   199,     0,
     202,   204,   205,     0,     0,     0,     0,   219,   219,    80,
       0,    88,    79,     0,     0,     0,     0,   850,   838,     0,
       0,   911,   912,   913,     0,   842,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   761,   763,   764,   758,     0,
     793,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1044,  1045,  1046,  1047,  1048,
    1049,  1050,  1051,     0,     0,     0,     0,  1035,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1016,  1018,  1017,     0,     0,   936,   793,   935,   363,     0,
     333,   365,   334,     0,     0,   931,  1060,   930,   929,   932,
     933,   934,   950,  1022,     0,   941,   833,   832,   373,   372,
       0,   273,   288,     0,   279,   281,   825,     0,     0,     0,
      72,     0,   818,     0,   828,     0,   818,     0,     0,   695,
     698,     0,   710,   711,   714,   716,   713,   725,   726,   727,
     715,   738,     0,   750,   705,   707,  1070,   591,   981,   632,
     641,   636,   635,     0,   646,   806,   780,   807,   836,   376,
       0,     0,   649,   668,     0,   782,   688,   783,   792,   502,
     164,   191,   183,   186,   187,     0,     0,     0,   422,   496,
     376,   376,   379,   278,   271,   280,   210,   378,   378,   379,
     212,   207,   213,     0,     0,   249,     0,   436,   379,   378,
     376,   435,     0,     0,     0,   405,   510,   492,   195,   196,
     172,     0,   182,   554,   555,   979,     0,     0,     0,   558,
       0,   581,     0,     0,   379,     0,   531,   533,     0,     0,
       0,     0,     0,     0,     0,   386,   280,   164,   143,   240,
     197,   406,   381,     0,    84,     0,   219,     0,    92,     0,
      86,     0,     0,    91,    87,     0,   804,  1063,     0,     0,
     843,     0,     0,   849,     0,     0,   841,   840,     0,   898,
       0,   887,   886,     0,   865,     0,   896,   914,     0,   906,
       0,     0,     0,   866,   892,     0,     0,   869,     0,     0,
       0,     0,     0,     0,     0,   867,     0,     0,   868,     0,
     900,     0,     0,   862,     0,     0,   863,     0,     0,   864,
     762,   765,   798,     0,     0,   219,     0,   986,     0,     0,
    1009,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1041,     0,     0,  1039,     0,     0,  1031,     0,     0,     0,
       0,  1042,  1003,  1004,  1005,  1010,  1006,     0,  1015,     0,
       0,   680,     0,     0,     0,   948,  1021,     0,   268,   287,
       0,   339,   368,   282,     0,     0,     0,     0,     0,  1088,
      42,    53,    55,    54,    63,    65,    42,    43,     0,     0,
       0,   810,   830,     0,   809,     0,     0,     0,   746,   713,
       0,   748,   742,   748,   746,   748,     0,     0,     0,   709,
       0,   755,   638,   639,   643,   793,   280,   198,   381,   650,
       0,     0,   192,     0,   188,     0,     0,     0,   418,   227,
     222,   951,   223,   224,   225,   226,     0,     0,   272,     0,
       0,     0,   228,     0,   249,   786,   245,     0,   437,   482,
     483,   397,   398,   394,   393,   396,   395,   378,   509,   379,
     181,  1059,     0,     0,   549,     0,   548,   584,     0,   576,
     152,     0,   529,     0,   557,   558,   564,   562,     0,   559,
     376,     0,     0,     0,   135,   144,   203,   378,     0,   377,
     106,     0,   133,   132,    95,     0,     0,    96,     0,    89,
      90,     0,   904,   905,   845,   844,   847,   846,     0,     0,
       0,     0,   780,     0,     0,     0,   907,     0,     0,     0,
       0,     0,     0,   899,   897,   893,     0,   901,     0,     0,
       0,     0,   902,     0,     0,     0,     0,     0,     0,   769,
       0,   766,     0,   770,   992,   991,     0,     0,   988,   987,
       0,   996,   995,   994,   993,   990,   989,     0,  1020,  1037,
    1036,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   366,   336,   335,   274,     0,     0,   284,   285,     0,
    1061,   322,     0,     0,     0,     0,     0,     0,  1090,  1095,
       0,     0,   360,   298,   293,     0,   339,   292,     0,     0,
       0,   318,   319,     0,   316,     0,     0,   317,     0,     0,
       0,   354,   356,   355,     0,     0,     0,     0,     0,     0,
       0,   808,     0,  1061,    67,    59,    60,    66,    61,    62,
       0,    25,     0,     0,    26,     0,   814,   815,   816,   812,
     817,   820,   819,   794,    46,    47,   709,   717,   712,   747,
     745,   743,     0,   744,     0,     0,     0,     0,     0,     0,
     759,   802,     0,   377,   787,   788,   689,   194,   193,   189,
       0,   379,   952,   276,   277,   379,   379,   150,   215,   229,
     230,   378,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,   280,   506,   551,     0,   552,     0,     0,   215,
     153,   154,   156,   157,   158,   532,   534,   530,   561,   280,
     406,   379,   194,     0,     0,   374,   378,    93,     0,     0,
      99,    94,   558,     0,   890,     0,     0,   916,   793,   915,
     888,     0,     0,     0,   894,     0,     0,     0,     0,   903,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     760,   765,     0,   772,     0,   768,   771,  1023,   218,  1043,
     999,     0,     0,  1038,  1033,  1032,     0,  1026,  1028,     0,
    1013,  1014,     0,   221,   283,     0,   320,     0,   315,     0,
     330,   314,   342,  1071,   357,   358,     0,   294,     0,   299,
     296,   345,   309,   310,   306,   307,   311,   304,     0,     0,
     313,   312,   305,     0,   301,   303,   302,   300,   308,   338,
       0,    56,    57,    58,    64,     0,   720,     0,   728,   746,
     737,   735,   749,   751,   753,   754,   685,   379,   375,   789,
     190,   379,   160,   265,     0,     0,   206,   231,     0,     0,
       0,   249,   232,   235,   822,     0,   379,   403,   404,   400,
     399,   402,   401,     0,   550,   577,   159,   147,   155,     0,
     378,   262,   142,   379,   380,     0,     0,    97,    98,    81,
       0,   873,   881,   679,   889,   874,   882,   877,   885,   891,
     895,   875,   883,   876,   884,   870,   878,   871,   879,   872,
     880,   767,     0,   778,  1019,  1011,  1034,     0,  1012,     0,
       0,   290,   286,     0,   321,   323,     0,   329,     0,   353,
       0,     0,     0,     0,   348,   346,     0,     0,     0,   331,
      41,   811,   722,   719,   729,     0,     0,   730,   731,   732,
       0,   263,   419,   209,   214,   233,   249,   822,   236,   255,
     248,   250,   407,   379,     0,   382,   379,    78,   100,   101,
       0,   775,     0,   774,  1027,   220,   289,     0,     0,     0,
       0,   352,   351,  1067,   350,   339,     0,     0,     0,     0,
       0,   718,   733,     0,   752,   237,   255,     0,   249,   251,
     252,   264,   379,     0,   773,     0,     0,     0,   326,   339,
     359,   347,   349,     0,   344,     0,     0,   723,     0,   249,
       0,     0,   247,     0,   254,     0,   253,   383,   134,   776,
       0,   291,   328,     0,     0,     0,   339,     0,   721,   734,
     234,     0,     0,   258,   257,   256,   777,   339,     0,     0,
     339,   337,   724,   261,   259,   260,   327,   339,   343,   324,
     325
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    21,    22,    23,   564,   146,   147,   148,   149,  1611,
     840,   835,    24,   160,   161,  1360,  1361,  1362,  1363,  1364,
     150,   151,   568,   569,    25,    90,   461,   991,   743,   744,
     745,  1244,  1237,  1238,  1701,   291,   747,    26,    82,    27,
     179,    28,   451,   265,   365,   727,   235,    83,  1470,   272,
     455,   735,  1228,  1464,   638,   260,   446,  1179,  1679,  1680,
    1681,  1682,  1683,  1684,   267,   282,   257,   680,   258,   442,
     443,   444,   687,   688,   689,   639,   640,  1162,  1163,  1164,
    1403,  1200,   275,   369,   457,   979,   980,   981,   912,   913,
     914,  1816,   662,   554,  1526,   746,  1881,  1408,  1658,  1659,
    1660,  1822,   982,   664,   921,   922,   923,   924,  1426,   925,
    1958,  1959,  1960,  2004,   270,   367,   729,   242,   423,   560,
     657,   658,  1100,   555,  1172,  1103,  1104,  1105,  1556,  1557,
    1350,  1882,  1558,  1573,  1574,  1575,  1576,  1577,  1578,  1579,
    1580,  1581,  1582,  1583,  1584,  1585,  1887,  1586,   516,  1081,
    1587,  1588,  1589,  1995,  1770,  1771,  1893,  1894,  1590,  1591,
    1592,  1593,  1766,  1594,   517,   813,    30,   311,   556,   557,
     279,   371,   984,  1695,   647,  1168,  1469,   277,   731,   432,
     675,   676,   938,   940,  1438,    31,    66,   900,   897,   414,
     415,   427,   428,   232,   233,   429,  1188,   670,   930,   239,
     250,   418,   655,   904,   191,   407,   408,   247,   677,  1439,
      32,   105,   712,   713,   714,  1215,  1216,   715,   825,   717,
     718,  1209,   952,   967,   719,   826,   721,   722,   723,   724,
     960,   725,   726,   957,    33,    34,    35,    36,    37,   620,
      38,   584,    39,    40,    68,   254,   435,    41,   353,   354,
     355,   356,   357,   358,   359,   360,   867,   361,  1143,  1394,
     604,   605,   872,   606,    42,    43,   391,   392,   393,   887,
     883,  1595,   187,   628,   396,    45,  1491,   806,    46,    47,
      48,   587,   343,  1400,  1809,   165,   166,   167,   341,   591,
     592,   593,   858,   855,  1122,  1123,  1124,  1125,  1902,  1951,
    1976,  1126,  1127,  1128,  1129,  1907,  1908,  1909,  1130,  1389,
     171,   172,  1387,  1630,  1391,  1802,  1803,  1640,   465,   776,
    1519,  1029,  1520,  1521,  1522,  1735,  1930,  1933,   190,   630,
     631,   287,  1646,  1157,   402,  1596,  1597,    51,    52,  1598,
      54,    55,   466,   610,  1599,  1600,    56,  1112,  1615,  1616,
    1371,  1620,   314,   315,   561,   575,   576,   842,   518,   519,
     613,   759,  1004,   760,   761,   762,   763,  1263,   764,   765,
     766,   767,   768,   769,   770,   771,   772,   773,  1006,  1266,
    1267,   774,   521,   522,   523,  1410,   524,   525,   526,   527,
     528,   529,   530,  1618,    87,   289,   274,  1207,  1217,   862,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,  1074,  1742,   542,   543,   544,   545,   546,   547,  1060,
     548,   792,  1324,   793,  1321,   549,  1053,   342,   400,   550,
     551,   749,  1440,  1085,   263,   909,  1246,   295,   236,   292,
    1942,   268,  1931,  1135,  1602,   624,  1131,   152,   552,   246,
     248,  1365,   810,   280,   180,   553,   154
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1539
static const short yypact[] =
{
    1906,  1558,    41,  1727,   335,   855,   336,  1923,    58,  7785,
     479,   247,  1107,    41,   695,   151,  6159,   465,  6159,    36,
     357,   545,   243, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539,   495,   343, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,  8676,  6159,
     502,  6159,  6159,  6159,  6159,  6159, -1539, -1539,   427,   926,
    6159,  6159,  6159,   621,  6159,   447,  6159,  6159, -1539,  6159,
    6159,  6159, -1539,   616, -1539,   466, -1539, -1539,   567,  6159,
   -1539,  6159,  6159,  6159,   606,  6159,  6159,  6159,  6159,   447,
    6159,  6159,  6159,  6159,  6159, -1539,  6159,   410,   566, -1539,
   -1539, -1539,   413, -1539,   413, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539,   595,   444,   625, -1539,
      21, -1539, -1539, -1539, -1539,  6159,  6159,  6159,   656,   669,
     543,  6159,   630,    85,   302,   244,   526, -1539,  6159,   747,
     558,   698, -1539,  6159, -1539,  6159,  6159,  6159,  6159, -1539,
   -1539, -1539,  6159,  2389, -1539, -1539,   666,   675,   842,   687,
     617,   727, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539,   166, -1539, -1539, -1539,   926, -1539,   694, -1539,
     224, -1539, -1539,   586, -1539,   486, -1539, -1539,   269, -1539,
   -1539,   638, -1539,   733, -1539, -1539,   709, -1539,   800, -1539,
   -1539,   920, -1539, -1539, -1539, -1539,   926, -1539, -1539,   440,
   -1539,   586, -1539, -1539,   227, -1539,   874, -1539,   878, -1539,
     413, -1539, -1539, -1539, -1539, -1539, -1539,   890, -1539, -1539,
     694, -1539,   879, -1539,   792, -1539, -1539, -1539,   694, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,  7458,
    6159,   840, -1539,  6159, -1539, -1539, -1539,   826,   629,   860,
    5247,  6159,   929,   741,   951,   971,   977,   939,   944,   172,
     831, -1539,  7677, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539,  5847, -1539, -1539, -1539,   896,  6159,   978,   899,   340,
    6159,   933, -1539, -1539,   558, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539,  6159, -1539, -1539, -1539,   926, -1539,   586, -1539,
     874, -1539,   413,   985,   959, -1539, -1539, -1539,  1057, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539,  1053,  6159,  6159,  1026,   906, -1539,  7677,  1003, -1539,
    1003,  7677, -1539,    89,   905,   910,   907,   727, -1539,  1138,
     708, -1539,  6159, -1539,   166, -1539, -1539, -1539, -1539,  1120,
   -1539, -1539,  6159,   840,  3295,   967,  3565,   889, -1539,  8676,
   -1539, -1539,   499,   984, -1539, -1539, -1539,   912,   912,   926,
    1073,   926,   115,   800, -1539, -1539,  8641, -1539,  6159,  6159,
    6159, -1539,   840, -1539, -1539,  1159,   436,   911,  6159,  1181,
    6159, -1539,    44,  5722,  4706,  1065, -1539, -1539,   916,   918,
     923,  1012, -1539, -1539,   928, -1539,   930, -1539,   932,   934,
   -1539,   935, -1539, -1539, -1539, -1539,   938,   943,  1017,  1027,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539,   954, -1539, -1539,
    6066,   955,   956, -1539,   957,  1134,   958,   961,   968,   974,
     979,   980,  7677,  7677,  6285,  6159, -1539,  1090,   949, -1539,
    1459, -1539, -1539,   960, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539,   982,   966,   981,  8641,   992,   614, -1539, -1539,   987,
    1262,   634, -1539, -1539,  6159, -1539,  6159,  6159,    70, -1539,
   -1539, -1539, -1539,  6159,  6159,   560, -1539,  6159, -1539, -1539,
     826,  1140,  5247, -1539,  6159,  6285,   654, -1539, -1539,  7677,
   -1539,  1216,   998, -1539,   184, -1539,   323, -1539,   420, -1539,
     112, -1539,   112, -1539,   999, -1539,   120, -1539,  1124, -1539,
     229, -1539, -1539,  1015,  1002, -1539,   840,  1009,  1304, -1539,
   -1539, -1539, -1539,   117,  1220, -1539,  1024,  6159, -1539, -1539,
    1031, -1539,   471, -1539, -1539,   365, -1539,  1230,  1232, -1539,
     926, -1539,  1236, -1539, -1539,  1305, -1539,  1306, -1539, -1539,
    1281, -1539,  8641, -1539,   926,  1218,  8641,   661, -1539,  1321,
    6159, -1539,  8352, -1539,   171, -1539,  6159,   765,   486,  1158,
    1128,  1204,  1169, -1539, -1539, -1539,   148,  1221,   398, -1539,
    1165,   447, -1539,   926, -1539,  1330,  1272, -1539,   115, -1539,
   -1539, -1539,    61,  1178, -1539, -1539, -1539,  1180, -1539,  1242,
    1081, -1539,  1279,  1085,   416, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539,  1084, -1539, -1539, -1539,
     153,  1088,  1096,    67, -1539,  1085, -1539, -1539,   926, -1539,
     586, -1539,  1323,  1377, -1539,   926,   926, -1539,  3295,   269,
   -1539,  1267, -1539,  1241,  1105, -1539,  2529,  1264, -1539, -1539,
   -1539,  1273,  1108,  4925,  1110,  1302,  1365,  1471,  4450,   202,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539,  2368,   757,  1278,   718,  7677,   155,
   -1539,  6159,  7677,   731,   761,   789,  1293, -1539, -1539,  7677,
    5144,  1459,   140,  1227,  7677,  7677,  5144,  1337,  7677,  7677,
    7677,  7677,  7677,  5363,   190,   190,  1119,   343,   671,   704,
   -1539, -1539,  4580, -1539,  7677,  6159,  7677,  7677,  7677,  7677,
    7677,  7677,  6504,  2746, -1539, -1539,  1396,  1126,  6159, -1539,
    6159,  1381,  6159, -1539,  1253,  1254, -1539, -1539,  1244,  5247,
   -1539,  1374,   711, -1539,  1157, -1539,  1374,  6159,  6159,  1164,
   -1539,   671, -1539,   679,  5018,  1261,  7677, -1539,  6159, -1539,
     577, -1539, -1539,   159, -1539,  1270, -1539, -1539, -1539,  6159,
     603,  6159, -1539, -1539,  6159,  1065, -1539,  7677,  6159,  1447,
    3295, -1539, -1539, -1539, -1539,   117,  6159,  1375,  1184,  7677,
    1225,  7677,   926, -1539,   141, -1539, -1539, -1539,  1193, -1539,
   -1539, -1539, -1539,   926, -1539,   154,  6159, -1539, -1539, -1539,
   -1539,  1454, -1539,  1199,  1453, -1539,  1473, -1539,  1412,  1425,
     413, -1539, -1539, -1539, -1539, -1539,  1490, -1539, -1539, -1539,
   -1539, -1539,  6159,  8450,   447,  1390,  1391,  1392, -1539,   447,
   -1539, -1539,   670, -1539,   834, -1539, -1539,  1366,   926, -1539,
    5722,   300,  1369, -1539, -1539, -1539,   447, -1539,  1081,   348,
   -1539, -1539, -1539,  1453,   348,  1370,   348, -1539,   447,   447,
    1243,   447, -1539, -1539,   840,  6159, -1539,  1430, -1539,   720,
   -1539, -1539, -1539,   499,  1003,  6159,   724,    56, -1539, -1539,
    1246,   212, -1539,  5722,  6159,  1003,  5628, -1539, -1539,  1003,
    4925, -1539, -1539, -1539,   128, -1539,   217,  1591,  5144,  5144,
    7677,  7677,  2099,  1247,   645,  2574,  7677,  2872,   691,  3151,
    3421,  6764,  3691,  3961,  4231, -1539, -1539,  1421, -1539,  1248,
     617,  7677,  7677,   382,  7677,  1249,  7677,  1245,   690,  7677,
    7677,  7677,  7677,  7677,  7677, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539,  1476,   662,   157,  7677, -1539,  5144,  7677,
     158,   386,   504,   149,  6159,   712,   721,   737,   775,   790,
   -1539, -1539, -1539,   597,  7020, -1539,   617, -1539, -1539,  4799,
   -1539,  1250, -1539,  1257,   450, -1539, -1539,  1517,   190,   190,
     774,   774,  1459, -1539,   743, -1539, -1539, -1539, -1539, -1539,
     766, -1539,  1361,  1534,  1381, -1539, -1539,  3835,  3835,  1427,
   -1539,   185,  1446,  6159, -1539,  7677,  1446,  1492,  1494, -1539,
   -1539,  4105,  1277,   209, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539,  1271,  5144,  1488, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539,   103, -1539, -1539,  1278,  1459, -1539, -1539,
     781,  1003, -1539, -1539,   117, -1539, -1539, -1539,  1459, -1539,
     912,  1552, -1539,   141, -1539,  1286,  5144,  1958, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539,  1381, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539,   413,   413,  1326,  1301, -1539, -1539, -1539,
   -1539, -1539,   169,   199,   553, -1539,  1556, -1539, -1539, -1539,
   -1539,  6159, -1539, -1539, -1539, -1539,   335,   793,  1433,  1396,
    1308, -1539,  1165,   808, -1539,   557, -1539,  1312,  6159,   564,
    1317,  1319,   447,  1322,  1599,   269,  1381,   912,  1232,  3295,
   -1539,  1221,  1423,  3025, -1539,   447,  1478,  1566, -1539,  2529,
   -1539,   447,   425, -1539, -1539,  1568, -1539, -1539,  1335,  1336,
   -1539,  5144,  5144, -1539,  5144,  5144, -1539,  1621,   194,  1459,
    1343, -1539, -1539,  1344,  1459,  6285, -1539, -1539,  1569, -1539,
     198,  1346,  1348,  1459,  1195,  1349,  1363,  1459,  7677,  7677,
    1247,  7677,  7239,  1364,  1372,  1459,  1373,  1376,  1459,  7677,
    1459,  1378,  1380,  1459,  1382,  1383,  1459,  1384,  1385,  1459,
   -1539,  4334, -1539,   801,   809, -1539,   836, -1539,   847,  7677,
   -1539,   952,   994,  1023,  1072,  1118,  1127,  7677,  7677,  7677,
    1459,  1601,   178,  1459,  1483,  7677, -1539,  7677,  7677,  7677,
    7677, -1539, -1539, -1539, -1539, -1539, -1539,  7677, -1539,  7677,
    1295, -1539,   815,  4799,  5940, -1539, -1539,  6159, -1539, -1539,
    6159,  8026, -1539, -1539,  6159,  6159,  6159,  6159,  6159,  6159,
    1469,  1387, -1539,  1389, -1539, -1539,  1469, -1539,   266,   266,
     291, -1539, -1539,   818, -1539,  3835,  3835,  1393,   225,  1394,
    5018,  1535, -1539,  1535,   650,  1535,  1579,  1583,  7677,  1186,
    1658,  1605, -1539, -1539, -1539,   617,  1381, -1539,  1423, -1539,
     528,  1165,  1549,  1165, -1539,  5144,   130,   853, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539,  1958,  1958, -1539,  1534,
    7677,  1958,  1234,  1538,  1326,   616, -1539,  5144, -1539, -1539,
   -1539,  1555,  1570,  1571,  1572,  1573,  1580, -1539, -1539, -1539,
   -1539, -1539,  1405,   335, -1539,  1508, -1539, -1539,   348, -1539,
     253,   348, -1539,   348, -1539,  1396, -1539, -1539,  1418, -1539,
   -1539,   499,  1534,  1165, -1539,  1232, -1539, -1539,  1686, -1539,
   -1539,  1426, -1539,  1184, -1539,  1424,   926,   163,  1428, -1539,
   -1539,   926, -1539, -1539, -1539,  1621, -1539,  1621,  7677,  1003,
    1003,  1429,   343,   823,  7677,  1651, -1539,  1003,  1003,  7677,
    1003,  1003,   228,  1459, -1539,  1358,  7677,  1459,  1003,  1003,
    1003,  1003,  1459,  1003,  1003,  1003,  1003,  1003,  1003, -1539,
    1431,  1432,   642,  6159, -1539, -1539,  1435,  8641, -1539, -1539,
    1155, -1539, -1539, -1539, -1539, -1539, -1539,  1171,  1403, -1539,
   -1539,  7677,  7677,  1673,  1553,  1179,   828,  1316,  1209,  1237,
    7677, -1539, -1539, -1539, -1539,  8641,  1448, -1539, -1539,  1695,
    1696,  6159,    75,  1450,  1449,  1655,  7677,  1460,  1461,  5512,
    6159,  6159, -1539, -1539, -1539,  1687,  7918, -1539,  1462,  1463,
    1474, -1539, -1539,  1475, -1539,  1477,  1480, -1539,   143,  1481,
    1484, -1539, -1539, -1539,  1485,  1657,  1486,  1487,  1489,  1495,
    1496, -1539,  1455,   -26, -1539, -1539, -1539, -1539, -1539, -1539,
    1698, -1539,  3835,  3835, -1539,  1582, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539,  1763, -1539,   209, -1539,
   -1539, -1539,  1692, -1539,  5018,  5018,   835,  6159,  7677,  5144,
    1065, -1539,  1534, -1539, -1539, -1539, -1539,   834, -1539, -1539,
     133, -1539, -1539, -1539, -1539, -1539,  1459, -1539,  1761,  1234,
   -1539,   868,  6159, -1539,  1710,   142,  1610,  1711,  1612,  1749,
    1716,  1753,  1381, -1539, -1539,  1511, -1539,  1513,  1708,  1761,
     253, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,  1381,
    1221, -1539,   834,  1556,  1679, -1539, -1539, -1539,  1694,    90,
   -1539, -1539,  1396,  1697,  1459,  1521,  1522, -1539,   617, -1539,
    1459,  7677,  1524,  1526,  1459,  1527,  1529,  7677,  7677,  1459,
    1530,  1531,  1532,  1533,  1537,  1539,  1540,  1541,  1542,  1544,
   -1539,  4334,  1546, -1539,  6159, -1539, -1539, -1539, -1539, -1539,
   -1539,  7677,  1547, -1539, -1539, -1539,  7677, -1539, -1539,  7677,
   -1539, -1539,  1260,   181, -1539,  1765,  7677,  7677, -1539,  5144,
    1459, -1539, -1539, -1539, -1539, -1539,  6159, -1539,   514, -1539,
    1633, -1539, -1539, -1539, -1539, -1539, -1539, -1539,   203,  1550,
   -1539, -1539, -1539,  4799, -1539, -1539, -1539, -1539, -1539, -1539,
    1712, -1539, -1539, -1539, -1539,   266,  6159,  5018, -1539,   581,
   -1539, -1539,  1554, -1539,  1459,   202, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539,  1551,  6159, -1539, -1539,  1744,  1755,
    6159,  1326, -1539, -1539,   413,  6159, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539,  1534, -1539, -1539, -1539, -1539, -1539,  1534,
   -1539, -1539, -1539, -1539, -1539,  7677,   926, -1539,  1638, -1539,
     926, -1539, -1539, -1539,  1459, -1539, -1539, -1539, -1539,  1459,
    1459, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539,  6159,  1764,  1459, -1539, -1539,  1287, -1539,  1958,
    1958, -1539, -1539,  1557,  1459,  1407,   145, -1539,  1767, -1539,
    6159,  6159,   335,   136, -1539, -1539,  1600,  1768,  5144,  1250,
   -1539, -1539,  1560, -1539, -1539,  5144,  1564, -1539, -1539, -1539,
    7677, -1539, -1539, -1539, -1539, -1539,  1326,   413, -1539,  1731,
   -1539, -1539, -1539, -1539,  1556, -1539,  1459, -1539, -1539, -1539,
    1574,  1575,  1567, -1539, -1539, -1539, -1539,  1003,  4799,  1683,
    4799, -1539, -1539, -1539, -1539,  8161,   514,  7677,  4799,   146,
    6159, -1539,   202,  6159, -1539, -1539,  1731,   216,  1326,  1735,
    1736, -1539, -1539,  1576, -1539,  6159,  6159,  1577,  1250,  8161,
    1250, -1539, -1539,  1419,   150,  1812,   851, -1539,   857,  1326,
     437,   437, -1539,  1816, -1539,  1700, -1539, -1539, -1539, -1539,
    1578, -1539,  1817,  4799,  1840,  1830,  8161,  6159, -1539, -1539,
   -1539,  1677,   509, -1539, -1539, -1539, -1539,  8161,   156,  6159,
    8161, -1539, -1539, -1539, -1539, -1539, -1539,  8161, -1539, -1539,
   -1539
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -1539, -1539, -1539, -1539,  1303,  1725, -1539,  1726,  1584,   521,
   -1539,  1040, -1539, -1539, -1539,  -930, -1539,  -480, -1539,  -459,
    1728,  1586,  1309,  1054, -1539, -1539, -1539,   653, -1539,  1152,
     909, -1539, -1539,   659, -1539, -1539,   908, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539,   672, -1539, -1539,   942, -1539, -1539,
     218, -1539, -1539,  1499,  1828, -1539, -1539,  -364, -1539, -1539,
   -1539,  1467, -1539, -1539,  1223,  -412, -1539, -1539, -1539,   749,
   -1539, -1393, -1539, -1539,  1545,  1036,   688,  1497,  1006,   685,
   -1539,   240,   -55,  -302, -1539,  -380, -1539, -1288, -1539, -1539,
     264, -1539,  1500, -1278, -1539, -1539, -1539, -1539, -1320,   265,
     -31,   -33,   -30,   -53, -1539, -1539, -1539, -1539,  -147,  -376,
   -1539,  1028, -1539,  -725, -1539, -1063, -1539,   833, -1539, -1539,
   -1539, -1539, -1539, -1445, -1034, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,  -800, -1035,
   -1539, -1539, -1539, -1539, -1539,   160, -1539,   -14, -1539, -1539,
   -1539, -1539, -1539,  1935, -1539, -1539, -1539, -1539, -1539,  1112,
   -1539, -1539,  -685,   299,  -407, -1115,   547, -1539, -1539,  -625,
    -860, -1539, -1539, -1105, -1538, -1539, -1539, -1539, -1539, -1539,
    1561, -1539,  1275,  1520, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539,  1559, -1539, -1539, -1539,
   -1539, -1539,  -626,  -522, -1539,   986,   500,  -628,  -420, -1539,
   -1539, -1539, -1539, -1087, -1539,  -417, -1539, -1539, -1539, -1539,
    1228, -1539, -1539,   996, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1539,  1946, -1539, -1539, -1539, -1539, -1539,
    1609, -1539, -1539, -1539,  1367, -1539,  1362, -1539, -1539, -1539,
   -1539,  1098, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
    -697,     1, -1539, -1539, -1539,  -477,   904, -1539,  -474,   579,
   -1539,  -228,   -67, -1539, -1539, -1539, -1539,  1803, -1539, -1539,
   -1539,  1114, -1218, -1539, -1539, -1025, -1462, -1539, -1539, -1539,
   -1539,   856, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
    1881,   -35,   590,  -297, -1539, -1539,    69, -1539,  -448,  -830,
    1205, -1539,   250, -1539,   461, -1539,  -809, -1539,  -675, -1539,
    1100,  -584, -1539, -1539,  -951,  1985,  1990, -1539, -1539,  1994,
   -1539, -1539,  1395, -1539,  -245, -1539, -1539,  1149, -1242, -1539,
     883, -1539,  -104,  -783,  -619, -1539, -1539, -1539,  -796,   -49,
    -523,  -771, -1539,  -602, -1539, -1539, -1539,   147, -1539, -1539,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539,  -610, -1539,   723,
   -1539,  1215, -1064, -1539,  -476, -1539,   599, -1052,  -141,  -992,
    -990,  -969,   -52,    -2,   -40, -1539,   -62,  -868,  -562,  -150,
   -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539, -1539, -1291, -1539, -1539, -1539, -1539, -1539, -1539,
   -1539, -1539,   680, -1539, -1180, -1539, -1539,   429, -1539,  -961,
     584,  1061,   794,   204,  -765,  1354, -1275, -1539,   -66,  1926,
   -1539,   -19,   -58, -1437,   453,  -274,     4,   -60,  -162,   -10,
     -59,  -289, -1539,   -78,   -47,    -9, -1539
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1103
static const short yytable[] =
{
     153,    44,    86,   240,   650,   266,   173,   174,   888,   181,
     316,   363,  1080,   335,   338,   261,  1082,   259,   278,  1055,
     283,   262,   334,   337,   306,  1063,   716,   297,   245,   720,
     690,   570,   824,   652,   861,   273,   915,   303,   809,   301,
     807,   276,   305,   307,  1342,  1146,   251,   659,   890,   234,
     237,   843,   241,   244,   174,   249,   252,  1418,  1097,   273,
    1601,   264,   237,   252,  1422,   244,   243,   174,   249,  1352,
     281,   153,   252,  1428,   681,  1559,   733,   300,   271,  1302,
     293,   173,   174,   296,   237,   308,   293,   252,   241,   244,
     304,   174,   249,   281,   153,   252,  1378,   244,   845,  1450,
     372,  1030,   302,  1409,  1663,  1101,   322,   588,   324,   366,
     309,   807,  1419,  1148,   983,  1411,   943,   611,   992,   626,
     656,   323,  1446,  1231,   452,  1341,  1467,  1619,  1653,  1654,
     901,  1769,  1076,  1657,   905,   373,  1251,  1185,  1008,  1539,
     916,  1008,   370,   998,  1661,  1543,   264,   174,   264,   345,
    1008,   997,   153,  1008,  1008,  1843,  1005,  1008,   685,   252,
    1994,   339,   875,  1462,   364,  1008,   237,   244,   174,   281,
     409,  1637,  1798,   181,   965,  1412,   459,  1413,  1366,   636,
     368,   870,  1699,  1945,   416,   600,  1008,  1847,  1152,  1235,
    1056,  1057,   935,   -82,   857,   320,  1151,  1170,  1414,   106,
     614,  1034,  1488,  2017,  1778,   636,  1415,   815,  1325,  1326,
    1008,   882,   816,   815,   447,   410,   106,   815,   816,   168,
    1173,   616,   816,  1175,  1879,  1254,   893,   420,   936,   865,
     950,  1242,  1182,   918,    67,   582,  1717,  1256,  1257,  1160,
     879,   453,   716,  1433,  1495,   720,  1392,  -706,  1368,  1431,
     339,   815,   970,  1252,  1810,  1009,   816,   926,  1009,  1896,
    1980,   558,   838, -1071,   562,   157, -1075,  1009,   618,   589,
    1009,  1009,   430,  1381,  1009,  -378,   690,   595,   837,  1080,
     182,   331,  1009,  1082,   807,  1601,  1382,  1322,   597,  1381,
     340,  1226,  1496,   570,  -746,  1764,  1765,  1393,   866,  1842,
     421,   264,  1382,  1009,   264,   963,  1243,   321,  1161,  1383,
     919,   571,   153,   612,   615,   607,   255,   256,   107,   169,
    1848,  1757,   411,  1384,  1673,  1383,   170,  1009,   937,  1058,
    1319,   742,   454,  1642,  1779,  1904,   602,   241,  1084,  1384,
     412,   174,  1255,  1236,   637,   431,  1094,  1327,   951,  1432,
     920,  1541,  1409,  1409,   971,  1628,   839,  1409,    14,  1369,
    1385,  1743,  1744,   651,  1411,  1411,   600,  1700,  1687,  1411,
     686,  1678,   332,   558,   413,   262,  1385,   667,   333,  1434,
     671,  1661,   728,   625,   625,  1655,  1962,   682,   815,   684,
    1250,   732,  1305,   816,   997,  1406,  1981,  1213,   997,    91,
    1423,  1424,   741,   264,   737,   815,   871,  1133,  1796,   815,
     816,  1189,   614,   264,   816,   264,  1253,   264,  1651,   464,
     672,  1811,  1946,   739,  1412,  1412,  1413,  1413,  1691,  1412,
    1826,  1413,  1171,  1939,  1975,  1330,  1343,   961,  1035,   237,
     244,   249,  1343,   966,  1641,  1624,  1625,  1414,  1414,   174,
    1472,   241,  1414,   730,   750,  1415,  1415,  1399,   321,  1880,
    1415,   748,    84,   601,  1396,   817,   818,   819,   820,   188,
    -706,  1395,  1386,   819,   820,   817,   818,   819,   820,   183,
    1484,  1485,   285,  1486,  1487,  1416,  1417,  1205,  1386,  1165,
     424,  1888,  1167,   425,   815,   448,  1205,   155,   331,   816,
    1971,  1918,  1622,   962,   673,  1430,   811,  1232,  1180,   817,
     818,   819,   820,  1977,   674,   874,   286,   602,  1248,   331,
     175,   189,  1249,  1889,  1992,   612,   736,   815,   185,   834,
     944,    84,   816,  1196,   603,   426,  1812,   850,   841,   891,
    1813,  1814,   846,  1552,   331,   184,   859,  1553,    85,   108,
     570,  2011,  2014,  1901,  2001,   174,   186,   244,   571,   333,
    2012,   156,  2016,   863,   174,  2019,  1876,   238,   174,  1890,
     836,   884,  2020,   571,   253,   181,  1841,  1676,   562,  1097,
    1891,   449,   351,   974,   333,  1840,  1206,   109,   894,   336,
    1372,  1935,  1936,  2002,   331,   333,  1955,  1435,  1224,   110,
    1461,  1690,   902,  2015,   656,   860,   176,  1239,  1807,  1833,
    1799,   992,  1153,   917,  1473,  1849,   450,    85,   264,   945,
     815,   941,  1554,   111,  1119,   816,  1839,   285,   269,  1145,
    1212,   946,   290,  1436,  1650,   255,   256,   177,  1982,  1373,
    2003,   273,   347,    84,  1377,  1381,   817,   818,   819,   820,
    1083,   910,   178,   264,  1601,  1479,  1665,   910,  1382,  2000,
    1337,   286,   288,   817,   818,   819,   820,   817,   818,   819,
     820,   298,  1328,   109,  1398,   348,   973,   815,  1601,  1892,
     942,  1383,   816,   977,   978,   815,  1098,  1480,   112,   262,
     816,  1268,  1911,   398,   815,  1384,  1912,   310,   113,   816,
     313,   349,   815,  1905,   350,  1601,   312,   816,  1278,   111,
    1138,  1921,   614,   815,  1381,    14,  1601,   317,   816,  1601,
     351,  1732,  1279,   114,   339,  1318,  1601,  1382,  1925,   264,
     318,   403,  1385,   399,  2018,   815,   821,   339,   648,  1269,
     816,  1345,   844,   404,   815,  1141,   352,   319,  1899,   816,
    1383,   649,   817,   818,   819,   820,  1733,  1853,   164,  1644,
     815,  1142,  1037,  1270,  1031,   816,  1280,   339,  1734,  1645,
    1420,  1421,   252,   331,   112,  1689,   405,  1039,   406,   558,
     325,   558,  1429,  1106,   113,   817,   818,   819,   820,  1493,
    1329,  1492,  1138,   326,  1281,   339,  1401,   815,   815,  1922,
    1137,  1385,   816,   816,   330,  1923,  1086,  1041,  1961,   114,
    1806,  1963,  1117,   815,   264,  1409,  1409,  1708,   816,   264,
    1906,   264,   419,   264,   815,   612,   346,  1411,  1411,   816,
     571,   262,   815,   884,   607,  1043,  1144,   816,   174,   244,
    1159,  1664,  1025,  1451,  1386,   244,   394,  1987,  1452,  1136,
    1451,  1169,   836,  1546,   362,  1455,  1239,   558,  1282,   815,
     174,   397,   174,  1463,   816,  1026,   331,   395,  1805,   264,
     815,   264,  1191,   422,   462,   816,   401,   625,   817,   818,
     819,   820,   467,  1187,  1190,  1338,  1677,  1412,  1412,  1413,
    1413,  1686,   417,  1197,   273,   434,  1202,   264,   437,  1195,
     828,  1738,   829,  1968,   433,  1970,  1220,  1221,   436,  1223,
    1414,  1414,  1636,  1974,    88,    89,  1210,  1083,  1415,  1415,
     832,  1027,   833,   264,   264,  1527,   163,   164,   273,   273,
     445,   273,   438,   456,   439,   817,   818,   819,   820,   458,
    1225,   750,   852,   817,   818,   819,   820,   906,  1203,   907,
    1233,   614,   817,   818,   819,   820,   440,   463,  2008,  1077,
     817,   818,   819,   820,  1198,  1199,   174,  1120,   928,   460,
     929,   817,   818,   819,   820,   815,   174,   633,  1310,   634,
     816,   441,   464,  1080,   750,  1247,  1818,  1082,  1886,   559,
     821,   748,  1078,   817,   818,   819,   820,  1113,   563,  1114,
    1332,   566,   817,   818,   819,   820,  1229,  1819,  1230,  1333,
     988,   573,  1234,  1820,   577,  1492,  1492,   815,   817,   818,
     819,   820,   816,  1492,  1492,  1334,  1492,  1492,   574,   821,
    1672,  1346,   578,  1753,  1492,  1492,  1492,  1492,   579,  1492,
    1492,  1492,  1492,  1492,  1492,  1331,   815,  1821,   483,   484,
     485,   816,  1347,   893,  1348,   252,   817,   818,   819,   820,
    1693,   580,  1138,  1335,   612,  1604,   581,  1229,  1607,  1397,
     596,   817,   818,   819,   820,   583,   493,   494,  1336,  1443,
     598,  1444,   817,   818,   819,   820,  1631,   599,  1633,  1524,
     817,   818,   819,   820,  1448,   815,  1449,  1525,   571,   571,
     816,  1343,   884,  1551,   821,   608,  1623,   716,   619,   821,
     720,  1709,   244,   -50,   821,   621,  1748,   817,   818,   819,
     820,   821,   622,  1800,  1528,   255,   256,  1949,   817,   818,
     819,   820,  1791,  1793,  1952,  1529,   623,  1997,  1080,  1998,
    1080,   815,  1082,   832,  1082,  1999,   816,   627,  1080,   629,
     815,   -50,  1082,  1792,  1794,   816,  1989,  1990,    14,   641,
    1458,   643,  1272,   -50,  1276,   642,  1284,  1287,   645,  1292,
    1295,  1298,   654,  1474,   665,   668,   678,   158,   815,  1478,
     262,  1083,   273,   816,   562,   683,  -736,   -50,   734,   614,
     679,   740,  1441,  1080,   815,   273,   857,  1082,   738,   816,
     775,   273,   815,   777,  1442,   778,  1032,   816,  1036,  1441,
     779,   780,  1040,  1042,  1044,   781,   787,   782,   815,   783,
     264,   784,   785,   816,   264, -1057,   788,  1617,  1617,  1621,
     786,   797,   815,   817,   818,   819,   820,   816,   812,   814,
    1531,   789,   794,   795,   796,   798,   821,  -736,   799,  1499,
    -736,  1647,   -50,  1649,   926,   800,  -240,  -736,   823,  -736,
     815,   801,   -50,  -736,   660,   816,   802,   803,  -736,   822,
     827,  -736,   831, -1075,   830,   817,   818,   819,   820,   854,
    1608,   848,  1532,   815,   856,   869,  -736,   -50,   816,  1845,
     873,  1523,   174,   877,   878,  1555,   880,  1606,   558,  1609,
    -736,   262,   612,   885,   817,   818,   819,   820,  -736,   159,
     815,  1533,  -736,  1692,   881,   816,   886,   889,   815, -1100,
     892,  -736,   636,   816,   899, -1100,   895,   896,   898,   903,
   -1100,   908,   932, -1100,  1978,   264,   933,  -736,   264,   815,
     934,  1560,  1603,  -439,   816,   571,   244,   249,   571,   281,
     153,   947,  -240,   817,   818,   819,   820,   939,  1550,  1605,
    1534,   331,   948,   953,  -736,   954,   571,   571,   956, -1100,
     958,   244,   959,  -240,   964,   968,  -736, -1100,  -736,  -240,
     955,   815, -1101,   969,   975, -1100,   816,   976, -1101,   985,
     987,   988,   993, -1101,   994,   995, -1101,   999,  1064,   817,
     818,   819,   820, -1100,   188, -1100,  1535,  1075,   817,   818,
     819,   820,  1718,  -240,   614,  1536,  1059,   965, -1100,   333,
   -1100, -1100, -1100,  1102,  1698,  -736,   815,  1107,  1108,  1703,
     815,   816, -1101,  1924,  1111,   816,   817,   818,   819,   820,
   -1101,  1675,   815,  1739,  1115,  -736,  1109,   816, -1101,  -736,
     839,  1132,   817,   818,   819,   820,  1139,  1149,  1154,  1740,
     817,   818,   819,   820,  1741,  1156, -1101,  1747, -1101, -1100,
     832,  -736,  -736,  1176,  -736,  1801,   817,   818,   819,   820,
    1166, -1101,   815, -1101, -1101, -1101,  1177,   816, -1102,  1938,
     817,   818,   819,   820, -1102,  1756,  1178,  1750,  1183, -1102,
     911,  1993, -1102,  1045,  1046,  1047,  1048,  1049,  1050,  1051,
    1052,  1184,  1186,  1523,   174,  1192,  1193,  1194,   817,   818,
     819,   820,  1201,  1208,   520,  1751,  1218,   612,  1227,  1300,
    1222,  1309, -1101,  1241,  1265,  1301,  1343,  1307, -1102,  1317,
     815,   817,   818,   819,   820,  1349, -1102,   586,  1878,  1344,
    1351,  1367,   237,  1370, -1102,  1375,   594,  1376,  1388,  1390,
    1763,  1247,  1247,  1380,  1402,  1425,  1437,  1603,   817,   818,
     819,   820, -1102,  1405, -1102,  1934,   817,   818,   819,   820,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1102,  1427, -1102,
   -1102, -1102, -1100,  1445, -1100,    57,  1447,   817,   818,   819,
     820,  1453,  1749,   571,   571,  1456,    58,  1457,  1010,  1460,
    1459,  1235,   632,    59,   815,  1468,   635,    60,  1476,   816,
    1481,  1083,  1011,  1482,  1483,   244,   244,  1824,   364,  1008,
    1489,  1490,  1494,  1497,  1903,  1498,  1500,    61, -1102,   817,
     818,   819,   820, -1101, -1101, -1101, -1101, -1101, -1101, -1101,
    1501,  1508,  1540,   174,  1617, -1101,  1542, -1101,  1012,  1509,
    1510,  1610,  1629,  1511,  1634,  1513,  1013,  1514,  1635,  1515,
    1516,  1517,  1518,  1612,  1014,  1613,  1873,  1638,  1639,  1648,
    1666,  1626,  1627,  1662,   817,   818,   819,   820,   817,   818,
     819,   820,  1015,  1674,  1016,  1667,  1668,  1669,  1670,    62,
     817,   818,   819,   820,  1205,  1671,  1688,  1017,  1694,  1018,
    1019,  1020,  1697,  1696,  1711,   791,  1702,  1707,  1731,  1730,
    1919,  1523,   174,  1737,  1745,   241,  1746,   804,   805,   808,
      63,  1755, -1063,  1754,    64,  1758,  1759,   156,  1767,  1783,
     817,   818,   819,   820,  1789,  1761,  -340,  1772,  1773, -1102,
   -1102, -1102, -1102, -1102, -1102, -1102,  1795,  1247,  1021,  1774,
    1775, -1102,  1776, -1102,    69,  1777,  1780,  1920,  1790,  1781,
    1782,  1784,  1785,   857,  1786,    70,  1083,  1797,  1083,  1897,
    1787,  1788,    71,   614,  1815,  1917,  1083,   364,   244,  1825,
    1827,  1828,  1829,  1830,  1927,    72,  1831,  1832,  1929,  1834,
     851,  1835,  1836,  1844,   853,  1846,  1086,   614,  1850,  1851,
    1852,   174,  1855,  1956,  1856,  1857,   241,  1858,  1861,  1862,
    1863,  1864,  1768,    65,  1941,  1865,  1883,  1866,  1867,  1868,
    1869,  1083,  1870,  1872,   614,  1875,  1900,  1898,  1915,  1913,
    1910,  1916,  1928,  1932,  1937,   614,  1947,  1950,   614,  1940,
    1948,  1953,    73,  1957,  1966,   614,  1969,  1983,  1985,  1996,
    1980,  1965,  1964,   241,  1988,  1991,  2006,  2007,    74,  1022,
    1023,  1024,   817,   818,   819,   820,  2009,  2010,  2013,  1077,
    1981,   237,  1943,   847,    75,   327,   328,  1614,  1118,   329,
    1944,   849,  1477,  1110,   986,  1475,   612,  1240,  1838,    76,
    1465,  1245,   565,    77,   562,  1214,    78,   572,   646,   284,
     691,   949,  1404,     1,    79,   617,  1150,  1466,  1471,  1837,
     612,   661,  1181,  1817,   663,  1979,  1823,  1986,  2005,  1984,
    1895,     2,  1972,    80,  1174,    29,  1603,  1353,  1967,     3,
    1099,  1136,  1808,   931,   264,  1643,     4,   612,     5,   669,
       6,  1685,  1219,   972,  1211,     7,   241,   241,   612,   162,
    1603,   612,     8,   609,   868,   864,   644,  1140,   612,   344,
    1134,    92,   294,  1007,  1632,   653,     9,  1379,    93,  1954,
    1028,  1871,    94,    95,  1736,    49,    10,  1603,  1136,  1155,
      50,    96,    81,  1033,    53,  1116,   471,  1038,  1603,  1374,
    1247,  1603,    97,  1504,  1054,   876,  1652,  1544,  1603,  1061,
    1062,  1204,  1454,  1065,  1066,  1067,  1068,  1069,  1073,  1914,
     927,   299,  1762,     0,   473,     0,     0,     0,     0,   520,
       0,  1087,  1088,  1089,  1090,  1091,  1092,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    11,
       0,     0,   475,     0,     0,     0,    12,     0,     0,    13,
       0,    14,    15,     0,    98,     0,     0,     0,     0,     0,
       0,   594,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,    16,     0,     0,     0,
       0,     0,  1147,     0,     0,   100,     0,     0,     0,   101,
       0,     0,     0,     0,   632,  1260,  1158,     0,  1261,     0,
     102,     0,   468,     0,     0,     0,     0,     0,     0,   469,
       0,     0,     0,   480,     0,     0,     0,     0,     0,   103,
       0,   470,     0,     0,     0,    17,     0,   471,   472,   482,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    19,     0,   483,   484,   485,     0,   255,   256,     0,
       0,     0,     0,     0,     0,   473,     0,     0,   474,     0,
       0,     0,     0,     0,     0,    20,   488,   489,   490,   491,
     492,   493,   494,   495,   496,     0,     0,     0,   104,     0,
       0,   498,   499,   475,     0,   476,   477,     0,     0,     0,
     503,     0,     0,     0,     0,   478,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1258,  1259,  1264,     0,     0,
    1273,  1274,  1277,     0,  1285,  1288,  1290,  1293,  1296,  1299,
    1407,     0,     0,     0,     0,     0,  1303,  1304,     0,  1306,
       0,  1308,     0,     0,  1311,  1312,  1313,  1314,  1315,  1316,
       0,     0,  1262,     0,   480,     0,     0,     0,     0,     0,
     481,  1320,     0,     0,  1323,     0,     0,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,     0,  1340,
       0,     0,     0,     0,   483,   484,   485,   486,   255,   256,
     116,     0,   117,   118,   119,   120,     0,   121,   487,     0,
       0,     0,     0,     0,     0,   122,   123,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   124,   497,     0,   126,
     520,     0,   498,   499,     0,   500,   501,   502,     0,   129,
     130,   503,   131,     0,     0,   132,   133,   134,   135,   136,
       0,     0,     0,   137,   138,     0,     0,     0,     0,   139,
     504,   141,     0,   505,   143,   144,     0,   145,     0,   506,
     507,   508,     0,   509,   510,     0,   511,     0,     0,     0,
     512,   513,     0,     0,     0,  1010,   585,     0,   515,     0,
       0,   815,   333,  1705,  1706,     0,   816,     0,     0,  1011,
       0,  1712,  1713,     0,  1715,  1716,     0,     0,     0,     0,
     374,     0,  1720,  1721,  1722,  1723,     0,  1724,  1725,  1726,
    1727,  1728,  1729,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,     0,  1012,     0,   376,     0,     0,
       0,     0,     0,  1013,   377,     0,     0,     0,   378,   379,
       0,  1014,     0,     0,     0,     0,     0,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,  1015,
       0,  1016,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,  1017,     0,  1018,  1019,  1020,     0,
       0,     0,     0,  1502,  1503,     0,  1505,  1507,     0,     0,
       0,     0,     0,     0,  1512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   382,     0,  1530,     0,     0,     0,     0,     0,
     383,     0,  1537,  1538,  1320,  1021,     0,     0,     0,     0,
    1320,     0,  1323,  1545,   520,  1547,     0,   989,     0,     0,
     693,     0,  1548,     0,  1549,     0,     0,     0,     0,     0,
       0,   384,     0,   990,     0,   385,     0,   694,     0,     0,
     695,     0,     0,     0,     0,     0,   386,     0,     0,     0,
    1271,     0,     0,  1261,     0,     0,     0,   468,     0,     0,
       0,     0,     0,     0,   469,   387,   388,     0,     0,     0,
       0,     0,     0,   520,     0,     0,   470,     0,     0,     0,
     696,     0,   471,   472,     0,     0,   697,   698,   699,     0,
     700,   701,   702,     0,   703,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1656,     0,     0,     0,     0,
     473,     0,   704,   474,   705,     0,  1022,  1023,  1024,   817,
     818,   819,   820,     0,   389,     0,     0,   390,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   475,     0,
     476,   477,     0,   706,     0,     0,     0,     0,     0,     0,
     478,     0,     0,     0,   479,     0,     0,     0,     0,     0,
     707,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1704,     0,     0,     0,     0,     0,  1710,
       0,     0,   708,     0,  1714,     0,     0,     0,     0,     0,
       0,  1719,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1262,     0,   480,
       0,     0,     0,     0,     0,   481,     0,   709,   710,     0,
       0,     0,     0,     0,     0,   482,  1320,  1320,     0,     0,
       0,     0,     0,     0,   711,  1752,     0,     0,     0,   483,
     484,   485,   486,   255,   256,   116,     0,   117,   118,   119,
     120,  1760,   121,   487,     0,  1095,     0,     0,     0,     0,
     122,   123,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   124,   497,     0,   126,     0,     0,   498,   499,     0,
     500,   501,   502,     0,   129,   130,   503,   131,     0,     0,
     132,   133,   134,   135,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,   139,   504,   141,     0,   505,   143,
     144,     0,   145,     0,   506,   507,   508,     0,   509,   510,
       0,   511,     0,  1804,     0,   512,   513,     0,     0,     0,
       0,   585,     0,   515,     0,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1275,     0,
       0,  1261,     0,     0,     0,   468,     0,     0,     0,     0,
       0,     0,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   470,     0,     0,     0,     0,     0,
     471,   472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1854,     0,     0,     0,
       0,     0,  1859,  1860,     0,     0,     0,     0,   473,     0,
       0,   474,     0,     0,   115,     0,     0,   116,     0,   117,
     118,   119,   120,     0,   121,     0,  1874,     0,     0,     0,
       0,  1320,   122,   123,  1877,     0,   475,     0,   476,   477,
       0,  1884,  1885,   124,   125,     0,   126,     0,   478,     0,
       0,     0,   479,   127,   128,     0,   129,   130,     0,   131,
       0,     0,   132,   133,   134,   135,   136,     0,     0,     0,
     137,   138,     0,     0,     0,     0,   139,   140,   141,     0,
     142,   143,   144,     0,   145,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1096,
       0,     0,     0,     0,     0,  1262,     0,   480,     0,     0,
       0,     0,     0,   481,     0,     0,     0,     0,     0,     0,
       0,     0,   911,   482,     0,     0,     0,     0,     0,     0,
    1926,     0,     0,     0,     0,     0,     0,   483,   484,   485,
     486,   255,   256,   116,     0,   117,   118,   119,   120,     0,
     121,   487,     0,     0,     0,     0,     0,     0,   122,   123,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   124,
     497,     0,   126,     0,     0,   498,   499,     0,   500,   501,
     502,     0,   129,   130,   503,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,  1804,   137,   138,     0,     0,
       0,     0,   139,   504,   141,     0,   505,   143,   144,     0,
     145,     0,   506,   507,   508,     0,   509,   510,     0,   511,
       0,     0,     0,   512,   513,     0,     0,  1283,     0,   585,
    1261,   515,  1973,     0,   468,   333,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   470,     0,     0,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   115,     0,     0,   116,     0,   117,   118,
     119,   120,     0,   121,     0,     0,     0,     0,     0,     0,
       0,   122,   123,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   124,   125,     0,   126,     0,   478,     0,     0,
       0,   479,   127,   128,     0,   129,   130,     0,   131,     0,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,     0,     0,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,  1262,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,   660,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     255,   256,   116,     0,   117,   118,   119,   120,     0,   121,
     487,     0,     0,     0,     0,     0,     0,   122,   123,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   124,   497,
       0,   126,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   129,   130,   503,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   504,   141,     0,   505,   143,   144,     0,   145,
       0,   506,   507,   508,     0,   509,   510,  1286,   511,     0,
    1261,     0,   512,   513,   468,     0,     0,     0,   585,     0,
     515,   469,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,   470,     0,     0,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   115,     0,     0,   116,     0,   117,   118,
     119,   120,     0,   121,     0,     0,     0,     0,     0,     0,
       0,   122,   123,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   124,   125,     0,   126,     0,   478,     0,     0,
       0,   479,   127,   128,     0,   129,   130,     0,   131,     0,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,     0,     0,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1262,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,   666,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     255,   256,   116,     0,   117,   118,   119,   120,     0,   121,
     487,     0,     0,     0,     0,     0,     0,   122,   123,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   124,   497,
       0,   126,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   129,   130,   503,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   504,   141,     0,   505,   143,   144,     0,   145,
       0,   506,   507,   508,     0,   509,   510,  1291,   511,     0,
    1261,     0,   512,   513,   468,     0,     0,     0,   585,     0,
     515,   469,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,   470,     0,     0,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   115,     0,     0,   116,     0,   117,   118,
     119,   120,     0,   121,     0,     0,     0,     0,     0,     0,
       0,   122,   123,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   124,   125,     0,   126,     0,   478,     0,     0,
       0,   479,   127,   128,     0,   129,   130,     0,   131,     0,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,     0,     0,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1262,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     255,   256,   116,     0,   117,   118,   119,   120,     0,   121,
     487,     0,     0,     0,     0,     0,  1354,   122,   123,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   124,   497,
       0,   126,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   129,   130,   503,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   504,   141,     0,   505,   143,   144,     0,   145,
       0,   506,   507,   508,     0,   509,   510,  1294,   511,     0,
    1261,     0,   512,   513,   468,     0,  1355,     0,   585,     0,
     515,   469,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,   470,     0,     0,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1356,     0,     0,     0,     0,  1357,     0,     0,     0,
       0,     0,  1358,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   115,     0,     0,   116,     0,   117,   118,
     119,  1359,     0,   121,     0,     0,     0,     0,     0,     0,
       0,   122,   123,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   124,   125,     0,   126,     0,   478,     0,     0,
       0,   479,   127,   128,     0,   129,   130,     0,   131,     0,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,     0,     0,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1262,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     255,   256,   116,     0,   117,   118,   119,   120,     0,   121,
     487,     0,     0,     0,     0,     0,     0,   122,   123,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   124,   497,
       0,   126,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   129,   130,   503,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   504,   141,     0,   505,   143,   144,     0,   145,
       0,   506,   507,   508,     0,   509,   510,  1297,   511,     0,
    1261,     0,   512,   513,   468,     0,     0,     0,   585,     0,
     515,   469,     0,     0,   333,     0,     0,     0,     0,     0,
      14,     0,     0,   470,     0,     0,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   115,     0,     0,   116,     0,   117,   118,
     119,   120,     0,   121,     0,     0,     0,     0,     0,     0,
       0,   122,   123,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   124,   125,     0,   126,     0,   478,     0,     0,
       0,   479,   127,   128,     0,   129,   130,     0,   131,     0,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,     0,     0,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1121,     0,  1262,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,  1025,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     255,   256,   116,     0,   117,   118,   119,   120,     0,   121,
     487,     0,  1026,     0,     0,     0,     0,   122,   123,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   124,   497,
       0,   126,     0,   468,   498,   499,     0,   500,   501,   502,
     469,   129,   130,   503,   131,     0,     0,   132,   133,   134,
     135,   136,   470,     0,     0,   137,   138,     0,   471,   472,
       0,   139,   504,   141,     0,   505,   143,   144,  1027,   145,
       0,   506,   507,   508,     0,   509,   510,   752,   511,     0,
       0,     0,   512,   513,     0,     0,   473,     0,   585,   474,
     515,     0,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,   115,     0,     0,   116,     0,   117,   118,   119,
     120,     0,   121,     0,   475,     0,   476,   477,     0,     0,
     122,   123,     0,     0,     0,     0,   478,     0,     0,     0,
     479,   124,   125,     0,   126,     0,     0,     0,  1000,     0,
       0,   127,   128,     0,   129,   130,     0,   131,     0,     0,
     132,   133,   134,   135,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,   139,   140,   141,     0,   142,   143,
     144,     0,   145,     0,     0,    14,     0,     0,     0,   754,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   481,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   483,   484,   485,   486,   255,
     256,   116,     0,   117,   118,   119,   120,     0,   121,   487,
       0,     0,     0,     0,     0,     0,   122,   123,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   124,   497,     0,
     126,     0,     0,   498,   499,     0,   500,   501,   502,     0,
     129,   130,   503,   131,     0,     0,   132,   133,   134,   135,
     136,  1001,  1002,  1003,   137,   138,     0,     0,     0,     0,
     139,   504,   141,     0,   505,   143,   144,     0,   145,   468,
     506,   507,   508,     0,   509,   510,   469,   511,     0,     0,
       0,   512,   513,     0,     0,     0,     0,   758,   470,   515,
       0,   751,     0,   333,   471,   472,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,     0,   474,     0,     0,   115,     0,
       0,   116,     0,   117,   118,   119,   120,     0,   121,     0,
       0,     0,     0,     0,     0,     0,   122,   123,     0,     0,
     475,     0,   476,   477,     0,     0,     0,   124,   125,     0,
     126,     0,   478,     0,     0,     0,   479,   127,   128,     0,
     129,   130,     0,   131,   753,     0,   132,   133,   134,   135,
     136,     0,     0,     0,   137,   138,     0,     0,     0,     0,
     139,   140,   141,     0,   142,   143,   144,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   754,     0,  1079,     0,   515,
       0,   480,     0,     0,     0,     0,     0,   481,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   483,   484,   485,   486,   255,   256,   116,     0,   117,
     118,   119,   120,     0,   121,   487,     0,     0,     0,     0,
       0,     0,   122,   123,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   124,   497,     0,   126,     0,   468,   498,
     499,     0,   500,   501,   502,   469,   129,   130,   503,   131,
       0,     0,   132,   133,   755,   756,   757,   470,     0,     0,
     137,   138,     0,   471,   472,     0,   139,   504,   141,     0,
     505,   143,   144,     0,   145,     0,   506,   507,   508,     0,
     509,   510,   752,   511,     0,     0,     0,   512,   513,     0,
       0,   473,     0,   758,   474,   515,     0,   115,     0,   333,
     116,     0,   117,   118,   119,   120,     0,   121,     0,     0,
       0,     0,     0,     0,     0,   122,   123,     0,     0,   475,
       0,   476,   477,     0,     0,     0,   124,   125,     0,   126,
       0,   478,     0,     0,     0,   479,   127,   128,     0,   129,
     130,     0,   131,   996,     0,   132,   133,   134,   135,   136,
       0,     0,     0,   137,   138,     0,     0,     0,     0,   139,
     140,   141,     0,   142,   143,   144,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   754,     0,     0,     0,   515,     0,
     480,     0,     0,     0,     0,     0,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     483,   484,   485,   486,   255,   256,   116,     0,   117,   118,
     119,   120,     0,   121,   487,     0,     0,     0,     0,     0,
       0,   122,   123,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   124,   497,     0,   126,     0,   468,   498,   499,
       0,   500,   501,   502,   469,   129,   130,   503,   131,     0,
       0,   132,   133,   755,   756,   757,   470,     0,     0,   137,
     138,     0,   471,   472,     0,   139,   504,   141,     0,   505,
     143,   144,     0,   145,     0,   506,   507,   508,     0,   509,
     510,   752,   511,     0,     0,     0,   512,   513,     0,     0,
     473,     0,   758,   474,   515,     0,   115,     0,   333,   116,
       0,   117,   118,   119,   120,     0,   121,     0,     0,     0,
       0,     0,     0,     0,   122,   123,     0,     0,   475,     0,
     476,   477,     0,     0,     0,   124,   125,     0,   126,     0,
     478,     0,     0,     0,   479,   127,   128,     0,   129,   130,
       0,   131,   753,     0,   132,   133,   134,   135,   136,     0,
       0,     0,   137,   138,     0,     0,     0,     0,   139,   140,
     141,     0,   142,   143,   144,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   754,     0,  1121,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   481,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   483,
     484,   485,   486,   255,   256,   116,     0,   117,   118,   119,
     120,     0,   121,   487,     0,     0,     0,     0,     0,     0,
     122,   123,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   124,   497,     0,   126,     0,   468,   498,   499,     0,
     500,   501,   502,   469,   129,   130,   503,   131,     0,     0,
     132,   133,   755,   756,   757,   470,     0,     0,   137,   138,
       0,   471,   472,     0,   139,   504,   141,     0,   505,   143,
     144,     0,   145,     0,   506,   507,   508,     0,   509,   510,
       0,   511,     0,     0,     0,   512,   513,     0,   567,   473,
       0,   758,   474,   515,     0,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,   115,     0,     0,   116,     0,
     117,   118,   119,   120,     0,   121,     0,   475,     0,   476,
     477,     0,     0,   122,   123,     0,     0,     0,     0,   478,
       0,     0,     0,   479,   124,   125,     0,   126,     0,     0,
       0,     0,     0,     0,   127,   128,     0,   129,   130,     0,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   140,   141,
       0,   142,   143,   144,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   480,     0,
       0,     0,     0,     0,   481,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   483,   484,
     485,   486,   255,   256,   116,     0,   117,   118,   119,   120,
       0,   121,   487,     0,     0,     0,     0,     0,     0,   122,
     123,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     124,   497,     0,   126,     0,     0,   498,   499,     0,   500,
     501,   502,     0,   129,   130,   503,   131,     0,     0,   132,
     133,   134,   135,   136,     0,     0,     0,   137,   138,     0,
       0,     0,     0,   139,   504,   141,     0,   505,   143,   144,
    1070,   145,     0,   506,   507,   508,  1071,   509,   510,  1072,
     511,   468,     0,     0,   512,   513,     0,     0,   469,     0,
     585,     0,   515,     0,     0,     0,   333,     0,     0,     0,
     470,     0,     0,     0,     0,     0,   471,   472,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,   474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     115,     0,     0,   116,     0,   117,   118,   119,   120,     0,
     121,     0,   475,     0,   476,   477,     0,     0,   122,   123,
       0,     0,     0,     0,   478,     0,     0,     0,   479,   124,
     125,     0,   126,     0,     0,     0,   996,     0,     0,   127,
     128,     0,   129,   130,     0,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,     0,
       0,     0,   139,   140,   141,     0,   142,   143,   144,     0,
     145,     0,     0,     0,     0,     0,     0,   754,     0,     0,
       0,     0,     0,   480,     0,     0,     0,  -341,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   483,   484,   485,   486,   255,   256,   116,
       0,   117,   118,   119,   120,     0,   121,   487,     0,     0,
       0,     0,     0,     0,   122,   123,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   124,   497,     0,   126,     0,
     468,   498,   499,     0,   500,   501,   502,   469,   129,   130,
     503,   131,     0,     0,   132,   133,   134,   135,   136,   470,
       0,     0,   137,   138,     0,   471,   472,     0,   139,   504,
     141,     0,   505,   143,   144,     0,   145,     0,   506,   507,
     508,     0,   509,   510,     0,   511,     0,     0,     0,   512,
     513,     0,     0,   473,     0,   758,   474,   515,    84,     0,
     115,   333,     0,   116,     0,   117,   118,   119,   120,     0,
     121,     0,     0,     0,     0,     0,     0,     0,   122,   123,
       0,   475,     0,   476,   477,     0,     0,     0,     0,   124,
     125,     0,   126,   478,     0,     0,     0,   479,     0,   127,
     128,     0,   129,   130,     0,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,     0,
       0,     0,   139,   140,   141,     0,   142,   143,   144,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   255,   256,   116,     0,
     117,   118,   119,   120,     0,   121,   487,     0,     0,     0,
       0,     0,     0,   122,   123,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   124,   497,     0,   126,     0,   468,
     498,   499,     0,   500,   501,   502,   469,   129,   130,   503,
     131,     0,     0,   132,   133,   134,   135,   136,   470,     0,
       0,   137,   138,     0,   471,   472,     0,   139,   504,   141,
       0,   505,   143,   144,     0,   145,     0,   506,   507,   508,
       0,   509,   510,     0,   511,     0,     0,     0,   512,   513,
     590,     0,   473,     0,   585,   474,   515,     0,   115,     0,
     333,   116,     0,   117,   118,   119,   120,     0,   121,     0,
       0,     0,     0,     0,     0,     0,   122,   123,     0,     0,
     475,     0,   476,   477,     0,     0,     0,   124,   125,     0,
     126,     0,   478,     0,     0,     0,   479,   127,   128,     0,
     129,   130,     0,   131,     0,     0,   132,   133,   134,   135,
     136,     0,     0,     0,   137,   138,     0,     0,     0,     0,
     139,   140,   141,     0,   142,   143,   144,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1096,     0,     0,     0,     0,     0,     0,
       0,   480,     0,     0,     0,     0,     0,   481,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   482,     0,     0,
       0,     0,     0,     0,     0,   790,     0,     0,     0,     0,
       0,   483,   484,   485,   486,   255,   256,   116,     0,   117,
     118,   119,   120,     0,   121,   487,     0,     0,     0,     0,
       0,     0,   122,   123,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   124,   497,     0,   126,     0,   468,   498,
     499,     0,   500,   501,   502,   469,   129,   130,   503,   131,
       0,     0,   132,   133,   134,   135,   136,   470,     0,     0,
     137,   138,     0,   471,   472,     0,   139,   504,   141,     0,
     505,   143,   144,     0,   145,     0,   506,   507,   508,     0,
     509,   510,     0,   511,     0,     0,     0,   512,   513,     0,
       0,   473,     0,   585,   474,   515,     0,   115,     0,   333,
     116,     0,   117,   118,   119,   120,     0,   121,     0,     0,
       0,     0,     0,     0,     0,   122,   123,     0,     0,   475,
       0,   476,   477,     0,     0,     0,   124,   125,     0,   126,
       0,   478,     0,     0,     0,   479,   127,   128,     0,   129,
     130,     0,   131,     0,     0,   132,   133,   134,   135,   136,
       0,     0,     0,   137,   138,     0,     0,     0,     0,   139,
     140,   141,     0,   142,   143,   144,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     480,     0,     0,     0,     0,     0,   481,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     483,   484,   485,   486,   255,   256,   116,     0,   117,   118,
     119,   120,     0,   121,   487,     0,     0,     0,     0,     0,
       0,   122,   123,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   124,   497,     0,   126,     0,   468,   498,   499,
       0,   500,   501,   502,   469,   129,   130,   503,   131,     0,
       0,   132,   133,   134,   135,   136,   470,     0,     0,   137,
     138,     0,   471,   472,     0,   139,   504,   141,     0,   505,
     143,   144,     0,   145,     0,   506,   507,   508,     0,   509,
     510,     0,   511,     0,     0,     0,   512,   513,     0,     0,
     473,     0,   585,   474,   515,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   475,     0,
     476,   477,     0,     0,     0,     0,     0,     0,     0,     0,
     478,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   481,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   483,
     484,   485,   486,   255,   256,   116,     0,   117,   118,   119,
     120,     0,   121,   487,     0,     0,     0,     0,     0,     0,
     122,   123,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   124,   497,     0,   126,     0,     0,   498,   499,     0,
     500,   501,   502,     0,   129,   130,   503,   131,     0,     0,
     132,   133,   134,   135,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,   139,   504,   141,     0,   505,   143,
     144,     0,   145,     0,   506,   507,   508,   468,   509,   510,
       0,   511,     0,     0,   469,   512,   513,     0,     0,     0,
       0,   585,  1093,   515,     0,     0,   470,   333,     0,     0,
       0,     0,   471,   472,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   475,     0,
     476,   477,     0,     0,     0,     0,     0,     0,     0,     0,
     478,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,   481,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1289,     0,     0,   483,
     484,   485,   486,   255,   256,   116,     0,   117,   118,   119,
     120,     0,   121,   487,     0,     0,     0,     0,     0,     0,
     122,   123,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   124,   497,     0,   126,     0,     0,   498,   499,     0,
     500,   501,   502,     0,   129,   130,   503,   131,     0,     0,
     132,   133,   134,   135,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,   139,   504,   141,     0,   505,   143,
     144,     0,   145,   468,   506,   507,   508,     0,   509,   510,
     469,   511,     0,     0,     0,   512,   513,     0,     0,     0,
       0,   585,   470,   515,     0,     0,     0,   333,   471,   472,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1339,     0,     0,   473,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   475,     0,   476,   477,     0,     0,
       0,     0,     0,     0,     0,     0,   478,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,   481,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   483,   484,   485,   486,   255,
     256,   116,     0,   117,   118,   119,   120,     0,   121,   487,
       0,     0,     0,     0,     0,     0,   122,   123,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   124,   497,     0,
     126,     0,   468,   498,   499,     0,   500,   501,   502,   469,
     129,   130,   503,   131,     0,     0,   132,   133,   134,   135,
     136,   470,     0,     0,   137,   138,     0,   471,   472,     0,
     139,   504,   141,     0,   505,   143,   144,     0,   145,     0,
     506,   507,   508,     0,   509,   510,     0,   511,     0,     0,
       0,   512,   513,     0,     0,   473,     0,   585,   474,   515,
       0,     0,     0,   333,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   475,     0,   476,   477,     0,     0,     0,
       0,     0,     0,     0,     0,   478,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   480,     0,     0,     0,     0,     0,
     481,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1506,     0,     0,   483,   484,   485,   486,   255,   256,
     116,     0,   117,   118,   119,   120,     0,   121,   487,     0,
       0,     0,     0,     0,     0,   122,   123,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   124,   497,     0,   126,
       0,   468,   498,   499,     0,   500,   501,   502,   469,   129,
     130,   503,   131,     0,     0,   132,   133,   134,   135,   136,
     470,     0,     0,   137,   138,     0,   471,   472,     0,   139,
     504,   141,     0,   505,   143,   144,     0,   145,     0,   506,
     507,   508,     0,   509,   510,     0,   511,     0,     0,     0,
     512,   513,     0,     0,   473,     0,   585,   474,   515,     0,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   475,     0,   476,   477,     0,     0,     0,     0,
       0,     0,     0,     0,   478,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   482,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   483,   484,   485,   486,   255,   256,   116,
       0,   117,   118,   119,   120,     0,   121,   487,     0,     0,
       0,     0,     0,     0,   122,   123,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   124,   497,     0,   126,     0,
     468,   498,   499,     0,   500,   501,   502,   469,   129,   130,
     503,   131,     0,     0,   132,   133,   134,   135,   136,   470,
       0,     0,   137,   138,     0,   471,   472,     0,   139,   504,
     141,     0,   505,   143,   144,     0,   145,     0,   506,   507,
     508,     0,   509,   510,     0,   511,     0,     0,     0,   512,
     513,     0,     0,   473,     0,   514,   474,   515,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,     0,   476,   477,     0,     0,     0,     0,     0,
       0,     0,     0,   478,     0,     0,     0,   479,     0,     0,
       0,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   255,   256,   116,     0,
     117,   118,   119,   120,     0,   121,   487,     0,     0,     0,
       0,     0,     0,   122,   123,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   124,   497,     0,   126,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   129,   130,   503,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
     112,   137,   138,     0,  1351,     0,     0,   139,   504,   141,
     113,   505,   143,   144,     0,   145,     0,   506,   507,   508,
       0,   509,   510,     0,   511,     0,     0,     0,   512,   513,
       0,     0,     6,     0,   585,   114,   515,     0,     0,  -295,
     333,     0,     0,  1561,  1562,     0,  1563,     0,     0,     0,
       0,     0,     0,   115,     0,     0,   116,     0,   117,   118,
     119,   120,  1564,   121,     0,     0,     0,     0,  1565,     0,
       0,   122,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,   125,     0,   126,     0,     0,     0,     0,
       0,     0,   127,   128,     0,   129,   130,     0,   131,     0,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,  1351,     0,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,  1566,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     0,     0,    14,     0,     0,     0,  -297,     0,     0,
       0,  1561,  1562,     0,  1563,     0,     0,     0,  1567,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
    1564,     0,     0,     0,     0,     0,  1565,  1768,     0,     0,
       0,     0,     0,     0,     0,     0,   486,     0,     0,   116,
       0,   117,   118,   119,   120,     0,   121,     0,     0,     0,
       0,     0,     0,     0,   122,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1568,   125,     0,   126,     0,
       0,     0,     0,     0,     0,   127,   128,     0,   129,   130,
       0,   131,     0,  1566,   132,  1569,   134,   135,   136,     0,
       0,     0,   137,   138,  1570,  1571,  1572,  1351,   139,   140,
     141,    14,   142,   143,   144,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1567,     0,     0,     0,
       0,     0,     0,     0,     0,     6,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1561,  1562,     0,  1563,
       0,     0,     0,     0,   486,     0,     0,   116,     0,   117,
     118,   119,   120,     0,   121,  1564,     0,     0,     0,     0,
       0,  1565,   122,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1568,   125,     0,   126,     0,     0,     0,
       0,     0,     0,   127,   128,     0,   129,   130,     0,   131,
       0,     0,   132,  1569,   134,   135,   136,     0,     0,     0,
     137,   138,  1570,  1571,  1572,     0,   139,   140,   141,     0,
     142,   143,   144,     0,   145,     0,     0,     0,  1566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1567,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   486,
       0,     0,   116,     0,   117,   118,   119,   120,     0,   121,
     692,     0,     0,   693,     0,     0,     0,   122,   123,   911,
       0,     0,     0,     0,     0,     0,     0,     0,  1568,   125,
     694,   126,     0,   695,     0,     0,     0,     0,   127,   128,
       0,   129,   130,     0,   131,     0,     0,   132,  1569,   134,
     135,   136,     0,     0,     0,   137,   138,  1570,  1571,  1572,
       0,   139,   140,   141,     0,   142,   143,   144,     0,   145,
       0,     0,     0,   696,     0,     0,     0,     0,     0,   697,
     698,   699,     0,   700,   701,   702,     0,   703,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   704,     0,   705,   692,     0,
       0,   693,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   694,     0,
       0,   695,     0,     0,     0,     0,   706,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   707,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   696,     0,     0,     0,   708,     0,   697,   698,   699,
       0,   700,   701,   702,     0,   703,     0,     0,     0,     0,
     115,     0,     0,   116,     0,   117,   118,   119,   120,     0,
     121,     0,     0,   704,     0,   705,     0,     0,   122,   123,
     709,   710,     0,     0,     0,     0,     0,     0,     0,   124,
     125,     0,   126,     0,     0,     0,     0,   711,     0,   127,
     128,     0,   129,   130,   706,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,     0,
       0,   707,   139,   140,   141,     0,   142,   143,   144,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   708,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,     0,
       0,   116,     0,   117,   118,   119,   120,     0,   121,   692,
       0,     0,   693,     0,     0,     0,   122,   123,   709,   710,
       0,     0,     0,     0,     0,     0,     0,   124,   125,   694,
     126,     0,   695,     0,     0,   711,     0,   127,   128,     0,
     129,   130,     0,   131,     0,     0,   132,   133,   134,   135,
     136,     0,     0,     0,   137,   138,     0,     0,     0,     0,
     139,   140,   141,     0,   142,   143,   144,     0,   145,     0,
       0,     0,   696,     0,     0,     0,     0,     0,   697,   698,
     699,     0,   700,   701,   702,     0,   703,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   704,     0,   705,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   706,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   707,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   708,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   709,
     710,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   115,     0,   711,   116,   192,   117,
     118,   119,   120,   193,   121,   194,   195,   196,   197,   198,
     199,   200,   122,   123,   201,   202,   203,   204,   205,     0,
       0,   206,   207,   124,   125,   208,   126,     0,     0,   209,
     210,   211,   212,   127,   128,   213,   129,   130,   214,   131,
     215,   216,   132,   133,   134,   135,   136,     0,     0,     0,
     137,   138,   217,   218,   219,   220,   139,   140,   141,   221,
     142,   143,   144,   222,   145,   223,   224,   225,   226,   227,
     228,   229,   230,   231
};

static const short yycheck[] =
{
       9,     0,     4,    61,   411,    71,    16,    16,   627,    18,
     114,   173,   812,   163,   164,    70,   812,    69,    77,   790,
      80,    70,   163,   164,   102,   796,   446,    93,    63,   446,
     442,   320,   554,   413,   596,    75,   662,    99,   514,    97,
     514,    76,   101,   103,  1079,   875,    65,   423,   632,    58,
      59,   574,    61,    62,    63,    64,    65,  1172,   823,    99,
    1351,    70,    71,    72,  1179,    74,    62,    76,    77,  1103,
      79,    80,    81,  1188,   438,  1350,   452,    96,    74,  1030,
      89,    91,    91,    92,    93,   104,    95,    96,    97,    98,
     100,   100,   101,   102,   103,   104,  1121,   106,   575,  1214,
     178,   776,    98,  1167,  1424,   830,   155,   335,   157,   175,
     106,   585,  1175,   878,   739,  1167,   678,   362,   746,   393,
     422,   156,  1209,   983,   271,  1076,  1231,  1369,  1416,  1417,
     652,  1576,   807,  1421,   656,   182,     8,   920,     8,  1319,
     662,     8,   177,   753,  1422,  1325,   155,   156,   157,   168,
       8,   753,   161,     8,     8,  1693,   758,     8,    43,   168,
      10,     6,   610,  1226,   173,     8,   175,   176,   177,   178,
       4,  1389,  1634,   182,    21,  1167,   280,  1167,  1108,    90,
     176,    61,    19,    47,   236,    26,     8,    97,   885,   133,
      50,    51,    44,   149,    10,   174,   881,    43,  1167,   141,
     362,    46,     8,    47,    61,    90,  1167,    23,    50,    51,
       8,    94,    28,    23,   266,    49,   141,    23,    28,    68,
     905,   368,    28,   908,    43,     8,   638,     3,    80,   117,
     169,    19,   917,    62,   193,    63,     8,  1008,  1009,    98,
     616,    14,   662,    44,    46,   662,   143,    63,    63,    80,
       6,    23,   185,   125,  1647,   125,    28,   664,   125,    56,
      44,   310,   192,   289,   313,    18,   292,   125,   372,   336,
     125,   125,     3,    64,   125,    22,   688,   344,   567,  1079,
     244,   196,   125,  1079,   758,  1576,    77,  1058,   346,    64,
      46,   976,    94,   582,    85,  1570,  1571,   194,   186,  1692,
      76,   310,    77,   125,   313,   712,    94,   286,   167,   100,
     139,   320,   321,   362,   366,   350,   199,   200,   260,   168,
     230,   246,   156,   114,  1439,   100,   175,   125,   180,   189,
     173,   287,   105,  1396,   191,  1797,   177,   346,   814,   114,
     174,   350,   125,   287,   255,    76,   822,   189,   287,   180,
     179,   173,  1416,  1417,   287,  1380,   286,  1421,   155,   174,
     151,  1541,  1542,   412,  1416,  1417,    26,   204,  1455,  1421,
     255,   118,   287,   422,   208,   424,   151,   426,   293,   180,
     429,  1659,   448,   392,   393,  1419,  1924,   439,    23,   441,
    1000,   450,    10,    28,   996,  1166,   180,   959,  1000,    63,
    1183,  1184,   460,   412,   456,    23,   286,   855,  1626,    23,
      28,   933,   574,   422,    28,   424,   288,   426,   288,   190,
     429,   288,   286,   458,  1416,  1417,  1416,  1417,  1462,  1421,
     288,  1421,   278,   288,   288,   286,   286,    21,   283,   448,
     449,   450,   286,   290,  1395,  1375,  1376,  1416,  1417,   458,
    1233,   460,  1421,   449,   463,  1416,  1417,  1154,   286,   278,
    1421,   463,   196,   123,  1149,   281,   282,   283,   284,   126,
     286,  1146,   263,   283,   284,   281,   282,   283,   284,   122,
    1251,  1252,    11,  1254,  1255,  1170,  1171,   196,   263,   896,
       4,  1766,   899,     7,    23,    55,   196,    18,   196,    28,
    1945,  1821,  1370,    87,     5,  1190,   515,   984,   915,   281,
     282,   283,   284,  1950,    15,   286,    45,   177,   995,   196,
      55,   178,   999,     9,  1969,   574,    90,    23,   285,   564,
     680,   196,    28,   940,   194,    49,  1651,   584,   573,   174,
    1655,  1656,   577,  1343,   196,     0,   223,  1343,   282,     6,
     839,  1996,    43,  1795,   117,   564,    61,   566,   567,   293,
    1997,    82,  2007,   143,   573,  2010,  1746,    65,   577,    55,
     566,   623,  2017,   582,   147,   584,  1691,  1445,   627,  1344,
      66,   141,   162,   730,   293,  1690,   286,    44,   640,   287,
    1113,  1879,  1880,   156,   196,   293,  1916,    44,   974,    56,
    1225,  1461,   654,    94,   906,   282,   141,   987,  1642,  1672,
    1635,  1239,   886,   662,  1233,  1702,   176,   282,   627,   681,
      23,   223,  1347,    80,   852,    28,  1689,    11,     7,   874,
     282,   683,    65,    80,  1405,   199,   200,   172,  1958,  1115,
     203,   681,    84,   196,  1121,    64,   281,   282,   283,   284,
     812,   660,   187,   662,  1945,   230,  1427,   666,    77,  1979,
      63,    45,   196,   281,   282,   283,   284,   281,   282,   283,
     284,    65,   286,    44,  1151,   117,   728,    23,  1969,   165,
     282,   100,    28,   735,   736,    23,   827,   262,   145,   738,
      28,    46,  1807,     6,    23,   114,  1811,   287,   155,    28,
     287,   143,    23,   122,   146,  1996,   140,    28,    17,    80,
     860,  1826,   874,    23,    64,   155,  2007,   122,    28,  2010,
     162,    79,    31,   180,     6,    63,  2017,    77,  1843,   738,
     286,     4,   151,    46,  2009,    23,   286,     6,    30,    94,
      28,   291,   182,    16,    23,   142,   188,   122,  1783,    28,
     100,    43,   281,   282,   283,   284,   114,  1708,   232,   231,
      23,   158,   781,   118,    46,    28,    75,     6,   126,   241,
    1177,  1178,   781,   196,   145,  1460,    49,    46,    51,   828,
     124,   830,  1189,   832,   155,   281,   282,   283,   284,  1265,
     286,  1265,   942,   124,   103,     6,  1160,    23,    23,  1833,
     223,   151,    28,    28,   174,  1839,   815,    46,  1923,   180,
    1640,  1926,   847,    23,   823,  1879,  1880,  1492,    28,   828,
     239,   830,   238,   832,    23,   874,    79,  1879,  1880,    28,
     839,   880,    23,   885,   869,    46,   871,    28,   847,   848,
     892,  1425,    85,   286,   263,   854,   180,  1962,   291,   858,
     286,   903,   848,  1329,   156,   291,  1236,   906,   167,    23,
     869,    19,   871,  1227,    28,   108,   196,   192,  1639,   878,
      23,   880,   934,   287,   290,    28,   259,   886,   281,   282,
     283,   284,   298,   932,   933,   288,  1448,  1879,  1880,  1879,
    1880,  1453,   198,   223,   934,   162,   948,   906,    98,   939,
     286,  1527,   288,  1938,   266,  1940,   968,   969,   199,   971,
    1879,  1880,  1388,  1948,    59,    60,   956,  1079,  1879,  1880,
     286,   164,   288,   932,   933,  1305,   231,   232,   968,   969,
      10,   971,   132,    59,   134,   281,   282,   283,   284,    61,
     975,   950,   288,   281,   282,   283,   284,   286,   950,   288,
     985,  1113,   281,   282,   283,   284,   156,    78,  1993,   288,
     281,   282,   283,   284,   130,   131,   975,   288,   203,    79,
     205,   281,   282,   283,   284,    23,   985,   398,   288,   400,
      28,   181,   190,  1783,   993,   994,   118,  1783,  1759,   149,
     286,   993,   288,   281,   282,   283,   284,   286,   172,   288,
     288,   141,   281,   282,   283,   284,   286,   139,   288,   288,
     286,    82,   288,   145,    63,  1489,  1490,    23,   281,   282,
     283,   284,    28,  1497,  1498,   288,  1500,  1501,   287,   286,
    1437,   288,    61,  1555,  1508,  1509,  1510,  1511,    61,  1513,
    1514,  1515,  1516,  1517,  1518,  1064,    23,   179,   195,   196,
     197,    28,   286,  1465,   288,  1064,   281,   282,   283,   284,
    1467,   122,  1212,   288,  1113,  1354,   122,   286,  1357,   288,
     174,   281,   282,   283,   284,   244,   223,   224,   288,   286,
     102,   288,   281,   282,   283,   284,  1383,   188,  1385,   288,
     281,   282,   283,   284,   286,    23,   288,   288,  1107,  1108,
      28,   286,  1154,   288,   286,   172,   288,  1527,   123,   286,
    1527,   288,  1121,     6,   286,   156,   288,   281,   282,   283,
     284,   286,    65,   288,   288,   199,   200,  1898,   281,   282,
     283,   284,  1612,  1613,  1905,   288,    83,   286,  1938,   288,
    1940,    23,  1938,   286,  1940,   288,    28,   121,  1948,   243,
      23,    44,  1948,  1612,  1613,    28,  1965,  1966,   155,   254,
    1222,   254,  1015,    56,  1017,   255,  1019,  1020,    30,  1022,
    1023,  1024,    52,  1235,   207,   286,   192,    70,    23,  1241,
    1229,  1343,  1222,    28,  1233,   112,     0,    80,    29,  1351,
     278,    10,  1201,  1993,    23,  1235,    10,  1993,   287,    28,
     135,  1241,    23,   287,  1206,   287,   777,    28,   779,  1218,
     287,   199,   783,   784,   785,   287,   199,   287,    23,   287,
    1229,   287,   287,    28,  1233,   287,   199,  1368,  1369,  1370,
     287,    97,    23,   281,   282,   283,   284,    28,   148,   290,
     288,   287,   287,   287,   287,   287,   286,    61,   287,    54,
      64,  1401,   145,  1403,  1661,   287,    22,    71,   292,    73,
      23,   287,   155,    77,    30,    28,   287,   287,    82,   287,
     278,    85,    10,   292,   287,   281,   282,   283,   284,    63,
    1358,   141,   288,    23,   286,   286,   100,   180,    28,  1696,
     166,  1301,  1301,   278,   292,  1350,   287,  1356,  1347,  1359,
     114,  1350,  1351,    83,   281,   282,   283,   284,   122,   202,
      23,   288,   126,  1463,    10,    28,   292,   286,    23,    17,
      90,   135,    90,    28,    43,    23,    90,    22,    22,   111,
      28,    10,   174,    31,  1953,  1344,   208,   151,  1347,    23,
     136,  1350,  1351,   174,    28,  1354,  1355,  1356,  1357,  1358,
    1359,    21,   118,   281,   282,   283,   284,   136,    63,  1355,
     288,   196,    90,   185,   178,   185,  1375,  1376,   287,    67,
      91,  1380,   287,   139,   290,   287,   190,    75,   192,   145,
     138,    23,    17,   287,    61,    83,    28,    10,    23,   122,
     149,   286,   128,    28,   121,   287,    31,   287,    61,   281,
     282,   283,   284,   101,   126,   103,   288,   288,   281,   282,
     283,   284,    54,   179,  1576,   288,   189,    21,   116,   293,
     118,   119,   120,    42,  1476,   239,    23,   174,   174,  1481,
      23,    28,    67,  1840,    60,    28,   281,   282,   283,   284,
      75,  1443,    23,   288,   287,   259,   202,    28,    83,   263,
     286,   190,   281,   282,   283,   284,   186,    10,    83,   288,
     281,   282,   283,   284,    61,   240,   101,   288,   103,   167,
     286,   285,   286,    19,   288,  1637,   281,   282,   283,   284,
     287,   116,    23,   118,   119,   120,   287,    28,    17,    82,
     281,   282,   283,   284,    23,  1561,    43,   288,    86,    28,
      27,    82,    31,   210,   211,   212,   213,   214,   215,   216,
     217,    86,    22,  1523,  1523,   125,   125,   125,   281,   282,
     283,   284,   156,   154,   309,   288,   156,  1576,    98,   108,
     287,   286,   167,   287,   287,   287,   286,   288,    67,    63,
      23,   281,   282,   283,   284,   184,    75,   332,   288,   292,
      16,   124,  1561,   107,    83,    63,   341,    63,   287,    71,
    1569,  1570,  1571,   286,    12,   239,    10,  1576,   281,   282,
     283,   284,   101,   287,   103,   288,   281,   282,   283,   284,
     278,   279,   280,   281,   282,   283,   284,   116,   287,   118,
     119,   120,   290,   160,   292,    37,   288,   281,   282,   283,
     284,   289,   286,  1612,  1613,   288,    48,   288,    17,    10,
     288,   133,   397,    55,    23,   192,   401,    59,    52,    28,
      52,  1783,    31,   288,   288,  1634,  1635,  1662,  1637,     8,
     287,   287,    63,   287,  1796,   287,   287,    79,   167,   281,
     282,   283,   284,   278,   279,   280,   281,   282,   283,   284,
     287,   287,    51,  1662,  1795,   290,   173,   292,    67,   287,
     287,   192,   127,   287,    85,   287,    75,   287,    85,   287,
     287,   287,   287,   286,    83,   286,  1734,    19,    73,   130,
     125,   288,   288,   145,   281,   282,   283,   284,   281,   282,
     283,   284,   101,   288,   103,   125,   125,   125,   125,   141,
     281,   282,   283,   284,   196,   125,   288,   116,    22,   118,
     119,   120,   288,   287,    63,   500,   288,   288,   286,   288,
    1824,  1731,  1731,   288,    51,  1734,   173,   512,   513,   514,
     172,    36,    36,   285,   176,   285,   287,    82,    51,    82,
     281,   282,   283,   284,   289,   285,   285,   285,   285,   278,
     279,   280,   281,   282,   283,   284,   174,  1766,   167,   285,
     285,   290,   285,   292,    37,   285,   285,  1825,    70,   285,
     285,   285,   285,    10,   285,    48,  1938,    85,  1940,  1778,
     285,   285,    55,  1945,    23,  1820,  1948,  1796,  1797,    79,
     180,    80,   180,    44,  1846,    68,    80,    44,  1850,   288,
     585,   288,    94,   124,   589,   111,  1815,  1969,   111,   288,
     288,  1820,   288,  1917,   288,   288,  1825,   288,   288,   288,
     288,   288,   189,   265,  1890,   288,    61,   288,   288,   288,
     288,  1993,   288,   287,  1996,   288,   124,   287,    94,   288,
     286,    86,   204,    79,   287,  2007,   246,   287,  2010,    82,
      82,   287,   125,   122,   287,  2017,   173,   122,   122,    47,
      44,   286,   288,  1872,   288,   288,   288,    50,   141,   278,
     279,   280,   281,   282,   283,   284,    36,    47,   201,   288,
     180,  1890,  1891,   580,   157,   160,   160,  1366,   848,   161,
    1892,   582,  1239,   839,   742,  1236,  1945,   988,  1680,   172,
    1228,   993,   318,   176,  1953,   963,   179,   321,   409,    81,
     443,   688,  1163,     7,   187,   370,   880,  1229,  1233,  1679,
    1969,   424,   916,  1659,   424,  1956,  1661,  1960,  1981,  1959,
    1770,    25,  1946,   206,   906,     0,  1945,  1104,  1937,    33,
     828,  1950,  1643,   668,  1953,  1398,    40,  1996,    42,   429,
      44,  1451,   966,   725,   958,    49,  1965,  1966,  2007,    13,
    1969,  2010,    56,   354,   602,   598,   407,   869,  2017,   166,
     856,    48,    91,   758,  1384,   414,    70,  1121,    55,  1910,
     775,  1731,    59,    60,  1523,     0,    80,  1996,  1997,   889,
       0,    68,   265,   778,     0,   846,    38,   782,  2007,  1116,
    2009,  2010,    79,  1280,   789,   610,  1407,  1327,  2017,   794,
     795,   950,  1218,   798,   799,   800,   801,   802,   803,  1815,
     666,    95,  1569,    -1,    66,    -1,    -1,    -1,    -1,   814,
      -1,   816,   817,   818,   819,   820,   821,   822,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    94,    -1,    -1,    -1,   150,    -1,    -1,   153,
      -1,   155,   156,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     157,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,   877,    -1,    -1,   172,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,   889,     6,   891,    -1,     9,    -1,
     187,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    32,    -1,    -1,    -1,   229,    -1,    38,    39,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     244,   245,    -1,   195,   196,   197,    -1,   199,   200,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,   269,   218,   219,   220,   221,
     222,   223,   224,   225,   226,    -1,    -1,    -1,   265,    -1,
      -1,   233,   234,    94,    -1,    96,    97,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1010,  1011,  1012,    -1,    -1,
    1015,  1016,  1017,    -1,  1019,  1020,  1021,  1022,  1023,  1024,
     282,    -1,    -1,    -1,    -1,    -1,  1031,  1032,    -1,  1034,
      -1,  1036,    -1,    -1,  1039,  1040,  1041,  1042,  1043,  1044,
      -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,  1056,    -1,    -1,  1059,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1074,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
    1115,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,   270,
     271,   272,    -1,   274,   275,    -1,   277,    -1,    -1,    -1,
     281,   282,    -1,    -1,    -1,    17,   287,    -1,   289,    -1,
      -1,    23,   293,  1489,  1490,    -1,    28,    -1,    -1,    31,
      -1,  1497,  1498,    -1,  1500,  1501,    -1,    -1,    -1,    -1,
      21,    -1,  1508,  1509,  1510,  1511,    -1,  1513,  1514,  1515,
    1516,  1517,  1518,    -1,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    67,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    75,    55,    -1,    -1,    -1,    59,    60,
      -1,    83,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,   101,
      -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1265,    -1,    -1,    -1,   116,    -1,   118,   119,   120,    -1,
      -1,    -1,    -1,  1278,  1279,    -1,  1281,  1282,    -1,    -1,
      -1,    -1,    -1,    -1,  1289,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,    -1,  1309,    -1,    -1,    -1,    -1,    -1,
     141,    -1,  1317,  1318,  1319,   167,    -1,    -1,    -1,    -1,
    1325,    -1,  1327,  1328,  1329,  1330,    -1,    18,    -1,    -1,
      21,    -1,  1337,    -1,  1339,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    34,    -1,   176,    -1,    38,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,
       6,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    20,   206,   207,    -1,    -1,    -1,
      -1,    -1,    -1,  1388,    -1,    -1,    32,    -1,    -1,    -1,
      81,    -1,    38,    39,    -1,    -1,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1420,    -1,    -1,    -1,    -1,
      66,    -1,   113,    69,   115,    -1,   278,   279,   280,   281,
     282,   283,   284,    -1,   265,    -1,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1488,    -1,    -1,    -1,    -1,    -1,  1494,
      -1,    -1,   183,    -1,  1499,    -1,    -1,    -1,    -1,    -1,
      -1,  1506,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,   218,   219,    -1,
      -1,    -1,    -1,    -1,    -1,   181,  1541,  1542,    -1,    -1,
      -1,    -1,    -1,    -1,   235,  1550,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,  1566,   208,   209,    -1,    39,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,    -1,  1638,    -1,   281,   282,    -1,    -1,    -1,
      -1,   287,    -1,   289,    -1,    -1,    -1,   293,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1711,    -1,    -1,    -1,
      -1,    -1,  1717,  1718,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,  1741,    -1,    -1,    -1,
      -1,  1746,   216,   217,  1749,    -1,    94,    -1,    96,    97,
      -1,  1756,  1757,   227,   228,    -1,   230,    -1,   106,    -1,
      -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,
      -1,    -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,   181,    -1,    -1,    -1,    -1,    -1,    -1,
    1845,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,  1910,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
      -1,    -1,    -1,   281,   282,    -1,    -1,     6,    -1,   287,
       9,   289,  1947,    -1,    13,   293,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   287,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    -1,   274,   275,     6,   277,    -1,
       9,    -1,   281,   282,    13,    -1,    -1,    -1,   287,    -1,
     289,    20,    -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    -1,   274,   275,     6,   277,    -1,
       9,    -1,   281,   282,    13,    -1,    -1,    -1,   287,    -1,
     289,    20,    -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    71,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    -1,   274,   275,     6,   277,    -1,
       9,    -1,   281,   282,    13,    -1,   141,    -1,   287,    -1,
     289,    20,    -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,   187,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    -1,   274,   275,     6,   277,    -1,
       9,    -1,   281,   282,    13,    -1,    -1,    -1,   287,    -1,
     289,    20,    -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   287,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,   108,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    13,   233,   234,    -1,   236,   237,   238,
      20,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    32,    -1,    -1,   254,   255,    -1,    38,    39,
      -1,   260,   261,   262,    -1,   264,   265,   266,   164,   268,
      -1,   270,   271,   272,    -1,   274,   275,    57,   277,    -1,
      -1,    -1,   281,   282,    -1,    -1,    66,    -1,   287,    69,
     289,    -1,    -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    94,    -1,    96,    97,    -1,    -1,
     216,   217,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,   227,   228,    -1,   230,    -1,    -1,    -1,   118,    -1,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,   155,    -1,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    13,
     270,   271,   272,    -1,   274,   275,    20,   277,    -1,    -1,
      -1,   281,   282,    -1,    -1,    -1,    -1,   287,    32,   289,
      -1,    35,    -1,   293,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,   118,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,   287,    -1,   289,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    13,   233,
     234,    -1,   236,   237,   238,    20,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,
     254,   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   270,   271,   272,    -1,
     274,   275,    57,   277,    -1,    -1,    -1,   281,   282,    -1,
      -1,    66,    -1,   287,    69,   289,    -1,   198,    -1,   293,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,   118,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,   289,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
      -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
     255,    -1,    38,    39,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    57,   277,    -1,    -1,    -1,   281,   282,    -1,    -1,
      66,    -1,   287,    69,   289,    -1,   198,    -1,   293,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,   118,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,   287,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
     236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
      -1,    38,    39,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,    -1,    -1,    -1,   281,   282,    -1,   181,    66,
      -1,   287,    69,   289,    -1,    -1,    -1,   293,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    94,    -1,    96,
      97,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,   227,   228,    -1,   230,    -1,    -1,
      -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
     267,   268,    -1,   270,   271,   272,   273,   274,   275,   276,
     277,    13,    -1,    -1,   281,   282,    -1,    -1,    20,    -1,
     287,    -1,   289,    -1,    -1,    -1,   293,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    94,    -1,    96,    97,    -1,    -1,   216,   217,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,   227,
     228,    -1,   230,    -1,    -1,    -1,   118,    -1,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,   285,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      13,   233,   234,    -1,   236,   237,   238,    20,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    32,
      -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,    -1,    -1,    -1,   281,
     282,    -1,    -1,    66,    -1,   287,    69,   289,   196,    -1,
     198,   293,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,   227,
     228,    -1,   230,   106,    -1,    -1,    -1,   110,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   282,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    13,
     233,   234,    -1,   236,   237,   238,    20,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    32,    -1,
      -1,   254,   255,    -1,    38,    39,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,    -1,    -1,    -1,   281,   282,
     283,    -1,    66,    -1,   287,    69,   289,    -1,   198,    -1,
     293,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    13,   233,
     234,    -1,   236,   237,   238,    20,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    32,    -1,    -1,
     254,   255,    -1,    38,    39,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   270,   271,   272,    -1,
     274,   275,    -1,   277,    -1,    -1,    -1,   281,   282,    -1,
      -1,    66,    -1,   287,    69,   289,    -1,   198,    -1,   293,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    13,   233,   234,
      -1,   236,   237,   238,    20,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    32,    -1,    -1,   254,
     255,    -1,    38,    39,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    -1,   277,    -1,    -1,    -1,   281,   282,    -1,    -1,
      66,    -1,   287,    69,   289,    -1,    -1,    -1,   293,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    13,   274,   275,
      -1,   277,    -1,    -1,    20,   281,   282,    -1,    -1,    -1,
      -1,   287,   288,   289,    -1,    -1,    32,   293,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   192,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    13,   270,   271,   272,    -1,   274,   275,
      20,   277,    -1,    -1,    -1,   281,   282,    -1,    -1,    -1,
      -1,   287,    32,   289,    -1,    -1,    -1,   293,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    13,   233,   234,    -1,   236,   237,   238,    20,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,    -1,    -1,
      -1,   281,   282,    -1,    -1,    66,    -1,   287,    69,   289,
      -1,    -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   192,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,   270,
     271,   272,    -1,   274,   275,    -1,   277,    -1,    -1,    -1,
     281,   282,    -1,    -1,    66,    -1,   287,    69,   289,    -1,
      -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      13,   233,   234,    -1,   236,   237,   238,    20,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    32,
      -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,    -1,    -1,    -1,   281,
     282,    -1,    -1,    66,    -1,   287,    69,   289,    -1,    -1,
      -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
     145,   254,   255,    -1,    16,    -1,    -1,   260,   261,   262,
     155,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,    -1,    -1,    -1,   281,   282,
      -1,    -1,    44,    -1,   287,   180,   289,    -1,    -1,    51,
     293,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    74,   208,    -1,    -1,    -1,    -1,    80,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    16,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,   155,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    55,    56,    -1,    58,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    80,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,   137,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,   256,   257,   258,    16,   260,   261,
     262,   155,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    74,    -1,    -1,    -1,    -1,
      -1,    80,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,   256,   257,   258,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,   137,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      18,    -1,    -1,    21,    -1,    -1,    -1,   216,   217,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,
      38,   230,    -1,    41,    -1,    -1,    -1,    -1,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,   256,   257,   258,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,    -1,   115,    18,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,
      -1,    41,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,   183,    -1,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,   113,    -1,   115,    -1,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,   237,
     238,    -1,   240,   241,   144,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,   161,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    18,
      -1,    -1,    21,    -1,    -1,    -1,   216,   217,   218,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    38,
     230,    -1,    41,    -1,    -1,   235,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,
     219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,   235,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,    -1,
      -1,   225,   226,   227,   228,   229,   230,    -1,    -1,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     7,    25,    33,    40,    42,    44,    49,    56,    70,
      80,   143,   150,   153,   155,   156,   180,   229,   244,   245,
     269,   295,   296,   297,   306,   318,   331,   333,   335,   457,
     460,   479,   504,   528,   529,   530,   531,   532,   534,   536,
     537,   541,   558,   559,   565,   569,   572,   573,   574,   629,
     630,   631,   632,   633,   634,   635,   640,    37,    48,    55,
      59,    79,   141,   172,   176,   265,   480,   193,   538,    37,
      48,    55,    68,   125,   141,   157,   172,   176,   179,   187,
     206,   265,   332,   341,   196,   282,   687,   688,    59,    60,
     319,    63,    48,    55,    59,    60,    68,    79,   141,   157,
     172,   176,   187,   206,   265,   505,   141,   260,     6,    44,
      56,    80,   145,   155,   180,   198,   201,   203,   204,   205,
     206,   208,   216,   217,   227,   228,   230,   237,   238,   240,
     241,   243,   246,   247,   248,   249,   250,   254,   255,   260,
     261,   262,   264,   265,   266,   268,   299,   300,   301,   302,
     314,   315,   741,   749,   750,    18,    82,    18,    70,   202,
     307,   308,   538,   231,   232,   579,   580,   581,    68,   168,
     175,   604,   605,   743,   749,    55,   141,   172,   187,   334,
     748,   749,   244,   122,     0,   285,    61,   566,   126,   178,
     622,   498,   202,   207,   209,   210,   211,   212,   213,   214,
     215,   218,   219,   220,   221,   222,   225,   226,   229,   233,
     234,   235,   236,   239,   242,   244,   245,   256,   257,   258,
     259,   263,   267,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   487,   488,   749,   340,   732,   749,    65,   493,
     736,   749,   411,   740,   749,   605,   743,   501,   744,   749,
     494,   735,   749,   147,   539,   199,   200,   360,   362,   686,
     349,   386,   653,   728,   749,   337,   732,   358,   735,     7,
     408,   740,   343,   688,   690,   376,   605,   471,   744,   464,
     747,   749,   359,   741,   358,    11,    45,   625,   196,   689,
      65,   329,   733,   749,   604,   731,   749,   732,    65,   733,
     735,   736,   740,   690,   743,   744,   747,   741,   735,   740,
     287,   461,   140,   287,   646,   647,   646,   122,   286,   122,
     174,   286,   653,   605,   653,   124,   124,   299,   301,   314,
     174,   196,   287,   293,   682,   693,   287,   682,   693,     6,
      46,   582,   721,   576,   581,   735,    79,    84,   117,   143,
     146,   162,   188,   542,   543,   544,   545,   546,   547,   548,
     549,   551,   156,   742,   749,   338,   732,   409,   740,   377,
     605,   465,   747,   748,    21,    37,    48,    55,    59,    60,
      68,    79,   133,   141,   172,   176,   187,   206,   207,   265,
     268,   560,   561,   562,   180,   192,   568,    19,     6,    46,
     722,   259,   628,     4,    16,    49,    51,   499,   500,     4,
      49,   156,   174,   208,   483,   484,   686,   198,   495,   724,
       3,    76,   287,   412,     4,     7,    49,   485,   486,   489,
       3,    76,   473,   266,   162,   540,   199,    98,   132,   134,
     156,   181,   363,   364,   365,    10,   350,   686,    55,   141,
     176,   336,   412,    14,   105,   344,    59,   378,    61,   646,
      79,   320,   724,    78,   190,   612,   636,   724,    13,    20,
      32,    38,    39,    66,    69,    94,    96,    97,   106,   110,
     165,   171,   181,   195,   196,   197,   198,   209,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   228,   233,   234,
     236,   237,   238,   242,   261,   264,   270,   271,   272,   274,
     275,   277,   281,   282,   287,   289,   442,   458,   652,   653,
     675,   676,   677,   678,   680,   681,   682,   683,   684,   685,
     686,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   707,   708,   709,   710,   711,   712,   714,   719,
     723,   724,   742,   749,   387,   417,   462,   463,   653,   149,
     413,   648,   653,   172,   298,   302,   141,   181,   316,   317,
     745,   749,   315,    82,   287,   649,   650,    63,    61,    61,
     122,   122,    63,   244,   535,   287,   675,   575,   575,   576,
     283,   583,   584,   585,   675,   576,   174,   736,   102,   188,
      26,   123,   177,   194,   554,   555,   557,   605,   172,   544,
     637,   638,   653,   654,   742,   686,   412,   378,   646,   123,
     533,   156,    65,    83,   739,   749,   739,   121,   567,   243,
     623,   624,   675,   573,   573,   675,    90,   255,   348,   369,
     370,   254,   255,   254,   500,    30,   357,   468,    30,    43,
     468,   653,   389,   484,    52,   496,   387,   414,   415,   413,
      30,   381,   386,   396,   397,   207,    30,   653,   286,   487,
     491,   653,   749,     5,    15,   474,   475,   502,   192,   278,
     361,   361,   686,   112,   686,    43,   255,   366,   367,   368,
     369,   365,    18,    21,    38,    41,    81,    87,    88,    89,
      91,    92,    93,    95,   113,   115,   144,   161,   183,   218,
     219,   235,   506,   507,   508,   511,   512,   513,   514,   518,
     519,   520,   521,   522,   523,   525,   526,   339,   732,   410,
     740,   472,   744,   413,    29,   345,    90,   686,   287,   605,
      10,   736,   287,   322,   323,   324,   389,   330,   687,   725,
     749,    35,    57,   118,   159,   248,   249,   250,   287,   655,
     657,   658,   659,   660,   662,   663,   664,   665,   666,   667,
     668,   669,   670,   671,   675,   135,   613,   287,   287,   287,
     199,   287,   287,   287,   287,   287,   287,   199,   199,   287,
     189,   675,   715,   717,   287,   287,   287,    97,   287,   287,
     287,   287,   287,   287,   675,   675,   571,   572,   675,   678,
     746,   749,   148,   459,   290,    23,    28,   281,   282,   283,
     284,   286,   287,   292,   507,   512,   519,   278,   286,   288,
     287,    10,   286,   288,   605,   305,   740,   745,   192,   286,
     304,   605,   651,   654,   182,   569,   605,   298,   141,   316,
     748,   675,   288,   675,    63,   587,   286,    10,   586,   223,
     282,   692,   693,   143,   548,   117,   186,   550,   550,   286,
      61,   286,   556,   166,   286,   612,   636,   278,   292,   413,
     287,    10,    94,   564,   686,    83,   292,   563,   648,   286,
     625,   174,    90,   369,   686,    90,    22,   482,    22,    43,
     481,   507,   686,   111,   497,   507,   286,   288,    10,   729,
     749,    27,   382,   383,   384,   506,   507,   653,    62,   139,
     179,   398,   399,   400,   401,   403,   468,   729,   203,   205,
     492,   486,   174,   208,   136,    44,    80,   180,   476,   136,
     477,   223,   282,   692,   693,   690,   686,    21,    90,   368,
     169,   287,   516,   185,   185,   138,   287,   527,    91,   287,
     524,    21,    87,   468,   290,    21,   290,   517,   287,   287,
     185,   287,   524,   686,   412,    61,    10,   686,   686,   379,
     380,   381,   396,   473,   466,   122,   323,   149,   286,    18,
      34,   321,   511,   128,   121,   287,   118,   657,   671,   287,
     118,   251,   252,   253,   656,   657,   672,   675,     8,   125,
      17,    31,    67,    75,    83,   101,   103,   116,   118,   119,
     120,   167,   278,   279,   280,    85,   108,   164,   614,   615,
     622,    46,   721,   675,    46,   283,   721,   735,   675,    46,
     721,    46,   721,    46,   721,   210,   211,   212,   213,   214,
     215,   216,   217,   720,   675,   655,    50,    51,   189,   189,
     713,   675,   675,   655,    61,   675,   675,   675,   675,   675,
     267,   273,   276,   675,   705,   288,   622,   288,   288,   287,
     442,   443,   652,   742,   678,   727,   749,   675,   675,   675,
     675,   675,   675,   288,   678,    39,   283,   728,   682,   463,
     416,   417,    42,   419,   420,   421,   653,   174,   174,   202,
     317,    60,   641,   286,   288,   287,   641,   605,   305,   575,
     288,   287,   588,   589,   590,   591,   595,   596,   597,   598,
     602,   740,   190,   612,   585,   737,   749,   223,   693,   186,
     555,   142,   158,   552,   605,   638,   613,   675,   728,    10,
     379,   466,   564,   739,    83,   624,   240,   627,   675,   686,
      98,   167,   371,   372,   373,   468,   287,   468,   469,   686,
      43,   278,   418,   466,   415,   466,    19,   287,    43,   351,
     468,   382,   466,    86,    86,   647,    22,   653,   490,   507,
     653,   690,   125,   125,   125,   688,   468,   223,   130,   131,
     375,   156,   686,   687,   725,   196,   286,   691,   154,   515,
     688,   527,   282,   692,   351,   509,   510,   692,   156,   509,
     690,   690,   287,   690,   413,   605,   466,    98,   346,   286,
     288,   474,   569,   605,   288,   133,   287,   326,   327,   389,
     324,   287,    19,    94,   325,   330,   730,   749,   569,   569,
     671,     8,   125,   288,     8,   125,   655,   655,   675,   675,
       6,     9,   163,   661,   675,   287,   673,   674,    46,    94,
     118,     6,   661,   675,   675,     6,   661,   675,    17,    31,
      75,   103,   167,     6,   661,   675,     6,   661,   675,   192,
     675,     6,   661,   675,     6,   661,   675,     6,   661,   675,
     108,   287,   628,   675,   675,    10,   675,   288,   675,   286,
     288,   675,   675,   675,   675,   675,   675,    63,    63,   173,
     675,   718,   655,   675,   716,    50,    51,   189,   286,   286,
     286,   735,   288,   288,   288,   288,   288,    63,   288,    63,
     675,   628,   443,   286,   292,   291,   288,   286,   288,   184,
     424,    16,   428,   421,    71,   141,   176,   181,   187,   206,
     309,   310,   311,   312,   313,   745,   309,   124,    63,   174,
     107,   644,   654,   678,   644,    63,    63,   569,   589,   595,
     286,    64,    77,   100,   114,   151,   263,   606,   287,   603,
      71,   608,   143,   194,   553,   622,   466,   288,   569,   564,
     577,   361,    12,   374,   373,   287,   655,   282,   391,   676,
     679,   681,   683,   684,   685,   723,   466,   466,   469,   419,
     468,   468,   469,   647,   647,   239,   402,   287,   469,   468,
     466,    80,   180,    44,   180,    44,    80,    10,   478,   503,
     726,   749,   687,   286,   288,   160,   517,   288,   286,   288,
     469,   286,   291,   289,   726,   291,   288,   288,   690,   288,
      10,   473,   419,   361,   347,   348,   380,   477,   192,   470,
     342,   383,   647,   648,   690,   327,    52,   321,   690,   230,
     262,    52,   288,   288,   655,   655,   655,   655,     8,   287,
     287,   570,   572,   678,    63,    46,    94,   287,   287,    54,
     287,   287,   675,   675,   673,   675,   192,   675,   287,   287,
     287,   287,   675,   287,   287,   287,   287,   287,   287,   614,
     616,   617,   618,   743,   288,   288,   388,   389,   288,   288,
     675,   288,   288,   288,   288,   288,   288,   675,   675,   718,
      51,   173,   173,   718,   716,   675,   678,   675,   675,   675,
      63,   288,   442,   652,   417,   386,   422,   423,   426,   730,
     749,    55,    56,    58,    74,    80,   137,   170,   227,   247,
     256,   257,   258,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   441,   444,   445,   446,
     452,   453,   454,   455,   457,   565,   629,   630,   633,   638,
     639,   707,   738,   749,   745,   740,   744,   745,   747,   741,
     192,   303,   286,   286,   303,   642,   643,   682,   687,   642,
     645,   682,   691,   288,   309,   309,   288,   288,   589,   127,
     607,   607,   606,   607,    85,    85,   678,   586,    19,    73,
     611,   628,   419,   470,   231,   241,   626,   693,   130,   693,
     655,   288,   680,   391,   391,   428,   675,   391,   392,   393,
     394,   397,   145,   402,   625,   655,   125,   125,   125,   125,
     125,   125,   468,   469,   288,   687,   691,   692,   118,   352,
     353,   354,   355,   356,   357,   510,   692,   517,   288,   466,
     474,   428,   693,   468,    22,   467,   287,   288,   686,    19,
     204,   328,   288,   686,   675,   570,   570,   288,   622,   288,
     675,    63,   570,   570,   675,   570,   570,     8,    54,   675,
     570,   570,   570,   570,   570,   570,   570,   570,   570,   570,
     288,   286,    79,   114,   126,   619,   618,   288,   506,   288,
     288,    61,   706,   718,   718,    51,   173,   288,   288,   286,
     288,   288,   675,   507,   285,    36,   732,   246,   285,   287,
     675,   285,   738,   749,   730,   730,   456,    51,   189,   427,
     448,   449,   285,   285,   285,   285,   285,   285,    61,   191,
     285,   285,   285,    82,   285,   285,   285,   285,   285,   289,
      70,   311,   313,   311,   313,   174,   586,    85,   590,   589,
     288,   742,   609,   610,   675,   655,   613,   428,   467,   578,
     375,   288,   469,   469,   469,    23,   385,   394,   118,   139,
     145,   179,   395,   403,   605,    79,   288,   180,    80,   180,
      44,    80,    44,   419,   288,   288,    94,   385,   354,   419,
     477,   469,   375,   478,   124,   468,   111,    97,   230,   517,
     111,   288,   288,   628,   675,   288,   288,   288,   288,   675,
     675,   288,   288,   288,   288,   288,   288,   288,   288,   288,
     288,   616,   287,   736,   675,   288,   718,   675,   288,    43,
     278,   390,   425,    61,   675,   675,   655,   440,   730,     9,
      55,    66,   165,   450,   451,   449,    56,   565,   287,   443,
     124,   642,   592,   742,   590,   122,   239,   599,   600,   601,
     286,   469,   469,   288,   727,    94,    86,   605,   402,   646,
     736,   469,   428,   428,   468,   469,   675,   686,   204,   686,
     620,   736,    79,   621,   288,   391,   391,   287,    82,   288,
      82,   732,   734,   749,   687,    47,   286,   246,    82,   655,
     287,   593,   655,   287,   610,   402,   646,   122,   404,   405,
     406,   469,   478,   469,   288,   286,   287,   565,   443,   173,
     443,   427,   451,   675,   443,   288,   594,   737,   648,   404,
      44,   180,   402,   122,   406,   122,   405,   469,   288,   620,
     620,   288,   427,    82,    10,   447,    47,   286,   288,   288,
     402,   117,   156,   203,   407,   407,   288,    50,   443,    36,
      47,   427,   737,   201,    43,    94,   427,    47,   730,   427,
     427
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

  case 24:

    { prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
    break;

  case 25:

    { yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;

  case 26:

    { yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
    break;

  case 27:

    { yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
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

    { yyval = NULL; }
    break;

  case 43:

    { yyval = make_node (nod_grant_admin, (int) 0, NULL); }
    break;

  case 44:

    { yyval = NULL; }
    break;

  case 45:

    { yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
    break;

  case 46:

    { yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
    break;

  case 47:

    { yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
    break;

  case 48:

    { yyval = make_node (nod_revoke, (int) e_grant_count,
					make_list(yyvsp[-2]), make_list(yyvsp[0]), NULL, yyvsp[-3]); }
    break;

  case 49:

    { yyval = make_node (nod_grant, (int) 0, NULL); }
    break;

  case 50:

    { yyval = NULL; }
    break;

  case 51:

    { yyval = make_node (nod_grant_admin, (int) 0, NULL); }
    break;

  case 52:

    { yyval = NULL; }
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
							  yyvsp[-2], yyvsp[0]); }
    break;

  case 88:

    { yyval = NULL; }
    break;

  case 89:

    { yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
    break;

  case 90:

    { yyval = MAKE_constant ((dsql_str*) Jrd::FUN_scalar_array, CONSTANT_SLONG); }
    break;

  case 91:

    { yyval = MAKE_constant ((dsql_str*) Jrd::FUN_ref_with_null, CONSTANT_SLONG); }
    break;

  case 93:

    { yyval = yyvsp[-1]; }
    break;

  case 94:

    { yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); }
    break;

  case 95:

    { yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
    break;

  case 96:

    { yyval = MAKE_constant ((dsql_str*) Jrd::FUN_reference, CONSTANT_SLONG); }
    break;

  case 97:

    { yyval = MAKE_constant ((dsql_str*) Jrd::FUN_value, CONSTANT_SLONG); }
    break;

  case 98:

    { yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
    break;

  case 99:

    { yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_reference), CONSTANT_SLONG); }
    break;

  case 100:

    { yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_descriptor), CONSTANT_SLONG); }
    break;

  case 101:

    { yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 102:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;

  case 103:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 104:

    { yyval = yyvsp[0]; }
    break;

  case 105:

    { yyval = yyvsp[0]; }
    break;

  case 106:

    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
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

    { yyval = yyvsp[0]; }
    break;

  case 126:

    { yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 127:

    { yyval = make_node (nod_redef_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 128:

    { yyval = make_node (nod_replace_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 129:

    { yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 130:

    { yyval = make_node (nod_unique, 0, NULL); }
    break;

  case 131:

    { yyval = NULL; }
    break;

  case 132:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 134:

    { yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;

  case 135:

    { yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 136:

    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 137:

    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 138:

    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 139:

    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 140:

    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 141:

    { yyval = (dsql_nod*) 0;}
    break;

  case 142:

    { yyval = yyvsp[-1]; }
    break;

  case 143:

    { yyval = NULL; }
    break;

  case 146:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 147:

    { yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 148:

    { yyval = NULL; }
    break;

  case 149:

    { yyval = NULL; }
    break;

  case 150:

    { yyval = yyvsp[0]; }
    break;

  case 151:

    { yyval = NULL; }
    break;

  case 152:

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

  case 220:

    { yyval = yyvsp[0]; }
    break;

  case 221:

    { yyval = NULL; }
    break;

  case 228:

    { yyval = NULL; }
    break;

  case 231:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 232:

    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 233:

    { yyval = make_node (nod_null, (int) 1, NULL); }
    break;

  case 234:

    { yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 236:

    { yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;

  case 237:

    { yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;

  case 238:

    { yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;

  case 239:

    { yyval = yyvsp[0]; }
    break;

  case 240:

    { yyval = NULL ;}
    break;

  case 245:

    { yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 246:

    { yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 247:

    { yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 248:

    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;

  case 249:

    { yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 250:

    { yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 251:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
    break;

  case 252:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
    break;

  case 253:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
    break;

  case 254:

    { yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
    break;

  case 255:

    { yyval = NULL;}
    break;

  case 256:

    { yyval = yyvsp[0];}
    break;

  case 257:

    { yyval = yyvsp[0];}
    break;

  case 258:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
    break;

  case 259:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
    break;

  case 260:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
    break;

  case 261:

    { yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
    break;

  case 262:

    { yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 263:

    { yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 264:

    { yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 265:

    { yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 266:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 267:

    { yyval = NULL; }
    break;

  case 268:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 269:

    { yyval = NULL; }
    break;

  case 271:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 272:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
    break;

  case 274:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 275:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
    break;

  case 276:

    { yyval = yyvsp[0]; }
    break;

  case 277:

    { yyval = yyvsp[0]; }
    break;

  case 278:

    { yyval = (dsql_nod*) NULL; }
    break;

  case 279:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 280:

    { yyval = NULL; }
    break;

  case 282:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 283:

    { yyval = yyvsp[-1]; }
    break;

  case 286:

    { yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
    break;

  case 287:

    { yyval = NULL; }
    break;

  case 288:

    { yyval = NULL; }
    break;

  case 289:

    { yyval = yyvsp[0]; }
    break;

  case 290:

    { yyval = yyvsp[0]; }
    break;

  case 291:

    { yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;

  case 294:

    { yyval = yyvsp[-1]; }
    break;

  case 295:

    { yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;

  case 296:

    { yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;

  case 297:

    { yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;

  case 299:

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

    { yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;

  case 378:

    { lex.beginning = lex.last_token; }
    break;

  case 379:

    { yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;

  case 380:

    { yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 381:

    { yyval = 0; }
    break;

  case 382:

    { yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 383:

    { yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 384:

    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 385:

    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 386:

    { yyval = NULL; }
    break;

  case 387:

    { yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;

  case 388:

    { yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
    break;

  case 389:

    { yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
    break;

  case 390:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
    break;

  case 391:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
    break;

  case 392:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
    break;

  case 393:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
    break;

  case 394:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
    break;

  case 395:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
    break;

  case 396:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
    break;

  case 397:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
    break;

  case 398:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
    break;

  case 399:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
    break;

  case 400:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
    break;

  case 401:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
    break;

  case 402:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
    break;

  case 403:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
    break;

  case 404:

    { yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
    break;

  case 405:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 406:

    { yyval = NULL; }
    break;

  case 407:

    { yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 408:

    { yyval = yyvsp[0]; }
    break;

  case 409:

    { yyval = yyvsp[0]; }
    break;

  case 410:

    { yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 411:

    { yyval = yyvsp[0]; }
    break;

  case 412:

    { yyval = yyvsp[0]; }
    break;

  case 413:

    { yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;

  case 414:

    { yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 415:

    { yyval = make_node (nod_mod_index, (int) e_mod_idx_count, yyvsp[0]); }
    break;

  case 416:

    { yyval = yyvsp[0]; }
    break;

  case 417:

    { yyval = yyvsp[0]; }
    break;

  case 418:

    { yyval = yyvsp[0]; }
    break;

  case 419:

    { yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;

  case 421:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 422:

    { yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;

  case 423:

    { yyval = yyvsp[0]; }
    break;

  case 424:

    { yyval = yyvsp[0]; }
    break;

  case 425:

    {yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;

  case 426:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;

  case 427:

    { yyval = yyvsp[0]; }
    break;

  case 428:

    { yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;

  case 430:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 431:

    { yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 432:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;

  case 433:

    { yyval = yyvsp[0]; }
    break;

  case 434:

    { yyval = yyvsp[0]; }
    break;

  case 435:

    { yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 436:

    { yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 437:

    { yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
    break;

  case 438:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 480:

    { yyval = NULL; }
    break;

  case 481:

    { yyval = NULL; }
    break;

  case 482:

    { yyval = NULL; }
    break;

  case 483:

    { yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;

  case 484:

    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 485:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 486:

    { yyval = make_node (nod_cascade, 0, NULL); }
    break;

  case 487:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 488:

    { yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;

  case 489:

    { yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;

  case 490:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 491:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;

  case 492:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;

  case 493:

    { yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 494:

    { yyval = yyvsp[0]; }
    break;

  case 495:

    { yyval = NULL; }
    break;

  case 496:

    { yyval = yyvsp[0]; }
    break;

  case 497:

    { yyval = NULL; }
    break;

  case 498:

    { yyval = NULL; }
    break;

  case 500:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 501:

    { yyval = yyvsp[0]; }
    break;

  case 502:

    { yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;

  case 503:

    { yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;

  case 504:

    { yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;

  case 505:

    { yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;

  case 506:

    { yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 508:

    { yyval = NULL; }
    break;

  case 510:

    { yyval = NULL; }
    break;

  case 511:

    { yyval = yyvsp[0]; }
    break;

  case 512:

    { yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;

  case 513:

    { yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;

  case 514:

    { yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;

  case 515:

    { yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;

  case 516:

    { yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;

  case 517:

    { yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;

  case 518:

    { yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;

  case 519:

    { yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;

  case 520:

    { yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;

  case 521:

    { yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;

  case 522:

    { yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;

  case 523:

    { yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;

  case 524:

    { yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;

  case 529:

    { lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;

  case 530:

    { lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;

  case 532:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 533:

    { if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;

  case 534:

    { yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 540:

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

  case 541:

    { 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;

  case 542:

    { 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;

  case 543:

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

  case 544:

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

  case 545:

    { 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;

  case 548:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;

  case 549:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;

  case 550:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 551:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 552:

    {
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;

  case 553:

    {
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;

  case 554:

    {
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;

  case 555:

    {
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;

  case 556:

    {
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;

  case 557:

    {
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;

  case 559:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 560:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 561:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 562:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 563:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;

  case 564:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 573:

    { 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;

  case 574:

    {  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;

  case 575:

    {
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;

  case 576:

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

  case 577:

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

  case 580:

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

  case 581:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 582:

    { 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;

  case 583:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 584:

    { yyval = yyvsp[-1]; }
    break;

  case 585:

    { yyval = 0; }
    break;

  case 589:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;

  case 590:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;

  case 591:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;

  case 595:

    { yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;

  case 596:

    { yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 597:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 598:

    { yyval = 0; }
    break;

  case 599:

    { yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;

  case 602:

    { yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;

  case 603:

    { yyval = make_node (nod_rollback, 0, NULL); }
    break;

  case 606:

    { yyval = make_node (nod_commit_retain, 0, NULL); }
    break;

  case 607:

    { yyval = NULL; }
    break;

  case 609:

    { yyval = NULL; }
    break;

  case 610:

    {yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;

  case 612:

    { yyval = NULL; }
    break;

  case 614:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 619:

    { yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;

  case 620:

    { yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;

  case 621:

    { yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;

  case 622:

    { yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;

  case 623:

    { yyval = yyvsp[0];}
    break;

  case 625:

    { yyval = yyvsp[0];}
    break;

  case 626:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 627:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 628:

    { yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;

  case 629:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 630:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 631:

    { yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;

  case 632:

    { yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;

  case 633:

    { yyval = 0; }
    break;

  case 634:

    { yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;

  case 635:

    { yyval = (dsql_nod*) NOD_SHARED; }
    break;

  case 636:

    { yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;

  case 637:

    { yyval = (dsql_nod*) 0; }
    break;

  case 638:

    { yyval = (dsql_nod*) NOD_READ; }
    break;

  case 639:

    { yyval = (dsql_nod*) NOD_WRITE; }
    break;

  case 641:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 642:

    { yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 643:

    { yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;

  case 644:

    { yyval = 0; }
    break;

  case 646:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 647:

    { yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;

  case 648:

    { yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
    break;

  case 649:

    { yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
    break;

  case 650:

    { yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
    break;

  case 651:

    { yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
    break;

  case 652:

    { yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
    break;

  case 653:

    { yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;

  case 654:

    { yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
    break;

  case 655:

    { yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;

  case 656:

    { yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
    break;

  case 657:

    { yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
    break;

  case 658:

    { yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
    break;

  case 659:

    { yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
    break;

  case 660:

    { yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;

  case 661:

    { yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;

  case 662:

    { yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
    break;

  case 663:

    { yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
    break;

  case 664:

    { yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
    break;

  case 665:

    { yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
    break;

  case 666:

    { yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;

  case 667:

    { yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;

  case 668:

    { yyval = yyvsp[0]; }
    break;

  case 670:

    { yyval = NULL; }
    break;

  case 671:

    { yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 672:

    { yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;

  case 673:

    { yyval = NULL; }
    break;

  case 674:

    { yyval = yyvsp[0]; }
    break;

  case 675:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 676:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 677:

    { yyval = NULL; }
    break;

  case 678:

    { yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 679:

    { yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 680:

    { yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 682:

    { yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 683:

    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 685:

    { yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 686:

    { lex.limit_clause = true; }
    break;

  case 687:

    { lex.limit_clause = false; }
    break;

  case 688:

    { lex.first_detection = true; }
    break;

  case 689:

    { lex.first_detection = false; }
    break;

  case 690:

    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;

  case 691:

    { yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;

  case 692:

    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;

  case 693:

    { yyval = 0; }
    break;

  case 694:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;

  case 695:

    { yyval = yyvsp[-2]; }
    break;

  case 696:

    { yyval = yyvsp[-1]; }
    break;

  case 697:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 698:

    { yyval = yyvsp[-1]; }
    break;

  case 699:

    { yyval = yyvsp[0]; }
    break;

  case 700:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 701:

    { yyval = 0; }
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

    { yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 710:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 712:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 717:

    { yyval = yyvsp[-1]; }
    break;

  case 718:

    { yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;

  case 720:

    { yyval = NULL; }
    break;

  case 721:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 722:

    { yyval = NULL; }
    break;

  case 724:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 728:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;

  case 729:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;

  case 730:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;

  case 733:

    { yyval = yyvsp[0]; }
    break;

  case 734:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 735:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;

  case 736:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 737:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 738:

    { yyval = NULL; }
    break;

  case 740:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 741:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;

  case 742:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 743:

    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 744:

    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 745:

    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 746:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 749:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 750:

    { yyval = NULL; }
    break;

  case 752:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 754:

    { yyval = yyvsp[0]; }
    break;

  case 755:

    { yyval = NULL; }
    break;

  case 756:

    { yyval = yyvsp[0]; }
    break;

  case 757:

    { yyval = NULL; }
    break;

  case 758:

    { yyval = yyvsp[0]; }
    break;

  case 759:

    { yyval = NULL; }
    break;

  case 760:

    { yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 761:

    { yyval = 0; }
    break;

  case 762:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 763:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 764:

    { yyval = 0; }
    break;

  case 765:

    { yyval = 0; }
    break;

  case 767:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 768:

    { yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 771:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 772:

    { yyval = make_node (nod_natural, (int) 0, NULL); }
    break;

  case 773:

    { yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;

  case 774:

    { yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 776:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 777:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 778:

    { yyval = 0; }
    break;

  case 779:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 780:

    { yyval = 0; }
    break;

  case 782:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 783:

    { yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 784:

    { yyval = 0; }
    break;

  case 785:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 786:

    { yyval = 0; }
    break;

  case 787:

    { yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;

  case 788:

    { yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;

  case 789:

    { yyval = yyvsp[-1]; }
    break;

  case 790:

    { yyval = 0; }
    break;

  case 791:

    { yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;

  case 792:

    { yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;

  case 793:

    { yyval = NULL; }
    break;

  case 794:

    { yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;

  case 795:

    { yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 798:

    { yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 799:

    { yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 802:

    { yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 803:

    { yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 804:

    { yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 806:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 807:

    { yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;

  case 808:

    { yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;

  case 809:

    { yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 810:

    { yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 811:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 812:

    { yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;

  case 813:

    { yyval = NULL; }
    break;

  case 816:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 817:

    { yyval = yyvsp[0]; }
    break;

  case 818:

    { yyval = NULL; }
    break;

  case 819:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 822:

    { yyval = NULL; }
    break;

  case 823:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 825:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 827:

    { yyval = NULL; }
    break;

  case 828:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 830:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 832:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 833:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;

  case 834:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 836:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 838:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 840:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 841:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 843:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 844:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 845:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 846:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 847:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 849:

    { yyval = yyvsp[-1]; }
    break;

  case 850:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 862:

    { yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 863:

    { yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 864:

    { yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 865:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 866:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 867:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 868:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 869:

    { yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 870:

    { yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 871:

    { yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 872:

    { yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 873:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 874:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 875:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 876:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 877:

    { yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 878:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 879:

    { yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 880:

    { yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 881:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 882:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 883:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 884:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 885:

    { yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 888:

    { yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 889:

    { yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;

  case 890:

    { yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 891:

    { yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 892:

    { yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 893:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 894:

    { yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 895:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 896:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 897:

    { yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 898:

    { yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 899:

    { yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 900:

    { yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 901:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 902:

    { yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 903:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 904:

    { yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;

  case 905:

    { yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;

  case 906:

    { yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;

  case 907:

    { yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;

  case 908:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 909:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;

  case 910:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;

  case 911:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 912:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;

  case 913:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;

  case 915:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 916:

    { yyval = yyvsp[-1]; }
    break;

  case 927:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 928:

    { yyval = yyvsp[0]; }
    break;

  case 929:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 930:

    { yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 931:

    { yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;

  case 932:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 933:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 934:

    {
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 935:

    { yyval = yyvsp[-1]; }
    break;

  case 936:

    { yyval = yyvsp[-1]; }
    break;

  case 940:

    { yyval = make_node (nod_dbkey, 1, NULL); }
    break;

  case 941:

    { yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;

  case 942:

    { 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;

  case 945:

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

  case 946:

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

  case 947:

    { yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;

  case 948:

    { yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 950:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 952:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 953:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;

  case 954:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;

  case 955:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;

  case 956:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 957:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 959:

    { yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;

  case 960:

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

  case 961:

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

  case 962:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;

  case 963:

    { yyval = make_parameter (); }
    break;

  case 964:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 965:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 966:

    { yyval = make_node (nod_current_role, 0, NULL); }
    break;

  case 967:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;

  case 968:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;

  case 969:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;

  case 970:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;

  case 971:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;

  case 972:

    { yyval = yyvsp[0]; }
    break;

  case 973:

    { ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;

  case 975:

    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 976:

    { if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 977:

    { if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 978:

    { if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;

  case 979:

    { if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 981:

    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 982:

    { yyval = yyvsp[0];}
    break;

  case 986:

    { yyval = make_node (nod_agg_count, 0, NULL); }
    break;

  case 987:

    { yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;

  case 988:

    { yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;

  case 989:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;

  case 990:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 991:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;

  case 992:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 993:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 994:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 995:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 996:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 999:

    { yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;

  case 1003:

    { yyval = make_node(nod_length, (int) e_length_count,
					MAKE_constant((dsql_str*)blr_length_bit, CONSTANT_SLONG), yyvsp[-1]); }
    break;

  case 1004:

    { yyval = make_node(nod_length, (int) e_length_count,
					MAKE_constant((dsql_str*)blr_length_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;

  case 1005:

    { yyval = make_node(nod_length, (int) e_length_count,
					MAKE_constant((dsql_str*)blr_length_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;

  case 1006:

    { yyval = make_node(nod_length, (int) e_length_count,
					MAKE_constant((dsql_str*)blr_length_octet, CONSTANT_SLONG), yyvsp[-1]); }
    break;

  case 1009:

    { yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;

  case 1010:

    { yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
    break;

  case 1011:

    { yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;

  case 1012:

    { yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
    break;

  case 1013:

    { yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
    break;

  case 1014:

    { yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
    break;

  case 1015:

    { yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
    break;

  case 1016:

    { yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
    break;

  case 1017:

    { yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
    break;

  case 1018:

    { yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
    break;

  case 1019:

    { yyval = yyvsp[0]; }
    break;

  case 1020:

    { yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;

  case 1021:

    { yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 1022:

    { yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;

  case 1023:

    { yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;

  case 1026:

    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;

  case 1027:

    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;

  case 1028:

    { yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 1031:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 1032:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 1033:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 1034:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 1035:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 1036:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 1037:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 1038:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 1042:

    { yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;

  case 1043:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;

  case 1044:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;

  case 1045:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;

  case 1046:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;

  case 1047:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;

  case 1048:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;

  case 1049:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;

  case 1050:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;

  case 1051:

    { yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;

  case 1056:

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

