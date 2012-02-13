/* A Bison parser, made by GNU Bison 1.875d.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

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
     TRIM = 532,
     RETURNING = 533,
     KW_IGNORE = 534,
     LIMBO = 535,
     UNDO = 536,
     REQUESTS = 537,
     TIMEOUT = 538,
     UPLUS = 539,
     UMINUS = 540
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
#define RETURNING 533
#define KW_IGNORE 534
#define LIMBO 535
#define UNDO 536
#define REQUESTS 537
#define TIMEOUT 538
#define UPLUS 539
#define UMINUS 540




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

#define YYMALLOC gds__alloc
#define YYFREE gds__free

static const char INTERNAL_FIELD_NAME[] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING[] = "";

inline SLONG trigger_type_suffix(const int slot1, const int slot2, const int slot3)
{
	return ((slot1 << 1) | (slot2 << 3) | (slot3 << 5));
}


dsql_nod* DSQL_parse;


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

static void	yyerror(const TEXT*);
static void	yyabandon (SLONG, ISC_STATUS);

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

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   define YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
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
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int DSQL_yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
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
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  191
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   9686

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  302
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  461
/* YYNRULES -- Number of rules. */
#define YYNRULES  1133
/* YYNRULES -- Number of states. */
#define YYNSTATES  2029

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   540

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned short int yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     295,   296,   289,   287,   294,   288,   300,   290,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   297,   293,
     285,   284,   286,   301,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   298,     2,   299,     2,     2,     2,     2,     2,     2,
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
     275,   276,   277,   278,   279,   280,   281,   282,   283,   291,
     292
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
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
     362,   365,   368,   371,   376,   379,   382,   385,   388,   391,
     394,   397,   399,   400,   402,   404,   411,   418,   420,   422,
     423,   424,   426,   427,   432,   433,   435,   437,   440,   449,
     451,   452,   456,   458,   459,   461,   462,   464,   467,   469,
     471,   473,   476,   483,   485,   487,   491,   492,   494,   496,
     497,   499,   501,   504,   508,   513,   516,   519,   523,   524,
     526,   528,   531,   533,   538,   542,   546,   548,   549,   551,
     553,   556,   560,   565,   566,   568,   571,   572,   574,   576,
     582,   588,   592,   595,   596,   598,   602,   604,   606,   613,
     617,   620,   627,   630,   632,   635,   638,   641,   642,   644,
     646,   649,   650,   652,   654,   656,   658,   660,   662,   663,
     665,   667,   670,   673,   675,   677,   683,   686,   690,   693,
     696,   697,   699,   701,   703,   705,   709,   714,   723,   728,
     729,   731,   733,   736,   739,   740,   744,   748,   750,   753,
     756,   759,   768,   777,   786,   795,   799,   800,   805,   806,
     808,   812,   818,   820,   824,   827,   831,   835,   836,   838,
     839,   841,   844,   849,   851,   853,   857,   859,   860,   863,
     866,   867,   874,   876,   878,   882,   884,   887,   888,   890,
     893,   896,   898,   900,   902,   904,   906,   908,   910,   912,
     914,   916,   918,   920,   922,   924,   926,   928,   930,   932,
     934,   936,   938,   941,   945,   947,   951,   960,   970,   976,
     985,   992,   996,   997,  1001,  1004,  1006,  1008,  1012,  1016,
    1024,  1027,  1028,  1030,  1032,  1035,  1039,  1040,  1042,  1045,
    1050,  1052,  1056,  1059,  1062,  1065,  1067,  1069,  1071,  1073,
    1076,  1079,  1085,  1086,  1092,  1094,  1098,  1099,  1102,  1107,
    1108,  1116,  1120,  1121,  1123,  1127,  1131,  1139,  1147,  1148,
    1149,  1150,  1151,  1155,  1156,  1165,  1174,  1183,  1185,  1187,
    1188,  1191,  1193,  1195,  1197,  1199,  1201,  1205,  1209,  1213,
    1217,  1221,  1225,  1231,  1237,  1243,  1249,  1255,  1261,  1264,
    1265,  1270,  1273,  1276,  1280,  1283,  1286,  1290,  1294,  1297,
    1300,  1304,  1306,  1309,  1313,  1317,  1320,  1323,  1326,  1329,
    1333,  1335,  1339,  1343,  1347,  1350,  1353,  1358,  1363,  1368,
    1374,  1379,  1381,  1383,  1385,  1387,  1389,  1391,  1393,  1395,
    1397,  1399,  1401,  1403,  1405,  1407,  1409,  1411,  1413,  1415,
    1417,  1419,  1421,  1423,  1425,  1427,  1429,  1431,  1433,  1435,
    1437,  1439,  1441,  1443,  1445,  1447,  1449,  1451,  1453,  1455,
    1457,  1459,  1461,  1463,  1465,  1468,  1470,  1472,  1474,  1476,
    1478,  1479,  1482,  1485,  1490,  1495,  1501,  1505,  1508,  1509,
    1512,  1513,  1514,  1516,  1519,  1522,  1527,  1531,  1534,  1537,
    1545,  1547,  1548,  1550,  1551,  1554,  1557,  1560,  1563,  1566,
    1569,  1572,  1575,  1578,  1582,  1585,  1588,  1591,  1594,  1596,
    1598,  1600,  1602,  1607,  1613,  1615,  1619,  1621,  1625,  1627,
    1630,  1632,  1634,  1636,  1638,  1640,  1642,  1644,  1646,  1648,
    1650,  1652,  1657,  1662,  1669,  1675,  1679,  1680,  1683,  1686,
    1687,  1691,  1692,  1697,  1699,  1705,  1710,  1712,  1717,  1719,
    1722,  1725,  1727,  1729,  1731,  1734,  1737,  1740,  1743,  1744,
    1748,  1754,  1756,  1758,  1761,  1765,  1767,  1770,  1774,  1775,
    1777,  1779,  1781,  1787,  1793,  1800,  1802,  1804,  1806,  1809,
    1814,  1816,  1817,  1823,  1825,  1826,  1830,  1834,  1836,  1837,
    1840,  1841,  1843,  1844,  1848,  1850,  1851,  1853,  1856,  1858,
    1860,  1862,  1864,  1866,  1868,  1871,  1874,  1876,  1879,  1883,
    1885,  1887,  1891,  1895,  1897,  1900,  1904,  1906,  1909,  1910,
    1914,  1917,  1920,  1924,  1927,  1929,  1931,  1932,  1934,  1936,
    1938,  1942,  1945,  1949,  1950,  1952,  1956,  1961,  1967,  1974,
    1982,  1984,  1986,  1988,  1990,  1992,  1994,  1997,  1999,  2001,
    2003,  2005,  2007,  2009,  2012,  2014,  2016,  2018,  2021,  2023,
    2025,  2029,  2033,  2034,  2037,  2038,  2041,  2042,  2046,  2050,
    2054,  2056,  2061,  2066,  2068,  2078,  2079,  2080,  2081,  2082,
    2086,  2089,  2091,  2092,  2096,  2102,  2106,  2109,  2115,  2118,
    2120,  2122,  2124,  2126,  2128,  2132,  2134,  2138,  2140,  2141,
    2144,  2146,  2150,  2152,  2154,  2156,  2158,  2162,  2169,  2171,
    2172,  2176,  2177,  2179,  2183,  2185,  2187,  2189,  2194,  2200,
    2206,  2208,  2210,  2213,  2218,  2223,  2226,  2230,  2231,  2233,
    2236,  2238,  2240,  2243,  2246,  2249,  2250,  2252,  2253,  2257,
    2258,  2260,  2264,  2266,  2269,  2270,  2273,  2274,  2277,  2278,
    2283,  2285,  2288,  2290,  2292,  2293,  2295,  2299,  2302,  2304,
    2306,  2309,  2311,  2316,  2320,  2322,  2326,  2331,  2332,  2336,
    2337,  2339,  2343,  2347,  2349,  2351,  2352,  2354,  2356,  2361,
    2362,  2365,  2370,  2371,  2381,  2387,  2389,  2391,  2399,  2404,
    2406,  2408,  2417,  2423,  2426,  2431,  2432,  2437,  2439,  2443,
    2447,  2449,  2457,  2465,  2471,  2475,  2476,  2478,  2480,  2482,
    2485,  2486,  2488,  2490,  2492,  2493,  2497,  2499,  2503,  2505,
    2506,  2510,  2512,  2516,  2518,  2522,  2526,  2528,  2530,  2534,
    2536,  2539,  2541,  2545,  2549,  2551,  2554,  2558,  2562,  2566,
    2570,  2572,  2576,  2579,  2581,  2583,  2585,  2587,  2589,  2591,
    2593,  2595,  2597,  2599,  2601,  2605,  2609,  2613,  2617,  2621,
    2625,  2629,  2633,  2640,  2647,  2654,  2661,  2668,  2675,  2682,
    2689,  2696,  2703,  2710,  2717,  2724,  2731,  2738,  2745,  2747,
    2749,  2755,  2762,  2768,  2775,  2779,  2784,  2790,  2797,  2801,
    2806,  2810,  2815,  2819,  2824,  2829,  2835,  2840,  2845,  2849,
    2854,  2856,  2858,  2860,  2862,  2864,  2866,  2868,  2872,  2876,
    2878,  2880,  2882,  2884,  2886,  2888,  2890,  2892,  2894,  2896,
    2899,  2902,  2906,  2910,  2914,  2918,  2922,  2926,  2930,  2934,
    2936,  2938,  2940,  2942,  2946,  2948,  2950,  2952,  2954,  2957,
    2960,  2964,  2965,  2970,  2972,  2976,  2978,  2981,  2983,  2985,
    2987,  2989,  2991,  2993,  2995,  2998,  3001,  3004,  3006,  3008,
    3010,  3012,  3014,  3016,  3018,  3020,  3022,  3024,  3027,  3029,
    3032,  3034,  3036,  3038,  3040,  3042,  3045,  3047,  3049,  3051,
    3053,  3058,  3064,  3070,  3076,  3082,  3088,  3094,  3100,  3106,
    3112,  3118,  3120,  3122,  3129,  3131,  3133,  3135,  3140,  3145,
    3150,  3155,  3157,  3159,  3164,  3169,  3177,  3180,  3181,  3189,
    3196,  3203,  3208,  3210,  3212,  3214,  3219,  3223,  3230,  3232,
    3234,  3241,  3250,  3257,  3259,  3261,  3266,  3273,  3278,  3284,
    3288,  3294,  3299,  3305,  3307,  3309,  3311,  3316,  3323,  3325,
    3327,  3329,  3331,  3333,  3335,  3337,  3339,  3341,  3342,  3344,
    3345,  3347,  3349,  3351,  3353,  3355,  3357,  3359,  3361,  3363,
    3365,  3367,  3369,  3371,  3373,  3375,  3377,  3379,  3381,  3383,
    3385,  3387,  3389,  3391,  3393,  3395,  3397,  3399,  3401,  3403,
    3405,  3407,  3409,  3411,  3413,  3415,  3417,  3419,  3421,  3423,
    3425,  3427,  3429,  3431,  3433,  3435,  3437,  3439,  3441,  3443,
    3445,  3447,  3449,  3451,  3453,  3455,  3457,  3459,  3461,  3463,
    3465,  3467,  3469,  3471
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     303,     0,    -1,   304,    -1,   304,   293,    -1,   489,    -1,
     651,    -1,   569,    -1,   544,    -1,   339,    -1,   326,    -1,
     640,    -1,   512,    -1,   305,    -1,   639,    -1,   466,    -1,
     469,    -1,   341,    -1,   343,    -1,   314,    -1,   545,    -1,
     538,    -1,   575,    -1,   536,    -1,   643,    -1,    40,   699,
      -1,    70,   307,   122,   306,   615,   174,   317,   311,    -1,
      70,   309,   122,   141,   313,   174,   317,   311,    -1,    70,
     322,   174,   324,   312,    -1,   172,    -1,    -1,     6,    -1,
       6,   140,    -1,   308,    -1,   310,    -1,   308,   294,   310,
      -1,    56,    -1,   155,    -1,    80,    -1,    44,    -1,   180,
     657,    -1,   145,   657,    -1,   192,    70,   124,    -1,    -1,
     192,   202,   124,    -1,    -1,   752,    -1,   150,   315,   307,
     122,   306,   615,    63,   317,    -1,   150,   315,   309,   122,
     141,   313,    63,   317,    -1,   150,   316,   322,    63,   324,
      -1,    70,   124,    61,    -1,    -1,   202,   124,    61,    -1,
      -1,   318,    -1,   320,    -1,   319,    -1,   318,   294,   319,
      -1,   318,   294,   321,    -1,   320,   294,   319,    -1,   141,
     752,    -1,   176,   756,    -1,   187,   759,    -1,   206,   753,
      -1,   321,    -1,   320,   294,   321,    -1,   757,    -1,   181,
     757,    -1,    71,   757,    -1,   323,    -1,   322,   294,   323,
      -1,   753,    -1,   325,    -1,   324,   294,   325,    -1,   757,
      -1,   181,   757,    -1,    42,   327,    -1,    60,   337,    -1,
      59,    65,   328,    -1,   736,   330,   149,   334,    52,   698,
     111,   698,    -1,   519,    -1,    18,    -1,    34,   295,   702,
     296,   525,    -1,    -1,   331,    -1,   295,   331,   296,    -1,
     332,    -1,   331,   294,   332,    -1,   398,   329,   333,    -1,
      -1,    19,   230,    -1,    19,   262,    -1,    94,    -1,   335,
      -1,   295,   335,   296,    -1,   398,   329,   336,    -1,   133,
     702,    -1,    -1,    19,    97,    -1,    19,   230,    -1,   204,
      -1,    19,   230,   204,    -1,   745,    78,   338,   128,   338,
      52,   698,   111,   698,    -1,   737,    -1,   699,    -1,    33,
     340,    -1,    55,   345,    -1,   349,   635,    79,   748,   122,
     615,   350,    -1,   141,   415,    -1,   172,   385,    -1,   176,
     480,    -1,   187,   473,    -1,    68,   367,    -1,   265,   367,
      -1,    37,   369,    -1,    48,   357,    -1,   157,   351,    -1,
     206,   368,    -1,   229,   342,    -1,   141,   416,    -1,   172,
     386,    -1,   187,   474,    -1,   176,   481,    -1,    55,   346,
      -1,    33,   125,     7,   344,    -1,   141,   417,    -1,   176,
     482,    -1,    55,   347,    -1,   744,   698,    -1,   744,   698,
      -1,   744,   698,    -1,   744,   698,    -1,   179,    -1,    -1,
     659,    -1,   658,    -1,   392,   295,   476,   686,   477,   296,
      -1,   702,   352,   353,   698,   354,   355,    -1,   105,    -1,
      14,    -1,    -1,    -1,    29,    -1,    -1,    98,   370,   705,
     384,    -1,    -1,   356,    -1,   378,    -1,   356,   378,    -1,
     395,   358,   514,   476,   360,   478,   361,   394,    -1,    10,
      -1,    -1,    43,   476,   399,    -1,   359,    -1,    -1,   362,
      -1,    -1,   363,    -1,   362,   363,    -1,   364,    -1,   365,
      -1,   366,    -1,   118,    94,    -1,    22,   476,   295,   666,
     296,   477,    -1,   747,    -1,   753,    -1,   371,   372,   375,
      -1,    -1,   284,    -1,   698,    -1,    -1,   373,    -1,   374,
      -1,   373,   374,    -1,   132,   370,   702,    -1,    98,   370,
     705,   384,    -1,   181,   698,    -1,   134,   698,    -1,   156,
     112,   698,    -1,    -1,   376,    -1,   377,    -1,   376,   377,
      -1,   378,    -1,    43,    21,   156,   738,    -1,   255,    90,
     698,    -1,   379,   698,   380,    -1,    90,    -1,    -1,   381,
      -1,   382,    -1,   381,   382,    -1,   167,   383,   705,    -1,
      98,   370,   705,   384,    -1,    -1,    12,    -1,    12,   130,
      -1,    -1,   130,    -1,   131,    -1,   615,   387,   295,   388,
     296,    -1,   615,   387,   295,   388,   296,    -1,    59,    90,
     698,    -1,    59,   698,    -1,    -1,   389,    -1,   388,   294,
     389,    -1,   390,    -1,   404,    -1,   395,   393,   360,   478,
     400,   394,    -1,   395,   515,   391,    -1,   395,   391,    -1,
     392,   295,   476,   686,   477,   296,    -1,    27,    19,    -1,
      27,    -1,   514,   476,    -1,   664,   475,    -1,    23,   739,
      -1,    -1,   664,    -1,   664,    -1,   398,   514,    -1,    -1,
     691,    -1,   695,    -1,   696,    -1,   697,    -1,   735,    -1,
     687,    -1,    -1,   401,    -1,   402,    -1,   401,   402,    -1,
     405,   403,    -1,   365,    -1,   366,    -1,   145,   615,   657,
     411,   410,    -1,   179,   410,    -1,   139,    86,   410,    -1,
     405,   406,    -1,    30,   741,    -1,    -1,   407,    -1,   408,
      -1,   409,    -1,   366,    -1,   179,   658,   410,    -1,   139,
      86,   658,   410,    -1,    62,    86,   658,   145,   615,   657,
     411,   410,    -1,   239,   635,    79,   748,    -1,    -1,   412,
      -1,   413,    -1,   413,   412,    -1,   412,   413,    -1,    -1,
     122,   180,   414,    -1,   122,    44,   414,    -1,   203,    -1,
     156,    43,    -1,   156,    94,    -1,   117,   201,    -1,   752,
     419,   420,    10,   475,   426,   435,   477,    -1,   752,   419,
     420,    10,   475,   426,   435,   477,    -1,   752,   419,   420,
      10,   475,   426,   435,   477,    -1,   752,   419,   420,    10,
     475,   426,   435,   477,    -1,   295,   421,   296,    -1,    -1,
     149,   295,   423,   296,    -1,    -1,   422,    -1,   421,   294,
     422,    -1,   396,   515,   476,   425,   478,    -1,   424,    -1,
     423,   294,   424,    -1,   396,   515,    -1,    43,   476,   399,
      -1,   284,   476,   399,    -1,    -1,   427,    -1,    -1,   428,
      -1,   427,   428,    -1,    42,   431,   429,   293,    -1,   430,
      -1,   433,    -1,   395,   515,   432,    -1,   184,    -1,    -1,
      43,   399,    -1,   284,   399,    -1,    -1,   742,    36,    61,
     295,   575,   296,    -1,   438,    -1,   435,    -1,    16,   436,
      51,    -1,   437,    -1,   437,   457,    -1,    -1,   434,    -1,
     437,   434,    -1,   439,   293,    -1,   440,    -1,   649,    -1,
     639,    -1,   643,    -1,   640,    -1,   450,    -1,   466,    -1,
     443,    -1,   446,    -1,   650,    -1,   441,    -1,   442,    -1,
     448,    -1,   461,    -1,   455,    -1,   170,    -1,    58,    -1,
     447,    -1,   453,    -1,   444,    -1,   445,    -1,    55,   744,
      -1,    55,   744,   686,    -1,    55,    -1,    56,   246,   686,
      -1,   454,    61,   575,    82,   452,   456,    47,   434,    -1,
     454,    61,    56,   246,   686,    82,   452,    47,   434,    -1,
      56,   246,   686,    82,   452,    -1,    74,   295,   666,   296,
     173,   434,    50,   434,    -1,    74,   295,   666,   296,   173,
     434,    -1,   137,   686,   449,    -1,    -1,   575,    82,   452,
      -1,   297,   758,    -1,   451,    -1,   663,    -1,   452,   294,
     663,    -1,   452,   294,   451,    -1,   454,   191,   295,   666,
     296,    47,   434,    -1,   750,   297,    -1,    -1,   227,    -1,
     247,    -1,   247,   750,    -1,    10,    36,   742,    -1,    -1,
     458,    -1,   457,   458,    -1,   189,   459,    47,   434,    -1,
     460,    -1,   459,   294,   460,    -1,   165,   699,    -1,    66,
     746,    -1,    55,   744,    -1,     9,    -1,   462,    -1,   464,
      -1,   463,    -1,   256,   742,    -1,   257,   742,    -1,   258,
     465,   742,    82,   452,    -1,    -1,    56,   141,   752,   467,
     468,    -1,   690,    -1,   295,   690,   296,    -1,    -1,   148,
     452,    -1,   148,   295,   452,   296,    -1,    -1,    56,   260,
     470,   420,    10,   426,   435,    -1,   295,   471,   296,    -1,
      -1,   472,    -1,   471,   294,   472,    -1,   424,   284,   694,
      -1,   759,   657,    10,   475,   579,   479,   477,    -1,   759,
     657,    10,   475,   579,   479,   477,    -1,    -1,    -1,    -1,
      -1,   192,    22,   124,    -1,    -1,   756,    61,   615,   483,
     484,   487,   488,   477,    -1,   756,    61,   615,   483,   484,
     487,   488,   477,    -1,   756,    61,   615,   483,   484,   487,
     488,   477,    -1,     3,    -1,    76,    -1,    -1,   485,   486,
      -1,    15,    -1,     5,    -1,    80,    -1,   180,    -1,    44,
      -1,    80,   125,   180,    -1,    80,   125,    44,    -1,   180,
     125,    80,    -1,   180,   125,    44,    -1,    44,   125,    80,
      -1,    44,   125,   180,    -1,    80,   125,   180,   125,    44,
      -1,    80,   125,    44,   125,   180,    -1,   180,   125,    80,
     125,    44,    -1,   180,   125,    44,   125,    80,    -1,    44,
     125,    80,   125,   180,    -1,    44,   125,   180,   125,    80,
      -1,   136,   700,    -1,    -1,    10,   476,   426,   435,    -1,
       7,   490,    -1,    55,   348,    -1,   172,   615,   493,    -1,
     176,   509,    -1,   141,   418,    -1,    37,   506,   507,    -1,
      48,   495,   491,    -1,    79,   501,    -1,   265,   502,    -1,
      59,    65,   503,    -1,   492,    -1,   491,   492,    -1,   156,
     359,   477,    -1,     4,    30,   366,    -1,     4,   366,    -1,
      49,    43,    -1,    49,    30,    -1,   174,   664,    -1,   208,
     398,   515,    -1,   494,    -1,   493,   294,   494,    -1,    49,
     664,   500,    -1,    49,    30,   741,    -1,     4,   390,    -1,
       4,   404,    -1,   497,   664,   136,   702,    -1,   497,   495,
     174,   664,    -1,   497,   499,   208,   498,    -1,   497,   499,
     156,   359,   477,    -1,   497,   499,    49,    43,    -1,   496,
      -1,   761,    -1,   202,    -1,   207,    -1,   209,    -1,   210,
      -1,   211,    -1,   212,    -1,   213,    -1,   214,    -1,   215,
      -1,   218,    -1,   219,    -1,   220,    -1,   221,    -1,   222,
      -1,   225,    -1,   226,    -1,   229,    -1,   233,    -1,   234,
      -1,   235,    -1,   236,    -1,   245,    -1,   242,    -1,   244,
      -1,   256,    -1,   257,    -1,   258,    -1,   259,    -1,   239,
      -1,   263,    -1,   270,    -1,   267,    -1,   271,    -1,   272,
      -1,   269,    -1,   273,    -1,   274,    -1,   275,    -1,   276,
      -1,   277,    -1,     7,    -1,     7,   207,    -1,   515,    -1,
     664,    -1,   664,    -1,   205,    -1,   203,    -1,    -1,   748,
       3,    -1,   748,    76,    -1,   747,   266,   192,   704,    -1,
     747,   266,   192,   223,    -1,   747,   266,   192,   288,   223,
      -1,   736,   504,   505,    -1,    52,   698,    -1,    -1,   111,
     698,    -1,    -1,    -1,   508,    -1,   507,   508,    -1,     4,
     356,    -1,     4,   255,    90,   698,    -1,    49,   255,    90,
      -1,    16,   254,    -1,    51,   254,    -1,   756,   483,   510,
     487,   476,   511,   477,    -1,   484,    -1,    -1,   488,    -1,
      -1,    49,   513,    -1,    55,   744,    -1,    79,   748,    -1,
     141,   752,    -1,   172,   755,    -1,   176,   756,    -1,   187,
     759,    -1,    60,   745,    -1,    48,   743,    -1,    59,    65,
     736,    -1,   157,   702,    -1,   206,   753,    -1,    68,   747,
      -1,   265,   747,    -1,   515,    -1,   516,    -1,   519,    -1,
     522,    -1,   520,   298,   517,   299,    -1,   527,   298,   517,
     299,   525,    -1,   518,    -1,   517,   294,   518,    -1,   704,
      -1,   704,   297,   704,    -1,   520,    -1,   527,   525,    -1,
     526,    -1,   531,    -1,   534,    -1,   235,    -1,   521,    -1,
     161,    -1,    38,    -1,   218,    -1,   219,    -1,    81,    -1,
      92,    -1,    18,   524,   523,   525,    -1,    18,   295,   703,
     296,    -1,    18,   295,   703,   294,   699,   296,    -1,    18,
     295,   294,   699,   296,    -1,   154,   160,   703,    -1,    -1,
     169,   699,    -1,   169,   737,    -1,    -1,    21,   156,   738,
      -1,    -1,   530,   295,   702,   296,    -1,   530,    -1,   530,
     185,   295,   702,   296,    -1,   529,   295,   702,   296,    -1,
     529,    -1,   528,   295,   702,   296,    -1,   183,    -1,    21,
     185,    -1,    87,   185,    -1,    21,    -1,    87,    -1,   115,
      -1,   113,    21,    -1,   113,    87,    -1,    95,   532,    -1,
     533,   532,    -1,    -1,   295,   704,   296,    -1,   295,   704,
     294,   704,   296,    -1,    41,    -1,    88,    -1,    91,   535,
      -1,    93,    91,   535,    -1,   144,    -1,    89,   138,    -1,
     295,   700,   296,    -1,    -1,   549,    -1,   537,    -1,   568,
      -1,   156,    68,   747,   174,   704,    -1,   156,    68,   747,
     174,   223,    -1,   156,    68,   747,   174,   288,   223,    -1,
     539,    -1,   540,    -1,   542,    -1,   244,   760,    -1,   245,
     244,   760,   541,    -1,   123,    -1,    -1,   153,   546,   174,
     543,   760,    -1,   244,    -1,    -1,    25,   546,   547,    -1,
     153,   546,   547,    -1,   193,    -1,    -1,   147,   548,    -1,
      -1,   162,    -1,    -1,   156,   175,   550,    -1,   551,    -1,
      -1,   552,    -1,   551,   552,    -1,   553,    -1,   554,    -1,
     555,    -1,   559,    -1,   560,    -1,   561,    -1,   143,   123,
      -1,   143,   194,    -1,   188,    -1,   117,   188,    -1,    84,
     102,   556,    -1,   556,    -1,   557,    -1,   143,   177,   558,
      -1,   143,    26,   558,    -1,   162,    -1,   162,   172,    -1,
     162,   172,   166,    -1,   186,    -1,   117,   186,    -1,    -1,
     117,    14,   281,    -1,   279,   280,    -1,   266,   282,    -1,
     243,   283,   700,    -1,   146,   564,    -1,   158,    -1,   142,
      -1,    -1,   143,    -1,   194,    -1,   565,    -1,   564,   294,
     565,    -1,   567,   566,    -1,    61,   562,   563,    -1,    -1,
     615,    -1,   567,   294,   615,    -1,   156,   168,    79,   748,
      -1,   269,   122,   570,    83,   574,    -1,   269,   122,   571,
     751,    83,   574,    -1,   269,   122,   572,   751,   573,    83,
     574,    -1,    37,    -1,    48,    -1,   172,    -1,   187,    -1,
     141,    -1,   176,    -1,    59,    65,    -1,    60,    -1,    55,
      -1,    68,    -1,   265,    -1,    79,    -1,   206,    -1,    21,
     156,    -1,   268,    -1,   207,    -1,   133,    -1,   300,   751,
      -1,   698,    -1,    94,    -1,   579,   576,   578,    -1,    61,
     180,   577,    -1,    -1,   121,   659,    -1,    -1,   192,   243,
      -1,    -1,   582,   632,   638,    -1,   582,   632,   638,    -1,
     582,   632,   638,    -1,   583,    -1,   582,   178,   734,   583,
      -1,   582,   178,     6,   583,    -1,   584,    -1,   155,   589,
     592,   593,   597,   622,   618,   621,   623,    -1,    -1,    -1,
      -1,    -1,   590,   591,   586,    -1,   590,   586,    -1,   591,
      -1,    -1,   231,   705,   585,    -1,   231,   295,   686,   296,
     585,    -1,   231,   694,   585,    -1,   232,   705,    -1,   232,
     295,   586,   686,   296,    -1,   232,   694,    -1,    46,    -1,
     733,    -1,   594,    -1,   289,    -1,   595,    -1,   594,   294,
     595,    -1,   686,    -1,   686,   596,   749,    -1,    10,    -1,
      -1,    63,   598,    -1,   599,    -1,   598,   294,   599,    -1,
     605,    -1,   600,    -1,   612,    -1,   601,    -1,   295,   605,
     296,    -1,   295,   579,   296,   596,   602,   603,    -1,   754,
      -1,    -1,   295,   604,   296,    -1,    -1,   749,    -1,   604,
     294,   749,    -1,   606,    -1,   607,    -1,   608,    -1,   599,
     263,    85,   600,    -1,   599,   114,   616,    85,   600,    -1,
     599,   616,    85,   599,   609,    -1,   610,    -1,   611,    -1,
     122,   666,    -1,   239,   295,   659,   296,    -1,   752,   613,
     596,   754,    -1,   752,   613,    -1,   295,   690,   296,    -1,
      -1,   615,    -1,   755,   754,    -1,   755,    -1,    77,    -1,
     100,   617,    -1,   151,   617,    -1,    64,   617,    -1,    -1,
     127,    -1,    -1,    71,    19,   619,    -1,    -1,   620,    -1,
     619,   294,   620,    -1,   686,    -1,    73,   666,    -1,    -1,
     190,   666,    -1,    -1,   135,   624,    -1,    -1,   625,   295,
     626,   296,    -1,    85,    -1,   164,   108,    -1,   108,    -1,
     164,    -1,    -1,   627,    -1,   627,   294,   626,    -1,   628,
     629,    -1,   624,    -1,   755,    -1,   755,   628,    -1,   114,
      -1,    79,   295,   630,   296,    -1,   126,   748,   631,    -1,
     748,    -1,   748,   294,   630,    -1,    79,   295,   630,   296,
      -1,    -1,   126,    19,   633,    -1,    -1,   634,    -1,   633,
     294,   634,    -1,   686,   635,   637,    -1,    11,    -1,    45,
      -1,    -1,   231,    -1,   241,    -1,   240,   587,   636,   588,
      -1,    -1,   259,   686,    -1,   259,   686,   174,   686,    -1,
      -1,    80,    82,   615,   660,   182,   295,   690,   296,   646,
      -1,    80,    82,   615,   660,   579,    -1,   641,    -1,   642,
      -1,    44,    63,   614,   622,   623,   632,   638,    -1,    44,
      63,   614,   647,    -1,   644,    -1,   645,    -1,   180,   614,
     156,   648,   622,   623,   632,   638,    -1,   180,   614,   156,
     648,   647,    -1,   278,   690,    -1,   278,   690,    82,   452,
      -1,    -1,   190,    35,   121,   742,    -1,   649,    -1,   648,
     294,   649,    -1,   665,   284,   686,    -1,   719,    -1,   143,
      18,   664,    63,   615,   652,   655,    -1,    80,    18,   664,
      82,   615,   652,   655,    -1,    60,    63,   653,   174,   653,
      -1,    60,   174,   653,    -1,    -1,   654,    -1,   694,    -1,
     699,    -1,   107,   656,    -1,    -1,   703,    -1,   694,    -1,
     658,    -1,    -1,   295,   659,   296,    -1,   664,    -1,   659,
     294,   664,    -1,   661,    -1,    -1,   295,   662,   296,    -1,
     665,    -1,   662,   294,   665,    -1,   664,    -1,   754,   300,
     740,    -1,   754,   300,   289,    -1,   740,    -1,   664,    -1,
     754,   300,   740,    -1,   682,    -1,   118,   682,    -1,   668,
      -1,   666,   125,   666,    -1,   666,     8,   666,    -1,   668,
      -1,   118,   682,    -1,   667,   125,   666,    -1,   667,     8,
     666,    -1,   683,   125,   666,    -1,   683,     8,   666,    -1,
     669,    -1,   295,   667,   296,    -1,   118,   668,    -1,   670,
      -1,   673,    -1,   674,    -1,   675,    -1,   676,    -1,   681,
      -1,   671,    -1,   679,    -1,   677,    -1,   678,    -1,   680,
      -1,   686,   284,   686,    -1,   686,   285,   686,    -1,   686,
     286,   686,    -1,   686,    67,   686,    -1,   686,   101,   686,
      -1,   686,   119,   686,    -1,   686,   120,   686,    -1,   686,
     116,   686,    -1,   686,   284,     6,   295,   580,   296,    -1,
     686,   285,     6,   295,   580,   296,    -1,   686,   286,     6,
     295,   580,   296,    -1,   686,    67,     6,   295,   580,   296,
      -1,   686,   101,     6,   295,   580,   296,    -1,   686,   119,
       6,   295,   580,   296,    -1,   686,   120,     6,   295,   580,
     296,    -1,   686,   116,     6,   295,   580,   296,    -1,   686,
     284,   672,   295,   580,   296,    -1,   686,   285,   672,   295,
     580,   296,    -1,   686,   286,   672,   295,   580,   296,    -1,
     686,    67,   672,   295,   580,   296,    -1,   686,   101,   672,
     295,   580,   296,    -1,   686,   119,   672,   295,   580,   296,
      -1,   686,   120,   672,   295,   580,   296,    -1,   686,   116,
     672,   295,   580,   296,    -1,   163,    -1,     9,    -1,   686,
      83,    46,    63,   686,    -1,   686,    83,   118,    46,    63,
     686,    -1,   686,    17,   686,     8,   686,    -1,   686,   118,
      17,   686,     8,   686,    -1,   686,   103,   686,    -1,   686,
     118,   103,   686,    -1,   686,   103,   686,    54,   686,    -1,
     686,   118,   103,   686,    54,   686,    -1,   686,    75,   684,
      -1,   686,   118,    75,   684,    -1,   686,    31,   686,    -1,
     686,   118,    31,   686,    -1,   686,   167,   686,    -1,   686,
     118,   167,   686,    -1,   686,   167,   192,   686,    -1,   686,
     118,   167,   192,   686,    -1,    57,   295,   579,   296,    -1,
     159,   295,   579,   296,    -1,   686,    83,    94,    -1,   686,
      83,   118,    94,    -1,   248,    -1,   249,    -1,   250,    -1,
     251,    -1,   252,    -1,   253,    -1,   685,    -1,   295,   690,
     296,    -1,   295,   580,   296,    -1,   663,    -1,   689,    -1,
     706,    -1,   693,    -1,   694,    -1,   451,    -1,   720,    -1,
     721,    -1,   731,    -1,   719,    -1,   288,   686,    -1,   287,
     686,    -1,   686,   287,   686,    -1,   686,    28,   686,    -1,
     686,    23,   739,    -1,   686,   288,   686,    -1,   686,   289,
     686,    -1,   686,   290,   686,    -1,   295,   686,   296,    -1,
     295,   581,   296,    -1,   695,    -1,   696,    -1,   697,    -1,
      39,    -1,   754,   300,    39,    -1,    97,    -1,   687,    -1,
     735,    -1,   220,    -1,   221,   688,    -1,   222,   688,    -1,
     295,   700,   296,    -1,    -1,   663,   298,   690,   299,    -1,
     686,    -1,   690,   294,   686,    -1,   693,    -1,   288,   692,
      -1,   197,    -1,   196,    -1,   195,    -1,   223,    -1,   224,
      -1,   692,    -1,   698,    -1,    38,   199,    -1,   218,   199,
      -1,   219,   199,    -1,   301,    -1,   181,    -1,   225,    -1,
     226,    -1,   233,    -1,   234,    -1,    66,    -1,   165,    -1,
     242,    -1,   199,    -1,   200,   199,    -1,   700,    -1,   288,
     701,    -1,   196,    -1,   196,    -1,   700,    -1,   196,    -1,
     705,    -1,   288,   705,    -1,   196,    -1,   707,    -1,   708,
      -1,   714,    -1,    32,   295,   289,   296,    -1,    32,   295,
     733,   686,   296,    -1,    32,   295,    46,   686,   296,    -1,
     171,   295,   733,   686,   296,    -1,   171,   295,    46,   686,
     296,    -1,    13,   295,   733,   686,   296,    -1,    13,   295,
      46,   686,   296,    -1,   110,   295,   733,   686,   296,    -1,
     110,   295,    46,   686,   296,    -1,   106,   295,   733,   686,
     296,    -1,   106,   295,    46,   686,   296,    -1,   709,    -1,
     710,    -1,   209,   295,   732,    63,   686,   296,    -1,   711,
      -1,   712,    -1,   713,    -1,   270,   295,   686,   296,    -1,
     271,   295,   686,   296,    -1,   272,   295,   686,   296,    -1,
     275,   295,   686,   296,    -1,   715,    -1,   717,    -1,    96,
     295,   686,   296,    -1,   274,   295,   686,   296,    -1,   228,
     295,   686,    63,   686,   716,   296,    -1,    61,   686,    -1,
      -1,   277,   295,   718,   686,    63,   686,   296,    -1,   277,
     295,   686,    63,   686,   296,    -1,   277,   295,   718,    63,
     686,   296,    -1,   277,   295,   686,   296,    -1,   267,    -1,
     276,    -1,   273,    -1,   736,   295,   690,   296,    -1,   736,
     295,   296,    -1,    20,   295,   686,    10,   397,   296,    -1,
     722,    -1,   723,    -1,   237,   295,   686,   294,   686,   296,
      -1,   261,   295,   666,   294,   686,   294,   686,   296,    -1,
     238,   295,   686,   294,   690,   296,    -1,   724,    -1,   726,
      -1,   236,   729,   725,    51,    -1,   236,   729,   725,    50,
     730,    51,    -1,   189,   728,   173,   730,    -1,   725,   189,
     728,   173,   730,    -1,   236,   727,    51,    -1,   236,   727,
      50,   730,    51,    -1,   189,   666,   173,   730,    -1,   727,
     189,   666,   173,   730,    -1,   686,    -1,   686,    -1,   686,
      -1,   264,    97,    61,   747,    -1,    69,   295,   747,   294,
     686,   296,    -1,   210,    -1,   211,    -1,   212,    -1,   213,
      -1,   214,    -1,   215,    -1,   216,    -1,   217,    -1,     6,
      -1,    -1,    46,    -1,    -1,    94,    -1,   198,    -1,   761,
      -1,   761,    -1,   761,    -1,   761,    -1,   761,    -1,   761,
      -1,   761,    -1,   761,    -1,   761,    -1,   761,    -1,   761,
      -1,   761,    -1,   761,    -1,   761,    -1,   761,    -1,   761,
      -1,   761,    -1,   761,    -1,   761,    -1,   761,    -1,   761,
      -1,   761,    -1,   761,    -1,   761,    -1,   198,    -1,   762,
      -1,   201,    -1,   203,    -1,   204,    -1,   205,    -1,   206,
      -1,   208,    -1,   227,    -1,   230,    -1,   228,    -1,   238,
      -1,   241,    -1,   247,    -1,   243,    -1,   237,    -1,   240,
      -1,   246,    -1,   248,    -1,   249,    -1,   250,    -1,   260,
      -1,   254,    -1,   255,    -1,   261,    -1,   262,    -1,   216,
      -1,   217,    -1,   265,    -1,   264,    -1,   266,    -1,   268,
      -1,   278,    -1,   279,    -1,   280,    -1,   281,    -1,   282,
      -1,   283,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   559,   559,   561,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   593,   597,   601,   606,   607,
     610,   612,   614,   618,   619,   623,   627,   629,   631,   633,
     635,   639,   642,   645,   648,   651,   658,   662,   666,   671,
     674,   677,   680,   683,   684,   687,   688,   690,   692,   696,
     698,   700,   702,   706,   707,   714,   716,   718,   722,   723,
     727,   731,   732,   736,   738,   745,   749,   751,   756,   762,
     763,   765,   772,   773,   774,   778,   779,   785,   791,   792,
     794,   796,   800,   801,   805,   808,   814,   815,   817,   819,
     822,   827,   833,   836,   842,   846,   848,   851,   853,   855,
     857,   859,   861,   863,   865,   867,   869,   878,   882,   884,
     886,   888,   894,   898,   902,   904,   910,   917,   922,   927,
     932,   940,   943,   946,   948,   949,   955,   961,   963,   966,
     970,   971,   976,   977,   982,   983,   986,   987,   994,  1012,
    1015,  1018,  1022,  1024,  1027,  1029,  1032,  1033,  1037,  1041,
    1042,  1045,  1049,  1057,  1064,  1129,  1134,  1135,  1138,  1143,
    1144,  1147,  1148,  1152,  1154,  1156,  1158,  1160,  1165,  1166,
    1169,  1170,  1174,  1175,  1177,  1181,  1187,  1191,  1192,  1195,
    1196,  1199,  1201,  1205,  1206,  1207,  1210,  1211,  1212,  1218,
    1223,  1228,  1230,  1233,  1236,  1237,  1241,  1242,  1249,  1253,
    1257,  1266,  1272,  1273,  1276,  1278,  1283,  1286,  1290,  1296,
    1302,  1307,  1312,  1313,  1314,  1315,  1316,  1317,  1321,  1322,
    1325,  1326,  1330,  1335,  1336,  1337,  1342,  1344,  1352,  1356,
    1359,  1362,  1363,  1364,  1365,  1368,  1372,  1376,  1383,  1391,
    1396,  1398,  1400,  1402,  1405,  1408,  1411,  1415,  1418,  1421,
    1424,  1433,  1444,  1454,  1464,  1474,  1477,  1480,  1483,  1486,
    1487,  1491,  1497,  1498,  1502,  1507,  1509,  1512,  1515,  1518,
    1521,  1522,  1526,  1530,  1531,  1534,  1539,  1542,  1545,  1547,
    1550,  1553,  1558,  1559,  1562,  1566,  1568,  1571,  1574,  1575,
    1579,  1580,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,
    1591,  1592,  1593,  1594,  1595,  1596,  1597,  1599,  1603,  1604,
    1605,  1606,  1609,  1611,  1615,  1619,  1623,  1628,  1632,  1636,
    1638,  1642,  1649,  1652,  1657,  1662,  1663,  1664,  1666,  1670,
    1674,  1677,  1680,  1682,  1684,  1689,  1693,  1696,  1697,  1701,
    1706,  1707,  1711,  1713,  1715,  1717,  1721,  1722,  1723,  1726,
    1730,  1734,  1739,  1781,  1786,  1788,  1791,  1794,  1796,  1799,
    1804,  1812,  1815,  1818,  1819,  1823,  1829,  1836,  1860,  1870,
    1874,  1879,  1885,  1889,  1896,  1906,  1916,  1926,  1928,  1931,
    1934,  1938,  1940,  1944,  1946,  1948,  1950,  1952,  1954,  1956,
    1958,  1960,  1962,  1964,  1966,  1968,  1970,  1972,  1976,  1979,
    1982,  1988,  1992,  1994,  2001,  2003,  2005,  2008,  2011,  2013,
    2015,  2019,  2020,  2024,  2026,  2028,  2030,  2032,  2034,  2036,
    2040,  2041,  2045,  2047,  2049,  2051,  2057,  2060,  2062,  2064,
    2067,  2072,  2080,  2081,  2082,  2083,  2084,  2085,  2086,  2087,
    2088,  2089,  2090,  2091,  2092,  2093,  2094,  2095,  2096,  2097,
    2098,  2099,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,
    2108,  2109,  2110,  2111,  2112,  2113,  2114,  2115,  2117,  2118,
    2119,  2121,  2122,  2125,  2127,  2131,  2133,  2138,  2144,  2146,
    2149,  2152,  2154,  2158,  2161,  2164,  2169,  2173,  2176,  2179,
    2182,  2189,  2192,  2193,  2197,  2199,  2201,  2203,  2205,  2212,
    2222,  2224,  2227,  2229,  2234,  2238,  2240,  2242,  2244,  2246,
    2248,  2250,  2252,  2254,  2256,  2258,  2260,  2262,  2269,  2270,
    2273,  2274,  2277,  2282,  2289,  2290,  2294,  2303,  2309,  2310,
    2313,  2314,  2315,  2316,  2333,  2338,  2343,  2362,  2379,  2386,
    2387,  2394,  2399,  2406,  2413,  2422,  2427,  2432,  2436,  2441,
    2446,  2450,  2457,  2463,  2469,  2477,  2482,  2487,  2494,  2495,
    2496,  2499,  2500,  2503,  2504,  2505,  2512,  2516,  2528,  2533,
    2587,  2649,  2650,  2657,  2670,  2675,  2680,  2687,  2690,  2696,
    2697,  2698,  2702,  2705,  2708,  2716,  2717,  2718,  2721,  2725,
    2729,  2732,  2735,  2739,  2740,  2743,  2747,  2751,  2752,  2755,
    2758,  2761,  2763,  2766,  2770,  2772,  2775,  2776,  2780,  2781,
    2782,  2783,  2784,  2785,  2788,  2790,  2794,  2796,  2800,  2802,
    2805,  2807,  2809,  2813,  2815,  2817,  2821,  2823,  2826,  2829,
    2831,  2833,  2837,  2841,  2845,  2847,  2850,  2853,  2855,  2859,
    2860,  2864,  2868,  2871,  2874,  2875,  2880,  2884,  2886,  2888,
    2892,  2896,  2898,  2900,  2902,  2904,  2906,  2908,  2910,  2912,
    2914,  2916,  2918,  2920,  2922,  2930,  2932,  2936,  2940,  2941,
    2948,  2952,  2955,  2958,  2961,  2964,  2967,  2973,  2977,  2982,
    2987,  2988,  2990,  2994,  2997,  3010,  3014,  3018,  3022,  3025,
    3027,  3029,  3032,  3035,  3037,  3039,  3043,  3045,  3047,  3051,
    3053,  3057,  3059,  3063,  3064,  3068,  3069,  3073,  3074,  3079,
    3083,  3084,  3088,  3089,  3092,  3093,  3094,  3100,  3104,  3106,
    3109,  3112,  3115,  3116,  3120,  3121,  3122,  3125,  3130,  3135,
    3139,  3140,  3143,  3147,  3151,  3154,  3159,  3162,  3165,  3166,
    3171,  3176,  3178,  3180,  3182,  3185,  3188,  3189,  3195,  3198,
    3201,  3202,  3208,  3211,  3214,  3217,  3220,  3226,  3229,  3232,
    3236,  3238,  3240,  3246,  3249,  3252,  3253,  3257,  3259,  3262,
    3263,  3267,  3269,  3271,  3275,  3276,  3280,  3283,  3288,  3291,
    3294,  3295,  3299,  3303,  3305,  3308,  3311,  3313,  3317,  3320,
    3325,  3328,  3337,  3343,  3347,  3355,  3356,  3359,  3365,  3373,
    3374,  3377,  3383,  3389,  3392,  3396,  3399,  3406,  3407,  3411,
    3415,  3422,  3424,  3428,  3430,  3433,  3436,  3437,  3440,  3444,
    3447,  3450,  3452,  3458,  3460,  3463,  3467,  3468,  3473,  3475,
    3478,  3482,  3483,  3488,  3489,  3492,  3497,  3502,  3505,  3512,
    3513,  3515,  3516,  3518,  3522,  3523,  3525,  3527,  3531,  3533,
    3537,  3538,  3540,  3544,  3545,  3546,  3547,  3548,  3549,  3550,
    3551,  3552,  3553,  3554,  3559,  3561,  3563,  3565,  3567,  3569,
    3571,  3573,  3579,  3581,  3583,  3585,  3587,  3589,  3591,  3593,
    3595,  3597,  3599,  3601,  3603,  3605,  3607,  3609,  3613,  3614,
    3620,  3622,  3626,  3628,  3633,  3635,  3637,  3639,  3644,  3646,
    3650,  3652,  3656,  3658,  3660,  3662,  3666,  3670,  3674,  3676,
    3680,  3685,  3690,  3697,  3702,  3707,  3716,  3717,  3721,  3728,
    3729,  3730,  3731,  3732,  3733,  3734,  3735,  3736,  3737,  3738,
    3740,  3742,  3749,  3751,  3753,  3760,  3767,  3774,  3776,  3778,
    3779,  3780,  3781,  3783,  3785,  3789,  3790,  3793,  3809,  3825,
    3829,  3832,  3835,  3839,  3840,  3844,  3845,  3849,  3851,  3853,
    3855,  3857,  3861,  3862,  3864,  3880,  3896,  3900,  3904,  3906,
    3910,  3914,  3917,  3920,  3923,  3926,  3931,  3933,  3938,  3939,
    3943,  3950,  3957,  3964,  3971,  3972,  3976,  3983,  3984,  3985,
    3988,  3990,  3992,  3995,  4002,  4011,  4018,  4027,  4029,  4031,
    4033,  4037,  4038,  4041,  4045,  4046,  4047,  4050,  4055,  4058,
    4063,  4068,  4069,  4070,  4072,  4076,  4085,  4088,  4091,  4093,
    4096,  4098,  4103,  4105,  4107,  4111,  4113,  4117,  4123,  4124,
    4127,  4131,  4134,  4138,  4139,  4142,  4144,  4148,  4150,  4154,
    4156,  4160,  4162,  4166,  4169,  4172,  4177,  4186,  4196,  4198,
    4200,  4202,  4204,  4206,  4208,  4210,  4214,  4215,  4218,  4219,
    4222,  4230,  4233,  4236,  4239,  4242,  4245,  4248,  4251,  4254,
    4257,  4260,  4263,  4266,  4269,  4272,  4275,  4278,  4281,  4284,
    4287,  4290,  4293,  4296,  4299,  4302,  4307,  4308,  4314,  4315,
    4316,  4317,  4318,  4319,  4320,  4321,  4322,  4323,  4324,  4325,
    4326,  4327,  4328,  4329,  4330,  4331,  4332,  4334,  4336,  4337,
    4338,  4340,  4342,  4343,  4344,  4345,  4346,  4347,  4348,  4349,
    4350,  4351,  4352,  4353
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
  "COMMA", "COMMIT", "COMMITTED", "COMPUTED", "CONCATENATE", "CONDITIONAL",
  "CONSTRAINT", "CONTAINING", "COUNT", "CREATE", "CSTRING", "CURRENT",
  "CURSOR", "DATABASE", "DATE", "DB_KEY", "KW_DEBUG", "DECIMAL", "DECLARE",
  "DEFAULT", "KW_DELETE", "DESC", "DISTINCT", "DO", "DOMAIN", "DROP",
  "ELSE", "END", "ENTRY_POINT", "EQL", "ESCAPE", "EXCEPTION", "EXECUTE",
  "EXISTS", "EXIT", "EXTERNAL", "FILTER", "FOR", "FOREIGN", "FROM", "FULL",
  "FUNCTION", "GDSCODE", "GEQ", "GENERATOR", "GEN_ID", "GRANT", "GROUP",
  "GTR", "HAVING", "IF", "KW_IN", "INACTIVE", "INNER", "INPUT_TYPE",
  "INDEX", "INSERT", "INTEGER", "INTO", "IS", "ISOLATION", "JOIN", "KEY",
  "KW_CHAR", "KW_DEC", "KW_DOUBLE", "KW_FILE", "KW_FLOAT", "KW_INT",
  "KW_LONG", "KW_NULL", "KW_NUMERIC", "KW_UPPER", "KW_VALUE", "LENGTH",
  "LPAREN", "LEFT", "LEQ", "LEVEL", "LIKE", "LSS", "MANUAL", "MAXIMUM",
  "MAX_SEGMENT", "MERGE", "MESSAGE", "MINIMUM", "MODULE_NAME", "NAMES",
  "NATIONAL", "NATURAL", "NCHAR", "NEQ", "NO", "NOT", "NOT_GTR", "NOT_LSS",
  "OF", "ON", "ONLY", "OPTION", "OR", "ORDER", "OUTER", "OUTPUT_TYPE",
  "OVERFLOW", "PAGE", "PAGES", "KW_PAGE_SIZE", "PARAMETER", "PASSWORD",
  "PLAN", "POSITION", "POST_EVENT", "PRECISION", "PRIMARY", "PRIVILEGES",
  "PROCEDURE", "PROTECTED", "READ", "REAL", "REFERENCES", "RESERVING",
  "RETAIN", "RETURNING_VALUES", "RETURNS", "REVOKE", "RIGHT", "RPAREN",
  "ROLLBACK", "SEGMENT", "SELECT", "SET", "SHADOW", "KW_SHARED",
  "SINGULAR", "KW_SIZE", "SMALLINT", "SNAPSHOT", "SOME", "SORT", "SQLCODE",
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
  "OCTET_LENGTH", "TRAILING", "TRIM", "RETURNING", "KW_IGNORE", "LIMBO",
  "UNDO", "REQUESTS", "TIMEOUT", "'='", "'<'", "'>'", "'+'", "'-'", "'*'",
  "'/'", "UPLUS", "UMINUS", "';'", "','", "'('", "')'", "':'", "'['",
  "']'", "'.'", "'?'", "$accept", "top", "statement", "grant",
  "table_noise", "privileges", "privilege_list", "proc_privileges",
  "privilege", "grant_option", "role_admin_option", "simple_proc_name",
  "revoke", "rev_grant_option", "rev_admin_option",
  "non_role_grantee_list", "grantee_list", "grantee", "user_grantee_list",
  "user_grantee", "role_name_list", "role_name", "role_grantee_list",
  "role_grantee", "declare", "declare_clause", "udf_decl_clause",
  "udf_data_type", "arg_desc_list1", "arg_desc_list", "arg_desc",
  "param_mechanism", "return_value1", "return_value", "return_mechanism",
  "filter_decl_clause", "blob_filter_subtype", "create", "create_clause",
  "recreate", "recreate_clause", "replace", "replace_clause",
  "exception_clause", "rexception_clause", "replace_exception_clause",
  "alter_exception_clause", "unique_opt", "index_definition",
  "shadow_clause", "manual_auto", "conditional", "first_file_length",
  "sec_shadow_files", "db_file_list", "domain_clause", "as_opt",
  "domain_default", "domain_default_opt", "domain_constraint_clause",
  "domain_constraint_list", "domain_constraint_def", "domain_constraint",
  "null_constraint", "check_constraint", "generator_clause", "role_clause",
  "db_clause", "equals", "db_name", "db_initial_desc1", "db_initial_desc",
  "db_initial_option", "db_rem_desc1", "db_rem_desc", "db_rem_option",
  "db_file", "file1", "file_desc1", "file_desc", "file_clause",
  "file_clause_noise", "page_noise", "table_clause", "rtable_clause",
  "external_file", "table_elements", "table_element", "column_def",
  "def_computed", "computed_by", "data_type_or_domain", "collate_clause",
  "column_def_name", "simple_column_def_name", "data_type_descriptor",
  "init_data_type", "default_value", "column_constraint_clause",
  "column_constraint_list", "column_constraint_def", "column_constraint",
  "table_constraint_definition", "constraint_name_opt", "table_constraint",
  "unique_constraint", "primary_constraint", "referential_constraint",
  "constraint_index_opt", "referential_trigger_action", "update_rule",
  "delete_rule", "referential_action", "procedure_clause",
  "rprocedure_clause", "replace_procedure_clause",
  "alter_procedure_clause", "input_parameters", "output_parameters",
  "input_proc_parameters", "input_proc_parameter",
  "output_proc_parameters", "proc_parameter", "default_par_opt",
  "local_declaration_list", "local_declarations", "local_declaration",
  "local_declaration_item", "var_declaration_item", "var_decl_opt",
  "var_init_opt", "cursor_declaration_item", "proc_block",
  "full_proc_block", "full_proc_block_body", "proc_statements",
  "proc_statement", "simple_proc_statement", "complex_proc_statement",
  "excp_statement", "raise_statement", "exec_sql", "for_select",
  "for_exec_into", "exec_into", "if_then_else", "post_event",
  "event_argument_opt", "singleton_select", "variable", "variable_list",
  "while", "label_opt", "breakleave", "cursor_def", "excp_hndl_statements",
  "excp_hndl_statement", "errors", "err", "cursor_statement",
  "open_cursor", "close_cursor", "fetch_cursor", "fetch_opt",
  "exec_procedure", "proc_inputs", "proc_outputs_opt", "exec_block",
  "block_input_params", "block_parameters", "block_parameter",
  "view_clause", "rview_clause", "begin_string", "begin_trigger",
  "end_trigger", "end_default_opt", "check_opt", "trigger_clause",
  "rtrigger_clause", "replace_trigger_clause", "trigger_active",
  "trigger_type", "trigger_type_prefix", "trigger_type_suffix",
  "trigger_position", "trigger_action", "alter", "alter_clause",
  "alter_domain_ops", "alter_domain_op", "alter_ops", "alter_op",
  "alter_column_name", "keyword_or_column", "col_opt",
  "alter_data_type_or_domain", "alter_col_name", "drop_behaviour",
  "alter_index_clause", "alter_sequence_clause", "alter_udf_clause",
  "entry_op", "module_op", "init_alter_db", "alter_db", "db_alter_clause",
  "alter_trigger_clause", "new_trigger_type", "new_trigger_action", "drop",
  "drop_clause", "data_type", "non_array_type", "array_type", "array_spec",
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
  "tra_misc_options", "tra_timeout", "tbl_reserve_options", "lock_type",
  "lock_mode", "restr_list", "restr_option", "table_lock", "table_list",
  "set_statistics", "comment", "ddl_type0", "ddl_type1", "ddl_type2",
  "ddl_subname", "ddl_desc", "select", "for_update_clause",
  "for_update_list", "lock_clause", "select_expr", "column_select",
  "column_singleton", "select_expr_body", "query_term", "query_spec",
  "begin_limit", "end_limit", "begin_first", "end_first", "limit_clause",
  "first_clause", "skip_clause", "distinct_clause", "select_list",
  "select_items", "select_item", "as_noise", "from_clause", "from_list",
  "table_reference", "table_primary", "derived_table", "correlation_name",
  "derived_column_list", "alias_list", "joined_table", "cross_join",
  "natural_join", "qualified_join", "join_specification", "join_condition",
  "named_columns_join", "table_proc", "table_proc_inputs", "table_name",
  "simple_table_name", "join_type", "outer_noise", "group_clause",
  "group_by_list", "group_by_item", "having_clause", "where_clause",
  "plan_clause", "plan_expression", "plan_type", "plan_item_list",
  "plan_item", "table_or_alias_list", "access_type", "index_list",
  "extra_indices_opt", "order_clause", "order_list", "order_item",
  "order_direction", "nulls_placement", "nulls_clause", "rows_clause",
  "insert", "delete", "delete_searched", "delete_positioned", "update",
  "update_searched", "update_positioned", "returning_clause",
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
  "datetime_value_expression", "sec_precision_opt", "array_element",
  "value_list", "constant", "u_numeric_constant", "u_constant",
  "parameter", "current_user", "current_role", "internal_info",
  "sql_string", "signed_short_integer", "nonneg_short_integer",
  "neg_short_integer", "pos_short_integer", "unsigned_short_integer",
  "signed_long_integer", "long_integer", "function", "aggregate_function",
  "numeric_value_function", "extract_expression", "length_expression",
  "bit_length_expression", "char_length_expression",
  "octet_length_expression", "string_value_function", "substring_function",
  "string_length_opt", "trim_function", "trim_specification", "udf",
  "cast_specification", "case_expression", "case_abbreviation",
  "case_specification", "simple_case", "simple_when_clause",
  "searched_case", "searched_when_clause", "when_operand", "case_operand",
  "case_result", "next_value_expression", "timestamp_part", "all_noise",
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
static const unsigned short int yytoknum[] =
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
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,    61,    60,    62,    43,    45,    42,
      47,   539,   540,    59,    44,    40,    41,    58,    91,    93,
      46,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   302,   303,   303,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   305,   305,   305,   306,   306,
     307,   307,   307,   308,   308,   309,   310,   310,   310,   310,
     310,   311,   311,   312,   312,   313,   314,   314,   314,   315,
     315,   316,   316,   317,   317,   318,   318,   318,   318,   319,
     319,   319,   319,   320,   320,   321,   321,   321,   322,   322,
     323,   324,   324,   325,   325,   326,   327,   327,   328,   329,
     329,   329,   330,   330,   330,   331,   331,   332,   333,   333,
     333,   333,   334,   334,   335,   335,   336,   336,   336,   336,
     336,   337,   338,   338,   339,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   341,   342,   342,
     342,   342,   342,   343,   344,   344,   344,   345,   346,   347,
     348,   349,   349,   350,   350,   350,   351,   352,   352,   352,
     353,   353,   354,   354,   355,   355,   356,   356,   357,   358,
     358,   359,   360,   360,   361,   361,   362,   362,   363,   364,
     364,   365,   366,   367,   368,   369,   370,   370,   371,   372,
     372,   373,   373,   374,   374,   374,   374,   374,   375,   375,
     376,   376,   377,   377,   377,   378,   379,   380,   380,   381,
     381,   382,   382,   383,   383,   383,   384,   384,   384,   385,
     386,   387,   387,   387,   388,   388,   389,   389,   390,   390,
     390,   391,   392,   392,   393,   393,   394,   394,   395,   396,
     397,   398,   399,   399,   399,   399,   399,   399,   400,   400,
     401,   401,   402,   403,   403,   403,   403,   403,   404,   405,
     405,   406,   406,   406,   406,   407,   408,   409,   410,   410,
     411,   411,   411,   411,   411,   412,   413,   414,   414,   414,
     414,   415,   416,   417,   418,   419,   419,   420,   420,   421,
     421,   422,   423,   423,   424,   425,   425,   425,   426,   426,
     427,   427,   428,   429,   429,   430,   431,   431,   432,   432,
     432,   433,   434,   434,   435,   436,   436,   436,   437,   437,
     438,   438,   439,   439,   439,   439,   439,   439,   439,   439,
     439,   439,   439,   439,   439,   439,   439,   439,   440,   440,
     440,   440,   441,   441,   442,   443,   444,   445,   446,   447,
     447,   448,   449,   450,   451,   452,   452,   452,   452,   453,
     454,   454,   455,   455,   455,   456,   456,   457,   457,   458,
     459,   459,   460,   460,   460,   460,   461,   461,   461,   462,
     463,   464,   465,   466,   467,   467,   467,   468,   468,   468,
     469,   470,   470,   471,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   479,   480,   481,   482,   483,   483,   483,
     484,   485,   485,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   487,   487,
     488,   489,   490,   490,   490,   490,   490,   490,   490,   490,
     490,   491,   491,   492,   492,   492,   492,   492,   492,   492,
     493,   493,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   495,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   497,   497,   498,   498,   499,   500,   500,
     500,   501,   501,   502,   502,   502,   503,   504,   504,   505,
     505,   506,   507,   507,   508,   508,   508,   508,   508,   509,
     510,   510,   511,   511,   512,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   514,   514,
     515,   515,   516,   516,   517,   517,   518,   518,   519,   519,
     520,   520,   520,   520,   520,   520,   520,   520,   520,   521,
     521,   522,   522,   522,   522,   523,   523,   524,   524,   524,
     525,   525,   526,   526,   526,   527,   527,   527,   528,   528,
     528,   529,   529,   530,   530,   530,   531,   531,   532,   532,
     532,   533,   533,   534,   534,   534,   534,   535,   535,   536,
     536,   536,   537,   537,   537,   538,   538,   538,   539,   540,
     541,   541,   542,   543,   543,   544,   545,   546,   546,   547,
     547,   548,   548,   549,   550,   550,   551,   551,   552,   552,
     552,   552,   552,   552,   553,   553,   554,   554,   555,   555,
     556,   556,   556,   557,   557,   557,   558,   558,   558,   559,
     559,   559,   560,   561,   562,   562,   562,   563,   563,   564,
     564,   565,   566,   566,   567,   567,   568,   569,   569,   569,
     570,   571,   571,   571,   571,   571,   571,   571,   571,   571,
     571,   571,   571,   571,   571,   572,   572,   573,   574,   574,
     575,   576,   576,   577,   577,   578,   578,   579,   580,   581,
     582,   582,   582,   583,   584,   585,   586,   587,   588,   589,
     589,   589,   589,   590,   590,   590,   591,   591,   591,   592,
     592,   593,   593,   594,   594,   595,   595,   596,   596,   597,
     598,   598,   599,   599,   600,   600,   600,   601,   602,   602,
     603,   603,   604,   604,   605,   605,   605,   606,   607,   608,
     609,   609,   610,   611,   612,   612,   613,   613,   614,   614,
     615,   616,   616,   616,   616,   616,   617,   617,   618,   618,
     619,   619,   620,   621,   621,   622,   622,   623,   623,   624,
     625,   625,   625,   625,   625,   626,   626,   627,   627,   628,
     628,   629,   629,   629,   630,   630,   631,   631,   632,   632,
     633,   633,   634,   635,   635,   635,   636,   636,   637,   637,
     638,   638,   638,   639,   639,   640,   640,   641,   642,   643,
     643,   644,   645,   646,   646,   646,   647,   648,   648,   649,
     650,   651,   651,   652,   652,   652,   653,   653,   654,   655,
     655,   656,   656,   657,   657,   658,   659,   659,   660,   660,
     661,   662,   662,   663,   663,   663,   664,   665,   665,   666,
     666,   666,   666,   666,   667,   667,   667,   667,   667,   667,
     668,   668,   668,   669,   669,   669,   669,   669,   669,   669,
     669,   669,   669,   669,   670,   670,   670,   670,   670,   670,
     670,   670,   671,   671,   671,   671,   671,   671,   671,   671,
     671,   671,   671,   671,   671,   671,   671,   671,   672,   672,
     673,   673,   674,   674,   675,   675,   675,   675,   676,   676,
     677,   677,   678,   678,   678,   678,   679,   680,   681,   681,
     682,   682,   682,   683,   683,   683,   684,   684,   685,   686,
     686,   686,   686,   686,   686,   686,   686,   686,   686,   686,
     686,   686,   686,   686,   686,   686,   686,   686,   686,   686,
     686,   686,   686,   686,   686,   686,   686,   687,   687,   687,
     688,   688,   689,   690,   690,   691,   691,   692,   692,   692,
     692,   692,   693,   693,   693,   693,   693,   694,   695,   695,
     696,   697,   697,   697,   697,   697,   698,   698,   699,   699,
     700,   701,   702,   703,   704,   704,   705,   706,   706,   706,
     707,   707,   707,   707,   707,   707,   707,   707,   707,   707,
     707,   708,   708,   709,   710,   710,   710,   711,   712,   712,
     713,   714,   714,   714,   714,   715,   716,   716,   717,   717,
     717,   717,   718,   718,   718,   719,   719,   720,   721,   721,
     722,   722,   722,   723,   723,   724,   724,   725,   725,   726,
     726,   727,   727,   728,   729,   730,   731,   731,   732,   732,
     732,   732,   732,   732,   732,   732,   733,   733,   734,   734,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   744,
     745,   746,   747,   748,   749,   750,   751,   752,   753,   754,
     755,   756,   757,   758,   759,   760,   761,   761,   762,   762,
     762,   762,   762,   762,   762,   762,   762,   762,   762,   762,
     762,   762,   762,   762,   762,   762,   762,   762,   762,   762,
     762,   762,   762,   762,   762,   762,   762,   762,   762,   762,
     762,   762,   762,   762
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
       2,     2,     2,     4,     2,     2,     2,     2,     2,     2,
       2,     1,     0,     1,     1,     6,     6,     1,     1,     0,
       0,     1,     0,     4,     0,     1,     1,     2,     8,     1,
       0,     3,     1,     0,     1,     0,     1,     2,     1,     1,
       1,     2,     6,     1,     1,     3,     0,     1,     1,     0,
       1,     1,     2,     3,     4,     2,     2,     3,     0,     1,
       1,     2,     1,     4,     3,     3,     1,     0,     1,     1,
       2,     3,     4,     0,     1,     2,     0,     1,     1,     5,
       5,     3,     2,     0,     1,     3,     1,     1,     6,     3,
       2,     6,     2,     1,     2,     2,     2,     0,     1,     1,
       2,     0,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     2,     2,     1,     1,     5,     2,     3,     2,     2,
       0,     1,     1,     1,     1,     3,     4,     8,     4,     0,
       1,     1,     2,     2,     0,     3,     3,     1,     2,     2,
       2,     8,     8,     8,     8,     3,     0,     4,     0,     1,
       3,     5,     1,     3,     2,     3,     3,     0,     1,     0,
       1,     2,     4,     1,     1,     3,     1,     0,     2,     2,
       0,     6,     1,     1,     3,     1,     2,     0,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     1,     3,     8,     9,     5,     8,
       6,     3,     0,     3,     2,     1,     1,     3,     3,     7,
       2,     0,     1,     1,     2,     3,     0,     1,     2,     4,
       1,     3,     2,     2,     2,     1,     1,     1,     1,     2,
       2,     5,     0,     5,     1,     3,     0,     2,     4,     0,
       7,     3,     0,     1,     3,     3,     7,     7,     0,     0,
       0,     0,     3,     0,     8,     8,     8,     1,     1,     0,
       2,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     5,     5,     5,     5,     5,     5,     2,     0,
       4,     2,     2,     3,     2,     2,     3,     3,     2,     2,
       3,     1,     2,     3,     3,     2,     2,     2,     2,     3,
       1,     3,     3,     3,     2,     2,     4,     4,     4,     5,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       0,     2,     2,     4,     4,     5,     3,     2,     0,     2,
       0,     0,     1,     2,     2,     4,     3,     2,     2,     7,
       1,     0,     1,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     2,     2,     2,     2,     1,     1,
       1,     1,     4,     5,     1,     3,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     6,     5,     3,     0,     2,     2,     0,
       3,     0,     4,     1,     5,     4,     1,     4,     1,     2,
       2,     1,     1,     1,     2,     2,     2,     2,     0,     3,
       5,     1,     1,     2,     3,     1,     2,     3,     0,     1,
       1,     1,     5,     5,     6,     1,     1,     1,     2,     4,
       1,     0,     5,     1,     0,     3,     3,     1,     0,     2,
       0,     1,     0,     3,     1,     0,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     3,     1,
       1,     3,     3,     1,     2,     3,     1,     2,     0,     3,
       2,     2,     3,     2,     1,     1,     0,     1,     1,     1,
       3,     2,     3,     0,     1,     3,     4,     5,     6,     7,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     2,     1,     1,
       3,     3,     0,     2,     0,     2,     0,     3,     3,     3,
       1,     4,     4,     1,     9,     0,     0,     0,     0,     3,
       2,     1,     0,     3,     5,     3,     2,     5,     2,     1,
       1,     1,     1,     1,     3,     1,     3,     1,     0,     2,
       1,     3,     1,     1,     1,     1,     3,     6,     1,     0,
       3,     0,     1,     3,     1,     1,     1,     4,     5,     5,
       1,     1,     2,     4,     4,     2,     3,     0,     1,     2,
       1,     1,     2,     2,     2,     0,     1,     0,     3,     0,
       1,     3,     1,     2,     0,     2,     0,     2,     0,     4,
       1,     2,     1,     1,     0,     1,     3,     2,     1,     1,
       2,     1,     4,     3,     1,     3,     4,     0,     3,     0,
       1,     3,     3,     1,     1,     0,     1,     1,     4,     0,
       2,     4,     0,     9,     5,     1,     1,     7,     4,     1,
       1,     8,     5,     2,     4,     0,     4,     1,     3,     3,
       1,     7,     7,     5,     3,     0,     1,     1,     1,     2,
       0,     1,     1,     1,     0,     3,     1,     3,     1,     0,
       3,     1,     3,     1,     3,     3,     1,     1,     3,     1,
       2,     1,     3,     3,     1,     2,     3,     3,     3,     3,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     1,     1,
       5,     6,     5,     6,     3,     4,     5,     6,     3,     4,
       3,     4,     3,     4,     4,     5,     4,     4,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     2,     2,
       3,     0,     4,     1,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       4,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     1,     1,     6,     1,     1,     1,     4,     4,     4,
       4,     1,     1,     4,     4,     7,     2,     0,     7,     6,
       6,     4,     1,     1,     1,     4,     3,     6,     1,     1,
       6,     8,     6,     1,     1,     4,     6,     4,     5,     3,
       5,     4,     5,     1,     1,     1,     4,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
