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
# define	RETURNING	532


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



#define	YYFINAL		2032
#define	YYFLAG		-32768
#define	YYNTBASE	295

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 532 ? yytranslate[x] : 752)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     288,   289,   284,   282,   287,   283,   293,   285,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   290,   286,
     280,   279,   281,   294,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   291,     2,   292,     2,     2,     2,     2,     2,     2,
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
     276,   277,   278
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
    1357,  1361,  1365,  1368,  1371,  1376,  1381,  1386,  1392,  1397,
    1399,  1401,  1403,  1405,  1407,  1409,  1411,  1413,  1415,  1417,
    1419,  1421,  1423,  1425,  1427,  1429,  1431,  1433,  1435,  1437,
    1439,  1441,  1443,  1445,  1447,  1449,  1451,  1453,  1455,  1457,
    1459,  1461,  1463,  1465,  1467,  1469,  1471,  1473,  1475,  1477,
    1479,  1481,  1483,  1486,  1489,  1492,  1494,  1496,  1498,  1499,
    1502,  1505,  1510,  1515,  1521,  1525,  1528,  1529,  1532,  1533,
    1534,  1536,  1539,  1542,  1547,  1551,  1554,  1557,  1565,  1567,
    1568,  1570,  1571,  1574,  1577,  1580,  1583,  1586,  1589,  1592,
    1595,  1598,  1602,  1605,  1608,  1611,  1614,  1616,  1618,  1620,
    1622,  1627,  1633,  1635,  1639,  1641,  1645,  1647,  1650,  1652,
    1654,  1656,  1658,  1660,  1662,  1664,  1666,  1668,  1670,  1672,
    1677,  1682,  1689,  1695,  1699,  1700,  1703,  1706,  1707,  1711,
    1712,  1717,  1719,  1725,  1730,  1732,  1737,  1739,  1742,  1745,
    1747,  1749,  1751,  1754,  1757,  1760,  1763,  1764,  1768,  1774,
    1776,  1778,  1781,  1785,  1787,  1790,  1794,  1795,  1797,  1799,
    1801,  1807,  1813,  1820,  1822,  1824,  1826,  1829,  1834,  1836,
    1837,  1843,  1845,  1846,  1850,  1854,  1856,  1857,  1860,  1861,
    1863,  1864,  1868,  1870,  1871,  1873,  1876,  1878,  1880,  1882,
    1884,  1887,  1890,  1892,  1895,  1899,  1901,  1903,  1907,  1911,
    1913,  1916,  1920,  1922,  1925,  1926,  1929,  1931,  1933,  1934,
    1936,  1938,  1940,  1944,  1947,  1951,  1952,  1954,  1958,  1963,
    1969,  1976,  1984,  1986,  1988,  1990,  1992,  1994,  1996,  1999,
    2001,  2003,  2005,  2007,  2009,  2011,  2014,  2016,  2018,  2020,
    2023,  2025,  2027,  2031,  2035,  2036,  2039,  2040,  2043,  2044,
    2048,  2052,  2056,  2058,  2063,  2068,  2070,  2080,  2081,  2082,
    2083,  2084,  2088,  2091,  2093,  2094,  2098,  2104,  2108,  2111,
    2117,  2120,  2122,  2124,  2126,  2128,  2130,  2134,  2136,  2140,
    2142,  2143,  2146,  2148,  2152,  2154,  2156,  2158,  2160,  2164,
    2171,  2173,  2174,  2178,  2179,  2181,  2185,  2187,  2189,  2191,
    2196,  2202,  2208,  2210,  2212,  2215,  2220,  2225,  2228,  2232,
    2233,  2235,  2238,  2240,  2242,  2245,  2248,  2251,  2252,  2254,
    2255,  2259,  2260,  2262,  2266,  2268,  2271,  2272,  2275,  2276,
    2279,  2280,  2285,  2287,  2290,  2292,  2294,  2295,  2297,  2301,
    2304,  2306,  2308,  2311,  2313,  2318,  2322,  2324,  2328,  2333,
    2334,  2338,  2339,  2341,  2345,  2349,  2351,  2353,  2354,  2356,
    2358,  2363,  2364,  2367,  2372,  2373,  2383,  2389,  2391,  2393,
    2401,  2406,  2408,  2410,  2419,  2425,  2428,  2433,  2434,  2439,
    2441,  2445,  2449,  2451,  2459,  2467,  2473,  2477,  2478,  2480,
    2482,  2484,  2487,  2488,  2490,  2492,  2494,  2495,  2499,  2501,
    2505,  2507,  2508,  2512,  2514,  2518,  2520,  2524,  2528,  2530,
    2532,  2536,  2538,  2541,  2543,  2547,  2551,  2553,  2556,  2560,
    2564,  2568,  2572,  2574,  2578,  2581,  2583,  2585,  2587,  2589,
    2591,  2593,  2595,  2597,  2599,  2601,  2603,  2607,  2611,  2615,
    2619,  2623,  2627,  2631,  2635,  2642,  2649,  2656,  2663,  2670,
    2677,  2684,  2691,  2698,  2705,  2712,  2719,  2726,  2733,  2740,
    2747,  2749,  2751,  2757,  2764,  2770,  2777,  2781,  2786,  2792,
    2799,  2803,  2808,  2812,  2817,  2821,  2826,  2831,  2837,  2842,
    2847,  2851,  2856,  2858,  2860,  2862,  2864,  2866,  2868,  2870,
    2874,  2878,  2880,  2882,  2884,  2886,  2888,  2890,  2892,  2894,
    2896,  2898,  2901,  2904,  2908,  2912,  2916,  2920,  2924,  2928,
    2932,  2936,  2938,  2940,  2942,  2944,  2948,  2950,  2952,  2954,
    2956,  2958,  2960,  2965,  2967,  2971,  2973,  2976,  2978,  2980,
    2982,  2984,  2986,  2988,  2990,  2993,  2996,  2999,  3001,  3003,
    3005,  3007,  3009,  3011,  3013,  3015,  3017,  3019,  3022,  3024,
    3027,  3029,  3031,  3033,  3035,  3037,  3040,  3042,  3044,  3046,
    3048,  3053,  3059,  3065,  3071,  3077,  3083,  3089,  3095,  3101,
    3107,  3113,  3115,  3117,  3124,  3126,  3128,  3130,  3135,  3140,
    3145,  3150,  3152,  3154,  3159,  3164,  3172,  3175,  3176,  3184,
    3191,  3198,  3203,  3205,  3207,  3209,  3214,  3218,  3225,  3227,
    3229,  3236,  3245,  3252,  3254,  3256,  3261,  3268,  3273,  3279,
    3283,  3289,  3294,  3300,  3302,  3304,  3306,  3311,  3318,  3320,
    3322,  3324,  3326,  3328,  3330,  3332,  3334,  3336,  3337,  3339,
    3340,  3342,  3344,  3346,  3348,  3350,  3352,  3354,  3356,  3358,
    3360,  3362,  3364,  3366,  3368,  3370,  3372,  3374,  3376,  3378,
    3380,  3382,  3384,  3386,  3388,  3390,  3392,  3394,  3396,  3398,
    3400,  3402,  3404,  3406,  3408,  3410,  3412,  3414,  3416,  3418,
    3420,  3422,  3424,  3426,  3428,  3430,  3432,  3434,  3436,  3438,
    3440,  3442,  3444,  3446,  3448,  3450,  3452,  3454,  3456
};
static const short yyrhs[] =
{
     296,     0,   296,   286,     0,   479,     0,   641,     0,   559,
       0,   536,     0,   331,     0,   318,     0,   630,     0,   504,
       0,   297,     0,   629,     0,   457,     0,   460,     0,   333,
       0,   335,     0,   306,     0,   537,     0,   530,     0,   565,
       0,   528,     0,   633,     0,    40,   688,     0,    70,   299,
     122,   298,   605,   174,   309,   303,     0,    70,   301,   122,
     141,   305,   174,   309,   303,     0,    70,   314,   174,   316,
     304,     0,   172,     0,     0,     6,     0,     6,   140,     0,
     300,     0,   302,     0,   300,   287,   302,     0,    56,     0,
     155,     0,    80,     0,    44,     0,   180,   647,     0,   145,
     647,     0,   192,    70,   124,     0,     0,   192,   202,   124,
       0,     0,   741,     0,   150,   307,   299,   122,   298,   605,
      63,   309,     0,   150,   307,   301,   122,   141,   305,    63,
     309,     0,   150,   308,   314,    63,   316,     0,    70,   124,
      61,     0,     0,   202,   124,    61,     0,     0,   310,     0,
     312,     0,   311,     0,   310,   287,   311,     0,   310,   287,
     313,     0,   312,   287,   311,     0,   141,   741,     0,   176,
     745,     0,   187,   748,     0,   206,   742,     0,   313,     0,
     312,   287,   313,     0,   746,     0,   181,   746,     0,    71,
     746,     0,   315,     0,   314,   287,   315,     0,   742,     0,
     317,     0,   316,   287,   317,     0,   746,     0,   181,   746,
       0,    42,   319,     0,    60,   329,     0,    59,    65,   320,
       0,   725,   322,   149,   326,    52,   687,   111,   687,     0,
     511,     0,    18,     0,    34,   288,   691,   289,   517,     0,
       0,   323,     0,   288,   323,   289,     0,   324,     0,   323,
     287,   324,     0,   389,   321,   325,     0,     0,    19,   230,
       0,    19,   262,     0,    94,     0,   327,     0,   288,   327,
     289,     0,   389,   321,   328,     0,   133,   691,     0,     0,
      19,    97,     0,    19,   230,     0,   204,     0,    19,   230,
     204,     0,   734,    78,   330,   128,   330,    52,   687,   111,
     687,     0,   726,     0,   688,     0,    33,   332,     0,    55,
     337,     0,   341,   625,    79,   737,   122,   605,   342,     0,
     141,   408,     0,   172,   376,     0,   176,   471,     0,   187,
     464,     0,    68,   358,     0,   265,   358,     0,    37,   360,
       0,    48,   349,     0,   157,   343,     0,   206,   359,     0,
     229,   334,     0,   141,   409,     0,   172,   377,     0,   187,
     465,     0,    55,   338,     0,    33,   125,     7,   336,     0,
     141,   410,     0,   176,   472,     0,    55,   339,     0,   733,
     687,     0,   733,   687,     0,   733,   687,     0,   733,   687,
       0,   179,     0,     0,   649,     0,   648,     0,   383,   288,
     468,   676,   469,   289,     0,   691,   344,   345,   687,   346,
     347,     0,   105,     0,    14,     0,     0,     0,    29,     0,
       0,    98,   361,   694,   375,     0,     0,   348,     0,   369,
       0,   348,   369,     0,   386,   350,   506,   468,   351,   469,
     352,   385,     0,    10,     0,     0,    43,   468,   391,     0,
       0,     0,   353,     0,   354,     0,   353,   354,     0,   355,
       0,   356,     0,   357,     0,   118,    94,     0,   468,    22,
     288,   656,   289,   469,     0,   736,     0,   742,     0,   362,
     363,   366,     0,     0,   279,     0,   687,     0,     0,   364,
       0,   365,     0,   364,   365,     0,   132,   361,   691,     0,
      98,   361,   694,   375,     0,   181,   687,     0,   134,   687,
       0,   156,   112,   687,     0,     0,   367,     0,   368,     0,
     367,   368,     0,   369,     0,    43,    21,   156,   727,     0,
     255,    90,   687,     0,   370,   687,   371,     0,    90,     0,
       0,   372,     0,   373,     0,   372,   373,     0,   167,   374,
     694,     0,    98,   361,   694,   375,     0,     0,    12,     0,
      12,   130,     0,     0,   130,     0,   131,     0,   605,   378,
     288,   379,   289,     0,   605,   378,   288,   379,   289,     0,
      59,    90,   687,     0,    59,   687,     0,     0,   380,     0,
     379,   287,   380,     0,   381,     0,   396,     0,   386,   384,
     351,   469,   392,   385,     0,   386,   507,   382,     0,   386,
     382,     0,   383,   288,   468,   676,   469,   289,     0,    27,
      19,     0,    27,     0,   506,   468,     0,   654,   466,     0,
      23,   728,     0,     0,   654,     0,   654,     0,   389,   506,
       0,     0,    43,   391,     0,     0,   680,     0,   684,     0,
     685,     0,   686,     0,   724,     0,   677,     0,     0,   393,
       0,   394,     0,   393,   394,     0,   397,   395,     0,   118,
      94,     0,   145,   605,   647,   404,   402,     0,   403,     0,
     179,   402,     0,   139,    86,   402,     0,   397,   398,     0,
      30,   730,     0,     0,   399,     0,   400,     0,   401,     0,
     403,     0,   179,   648,   402,     0,   139,    86,   648,   402,
       0,    62,    86,   648,   145,   605,   647,   404,   402,     0,
     239,   625,    79,   737,     0,     0,   468,    22,   288,   656,
     289,   469,     0,   405,     0,   406,     0,   406,   405,     0,
     405,   406,     0,     0,   122,   180,   407,     0,   122,    44,
     407,     0,   203,     0,   156,    43,     0,   156,    94,     0,
     117,   201,     0,   741,   412,   413,    10,   466,   419,   428,
     469,     0,   741,   412,   413,    10,   466,   419,   428,   469,
       0,   741,   412,   413,    10,   466,   419,   428,   469,     0,
     741,   412,   413,    10,   466,   419,   428,   469,     0,   288,
     414,   289,     0,     0,   149,   288,   416,   289,     0,     0,
     415,     0,   414,   287,   415,     0,   387,   507,   418,   469,
       0,   417,     0,   416,   287,   417,     0,   387,   507,     0,
      43,   466,   391,     0,   279,   466,   391,     0,   466,     0,
     420,     0,     0,   421,     0,   420,   421,     0,    42,   424,
     422,   286,     0,   423,     0,   426,     0,   386,   507,   425,
       0,   184,     0,     0,   279,   391,     0,   390,     0,   731,
      36,    61,   288,   565,   289,     0,   431,     0,   428,     0,
      16,   429,    51,     0,   430,     0,   430,   448,     0,     0,
     427,     0,   430,   427,     0,   639,   286,     0,   629,   286,
       0,   633,   286,     0,   630,   286,     0,   441,   286,     0,
     457,   286,     0,   434,   286,     0,   437,   286,     0,   640,
     286,     0,   432,   286,     0,   433,   286,     0,   439,   286,
       0,   452,   286,     0,   446,   286,     0,   170,   286,     0,
      58,   286,     0,   438,     0,   444,     0,   435,     0,   436,
       0,    55,   733,     0,    55,   733,   676,     0,    55,     0,
      56,   246,   676,     0,   445,    61,   565,    82,   443,   447,
      47,   427,     0,   445,    61,    56,   246,   676,    82,   443,
      47,   427,     0,    56,   246,   676,    82,   443,     0,    74,
     288,   656,   289,   173,   427,    50,   427,     0,    74,   288,
     656,   289,   173,   427,     0,   137,   676,   440,     0,     0,
     565,    82,   443,     0,   290,   747,     0,   442,     0,   653,
       0,   443,   287,   653,     0,   443,   287,   442,     0,   445,
     191,   288,   656,   289,    47,   427,     0,   739,   290,     0,
       0,   227,     0,   247,     0,   247,   739,     0,    10,    36,
     731,     0,     0,   449,     0,   448,   449,     0,   189,   450,
      47,   427,     0,   451,     0,   450,   287,   451,     0,   165,
     688,     0,    66,   735,     0,    55,   733,     0,     9,     0,
     453,     0,   455,     0,   454,     0,   256,   731,     0,   257,
     731,     0,   258,   456,   731,    82,   443,     0,     0,    56,
     141,   741,   458,   459,     0,   679,     0,   288,   679,   289,
       0,     0,   148,   443,     0,   148,   288,   443,   289,     0,
       0,    56,   260,   461,   413,    10,   419,   428,     0,   288,
     462,   289,     0,     0,   463,     0,   462,   287,   463,     0,
     417,   279,   683,     0,   748,   647,    10,   466,   569,   470,
     467,     0,   748,   647,    10,   466,   569,   470,   467,     0,
       0,     0,     0,     0,   192,    22,   124,     0,     0,   745,
      61,   605,   473,   474,   477,   468,   478,   469,     0,   745,
      61,   605,   473,   474,   477,   468,   478,   469,     0,     3,
       0,    76,     0,     0,   475,   476,     0,    15,     0,     5,
       0,    80,     0,   180,     0,    44,     0,    80,   125,   180,
       0,    80,   125,    44,     0,   180,   125,    80,     0,   180,
     125,    44,     0,    44,   125,    80,     0,    44,   125,   180,
       0,    80,   125,   180,   125,    44,     0,    80,   125,    44,
     125,   180,     0,   180,   125,    80,   125,    44,     0,   180,
     125,    44,   125,    80,     0,    44,   125,    80,   125,   180,
       0,    44,   125,   180,   125,    80,     0,   136,   689,     0,
       0,    10,   468,   419,   428,     0,     7,   480,     0,    55,
     340,     0,   172,   605,   485,     0,   176,   501,     0,   141,
     411,     0,    37,   498,   499,     0,    48,   487,   483,     0,
      79,   493,     0,   265,   494,     0,    59,    65,   495,     0,
      43,   468,   391,     0,    22,   468,   288,   656,   289,   469,
       0,   484,     0,   483,   484,     0,   156,   468,   481,   469,
       0,     4,    30,   482,     0,     4,   357,     0,    49,    43,
       0,    49,    30,     0,   174,   654,     0,   208,   389,   507,
       0,   486,     0,   485,   287,   486,     0,    49,   654,   492,
       0,    49,    30,   730,     0,     4,   381,     0,     4,   396,
       0,   489,   654,   136,   691,     0,   489,   487,   174,   654,
       0,   489,   491,   208,   490,     0,   489,   491,   156,   481,
     469,     0,   489,   491,    49,    43,     0,   488,     0,   750,
       0,   202,     0,   207,     0,   209,     0,   210,     0,   211,
       0,   212,     0,   213,     0,   214,     0,   215,     0,   218,
       0,   219,     0,   220,     0,   221,     0,   222,     0,   225,
       0,   226,     0,   229,     0,   233,     0,   234,     0,   235,
       0,   236,     0,   245,     0,   242,     0,   244,     0,   256,
       0,   257,     0,   258,     0,   259,     0,   239,     0,   263,
       0,   270,     0,   267,     0,   271,     0,   272,     0,   269,
       0,   273,     0,   274,     0,   275,     0,   276,     0,   277,
       0,     7,     0,     7,   207,     0,   507,   468,     0,   654,
     466,     0,   654,     0,   205,     0,   203,     0,     0,   737,
       3,     0,   737,    76,     0,   736,   266,   192,   693,     0,
     736,   266,   192,   223,     0,   736,   266,   192,   283,   223,
       0,   725,   496,   497,     0,    52,   687,     0,     0,   111,
     687,     0,     0,     0,   500,     0,   499,   500,     0,     4,
     348,     0,     4,   255,    90,   687,     0,    49,   255,    90,
       0,    16,   254,     0,    51,   254,     0,   745,   473,   502,
     477,   468,   503,   469,     0,   474,     0,     0,   478,     0,
       0,    49,   505,     0,    55,   733,     0,    79,   737,     0,
     141,   741,     0,   172,   744,     0,   176,   745,     0,   187,
     748,     0,    60,   734,     0,    48,   732,     0,    59,    65,
     725,     0,   157,   691,     0,   206,   742,     0,    68,   736,
       0,   265,   736,     0,   507,     0,   508,     0,   511,     0,
     514,     0,   512,   291,   509,   292,     0,   519,   291,   509,
     292,   517,     0,   510,     0,   509,   287,   510,     0,   693,
       0,   693,   290,   693,     0,   512,     0,   519,   517,     0,
     518,     0,   523,     0,   526,     0,   235,     0,   513,     0,
     161,     0,    38,     0,   218,     0,   219,     0,    81,     0,
      92,     0,    18,   516,   515,   517,     0,    18,   288,   692,
     289,     0,    18,   288,   692,   287,   688,   289,     0,    18,
     288,   287,   688,   289,     0,   154,   160,   692,     0,     0,
     169,   688,     0,   169,   726,     0,     0,    21,   156,   727,
       0,     0,   522,   288,   691,   289,     0,   522,     0,   522,
     185,   288,   691,   289,     0,   521,   288,   691,   289,     0,
     521,     0,   520,   288,   691,   289,     0,   183,     0,    21,
     185,     0,    87,   185,     0,    21,     0,    87,     0,   115,
       0,   113,    21,     0,   113,    87,     0,    95,   524,     0,
     525,   524,     0,     0,   288,   693,   289,     0,   288,   693,
     287,   693,   289,     0,    41,     0,    88,     0,    91,   527,
       0,    93,    91,   527,     0,   144,     0,    89,   138,     0,
     288,   689,   289,     0,     0,   541,     0,   529,     0,   558,
       0,   156,    68,   736,   174,   693,     0,   156,    68,   736,
     174,   223,     0,   156,    68,   736,   174,   283,   223,     0,
     531,     0,   532,     0,   534,     0,   244,   749,     0,   245,
     244,   749,   533,     0,   123,     0,     0,   153,   538,   174,
     535,   749,     0,   244,     0,     0,    25,   538,   539,     0,
     153,   538,   539,     0,   193,     0,     0,   147,   540,     0,
       0,   162,     0,     0,   156,   175,   542,     0,   543,     0,
       0,   544,     0,   543,   544,     0,   545,     0,   546,     0,
     547,     0,   551,     0,   143,   123,     0,   143,   194,     0,
     188,     0,   117,   188,     0,    84,   102,   548,     0,   548,
       0,   549,     0,   143,   177,   550,     0,   143,    26,   550,
       0,   162,     0,   162,   172,     0,   162,   172,   166,     0,
     186,     0,   117,   186,     0,     0,   146,   554,     0,   158,
       0,   142,     0,     0,   143,     0,   194,     0,   555,     0,
     554,   287,   555,     0,   557,   556,     0,    61,   552,   553,
       0,     0,   605,     0,   557,   287,   605,     0,   156,   168,
      79,   737,     0,   269,   122,   560,    83,   564,     0,   269,
     122,   561,   740,    83,   564,     0,   269,   122,   562,   740,
     563,    83,   564,     0,    37,     0,    48,     0,   172,     0,
     187,     0,   141,     0,   176,     0,    59,    65,     0,    60,
       0,    55,     0,    68,     0,   265,     0,    79,     0,   206,
       0,    21,   156,     0,   268,     0,   207,     0,   133,     0,
     293,   740,     0,   687,     0,    94,     0,   569,   566,   568,
       0,    61,   180,   567,     0,     0,   121,   649,     0,     0,
     192,   243,     0,     0,   572,   622,   628,     0,   572,   622,
     628,     0,   572,   622,   628,     0,   573,     0,   572,   178,
     723,   573,     0,   572,   178,     6,   573,     0,   574,     0,
     155,   579,   582,   583,   587,   612,   608,   611,   613,     0,
       0,     0,     0,     0,   580,   581,   576,     0,   580,   576,
       0,   581,     0,     0,   231,   694,   575,     0,   231,   288,
     676,   289,   575,     0,   231,   683,   575,     0,   232,   694,
       0,   232,   288,   576,   676,   289,     0,   232,   683,     0,
      46,     0,   722,     0,   584,     0,   284,     0,   585,     0,
     584,   287,   585,     0,   676,     0,   676,   586,   738,     0,
      10,     0,     0,    63,   588,     0,   589,     0,   588,   287,
     589,     0,   595,     0,   590,     0,   602,     0,   591,     0,
     288,   595,   289,     0,   288,   569,   289,   586,   592,   593,
       0,   743,     0,     0,   288,   594,   289,     0,     0,   738,
       0,   594,   287,   738,     0,   596,     0,   597,     0,   598,
       0,   589,   263,    85,   590,     0,   589,   114,   606,    85,
     590,     0,   589,   606,    85,   589,   599,     0,   600,     0,
     601,     0,   122,   656,     0,   239,   288,   649,   289,     0,
     741,   603,   586,   743,     0,   741,   603,     0,   288,   679,
     289,     0,     0,   605,     0,   744,   743,     0,   744,     0,
      77,     0,   100,   607,     0,   151,   607,     0,    64,   607,
       0,     0,   127,     0,     0,    71,    19,   609,     0,     0,
     610,     0,   609,   287,   610,     0,   676,     0,    73,   656,
       0,     0,   190,   656,     0,     0,   135,   614,     0,     0,
     615,   288,   616,   289,     0,    85,     0,   164,   108,     0,
     108,     0,   164,     0,     0,   617,     0,   617,   287,   616,
       0,   618,   619,     0,   614,     0,   744,     0,   744,   618,
       0,   114,     0,    79,   288,   620,   289,     0,   126,   737,
     621,     0,   737,     0,   737,   287,   620,     0,    79,   288,
     620,   289,     0,     0,   126,    19,   623,     0,     0,   624,
       0,   623,   287,   624,     0,   676,   625,   627,     0,    11,
       0,    45,     0,     0,   231,     0,   241,     0,   240,   577,
     626,   578,     0,     0,   259,   676,     0,   259,   676,   174,
     676,     0,     0,    80,    82,   605,   650,   182,   288,   679,
     289,   636,     0,    80,    82,   605,   650,   569,     0,   631,
       0,   632,     0,    44,    63,   604,   612,   613,   622,   628,
       0,    44,    63,   604,   637,     0,   634,     0,   635,     0,
     180,   604,   156,   638,   612,   613,   622,   628,     0,   180,
     604,   156,   638,   637,     0,   278,   649,     0,   278,   649,
      82,   443,     0,     0,   190,    35,   121,   731,     0,   639,
       0,   638,   287,   639,     0,   655,   279,   676,     0,   708,
       0,   143,    18,   654,    63,   605,   642,   645,     0,    80,
      18,   654,    82,   605,   642,   645,     0,    60,    63,   643,
     174,   643,     0,    60,   174,   643,     0,     0,   644,     0,
     683,     0,   688,     0,   107,   646,     0,     0,   692,     0,
     683,     0,   648,     0,     0,   288,   649,   289,     0,   654,
       0,   649,   287,   654,     0,   651,     0,     0,   288,   652,
     289,     0,   655,     0,   652,   287,   655,     0,   654,     0,
     743,   293,   729,     0,   743,   293,   284,     0,   729,     0,
     654,     0,   743,   293,   729,     0,   672,     0,   118,   672,
       0,   658,     0,   656,   125,   656,     0,   656,     8,   656,
       0,   658,     0,   118,   672,     0,   657,   125,   656,     0,
     657,     8,   656,     0,   673,   125,   656,     0,   673,     8,
     656,     0,   659,     0,   288,   657,   289,     0,   118,   658,
       0,   660,     0,   663,     0,   664,     0,   665,     0,   666,
       0,   671,     0,   661,     0,   669,     0,   667,     0,   668,
       0,   670,     0,   676,   279,   676,     0,   676,   280,   676,
       0,   676,   281,   676,     0,   676,    67,   676,     0,   676,
     101,   676,     0,   676,   119,   676,     0,   676,   120,   676,
       0,   676,   116,   676,     0,   676,   279,     6,   288,   570,
     289,     0,   676,   280,     6,   288,   570,   289,     0,   676,
     281,     6,   288,   570,   289,     0,   676,    67,     6,   288,
     570,   289,     0,   676,   101,     6,   288,   570,   289,     0,
     676,   119,     6,   288,   570,   289,     0,   676,   120,     6,
     288,   570,   289,     0,   676,   116,     6,   288,   570,   289,
       0,   676,   279,   662,   288,   570,   289,     0,   676,   280,
     662,   288,   570,   289,     0,   676,   281,   662,   288,   570,
     289,     0,   676,    67,   662,   288,   570,   289,     0,   676,
     101,   662,   288,   570,   289,     0,   676,   119,   662,   288,
     570,   289,     0,   676,   120,   662,   288,   570,   289,     0,
     676,   116,   662,   288,   570,   289,     0,   163,     0,     9,
       0,   676,    83,    46,    63,   676,     0,   676,    83,   118,
      46,    63,   676,     0,   676,    17,   676,     8,   676,     0,
     676,   118,    17,   676,     8,   676,     0,   676,   103,   676,
       0,   676,   118,   103,   676,     0,   676,   103,   676,    54,
     676,     0,   676,   118,   103,   676,    54,   676,     0,   676,
      75,   674,     0,   676,   118,    75,   674,     0,   676,    31,
     676,     0,   676,   118,    31,   676,     0,   676,   167,   676,
       0,   676,   118,   167,   676,     0,   676,   167,   192,   676,
       0,   676,   118,   167,   192,   676,     0,    57,   288,   569,
     289,     0,   159,   288,   569,   289,     0,   676,    83,    94,
       0,   676,    83,   118,    94,     0,   248,     0,   249,     0,
     250,     0,   251,     0,   252,     0,   253,     0,   675,     0,
     288,   679,   289,     0,   288,   570,   289,     0,   653,     0,
     678,     0,   695,     0,   682,     0,   683,     0,   442,     0,
     709,     0,   710,     0,   720,     0,   708,     0,   283,   676,
       0,   282,   676,     0,   676,   282,   676,     0,   676,    28,
     676,     0,   676,    23,   728,     0,   676,   283,   676,     0,
     676,   284,   676,     0,   676,   285,   676,     0,   288,   676,
     289,     0,   288,   571,   289,     0,   684,     0,   685,     0,
     686,     0,    39,     0,   743,   293,    39,     0,    97,     0,
     677,     0,   724,     0,   220,     0,   221,     0,   222,     0,
     653,   291,   679,   292,     0,   676,     0,   679,   287,   676,
       0,   682,     0,   283,   681,     0,   197,     0,   196,     0,
     195,     0,   223,     0,   224,     0,   681,     0,   687,     0,
      38,   199,     0,   218,   199,     0,   219,   199,     0,   294,
       0,   181,     0,   225,     0,   226,     0,   233,     0,   234,
       0,    66,     0,   165,     0,   242,     0,   199,     0,   200,
     199,     0,   689,     0,   283,   690,     0,   196,     0,   196,
       0,   689,     0,   196,     0,   694,     0,   283,   694,     0,
     196,     0,   696,     0,   697,     0,   703,     0,    32,   288,
     284,   289,     0,    32,   288,   722,   676,   289,     0,    32,
     288,    46,   676,   289,     0,   171,   288,   722,   676,   289,
       0,   171,   288,    46,   676,   289,     0,    13,   288,   722,
     676,   289,     0,    13,   288,    46,   676,   289,     0,   110,
     288,   722,   676,   289,     0,   110,   288,    46,   676,   289,
       0,   106,   288,   722,   676,   289,     0,   106,   288,    46,
     676,   289,     0,   698,     0,   699,     0,   209,   288,   721,
      63,   676,   289,     0,   700,     0,   701,     0,   702,     0,
     270,   288,   676,   289,     0,   271,   288,   676,   289,     0,
     272,   288,   676,   289,     0,   275,   288,   676,   289,     0,
     704,     0,   706,     0,    96,   288,   676,   289,     0,   274,
     288,   676,   289,     0,   228,   288,   676,    63,   676,   705,
     289,     0,    61,   676,     0,     0,   277,   288,   707,   676,
      63,   676,   289,     0,   277,   288,   676,    63,   676,   289,
       0,   277,   288,   707,    63,   676,   289,     0,   277,   288,
     676,   289,     0,   267,     0,   276,     0,   273,     0,   725,
     288,   679,   289,     0,   725,   288,   289,     0,    20,   288,
     676,    10,   388,   289,     0,   711,     0,   712,     0,   237,
     288,   676,   287,   676,   289,     0,   261,   288,   656,   287,
     676,   287,   676,   289,     0,   238,   288,   676,   287,   679,
     289,     0,   713,     0,   715,     0,   236,   718,   714,    51,
       0,   236,   718,   714,    50,   719,    51,     0,   189,   717,
     173,   719,     0,   714,   189,   717,   173,   719,     0,   236,
     716,    51,     0,   236,   716,    50,   719,    51,     0,   189,
     656,   173,   719,     0,   716,   189,   656,   173,   719,     0,
     676,     0,   676,     0,   676,     0,   264,    97,    61,   736,
       0,    69,   288,   736,   287,   676,   289,     0,   210,     0,
     211,     0,   212,     0,   213,     0,   214,     0,   215,     0,
     216,     0,   217,     0,     6,     0,     0,    46,     0,     0,
      94,     0,   198,     0,   750,     0,   750,     0,   750,     0,
     750,     0,   750,     0,   750,     0,   750,     0,   750,     0,
     750,     0,   750,     0,   750,     0,   750,     0,   750,     0,
     750,     0,   750,     0,   750,     0,   750,     0,   750,     0,
     750,     0,   750,     0,   750,     0,   750,     0,   750,     0,
     750,     0,   198,     0,   751,     0,   201,     0,   203,     0,
     204,     0,   205,     0,   206,     0,   208,     0,   227,     0,
     230,     0,   228,     0,   238,     0,   241,     0,   247,     0,
     243,     0,   237,     0,   240,     0,   246,     0,   248,     0,
     249,     0,   250,     0,   260,     0,   254,     0,   255,     0,
     261,     0,   262,     0,   216,     0,   217,     0,   265,     0,
     264,     0,   266,     0,   268,     0,   278,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   551,   553,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   585,   589,   593,   598,   599,   602,
     604,   606,   610,   611,   615,   619,   621,   623,   625,   627,
     631,   633,   637,   639,   643,   650,   654,   658,   663,   665,
     669,   671,   675,   676,   679,   680,   682,   684,   688,   690,
     692,   694,   698,   699,   706,   708,   710,   714,   715,   719,
     723,   724,   728,   730,   737,   741,   743,   748,   754,   755,
     757,   763,   765,   766,   770,   771,   777,   782,   784,   786,
     788,   792,   793,   797,   800,   805,   807,   809,   811,   814,
     819,   825,   827,   834,   838,   840,   843,   845,   847,   849,
     851,   853,   855,   857,   859,   861,   870,   874,   876,   878,
     886,   890,   894,   896,   902,   909,   914,   919,   924,   932,
     934,   938,   940,   941,   947,   953,   955,   957,   961,   963,
     967,   969,   973,   975,   978,   979,   986,  1004,  1006,  1010,
    1012,  1016,  1018,  1021,  1022,  1025,  1030,  1031,  1034,  1038,
    1047,  1054,  1119,  1124,  1125,  1128,  1132,  1134,  1137,  1138,
    1142,  1144,  1146,  1148,  1150,  1154,  1156,  1159,  1160,  1164,
    1165,  1167,  1171,  1177,  1181,  1182,  1185,  1186,  1189,  1191,
    1195,  1196,  1197,  1200,  1201,  1202,  1208,  1213,  1218,  1220,
    1222,  1226,  1227,  1231,  1232,  1239,  1243,  1247,  1256,  1262,
    1263,  1266,  1268,  1273,  1275,  1280,  1286,  1292,  1296,  1302,
    1304,  1308,  1309,  1310,  1311,  1312,  1313,  1316,  1318,  1321,
    1322,  1326,  1331,  1333,  1338,  1339,  1341,  1349,  1353,  1355,
    1358,  1359,  1360,  1361,  1364,  1368,  1372,  1379,  1386,  1391,
    1397,  1400,  1402,  1404,  1406,  1410,  1413,  1417,  1420,  1423,
    1426,  1435,  1446,  1456,  1466,  1476,  1478,  1482,  1484,  1488,
    1489,  1493,  1499,  1500,  1504,  1509,  1511,  1513,  1517,  1519,
    1523,  1524,  1528,  1532,  1533,  1536,  1541,  1543,  1547,  1549,
    1553,  1558,  1559,  1562,  1566,  1568,  1570,  1574,  1575,  1579,
    1580,  1581,  1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,
    1590,  1591,  1592,  1593,  1595,  1597,  1598,  1599,  1600,  1603,
    1605,  1609,  1613,  1617,  1622,  1626,  1630,  1632,  1636,  1640,
    1646,  1651,  1656,  1657,  1658,  1660,  1664,  1668,  1670,  1674,
    1676,  1678,  1683,  1686,  1690,  1691,  1695,  1700,  1701,  1705,
    1707,  1709,  1711,  1715,  1716,  1717,  1720,  1724,  1728,  1732,
    1775,  1780,  1782,  1784,  1788,  1790,  1792,  1798,  1806,  1808,
    1812,  1813,  1817,  1823,  1830,  1853,  1857,  1863,  1867,  1873,
    1877,  1885,  1896,  1907,  1909,  1911,  1915,  1919,  1921,  1925,
    1927,  1929,  1931,  1933,  1935,  1937,  1939,  1941,  1943,  1945,
    1947,  1949,  1951,  1953,  1957,  1959,  1963,  1969,  1973,  1975,
    1982,  1984,  1986,  1989,  1992,  1994,  1996,  2000,  2004,  2010,
    2011,  2015,  2024,  2028,  2030,  2032,  2034,  2036,  2040,  2041,
    2045,  2047,  2049,  2051,  2057,  2060,  2062,  2064,  2067,  2072,
    2080,  2081,  2082,  2083,  2084,  2085,  2086,  2087,  2088,  2089,
    2090,  2091,  2092,  2093,  2094,  2095,  2096,  2097,  2098,  2099,
    2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,  2108,  2109,
    2110,  2111,  2112,  2113,  2114,  2115,  2117,  2118,  2119,  2121,
    2122,  2125,  2127,  2131,  2133,  2138,  2144,  2146,  2148,  2152,
    2154,  2158,  2161,  2164,  2169,  2173,  2175,  2179,  2181,  2188,
    2192,  2193,  2197,  2199,  2201,  2203,  2205,  2212,  2222,  2223,
    2227,  2228,  2234,  2238,  2240,  2242,  2244,  2246,  2248,  2250,
    2252,  2254,  2256,  2258,  2260,  2262,  2269,  2270,  2273,  2274,
    2277,  2282,  2289,  2290,  2294,  2303,  2309,  2310,  2313,  2314,
    2315,  2316,  2333,  2338,  2343,  2362,  2379,  2386,  2387,  2394,
    2399,  2406,  2413,  2422,  2426,  2432,  2436,  2440,  2446,  2450,
    2457,  2463,  2469,  2477,  2482,  2487,  2494,  2495,  2496,  2499,
    2500,  2503,  2504,  2505,  2512,  2516,  2527,  2533,  2587,  2649,
    2650,  2657,  2670,  2675,  2680,  2687,  2689,  2696,  2697,  2698,
    2702,  2705,  2708,  2716,  2717,  2718,  2721,  2725,  2729,  2731,
    2735,  2739,  2740,  2743,  2747,  2751,  2752,  2755,  2757,  2761,
    2762,  2766,  2770,  2771,  2775,  2776,  2780,  2781,  2782,  2783,
    2786,  2788,  2792,  2794,  2798,  2800,  2803,  2805,  2807,  2811,
    2813,  2815,  2819,  2821,  2823,  2827,  2831,  2833,  2835,  2839,
    2841,  2845,  2846,  2850,  2854,  2856,  2860,  2861,  2866,  2870,
    2872,  2874,  2878,  2882,  2884,  2886,  2888,  2890,  2892,  2894,
    2896,  2898,  2900,  2902,  2904,  2906,  2908,  2916,  2918,  2922,
    2926,  2927,  2934,  2938,  2940,  2944,  2946,  2950,  2952,  2959,
    2963,  2968,  2973,  2974,  2976,  2980,  2983,  2995,  2999,  3003,
    3007,  3011,  3013,  3015,  3017,  3021,  3023,  3025,  3029,  3031,
    3033,  3037,  3039,  3043,  3045,  3049,  3050,  3054,  3055,  3059,
    3060,  3065,  3069,  3070,  3074,  3075,  3078,  3079,  3080,  3085,
    3090,  3091,  3095,  3097,  3101,  3102,  3106,  3107,  3108,  3111,
    3116,  3121,  3125,  3126,  3129,  3133,  3137,  3140,  3145,  3147,
    3151,  3152,  3157,  3162,  3164,  3166,  3168,  3170,  3174,  3175,
    3181,  3183,  3187,  3188,  3194,  3197,  3199,  3203,  3205,  3212,
    3214,  3218,  3222,  3224,  3226,  3232,  3234,  3238,  3239,  3243,
    3245,  3248,  3249,  3253,  3255,  3257,  3261,  3262,  3266,  3268,
    3274,  3276,  3280,  3281,  3285,  3289,  3291,  3293,  3297,  3299,
    3303,  3305,  3311,  3314,  3322,  3329,  3333,  3341,  3342,  3345,
    3351,  3359,  3360,  3363,  3369,  3375,  3378,  3381,  3385,  3392,
    3393,  3397,  3401,  3408,  3410,  3414,  3416,  3418,  3422,  3423,
    3426,  3430,  3432,  3436,  3438,  3444,  3445,  3449,  3453,  3454,
    3459,  3460,  3464,  3468,  3469,  3474,  3475,  3478,  3483,  3488,
    3491,  3498,  3499,  3501,  3502,  3504,  3508,  3509,  3511,  3513,
    3517,  3519,  3523,  3524,  3526,  3530,  3531,  3532,  3533,  3534,
    3535,  3536,  3537,  3538,  3539,  3540,  3545,  3547,  3549,  3551,
    3553,  3555,  3557,  3559,  3565,  3567,  3569,  3571,  3573,  3575,
    3577,  3579,  3581,  3583,  3585,  3587,  3589,  3591,  3593,  3595,
    3599,  3600,  3606,  3608,  3612,  3614,  3619,  3621,  3623,  3625,
    3630,  3632,  3636,  3638,  3642,  3644,  3646,  3648,  3652,  3656,
    3660,  3662,  3666,  3671,  3676,  3683,  3688,  3693,  3702,  3703,
    3707,  3714,  3715,  3716,  3717,  3718,  3719,  3720,  3721,  3722,
    3723,  3724,  3726,  3728,  3735,  3737,  3739,  3746,  3753,  3760,
    3762,  3764,  3765,  3766,  3767,  3769,  3771,  3775,  3776,  3779,
    3795,  3811,  3815,  3819,  3820,  3824,  3825,  3829,  3831,  3833,
    3835,  3837,  3841,  3842,  3844,  3860,  3876,  3880,  3884,  3886,
    3890,  3894,  3897,  3900,  3903,  3906,  3911,  3913,  3918,  3919,
    3923,  3930,  3937,  3944,  3951,  3952,  3956,  3963,  3964,  3965,
    3968,  3970,  3972,  3975,  3982,  3991,  3998,  4007,  4009,  4011,
    4013,  4017,  4018,  4021,  4025,  4026,  4027,  4030,  4035,  4038,
    4043,  4048,  4049,  4050,  4052,  4056,  4065,  4067,  4071,  4073,
    4076,  4078,  4083,  4085,  4087,  4091,  4093,  4097,  4103,  4104,
    4107,  4111,  4114,  4118,  4119,  4122,  4124,  4128,  4130,  4134,
    4136,  4140,  4142,  4146,  4149,  4152,  4157,  4166,  4176,  4178,
    4180,  4182,  4184,  4186,  4188,  4190,  4194,  4195,  4198,  4199,
    4202,  4210,  4213,  4216,  4219,  4222,  4225,  4228,  4231,  4234,
    4237,  4240,  4243,  4246,  4249,  4252,  4255,  4258,  4261,  4264,
    4267,  4270,  4273,  4276,  4279,  4282,  4287,  4288,  4293,  4295,
    4296,  4297,  4298,  4299,  4300,  4301,  4302,  4303,  4304,  4305,
    4306,  4307,  4308,  4309,  4310,  4311,  4312,  4314,  4316,  4317,
    4318,  4320,  4322,  4323,  4324,  4325,  4326,  4327,  4328
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
  "OCTET_LENGTH", "TRAILING", "TRIM", "RETURNING", "'='", "'<'", "'>'", 
  "'+'", "'-'", "'*'", "'/'", "';'", "','", "'('", "')'", "':'", "'['", 
  "']'", "'.'", "'?'", "top", "statement", "grant", "table_noise", 
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
  "returning_clause", "cursor_clause", "assignments", "assignment", 
  "exec_udf", "blob_io", "filter_clause_io", "blob_subtype_value_io", 
  "blob_subtype_io", "segment_clause_io", "segment_length_io", 
  "column_parens_opt", "column_parens", "column_list", 
  "ins_column_parens_opt", "ins_column_parens", "ins_column_list", 
  "column_name", "simple_column_name", "update_column_name", 
  "search_condition", "bracable_search_condition", 
  "simple_search_condition", "predicate", "comparison_predicate", 
  "quantified_predicate", "some", "distinct_predicate", 
  "between_predicate", "like_predicate", "in_predicate", 
  "containing_predicate", "starting_predicate", "exists_predicate", 
  "singular_predicate", "null_predicate", "trigger_action_predicate", 
  "special_trigger_action_predicate", "in_predicate_value", 
  "table_subquery", "value", "datetime_value_expression", "array_element", 
  "value_list", "constant", "u_numeric_constant", "u_constant", 
  "parameter", "current_user", "current_role", "internal_info", 
  "sql_string", "signed_short_integer", "nonneg_short_integer", 
  "neg_short_integer", "pos_short_integer", "unsigned_short_integer", 
  "signed_long_integer", "long_integer", "function", "aggregate_function", 
  "numeric_value_function", "extract_expression", "length_expression", 
  "bit_length_expression", "char_length_expression", 
  "octet_length_expression", "string_value_function", 
  "substring_function", "string_length_opt", "trim_function", 
  "trim_specification", "udf", "cast_specification", "case_expression", 
  "case_abbreviation", "case_specification", "simple_case", 
  "simple_when_clause", "searched_case", "searched_when_clause", 
  "when_operand", "case_operand", "case_result", "next_value_expression", 
  "timestamp_part", "all_noise", "distinct_noise", "null_value", 
  "symbol_UDF_name", "symbol_blob_subtype_name", 
  "symbol_character_set_name", "symbol_collation_name", 
  "symbol_column_name", "symbol_constraint_name", "symbol_cursor_name", 
  "symbol_domain_name", "symbol_exception_name", "symbol_filter_name", 
  "symbol_gdscode_name", "symbol_generator_name", "symbol_index_name", 
  "symbol_item_alias_name", "symbol_label_name", "symbol_ddl_name", 
  "symbol_procedure_name", "symbol_role_name", "symbol_table_alias_name", 
  "symbol_table_name", "symbol_trigger_name", "symbol_user_name", 
  "symbol_variable_name", "symbol_view_name", "symbol_savepoint_name", 
  "valid_symbol_name", "non_reserved_word", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   295,   295,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   297,   297,   297,   298,   298,   299,
     299,   299,   300,   300,   301,   302,   302,   302,   302,   302,
     303,   303,   304,   304,   305,   306,   306,   306,   307,   307,
     308,   308,   309,   309,   310,   310,   310,   310,   311,   311,
     311,   311,   312,   312,   313,   313,   313,   314,   314,   315,
     316,   316,   317,   317,   318,   319,   319,   320,   321,   321,
     321,   322,   322,   322,   323,   323,   324,   325,   325,   325,
     325,   326,   326,   327,   327,   328,   328,   328,   328,   328,
     329,   330,   330,   331,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   333,   334,   334,   334,
     334,   335,   336,   336,   336,   337,   338,   339,   340,   341,
     341,   342,   342,   342,   343,   344,   344,   344,   345,   345,
     346,   346,   347,   347,   348,   348,   349,   350,   350,   351,
     351,   352,   352,   353,   353,   354,   355,   355,   356,   357,
     358,   359,   360,   361,   361,   362,   363,   363,   364,   364,
     365,   365,   365,   365,   365,   366,   366,   367,   367,   368,
     368,   368,   369,   370,   371,   371,   372,   372,   373,   373,
     374,   374,   374,   375,   375,   375,   376,   377,   378,   378,
     378,   379,   379,   380,   380,   381,   381,   381,   382,   383,
     383,   384,   384,   385,   385,   386,   387,   388,   389,   390,
     390,   391,   391,   391,   391,   391,   391,   392,   392,   393,
     393,   394,   395,   395,   395,   395,   395,   396,   397,   397,
     398,   398,   398,   398,   399,   400,   401,   402,   402,   403,
     404,   404,   404,   404,   404,   405,   406,   407,   407,   407,
     407,   408,   409,   410,   411,   412,   412,   413,   413,   414,
     414,   415,   416,   416,   417,   418,   418,   418,   419,   419,
     420,   420,   421,   422,   422,   423,   424,   424,   425,   425,
     426,   427,   427,   428,   429,   429,   429,   430,   430,   431,
     431,   431,   431,   431,   431,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   431,   431,   431,   431,   432,
     432,   433,   434,   435,   436,   437,   438,   438,   439,   440,
     441,   442,   443,   443,   443,   443,   444,   445,   445,   446,
     446,   446,   447,   447,   448,   448,   449,   450,   450,   451,
     451,   451,   451,   452,   452,   452,   453,   454,   455,   456,
     457,   458,   458,   458,   459,   459,   459,   460,   461,   461,
     462,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     470,   471,   472,   473,   473,   473,   474,   475,   475,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   477,   477,   478,   479,   480,   480,
     480,   480,   480,   480,   480,   480,   480,   481,   482,   483,
     483,   484,   484,   484,   484,   484,   484,   484,   485,   485,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   487,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   488,   488,   488,   488,   488,   488,   488,   488,   488,
     488,   489,   489,   490,   490,   491,   492,   492,   492,   493,
     493,   494,   494,   494,   495,   496,   496,   497,   497,   498,
     499,   499,   500,   500,   500,   500,   500,   501,   502,   502,
     503,   503,   504,   505,   505,   505,   505,   505,   505,   505,
     505,   505,   505,   505,   505,   505,   506,   506,   507,   507,
     508,   508,   509,   509,   510,   510,   511,   511,   512,   512,
     512,   512,   512,   512,   512,   512,   512,   513,   513,   514,
     514,   514,   514,   515,   515,   516,   516,   516,   517,   517,
     518,   518,   518,   519,   519,   519,   520,   520,   520,   521,
     521,   522,   522,   522,   523,   523,   524,   524,   524,   525,
     525,   526,   526,   526,   526,   527,   527,   528,   528,   528,
     529,   529,   529,   530,   530,   530,   531,   532,   533,   533,
     534,   535,   535,   536,   537,   538,   538,   539,   539,   540,
     540,   541,   542,   542,   543,   543,   544,   544,   544,   544,
     545,   545,   546,   546,   547,   547,   548,   548,   548,   549,
     549,   549,   550,   550,   550,   551,   552,   552,   552,   553,
     553,   554,   554,   555,   556,   556,   557,   557,   558,   559,
     559,   559,   560,   561,   561,   561,   561,   561,   561,   561,
     561,   561,   561,   561,   561,   561,   561,   562,   562,   563,
     564,   564,   565,   566,   566,   567,   567,   568,   568,   569,
     570,   571,   572,   572,   572,   573,   574,   575,   576,   577,
     578,   579,   579,   579,   579,   580,   580,   580,   581,   581,
     581,   582,   582,   583,   583,   584,   584,   585,   585,   586,
     586,   587,   588,   588,   589,   589,   590,   590,   590,   591,
     592,   592,   593,   593,   594,   594,   595,   595,   595,   596,
     597,   598,   599,   599,   600,   601,   602,   602,   603,   603,
     604,   604,   605,   606,   606,   606,   606,   606,   607,   607,
     608,   608,   609,   609,   610,   611,   611,   612,   612,   613,
     613,   614,   615,   615,   615,   615,   615,   616,   616,   617,
     617,   618,   618,   619,   619,   619,   620,   620,   621,   621,
     622,   622,   623,   623,   624,   625,   625,   625,   626,   626,
     627,   627,   628,   628,   628,   629,   629,   630,   630,   631,
     632,   633,   633,   634,   635,   636,   636,   636,   637,   638,
     638,   639,   640,   641,   641,   642,   642,   642,   643,   643,
     644,   645,   645,   646,   646,   647,   647,   648,   649,   649,
     650,   650,   651,   652,   652,   653,   653,   653,   654,   655,
     655,   656,   656,   656,   656,   656,   657,   657,   657,   657,
     657,   657,   658,   658,   658,   659,   659,   659,   659,   659,
     659,   659,   659,   659,   659,   659,   660,   660,   660,   660,
     660,   660,   660,   660,   661,   661,   661,   661,   661,   661,
     661,   661,   661,   661,   661,   661,   661,   661,   661,   661,
     662,   662,   663,   663,   664,   664,   665,   665,   665,   665,
     666,   666,   667,   667,   668,   668,   668,   668,   669,   670,
     671,   671,   672,   672,   672,   673,   673,   673,   674,   674,
     675,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   676,
     676,   676,   676,   676,   676,   676,   676,   676,   676,   677,
     677,   677,   678,   679,   679,   680,   680,   681,   681,   681,
     681,   681,   682,   682,   682,   682,   682,   683,   684,   684,
     685,   686,   686,   686,   686,   686,   687,   687,   688,   688,
     689,   690,   691,   692,   693,   693,   694,   695,   695,   695,
     696,   696,   696,   696,   696,   696,   696,   696,   696,   696,
     696,   697,   697,   698,   699,   699,   699,   700,   701,   701,
     702,   703,   703,   703,   703,   704,   705,   705,   706,   706,
     706,   706,   707,   707,   707,   708,   708,   709,   710,   710,
     711,   711,   711,   712,   712,   713,   713,   714,   714,   715,
     715,   716,   716,   717,   718,   719,   720,   720,   721,   721,
     721,   721,   721,   721,   721,   721,   722,   722,   723,   723,
     724,   725,   726,   727,   728,   729,   730,   731,   732,   733,
     734,   735,   736,   737,   738,   739,   740,   741,   742,   743,
     744,   745,   746,   747,   748,   749,   750,   750,   751,   751,
     751,   751,   751,   751,   751,   751,   751,   751,   751,   751,
     751,   751,   751,   751,   751,   751,   751,   751,   751,   751,
     751,   751,   751,   751,   751,   751,   751,   751,   751
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
       3,     3,     2,     2,     4,     4,     4,     5,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     1,     1,     1,     0,     2,
       2,     4,     4,     5,     3,     2,     0,     2,     0,     0,
       1,     2,     2,     4,     3,     2,     2,     7,     1,     0,
       1,     0,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     2,     2,     2,     2,     1,     1,     1,     1,
       4,     5,     1,     3,     1,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     6,     5,     3,     0,     2,     2,     0,     3,     0,
       4,     1,     5,     4,     1,     4,     1,     2,     2,     1,
       1,     1,     2,     2,     2,     2,     0,     3,     5,     1,
       1,     2,     3,     1,     2,     3,     0,     1,     1,     1,
       5,     5,     6,     1,     1,     1,     2,     4,     1,     0,
       5,     1,     0,     3,     3,     1,     0,     2,     0,     1,
       0,     3,     1,     0,     1,     2,     1,     1,     1,     1,
       2,     2,     1,     2,     3,     1,     1,     3,     3,     1,
       2,     3,     1,     2,     0,     2,     1,     1,     0,     1,
       1,     1,     3,     2,     3,     0,     1,     3,     4,     5,
       6,     7,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     2,
       1,     1,     3,     3,     0,     2,     0,     2,     0,     3,
       3,     3,     1,     4,     4,     1,     9,     0,     0,     0,
       0,     3,     2,     1,     0,     3,     5,     3,     2,     5,
       2,     1,     1,     1,     1,     1,     3,     1,     3,     1,
       0,     2,     1,     3,     1,     1,     1,     1,     3,     6,
       1,     0,     3,     0,     1,     3,     1,     1,     1,     4,
       5,     5,     1,     1,     2,     4,     4,     2,     3,     0,
       1,     2,     1,     1,     2,     2,     2,     0,     1,     0,
       3,     0,     1,     3,     1,     2,     0,     2,     0,     2,
       0,     4,     1,     2,     1,     1,     0,     1,     3,     2,
       1,     1,     2,     1,     4,     3,     1,     3,     4,     0,
       3,     0,     1,     3,     3,     1,     1,     0,     1,     1,
       4,     0,     2,     4,     0,     9,     5,     1,     1,     7,
       4,     1,     1,     8,     5,     2,     4,     0,     4,     1,
       3,     3,     1,     7,     7,     5,     3,     0,     1,     1,
       1,     2,     0,     1,     1,     1,     0,     3,     1,     3,
       1,     0,     3,     1,     3,     1,     3,     3,     1,     1,
       3,     1,     2,     1,     3,     3,     1,     2,     3,     3,
       3,     3,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       1,     1,     5,     6,     5,     6,     3,     4,     5,     6,
       3,     4,     3,     4,     3,     4,     4,     5,     4,     4,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     4,     1,     3,     1,     2,     1,     1,     1,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       0,     0,   606,   130,     0,     0,     0,     0,     0,     0,
       0,     0,    51,   606,   694,     0,     0,     0,     0,     0,
       0,     1,    11,    17,     8,     7,    15,    16,    13,    14,
       3,    10,    21,   588,    19,   593,   594,   595,     6,    18,
     587,   589,     5,    20,   674,   781,   682,   685,    12,     9,
     797,   798,    22,   801,   802,     4,   499,     0,     0,     0,
       0,     0,     0,     0,     0,   407,   605,   608,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   129,     0,     0,
       0,   103,   787,   980,     0,    23,   978,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,     0,   369,    29,    37,    34,
      36,   826,    35,   826,  1086,  1088,  1089,  1090,  1091,  1092,
    1093,  1112,  1113,  1094,  1096,  1095,  1101,  1097,  1102,  1098,
    1100,  1103,  1099,  1104,  1105,  1106,  1108,  1109,  1107,  1110,
    1111,  1115,  1114,  1116,  1117,  1118,     0,    31,     0,    32,
       0,    67,    69,  1078,  1087,     0,     0,     0,     0,     0,
       0,     0,   608,     0,     0,  1057,   688,   693,     0,     0,
     613,     0,   740,   742,  1080,     0,     0,     0,     0,   116,
     596,  1085,     0,     0,     2,     0,   678,     0,  1059,   794,
       0,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   469,   463,   464,   462,   465,   466,   467,   468,
     470,   472,   475,   471,   473,   474,   476,   477,   478,   479,
     480,     0,   439,   440,   408,     0,  1069,     0,   414,     0,
    1073,   411,   266,  1077,     0,   742,   410,   385,  1081,   415,
       0,  1072,   610,   603,   976,     0,   112,   166,   165,   113,
     148,   215,   838,  1065,   104,     0,   110,   160,     0,   106,
     266,   114,   982,   137,   107,   200,   108,     0,   109,   826,
    1084,   115,   161,   111,   785,   786,     0,   981,   979,     0,
      75,     0,  1070,   758,   520,  1068,   513,     0,   519,   524,
     514,   515,   522,   516,   517,   518,   523,   525,   363,     0,
     268,    30,     0,    39,   825,    38,    28,     0,     0,     0,
       0,     0,   831,     0,     0,     0,     0,     0,     0,   602,
     604,   986,     0,   967,   687,   687,   688,   700,   698,  1056,
     701,     0,   702,   692,   688,     0,     0,     0,     0,     0,
       0,   629,   622,   611,   612,   614,   616,   617,   618,   625,
     626,   619,     0,   741,  1079,   120,     0,   117,   266,   118,
     200,   119,   826,   599,     0,   652,   653,   660,     0,   659,
     661,   663,   668,   656,   654,   657,   655,   664,   667,   662,
     666,     0,     0,     0,   676,     0,   672,     0,     0,  1058,
       0,     0,   679,     0,     0,     0,     0,   412,   500,   377,
       0,   377,     0,   218,   413,   419,   128,  1061,   416,   496,
     489,   490,     0,   268,   239,   481,     0,   409,   428,     0,
     383,   384,   509,     0,   609,   607,   977,   163,   163,     0,
       0,     0,   175,   167,   168,   147,     0,   125,     0,     0,
       0,   121,   268,   136,   135,   138,     0,     0,     0,     0,
       0,    76,   218,     0,     0,   760,   800,   521,     0,     0,
       0,     0,   944,   973,     0,  1060,     0,   946,     0,     0,
     974,     0,   968,   959,   958,   957,  1086,     0,     0,     0,
     949,   950,   951,   960,   961,   969,   970,  1096,   971,   972,
       0,  1101,  1097,   975,  1110,  1115,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   926,   366,   921,   835,
     953,   947,   922,   361,   962,   924,   925,   941,   942,   943,
     963,   923,   987,   988,  1001,  1002,  1004,  1005,  1006,   989,
    1011,  1012,   930,   927,   928,  1028,  1029,  1033,  1034,   929,
     948,     0,     0,  1065,     0,     0,     0,   370,   216,     0,
       0,     0,   828,    27,     0,    33,     0,     0,    43,    70,
      72,  1082,    68,     0,     0,     0,   830,     0,    48,    50,
      28,     0,     0,   601,     0,     0,     0,   697,   695,     0,
     704,     0,   703,   705,   710,   691,     0,   648,     0,   623,
     634,   620,   634,   621,   635,   641,   645,   646,   630,   615,
     758,   809,   839,     0,     0,   126,   268,     0,     0,   598,
     597,   665,   658,     0,     0,  1076,     0,     0,   673,   677,
     780,   782,   787,   684,   683,   792,   183,     0,   502,   144,
       0,   505,     0,   506,   501,     0,   423,     0,   425,   424,
       0,   426,     0,   420,     0,   498,     0,     0,   269,     0,
       0,   432,     0,   433,   377,   482,     0,   488,     0,     0,
       0,   485,  1065,   388,   387,   508,     0,   405,     0,   164,
       0,     0,   173,     0,   172,     0,     0,   162,   176,   177,
     179,   169,   557,   569,   544,   579,   547,   570,   580,     0,
     586,   548,     0,   576,     0,   571,   583,   543,   566,   545,
     546,   541,   377,   526,   527,   528,   536,   542,   529,   538,
     559,     0,   564,   561,   539,   576,   540,   124,     0,   122,
     266,   123,     0,     0,   139,     0,     0,   199,   239,   385,
     375,     0,   218,     0,    82,    84,     0,     0,   102,   101,
    1062,     0,     0,     0,     0,   912,   913,   914,     0,   757,
     843,   852,   855,   861,   856,   857,   858,   859,   863,   864,
     862,   865,   860,   841,     0,   766,   781,  1057,     0,  1057,
     964,     0,     0,  1057,  1057,  1057,     0,   965,   966,     0,
       0,  1044,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   932,   931,     0,   781,   953,     0,
     331,  1083,     0,   360,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   274,   536,   559,     0,     0,   368,
       0,   279,     0,   827,     0,     0,    44,    73,     0,     0,
      26,   817,     0,   833,     0,   796,   817,     0,     0,    47,
     600,     0,   687,     0,     0,   758,     0,   709,     0,   591,
       0,   590,   984,     0,   624,     0,   632,   628,   627,     0,
     638,     0,   643,   631,     0,   760,   804,     0,     0,     0,
     239,   375,   671,   649,   670,     0,     0,     0,   675,     0,
     791,     0,     0,   145,   184,   504,   377,   422,     0,   377,
     378,   427,   495,     0,   494,   375,     0,   265,   375,   238,
    1066,   210,   207,     0,   150,   377,   526,   375,     0,     0,
       0,   237,   240,   241,   242,   243,     0,   431,   487,   486,
     430,   429,     0,     0,     0,     0,     0,   391,   389,   390,
     386,     0,   377,   492,     0,   491,   193,   170,   174,     0,
       0,   178,     0,     0,   554,   567,   568,   584,     0,   581,
     586,     0,   574,   572,   573,   150,     0,     0,     0,   537,
       0,     0,     0,     0,   575,   127,   268,     0,   375,   140,
     198,     0,   201,   203,   204,     0,     0,     0,     0,   218,
     218,    79,     0,    87,    78,     0,     0,     0,     0,   854,
     842,     0,     0,   915,   916,   917,     0,   846,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   762,   764,   765,
     759,     0,   794,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1048,  1049,  1050,
    1051,  1052,  1053,  1054,  1055,     0,     0,     0,     0,  1039,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1022,  1024,  1023,     0,     0,   940,   794,   939,
     362,     0,   332,   364,   333,     0,     0,   935,  1064,   934,
     933,   936,   937,   938,   954,  1026,     0,   945,   837,   836,
     372,   371,     0,   272,   287,     0,   278,   280,   829,     0,
       0,     0,    71,     0,   822,     0,   832,     0,   822,     0,
       0,   696,   699,     0,   711,   712,   715,   717,   714,   726,
     727,   728,   716,   739,     0,   751,   706,   708,  1074,   592,
     985,   633,   642,   637,   636,     0,   647,   810,   781,   811,
     840,   375,     0,     0,   650,   669,     0,   783,   689,   784,
     793,   503,   163,   190,   182,   185,   186,     0,     0,     0,
     421,   497,   375,   375,   378,   277,   270,   279,   209,   377,
     377,   378,   211,   206,   212,     0,     0,   248,     0,   435,
     438,   378,   436,   377,   375,   434,     0,     0,     0,   404,
     511,   493,   194,   195,   171,     0,   181,   555,   556,   983,
       0,     0,     0,   559,     0,   582,     0,     0,   378,     0,
     532,   534,     0,     0,     0,     0,     0,     0,     0,   385,
     279,   163,   142,   239,   196,   405,   380,     0,    83,     0,
     218,     0,    91,     0,    85,     0,     0,    90,    86,     0,
     808,  1067,     0,     0,   847,     0,     0,   853,     0,     0,
     845,   844,     0,   902,     0,   891,   890,     0,   869,     0,
     900,   918,     0,   910,     0,     0,     0,   870,   896,     0,
       0,   873,     0,     0,     0,     0,     0,     0,     0,   871,
       0,     0,   872,     0,   904,     0,     0,   866,     0,     0,
     867,     0,     0,   868,   763,   766,   799,     0,     0,   218,
       0,   990,     0,     0,  1013,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1045,     0,     0,  1043,     0,     0,
    1035,     0,     0,     0,     0,  1046,  1007,  1008,  1009,  1014,
    1010,     0,  1021,     0,     0,   681,     0,     0,     0,   952,
    1025,     0,   267,   286,     0,   338,   367,   281,     0,     0,
       0,     0,     0,  1092,    41,    52,    54,    53,    62,    64,
      41,    42,     0,     0,     0,   814,   834,     0,   813,     0,
       0,     0,   747,   714,     0,   749,   743,   749,   747,   749,
       0,     0,     0,   710,     0,   756,   639,   640,   644,   794,
     279,   197,   380,   651,     0,     0,   191,     0,   187,     0,
       0,     0,   417,   226,   221,   955,   222,   223,   224,   225,
       0,     0,   271,     0,     0,     0,   227,     0,   248,   787,
     244,     0,   437,   483,   484,   396,   397,   393,   392,   395,
     394,   377,   510,   378,   180,  1063,     0,     0,   550,     0,
     549,   585,     0,   577,   151,     0,   530,     0,   558,   559,
     565,   563,     0,   560,   375,     0,     0,     0,   134,   143,
     202,   377,     0,   376,   105,     0,   132,   131,    94,     0,
       0,    95,     0,    88,    89,     0,   908,   909,   849,   848,
     851,   850,     0,     0,     0,     0,   781,     0,     0,     0,
     911,     0,     0,     0,     0,     0,     0,   903,   901,   897,
       0,   905,     0,     0,     0,     0,   906,     0,     0,     0,
       0,     0,     0,   770,     0,   767,     0,   771,   996,   995,
       0,     0,   992,   991,     0,  1000,   999,   998,   997,   994,
     993,     0,  1017,  1041,  1040,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   335,   334,   273,     0,
       0,   283,   284,     0,  1065,   321,     0,     0,     0,     0,
       0,     0,  1094,  1099,     0,     0,   359,   297,   292,     0,
     338,   291,     0,     0,     0,   317,   318,     0,   315,     0,
       0,   316,     0,     0,     0,   353,   355,   354,     0,     0,
       0,     0,     0,     0,     0,   812,     0,  1065,    66,    58,
      59,    65,    60,    61,     0,    24,     0,     0,    25,     0,
     818,   819,   820,   816,   821,   824,   823,   807,    45,    46,
     710,   718,   713,   748,   746,   744,     0,   745,     0,     0,
       0,     0,     0,     0,   760,   803,     0,   376,   788,   789,
     690,   193,   192,   188,     0,   378,   956,   275,   276,   378,
     378,   149,   214,   228,   229,   377,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   279,   507,   552,     0,
     553,     0,     0,   214,   152,   153,   155,   156,   157,   533,
     535,   531,   562,   279,   405,   378,   193,     0,     0,   373,
     377,    92,     0,     0,    98,    93,   559,     0,   894,     0,
       0,   920,   794,   919,   892,     0,     0,     0,   898,     0,
       0,     0,     0,   907,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   761,   766,     0,   773,     0,   769,
     772,  1027,   217,  1047,  1003,     0,     0,  1042,  1037,  1036,
       0,  1030,  1032,     0,  1019,  1020,     0,   220,   282,     0,
     319,     0,   314,     0,   329,   313,   341,  1075,   356,   357,
       0,   293,     0,   298,   295,   344,   308,   309,   305,   306,
     310,   303,     0,     0,   312,   311,   304,     0,   300,   302,
     301,   299,   307,   337,     0,    55,    56,    57,    63,     0,
       0,   795,   721,     0,   729,   747,   738,   736,   750,   752,
     754,   755,   686,   378,   374,   790,   189,   378,   159,   264,
       0,     0,   205,   230,     0,     0,     0,   248,   231,   234,
     826,     0,   378,   402,   403,   399,   398,   401,   400,     0,
     551,   578,   158,   146,   154,     0,   377,   261,   141,   378,
     379,     0,     0,    96,    97,    80,     0,   877,   885,   680,
     893,   878,   886,   881,   889,   895,   899,   879,   887,   880,
     888,   874,   882,   875,   883,   876,   884,   768,     0,   779,
    1016,  1015,  1038,     0,  1018,     0,     0,   289,   285,     0,
     320,   322,     0,   328,     0,   352,     0,     0,     0,     0,
     347,   345,     0,     0,     0,   330,    40,   815,   805,   723,
     720,   730,     0,     0,   731,   732,   733,     0,   262,   418,
     208,   213,   232,   248,   826,   235,   254,   247,   249,   406,
     378,     0,   381,   378,    77,    99,   100,     0,   776,     0,
     775,  1031,   219,   288,     0,     0,     0,     0,   351,   350,
    1071,   349,   338,     0,     0,     0,     0,     0,     0,   719,
     734,     0,   753,   236,   254,     0,   248,   250,   251,   263,
     378,     0,   774,     0,     0,     0,   325,   338,   358,   346,
     348,     0,   343,     0,   806,     0,   724,     0,   248,     0,
       0,   246,     0,   253,     0,   252,   382,   133,   777,     0,
     290,   327,     0,     0,     0,   338,     0,   722,   735,   233,
       0,     0,   257,   256,   255,   778,   338,     0,     0,   338,
     336,   725,   260,   258,   259,   326,   338,   342,   323,   324,
       0,     0,     0
};

