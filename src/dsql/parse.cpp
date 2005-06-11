/* A Bison parser, made from F:\fb2dev\fbbuild\firebird2\src\dsql\parse.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	ACTIVE	257
# define	ADD	258
# define	AFTER	259
# define	ALL	260
# define	ALTER	261
# define	AND	262
# define	ANY	263
# define	AS	264
# define	ASC	265
# define	AT	266
# define	AVG	267
# define	AUTO	268
# define	BEFORE	269
# define	BEGIN	270
# define	BETWEEN	271
# define	BLOB	272
# define	BY	273
# define	CAST	274
# define	CHARACTER	275
# define	CHECK	276
# define	COLLATE	277
# define	COMMA	278
# define	COMMIT	279
# define	COMMITTED	280
# define	COMPUTED	281
# define	CONCATENATE	282
# define	CONDITIONAL	283
# define	CONSTRAINT	284
# define	CONTAINING	285
# define	COUNT	286
# define	CREATE	287
# define	CSTRING	288
# define	CURRENT	289
# define	CURSOR	290
# define	DATABASE	291
# define	DATE	292
# define	DB_KEY	293
# define	KW_DEBUG	294
# define	DECIMAL	295
# define	DECLARE	296
# define	DEFAULT	297
# define	KW_DELETE	298
# define	DESC	299
# define	DISTINCT	300
# define	DO	301
# define	DOMAIN	302
# define	DROP	303
# define	ELSE	304
# define	END	305
# define	ENTRY_POINT	306
# define	EQL	307
# define	ESCAPE	308
# define	EXCEPTION	309
# define	EXECUTE	310
# define	EXISTS	311
# define	EXIT	312
# define	EXTERNAL	313
# define	FILTER	314
# define	FOR	315
# define	FOREIGN	316
# define	FROM	317
# define	FULL	318
# define	FUNCTION	319
# define	GDSCODE	320
# define	GEQ	321
# define	GENERATOR	322
# define	GEN_ID	323
# define	GRANT	324
# define	GROUP	325
# define	GTR	326
# define	HAVING	327
# define	IF	328
# define	KW_IN	329
# define	INACTIVE	330
# define	INNER	331
# define	INPUT_TYPE	332
# define	INDEX	333
# define	INSERT	334
# define	INTEGER	335
# define	INTO	336
# define	IS	337
# define	ISOLATION	338
# define	JOIN	339
# define	KEY	340
# define	KW_CHAR	341
# define	KW_DEC	342
# define	KW_DOUBLE	343
# define	KW_FILE	344
# define	KW_FLOAT	345
# define	KW_INT	346
# define	KW_LONG	347
# define	KW_NULL	348
# define	KW_NUMERIC	349
# define	KW_UPPER	350
# define	KW_VALUE	351
# define	LENGTH	352
# define	LPAREN	353
# define	LEFT	354
# define	LEQ	355
# define	LEVEL	356
# define	LIKE	357
# define	LSS	358
# define	MANUAL	359
# define	MAXIMUM	360
# define	MAX_SEGMENT	361
# define	MERGE	362
# define	MESSAGE	363
# define	MINIMUM	364
# define	MODULE_NAME	365
# define	NAMES	366
# define	NATIONAL	367
# define	NATURAL	368
# define	NCHAR	369
# define	NEQ	370
# define	NO	371
# define	NOT	372
# define	NOT_GTR	373
# define	NOT_LSS	374
# define	OF	375
# define	ON	376
# define	ONLY	377
# define	OPTION	378
# define	OR	379
# define	ORDER	380
# define	OUTER	381
# define	OUTPUT_TYPE	382
# define	OVERFLOW	383
# define	PAGE	384
# define	PAGES	385
# define	KW_PAGE_SIZE	386
# define	PARAMETER	387
# define	PASSWORD	388
# define	PLAN	389
# define	POSITION	390
# define	POST_EVENT	391
# define	PRECISION	392
# define	PRIMARY	393
# define	PRIVILEGES	394
# define	PROCEDURE	395
# define	PROTECTED	396
# define	READ	397
# define	REAL	398
# define	REFERENCES	399
# define	RESERVING	400
# define	RETAIN	401
# define	RETURNING_VALUES	402
# define	RETURNS	403
# define	REVOKE	404
# define	RIGHT	405
# define	RPAREN	406
# define	ROLLBACK	407
# define	SEGMENT	408
# define	SELECT	409
# define	SET	410
# define	SHADOW	411
# define	KW_SHARED	412
# define	SINGULAR	413
# define	KW_SIZE	414
# define	SMALLINT	415
# define	SNAPSHOT	416
# define	SOME	417
# define	SORT	418
# define	SQLCODE	419
# define	STABILITY	420
# define	STARTING	421
# define	STATISTICS	422
# define	SUB_TYPE	423
# define	SUSPEND	424
# define	SUM	425
# define	TABLE	426
# define	THEN	427
# define	TO	428
# define	TRANSACTION	429
# define	TRIGGER	430
# define	UNCOMMITTED	431
# define	UNION	432
# define	UNIQUE	433
# define	UPDATE	434
# define	USER	435
# define	VALUES	436
# define	VARCHAR	437
# define	VARIABLE	438
# define	VARYING	439
# define	VERSION	440
# define	VIEW	441
# define	WAIT	442
# define	WHEN	443
# define	WHERE	444
# define	WHILE	445
# define	WITH	446
# define	WORK	447
# define	WRITE	448
# define	FLOAT_NUMBER	449
# define	NUMBER	450
# define	NUMERIC	451
# define	SYMBOL	452
# define	STRING	453
# define	INTRODUCER	454
# define	ACTION	455
# define	ADMIN	456
# define	CASCADE	457
# define	FREE_IT	458
# define	RESTRICT	459
# define	ROLE	460
# define	COLUMN	461
# define	TYPE	462
# define	EXTRACT	463
# define	YEAR	464
# define	MONTH	465
# define	DAY	466
# define	HOUR	467
# define	MINUTE	468
# define	SECOND	469
# define	WEEKDAY	470
# define	YEARDAY	471
# define	TIME	472
# define	TIMESTAMP	473
# define	CURRENT_DATE	474
# define	CURRENT_TIME	475
# define	CURRENT_TIMESTAMP	476
# define	NUMBER64BIT	477
# define	SCALEDINT	478
# define	CURRENT_USER	479
# define	CURRENT_ROLE	480
# define	KW_BREAK	481
# define	SUBSTRING	482
# define	RECREATE	483
# define	KW_DESCRIPTOR	484
# define	FIRST	485
# define	SKIP	486
# define	CURRENT_CONNECTION	487
# define	CURRENT_TRANSACTION	488
# define	BIGINT	489
# define	CASE	490
# define	NULLIF	491
# define	COALESCE	492
# define	USING	493
# define	NULLS	494
# define	LAST	495
# define	ROW_COUNT	496
# define	LOCK	497
# define	SAVEPOINT	498
# define	RELEASE	499
# define	STATEMENT	500
# define	LEAVE	501
# define	INSERTING	502
# define	UPDATING	503
# define	DELETING	504
# define	KW_INSERTING	505
# define	KW_UPDATING	506
# define	KW_DELETING	507
# define	BACKUP	508
# define	KW_DIFFERENCE	509
# define	OPEN	510
# define	CLOSE	511
# define	FETCH	512
# define	ROWS	513
# define	BLOCK	514
# define	IIF	515
# define	SCALAR_ARRAY	516
# define	CROSS	517
# define	NEXT	518
# define	SEQUENCE	519
# define	RESTART	520
# define	BOTH	521
# define	COLLATION	522
# define	COMMENT	523
# define	BIT_LENGTH	524
# define	CHAR_LENGTH	525
# define	CHARACTER_LENGTH	526
# define	LEADING	527
# define	KW_LOWER	528
# define	OCTET_LENGTH	529
# define	TRAILING	530
# define	TRIM	531


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

#ifndef YYSTYPE
# define YYSTYPE int
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		2021
#define	YYFLAG		-32768
#define	YYNTBASE	294

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 531 ? yytranslate[x] : 750)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     2,     5,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    33,    35,    37,
      39,    41,    43,    45,    48,    57,    66,    72,    74,    75,
      77,    80,    82,    84,    88,    90,    92,    94,    96,    99,
     102,   106,   107,   111,   112,   114,   123,   132,   138,   142,
     143,   147,   148,   150,   152,   154,   158,   162,   166,   169,
     172,   175,   178,   180,   184,   186,   189,   192,   194,   198,
     200,   202,   206,   208,   211,   214,   217,   221,   230,   232,
     234,   240,   241,   243,   247,   249,   253,   257,   258,   261,
     264,   266,   268,   272,   276,   279,   280,   283,   286,   288,
     292,   302,   304,   306,   309,   312,   320,   323,   326,   329,
     332,   335,   338,   341,   344,   347,   350,   353,   356,   359,
     362,   365,   370,   373,   376,   379,   382,   385,   388,   391,
     393,   394,   396,   398,   405,   412,   414,   416,   417,   418,
     420,   421,   426,   427,   429,   431,   434,   443,   445,   446,
     450,   451,   452,   454,   456,   459,   461,   463,   465,   468,
     475,   477,   479,   483,   484,   486,   488,   489,   491,   493,
     496,   500,   505,   508,   511,   515,   516,   518,   520,   523,
     525,   530,   534,   538,   540,   541,   543,   545,   548,   552,
     557,   558,   560,   563,   564,   566,   568,   574,   580,   584,
     587,   588,   590,   594,   596,   598,   605,   609,   612,   619,
     622,   624,   627,   630,   633,   634,   636,   638,   641,   642,
     645,   646,   648,   650,   652,   654,   656,   658,   659,   661,
     663,   666,   669,   672,   678,   680,   683,   687,   690,   693,
     694,   696,   698,   700,   702,   706,   711,   720,   725,   726,
     733,   735,   737,   740,   743,   744,   748,   752,   754,   757,
     760,   763,   772,   781,   790,   799,   803,   804,   809,   810,
     812,   816,   821,   823,   827,   830,   834,   838,   840,   842,
     843,   845,   848,   853,   855,   857,   861,   863,   864,   867,
     869,   876,   878,   880,   884,   886,   889,   890,   892,   895,
     898,   901,   904,   907,   910,   913,   916,   919,   922,   925,
     928,   931,   934,   937,   940,   943,   945,   947,   949,   951,
     954,   958,   960,   964,   973,   983,   989,   998,  1005,  1009,
    1010,  1014,  1017,  1019,  1021,  1025,  1029,  1037,  1040,  1041,
    1043,  1045,  1048,  1052,  1053,  1055,  1058,  1063,  1065,  1069,
    1072,  1075,  1078,  1080,  1082,  1084,  1086,  1089,  1092,  1098,
    1099,  1105,  1107,  1111,  1112,  1115,  1120,  1121,  1129,  1133,
    1134,  1136,  1140,  1144,  1152,  1160,  1161,  1162,  1163,  1164,
    1168,  1169,  1179,  1189,  1191,  1193,  1194,  1197,  1199,  1201,
    1203,  1205,  1207,  1211,  1215,  1219,  1223,  1227,  1231,  1237,
    1243,  1249,  1255,  1261,  1267,  1270,  1271,  1276,  1279,  1282,
    1286,  1289,  1292,  1296,  1300,  1303,  1306,  1310,  1314,  1321,
    1323,  1326,  1331,  1335,  1338,  1341,  1344,  1347,  1351,  1353,
    1357,  1361,  1365,  1368,  1371,  1376,  1381,  1387,  1389,  1391,
    1393,  1395,  1397,  1399,  1401,  1403,  1405,  1407,  1409,  1411,
    1413,  1415,  1417,  1419,  1421,  1423,  1425,  1427,  1429,  1431,
    1433,  1435,  1437,  1439,  1441,  1443,  1445,  1447,  1449,  1451,
    1453,  1455,  1457,  1459,  1461,  1463,  1465,  1467,  1469,  1471,
    1473,  1476,  1479,  1482,  1484,  1486,  1488,  1489,  1492,  1495,
    1500,  1505,  1511,  1515,  1518,  1519,  1522,  1523,  1524,  1526,
    1529,  1532,  1537,  1541,  1544,  1547,  1555,  1557,  1558,  1560,
    1561,  1564,  1567,  1570,  1573,  1576,  1579,  1582,  1585,  1588,
    1592,  1595,  1598,  1601,  1604,  1606,  1608,  1610,  1612,  1617,
    1623,  1625,  1629,  1631,  1635,  1637,  1640,  1642,  1644,  1646,
    1648,  1650,  1652,  1654,  1656,  1658,  1660,  1662,  1667,  1672,
    1679,  1685,  1689,  1690,  1693,  1696,  1697,  1701,  1702,  1707,
    1709,  1715,  1720,  1722,  1727,  1729,  1732,  1735,  1737,  1739,
    1741,  1744,  1747,  1750,  1753,  1754,  1758,  1764,  1766,  1768,
    1771,  1775,  1777,  1780,  1784,  1785,  1787,  1789,  1791,  1797,
    1803,  1810,  1812,  1814,  1816,  1819,  1824,  1826,  1827,  1833,
    1835,  1836,  1840,  1843,  1845,  1846,  1849,  1850,  1852,  1853,
    1857,  1859,  1860,  1862,  1865,  1867,  1869,  1871,  1873,  1876,
    1879,  1881,  1884,  1888,  1890,  1892,  1896,  1900,  1902,  1905,
    1909,  1911,  1914,  1915,  1918,  1920,  1922,  1923,  1925,  1927,
    1929,  1933,  1936,  1940,  1941,  1943,  1947,  1952,  1958,  1965,
    1973,  1975,  1977,  1979,  1981,  1983,  1985,  1988,  1990,  1992,
    1994,  1996,  1998,  2000,  2003,  2005,  2007,  2009,  2012,  2014,
    2016,  2020,  2024,  2025,  2028,  2029,  2032,  2033,  2037,  2041,
    2045,  2047,  2052,  2057,  2059,  2069,  2070,  2071,  2072,  2073,
    2077,  2080,  2082,  2083,  2087,  2093,  2097,  2100,  2106,  2109,
    2111,  2113,  2115,  2117,  2119,  2123,  2125,  2129,  2131,  2132,
    2135,  2137,  2141,  2143,  2145,  2147,  2149,  2153,  2160,  2162,
    2163,  2167,  2168,  2170,  2174,  2176,  2178,  2180,  2185,  2191,
    2197,  2199,  2201,  2204,  2209,  2214,  2217,  2221,  2222,  2224,
    2227,  2229,  2231,  2234,  2237,  2240,  2241,  2243,  2244,  2248,
    2249,  2251,  2255,  2257,  2260,  2261,  2264,  2265,  2268,  2269,
    2274,  2276,  2279,  2281,  2283,  2284,  2286,  2290,  2293,  2295,
    2297,  2300,  2302,  2307,  2311,  2313,  2317,  2322,  2323,  2327,
    2328,  2330,  2334,  2338,  2340,  2342,  2343,  2345,  2347,  2352,
    2353,  2356,  2361,  2362,  2371,  2377,  2379,  2381,  2389,  2394,
    2396,  2398,  2407,  2413,  2418,  2420,  2424,  2428,  2430,  2438,
    2446,  2452,  2456,  2457,  2459,  2461,  2463,  2466,  2467,  2469,
    2471,  2473,  2474,  2478,  2480,  2484,  2486,  2487,  2491,  2493,
    2497,  2499,  2503,  2507,  2509,  2511,  2515,  2517,  2520,  2522,
    2526,  2530,  2532,  2535,  2539,  2543,  2547,  2551,  2553,  2557,
    2560,  2562,  2564,  2566,  2568,  2570,  2572,  2574,  2576,  2578,
    2580,  2582,  2586,  2590,  2594,  2598,  2602,  2606,  2610,  2614,
    2621,  2628,  2635,  2642,  2649,  2656,  2663,  2670,  2677,  2684,
    2691,  2698,  2705,  2712,  2719,  2726,  2728,  2730,  2736,  2743,
    2749,  2756,  2760,  2765,  2771,  2778,  2782,  2787,  2791,  2796,
    2800,  2805,  2810,  2816,  2821,  2826,  2830,  2835,  2837,  2839,
    2841,  2843,  2845,  2847,  2849,  2853,  2857,  2859,  2861,  2863,
    2865,  2867,  2869,  2871,  2873,  2875,  2877,  2880,  2883,  2887,
    2891,  2895,  2899,  2903,  2907,  2911,  2915,  2917,  2919,  2921,
    2923,  2927,  2929,  2931,  2933,  2935,  2937,  2939,  2944,  2946,
    2950,  2952,  2955,  2957,  2959,  2961,  2963,  2965,  2967,  2969,
    2972,  2975,  2978,  2980,  2982,  2984,  2986,  2988,  2990,  2992,
    2994,  2996,  2998,  3001,  3003,  3006,  3008,  3010,  3012,  3014,
    3016,  3019,  3021,  3023,  3025,  3027,  3032,  3038,  3044,  3050,
    3056,  3062,  3068,  3074,  3080,  3086,  3092,  3094,  3096,  3103,
    3105,  3107,  3109,  3114,  3119,  3124,  3129,  3131,  3133,  3138,
    3143,  3151,  3154,  3155,  3163,  3170,  3177,  3182,  3184,  3186,
    3188,  3193,  3197,  3204,  3206,  3208,  3215,  3224,  3231,  3233,
    3235,  3240,  3247,  3252,  3258,  3262,  3268,  3273,  3279,  3281,
    3283,  3285,  3290,  3297,  3299,  3301,  3303,  3305,  3307,  3309,
    3311,  3313,  3315,  3316,  3318,  3319,  3321,  3323,  3325,  3327,
    3329,  3331,  3333,  3335,  3337,  3339,  3341,  3343,  3345,  3347,
    3349,  3351,  3353,  3355,  3357,  3359,  3361,  3363,  3365,  3367,
    3369,  3371,  3373,  3375,  3377,  3379,  3381,  3383,  3385,  3387,
    3389,  3391,  3393,  3395,  3397,  3399,  3401,  3403,  3405,  3407,
    3409,  3411,  3413,  3415,  3417,  3419,  3421,  3423,  3425,  3427,
    3429,  3431,  3433
};
static const short yyrhs[] =
{
     295,     0,   295,   285,     0,   478,     0,   639,     0,   558,
       0,   535,     0,   330,     0,   317,     0,   629,     0,   503,
       0,   296,     0,   628,     0,   456,     0,   459,     0,   332,
       0,   334,     0,   305,     0,   536,     0,   529,     0,   564,
       0,   527,     0,   632,     0,    40,   686,     0,    70,   298,
     122,   297,   604,   174,   308,   302,     0,    70,   300,   122,
     141,   304,   174,   308,   302,     0,    70,   313,   174,   315,
     303,     0,   172,     0,     0,     6,     0,     6,   140,     0,
     299,     0,   301,     0,   299,   286,   301,     0,    56,     0,
     155,     0,    80,     0,    44,     0,   180,   645,     0,   145,
     645,     0,   192,    70,   124,     0,     0,   192,   202,   124,
       0,     0,   739,     0,   150,   306,   298,   122,   297,   604,
      63,   308,     0,   150,   306,   300,   122,   141,   304,    63,
     308,     0,   150,   307,   313,    63,   315,     0,    70,   124,
      61,     0,     0,   202,   124,    61,     0,     0,   309,     0,
     311,     0,   310,     0,   309,   286,   310,     0,   309,   286,
     312,     0,   311,   286,   310,     0,   141,   739,     0,   176,
     743,     0,   187,   746,     0,   206,   740,     0,   312,     0,
     311,   286,   312,     0,   744,     0,   181,   744,     0,    71,
     744,     0,   314,     0,   313,   286,   314,     0,   740,     0,
     316,     0,   315,   286,   316,     0,   744,     0,   181,   744,
       0,    42,   318,     0,    60,   328,     0,    59,    65,   319,
       0,   723,   321,   149,   325,    52,   685,   111,   685,     0,
     510,     0,    18,     0,    34,   287,   689,   288,   516,     0,
       0,   322,     0,   287,   322,   288,     0,   323,     0,   322,
     286,   323,     0,   388,   320,   324,     0,     0,    19,   230,
       0,    19,   262,     0,    94,     0,   326,     0,   287,   326,
     288,     0,   388,   320,   327,     0,   133,   689,     0,     0,
      19,    97,     0,    19,   230,     0,   204,     0,    19,   230,
     204,     0,   732,    78,   329,   128,   329,    52,   685,   111,
     685,     0,   724,     0,   686,     0,    33,   331,     0,    55,
     336,     0,   340,   624,    79,   735,   122,   604,   341,     0,
     141,   407,     0,   172,   375,     0,   176,   470,     0,   187,
     463,     0,    68,   357,     0,   265,   357,     0,    37,   359,
       0,    48,   348,     0,   157,   342,     0,   206,   358,     0,
     229,   333,     0,   141,   408,     0,   172,   376,     0,   187,
     464,     0,    55,   337,     0,    33,   125,     7,   335,     0,
     141,   409,     0,   176,   471,     0,    55,   338,     0,   731,
     685,     0,   731,   685,     0,   731,   685,     0,   731,   685,
       0,   179,     0,     0,   647,     0,   646,     0,   382,   287,
     467,   674,   468,   288,     0,   689,   343,   344,   685,   345,
     346,     0,   105,     0,    14,     0,     0,     0,    29,     0,
       0,    98,   360,   692,   374,     0,     0,   347,     0,   368,
       0,   347,   368,     0,   385,   349,   505,   467,   350,   468,
     351,   384,     0,    10,     0,     0,    43,   467,   390,     0,
       0,     0,   352,     0,   353,     0,   352,   353,     0,   354,
       0,   355,     0,   356,     0,   118,    94,     0,   467,    22,
     287,   654,   288,   468,     0,   734,     0,   740,     0,   361,
     362,   365,     0,     0,   278,     0,   685,     0,     0,   363,
       0,   364,     0,   363,   364,     0,   132,   360,   689,     0,
      98,   360,   692,   374,     0,   181,   685,     0,   134,   685,
       0,   156,   112,   685,     0,     0,   366,     0,   367,     0,
     366,   367,     0,   368,     0,    43,    21,   156,   725,     0,
     255,    90,   685,     0,   369,   685,   370,     0,    90,     0,
       0,   371,     0,   372,     0,   371,   372,     0,   167,   373,
     692,     0,    98,   360,   692,   374,     0,     0,    12,     0,
      12,   130,     0,     0,   130,     0,   131,     0,   604,   377,
     287,   378,   288,     0,   604,   377,   287,   378,   288,     0,
      59,    90,   685,     0,    59,   685,     0,     0,   379,     0,
     378,   286,   379,     0,   380,     0,   395,     0,   385,   383,
     350,   468,   391,   384,     0,   385,   506,   381,     0,   385,
     381,     0,   382,   287,   467,   674,   468,   288,     0,    27,
      19,     0,    27,     0,   505,   467,     0,   652,   465,     0,
      23,   726,     0,     0,   652,     0,   652,     0,   388,   505,
       0,     0,    43,   390,     0,     0,   678,     0,   682,     0,
     683,     0,   684,     0,   722,     0,   675,     0,     0,   392,
       0,   393,     0,   392,   393,     0,   396,   394,     0,   118,
      94,     0,   145,   604,   645,   403,   401,     0,   402,     0,
     179,   401,     0,   139,    86,   401,     0,   396,   397,     0,
      30,   728,     0,     0,   398,     0,   399,     0,   400,     0,
     402,     0,   179,   646,   401,     0,   139,    86,   646,   401,
       0,    62,    86,   646,   145,   604,   645,   403,   401,     0,
     239,   624,    79,   735,     0,     0,   467,    22,   287,   654,
     288,   468,     0,   404,     0,   405,     0,   405,   404,     0,
     404,   405,     0,     0,   122,   180,   406,     0,   122,    44,
     406,     0,   203,     0,   156,    43,     0,   156,    94,     0,
     117,   201,     0,   739,   411,   412,    10,   465,   418,   427,
     468,     0,   739,   411,   412,    10,   465,   418,   427,   468,
       0,   739,   411,   412,    10,   465,   418,   427,   468,     0,
     739,   411,   412,    10,   465,   418,   427,   468,     0,   287,
     413,   288,     0,     0,   149,   287,   415,   288,     0,     0,
     414,     0,   413,   286,   414,     0,   386,   506,   417,   468,
       0,   416,     0,   415,   286,   416,     0,   386,   506,     0,
      43,   465,   390,     0,   278,   465,   390,     0,   465,     0,
     419,     0,     0,   420,     0,   419,   420,     0,    42,   423,
     421,   285,     0,   422,     0,   425,     0,   385,   506,   424,
       0,   184,     0,     0,   278,   390,     0,   389,     0,   729,
      36,    61,   287,   564,   288,     0,   430,     0,   427,     0,
      16,   428,    51,     0,   429,     0,   429,   447,     0,     0,
     426,     0,   429,   426,     0,   637,   285,     0,   628,   285,
       0,   632,   285,     0,   629,   285,     0,   440,   285,     0,
     456,   285,     0,   433,   285,     0,   436,   285,     0,   638,
     285,     0,   431,   285,     0,   432,   285,     0,   438,   285,
       0,   451,   285,     0,   445,   285,     0,   170,   285,     0,
      58,   285,     0,   437,     0,   443,     0,   434,     0,   435,
       0,    55,   731,     0,    55,   731,   674,     0,    55,     0,
      56,   246,   674,     0,   444,    61,   564,    82,   442,   446,
      47,   426,     0,   444,    61,    56,   246,   674,    82,   442,
      47,   426,     0,    56,   246,   674,    82,   442,     0,    74,
     287,   654,   288,   173,   426,    50,   426,     0,    74,   287,
     654,   288,   173,   426,     0,   137,   674,   439,     0,     0,
     564,    82,   442,     0,   289,   745,     0,   441,     0,   651,
       0,   442,   286,   651,     0,   442,   286,   441,     0,   444,
     191,   287,   654,   288,    47,   426,     0,   737,   289,     0,
       0,   227,     0,   247,     0,   247,   737,     0,    10,    36,
     729,     0,     0,   448,     0,   447,   448,     0,   189,   449,
      47,   426,     0,   450,     0,   449,   286,   450,     0,   165,
     686,     0,    66,   733,     0,    55,   731,     0,     9,     0,
     452,     0,   454,     0,   453,     0,   256,   729,     0,   257,
     729,     0,   258,   455,   729,    82,   442,     0,     0,    56,
     141,   739,   457,   458,     0,   677,     0,   287,   677,   288,
       0,     0,   148,   442,     0,   148,   287,   442,   288,     0,
       0,    56,   260,   460,   412,    10,   418,   427,     0,   287,
     461,   288,     0,     0,   462,     0,   461,   286,   462,     0,
     416,   278,   681,     0,   746,   645,    10,   465,   568,   469,
     466,     0,   746,   645,    10,   465,   568,   469,   466,     0,
       0,     0,     0,     0,   192,    22,   124,     0,     0,   743,
      61,   604,   472,   473,   476,   467,   477,   468,     0,   743,
      61,   604,   472,   473,   476,   467,   477,   468,     0,     3,
       0,    76,     0,     0,   474,   475,     0,    15,     0,     5,
       0,    80,     0,   180,     0,    44,     0,    80,   125,   180,
       0,    80,   125,    44,     0,   180,   125,    80,     0,   180,
     125,    44,     0,    44,   125,    80,     0,    44,   125,   180,
       0,    80,   125,   180,   125,    44,     0,    80,   125,    44,
     125,   180,     0,   180,   125,    80,   125,    44,     0,   180,
     125,    44,   125,    80,     0,    44,   125,    80,   125,   180,
       0,    44,   125,   180,   125,    80,     0,   136,   687,     0,
       0,    10,   467,   418,   427,     0,     7,   479,     0,    55,
     339,     0,   172,   604,   484,     0,   176,   500,     0,   141,
     410,     0,    37,   497,   498,     0,    48,   486,   482,     0,
      79,   492,     0,   265,   493,     0,    59,    65,   494,     0,
      43,   467,   390,     0,    22,   467,   287,   654,   288,   468,
       0,   483,     0,   482,   483,     0,   156,   467,   480,   468,
       0,     4,    30,   481,     0,     4,   356,     0,    49,    43,
       0,    49,    30,     0,   174,   652,     0,   208,   388,   506,
       0,   485,     0,   484,   286,   485,     0,    49,   652,   491,
       0,    49,    30,   728,     0,     4,   380,     0,     4,   395,
       0,   488,   652,   136,   689,     0,   488,   486,   174,   652,
       0,   488,   490,   208,   489,   468,     0,   487,     0,   748,
       0,   202,     0,   207,     0,   209,     0,   210,     0,   211,
       0,   212,     0,   213,     0,   214,     0,   215,     0,   218,
       0,   219,     0,   220,     0,   221,     0,   222,     0,   225,
       0,   226,     0,   229,     0,   233,     0,   234,     0,   235,
       0,   236,     0,   245,     0,   242,     0,   244,     0,   256,
       0,   257,     0,   258,     0,   259,     0,   239,     0,   263,
       0,   270,     0,   267,     0,   271,     0,   272,     0,   269,
       0,   273,     0,   274,     0,   275,     0,   276,     0,   277,
       0,     7,     0,     7,   207,     0,   506,   467,     0,   652,
     465,     0,   652,     0,   205,     0,   203,     0,     0,   735,
       3,     0,   735,    76,     0,   734,   266,   192,   691,     0,
     734,   266,   192,   223,     0,   734,   266,   192,   282,   223,
       0,   723,   495,   496,     0,    52,   685,     0,     0,   111,
     685,     0,     0,     0,   499,     0,   498,   499,     0,     4,
     347,     0,     4,   255,    90,   685,     0,    49,   255,    90,
       0,    16,   254,     0,    51,   254,     0,   743,   472,   501,
     476,   467,   502,   468,     0,   473,     0,     0,   477,     0,
       0,    49,   504,     0,    55,   731,     0,    79,   735,     0,
     141,   739,     0,   172,   742,     0,   176,   743,     0,   187,
     746,     0,    60,   732,     0,    48,   730,     0,    59,    65,
     723,     0,   157,   689,     0,   206,   740,     0,    68,   734,
       0,   265,   734,     0,   506,     0,   507,     0,   510,     0,
     513,     0,   511,   290,   508,   291,     0,   518,   290,   508,
     291,   516,     0,   509,     0,   508,   286,   509,     0,   691,
       0,   691,   289,   691,     0,   511,     0,   518,   516,     0,
     517,     0,   522,     0,   525,     0,   235,     0,   512,     0,
     161,     0,    38,     0,   218,     0,   219,     0,    81,     0,
      92,     0,    18,   515,   514,   516,     0,    18,   287,   690,
     288,     0,    18,   287,   690,   286,   686,   288,     0,    18,
     287,   286,   686,   288,     0,   154,   160,   690,     0,     0,
     169,   686,     0,   169,   724,     0,     0,    21,   156,   725,
       0,     0,   521,   287,   689,   288,     0,   521,     0,   521,
     185,   287,   689,   288,     0,   520,   287,   689,   288,     0,
     520,     0,   519,   287,   689,   288,     0,   183,     0,    21,
     185,     0,    87,   185,     0,    21,     0,    87,     0,   115,
       0,   113,    21,     0,   113,    87,     0,    95,   523,     0,
     524,   523,     0,     0,   287,   691,   288,     0,   287,   691,
     286,   691,   288,     0,    41,     0,    88,     0,    91,   526,
       0,    93,    91,   526,     0,   144,     0,    89,   138,     0,
     287,   687,   288,     0,     0,   540,     0,   528,     0,   557,
       0,   156,    68,   734,   174,   691,     0,   156,    68,   734,
     174,   223,     0,   156,    68,   734,   174,   282,   223,     0,
     530,     0,   531,     0,   533,     0,   244,   747,     0,   245,
     244,   747,   532,     0,   123,     0,     0,   153,   537,   174,
     534,   747,     0,   244,     0,     0,    25,   537,   538,     0,
     153,   537,     0,   193,     0,     0,   147,   539,     0,     0,
     162,     0,     0,   156,   175,   541,     0,   542,     0,     0,
     543,     0,   542,   543,     0,   544,     0,   545,     0,   546,
       0,   550,     0,   143,   123,     0,   143,   194,     0,   188,
       0,   117,   188,     0,    84,   102,   547,     0,   547,     0,
     548,     0,   143,   177,   549,     0,   143,    26,   549,     0,
     162,     0,   162,   172,     0,   162,   172,   166,     0,   186,
       0,   117,   186,     0,     0,   146,   553,     0,   158,     0,
     142,     0,     0,   143,     0,   194,     0,   554,     0,   553,
     286,   554,     0,   556,   555,     0,    61,   551,   552,     0,
       0,   604,     0,   556,   286,   604,     0,   156,   168,    79,
     735,     0,   269,   122,   559,    83,   563,     0,   269,   122,
     560,   738,    83,   563,     0,   269,   122,   561,   738,   562,
      83,   563,     0,    37,     0,    48,     0,   172,     0,   187,
       0,   141,     0,   176,     0,    59,    65,     0,    60,     0,
      55,     0,    68,     0,   265,     0,    79,     0,   206,     0,
      21,   156,     0,   268,     0,   207,     0,   133,     0,   292,
     738,     0,   685,     0,    94,     0,   568,   565,   567,     0,
      61,   180,   566,     0,     0,   121,   647,     0,     0,   192,
     243,     0,     0,   571,   621,   627,     0,   571,   621,   627,
       0,   571,   621,   627,     0,   572,     0,   571,   178,   721,
     572,     0,   571,   178,     6,   572,     0,   573,     0,   155,
     578,   581,   582,   586,   611,   607,   610,   612,     0,     0,
       0,     0,     0,   579,   580,   575,     0,   579,   575,     0,
     580,     0,     0,   231,   692,   574,     0,   231,   287,   674,
     288,   574,     0,   231,   681,   574,     0,   232,   692,     0,
     232,   287,   575,   674,   288,     0,   232,   681,     0,    46,
       0,   720,     0,   583,     0,   283,     0,   584,     0,   583,
     286,   584,     0,   674,     0,   674,   585,   736,     0,    10,
       0,     0,    63,   587,     0,   588,     0,   587,   286,   588,
       0,   594,     0,   589,     0,   601,     0,   590,     0,   287,
     594,   288,     0,   287,   568,   288,   585,   591,   592,     0,
     741,     0,     0,   287,   593,   288,     0,     0,   736,     0,
     593,   286,   736,     0,   595,     0,   596,     0,   597,     0,
     588,   263,    85,   589,     0,   588,   114,   605,    85,   589,
       0,   588,   605,    85,   588,   598,     0,   599,     0,   600,
       0,   122,   654,     0,   239,   287,   647,   288,     0,   739,
     602,   585,   741,     0,   739,   602,     0,   287,   677,   288,
       0,     0,   604,     0,   742,   741,     0,   742,     0,    77,
       0,   100,   606,     0,   151,   606,     0,    64,   606,     0,
       0,   127,     0,     0,    71,    19,   608,     0,     0,   609,
       0,   608,   286,   609,     0,   674,     0,    73,   654,     0,
       0,   190,   654,     0,     0,   135,   613,     0,     0,   614,
     287,   615,   288,     0,    85,     0,   164,   108,     0,   108,
       0,   164,     0,     0,   616,     0,   616,   286,   615,     0,
     617,   618,     0,   613,     0,   742,     0,   742,   617,     0,
     114,     0,    79,   287,   619,   288,     0,   126,   735,   620,
       0,   735,     0,   735,   286,   619,     0,    79,   287,   619,
     288,     0,     0,   126,    19,   622,     0,     0,   623,     0,
     622,   286,   623,     0,   674,   624,   626,     0,    11,     0,
      45,     0,     0,   231,     0,   241,     0,   240,   576,   625,
     577,     0,     0,   259,   674,     0,   259,   674,   174,   674,
       0,     0,    80,    82,   604,   648,   182,   287,   677,   288,
       0,    80,    82,   604,   648,   568,     0,   630,     0,   631,
       0,    44,    63,   603,   611,   612,   621,   627,     0,    44,
      63,   603,   635,     0,   633,     0,   634,     0,   180,   603,
     156,   636,   611,   612,   621,   627,     0,   180,   603,   156,
     636,   635,     0,   190,    35,   121,   729,     0,   637,     0,
     636,   286,   637,     0,   653,   278,   674,     0,   706,     0,
     143,    18,   652,    63,   604,   640,   643,     0,    80,    18,
     652,    82,   604,   640,   643,     0,    60,    63,   641,   174,
     641,     0,    60,   174,   641,     0,     0,   642,     0,   681,
       0,   686,     0,   107,   644,     0,     0,   690,     0,   681,
       0,   646,     0,     0,   287,   647,   288,     0,   652,     0,
     647,   286,   652,     0,   649,     0,     0,   287,   650,   288,
       0,   653,     0,   650,   286,   653,     0,   652,     0,   741,
     292,   727,     0,   741,   292,   283,     0,   727,     0,   652,
       0,   741,   292,   727,     0,   670,     0,   118,   670,     0,
     656,     0,   654,   125,   654,     0,   654,     8,   654,     0,
     656,     0,   118,   670,     0,   655,   125,   654,     0,   655,
       8,   654,     0,   671,   125,   654,     0,   671,     8,   654,
       0,   657,     0,   287,   655,   288,     0,   118,   656,     0,
     658,     0,   661,     0,   662,     0,   663,     0,   664,     0,
     669,     0,   659,     0,   667,     0,   665,     0,   666,     0,
     668,     0,   674,   278,   674,     0,   674,   279,   674,     0,
     674,   280,   674,     0,   674,    67,   674,     0,   674,   101,
     674,     0,   674,   119,   674,     0,   674,   120,   674,     0,
     674,   116,   674,     0,   674,   278,     6,   287,   569,   288,
       0,   674,   279,     6,   287,   569,   288,     0,   674,   280,
       6,   287,   569,   288,     0,   674,    67,     6,   287,   569,
     288,     0,   674,   101,     6,   287,   569,   288,     0,   674,
     119,     6,   287,   569,   288,     0,   674,   120,     6,   287,
     569,   288,     0,   674,   116,     6,   287,   569,   288,     0,
     674,   278,   660,   287,   569,   288,     0,   674,   279,   660,
     287,   569,   288,     0,   674,   280,   660,   287,   569,   288,
       0,   674,    67,   660,   287,   569,   288,     0,   674,   101,
     660,   287,   569,   288,     0,   674,   119,   660,   287,   569,
     288,     0,   674,   120,   660,   287,   569,   288,     0,   674,
     116,   660,   287,   569,   288,     0,   163,     0,     9,     0,
     674,    83,    46,    63,   674,     0,   674,    83,   118,    46,
      63,   674,     0,   674,    17,   674,     8,   674,     0,   674,
     118,    17,   674,     8,   674,     0,   674,   103,   674,     0,
     674,   118,   103,   674,     0,   674,   103,   674,    54,   674,
       0,   674,   118,   103,   674,    54,   674,     0,   674,    75,
     672,     0,   674,   118,    75,   672,     0,   674,    31,   674,
       0,   674,   118,    31,   674,     0,   674,   167,   674,     0,
     674,   118,   167,   674,     0,   674,   167,   192,   674,     0,
     674,   118,   167,   192,   674,     0,    57,   287,   568,   288,
       0,   159,   287,   568,   288,     0,   674,    83,    94,     0,
     674,    83,   118,    94,     0,   248,     0,   249,     0,   250,
       0,   251,     0,   252,     0,   253,     0,   673,     0,   287,
     677,   288,     0,   287,   569,   288,     0,   651,     0,   676,
       0,   693,     0,   680,     0,   681,     0,   441,     0,   707,
       0,   708,     0,   718,     0,   706,     0,   282,   674,     0,
     281,   674,     0,   674,   281,   674,     0,   674,    28,   674,
       0,   674,    23,   726,     0,   674,   282,   674,     0,   674,
     283,   674,     0,   674,   284,   674,     0,   287,   674,   288,
       0,   287,   570,   288,     0,   682,     0,   683,     0,   684,
       0,    39,     0,   741,   292,    39,     0,    97,     0,   675,
       0,   722,     0,   220,     0,   221,     0,   222,     0,   651,
     290,   677,   291,     0,   674,     0,   677,   286,   674,     0,
     680,     0,   282,   679,     0,   197,     0,   196,     0,   195,
       0,   223,     0,   224,     0,   679,     0,   685,     0,    38,
     199,     0,   218,   199,     0,   219,   199,     0,   293,     0,
     181,     0,   225,     0,   226,     0,   233,     0,   234,     0,
      66,     0,   165,     0,   242,     0,   199,     0,   200,   199,
       0,   687,     0,   282,   688,     0,   196,     0,   196,     0,
     687,     0,   196,     0,   692,     0,   282,   692,     0,   196,
       0,   694,     0,   695,     0,   701,     0,    32,   287,   283,
     288,     0,    32,   287,   720,   674,   288,     0,    32,   287,
      46,   674,   288,     0,   171,   287,   720,   674,   288,     0,
     171,   287,    46,   674,   288,     0,    13,   287,   720,   674,
     288,     0,    13,   287,    46,   674,   288,     0,   110,   287,
     720,   674,   288,     0,   110,   287,    46,   674,   288,     0,
     106,   287,   720,   674,   288,     0,   106,   287,    46,   674,
     288,     0,   696,     0,   697,     0,   209,   287,   719,    63,
     674,   288,     0,   698,     0,   699,     0,   700,     0,   270,
     287,   674,   288,     0,   271,   287,   674,   288,     0,   272,
     287,   674,   288,     0,   275,   287,   674,   288,     0,   702,
       0,   704,     0,    96,   287,   674,   288,     0,   274,   287,
     674,   288,     0,   228,   287,   674,    63,   674,   703,   288,
       0,    61,   674,     0,     0,   277,   287,   705,   674,    63,
     674,   288,     0,   277,   287,   674,    63,   674,   288,     0,
     277,   287,   705,    63,   674,   288,     0,   277,   287,   674,
     288,     0,   267,     0,   276,     0,   273,     0,   723,   287,
     677,   288,     0,   723,   287,   288,     0,    20,   287,   674,
      10,   387,   288,     0,   709,     0,   710,     0,   237,   287,
     674,   286,   674,   288,     0,   261,   287,   654,   286,   674,
     286,   674,   288,     0,   238,   287,   674,   286,   677,   288,
       0,   711,     0,   713,     0,   236,   716,   712,    51,     0,
     236,   716,   712,    50,   717,    51,     0,   189,   715,   173,
     717,     0,   712,   189,   715,   173,   717,     0,   236,   714,
      51,     0,   236,   714,    50,   717,    51,     0,   189,   654,
     173,   717,     0,   714,   189,   654,   173,   717,     0,   674,
       0,   674,     0,   674,     0,   264,    97,    61,   734,     0,
      69,   287,   734,   286,   674,   288,     0,   210,     0,   211,
       0,   212,     0,   213,     0,   214,     0,   215,     0,   216,
       0,   217,     0,     6,     0,     0,    46,     0,     0,    94,
       0,   198,     0,   748,     0,   748,     0,   748,     0,   748,
       0,   748,     0,   748,     0,   748,     0,   748,     0,   748,
       0,   748,     0,   748,     0,   748,     0,   748,     0,   748,
       0,   748,     0,   748,     0,   748,     0,   748,     0,   748,
       0,   748,     0,   748,     0,   748,     0,   748,     0,   748,
       0,   198,     0,   749,     0,   201,     0,   203,     0,   204,
       0,   205,     0,   206,     0,   208,     0,   227,     0,   230,
       0,   228,     0,   238,     0,   241,     0,   247,     0,   243,
       0,   237,     0,   240,     0,   246,     0,   248,     0,   249,
       0,   250,     0,   260,     0,   254,     0,   268,     0,   255,
       0,   261,     0,   262,     0,   216,     0,   217,     0,   265,
       0,   264,     0,   266,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   550,   552,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   584,   588,   592,   597,   598,   601,
     603,   605,   609,   610,   614,   618,   620,   622,   624,   626,
     630,   632,   636,   638,   642,   649,   653,   657,   662,   664,
     668,   670,   674,   675,   678,   679,   681,   683,   687,   689,
     691,   693,   697,   698,   705,   707,   709,   713,   714,   718,
     722,   723,   727,   729,   736,   740,   742,   747,   753,   754,
     756,   762,   764,   765,   769,   770,   776,   781,   783,   785,
     787,   791,   792,   796,   799,   804,   806,   808,   810,   813,
     818,   824,   826,   833,   837,   839,   842,   844,   846,   848,
     850,   852,   854,   856,   858,   860,   869,   873,   875,   877,
     885,   889,   893,   895,   901,   908,   913,   918,   923,   931,
     933,   937,   939,   940,   946,   952,   954,   956,   960,   962,
     966,   968,   972,   974,   977,   978,   985,  1003,  1005,  1009,
    1011,  1015,  1017,  1020,  1021,  1024,  1029,  1030,  1033,  1037,
    1046,  1053,  1118,  1123,  1124,  1127,  1131,  1133,  1136,  1137,
    1141,  1143,  1145,  1147,  1149,  1153,  1155,  1158,  1159,  1163,
    1164,  1166,  1170,  1176,  1180,  1181,  1184,  1185,  1188,  1190,
    1194,  1195,  1196,  1199,  1200,  1201,  1207,  1212,  1217,  1219,
    1221,  1225,  1226,  1230,  1231,  1238,  1242,  1246,  1255,  1261,
    1262,  1265,  1267,  1272,  1274,  1279,  1285,  1291,  1295,  1301,
    1303,  1307,  1308,  1309,  1310,  1311,  1312,  1315,  1317,  1320,
    1321,  1325,  1330,  1332,  1337,  1338,  1340,  1348,  1352,  1354,
    1357,  1358,  1359,  1360,  1363,  1367,  1371,  1378,  1385,  1390,
    1396,  1399,  1401,  1403,  1405,  1409,  1412,  1416,  1419,  1422,
    1425,  1434,  1445,  1455,  1465,  1475,  1477,  1481,  1483,  1487,
    1488,  1492,  1498,  1499,  1503,  1508,  1510,  1512,  1516,  1518,
    1522,  1523,  1527,  1531,  1532,  1535,  1540,  1542,  1546,  1548,
    1552,  1557,  1558,  1561,  1565,  1567,  1569,  1573,  1574,  1578,
    1579,  1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,
    1589,  1590,  1591,  1592,  1594,  1596,  1597,  1598,  1599,  1602,
    1604,  1608,  1612,  1616,  1621,  1625,  1629,  1631,  1635,  1639,
    1645,  1650,  1655,  1656,  1657,  1659,  1663,  1667,  1669,  1673,
    1675,  1677,  1682,  1685,  1689,  1690,  1694,  1699,  1700,  1704,
    1706,  1708,  1710,  1714,  1715,  1716,  1719,  1723,  1727,  1731,
    1774,  1779,  1781,  1783,  1787,  1789,  1791,  1797,  1805,  1807,
    1811,  1812,  1816,  1822,  1829,  1852,  1856,  1862,  1866,  1872,
    1876,  1884,  1895,  1906,  1908,  1910,  1914,  1918,  1920,  1924,
    1926,  1928,  1930,  1932,  1934,  1936,  1938,  1940,  1942,  1944,
    1946,  1948,  1950,  1952,  1956,  1958,  1962,  1968,  1972,  1974,
    1981,  1983,  1985,  1988,  1991,  1993,  1995,  1999,  2003,  2009,
    2010,  2014,  2023,  2027,  2029,  2031,  2033,  2035,  2039,  2040,
    2044,  2046,  2048,  2050,  2056,  2059,  2061,  2065,  2073,  2074,
    2075,  2076,  2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,
    2085,  2086,  2087,  2088,  2089,  2090,  2091,  2092,  2093,  2094,
    2095,  2096,  2097,  2098,  2099,  2100,  2101,  2102,  2103,  2104,
    2105,  2106,  2107,  2108,  2110,  2111,  2112,  2114,  2115,  2118,
    2120,  2124,  2126,  2131,  2137,  2139,  2141,  2145,  2147,  2151,
    2154,  2157,  2162,  2166,  2168,  2172,  2174,  2181,  2185,  2186,
    2190,  2192,  2194,  2196,  2198,  2205,  2215,  2216,  2220,  2221,
    2227,  2231,  2233,  2235,  2237,  2239,  2241,  2243,  2245,  2247,
    2249,  2251,  2253,  2255,  2262,  2263,  2266,  2267,  2270,  2275,
    2282,  2283,  2287,  2296,  2302,  2303,  2306,  2307,  2308,  2309,
    2326,  2331,  2336,  2355,  2372,  2379,  2380,  2387,  2392,  2399,
    2406,  2415,  2419,  2425,  2429,  2433,  2439,  2443,  2450,  2456,
    2462,  2470,  2475,  2480,  2487,  2488,  2489,  2492,  2493,  2496,
    2497,  2498,  2505,  2509,  2520,  2526,  2580,  2642,  2643,  2650,
    2663,  2668,  2673,  2680,  2682,  2689,  2690,  2691,  2695,  2698,
    2701,  2709,  2710,  2711,  2714,  2718,  2722,  2724,  2728,  2732,
    2733,  2736,  2740,  2744,  2745,  2748,  2750,  2754,  2755,  2759,
    2763,  2764,  2768,  2769,  2773,  2774,  2775,  2776,  2779,  2781,
    2785,  2787,  2791,  2793,  2796,  2798,  2800,  2804,  2806,  2808,
    2812,  2814,  2816,  2820,  2824,  2826,  2828,  2832,  2834,  2838,
    2839,  2843,  2847,  2849,  2853,  2854,  2859,  2863,  2865,  2867,
    2871,  2875,  2877,  2879,  2881,  2883,  2885,  2887,  2889,  2891,
    2893,  2895,  2897,  2899,  2901,  2909,  2911,  2915,  2919,  2920,
    2927,  2931,  2933,  2937,  2939,  2943,  2945,  2952,  2956,  2961,
    2966,  2967,  2969,  2973,  2976,  2988,  2992,  2996,  3000,  3004,
    3006,  3008,  3010,  3014,  3016,  3018,  3022,  3024,  3026,  3030,
    3032,  3036,  3038,  3042,  3043,  3047,  3048,  3052,  3053,  3058,
    3062,  3063,  3067,  3068,  3071,  3072,  3073,  3078,  3083,  3084,
    3088,  3090,  3094,  3095,  3099,  3100,  3101,  3104,  3109,  3114,
    3118,  3119,  3122,  3126,  3130,  3133,  3138,  3140,  3144,  3145,
    3150,  3155,  3157,  3159,  3161,  3163,  3167,  3168,  3174,  3176,
    3180,  3181,  3187,  3190,  3192,  3196,  3198,  3205,  3207,  3211,
    3215,  3217,  3219,  3225,  3227,  3231,  3232,  3236,  3238,  3241,
    3242,  3246,  3248,  3250,  3254,  3255,  3259,  3261,  3267,  3269,
    3273,  3274,  3278,  3282,  3284,  3286,  3290,  3292,  3296,  3298,
    3304,  3307,  3315,  3322,  3325,  3332,  3333,  3336,  3340,  3347,
    3348,  3351,  3356,  3362,  3369,  3370,  3374,  3378,  3385,  3387,
    3391,  3393,  3395,  3399,  3400,  3403,  3407,  3409,  3413,  3415,
    3421,  3422,  3426,  3430,  3431,  3436,  3437,  3441,  3445,  3446,
    3451,  3452,  3455,  3460,  3465,  3468,  3475,  3476,  3478,  3479,
    3481,  3485,  3486,  3488,  3490,  3494,  3496,  3500,  3501,  3503,
    3507,  3508,  3509,  3510,  3511,  3512,  3513,  3514,  3515,  3516,
    3517,  3522,  3524,  3526,  3528,  3530,  3532,  3534,  3536,  3542,
    3544,  3546,  3548,  3550,  3552,  3554,  3556,  3558,  3560,  3562,
    3564,  3566,  3568,  3570,  3572,  3576,  3577,  3583,  3585,  3589,
    3591,  3596,  3598,  3600,  3602,  3607,  3609,  3613,  3615,  3619,
    3621,  3623,  3625,  3629,  3633,  3637,  3639,  3643,  3648,  3653,
    3660,  3665,  3670,  3679,  3680,  3684,  3691,  3692,  3693,  3694,
    3695,  3696,  3697,  3698,  3699,  3700,  3701,  3703,  3705,  3712,
    3714,  3716,  3723,  3730,  3737,  3739,  3741,  3742,  3743,  3744,
    3746,  3748,  3752,  3753,  3756,  3772,  3788,  3792,  3796,  3797,
    3801,  3802,  3806,  3808,  3810,  3812,  3814,  3818,  3819,  3821,
    3837,  3853,  3857,  3861,  3863,  3867,  3871,  3874,  3877,  3880,
    3883,  3888,  3890,  3895,  3896,  3900,  3907,  3914,  3921,  3928,
    3929,  3933,  3940,  3941,  3942,  3945,  3947,  3949,  3952,  3959,
    3968,  3975,  3984,  3986,  3988,  3990,  3994,  3995,  3998,  4002,
    4003,  4004,  4007,  4012,  4015,  4020,  4025,  4026,  4027,  4029,
    4033,  4042,  4044,  4048,  4050,  4053,  4055,  4060,  4062,  4064,
    4068,  4070,  4074,  4080,  4081,  4084,  4088,  4091,  4095,  4096,
    4099,  4101,  4105,  4107,  4111,  4113,  4117,  4119,  4123,  4126,
    4129,  4134,  4137,  4147,  4149,  4151,  4153,  4155,  4157,  4159,
    4161,  4165,  4166,  4169,  4170,  4173,  4181,  4184,  4187,  4190,
    4193,  4196,  4199,  4202,  4205,  4208,  4211,  4214,  4217,  4220,
    4223,  4226,  4229,  4232,  4235,  4238,  4241,  4244,  4247,  4250,
    4253,  4258,  4259,  4264,  4266,  4267,  4268,  4269,  4270,  4271,
    4272,  4273,  4274,  4275,  4276,  4277,  4278,  4279,  4280,  4281,
    4282,  4283,  4285,  4287,  4288,  4289,  4290,  4292,  4294,  4295,
    4296,  4297,  4298
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "ACTIVE", "ADD", "AFTER", "ALL", "ALTER", 
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
  "'?'", "top", "statement", "grant", "table_noise", "privileges", 
  "privilege_list", "proc_privileges", "privilege", "grant_option", 
  "role_admin_option", "simple_proc_name", "revoke", "rev_grant_option", 
  "rev_admin_option", "non_role_grantee_list", "grantee_list", "grantee", 
  "user_grantee_list", "user_grantee", "role_name_list", "role_name", 
  "role_grantee_list", "role_grantee", "declare", "declare_clause", 
  "udf_decl_clause", "udf_data_type", "arg_desc_list1", "arg_desc_list", 
  "arg_desc", "param_mechanism", "return_value1", "return_value", 
  "return_mechanism", "filter_decl_clause", "blob_filter_subtype", 
  "create", "create_clause", "recreate", "recreate_clause", "replace", 
  "replace_clause", "exception_clause", "rexception_clause", 
  "replace_exception_clause", "alter_exception_clause", "unique_opt", 
  "index_definition", "shadow_clause", "manual_auto", "conditional", 
  "first_file_length", "sec_shadow_files", "db_file_list", 
  "domain_clause", "as_opt", "domain_default_opt", 
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
  "string_value_function", "substring_function", "string_length_opt", 
  "trim_function", "trim_specification", "udf", "cast_specification", 
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

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   294,   294,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   295,   295,   296,   296,   296,   297,   297,   298,
     298,   298,   299,   299,   300,   301,   301,   301,   301,   301,
     302,   302,   303,   303,   304,   305,   305,   305,   306,   306,
     307,   307,   308,   308,   309,   309,   309,   309,   310,   310,
     310,   310,   311,   311,   312,   312,   312,   313,   313,   314,
     315,   315,   316,   316,   317,   318,   318,   319,   320,   320,
     320,   321,   321,   321,   322,   322,   323,   324,   324,   324,
     324,   325,   325,   326,   326,   327,   327,   327,   327,   327,
     328,   329,   329,   330,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   332,   333,   333,   333,
     333,   334,   335,   335,   335,   336,   337,   338,   339,   340,
     340,   341,   341,   341,   342,   343,   343,   343,   344,   344,
     345,   345,   346,   346,   347,   347,   348,   349,   349,   350,
     350,   351,   351,   352,   352,   353,   354,   354,   355,   356,
     357,   358,   359,   360,   360,   361,   362,   362,   363,   363,
     364,   364,   364,   364,   364,   365,   365,   366,   366,   367,
     367,   367,   368,   369,   370,   370,   371,   371,   372,   372,
     373,   373,   373,   374,   374,   374,   375,   376,   377,   377,
     377,   378,   378,   379,   379,   380,   380,   380,   381,   382,
     382,   383,   383,   384,   384,   385,   386,   387,   388,   389,
     389,   390,   390,   390,   390,   390,   390,   391,   391,   392,
     392,   393,   394,   394,   394,   394,   394,   395,   396,   396,
     397,   397,   397,   397,   398,   399,   400,   401,   401,   402,
     403,   403,   403,   403,   403,   404,   405,   406,   406,   406,
     406,   407,   408,   409,   410,   411,   411,   412,   412,   413,
     413,   414,   415,   415,   416,   417,   417,   417,   418,   418,
     419,   419,   420,   421,   421,   422,   423,   423,   424,   424,
     425,   426,   426,   427,   428,   428,   428,   429,   429,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   430,
     430,   430,   430,   430,   430,   430,   430,   430,   430,   431,
     431,   432,   433,   434,   435,   436,   437,   437,   438,   439,
     440,   441,   442,   442,   442,   442,   443,   444,   444,   445,
     445,   445,   446,   446,   447,   447,   448,   449,   449,   450,
     450,   450,   450,   451,   451,   451,   452,   453,   454,   455,
     456,   457,   457,   457,   458,   458,   458,   459,   460,   460,
     461,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     469,   470,   471,   472,   472,   472,   473,   474,   474,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   476,   476,   477,   478,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   480,   481,   482,
     482,   483,   483,   483,   483,   483,   483,   483,   484,   484,
     485,   485,   485,   485,   485,   485,   485,   486,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   487,
     487,   487,   487,   487,   487,   487,   487,   487,   487,   488,
     488,   489,   489,   490,   491,   491,   491,   492,   492,   493,
     493,   493,   494,   495,   495,   496,   496,   497,   498,   498,
     499,   499,   499,   499,   499,   500,   501,   501,   502,   502,
     503,   504,   504,   504,   504,   504,   504,   504,   504,   504,
     504,   504,   504,   504,   505,   505,   506,   506,   507,   507,
     508,   508,   509,   509,   510,   510,   511,   511,   511,   511,
     511,   511,   511,   511,   511,   512,   512,   513,   513,   513,
     513,   514,   514,   515,   515,   515,   516,   516,   517,   517,
     517,   518,   518,   518,   519,   519,   519,   520,   520,   521,
     521,   521,   522,   522,   523,   523,   523,   524,   524,   525,
     525,   525,   525,   526,   526,   527,   527,   527,   528,   528,
     528,   529,   529,   529,   530,   531,   532,   532,   533,   534,
     534,   535,   536,   537,   537,   538,   538,   539,   539,   540,
     541,   541,   542,   542,   543,   543,   543,   543,   544,   544,
     545,   545,   546,   546,   547,   547,   547,   548,   548,   548,
     549,   549,   549,   550,   551,   551,   551,   552,   552,   553,
     553,   554,   555,   555,   556,   556,   557,   558,   558,   558,
     559,   560,   560,   560,   560,   560,   560,   560,   560,   560,
     560,   560,   560,   560,   560,   561,   561,   562,   563,   563,
     564,   565,   565,   566,   566,   567,   567,   568,   569,   570,
     571,   571,   571,   572,   573,   574,   575,   576,   577,   578,
     578,   578,   578,   579,   579,   579,   580,   580,   580,   581,
     581,   582,   582,   583,   583,   584,   584,   585,   585,   586,
     587,   587,   588,   588,   589,   589,   589,   590,   591,   591,
     592,   592,   593,   593,   594,   594,   594,   595,   596,   597,
     598,   598,   599,   600,   601,   601,   602,   602,   603,   603,
     604,   605,   605,   605,   605,   605,   606,   606,   607,   607,
     608,   608,   609,   610,   610,   611,   611,   612,   612,   613,
     614,   614,   614,   614,   614,   615,   615,   616,   616,   617,
     617,   618,   618,   618,   619,   619,   620,   620,   621,   621,
     622,   622,   623,   624,   624,   624,   625,   625,   626,   626,
     627,   627,   627,   628,   628,   629,   629,   630,   631,   632,
     632,   633,   634,   635,   636,   636,   637,   638,   639,   639,
     640,   640,   640,   641,   641,   642,   643,   643,   644,   644,
     645,   645,   646,   647,   647,   648,   648,   649,   650,   650,
     651,   651,   651,   652,   653,   653,   654,   654,   654,   654,
     654,   655,   655,   655,   655,   655,   655,   656,   656,   656,
     657,   657,   657,   657,   657,   657,   657,   657,   657,   657,
     657,   658,   658,   658,   658,   658,   658,   658,   658,   659,
     659,   659,   659,   659,   659,   659,   659,   659,   659,   659,
     659,   659,   659,   659,   659,   660,   660,   661,   661,   662,
     662,   663,   663,   663,   663,   664,   664,   665,   665,   666,
     666,   666,   666,   667,   668,   669,   669,   670,   670,   670,
     671,   671,   671,   672,   672,   673,   674,   674,   674,   674,
     674,   674,   674,   674,   674,   674,   674,   674,   674,   674,
     674,   674,   674,   674,   674,   674,   674,   674,   674,   674,
     674,   674,   674,   674,   675,   675,   675,   676,   677,   677,
     678,   678,   679,   679,   679,   679,   679,   680,   680,   680,
     680,   680,   681,   682,   682,   683,   684,   684,   684,   684,
     684,   685,   685,   686,   686,   687,   688,   689,   690,   691,
     691,   692,   693,   693,   693,   694,   694,   694,   694,   694,
     694,   694,   694,   694,   694,   694,   695,   695,   696,   697,
     697,   697,   698,   699,   699,   700,   701,   701,   701,   701,
     702,   703,   703,   704,   704,   704,   704,   705,   705,   705,
     706,   706,   707,   708,   708,   709,   709,   709,   710,   710,
     711,   711,   712,   712,   713,   713,   714,   714,   715,   716,
     717,   718,   718,   719,   719,   719,   719,   719,   719,   719,
     719,   720,   720,   721,   721,   722,   723,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,   746,
     747,   748,   748,   749,   749,   749,   749,   749,   749,   749,
     749,   749,   749,   749,   749,   749,   749,   749,   749,   749,
     749,   749,   749,   749,   749,   749,   749,   749,   749,   749,
     749,   749,   749
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     8,     8,     5,     1,     0,     1,
       2,     1,     1,     3,     1,     1,     1,     1,     2,     2,
       3,     0,     3,     0,     1,     8,     8,     5,     3,     0,
       3,     0,     1,     1,     1,     3,     3,     3,     2,     2,
       2,     2,     1,     3,     1,     2,     2,     1,     3,     1,
       1,     3,     1,     2,     2,     2,     3,     8,     1,     1,
       5,     0,     1,     3,     1,     3,     3,     0,     2,     2,
       1,     1,     3,     3,     2,     0,     2,     2,     1,     3,
       9,     1,     1,     2,     2,     7,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     4,     2,     2,     2,     2,     2,     2,     2,     1,
       0,     1,     1,     6,     6,     1,     1,     0,     0,     1,
       0,     4,     0,     1,     1,     2,     8,     1,     0,     3,
       0,     0,     1,     1,     2,     1,     1,     1,     2,     6,
       1,     1,     3,     0,     1,     1,     0,     1,     1,     2,
       3,     4,     2,     2,     3,     0,     1,     1,     2,     1,
       4,     3,     3,     1,     0,     1,     1,     2,     3,     4,
       0,     1,     2,     0,     1,     1,     5,     5,     3,     2,
       0,     1,     3,     1,     1,     6,     3,     2,     6,     2,
       1,     2,     2,     2,     0,     1,     1,     2,     0,     2,
       0,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       2,     2,     2,     5,     1,     2,     3,     2,     2,     0,
       1,     1,     1,     1,     3,     4,     8,     4,     0,     6,
       1,     1,     2,     2,     0,     3,     3,     1,     2,     2,
       2,     8,     8,     8,     8,     3,     0,     4,     0,     1,
       3,     4,     1,     3,     2,     3,     3,     1,     1,     0,
       1,     2,     4,     1,     1,     3,     1,     0,     2,     1,
       6,     1,     1,     3,     1,     2,     0,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     1,     1,     2,
       3,     1,     3,     8,     9,     5,     8,     6,     3,     0,
       3,     2,     1,     1,     3,     3,     7,     2,     0,     1,
       1,     2,     3,     0,     1,     2,     4,     1,     3,     2,
       2,     2,     1,     1,     1,     1,     2,     2,     5,     0,
       5,     1,     3,     0,     2,     4,     0,     7,     3,     0,
       1,     3,     3,     7,     7,     0,     0,     0,     0,     3,
       0,     9,     9,     1,     1,     0,     2,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     5,     5,
       5,     5,     5,     5,     2,     0,     4,     2,     2,     3,
       2,     2,     3,     3,     2,     2,     3,     3,     6,     1,
       2,     4,     3,     2,     2,     2,     2,     3,     1,     3,
       3,     3,     2,     2,     4,     4,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     1,     1,     0,     2,     2,     4,
       4,     5,     3,     2,     0,     2,     0,     0,     1,     2,
       2,     4,     3,     2,     2,     7,     1,     0,     1,     0,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       2,     2,     2,     2,     1,     1,     1,     1,     4,     5,
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
       2,     4,     0,     8,     5,     1,     1,     7,     4,     1,
       1,     8,     5,     4,     1,     3,     3,     1,     7,     7,
       5,     3,     0,     1,     1,     1,     2,     0,     1,     1,
       1,     0,     3,     1,     3,     1,     0,     3,     1,     3,
       1,     3,     3,     1,     1,     3,     1,     2,     1,     3,
       3,     1,     2,     3,     3,     3,     3,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     1,     1,     5,     6,     5,
       6,     3,     4,     5,     6,     3,     4,     3,     4,     3,
       4,     4,     5,     4,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     4,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     1,     1,     6,     1,
       1,     1,     4,     4,     4,     4,     1,     1,     4,     4,
       7,     2,     0,     7,     6,     6,     4,     1,     1,     1,
       4,     3,     6,     1,     1,     6,     8,     6,     1,     1,
       4,     6,     4,     5,     3,     5,     4,     5,     1,     1,
       1,     4,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   604,   130,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   604,   692,     0,     0,     0,     0,     0,
       0,     1,    11,    17,     8,     7,    15,    16,    13,    14,
       3,    10,    21,   586,    19,   591,   592,   593,     6,    18,
     585,   587,     5,    20,   672,   779,   680,   683,    12,     9,
     795,   796,    22,   799,   800,     4,   497,     0,     0,     0,
       0,     0,     0,     0,     0,   407,   603,   606,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   129,     0,     0,
       0,   103,   785,   975,     0,    23,   973,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,     0,   369,    29,    37,    34,
      36,   821,    35,   821,  1081,  1083,  1084,  1085,  1086,  1087,
    1088,  1108,  1109,  1089,  1091,  1090,  1096,  1092,  1097,  1093,
    1095,  1098,  1094,  1099,  1100,  1101,  1103,  1105,  1102,  1106,
    1107,  1111,  1110,  1112,  1104,     0,    31,     0,    32,     0,
      67,    69,  1073,  1082,     0,     0,     0,     0,     0,     0,
       0,   602,     0,     0,  1052,   686,   691,     0,     0,   611,
       0,   738,   740,  1075,     0,     0,     0,     0,   116,   594,
    1080,     0,     0,     2,     0,   676,     0,  1054,   792,     0,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   467,   461,   462,   460,   463,   464,   465,   466,   468,
     470,   473,   469,   471,   472,   474,   475,   476,   477,   478,
       0,   437,   438,   408,     0,  1064,     0,   414,     0,  1068,
     411,   266,  1072,     0,   740,   410,   385,  1076,   415,     0,
    1067,   608,   601,   971,     0,   112,   166,   165,   113,   148,
     215,   833,  1060,   104,     0,   110,   160,     0,   106,   266,
     114,   977,   137,   107,   200,   108,     0,   109,   821,  1079,
     115,   161,   111,   783,   784,     0,   976,   974,     0,    75,
       0,  1065,   756,   518,  1063,   511,     0,   517,   522,   512,
     513,   520,   514,   515,   516,   521,   523,   363,     0,   268,
      30,     0,    39,   820,    38,    28,     0,     0,     0,     0,
       0,   826,     0,     0,     0,     0,     0,     0,   600,   981,
       0,   962,   685,   685,   686,   698,   696,  1051,   699,     0,
     700,   690,   686,     0,     0,     0,     0,     0,     0,   627,
     620,   609,   610,   612,   614,   615,   616,   623,   624,   617,
       0,   739,  1074,   120,     0,   117,   266,   118,   200,   119,
     821,   597,     0,   650,   651,   658,     0,   657,   659,   661,
     666,   654,   652,   655,   653,   662,   665,   660,   664,     0,
       0,     0,   674,     0,   670,     0,     0,  1053,     0,     0,
     677,     0,     0,     0,     0,   412,   498,   377,     0,   377,
       0,   218,   413,   419,   128,  1056,   416,   494,   487,   488,
       0,   268,   239,   479,     0,   409,   428,     0,   383,   384,
     507,     0,   607,   605,   972,   163,   163,     0,     0,     0,
     175,   167,   168,   147,     0,   125,     0,     0,     0,   121,
     268,   136,   135,   138,     0,     0,     0,     0,     0,    76,
     218,     0,     0,   758,   798,   519,     0,     0,     0,     0,
     939,   968,     0,  1055,     0,   941,     0,     0,   969,     0,
     963,   954,   953,   952,  1081,     0,     0,     0,   944,   945,
     946,   955,   956,   964,   965,  1091,   966,   967,     0,  1096,
    1092,   970,  1106,  1111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   921,   366,   916,   830,   948,   942,
     917,   361,   957,   919,   920,   936,   937,   938,   958,   918,
     982,   983,   996,   997,   999,  1000,  1001,   984,  1006,  1007,
     925,   922,   923,  1023,  1024,  1028,  1029,   924,   943,     0,
       0,  1060,     0,     0,     0,   370,   216,     0,     0,     0,
     823,    27,     0,    33,     0,     0,    43,    70,    72,  1077,
      68,     0,     0,     0,   825,     0,    48,    50,    28,     0,
       0,   599,     0,     0,     0,   695,   693,     0,   702,     0,
     701,   703,   708,   689,     0,   646,     0,   621,   632,   618,
     632,   619,   633,   639,   643,   644,   628,   613,   756,   804,
     834,     0,     0,   126,   268,     0,     0,   596,   595,   663,
     656,     0,     0,  1071,     0,     0,   671,   675,   778,   780,
     785,   682,   681,   790,   183,     0,   500,   144,     0,   503,
       0,   504,   499,     0,   423,     0,   425,   424,     0,   426,
       0,   420,     0,   496,     0,     0,   269,     0,     0,   432,
       0,   433,   377,   480,     0,   486,     0,     0,     0,   483,
    1060,   388,   387,   506,     0,   405,     0,   164,     0,     0,
     173,     0,   172,     0,     0,   162,   176,   177,   179,   169,
     555,   567,   542,   577,   545,   568,   578,     0,   584,   546,
       0,   574,     0,   569,   581,   541,   564,   543,   544,   539,
     377,   524,   525,   526,   534,   540,   527,   536,   557,     0,
     562,   559,   537,   574,   538,   124,     0,   122,   266,   123,
       0,     0,   139,     0,     0,   199,   239,   385,   375,     0,
     218,     0,    82,    84,     0,     0,   102,   101,  1057,     0,
       0,     0,     0,   907,   908,   909,     0,   755,   838,   847,
     850,   856,   851,   852,   853,   854,   858,   859,   857,   860,
     855,   836,     0,   764,   779,  1052,     0,  1052,   959,     0,
       0,  1052,  1052,  1052,     0,   960,   961,     0,     0,  1039,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   927,   926,     0,   779,   948,     0,   331,  1078,
       0,   360,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,   534,   557,     0,     0,   368,     0,   279,
       0,   822,     0,     0,    44,    73,     0,     0,    26,   812,
       0,   828,     0,   794,   812,     0,     0,    47,   598,     0,
     685,     0,     0,   756,     0,   707,     0,   589,     0,   588,
     979,     0,   622,     0,   630,   626,   625,     0,   636,     0,
     641,   629,     0,   758,   802,     0,     0,     0,   239,   375,
     669,   647,   668,     0,     0,     0,   673,     0,   789,     0,
       0,   145,   184,   502,   377,   422,     0,   377,   378,   427,
     493,     0,   492,   375,     0,   265,   375,   238,  1061,   210,
     207,     0,   150,   377,   524,   375,     0,     0,     0,   237,
     240,   241,   242,   243,     0,   431,   485,   484,   430,   429,
       0,     0,     0,   391,   389,   390,   386,     0,   377,   490,
       0,   489,   193,   170,   174,     0,     0,   178,     0,     0,
     552,   565,   566,   582,     0,   579,   584,     0,   572,   570,
     571,   150,     0,     0,     0,   535,     0,     0,     0,     0,
     573,   127,   268,     0,   375,   140,   198,     0,   201,   203,
     204,     0,     0,     0,     0,   218,   218,    79,     0,    87,
      78,     0,     0,     0,     0,   849,   837,     0,     0,   910,
     911,   912,     0,   841,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   762,   763,   757,     0,   792,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1043,  1044,  1045,  1046,  1047,  1048,  1049,
    1050,     0,     0,     0,     0,  1034,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1017,  1019,
    1018,     0,     0,   935,   792,   934,   362,     0,   332,   364,
     333,     0,     0,   930,  1059,   929,   928,   931,   932,   933,
     949,  1021,     0,   940,   832,   831,   372,   371,     0,   272,
     287,     0,   278,   280,   824,     0,     0,     0,    71,     0,
     817,     0,   827,     0,   817,     0,     0,   694,   697,     0,
     709,   710,   713,   715,   712,   724,   725,   726,   714,   737,
       0,   749,   704,   706,  1069,   590,   980,   631,   640,   635,
     634,     0,   645,   805,   779,   806,   835,   375,     0,     0,
     648,   667,     0,   781,   687,   782,   791,   501,   163,   190,
     182,   185,   186,     0,     0,     0,   421,   495,   375,   375,
     378,   277,   270,   279,   209,   377,   377,   378,   211,   206,
     212,     0,     0,   248,     0,   435,   378,   377,   375,   434,
       0,     0,     0,   404,   509,   491,   194,   195,   171,     0,
     181,   553,   554,   978,     0,     0,     0,   557,     0,   580,
       0,     0,   378,     0,   530,   532,     0,     0,     0,     0,
       0,     0,     0,   385,   279,   163,   142,   239,   196,   405,
     380,     0,    83,     0,   218,     0,    91,     0,    85,     0,
       0,    90,    86,     0,   803,  1062,     0,     0,   842,     0,
       0,   848,     0,     0,   840,   839,     0,   897,     0,   886,
     885,     0,   864,     0,   895,   913,     0,   905,     0,     0,
       0,   865,   891,     0,     0,   868,     0,     0,     0,     0,
       0,     0,     0,   866,     0,     0,   867,     0,   899,     0,
       0,   861,     0,     0,   862,     0,     0,   863,   761,   764,
     797,     0,     0,   218,     0,   985,     0,     0,  1008,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1040,     0,
       0,  1038,     0,     0,  1030,     0,     0,     0,     0,  1041,
    1002,  1003,  1004,  1009,  1005,     0,  1016,     0,     0,   679,
       0,     0,     0,   947,  1020,     0,   267,   286,     0,   338,
     367,   281,     0,     0,     0,     0,     0,  1087,    41,    52,
      54,    53,    62,    64,    41,    42,     0,     0,     0,   809,
     829,     0,   808,     0,     0,     0,   745,   712,     0,   747,
     741,   747,   745,   747,     0,     0,     0,   708,     0,   754,
     637,   638,   642,   792,   279,   197,   380,   649,     0,     0,
     191,     0,   187,     0,     0,     0,   417,   226,   221,   950,
     222,   223,   224,   225,     0,     0,   271,     0,     0,     0,
     227,     0,   248,   785,   244,     0,   436,   481,   482,   396,
     397,   393,   392,   395,   394,   377,   508,   378,   180,  1058,
       0,     0,   548,     0,   547,   583,     0,   575,   151,     0,
     528,     0,   556,   557,   563,   561,     0,   558,   375,     0,
       0,     0,   134,   143,   202,   377,     0,   376,   105,     0,
     132,   131,    94,     0,     0,    95,     0,    88,    89,     0,
     903,   904,   844,   843,   846,   845,     0,     0,     0,     0,
     779,     0,     0,     0,   906,     0,     0,     0,     0,     0,
       0,   898,   896,   892,     0,   900,     0,     0,     0,     0,
     901,     0,     0,     0,     0,     0,     0,   768,     0,   765,
       0,   769,   991,   990,     0,     0,   987,   986,     0,   995,
     994,   993,   992,   989,   988,     0,  1012,  1036,  1035,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   365,
     335,   334,   273,     0,     0,   283,   284,     0,  1060,   321,
       0,     0,     0,     0,     0,     0,  1089,  1094,     0,     0,
     359,   297,   292,     0,   338,   291,     0,     0,     0,   317,
     318,     0,   315,     0,     0,   316,     0,     0,     0,   353,
     355,   354,     0,     0,     0,     0,     0,     0,     0,   807,
       0,  1060,    66,    58,    59,    65,    60,    61,     0,    24,
       0,     0,    25,     0,   813,   814,   815,   811,   816,   819,
     818,   793,    45,    46,   708,   716,   711,   746,   744,   742,
       0,   743,     0,     0,     0,     0,     0,     0,   758,   801,
       0,   376,   786,   787,   688,   193,   192,   188,     0,   378,
     951,   275,   276,   378,   378,   149,   214,   228,   229,   377,
       0,   245,     0,     0,     0,     0,     0,     0,     0,     0,
     279,   505,   550,     0,   551,     0,     0,   214,   152,   153,
     155,   156,   157,   531,   533,   529,   560,   279,   405,   378,
     193,     0,     0,   373,   377,    92,     0,     0,    98,    93,
     557,     0,   889,     0,     0,   915,   792,   914,   887,     0,
       0,     0,   893,     0,     0,     0,     0,   902,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   759,   764,
       0,   771,     0,   767,   770,  1022,   217,  1042,   998,     0,
       0,  1037,  1032,  1031,     0,  1025,  1027,     0,  1014,  1015,
       0,   220,   282,     0,   319,     0,   314,     0,   329,   313,
     341,  1070,   356,   357,     0,   293,     0,   298,   295,   344,
     308,   309,   305,   306,   310,   303,     0,     0,   312,   311,
     304,     0,   300,   302,   301,   299,   307,   337,     0,    55,
      56,    57,    63,     0,   719,     0,   727,   745,   736,   734,
     748,   750,   752,   753,   684,   378,   374,   788,   189,   378,
     159,   264,     0,     0,   205,   230,     0,     0,     0,   248,
     231,   234,   821,     0,   378,   402,   403,   399,   398,   401,
     400,     0,   549,   576,   158,   146,   154,     0,   377,   261,
     141,   378,   379,     0,     0,    96,    97,    80,     0,   872,
     880,   678,   888,   873,   881,   876,   884,   890,   894,   874,
     882,   875,   883,   869,   877,   870,   878,   871,   879,   766,
       0,   777,  1011,  1010,  1033,     0,  1013,     0,     0,   289,
     285,     0,   320,   322,     0,   328,     0,   352,     0,     0,
       0,     0,   347,   345,     0,     0,     0,   330,    40,   810,
     721,   718,   728,     0,     0,   729,   730,   731,     0,   262,
     418,   208,   213,   232,   248,   821,   235,   254,   247,   249,
     406,   378,     0,   381,   378,    77,    99,   100,     0,   774,
       0,   773,  1026,   219,   288,     0,     0,     0,     0,   351,
     350,  1066,   349,   338,     0,     0,     0,     0,     0,   717,
     732,     0,   751,   236,   254,     0,   248,   250,   251,   263,
     378,     0,   772,     0,     0,     0,   325,   338,   358,   346,
     348,     0,   343,     0,     0,   722,     0,   248,     0,     0,
     246,     0,   253,     0,   252,   382,   133,   775,     0,   290,
     327,     0,     0,     0,   338,     0,   720,   733,   233,     0,
       0,   257,   256,   255,   776,   338,     0,     0,   338,   336,
     723,   260,   258,   259,   326,   338,   342,   323,   324,     0,
       0,     0
};

static const short yydefgoto[] =
{
    2019,    21,    22,   562,   145,   146,   147,   148,  1609,   838,
     833,    23,   159,   160,  1358,  1359,  1360,  1361,  1362,   149,
     150,   566,   567,    24,    89,   459,   989,   741,   742,   743,
    1242,  1235,  1236,  1699,   289,   745,    25,    81,    26,   178,
      27,   449,   263,   363,   725,   233,    82,  1468,   270,   453,
     733,  1226,  1462,   636,   258,   444,  1177,  1677,  1678,  1679,
    1680,  1681,  1682,   265,   280,   255,   678,   256,   440,   441,
     442,   685,   686,   687,   637,   638,  1160,  1161,  1162,  1401,
    1198,   273,   367,   455,   977,   978,   979,   910,   911,   912,
    1814,   660,   552,  1524,   744,  1879,  1406,  1656,  1657,  1658,
    1820,   980,   662,   919,   920,   921,   922,  1424,   923,  1956,
    1957,  1958,  2002,   268,   365,   727,   240,   421,   558,   655,
     656,  1098,   553,  1170,  1101,  1102,  1103,  1554,  1555,  1348,
    1880,  1556,  1571,  1572,  1573,  1574,  1575,  1576,  1577,  1578,
    1579,  1580,  1581,  1582,  1583,  1885,  1584,   514,  1079,  1585,
    1586,  1587,  1993,  1768,  1769,  1891,  1892,  1588,  1589,  1590,
    1591,  1764,  1592,   515,   811,    29,   309,   554,   555,   277,
     369,   982,  1693,   645,  1166,  1467,   275,   729,   430,   673,
     674,   936,   938,  1436,    30,    65,   898,   895,   412,   413,
     425,   426,   230,   231,   427,  1186,   668,   928,   237,   248,
     416,   653,   902,   189,   405,   406,   245,   675,  1437,    31,
     104,   710,   711,   712,  1213,  1214,   713,   823,   715,   716,
    1207,   950,   965,   717,   824,   719,   720,   721,   722,   958,
     723,   724,   955,    32,    33,    34,    35,    36,   618,    37,
     582,    38,    39,    67,   252,   433,    40,   351,   352,   353,
     354,   355,   356,   357,   358,   865,   359,  1141,  1392,   602,
     603,   870,   604,    41,    42,   389,   390,   391,   885,   881,
    1593,   185,   626,   394,    44,  1489,   804,    45,    46,    47,
     585,   341,  1398,  1807,   164,   165,   166,   339,   589,   590,
     591,   856,   853,  1120,  1121,  1122,  1123,  1900,  1949,  1974,
    1124,  1125,  1126,  1127,  1905,  1906,  1907,  1128,  1387,   170,
     171,  1385,  1628,  1389,  1800,  1801,  1638,   463,   774,  1517,
    1027,  1518,  1519,  1520,  1733,  1928,  1931,   188,   628,   629,
     285,  1644,  1155,   400,  1594,  1595,    50,    51,  1596,    53,
      54,   464,   608,  1597,  1598,    55,  1110,  1613,  1614,  1369,
    1618,   312,   313,   559,   573,   574,   840,   516,   517,   611,
     757,  1002,   758,   759,   760,   761,  1261,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,  1004,  1264,  1265,
     772,   519,   520,   521,  1408,   522,   523,   524,   525,   526,
     527,   528,  1616,    86,   287,   272,  1205,  1215,   860,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,  1740,
     539,  1072,   540,   541,   542,   543,   544,   545,  1058,   546,
     790,  1322,   791,  1319,   547,  1051,   340,   398,   548,   549,
     747,  1438,  1083,   261,   907,  1244,   293,   234,   290,  1940,
     266,  1929,  1133,  1600,   622,  1129,   151,   550,   244,   246,
    1363,   808,   278,   179,   551,   153
};

static const short yypact[] =
{
    2368,  1183,    54,  1880,   279,   544,   204,  3405,   123,  7783,
     488,   236,   996,    54,   680,   328,  5938,   489,  5938,    38,
     185,   193,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   452,   110,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  8517,  5938,   463,
    5938,  5938,  5938,  5938,  5938,-32768,-32768,   370,   761,  5938,
    5938,  5938,   645,  5938,   354,  5938,  5938,-32768,  5938,  5938,
    5938,-32768,   615,-32768,   469,-32768,-32768,   684,  5938,-32768,
    5938,  5938,  5938,   729,  5938,  5938,  5938,  5938,   354,  5938,
    5938,  5938,  5938,  5938,-32768,  5938,   531,   660,-32768,-32768,
  -32768,   566,-32768,   566,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   685,   564,   737,-32768,    46,
  -32768,-32768,-32768,-32768,  5938,  5938,  5938,   752,   772,  1068,
    5938,   694,   335,   402,   715,   668,-32768,  5938,   845,   504,
     736,-32768,  5938,-32768,  5938,  5938,  5938,  5938,-32768,-32768,
  -32768,  5938,  1816,-32768,   757,   774,   912,   725,   754,   795,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     514,-32768,-32768,-32768,   761,-32768,   755,-32768,   228,-32768,
  -32768,   701,-32768,   244,-32768,-32768,   588,-32768,-32768,   743,
  -32768,   861,-32768,-32768,   825,-32768,   831,-32768,-32768,  1019,
  -32768,-32768,-32768,-32768,   761,-32768,-32768,   188,-32768,   701,
  -32768,-32768,   269,-32768,   982,-32768,   993,-32768,   566,-32768,
  -32768,-32768,-32768,-32768,-32768,   989,-32768,-32768,   755,-32768,
     994,-32768,   888,-32768,-32768,-32768,   755,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  7456,  5938,   930,
  -32768,  5938,-32768,-32768,-32768,   916,   804,   948,  5026,  5938,
    1012,   808,  1041,  1044,  1047,   991,   997,   138,   898,-32768,
    7675,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  5845,
  -32768,-32768,-32768,   972,  5938,  1048,   961,   391,  5938,   980,
  -32768,-32768,   504,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    5938,-32768,-32768,-32768,   761,-32768,   701,-32768,   982,-32768,
     566,  1035,  1004,-32768,-32768,-32768,  1096,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1080,
    5938,  5938,  1045,   919,-32768,  7675,  1016,-32768,  1016,  7675,
  -32768,    96,   918,   920,   925,   795,-32768,  1143,   563,-32768,
    5938,-32768,   514,-32768,-32768,-32768,-32768,  1128,-32768,-32768,
    5938,   930,  3023,   977,  3293,   900,-32768,  8517,-32768,-32768,
     253,   995,-32768,-32768,-32768,   910,   910,   761,  1077,   761,
     133,   831,-32768,-32768,  2475,-32768,  5938,  5938,  5938,-32768,
     930,-32768,-32768,  1163,   352,   906,  5938,  1185,  5938,-32768,
      53,  5500,  4704,  1062,-32768,-32768,   924,   934,   937,  1013,
  -32768,-32768,   938,-32768,   940,-32768,   941,   942,-32768,   943,
  -32768,-32768,-32768,-32768,   950,   952,  1042,  1054,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   953,-32768,-32768,  6064,   956,
     968,-32768,   973,  1164,   978,   979,   981,   986,   987,   998,
    7675,  7675,  6283,  5938,-32768,  1127,   974,-32768,  1417,-32768,
  -32768,   992,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   999,
    1000,  1001,  2475,  1002,   586,-32768,-32768,  1007,  1271,   622,
  -32768,-32768,  5938,-32768,  5938,  5938,    29,-32768,-32768,-32768,
  -32768,  5938,  5938,   512,-32768,  5938,-32768,-32768,   916,  1146,
    5026,-32768,  5938,  6283,   227,-32768,-32768,  7675,-32768,  1221,
    1009,-32768,   182,-32768,   494,-32768,   658,-32768,   359,-32768,
     359,-32768,  1023,-32768,   158,-32768,  1122,-32768,   240,-32768,
  -32768,  1030,  1024,-32768,   930,  1027,  1301,-32768,-32768,-32768,
  -32768,   425,  1232,-32768,  1034,  5938,-32768,-32768,  1036,-32768,
     361,-32768,-32768,  1390,-32768,  1237,  1240,-32768,   761,-32768,
    1241,-32768,-32768,  1310,-32768,  1312,-32768,-32768,  1292,-32768,
    2475,-32768,   761,  1227,  2475,   655,-32768,  1327,  5938,-32768,
    8350,-32768,   386,-32768,  5938,   739,   244,  1167,  1134,  1213,
    1176,-32768,-32768,-32768,   189,  1220,   520,-32768,  1161,   354,
  -32768,   761,-32768,  1345,  1280,-32768,   133,-32768,-32768,-32768,
      34,  1187,-32768,-32768,-32768,  1188,-32768,  1238,  1088,-32768,
    1286,  1094,   435,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1092,-32768,-32768,-32768,   156,  1099,
    1100,    19,-32768,  1094,-32768,-32768,   761,-32768,   701,-32768,
    1322,  1378,-32768,   761,   761,-32768,  3023,   588,-32768,  1269,
  -32768,  1244,  1115,-32768,  2364,  1274,-32768,-32768,-32768,  1283,
    1118,  4923,  1124,  1116,  1386,  1413,  4448,   224,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  2366,   593,  1289,   744,  7675,   143,-32768,  5938,
    7675,   768,   787,   856,  1361,-32768,-32768,  7675,  5142,  1417,
     306,  1223,  7675,  7675,  5142,  1355,  7675,  7675,  7675,  7675,
    7675,  5361,   171,   171,  1131,   110,   430,   722,-32768,-32768,
    4104,-32768,  7675,  5938,  7675,  7675,  7675,  7675,  7675,  7675,
    6502,  2744,-32768,-32768,  1399,  1132,  5938,-32768,  5938,  1382,
    5938,-32768,  1252,  1257,-32768,-32768,  1230,  5026,-32768,  1374,
     733,-32768,  1148,-32768,  1374,  5938,  5938,  1151,-32768,   430,
  -32768,   470,  4797,  1248,  7675,-32768,  5938,-32768,   536,-32768,
  -32768,   164,-32768,  1261,-32768,-32768,-32768,  5938,   527,  5938,
  -32768,-32768,  5938,  1062,-32768,  7675,  5938,  1433,  3023,-32768,
  -32768,-32768,-32768,   425,  5938,  1366,  1165,  7675,  1210,  7675,
     761,-32768,   539,-32768,-32768,-32768,  1175,-32768,-32768,-32768,
  -32768,   761,-32768,   141,  5938,-32768,-32768,-32768,-32768,  1441,
  -32768,  1177,  1409,-32768,  1436,-32768,  1379,  1380,   566,-32768,
  -32768,-32768,-32768,-32768,  1445,-32768,-32768,-32768,-32768,-32768,
    5938,  8448,   354,  1343,  1348,  1349,-32768,   354,-32768,-32768,
     619,-32768,   862,-32768,-32768,  1314,   761,-32768,  5500,   261,
    1321,-32768,-32768,-32768,   354,-32768,  1088,   285,-32768,-32768,
  -32768,  1409,   285,  1320,   285,-32768,   354,   354,  1192,   354,
  -32768,-32768,   930,  5938,-32768,  1383,-32768,   734,-32768,-32768,
  -32768,   253,  1016,  5938,   750,    60,-32768,-32768,  1195,   443,
  -32768,  5500,  5938,  1016,  5626,-32768,-32768,  1016,  4923,-32768,
  -32768,-32768,   124,-32768,   344,  1795,  5142,  5142,  7675,  7675,
    2097,  1197,   611,  2572,  7675,  2870,   596,  3149,  3419,  6762,
    3689,  3959,  4229,-32768,-32768,  1384,-32768,  1207,   754,  7675,
    7675,   207,  7675,  1212,  7675,  1225,   484,  7675,  7675,  7675,
    7675,  7675,  7675,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1435,  1226,   173,  7675,-32768,  5142,  7675,   449,   799,
     922,   119,  5938,   582,   635,   652,   674,   687,-32768,-32768,
  -32768,   251,  7018,-32768,   754,-32768,-32768,  4578,-32768,  1229,
  -32768,  1209,   -15,-32768,-32768,  1490,   171,   171,   670,   670,
    1417,-32768,   751,-32768,-32768,-32768,-32768,-32768,   756,-32768,
    1335,  1505,  1382,-32768,-32768,  3563,  3563,  1402,-32768,   145,
    1416,  5938,-32768,  7675,  1416,  1461,  1464,-32768,-32768,  3833,
    1253,   577,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1243,
    5142,  1469,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   513,-32768,-32768,  1289,  1417,-32768,-32768,   760,  1016,
  -32768,-32768,   425,-32768,-32768,-32768,  1417,-32768,   910,  1522,
  -32768,   539,-32768,  1256,  5142,  2983,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1382,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   566,   566,  1302,  1260,-32768,-32768,-32768,-32768,-32768,
     280,   136,   497,-32768,  1534,-32768,-32768,-32768,-32768,  5938,
  -32768,-32768,-32768,-32768,   279,   763,  1388,  1399,  1263,-32768,
    1161,   783,-32768,   594,-32768,  1266,  5938,   597,  1264,  1268,
     354,  1273,  1539,   588,  1382,   910,  1240,  3023,-32768,  1220,
    1365,  1910,-32768,   354,  1429,  1511,-32768,  2364,-32768,   354,
     543,-32768,-32768,  1515,-32768,-32768,  1281,  1291,-32768,  5142,
    5142,-32768,  5142,  5142,-32768,  1560,   190,  1417,  1298,-32768,
  -32768,  1299,  1417,  6283,-32768,-32768,  1524,-32768,   191,  1303,
    1304,  1417,  1449,  1309,  1313,  1417,  7675,  7675,  1197,  7675,
    7237,  1317,  1318,  1417,  1319,  1324,  1417,  7675,  1417,  1325,
    1326,  1417,  1328,  1329,  1417,  1331,  1332,  1417,-32768,  4332,
  -32768,   697,   713,-32768,   775,-32768,   833,  7675,-32768,   886,
     963,   975,   988,  1022,  1037,  7675,  7675,  7675,  1417,  1546,
     175,  1417,  1426,  7675,-32768,  7675,  7675,  7675,  7675,-32768,
  -32768,-32768,-32768,-32768,-32768,  7675,-32768,  7675,  1455,-32768,
     789,  4578,  5719,-32768,-32768,  5938,-32768,-32768,  5938,  8024,
  -32768,-32768,  5938,  5938,  5938,  5938,  5938,  5938,  1415,  1323,
  -32768,  1334,-32768,-32768,  1415,-32768,    15,    15,    31,-32768,
  -32768,   813,-32768,  3563,  3563,  1333,   217,  1339,  4797,  1481,
  -32768,  1481,   662,  1481,  1537,  1543,  7675,  1307,  1610,  1558,
  -32768,-32768,-32768,   754,  1382,-32768,  1365,-32768,   610,  1161,
    1506,  1161,-32768,  5142,   128,   682,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  2983,  2983,-32768,  1505,  7675,  2983,
     585,  1492,  1302,   615,-32768,  5142,-32768,-32768,-32768,  1513,
    1517,  1527,  1528,  1529,  1530,-32768,-32768,-32768,-32768,-32768,
    1352,   279,-32768,  1451,-32768,-32768,   285,-32768,   412,   285,
  -32768,   285,-32768,  1399,-32768,-32768,  1368,-32768,-32768,   253,
    1505,  1161,-32768,  1240,-32768,-32768,  1627,-32768,-32768,  1371,
  -32768,  1165,-32768,  1372,   761,   140,  1373,-32768,-32768,   761,
  -32768,-32768,-32768,  1560,-32768,  1560,  7675,  1016,  1016,  1375,
     110,   823,  7675,  1596,-32768,  1016,  1016,  7675,  1016,  1016,
     201,  1417,-32768,  1463,  7675,  1417,  1016,  1016,  1016,  1016,
    1417,  1016,  1016,  1016,  1016,  1016,  1016,-32768,  1387,  1376,
     621,  5938,-32768,-32768,  1391,  2475,-32768,-32768,  1070,-32768,
  -32768,-32768,-32768,-32768,-32768,  1079,  1467,-32768,-32768,  7675,
    7675,  1629,  1512,  1254,   842,  1173,  1300,  1351,  7675,-32768,
  -32768,-32768,-32768,  2475,  1401,-32768,-32768,  1651,  1653,  5938,
      74,  1405,  1419,  1620,  7675,  1422,  1423,  5235,  5938,  5938,
  -32768,-32768,-32768,  1658,  7916,-32768,  1425,  1427,  1431,-32768,
  -32768,  1432,-32768,  1434,  1437,-32768,   200,  1439,  1444,-32768,
  -32768,-32768,  1450,  1622,  1456,  1457,  1458,  1468,  1471,-32768,
    1465,   456,-32768,-32768,-32768,-32768,-32768,-32768,  1641,-32768,
    3563,  3563,-32768,  1540,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  1708,-32768,   577,-32768,-32768,-32768,
    1636,-32768,  4797,  4797,   843,  5938,  7675,  5142,  1062,-32768,
    1505,-32768,-32768,-32768,-32768,   862,-32768,-32768,   132,-32768,
  -32768,-32768,-32768,-32768,  1417,-32768,  1711,   585,-32768,   107,
    5938,-32768,  1661,   135,  1572,  1677,  1578,  1715,  1680,  1717,
    1382,-32768,-32768,  1474,-32768,  1475,  1670,  1711,   412,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1382,  1220,-32768,
     862,  1534,  1642,-32768,-32768,-32768,  1654,    98,-32768,-32768,
    1399,  1656,  1417,  1482,  1483,-32768,   754,-32768,  1417,  7675,
    1485,  1487,  1417,  1488,  1493,  7675,  7675,  1417,  1496,  1499,
    1500,  1501,  1502,  1503,  1507,  1509,  1518,  1519,-32768,  4332,
    1491,-32768,  5938,-32768,-32768,-32768,-32768,-32768,-32768,  7675,
    1523,-32768,-32768,-32768,  7675,-32768,-32768,  7675,-32768,-32768,
    1362,   144,-32768,  1719,  7675,  7675,-32768,  5142,  1417,-32768,
  -32768,-32768,-32768,-32768,  5938,-32768,   225,-32768,  1580,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   150,  1514,-32768,-32768,
  -32768,  4578,-32768,-32768,-32768,-32768,-32768,-32768,  1669,-32768,
  -32768,-32768,-32768,    15,  5938,  4797,-32768,   517,-32768,-32768,
    1508,-32768,  1417,   224,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  1525,  5938,-32768,-32768,  1705,  1714,  5938,  1302,
  -32768,-32768,   566,  5938,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1505,-32768,-32768,-32768,-32768,-32768,  1505,-32768,-32768,
  -32768,-32768,-32768,  7675,   761,-32768,  1601,-32768,   761,-32768,
  -32768,-32768,  1417,-32768,-32768,-32768,-32768,  1417,  1417,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    5938,  1731,  1417,-32768,-32768,  1400,-32768,  2983,  2983,-32768,
  -32768,  1532,  1417,  1566,   147,-32768,  1733,-32768,  5938,  5938,
     279,    91,-32768,-32768,  1570,  1735,  5142,  1229,-32768,-32768,
    1533,-32768,-32768,  5142,  1538,-32768,-32768,-32768,  7675,-32768,
  -32768,-32768,-32768,-32768,  1302,   566,-32768,  1699,-32768,-32768,
  -32768,-32768,  1534,-32768,  1417,-32768,-32768,-32768,  1536,  1542,
    1544,-32768,-32768,-32768,-32768,  1016,  4578,  1659,  4578,-32768,
  -32768,-32768,-32768,  8159,   225,  7675,  4578,   148,  5938,-32768,
     224,  5938,-32768,-32768,  1699,   170,  1302,  1707,  1712,-32768,
  -32768,  1545,-32768,  5938,  5938,  1547,  1229,  8159,  1229,-32768,
  -32768,  1575,   111,  1789,   846,-32768,   849,  1302,   431,   431,
  -32768,  1794,-32768,  1660,-32768,-32768,-32768,-32768,  1551,-32768,
    1792,  4578,  1807,  1798,  8159,  5938,-32768,-32768,-32768,  1645,
     444,-32768,-32768,-32768,-32768,  8159,   106,  5938,  8159,-32768,
  -32768,-32768,-32768,-32768,-32768,  8159,-32768,-32768,-32768,  1852,
    1854,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1277,  1701,-32768,  1704,  1549,   502,-32768,
    1021,-32768,-32768,-32768,  -964,-32768,  -638,-32768,  -634,  1709,
    1553,  1288,  1040,-32768,-32768,-32768,   636,-32768,  1141,   896,
  -32768,-32768,   640,-32768,-32768,   892,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   659,-32768,-32768,   926,-32768,-32768,   208,
  -32768,-32768,  1484,  1809,-32768,-32768,  -380,-32768,-32768,-32768,
    1452,-32768,-32768,  1204,  -408,-32768,-32768,-32768,   731,-32768,
   -1406,-32768,-32768,  1531,  1025,   673,  1479,   983,   675,-32768,
     230,   -57,  -303,-32768,  -377,-32768, -1245,-32768,-32768,   247,
  -32768,  1486, -1287,-32768,-32768,-32768,-32768, -1342,   246,   -45,
     -48,   -41,   -67,-32768,-32768,-32768,-32768,  -144,  -324,-32768,
    1015,-32768,  -733,-32768, -1051,-32768,   818,-32768,-32768,-32768,
  -32768,-32768, -1451, -1062,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -782, -1039,-32768,
  -32768,-32768,-32768,-32768,   153,-32768,   -22,-32768,-32768,-32768,
  -32768,-32768,  1923,-32768,-32768,-32768,-32768,-32768,  1098,-32768,
  -32768,  -718,   284,  -392,  -970,   530,-32768,-32768,  -627,  -869,
  -32768,-32768, -1114, -1563,-32768,-32768,-32768,-32768,-32768,  1520,
  -32768,  1265,  1516,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1535,-32768,-32768,-32768,-32768,
  -32768,  -624,  -506,-32768,   965,   478,  -640,  -422,-32768,-32768,
  -32768,-32768, -1108,-32768,  -419,-32768,-32768,-32768,-32768,  1224,
  -32768,-32768,   985,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1917,-32768,-32768,-32768,-32768,-32768,  1581,
  -32768,-32768,-32768,  1342,-32768,  1344,-32768,-32768,-32768,-32768,
    1083,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -719,
       1,-32768,-32768,-32768,  -496,  1218,-32768,  -481,   740,-32768,
    -232,   482,-32768,-32768,-32768,-32768,  1781,-32768,-32768,-32768,
    1097, -1248,-32768,-32768, -1013, -1453,-32768,-32768,-32768,-32768,
     834,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1862,
     -35,   578,  -238,-32768,-32768,    55,-32768,  -474,  -830,  1191,
  -32768,   232,-32768,   446,-32768,  -959,-32768,  -730,-32768,  1078,
    -593,-32768,-32768,  -914,  1968,  1969,-32768,-32768,  1970,-32768,
  -32768,  1364,-32768,  -297,-32768,-32768,  1129, -1251,-32768,   860,
  -32768,  -107,  -788,  -622,-32768,-32768,-32768,  -781,   -49,  -522,
    -670,-32768,  -594,-32768,-32768,-32768,   105,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  -599,-32768,   698,-32768,
    1215, -1092,-32768,  -454,-32768,   570, -1089,  -152, -1081, -1052,
    -987,   -52,    -2,   -39,-32768,   -80, -1142,  -526,  -149,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768, -1271,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   653,-32768, -1186,-32768,-32768,   309,-32768,  -974,   450,
    1029,   764,   166,  -747,  1330, -1303,-32768,   -66,  1887,-32768,
       8,   -55, -1708,   415,  -282,   -40,   -60,  -164,    -1,   -53,
    -288,-32768,   -77,   -62,    -9,-32768
};


#define	YYLAST		8794


static const short yytable[] =
{
     152,    43,    85,   886,   264,   238,   314,   173,   361,   180,
     332,   335,   259,   333,   336,   172,   257,   648,   301,   281,
     260,   241,   714,   276,   304,   718,   295,   243,  1078,  1080,
     568,   805,   688,   269,   650,   271,   913,   888,  1340,  1350,
     274,   299,   305,  1144,  1028,  1557,   822,   303,   232,   235,
     841,   239,   242,   173,   247,   250,   679,   300,   807,   271,
     262,   235,   250,   609,   242,   307,   173,   247,   859,   279,
     152,   250,   249,  1407,  1095,  1074,  1409,   843,  1599,   291,
    1661,   173,   294,   235,  1410,   291,   250,   239,   242,   172,
     173,   247,   279,   152,   250,  1099,   242,   657,   302,  1444,
     370,   586,   805,   298,   990,   320,  1376,   322,   364,   624,
     981,   306,  1229,  1411,  1300,  1465,  1617,   654,  1053,   371,
     321,  1992,  1417,  1767,  1061,   450,   731,  1006,  1841,  1146,
    1183,  1537,  1249,  1659,   873,   366,  1006,  1541,  1943,  1635,
    1006,   368,  1364,  1006,   899,   262,   173,   262,   903,   337,
     941,   152,   996,  2015,   914,  1006,  1006,   995,   250,  1697,
    1339,  1149,  1003,   362,  1150,   235,   242,   173,   279,  1651,
    1652,   457,   180,  1460,  1655,   343,   683,   963,  1412,  1796,
    1431,  1006,   414,  1006,  1168,  1171,   634,  1877,  1173,  1032,
     598,  1413,   855,  1233,   813,  1845,   612,  1180,  1486,   814,
    1416,   580,   -81,   948,   968,   813,  1894,  1420,  1366,  1715,
     814,    83,   445,   813,  1978,   105,  1426,  1303,   814,   868,
     318,   836,   614,   634,   813,  1816,  1620,  1203,   891,   814,
     813,   418,  1006,   933,  1887,   814,   186,  1493,   714,  1808,
    1975,   718,  1448,   446,  1007,  -705,  1817,    66,   422,  1250,
     813,   423,  1818,  1007,   156,   814,  1224,  1007,   671,   556,
    1007,  1776,   560,   616,   105,  1762,  1763,    90,   672,   934,
     924,   819,  1007,  1007,   813,   805,  1343,   835,   688,   814,
    1888,  1379,   181,   451,  1840,  1494,  1819,  2010,   187,   595,
     877,  1889,   568,   424,  1380,  1078,  1080,    84,  1007,   262,
    1007,  1674,   262,  1599,   419,    14,   969,   182,   331,   569,
     152,   610,   613,   605,  1335,   837,  1432,  1381,   961,  1367,
    1755,   949,  1407,  1407,   331,  1409,  1409,  1407,  1846,   447,
    1409,  1382,   319,  1410,  1410,   239,  1254,  1255,  1410,   173,
     740,   600,  1902,  1640,  1698,  1685,  1317,  1234,  1539,  1007,
    1979,   635,  1252,  1741,  1742,  1653,  1054,  1055,  1082,  1960,
    1429,   649,  1411,  1411,   448,  1626,  1092,  1411,  1383,   935,
    1659,   556,   283,   260,   452,   665,  1794,  1944,   669,  1131,
     726,   623,   623,   106,   813,   680,  1320,   682,   684,   814,
    1890,  1777,  1341,  1421,  1422,   730,   167,  1341,  1689,  1248,
     995,   262,   735,   739,   995,  1328,   284,   728,   612,  1622,
    1623,   262,  1251,   262,  1393,   262,  1649,   598,   670,  1169,
    1809,   737,  1878,  1824,   319,  1187,  1033,  1412,  1412,  1394,
     462,  1211,  1412,  1397,  -377,  1937,  1973,   235,   242,   247,
    1413,  1413,   734,  1470,   869,  1413,   964,   173,   916,   239,
    1414,  1415,   748,   813,   817,   818,   959,  1203,   814,   746,
    1430,  1886,  1240,   815,   816,   817,   818,  1671,  -705,  1253,
    1428,   815,   816,   817,   818,    83,   863,  1916,   183,  1639,
    1384,   329,   815,   816,   817,   818,  1230,  2012,   815,   816,
     817,   818,  1969,   813,  1404,  1056,   168,  1246,   814,  1323,
    1324,  1247,  1163,   169,   809,  1165,   154,   813,   815,   816,
     817,   818,   814,   184,   599,   850,  1990,   251,   407,   880,
     848,  1178,   960,   610,   834,   917,   872,   832,   236,   942,
    1676,   329,   815,   816,   817,   818,   839,  1241,  2013,  1336,
     844,  1433,  1899,  2009,   174,   864,  1194,  1204,  1999,   568,
      83,   253,   254,   173,  2014,   242,   569,  2017,  1874,  1550,
    1551,    84,   173,   408,  2018,   918,   173,  1210,   600,   882,
     155,   569,  1953,   180,  1838,  1143,   560,  1434,  1805,  1482,
    1483,  1379,  1484,  1485,   972,   601,   892,  2000,   345,  1370,
    1688,   428,  1847,   646,  1380,  1095,  1459,   990,   329,   943,
     900,   654,  1151,    87,    88,   813,   647,  -239,  1237,  1471,
     814,   915,  1552,  1276,  1980,   658,   262,  1381,  1117,  1831,
    1797,   346,   330,  1375,   253,   254,   283,  1277,   331,   944,
     175,  1382,  1933,  1934,  2001,  1998,  1837,  1158,  1325,  1903,
     271,  1379,   815,   816,   817,   818,  1081,   347,  1222,   908,
     348,   262,   267,  1396,  1380,   908,  1390,  1266,   813,  1371,
     284,   176,  -745,   814,   429,   286,   349,    14,  1383,  1139,
     409,  1278,  1599,  1096,   971,   813,   177,  1381,  1023,  1810,
     814,   975,   976,  1811,  1812,  1140,   417,   260,   410,   334,
     329,  1382,   350,   813,   842,   331,  1599,   813,   814,  1279,
    1730,  1024,   814,  -239,  2016,  1267,  1159,  1391,   612,  1136,
     813,   815,   816,   817,   818,   814,   329,   857,  1075,  1839,
     813,   337,   411,  1599,  -239,   814,  1379,   262,  1383,  1268,
    -239,   396,   329,  1648,  1599,  1731,   813,  1599,   460,  1380,
    1687,   814,  1897,   939,  1599, -1070,   465,  1732, -1074,   288,
     337,   815,   816,   817,   818,  1663,  1904,  1025,  1118,  1135,
    1706,   338,  1381,  1280,  -239,   815,   816,   817,   818,  1920,
     250,   397,  1308,  1477,   337,  1921,   858,   556,  1399,   556,
    1384,  1104,  1490,  1418,  1419,  1407,  1407,  1035,  1409,  1409,
    1029,  1136,  1851,   337,   296,  1427,  1410,  1410,   813,   401,
     310,   861,   940,   814,  1084,  1478,   834,   315,  1804,  1491,
    1115,   402,   262,  1383,  1037,   329,   587,   262,   308,   262,
     349,   262,   813,   610,   593,  1411,  1411,   814,   569,   260,
    1662,   882,   605,  1039,  1142,  1909,   173,   242,  1157,  1910,
    1384,  1642,  1195,   242,   403,  1461,   404,  1134,   108,  1167,
     316,  1643,  1189,   311,  1919,   556,   813,  1237,   173,   317,
     173,   814,   337,   815,   816,   817,   818,   262,   328,   262,
    1330,  1923,   826,  1544,   827,   623,   323,   481,   482,   483,
    1449,  1185,  1188,  1449,   110,  1450,  1218,  1219,  1453,  1221,
    1412,  1412,   360,   271,  1200,   262,   324,  1966,  1193,  1968,
     163,  1736,  1041,  1413,  1413,   491,   492,  1972,   830,   813,
     831,   162,   163,  1081,   814,  1208,   815,   816,   817,   818,
    1675,   262,   262,  1331,   344,  1684,  1525,   271,   271,   435,
     271,   395,  1634,   815,   816,   817,   818,   392,  1223,   748,
    1332,   904,   926,   905,   927,   813,  1201,   612,  1231,   111,
     814,  1959,  2006,   415,  1961,   815,   816,   817,   818,   112,
     253,   254,  1333,   436,   173,   437,   393,  1803,   815,   816,
     817,   818,  1789,  1791,   173,  1334,  1790,  1792,   815,   816,
     817,   818,   748,  1245,   113,  1522,   813,   438,   420,   746,
    1985,   814,  1196,  1197,   815,   816,   817,   818,   813,  1078,
    1080,  1523,   -49,   814,  1987,  1988,  1490,  1490,   819,   431,
    1076,   813,   439,   399,  1490,  1490,   814,  1490,  1490,  1111,
    1227,  1112,  1228,   432,   434,  1490,  1490,  1490,  1490,   443,
    1490,  1490,  1490,  1490,  1490,  1490,   986,   819,  1232,  1344,
     -49,   454,  1345,  1670,  1346,   813,  1227,  1751,  1395,  1441,
     814,  1442,   -49,   250,   456,   891,   815,   816,   817,   818,
     813,  1136,   610,  1526,  1602,   814,   157,  1605,   458,  1446,
    1329,  1447,   461,  1691,   107,  1341,   -49,  1549,   462,   557,
     815,   816,   817,   818,  1030,  1326,  1034,  1884,   561,   564,
    1038,  1040,  1042,   813,   571,   572,   569,   569,   814,   819,
     882,  1621,   813,   714,   575,   576,   718,   814,   577,   819,
     242,  1707,   108,   578,   815,   816,   817,   818,  1270,   579,
    1274,  1527,  1282,  1285,   109,  1290,  1293,  1296,   819,   819,
    1746,  1798,  1995, -1099,  1996,   830,   631,  1997,   632, -1099,
    1456,   -49,   581,  1629, -1099,  1631,   594, -1099,   110,   597,
     596,   -49,   606,  1472,  1078,  1080,  1078,  1080,   617,  1476,
     619,   620,   627,   621,  1078,  1080,   625,   815,   816,   817,
     818,    14,   639,   643,  1529,   640,   -49,  1081,   260,   641,
     652,   271,   560, -1099,   663,   612,   666,   676,   677,   681,
    1439, -1099,   732,   736,   271,   738,   813,   773,   158, -1099,
     271,   814,  1440,   815,   816,   817,   818,  1439,  1327,  1078,
    1080,   775,   778,   111,  1615,  1615,  1619, -1099,   262, -1099,
      56,   776,   262,   112,   777,   779,  1947,   780,   781,   782,
     783,    57, -1099,  1950, -1099, -1099, -1099, -1056,    58,   784,
     787,   785,    59,   792,   815,   816,   817,   818,   113,   813,
    1645,  1530,  1647,   786,   814,   793,   815,   816,   817,   818,
     794,   795,    60,  1531,   812,   796,   797,   924,   798,   815,
     816,   817,   818,   799,   800,   810,  1532,   813,   819,  1606,
     825,   829,   814, -1099,   852,   801,   820,   846,   871,  1316,
     173,  1553,   821, -1074,   828,   854,   556,  1607,  1521,   260,
     610,  1604,  1843,   815,   816,   817,   818,  -735,   875,   867,
    1533,   879,  1690,  1603,   878,   883,   876,   855,   815,   816,
     817,   818,   887,   813,    61,  1534,   884,   890,   814,  1976,
     634,   893,   894,   262,   896,   897,   262,   906,   901,  1558,
    1601,   930,   931,   569,   242,   247,   569,   279,   152,   932,
    -438,   815,   816,   817,   818,    62,   937,   329,  1737,    63,
     815,   816,   817,   818,   569,   569,   945,  1738,  -735,   242,
     946,  -735,   951,   952,   813,   954,   953,   956,  -735,   814,
    -735,   957,   962,   973,  -735,   813,   966,   967,   974,  -735,
     814,   983,  -735,   985, -1099, -1099, -1099, -1099, -1099, -1099,
   -1099,   986,   991, -1100,   992,   993, -1099,  -735, -1099, -1100,
     612,   997,  1057,   813, -1100,   186,  1062, -1100,   814,  1073,
     963,  -735,  1696,   813,  1100,   331,  1105,  1701,   814,  -735,
   -1101,  1106,  1107,  -735,  1109,  1113, -1101,   837,  1130,  1673,
     813, -1101,  -735,  1147, -1101,   814,  1922,  1137,    64,  1152,
    1154,   830,  1176, -1100,   815,   816,   817,   818,  -735,  1747,
    1174, -1100,  1164,   909,  1175,  1181,  1182,  1184,  1190, -1100,
    1199,  1799,   813,  1191,  1192,  1206,  1216,   814,   813,  1220,
   -1101,  1225,  1239,   814,  1263,  -735,   813, -1100, -1101, -1100,
     813,   814,  1298,  1754,  1299,   814, -1101,  -735,  1315,  -735,
    1305,  1342, -1100,  1497, -1100, -1100, -1100,   815,   816,   817,
     818,  1307,   173,   813, -1101,  1341, -1101,  1716,  1548,  1347,
    1521,  1349,   518,  1368,  1373,   610,  1365,  1374,  1739, -1101,
    1386, -1101, -1101, -1101,  1400,   815,   816,   817,   818,  1378,
    1388,  1423,  1745,  1403,  1435,   584,  -735,  1425,  1443,  1458,
     235,  1445,  1454, -1100,   592,  1451,  1455,  1466,  1761,  1245,
    1245,  1457,  1233,  1474,   889,  1601,  -735,  1479,  1006,  1480,
    -735,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1481,
   -1101,   815,   816,   817,   818,  1487,  1488,  1492,  1748,   813,
    1495,  1496,  -735,  -735,   814,  -735,  1498,  1538,   813,  1540,
    1499,   569,   569,   814,  1506,  1507,  1508,  1608,  1627,  1610,
     630,  1509,  1511,  1512,   633,  1513,  1514,  1081,  1515,  1516,
    1611,  1624,  1632,   242,   242,  1822,   362,  1625,  1633,  1636,
    1901,  1637,   815,   816,   817,   818,  1646,  1660,  1664,  1749,
    1672,  1615,  1665,   815,   816,   817,   818,  1203,  1936,  1692,
    1876,   173,  1666,  1667,  1668,  1669,  1686,  1991,  1694,  1709,
    1695,  1700,  1729,  1705, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100,   815,   816,   817,   818,  1728, -1100,  1871, -1100,  1735,
    1743,   815,   816,   817,   818,  1744,  1752,  1753,  1932, -1062,
    1756, -1101, -1101, -1101, -1101, -1101, -1101, -1101,   815,   816,
     817,   818,   155, -1101,  1781, -1101,  1757,  1759,  -339,  1765,
    1770,  1788,  1771,   789,  1793,  1917,  1772,  1773,   855,  1774,
     173,  1795,  1775,   239,  1778,   802,   803,   806,  1521,  1779,
     815,   816,   817,   818,  1813,  1780,   815,   816,   817,   818,
    1823,  1782,  1783,  1784,   815,   816,   817,   818,   815,   816,
     817,   818,  1825,  1785,  1787,  1245,  1786,  1826,  1827,  1828,
    1829,  1830,  1832,  1833,  1834,  1844,  1842,  1848,  1918,  1766,
    1849,  1850,  1081,  1853,  1081,  1854,  1855,  1895,  1870,   612,
    1881,  1856,  1081,  1915,  1859,   362,   242,  1860,  1861,  1862,
    1863,  1864,  1925,  1898,  1908,  1865,  1927,  1866,   849,  1913,
    1914,  1896,   851,   612,  1084,  1926,  1867,  1868,  1954,   173,
    1930,  1873,  1008,  1911,   239,  1938,  1945,  1946,   813,  1935,
    1948,  1955,  1939,   814,  1962,  1951,  1009,  1081,  1963,  1981,
     612,  1964,  1967,  1986,  1983,  1989,  1994,   372,  1978,  2004,
    1979,   612,  2005,  2007,   612,  2008,  2011,   815,   816,   817,
     818,   612,  2020,   373,  2021,   845,   815,   816,   817,   818,
     325,   239,  1010,   326,   374,   563,  1612,  1116,   847,   327,
    1011,   375,   570,  1475,  1473,   376,   377,  1108,  1012,   235,
    1941,   984,  1238,  1243,   378,  1463,  1836,  1212,  1942,   282,
     947,   644,  1402,   689,   610,   379,  1013,  1179,  1014,   615,
    1464,   659,   560,  1148,  1815,  1821,  1469,  1835,   661,  1977,
    1984,  1015,  2003,  1016,  1017,  1018,  1982,    68,   610,  1172,
    1351,  1893,  1970,    28,  1097,  1806,  1641,  1683,    69,  1217,
     161,   929,   651,   607,  1601,    70,  1965,   909,   862,  1134,
     642,  1209,   262,   667,   866,   610,   342,   970,    71,   380,
    1138,  1132,   292,  1377,   239,   239,   610,   381,  1601,   610,
    1630,  1869,  1019,  1952,  1026,  1153,   610,  1734,    48,    49,
      52,  1005,   874,  1114,  1372,  1650,  1502,  1202,  1542,  1912,
    1452,   297,  1760,     0,     0,  1601,  1134,     0,   382,     0,
       0,  1031,   383,     0,   925,  1036,  1601,     0,  1245,  1601,
       0,     0,  1052,   384,     0,    72,  1601,  1059,  1060,     0,
       0,  1063,  1064,  1065,  1066,  1067,  1071,     0,     0,     0,
       0,    73,   385,   386,     0,     0,     0,   518,     0,  1085,
    1086,  1087,  1088,  1089,  1090,   518,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,     0,     0,     0,    76,     0,     0,    77,
       0,     0,     0,     0,     0,     0,     0,    78,     0,   592,
       0,     0,     0,  1020,  1021,  1022,   815,   816,   817,   818,
       0,   387,     0,  1075,   388,     0,    79,     0,     0,     0,
    1145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   630,  1258,  1156,     0,  1259,     0,   114,     0,
     466,   115,     0,   116,   117,   118,   119,   467,   120,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,   468,
       0,     0,     0,     0,     0,   469,   470,   123,   124,     0,
     125,     0,     0,     0,     0,    80,     0,   126,   127,     0,
     128,   129,     0,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,   471,   136,   137,   472,     0,     0,     0,
     138,   139,   140,     0,   141,   142,   143,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   473,     0,   474,   475,     0,     0,   311,     0,     0,
       0,     0,     0,   476,     0,     0,     0,   477,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1256,  1257,  1262,     0,     0,  1271,  1272,
    1275,     0,  1283,  1286,  1288,  1291,  1294,  1297,     0,     0,
       0,     0,     0,     0,  1301,  1302,     0,  1304,     0,  1306,
       0,     0,  1309,  1310,  1311,  1312,  1313,  1314,     0,     0,
    1260,     0,   478,     0,     0,     0,     0,     0,   479,  1318,
       0,     0,  1321,     0,     0,     0,     0,     0,   480,     0,
       0,     0,     0,     0,     0,     0,     0,  1338,     0,     0,
       0,     0,   481,   482,   483,   484,   253,   254,   115,     0,
     116,   117,   118,   119,     0,   120,   485,     0,     0,     0,
       0,     0,     0,   121,   122,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   123,   495,     0,   125,   518,     0,
     496,   497,     0,   498,   499,   500,     0,   128,   129,   501,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   502,   140,
       0,   503,   142,   143,     0,   144,     0,   504,   505,   506,
       0,   507,   508,     0,   509,     1,     0,     0,   510,   511,
       0,     0,   987,  1008,   583,   691,   513,     0,     0,   813,
     331,     0,     0,     2,   814,     0,     0,  1009,   988,     0,
       0,     3,   692,     0,     0,   693,     0,     0,     4,     0,
       5,     0,     6,     0,     0,     0,     0,     7,     0,     0,
       0,     0,     0,     0,     8,     0,     0,     0,     0,     0,
       0,     0,     0,  1010,     0,     0,     0,     0,     9,     0,
       0,  1011,     0,     0,     0,   694,     0,     0,    10,  1012,
       0,   695,   696,   697,     0,   698,   699,   700,     0,   701,
       0,     0,     0,     0,     0,     0,     0,  1013,     0,  1014,
       0,     0,     0,     0,     0,     0,     0,   702,   518,   703,
       0,     0,  1015,     0,  1016,  1017,  1018,     0,     0,     0,
       0,  1500,  1501,   690,  1503,  1505,   691,     0,     0,     0,
       0,     0,  1510,     0,     0,     0,     0,     0,   704,     0,
       0,    11,     0,   692,     0,     0,   693,     0,    12,     0,
       0,    13,  1528,    14,    15,   705,     0,     0,     0,     0,
    1535,  1536,  1318,  1019,     0,     0,     0,     0,  1318,     0,
    1321,  1543,   518,  1545,     0,     0,     0,   706,    16,     0,
    1546,     0,  1547,     0,     0,     0,   694,     0,     0,     0,
       0,     0,   695,   696,   697,     0,   698,   699,   700,     0,
     701,     0,     0,     0,     0,     0,     0,     0,  1269,     0,
       0,  1259,   707,   708,     0,   466,     0,     0,   702,     0,
     703,     0,   467,     0,     0,     0,     0,    17,     0,   709,
       0,   518,     0,     0,   468,     0,     0,     0,     0,     0,
     469,   470,    18,    19,     0,     0,     0,     0,     0,   704,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1654,     0,     0,   705,    20,   471,     0,
       0,   472,     0,     0,  1020,  1021,  1022,   815,   816,   817,
     818,     0,     0,     0,     0,     0,     0,     0,   706,     0,
       0,     0,     0,     0,     0,     0,   473,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,     0,   476,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   707,   708,     0,     0,     0,     0,     0,
       0,  1702,     0,     0,     0,  1703,  1704,  1708,     0,     0,
     709,     0,  1712,  1710,  1711,     0,  1713,  1714,     0,  1717,
       0,     0,     0,     0,  1718,  1719,  1720,  1721,     0,  1722,
    1723,  1724,  1725,  1726,  1727,  1260,     0,   478,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,  1318,  1318,     0,     0,     0,     0,
       0,     0,     0,  1750,     0,     0,     0,   481,   482,   483,
     484,   253,   254,   115,     0,   116,   117,   118,   119,  1758,
     120,   485,     0,  1093,     0,     0,     0,     0,   121,   122,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   123,
     495,     0,   125,     0,     0,   496,   497,     0,   498,   499,
     500,     0,   128,   129,   501,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   502,   140,     0,   503,   142,   143,     0,
     144,     0,   504,   505,   506,     0,   507,   508,     0,   509,
       0,  1802,     0,   510,   511,     0,     0,     0,     0,   583,
       0,   513,     0,     0,     0,   331,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1273,     0,     0,  1259,
       0,     0,     0,   466,     0,     0,     0,     0,     0,     0,
     467,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,     0,     0,     0,     0,     0,   469,   470,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1852,     0,     0,     0,     0,     0,
    1857,  1858,     0,     0,     0,     0,   471,     0,     0,   472,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,  1872,     0,     0,     0,     0,  1318,
     121,   122,  1875,     0,   473,     0,   474,   475,     0,  1882,
    1883,   123,   124,     0,   125,     0,   476,     0,     0,     0,
     477,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,  1094,     0,     0,
       0,     0,     0,  1260,     0,   478,     0,     0,     0,     0,
       0,   479,     0,     0,     0,     0,     0,     0,     0,   471,
       0,   480,     0,   658,     0,     0,     0,     0,  1924,     0,
       0,     0,     0,     0,     0,   481,   482,   483,   484,   253,
     254,   115,     0,   116,   117,   118,   119,   473,   120,   485,
       0,     0,     0,     0,     0,     0,   121,   122,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   123,   495,     0,
     125,     0,     0,   496,   497,     0,   498,   499,   500,     0,
     128,   129,   501,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,  1802,   136,   137,     0,     0,     0,     0,
     138,   502,   140,     0,   503,   142,   143,     0,   144,     0,
     504,   505,   506,     0,   507,   508,     0,   509,   478,     0,
       0,   510,   511,     0,     0,  1281,     0,   583,  1259,   513,
    1971,     0,   466,   331,   480,     0,     0,     0,     0,   467,
       0,     0,     0,     0,     0,     0,     0,     0,   481,   482,
     483,   468,   253,   254,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   486,   487,   488,   489,   490,   491,   492,   493,   494,
       0,     0,     0,     0,     0,   471,   496,   497,   472,     0,
       0,   114,     0,     0,   115,   501,   116,   117,   118,   119,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   473,     0,   474,   475,     0,     0,     0,
     123,   124,     0,   125,     0,   476,     0,     0,     0,   477,
     126,   127,     0,   128,   129,  1405,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1260,     0,   478,     0,     0,     0,     0,     0,
     479,     0,     0,   664,     0,     0,     0,     0,     0,     0,
     480,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   481,   482,   483,   484,   253,   254,
     115,     0,   116,   117,   118,   119,     0,   120,   485,     0,
       0,     0,     0,     0,     0,   121,   122,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   123,   495,     0,   125,
       0,     0,   496,   497,     0,   498,   499,   500,     0,   128,
     129,   501,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     502,   140,     0,   503,   142,   143,     0,   144,     0,   504,
     505,   506,     0,   507,   508,  1284,   509,     0,  1259,     0,
     510,   511,   466,     0,     0,     0,   583,     0,   513,   467,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,   468,     0,    91,     0,     0,     0,   469,   470,     0,
      92,     0,     0,     0,    93,    94,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,   471,     0,     0,   472,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   473,     0,   474,   475,     0,     0,     0,
     123,   124,     0,   125,     0,   476,     0,     0,     0,   477,
     126,   127,     0,   128,   129,     0,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,    97,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
       0,   100,  1260,     0,   478,     0,     0,     0,     0,     0,
     479,     0,   101,     0,     0,     0,     0,     0,     0,     0,
     480,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   481,   482,   483,   484,   253,   254,
     115,     0,   116,   117,   118,   119,     0,   120,   485,     0,
       0,     0,     0,     0,  1352,   121,   122,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   123,   495,     0,   125,
       0,     0,   496,   497,     0,   498,   499,   500,     0,   128,
     129,   501,   130,     0,     0,   131,   132,   133,   134,   135,
     103,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     502,   140,     0,   503,   142,   143,     0,   144,     0,   504,
     505,   506,     0,   507,   508,  1289,   509,     0,  1259,     0,
     510,   511,   466,     0,  1353,     0,   583,     0,   513,   467,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,   468,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1354,
       0,     0,     0,     0,  1355,     0,     0,     0,     0,     0,
    1356,     0,     0,     0,     0,   471,     0,     0,   472,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,  1357,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   473,     0,   474,   475,     0,     0,     0,
     123,   124,     0,   125,     0,   476,     0,     0,     0,   477,
     126,   127,     0,   128,   129,     0,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1260,     0,   478,     0,     0,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     480,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   481,   482,   483,   484,   253,   254,
     115,     0,   116,   117,   118,   119,     0,   120,   485,     0,
       0,     0,     0,     0,     0,   121,   122,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   123,   495,     0,   125,
       0,     0,   496,   497,     0,   498,   499,   500,     0,   128,
     129,   501,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     502,   140,     0,   503,   142,   143,     0,   144,     0,   504,
     505,   506,     0,   507,   508,  1292,   509,     0,  1259,     0,
     510,   511,   466,     0,     0,     0,   583,     0,   513,   467,
       0,     0,   331,     0,     0,     0,     0,     0,    14,     0,
       0,   468,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,   472,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
       0,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,     0,   473,     0,   474,   475,     0,     0,     0,
     123,   124,     0,   125,     0,   476,     0,     0,     0,   477,
     126,   127,     0,   128,   129,     0,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1119,     0,  1260,     0,   478,     0,     0,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     480,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   481,   482,   483,   484,   253,   254,
     115,     0,   116,   117,   118,   119,     0,   120,   485,     0,
       0,     0,     0,     0,     0,   121,   122,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   123,   495,     0,   125,
       0,     0,   496,   497,     0,   498,   499,   500,     0,   128,
     129,   501,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     502,   140,     0,   503,   142,   143,     0,   144,     0,   504,
     505,   506,     0,   507,   508,  1295,   509,     0,  1259,     0,
     510,   511,   466,     0,     0,     0,   583,     0,   513,   467,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,   468,     0,     0,     0,     0,     0,   469,   470,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,   472,     0,
       0,     0,   114,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,   473,     0,   474,   475,     0,     0,     0,
       0,   123,   124,     0,   125,   476,     0,     0,     0,   477,
       0,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1077,  1260,   513,   478,     0,     0,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     480,     0,     0,     0,     0,     0,     0,  1023,     0,     0,
       0,     0,     0,     0,   481,   482,   483,   484,   253,   254,
     115,     0,   116,   117,   118,   119,     0,   120,   485,     0,
    1024,     0,     0,     0,     0,   121,   122,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   123,   495,     0,   125,
       0,   466,   496,   497,     0,   498,   499,   500,   467,   128,
     129,   501,   130,     0,     0,   131,   132,   133,   134,   135,
     468,     0,     0,   136,   137,     0,   469,   470,     0,   138,
     502,   140,     0,   503,   142,   143,  1025,   144,     0,   504,
     505,   506,     0,   507,   508,   750,   509,     0,     0,     0,
     510,   511,     0,     0,   471,     0,   583,   472,   513,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,   473,     0,   474,   475,     0,     0,   121,   122,
       0,     0,     0,     0,   476,     0,     0,     0,   477,   123,
     124,     0,   125,     0,     0,     0,   998,     0,     0,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,    14,     0,     0,     0,   752,     0,     0,
       0,     0,     0,   478,     0,     0,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,   483,   484,   253,   254,   115,
       0,   116,   117,   118,   119,     0,   120,   485,     0,     0,
       0,     0,     0,     0,   121,   122,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   123,   495,     0,   125,     0,
       0,   496,   497,     0,   498,   499,   500,     0,   128,   129,
     501,   130,     0,     0,   131,   132,   133,   134,   135,   999,
    1000,  1001,   136,   137,     0,     0,     0,     0,   138,   502,
     140,     0,   503,   142,   143,     0,   144,   466,   504,   505,
     506,     0,   507,   508,   467,   509,     0,     0,     0,   510,
     511,     0,     0,     0,     0,   756,   468,   513,     0,   749,
       0,   331,   469,   470,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   750,     0,     0,     0,     0,     0,     0,     0,     0,
     471,     0,     0,   472,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,   473,     0,
     474,   475,     0,     0,     0,   123,   124,     0,   125,     0,
     476,     0,     0,     0,   477,   126,   127,     0,   128,   129,
       0,   130,   751,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   752,     0,     0,     0,   513,     0,   478,
       0,     0,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     482,   483,   484,   253,   254,   115,     0,   116,   117,   118,
     119,     0,   120,   485,     0,     0,     0,     0,     0,     0,
     121,   122,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   123,   495,     0,   125,     0,   466,   496,   497,     0,
     498,   499,   500,   467,   128,   129,   501,   130,     0,     0,
     131,   132,   753,   754,   755,   468,     0,     0,   136,   137,
       0,   469,   470,     0,   138,   502,   140,     0,   503,   142,
     143,     0,   144,     0,   504,   505,   506,     0,   507,   508,
     750,   509,     0,     0,     0,   510,   511,     0,     0,   471,
       0,   756,   472,   513,     0,   114,     0,   331,   115,     0,
     116,   117,   118,   119,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,   473,     0,   474,
     475,     0,     0,     0,   123,   124,     0,   125,     0,   476,
       0,     0,     0,   477,   126,   127,     0,   128,   129,     0,
     130,   994,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   139,   140,
       0,   141,   142,   143,     0,   144,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   752,     0,  1119,     0,     0,     0,   478,     0,
       0,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   480,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   481,   482,
     483,   484,   253,   254,   115,     0,   116,   117,   118,   119,
       0,   120,   485,     0,     0,     0,     0,     0,     0,   121,
     122,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     123,   495,     0,   125,     0,   466,   496,   497,     0,   498,
     499,   500,   467,   128,   129,   501,   130,     0,     0,   131,
     132,   753,   754,   755,   468,     0,     0,   136,   137,     0,
     469,   470,     0,   138,   502,   140,     0,   503,   142,   143,
       0,   144,     0,   504,   505,   506,     0,   507,   508,   750,
     509,     0,     0,     0,   510,   511,     0,   565,   471,     0,
     756,   472,   513,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,   473,     0,   474,   475,
       0,     0,   121,   122,     0,     0,     0,     0,   476,     0,
       0,     0,   477,   123,   124,     0,   125,     0,     0,     0,
     751,     0,     0,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,   752,     0,     0,     0,     0,     0,   478,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,   483,
     484,   253,   254,   115,     0,   116,   117,   118,   119,     0,
     120,   485,     0,     0,     0,     0,     0,     0,   121,   122,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   123,
     495,     0,   125,     0,   466,   496,   497,     0,   498,   499,
     500,   467,   128,   129,   501,   130,     0,     0,   131,   132,
     753,   754,   755,   468,     0,     0,   136,   137,     0,   469,
     470,     0,   138,   502,   140,     0,   503,   142,   143,     0,
     144,     0,   504,   505,   506,     0,   507,   508,     0,   509,
       0,     0,     0,   510,   511,     0,     0,   471,     0,   756,
     472,   513,     0,   114,     0,   331,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   473,     0,   474,   475,     0,
       0,     0,   123,   124,     0,   125,     0,   476,     0,     0,
       0,   477,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -340,     0,     0,     0,     0,     0,   478,     0,     0,     0,
       0,     0,   479,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   481,   482,   483,   484,
     253,   254,   115,     0,   116,   117,   118,   119,     0,   120,
     485,     0,     0,     0,     0,     0,     0,   121,   122,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   123,   495,
       0,   125,     0,     0,   496,   497,     0,   498,   499,   500,
       0,   128,   129,   501,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   502,   140,     0,   503,   142,   143,  1068,   144,
       0,   504,   505,   506,  1069,   507,   508,  1070,   509,   466,
       0,     0,   510,   511,     0,     0,   467,     0,   583,     0,
     513,     0,     0,     0,   331,     0,     0,     0,   468,     0,
       0,     0,     0,     0,   469,   470,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   750,     0,     0,     0,     0,     0,     0,
       0,     0,   471,     0,     0,   472,    83,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,     0,   120,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
     473,     0,   474,   475,     0,     0,     0,   123,   124,     0,
     125,     0,   476,     0,     0,     0,   477,   126,   127,     0,
     128,   129,     0,   130,   994,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   139,   140,     0,   141,   142,   143,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    84,     0,     0,   752,     0,     0,     0,     0,
       0,   478,     0,     0,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,   483,   484,   253,   254,   115,     0,   116,
     117,   118,   119,     0,   120,   485,     0,     0,     0,     0,
       0,     0,   121,   122,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   123,   495,     0,   125,     0,   466,   496,
     497,     0,   498,   499,   500,   467,   128,   129,   501,   130,
       0,     0,   131,   132,   133,   134,   135,   468,     0,     0,
     136,   137,     0,   469,   470,     0,   138,   502,   140,     0,
     503,   142,   143,     0,   144,     0,   504,   505,   506,     0,
     507,   508,     0,   509,     0,     0,     0,   510,   511,     0,
       0,   471,     0,   756,   472,   513,     0,   114,     0,   331,
     115,     0,   116,   117,   118,   119,     0,   120,     0,     0,
       0,     0,     0,     0,     0,   121,   122,     0,     0,   473,
       0,   474,   475,     0,     0,     0,   123,   124,     0,   125,
       0,   476,     0,     0,     0,   477,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,   132,   133,   134,   135,
       0,     0,     0,   136,   137,     0,     0,     0,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1094,     0,     0,     0,     0,     0,     0,     0,
     478,     0,     0,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     481,   482,   483,   484,   253,   254,   115,     0,   116,   117,
     118,   119,     0,   120,   485,     0,     0,     0,     0,     0,
       0,   121,   122,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   123,   495,     0,   125,     0,   466,   496,   497,
       0,   498,   499,   500,   467,   128,   129,   501,   130,     0,
       0,   131,   132,   133,   134,   135,   468,     0,     0,   136,
     137,     0,   469,   470,     0,   138,   502,   140,     0,   503,
     142,   143,     0,   144,     0,   504,   505,   506,     0,   507,
     508,     0,   509,     0,     0,     0,   510,   511,   588,     0,
     471,     0,   583,   472,   513,     0,   114,     0,   331,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,   473,     0,
     474,   475,     0,     0,     0,   123,   124,     0,   125,     0,
     476,     0,     0,     0,   477,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   478,
       0,     0,     0,     0,     0,   479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,     0,     0,   788,     0,     0,     0,     0,     0,   481,
     482,   483,   484,   253,   254,   115,     0,   116,   117,   118,
     119,     0,   120,   485,     0,     0,     0,     0,     0,     0,
     121,   122,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   123,   495,     0,   125,     0,   466,   496,   497,     0,
     498,   499,   500,   467,   128,   129,   501,   130,     0,     0,
     131,   132,   133,   134,   135,   468,     0,     0,   136,   137,
       0,   469,   470,     0,   138,   502,   140,     0,   503,   142,
     143,     0,   144,     0,   504,   505,   506,     0,   507,   508,
       0,   509,     0,     0,     0,   510,   511,     0,     0,   471,
       0,   583,   472,   513,     0,     0,     0,   331,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,   474,
     475,     0,     0,     0,     0,     0,     0,     0,     0,   476,
       0,     0,     0,   477,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   478,     0,
       0,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   480,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   481,   482,
     483,   484,   253,   254,   115,     0,   116,   117,   118,   119,
       0,   120,   485,     0,     0,     0,     0,     0,     0,   121,
     122,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     123,   495,     0,   125,     0,   466,   496,   497,     0,   498,
     499,   500,   467,   128,   129,   501,   130,     0,     0,   131,
     132,   133,   134,   135,   468,     0,     0,   136,   137,     0,
     469,   470,     0,   138,   502,   140,     0,   503,   142,   143,
       0,   144,     0,   504,   505,   506,     0,   507,   508,     0,
     509,     0,     0,     0,   510,   511,     0,     0,   471,     0,
     583,   472,   513,     0,     0,     0,   331,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,     0,   476,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   478,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   481,   482,   483,
     484,   253,   254,   115,     0,   116,   117,   118,   119,     0,
     120,   485,     0,     0,     0,     0,     0,     0,   121,   122,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   123,
     495,     0,   125,     0,     0,   496,   497,     0,   498,   499,
     500,     0,   128,   129,   501,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   502,   140,     0,   503,   142,   143,     0,
     144,     0,   504,   505,   506,   466,   507,   508,     0,   509,
       0,     0,   467,   510,   511,     0,     0,     0,     0,   583,
    1091,   513,     0,     0,   468,   331,     0,     0,     0,     0,
     469,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,   472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,   474,   475,
       0,     0,     0,     0,     0,     0,     0,     0,   476,     0,
       0,     0,   477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   478,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   480,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1287,     0,     0,   481,   482,   483,
     484,   253,   254,   115,     0,   116,   117,   118,   119,     0,
     120,   485,     0,     0,     0,     0,     0,     0,   121,   122,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   123,
     495,     0,   125,     0,     0,   496,   497,     0,   498,   499,
     500,     0,   128,   129,   501,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   502,   140,     0,   503,   142,   143,     0,
     144,   466,   504,   505,   506,     0,   507,   508,   467,   509,
       0,     0,     0,   510,   511,     0,     0,     0,     0,   583,
     468,   513,     0,     0,     0,   331,   469,   470,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1337,     0,     0,   471,     0,     0,   472,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   473,     0,   474,   475,     0,     0,     0,     0,
       0,     0,     0,     0,   476,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   478,     0,     0,     0,     0,     0,   479,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   481,   482,   483,   484,   253,   254,   115,
       0,   116,   117,   118,   119,     0,   120,   485,     0,     0,
       0,     0,     0,     0,   121,   122,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   123,   495,     0,   125,     0,
     466,   496,   497,     0,   498,   499,   500,   467,   128,   129,
     501,   130,     0,     0,   131,   132,   133,   134,   135,   468,
       0,     0,   136,   137,     0,   469,   470,     0,   138,   502,
     140,     0,   503,   142,   143,     0,   144,     0,   504,   505,
     506,     0,   507,   508,     0,   509,     0,     0,     0,   510,
     511,     0,     0,   471,     0,   583,   472,   513,     0,     0,
       0,   331,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   473,     0,   474,   475,     0,     0,     0,     0,     0,
       0,     0,     0,   476,     0,     0,     0,   477,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   478,     0,     0,     0,     0,     0,   479,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   480,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1504,
       0,     0,   481,   482,   483,   484,   253,   254,   115,     0,
     116,   117,   118,   119,     0,   120,   485,     0,     0,     0,
       0,     0,     0,   121,   122,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   123,   495,     0,   125,     0,   466,
     496,   497,     0,   498,   499,   500,   467,   128,   129,   501,
     130,     0,     0,   131,   132,   133,   134,   135,   468,     0,
       0,   136,   137,     0,   469,   470,     0,   138,   502,   140,
       0,   503,   142,   143,     0,   144,     0,   504,   505,   506,
       0,   507,   508,     0,   509,     0,     0,     0,   510,   511,
       0,     0,   471,     0,   583,   472,   513,     0,     0,     0,
     331,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,   474,   475,     0,     0,     0,     0,     0,     0,
       0,     0,   476,     0,     0,     0,   477,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   478,     0,     0,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   480,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   481,   482,   483,   484,   253,   254,   115,     0,   116,
     117,   118,   119,     0,   120,   485,     0,     0,     0,     0,
       0,     0,   121,   122,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   123,   495,     0,   125,     0,   466,   496,
     497,     0,   498,   499,   500,   467,   128,   129,   501,   130,
       0,     0,   131,   132,   133,   134,   135,   468,     0,     0,
     136,   137,     0,   469,   470,     0,   138,   502,   140,     0,
     503,   142,   143,     0,   144,     0,   504,   505,   506,     0,
     507,   508,     0,   509,     0,     0,     0,   510,   511,     0,
       0,   471,     0,   512,   472,   513,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   473,
       0,   474,   475,     0,     0,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,   477,     0,     0,     0,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
     478,     0,     0,     0,     0,     0,   479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   480,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
     481,   482,   483,   484,   253,   254,   115,     0,   116,   117,
     118,   119,     0,   120,   485,     0,     0,     0,     0,     0,
       0,   121,   122,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   123,   495,     0,   125,     0,     0,   496,   497,
       0,   498,   499,   500,     0,   128,   129,   501,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,   111,   136,
     137,     0,  1349,     0,     0,   138,   502,   140,   112,   503,
     142,   143,     0,   144,     0,   504,   505,   506,     0,   507,
     508,     0,   509,     0,     0,     0,   510,   511,     0,     0,
       6,     0,   583,   113,   513,     0,     0,  -294,   331,     0,
       0,  1559,  1560,     0,  1561,     0,     0,     0,     0,     0,
       0,   114,     0,     0,   115,     0,   116,   117,   118,   119,
    1562,   120,     0,     0,     0,     0,  1563,     0,     0,   121,
     122,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,   124,     0,   125,     0,     0,     0,     0,     0,     0,
     126,   127,     0,   128,   129,     0,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
    1349,     0,     0,   138,   139,   140,     0,   141,   142,   143,
       0,   144,     0,  1564,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     0,
       0,    14,     0,     0,     0,  -296,     0,     0,     0,  1559,
    1560,     0,  1561,     0,     0,     0,  1565,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,  1562,     0,
       0,     0,     0,     0,  1563,  1766,     0,     0,     0,     0,
       0,     0,     0,     0,   484,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1566,   124,     0,   125,     0,     0,     0,
       0,     0,     0,   126,   127,     0,   128,   129,     0,   130,
       0,  1564,   131,  1567,   133,   134,   135,     0,     0,     0,
     136,   137,  1568,  1569,  1570,  1349,   138,   139,   140,    14,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1565,     0,     0,     0,     0,     0,
       0,     0,     0,     6,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1559,  1560,     0,  1561,     0,     0,
       0,     0,   484,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,  1562,     0,     0,     0,     0,     0,  1563,
     121,   122,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1566,   124,     0,   125,     0,     0,     0,     0,     0,
       0,   126,   127,     0,   128,   129,     0,   130,     0,     0,
     131,  1567,   133,   134,   135,     0,     0,     0,   136,   137,
    1568,  1569,  1570,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,  1564,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1565,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   484,     0,     0,
     115,     0,   116,   117,   118,   119,     0,   120,   690,     0,
       0,   691,     0,     0,     0,   121,   122,   909,     0,     0,
       0,     0,     0,     0,     0,     0,  1566,   124,   692,   125,
       0,   693,     0,     0,     0,     0,   126,   127,     0,   128,
     129,     0,   130,     0,     0,   131,  1567,   133,   134,   135,
       0,     0,     0,   136,   137,  1568,  1569,  1570,     0,   138,
     139,   140,     0,   141,   142,   143,     0,   144,     0,     0,
       0,   694,     0,     0,     0,     0,     0,   695,   696,   697,
       0,   698,   699,   700,     0,   701,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   702,     0,   703,   690,     0,     0,   691,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   692,     0,     0,   693,
       0,     0,     0,     0,   704,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   705,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   694,
       0,     0,     0,   706,     0,   695,   696,   697,     0,   698,
     699,   700,     0,   701,     0,     0,     0,     0,   114,     0,
       0,   115,     0,   116,   117,   118,   119,     0,   120,     0,
       0,   702,     0,   703,     0,     0,   121,   122,   707,   708,
       0,     0,     0,     0,     0,     0,     0,   123,   124,     0,
     125,     0,     0,     0,     0,   709,     0,   126,   127,     0,
     128,   129,   704,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,   705,
     138,   139,   140,     0,   141,   142,   143,     0,   144,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   706,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,   707,   708,     0,     0,
       0,     0,     0,     0,     0,   123,   124,     0,   125,     0,
       0,     0,     0,   709,     0,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,   114,   144,     0,   115,   190,
     116,   117,   118,   119,   191,   120,   192,   193,   194,   195,
     196,   197,   198,   121,   122,   199,   200,   201,   202,   203,
       0,     0,   204,   205,   123,   124,   206,   125,     0,     0,
     207,   208,   209,   210,   126,   127,   211,   128,   129,   212,
     130,   213,   214,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,   215,   216,   217,   218,   138,   139,   140,
     219,   141,   142,   143,   220,   144,   221,   222,   223,   224,
     225,   226,   227,   228,   229
};

static const short yycheck[] =
{
       9,     0,     4,   625,    70,    60,   113,    16,   172,    18,
     162,   163,    69,   162,   163,    16,    68,   409,    98,    79,
      69,    61,   444,    76,   101,   444,    92,    62,   810,   810,
     318,   512,   440,    73,   411,    74,   660,   630,  1077,  1101,
      75,    96,   102,   873,   774,  1348,   552,   100,    57,    58,
     572,    60,    61,    62,    63,    64,   436,    97,   512,    98,
      69,    70,    71,   360,    73,   105,    75,    76,   594,    78,
      79,    80,    64,  1165,   821,   805,  1165,   573,  1349,    88,
    1422,    90,    91,    92,  1165,    94,    95,    96,    97,    90,
      99,   100,   101,   102,   103,   828,   105,   421,    99,  1207,
     177,   333,   583,    95,   744,   154,  1119,   156,   174,   391,
     737,   103,   981,  1165,  1028,  1229,  1367,   420,   788,   181,
     155,    10,  1173,  1574,   794,   269,   450,     8,  1691,   876,
     918,  1317,     8,  1420,   608,   175,     8,  1323,    47,  1387,
       8,   176,  1106,     8,   650,   154,   155,   156,   654,     6,
     676,   160,   751,    47,   660,     8,     8,   751,   167,    19,
    1074,   879,   756,   172,   883,   174,   175,   176,   177,  1414,
    1415,   278,   181,  1224,  1419,   167,    43,    21,  1165,  1632,
      44,     8,   234,     8,    43,   903,    90,    43,   906,    46,
      26,  1165,    10,   133,    23,    97,   360,   915,     8,    28,
    1170,    63,   149,   169,   185,    23,    56,  1177,    63,     8,
      28,   196,   264,    23,    44,   141,  1186,    10,    28,    61,
     174,   192,   366,    90,    23,   118,  1368,   196,   636,    28,
      23,     3,     8,    44,     9,    28,   126,    46,   660,  1645,
    1948,   660,  1212,    55,   125,    63,   139,   193,     4,   125,
      23,     7,   145,   125,    18,    28,   974,   125,     5,   308,
     125,    61,   311,   370,   141,  1568,  1569,    63,    15,    80,
     662,   286,   125,   125,    23,   756,   291,   565,   686,    28,
      55,    64,   244,    14,  1690,    94,   179,  1995,   178,   344,
     614,    66,   580,    49,    77,  1077,  1077,   282,   125,   308,
     125,  1443,   311,  1574,    76,   155,   287,   122,   293,   318,
     319,   360,   364,   348,    63,   286,   180,   100,   710,   174,
     246,   287,  1414,  1415,   293,  1414,  1415,  1419,   230,   141,
    1419,   114,   286,  1414,  1415,   344,  1006,  1007,  1419,   348,
     287,   177,  1795,  1394,   204,  1453,   173,   287,   173,   125,
     180,   255,     8,  1539,  1540,  1417,    50,    51,   812,  1922,
      80,   410,  1414,  1415,   176,  1378,   820,  1419,   151,   180,
    1657,   420,    11,   422,   105,   424,  1624,   286,   427,   853,
     446,   390,   391,   260,    23,   437,  1056,   439,   255,    28,
     165,   191,   286,  1181,  1182,   448,    68,   286,  1460,   998,
     994,   410,   454,   458,   998,   286,    45,   447,   572,  1373,
    1374,   420,   288,   422,  1144,   424,   288,    26,   427,   278,
     288,   456,   278,   288,   286,   931,   283,  1414,  1415,  1147,
     190,   957,  1419,  1152,    22,   288,   288,   446,   447,   448,
    1414,  1415,    90,  1231,   286,  1419,   290,   456,    62,   458,
    1168,  1169,   461,    23,   283,   284,    21,   196,    28,   461,
     180,  1764,    19,   281,   282,   283,   284,  1437,   286,   125,
    1188,   281,   282,   283,   284,   196,   117,  1819,   285,  1393,
     263,   196,   281,   282,   283,   284,   982,    43,   281,   282,
     283,   284,  1943,    23,  1164,   189,   168,   993,    28,    50,
      51,   997,   894,   175,   513,   897,    18,    23,   281,   282,
     283,   284,    28,    61,   123,   288,  1967,   147,     4,    94,
     582,   913,    87,   572,   564,   139,   286,   562,    65,   678,
     118,   196,   281,   282,   283,   284,   571,    94,    94,   288,
     575,    44,  1793,  1994,    55,   186,   938,   286,   117,   837,
     196,   199,   200,   562,  2005,   564,   565,  2008,  1744,  1341,
    1341,   282,   571,    49,  2015,   179,   575,   282,   177,   621,
      82,   580,  1914,   582,  1688,   872,   625,    80,  1640,  1249,
    1250,    64,  1252,  1253,   728,   194,   638,   156,    84,  1111,
    1459,     3,  1700,    30,    77,  1342,  1223,  1237,   196,   679,
     652,   904,   884,    59,    60,    23,    43,    22,   985,  1231,
      28,   660,  1345,    17,  1956,    30,   625,   100,   850,  1670,
    1633,   117,   287,  1119,   199,   200,    11,    31,   293,   681,
     141,   114,  1877,  1878,   203,  1977,  1687,    98,   189,   122,
     679,    64,   281,   282,   283,   284,   810,   143,   972,   658,
     146,   660,     7,  1149,    77,   664,   143,    46,    23,  1113,
      45,   172,    85,    28,    76,   196,   162,   155,   151,   142,
     156,    75,  1943,   825,   726,    23,   187,   100,    85,  1649,
      28,   733,   734,  1653,  1654,   158,   236,   736,   174,   287,
     196,   114,   188,    23,   182,   293,  1967,    23,    28,   103,
      79,   108,    28,   118,  2007,    94,   167,   194,   872,   858,
      23,   281,   282,   283,   284,    28,   196,   223,   288,  1689,
      23,     6,   208,  1994,   139,    28,    64,   736,   151,   118,
     145,     6,   196,  1403,  2005,   114,    23,  2008,   288,    77,
    1458,    28,  1781,   223,  2015,   289,   296,   126,   292,    65,
       6,   281,   282,   283,   284,  1425,   239,   164,   288,   223,
    1490,    46,   100,   167,   179,   281,   282,   283,   284,  1831,
     779,    46,   288,   230,     6,  1837,   282,   826,  1158,   828,
     263,   830,  1263,  1175,  1176,  1877,  1878,   779,  1877,  1878,
      46,   940,  1706,     6,    65,  1187,  1877,  1878,    23,     4,
     140,   143,   282,    28,   813,   262,   846,   122,  1638,  1263,
     845,    16,   821,   151,    46,   196,   334,   826,   287,   828,
     162,   830,    23,   872,   342,  1877,  1878,    28,   837,   878,
    1423,   883,   867,    46,   869,  1805,   845,   846,   890,  1809,
     263,   231,   223,   852,    49,  1225,    51,   856,    44,   901,
     286,   241,   932,   287,  1824,   904,    23,  1234,   867,   122,
     869,    28,     6,   281,   282,   283,   284,   876,   174,   878,
     288,  1841,   286,  1327,   288,   884,   124,   195,   196,   197,
     286,   930,   931,   286,    80,   291,   966,   967,   291,   969,
    1877,  1878,   156,   932,   946,   904,   124,  1936,   937,  1938,
     232,  1525,    46,  1877,  1878,   223,   224,  1946,   286,    23,
     288,   231,   232,  1077,    28,   954,   281,   282,   283,   284,
    1446,   930,   931,   288,    79,  1451,  1303,   966,   967,    98,
     969,    19,  1386,   281,   282,   283,   284,   180,   973,   948,
     288,   286,   203,   288,   205,    23,   948,  1111,   983,   145,
      28,  1921,  1991,   198,  1924,   281,   282,   283,   284,   155,
     199,   200,   288,   132,   973,   134,   192,  1637,   281,   282,
     283,   284,  1610,  1611,   983,   288,  1610,  1611,   281,   282,
     283,   284,   991,   992,   180,   288,    23,   156,   287,   991,
    1960,    28,   130,   131,   281,   282,   283,   284,    23,  1781,
    1781,   288,     6,    28,  1963,  1964,  1487,  1488,   286,   266,
     288,    23,   181,   259,  1495,  1496,    28,  1498,  1499,   286,
     286,   288,   288,   162,   199,  1506,  1507,  1508,  1509,    10,
    1511,  1512,  1513,  1514,  1515,  1516,   286,   286,   288,   288,
      44,    59,   286,  1435,   288,    23,   286,  1553,   288,   286,
      28,   288,    56,  1062,    61,  1463,   281,   282,   283,   284,
      23,  1210,  1111,   288,  1352,    28,    70,  1355,    79,   286,
    1062,   288,    78,  1465,     6,   286,    80,   288,   190,   149,
     281,   282,   283,   284,   775,   286,   777,  1757,   172,   141,
     781,   782,   783,    23,    82,   287,  1105,  1106,    28,   286,
    1152,   288,    23,  1525,    63,    61,  1525,    28,    61,   286,
    1119,   288,    44,   122,   281,   282,   283,   284,  1013,   122,
    1015,   288,  1017,  1018,    56,  1020,  1021,  1022,   286,   286,
     288,   288,   286,    17,   288,   286,   396,   288,   398,    23,
    1220,   145,   244,  1381,    28,  1383,   174,    31,    80,   188,
     102,   155,   172,  1233,  1936,  1936,  1938,  1938,   123,  1239,
     156,    65,   243,    83,  1946,  1946,   121,   281,   282,   283,
     284,   155,   254,    30,   288,   255,   180,  1341,  1227,   254,
      52,  1220,  1231,    67,   207,  1349,   286,   192,   278,   112,
    1199,    75,    29,   287,  1233,    10,    23,   135,   202,    83,
    1239,    28,  1204,   281,   282,   283,   284,  1216,   286,  1991,
    1991,   287,   199,   145,  1366,  1367,  1368,   101,  1227,   103,
      37,   287,  1231,   155,   287,   287,  1896,   287,   287,   287,
     287,    48,   116,  1903,   118,   119,   120,   287,    55,   287,
     287,   199,    59,   287,   281,   282,   283,   284,   180,    23,
    1399,   288,  1401,   199,    28,   287,   281,   282,   283,   284,
     287,    97,    79,   288,   290,   287,   287,  1659,   287,   281,
     282,   283,   284,   287,   287,   148,   288,    23,   286,  1356,
     278,    10,    28,   167,    63,   287,   287,   141,   166,    63,
    1299,  1348,   292,   292,   287,   286,  1345,  1357,  1299,  1348,
    1349,  1354,  1694,   281,   282,   283,   284,     0,   278,   286,
     288,    10,  1461,  1353,   287,    83,   292,    10,   281,   282,
     283,   284,   286,    23,   141,   288,   292,    90,    28,  1951,
      90,    90,    22,  1342,    22,    43,  1345,    10,   111,  1348,
    1349,   174,   208,  1352,  1353,  1354,  1355,  1356,  1357,   136,
     174,   281,   282,   283,   284,   172,   136,   196,   288,   176,
     281,   282,   283,   284,  1373,  1374,    21,   288,    61,  1378,
      90,    64,   185,   185,    23,   287,   138,    91,    71,    28,
      73,   287,   290,    61,    77,    23,   287,   287,    10,    82,
      28,   122,    85,   149,   278,   279,   280,   281,   282,   283,
     284,   286,   128,    17,   121,   287,   290,   100,   292,    23,
    1574,   287,   189,    23,    28,   126,    61,    31,    28,   288,
      21,   114,  1474,    23,    42,   293,   174,  1479,    28,   122,
      17,   174,   202,   126,    60,   287,    23,   286,   190,  1441,
      23,    28,   135,    10,    31,    28,  1838,   186,   265,    83,
     240,   286,    43,    67,   281,   282,   283,   284,   151,   286,
      19,    75,   287,    27,   287,    86,    86,    22,   125,    83,
     156,  1635,    23,   125,   125,   154,   156,    28,    23,   287,
      67,    98,   287,    28,   287,   178,    23,   101,    75,   103,
      23,    28,   108,  1559,   287,    28,    83,   190,    63,   192,
     288,   292,   116,    54,   118,   119,   120,   281,   282,   283,
     284,   286,  1521,    23,   101,   286,   103,    54,    63,   184,
    1521,    16,   307,   107,    63,  1574,   124,    63,    61,   116,
     287,   118,   119,   120,    12,   281,   282,   283,   284,   286,
      71,   239,   288,   287,    10,   330,   239,   287,   160,    10,
    1559,   288,   288,   167,   339,   289,   288,   192,  1567,  1568,
    1569,   288,   133,    52,   174,  1574,   259,    52,     8,   288,
     263,   210,   211,   212,   213,   214,   215,   216,   217,   288,
     167,   281,   282,   283,   284,   287,   287,    63,   288,    23,
     287,   287,   285,   286,    28,   288,   287,    51,    23,   173,
     287,  1610,  1611,    28,   287,   287,   287,   192,   127,   286,
     395,   287,   287,   287,   399,   287,   287,  1781,   287,   287,
     286,   288,    85,  1632,  1633,  1660,  1635,   288,    85,    19,
    1794,    73,   281,   282,   283,   284,   130,   145,   125,   288,
     288,  1793,   125,   281,   282,   283,   284,   196,    82,    22,
     288,  1660,   125,   125,   125,   125,   288,    82,   287,    63,
     288,   288,   286,   288,   278,   279,   280,   281,   282,   283,
     284,   281,   282,   283,   284,   288,   290,  1732,   292,   288,
      51,   281,   282,   283,   284,   173,   285,    36,   288,    36,
     285,   278,   279,   280,   281,   282,   283,   284,   281,   282,
     283,   284,    82,   290,    82,   292,   287,   285,   285,    51,
     285,    70,   285,   498,   174,  1822,   285,   285,    10,   285,
    1729,    85,   285,  1732,   285,   510,   511,   512,  1729,   285,
     281,   282,   283,   284,    23,   285,   281,   282,   283,   284,
      79,   285,   285,   285,   281,   282,   283,   284,   281,   282,
     283,   284,   180,   285,   289,  1764,   285,    80,   180,    44,
      80,    44,   288,   288,    94,   111,   124,   111,  1823,   189,
     288,   288,  1936,   288,  1938,   288,   288,  1776,   287,  1943,
      61,   288,  1946,  1818,   288,  1794,  1795,   288,   288,   288,
     288,   288,  1844,   124,   286,   288,  1848,   288,   583,    94,
      86,   287,   587,  1967,  1813,   204,   288,   288,  1915,  1818,
      79,   288,    17,   288,  1823,    82,   246,    82,    23,   287,
     287,   122,  1888,    28,   288,   287,    31,  1991,   286,   122,
    1994,   287,   173,   288,   122,   288,    47,    21,    44,   288,
     180,  2005,    50,    36,  2008,    47,   201,   281,   282,   283,
     284,  2015,     0,    37,     0,   578,   281,   282,   283,   284,
     159,  1870,    67,   159,    48,   316,  1364,   846,   580,   160,
      75,    55,   319,  1237,  1234,    59,    60,   837,    83,  1888,
    1889,   740,   986,   991,    68,  1226,  1678,   961,  1890,    80,
     686,   407,  1161,   441,  1943,    79,   101,   914,   103,   368,
    1227,   422,  1951,   878,  1657,  1659,  1231,  1677,   422,  1954,
    1958,   116,  1979,   118,   119,   120,  1957,    37,  1967,   904,
    1102,  1768,  1944,     0,   826,  1641,  1396,  1449,    48,   964,
      13,   666,   412,   352,  1943,    55,  1935,    27,   596,  1948,
     405,   956,  1951,   427,   600,  1994,   165,   723,    68,   133,
     867,   854,    90,  1119,  1963,  1964,  2005,   141,  1967,  2008,
    1382,  1729,   167,  1908,   773,   887,  2015,  1521,     0,     0,
       0,   756,   608,   844,  1114,  1405,  1278,   948,  1325,  1813,
    1216,    94,  1567,    -1,    -1,  1994,  1995,    -1,   172,    -1,
      -1,   776,   176,    -1,   664,   780,  2005,    -1,  2007,  2008,
      -1,    -1,   787,   187,    -1,   125,  2015,   792,   793,    -1,
      -1,   796,   797,   798,   799,   800,   801,    -1,    -1,    -1,
      -1,   141,   206,   207,    -1,    -1,    -1,   812,    -1,   814,
     815,   816,   817,   818,   819,   820,    -1,   157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,   176,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,   854,
      -1,    -1,    -1,   278,   279,   280,   281,   282,   283,   284,
      -1,   265,    -1,   288,   268,    -1,   206,    -1,    -1,    -1,
     875,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   887,     6,   889,    -1,     9,    -1,   198,    -1,
      13,   201,    -1,   203,   204,   205,   206,    20,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,   265,    -1,   237,   238,    -1,
     240,   241,    -1,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    66,   254,   255,    69,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,   287,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1008,  1009,  1010,    -1,    -1,  1013,  1014,
    1015,    -1,  1017,  1018,  1019,  1020,  1021,  1022,    -1,    -1,
      -1,    -1,    -1,    -1,  1029,  1030,    -1,  1032,    -1,  1034,
      -1,    -1,  1037,  1038,  1039,  1040,  1041,  1042,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,  1054,
      -1,    -1,  1057,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1072,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,  1113,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,     7,    -1,    -1,   281,   282,
      -1,    -1,    18,    17,   287,    21,   289,    -1,    -1,    23,
     293,    -1,    -1,    25,    28,    -1,    -1,    31,    34,    -1,
      -1,    33,    38,    -1,    -1,    41,    -1,    -1,    40,    -1,
      42,    -1,    44,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    75,    -1,    -1,    -1,    81,    -1,    -1,    80,    83,
      -1,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,  1263,   115,
      -1,    -1,   116,    -1,   118,   119,   120,    -1,    -1,    -1,
      -1,  1276,  1277,    18,  1279,  1280,    21,    -1,    -1,    -1,
      -1,    -1,  1287,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,   143,    -1,    38,    -1,    -1,    41,    -1,   150,    -1,
      -1,   153,  1307,   155,   156,   161,    -1,    -1,    -1,    -1,
    1315,  1316,  1317,   167,    -1,    -1,    -1,    -1,  1323,    -1,
    1325,  1326,  1327,  1328,    -1,    -1,    -1,   183,   180,    -1,
    1335,    -1,  1337,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,
      -1,     9,   218,   219,    -1,    13,    -1,    -1,   113,    -1,
     115,    -1,    20,    -1,    -1,    -1,    -1,   229,    -1,   235,
      -1,  1386,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,   244,   245,    -1,    -1,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1418,    -1,    -1,   161,   269,    66,    -1,
      -1,    69,    -1,    -1,   278,   279,   280,   281,   282,   283,
     284,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,   219,    -1,    -1,    -1,    -1,    -1,
      -1,  1486,    -1,    -1,    -1,  1487,  1488,  1492,    -1,    -1,
     235,    -1,  1497,  1495,  1496,    -1,  1498,  1499,    -1,  1504,
      -1,    -1,    -1,    -1,  1506,  1507,  1508,  1509,    -1,  1511,
    1512,  1513,  1514,  1515,  1516,   163,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,  1539,  1540,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1548,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,  1564,
     208,   209,    -1,    39,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
      -1,  1636,    -1,   281,   282,    -1,    -1,    -1,    -1,   287,
      -1,   289,    -1,    -1,    -1,   293,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     6,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1709,    -1,    -1,    -1,    -1,    -1,
    1715,  1716,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,  1739,    -1,    -1,    -1,    -1,  1744,
     216,   217,  1747,    -1,    94,    -1,    96,    97,    -1,  1754,
    1755,   227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,
     110,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,   181,    -1,    30,    -1,    -1,    -1,    -1,  1843,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    94,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,  1908,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,   165,    -1,
      -1,   281,   282,    -1,    -1,     6,    -1,   287,     9,   289,
    1945,    -1,    13,   293,   181,    -1,    -1,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,    32,   199,   200,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,   219,   220,   221,   222,   223,   224,   225,   226,
      -1,    -1,    -1,    -1,    -1,    66,   233,   234,    69,    -1,
      -1,   198,    -1,    -1,   201,   242,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,   282,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,   270,
     271,   272,    -1,   274,   275,     6,   277,    -1,     9,    -1,
     281,   282,    13,    -1,    -1,    -1,   287,    -1,   289,    20,
      -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    48,    -1,    -1,    -1,    38,    39,    -1,
      55,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,   141,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,   157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,
      -1,   176,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   206,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
      -1,    -1,    -1,    -1,    71,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
     265,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,   270,
     271,   272,    -1,   274,   275,     6,   277,    -1,     9,    -1,
     281,   282,    13,    -1,   141,    -1,   287,    -1,   289,    20,
      -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
     187,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
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
     271,   272,    -1,   274,   275,     6,   277,    -1,     9,    -1,
     281,   282,    13,    -1,    -1,    -1,   287,    -1,   289,    20,
      -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,   106,    -1,    -1,    -1,   110,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     287,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,
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
     271,   272,    -1,   274,   275,     6,   277,    -1,     9,    -1,
     281,   282,    13,    -1,    -1,    -1,   287,    -1,   289,    20,
      -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,    94,    -1,    96,    97,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,   106,    -1,    -1,    -1,   110,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
      -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   287,   163,   289,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,   200,
     201,    -1,   203,   204,   205,   206,    -1,   208,   209,    -1,
     108,    -1,    -1,    -1,    -1,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,    -1,   230,
      -1,    13,   233,   234,    -1,   236,   237,   238,    20,   240,
     241,   242,   243,    -1,    -1,   246,   247,   248,   249,   250,
      32,    -1,    -1,   254,   255,    -1,    38,    39,    -1,   260,
     261,   262,    -1,   264,   265,   266,   164,   268,    -1,   270,
     271,   272,    -1,   274,   275,    57,   277,    -1,    -1,    -1,
     281,   282,    -1,    -1,    66,    -1,   287,    69,   289,    -1,
      -1,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     198,    -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,
     208,    -1,    94,    -1,    96,    97,    -1,    -1,   216,   217,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,   110,   227,
     228,    -1,   230,    -1,    -1,    -1,   118,    -1,    -1,   237,
     238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,    -1,   155,    -1,    -1,    -1,   159,    -1,    -1,
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
     262,    -1,   264,   265,   266,    -1,   268,    13,   270,   271,
     272,    -1,   274,   275,    20,   277,    -1,    -1,    -1,   281,
     282,    -1,    -1,    -1,    -1,   287,    32,   289,    -1,    35,
      -1,   293,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,   118,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,    -1,   289,    -1,   165,
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
      57,   277,    -1,    -1,    -1,   281,   282,    -1,    -1,    66,
      -1,   287,    69,   289,    -1,   198,    -1,   293,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,   118,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,   287,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,
     237,   238,    20,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,
      38,    39,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   270,   271,   272,    -1,   274,   275,    57,
     277,    -1,    -1,    -1,   281,   282,    -1,   181,    66,    -1,
     287,    69,   289,    -1,    -1,    -1,   293,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    94,    -1,    96,    97,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,   227,   228,    -1,   230,    -1,    -1,    -1,
     118,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    13,   233,   234,    -1,   236,   237,
     238,    20,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    32,    -1,    -1,   254,   255,    -1,    38,
      39,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    -1,   274,   275,    -1,   277,
      -1,    -1,    -1,   281,   282,    -1,    -1,    66,    -1,   287,
      69,   289,    -1,   198,    -1,   293,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     285,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,   267,   268,
      -1,   270,   271,   272,   273,   274,   275,   276,   277,    13,
      -1,    -1,   281,   282,    -1,    -1,    20,    -1,   287,    -1,
     289,    -1,    -1,    -1,   293,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    69,   196,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,    -1,
     240,   241,    -1,   243,   118,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   282,    -1,    -1,   159,    -1,    -1,    -1,    -1,
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
      -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     275,    -1,   277,    -1,    -1,    -1,   281,   282,   283,    -1,
      66,    -1,   287,    69,   289,    -1,   198,    -1,   293,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,   195,
     196,   197,   198,   199,   200,   201,    -1,   203,   204,   205,
     206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,    -1,   230,    -1,    13,   233,   234,    -1,
     236,   237,   238,    20,   240,   241,   242,   243,    -1,    -1,
     246,   247,   248,   249,   250,    32,    -1,    -1,   254,   255,
      -1,    38,    39,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,   270,   271,   272,    -1,   274,   275,
      -1,   277,    -1,    -1,    -1,   281,   282,    -1,    -1,    66,
      -1,   287,    69,   289,    -1,    -1,    -1,   293,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    13,   233,   234,    -1,   236,
     237,   238,    20,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    32,    -1,    -1,   254,   255,    -1,
      38,    39,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   270,   271,   272,    -1,   274,   275,    -1,
     277,    -1,    -1,    -1,   281,   282,    -1,    -1,    66,    -1,
     287,    69,   289,    -1,    -1,    -1,   293,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    -1,   270,   271,   272,    13,   274,   275,    -1,   277,
      -1,    -1,    20,   281,   282,    -1,    -1,    -1,    -1,   287,
     288,   289,    -1,    -1,    32,   293,    -1,    -1,    -1,    -1,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   192,    -1,    -1,   195,   196,   197,
     198,   199,   200,   201,    -1,   203,   204,   205,   206,    -1,
     208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,
     238,    -1,   240,   241,   242,   243,    -1,    -1,   246,   247,
     248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,
      -1,    -1,   260,   261,   262,    -1,   264,   265,   266,    -1,
     268,    13,   270,   271,   272,    -1,   274,   275,    20,   277,
      -1,    -1,    -1,   281,   282,    -1,    -1,    -1,    -1,   287,
      32,   289,    -1,    -1,    -1,   293,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    13,
     233,   234,    -1,   236,   237,   238,    20,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    32,    -1,
      -1,   254,   255,    -1,    38,    39,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      -1,   274,   275,    -1,   277,    -1,    -1,    -1,   281,   282,
      -1,    -1,    66,    -1,   287,    69,   289,    -1,    -1,    -1,
     293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     274,   275,    -1,   277,    -1,    -1,    -1,   281,   282,    -1,
      -1,    66,    -1,   287,    69,   289,    -1,    -1,    -1,   293,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,    -1,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,   145,   254,
     255,    -1,    16,    -1,    -1,   260,   261,   262,   155,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,    -1,   277,    -1,    -1,    -1,   281,   282,    -1,    -1,
      44,    -1,   287,   180,   289,    -1,    -1,    51,   293,    -1,
      -1,    55,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,   206,
      74,   208,    -1,    -1,    -1,    -1,    80,    -1,    -1,   216,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      16,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,   155,    -1,    -1,    -1,    51,    -1,    -1,    -1,    55,
      56,    -1,    58,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    80,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,   228,    -1,   230,    -1,    -1,    -1,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,   137,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,   256,   257,   258,    16,   260,   261,   262,   155,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    -1,    58,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    74,    -1,    -1,    -1,    -1,    -1,    80,
     216,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   227,   228,    -1,   230,    -1,    -1,    -1,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,   137,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,
     201,    -1,   203,   204,   205,   206,    -1,   208,    18,    -1,
      -1,    21,    -1,    -1,    -1,   216,   217,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    38,   230,
      -1,    41,    -1,    -1,    -1,    -1,   237,   238,    -1,   240,
     241,    -1,   243,    -1,    -1,   246,   247,   248,   249,   250,
      -1,    -1,    -1,   254,   255,   256,   257,   258,    -1,   260,
     261,   262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,    -1,   115,    18,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,   183,    -1,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    -1,    -1,    -1,    -1,   198,    -1,
      -1,   201,    -1,   203,   204,   205,   206,    -1,   208,    -1,
      -1,   113,    -1,   115,    -1,    -1,   216,   217,   218,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,   228,    -1,
     230,    -1,    -1,    -1,    -1,   235,    -1,   237,   238,    -1,
     240,   241,   144,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,   161,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,   235,    -1,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,   198,   268,    -1,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
      -1,    -1,   225,   226,   227,   228,   229,   230,    -1,    -1,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

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
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short DSQL_yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


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
#define YYABORT 	goto yyabortlab
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
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)
#endif /* !YYPURE */


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
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
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