static const unsigned short int yydefact[] =
{
       0,     0,   608,   132,     0,     0,     0,     0,     0,     0,
       0,     0,    52,   608,   702,     0,     0,     0,     0,     0,
       0,     0,     2,    12,    18,     9,     8,    16,    17,    14,
      15,     4,    11,    22,   590,    20,   595,   596,   597,     7,
      19,   589,   591,     6,    21,   682,   789,   690,   693,    13,
      10,   805,   806,    23,   809,   810,     5,   501,     0,     0,
       0,     0,     0,     0,     0,     0,   411,   607,   610,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   131,     0,
       0,     0,   104,   795,   990,     0,    24,   988,     0,     0,
      75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   514,     0,   372,    30,    38,
      35,    37,   834,    36,   834,  1096,  1098,  1099,  1100,  1101,
    1102,  1103,  1122,  1123,  1104,  1106,  1105,  1111,  1107,  1112,
    1108,  1110,  1113,  1109,  1114,  1115,  1116,  1118,  1119,  1117,
    1120,  1121,  1125,  1124,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,     0,    32,     0,    33,     0,    68,    70,  1088,
    1097,     0,     0,     0,     0,     0,     0,     0,   610,     0,
       0,  1067,   696,   701,     0,     0,   615,     0,   748,   750,
    1090,     0,     0,     0,     0,     0,   117,   598,  1095,     0,
       0,     1,     3,     0,   686,     0,  1069,   802,     0,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     471,   465,   466,   464,   467,   468,   469,   470,   472,   474,
     477,   473,   475,   476,   478,   479,   480,   481,   482,     0,
     441,   442,   412,     0,  1079,     0,   418,     0,  1083,   415,
     266,  1087,     0,   750,   414,   389,  1091,   419,     0,  1082,
     612,   605,   986,     0,   113,   169,   168,   114,   150,   218,
     846,  1075,   105,     0,   111,   163,     0,   107,   266,   115,
     992,   139,   108,   203,   109,     0,   110,   834,  1094,   116,
     164,   112,   793,   794,     0,   991,   989,     0,    76,     0,
    1080,   766,   522,  1078,   515,     0,   521,   526,   516,   517,
     524,   518,   519,   520,   525,   527,   366,     0,   268,    31,
       0,    40,   833,    39,    29,     0,     0,     0,     0,     0,
     839,     0,     0,     0,     0,     0,     0,   604,   606,   996,
       0,   977,   695,   695,   696,   708,   706,  1066,   709,     0,
     710,   700,   696,     0,     0,     0,     0,     0,     0,   633,
     626,     0,     0,     0,   613,   614,   616,   618,   619,   620,
     629,   630,   621,   622,   623,     0,   749,  1089,   122,     0,
     118,   266,   119,   203,   121,     0,   120,   834,   601,     0,
     660,   661,   668,     0,   667,   669,   671,   676,   664,   662,
     665,   663,   672,   675,   670,   674,     0,     0,     0,   684,
       0,   680,     0,     0,  1068,     0,     0,   687,     0,     0,
       0,     0,   416,   502,     0,     0,     0,     0,   221,   417,
     421,   130,  1071,   420,   498,   491,   492,     0,   268,   240,
     483,     0,   413,   430,     0,   387,   388,   511,     0,   611,
     609,   987,   166,   166,     0,     0,     0,   178,   170,   171,
     149,     0,   127,     0,     0,     0,   123,   268,   138,   137,
     140,     0,     0,     0,     0,     0,    77,   221,     0,     0,
     768,   808,   523,     0,     0,     0,     0,   952,   983,     0,
    1070,     0,   954,     0,     0,   984,     0,   978,   969,   968,
     967,  1096,     0,     0,     0,   957,   961,   961,   970,   971,
     979,   980,  1106,   981,   982,     0,  1111,  1107,   985,  1120,
    1125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   934,   369,   929,   843,   963,   955,   930,   364,   972,
     932,   933,   949,   950,   951,   973,   931,   997,   998,  1011,
    1012,  1014,  1015,  1016,   999,  1021,  1022,   938,   935,   936,
    1038,  1039,  1043,  1044,   937,   956,     0,     0,  1075,     0,
       0,     0,   373,   219,     0,     0,     0,   836,    28,     0,
      34,     0,     0,    44,    71,    73,  1092,    69,     0,     0,
       0,   838,     0,    49,    51,    29,     0,     0,   603,     0,
       0,     0,   705,   703,     0,   712,     0,   711,   713,   718,
     699,     0,   656,     0,     0,   627,   638,   624,   638,   625,
     643,   649,   653,   654,   634,     0,   641,   640,   617,   766,
     817,   847,     0,     0,   128,   268,     0,     0,     0,   600,
     599,   673,   666,     0,     0,  1086,     0,     0,   681,   685,
     788,   790,   795,   692,   691,   800,   186,     0,   504,   146,
       0,   507,     0,   508,   503,   379,     0,   425,   427,   426,
     379,   380,   428,     0,   422,     0,   500,     0,     0,   269,
       0,     0,   434,     0,   435,     0,   484,     0,   490,     0,
       0,     0,   487,  1075,   392,   391,   510,     0,   409,     0,
     167,     0,     0,   176,     0,   175,     0,     0,   165,   179,
     180,   182,   172,   559,   571,   546,   581,   549,   572,   582,
       0,   588,   550,     0,   578,     0,   573,   585,   545,   568,
     547,   548,   543,   379,   528,   529,   530,   538,   544,   531,
     540,   561,     0,   566,   563,   541,   578,   542,   126,     0,
     124,   266,   125,     0,     0,   141,     0,     0,   202,   240,
     389,   378,     0,   221,     0,    83,    85,     0,     0,   103,
     102,  1072,     0,     0,     0,     0,   920,   921,   922,     0,
     765,   851,   860,   863,   869,   864,   865,   866,   867,   871,
     872,   870,   873,   868,   849,     0,   774,   789,  1067,     0,
    1067,   974,     0,     0,  1067,  1067,  1067,     0,   975,   976,
       0,   958,   959,     0,     0,  1054,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   940,   939,
       0,   789,   963,     0,   334,  1093,     0,   363,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   274,   538,
     561,     0,     0,   371,     0,   279,     0,   835,     0,     0,
      45,    74,     0,     0,    27,   825,     0,   841,     0,   804,
     825,     0,     0,    48,   602,     0,   695,     0,     0,   766,
       0,   717,     0,   593,     0,   592,   994,     0,   628,   639,
       0,   636,   632,   631,     0,   646,     0,   651,   635,   642,
       0,   768,   812,     0,     0,     0,   240,   389,   378,   679,
     657,   678,     0,     0,     0,   683,     0,   799,     0,     0,
     147,   187,   506,     0,   424,     0,   423,   429,   497,     0,
     496,   379,     0,   265,   378,   239,  1076,   213,   210,     0,
     153,   379,   528,   378,     0,     0,     0,   244,   238,   241,
     242,   243,   433,   489,   488,   432,   431,     0,     0,     0,
       0,     0,   395,   393,   394,   390,     0,   379,   494,     0,
     493,   196,   173,   177,     0,     0,   181,     0,     0,   556,
     569,   570,   586,     0,   583,   588,     0,   576,   574,   575,
     153,     0,     0,     0,   539,     0,     0,     0,     0,   577,
     129,   268,     0,   378,   142,   201,     0,   204,   206,   207,
       0,     0,     0,     0,   221,   221,    80,     0,    88,    79,
       0,     0,     0,     0,   862,   850,     0,     0,   923,   924,
     925,     0,   854,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   770,   772,   773,   767,     0,   802,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,
       0,     0,     0,     0,     0,  1049,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1032,  1034,
    1033,     0,     0,   948,   802,   947,   365,     0,   335,   367,
     336,     0,     0,   943,  1074,   942,   941,   944,   945,   946,
     964,  1036,     0,   953,   845,   844,   375,   374,     0,   272,
     287,     0,   278,   280,   837,     0,     0,     0,    72,     0,
     830,     0,   840,     0,   830,     0,     0,   704,   707,     0,
     719,   720,   723,   725,   722,   734,   735,   736,   724,   747,
       0,   759,   714,   716,  1084,   594,   995,   637,   650,   645,
     644,     0,   655,   818,   789,   819,   848,   378,     0,     0,
       0,   658,   677,     0,   791,   697,   792,   801,   505,   166,
     193,   185,   188,   189,     0,     0,   151,   227,   222,   965,
     223,   224,   225,   226,   499,   277,   270,   279,   212,   379,
     152,   381,   214,   209,   215,     0,     0,   249,   437,   440,
     380,   438,   485,   486,   436,     0,     0,     0,   408,   513,
     495,   197,   198,   174,     0,   184,   557,   558,   993,     0,
       0,     0,   561,     0,   584,     0,     0,   381,     0,   534,
     536,     0,     0,     0,     0,     0,     0,     0,   389,   279,
     166,   144,   240,   199,   409,   383,     0,    84,     0,   221,
       0,    92,     0,    86,     0,     0,    91,    87,     0,   816,
    1077,     0,     0,   855,     0,     0,   861,     0,     0,   853,
     852,     0,   910,     0,   899,   898,     0,   877,     0,   908,
     926,     0,   918,     0,     0,     0,   878,   904,     0,     0,
     881,     0,     0,     0,     0,     0,     0,     0,   879,     0,
       0,   880,     0,   912,     0,     0,   874,     0,     0,   875,
       0,     0,   876,   771,   774,   807,     0,     0,   221,     0,
    1000,     0,     0,  1023,     0,     0,     0,     0,     0,     0,
       0,   960,     0,     0,  1055,     0,     0,  1053,     0,     0,
    1045,     0,     0,     0,     0,  1056,  1017,  1018,  1019,  1024,
    1020,     0,  1031,     0,     0,   689,     0,     0,     0,   962,
    1035,     0,   267,   286,     0,   341,   370,   281,     0,     0,
       0,     0,     0,  1102,    42,    53,    55,    54,    63,    65,
      42,    43,     0,     0,     0,   822,   842,     0,   821,     0,
       0,     0,   755,   722,     0,   757,   751,   757,   755,   757,
       0,     0,     0,   718,     0,   764,   647,   648,   652,   802,
     279,   200,   409,   383,   659,     0,     0,   194,     0,   190,
       0,   966,   379,   379,   381,     0,     0,   228,     0,   249,
     795,   245,   439,   400,   401,   397,   396,   399,   398,   379,
     512,   380,   183,  1073,     0,     0,   552,     0,   551,   587,
       0,   579,   155,     0,   532,     0,   560,   561,   567,   565,
       0,   562,   378,     0,     0,     0,   136,   145,   205,     0,
       0,   380,   106,     0,   134,   133,    95,     0,     0,    96,
       0,    89,    90,     0,   916,   917,   857,   856,   859,   858,
       0,     0,     0,     0,   789,     0,     0,     0,   919,     0,
       0,     0,     0,     0,     0,   911,   909,   905,     0,   913,
       0,     0,     0,     0,   914,     0,     0,     0,     0,     0,
       0,   778,     0,   775,     0,   779,  1006,  1005,     0,     0,
    1002,  1001,     0,  1010,  1009,  1008,  1007,  1004,  1003,     0,
    1027,  1051,  1050,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   368,   338,   337,   273,     0,     0,   283,
     284,     0,  1075,   324,     0,   317,     0,     0,     0,   316,
    1104,  1109,     0,     0,   362,   298,   293,     0,   341,   292,
       0,   301,   311,   312,   308,   320,   321,   309,   318,   313,
     306,   319,     0,   315,   314,   356,   358,   357,   307,     0,
     303,   305,   304,   302,   310,   820,     0,  1075,    67,    59,
      60,    66,    61,    62,     0,    25,     0,     0,    26,     0,
     826,   827,   828,   824,   829,   832,   831,   815,    46,    47,
     718,   726,   721,   756,   754,   752,     0,   753,     0,     0,
       0,     0,     0,     0,   768,   811,     0,     0,   380,   796,
     797,   698,   196,   195,   191,   380,     0,     0,   271,   380,
     380,   217,   229,   230,     0,     0,   246,     0,     0,     0,
       0,     0,     0,     0,   279,   509,   554,     0,   555,     0,
       0,   217,   154,   156,   158,   159,   160,   535,   537,   533,
     564,   279,   409,   380,   196,   380,     0,   376,   379,    93,
       0,     0,    99,    94,   561,     0,   902,     0,     0,   928,
     802,   927,   900,     0,     0,     0,   906,     0,     0,     0,
       0,   915,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   769,   774,     0,   781,     0,   777,   780,  1037,
     220,  1057,  1013,     0,     0,  1052,  1047,  1046,     0,  1040,
    1042,     0,  1029,  1030,     0,   290,   282,     0,   322,     0,
       0,   332,   344,  1085,   359,   360,     0,   294,     0,   299,
     296,   347,   300,     0,     0,     0,   340,     0,    56,    57,
      58,    64,     0,     0,   803,   729,     0,   737,   755,   746,
     744,   758,   760,   762,   763,   694,   380,   380,   377,   798,
     192,   162,   275,   276,   264,     0,     0,   208,   231,     0,
       0,   249,   233,   234,   232,   834,     0,   406,   407,   403,
     402,   405,   404,     0,   553,   580,   161,   148,   157,     0,
       0,   261,   143,   384,   382,     0,     0,    97,    98,    81,
       0,   885,   893,   688,   901,   886,   894,   889,   897,   903,
     907,   887,   895,   888,   896,   882,   890,   883,   891,   884,
     892,   776,     0,   787,  1026,  1025,  1048,     0,  1028,     0,
       0,   285,     0,   323,   325,     0,   331,     0,   355,     0,
       0,     0,     0,   350,   348,     0,     0,     0,   333,    41,
     823,   813,   731,   728,   738,     0,     0,   739,   740,   741,
       0,   262,   385,   211,   216,   249,   834,   236,   254,   248,
     410,   380,   380,   380,    78,   100,   101,     0,   784,     0,
     783,  1041,   288,   289,     0,     0,     0,     0,   354,   353,
    1081,   352,   341,     0,     0,     0,     0,     0,     0,   727,
     742,     0,   761,   237,   254,     0,   249,   250,   251,   263,
     386,     0,   782,     0,     0,     0,   328,   341,   361,   349,
     351,     0,   346,     0,   814,     0,   732,     0,   249,     0,
       0,   247,     0,   253,     0,   252,   135,   785,     0,   291,
     330,     0,     0,     0,   341,     0,   730,   743,   235,     0,
       0,   257,   256,   255,   786,   341,     0,     0,   341,   339,
     733,   260,   258,   259,   329,   341,   345,   326,   327
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    21,    22,    23,   579,   152,   153,   154,   155,  1635,
     864,   859,    24,   166,   167,  1394,  1395,  1396,  1397,  1398,
     156,   157,   583,   584,    25,    90,   476,  1018,   764,   765,
     766,  1277,  1270,  1271,  1723,   298,   768,    26,    82,    27,
     186,    28,   466,   272,   378,   748,   242,    83,  1492,   279,
     470,   756,  1261,  1486,   658,   267,   461,  1210,  1211,  1701,
    1702,  1703,  1704,  1705,  1706,   274,   289,   264,   701,   265,
     457,   458,   459,   708,   709,   710,   659,   660,  1191,  1192,
    1193,  1438,  1233,   282,   382,   472,  1006,  1007,  1008,   938,
     939,   940,  1827,   683,   569,  1548,   767,  1196,  1681,  1682,
    1683,  1834,  1009,   685,   948,   949,   950,   951,  1451,  1966,
    1967,  1968,  2012,   277,   380,   750,   249,   438,   575,   678,
     679,  1128,   570,  1444,  1131,  1132,  1133,  1578,  1579,  1384,
    1891,  1580,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,
    1603,  1604,  1605,  1606,  1607,  1608,  1609,  1896,  1610,   531,
    1109,  1611,  1612,  1613,  2003,  1790,  1791,  1902,  1903,  1614,
    1615,  1616,  1617,  1786,  1618,   532,   837,    30,   318,   571,
     572,   286,   386,  1011,   923,   926,  1447,  1491,   284,   384,
     752,   447,   696,   697,   965,   967,  1460,    31,    66,   429,
     430,   442,   443,   239,   240,   444,  1221,   691,   955,   246,
     257,   433,   676,   930,   198,   422,   423,   254,   698,  1461,
      32,   105,   733,   734,   735,  1248,  1249,   736,   849,   738,
     739,  1242,   979,   994,   740,   850,   742,   743,   744,   745,
     987,   746,   747,   984,    33,    34,    35,    36,    37,   640,
      38,   599,    39,    40,    68,   261,   450,    41,   364,   365,
     366,   367,   368,   369,   370,   371,   892,   372,   373,   374,
    1171,  1428,   620,   621,   897,   622,    42,    43,   406,   407,
     408,   914,   910,  1619,   194,   648,   411,    45,  1513,   830,
      46,    47,    48,   602,   351,  1435,  1819,   171,   172,   173,
     349,   606,   607,   608,   882,   879,  1150,  1151,  1152,  1153,
    1912,  1959,  1985,  1154,  1155,  1156,  1157,  1917,  1918,  1919,
    1158,  1423,   177,   178,  1421,  1654,  1425,  1811,  1812,  1664,
     480,   797,  1541,  1056,  1542,  1543,  1544,  1757,  1937,  1940,
     197,   650,   651,   294,  1671,  1186,   417,  1620,  1621,    51,
      52,  1622,    54,    55,  1804,   481,   629,  1623,  1624,    56,
    1140,  1639,  1640,  1405,  1644,   321,   322,   576,   590,   591,
     866,   533,   534,   632,   780,  1031,   781,   782,   783,   784,
    1296,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     794,  1033,  1299,  1300,   795,   536,   811,   537,   538,  1198,
     539,   540,   541,   542,   543,   544,   545,  1642,    87,   296,
     281,  1240,  1250,   886,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,  1764,   556,  1102,   557,   558,   559,
     560,   561,   562,  1088,   563,   816,  1358,   817,  1355,   564,
    1080,   350,   415,   565,   566,   770,  1462,  1113,   270,   935,
    1279,   302,   243,   299,  1949,   275,  1938,  1163,  1626,   644,
    1159,   158,   567,   253,   255,  1399,   834,   287,   187,   568,
     160
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1467
static const short int yypact[] =
{
    2672,  1755,   105,  1745,    43,   757,   274,  2623,   116,  8324,
     498,   389,  2371,   105,   621,   501,  7002,   597,  7002,   221,
     329,   522,   232, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,   467,   387, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,  9403,  7002,
     542,  7002,  7002,  7002,  7002,  7002, -1467, -1467,   543,   661,
    7002,  7002,  7002,   708,  7002,   523,  7002,  7002, -1467,  7002,
    7002,  7002, -1467,   536, -1467,   534, -1467, -1467,   657,  7002,
   -1467,  7002,  7002,  7002,   696,  7002,  7002,  7002,  7002,   523,
    7002,  7002,  7002,  7002,  7002, -1467,  7002,   473,   634, -1467,
   -1467, -1467,   504, -1467,   504, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,   704,   492,   707, -1467,    42, -1467, -1467, -1467,
   -1467,  7002,  7002,  7002,   722,   740,   207,  7002,   456,   362,
     382,   517,   635, -1467,  7002,   727,  1872,   715, -1467,  7002,
   -1467,  7002,  7002,  7002,  7002,  7002, -1467, -1467, -1467,  7002,
    9032, -1467, -1467,   702,   701,   886,   547,   654,   751, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,   311,
   -1467, -1467, -1467,   661, -1467,   718, -1467,   237, -1467, -1467,
     662, -1467,   598, -1467, -1467,   240, -1467, -1467,   674, -1467,
     775, -1467, -1467,   762, -1467,   933, -1467, -1467,   968, -1467,
   -1467, -1467, -1467,   661, -1467, -1467,   499, -1467,   662, -1467,
   -1467,   296, -1467,   922, -1467,   909, -1467,   504, -1467, -1467,
   -1467, -1467, -1467, -1467,   905, -1467, -1467,   718, -1467,   916,
   -1467,   817, -1467, -1467, -1467,   718, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467,  7941,  7002,   866, -1467,
    7002, -1467, -1467, -1467,   880,   430,   897,  4834,  7002,   974,
     769,   994,  1009,  1010,   953,   958,   121,   840, -1467, -1467,
    8212, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,  6044,
   -1467, -1467, -1467,   923,  7002,   996,   178,   170,  7002,   930,
   -1467,   820,   825,   828, -1467,  1872, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,  7002, -1467, -1467, -1467,   661,
   -1467,   662, -1467,   922, -1467,  1051, -1467,   504,   999,   961,
   -1467, -1467, -1467,  1053, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467,  1045,  7002,  7002,  1011,
     887, -1467,  8212,   976, -1467,   976,  8212, -1467,   112,   879,
     881,   884,   751, -1467,   262,   484,  1091,  7002, -1467,   311,
   -1467, -1467, -1467, -1467,  1087, -1467, -1467,  7002,   866,  3181,
     934,  3461,   849, -1467,  9403, -1467, -1467,   724,   963, -1467,
   -1467, -1467,   872,   872,   661,  1047,   661,   128,   933, -1467,
   -1467,  4102, -1467,  7002,  7002,  7002, -1467,   866, -1467, -1467,
    1131,   182,   867,  7002,  1153,  7002, -1467,    76,  6189,  4689,
    1031, -1467, -1467,   870,   873,   874,   971, -1467, -1467,   877,
   -1467,   883, -1467,   885,   888, -1467,   891, -1467, -1467, -1467,
   -1467,   892,   894,   975,   977, -1467,   895,   895, -1467, -1467,
   -1467, -1467,   899, -1467, -1467,  6315,   900,   901, -1467,   904,
    1070,   907,   912,   913,   920,   921,   928,  8212,  8212,  6586,
    7002, -1467,  1027,   906, -1467,  1571, -1467, -1467,   898, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467,   929,   932,   936,  4102,
     927,   288, -1467, -1467,   938,  1169,   541, -1467, -1467,  7002,
   -1467,  7002,  7002,    67, -1467, -1467, -1467, -1467,  7002,  7002,
     616, -1467,  7002, -1467, -1467,   880,  1040,  4834, -1467,  7002,
    6586,   219, -1467, -1467,  8212, -1467,  1143,   940, -1467,   183,
   -1467,   465, -1467,   573,   957, -1467,   343, -1467,   343, -1467,
     945, -1467,   118, -1467,  1074,   523, -1467, -1467, -1467,    39,
   -1467, -1467,   962,   952, -1467,   866,   946,  7002,  1244, -1467,
   -1467, -1467, -1467,   449,  1172, -1467,   964,  7002, -1467, -1467,
     969, -1467,   213, -1467, -1467,   941, -1467,  1175,  1176, -1467,
     661, -1467,  1177, -1467, -1467, -1467,  1247, -1467, -1467, -1467,
   -1467, -1467, -1467,  4102, -1467,   661,  1160,  4102,   574, -1467,
    1260,  7002, -1467,  8914, -1467,   212, -1467,  7002,   671,   598,
    1098,   146,  1137,  1103, -1467, -1467, -1467,   205,  1146,   470,
   -1467,  1092,   523, -1467,   661, -1467,  1262,  1204, -1467,   128,
   -1467, -1467, -1467,    58,  1112, -1467, -1467, -1467,  1113, -1467,
    1162,  1006, -1467,  1217,  1014,   477, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467,  1013, -1467, -1467,
   -1467,   139,  1018,  1019,    85, -1467,  1014, -1467, -1467,   661,
   -1467,   662, -1467,  1255,  1311, -1467,   661,   661, -1467,  3181,
     240, -1467,  1200, -1467,  1174,  1033, -1467,  2325,  1201, -1467,
   -1467, -1467,  1213,  1043,  4960,  1050,  1323,  1406,  1428,  4418,
     214, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,  2765,   153,  1221,   688,  8212,
     155, -1467,  7002,  8212,   695,   703,   714,  1198, -1467, -1467,
     523, -1467, -1467,  8212,  5231,  1571,   181,  1163,  8212,  8212,
    5231,  1287,  8212,  8212,  8212,  8212,  8212,  5502,   764,   764,
    1057,   387,   455,   592, -1467, -1467,  5377, -1467,  8212,  7002,
    8212,  8212,  8212,  8212,  8212,  8212,  6857,  3742, -1467, -1467,
    1334,  1055,  7002, -1467,  7002,  1315,  7002, -1467,  1184,  1186,
   -1467, -1467,  1168,  4834, -1467,  1302,   600, -1467,  1069, -1467,
    1302,  7002,  7002,  1077, -1467,   455, -1467,   476,  5105,  1183,
    8212, -1467,  7002, -1467,    71, -1467, -1467,   191, -1467, -1467,
    1191, -1467, -1467, -1467,  7002,   594,  7002, -1467, -1467, -1467,
    7002,  1031, -1467,  8212,  7002,  1368,  3181,   240, -1467, -1467,
   -1467, -1467,   449,  7002,  1303,  1093,  8212,  1145,  8212,   661,
   -1467,   365, -1467,  1094, -1467,  1847, -1467, -1467, -1467,   661,
   -1467, -1467,  7002, -1467, -1467, -1467, -1467,  1372, -1467,  1104,
    1091, -1467,  1361, -1467,  1309,  1317,   504, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467,  7002,  1382,  1091,
    9118,   523,  1305,  1306,  1307, -1467,   523, -1467, -1467,   491,
   -1467,   799, -1467, -1467,  1248,   661, -1467,  6189,    34,  1273,
   -1467, -1467, -1467,   523, -1467,  1006,   249, -1467, -1467, -1467,
    1091,   249,  1272,   249, -1467,   523,   523,  1138,   523, -1467,
   -1467,   866,  7002, -1467,  1338, -1467,   605, -1467, -1467, -1467,
     724,   976,  7002,   606,    64, -1467, -1467,  1151,   505, -1467,
    6189,  7002,   976,  5773, -1467, -1467,   976,  4960, -1467, -1467,
   -1467,   123, -1467,   225,  1737,  5231,  5231,  8212,  8212,  2070,
    1152,   209,  2366,  8212,  2747,   911,  3027,  3307,  7128,  3587,
    3867,  4147, -1467, -1467,  1330, -1467,  1155,   654,  8212,  8212,
     200,  8212,  1164,  8212,  1154,   489,  8212,  8212,  8212,  8212,
    8212,  8212, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
    1395,  1170,  1252,   196,  8212, -1467,  5231,  8212,   279,   335,
    1451,   148,  7002,   532,   645,   656,   684,   709, -1467, -1467,
   -1467,   366,  7399, -1467,   654, -1467, -1467,  5919, -1467,  1171,
   -1467,  1167,   432, -1467, -1467,  1441,   163,   163,   764,   764,
    1571, -1467,   618, -1467, -1467, -1467, -1467, -1467,   626, -1467,
    1284,  1460,  1315, -1467, -1467,  9228,  9228,  1353, -1467,   158,
    1373,  7002, -1467,  8212,  1373,  1419,  1420, -1467, -1467,  4563,
    1190,   862, -1467, -1467, -1467, -1467, -1467, -1467, -1467,  1192,
    5231,  1417, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467,   126, -1467, -1467,  1221,  1571, -1467, -1467,   642,   724,
     976, -1467, -1467,   449, -1467, -1467, -1467,  1571, -1467,   872,
    1480, -1467,   365, -1467,  5231,   831, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,   120, -1467,  1315, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,   504,   504,  1254, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,   206,   164,   571, -1467,  1484,
   -1467, -1467, -1467, -1467,  7002, -1467, -1467, -1467, -1467,    43,
     659,  1336,  1334,  1202, -1467,  1092,   664, -1467,   510, -1467,
    1203,  7002,   550,  1205,  1206,   523,  1208,  1487,   240,  1315,
     872,  1176,  3181, -1467,  1146,  1313,  2902, -1467,   523,  1377,
    1454, -1467,  2325, -1467,   523,   371, -1467, -1467,  1461, -1467,
   -1467,  1216,  1218, -1467,  5231,  5231, -1467,  5231,  5231, -1467,
    1507,   160,  1571,  1223, -1467, -1467,  1224,  1571,  6586, -1467,
   -1467,  1453, -1467,   472,  1225,  1226,  1571,   747,  1232,  1235,
    1571,  8212,  8212,  1152,  8212,  7670,  1237,  1238,  1571,  1239,
    1242,  1571,  8212,  1571,  1243,  1250,  1571,  1256,  1257,  1571,
    1258,  1259,  1571, -1467,  9317, -1467,   861,   931, -1467,   960,
   -1467,   972,  8212, -1467,   997,  1016,  1030,  1054,  1078,  1165,
    8212, -1467,  8212,  8212,  1571,  1492,   199,  1571,  1386,  8212,
   -1467,  8212,  8212,  8212,  8212, -1467, -1467, -1467, -1467, -1467,
   -1467,  8212, -1467,  8212,  1500, -1467,   683,  5919,  6731, -1467,
   -1467,  7002, -1467, -1467,  7002,  8575, -1467, -1467,  7002,  7002,
    7002,  7002,  7002,  7002,  1358,  1266, -1467,  1267, -1467, -1467,
    1358, -1467,   412,   412,    54, -1467, -1467,   712, -1467,  9228,
    9228,  1276,   995,  1280,  5105,  1435, -1467,  1435,   682,  1435,
    1493,  1494,  8212,  1100,  1558,  1508, -1467, -1467, -1467,   654,
    1315, -1467,  1146,  1313, -1467,   549,  1092,  1456,  1092, -1467,
     127, -1467, -1467, -1467, -1467,  1460,  8212,   526,  1442,  1254,
     536, -1467, -1467,  1465,  1466,  1467,  1468,  1472,  1473, -1467,
   -1467, -1467, -1467, -1467,  1292,    43, -1467,  1404, -1467, -1467,
     249, -1467,   508,   249, -1467,   249, -1467,  1334, -1467, -1467,
    1308, -1467, -1467,   724,  1460,  1092, -1467,  1176, -1467,  1484,
    1579, -1467, -1467,  1310, -1467,  1093, -1467,  1318,   661,   175,
    1319, -1467, -1467,   661, -1467, -1467, -1467,  1507, -1467,  1507,
    8212,   976,   976,  1320,   387,   766,  8212,  1539, -1467,   976,
     976,  8212,   976,   976,   192,  1571, -1467,  1542,  8212,  1571,
     976,   976,   976,   976,  1571,   976,   976,   976,   976,   976,
     976, -1467,  1321,  1312,   607,  7002, -1467, -1467,  1324,  4102,
   -1467, -1467,  1182, -1467, -1467, -1467, -1467, -1467, -1467,  1268,
    1557, -1467, -1467,  8212,  8212,  1552,  1449,  1369,   767,  1463,
    1379,  1393,  8212, -1467, -1467, -1467, -1467,  4102,  1331, -1467,
   -1467,  1589,  1590,  7002,   159, -1467,  1335,  1549,  8212, -1467,
    1339,  5647,  7002,  7002, -1467, -1467, -1467,  1583,  8459, -1467,
    1343, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,   174, -1467, -1467, -1467, -1467, -1467, -1467,  1562,
   -1467, -1467, -1467, -1467, -1467, -1467,  1348,   -17, -1467, -1467,
   -1467, -1467, -1467, -1467,  1576, -1467,  9228,  9228, -1467,  1474,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467,  1375, -1467, -1467,
    1637, -1467,   862, -1467, -1467, -1467,  1566, -1467,  5105,  5105,
     780,  7002,  8212,  5231,  1031, -1467,  1460,  1484, -1467, -1467,
   -1467, -1467,   799, -1467, -1467, -1467,  1847,  1847, -1467, -1467,
    1571,  1631,   526, -1467,   546,  7002, -1467,  1581,  1475,  1582,
    1481,  1619,  1584,  1626,  1315, -1467, -1467,  1376, -1467,  1381,
    1577,  1631,   508, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467,  1315,  1146, -1467,   799, -1467,  1554, -1467, -1467, -1467,
    1568,   108, -1467, -1467,  1334,  1573,  1571,  1389,  1401, -1467,
     654, -1467,  1571,  8212,  1402,  1409,  1571,  1411,  1413,  8212,
    8212,  1571,  1414,  1415,  1423,  1424,  1431,  1433,  1436,  1437,
    1438,  1439, -1467,  9317,  1378, -1467,  7002, -1467, -1467, -1467,
   -1467, -1467, -1467,  8212,  1440, -1467, -1467, -1467,  8212, -1467,
   -1467,  8212, -1467, -1467,  1412,   124, -1467,  1670,  8212,  8212,
    5231,  1571, -1467, -1467, -1467, -1467,  7002, -1467,   429, -1467,
    1548, -1467, -1467,   220,  1447,  5919, -1467,  1622, -1467, -1467,
   -1467, -1467,   412,  8212, -1467,  7002,  5105, -1467,   640, -1467,
   -1467,  1455, -1467,  1571,   214, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467,  1452,  7002, -1467, -1467,  1669,
    7002,  1254, -1467, -1467, -1467,   504,  7002, -1467, -1467, -1467,
   -1467, -1467, -1467,  1460, -1467, -1467, -1467, -1467, -1467,  1460,
    1484, -1467, -1467, -1467, -1467,  8212,   661, -1467,  1560, -1467,
     661, -1467, -1467, -1467,  1571, -1467, -1467, -1467, -1467,  1571,
    1571, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,  7002,  1677,  1571, -1467, -1467,  1434, -1467,  1847,
    1847, -1467,  1471,  1571,  1130,   137, -1467,  1676, -1467,  7002,
    7002,    43,   102, -1467, -1467,  1515,  1680,  5231,  1171, -1467,
   -1467,    99,  1477, -1467, -1467,  5231,  1478, -1467, -1467, -1467,
    8212, -1467, -1467, -1467, -1467,  1254,   504, -1467,  1645, -1467,
   -1467, -1467, -1467,  1571, -1467, -1467, -1467,  1479,  1476,  1483,
   -1467, -1467, -1467, -1467,   976,  5919,  1601,  5919, -1467, -1467,
   -1467, -1467,  8710,   429,  8212,  5919,   140,  5919,  7002, -1467,
     214,  7002, -1467, -1467,  1645,   168,  1254,  1658,  1663, -1467,
   -1467,  1490, -1467,  7002,  7002,  1495,  1171,  8710,  1171, -1467,
   -1467,  1279,   149,  1748,  1171,   785, -1467,   791,  1254,   525,
     525, -1467,  1754, -1467,  1625, -1467, -1467, -1467,  1503, -1467,
    1757,  5919,  1772,  1762,  8710,  7002, -1467, -1467, -1467,  1610,
     493, -1467, -1467, -1467, -1467,  8710,   122,  7002,  8710, -1467,
   -1467, -1467, -1467, -1467, -1467,  8710, -1467, -1467, -1467
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
   -1467, -1467, -1467, -1467,  1220,  1652, -1467,  1653,  1497,   423,
   -1467,   954, -1467, -1467, -1467,  -970, -1467,  -687, -1467,  -646,
    1657,  1509,  1246,   973, -1467, -1467, -1467,   563, -1467,  1085,
     826, -1467, -1467,   581, -1467, -1467,   832, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467,   590, -1467, -1467,  -350,   864, -1467,
   -1467,   161, -1467,   180,  -410,  1781, -1467, -1467,  -394, -1467,
   -1467, -1467,  1407, -1467, -1467,  1157,  -421, -1467, -1467, -1467,
     675, -1467, -1408, -1467, -1467,  1485,   965,   610,  1430,   935,
     608, -1467,   177,   -51,  -319, -1467,  -371, -1398, -1467, -1467,
     193, -1467,  1443, -1257, -1467, -1467, -1467, -1467, -1374,   -88,
     -89,   -87,  -109, -1467, -1467, -1467, -1467,  -155,  -321, -1467,
     951, -1467,  -754, -1467, -1074, -1467,   752, -1467, -1467, -1467,
   -1467, -1467, -1466, -1025, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,  -810,
   -1038, -1467, -1467, -1467, -1467, -1467,    97, -1467,   -64, -1467,
   -1467, -1467, -1467, -1467,  1888, -1467, -1467, -1467, -1467, -1467,
    1041, -1467, -1467,  -800,  -632, -1118, -1060,   459, -1467, -1467,
   -1467,  -641,  -872, -1467, -1467, -1136, -1345, -1467, -1467, -1467,
    1469, -1467,  1211,  1450, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467,  1486, -1467, -1467, -1467,
   -1467, -1467,  -643,  -526, -1467,   902,   424,  -656,  -430, -1467,
   -1467, -1467, -1467, -1120, -1467,  -429, -1467, -1467, -1467, -1467,
    1161, -1467, -1467,   924, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467,  1892,  1733, -1467, -1467, -1467, -1467,
    1541, -1467, -1467, -1467,  1297, -1467,  1293, -1467, -1467, -1467,
   -1467, -1467, -1467,  1020, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,  -762,     1, -1467, -1467, -1467,  -491,   518, -1467,
    -490,   698, -1467,  -264,   315, -1467, -1467, -1467, -1467,  1743,
   -1467, -1467, -1467,  1036, -1241, -1467, -1467, -1040, -1460, -1467,
   -1467, -1467, -1467,   770, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467,  1827,   -46,   502,  -296, -1467, -1467,     2, -1467,
    -465,  -841,  1127, -1467,   172, -1467,   381, -1467,  -950, -1467,
    -733, -1467,  1017,  -611, -1467, -1467,  -983,  1929,  1930, -1467,
   -1467,  1934, -1467, -1467, -1467,  1325, -1467,  -268, -1467, -1467,
    1065, -1276, -1467,   792, -1467,  -110,  -805,  -631, -1467, -1467,
   -1467,  -803,   -49,  -541,  -791, -1467,  -565, -1467, -1467, -1467,
     591, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1467,
    -596, -1467,   624, -1467,  1159,  -888,  1432, -1467,  -504, -1467,
     745,  -880,  -159,  -821,  -799,  -796,   -54,    -2,    21, -1467,
     -71, -1144,  -560,  -157, -1467, -1467, -1467, -1467, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467, -1467, -1307, -1467, -1467,
   -1467, -1467, -1467, -1467, -1467, -1467,   583, -1467, -1223, -1467,
   -1467,    36, -1467,  -785,   451,   970,   691,   131,  -742,  1261,
   -1311, -1467,   -66,  1851, -1467,   -19,   -53, -1419,   359,  -295,
     -40,   -56,  -176,   -10,   -59,  -292, -1467,   -82,   -65,    -9,
   -1467
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1117
static const short int yytable[] =
{
     159,    44,    86,   376,   323,   273,   179,   180,   247,   188,
     342,   345,   343,   346,   667,   266,   915,   252,   285,   268,
     313,   269,   250,  1083,   290,   833,  1108,   304,   310,  1091,
     283,   737,   741,  1110,   278,   585,   711,  1197,   925,   831,
     941,   917,   312,   848,   308,  1199,   258,   314,   867,   241,
     244,   885,   248,   251,   180,   256,   259,   673,   309,   702,
    1174,   271,   244,   259,  1057,   251,   316,   180,   256,  1376,
     288,   159,   259,  1581,  1335,  1686,   671,   307,  1625,   603,
     300,   179,   180,   303,   244,   315,   300,   259,   248,   251,
     311,   180,   256,   288,   159,   259,   280,   251,  1104,   869,
    1129,   990,  1452,   387,  1200,  1125,  1386,   630,  1180,  1412,
     831,  1019,   329,   646,   331,   379,   330,   680,   677,  1010,
     280,  1375,  1468,   467,   388,   385,  1201,  1643,  1489,  1202,
    1561,  1284,  1789,  1445,  1207,  1035,  1565,   383,  1264,   970,
    1203,  1217,   381,  1214,  1715,  1035,   754,   927,  1035,  1952,
    1181,   931,   271,   180,   271,   353,  1035,   942,   159,  2002,
     992,   347,  1176,  1442,   901,   259,  1400,  1889,  1510,  2025,
     377,   706,   244,   251,   180,   256,   288,   474,  1025,   895,
     188,  1957,  1661,   839,   597,  1484,   839,  1472,   840,   431,
    1684,   840,   614,   881,  1721,   958,   616,  1268,  1807,   633,
    1739,  1061,   656,  1259,  1035,  1857,   839,  1035,  1455,  1024,
    1338,   840,  1989,   108,  1032,   839,   327,   616,   656,   462,
     840,  1402,  1035,   839,   292,   -82,   635,   977,   840,   479,
    1238,  1084,  1085,  1287,   665,  1793,   839,   920,  1052,    84,
     435,   840,   839,   445,  1289,  1290,  -715,   840,  1285,   962,
    1238,   109,  1036,   737,   741,  1301,   924,   106,   293,   862,
    1646,  1053,  1036,   110,  1820,  1036,  1179,   339,   573,  1426,
     997,   577,   757,  1036,   944,   947,  1905,   638,  1822,  1823,
   -1085,  1784,  1785, -1089,   665,   963,  1453,   111,   711,   831,
     861,  1625,   666,   617,  1165,  1356,  1667,  1108,    67,  1205,
     106,   612,   959,  1302,  1110,   585,  1852,  1432,   271,  1212,
     468,   271,   623,   436,   905,   424,   446,  1054,   586,   159,
    1427,  1036,  1817,  1698,  1036,   634,   631,  1303,  1239,  1359,
    1360,    85,  1403,   900,  1112,  1229,   328,    91,  1858,  1036,
    1765,  1766,  1122,  1695,  1456,   248,  1914,   618,  1990,   180,
    1288,   945,   112,   978,   960,   341,  1666,  1709,   839,  1269,
     425,   863,   113,   840,   619,  1794,   615,   657,   618,  1353,
    1086,   763,  1563,  1717,  1652,    14,   107,  1430,   672,  1722,
     998,   262,   263,   707,  1678,   964,  1454,   114,   573,   839,
     269,   946,   688,   845,   840,   692,  1953,   749,   645,   645,
     703,   469,   705,  1440,  1443,  1779,   753,   163,  1890,  1805,
    1448,  1449,   896,   633,  1161,   328,  1377,   758,   271,  1286,
    1679,  1434,   762,  1675,   751,  1684,  1246,   760,   271,  1371,
     271,  1283,   271,  1946,  1222,   693,  1983,   993,  1898,  1648,
    1649,  1429,  1364,  1377,  1062,   339,  1665,   841,   842,   843,
     844,   190,   843,   844,   244,   251,   256,  1927,  1024,  1713,
     890,  1494,  1024,  1189,   180,   189,   248,   426,  1361,   771,
     841,   842,   843,   844,   109,  1897,   769,  -715,   839,   841,
     842,   843,   844,   840,  1899,   427,  1979,   841,   842,   843,
     844,  1942,  1943,  1506,  1507,  1900,  1508,  1509,   988,   839,
     841,   842,   843,   844,   840,  1932,   841,   842,   843,   844,
     111,  2000,   839,   195,   668,   876,   161,   840,  1517,   428,
    1265,   835,   191,   347,  1275,   192,  1910,   669,   193,   891,
     665,  1281,  1190,   858,   874,  1282,  2022,  1245,  2019,  1986,
     631,   860,   865,   909,   971,  1886,   870,   292,  -240,  2024,
    1818,  1963,  2027,   413,   463,   839,   681,  1821,   339,  2028,
     840,  1824,  1825,   348,   989,   196,  1518,  1574,   665,   174,
     180,   585,   251,   586,  1575,   112,  1850,  1446,   339,   180,
     162,   293,   852,   180,   853,   113,  2020,  2023,   586,   911,
     188,   907,  1991,   414,  1901,  1851,  1001,  1853,   577,  1276,
    1406,  1501,   439,   260,  1859,   440,   921,   245,    84,  1220,
     114,  1712,  1147,   677,  2008,  1457,  1019,  1483,  1182,  1808,
    1843,   928,   841,   842,   843,   844,  1700,  1576,   180,  1362,
     337,   972,  1173,  1502,   943,  1495,  1125,  1849,   271,  1407,
     464,  1816,  2009,  1272,  -240,  1625,   899,   441,   262,   263,
     973,  1458,   181,   841,   842,   843,   844,   340,  1411,   604,
    1111,   339,  1372,   341,  1700,  -240,   339,   610,   839,   175,
    1625,  -240,   936,   840,   271,   465,   176,   344,   936,   839,
    1257,  2010,  1711,   341,   840,  1829,  1754,   339,   883,  1433,
     260,  1830,  1126,   968,   347,  1000,   434,  1625,  1921,  1922,
      85,   347,  1004,  1005,  1415,  -240,  2026,   839,  1625,   347,
     269,  1625,   840,   341,  1230,   276,   887,  1416,  1625,    84,
     347,  1755,   297,   280,   633,  1831,   845,  1166,  2011,   694,
     295,  1379,   839,  1756,  1058,   359,  1169,   840,   182,   695,
    1417,  1066,   841,   842,   843,   844,  1415,  1863,   477,  1068,
     271,  1105,  1170,   884,  1418,   418,   482,  1908,   969,  1416,
    1070,   305,  1915,   841,   842,   843,   844,   419,   317,   183,
     839,    14,  1148,   184,   319,   840,   841,   842,   843,   844,
    1669,  1730,  1417,  1064,   185,  1343,   325,   839,  1197,  1197,
    1670,  1419,   840,   259,  1515,  1436,  1199,  1199,   868,   320,
     420,  1521,   421,   573,  1473,   573,   354,  1134,  1514,  1474,
    1676,  1677,  1166,  1969,  1970,  1971,    88,    89,  1930,   841,
     842,   843,   844,  1815,  1931,  1145,   324,  1694,  1366,   326,
    1114,  1081,   860,  1419,  1059,   856,  1063,   857,   271,  1687,
    1067,  1069,  1071,   271,  1473,   271,   332,   271,   623,  1477,
    1172,   631,   169,   170,   586,  1200,  1200,   269,   911,  1568,
     262,   263,   180,   251,   333,  1188,  1485,   170,   932,   251,
     933,   375,  1814,  1164,   953,  1204,   954,  1201,  1201,  1916,
    1202,  1202,   409,   573,   839,   180,   845,   180,  1106,   840,
    1224,  1203,  1203,   410,  1141,   271,  1142,   271,  1272,  1262,
    1015,  1263,  1267,  1420,   645,   412,  1760,  1976,  1218,  1978,
    1699,  1223,   845,   416,  1380,  1708,   432,  1982,  1660,  1984,
    1381,  1235,  1382,   271,  1253,  1254,  1415,  1256,  1311,  1231,
    1232,  1111,   841,   842,   843,   844,  1262,   449,  1431,  1416,
     448,  1367,  1312,   841,   842,   843,   844,  -755,   271,  1798,
    1800,   271,  1368,  1465,   839,  1466,  1258,   437,  1470,   840,
    1471,   451,  1417,  2016,   839,   633,  1266,  1549,   771,   840,
     473,   841,   842,   843,   844,  1236,  1418,  1377,   460,  1573,
    1369,   471,   280,   839,   475,  1108,  1313,  1228,   840,  1895,
    1799,  1801,  1110,   180,   478,   839,   841,   842,   843,   844,
     840,  1197,  1197,   180,  1243,  1370,   845,   479,  1647,  1199,
    1199,   771,  1280,  1419,  1314,   574,   280,   280,   769,   280,
     839,  1514,  1514,  1997,  1998,   840,   498,   499,   500,  1514,
    1514,   452,  1514,  1514,   841,   842,   843,   844,   581,   839,
    1514,  1514,  1514,  1514,   840,  1514,  1514,  1514,  1514,  1514,
    1514,  1775,   578,   839,   508,   509,   588,   592,   840,  1415,
     845,   845,  1731,  1770,   589,   453,   920,   454,  1200,  1200,
     593,   594,  1416,  1365,   845,   595,  1809,   839,  1315,  2005,
     596,  2006,   840,   259,   598,   856,  1855,  2007,  1166,   455,
    1201,  1201,   631,  1202,  1202,  1417,  1628,   611,   613,  1631,
    -745,   839,   624,   625,  1203,  1203,   840,   626,   627,  1418,
     881,   653,   637,   654,   456,   918,  1956,   641,   642,   737,
     741,  1655,   639,  1657,  1960,  1420,   586,   586,   643,   911,
     649,    14,   647,   661,   670,  1108,   662,  1108,   663,   675,
     251,   686,  1110,   689,  1110,  1108,  1419,  1108,   841,   842,
     843,   844,  1110,   839,  1110,   699,   700,  1546,   840,   704,
     755,  -745,   759,   761,  -745,   798,   796,   821,   799,   800,
     801,  -745,   802,  -745,   808,   836,   809,  -745,   803,   855,
     804,   872,  -745,   805,  1480,  -745,   806, -1071,   839,   807,
     810,  1108,   845,   840,   813,   818,   819,  1496,  1110,   820,
    -745,  1111,   822,  1500,   838,   839,   878,   823,   824,   633,
     840,   851,  1945,   269,  -745,   825,   826,   577,   841,   842,
     843,   844,  -745,   827,   846,  1463,  -745,  1547,   841,   842,
     843,   844,   847,   854,   880,  -745, -1089,  1464,   889,   894,
     898,   906,  1463,  1641,  1641,  1645,   903,   841,   842,   843,
     844,  -745,   904,   271,   908,   912,  1550,   271,  1420,   841,
     842,   843,   844,   916,   913,   919,   656,   922,  1551,   665,
     934,   929,   957,   961,  1833,   839,   280,  -442,  -745,  1672,
     840,  1674,   966,   974,   841,   842,   843,   844,   339,   280,
    -745,   839,  -745,  1553,   975,   280,   840,   980,   981,  1911,
     982,   983,   839,   841,   842,   843,   844,   840,   985,   986,
    1632,   991,  1554,   995,   996,  1352,  1002,   841,   842,   843,
     844,  1003,  1012,  1014,  1545,   180,  1555,  1015,  1714,  1020,
    1987,  1630,   573,  1577,  1021,   269,   631,  1633,  1022,  -745,
   -1114,   841,   842,   843,   844,  1026, -1114,   195,  1092,  1629,
    1556, -1114,  1087,  1103, -1114,   992,   341,  1130,  1135,  -745,
    1136,  2001,  1139,  -745,  1143,   841,   842,   843,   844,   271,
    1137,   863,   271,  1160,  1557,  1582,  1627,  1167,  1177,   586,
     251,   256,   586,   288,   159,  1185,  1183,   856,   937,  1194,
   -1114,  1208,   839,  -745,  -745,  1215,  -745,   840, -1114,  1209,
     586,   586,   839,  1216,  1234,   251, -1114,   840,  1072,  1073,
    1074,  1075,  1076,  1077,  1078,  1079,   839,   841,   842,   843,
     844,   840,   633, -1115, -1114,  1219, -1114,  1241,  1251, -1115,
    1225,  1226,  1227,  1255, -1115,   839,  1260, -1115,  1333, -1114,
     840, -1114, -1114, -1114,  1720, -1116,  1274,  1298,  1342,  1725,
    1334, -1116,   841,   842,   843,   844, -1116,   839,  1350, -1116,
    1340,  1558,   840,  1697,   839,  1377,  1351,  1378,  1383,   841,
     842,   843,   844, -1115,   839,   535,  1385,  1401,  1761,   840,
    1404, -1115,  1409,  1410,  1414,  1810,   839,  1422,  1424, -1115,
   -1114,   840,  1437,  1450,  1459, -1116,  1467,  1482,  1469,   601,
    1475,  1478,  1479, -1116,  1481,  1490,  1498, -1115,   609, -1115,
    1268, -1116,  1504,  1503,  1505,  1035,  1516,  1778,  1511,  1512,
    1519,  1520, -1115,   839, -1115, -1115, -1115,  1522,   840, -1116,
    1523, -1116,  1530,  1531,  1532,  1545,   180,  1533,  1535,   841,
     842,   843,   844,  1562, -1116,  1536, -1116, -1116, -1116,   631,
    1634,  1537,  1538,  1539,  1540,   841,   842,   843,   844,  1564,
    1636,  1637,  1653,  1572,  1762,   839,   841,   842,   843,   844,
     840,   652,  1650, -1115,   244,   655,  1651,  1662,  1658,  1659,
     839,  1663,  1783,  1280,  1280,   840,  1673,  1685,  1696,  1627,
    1688,  1689,  1690,  1691,   839, -1116,  1740,  1692,  1693,   840,
    1238,  1716,  1733,  1767,  1710,  1718,  1753, -1114, -1114, -1114,
   -1114, -1114, -1114, -1114,  1719,  1724,  1729,  1752,  1763,  1111,
    1759, -1114,  1768, -1114,  1776,  1777, -1077,   586,   586,  1913,
    1780,   162,  -342,  1305,  1787,  1309,  1792,  1317,  1320,  1835,
    1325,  1328,  1331,  1641,  1795,  1796,  1797,   881,  1802,   251,
     251,  1806,   377,  1803,  1826,  1837,   841,   842,   843,   844,
    1836,  1839,  1838,  1840,  1841,  1769,   841,   842,   843,   844,
    1842,  1846,  1844,  1882,   815,  1772,   180,  1845,  1854,  1856,
     841,   842,   843,   844,  1860,  1861,   828,   829,   832,  1773,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115,  1862,  1865,   841,
     842,   843,   844,  1883, -1115,  1866, -1115,  1867,  1888,  1868,
    1871,  1872, -1116, -1116, -1116, -1116, -1116, -1116, -1116,  1873,
    1874,   841,   842,   843,   844,  1928, -1116,  1875, -1116,  1876,
    1941,  1892,  1877,  1878,  1879,  1880,  1885,  1788,   841,   842,
     843,   844,  1907,  1545,   180,  1363,  1909,   248,  1923,  1920,
     841,   842,   843,   844,  1037,  1925,  1939,  1771,  1947,   875,
     839,  1954,  1955,   877,  1935,   840,  1944,  1965,  1038,  1111,
    1973,  1111,  1958,  1961,  1977,  1972,   633,  1280,  1974,  1111,
    1992,  1111,    69,  1929,  1926,  1994,  1996,   841,   842,   843,
     844,  1999,    57,    70,  1906,  2004,   377,   251,  1989,  2014,
      71,   633,  1934,    58,  1039,  1990,  1936,  2015,  2017,  2018,
      59,  2021,  1040,    72,    60,   871,  1964,  1114,   334,   335,
    1041,   180,   580,  1638,   336,  1111,  1146,   248,   633,   841,
     842,   843,   844,  1948,    61,  1499,  1138,   587,  1042,   633,
    1043,  1273,   633,   873,   841,   842,   843,   844,  1013,   633,
    1497,  1487,  1278,  1044,  1247,  1045,  1046,  1047,   841,   842,
     843,   844,   291,  1848,  1832,   712,   976,  1439,   636,   682,
      73,  1178,  1488,   248,  1493,  1828,  1988,  1213,  1847,  1995,
    1993,  2013,   684,  1206,  1387,   486,    74,  1904,    29,  1980,
     244,  1950,  1668,  1127,   690,  1252,    62,  1707,   674,  1951,
     956,   338,    75,   631,  1048,   168,   628,   999,   664,  1244,
     888,   893,   577,   488,  1168,   352,  1162,    76,   301,  1413,
    1656,    77,  1962,  1055,    78,  1881,  1758,    63,   631,    49,
      50,    64,    79,  1184,    53,  1144,  1408,  1526,  1034,   812,
    1441,   490,  1476,  1627,  1566,  1975,   306,  1237,   952,  1164,
    1782,    80,   271,     0,   902,   631,   355,  1924,  1060,     0,
       0,     0,  1065,     0,   248,   248,   631,     0,  1627,   631,
       0,     0,  1082,     0,     0,     0,   631,  1089,  1090,     0,
       0,  1093,  1094,  1095,  1096,  1097,  1101,     0,     0,   356,
       0,     0,     0,     0,     0,  1627,  1164,   535,     0,  1115,
    1116,  1117,  1118,  1119,  1120,   535,  1627,     0,  1280,  1627,
      81,     0,   495,     0,     0,   357,  1627,     0,   358,     0,
      65,  1049,  1050,  1051,   841,   842,   843,   844,   497,  1727,
    1728,     0,     0,  1105,   359,     0,     0,  1734,  1735,   609,
    1737,  1738,   498,   499,   500,     0,   262,   263,  1742,  1743,
    1744,  1745,     0,  1746,  1747,  1748,  1749,  1750,  1751,     0,
     360,     0,  1175,     0,     0,   503,   504,   505,   506,   507,
     508,   509,   510,   511,     0,   652,  1293,  1187,     0,  1294,
     513,   514,     0,   483,     0,     0,     0,     0,     0,   518,
     484,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   485,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,   361,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1195,   488,     0,   362,   489,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   363,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,     0,   491,   492,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1291,  1292,  1297,     0,
       0,  1306,  1307,  1310,     0,  1318,  1321,  1323,  1326,  1329,
    1332,     0,     0,     0,     0,     0,     0,  1336,  1337,     0,
    1339,     0,  1341,     0,     0,  1344,  1345,  1346,  1347,  1348,
    1349,     0,     0,  1295,     0,   495,     0,     0,     0,     0,
       0,   496,     0,  1354,     0,     0,  1357,     0,     0,     0,
       0,   497,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1374,     0,     0,     0,   498,   499,   500,   501,   262,
     263,   116,     0,   117,   118,   119,   120,     0,   121,   502,
       0,     0,     0,     0,     0,     0,   122,   123,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   124,   512,     0,
     126,     0,   535,   513,   514,     0,   515,   516,   517,     0,
     129,   130,   518,   131,     0,     0,   132,   133,   134,   135,
     136,     0,     0,     0,   137,   138,     0,     0,     0,     0,
     139,   519,   141,     0,   520,   143,   144,     0,   145,     0,
     521,   522,   523,  1016,   524,   525,   714,   526,   146,   147,
     148,   149,   150,   151,     0,     0,     0,   527,   528,  1017,
       0,     0,     0,   715,     0,   600,   716,   530,     0,     0,
       0,   341,  1304,     0,     0,  1294,     0,   -50,     0,   483,
       0,     0,     0,     0,     0,     0,   484,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   485,     0,
       0,     0,     0,     0,   486,   487,   717,     0,     0,     0,
       0,     0,   718,   719,   720,   -50,   721,   722,   723,     0,
     724,     0,     0,     0,     0,     0,     0,   -50,     0,     0,
       0,     0,   488,     0,     0,   489,     0,     0,   725,     0,
     726,   164,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -50,     0,     0,     0,     0,     0,   535,     0,     0,
     490,     0,   491,   492,     0,     0,     0,     0,     0,   727,
    1524,  1525,   493,  1527,  1529,     0,   494,     0,     0,     0,
       0,  1534,     0,     0,     0,     0,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1552,     0,     0,     0,     0,     0,     0,   729,  1559,
       0,  1560,  1354,     0,     0,     0,   -50,     0,  1354,     0,
    1357,  1567,   535,  1569,     0,     0,   -50,     0,     0,  1295,
    1570,   495,  1571,     0,     0,     0,     0,   496,     0,     0,
       0,     0,     0,   730,   731,     0,     0,   497,     0,     0,
       0,   -50,     0,     0,     0,     0,     0,     0,     0,     0,
     732,   498,   499,   500,   501,   262,   263,   116,     0,   117,
     118,   119,   120,   165,   121,   502,     0,     0,     0,     0,
       0,   535,   122,   123,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   124,   512,     0,   126,     0,     0,   513,
     514,     0,   515,   516,   517,  1680,   129,   130,   518,   131,
       0,     0,   132,   133,   134,   135,   136,     0,     0,     0,
     137,   138,     0,     0,     0,     0,   139,   519,   141,     0,
     520,   143,   144,     0,   145,     0,   521,   522,   523,     0,
     524,   525,     0,   526,   146,   147,   148,   149,   150,   151,
       0,     0,     0,   527,   528,     0,     0,     0,     0,     0,
       0,   600,     0,   530,     0,     0,     0,   341,     0,  1726,
       0,    92,     0,     0,     0,  1732,     0,     0,    93,     1,
    1736,     0,    94,    95,     0,     0,     0,  1741,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     2,     0,     0,
       0,     0,    97,     0,     0,     3,     0,     0,     0,     0,
       0,     0,     4,     0,     5,     0,     6,     0,     0,     0,
       0,     7,  1354,  1354,     0,     0,     0,     0,     8,     0,
       0,  1774,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,     0,     0,     0,     0,  1781,     0,     0,
       0,     0,    10,  1308,     0,     0,  1294,     0,     0,     0,
     483,     0,     0,     0,    98,     0,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
      99,     0,  1037,     0,     0,   486,   487,     0,   839,     0,
       0,     0,     0,   840,     0,   100,  1038,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   488,     0,    11,   489,     0,     0,     0,
       0,  1813,    12,     0,     0,    13,     0,    14,    15,   103,
       0,     0,  1039,     0,     0,     0,     0,     0,     0,     0,
    1040,   490,     0,   491,   492,     0,     0,     0,  1041,     0,
       0,     0,    16,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,     0,     0,     0,  1042,     0,  1043,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1044,     0,  1045,  1046,  1047,     0,     0,   104,     0,
       0,     0,  1864,     0,     0,     0,     0,     0,  1869,  1870,
       0,    17,     0,     0,     0,     0,     0,     0,     0,     0,
    1295,     0,   495,     0,     0,     0,    18,    19,   496,     0,
       0,     0,  1884,     0,     0,     0,     0,  1354,   497,   937,
    1887,     0,  1048,     0,     0,     0,     0,  1893,  1894,     0,
       0,    20,   498,   499,   500,   501,   262,   263,   116,     0,
     117,   118,   119,   120,     0,   121,   502,     0,     0,     0,
       0,     0,   535,   122,   123,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   124,   512,     0,   126,     0,     0,
     513,   514,     0,   515,   516,   517,     0,   129,   130,   518,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   519,   141,
       0,   520,   143,   144,  1933,   145,     0,   521,   522,   523,
       0,   524,   525,     0,   526,   146,   147,   148,   149,   150,
     151,     0,     0,  1316,   527,   528,  1294,     0,     0,     0,
     483,     0,   600,     0,   530,     0,     0,   484,   341,  1049,
    1050,  1051,   841,   842,   843,   844,     0,     0,     0,   485,
       0,     0,     0,     0,     0,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1813,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,     0,
     115,     0,     0,   116,     0,   117,   118,   119,   120,     0,
     121,     0,     0,  1981,     0,     0,     0,     0,   122,   123,
       0,   490,     0,   491,   492,     0,     0,     0,     0,   124,
     125,     0,   126,   493,     0,     0,     0,   494,     0,   127,
     128,     0,   129,   130,     0,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,     0,
       0,     0,   139,   140,   141,     0,   142,   143,   144,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,   147,   148,   149,   150,   151,     0,     0,     0,     0,
    1295,     0,   495,     0,     0,     0,     0,   320,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
       0,   681,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   498,   499,   500,   501,   262,   263,   116,     0,
     117,   118,   119,   120,     0,   121,   502,     0,     0,     0,
       0,     0,     0,   122,   123,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   124,   512,     0,   126,     0,     0,
     513,   514,     0,   515,   516,   517,     0,   129,   130,   518,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   519,   141,
       0,   520,   143,   144,     0,   145,     0,   521,   522,   523,
       0,   524,   525,     0,   526,   146,   147,   148,   149,   150,
     151,     0,     0,  1319,   527,   528,  1294,     0,     0,     0,
     483,     0,   600,     0,   530,     0,     0,   484,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
       0,     0,     0,     0,     0,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   115,
       0,     0,   116,     0,   117,   118,   119,   120,     0,   121,
       0,     0,     0,     0,     0,     0,     0,   122,   123,     0,
       0,   490,     0,   491,   492,     0,     0,     0,   124,   125,
       0,   126,     0,   493,     0,     0,     0,   494,   127,   128,
       0,   129,   130,     0,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   140,   141,     0,   142,   143,   144,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,   148,   149,   150,   151,     0,     0,     0,     0,     0,
    1295,     0,   495,     0,     0,     0,     0,     0,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
       0,   687,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   498,   499,   500,   501,   262,   263,   116,     0,
     117,   118,   119,   120,     0,   121,   502,     0,     0,     0,
       0,     0,     0,   122,   123,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   124,   512,     0,   126,     0,     0,
     513,   514,     0,   515,   516,   517,     0,   129,   130,   518,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   519,   141,
       0,   520,   143,   144,     0,   145,     0,   521,   522,   523,
       0,   524,   525,     0,   526,   146,   147,   148,   149,   150,
     151,     0,     0,  1324,   527,   528,  1294,     0,     0,     0,
     483,     0,   600,     0,   530,     0,     0,   484,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
       0,     0,     0,     0,     0,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   115,
       0,     0,   116,     0,   117,   118,   119,   120,     0,   121,
       0,     0,     0,     0,     0,     0,     0,   122,   123,     0,
       0,   490,     0,   491,   492,     0,     0,     0,   124,   125,
       0,   126,     0,   493,     0,     0,     0,   494,   127,   128,
       0,   129,   130,     0,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   140,   141,     0,   142,   143,   144,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,   148,   149,   150,   151,     0,     0,     0,     0,     0,
    1295,     0,   495,     0,     0,     0,     0,     0,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1123,   498,   499,   500,   501,   262,   263,   116,     0,
     117,   118,   119,   120,     0,   121,   502,     0,     0,     0,
       0,     0,     0,   122,   123,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   124,   512,     0,   126,     0,     0,
     513,   514,     0,   515,   516,   517,     0,   129,   130,   518,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   519,   141,
       0,   520,   143,   144,     0,   145,     0,   521,   522,   523,
       0,   524,   525,     0,   526,   146,   147,   148,   149,   150,
     151,     0,     0,  1327,   527,   528,  1294,     0,     0,     0,
     483,     0,   600,     0,   530,     0,     0,   484,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
       0,     0,     0,     0,     0,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,     0,
     115,     0,     0,   116,     0,   117,   118,   119,   120,     0,
     121,     0,     0,     0,     0,     0,     0,     0,   122,   123,
       0,   490,     0,   491,   492,     0,     0,     0,     0,   124,
     125,     0,   126,   493,     0,     0,     0,   494,     0,   127,
     128,     0,   129,   130,     0,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,     0,
       0,     0,   139,   140,   141,     0,   142,   143,   144,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,   147,   148,   149,   150,   151,     0,     0,     0,     0,
    1295,  1124,   495,     0,     0,     0,     0,     0,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   498,   499,   500,   501,   262,   263,   116,     0,
     117,   118,   119,   120,     0,   121,   502,     0,     0,     0,
       0,     0,     0,   122,   123,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   124,   512,     0,   126,     0,     0,
     513,   514,     0,   515,   516,   517,     0,   129,   130,   518,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
     713,   137,   138,   714,     0,     0,     0,   139,   519,   141,
       0,   520,   143,   144,     0,   145,     0,   521,   522,   523,
     715,   524,   525,   716,   526,   146,   147,   148,   149,   150,
     151,     0,     0,  1330,   527,   528,  1294,     0,     0,     0,
     483,     0,   600,     0,   530,     0,     0,   484,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   485,
       0,     0,     0,   717,     0,   486,   487,     0,     0,   718,
     719,   720,     0,   721,   722,   723,     0,   724,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,   725,   489,   726,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   490,     0,   491,   492,     0,   727,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,   494,     0,     0,
       0,     0,     0,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1295,     0,   495,     0,     0,     0,     0,     0,   496,     0,
     730,   731,     0,     0,     0,     0,     0,     0,   497,     0,
       0,     0,     0,     0,     0,     0,     0,   732,     0,     0,
       0,     0,   498,   499,   500,   501,   262,   263,   116,     0,
     117,   118,   119,   120,     0,   121,   502,     0,     0,     0,
       0,     0,     0,   122,   123,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   124,   512,     0,   126,     0,     0,
     513,   514,     0,   515,   516,   517,     0,   129,   130,   518,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   519,   141,
       0,   520,   143,   144,     0,   145,     0,   521,   522,   523,
       0,   524,   525,     0,   526,   146,   147,   148,   149,   150,
     151,   483,     0,     0,   527,   528,     0,     0,   484,     0,
       0,     0,   600,     0,   530,     0,     0,     0,   341,     0,
     485,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   773,     0,     0,     0,     0,
       0,     0,     0,     0,   488,     0,     0,   489,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,     0,   491,   492,     0,     0,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,   494,     0,
       0,     0,     0,     0,     0,     0,  1027,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    14,     0,     0,     0,   775,     0,     0,
       0,     0,     0,   495,     0,     0,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   498,   499,   500,   501,   262,   263,   116,
       0,   117,   118,   119,   120,     0,   121,   502,     0,     0,
       0,     0,     0,     0,   122,   123,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   124,   512,     0,   126,     0,
       0,   513,   514,     0,   515,   516,   517,     0,   129,   130,
     518,   131,     0,     0,   132,   133,   134,   135,   136,  1028,
    1029,  1030,   137,   138,     0,     0,     0,     0,   139,   519,
     141,     0,   520,   143,   144,     0,   145,     0,   521,   522,
     523,     0,   524,   525,     0,   526,   146,   147,   148,   149,
     150,   151,   483,     0,     0,   527,   528,     0,     0,   484,
       0,     0,     0,   779,     0,   530,     0,     0,    14,   341,
       0,   485,     0,     0,   772,     0,     0,   486,   487,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   773,     0,     0,     0,
       0,     0,     0,     0,     0,   488,     0,     0,   489,     0,
       0,   115,     0,     0,   116,     0,   117,   118,   119,   120,
       0,   121,     0,     0,     0,     0,     0,     0,     0,   122,
     123,     0,     0,   490,     0,   491,   492,     0,     0,     0,
     124,   125,     0,   126,     0,   493,     0,     0,     0,   494,
     127,   128,     0,   129,   130,     0,   131,   774,     0,   132,
     133,   134,   135,   136,     0,     0,     0,   137,   138,     0,
       0,     0,     0,   139,   140,   141,     0,   142,   143,   144,
       0,   145,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,   147,   148,   149,   150,   151,     0,   775,     0,
       0,     0,     0,     0,   495,     0,     0,     0,  1149,     0,
     496,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   498,   499,   500,   501,   262,   263,
     116,     0,   117,   118,   119,   120,     0,   121,   502,     0,
       0,     0,     0,     0,     0,   122,   123,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   124,   512,     0,   126,
       0,     0,   513,   514,     0,   515,   516,   517,     0,   129,
     130,   518,   131,     0,     0,   132,   133,   776,   777,   778,
       0,     0,     0,   137,   138,     0,     0,     0,     0,   139,
     519,   141,     0,   520,   143,   144,     0,   145,     0,   521,
     522,   523,     0,   524,   525,     0,   526,   146,   147,   148,
     149,   150,   151,   483,     0,     0,   527,   528,     0,     0,
     484,     0,     0,     0,   779,     0,   530,     0,     0,     0,
     341,     0,   485,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   582,     0,   773,     0,     0,
       0,     0,     0,     0,     0,     0,   488,     0,     0,   489,
       0,     0,   115,     0,     0,   116,     0,   117,   118,   119,
     120,     0,   121,     0,     0,     0,     0,     0,     0,     0,
     122,   123,     0,     0,   490,     0,   491,   492,     0,     0,
       0,   124,   125,     0,   126,     0,   493,     0,     0,     0,
     494,   127,   128,     0,   129,   130,     0,   131,  1023,     0,
     132,   133,   134,   135,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,   139,   140,   141,     0,   142,   143,
     144,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   146,   147,   148,   149,   150,   151,     0,   775,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   498,   499,   500,   501,   262,
     263,   116,     0,   117,   118,   119,   120,     0,   121,   502,
       0,     0,     0,     0,     0,     0,   122,   123,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   124,   512,     0,
     126,     0,     0,   513,   514,     0,   515,   516,   517,     0,
     129,   130,   518,   131,     0,     0,   132,   133,   776,   777,
     778,     0,     0,     0,   137,   138,     0,     0,     0,     0,
     139,   519,   141,     0,   520,   143,   144,     0,   145,     0,
     521,   522,   523,     0,   524,   525,     0,   526,   146,   147,
     148,   149,   150,   151,   483,     0,     0,   527,   528,     0,
       0,   484,     0,     0,     0,   779,     0,   530,     0,     0,
       0,   341,     0,   485,     0,     0,     0,     0,     0,   486,
     487,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   773,     0,
       0,     0,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,   115,     0,     0,   116,     0,   117,   118,
     119,   120,     0,   121,     0,     0,     0,     0,     0,     0,
       0,   122,   123,     0,     0,   490,     0,   491,   492,     0,
       0,     0,   124,   125,     0,   126,     0,   493,     0,     0,
       0,   494,   127,   128,     0,   129,   130,     0,   131,   774,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,     0,     0,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   146,   147,   148,   149,   150,   151,     0,
     775,     0,     0,     0,     0,     0,   495,     0,     0,     0,
    1149,     0,   496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   498,   499,   500,   501,
     262,   263,   116,     0,   117,   118,   119,   120,     0,   121,
     502,     0,     0,     0,     0,     0,     0,   122,   123,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   124,   512,
       0,   126,     0,     0,   513,   514,     0,   515,   516,   517,
       0,   129,   130,   518,   131,     0,     0,   132,   133,   776,
     777,   778,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   519,   141,     0,   520,   143,   144,     0,   145,
       0,   521,   522,   523,     0,   524,   525,     0,   526,   146,
     147,   148,   149,   150,   151,   483,     0,     0,   527,   528,
       0,     0,   484,     0,     0,     0,   779,     0,   530,     0,
       0,     0,   341,     0,   485,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   488,     0,
       0,   489,     0,     0,     0,   115,     0,     0,   116,     0,
     117,   118,   119,   120,     0,   121,     0,     0,     0,     0,
       0,     0,     0,   122,   123,     0,   490,     0,   491,   492,
       0,     0,     0,     0,   124,   125,     0,   126,   493,     0,
       0,     0,   494,     0,   127,   128,     0,   129,   130,     0,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   140,   141,
       0,   142,   143,   144,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   146,   147,   148,   149,   150,
     151,     0,     0,     0,     0,     0,     0,   495,     0,     0,
       0,     0,  1107,   496,   530,     0,     0,     0,     0,     0,
       0,     0,     0,   497,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   498,   499,   500,
     501,   262,   263,   116,     0,   117,   118,   119,   120,     0,
     121,   502,     0,     0,     0,     0,     0,     0,   122,   123,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   124,
     512,     0,   126,     0,     0,   513,   514,     0,   515,   516,
     517,     0,   129,   130,   518,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,     0,
       0,     0,   139,   519,   141,     0,   520,   143,   144,  1098,
     145,     0,   521,   522,   523,  1099,   524,   525,  1100,   526,
     146,   147,   148,   149,   150,   151,   483,     0,     0,   527,
     528,     0,     0,   484,     0,     0,     0,   600,     0,   530,
       0,     0,     0,   341,     0,   485,     0,     0,     0,     0,
       0,   486,   487,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     773,     0,     0,     0,     0,     0,     0,     0,     0,   488,
       0,     0,   489,     0,     0,   115,     0,     0,   116,     0,
     117,   118,   119,   120,     0,   121,     0,     0,     0,     0,
       0,     0,     0,   122,   123,     0,     0,   490,     0,   491,
     492,     0,     0,     0,   124,   125,     0,   126,     0,   493,
       0,     0,     0,   494,   127,   128,     0,   129,   130,     0,
     131,  1023,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   140,   141,
       0,   142,   143,   144,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   146,   147,   148,   149,   150,
     151,     0,   775,     0,     0,     0,     0,     0,   495,     0,
    -343,     0,     0,     0,   496,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   498,   499,
     500,   501,   262,   263,   116,     0,   117,   118,   119,   120,
       0,   121,   502,     0,     0,     0,     0,     0,     0,   122,
     123,   503,   504,   505,   506,   507,   508,   509,   510,   511,
     124,   512,     0,   126,     0,     0,   513,   514,     0,   515,
     516,   517,     0,   129,   130,   518,   131,     0,     0,   132,
     133,   134,   135,   136,     0,     0,     0,   137,   138,     0,
       0,     0,     0,   139,   519,   141,     0,   520,   143,   144,
       0,   145,     0,   521,   522,   523,     0,   524,   525,     0,
     526,   146,   147,   148,   149,   150,   151,   483,     0,     0,
     527,   528,     0,     0,   484,     0,     0,     0,   779,     0,
     530,     0,     0,     0,   341,     0,   485,     0,     0,     0,
       0,     0,   486,   487,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     488,     0,     0,   489,     0,     0,     0,   115,     0,     0,
     116,     0,   117,   118,   119,   120,     0,   121,     0,     0,
       0,     0,     0,     0,     0,   122,   123,     0,   490,     0,
     491,   492,     0,     0,     0,     0,   124,   125,     0,   126,
     493,     0,     0,     0,   494,     0,   127,   128,     0,   129,
     130,     0,   131,     0,     0,   132,   133,   134,   135,   136,
       0,     0,     0,   137,   138,     0,     0,     0,     0,   139,
     140,   141,     0,   142,   143,   144,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   146,   147,   148,
     149,   150,   151,     0,     0,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,   496,   530,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   498,
     499,   500,   501,   262,   263,   116,     0,   117,   118,   119,
     120,     0,   121,   502,     0,     0,     0,     0,     0,     0,
     122,   123,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   124,   512,     0,   126,     0,     0,   513,   514,     0,
     515,   516,   517,     0,   129,   130,   518,   131,     0,     0,
     132,   133,   134,   135,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,   139,   519,   141,     0,   520,   143,
     144,     0,   145,     0,   521,   522,   523,     0,   524,   525,
       0,   526,   146,   147,   148,   149,   150,   151,   483,     0,
       0,   527,   528,   605,     0,   484,     0,     0,     0,   600,
       0,   530,     0,     0,     0,   341,     0,   485,     0,     0,
       0,     0,     0,   486,   487,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,     0,     0,   489,    84,     0,   115,     0,     0,
     116,     0,   117,   118,   119,   120,     0,   121,     0,     0,
       0,     0,     0,     0,     0,   122,   123,     0,     0,   490,
       0,   491,   492,     0,     0,     0,   124,   125,     0,   126,
       0,   493,     0,     0,     0,   494,   127,   128,     0,   129,
     130,     0,   131,     0,     0,   132,   133,   134,   135,   136,
       0,     0,     0,   137,   138,     0,     0,     0,     0,   139,
     140,   141,     0,   142,   143,   144,     0,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   146,   147,   148,
     149,   150,   151,     0,     0,     0,     0,    85,     0,     0,
     495,     0,     0,     0,     0,     0,   496,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   497,     0,     0,     0,
       0,     0,     0,     0,   814,     0,     0,     0,     0,     0,
     498,   499,   500,   501,   262,   263,   116,     0,   117,   118,
     119,   120,     0,   121,   502,     0,     0,     0,     0,     0,
       0,   122,   123,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   124,   512,     0,   126,     0,     0,   513,   514,
       0,   515,   516,   517,     0,   129,   130,   518,   131,     0,
       0,   132,   133,   134,   135,   136,     0,     0,     0,   137,
     138,     0,     0,     0,     0,   139,   519,   141,     0,   520,
     143,   144,     0,   145,     0,   521,   522,   523,     0,   524,
     525,     0,   526,   146,   147,   148,   149,   150,   151,   483,
       0,     0,   527,   528,     0,     0,   484,     0,     0,     0,
     600,     0,   530,     0,     0,     0,   341,     0,   485,     0,
       0,     0,     0,     0,   486,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   488,     0,     0,   489,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     490,     0,   491,   492,     0,     0,     0,     0,     0,     0,
       0,     0,   493,     0,     0,     0,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   495,     0,     0,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   498,   499,   500,   501,   262,   263,   116,     0,   117,
     118,   119,   120,     0,   121,   502,     0,     0,     0,     0,
       0,     0,   122,   123,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   124,   512,     0,   126,     0,     0,   513,
     514,     0,   515,   516,   517,     0,   129,   130,   518,   131,
       0,     0,   132,   133,   134,   135,   136,     0,     0,     0,
     137,   138,     0,     0,     0,     0,   139,   519,   141,     0,
     520,   143,   144,     0,   145,     0,   521,   522,   523,     0,
     524,   525,     0,   526,   146,   147,   148,   149,   150,   151,
     483,     0,     0,   527,   528,     0,     0,   484,     0,     0,
       0,   600,     0,   530,     0,     0,     0,   341,     0,   485,
       0,     0,     0,     0,     0,   486,   487,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   488,     0,     0,   489,     0,     0,   115,
       0,     0,   116,     0,   117,   118,   119,   120,     0,   121,
       0,     0,     0,     0,     0,     0,     0,   122,   123,     0,
       0,   490,     0,   491,   492,     0,     0,     0,   124,   125,
       0,   126,     0,   493,     0,     0,     0,   494,   127,   128,
       0,   129,   130,     0,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   140,   141,     0,   142,   143,   144,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,   148,   149,   150,   151,     0,     0,     0,     0,     0,
    1124,     0,   495,     0,     0,     0,     0,     0,   496,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   498,   499,   500,   501,   262,   263,   116,     0,
     117,   118,   119,   120,     0,   121,   502,     0,     0,     0,
       0,     0,     0,   122,   123,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   124,   512,     0,   126,     0,     0,
     513,   514,     0,   515,   516,   517,     0,   129,   130,   518,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   519,   141,
       0,   520,   143,   144,     0,   145,     0,   521,   522,   523,
       0,   524,   525,     0,   526,   146,   147,   148,   149,   150,
     151,   483,     0,     0,   527,   528,     0,     0,   484,     0,
       0,     0,   600,  1121,   530,     0,     0,     0,   341,     0,
     485,     0,     0,     0,     0,     0,   486,   487,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   488,     0,     0,   489,     0,     0,
     115,     0,     0,   116,     0,   117,   118,   119,   120,     0,
     121,     0,     0,     0,     0,     0,     0,     0,   122,   123,
       0,     0,   490,     0,   491,   492,     0,     0,     0,   124,
     125,     0,   126,     0,   493,     0,     0,     0,   494,   127,
     128,     0,   129,   130,     0,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,     0,
       0,     0,   139,   140,   141,     0,   142,   143,   144,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     146,   147,   148,   149,   150,   151,     0,     0,     0,     0,
       0,     0,     0,   495,     0,     0,     0,     0,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1322,     0,     0,   498,   499,   500,   501,   262,   263,   116,
       0,   117,   118,   119,   120,     0,   121,   502,     0,     0,
       0,     0,     0,     0,   122,   123,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   124,   512,     0,   126,     0,
       0,   513,   514,     0,   515,   516,   517,     0,   129,   130,
     518,   131,     0,     0,   132,   133,   134,   135,   136,     0,
       0,     0,   137,   138,     0,     0,     0,     0,   139,   519,
     141,     0,   520,   143,   144,     0,   145,     0,   521,   522,
     523,     0,   524,   525,     0,   526,   146,   147,   148,   149,
     150,   151,   483,     0,     0,   527,   528,     0,     0,   484,
       0,     0,     0,   600,     0,   530,     0,     0,     0,   341,
       0,   485,     0,     0,     0,     0,     0,   486,   487,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1373,     0,     0,   488,     0,     0,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,     0,   491,   492,     0,     0,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,   494,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   495,     0,     0,     0,     0,     0,
     496,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   498,   499,   500,   501,   262,   263,
     116,     0,   117,   118,   119,   120,     0,   121,   502,     0,
       0,     0,     0,     0,     0,   122,   123,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   124,   512,     0,   126,
       0,     0,   513,   514,     0,   515,   516,   517,     0,   129,
     130,   518,   131,     0,     0,   132,   133,   134,   135,   136,
       0,     0,     0,   137,   138,     0,     0,     0,     0,   139,
     519,   141,     0,   520,   143,   144,     0,   145,     0,   521,
     522,   523,     0,   524,   525,     0,   526,   146,   147,   148,
     149,   150,   151,   483,     0,     0,   527,   528,     0,     0,
     484,     0,     0,     0,   600,     0,   530,     0,     0,     0,
     341,     0,   485,     0,     0,     0,     0,     0,   486,   487,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   488,     0,     0,   489,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   490,     0,   491,   492,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1528,     0,     0,   498,   499,   500,   501,   262,
     263,   116,     0,   117,   118,   119,   120,     0,   121,   502,
       0,     0,     0,     0,     0,     0,   122,   123,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   124,   512,     0,
     126,     0,     0,   513,   514,     0,   515,   516,   517,     0,
     129,   130,   518,   131,     0,     0,   132,   133,   134,   135,
     136,     0,     0,     0,   137,   138,     0,     0,     0,     0,
     139,   519,   141,     0,   520,   143,   144,     0,   145,     0,
     521,   522,   523,     0,   524,   525,     0,   526,   146,   147,
     148,   149,   150,   151,   483,     0,     0,   527,   528,     0,
       0,   484,     0,     0,     0,   600,     0,   530,     0,     0,
       0,   341,     0,   485,     0,     0,     0,     0,     0,   486,
     487,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   488,     0,     0,
     489,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,     0,   491,   492,     0,
       0,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,   494,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   495,     0,     0,     0,
       0,     0,   496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   497,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   498,   499,   500,   501,
     262,   263,   116,     0,   117,   118,   119,   120,     0,   121,
     502,     0,     0,     0,     0,     0,     0,   122,   123,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   124,   512,
       0,   126,     0,     0,   513,   514,     0,   515,   516,   517,
       0,   129,   130,   518,   131,     0,     0,   132,   133,   134,
     135,   136,     0,     0,     0,   137,   138,     0,     0,     0,
       0,   139,   519,   141,     0,   520,   143,   144,     0,   145,
       0,   521,   522,   523,     0,   524,   525,     0,   526,   146,
     147,   148,   149,   150,   151,   483,     0,     0,   527,   528,
       0,     0,   484,     0,     0,     0,   529,     0,   530,     0,
       0,     0,   341,     0,   485,     0,     0,     0,     0,     0,
     486,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   488,     0,
       0,   489,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,     0,   491,   492,
       0,     0,     0,     0,     0,     0,     0,     0,   493,     0,
       0,     0,   494,     0,     0,     0,     0,     0,     0,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   109,     0,
       0,     0,     0,     0,     0,     0,     0,   495,     0,     0,
     110,     0,     0,   496,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   497,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,     0,   498,   499,   500,
     501,   262,   263,   116,     0,   117,   118,   119,   120,     0,
     121,   502,     0,     0,     0,     0,     0,     0,   122,   123,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   124,
     512,     0,   126,     0,     0,   513,   514,     0,   515,   516,
     517,     0,   129,   130,   518,   131,     0,     0,   132,   133,
     134,   135,   136,     0,     0,     0,   137,   138,     0,   112,
       0,     0,   139,   519,   141,  1385,   520,   143,   144,   113,
     145,     0,   521,   522,   523,     0,   524,   525,     0,   526,
     146,   147,   148,   149,   150,   151,     0,     0,     0,   527,
     528,     0,     0,     6,   114,     0,     0,   600,     0,   530,
    -295,     0,     0,   341,  1583,  1584,     0,  1585,     0,     0,
       0,     0,   115,     0,     0,   116,     0,   117,   118,   119,
     120,     0,   121,  1586,     0,     0,     0,     0,     0,  1587,
     122,   123,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   124,   125,     0,   126,     0,     0,     0,     0,     0,
       0,   127,   128,     0,   129,   130,     0,   131,     0,     0,
     132,   133,   134,   135,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,   139,   140,   141,     0,   142,   143,
     144,  1385,   145,     0,     0,     0,  1588,     0,     0,     0,
       0,     0,   146,   147,   148,   149,   150,   151,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     6,
       0,     0,     0,     0,     0,     0,  -297,     0,     0,  1589,
    1583,  1584,     0,  1585,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,  1788,  1586,
       0,     0,     0,     0,     0,  1587,     0,   501,     0,     0,
     116,     0,   117,   118,   119,   120,     0,   121,     0,     0,
       0,     0,     0,     0,     0,   122,   123,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1590,   125,     0,   126,
       0,     0,     0,     0,     0,     0,   127,   128,     0,   129,
     130,     0,   131,     0,     0,   132,  1591,   134,   135,   136,
       0,     0,  1588,   137,   138,  1592,  1593,  1594,     0,   139,
     140,   141,     0,   142,   143,   144,  1385,   145,     0,     0,
      14,     0,     0,     0,     0,     0,     0,   146,   147,   148,
     149,   150,   151,     0,     0,  1589,     0,     0,     0,     0,
       0,     0,     0,     0,     6,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1583,  1584,     0,  1585,     0,
       0,     0,     0,   501,     0,     0,   116,     0,   117,   118,
     119,   120,     0,   121,  1586,     0,     0,     0,     0,     0,
    1587,   122,   123,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1590,   125,     0,   126,     0,     0,     0,     0,
       0,     0,   127,   128,     0,   129,   130,     0,   131,     0,
       0,   132,  1591,   134,   135,   136,     0,     0,     0,   137,
     138,  1592,  1593,  1594,     0,   139,   140,   141,     0,   142,
     143,   144,     0,   145,     0,     0,     0,  1588,     0,     0,
       0,     0,     0,   146,   147,   148,   149,   150,   151,     0,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   501,     0,
       0,   116,     0,   117,   118,   119,   120,     0,   121,     0,
       0,     0,     0,     0,     0,     0,   122,   123,     0,     0,
       0,     0,   713,     0,     0,   714,     0,  1590,   125,     0,
     126,   937,     0,     0,     0,     0,     0,   127,   128,     0,
     129,   130,   715,   131,     0,   716,   132,  1591,   134,   135,
     136,     0,     0,     0,   137,   138,  1592,  1593,  1594,     0,
     139,   140,   141,     0,   142,   143,   144,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   146,   147,
     148,   149,   150,   151,     0,   717,     0,     0,     0,     0,
       0,   718,   719,   720,     0,   721,   722,   723,     0,   724,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   725,     0,   726,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   389,     0,     0,     0,     0,   727,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   390,
       0,     0,     0,     0,     0,   728,     0,     0,     0,     0,
     391,     0,     0,     0,     0,     0,     0,   392,     0,     0,
       0,   393,   394,     0,     0,     0,     0,   729,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   396,   115,     0,     0,   116,     0,   117,   118,   119,
     120,     0,   121,     0,     0,     0,     0,     0,     0,     0,
     122,   123,   730,   731,     0,     0,   713,     0,     0,   714,
       0,   124,   125,     0,   126,     0,     0,     0,     0,   732,
       0,   127,   128,     0,   129,   130,   715,   131,     0,   716,
     132,   133,   134,   135,   136,   397,     0,     0,   137,   138,
       0,     0,     0,   398,   139,   140,   141,     0,   142,   143,
     144,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   146,   147,   148,   149,   150,   151,     0,   717,
       0,     0,     0,     0,   399,   718,   719,   720,   400,   721,
     722,   723,     0,   724,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   725,     0,   726,     0,     0,     0,     0,   402,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   727,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,     0,  1388,
     405,   729,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   115,     0,     0,   116,
       0,   117,   118,   119,   120,     0,   121,     0,     0,     0,
       0,     0,     0,     0,   122,   123,   730,   731,     0,     0,
       0,     0,     0,     0,     0,   124,   125,     0,   126,     0,
       0,     0,     0,   732,     0,   127,   128,     0,   129,   130,
       0,   131,     0,     0,   132,   133,   134,   135,   136,  1389,
       0,     0,   137,   138,     0,     0,     0,     0,   139,   140,
     141,     0,   142,   143,   144,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   146,   147,   148,   149,
     150,   151,  1052,     0,  1390,     0,     0,     0,     0,  1391,
       0,     0,     0,     0,     0,  1392,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1053,   115,     0,     0,   116,
       0,   117,   118,   119,  1393,     0,   121,     0,     0,     0,
       0,     0,     0,     0,   122,   123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   124,   125,     0,   126,     0,
       0,     0,     0,     0,     0,   127,   128,     0,   129,   130,
       0,   131,     0,     0,   132,   133,   134,   135,   136,     0,
       0,  1054,   137,   138,     0,     0,     0,     0,   139,   140,
     141,     0,   142,   143,   144,     0,   145,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   146,   147,   148,   149,
     150,   151,     0,     0,     0,   115,     0,     0,   116,     0,
     117,   118,   119,   120,     0,   121,     0,     0,     0,     0,
       0,     0,     0,   122,   123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   124,   125,     0,   126,     0,     0,
       0,     0,     0,     0,   127,   128,     0,   129,   130,     0,
     131,     0,     0,   132,   133,   134,   135,   136,     0,     0,
       0,   137,   138,     0,     0,     0,     0,   139,   140,   141,
       0,   142,   143,   144,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   146,   147,   148,   149,   150,
     151,   115,     0,     0,   116,   199,   117,   118,   119,   120,
     200,   121,   201,   202,   203,   204,   205,   206,   207,   122,
     123,   208,   209,   210,   211,   212,     0,     0,   213,   214,
     124,   125,   215,   126,     0,     0,   216,   217,   218,   219,
     127,   128,   220,   129,   130,   221,   131,   222,   223,   132,
     133,   134,   135,   136,     0,     0,     0,   137,   138,   224,
     225,   226,   227,   139,   140,   141,   228,   142,   143,   144,
     229,   145,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   146,   147,   148,   149,   150,   151
};

static const short int yycheck[] =
{
       9,     0,     4,   179,   114,    71,    16,    16,    61,    18,
     169,   170,   169,   170,   424,    69,   647,    63,    77,    70,
     102,    70,    62,   814,    80,   529,   836,    93,    99,   820,
      76,   461,   461,   836,    74,   327,   457,   925,   670,   529,
     683,   652,   101,   569,    97,   925,    65,   103,   589,    58,
      59,   611,    61,    62,    63,    64,    65,   428,    98,   453,
     901,    70,    71,    72,   797,    74,   106,    76,    77,  1107,
      79,    80,    81,  1384,  1057,  1449,   426,    96,  1385,   343,
      89,    91,    91,    92,    93,   104,    95,    96,    97,    98,
     100,   100,   101,   102,   103,   104,    75,   106,   831,   590,
     854,   733,  1220,   185,   925,   847,  1131,   375,   908,  1149,
     600,   767,   161,   408,   163,   181,   162,   438,   437,   760,
      99,  1104,  1242,   278,   189,   184,   925,  1403,  1264,   925,
    1353,     8,  1598,  1207,   934,     8,  1359,   183,  1010,   699,
     925,   946,   182,   943,  1489,     8,   467,   673,     8,    47,
     912,   677,   161,   162,   163,   174,     8,   683,   167,    10,
      21,     6,   904,    43,   629,   174,  1136,    43,     8,    47,
     179,    43,   181,   182,   183,   184,   185,   287,   774,    61,
     189,    82,  1423,    23,    63,  1259,    23,  1247,    28,   243,
    1447,    28,    14,    10,    19,    49,    26,   133,  1658,   375,
       8,    46,    90,  1003,     8,    97,    23,     8,    44,   774,
      10,    28,    44,     6,   779,    23,   174,    26,    90,   273,
      28,    63,     8,    23,    11,   149,   381,   169,    28,   190,
     196,    50,    51,     8,    22,    61,    23,   658,    85,   196,
       3,    28,    23,     3,  1035,  1036,    63,    28,   125,    44,
     196,    44,   125,   683,   683,    46,   666,   141,    45,   192,
    1404,   108,   125,    56,  1672,   125,   907,   196,   317,   143,
     185,   320,    90,   125,    62,   685,    56,   387,  1676,  1677,
     297,  1592,  1593,   300,    22,    80,    80,    80,   709,   779,
     582,  1598,    30,   123,   223,  1086,  1432,  1107,   193,   931,
     141,   354,   156,    94,  1107,   597,  1714,  1179,   317,   941,
      14,   320,   358,    76,   635,     4,    76,   164,   327,   328,
     194,   125,  1667,  1467,   125,   379,   375,   118,   294,    50,
      51,   288,   174,   294,   838,   967,   294,    63,   230,   125,
    1563,  1564,   846,  1461,   180,   354,  1806,   177,   180,   358,
     125,   139,   145,   295,   208,   301,  1430,  1477,    23,   295,
      49,   294,   155,    28,   194,   191,   188,   255,   177,   173,
     189,   295,   173,  1491,  1414,   155,   260,  1177,   427,   204,
     295,   199,   200,   255,  1444,   180,   180,   180,   437,    23,
     439,   179,   441,   294,    28,   444,   294,   463,   407,   408,
     454,   105,   456,  1194,   284,   246,   465,    18,   284,  1650,
    1215,  1216,   294,   589,   879,   294,   294,   471,   427,   296,
    1445,  1183,   475,   296,   464,  1682,   986,   473,   437,    63,
     439,  1027,   441,   296,   960,   444,   296,   298,     9,  1409,
    1410,  1174,   294,   294,   289,   196,  1429,   287,   288,   289,
     290,   122,   289,   290,   463,   464,   465,  1831,  1023,  1484,
     117,  1266,  1027,    98,   473,   244,   475,   156,   189,   478,
     287,   288,   289,   290,    44,  1786,   478,   294,    23,   287,
     288,   289,   290,    28,    55,   174,  1952,   287,   288,   289,
     290,  1889,  1890,  1284,  1285,    66,  1287,  1288,    21,    23,
     287,   288,   289,   290,    28,  1850,   287,   288,   289,   290,
      80,  1977,    23,   126,    30,   296,    18,    28,    46,   208,
    1011,   530,     0,     6,    19,   293,  1802,    43,    61,   186,
      22,  1022,   167,   579,   599,  1026,    43,   288,  2004,  1958,
     589,   581,   588,    94,   701,  1768,   592,    11,    22,  2015,
    1668,  1925,  2018,     6,    55,    23,    30,  1675,   196,  2025,
      28,  1679,  1680,    46,    87,   178,    94,  1377,    22,    68,
     579,   863,   581,   582,  1377,   145,  1712,  1209,   196,   588,
      82,    45,   294,   592,   296,   155,  2005,    94,   597,   643,
     599,   637,  1966,    46,   165,  1713,   751,  1715,   647,    94,
    1141,   230,     4,   147,  1724,     7,   660,    65,   196,   959,
     180,  1483,   876,   932,  1988,    44,  1272,  1258,   913,  1659,
    1694,   675,   287,   288,   289,   290,   118,  1381,   637,   294,
     174,   702,   900,   262,   683,  1266,  1378,  1711,   647,  1143,
     141,  1666,   117,  1014,   118,  1952,   625,    49,   199,   200,
     704,    80,    55,   287,   288,   289,   290,   295,  1149,   344,
     836,   196,   296,   301,   118,   139,   196,   352,    23,   168,
    1977,   145,   681,    28,   683,   176,   175,   295,   687,    23,
    1001,   156,  1482,   301,    28,   139,    79,   196,   223,  1180,
     147,   145,   851,   223,     6,   749,   245,  2004,  1816,  1817,
     288,     6,   756,   757,    64,   179,  2017,    23,  2015,     6,
     759,  2018,    28,   301,   223,     7,   143,    77,  2025,   196,
       6,   114,    65,   702,   900,   179,   294,   884,   203,     5,
     196,   299,    23,   126,    46,   162,   142,    28,   141,    15,
     100,    46,   287,   288,   289,   290,    64,  1730,   297,    46,
     759,   296,   158,   288,   114,     4,   305,  1795,   288,    77,
      46,    65,   122,   287,   288,   289,   290,    16,   295,   172,
      23,   155,   296,   176,   140,    28,   287,   288,   289,   290,
     231,  1514,   100,   802,   187,   296,   294,    23,  1676,  1677,
     241,   151,    28,   802,  1298,  1189,  1676,  1677,   182,   295,
      49,    54,    51,   852,   294,   854,    79,   856,  1298,   299,
    1442,  1443,   969,  1931,  1932,  1933,    59,    60,  1843,   287,
     288,   289,   290,  1664,  1849,   871,   122,  1459,   296,   122,
     839,   810,   872,   151,   798,   294,   800,   296,   847,  1450,
     804,   805,   806,   852,   294,   854,   124,   856,   894,   299,
     896,   900,   231,   232,   863,  1676,  1677,   906,   912,  1363,
     199,   200,   871,   872,   124,   919,  1260,   232,   294,   878,
     296,   156,  1663,   882,   203,   929,   205,  1676,  1677,   239,
    1676,  1677,   180,   932,    23,   894,   294,   896,   296,    28,
     961,  1676,  1677,   192,   294,   904,   296,   906,  1269,   294,
     294,   296,   296,   263,   913,    19,  1549,  1945,   957,  1947,
    1470,   960,   294,   259,   296,  1475,   198,  1955,  1422,  1957,
     294,   975,   296,   932,   995,   996,    64,   998,    17,   130,
     131,  1107,   287,   288,   289,   290,   294,   162,   296,    77,
     266,   296,    31,   287,   288,   289,   290,    85,   957,  1636,
    1637,   960,   296,   294,    23,   296,  1002,   295,   294,    28,
     296,   199,   100,  2001,    23,  1141,  1012,  1338,   977,    28,
      61,   287,   288,   289,   290,   977,   114,   294,    10,   296,
     296,    59,   961,    23,    79,  1795,    75,   966,    28,  1780,
    1636,  1637,  1795,  1002,    78,    23,   287,   288,   289,   290,
      28,  1889,  1890,  1012,   983,   296,   294,   190,   296,  1889,
    1890,  1020,  1021,   151,   103,   149,   995,   996,  1020,   998,
      23,  1511,  1512,  1973,  1974,    28,   195,   196,   197,  1519,
    1520,    98,  1522,  1523,   287,   288,   289,   290,   141,    23,
    1530,  1531,  1532,  1533,    28,  1535,  1536,  1537,  1538,  1539,
    1540,  1577,   172,    23,   223,   224,    82,    63,    28,    64,
     294,   294,   296,   296,   295,   132,  1487,   134,  1889,  1890,
      61,    61,    77,  1092,   294,   122,   296,    23,   167,   294,
     122,   296,    28,  1092,   244,   294,  1718,   296,  1245,   156,
    1889,  1890,  1141,  1889,  1890,   100,  1388,   174,   102,  1391,
       0,    23,   172,   283,  1889,  1890,    28,   282,   280,   114,
      10,   413,    61,   415,   181,   174,  1907,   156,    65,  1549,
    1549,  1417,   123,  1419,  1915,   263,  1135,  1136,    83,  1183,
     243,   155,   121,   254,    43,  1945,   255,  1947,   254,    52,
    1149,   207,  1945,   294,  1947,  1955,   151,  1957,   287,   288,
     289,   290,  1955,    23,  1957,   192,   284,   296,    28,   112,
      29,    61,   295,    10,    64,   295,   135,    97,   295,   295,
     199,    71,   295,    73,   199,   148,   199,    77,   295,    10,
     295,   141,    82,   295,  1255,    85,   295,   295,    23,   295,
     295,  2001,   294,    28,   295,   295,   295,  1268,  2001,   295,
     100,  1377,   295,  1274,   298,    23,    63,   295,   295,  1385,
      28,   284,    82,  1262,   114,   295,   295,  1266,   287,   288,
     289,   290,   122,   295,   295,  1234,   126,   296,   287,   288,
     289,   290,   300,   295,   294,   135,   300,  1239,   281,   294,
     166,   295,  1251,  1402,  1403,  1404,   284,   287,   288,   289,
     290,   151,   300,  1262,    10,    83,   296,  1266,   263,   287,
     288,   289,   290,   294,   300,    90,    90,    90,   296,    22,
      10,   111,   174,   136,  1684,    23,  1255,   174,   178,  1436,
      28,  1438,   136,    21,   287,   288,   289,   290,   196,  1268,
     190,    23,   192,   296,    90,  1274,    28,   185,   185,  1803,
     138,   295,    23,   287,   288,   289,   290,    28,    91,   295,
    1392,   298,   296,   295,   295,    63,    61,   287,   288,   289,
     290,    10,   122,   149,  1334,  1334,   296,   294,  1485,   128,
    1961,  1390,  1381,  1384,   121,  1384,  1385,  1393,   295,   239,
      17,   287,   288,   289,   290,   295,    23,   126,    61,  1389,
     296,    28,   189,   296,    31,    21,   301,    42,   174,   259,
     174,    82,    60,   263,   295,   287,   288,   289,   290,  1378,
     202,   294,  1381,   190,   296,  1384,  1385,   186,    10,  1388,
    1389,  1390,  1391,  1392,  1393,   240,    83,   294,    27,   295,
      67,    19,    23,   293,   294,    86,   296,    28,    75,   295,
    1409,  1410,    23,    86,   156,  1414,    83,    28,   210,   211,
     212,   213,   214,   215,   216,   217,    23,   287,   288,   289,
     290,    28,  1598,    17,   101,    43,   103,   154,   156,    23,
     125,   125,   125,   295,    28,    23,    98,    31,   108,   116,
      28,   118,   119,   120,  1498,    17,   295,   295,   294,  1503,
     295,    23,   287,   288,   289,   290,    28,    23,    63,    31,
     296,   296,    28,  1465,    23,   294,   296,   300,   184,   287,
     288,   289,   290,    67,    23,   316,    16,   124,   296,    28,
     107,    75,    63,    63,   294,  1661,    23,   295,    71,    83,
     167,    28,    12,   239,    10,    67,   160,    10,   296,   340,
     297,   296,   296,    75,   296,   192,    52,   101,   349,   103,
     133,    83,   296,    52,   296,     8,    63,  1583,   295,   295,
     295,   295,   116,    23,   118,   119,   120,   295,    28,   101,
     295,   103,   295,   295,   295,  1545,  1545,   295,   295,   287,
     288,   289,   290,    51,   116,   295,   118,   119,   120,  1598,
     192,   295,   295,   295,   295,   287,   288,   289,   290,   173,
     294,   294,   127,    63,   296,    23,   287,   288,   289,   290,
      28,   412,   296,   167,  1583,   416,   296,    19,    85,    85,
      23,    73,  1591,  1592,  1593,    28,   130,   145,   296,  1598,
     125,   125,   125,   125,    23,   167,    54,   125,   125,    28,
     196,    22,    63,    51,   296,   295,   294,   284,   285,   286,
     287,   288,   289,   290,   296,   296,   296,   296,    61,  1795,
     296,   298,   173,   300,   293,    36,    36,  1636,  1637,  1805,
     295,    82,   293,  1042,    51,  1044,   293,  1046,  1047,  1685,
    1049,  1050,  1051,  1802,    82,   297,    70,    10,   174,  1658,
    1659,    85,  1661,   278,    23,   180,   287,   288,   289,   290,
      79,   180,    80,    44,    80,   296,   287,   288,   289,   290,
      44,    94,   296,   295,   515,   296,  1685,   296,   124,   111,
     287,   288,   289,   290,   111,   296,   527,   528,   529,   296,
     284,   285,   286,   287,   288,   289,   290,   296,   296,   287,
     288,   289,   290,  1756,   298,   296,   300,   296,   296,   296,
     296,   296,   284,   285,   286,   287,   288,   289,   290,   296,
     296,   287,   288,   289,   290,  1835,   298,   296,   300,   296,
     296,    61,   296,   296,   296,   296,   296,   189,   287,   288,
     289,   290,   295,  1753,  1753,   294,   124,  1756,   296,   294,
     287,   288,   289,   290,    17,    86,    79,   294,    82,   600,
      23,   246,    82,   604,   204,    28,   295,   122,    31,  1945,
     294,  1947,   295,   295,   173,   296,  1952,  1786,   295,  1955,
     122,  1957,    37,  1836,  1830,   122,   296,   287,   288,   289,
     290,   296,    37,    48,  1793,    47,  1805,  1806,    44,   296,
      55,  1977,  1856,    48,    67,   180,  1860,    50,    36,    47,
      55,   201,    75,    68,    59,   595,  1926,  1826,   166,   166,
      83,  1830,   325,  1400,   167,  2001,   872,  1836,  2004,   287,
     288,   289,   290,  1899,    79,  1272,   863,   328,   101,  2015,
     103,  1015,  2018,   597,   287,   288,   289,   290,   763,  2025,
    1269,  1261,  1020,   116,   990,   118,   119,   120,   287,   288,
     289,   290,    81,  1702,  1684,   458,   709,  1192,   383,   439,
     125,   906,  1262,  1882,  1266,  1682,  1964,   942,  1701,  1968,
    1967,  1990,   439,   932,  1132,    38,   141,  1790,     0,  1953,
    1899,  1900,  1433,   852,   444,   993,   141,  1473,   429,  1901,
     689,   168,   157,  1952,   167,    13,   365,   746,   422,   985,
     613,   618,  1961,    66,   894,   172,   880,   172,    91,  1149,
    1418,   176,  1920,   796,   179,  1753,  1545,   172,  1977,     0,
       0,   176,   187,   916,     0,   870,  1144,  1313,   779,   507,
    1195,    94,  1251,  1952,  1361,  1944,    95,   977,   687,  1958,
    1591,   206,  1961,    -1,   629,  2004,    84,  1826,   799,    -1,
      -1,    -1,   803,    -1,  1973,  1974,  2015,    -1,  1977,  2018,
      -1,    -1,   813,    -1,    -1,    -1,  2025,   818,   819,    -1,
      -1,   822,   823,   824,   825,   826,   827,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,  2004,  2005,   838,    -1,   840,
     841,   842,   843,   844,   845,   846,  2015,    -1,  2017,  2018,
     265,    -1,   165,    -1,    -1,   143,  2025,    -1,   146,    -1,
     265,   284,   285,   286,   287,   288,   289,   290,   181,  1511,
    1512,    -1,    -1,   296,   162,    -1,    -1,  1519,  1520,   880,
    1522,  1523,   195,   196,   197,    -1,   199,   200,  1530,  1531,
    1532,  1533,    -1,  1535,  1536,  1537,  1538,  1539,  1540,    -1,
     188,    -1,   903,    -1,    -1,   218,   219,   220,   221,   222,
     223,   224,   225,   226,    -1,   916,     6,   918,    -1,     9,
     233,   234,    -1,    13,    -1,    -1,    -1,    -1,    -1,   242,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    66,    -1,   266,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1037,  1038,  1039,    -1,
      -1,  1042,  1043,  1044,    -1,  1046,  1047,  1048,  1049,  1050,
    1051,    -1,    -1,    -1,    -1,    -1,    -1,  1058,  1059,    -1,
    1061,    -1,  1063,    -1,    -1,  1066,  1067,  1068,  1069,  1070,
    1071,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,  1084,    -1,    -1,  1087,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1102,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,  1143,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    18,   274,   275,    21,   277,   278,   279,
     280,   281,   282,   283,    -1,    -1,    -1,   287,   288,    34,
      -1,    -1,    -1,    38,    -1,   295,    41,   297,    -1,    -1,
      -1,   301,     6,    -1,    -1,     9,    -1,     6,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    39,    81,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    44,    91,    92,    93,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   113,    -1,
     115,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,  1298,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,   144,
    1311,  1312,   106,  1314,  1315,    -1,   110,    -1,    -1,    -1,
      -1,  1322,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1342,    -1,    -1,    -1,    -1,    -1,    -1,   183,  1350,
      -1,  1352,  1353,    -1,    -1,    -1,   145,    -1,  1359,    -1,
    1361,  1362,  1363,  1364,    -1,    -1,   155,    -1,    -1,   163,
    1371,   165,  1373,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,   218,   219,    -1,    -1,   181,    -1,    -1,
      -1,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     235,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,   202,   208,   209,    -1,    -1,    -1,    -1,
      -1,  1422,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,  1446,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   270,   271,   272,    -1,
     274,   275,    -1,   277,   278,   279,   280,   281,   282,   283,
      -1,    -1,    -1,   287,   288,    -1,    -1,    -1,    -1,    -1,
      -1,   295,    -1,   297,    -1,    -1,    -1,   301,    -1,  1510,
      -1,    48,    -1,    -1,    -1,  1516,    -1,    -1,    55,     7,
    1521,    -1,    59,    60,    -1,    -1,    -1,  1528,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    42,    -1,    44,    -1,    -1,    -1,
      -1,    49,  1563,  1564,    -1,    -1,    -1,    -1,    56,    -1,
      -1,  1572,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,  1588,    -1,    -1,
      -1,    -1,    80,     6,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,    -1,    -1,   141,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
     157,    -1,    17,    -1,    -1,    38,    39,    -1,    23,    -1,
      -1,    -1,    -1,    28,    -1,   172,    31,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    66,    -1,   143,    69,    -1,    -1,    -1,
      -1,  1662,   150,    -1,    -1,   153,    -1,   155,   156,   206,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    94,    -1,    96,    97,    -1,    -1,    -1,    83,    -1,
      -1,    -1,   180,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   116,    -1,   118,   119,   120,    -1,    -1,   265,    -1,
      -1,    -1,  1733,    -1,    -1,    -1,    -1,    -1,  1739,  1740,
      -1,   229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,   244,   245,   171,    -1,
      -1,    -1,  1763,    -1,    -1,    -1,    -1,  1768,   181,    27,
    1771,    -1,   167,    -1,    -1,    -1,    -1,  1778,  1779,    -1,
      -1,   269,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,  1803,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,  1855,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    -1,    -1,     6,   287,   288,     9,    -1,    -1,    -1,
      13,    -1,   295,    -1,   297,    -1,    -1,    20,   301,   284,
     285,   286,   287,   288,   289,   290,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1920,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,  1954,    -1,    -1,    -1,    -1,   216,   217,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,   227,
     228,    -1,   230,   106,    -1,    -1,    -1,   110,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,   295,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    -1,    -1,     6,   287,   288,     9,    -1,    -1,    -1,
      13,    -1,   295,    -1,   297,    -1,    -1,    20,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
     279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    -1,    -1,     6,   287,   288,     9,    -1,    -1,    -1,
      13,    -1,   295,    -1,   297,    -1,    -1,    20,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
     279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    -1,    -1,     6,   287,   288,     9,    -1,    -1,    -1,
      13,    -1,   295,    -1,   297,    -1,    -1,    20,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,   227,
     228,    -1,   230,   106,    -1,    -1,    -1,   110,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,
     163,   289,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      18,   254,   255,    21,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      38,   274,   275,    41,   277,   278,   279,   280,   281,   282,
     283,    -1,    -1,     6,   287,   288,     9,    -1,    -1,    -1,
      13,    -1,   295,    -1,   297,    -1,    -1,    20,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    81,    -1,    38,    39,    -1,    -1,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,   113,    69,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,    -1,
      -1,    -1,   295,    -1,   297,    -1,    -1,    -1,   301,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    -1,    -1,   159,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,   278,   279,   280,   281,
     282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,
      -1,    -1,    -1,   295,    -1,   297,    -1,    -1,   155,   301,
      -1,    32,    -1,    -1,    35,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,   118,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   278,   279,   280,   281,   282,   283,    -1,   159,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,   295,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,   270,
     271,   272,    -1,   274,   275,    -1,   277,   278,   279,   280,
     281,   282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,
      20,    -1,    -1,    -1,   295,    -1,   297,    -1,    -1,    -1,
     301,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,   118,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,   159,
      -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,   278,   279,
     280,   281,   282,   283,    13,    -1,    -1,   287,   288,    -1,
      -1,    20,    -1,    -1,    -1,   295,    -1,   297,    -1,    -1,
      -1,   301,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,   118,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   278,   279,   280,   281,   282,   283,    -1,
     159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
     295,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    -1,   274,   275,    -1,   277,   278,
     279,   280,   281,   282,   283,    13,    -1,    -1,   287,   288,
      -1,    -1,    20,    -1,    -1,    -1,   295,    -1,   297,    -1,
      -1,    -1,   301,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,   227,   228,    -1,   230,   106,    -1,
      -1,    -1,   110,    -1,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,
     283,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,   295,   171,   297,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,   267,
     268,    -1,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,    13,    -1,    -1,   287,
     288,    -1,    -1,    20,    -1,    -1,    -1,   295,    -1,   297,
      -1,    -1,    -1,   301,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,
     283,    -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,
     293,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   270,   271,   272,    -1,   274,   275,    -1,
     277,   278,   279,   280,   281,   282,   283,    13,    -1,    -1,
     287,   288,    -1,    -1,    20,    -1,    -1,    -1,   295,    -1,
     297,    -1,    -1,    -1,   301,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
     106,    -1,    -1,    -1,   110,    -1,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,   297,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,
     236,   237,   238,    -1,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,   278,   279,   280,   281,   282,   283,    13,    -1,
      -1,   287,   288,   289,    -1,    20,    -1,    -1,    -1,   295,
      -1,   297,    -1,    -1,    -1,   301,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    69,   196,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,    -1,    -1,   288,    -1,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    -1,   277,   278,   279,   280,   281,   282,   283,    13,
      -1,    -1,   287,   288,    -1,    -1,    20,    -1,    -1,    -1,
     295,    -1,   297,    -1,    -1,    -1,   301,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   195,   196,   197,   198,   199,   200,   201,    -1,   203,
     204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,    -1,   230,    -1,    -1,   233,
     234,    -1,   236,   237,   238,    -1,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,   270,   271,   272,    -1,
     274,   275,    -1,   277,   278,   279,   280,   281,   282,   283,
      13,    -1,    -1,   287,   288,    -1,    -1,    20,    -1,    -1,
      -1,   295,    -1,   297,    -1,    -1,    -1,   301,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
     279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,    -1,
     289,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,   278,   279,   280,   281,   282,
     283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,    -1,
      -1,    -1,   295,   296,   297,    -1,    -1,    -1,   301,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
      -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,
     228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     192,    -1,    -1,   195,   196,   197,   198,   199,   200,   201,
      -1,   203,   204,   205,   206,    -1,   208,   209,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,    -1,   230,    -1,
      -1,   233,   234,    -1,   236,   237,   238,    -1,   240,   241,
     242,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,   270,   271,
     272,    -1,   274,   275,    -1,   277,   278,   279,   280,   281,
     282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,    20,
      -1,    -1,    -1,   295,    -1,   297,    -1,    -1,    -1,   301,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,   270,
     271,   272,    -1,   274,   275,    -1,   277,   278,   279,   280,
     281,   282,   283,    13,    -1,    -1,   287,   288,    -1,    -1,
      20,    -1,    -1,    -1,   295,    -1,   297,    -1,    -1,    -1,
     301,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
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
      -1,    -1,   192,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,   278,   279,
     280,   281,   282,   283,    13,    -1,    -1,   287,   288,    -1,
      -1,    20,    -1,    -1,    -1,   295,    -1,   297,    -1,    -1,
      -1,   301,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
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
      -1,   270,   271,   272,    -1,   274,   275,    -1,   277,   278,
     279,   280,   281,   282,   283,    13,    -1,    -1,   287,   288,
      -1,    -1,    20,    -1,    -1,    -1,   295,    -1,   297,    -1,
      -1,    -1,   301,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      56,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,   145,
      -1,    -1,   260,   261,   262,    16,   264,   265,   266,   155,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
     278,   279,   280,   281,   282,   283,    -1,    -1,    -1,   287,
     288,    -1,    -1,    44,   180,    -1,    -1,   295,    -1,   297,
      51,    -1,    -1,   301,    55,    56,    -1,    58,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    74,    -1,    -1,    -1,    -1,    -1,    80,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    16,   268,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,   170,
      55,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    74,
      -1,    -1,    -1,    -1,    -1,    80,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,
      -1,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,   137,   254,   255,   256,   257,   258,    -1,   260,
     261,   262,    -1,   264,   265,   266,    16,   268,    -1,    -1,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,
     281,   282,   283,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    74,    -1,    -1,    -1,    -1,    -1,
      80,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,   256,   257,   258,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,   137,    -1,    -1,
      -1,    -1,    -1,   278,   279,   280,   281,   282,   283,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      -1,    -1,    18,    -1,    -1,    21,    -1,   227,   228,    -1,
     230,    27,    -1,    -1,    -1,    -1,    -1,   237,   238,    -1,
     240,   241,    38,   243,    -1,    41,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,   256,   257,   258,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,
     280,   281,   282,   283,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,   183,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    79,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,    -1,    -1,    18,    -1,    -1,    21,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,   235,
      -1,   237,   238,    -1,   240,   241,    38,   243,    -1,    41,
     246,   247,   248,   249,   250,   133,    -1,    -1,   254,   255,
      -1,    -1,    -1,   141,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,   282,   283,    -1,    81,
      -1,    -1,    -1,    -1,   172,    87,    88,    89,   176,    91,
      92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,    -1,    -1,    -1,    -1,   206,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,    -1,    71,
     268,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,   235,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,   141,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
     282,   283,    85,    -1,   176,    -1,    -1,    -1,    -1,   181,
      -1,    -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,   164,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
     282,   283,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,
      -1,    -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,   282,
     283,   198,    -1,    -1,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,    -1,    -1,   225,   226,
     227,   228,   229,   230,    -1,    -1,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     7,    25,    33,    40,    42,    44,    49,    56,    70,
      80,   143,   150,   153,   155,   156,   180,   229,   244,   245,
     269,   303,   304,   305,   314,   326,   339,   341,   343,   466,
     469,   489,   512,   536,   537,   538,   539,   540,   542,   544,
     545,   549,   568,   569,   575,   579,   582,   583,   584,   639,
     640,   641,   642,   643,   644,   645,   651,    37,    48,    55,
      59,    79,   141,   172,   176,   265,   490,   193,   546,    37,
      48,    55,    68,   125,   141,   157,   172,   176,   179,   187,
     206,   265,   340,   349,   196,   288,   699,   700,    59,    60,
     327,    63,    48,    55,    59,    60,    68,    79,   141,   157,
     172,   176,   187,   206,   265,   513,   141,   260,     6,    44,
      56,    80,   145,   155,   180,   198,   201,   203,   204,   205,
     206,   208,   216,   217,   227,   228,   230,   237,   238,   240,
     241,   243,   246,   247,   248,   249,   250,   254,   255,   260,
     261,   262,   264,   265,   266,   268,   278,   279,   280,   281,
     282,   283,   307,   308,   309,   310,   322,   323,   753,   761,
     762,    18,    82,    18,    70,   202,   315,   316,   546,   231,
     232,   589,   590,   591,    68,   168,   175,   614,   615,   755,
     761,    55,   141,   172,   176,   187,   342,   760,   761,   244,
     122,     0,   293,    61,   576,   126,   178,   632,   506,   202,
     207,   209,   210,   211,   212,   213,   214,   215,   218,   219,
     220,   221,   222,   225,   226,   229,   233,   234,   235,   236,
     239,   242,   244,   245,   256,   257,   258,   259,   263,   267,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   495,
     496,   761,   348,   744,   761,    65,   501,   748,   761,   418,
     752,   761,   615,   755,   509,   756,   761,   502,   747,   761,
     147,   547,   199,   200,   369,   371,   698,   357,   395,   664,
     740,   761,   345,   744,   367,   747,     7,   415,   752,   351,
     700,   702,   385,   615,   480,   756,   473,   759,   761,   368,
     753,   367,    11,    45,   635,   196,   701,    65,   337,   745,
     761,   614,   743,   761,   744,    65,   745,   747,   748,   752,
     702,   755,   756,   759,   753,   747,   752,   295,   470,   140,
     295,   657,   658,   657,   122,   294,   122,   174,   294,   664,
     615,   664,   124,   124,   307,   309,   322,   174,   547,   196,
     295,   301,   694,   705,   295,   694,   705,     6,    46,   592,
     733,   586,   591,   747,    79,    84,   117,   143,   146,   162,
     188,   243,   266,   279,   550,   551,   552,   553,   554,   555,
     556,   557,   559,   560,   561,   156,   754,   761,   346,   744,
     416,   752,   386,   615,   481,   756,   474,   759,   760,    21,
      37,    48,    55,    59,    60,    68,    79,   133,   141,   172,
     176,   187,   206,   207,   265,   268,   570,   571,   572,   180,
     192,   578,    19,     6,    46,   734,   259,   638,     4,    16,
      49,    51,   507,   508,     4,    49,   156,   174,   208,   491,
     492,   698,   198,   503,   736,     3,    76,   295,   419,     4,
       7,    49,   493,   494,   497,     3,    76,   483,   266,   162,
     548,   199,    98,   132,   134,   156,   181,   372,   373,   374,
      10,   358,   698,    55,   141,   176,   344,   419,    14,   105,
     352,    59,   387,    61,   657,    79,   328,   736,    78,   190,
     622,   647,   736,    13,    20,    32,    38,    39,    66,    69,
      94,    96,    97,   106,   110,   165,   171,   181,   195,   196,
     197,   198,   209,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   228,   233,   234,   236,   237,   238,   242,   261,
     264,   270,   271,   272,   274,   275,   277,   287,   288,   295,
     297,   451,   467,   663,   664,   686,   687,   689,   690,   692,
     693,   694,   695,   696,   697,   698,   706,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   717,   719,   720,   721,
     722,   723,   724,   726,   731,   735,   736,   754,   761,   396,
     424,   471,   472,   664,   149,   420,   659,   664,   172,   306,
     310,   141,   181,   324,   325,   757,   761,   323,    82,   295,
     660,   661,    63,    61,    61,   122,   122,    63,   244,   543,
     295,   686,   585,   585,   586,   289,   593,   594,   595,   686,
     586,   174,   748,   102,    14,   188,    26,   123,   177,   194,
     564,   565,   567,   615,   172,   283,   282,   280,   552,   648,
     649,   664,   665,   754,   698,   419,   387,    61,   657,   123,
     541,   156,    65,    83,   751,   761,   751,   121,   577,   243,
     633,   634,   686,   583,   583,   686,    90,   255,   356,   378,
     379,   254,   255,   254,   508,    22,    30,   366,    30,    43,
      43,   359,   664,   398,   492,    52,   504,   396,   421,   422,
     420,    30,   390,   395,   404,   405,   207,    30,   664,   294,
     495,   499,   664,   761,     5,    15,   484,   485,   510,   192,
     284,   370,   370,   698,   112,   698,    43,   255,   375,   376,
     377,   378,   374,    18,    21,    38,    41,    81,    87,    88,
      89,    91,    92,    93,    95,   113,   115,   144,   161,   183,
     218,   219,   235,   514,   515,   516,   519,   520,   521,   522,
     526,   527,   528,   529,   530,   531,   533,   534,   347,   744,
     417,   752,   482,   756,   420,    29,   353,    90,   698,   295,
     615,    10,   748,   295,   330,   331,   332,   398,   338,   699,
     737,   761,    35,    57,   118,   159,   248,   249,   250,   295,
     666,   668,   669,   670,   671,   673,   674,   675,   676,   677,
     678,   679,   680,   681,   682,   686,   135,   623,   295,   295,
     295,   199,   295,   295,   295,   295,   295,   295,   199,   199,
     295,   688,   688,   295,   189,   686,   727,   729,   295,   295,
     295,    97,   295,   295,   295,   295,   295,   295,   686,   686,
     581,   582,   686,   690,   758,   761,   148,   468,   298,    23,
      28,   287,   288,   289,   290,   294,   295,   300,   515,   520,
     527,   284,   294,   296,   295,    10,   294,   296,   615,   313,
     752,   757,   192,   294,   312,   615,   662,   665,   182,   579,
     615,   306,   141,   324,   760,   686,   296,   686,    63,   597,
     294,    10,   596,   223,   288,   704,   705,   143,   556,   281,
     117,   186,   558,   558,   294,    61,   294,   566,   166,   700,
     294,   622,   647,   284,   300,   420,   295,   615,    10,    94,
     574,   698,    83,   300,   573,   659,   294,   635,   174,    90,
     378,   698,    90,   476,   366,   476,   477,   515,   698,   111,
     505,   515,   294,   296,    10,   741,   761,    27,   391,   392,
     393,   514,   515,   664,    62,   139,   179,   366,   406,   407,
     408,   409,   741,   203,   205,   500,   494,   174,    49,   156,
     208,   136,    44,    80,   180,   486,   136,   487,   223,   288,
     704,   705,   702,   698,    21,    90,   377,   169,   295,   524,
     185,   185,   138,   295,   535,    91,   295,   532,    21,    87,
     476,   298,    21,   298,   525,   295,   295,   185,   295,   532,
     698,   419,    61,    10,   698,   698,   388,   389,   390,   404,
     483,   475,   122,   331,   149,   294,    18,    34,   329,   519,
     128,   121,   295,   118,   668,   682,   295,   118,   251,   252,
     253,   667,   668,   683,   686,     8,   125,    17,    31,    67,
      75,    83,   101,   103,   116,   118,   119,   120,   167,   284,
     285,   286,    85,   108,   164,   624,   625,   632,    46,   733,
     686,    46,   289,   733,   747,   686,    46,   733,    46,   733,
      46,   733,   210,   211,   212,   213,   214,   215,   216,   217,
     732,   700,   686,   666,    50,    51,   189,   189,   725,   686,
     686,   666,    61,   686,   686,   686,   686,   686,   267,   273,
     276,   686,   718,   296,   632,   296,   296,   295,   451,   452,
     663,   754,   690,   739,   761,   686,   686,   686,   686,   686,
     686,   296,   690,    39,   289,   740,   694,   472,   423,   424,
      42,   426,   427,   428,   664,   174,   174,   202,   325,    60,
     652,   294,   296,   295,   652,   615,   313,   585,   296,   295,
     598,   599,   600,   601,   605,   606,   607,   608,   612,   752,
     190,   622,   595,   749,   761,   223,   705,   186,   565,   142,
     158,   562,   615,   649,   623,   686,   740,    10,   388,   483,
     475,   574,   751,    83,   634,   240,   637,   686,   698,    98,
     167,   380,   381,   382,   295,   288,   399,   687,   691,   693,
     695,   696,   697,   735,   698,   476,   422,   475,    19,   295,
     359,   360,   476,   391,   475,    86,    86,   658,   664,    43,
     359,   498,   515,   664,   702,   125,   125,   125,   700,   476,
     223,   130,   131,   384,   156,   698,   699,   737,   196,   294,
     703,   154,   523,   700,   535,   288,   704,   360,   517,   518,
     704,   156,   517,   702,   702,   295,   702,   420,   615,   475,
      98,   354,   294,   296,   484,   579,   615,   296,   133,   295,
     334,   335,   398,   332,   295,    19,    94,   333,   338,   742,
     761,   579,   579,   682,     8,   125,   296,     8,   125,   666,
     666,   686,   686,     6,     9,   163,   672,   686,   295,   684,
     685,    46,    94,   118,     6,   672,   686,   686,     6,   672,
     686,    17,    31,    75,   103,   167,     6,   672,   686,     6,
     672,   686,   192,   686,     6,   672,   686,     6,   672,   686,
       6,   672,   686,   108,   295,   638,   686,   686,    10,   686,
     296,   686,   294,   296,   686,   686,   686,   686,   686,   686,
      63,   296,    63,   173,   686,   730,   666,   686,   728,    50,
      51,   189,   294,   294,   294,   747,   296,   296,   296,   296,
     296,    63,   296,    63,   686,   638,   452,   294,   300,   299,
     296,   294,   296,   184,   431,    16,   435,   428,    71,   141,
     176,   181,   187,   206,   317,   318,   319,   320,   321,   757,
     317,   124,    63,   174,   107,   655,   665,   690,   655,    63,
      63,   579,   599,   605,   294,    64,    77,   100,   114,   151,
     263,   616,   295,   613,    71,   618,   143,   194,   563,   632,
     475,   296,   484,   579,   574,   587,   370,    12,   383,   382,
     666,   692,    43,   284,   425,   426,   476,   478,   658,   658,
     239,   410,   477,    80,   180,    44,   180,    44,    80,    10,
     488,   511,   738,   761,   699,   294,   296,   160,   525,   296,
     294,   296,   478,   294,   299,   297,   738,   299,   296,   296,
     702,   296,    10,   483,   426,   370,   355,   356,   389,   487,
     192,   479,   350,   392,   658,   659,   702,   335,    52,   329,
     702,   230,   262,    52,   296,   296,   666,   666,   666,   666,
       8,   295,   295,   580,   582,   690,    63,    46,    94,   295,
     295,    54,   295,   295,   686,   686,   684,   686,   192,   686,
     295,   295,   295,   295,   686,   295,   295,   295,   295,   295,
     295,   624,   626,   627,   628,   755,   296,   296,   397,   398,
     296,   296,   686,   296,   296,   296,   296,   296,   296,   686,
     686,   730,    51,   173,   173,   730,   728,   686,   690,   686,
     686,   686,    63,   296,   451,   663,   424,   395,   429,   430,
     433,   742,   761,    55,    56,    58,    74,    80,   137,   170,
     227,   247,   256,   257,   258,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     450,   453,   454,   455,   461,   462,   463,   464,   466,   575,
     639,   640,   643,   649,   650,   719,   750,   761,   757,   752,
     756,   757,   759,   753,   192,   311,   294,   294,   311,   653,
     654,   694,   699,   653,   656,   694,   703,   296,   317,   317,
     296,   296,   599,   127,   617,   617,   616,   617,    85,    85,
     690,   596,    19,    73,   621,   638,   426,   487,   479,   231,
     241,   636,   705,   130,   705,   296,   476,   476,   478,   435,
     686,   400,   401,   402,   405,   145,   410,   635,   125,   125,
     125,   125,   125,   125,   476,   477,   296,   699,   703,   704,
     118,   361,   362,   363,   364,   365,   366,   518,   704,   525,
     296,   475,   484,   435,   705,   488,    22,   477,   295,   296,
     698,    19,   204,   336,   296,   698,   686,   580,   580,   296,
     632,   296,   686,    63,   580,   580,   686,   580,   580,     8,
      54,   686,   580,   580,   580,   580,   580,   580,   580,   580,
     580,   580,   296,   294,    79,   114,   126,   629,   628,   296,
     514,   296,   296,    61,   716,   730,   730,    51,   173,   296,
     296,   294,   296,   296,   686,   515,   293,    36,   744,   246,
     295,   686,   750,   761,   742,   742,   465,    51,   189,   434,
     457,   458,   293,    61,   191,    82,   297,    70,   319,   321,
     319,   321,   174,   278,   646,   596,    85,   600,   599,   296,
     754,   619,   620,   686,   666,   623,   435,   488,   477,   588,
     384,   477,   399,   399,   477,   477,    23,   394,   402,   139,
     145,   179,   365,   366,   403,   615,    79,   180,    80,   180,
      44,    80,    44,   426,   296,   296,    94,   394,   363,   426,
     487,   477,   384,   477,   124,   476,   111,    97,   230,   525,
     111,   296,   296,   638,   686,   296,   296,   296,   296,   686,
     686,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   626,   295,   748,   686,   296,   730,   686,   296,    43,
     284,   432,    61,   686,   686,   666,   449,   742,     9,    55,
      66,   165,   459,   460,   458,    56,   575,   295,   452,   124,
     653,   690,   602,   754,   600,   122,   239,   609,   610,   611,
     294,   477,   477,   296,   739,    86,   615,   410,   657,   748,
     435,   435,   488,   686,   698,   204,   698,   630,   748,    79,
     631,   296,   399,   399,   295,    82,   296,    82,   744,   746,
     761,   699,    47,   294,   246,    82,   666,    82,   295,   603,
     666,   295,   620,   410,   657,   122,   411,   412,   413,   477,
     477,   477,   296,   294,   295,   575,   452,   173,   452,   434,
     460,   686,   452,   296,   452,   604,   749,   659,   411,    44,
     180,   410,   122,   413,   122,   412,   296,   630,   630,   296,
     434,    82,    10,   456,    47,   294,   296,   296,   410,   117,
     156,   203,   414,   414,   296,    50,   452,    36,    47,   434,
     749,   201,    43,    94,   434,    47,   742,   434,   434
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
#define YYERROR		goto yyerrorlab


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
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
   ((Current).first_line   = (Rhs)[1].first_line,	\
    (Current).first_column = (Rhs)[1].first_column,	\
    (Current).last_line    = (Rhs)[N].last_line,	\
    (Current).last_column  = (Rhs)[N].last_column)
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
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
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
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
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

#if defined (YYMAXDEPTH) && YYMAXDEPTH == 0
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
  short int DSQL_yyssa[YYINITDEPTH];
  short int *DSQL_yyss = DSQL_yyssa;
  register short int *DSQL_DSQL_yyssp;

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
	short int *DSQL_yyss1 = DSQL_yyss;


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
	short int *DSQL_yyss1 = DSQL_yyss;
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

    { yyval = MAKE_const_slong (Jrd::FUN_descriptor); }
    break;

  case 90:

    { yyval = MAKE_const_slong (Jrd::FUN_scalar_array); }
    break;

  case 91:

    { yyval = MAKE_const_slong (Jrd::FUN_ref_with_null); }
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
				NULL, MAKE_const_slong ((IPTR) yyvsp[0]));}
    break;

  case 96:

    { yyval = MAKE_const_slong (Jrd::FUN_reference); }
    break;

  case 97:

    { yyval = MAKE_const_slong (Jrd::FUN_value); }
    break;

  case 98:

    { yyval = MAKE_const_slong (Jrd::FUN_descriptor); }
    break;

  case 99:

    { yyval = MAKE_const_slong (-1 * Jrd::FUN_reference); }
    break;

  case 100:

    { yyval = MAKE_const_slong (-1 * Jrd::FUN_descriptor); }
    break;

  case 101:

    { yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 102:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;

  case 103:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[0]); }
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

    { yyval = yyvsp[0]; }
    break;

  case 127:

    { yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 128:

    { yyval = make_node (nod_redef_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 129:

    { yyval = make_node (nod_replace_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 130:

    { yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
    break;

  case 131:

    { yyval = make_node (nod_unique, 0, NULL); }
    break;

  case 132:

    { yyval = NULL; }
    break;

  case 133:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 135:

    { yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;

  case 136:

    { yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 137:

    { yyval = MAKE_const_slong (1); }
    break;

  case 138:

    { yyval = MAKE_const_slong (0); }
    break;

  case 139:

    { yyval = MAKE_const_slong (0); }
    break;

  case 140:

    { yyval = MAKE_const_slong (0); }
    break;

  case 141:

    { yyval = MAKE_const_slong (1); }
    break;

  case 142:

    { yyval = (dsql_nod*) 0;}
    break;

  case 143:

    { yyval = yyvsp[-1]; }
    break;

  case 144:

    { yyval = NULL; }
    break;

  case 147:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 148:

    { yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 149:

    { yyval = NULL; }
    break;

  case 150:

    { yyval = NULL; }
    break;

  case 151:

    { yyval = yyvsp[0]; }
    break;

  case 153:

    { yyval = NULL; }
    break;

  case 155:

    { yyval = NULL; }
    break;

  case 157:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 158:

    { yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;

  case 161:

    { yyval = make_node (nod_null, (int) 0, NULL); }
    break;

  case 162:

    { yyval = make_node (nod_def_constraint, (int) e_cnstr_count,
					NULL, NULL, yyvsp[-2], NULL, yyvsp[0]); }
    break;

  case 163:

    { yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
    break;

  case 164:

    { yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
    break;

  case 165:

    { yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;

  case 168:

    { yyval = (dsql_nod*) yyvsp[0]; }
    break;

  case 169:

    {yyval = NULL;}
    break;

  case 172:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 173:

    { yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;

  case 174:

    { yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;

  case 175:

    { yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;

  case 176:

    { yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;

  case 177:

    { yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;

  case 178:

    {yyval = NULL;}
    break;

  case 181:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 183:

    { yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;

  case 184:

    { yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;

  case 185:

    { lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;

  case 186:

    { lex.g_file  = make_file();}
    break;

  case 191:

    { lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;

  case 192:

    { lex.g_file->fil_length = (IPTR) yyvsp[-1];}
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

    { yyval = NULL; }
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

    { yyval = NULL; }
    break;

  case 218:

    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 219:

    { lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 220:

    { yyval = yyvsp[-1]; }
    break;

  case 221:

    { lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
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

  case 235:

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
				yyvsp[-4], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
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

    { yyval = NULL; }
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

    { yyval = NULL; }
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

  case 316:

    { yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;

  case 317:

    { yyval = make_node (nod_exit, 0, NULL); }
    break;

  case 322:

    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
    break;

  case 323:

    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 324:

    { yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;

  case 325:

    { yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
    break;

  case 326:

    { yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;

  case 327:

    { yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;

  case 328:

    { yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], NULL, make_list (yyvsp[0]), NULL); }
    break;

  case 329:

    { yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;

  case 330:

    { yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;

  case 331:

    { yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 332:

    { yyval = NULL; }
    break;

  case 333:

    { yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
    break;

  case 334:

    { yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
    break;

  case 337:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 338:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 339:

    { yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
    break;

  case 340:

    { yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
    break;

  case 341:

    { yyval = NULL; }
    break;

  case 342:

    { yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;

  case 343:

    { yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
    break;

  case 344:

    { yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
    break;

  case 345:

    { yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 346:

    { yyval = NULL; }
    break;

  case 348:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 349:

    { yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
    break;

  case 351:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 352:

    { yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
    break;

  case 353:

    { yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
    break;

  case 354:

    { yyval = make_node (nod_exception, 1, yyvsp[0]); }
    break;

  case 355:

    { yyval = make_node (nod_default, 1, NULL); }
    break;

  case 359:

    { yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;

  case 360:

    { yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
    break;

  case 361:

    { yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
    break;

  case 362:

    { yyval = NULL; }
    break;

  case 363:

    { yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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

    { yyval = make_list (yyvsp[0]); }
    break;

  case 368:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 369:

    { yyval = NULL; }
    break;

  case 370:

    { yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;

  case 371:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 372:

    { yyval = NULL; }
    break;

  case 374:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 375:

    { yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;

  case 376:

    { yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 377:

    { yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 378:

    { lex.beginning = lex_position(); }
    break;

  case 379:

    { lex.beginning = lex.last_token; }
    break;

  case 380:

    { yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
    break;

  case 381:

    { yyval = (dsql_nod*) MAKE_string(lex.beginning, 
					(DSQL_yychar <= 0 ? lex_position() : lex.last_token) - lex.beginning); 
			}
    break;

  case 382:

    { yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;

  case 383:

    { yyval = 0; }
    break;

  case 384:

    { yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 385:

    { yyval = make_node (nod_redef_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 386:

    { yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-7], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 387:

    { yyval = MAKE_const_slong (0); }
    break;

  case 388:

    { yyval = MAKE_const_slong (1); }
    break;

  case 389:

    { yyval = NULL; }
    break;

  case 390:

    { yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
    break;

  case 391:

    { yyval = MAKE_const_slong (0); }
    break;

  case 392:

    { yyval = MAKE_const_slong (1); }
    break;

  case 393:

    { yyval = MAKE_const_slong (trigger_type_suffix (1, 0, 0)); }
    break;

  case 394:

    { yyval = MAKE_const_slong (trigger_type_suffix (2, 0, 0)); }
    break;

  case 395:

    { yyval = MAKE_const_slong (trigger_type_suffix (3, 0, 0)); }
    break;

  case 396:

    { yyval = MAKE_const_slong (trigger_type_suffix (1, 2, 0)); }
    break;

  case 397:

    { yyval = MAKE_const_slong (trigger_type_suffix (1, 3, 0)); }
    break;

  case 398:

    { yyval = MAKE_const_slong (trigger_type_suffix (2, 1, 0)); }
    break;

  case 399:

    { yyval = MAKE_const_slong (trigger_type_suffix (2, 3, 0)); }
    break;

  case 400:

    { yyval = MAKE_const_slong (trigger_type_suffix (3, 1, 0)); }
    break;

  case 401:

    { yyval = MAKE_const_slong (trigger_type_suffix (3, 2, 0)); }
    break;

  case 402:

    { yyval = MAKE_const_slong (trigger_type_suffix (1, 2, 3)); }
    break;

  case 403:

    { yyval = MAKE_const_slong (trigger_type_suffix (1, 3, 2)); }
    break;

  case 404:

    { yyval = MAKE_const_slong (trigger_type_suffix (2, 1, 3)); }
    break;

  case 405:

    { yyval = MAKE_const_slong (trigger_type_suffix (2, 3, 1)); }
    break;

  case 406:

    { yyval = MAKE_const_slong (trigger_type_suffix (3, 1, 2)); }
    break;

  case 407:

    { yyval = MAKE_const_slong (trigger_type_suffix (3, 2, 1)); }
    break;

  case 408:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[0]); }
    break;

  case 409:

    { yyval = NULL; }
    break;

  case 410:

    { yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 411:

    { yyval = yyvsp[0]; }
    break;

  case 412:

    { yyval = yyvsp[0]; }
    break;

  case 413:

    { yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 414:

    { yyval = yyvsp[0]; }
    break;

  case 415:

    { yyval = yyvsp[0]; }
    break;

  case 416:

    { yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
    break;

  case 417:

    { yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
    break;

  case 418:

    { yyval = make_node (nod_mod_index, (int) e_mod_idx_count, yyvsp[0]); }
    break;

  case 419:

    { yyval = yyvsp[0]; }
    break;

  case 420:

    { yyval = yyvsp[0]; }
    break;

  case 422:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 423:

    { yyval = make_node (nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 424:

    { yyval = yyvsp[0]; }
    break;

  case 425:

    { yyval = yyvsp[0]; }
    break;

  case 426:

    {yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;

  case 427:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;

  case 428:

    { yyval = yyvsp[0]; }
    break;

  case 429:

    { yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;

  case 431:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 432:

    { yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 433:

    { yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;

  case 434:

    { yyval = yyvsp[0]; }
    break;

  case 435:

    { yyval = yyvsp[0]; }
    break;

  case 436:

    { yyval = make_node(nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_const_slong((IPTR) yyvsp[0])); }
    break;

  case 437:

    { yyval = make_node(nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 438:

    { yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], yyvsp[0], NULL); }
    break;

  case 439:

    { yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-3], NULL,
					make_node(nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0])); }
    break;

  case 440:

    { yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], NULL,
					make_node(nod_del_default, (int) 0, NULL)); }
    break;

  case 441:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 483:

    { yyval = NULL; }
    break;

  case 484:

    { yyval = NULL; }
    break;

  case 485:

    { yyval = NULL; }
    break;

  case 486:

    { yyval = make_node (nod_def_domain, (int) e_dom_count,
					yyvsp[0], NULL, NULL, NULL, NULL); }
    break;

  case 487:

    { lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;

  case 488:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 489:

    { yyval = make_node (nod_cascade, 0, NULL); }
    break;

  case 490:

    { yyval = make_node (nod_restrict, 0, NULL); }
    break;

  case 491:

    { yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;

  case 492:

    { yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;

  case 493:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_const_slong ((IPTR) yyvsp[0])); }
    break;

  case 494:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;

  case 495:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;

  case 496:

    { yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 497:

    { yyval = yyvsp[0]; }
    break;

  case 498:

    { yyval = NULL; }
    break;

  case 499:

    { yyval = yyvsp[0]; }
    break;

  case 500:

    { yyval = NULL; }
    break;

  case 501:

    { yyval = NULL; }
    break;

  case 503:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 504:

    { yyval = yyvsp[0]; }
    break;

  case 505:

    { yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;

  case 506:

    { yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;

  case 507:

    { yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;

  case 508:

    { yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;

  case 509:

    { yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;

  case 511:

    { yyval = NULL; }
    break;

  case 513:

    { yyval = NULL; }
    break;

  case 514:

    { yyval = yyvsp[0]; }
    break;

  case 515:

    { yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;

  case 516:

    { yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;

  case 517:

    { yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;

  case 518:

    { yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;

  case 519:

    { yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;

  case 520:

    { yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;

  case 521:

    { yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;

  case 522:

    { yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;

  case 523:

    { yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;

  case 524:

    { yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;

  case 525:

    { yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;

  case 526:

    { yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;

  case 527:

    { yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;

  case 532:

    { lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;

  case 533:

    { lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;

  case 535:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 536:

    { if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_const_slong ((IPTR) yyvsp[0]),
					MAKE_const_slong (1));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_const_slong (1),
					MAKE_const_slong ((IPTR) yyvsp[0]) ); }
    break;

  case 537:

    { yyval = make_node (nod_list, (int) 2, 
			 	MAKE_const_slong ((IPTR) yyvsp[-2]),
				MAKE_const_slong ((IPTR) yyvsp[0])); }
    break;

  case 543:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "BIGINT",
					isc_arg_end);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "BIGINT",
					isc_arg_end);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
    break;

  case 544:

    { 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;

  case 545:

    { 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;

  case 546:

    { 
			*stmt_ambiguous = true;
			if (client_dialect <= SQL_DIALECT_V5)
				{
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
					ERRD_post_warning (isc_sqlwarn, isc_arg_number, (SLONG) 301, 
											   isc_arg_warning, isc_dtype_renamed, isc_arg_end);
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

  case 547:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					isc_arg_end);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					isc_arg_end);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
    break;

  case 548:

    { 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;

  case 551:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;

  case 552:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;

  case 553:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 554:

    { 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;

  case 555:

    {
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;

  case 556:

    {
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;

  case 557:

    {
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;

  case 558:

    {
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;

  case 559:

    {
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;

  case 560:

    {
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;

  case 562:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 563:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 564:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;

  case 565:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 566:

    { 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;

  case 567:

    { 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;

  case 576:

    { 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;

  case 577:

    {  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;

  case 578:

    {
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;

  case 579:

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
					   isc_arg_end);
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

  case 580:

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
					   isc_arg_end);
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

  case 583:

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

  case 584:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 585:

    { 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;

  case 586:

    { 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;

  case 587:

    { yyval = yyvsp[-1]; }
    break;

  case 588:

    { yyval = 0; }
    break;

  case 592:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_const_slong ((IPTR) yyvsp[0])); }
    break;

  case 593:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;

  case 594:

    { yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;

  case 598:

    { yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;

  case 599:

    { yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 600:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 601:

    { yyval = 0; }
    break;

  case 602:

    { yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;

  case 605:

    { yyval = make_node (nod_commit, e_commit_count, yyvsp[0]); }
    break;

  case 606:

    { yyval = make_node (nod_rollback, e_rollback_count, yyvsp[0]); }
    break;

  case 609:

    { yyval = make_node (nod_retain, 0, NULL); }
    break;

  case 610:

    { yyval = NULL; }
    break;

  case 612:

    { yyval = NULL; }
    break;

  case 613:

    {yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;

  case 615:

    { yyval = NULL; }
    break;

  case 617:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 624:

    { yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;

  case 625:

    { yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;

  case 626:

    { yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;

  case 627:

    { yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;

  case 628:

    { yyval = yyvsp[0];}
    break;

  case 630:

    { yyval = yyvsp[0];}
    break;

  case 631:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 632:

    { yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;

  case 633:

    { yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;

  case 634:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 635:

    { yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;

  case 636:

    { yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;

  case 637:

    { yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;

  case 638:

    { yyval = 0; }
    break;

  case 639:

    { yyval = make_flag_node(nod_tra_misc, NOD_NO_AUTO_UNDO, 0, NULL); }
    break;

  case 640:

    { yyval = make_flag_node(nod_tra_misc, NOD_IGNORE_LIMBO, 0, NULL); }
    break;

  case 641:

    { yyval = make_flag_node(nod_tra_misc, NOD_RESTART_REQUESTS, 0, NULL); }
    break;

  case 642:

    { yyval = make_node(nod_lock_timeout, 1, MAKE_const_slong ((IPTR) yyvsp[0])); }
    break;

  case 643:

    { yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;

  case 644:

    { yyval = (dsql_nod*) NOD_SHARED; }
    break;

  case 645:

    { yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;

  case 646:

    { yyval = (dsql_nod*) 0; }
    break;

  case 647:

    { yyval = (dsql_nod*) NOD_READ; }
    break;

  case 648:

    { yyval = (dsql_nod*) NOD_WRITE; }
    break;

  case 650:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 651:

    { yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 652:

    { yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;

  case 653:

    { yyval = 0; }
    break;

  case 655:

    { yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 656:

    { yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;

  case 657:

    { yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
    break;

  case 658:

    { yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
    break;

  case 659:

    { yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
    break;

  case 660:

    { yyval = MAKE_const_slong(ddl_database); }
    break;

  case 661:

    { yyval = MAKE_const_slong(ddl_domain); }
    break;

  case 662:

    { yyval = MAKE_const_slong(ddl_relation); }
    break;

  case 663:

    { yyval = MAKE_const_slong(ddl_view); }
    break;

  case 664:

    { yyval = MAKE_const_slong(ddl_procedure); }
    break;

  case 665:

    { yyval = MAKE_const_slong(ddl_trigger); }
    break;

  case 666:

    { yyval = MAKE_const_slong(ddl_udf); }
    break;

  case 667:

    { yyval = MAKE_const_slong(ddl_blob_filter); }
    break;

  case 668:

    { yyval = MAKE_const_slong(ddl_exception); }
    break;

  case 669:

    { yyval = MAKE_const_slong(ddl_generator); }
    break;

  case 670:

    { yyval = MAKE_const_slong(ddl_generator); }
    break;

  case 671:

    { yyval = MAKE_const_slong(ddl_index); }
    break;

  case 672:

    { yyval = MAKE_const_slong(ddl_role); }
    break;

  case 673:

    { yyval = MAKE_const_slong(ddl_charset); }
    break;

  case 674:

    { yyval = MAKE_const_slong(ddl_collation); }
    break;

  case 675:

    { yyval = MAKE_const_slong(ddl_relation); }
    break;

  case 676:

    { yyval = MAKE_const_slong(ddl_procedure); }
    break;

  case 677:

    { yyval = yyvsp[0]; }
    break;

  case 679:

    { yyval = NULL; }
    break;

  case 680:

    { yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 681:

    { yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;

  case 682:

    { yyval = NULL; }
    break;

  case 683:

    { yyval = yyvsp[0]; }
    break;

  case 684:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 685:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 686:

    { yyval = NULL; }
    break;

  case 687:

    { yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 688:

    { yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 689:

    { yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 691:

    { yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 692:

    { yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 694:

    { yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 695:

    { lex.limit_clause = true; }
    break;

  case 696:

    { lex.limit_clause = false; }
    break;

  case 697:

    { lex.first_detection = true; }
    break;

  case 698:

    { lex.first_detection = false; }
    break;

  case 699:

    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;

  case 700:

    { yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;

  case 701:

    { yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;

  case 702:

    { yyval = 0; }
    break;

  case 703:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[-1]); }
    break;

  case 704:

    { yyval = yyvsp[-2]; }
    break;

  case 705:

    { yyval = yyvsp[-1]; }
    break;

  case 706:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[0]); }
    break;

  case 707:

    { yyval = yyvsp[-1]; }
    break;

  case 708:

    { yyval = yyvsp[0]; }
    break;

  case 709:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 710:

    { yyval = 0; }
    break;

  case 711:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 712:

    { yyval = 0; }
    break;

  case 714:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 716:

    { yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 719:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 721:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 726:

    { yyval = yyvsp[-1]; }
    break;

  case 727:

    { yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;

  case 729:

    { yyval = NULL; }
    break;

  case 730:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 731:

    { yyval = NULL; }
    break;

  case 733:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 737:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;

  case 738:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;

  case 739:

    { yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;

  case 742:

    { yyval = yyvsp[0]; }
    break;

  case 743:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 744:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;

  case 745:

    { yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;

  case 746:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 747:

    { yyval = NULL; }
    break;

  case 749:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;

  case 750:

    { yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;

  case 751:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 752:

    { yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;

  case 753:

    { yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;

  case 754:

    { yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;

  case 755:

    { yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;

  case 758:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 759:

    { yyval = NULL; }
    break;

  case 761:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 763:

    { yyval = yyvsp[0]; }
    break;

  case 764:

    { yyval = NULL; }
    break;

  case 765:

    { yyval = yyvsp[0]; }
    break;

  case 766:

    { yyval = NULL; }
    break;

  case 767:

    { yyval = yyvsp[0]; }
    break;

  case 768:

    { yyval = NULL; }
    break;

  case 769:

    { yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 770:

    { yyval = 0; }
    break;

  case 771:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 772:

    { yyval = make_node (nod_merge, (int) 0, NULL); }
    break;

  case 773:

    { yyval = 0; }
    break;

  case 774:

    { yyval = 0; }
    break;

  case 776:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 777:

    { yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;

  case 780:

    { yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 781:

    { yyval = make_node (nod_natural, (int) 0, NULL); }
    break;

  case 782:

    { yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;

  case 783:

    { yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;

  case 785:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 786:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 787:

    { yyval = 0; }
    break;

  case 788:

    { yyval = make_list (yyvsp[0]); }
    break;

  case 789:

    { yyval = 0; }
    break;

  case 791:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 792:

    { yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 793:

    { yyval = 0; }
    break;

  case 794:

    { yyval = make_node (nod_flag, 0, NULL); }
    break;

  case 795:

    { yyval = 0; }
    break;

  case 796:

    { yyval = MAKE_const_slong(NOD_NULLS_FIRST); }
    break;

  case 797:

    { yyval = MAKE_const_slong(NOD_NULLS_LAST); }
    break;

  case 798:

    { yyval = yyvsp[-1]; }
    break;

  case 799:

    { yyval = 0; }
    break;

  case 800:

    { yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;

  case 801:

    { yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_const_slong (1)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_const_slong (1))); }
    break;

  case 802:

    { yyval = NULL; }
    break;

  case 803:

    { yyval = make_node (nod_insert, (int) e_ins_count, 
				yyvsp[-6], make_list (yyvsp[-5]), make_list (yyvsp[-2]), NULL, yyvsp[0]); }
    break;

  case 804:

    { yyval = make_node (nod_insert, (int) e_ins_count,
				yyvsp[-2], yyvsp[-1], NULL, yyvsp[0], NULL); }
    break;

  case 807:

    { yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 808:

    { yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 811:

    { yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;

  case 812:

    { yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;

  case 813:

    { yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[0]), NULL); }
    break;

  case 814:

    { yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[-2]), make_list (yyvsp[0])); }
    break;

  case 815:

    { yyval = NULL; }
    break;

  case 816:

    { yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;

  case 818:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 819:

    { yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;

  case 820:

    { yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;

  case 821:

    { yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 822:

    { yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;

  case 823:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 824:

    { yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;

  case 825:

    { yyval = NULL; }
    break;

  case 828:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[0]); }
    break;

  case 829:

    { yyval = yyvsp[0]; }
    break;

  case 830:

    { yyval = NULL; }
    break;

  case 831:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[0]); }
    break;

  case 834:

    { yyval = NULL; }
    break;

  case 835:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 837:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 839:

    { yyval = NULL; }
    break;

  case 840:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 842:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 844:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 845:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;

  case 846:

    { yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;

  case 848:

    { yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;

  case 850:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 852:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 853:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 855:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 856:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 857:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 858:

    { yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 859:

    { yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 861:

    { yyval = yyvsp[-1]; }
    break;

  case 862:

    { yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;

  case 874:

    { yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 875:

    { yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 876:

    { yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 877:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 878:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 879:

    { yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 880:

    { yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 881:

    { yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 882:

    { yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 883:

    { yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 884:

    { yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 885:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 886:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 887:

    { yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 888:

    { yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 889:

    { yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 890:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 891:

    { yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 892:

    { yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 893:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 894:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 895:

    { yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 896:

    { yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 897:

    { yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;

  case 900:

    { yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 901:

    { yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;

  case 902:

    { yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 903:

    { yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 904:

    { yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 905:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 906:

    { yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;

  case 907:

    { yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;

  case 908:

    { yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 909:

    { yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 910:

    { yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 911:

    { yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 912:

    { yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 913:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;

  case 914:

    { yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;

  case 915:

    { yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;

  case 916:

    { yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;

  case 917:

    { yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;

  case 918:

    { yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;

  case 919:

    { yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;

  case 920:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (1)); }
    break;

  case 921:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (2)); }
    break;

  case 922:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (3)); }
    break;

  case 923:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (1)); }
    break;

  case 924:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (2)); }
    break;

  case 925:

    { yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_trigger_action)),
						MAKE_const_slong (3)); }
    break;

  case 927:

    { yyval = make_list (yyvsp[-1]); }
    break;

  case 928:

    { yyval = yyvsp[-1]; }
    break;

  case 939:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 940:

    { yyval = yyvsp[0]; }
    break;

  case 941:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 942:

    { yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 943:

    { yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;

  case 944:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 945:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 946:

    {
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;

  case 947:

    { yyval = yyvsp[-1]; }
    break;

  case 948:

    { yyval = yyvsp[-1]; }
    break;

  case 952:

    { yyval = make_node (nod_dbkey, 1, NULL); }
    break;

  case 953:

    { yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;

  case 954:

    { 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;

  case 957:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					isc_arg_end);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					isc_arg_end);
			yyval = make_node (nod_current_date, 0, NULL);
			}
    break;

  case 958:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					isc_arg_end);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					isc_arg_end);
			yyval = make_node (nod_current_time, 1, yyvsp[0]);
			}
    break;

  case 959:

    { yyval = make_node (nod_current_timestamp, 1, yyvsp[0]); }
    break;

  case 960:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[-1]); }
    break;

  case 961:

    { yyval = NULL; }
    break;

  case 962:

    { yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;

  case 964:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 966:

    { yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;

  case 967:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;

  case 968:

    { yyval = MAKE_const_slong ((IPTR) yyvsp[0]); }
    break;

  case 969:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;

  case 970:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 971:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;

  case 973:

    { yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;

  case 974:

    { 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					isc_arg_end);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					isc_arg_end);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
			}
    break;

  case 975:

    {
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					isc_arg_end);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					isc_arg_end);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
    break;

  case 976:

    { yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;

  case 977:

    { yyval = make_parameter (); }
    break;

  case 978:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 979:

    { yyval = make_node (nod_user_name, 0, NULL); }
    break;

  case 980:

    { yyval = make_node (nod_current_role, 0, NULL); }
    break;

  case 981:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_connection_id)); }
    break;

  case 982:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_transaction_id)); }
    break;

  case 983:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_gdscode)); }
    break;

  case 984:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_sqlcode)); }
    break;

  case 985:

    { yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_const_slong (internal_rows_affected)); }
    break;

  case 986:

    { yyval = yyvsp[0]; }
    break;

  case 987:

    { ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;

  case 989:

    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 990:

    { if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 991:

    { if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 992:

    { if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;

  case 993:

    { if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;

  case 995:

    { yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;

  case 996:

    { yyval = yyvsp[0];}
    break;

  case 1000:

    { yyval = make_node (nod_agg_count, 0, NULL); }
    break;

  case 1001:

    { yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;

  case 1002:

    { yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;

  case 1003:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;

  case 1004:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 1005:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;

  case 1006:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;

  case 1007:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 1008:

    { yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;

  case 1009:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 1010:

    { yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;

  case 1013:

    { yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;

  case 1017:

    { yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_bit), yyvsp[-1]); }
    break;

  case 1018:

    { yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_char), yyvsp[-1]); }
    break;

  case 1019:

    { yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_char), yyvsp[-1]); }
    break;

  case 1020:

    { yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_const_slong(blr_strlen_octet), yyvsp[-1]); }
    break;

  case 1023:

    { yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;

  case 1024:

    { yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
    break;

  case 1025:

    { yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_const_slong (1)), yyvsp[-1]); }
    break;

  case 1026:

    { yyval = yyvsp[0]; }
    break;

  case 1027:

    { yyval = MAKE_const_slong (SHRT_POS_MAX); }
    break;

  case 1028:

    { yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
    break;

  case 1029:

    { yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_const_slong (blr_trim_both), yyvsp[-3], yyvsp[-1]); }
    break;

  case 1030:

    { yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
    break;

  case 1031:

    { yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_const_slong (blr_trim_both), NULL, yyvsp[-1]); }
    break;

  case 1032:

    { yyval = MAKE_const_slong (blr_trim_both); }
    break;

  case 1033:

    { yyval = MAKE_const_slong (blr_trim_trailing); }
    break;

  case 1034:

    { yyval = MAKE_const_slong (blr_trim_leading); }
    break;

  case 1035:

    { yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 1036:

    { yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;

  case 1037:

    { yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;

  case 1040:

    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;

  case 1041:

    { yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;

  case 1042:

    { yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;

  case 1045:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 1046:

    { yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 1047:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 1048:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 1049:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;

  case 1050:

    { yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;

  case 1051:

    { yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;

  case 1052:

    { yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;

  case 1056:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[0],
						MAKE_const_slong(1));
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[0],
						MAKE_const_slong(1));
			}
    break;

  case 1057:

    { 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;

  case 1058:

    { yyval = MAKE_const_slong (blr_extract_year); }
    break;

  case 1059:

    { yyval = MAKE_const_slong (blr_extract_month); }
    break;

  case 1060:

    { yyval = MAKE_const_slong (blr_extract_day); }
    break;

  case 1061:

    { yyval = MAKE_const_slong (blr_extract_hour); }
    break;

  case 1062:

    { yyval = MAKE_const_slong (blr_extract_minute); }
    break;

  case 1063:

    { yyval = MAKE_const_slong (blr_extract_second); }
    break;

  case 1064:

    { yyval = MAKE_const_slong (blr_extract_weekday); }
    break;

  case 1065:

    { yyval = MAKE_const_slong (blr_extract_yearday); }
    break;

  case 1070:

    { yyval = make_node (nod_null, 0, NULL); }
    break;


    }

/* Line 1010 of yacc.c.  */


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
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
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

      if (DSQL_yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (DSQL_yychar == YYEOF)
	     for (;;)
	       {
		 YYPOPSTACK;
		 if (DSQL_DSQL_yyssp == DSQL_yyss)
		   YYABORT;
		 YYDSYMPRINTF ("Error: popping", yystos[*DSQL_DSQL_yyssp], yyvsp, yylsp);
		 yydestruct (yystos[*DSQL_DSQL_yyssp], yyvsp);
	       }
        }
      else
	{
	  YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
	  yydestruct (yytoken, &yylval);
	  DSQL_yychar = YYEMPTY;

	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

  yyvsp -= yylen;
  DSQL_DSQL_yyssp -= yylen;
  yystate = *DSQL_DSQL_yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
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
      YYPOPSTACK;
      yystate = *DSQL_DSQL_yyssp;
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

		if ((c == '-') && (*ptr == '-'))
		{
			
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
		else if ((c == '/') && (*ptr == '*'))
		{
			
			/* multi-line */
			
			const TEXT& start_block = ptr[-1];
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
			{
				// I need this to report the correct beginning of the block,
				// since it's not a token really.
				last_token = &start_block;
				yyerror("unterminated block comment");
				return -1;
			}
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
				yyerror("unterminated string");
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
		FB_UINT64	number		 = 0;
		FB_UINT64	limit_by_10	= MAX_SINT64 / 10;

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
	const TEXT* line_start = lex.line_start;
	SLONG lines = lex.lines;
	if (lex.last_token < lex.line_start)
	{
		line_start = lex.line_start_bk;
		lines--;
	}

	if (DSQL_yychar < 1)
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			isc_arg_gds, isc_command_end_err2,	/* Unexpected end of command */
			isc_arg_number, lines,
			isc_arg_number, (SLONG) (lex.last_token - line_start + 1),
			isc_arg_end);
	else
	{
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			/* Token unknown - line %d, column %d */
			isc_arg_gds, isc_dsql_token_unk_err,
			isc_arg_number, (SLONG) lines,
			isc_arg_number, (SLONG) (lex.last_token - line_start + 1), /*CVC: +1*/
			/* Show the token */
			isc_arg_gds, isc_random,
			isc_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token,
			isc_arg_end);
	}
}


static void yyabandon (SLONG		sql_code,
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

	ERRD_post (isc_sqlerr, isc_arg_number, sql_code,
		isc_arg_gds, error_symbol, isc_arg_end);
}