static const short yydefgoto[] =
{
    2030,    21,    22,   564,   146,   147,   148,   149,  1615,   840,
     835,    23,   160,   161,  1364,  1365,  1366,  1367,  1368,   150,
     151,   568,   569,    24,    89,   461,   993,   743,   744,   745,
    1248,  1241,  1242,  1705,   290,   747,    25,    81,    26,   179,
      27,   451,   264,   365,   727,   234,    82,  1474,   271,   455,
     735,  1232,  1468,   638,   259,   446,  1181,  1683,  1684,  1685,
    1686,  1687,  1688,   266,   281,   256,   680,   257,   442,   443,
     444,   687,   688,   689,   639,   640,  1164,  1165,  1166,  1407,
    1204,   274,   369,   457,   981,   982,   983,   912,   913,   914,
    1822,   662,   554,  1530,   746,  1887,  1412,  1662,  1663,  1664,
    1828,   984,   664,   921,   922,   923,   924,  1430,   925,  1966,
    1967,  1968,  2013,   269,   367,   729,   241,   423,   560,   657,
     658,  1102,   555,  1174,  1105,  1106,  1107,  1560,  1561,  1354,
    1888,  1562,  1577,  1578,  1579,  1580,  1581,  1582,  1583,  1584,
    1585,  1586,  1587,  1588,  1589,  1893,  1590,   516,  1083,  1591,
    1592,  1593,  2004,  1774,  1775,  1899,  1900,  1594,  1595,  1596,
    1597,  1770,  1598,   517,   813,    29,   310,   556,   557,   278,
     371,   986,  1699,   647,  1170,  1473,   276,   731,   432,   675,
     676,   940,   942,  1442,    30,    65,   900,   897,   414,   415,
     427,   428,   231,   232,   429,  1192,   670,   930,   238,   249,
     418,   655,   904,   190,   407,   408,   246,   677,  1443,    31,
     104,   712,   713,   714,  1219,  1220,   715,   825,   717,   718,
    1213,   954,   969,   719,   826,   721,   722,   723,   724,   962,
     725,   726,   959,    32,    33,    34,    35,    36,   620,    37,
     584,    38,    39,    67,   253,   435,    40,   353,   354,   355,
     356,   357,   358,   359,   360,   867,   361,  1145,  1398,   604,
     605,   872,   606,    41,    42,   391,   392,   393,   887,   883,
    1599,   186,   628,   396,    44,  1495,   806,    45,    46,    47,
     587,   343,  1404,  1815,   165,   166,   167,   341,   591,   592,
     593,   858,   855,  1124,  1125,  1126,  1127,  1909,  1959,  1985,
    1128,  1129,  1130,  1131,  1914,  1915,  1916,  1132,  1393,   171,
     172,  1391,  1634,  1395,  1808,  1809,  1644,   465,   776,  1523,
    1031,  1524,  1525,  1526,  1739,  1937,  1940,   189,   630,   631,
     286,  1650,  1159,   402,  1600,  1601,    50,    51,  1602,    53,
      54,  1801,   466,   610,  1603,  1604,    55,  1114,  1619,  1620,
    1375,  1624,   313,   314,   561,   575,   576,   842,   518,   519,
     613,   759,  1006,   760,   761,   762,   763,  1267,   764,   765,
     766,   767,   768,   769,   770,   771,   772,   773,  1008,  1270,
    1271,   774,   521,   522,   523,  1414,   524,   525,   526,   527,
     528,   529,   530,  1622,    86,   288,   273,  1211,  1221,   862,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   540,
    1746,   541,  1076,   542,   543,   544,   545,   546,   547,  1062,
     548,   792,  1328,   793,  1325,   549,  1055,   342,   400,   550,
     551,   749,  1444,  1087,   262,   909,  1250,   294,   235,   291,
    1949,   267,  1938,  1137,  1606,   624,  1133,   152,   552,   245,
     247,  1369,   810,   279,   180,   553,   154
};