#ifdef YYERROR_VERBOSE

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
#endif



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

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\

int DSQL_yychar;
						\
							\
/* The semantic value of the lookahead symbol. */	\
static YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
static int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
dsql_yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

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

  /* The state stack. */
  short	DSQL_yyssa[YYINITDEPTH];
  short *DSQL_yyss = DSQL_yyssa;
  register short *DSQL_DSQL_yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, DSQL_DSQL_yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, DSQL_DSQL_yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
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
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
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
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &DSQL_yyss1, yysize * sizeof (*DSQL_DSQL_yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &DSQL_yyss1, yysize * sizeof (*DSQL_DSQL_yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
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
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (DSQL_yyss1 != DSQL_yyssa)
	  YYSTACK_FREE (DSQL_yyss1);
      }
# endif
#endif /* no yyoverflow */

      DSQL_DSQL_yyssp = DSQL_yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

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
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* DSQL_yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (DSQL_yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      DSQL_yychar = YYLEX;
    }

  /* Convert token to internal form (in DSQL_yychar1) for indexing tables with */

  if (DSQL_yychar <= 0)		/* This means end of input. */
    {
      DSQL_yychar1 = 0;
      DSQL_yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      DSQL_yychar1 = YYTRANSLATE (DSQL_yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (lex.dsql_debug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     DSQL_yychar, yytname[DSQL_yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, DSQL_yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += DSQL_yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != DSQL_yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      DSQL_yychar, yytname[DSQL_yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (DSQL_yychar != YYEOF)
    DSQL_yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (lex.dsql_debug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

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
case 150:
{ yyval = NULL; }
    break;
case 151:
{ yyval = NULL; }
    break;
case 154:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 155:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
    break;
case 158:
{ yyval = make_node (nod_null, (int) 0, NULL); }
    break;
case 159:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 160:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
    break;
case 161:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
    break;
case 162:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
    break;
case 165:
{ yyval = (dsql_nod*) yyvsp[0]; }
    break;
case 166:
{yyval = NULL;}
    break;
case 169:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 170:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
    break;
case 171:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
    break;
case 172:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
    break;
case 173:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
    break;
case 174:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
    break;
case 175:
{yyval = NULL;}
    break;
case 178:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 180:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
    break;
case 181:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
    break;
case 182:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
    break;
case 183:
{ lex.g_file  = make_file();}
    break;
case 188:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
    break;
case 189:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
    break;
case 196:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 197:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
    break;
case 198:
{ yyval = yyvsp[0]; }
    break;
case 199:
{ yyval = yyvsp[0]; }
    break;
case 200:
{ yyval = NULL; }
    break;
case 202:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 205:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
    break;
case 206:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 207:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 208:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
    break;
case 211:
{ yyval = NULL; }
    break;
case 212:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 213:
{ yyval = yyvsp[0]; }
    break;
case 214:
{ yyval = NULL; }
    break;
case 215:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 216:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
    break;
case 217:
{ yyval = yyvsp[-1]; }
    break;
case 218:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 219:
{ yyval = yyvsp[0]; }
    break;
case 220:
{ yyval = NULL; }
    break;
case 227:
{ yyval = NULL; }
    break;
case 230:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 231:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 232:
{ yyval = make_node (nod_null, (int) 1, NULL); }
    break;
case 233:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 235:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
    break;
case 236:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
    break;
case 237:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
    break;
case 238:
{ yyval = yyvsp[0]; }
    break;
case 239:
{ yyval = NULL ;}
    break;
case 244:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 245:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
    break;
case 246:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 247:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
    break;
case 248:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
    break;
case 249:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
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
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
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
{ yyval = (dsql_nod*) NULL; }
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
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
    break;
case 293:
{ yyval = yyvsp[-1]; }
    break;
case 294:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
    break;
case 295:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
    break;
case 296:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
    break;
case 298:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 313:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
    break;
case 314:
{ yyval = make_node (nod_exit, 0, NULL); }
    break;
case 319:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
    break;
case 320:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
    break;
case 321:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
    break;
case 322:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
    break;
case 323:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
    break;
case 324:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
    break;
case 325:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
    break;
case 326:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
    break;
case 327:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
    break;
case 328:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
    break;
case 329:
{ yyval = NULL; }
    break;
case 330:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
    break;
case 331:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
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
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
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
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 361:
{ yyval = make_list (yyvsp[0]); }
    break;
case 362:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 363:
{ yyval = NULL; }
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
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
    break;
case 368:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 369:
{ yyval = NULL; }
    break;
case 371:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 372:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
    break;
case 373:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 374:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 375:
{ lex.beginning = lex_position(); }
    break;
case 376:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
    break;
case 377:
{ lex.beginning = lex.last_token; }
    break;
case 378:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
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
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 382:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
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
case 417:
{ yyval = yyvsp[0]; }
    break;
case 418:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
    break;
case 420:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 421:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
    break;
case 422:
{ yyval = yyvsp[0]; }
    break;
case 423:
{ yyval = yyvsp[0]; }
    break;
case 424:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
    break;
case 425:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
    break;
case 426:
{ yyval = yyvsp[0]; }
    break;
case 427:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
    break;
case 429:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 430:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 431:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
    break;
case 432:
{ yyval = yyvsp[0]; }
    break;
case 433:
{ yyval = yyvsp[0]; }
    break;
case 434:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 435:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 436:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
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
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
    break;
case 602:
{ yyval = make_node (nod_rollback, 0, NULL); }
    break;
case 605:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
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
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
    break;
case 794:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 797:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 798:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
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
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 805:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 806:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 807:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 808:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 809:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 810:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 811:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 812:
{ yyval = NULL; }
    break;
case 815:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 816:
{ yyval = yyvsp[0]; }
    break;
case 817:
{ yyval = NULL; }
    break;
case 818:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 821:
{ yyval = NULL; }
    break;
case 822:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 824:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 826:
{ yyval = NULL; }
    break;
case 827:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 829:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 831:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 832:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 833:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 835:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 837:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 839:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 840:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 842:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 843:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 844:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 845:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 846:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 848:
{ yyval = yyvsp[-1]; }
    break;
case 849:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 861:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 862:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 863:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 864:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 865:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 866:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 867:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 868:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 869:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 870:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 871:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 872:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 873:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 874:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 875:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 876:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 877:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 878:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 879:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 880:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 881:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 882:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 883:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 884:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 887:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 888:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 889:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 890:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 891:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 892:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 893:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 894:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 895:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 896:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 897:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 898:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 899:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 900:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 901:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 902:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 903:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 904:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 905:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 906:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 907:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 908:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 909:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
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
case 914:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 915:
{ yyval = yyvsp[-1]; }
    break;
case 926:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 927:
{ yyval = yyvsp[0]; }
    break;
case 928:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 929:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 930:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 932:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 933:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 934:
{ yyval = yyvsp[-1]; }
    break;
case 935:
{ yyval = yyvsp[-1]; }
    break;
case 939:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 940:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 941:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 944:
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
case 945:
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
case 946:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 947:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 949:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 951:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 952:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 953:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 954:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 955:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 956:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 958:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 959:
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
case 960:
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
case 961:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 962:
{ yyval = make_parameter (); }
    break;
case 963:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 964:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 965:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 966:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 967:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 968:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 969:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 970:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 971:
{ yyval = yyvsp[0]; }
    break;
case 972:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 974:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 975:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 976:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 977:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 978:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 980:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 981:
{ yyval = yyvsp[0];}
    break;
case 985:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 986:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 987:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 988:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 989:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 990:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 991:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 992:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 993:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 994:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 995:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 998:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 1002:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1003:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1004:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1005:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1008:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 1009:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
    break;
case 1010:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 1011:
{ yyval = yyvsp[0]; }
    break;
case 1012:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 1013:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
    break;
case 1014:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
    break;
case 1015:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
    break;
case 1016:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
    break;
case 1017:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
    break;
case 1018:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
    break;
case 1019:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
    break;
case 1020:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1021:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 1022:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 1025:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 1026:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 1027:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1030:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1031:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1032:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1033:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1034:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1035:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1036:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1037:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1041:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 1042:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 1043:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 1044:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 1045:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 1046:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 1047:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 1048:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 1049:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 1050:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 1055:
{ yyval = make_node (nod_null, 0, NULL); }
    break;
}



  yyvsp -= yylen;
  DSQL_DSQL_yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

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
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *DSQL_DSQL_yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *DSQL_DSQL_yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (DSQL_yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (DSQL_yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
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
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (DSQL_yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  DSQL_yychar, yytname[DSQL_yychar1]));
      DSQL_yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (DSQL_DSQL_yyssp == DSQL_yyss)
    YYABORT;
  yyvsp--;
  yystate = *--DSQL_DSQL_yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

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

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

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
			isc_arg_gds, isc_command_end_err2,	/* Unexpected end of command */
			isc_arg_number, lex.lines,
			isc_arg_number, lex.last_token - lex.line_start + 1,
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