static const short yypact[] =
{
    1706,  1484,    72,  1978,   366,   825,   209,  4159,    95,  1883,
     487,   269,  1698,    72,   664,   385,  6911,   405,  6911,   265,
     428,   281,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,   580,   113,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  8914,  6911,   538,
    6911,  6911,  6911,  6911,  6911,-32768,-32768,   555,   705,  6911,
    6911,  6911,   709,  6911,   529,  6911,  6911,-32768,  6911,  6911,
    6911,-32768,   681,-32768,   540,-32768,-32768,   674,  6911,-32768,
    6911,  6911,  6911,   688,  6911,  6911,  6911,  6911,   529,  6911,
    6911,  6911,  6911,  6911,-32768,  6911,   499,   638,-32768,-32768,
  -32768,   508,-32768,   508,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   687,   548,   698,-32768,
      57,-32768,-32768,-32768,-32768,  6911,  6911,  6911,   737,   746,
    1009,  6911,   566,   288,   326,   533,   597,-32768,  6911,   795,
    1048,   745,-32768,  6911,-32768,  6911,  6911,  6911,  6911,-32768,
  -32768,-32768,  6911,  1847,-32768,   730,   725,   901,   644,   666,
     753,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   171,-32768,-32768,-32768,   705,-32768,   755,-32768,   302,
  -32768,-32768,   706,-32768,   255,-32768,-32768,   490,-32768,-32768,
     693,-32768,   807,-32768,-32768,   805,-32768,   712,-32768,-32768,
     966,-32768,-32768,-32768,-32768,   705,-32768,-32768,   182,-32768,
     706,-32768,-32768,   212,-32768,   942,-32768,   953,-32768,   508,
  -32768,-32768,-32768,-32768,-32768,-32768,   943,-32768,-32768,   755,
  -32768,   948,-32768,   842,-32768,-32768,-32768,   755,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  7817,  6911,
     907,-32768,  6911,-32768,-32768,-32768,   895,   500,   932,  5085,
    6911,  1000,   804,  1035,  1038,  1042,   984,  1017,    98,   878,
  -32768,-32768,  8077,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  5997,-32768,-32768,-32768,   974,  6911,  1029,   952,   162,
    6911,   977,-32768,-32768,  1048,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,  6911,-32768,-32768,-32768,   705,-32768,   706,-32768,
     942,-32768,   508,  1039,   996,-32768,-32768,-32768,  1101,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1089,  6911,  6911,  1055,   934,-32768,  8077,  1030,-32768,
    1030,  8077,-32768,    91,   927,   938,   949,   753,-32768,  1176,
     756,-32768,  6911,-32768,   171,-32768,-32768,-32768,-32768,  1157,
  -32768,-32768,  6911,   907,  3245,  1005,  3512,   928,-32768,  8914,
  -32768,-32768,   576,  1025,-32768,-32768,-32768,   944,   944,   705,
    1114,   705,   135,   712,-32768,-32768,  8879,-32768,  6911,  6911,
    6911,-32768,   907,-32768,-32768,  1200,   396,   945,  6911,  1225,
    6911,-32768,    55,  6391,  4691,  1103,-32768,-32768,   956,   969,
     970,  1063,-32768,-32768,   975,-32768,   978,-32768,   979,   980,
  -32768,   982,-32768,-32768,-32768,-32768,   983,   992,  1083,  1084,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,   999,-32768,-32768,
    6257,  1004,  1010,-32768,  1012,  1140,  1013,  1015,  1028,  1034,
    1037,  1040,  8077,  8077,  6517,  6911,-32768,  1106,  1002,-32768,
    1394,-32768,-32768,  1007,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,  1041,  1024,  1043,  8879,  1018,   475,-32768,-32768,  1052,
    1313,   561,-32768,-32768,  6911,-32768,  6911,  6911,   274,-32768,
  -32768,-32768,-32768,  6911,  6911,   595,-32768,  6911,-32768,-32768,
     895,  1186,  5085,-32768,  6911,  6517,   377,-32768,-32768,  8077,
  -32768,  1274,  1054,-32768,   180,-32768,   322,-32768,   114,-32768,
     147,-32768,   147,-32768,  1056,-32768,    99,-32768,  1178,-32768,
     330,-32768,-32768,  1069,  1068,-32768,   907,  1080,  1352,-32768,
  -32768,-32768,-32768,   337,  1289,-32768,  1081,  6911,-32768,-32768,
    1091,-32768,   195,-32768,-32768,   219,-32768,  1292,  1300,-32768,
     705,-32768,  1302,-32768,-32768,  1357,-32768,  1358,-32768,-32768,
    1351,-32768,  8879,-32768,   705,  1285,  8879,   622,-32768,  1388,
    6911,-32768,  8588,-32768,   525,-32768,  6911,   719,   255,  1230,
     123,  1270,  1234,-32768,-32768,-32768,   148,  1275,   451,-32768,
    1219,   529,-32768,   705,-32768,  1397,  1329,-32768,   135,-32768,
  -32768,-32768,    76,  1239,-32768,-32768,-32768,  1240,-32768,  1288,
    1146,-32768,  1350,  1154,   506,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,  1159,-32768,-32768,-32768,
     121,  1158,  1160,    71,-32768,  1154,-32768,-32768,   705,-32768,
     706,-32768,  1398,  1445,-32768,   705,   705,-32768,  3245,   490,
  -32768,  1342,-32768,  1317,  1181,-32768,  4167,  1341,-32768,-32768,
  -32768,  1354,  1183,  4951,  1189,   993,  1360,  1382,  4431,   226,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,  2342,   624,  1346,   685,  8077,   152,
  -32768,  6911,  8077,   717,   776,   846,  1387,-32768,-32768,  8077,
    5211,  1394,   465,  1293,  8077,  8077,  5211,  1413,  8077,  8077,
    8077,  8077,  8077,  5471,   246,   246,  1192,   113,   414,   655,
  -32768,-32768,  5345,-32768,  8077,  6911,  8077,  8077,  8077,  8077,
    8077,  8077,  6777,  3778,-32768,-32768,  1463,  1196,  6911,-32768,
    6911,  1449,  6911,-32768,  1320,  1321,-32768,-32768,  1294,  5085,
  -32768,  1437,   659,-32768,  1215,-32768,  1437,  6911,  6911,  1217,
  -32768,   414,-32768,   460,  5871,  1315,  8077,-32768,  6911,-32768,
     620,-32768,-32768,   161,-32768,  1322,-32768,-32768,-32768,  6911,
     612,  6911,-32768,-32768,  6911,  1103,-32768,  8077,  6911,  1497,
    3245,-32768,-32768,-32768,-32768,   337,  6911,  1430,  1228,  8077,
    1277,  8077,   705,-32768,   467,-32768,-32768,-32768,  1231,-32768,
  -32768,-32768,-32768,   705,-32768,   150,  6911,-32768,-32768,-32768,
  -32768,  1501,-32768,  1235,  1481,-32768,  1502,-32768,  1442,  1444,
     508,-32768,-32768,-32768,-32768,-32768,  1511,-32768,-32768,-32768,
  -32768,-32768,  6911,  1491,  1351,  8686,   529,  1410,  1412,  1419,
  -32768,   529,-32768,-32768,   632,-32768,   887,-32768,-32768,  1389,
     705,-32768,  6391,   237,  1384,-32768,-32768,-32768,   529,-32768,
    1146,   388,-32768,-32768,-32768,  1481,   388,  1391,   388,-32768,
     529,   529,  1260,   529,-32768,-32768,   907,  6911,-32768,  1452,
  -32768,   662,-32768,-32768,-32768,   576,  1030,  6911,   671,    37,
  -32768,-32768,  1264,   429,-32768,  6391,  6911,  1030,  5737,-32768,
  -32768,  1030,  4951,-32768,-32768,-32768,   130,-32768,   267,  1439,
    5211,  5211,  8077,  8077,  2070,  1265,   594,  2547,  8077,  2825,
     604,  3103,  3370,  7037,  3637,  3904,  4171,-32768,-32768,  1446,
  -32768,  1272,   666,  8077,  8077,   174,  8077,  1273,  8077,  1280,
     491,  8077,  8077,  8077,  8077,  8077,  8077,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1498,  1332,   157,  8077,-32768,
    5211,  8077,   498,   654,  1082,   129,  6911,   605,   645,   672,
     682,   699,-32768,-32768,-32768,   258,  7297,-32768,   666,-32768,
  -32768,  5611,-32768,  1281,-32768,  1276,   220,-32768,-32768,  1547,
     246,   246,   819,   819,  1394,-32768,   683,-32768,-32768,-32768,
  -32768,-32768,   708,-32768,  1392,  1561,  1449,-32768,-32768,  4575,
    4575,  1454,-32768,   138,  1473,  6911,-32768,  8077,  1473,  1519,
    1520,-32768,-32768,  4825,  1297,   542,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1298,  5211,  1516,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   524,-32768,-32768,  1346,  1394,
  -32768,-32768,   711,  1030,-32768,-32768,   337,-32768,-32768,-32768,
    1394,-32768,   944,  1576,-32768,   467,-32768,  1305,  5211,  2655,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1449,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,   508,   508,  1355,  1323,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   127,   185,   229,-32768,
    1599,-32768,-32768,-32768,-32768,  6911,-32768,-32768,-32768,-32768,
     366,   722,  1453,  1463,  1331,-32768,  1219,   736,-32768,   567,
  -32768,  1328,  6911,   586,  1333,  1334,   529,  1335,  1602,   490,
    1449,   944,  1300,  3245,-32768,  1275,  1429,  2977,-32768,   529,
    1493,  1575,-32768,  4167,-32768,   529,   308,-32768,-32768,  1579,
  -32768,-32768,  1339,  1344,-32768,  5211,  5211,-32768,  5211,  5211,
  -32768,  1638,   187,  1394,  1361,-32768,-32768,  1362,  1394,  6517,
  -32768,-32768,  1585,-32768,   592,  1364,  1366,  1394,   207,  1367,
    1370,  1394,  8077,  8077,  1265,  8077,  7557,  1371,  1393,  1394,
    1395,  1399,  1394,  8077,  1394,  1401,  1402,  1394,  1403,  1404,
    1394,  1406,  1407,  1394,-32768,  4315,-32768,   707,   723,-32768,
     835,-32768,   844,  8077,-32768,   853,   957,  1006,  1026,  1049,
    1074,  8077,  8077,  8077,  1394,  1621,   362,  1394,  1509,  8077,
  -32768,  8077,  8077,  8077,  8077,-32768,-32768,-32768,-32768,-32768,
  -32768,  8077,-32768,  8077,  1353,-32768,   772,  5611,  6651,-32768,
  -32768,  6911,-32768,-32768,  6911,  8306,-32768,-32768,  6911,  6911,
    6911,  6911,  6911,  6911,  1482,  1414,-32768,  1415,-32768,-32768,
    1482,-32768,   434,   434,    94,-32768,-32768,   791,-32768,  4575,
    4575,  1396,   601,  1411,  5871,  1559,-32768,  1559,   660,  1559,
    1614,  1618,  8077,  1134,  1686,  1633,-32768,-32768,-32768,   666,
    1449,-32768,  1429,-32768,   480,  1219,  1577,  1219,-32768,  5211,
     133,   867,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    2655,  2655,-32768,  1561,  8077,  2655,  1086,  1563,  1355,   681,
  -32768,  5211,-32768,-32768,-32768,  1584,  1586,  1587,  1589,  1590,
    1592,-32768,-32768,-32768,-32768,-32768,  1421,   366,-32768,  1531,
  -32768,-32768,   388,-32768,   231,   388,-32768,   388,-32768,  1463,
  -32768,-32768,  1443,-32768,-32768,   576,  1561,  1219,-32768,  1300,
  -32768,-32768,  1711,-32768,-32768,  1447,-32768,  1228,-32768,  1448,
     705,   144,  1451,-32768,-32768,   705,-32768,-32768,-32768,  1638,
  -32768,  1638,  8077,  1030,  1030,  1455,   113,   806,  8077,  1673,
  -32768,  1030,  1030,  8077,  1030,  1030,   191,  1394,-32768,  1227,
    8077,  1394,  1030,  1030,  1030,  1030,  1394,  1030,  1030,  1030,
    1030,  1030,  1030,-32768,  1456,  1460,   777,  6911,-32768,-32768,
    1464,  8879,-32768,-32768,  1118,-32768,-32768,-32768,-32768,-32768,
  -32768,  1147,  1386,-32768,-32768,  8077,  8077,  1690,  1570,  1155,
     858,  1102,  1169,  1204,  8077,-32768,-32768,-32768,-32768,  8879,
    1466,-32768,-32768,  1720,  1721,  6911,    83,  1472,  1471,  1678,
    8077,  1477,  1478,  6131,  6911,  6911,-32768,-32768,-32768,  1714,
    2334,-32768,  1480,  1485,  1486,-32768,-32768,  1488,-32768,  1489,
    1494,-32768,   156,  1496,  1499,-32768,-32768,-32768,  1504,  1710,
    1515,  1517,  1524,  1525,  1527,-32768,  1512,   -42,-32768,-32768,
  -32768,-32768,-32768,-32768,  1718,-32768,  4575,  4575,-32768,  1625,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  1536,-32768,-32768,
    1805,-32768,   542,-32768,-32768,-32768,  1731,-32768,  5871,  5871,
     869,  6911,  8077,  5211,  1103,-32768,  1561,-32768,-32768,-32768,
  -32768,   887,-32768,-32768,   141,-32768,-32768,-32768,-32768,-32768,
    1394,-32768,  1795,  1086,-32768,   834,  6911,-32768,  1741,   143,
    1641,  1743,  1644,  1781,  1746,  1783,  1449,-32768,-32768,  1539,
  -32768,  1540,  1737,  1795,   231,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  1449,  1275,-32768,   887,  1599,  1708,-32768,
  -32768,-32768,  1722,    88,-32768,-32768,  1463,  1724,  1394,  1549,
    1550,-32768,   666,-32768,  1394,  8077,  1551,  1552,  1394,  1553,
    1555,  8077,  8077,  1394,  1556,  1557,  1558,  1565,  1568,  1571,
    1574,  1578,  1581,  1582,-32768,  4315,  1548,-32768,  6911,-32768,
  -32768,-32768,-32768,-32768,-32768,  8077,  1583,-32768,-32768,-32768,
    8077,-32768,-32768,  8077,-32768,-32768,  1290,   173,-32768,  1789,
    8077,  8077,-32768,  5211,  1394,-32768,-32768,-32768,-32768,-32768,
    6911,-32768,   167,-32768,  1663,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   153,  1588,-32768,-32768,-32768,  5611,-32768,-32768,
  -32768,-32768,-32768,-32768,  1740,-32768,-32768,-32768,-32768,   434,
    6911,-32768,  6911,  5871,-32768,  1616,-32768,-32768,  1593,-32768,
    1394,   226,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    1594,  6911,-32768,-32768,  1771,  1780,  6911,  1355,-32768,-32768,
     508,  6911,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1561,
  -32768,-32768,-32768,-32768,-32768,  1561,-32768,-32768,-32768,-32768,
  -32768,  8077,   705,-32768,  1669,-32768,   705,-32768,-32768,-32768,
    1394,-32768,-32768,-32768,-32768,  1394,  1394,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  6911,  1796,
    1394,-32768,-32768,  1307,-32768,  2655,  2655,-32768,-32768,  1597,
    1394,  1513,   145,-32768,  1792,-32768,  6911,  6911,   366,   109,
  -32768,-32768,  1631,  1799,  5211,  1281,-32768,-32768,    80,  1603,
  -32768,-32768,  5211,  1604,-32768,-32768,-32768,  8077,-32768,-32768,
  -32768,-32768,-32768,  1355,   508,-32768,  1760,-32768,-32768,-32768,
  -32768,  1599,-32768,  1394,-32768,-32768,-32768,  1601,  1606,  1609,
  -32768,-32768,-32768,-32768,  1030,  5611,  1725,  5611,-32768,-32768,
  -32768,-32768,  8395,   167,  8077,  5611,   146,  5611,  6911,-32768,
     226,  6911,-32768,-32768,  1760,   188,  1355,  1772,  1777,-32768,
  -32768,  1612,-32768,  6911,  6911,  1615,  1281,  8395,  1281,-32768,
  -32768,  1523,   134,  1858,  1281,   893,-32768,   912,  1355,   552,
     552,-32768,  1864,-32768,  1729,-32768,-32768,-32768,-32768,  1622,
  -32768,  1860,  5611,  1877,  1867,  8395,  6911,-32768,-32768,-32768,
    1715,   198,-32768,-32768,-32768,-32768,  8395,   139,  6911,  8395,
  -32768,-32768,-32768,-32768,-32768,-32768,  8395,-32768,-32768,-32768,
    1917,  1918,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,  1340,  1759,-32768,  1761,  1605,   553,-32768,
    1076,-32768,-32768,-32768,  -898,-32768,  -684,-32768,  -581,  1764,
    1610,  1347,  1092,-32768,-32768,-32768,   689,-32768,  1191,   946,
  -32768,-32768,   694,-32768,-32768,   951,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   715,-32768,-32768,   972,-32768,-32768,   254,
  -32768,-32768,  1532,  1868,-32768,-32768,  -397,-32768,-32768,-32768,
    1510,-32768,-32768,  1252,  -411,-32768,-32768,-32768,   779,-32768,
   -1407,-32768,-32768,  1591,  1075,   721,  1533,  1044,   732,-32768,
     275,   -55,  -305,-32768,  -374,-32768, -1296,-32768,-32768,   296,
  -32768,  1566, -1294,-32768,-32768,-32768,-32768, -1372,   306,     8,
       6,    14,   -14,-32768,-32768,-32768,-32768,  -147,  -318,-32768,
    1085,-32768,  -753,-32768, -1050,-32768,   883,-32768,-32768,-32768,
  -32768,-32768, -1380, -1042,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -788, -1039,-32768,
  -32768,-32768,-32768,-32768,   218,-32768,    40,-32768,-32768,-32768,
  -32768,-32768,  1994,-32768,-32768,-32768,-32768,-32768,  1167,-32768,
  -32768,  -726,   352,  -386,  -780,   606,-32768,-32768,  -621,  -866,
  -32768,-32768, -1109, -1540,-32768,-32768,  1077,-32768,-32768,  1598,
  -32768,  1345,  1595,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,  1607,-32768,-32768,-32768,-32768,
  -32768,  -624,  -517,-32768,  1050,   565,  -631,  -424,-32768,-32768,
  -32768,-32768, -1099,-32768,  -416,-32768,-32768,-32768,-32768,  1291,
  -32768,-32768,  1061,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,  2009,  1863,-32768,-32768,-32768,-32768,  1675,
  -32768,-32768,-32768,  1432,-32768,  1425,-32768,-32768,-32768,-32768,
    1162,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  -742,
       1,-32768,-32768,-32768,  -501,   882,-32768,  -474,   802,-32768,
    -228,   488,-32768,-32768,-32768,-32768,  1866,-32768,-32768,-32768,
    1179, -1222,-32768,-32768, -1010, -1449,-32768,-32768,-32768,-32768,
     913,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,  1947,
     -56,   651,  -266,-32768,-32768,   124,-32768,  -460,  -832,  1267,
  -32768,   309,-32768,   516,-32768,  -923,-32768,  -704,-32768,  1156,
    -588,-32768,-32768,  -956,  2047,  2048,-32768,-32768,  2049,-32768,
  -32768,-32768,  1440,-32768,  -263,-32768,-32768,  1205, -1245,-32768,
     937,-32768,  -105,  -787,  -614,-32768,-32768,-32768,  -784,   -49,
    -529,  -761,-32768,  -470,-32768,-32768,-32768,    62,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,  -557,-32768,   768,
  -32768,  1184, -1119,-32768,  -480,-32768,   646, -1101,  -152, -1085,
   -1068, -1059,   -52,    -2,     4,-32768,   -71,  -875,  -538,  -146,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768, -1258,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   727,-32768, -1118,-32768,-32768,   468,-32768, -1049,
     444,  1104,   837,   239,  -748,  1400, -1259,-32768,   -66,  1968,
  -32768,     9,   -50, -1286,   492,  -277,   -40,   -43,  -168,    -1,
     -53,  -287,-32768,   -98,   -51,    -9,-32768
};


#define	YYLAST		9192


static const short yytable[] =
{
     153,    43,    85,   305,   265,   363,   244,   174,   315,   181,
     239,   334,   337,   888,   260,   173,   258,   335,   338,   275,
     261,   242,   716,   277,  1082,   650,   296,   302,  1084,  1057,
     720,   690,   570,   270,   809,  1065,   282,   824,   915,   652,
     807,   681,  1346,  1148,   890,   843,   300,   304,   233,   236,
    1413,   240,   243,   174,   248,   251,  1667,   301,   861,   306,
     263,   236,   251,  1356,   243,   308,   174,   248,  1415,   280,
     153,   251,  1032,   250,   845,  1099,  1306,  1103,   272,   292,
     372,   174,   295,   236,  1416,   292,   251,   240,   243,   173,
     174,   248,   280,   153,   251,  1563,   243,  1605,   303,   611,
     322,  1417,   272,  1078,   299,   659,   321,   588,   323,   366,
    1418,   807,   307,  1382,  1450,   994,   626,   656,   985,  1235,
    1419,   370,  1345,   452,  1657,  1658,  1471,  1423,  1623,  1661,
    1150,   373,  1665,  1187,   733,   901,   368,  1010,  1255,   905,
     945,  1010,   967,  1154,  2003,   916,   263,   174,   263,  1010,
     875,  1010,   153,  1010,  1010,  1153,  1952,  1849,   339,   251,
     870,   582,  1957,  1703,   364,  1010,   236,   243,   174,   280,
    1239,  1641,   933,   181,   459,   409,  1895,   345,   685,  1175,
    1466,   636,  1177,   416,  1309,  1853,  2026,   600,   600,  1804,
     857,  1184,   937,  1172,   614,  1492,  1000,   815,  1036,  1721,
    1773,  1372,   816,   815,   -81,  1543,   284,  1435,   816,  1902,
     815,  1547,  1370,   447,   815,   816,  1885,  1782,   815,   816,
     410,   616,  1896,   816,   105,   636,   453,   893,   938,  1437,
     815,   319,  1989,  1897,  1010,   816,   105,   448,   716,   187,
     285,  2023,   815,  -707,  1816,   952,   720,   816, -1075,  1260,
    1261, -1079,  1230,  -377,  1011,  1256,   972,   863,  1011,   424,
     558,  1503,   425,   562,   865,    66,  1011,   618,  1011,   815,
    1011,  1011,    90,  1439,   816,  1258,   351,   690,   926,   934,
     837,   815,  1011,   999,   807,   601,   816,   157,  1007,  1848,
    1209,   188,  2024,  1082,   607,   570,   597,  1084,   879,  1326,
     263,  1413,  1413,   263,   426,   420,  1413,  1436,    14,  1440,
     571,   153,  1373,   612,   615,  1768,  1769,   454,  1854,  1415,
    1415,  1341,  1605,   449,  1415,  1240,   965,   411,   939,  1761,
    1323,   935,  1898,   866,  1086,  1416,  1416,   240,   602,   602,
    1416,   174,  1096,   742,   320,   412,   637,  1783,  1704,  1682,
    1646,  1011,  1417,  1417,  1911,   106,   603,  1417,   450,   973,
    1691,  1418,  1418,   651,   953,  1438,  1418,   832,  1990,  1665,
    1010,  1419,  1419,   558,  1632,   261,  1419,   667,   421,   413,
     671,  1659,   728,   625,   625,   320,   871,   682,   333,   684,
     686,  1970,  1259,   891,  1422,  1135,  1953,   732,  1427,  1428,
     815,  1426,   739,   263,   737,   816,   614,  1410,  1802,   730,
     741,  1432,   968,   263,  1403,   263,  1334,   263,  1193,  1257,
     672,  1347,  1655,  1217,  1695,  1400,  1347,  1747,  1748,  1173,
    1817,   882,  1832,  1209,  1946,  1983,  1037,   815,  1454,   236,
     243,   248,   816,  1645,  1399,  1254,  1420,  1421,  1246,   174,
    1476,   240,  1886,   168,   750,  1925,   817,   818,   819,   820,
     175,   748,   817,   818,   819,   820,   838,  -707,  1434,   817,
     818,   819,   820,   817,   818,   819,   820,   817,   818,   819,
     820,  1628,  1629,   815,   331,  1236,   736,  1011,   816,   817,
     818,   819,   820,   430,  1488,  1489,  1252,  1490,  1491,  1626,
    1253,   817,   818,   819,   820,   155,   811,   821,   834,   182,
    1167,  1894,  1349,  1169,   815,  1058,  1059,   841,   331,   816,
     464,   846,   331,  1247,  1210,   612,   836,   963,   999,  1182,
     819,   820,   999,   850,   946,  1545,   254,   255,  1483,   339,
     817,   818,   819,   820,   108,   859,   176,  1342,  1329,  1330,
     183,  1963,   570,   169,  1907,   174,  1200,   243,   571,  1556,
     170,   839,    83,  1557,   174,  1162,   431,   184,   174,   156,
    1484,   884,  1979,   571,  1680,   181,   332,   177,   562,   340,
     110,   673,   333,   976,   331,  1846,  1376,   918,   894,  1942,
    1943,   674,   178,   964,  1991,   254,   255,  2001,  1558,  1694,
    1099,   656,   902,   237,  1813,   860,  1385,  1855,  1465,  1155,
     947,  1147,   994,   917,   336,  1243,  2009,   874,   263,  1386,
     333,  1282,  1381,  1477,  1121,  2020,  1839,  -747,   815,  1805,
      83,   948,  1882,   816,  1163,  1283,  2025,  1377,  1499,  2028,
    1272,   185,  1387,  1845,  1085,   111,  2029,   331,  1654,    84,
     398,   910,  1402,   263,  1060,   112,  1388,   910,  1228,   817,
     818,   819,   820,  1677,   919,  1385,   852,  1396,   815,  2010,
    1669,  1216,  1986,   816,   943,  1100,   975,   815,  1386,  1284,
     113,   419,   816,   979,   980,   272,  1500,  1331,  1273,   261,
     399,   339,   284,  1389,  1605,   815,   817,   818,   819,   820,
     816,  1387,   252,  1079,   920,   815,   614,  1285,  2011,  1027,
     816,  1648,  1274,   252,  1140,  1388,   268,    84,  1397,  1605,
    2021,  1649,   815,   339,  1385,    83,   285,   816,   333,   263,
     815,  1033,  1028,   462,   944,   816,   287,  1386,  1693,   289,
     329,   467,   817,   818,   819,   820,   815,  1605,  1905,  1122,
      14,   816,  1389,   297,  1143,  2012,  1859,   403,  1605,  2027,
    1387,  1605,   828,  1041,   829,  1405,  1413,  1413,  1605,   404,
    1144,  1286,   251,   817,   818,   819,   820,   844,   311,   558,
    1314,   558,   339,  1108,  1415,  1415,   648,   309,  1029,  1497,
    1039,  1119,  1712,  1424,  1425,  1496,   312,  1929,  1140,   649,
    1416,  1416,   405,  1930,   406,  1390,  1088,  1433,   836,   316,
     437,  1389,  1812,   607,   263,  1146,   331,  1417,  1417,   263,
     318,   263,  1043,   263,   589,   612,  1418,  1418,   331,   164,
     571,   261,   595,   884,  1467,   317,  1419,  1419,   174,   243,
    1161,  1668,   815,  1139,   438,   243,   439,   816,   832,  1138,
     833,  1171,   339,  1550,  1455,  1201,  1736,   558,   815,  1456,
     174,   324,   174,   816,  1390,  1195,  1243,   815,   440,   263,
     325,   263,   816,  1455,   346,  1818,   815,   625,  1459,  1819,
    1820,   816,  1811,  1189,    87,    88,  1194,   817,   818,   819,
     820,  1737,  1045,   441,  1336,   163,   164,   263,  1206,  1224,
    1225,   362,  1227,  1738,   254,   255,  1976,  1742,  1978,   906,
     394,   907,  1640,  1085,  1681,  1847,  1982,   395,  1984,  1690,
     397,  1229,   928,   263,   929,   401,   263,   817,   818,   819,
     820,  1237,  1795,  1797,  1337,  1531,   817,   818,   819,   820,
     272,  1332,   821,   750,  1080,  1199,  1115,   614,  1116,  1233,
    1207,  1234,  1824,   417,   817,   818,   819,   820,   990,   433,
    1238,  1338,  1214,  2017,   817,   818,   819,   820,   174,   434,
     821,  1339,  1350,  1825,   272,   272,   445,   272,   174,  1826,
     815,   817,   818,   819,   820,   816,   750,  1251,  1340,   817,
     818,   819,   820,   748,   422,  1351,  1528,  1352,  1233,  1082,
    1401,   456,  1892,  1084,   436,   817,   818,   819,   820,  1447,
   -1104,  1448,  1529,  1827,   458,   107, -1104,  1202,  1203,  1496,
    1496, -1104,   460,  1452, -1104,  1453,   463,  1496,  1496,   815,
    1496,  1496,   464,  1918,   816,  1796,  1798,  1919,  1496,  1496,
    1496,  1496,  1757,  1496,  1496,  1496,  1496,  1496,  1496,   815,
    1998,  1999,  1928,   108,   816,  1676,   559,   251,   893,  1347,
   -1104,  1555,   483,   484,   485,   109,   612,   563, -1104,  1932,
    1140,  1608,   815,   566,  1611,  1335, -1104,   816,   821,  1276,
    1627,  1280,   573,  1288,  1291,  1697,  1296,  1299,  1302,   110,
     493,   494,   574,   821, -1104,  1713, -1104,   815,   577,   578,
     571,   571,   816,   579,   884,   815,   580,   716,  -239, -1104,
     816, -1104, -1104, -1104,   243,   720,   660,   817,   818,   819,
     820,  1635,   583,  1637,  1532,   815,   817,   818,   819,   820,
     816,   598,   347,  1533,  -737,   817,   818,   819,   820,   581,
     599,   815,  1535,  1956,   857,   821,   816,  1752,   596,   608,
    1969,  1960,   621,  1971,   111,  1462,   821,  1082,  1806,  1082,
   -1104,  1084,   619,  1084,   112,   348,   622,  1082,  1478,  1082,
     815,  1084,   623,  1084,  1482,   816,   627,   629,   815,  1085,
    2006,   641,  2007,   816,   261,    14,  1908,   614,   562,   113,
    1996,   349,   815,   642,   350,  -737,  1445,   816,  -737,   832,
     633,  2008,   634,   643,  -239,  -737,   645,  -737,  1446,   654,
     351,  -737,   665,  1445,  1082,   668,  -737,   678,  1084,  -737,
    1621,  1621,  1625,   679,   263,  -239,   683,   815,   263,   734,
     272,  -239,   816,   738,  -737,   740,   352,   797,   775,   817,
     818,   819,   820,   272,   777,  1034,  1536,  1038,  -737,   272,
     815,  1042,  1044,  1046,   812,   816,  -737,   778,   779,  1651,
    -737,  1653,   780,   781,  1612,  -239,   782,   783,   784,  -737,
     785, -1061, -1104, -1104, -1104, -1104, -1104, -1104, -1104,   926,
     786,  1722,   787,   788, -1104,  -737, -1104,   789,   817,   818,
     819,   820,   794,   814,   821,  1537,   174,   827,   795,  1559,
     796,   798,   558,   799,  1527,   261,   612,  1610,   817,   818,
     819,   820,  -737,   815,  1851,  1538,   800,   823,   816,  1609,
    1613,  1696,   801,   831,  -737,   802,  -737,   848,   803,   822,
     815,   817,   818,   819,   820,   816, -1079,   854,  1539,   263,
     830,   856,   263,   869,   873,  1564,  1607,  1987,   877,   571,
     243,   248,   571,   280,   153,   815,   817,   818,   819,   820,
     816,   878,   881,  1540,   817,   818,   819,   820,   880,  1333,
     571,   571,   885,  -737,   886,   243,   815, -1105,   889,   896,
     898,   816,   892, -1105,   817,   818,   819,   820, -1105,  1753,
     636, -1105,   895,  -737,   899,  1322,   903,  -737,   908, -1106,
     817,   818,   819,   820,   932, -1106,   936,  1743,  -440,   815,
   -1106,   941,   614, -1106,   816,   331,  1554,   815,   949,   950,
    -737,  -737,   816,  -737,   955,   956,   957, -1105,  1702,   817,
     818,   819,   820,  1707,   958, -1105,  1744,   817,   818,   819,
     820,   960,   961, -1105,  1751,  1679,   970,  1745,   971, -1106,
     966,   817,   818,   819,   820,   978,  1012, -1106,  1754,   977,
    1931, -1105,   815, -1105,   987, -1106,   989,   816,   990,   995,
    1013,   997,   187,  1807,  1066,   996, -1105,  1001, -1105, -1105,
   -1105,  1077,  1061, -1106,   967, -1106,   817,   818,   819,   820,
     333,  1104,   520,  1755,  1109,  1110,  1111,  1113, -1106,  1760,
   -1106, -1106, -1106,  1117,   839,  1134,  1014,  1151,  1141,   817,
     818,   819,   820,  1156,  1015,   832,   586,  1158,   174,  1168,
    1178,    56,  1016,  1179,  1180,   594,  1527, -1105,  1185,   911,
    1186,   612,    57,  1188,  1190,  1196,   815,  1197,  1212,    58,
    1017,   816,  1018,    59,  1198,  1205,   815,  1222,  1226, -1106,
    1231,   816,  1245,  1269,  1304,  1019,   236,  1020,  1021,  1022,
    1305,  1321,  1311,    60,  1767,  1251,  1251,  1313,  1347,  1348,
     815,  1607,   817,   818,   819,   820,  1353,  1355,  1371,  1884,
    1374,   632,  1379,  1380,  1384,   635,  1392,  1394,  1406,   817,
     818,   819,   820,  1409,  1429,  1945,  1941,  1047,  1048,  1049,
    1050,  1051,  1052,  1053,  1054,  2002,  1023,   571,   571,  1441,
    1830,  1431,  1464,  1449,   817,   818,   819,   820,  1457,  1085,
    1451,  1472,  1460,  1461,  1463,    61,  1239,  1480,  1486,   243,
     243,  1485,   364,  1487,  1910,   817,   818,   819,   820, -1105,
   -1105, -1105, -1105, -1105, -1105, -1105,  1010,  1621,  1498,  1493,
    1494, -1105,  1501, -1105,  1502,  1504,    62,   174,  1505,  1512,
      63, -1106, -1106, -1106, -1106, -1106, -1106, -1106,   817,   818,
     819,   820,  1544, -1106,  1614, -1106,   817,   818,   819,   820,
    1385,  1513,  1546,  1514,   791,  1630,  1633,  1515,  1879,  1517,
    1518,  1519,  1520,  1386,  1521,  1522,   804,   805,   808,  1638,
    1631,  1616,  1617,  1639,   -49,  1642,  1643,  1652,  1666,  1670,
    1678,  1671,  1672,     1,  1673,  1674,  1387,  1675,  1024,  1025,
    1026,   817,   818,   819,   820,  1926,   174,  1209,  1079,   240,
    1388,     2,  1692,  1698,  1527,  1700,  1715,  1701,  1912,     3,
    1706,  1749,   -49,  1750,  1711,  1734,     4,  1735,     5,    64,
       6,   562,  1758,  1741,   -49,     7,  1759, -1067,  1762,  1763,
     156,  1251,     8,  1765,  -339,  1771,  1776,  1389,   158,   851,
    1924,  1777,  1778,   853,  1779,  1780,     9,  1085,   -49,  1085,
    1781,  1927,  1784,  1903,   614,  1785,    10,  1085,  1794,  1085,
    1786,   263,  1787,   364,   243,   817,   818,   819,   820,  1799,
    1934,  1788,  1793,  1789,  1936,   817,   818,   819,   820,   614,
    1790,  1791,  1088,  1792,  1800,   857,  1803,   174,  1821,  1964,
    1831,  1833,   240,  1834,  1835,  1836,  1837,  1838,  1840,  1841,
    1948,  1842,  1850,  1852,  1085,  1856,  1878,   614,  1857,  1858,
    1861,  1862,  1863,   -49,  1864,  1867,  1868,  1869,   614,    11,
    1889,   614,  1772,   -49,  1870,  1913,    12,  1871,   614,    13,
    1872,    14,    15,  1873,  1906,  1922,  1923,  1874,   374,   240,
    1875,  1876,  1881,  1935,  1947,  1939,  1904,  1954,   -49,  1390,
    1917,  1955,  1965,  1920,   375,  1944,    16,   236,  1950,   107,
    1972,  1958,  1961,  1973,  1992,   376,  1951,  1974,  1977,  1994,
     159,  1997,   377,   612,  2000,  2005,   378,   379,  1989,  1990,
    2016,  2015,   562,  2018,  2019,   380,  2022,  2031,  2032,   326,
     847,   327,   565,  1618,  1120,   328,   381,   108,   612,   849,
     572,  1112,  1481,   988,  1479,    17,  1244,  1218,  1844,   109,
     951,   646,  1009,  1607,  1408,  1975,  1249,  1469,   283,  1138,
      18,    19,   263,   691,  1470,  1152,   612,   661,  1843,  1823,
    1183,   617,  1035,   110,   240,   240,  1040,   612,  1607,  1475,
     612,  1829,  1988,  1056,  1995,    20,  2014,   612,  1063,  1064,
     382,  1993,  1067,  1068,  1069,  1070,  1071,  1075,   383,  1357,
     663,  1176,  1901,  1980,    28,  1101,  1607,  1138,   520,  1814,
    1089,  1090,  1091,  1092,  1093,  1094,   520,  1607,  1647,  1251,
    1607,  1191,   653,   931,   644,    68,   974,  1607,  1223,   384,
    1689,  1215,   162,   385,   669,   330,    69,   868,   111,   609,
     864,  1142,   344,    70,   386,  1136,  1383,   293,   112,  1636,
     594,  1962,  1030,  1740,  1877,  1157,    71,    48,    49,    52,
     876,  1118,  1508,   387,   388,  1378,  1208,  1656,  1548,  1458,
    1921,  1149,   298,   113,     0,  1766,   927,     0,     0,     0,
       0,     0,     0,   632,     0,  1160,  1264,     0,     0,  1265,
       0,   114,     0,   468,   115,     0,   116,   117,   118,   119,
     469,   120,     0,     0,     0,     0,     0,     0,     0,   121,
     122,     0,   470,    72,     0,     0,     0,     0,   471,   472,
     123,   124,   389,   125,     0,   390,     0,     0,     0,    73,
     126,   127,     0,   128,   129,     0,   130,     0,     0,   131,
     132,   133,   134,   135,     0,    74,   473,   136,   137,   474,
       0,     0,     0,   138,   139,   140,     0,   141,   142,   143,
      75,   144,     0,     0,    76,     0,     0,    77,     0,     0,
       0,   145,     0,     0,   475,    78,   476,   477,     0,     0,
       0,     0,     0,     0,     0,     0,   478,     0,     0,     0,
     479,     0,     0,     0,    79,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1262,  1263,  1268,     0,
       0,  1277,  1278,  1281,     0,  1289,  1292,  1294,  1297,  1300,
    1303,     0,     0,     0,     0,     0,     0,  1307,  1308,     0,
    1310,     0,  1312,     0,     0,  1315,  1316,  1317,  1318,  1319,
    1320,     0,     0,  1266,     0,   480,     0,     0,     0,     0,
       0,   481,  1324,    80,     0,  1327,     0,     0,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
    1344,     0,     0,     0,     0,   483,   484,   485,   486,   254,
     255,   115,     0,   116,   117,   118,   119,     0,   120,   487,
       0,     0,     0,     0,     0,     0,   121,   122,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   123,   497,     0,
     125,   520,     0,   498,   499,     0,   500,   501,   502,     0,
     128,   129,   503,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   504,   140,     0,   505,   142,   143,     0,   144,     0,
     506,   507,   508,     0,   509,   510,     0,   511,   145,     0,
    1355,     0,   512,   513,     0,     0,     0,     0,   585,  1012,
     515,     0,     0,     0,   333,   815,     0,     0,     0,     0,
     816,     0,     0,  1013,     0,  1709,  1710,     0,     6,     0,
       0,     0,     0,  1716,  1717,  -294,  1719,  1720,     0,  1565,
    1566,     0,  1567,     0,  1724,  1725,  1726,  1727,     0,  1728,
    1729,  1730,  1731,  1732,  1733,     0,     0,     0,  1568,  1014,
       0,     0,     0,     0,  1569,     0,     0,  1015,     0,     0,
       0,     0,     0,     0,     0,  1016,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1017,     0,  1018,     0,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,  1019,     0,
    1020,  1021,  1022,     0,     0,     0,  1506,  1507,     0,  1509,
    1511,  1570,     0,     0,     0,     0,     0,  1516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     0,     0,  1534,     0,     0,
       0,     0,     0,     0,  1571,  1541,  1542,  1324,     0,  1023,
       0,     0,     0,  1324,    16,  1327,  1549,   520,  1551,     0,
       0,     0,     0,  1772,     0,  1552,     0,  1553,     0,     0,
       0,     0,   486,     0,     0,   115,     0,   116,   117,   118,
     119,     0,   120,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,  1275,     0,     0,  1265,     0,     0,     0,
     468,  1572,   124,     0,   125,     0,     0,   469,     0,     0,
       0,   126,   127,     0,   128,   129,   520,   130,     0,   470,
     131,  1573,   133,   134,   135,   471,   472,     0,   136,   137,
    1574,  1575,  1576,     0,   138,   139,   140,     0,   141,   142,
     143,     0,   144,     0,     0,     0,     0,     0,  1660,     0,
       0,     0,   145,   473,     0,     0,   474,     0,     0,     0,
       0,  1024,  1025,  1026,   817,   818,   819,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,     0,   476,   477,     0,     0,     0,     0,     0,
       0,     0,     0,   478,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1708,     0,     0,     0,
       0,     0,  1714,     0,     0,     0,     0,  1718,     0,     0,
       0,     0,     0,   471,  1723,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1266,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,   473,     0,     0,     0,     0,     0,     0,   482,  1324,
    1324,     0,     0,     0,     0,     0,     0,     0,  1756,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,   475,
     116,   117,   118,   119,  1764,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     480,   509,   510,     0,   511,   145,  1810,     0,     0,   512,
     513,  1279,     0,     0,  1265,   585,   482,   515,   468,     0,
       0,   333,     0,     0,     0,   469,     0,     0,     0,     0,
     483,   484,   485,     0,   254,   255,     0,   470,     0,     0,
       0,     0,     0,   471,   472,     0,     0,     0,     0,     0,
       0,     0,     0,   488,   489,   490,   491,   492,   493,   494,
     495,   496,     0,     0,     0,     0,     0,     0,   498,   499,
       0,   473,     0,     0,   474,     0,     0,   503,     0,  1860,
       0,     0,     0,     0,     0,  1865,  1866,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   475,
       0,   476,   477,     0,     0,     0,     0,     0,     0,  1880,
       0,   478,     0,     0,  1324,   479,     0,  1883,  1411,     0,
       0,     0,     0,     0,  1890,  1891,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1266,     0,
     480,     0,     0,     0,     0,     0,   481,     0,     0,     0,
       0,     0,     0,     0,   911,     0,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     483,   484,   485,   486,   254,   255,   115,     0,   116,   117,
     118,   119,     0,   120,   487,  1933,     0,     0,     0,     0,
       0,   121,   122,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   123,   497,     0,   125,     0,     0,   498,   499,
       0,   500,   501,   502,     0,   128,   129,   503,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   504,   140,     0,   505,
     142,   143,     0,   144,     0,   506,   507,   508,     0,   509,
     510,  1810,   511,   145,     0,     0,     0,   512,   513,  1287,
       0,     0,  1265,   585,     0,   515,   468,     0,     0,   333,
       0,     0,     0,   469,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   470,     0,     0,  1981,     0,
       0,   471,   472,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   473,
       0,     0,   474,     0,     0,   114,     0,     0,   115,     0,
     116,   117,   118,   119,     0,   120,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,   475,     0,   476,
     477,     0,     0,     0,   123,   124,     0,   125,     0,   478,
       0,     0,     0,   479,   126,   127,     0,   128,   129,     0,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   139,   140,
       0,   141,   142,   143,     0,   144,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   312,  1266,     0,   480,     0,
       0,     0,     0,     0,   481,   660,     0,     0,     0,     0,
       0,     0,     0,     0,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   483,   484,
     485,   486,   254,   255,   115,     0,   116,   117,   118,   119,
       0,   120,   487,     0,     0,     0,     0,     0,     0,   121,
     122,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     123,   497,     0,   125,     0,     0,   498,   499,     0,   500,
     501,   502,     0,   128,   129,   503,   130,     0,     0,   131,
     132,   133,   134,   135,     0,     0,     0,   136,   137,     0,
       0,     0,     0,   138,   504,   140,     0,   505,   142,   143,
       0,   144,     0,   506,   507,   508,  1290,   509,   510,  1265,
     511,   145,     0,   468,     0,   512,   513,     0,     0,     0,
     469,   585,     0,   515,     0,     0,     0,   333,     0,     0,
       0,     0,   470,     0,     0,     0,     0,     0,   471,   472,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,     0,     0,   474,
       0,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,   475,     0,   476,   477,     0,     0,
       0,     0,   123,   124,     0,   125,   478,     0,     0,     0,
     479,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1266,     0,   480,     0,     0,     0,     0,
       0,   481,   666,     0,     0,     0,     0,     0,     0,     0,
       0,   482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   483,   484,   485,   486,   254,
     255,   115,     0,   116,   117,   118,   119,     0,   120,   487,
       0,     0,     0,     0,     0,     0,   121,   122,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   123,   497,     0,
     125,     0,     0,   498,   499,     0,   500,   501,   502,     0,
     128,   129,   503,   130,     0,     0,   131,   132,   133,   134,
     135,     0,     0,     0,   136,   137,     0,     0,     0,     0,
     138,   504,   140,     0,   505,   142,   143,     0,   144,     0,
     506,   507,   508,  1295,   509,   510,  1265,   511,   145,     0,
     468,     0,   512,   513,     0,     0,     0,   469,   585,     0,
     515,     0,     0,     0,   333,     0,     0,     0,     0,   470,
       0,     0,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,     0,
     114,     0,     0,   115,     0,   116,   117,   118,   119,     0,
     120,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,   475,     0,   476,   477,     0,     0,     0,     0,   123,
     124,     0,   125,   478,     0,     0,     0,   479,     0,   126,
     127,     0,   128,   129,     0,   130,     0,     0,   131,   132,
     133,   134,   135,     0,     0,     0,   136,   137,     0,     0,
       0,     0,   138,   139,   140,     0,   141,   142,   143,     0,
     144,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1266,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,  1097,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
    1298,   509,   510,  1265,   511,   145,     0,   468,     0,   512,
     513,     0,     0,     0,   469,   585,     0,   515,     0,     0,
       0,   333,     0,     0,     0,     0,   470,     0,     0,     0,
       0,     0,   471,   472,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     473,     0,     0,   474,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,   475,     0,
     476,   477,     0,     0,     0,   123,   124,     0,   125,     0,
     478,     0,     0,     0,   479,   126,   127,     0,   128,   129,
       0,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,     0,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,     0,     0,     0,
       0,     0,  1098,     0,     0,     0,     0,  1266,     0,   480,
       0,     0,     0,     0,     0,   481,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   482,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   483,
     484,   485,   486,   254,   255,   115,     0,   116,   117,   118,
     119,     0,   120,   487,     0,     0,     0,     0,     0,     0,
     121,   122,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   123,   497,     0,   125,     0,     0,   498,   499,     0,
     500,   501,   502,     0,   128,   129,   503,   130,     0,     0,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
       0,     0,     0,     0,   138,   504,   140,     0,   505,   142,
     143,     0,   144,     0,   506,   507,   508,  1301,   509,   510,
    1265,   511,   145,     0,   468,   991,   512,   513,   693,     0,
       0,   469,   585,     0,   515,     0,     0,     0,   333,     0,
       0,   992,     0,   470,     0,   694,     0,    91,   695,   471,
     472,     0,     0,     0,    92,     0,     0,     0,    93,    94,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,    96,     0,
     474,     0,     0,     0,     0,     0,     0,     0,   696,     0,
       0,     0,     0,     0,   697,   698,   699,     0,   700,   701,
     702,     0,   703,     0,     0,   475,     0,   476,   477,     0,
       0,     0,     0,     0,     0,     0,     0,   478,     0,     0,
     704,   479,   705,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   706,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   707,     0,
       0,    99,     0,     0,  1266,   100,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,   101,     0,     0,     0,
     708,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,   483,   484,   485,   486,
     254,   255,   115,     0,   116,   117,   118,   119,     0,   120,
     487,     0,     0,     0,     0,   709,   710,   121,   122,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   123,   497,
    1027,   125,   711,     0,   498,   499,     0,   500,   501,   502,
       0,   128,   129,   503,   130,     0,     0,   131,   132,   133,
     134,   135,     0,  1028,   103,   136,   137,     0,     0,     0,
       0,   138,   504,   140,     0,   505,   142,   143,     0,   144,
       0,   506,   507,   508,   468,   509,   510,     0,   511,   145,
       0,   469,     0,   512,   513,     0,     0,     0,     0,   585,
       0,   515,     0,   470,     0,   333,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,  1029,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,   475,     0,   476,   477,     0,
       0,   121,   122,     0,     0,     0,     0,   478,     0,     0,
       0,   479,   123,   124,     0,   125,     0,     0,     0,  1002,
       0,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,    14,     0,     0,     0,
     754,     0,     0,   145,     0,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     254,   255,   115,     0,   116,   117,   118,   119,     0,   120,
     487,     0,     0,     0,     0,     0,  1358,   121,   122,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   123,   497,
       0,   125,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   128,   129,   503,   130,     0,     0,   131,   132,   133,
     134,   135,  1003,  1004,  1005,   136,   137,     0,     0,     0,
       0,   138,   504,   140,     0,   505,   142,   143,     0,   144,
       0,   506,   507,   508,   468,   509,   510,     0,   511,   145,
       0,   469,     0,   512,   513,     0,  1359,     0,     0,   758,
       0,   515,     0,   470,     0,   333,   751,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,  1360,     0,     0,     0,     0,  1361,   473,     0,     0,
     474,     0,  1362,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,  1363,     0,   120,     0,   475,     0,   476,   477,     0,
       0,   121,   122,     0,     0,     0,     0,   478,     0,     0,
       0,   479,   123,   124,     0,   125,     0,     0,     0,   753,
       0,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
     754,     0,     0,   145,     0,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     254,   255,   115,     0,   116,   117,   118,   119,     0,   120,
     487,     0,     0,     0,     0,     0,     0,   121,   122,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   123,   497,
       0,   125,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   128,   129,   503,   130,     0,     0,   131,   132,   755,
     756,   757,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   504,   140,     0,   505,   142,   143,     0,   144,
       0,   506,   507,   508,   468,   509,   510,     0,   511,   145,
       0,   469,     0,   512,   513,     0,     0,     0,     0,   758,
      14,   515,     0,   470,     0,   333,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   752,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   123,   124,     0,   125,     0,   478,     0,     0,
       0,   479,   126,   127,     0,   128,   129,     0,   130,   998,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
     754,     0,     0,  1123,     0,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     254,   255,   115,     0,   116,   117,   118,   119,     0,   120,
     487,     0,     0,     0,     0,     0,     0,   121,   122,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   123,   497,
       0,   125,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   128,   129,   503,   130,     0,     0,   131,   132,   755,
     756,   757,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   504,   140,     0,   505,   142,   143,     0,   144,
       0,   506,   507,   508,   468,   509,   510,     0,   511,   145,
       0,   469,     0,   512,   513,     0,     0,     0,     0,   758,
       0,   515,     0,   470,     0,   333,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   567,     0,   752,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   123,   124,     0,   125,     0,   478,     0,     0,
       0,   479,   126,   127,     0,   128,   129,     0,   130,   753,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
     754,     0,     0,     0,     0,     0,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     254,   255,   115,     0,   116,   117,   118,   119,     0,   120,
     487,     0,     0,     0,     0,     0,     0,   121,   122,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   123,   497,
       0,   125,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   128,   129,   503,   130,     0,     0,   131,   132,   755,
     756,   757,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   504,   140,     0,   505,   142,   143,     0,   144,
       0,   506,   507,   508,   468,   509,   510,     0,   511,   145,
       0,   469,     0,   512,   513,     0,     0,     0,     0,   758,
       0,   515,     0,   470,     0,   333,     0,     0,     0,   471,
     472,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   473,     0,     0,
     474,     0,     0,   114,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,     0,     0,     0,     0,
       0,   121,   122,     0,     0,   475,     0,   476,   477,     0,
       0,     0,   123,   124,     0,   125,     0,   478,     0,     0,
       0,   479,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,   132,   133,   134,   135,     0,     0,     0,   136,
     137,     0,     0,     0,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1081,     0,   515,   480,     0,     0,     0,
       0,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   483,   484,   485,   486,
     254,   255,   115,     0,   116,   117,   118,   119,     0,   120,
     487,     0,     0,     0,     0,     0,     0,   121,   122,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   123,   497,
       0,   125,     0,     0,   498,   499,     0,   500,   501,   502,
       0,   128,   129,   503,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   504,   140,     0,   505,   142,   143,  1072,   144,
       0,   506,   507,   508,  1073,   509,   510,  1074,   511,   145,
     468,     0,     0,   512,   513,     0,     0,   469,     0,   585,
       0,   515,     0,     0,     0,   333,     0,     0,     0,   470,
       0,     0,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   752,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   475,     0,   476,   477,     0,     0,     0,   123,   124,
       0,   125,     0,   478,     0,     0,     0,   479,   126,   127,
       0,   128,   129,     0,   130,   998,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,   515,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   758,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   475,     0,   476,   477,     0,     0,     0,   123,   124,
       0,   125,     0,   478,     0,     0,     0,   479,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1123,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,   590,     0,     0,     0,   585,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   475,     0,   476,   477,     0,     0,     0,   123,   124,
       0,   125,     0,   478,     0,     0,     0,   479,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,     0,     0,  -340,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,   790,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   585,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,    83,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   475,     0,   476,   477,     0,     0,     0,   123,   124,
       0,   125,     0,   478,     0,     0,     0,   479,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,    14,     0,    84,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   585,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   475,     0,   476,   477,     0,     0,     0,   123,   124,
       0,   125,     0,   478,     0,     0,     0,   479,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,  1098,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   585,  1095,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,   114,
       0,     0,   115,     0,   116,   117,   118,   119,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   121,   122,     0,
       0,   475,     0,   476,   477,     0,     0,     0,   123,   124,
       0,   125,     0,   478,     0,     0,     0,   479,   126,   127,
       0,   128,   129,     0,   130,     0,     0,   131,   132,   133,
     134,   135,     0,     0,     0,   136,   137,     0,     0,     0,
       0,   138,   139,   140,     0,   141,   142,   143,     0,   144,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1293,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   585,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1343,     0,     0,   473,     0,     0,   474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,     0,   476,   477,     0,     0,     0,     0,     0,
       0,     0,     0,   478,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   585,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,     0,   476,   477,     0,     0,     0,     0,     0,
       0,     0,     0,   478,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1510,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   585,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,     0,   476,   477,     0,     0,     0,     0,     0,
       0,     0,     0,   478,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,     0,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
     468,   509,   510,     0,   511,   145,     0,   469,     0,   512,
     513,     0,     0,     0,     0,   514,     0,   515,     0,   470,
       0,   333,     0,     0,     0,   471,   472,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,     0,   474,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,     0,   476,   477,     0,     0,     0,     0,     0,
       0,     0,     0,   478,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   480,     0,     0,     0,     0,     0,   481,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   483,   484,   485,   486,   254,   255,   115,     0,
     116,   117,   118,   119,     0,   120,   487,     0,     0,     0,
       0,     0,     0,   121,   122,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   123,   497,     0,   125,     0,     0,
     498,   499,     0,   500,   501,   502,     0,   128,   129,   503,
     130,     0,  1355,   131,   132,   133,   134,   135,     0,     0,
       0,   136,   137,     0,     0,     0,     0,   138,   504,   140,
       0,   505,   142,   143,     0,   144,     0,   506,   507,   508,
       6,   509,   510,     0,   511,   145,     0,  -296,     0,   512,
     513,  1565,  1566,     0,  1567,   585,     0,   515,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
    1568,     0,     0,     0,     0,     0,  1569,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1355,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       0,     0,     0,  1570,     0,     0,     0,     0,     0,     0,
    1565,  1566,     0,  1567,     0,     0,     0,     0,     0,     0,
       0,    14,     0,     0,     0,     0,     0,     0,     0,  1568,
       0,     0,     0,     0,     0,  1569,  1571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   486,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,     0,     0,     0,     0,
       0,     0,  1570,  1572,   124,     0,   125,     0,     0,     0,
       0,     0,     0,   126,   127,     0,   128,   129,     0,   130,
      14,     0,   131,  1573,   133,   134,   135,     0,     0,     0,
     136,   137,  1574,  1575,  1576,  1571,   138,   139,   140,     0,
     141,   142,   143,     0,   144,    16,     0,     0,     0,     0,
       0,     0,     0,     0,   145,     0,     0,     0,     0,     0,
       0,     0,     0,   486,     0,     0,   115,     0,   116,   117,
     118,   119,     0,   120,     0,     0,   692,     0,     0,   693,
       0,   121,   122,     0,     0,   911,     0,     0,     0,     0,
       0,     0,  1572,   124,     0,   125,   694,     0,     0,   695,
       0,     0,   126,   127,     0,   128,   129,     0,   130,     0,
       0,   131,  1573,   133,   134,   135,     0,     0,     0,   136,
     137,  1574,  1575,  1576,     0,   138,   139,   140,     0,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,   696,
       0,     0,     0,   145,     0,   697,   698,   699,     0,   700,
     701,   702,     0,   703,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   704,     0,   705,   692,     0,     0,   693,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   694,     0,     0,   695,     0,     0,
       0,     0,   706,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   707,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   696,     0,     0,
       0,   708,     0,   697,   698,   699,     0,   700,   701,   702,
       0,   703,     0,     0,     0,     0,   114,     0,     0,   115,
       0,   116,   117,   118,   119,     0,   120,     0,     0,   704,
       0,   705,     0,     0,   121,   122,   709,   710,     0,     0,
       0,     0,     0,     0,     0,   123,   124,     0,   125,     0,
       0,     0,     0,   711,     0,   126,   127,     0,   128,   129,
     706,   130,     0,     0,   131,   132,   133,   134,   135,     0,
       0,     0,   136,   137,     0,     0,     0,   707,   138,   139,
     140,     0,   141,   142,   143,     0,   144,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,     0,     0,   708,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   115,     0,   116,
     117,   118,   119,     0,   120,     0,     0,   692,     0,     0,
     693,     0,   121,   122,   709,   710,     0,     0,     0,     0,
       0,     0,     0,   123,   124,     0,   125,   694,     0,     0,
     695,   711,     0,   126,   127,     0,   128,   129,     0,   130,
       0,     0,   131,   132,   133,   134,   135,     0,     0,     0,
     136,   137,     0,     0,     0,     0,   138,   139,   140,     0,
     141,   142,   143,     0,   144,     0,     0,     0,     0,     0,
     696,     0,     0,     0,   145,     0,   697,   698,   699,     0,
     700,   701,   702,     0,   703,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   704,     0,   705,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   706,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     707,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   708,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   709,   710,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,     0,   711,   115,   191,   116,   117,   118,
     119,   192,   120,   193,   194,   195,   196,   197,   198,   199,
     121,   122,   200,   201,   202,   203,   204,     0,     0,   205,
     206,   123,   124,   207,   125,     0,     0,   208,   209,   210,
     211,   126,   127,   212,   128,   129,   213,   130,   214,   215,
     131,   132,   133,   134,   135,     0,     0,     0,   136,   137,
     216,   217,   218,   219,   138,   139,   140,   220,   141,   142,
     143,   221,   144,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   145
};

static const short yycheck[] =
{
       9,     0,     4,   101,    70,   173,    62,    16,   113,    18,
      60,   163,   164,   627,    69,    16,    68,   163,   164,    75,
      69,    61,   446,    76,   812,   411,    92,    98,   812,   790,
     446,   442,   319,    73,   514,   796,    79,   554,   662,   413,
     514,   438,  1081,   875,   632,   574,    96,   100,    57,    58,
    1169,    60,    61,    62,    63,    64,  1428,    97,   596,   102,
      69,    70,    71,  1105,    73,   105,    75,    76,  1169,    78,
      79,    80,   776,    64,   575,   823,  1032,   830,    74,    88,
     178,    90,    91,    92,  1169,    94,    95,    96,    97,    90,
      99,   100,   101,   102,   103,  1354,   105,  1355,    99,   362,
     156,  1169,    98,   807,    95,   423,   155,   335,   157,   175,
    1169,   585,   103,  1123,  1213,   746,   393,   422,   739,   985,
    1169,   177,  1078,   270,  1420,  1421,  1235,  1177,  1373,  1425,
     878,   182,  1426,   920,   452,   652,   176,     8,     8,   656,
     678,     8,    21,   885,    10,   662,   155,   156,   157,     8,
     610,     8,   161,     8,     8,   881,    47,  1697,     6,   168,
      61,    63,    82,    19,   173,     8,   175,   176,   177,   178,
     133,  1393,    49,   182,   279,     4,     9,   168,    43,   905,
    1230,    90,   908,   235,    10,    97,    47,    26,    26,  1638,
      10,   917,    44,    43,   362,     8,   753,    23,    46,     8,
    1580,    63,    28,    23,   149,  1323,    11,    80,    28,    56,
      23,  1329,  1110,   265,    23,    28,    43,    61,    23,    28,
      49,   368,    55,    28,   141,    90,    14,   638,    80,    44,
      23,   174,    44,    66,     8,    28,   141,    55,   662,   126,
      45,    43,    23,    63,  1651,   169,   662,    28,   290,  1010,
    1011,   293,   978,    22,   125,   125,   185,   143,   125,     4,
     309,    54,     7,   312,   117,   193,   125,   372,   125,    23,
     125,   125,    63,    44,    28,     8,   162,   688,   664,   156,
     567,    23,   125,   753,   758,   123,    28,    18,   758,  1696,
     196,   178,    94,  1081,   350,   582,   346,  1081,   616,  1060,
     309,  1420,  1421,   312,    49,     3,  1425,   180,   155,    80,
     319,   320,   174,   362,   366,  1574,  1575,   105,   230,  1420,
    1421,    63,  1580,   141,  1425,   288,   712,   156,   180,   246,
     173,   208,   165,   186,   814,  1420,  1421,   346,   177,   177,
    1425,   350,   822,   288,   287,   174,   255,   191,   204,   118,
    1400,   125,  1420,  1421,  1803,   260,   194,  1425,   176,   288,
    1459,  1420,  1421,   412,   288,   180,  1425,   287,   180,  1663,
       8,  1420,  1421,   422,  1384,   424,  1425,   426,    76,   208,
     429,  1423,   448,   392,   393,   287,   287,   439,   294,   441,
     255,  1931,   125,   174,  1174,   855,   287,   450,  1185,  1186,
      23,  1181,   458,   412,   456,    28,   574,  1168,  1630,   449,
     460,  1191,   291,   422,  1156,   424,   287,   426,   935,   289,
     429,   287,   289,   961,  1466,  1151,   287,  1545,  1546,   279,
     289,    94,   289,   196,   289,   289,   284,    23,  1218,   448,
     449,   450,    28,  1399,  1148,  1002,  1172,  1173,    19,   458,
    1237,   460,   279,    68,   463,  1827,   282,   283,   284,   285,
      55,   463,   282,   283,   284,   285,   192,   287,  1194,   282,
     283,   284,   285,   282,   283,   284,   285,   282,   283,   284,
     285,  1379,  1380,    23,   196,   986,    90,   125,    28,   282,
     283,   284,   285,     3,  1255,  1256,   997,  1258,  1259,  1374,
    1001,   282,   283,   284,   285,    18,   515,   287,   564,   244,
     896,  1770,   292,   899,    23,    50,    51,   573,   196,    28,
     190,   577,   196,    94,   287,   574,   566,    21,   998,   915,
     284,   285,  1002,   584,   680,   173,   199,   200,   230,     6,
     282,   283,   284,   285,    44,   223,   141,   289,    50,    51,
     122,  1923,   839,   168,  1799,   564,   942,   566,   567,  1347,
     175,   287,   196,  1347,   573,    98,    76,   286,   577,    82,
     262,   623,  1952,   582,  1449,   584,   288,   172,   627,    46,
      80,     5,   294,   730,   196,  1694,  1115,    62,   640,  1885,
    1886,    15,   187,    87,  1966,   199,   200,  1977,  1351,  1465,
    1348,   906,   654,    65,  1646,   283,    64,  1706,  1229,   886,
     681,   874,  1243,   662,   288,   989,  1988,   287,   627,    77,
     294,    17,  1123,  1237,   852,  2005,  1676,    85,    23,  1639,
     196,   683,  1750,    28,   167,    31,  2016,  1117,    46,  2019,
      46,    61,   100,  1693,   812,   145,  2026,   196,  1409,   283,
       6,   660,  1153,   662,   189,   155,   114,   666,   976,   282,
     283,   284,   285,  1443,   139,    64,   289,   143,    23,   117,
    1431,   283,  1958,    28,   223,   827,   728,    23,    77,    75,
     180,   237,    28,   735,   736,   681,    94,   189,    94,   738,
      46,     6,    11,   151,  1952,    23,   282,   283,   284,   285,
      28,   100,   147,   289,   179,    23,   874,   103,   156,    85,
      28,   231,   118,   147,   860,   114,     7,   283,   194,  1977,
    2006,   241,    23,     6,    64,   196,    45,    28,   294,   738,
      23,    46,   108,   289,   283,    28,   196,    77,  1464,    65,
     174,   297,   282,   283,   284,   285,    23,  2005,  1787,   289,
     155,    28,   151,    65,   142,   203,  1712,     4,  2016,  2018,
     100,  2019,   287,    46,   289,  1162,  1885,  1886,  2026,    16,
     158,   167,   781,   282,   283,   284,   285,   182,   140,   828,
     289,   830,     6,   832,  1885,  1886,    30,   288,   164,  1269,
     781,   847,  1496,  1179,  1180,  1269,   288,  1839,   944,    43,
    1885,  1886,    49,  1845,    51,   263,   815,  1193,   848,   122,
      98,   151,  1644,   869,   823,   871,   196,  1885,  1886,   828,
     122,   830,    46,   832,   336,   874,  1885,  1886,   196,   232,
     839,   880,   344,   885,  1231,   287,  1885,  1886,   847,   848,
     892,  1429,    23,   223,   132,   854,   134,    28,   287,   858,
     289,   903,     6,  1333,   287,   223,    79,   906,    23,   292,
     869,   124,   871,    28,   263,   936,  1240,    23,   156,   878,
     124,   880,    28,   287,    79,  1655,    23,   886,   292,  1659,
    1660,    28,  1643,   932,    59,    60,   935,   282,   283,   284,
     285,   114,    46,   181,   289,   231,   232,   906,   950,   970,
     971,   156,   973,   126,   199,   200,  1945,  1531,  1947,   287,
     180,   289,  1392,  1081,  1452,  1695,  1955,   192,  1957,  1457,
      19,   977,   203,   932,   205,   259,   935,   282,   283,   284,
     285,   987,  1616,  1617,   289,  1309,   282,   283,   284,   285,
     936,   287,   287,   952,   289,   941,   287,  1115,   289,   287,
     952,   289,   118,   198,   282,   283,   284,   285,   287,   266,
     289,   289,   958,  2002,   282,   283,   284,   285,   977,   162,
     287,   289,   289,   139,   970,   971,    10,   973,   987,   145,
      23,   282,   283,   284,   285,    28,   995,   996,   289,   282,
     283,   284,   285,   995,   288,   287,   289,   289,   287,  1787,
     289,    59,  1763,  1787,   199,   282,   283,   284,   285,   287,
      17,   289,   289,   179,    61,     6,    23,   130,   131,  1493,
    1494,    28,    79,   287,    31,   289,    78,  1501,  1502,    23,
    1504,  1505,   190,  1813,    28,  1616,  1617,  1817,  1512,  1513,
    1514,  1515,  1559,  1517,  1518,  1519,  1520,  1521,  1522,    23,
    1973,  1974,  1832,    44,    28,  1441,   149,  1066,  1469,   287,
      67,   289,   195,   196,   197,    56,  1115,   172,    75,  1849,
    1216,  1358,    23,   141,  1361,  1066,    83,    28,   287,  1017,
     289,  1019,    82,  1021,  1022,  1471,  1024,  1025,  1026,    80,
     223,   224,   288,   287,   101,   289,   103,    23,    63,    61,
    1109,  1110,    28,    61,  1156,    23,   122,  1531,    22,   116,
      28,   118,   119,   120,  1123,  1531,    30,   282,   283,   284,
     285,  1387,   244,  1389,   289,    23,   282,   283,   284,   285,
      28,   102,    84,   289,     0,   282,   283,   284,   285,   122,
     188,    23,   289,  1904,    10,   287,    28,   289,   174,   172,
    1930,  1912,   156,  1933,   145,  1226,   287,  1945,   289,  1947,
     167,  1945,   123,  1947,   155,   117,    65,  1955,  1239,  1957,
      23,  1955,    83,  1957,  1245,    28,   121,   243,    23,  1347,
     287,   254,   289,    28,  1233,   155,  1800,  1355,  1237,   180,
    1970,   143,    23,   255,   146,    61,  1205,    28,    64,   287,
     398,   289,   400,   254,   118,    71,    30,    73,  1210,    52,
     162,    77,   207,  1222,  2002,   287,    82,   192,  2002,    85,
    1372,  1373,  1374,   279,  1233,   139,   112,    23,  1237,    29,
    1226,   145,    28,   288,   100,    10,   188,    97,   135,   282,
     283,   284,   285,  1239,   288,   777,   289,   779,   114,  1245,
      23,   783,   784,   785,   148,    28,   122,   288,   288,  1405,
     126,  1407,   199,   288,  1362,   179,   288,   288,   288,   135,
     288,   288,   279,   280,   281,   282,   283,   284,   285,  1665,
     288,    54,   199,   199,   291,   151,   293,   288,   282,   283,
     284,   285,   288,   291,   287,   289,  1305,   279,   288,  1354,
     288,   288,  1351,   288,  1305,  1354,  1355,  1360,   282,   283,
     284,   285,   178,    23,  1700,   289,   288,   293,    28,  1359,
    1363,  1467,   288,    10,   190,   288,   192,   141,   288,   288,
      23,   282,   283,   284,   285,    28,   293,    63,   289,  1348,
     288,   287,  1351,   287,   166,  1354,  1355,  1961,   279,  1358,
    1359,  1360,  1361,  1362,  1363,    23,   282,   283,   284,   285,
      28,   293,    10,   289,   282,   283,   284,   285,   288,   287,
    1379,  1380,    83,   239,   293,  1384,    23,    17,   287,    22,
      22,    28,    90,    23,   282,   283,   284,   285,    28,   287,
      90,    31,    90,   259,    43,    63,   111,   263,    10,    17,
     282,   283,   284,   285,   174,    23,   136,   289,   174,    23,
      28,   136,  1580,    31,    28,   196,    63,    23,    21,    90,
     286,   287,    28,   289,   185,   185,   138,    67,  1480,   282,
     283,   284,   285,  1485,   288,    75,   289,   282,   283,   284,
     285,    91,   288,    83,   289,  1447,   288,    61,   288,    67,
     291,   282,   283,   284,   285,    10,    17,    75,   289,    61,
    1846,   101,    23,   103,   122,    83,   149,    28,   287,   128,
      31,   288,   126,  1641,    61,   121,   116,   288,   118,   119,
     120,   289,   189,   101,    21,   103,   282,   283,   284,   285,
     294,    42,   308,   289,   174,   174,   202,    60,   116,  1565,
     118,   119,   120,   288,   287,   190,    67,    10,   186,   282,
     283,   284,   285,    83,    75,   287,   332,   240,  1527,   288,
      19,    37,    83,   288,    43,   341,  1527,   167,    86,    27,
      86,  1580,    48,    22,    43,   125,    23,   125,   154,    55,
     101,    28,   103,    59,   125,   156,    23,   156,   288,   167,
      98,    28,   288,   288,   108,   116,  1565,   118,   119,   120,
     288,    63,   289,    79,  1573,  1574,  1575,   287,   287,   293,
      23,  1580,   282,   283,   284,   285,   184,    16,   124,   289,
     107,   397,    63,    63,   287,   401,   288,    71,    12,   282,
     283,   284,   285,   288,   239,    82,   289,   210,   211,   212,
     213,   214,   215,   216,   217,    82,   167,  1616,  1617,    10,
    1666,   288,    10,   160,   282,   283,   284,   285,   290,  1787,
     289,   192,   289,   289,   289,   141,   133,    52,   289,  1638,
    1639,    52,  1641,   289,  1802,   282,   283,   284,   285,   279,
     280,   281,   282,   283,   284,   285,     8,  1799,    63,   288,
     288,   291,   288,   293,   288,   288,   172,  1666,   288,   288,
     176,   279,   280,   281,   282,   283,   284,   285,   282,   283,
     284,   285,    51,   291,   192,   293,   282,   283,   284,   285,
      64,   288,   173,   288,   500,   289,   127,   288,  1738,   288,
     288,   288,   288,    77,   288,   288,   512,   513,   514,    85,
     289,   287,   287,    85,     6,    19,    73,   130,   145,   125,
     289,   125,   125,     7,   125,   125,   100,   125,   279,   280,
     281,   282,   283,   284,   285,  1830,  1735,   196,   289,  1738,
     114,    25,   289,    22,  1735,   288,    63,   289,   122,    33,
     289,    51,    44,   173,   289,   289,    40,   287,    42,   265,
      44,  1800,   286,   289,    56,    49,    36,    36,   286,   288,
      82,  1770,    56,   286,   286,    51,   286,   151,    70,   585,
    1826,   286,   286,   589,   286,   286,    70,  1945,    80,  1947,
     286,  1831,   286,  1782,  1952,   286,    80,  1955,    70,  1957,
     286,  1800,    82,  1802,  1803,   282,   283,   284,   285,   174,
    1852,   286,   290,   286,  1856,   282,   283,   284,   285,  1977,
     286,   286,  1821,   286,   278,    10,    85,  1826,    23,  1924,
      79,   180,  1831,    80,   180,    44,    80,    44,   289,   289,
    1896,    94,   124,   111,  2002,   111,   288,  2005,   289,   289,
     289,   289,   289,   145,   289,   289,   289,   289,  2016,   143,
      61,  2019,   189,   155,   289,   239,   150,   289,  2026,   153,
     289,   155,   156,   289,   124,    94,    86,   289,    21,  1878,
     289,   289,   289,   204,    82,    79,   288,   246,   180,   263,
     287,    82,   122,   289,    37,   288,   180,  1896,  1897,     6,
     289,   288,   288,   287,   122,    48,  1898,   288,   173,   122,
     202,   289,    55,  1952,   289,    47,    59,    60,    44,   180,
      50,   289,  1961,    36,    47,    68,   201,     0,     0,   160,
     580,   160,   317,  1370,   848,   161,    79,    44,  1977,   582,
     320,   839,  1243,   742,  1240,   229,   990,   965,  1684,    56,
     688,   409,   758,  1952,  1165,  1944,   995,  1232,    80,  1958,
     244,   245,  1961,   443,  1233,   880,  2005,   424,  1683,  1663,
     916,   370,   778,    80,  1973,  1974,   782,  2016,  1977,  1237,
    2019,  1665,  1964,   789,  1968,   269,  1990,  2026,   794,   795,
     133,  1967,   798,   799,   800,   801,   802,   803,   141,  1106,
     424,   906,  1774,  1953,     0,   828,  2005,  2006,   814,  1647,
     816,   817,   818,   819,   820,   821,   822,  2016,  1402,  2018,
    2019,   934,   414,   668,   407,    37,   725,  2026,   968,   172,
    1455,   960,    13,   176,   429,   162,    48,   602,   145,   354,
     598,   869,   166,    55,   187,   856,  1123,    90,   155,  1388,
     856,  1917,   775,  1527,  1735,   889,    68,     0,     0,     0,
     610,   846,  1284,   206,   207,  1118,   952,  1411,  1331,  1222,
    1821,   877,    94,   180,    -1,  1573,   666,    -1,    -1,    -1,
      -1,    -1,    -1,   889,    -1,   891,     6,    -1,    -1,     9,
      -1,   198,    -1,    13,   201,    -1,   203,   204,   205,   206,
      20,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,    -1,    32,   125,    -1,    -1,    -1,    -1,    38,    39,
     227,   228,   265,   230,    -1,   268,    -1,    -1,    -1,   141,
     237,   238,    -1,   240,   241,    -1,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,   157,    66,   254,   255,    69,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
     172,   268,    -1,    -1,   176,    -1,    -1,   179,    -1,    -1,
      -1,   278,    -1,    -1,    94,   187,    96,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1012,  1013,  1014,    -1,
      -1,  1017,  1018,  1019,    -1,  1021,  1022,  1023,  1024,  1025,
    1026,    -1,    -1,    -1,    -1,    -1,    -1,  1033,  1034,    -1,
    1036,    -1,  1038,    -1,    -1,  1041,  1042,  1043,  1044,  1045,
    1046,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,  1058,   265,    -1,  1061,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1076,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,  1117,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,    -1,   274,   275,    -1,   277,   278,    -1,
      16,    -1,   282,   283,    -1,    -1,    -1,    -1,   288,    17,
     290,    -1,    -1,    -1,   294,    23,    -1,    -1,    -1,    -1,
      28,    -1,    -1,    31,    -1,  1493,  1494,    -1,    44,    -1,
      -1,    -1,    -1,  1501,  1502,    51,  1504,  1505,    -1,    55,
      56,    -1,    58,    -1,  1512,  1513,  1514,  1515,    -1,  1517,
    1518,  1519,  1520,  1521,  1522,    -1,    -1,    -1,    74,    67,
      -1,    -1,    -1,    -1,    80,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1269,    -1,    -1,    -1,    -1,   116,    -1,
     118,   119,   120,    -1,    -1,    -1,  1282,  1283,    -1,  1285,
    1286,   137,    -1,    -1,    -1,    -1,    -1,  1293,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1313,    -1,    -1,
      -1,    -1,    -1,    -1,   170,  1321,  1322,  1323,    -1,   167,
      -1,    -1,    -1,  1329,   180,  1331,  1332,  1333,  1334,    -1,
      -1,    -1,    -1,   189,    -1,  1341,    -1,  1343,    -1,    -1,
      -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,   205,
     206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     216,   217,    -1,     6,    -1,    -1,     9,    -1,    -1,    -1,
      13,   227,   228,    -1,   230,    -1,    -1,    20,    -1,    -1,
      -1,   237,   238,    -1,   240,   241,  1392,   243,    -1,    32,
     246,   247,   248,   249,   250,    38,    39,    -1,   254,   255,
     256,   257,   258,    -1,   260,   261,   262,    -1,   264,   265,
     266,    -1,   268,    -1,    -1,    -1,    -1,    -1,  1424,    -1,
      -1,    -1,   278,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,   279,   280,   281,   282,   283,   284,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1492,    -1,    -1,    -1,
      -1,    -1,  1498,    -1,    -1,    -1,    -1,  1503,    -1,    -1,
      -1,    -1,    -1,    38,  1510,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,   181,  1545,
    1546,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1554,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    94,
     203,   204,   205,   206,  1570,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
     165,   274,   275,    -1,   277,   278,  1642,    -1,    -1,   282,
     283,     6,    -1,    -1,     9,   288,   181,   290,    13,    -1,
      -1,   294,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,
     195,   196,   197,    -1,   199,   200,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   218,   219,   220,   221,   222,   223,   224,
     225,   226,    -1,    -1,    -1,    -1,    -1,    -1,   233,   234,
      -1,    66,    -1,    -1,    69,    -1,    -1,   242,    -1,  1715,
      -1,    -1,    -1,    -1,    -1,  1721,  1722,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,  1745,
      -1,   106,    -1,    -1,  1750,   110,    -1,  1753,   283,    -1,
      -1,    -1,    -1,    -1,  1760,  1761,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,   181,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     195,   196,   197,   198,   199,   200,   201,    -1,   203,   204,
     205,   206,    -1,   208,   209,  1851,    -1,    -1,    -1,    -1,
      -1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,    -1,   230,    -1,    -1,   233,   234,
      -1,   236,   237,   238,    -1,   240,   241,   242,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,   270,   271,   272,    -1,   274,
     275,  1917,   277,   278,    -1,    -1,    -1,   282,   283,     6,
      -1,    -1,     9,   288,    -1,   290,    13,    -1,    -1,   294,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,  1954,    -1,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,    -1,
     203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,    96,
      97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,   106,
      -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,    -1,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,   163,    -1,   165,    -1,
      -1,    -1,    -1,    -1,   171,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   195,   196,
     197,   198,   199,   200,   201,    -1,   203,   204,   205,   206,
      -1,   208,   209,    -1,    -1,    -1,    -1,    -1,    -1,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,    -1,   230,    -1,    -1,   233,   234,    -1,   236,
     237,   238,    -1,   240,   241,   242,   243,    -1,    -1,   246,
     247,   248,   249,   250,    -1,    -1,    -1,   254,   255,    -1,
      -1,    -1,    -1,   260,   261,   262,    -1,   264,   265,   266,
      -1,   268,    -1,   270,   271,   272,     6,   274,   275,     9,
     277,   278,    -1,    13,    -1,   282,   283,    -1,    -1,    -1,
      20,   288,    -1,   290,    -1,    -1,    -1,   294,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    69,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    94,    -1,    96,    97,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,   106,    -1,    -1,    -1,
     110,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,   199,
     200,   201,    -1,   203,   204,   205,   206,    -1,   208,   209,
      -1,    -1,    -1,    -1,    -1,    -1,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,    -1,
     230,    -1,    -1,   233,   234,    -1,   236,   237,   238,    -1,
     240,   241,   242,   243,    -1,    -1,   246,   247,   248,   249,
     250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,    -1,
     260,   261,   262,    -1,   264,   265,   266,    -1,   268,    -1,
     270,   271,   272,     6,   274,   275,     9,   277,   278,    -1,
      13,    -1,   282,   283,    -1,    -1,    -1,    20,   288,    -1,
     290,    -1,    -1,    -1,   294,    -1,    -1,    -1,    -1,    32,
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
     278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
       6,   274,   275,     9,   277,   278,    -1,    13,    -1,   282,
     283,    -1,    -1,    -1,    20,   288,    -1,   290,    -1,    -1,
      -1,   294,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    69,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   216,   217,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
     106,    -1,    -1,    -1,   110,   237,   238,    -1,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,
      -1,    -1,   284,    -1,    -1,    -1,    -1,   163,    -1,   165,
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
     266,    -1,   268,    -1,   270,   271,   272,     6,   274,   275,
       9,   277,   278,    -1,    13,    18,   282,   283,    21,    -1,
      -1,    20,   288,    -1,   290,    -1,    -1,    -1,   294,    -1,
      -1,    34,    -1,    32,    -1,    38,    -1,    48,    41,    38,
      39,    -1,    -1,    -1,    55,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    79,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
     113,   110,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,    -1,    -1,    -1,    -1,   157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,   172,    -1,    -1,   163,   176,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,   187,    -1,    -1,    -1,
     183,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,   218,   219,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      85,   230,   235,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,   108,   265,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    13,   274,   275,    -1,   277,   278,
      -1,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,   288,
      -1,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    94,    -1,    96,    97,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,   110,   227,   228,    -1,   230,    -1,    -1,    -1,   118,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,   155,    -1,    -1,    -1,
     159,    -1,    -1,   278,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   195,   196,   197,   198,
     199,   200,   201,    -1,   203,   204,   205,   206,    -1,   208,
     209,    -1,    -1,    -1,    -1,    -1,    71,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
      -1,   230,    -1,    -1,   233,   234,    -1,   236,   237,   238,
      -1,   240,   241,   242,   243,    -1,    -1,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,   270,   271,   272,    13,   274,   275,    -1,   277,   278,
      -1,    20,    -1,   282,   283,    -1,   141,    -1,    -1,   288,
      -1,   290,    -1,    32,    -1,   294,    35,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,   176,    -1,    -1,    -1,    -1,   181,    66,    -1,    -1,
      69,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    94,    -1,    96,    97,    -1,
      -1,   216,   217,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,   110,   227,   228,    -1,   230,    -1,    -1,    -1,   118,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   278,    -1,    -1,   165,    -1,    -1,    -1,
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
      -1,   270,   271,   272,    13,   274,   275,    -1,   277,   278,
      -1,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,   288,
     155,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,    38,
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
      -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   288,    -1,    -1,   165,    -1,    -1,    -1,
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
      -1,   270,   271,   272,    13,   274,   275,    -1,   277,   278,
      -1,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,   288,
      -1,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      69,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   216,   217,    -1,    -1,    94,    -1,    96,    97,    -1,
      -1,    -1,   227,   228,    -1,   230,    -1,   106,    -1,    -1,
      -1,   110,   237,   238,    -1,   240,   241,    -1,   243,   118,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,
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
      -1,   270,   271,   272,    13,   274,   275,    -1,   277,   278,
      -1,    20,    -1,   282,   283,    -1,    -1,    -1,    -1,   288,
      -1,   290,    -1,    32,    -1,   294,    -1,    -1,    -1,    38,
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
      -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,   290,   165,    -1,    -1,    -1,
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
      -1,   270,   271,   272,   273,   274,   275,   276,   277,   278,
      13,    -1,    -1,   282,   283,    -1,    -1,    20,    -1,   288,
      -1,   290,    -1,    -1,    -1,   294,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,   118,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,
      -1,   290,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
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
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
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
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,   284,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   286,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,   196,    -1,   198,
      -1,    -1,   201,    -1,   203,   204,   205,   206,    -1,   208,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,   217,    -1,
      -1,    94,    -1,    96,    97,    -1,    -1,    -1,   227,   228,
      -1,   230,    -1,   106,    -1,    -1,    -1,   110,   237,   238,
      -1,   240,   241,    -1,   243,    -1,    -1,   246,   247,   248,
     249,   250,    -1,    -1,    -1,   254,   255,    -1,    -1,    -1,
      -1,   260,   261,   262,    -1,   264,   265,   266,    -1,   268,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    -1,   155,    -1,   283,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
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
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
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
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,   289,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   192,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      13,   274,   275,    -1,   277,   278,    -1,    20,    -1,   282,
     283,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,    32,
      -1,   294,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,    -1,
     203,   204,   205,   206,    -1,   208,   209,    -1,    -1,    -1,
      -1,    -1,    -1,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,    -1,   230,    -1,    -1,
     233,   234,    -1,   236,   237,   238,    -1,   240,   241,   242,
     243,    -1,    16,   246,   247,   248,   249,   250,    -1,    -1,
      -1,   254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,
      -1,   264,   265,   266,    -1,   268,    -1,   270,   271,   272,
      44,   274,   275,    -1,   277,   278,    -1,    51,    -1,   282,
     283,    55,    56,    -1,    58,   288,    -1,   290,    -1,    -1,
      -1,   294,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,    -1,    80,   170,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   216,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   227,   228,    -1,   230,    -1,    -1,    -1,
      -1,    -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,
     155,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,   256,   257,   258,   170,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,   204,
     205,   206,    -1,   208,    -1,    -1,    18,    -1,    -1,    21,
      -1,   216,   217,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,   227,   228,    -1,   230,    38,    -1,    -1,    41,
      -1,    -1,   237,   238,    -1,   240,   241,    -1,   243,    -1,
      -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,   254,
     255,   256,   257,   258,    -1,   260,   261,   262,    -1,   264,
     265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,   278,    -1,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,    -1,   115,    18,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    41,    -1,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,   183,    -1,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    -1,    -1,    -1,    -1,   198,    -1,    -1,   201,
      -1,   203,   204,   205,   206,    -1,   208,    -1,    -1,   113,
      -1,   115,    -1,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   227,   228,    -1,   230,    -1,
      -1,    -1,    -1,   235,    -1,   237,   238,    -1,   240,   241,
     144,   243,    -1,    -1,   246,   247,   248,   249,   250,    -1,
      -1,    -1,   254,   255,    -1,    -1,    -1,   161,   260,   261,
     262,    -1,   264,   265,   266,    -1,   268,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,    -1,    -1,   183,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   198,    -1,    -1,   201,    -1,   203,
     204,   205,   206,    -1,   208,    -1,    -1,    18,    -1,    -1,
      21,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,   228,    -1,   230,    38,    -1,    -1,
      41,   235,    -1,   237,   238,    -1,   240,   241,    -1,   243,
      -1,    -1,   246,   247,   248,   249,   250,    -1,    -1,    -1,
     254,   255,    -1,    -1,    -1,    -1,   260,   261,   262,    -1,
     264,   265,   266,    -1,   268,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,   278,    -1,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   218,   219,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,    -1,   235,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,    -1,    -1,   225,
     226,   227,   228,   229,   230,    -1,    -1,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,    -1,    -1,    -1,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278
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
{ yyval = make_node(nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 435:
{ yyval = make_node(nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 436:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], yyvsp[0], NULL); }
    break;
case 437:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-3], NULL,
					make_node(nod_def_default, (int) e_dft_count, yyvsp[-1], yyvsp[0])); }
    break;
case 438:
{ yyval = make_node(nod_mod_field_type, e_mod_fld_type_count, yyvsp[-2], NULL,
					make_node(nod_del_default, (int) 0, NULL)); }
    break;
case 439:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 481:
{ yyval = NULL; }
    break;
case 482:
{ yyval = NULL; }
    break;
case 483:
{ yyval = NULL; }
    break;
case 484:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
    break;
case 485:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
    break;
case 486:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 487:
{ yyval = make_node (nod_cascade, 0, NULL); }
    break;
case 488:
{ yyval = make_node (nod_restrict, 0, NULL); }
    break;
case 489:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
    break;
case 490:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
    break;
case 491:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 492:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 493:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 494:
{ yyval = make_node(nod_mod_udf, e_mod_udf_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 495:
{ yyval = yyvsp[0]; }
    break;
case 496:
{ yyval = NULL; }
    break;
case 497:
{ yyval = yyvsp[0]; }
    break;
case 498:
{ yyval = NULL; }
    break;
case 499:
{ yyval = NULL; }
    break;
case 501:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 502:
{ yyval = yyvsp[0]; }
    break;
case 503:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
    break;
case 504:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
    break;
case 505:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
    break;
case 506:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
    break;
case 507:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 509:
{ yyval = NULL; }
    break;
case 511:
{ yyval = NULL; }
    break;
case 512:
{ yyval = yyvsp[0]; }
    break;
case 513:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
    break;
case 514:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
    break;
case 515:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
    break;
case 516:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
    break;
case 517:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
    break;
case 518:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
    break;
case 519:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
    break;
case 520:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
    break;
case 521:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
    break;
case 522:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
    break;
case 523:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
    break;
case 524:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 525:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
    break;
case 530:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
    break;
case 531:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
    break;
case 533:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 534:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
    break;
case 535:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 541:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
    break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
    break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
    break;
case 544:
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
case 545:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
    break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
    break;
case 549:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
    break;
case 550:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
    break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
    break;
case 553:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
    break;
case 554:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
    break;
case 555:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
    break;
case 556:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
    break;
case 557:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
    break;
case 558:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
    break;
case 560:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 561:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 562:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
    break;
case 563:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 564:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
    break;
case 565:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
    break;
case 574:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
    break;
case 575:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
    break;
case 576:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
    break;
case 577:
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
case 578:
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
case 581:
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
case 582:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 583:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
    break;
case 584:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
    break;
case 585:
{ yyval = yyvsp[-1]; }
    break;
case 586:
{ yyval = 0; }
    break;
case 590:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
    break;
case 591:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
    break;
case 592:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
    break;
case 596:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
    break;
case 597:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 598:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 599:
{ yyval = 0; }
    break;
case 600:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
    break;
case 603:
{ yyval = make_node (nod_commit, e_commit_count, yyvsp[0]); }
    break;
case 604:
{ yyval = make_node (nod_rollback, e_rollback_count, yyvsp[0]); }
    break;
case 607:
{ yyval = make_node (nod_retain, 0, NULL); }
    break;
case 608:
{ yyval = NULL; }
    break;
case 610:
{ yyval = NULL; }
    break;
case 611:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
    break;
case 613:
{ yyval = NULL; }
    break;
case 615:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
    break;
case 620:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
    break;
case 621:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
    break;
case 622:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
    break;
case 623:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
    break;
case 624:
{ yyval = yyvsp[0];}
    break;
case 626:
{ yyval = yyvsp[0];}
    break;
case 627:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 628:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
    break;
case 629:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
    break;
case 630:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 631:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
    break;
case 632:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
    break;
case 633:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
    break;
case 634:
{ yyval = 0; }
    break;
case 635:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
    break;
case 636:
{ yyval = (dsql_nod*) NOD_SHARED; }
    break;
case 637:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
    break;
case 638:
{ yyval = (dsql_nod*) 0; }
    break;
case 639:
{ yyval = (dsql_nod*) NOD_READ; }
    break;
case 640:
{ yyval = (dsql_nod*) NOD_WRITE; }
    break;
case 642:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 643:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 644:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
    break;
case 645:
{ yyval = 0; }
    break;
case 647:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
    break;
case 648:
{ yyval = make_node (nod_set_statistics, (int) e_stat_count, yyvsp[0]); }
    break;
case 649:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-2], NULL, NULL, yyvsp[0]); }
    break;
case 650:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-3], yyvsp[-2], NULL, yyvsp[0]); }
    break;
case 651:
{ yyval = make_node(nod_comment, e_comment_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[0]); }
    break;
case 652:
{ yyval = MAKE_constant((dsql_str*) ddl_database, CONSTANT_SLONG); }
    break;
case 653:
{ yyval = MAKE_constant((dsql_str*) ddl_domain, CONSTANT_SLONG); }
    break;
case 654:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 655:
{ yyval = MAKE_constant((dsql_str*) ddl_view, CONSTANT_SLONG); }
    break;
case 656:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 657:
{ yyval = MAKE_constant((dsql_str*) ddl_trigger, CONSTANT_SLONG); }
    break;
case 658:
{ yyval = MAKE_constant((dsql_str*) ddl_udf, CONSTANT_SLONG); }
    break;
case 659:
{ yyval = MAKE_constant((dsql_str*) ddl_blob_filter, CONSTANT_SLONG); }
    break;
case 660:
{ yyval = MAKE_constant((dsql_str*) ddl_exception, CONSTANT_SLONG); }
    break;
case 661:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 662:
{ yyval = MAKE_constant((dsql_str*) ddl_generator, CONSTANT_SLONG); }
    break;
case 663:
{ yyval = MAKE_constant((dsql_str*) ddl_index, CONSTANT_SLONG); }
    break;
case 664:
{ yyval = MAKE_constant((dsql_str*) ddl_role, CONSTANT_SLONG); }
    break;
case 665:
{ yyval = MAKE_constant((dsql_str*) ddl_charset, CONSTANT_SLONG); }
    break;
case 666:
{ yyval = MAKE_constant((dsql_str*) ddl_collation, CONSTANT_SLONG); }
    break;
case 667:
{ yyval = MAKE_constant((dsql_str*) ddl_relation, CONSTANT_SLONG); }
    break;
case 668:
{ yyval = MAKE_constant((dsql_str*) ddl_procedure, CONSTANT_SLONG); }
    break;
case 669:
{ yyval = yyvsp[0]; }
    break;
case 671:
{ yyval = NULL; }
    break;
case 672:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 673:
{ yyval = make_node (nod_for_update, (int) e_fpd_count, yyvsp[0]); }
    break;
case 674:
{ yyval = NULL; }
    break;
case 675:
{ yyval = yyvsp[0]; }
    break;
case 676:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 677:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 678:
{ yyval = NULL; }
    break;
case 679:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 680:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 681:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 683:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 684:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 686:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 687:
{ lex.limit_clause = true; }
    break;
case 688:
{ lex.limit_clause = false; }
    break;
case 689:
{ lex.first_detection = true; }
    break;
case 690:
{ lex.first_detection = false; }
    break;
case 691:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
    break;
case 692:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
    break;
case 693:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
    break;
case 694:
{ yyval = 0; }
    break;
case 695:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
    break;
case 696:
{ yyval = yyvsp[-2]; }
    break;
case 697:
{ yyval = yyvsp[-1]; }
    break;
case 698:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 699:
{ yyval = yyvsp[-1]; }
    break;
case 700:
{ yyval = yyvsp[0]; }
    break;
case 701:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 702:
{ yyval = 0; }
    break;
case 703:
{ yyval = make_list (yyvsp[0]); }
    break;
case 704:
{ yyval = 0; }
    break;
case 706:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 708:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 711:
{ yyval = make_list (yyvsp[0]); }
    break;
case 713:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 718:
{ yyval = yyvsp[-1]; }
    break;
case 719:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
    break;
case 721:
{ yyval = NULL; }
    break;
case 722:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 723:
{ yyval = NULL; }
    break;
case 725:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 729:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
    break;
case 730:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
    break;
case 731:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
    break;
case 734:
{ yyval = yyvsp[0]; }
    break;
case 735:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 736:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
    break;
case 737:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
    break;
case 738:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 739:
{ yyval = NULL; }
    break;
case 741:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
    break;
case 742:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
    break;
case 743:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 744:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
    break;
case 745:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
    break;
case 746:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
    break;
case 747:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
    break;
case 750:
{ yyval = make_list (yyvsp[0]); }
    break;
case 751:
{ yyval = NULL; }
    break;
case 753:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
    break;
case 760:
{ yyval = NULL; }
    break;
case 761:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 762:
{ yyval = 0; }
    break;
case 763:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 764:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
    break;
case 765:
{ yyval = 0; }
    break;
case 766:
{ yyval = 0; }
    break;
case 768:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 769:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
    break;
case 772:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 773:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
    break;
case 774:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
    break;
case 775:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
    break;
case 777:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 778:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 779:
{ yyval = 0; }
    break;
case 780:
{ yyval = make_list (yyvsp[0]); }
    break;
case 781:
{ yyval = 0; }
    break;
case 783:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 784:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 785:
{ yyval = 0; }
    break;
case 786:
{ yyval = make_node (nod_flag, 0, NULL); }
    break;
case 787:
{ yyval = 0; }
    break;
case 788:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
    break;
case 789:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
    break;
case 790:
{ yyval = yyvsp[-1]; }
    break;
case 791:
{ yyval = 0; }
    break;
case 792:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
    break;
case 793:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
    break;
case 794:
{ yyval = NULL; }
    break;
case 795:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
				yyvsp[-6], make_list (yyvsp[-5]), make_list (yyvsp[-2]), NULL, yyvsp[0]); }
    break;
case 796:
{ yyval = make_node (nod_insert, (int) e_ins_count,
				yyvsp[-2], yyvsp[-1], NULL, yyvsp[0], NULL); }
    break;
case 799:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 800:
{ yyval = make_node (nod_delete, (int) e_del_count,
				yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 803:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
    break;
case 804:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
    break;
case 805:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[0]), NULL); }
    break;
case 806:
{ yyval = make_node (nod_returning, (int) e_ret_count,
					make_list (yyvsp[-2]), make_list (yyvsp[0])); }
    break;
case 807:
{ yyval = NULL; }
    break;
case 808:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
    break;
case 810:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 811:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
    break;
case 812:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
    break;
case 813:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 814:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
    break;
case 815:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 816:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
    break;
case 817:
{ yyval = NULL; }
    break;
case 820:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 821:
{ yyval = yyvsp[0]; }
    break;
case 822:
{ yyval = NULL; }
    break;
case 823:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
    break;
case 832:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 834:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 836:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 837:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
    break;
case 838:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
    break;
case 840:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
    break;
case 842:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 844:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 845:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 847:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 848:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 849:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 850:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 851:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 853:
{ yyval = yyvsp[-1]; }
    break;
case 854:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
    break;
case 866:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 867:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 868:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 869:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 870:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 871:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 872:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 873:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 874:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 875:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 876:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 877:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 878:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 879:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 880:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 881:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 882:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 883:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 884:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 885:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 886:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 887:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 888:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 889:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
    break;
case 892:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 893:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
    break;
case 894:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 895:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 896:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 897:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 898:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
    break;
case 899:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
    break;
case 900:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 901:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 902:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 903:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 904:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 905:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
    break;
case 906:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
    break;
case 907:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
    break;
case 908:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
    break;
case 909:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
    break;
case 910:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
    break;
case 911:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
    break;
case 912:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 913:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 914:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 915:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
    break;
case 916:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
    break;
case 917:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
    break;
case 919:
{ yyval = make_list (yyvsp[-1]); }
    break;
case 920:
{ yyval = yyvsp[-1]; }
    break;
case 931:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 932:
{ yyval = yyvsp[0]; }
    break;
case 933:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 934:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 935:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
    break;
case 936:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 937:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 938:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
    break;
case 939:
{ yyval = yyvsp[-1]; }
    break;
case 940:
{ yyval = yyvsp[-1]; }
    break;
case 944:
{ yyval = make_node (nod_dbkey, 1, NULL); }
    break;
case 945:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
    break;
case 946:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
    break;
case 949:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
    break;
case 950:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 0, NULL);
			}
    break;
case 951:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
    break;
case 952:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
    break;
case 954:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 956:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
    break;
case 957:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
    break;
case 958:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
    break;
case 959:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
    break;
case 960:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 961:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
    break;
case 963:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
    break;
case 964:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
			}
    break;
case 965:
{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, (SLONG) client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, (SLONG) db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
    break;
case 966:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
    break;
case 967:
{ yyval = make_parameter (); }
    break;
case 968:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 969:
{ yyval = make_node (nod_user_name, 0, NULL); }
    break;
case 970:
{ yyval = make_node (nod_current_role, 0, NULL); }
    break;
case 971:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
    break;
case 972:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
    break;
case 973:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
    break;
case 974:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
    break;
case 975:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
    break;
case 976:
{ yyval = yyvsp[0]; }
    break;
case 977:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
    break;
case 979:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 980:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 981:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
    break;
case 982:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
    break;
case 983:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
    break;
case 985:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
    break;
case 986:
{ yyval = yyvsp[0];}
    break;
case 990:
{ yyval = make_node (nod_agg_count, 0, NULL); }
    break;
case 991:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
    break;
case 992:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
    break;
case 993:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
    break;
case 994:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 995:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
    break;
case 996:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
    break;
case 997:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 998:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
    break;
case 999:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 1000:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
    break;
case 1003:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
    break;
case 1007:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_bit, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1008:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1009:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_char, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1010:
{ yyval = make_node(nod_strlen, (int) e_strlen_count,
					MAKE_constant((dsql_str*)blr_strlen_octet, CONSTANT_SLONG), yyvsp[-1]); }
    break;
case 1013:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
    break;
case 1014:
{ yyval = make_node (nod_lowcase, 1, yyvsp[-1]); }
    break;
case 1015:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
    break;
case 1016:
{ yyval = yyvsp[0]; }
    break;
case 1017:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
    break;
case 1018:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-4], yyvsp[-3], yyvsp[-1]); }
    break;
case 1019:
{ yyval = make_node (nod_trim, (int) e_trim_count, 
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), yyvsp[-3], yyvsp[-1]); }
    break;
case 1020:
{ yyval = make_node (nod_trim, (int) e_trim_count, yyvsp[-3], NULL, yyvsp[-1]); }
    break;
case 1021:
{ yyval = make_node (nod_trim, (int) e_trim_count,
				MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG), NULL, yyvsp[-1]); }
    break;
case 1022:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_both, CONSTANT_SLONG); }
    break;
case 1023:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_trailing, CONSTANT_SLONG); }
    break;
case 1024:
{ yyval = MAKE_constant ((dsql_str*)blr_trim_leading, CONSTANT_SLONG); }
    break;
case 1025:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1026:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
    break;
case 1027:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
    break;
case 1030:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
    break;
case 1031:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
    break;
case 1032:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
    break;
case 1035:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1036:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1037:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1038:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1039:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
    break;
case 1040:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
    break;
case 1041:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
    break;
case 1042:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
    break;
case 1046:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[0],
						MAKE_constant((dsql_str*) 1, CONSTANT_SLONG));
			}
    break;
case 1047:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
    break;
case 1048:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
    break;
case 1049:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
    break;
case 1050:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
    break;
case 1051:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
    break;
case 1052:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
    break;
case 1053:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
    break;
case 1054:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
    break;
case 1055:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
    break;
case 1060:
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
			isc_arg_number, (SLONG) lex.lines,
			isc_arg_number, (SLONG) (lex.last_token - lex.line_start + 1),
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
